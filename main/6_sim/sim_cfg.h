#ifndef SIM_CFG_H
#define SIM_CFG_H

/* One A/D sample per TIM1 trigger */
#define SIM_SAMPLE_PERIOD_US 100u

#define SIM_TICK_PERIOD_US 1000u

#define SIM_RUN_MS      5600u   /* set point list twice */
#define SIM_RUN_SAMPLES ((SIM_RUN_MS * 1000u) / SIM_SAMPLE_PERIOD_US)

#define SIM_SETPOINT_DURATION_MS 200u

/* Noise around the set point, in 0.1 C */
#define SIM_NOISE_TENTHS_DEG 5u

/* One spike every N samples, for the median filter */
#define SIM_SPIKE_INTERVAL    37u
#define SIM_SPIKE_TENTHS_DEG 200u

/* 12 bit A/D full scale */
#define SIM_ADC_MAX_COUNT 4095u

#endif /* SIM_CFG_H */
