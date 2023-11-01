#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "build/dco.pio.h"

#include "isr.h"

extern ISRcontext ISRcntx;

void __not_in_flash_func (dcoISR)(void)
{
    if(!ISRcntx.ppio_dco || !ISRcntx.ppio_dco->_brun)
    {
        ISRcntx.tm_next += 1000;
        goto EXIT;
    }

    const piodco *DCO = ISRcntx.ppio_dco;

    //DCO->
    
EXIT:
    hw_clear_bits(&timer_hw->intr, 1U<<TIMER_ALARM_NUM);         /* Stop protection. */
    timer_hw->alarm[TIMER_ALARM_NUM] = (uint32_t)ISRcntx.tm_next;       /* Set next. */
}
