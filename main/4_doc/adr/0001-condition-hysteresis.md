# ADR 0001: 2 °C hysteresis on the temperature condition

- **Status:** Accepted
- **Date:** 2026-09-30

## Context

The specification defines three conditions and their limits:

| Condition | Limit |
|---|---|
| NORMAL | < 85 °C |
| WARNING | >= 85 °C |
| CRITICAL | >= 105 °C or < 5 °C |

It says nothing about what happens when the temperature sits on a limit.

Each half buffer (100 samples, 10 ms) is reduced to one value: median of 5
over each window, then the average of the 20 medians. The filter removes spikes
and most of the random noise, but a temperature that really is 85 °C still
lands on 84 or 85 after filtering. No filter can decide which side of the line
a value on the line belongs to.

The measurement itself is coarse:

| | Rev A | Rev B |
|---|---|---|
| Resolution | 1 count/°C | 10 counts/°C |
| Typical 12 bit SAR ADC total error (±2–4 LSB) | ±2–4 °C | ±0.2–0.4 °C |

VREF tolerance and the sensor's own accuracy (±0.5–2 °C) come on top.

The error has two parts, and they need different fixes:

- **Random noise** makes the reading jump between neighbouring values. Near a
  limit this is what makes the LED flicker.
- **Systematic error** (offset, gain, INL, VREF) shifts where the switch
  happens, but does not make it flicker.

## Decision

A condition is entered exactly at its limit, and left only once the
temperature is `TEMP_SENSOR_HYSTERESIS_DEG` = 2 °C back inside the band it came
from.

| Transition | Enter at | Leave at |
|---|---|---|
| NORMAL ↔ WARNING | >= 85 °C | <= 83 °C |
| WARNING ↔ CRITICAL (high) | >= 105 °C | <= 103 °C |
| NORMAL ↔ CRITICAL (low) | < 5 °C | >= 7 °C |

This is a derived requirement: a status LED that toggles every 10 ms near a
limit gives no information to whoever reads it.

## Alternatives considered

- **No hysteresis.** Rejected, see the measurement below. It also breaks the
  exit formula: with 0, `temp <= 85` (exit) and `temp >= 85` (entry) are both
  true at 85 °C, so the condition flips even on a steady reading. The value must
  be at least 1.
- **Time debounce** (switch only after N consecutive halves beyond the limit).
  It works, but it delays every transition, including real ones into CRITICAL,
  and a value sitting on the limit still toggles, only slower.
- **Hysteresis plus debounce.** Not needed at the current noise level. Worth
  adding if the hardware turns out noisier than the simulation.
- **Larger filter window.** Reduces noise further, but cannot resolve a value
  that sits on the limit, and costs latency and processing time within the
  10 ms half period.

## Measurement

The simulator holds set points on the limits (85, 105 and 5 °C) with noise and
spikes, so the filtered value crosses the line back and forth. Same run, only
the hysteresis changed:

| Hysteresis | LED switches in the 5.6 s run |
|---|---|
| 2 °C | 25, one per set point change |
| 0 °C | 116, toggling every 10 ms on the limits |

## Consequences

- The LEDs change only on real temperature changes.
- A condition is left 2 °C later than a plain threshold would leave it.
- On Rev B, 2 °C is 20 LSB, far above the noise that is left after filtering.
  On Rev A it is 2 LSB, about the same size as the ADC error, so the margin is
  thin. A noisier Rev A board may need a larger value or an added debounce.
- Hysteresis does not improve accuracy. With the systematic error, the device
  may enter WARNING at a real 83 or 87 °C. Fixing that needs calibration
  (offset and gain, or the factory values stored in the MCU), which is out of
  scope here.
- The limits and the hysteresis are configuration: `temp_sensor_cfg.h` in C,
  the private constants of `TempController` in C++.
