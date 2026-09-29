#ifndef DMA_INC_H
#define DMA_INC_H

#include <stdint.h>
#include <stddef.h>

#include "platform_log.h"
#include "platform_barrier.h"

#include "dma_ifa.h"
#include "dma_cfg.h"

#define DMA_DSB() PLATFORM_DSB()

#define DMA_LOG_TAG "#DMA_LOG "
#define DMA_LOG(...)  PLATFORM_LOG(DMA_LOG_TAG __VA_ARGS__)

#endif /* DMA_INC_H */
