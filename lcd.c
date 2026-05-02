#include "aurdino_define.h"
#include "types.h"
#include "lcd_defines.h"
#include "delay.h"
#include "aurdino.h"

// Write a byte to the LCD data bus
void writelcd(u8 byte)
{
    digitalwrite(LCD_RW, 0);
    write2pins(LCD_DATA, 8, byte);
    digitalwrite(LCD_EN, 1);
    delay_us(1);
    digitalwrite(LCD_EN, 0);
    delay_ms(2);
}

// Send command to LCD
void cmdlcd(u8 cmdbyte)
{
    digitalwrite(LCD_RS, 0);
    writelcd(cmdbyte);
}

// Initialize LCD display
void init_lcd(void)
{
    portmode(LCD_DATA, 8, OUTPUT);
    portmode(LCD_RS, 3, OUTPUT);
    delay_ms(15);
    cmdlcd(MODE_8BIT_1LINE);
    delay_ms(5);
    cmdlcd(MODE_8BIT_1LINE);
    delay_us(100);
    cmdlcd(MODE_8BIT_1LINE);
    cmdlcd(MODE_8BIT_2LINE);
    cmdlcd(DSP_ON_CUR_OFF);
    cmdlcd(CLEAR_LCD);
    cmdlcd(SHIFT_CUR_RIGHT);
}

// Write a single ASCII character to LCD
void charlcd(u8 asciival)
{
    digitalwrite(LCD_RS, 1);
    writelcd(asciival);
}

// Write a string to LCD
void strlcd(s8 *p)
{
    while (*p)
        charlcd(*p++);
}

// Write unsigned 32-bit integer to LCD
void u32lcd(u32 n)
{
    u8 digit[10];
    s32 i = 0;
    if (n == 0)
    {
        charlcd('0');
    }
    else
    {
        while (n > 0)
        {
            digit[i++] = (n % 10) + '0';
            n /= 10;
        }
        for (--i; i >= 0; i--)
        {
            charlcd(digit[i]);
        }
    }
}

// Write signed 32-bit integer to LCD
void s32lcd(s32 n)
{
    if (n < 0)
    {
        charlcd('-');
        n = -n;
    }
    u32lcd(n);
}

// Write float to LCD with ndp decimal places
void f32lcd(f32 fn, u32 ndp)
{
    u32 n, i;
    if (fn < 0.0)
    {
        charlcd('-');
        fn = -fn;
    }
    n = fn;
    u32lcd(n);
    charlcd('.');
    for (i = 0; i < ndp; i++)
    {
        fn = (fn - n) * 10;
        n = fn;
        charlcd(n + '0');
    }
}

// Build custom characters in LCD CGRAM
void buildcgram(u8 *p, u8 nbytes)
{
    u32 i;
    cmdlcd(GOTO_CGRAM_START);
    digitalwrite(LCD_RS, 1);
    for (i = 0; i < nbytes; i++)
    {
        writelcd(p[i]);
    }
    cmdlcd(GOTO_LINE1_POS0);
}
