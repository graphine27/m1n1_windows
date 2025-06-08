/* SPDX-License-Identifier: MIT */

#ifndef HV_VGIC_H
#define HV_VGIC_H

//========
// Offsets
//========

//
// Distributor offsets
//
#define GIC_DIST_CTLR 0x0
#define GIC_DIST_TYPER 0x4
#define GIC_DIST_IIDR 0x8
#define GIC_DIST_TYPER2 0xC
#define GIC_DIST_STATUSR 0x10
#define GIC_DIST_SETSPI_NSR 0x40
#define GIC_DIST_CLRSPI_NSR 0x48
#define GIC_DIST_SETSPI_SR 0x50
#define GIC_DIST_CLRSPI_SR 0x58
#define GIC_DIST_IGROUPR0 0x80
// #define GIC_DIST_IGROUPR1 0x84
// #define GIC_DIST_IGROUPR2 0x88
// #define GIC_DIST_IGROUPR3 0x8C
// #define GIC_DIST_IGROUPR4 0x90
// #define GIC_DIST_IGROUPR5 0x94
// #define GIC_DIST_IGROUPR6 0x98
// #define GIC_DIST_IGROUPR7 0x9C
// #define GIC_DIST_IGROUPR8 0xA0
// #define GIC_DIST_IGROUPR9 0xA4
// #define GIC_DIST_IGROUPR10 0xA8
// #define GIC_DIST_IGROUPR11 0xAC
// #define GIC_DIST_IGROUPR12 0xB0
// #define GIC_DIST_IGROUPR13 0xB4
// #define GIC_DIST_IGROUPR14 0xB8
// #define GIC_DIST_IGROUPR15 0xBC
// #define GIC_DIST_IGROUPR16 0xC0
// #define GIC_DIST_IGROUPR17 0xC4
// #define GIC_DIST_IGROUPR18 0xC8
// #define GIC_DIST_IGROUPR19 0xCC
// #define GIC_DIST_IGROUPR20 0xD0
// #define GIC_DIST_IGROUPR21 0xD4
// #define GIC_DIST_IGROUPR22 0xD8
// #define GIC_DIST_IGROUPR23 0xDC
// #define GIC_DIST_IGROUPR24 0xE0
// #define GIC_DIST_IGROUPR25 0xE4
// #define GIC_DIST_IGROUPR26 0xE8
// #define GIC_DIST_IGROUPR27 0xEC
// #define GIC_DIST_IGROUPR28 0xF0
// #define GIC_DIST_IGROUPR29 0xF4
// #define GIC_DIST_IGROUPR30 0xF8
#define GIC_DIST_IGROUPR31 0xFC

#define GIC_DIST_ISENABLER0 0x100
// #define GIC_DIST_ISENABLER1 0x104
// #define GIC_DIST_ISENABLER2 0x108
// #define GIC_DIST_ISENABLER3 0x10C
// #define GIC_DIST_ISENABLER4 0x110
// #define GIC_DIST_ISENABLER5 0x114
// #define GIC_DIST_ISENABLER6 0x118
// #define GIC_DIST_ISENABLER7 0x11C
// #define GIC_DIST_ISENABLER8 0x120
// #define GIC_DIST_ISENABLER9 0x124
// #define GIC_DIST_ISENABLER10 0x128
// #define GIC_DIST_ISENABLER11 0x12C
// #define GIC_DIST_ISENABLER12 0x130
// #define GIC_DIST_ISENABLER13 0x134
// #define GIC_DIST_ISENABLER14 0x138
// #define GIC_DIST_ISENABLER15 0x13C
// #define GIC_DIST_ISENABLER16 0x140
// #define GIC_DIST_ISENABLER17 0x144
// #define GIC_DIST_ISENABLER18 0x148
// #define GIC_DIST_ISENABLER19 0x14C
// #define GIC_DIST_ISENABLER20 0x150
// #define GIC_DIST_ISENABLER21 0x154
// #define GIC_DIST_ISENABLER22 0x158
// #define GIC_DIST_ISENABLER23 0x15C
// #define GIC_DIST_ISENABLER24 0x160
// #define GIC_DIST_ISENABLER25 0x164
// #define GIC_DIST_ISENABLER26 0x168
// #define GIC_DIST_ISENABLER27 0x16C
// #define GIC_DIST_ISENABLER28 0x170
// #define GIC_DIST_ISENABLER29 0x174
// #define GIC_DIST_ISENABLER30 0x178
#define GIC_DIST_ISENABLER31 0x17C

