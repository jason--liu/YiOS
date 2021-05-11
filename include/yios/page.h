#ifndef _YIOS_PAGE_H_
#define _YIOS_PAGE_H_

#include <yios/types.h>

#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define SECTION_SIZE (1 << SECTION_SHIFT)

#define TOTAL_MEMORY (SIZE_1G)

#define NR_PAGES (TOTAL_MEMORY / PAGE_SIZE)

#define PAGE_ALIGN(x) ALIGN(x, PAGE_SIZE)
/* to align the pointer to the (next) page boundary */
#define PAGE_BALIGN(x) BALIGN(x, PAGE_SIZE)

#define IS_PAGE_ALIGN(x) (!((unsigned long)(x) & (PAGE_SIZE - 1)))
#define IS_BLOCK_ALIGN(x) (!((unsigned long)(x) & (0x1fffff)))

#endif
