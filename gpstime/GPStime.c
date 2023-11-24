///////////////////////////////////////////////////////////////////////////////
//
//  Roman Piksaykin [piksaykin@gmail.com], R2BDY, PhD
//  https://www.qrz.com/db/r2bdy
//
///////////////////////////////////////////////////////////////////////////////
//
//
//  gpstime.h - GPS time reference utilities for digital controlled radio freq
//              oscillator based on Raspberry Pi Pico.
//
//  DESCRIPTION
//
//      GPS time utilities for PioDco oscillator provides a precise reference
//  frequency in order to obtain an absolute accuracy of PioDco. The value of
//  this  accuracy  depends  on quality of navigation solution of GPS receiver.
//  This quality can be estimated by GDOP (geometric dilution of precision) and
//  TDOP (time dilution of precision) received in NMEA-0183 message packet from
//  GPS receiver.
//      The Pico's frequency error due to drift of its CLK oscillator is
//  calculated by that utilities and is used to shift PioDco frequency to
//  compensate the drift. So, the absolute frequency value which we possess
//  on the GPIO pin is supposed to be much more accurate when using GPS time
//  utilities.
//      Owing to the meager PioDco frequency step in millihertz range, we obtain
//  a quasi-analog precision frequency source.
//      This is an experimental project of amateur radio class and it is devised
//  by me on the free will base in order to experiment with QRP narrowband
//  digital modes including extremely ones such as QRSS.
//      I gracefully appreciate any thoughts or comments on that matter.
//
//  PLATFORM
//      Raspberry Pi pico.
//
//  REVISION HISTORY
// 
//      Rev 0.1   25 Nov 2023   Initial release
//
//  PROJECT PAGE
//      https://github.com/RPiks/pico-hf-oscillator
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
#include "GPStime.h"

static GPStimeData *spGPStimeData = NULL;

GPStimeContext *GPStimeInit(int uart_id, int uart_baud, int pps_gpio)
{
    ASSERT_(0 == uart_id || 1 == uart_id);
    ASSERT_(uart_baud <= 115200);
    ASSERT_(pps_gpio < 29);

    // Set up our UART with the required speed.
    uart_init(uart_id ? uart1 : uart0, uart_baud);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(uart_id ? 8 : 12, GPIO_FUNC_UART);
    gpio_set_function(uart_id ? 9 : 13, GPIO_FUNC_UART);

    GPStimeContext *pgt = calloc(1, sizeof(GPStimeContext));
    ASSERT_(pgt);
    pgt->_uart_id = uart_id;
    pgt->_uart_baudrate = uart_baud;
    pgt->_pps_pio = pps_gpio;

    spGPStimeData = &pgt->_time_data;

    gpio_init(pps_gpio);
    gpio_set_dir(pps_gpio, GPIO_IN);
    gpio_set_irq_enabled_with_callback(pps_gpio, GPIO_IRQ_EDGE_RISE, true, &GPStimePPScallback);
}

void GPStimeDestroy(GPStimeContext **pp)
{
    ASSERT_(pp);
    ASSERT_(*pp);

    uart_deinit((*pp)->_uart_id ? uart1 : uart0);
    free(*pp);
    *pp = NULL;
}

void __not_in_flash_func (GPStimePPScallback)(uint gpio, uint32_t events)
{   
    if(spGPStimeData)
    {
        const uint64_t tm64 = GetUptime64();
        spGPStimeData->_u64_sysclk_pps_last = tm64;
        const int32_t ksliding_len = sizeof(spGPStimeData->_pu64_sliding_pps_tm) 
                                   / sizeof(spGPStimeData->_pu64_sliding_pps_tm[0]);
        
        ++spGPStimeData->_ix_last;
        spGPStimeData->_ix_last %= ksliding_len;
        const uint8_t ix_oldest = spGPStimeData->_ix_last;

        int64_t dt_1M = 1000000LL * (tm64 - spGPStimeData->_pu64_sliding_pps_tm[ix_oldest]);
        spGPStimeData->_pu64_sliding_pps_tm[spGPStimeData->_ix_last] = tm64;

        if(spGPStimeData->_u64_pps_period_1M)
        {
            spGPStimeData->_u64_pps_period_1M += iSAR(dt_1M - spGPStimeData->_u64_pps_period_1M + 4, 3);
        }
        else
        {
            spGPStimeData->_u64_pps_period_1M = dt_1M;
        }
#if 1
    dt_1M = (dt_1M + ksliding_len/2) / ksliding_len;
    const uint64_t tmp = (spGPStimeData->_u64_pps_period_1M + ksliding_len/2) / ksliding_len;
    printf("%llu %lld %llu\n", spGPStimeData->_u64_sysclk_pps_last, dt_1M, tmp);
#endif

    }
}
