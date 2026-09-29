#include "dma.hpp"

extern "C"
{
#include "dma_ifa.h"
}

static_assert(IDma::OK == DMA_OK, "IDma::OK differs from DMA_OK");
static_assert(IDma::ERR == DMA_ERR, "IDma::ERR differs from DMA_ERR");

uint8_t Dma::init(uint16_t *buffer, uint16_t size)
{
    return dma_init(buffer, size);
}

uint8_t Dma::deinit()
{
    return dma_deinit();
}

void Dma::get_transfer_counts(uint32_t *half, uint32_t *full)
{
    dma_get_transfer_counts(half, full);
}

uint16_t Dma::get_half_event_number()
{
    return dma_get_half_event_number();
}

uint16_t Dma::get_full_event_number()
{
    return dma_get_full_event_number();
}
