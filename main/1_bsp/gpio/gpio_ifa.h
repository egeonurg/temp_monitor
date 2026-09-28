#ifndef GPIO_IFA_H
#define GPIO_IFA_H

#include <stdint.h>

extern void gpio_init(uint8_t pin, uint8_t direction);

extern void gpio_deinit(uint8_t pin, uint8_t direction);

extern void gpio_write(uint8_t pin, uint8_t value);

extern uint8_t gpio_read(uint8_t pin);

#endif /* GPIO_IFA_H */
