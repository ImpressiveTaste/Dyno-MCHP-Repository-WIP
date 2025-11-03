/**
 * @file
 * @brief Common functions and defines.
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
 * $LastChangedRevision: 3570 $
 * $LastChangedDate:: 2025-01-08 17:15:41 +0100#$
 */
#ifndef COMMONFCTS_H
#define COMMONFCTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Target.h"

#if defined(_MSC_VER) && (_MSC_VER < 1900) && !defined(__cplusplus)
#define X2C_INLINE  __inline
#else
#define X2C_INLINE  inline
#endif

/** Deprecated - Use getLowByte instead */
#define X2C_GET_LOW_BYTE(data)  ((data) & 0xFF)
/** Deprecated - Use getHighByte instead */
#define X2C_GET_HIGH_BYTE(data) (((uint16)(data)) >> 8)

/* Limitation */
/** Deprecated - Use limit code directly for fastest MISRA compliant code */
#define LIMIT(data, limit) 								\
					do{									\
						if((data) > (limit)){         	\
                        	(data) = (limit);			\
                        }else{							\
                        	if((data) < -(limit)){   	\
                            	(data) = -(limit);		\
                            }							\
                        }								\
                    }while(0)
                        
/* Lookup table 8 bit (256+1 values) */
/** Deprecated - Use function lookupTableInt8 */
#define LOOKUP8(Table, Index, RetVal)						\
					do{										\
						(RetVal) = Table[(uint8)(Index)];	\
					}while(0)

/* Lookup table with 16 bit interpolation (256+1 values) */
/** Deprecated - Use function lookupTableInt16 */
#ifdef __XC16__
                    #define LOOKUP16(Table, Index, RetVal)								\
					do{																	\
						int16 tbl;														\
						int32 tmp;														\
						(RetVal)  = (Table)[(uint8)((uint16)(Index) >> 8)];				\
						tbl       = (Table)[(uint8)((uint16)(Index) >> 8) + 1];			\
						tmp       = __builtin_mulss((tbl-(RetVal)),((Index) & 0x00FF));	\
						(RetVal) += ((int16)(tmp >> 8));								\
					}while(0)
#else
#define LOOKUP16(Table, Index, RetVal)													\
					do{																	\
						int16 tbl;														\
						int32 tmp;														\
						(RetVal)  = (Table)[(uint8)((uint16)(Index) >> 8)];				\
						tbl       = (Table)[(uint8)((uint16)(Index) >> 8) + 1];			\
						tmp       = (int32)(tbl-(RetVal)) * (int32)((Index) & 0x00FF);	\
						(RetVal) += ((int16)(tmp >> 8));								\
					}while(0)
#endif

/* Lookup table with 32 bit interpolation (256+1 values) */
/** Deprecated - Use function lookupTableInt32 */
#define LOOKUP32(Table, Index, RetVal)														\
					do{																		\
						int32 tbl;															\
						int64 tmp;															\
						(RetVal)  = (Table)[(uint8)((uint32)(Index) >> 24)];				\
						tbl       = (Table)[(uint8)((uint32)(Index) >> 24) + 1];			\
						tmp  	  = (int64)(tbl-(RetVal)) * (int64)((Index) & 0x00FFFFFF);	\
						(RetVal) += ((int32)(tmp >> 24));									\
					}while(0)




/* abs(x) macros for compatibality reasons (will be removed in the future) */
#define ABS_I8		getAbsValI8
#define ABS_I16		getAbsValI16
#define ABS_I32		getAbsValI32
#define ABS_I64		getAbsValI64
#define ABS_R32		getAbsValR32
#define ABS_R64		getAbsValR64

#define MAX_SCOPE_CHANNELS (8U)
#define MAX_PROTOCOLS      (2U)

#define ERROR_SUCCESS        (0U)
#define ERROR_TABLE_NOT_INIT (1U)
#define ERROR_INVALID_ID     (2U)


