#include "piodco.h"

#include <string.h>
#include "../lib/assert.h"

int PioDCOInit(piodco *pdco, uint32_t ui32_clock_hz)
{
    assert_(pdco);

    memset(pdco, 0, sizeof(piodco));

    pdco->_clkfreq_hz = ui32_clock_hz;

    return 0;
}

int PioDCOSet(piodco *pdco, uint32_t ui32_frq_hz)
{
    assert_(pdco);
    assert(pdco->_clkfreq_hz);

    /* RPix: Calculate an accurate value of phase increment of the synth freq 
       per 1 tick of CPU clock, here 2^32 = 2PI. */
    const uint64_t frq_lsh32 = (pdco->_clkfreq_hz >> 1) + ((uint64_t)ui32_frq_hz << 32);
    pdco->_phase_increment = (uint32_t)(frq_lsh32 / (uint64_t)pdco->_clkfreq_hz);

    return 0;
}

void PioDCOStart(piodco *pdco)
{

}

void PioDCOStop(piodco *pdco)
{

}

void PioDCOTick(piodco *pdco)
{

}
