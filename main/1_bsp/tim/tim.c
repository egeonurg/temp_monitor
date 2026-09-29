#include "tim_ifa.h"
#include "tim_inc.h"

typedef struct
{
    uint32_t reload;
    uint32_t period_us;
    uint8_t  mode;
} tim_config_t;

static const tim_config_t tim_config[TIM_COUNT] =
{
    { TIM0_RELOAD, TIM0_PERIOD_US, TIM_MODE_INTERRUPT  },
    { TIM1_RELOAD, TIM1_PERIOD_US, TIM_MODE_AD_TRIGGER }
};

_Static_assert(TIM0_PERIOD_US > 0u, "TIM0 period must be non-zero");
_Static_assert(TIM1_PERIOD_US > 0u, "TIM1 period must be non-zero");
_Static_assert(TIM0_RELOAD <= TIM_MAX_RELOAD, "TIM0 reload exceeds counter width");
_Static_assert(TIM1_RELOAD <= TIM_MAX_RELOAD, "TIM1 reload exceeds counter width");

/* Single byte, so read and write are atomic. */
static volatile uint8_t  tim0_1ms_flag   = 0u;
static volatile uint32_t tim0_tick_count = 0u;

/* Mock */
uint8_t tim_init(uint8_t tim_id)
{
    uint8_t ret = TIM_ERR;

    if (tim_id < TIM_COUNT)
    {
        PLATFORM_LOG_TAG(TIM_LOG_TAG, "TIM%u init: reload %u ticks, period %u us @ %u Hz\n",
                         (unsigned int)tim_id,
                         (unsigned int)tim_config[tim_id].reload,
                         (unsigned int)tim_config[tim_id].period_us,
                         (unsigned int)TIM_CLOCK_HZ);

        if (tim_config[tim_id].mode == TIM_MODE_AD_TRIGGER)
        {
            PLATFORM_LOG_TAG(TIM_LOG_TAG, "TIM%u update event routed to A/D trigger\n",
                             (unsigned int)tim_id);
        }
        else
        {
            PLATFORM_LOG_TAG(TIM_LOG_TAG, "TIM%u update interrupt enabled (%u us period)\n",
                             (unsigned int)tim_id,
                             (unsigned int)tim_config[tim_id].period_us);
        }

        ret = TIM_OK;
    }

    return ret;
}

/* Mock */
uint8_t tim_deinit(uint8_t tim_id)
{
    uint8_t ret = TIM_ERR;

    if (tim_id < TIM_COUNT)
    {
        PLATFORM_LOG_TAG(TIM_LOG_TAG, "TIM%u deinit\n", (unsigned int)tim_id);

        ret = TIM_OK;
    }

    return ret;
}

void tim0_periodic_isr(void)
{
    tim0_1ms_flag = 1u;
    tim0_tick_count++;

    if ((tim0_tick_count % TIM0_LOG_INTERVAL) == 0u)
    {
        PLATFORM_LOG_TAG(TIM_LOG_TAG, "TIM0 interrupt %u (%u ms elapsed)\n",
                         (unsigned int)tim0_tick_count,
                         (unsigned int)(tim0_tick_count * TIM0_PERIOD_MS));
    }
}

uint8_t tim_get_1ms_flag(void)
{
    return tim0_1ms_flag;
}

void tim_clear_1ms_flag(void)
{
    tim0_1ms_flag = 0u;
}
