#ifndef SIM_IFA_H
#define SIM_IFA_H

#include <stdint.h>

#define SIM_OK  0x00
#define SIM_ERR 0x01

/* Starts the simulated hardware on its own thread, so its interrupts preempt
   the superloop the way real ones do. It drives the BSP drivers directly and
   knows nothing about the layers above them. */
extern uint8_t sim_start(void);

/* Superloop condition: false once the configured run time has elapsed, so a
   host build terminates instead of spinning forever. */
extern uint8_t sim_is_running(void);

#endif /* SIM_IFA_H */
