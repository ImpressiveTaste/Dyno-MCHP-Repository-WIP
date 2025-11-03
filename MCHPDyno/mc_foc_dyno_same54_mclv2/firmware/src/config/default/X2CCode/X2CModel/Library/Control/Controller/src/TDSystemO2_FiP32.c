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
/**     Description: 2nd Order Time Discrete System with 2 in- and outputs.                                          **/
/**                  Calculation:                                                                                    **/
/**                     x(k+1) = A*x(k) + B*u(k)                                                                     **/
/**                     y(k)   =   x(k)                                                                              **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "TDSystemO2_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TDSystemO2_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs*/
#define IN1     (*block->In1)
#define IN2     (*block->In2)

/* Outputs*/
#define OUT1    (block->Out1)
#define OUT2    (block->Out2)

/* Matrices */
#define A11     (block->a11)
#define A12     (block->a12)
#define A21     (block->a21)
#define A22     (block->a22)
#define B11     (block->b11)
#define B12     (block->b12)
#define B21     (block->b21)
#define B22     (block->b22)

/* Shift factor of matrices */
#define SFRA11  (block->sfra11)
#define SFRA12  (block->sfra12)
#define SFRA21  (block->sfra21)
#define SFRA22  (block->sfra22)
#define SFRB11  (block->sfrb11)
#define SFRB12  (block->sfrb12)
#define SFRB21  (block->sfrb21)
#define SFRB22  (block->sfrb22)

