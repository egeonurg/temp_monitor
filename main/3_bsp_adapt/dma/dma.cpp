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

uint8_t Dma::get_half_flag()
{
    return dma_get_half_flag();
}

void Dma::clear_half_flag()
{
    dma_clear_half_flag();
}

uint8_t Dma::get_full_flag()
{
    return dma_get_full_flag();
}

void Dma::clear_full_flag()
{
    dma_clear_full_flag();
}
