#ifndef DMA_HPP
#define DMA_HPP

#include "idma.hpp"

class Dma final : public IDma
{
public:
    uint8_t init(uint16_t *buffer, uint16_t size) override;
    uint8_t deinit() override;

    void get_transfer_counts(uint32_t *half, uint32_t *full) override;

    uint16_t get_half_event_number() override;
    uint16_t get_full_event_number() override;
};

#endif /* DMA_HPP */
