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
#include "ConvFnc_LookupTable3D_FiP16.h"

/* USERCODE-BEGIN:PreProcessor                                                                                        */
/* USERCODE-END:PreProcessor                                                                                          */

extern uint8 MaskParamBuffer[];
extern uint8 ImplParamBuffer[];
extern uint8 SaveFncDataBuffer[];

/** Implementation parameter data structure */
typedef struct {
    uint8 sfrX;
    uint8 sfrY;
    uint8 sfrZ;
    uint16 maskX;
    uint16 maskY;
    uint16 maskZ;
    uint16 idxOffsetX;
    uint16 idxOffsetY;
    uint16 idxOffsetZ;
    uint16 sizeX;
    uint16 sizeY;
    uint16 sizeXY;
    int16 gainX;
    int16 gainY;
    int16 gainZ;
    uint8 gainXsfr;
    uint8 gainYsfr;
    uint8 gainZsfr;
    int16 offsetX;
    int16 offsetY;
    int16 offsetZ;
    int16 minX;
    int16 maxX;
    int16 minY;
    int16 maxY;
    int16 minZ;
    int16 maxZ;
} LOOKUPTABLE3D_FIP16_IMPL_PARAM;

/** DimX enumeration */
typedef enum {
    DIMX_3 = 0,
    DIMX_5 = 1,
    DIMX_9 = 2,
    DIMX_17 = 3,
    DIMX_33 = 4,
    DIMX_65 = 5,
    DIMX_129 = 6,
    DIMX_257 = 7,
    DIMX_513 = 8,
    DIMX_1025 = 9,
    DIMX_2049 = 10,
    DIMX_4097 = 11
} tDimX;

/** DimY enumeration */
typedef enum {
    DIMY_3 = 0,
    DIMY_5 = 1,
    DIMY_9 = 2,
    DIMY_17 = 3,
    DIMY_33 = 4,
    DIMY_65 = 5,
    DIMY_129 = 6,
    DIMY_257 = 7,
    DIMY_513 = 8,
    DIMY_1025 = 9,
    DIMY_2049 = 10,
    DIMY_4097 = 11
} tDimY;

/** DimZ enumeration */
typedef enum {
    DIMZ_3 = 0,
    DIMZ_5 = 1,
    DIMZ_9 = 2,
    DIMZ_17 = 3,
    DIMZ_33 = 4,
    DIMZ_65 = 5,
    DIMZ_129 = 6,
    DIMZ_257 = 7
} tDimZ;

/** Private prototypes */
static uint8 convertM2I(const LOOKUPTABLE3D_FIP16_MASK_PARAM *maskParam, LOOKUPTABLE3D_FIP16_IMPL_PARAM *impParam);

/**
 * @brief Converts Mask parameters to Implementation parameters.
 *
 * @param[in] maskParam Mask parameters
 * @param[out] impParam Implementation parameters
 *
 * @return Error: zero on success, not zero in case of conversion error
 */
