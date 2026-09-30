# ADR 0002: Timer triggered A/D with DMA half/full transfer interrupts

- **Status:** Accepted
- **Date:** 2026-09-30

## Context

The temperature is sampled every 100 µs (10 kHz), and the sampling instant must
have minimal jitter. At that rate the CPU must not be involved in every
sample: 10 000 interrupts per second would cost CPU time, and each sample
would be taken or collected with a variable interrupt latency.

Temperature changes slowly. It does not move on a millisecond scale, so
reacting within tens of milliseconds is more than enough.

## Decision

- **TIM1 triggers the A/D in hardware** every 100 µs. The conversion start does
  not depend on software, so the sampling instant has no interrupt latency
  jitter.
- **DMA writes every result into a circular 200 sample buffer** owned by the
  temp sensor.
- **Only the DMA half and full transfer interrupts are used.** Each fires every
  10 ms (100 samples). The handler does nothing but publish an event counter,
  with a DSB first, so the superloop never sees the count before the memory
  accesses that came before it.
- **The superloop processes the stable half in place.** While the DMA fills one
  half, the other half is not written for 10 ms, so it is read directly from the
  DMA buffer. **No data is copied.**
- **Missed halves are detected**, not assumed away: the superloop compares the
  counters in `uint16_t`, which stays correct across wrap-around, and logs any
  gap.

## Alternatives considered

- **A/D end of conversion interrupt per sample.** 10 000 interrupts per second
  for 1 byte of work each. More CPU load, and a late interrupt delays the
  sample being collected.
- **Software triggered conversion from a timer interrupt.** The sampling
  instant then inherits the interrupt latency, which is exactly the jitter to
  avoid.
- **Copying each half into a separate buffer.** Needed only if processing could
  take longer than 10 ms. Here it costs RAM and time for nothing: filtering 100
  samples takes microseconds.
- **Single buffer, stop and restart the DMA.** Leaves gaps in the sampling
  while the buffer is processed.

## Consequences

- 2 interrupts per 10 ms instead of 100, and each is a few instructions.
- **Deadline:** a half must be processed within 10 ms, before the DMA wraps
  back to it. The superloop polls on a 1 ms tick, so the real budget is about
  9 ms. Processing takes microseconds, so the margin is large.
- The condition is updated every 10 ms, plus up to 1 ms of polling latency.
  Irrelevant for temperature.
- The buffer is shared with the DMA. Code must only read the half that was just
  completed, never the one being written.
- On a core with a data cache (e.g. Cortex-M7), the completed half must be
  invalidated in the cache before it is read. Not needed on cores without a
  data cache (Cortex-M0/M3/M4).
