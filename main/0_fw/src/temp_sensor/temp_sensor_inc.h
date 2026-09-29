#ifndef TEMP_SENSOR_INC_H
#define TEMP_SENSOR_INC_H

#include <stdint.h>
#include "platform_assert.h"
#include "platform_log.h"

#define TEMP_SENSOR_ASSERT(condition, message)   PLATFORM_ASSERT(condition, message)

#define TEMP_SENSOR_LOG_TAG "#TEMP_SENSOR_LOG "
#define TEMP_SENSOR_LOG(...) PLATFORM_LOG(TEMP_SENSOR_LOG_TAG __VA_ARGS__)

#endif /* TEMP_SENSOR_INC_H */
