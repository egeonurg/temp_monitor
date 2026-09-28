#include "adc_ifa.h"
#include "adc_inc.h"

uint8_t adc_init(void)
{
    /* Mock: on target this powers up the converter, programs the resolution
       and selects the timer trigger event as the conversion start source. */
    ADC_LOG("init: %u bit, trigger source %u (timer event)\n",
           (unsigned int)ADC_RESOLUTION_BITS,
           (unsigned int)ADC_TRIGGER_SOURCE);

    return ADC_OK;
}

uint8_t adc_deinit(void)
{
    /* Mock: on target this releases the trigger input and powers the
       converter down. */
    ADC_LOG("deinit: trigger released, converter powered down\n");

    return ADC_OK;
}
