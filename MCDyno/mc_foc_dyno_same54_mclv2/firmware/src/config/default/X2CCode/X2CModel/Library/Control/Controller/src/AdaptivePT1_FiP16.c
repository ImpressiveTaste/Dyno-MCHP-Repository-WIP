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
#include "AdaptivePT1_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(AdaptivePT1_FiP16_Update, "ramfuncs")
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
void AdaptivePT1_FiP16_Update(ADAPTIVEPT1_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int16 exp;
    int32 temp;

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    temp = ((int32)F_C * (int32)W_SCALE) >> 15;             /* -2*pi*Ts*fc */

    /* Previous: LOOKUP16(Exp_Table16, (int16)temp, exp); */
    exp = lookupTableInt16(Exp_Table16, (int16)temp);       /* exp(-2*pi*Ts*fc) */

    temp  = INT16_MAX - exp;                                /* 1 - exp(-wc*Ts) */

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  = ((int32)temp * (int32)(GAIN)) >> SFR;           /* V*(1-exp(-wc*Ts) [Q15] */
    temp  = ((int32)temp * (int32)IN_OLD);                  /* u(k-1)*V*(1-exp(-wc*Ts) [Q30] */

    temp += ((int32)exp * (int32)OUT);                      /* y(k-1)*exp(-wc*Ts) + u(k-1)*V*(1-exp(-wc*Ts) [Q30]*/

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    temp  = temp >> 15;                                     /* [Q30] -> [Q15] */

    /* Data width limitation Previous: LIMIT(temp, INT16_MAX); */
    if (temp > INT16_MAX)
    {
        temp = INT16_MAX;
    }
    else
    {
        if (temp < -INT16_MAX)
        {
            temp = -INT16_MAX;
        }
    }

    OUT = (int16)temp;                                      /* assign output */
    IN_OLD = IN;                                            /* save input for next cycle */

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void AdaptivePT1_FiP16_Init(ADAPTIVEPT1_FIP16 *block)
{
    block->ID = ADAPTIVEPT1_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* reset old input value */
    IN_OLD = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 AdaptivePT1_FiP16_Load(const ADAPTIVEPT1_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 5U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint16)block->w_scale & 0xFFU);
        data[1] = (uint8)(((uint16)block->w_scale >> 8) & 0xFFU);
        data[2] = (uint8)((uint16)block->gain & 0xFFU);
        data[3] = (uint8)(((uint16)block->gain >> 8) & 0xFFU);
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
uint8 AdaptivePT1_FiP16_Save(ADAPTIVEPT1_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 5U)
    {
        error = 1U;
    }
    else
    {
        block->w_scale = castUint16ToInt16((uint16)((uint16)data[0] | ((uint16)data[1] << 8)));
        block->gain = castUint16ToInt16((uint16)((uint16)data[2] | ((uint16)data[3] << 8)));
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
#if !defined(ADAPTIVEPT1_FIP16_ISLINKED)
void* AdaptivePT1_FiP16_GetAddress(ADAPTIVEPT1_FIP16* block, uint16 elementId)
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
