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
#include <stdint.h>
#include <stdbool.h>
#include "RingBufSharedMem.h"

/* Helper defines to access the HEAD- and TAIL index value */
#define RBSM_GET_HEAD(drv) (*(drv->head))
#define RBSM_GET_TAIL(drv) (*(drv->tail))


/**
 * This function initializes the ring buffer.
 * This constructor allows to supply the HEAD- and TAIL index by pointers.
 * It may be used for systems with shared memory, where 1 side has write access to the HEAD index and the other side has write access to the TAIL index.
 * @param me Driver handle
 * @param buffer Storage buffer
 * @param bufferSize Storage buffer size
 * @param head HEAD index
 * @param tail TAIL index
 */
void initRingBuffer(RingBuf * const me, volatile RingBufElement buffer[], RingBufCtr bufferSize, volatile RingBufCtr * const head,
                  volatile RingBufCtr * const tail)
{
    me->buf  = &buffer[0];
    me->end  = bufferSize;
    me->head = head;
    me->tail = tail;
}

/**
 * This function adds a new element to the buffer. If the buffer is full, the element is not added to the buffer.
 * @param me Driver handle
 * @param[in] element New element
 * @retval true Element was successfully added to the buffer
 * @retval false Element was not added to the buffer because it is full
 */
bool putRingBufferElement(RingBuf * const me, RingBufElement const element)
{
    RingBufCtr head = RBSM_GET_HEAD(me) + 1U;
    if (head == me->end)
    {
        head = 0U;
    }
    if (head != RBSM_GET_TAIL(me))
    { /* buffer NOT full? */
        me->buf[RBSM_GET_HEAD(me)] = element; /* copy the element into the buffer */
        RBSM_GET_HEAD(me) = head; /* update the head to a *valid* index */
        return (true);  /* element placed in the buffer */
    }
    else
    {
        return (false); /* element NOT placed in the buffer */
    }
}

/**
 * This function gets the next buffer element. If the buffer is empty, no element is returned.
 * @param me Driver handle
 * @param[out] element Next buffer element
 * @retval true Buffer element returned
 * @retval false No buffer element returned because buffer is empty
 */
bool getRingBufferElement(RingBuf * const me, RingBufElement *element)
{
    RingBufCtr tail = RBSM_GET_TAIL(me);
    if (RBSM_GET_HEAD(me) != tail)
    { /* ring buffer NOT empty? */
        *element = me->buf[tail];
        ++tail;
        if (tail == me->end)
        {
            tail = 0U;
        }
        RBSM_GET_TAIL(me) = tail; /* update the tail to a *valid* index */
        return (true);
    }
    else
    {
        return (false);
    }
}

/**
 * This function returns the number of the free (not used) buffer elements.
 * @param me Driver handle
 * @returns Number of free elements
 */
RingBufCtr getNumFreeRingBufferElements(RingBuf * const me)
{
    RingBufCtr head = RBSM_GET_HEAD(me);
    RingBufCtr tail = RBSM_GET_TAIL(me);
    if (head == tail)
    { /* buffer empty? */
        return ((RingBufCtr)(me->end - 1U));
    }
    else if (head < tail)
    {
        return ((RingBufCtr)(tail - head - 1U));
    }
    else
    {
        return ((RingBufCtr)(me->end + tail - head - 1U));
    }
}

/**
 * This function returns the number of the used buffer elements.
 * @param me Driver handle
 * @returns Number of used elements
 */
RingBufCtr getNumUsedRingBufferElements(RingBuf * const me)
{
    return ((me->end - 1U - getNumFreeRingBufferElements(me)));
}
