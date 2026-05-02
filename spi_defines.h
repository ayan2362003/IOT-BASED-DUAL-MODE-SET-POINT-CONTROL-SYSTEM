#define CPHA_BIT  3
#define CPOL_BIT 4
#define MSTR_BIT 5
#define LSBF_BIT 6
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define SPI_CLK 100000

#define SPI_DIVIDER (PCLK/SPI_CLK)
