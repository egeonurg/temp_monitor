#ifndef IDMA_HPP
#define IDMA_HPP

#include <cstdint>

class IDma
{
public:
    static constexpr uint8_t OK  = 0x00u;
    static constexpr uint8_t ERR = 0x01u;

    /* buffer is owned by the caller. */
    virtual uint8_t init(uint16_t *buffer, uint16_t size) = 0;
    virtual uint8_t deinit()                              = 0;

    virtual void get_transfer_counts(uint32_t *half, uint32_t *full) = 0;

    virtual uint16_t get_half_event_number() = 0;
    virtual uint16_t get_full_event_number() = 0;

protected:
    IDma()  = default;
    ~IDma() = default;

    IDma(const IDma &)            = delete;
    IDma &operator=(const IDma &) = delete;
};

#endif /* IDMA_HPP */
