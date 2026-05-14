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
 * $LastChangedRevision: 2945 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/*     Description:    I Controller with
 *                         - Enable Input
 *                         - Anti Wind-up
 *                         - Initial Condition Input
 *                         - Output Limitation
 *                     Calculation ZOH:
 *                                        1
 *                         y = ( Ki*Ts* -----  ) * u
 *                                      z - 1
 *
 *                     -> y(k) = b1.u(k) + b0.u(k-1) + y(k-1)
 */
/* USERCODE-END:Description                                                                                           */
#include "ILimit_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(ILimit_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*pTILimit_FiP16->In)       /* Q15 */
#define INIT            (*pTILimit_FiP16->I0)       /* Q15 */
#define ENABLE          (*pTILimit_FiP16->Enable)   /* Q0 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*pTILimit_FiP16->max)      /* Q15 */
    #define LIMIT_DOWN  (*pTILimit_FiP16->min)      /* Q15 */
#endif

/* Outputs */
#define OUT             (pTILimit_FiP16->Out)        /* Q15 */

/* Parameter */
#define B0              (pTILimit_FiP16->b0)         /* Qy */
#define SFR0            (pTILimit_FiP16->sfr)        /* y */

/* Variables */
#define I_OLD           (pTILimit_FiP16->i_old)      /* Q30 */
#define ENABLE_OLD      (pTILimit_FiP16->enable_old)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void ILimit_FiP16_Update(ILIMIT_FIP16 *pTILimit_FiP16)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 y;

    if (ENABLE) /* Block enabled */
    {
        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Initialize integral part */
            I_OLD = ((int32)INIT) << 15;
        }

        /* Calculate output */
        y = I_OLD >> 15;                                    /* Q15 */

        /* Calculate integral part */
#ifdef __XC16__
        I_OLD += ((__builtin_mulss(B0,IN)) << (15-SFR0));   /* Q30 */
#else
        I_OLD += (((int32)B0 * (int32)IN) << (15-SFR0));    /* Q30 */
#endif

        /* Output limitation and anti wind-up */
        if (y > LIMIT_UP)
        {
            /* Output beyond upper limit -> clamp */
            y = LIMIT_UP;

            /* Limitation of integral part (anti wind-up) */
            if (I_OLD > ((int32)LIMIT_UP << 15))
            {
                I_OLD = (int32)LIMIT_UP << 15;
            }
        }
        else if (y < LIMIT_DOWN)
        {
            /* Output limitation to lower boundary */
            y = LIMIT_DOWN;

            /* Limitation of integral part (anti wind-up) */
            if (I_OLD < ((int32)LIMIT_DOWN << 15))
            {
                I_OLD = (int32)LIMIT_DOWN << 15;
            }
        }
        /* else: no output limitation */

        /* Assign output */
        OUT = (int16)y;
    }
    else                /* Block disabled */
    {
        OUT = 0;    /* Reset output */
    }
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void ILimit_FiP16_Init(ILIMIT_FIP16 *pTILimit_FiP16)
{
    pTILimit_FiP16->ID = ILIMIT_FIP16_ID;
    pTILimit_FiP16->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    I_OLD = ((int32)INIT) << 15;
    ENABLE_OLD = false;
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 ILimit_FiP16_Load(const ILIMIT_FIP16 *pTILimit_FiP16, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = (uint8)0;
    if ((uint16)3 > maxSize)
    {
        error = (uint8)1;
    }
    else
    {
        data[0] = (uint8)(pTILimit_FiP16->b0 & 0x00FF);
        data[1] = (uint8)((pTILimit_FiP16->b0 >> 8) & 0x00FF);
        data[2] = (uint8)pTILimit_FiP16->sfr;
        *dataLength = (uint16)3;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 ILimit_FiP16_Save(ILIMIT_FIP16 *pTILimit_FiP16, const uint8 data[], uint16 dataLength)
{
    uint8 error;

    if (dataLength != (uint16)3)
    {
        error = (uint8)1;
    }
    else
    {
        pTILimit_FiP16->b0 = UINT16_TO_INT16((uint16)data[0] + \
            ((uint16)data[1] << 8));
        pTILimit_FiP16->sfr = UINT8_TO_INT8((uint8)data[2]);
        error = (uint8)0;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(ILIMIT_FIP16_ISLINKED)
void* ILimit_FiP16_GetAddress(const ILIMIT_FIP16* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = (void*)block->In;
            break;
        case 2:
            addr = (void*)block->I0;
            break;
        case 3:
            addr = (void*)block->max;
            break;
        case 4:
            addr = (void*)block->min;
            break;
        case 5:
            addr = (void*)block->Enable;
            break;
        case 6:
            addr = (void*)&block->Out;
            break;
        default:
            addr = (void*)0;
            break;
    }
    return (addr);
}
#endif
