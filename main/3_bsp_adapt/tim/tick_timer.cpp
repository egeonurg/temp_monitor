#include "tick_timer.hpp"

extern "C"
{
#include "tim_ifa.h"
}

uint8_t TickTimer::init()
{
    return timer_.init();
}

uint8_t TickTimer::deinit()
{
    return timer_.deinit();
}

uint8_t TickTimer::get_1ms_flag()
{
    return tim_get_1ms_flag();
}

void TickTimer::clear_1ms_flag()
{
    tim_clear_1ms_flag();
}
