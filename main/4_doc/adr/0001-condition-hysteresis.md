# ADR 0001: 2 °C hysteresis on the temperature condition

- **Status:** Accepted
- **Date:** 2026-09-30

## Context

| Condition | Limit |
|---|---|
| NORMAL | < 85 °C |
| WARNING | >= 85 °C |
| CRITICAL | >= 105 °C or < 5 °C |

The specification does not say what happens when the temperature sits on a
limit. Even after the median filter, a real 85 °C lands on 84 or 85, so a plain
threshold makes the LED flicker every 10 ms.

| | Rev A | Rev B |
|---|---|---|
| Resolution | 1 count/°C | 10 counts/°C |
| Typical ADC error (±2–4 LSB) | ±2–4 °C | ±0.2–0.4 °C |

## Decision

A condition is entered at its limit and left only 2 °C
(`TEMP_SENSOR_HYSTERESIS_DEG`) back inside the band.

| Transition | Enter at | Leave at |
|---|---|---|
| NORMAL ↔ WARNING | >= 85 °C | <= 83 °C |
| WARNING ↔ CRITICAL (high) | >= 105 °C | <= 103 °C |
| NORMAL ↔ CRITICAL (low) | < 5 °C | >= 7 °C |

## Alternatives considered

- **No hysteresis.** The LED flickers on the limits.
- **Time debounce.** Delays every transition, including real ones into
  CRITICAL, and a value on the limit still toggles, only slower.
- **Larger filter window.** Less noise, but still cannot decide a value that
  sits on the limit.

## Measurement

Same simulator run with set points on the limits, only the hysteresis changed:

| Hysteresis | LED switches in 5.6 s |
|---|---|
| 2 °C | 25, one per set point change |
| 0 °C | 116, toggling on the limits |

## Consequences

- The LEDs change only on real temperature changes.
- On Rev A, 2 °C is 2 LSB, close to the ADC error, so the margin is thin.
- Hysteresis does not fix accuracy. Offset and gain errors need calibration,
  which is out of scope.
