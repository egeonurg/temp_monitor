# Sequence: DMA half/full transfer → temperature → LED

One DMA cycle, from the hardware-triggered sampling to the LED update. The
full-transfer path is identical to the half-transfer one, with the other half
of the buffer.

```mermaid
sequenceDiagram
    autonumber

    box Hardware (no CPU involvement)
        participant TIM1 as TIM1<br/>(100 µs trigger)
        participant ADC
        participant DMA
    end
    box Interrupt context
        participant DMA_ISR as dma_half/full<br/>_transfer_isr
        participant TIM0_ISR as tim0_periodic_isr<br/>(1 ms)
    end
    box Superloop (main context)
        participant MAIN as main
        participant APP as app
        participant TS as temp_sensor
        participant LED as led / gpio
    end

    Note over TIM1,DMA: Sampling runs entirely in hardware:<br/>CPU load cannot delay a sample → low jitter

    loop every 100 µs (samples 0…99)
        TIM1->>ADC: update event starts conversion
        ADC->>DMA: conversion done (DMA request)
        DMA->>DMA: buffer[i] = sample
    end

    DMA-)DMA_ISR: half-transfer IRQ (buffer[0…99] stable)
    DMA_ISR->>DMA_ISR: dma_half_count++ (nothing else)

    par DMA keeps filling buffer[100…199]
        TIM1->>ADC: next samples…
    and Superloop processes buffer[0…99] (must finish within 10 ms)
        TIM0_ISR-)MAIN: tim0_1ms_flag = 1
        MAIN->>APP: app_get_1ms_flag() != 0
        MAIN->>APP: app_clear_1ms_flag()
        MAIN->>APP: app_handle_1ms_event()
        APP->>DMA: dma_get_half_event_number()
        DMA-->>APP: count (changed since last tick)
        APP->>TS: temp_sensor_process_half(HALF_TRANSFER_EVENT)
        TS->>TS: temp_sensor_filtered_average(&buffer[0])<br/>20 × median of 5 → average
        TS->>TS: temp_sensor_evaluate_condition(avg)<br/>temp_deg = avg / counts_per_deg

        alt condition changed (e.g. NORMAL → WARNING at ≥ 85 °C)
            TS->>TS: temp_sensor_change_condition()<br/>state = WARNING, log
            TS->>APP: temp_sensor_callback(WARNING)<br/>= app_on_temp_condition
            APP->>LED: led_set_active(APP_LED_WARNING = LED_YELLOW)
            LED->>LED: gpio_write × 3 (yellow on, others off)
        else no change (incl. hysteresis band)
            Note over TS: nothing to do
        end
    end

    DMA-)DMA_ISR: full-transfer IRQ (buffer[100…199] stable), DMA wraps to 0
    DMA_ISR->>DMA_ISR: dma_full_count++
    Note over MAIN,LED: Same path with FULL_TRANSFER_EVENT → &buffer[100]<br/>while DMA refills buffer[0…99]
```

## Notes

- **Low jitter:** TIM1 → ADC → DMA is a hardware chain. The sampling instant does
  not depend on what the CPU is doing.
- **Short ISRs:** the DMA interrupts only count events. All processing runs in the
  superloop.
- **Double buffer:** the CPU processes one half while the DMA fills the other. A
  half must be processed within one half period (100 samples × 100 µs = 10 ms).
  It is picked up on the next 1 ms tick, so it starts at most 1 ms late.
- **Decoupling:** temp_sensor and led never call each other. temp_sensor reports a
  condition through a callback, and app decides which LED shows it.
