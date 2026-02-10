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
#include "TF2_Float64.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TF2_Float64_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN          (*block->In)

/* Outputs */
#define OUT         (block->Out)

/* Parameter */
#define A0          (block->a0)
#define A1          (block->a1)
#define B0          (block->b0)
#define B1          (block->b1)
#define B2          (block->b2)

/* Variables */
#define IN_OLD      (block->in_old)
#define IN_VERYOLD  (block->in_veryold)
#define OUT_OLD     (block->out_old)
#define OUT_VERYOLD (block->out_veryold)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void TF2_Float64_Update(TF2_FLOAT64 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float64 temp, temp1;

    temp  =  B0 * IN_VERYOLD;   /* y(k) = b0.u(k-2) */
    temp += (B1 * IN_OLD);      /* y(k) = b0.u(k-2) + b1.u(k-1) */
    temp += (B2 * IN);          /* y(k) = b0.u(k-2) + b1.u(k-1) + b2.u(k) */

    temp1  =  A0 * OUT_VERYOLD; /* y1(k) = a0.y(k-2) */
    temp1 += (A1 * OUT_OLD);    /* y1(k) = a0.y(k-2) + a1.y(k-1) */

    temp   = temp - temp1;      /* y(k) = b0.u(k-2) + b1.u(k-1) + b2.u(k) - a0.y(k-2) - a1.y(k-1) */


    /* save values for next update call and assign output */
    OUT         = temp;
    IN_VERYOLD  = IN_OLD;
    IN_OLD      = IN;
    OUT_VERYOLD = OUT_OLD;
    OUT_OLD     = OUT;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void TF2_Float64_Init(TF2_FLOAT64 *block)
{
    block->ID = TF2_FLOAT64_ID;
    block->Out = 0.0;
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
uint8 TF2_Float64_Load(const TF2_FLOAT64 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 40U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        (void) memcpy((void*)&tmp64, (const void*)&block->b0, sizeof(uint64));
        data[0] = (uint8)(tmp64 & 0xFFU);
        data[1] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[4] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[5] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[6] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[7] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->b1, sizeof(uint64));
        data[8] = (uint8)(tmp64 & 0xFFU);
        data[9] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[10] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[11] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[12] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[13] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[14] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[15] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->b2, sizeof(uint64));
        data[16] = (uint8)(tmp64 & 0xFFU);
        data[17] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[18] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[19] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[20] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[21] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[22] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[23] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->a0, sizeof(uint64));
        data[24] = (uint8)(tmp64 & 0xFFU);
        data[25] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[26] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[27] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[28] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[29] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[30] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[31] = (uint8)((tmp64 >> 56) & 0xFFU);
        (void) memcpy((void*)&tmp64, (const void*)&block->a1, sizeof(uint64));
        data[32] = (uint8)(tmp64 & 0xFFU);
        data[33] = (uint8)((tmp64 >> 8) & 0xFFU);
        data[34] = (uint8)((tmp64 >> 16) & 0xFFU);
        data[35] = (uint8)((tmp64 >> 24) & 0xFFU);
        data[36] = (uint8)((tmp64 >> 32) & 0xFFU);
        data[37] = (uint8)((tmp64 >> 40) & 0xFFU);
        data[38] = (uint8)((tmp64 >> 48) & 0xFFU);
        data[39] = (uint8)((tmp64 >> 56) & 0xFFU);
        *dataLength = 40U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TF2_Float64_Save(TF2_FLOAT64 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 40U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        tmp64 = (uint64)((uint64)data[0] | ((uint64)data[1] << 8) | ((uint64)data[2] << 16) | ((uint64)data[3] << 24) | 
            ((uint64)data[4] << 32) | ((uint64)data[5] << 40) | ((uint64)data[6] << 48) | ((uint64)data[7] << 56));
        (void) memcpy((void*)&block->b0, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[8] | ((uint64)data[9] << 8) | ((uint64)data[10] << 16) | ((uint64)data[11] << 24) | 
            ((uint64)data[12] << 32) | ((uint64)data[13] << 40) | ((uint64)data[14] << 48) | ((uint64)data[15] << 56));
        (void) memcpy((void*)&block->b1, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[16] | ((uint64)data[17] << 8) | ((uint64)data[18] << 16) | ((uint64)data[19] << 24) | 
            ((uint64)data[20] << 32) | ((uint64)data[21] << 40) | ((uint64)data[22] << 48) | ((uint64)data[23] << 56));
        (void) memcpy((void*)&block->b2, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[24] | ((uint64)data[25] << 8) | ((uint64)data[26] << 16) | ((uint64)data[27] << 24) | 
            ((uint64)data[28] << 32) | ((uint64)data[29] << 40) | ((uint64)data[30] << 48) | ((uint64)data[31] << 56));
        (void) memcpy((void*)&block->a0, (const void*)&tmp64, sizeof(float64));
        tmp64 = (uint64)((uint64)data[32] | ((uint64)data[33] << 8) | ((uint64)data[34] << 16) | ((uint64)data[35] << 24) | 
            ((uint64)data[36] << 32) | ((uint64)data[37] << 40) | ((uint64)data[38] << 48) | ((uint64)data[39] << 56));
        (void) memcpy((void*)&block->a1, (const void*)&tmp64, sizeof(float64));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TF2_FLOAT64_ISLINKED)
void* TF2_Float64_GetAddress(TF2_FLOAT64* block, uint16 elementId)
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
