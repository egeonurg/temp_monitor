#include "led_ifa.h"
#include "led_inc.h"

/* Pin of each LED, indexed by led id. */
static const uint8_t led_pin[LED_COUNT] =
{
    LED_GREEN_PIN,
    LED_RED_PIN,
    LED_ORANGE_PIN
};

static uint8_t led_active = LED_DEFAULT_ACTIVE;

static uint8_t led_id_is_valid(uint8_t led_id)
{
    return (uint8_t)(led_id < LED_COUNT);
}

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
    uint8_t index = 0u;

    if (led_id_is_valid(led_id) == 0u)
    {
        return LED_ERR;
    }

    /* Drive every pin on the way through, so exactly one LED ends up lit
       whatever state they were in. */
    for (index = 0u; index < LED_COUNT; index++)
    {
        if (index == led_id)
        {
            LED_GPIO_WRITE(led_pin[index], LED_LEVEL_ON);
        }
        else
        {
            LED_GPIO_WRITE(led_pin[index], LED_LEVEL_OFF);
        }
    }

    led_active = led_id;

    LED_LOG("active LED %u (pin %u)\n",
            (unsigned int)led_active,
            (unsigned int)led_pin[led_active]);

    return LED_OK;
}
