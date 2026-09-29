#include "eeprom_ifa.h"
#include "eeprom_inc.h"

uint8_t eeprom_read(uint16_t address, void *data, uint16_t size)
{
    uint8_t ret = EEPROM_OK;

    if (EEPROM_I2C_READ(address, data, size) != EEPROM_I2C_OK)
    {
        ret = EEPROM_ERR;
    }

    return ret;
}
