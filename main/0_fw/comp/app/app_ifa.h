#ifndef APP_IFA_H
#define APP_IFA_H

#include <stdint.h>

/* Wires the device drivers together at start-up: reads the stored
   configuration and hands it to the drivers that need it. */
extern void app_init(void);

/* 1 ms event flag for the superloop to poll. */
extern uint8_t app_get_1ms_flag(void);
extern void app_clear_1ms_flag(void);
extern void app_handle_1ms_event(void);

#endif /* APP_IFA_H */
