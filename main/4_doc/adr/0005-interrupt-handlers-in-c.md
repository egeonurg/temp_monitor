# ADR 0005: Interrupt handlers stay in the C BSP

- **Status:** Accepted
- **Date:** 2026-09-30

## Context

The C++ firmware could have its own interrupt handlers: an `extern "C"` free
function has C linkage, so the vector table can reference it by name, and its
body can call into C++ objects.

That brings its own rules: the handler has no `this`, so it needs a static
instance to reach; static constructors must have run before interrupts are
enabled; member functions cannot have C linkage.

## Decision

Interrupt handlers (`dma_half_transfer_isr`, `dma_full_transfer_isr`,
`tim0_periodic_isr`) stay in the C BSP. They only publish an event: increment a
counter or set a flag. All processing happens in the superloop, which is where
the C++ code runs.

## Alternatives considered

- **Handlers in C++ with `extern "C"`.** Works, but the C++ layer would run in
  interrupt context, with the rules above, for no functional gain.
- **Handlers that do the processing.** Long handlers raise latency for every
  other interrupt, and the processing would share data with the superloop.

## Consequences

- The vector table and startup code stay vendor C, unchanged.
- No C++ object is touched in interrupt context, so static initialisation
  order and object lifetime never matter there.
- Both firmware versions use the same handlers.
- Data shared with a handler is a single counter or flag, written only by the
  handler. A single byte flag needs no critical section; the counters are
  published after a DSB ([ADR 0002](0002-dma-half-full-sampling.md)).
