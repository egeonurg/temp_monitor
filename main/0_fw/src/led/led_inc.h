#ifndef LED_INC_H
#define LED_INC_H

#include <stdint.h>

#include "platform_assert.h"
#include "platform_log.h"

#include "led_ifa.h"
#include "led_cfg.h"
#include "gpio_ifa.h"

/* Required interfaces */
#define LED_GPIO_INIT(pin, direction)   gpio_init(pin, direction)
#define LED_GPIO_DEINIT(pin, direction) gpio_deinit(pin, direction)
#define LED_GPIO_WRITE(pin, value)      gpio_write(pin, value)

#define LED_LOG_TAG "LED"

#endif /* LED_INC_H */
