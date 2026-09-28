#include "app_ifa.h"
#include "app_inc.h"

/* Last transfer event number this component has processed. */
static uint16_t app_half_event_number = 0u;
static uint16_t app_full_event_number = 0u;

/* Placeholder only: read and reported, not used. +1 for the terminator. */
static char app_serial_number[EEPROM_SERIAL_NUMBER_LENGTH + 1u] = {0};

void app_init(void)
{
    uint16_t temp_sensor_revision = 0;
    uint16_t *sample_buffer      = NULL;
    uint16_t  sample_buffer_size = 0;

    uint16_t eeprom_status = APP_READ_TEMP_SENSOR_REVISION(&temp_sensor_revision);

    if (eeprom_status != APP_EEPROM_OK)
    {
        /* Leave the driver on its safe defaults rather than configuring it
           from a value that was never read. */
        APP_ASSERT(0, "Failed to read temp sensor revision from EEPROM");
    }
    else
    {
        APP_TEMP_SENSOR_INIT(temp_sensor_revision);
    }

    if (APP_READ_SERIAL_NUMBER(app_serial_number) != APP_EEPROM_OK)
    {
        APP_ASSERT(0, "Failed to read serial number from EEPROM");
    }

    APP_LOG("serial number %s\n", app_serial_number);

    APP_LOG("temp sensor revision %u, prescaler %u\n",
           (unsigned int)APP_TEMP_SENSOR_REVISION(),
           (unsigned int)APP_TEMP_SENSOR_PRESCALER());

    /* The samples belong to the temp sensor; the DMA only needs to know where
       to put them. */
    sample_buffer      = APP_TEMP_SENSOR_BUFFER();
    sample_buffer_size = APP_TEMP_SENSOR_BUFFER_SIZE();

    /* DMA before the A/D: the destination must exist before a conversion can
       complete. */
    if (sample_buffer == NULL)
    {
        APP_ASSERT(0, "Failed to get temp sensor buffer");
    }
    else if (APP_DMA_INIT(sample_buffer, sample_buffer_size) != APP_DMA_OK)
    {
        APP_ASSERT(0, "Failed to initialise the A/D sample DMA");
    }
    else
    {
        /* DMA armed on the temp sensor buffer. */
    }

    /* A/D before its trigger source: the converter must be listening before
       the timer starts raising trigger events. */
    if (APP_ADC_INIT() != APP_ADC_OK)
    {
        APP_ASSERT(0, "Failed to initialise the A/D converter");
    }

    /* Timers last: the 1 ms interrupt must not fire before the drivers it
       serves have been configured. */
    if (APP_TIM_INIT(APP_TIM_1MS_BASE) != APP_TIM_OK)
    {
        APP_ASSERT(0, "Failed to initialise the 1 ms system tick timer");
    }

    if (APP_TIM_INIT(APP_TIM_AD_TRIGGER) != APP_TIM_OK)
    {
        APP_ASSERT(0, "Failed to initialise the A/D trigger timer");
    }
}

uint8_t app_get_1ms_flag(void)
{
    return APP_TIM_GET_1MS_FLAG();
}

void app_clear_1ms_flag(void)
{
    APP_TIM_CLEAR_1MS_FLAG();
}


void app_handle_1ms_event(void)
{
    uint16_t half_event_number = APP_DMA_HALF_EVENT_NUMBER();
    uint16_t full_event_number = APP_DMA_FULL_EVENT_NUMBER();
    uint16_t missed            = 0u;

    /* The difference is what matters, not which counter is larger: computing it
       in uint16_t keeps this correct when the event number wraps. */
    missed = (uint16_t)(half_event_number - app_half_event_number);

    if (missed != 0u)
    {
        app_half_event_number = half_event_number;

        if (missed > 1u)
        {
            APP_LOG("Missed %u half transfer event(s)\n", (unsigned int)(missed - 1u));
        }

        /* Process the first half of the buffer. */
        APP_TEMP_SENSOR_PROCESS(APP_TEMP_SENSOR_HALF_TRANSFER);
    }

    missed = (uint16_t)(full_event_number - app_full_event_number);

    if (missed != 0u)
    {
        app_full_event_number = full_event_number;

        if (missed > 1u)
        {
            APP_LOG("Missed %u full transfer event(s)\n", (unsigned int)(missed - 1u));
        }

        /* Process the second half of the buffer. */
        APP_TEMP_SENSOR_PROCESS(APP_TEMP_SENSOR_FULL_TRANSFER);
    }
}
