#ifndef PLATFORM_BARRIER_H
#define PLATFORM_BARRIER_H

/* Data synchronization barrier (CMSIS __DSB). Full memory barrier on the host. */
#if defined(__arm__)
#define PLATFORM_DSB() __asm__ volatile ("dsb 0xF" ::: "memory")
#else
#define PLATFORM_DSB() __sync_synchronize()
#endif

#endif /* PLATFORM_BARRIER_H */
