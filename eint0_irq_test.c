#include <lpc21xx.h>
#include "types.h"
#include "pin_connect_block.h"
#include "pin_functions_definition.h"
#include "lcd.h"
#include "lcd_defines.h"

#define EINT0_VIC_CHNO     15
#define EINT0_STATUS_LED   3
#define EINT0_PIN_0_1      3

extern volatile int flag;    // FIX: added volatile

void eint0_isr(void) __irq;

void eint(void)
{
    IODIR1 |= (1 << EINT0_STATUS_LED);
    cfgportpinfunc(0, 3, EINT0_PIN_0_1);
    VICIntEnable = 1 << EINT0_VIC_CHNO;
    VICVectCntl1 = (1 << 5) | EINT0_VIC_CHNO;
    VICVectAddr1 = (u32)eint0_isr;
    EXTMODE = 1 << 0;
}

void eint0_isr(void) __irq
{
    flag = 1;

    EXTINT = 1 << 0;
    VICVectAddr = 0;
}
