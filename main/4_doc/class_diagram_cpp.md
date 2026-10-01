# C++ class diagram

Classes, interfaces and their dependencies in the C++ version. Every dependency
is a reference to an interface, passed in through the constructor. All objects
are static and wired in `app_dat.cpp`.

```mermaid
classDiagram
    direction TB

    namespace fw_cpp {
        class AppOrchestrator {
            -shownCondition : Condition
            +init()
            +performServices()
            +getCountsPerDeg() uint16_t
            -updateLeds()
        }

        class ITempController {
            <<interface>>
            +init(revision)
            +filterHalf(event) uint16_t
            +evaluate(value)
            +getCondition() Condition
            +getCountsPerDeg() uint16_t
            +getBuffer()
            +getBufferSize() uint16_t
        }
        class TempController {
            -sensorCondition : Condition
            -sensorCountPerDegree : uint16_t
            -sampleBuffer : uint16_t[200]
            -medianFilter5(buffer) uint16_t
            -generateFilteredAverage(halfBuffer) uint32_t
            -changeCondition(condition, tempDeg)
        }

        class ILedController {
            <<interface>>
            +init()
            +allOff()
            +setActive(color)
        }
        class LedController

        class IEepromRead {
            <<interface>>
            +read(reg_address, data, size) uint8_t
        }
        class EepromController
    }

    namespace bsp_adapt {
        class IDma {
            <<interface>>
            +init(buffer, size) uint8_t
            +deinit() uint8_t
            +get_half_flag() uint8_t
            +clear_half_flag()
            +get_full_flag() uint8_t
            +clear_full_flag()
        }
        class Dma

        class IAdc {
            <<interface>>
            +init() uint8_t
            +deinit() uint8_t
        }
        class Adc

        class ITimer {
            <<interface>>
            +init() uint8_t
            +deinit() uint8_t
        }
        class ITickTimer {
            <<interface>>
            +get_1ms_flag() uint8_t
            +clear_1ms_flag()
        }
        class Timer {
            -id_ : uint8_t
        }
        class TickTimer

        class IGpioPin {
            <<interface>>
            +init()
            +deinit()
            +write(value)
        }
        class GpioPin {
            -pin_ : uint8_t
            -direction_ : uint8_t
        }

        class II2c {
            <<interface>>
            +read(slave_address, reg_address, data, size) uint8_t
        }
        class I2c
    }

    %% AppOrchestrator depends on interfaces only
    AppOrchestrator --> IEepromRead : eeprom
    AppOrchestrator --> ILedController : led
    AppOrchestrator --> ITempController : tempSensor
    AppOrchestrator --> IDma : dma
    AppOrchestrator --> IAdc : adc
    AppOrchestrator --> ITimer : adTriggerTimer
    AppOrchestrator --> ITickTimer : tickTimer

    %% Firmware classes
    ITempController <|.. TempController
    ILedController <|.. LedController
    IEepromRead <|.. EepromController
    LedController --> "3" IGpioPin : green, yellow, red
    EepromController --> II2c : i2cInterface

    %% BSP adapters
    IDma <|.. Dma
    IAdc <|.. Adc
    ITimer <|.. Timer
    ITimer <|-- ITickTimer
    ITickTimer <|.. TickTimer
    TickTimer *-- Timer : timer_ (TIM0)
    IGpioPin <|.. GpioPin
    II2c <|.. I2c
```

## Notes

- **Interfaces:** pure virtual, with protected non-virtual destructors and no
  copy. Objects are never deleted through an interface.
- **Adapters:** each `bsp_adapt` class forwards to the C BSP (`adc_init()`,
  `dma_get_half_flag()`, ...) through `extern "C"`. The interrupt handlers stay
  in the C BSP.
