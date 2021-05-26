#include <asm/barrier.h>
#include <yios/string.h>
#include <yios/page_alloc.h>
#include <yios/page.h>
#include <asm/base.h>
#include <yios/types.h>
#include <yios/config.h>
#include <asm/pgtable_types.h>
#include <asm/pgtable_hwdef.h>
#include <asm/pgtable_prot.h>
#include <asm/pgtable.h>
#include <asm/sysreg.h>
#include <yios/printk.h>
#include <asm/mmu.h>

#define NO_BLOCK_MAPPINGS BIT(0)
#define NO_CONT_MAPPINGS BIT(1)

extern char idmap_pg_dir[];
extern char _text_boot, etext[];
extern char _text[], _etext[];

static u64 early_pgtable_alloc(void)
{
	u64 phys;

	phys = get_free_page();
	memset((void *)phys, 0, PAGE_SIZE);

	return phys;
}

static void pmd_set_section(pmd_t *pmdp, u64 phys, u64 prot)
{
	u64 sect_prot = PMD_TYPE_SECT | mk_sect_prot(prot);

	pmd_t new_pmd = pfn_pmd(phys >> PMD_SHIFT, sect_prot);

	set_pmd(pmdp, new_pmd);
}

static void alloc_init_pte(pmd_t *pmdp, u64 addr, u64 end, u64 phys, u64 prot,
			   u64 (*alloc_pgtable)(void), u64 flags)
{
	pmd_t pmd = *pmdp;
	pte_t *ptep;

	if (pmd_none(pmd)) {
		u64 pte_phys;

		pte_phys = alloc_pgtable();
		set_pmd(pmdp, __pmd(pte_phys | PMD_TYPE_TABLE));
		pmd = *pmdp;
	}

	ptep = pte_offset_phys(pmdp, addr);
	do {
		set_pte(ptep, pfn_pte(phys >> PAGE_SHIFT, prot));
		phys += PAGE_SIZE;
		ptep++;
		addr += PAGE_SIZE;
	} while (addr != end);
}

static void alloc_init_pmd(pud_t *pudp, u64 addr, u64 end, u64 phys, u64 prot,
			   u64 (*alloc_pgtale)(void), u64 flags)
{
	pud_t pud = *pudp;
	pmd_t *pmdp;
	u64 next;

	if (pud_none(pud)) {
		u64 pmd_phys;

		pmd_phys = alloc_pgtale();

		set_pud(pudp, __pud(pmd_phys | PUD_TYPE_TABLE));
		pud = *pudp;
	}

	pmdp = pmd_offset_phys(pudp, addr);
	do {
		next = pmd_addr_end(addr, end);

		if (((addr | next | phys) & ~SECTION_MASK) == 0 &&
		    (flags & NO_BLOCK_MAPPINGS) == 0)
			pmd_set_section(pmdp, phys, prot);
		else
			alloc_init_pte(pmdp, addr, next, phys, prot,
				       alloc_pgtale, flags);
		phys += next - addr;
		pmdp++;
		addr = next;
	} while (addr != end);
}

static void alloc_init_pud(pgd_t *pgdp, u64 addr, u64 end, u64 phys, u64 prot,
			   u64 (*alloc_pgtable)(void), u64 flags)
{
	pgd_t pgd = *pgdp;
	pud_t *pudp;
	u64 next;

	if (pgd_none(pgd)) {
		u64 pud_phys;

		pud_phys = alloc_pgtable();

		set_pgd(pgdp, __pgd(pud_phys | PUD_TYPE_TABLE));
		pgd = *pgdp;
	}

	pudp = pud_offset_phys(pgdp, addr);
	do {
		next = pud_addr_end(addr, end);
		alloc_init_pmd(pudp, addr, next, phys, prot, alloc_pgtable,
			       flags);
		phys += next - addr;
		pudp++;
		addr = next;
	} while (addr != end);
}
static void __create_pgd_mapping(pgd_t *pgdir, u64 phys, u64 virt, u64 size,
				 u64 prot, u64 (*alloc_pgtable)(void),
				 u64 flags)
{
	pgd_t *pgdp = pgd_offset_raw(pgdir, virt);
	u64 addr, end, next;

	phys &= PAGE_MASK;
	addr = virt & PAGE_MASK;
	end = PAGE_BALIGN(virt + size);

	do {
		next = pgd_addr_end(addr, end);
		alloc_init_pud(pgdp, addr, next, phys, prot, alloc_pgtable,
			       flags);
		phys += next - addr;
		pgdp++;
		addr = next;
	} while (addr != end);
}

static void create_identical_mapping(void)
{
	u64 start, end;

	/* map text section */
	start = (u64)_text_boot;
	end = (u64)_etext;
	__create_pgd_mapping((pgd_t *)idmap_pg_dir, start, start, end - start,
			     PAGE_KERNNEL_ROX, early_pgtable_alloc, 0);

	/* map memory */
	start = PAGE_BALIGN((u64)_etext);
	end = TOTAL_MEMORY;
	__create_pgd_mapping((pgd_t *)idmap_pg_dir, start, start, end - start,
			     PAGE_KERNEL, early_pgtable_alloc, 0);
}

static void create_mmio_mapping(void)
{
	__create_pgd_mapping((pgd_t *)idmap_pg_dir, PBASE, PBASE, DEVICE_SIZE,
			     PROT_DEVICE_nGnRnE, early_pgtable_alloc, 0);
}

static void cpu_init(void)
{
	u64 mair = 0;
	u64 tcr = 0;
	u64 tmp;
	u64 parang;

	asm("tlbi vmalle1");
	dsb(nsh);

	write_sysreg(3UL << 20, cpacr_el1);
	write_sysreg(1 << 12, mdscr_el1);

	mair = MAIR(0x00UL, MT_DEVICE_nGnRnE) | MAIR(0x04UL, MT_DEVICE_nGnRE) |
	       MAIR(0x0cUL, MT_DEVICE_GRE) | MAIR(0x44UL, MT_NORMAL_NC) |
	       MAIR(0xffUL, MT_NORMAL) | MAIR(0xbbUL, MT_NORMAL_WT);
	write_sysreg(mair, mair_el1);

	tcr = TCR_TxSZ(VA_BITS) | TCR_TG_FLAGS;

	tmp = read_sysreg(ID_AA64MMFR0_EL1);
	parang = tmp & 0xf;
	if (parang > ID_AA64MMFR0_PARANGE_48)
		parang = ID_AA64MMFR0_PARANGE_48;

	tcr |= parang << TCR_IPS_SHIFT;
	write_sysreg(tcr, tcr_el1);
}

static int enable_mmu(void)
{
	u64 tmp;
	int tgran4;

	tmp = read_sysreg(ID_AA64MMFR0_EL1);
	tgran4 = (tmp >> ID_AA64MMFR0_TGRAN4_SHIFT) & 0xf;
	if (tgran4 != ID_AA64MMFR0_TGRAN4_SUPPORTED)
		return -1;

	write_sysreg(idmap_pg_dir, ttbr0_el1);
	isb();

	write_sysreg(SCTLR_ELx_M, sctlr_el1);
	isb();
	asm("ic iallu");
	dsb(nsh);
	isb();

	return 0;
}

void paging_init(void)
{
	memset(idmap_pg_dir, 0, PAGE_SIZE);
	create_identical_mapping();
	create_mmio_mapping();
	cpu_init();
	enable_mmu();
	printk("enable mmu done\n");
}
