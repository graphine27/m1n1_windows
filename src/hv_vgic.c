/**
 * hv_vgic.c
 * @author amarioguy (Arminder Singh)
 * 
 * Virtual Generic Interrupt Controller implementation for m1n1, to aid in running non open source operating systems.
 * 
 * Enables CPU interface before guest OS boot, sets up emulated distributor/redistributor regions
 * 
 * @version 1.0
 * @date 2022-08-29 (refactored from original code)
 * 
 * @copyright Copyright (c) amarioguy (Arminder Singh), 2022.
 * 
 * SPDX-License-Identifier: MIT
 * 
 */



#include "hv.h"
#include "hv_vgic.h"
#include "assert.h"
#include "cpu_regs.h"
#include "display.h"
#include "memory.h"
#include "pcie.h"
#include "smp.h"
#include "string.h"
#include "usb.h"
#include "utils.h"
#include "aic.h"
#include "malloc.h"
#include "heapblock.h"
#include "smp.h"
#include "string.h"
#include "types.h"
#include "uartproxy.h"

/**
 * General idea of how this should work:
 * 
 * Apple Silicon chips since the M1 implement the GIC CPU interface registers in hardware, meaning
 * only the distributor, the core specific redistributors, and (potentially) an ITS need to be emulated by m1n1.
 * 
 * As such, this file implements most of the code needed to make this possible. The emulated distributor/redistributors
 * will need to meet a few constraints (namely it's limited by what the GIC CPU interface supports)
 * 
 * Apple's vGIC CPU interface has the following characteristics (on M1 and M2):
 * - 32 levels of virtual priority and preemption priority (5 preemption/priority bits)
 * - 16 bits of virtual interrupt ID bits (meaning up to 65535 interrupts are supported theoretically, however practically limited by the number of IRQs the AIC supports)
 * - supports guest-generated SEIs upon writing to GIC registers in a bad way 
 *   (note that an errata here exists on pre-M3 SoCs that can result in a host SError - we implement special handling for this.)
 * - 3 level affinity (aff2/aff1/aff0 valid, aff3 invalid/reserved as 0)
 * - legacy operation is not supported (ICC_SRE_EL2.SRE is reserved, set to 1) (no GICv2 operations)
 * - TDIR bit is supported (FEAT_GICv3_TDIR)
 * - extended SPI and PPI ranges are *not* supported on M1/M2 (and their Pro counterparts, even if the SoC itself has > 16 cores)
 * - 8 list registers
 * - direct injection of virtual interrupts are not supported (not a GICv4, and by extension, no NMIs supported)
 * - IRQ/FIQ bypass are not supported
 * 
 * 
 * The mappings are different for platforms with 36-bit vs 42-bit physical addressing, with 36-bit platforms tentatively
 * having the distributor being mapped to 0xF00000000, redistributors at offset +0x10000000
 * and 42-bit platforms having the distributor at 0x5000000000, redistributors at offset +0x100000000
 * 
 * A major note about processor affinities: since AICv2 platforms don't support setting core affinities easily,
 * the tentative solution is to do routing to any virtual CPU once we receive an IRQ, we can't assume
 * that the core that got the IRQ is the one that needs to be signaled. (for FIQs, because they're core specific,
 * we'll know which core needs to be signaled in those cases.)
 * 
 */

#define DIST_BASE_36_BIT 0xF00000000
#define REDIST_BASE_36_BIT 0xF10000000
#define DIST_BASE_42_BIT 0x5000000000
#define REDIST_BASE_42_BIT 0x5100000000
//
// This is tentative - depends on if direct MSIs or ITS translated IRQs end up being easier to implement.
//
#define ITS_BASE_36_BIT 0xF20000000
#define ITS_BASE_42_BIT 0x5200000000


vgicv3_dist *distributor;
vgicv3_vcpu_redist *redistributors;
vgicv3_its *interrupt_translation_service;
static u64 dist_base, redist_base, its_base;
static u16 num_cpus;
static bool vgic_inited;




