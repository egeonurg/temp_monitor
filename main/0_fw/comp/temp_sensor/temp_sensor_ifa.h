#ifndef TEMP_SENSOR_IFA_H
#define TEMP_SENSOR_IFA_H

#include <stdint.h>

typedef enum
{
   TEMP_SENSOR_HALF_TRANSFER_EVENT = 0u,
   TEMP_SENSOR_FULL_TRANSFER_EVENT = 1u,
   TEMP_SENSOR_EVENT_COUNT
}temp_sensor_event_t;

typedef enum
{
    LED_NORMAL_CONDITION = 0u,
    LED_WARNING_CONDITION = 1u,
    LED_CRITICAL_CONDITION = 2u
}led_condition_t;

/* Called when the temperature condition changes. */
typedef void (*temp_sensor_callback_t)(led_condition_t condition);

/* The revision is supplied by the caller: the driver does not know or care
   where it is stored. */
extern void temp_sensor_init(uint16_t revision);

extern uint16_t temp_sensor_get_revision(void);

extern uint16_t temp_sensor_get_prescaler(void);

extern uint16_t* get_temp_sensor_buffer(void);

extern uint16_t get_temp_sensor_buffer_size(void);

extern void temp_sensor_process_sample(temp_sensor_event_t event);

extern uint8_t set_condition_update_callback(temp_sensor_callback_t function);

#endif /* TEMP_SENSOR_IFA_H */
