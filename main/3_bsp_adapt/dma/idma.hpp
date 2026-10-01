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

    /* Set by the interrupt handlers, cleared by the superloop. */
    virtual uint8_t get_half_flag()   = 0;
    virtual void    clear_half_flag() = 0;

    virtual uint8_t get_full_flag()   = 0;
    virtual void    clear_full_flag() = 0;

protected:
    IDma()  = default;
    ~IDma() = default;

    IDma(const IDma &)            = delete;
    IDma &operator=(const IDma &) = delete;
};

#endif /* IDMA_HPP */
