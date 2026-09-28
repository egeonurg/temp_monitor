#ifndef APP_INC_H
#define APP_INC_H
#include <stdint.h>
#include <stddef.h>

#include "eeprom_ifa.h"
#include "temp_sensor_ifa.h"
#include "tim_ifa.h"
#include "adc_ifa.h"
#include "dma_ifa.h"
#include "platform_assert.h"
#include "platform_log.h"
#include "app_cfg.h"

/* Required interfaces, remapped to names this component owns. app.c uses only
   the APP_* names, so any provider below can be swapped or cut out without
   touching the implementation. */

#define APP_EEPROM_OK                       EEPROM_READ_OK
#define APP_READ_TEMP_SENSOR_REVISION(data) EEPROM_READ(EEPROM_TEMP_SENSOR_ADDRESS, data)

#define APP_TEMP_SENSOR_INIT()              temp_sensor_init()
#define APP_TEMP_SENSOR_REVISION()          temp_sensor_get_revision()
#define APP_TEMP_SENSOR_PRESCALER()         temp_sensor_get_prescaler()
#define APP_TEMP_SENSOR_BUFFER()            get_temp_sensor_buffer()
#define APP_TEMP_SENSOR_BUFFER_SIZE()       get_temp_sensor_buffer_size()
#define APP_TEMP_SENSOR_PROCESS(event)      temp_sensor_process_sample(event)
#define APP_TEMP_SENSOR_HALF_TRANSFER       TEMP_SENSOR_HALF_TRANSFER_EVENT
#define APP_TEMP_SENSOR_FULL_TRANSFER       TEMP_SENSOR_FULL_TRANSFER_EVENT

#define APP_DMA_OK                          DMA_OK
#define APP_DMA_INIT(buffer, size)          dma_init(buffer, size)
#define APP_DMA_HALF_EVENT_NUMBER()         dma_get_half_event_number()
#define APP_DMA_FULL_EVENT_NUMBER()         dma_get_full_event_number()

#define APP_ADC_OK                          ADC_OK
#define APP_ADC_INIT()                      adc_init()

#define APP_TIM_OK                          TIM_OK
#define APP_TIM_1MS_BASE                    TIM0
#define APP_TIM_AD_TRIGGER                  TIM1
#define APP_TIM_INIT(tim_id)                tim_init(tim_id)
#define APP_TIM_GET_1MS_FLAG()              tim_get_1ms_flag()
#define APP_TIM_CLEAR_1MS_FLAG()            tim_clear_1ms_flag()

#define APP_ASSERT(condition, message)      PLATFORM_ASSERT(condition, message)

/* Logging for this component: the tag is prefixed to every message, and the
   whole thing disappears on a target build. */
#define APP_LOG_TAG "#APP_LOG "
#define APP_LOG(...) PLATFORM_LOG(APP_LOG_TAG __VA_ARGS__)

#endif /* APP_INC_H */
