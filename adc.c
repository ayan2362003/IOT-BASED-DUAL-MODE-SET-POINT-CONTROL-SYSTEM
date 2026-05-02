#include "types.h"
#include <lpc21xx.h>
#include "delay.h"
#include "adc_defines.h"
#include "pin_connect_block.h"
#include "pin_functions_definition.h"

/* ================= ADC INITIALIZATION ================= */

void init_adc(void)
{
    /* Configure P0.28 as AD0.1 (LM35 input for example) */
    cfgportpinfunc(0, 28, AINO_0_27);
	
   
    ADCR = (1 << PDN_BIT) | (CLKDIV << CLKDIV_BITS);
}

/* ================= ADC READ FUNCTION ================= */

void read_adc(u32 ch_no, u32 *adcdval, f32 *analog_read)
{
    /* Clear only channel select bits (SEL bits 0–7) */
    ADCR &= ~0xFF;

    /* Select channel */
    ADCR |= (1 << ch_no);

    /* Start ADC conversion (START = 001) */
    ADCR |= (1 << START_CONV_BIT);

    /* Wait till conversion complete */
    while(((ADDR >> DONE_BIT) & 1) == 0);

    /* Stop conversion */
    ADCR &= ~(1 << START_CONV_BIT);

    /* Read 10-bit ADC result */
    *adcdval = (ADDR >> RESULT_BITS) & 0x3FF;

    /* Convert to voltage (3.3V reference) */
    *analog_read = (*adcdval) * (3.3f / 1023.0f);
}
