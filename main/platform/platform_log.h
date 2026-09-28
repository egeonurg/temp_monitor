#ifndef PLATFORM_LOG_H
#define PLATFORM_LOG_H

#include <stdio.h>

/* Logging is a desktop simulation facility: on a host build it prints, on a
   target build it disappears. The arguments are still compiled in the unused
   case, inside sizeof, so parameters used only for logging do not become
   unused. Note: with -std=c11, MinGW.org's printf carries no format
   attribute, so format strings are not checked on this toolchain. */
#if defined(SIM_ENABLE)
#define PLATFORM_LOG(...) ((void)printf(__VA_ARGS__))
#else
#define PLATFORM_LOG(...) ((void)sizeof(printf(__VA_ARGS__)))
#endif

#endif /* PLATFORM_LOG_H */
