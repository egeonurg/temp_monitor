#include "EepromController.hpp"
#include "ii2c.hpp"

static_assert(IEepromRead::OK == II2c::OK, "IEepromRead::OK differs from I2C_OK");
static_assert(IEepromRead::ERR == II2c::ERR, "IEepromRead::ERR differs from I2C_ERR");
static_assert(IEepromRead::TIMEOUT == II2c::TIMEOUT, "IEepromRead::TIMEOUT differs from I2C_TIMEOUT");


uint8_t EepromController::read(uint16_t reg_address,
                               uint8_t *data, 
                               uint16_t size)
{
    uint8_t ret = IEepromRead::ERR;
    uint8_t i2c_ret = i2cInterface.read(EEPROM_I2C_SLAVE_ADDR, reg_address, data, size);

    if(i2c_ret != II2c::OK)
    {
        ret = IEepromRead::ERR;
    }
    else
    {
        ret = IEepromRead::OK;
    }
    return ret; 
}