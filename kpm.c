#include "kpm_defines.h"
#include "kpm.h"
#include <lpc21xx.h>
#include "types.h"

// Keypad lookup tables
u32 kpmlut[4][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
};

u32 kpm_lut[4][4] = {
    {'1', '2', '3', '/'},
    {'4', '5', '6', '*'},
    {'7', '8', '9', '-'},
    {'e', '0', '.', '+'}
};

// Initialize keypad: set row lines as output on port 1
void init_kpm(void)
{
    IODIR1 |= (0xF << ROW0); // Set 4 rows as output
}

// Scan columns for key press, return 0 if pressed, 1 otherwise
u32 colscan(void)
{
    u32 status = (((IOPIN1 >> COL0) & 0xF) < 0xF) ? 0 : 1;
    return status;
}

// Activate each row, check for column key press, return row index
u32 rowcheck(void)
{
    u32 r;
    for (r = 0; r <= 3; r++)
    {
        // Set only row r low, rest high
        IOPIN1 = (IOPIN1 & ~(0xF << ROW0)) | (~(1 << r) << ROW0);

        if (colscan() == 0)
        {
            break;
        }
    }
    IOCLR1 = 0xF << ROW0; // Clear rows after check
    return r;
}

// Check columns, return column index where key is pressed
u32 colcheck(void)
{
    u32 c;
    for (c = 0; c <= 3; c++)
    {
        if (((IOPIN1 >> (COL0 + c)) & 1) == 0)
        {
            break;
        }
    }
    return c;
}

// Scan keypad, return ASCII value of pressed key
u32 keyscan(void)
{
    u32 r, c, keyv;
    init_kpm();
    while (colscan()); // Wait for no key pressed
    r = rowcheck();
    c = colcheck();
    keyv = kpm_lut[r][c];
    return keyv;
}

/*
Alternative implementation for port 0 (commented, not used):

void init_kpm(void)
{
    IODIR0 |= (0xF << ROW0);
}
u32 colscan(void)
{
    u32 status = (((IOPIN0 >> COL0) & 0xF) < 0xF) ? 0 : 1;
    return status;
}
u32 rowcheck(void)
{
    u32 r;
    for (r = 0; r <= 3; r++)
    {
        IOPIN0 = (IOPIN0 & ~(0xF << ROW0)) | (~(1 << r) << ROW0);

        if (colscan() == 0)
        {
            break;
        }
    }
    IOCLR0 = 0xF << ROW0;
    return r;
}
u32 colcheck(void)
{
    u32 c;
    for (c = 0; c <= 3; c++)
    {
        if (((IOPIN0 >> (COL0 + c)) & 1) == 0)
        {
            break;
        }
    }
    return c;
}
u32 keyscan(void)
{
    u32 r, c, keyv;
    init_kpm();
    while (colscan());
    r = rowcheck();
    c = colcheck();
    keyv = kpm_lut[r][c];
    return keyv;
}
*/
