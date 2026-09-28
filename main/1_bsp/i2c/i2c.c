#include "i2c_ifa.h"
#include "i2c_inc.h"

/* Mock */
uint8_t i2c_read(uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t size)
{
    static const char serial[] = I2C_MOCK_SERIAL_NUMBER;
    uint16_t index = 0u;

    (void)slave_address;

    if (reg_address == I2C_MOCK_SERIAL_NUMBER_REG)
    {
        for (index = 0u; (index < size) && (index < sizeof(serial)); index++)
        {
            data[index] = (uint8_t)serial[index];
        }
    }
    else
    {
        data[0] = I2C_MOCK_READ_VALUE;
    }

    return I2C_OK;
}