static uint8 convertM2I(const LOOKUPTABLE3D_FIP16_MASK_PARAM *maskParam, LOOKUPTABLE3D_FIP16_IMPL_PARAM *impParam)
{
    uint8 error = 0U;
/* USERCODE-BEGIN:Conversion                                                                                          */
    error = 1;  /* conversion of flash table is not supported/reasonable */
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
uint8 LookupTable3D_FiP16_LoadMP(const LOOKUPTABLE3D_FIP16_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize)
{
    uint8 error = 0U;
    if (59U > maxSize)
    {
        error = 1U;
    }
    else
    {
        data[0] = (uint8)((*(uint64*)&(maskParam->TableData)) & 0x00000000000000FF);
        data[1] = (uint8)((*(uint64*)&(maskParam->TableData) >> 8) & 0x00000000000000FF);
        data[2] = (uint8)((*(uint64*)&(maskParam->TableData) >> 16) & 0x00000000000000FF);
        data[3] = (uint8)((*(uint64*)&(maskParam->TableData) >> 24) & 0x00000000000000FF);
        data[4] = (uint8)((*(uint64*)&(maskParam->TableData) >> 32) & 0x00000000000000FF);
        data[5] = (uint8)((*(uint64*)&(maskParam->TableData) >> 40) & 0x00000000000000FF);
        data[6] = (uint8)((*(uint64*)&(maskParam->TableData) >> 48) & 0x00000000000000FF);
        data[7] = (uint8)((*(uint64*)&(maskParam->TableData) >> 56) & 0x00000000000000FF);
        data[8] = (uint8)(maskParam->DimX);
        data[9] = (uint8)((*(uint64*)&(maskParam->x_min)) & 0x00000000000000FF);
        data[10] = (uint8)((*(uint64*)&(maskParam->x_min) >> 8) & 0x00000000000000FF);
        data[11] = (uint8)((*(uint64*)&(maskParam->x_min) >> 16) & 0x00000000000000FF);
        data[12] = (uint8)((*(uint64*)&(maskParam->x_min) >> 24) & 0x00000000000000FF);
        data[13] = (uint8)((*(uint64*)&(maskParam->x_min) >> 32) & 0x00000000000000FF);
        data[14] = (uint8)((*(uint64*)&(maskParam->x_min) >> 40) & 0x00000000000000FF);
        data[15] = (uint8)((*(uint64*)&(maskParam->x_min) >> 48) & 0x00000000000000FF);
        data[16] = (uint8)((*(uint64*)&(maskParam->x_min) >> 56) & 0x00000000000000FF);
        data[17] = (uint8)((*(uint64*)&(maskParam->x_max)) & 0x00000000000000FF);
        data[18] = (uint8)((*(uint64*)&(maskParam->x_max) >> 8) & 0x00000000000000FF);
        data[19] = (uint8)((*(uint64*)&(maskParam->x_max) >> 16) & 0x00000000000000FF);
        data[20] = (uint8)((*(uint64*)&(maskParam->x_max) >> 24) & 0x00000000000000FF);
        data[21] = (uint8)((*(uint64*)&(maskParam->x_max) >> 32) & 0x00000000000000FF);
        data[22] = (uint8)((*(uint64*)&(maskParam->x_max) >> 40) & 0x00000000000000FF);
        data[23] = (uint8)((*(uint64*)&(maskParam->x_max) >> 48) & 0x00000000000000FF);
        data[24] = (uint8)((*(uint64*)&(maskParam->x_max) >> 56) & 0x00000000000000FF);
        data[25] = (uint8)(maskParam->DimY);
        data[26] = (uint8)((*(uint64*)&(maskParam->y_max)) & 0x00000000000000FF);
        data[27] = (uint8)((*(uint64*)&(maskParam->y_max) >> 8) & 0x00000000000000FF);
        data[28] = (uint8)((*(uint64*)&(maskParam->y_max) >> 16) & 0x00000000000000FF);
        data[29] = (uint8)((*(uint64*)&(maskParam->y_max) >> 24) & 0x00000000000000FF);
        data[30] = (uint8)((*(uint64*)&(maskParam->y_max) >> 32) & 0x00000000000000FF);
        data[31] = (uint8)((*(uint64*)&(maskParam->y_max) >> 40) & 0x00000000000000FF);
        data[32] = (uint8)((*(uint64*)&(maskParam->y_max) >> 48) & 0x00000000000000FF);
        data[33] = (uint8)((*(uint64*)&(maskParam->y_max) >> 56) & 0x00000000000000FF);
        data[34] = (uint8)((*(uint64*)&(maskParam->y_min)) & 0x00000000000000FF);
        data[35] = (uint8)((*(uint64*)&(maskParam->y_min) >> 8) & 0x00000000000000FF);
        data[36] = (uint8)((*(uint64*)&(maskParam->y_min) >> 16) & 0x00000000000000FF);
        data[37] = (uint8)((*(uint64*)&(maskParam->y_min) >> 24) & 0x00000000000000FF);
        data[38] = (uint8)((*(uint64*)&(maskParam->y_min) >> 32) & 0x00000000000000FF);
        data[39] = (uint8)((*(uint64*)&(maskParam->y_min) >> 40) & 0x00000000000000FF);
        data[40] = (uint8)((*(uint64*)&(maskParam->y_min) >> 48) & 0x00000000000000FF);
        data[41] = (uint8)((*(uint64*)&(maskParam->y_min) >> 56) & 0x00000000000000FF);
        data[42] = (uint8)(maskParam->DimZ);
        data[43] = (uint8)((*(uint64*)&(maskParam->z_min)) & 0x00000000000000FF);
        data[44] = (uint8)((*(uint64*)&(maskParam->z_min) >> 8) & 0x00000000000000FF);
        data[45] = (uint8)((*(uint64*)&(maskParam->z_min) >> 16) & 0x00000000000000FF);
        data[46] = (uint8)((*(uint64*)&(maskParam->z_min) >> 24) & 0x00000000000000FF);
        data[47] = (uint8)((*(uint64*)&(maskParam->z_min) >> 32) & 0x00000000000000FF);
        data[48] = (uint8)((*(uint64*)&(maskParam->z_min) >> 40) & 0x00000000000000FF);
        data[49] = (uint8)((*(uint64*)&(maskParam->z_min) >> 48) & 0x00000000000000FF);
        data[50] = (uint8)((*(uint64*)&(maskParam->z_min) >> 56) & 0x00000000000000FF);
        data[51] = (uint8)((*(uint64*)&(maskParam->z_max)) & 0x00000000000000FF);
        data[52] = (uint8)((*(uint64*)&(maskParam->z_max) >> 8) & 0x00000000000000FF);
        data[53] = (uint8)((*(uint64*)&(maskParam->z_max) >> 16) & 0x00000000000000FF);
        data[54] = (uint8)((*(uint64*)&(maskParam->z_max) >> 24) & 0x00000000000000FF);
        data[55] = (uint8)((*(uint64*)&(maskParam->z_max) >> 32) & 0x00000000000000FF);
        data[56] = (uint8)((*(uint64*)&(maskParam->z_max) >> 40) & 0x00000000000000FF);
        data[57] = (uint8)((*(uint64*)&(maskParam->z_max) >> 48) & 0x00000000000000FF);
        data[58] = (uint8)((*(uint64*)&(maskParam->z_max) >> 56) & 0x00000000000000FF);
        *dataLen = 59U;
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
uint8 LookupTable3D_FiP16_SaveMP(LOOKUPTABLE3D_FIP16 *block, LOOKUPTABLE3D_FIP16_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen)
{
    uint8 error = 0U;

    if (dataLen != 59U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        /** - Cache old mask parameters */
        LookupTable3D_FiP16_BackupMP(maskParam);

        /** - Save new mask parameters */
        tmp64 = (uint64)data[0] + \
            ((uint64)data[1] << 8) + ((uint64)data[2] << 16) + \
            ((uint64)data[3] << 24) + ((uint64)data[4] << 32) + \
            ((uint64)data[5] << 40) + ((uint64)data[6] << 48) + \
            ((uint64)data[7] << 56);
        maskParam->TableData = (float64)(*(float64*)&tmp64);
        maskParam->DimX = data[8];
        tmp64 = (uint64)data[9] + \
            ((uint64)data[10] << 8) + ((uint64)data[11] << 16) + \
            ((uint64)data[12] << 24) + ((uint64)data[13] << 32) + \
            ((uint64)data[14] << 40) + ((uint64)data[15] << 48) + \
            ((uint64)data[16] << 56);
        maskParam->x_min = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[17] + \
            ((uint64)data[18] << 8) + ((uint64)data[19] << 16) + \
            ((uint64)data[20] << 24) + ((uint64)data[21] << 32) + \
            ((uint64)data[22] << 40) + ((uint64)data[23] << 48) + \
            ((uint64)data[24] << 56);
        maskParam->x_max = (float64)(*(float64*)&tmp64);
        maskParam->DimY = data[25];
        tmp64 = (uint64)data[26] + \
            ((uint64)data[27] << 8) + ((uint64)data[28] << 16) + \
            ((uint64)data[29] << 24) + ((uint64)data[30] << 32) + \
            ((uint64)data[31] << 40) + ((uint64)data[32] << 48) + \
            ((uint64)data[33] << 56);
        maskParam->y_max = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[34] + \
            ((uint64)data[35] << 8) + ((uint64)data[36] << 16) + \
            ((uint64)data[37] << 24) + ((uint64)data[38] << 32) + \
            ((uint64)data[39] << 40) + ((uint64)data[40] << 48) + \
            ((uint64)data[41] << 56);
        maskParam->y_min = (float64)(*(float64*)&tmp64);
        maskParam->DimZ = data[42];
        tmp64 = (uint64)data[43] + \
            ((uint64)data[44] << 8) + ((uint64)data[45] << 16) + \
            ((uint64)data[46] << 24) + ((uint64)data[47] << 32) + \
            ((uint64)data[48] << 40) + ((uint64)data[49] << 48) + \
            ((uint64)data[50] << 56);
        maskParam->z_min = (float64)(*(float64*)&tmp64);
        tmp64 = (uint64)data[51] + \
            ((uint64)data[52] << 8) + ((uint64)data[53] << 16) + \
            ((uint64)data[54] << 24) + ((uint64)data[55] << 32) + \
            ((uint64)data[56] << 40) + ((uint64)data[57] << 48) + \
            ((uint64)data[58] << 56);
        maskParam->z_max = (float64)(*(float64*)&tmp64);

        error = LookupTable3D_FiP16_ConvertMP(block, maskParam);

        /** - Check for errors during execution of save function */
        if (error != 0U)
        {
            /* Restore old mask parameter */
            LookupTable3D_FiP16_RestoreMP(maskParam);
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
uint8 LookupTable3D_FiP16_InitMP(LOOKUPTABLE3D_FIP16 *block, const LOOKUPTABLE3D_FIP16_MASK_PARAM *maskParam)
{
    uint8 error = 0U;

    LOOKUPTABLE3D_FIP16_IMPL_PARAM *implParam = (LOOKUPTABLE3D_FIP16_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)implParam->sfrX;
    SaveFncDataBuffer[1] = (uint8)implParam->sfrY;
    SaveFncDataBuffer[2] = (uint8)implParam->sfrZ;
    SaveFncDataBuffer[3] = (uint8)(implParam->maskX & 0x00FF);
    SaveFncDataBuffer[4] = (uint8)((implParam->maskX >> 8) & 0x00FF);
    SaveFncDataBuffer[5] = (uint8)(implParam->maskY & 0x00FF);
    SaveFncDataBuffer[6] = (uint8)((implParam->maskY >> 8) & 0x00FF);
    SaveFncDataBuffer[7] = (uint8)(implParam->maskZ & 0x00FF);
    SaveFncDataBuffer[8] = (uint8)((implParam->maskZ >> 8) & 0x00FF);
    SaveFncDataBuffer[9] = (uint8)(implParam->idxOffsetX & 0x00FF);
    SaveFncDataBuffer[10] = (uint8)((implParam->idxOffsetX >> 8) & 0x00FF);
    SaveFncDataBuffer[11] = (uint8)(implParam->idxOffsetY & 0x00FF);
    SaveFncDataBuffer[12] = (uint8)((implParam->idxOffsetY >> 8) & 0x00FF);
    SaveFncDataBuffer[13] = (uint8)(implParam->idxOffsetZ & 0x00FF);
    SaveFncDataBuffer[14] = (uint8)((implParam->idxOffsetZ >> 8) & 0x00FF);
    SaveFncDataBuffer[15] = (uint8)(implParam->sizeX & 0x00FF);
    SaveFncDataBuffer[16] = (uint8)((implParam->sizeX >> 8) & 0x00FF);
    SaveFncDataBuffer[17] = (uint8)(implParam->sizeY & 0x00FF);
    SaveFncDataBuffer[18] = (uint8)((implParam->sizeY >> 8) & 0x00FF);
    SaveFncDataBuffer[19] = (uint8)(implParam->sizeXY & 0x00FF);
    SaveFncDataBuffer[20] = (uint8)((implParam->sizeXY >> 8) & 0x00FF);
    SaveFncDataBuffer[21] = (uint8)(implParam->gainX & 0x00FF);
    SaveFncDataBuffer[22] = (uint8)((implParam->gainX >> 8) & 0x00FF);
    SaveFncDataBuffer[23] = (uint8)(implParam->gainY & 0x00FF);
    SaveFncDataBuffer[24] = (uint8)((implParam->gainY >> 8) & 0x00FF);
    SaveFncDataBuffer[25] = (uint8)(implParam->gainZ & 0x00FF);
    SaveFncDataBuffer[26] = (uint8)((implParam->gainZ >> 8) & 0x00FF);
    SaveFncDataBuffer[27] = (uint8)implParam->gainXsfr;
    SaveFncDataBuffer[28] = (uint8)implParam->gainYsfr;
    SaveFncDataBuffer[29] = (uint8)implParam->gainZsfr;
    SaveFncDataBuffer[30] = (uint8)(implParam->offsetX & 0x00FF);
    SaveFncDataBuffer[31] = (uint8)((implParam->offsetX >> 8) & 0x00FF);
    SaveFncDataBuffer[32] = (uint8)(implParam->offsetY & 0x00FF);
    SaveFncDataBuffer[33] = (uint8)((implParam->offsetY >> 8) & 0x00FF);
    SaveFncDataBuffer[34] = (uint8)(implParam->offsetZ & 0x00FF);
    SaveFncDataBuffer[35] = (uint8)((implParam->offsetZ >> 8) & 0x00FF);
    SaveFncDataBuffer[36] = (uint8)(implParam->minX & 0x00FF);
    SaveFncDataBuffer[37] = (uint8)((implParam->minX >> 8) & 0x00FF);
    SaveFncDataBuffer[38] = (uint8)(implParam->maxX & 0x00FF);
    SaveFncDataBuffer[39] = (uint8)((implParam->maxX >> 8) & 0x00FF);
    SaveFncDataBuffer[40] = (uint8)(implParam->minY & 0x00FF);
    SaveFncDataBuffer[41] = (uint8)((implParam->minY >> 8) & 0x00FF);
    SaveFncDataBuffer[42] = (uint8)(implParam->maxY & 0x00FF);
    SaveFncDataBuffer[43] = (uint8)((implParam->maxY >> 8) & 0x00FF);
    SaveFncDataBuffer[44] = (uint8)(implParam->minZ & 0x00FF);
    SaveFncDataBuffer[45] = (uint8)((implParam->minZ >> 8) & 0x00FF);
    SaveFncDataBuffer[46] = (uint8)(implParam->maxZ & 0x00FF);
    SaveFncDataBuffer[47] = (uint8)((implParam->maxZ >> 8) & 0x00FF);

    /** - Execute save function of block */
    error = LookupTable3D_FiP16_Save(block, SaveFncDataBuffer, 48);

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
uint8 LookupTable3D_FiP16_ConvertMP(LOOKUPTABLE3D_FIP16 *block, LOOKUPTABLE3D_FIP16_MASK_PARAM *maskParam)
{
    uint8 error;
    LOOKUPTABLE3D_FIP16_IMPL_PARAM *implParam = (LOOKUPTABLE3D_FIP16_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);
    if (error != 0U)
    {
        return (error);
    }

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)implParam->sfrX;
    SaveFncDataBuffer[1] = (uint8)implParam->sfrY;
    SaveFncDataBuffer[2] = (uint8)implParam->sfrZ;
    SaveFncDataBuffer[3] = (uint8)(implParam->maskX & 0x00FF);
    SaveFncDataBuffer[4] = (uint8)((implParam->maskX >> 8) & 0x00FF);
    SaveFncDataBuffer[5] = (uint8)(implParam->maskY & 0x00FF);
    SaveFncDataBuffer[6] = (uint8)((implParam->maskY >> 8) & 0x00FF);
    SaveFncDataBuffer[7] = (uint8)(implParam->maskZ & 0x00FF);
    SaveFncDataBuffer[8] = (uint8)((implParam->maskZ >> 8) & 0x00FF);
    SaveFncDataBuffer[9] = (uint8)(implParam->idxOffsetX & 0x00FF);
    SaveFncDataBuffer[10] = (uint8)((implParam->idxOffsetX >> 8) & 0x00FF);
    SaveFncDataBuffer[11] = (uint8)(implParam->idxOffsetY & 0x00FF);
    SaveFncDataBuffer[12] = (uint8)((implParam->idxOffsetY >> 8) & 0x00FF);
    SaveFncDataBuffer[13] = (uint8)(implParam->idxOffsetZ & 0x00FF);
    SaveFncDataBuffer[14] = (uint8)((implParam->idxOffsetZ >> 8) & 0x00FF);
    SaveFncDataBuffer[15] = (uint8)(implParam->sizeX & 0x00FF);
    SaveFncDataBuffer[16] = (uint8)((implParam->sizeX >> 8) & 0x00FF);
    SaveFncDataBuffer[17] = (uint8)(implParam->sizeY & 0x00FF);
    SaveFncDataBuffer[18] = (uint8)((implParam->sizeY >> 8) & 0x00FF);
    SaveFncDataBuffer[19] = (uint8)(implParam->sizeXY & 0x00FF);
    SaveFncDataBuffer[20] = (uint8)((implParam->sizeXY >> 8) & 0x00FF);
    SaveFncDataBuffer[21] = (uint8)(implParam->gainX & 0x00FF);
    SaveFncDataBuffer[22] = (uint8)((implParam->gainX >> 8) & 0x00FF);
    SaveFncDataBuffer[23] = (uint8)(implParam->gainY & 0x00FF);
    SaveFncDataBuffer[24] = (uint8)((implParam->gainY >> 8) & 0x00FF);
    SaveFncDataBuffer[25] = (uint8)(implParam->gainZ & 0x00FF);
    SaveFncDataBuffer[26] = (uint8)((implParam->gainZ >> 8) & 0x00FF);
    SaveFncDataBuffer[27] = (uint8)implParam->gainXsfr;
    SaveFncDataBuffer[28] = (uint8)implParam->gainYsfr;
    SaveFncDataBuffer[29] = (uint8)implParam->gainZsfr;
    SaveFncDataBuffer[30] = (uint8)(implParam->offsetX & 0x00FF);
    SaveFncDataBuffer[31] = (uint8)((implParam->offsetX >> 8) & 0x00FF);
    SaveFncDataBuffer[32] = (uint8)(implParam->offsetY & 0x00FF);
    SaveFncDataBuffer[33] = (uint8)((implParam->offsetY >> 8) & 0x00FF);
    SaveFncDataBuffer[34] = (uint8)(implParam->offsetZ & 0x00FF);
    SaveFncDataBuffer[35] = (uint8)((implParam->offsetZ >> 8) & 0x00FF);
    SaveFncDataBuffer[36] = (uint8)(implParam->minX & 0x00FF);
    SaveFncDataBuffer[37] = (uint8)((implParam->minX >> 8) & 0x00FF);
    SaveFncDataBuffer[38] = (uint8)(implParam->maxX & 0x00FF);
    SaveFncDataBuffer[39] = (uint8)((implParam->maxX >> 8) & 0x00FF);
    SaveFncDataBuffer[40] = (uint8)(implParam->minY & 0x00FF);
    SaveFncDataBuffer[41] = (uint8)((implParam->minY >> 8) & 0x00FF);
    SaveFncDataBuffer[42] = (uint8)(implParam->maxY & 0x00FF);
    SaveFncDataBuffer[43] = (uint8)((implParam->maxY >> 8) & 0x00FF);
    SaveFncDataBuffer[44] = (uint8)(implParam->minZ & 0x00FF);
    SaveFncDataBuffer[45] = (uint8)((implParam->minZ >> 8) & 0x00FF);
    SaveFncDataBuffer[46] = (uint8)(implParam->maxZ & 0x00FF);
    SaveFncDataBuffer[47] = (uint8)((implParam->maxZ >> 8) & 0x00FF);

    /** - Execute save function of block */
    error = LookupTable3D_FiP16_Save(block, SaveFncDataBuffer, 48);

    return (error);
}

/**
 * @brief Backups current Mask parameters into global Mask parameter backup array.
 *
 * @param[in] maskParam Mask parameters
 *
 * @return Nothing
 */
void LookupTable3D_FiP16_BackupMP(LOOKUPTABLE3D_FIP16_MASK_PARAM* maskParam)
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
void LookupTable3D_FiP16_RestoreMP(LOOKUPTABLE3D_FIP16_MASK_PARAM* maskParam)
{
    memcpy(maskParam, MaskParamBuffer, sizeof(*maskParam));
}
