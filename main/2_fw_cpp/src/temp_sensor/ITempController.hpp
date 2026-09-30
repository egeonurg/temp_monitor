#pragma once

#include <cstdint>

/* Temp Controller Interface */
class ITempController
{
public:
    enum class Event : uint8_t
    {
        HALF_TRANSFER,
        FULL_TRANSFER
    };

    enum class Condition : uint8_t
    {
        NORMAL,
        WARNING,
        CRITICAL,
        NONE    /* no measurement yet */
    };

    /* Revision values stored in the EEPROM */
    static constexpr uint16_t REVISION_A = 0x0000u;
    static constexpr uint16_t REVISION_B = 0x0001u;

    virtual void init(uint16_t revision) = 0;
    /* Filtered value of the half the event reports, in A/D counts */
    virtual uint16_t filterHalf(Event event) const = 0;
    /* Runs the condition state machine on a filtered value */
    virtual void evaluate(uint16_t value) = 0;

    virtual Condition getCondition() const = 0;
    virtual uint16_t getCountsPerDeg() const = 0;
    virtual uint16_t *getBuffer() = 0;
    virtual uint16_t getBufferSize() const = 0;

protected:
    ITempController()  = default;
    ~ITempController() = default;

    ITempController(const ITempController &)            = delete;
    ITempController &operator=(const ITempController &) = delete;
};
