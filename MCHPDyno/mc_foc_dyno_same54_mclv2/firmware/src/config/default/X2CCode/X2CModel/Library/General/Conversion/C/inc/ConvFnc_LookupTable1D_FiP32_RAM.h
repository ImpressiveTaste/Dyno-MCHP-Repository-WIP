/**
 * @file
 * @brief Load, save and conversion function for mask parameter.
 * 
 * Loading of mask parameter from target, saving of mask parameter on target and conversion of mask parameter to implementation parameter.
 */
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
#ifndef CONVFNC_LOOKUPTABLE1D_FIP32_RAM_H
#define CONVFNC_LOOKUPTABLE1D_FIP32_RAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CommonFcts.h"
#include "LookupTable1D_FiP32_RAM.h"

/** Mask parameter data structure */
typedef struct {
    float64 TableData;
    uint8 DimX;
    float64 x_min;
    float64 x_max;
} LOOKUPTABLE1D_FIP32_RAM_MASK_PARAM;

/* public prototypes */
uint8 LookupTable1D_FiP32_RAM_LoadMP(const LOOKUPTABLE1D_FIP32_RAM_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize);
uint8 LookupTable1D_FiP32_RAM_SaveMP(LOOKUPTABLE1D_FIP32_RAM *block, LOOKUPTABLE1D_FIP32_RAM_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen);
uint8 LookupTable1D_FiP32_RAM_InitMP(LOOKUPTABLE1D_FIP32_RAM *block, const LOOKUPTABLE1D_FIP32_RAM_MASK_PARAM *maskParam);
uint8 LookupTable1D_FiP32_RAM_ConvertMP(LOOKUPTABLE1D_FIP32_RAM *block, LOOKUPTABLE1D_FIP32_RAM_MASK_PARAM *maskParam);
void LookupTable1D_FiP32_RAM_BackupMP(LOOKUPTABLE1D_FIP32_RAM_MASK_PARAM *maskParam);
void LookupTable1D_FiP32_RAM_RestoreMP(LOOKUPTABLE1D_FIP32_RAM_MASK_PARAM *maskParam);

#ifdef __cplusplus
}
#endif

#endif
