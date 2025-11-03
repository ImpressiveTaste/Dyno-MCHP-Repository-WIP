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
#include "PILimit_FiP32.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(PILimit_FiP32_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)          /* Q31 */
#define ENABLE          (*block->Enable)      /* Q0 */
#define INIT            (*block->Init)        /* Q31 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*block->max)         /* Q62 */
    #define LIMIT_DOWN  (*block->min)         /* Q62 */
#endif

/* Outputs */
#define OUT             (block->Out)          /* Q31 */

/* Parameter */
#define B0              (block->b0)           /* Qx */
#define B1              (block->b1)           /* Qy */
#define SFR0            (block->sfrb0)        /* x */
#define SFR1            (block->sfrb1)        /* y */

/* Variables */
#define I_OLD           (block->i_old)        /* Q62 */
#define ENABLE_OLD      (block->enable_old)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void PILimit_FiP32_Update(PILIMIT_FIP32 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int64 y, yp;
    bool clamp;             /* Flag used for anti windup strategy clamping */


    if (ENABLE) /* Block enabled */
    {
        /* Proportional term */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        yp = ((int64)B1 * (int64)IN) >> SFR1;           /* Q31 */

        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Check if intial output is out of limits */
            if (INIT > LIMIT_UP)
            {
                y = (int64)LIMIT_UP;
            }
            else if (INIT < LIMIT_DOWN)
            {
                y = (int64)LIMIT_DOWN;
            }
            else
            {
                y = (int64)INIT;
            }

            /* Initialization of integral part so that first y = INIT */
            /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0004", true_no_defect) */
            I_OLD = (y - yp) << 31;
        }

        /* Sum of proportional and integral term */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
        y = yp + (I_OLD >> 31);                         /* Q31 */

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
            I_OLD += (((int64)B0 * (int64)IN) << (31 - SFR0));/* Q62 */
        }

        /* Assign output */
        OUT = (int32)y;
    }
    else        /* Block disabled */
    {
        OUT = 0;    /* Reset output */
    }
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void PILimit_FiP32_Init(PILIMIT_FIP32 *block)
{
    block->ID = PILIMIT_FIP32_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    I_OLD = 0;
    ENABLE_OLD = false;
    
/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PILimit_FiP32_Load(const PILIMIT_FIP32 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0u;
    if (maxSize < 10u)
    {
        error = 1u;
    }
    else
    {
        data[0] = (uint8)((uint32)block->b0 & 0xFFu);
        data[1] = (uint8)(((uint32)block->b0 >> 8) & 0xFFu);
        data[2] = (uint8)(((uint32)block->b0 >> 16) & 0xFFu);
        data[3] = (uint8)(((uint32)block->b0 >> 24) & 0xFFu);
        data[4] = (uint8)((uint32)block->b1 & 0xFFu);
        data[5] = (uint8)(((uint32)block->b1 >> 8) & 0xFFu);
        data[6] = (uint8)(((uint32)block->b1 >> 16) & 0xFFu);
        data[7] = (uint8)(((uint32)block->b1 >> 24) & 0xFFu);
        data[8] = (uint8)(block->sfrb0 & 0xFFu);
        data[9] = (uint8)(block->sfrb1 & 0xFFu);
        *dataLength = 10u;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PILimit_FiP32_Save(PILIMIT_FIP32 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 10u)
    {
        error = 1u;
    }
    else
    {
        block->b0 = castUint32ToInt32((uint32)((uint32)data[0] | ((uint32)data[1] << 8) | ((uint32)data[2] << 16) | ((uint32)data[3] << 24)));
        block->b1 = castUint32ToInt32((uint32)((uint32)data[4] | ((uint32)data[5] << 8) | ((uint32)data[6] << 16) | ((uint32)data[7] << 24)));
        block->sfrb0 = data[8];
        block->sfrb1 = data[9];
        error = 0u;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(PILIMIT_FIP32_ISLINKED)
void* PILimit_FiP32_GetAddress(PILIMIT_FIP32* block, uint16 elementId)
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
