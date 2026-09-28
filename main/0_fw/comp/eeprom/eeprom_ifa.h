#ifndef EEPROM_IFA_H
#define EEPROM_IFA_H

#include <stdint.h>

#define EEPROM_OK  0x00
#define EEPROM_ERR 0x01

/* The address is an offset into the EEPROM, not a pointer into memory. */
extern uint8_t eeprom_read(uint16_t address, void *data, uint16_t size);

/* Reads exactly as many bytes as the destination holds. */
#define EEPROM_READ(address, data) eeprom_read(address, data, (uint16_t)sizeof(*(data)))

#endif /* EEPROM_IFA_H */
