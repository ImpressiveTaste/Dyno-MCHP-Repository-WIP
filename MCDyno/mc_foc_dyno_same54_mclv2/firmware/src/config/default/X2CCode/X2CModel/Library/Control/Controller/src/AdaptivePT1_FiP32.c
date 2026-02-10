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
/**     Description:    First order proportional element with adjustable cut-off frequency                           **/
/**                     Calculation:                                                                                 **/
/**                               b0                                                                                 **/
/**                         y = ------ u                                                                             **/
/**                             z + a0                                                                               **/
/**                         -> y(k) = u(k-1).b0 - y(k-1).a0                                                          **/
/**                             with a0 = -exp(-2*pi*Ts*fc)                                                          **/
/**                             and  b0 = V*(1 - exp(-2*pi*Ts*fc))                                                   **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "AdaptivePT1_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(AdaptivePT1_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
#include "Exp_Data.h"


/* Inputs */
#define IN          (*block->In)
#define F_C         (*block->fc)

/* Outputs */
#define OUT         (block->Out)

/* Parameter */
#define W_SCALE     (block->w_scale)  /* Calculation base for wc: -2*pi*Ts*fmax */
#define GAIN        (block->gain)     /* Gain */
#define SFR         (block->sfr)      /* Shift factor for gain */

/* Variables */
#define IN_OLD      (block->in_old)
/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void AdaptivePT1_FiP32_Update(ADAPTIVEPT1_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 exp;
    int64 temp;

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    temp = ((int64)F_C * (int64)W_SCALE) >> 31;     /* -2*pi*Ts*fc */

    /* Previous: LOOKUP32(Exp_Table32, (int32)temp, exp); */
    exp = lookupTableInt32(Exp_Table32, (int32)temp);       /* exp(-2*pi*Ts*fc) */

    temp  = INT32_MAX - exp;                        /* 1 - exp(-wc*Ts) */

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  = ((int64)temp * (int64)(GAIN)) >> SFR;   /* V*(1-exp(-wc*Ts) [Q31] */

    temp  = ((int64)temp * (int64)IN_OLD);          /* u(k-1)*V*(1-exp(-wc*Ts) [Q62] */

    temp += ((int64)exp * (int64)OUT);              /* y(k-1)*exp(-wc*Ts) + u(k-1)*V*(1-exp(-wc*Ts) [Q62]*/

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    temp  = temp >> 31;                             /* [Q62] -> [Q31] */

    /* Data width limitation Previous: LIMIT(temp, INT32_MAX); */
    if (temp > INT32_MAX)
    {
        temp = INT32_MAX;
    }
    else
    {
        if (temp < -INT32_MAX)
        {
            temp = -INT32_MAX;
        }
    }

    OUT = (int32)temp;                              /* assign output */
    IN_OLD = IN;                                    /* save input for next cycle */

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void AdaptivePT1_FiP32_Init(ADAPTIVEPT1_FIP32 *block)
{
    block->ID = ADAPTIVEPT1_FIP32_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset old input value */
     IN_OLD = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 AdaptivePT1_FiP32_Load(const ADAPTIVEPT1_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 9U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint32)block->w_scale & 0xFFU);
        data[1] = (uint8)(((uint32)block->w_scale >> 8) & 0xFFU);
        data[2] = (uint8)(((uint32)block->w_scale >> 16) & 0xFFU);
        data[3] = (uint8)(((uint32)block->w_scale >> 24) & 0xFFU);
        data[4] = (uint8)((uint32)block->gain & 0xFFU);
        data[5] = (uint8)(((uint32)block->gain >> 8) & 0xFFU);
        data[6] = (uint8)(((uint32)block->gain >> 16) & 0xFFU);
        data[7] = (uint8)(((uint32)block->gain >> 24) & 0xFFU);
        data[8] = (uint8)(block->sfr & 0xFFU);
        *dataLength = 9U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 AdaptivePT1_FiP32_Save(ADAPTIVEPT1_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 9U)
    {
        error = 1U;
    }
    else
    {
        block->w_scale = castUint32ToInt32((uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24)));
        block->gain = castUint32ToInt32((uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24)));
        block->sfr = data[8];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(ADAPTIVEPT1_FIP32_ISLINKED)
void* AdaptivePT1_FiP32_GetAddress(ADAPTIVEPT1_FIP32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        case 2:
            addr = block->fc;
            break;
        case 3:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
