#include <string.h>
#include "uart0.h"
#include "delay.h"
#include "types.h"
#include "lcd.h"

extern u8 value[20];
extern char buff[400];
extern unsigned int i;

/* ================= CONNECT WIFI ================= */

void esp01_connectAP(void)
{
    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("ESP CHECK");
    uart0_str("AT\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(1000);

    cmdlcd(0x01); cmdlcd(0x80);
    if(strstr(buff,"OK")) strlcd("ESP READY");
    else                  strlcd("ESP ERROR");
    delay_ms(1000);

    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("ECHO OFF");
    uart0_str("ATE0\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(500);

    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("WIFI MODE");
    uart0_str("AT+CWMODE=1\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(1000);

    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("NET CONFIG");
    uart0_str("AT+CIPMUX=0\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(500);

    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("RESET WIFI");
    uart0_str("AT+CWQAP\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(1000);

    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("CONNECT WIFI");
    uart0_str("AT+CWJAP=\"Ayan\",\"1234567890\"\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(12000);

    cmdlcd(0x01); cmdlcd(0x80);
    if(strstr(buff,"WIFI CONNECTED") || strstr(buff,"OK"))
        strlcd("WIFI CONNECTED");
    else
        strlcd("WIFI FAILED");
    delay_ms(2000);
}

/* ================= SEND FIELD1 ================= */

void esp01_sendToThingspeak(f32 val)
{
    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("SENDING DATA");

    /* FIX: close stale TCP before new CIPSTART */
    uart0_str("AT+CIPCLOSE\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(500);

    uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(3000);

    if(strstr(buff,"CONNECT"))
    {
        uart0_str("AT+CIPSEND=52\r\n");
        delay_ms(500);

        uart0_str("GET /update?api_key=0V4LV45QS58V4QF6&field1=");
        uart0_float(val);
        uart0_str("\r\n\r\n");

        delay_ms(8000);

        cmdlcd(0x01); cmdlcd(0x80);
        if(strstr(buff,"SEND OK")) strlcd("DATA SENT OK");
        else                       strlcd("SEND FAILED");

        uart0_str("AT+CIPCLOSE\r\n");
        delay_ms(500);
    }
    else
    {
        cmdlcd(0x01); cmdlcd(0x80);
        strlcd("TCP FAILED");
        delay_ms(2000);
    }
}

/* ================= SEND FIELD2 ================= */

void esp01_sendToThingspeak_set_point(float val)
{
    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("SENDING DATA");

    /* FIX: close stale TCP before new CIPSTART */
    uart0_str("AT+CIPCLOSE\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(500);

    uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(3000);

    if(strstr(buff,"CONNECT"))
    {
        uart0_str("AT+CIPSEND=52\r\n");
        delay_ms(500);

        uart0_str("GET /update?api_key=0V4LV45QS58V4QF6&field2=");
        uart0_float(val);
        uart0_str("\r\n\r\n");

        delay_ms(8000);

        cmdlcd(0x01); cmdlcd(0x80);
        if(strstr(buff,"SEND OK")) strlcd("DATA SENT OK");
        else                       strlcd("SEND FAILED");

        uart0_str("AT+CIPCLOSE\r\n");
        delay_ms(500);
    }
    else
    {
        cmdlcd(0x01); cmdlcd(0x80);
        strlcd("TCP FAILED");
        delay_ms(2000);
    }
}

/* ================= READ FIELD2 ================= */

void esp01_readFromThingspeak(void)
{
    char *ptr = NULL, *p = NULL;

    memset(value, 0, sizeof(value));

    cmdlcd(0x01); cmdlcd(0x80);
    strlcd("READING DATA");

 
    uart0_str("AT+CIPCLOSE\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(500);

    uart0_str("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    i = 0; memset(buff, 0, sizeof(buff));
    delay_ms(3000);

    if(strstr(buff,"CONNECT"))
    {
        uart0_str("AT+CIPSEND=66\r\n");
        delay_ms(500);

        uart0_str("GET /channels/3246033/feeds/last.json?api_key=LC3FVAJ49NHP4OKB\r\n\r\n");

        /* FIX 2: increased wait — ThingSpeak JSON response needs more time */
        delay_ms(8000);

        /* FIX 3: safe null-terminate buff before any strstr */
        if(i < 399) buff[i] = '\0';
        else        buff[399] = '\0';

        ptr = strstr(buff, "\"field2\":\"");
        if(ptr != NULL)
        {
            ptr += 10;
            p = strchr(ptr, '\"');
            if(p != NULL)
            {
                *p = '\0';
                strncpy((char*)value, ptr, 19);
                value[19] = '\0';

                cmdlcd(0x01); cmdlcd(0x80);
                strlcd((s8*)value);
            }
            else
            {
                /* FIX 4: show error instead of blank LCD */
                cmdlcd(0x01); cmdlcd(0x80);
                strlcd("PARSE ERR");
            }
        }
        else
        {
            /* FIX 4: show error instead of blank LCD */
            cmdlcd(0x01); cmdlcd(0x80);
            strlcd("NO DATA");
        }

        delay_ms(2000);

        uart0_str("AT+CIPCLOSE\r\n");
        delay_ms(500);
    }
    else
    {
        cmdlcd(0x01); cmdlcd(0x80);
        strlcd("TCP FAILED");
        delay_ms(2000);
    }
}
