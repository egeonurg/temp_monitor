# ADR 0003: Component structure with `_ifa`, `_inc` and `_cfg` files

- **Status:** Accepted
- **Date:** 2026-09-30

## Context

The C firmware and BSP are made of components (adc, dma, tim, led,
temp_sensor, app, ...) that depend on each other. Without a fixed structure,
dependencies end up spread across `#include` lines in every file, and it is
hard to see what a component offers, what it needs, and what can be tuned.

## Decision

Every C component is one folder with fixed file roles:

| File | Role | Visibility |
|---|---|---|
| `<name>_ifa.h` | provided interface: prototypes, status codes, public types | public, the only header others include |
| `<name>_inc.h` | required interfaces: includes the `_ifa.h` of each dependency and remaps it to names this component owns (`APP_DMA_INIT`, `LED_GPIO_WRITE`) | private |
| `<name>_cfg.h` | tunables: pins, limits, periods | private |
| `<name>.c` | implementation, uses only its own names | |
| `CMakeLists.txt` | one static library, dependencies linked `PRIVATE` | |

`_ifa.h` must compile on its own: included first, alone, in an empty file.

The details are in [component_structure.md](../component_structure.md).

## Alternatives considered

- **One header per module.** Fewer files, but the provided interface, private
  dependencies and configuration are mixed, and every user sees all of them.
- **Direct calls to other components everywhere.** Simpler to write, but
  replacing a dependency means editing every call site.

## Consequences

- What a component needs is listed in one place, its `_inc.h`. Replacing a
  provider, or cutting it out for a test, means editing only that file.
- Configuration never leaks into the public interface.
- More files per component, and the remap macros are one level of indirection
  to read through.
- The C++ version reaches the same goal with interfaces and constructor
  injection instead of header remapping ([ADR 0004](0004-bsp-adaptation-layer.md)).
