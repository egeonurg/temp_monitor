#ifndef PLATFORM_IRQ_H
#define PLATFORM_IRQ_H

/* Global interrupt disable / enable (CMSIS __disable_irq / __enable_irq).
   On the host the simulated interrupts run on their own thread and cannot be
   masked, so these do nothing there. */
#if defined(__arm__)
#define PLATFORM_IRQ_DISABLE() __asm__ volatile ("cpsid i" ::: "memory")
#define PLATFORM_IRQ_ENABLE()  __asm__ volatile ("cpsie i" ::: "memory")
#else
#define PLATFORM_IRQ_DISABLE() ((void)0)
#define PLATFORM_IRQ_ENABLE()  ((void)0)
#endif

#endif /* PLATFORM_IRQ_H */
