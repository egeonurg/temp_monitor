#ifndef APP_CFG_H
#define APP_CFG_H

/* EEPROM memory map. Owned by the integration layer: no driver should know
   where another driver's data is stored. */
#define EEPROM_TEMP_SENSOR_ADDRESS    0x5555u
#define EEPROM_SERIAL_NUMBER_ADDRESS  0x5560u
#define EEPROM_SERIAL_NUMBER_LENGTH   7u        /* e.g. "ABC1234" */

#endif /* APP_CFG_H */