//
// Description:
//   the vGIC guest interrupt handler for distributor writes.
//
// Return values:
//   true - access has been handled successfully, even if the access itself is either bad or not permitted.
//   false - access was not handled successfully.
//
static bool handle_vgic_dist_access(struct exc_info *ctx, u64 addr, u64 *val, bool write, int width)
{
    u64 relative_addr;
    bool register_handled;
    relative_addr = addr - dist_base;
    register_handled = false;
    if(write) {
        //
        // The guest attempted to write a register.
        // Handle it based on what they're trying to write, and preserve the value if
        // the value is going to a RW register.
        // Emit a warning (to become an error later) if the guest is attempting to write a register that doesn't exist or is read only.
        //

        //
        // This switch statement covers all the cases that don't involve a range of registers.
        //
        switch(relative_addr) {
            case GIC_DIST_CTLR:
                //
                // GICD_CTLR has fields that we cannot change (due to the underlying physical environment or constraints)
                // and fields we can change, so check for RO fields here first.
                //
                u32 gicd_ctlr_new_val = (u32)(*val);
                printf("HV vGIC DEBUG: guest writing GICD_CTLR = 0x%x, old value 0x%x\n", gicd_ctlr_new_val, distributor->gicd_ctl_reg);
                bool is_rwp_to_be_set = false;
                if(((gicd_ctlr_new_val & GENMASK(30, 8)) != 0) || ((gicd_ctlr_new_val & BIT(5)) != 0) || ((gicd_ctlr_new_val & GENMASK(3, 2)) != 0)) {
                    //
                    // these bits are RES0 - clear out this bitmask.
                    //
                    gicd_ctlr_new_val &= ~(GENMASK(30, 8));
                    gicd_ctlr_new_val &= ~(GENMASK(3, 2));
                    gicd_ctlr_new_val &= ~(BIT(5));
                    printf("HV vGIC DEBUG [WARN]: guest attempted to write RES0 bits, discarding\n");
                }
                
                if((gicd_ctlr_new_val & BIT(6)) == 0) {
                    //
                    // the guest is trying to set DS = 0. we do not support this so ensure that bit 6 is always written,
                    // however we need to emit a warning because this means that our GIC configuration is wrong.
                    //
                    gicd_ctlr_new_val |= BIT(6);
                    printf("HV vGIC DEBUG [WARN]: guest attempted to set DS = 0, discarding\n");
                }
                if((gicd_ctlr_new_val & BIT(4)) == 0) {
                    //
                    // the guest is trying to set ARE = 0. we do not support this so ensure that bit 4 is always written,
                    // however we need to emit a warning because this means that our GIC configuration is wrong.
                    //
                    gicd_ctlr_new_val |= BIT(4);
                    printf("HV vGIC DEBUG [WARN]: guest attempted to set ARE = 0, discarding\n");
                }
                if((((gicd_ctlr_new_val & BIT(7)) != 0) && ((distributor->gicd_ctl_reg & BIT(7)) == 0)) 
                || (((gicd_ctlr_new_val & BIT(7)) == 0) && ((distributor->gicd_ctl_reg & BIT(7)) != 0))) {
                    //
                    // the guest is trying to set EN1WF either way. we need to know about any attempt to change this, as it affects IRQ behavior.
                    // we also need to flag that RWP needs to be set to 1.
                    //
                    is_rwp_to_be_set = true;
                    printf("HV vGIC DEBUG [INFO]: guest is changing EN1WF\n");
                }
                if(((gicd_ctlr_new_val & BIT(1)) == 0) && ((distributor->gicd_ctl_reg & BIT(1)) != 0)) {
                    //
                    // the guest is trying to set EnableGrp1 = 0. we need to know about any attempt to set this, as it affects IRQ behavior.
                    // we also need to flag that RWP needs to be set to 1.
                    //
                    is_rwp_to_be_set = true;
                    printf("HV vGIC DEBUG [INFO]: guest is setting EnableGrp1 = 0\n");
                }
                if(((gicd_ctlr_new_val & BIT(0)) == 0) && ((distributor->gicd_ctl_reg & BIT(0)) != 0)) {
                    //
                    // the guest is trying to set EnableGrp1 = 0. we need to know about any attempt to set this, as it affects IRQ behavior.
                    // we also need to flag that RWP needs to be set to 1.
                    //
                    is_rwp_to_be_set = true;
                    printf("HV vGIC DEBUG [INFO]: guest is setting EnableGrp0 = 0\n");
                }

                //
                // RWP (Register Write Pending bit) - this bit is a tad bit special - it's RO, but it has to be set if bits 0 or 1 are transitioning
                // from 1 to 0.
                //
                // if(is_rwp_to_be_set == true) {
                //     //
                //     // 
                // }

                distributor->gicd_ctl_reg = gicd_ctlr_new_val;
                break;
            default:
                //
                // we're dealing with a register that is banked n times, we need to get to the if statements.
                //
                break;
        }

        //
        // Fair warning this code is probably dicey...
        //
        // if((register_handled == false) && )
    }
    else {
        //
        // The guest is attempting to read a register.
        // Handle it appropriately. Emit a warning (to become an error later) if a register is write only or doesn't exist
        //
        switch(relative_addr) {
            
            default:
        }
    }
    return true;
}



