#include <string.h>
#include <stdlib.h>

#include "defines.h"

#include "piodco/piodco.h"
#include "build/dco.pio.h"
#include "hardware/vreg.h"

#include "./lib/assert.h"
#include "hwdefs.h"
#include "./dcoisr/dcoisr.h"

void __not_in_flash_func (WorkerCycle)(PioDco *pDCO, const int32_t i32frq_in_clk, uint32_t *preg32)
{
    register int32_t acc_phase_error = 0;
    register uint8_t *preg8 = (uint8_t *)preg32;
    register PIO pio = pDCO->_pio;
    register uint sm = pDCO->_ism;

    pio_sm_set_enabled(pio, sm, true);

    for(;;)
    {
        preg8 = (uint8_t *)preg32;
        for(int i = 0; i < 32; ++i)
        {
            register const int32_t i32wc = (i32frq_in_clk - acc_phase_error + (1<<23)) >> 24;
            acc_phase_error += (i32wc<<24) - i32frq_in_clk;

            *preg8++ = i32wc - PIOASM_DELAY_CYCLES;
        }

        dco_program_puts(pio, sm, preg32);
    }
}

int main() 
{
    PioDco DCO;
    //DcoIsrContext ISRC;

    const uint32_t clkhz = PLL_SYS_MHZ * 1000000L;
    set_sys_clock_khz(clkhz / 1000L, true);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    const uint32_t freq_hz = 2*10000000L;

    assert_(0 == PioDCOInit(&DCO, 6, clkhz));
    PioDCOStart(&DCO);
    assert_(0 == PioDCOSetFreq(&DCO, freq_hz));
    PioDCOWorker(&DCO);

/*
    //DcoIsrInit(&ISRC, &DCO);

    uint32_t preg32[8] = {0};
    uint8_t *preg8 = (uint8_t *)preg32;
    
    //const uint64_t frqhz = 2*(3573000L);
    //const uint64_t frqhz = 2*7074000L + 1;
    const uint64_t frqhz = 2*10000000L;
    //const uint64_t frqhz = 2*10136000L;
    //const int64_t frqhz = 2*(14074000L);
    const int64_t i64frq_in_clk = ((int64_t)clkhz * (int64_t)(1<<24) + (frqhz>>1)) / frqhz;
    const int32_t i32frq_in_clk = i64frq_in_clk;

    WorkerCycle(&DCO, i32frq_in_clk, preg32);
*/
}