#define GIC_DIST_ICENABLER0 0x180
// #define GIC_DIST_ICENABLER1 0x184
// #define GIC_DIST_ICENABLER2 0x188
// #define GIC_DIST_ICENABLER3 0x18C
// #define GIC_DIST_ICENABLER4 0x190
// #define GIC_DIST_ICENABLER5 0x194
// #define GIC_DIST_ICENABLER6 0x198
// #define GIC_DIST_ICENABLER7 0x19C
// #define GIC_DIST_ICENABLER8 0x1A0
// #define GIC_DIST_ICENABLER9 0x1A4
// #define GIC_DIST_ICENABLER10 0x1A8
// #define GIC_DIST_ICENABLER11 0x1AC
// #define GIC_DIST_ICENABLER12 0x1B0
// #define GIC_DIST_ICENABLER13 0x1B4
// #define GIC_DIST_ICENABLER14 0x1B8
// #define GIC_DIST_ICENABLER15 0x1BC
// #define GIC_DIST_ICENABLER16 0x1C0
// #define GIC_DIST_ICENABLER17 0x1C4
// #define GIC_DIST_ICENABLER18 0x1C8
// #define GIC_DIST_ICENABLER19 0x1CC
// #define GIC_DIST_ICENABLER20 0x1D0
// #define GIC_DIST_ICENABLER21 0x1D4
// #define GIC_DIST_ICENABLER22 0x1D8
// #define GIC_DIST_ICENABLER23 0x1DC
// #define GIC_DIST_ICENABLER24 0x1E0
// #define GIC_DIST_ICENABLER25 0x1E4
// #define GIC_DIST_ICENABLER26 0x1E8
// #define GIC_DIST_ICENABLER27 0x1EC
// #define GIC_DIST_ICENABLER28 0x1F0
// #define GIC_DIST_ICENABLER29 0x1F4
// #define GIC_DIST_ICENABLER30 0x1F8
#define GIC_DIST_ICENABLER31 0x1FC

#define GIC_DIST_ISPENDR0 0x200
// #define GIC_DIST_ISPENDR1 0x204
// #define GIC_DIST_ISPENDR2 0x208
// #define GIC_DIST_ISPENDR3 0x20C
// #define GIC_DIST_ISPENDR4 0x210
// #define GIC_DIST_ISPENDR5 0x214
// #define GIC_DIST_ISPENDR6 0x218
// #define GIC_DIST_ISPENDR7 0x21C
// #define GIC_DIST_ISPENDR8 0x220
// #define GIC_DIST_ISPENDR9 0x224
// #define GIC_DIST_ISPENDR10 0x228
// #define GIC_DIST_ISPENDR11 0x22C
// #define GIC_DIST_ISPENDR12 0x230
// #define GIC_DIST_ISPENDR13 0x234
// #define GIC_DIST_ISPENDR14 0x238
// #define GIC_DIST_ISPENDR15 0x23C
// #define GIC_DIST_ISPENDR16 0x240
// #define GIC_DIST_ISPENDR17 0x244
// #define GIC_DIST_ISPENDR18 0x248
// #define GIC_DIST_ISPENDR19 0x24C
// #define GIC_DIST_ISPENDR20 0x250
// #define GIC_DIST_ISPENDR21 0x254
// #define GIC_DIST_ISPENDR22 0x258
// #define GIC_DIST_ISPENDR23 0x25C
// #define GIC_DIST_ISPENDR24 0x260
// #define GIC_DIST_ISPENDR25 0x264
// #define GIC_DIST_ISPENDR26 0x268
// #define GIC_DIST_ISPENDR27 0x26C
// #define GIC_DIST_ISPENDR28 0x270
// #define GIC_DIST_ISPENDR29 0x274
// #define GIC_DIST_ISPENDR30 0x278
#define GIC_DIST_ISPENDR31 0x27C

