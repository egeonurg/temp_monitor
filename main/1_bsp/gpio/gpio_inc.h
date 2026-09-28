#ifndef GPIO_INC_H
#define GPIO_INC_H

#include <stdint.h>

#include "platform_log.h"

#include "gpio_ifa.h"

/* Logging for this component: the tag is prefixed to every message, and the
   whole thing disappears on a target build. */
#define GPIO_LOG_TAG "#GPIO_LOG "
#define GPIO_LOG(...)  PLATFORM_LOG(GPIO_LOG_TAG __VA_ARGS__)

#endif /* GPIO_INC_H */
