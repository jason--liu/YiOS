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

extern unsigned char _text_boot[], _etext_boot[];
extern unsigned char _text[], _etext[];
extern unsigned char _rodata[], _erodata[];
extern unsigned char _data[], _edata[];
extern unsigned char _bss[], _ebss[];

extern void traigger_alignment();
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

void kernel_thread(void)
{
    while(1){
        printk("%s: %s\n", __FUNCTION__, "12345");
    }
}
void kernel_main(void)
{
	uart_init();
	/* init_printk_done(); */
	print_segment();
	mem_init((unsigned long)_ebss, TOTAL_MEMORY);

	gic_init(0, GIC_V2_DISTRIBUTOR_BASE, GIC_V2_CPU_INTERFACE_BASE);
	timer_init();
	//	raw_local_irq_enable();

/* test fork */
    int pid;
    pid=do_fork(PF_KTHREAD, (unsigned)&kernel_thread,0);
    if (pid<0)
        printk("create kthread failed\n");
    struct task_struct *next=g_task[pid];

    switch_to(next);
	return;
}
