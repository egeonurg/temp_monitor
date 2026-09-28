#include "i2c_ifa.h"
#include "i2c_inc.h"

/* Mock peripheral state. On real hardware these are register reads. */
static uint8_t i2c_bus_is_busy(void)
{
    return 0u;
}

static uint8_t i2c_receive_byte(uint16_t reg_address, uint16_t index)
{
    return (uint8_t)(reg_address + index);
}

uint8_t i2c_read(uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t size)
{
    uint16_t timeout = 0u;
    uint16_t index = 0u;

    if ((data == NULL) || (size == 0u))
    {
        return I2C_ERR;
    }

    /* Blocking: spin until the bus is free or the timeout expires. */
    while (i2c_bus_is_busy() != 0u)
    {
        timeout++;
        if (timeout >= I2C_READ_TIMEOUT)
        {
            return I2C_TIMEOUT;
        }
    }

    I2C_LOG("I2C read: slave 0x%02X, reg 0x%04X, %u byte(s)\n",
           slave_address, reg_address, size);

    for (index = 0u; index < size; index++)
    {
        data[index] = i2c_receive_byte(reg_address, index);
    }

    return I2C_OK;
}
