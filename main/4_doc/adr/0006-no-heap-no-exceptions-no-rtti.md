# ADR 0006: No heap, no exceptions, no RTTI

- **Status:** Accepted
- **Date:** 2026-09-30

## Context

The target is a bare metal microcontroller. Memory use and timing must be
known at build time, and the image should not carry runtime support it does
not need.

## Decision

- **No heap.** Every object is statically allocated. In C++ they are all created
  in one file, `app_dat.cpp`, and wired through constructors.
- **No exceptions** (`-fno-exceptions`). Errors are reported through return
  codes, and programming errors through `PLATFORM_ASSERT`, which logs and traps.
- **No RTTI** (`-fno-rtti`). No `dynamic_cast` or `typeid`; the design does not
  need them.
- The C BSP and C firmware follow the same rules.

## Alternatives considered

- **Heap for object creation.** Flexible, but brings fragmentation and
  allocation failure at runtime, which a device that runs for years cannot
  recover from.
- **Exceptions for error handling.** Unwinding tables and runtime support in
  the image, and hard to bound the time a throw takes.

## Consequences

- RAM use is fixed and visible in the map file.
- No standard library parts that allocate: no `std::vector`, `std::string`,
  `std::function`.
- Interfaces have protected, non-virtual destructors and cannot be copied:
  objects are never deleted through an interface, so no virtual destructor is
  needed.
- Every error path is visible in the code: a returned code or an assert.
