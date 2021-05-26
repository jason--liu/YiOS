#ifndef _YIOS_PGTABLE_PROT_H_
#define _YIOS_PGTABLE_PROT_H_

#include <asm/pgtable_hwdef.h>
#include <asm/memory.h>

/*
 * Software defined PTE bits definition.
 */
#define PTE_VALID (1UL << 0)
#define PTE_WRITE (PTE_DBM) /* same as DBM (51)*/
#define PTE_DIRTY (1UL << 55)
#define PTE_SPECIAL (1UL << 56)
#define PTE_PROT_NONE (1 << 58) /* only when !PTE_VALID */

#define _PROT_DEFAULT (PTE_TYPE_PAGE | PTE_AF | PTE_SHARED)
#define PROT_DEFAULT (_PROT_DEFAULT)

#define PAGE_KERNEL_RO ((PROT_NORMAL & ~PTE_WRITE) | PTE_RDONLY)
#define PAGE_KERNNEL_ROX ((PROT_NORMAL & ~(PTE_WRITE | PTE_PXN)) | PTE_RDONLY)
#define PAGE_KERNEL_EXEC (PROT_NORMAL & ~PTE_PXN)

#define PROT_DEVICE_nGnRnE                                                     \
	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE |            \
	 PTE_ATTRINDX(MT_DEVICE_nGnRnE))
#define PROT_DEVICE_nGnRE                                                      \
	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE |            \
	 PTE_ATTRINDX(MT_DEVICE_nGnRE))
#define PROT_NORMAL_NC                                                         \
	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE |            \
	 PTE_ATTRINDX(MT_NORMAL_NC))
#define PROT_NORMAL_WT                                                         \
	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE |            \
	 PTE_ATTRINDX(MT_NORMAL_WT))
#define PROT_NORMAL                                                            \
	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE |            \
	 PTE_ATTRINDX(MT_NORMAL))

#define PAGE_KERNEL PROT_NORMAL
#endif
