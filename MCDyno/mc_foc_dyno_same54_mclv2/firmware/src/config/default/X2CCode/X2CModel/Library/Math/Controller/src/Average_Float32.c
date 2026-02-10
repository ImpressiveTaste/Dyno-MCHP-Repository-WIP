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
/**     Description:    Calculation of moving average value over n numbers.  **/
/**                                                                          **/
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "Average_Float32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(Average_Float32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)

/* Outputs */
#define OUT             (block->Out)

/* Parameter */
#define NUMBER          (block->n)

/* Variables */
#define SUM             (block->sum)
#define CNT             (block->count)
#if !defined(X2C_USE_UNION_FOR_POINTER)
#define AVG             (block->avg)
#else
#define AVG             (block->avg.ptr)
#endif

/* Constants */
#define MAX_NUMBER      256     /* max size of average window */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void Average_Float32_Update(AVERAGE_FLOAT32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    /* Adjust array */
    SUM     -= AVG[CNT];        /* remove last entry from sum */
    SUM     += IN;              /* add new value to sum */
    AVG[CNT] = IN;              /* put new value into buffer */

    /* Cyclic index counter */
    CNT++;
    if (CNT >= NUMBER)
    {
        CNT = 0;
    }

    /* Assign output */
    OUT = SUM / NUMBER;         /* calculate average value */

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void Average_Float32_Init(AVERAGE_FLOAT32 *block)
{
    block->ID = AVERAGE_FLOAT32_ID;
    block->Out = 0.0f;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     /* Reset sum */
     SUM = 0;

     /* Reset array */
     for (CNT=0; CNT<MAX_NUMBER; CNT++)
     {
         AVG[CNT] = 0;
     }

     /* Reset counter */
     CNT = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Average_Float32_Load(const AVERAGE_FLOAT32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 2U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(block->n & 0xFFU);
        data[1] = (uint8)((block->n >> 8) & 0xFFU);
        *dataLength = 2U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Average_Float32_Save(AVERAGE_FLOAT32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 2U)
    {
        error = 1U;
    }
    else
    {
        block->n = (uint16)((uint16)data[0] | ((uint16)data[1] << 8));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
     /* Reset sum */
     SUM = 0;

     /* Reset array */
     for (CNT=0; CNT<MAX_NUMBER; CNT++)
     {
         AVG[CNT] = 0;
     }

     /* Reset counter */
     CNT = 0;

/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(AVERAGE_FLOAT32_ISLINKED)
void* Average_Float32_GetAddress(AVERAGE_FLOAT32* block, uint16 elementId)
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
