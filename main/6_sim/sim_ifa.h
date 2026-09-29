#ifndef SIM_IFA_H
#define SIM_IFA_H

#include <stdint.h>

#define SIM_OK  0x00
#define SIM_ERR 0x01

/* Runs the simulated interrupts on their own thread. */
extern uint8_t sim_start(uint16_t counts_per_deg);

/* 0 once the configured run time has elapsed */
extern uint8_t sim_is_running(void);

#endif /* SIM_IFA_H */
