#pragma once

#include <cstdint>

/* Eeprom Read Interface */
class IEepromRead
{
public:
    virtual uint8_t read(uint16_t reg_address,
                         uint8_t *data, 
                         uint16_t size) = 0;
    static constexpr uint8_t OK      = 0x00u;
    static constexpr uint8_t ERR     = 0x01u;

protected:
    IEepromRead()  = default;
    ~IEepromRead() = default;

    IEepromRead(const IEepromRead &)            = delete;
    IEepromRead &operator=(const IEepromRead &) = delete;
};