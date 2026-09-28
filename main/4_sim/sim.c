#include "sim_ifa.h"
#include "sim_inc.h"

#include "dma_ifa.h"
#include "tim_ifa.h"
#include "temp_sensor_ifa.h"

#include <windows.h>

/* Host-only: this component exists so the firmware can be exercised on a PC.
   It is not part of a target build.

   One base period of SIM_SAMPLE_PERIOD_US models the TIM1 trigger event: a
   conversion completes and the DMA writes the result, which the DMA driver
   turns into its own half and full transfer interrupts. Every
   SIM_TICK_PERIOD_US the TIM0 interrupt is raised as well.

   Sample values are temperatures scaled by the sensor resolution: with
   revision A one count is 1 degree, with revision B one count is 0.1 degree,
   so 100 degrees is 100 counts or 1000 counts respectively. */

/* Temperatures the run walks through, in whole degrees, with the condition
   each one must produce. Spec: G < 85, Y >= 85, R >= 105 or < 5, plus 2 C
   exit hysteresis. The noise swings the filtered average between the set point
   and one below it, so a set point ON a threshold proves the hysteresis: the
   average crosses the line back and forth, the condition must not.
   The list ends where it starts, so it loops cleanly. */
static const uint16_t sim_setpoint_deg[] =
{
     80u,   /* NORMAL                                           */
     85u,   /* WARNING  on the threshold: 84/85, no flicker     */
     83u,   /* NORMAL   leaves warning at 83                    */
    110u,   /* CRITICAL straight from normal, no yellow step    */
    105u,   /* CRITICAL on the threshold: 104/105, no flicker   */
    103u,   /* WARNING  leaves critical at 103                  */
    105u,   /* CRITICAL                                         */
     84u,   /* NORMAL   straight from critical                  */
      5u,   /* CRITICAL on the threshold: 4/5, held once in     */
      7u,   /* NORMAL   leaves low critical at 7                */
     90u,   /* WARNING                                          */
      4u,   /* CRITICAL straight from warning, no green step    */
     90u,   /* WARNING  straight from low critical              */
     80u    /* NORMAL                                           */
};

#define SIM_SETPOINT_COUNT (sizeof(sim_setpoint_deg) / sizeof(sim_setpoint_deg[0]))

static volatile uint8_t  sim_running      = 0u;
static volatile uint32_t sim_tick_count   = 0u;
static volatile uint32_t sim_sample_count = 0u;
static volatile uint32_t sim_spike_count  = 0u;

/* Counts per degree, taken from the sensor: 1 for revision A, 10 for B. */
static uint16_t sim_counts_per_degree = 1u;
static uint16_t sim_noise_counts      = 1u;
static uint16_t sim_spike_counts      = 1u;

/* Sleep() granularity is ~15 ms on Windows, far too coarse for a 100 us base
   period, so the schedule is kept on the performance counter instead. */
static LONGLONG sim_period_ticks = 0;

/* Deterministic pseudo random noise: the same run produces the same samples,
   so a filter's output stays reproducible between runs. */
static uint32_t sim_rand_state = 0x12345678u;

static uint16_t sim_rand(void)
{
    sim_rand_state = (sim_rand_state * 1103515245u) + 12345u;

    return (uint16_t)(sim_rand_state >> 16);
}

static void sim_wait_until(LONGLONG deadline)
{
    LARGE_INTEGER now;

    do
    {
        (void)QueryPerformanceCounter(&now);
    }
    while (now.QuadPart < deadline);
}

static uint32_t sim_setpoint_slot(uint32_t sample_index)
{
    uint32_t elapsed_ms = (sample_index * SIM_SAMPLE_PERIOD_US) / 1000u;

    return (elapsed_ms / SIM_SETPOINT_DURATION_MS) % (uint32_t)SIM_SETPOINT_COUNT;
}

static uint16_t sim_build_sample(uint32_t sample_index, uint32_t slot)
{
    int32_t  value = 0;
    uint16_t span  = (uint16_t)((2u * sim_noise_counts) + 1u);

    value = (int32_t)sim_setpoint_deg[slot] * (int32_t)sim_counts_per_degree;

    /* Noise, evenly spread around the set point. */
    value += (int32_t)(sim_rand() % span) - (int32_t)sim_noise_counts;

    /* Outlier, alternating high and low so a median filter has to reject both. */
    if ((sample_index % SIM_SPIKE_INTERVAL) == 0u)
    {
        if ((sim_spike_count % 2u) == 0u)
        {
            value += (int32_t)sim_spike_counts;
        }
        else
        {
            value -= (int32_t)sim_spike_counts;
        }

        sim_spike_count++;
    }

    /* The converter cannot report outside its own range. */
    if (value < 0)
    {
        value = 0;
    }
    else if (value > (int32_t)SIM_ADC_MAX_COUNT)
    {
        value = (int32_t)SIM_ADC_MAX_COUNT;
    }
    else
    {
        /* In range. */
    }

    return (uint16_t)value;
}

