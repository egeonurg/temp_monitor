#ifndef I2C_CFG_H
#define I2C_CFG_H

/* 0x00 = revision A, 0x01 = revision B */
#define I2C_MOCK_READ_VALUE 0x00u

/* Must match the serial number address in app_cfg.h */
#define I2C_MOCK_SERIAL_NUMBER_REG 0x5560u
#define I2C_MOCK_SERIAL_NUMBER     "ABC1234"

#endif /* I2C_CFG_H */
