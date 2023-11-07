#ifndef PIODCO_H_
#define PIODCO_H_

#include <stdbool.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

typedef struct
{
    PIO _pio;                   /* Worker PIO on this DCO. */
    int _gpio;                  /* Pico' GPIO for DCO output. */

    pio_sm_config _pio_sm;      /* Worker PIO parameter. */
    int _ism;                   /* Index of state maschine. */
    int _offset;                /* Worker PIO u-program offset. */

    uint32_t _phase_prompt;     /* Last DCO phase val, [0...2PI). */
    uint32_t _phase_increment;  /* DCO phase increment, per 1 CLK. */

    uint32_t _ui32_pioreg[8];   /* Shift register to PIO. */

    uint32_t _clkfreq_hz;       /* CPU CLK freq, Hz. */

} PioDco;

int PioDCOInit(PioDco *pdco, uint32_t ui32_clock_hz);
int PioDCOSet(PioDco *pdco, uint32_t ui32_frq_hz);

void PioDCOStart(PioDco *pdco);
void PioDCOStop(PioDco *pdco);

void PioDCOTick(PioDco *pdco);

#endif
