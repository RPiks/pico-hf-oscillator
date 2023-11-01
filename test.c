#include <string.h>
#include <stdlib.h>

#include "piodco/piodco.h"
#include "build/dco.pio.h"
#include "hardware/vreg.h"

int main() 
{
    //vreg_set_voltage(VREG_VOLTAGE_1_25);
    set_sys_clock_khz(270000, true);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    const int offset = pio_add_program(pio0, &dco_program);
    const int ism = pio_claim_unused_sm(pio0, true);

    const int ipin = 6;

    dco_program_init(pio0, ism, offset, ipin);
    pio_sm_config smc = dco_program_get_default_config(offset);

    sm_config_set_set_pins(&smc, ipin, 1);
    pio_gpio_init(pio0, ipin);
    pio_sm_init(pio0, ism, offset, &smc);
    pio_sm_set_enabled(pio0, ism, true);

    for(;;)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(1000);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(1000);
    }
/*
    pcb->c = bitdco_program_get_default_config(offset);
    pcb->pio = pio;
    pcb->ism = ism;
    pcb->ipin = ipin;
    pcb->ioffset = offset;
    pcb->ifreq_hz = ifreqhz;
*/
}
