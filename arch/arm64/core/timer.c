#include <yios/types.h>
#include <yios/printk.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <yios/arm-gic.h>
#include <asm/timer.h>
#include <asm/arm_local_reg.h>

#define HZ 250
#define NSEC_PER_SEC 1000000000L

static unsigned int val = NSEC_PER_SEC / HZ;

static int generic_timer_init(void)
{
	asm volatile("mov x0,#1\n"
		     "msr cntp_ctl_el0,x0"
		     :
		     :
		     : "memory");
	return 0;
}

static int generic_timer_reset(u32 val)
{
	asm volatile("msr cntp_tval_el0, %x[timer_val]"
		     :
		     : [timer_val] "r"(val)
		     : "memory");
	return 0;
}

static void enable_timer_interrupt(void)
{
	writel(CNT_PNS_IRQ, TIMER_CNTRL0);
	return;
}
void timer_init(void)
{
	generic_timer_init();
	generic_timer_reset(val);

	gicv2_unmask_irq(GENERIC_TIMER_IRQ);
	enable_timer_interrupt();
}

void handle_timer_irq()
{
	generic_timer_reset(val);
	printk("Core0 timer interrupt\n");
    return;
}
