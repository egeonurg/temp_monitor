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

#endif /* PLATFORM_LOG_H */
