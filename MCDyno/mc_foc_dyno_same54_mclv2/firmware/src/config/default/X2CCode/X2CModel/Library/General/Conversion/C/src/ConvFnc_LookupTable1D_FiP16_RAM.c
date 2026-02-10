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
#include "ConvFnc_LookupTable1D_FiP16_RAM.h"

/* USERCODE-BEGIN:PreProcessor                                                                                        */
#define MAX_DIM     257U
/* USERCODE-END:PreProcessor                                                                                          */

extern uint8 MaskParamBuffer[];
extern uint8 ImplParamBuffer[];
extern uint8 SaveFncDataBuffer[];

/** Implementation parameter data structure */
typedef struct {
/*  ARRAYS CURRENTLY NOT SUPPORTED                                                                                    */
    int16* Table;
    uint8 sfrX;
    uint16 maskX;
    uint16 idxOffsetX;
    int16 gainX;
    uint8 gainXsfr;
    int16 offsetX;
    int16 minX;
    int16 maxX;
} LOOKUPTABLE1D_FIP16_RAM_IMPL_PARAM;

/** DimX enumeration */
typedef enum {
    DIMX_17 = 0,
    DIMX_33 = 1,
    DIMX_65 = 2,
    DIMX_129 = 3,
    DIMX_257 = 4,
    DIMX_513 = 5,
    DIMX_1025 = 6,
    DIMX_2049 = 7,
    DIMX_4097 = 8
} tDimX;

/** Private prototypes */
static uint8 convertM2I(const LOOKUPTABLE1D_FIP16_RAM_MASK_PARAM *maskParam, LOOKUPTABLE1D_FIP16_RAM_IMPL_PARAM *impParam);

/**
 * @brief Converts Mask parameters to Implementation parameters.
 *
 * @param[in] maskParam Mask parameters
 * @param[out] impParam Implementation parameters
 *
 * @return Error: zero on success, not zero in case of conversion error
 */
