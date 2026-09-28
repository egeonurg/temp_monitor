#ifndef TEMP_SENSOR_CFG_H
#define TEMP_SENSOR_CFG_H

/* Counts per degree Celsius of each revision. */
#define TEMP_SENSOR_REVISION_A_PRESCALER 1u
#define TEMP_SENSOR_REVISION_B_PRESCALER 10u

#define TEMP_SENSOR_SAMPLE_COUNT 200u

/* Condition limits in degrees Celsius. */
#define TEMP_SENSOR_CRITICAL_LOW_DEG  5u    /* critical below this */
#define TEMP_SENSOR_WARNING_DEG       85u   /* warning from this   */
#define TEMP_SENSOR_CRITICAL_HIGH_DEG 105u  /* critical from this  */

/* A condition is left only this far back inside the band it came from. */
#define TEMP_SENSOR_HYSTERESIS_DEG 2u

#endif /* TEMP_SENSOR_CFG_H */
