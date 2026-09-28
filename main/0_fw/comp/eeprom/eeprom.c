#include "eeprom_ifa.h"
#include "eeprom_inc.h"

uint16_t eeprom_read(uint16_t address, void *data, uint16_t size)
{
    uint16_t ret = EEPROM_READ_ERR;
    uint8_t result = EEPROM_I2C_READ(address, data, size);

    if (result == EEPROM_I2C_OK)
    {
        ret = EEPROM_READ_OK;
    }
    else
    {
        ret = EEPROM_READ_ERR;
    }
    return ret;
}
