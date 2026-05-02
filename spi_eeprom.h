#include"types.h"
void write_enable(void);
void spi_eeprom_byte_write(u16 w_buff_addr,u8 sdat);
u8 spi_eeprom_byte_read(u16 r_buff_addr);