#define GIC_DIST_ICPENDR0 0x280
// #define GIC_DIST_ICPENDR1 0x284
// #define GIC_DIST_ICPENDR2 0x288
// #define GIC_DIST_ICPENDR3 0x28C
// #define GIC_DIST_ICPENDR4 0x290
// #define GIC_DIST_ICPENDR5 0x294
// #define GIC_DIST_ICPENDR6 0x298
// #define GIC_DIST_ICPENDR7 0x29C
// #define GIC_DIST_ICPENDR8 0x2A0
// #define GIC_DIST_ICPENDR9 0x2A4
// #define GIC_DIST_ICPENDR10 0x2A8
// #define GIC_DIST_ICPENDR11 0x2AC
// #define GIC_DIST_ICPENDR12 0x2B0
// #define GIC_DIST_ICPENDR13 0x2B4
// #define GIC_DIST_ICPENDR14 0x2B8
// #define GIC_DIST_ICPENDR15 0x2BC
// #define GIC_DIST_ICPENDR16 0x2C0
// #define GIC_DIST_ICPENDR17 0x2C4
// #define GIC_DIST_ICPENDR18 0x2C8
// #define GIC_DIST_ICPENDR19 0x2CC
// #define GIC_DIST_ICPENDR20 0x2D0
// #define GIC_DIST_ICPENDR21 0x2D4
// #define GIC_DIST_ICPENDR22 0x2D8
// #define GIC_DIST_ICPENDR23 0x2DC
// #define GIC_DIST_ICPENDR24 0x2E0
// #define GIC_DIST_ICPENDR25 0x2E4
// #define GIC_DIST_ICPENDR26 0x2E8
// #define GIC_DIST_ICPENDR27 0x2EC
// #define GIC_DIST_ICPENDR28 0x2F0
// #define GIC_DIST_ICPENDR29 0x2F4
// #define GIC_DIST_ICPENDR30 0x2F8
#define GIC_DIST_ICPENDR31 0x2FC

#define GIC_DIST_ISACTIVER0 0x300
// #define GIC_DIST_ISACTIVER1 0x304
// #define GIC_DIST_ISACTIVER2 0x308
// #define GIC_DIST_ISACTIVER3 0x30C
// #define GIC_DIST_ISACTIVER4 0x310
// #define GIC_DIST_ISACTIVER5 0x314
// #define GIC_DIST_ISACTIVER6 0x318
// #define GIC_DIST_ISACTIVER7 0x31C
// #define GIC_DIST_ISACTIVER8 0x320
// #define GIC_DIST_ISACTIVER9 0x324
// #define GIC_DIST_ISACTIVER10 0x328
// #define GIC_DIST_ISACTIVER11 0x32C
// #define GIC_DIST_ISACTIVER12 0x330
// #define GIC_DIST_ISACTIVER13 0x334
// #define GIC_DIST_ISACTIVER14 0x338
// #define GIC_DIST_ISACTIVER15 0x33C
// #define GIC_DIST_ISACTIVER16 0x340
// #define GIC_DIST_ISACTIVER17 0x344
// #define GIC_DIST_ISACTIVER18 0x348
// #define GIC_DIST_ISACTIVER19 0x34C
// #define GIC_DIST_ISACTIVER20 0x350
// #define GIC_DIST_ISACTIVER21 0x354
// #define GIC_DIST_ISACTIVER22 0x358
// #define GIC_DIST_ISACTIVER23 0x35C
// #define GIC_DIST_ISACTIVER24 0x360
// #define GIC_DIST_ISACTIVER25 0x364
// #define GIC_DIST_ISACTIVER26 0x368
// #define GIC_DIST_ISACTIVER27 0x36C
// #define GIC_DIST_ISACTIVER28 0x370
// #define GIC_DIST_ISACTIVER29 0x374
// #define GIC_DIST_ISACTIVER30 0x378
#define GIC_DIST_ISACTIVER31 0x37C

