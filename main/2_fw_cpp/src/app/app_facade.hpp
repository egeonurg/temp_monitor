#ifndef APP_FACADE_HPP
#define APP_FACADE_HPP

#include "iadc.hpp"
#include "idma.hpp"
#include "igpio_pin.hpp"
#include "ii2c.hpp"
#include "itimer.hpp"
#include "IEepromRead.hpp"
#include "ILedController.hpp"

struct AppFacade
{
    struct Bsp
    {
        IAdc       &adc;
        IDma       &dma;
        II2c       &i2c;
        ITimer     &ad_trigger_timer; /* TIM1 */
        ITickTimer &tick_timer;       /* TIM0 */

        struct Led
        {
            IGpioPin &green;
            IGpioPin &red;
            IGpioPin &yellow;
        } led;
    } bsp;

    struct Service
    {
        IEepromRead    &eeprom;
        ILedController &led;
    } service;
};

/* Defined in app_dat.cpp */
extern const AppFacade app_facade;

#endif /* APP_FACADE_HPP */
