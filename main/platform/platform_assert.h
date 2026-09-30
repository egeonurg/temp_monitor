#ifndef PLATFORM_ASSERT_H
#define PLATFORM_ASSERT_H

#include "platform_log.h"

#define PLATFORM_ASSERT_LOG "#ERR_LOG "

#if defined(SIM_ENABLE)
#define PLATFORM_ASSERT_FLUSH() ((void)fflush(stdout))
#else
#define PLATFORM_ASSERT_FLUSH() ((void)0)
#endif

#ifdef __cplusplus
extern "C"
#endif
/* Provided by the application: drives the outputs to a safe state (LEDs off). */
void platform_safe_state(void);

/* Logs, goes to the safe state and traps. On target the watchdog resets the
   device. */
#define PLATFORM_ASSERT(condition, message)                        \
    do                                                             \
    {                                                              \
        if (!(condition))                                          \
        {                                                          \
            PLATFORM_LOG(PLATFORM_ASSERT_LOG "%s:%d: %s\n",        \
                         __FILE__, __LINE__, (message));           \
            platform_safe_state();                                 \
            PLATFORM_ASSERT_FLUSH();                               \
            for (;;)                                               \
            {                                                      \
            }                                                      \
        }                                                          \
    } while (0)

#endif /* PLATFORM_ASSERT_H */
