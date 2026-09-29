#ifndef TIM_INC_H
#define TIM_INC_H

#include <stdint.h>

#include "platform_log.h"

#include "tim_ifa.h"
#include "tim_cfg.h"

#define TIM_MODE_INTERRUPT  0x00
#define TIM_MODE_AD_TRIGGER 0x01

#define TIM_LOG_TAG "#TIM_LOG "
#define TIM_LOG(...)  PLATFORM_LOG(TIM_LOG_TAG __VA_ARGS__)

#endif /* TIM_INC_H */
