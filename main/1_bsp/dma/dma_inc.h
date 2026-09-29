#ifndef DMA_INC_H
#define DMA_INC_H

#include <stdint.h>
#include <stddef.h>

#include "platform_log.h"
#include "platform_barrier.h"

#include "dma_ifa.h"
#include "dma_cfg.h"

#define DMA_DSB() PLATFORM_DSB()

#define DMA_LOG_TAG "DMA"

#endif /* DMA_INC_H */
