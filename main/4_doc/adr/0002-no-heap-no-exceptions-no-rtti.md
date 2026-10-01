# ADR 0002: No heap, no exceptions, no RTTI

- **Status:** Accepted
- **Date:** 2026-09-30

## Context

Bare metal target: memory use and timing must be known at build time.

## Decision

- **No heap.** All objects are static. In C++ they are created and wired in
  `app_dat.cpp`.
- **No exceptions** (`-fno-exceptions`). Errors are return codes, programming
  errors are `PLATFORM_ASSERT`.
- **No RTTI** (`-fno-rtti`). Nothing needs `dynamic_cast` or `typeid`.

## Alternatives considered

- **Heap.** Fragmentation and allocation failure at runtime.
- **Exceptions.** Unwind tables in the image and unbounded throw time.

## Consequences

- RAM use is fixed and visible in the map file.
- No allocating standard library types (`std::vector`, `std::string`, ...).
- Interfaces have protected, non-virtual destructors, since objects are never
  deleted through an interface.
