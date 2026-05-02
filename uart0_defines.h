
//#define TXD0_PIN_0_0 pin_func2 
//#define RXD0_PIN_0_1 pin_func2 
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK  (CCLK/4)
#define BAUD 9600
#define DIVISOR (PCLK/(16*BAUD))
//#define _8BIT 3
//#define WORD_LEN  _8BIT
//#define DLAB_BIT 7
//#define RDR_BIT 0
//#define THRE_BIT 5
//#define TEMT_BIT 6
#define UART_INT_ENABLE 1
