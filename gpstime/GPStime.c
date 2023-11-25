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
//      GPS time utilities for PioDco oscillator calculates a precise frequency
//  between the local Pico oscillator and reference oscillator of GPS system.
//  The value of the shift is used to correct PioDco generated frequency. The
//  practical precision of this solution within tenths millihertz range.
//  The value of this accuracy depends  on quality of navigation solution of GPS 
//  receiver. This quality can be estimated by GDOP and TDOP parameters received 
//  in NMEA-0183 message packet from GPS receiver.
//      Owing to the meager PioDco frequency step in millihertz range, we obtain
//  a quasi-analog precision frequency source (if the GPS navigation works ok).
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

static GPStimeContext *spGPStimeContext = NULL;
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
    pgt->_pps_gpio = pps_gpio;

    spGPStimeContext = pgt;
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
    const uint64_t tm64 = GetUptime64();
    if(spGPStimeData)
    {
        spGPStimeData->_u64_sysclk_pps_last = tm64;   
        ++spGPStimeData->_ix_last;
        spGPStimeData->_ix_last %= eSlidingLen;

        const int64_t dt_per_window = tm64 - spGPStimeData->_pu64_sliding_pps_tm[spGPStimeData->_ix_last];
        spGPStimeData->_pu64_sliding_pps_tm[spGPStimeData->_ix_last] = tm64;

        if(ABS(dt_per_window - eCLKperTimeMark * eSlidingLen) < eMaxCLKdevPPM * eSlidingLen)
        {
            if(spGPStimeData->_u64_pps_period_1M)
            {
                spGPStimeData->_u64_pps_period_1M += iSAR64((int64_t)eDtUpscale * dt_per_window 
                                                            - spGPStimeData->_u64_pps_period_1M + 2, 2);
                spGPStimeData->_i32_freq_shift_ppb = (spGPStimeData->_u64_pps_period_1M
                                                      - (int64_t)eDtUpscale * eCLKperTimeMark * eSlidingLen
                                                      + (eSlidingLen >> 1)) / eSlidingLen;
            }
            else
            {
                spGPStimeData->_u64_pps_period_1M = (int64_t)eDtUpscale * dt_per_window;
            }
        }

#ifdef DEBUGLOG
        const int64_t dt_1M = (dt_per_window + (eSlidingLen >> 1)) / eSlidingLen;
        const uint64_t tmp = (spGPStimeData->_u64_pps_period_1M + (eSlidingLen >> 1)) / eSlidingLen;
        printf("%llu %lld %llu %lld\n", spGPStimeData->_u64_sysclk_pps_last, dt_1M, tmp, 
               spGPStimeData->_i32_freq_shift_ppb);
#endif

    }
}

/// @brief Calculates current unixtime using information available.
/// @param pg Ptr to the context.
/// @param u32_tmdst Ptr to destination unixtime val.
/// @return 0 if OK.
/// @return -1 There was NO historical GPS fixes.
/// @return -2 The fix was expired (24hrs or more time ago).
int GPStimeGetTime(const GPStimeContext *pg, uint32_t *u32_tmdst)
{
    assert_(pg);
    assert(u32_tmdst);

    /* If there has been no fix, it's no way to get any time data... */
    if(!pg->_time_data._u32_utime_nmea_last)
    {
        return -1;
    }

    const uint64_t tm64 = GetUptime64();
    const uint64_t dt = tm64 - pg->_time_data._u64_sysclk_nmea_last;
    const uint32_t dt_sec = PicoU64timeToSeconds(dt);

    /* If expired. */
    if(dt_sec > 86400)
    {
        return -2;
    }

    *u32_tmdst = pg->_time_data._u32_utime_nmea_last + dt_sec;

    return 0;
}

void __not_in_flash_func (GPStimeUartRxIsr)()
{
    if(spGPStimeContext)
    {
        uart_inst_t *puart_id = spGPStimeContext->_uart_id ? uart1 : uart0;
        for(;;uart_is_readable(puart_id))
        {
            uint8_t chr = uart_getc(puart_id);
            spGPStimeContext->_pbytebuff[spGPStimeContext->_u8_ixw++] = chr;
            spGPStimeContext->_is_sentence_ready = ('\n' == chr);
            break;
        }

        if(spGPStimeContext->_is_sentence_ready)
        {
            spGPStimeContext->_i32_error_count -= GPStimeProcNMEAsentence(spGPStimeContext);
        }
    }
}

int GPStimeProcNMEAsentence(GPStimeContext *pg)
{
    assert_(pg);

    uint8_t *prmc = strnstr(pg->_pbytebuff, "$GPRMC", sizeof(pg->_pbytebuff));
    if(prmc)
    {
        uint8_t u8ixcollector[16], chksum = '$'^'G'^'P'^'R'^'M'^'C'^',';
        for(uint8_t u8ix = prmc - pg->_pbytebuff + 7, i = 0;
            u8ix != prmc - pg->_pbytebuff; ++u8ix)
        {
            chksum ^= pg->_pbytebuff[u8ix];
            if(',' == pg->_pbytebuff[u8ix])
            {
                pg->_pbytebuff[u8ix] = 0;
                u8ixcollector[i++] = u8ix;
                if(12 == i)
                {
                    break;
                }
            }
        }
    }

    /*
        "$GPRMC,105954.000,A,3150.6731,N,11711.9399,E,0.00,96.10,250313,,,A*53";
    */
    return 0;
}
