#include "app_dat.hpp"
#include "AppOrchestrator.hpp"

#include "adc.hpp"
#include "dma.hpp"
#include "gpio_pin.hpp"
#include "i2c.hpp"
#include "timer.hpp"
#include "tick_timer.hpp"
#include "EepromController.hpp"
#include "LedController.hpp"
#include "TempController.hpp"

static constexpr uint8_t LED_GREEN_PIN  = 0x0Au;
static constexpr uint8_t LED_RED_PIN    = 0x0Bu;
static constexpr uint8_t LED_YELLOW_PIN = 0x0Cu;

static constexpr uint8_t LED_PIN_OUTPUT = 0x01u;

/* BSP */
static Adc       adc;
static Dma       dma;
static I2c       i2c;
static Timer     adTriggerTimer{Timer::ID_TIM1};
static TickTimer tickTimer;
static GpioPin   ledGreenPin{LED_GREEN_PIN, LED_PIN_OUTPUT};
static GpioPin   ledRedPin{LED_RED_PIN, LED_PIN_OUTPUT};
static GpioPin   ledYellowPin{LED_YELLOW_PIN, LED_PIN_OUTPUT};

/* Services */
static EepromController eeprom{i2c};
static LedController    led{ledGreenPin, ledYellowPin, ledRedPin};
static TempController   tempSensor;

/* App */
static AppOrchestrator app{eeprom, led, tempSensor, dma, adc, adTriggerTimer, tickTimer};

AppOrchestrator &getApp()
{
    return app;
}
