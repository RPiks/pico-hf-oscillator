#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdint.h>
#include "pico/stdlib.h"

inline uint64_t GetUptime64(void)
{
    const uint32_t lo = timer_hw->timelr;
    const uint32_t hi = timer_hw->timehr;

    return ((uint64_t)hi << 32U) | lo;
}

inline uint32_t GetTime32(void)
{
    return timer_hw->timelr;
}

inline uint32_t PicoU64timeToSeconds(uint64_t u64tm)
{
    return u64tm / 1000000U;    // No rounding deliberately!
}

inline uint32_t DecimalStr2ToNumber(const char *p)
{
    return 10U * (p[0] - '0') + (p[1] - '0');
}

#endif
