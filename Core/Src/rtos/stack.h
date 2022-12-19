/*
 * stack.h
 *
 *  Created on: Dec 15, 2022
 *      Author: calebkang
 */

#ifndef SRC_RTOS_STACK_H_
#define SRC_RTOS_STACK_H_

#define STACK_SIZE  (512)

struct stack_frame
{
  uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
};

struct extra_frame
{
  uint32_t r4, r5, r6, r7, r8, r9, r10, r11;
};

struct fpu_frame
{
  uint32_t s16, s17, s18, s19, s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s30, s31;
};

extern void stack_reset(void);

extern uint32_t _stored_data;
extern uint32_t _start_data;
extern uint32_t _end_data;
extern uint32_t _start_bss;
extern uint32_t _end_bss;
extern uint32_t _end_stack;
extern uint32_t _start_heap;
extern uint32_t stack_space;

#endif /* SRC_RTOS_STACK_H_ */
