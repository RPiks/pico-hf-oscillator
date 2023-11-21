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
#ifndef GPSTIME_H_
#define GPSTIME_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "../lib/assert.h"

#define ASSERT_(x) assert_(x)

typedef struct
{
    uint32_t _tm_unix_last;
    float _flt_lat_deg, _flt_lon_deg;

    float _flt_GDOP, _flt_TDOP;

} GPStimeData;

typedef struct
{
    int _uart_id;
    int _uart_baudrate;
    int _pps_pio;

    GPStimeData _time_data;

    uint8_t _pbytebuff[256];
    uint8_t _is_sentence_ready;

} GPStimeContext;

GPStimeContext *GPStimeInit(int uart_id, int uart_baud, int pps_pio);
void GPStimeDestroy(GPStimeContext **pp);

void GPStimeTick(GPStimeContext *pg);

#endif
