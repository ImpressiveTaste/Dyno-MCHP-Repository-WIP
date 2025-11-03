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
/*     Description:    I Controller with
 *                         - Enable Input
 *                         - Anti Wind-up
 *                         - Initial Condition Input
 *                         - Output Limitation
 *                     Calculation ZOH:
 *                                        1
 *                         y = ( Ki*Ts* -----  ) * u
 *                                      z - 1
 *
 *                     -> y(k) = b1.u(k) + b0.u(k-1) + y(k-1)
 */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "ILimit_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(ILimit_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)       /* Q31 */
#define INIT            (*block->I0)       /* Q31 */
#define ENABLE          (*block->Enable)   /* Q0 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*block->max)      /* Q31 */
    #define LIMIT_DOWN  (*block->min)      /* Q31 */
#endif

/* Outputs */
#define OUT             (block->Out)       /* Q31 */

/* Parameter */
#define B0              (block->b0)        /* Qy */
#define SFR0            (block->sfr)       /* y */

/* Variables */
#define I_OLD           (block->i_old)     /* Q62 */
#define ENABLE_OLD      (block->enable_old)
/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void ILimit_FiP32_Update(ILIMIT_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 y;

    if (ENABLE) /* Block enabled */
    {
        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Initialize integral part */
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0004", true_no_defect) */
            I_OLD = ((int64)INIT) << 31;
        }

        /* Calculate output */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
        y = I_OLD >> 31;                                    /* Q31 */

        /* Calculate integral part */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0003", true_no_defect) */
        I_OLD += (((int64)B0 * (int64)IN) << (31-SFR0));    /* Q62 */

        /* Output limitation and anti wind-up */
        if (y > LIMIT_UP)
        {
            /* Output beyond upper limit -> clamp */
            y = LIMIT_UP;

            /* Limitation of integral part (anti wind-up) */
            /* RULECHECKER_comment(1:0, 3:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0004", true_no_defect) */
            if (I_OLD > ((int64)LIMIT_UP << 31))
            {
                I_OLD = (int64)LIMIT_UP << 31;
            }
        }
        else if (y < LIMIT_DOWN)
        {
            /* Output limitation to lower boundary */
            y = LIMIT_DOWN;

            /* Limitation of integral part (anti wind-up) */
            /* RULECHECKER_comment(1:0, 3:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0004", true_no_defect) */
            if (I_OLD < ((int64)LIMIT_DOWN << 31))
            {
                I_OLD = (int64)LIMIT_DOWN << 31;
            }
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
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void ILimit_FiP32_Init(ILIMIT_FIP32 *block)
{
    block->ID = ILIMIT_FIP32_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0004", true_no_defect) */
    I_OLD = ((int64)INIT) << 31;
    ENABLE_OLD = false;
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 ILimit_FiP32_Load(const ILIMIT_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 5U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint32)block->b0 & 0xFFU);
        data[1] = (uint8)(((uint32)block->b0 >> 8) & 0xFFU);
        data[2] = (uint8)(((uint32)block->b0 >> 16) & 0xFFU);
        data[3] = (uint8)(((uint32)block->b0 >> 24) & 0xFFU);
        data[4] = (uint8)(block->sfr & 0xFFU);
        *dataLength = 5U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 ILimit_FiP32_Save(ILIMIT_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 5U)
    {
        error = 1U;
    }
    else
    {
        block->b0 = castUint32ToInt32((uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24)));
        block->sfr = data[4];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(ILIMIT_FIP32_ISLINKED)
void* ILimit_FiP32_GetAddress(ILIMIT_FIP32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        case 2:
            addr = block->I0;
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
