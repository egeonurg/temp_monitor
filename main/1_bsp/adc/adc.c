#include "adc_ifa.h"
#include "adc_inc.h"

/* Mock */
uint8_t adc_init(void)
{
    PLATFORM_LOG_TAG(ADC_LOG_TAG, "init: %u bit, trigger source %u (timer event)\n",
                     (unsigned int)ADC_RESOLUTION_BITS,
                     (unsigned int)ADC_TRIGGER_SOURCE);

    return ADC_OK;
}

/* Mock */
uint8_t adc_deinit(void)
{
    PLATFORM_LOG_TAG(ADC_LOG_TAG, "deinit\n");

    return ADC_OK;
}
