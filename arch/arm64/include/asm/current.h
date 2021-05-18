#ifndef _YIOS_CURRENT_H_
#define _YIOS_CURRENT_H_

#ifndef __ASSEMBLY__
struct task_struct;

static inline struct task_struct *get_current(void)
{
	unsigned long sp_el0;
	asm("mrs %0, sp_el0" : "=r"(sp_el0));
	return (struct task_struct *)sp_el0;
}
#define current get_current()

#endif /* __ASSEMBLY__ */
#endif
