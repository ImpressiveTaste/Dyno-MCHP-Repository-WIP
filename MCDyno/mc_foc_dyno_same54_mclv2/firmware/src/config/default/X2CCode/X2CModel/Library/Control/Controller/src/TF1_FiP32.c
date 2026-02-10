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
/**     Description:    First order transfer function                                                                **/
/**                     Calculation:                                                                                 **/
/**                              b1.z + b0                                                                           **/
/**                         y = ----------- u                                                                        **/
/**                                z + a0                                                                            **/
/**                         -> y(k) = u(k).b1 + u(k-1).b0 - y(k-1).a0                                                **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "TF1_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TF1_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN          (*block->In)      /* Q31 */

/* Outputs */
#define OUT         (block->Out)      /* Q31 */

/* Parameter */
#define A0          (block->a0)       /* Qa */
#define B0          (block->b0)       /* Qb */
#define B1          (block->b1)       /* Qb */
#define SFRA        (block->sfra)     /* a */
#define SFRB        (block->sfrb)     /* b */

/* Variables */
#define IN_OLD      (block->in_old)   /* Q31 */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void TF1_FiP32_Update(TF1_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 temp;

    temp  = ((int64)B0 * (int64)IN_OLD);        /* y = b0.u(k-1) */
    temp += ((int64)B1 * (int64)IN);            /* y = b0.u(k-1) + b1.u(k) */

    /* RULECHECKER_comment(1:0, 2:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  = temp >> SFRB;       /* Q31 */
    temp -= (((int64)A0 * (int64)OUT) >> SFRA); /* y = b0.u(k-1) + b1.u(k) - a0.y(k-1) */

    /* check for overflow, though it shouldn't be possible to get one */
    if (temp > (int64)INT32_MAX)
    {
        temp = (int64)INT32_MAX;
    }
    else
    {
        if (temp < -(int64)INT32_MAX)
        {
            temp = -(int64)INT32_MAX;
        }
    }

    /* save values for next update call and assign output */
    IN_OLD = IN;
    OUT    = (int32)temp;


/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void TF1_FiP32_Init(TF1_FIP32 *block)
{
    block->ID = TF1_FIP32_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset old input value */
     IN_OLD = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TF1_FiP32_Load(const TF1_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 14U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint32)block->b0 & 0xFFU);
        data[1] = (uint8)(((uint32)block->b0 >> 8) & 0xFFU);
        data[2] = (uint8)(((uint32)block->b0 >> 16) & 0xFFU);
        data[3] = (uint8)(((uint32)block->b0 >> 24) & 0xFFU);
        data[4] = (uint8)((uint32)block->b1 & 0xFFU);
        data[5] = (uint8)(((uint32)block->b1 >> 8) & 0xFFU);
        data[6] = (uint8)(((uint32)block->b1 >> 16) & 0xFFU);
        data[7] = (uint8)(((uint32)block->b1 >> 24) & 0xFFU);
        data[8] = (uint8)((uint32)block->a0 & 0xFFU);
        data[9] = (uint8)(((uint32)block->a0 >> 8) & 0xFFU);
        data[10] = (uint8)(((uint32)block->a0 >> 16) & 0xFFU);
        data[11] = (uint8)(((uint32)block->a0 >> 24) & 0xFFU);
        data[12] = (uint8)(block->sfrb & 0xFFU);
        data[13] = (uint8)(block->sfra & 0xFFU);
        *dataLength = 14U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TF1_FiP32_Save(TF1_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 14U)
    {
        error = 1U;
    }
    else
    {
        block->b0 = castUint32ToInt32((uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24)));
        block->b1 = castUint32ToInt32((uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24)));
        block->a0 = castUint32ToInt32((uint32)((uint32)data[8] | ((uint32)data[9] << 8) | ((uint32)data[10] << 16) | ((uint32)data[11] << 24)));
        block->sfrb = data[12];
        block->sfra = data[13];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TF1_FIP32_ISLINKED)
void* TF1_FiP32_GetAddress(TF1_FIP32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        case 2:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
