#include "gpio_pin.hpp"

extern "C"
{
#include "gpio_ifa.h"
}

void GpioPin::init()
{
    gpio_init(pin_, direction_);
}

void GpioPin::deinit()
{
    gpio_deinit(pin_, direction_);
}

void GpioPin::write(uint8_t value)
{
    gpio_write(pin_, value);
}
