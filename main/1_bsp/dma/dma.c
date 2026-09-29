#include "dma_ifa.h"
#include "dma_inc.h"

static uint16_t *dma_buffer    = NULL;
static uint16_t  dma_size      = 0u;
static uint16_t  dma_half_size = 0u;
static uint16_t  dma_index     = 0u;

static volatile uint32_t dma_half_count = 0u;
static volatile uint32_t dma_full_count = 0u;

/* Mock */
uint8_t dma_init(uint16_t *buffer, uint16_t size)
{
    uint8_t ret = DMA_ERR;

    if ((buffer != NULL) && (size >= 2u) && ((size % 2u) == 0u))
    {
        dma_buffer    = buffer;
        dma_size      = size;
        dma_half_size = (uint16_t)(size / 2u);
        dma_index     = 0u;

        dma_half_count = 0u;
        dma_full_count = 0u;

        PLATFORM_LOG_TAG(DMA_LOG_TAG, "init: %u samples, half %u, request source %u\n",
                         (unsigned int)dma_size,
                         (unsigned int)dma_half_size,
                         (unsigned int)DMA_REQUEST_SOURCE);

        ret = DMA_OK;
    }

    return ret;
}

/* Mock */
uint8_t dma_deinit(void)
{
    dma_buffer    = NULL;
    dma_size      = 0u;
    dma_half_size = 0u;
    dma_index     = 0u;

    PLATFORM_LOG_TAG(DMA_LOG_TAG, "deinit\n");

    return DMA_OK;
}

void dma_half_transfer_isr(void)
{
    if (dma_buffer != NULL)
    {
        /* Complete all prior memory accesses before publishing the count. */
        DMA_DSB();
        dma_half_count++;
    }
}

void dma_full_transfer_isr(void)
{
    if (dma_buffer != NULL)
    {
        /* Complete all prior memory accesses before publishing the count. */
        DMA_DSB();
        dma_full_count++;
    }
}

void dma_get_transfer_counts(uint32_t *half, uint32_t *full)
{
    if ((half != NULL) && (full != NULL))
    {
        *half = dma_half_count;
        *full = dma_full_count;
    }
}

#if defined(SIM_ENABLE)
void dma_mock_sample(uint16_t sample)
{
    if (dma_buffer != NULL)
    {
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
    }
}
#endif

uint16_t dma_get_half_event_number(void)
{
    return (uint16_t)dma_half_count;
}

uint16_t dma_get_full_event_number(void)
{
    return (uint16_t)dma_full_count;
}
