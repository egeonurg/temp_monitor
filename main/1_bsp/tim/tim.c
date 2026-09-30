#include "tim_ifa.h"
#include "tim_inc.h"

static const uint16_t tim_period_us[TIM_COUNT] =
{
    TIM0_PERIOD_US,
    TIM1_PERIOD_US
};

/* Single byte, so read and write are atomic. */
static volatile uint8_t tim0_1ms_flag = 0u;

/* Mock */
uint8_t tim_init(uint8_t tim_id)
{
    uint8_t ret = TIM_ERR;

    if (tim_id < TIM_COUNT)
    {
        PLATFORM_LOG_TAG(TIM_LOG_TAG, "TIM%u init: period %u us\n",
                         (unsigned int)tim_id,
                         (unsigned int)tim_period_us[tim_id]);

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
}

uint8_t tim_get_1ms_flag(void)
{
    return tim0_1ms_flag;
}

void tim_clear_1ms_flag(void)
{
    tim0_1ms_flag = 0u;
}
