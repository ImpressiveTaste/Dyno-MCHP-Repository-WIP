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
/**     Description: 1st Order Time Discrete System                                                                  **/
/**                  Calculation:                                                                                    **/
/**                     x(k+1) = a11*x(k) + b11*u(k)                                                                 **/
/**                     y(k)   = c11*x(k) + d11*u(k)                                                                 **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "TDSystemO1_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TDSystemO1_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs*/
#define IN      (*block->In)       /* Q15 */

/* Outputs*/
#define OUT     (block->Out)       /* Q15 */

/* Matrices */
#define A11     (block->a11)       /* Qa */
#define B11     (block->b11)       /* Qb */
#define C11     (block->c11)       /* Qc */
#define D11     (block->d11)       /* Qd */

/* Shift factor of matrices */
#define SFRA11  (block->sfra11)    /* a */
#define SFRB11  (block->sfrb11)    /* b */
#define SFRC11  (block->sfrc11)    /* c */
#define SFRD11  (block->sfrd11)    /* d */

/* State */
#define X1      (block->x1)        /* Q15 */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void TDSystemO1_FiP16_Update(TDSYSTEMO1_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 temp;

    /* Calculation of output (with old state) */
    /* RULECHECKER_comment(1:0, 2:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  =  ((int32)C11 * (int32)X1) >> SFRC11;    /* y = c11*x1 */
    temp += (((int32)D11 * (int32)IN) >> SFRD11);   /* y = c11*x1 + d11*u */

    /* limit output to 16Bit range Previous: LIMIT(temp, INT16_MAX);*/
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

    OUT = (int16)temp;                              /* assign output */

    /* Calculation of new state */
    /* RULECHECKER_comment(1:0, 2:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  =  ((int32)A11 * (int32)X1) >> SFRA11;    /* x1 = a11*x1 */
    temp += (((int32)B11 * (int32)IN) >> SFRB11);   /* x1 = a11*x1 + b11*u */

    /* limit output to 16Bit range Previous: LIMIT(temp, INT16_MAX);*/
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

    X1 = (int16)temp;                               /* assign state */

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void TDSystemO1_FiP16_Init(TDSYSTEMO1_FIP16 *block)
{
    block->ID = TDSYSTEMO1_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset state */
     X1 = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO1_FiP16_Load(const TDSYSTEMO1_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 12U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint16)block->a11 & 0xFFU);
        data[1] = (uint8)(((uint16)block->a11 >> 8) & 0xFFU);
        data[2] = (uint8)((uint16)block->b11 & 0xFFU);
        data[3] = (uint8)(((uint16)block->b11 >> 8) & 0xFFU);
        data[4] = (uint8)((uint16)block->c11 & 0xFFU);
        data[5] = (uint8)(((uint16)block->c11 >> 8) & 0xFFU);
        data[6] = (uint8)((uint16)block->d11 & 0xFFU);
        data[7] = (uint8)(((uint16)block->d11 >> 8) & 0xFFU);
        data[8] = (uint8)(block->sfra11 & 0xFFU);
        data[9] = (uint8)(block->sfrb11 & 0xFFU);
        data[10] = (uint8)(block->sfrc11 & 0xFFU);
        data[11] = (uint8)(block->sfrd11 & 0xFFU);
        *dataLength = 12U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO1_FiP16_Save(TDSYSTEMO1_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 12U)
    {
        error = 1U;
    }
    else
    {
        block->a11 = castUint16ToInt16((uint16)((uint16)data[0] | ((uint16)data[1] << 8)));
        block->b11 = castUint16ToInt16((uint16)((uint16)data[2] | ((uint16)data[3] << 8)));
        block->c11 = castUint16ToInt16((uint16)((uint16)data[4] | ((uint16)data[5] << 8)));
        block->d11 = castUint16ToInt16((uint16)((uint16)data[6] | ((uint16)data[7] << 8)));
        block->sfra11 = data[8];
        block->sfrb11 = data[9];
        block->sfrc11 = data[10];
        block->sfrd11 = data[11];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TDSYSTEMO1_FIP16_ISLINKED)
void* TDSystemO1_FiP16_GetAddress(TDSYSTEMO1_FIP16* block, uint16 elementId)
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
