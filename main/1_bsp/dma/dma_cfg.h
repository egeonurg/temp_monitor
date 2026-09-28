#ifndef DMA_CFG_H
#define DMA_CFG_H

/* Peripheral request that drives the transfers. */
#define DMA_REQUEST_SOURCE_ADC 0x01u
#define DMA_REQUEST_SOURCE     DMA_REQUEST_SOURCE_ADC

/* Largest half buffer the transfer interrupts copy out. */
#define DMA_WORK_BUFFER_SIZE 100u


#endif /* DMA_CFG_H */
