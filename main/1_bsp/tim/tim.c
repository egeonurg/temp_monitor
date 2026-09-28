#include "tim_ifa.h"
#include "tim_inc.h"

typedef struct
{
    uint32_t reload;     /* value written to the counter reload register */
    uint32_t period_us;  /* event period the reload value produces      */
    uint8_t  mode;       /* what the update event drives                */
} tim_config_t;

/* Per-instance configuration, indexed by timer id. Adding a timer is a table
   entry plus a few defines in tim_cfg.h, not new code. */
static const tim_config_t tim_config[TIM_COUNT] =
{
    { TIM0_RELOAD, TIM0_PERIOD_US, TIM_MODE_INTERRUPT  },
    { TIM1_RELOAD, TIM1_PERIOD_US, TIM_MODE_AD_TRIGGER }
};

/* Catch a mistyped period at compile time rather than on the bench. */
_Static_assert(TIM0_PERIOD_US > 0u, "TIM0 period must be non-zero");
_Static_assert(TIM1_PERIOD_US > 0u, "TIM1 period must be non-zero");
_Static_assert(TIM0_RELOAD <= TIM_MAX_RELOAD, "TIM0 reload exceeds counter width");
_Static_assert(TIM1_RELOAD <= TIM_MAX_RELOAD, "TIM1 reload exceeds counter width");

static uint8_t tim_id_is_valid(uint8_t tim_id)
{
    return (uint8_t)(tim_id < TIM_COUNT);
}

uint8_t tim_init(uint8_t tim_id)
{
    if (tim_id_is_valid(tim_id) == 0u)
    {
        return TIM_ERR;
    }

    /* Mock: on target this programs the prescaler and reload registers,
       clears the counter and enables the peripheral. */
    TIM_LOG("TIM%u init: reload %u ticks, period %u us @ %u Hz\n",
           (unsigned int)tim_id,
           (unsigned int)tim_config[tim_id].reload,
           (unsigned int)tim_config[tim_id].period_us,
           (unsigned int)TIM_CLOCK_HZ);

    if (tim_config[tim_id].mode == TIM_MODE_AD_TRIGGER)
    {
        /* Mock: route the update event to the A/D trigger input. The
           conversion starts in hardware, so no interrupt is enabled. */
        TIM_LOG("TIM%u update event routed to A/D trigger\n",
               (unsigned int)tim_id);
    }
    else
    {
        /* Mock: enable the update interrupt in the interrupt controller.
           The handler itself is application code, bound by the vector table. */
        TIM_LOG("TIM%u update interrupt enabled (%u us period)\n",
               (unsigned int)tim_id,
               (unsigned int)tim_config[tim_id].period_us);
    }

    return TIM_OK;
}

uint8_t tim_deinit(uint8_t tim_id)
{
    if (tim_id_is_valid(tim_id) == 0u)
    {
        return TIM_ERR;
    }

    /* Mock: on target this stops the counter and disables the update event,
       masking the interrupt or releasing the A/D trigger accordingly. */
    TIM_LOG("TIM%u deinit: counter stopped, event disabled\n",
           (unsigned int)tim_id);

    return TIM_OK;
}

/* Set by the TIM0 interrupt, cleared by whoever handles the 1 ms event. A
   single byte, so the read and write are atomic and need no critical section. */
static volatile uint8_t  tim0_1ms_flag   = 0u;
static volatile uint32_t tim0_tick_count = 0u;

void tim0_periodic_isr(void)
{
    tim0_1ms_flag = 1u;
    tim0_tick_count++;

    if ((tim0_tick_count % TIM0_LOG_INTERVAL) == 0u)
    {
        TIM_LOG("TIM0 interrupt %u (%u ms elapsed)\n",
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
