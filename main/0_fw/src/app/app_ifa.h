#ifndef APP_IFA_H
#define APP_IFA_H

#include <stdint.h>

extern void app_init(void);

extern uint8_t app_get_1ms_flag(void);
extern void app_clear_1ms_flag(void);
extern void app_handle_1ms_event(void);

extern uint16_t app_get_counts_per_deg(void);

#endif /* APP_IFA_H */
