#ifndef APP_INC_H
#define APP_INC_H

#include <stdint.h>
#include <stddef.h>

#include "platform_assert.h"
#include "platform_log.h"
#include "platform_barrier.h"

#include "app_cfg.h"
#include "eeprom_ifa.h"
#include "temp_sensor_ifa.h"
#include "led_ifa.h"
#include "tim_ifa.h"
#include "adc_ifa.h"
#include "dma_ifa.h"

/* Required interfaces */

#define APP_EEPROM_OK                       EEPROM_OK
#define APP_READ_TEMP_SENSOR_REVISION(data) EEPROM_READ(APP_EEPROM_TEMP_SENSOR_ADDRESS, data)
#define APP_READ_SERIAL_NUMBER(data)        eeprom_read(APP_EEPROM_SERIAL_NUMBER_ADDRESS, data, APP_EEPROM_SERIAL_NUMBER_LENGTH)

#define APP_TEMP_SENSOR_INIT(revision)      temp_sensor_init(revision)
#define APP_TEMP_SENSOR_REVISION()          temp_sensor_get_revision()
#define APP_TEMP_SENSOR_COUNTS_PER_DEG()    temp_sensor_get_counts_per_deg()
#define APP_TEMP_SENSOR_BUFFER()            temp_sensor_get_buffer()
#define APP_TEMP_SENSOR_BUFFER_SIZE()       temp_sensor_get_buffer_size()
#define APP_TEMP_SENSOR_FILTER(event)       temp_sensor_filter_half(event)
#define APP_TEMP_SENSOR_EVALUATE(value)     temp_sensor_evaluate(value)
#define APP_TEMP_SENSOR_EVENT_T             temp_sensor_event_t
#define APP_TEMP_SENSOR_HALF_TRANSFER       TEMP_SENSOR_HALF_TRANSFER_EVENT
#define APP_TEMP_SENSOR_FULL_TRANSFER       TEMP_SENSOR_FULL_TRANSFER_EVENT
#define APP_TEMP_SENSOR_OK                  TEMP_SENSOR_OK
#define APP_TEMP_SENSOR_SET_CALLBACK(cb)    temp_sensor_set_condition_callback(cb)
#define APP_TEMP_SENSOR_CONDITION_T         temp_sensor_condition_t
#define APP_TEMP_SENSOR_NORMAL              TEMP_SENSOR_CONDITION_NORMAL
#define APP_TEMP_SENSOR_WARNING             TEMP_SENSOR_CONDITION_WARNING
#define APP_TEMP_SENSOR_CRITICAL            TEMP_SENSOR_CONDITION_CRITICAL

/* Condition to LED mapping */
#define APP_LED_OK                          LED_OK
#define APP_LED_INIT()                      led_init()
#define APP_LED_SET_ACTIVE(led_id)          led_set_active(led_id)
#define APP_LED_NORMAL                      LED_GREEN
#define APP_LED_WARNING                     LED_YELLOW
#define APP_LED_CRITICAL                    LED_RED

#define APP_DMA_OK                          DMA_OK
#define APP_DMA_INIT(buffer, size)          dma_init(buffer, size)
#define APP_DMA_EVENT_NUMBER()              ((uint16_t)(dma_get_half_event_number() + dma_get_full_event_number()))

#define APP_ADC_OK                          ADC_OK
#define APP_ADC_INIT()                      adc_init()

#define APP_TIM_OK                          TIM_OK
#define APP_TIM_1MS_BASE                    TIM0
#define APP_TIM_AD_TRIGGER                  TIM1
#define APP_TIM_INIT(tim_id)                tim_init(tim_id)
#define APP_TIM_GET_1MS_FLAG()              tim_get_1ms_flag()
#define APP_TIM_CLEAR_1MS_FLAG()            tim_clear_1ms_flag()

#define APP_DSB()                           PLATFORM_DSB()

#define APP_LOG_TAG "APP"

#endif /* APP_INC_H */
