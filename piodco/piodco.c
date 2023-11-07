#include "piodco.h"

#include <string.h>
#include "../lib/assert.h"

#include "build/dco.pio.h"

int PioDCOInit(PioDco *pdco, uint32_t ui32_clock_hz)
{
    assert_(pdco);

    memset(pdco, 0, sizeof(PioDco));

    pdco->_clkfreq_hz = ui32_clock_hz;
    pdco->_pio = pio0;
    pdco->_gpio = 6;
    pdco->_offset = pio_add_program(pdco->_pio, &dco_program);
    pdco->_ism = pio_claim_unused_sm(pdco->_pio, true);

    dco_program_init(pdco->_pio, pdco->_ism, pdco->_offset, pdco->_gpio);
    pdco->_pio_sm = dco_program_get_default_config(pdco->_offset);

    sm_config_set_set_pins(&pdco->_pio_sm, pdco->_gpio, 1);
    pio_gpio_init(pdco->_pio, pdco->_gpio);
    pio_sm_init(pdco->_pio, pdco->_ism, pdco->_offset, &pdco->_pio_sm);
    pio_sm_set_enabled(pdco->_pio, pdco->_ism, true);

    return 0;
}

int PioDCOSet(PioDco *pdco, uint32_t ui32_frq_hz)
{
    assert_(pdco);
    assert(pdco->_clkfreq_hz);

    /* RPix: Calculate an accurate value of phase increment of the synth freq 
       per 1 tick of CPU clock, here 2^32 = 2PI. */
    const uint64_t frq_lsh32 = (pdco->_clkfreq_hz >> 1) + ((uint64_t)ui32_frq_hz << 32);
    pdco->_phase_increment = (uint32_t)(frq_lsh32 / (uint64_t)pdco->_clkfreq_hz);

    return 0;
}

void PioDCOStart(PioDco *pdco)
{

}

void PioDCOStop(PioDco *pdco)
{

}

void PioDCOTick(PioDco *pdco)
{

}
