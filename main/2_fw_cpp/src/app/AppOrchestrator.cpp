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
    uint8_t revision = 0u;
    char serial[SERIAL_LENGTH + 1u] = {0};
    uint8_t ret = 0u;

    ret = eeprom.read(EEPROM_REVISION_ADDR, &revision, sizeof(revision));
    PLATFORM_ASSERT(ret == IEepromRead::OK, "Failed to read revision");

    tempSensor.init(revision);

    ret = eeprom.read(EEPROM_SERIAL_ADDR, reinterpret_cast<uint8_t *>(serial), SERIAL_LENGTH);
    PLATFORM_ASSERT(ret == IEepromRead::OK, "Failed to read serial number");

    PLATFORM_LOG_TAG(APP_LOG_TAG, "serial number %s\n", serial);

    PLATFORM_LOG_TAG(APP_LOG_TAG, "temp sensor revision %u, %u count(s) per degree\n",
                     static_cast<unsigned int>(revision),
                     static_cast<unsigned int>(tempSensor.getCountsPerDeg()));

    led.init();

    /* Init order: DMA, A/D, then the timers that start everything. */
    ret = dma.init(tempSensor.getBuffer(), tempSensor.getBufferSize());
    PLATFORM_ASSERT(ret == IDma::OK, "DMA init failed");

    ret = adc.init();
    PLATFORM_ASSERT(ret == IAdc::OK, "ADC init failed");

    ret = tickTimer.init();
    PLATFORM_ASSERT(ret == ITimer::OK, "Tick timer init failed");

    ret = adTriggerTimer.init();
    PLATFORM_ASSERT(ret == ITimer::OK, "AD trigger timer init failed");
}

void AppOrchestrator::performServices()
{
    if (tickTimer.get_1ms_flag())
    {
        tickTimer.clear_1ms_flag();

        if (dma.get_half_flag())
        {
            dma.clear_half_flag();
            tempSensor.evaluate(tempSensor.filterHalf(ITempController::Event::HALF_TRANSFER));
        }

        if (dma.get_full_flag())
        {
            dma.clear_full_flag();
            tempSensor.evaluate(tempSensor.filterHalf(ITempController::Event::FULL_TRANSFER));
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