/* States */
#define X1      (block->x1)
#define X2      (block->x2)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void TDSystemO2_FiP32_Update(TDSYSTEMO2_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 temp;
    int32 state1;


    /* Calculation of output (= old state) */
    OUT1 = X1;
    OUT2 = X2;

    /* Calculation of new state1 */
    /* RULECHECKER_comment(1:0, 4:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  =  ((int64)A11 * (int64)X1)  >> SFRA11;   /* x1 = a11*x1 */
    temp += (((int64)A12 * (int64)X2)  >> SFRA12);  /* x1 = a11*x1 + a12*x2 */
    temp += (((int64)B11 * (int64)IN1) >> SFRB11);  /* x1 = a11*x1 + a12*x2 + b11*u1 */
    temp += (((int64)B12 * (int64)IN2) >> SFRB12);  /* x1 = a11*x1 + a12*x2 + b11*u1 + b12*u2 */

    /* limit output to 32Bit range Previous: LIMIT(temp, INT32_MAX);*/
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

    state1 = (int32)temp;

    /* Calculation of new state2 */
    /* RULECHECKER_comment(1:0, 4:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  =  ((int64)A21 * (int64)X1)  >> SFRA21;   /* x2 = a21*x1 */
    temp += (((int64)A22 * (int64)X2)  >> SFRA22);  /* x2 = a21*x1 + a22*x2 */
    temp += (((int64)B21 * (int64)IN1) >> SFRB21);  /* x2 = a21*x1 + a22*x2 + b21*u1 */
    temp += (((int64)B22 * (int64)IN2) >> SFRB22);  /* x2 = a21*x1 + a22*x2 + b21*u1 + b22*u2 */

    /* limit output to 32Bit range Previous: LIMIT(temp, INT32_MAX);*/
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

    /* assign states */
    X1 = (int32)state1;
    X2 = (int32)temp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void TDSystemO2_FiP32_Init(TDSYSTEMO2_FIP32 *block)
{
    block->ID = TDSYSTEMO2_FIP32_ID;
    block->Out1 = 0;
    block->Out2 = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* reset states */
     X1 = 0;
     X2 = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO2_FiP32_Load(const TDSYSTEMO2_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 40U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint32)block->a11 & 0xFFU);
        data[1] = (uint8)(((uint32)block->a11 >> 8) & 0xFFU);
        data[2] = (uint8)(((uint32)block->a11 >> 16) & 0xFFU);
        data[3] = (uint8)(((uint32)block->a11 >> 24) & 0xFFU);
        data[4] = (uint8)((uint32)block->a12 & 0xFFU);
        data[5] = (uint8)(((uint32)block->a12 >> 8) & 0xFFU);
        data[6] = (uint8)(((uint32)block->a12 >> 16) & 0xFFU);
        data[7] = (uint8)(((uint32)block->a12 >> 24) & 0xFFU);
        data[8] = (uint8)((uint32)block->a21 & 0xFFU);
        data[9] = (uint8)(((uint32)block->a21 >> 8) & 0xFFU);
        data[10] = (uint8)(((uint32)block->a21 >> 16) & 0xFFU);
        data[11] = (uint8)(((uint32)block->a21 >> 24) & 0xFFU);
        data[12] = (uint8)((uint32)block->a22 & 0xFFU);
        data[13] = (uint8)(((uint32)block->a22 >> 8) & 0xFFU);
        data[14] = (uint8)(((uint32)block->a22 >> 16) & 0xFFU);
        data[15] = (uint8)(((uint32)block->a22 >> 24) & 0xFFU);
        data[16] = (uint8)((uint32)block->b11 & 0xFFU);
        data[17] = (uint8)(((uint32)block->b11 >> 8) & 0xFFU);
        data[18] = (uint8)(((uint32)block->b11 >> 16) & 0xFFU);
        data[19] = (uint8)(((uint32)block->b11 >> 24) & 0xFFU);
        data[20] = (uint8)((uint32)block->b12 & 0xFFU);
        data[21] = (uint8)(((uint32)block->b12 >> 8) & 0xFFU);
        data[22] = (uint8)(((uint32)block->b12 >> 16) & 0xFFU);
        data[23] = (uint8)(((uint32)block->b12 >> 24) & 0xFFU);
        data[24] = (uint8)((uint32)block->b21 & 0xFFU);
        data[25] = (uint8)(((uint32)block->b21 >> 8) & 0xFFU);
        data[26] = (uint8)(((uint32)block->b21 >> 16) & 0xFFU);
        data[27] = (uint8)(((uint32)block->b21 >> 24) & 0xFFU);
        data[28] = (uint8)((uint32)block->b22 & 0xFFU);
        data[29] = (uint8)(((uint32)block->b22 >> 8) & 0xFFU);
        data[30] = (uint8)(((uint32)block->b22 >> 16) & 0xFFU);
        data[31] = (uint8)(((uint32)block->b22 >> 24) & 0xFFU);
        data[32] = (uint8)(block->sfra11 & 0xFFU);
        data[33] = (uint8)(block->sfra12 & 0xFFU);
        data[34] = (uint8)(block->sfra21 & 0xFFU);
        data[35] = (uint8)(block->sfra22 & 0xFFU);
        data[36] = (uint8)(block->sfrb11 & 0xFFU);
        data[37] = (uint8)(block->sfrb12 & 0xFFU);
        data[38] = (uint8)(block->sfrb21 & 0xFFU);
        data[39] = (uint8)(block->sfrb22 & 0xFFU);
        *dataLength = 40U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO2_FiP32_Save(TDSYSTEMO2_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 40U)
    {
        error = 1U;
    }
    else
    {
        block->a11 = castUint32ToInt32((uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24)));
        block->a12 = castUint32ToInt32((uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24)));
        block->a21 = castUint32ToInt32((uint32)((uint32)data[8] | ((uint32)data[9] << 8) | ((uint32)data[10] << 16) | ((uint32)data[11] << 24)));
        block->a22 = castUint32ToInt32((uint32)((uint32)data[12] | ((uint32)data[13] << 8) | ((uint32)data[14] << 16) | ((uint32)data[15] << 24)));
        block->b11 = castUint32ToInt32((uint32)((uint32)data[16] | ((uint32)data[17] << 8) | ((uint32)data[18] << 16) | ((uint32)data[19] << 24)));
        block->b12 = castUint32ToInt32((uint32)((uint32)data[20] | ((uint32)data[21] << 8) | ((uint32)data[22] << 16) | ((uint32)data[23] << 24)));
        block->b21 = castUint32ToInt32((uint32)((uint32)data[24] | ((uint32)data[25] << 8) | ((uint32)data[26] << 16) | ((uint32)data[27] << 24)));
        block->b22 = castUint32ToInt32((uint32)((uint32)data[28] | ((uint32)data[29] << 8) | ((uint32)data[30] << 16) | ((uint32)data[31] << 24)));
        block->sfra11 = data[32];
        block->sfra12 = data[33];
        block->sfra21 = data[34];
        block->sfra22 = data[35];
        block->sfrb11 = data[36];
        block->sfrb12 = data[37];
        block->sfrb21 = data[38];
        block->sfrb22 = data[39];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TDSYSTEMO2_FIP32_ISLINKED)
void* TDSystemO2_FiP32_GetAddress(TDSYSTEMO2_FIP32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In1;
            break;
        case 2:
            addr = block->In2;
            break;
        case 3:
            addr = &block->Out1;
            break;
        case 4:
            addr = &block->Out2;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
