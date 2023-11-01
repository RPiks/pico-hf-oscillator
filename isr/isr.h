#ifndef ISR_H
#define ISR_H

#include "../piodco/piodco.h"
#include "build/dco.pio.h"

#include "../hwdefs.h"

typedef struct
{
    uint64_t tm_next;
    piodco *ppio_dco;

} ISRcontext;

void __not_in_flash_func (dcoISR)(void);

#endif
