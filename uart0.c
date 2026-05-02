#include <lpc21xx.h>
#include "pin_connect_block.h"
#include "pin_functions_definition.h"
#include "uart0_defines.h"
#include "uart0.h"
#include "types.h"

char buff[200] = "hello";
unsigned char i = 0, ch, r_flag;
char dummy;

// UART0 interrupt service routine
void uart0_isr(void) __irq
{
    if (U0IIR & 0x04) // Check if receive interrupt
    {
        ch = U0RBR; // Read to clear receive interrupt
        if (i < 200)
            buff[i++] = ch;
    }
    else
    {
        dummy = U0IIR; // Read to clear other (transmit) interrupt
    }
    VICVectAddr = 0; // Dummy write to acknowledge interrupt
}

// UART0 initialization
void uart0_init(void)
{
    // Set pin functions for UART0
    // cfgportpinfunc(0,0,TXD0_PIN_0_0);
    // cfgportpinfunc(0,1,RXD0_PIN_0_1);
    // U0LCR = WORD_LEN | (1 << DLAB_BIT);
    // U0DLL = DIVISOR;
    // U0DLM = DIVISOR >> 8;
    // U0LCR &= ~(1 << DLAB_BIT);

    PINSEL0 |= 0x00000005; // Enable RxD0 and TxD0

    U0LCR = 0x83;           // 8 bits, no Parity, 1 Stop bit (DLAB=1)
    U0DLL = DIVISOR;        // Baud rate low
    U0DLM = DIVISOR >> 8;   // Baud rate high
    U0LCR = 0x03;           // DLAB = 0

#if UART_INT_ENABLE > 0
    VICIntSelect = 0x00000000;                 // IRQ interrupt
    VICVectAddr0 = (unsigned)uart0_isr;        // ISR address
    VICVectCntl0 = 0x20 | 6;                   // Enable UART0 interrupt slot
    VICIntEnable = 1 << 6;                     // Enable UART0 interrupt
    U0IER = 0x03;                              // Enable RX and THRE interrupts
#endif
}

// Transmit a single character via UART0
void uart0_txchar(u8 txbyte)
{
    while (!(U0LSR & 0x20)); // Wait for transmitter holding register to be empty
    U0THR = txbyte;
}

// Receive a single character via UART0
u8 uart0_rxchar(void)
{
    while (!(U0LSR & 0x01)); // Wait for data ready
    return U0RBR;
}

// Transmit a null-terminated string via UART0
void uart0_str(u8 *s)
{
    while (*s)
        uart0_txchar(*s++);
}

// Transmit a floating-point number via UART0 (2 decimal places)
void uart0_float(f32 f)
{
    int x;
    float temp;
    x = f;
    uart0_int(x);
    uart0_txchar('.');
    temp = (f - x) * 100;
    x = temp;
    uart0_int(x);
}

// Transmit an unsigned integer via UART0
void uart0_int(u32 n)
{
    unsigned char a[10] = {0};
    int i = 0;
    if (n == 0)
    {
        uart0_txchar('0');
        return;
    }
    while (n > 0)
    {
        a[i++] = (n % 10) + '0';
        n = n / 10;
    }
    for (--i; i >= 0; i--)
    {
        uart0_txchar(a[i]);
    }
}
