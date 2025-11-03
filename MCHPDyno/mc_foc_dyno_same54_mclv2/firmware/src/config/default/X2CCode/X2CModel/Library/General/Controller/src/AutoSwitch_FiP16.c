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
/*     Description:  Switch between In1 and In3 dependent on Switch signal:   */
/*              Switch signal rising:  Switch > Threshold up --> Out = In1    */
/*              Switch signal falling: Switch < Threshold down --> Out = In3  */
/*                                                                            */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "AutoSwitch_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(AutoSwitch_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN1                 (*block->In1)
#define SWITCH              (*block->Switch)
#define IN3                 (*block->In3)

/* Outputs */
#define OUT                 (block->Out)

/* Parameter */
#define THRESH_UP           (block->Thresh_up)
#define THRESH_DOWN         (block->Thresh_down)

/* Variables */
#if !defined(X2C_USE_UNION_FOR_POINTER)
#define STATUS              (block->Status)
#else
#define STATUS              (block->Status.ptr)
#endif

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void AutoSwitch_FiP16_Update(AUTOSWITCH_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    if (SWITCH >= THRESH_UP)
    {
        /* positive crossing of upper threshold level -> change hysteresis state */
        STATUS = &IN1;
    }
    else if (SWITCH < THRESH_DOWN)
    {
        /* negative crossing of lower threshold level -> change hysteresis state */
        STATUS = &IN3;
    }
    else
    {
        /* else: no change in hysteresis state */
    }


    /* set output according to hysteresis status */
    OUT = *STATUS;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void AutoSwitch_FiP16_Init(AUTOSWITCH_FIP16 *block)
{
    block->ID = AUTOSWITCH_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
     STATUS = &IN3;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 AutoSwitch_FiP16_Load(const AUTOSWITCH_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0U;
    if (maxSize < 4U)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((uint16)block->Thresh_up & 0xFFU);
        data[1] = (uint8)(((uint16)block->Thresh_up >> 8) & 0xFFU);
        data[2] = (uint8)((uint16)block->Thresh_down & 0xFFU);
        data[3] = (uint8)(((uint16)block->Thresh_down >> 8) & 0xFFU);
        *dataLength = 4U;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 AutoSwitch_FiP16_Save(AUTOSWITCH_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 4U)
    {
        error = 1U;
    }
    else
    {
        block->Thresh_up = castUint16ToInt16((uint16)((uint16)data[0] | ((uint16)data[1] << 8)));
        block->Thresh_down = castUint16ToInt16((uint16)((uint16)data[2] | ((uint16)data[3] << 8)));
        error = 0U;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(AUTOSWITCH_FIP16_ISLINKED)
void* AutoSwitch_FiP16_GetAddress(AUTOSWITCH_FIP16* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In1;
            break;
        case 2:
            addr = block->Switch;
            break;
        case 3:
            addr = block->In3;
            break;
        case 4:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
