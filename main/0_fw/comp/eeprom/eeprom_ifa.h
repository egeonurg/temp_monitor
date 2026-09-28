#ifndef EEPROM_IFA_H
#define EEPROM_IFA_H

#include <stdint.h>

#define EEPROM_READ_ERR 0x01
#define EEPROM_READ_OK  0x00

/* The address is an offset into the EEPROM, not a pointer into memory. */
extern uint16_t eeprom_read(uint16_t address, void *data, uint16_t size);

#define EEPROM_READ(address, data) eeprom_read(address, data, sizeof(uint16_t))

#endif /* EEPROM_IFA_H */
