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
/* Description: */
/* USERCODE-END:Description                                                                                           */
#include <string.h>
#include "ConvFnc_PI_FiP32.h"

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* USERCODE-END:PreProcessor                                                                                          */

extern uint8 MaskParamBuffer[];
extern uint8 ImplParamBuffer[];
extern uint8 SaveFncDataBuffer[];

/** Implementation parameter data structure */
typedef struct {
    int32 b0;
    int32 b1;
    uint8 sfrb0;
    uint8 sfrb1;
} PI_FIP32_IMPL_PARAM;

/** Private prototypes */
static uint8 convertM2I(const PI_FIP32_MASK_PARAM *maskParam, PI_FIP32_IMPL_PARAM *impParam, float_CoT ts);

/**
 * @brief Converts Mask parameters to Implementation parameters.
 *
 * @param[in] maskParam Mask parameters
 * @param[out] impParam Implementation parameters
 * @param[in] ts Block sample time
 *
 * @return Error: zero on success, not zero in case of conversion error
 */
static uint8 convertM2I(const PI_FIP32_MASK_PARAM *maskParam, PI_FIP32_IMPL_PARAM *impParam, float_CoT ts)
{
    uint8 error = 0u;
/* USERCODE-BEGIN:Conversion                                                                                          */
    float_CoT mp_Kp, mp_Ki;
    float_CoT b0, b1;

    /* get mask parameter values */
    mp_Kp = (float_CoT)(maskParam->Kp);
    mp_Ki = (float_CoT)(maskParam->Ki);


    /* Zero order hold (zoh):
     * G(s) = Kp + Ki/s -> G(z) = Kp + Ki*Ts*1/(z-1)
     */
    b0 = mp_Ki * ts;
    b1 = mp_Kp;

    impParam->sfrb0 = getQFormat(b0, 32);
    impParam->sfrb1 = getQFormat(b1, 32);
    impParam->b0 = getQx32Value(b0, (impParam->sfrb0));
    impParam->b1 = getQx32Value(b1, (impParam->sfrb1));

/* USERCODE-END:Conversion                                                                                            */
    return (error);
}

/**
 * @brief Load block mask parameter data.
 *
 * @param[in] maskParam Mask parameter data structure
 * @param[out] data Data
 * @param[out] dataLen Data length
 * @param[in] maxSize Maximum Service data length
 *
 * @return Error: zero on success, not zero in case of load- or conversion error
 */
uint8 PI_FiP32_LoadMP(const PI_FIP32_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize)
{
    uint8 error = 0u;
    if (maxSize < 24u)
    {
        error = 1u;
    }
    else
    {
        data[0] = (uint8)((*(uint64*)&(maskParam->Kp)) & 0xFFu);
        data[1] = (uint8)((*(uint64*)&(maskParam->Kp) >> 8) & 0xFFu);
        data[2] = (uint8)((*(uint64*)&(maskParam->Kp) >> 16) & 0xFFu);
        data[3] = (uint8)((*(uint64*)&(maskParam->Kp) >> 24) & 0xFFu);
        data[4] = (uint8)((*(uint64*)&(maskParam->Kp) >> 32) & 0xFFu);
        data[5] = (uint8)((*(uint64*)&(maskParam->Kp) >> 40) & 0xFFu);
        data[6] = (uint8)((*(uint64*)&(maskParam->Kp) >> 48) & 0xFFu);
        data[7] = (uint8)((*(uint64*)&(maskParam->Kp) >> 56) & 0xFFu);
        data[8] = (uint8)((*(uint64*)&(maskParam->Ki)) & 0xFFu);
        data[9] = (uint8)((*(uint64*)&(maskParam->Ki) >> 8) & 0xFFu);
        data[10] = (uint8)((*(uint64*)&(maskParam->Ki) >> 16) & 0xFFu);
        data[11] = (uint8)((*(uint64*)&(maskParam->Ki) >> 24) & 0xFFu);
        data[12] = (uint8)((*(uint64*)&(maskParam->Ki) >> 32) & 0xFFu);
        data[13] = (uint8)((*(uint64*)&(maskParam->Ki) >> 40) & 0xFFu);
        data[14] = (uint8)((*(uint64*)&(maskParam->Ki) >> 48) & 0xFFu);
        data[15] = (uint8)((*(uint64*)&(maskParam->Ki) >> 56) & 0xFFu);
        data[16] = (uint8)((*(uint64*)&(maskParam->ts_fact)) & 0xFFu);
        data[17] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 8) & 0xFFu);
        data[18] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 16) & 0xFFu);
        data[19] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 24) & 0xFFu);
        data[20] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 32) & 0xFFu);
        data[21] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 40) & 0xFFu);
        data[22] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 48) & 0xFFu);
        data[23] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 56) & 0xFFu);
        *dataLen = 24u;
    }
    return (error);
}

/**
 * @brief Save block mask parameter data.
 *
 * @param[in] block Pointer to block structure
 * @param[out] maskParam Mask parameter data structure
 * @param[in] data Data
 * @param dataLen Length of mask parameter data stream
 *
 * @return Error: zero on success, not zero in case of save- or conversion error
 */
