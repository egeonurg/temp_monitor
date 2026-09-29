#pragma once

#include "ILedController.hpp"

class IGpioPin; // Forward declaration, since interface is reference

class LedController final : public ILedController
{
public:
    LedController(IGpioPin &green, IGpioPin &yellow, IGpioPin &red)
        : greenPin(green), yellowPin(yellow), redPin(red) {}

    void init() override;
    void setActive(Color color) override;

private:
    IGpioPin &greenPin;
    IGpioPin &yellowPin;
    IGpioPin &redPin;
};
