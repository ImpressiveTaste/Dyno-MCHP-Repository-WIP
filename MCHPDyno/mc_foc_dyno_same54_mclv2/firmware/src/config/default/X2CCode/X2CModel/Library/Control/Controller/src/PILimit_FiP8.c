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
#include "PILimit_FiP8.h"

/* all used update functions to ram for c2000                                                                         */
#if defined(__ALL_UPDATE_FUNC_2_RAM_C2000__)
    #pragma CODE_SECTION(PILimit_FiP8_Update, "ramfuncs")
#endif

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* Inputs */
#define IN              (*block->In)           /* Q7 */
#define ENABLE          (*block->Enable)       /* Q0 */
#define INIT            (*block->Init)         /* Q0 */
#ifndef LIMIT_UP
    #define LIMIT_UP    (*block->max)          /* Q7 */
    #define LIMIT_DOWN  (*block->min)          /* Q7 */
#endif

/* Outputs */
#define OUT             (block->Out)           /* Q7 */

/* Parameter */
#define B0              (block->b0)            /* Qx */
#define B1              (block->b1)            /* Qy */
#define SFR0            (block->sfrb0)         /* x */
#define SFR1            (block->sfrb1)         /* y */

/* Variables */
#define I_OLD           (block->i_old)         /* Q14 */
#define ENABLE_OLD      (block->enable_old)

/* USERCODE-END:PreProcessor                                                                                          */

/**********************************************************************************************************************/
/** Update                                                                                                           **/
/**********************************************************************************************************************/
void PILimit_FiP8_Update(PILIMIT_FIP8 *block)
{
/* USERCODE-BEGIN:UpdateFnc                                                                                           */
    int16 y, yp;
    bool clamp;             /* Flag used for anti windup strategy clamping */

    if (ENABLE != 0)        /* Block enabled */
    {
        /* Proportional term */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0001", true_no_defect) */
        yp = ((int16)B1 * (int16)IN) >> SFR1;       /* Q7 */

        if (!ENABLE_OLD)    /* Rising edge of enable signal occurred */
        {
            /* Check if intial output is out of limits */
            if (INIT > LIMIT_UP)
            {
                y = (int16)LIMIT_UP;
            }
            else if (INIT < LIMIT_DOWN)
            {
                y = (int16)LIMIT_DOWN;
            }
            else
            {
                y = (int16)INIT;
            }

            /* Initialization of integral part so that first y = INIT */
			/* RULECHECKER_comment(1:0, 3:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0004", true_no_defect) */
            I_OLD = (y - yp) << 7;                    /* Q14 */
        }

        /* Sum of proportional and integral term */
        /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
        y = yp + (I_OLD>>7);                        /* Q7 */

        /* Output limitation and anti wind-up clamping */
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
            I_OLD += (((int16)B0 * (int16)IN) << (7-SFR0)); /* Q14 */
        }
        
        /* Assign output */
        OUT = (int8)y;
    }
    else            /* Block disabled */
    {
        OUT = 0;    /* reset output */
    }
    ENABLE_OLD = ENABLE;

/* USERCODE-END:UpdateFnc                                                                                             */

}

/**********************************************************************************************************************/
/** Initialization                                                                                                   **/
/**********************************************************************************************************************/
void PILimit_FiP8_Init(PILIMIT_FIP8 *block)
{
    block->ID = PILIMIT_FIP8_ID;
    block->Out = 0;
/* USERCODE-BEGIN:InitFnc                                                                                             */
    /* Preset old values */
    I_OLD = 0;
    ENABLE_OLD = 0;

/* USERCODE-END:InitFnc                                                                                               */
}

/**********************************************************************************************************************/
/** Load block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PILimit_FiP8_Load(const PILIMIT_FIP8 *block, uint8 data[], uint16 *dataLength, uint16 maxSize)
{
    uint8 error = 0u;
    if (maxSize < 4u)
    {
        error = 1u;
    }
    else
    {
        data[0] = (uint8)((uint8)block->b0 & 0xFFu);
        data[1] = (uint8)((uint8)block->b1 & 0xFFu);
        data[2] = (uint8)(block->sfrb0 & 0xFFu);
        data[3] = (uint8)(block->sfrb1 & 0xFFu);
        *dataLength = 4u;
/* USERCODE-BEGIN:LoadFnc                                                                                             */
/* USERCODE-END:LoadFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Save block data                                                                                                  **/
/**********************************************************************************************************************/
uint8 PILimit_FiP8_Save(PILIMIT_FIP8 *block, const uint8 data[], uint16 dataLength)
{
    uint8 error;
    if (dataLength != 4u)
    {
        error = 1u;
    }
    else
    {
        block->b0 = castUint8ToInt8(data[0]);
        block->b1 = castUint8ToInt8(data[1]);
        block->sfrb0 = data[2];
        block->sfrb1 = data[3];
        error = 0u;
/* USERCODE-BEGIN:SaveFnc                                                                                             */
/* USERCODE-END:SaveFnc                                                                                               */
    }
    return (error);
}

/**********************************************************************************************************************/
/** Get block element address                                                                                        **/
/**********************************************************************************************************************/
#if !defined(PILIMIT_FIP8_ISLINKED)
void* PILimit_FiP8_GetAddress(PILIMIT_FIP8* block, uint16 elementId)
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
