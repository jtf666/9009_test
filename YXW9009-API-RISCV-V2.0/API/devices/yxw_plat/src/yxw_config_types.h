/**
* \file Yxw_config_types.h
* \brief Contains Y9009 API configuration settings data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef _YXW_CONFIG_TYPES_H_
#define _YXW_CONFIG_TYPES_H_
#include "yxw_agc_types.h"
#include "yxw_cals_types.h"
#include "yxw_data_types.h"
#include "yxw_hopping_types.h"
#include "yxw_jesd204b_types.h"
#include "yxw_radio_types.h"
#include "yxw_riscv_types.h"
#include "yxw_rx_types.h"
#include "yxw_tx_types.h"

typedef enum
{
	YXW_TX_FILTER 	= 0x0,	  /*!< Tx Filter*/
	YXW_RX_FILTER 	= 0x1,	  /*!< Rx Filter*/
	YXW_ORX_FILTER	= 0x2	  /*!< ORx Filter*/
}YxwFilterType_t;

typedef enum
{
	YXW_STATE_POWERONRESET 	= 0x00,
	YXW_STATE_INITIALIZED  	= 0x01,
	YXW_STATE_RISCVLOADED  	= 0x02,
	YXW_STATE_CALS_RUN     	= 0x04,
	YXW_STATE_RADIOON      	= 0x08
} YxwStates_t;

/* interfaces for the drivers */
typedef enum
{
	YXW_RFPLLMCS_NOSYNC 				= 0x0,
	YXW_RFPLLMCS_INIT_AND_SYNC 		= 0x1,
	YXW_RFPLLMCS_INIT_AND_1TRACK 	= 0x2,
	YXW_RFPLLMCS_INIT_AND_CONTTRACK  = 0x3
} YxwRfPllMcs_t;

/**
 *  \brief Data structure to hold Y9009 FIR filter settings
 */
typedef struct
{
	sint16_t gain_dB;
	uint32_t numFirCoefs;
	sint16_t *coefs;
} YxwFir_t;

/**
 *  \brief Data structure to hold settings
 * \for the current tx specific use case profile
 */
typedef struct
{
	YxwFir_t txFir;
	uint8_t txFirBypass;
	uint8_t txFirInterpolation;
	uint8_t thb1Interpolation;
	uint8_t thb2Interpolation;
	uint8_t thb3Interpolation;
	uint32_t txInputRate_kHz;
	uint32_t primarySigBandwidth_Hz;
	uint32_t rfBandwidth_Hz;
	uint32_t txBbf3dBCorner_kHz;
} YxwTxProfile_t;

/**
 *  \brief Data structure to hold settings
 * \for the current Rx specific use case profile
 */
typedef struct
{
	YxwFir_t rxFir;
	uint8_t rxFirBypass;
	uint8_t rxFirDecimation;
	uint8_t rhb1Decimation;
	uint32_t rxOutputRate_kHz;
	uint32_t rfBandwidth_Hz;
	uint32_t rxBbf3dBCorner_kHz;
} YxwRxProfile_t;

/**
 *  \brief Data structure to hold settings
 * \for the current oRx specific use case profile
 */
typedef struct
{
	YxwFir_t rxFir;
	uint8_t orxFirBypass;
	uint8_t orxFirDecimation;
	uint32_t orxOutputRate_kHz;
	uint32_t rfBandwidth_Hz;
	uint32_t rxBbf3dBCorner_kHz;
} YxwORxProfile_t;

/**
 *  \brief Data structure to hold digital clock settings
 */
typedef struct
{
	uint32_t deviceClock_kHz;
	uint64_t rfpll_frequency_Hz;
	uint64_t auxpll_frequency_Hz;
	uint8_t rfPllUseExternalLo;
	YxwRfPllMcs_t rfPllPhaseSyncMode;
} YxwDigClocks_t;

/**
 * \brief Data structure to hold ORx gain control
 * \settings for initialization and during use
*/
typedef struct
{
	YxwGainMode_t orxgainMode;
	YxwCtrlWay_t   orxmgcMode;
	uint8_t orx1GainIndex;
	uint8_t orx2GainIndex;
	uint8_t orx1MaxGainIndex;
	uint8_t orx1MinGainIndex;
	uint8_t orx2MaxGainIndex;
	uint8_t orx2MinGainIndex;
} YxwORxGainControl_t;

