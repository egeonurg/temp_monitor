#ifndef IADC_HPP
#define IADC_HPP

#include <cstdint>

class IAdc
{
public:
    static constexpr uint8_t OK  = 0x00u;
    static constexpr uint8_t ERR = 0x01u;

    virtual uint8_t init()   = 0;
    virtual uint8_t deinit() = 0;

protected:
    IAdc()  = default;
    ~IAdc() = default;

    IAdc(const IAdc &)            = delete;
    IAdc &operator=(const IAdc &) = delete;
};

#endif /* IADC_HPP */
