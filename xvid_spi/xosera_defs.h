/*
 * vim: set et ts=4 sw=4
 *------------------------------------------------------------
 *  __ __
 * |  |  |___ ___ ___ ___ ___
 * |-   -| . |_ -| -_|  _| .'|
 * |__|__|___|___|___|_| |__,|
 *
 * Xark's Open Source Enhanced Retro Adapter
 *
 * - "Not as clumsy or random as a GPU, an embedded retro
 *    adapter for a more civilized age."
 *
 * ------------------------------------------------------------
 * Copyright (c) 2021 Xark
 * MIT License
 *
 * Xosera cross-platform C register definition header file
 * ------------------------------------------------------------
 */

// See: https://github.com/XarkLabs/Xosera/blob/master/REFERENCE.md

#error out of date

// Xosera Main Registers (XM Registers, directly CPU accessable)
#define XM_XR_ADDR   0x0        // (R /W+) XR register number/address for XM_XR_DATA read/write access
#define XM_XR_DATA   0x1        // (R /W+) read/write XR register/memory at XM_XR_ADDR (XM_XR_ADDR incr. on write)
#define XM_RD_INCR   0x2        // (R /W ) increment value for XM_RD_ADDR read from XM_DATA/XM_DATA_2
#define XM_RD_ADDR   0x3        // (R /W+) VRAM address for reading from VRAM when XM_DATA/XM_DATA_2 is read
#define XM_WR_INCR   0x4        // (R /W ) increment value for XM_WR_ADDR on write to XM_DATA/XM_DATA_2
#define XM_WR_ADDR   0x5        // (R /W ) VRAM address for writing to VRAM when XM_DATA/XM_DATA_2 is written
#define XM_DATA      0x6        // (R+/W+) read/write VRAM word at XM_RD_ADDR/XM_WR_ADDR (and add XM_RD_INCR/XM_WR_INCR)
#define XM_DATA_2    0x7        // (R+/W+) 2nd XM_DATA(to allow for 32-bit read/write access)
#define XM_SYS_CTRL  0x8        // (R /W+) busy status, FPGA reconfig, interrupt status/control, write masking
#define XM_TIMER     0x9        // (RO   ) read 1/10th millisecond timer [TODO]
#define XM_UNUSED_A  0xA        // (R /W ) unused direct register 0xA [TODO]
#define XM_UNUSED_B  0xB        // (R /W ) unused direct register 0xB [TODO]
#define XM_RW_INCR   0xC        // (R /W ) XM_RW_ADDR increment value on read/write of XM_RW_DATA/XM_RW_DATA_2
#define XM_RW_ADDR   0xD        // (R /W+) read/write address for VRAM access from XM_RW_DATA/XM_RW_DATA_2
#define XM_RW_DATA   0xE        // (R+/W+) read/write VRAM word at XM_RW_ADDR (and add XM_RW_INCR)
#define XM_RW_DATA_2 0xF        // (R+/W+) 2nd XM_RW_DATA(to allow for 32-bit read/write access)

// XR Extended Register / Region (accessed via XM_XR_ADDR and XM_XR_DATA)

// XR Register Regions
#define XR_CONFIG_REGS   0x0000        // 0x0000-0x000F 16 config/copper registers
#define XR_PA_REGS       0x0010        // 0x0000-0x0017 8 playfield A video registers
#define XR_PB_REGS       0x0018        // 0x0000-0x000F 8 playfield B video registers
#define XR_BLIT_REGS     0x0020        // 0x0000-0x000F 16 blit registers [TBD]
#define XR_POLYDRAW_REGS 0x0030        // 0x0000-0x000F 16 line/polygon draw registers [TBD]

