#ifndef DMA_INC_H
#define DMA_INC_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "platform_log.h"

#include "dma_ifa.h"
#include "dma_cfg.h"

/* Logging for this component: the tag is prefixed to every message, and the
   whole thing disappears on a target build. */
#define DMA_LOG_TAG "#DMA_LOG "
#define DMA_LOG(...)  PLATFORM_LOG(DMA_LOG_TAG __VA_ARGS__)

#endif /* DMA_INC_H */
