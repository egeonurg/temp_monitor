#include "led_ifa.h"
#include "led_inc.h"

static const uint8_t led_pin[LED_COUNT] =
{
    LED_GREEN_PIN,
    LED_RED_PIN,
    LED_YELLOW_PIN
};

static const char * const led_name[LED_COUNT] =
{
    "GREEN",
    "RED",
    "YELLOW"
};

static uint8_t led_active = LED_DEFAULT_ACTIVE;

uint8_t led_init(void)
{
    uint8_t led_id = 0u;

    for (led_id = 0u; led_id < LED_COUNT; led_id++)
    {
        LED_GPIO_INIT(led_pin[led_id], LED_PIN_OUTPUT);
    }

    LED_LOG("init: %u LEDs, default active %u\n",
            (unsigned int)LED_COUNT,
            (unsigned int)LED_DEFAULT_ACTIVE);

    return led_set_active(LED_DEFAULT_ACTIVE);
}

uint8_t led_deinit(void)
{
    uint8_t led_id = 0u;

    for (led_id = 0u; led_id < LED_COUNT; led_id++)
    {
        LED_GPIO_WRITE(led_pin[led_id], LED_LEVEL_OFF);
        LED_GPIO_DEINIT(led_pin[led_id], LED_PIN_OUTPUT);
    }

    LED_LOG("deinit: all LEDs off\n");

    return LED_OK;
}

uint8_t led_set_active(uint8_t led_id)
{
    uint8_t ret   = LED_ERR;
    uint8_t index = 0u;

    if (led_id < LED_COUNT)
    {
        for (index = 0u; index < LED_COUNT; index++)
        {
            LED_GPIO_WRITE(led_pin[index], (index == led_id) ? LED_LEVEL_ON : LED_LEVEL_OFF);
        }

        led_active = led_id;

        LED_LOG("%s on\n", led_name[led_active]);

        ret = LED_OK;
    }

    return ret;
}
