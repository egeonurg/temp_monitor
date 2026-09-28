#ifndef TIM_INC_H
#define TIM_INC_H

#include <stdint.h>

#include "platform_log.h"

#include "tim_ifa.h"
#include "tim_cfg.h"

/* How a timer's update event is used. Chosen by configuration, not by callers. */
#define TIM_MODE_INTERRUPT  0x00  /* raises the 1 ms CPU interrupt */
#define TIM_MODE_AD_TRIGGER 0x01  /* drives the A/D trigger input  */

/* Logging for this component: the tag is prefixed to every message, and the
   whole thing disappears on a target build. */
#define TIM_LOG_TAG "#TIM_LOG "
#define TIM_LOG(...)  PLATFORM_LOG(TIM_LOG_TAG __VA_ARGS__)

#endif /* TIM_INC_H */