#define GIC_DIST_ICACTIVER0 0x380
// #define GIC_DIST_ICACTIVER1 0x384
// #define GIC_DIST_ICACTIVER2 0x388
// #define GIC_DIST_ICACTIVER3 0x38C
// #define GIC_DIST_ICACTIVER4 0x390
// #define GIC_DIST_ICACTIVER5 0x394
// #define GIC_DIST_ICACTIVER6 0x398
// #define GIC_DIST_ICACTIVER7 0x39C
// #define GIC_DIST_ICACTIVER8 0x3A0
// #define GIC_DIST_ICACTIVER9 0x3A4
// #define GIC_DIST_ICACTIVER10 0x3A8
// #define GIC_DIST_ICACTIVER11 0x3AC
// #define GIC_DIST_ICACTIVER12 0x3B0
// #define GIC_DIST_ICACTIVER13 0x3B4
// #define GIC_DIST_ICACTIVER14 0x3B8
// #define GIC_DIST_ICACTIVER15 0x3BC
// #define GIC_DIST_ICACTIVER16 0x3C0
// #define GIC_DIST_ICACTIVER17 0x3C4
// #define GIC_DIST_ICACTIVER18 0x3C8
// #define GIC_DIST_ICACTIVER19 0x3CC
// #define GIC_DIST_ICACTIVER20 0x3D0
// #define GIC_DIST_ICACTIVER21 0x3D4
// #define GIC_DIST_ICACTIVER22 0x3D8
// #define GIC_DIST_ICACTIVER23 0x3DC
// #define GIC_DIST_ICACTIVER24 0x3E0
// #define GIC_DIST_ICACTIVER25 0x3E4
// #define GIC_DIST_ICACTIVER26 0x3E8
// #define GIC_DIST_ICACTIVER27 0x3EC
// #define GIC_DIST_ICACTIVER28 0x3F0
// #define GIC_DIST_ICACTIVER29 0x3F4
// #define GIC_DIST_ICACTIVER30 0x3F8
#define GIC_DIST_ICACTIVER31 0x3FC

#define GIC_DIST_IPRIORITYR0 0x400
#define GIC_DIST_IPRIORITYR254 0x7F8

//
// Apple platforms only support affinity routing *on*, so these will be reserved.
//
#define GIC_DIST_ITARGETSR0 0x800
#define GIC_DIST_ITARGETSR254 0xBF8

#define GIC_DIST_ICFGR0 0xC00
#define GIC_DIST_ICFGR63 0xCFC

//
// These are RAZ/WI. (GICD_CTLR.DS = 1 always for us.)
//
#define GIC_DIST_IGRPMODR0 0xD00
#define GIC_DIST_IGRPMODR31 0xD7C

//
// Ditto the above.
//
#define GIC_DIST_NSACR0 0xE00
#define GIC_DIST_NSACR63 0xEFC
#define GIC_DIST_SGIR 0xF00
#define GIC_DIST_CPENDSGIR0 0xF10
#define GIC_DIST_CPENDSGIR3 0xF1C
#define GIC_DIST_SPENDSGIR0 0xF20
#define GIC_DIST_SPENDSGIR3 0xF2C




//========
// Structs
//========

/**
 * Distributor registers
 * 
 * These are global to the system, accesses from the guest via MMIO writes or reads will read/write data from an instance
 * of this struct.
 * 
 */

