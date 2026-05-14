/*
 * Copyright (c) 2013, Linz Center of Mechatronics GmbH (LCM) http://www.lcm.at/
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
 * This file is part of X2C. http://x2c.lcm.at/
 * $LastChangedRevision: 2800 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/*     Description:    P Controller with
 *                         - Enable Input
 *                         - Output Limitation
 *                     Calculation ZOH:
 *
 *                         y = Kp * u
 *
 *                     -> y(k) = b1.u(k)
 */
/* USERCODE-END:Description                                                                                           */
#include "PLimit_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(PLimit_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*pTPLimit_FiP32->In)           /* Q31 */
#define ENABLE          (*pTPLimit_FiP32->Enable)       /* Q0 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*pTPLimit_FiP32->max)          /* Q31 */
    #define LIMIT_DOWN  (*pTPLimit_FiP32->min)          /* Q31 */
#endif

/* Outputs */
#define OUT             (pTPLimit_FiP32->Out)           /* Q31 */

/* Parameter */
#define B1              (pTPLimit_FiP32->b1)            /* Qy */
#define SFR1            (pTPLimit_FiP32->sfrb1)         /* y */

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void PLimit_FiP32_Update(PLIMIT_FIP32 *pTPLimit_FiP32)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 y;

    if (ENABLE) /* Block enabled */
    {
        /* Proportional term */
        y = ((int64)B1 * (int64)IN) >> SFR1;    /* Q31 */

        /* Output limitation */
        if (y > LIMIT_UP)           /* output is beyond upper limit */
        {
            /* output limitation to upper boundary */
            y = LIMIT_UP;
        }
        else if (y < LIMIT_DOWN)    /* output is beyond lower limit */
        {
            /* output limitation to lower boundary */
            y = LIMIT_DOWN;
        }
        /* else: output is within limits */

        /* Assign output */
        OUT = (int32)y;
    }
    else                /* Block disabled */
    {
        OUT = 0;    /* reset output */
    }

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void PLimit_FiP32_Init(PLIMIT_FIP32 *pTPLimit_FiP32)
{
    pTPLimit_FiP32->ID = PLIMIT_FIP32_ID;
    pTPLimit_FiP32->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PLimit_FiP32_Load(const PLIMIT_FIP32 *pTPLimit_FiP32, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)5 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)(pTPLimit_FiP32->b1 & 0x000000FF);
        data[1] = (uint8)((pTPLimit_FiP32->b1 >> 8) & 0x000000FF);
        data[2] = (uint8)((pTPLimit_FiP32->b1 >> 16) & 0x000000FF);
        data[3] = (uint8)((pTPLimit_FiP32->b1 >> 24) & 0x000000FF);
        data[4] = (uint8)pTPLimit_FiP32->sfrb1;
        *dataLength = (uint16)5;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PLimit_FiP32_Save(PLIMIT_FIP32 *pTPLimit_FiP32, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != (uint16)5)
    {
        error = (uint8)1;
    }
    else
    {
        pTPLimit_FiP32->b1 = UINT32_TO_INT32((uint32)data[0] + \
            ((uint32)data[1] << 8) + ((uint32)data[2] << 16) + \
            ((uint32)data[3] << 24));
        pTPLimit_FiP32->sfrb1 = UINT8_TO_INT8((uint8)data[4]);
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(PLIMIT_FIP32_ISLINKED)
void* PLimit_FiP32_GetAddress(const PLIMIT_FIP32* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = (void*)block->In;
            break;
        case 2:
            addr = (void*)block->max;
            break;
        case 3:
            addr = (void*)block->min;
            break;
        case 4:
            addr = (void*)block->Enable;
            break;
        case 5:
            addr = (void*)&block->Out;
            break;
        default:
            addr = (void*)0;
            break;
    }
    return (addr);
}
#endif
