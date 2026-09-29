#pragma once

#include <cstdint>

/* Led Controller Interface */
class ILedController
{
public:
    enum class Color : uint8_t
    {
        GREEN,
        YELLOW,
        RED
    };

    virtual void init() = 0;

    virtual void setActive(Color color) = 0;

protected:
    ILedController()  = default;
    ~ILedController() = default;

    ILedController(const ILedController &)            = delete;
    ILedController &operator=(const ILedController &) = delete;
};
