#ifndef __LINKAGE_H_INCLUDE__
#define __LINKAGE_H_INCLUDE__

#ifdef __ASSEMBLY__
#ifndef __ALIGN
#define __ALIGN .align 2
#endif


#ifndef LINKER_SCRIPT
#define _ALIGN __ALIGN
#endif

#ifndef ENTRY
#define ENTRY(name)                                                            \
	.global name;                                                       \
	_ALIGN;                                                              \
    name:
#endif

#ifndef END
#define END(name) .size name, .- name
#endif

#ifndef ENDPROC
#define ENDPROC(name)                                                          \
	.type name, %function;                                              \
	END(name)
#endif

#endif /* __ASSEMBLY__ */

#endif /* __LINKAGE_H_INCLUDE__ */
