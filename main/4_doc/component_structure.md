# Component structure

Every component is a folder of four files with fixed roles. The names say what
belongs where, so a declaration has exactly one correct home.

```
<name>/
    <name>_ifa.h      what the component offers      (public)
    <name>_inc.h      what the component needs       (private)
    <name>_cfg.h      what can be tuned              (private)
    <name>.c          implementation
    CMakeLists.txt    one static library
```

## `<name>_ifa.h` — provided interface

The only header other components may include. It holds:

- the public function prototypes, each marked `extern`
- the status codes and identifiers callers pass in (`I2C_OK`, `LED_GREEN`, `TIM0`)
- public types (`temp_sensor_event_t`)
- `#include <stdint.h>` and anything else its own declarations need

It must compile standalone: including it first, on its own, in an empty file
has to work. Nothing in it may depend on another component.

```c
#ifndef I2C_IFA_H
#define I2C_IFA_H

#include <stdint.h>

#define I2C_OK      0x00
#define I2C_ERR     0x01
#define I2C_TIMEOUT 0x02

extern uint8_t i2c_read(uint8_t slave_address, uint16_t reg_address, uint8_t *data, uint16_t size);

#endif /* I2C_IFA_H */
```

## `<name>_inc.h` — required interface

Private to the component, and the reason the layering holds. It includes the
`_ifa.h` of every component this one depends on, then **remaps each one to a
name this component owns**:

```c
/* led_inc.h */
#include "gpio_ifa.h"

#define LED_GPIO_INIT(pin, direction)   gpio_init(pin, direction)
#define LED_GPIO_WRITE(pin, value)      gpio_write(pin, value)
```

`led.c` calls `LED_GPIO_WRITE`, never `gpio_write`. Swapping the GPIO driver for
a different one, or for a test stub, is an edit to this header alone — the `.c`
does not change. This is compile-time dependency inversion, at no runtime cost.

The same header defines the component's log tag, used with the platform
logging macro. Asserts use `PLATFORM_ASSERT` directly:

```c
#define LED_LOG_TAG "LED"

PLATFORM_LOG_TAG(LED_LOG_TAG, "%s on\n", led_name[led_id]);   /* "#LED_LOG GREEN on" */
```

Naming by role rather than by provider is what makes the remap worth having:

```c
/* app_inc.h — app says what the timer is for, not which one it is */
#define APP_TIM_1MS_BASE     TIM0
#define APP_TIM_AD_TRIGGER   TIM1
```

## `<name>_cfg.h` — configuration

Values that are tuned rather than designed: periods, pin numbers, buffer sizes,
addresses, resolutions. Private to the component.

```c
/* tim_cfg.h */
#define TIM0_PERIOD_US 1000u   /* 1 ms system tick   */
#define TIM1_PERIOD_US 100u    /* 100 us A/D trigger */
```

Configuration belongs to whoever owns the concept, not to whoever reads it:
the EEPROM memory map lives in `app_cfg.h`, because it is the integration layer
that decides where each driver's data is stored — no driver knows another
driver's layout.

## `<name>.c` — implementation

Includes its own two headers first, in this order:

```c
#include "led_ifa.h"
#include "led_inc.h"
```

`_ifa.h` first so the compiler checks every definition against the published
prototype. Everything else the file needs arrives through `_inc.h`.

## Rules

1. Outside a component, include only its `_ifa.h`.
2. Inside a component, include `_ifa.h` then `_inc.h`.
3. Never call another component's function directly from a `.c` — go through a
   remap defined in `_inc.h`.
4. Every header compiles standalone and survives double inclusion (include
   guards are `<NAME>_<SUFFIX>_H`).
5. Dependencies point downward only: `0_fw` → `1_bsp` → `platform`. No sideways
   links between components in the same layer.

## Enforcement

The rules are not conventions to remember; the build applies them. Each
component is a static library whose own folder is a `PUBLIC` include directory,
while its dependencies are linked `PRIVATE`:

```cmake
add_library(led STATIC led.c)
target_include_directories(led PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(led PRIVATE gpio platform project_warnings)
```

`PRIVATE` means `gpio`'s headers reach `led` and stop there. A component that
tries to include a header it has not declared a dependency on fails to compile
with `No such file or directory`, not with a broken abstraction.

Note the one gap: CMake does not detect cycles between static libraries, so
"no sideways links" rests on review rather than on the build.

## Current components

| Layer | Component | `_ifa.h` | `_inc.h` | `_cfg.h` | Depends on |
|---|---|:--:|:--:|:--:|---|
| `platform` | platform | — | — | — | (headers only: log, assert, irq) |
| `1_bsp` | gpio | yes | yes | — | platform |
| `1_bsp` | i2c | yes | yes | yes | (none) |
| `1_bsp` | tim | yes | yes | yes | platform |
| `1_bsp` | adc | yes | yes | yes | platform |
| `1_bsp` | dma | yes | yes | yes | platform |
| `0_fw` | eeprom | yes | yes | yes | i2c |
| `0_fw` | temp_sensor | yes | yes | yes | platform |
| `0_fw` | led | yes | yes | yes | gpio, platform |
| `0_fw` | app | yes | yes | yes | eeprom, temp_sensor, led, tim, adc, dma, platform |
| `6_sim` | sim | yes | yes | yes | tim, dma, platform (host build only) |

`platform` is the exception to the four-file shape: it is a header-only
interface library (`platform_log.h`, `platform_assert.h`, `platform_irq.h`) sitting below the BSP,
so drivers can log and assert without depending upwards on the firmware layer.

`gpio` has no `_cfg.h` because it has nothing to tune yet; pin numbers belong to
the components that own the pins.

## Adding a component

1. Create the folder with the four files and the include guards.
2. Put the public prototypes in `_ifa.h`, with `extern` and the includes they need.
3. In `_inc.h`, include the `_ifa.h` of each dependency and remap them to
   `<NAME>_*` macros; add the log tag.
4. Put tunables in `_cfg.h`.
5. Write `CMakeLists.txt` as above and add the folder to the parent's
   `add_subdirectory` list.
6. Build with warnings as errors: `main\5_bat\build_c.bat`.
