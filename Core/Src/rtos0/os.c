/*
 * os.c
 *
 *  Created on: Dec 16, 2022
 *      Author: calebkang
 */
#include "os.h"

struct task_block TASKS[MAX_TASKS];

int n_tasks = 1;
int running_task_id = 0;

struct task_block *t_cur = &TASKS[0];
struct task_block *tasklist_active = NULL;
struct task_block *tasklist_waiting = NULL;

void tasklist_add(struct task_block **list, struct task_block *el)
{
    el->next = *list;
    *list = el;
}

int tasklist_del(struct task_block **list, struct task_block *delme)
{
    struct task_block *t = *list;
    struct task_block *p = NULL;
    while (t) {
        if (t == delme) {
            if (p == NULL)
                *list = t->next;
            else
                p->next = t->next;
            return 0;
        }
        p = t;
        t = t->next;
    }
    return -1;
}

void task_waiting(struct task_block *t)
{
    if (tasklist_del(&tasklist_active, t) == 0) {
        tasklist_add(&tasklist_waiting, t);
        t->state = TASK_WAITING;
    }
}

void task_ready(struct task_block *t)
{
    if (tasklist_del(&tasklist_waiting, t) == 0) {
        tasklist_add(&tasklist_active, t);
        t->state = TASK_READY;
    }
}

volatile unsigned int jiffies = 0;
volatile unsigned int switcher = 0;

void tick_increase(void)
{
  if(jiffies > 1000)
  {
    jiffies = 0;
    switcher = 1;
  }
}

struct stack_frame {
    uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
};

struct extra_frame {
    uint32_t r4, r5, r6, r7, r8, r9, r10, r11;
};

void task_terminated(void)
{
    while(1){};
}


static void task_stack_init(struct task_block *t)
{
    struct stack_frame *tf;
    t->sp -= sizeof(struct stack_frame);
    tf = (struct stack_frame *)(t->sp);
    tf->r0 = (uint32_t) t->arg;
    tf->pc = (uint32_t) t->start;
    tf->lr = (uint32_t) task_terminated;
    tf->xpsr =  (1 << 24);
    t->sp -= sizeof(struct extra_frame);
}


struct task_block *task_create(char *name, void (*start)(void *arg), void *arg)
{
  struct task_block *t;
  int i;

  if (n_tasks >= MAX_TASKS)
      return NULL;
  t = &TASKS[n_tasks];
  t->id = n_tasks++;
  for (i = 0; i < TASK_NAME_MAXLEN; i++) {
      t->name[i] = name[i];
      if (name[i] == 0)
          break;
  }
  t->state = TASK_READY;
  t->start = start;
  t->arg = arg;
  t->wakeup_time = 0;
  t->sp = (uint8_t *)((&stack_space) + n_tasks * STACK_SIZE);
  task_stack_init(t);
  tasklist_add(&tasklist_active, t);
  return t;
}

void sleep_ms(int ms)
{
    if (ms < 2)
        return;
    t_cur->wakeup_time = jiffies + ms;
    task_waiting(t_cur);
    schedule();
}
