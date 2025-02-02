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
 * Xosera rosco_m68k low-level C API for Xosera registers
 * ------------------------------------------------------------
 */

#include <stdbool.h>
#include <stdint.h>

#include <machine.h>

#define XV_PREP_REQUIRED
#include "xosera_m68k_api.h"

#define SYNC_RETRIES 250        // ~1/4 second

// TODO: This is less than ideal (tuned for ~10MHz)
__attribute__((noinline)) void cpu_delay(int ms)
{
    __asm__ __volatile__(
        "    lsl.l   #8,%[temp]\n"
        "    add.l   %[temp],%[temp]\n"
        "0:  sub.l   #1,%[temp]\n"
        "    tst.l   %[temp]\n"
        "    bne.s   0b\n"
        : [temp] "+d"(ms));
}

// delay for approx ms milliseconds
void xv_delay(uint32_t ms)
{
    if (!xosera_sync())
    {
        return;
    }

    xv_prep();
    while (ms--)
    {
        for (uint16_t tms = 10; tms != 0; --tms)
        {
            uint16_t tv = xm_getw(TIMER);
            while (tv == xm_getw(TIMER))
                ;
        }
    }
}

// return true if Xosera responding (may BUS ERROR if no hardware present)
bool xosera_sync()
{
    xv_prep();

    uint16_t rd_incr   = xm_getw(RD_INCR);
    uint16_t test_incr = rd_incr ^ 0xF5FA;
    xm_setw(RD_INCR, test_incr);
    if (xm_getw(RD_INCR) != test_incr)
    {
        return false;        // not detected
    }
    xm_setw(RD_INCR, rd_incr);

    return true;
}

// wait for Xosera to respond after reconfigure
bool xosera_wait_sync()
{
    // check for Xosera presense (retry in case it is reconfiguring)
    for (uint16_t r = SYNC_RETRIES; r != 0; --r)
    {
        if (xosera_sync())
        {
            return true;
        }
        cpu_delay(10);
    }
    return false;
}

// reconfigure or sync Xosera and return true if it is responsive
bool xosera_init(int reconfig_num)
{
    xv_prep();

    bool detected = xosera_wait_sync();

    if (detected)
    {
        // reconfig if configuration valid (0 to 3)
        if ((reconfig_num & 3) == reconfig_num)
        {
            uint16_t sys_ctrl_save = xm_getw(SYS_CTRL) & 0x0F0F;               // save SYS_CTRL
            xm_setw(SYS_CTRL, 0x800F | (uint16_t)(reconfig_num << 13));        // reboot FPGA to config_num
            detected = xosera_wait_sync();                                     // wait for detect
            xm_setw(SYS_CTRL, sys_ctrl_save);                                  // restore SYS_CTRL
        }
    }

    return detected;
}

// define xosera_ptr in a way that GCC can't see the immediate const value (causing it to keep it in a register).
__asm__(
    "               .text\n"
    "               .align      2\n"
    "               .globl      xosera_ptr\n"
    "xosera_ptr:    .long       " XM_STR(XM_BASEADDR) "\n");
