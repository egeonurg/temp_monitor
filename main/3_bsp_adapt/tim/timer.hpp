#ifndef TIMER_HPP
#define TIMER_HPP

#include "itimer.hpp"

class Timer final : public ITimer
{
public:
    static constexpr uint8_t ID_TIM0 = 0x00u;
    static constexpr uint8_t ID_TIM1 = 0x01u;

    explicit constexpr Timer(uint8_t id)
        : id_(id)
    {
    }

    uint8_t init() override;
    uint8_t deinit() override;

private:
    const uint8_t id_;
};

#endif /* TIMER_HPP */