/**
 * \brief Data structure to hold Tx gain control
 * \settings for initialization and during use
*/
typedef struct
{
	YxwGainMode_t txgainMode;
	YxwCtrlWay_t   txmgcMode;
	uint8_t tx1GainIndex;
	uint8_t tx2GainIndex;
	uint8_t tx1MaxGainIndex;
	uint8_t tx1MinGainIndex;
	uint8_t tx2MaxGainIndex;
	uint8_t tx2MinGainIndex;
} YxwTxGainControl_t;

/**
 *  \brief Data structure to hold Y9009 Orx Gain Control Pin Configuration Info
 */
typedef struct
{
	YxwGpioPinSel_t  orxGainIncPin;		/*!< GPIO used for the Increment  gain input:the gpioNum user can select via gpio function*/
	YxwGpioPinSel_t  orxGainDecPin;		/*!< GPIO used for the Decrement  gain input:the gpioNum user can select via gpio function*/
	YxwGpioPinFunc_t orxGainIncPinFunc;   /*!< GPIO function select 0-7: make GPIO function  gain Increment function */
	YxwGpioPinFunc_t orxGainDecPinFunc;   /*!< GPIO function select 0-7:*make GPIO function  gain Decrement function */
} YxwOrxGainCtrlPinInfo_t;

/**
 *  \brief Data structure to hold Y9009 Orx Gain Control Pin Configuration
 */
typedef struct
{
	uint8_t incStep;			        /* Increment in gain index applied when the increment gain pin is pulsed(1-16) */
	uint8_t decStep;			        /* Decrement in gain index applied when the increment gain pin is pulsed(1-16) */
	YxwOrxGainCtrlPinInfo_t orx1GainCtrlPinInfo;
	YxwOrxGainCtrlPinInfo_t orx2GainCtrlPinInfo;
} YxwOrxGainCtrlPin_t;

/**
 *  \brief Data structure to hold Orx data path settings
 */
typedef struct
{
	YxwORxProfile_t orxProfile;
	YxwGainTab_t	*TableP;
	YxwORxGainControl_t orxGainCtrl;
	YxwOrxGainCtrlPin_t orxGainCtrlPinInfo;
	YxwOrxSlicerCfg_t   orxSlicerPinInfo;
	YxwObsRxChannels_t obsRxChannelsEnable;
} YxwObsRxSettings_t;

/**
 *  \brief Data structure to hold Tx data path settings
 */
typedef struct
{
	YxwTxProfile_t txProfile;
	YxwTxGainControl_t txGainCtrl;
	YxwTxChannels_t txChannels;
} YxwTxSettings_t;

/**
 *  \brief Data structure to hold Rx data path settings
 */
typedef struct
{
	YxwGainMode_t rxgainMode;
	YxwCtrlWay_t   rxmgcMode;
	uint8_t rx1GainIndex;
	uint8_t rx2GainIndex;
	uint8_t rx1MaxGainIndex;
	uint8_t rx1MinGainIndex;
	uint8_t rx2MaxGainIndex;
	uint8_t rx2MinGainIndex;
} YxwRxGainControl_t;

/**
 *  \brief Data structure to hold Rx data path settings
 */
typedef struct
{
	YxwRxProfile_t rxProfile;
	YxwRxGainControl_t rxGainCtrl;
	YxwRxGainCtrlPin_t rxGainCtrlPinInfo;
	YxwRxSlicerPin_t	rxSlicerPinInfo;
	YxwRxChannels_t rxChannels;
} YxwRxSettings_t;

typedef struct
{
	YxwModeSel_t tddMode;
	YxwDpdEn_t dpdMode;

}YxwModeSetting_t;

typedef struct
{
	uint8_t 		Loglevel;
	uint8_t 	    mode;
	YxwInitCalibrations_t initcaltype;
	YxwRxSettings_t    rx;
	YxwTxSettings_t    tx;
	YxwObsRxSettings_t obsRx;
	YxwDigClocks_t     clocks;
	YxwJesdSettings_t  jesd204Settings;
	YxwAgcCfg_t        agcSettings;
} YxwInit_t;

#endif

