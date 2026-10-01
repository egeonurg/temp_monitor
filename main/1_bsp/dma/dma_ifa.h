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

#if defined(SIM_ENABLE)
/* Called by the simulator for every converted sample. */
extern void dma_mock_sample(uint16_t sample);
#endif

/* Set by the interrupt handlers. Returns the flag and clears it, with
   interrupts disabled, so no event is lost between the read and the clear. */
extern uint8_t dma_take_half_flag(void);
extern uint8_t dma_take_full_flag(void);

#endif /* DMA_IFA_H */
