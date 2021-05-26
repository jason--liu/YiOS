#include <yios/printk.h>
#include <yios/page.h>
#include <yios/config.h>
#include <yios/mm.h>

static unsigned short mem_map[NR_PAGES] = { 0 };

void mem_init(unsigned long start, unsigned long end)
{
	unsigned long nr_free_pages = 0;
	unsigned long free;

	start = PAGE_BALIGN(start);
	end &= PAGE_MASK;
	free = end - start;

	while (start < end) {
		nr_free_pages++;
		start += PAGE_SIZE;
	}

	printk("Memory: %uKB available, %u free pages\n", free / 1024,
	       nr_free_pages);
}

unsigned long get_free_page(void)
{
	int i;

	for (i = 0; i < NR_PAGES; i++) {
		if (mem_map[i] == 0) {
			mem_map[i] = 1;
			return LOW_MEMORY + i * PAGE_SIZE;
		}
	}

	return 0;
}

void free_page(unsigned long p)
{
	mem_map[(p - LOW_MEMORY) / PAGE_SIZE] = 0;
}
