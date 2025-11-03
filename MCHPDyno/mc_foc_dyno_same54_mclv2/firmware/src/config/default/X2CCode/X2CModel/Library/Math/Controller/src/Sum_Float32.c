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
/**     Description:    Sum of all inputs.                                                                           **/
/**                                                                                                                  **/
/**                     Sign bit field:                                                                              **/
/**                         00->0 ... Input will be ignored.                                                         **/
/**                         01->+ ... Input will be added to result.                                                 **/
/**                         10->- ... Input will be subtracted from result.                                          **/
/**                                                                                                                  **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "Sum_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(Sum_Float32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN1             (*block->In1)
#define IN2             (*block->In2)
#define IN3             (*block->In3)
#define IN4             (*block->In4)
#define IN5             (*block->In5)
#define IN6             (*block->In6)
#define IN7             (*block->In7)
#define IN8             (*block->In8)

/* Outputs */
#define OUT             (block->Out)

/* Parameter */
#define SIGN            (block->sign)       /* Bitfield */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void Sum_Float32_Update(SUM_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    float32 sum  =  (float32)0;
    uint16  mask = (uint16)0;

    /* Input 1 */
    mask = (uint16)SIGN & (uint16)0x0003;
    if(mask == (uint16)1)       /* plus */
    {
        sum = IN1;
    }
    else
    {
        if (mask == (uint16)2)  /* minus */
        {
            sum = -IN1;
        }
    }

    /* Input 2 */
    mask = (((uint16)SIGN) >> 2) & (uint16)0x0003;
    if(mask == (uint16)1)       /* plus */
    {
        sum += IN2;
    }
    else
    {
        if (mask == (uint16)2)  /* minus */
        {
            sum -= IN2;
        }
    }

    /* Input 3 */
    mask = (((uint16)SIGN) >> 4) & (uint16)0x0003;
    if(mask == (uint16)1)       /* plus */
    {
        sum += IN3;
    }
    else
    {
        if (mask == (uint16)2)  /* minus */
        {
            sum -= IN3;
        }
    }

    /* Input 4 */
    mask = (((uint16)SIGN) >> 6) & (uint16)0x0003;
    if(mask == (uint16)1)       /* plus */
    {
        sum += IN4;
    }
    else
    {
        if (mask == (uint16)2)  /* minus */
        {
            sum -= IN4;
        }
    }

    /* Input 5 */
    mask = (((uint16)SIGN) >> 8) & (uint16)0x0003;
    if(mask == (uint16)1)       /* plus */
    {
        sum += IN5;
    }
    else
    {
        if (mask == (uint16)2)  /* minus */
        {
            sum -= IN5;
        }
    }

    /* Input 6 */
    mask = (((uint16)SIGN) >> 10) & (uint16)0x0003;
    if(mask == (uint16)1)       /* plus */
    {
        sum += IN6;
    }
    else
    {
        if (mask == (uint16)2)  /* minus */
        {
            sum -= IN6;
        }
    }

    /* Input 7 */
    mask = (((uint16)SIGN) >> 12) & (uint16)0x0003;
    if(mask == (uint16)1)       /* plus */
    {
        sum += IN7;
    }
    else
    {
        if (mask == (uint16)2)  /* minus */
        {
            sum -= IN7;
        }
    }

    /* Input 8 */
    mask = (((uint16)SIGN) >> 14) & (uint16)0x0003;
    if(mask == (uint16)1)       /* plus */
    {
        sum += IN8;
    }
    else
    {
        if (mask == (uint16)2)  /* minus */
        {
            sum -= IN8;
        }
    }

    /* Output */
    OUT = sum;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void Sum_Float32_Init(SUM_FLOAT32 *block)
{
    block->ID = SUM_FLOAT32_ID;
    block->Out = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sum_Float32_Load(const SUM_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 2U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(block->sign & 0xFFU);
        data[1] = (uint8)((block->sign >> 8) & 0xFFU);
        *dataLength = 2U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sum_Float32_Save(SUM_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 2U)
    {
        error = 1U;
    }
    else
    {
        block->sign = (uint16)((uint16)data[0] | ((uint16)data[1] << 8));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SUM_FLOAT32_ISLINKED)
void* Sum_Float32_GetAddress(SUM_FLOAT32* block, uint16 elementId)
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
            addr = block->In3;
            break;
        case 4:
            addr = block->In4;
            break;
        case 5:
            addr = block->In5;
            break;
        case 6:
            addr = block->In6;
            break;
        case 7:
            addr = block->In7;
            break;
        case 8:
            addr = block->In8;
            break;
        case 9:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
