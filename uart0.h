#include"types.h"
void uart0_init(void);
void uart0_txchar(u8 txbyte);
u8 uart0_rxchar(void);
void uart0_str(u8 *);
void uart0_int(u32 n);
void uart0_float(f32 f);

