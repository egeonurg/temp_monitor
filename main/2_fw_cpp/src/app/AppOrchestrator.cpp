#include "AppOrchestrator.hpp"
#include "app_facade.hpp"

#include "platform_assert.h"
#include "platform_log.h"

#define APP_LOG(...) PLATFORM_LOG("#APP_LOG " __VA_ARGS__)

/* EEPROM memory map */
static constexpr uint16_t EEPROM_REVISION_ADDR = 0x5555u;
static constexpr uint16_t EEPROM_SERIAL_ADDR   = 0x5560u;
static constexpr uint16_t SERIAL_LENGTH        = 7u;

void AppOrchestrator::init()
{
    uint16_t revision = 0u;
    char serial[SERIAL_LENGTH + 1u] = {0};

    PLATFORM_ASSERT(app_facade.service.eeprom.read(EEPROM_REVISION_ADDR,
                                           reinterpret_cast<uint8_t *>(&revision),
                                           sizeof(revision)) == IEepromRead::OK,
                    "Failed to read revision");

    PLATFORM_ASSERT(revision <= 1u, "Unknown temp sensor revision");
    
    countsPerDeg = (revision == 0u) ? 1u : 10u;

    PLATFORM_ASSERT(app_facade.service.eeprom.read(EEPROM_SERIAL_ADDR,
                                           reinterpret_cast<uint8_t *>(serial),
                                           SERIAL_LENGTH) == IEepromRead::OK,
                    "Failed to read serial number");

    APP_LOG("serial number %s, revision %u\n", serial, static_cast<unsigned int>(revision));

    app_facade.service.led.init();

    /* Init order: DMA, A/D, then the timers that start everything. */
    PLATFORM_ASSERT(app_facade.bsp.dma.init(sampleBuffer, SAMPLE_COUNT) == IDma::OK, "DMA init failed");
    PLATFORM_ASSERT(app_facade.bsp.adc.init() == IAdc::OK, "ADC init failed");
    PLATFORM_ASSERT(app_facade.bsp.tick_timer.init() == ITimer::OK, "Tick timer init failed");
    PLATFORM_ASSERT(app_facade.bsp.ad_trigger_timer.init() == ITimer::OK, "AD trigger timer init failed");
}

void AppOrchestrator::performServices()
{
    if (app_facade.bsp.tick_timer.get_1ms_flag())
    {
        app_facade.bsp.tick_timer.clear_1ms_flag();

        uint16_t half = app_facade.bsp.dma.get_half_event_number();
        uint16_t full = app_facade.bsp.dma.get_full_event_number();

        if (half != halfEventNumber)
        {
            halfEventNumber = half;
            /* First half of sampleBuffer is ready. */
        }

        if (full != fullEventNumber)
        {
            fullEventNumber = full;
            /* Second half of sampleBuffer is ready. */
        }
    }
}
