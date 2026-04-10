/**
* \file yxw_types.h
* \brief Contains Y9009 API configuration and run-time type definitions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_TYPES_H__
#define __YXW_TYPES_H__
#include "yxw_config_types.h"
#include "yxw_data_types.h"

#define  BIT(n) (1<<(n))


/**
 * \brief Data structure to hold a yxw device instance status information
 */
typedef struct
{
    YxwStates_t devState;
    YxwDigClocks_t clocks;
    YxwGainIndex_t gainIndexes;
	YxwHoppingInfo_t hoppingInfo;
    uint8_t supportChannels;     
    uint8_t profilesValid;        
    uint32_t errSource;           
    uint32_t errCode;             
    uint32_t usedGpiopins;                     
    uint16_t usedGpio3p3pins;                  
    uint8_t rxFramerNp;                         
    uint8_t orxFramerNp;                       
    uint32_t rxOutputRate_kHz;                 
    uint32_t txInputRate_kHz;                  
    uint8_t rxDualBandEnabled;                 
    uint32_t rxBandwidth_Hz;                 
    uint32_t txBandwidth_Hz;                    
    uint32_t orxBandwidth_Hz;
}yxwInfo_t;
typedef struct
{
	char firmwareCompiler[32];
	uint32_t firmwareCurrentSvnversion;
	char firmwareBuildTime[32];
	uint32_t firmwareReady;
	char strChipID[8];
}YxwCaps_t;

/**
 * \brief Data structure to hold yxw device instance settings
 */
typedef struct
{
    void*    devHalInfo;      
	yxwInfo_t YxwState;
	YxwCaps_t caps;
	uint32_t currentCalMask;
	uint32_t rfpll_done;
	uint32_t initcal_done;
	uint32_t profile_done;
	uint32_t j204b_done;
	uint32_t gpio_get_value_done;
	uint32_t waitTimeout;
	uint32_t dbg_done;
	uint32_t intr_status;
    uint32_t framer_status;
	uint32_t deframer_status;
    uint8_t  loglevel;
    YxwJesdSettings_t jesd204Settings;
}YxwDevice_t;

#endif
