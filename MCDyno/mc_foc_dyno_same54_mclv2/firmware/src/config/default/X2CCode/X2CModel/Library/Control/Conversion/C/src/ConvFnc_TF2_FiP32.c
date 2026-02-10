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
/* Description: */
/* USERCODE-END:Description                                                                                           */
#include <string.h>
#include "ConvFnc_TF2_FiP32.h"

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* USERCODE-END:PreProcessor                                                                                          */

extern uint8 MaskParamBuffer[];
extern uint8 ImplParamBuffer[];
extern uint8 SaveFncDataBuffer[];

/** Implementation parameter data structure */
typedef struct {
    int32 b0;
    int32 b1;
    int32 b2;
    int32 a0;
    int32 a1;
    uint8 sfrb;
    uint8 sfra;
} TF2_FIP32_IMPL_PARAM;

/** Private prototypes */
static uint8 convertM2I(const TF2_FIP32_MASK_PARAM *maskParam, TF2_FIP32_IMPL_PARAM *impParam, float_CoT ts);

/**
 * @brief Converts Mask parameters to Implementation parameters.
 *
 * @param[in] maskParam Mask parameters
 * @param[out] impParam Implementation parameters
 * @param[in] ts Block sample time
 *
 * @return Error: zero on success, not zero in case of conversion error
 */
