#include "LedController.hpp"
#include "igpio_pin.hpp"

/* Active high */
static constexpr uint8_t LED_ON  = 0x01u;
static constexpr uint8_t LED_OFF = 0x00u;

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
}
