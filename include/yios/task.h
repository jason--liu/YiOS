#ifndef _YIOS_SCHED_TASK_H_
#define _YIOS_SCHED_TASK_H_

#include <yios/sched.h>
extern union thread_union init_task_union;

extern struct task_struct *g_task[];
int do_fork(unsigned long clone_flags, unsigned long fn, unsigned long arg);

#endif
