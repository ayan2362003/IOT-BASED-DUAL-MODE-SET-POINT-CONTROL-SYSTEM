#include "pin_connect_block.h"
#include "pin_functions_definition.h"
#include "spi_eeprom_defines.h"
#include "types.h"
#include <lpc21xx.h>
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "spi.h"

// Enable EEPROM write operation
void write_enable(void)
{
    IOCLR0 = 1 << CS_0_7;
    spi(WREN);
    IOSET0 = 1 << CS_0_7;
}

// Disable EEPROM write operation
void write_disable(void)
{
    IOCLR0 = 1 << CS_0_7;
    spi(WRDI);
    IOSET0 = 1 << CS_0_7;
}

// Write a single byte to SPI EEPROM
void spi_eeprom_byte_write(u16 w_buff_addr, u8 sdat)
{
    write_enable();
    IOCLR0 = 1 << CS_0_7;
    spi(WRITE);
    spi((w_buff_addr >> 8) & 0xFF);   // Address high byte
    spi(w_buff_addr & 0xFF);          // Address low byte
    spi(sdat);
    IOSET0 = 1 << CS_0_7;
    delay_ms(5);                      // Wait for EEPROM write complete
    write_disable();
}

// Read a single byte from SPI EEPROM
u8 spi_eeprom_byte_read(u16 r_buff_addr)
{
    u8 rdat;
    IOCLR0 = 1 << CS_0_7;
    spi(READ);
    spi((r_buff_addr >> 8) & 0xFF); // Address high byte
    spi(r_buff_addr & 0xFF);        // Address low byte
    rdat = spi(0x00);               // Read byte
    IOSET0 = 1 << CS_0_7;
    return rdat;
}
