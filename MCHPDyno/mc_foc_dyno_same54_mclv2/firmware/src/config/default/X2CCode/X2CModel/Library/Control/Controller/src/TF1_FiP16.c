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
/**                               z + a0                                                                             **/
/**                         -> y(k) = u(k).b1 + u(k-1).b0 - y(k-1).a0                                                **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "TF1_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TF1_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN          (*block->In)      /* Q15 */

/* Outputs */
#define OUT         (block->Out)      /* Q15 */

/* Parameter */
#define A0          (block->a0)       /* Qa */
#define B0          (block->b0)       /* Qb */
#define B1          (block->b1)       /* Qb */
#define SFRA        (block->sfra)     /* a */
#define SFRB        (block->sfrb)     /* b */

/* Variables */
#define IN_OLD      (block->in_old)   /* Q15 */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void TF1_FiP16_Update(TF1_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 temp;

    temp  = ((int32)B0 * (int32)IN_OLD);        /* y = b0.u(k-1) */
    temp += ((int32)B1 * (int32)IN);        /* y = b0.u(k-1) + b1.u(k) */

    /* RULECHECKER_comment(1:0, 2:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  = temp >> SFRB;           /* Q15 */
    temp -= (((int32)A0 * (int32)OUT) >> SFRA); /* y = b0.u(k-1) + b1.u(k) - a0.y(k-1) */

    /* check for overflow */
    if (temp > (int32)INT16_MAX)
    {
        temp = (int32)INT16_MAX;
    }
    else
    {
        if (temp < -(int32)INT16_MAX)
        {
            temp = -(int32)INT16_MAX;
        }
    }

    /* save values for next update call and assign output */
    IN_OLD = IN;
    OUT    = (int16)temp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void TF1_FiP16_Init(TF1_FIP16 *block)
{
    block->ID = TF1_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset old input value */
     IN_OLD = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TF1_FiP16_Load(const TF1_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 8U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint16)block->b0 & 0xFFU);
        data[1] = (uint8)(((uint16)block->b0 >> 8) & 0xFFU);
        data[2] = (uint8)((uint16)block->b1 & 0xFFU);
        data[3] = (uint8)(((uint16)block->b1 >> 8) & 0xFFU);
        data[4] = (uint8)((uint16)block->a0 & 0xFFU);
        data[5] = (uint8)(((uint16)block->a0 >> 8) & 0xFFU);
        data[6] = (uint8)(block->sfrb & 0xFFU);
        data[7] = (uint8)(block->sfra & 0xFFU);
        *dataLength = 8U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TF1_FiP16_Save(TF1_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 8U)
    {
        error = 1U;
    }
    else
    {
        block->b0 = castUint16ToInt16((uint16)((uint16)data[0] | ((uint16)data[1] << 8)));
        block->b1 = castUint16ToInt16((uint16)((uint16)data[2] | ((uint16)data[3] << 8)));
        block->a0 = castUint16ToInt16((uint16)((uint16)data[4] | ((uint16)data[5] << 8)));
        block->sfrb = data[6];
        block->sfra = data[7];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TF1_FIP16_ISLINKED)
void* TF1_FiP16_GetAddress(TF1_FIP16* block, uint16 elementId)
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
