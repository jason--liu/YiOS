#ifndef _YIOS_TRAPS_H_
#define _YIOS_TRAPS_H_

void do_sync(struct pt_regs *regs);
void do_irq(struct pt_regs *regs);

#endif
