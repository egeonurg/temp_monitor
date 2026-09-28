#ifndef DMA_IFA_H
#define DMA_IFA_H

#include <stdint.h>

#define DMA_OK  0x00
#define DMA_ERR 0x01

/* The destination buffer is owned by the caller: the driver only stores where
   it is and how many uint16_t samples fit in it. */
extern uint8_t dma_init(uint16_t *buffer, uint16_t size);

extern uint8_t dma_deinit(void);

/* Vector table entries. Each copies the half of the buffer that is now stable,
   so the transfer is handled here and not deferred to the superloop. */
extern void dma_half_transfer_isr(void);
extern void dma_full_transfer_isr(void);

extern void dma_get_transfer_counts(uint32_t *half, uint32_t *full);

/* Host mock only: models one completed conversion being written by the DMA,
   raising the transfer interrupts as the buffer fills. */
extern void dma_mock_sample(uint16_t sample);

extern uint16_t dma_get_half_event_number(void);

extern uint16_t dma_get_full_event_number(void);


#endif /* DMA_IFA_H */
