#pragma once

#include "IEepromRead.hpp"

class II2c; // Forward declaration, since interface is reference

/* I2C address of the EEPROM device. */


class EepromController final : public IEepromRead
{
public:
    EepromController(II2c& i2c_ifa):i2cInterface(i2c_ifa){}

    uint8_t read(uint16_t reg_address,
                 uint8_t *data,
                 uint16_t size) override;
    
private:
    II2c& i2cInterface;
    static constexpr auto EEPROM_I2C_SLAVE_ADDR = 0x50u;
};