#ifndef TIM_CFG_H
#define TIM_CFG_H

#define TIM_CLOCK_HZ 48000000u

/* 16 bit counter */
#define TIM_MAX_RELOAD 0xFFFFu

#define TIM_RELOAD_FROM_US(period_us) ((TIM_CLOCK_HZ / 1000000u) * (period_us))

/* TIM0: 1 ms system tick */
#define TIM0_PERIOD_MS 1u
#define TIM0_PERIOD_US (TIM0_PERIOD_MS * 1000u)
#define TIM0_RELOAD    TIM_RELOAD_FROM_US(TIM0_PERIOD_US)

/* TIM1: A/D trigger, sets the sampling rate */
#define TIM1_TRIGGER_HZ 10000u
#define TIM1_PERIOD_US  (1000000u / TIM1_TRIGGER_HZ)
#define TIM1_RELOAD     TIM_RELOAD_FROM_US(TIM1_PERIOD_US)

/* Mock: log every Nth TIM0 interrupt */
#define TIM0_LOG_INTERVAL 1000u

#endif /* TIM_CFG_H */
