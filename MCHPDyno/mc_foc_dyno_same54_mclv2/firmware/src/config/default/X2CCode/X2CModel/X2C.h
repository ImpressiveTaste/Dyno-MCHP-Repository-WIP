/**
 * @file
 * @brief Generated model file.
 * 
 * Date:  2025-04-25 09:49
 * 
 * X2C-Version: 6.5.3600
 * X2C-Edition: Free
 */
/* This file is part of X2C. http://x2c.lcm.at/                                                                       */

/* Model: MC_FOC_DYNO_SAME54_MCLV2                                                                                    */
/* Date:  2025-04-25 09:49                                                                                            */

/* X2C-Version: 6.5.3600                                                                                              */
/* X2C-Edition: Free                                                                                                  */

#ifndef X2C_H
#define X2C_H

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************/
/**     Includes                                                                                                     **/
/**********************************************************************************************************************/
/* Common includes                                                                                                    */
#include "Constant_Bool.h"
#include "Gain_FiP16.h"
#include "TypeConv_Bool_FiP16.h"
#include "Gain_Float32.h"
#include "RateLimiter_FiP16.h"
#include "Int2Real_FiP16_Float32.h"
#include "Real2Int_Float32_FiP8.h"
#include "Negation_FiP16.h"
#include "ManualSwitch_FiP16.h"
#include "Sum_Float32.h"
#include "Constant_FiP16.h"
#include "Not_Bool.h"
#include "PI_FiP16.h"
#include "ManualSwitch_Bool.h"
#include "Saturation_FiP16.h"
#include "LookupTable_FiP16.h"
#include "PILimit_FiP16.h"
#include "AutoSwitch_FiP16.h"
#include "Sequencer_FiP16.h"
#include "SinGen_FiP16.h"
#include "Mult_Float32.h"
#include "Sub_FiP16.h"
#include "Clarke_Park_MCHP_FiP16.h"
#include "Constant_Float32.h"
#include "Average_FiP16.h"
#include "Abs_FiP16.h"
#include "TypeConv_FiP16_Bool.h"
#include "Selector_FiP16.h"
#include "Park_Clarke_inv_SVM_MCHP_FiP16.h"
#include "Add_FiP16.h"
#include "Sign_FiP16.h"
#include "SaveSignal_Float32.h"
#include "uAdd_FiP16.h"
#include "Mult_FiP16.h"
#include "Scope_Main.h"
#include "CommonFcts.h"

/**********************************************************************************************************************/
/**     Defines                                                                                                      **/
/**********************************************************************************************************************/
#define FUNCTIONS \
    CONSTANT_BOOL_FUNCTIONS , \
    GAIN_FIP16_FUNCTIONS , \
    TYPECONV_BOOL_FIP16_FUNCTIONS , \
    GAIN_FLOAT32_FUNCTIONS , \
    RATELIMITER_FIP16_FUNCTIONS , \
    INT2REAL_FIP16_FLOAT32_FUNCTIONS , \
    REAL2INT_FLOAT32_FIP8_FUNCTIONS , \
    NEGATION_FIP16_FUNCTIONS , \
    MANUALSWITCH_FIP16_FUNCTIONS , \
    SUM_FLOAT32_FUNCTIONS , \
    CONSTANT_FIP16_FUNCTIONS , \
    NOT_BOOL_FUNCTIONS , \
    PI_FIP16_FUNCTIONS , \
    MANUALSWITCH_BOOL_FUNCTIONS , \
    SATURATION_FIP16_FUNCTIONS , \
    LOOKUPTABLE_FIP16_FUNCTIONS , \
    PILIMIT_FIP16_FUNCTIONS , \
    AUTOSWITCH_FIP16_FUNCTIONS , \
    SEQUENCER_FIP16_FUNCTIONS , \
    SINGEN_FIP16_FUNCTIONS , \
    MULT_FLOAT32_FUNCTIONS , \
    SUB_FIP16_FUNCTIONS , \
    CLARKE_PARK_MCHP_FIP16_FUNCTIONS , \
    CONSTANT_FLOAT32_FUNCTIONS , \
    AVERAGE_FIP16_FUNCTIONS , \
    ABS_FIP16_FUNCTIONS , \
    TYPECONV_FIP16_BOOL_FUNCTIONS , \
    SELECTOR_FIP16_FUNCTIONS , \
    PARK_CLARKE_INV_SVM_MCHP_FIP16_FUNCTIONS , \
    ADD_FIP16_FUNCTIONS , \
    SIGN_FIP16_FUNCTIONS , \
    SAVESIGNAL_FLOAT32_FUNCTIONS , \
    UADD_FIP16_FUNCTIONS , \
    MULT_FIP16_FUNCTIONS , \
    SCOPE_MAIN_FUNCTIONS

