#ifndef TIM_CFG_H
#define TIM_CFG_H

/* Timer peripheral input clock. */
#define TIM_CLOCK_HZ 48000000u

/* Counter width of the hardware timers: a reload value must fit in it. */
#define TIM_MAX_RELOAD 0xFFFFu

/* Reload value that produces the requested period at TIM_CLOCK_HZ. */
#define TIM_RELOAD_FROM_US(period_us) ((TIM_CLOCK_HZ / 1000000u) * (period_us))

/* TIM0: periodic interrupt used as the 1 ms system tick. */
#define TIM0_PERIOD_MS 1u
#define TIM0_PERIOD_US (TIM0_PERIOD_MS * 1000u)
#define TIM0_RELOAD    TIM_RELOAD_FROM_US(TIM0_PERIOD_US)

/* TIM1: trigger source for the A/D converter. Its update event starts a
   conversion, so the period sets the sampling rate. */
#define TIM1_TRIGGER_HZ 10000u
#define TIM1_PERIOD_US  (1000000u / TIM1_TRIGGER_HZ)
#define TIM1_RELOAD     TIM_RELOAD_FROM_US(TIM1_PERIOD_US)

/* Mock: log one in every N TIM0 interrupts. Logging each one would flood the
   output, and a real driver would not print from interrupt context at all. */
#define TIM0_LOG_INTERVAL 1000u

#endif /* TIM_CFG_H */
