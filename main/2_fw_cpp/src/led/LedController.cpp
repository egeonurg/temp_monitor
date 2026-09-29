#include "LedController.hpp"
#include "igpio_pin.hpp"

#include "platform_log.h"

#define LED_LOG_TAG "LED"

/* Active high */
static constexpr uint8_t LED_ON  = 0x01u;
static constexpr uint8_t LED_OFF = 0x00u;

/* Indexed by Color */
static const char * const COLOR_NAME[] =
{
    "GREEN",
    "YELLOW",
    "RED"
};

void LedController::init()
{
    greenPin.init();
    yellowPin.init();
    redPin.init();

    setActive(Color::GREEN);
}

void LedController::setActive(Color color)
{
    greenPin.write((color == Color::GREEN) ? LED_ON : LED_OFF);
    yellowPin.write((color == Color::YELLOW) ? LED_ON : LED_OFF);
    redPin.write((color == Color::RED) ? LED_ON : LED_OFF);

    PLATFORM_LOG_TAG(LED_LOG_TAG, "%s on\n", COLOR_NAME[static_cast<uint8_t>(color)]);
}
