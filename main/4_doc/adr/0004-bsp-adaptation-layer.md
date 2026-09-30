# ADR 0004: C++ adaptation layer over the C BSP

- **Status:** Accepted
- **Date:** 2026-09-30

## Context

The firmware exists in two versions, C and C++, on the same hardware. The BSP
is written in C, and it is also what the vendor code, the vector table and the
simulator talk to. The C++ firmware should be object oriented and testable,
without calling C functions and C headers directly from every class.

## Decision

`3_bsp_adapt` sits between the C BSP and the C++ firmware. For every driver:

- **An interface** (`IAdc`, `IDma`, `IGpioPin`, `II2c`, `ITimer`, `ITickTimer`):
  pure virtual methods mirroring the C functions one to one, and the driver's
  `OK`/`ERR` codes as `uint8_t` constants.
- **A concrete class** (`Adc`, `Dma`, ...) that forwards each call to the C
  function. Instance parameters the C API takes on every call (pin, direction,
  timer id) are bound once in the constructor.
- **C headers are included only in the adapter `.cpp` files**, wrapped in
  `extern "C"`. The C BSP library is linked `PRIVATE`, so nothing above sees it.
- `static_assert` checks that each interface code equals the C driver's code.
- Interrupt handlers and simulator hooks stay in the C BSP
  ([ADR 0005](0005-interrupt-handlers-in-c.md)).

The C++ firmware depends only on the interfaces, and receives the concrete
objects through its constructors.

## Alternatives considered

- **Rewrite the BSP in C++.** Two BSPs to maintain for the same hardware, and
  the C version would lose its shared base.
- **Call the C API directly from the C++ firmware.** No layer to maintain, but
  C headers and `extern "C"` spread through the firmware, and nothing can be
  replaced by a mock.
- **Static polymorphism (templates or CRTP) instead of virtual interfaces.** No
  vtables, but harder to read and every user becomes a template. The virtual
  calls here happen only in the superloop and at start-up, never per sample,
  so their cost is negligible.

## Consequences

- The C BSP is unchanged and shared by both firmware versions.
- Any driver can be replaced by a mock that implements its interface.
- One extra call level per driver access.
- The adapters mirror the C API completely, so some methods (`deinit`,
  `get_transfer_counts`) are not used by the firmware yet.
- Interfaces have protected non-virtual destructors and no copying: objects are
  statically allocated and never deleted through an interface
  ([ADR 0006](0006-no-heap-no-exceptions-no-rtti.md)).
