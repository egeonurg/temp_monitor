#ifndef IGPIO_PIN_HPP
#define IGPIO_PIN_HPP

#include <cstdint>

class IGpioPin
{
public:
    virtual void init()               = 0;
    virtual void deinit()             = 0;
    virtual void write(uint8_t value) = 0;

protected:
    IGpioPin()  = default;
    ~IGpioPin() = default;

    IGpioPin(const IGpioPin &)            = delete;
    IGpioPin &operator=(const IGpioPin &) = delete;
};

#endif /* IGPIO_PIN_HPP */
