/**
 * @file
 * @brief Ring buffer implementation supporting 1 producer and 1 consumer for use in shared memory environments
 *
 * This file currently supports the following macros:
 * - RBSM_USE_16BIT_INDEX_TYPE ... set the ring buffer index (HEAD and TAIL) data type to 16-bit (default: 8-bit)
 */
/*
 * Copyright (c) 2024, Linz Center of Mechatronics GmbH (LCM) http://www.lcm.at/
 * All rights reserved.
 */
/*
 * This file is licensed according to the BSD 3-clause license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the "Linz Center of Mechatronics GmbH" and "LCM" nor
 *       the names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL "Linz Center of Mechatronics GmbH" BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * This file is part of X2C. http://x2c.lcm.at/
 * $LastChangedRevision: 3315 $
 * $LastChangedDate:: 2024-06-14 15:36:02 +0200#$
 */
/*
 * This file incorporates work covered by the following copyright and permission notice:
 *
 *     Lock-Free Ring Buffer (LFRB) for embedded systems
 *     GitHub: https://github.com/QuantumLeaps/lock-free-ring-buffer
 *
 *                        Q u a n t u m  L e a P s
 *                        ------------------------
 *                        Modern Embedded Software
 *
 *     Copyright (C) 2005 Quantum Leaps, <state-machine.com>.
 *
 *     SPDX-License-Identifier: MIT
 *
 *     Permission is hereby granted, free of charge, to any person obtaining a
 *     copy of this software and associated documentation files (the "Software"),
 *     to deal in the Software without restriction, including without limitation
 *     the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *     and/or sell copies of the Software, and to permit persons to whom the
 *     Software is furnished to do so, subject to the following conditions:
 *
 *     The above copyright notice and this permission notice shall be included in
 *     all copies or substantial portions of the Software.
 *
 *     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *     IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *     FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *     THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *     LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *     FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *     DEALINGS IN THE SOFTWARE.
 */
#ifndef RINGBUFSHAREDMEM_H
#define RINGBUFSHAREDMEM_H

#include "Target.h"

/*! Ring buffer counter/index
*
* @attention
* The following RingBufCtr type is assumed to be "atomic" in a target CPU,
* meaning that the CPU needs to write the whole RingBufCtr in one machine
* instruction. An example of violating this assumption would be an 8-bit
* CPU, which writes uint16_t (2-bytes) in 2 machine instructions. For such
* 8-bit CPU, the maximum size of RingBufCtr would be uint8_t (1-byte).
*
* Another case of violating the "atomic" writes to RingBufCtr type would
* be misalignment of a RingBufCtr variable in memory, which could cause
* the compiler to generate multiple instructions to write a RingBufCtr value.
* Therefore, it is further assumed that all RingBufCtr variables in the
* following RingBuf struct *are* correctly aligned for "atomic" access.
* In practice, most C compilers should provide such natural alignment
* (by inserting some padding into the ::RingBuf struct, if necessary).
*/
#if defined(RBSM_USE_16BIT_INDEX_TYPE)
typedef uint16_t RingBufCtr;
#else
typedef uint8_t RingBufCtr;
#endif

/*! Ring buffer element type
*
* @details
* The type of the ring buffer elements is not critical for the lock-free
* operation and does not need to be "atomic". For example, it can be
* an integer type (uint16_t, uint32_t, uint64_t), a pointer type,
* or even a struct type. However, the bigger the type the more RAM is
* needed for the ring buffer and more CPU cycles are needed to copy it
* into and out of the buffer memory.
*/
typedef uint8_t RingBufElement;

/*! Ring buffer struct */
typedef struct {
    volatile RingBufElement *buf; /*!< pointer to the start of the ring buffer */
    RingBufCtr end;  /*!< offset of the end of the ring buffer */
    volatile RingBufCtr *head; /*!< pointer to the offset to where next el. will be inserted */
    volatile RingBufCtr *tail; /*!< pointer to the offset of where next el. will be removed */
} RingBuf;

/* Public prototypes */
void initRingBuffer(RingBuf * const me, volatile RingBufElement buffer[], RingBufCtr bufferSize, volatile RingBufCtr * const head,
                  volatile RingBufCtr * const tail);
bool putRingBufferElement(RingBuf * const me, RingBufElement const element);
bool getRingBufferElement(RingBuf * const me, RingBufElement *element);
RingBufCtr getNumFreeRingBufferElements(RingBuf * const me);
RingBufCtr getNumUsedRingBufferElements(RingBuf * const me);

#endif /* RINGBUFSHAREDMEM_H */
