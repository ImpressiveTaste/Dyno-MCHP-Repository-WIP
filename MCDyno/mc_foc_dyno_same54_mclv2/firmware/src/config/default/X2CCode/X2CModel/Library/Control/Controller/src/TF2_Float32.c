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
#include "TF2_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(TF2_Float32_Update, "ramfuncs")
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
void TF2_Float32_Update(TF2_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float32 temp, temp1;

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
void TF2_Float32_Init(TF2_FLOAT32 *block)
{
    block->ID = TF2_FLOAT32_ID;
    block->Out = 0.0f;
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
uint8 TF2_Float32_Load(const TF2_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 20U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        (void) memcpy((void*)&tmp32, (const void*)&block->b0, sizeof(uint32));
        data[0] = (uint8)(tmp32 & 0xFFU);
        data[1] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[2] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[3] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->b1, sizeof(uint32));
        data[4] = (uint8)(tmp32 & 0xFFU);
        data[5] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[6] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[7] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->b2, sizeof(uint32));
        data[8] = (uint8)(tmp32 & 0xFFU);
        data[9] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[10] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[11] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->a0, sizeof(uint32));
        data[12] = (uint8)(tmp32 & 0xFFU);
        data[13] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[14] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[15] = (uint8)((tmp32 >> 24) & 0xFFU);
        (void) memcpy((void*)&tmp32, (const void*)&block->a1, sizeof(uint32));
        data[16] = (uint8)(tmp32 & 0xFFU);
        data[17] = (uint8)((tmp32 >> 8) & 0xFFU);
        data[18] = (uint8)((tmp32 >> 16) & 0xFFU);
        data[19] = (uint8)((tmp32 >> 24) & 0xFFU);
        *dataLength = 20U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 TF2_Float32_Save(TF2_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 20U)
    {
        error = 1U;
    }
    else
    {
        uint32 tmp32;
        tmp32 = (uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24));
        (void) memcpy((void*)&block->b0, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24));
        (void) memcpy((void*)&block->b1, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[8] | ((uint32)data[9] << 8) | ((uint32)data[10] << 16) | ((uint32)data[11] << 24));
        (void) memcpy((void*)&block->b2, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[12] | ((uint32)data[13] << 8) | ((uint32)data[14] << 16) | ((uint32)data[15] << 24));
        (void) memcpy((void*)&block->a0, (const void*)&tmp32, sizeof(float32));
        tmp32 = (uint32)((uint32)data[16] | ((uint32)data[17] << 8) | ((uint32)data[18] << 16) | ((uint32)data[19] << 24));
        (void) memcpy((void*)&block->a1, (const void*)&tmp32, sizeof(float32));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(TF2_FLOAT32_ISLINKED)
void* TF2_Float32_GetAddress(TF2_FLOAT32* block, uint16 elementId)
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
