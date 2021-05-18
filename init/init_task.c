#include <yios/init_task.h>
#include <yios/sched.h>
#include <yios/task.h>

struct task_struct init_task = {
	.state = 0,
	.priority = 1,
	.flags = PF_KTHREAD,
	.pid = 0,
	.count = DEF_COUNTER,
	.need_resched = 0,
	.preempt_count = 0,
	.next_task = &init_task,
	.prev_task = &init_task,
	/* .stack = init_stack, */
};

/* union thread_union init_task_union __init_task_data = { .task = init_task }; */

/* #define INIT_TASK(task)     \ */
/*  {                          \ */
/*      .state = 0,            \ */
/*      .priority = 1,         \ */
/*      .flags = PF_KTHREAD,   \ */
/*      .pid = 0,              \ */
/*  } */
