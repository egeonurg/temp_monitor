#ifndef DMA_CFG_H
#define DMA_CFG_H

/* Peripheral request that drives the transfers. */
#define DMA_REQUEST_SOURCE_ADC 0x01u
#define DMA_REQUEST_SOURCE     DMA_REQUEST_SOURCE_ADC

/* Largest half buffer the transfer interrupts copy out. */
#define DMA_WORK_BUFFER_SIZE 100u

/* Mock: log one in every N transfers. Logging each one would flood the output,
   and a real driver would not print from interrupt context at all. */
#define DMA_LOG_INTERVAL 50u

#endif /* DMA_CFG_H */
