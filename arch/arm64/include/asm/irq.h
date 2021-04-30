#ifndef _YIOS_IRQ_H_
#define _YIOS_IRQ_H_

#include <asm/base.h>

#define IRQ_BASIC_PENDING	(PBASE+0x0000B200)
#define IRQ_PENDING_1		(PBASE+0x0000B204)
#define IRQ_PENDING_2		(PBASE+0x0000B208)
#define FIQ_CONTROL		(PBASE+0x0000B20C)
#define ENABLE_IRQS_1		(PBASE+0x0000B210)
#define ENABLE_IRQS_2		(PBASE+0x0000B214)
#define ENABLE_BASIC_IRQS	(PBASE+0x0000B218)
#define DISABLE_IRQS_1		(PBASE+0x0000B21C)
#define DISABLE_IRQS_2		(PBASE+0x0000B220)
#define DISABLE_BASIC_IRQS	(PBASE+0x0000B224)

#define SYSTEM_TIMER_IRQ_0	(1 << 0)
#define SYSTEM_TIMER_IRQ_1	(1 << 1)
#define SYSTEM_TIMER_IRQ_2	(1 << 2)
#define SYSTEM_TIMER_IRQ_3	(1 << 3)

#define ARM_TIMER_IRQ (1 << 0)

#define CORE0_INT_CTR  (PERIPHERAL_BASE+0x40)
#define CORE0_INT_SOURCE (PERIPHERAL_BASE+0x60)
#define LOCAL_TIMER_INT         (1 << 11)
#define CNTPNSIRQ_Int           (1 << 1)

#define GENERIC_TIMER_IRQ 30

static inline void arch_local_irq_enable(void)
{
	asm volatile("msr    daifclr, #2" : : : "memory");
}

static inline void arch_local_irq_disable(void)
{
	asm volatile("msr    daifset, #2" : : : "memory");
}

#define raw_local_irq_disable() arch_local_irq_disable()
#define raw_local_irq_enable() arch_local_irq_enable()

#endif
