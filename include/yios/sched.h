#ifndef _YIOS_SCHED_H_
#define _YIOS_SCHED_H_
#include <yios/page.h>
#include <yios/list.h>
#include <asm/current.h>

#define NR_TASK 64
#define THREAD_SIZE PAGE_SIZE
#define THREAD_START_SP (THREAD_SIZE - 8)

#ifndef __ASSEMBLY__
#include <asm/processor.h>
#include <yios/init_task.h>

struct task_struct;
struct run_queue;

struct sched_class {
	const struct sched_class *next;
	void (*task_fork)(struct task_struct *p);
	void (*enqueure_task)(struct run_queue *rq, struct task_struct *p);
	void (*dequeue_task)(struct run_queue *rq, struct task_struct *p);
	void (*task_tick)(struct run_queue *rq, struct task_struct *p);
	struct task_struct *(*pick_next_task)(struct run_queue *rq,
					      struct task_struct *prev);
};

struct run_queue {
	struct list_head rq_head;
	unsigned int nr_running;
	unsigned long nr_switches;
	struct task_struct *curr;
};

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
	struct list_head run_list;
	int need_resched;
	int preempt_count;
	struct task_struct *next_task;
	struct task_struct *prev_task;
};

union thread_union {
	struct task_struct task;
	unsigned long stack[THREAD_SIZE / sizeof(long)];
};

#define HZ 250
#define DEF_COUNTER (HZ / 10) /* 100ms time  */

#define SET_LINKS(p)                                                           \
	do {                                                                   \
		(p)->next_task = &init_task;                                   \
		(p)->prev_task = init_task.prev_task;                          \
		init_task.prev_task->next_task = (p);                          \
		init_task.prev_task = (p);                                     \
	} while (0)

#define for_each_task(p) for (p = &init_task; (p = p->next_task) != &init_task;)

#include <asm/current.h>
#define preempt_count() (current->preempt_count)
#define in_atomic_preempt_off() (preempt_count() != 1)

static inline void preempt_disable(void)
{
	current->preempt_count++;
}

static inline void preempt_enable(void)
{
	current->preempt_count--;
}

static inline void preempt_add(int val)
{
	current->preempt_count += val;
}

static inline void preempt_sub(int val)
{
	current->preempt_count -= val;
}

static inline void clear_task_resched(struct task_struct *p)
{
	p->need_resched = 0;
}

extern const struct sched_class simple_sched_class;

extern void ret_from_fork();
void wake_up_process(struct task_struct *p);
void tick_handle_periodic(void);
extern struct task_struct *cpu_switch_to(struct task_struct *prev,
					 struct task_struct *next);
void sched_init(void);

#endif /* __ASSEMBLY */

#endif
