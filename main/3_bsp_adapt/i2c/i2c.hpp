#ifndef I2C_HPP
#define I2C_HPP

#include "ii2c.hpp"

class I2c final : public II2c
{
public:
    uint8_t read(uint8_t slave_address, uint16_t reg_address,
                 uint8_t *data, uint16_t size) override;
};

#endif /* I2C_HPP */
