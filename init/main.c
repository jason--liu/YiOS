#include <yios/mm.h>
#include <yios/printk.h>
#include <asm/uart.h>

void kernel_main(void)
{
	uart_init();
	init_printk_done();
    printk("hello world\n");
	return;
}
