#include "app_ifa.h"
#include "app_inc.h"

static char app_serial_number[APP_EEPROM_SERIAL_NUMBER_LENGTH + 1u] = {0};

static void app_on_temp_condition(APP_TEMP_SENSOR_CONDITION_T condition)
{
    switch (condition)
    {
        case APP_TEMP_SENSOR_NORMAL:
            (void)APP_LED_SET_ACTIVE(APP_LED_NORMAL);
            break;

        case APP_TEMP_SENSOR_WARNING:
            (void)APP_LED_SET_ACTIVE(APP_LED_WARNING);
            break;

        case APP_TEMP_SENSOR_CRITICAL:
            (void)APP_LED_SET_ACTIVE(APP_LED_CRITICAL);
            break;

        default:
            PLATFORM_ASSERT(0, "Unknown temp sensor condition");
            break;
    }
}

void app_init(void)
{
    uint8_t   temp_sensor_revision = 0u;
    uint16_t *sample_buffer        = NULL;
    uint16_t  sample_buffer_size   = 0u;

    if (APP_READ_TEMP_SENSOR_REVISION(&temp_sensor_revision) != APP_EEPROM_OK)
    {
        PLATFORM_ASSERT(0, "Failed to read temp sensor revision from EEPROM");
    }

    APP_TEMP_SENSOR_INIT(temp_sensor_revision);

    if (APP_READ_SERIAL_NUMBER(app_serial_number) != APP_EEPROM_OK)
    {
        PLATFORM_ASSERT(0, "Failed to read serial number from EEPROM");
    }

    PLATFORM_LOG_TAG(APP_LOG_TAG, "serial number %s\n", app_serial_number);

    PLATFORM_LOG_TAG(APP_LOG_TAG, "temp sensor revision %u, %u count(s) per degree\n",
                     (unsigned int)APP_TEMP_SENSOR_REVISION(),
                     (unsigned int)APP_TEMP_SENSOR_COUNTS_PER_DEG());

    /* LEDs must be ready before the first condition callback. */
    if (APP_LED_INIT() != APP_LED_OK)
    {
        PLATFORM_ASSERT(0, "Failed to initialise the LEDs");
    }

    if (APP_TEMP_SENSOR_SET_CALLBACK(app_on_temp_condition) != APP_TEMP_SENSOR_OK)
    {
        PLATFORM_ASSERT(0, "Failed to register the temp sensor callback");
    }

    sample_buffer      = APP_TEMP_SENSOR_BUFFER();
    sample_buffer_size = APP_TEMP_SENSOR_BUFFER_SIZE();

    PLATFORM_ASSERT(sample_buffer != NULL, "Failed to get temp sensor buffer");

    /* Init order: DMA, A/D, then the timers that start everything. */
    if (APP_DMA_INIT(sample_buffer, sample_buffer_size) != APP_DMA_OK)
    {
        PLATFORM_ASSERT(0, "Failed to initialise the A/D sample DMA");
    }

    if (APP_ADC_INIT() != APP_ADC_OK)
    {
        PLATFORM_ASSERT(0, "Failed to initialise the A/D converter");
    }

    if (APP_TIM_INIT(APP_TIM_1MS_BASE) != APP_TIM_OK)
    {
        PLATFORM_ASSERT(0, "Failed to initialise the 1 ms system tick timer");
    }

    if (APP_TIM_INIT(APP_TIM_AD_TRIGGER) != APP_TIM_OK)
    {
        PLATFORM_ASSERT(0, "Failed to initialise the A/D trigger timer");
    }
}

uint16_t app_get_counts_per_deg(void)
{
    return APP_TEMP_SENSOR_COUNTS_PER_DEG();
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
    uint16_t value = 0u;

    if (APP_DMA_TAKE_HALF_FLAG() != 0u)
    {
        value = APP_TEMP_SENSOR_FILTER(APP_TEMP_SENSOR_HALF_TRANSFER);
        APP_TEMP_SENSOR_EVALUATE(value);
    }

    if (APP_DMA_TAKE_FULL_FLAG() != 0u)
    {
        value = APP_TEMP_SENSOR_FILTER(APP_TEMP_SENSOR_FULL_TRANSFER);
        APP_TEMP_SENSOR_EVALUATE(value);
    }
}