#define PARAMETER_TABLE \
    { 1u, &x2cModel.blocks.bAverage } , \
    { 2u, &x2cModel.blocks.sDyno.bAdd } , \
    { 3u, &x2cModel.blocks.sDyno.bAdd1 } , \
    { 4u, &x2cModel.blocks.sDyno.bAdd2 } , \
    { 5u, &x2cModel.blocks.sDyno.bAutoSwitch } , \
    { 6u, &x2cModel.blocks.sDyno.bConstT5 } , \
    { 7u, &x2cModel.blocks.sDyno.bConstT6 } , \
    { 8u, &x2cModel.blocks.sDyno.bConstT7 } , \
    { 9u, &x2cModel.blocks.sDyno.bConstant } , \
    { 10u, &x2cModel.blocks.sDyno.bConstant4 } , \
    { 11u, &x2cModel.blocks.sDyno.bConstant6 } , \
    { 12u, &x2cModel.blocks.sDyno.bGainT5 } , \
    { 13u, &x2cModel.blocks.sDyno.bGainT6 } , \
    { 14u, &x2cModel.blocks.sDyno.bGainT7 } , \
    { 15u, &x2cModel.blocks.sDyno.bLookupTable } , \
    { 16u, &x2cModel.blocks.sDyno.bLookupTable1 } , \
    { 17u, &x2cModel.blocks.sDyno.bLookupTable2 } , \
    { 18u, &x2cModel.blocks.sDyno.bMult } , \
    { 19u, &x2cModel.blocks.sDyno.bReal2Int } , \
    { 20u, &x2cModel.blocks.sDyno.bSelector } , \
    { 21u, &x2cModel.blocks.sDyno.bSinGen1 } , \
    { 22u, &x2cModel.blocks.sDyno.bSin_load } , \
    { 23u, &x2cModel.blocks.sDyno.bconst_torque } , \
    { 24u, &x2cModel.blocks.sDyno.bfan_gain } , \
    { 25u, &x2cModel.blocks.sDyno.bsine_A } , \
    { 26u, &x2cModel.blocks.sDyno.bsine_F } , \
    { 27u, &x2cModel.blocks.sDyno.bswitch_mech_el } , \
    { 28u, &x2cModel.blocks.sDyno.bt_load_f } , \
    { 29u, &x2cModel.blocks.sDyno.bt_load_high } , \
    { 30u, &x2cModel.blocks.sDyno.bt_load_low } , \
    { 31u, &x2cModel.blocks.sDyno.bt_load_rate } , \
    { 32u, &x2cModel.blocks.sDyno.btorque_mode } , \
    { 33u, &x2cModel.blocks.bDynoMotor } , \
    { 34u, &x2cModel.blocks.sFOCmain.bAdd } , \
    { 35u, &x2cModel.blocks.sFOCmain.bClarke_Park_MCHP } , \
    { 36u, &x2cModel.blocks.sFOCmain.bGain } , \
    { 37u, &x2cModel.blocks.sFOCmain.bGain1 } , \
    { 38u, &x2cModel.blocks.sFOCmain.bGain2 } , \
    { 39u, &x2cModel.blocks.sFOCmain.bGain3 } , \
    { 40u, &x2cModel.blocks.sFOCmain.bGain4 } , \
    { 41u, &x2cModel.blocks.sFOCmain.bGain5 } , \
    { 42u, &x2cModel.blocks.sFOCmain.bGain6 } , \
    { 43u, &x2cModel.blocks.sFOCmain.bInt2Real } , \
    { 44u, &x2cModel.blocks.sFOCmain.bInt2Real1 } , \
    { 45u, &x2cModel.blocks.sFOCmain.bInt2Real2 } , \
    { 46u, &x2cModel.blocks.sFOCmain.bInt2Real3 } , \
    { 47u, &x2cModel.blocks.sFOCmain.bInt2Real4 } , \
    { 48u, &x2cModel.blocks.sFOCmain.bInt2Real5 } , \
    { 49u, &x2cModel.blocks.sFOCmain.bManualSwitch } , \
    { 50u, &x2cModel.blocks.sFOCmain.bManualSwitch1 } , \
    { 51u, &x2cModel.blocks.sFOCmain.bMult } , \
    { 52u, &x2cModel.blocks.sFOCmain.bMult1 } , \
    { 53u, &x2cModel.blocks.sFOCmain.bMult2 } , \
    { 54u, &x2cModel.blocks.sFOCmain.bNot } , \
    { 55u, &x2cModel.blocks.sFOCmain.bOpenLoop_Vd } , \
    { 56u, &x2cModel.blocks.sFOCmain.bOpenLoop_Vq } , \
    { 57u, &x2cModel.blocks.sFOCmain.bPI_flux } , \
    { 58u, &x2cModel.blocks.sFOCmain.bPI_torque } , \
    { 59u, &x2cModel.blocks.sFOCmain.bPark_Clarke_inv_SVM_MCHP } , \
    { 60u, &x2cModel.blocks.sFOCmain.bSaturation } , \
    { 61u, &x2cModel.blocks.sFOCmain.bSaturation1 } , \
    { 62u, &x2cModel.blocks.sFOCmain.bSub_flux } , \
    { 63u, &x2cModel.blocks.sFOCmain.bSub_torque } , \
    { 64u, &x2cModel.blocks.sFOCmain.bSum } , \
    { 65u, &x2cModel.blocks.sFOCmain.bTypeConv2 } , \
    { 66u, &x2cModel.blocks.sFOCmain.bbreakPowerW } , \
    { 67u, &x2cModel.blocks.sFOCmain.bforced_current } , \
    { 68u, &x2cModel.blocks.sFOCmain.bforced_current1 } , \
    { 69u, &x2cModel.blocks.sFOCmain.btheta_offset } , \
    { 70u, &x2cModel.blocks.sFOCmain.buAdd } , \
    { 71u, &x2cModel.blocks.bFlux_SP1 } , \
    { 72u, &x2cModel.blocks.sMotor.bAutoSwitch } , \
    { 73u, &x2cModel.blocks.sMotor.bConstant } , \
    { 74u, &x2cModel.blocks.sMotor.bConstant1 } , \
    { 75u, &x2cModel.blocks.sMotor.bConstant3 } , \
    { 76u, &x2cModel.blocks.sMotor.bConstant4 } , \
    { 77u, &x2cModel.blocks.sMotor.bConstant6 } , \
    { 78u, &x2cModel.blocks.sMotor.bGain } , \
    { 79u, &x2cModel.blocks.sMotor.bGain1 } , \
    { 80u, &x2cModel.blocks.sMotor.bGain3 } , \
    { 81u, &x2cModel.blocks.sMotor.bInt2Real } , \
    { 82u, &x2cModel.blocks.sMotor.bMAX_SPEED } , \
    { 83u, &x2cModel.blocks.sMotor.bManualSwitch } , \
    { 84u, &x2cModel.blocks.sMotor.bPILimit } , \
    { 85u, &x2cModel.blocks.sMotor.bReal2Int } , \
    { 86u, &x2cModel.blocks.sMotor.bSelector } , \
    { 87u, &x2cModel.blocks.sMotor.bSinGen1 } , \
    { 88u, &x2cModel.blocks.sMotor.bSubSpeed } , \
    { 89u, &x2cModel.blocks.sMotor.sSuperBlock.bConstant2 } , \
    { 90u, &x2cModel.blocks.sMotor.sSuperBlock.bGain2 } , \
    { 91u, &x2cModel.blocks.sMotor.sSuperBlock.bSub } , \
    { 92u, &x2cModel.blocks.sMotor.bTypeConv } , \
    { 93u, &x2cModel.blocks.sMotor.bconst_speed } , \
    { 94u, &x2cModel.blocks.sMotor.bconst_speed1 } , \
    { 95u, &x2cModel.blocks.sMotor.bconst_torque_motor } , \
    { 96u, &x2cModel.blocks.sMotor.bmotor_torquemode } , \
    { 97u, &x2cModel.blocks.sMotor.bomega_rpm } , \
    { 98u, &x2cModel.blocks.sMotor.bspeedPI_limit } , \
    { 99u, &x2cModel.blocks.sMotor.bspeed_mode } , \
    { 100u, &x2cModel.blocks.sMotor.bt_load_rate } , \
    { 101u, &x2cModel.blocks.sMotor.bt_speed_f } , \
    { 102u, &x2cModel.blocks.sMotor.bt_speed_high } , \
    { 103u, &x2cModel.blocks.sMotor.bt_speed_low } , \
    { 104u, &x2cModel.blocks.sQEI_init.bGain1 } , \
    { 105u, &x2cModel.blocks.sQEI_init.bNot } , \
    { 106u, &x2cModel.blocks.sQEI_init.bTypeConv } , \
    { 107u, &x2cModel.blocks.sQEI_init.bTypeConv1 } , \
    { 108u, &x2cModel.blocks.bSequencer } , \
    { 109u, &x2cModel.blocks.sdyno_quadrant.bAbs } , \
    { 110u, &x2cModel.blocks.sdyno_quadrant.bAutoSwitch } , \
    { 111u, &x2cModel.blocks.sdyno_quadrant.bConstant1 } , \
    { 112u, &x2cModel.blocks.sdyno_quadrant.bConstant2 } , \
    { 113u, &x2cModel.blocks.sdyno_quadrant.bManualSwitch3 } , \
    { 114u, &x2cModel.blocks.sdyno_quadrant.bMult1 } , \
    { 115u, &x2cModel.blocks.sdyno_quadrant.bMult2 } , \
    { 116u, &x2cModel.blocks.sdyno_quadrant.bNegation } , \
    { 117u, &x2cModel.blocks.sdyno_quadrant.bRateLimiter } , \
    { 118u, &x2cModel.blocks.sdyno_quadrant.bSaturation } , \
    { 119u, &x2cModel.blocks.sdyno_quadrant.bSign1 } , \
    { 120u, &x2cModel.blocks.sdyno_quadrant.bSpeedAverage } , \
    { 121u, &x2cModel.blocks.sdyno_quadrant.bTorque_SP } , \
    { 141u, &x2cScope }

