#ifndef PIODCO_H_
#define PIODCO_H_

#include <stdbool.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"

typedef struct
{
    bool _brun;

    PIO _pio_I, _pio_Q;
    uint8_t _gpio_I, _gpio_Q;

    uint32_t _phase_prompt;
    uint32_t _phase_increment;

} piodco;

int PioDCOInit(piodco *pdco, uint32_t ui32_clock_hz);
int PioDCOSet(piodco *pdco, uint32_t ui32_frq_hz);

void PioDCOStart(piodco *pdco);
void PioDCOStop(piodco *pdco);

void PioDCOTick(piodco *pdco);

#endif