//
// Description:
//   the vGIC guest interrupt handler for redistributor writes.
//
// Return values:
//   true - access has been handled successfully, even if the access itself is either bad or not permitted.
//   false - access was not handled successfully.
//
static bool handle_vgic_redist_access(struct exc_info *ctx, u64 addr, u64 *val, bool write, int width)
{
    if(write) {
        //
        // The guest attempted to write a register.
        // Handle it based on what they're trying to write, and preserve the value if
        // the value is going to a RW register.
        // Emit a warning (to become an error later) if the guest is attempting to write a register that doesn't exist or is read only.
        //
    }
    else {
        //
        // The guest is attempting to read a register.
        // Handle it appropriately. Emit a warning (to become an error later) if a register is write only or doesn't exist
        //
    }
    return true;
}


/**
 * @brief hv_vgicv3_init
 * 
 * Initializes the vGIC and prepares it for use by the guest OS.
 * 
 * Note that this function is only expected to be called once and subsequent calls will have undefined behavior
 * 
 * @return 
 * 
 * 0 - success, vGIC is ready for use by the guest
 * -1 - an error has occurred during vGIC initialization, refer to m1n1 output log for details on the specific error 
 */

int hv_vgicv3_init(void)
{
    printf("HV vGIC DEBUG: start\n");
    vgic_inited = false;
    //
    // First things first - set the parameters appropriately based on whether
    // we're running on a 36-bit or 42-bit platform.
    // Also for now, we are catering to "lowest common denominator" for all chips,
    // so on more powerful systems we may not be using all cores.
    //
    switch(chip_id) {
        case T8103:
        case T8112:
            dist_base = DIST_BASE_36_BIT;
            redist_base = REDIST_BASE_36_BIT;
            its_base = ITS_BASE_36_BIT;
            num_cpus = 8;
            break;
        // case T8010:
        // case T8015:
        // case T8011:
        // case T8012:
        //     dist_base = DIST_BASE_36_BIT;
        //     redist_base = REDIST_BASE_36_BIT;
        //     its_base = ITS_BASE_36_BIT;
        //     break;
        case T6000:
            dist_base = DIST_BASE_42_BIT;
            redist_base = REDIST_BASE_42_BIT;
            its_base = ITS_BASE_42_BIT;
            num_cpus = 8; // cannot assume that we have 10 cores for M1 Pro
        case T6001:
            dist_base = DIST_BASE_42_BIT;
            redist_base = REDIST_BASE_42_BIT;
            its_base = ITS_BASE_42_BIT;
            num_cpus = 10;
        case T6002:
            dist_base = DIST_BASE_42_BIT;
            redist_base = REDIST_BASE_42_BIT;
            its_base = ITS_BASE_42_BIT;
            num_cpus = 20;
        case T6020:
            dist_base = DIST_BASE_42_BIT;
            redist_base = REDIST_BASE_42_BIT;
            its_base = ITS_BASE_42_BIT;
            num_cpus = 10;
        case T6021:
            dist_base = DIST_BASE_42_BIT;
            redist_base = REDIST_BASE_42_BIT;
            its_base = ITS_BASE_42_BIT;
            num_cpus = 12;
        case T6022:
            dist_base = DIST_BASE_42_BIT;
            redist_base = REDIST_BASE_42_BIT;
            its_base = ITS_BASE_42_BIT;
            num_cpus = 24;
        // case T6030:
        // case T6031:
        // case 0x6032:
        // case T6034:
        // case 0x6040:
        // case 0x6041:
        // case T8122:
        //     dist_base = DIST_BASE_42_BIT;
        //     redist_base = REDIST_BASE_42_BIT;
        //     its_base = ITS_BASE_42_BIT;
        //     break;
    }
    //
    // Step 1 - distributor setup.
    //
    printf("HV vGIC DEBUG: setting up distributor\n");
    distributor = heapblock_alloc(sizeof(vgicv3_dist));
    hv_vgicv3_init_dist_registers();
    //
    // Map the vGIC distributor into unoccupied MMIO space.
    //
    printf("HV vGIC DEBUG: mapping distributor into guest space\n");
    hv_map_hook(dist_base, handle_vgic_dist_access, sizeof(vgicv3_dist));


    /* Redistributor setup */
    printf("HV vGIC DEBUG: setting up redistributors\n");
    redistributors = heapblock_alloc(sizeof(vgicv3_vcpu_redist) * num_cpus);
    hv_vgicv3_init_redist_registers();
    printf("HV vGIC DEBUG: mapping redistributors into guest space\n");
    hv_map_hook(redist_base, handle_vgic_redist_access, ((sizeof(vgicv3_vcpu_redist)) * num_cpus));

    //
    // ITS setup (for MSIs - PCIe devices usually signal via these.)
    // Disabled for now, seems like direct injection into the guest is easier.
    //

    // hv_map_hook(its_base, handle_vgic_access, sizeof(vgicv3_its));

    //vGIC setup is successful.
    vgic_inited = true;
    return 0;
}

