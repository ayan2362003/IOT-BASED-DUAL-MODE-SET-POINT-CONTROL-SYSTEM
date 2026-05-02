#include "types.h"
#include <lpc21xx.h>
#include "pin_connect_block.h"
#include "pin_functions_definition.h"
#include "spi_defines.h"

// Initialize SPI Peripheral
void init_spi(void)
{
    // Configure SPI pins: SCK, MISO, MOSI
    cfgportpinfunc(0, 4, SCK_0_4);
    cfgportpinfunc(0, 5, MISO_0_5);
    cfgportpinfunc(0, 6, MOSI_0_6);

    // Set CS pin (chip select) high and as output
    IOSET0 = 1 << CS_0_7;
    IODIR0 |= 1 << CS_0_7;

    // Set SPI clock divider (must be even and >= 8)
    if (SPI_DIVIDER >= 8)
    {
        // If divider is odd, increment to make it even
        S0SPCCR = (SPI_DIVIDER % 2 == 0) ? SPI_DIVIDER : SPI_DIVIDER + 1;
    }

    // Configure SPI control register: Master mode, CPOL, CPHA enabled
    S0SPCR = (1 << MSTR_BIT) | (1 << CPOL_BIT) | (1 << CPHA_BIT);
}

// Exchange a single byte over SPI (full-duplex)
u8 spi(u8 sbyte)
{
    u8 rbyte;
    S0SPDR = sbyte; // Load byte to SPI data register
    while (((S0SPSR >> SPIF_BIT) & 1) == 0); // Wait for transfer complete
    rbyte = S0SPDR; // Read received byte
    return rbyte;
}
