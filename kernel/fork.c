#include <yios/sched.h>
#include <yios/page_alloc.h>
#include <asm/ptrace.h>

extern void memset(unsigned long start, int i, unsigned long size);
extern union thread_union init_task_union;

struct task_struct *current = &init_task_union.task;

/* global task array to store all task */
struct task_struct *g_task[NR_TASK] = {
	&init_task_union.task,
};

static int find_empty_task(void)
{
	int i;

	for (i = 0; i < NR_TASK; i++) {
		if (!g_task[i])
			return i;
	}

	return -1;
}

static struct pt_regs *task_pt_regs(struct task_struct *task)
{
	unsigned long p;
	p = (unsigned long)task + THREAD_SIZE - sizeof(struct pt_regs);

	return (struct pt_regs *)p;
}

static int copy_thread(unsigned long clone_flags, struct task_struct *p,
		       unsigned long fn, unsigned long arg)
{
	struct pt_regs *childregs;
	childregs = task_pt_regs(p);
	memset(childregs, 0, sizeof(struct pt_regs));
	memset(&p->cpu_context, 0, sizeof(struct cpu_context));

	if (clone_flags & PF_KTHREAD) {
		childregs->pstate = PSR_MODE_EL1h;
		p->cpu_context.x19 = fn;
		p->cpu_context.x20 = arg;
	}

	p->cpu_context.pc = (unsigned long)ret_from_fork;
	p->cpu_context.sp = (unsigned long)childregs;

    return 0;
}

int do_fork(unsigned long clone_flags, unsigned long fn, unsigned long arg)
{
	struct task_struct *p;
	int pid;

	p = (struct task_struct *)get_free_page();
	if (!p)
		goto error;

	pid = find_empty_task();
	if (pid < 0)
		goto error;

	if (copy_thread(clone_flags, p, fn, arg))
		goto error;

	p->state = TASK_RUNNING;
	p->pid = pid;
	g_task[pid] = p;

	return pid;

error:
	return -1;
}

void switch_to(struct task_struct *next)
{
	struct task_struct *prev = current;

	if (current == next)
		return;

	current = next;
	cpu_switch_to(prev, next);
}