static uint8 convertM2I(const TF2_FIP32_MASK_PARAM *maskParam, TF2_FIP32_IMPL_PARAM *impParam, float_CoT ts)
{
    uint8 error = 0U;
/* USERCODE-BEGIN:Conversion                                                                                          */
    float_CoT mp_b0, mp_b1, mp_b2, mp_a0, mp_a1;
    uint8 sfrb, sfrb0, sfrb1, sfrb2, sfra, sfra0, sfra1;

    /* get controller parameter values */
    mp_b0 = (float_CoT)(maskParam->b0);
    mp_b1 = (float_CoT)(maskParam->b1);
    mp_b2 = (float_CoT)(maskParam->b2);
    mp_a0 = (float_CoT)(maskParam->a0);
    mp_a1 = (float_CoT)(maskParam->a1);


    /* calculate shift factors */
    sfrb0 = getQFormat(mp_b0, 32);
    sfrb1 = getQFormat(mp_b1, 32);
    sfrb2 = getQFormat(mp_b2, 32);
    sfrb = sfrb0;
    if (sfrb > sfrb1)
    {
        sfrb = sfrb1;
    }
    if (sfrb > sfrb2)
    {
        sfrb = sfrb2;
    }
    sfra0 = getQFormat(mp_a0, 32);
    sfra1 = getQFormat(mp_a1, 32);
    if (sfra0 > sfra1)
    {
        sfra = sfra1;
    }
    else
    {
        sfra = sfra0;
    }

    /* set shift factors and Q values */
    impParam->sfrb = sfrb;
    impParam->sfra = sfra;
    impParam->b0 = getQx32Value(mp_b0, sfrb);
    impParam->b1 = getQx32Value(mp_b1, sfrb);
    impParam->b2 = getQx32Value(mp_b2, sfrb);
    impParam->a0 = getQx32Value(mp_a0, sfrb);
    impParam->a1 = getQx32Value(mp_a1, sfrb);

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
uint8 TF2_FiP32_LoadMP(const TF2_FIP32_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize)
{
    uint8 error = 0U;
    if (48U > maxSize)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((*(uint64*)&(maskParam->b2)) & 0x00000000000000FF);
        data[1] = (uint8)((*(uint64*)&(maskParam->b2) >> 8) & 0x00000000000000FF);
        data[2] = (uint8)((*(uint64*)&(maskParam->b2) >> 16) & 0x00000000000000FF);
        data[3] = (uint8)((*(uint64*)&(maskParam->b2) >> 24) & 0x00000000000000FF);
        data[4] = (uint8)((*(uint64*)&(maskParam->b2) >> 32) & 0x00000000000000FF);
        data[5] = (uint8)((*(uint64*)&(maskParam->b2) >> 40) & 0x00000000000000FF);
        data[6] = (uint8)((*(uint64*)&(maskParam->b2) >> 48) & 0x00000000000000FF);
        data[7] = (uint8)((*(uint64*)&(maskParam->b2) >> 56) & 0x00000000000000FF);
        data[8] = (uint8)((*(uint64*)&(maskParam->b1)) & 0x00000000000000FF);
        data[9] = (uint8)((*(uint64*)&(maskParam->b1) >> 8) & 0x00000000000000FF);
        data[10] = (uint8)((*(uint64*)&(maskParam->b1) >> 16) & 0x00000000000000FF);
        data[11] = (uint8)((*(uint64*)&(maskParam->b1) >> 24) & 0x00000000000000FF);
        data[12] = (uint8)((*(uint64*)&(maskParam->b1) >> 32) & 0x00000000000000FF);
        data[13] = (uint8)((*(uint64*)&(maskParam->b1) >> 40) & 0x00000000000000FF);
        data[14] = (uint8)((*(uint64*)&(maskParam->b1) >> 48) & 0x00000000000000FF);
        data[15] = (uint8)((*(uint64*)&(maskParam->b1) >> 56) & 0x00000000000000FF);
        data[16] = (uint8)((*(uint64*)&(maskParam->b0)) & 0x00000000000000FF);
        data[17] = (uint8)((*(uint64*)&(maskParam->b0) >> 8) & 0x00000000000000FF);
        data[18] = (uint8)((*(uint64*)&(maskParam->b0) >> 16) & 0x00000000000000FF);
        data[19] = (uint8)((*(uint64*)&(maskParam->b0) >> 24) & 0x00000000000000FF);
        data[20] = (uint8)((*(uint64*)&(maskParam->b0) >> 32) & 0x00000000000000FF);
        data[21] = (uint8)((*(uint64*)&(maskParam->b0) >> 40) & 0x00000000000000FF);
        data[22] = (uint8)((*(uint64*)&(maskParam->b0) >> 48) & 0x00000000000000FF);
        data[23] = (uint8)((*(uint64*)&(maskParam->b0) >> 56) & 0x00000000000000FF);
        data[24] = (uint8)((*(uint64*)&(maskParam->a1)) & 0x00000000000000FF);
        data[25] = (uint8)((*(uint64*)&(maskParam->a1) >> 8) & 0x00000000000000FF);
        data[26] = (uint8)((*(uint64*)&(maskParam->a1) >> 16) & 0x00000000000000FF);
        data[27] = (uint8)((*(uint64*)&(maskParam->a1) >> 24) & 0x00000000000000FF);
        data[28] = (uint8)((*(uint64*)&(maskParam->a1) >> 32) & 0x00000000000000FF);
        data[29] = (uint8)((*(uint64*)&(maskParam->a1) >> 40) & 0x00000000000000FF);
        data[30] = (uint8)((*(uint64*)&(maskParam->a1) >> 48) & 0x00000000000000FF);
        data[31] = (uint8)((*(uint64*)&(maskParam->a1) >> 56) & 0x00000000000000FF);
        data[32] = (uint8)((*(uint64*)&(maskParam->a0)) & 0x00000000000000FF);
        data[33] = (uint8)((*(uint64*)&(maskParam->a0) >> 8) & 0x00000000000000FF);
        data[34] = (uint8)((*(uint64*)&(maskParam->a0) >> 16) & 0x00000000000000FF);
        data[35] = (uint8)((*(uint64*)&(maskParam->a0) >> 24) & 0x00000000000000FF);
        data[36] = (uint8)((*(uint64*)&(maskParam->a0) >> 32) & 0x00000000000000FF);
        data[37] = (uint8)((*(uint64*)&(maskParam->a0) >> 40) & 0x00000000000000FF);
        data[38] = (uint8)((*(uint64*)&(maskParam->a0) >> 48) & 0x00000000000000FF);
        data[39] = (uint8)((*(uint64*)&(maskParam->a0) >> 56) & 0x00000000000000FF);
        data[40] = (uint8)((*(uint64*)&(maskParam->ts_fact)) & 0x00000000000000FF);
        data[41] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 8) & 0x00000000000000FF);
        data[42] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 16) & 0x00000000000000FF);
        data[43] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 24) & 0x00000000000000FF);
        data[44] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 32) & 0x00000000000000FF);
        data[45] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 40) & 0x00000000000000FF);
        data[46] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 48) & 0x00000000000000FF);
        data[47] = (uint8)((*(uint64*)&(maskParam->ts_fact) >> 56) & 0x00000000000000FF);
        *dataLen = 48U;
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
uint8 TF2_FiP32_SaveMP(TF2_FIP32 *block, TF2_FIP32_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen)
{
    uint8 error = 0U;

    if (dataLen != 48U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        /** - Cache old mask parameters */
        TF2_FiP32_BackupMP(maskParam);

        /** - Save new mask parameters */
        tmp64 = (uint64)data[0] + \
            ((uint64)data[1] << 8) + ((uint64)data[2] << 16) + \
            ((uint64)data[3] << 24) + ((uint64)data[4] << 32) + \
            ((uint64)data[5] << 40) + ((uint64)data[6] << 48) + \
            ((uint64)data[7] << 56);
        maskParam->b2 = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[8] + \
            ((uint64)data[9] << 8) + ((uint64)data[10] << 16) + \
            ((uint64)data[11] << 24) + ((uint64)data[12] << 32) + \
            ((uint64)data[13] << 40) + ((uint64)data[14] << 48) + \
            ((uint64)data[15] << 56);
        maskParam->b1 = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[16] + \
            ((uint64)data[17] << 8) + ((uint64)data[18] << 16) + \
            ((uint64)data[19] << 24) + ((uint64)data[20] << 32) + \
            ((uint64)data[21] << 40) + ((uint64)data[22] << 48) + \
            ((uint64)data[23] << 56);
        maskParam->b0 = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[24] + \
            ((uint64)data[25] << 8) + ((uint64)data[26] << 16) + \
            ((uint64)data[27] << 24) + ((uint64)data[28] << 32) + \
            ((uint64)data[29] << 40) + ((uint64)data[30] << 48) + \
            ((uint64)data[31] << 56);
        maskParam->a1 = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[32] + \
            ((uint64)data[33] << 8) + ((uint64)data[34] << 16) + \
            ((uint64)data[35] << 24) + ((uint64)data[36] << 32) + \
            ((uint64)data[37] << 40) + ((uint64)data[38] << 48) + \
            ((uint64)data[39] << 56);
        maskParam->a0 = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[40] + \
            ((uint64)data[41] << 8) + ((uint64)data[42] << 16) + \
            ((uint64)data[43] << 24) + ((uint64)data[44] << 32) + \
            ((uint64)data[45] << 40) + ((uint64)data[46] << 48) + \
            ((uint64)data[47] << 56);
        maskParam->ts_fact = (float64)(*(float64*)&tmp64);

        error = TF2_FiP32_ConvertMP(block, maskParam);

        /** - Check for errors during execution of save function */
        if (error != 0U)
        {
            /* Restore old mask parameter */
            TF2_FiP32_RestoreMP(maskParam);
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
uint8 TF2_FiP32_InitMP(TF2_FIP32 *block, const TF2_FIP32_MASK_PARAM *maskParam)
{
    uint8 error = 0U;
    float_CoT block_ts;

    TF2_FIP32_IMPL_PARAM *implParam = (TF2_FIP32_IMPL_PARAM*)ImplParamBuffer;

    /** - Get Block Sample Time */
    block_ts = maskParam->ts_fact * getModelSampleTime();
    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam, block_ts);

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->b0 & 0x000000FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->b0 >> 8) & 0x000000FF);
    SaveFncDataBuffer[2] = (uint8)((implParam->b0 >> 16) & 0x000000FF);
    SaveFncDataBuffer[3] = (uint8)((implParam->b0 >> 24) & 0x000000FF);
    SaveFncDataBuffer[4] = (uint8)(implParam->b1 & 0x000000FF);
    SaveFncDataBuffer[5] = (uint8)((implParam->b1 >> 8) & 0x000000FF);
    SaveFncDataBuffer[6] = (uint8)((implParam->b1 >> 16) & 0x000000FF);
    SaveFncDataBuffer[7] = (uint8)((implParam->b1 >> 24) & 0x000000FF);
    SaveFncDataBuffer[8] = (uint8)(implParam->b2 & 0x000000FF);
    SaveFncDataBuffer[9] = (uint8)((implParam->b2 >> 8) & 0x000000FF);
    SaveFncDataBuffer[10] = (uint8)((implParam->b2 >> 16) & 0x000000FF);
    SaveFncDataBuffer[11] = (uint8)((implParam->b2 >> 24) & 0x000000FF);
    SaveFncDataBuffer[12] = (uint8)(implParam->a0 & 0x000000FF);
    SaveFncDataBuffer[13] = (uint8)((implParam->a0 >> 8) & 0x000000FF);
    SaveFncDataBuffer[14] = (uint8)((implParam->a0 >> 16) & 0x000000FF);
    SaveFncDataBuffer[15] = (uint8)((implParam->a0 >> 24) & 0x000000FF);
    SaveFncDataBuffer[16] = (uint8)(implParam->a1 & 0x000000FF);
    SaveFncDataBuffer[17] = (uint8)((implParam->a1 >> 8) & 0x000000FF);
    SaveFncDataBuffer[18] = (uint8)((implParam->a1 >> 16) & 0x000000FF);
    SaveFncDataBuffer[19] = (uint8)((implParam->a1 >> 24) & 0x000000FF);
    SaveFncDataBuffer[20] = (uint8)implParam->sfrb;
    SaveFncDataBuffer[21] = (uint8)implParam->sfra;

    /** - Execute save function of block */
    error = TF2_FiP32_Save(block, SaveFncDataBuffer, 22);

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
uint8 TF2_FiP32_ConvertMP(TF2_FIP32 *block, TF2_FIP32_MASK_PARAM *maskParam)
{
    uint8 error;
    float_CoT block_ts;
    TF2_FIP32_IMPL_PARAM *implParam = (TF2_FIP32_IMPL_PARAM*)ImplParamBuffer;

    /** - Get Block Sample Time */
    block_ts = maskParam->ts_fact * getModelSampleTime();
    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam, block_ts);
    if (error != 0U)
    {
        return (error);
    }

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->b0 & 0x000000FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->b0 >> 8) & 0x000000FF);
    SaveFncDataBuffer[2] = (uint8)((implParam->b0 >> 16) & 0x000000FF);
    SaveFncDataBuffer[3] = (uint8)((implParam->b0 >> 24) & 0x000000FF);
    SaveFncDataBuffer[4] = (uint8)(implParam->b1 & 0x000000FF);
    SaveFncDataBuffer[5] = (uint8)((implParam->b1 >> 8) & 0x000000FF);
    SaveFncDataBuffer[6] = (uint8)((implParam->b1 >> 16) & 0x000000FF);
    SaveFncDataBuffer[7] = (uint8)((implParam->b1 >> 24) & 0x000000FF);
    SaveFncDataBuffer[8] = (uint8)(implParam->b2 & 0x000000FF);
    SaveFncDataBuffer[9] = (uint8)((implParam->b2 >> 8) & 0x000000FF);
    SaveFncDataBuffer[10] = (uint8)((implParam->b2 >> 16) & 0x000000FF);
    SaveFncDataBuffer[11] = (uint8)((implParam->b2 >> 24) & 0x000000FF);
    SaveFncDataBuffer[12] = (uint8)(implParam->a0 & 0x000000FF);
    SaveFncDataBuffer[13] = (uint8)((implParam->a0 >> 8) & 0x000000FF);
    SaveFncDataBuffer[14] = (uint8)((implParam->a0 >> 16) & 0x000000FF);
    SaveFncDataBuffer[15] = (uint8)((implParam->a0 >> 24) & 0x000000FF);
    SaveFncDataBuffer[16] = (uint8)(implParam->a1 & 0x000000FF);
    SaveFncDataBuffer[17] = (uint8)((implParam->a1 >> 8) & 0x000000FF);
    SaveFncDataBuffer[18] = (uint8)((implParam->a1 >> 16) & 0x000000FF);
    SaveFncDataBuffer[19] = (uint8)((implParam->a1 >> 24) & 0x000000FF);
    SaveFncDataBuffer[20] = (uint8)implParam->sfrb;
    SaveFncDataBuffer[21] = (uint8)implParam->sfra;

    /** - Execute save function of block */
    error = TF2_FiP32_Save(block, SaveFncDataBuffer, 22);

    return (error);
}

/**
 * @brief Backups current Mask parameters into global Mask parameter backup array.
 *
 * @param[in] maskParam Mask parameters
 *
 * @return Nothing
 */
void TF2_FiP32_BackupMP(TF2_FIP32_MASK_PARAM* maskParam)
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
void TF2_FiP32_RestoreMP(TF2_FIP32_MASK_PARAM* maskParam)
{
    memcpy(maskParam, MaskParamBuffer, sizeof(*maskParam));
}
