#include "AppOrchestrator.hpp"

#include "iadc.hpp"
#include "idma.hpp"
#include "itimer.hpp"
#include "IEepromRead.hpp"
#include "ILedController.hpp"

#include "platform_assert.h"
#include "platform_log.h"

#define APP_LOG_TAG "APP"

/* EEPROM memory map */
static constexpr uint16_t EEPROM_REVISION_ADDR = 0x5555u;
static constexpr uint16_t EEPROM_SERIAL_ADDR   = 0x5560u;
static constexpr uint16_t SERIAL_LENGTH        = 7u;

void AppOrchestrator::init()
{
    uint16_t revision = 0u;
    char serial[SERIAL_LENGTH + 1u] = {0};

    PLATFORM_ASSERT(eeprom.read(EEPROM_REVISION_ADDR,
                                reinterpret_cast<uint8_t *>(&revision),
                                sizeof(revision)) == IEepromRead::OK,
                    "Failed to read revision");

    tempSensor.init(revision);

    PLATFORM_ASSERT(eeprom.read(EEPROM_SERIAL_ADDR,
                                reinterpret_cast<uint8_t *>(serial),
                                SERIAL_LENGTH) == IEepromRead::OK,
                    "Failed to read serial number");

    PLATFORM_LOG_TAG(APP_LOG_TAG, "serial number %s, revision %u\n",
                     serial, static_cast<unsigned int>(revision));

    led.init();

    /* Init order: DMA, A/D, then the timers that start everything. */
    PLATFORM_ASSERT(dma.init(tempSensor.getBuffer(), tempSensor.getBufferSize()) == IDma::OK,
                    "DMA init failed");
    PLATFORM_ASSERT(adc.init() == IAdc::OK, "ADC init failed");
    PLATFORM_ASSERT(tickTimer.init() == ITimer::OK, "Tick timer init failed");
    PLATFORM_ASSERT(adTriggerTimer.init() == ITimer::OK, "AD trigger timer init failed");
}

void AppOrchestrator::performServices()
{
    if (tickTimer.get_1ms_flag())
    {
        tickTimer.clear_1ms_flag();

        uint16_t half = dma.get_half_event_number();
        uint16_t full = dma.get_full_event_number();

        if (half != halfEventNumber)
        {
            halfEventNumber = half;
            tempSensor.processHalfEvent(ITempController::Event::HALF_TRANSFER);
        }

        if (full != fullEventNumber)
        {
            fullEventNumber = full;
            tempSensor.processHalfEvent(ITempController::Event::FULL_TRANSFER);
        }

        updateLeds();
    }
}

uint16_t AppOrchestrator::getCountsPerDeg() const
{
    return tempSensor.getCountsPerDeg();
}

void AppOrchestrator::updateLeds()
{
    ITempController::Condition condition = tempSensor.getCondition();

    if (condition != shownCondition)
    {
        shownCondition = condition;

        switch (condition)
        {
            case ITempController::Condition::NORMAL:
                led.setActive(ILedController::Color::GREEN);
                break;

            case ITempController::Condition::WARNING:
                led.setActive(ILedController::Color::YELLOW);
                break;

            case ITempController::Condition::CRITICAL:
                led.setActive(ILedController::Color::RED);
                break;

            default:
                PLATFORM_ASSERT(0, "Unknown temp sensor condition");
                break;
        }
    }
}
