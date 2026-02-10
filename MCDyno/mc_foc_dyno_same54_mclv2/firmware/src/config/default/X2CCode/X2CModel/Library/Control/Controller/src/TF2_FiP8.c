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
/**     Description:    Second order transfer function                                                               **/
/**                     Calculation:                                                                                 **/
/**                              b2.z^2 + b1.z + b0                                                                  **/
/**                         y = ------------------ u                                                                 **/
/**                               z^2 + a1.z + a0                                                                    **/
/**                         -> y(k) = u(k).b2 + u(k-1).b1 + u(k-2).b0                                                **/
/**                                 - y(k-1).a1 - y(k-2).a0                                                          **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "TF2_FiP8.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TF2_FiP8_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN          (*block->In)           /* Q7 */

/* Outputs */
#define OUT         (block->Out)           /* Q7 */

/* Parameter */
#define A0          (block->a0)            /* Qa */
#define A1          (block->a1)            /* Qa */
#define B0          (block->b0)            /* Qb */
#define B1          (block->b1)            /* Qb */
#define B2          (block->b2)            /* Qb */
#define SFRA        (block->sfra)          /* a */
#define SFRB        (block->sfrb)          /* b */

/* Variables */
#define IN_OLD      (block->in_old)        /* Q7 */
#define IN_VERYOLD  (block->in_veryold)    /* Q7 */
#define OUT_OLD     (block->out_old)       /* Q7 */
#define OUT_VERYOLD (block->out_veryold)   /* Q7 */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void TF2_FiP8_Update(TF2_FIP8 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int16 temp, temp1;

    /* RULECHECKER_comment(1:0, 3:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    temp  =  ((int16)B0 * (int16)IN_VERYOLD) >> 1;  /* y(k) = b0.u(k-2) */
    temp += (((int16)B1 * (int16)IN_OLD)     >> 1); /* y(k) = b0.u(k-2) + b1.u(k-1) */
    temp += (((int16)B2 * (int16)IN)         >> 1); /* y(k) = b0.u(k-2) + b1.u(k-1) + b2.u(k) */

    if (1 > SFRB)   /* check which direction to shift */
    {
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0003", true_no_defect) */
        temp = temp << (1-SFRB);                    /* Q7 */
    }
    else
    {
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        temp = temp >> (SFRB-1);                    /* Q7 */
    }

    temp1  =  (int16)A0 * (int16)OUT_VERYOLD;       /* y1(k) = a0.y(k-2) */
    temp1 += ((int16)A1 * (int16)OUT_OLD);          /* y1(k) = a0.y(k-2) + a1.y(k-1) */

    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp1  = temp1 >> SFRA;                         /* Q7 */

    temp   = temp - temp1;                          /* y(k) = b0.u(k-2) + b1.u(k-1) + b2.u(k) - a0.y(k-2) - a1.y(k-1)*/

    /* check for overflow */
    if (temp > (int16)INT8_MAX)
    {
        temp = (int16)INT8_MAX;
    }
    else
    {
        if (temp < -(int16)INT8_MAX)
        {
            temp = -(int16)INT8_MAX;
        }
    }

    /* save values for next update call and assign output */
    IN_VERYOLD  = IN_OLD;
    IN_OLD      = IN;
    OUT_VERYOLD = OUT_OLD;
    OUT_OLD     = (int8)temp;
    OUT         = OUT_OLD;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void TF2_FiP8_Init(TF2_FIP8 *block)
{
    block->ID = TF2_FIP8_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset old values */
     IN_OLD      = 0;
     IN_VERYOLD  = 0;
     OUT_OLD     = 0;
     OUT_VERYOLD = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TF2_FiP8_Load(const TF2_FIP8 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 7U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint8)block->b0 & 0xFFU);
        data[1] = (uint8)((uint8)block->b1 & 0xFFU);
        data[2] = (uint8)((uint8)block->b2 & 0xFFU);
        data[3] = (uint8)((uint8)block->a0 & 0xFFU);
        data[4] = (uint8)((uint8)block->a1 & 0xFFU);
        data[5] = (uint8)(block->sfrb & 0xFFU);
        data[6] = (uint8)(block->sfra & 0xFFU);
        *dataLength = 7U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TF2_FiP8_Save(TF2_FIP8 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 7U)
    {
        error = 1U;
    }
    else
    {
        block->b0 = castUint8ToInt8(data[0]);
        block->b1 = castUint8ToInt8(data[1]);
        block->b2 = castUint8ToInt8(data[2]);
        block->a0 = castUint8ToInt8(data[3]);
        block->a1 = castUint8ToInt8(data[4]);
        block->sfrb = data[5];
        block->sfra = data[6];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TF2_FIP8_ISLINKED)
void* TF2_FiP8_GetAddress(TF2_FIP8* block, uint16 elementId)
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
