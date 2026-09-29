#ifndef TIM_IFA_H
#define TIM_IFA_H

#include <stdint.h>

#define TIM_OK  0x00
#define TIM_ERR 0x01

/* TIM0: 1 ms system tick, TIM1: A/D trigger (see tim_cfg.h) */
#define TIM0      0x00
#define TIM1      0x01
#define TIM_COUNT 0x02

extern uint8_t tim_init(uint8_t tim_id);

extern uint8_t tim_deinit(uint8_t tim_id);

/* Interrupt handler */
extern void tim0_periodic_isr(void);

extern uint8_t tim_get_1ms_flag(void);
extern void    tim_clear_1ms_flag(void);

#endif /* TIM_IFA_H */
