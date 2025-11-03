/**
 * @file
 * @brief Load, save and conversion function for mask parameter.
 * 
 * Loading of mask parameter from target, saving of mask parameter on target and conversion of mask parameter to implementation parameter.
 */
/*
 * Copyright (c) {YEAR}, {ORGANIZATION_NAME}, {ORGANIZATION_WEB}
 * All rights reserved.
 */
/*
 * This file is licensed according to the BSD 3-clause license as follows:
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the "{ORGANIZATION_NAME}" nor
 *     the names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL "{ORGANIZATION_NAME}" BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * This file is part of X2C. http://x2c.lcm.at/
 * $LastChangedRevision: 2457 $
 * $LastChangedDate:: 2022-02-11 12:58:55 +0100#$
 */
/* USERCODE-BEGIN:Description                                                                                         */
/* Description: */
/* USERCODE-END:Description                                                                                           */
#ifndef CONVFNC_PERTURBATIONGEN_FLOAT64_H
#define CONVFNC_PERTURBATIONGEN_FLOAT64_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CommonFcts.h"
#include "PerturbationGen_Float64.h"

/** Mask parameter data structure */
typedef struct {
    float64 fmax;
    float64 LL;
    float64 LH;
    float64 Tr;
    float64 Tf;
    float64 ts_fact;
} PERTURBATIONGEN_FLOAT64_MASK_PARAM;

/* public prototypes */
uint8 PerturbationGen_Float64_LoadMP(const PERTURBATIONGEN_FLOAT64_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize);
uint8 PerturbationGen_Float64_SaveMP(PERTURBATIONGEN_FLOAT64 *block, PERTURBATIONGEN_FLOAT64_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen);
uint8 PerturbationGen_Float64_InitMP(PERTURBATIONGEN_FLOAT64 *block, const PERTURBATIONGEN_FLOAT64_MASK_PARAM *maskParam);
uint8 PerturbationGen_Float64_ConvertMP(PERTURBATIONGEN_FLOAT64 *block, PERTURBATIONGEN_FLOAT64_MASK_PARAM *maskParam);
void PerturbationGen_Float64_BackupMP(PERTURBATIONGEN_FLOAT64_MASK_PARAM *maskParam);
void PerturbationGen_Float64_RestoreMP(PERTURBATIONGEN_FLOAT64_MASK_PARAM *maskParam);

#ifdef __cplusplus
}
#endif

#endif
