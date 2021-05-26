#include <yios/mm.h>
#include <yios/printk.h>
#include <yios/arm-gic.h>
#include <asm/uart.h>
#include <asm/base.h>
#include <asm/irq.h>
#include <asm/timer.h>
#include <yios/page_alloc.h>
#include <yios/page.h>
#include <yios/sched.h>
#include <yios/task.h>
#include <yios/config.h>
#include <asm/mmu.h>

extern unsigned char _text_boot[], _etext_boot[];
extern unsigned char _text[], _etext[];
extern unsigned char _rodata[], _erodata[];
extern unsigned char _data[], _edata[];
extern unsigned char _bss[], _ebss[];

extern void traigger_alignment();
/* struct task_struct; */
static void print_segment(void)
{
	printk("YiOS image layout:\n");
	printk("  .text.boot: 0x%08lx - 0x%08lx (%6ld B)\n",
	       (unsigned long)_text_boot, (unsigned long)_etext_boot,
	       (unsigned long)(_etext_boot - _text_boot));
	printk("       .text: 0x%08lx - 0x%08lx (%6ld B)\n",
	       (unsigned long)_text, (unsigned long)_etext,
	       (unsigned long)(_etext - _text));
	printk("     .rodata: 0x%08lx - 0x%08lx (%6ld B)\n",
	       (unsigned long)_rodata, (unsigned long)_erodata,
	       (unsigned long)(_erodata - _rodata));
	printk("       .data: 0x%08lx - 0x%08lx (%6ld B)\n",
	       (unsigned long)_data, (unsigned long)_edata,
	       (unsigned long)(_edata - _data));
	printk("        .bss: 0x%08lx - 0x%08lx (%6ld B)\n",
	       (unsigned long)_bss, (unsigned long)_ebss,
	       (unsigned long)(_ebss - _bss));
}

static void delay(n)
{
	while (n--)
		;
}

void kernel_thread(void)
{
	while (1) {
		delay(10000000);
		printk("%s: %s\n", __FUNCTION__, "12345");
	}
}

static unsigned long timer_freq(void)
{
	unsigned long freq;

	asm("mrs %0, cntfrq_el0" : "=r"(freq));
	return freq;
}
void kernel_thread1(u64 args)
{
	u64 count = 0;
	while (1) {
		delay(80000);
		printk("%s: %s count=%ld\n", __func__, (char *)args, count++);
	}
}

void kernel_thread2(u64 args)
{
	u64 count = 0;
	while (1) {
		delay(50000);
		printk("%s: %s count=%d\n", __func__, (char *)args, count++);
	}
}

static int test_access_map_address(void)
{
	unsigned long address = TOTAL_MEMORY - 4096;

	*(unsigned long *)address = 0x55;

	printk("%s access 0x%x done\n", __func__, address);

	return 0;
}

/*
 * 访问一个没有建立映射的地址
 * 应该会触发一级页表访问错误。
 *
 * Translation fault, level 1
 *
 * 见armv8.6手册第2995页
 */
static int test_access_unmap_address(void)
{
	unsigned long address = TOTAL_MEMORY + 4096;

	*(unsigned long *)address = 0x55;

	printk("%s access 0x%x done\n", __func__, address);

	return 0;
}

static void test_mmu(void)
{
	test_access_map_address();
	test_access_unmap_address();
}

void kernel_main(void)
{
	uart_init();
	/* init_printk_done(); */
	print_segment();
	mem_init((unsigned long)_ebss, TOTAL_MEMORY);
	sched_init();

	printk("freq: 0x%lx\n", timer_freq());

	gic_init(0, GIC_V2_DISTRIBUTOR_BASE, GIC_V2_CPU_INTERFACE_BASE);
	timer_init();
	raw_local_irq_enable();

#if 0
	/* test fork */
	int pid;
	pid = do_fork(PF_KTHREAD, (unsigned long)&kernel_thread1,
		      (unsigned long)"12345");
	if (pid < 0)
		printk("create kthread1 failed\n");

	pid = do_fork(PF_KTHREAD, (unsigned long)&kernel_thread2,
		      (unsigned long)"abcde");
	if (pid < 0)
		printk("create kthread2 failed\n");
#endif
	paging_init();
    test_mmu();

	while (1)
		;
}
