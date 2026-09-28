#include "temp_sensor_ifa.h"
#include "temp_sensor_inc.h"
#include "temp_sensor_cfg.h"

#define TEMP_SENSOR_MEDIAN_WINDOW    5u
#define TEMP_SENSOR_HALF_COUNT       (TEMP_SENSOR_SAMPLE_COUNT / 2u)
#define TEMP_SENSOR_WINDOWS_PER_HALF (TEMP_SENSOR_HALF_COUNT / TEMP_SENSOR_MEDIAN_WINDOW)

static uint16_t temp_sensor_revision  = TEMP_SENSOR_REVISION_EMPTY;
static uint16_t temp_sensor_prescaler = TEMP_SENSOR_REVISION_A_PRESCALER;

/* Filled by the DMA: the first half while the second is processed, and the
   other way round. A half must be processed within one half period (10 ms),
   before the DMA comes back to it. */
static uint16_t temp_sensor_buffer[TEMP_SENSOR_SAMPLE_COUNT] = {0};

static temp_sensor_condition_t temp_sensor_condition = TEMP_SENSOR_CONDITION_NORMAL;
static temp_sensor_callback_t  temp_sensor_callback  = NULL;

/* Name of each condition for the log, indexed by condition. */
static const char * const temp_sensor_condition_name[TEMP_SENSOR_CONDITION_COUNT] =
{
    "NORMAL",
    "WARNING",
    "CRITICAL"
};

uint8_t temp_sensor_set_condition_callback(temp_sensor_callback_t callback)
{
    if (callback == NULL)
    {
        return TEMP_SENSOR_ERR;
    }

    temp_sensor_callback = callback;

    return TEMP_SENSOR_OK;
}

uint16_t *temp_sensor_get_buffer(void)
{
    return temp_sensor_buffer;
}

uint16_t temp_sensor_get_buffer_size(void)
{
    return TEMP_SENSOR_SAMPLE_COUNT;
}

void temp_sensor_init(uint16_t revision)
{
    switch (revision)
    {
        case TEMP_SENSOR_REVISION_A:
            temp_sensor_revision  = TEMP_SENSOR_REVISION_A;
            temp_sensor_prescaler = TEMP_SENSOR_REVISION_A_PRESCALER;
            break;

        case TEMP_SENSOR_REVISION_B:
            temp_sensor_revision  = TEMP_SENSOR_REVISION_B;
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
    for (uint8_t j = 0u; j < 4u; j++)
    {
        for (uint8_t k = (uint8_t)(j + 1u); k < 5u; k++)
        {
            if (v[j] > v[k])
            {
                uint16_t tmp = v[j];
                v[j] = v[k];
                v[k] = tmp;
            }
        }
    }

    return v[2]; /* middle element */
}

/* Average of the medians of every 5 sample window in one half buffer. */
static uint32_t temp_sensor_filtered_average(const uint16_t *half_buffer)
{
    uint32_t filtered_sum = 0u;

    for (uint16_t i = 0u; i < TEMP_SENSOR_WINDOWS_PER_HALF; i++)
    {
        filtered_sum += temp_sensor_median_5_filter(&half_buffer[i * TEMP_SENSOR_MEDIAN_WINDOW]);
    }

    return filtered_sum / TEMP_SENSOR_WINDOWS_PER_HALF;
}

static void temp_sensor_change_condition(temp_sensor_condition_t condition, uint16_t temp_deg)
{
    temp_sensor_condition = condition;

    TEMP_SENSOR_LOG("condition -> %s at %u C\n",
                    temp_sensor_condition_name[condition],
                    (unsigned int)temp_deg);

    temp_sensor_callback(condition);
}

/* Spec: NORMAL < 85 C, WARNING >= 85 C, CRITICAL >= 105 C or < 5 C. A condition
   is entered exactly at its limit and left TEMP_SENSOR_HYSTERESIS_DEG back
   inside the band, so noise on a limit cannot make the LEDs flicker. */
static void temp_sensor_evaluate_condition(uint16_t value)
{
    uint16_t temp_deg = 0u;

    if (temp_sensor_prescaler == 0u)
    {
        TEMP_SENSOR_ASSERT(0, "Prescaler is zero");
        return;
    }

    if (temp_sensor_callback == NULL)
    {
        TEMP_SENSOR_ASSERT(0, "Condition callback not registered");
        return;
    }

    temp_deg = value / temp_sensor_prescaler;

    switch (temp_sensor_condition)
    {
        case TEMP_SENSOR_CONDITION_NORMAL:
            if ((temp_deg < TEMP_SENSOR_CRITICAL_LOW_DEG) ||
                (temp_deg >= TEMP_SENSOR_CRITICAL_HIGH_DEG))
            {
                temp_sensor_change_condition(TEMP_SENSOR_CONDITION_CRITICAL, temp_deg);
            }
            else if (temp_deg >= TEMP_SENSOR_WARNING_DEG)
            {
                temp_sensor_change_condition(TEMP_SENSOR_CONDITION_WARNING, temp_deg);
            }
            else
            {
                /* Still NORMAL: no change. */
            }
            break;

        case TEMP_SENSOR_CONDITION_WARNING:
            if ((temp_deg < TEMP_SENSOR_CRITICAL_LOW_DEG) ||
                (temp_deg >= TEMP_SENSOR_CRITICAL_HIGH_DEG))
            {
                temp_sensor_change_condition(TEMP_SENSOR_CONDITION_CRITICAL, temp_deg);
            }
            else if (temp_deg <= (TEMP_SENSOR_WARNING_DEG - TEMP_SENSOR_HYSTERESIS_DEG))
            {
                temp_sensor_change_condition(TEMP_SENSOR_CONDITION_NORMAL, temp_deg);
            }
            else
            {
                /* Still WARNING, including the 84 C hysteresis band: no change. */
            }
            break;

        case TEMP_SENSOR_CONDITION_CRITICAL:
            if ((temp_deg >= (TEMP_SENSOR_CRITICAL_LOW_DEG + TEMP_SENSOR_HYSTERESIS_DEG)) &&
                (temp_deg < TEMP_SENSOR_WARNING_DEG))
            {
                temp_sensor_change_condition(TEMP_SENSOR_CONDITION_NORMAL, temp_deg);
            }
            else if ((temp_deg >= TEMP_SENSOR_WARNING_DEG) &&
                     (temp_deg <= (TEMP_SENSOR_CRITICAL_HIGH_DEG - TEMP_SENSOR_HYSTERESIS_DEG)))
            {
                temp_sensor_change_condition(TEMP_SENSOR_CONDITION_WARNING, temp_deg);
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

void temp_sensor_process_sample(temp_sensor_event_t event)
{
    uint32_t filtered_average = 0u;

    if (event == TEMP_SENSOR_HALF_TRANSFER_EVENT)
    {
        filtered_average = temp_sensor_filtered_average(&temp_sensor_buffer[0]);
    }
    else if (event == TEMP_SENSOR_FULL_TRANSFER_EVENT)
    {
        filtered_average = temp_sensor_filtered_average(&temp_sensor_buffer[TEMP_SENSOR_HALF_COUNT]);
    }
    else
    {
        TEMP_SENSOR_ASSERT(0, "Invalid temp sensor event");
        return;
    }

    temp_sensor_evaluate_condition((uint16_t)filtered_average);
}