/****************/
/* DATA TYPES   */
/****************/
/** Implementation Parameter Save function pointer type */
typedef uint8 (*tSaveImplementationParameter) (void* block, const uint8 data[], uint16 dataLength);
/** Implementation Parameter Load function pointer type */
typedef uint8 (*tLoadImplementationParameter) (const void* block, uint8 data[], uint16* dataLength, uint16 maxSize);

/** Block functions contain block identifier and functions pointers for update, init, load & save function */
typedef struct {
    uint16 iBlockID;
    void  (*pFUpdate)(void *block);
    void  (*pFInit)(void *block);
    tLoadImplementationParameter pFLoad;
    tSaveImplementationParameter pFSave;
    void* (*pFGetAddress)(void* block, uint16 blockMember);
} tBlockFunctions;

/** Mask Parameter Save function pointer type */
typedef uint8 (*tSaveMaskParameter) (void* block, void* maskParam, const uint8 data[], uint16 dataLength);
/** Mask Parameter Load function pointer type */
typedef uint8 (*tLoadMaskParameter) (const void* block, uint8 data[], uint16* dataLength, uint16 maxSize);

typedef void (*tBackupMaskParameter) (void* maskParam);
typedef void (*tRestoreMaskParameter) (void* maskParam);
typedef uint8 (*tConvertMaskParameter) (void* block, void* maskParam);

/**
 * Mask Parameter record.
 * Contains
 * - Block Parameter ID
 * - Pointer to Block Implementation structure
 * - Pointer to Mask Parameter structure
 * - Pointer to Mask Parameter Save function
 * - Pointer to Mask Parameter Load function
 */
typedef struct tMaskParameterEntry tMaskParameterEntry;
struct tMaskParameterEntry {
	uint16 id;
	void* block;
	void* maskParameter;
	tSaveMaskParameter saveMaskParameter;
	tLoadMaskParameter loadMaskParameter;
};

/**
 * Extended Mask Parameter record.
 * Contains
 * - Block Parameter ID
 * - Number of Mask Parameters for this Block
 * - Pointer to Block Implementation structure
 * - Pointer to Mask Parameter structure
 * - Pointer to Mask Parameter Conversion function
 * - Pointer to Mask Parameter Backup function
 * - Pointer to Mask Parameter Restore function
 */
typedef struct tMaskParamExtRecord tMaskParamExtRecord;
struct tMaskParamExtRecord {
    uint16 id;
    uint8 maskParameterCount;
    void* block;
    void* maskParameter;
    tConvertMaskParameter convertMaskParameter;
    tBackupMaskParameter backupMaskParameter;
    tRestoreMaskParameter restoreMaskParameter;
};

/**
 * Mask Parameter data record.
 * Contains information to lookup Mask Parameter data via Block Parameter ID and Mask Parameter ID.
 */
typedef struct {
    uint16 blockParamId;
    uint8 maskParamId;
    void* data;
    uint16 dataSize;
    uint8 type;
} tMaskParamDataRecord;

/** Parameter table contains parameter identifier & block address */
typedef struct {
    uint16 uiParID;
    void *pAdr;
} tParameterTable;

typedef struct tIoParamIdEntry tIoParamIdEntry;
struct tIoParamIdEntry {
	uint16 id;
	uint8 size;
	void* data;
};

/** Limit save function table contains parameter identifier and pointer to limit-save function */
typedef struct {
	uint16 uiParID;
	uint8 (*pFLimitSave)(void *block, uint8 data[], uint16 dataLength);
} tLimitSaveFunctionTable;

typedef struct {
    void (*pU)(void*);
    void *pData;
} tStateMachine;

typedef enum {
	RECEIVE_ALLOWED, SEND_ALLOWED
} tCommState;

/* 'base class' interface */
typedef struct tInterface tInterface;
struct tInterface {
	void (*send)(tInterface* hwInterface, uint8 data);
	uint8 (*receive)(tInterface* hwInterface);
	uint8 (*isReceiveDataAvailable)(tInterface* hwInterface);
	uint8 (*isSendReady)(tInterface* hwInterface);
	uint8 (*getTxFifoFree)(tInterface* hwInterface);
	void (*flush)(tInterface* hwInterface);
};

