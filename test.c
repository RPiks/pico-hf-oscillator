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

    const uint32_t clkhz = PLL_SYS_MHZ * 1000000L;
    set_sys_clock_khz(clkhz / 1000L, true);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    assert_(0 == PioDCOInit(&DCO, PLL_SYS_MHZ * 1000000L));  
    //DcoIsrInit(&ISRC, &DCO);

    uint32_t preg32[8] = {0};
    uint8_t *preg8 = (uint8_t *)preg32;
    
    const uint64_t frqhz = 2*(3573000L);
    //const uint64_t frqhz = 2*(7074000L);
    const uint64_t u64frq_in_clk_E20 = ((uint64_t)clkhz * (uint64_t)(1<<20) + (frqhz>>1)) / frqhz;
    uint32_t u32frq_in_clk_E20 = u64frq_in_clk_E20;

    int32_t acc_phase_discriminator = 0;
    int tick;
    for(;;)
    {
        ++tick;
        for(int i = 0; i < 8; ++i)
        {
            const int32_t i32wc = (u32frq_in_clk_E20 - acc_phase_discriminator + (1<<19)) >> 20;
            acc_phase_discriminator += (i32wc<<20) - u32frq_in_clk_E20;

            preg32[i] = i32wc - 4;
        }

        dco_program_puts(DCO._pio, DCO._ism, preg32);

        if(tick > 10000000L)
        {
            tick = 0;
        }

        u32frq_in_clk_E20 = u64frq_in_clk_E20 + tick/10000;

    }
}
