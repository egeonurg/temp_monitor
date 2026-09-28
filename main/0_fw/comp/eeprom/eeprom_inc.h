#ifndef EEPROM_INC_H
#define EEPROM_INC_H

#include <stdint.h>

#include "i2c_ifa.h"
#include "eeprom_cfg.h"

#define EEPROM_I2C_OK      I2C_OK
#define EEPROM_I2C_ERR     I2C_ERR
#define EEPROM_I2C_TIMEOUT I2C_TIMEOUT

#define EEPROM_I2C_READ(address, data, size) \
    i2c_read(EEPROM_I2C_SLAVE_ADDRESS, address, (uint8_t *)(data), size)

#endif /* EEPROM_INC_H */
