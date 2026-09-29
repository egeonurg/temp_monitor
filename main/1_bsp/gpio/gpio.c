#include "gpio_ifa.h"
#include "gpio_inc.h"

/* Mock */
void gpio_init(uint8_t pin, uint8_t direction)
{
    PLATFORM_LOG_TAG(GPIO_LOG_TAG, "pin %u init, direction %u\n",
                     (unsigned int)pin, (unsigned int)direction);
}

/* Mock */
void gpio_deinit(uint8_t pin, uint8_t direction)
{
    PLATFORM_LOG_TAG(GPIO_LOG_TAG, "pin %u deinit, direction %u\n",
                     (unsigned int)pin, (unsigned int)direction);
}

/* Mock */
void gpio_write(uint8_t pin, uint8_t value)
{
    (void)pin;
    (void)value;
}
