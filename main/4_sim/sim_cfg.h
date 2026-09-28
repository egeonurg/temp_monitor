#ifndef SIM_CFG_H
#define SIM_CFG_H

/* Base period of the simulation: one A/D sample per TIM1 trigger event. */
#define SIM_SAMPLE_PERIOD_US 100u

/* Period of the simulated TIM0 interrupt. */
#define SIM_TICK_PERIOD_US 1000u

/* How long a simulated run lasts before the superloop is asked to stop. */
#define SIM_RUN_MS      10000u
#define SIM_RUN_SAMPLES ((SIM_RUN_MS * 1000u) / SIM_SAMPLE_PERIOD_US)

/* How long each temperature set point is held before moving to the next. */
#define SIM_SETPOINT_DURATION_MS 200u

/* Noise added around the set point, in tenths of a degree. Converted to counts
   with the sensor resolution, and never less than one count. */
#define SIM_NOISE_TENTHS_DEG 5u

/* Outliers, so a median filter has something to reject: one spike every N
   samples, alternating above and below the set point. */
#define SIM_SPIKE_INTERVAL    37u
#define SIM_SPIKE_TENTHS_DEG 200u

/* Full scale of the simulated converter, matching ADC_RESOLUTION_BITS. */
#define SIM_ADC_MAX_COUNT 4095u

#endif /* SIM_CFG_H */