#define INPORT_PARAMID_TABLE \
    { 122u, 2u, &x2cModel.inports.bCPU_LOAD } , \
    { 123u, 2u, &x2cModel.inports.bI_a } , \
    { 124u, 2u, &x2cModel.inports.bI_b } , \
    { 125u, 2u, &x2cModel.inports.bI_sum } , \
    { 126u, 2u, &x2cModel.inports.bQEI_POS } , \
    { 127u, 2u, &x2cModel.inports.bQEI_POS_MECH } , \
    { 128u, 2u, &x2cModel.inports.bQEI_VEL } , \
    { 129u, 2u, &x2cModel.inports.bS2 } , \
    { 130u, 2u, &x2cModel.inports.bS3 } , \
    { 131u, 2u, &x2cModel.inports.bV_DCLINK } , \
    { 132u, 2u, &x2cModel.inports.bV_POT } 

#define OUTPORT_PARAMID_TABLE \
    { 133u, 2u, &x2cModel.outports.bHOME_INIT } , \
    { 134u, 2u, &x2cModel.outports.bIq } , \
    { 135u, 2u, &x2cModel.outports.bLED_D17 } , \
    { 136u, 2u, &x2cModel.outports.bLED_D2 } , \
    { 137u, 2u, &x2cModel.outports.bPWM1 } , \
    { 138u, 2u, &x2cModel.outports.bPWM2 } , \
    { 139u, 2u, &x2cModel.outports.bPWM3 } , \
    { 140u, 2u, &x2cModel.outports.sdyno_quadrant.bSPEED_ABS } 


