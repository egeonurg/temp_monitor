#include "eeprom_ifa.h"
#include "eeprom_inc.h"

uint8_t eeprom_read(uint16_t address, void *data, uint16_t size)
{
    if (EEPROM_I2C_READ(address, data, size) != EEPROM_I2C_OK)
    {
        return EEPROM_ERR;
    }

    return EEPROM_OK;
}
