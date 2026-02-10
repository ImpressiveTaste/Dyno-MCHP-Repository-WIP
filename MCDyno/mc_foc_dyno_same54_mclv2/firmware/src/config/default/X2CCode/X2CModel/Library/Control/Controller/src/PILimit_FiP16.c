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
 * $LastChangedRevision: 3575 $
 */
/* USERCODE-BEGIN:Description                                                                                         */
/*     Description:  PI Controller with
 *                          - Enable Input
 *                          - Anti Windup
 *                          - Initial Condition Input
 *                          - Output Limitation
 *                      Calculation ZOH:
 *                                        1
 *                      y = (Kp + Ki*Ts -----)*u
 *                                      z - 1
 *
 *                      > y(k) = b1.u(k) + b0.u(k-1) + y(k-1)
 */
/* USERCODE-END:Description                                                                                           */
#include <stddef.h>
#include "PILimit_FiP16.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(PILimit_FiP16_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)          /* Q15 */
#define ENABLE          (*block->Enable)      /* Q0 */
#define INIT            (*block->Init)        /* Q15 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*block->max)         /* Q15 */
    #define LIMIT_DOWN  (*block->min)         /* Q15 */
#endif

/* Outputs */
#define OUT             (block->Out)          /* Q15 */

/* Parameter */
#define B0              (block->b0)           /* Qx */
#define B1              (block->b1)           /* Qy */
#define SFR0            (block->sfrb0)        /* x */
#define SFR1            (block->sfrb1)        /* y */

/* Variables */
#define I_OLD           (block->i_old)        /* Q30 */
#define ENABLE_OLD      (block->enable_old)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void PILimit_FiP16_Update(PILIMIT_FIP16 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int32 y, yp;
    bool clamp;             /* Flag used for anti windup strategy clamping */


    if (ENABLE) /* Block enabled */
    {
        /* Proportional term */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
#ifdef __XC16__
        yp = __builtin_mulss(B1, IN) >> SFR1;   /* Q15 */
#else
        yp = ((int32)B1 * (int32)IN) >> SFR1;   /* Q15 */
#endif

        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Check if intial output is out of limits */
            if (INIT > LIMIT_UP)
            {
                y = (int32)LIMIT_UP;
            }
            else if (INIT < LIMIT_DOWN)
            {
                y = (int32)LIMIT_DOWN;
            }
            else
            {
                y = (int32)INIT;
            }

            /* Initialization of integral part so that first y = INIT */
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0004", true_no_defect) */
            I_OLD = (y - yp) << 15;
        }

        /* Sum of proportional and integral term */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
        y = yp + (I_OLD>>15);                       /* Q15 */

        /* Output limitation and anti windup clamping */
        if (y > LIMIT_UP)
        {
            /* Output beyond upper limit */
            y = LIMIT_UP;

            /* Set integrator clamping condition: 
            clamp if integration of error-input would cause more saturation */
            clamp = (IN > 0);
        }
        else if (y < LIMIT_DOWN)
        {
            /* Output beyond lower limit */
            y = LIMIT_DOWN;

            /* Set integrator clamping condition: 
            clamp if integration of error-input would cause more saturation */
            clamp = (IN < 0);
        }
        /* else: no output limitation */
        else
        {
            /* Do not clamp the integrator */
            clamp = false;
        }

        /* Conditional integration = check clamping condition */
        if (!clamp)
        {
            /* Calculation of integral part */
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0003", true_no_defect) */
#ifdef __XC16__
            I_OLD += (__builtin_mulss(B0, IN) << (15-SFR0));    /* Q30 */
#else
            I_OLD += (((int32)B0 * (int32)IN) << (15-SFR0));    /* Q30 */
#endif
        }

        /* Assign output */
        OUT = (int16)y;
    }
    else            /* Block disabled */
    {
        OUT = 0;    /* Reset output */
    }
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void PILimit_FiP16_Init(PILIMIT_FIP16 *block)
{
    block->ID = PILIMIT_FIP16_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* preset old values */
    I_OLD = 0;
    ENABLE_OLD = false;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PILimit_FiP16_Load(const PILIMIT_FIP16 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0u;
    if (maxSize < 6u)
    {
        error = 1u;
    }
    else
    {
        data[0] = (uint8)((uint16)block->b0 & 0xFFu);
        data[1] = (uint8)(((uint16)block->b0 >> 8) & 0xFFu);
        data[2] = (uint8)((uint16)block->b1 & 0xFFu);
        data[3] = (uint8)(((uint16)block->b1 >> 8) & 0xFFu);
        data[4] = (uint8)(block->sfrb0 & 0xFFu);
        data[5] = (uint8)(block->sfrb1 & 0xFFu);
        *dataLength = 6u;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PILimit_FiP16_Save(PILIMIT_FIP16 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 6u)
    {
        error = 1u;
    }
    else
    {
        block->b0 = castUint16ToInt16((uint16)((uint16)data[0] | ((uint16)data[1] << 8)));
        block->b1 = castUint16ToInt16((uint16)((uint16)data[2] | ((uint16)data[3] << 8)));
        block->sfrb0 = data[4];
        block->sfrb1 = data[5];
        error = 0u;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(PILIMIT_FIP16_ISLINKED)
void* PILimit_FiP16_GetAddress(PILIMIT_FIP16* block, uint16 elementId)
{
    void* addr;
    switch (elementId)
    {
        case 1:
            addr = block->In;
            break;
        case 2:
            addr = block->Init;
            break;
        case 3:
            addr = block->max;
            break;
        case 4:
            addr = block->min;
            break;
        case 5:
            addr = block->Enable;
            break;
        case 6:
            addr = &block->Out;
            break;
        default:
            addr = NULL;
            break;
    }
    return (addr);
}
#endif
