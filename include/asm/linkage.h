#ifndef __LINKAGE_H_INCLUDE__
#define __LINKAGE_H_INCLUDE__

#ifndef __ALIGN
#define __ALIGN .align 4, 0x90
#define __ALIGN_STR ".align 4,0x90"
#endif

#ifdef __ASSEMBLY__

#ifndef LINKER_SCRIPT
#define ALIGN __ALIGN
#define ALIGN_STR __ALIGN_STR
#endif

#ifndef ENTRY
#define ENTRY(name)                                                            \
	.globl name;                                                           \
	ALIGN;                                                                 \
    name:
#endif

#ifndef END
#define END(name) .size name, .- name
#endif

/* If symbol 'name' is treated as a subroutine (gets called, and returns)
 * then please use ENDPROC to mark 'name' as STT_FUNC for the benefit of
 * static analysis tools such as stack depth analyzer.
 */
#ifndef ENDPROC
#define ENDPROC(name)                                                          \
	.type name, @function;                                                 \
	END(name)
#endif

#endif /* end of __ASSEMBLY */

#endif /* end of __LINKAGE_H_INCLUDE__ */
