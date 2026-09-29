#pragma once

#include <cstdint>

class AppOrchestrator
{
public:
    AppOrchestrator()=default;
    ~AppOrchestrator()=default;

    void init();
    void performServices();

    uint16_t getCountsPerDeg() const { return countsPerDeg; }

private:
    static constexpr uint16_t SAMPLE_COUNT = 200u;

    uint16_t countsPerDeg = 1u;
    uint16_t halfEventNumber = 0u;
    uint16_t fullEventNumber = 0u;
    uint16_t sampleBuffer[SAMPLE_COUNT] = {0};
};