// XR Memory Regions
// Xosera XR Memory Regions (size in 16-bit words)
#define XR_COLOR_ADDR   0x8000        // (R/W) 0x8000-0x81FF 2 x A & B color lookup memory
#define XR_COLOR_SIZE   0x0200        //                      2 x 256 x 16-bit words  (0xARGB)
#define XR_COLOR_A_ADDR 0x8000        // (R/W) 0x8000-0x80FF A 256 entry color lookup memory
#define XR_COLOR_A_SIZE 0x0100        //                      256 x 16-bit words (0xARGB)
#define XR_COLOR_B_ADDR 0x8100        // (R/W) 0x8100-0x81FF B 256 entry color lookup memory
#define XR_COLOR_B_SIZE 0x0100        //                      256 x 16-bit words (0xARGB)
#define XR_TILE_ADDR    0xA000        // (R/W) 0xA000-0xB3FF tile glyph/tile map memory
#define XR_TILE_SIZE    0x1400        //                      5120 x 16-bit tile glyph/tile map memory
#define XR_COPPER_ADDR  0xC000        // (R/W) 0xC000-0xC7FF copper program memory (32-bit instructions)
#define XR_COPPER_SIZE  0x0800        //                      2048 x 16-bit copper program memory addresses
#define XR_UNUSED_ADDR  0xE000        // (-/-) 0xE000-0xFFFF unused

// Video Config / Copper XR Registers
#define XR_VID_CTRL  0x00        // (R /W) display control and border color index
#define XR_COPP_CTRL 0x01        // (R /W) display synchronized coprocessor control
#define XR_UNUSED_02 0x02        // (R /W) // TODO:
#define XR_UNUSED_03 0x03        // (R /W) // TODO:
#define XR_UNUSED_04 0x04        // (R /W) // TODO:
#define XR_UNUSED_05 0x05        // (R /W) // TODO:
#define XR_VID_LEFT  0x06        // (R /W) left edge of active display window (typically 0)
#define XR_VID_RIGHT 0x07        // (R /W) right edge of active display window (typically 639 or 847)
#define XR_SCANLINE  0x08        // (RO  ) [15] in V blank, [14] in H blank [10:0] V scanline
#define XR_UNUSED_09 0x09        // (RO  )
#define XR_VERSION   0x0A        // (RO  ) Xosera optional feature bits [15:8] and version code [7:0] [TODO]
#define XR_GITHASH_H 0x0B        // (RO  ) [15:0] high 16-bits of 32-bit Git hash build identifier
#define XR_GITHASH_L 0x0C        // (RO  ) [15:0] low 16-bits of 32-bit Git hash build identifier
#define XR_VID_HSIZE 0x0D        // (RO  ) native pixel width of monitor mode (e.g. 640/848)
#define XR_VID_VSIZE 0x0E        // (RO  ) native pixel height of monitor mode (e.g. 480)
#define XR_VID_VFREQ 0x0F        // (RO  ) update frequency of monitor mode in BCD 1/100th Hz (0x5997 = 59.97 Hz)

// Playfield A Control XR Registers
#define XR_PA_GFX_CTRL  0x10        //  playfield A graphics control
#define XR_PA_TILE_CTRL 0x11        //  playfield A tile control
#define XR_PA_DISP_ADDR 0x12        //  playfield A display VRAM start address
#define XR_PA_LINE_LEN  0x13        //  playfield A display line width in words
#define XR_PA_HV_SCROLL 0x14        //  playfield A horizontal and vertical fine scroll
#define XR_PA_LINE_ADDR 0x15        //  playfield A scanline start address (loaded at start of line)
#define XR_PA_UNUSED_16 0x16        //
#define XR_PA_UNUSED_17 0x17        //

// Playfield B Control XR Registers
#define XR_PB_GFX_CTRL  0x18        //  playfield B graphics control
#define XR_PB_TILE_CTRL 0x19        //  playfield B tile control
#define XR_PB_DISP_ADDR 0x1A        //  playfield B display VRAM start address
#define XR_PB_LINE_LEN  0x1B        //  playfield B display line width in words
#define XR_PB_HV_SCROLL 0x1C        //  playfield B horizontal and vertical fine scroll
#define XR_PB_LINE_ADDR 0x1D        //  playfield B scanline start address (loaded at start of line)
#define XR_PB_UNUSED_1E 0x1E        //
#define XR_PB_UNUSED_1F 0x1F        //
