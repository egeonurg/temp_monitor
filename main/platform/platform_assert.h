#ifndef PLATFORM_ASSERT_H
#define PLATFORM_ASSERT_H

#include "platform_log.h"

/* Log tag prefixed to every assertion failure. */
#define PLATFORM_ASSERT_LOG "#ERR_LOG "

/* Push the message out before trapping; a target build has no console. */
#if defined(SIM_ENABLE)
#define PLATFORM_ASSERT_FLUSH() ((void)fflush(stdout))
#else
#define PLATFORM_ASSERT_FLUSH() ((void)0)
#endif

/* An assertion failure is a programming error, not a runtime condition: the
   message is reported and the CPU is trapped, because carrying on would run
   the system on state that is known to be invalid. On target the trap is where
   the debugger stops and the watchdog expires. */
#define PLATFORM_ASSERT(condition, message)                        \
    do                                                             \
    {                                                              \
        if (!(condition))                                          \
        {                                                          \
            PLATFORM_LOG(PLATFORM_ASSERT_LOG "%s:%d: %s\n",        \
                         __FILE__, __LINE__, (message));           \
            PLATFORM_ASSERT_FLUSH();                               \
            for (;;)                                               \
            {                                                      \
                /* trap */                                         \
            }                                                      \
        }                                                          \
    } while (0)

#endif /* PLATFORM_ASSERT_H */
