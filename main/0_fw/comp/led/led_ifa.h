#ifndef LED_IFA_H
#define LED_IFA_H

#include <stdint.h>

#define LED_OK  0x00
#define LED_ERR 0x01

/* Exactly one of these is lit at any time. */
#define LED_GREEN  0x00
#define LED_RED    0x01
#define LED_ORANGE 0x02
#define LED_COUNT  0x03

extern uint8_t led_init(void);

extern uint8_t led_deinit(void);

/* Lights the given LED and turns the other two off. */
extern uint8_t led_set_active(uint8_t led_id);

#endif /* LED_IFA_H */
