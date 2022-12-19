/*
 * stack.c
 *
 *  Created on: Dec 15, 2022
 *      Author: calebkang
 */
#include "os.h"



/**
  * @brief
  * @retval
  */
void stack_reset(void)
{
  register uint32_t *dst;

  dst = (uint32_t *)&stack_space;

  while (dst < &_end_stack)
  {
    *dst = 0xFFFFFFFF;
    dst++;
  }
}
