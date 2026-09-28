#ifndef TEMP_SENSOR_IFA_H
#define TEMP_SENSOR_IFA_H

#include <stdint.h>

typedef enum
{
   TEMP_SENSOR_HALF_TRANSFER_EVENT = 0u,
   TEMP_SENSOR_FULL_TRANSFER_EVENT = 1u,
   TEMP_SENSOR_EVENT_COUNT
}temp_sensor_event_t;

/* The revision is supplied by the caller: the driver does not know or care
   where it is stored. */
extern void temp_sensor_init(void);

extern uint16_t temp_sensor_get_revision(void);

extern uint16_t temp_sensor_get_prescaler(void);

extern uint16_t* get_temp_sensor_buffer(void);

extern uint16_t get_temp_sensor_buffer_size(void);

extern void temp_sensor_process_sample(temp_sensor_event_t event);
#endif /* TEMP_SENSOR_IFA_H */
