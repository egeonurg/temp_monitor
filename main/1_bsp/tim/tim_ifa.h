#ifndef TIM_IFA_H
#define TIM_IFA_H

#include <stdint.h>

#define TIM_OK  0x00
#define TIM_ERR 0x01

/* Timer instances. What each one does is fixed by tim_cfg.h, not by the caller:
   TIM0 raises the 1 ms system interrupt, TIM1 triggers the A/D in hardware. */
#define TIM0      0x00
#define TIM1      0x01
#define TIM_COUNT 0x02

extern uint8_t tim_init(uint8_t tim_id);

extern uint8_t tim_deinit(uint8_t tim_id);

/* Vector table entry for the TIM0 update interrupt. */
extern void tim0_periodic_isr(void);

/* 1 ms event flag, set by the interrupt and cleared by whoever handles it. */
extern uint8_t tim_get_1ms_flag(void);
extern void    tim_clear_1ms_flag(void);

#endif /* TIM_IFA_H */
