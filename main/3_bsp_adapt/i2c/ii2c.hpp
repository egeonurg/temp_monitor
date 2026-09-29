#ifndef II2C_HPP
#define II2C_HPP

#include <cstdint>

class II2c
{
public:
    static constexpr uint8_t OK      = 0x00u;
    static constexpr uint8_t ERR     = 0x01u;
    static constexpr uint8_t TIMEOUT = 0x02u;

    virtual uint8_t read(uint8_t slave_address, uint16_t reg_address,
                         uint8_t *data, uint16_t size) = 0;

protected:
    II2c()  = default;
    ~II2c() = default;

    II2c(const II2c &)            = delete;
    II2c &operator=(const II2c &) = delete;
};

#endif /* II2C_HPP */
