#ifndef DMA_IFA_H
#define DMA_IFA_H

#include <stdint.h>

#define DMA_OK  0x00
#define DMA_ERR 0x01

/* buffer is owned by the caller. */
extern uint8_t dma_init(uint16_t *buffer, uint16_t size);

extern uint8_t dma_deinit(void);

/* Interrupt handlers */
extern void dma_half_transfer_isr(void);
extern void dma_full_transfer_isr(void);

extern void dma_get_transfer_counts(uint32_t *half, uint32_t *full);

#if defined(SIM_ENABLE)
/* Called by the simulator for every converted sample. */
extern void dma_mock_sample(uint16_t sample);
#endif

extern uint16_t dma_get_half_event_number(void);

extern uint16_t dma_get_full_event_number(void);

#endif /* DMA_IFA_H */
