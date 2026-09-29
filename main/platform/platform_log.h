#ifndef PLATFORM_LOG_H
#define PLATFORM_LOG_H

#include <stdio.h>

/* Prints on the host build only. On target the arguments stay inside sizeof,
   so variables used only for logging don't trigger unused warnings. */
#if defined(SIM_ENABLE)
#define PLATFORM_LOG(...) ((void)printf(__VA_ARGS__))
#else
#define PLATFORM_LOG(...) ((void)sizeof(printf(__VA_ARGS__)))
#endif

/* PLATFORM_LOG_TAG(ADC_LOG_TAG, "x %u\n", v) prints "#ADC_LOG x 5" */
#define PLATFORM_LOG_TAG(tag, ...) PLATFORM_LOG("#" tag "_LOG " __VA_ARGS__)

#endif /* PLATFORM_LOG_H */
