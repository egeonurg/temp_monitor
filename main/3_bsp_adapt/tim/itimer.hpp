#ifndef ITIMER_HPP
#define ITIMER_HPP

#include <cstdint>

class ITimer
{
public:
    static constexpr uint8_t OK  = 0x00u;
    static constexpr uint8_t ERR = 0x01u;

    virtual uint8_t init()   = 0;
    virtual uint8_t deinit() = 0;

protected:
    ITimer()  = default;
    ~ITimer() = default;

    ITimer(const ITimer &)            = delete;
    ITimer &operator=(const ITimer &) = delete;
};

/* TIM0, raises the 1 ms tick */
class ITickTimer : public ITimer
{
public:
    virtual uint8_t get_1ms_flag()   = 0;
    virtual void    clear_1ms_flag() = 0;

protected:
    ITickTimer()  = default;
    ~ITickTimer() = default;
};

#endif /* ITIMER_HPP */
