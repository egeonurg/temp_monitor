#ifndef ADC_IFA_H
#define ADC_IFA_H

#include <stdint.h>

#define ADC_OK  0x00
#define ADC_ERR 0x01

extern uint8_t adc_init(void);

extern uint8_t adc_deinit(void);

#endif /* ADC_IFA_H */
