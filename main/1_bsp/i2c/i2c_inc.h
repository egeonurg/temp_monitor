#ifndef I2C_INC_H
#define I2C_INC_H

#include <stdint.h>

#include "platform_log.h"

#include "i2c_ifa.h"
#include "i2c_cfg.h"

/* Logging for this component: the tag is prefixed to every message, and the
   whole thing disappears on a target build. */
#define I2C_LOG_TAG "#I2C_LOG "
#define I2C_LOG(...)  PLATFORM_LOG(I2C_LOG_TAG __VA_ARGS__)

#endif /* I2C_INC_H */
