#ifndef _YIOS_ASM_BARRIER_H_
#define _YIOS_ASM_BARRIER_H_

#ifndef __ASSEMBLY__
#define isb() asm volatile("isb" ::: "memory")
#define dmb(opt) asm volatile("dmb " #opt ::: "memory")
#define dsb(opt) asm volatile("dsb " #opt ::: "memory")
#endif

#endif
