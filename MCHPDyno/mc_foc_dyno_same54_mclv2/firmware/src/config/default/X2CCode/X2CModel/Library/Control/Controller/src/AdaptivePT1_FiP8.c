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
 * $LastChangedRevision: 3528 $
 * $LastChangedDate:: 2024-12-03 18:52:28 +0100#$
 */
/* USERCODE-BEGIN:Description                                                                                         */
/**     Description:    First order proportional element with adjustable cut-off frequency                           **/
/**                     Calculation:                                                                                 **/
/**                                 b0                                                                               **/
/**                         y = ------ u                                                                             **/
/**                             z + a0                                                                               **/
/**                         -> y(k) = u(k-1).b0 - y(k-1).a0                                                          **/
/**                             with a0 = -exp(-2*pi*Ts*fc)                                                          **/
/**                             and  b0 = V*(1 - exp(-2*pi*Ts*fc))                                                   **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "AdaptivePT1_FiP8.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(AdaptivePT1_FiP8_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
#include "Exp_Data.h"

/* Inputs */
#define IN          (*block->In)
#define F_C         (*block->fc)

/* Outputs */
#define OUT         (block->Out)

/* Parameter */
#define W_SCALE     (block->w_scale)   /* Calculation base for wc: -2*pi*Ts*fmax */
#define GAIN        (block->gain)      /* Gain */
#define SFR         (block->sfr)       /* Shift factor for gain */

/* Variables */
#define IN_OLD      (block->in_old)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void AdaptivePT1_FiP8_Update(ADAPTIVEPT1_FIP8 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int8 exp;
    int16 temp;

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    temp = ((int16)F_C * (int16)W_SCALE) >> 7;          /* -2*pi*Ts*fc */

    #if defined (__C2000__)
        /* on Texas Instruments DSPs 16 bits are used for int8 datatypes -> masking is required */
        /* Previous: LOOKUP8(Exp_Table8, (temp & 0x00FF), exp); */
        exp = lookupTableInt8(Exp_Table8, (temp & 0x00FF));  /* exp(-2*pi*Ts*fc) */
    #else
        /* Previous: LOOKUP8(Exp_Table8, (int8)temp, exp); */
        exp = lookupTableInt8(Exp_Table8, (int8)temp);         /* exp(-2*pi*Ts*fc) */
    #endif

    temp  = INT8_MAX - exp;                             /* 1 - exp(-wc*Ts) */

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  = ((int16)temp * (int16)(GAIN)) >> SFR;       /* V*(1-exp(-wc*Ts) [Q7] */
    temp  = ((int16)temp * (int16)IN_OLD);              /* u(k-1)*V*(1-exp(-wc*Ts) [Q15] */

    temp += ((int16)exp * (int16)OUT);                  /* y(k-1)*exp(-wc*Ts) + u(k-1)*V*(1-exp(-wc*Ts) [Q15]*/

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    temp  = temp >> 7;                                  /* [Q15] -> [Q7] */

    /* Data width limitation Previous: LIMIT(temp, INT8_MAX); */
    if (temp > INT8_MAX)
    {
        temp = INT8_MAX;
    }
    else
    {
        if (temp < -INT8_MAX)
        {
            temp = -INT8_MAX;
        }
    }

    OUT = (int8)temp;                                   /* assign output */
    IN_OLD = IN;                                        /* save input for next cycle */

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void AdaptivePT1_FiP8_Init(ADAPTIVEPT1_FIP8 *block)
{
    block->ID = ADAPTIVEPT1_FIP8_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset old input value */
     IN_OLD = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 AdaptivePT1_FiP8_Load(const ADAPTIVEPT1_FIP8 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 3U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint8)block->w_scale & 0xFFU);
        data[1] = (uint8)((uint8)block->gain & 0xFFU);
        data[2] = (uint8)(block->sfr & 0xFFU);
        *dataLength = 3U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 AdaptivePT1_FiP8_Save(ADAPTIVEPT1_FIP8 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 3U)
    {
        error = 1U;
    }
    else
    {
        block->w_scale = castUint8ToInt8(data[0]);
        block->gain = castUint8ToInt8(data[1]);
        block->sfr = data[2];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(ADAPTIVEPT1_FIP8_ISLINKED)
void* AdaptivePT1_FiP8_GetAddress(ADAPTIVEPT1_FIP8* block, uint16 elementId)
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