/**********************************************************************************************************************/
/**                                            Model structure definition                                            **/
/**********************************************************************************************************************/
struct x2cModel {
/*  Control Blocks                                                                                                    */
    struct {
/*      Sub-system Dyno                                                                                               */
        struct {
            ADD_FIP16 bAdd;
            ADD_FIP16 bAdd1;
            ADD_FIP16 bAdd2;
            AUTOSWITCH_FIP16 bAutoSwitch;
            CONSTANT_FIP16 bConstT5;
            CONSTANT_FIP16 bConstT6;
            CONSTANT_FIP16 bConstT7;
            CONSTANT_FIP16 bConstant;
            CONSTANT_FIP16 bConstant4;
            CONSTANT_BOOL bConstant6;
            GAIN_FIP16 bGainT5;
            GAIN_FIP16 bGainT6;
            GAIN_FIP16 bGainT7;
            LOOKUPTABLE_FIP16 bLookupTable;
            LOOKUPTABLE_FIP16 bLookupTable1;
            LOOKUPTABLE_FIP16 bLookupTable2;
            MULT_FIP16 bMult;
            REAL2INT_FLOAT32_FIP8 bReal2Int;
            SELECTOR_FIP16 bSelector;
            SINGEN_FIP16 bSinGen1;
            SINGEN_FIP16 bSin_load;
            CONSTANT_FIP16 bconst_torque;
            GAIN_FIP16 bfan_gain;
            CONSTANT_FIP16 bsine_A;
            CONSTANT_FIP16 bsine_F;
            MANUALSWITCH_FIP16 bswitch_mech_el;
            CONSTANT_FIP16 bt_load_f;
            CONSTANT_FIP16 bt_load_high;
            CONSTANT_FIP16 bt_load_low;
            RATELIMITER_FIP16 bt_load_rate;
            CONSTANT_FLOAT32 btorque_mode;
        } sDyno;
/*      Sub-system FOCmain                                                                                            */
        struct {
            ADD_FIP16 bAdd;
            CLARKE_PARK_MCHP_FIP16 bClarke_Park_MCHP;
            GAIN_FLOAT32 bGain;
            GAIN_FIP16 bGain1;
            GAIN_FLOAT32 bGain2;
            GAIN_FLOAT32 bGain3;
            GAIN_FLOAT32 bGain4;
            GAIN_FLOAT32 bGain5;
            GAIN_FLOAT32 bGain6;
            INT2REAL_FIP16_FLOAT32 bInt2Real;
            INT2REAL_FIP16_FLOAT32 bInt2Real1;
            INT2REAL_FIP16_FLOAT32 bInt2Real2;
            INT2REAL_FIP16_FLOAT32 bInt2Real3;
            INT2REAL_FIP16_FLOAT32 bInt2Real4;
            INT2REAL_FIP16_FLOAT32 bInt2Real5;
            MANUALSWITCH_FIP16 bManualSwitch;
            MANUALSWITCH_FIP16 bManualSwitch1;
            MULT_FLOAT32 bMult;
            MULT_FLOAT32 bMult1;
            MULT_FLOAT32 bMult2;
            NOT_BOOL bNot;
            CONSTANT_FIP16 bOpenLoop_Vd;
            CONSTANT_FIP16 bOpenLoop_Vq;
            PI_FIP16 bPI_flux;
            PI_FIP16 bPI_torque;
            PARK_CLARKE_INV_SVM_MCHP_FIP16 bPark_Clarke_inv_SVM_MCHP;
            SATURATION_FIP16 bSaturation;
            SATURATION_FIP16 bSaturation1;
            SUB_FIP16 bSub_flux;
            SUB_FIP16 bSub_torque;
            SUM_FLOAT32 bSum;
            TYPECONV_FIP16_BOOL bTypeConv2;
            SAVESIGNAL_FLOAT32 bbreakPowerW;
            CONSTANT_FIP16 bforced_current;
            CONSTANT_FLOAT32 bforced_current1;
            CONSTANT_FIP16 btheta_offset;
            UADD_FIP16 buAdd;
        } sFOCmain;
/*      Sub-system Motor                                                                                              */
        struct {
/*          Sub-system SuperBlock                                                                                     */
            struct {
                CONSTANT_FIP16 bConstant2;
                GAIN_FIP16 bGain2;
                SUB_FIP16 bSub;
            } sSuperBlock;
            AUTOSWITCH_FIP16 bAutoSwitch;
            CONSTANT_BOOL bConstant;
            CONSTANT_FIP16 bConstant1;
            CONSTANT_FIP16 bConstant3;
            CONSTANT_FIP16 bConstant4;
            CONSTANT_BOOL bConstant6;
            GAIN_FIP16 bGain;
            GAIN_FIP16 bGain1;
            GAIN_FIP16 bGain3;
            INT2REAL_FIP16_FLOAT32 bInt2Real;
            GAIN_FLOAT32 bMAX_SPEED;
            MANUALSWITCH_BOOL bManualSwitch;
            PILIMIT_FIP16 bPILimit;
            REAL2INT_FLOAT32_FIP8 bReal2Int;
            SELECTOR_FIP16 bSelector;
            SINGEN_FIP16 bSinGen1;
            SUB_FIP16 bSubSpeed;
            TYPECONV_FIP16_BOOL bTypeConv;
            CONSTANT_FIP16 bconst_speed;
            CONSTANT_FIP16 bconst_speed1;
            CONSTANT_FIP16 bconst_torque_motor;
            MANUALSWITCH_FIP16 bmotor_torquemode;
            SAVESIGNAL_FLOAT32 bomega_rpm;
            CONSTANT_FIP16 bspeedPI_limit;
            CONSTANT_FLOAT32 bspeed_mode;
            RATELIMITER_FIP16 bt_load_rate;
            CONSTANT_FIP16 bt_speed_f;
            CONSTANT_FIP16 bt_speed_high;
            CONSTANT_FIP16 bt_speed_low;
        } sMotor;
/*      Sub-system QEI_init                                                                                           */
        struct {
            GAIN_FIP16 bGain1;
            NOT_BOOL bNot;
            TYPECONV_FIP16_BOOL bTypeConv;
            TYPECONV_BOOL_FIP16 bTypeConv1;
        } sQEI_init;
/*      Sub-system dyno_quadrant                                                                                      */
        struct {
            ABS_FIP16 bAbs;
            AUTOSWITCH_FIP16 bAutoSwitch;
            CONSTANT_BOOL bConstant1;
            CONSTANT_FIP16 bConstant2;
            MANUALSWITCH_FIP16 bManualSwitch3;
            MULT_FIP16 bMult1;
            MULT_FIP16 bMult2;
            NEGATION_FIP16 bNegation;
            RATELIMITER_FIP16 bRateLimiter;
            SATURATION_FIP16 bSaturation;
            SIGN_FIP16 bSign1;
            AVERAGE_FIP16 bSpeedAverage;
            CONSTANT_FIP16 bTorque_SP;
        } sdyno_quadrant;
        AVERAGE_FIP16 bAverage;
        MANUALSWITCH_FIP16 bDynoMotor;
        CONSTANT_FIP16 bFlux_SP1;
        SEQUENCER_FIP16 bSequencer;
    } blocks;
/*  Inports                                                                                                           */
    struct {
        int16 bCPU_LOAD;
        int16 bI_a;
        int16 bI_b;
        int16 bI_sum;
        int16 bQEI_POS;
        int16 bQEI_POS_MECH;
        int16 bQEI_VEL;
        int16 bS2;
        int16 bS3;
        int16 bV_DCLINK;
        int16 bV_POT;
    } inports;
/*  Outports                                                                                                          */
    struct {
/*      Sub-system dyno_quadrant                                                                                      */
        struct {
            int16* bSPEED_ABS;
        } sdyno_quadrant;
        int16* bHOME_INIT;
        int16* bIq;
        int16* bLED_D17;
        int16* bLED_D2;
        int16* bPWM1;
        int16* bPWM2;
        int16* bPWM3;
    } outports;
};

/**********************************************************************************************************************/
/**     Externals                                                                                                    **/
/**********************************************************************************************************************/
extern struct x2cModel x2cModel;

extern const uint32 x2cModelIdentifier;

extern const tBlockFunctions blockFunctionTable[];
extern const tParameterTable parameterIdTable[];
extern const tIoParamIdEntry inportParamIdTable[];
extern const tIoParamIdEntry outportParamIdTable[];
extern const tMaskParameterEntry maskParamIdTable[];
extern const tMaskParamExtRecord maskParamExtTable[];
extern const tMaskParamDataRecord maskParamDataTable[];

#define X2C_UPDATE_10_DIVIDER 10

/**********************************************************************************************************************/
/**     Prototypes                                                                                                   **/
/**********************************************************************************************************************/
void X2C_Init(void);
void X2C_Update(void);
void X2C_Update_1(void);
void X2C_Update_10(void);

#ifdef __cplusplus
}
#endif

#endif
