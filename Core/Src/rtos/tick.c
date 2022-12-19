/*
 * tick.c
 *
 *  Created on: Dec 15, 2022
 *      Author: calebkang
 */
#include "os.h"


volatile unsigned int jiffies = 0;

/**
* @brief
* @retval
*/
void tick_increase(void)
{
  if ((++jiffies % TIMESLICE) == 0)
    schedule();
}


/**
* @brief
* @retval
*/
void sleep_ms(int ms)
{
  if (ms < 2)
    return;

  task_get_current()->wakeup_time = jiffies + ms;
  task_waiting(task_get_current());
  schedule();
}
