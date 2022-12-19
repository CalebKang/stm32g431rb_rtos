/*
 * kernel.c
 *
 *  Created on: Dec 15, 2022
 *      Author: calebkang
 */
#include "os.h"


static task_block_t TASKS[MAX_TASKS];
static uint8_t tasks_count = 1;
static task_block_t *tasks_list_active[MAX_PRIO]={};
static task_block_t *tasks_list_waiting = NULL;
task_block_t *task_curr = &TASKS[0];

/**
* @brief
* @retval
*/
static void task_stack_init(task_block_t *t)
{
  struct stack_frame *tf;

  t->sp   -= sizeof(struct stack_frame);
  tf       = (struct stack_frame *)(t->sp);
  tf->r0   = (uint32_t) t->arg;
  tf->pc   = (uint32_t) t->start;
  tf->lr   = (uint32_t) task_terminated;
  tf->xpsr =  (1 << 24);
  t->sp   -= sizeof(struct extra_frame);
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  t->sp   -= sizeof(struct fpu_frame);
#endif
}

/**
* @brief
* @retval
*/
static void task_list_add(task_block_t **list, task_block_t *el)
{
  el->next = *list;
  *list    = el;
}

/**
* @brief
* @retval
*/
static void task_list_add_active(task_block_t *el)
{
  task_list_add(&tasks_list_active[el->priority], el);
}

/**
* @brief
* @retval
*/
static int task_list_del(task_block_t **list, task_block_t *delme)
{
  task_block_t *t = *list;
  task_block_t *p = NULL;

  while (t)
  {
    if (t == delme)
    {
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

/**
* @brief
* @retval
*/
static int task_list_del_active(task_block_t *el)
{
  return task_list_del(&tasks_list_active[el->priority], el);
}

/**
* @brief
* @retval
*/
task_block_t *task_list_next_ready(task_block_t *t)
{
  int i;

  for (i = MAX_PRIO - 1; i >= 0; i--)
  {
    if ((i == t->priority) && (t->next != NULL) && (t->next->state == TASK_READY))
      return t->next;
    if (tasks_list_active[i])
      return tasks_list_active[i];
  }
  return t;
}


/**
* @brief
* @retval
*/
void task_waiting(task_block_t *t)
{
  if (task_list_del_active(t) == 0)
  {
    task_list_add(&tasks_list_waiting, t);
    t->state = TASK_WAITING;
  }
}


/**
* @brief
* @retval
*/
void task_ready(task_block_t *t)
{
  if (task_list_del(&tasks_list_waiting, t) == 0)
  {
    task_list_add_active(t);
    t->state = TASK_READY;
  }
}


/**
* @brief
* @retval
*/
task_block_t *task_get_current(void)
{
  return task_curr;
}


/**
* @brief
* @retval
*/
void task_set_current(task_block_t *t)
{
  task_curr = t;
}


/**
* @brief
* @retval
*/
task_block_t *task_get(int id)
{
  return &TASKS[id];
}


/**
* @brief
* @retval
*/
task_block_t *task_create(char *name, void (*start)(void *arg), void *arg, int prio)
{
  task_block_t *t;
  int i;

  if (tasks_count >= MAX_TASKS)
    return NULL;

  t = &TASKS[tasks_count];
  t->id = tasks_count++;

  for (i = 0; i < TASK_NAME_MAXLEN; i++)
  {
    t->name[i] = name[i];

    if (name[i] == 0)
      break;
  }

  t->state = TASK_READY;
  t->start = start;
  t->arg = arg;
  t->wakeup_time = 0;
  t->priority = prio;
  t->sp = (uint8_t *)((uint32_t)(&stack_space) + tasks_count * STACK_SIZE);
  task_stack_init(t);
  task_list_add_active(t);

  return t;
}


/**
* @brief
* @retval
*/
void task_terminated(void)
{
  while(1){};
}

/**
* @brief
* @retval
*/
#define kernel TASKS[0]
extern volatile unsigned int jiffies;

void task_os_start(void)
{
  while(1)
  {
    task_block_t *t = tasks_list_waiting;

    while (t)
    {
      if (t->wakeup_time && (t->wakeup_time < jiffies))
      {
        t->wakeup_time = 0;
        task_ready(t);
        break;
      }
      t = t->next;
    }
    WFI();
  }
}


/**
* @brief
* @retval
*/
void task_init(void)
{
  stack_reset();

  kernel.name[0] = 0;
  kernel.id = 0;
  kernel.state = TASK_RUNNING;
  kernel.wakeup_time = 0;
  kernel.priority = 0;
  task_list_add_active(&kernel);
}











