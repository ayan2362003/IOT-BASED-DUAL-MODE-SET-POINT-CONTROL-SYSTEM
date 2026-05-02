#include <lpc21xx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "adc.h"
#include "adc_defines.h"
#include "types.h"
#include "kpm.h"
#include "kpm_defines.h"
#include "spi.h"
#include "spi_defines.h"
#include "spi_eeprom.h"
#include "spi_eeprom_defines.h"
#include "uart0.h"
#include "uart0_defines.h"
#include "rtc.h"
#include "rtc_defines.h"
#include "esp01.h"

#define BUZZER            23
#define UPLOAD_INTERVAL   120
#define SYNC_INTERVAL     180
#define DEFAULT_SET_POINT "30.0"

u8 set_point[20], eeprom_read[20], value[20];
s32 hour, min, second;
f32 sp;
volatile int flag = 0;
u32 key;
u32 last_upload_time = 0;
u32 last_sync_time   = 0;
u32 current_time     = 0;

void eint(void);
void read_temp(u8 *temp);
void set_point_change(void);
float my_atof(u8 *s);
void read_lm35(f32 *degc, f32 *degf);
void update_to_eeprom(void);
void collect_from_eeprom(void);

/* ---------------- EEPROM SAVE ---------------- */

void update_to_eeprom(void)
{
    u32 i = 0;

    while(1)
    {
        spi_eeprom_byte_write(i, set_point[i]);
        if(set_point[i] == '\0')
            break;
        i++;
    }

    cmdlcd(0x01); delay_ms(5);
    strlcd("Saving SetPoint");
    cmdlcd(0xC0);
    strlcd("EEPROM Update");
    delay_s(2);
    cmdlcd(0x01); delay_ms(5);
}

/* ---------------- SET POINT CHANGE ---------------- */

void set_point_change(void)
{
    IOCLR0 = 1 << BUZZER;

    cmdlcd(0x01); delay_ms(5); cmdlcd(0x80);
    strlcd("New Set Point");
    cmdlcd(0xC0);
    strlcd("Enter Value");
    delay_s(1);

    read_temp(set_point);

    cmdlcd(0x01); delay_ms(5); cmdlcd(0x80);
    strlcd("SetPt Saved");
    cmdlcd(0xC0);
    strlcd("Successful");
    delay_s(2);

    update_to_eeprom();
    sp = my_atof(set_point);

    cmdlcd(0x01); delay_ms(5); cmdlcd(0x80);
    strlcd("Upload to Cloud");
    cmdlcd(0xC0);
    strlcd("Please Wait...");
    esp01_sendToThingspeak_set_point(sp);
    delay_s(2);

    // last_upload_time = current_time;   // FIX: REMOVED
    last_sync_time = current_time;
}

/* ---------------- KEYPAD INPUT ---------------- */

void read_temp(u8 *temp)
{
    u32 i = 0;
    u32 j = 0;

    cmdlcd(0xC0);
    strlcd("                ");

    while(1)
    {
        key = keyscan();
        delay_ms(100);

        if((key >= '0') && (key <= '9'))
        {
            temp[i++] = key;
            cmdlcd(0xC0 + j++);
            charlcd(key);
            while(colscan() == 0);
        }
        else if(key == '.')
        {
            temp[i++] = key;
            cmdlcd(0xC0 + j++);
            charlcd(key);
            while(colscan() == 0);
        }
        else if(key == 'e')
        {
            temp[i] = '\0';
            break;
        }
        else if(key == '+')
        {
            if(j > 0 && i > 0)
            {
                cmdlcd(0xC0 + --j);
                strlcd(" ");
                i--;
            }
        }
    }
}

/* ---------------- LM35 ---------------- */

void read_lm35(f32 *degc, f32 *degf)
{
    u32 adcdval;
    f32 ear;

    read_adc(1, &adcdval, &ear);

    *degc = ear * 100.0;
    *degf = (*degc * 1.8) + 32.0;
}

