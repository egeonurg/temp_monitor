#pragma once

#include "ITempController.hpp"

class TempController final : public ITempController
{
public:
    TempController()=default;
    ~TempController()=default;

    void init(uint8_t revision) override;
    uint16_t filterHalf(Event event) const override;
    void evaluate(uint16_t value) override;

    Condition getCondition() const override { return sensorCondition; }
    uint16_t getCountsPerDeg() const override { return sensorCountPerDegree; }
    uint16_t *getBuffer() override { return sampleBuffer; }
    uint16_t getBufferSize() const override { return SAMPLE_COUNT; }

private:
    /* Counts per degree C */
    static constexpr uint16_t REVISION_A_COUNTS_PER_DEG = 1u;
    static constexpr uint16_t REVISION_B_COUNTS_PER_DEG = 10u;

    static constexpr uint16_t SAMPLE_COUNT     = 200u;
    static constexpr uint16_t MEDIAN_WINDOW    = 5u;
    static constexpr uint16_t HALF_COUNT       = SAMPLE_COUNT / 2u;
    static constexpr uint16_t WINDOWS_PER_HALF = HALF_COUNT / MEDIAN_WINDOW;

    /* Condition limits in degrees C */
    static constexpr uint16_t CRITICAL_LOW_DEG  = 5u;    /* critical below this */
    static constexpr uint16_t WARNING_DEG       = 85u;   /* warning from this   */
    static constexpr uint16_t CRITICAL_HIGH_DEG = 105u;  /* critical from this  */
    static constexpr uint16_t HYSTERESIS_DEG    = 2u;

    static uint16_t medianFilter5(const uint16_t *buffer);
    uint32_t generateFilteredAverage(const uint16_t *halfBuffer) const;
    void changeCondition(Condition condition, uint16_t tempDeg);

    Condition sensorCondition      = Condition::NONE;
    uint16_t  sensorCountPerDegree = REVISION_A_COUNTS_PER_DEG;

    /* Written by the DMA */
    uint16_t sampleBuffer[SAMPLE_COUNT] = {0};
};
