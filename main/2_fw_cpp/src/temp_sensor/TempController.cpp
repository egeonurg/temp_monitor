#include "TempController.hpp"

#include "platform_assert.h"
#include "platform_log.h"

#define TEMP_SENSOR_LOG_TAG "TEMP_SENSOR"

static const char * const CONDITION_NAME[] =
{
    "NORMAL",
    "WARNING",
    "CRITICAL",
    "NONE"
};

void TempController::init(uint8_t revision)
{
    switch (revision)
    {
        case REVISION_A:
            sensorCountPerDegree = REVISION_A_COUNTS_PER_DEG;
            break;

        case REVISION_B:
            sensorCountPerDegree = REVISION_B_COUNTS_PER_DEG;
            break;

        default:
            PLATFORM_ASSERT(0, "Unknown temp sensor revision");
            break;
    }
}

uint16_t TempController::filterHalf(Event event) const
{
    const uint16_t *half = nullptr;

    switch (event)
    {
        case Event::HALF_TRANSFER:
            half = &sampleBuffer[0];
            break;

        case Event::FULL_TRANSFER:
            half = &sampleBuffer[HALF_COUNT];
            break;

        default:
            PLATFORM_ASSERT(0, "Invalid temp sensor event");
            break;
    }

    return static_cast<uint16_t>(generateFilteredAverage(half));
}

uint16_t TempController::medianFilter5(const uint16_t *buffer)
{
    static_assert(MEDIAN_WINDOW == 5u, "medianFilter5 needs a window of 5");

    uint16_t v[5] = {buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]};

    for (uint8_t j = 0u; j < 4u; j++)
    {
        for (uint8_t k = static_cast<uint8_t>(j + 1u); k < 5u; k++)
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
uint32_t TempController::generateFilteredAverage(const uint16_t *halfBuffer) const
{
    uint32_t filteredSum = 0u;

    for (uint16_t i = 0u; i < WINDOWS_PER_HALF; i++)
    {
        filteredSum += medianFilter5(&halfBuffer[i * MEDIAN_WINDOW]);
    }

    return filteredSum / WINDOWS_PER_HALF;
}

void TempController::changeCondition(Condition condition, uint16_t tempDeg)
{
    sensorCondition = condition;

    PLATFORM_LOG_TAG(TEMP_SENSOR_LOG_TAG, "condition -> %s at %u C\n",
                     CONDITION_NAME[static_cast<uint8_t>(condition)],
                     static_cast<unsigned int>(tempDeg));
}

/* NORMAL < 85 C, WARNING >= 85 C, CRITICAL >= 105 C or < 5 C.
   A condition is left only HYSTERESIS_DEG back inside the band.
   The first measurement has nothing to hold, so it takes the plain limits. */
void TempController::evaluate(uint16_t value)
{
    uint16_t tempDeg = 0u;

    PLATFORM_ASSERT(sensorCountPerDegree != 0u, "Counts per degree is zero");

    tempDeg = static_cast<uint16_t>(value / sensorCountPerDegree);

    switch (sensorCondition)
    {
        case Condition::NONE:
            if ((tempDeg < CRITICAL_LOW_DEG) || (tempDeg >= CRITICAL_HIGH_DEG))
            {
                changeCondition(Condition::CRITICAL, tempDeg);
            }
            else if (tempDeg >= WARNING_DEG)
            {
                changeCondition(Condition::WARNING, tempDeg);
            }
            else
            {
                changeCondition(Condition::NORMAL, tempDeg);
            }
            break;

        case Condition::NORMAL:
            if ((tempDeg < CRITICAL_LOW_DEG) || (tempDeg >= CRITICAL_HIGH_DEG))
            {
                changeCondition(Condition::CRITICAL, tempDeg);
            }
            else if (tempDeg >= WARNING_DEG)
            {
                changeCondition(Condition::WARNING, tempDeg);
            }
            break;

        case Condition::WARNING:
            if ((tempDeg < CRITICAL_LOW_DEG) || (tempDeg >= CRITICAL_HIGH_DEG))
            {
                changeCondition(Condition::CRITICAL, tempDeg);
            }
            else if (tempDeg <= (WARNING_DEG - HYSTERESIS_DEG))
            {
                changeCondition(Condition::NORMAL, tempDeg);
            }
            break;

        case Condition::CRITICAL:
            if ((tempDeg >= (CRITICAL_LOW_DEG + HYSTERESIS_DEG)) && (tempDeg < WARNING_DEG))
            {
                changeCondition(Condition::NORMAL, tempDeg);
            }
            else if ((tempDeg >= WARNING_DEG) && (tempDeg <= (CRITICAL_HIGH_DEG - HYSTERESIS_DEG)))
            {
                changeCondition(Condition::WARNING, tempDeg);
            }
            break;

        default:
            PLATFORM_ASSERT(0, "Unknown condition");
            break;
    }
}
