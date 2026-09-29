#include "timer.hpp"

extern "C"
{
#include "tim_ifa.h"
}

static_assert(ITimer::OK == TIM_OK, "ITimer::OK differs from TIM_OK");
static_assert(ITimer::ERR == TIM_ERR, "ITimer::ERR differs from TIM_ERR");
static_assert(Timer::ID_TIM0 == TIM0, "Timer::ID_TIM0 differs from TIM0");
static_assert(Timer::ID_TIM1 == TIM1, "Timer::ID_TIM1 differs from TIM1");

uint8_t Timer::init()
{
    return tim_init(id_);
}

uint8_t Timer::deinit()
{
    return tim_deinit(id_);
}