/* ---------------- STRING TO FLOAT ---------------- */

float my_atof(u8 *s)
{
    f32 val = 0.0, div = 1.0;
    u32 dot_found = 0, i = 0;

    if(s == NULL || s[0] == '\0')
        return 0.0;

    while(s[i])
    {
        if(s[i] == '.')
        {
            dot_found = 1;
            i++;
            continue;
        }

        if(s[i] < '0' || s[i] > '9')
            break;

        val = val * 10 + (s[i] - '0');

        if(dot_found)
            div *= 10.0;

        i++;
    }

    return val / div;
}

/* ---------------- EEPROM READ ---------------- */

void collect_from_eeprom(void)
{
    u32 i = 0;
    u8 first_byte = spi_eeprom_byte_read(0);

    if(first_byte == 0xFF || first_byte == 0x00)
    {
        strcpy((char *)eeprom_read, DEFAULT_SET_POINT);

        cmdlcd(0x01); delay_ms(5); cmdlcd(0x80);
        strlcd("No SetPt Found");
        cmdlcd(0xC0);
        strlcd("Default:30.0");
        delay_s(3);
        cmdlcd(0x01); delay_ms(5);
        return;
    }

    while(i < 19)
    {
        eeprom_read[i] = spi_eeprom_byte_read(i);
        if(eeprom_read[i] == '\0')
            break;
        i++;
    }

    eeprom_read[i] = '\0';

    cmdlcd(0x01); delay_ms(5); cmdlcd(0x80);
    strlcd("Loading SetPoint");
    cmdlcd(0xC0);
    strlcd((s8*)eeprom_read);
    delay_s(2);
    cmdlcd(0x01); delay_ms(5);
}

/* ---------------- MAIN ---------------- */

int main(void)
{
    f32 degc, degf;

    init_rtc();
    init_adc();
    init_lcd();
    init_kpm();
    init_spi();
    uart0_init();
    eint();

    cmdlcd(0x01); delay_ms(5); cmdlcd(0x80);
    strlcd("Connecting WiFi");
    cmdlcd(0xC0);
    strlcd("Please Wait...");
    esp01_connectAP();
    delay_s(2);

    IODIR0 |= 1 << BUZZER;

    collect_from_eeprom();

    if(my_atof(eeprom_read) <= 0.0)
        strcpy((char*)eeprom_read, DEFAULT_SET_POINT);

    strcpy((char *)set_point, (char *)eeprom_read);

    set_rtc_time(1, 57, 0);

    current_time     = SYNC_INTERVAL;
    last_upload_time = SYNC_INTERVAL;
    last_sync_time   = SYNC_INTERVAL;

    while(1)
    {
        if(flag == 1)
        {
            set_point_change();
            flag = 0;
        }

        read_lm35(&degc, &degf);
        sp = my_atof(set_point);

        if(sp <= 0.0)
        {
            strcpy((char*)set_point, DEFAULT_SET_POINT);
            sp = my_atof(set_point);
        }

        cmdlcd(0x80);
        strlcd("TEMP:");
        f32lcd(degc, 2);
        strlcd("C          ");

        cmdlcd(0xC0);
        strlcd("SP:");
        f32lcd(sp, 2);
        strlcd("C          ");

        if((current_time - last_upload_time) >= UPLOAD_INTERVAL)
        {
            esp01_sendToThingspeak(degc);
            last_upload_time = current_time;
        }

        if((current_time - last_sync_time) >= SYNC_INTERVAL)
        {
            esp01_readFromThingspeak();

         
            if(value[0] != '\0'
               && my_atof(value) > 0.0
               && strcmp((char*)value, (char*)set_point) != 0)
            {
                strcpy((char*)set_point, (char*)value);
                sp = my_atof(set_point);
                update_to_eeprom();
            }

            last_sync_time = current_time;
        }

        if(degc > sp)
            IOSET0 = 1 << BUZZER;
        else
            IOCLR0 = 1 << BUZZER;

        delay_s(1);
        current_time++;
    }
}
