#ifndef ADC_HPP
#define ADC_HPP

#include "iadc.hpp"

class Adc final : public IAdc
{
public:
    uint8_t init() override;
    uint8_t deinit() override;
};

#endif /* ADC_HPP */
