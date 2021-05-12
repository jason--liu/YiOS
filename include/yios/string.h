#ifndef _STRING_H
#define _STRING_H
#include <yios/types.h>

#ifndef __ASSEMBLER__

size_t strlen(const char *s);

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *start, char c, int size);

#endif

#endif
