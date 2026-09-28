#ifndef I2C_IFA_H
#define I2C_IFA_H

#include <stdint.h>

#define I2C_OK      0x00
#define I2C_ERR     0x01
#define I2C_TIMEOUT 0x02

/* Blocking read: returns only once the transfer finished or timed out. The
   register address is 16 bit, as word addressed devices such as 24Cxx EEPROMs
   require. */
extern uint8_t i2c_read(uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t size);

#endif /* I2C_IFA_H */
