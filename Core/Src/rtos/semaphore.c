/*
 * semaphore.c
 *
 *  Created on: Dec 15, 2022
 *      Author: calebkang
 */
#include "os.h"

/**
* @brief
* @retval
*/
int sem_wait(semaphore *s)
{
  int i;

  if (s == NULL)
    return -1;

  if (sem_trywait(s) == 0)
    return 0;

  for (i = 0; i < MAX_LISTENERS; i++)
  {
    if (!s->listeners[i])
      s->listeners[i] = task_get_current()->id;
    if (s->listeners[i] == task_get_current()->id)
      break;
  }

  task_waiting(task_get_current());
  schedule();

  return sem_wait(s);
}


/**
* @brief
* @retval
*/
int sem_post(semaphore *s)
{
  int i;

  if (s == NULL)
    return -1;

  if (sem_dopost(s) > 0)
  {
    for (i = 0; i < MAX_LISTENERS; i++)
    {
      if (s->listeners[i])
      {
        task_ready(task_get(s->listeners[i]));
        s->listeners[i] = 0;
      }
    }

    schedule();
  }
  return 0;
}





