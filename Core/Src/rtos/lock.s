/*
 * lock.s
 *
 *  Created on: Dec 15, 2022
 *      Author: calebkang
 */
/*
 *
 * Embedded System Architecture
 * Copyright (c) 2018 Packt
 *
 * Author: Daniele Lacamera <root@danielinux.net>
 *
 * MIT License
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
.syntax unified

.global sem_trywait
sem_trywait:
   LDREX   r1, [r0]
   CMP     r1, #0
   BEQ     sem_trywait_fail
   SUBS    r1, #1
   STREX   r2, r1, [r0]
   CMP     r2, #0
   BNE     sem_trywait
   DMB
   MOVS    r0, #0
   BX      lr
sem_trywait_fail:
   DMB
   MOV     r0, #-1
   BX      lr

.global sem_dopost
sem_dopost:
   LDREX   r1, [r0]
   ADDS    r1, #1
   STREX   r2, r1, [r0]
   CMP     r2, #0
   BNE     sem_dopost
   CMP     r0, #1
   DMB
   BGE     sem_signal_up
   MOVS    r0, #0
   BX      lr
sem_signal_up:
   MOVS    r0, #1
   BX      lr

