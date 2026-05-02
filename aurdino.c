#include <lpc21xx.h>
#include "aurdino.h"
#include "types.h"

// Set pin direction for a single pin
void pinmode(u32 pinno, u32 pindir)
{
    if (pinno < 32) // Port 0: pins 0–31
    {
        if (pindir == 1)         // Output
            IODIR0 |= (1 << pinno);
        else                     // Input
            IODIR0 &= ~(1 << pinno);
    }
    else if (pinno <= 47)        // Port 1: pins 32–47
    {
        u32 adj_pin = pinno - 32;
        if (pindir == 1)
            IODIR1 |= (1 << adj_pin);
        else
            IODIR1 &= ~(1 << adj_pin);
    }
}

// Write single pin (digital output)
void digitalwrite(u32 pinno, u32 bit)
{
    if (pinno < 32)
    {
        IOPIN0 = (IOPIN0 & ~(1 << pinno)) | (bit << pinno);
    }
    else if (pinno <= 47)
    {
        u32 adj_pin = pinno - 32;
        IOPIN1 = (IOPIN1 & ~(1 << adj_pin)) | (bit << adj_pin);
    }
}

// Read logic level from a pin (digital input)
u32 digitalread(u32 pinno)
{
    u32 data = 0;
    if (pinno < 32)
    {
        data = (IOPIN0 >> pinno) & 1;
    }
    else if (pinno <= 47)
    {
        u32 adj_pin = pinno - 32;
        data = (IOPIN1 >> adj_pin) & 1;
    }
    return data;
}

// Set direction for a group of pins, starting at pinstartno
void portmode(u32 pinstartno, u32 npins, u32 pindir)
{
    u32 mask = ((1 << npins) - 1);
    if (pinstartno < 32)
    {
        mask <<= pinstartno;
        if (pindir == 1)
            IODIR0 |= mask;
        else
            IODIR0 &= ~mask;
    }
    else if (pinstartno <= 47)
    {
        u32 adj_pin = pinstartno - 32;
        mask <<= adj_pin;
        if (pindir == 1)
            IODIR1 |= mask;
        else
            IODIR1 &= ~mask;
    }
}

// Write a value to a group of pins (parallel output)
void write2pins(u32 pinstartno, u32 npins, u32 data)
{
    u32 mask = ((1 << npins) - 1);
    if (pinstartno < 32)
    {
        mask <<= pinstartno;
        IOPIN0 = (IOPIN0 & ~mask) | ((data << pinstartno) & mask);
    }
    else if (pinstartno <= 47)
    {
        u32 adj_pin = pinstartno - 32;
        mask <<= adj_pin;
        IOPIN1 = (IOPIN1 & ~mask) | ((data << adj_pin) & mask);
    }
}

// Read value from a group of pins (parallel input)
u32 readpins(u32 pinstartno, u32 npins)
{
    u32 mask = ((1 << npins) - 1);
    u32 data = 0;
    if (pinstartno < 32)
    {
        data = (IOPIN0 >> pinstartno) & mask;
    }
    else if (pinstartno <= 47)
    {
        u32 adj_pin = pinstartno - 32;
        data = (IOPIN1 >> adj_pin) & mask;
    }
    return data;
}
