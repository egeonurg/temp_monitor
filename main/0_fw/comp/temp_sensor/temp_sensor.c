#include "temp_sensor_ifa.h"
#include "temp_sensor_inc.h"
#include "temp_sensor_cfg.h"

#define TEMP_SENSOR_MEDIAN_WINDOW    5u
#define TEMP_SENSOR_HALF_COUNT       (TEMP_SENSOR_SAMPLE_COUNT / 2u)
#define TEMP_SENSOR_WINDOWS_PER_HALF (TEMP_SENSOR_HALF_COUNT / TEMP_SENSOR_MEDIAN_WINDOW)

static uint16_t temp_sensor_revision  = TEMP_SENSOR_REVISION_EMPTY;
static uint16_t temp_sensor_prescaler = TEMP_SENSOR_REVISION_A_PRESCALER;

static uint16_t temp_sensor_buffer[TEMP_SENSOR_SAMPLE_COUNT] = {0}; 
static led_condition_t condition = LED_NORMAL_CONDITION;

static temp_sensor_callback_t condition_update_callback = NULL;

uint8_t set_condition_update_callback(temp_sensor_callback_t function)
{
    if(function != NULL)
    {
        condition_update_callback = function;
        return 1;
    }
    else
    {
        return 0;
    }
}

uint16_t* get_temp_sensor_buffer(void)
{
    return temp_sensor_buffer;
}

uint16_t get_temp_sensor_buffer_size(void)
{
    return TEMP_SENSOR_SAMPLE_COUNT;
}

