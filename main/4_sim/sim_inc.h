#ifndef SIM_INC_H
#define SIM_INC_H

#include <stdint.h>

#include "platform_log.h"

#include "sim_ifa.h"
#include "sim_cfg.h"

/* Logging for this component: the tag is prefixed to every message, and the
   whole thing disappears on a target build. */
#define SIM_LOG_TAG "#SIM_LOG "
#define SIM_LOG(...)  PLATFORM_LOG(SIM_LOG_TAG __VA_ARGS__)

#endif /* SIM_INC_H */
