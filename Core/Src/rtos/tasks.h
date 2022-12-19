/*
 * kernel.h
 *
 *  Created on: Dec 15, 2022
 *      Author: calebkang
 */

#ifndef SRC_RTOS_TASKS_H_
#define SRC_RTOS_TASKS_H_

#define TASK_NAME_MAXLEN    (16)

enum
{
  TASK_WAITING,
  TASK_READY,
  TASK_RUNNING,
};

struct task_block
{
  char name[TASK_NAME_MAXLEN];
  int id;
  int state;
  void (*start)(void *arg);
  void *arg;
  uint8_t *sp;
  uint32_t wakeup_time;
  uint8_t priority;
  struct task_block *next;
};

typedef struct task_block task_block_t;

#define MAX_TASKS           (16)
#define MAX_PRIO            (4)

#define SCB_ICSR (*((volatile uint32_t *)0xE000ED04))
#define schedule()  SCB_ICSR |= (1 << 28)
#define WFI() __asm__ volatile ("wfi");

extern task_block_t *task_create(char *name, void (*start)(void *arg), void *arg, int prio);
extern void task_terminated(void);
extern void task_waiting(task_block_t *t);
extern void task_ready(task_block_t *t);
extern task_block_t *task_get_current(void);
extern void task_set_current(task_block_t *t);
extern task_block_t *task_get(int id);
extern task_block_t *task_list_next_ready(task_block_t *t);
extern void task_os_start(void);
extern void task_init(void);

#endif /* SRC_RTOS_TASKS_H_ */
