#ifndef _YIOS_INIT_TASK_H_
#define _YIOS_INIT_TASK_H_

extern unsigned char init_stack[];
#define __init_task_data __attribute__((__section__(".data.init_task")))
extern struct task_struct init_task;

/* #define INIT_TASK(task)     \ */
/*  {                          \ */
/*      .state = 0,            \ */
/*      .priority = 1,         \ */
/*      .flags = PF_KTHREAD,   \ */
/*      .pid = 0,              \ */
/*  } */

#endif
