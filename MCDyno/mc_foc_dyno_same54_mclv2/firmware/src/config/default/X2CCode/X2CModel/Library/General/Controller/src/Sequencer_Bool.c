/*
 * Copyright (c) 2014, Linz Center of Mechatronics GmbH (LCM), web: www.lcm.at
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
/* Description: Generation of time delayed (enable) sequence. */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "Sequencer_Bool.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(Sequencer_Bool_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define START       (*block->Start)

/* Outputs */
#define OUT1        (block->Out1)
#define OUT2        (block->Out2)
#define OUT3        (block->Out3)
#define OUT4        (block->Out4)

/* Parameters */
#define DELAY1      (block->delay1)
#define DELAY2      (block->delay2)
#define DELAY3      (block->delay3)
#define DELAY4      (block->delay4)

/* Variables */
#define CNT         (block->cnt)
#define START_OLD   (block->start_old)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void Sequencer_Bool_Update(SEQUENCER_BOOL *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    bool out1 = 0;
    bool out2 = 0;
    bool out3 = 0;
    bool out4 = 0;

    if (START) /* start signal */
    {
        /* check for rising flank */
        if (!START_OLD) /* rising edge of start signal occurred */
        {
            /* reset timer value */
            CNT  = 0;
        }

        /* compare delay values with timer value */
        if (CNT >= DELAY1)
        {
            out1 = true;
        }
        if (CNT >= DELAY2)
        {
            out2 = true;
        }
        if (CNT >= DELAY3)
        {
            out3 = true;
        }
        if (CNT >= DELAY4)
        {
            out4 = true;
        }

        /* check for timer overflow */
        if (CNT < UINT16_MAX)
        {
            CNT++;
        }
        else
        {
            CNT = UINT16_MAX;
        }
    }

    /* assign outputs */
    OUT1 = out1;
    OUT2 = out2;
    OUT3 = out3;
    OUT4 = out4;

    /* save start signal for next cycle */
    START_OLD = START;
/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void Sequencer_Bool_Init(SEQUENCER_BOOL *block)
{
    block->ID = SEQUENCER_BOOL_ID;
    block->Out1 = false;
    block->Out2 = false;
    block->Out3 = false;
    block->Out4 = false;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    CNT = 0;
    START_OLD = false;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sequencer_Bool_Load(const SEQUENCER_BOOL *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 8U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)(block->delay1 & 0xFFU);
        data[1] = (uint8)((block->delay1 >> 8) & 0xFFU);
        data[2] = (uint8)(block->delay2 & 0xFFU);
        data[3] = (uint8)((block->delay2 >> 8) & 0xFFU);
        data[4] = (uint8)(block->delay3 & 0xFFU);
        data[5] = (uint8)((block->delay3 >> 8) & 0xFFU);
        data[6] = (uint8)(block->delay4 & 0xFFU);
        data[7] = (uint8)((block->delay4 >> 8) & 0xFFU);
        *dataLength = 8U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 Sequencer_Bool_Save(SEQUENCER_BOOL *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 8U)
    {
        error = 1U;
    }
    else
    {
        block->delay1 = (uint16)((uint16)data[0] | ((uint16)data[1] << 8));
        block->delay2 = (uint16)((uint16)data[2] | ((uint16)data[3] << 8));
        block->delay3 = (uint16)((uint16)data[4] | ((uint16)data[5] << 8));
        block->delay4 = (uint16)((uint16)data[6] | ((uint16)data[7] << 8));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(SEQUENCER_BOOL_ISLINKED)
void* Sequencer_Bool_GetAddress(SEQUENCER_BOOL* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->Start;
            break;
        case 2:
            addr = &block->Out1;
            break;
        case 3:
            addr = &block->Out2;
            break;
        case 4:
            addr = &block->Out3;
            break;
        case 5:
            addr = &block->Out4;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