void temp_sensor_init(uint16_t revision)
{    
    switch (revision)
    {
        case TEMP_SENSOR_REVISION_A:
            temp_sensor_revision = TEMP_SENSOR_REVISION_A;
            temp_sensor_prescaler = TEMP_SENSOR_REVISION_A_PRESCALER;
            break;

        case TEMP_SENSOR_REVISION_B:
            temp_sensor_revision = TEMP_SENSOR_REVISION_B;
            temp_sensor_prescaler = TEMP_SENSOR_REVISION_B_PRESCALER;
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

/* Median of 5 consecutive samples. Caller guarantees buffer has >= 5 entries. */
static uint16_t temp_sensor_median_5_filter(const uint16_t *buffer)
{
    uint16_t v[5] = {buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]};

    /* Simple exchange sort; trivial cost for 5 elements. */
    for(uint8_t j = 0; j < 4u; j++)
    {
        for(uint8_t k = (uint8_t)(j + 1u); k < 5u; k++)
        {
            if(v[j] > v[k])
            {
                uint16_t tmp = v[j];
                v[j] = v[k];
                v[k] = tmp;
            }
        }
    }

    return v[2]; /* middle element */
}

static void temp_sensor_evaluate_codition(uint16_t value)
{
    uint16_t filtered_avg_temp_with_prescaler = 0;

    if(temp_sensor_prescaler != 0)
    {
        filtered_avg_temp_with_prescaler = value / temp_sensor_prescaler;
    }
    else
    {
        TEMP_SENSOR_ASSERT(0, "Prescaler is zero");
        return;
    }

    if(condition_update_callback == 0)
    {
        TEMP_SENSOR_ASSERT(0, "Callback function uninitialized");
        return;
    }
    
  switch(condition)
  {
   case LED_NORMAL_CONDITION:
        if(   filtered_avg_temp_with_prescaler >= TEMP_SENSOR_WARNING_UPPER_THRESHOLD
           && filtered_avg_temp_with_prescaler < TEMP_SENSOR_CRITICAL_LOWER_THRESHOLD) /* 85 C <= t < 105 C */
        {
            condition = LED_WARNING_CONDITION;
            TEMP_SENSOR_LOG("condition -> WARNING at %u C\n", (unsigned int)filtered_avg_temp_with_prescaler);
            condition_update_callback(LED_WARNING_CONDITION);
        }
        else if(   filtered_avg_temp_with_prescaler < TEMP_SENSOR_WARNING_LOWER_THRESHOLD
                || filtered_avg_temp_with_prescaler >= TEMP_SENSOR_CRITICAL_LOWER_THRESHOLD) /* t < 5 C or t >= 105 C */
        {
            condition = LED_CRITICAL_CONDITION;
            TEMP_SENSOR_LOG("condition -> CRITICAL at %u C\n", (unsigned int)filtered_avg_temp_with_prescaler);
            condition_update_callback(LED_CRITICAL_CONDITION);
        }
        else
        {
            /* Still NORMAL: no change. */
        }
        break;
    case LED_WARNING_CONDITION:
        if(   filtered_avg_temp_with_prescaler <= (TEMP_SENSOR_WARNING_UPPER_THRESHOLD - TEMP_SENSOR_HYSTERESIS_VALUE)
           && filtered_avg_temp_with_prescaler >= TEMP_SENSOR_WARNING_LOWER_THRESHOLD)
        {
            condition = LED_NORMAL_CONDITION;
            TEMP_SENSOR_LOG("condition -> NORMAL at %u C\n", (unsigned int)filtered_avg_temp_with_prescaler);
            condition_update_callback(LED_NORMAL_CONDITION);
        }
        else if(   filtered_avg_temp_with_prescaler >= TEMP_SENSOR_CRITICAL_LOWER_THRESHOLD 
                || filtered_avg_temp_with_prescaler < TEMP_SENSOR_WARNING_LOWER_THRESHOLD)
        {
            condition = LED_CRITICAL_CONDITION;
            TEMP_SENSOR_LOG("condition -> CRITICAL at %u C\n", (unsigned int)filtered_avg_temp_with_prescaler);
            condition_update_callback(LED_CRITICAL_CONDITION);
        }
        else
        {
            /* Still WARNING, including the 84 C hysteresis band: no change. */
        }
        break;
    case LED_CRITICAL_CONDITION:
        if(    filtered_avg_temp_with_prescaler >= (TEMP_SENSOR_WARNING_LOWER_THRESHOLD + TEMP_SENSOR_HYSTERESIS_VALUE) 
            && filtered_avg_temp_with_prescaler < TEMP_SENSOR_WARNING_UPPER_THRESHOLD)
        {
            condition = LED_NORMAL_CONDITION;
            TEMP_SENSOR_LOG("condition -> NORMAL at %u C\n", (unsigned int)filtered_avg_temp_with_prescaler);
            condition_update_callback(LED_NORMAL_CONDITION);
        }
        else if(   filtered_avg_temp_with_prescaler <= (TEMP_SENSOR_CRITICAL_LOWER_THRESHOLD - TEMP_SENSOR_HYSTERESIS_VALUE) 
                && filtered_avg_temp_with_prescaler >= TEMP_SENSOR_WARNING_UPPER_THRESHOLD)
        {
            condition = LED_WARNING_CONDITION;
            TEMP_SENSOR_LOG("condition -> WARNING at %u C\n", (unsigned int)filtered_avg_temp_with_prescaler);
            condition_update_callback(LED_WARNING_CONDITION);
        }
        else
        {
            /* Still CRITICAL, including the 5-6 C and 104 C hysteresis bands: no change. */
        }
        break;
        default:
            TEMP_SENSOR_ASSERT(0, "Unknown condition");
            break;
    }
}

static uint32_t temp_sensor_filtered_average(const uint16_t * const half_buffer)
{
    uint32_t filtered_sum = 0;

    for(uint16_t i = 0; i < TEMP_SENSOR_WINDOWS_PER_HALF; i++)
    {
        filtered_sum += temp_sensor_median_5_filter(&half_buffer[i * TEMP_SENSOR_MEDIAN_WINDOW]);
    }

    return filtered_sum / TEMP_SENSOR_WINDOWS_PER_HALF;
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
        uint32_t filtered_average_temp = temp_sensor_filtered_average(&temp_sensor_buffer[0]);
        temp_sensor_evaluate_codition((uint16_t)filtered_average_temp);
    }
    else if(event == TEMP_SENSOR_FULL_TRANSFER_EVENT)
    {
        /* Process the second half of the buffer. */
        uint32_t filtered_average_temp = temp_sensor_filtered_average(&temp_sensor_buffer[TEMP_SENSOR_HALF_COUNT]);
        temp_sensor_evaluate_codition((uint16_t)filtered_average_temp);
    }
}
