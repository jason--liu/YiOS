#ifndef __ASSEMBLY__
#error "Only include this from assembly code"
#endif

#ifndef __ASM_ASSEMBLER_H
#define __ASM_ASSEMBLER_H

/*
 * Return the current thread_info.
 */
.macro get_thread_info, rd
    mrs     \rd, sp_el0
 .endm

.macro adr_l, dst, sym
     adrp	\dst, \sym
     add	\dst, \dst, : lo12 :\sym
 .endm

#endif