static uint8 convertM2I(const LOOKUPTABLE1D_FIP16_RAM_MASK_PARAM *maskParam, LOOKUPTABLE1D_FIP16_RAM_IMPL_PARAM *impParam)
{
    uint8 error = 0U;
/* USERCODE-BEGIN:Conversion                                                                                          */
#if defined(ARRAYS_IN_COT_ARE_SUPPORTED)
#else
    error = 1;
#endif
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
uint8 LookupTable1D_FiP16_RAM_LoadMP(const LOOKUPTABLE1D_FIP16_RAM_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize)
{
    uint8 error = 0U;
    if (25U > maxSize)
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
        *dataLen = 25U;
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
uint8 LookupTable1D_FiP16_RAM_SaveMP(LOOKUPTABLE1D_FIP16_RAM *block, LOOKUPTABLE1D_FIP16_RAM_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen)
{
    uint8 error = 0U;

    if (dataLen != 25U)
    {
        error = 1U;
    }
    else
    {
        uint64 tmp64;
        /** - Cache old mask parameters */
        LookupTable1D_FiP16_RAM_BackupMP(maskParam);

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

        error = LookupTable1D_FiP16_RAM_ConvertMP(block, maskParam);

        /** - Check for errors during execution of save function */
        if (error != 0U)
        {
            /* Restore old mask parameter */
            LookupTable1D_FiP16_RAM_RestoreMP(maskParam);
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
uint8 LookupTable1D_FiP16_RAM_InitMP(LOOKUPTABLE1D_FIP16_RAM *block, const LOOKUPTABLE1D_FIP16_RAM_MASK_PARAM *maskParam)
{
    uint8 error = 0U;

    LOOKUPTABLE1D_FIP16_RAM_IMPL_PARAM *implParam = (LOOKUPTABLE1D_FIP16_RAM_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->Table[0] & 0x00FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->Table[0] >> 8) & 0x00FF);
    SaveFncDataBuffer[2] = (uint8)(implParam->Table[1] & 0x00FF);
    SaveFncDataBuffer[3] = (uint8)((implParam->Table[1] >> 8) & 0x00FF);
    SaveFncDataBuffer[4] = (uint8)(implParam->Table[2] & 0x00FF);
    SaveFncDataBuffer[5] = (uint8)((implParam->Table[2] >> 8) & 0x00FF);
    SaveFncDataBuffer[6] = (uint8)(implParam->Table[3] & 0x00FF);
    SaveFncDataBuffer[7] = (uint8)((implParam->Table[3] >> 8) & 0x00FF);
    SaveFncDataBuffer[8] = (uint8)(implParam->Table[4] & 0x00FF);
    SaveFncDataBuffer[9] = (uint8)((implParam->Table[4] >> 8) & 0x00FF);
    SaveFncDataBuffer[10] = (uint8)(implParam->Table[5] & 0x00FF);
    SaveFncDataBuffer[11] = (uint8)((implParam->Table[5] >> 8) & 0x00FF);
    SaveFncDataBuffer[12] = (uint8)(implParam->Table[6] & 0x00FF);
    SaveFncDataBuffer[13] = (uint8)((implParam->Table[6] >> 8) & 0x00FF);
    SaveFncDataBuffer[14] = (uint8)(implParam->Table[7] & 0x00FF);
    SaveFncDataBuffer[15] = (uint8)((implParam->Table[7] >> 8) & 0x00FF);
    SaveFncDataBuffer[16] = (uint8)(implParam->Table[8] & 0x00FF);
    SaveFncDataBuffer[17] = (uint8)((implParam->Table[8] >> 8) & 0x00FF);
    SaveFncDataBuffer[18] = (uint8)(implParam->Table[9] & 0x00FF);
    SaveFncDataBuffer[19] = (uint8)((implParam->Table[9] >> 8) & 0x00FF);
    SaveFncDataBuffer[20] = (uint8)(implParam->Table[10] & 0x00FF);
    SaveFncDataBuffer[21] = (uint8)((implParam->Table[10] >> 8) & 0x00FF);
    SaveFncDataBuffer[22] = (uint8)(implParam->Table[11] & 0x00FF);
    SaveFncDataBuffer[23] = (uint8)((implParam->Table[11] >> 8) & 0x00FF);
    SaveFncDataBuffer[24] = (uint8)(implParam->Table[12] & 0x00FF);
    SaveFncDataBuffer[25] = (uint8)((implParam->Table[12] >> 8) & 0x00FF);
    SaveFncDataBuffer[26] = (uint8)(implParam->Table[13] & 0x00FF);
    SaveFncDataBuffer[27] = (uint8)((implParam->Table[13] >> 8) & 0x00FF);
    SaveFncDataBuffer[28] = (uint8)(implParam->Table[14] & 0x00FF);
    SaveFncDataBuffer[29] = (uint8)((implParam->Table[14] >> 8) & 0x00FF);
    SaveFncDataBuffer[30] = (uint8)(implParam->Table[15] & 0x00FF);
    SaveFncDataBuffer[31] = (uint8)((implParam->Table[15] >> 8) & 0x00FF);
    SaveFncDataBuffer[32] = (uint8)(implParam->Table[16] & 0x00FF);
    SaveFncDataBuffer[33] = (uint8)((implParam->Table[16] >> 8) & 0x00FF);
    SaveFncDataBuffer[34] = (uint8)(implParam->Table[17] & 0x00FF);
    SaveFncDataBuffer[35] = (uint8)((implParam->Table[17] >> 8) & 0x00FF);
    SaveFncDataBuffer[36] = (uint8)(implParam->Table[18] & 0x00FF);
    SaveFncDataBuffer[37] = (uint8)((implParam->Table[18] >> 8) & 0x00FF);
    SaveFncDataBuffer[38] = (uint8)(implParam->Table[19] & 0x00FF);
    SaveFncDataBuffer[39] = (uint8)((implParam->Table[19] >> 8) & 0x00FF);
    SaveFncDataBuffer[40] = (uint8)(implParam->Table[20] & 0x00FF);
    SaveFncDataBuffer[41] = (uint8)((implParam->Table[20] >> 8) & 0x00FF);
    SaveFncDataBuffer[42] = (uint8)(implParam->Table[21] & 0x00FF);
    SaveFncDataBuffer[43] = (uint8)((implParam->Table[21] >> 8) & 0x00FF);
    SaveFncDataBuffer[44] = (uint8)(implParam->Table[22] & 0x00FF);
    SaveFncDataBuffer[45] = (uint8)((implParam->Table[22] >> 8) & 0x00FF);
    SaveFncDataBuffer[46] = (uint8)(implParam->Table[23] & 0x00FF);
    SaveFncDataBuffer[47] = (uint8)((implParam->Table[23] >> 8) & 0x00FF);
    SaveFncDataBuffer[48] = (uint8)(implParam->Table[24] & 0x00FF);
    SaveFncDataBuffer[49] = (uint8)((implParam->Table[24] >> 8) & 0x00FF);
    SaveFncDataBuffer[50] = (uint8)(implParam->Table[25] & 0x00FF);
    SaveFncDataBuffer[51] = (uint8)((implParam->Table[25] >> 8) & 0x00FF);
    SaveFncDataBuffer[52] = (uint8)(implParam->Table[26] & 0x00FF);
    SaveFncDataBuffer[53] = (uint8)((implParam->Table[26] >> 8) & 0x00FF);
    SaveFncDataBuffer[54] = (uint8)(implParam->Table[27] & 0x00FF);
    SaveFncDataBuffer[55] = (uint8)((implParam->Table[27] >> 8) & 0x00FF);
    SaveFncDataBuffer[56] = (uint8)(implParam->Table[28] & 0x00FF);
    SaveFncDataBuffer[57] = (uint8)((implParam->Table[28] >> 8) & 0x00FF);
    SaveFncDataBuffer[58] = (uint8)(implParam->Table[29] & 0x00FF);
    SaveFncDataBuffer[59] = (uint8)((implParam->Table[29] >> 8) & 0x00FF);
    SaveFncDataBuffer[60] = (uint8)(implParam->Table[30] & 0x00FF);
    SaveFncDataBuffer[61] = (uint8)((implParam->Table[30] >> 8) & 0x00FF);
    SaveFncDataBuffer[62] = (uint8)(implParam->Table[31] & 0x00FF);
    SaveFncDataBuffer[63] = (uint8)((implParam->Table[31] >> 8) & 0x00FF);
    SaveFncDataBuffer[64] = (uint8)(implParam->Table[32] & 0x00FF);
    SaveFncDataBuffer[65] = (uint8)((implParam->Table[32] >> 8) & 0x00FF);
    SaveFncDataBuffer[66] = (uint8)(implParam->Table[33] & 0x00FF);
    SaveFncDataBuffer[67] = (uint8)((implParam->Table[33] >> 8) & 0x00FF);
    SaveFncDataBuffer[68] = (uint8)(implParam->Table[34] & 0x00FF);
    SaveFncDataBuffer[69] = (uint8)((implParam->Table[34] >> 8) & 0x00FF);
    SaveFncDataBuffer[70] = (uint8)(implParam->Table[35] & 0x00FF);
    SaveFncDataBuffer[71] = (uint8)((implParam->Table[35] >> 8) & 0x00FF);
    SaveFncDataBuffer[72] = (uint8)(implParam->Table[36] & 0x00FF);
    SaveFncDataBuffer[73] = (uint8)((implParam->Table[36] >> 8) & 0x00FF);
    SaveFncDataBuffer[74] = (uint8)(implParam->Table[37] & 0x00FF);
    SaveFncDataBuffer[75] = (uint8)((implParam->Table[37] >> 8) & 0x00FF);
    SaveFncDataBuffer[76] = (uint8)(implParam->Table[38] & 0x00FF);
    SaveFncDataBuffer[77] = (uint8)((implParam->Table[38] >> 8) & 0x00FF);
    SaveFncDataBuffer[78] = (uint8)(implParam->Table[39] & 0x00FF);
    SaveFncDataBuffer[79] = (uint8)((implParam->Table[39] >> 8) & 0x00FF);
    SaveFncDataBuffer[80] = (uint8)(implParam->Table[40] & 0x00FF);
    SaveFncDataBuffer[81] = (uint8)((implParam->Table[40] >> 8) & 0x00FF);
    SaveFncDataBuffer[82] = (uint8)(implParam->Table[41] & 0x00FF);
    SaveFncDataBuffer[83] = (uint8)((implParam->Table[41] >> 8) & 0x00FF);
    SaveFncDataBuffer[84] = (uint8)(implParam->Table[42] & 0x00FF);
    SaveFncDataBuffer[85] = (uint8)((implParam->Table[42] >> 8) & 0x00FF);
    SaveFncDataBuffer[86] = (uint8)(implParam->Table[43] & 0x00FF);
    SaveFncDataBuffer[87] = (uint8)((implParam->Table[43] >> 8) & 0x00FF);
    SaveFncDataBuffer[88] = (uint8)(implParam->Table[44] & 0x00FF);
    SaveFncDataBuffer[89] = (uint8)((implParam->Table[44] >> 8) & 0x00FF);
    SaveFncDataBuffer[90] = (uint8)(implParam->Table[45] & 0x00FF);
    SaveFncDataBuffer[91] = (uint8)((implParam->Table[45] >> 8) & 0x00FF);
    SaveFncDataBuffer[92] = (uint8)(implParam->Table[46] & 0x00FF);
    SaveFncDataBuffer[93] = (uint8)((implParam->Table[46] >> 8) & 0x00FF);
    SaveFncDataBuffer[94] = (uint8)(implParam->Table[47] & 0x00FF);
    SaveFncDataBuffer[95] = (uint8)((implParam->Table[47] >> 8) & 0x00FF);
    SaveFncDataBuffer[96] = (uint8)(implParam->Table[48] & 0x00FF);
    SaveFncDataBuffer[97] = (uint8)((implParam->Table[48] >> 8) & 0x00FF);
    SaveFncDataBuffer[98] = (uint8)(implParam->Table[49] & 0x00FF);
    SaveFncDataBuffer[99] = (uint8)((implParam->Table[49] >> 8) & 0x00FF);
    SaveFncDataBuffer[100] = (uint8)(implParam->Table[50] & 0x00FF);
    SaveFncDataBuffer[101] = (uint8)((implParam->Table[50] >> 8) & 0x00FF);
    SaveFncDataBuffer[102] = (uint8)(implParam->Table[51] & 0x00FF);
    SaveFncDataBuffer[103] = (uint8)((implParam->Table[51] >> 8) & 0x00FF);
    SaveFncDataBuffer[104] = (uint8)(implParam->Table[52] & 0x00FF);
    SaveFncDataBuffer[105] = (uint8)((implParam->Table[52] >> 8) & 0x00FF);
    SaveFncDataBuffer[106] = (uint8)(implParam->Table[53] & 0x00FF);
    SaveFncDataBuffer[107] = (uint8)((implParam->Table[53] >> 8) & 0x00FF);
    SaveFncDataBuffer[108] = (uint8)(implParam->Table[54] & 0x00FF);
    SaveFncDataBuffer[109] = (uint8)((implParam->Table[54] >> 8) & 0x00FF);
    SaveFncDataBuffer[110] = (uint8)(implParam->Table[55] & 0x00FF);
    SaveFncDataBuffer[111] = (uint8)((implParam->Table[55] >> 8) & 0x00FF);
    SaveFncDataBuffer[112] = (uint8)(implParam->Table[56] & 0x00FF);
    SaveFncDataBuffer[113] = (uint8)((implParam->Table[56] >> 8) & 0x00FF);
    SaveFncDataBuffer[114] = (uint8)(implParam->Table[57] & 0x00FF);
    SaveFncDataBuffer[115] = (uint8)((implParam->Table[57] >> 8) & 0x00FF);
    SaveFncDataBuffer[116] = (uint8)(implParam->Table[58] & 0x00FF);
    SaveFncDataBuffer[117] = (uint8)((implParam->Table[58] >> 8) & 0x00FF);
    SaveFncDataBuffer[118] = (uint8)(implParam->Table[59] & 0x00FF);
    SaveFncDataBuffer[119] = (uint8)((implParam->Table[59] >> 8) & 0x00FF);
    SaveFncDataBuffer[120] = (uint8)(implParam->Table[60] & 0x00FF);
    SaveFncDataBuffer[121] = (uint8)((implParam->Table[60] >> 8) & 0x00FF);
    SaveFncDataBuffer[122] = (uint8)(implParam->Table[61] & 0x00FF);
    SaveFncDataBuffer[123] = (uint8)((implParam->Table[61] >> 8) & 0x00FF);
    SaveFncDataBuffer[124] = (uint8)(implParam->Table[62] & 0x00FF);
    SaveFncDataBuffer[125] = (uint8)((implParam->Table[62] >> 8) & 0x00FF);
    SaveFncDataBuffer[126] = (uint8)(implParam->Table[63] & 0x00FF);
    SaveFncDataBuffer[127] = (uint8)((implParam->Table[63] >> 8) & 0x00FF);
    SaveFncDataBuffer[128] = (uint8)(implParam->Table[64] & 0x00FF);
    SaveFncDataBuffer[129] = (uint8)((implParam->Table[64] >> 8) & 0x00FF);
    SaveFncDataBuffer[130] = (uint8)(implParam->Table[65] & 0x00FF);
    SaveFncDataBuffer[131] = (uint8)((implParam->Table[65] >> 8) & 0x00FF);
    SaveFncDataBuffer[132] = (uint8)(implParam->Table[66] & 0x00FF);
    SaveFncDataBuffer[133] = (uint8)((implParam->Table[66] >> 8) & 0x00FF);
    SaveFncDataBuffer[134] = (uint8)(implParam->Table[67] & 0x00FF);
    SaveFncDataBuffer[135] = (uint8)((implParam->Table[67] >> 8) & 0x00FF);
    SaveFncDataBuffer[136] = (uint8)(implParam->Table[68] & 0x00FF);
    SaveFncDataBuffer[137] = (uint8)((implParam->Table[68] >> 8) & 0x00FF);
    SaveFncDataBuffer[138] = (uint8)(implParam->Table[69] & 0x00FF);
    SaveFncDataBuffer[139] = (uint8)((implParam->Table[69] >> 8) & 0x00FF);
    SaveFncDataBuffer[140] = (uint8)(implParam->Table[70] & 0x00FF);
    SaveFncDataBuffer[141] = (uint8)((implParam->Table[70] >> 8) & 0x00FF);
    SaveFncDataBuffer[142] = (uint8)(implParam->Table[71] & 0x00FF);
    SaveFncDataBuffer[143] = (uint8)((implParam->Table[71] >> 8) & 0x00FF);
    SaveFncDataBuffer[144] = (uint8)(implParam->Table[72] & 0x00FF);
    SaveFncDataBuffer[145] = (uint8)((implParam->Table[72] >> 8) & 0x00FF);
    SaveFncDataBuffer[146] = (uint8)(implParam->Table[73] & 0x00FF);
    SaveFncDataBuffer[147] = (uint8)((implParam->Table[73] >> 8) & 0x00FF);
    SaveFncDataBuffer[148] = (uint8)(implParam->Table[74] & 0x00FF);
    SaveFncDataBuffer[149] = (uint8)((implParam->Table[74] >> 8) & 0x00FF);
    SaveFncDataBuffer[150] = (uint8)(implParam->Table[75] & 0x00FF);
    SaveFncDataBuffer[151] = (uint8)((implParam->Table[75] >> 8) & 0x00FF);
    SaveFncDataBuffer[152] = (uint8)(implParam->Table[76] & 0x00FF);
    SaveFncDataBuffer[153] = (uint8)((implParam->Table[76] >> 8) & 0x00FF);
    SaveFncDataBuffer[154] = (uint8)(implParam->Table[77] & 0x00FF);
    SaveFncDataBuffer[155] = (uint8)((implParam->Table[77] >> 8) & 0x00FF);
    SaveFncDataBuffer[156] = (uint8)(implParam->Table[78] & 0x00FF);
    SaveFncDataBuffer[157] = (uint8)((implParam->Table[78] >> 8) & 0x00FF);
    SaveFncDataBuffer[158] = (uint8)(implParam->Table[79] & 0x00FF);
    SaveFncDataBuffer[159] = (uint8)((implParam->Table[79] >> 8) & 0x00FF);
    SaveFncDataBuffer[160] = (uint8)(implParam->Table[80] & 0x00FF);
    SaveFncDataBuffer[161] = (uint8)((implParam->Table[80] >> 8) & 0x00FF);
    SaveFncDataBuffer[162] = (uint8)(implParam->Table[81] & 0x00FF);
    SaveFncDataBuffer[163] = (uint8)((implParam->Table[81] >> 8) & 0x00FF);
    SaveFncDataBuffer[164] = (uint8)(implParam->Table[82] & 0x00FF);
    SaveFncDataBuffer[165] = (uint8)((implParam->Table[82] >> 8) & 0x00FF);
    SaveFncDataBuffer[166] = (uint8)(implParam->Table[83] & 0x00FF);
    SaveFncDataBuffer[167] = (uint8)((implParam->Table[83] >> 8) & 0x00FF);
    SaveFncDataBuffer[168] = (uint8)(implParam->Table[84] & 0x00FF);
    SaveFncDataBuffer[169] = (uint8)((implParam->Table[84] >> 8) & 0x00FF);
    SaveFncDataBuffer[170] = (uint8)(implParam->Table[85] & 0x00FF);
    SaveFncDataBuffer[171] = (uint8)((implParam->Table[85] >> 8) & 0x00FF);
    SaveFncDataBuffer[172] = (uint8)(implParam->Table[86] & 0x00FF);
    SaveFncDataBuffer[173] = (uint8)((implParam->Table[86] >> 8) & 0x00FF);
    SaveFncDataBuffer[174] = (uint8)(implParam->Table[87] & 0x00FF);
    SaveFncDataBuffer[175] = (uint8)((implParam->Table[87] >> 8) & 0x00FF);
    SaveFncDataBuffer[176] = (uint8)(implParam->Table[88] & 0x00FF);
    SaveFncDataBuffer[177] = (uint8)((implParam->Table[88] >> 8) & 0x00FF);
    SaveFncDataBuffer[178] = (uint8)(implParam->Table[89] & 0x00FF);
    SaveFncDataBuffer[179] = (uint8)((implParam->Table[89] >> 8) & 0x00FF);
    SaveFncDataBuffer[180] = (uint8)(implParam->Table[90] & 0x00FF);
    SaveFncDataBuffer[181] = (uint8)((implParam->Table[90] >> 8) & 0x00FF);
    SaveFncDataBuffer[182] = (uint8)(implParam->Table[91] & 0x00FF);
    SaveFncDataBuffer[183] = (uint8)((implParam->Table[91] >> 8) & 0x00FF);
    SaveFncDataBuffer[184] = (uint8)(implParam->Table[92] & 0x00FF);
    SaveFncDataBuffer[185] = (uint8)((implParam->Table[92] >> 8) & 0x00FF);
    SaveFncDataBuffer[186] = (uint8)(implParam->Table[93] & 0x00FF);
    SaveFncDataBuffer[187] = (uint8)((implParam->Table[93] >> 8) & 0x00FF);
    SaveFncDataBuffer[188] = (uint8)(implParam->Table[94] & 0x00FF);
    SaveFncDataBuffer[189] = (uint8)((implParam->Table[94] >> 8) & 0x00FF);
    SaveFncDataBuffer[190] = (uint8)(implParam->Table[95] & 0x00FF);
    SaveFncDataBuffer[191] = (uint8)((implParam->Table[95] >> 8) & 0x00FF);
    SaveFncDataBuffer[192] = (uint8)(implParam->Table[96] & 0x00FF);
    SaveFncDataBuffer[193] = (uint8)((implParam->Table[96] >> 8) & 0x00FF);
    SaveFncDataBuffer[194] = (uint8)(implParam->Table[97] & 0x00FF);
    SaveFncDataBuffer[195] = (uint8)((implParam->Table[97] >> 8) & 0x00FF);
    SaveFncDataBuffer[196] = (uint8)(implParam->Table[98] & 0x00FF);
    SaveFncDataBuffer[197] = (uint8)((implParam->Table[98] >> 8) & 0x00FF);
    SaveFncDataBuffer[198] = (uint8)(implParam->Table[99] & 0x00FF);
    SaveFncDataBuffer[199] = (uint8)((implParam->Table[99] >> 8) & 0x00FF);
    SaveFncDataBuffer[200] = (uint8)(implParam->Table[100] & 0x00FF);
    SaveFncDataBuffer[201] = (uint8)((implParam->Table[100] >> 8) & 0x00FF);
    SaveFncDataBuffer[202] = (uint8)(implParam->Table[101] & 0x00FF);
    SaveFncDataBuffer[203] = (uint8)((implParam->Table[101] >> 8) & 0x00FF);
    SaveFncDataBuffer[204] = (uint8)(implParam->Table[102] & 0x00FF);
    SaveFncDataBuffer[205] = (uint8)((implParam->Table[102] >> 8) & 0x00FF);
    SaveFncDataBuffer[206] = (uint8)(implParam->Table[103] & 0x00FF);
    SaveFncDataBuffer[207] = (uint8)((implParam->Table[103] >> 8) & 0x00FF);
    SaveFncDataBuffer[208] = (uint8)(implParam->Table[104] & 0x00FF);
    SaveFncDataBuffer[209] = (uint8)((implParam->Table[104] >> 8) & 0x00FF);
    SaveFncDataBuffer[210] = (uint8)(implParam->Table[105] & 0x00FF);
    SaveFncDataBuffer[211] = (uint8)((implParam->Table[105] >> 8) & 0x00FF);
    SaveFncDataBuffer[212] = (uint8)(implParam->Table[106] & 0x00FF);
    SaveFncDataBuffer[213] = (uint8)((implParam->Table[106] >> 8) & 0x00FF);
    SaveFncDataBuffer[214] = (uint8)(implParam->Table[107] & 0x00FF);
    SaveFncDataBuffer[215] = (uint8)((implParam->Table[107] >> 8) & 0x00FF);
    SaveFncDataBuffer[216] = (uint8)(implParam->Table[108] & 0x00FF);
    SaveFncDataBuffer[217] = (uint8)((implParam->Table[108] >> 8) & 0x00FF);
    SaveFncDataBuffer[218] = (uint8)(implParam->Table[109] & 0x00FF);
    SaveFncDataBuffer[219] = (uint8)((implParam->Table[109] >> 8) & 0x00FF);
    SaveFncDataBuffer[220] = (uint8)(implParam->Table[110] & 0x00FF);
    SaveFncDataBuffer[221] = (uint8)((implParam->Table[110] >> 8) & 0x00FF);
    SaveFncDataBuffer[222] = (uint8)(implParam->Table[111] & 0x00FF);
    SaveFncDataBuffer[223] = (uint8)((implParam->Table[111] >> 8) & 0x00FF);
    SaveFncDataBuffer[224] = (uint8)(implParam->Table[112] & 0x00FF);
    SaveFncDataBuffer[225] = (uint8)((implParam->Table[112] >> 8) & 0x00FF);
    SaveFncDataBuffer[226] = (uint8)(implParam->Table[113] & 0x00FF);
    SaveFncDataBuffer[227] = (uint8)((implParam->Table[113] >> 8) & 0x00FF);
    SaveFncDataBuffer[228] = (uint8)(implParam->Table[114] & 0x00FF);
    SaveFncDataBuffer[229] = (uint8)((implParam->Table[114] >> 8) & 0x00FF);
    SaveFncDataBuffer[230] = (uint8)(implParam->Table[115] & 0x00FF);
    SaveFncDataBuffer[231] = (uint8)((implParam->Table[115] >> 8) & 0x00FF);
    SaveFncDataBuffer[232] = (uint8)(implParam->Table[116] & 0x00FF);
    SaveFncDataBuffer[233] = (uint8)((implParam->Table[116] >> 8) & 0x00FF);
    SaveFncDataBuffer[234] = (uint8)(implParam->Table[117] & 0x00FF);
    SaveFncDataBuffer[235] = (uint8)((implParam->Table[117] >> 8) & 0x00FF);
    SaveFncDataBuffer[236] = (uint8)(implParam->Table[118] & 0x00FF);
    SaveFncDataBuffer[237] = (uint8)((implParam->Table[118] >> 8) & 0x00FF);
    SaveFncDataBuffer[238] = (uint8)(implParam->Table[119] & 0x00FF);
    SaveFncDataBuffer[239] = (uint8)((implParam->Table[119] >> 8) & 0x00FF);
    SaveFncDataBuffer[240] = (uint8)(implParam->Table[120] & 0x00FF);
    SaveFncDataBuffer[241] = (uint8)((implParam->Table[120] >> 8) & 0x00FF);
    SaveFncDataBuffer[242] = (uint8)(implParam->Table[121] & 0x00FF);
    SaveFncDataBuffer[243] = (uint8)((implParam->Table[121] >> 8) & 0x00FF);
    SaveFncDataBuffer[244] = (uint8)(implParam->Table[122] & 0x00FF);
    SaveFncDataBuffer[245] = (uint8)((implParam->Table[122] >> 8) & 0x00FF);
    SaveFncDataBuffer[246] = (uint8)(implParam->Table[123] & 0x00FF);
    SaveFncDataBuffer[247] = (uint8)((implParam->Table[123] >> 8) & 0x00FF);
    SaveFncDataBuffer[248] = (uint8)(implParam->Table[124] & 0x00FF);
    SaveFncDataBuffer[249] = (uint8)((implParam->Table[124] >> 8) & 0x00FF);
    SaveFncDataBuffer[250] = (uint8)(implParam->Table[125] & 0x00FF);
    SaveFncDataBuffer[251] = (uint8)((implParam->Table[125] >> 8) & 0x00FF);
    SaveFncDataBuffer[252] = (uint8)(implParam->Table[126] & 0x00FF);
    SaveFncDataBuffer[253] = (uint8)((implParam->Table[126] >> 8) & 0x00FF);
    SaveFncDataBuffer[254] = (uint8)(implParam->Table[127] & 0x00FF);
    SaveFncDataBuffer[255] = (uint8)((implParam->Table[127] >> 8) & 0x00FF);
    SaveFncDataBuffer[256] = (uint8)(implParam->Table[128] & 0x00FF);
    SaveFncDataBuffer[257] = (uint8)((implParam->Table[128] >> 8) & 0x00FF);
    SaveFncDataBuffer[258] = (uint8)(implParam->Table[129] & 0x00FF);
    SaveFncDataBuffer[259] = (uint8)((implParam->Table[129] >> 8) & 0x00FF);
    SaveFncDataBuffer[260] = (uint8)(implParam->Table[130] & 0x00FF);
    SaveFncDataBuffer[261] = (uint8)((implParam->Table[130] >> 8) & 0x00FF);
    SaveFncDataBuffer[262] = (uint8)(implParam->Table[131] & 0x00FF);
    SaveFncDataBuffer[263] = (uint8)((implParam->Table[131] >> 8) & 0x00FF);
    SaveFncDataBuffer[264] = (uint8)(implParam->Table[132] & 0x00FF);
    SaveFncDataBuffer[265] = (uint8)((implParam->Table[132] >> 8) & 0x00FF);
    SaveFncDataBuffer[266] = (uint8)(implParam->Table[133] & 0x00FF);
    SaveFncDataBuffer[267] = (uint8)((implParam->Table[133] >> 8) & 0x00FF);
    SaveFncDataBuffer[268] = (uint8)(implParam->Table[134] & 0x00FF);
    SaveFncDataBuffer[269] = (uint8)((implParam->Table[134] >> 8) & 0x00FF);
    SaveFncDataBuffer[270] = (uint8)(implParam->Table[135] & 0x00FF);
    SaveFncDataBuffer[271] = (uint8)((implParam->Table[135] >> 8) & 0x00FF);
    SaveFncDataBuffer[272] = (uint8)(implParam->Table[136] & 0x00FF);
    SaveFncDataBuffer[273] = (uint8)((implParam->Table[136] >> 8) & 0x00FF);
    SaveFncDataBuffer[274] = (uint8)(implParam->Table[137] & 0x00FF);
    SaveFncDataBuffer[275] = (uint8)((implParam->Table[137] >> 8) & 0x00FF);
    SaveFncDataBuffer[276] = (uint8)(implParam->Table[138] & 0x00FF);
    SaveFncDataBuffer[277] = (uint8)((implParam->Table[138] >> 8) & 0x00FF);
    SaveFncDataBuffer[278] = (uint8)(implParam->Table[139] & 0x00FF);
    SaveFncDataBuffer[279] = (uint8)((implParam->Table[139] >> 8) & 0x00FF);
    SaveFncDataBuffer[280] = (uint8)(implParam->Table[140] & 0x00FF);
    SaveFncDataBuffer[281] = (uint8)((implParam->Table[140] >> 8) & 0x00FF);
    SaveFncDataBuffer[282] = (uint8)(implParam->Table[141] & 0x00FF);
    SaveFncDataBuffer[283] = (uint8)((implParam->Table[141] >> 8) & 0x00FF);
    SaveFncDataBuffer[284] = (uint8)(implParam->Table[142] & 0x00FF);
    SaveFncDataBuffer[285] = (uint8)((implParam->Table[142] >> 8) & 0x00FF);
    SaveFncDataBuffer[286] = (uint8)(implParam->Table[143] & 0x00FF);
    SaveFncDataBuffer[287] = (uint8)((implParam->Table[143] >> 8) & 0x00FF);
    SaveFncDataBuffer[288] = (uint8)(implParam->Table[144] & 0x00FF);
    SaveFncDataBuffer[289] = (uint8)((implParam->Table[144] >> 8) & 0x00FF);
    SaveFncDataBuffer[290] = (uint8)(implParam->Table[145] & 0x00FF);
    SaveFncDataBuffer[291] = (uint8)((implParam->Table[145] >> 8) & 0x00FF);
    SaveFncDataBuffer[292] = (uint8)(implParam->Table[146] & 0x00FF);
    SaveFncDataBuffer[293] = (uint8)((implParam->Table[146] >> 8) & 0x00FF);
    SaveFncDataBuffer[294] = (uint8)(implParam->Table[147] & 0x00FF);
    SaveFncDataBuffer[295] = (uint8)((implParam->Table[147] >> 8) & 0x00FF);
    SaveFncDataBuffer[296] = (uint8)(implParam->Table[148] & 0x00FF);
    SaveFncDataBuffer[297] = (uint8)((implParam->Table[148] >> 8) & 0x00FF);
    SaveFncDataBuffer[298] = (uint8)(implParam->Table[149] & 0x00FF);
    SaveFncDataBuffer[299] = (uint8)((implParam->Table[149] >> 8) & 0x00FF);
    SaveFncDataBuffer[300] = (uint8)(implParam->Table[150] & 0x00FF);
    SaveFncDataBuffer[301] = (uint8)((implParam->Table[150] >> 8) & 0x00FF);
    SaveFncDataBuffer[302] = (uint8)(implParam->Table[151] & 0x00FF);
    SaveFncDataBuffer[303] = (uint8)((implParam->Table[151] >> 8) & 0x00FF);
    SaveFncDataBuffer[304] = (uint8)(implParam->Table[152] & 0x00FF);
    SaveFncDataBuffer[305] = (uint8)((implParam->Table[152] >> 8) & 0x00FF);
    SaveFncDataBuffer[306] = (uint8)(implParam->Table[153] & 0x00FF);
    SaveFncDataBuffer[307] = (uint8)((implParam->Table[153] >> 8) & 0x00FF);
    SaveFncDataBuffer[308] = (uint8)(implParam->Table[154] & 0x00FF);
    SaveFncDataBuffer[309] = (uint8)((implParam->Table[154] >> 8) & 0x00FF);
    SaveFncDataBuffer[310] = (uint8)(implParam->Table[155] & 0x00FF);
    SaveFncDataBuffer[311] = (uint8)((implParam->Table[155] >> 8) & 0x00FF);
    SaveFncDataBuffer[312] = (uint8)(implParam->Table[156] & 0x00FF);
    SaveFncDataBuffer[313] = (uint8)((implParam->Table[156] >> 8) & 0x00FF);
    SaveFncDataBuffer[314] = (uint8)(implParam->Table[157] & 0x00FF);
    SaveFncDataBuffer[315] = (uint8)((implParam->Table[157] >> 8) & 0x00FF);
    SaveFncDataBuffer[316] = (uint8)(implParam->Table[158] & 0x00FF);
    SaveFncDataBuffer[317] = (uint8)((implParam->Table[158] >> 8) & 0x00FF);
    SaveFncDataBuffer[318] = (uint8)(implParam->Table[159] & 0x00FF);
    SaveFncDataBuffer[319] = (uint8)((implParam->Table[159] >> 8) & 0x00FF);
    SaveFncDataBuffer[320] = (uint8)(implParam->Table[160] & 0x00FF);
    SaveFncDataBuffer[321] = (uint8)((implParam->Table[160] >> 8) & 0x00FF);
    SaveFncDataBuffer[322] = (uint8)(implParam->Table[161] & 0x00FF);
    SaveFncDataBuffer[323] = (uint8)((implParam->Table[161] >> 8) & 0x00FF);
    SaveFncDataBuffer[324] = (uint8)(implParam->Table[162] & 0x00FF);
    SaveFncDataBuffer[325] = (uint8)((implParam->Table[162] >> 8) & 0x00FF);
    SaveFncDataBuffer[326] = (uint8)(implParam->Table[163] & 0x00FF);
    SaveFncDataBuffer[327] = (uint8)((implParam->Table[163] >> 8) & 0x00FF);
    SaveFncDataBuffer[328] = (uint8)(implParam->Table[164] & 0x00FF);
    SaveFncDataBuffer[329] = (uint8)((implParam->Table[164] >> 8) & 0x00FF);
    SaveFncDataBuffer[330] = (uint8)(implParam->Table[165] & 0x00FF);
    SaveFncDataBuffer[331] = (uint8)((implParam->Table[165] >> 8) & 0x00FF);
    SaveFncDataBuffer[332] = (uint8)(implParam->Table[166] & 0x00FF);
    SaveFncDataBuffer[333] = (uint8)((implParam->Table[166] >> 8) & 0x00FF);
    SaveFncDataBuffer[334] = (uint8)(implParam->Table[167] & 0x00FF);
    SaveFncDataBuffer[335] = (uint8)((implParam->Table[167] >> 8) & 0x00FF);
    SaveFncDataBuffer[336] = (uint8)(implParam->Table[168] & 0x00FF);
    SaveFncDataBuffer[337] = (uint8)((implParam->Table[168] >> 8) & 0x00FF);
    SaveFncDataBuffer[338] = (uint8)(implParam->Table[169] & 0x00FF);
    SaveFncDataBuffer[339] = (uint8)((implParam->Table[169] >> 8) & 0x00FF);
    SaveFncDataBuffer[340] = (uint8)(implParam->Table[170] & 0x00FF);
    SaveFncDataBuffer[341] = (uint8)((implParam->Table[170] >> 8) & 0x00FF);
    SaveFncDataBuffer[342] = (uint8)(implParam->Table[171] & 0x00FF);
    SaveFncDataBuffer[343] = (uint8)((implParam->Table[171] >> 8) & 0x00FF);
    SaveFncDataBuffer[344] = (uint8)(implParam->Table[172] & 0x00FF);
    SaveFncDataBuffer[345] = (uint8)((implParam->Table[172] >> 8) & 0x00FF);
    SaveFncDataBuffer[346] = (uint8)(implParam->Table[173] & 0x00FF);
    SaveFncDataBuffer[347] = (uint8)((implParam->Table[173] >> 8) & 0x00FF);
    SaveFncDataBuffer[348] = (uint8)(implParam->Table[174] & 0x00FF);
    SaveFncDataBuffer[349] = (uint8)((implParam->Table[174] >> 8) & 0x00FF);
    SaveFncDataBuffer[350] = (uint8)(implParam->Table[175] & 0x00FF);
    SaveFncDataBuffer[351] = (uint8)((implParam->Table[175] >> 8) & 0x00FF);
    SaveFncDataBuffer[352] = (uint8)(implParam->Table[176] & 0x00FF);
    SaveFncDataBuffer[353] = (uint8)((implParam->Table[176] >> 8) & 0x00FF);
    SaveFncDataBuffer[354] = (uint8)(implParam->Table[177] & 0x00FF);
    SaveFncDataBuffer[355] = (uint8)((implParam->Table[177] >> 8) & 0x00FF);
    SaveFncDataBuffer[356] = (uint8)(implParam->Table[178] & 0x00FF);
    SaveFncDataBuffer[357] = (uint8)((implParam->Table[178] >> 8) & 0x00FF);
    SaveFncDataBuffer[358] = (uint8)(implParam->Table[179] & 0x00FF);
    SaveFncDataBuffer[359] = (uint8)((implParam->Table[179] >> 8) & 0x00FF);
    SaveFncDataBuffer[360] = (uint8)(implParam->Table[180] & 0x00FF);
    SaveFncDataBuffer[361] = (uint8)((implParam->Table[180] >> 8) & 0x00FF);
    SaveFncDataBuffer[362] = (uint8)(implParam->Table[181] & 0x00FF);
    SaveFncDataBuffer[363] = (uint8)((implParam->Table[181] >> 8) & 0x00FF);
    SaveFncDataBuffer[364] = (uint8)(implParam->Table[182] & 0x00FF);
    SaveFncDataBuffer[365] = (uint8)((implParam->Table[182] >> 8) & 0x00FF);
    SaveFncDataBuffer[366] = (uint8)(implParam->Table[183] & 0x00FF);
    SaveFncDataBuffer[367] = (uint8)((implParam->Table[183] >> 8) & 0x00FF);
    SaveFncDataBuffer[368] = (uint8)(implParam->Table[184] & 0x00FF);
    SaveFncDataBuffer[369] = (uint8)((implParam->Table[184] >> 8) & 0x00FF);
    SaveFncDataBuffer[370] = (uint8)(implParam->Table[185] & 0x00FF);
    SaveFncDataBuffer[371] = (uint8)((implParam->Table[185] >> 8) & 0x00FF);
    SaveFncDataBuffer[372] = (uint8)(implParam->Table[186] & 0x00FF);
    SaveFncDataBuffer[373] = (uint8)((implParam->Table[186] >> 8) & 0x00FF);
    SaveFncDataBuffer[374] = (uint8)(implParam->Table[187] & 0x00FF);
    SaveFncDataBuffer[375] = (uint8)((implParam->Table[187] >> 8) & 0x00FF);
    SaveFncDataBuffer[376] = (uint8)(implParam->Table[188] & 0x00FF);
    SaveFncDataBuffer[377] = (uint8)((implParam->Table[188] >> 8) & 0x00FF);
    SaveFncDataBuffer[378] = (uint8)(implParam->Table[189] & 0x00FF);
    SaveFncDataBuffer[379] = (uint8)((implParam->Table[189] >> 8) & 0x00FF);
    SaveFncDataBuffer[380] = (uint8)(implParam->Table[190] & 0x00FF);
    SaveFncDataBuffer[381] = (uint8)((implParam->Table[190] >> 8) & 0x00FF);
    SaveFncDataBuffer[382] = (uint8)(implParam->Table[191] & 0x00FF);
    SaveFncDataBuffer[383] = (uint8)((implParam->Table[191] >> 8) & 0x00FF);
    SaveFncDataBuffer[384] = (uint8)(implParam->Table[192] & 0x00FF);
    SaveFncDataBuffer[385] = (uint8)((implParam->Table[192] >> 8) & 0x00FF);
    SaveFncDataBuffer[386] = (uint8)(implParam->Table[193] & 0x00FF);
    SaveFncDataBuffer[387] = (uint8)((implParam->Table[193] >> 8) & 0x00FF);
    SaveFncDataBuffer[388] = (uint8)(implParam->Table[194] & 0x00FF);
    SaveFncDataBuffer[389] = (uint8)((implParam->Table[194] >> 8) & 0x00FF);
    SaveFncDataBuffer[390] = (uint8)(implParam->Table[195] & 0x00FF);
    SaveFncDataBuffer[391] = (uint8)((implParam->Table[195] >> 8) & 0x00FF);
    SaveFncDataBuffer[392] = (uint8)(implParam->Table[196] & 0x00FF);
    SaveFncDataBuffer[393] = (uint8)((implParam->Table[196] >> 8) & 0x00FF);
    SaveFncDataBuffer[394] = (uint8)(implParam->Table[197] & 0x00FF);
    SaveFncDataBuffer[395] = (uint8)((implParam->Table[197] >> 8) & 0x00FF);
    SaveFncDataBuffer[396] = (uint8)(implParam->Table[198] & 0x00FF);
    SaveFncDataBuffer[397] = (uint8)((implParam->Table[198] >> 8) & 0x00FF);
    SaveFncDataBuffer[398] = (uint8)(implParam->Table[199] & 0x00FF);
    SaveFncDataBuffer[399] = (uint8)((implParam->Table[199] >> 8) & 0x00FF);
    SaveFncDataBuffer[400] = (uint8)(implParam->Table[200] & 0x00FF);
    SaveFncDataBuffer[401] = (uint8)((implParam->Table[200] >> 8) & 0x00FF);
    SaveFncDataBuffer[402] = (uint8)(implParam->Table[201] & 0x00FF);
    SaveFncDataBuffer[403] = (uint8)((implParam->Table[201] >> 8) & 0x00FF);
    SaveFncDataBuffer[404] = (uint8)(implParam->Table[202] & 0x00FF);
    SaveFncDataBuffer[405] = (uint8)((implParam->Table[202] >> 8) & 0x00FF);
    SaveFncDataBuffer[406] = (uint8)(implParam->Table[203] & 0x00FF);
    SaveFncDataBuffer[407] = (uint8)((implParam->Table[203] >> 8) & 0x00FF);
    SaveFncDataBuffer[408] = (uint8)(implParam->Table[204] & 0x00FF);
    SaveFncDataBuffer[409] = (uint8)((implParam->Table[204] >> 8) & 0x00FF);
    SaveFncDataBuffer[410] = (uint8)(implParam->Table[205] & 0x00FF);
    SaveFncDataBuffer[411] = (uint8)((implParam->Table[205] >> 8) & 0x00FF);
    SaveFncDataBuffer[412] = (uint8)(implParam->Table[206] & 0x00FF);
    SaveFncDataBuffer[413] = (uint8)((implParam->Table[206] >> 8) & 0x00FF);
    SaveFncDataBuffer[414] = (uint8)(implParam->Table[207] & 0x00FF);
    SaveFncDataBuffer[415] = (uint8)((implParam->Table[207] >> 8) & 0x00FF);
    SaveFncDataBuffer[416] = (uint8)(implParam->Table[208] & 0x00FF);
    SaveFncDataBuffer[417] = (uint8)((implParam->Table[208] >> 8) & 0x00FF);
    SaveFncDataBuffer[418] = (uint8)(implParam->Table[209] & 0x00FF);
    SaveFncDataBuffer[419] = (uint8)((implParam->Table[209] >> 8) & 0x00FF);
    SaveFncDataBuffer[420] = (uint8)(implParam->Table[210] & 0x00FF);
    SaveFncDataBuffer[421] = (uint8)((implParam->Table[210] >> 8) & 0x00FF);
    SaveFncDataBuffer[422] = (uint8)(implParam->Table[211] & 0x00FF);
    SaveFncDataBuffer[423] = (uint8)((implParam->Table[211] >> 8) & 0x00FF);
    SaveFncDataBuffer[424] = (uint8)(implParam->Table[212] & 0x00FF);
    SaveFncDataBuffer[425] = (uint8)((implParam->Table[212] >> 8) & 0x00FF);
    SaveFncDataBuffer[426] = (uint8)(implParam->Table[213] & 0x00FF);
    SaveFncDataBuffer[427] = (uint8)((implParam->Table[213] >> 8) & 0x00FF);
    SaveFncDataBuffer[428] = (uint8)(implParam->Table[214] & 0x00FF);
    SaveFncDataBuffer[429] = (uint8)((implParam->Table[214] >> 8) & 0x00FF);
    SaveFncDataBuffer[430] = (uint8)(implParam->Table[215] & 0x00FF);
    SaveFncDataBuffer[431] = (uint8)((implParam->Table[215] >> 8) & 0x00FF);
    SaveFncDataBuffer[432] = (uint8)(implParam->Table[216] & 0x00FF);
    SaveFncDataBuffer[433] = (uint8)((implParam->Table[216] >> 8) & 0x00FF);
    SaveFncDataBuffer[434] = (uint8)(implParam->Table[217] & 0x00FF);
    SaveFncDataBuffer[435] = (uint8)((implParam->Table[217] >> 8) & 0x00FF);
    SaveFncDataBuffer[436] = (uint8)(implParam->Table[218] & 0x00FF);
    SaveFncDataBuffer[437] = (uint8)((implParam->Table[218] >> 8) & 0x00FF);
    SaveFncDataBuffer[438] = (uint8)(implParam->Table[219] & 0x00FF);
    SaveFncDataBuffer[439] = (uint8)((implParam->Table[219] >> 8) & 0x00FF);
    SaveFncDataBuffer[440] = (uint8)(implParam->Table[220] & 0x00FF);
    SaveFncDataBuffer[441] = (uint8)((implParam->Table[220] >> 8) & 0x00FF);
    SaveFncDataBuffer[442] = (uint8)(implParam->Table[221] & 0x00FF);
    SaveFncDataBuffer[443] = (uint8)((implParam->Table[221] >> 8) & 0x00FF);
    SaveFncDataBuffer[444] = (uint8)(implParam->Table[222] & 0x00FF);
    SaveFncDataBuffer[445] = (uint8)((implParam->Table[222] >> 8) & 0x00FF);
    SaveFncDataBuffer[446] = (uint8)(implParam->Table[223] & 0x00FF);
    SaveFncDataBuffer[447] = (uint8)((implParam->Table[223] >> 8) & 0x00FF);
    SaveFncDataBuffer[448] = (uint8)(implParam->Table[224] & 0x00FF);
    SaveFncDataBuffer[449] = (uint8)((implParam->Table[224] >> 8) & 0x00FF);
    SaveFncDataBuffer[450] = (uint8)(implParam->Table[225] & 0x00FF);
    SaveFncDataBuffer[451] = (uint8)((implParam->Table[225] >> 8) & 0x00FF);
    SaveFncDataBuffer[452] = (uint8)(implParam->Table[226] & 0x00FF);
    SaveFncDataBuffer[453] = (uint8)((implParam->Table[226] >> 8) & 0x00FF);
    SaveFncDataBuffer[454] = (uint8)(implParam->Table[227] & 0x00FF);
    SaveFncDataBuffer[455] = (uint8)((implParam->Table[227] >> 8) & 0x00FF);
    SaveFncDataBuffer[456] = (uint8)(implParam->Table[228] & 0x00FF);
    SaveFncDataBuffer[457] = (uint8)((implParam->Table[228] >> 8) & 0x00FF);
    SaveFncDataBuffer[458] = (uint8)(implParam->Table[229] & 0x00FF);
    SaveFncDataBuffer[459] = (uint8)((implParam->Table[229] >> 8) & 0x00FF);
    SaveFncDataBuffer[460] = (uint8)(implParam->Table[230] & 0x00FF);
    SaveFncDataBuffer[461] = (uint8)((implParam->Table[230] >> 8) & 0x00FF);
    SaveFncDataBuffer[462] = (uint8)(implParam->Table[231] & 0x00FF);
    SaveFncDataBuffer[463] = (uint8)((implParam->Table[231] >> 8) & 0x00FF);
    SaveFncDataBuffer[464] = (uint8)(implParam->Table[232] & 0x00FF);
    SaveFncDataBuffer[465] = (uint8)((implParam->Table[232] >> 8) & 0x00FF);
    SaveFncDataBuffer[466] = (uint8)(implParam->Table[233] & 0x00FF);
    SaveFncDataBuffer[467] = (uint8)((implParam->Table[233] >> 8) & 0x00FF);
    SaveFncDataBuffer[468] = (uint8)(implParam->Table[234] & 0x00FF);
    SaveFncDataBuffer[469] = (uint8)((implParam->Table[234] >> 8) & 0x00FF);
    SaveFncDataBuffer[470] = (uint8)(implParam->Table[235] & 0x00FF);
    SaveFncDataBuffer[471] = (uint8)((implParam->Table[235] >> 8) & 0x00FF);
    SaveFncDataBuffer[472] = (uint8)(implParam->Table[236] & 0x00FF);
    SaveFncDataBuffer[473] = (uint8)((implParam->Table[236] >> 8) & 0x00FF);
    SaveFncDataBuffer[474] = (uint8)(implParam->Table[237] & 0x00FF);
    SaveFncDataBuffer[475] = (uint8)((implParam->Table[237] >> 8) & 0x00FF);
    SaveFncDataBuffer[476] = (uint8)(implParam->Table[238] & 0x00FF);
    SaveFncDataBuffer[477] = (uint8)((implParam->Table[238] >> 8) & 0x00FF);
    SaveFncDataBuffer[478] = (uint8)(implParam->Table[239] & 0x00FF);
    SaveFncDataBuffer[479] = (uint8)((implParam->Table[239] >> 8) & 0x00FF);
    SaveFncDataBuffer[480] = (uint8)(implParam->Table[240] & 0x00FF);
    SaveFncDataBuffer[481] = (uint8)((implParam->Table[240] >> 8) & 0x00FF);
    SaveFncDataBuffer[482] = (uint8)(implParam->Table[241] & 0x00FF);
    SaveFncDataBuffer[483] = (uint8)((implParam->Table[241] >> 8) & 0x00FF);
    SaveFncDataBuffer[484] = (uint8)(implParam->Table[242] & 0x00FF);
    SaveFncDataBuffer[485] = (uint8)((implParam->Table[242] >> 8) & 0x00FF);
    SaveFncDataBuffer[486] = (uint8)(implParam->Table[243] & 0x00FF);
    SaveFncDataBuffer[487] = (uint8)((implParam->Table[243] >> 8) & 0x00FF);
    SaveFncDataBuffer[488] = (uint8)(implParam->Table[244] & 0x00FF);
    SaveFncDataBuffer[489] = (uint8)((implParam->Table[244] >> 8) & 0x00FF);
    SaveFncDataBuffer[490] = (uint8)(implParam->Table[245] & 0x00FF);
    SaveFncDataBuffer[491] = (uint8)((implParam->Table[245] >> 8) & 0x00FF);
    SaveFncDataBuffer[492] = (uint8)(implParam->Table[246] & 0x00FF);
    SaveFncDataBuffer[493] = (uint8)((implParam->Table[246] >> 8) & 0x00FF);
    SaveFncDataBuffer[494] = (uint8)(implParam->Table[247] & 0x00FF);
    SaveFncDataBuffer[495] = (uint8)((implParam->Table[247] >> 8) & 0x00FF);
    SaveFncDataBuffer[496] = (uint8)(implParam->Table[248] & 0x00FF);
    SaveFncDataBuffer[497] = (uint8)((implParam->Table[248] >> 8) & 0x00FF);
    SaveFncDataBuffer[498] = (uint8)(implParam->Table[249] & 0x00FF);
    SaveFncDataBuffer[499] = (uint8)((implParam->Table[249] >> 8) & 0x00FF);
    SaveFncDataBuffer[500] = (uint8)(implParam->Table[250] & 0x00FF);
    SaveFncDataBuffer[501] = (uint8)((implParam->Table[250] >> 8) & 0x00FF);
    SaveFncDataBuffer[502] = (uint8)(implParam->Table[251] & 0x00FF);
    SaveFncDataBuffer[503] = (uint8)((implParam->Table[251] >> 8) & 0x00FF);
    SaveFncDataBuffer[504] = (uint8)(implParam->Table[252] & 0x00FF);
    SaveFncDataBuffer[505] = (uint8)((implParam->Table[252] >> 8) & 0x00FF);
    SaveFncDataBuffer[506] = (uint8)(implParam->Table[253] & 0x00FF);
    SaveFncDataBuffer[507] = (uint8)((implParam->Table[253] >> 8) & 0x00FF);
    SaveFncDataBuffer[508] = (uint8)(implParam->Table[254] & 0x00FF);
    SaveFncDataBuffer[509] = (uint8)((implParam->Table[254] >> 8) & 0x00FF);
    SaveFncDataBuffer[510] = (uint8)(implParam->Table[255] & 0x00FF);
    SaveFncDataBuffer[511] = (uint8)((implParam->Table[255] >> 8) & 0x00FF);
    SaveFncDataBuffer[512] = (uint8)(implParam->Table[256] & 0x00FF);
    SaveFncDataBuffer[513] = (uint8)((implParam->Table[256] >> 8) & 0x00FF);
    SaveFncDataBuffer[514] = (uint8)implParam->sfrX;
    SaveFncDataBuffer[515] = (uint8)(implParam->maskX & 0x00FF);
    SaveFncDataBuffer[516] = (uint8)((implParam->maskX >> 8) & 0x00FF);
    SaveFncDataBuffer[517] = (uint8)(implParam->idxOffsetX & 0x00FF);
    SaveFncDataBuffer[518] = (uint8)((implParam->idxOffsetX >> 8) & 0x00FF);
    SaveFncDataBuffer[519] = (uint8)(implParam->gainX & 0x00FF);
    SaveFncDataBuffer[520] = (uint8)((implParam->gainX >> 8) & 0x00FF);
    SaveFncDataBuffer[521] = (uint8)implParam->gainXsfr;
    SaveFncDataBuffer[522] = (uint8)(implParam->offsetX & 0x00FF);
    SaveFncDataBuffer[523] = (uint8)((implParam->offsetX >> 8) & 0x00FF);
    SaveFncDataBuffer[524] = (uint8)(implParam->minX & 0x00FF);
    SaveFncDataBuffer[525] = (uint8)((implParam->minX >> 8) & 0x00FF);
    SaveFncDataBuffer[526] = (uint8)(implParam->maxX & 0x00FF);
    SaveFncDataBuffer[527] = (uint8)((implParam->maxX >> 8) & 0x00FF);

    /** - Execute save function of block */
    error = LookupTable1D_FiP16_RAM_Save(block, SaveFncDataBuffer, 528);

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
uint8 LookupTable1D_FiP16_RAM_ConvertMP(LOOKUPTABLE1D_FIP16_RAM *block, LOOKUPTABLE1D_FIP16_RAM_MASK_PARAM *maskParam)
{
    uint8 error;
    LOOKUPTABLE1D_FIP16_RAM_IMPL_PARAM *implParam = (LOOKUPTABLE1D_FIP16_RAM_IMPL_PARAM*)ImplParamBuffer;

    /** - Convert mask parameters to implementation parameters */
    error = convertM2I(maskParam, implParam);
    if (error != 0U)
    {
        return (error);
    }

    /** - Prepare implementation parameters for save function */
    SaveFncDataBuffer[0] = (uint8)(implParam->Table[0] & 0x00FF);
    SaveFncDataBuffer[1] = (uint8)((implParam->Table[0] >> 8) & 0x00FF);
    SaveFncDataBuffer[2] = (uint8)(implParam->Table[1] & 0x00FF);
    SaveFncDataBuffer[3] = (uint8)((implParam->Table[1] >> 8) & 0x00FF);
    SaveFncDataBuffer[4] = (uint8)(implParam->Table[2] & 0x00FF);
    SaveFncDataBuffer[5] = (uint8)((implParam->Table[2] >> 8) & 0x00FF);
    SaveFncDataBuffer[6] = (uint8)(implParam->Table[3] & 0x00FF);
    SaveFncDataBuffer[7] = (uint8)((implParam->Table[3] >> 8) & 0x00FF);
    SaveFncDataBuffer[8] = (uint8)(implParam->Table[4] & 0x00FF);
    SaveFncDataBuffer[9] = (uint8)((implParam->Table[4] >> 8) & 0x00FF);
    SaveFncDataBuffer[10] = (uint8)(implParam->Table[5] & 0x00FF);
    SaveFncDataBuffer[11] = (uint8)((implParam->Table[5] >> 8) & 0x00FF);
    SaveFncDataBuffer[12] = (uint8)(implParam->Table[6] & 0x00FF);
    SaveFncDataBuffer[13] = (uint8)((implParam->Table[6] >> 8) & 0x00FF);
    SaveFncDataBuffer[14] = (uint8)(implParam->Table[7] & 0x00FF);
    SaveFncDataBuffer[15] = (uint8)((implParam->Table[7] >> 8) & 0x00FF);
    SaveFncDataBuffer[16] = (uint8)(implParam->Table[8] & 0x00FF);
    SaveFncDataBuffer[17] = (uint8)((implParam->Table[8] >> 8) & 0x00FF);
    SaveFncDataBuffer[18] = (uint8)(implParam->Table[9] & 0x00FF);
    SaveFncDataBuffer[19] = (uint8)((implParam->Table[9] >> 8) & 0x00FF);
    SaveFncDataBuffer[20] = (uint8)(implParam->Table[10] & 0x00FF);
    SaveFncDataBuffer[21] = (uint8)((implParam->Table[10] >> 8) & 0x00FF);
    SaveFncDataBuffer[22] = (uint8)(implParam->Table[11] & 0x00FF);
    SaveFncDataBuffer[23] = (uint8)((implParam->Table[11] >> 8) & 0x00FF);
    SaveFncDataBuffer[24] = (uint8)(implParam->Table[12] & 0x00FF);
    SaveFncDataBuffer[25] = (uint8)((implParam->Table[12] >> 8) & 0x00FF);
    SaveFncDataBuffer[26] = (uint8)(implParam->Table[13] & 0x00FF);
    SaveFncDataBuffer[27] = (uint8)((implParam->Table[13] >> 8) & 0x00FF);
    SaveFncDataBuffer[28] = (uint8)(implParam->Table[14] & 0x00FF);
    SaveFncDataBuffer[29] = (uint8)((implParam->Table[14] >> 8) & 0x00FF);
    SaveFncDataBuffer[30] = (uint8)(implParam->Table[15] & 0x00FF);
    SaveFncDataBuffer[31] = (uint8)((implParam->Table[15] >> 8) & 0x00FF);
    SaveFncDataBuffer[32] = (uint8)(implParam->Table[16] & 0x00FF);
    SaveFncDataBuffer[33] = (uint8)((implParam->Table[16] >> 8) & 0x00FF);
    SaveFncDataBuffer[34] = (uint8)(implParam->Table[17] & 0x00FF);
    SaveFncDataBuffer[35] = (uint8)((implParam->Table[17] >> 8) & 0x00FF);
    SaveFncDataBuffer[36] = (uint8)(implParam->Table[18] & 0x00FF);
    SaveFncDataBuffer[37] = (uint8)((implParam->Table[18] >> 8) & 0x00FF);
    SaveFncDataBuffer[38] = (uint8)(implParam->Table[19] & 0x00FF);
    SaveFncDataBuffer[39] = (uint8)((implParam->Table[19] >> 8) & 0x00FF);
    SaveFncDataBuffer[40] = (uint8)(implParam->Table[20] & 0x00FF);
    SaveFncDataBuffer[41] = (uint8)((implParam->Table[20] >> 8) & 0x00FF);
    SaveFncDataBuffer[42] = (uint8)(implParam->Table[21] & 0x00FF);
    SaveFncDataBuffer[43] = (uint8)((implParam->Table[21] >> 8) & 0x00FF);
    SaveFncDataBuffer[44] = (uint8)(implParam->Table[22] & 0x00FF);
    SaveFncDataBuffer[45] = (uint8)((implParam->Table[22] >> 8) & 0x00FF);
    SaveFncDataBuffer[46] = (uint8)(implParam->Table[23] & 0x00FF);
    SaveFncDataBuffer[47] = (uint8)((implParam->Table[23] >> 8) & 0x00FF);
    SaveFncDataBuffer[48] = (uint8)(implParam->Table[24] & 0x00FF);
    SaveFncDataBuffer[49] = (uint8)((implParam->Table[24] >> 8) & 0x00FF);
    SaveFncDataBuffer[50] = (uint8)(implParam->Table[25] & 0x00FF);
    SaveFncDataBuffer[51] = (uint8)((implParam->Table[25] >> 8) & 0x00FF);
    SaveFncDataBuffer[52] = (uint8)(implParam->Table[26] & 0x00FF);
    SaveFncDataBuffer[53] = (uint8)((implParam->Table[26] >> 8) & 0x00FF);
    SaveFncDataBuffer[54] = (uint8)(implParam->Table[27] & 0x00FF);
    SaveFncDataBuffer[55] = (uint8)((implParam->Table[27] >> 8) & 0x00FF);
    SaveFncDataBuffer[56] = (uint8)(implParam->Table[28] & 0x00FF);
    SaveFncDataBuffer[57] = (uint8)((implParam->Table[28] >> 8) & 0x00FF);
    SaveFncDataBuffer[58] = (uint8)(implParam->Table[29] & 0x00FF);
    SaveFncDataBuffer[59] = (uint8)((implParam->Table[29] >> 8) & 0x00FF);
    SaveFncDataBuffer[60] = (uint8)(implParam->Table[30] & 0x00FF);
    SaveFncDataBuffer[61] = (uint8)((implParam->Table[30] >> 8) & 0x00FF);
    SaveFncDataBuffer[62] = (uint8)(implParam->Table[31] & 0x00FF);
    SaveFncDataBuffer[63] = (uint8)((implParam->Table[31] >> 8) & 0x00FF);
    SaveFncDataBuffer[64] = (uint8)(implParam->Table[32] & 0x00FF);
    SaveFncDataBuffer[65] = (uint8)((implParam->Table[32] >> 8) & 0x00FF);
    SaveFncDataBuffer[66] = (uint8)(implParam->Table[33] & 0x00FF);
    SaveFncDataBuffer[67] = (uint8)((implParam->Table[33] >> 8) & 0x00FF);
    SaveFncDataBuffer[68] = (uint8)(implParam->Table[34] & 0x00FF);
    SaveFncDataBuffer[69] = (uint8)((implParam->Table[34] >> 8) & 0x00FF);
    SaveFncDataBuffer[70] = (uint8)(implParam->Table[35] & 0x00FF);
    SaveFncDataBuffer[71] = (uint8)((implParam->Table[35] >> 8) & 0x00FF);
    SaveFncDataBuffer[72] = (uint8)(implParam->Table[36] & 0x00FF);
    SaveFncDataBuffer[73] = (uint8)((implParam->Table[36] >> 8) & 0x00FF);
    SaveFncDataBuffer[74] = (uint8)(implParam->Table[37] & 0x00FF);
    SaveFncDataBuffer[75] = (uint8)((implParam->Table[37] >> 8) & 0x00FF);
    SaveFncDataBuffer[76] = (uint8)(implParam->Table[38] & 0x00FF);
    SaveFncDataBuffer[77] = (uint8)((implParam->Table[38] >> 8) & 0x00FF);
    SaveFncDataBuffer[78] = (uint8)(implParam->Table[39] & 0x00FF);
    SaveFncDataBuffer[79] = (uint8)((implParam->Table[39] >> 8) & 0x00FF);
    SaveFncDataBuffer[80] = (uint8)(implParam->Table[40] & 0x00FF);
    SaveFncDataBuffer[81] = (uint8)((implParam->Table[40] >> 8) & 0x00FF);
    SaveFncDataBuffer[82] = (uint8)(implParam->Table[41] & 0x00FF);
    SaveFncDataBuffer[83] = (uint8)((implParam->Table[41] >> 8) & 0x00FF);
    SaveFncDataBuffer[84] = (uint8)(implParam->Table[42] & 0x00FF);
    SaveFncDataBuffer[85] = (uint8)((implParam->Table[42] >> 8) & 0x00FF);
    SaveFncDataBuffer[86] = (uint8)(implParam->Table[43] & 0x00FF);
    SaveFncDataBuffer[87] = (uint8)((implParam->Table[43] >> 8) & 0x00FF);
    SaveFncDataBuffer[88] = (uint8)(implParam->Table[44] & 0x00FF);
    SaveFncDataBuffer[89] = (uint8)((implParam->Table[44] >> 8) & 0x00FF);
    SaveFncDataBuffer[90] = (uint8)(implParam->Table[45] & 0x00FF);
    SaveFncDataBuffer[91] = (uint8)((implParam->Table[45] >> 8) & 0x00FF);
    SaveFncDataBuffer[92] = (uint8)(implParam->Table[46] & 0x00FF);
    SaveFncDataBuffer[93] = (uint8)((implParam->Table[46] >> 8) & 0x00FF);
    SaveFncDataBuffer[94] = (uint8)(implParam->Table[47] & 0x00FF);
    SaveFncDataBuffer[95] = (uint8)((implParam->Table[47] >> 8) & 0x00FF);
    SaveFncDataBuffer[96] = (uint8)(implParam->Table[48] & 0x00FF);
    SaveFncDataBuffer[97] = (uint8)((implParam->Table[48] >> 8) & 0x00FF);
    SaveFncDataBuffer[98] = (uint8)(implParam->Table[49] & 0x00FF);
    SaveFncDataBuffer[99] = (uint8)((implParam->Table[49] >> 8) & 0x00FF);
    SaveFncDataBuffer[100] = (uint8)(implParam->Table[50] & 0x00FF);
    SaveFncDataBuffer[101] = (uint8)((implParam->Table[50] >> 8) & 0x00FF);
    SaveFncDataBuffer[102] = (uint8)(implParam->Table[51] & 0x00FF);
    SaveFncDataBuffer[103] = (uint8)((implParam->Table[51] >> 8) & 0x00FF);
    SaveFncDataBuffer[104] = (uint8)(implParam->Table[52] & 0x00FF);
    SaveFncDataBuffer[105] = (uint8)((implParam->Table[52] >> 8) & 0x00FF);
    SaveFncDataBuffer[106] = (uint8)(implParam->Table[53] & 0x00FF);
    SaveFncDataBuffer[107] = (uint8)((implParam->Table[53] >> 8) & 0x00FF);
    SaveFncDataBuffer[108] = (uint8)(implParam->Table[54] & 0x00FF);
    SaveFncDataBuffer[109] = (uint8)((implParam->Table[54] >> 8) & 0x00FF);
    SaveFncDataBuffer[110] = (uint8)(implParam->Table[55] & 0x00FF);
    SaveFncDataBuffer[111] = (uint8)((implParam->Table[55] >> 8) & 0x00FF);
    SaveFncDataBuffer[112] = (uint8)(implParam->Table[56] & 0x00FF);
    SaveFncDataBuffer[113] = (uint8)((implParam->Table[56] >> 8) & 0x00FF);
    SaveFncDataBuffer[114] = (uint8)(implParam->Table[57] & 0x00FF);
    SaveFncDataBuffer[115] = (uint8)((implParam->Table[57] >> 8) & 0x00FF);
    SaveFncDataBuffer[116] = (uint8)(implParam->Table[58] & 0x00FF);
    SaveFncDataBuffer[117] = (uint8)((implParam->Table[58] >> 8) & 0x00FF);
    SaveFncDataBuffer[118] = (uint8)(implParam->Table[59] & 0x00FF);
    SaveFncDataBuffer[119] = (uint8)((implParam->Table[59] >> 8) & 0x00FF);
    SaveFncDataBuffer[120] = (uint8)(implParam->Table[60] & 0x00FF);
    SaveFncDataBuffer[121] = (uint8)((implParam->Table[60] >> 8) & 0x00FF);
    SaveFncDataBuffer[122] = (uint8)(implParam->Table[61] & 0x00FF);
    SaveFncDataBuffer[123] = (uint8)((implParam->Table[61] >> 8) & 0x00FF);
    SaveFncDataBuffer[124] = (uint8)(implParam->Table[62] & 0x00FF);
    SaveFncDataBuffer[125] = (uint8)((implParam->Table[62] >> 8) & 0x00FF);
    SaveFncDataBuffer[126] = (uint8)(implParam->Table[63] & 0x00FF);
    SaveFncDataBuffer[127] = (uint8)((implParam->Table[63] >> 8) & 0x00FF);
    SaveFncDataBuffer[128] = (uint8)(implParam->Table[64] & 0x00FF);
    SaveFncDataBuffer[129] = (uint8)((implParam->Table[64] >> 8) & 0x00FF);
    SaveFncDataBuffer[130] = (uint8)(implParam->Table[65] & 0x00FF);
    SaveFncDataBuffer[131] = (uint8)((implParam->Table[65] >> 8) & 0x00FF);
    SaveFncDataBuffer[132] = (uint8)(implParam->Table[66] & 0x00FF);
    SaveFncDataBuffer[133] = (uint8)((implParam->Table[66] >> 8) & 0x00FF);
    SaveFncDataBuffer[134] = (uint8)(implParam->Table[67] & 0x00FF);
    SaveFncDataBuffer[135] = (uint8)((implParam->Table[67] >> 8) & 0x00FF);
    SaveFncDataBuffer[136] = (uint8)(implParam->Table[68] & 0x00FF);
    SaveFncDataBuffer[137] = (uint8)((implParam->Table[68] >> 8) & 0x00FF);
    SaveFncDataBuffer[138] = (uint8)(implParam->Table[69] & 0x00FF);
    SaveFncDataBuffer[139] = (uint8)((implParam->Table[69] >> 8) & 0x00FF);
    SaveFncDataBuffer[140] = (uint8)(implParam->Table[70] & 0x00FF);
    SaveFncDataBuffer[141] = (uint8)((implParam->Table[70] >> 8) & 0x00FF);
    SaveFncDataBuffer[142] = (uint8)(implParam->Table[71] & 0x00FF);
    SaveFncDataBuffer[143] = (uint8)((implParam->Table[71] >> 8) & 0x00FF);
    SaveFncDataBuffer[144] = (uint8)(implParam->Table[72] & 0x00FF);
    SaveFncDataBuffer[145] = (uint8)((implParam->Table[72] >> 8) & 0x00FF);
    SaveFncDataBuffer[146] = (uint8)(implParam->Table[73] & 0x00FF);
    SaveFncDataBuffer[147] = (uint8)((implParam->Table[73] >> 8) & 0x00FF);
    SaveFncDataBuffer[148] = (uint8)(implParam->Table[74] & 0x00FF);
    SaveFncDataBuffer[149] = (uint8)((implParam->Table[74] >> 8) & 0x00FF);
    SaveFncDataBuffer[150] = (uint8)(implParam->Table[75] & 0x00FF);
    SaveFncDataBuffer[151] = (uint8)((implParam->Table[75] >> 8) & 0x00FF);
    SaveFncDataBuffer[152] = (uint8)(implParam->Table[76] & 0x00FF);
    SaveFncDataBuffer[153] = (uint8)((implParam->Table[76] >> 8) & 0x00FF);
    SaveFncDataBuffer[154] = (uint8)(implParam->Table[77] & 0x00FF);
    SaveFncDataBuffer[155] = (uint8)((implParam->Table[77] >> 8) & 0x00FF);
    SaveFncDataBuffer[156] = (uint8)(implParam->Table[78] & 0x00FF);
    SaveFncDataBuffer[157] = (uint8)((implParam->Table[78] >> 8) & 0x00FF);
    SaveFncDataBuffer[158] = (uint8)(implParam->Table[79] & 0x00FF);
    SaveFncDataBuffer[159] = (uint8)((implParam->Table[79] >> 8) & 0x00FF);
    SaveFncDataBuffer[160] = (uint8)(implParam->Table[80] & 0x00FF);
    SaveFncDataBuffer[161] = (uint8)((implParam->Table[80] >> 8) & 0x00FF);
    SaveFncDataBuffer[162] = (uint8)(implParam->Table[81] & 0x00FF);
    SaveFncDataBuffer[163] = (uint8)((implParam->Table[81] >> 8) & 0x00FF);
    SaveFncDataBuffer[164] = (uint8)(implParam->Table[82] & 0x00FF);
    SaveFncDataBuffer[165] = (uint8)((implParam->Table[82] >> 8) & 0x00FF);
    SaveFncDataBuffer[166] = (uint8)(implParam->Table[83] & 0x00FF);
    SaveFncDataBuffer[167] = (uint8)((implParam->Table[83] >> 8) & 0x00FF);
    SaveFncDataBuffer[168] = (uint8)(implParam->Table[84] & 0x00FF);
    SaveFncDataBuffer[169] = (uint8)((implParam->Table[84] >> 8) & 0x00FF);
    SaveFncDataBuffer[170] = (uint8)(implParam->Table[85] & 0x00FF);
    SaveFncDataBuffer[171] = (uint8)((implParam->Table[85] >> 8) & 0x00FF);
    SaveFncDataBuffer[172] = (uint8)(implParam->Table[86] & 0x00FF);
    SaveFncDataBuffer[173] = (uint8)((implParam->Table[86] >> 8) & 0x00FF);
    SaveFncDataBuffer[174] = (uint8)(implParam->Table[87] & 0x00FF);
    SaveFncDataBuffer[175] = (uint8)((implParam->Table[87] >> 8) & 0x00FF);
    SaveFncDataBuffer[176] = (uint8)(implParam->Table[88] & 0x00FF);
    SaveFncDataBuffer[177] = (uint8)((implParam->Table[88] >> 8) & 0x00FF);
    SaveFncDataBuffer[178] = (uint8)(implParam->Table[89] & 0x00FF);
    SaveFncDataBuffer[179] = (uint8)((implParam->Table[89] >> 8) & 0x00FF);
    SaveFncDataBuffer[180] = (uint8)(implParam->Table[90] & 0x00FF);
    SaveFncDataBuffer[181] = (uint8)((implParam->Table[90] >> 8) & 0x00FF);
    SaveFncDataBuffer[182] = (uint8)(implParam->Table[91] & 0x00FF);
    SaveFncDataBuffer[183] = (uint8)((implParam->Table[91] >> 8) & 0x00FF);
    SaveFncDataBuffer[184] = (uint8)(implParam->Table[92] & 0x00FF);
    SaveFncDataBuffer[185] = (uint8)((implParam->Table[92] >> 8) & 0x00FF);
    SaveFncDataBuffer[186] = (uint8)(implParam->Table[93] & 0x00FF);
    SaveFncDataBuffer[187] = (uint8)((implParam->Table[93] >> 8) & 0x00FF);
    SaveFncDataBuffer[188] = (uint8)(implParam->Table[94] & 0x00FF);
    SaveFncDataBuffer[189] = (uint8)((implParam->Table[94] >> 8) & 0x00FF);
    SaveFncDataBuffer[190] = (uint8)(implParam->Table[95] & 0x00FF);
    SaveFncDataBuffer[191] = (uint8)((implParam->Table[95] >> 8) & 0x00FF);
    SaveFncDataBuffer[192] = (uint8)(implParam->Table[96] & 0x00FF);
    SaveFncDataBuffer[193] = (uint8)((implParam->Table[96] >> 8) & 0x00FF);
    SaveFncDataBuffer[194] = (uint8)(implParam->Table[97] & 0x00FF);
    SaveFncDataBuffer[195] = (uint8)((implParam->Table[97] >> 8) & 0x00FF);
    SaveFncDataBuffer[196] = (uint8)(implParam->Table[98] & 0x00FF);
    SaveFncDataBuffer[197] = (uint8)((implParam->Table[98] >> 8) & 0x00FF);
    SaveFncDataBuffer[198] = (uint8)(implParam->Table[99] & 0x00FF);
    SaveFncDataBuffer[199] = (uint8)((implParam->Table[99] >> 8) & 0x00FF);
    SaveFncDataBuffer[200] = (uint8)(implParam->Table[100] & 0x00FF);
    SaveFncDataBuffer[201] = (uint8)((implParam->Table[100] >> 8) & 0x00FF);
    SaveFncDataBuffer[202] = (uint8)(implParam->Table[101] & 0x00FF);
    SaveFncDataBuffer[203] = (uint8)((implParam->Table[101] >> 8) & 0x00FF);
    SaveFncDataBuffer[204] = (uint8)(implParam->Table[102] & 0x00FF);
    SaveFncDataBuffer[205] = (uint8)((implParam->Table[102] >> 8) & 0x00FF);
    SaveFncDataBuffer[206] = (uint8)(implParam->Table[103] & 0x00FF);
    SaveFncDataBuffer[207] = (uint8)((implParam->Table[103] >> 8) & 0x00FF);
    SaveFncDataBuffer[208] = (uint8)(implParam->Table[104] & 0x00FF);
    SaveFncDataBuffer[209] = (uint8)((implParam->Table[104] >> 8) & 0x00FF);
    SaveFncDataBuffer[210] = (uint8)(implParam->Table[105] & 0x00FF);
    SaveFncDataBuffer[211] = (uint8)((implParam->Table[105] >> 8) & 0x00FF);
    SaveFncDataBuffer[212] = (uint8)(implParam->Table[106] & 0x00FF);
    SaveFncDataBuffer[213] = (uint8)((implParam->Table[106] >> 8) & 0x00FF);
    SaveFncDataBuffer[214] = (uint8)(implParam->Table[107] & 0x00FF);
    SaveFncDataBuffer[215] = (uint8)((implParam->Table[107] >> 8) & 0x00FF);
    SaveFncDataBuffer[216] = (uint8)(implParam->Table[108] & 0x00FF);
    SaveFncDataBuffer[217] = (uint8)((implParam->Table[108] >> 8) & 0x00FF);
    SaveFncDataBuffer[218] = (uint8)(implParam->Table[109] & 0x00FF);
    SaveFncDataBuffer[219] = (uint8)((implParam->Table[109] >> 8) & 0x00FF);
    SaveFncDataBuffer[220] = (uint8)(implParam->Table[110] & 0x00FF);
    SaveFncDataBuffer[221] = (uint8)((implParam->Table[110] >> 8) & 0x00FF);
    SaveFncDataBuffer[222] = (uint8)(implParam->Table[111] & 0x00FF);
    SaveFncDataBuffer[223] = (uint8)((implParam->Table[111] >> 8) & 0x00FF);
    SaveFncDataBuffer[224] = (uint8)(implParam->Table[112] & 0x00FF);
    SaveFncDataBuffer[225] = (uint8)((implParam->Table[112] >> 8) & 0x00FF);
    SaveFncDataBuffer[226] = (uint8)(implParam->Table[113] & 0x00FF);
    SaveFncDataBuffer[227] = (uint8)((implParam->Table[113] >> 8) & 0x00FF);
    SaveFncDataBuffer[228] = (uint8)(implParam->Table[114] & 0x00FF);
    SaveFncDataBuffer[229] = (uint8)((implParam->Table[114] >> 8) & 0x00FF);
    SaveFncDataBuffer[230] = (uint8)(implParam->Table[115] & 0x00FF);
    SaveFncDataBuffer[231] = (uint8)((implParam->Table[115] >> 8) & 0x00FF);
    SaveFncDataBuffer[232] = (uint8)(implParam->Table[116] & 0x00FF);
    SaveFncDataBuffer[233] = (uint8)((implParam->Table[116] >> 8) & 0x00FF);
    SaveFncDataBuffer[234] = (uint8)(implParam->Table[117] & 0x00FF);
    SaveFncDataBuffer[235] = (uint8)((implParam->Table[117] >> 8) & 0x00FF);
    SaveFncDataBuffer[236] = (uint8)(implParam->Table[118] & 0x00FF);
    SaveFncDataBuffer[237] = (uint8)((implParam->Table[118] >> 8) & 0x00FF);
    SaveFncDataBuffer[238] = (uint8)(implParam->Table[119] & 0x00FF);
    SaveFncDataBuffer[239] = (uint8)((implParam->Table[119] >> 8) & 0x00FF);
    SaveFncDataBuffer[240] = (uint8)(implParam->Table[120] & 0x00FF);
    SaveFncDataBuffer[241] = (uint8)((implParam->Table[120] >> 8) & 0x00FF);
    SaveFncDataBuffer[242] = (uint8)(implParam->Table[121] & 0x00FF);
    SaveFncDataBuffer[243] = (uint8)((implParam->Table[121] >> 8) & 0x00FF);
    SaveFncDataBuffer[244] = (uint8)(implParam->Table[122] & 0x00FF);
    SaveFncDataBuffer[245] = (uint8)((implParam->Table[122] >> 8) & 0x00FF);
    SaveFncDataBuffer[246] = (uint8)(implParam->Table[123] & 0x00FF);
    SaveFncDataBuffer[247] = (uint8)((implParam->Table[123] >> 8) & 0x00FF);
    SaveFncDataBuffer[248] = (uint8)(implParam->Table[124] & 0x00FF);
    SaveFncDataBuffer[249] = (uint8)((implParam->Table[124] >> 8) & 0x00FF);
    SaveFncDataBuffer[250] = (uint8)(implParam->Table[125] & 0x00FF);
    SaveFncDataBuffer[251] = (uint8)((implParam->Table[125] >> 8) & 0x00FF);
    SaveFncDataBuffer[252] = (uint8)(implParam->Table[126] & 0x00FF);
    SaveFncDataBuffer[253] = (uint8)((implParam->Table[126] >> 8) & 0x00FF);
    SaveFncDataBuffer[254] = (uint8)(implParam->Table[127] & 0x00FF);
    SaveFncDataBuffer[255] = (uint8)((implParam->Table[127] >> 8) & 0x00FF);
    SaveFncDataBuffer[256] = (uint8)(implParam->Table[128] & 0x00FF);
    SaveFncDataBuffer[257] = (uint8)((implParam->Table[128] >> 8) & 0x00FF);
    SaveFncDataBuffer[258] = (uint8)(implParam->Table[129] & 0x00FF);
    SaveFncDataBuffer[259] = (uint8)((implParam->Table[129] >> 8) & 0x00FF);
    SaveFncDataBuffer[260] = (uint8)(implParam->Table[130] & 0x00FF);
    SaveFncDataBuffer[261] = (uint8)((implParam->Table[130] >> 8) & 0x00FF);
    SaveFncDataBuffer[262] = (uint8)(implParam->Table[131] & 0x00FF);
    SaveFncDataBuffer[263] = (uint8)((implParam->Table[131] >> 8) & 0x00FF);
    SaveFncDataBuffer[264] = (uint8)(implParam->Table[132] & 0x00FF);
    SaveFncDataBuffer[265] = (uint8)((implParam->Table[132] >> 8) & 0x00FF);
    SaveFncDataBuffer[266] = (uint8)(implParam->Table[133] & 0x00FF);
    SaveFncDataBuffer[267] = (uint8)((implParam->Table[133] >> 8) & 0x00FF);
    SaveFncDataBuffer[268] = (uint8)(implParam->Table[134] & 0x00FF);
    SaveFncDataBuffer[269] = (uint8)((implParam->Table[134] >> 8) & 0x00FF);
    SaveFncDataBuffer[270] = (uint8)(implParam->Table[135] & 0x00FF);
    SaveFncDataBuffer[271] = (uint8)((implParam->Table[135] >> 8) & 0x00FF);
    SaveFncDataBuffer[272] = (uint8)(implParam->Table[136] & 0x00FF);
    SaveFncDataBuffer[273] = (uint8)((implParam->Table[136] >> 8) & 0x00FF);
    SaveFncDataBuffer[274] = (uint8)(implParam->Table[137] & 0x00FF);
    SaveFncDataBuffer[275] = (uint8)((implParam->Table[137] >> 8) & 0x00FF);
    SaveFncDataBuffer[276] = (uint8)(implParam->Table[138] & 0x00FF);
    SaveFncDataBuffer[277] = (uint8)((implParam->Table[138] >> 8) & 0x00FF);
    SaveFncDataBuffer[278] = (uint8)(implParam->Table[139] & 0x00FF);
    SaveFncDataBuffer[279] = (uint8)((implParam->Table[139] >> 8) & 0x00FF);
    SaveFncDataBuffer[280] = (uint8)(implParam->Table[140] & 0x00FF);
    SaveFncDataBuffer[281] = (uint8)((implParam->Table[140] >> 8) & 0x00FF);
    SaveFncDataBuffer[282] = (uint8)(implParam->Table[141] & 0x00FF);
    SaveFncDataBuffer[283] = (uint8)((implParam->Table[141] >> 8) & 0x00FF);
    SaveFncDataBuffer[284] = (uint8)(implParam->Table[142] & 0x00FF);
    SaveFncDataBuffer[285] = (uint8)((implParam->Table[142] >> 8) & 0x00FF);
    SaveFncDataBuffer[286] = (uint8)(implParam->Table[143] & 0x00FF);
    SaveFncDataBuffer[287] = (uint8)((implParam->Table[143] >> 8) & 0x00FF);
    SaveFncDataBuffer[288] = (uint8)(implParam->Table[144] & 0x00FF);
    SaveFncDataBuffer[289] = (uint8)((implParam->Table[144] >> 8) & 0x00FF);
    SaveFncDataBuffer[290] = (uint8)(implParam->Table[145] & 0x00FF);
    SaveFncDataBuffer[291] = (uint8)((implParam->Table[145] >> 8) & 0x00FF);
    SaveFncDataBuffer[292] = (uint8)(implParam->Table[146] & 0x00FF);
    SaveFncDataBuffer[293] = (uint8)((implParam->Table[146] >> 8) & 0x00FF);
    SaveFncDataBuffer[294] = (uint8)(implParam->Table[147] & 0x00FF);
    SaveFncDataBuffer[295] = (uint8)((implParam->Table[147] >> 8) & 0x00FF);
    SaveFncDataBuffer[296] = (uint8)(implParam->Table[148] & 0x00FF);
    SaveFncDataBuffer[297] = (uint8)((implParam->Table[148] >> 8) & 0x00FF);
    SaveFncDataBuffer[298] = (uint8)(implParam->Table[149] & 0x00FF);
    SaveFncDataBuffer[299] = (uint8)((implParam->Table[149] >> 8) & 0x00FF);
    SaveFncDataBuffer[300] = (uint8)(implParam->Table[150] & 0x00FF);
    SaveFncDataBuffer[301] = (uint8)((implParam->Table[150] >> 8) & 0x00FF);
    SaveFncDataBuffer[302] = (uint8)(implParam->Table[151] & 0x00FF);
    SaveFncDataBuffer[303] = (uint8)((implParam->Table[151] >> 8) & 0x00FF);
    SaveFncDataBuffer[304] = (uint8)(implParam->Table[152] & 0x00FF);
    SaveFncDataBuffer[305] = (uint8)((implParam->Table[152] >> 8) & 0x00FF);
    SaveFncDataBuffer[306] = (uint8)(implParam->Table[153] & 0x00FF);
    SaveFncDataBuffer[307] = (uint8)((implParam->Table[153] >> 8) & 0x00FF);
    SaveFncDataBuffer[308] = (uint8)(implParam->Table[154] & 0x00FF);
    SaveFncDataBuffer[309] = (uint8)((implParam->Table[154] >> 8) & 0x00FF);
    SaveFncDataBuffer[310] = (uint8)(implParam->Table[155] & 0x00FF);
    SaveFncDataBuffer[311] = (uint8)((implParam->Table[155] >> 8) & 0x00FF);
    SaveFncDataBuffer[312] = (uint8)(implParam->Table[156] & 0x00FF);
    SaveFncDataBuffer[313] = (uint8)((implParam->Table[156] >> 8) & 0x00FF);
    SaveFncDataBuffer[314] = (uint8)(implParam->Table[157] & 0x00FF);
    SaveFncDataBuffer[315] = (uint8)((implParam->Table[157] >> 8) & 0x00FF);
    SaveFncDataBuffer[316] = (uint8)(implParam->Table[158] & 0x00FF);
    SaveFncDataBuffer[317] = (uint8)((implParam->Table[158] >> 8) & 0x00FF);
    SaveFncDataBuffer[318] = (uint8)(implParam->Table[159] & 0x00FF);
    SaveFncDataBuffer[319] = (uint8)((implParam->Table[159] >> 8) & 0x00FF);
    SaveFncDataBuffer[320] = (uint8)(implParam->Table[160] & 0x00FF);
    SaveFncDataBuffer[321] = (uint8)((implParam->Table[160] >> 8) & 0x00FF);
    SaveFncDataBuffer[322] = (uint8)(implParam->Table[161] & 0x00FF);
    SaveFncDataBuffer[323] = (uint8)((implParam->Table[161] >> 8) & 0x00FF);
    SaveFncDataBuffer[324] = (uint8)(implParam->Table[162] & 0x00FF);
    SaveFncDataBuffer[325] = (uint8)((implParam->Table[162] >> 8) & 0x00FF);
    SaveFncDataBuffer[326] = (uint8)(implParam->Table[163] & 0x00FF);
    SaveFncDataBuffer[327] = (uint8)((implParam->Table[163] >> 8) & 0x00FF);
    SaveFncDataBuffer[328] = (uint8)(implParam->Table[164] & 0x00FF);
    SaveFncDataBuffer[329] = (uint8)((implParam->Table[164] >> 8) & 0x00FF);
    SaveFncDataBuffer[330] = (uint8)(implParam->Table[165] & 0x00FF);
    SaveFncDataBuffer[331] = (uint8)((implParam->Table[165] >> 8) & 0x00FF);
    SaveFncDataBuffer[332] = (uint8)(implParam->Table[166] & 0x00FF);
    SaveFncDataBuffer[333] = (uint8)((implParam->Table[166] >> 8) & 0x00FF);
    SaveFncDataBuffer[334] = (uint8)(implParam->Table[167] & 0x00FF);
    SaveFncDataBuffer[335] = (uint8)((implParam->Table[167] >> 8) & 0x00FF);
    SaveFncDataBuffer[336] = (uint8)(implParam->Table[168] & 0x00FF);
    SaveFncDataBuffer[337] = (uint8)((implParam->Table[168] >> 8) & 0x00FF);
    SaveFncDataBuffer[338] = (uint8)(implParam->Table[169] & 0x00FF);
    SaveFncDataBuffer[339] = (uint8)((implParam->Table[169] >> 8) & 0x00FF);
    SaveFncDataBuffer[340] = (uint8)(implParam->Table[170] & 0x00FF);
    SaveFncDataBuffer[341] = (uint8)((implParam->Table[170] >> 8) & 0x00FF);
    SaveFncDataBuffer[342] = (uint8)(implParam->Table[171] & 0x00FF);
    SaveFncDataBuffer[343] = (uint8)((implParam->Table[171] >> 8) & 0x00FF);
    SaveFncDataBuffer[344] = (uint8)(implParam->Table[172] & 0x00FF);
    SaveFncDataBuffer[345] = (uint8)((implParam->Table[172] >> 8) & 0x00FF);
    SaveFncDataBuffer[346] = (uint8)(implParam->Table[173] & 0x00FF);
    SaveFncDataBuffer[347] = (uint8)((implParam->Table[173] >> 8) & 0x00FF);
    SaveFncDataBuffer[348] = (uint8)(implParam->Table[174] & 0x00FF);
    SaveFncDataBuffer[349] = (uint8)((implParam->Table[174] >> 8) & 0x00FF);
    SaveFncDataBuffer[350] = (uint8)(implParam->Table[175] & 0x00FF);
    SaveFncDataBuffer[351] = (uint8)((implParam->Table[175] >> 8) & 0x00FF);
    SaveFncDataBuffer[352] = (uint8)(implParam->Table[176] & 0x00FF);
    SaveFncDataBuffer[353] = (uint8)((implParam->Table[176] >> 8) & 0x00FF);
    SaveFncDataBuffer[354] = (uint8)(implParam->Table[177] & 0x00FF);
    SaveFncDataBuffer[355] = (uint8)((implParam->Table[177] >> 8) & 0x00FF);
    SaveFncDataBuffer[356] = (uint8)(implParam->Table[178] & 0x00FF);
    SaveFncDataBuffer[357] = (uint8)((implParam->Table[178] >> 8) & 0x00FF);
    SaveFncDataBuffer[358] = (uint8)(implParam->Table[179] & 0x00FF);
    SaveFncDataBuffer[359] = (uint8)((implParam->Table[179] >> 8) & 0x00FF);
    SaveFncDataBuffer[360] = (uint8)(implParam->Table[180] & 0x00FF);
    SaveFncDataBuffer[361] = (uint8)((implParam->Table[180] >> 8) & 0x00FF);
    SaveFncDataBuffer[362] = (uint8)(implParam->Table[181] & 0x00FF);
    SaveFncDataBuffer[363] = (uint8)((implParam->Table[181] >> 8) & 0x00FF);
    SaveFncDataBuffer[364] = (uint8)(implParam->Table[182] & 0x00FF);
    SaveFncDataBuffer[365] = (uint8)((implParam->Table[182] >> 8) & 0x00FF);
    SaveFncDataBuffer[366] = (uint8)(implParam->Table[183] & 0x00FF);
    SaveFncDataBuffer[367] = (uint8)((implParam->Table[183] >> 8) & 0x00FF);
    SaveFncDataBuffer[368] = (uint8)(implParam->Table[184] & 0x00FF);
    SaveFncDataBuffer[369] = (uint8)((implParam->Table[184] >> 8) & 0x00FF);
    SaveFncDataBuffer[370] = (uint8)(implParam->Table[185] & 0x00FF);
    SaveFncDataBuffer[371] = (uint8)((implParam->Table[185] >> 8) & 0x00FF);
    SaveFncDataBuffer[372] = (uint8)(implParam->Table[186] & 0x00FF);
    SaveFncDataBuffer[373] = (uint8)((implParam->Table[186] >> 8) & 0x00FF);
    SaveFncDataBuffer[374] = (uint8)(implParam->Table[187] & 0x00FF);
    SaveFncDataBuffer[375] = (uint8)((implParam->Table[187] >> 8) & 0x00FF);
    SaveFncDataBuffer[376] = (uint8)(implParam->Table[188] & 0x00FF);
    SaveFncDataBuffer[377] = (uint8)((implParam->Table[188] >> 8) & 0x00FF);
    SaveFncDataBuffer[378] = (uint8)(implParam->Table[189] & 0x00FF);
    SaveFncDataBuffer[379] = (uint8)((implParam->Table[189] >> 8) & 0x00FF);
    SaveFncDataBuffer[380] = (uint8)(implParam->Table[190] & 0x00FF);
    SaveFncDataBuffer[381] = (uint8)((implParam->Table[190] >> 8) & 0x00FF);
    SaveFncDataBuffer[382] = (uint8)(implParam->Table[191] & 0x00FF);
    SaveFncDataBuffer[383] = (uint8)((implParam->Table[191] >> 8) & 0x00FF);
    SaveFncDataBuffer[384] = (uint8)(implParam->Table[192] & 0x00FF);
    SaveFncDataBuffer[385] = (uint8)((implParam->Table[192] >> 8) & 0x00FF);
    SaveFncDataBuffer[386] = (uint8)(implParam->Table[193] & 0x00FF);
    SaveFncDataBuffer[387] = (uint8)((implParam->Table[193] >> 8) & 0x00FF);
    SaveFncDataBuffer[388] = (uint8)(implParam->Table[194] & 0x00FF);
    SaveFncDataBuffer[389] = (uint8)((implParam->Table[194] >> 8) & 0x00FF);
    SaveFncDataBuffer[390] = (uint8)(implParam->Table[195] & 0x00FF);
    SaveFncDataBuffer[391] = (uint8)((implParam->Table[195] >> 8) & 0x00FF);
    SaveFncDataBuffer[392] = (uint8)(implParam->Table[196] & 0x00FF);
    SaveFncDataBuffer[393] = (uint8)((implParam->Table[196] >> 8) & 0x00FF);
    SaveFncDataBuffer[394] = (uint8)(implParam->Table[197] & 0x00FF);
    SaveFncDataBuffer[395] = (uint8)((implParam->Table[197] >> 8) & 0x00FF);
    SaveFncDataBuffer[396] = (uint8)(implParam->Table[198] & 0x00FF);
    SaveFncDataBuffer[397] = (uint8)((implParam->Table[198] >> 8) & 0x00FF);
    SaveFncDataBuffer[398] = (uint8)(implParam->Table[199] & 0x00FF);
    SaveFncDataBuffer[399] = (uint8)((implParam->Table[199] >> 8) & 0x00FF);
    SaveFncDataBuffer[400] = (uint8)(implParam->Table[200] & 0x00FF);
    SaveFncDataBuffer[401] = (uint8)((implParam->Table[200] >> 8) & 0x00FF);
    SaveFncDataBuffer[402] = (uint8)(implParam->Table[201] & 0x00FF);
    SaveFncDataBuffer[403] = (uint8)((implParam->Table[201] >> 8) & 0x00FF);
    SaveFncDataBuffer[404] = (uint8)(implParam->Table[202] & 0x00FF);
    SaveFncDataBuffer[405] = (uint8)((implParam->Table[202] >> 8) & 0x00FF);
    SaveFncDataBuffer[406] = (uint8)(implParam->Table[203] & 0x00FF);
    SaveFncDataBuffer[407] = (uint8)((implParam->Table[203] >> 8) & 0x00FF);
    SaveFncDataBuffer[408] = (uint8)(implParam->Table[204] & 0x00FF);
    SaveFncDataBuffer[409] = (uint8)((implParam->Table[204] >> 8) & 0x00FF);
    SaveFncDataBuffer[410] = (uint8)(implParam->Table[205] & 0x00FF);
    SaveFncDataBuffer[411] = (uint8)((implParam->Table[205] >> 8) & 0x00FF);
    SaveFncDataBuffer[412] = (uint8)(implParam->Table[206] & 0x00FF);
    SaveFncDataBuffer[413] = (uint8)((implParam->Table[206] >> 8) & 0x00FF);
    SaveFncDataBuffer[414] = (uint8)(implParam->Table[207] & 0x00FF);
    SaveFncDataBuffer[415] = (uint8)((implParam->Table[207] >> 8) & 0x00FF);
    SaveFncDataBuffer[416] = (uint8)(implParam->Table[208] & 0x00FF);
    SaveFncDataBuffer[417] = (uint8)((implParam->Table[208] >> 8) & 0x00FF);
    SaveFncDataBuffer[418] = (uint8)(implParam->Table[209] & 0x00FF);
    SaveFncDataBuffer[419] = (uint8)((implParam->Table[209] >> 8) & 0x00FF);
    SaveFncDataBuffer[420] = (uint8)(implParam->Table[210] & 0x00FF);
    SaveFncDataBuffer[421] = (uint8)((implParam->Table[210] >> 8) & 0x00FF);
    SaveFncDataBuffer[422] = (uint8)(implParam->Table[211] & 0x00FF);
    SaveFncDataBuffer[423] = (uint8)((implParam->Table[211] >> 8) & 0x00FF);
    SaveFncDataBuffer[424] = (uint8)(implParam->Table[212] & 0x00FF);
    SaveFncDataBuffer[425] = (uint8)((implParam->Table[212] >> 8) & 0x00FF);
    SaveFncDataBuffer[426] = (uint8)(implParam->Table[213] & 0x00FF);
    SaveFncDataBuffer[427] = (uint8)((implParam->Table[213] >> 8) & 0x00FF);
    SaveFncDataBuffer[428] = (uint8)(implParam->Table[214] & 0x00FF);
    SaveFncDataBuffer[429] = (uint8)((implParam->Table[214] >> 8) & 0x00FF);
    SaveFncDataBuffer[430] = (uint8)(implParam->Table[215] & 0x00FF);
    SaveFncDataBuffer[431] = (uint8)((implParam->Table[215] >> 8) & 0x00FF);
    SaveFncDataBuffer[432] = (uint8)(implParam->Table[216] & 0x00FF);
    SaveFncDataBuffer[433] = (uint8)((implParam->Table[216] >> 8) & 0x00FF);
    SaveFncDataBuffer[434] = (uint8)(implParam->Table[217] & 0x00FF);
    SaveFncDataBuffer[435] = (uint8)((implParam->Table[217] >> 8) & 0x00FF);
    SaveFncDataBuffer[436] = (uint8)(implParam->Table[218] & 0x00FF);
    SaveFncDataBuffer[437] = (uint8)((implParam->Table[218] >> 8) & 0x00FF);
    SaveFncDataBuffer[438] = (uint8)(implParam->Table[219] & 0x00FF);
    SaveFncDataBuffer[439] = (uint8)((implParam->Table[219] >> 8) & 0x00FF);
    SaveFncDataBuffer[440] = (uint8)(implParam->Table[220] & 0x00FF);
    SaveFncDataBuffer[441] = (uint8)((implParam->Table[220] >> 8) & 0x00FF);
    SaveFncDataBuffer[442] = (uint8)(implParam->Table[221] & 0x00FF);
    SaveFncDataBuffer[443] = (uint8)((implParam->Table[221] >> 8) & 0x00FF);
    SaveFncDataBuffer[444] = (uint8)(implParam->Table[222] & 0x00FF);
    SaveFncDataBuffer[445] = (uint8)((implParam->Table[222] >> 8) & 0x00FF);
    SaveFncDataBuffer[446] = (uint8)(implParam->Table[223] & 0x00FF);
    SaveFncDataBuffer[447] = (uint8)((implParam->Table[223] >> 8) & 0x00FF);
    SaveFncDataBuffer[448] = (uint8)(implParam->Table[224] & 0x00FF);
    SaveFncDataBuffer[449] = (uint8)((implParam->Table[224] >> 8) & 0x00FF);
    SaveFncDataBuffer[450] = (uint8)(implParam->Table[225] & 0x00FF);
    SaveFncDataBuffer[451] = (uint8)((implParam->Table[225] >> 8) & 0x00FF);
    SaveFncDataBuffer[452] = (uint8)(implParam->Table[226] & 0x00FF);
    SaveFncDataBuffer[453] = (uint8)((implParam->Table[226] >> 8) & 0x00FF);
    SaveFncDataBuffer[454] = (uint8)(implParam->Table[227] & 0x00FF);
    SaveFncDataBuffer[455] = (uint8)((implParam->Table[227] >> 8) & 0x00FF);
    SaveFncDataBuffer[456] = (uint8)(implParam->Table[228] & 0x00FF);
    SaveFncDataBuffer[457] = (uint8)((implParam->Table[228] >> 8) & 0x00FF);
    SaveFncDataBuffer[458] = (uint8)(implParam->Table[229] & 0x00FF);
    SaveFncDataBuffer[459] = (uint8)((implParam->Table[229] >> 8) & 0x00FF);
    SaveFncDataBuffer[460] = (uint8)(implParam->Table[230] & 0x00FF);
    SaveFncDataBuffer[461] = (uint8)((implParam->Table[230] >> 8) & 0x00FF);
    SaveFncDataBuffer[462] = (uint8)(implParam->Table[231] & 0x00FF);
    SaveFncDataBuffer[463] = (uint8)((implParam->Table[231] >> 8) & 0x00FF);
    SaveFncDataBuffer[464] = (uint8)(implParam->Table[232] & 0x00FF);
    SaveFncDataBuffer[465] = (uint8)((implParam->Table[232] >> 8) & 0x00FF);
    SaveFncDataBuffer[466] = (uint8)(implParam->Table[233] & 0x00FF);
    SaveFncDataBuffer[467] = (uint8)((implParam->Table[233] >> 8) & 0x00FF);
    SaveFncDataBuffer[468] = (uint8)(implParam->Table[234] & 0x00FF);
    SaveFncDataBuffer[469] = (uint8)((implParam->Table[234] >> 8) & 0x00FF);
    SaveFncDataBuffer[470] = (uint8)(implParam->Table[235] & 0x00FF);
    SaveFncDataBuffer[471] = (uint8)((implParam->Table[235] >> 8) & 0x00FF);
    SaveFncDataBuffer[472] = (uint8)(implParam->Table[236] & 0x00FF);
    SaveFncDataBuffer[473] = (uint8)((implParam->Table[236] >> 8) & 0x00FF);
    SaveFncDataBuffer[474] = (uint8)(implParam->Table[237] & 0x00FF);
    SaveFncDataBuffer[475] = (uint8)((implParam->Table[237] >> 8) & 0x00FF);
    SaveFncDataBuffer[476] = (uint8)(implParam->Table[238] & 0x00FF);
    SaveFncDataBuffer[477] = (uint8)((implParam->Table[238] >> 8) & 0x00FF);
    SaveFncDataBuffer[478] = (uint8)(implParam->Table[239] & 0x00FF);
    SaveFncDataBuffer[479] = (uint8)((implParam->Table[239] >> 8) & 0x00FF);
    SaveFncDataBuffer[480] = (uint8)(implParam->Table[240] & 0x00FF);
    SaveFncDataBuffer[481] = (uint8)((implParam->Table[240] >> 8) & 0x00FF);
    SaveFncDataBuffer[482] = (uint8)(implParam->Table[241] & 0x00FF);
    SaveFncDataBuffer[483] = (uint8)((implParam->Table[241] >> 8) & 0x00FF);
    SaveFncDataBuffer[484] = (uint8)(implParam->Table[242] & 0x00FF);
    SaveFncDataBuffer[485] = (uint8)((implParam->Table[242] >> 8) & 0x00FF);
    SaveFncDataBuffer[486] = (uint8)(implParam->Table[243] & 0x00FF);
    SaveFncDataBuffer[487] = (uint8)((implParam->Table[243] >> 8) & 0x00FF);
    SaveFncDataBuffer[488] = (uint8)(implParam->Table[244] & 0x00FF);
    SaveFncDataBuffer[489] = (uint8)((implParam->Table[244] >> 8) & 0x00FF);
    SaveFncDataBuffer[490] = (uint8)(implParam->Table[245] & 0x00FF);
    SaveFncDataBuffer[491] = (uint8)((implParam->Table[245] >> 8) & 0x00FF);
    SaveFncDataBuffer[492] = (uint8)(implParam->Table[246] & 0x00FF);
    SaveFncDataBuffer[493] = (uint8)((implParam->Table[246] >> 8) & 0x00FF);
    SaveFncDataBuffer[494] = (uint8)(implParam->Table[247] & 0x00FF);
    SaveFncDataBuffer[495] = (uint8)((implParam->Table[247] >> 8) & 0x00FF);
    SaveFncDataBuffer[496] = (uint8)(implParam->Table[248] & 0x00FF);
    SaveFncDataBuffer[497] = (uint8)((implParam->Table[248] >> 8) & 0x00FF);
    SaveFncDataBuffer[498] = (uint8)(implParam->Table[249] & 0x00FF);
    SaveFncDataBuffer[499] = (uint8)((implParam->Table[249] >> 8) & 0x00FF);
    SaveFncDataBuffer[500] = (uint8)(implParam->Table[250] & 0x00FF);
    SaveFncDataBuffer[501] = (uint8)((implParam->Table[250] >> 8) & 0x00FF);
    SaveFncDataBuffer[502] = (uint8)(implParam->Table[251] & 0x00FF);
    SaveFncDataBuffer[503] = (uint8)((implParam->Table[251] >> 8) & 0x00FF);
    SaveFncDataBuffer[504] = (uint8)(implParam->Table[252] & 0x00FF);
    SaveFncDataBuffer[505] = (uint8)((implParam->Table[252] >> 8) & 0x00FF);
    SaveFncDataBuffer[506] = (uint8)(implParam->Table[253] & 0x00FF);
    SaveFncDataBuffer[507] = (uint8)((implParam->Table[253] >> 8) & 0x00FF);
    SaveFncDataBuffer[508] = (uint8)(implParam->Table[254] & 0x00FF);
    SaveFncDataBuffer[509] = (uint8)((implParam->Table[254] >> 8) & 0x00FF);
    SaveFncDataBuffer[510] = (uint8)(implParam->Table[255] & 0x00FF);
    SaveFncDataBuffer[511] = (uint8)((implParam->Table[255] >> 8) & 0x00FF);
    SaveFncDataBuffer[512] = (uint8)(implParam->Table[256] & 0x00FF);
    SaveFncDataBuffer[513] = (uint8)((implParam->Table[256] >> 8) & 0x00FF);
    SaveFncDataBuffer[514] = (uint8)implParam->sfrX;
    SaveFncDataBuffer[515] = (uint8)(implParam->maskX & 0x00FF);
    SaveFncDataBuffer[516] = (uint8)((implParam->maskX >> 8) & 0x00FF);
    SaveFncDataBuffer[517] = (uint8)(implParam->idxOffsetX & 0x00FF);
    SaveFncDataBuffer[518] = (uint8)((implParam->idxOffsetX >> 8) & 0x00FF);
    SaveFncDataBuffer[519] = (uint8)(implParam->gainX & 0x00FF);
    SaveFncDataBuffer[520] = (uint8)((implParam->gainX >> 8) & 0x00FF);
    SaveFncDataBuffer[521] = (uint8)implParam->gainXsfr;
    SaveFncDataBuffer[522] = (uint8)(implParam->offsetX & 0x00FF);
    SaveFncDataBuffer[523] = (uint8)((implParam->offsetX >> 8) & 0x00FF);
    SaveFncDataBuffer[524] = (uint8)(implParam->minX & 0x00FF);
    SaveFncDataBuffer[525] = (uint8)((implParam->minX >> 8) & 0x00FF);
    SaveFncDataBuffer[526] = (uint8)(implParam->maxX & 0x00FF);
    SaveFncDataBuffer[527] = (uint8)((implParam->maxX >> 8) & 0x00FF);

    /** - Execute save function of block */
    error = LookupTable1D_FiP16_RAM_Save(block, SaveFncDataBuffer, 528);

    return (error);
}

/**
 * @brief Backups current Mask parameters into global Mask parameter backup array.
 *
 * @param[in] maskParam Mask parameters
 *
 * @return Nothing
 */
void LookupTable1D_FiP16_RAM_BackupMP(LOOKUPTABLE1D_FIP16_RAM_MASK_PARAM* maskParam)
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
void LookupTable1D_FiP16_RAM_RestoreMP(LOOKUPTABLE1D_FIP16_RAM_MASK_PARAM* maskParam)
{
    memcpy(maskParam, MaskParamBuffer, sizeof(*maskParam));
}
