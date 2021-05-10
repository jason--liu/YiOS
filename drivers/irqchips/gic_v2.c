#include <yios/arm-gic.h>
#include <yios/printk.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/timer.h>

struct gic_chip_data {
	u64 raw_dist_base;
	u64 raw_cpu_base;
	struct irq_domain *domain;
	struct irq_chip *chips;
	u32 gic_irqs;
};


static struct gic_chip_data gic_data[ARM_GIC_MAX_NR];

#define HW_IRQ_START 16

static u64 gic_get_dist_base(void)
{
	struct gic_chip_data *gic = &gic_data[0];
	return gic_dist_base(gic);
}

static u64 gic_get_cpu_base(void)
{
	struct gic_chip_data *gic = &gic_data[0];
	return gic_cpu_base(gic);
}

static void gic_set_irq(u32 irq, u32 offset)
{
	u32 mask = 1 << (irq % 32);
	writel(mask, gic_get_dist_base() + offset + (irq / 32) * 4);
}

void gicv2_mask_irq(u32 irq)
{
	gic_set_irq(irq, GIC_DIST_ENABLE_CLEAR);
}

void gicv2_unmask_irq(u32 irq)
{
	gic_set_irq(irq, GIC_DIST_ENABLE_SET);
}

void gicv2_eoi_irq(u32 irq)
{
	writel(irq, gic_get_cpu_base() + GIC_CPU_EOI);
}

static u32 gic_get_cpumask(struct gic_chip_data *gic)
{
	u64 base = gic_dist_base(gic);
	u32 mask, i;

	for (i = mask = 0; i < 32; i += 4) {
		mask = readl(base + GIC_DIST_TARGET + i);
		mask |= mask >> 16;
		mask |= mask >> 8;
		if (mask)
			break;
	}

	return mask;
}

static void gic_dist_init(struct gic_chip_data *gic)
{
	u64 base = gic_dist_base(gic);
	u32 cpumask;
	u32 gic_irqs = gic->gic_irqs;
	u32 i;

	writel(GICD_DISABLE, base + GIC_DIST_CTRL);

	cpumask = gic_get_cpumask(gic);
	cpumask |= cpumask << 8;
	cpumask |= cpumask << 16;

	for (i = 32; i < gic_irqs; i += 4)
		writel(cpumask, base + GIC_DIST_TARGET + i * 4 / 4);

	/* Set all global interrupts to be level triggered, active low */
	for (i = 32; i < gic_irqs; i += 16)
		writel(GICD_INT_ACTLOW_LVLTRIG, base + GIC_DIST_CONFIG + i / 4);

	/* Deactivate and disable all */
	for (i = 0; i < gic_irqs; i += 32) {
		writel(GICD_INT_EN_CLR_X32,
		       base + GIC_DIST_ACTIVE_CLEAR + i / 8);
		writel(GICD_INT_EN_CLR_X32,
		       base + GIC_DIST_ENABLE_CLEAR + i / 8);
	}

	/* enable (0~15) irq */
	writel(GICD_INT_EN_SET_SGI, base + GIC_DIST_ENABLE_SET);

	writel(GICD_ENABLE, base + GIC_DIST_CTRL);
}

static void gic_cpu_init(struct gic_chip_data *gic)
{
	int i;
	u64 base = gic_cpu_base(gic);
	u64 dist_base = gic_dist_base(gic);

	/* Set priority on PPI and SCI interrupts */
	for (i = 0; i < 32; i += 4)
		writel(0xa0a0a0a0, dist_base + GIC_DIST_PRI + i * 4 / 4);

	writel(GICC_INT_PRI_THRESHOLD, base + GIC_CPU_PRIMASK);
	writel(GICC_ENABLE, base + GIC_CPU_CTRL);
}

void gic_handle_irq(void)
{
	struct gic_chip_data *gic = &gic_data[0];
	u64 base = gic_cpu_base(gic);
	u32 irqstat, irqnr;

	do {
		irqstat = readl(base + GIC_CPU_INTACK);
		irqnr = irqstat & GICC_IAR_INT_ID_MASK;

        printk("irqnr=%d\n", irqnr);
		if (irqnr == GENERIC_TIMER_IRQ)
			handle_timer_irq();

		gicv2_eoi_irq(irqnr);

	} while (0);
}

int gic_init(int chip, u64 dist_base, u64 cpu_base)
{
	struct gic_chip_data *gic;
	int gic_irqs;

	gic = &gic_data[chip];

	gic->raw_cpu_base = cpu_base;
	gic->raw_dist_base = dist_base;

	gic_irqs = readl(gic_dist_base(gic) + GIC_DIST_CTR) & 0x1f;
	gic_irqs = (gic_irqs + 1) * 32;

	if (gic_irqs > 1020)
		gic_irqs = 1020;

	printk("%s: cpu_base:0x%0x, dist_base:0x%0x, gic_irqs:%d\n",
	       __FUNCTION__, cpu_base, dist_base, gic->gic_irqs);

    gic_dist_init(gic);
    gic_cpu_init(gic);

    return 0;
}
