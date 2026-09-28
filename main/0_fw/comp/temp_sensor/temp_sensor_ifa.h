#ifndef TEMP_SENSOR_IFA_H
#define TEMP_SENSOR_IFA_H

#include <stdint.h>

#define TEMP_SENSOR_OK  0x00
#define TEMP_SENSOR_ERR 0x01

/* Hardware revisions, as stored in the EEPROM. */
#define TEMP_SENSOR_REVISION_A     0x0000u
#define TEMP_SENSOR_REVISION_B     0x0001u
#define TEMP_SENSOR_REVISION_EMPTY 0xFFFFu

typedef enum
{
    TEMP_SENSOR_HALF_TRANSFER_EVENT = 0u,
    TEMP_SENSOR_FULL_TRANSFER_EVENT = 1u
} temp_sensor_event_t;

typedef enum
{
    TEMP_SENSOR_CONDITION_NORMAL   = 0u,
    TEMP_SENSOR_CONDITION_WARNING  = 1u,
    TEMP_SENSOR_CONDITION_CRITICAL = 2u,
    TEMP_SENSOR_CONDITION_COUNT
} temp_sensor_condition_t;

/* Called when the temperature condition changes. */
typedef void (*temp_sensor_callback_t)(temp_sensor_condition_t condition);

/* The revision is supplied by the caller: the driver does not know or care
   where it is stored. */
extern void temp_sensor_init(uint16_t revision);

extern uint16_t temp_sensor_get_revision(void);

extern uint16_t temp_sensor_get_counts_per_deg(void);

extern uint16_t *temp_sensor_get_buffer(void);

extern uint16_t temp_sensor_get_buffer_size(void);

extern void temp_sensor_process_half(temp_sensor_event_t event);

extern uint8_t temp_sensor_set_condition_callback(temp_sensor_callback_t callback);

#endif /* TEMP_SENSOR_IFA_H */
