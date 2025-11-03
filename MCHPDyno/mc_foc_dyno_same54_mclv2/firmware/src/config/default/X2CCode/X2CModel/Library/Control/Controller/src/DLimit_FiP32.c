/*
 * Copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM), web: www.lcm.at
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
 * This file is part of X2C. web: x2c.lcm.at
 * $LastChangedRevision: 3474 $
 * $LastChangedDate:: 2024-11-06 18:00:53 +0100#$
 */
/* USERCODE-BEGIN:Description                                                                                         */
/*     Description:  D Controller with
 *                          - Enable Input
 *                          - Initial Condition Input
 *                          - Output Limitation
 *                      Calculation ZOH:
 *                                      z - 1
 *                      y = (Kd*wc --------------- )*u
 *                                 z - exp(-wc*Ts)
 *
 *                      -> y(k) = y(k-1) + b1d.u(k) + b0d.u(k-1) - a0d.yd(k-1)
 */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "DLimit_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(DLimit_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)           /* Q31 */
#define INIT            (*block->D0)           /* Q31 */
#define ENABLE          (*block->Enable)       /* Q0 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*block->max)          /* Q62 */
    #define LIMIT_DOWN  (*block->min)          /* Q62 */
#endif

/* Outputs */
#define OUT             (block->Out)           /* Q31 */

/* Parameter */
#define B0D             (block->b0d)           /* Qz */
#define B1D             (block->b1d)           /* Qz */
#define A0D             (block->a0d)           /* Q31 */

#define SFRD            (block->sfrd)          /* z */

/* Variables */
#define IN_OLD          (block->in_old)        /* Q31 */
#define ENABLE_OLD      (block->enable_old)
#define OUT_OLD         (OUT)                           /* Out holds the old value before it's getting updated */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void DLimit_FiP32_Update(DLIMIT_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 y;

    if (ENABLE) /* Block enabled */
    {
        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Initialize derivative part */
            IN_OLD = INIT;
        }

        /* Derivative term */
        y  =  ((int64)B0D * (int64)IN_OLD);         /* Qz */
        y += (((int64)B1D * (int64)IN));            /* Qz */

        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        y  = y >> SFRD;                             /* Qz -> Q31 */

        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
        y -= (((int64)A0D * (int64)OUT_OLD) >> 31); /* Q31 */

        /* Data width limitation Previous: LIMIT(y, INT32_MAX); */
        if (y > INT32_MAX)
        {
            y = INT32_MAX;
        }
        else
        {
            if (y < -INT32_MAX)
            {
                y = -INT32_MAX;
            }
        }

        /* Output limitation */
        if (y > LIMIT_UP)           /* Output is beyond upper limit */
        {
            /* Limitation of output to upper boundary */
            y = LIMIT_UP;
        }
        else if (y < LIMIT_DOWN)    /* Output is beyond lower limit */
        {
            /* Limitation of output to lower boundary */
            y = LIMIT_DOWN;
        }
        else
        {
            /* no output limitation */
        }

        /* Assign output */
        OUT = (int32)y;
    }
    else                /* Block disabled */
    {
        OUT = 0;    /* Reset output */
    }

    /* Save relevant data for next call */
    IN_OLD = IN;
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void DLimit_FiP32_Init(DLIMIT_FIP32 *block)
{
    block->ID = DLIMIT_FIP32_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    IN_OLD = 0;
    ENABLE_OLD = false;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 DLimit_FiP32_Load(const DLIMIT_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 13U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint32)block->b0d & 0xFFU);
        data[1] = (uint8)(((uint32)block->b0d >> 8) & 0xFFU);
        data[2] = (uint8)(((uint32)block->b0d >> 16) & 0xFFU);
        data[3] = (uint8)(((uint32)block->b0d >> 24) & 0xFFU);
        data[4] = (uint8)((uint32)block->b1d & 0xFFU);
        data[5] = (uint8)(((uint32)block->b1d >> 8) & 0xFFU);
        data[6] = (uint8)(((uint32)block->b1d >> 16) & 0xFFU);
        data[7] = (uint8)(((uint32)block->b1d >> 24) & 0xFFU);
        data[8] = (uint8)((uint32)block->a0d & 0xFFU);
        data[9] = (uint8)(((uint32)block->a0d >> 8) & 0xFFU);
        data[10] = (uint8)(((uint32)block->a0d >> 16) & 0xFFU);
        data[11] = (uint8)(((uint32)block->a0d >> 24) & 0xFFU);
        data[12] = (uint8)(block->sfrd & 0xFFU);
        *dataLength = 13U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 DLimit_FiP32_Save(DLIMIT_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 13U)
    {
        error = 1U;
    }
    else
    {
        block->b0d = castUint32ToInt32((uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24)));
        block->b1d = castUint32ToInt32((uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24)));
        block->a0d = castUint32ToInt32((uint32)((uint32)data[8] | ((uint32)data[9] << 8) | ((uint32)data[10] << 16) | ((uint32)data[11] << 24)));
        block->sfrd = data[12];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(DLIMIT_FIP32_ISLINKED)
void* DLimit_FiP32_GetAddress(DLIMIT_FIP32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        case 2:
            addr = block->D0;
            break;
        case 3:
            addr = block->max;
            break;
        case 4:
            addr = block->min;
            break;
        case 5:
            addr = block->Enable;
            break;
        case 6:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
