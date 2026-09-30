#pragma once

#include <cstdint>

/* Needed in full for the nested Condition type */
#include "ITempController.hpp"

/* Forward declarations, since interfaces are references */
class IAdc;
class IDma;
class ITimer;
class ITickTimer;
class IEepromRead;
class ILedController;

class AppOrchestrator
{
public:
    AppOrchestrator(IEepromRead &eepromIfa, ILedController &ledIfa, ITempController &tempSensorIfa,
                    IDma &dmaIfa, IAdc &adcIfa, ITimer &adTriggerTimerIfa, ITickTimer &tickTimerIfa)
        : eeprom(eepromIfa), led(ledIfa), tempSensor(tempSensorIfa),
          dma(dmaIfa), adc(adcIfa), adTriggerTimer(adTriggerTimerIfa), tickTimer(tickTimerIfa) {}

    ~AppOrchestrator()=default;

    void init();
    void performServices();

    uint16_t getCountsPerDeg() const;

private:
    void updateLeds();

    IEepromRead     &eeprom;
    ILedController  &led;
    ITempController &tempSensor;
    IDma            &dma;
    IAdc            &adc;
    ITimer          &adTriggerTimer;
    ITickTimer      &tickTimer;

    uint16_t halfEventNumber = 0u;
    uint16_t fullEventNumber = 0u;

    /* The LEDs start green */
    ITempController::Condition shownCondition = ITempController::Condition::NORMAL;
};
