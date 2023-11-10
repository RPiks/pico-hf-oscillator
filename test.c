///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  test.c - Simple test of digital controlled radio freq oscillator based on PIO.
// 
//
//  DESCRIPTION
//
//      The oscillator provides precise generation of any frequency ranging
//  from 1.1 to 9.4 MHz with tenth's of millihertz resolution (please note that
//  this is relative resolution owing to the fact that the absolute accuracy of 
//  onboard crystal of pi pico is limited).
//      The DCO uses phase locked loop principle programmed in C.
//      The DCO does *NOT* use any floating point operations - all time-critical
//  instructions run in 1 CPU cycle.
//      Currently the upper freq. limit is about 9.8 MHz and it is achieved only
//  using pi pico overclocking to 270MHz.
//      Owing to the meager frequency step, it is possible to use 3, 5, or 7th
//  harmonics of generated frequency. Such solution completely cover all HF and
//  low band up to about 66 MHz.
//      This is an experimental project of amateur radio class and it is devised
//  by me on the free will base in order to experiment with QRP narrowband
//  digital modes.
//      I gracefully appreciate any thoughts or comments on that matter.
//
//  HOWTOSTART
//      Set frequency by #define GEN_FRQ_HZ and build the project. The default 
//  output pin is GPIO6.
//
//  PLATFORM
//      Raspberry Pi pico.
//
//  REVISION HISTORY
// 
//      Rev 0.1   05 Nov 2023
//  Initial release.
//
//  LICENCE
//      MIT License (http://www.opensource.org/licenses/mit-license.php)
//
//  Copyright (c) 2023 by Roman Piksaykin
//  
//  Permission is hereby granted, free of charge,to any person obtaining a copy
//  of this software and associated documentation files (the Software), to deal
//  in the Software without restriction,including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY,WHETHER IN AN ACTION OF CONTRACT,TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdlib.h>

#include "defines.h"

#include "piodco/piodco.h"
#include "build/dco.pio.h"
#include "hardware/vreg.h"
#include "pico/multicore.h"

#include "./lib/assert.h"
#include "hwdefs.h"

#define GEN_FRQ_HZ 9400000L

PioDco DCO;

/* This is the code of dedicated core. 
   We deal with extremely precise real-time task. */
void core1_entry()
{
    const uint32_t clkhz = PLL_SYS_MHZ * 1000000L;
    const uint32_t freq_hz = GEN_FRQ_HZ;

    /* Initialize DCO */
    assert_(0 == PioDCOInit(&DCO, 6, clkhz));

    /* Run DCO. */
    PioDCOStart(&DCO);

    /* Set initial freq. */
    assert_(0 == PioDCOSetFreq(&DCO, freq_hz));

    /* Run the main DCO algorithm. It spins forever. */
    PioDCOWorker(&DCO);
}

void RAM (Spinner)(void)
{
    int i = 0;
    for(;;)
    {
        /* This example sets new frequency every ~500 ms.
           Frequency shift is 5 Hz for each step.
        */
        PioDCOSetFreq(&DCO, GEN_FRQ_HZ - 5*i);

        /* LED signal */
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(500);

        /* Return to initial freq after 20 steps (100 Hz). */
        if(++i == 20)
            i = 0;
    }
}

int main() 
{
    const uint32_t clkhz = PLL_SYS_MHZ * 1000000L;
    set_sys_clock_khz(clkhz / 1000L, true);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    multicore_launch_core1(core1_entry);

    Spinner();
}
