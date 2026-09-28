#include "gpio_ifa.h"
#include "gpio_inc.h"

void gpio_init(uint8_t pin, uint8_t direction)
{
    /* Mock: on target this sets the pin mode register. */
    GPIO_LOG("pin %u init, direction %u\n", (unsigned int)pin, (unsigned int)direction);
}

void gpio_deinit(uint8_t pin, uint8_t direction)
{
    /* Mock: on target this returns the pin to its reset state. */
    GPIO_LOG("pin %u deinit, direction %u\n", (unsigned int)pin, (unsigned int)direction);
}

void gpio_write(uint8_t pin, uint8_t value)
{
    /* Mock: on target this writes the output data register. */
    (void)pin;
    (void)value;
}
