#ifndef GPIO_PIN_HPP
#define GPIO_PIN_HPP

#include "igpio_pin.hpp"

class GpioPin final : public IGpioPin
{
public:
    constexpr GpioPin(uint8_t pin, uint8_t direction)
        : pin_(pin), direction_(direction)
    {
    }

    void init() override;
    void deinit() override;
    void write(uint8_t value) override;

private:
    const uint8_t pin_;
    const uint8_t direction_;
};

#endif /* GPIO_PIN_HPP */