/**
 * @brief hv_vgicv3_init_dist_registers
 * 
 * Sets up the initial values for the distributor registers.
 * 
 * For registers that deal with unsupported features, set them to 0 and just never interact with them
 * 
 * For write only registers, set them to 0, and emulate the effect upon attempting to write that register.
 * Read-only registers, set their value here and don't let the guest touch their values.
 * 
 */
void hv_vgicv3_init_dist_registers(void)
{
    memset(distributor, 0, sizeof(vgicv3_dist));
    //
    // For now - taking the easy route of saying that at least 1024 IRQs are supported on all platforms.
    //
    distributor->gicd_ctl_reg = (BIT(6) | BIT(4) | BIT(1) | BIT(0));
    //
    // GIC type will be defined as the following:
    // - No extended SPIs
    // - Affinity level 0 can go up to 15
    // - 1 of N SPI interrupts are supported (kind of how AIC2 can behave?)
    // - Affinity 3 invalid
    // - 16 interrupt ID bits (to match what the CPU interface supports)
    // - LPIs/MSIs supported
    // - MSIs not supported by distributor registers (using an ITS here)
    //
    distributor->gicd_type_reg = (BIT(22) | BIT(21) | BIT(20) | BIT(19) | BIT(17) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0));
    distributor->gicd_imp_id_reg = (BIT(10) | BIT(5) | BIT(4) | BIT(3) | BIT(1) | BIT(0));
    distributor->gicd_type_reg_2 = 0; 
    distributor->gicd_err_sts = 0;
    return;

}

