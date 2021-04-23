#include <yios/mm.h>
#include <yios/printk.h>
#include <asm/uart.h>

extern unsigned char _text_boot[], _etext_boot[];
extern unsigned char _text[], _etext[];
extern unsigned char _rodata[], _erodata[];
extern unsigned char _data[], _edata[];
extern unsigned char _bss[], _ebss[];

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

void kernel_main(void)
{
	uart_init();
	init_printk_done();
	print_segment();
	return;
}
