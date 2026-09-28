#include "temp_sensor_ifa.h"
#include "temp_sensor_inc.h"
#include "temp_sensor_cfg.h"

static uint16_t temp_sensor_revision  = TEMP_SENSOR_REVISION;
static uint16_t temp_sensor_prescaler = TEMP_SENSOR_REVISION_1_PRESCALER;

static uint16_t temp_sensor_buffer[TEMP_SENSOR_SAMPLE_COUNT] = {0}; 

uint16_t* get_temp_sensor_buffer(void)
{
    return temp_sensor_buffer;
}

uint16_t get_temp_sensor_buffer_size(void)
{
    return TEMP_SENSOR_SAMPLE_COUNT;
}

void temp_sensor_init(void)
{
    switch (temp_sensor_revision)
    {
        case TEMP_SENSOR_REVISION_A:
            temp_sensor_prescaler = TEMP_SENSOR_REVISION_1_PRESCALER;
            break;

        case TEMP_SENSOR_REVISION_B:
            temp_sensor_prescaler = TEMP_SENSOR_REVISION_2_PRESCALER;
            break;

        default:
            /* Unknown silicon: keep the safe defaults. */
            TEMP_SENSOR_ASSERT(0, "Unknown temp sensor revision");
            break;
    }
}

uint16_t temp_sensor_get_revision(void)
{
    return temp_sensor_revision;
}

uint16_t temp_sensor_get_prescaler(void)
{
    return temp_sensor_prescaler;
}

void temp_sensor_process_sample(temp_sensor_event_t event)
{
    if(event >= TEMP_SENSOR_EVENT_COUNT)
    {
        TEMP_SENSOR_ASSERT(0, "Invalid temp sensor event");
        return;
    }

    if(event == TEMP_SENSOR_HALF_TRANSFER_EVENT)
    {
        /* Process the first half of the buffer. */
        for(uint16_t i = 0; i < (TEMP_SENSOR_SAMPLE_COUNT / 2u); i++)
        {
            /* Example processing: print the sample value. */
            //printf(TEMP_SENSOR_LOG "Half transfer sample %u: %u\n", (unsigned int)i, (unsigned int)temp_sensor_buffer[i]);
        }
    }
    else if(event == TEMP_SENSOR_FULL_TRANSFER_EVENT)
    {
        /* Process the second half of the buffer. */
        for(uint16_t i = (TEMP_SENSOR_SAMPLE_COUNT / 2u); i < TEMP_SENSOR_SAMPLE_COUNT; i++)
        {
            /* Example processing: print the sample value. */
            //printf(TEMP_SENSOR_LOG "Full transfer sample %u: %u\n", (unsigned int)i, (unsigned int)temp_sensor_buffer[i]);
        }
    }
}