typedef struct vgicv3_distributor_registers {
    //0x0000-0x0010
    // Control, type, implementer ID, type register 2, error status regs

    //GICD_CTLR
    unsigned int gicd_ctl_reg;
    //GICD_TYPER
    unsigned int gicd_type_reg;
    //GICD_IIDR
    unsigned int gicd_imp_id_reg;
    //GICD_TYPER2
    unsigned int gicd_type_reg_2;
    //GICD_
    unsigned int gicd_err_sts;
    
    //0x0040 - GICD_SETSPI_NSR
    // Set SPI reg, non secure mode
    unsigned int gicd_set_spi_reg;
    
    //0x0048 - GICD_CLRSPI_NSR
    // Clear SPI reg, non secure mode
    unsigned int gicd_clear_spi_reg;

    //0x0080-0x00fc
    unsigned int gicd_interrupt_group_regs[32];

    //0x0100-0x017c
    unsigned int gicd_interrupt_set_enable_regs[32];

    //0x0180-0x01fc
    unsigned int gicd_interrupt_clear_enable_regs[32];

    //0x0200-0x027c
    unsigned int gicd_interrupt_set_pending_regs[32];

    //0x0280-0x02fc
    unsigned int gicd_interrupt_clear_pending_regs[32];

    //0x0300-0x037c
    unsigned int gicd_interrupt_set_active_regs[32];

    //0x038c-0x03fc
    unsigned int gicd_interrupt_clear_active_regs[32];

    //0x0400-0x07f8
    unsigned int gicd_interrupt_priority_regs[255];

    //0x0800-0x081c - GICD_ITARGETSR0-R7
    //reserved, Apple SoCs do not support legacy operation, so this is useless
    unsigned int gicd_interrupt_processor_target_regs_ro[8];

    //0x0820-0xBF8 - GICD_ITARGETSR8-R255
    //ditto above
    unsigned int gicd_interrupt_processor_target_regs_ro_2[248];

    //0x0C00-0x0CFC - GICD_ICFGR0-63
    unsigned int gicd_interrupt_config_regs[64];

    //0x0D00-0x0D7C - GICD_IGRPMODR0-31
    // RAZ/WI since we only support a single security state (EL2/GL2 get treated equally in this regard)
    unsigned int gicd_interrupt_group_modifier_regs[32];

    //0x0E00-0x0EFC - GICD_NSACR0-63
    //i have doubts as to whether this is necessary, given M series don't implement EL3
    unsigned int gicd_interrupt_nonsecure_access_ctl_regs[64];

    //0x0F00 - GICD_SGIR (software generated interrupts)
    unsigned int gicd_interrupt_software_generated_reg;

    //0x0F10-0x0F1C - GICD_CPENDSGIR0-3
    unsigned int gicd_interrupt_sgi_clear_pending_regs[4];

    //0x0F20-0x0F2C - GICD_SPENDSGIR0-3
    unsigned int gicd_interrupt_sgi_set_pending_regs[4];

    //0x0F80-0x0FFC - GICD_INMIR - NMI Regs
    //Apple SoCs as of 8/17/2022 do not implement NMI, these will never be used by anything but add them so that the size of the dist follows ARM spec
    unsigned int gicd_interrupt_nmi_regs[32];

    //0x1000-0x107C - GICD_IGROUPR0E-31E
    unsigned int gicd_interrupt_group_regs_ext_spi_range[32];

    //0x1200-0x127C - GICD_ISENABLER0E-31E
    unsigned int gicd_interrupt_set_enable_ext_spi_range_regs[32];

    //0x1400-0x147C - GICD_ICENABLER0E-31E
    unsigned int gicd_interrupt_clear_enable_ext_spi_range_regs[32];

    //0x1600-0x167C - GICD_ISPENDR0E-31E
    unsigned int gicd_interrupt_set_pending_ext_spi_range_regs[32];

    //0x1800-0x187C - GICD_ICPENDR0E-31E
    unsigned int gicd_interrupt_clear_pending_ext_spi_range_regs[32];

    //0x1A00-0x1A7C - GICD_ISACTIVER0E-31E
    unsigned int gicd_interrupt_set_active_ext_spi_range_regs[32];

    //0x1C00-0x1C7C - GICD_ICACTIVER0E-31E
    unsigned int gicd_interrupt_clear_active_ext_spi_range_regs[32];

    //0x2000-0x23FC - GICD_IPRIORITYR0E-255E
    unsigned int gicd_interrupt_priority_ext_spi_range_regs[256];

    //0x3000-0x30FC - GICD_ICFGR0E-63E
    unsigned int gicd_interrupt_ext_spi_config_regs[64];

    //0x3400-0x347C - GICD_IGRPMODR0E-61E
    unsigned int gicd_interrupt_group_modifier_ext_spi_range_regs[32];

    //0x3600-0x367C - GICD_NSACR0E-31E
    unsigned int gicd_non_secure_ext_spi_range_interrupt_regs[32];

    //0x3B00-0x3B7C
    //NMI regs for extended SPI range
    //ditto above point, no NMI support on Apple chips, but add it so that the size of the dist is the same as ARM spec
    unsigned int gicd_interrupt_nmi_reg_ext_spi_range[32];

    //0x6100-0x7FD8 - GICD_IROUTER(32-1019)
    unsigned long gicd_interrupt_router_regs[988];

    //0x8000-0x9FFC - GICD_IROUTER(0-1023)E
    unsigned long gicd_interrupt_router_ext_spi_range_regs[1024];


} vgicv3_dist;
//
// Redistributor registers (we're emulating a GICv3 so only need the RD and SGI frames.)
//
typedef struct vgicv3_redistributor_rd_region {
    //GICR_CTLR
    unsigned int gicr_ctl_reg;
    //GICR_IIDR
    unsigned int gicr_iidr;
    //GICR_TYPER
    uint64_t gicr_type_reg;
    //GICR_STATUSR
    unsigned int gicr_status_reg;
    //GICR_WAKER
    unsigned int gicr_wake_reg;
    //GICR_MPAMIDR
    unsigned int gicr_mpamidr;
    //GICR_PARTIDR
    unsigned int gicr_partidr;
    //0x20 - 0x3C: IMPDEF registers - we can put whatever type of extra register we need in this region.
    unsigned int gicr_impdef_reserved0[7];
    //0x40 - GICR_SETLPIR
    uint64_t gicr_setlpir;
    //0x48 - GICR_CLRLPIR
    uint64_t gicr_clrlpir;
    //0x50-0x6C - GICR reserved region
    unsigned int gicr_reserved0[7];
    //0x70 - GICR_PROPBASER
    uint64_t gicr_propbaser;
    //0x78 - GICR_PENDBASER
    uint64_t gicr_pendbaser;
    //0x80-0xA0 - reserved region.
    uint64_t gicr_reserved1[4];
    //0xA0 - GICR_INVLPIR
    uint64_t gicr_invlpir;
    //0xA8 - reserved
    uint64_t gicr_reserved2;
    //0xB0 - GICR_INVALLR
    uint64_t gicr_invallr;
    //0xB8 - reserved
    uint64_t gicr_reserved3;
    //0xC0 - GICR_SYNCR
    uint64_t gicr_syncr;
    //0xC8 - 0xFC - reserved
    unsigned int gicr_reserved4[13];
    //0x100 - GICR IMPDEF register
    uint64_t gicr_impdef_reserved1;
    //0x108 - reserved
    uint64_t gicr_reserved5;
    //0x110 - GICR IMPDEF register
    uint64_t gicr_impdef_reserved2;
    //0x118-0xBFFC - reserved
    unsigned int gicr_reserved6[12217];
    //0xc000-0xFFCC - reserved
    unsigned int gicr_reserved8[4083];
    //0xFFCC - filler
    unsigned int gicr_reserved9;
    //0xFFD0 - 0xFFFC - reserved for id registers
    uint64_t gicr_reserved_idreg[6];
    
} vgicv3_redistributor_rd_region_t;

