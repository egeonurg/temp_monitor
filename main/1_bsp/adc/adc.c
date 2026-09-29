#include "adc_ifa.h"
#include "adc_inc.h"

/* Mock */
uint8_t adc_init(void)
{
    ADC_LOG("init: %u bit, trigger source %u (timer event)\n",
           (unsigned int)ADC_RESOLUTION_BITS,
           (unsigned int)ADC_TRIGGER_SOURCE);

    return ADC_OK;
}

/* Mock */
uint8_t adc_deinit(void)
{
    ADC_LOG("deinit\n");

    return ADC_OK;
}
