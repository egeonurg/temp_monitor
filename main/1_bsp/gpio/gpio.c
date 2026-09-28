#include "gpio_ifa.h"
#include "gpio_inc.h"



void gpio_init(uint8_t pin, uint8_t direction)
{
    // Mock Function
    GPIO_LOG("GPIO pin %d initialized with direction %d\n", pin, direction);
}


void gpio_deinit(uint8_t pin, uint8_t direction)
{
    // Mock Function
    GPIO_LOG("GPIO pin %d deinitialized with direction %d\n", pin, direction);
}

void gpio_write(uint8_t pin, uint8_t value)
{
    GPIO_LOG("GPIO pin %d written with value %d\n", pin, value);
}

uint8_t gpio_read(uint8_t pin)
{
    uint8_t value = 1; // Mock value
    GPIO_LOG("GPIO pin %d read with value %d\n", pin, value);
    return value;
}
