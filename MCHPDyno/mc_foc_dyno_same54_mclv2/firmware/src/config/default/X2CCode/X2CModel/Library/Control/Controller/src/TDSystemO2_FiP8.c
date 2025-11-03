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
#include "TDSystemO2_FiP8.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TDSystemO2_FiP8_Update, "ramfuncs")
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
void TDSystemO2_FiP8_Update(TDSYSTEMO2_FIP8 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int16 temp;
    int8 state1;

    /* Calculation of output (= old state) */
    OUT1 = X1;
    OUT2 = X2;

    /* Calculation of new state1 */
    /* RULECHECKER_comment(1:0, 4:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  =  ((int16)A11 * (int16)X1)  >> SFRA11;   /* x1 = a11*x1 */
    temp += (((int16)A12 * (int16)X2)  >> SFRA12);  /* x1 = a11*x1 + a12*x2 */
    temp += (((int16)B11 * (int16)IN1) >> SFRB11);  /* x1 = a11*x1 + a12*x2 + b11*u1 */
    temp += (((int16)B12 * (int16)IN2) >> SFRB12);  /* x1 = a11*x1 + a12*x2 + b11*u1 + b12*u2 */

    /* limit output to 8Bit range Previous: LIMIT(temp, INT8_MAX);*/
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

    state1 = (int8)temp;

    /* Calculation of new state2 */
    /* RULECHECKER_comment(1:0, 4:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
    temp  =  ((int16)A21 * (int16)X1)  >> SFRA21;   /* x2 = a21*x1 */
    temp += (((int16)A22 * (int16)X2)  >> SFRA22);  /* x2 = a21*x1 + a22*x2 */
    temp += (((int16)B21 * (int16)IN1) >> SFRB21);  /* x2 = a21*x1 + a22*x2 + b21*u1 */
    temp += (((int16)B22 * (int16)IN2) >> SFRB22);  /* x2 = a21*x1 + a22*x2 + b21*u1 + b22*u2 */

    /* limit output to 8Bit range Previous: LIMIT(temp, INT8_MAX);*/
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

    /* assign states */
    X1 = (int8)state1;
    X2 = (int8)temp;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void TDSystemO2_FiP8_Init(TDSYSTEMO2_FIP8 *block)
{
    block->ID = TDSYSTEMO2_FIP8_ID;
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
uint8 TDSystemO2_FiP8_Load(const TDSYSTEMO2_FIP8 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 16U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint8)block->a11 & 0xFFU);
        data[1] = (uint8)((uint8)block->a12 & 0xFFU);
        data[2] = (uint8)((uint8)block->a21 & 0xFFU);
        data[3] = (uint8)((uint8)block->a22 & 0xFFU);
        data[4] = (uint8)((uint8)block->b11 & 0xFFU);
        data[5] = (uint8)((uint8)block->b12 & 0xFFU);
        data[6] = (uint8)((uint8)block->b21 & 0xFFU);
        data[7] = (uint8)((uint8)block->b22 & 0xFFU);
        data[8] = (uint8)(block->sfra11 & 0xFFU);
        data[9] = (uint8)(block->sfra12 & 0xFFU);
        data[10] = (uint8)(block->sfra21 & 0xFFU);
        data[11] = (uint8)(block->sfra22 & 0xFFU);
        data[12] = (uint8)(block->sfrb11 & 0xFFU);
        data[13] = (uint8)(block->sfrb12 & 0xFFU);
        data[14] = (uint8)(block->sfrb21 & 0xFFU);
        data[15] = (uint8)(block->sfrb22 & 0xFFU);
        *dataLength = 16U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TDSystemO2_FiP8_Save(TDSYSTEMO2_FIP8 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 16U)
    {
        error = 1U;
    }
    else
    {
        block->a11 = castUint8ToInt8(data[0]);
        block->a12 = castUint8ToInt8(data[1]);
        block->a21 = castUint8ToInt8(data[2]);
        block->a22 = castUint8ToInt8(data[3]);
        block->b11 = castUint8ToInt8(data[4]);
        block->b12 = castUint8ToInt8(data[5]);
        block->b21 = castUint8ToInt8(data[6]);
        block->b22 = castUint8ToInt8(data[7]);
        block->sfra11 = data[8];
        block->sfra12 = data[9];
        block->sfra21 = data[10];
        block->sfra22 = data[11];
        block->sfrb11 = data[12];
        block->sfrb12 = data[13];
        block->sfrb21 = data[14];
        block->sfrb22 = data[15];
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TDSYSTEMO2_FIP8_ISLINKED)
void* TDSystemO2_FiP8_GetAddress(TDSYSTEMO2_FIP8* block, uint16 elementId)
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
