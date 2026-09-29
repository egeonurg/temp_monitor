#ifndef ADC_INC_H
#define ADC_INC_H

#include <stdint.h>

#include "platform_log.h"

#include "adc_ifa.h"
#include "adc_cfg.h"

#define ADC_LOG_TAG "#ADC_LOG "
#define ADC_LOG(...)  PLATFORM_LOG(ADC_LOG_TAG __VA_ARGS__)

#endif /* ADC_INC_H */
