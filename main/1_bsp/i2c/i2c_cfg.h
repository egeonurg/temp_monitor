#ifndef I2C_CFG_H
#define I2C_CFG_H

/* Mock read value: 0x00 = revision A, 0x01 = revision B */
#define I2C_MOCK_READ_VALUE 0x00u

/* Mock serial number, returned for this register (matches app_cfg.h). */
#define I2C_MOCK_SERIAL_NUMBER_REG 0x5560u
#define I2C_MOCK_SERIAL_NUMBER     "ABC1234"

#endif /* I2C_CFG_H */
