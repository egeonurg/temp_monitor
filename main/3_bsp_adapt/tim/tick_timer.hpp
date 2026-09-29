#ifndef TICK_TIMER_HPP
#define TICK_TIMER_HPP

#include "itimer.hpp"
#include "timer.hpp"

class TickTimer final : public ITickTimer
{
public:
    constexpr TickTimer()
        : timer_(Timer::ID_TIM0)
    {
    }

    uint8_t init() override;
    uint8_t deinit() override;

    uint8_t get_1ms_flag() override;
    void    clear_1ms_flag() override;

private:
    Timer timer_;
};

#endif /* TICK_TIMER_HPP */
