#ifndef _YIOS_PAGE_ALLOC_H_
#define _YIOS_PAGE_ALLOC_H_

void mem_init(unsigned long start, unsigned long end);
unsigned long get_free_page(void);
void free_page(unsigned long p);

#endif
