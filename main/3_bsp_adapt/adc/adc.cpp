#include "adc.hpp"

extern "C"
{
#include "adc_ifa.h"
}

static_assert(IAdc::OK == ADC_OK, "IAdc::OK differs from ADC_OK");
static_assert(IAdc::ERR == ADC_ERR, "IAdc::ERR differs from ADC_ERR");

uint8_t Adc::init()
{
    return adc_init();
}

uint8_t Adc::deinit()
{
    return adc_deinit();
}
