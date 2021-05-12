#ifndef _YIOS_SCHED_H_
#define _YIOS_SCHED_H_
#include <yios/page.h>

#define NR_TASK 64
#define THREAD_SIZE PAGE_SIZE
#define THREAD_START_SP (THREAD_SIZE - 8)

#ifndef __ASSEMBLY__
#include <asm/processor.h>
/* #include <yios/init_task.h> */

enum task_state {
	TASK_RUNNING = 0,
	TASK_INTERRUPTIBLE = 1,
	TASK_UNINTERRUPTIBLE = 2,
	TASK_ZOMBIE = 3,
	TASK_STOPPED = 4,
};

enum task_flags {
	PF_KTHREAD = 1 << 0,
};

struct task_struct {
	enum task_state state;
	enum task_flags flags;
	long count;
	int priority;
	int pid;
	struct cpu_context cpu_context;
};

union thread_union {
	struct task_struct task;
	unsigned long stack[THREAD_SIZE / sizeof(long)];
};

extern struct task_struct *g_task[];
int do_fork(unsigned long clone_flags, unsigned long fn, unsigned long arg);

extern void ret_from_fork();
void switch_to(struct task_struct *next);
extern struct task_struct *cpu_switch_to(struct task_struct *prev,
					 struct task_struct *next);
#endif /* __ASSEMBLY */

#endif