void hv_vgicv3_assign_redist_affinity_value(u16 cpu_num, bool last_cpu) {
    u32 cpu_affinity_value;
    uint64_t mpidr_val;
    uint64_t gicr_typer;
    mpidr_val = smp_get_mpidr(cpu_num);
    //
    // Affinity level 3 is always 0.
    //
    cpu_affinity_value = (0 << 24);
    //
    // Affinity level 2 signifies if we're targeting a P-core or E-core cluster.
    // (0x0 for an E-core, 0x1 for a P-core)
    //
    cpu_affinity_value |= ((mpidr_val >> 16) & 0xFF);
    //
    // Affinity level 1 signifies the cluster number on the local die (for multi-die systems it's cluster_num + (die_num * 8)).
    //
    cpu_affinity_value |= ((mpidr_val >> 8) & 0xFF);
    //
    // Affinity level 0 is the core number on the local cluster.
    //
    cpu_affinity_value |= ((mpidr_val) & 0xFF);
    gicr_typer = (((uint64_t)cpu_affinity_value) << 32);
    //
    // Apple silicon platforms (at least the M1 and M2 and the Pro counterparts) do not support the extended PPI/SPI ranges
    // so bits 31:27 remain 0. If M3 or M4 do support the extended ranges, check the Chip ID here and toggle those bits
    //
    // We're also sharing a common LPI configuration table across all the vCPUs.
    //
    // Put the processor/CPU number in the right field here, 
    // the way we're doing it here ensures we have a one to one mapping with how m1n1 identifies the CPUs.
    //
    gicr_typer |= (cpu_num << 8);

    //
    // Leave out MPAM support for now - we can't assume the CPU supports it.
    // Let processors opt out of interrupts though. (bit 5, GICR_TYPER.DPGS bit)
    //
    gicr_typer |= BIT(5);
    if(last_cpu == true) {
        //
        // this is the last redistributor, set bit 4 to indicate this.
        //
        gicr_typer |= BIT(4);
    }

    //
    // If we need an ITS, we need to comment out the bottom line, since we wouldn't be supporting direct LPI injection to
    // redistributors.
    //
    gicr_typer |= BIT(3);
    //
    // say that we have physical LPIs to be safe.
    //
    gicr_typer |= BIT(0);

    //
    // write back this feature set to our vGIC registers.
    //
    redistributors[cpu_num].rd_region.gicr_type_reg = gicr_typer;

    return;

}

void hv_vgicv3_init_redist_registers() {
    memset(redistributors, 0, (sizeof(vgicv3_vcpu_redist) * num_cpus));
    for(u16 i = 0; i < num_cpus; i++) {
        bool last_cpu = (i + 1 == num_cpus) ? true : false;
        redistributors[i].rd_region.gicr_ctl_reg = (BIT(2) | BIT(1));
        redistributors[i].rd_region.gicr_iidr = (BIT(10) | BIT(5) | BIT(4) | BIT(3) | BIT(1) | BIT(0));
        //
        // assign affinity values to redistributors.
        //
        hv_vgicv3_assign_redist_affinity_value(i, last_cpu);
        redistributors[i].rd_region.gicr_status_reg = 0;
        redistributors[i].rd_region.gicr_wake_reg = (BIT(2) | BIT(1)); //GICR_WAKER reset values, currently not using bits 31 or 0.
        //
        // Generate and set the LPI configuration table here.
        // (Right now this is ignored just to test if stuff is working since we have no MSIs and LPIs are disabled right now.)
        //
        

    }
}


/**
 * @brief hv_vgicv3_init_list_registers
 * 
 * Enables the platform's list registers for use by the guest OS.
 * 
 * @param n - the number of the list register to be turned on
 */
void hv_vgicv3_init_list_registers(int n)
{
    switch(n)
        {
            case 0:
                msr(ICH_LR0_EL2, 0);
            case 1:
                msr(ICH_LR1_EL2, 0);
            case 2:
                msr(ICH_LR2_EL2, 0);
            case 3:
                msr(ICH_LR3_EL2, 0);
            case 4:
                msr(ICH_LR4_EL2, 0);
            case 5:
                msr(ICH_LR5_EL2, 0);
            case 6:
                msr(ICH_LR6_EL2, 0);
            case 7:
                msr(ICH_LR7_EL2, 0);
        }
}


/**
 * @brief hv_vgicv3_enable_virtual_interrupts
 * 
 * Enables virtual interrupts for the guest.
 * 
 * Note that actual interrupts are always handled by m1n1, then passed onto the vGIC which will signal the virtual interrupt to the OS.
 * 
 * @return
 * 0 - success
 * -1 - there was an error.
 */

int hv_vgicv3_enable_virtual_interrupts(void)
{
    //set VMCR to reset values, then enable virtual group 0 and 1 interrupts
    msr(ICH_VMCR_EL2, 0);
    msr(ICH_VMCR_EL2, (BIT(1)));
    //bit 0 enables the virtual CPU interface registers
    //AMO/IMO/FMO set by m1n1 on boot
    msr(ICH_HCR_EL2, (BIT(0)));


    return 0;
}