/* 'base class' protocol */
typedef struct tProtocol tProtocol;
struct tProtocol {
    void (*pCommunicate)(tProtocol* protocol);
    void (*pSnd_Enable)(tProtocol* protocol);
    
    uint16 ucFRAMESize;
    uint16 ucMaxCommSize;
    uint8* ucFRAMEData;
    
    void* pServiceTable;

	tInterface* hwInterface;
};

/** Scope states */
typedef enum {
	SCOPE_IDLE=0, SCOPE_WAITTRG_OFFLINE=1, SCOPE_SAMPLE_OFFLINE=2, SCOPE_SAMPLE_ONLINE=4,
	SCOPE_WAITTRGPOS_ONLINE=5, SCOPE_WAITTRGNEG_ONLINE=6, SCOPE_TRG_SAMPLE_OFFLINE=7,
	SCOPE_WAIT_TRG_NEG_DELAY=8
} tScopeState;

typedef enum {
	EDGE_FALLING=0, EDGE_RISING=1
} tTrgEdgeType;


/** Scope block structure */
typedef struct {
	uint16 ID;								/** Scope block identifier */
	void *channelAddr[MAX_SCOPE_CHANNELS];	/** Pointer to each channel sample location (location of data to sample) */
	uint8 dataSize[MAX_SCOPE_CHANNELS];		/** Data type size for each channel */
	void *arrayAddr;						/** Pointer to data array (where all sample data is stored) */
	uint32 arraySize;						/** RAM array size */
	uint64 trgLevel;						/** Trigger level */
	uint64 trgLastValue;					/** Last value in trigger mode */
	void *trgAddr;							/** Pointer to trigger source (location of trigger source) */
	uint8 trgDataType;						/** Trigger data type */
	tScopeState state;						/** Sampling state */
	uint32 offlinePtr;						/** Pointer to current value in data array */
	uint8 dataSizeTotal;					/** Total data type size of all channels */
	uint8 noChannels;						/** Number of selected channels */
	uint16 stf;								/** Sample time factor */
	uint16 stfCnt;							/** Sample time factor counter */
	uint16 timestamp;						/** Time stamp (only used in OSM) */
	uint8 txFrameSize;						/** OSM frame size (only used in OSM) */
	int32 trgDelay;							/** Trigger delay in offline array steps */
	int32 trgEventPos;	 					/** Trigger event position (offline array index) */
	uint8 trgCountReached;					/** Indicates if minimum amount of pre/post trigger values have been sampled */
	uint32 trgCount;						/** Absolute value of trgDelay, for comparison if trigger delay values have been sampled */
	uint32 maxUsedLength;					/** Maximum used length of data array */
	tTrgEdgeType trgEdge;					/** Trigger edge (rising, falling) */
} SCOPE_MAIN;

/** DSP states */
typedef enum {
	BOOTLOADER_STATE=0,		/**< DSP executes bootloader program */
	PRG_LOADED_STATE=1,		/**< DSP executes application program */
	IDLE_STATE=2,			/**< DSP is idle */
	INIT_STATE=3,			/**< Initialization of application program */
	RUN_STATE_POWER_OFF=4,	/**< DSP executes control task, power electronics is disabled */
    RUN_STATE_POWER_ON=5	/**< DSP executes control task, power electronics is enabled */
} tDSPState;

typedef struct {
	void* dummy;
} tDynamicCodeData;

/** @brief TableStruct
 * 
 * Consists of:
 * - parameter table
 * - block function table
 * - limit-save function table
 * - DSP state
 * - event type
 * - event ID
 * - current protocol count
 * - pointer to protocol array
 * - frame program version
 * - pointer to frame program compilation date & time string
 * - pointer to Scope block
 * - pointer to dynamic code data
 * - Inport parameter table
 * - Outport parameter table
 * - Mask parameter table
 * .
 */
typedef struct tTableStruct tTableStruct;
struct tTableStruct {
    const tParameterTable* TParamTable;
    const tBlockFunctions* TFncTable;
	const tLimitSaveFunctionTable* TLimitSaveFncTable;

	/* TODO: maybe struct 'DspStatus' and only add pointer here ? */
	tDSPState DSPState;
	uint16 eventType;
	uint32 eventId;

