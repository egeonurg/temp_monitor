#ifndef DMA_HPP
#define DMA_HPP

#include "idma.hpp"

class Dma final : public IDma
{
public:
    uint8_t init(uint16_t *buffer, uint16_t size) override;
    uint8_t deinit() override;

    uint8_t get_half_flag() override;
    void    clear_half_flag() override;

    uint8_t get_full_flag() override;
    void    clear_full_flag() override;
};

#endif /* DMA_HPP */
