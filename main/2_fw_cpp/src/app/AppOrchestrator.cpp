#include "AppOrchestrator.hpp"

#include "iadc.hpp"
#include "idma.hpp"
#include "itimer.hpp"
#include "IEepromRead.hpp"
#include "ILedController.hpp"

#include "platform_assert.h"
#include "platform_log.h"
#include "platform_barrier.h"

#define APP_LOG_TAG "APP"

/* EEPROM memory map */
static constexpr uint16_t EEPROM_REVISION_ADDR = 0x5555u;
static constexpr uint16_t EEPROM_SERIAL_ADDR   = 0x5560u;
static constexpr uint16_t SERIAL_LENGTH        = 7u;

void AppOrchestrator::init()
{
    uint16_t revision = 0u;
    char serial[SERIAL_LENGTH + 1u] = {0};
    uint8_t ret = 0u;

    ret = eeprom.read(EEPROM_REVISION_ADDR, reinterpret_cast<uint8_t *>(&revision), sizeof(revision));
    PLATFORM_ASSERT(ret == IEepromRead::OK, "Failed to read revision");

    tempSensor.init(revision);

    ret = eeprom.read(EEPROM_SERIAL_ADDR, reinterpret_cast<uint8_t *>(serial), SERIAL_LENGTH);
    PLATFORM_ASSERT(ret == IEepromRead::OK, "Failed to read serial number");

    PLATFORM_LOG_TAG(APP_LOG_TAG, "serial number %s, revision %u\n",
                     serial, static_cast<unsigned int>(revision));

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

        /* Complete all memory accesses before reading the DMA event counters. */
        PLATFORM_DSB();
        uint16_t events    = static_cast<uint16_t>(dma.get_half_event_number() + dma.get_full_event_number());
        uint16_t newEvents = static_cast<uint16_t>(events - eventNumber);

        if (newEvents != 0u)
        {
            eventNumber = events;

            /* The DMA alternates half, full, half, ... so an odd count means the
               first half was filled last. uint16_t wrap-around keeps the parity. */
            ITempController::Event event = ((events % 2u) != 0u) ?
                                           ITempController::Event::HALF_TRANSFER :
                                           ITempController::Event::FULL_TRANSFER;

            /* Only the latest half is still intact, older ones are being overwritten. */
            if (newEvents > 1u)
            {
                PLATFORM_LOG_TAG(APP_LOG_TAG, "Overrun: skipped %u half buffer(s)\n",
                                 static_cast<unsigned int>(newEvents - 1u));
            }

            uint16_t value = tempSensor.filterHalf(event);

            /* The next event means the DMA is writing into this half again, so
               the value may mix old and new samples: drop it. */
            if (static_cast<uint16_t>(dma.get_half_event_number() + dma.get_full_event_number()) == events)
            {
                tempSensor.evaluate(value);
            }
            else
            {
                PLATFORM_LOG_TAG(APP_LOG_TAG, "Torn read: half buffer dropped\n");
            }
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