static DWORD WINAPI sim_thread(LPVOID argument)
{
    LARGE_INTEGER now;
    LONGLONG      deadline;
    uint32_t      us_since_tick = 0u;
    uint32_t      slot          = 0u;
    uint32_t      logged_slot   = (uint32_t)SIM_SETPOINT_COUNT;
    uint32_t      half_count    = 0u;
    uint32_t      full_count    = 0u;

    (void)argument;

    (void)QueryPerformanceCounter(&now);
    deadline = now.QuadPart + sim_period_ticks;

    while ((sim_running != 0u) && (sim_sample_count < SIM_RUN_SAMPLES))
    {
        sim_wait_until(deadline);
        deadline += sim_period_ticks;

        slot = sim_setpoint_slot(sim_sample_count);

        if (slot != logged_slot)
        {
            logged_slot = slot;

            SIM_LOG("%u ms: set point %u C -> %u counts\n",
                   (unsigned int)((sim_sample_count * SIM_SAMPLE_PERIOD_US) / 1000u),
                   (unsigned int)sim_setpoint_deg[slot],
                   (unsigned int)(sim_setpoint_deg[slot] * sim_counts_per_degree));
        }

        /* TIM1 trigger -> A/D conversion -> DMA writes the result. The DMA
           driver raises its own half and full transfer interrupts. */
        dma_mock_sample(sim_build_sample(sim_sample_count, slot));
        sim_sample_count++;

        /* TIM0 update interrupt. */
        us_since_tick += SIM_SAMPLE_PERIOD_US;
        if (us_since_tick >= SIM_TICK_PERIOD_US)
        {
            us_since_tick = 0u;
            sim_tick_count++;

            tim0_periodic_isr();
        }
    }

    sim_running = 0u;

    dma_get_transfer_counts(&half_count, &full_count);

    SIM_LOG("run finished: %u ms, %u tick(s), %u sample(s), "
                   "%u half + %u full DMA interrupt(s), %u spike(s)\n",
           (unsigned int)SIM_RUN_MS,
           (unsigned int)sim_tick_count,
           (unsigned int)sim_sample_count,
           (unsigned int)half_count,
           (unsigned int)full_count,
           (unsigned int)sim_spike_count);

    return 0;
}

uint8_t sim_start(void)
{
    HANDLE        thread;
    LARGE_INTEGER frequency;
    uint32_t      noise = 0u;

    /* Resolution comes from the sensor itself, so the samples follow whichever
       revision is fitted. */
    sim_counts_per_degree = temp_sensor_get_counts_per_deg();

    if (sim_counts_per_degree == 0u)
    {
        return SIM_ERR;
    }

    noise = ((uint32_t)SIM_NOISE_TENTHS_DEG * (uint32_t)sim_counts_per_degree) / 10u;
    if (noise == 0u)
    {
        /* One count is the finest step this resolution can express. */
        noise = 1u;
    }
    sim_noise_counts = (uint16_t)noise;

    sim_spike_counts = (uint16_t)(((uint32_t)SIM_SPIKE_TENTHS_DEG *
                                   (uint32_t)sim_counts_per_degree) / 10u);

    (void)QueryPerformanceFrequency(&frequency);
    sim_period_ticks = (frequency.QuadPart * (LONGLONG)SIM_SAMPLE_PERIOD_US) / 1000000;

    sim_tick_count   = 0u;
    sim_sample_count = 0u;
    sim_spike_count  = 0u;
    sim_running      = 1u;

    thread = CreateThread(NULL, 0, sim_thread, NULL, 0, NULL);

    if (thread == NULL)
    {
        sim_running = 0u;
        return SIM_ERR;
    }

    (void)CloseHandle(thread);

    SIM_LOG("start: %u us sample / %u us tick for %u ms, "
                   "%u count(s) per degree\n",
           (unsigned int)SIM_SAMPLE_PERIOD_US,
           (unsigned int)SIM_TICK_PERIOD_US,
           (unsigned int)SIM_RUN_MS,
           (unsigned int)sim_counts_per_degree);

    return SIM_OK;
}

uint8_t sim_is_running(void)
{
    return sim_running;
}