uint8 PI_FiP32_SaveMP(PI_FIP32 *block, PI_FIP32_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen)
{
    uint8 error = 0u;

    if (dataLen != 24u)
    {
        error = 1u;
    }
    else
    {
        uint64 tmp64;
        /** - Cache old mask parameters */
        PI_FiP32_BackupMP(maskParam);

        /** - Save new mask parameters */
        tmp64 = (uint64)data[0] + \
            ((uint64)data[1] << 8) + ((uint64)data[2] << 16) + \
            ((uint64)data[3] << 24) + ((uint64)data[4] << 32) + \
            ((uint64)data[5] << 40) + ((uint64)data[6] << 48) + \
            ((uint64)data[7] << 56);
        maskParam->Kp = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[8] + \
            ((uint64)data[9] << 8) + ((uint64)data[10] << 16) + \
            ((uint64)data[11] << 24) + ((uint64)data[12] << 32) + \
            ((uint64)data[13] << 40) + ((uint64)data[14] << 48) + \
            ((uint64)data[15] << 56);
        maskParam->Ki = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[16] + \
            ((uint64)data[17] << 8) + ((uint64)data[18] << 16) + \
            ((uint64)data[19] << 24) + ((uint64)data[20] << 32) + \
            ((uint64)data[21] << 40) + ((uint64)data[22] << 48) + \
            ((uint64)data[23] << 56);
        maskParam->ts_fact = (float64)(*(float64*)&tmp64);

        error = PI_FiP32_ConvertMP(block, maskParam);

        /** - Check for errors during execution of save function */
        if (error != 0u)
        {
            /* Restore old mask parameter */
            PI_FiP32_RestoreMP(maskParam);
        }

    }
    return (error);
}

/**
 * @brief Initializes Implementation parameters from Mask parameters.
 *
 * @param[out] block Block
 * @param[in] maskParam Mask parameters
 *
 * @return Error: zero on success, not zero in case of initialization error
 */
uint8 PI_FiP32_InitMP(PI_FIP32 *block, const PI_FIP32_MASK_PARAM *maskParam)
{
    uint8 error = 0u;
    float_CoT block_ts;

    PI_FIP32_IMPL_PARAM *implParam = (PI_FIP32_IMPL_PARAM*)ImplParamBuffer;

    /** - Get Block Sample Time */
    block_ts = maskParam->ts_fact * getModelSampleTime();
    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam, block_ts);

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)((uint32)implParam->b0 & 0xFFu);
    SaveFncDataBuffer[1] = (uint8)(((uint32)implParam->b0 >> 8) & 0xFFu);
    SaveFncDataBuffer[2] = (uint8)(((uint32)implParam->b0 >> 16) & 0xFFu);
    SaveFncDataBuffer[3] = (uint8)(((uint32)implParam->b0 >> 24) & 0xFFu);
    SaveFncDataBuffer[4] = (uint8)((uint32)implParam->b1 & 0xFFu);
    SaveFncDataBuffer[5] = (uint8)(((uint32)implParam->b1 >> 8) & 0xFFu);
    SaveFncDataBuffer[6] = (uint8)(((uint32)implParam->b1 >> 16) & 0xFFu);
    SaveFncDataBuffer[7] = (uint8)(((uint32)implParam->b1 >> 24) & 0xFFu);
    SaveFncDataBuffer[8] = (uint8)(implParam->sfrb0 & 0xFFu);
    SaveFncDataBuffer[9] = (uint8)(implParam->sfrb1 & 0xFFu);

    /** - Execute save function of block */
    error = PI_FiP32_Save(block, SaveFncDataBuffer, 10);

    return (error);
}

/**
 * @brief Converts Mask parameters into Implementation parameters and executes Block Save function.
 *
 * @param[out] block Block Implementation
 * @param[in] maskParam Mask Parameters
 *
 * @return 0 if successful, greater than 0 if conversion failed
 */
uint8 PI_FiP32_ConvertMP(PI_FIP32 *block, PI_FIP32_MASK_PARAM *maskParam)
{
    uint8 error;
    float_CoT block_ts;
    PI_FIP32_IMPL_PARAM *implParam = (PI_FIP32_IMPL_PARAM*)ImplParamBuffer;

    /** - Get Block Sample Time */
    block_ts = maskParam->ts_fact * getModelSampleTime();
    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam, block_ts);
    if (error != 0u)
    {
        return (error);
    }

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)((uint32)implParam->b0 & 0xFFu);
    SaveFncDataBuffer[1] = (uint8)(((uint32)implParam->b0 >> 8) & 0xFFu);
    SaveFncDataBuffer[2] = (uint8)(((uint32)implParam->b0 >> 16) & 0xFFu);
    SaveFncDataBuffer[3] = (uint8)(((uint32)implParam->b0 >> 24) & 0xFFu);
    SaveFncDataBuffer[4] = (uint8)((uint32)implParam->b1 & 0xFFu);
    SaveFncDataBuffer[5] = (uint8)(((uint32)implParam->b1 >> 8) & 0xFFu);
    SaveFncDataBuffer[6] = (uint8)(((uint32)implParam->b1 >> 16) & 0xFFu);
    SaveFncDataBuffer[7] = (uint8)(((uint32)implParam->b1 >> 24) & 0xFFu);
    SaveFncDataBuffer[8] = (uint8)(implParam->sfrb0 & 0xFFu);
    SaveFncDataBuffer[9] = (uint8)(implParam->sfrb1 & 0xFFu);

    /** - Execute save function of block */
    error = PI_FiP32_Save(block, SaveFncDataBuffer, 10);

    return (error);
}

/**
 * @brief Backups current Mask parameters into global Mask parameter backup array.
 *
 * @param[in] maskParam Mask parameters
 *
 * @return Nothing
 */
void PI_FiP32_BackupMP(PI_FIP32_MASK_PARAM* maskParam)
{
    memcpy(MaskParamBuffer, maskParam, sizeof(*maskParam));
}

/**
 * @brief Restores Mask parameters from global Mask parameter backup array.
 *
 * @param[out] maskParam Mask parameters
 *
 * @return Nothing
 */
void PI_FiP32_RestoreMP(PI_FIP32_MASK_PARAM* maskParam)
{
    memcpy(maskParam, MaskParamBuffer, sizeof(*maskParam));
}
