#ifndef __RTC_H

#define __RTC_H


#include "types.h"


void init_rtc(void);

void get_rtc_time(s32 *hour, s32 *minute, s32 *second);

void disp_rtc_time(u32 hour, u32 minute, u32 second);

void get_rtc_date(s32 *date, s32 *month, s32 *year);

void disp_rtc_date(u32 date, u32 month, u32 year);

void set_rtc_time(u32 hour, u32 minute, u32 second);

void set_rtc_date(u32 date, u32 month, u32 year);


void get_rtc_day(s32 *day);

void disp_rtc_day(u32 dow);

void set_rtc_day(u32 day);


#endif
