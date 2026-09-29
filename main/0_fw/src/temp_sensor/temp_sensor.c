#include "temp_sensor_ifa.h"
#include "temp_sensor_inc.h"
#include "temp_sensor_cfg.h"

#define TEMP_SENSOR_MEDIAN_WINDOW    5u
#define TEMP_SENSOR_HALF_COUNT       (TEMP_SENSOR_SAMPLE_COUNT / 2u)
#define TEMP_SENSOR_WINDOWS_PER_HALF (TEMP_SENSOR_HALF_COUNT / TEMP_SENSOR_MEDIAN_WINDOW)

#if (TEMP_SENSOR_HALF_COUNT % TEMP_SENSOR_MEDIAN_WINDOW) != 0u
#error "Half buffer must hold a whole number of median windows"
#endif

static uint16_t temp_sensor_revision       = TEMP_SENSOR_REVISION_EMPTY;
static uint16_t temp_sensor_counts_per_deg = TEMP_SENSOR_REVISION_A_COUNTS_PER_DEG;

/* Written by the DMA. Each half must be processed within 10 ms, before the
   DMA wraps back to it. */
static uint16_t temp_sensor_buffer[TEMP_SENSOR_SAMPLE_COUNT] = {0};

static temp_sensor_condition_t temp_sensor_condition = TEMP_SENSOR_CONDITION_NORMAL;
static temp_sensor_callback_t  temp_sensor_callback  = NULL;

static const char * const temp_sensor_condition_name[TEMP_SENSOR_CONDITION_COUNT] =
{
    "NORMAL",
    "WARNING",
    "CRITICAL"
};

uint8_t temp_sensor_set_condition_callback(temp_sensor_callback_t callback)
{
    uint8_t ret = TEMP_SENSOR_ERR;

    if (callback != NULL)
    {
        temp_sensor_callback = callback;
        ret = TEMP_SENSOR_OK;
    }

    return ret;
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
            temp_sensor_revision       = TEMP_SENSOR_REVISION_A;
            temp_sensor_counts_per_deg = TEMP_SENSOR_REVISION_A_COUNTS_PER_DEG;
            break;

        case TEMP_SENSOR_REVISION_B:
            temp_sensor_revision       = TEMP_SENSOR_REVISION_B;
            temp_sensor_counts_per_deg = TEMP_SENSOR_REVISION_B_COUNTS_PER_DEG;
            break;

        default:
            PLATFORM_ASSERT(0, "Unknown temp sensor revision");
            break;
    }
}

uint16_t temp_sensor_get_revision(void)
{
    return temp_sensor_revision;
}

uint16_t temp_sensor_get_counts_per_deg(void)
{
    return temp_sensor_counts_per_deg;
}

static uint16_t temp_sensor_median_5_filter(const uint16_t *buffer)
{
    uint16_t v[5] = {buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]};

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

    return v[2];
}

/* Average of the 5-sample medians over one half buffer. */
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

    PLATFORM_LOG_TAG(TEMP_SENSOR_LOG_TAG, "condition -> %s at %u C\n",
                     temp_sensor_condition_name[condition],
                     (unsigned int)temp_deg);

    temp_sensor_callback(condition);
}

/* NORMAL < 85 C, WARNING >= 85 C, CRITICAL >= 105 C or < 5 C.
   A condition is left only TEMP_SENSOR_HYSTERESIS_DEG back inside the band. */
static void temp_sensor_evaluate_condition(uint16_t value)
{
    uint16_t temp_deg = 0u;

    PLATFORM_ASSERT(temp_sensor_counts_per_deg != 0u, "Counts per degree is zero");
    PLATFORM_ASSERT(temp_sensor_callback != NULL, "Condition callback not registered");

    temp_deg = value / temp_sensor_counts_per_deg;

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
            break;

        default:
            PLATFORM_ASSERT(0, "Unknown condition");
            break;
    }
}

void temp_sensor_process_half(temp_sensor_event_t event)
{
    const uint16_t *half = NULL;

    switch (event)
    {
        case TEMP_SENSOR_HALF_TRANSFER_EVENT:
            half = &temp_sensor_buffer[0];
            break;

        case TEMP_SENSOR_FULL_TRANSFER_EVENT:
            half = &temp_sensor_buffer[TEMP_SENSOR_HALF_COUNT];
            break;

        default:
            PLATFORM_ASSERT(0, "Invalid temp sensor event");
            break;
    }

    temp_sensor_evaluate_condition((uint16_t)temp_sensor_filtered_average(half));
}
