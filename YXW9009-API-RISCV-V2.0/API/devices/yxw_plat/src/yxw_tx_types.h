/**
* \file yxw_tx_types.h
* \brief Contains Y9009 API Tx datapath data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef _YXW_TX_TYPES_H_
#define _YXW_TX_TYPES_H_
#include "yxw_data_types.h"
/**
 *  \brief Enum of possible Tx channel enables
 */
typedef enum
{
    YXW_TXOFF 	= 0x0,      /*!< Tx channels are disable */
    YXW_TX1	 	= 0x1,      /*!< Tx1 channel enabled */
    YXW_TX2	 	= 0x2,      /*!< Tx2 channel enabled */
    YXW_TX1TX2 	= 0x3     	/*!< Tx1 + Tx2 channels enabled */
} YxwTxChannels_t;

/**
 *  \brief Data structure to hold Y9009 Tx DDS test tone Configuration
 */
typedef struct
{
    int txTone1Freq_kHz;        /*!< Signed frequency in kHz of the desired tone1 */
    int txTone2Freq_kHz;        /*!< Signed frequency in kHz of the desired tone2 */
} YxwTxDdsTestToneCfg_t;
/**
 *  \brief Data structure to hold Y9009 Tx DDS test tone Configuration
 */
typedef enum
{
	TX_NONE_BYPASS = 0x0,		/*!< no filter bypass */
	TX_THB1_BYPASS	   = 0x4,	 /*!< txFirter Bypass*/
	TX_FIRANDTHB3_BYPASS = 0x9,	 /*!< txFirter Bypass & THB3 By pass*/
	TX_FIR_BYPASS = 0x8         /*!< THB1 By pass*/
}YxwTxFirCfg_t;

/**
 *  \brief Data structure to hold Y9009 Tx DDS test tone attenuation Configuration
 */
typedef struct
{
    uint32_t txTone0Atten;        /*!< Set attenuation of tone1 with I data,Value:0-7,0:0db,1:3db,2:6db etc, 7: close tone*/
    uint32_t txTone1Atten;        /*!< Set attenuation of tone1 with Q data,Value:0-7,0:0db,1:3db,2:6db etc, 7: close tone */
    uint32_t txTone2Atten;        /*!< Set attenuation of tone2 with I data,Value:0-7,0:0db,1:3db,2:6db etc, 7: close tone */
	uint32_t txTone3Atten;        /*!< Set attenuation of tone2 with Q data,Value:0-7,0:0db,1:3db,2:6db etc, 7: close tone */
} YxwTxDdsAttenCfg_t;

/**
 *  \brief Data structure to hold Y9009 Tx DDS test tone Configuration
 */
typedef struct 
{
	uint32_t TxCaliCnt;			 /*!< Set TxCaliCnt for PAProtect */
	uint32_t TxPowerThreshold;	 /*!< Set (dbfs=: 20*log(TxPowerThreshold/32767)) for PAProtect */
	uint32_t Enable;   			 /*!< Enable Tx1/Tx2 PAProtect */
}YxwTxPaProtectCfg_t;

#endif

