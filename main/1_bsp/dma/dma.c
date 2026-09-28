#include "dma_ifa.h"
#include "dma_inc.h"

static uint16_t *dma_buffer    = NULL;
static uint16_t  dma_size      = 0u;
static uint16_t  dma_half_size = 0u;
static uint16_t  dma_index     = 0u;

/* Samples are copied out of the DMA buffer here, so the controller can keep
   filling the other half while they are processed. */
static uint16_t dma_work_buffer[DMA_WORK_BUFFER_SIZE];

static volatile uint32_t dma_half_count = 0u;
static volatile uint32_t dma_full_count = 0u;

static volatile uint32_t dma_half_event_number = 0u;
static volatile uint32_t dma_full_event_number = 0u;

uint8_t dma_init(uint16_t *buffer, uint16_t size)
{
    if ((buffer == NULL) || (size < 2u) || ((size % 2u) != 0u))
    {
        return DMA_ERR;
    }

    if ((size / 2u) > DMA_WORK_BUFFER_SIZE)
    {
        return DMA_ERR;
    }

    dma_buffer    = buffer;
    dma_size      = size;
    dma_half_size = (uint16_t)(size / 2u);
    dma_index     = 0u;

    dma_half_count = 0u;
    dma_full_count = 0u;

    /* Mock: on target this programs the peripheral and memory addresses, the
       transfer count and the request source, enables the half and full
       transfer interrupts, then enables the channel. */
    DMA_LOG("init: %u samples of %u byte(s), half %u, request source %u\n",
           (unsigned int)dma_size,
           (unsigned int)sizeof(uint16_t),
           (unsigned int)dma_half_size,
           (unsigned int)DMA_REQUEST_SOURCE);

    return DMA_OK;
}

uint8_t dma_deinit(void)
{
    /* Mock: on target this disables the channel and its interrupts, and clears
       the flags before the buffer reference is dropped. */
    dma_buffer    = NULL;
    dma_size      = 0u;
    dma_half_size = 0u;
    dma_index     = 0u;

    DMA_LOG("deinit: channel disabled, buffer released\n");

    return DMA_OK;
}

void dma_half_transfer_isr(void)
{
    if (dma_buffer == NULL)
    {
        return;
    }

    /* The first half is stable now; copy it out while the DMA fills the rest. */
    (void)memcpy(dma_work_buffer, &dma_buffer[0], (size_t)dma_half_size * sizeof(uint16_t));

    dma_half_event_number++;
    dma_half_count++;

    if ((dma_half_count % DMA_LOG_INTERVAL) == 0u)
    {
        DMA_LOG("half transfer %u: %u samples copied (first %u, last %u)\n",
               (unsigned int)dma_half_count,
               (unsigned int)dma_half_size,
               (unsigned int)dma_work_buffer[0],
               (unsigned int)dma_work_buffer[dma_half_size - 1u]);
    }
}

void dma_full_transfer_isr(void)
{
    if (dma_buffer == NULL)
    {
        return;
    }

    /* The second half is stable now; copy it out while the DMA wraps around. */
    (void)memcpy(dma_work_buffer, &dma_buffer[dma_half_size], (size_t)dma_half_size * sizeof(uint16_t));

    dma_full_event_number++;
    dma_full_count++;

    if ((dma_full_count % DMA_LOG_INTERVAL) == 0u)
    {
        DMA_LOG("full transfer %u: %u samples copied (first %u, last %u)\n",
               (unsigned int)dma_full_count,
               (unsigned int)dma_half_size,
               (unsigned int)dma_work_buffer[0],
               (unsigned int)dma_work_buffer[dma_half_size - 1u]);
    }
}

void dma_get_transfer_counts(uint32_t *half, uint32_t *full)
{
    if ((half == NULL) || (full == NULL))
    {
        return;
    }

    *half = dma_half_count;
    *full = dma_full_count;
}

void dma_mock_sample(uint16_t sample)
{
    if (dma_buffer == NULL)
    {
        return;
    }

    dma_buffer[dma_index] = sample;
    dma_index = (uint16_t)(dma_index + 1u);

    if (dma_index == dma_half_size)
    {
        dma_half_transfer_isr();
    }
    else if (dma_index >= dma_size)
    {
        dma_index = 0u;
        dma_full_transfer_isr();
    }
    else
    {
        /* Transfer still in progress. */
    }
}

uint16_t dma_get_half_event_number(void)
{
    return (uint16_t)dma_half_event_number;
}

uint16_t dma_get_full_event_number(void)
{
    return (uint16_t)dma_full_event_number;
}
