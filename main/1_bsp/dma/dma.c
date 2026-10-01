#include "dma_ifa.h"
#include "dma_inc.h"

static uint16_t *dma_buffer    = NULL;
static uint16_t  dma_size      = 0u;
static uint16_t  dma_half_size = 0u;
static uint16_t  dma_index     = 0u;

static volatile uint8_t dma_half_flag = 0u;
static volatile uint8_t dma_full_flag = 0u;

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

        dma_half_flag = 0u;
        dma_full_flag = 0u;

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
        dma_half_flag = 1u;
    }
}

void dma_full_transfer_isr(void)
{
    if (dma_buffer != NULL)
    {
        dma_full_flag = 1u;
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

uint8_t dma_get_half_flag(void)
{
    return dma_half_flag;
}

void dma_clear_half_flag(void)
{
    DMA_IRQ_DISABLE();
    dma_half_flag = 0u;
    DMA_IRQ_ENABLE();
}

uint8_t dma_get_full_flag(void)
{
    return dma_full_flag;
}

void dma_clear_full_flag(void)
{
    DMA_IRQ_DISABLE();
    dma_full_flag = 0u;
    DMA_IRQ_ENABLE();
}