	uint16 protocolCount;
	tProtocol* protocols[MAX_PROTOCOLS];
    
    uint16 framePrgVersion;
    uint8* framePrgCompDateTime;

	SCOPE_MAIN* piScope;

	tDynamicCodeData* dynamicCodeData;

	const tIoParamIdEntry* inportParamTable;
	const tIoParamIdEntry* outportParamTable;

	const tMaskParameterEntry* maskParameterTable;
};


/**
 * This function returns the low byte of an unsigned 16-bit value.
 * @param data 16-bit value
 * @return Low byte
 */
static X2C_INLINE uint8 getLowByte(const uint16 data)
{
    return ((uint8)(data & 0xFFU));
}

/**
 * This function returns the high byte of an unsigned 16-bit value.
 * @param data 16-bit value
 * @return High byte
 */
static X2C_INLINE uint8 getHighByte(const uint16 data)
{
    return ((uint8)(data >> 8));
}

/**
 * This function returns a value from a lookup table (256+1 values).
 * @param table Lookup table with 256+1 values
 * @param index Table index
 * @return Lookup table value
 */
static X2C_INLINE int8 lookupTableInt8(const int8 table[], const int8 index)
{
    return (table[index]);
}

/**
 * This function returns a value from a lookup table (256+1 values) with 16 bit interpolation.
 * @param table Lookup table with 256+1 values
 * @param index Table index in FiP16 format
 * @return Lookup table value
 */
static X2C_INLINE int16 lookupTableInt16(const int16 table[], const int16 index)
{
    int16 value;
    int16 tbl;
    int32 tmp;
    value = table[(uint8)((uint16)index >> 8)];
    tbl = table[(uint8)((uint16)index >> 8) + 1U];
#ifdef __XC16__
    tmp = __builtin_mulss(tbl - value, (int32)(int16)(uint16)((uint16)index & 0x00FFU));
#else
    tmp = (int32)(int16)(tbl - value) * (int32)(int16)(uint16)((uint16)index & 0x00FFU);
#endif
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    value += (int16)(tmp >> 8);
    return (value);
}

/**
 * This function returns a value from a lookup table (256+1 values) with 32 bit interpolation.
 * @param table Lookup table with 256+1 values
 * @param index Table index in FiP32 format
 * @return Lookup table value
 */
static X2C_INLINE int32 lookupTableInt32(const int32 table[], const int32 index)
{
    int32 value;
    int32 tbl;
    int64 tmp;
    value = table[(uint8)((uint32)index >> 24)];
    tbl = table[(uint8)((uint32)index >> 24) + 1U];
    tmp = (int64)(int32)(tbl - value) * (int64)(int32)(uint32)((uint32)index & 0x00FFFFFFU);
    /* RULECHECKER_comment(1:0, 1:0, check_inappropriate_int, "see X2C MISRA Deviations, DEV0002", true_no_defect) */
    value += (int32)(tmp >> 24);
    return (value);
}


/** @brief Calculation of 8-bit integer absolute value.
 *
 * out = |in|
 *
 * @param x 8-bit fixed point input value.
 * @return Absolute value of input.
 */
static X2C_INLINE int8 getAbsValI8(int8 x)
{
    if (x == ((int8)0x80))
    {
        x = (int8)0x7F;
    }
    else if (x < 0)
    {
        x = -(x);
    }
    return (x);
}

/** @brief Calculation of 16-bit integer absolute value.
 *
 * out = |in|
 *
 * @param x 16-bit fixed point input value.
 * @return Absolute value of input.
 */
static X2C_INLINE int16 getAbsValI16(int16 x)
{
    if (x == ((int16)0x8000))
    {
        x = (int16)0x7FFF;
    }
    else if (x < 0)
    {
        x = -(x);
    }
    return (x);
}

/** @brief Calculation of 32-bit integer absolute value.
 *
 * out = |in|
 *
 * @param x 32-bit fixed point input value.
 * @return Absolute value of input.
 */
