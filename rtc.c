#include <lpc21xx.h>
#include "rtc_defines.h"
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"

char week[][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

void init_rtc(void)
{
    CCR = RTC_RESET;
    PREINT = PREINT_VAL;
    PREFRAC = PREFRAC_VAL;
    CCR = RTC_ENABLE | RTC_CLKSRC;
}

void get_rtc_time(s32 *hour, s32 *minute, s32 *second)
{
    *hour   = HOUR;
    *minute = MIN;
    *second = SEC;
}

void disp_rtc_time(u32 hour, u32 minute, u32 second)
{
    cmdlcd(0xc0);
    strlcd("T:");
    charlcd((hour / 10) + '0');
    charlcd((hour % 10) + '0');
    charlcd('-');
    charlcd((minute / 10) + '0');
    charlcd((minute % 10) + '0');
}

void get_rtc_date(s32 *date, s32 *month, s32 *year)
{
    *date  = DOM;
    *month = MONTH;
    *year  = YEAR;
}

void disp_rtc_date(u32 date, u32 month, u32 year)
{
    cmdlcd(GOTO_LINE2_POS0);
    charlcd((date / 10) + '0');
    charlcd((date % 10) + '0');
    charlcd('-');
    charlcd((month / 10) + '0');
    charlcd((month % 10) + '0');
    charlcd('-');
    u32lcd(year);
}

void set_rtc_time(u32 hour, u32 minute, u32 second)
{
    HOUR = hour;
    MIN  = minute;
    SEC  = second;
}

void set_rtc_date(u32 date, u32 month, u32 year)
{
    DOM   = date;
    MONTH = month;
    YEAR  = year;
}

void get_rtc_day(s32 *day)
{
    *day = DOW;
}

void disp_rtc_day(u32 dow)
{
    cmdlcd(GOTO_LINE1_POS0 + 10);
    strlcd(week[dow]);
}

void set_rtc_day(u32 day)
{
    DOW = day;
}
