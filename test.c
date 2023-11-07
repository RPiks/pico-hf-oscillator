#include <string.h>
#include <stdlib.h>

#include "piodco/piodco.h"
#include "build/dco.pio.h"
#include "hardware/vreg.h"

#include "./lib/assert.h"
#include "hwdefs.h"
#include "./dcoisr/dcoisr.h"

int main() 
{
    PioDco DCO;
    DcoIsrContext ISRC;

    set_sys_clock_khz(PLL_SYS_MHZ * 1000L, true);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    assert_(0 == PioDCOInit(&DCO, PLL_SYS_MHZ * 1000000L));  
    //DcoIsrInit(&ISRC, &DCO);

    uint32_t pval[8];
    memset((char *)pval, 0x00, 32);
    for(;;)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        dco_program_puts(DCO._pio, DCO._ism, pval);
        
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        dco_program_puts(DCO._pio, DCO._ism, pval);
    }

/*
    const int offset = pio_add_program(pio0, &dco_program);
    const int ism = pio_claim_unused_sm(pio0, true);

    const int ipin = 6;

    dco_program_init(pio0, ism, offset, ipin);
    pio_sm_config smc = dco_program_get_default_config(offset);

    sm_config_set_set_pins(&smc, ipin, 1);
    pio_gpio_init(pio0, ipin);
    pio_sm_init(pio0, ism, offset, &smc);
    pio_sm_set_enabled(pio0, ism, true);

    uint32_t pval[8];
    memset(pval, 0x10, 4*8);
    for(;;)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        dco_program_puts(pio0, ism, pval);

        //sleep_ms(1000);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        //sleep_ms(1000);
    }
    */
/*
    pcb->c = bitdco_program_get_default_config(offset);
    pcb->pio = pio;
    pcb->ism = ism;
    pcb->ipin = ipin;
    pcb->ioffset = offset;
    pcb->ifreq_hz = ifreqhz;
*/
}