static X2C_INLINE int32 getAbsValI32(int32 x)
{
    if (x == ((int32)0x80000000))
    {
        x = (int32)0x7FFFFFFF;
    }
    else if (x < 0)
    {
        x = -(x);
    }
    return (x);
}

/** @brief Calculation of 64-bit integer absolute value.
 *
 * out = |in|
 *
 * @param x 64-bit fixed point input value.
 * @return Absolute value of input.
 */
static X2C_INLINE int64 getAbsValI64(int64 x)
{
    if (x == ((int64)0x8000000000000000))
    {
        x = (int64)0x7FFFFFFFFFFFFFFF;
    }
    else if (x < 0)
    {
        x = -(x);
    }
    return (x);
}

/** @brief Calculation of single precision floating point absolute value.
 *
 * out = |in|
 *
 * @param x 32-bit floating point input value.
 * @return Absolute value of input.
 */
static X2C_INLINE float32 getAbsValR32(float32 x)
{
    if (x < 0)
    {
        x = -(x);
    }
    return (x);
}

/** @brief Calculation of double precision floating point absolute value.
 *
 * out = |in|
 *
 * @param x 64-bit floating point input value.
 * @return Absolute value of input.
 */
static X2C_INLINE float64 getAbsValR64(float64 x)
{
    if (x < 0)
    {
        x = -(x);
    }
    return (x);
}


/* Public prototypes */
void Common_Init(void* block);
void Common_Update(void* block);
uint8 Common_Load(const void* block, uint8 data[], uint16* dataLength, uint16 maxSize);
uint8 Common_Save(void* block, const uint8 data[], uint16 dataLength);
void* Common_GetAddress(void* block, uint16 elementId);
uint8 Common_InitMP(void *block, const void *maskParam);
uint8 Common_SaveMP(void* block, void* maskParam, const uint8 data[], uint16 dataLength);
uint8 Common_LoadMP(const void* block, uint8 data[], uint16* dataLength, uint16 maxSize);
uint8 Common_ConvertMP(void* block, void* maskParam);
void Common_BackupMP(void* maskParam);
void Common_RestoreMP(void* maskParam);

uint8 getIoParamIndex(const tIoParamIdEntry ioParamTbl[], uint16 paramId, uint16* index);
uint8 getBlockParamIndex(const tParameterTable* paramTable, uint16 paramId, uint16* index);
uint8 getBlockFunctionIndex(const tBlockFunctions* fncTable, uint16 blockId, uint16* index);
uint8 getMaskParamIndex(const tMaskParameterEntry* mpTable, uint16 paramId, uint16* index);
uint8 getMaskParamExtIndex(const tMaskParamExtRecord* mpTable, uint16 paramId, uint16* index);
/**
 * @brief Lookup Mask Parameter data record.
 *
 * Searches the Mask Parameter table for the requested record and returns its index.
 * The function also checks if the Mask Parameter data table is initialized.
 *
 * @param[in] mpTable Mask Parameter data table
 * @param[in] blockParamId Block Parameter ID
 * @param[in] maskParamId Mask Parameter ID
 * @param[out] index Mask Parameter record index
 *
 * @return 0 if lookup was successful
 * @return 1 if table is not initialized or Block- or Mask Parameter ID is zero
 * @return 2 if record was not found in the table
 */
uint8 getMaskParamDataTableIndex(const tMaskParamDataRecord* mpTable, uint16 blockParamId, uint8 maskParamId, uint16* index);

uint8 getQFormat(float_CoT decValue, uint8 maxBits);
int8  getQx8Value(float_CoT decValue, uint8 qFormat);
int16 getQx16Value(float_CoT decValue, uint8 qFormat);
int32 getQx32Value(float_CoT decValue, uint8 qFormat);
int8  getAQx8Value(float_CoT decValue, uint8 qFormat);
int16 getAQx16Value(float_CoT decValue, uint8 qFormat);
int32 getAQx32Value(float_CoT decValue, uint8 qFormat);

/**
 * @brief Function is created in generated Model file returning Model sample time.
 *
 * @return Model sample time
 */
extern float_CoT getModelSampleTime(void);

#ifdef __cplusplus
}
#endif

#endif
