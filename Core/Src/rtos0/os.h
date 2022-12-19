/*
 * os.h
 *
 *  Created on: Dec 16, 2022
 *      Author: calebkang
 */

#ifndef SRC_RTOS0_OS_H_
#define SRC_RTOS0_OS_H_

#include "main.h"

#define TASK_WAITING 0
#define TASK_READY   1
#define TASK_RUNNING 2

#define TASK_NAME_MAXLEN 16

struct task_block {
  char name[TASK_NAME_MAXLEN];
  int id;
  int state;
  void (*start)(void *arg);
  void *arg;
  uint8_t *sp;
  uint32_t wakeup_time;
  struct task_block *next;
};

#define MAX_TASKS 16
extern struct task_block TASKS[MAX_TASKS];
#define kernel TASKS[0]

#define TIMESLICE (20)
#define STACK_SIZE (256)
extern uint32_t stack_space;

#define SCB_ICSR (*((volatile uint32_t *)0xE000ED04))
#define schedule()  SCB_ICSR |= (1 << 28)

extern void tasklist_add(struct task_block **list, struct task_block *el);
extern int tasklist_del(struct task_block **list, struct task_block *delme);
extern void task_waiting(struct task_block *t);
extern void task_ready(struct task_block *t);

extern void tick_increase(void);
extern struct task_block *task_create(char *name, void (*start)(void *arg), void *arg);
extern void sleep_ms(int ms);

extern volatile unsigned int jiffies;
extern volatile unsigned int switcher;

extern int n_tasks;
extern int running_task_id;
extern struct task_block *t_cur;

extern struct task_block *tasklist_active;
extern struct task_block *tasklist_waiting;

#endif /* SRC_RTOS0_OS_H_ */
