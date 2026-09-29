#ifndef ADC_CFG_H
#define ADC_CFG_H

#define ADC_RESOLUTION_BITS 12u

/* Conversions are started by the TIM1 trigger. */
#define ADC_TRIGGER_SOURCE_TIM_EVENT 0x01u
#define ADC_TRIGGER_SOURCE           ADC_TRIGGER_SOURCE_TIM_EVENT

#endif /* ADC_CFG_H */