typedef struct vgicv3_redistributor_sgi_region {
    //0x0-0x7C - padding
    uint64_t gicr_sgi_region_padding0[16];
    //0x80 - GICR_IGROUPR0
    unsigned int gicr_igroupr0;
    //0x84 - GICR_IGROUPR1E
    unsigned int gicr_igroupr1e;
    //0x88 - GICR_IGROUPR2E
    unsigned int gicr_igroupr2e;
    //0x8C-0x100 - padding
    unsigned int gicr_sgi_region_padding1[5];
    //0x100 - GICR_ISENABLER0
    unsigned int gicr_isenabler0;
    //0x104 - GICR_ISENABLER1E
    unsigned int gicr_isenabler1e;
    //0x108 - GICR_ISENABLER2E
    unsigned int gicr_isenabler2e;
    //0x10C-0x180 - padding
    unsigned int gicr_sgi_region_padding2[29];
    //0x180 - GICR_ICENABLER0
    unsigned int gicr_icenabler0;
    //0x184 - GICR_ICENABLER1E
    unsigned int gicr_icenabler1e;
    //0x188 - GICR_ICENABLER2E
    unsigned int gicr_icenabler2e;
    //0x18C-0x200 - padding
    unsigned int gicr_sgi_region_padding3[29];
    //0x200 - GICR_ISPENDR0
    unsigned int gicr_ispendr0;
    //0x204 - GICR_ISPENDR1E
    unsigned int gicr_ispendr1e;
    //0x208 - GICR_ISPENDR2E
    unsigned int gicr_ispendr2e;
    //0x20C-0x280 - padding
    unsigned int gicr_sgi_region_padding4[29];
    //0x280 - GICR_ICPENDR0
    unsigned int gicr_icpendr0;
    //0x284 - GICR_ICPENDR1E
    unsigned int gicr_icpendr1e;
    //0x288 - GICR_ICPENDR2E
    unsigned int gicr_icpendr2e;
    //0x28C-0x300 - padding
    unsigned int gicr_sgi_region_padding5[29];
    //0x300 - GICR_ISACTIVER0
    unsigned int gicr_isactiver0;
    //0x304 - GICR_ISACTIVER1E
    unsigned int gicr_isactiver1e;
    //0x308 - GICR_ISACTIVER2E
    unsigned int gicr_isactiver2e;
    //0x30C-0x380 - padding
    unsigned int gicr_sgi_region_padding6[29];
    //0x380 - GICR_ICACTIVER0
    unsigned int gicr_icactiver0;
    //0x384 - GICR_ICACTIVER1E
    unsigned int gicr_icactiver1e;
    //0x388 - GICR_ICACTIVER2E
    unsigned int gicr_icactiver2e;
    //0x38C-0x3FC - padding
    unsigned int gicr_sgi_region_padding7[28];
    //0x400-0x40C - GICR_IPRIORITYR(0-3) (SGI priorities)
    unsigned int gicr_sgi_ipriority_reg[4];
    //0x410-0x41C - GICR_IPRIORITYR(4-7) (PPI/core specific interrupt priorities)
    unsigned int gicr_ppi_ipriority_reg[4];
    //0x420-0x45C - GICR_IPRIORITYR(8-23)E
    unsigned int gicr_ipriorityr_ext_ppi[15];
    //0x460-0xBFC - padding
    unsigned int gicr_sgi_region_padding8[487];
    //0xC00 - GICR_ICFGR0
    unsigned int gicr_icfgr0;
    //0xC04 - GICR_ICFGR1
    unsigned int gicr_icfgr1;
    //0xC08 - GICR_ICFGR2E
    unsigned int gicr_icfgr2e;
    //0xC0C - GICR_ICFGR3E
    unsigned int gicr_icfgr3e;
    //0xC10 - GICR_ICFGR4E
    unsigned int gicr_icfgr4e;
    //0xC14 - GICR_ICFGR5E
    unsigned int gicr_icfgr5e;
    //0xC14-0xCFC - padding
    unsigned int gicr_sgi_region_padding9[58];
    //0xD00 - GICR_IGRPMODR0
    unsigned int gicr_igrpmodr0;
    //0xD04 - GICR_IGRPMODR1E
    unsigned int gicr_igrpmodr1e;
    //0xD08 - GICR_IGRPMODR2E
    unsigned int gicr_igrpmodr2e;
    //0xD0C-0xDFC - padding
    unsigned int gicr_sgi_region_padding10[60];
    //0xE00 - GICR_NSACR
    unsigned int gicr_nsacr;
    //0xE04-0xF7C - reserved
    unsigned int gicr_sgi_region_reserved0[94];
    //0xF80 - GICR_INMIR0
    unsigned int gicr_inmir0;
    //0xF84-0xFFC - GICR_INMIR(1-2)E (leaving this RAZ/WI for now)
    unsigned int gicr_inmir_e[30];
    //0x1000-0xBFFC - reserved
    unsigned int gicr_sgi_region_reserved1[11263];
    //0xC000-0xFFCC - IMPDEF registers
    unsigned int gicr_sgi_region_impdef_reserved0[4083];
    //0xFFD0-0xFFFC - reserved
    unsigned int gicr_sgi_region_reserved2[11];


} vgicv3_redistributor_sgi_region_t;

