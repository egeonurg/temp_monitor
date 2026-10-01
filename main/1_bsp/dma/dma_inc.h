#ifndef DMA_INC_H
#define DMA_INC_H

#include <stdint.h>
#include <stddef.h>

#include "platform_log.h"
#include "platform_irq.h"

#include "dma_ifa.h"
#include "dma_cfg.h"

#define DMA_IRQ_DISABLE() PLATFORM_IRQ_DISABLE()
#define DMA_IRQ_ENABLE()  PLATFORM_IRQ_ENABLE()

#define DMA_LOG_TAG "DMA"

#endif /* DMA_INC_H */
