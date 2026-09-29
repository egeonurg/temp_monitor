#include "i2c.hpp"

extern "C"
{
#include "i2c_ifa.h"
}

static_assert(II2c::OK == I2C_OK, "II2c::OK differs from I2C_OK");
static_assert(II2c::ERR == I2C_ERR, "II2c::ERR differs from I2C_ERR");
static_assert(II2c::TIMEOUT == I2C_TIMEOUT, "II2c::TIMEOUT differs from I2C_TIMEOUT");

uint8_t I2c::read(uint8_t slave_address, uint16_t reg_address,
                  uint8_t *data, uint16_t size)
{
    return i2c_read(slave_address, reg_address, data, size);
}