typedef struct vgicv3_redistributor_region {
    vgicv3_redistributor_rd_region_t rd_region;
    vgicv3_redistributor_sgi_region_t sgi_region;
} vgicv3_vcpu_redist;

//
// ITS registers
//

typedef struct vgicv3_its_ctl_region {
    //0x0 - GITS_CTLR
    unsigned int gits_ctl_reg;
    //0x4 - GITS_IIDR
    unsigned int gits_iidr;
    //0x8 - GITS_TYPER
    uint64_t gits_type_reg;
    //0x10 - GITS_MPAMIDR
    unsigned int gits_mpamidr;
    //0x14 - GITS_PARTIDR
    unsigned int gits_partidr;
    //0x18 - GITS_MPIDR
    unsigned int gits_mpidr;
    //0x1C - reserved
    unsigned int gits_reserved0;
    //0x20-0x3C - IMPDEF reserved
    unsigned int gits_impdef_reserved0[7];
    //0x40 - GITS_STATUSR
    unsigned int gits_statusr;
    //0x44 - reserved
    unsigned int gits_reserved1;
    //0x48 - GITS_UMSIR
    uint64_t gits_umsir;
    //0x50-0x7C - reserved
    unsigned int gits_reserved2[11];
    //0x80 - GITS_CBASER
    uint64_t gits_cbaser;
    //0x88 - GITS_CWRITER
    uint64_t gits_cwriter;
    //0x90 - GITS_CREADR
    uint64_t gits_creadr;
    //0x98-0xFC - reserved
    unsigned int gits_reserved3[25];
    //0x100-0x138 - GITS_BASER(0-7)
    uint64_t gits_baser[8];
    //0x140-0xBFFC - reserved
    unsigned int gits_reserved4[12207];
    //0xC000-0xFFCC - IMPDEF registers
    unsigned int gits_impdef_reserved1[4083];
    //0xFFD0-0xFFFC - ID register reserved
    unsigned int gits_impdef_reserved2[11];
} vgicv3_its_ctl_region_t;

typedef struct vgicv3_its_translation_region {
    //0x0-0x3C - reserved
    unsigned int gits_translation_reserved0[15];
    //0x40 - GITS_TRANSLATER
    unsigned int gits_translater;
    //0x44-0xFFFC - reserved
    unsigned int gits_translation_reserved0[16366];
} vgicv3_its_translation_region_t;

typedef struct vgicv3_its_region {
    vgicv3_its_ctl_region_t its_ctl_region;
    //TODO: check if there needs to be a filler here.
    vgicv3_its_translation_region_t its_translation_region;
} vgicv3_its;


/* vGIC */

int hv_vgicv3_init(void);

void hv_vgicv3_init_dist_registers(void);

void hv_vgicv3_init_list_registers(int n);

int hv_vgicv3_enable_virtual_interrupts(void);

#endif //HV_VGIC_H