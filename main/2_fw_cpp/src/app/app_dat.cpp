#include "app_facade.hpp"

#include "adc.hpp"
#include "dma.hpp"
#include "gpio_pin.hpp"
#include "i2c.hpp"
#include "timer.hpp"
#include "tick_timer.hpp"
#include "EepromController.hpp"
#include "LedController.hpp"

static constexpr uint8_t LED_GREEN_PIN  = 0x0Au;
static constexpr uint8_t LED_RED_PIN    = 0x0Bu;
static constexpr uint8_t LED_YELLOW_PIN = 0x0Cu;

static constexpr uint8_t LED_PIN_OUTPUT = 0x01u;

/* BSP */
static Adc       adc;
static Dma       dma;
static I2c       i2c;
static Timer     ad_trigger_timer{Timer::ID_TIM1};
static TickTimer tick_timer;
static GpioPin   led_green_pin{LED_GREEN_PIN, LED_PIN_OUTPUT};
static GpioPin   led_red_pin{LED_RED_PIN, LED_PIN_OUTPUT};
static GpioPin   led_yellow_pin{LED_YELLOW_PIN, LED_PIN_OUTPUT};

/* Services */
static EepromController eeprom{i2c};
static LedController    led{led_green_pin, led_yellow_pin, led_red_pin};

const AppFacade app_facade =
{
    /* bsp */
    {
        adc,
        dma,
        i2c,
        ad_trigger_timer,
        tick_timer,
        /* led */
        {
            led_green_pin,
            led_red_pin,
            led_yellow_pin
        }
    },
    /* service */
    {
        eeprom,
        led
    }
};
