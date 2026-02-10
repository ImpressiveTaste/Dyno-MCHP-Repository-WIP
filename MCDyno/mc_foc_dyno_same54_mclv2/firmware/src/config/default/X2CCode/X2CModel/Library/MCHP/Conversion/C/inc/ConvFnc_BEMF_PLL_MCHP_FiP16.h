/**
 * @file
 * @brief Load, save and conversion function for mask parameter.
 * 
 * Loading of mask parameter from target, saving of mask parameter on target and conversion of mask parameter to implementation parameter.
 */
/*
 * This file is part of X2C. http://x2c.lcm.at/
 * $LastChangedRevision: 2830 $
 * $LastChangedDate:: 2023-05-08 16:06:26 +0200#$
 */
/* USERCODE-BEGIN:Description                                                                                         */
/* Description: */
/* USERCODE-END:Description                                                                                           */
#ifndef CONVFNC_BEMF_PLL_MCHP_FIP16_H
#define CONVFNC_BEMF_PLL_MCHP_FIP16_H

#ifdef __cplusplus
extern "C" {
#endif

#include "CommonFcts.h"
#include "BEMF_PLL_MCHP_FiP16.h"

/** Mask parameter data structure */
typedef struct {
    float64 Ls;
    float64 Rs;
    float64 U0;
    float64 I0;
    float64 CurrentSampleFactor;
    uint8 n;
    float64 Gain;
    float64 p;
    float64 n_max;
    float64 ts_fact;
} BEMF_PLL_MCHP_FIP16_MASK_PARAM;

/* public prototypes */
uint8 BEMF_PLL_MCHP_FiP16_LoadMP(const BEMF_PLL_MCHP_FIP16_MASK_PARAM *maskParam, uint8 data[], uint16* dataLen, uint16 maxSize);
uint8 BEMF_PLL_MCHP_FiP16_SaveMP(BEMF_PLL_MCHP_FIP16 *block, BEMF_PLL_MCHP_FIP16_MASK_PARAM *maskParam, const uint8 data[], uint16 dataLen);
uint8 BEMF_PLL_MCHP_FiP16_InitMP(BEMF_PLL_MCHP_FIP16 *block, const BEMF_PLL_MCHP_FIP16_MASK_PARAM *maskParam);
uint8 BEMF_PLL_MCHP_FiP16_ConvertMP(BEMF_PLL_MCHP_FIP16 *block, BEMF_PLL_MCHP_FIP16_MASK_PARAM *maskParam);
void BEMF_PLL_MCHP_FiP16_BackupMP(BEMF_PLL_MCHP_FIP16_MASK_PARAM *maskParam);
void BEMF_PLL_MCHP_FiP16_RestoreMP(BEMF_PLL_MCHP_FIP16_MASK_PARAM *maskParam);

#ifdef __cplusplus
}
#endif

#endif
