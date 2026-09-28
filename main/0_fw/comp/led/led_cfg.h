#ifndef LED_CFG_H
#define LED_CFG_H

/* Pin assigned to each LED. */
#define LED_GREEN_PIN  0x0Au
#define LED_RED_PIN    0x0Bu
#define LED_YELLOW_PIN 0x0Cu

/* The LEDs are driven high to light them. */
#define LED_LEVEL_ON  0x01u
#define LED_LEVEL_OFF 0x00u

/* Pin direction for an output, as the GPIO driver expects it. */
#define LED_PIN_OUTPUT 0x01u

/* Lit once the component is initialised. */
#define LED_DEFAULT_ACTIVE LED_GREEN

#endif /* LED_CFG_H */
