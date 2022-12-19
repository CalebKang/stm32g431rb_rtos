/*
 * tick.h
 *
 *  Created on: Dec 15, 2022
 *      Author: calebkang
 */

#ifndef SRC_RTOS_TICK_H_
#define SRC_RTOS_TICK_H_

#define TIMESLICE (20)

extern void sleep_ms(int ms);
extern void tick_increase(void);

#endif /* SRC_RTOS_TICK_H_ */
