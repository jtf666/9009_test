/**
* \file yxw_rx_types.h
* \brief Contains Y9009 API Rx datapath data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef _YXW_RX_TYPES_H_
#define _YXW_RX_TYPES_H_

#include "yxw_data_types.h"
#include "yxw_gpio_types.h"

typedef enum
{
    YXW_RXOFF 	= 0x00,     		/*!< No Rx channels are enabled */
    YXW_RX1	= 0x01,          /*!< Rx1 channel enabled */
    YXW_RX2	= 0x02,          /*!< Rx2 channel enabled */
    YXW_RX1RX2	= 0x03         	/*!< Rx1 + Rx2 channels enabled */
} YxwRxChannels_t;
typedef enum
{
    YXW_ORXOFF      = 0x00,		/*!< No ORx channels are enabled */
    YXW_ORX1        = 0x01,		/*!< ORx1 channel enabled */
    YXW_ORX2        = 0x02,		/*!< ORx2 channel enabled */
    YXW_ORX1ORX2    = 0x03		/*!< ORx1 + Rx2 channels enabled */
} YxwObsRxChannels_t;
/**
 *  \brief Enum to set the Rx Gain control mode
 */
typedef enum
{
    YXW_MGC = 0x00,          /*Manual Gain Control */
    YXW_AGC = 0x01,   	   /*Auto Gain Control */
} YxwGainMode_t;


typedef enum
{
	 YXW_APIMODE = 0x00,		/*!<Gaincontrol ways spi*/
	 YXW_GPIOMODE = 0x01,    /*!<Gaincontrol ways gpio*/
}YxwCtrlWay_t;


typedef enum
{
	YXW_RXSLICER_DIS = 0x00,	/*!<Rxslicer enable*/
	YXW_RXSLICER_EN  = 0x01,	/*!<Rxslicer disable*/
}YxwRxSlicerEn_t;

typedef enum
{
	YXW_ORXSLICER_DIS = 0x00,	/*!<ORxslicer enable*/
	YXW_ORXSLICER_EN  = 0x01,	/*!<ORxslicer disable*/
}YxwOrxSlicerEn_t;



/**
 *  \brief Enum to set the Rx Gain ctrl
 */
typedef struct
{      
    float Atten;
    uint32_t regVelue;
} YxwRXPPAGainAttSel_t;

/**
 * \brief Data Structure to hold Y9009 device gain settings
 */
typedef struct
{
    uint8_t rx1MinGainIndex;         /*!<minimum Rx1 gain index */
    uint8_t rx1MaxGainIndex;         /*!<maximum Rx1 gain index */
    uint8_t rx2MinGainIndex;         /*!<minimum Rx2 gain index */
    uint8_t rx2MaxGainIndex;         /*!<maximum Rx2 gain index */
    uint8_t orx1MinGainIndex;        /*!<minimum Orx1 gain index */
    uint8_t orx1MaxGainIndex;        /*!<maximum Orx1 gain index */
    uint8_t orx2MinGainIndex;        /*!<minimum Orx2 gain index */
    uint8_t orx2MaxGainIndex;        /*!<maximum Orx2 gain index */
    uint8_t tx1MinGainIndex;         /*!<minimum Tx1 gain index */
    uint8_t tx1MaxGainIndex;         /*!<maximum Tx1 gain index */
    uint8_t tx2MinGainIndex;         /*!<minimum Tx2 gain index */
    uint8_t tx2MaxGainIndex;         /*!<maximum Tx2 gain index */
} YxwGainIndex_t;

/**
 *  \brief Data structure to hold Y9009 Rx Gain Control Pin Configuration Info
 */
typedef struct
{
    YxwGpioPinSel_t  rxGainIncPin;		/*!< GPIO used for the Increment  gain input:the gpioNum user can select via gpio function*/
    YxwGpioPinSel_t  rxGainDecPin;		/*!< GPIO used for the Decrement  gain input:the gpioNum user can select via gpio function*/
	YxwGpioPinFunc_t rxGainIncPinFunc;   /*!< GPIO function select 0-7: make GPIO function  gain Increment function */
	YxwGpioPinFunc_t rxGainDecPinFunc;   /*!< GPIO function select 0-7:*make GPIO function  gain Decrement function */
} YxwRxGainCtrlPinInfo_t;


/**
 *  \brief Data structure to hold Y9009 Rx Gain Control Pin Configuration
 */
typedef struct
{
    uint8_t incStep;      				/*!< Increment in gain index applied when the increment gain pin is pulsed(1-16) */          
    uint8_t decStep;               		/*!< Decrement in gain index applied when the increment gain pin is pulsed(1-16) */
    YxwRxGainCtrlPinInfo_t  rx1GainCtrlPinInfo;		/*!< rx1 gain control pin configuration info*/
    YxwRxGainCtrlPinInfo_t  rx2GainCtrlPinInfo;		/*!< rx2 gain control pin configuration info*/
} YxwRxGainCtrlPin_t;

/**
 *  \brief Data structure to hold Y9009 Rx slicer Pin Configuration
 */

typedef struct
{
    YxwGpioPinSel_t  rx1SlicerPin_0;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
    YxwGpioPinSel_t  rx1SlicerPin_1;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
	YxwGpioPinSel_t	rx1SlicerPin_2;     /*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
} YxwRx1SlicerPin_t;

/**
 *  \brief Data structure to hold Y9009 Rx slicer Pin Configuration
 */

typedef struct
{
    YxwGpioPinSel_t  rx2SlicerPin_0;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
    YxwGpioPinSel_t  rx2SlicerPin_1;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
	YxwGpioPinSel_t	rx2SlicerPin_2;     /*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
} YxwRx2SlicerPin_t;

/**
 *  \brief Data structure to hold Y9009 Rx Slicer out Pin Configuration
 */
typedef struct
{
	YxwRxSlicerEn_t		  EnableSlicer;			/*!<Digital Gain compensation with slicer gpio output*/
    YxwRx1SlicerPin_t  rx1SlicerPinInfo;		/*!< rx1 slicer control pin configuration info*/
    YxwRx2SlicerPin_t  rx2SlicerPinInfo;		/*!< rx2 slicer control pin configuration info*/
	YxwGpioPinFunc_t   rxSlicerPinFunc;		/*!<select gpio fun for slicer*/
} YxwRxSlicerPin_t;

/**
 *  \brief Data structure to hold Y9009 ORx Slicer out Pin Configuration
 */
typedef struct
{
    YxwGpioPinSel_t  orxSlicerPin_0;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
    YxwGpioPinSel_t  orxSlicerPin_1;		/*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
	YxwGpioPinSel_t	orxSlicerPin_2;     /*!< GPIO used for the slicer output:the gpioNum user can select via gpio function*/
} YxwOrxSlicerPin_t;

typedef struct
{
	YxwOrxSlicerEn_t		  EnableSlicer;			/*!<Digital Gain compensation with slicer gpio output*/
    YxwOrxSlicerPin_t  orxSlicerPinInfo;		/*!< orx slicer control pin configuration info*/
	YxwGpioPinFunc_t   orxSlicerPinFunc;		/*!<select gpio fun for slicer*/
} YxwOrxSlicerCfg_t;


typedef struct
{
	uint8_t  sizeofTb;
	uint32_t anaGainTB[128];
	uint32_t digGainTB[128];
}YxwGainTab_t;

/**
 *  \brief Data structure to hold tia channel
 */
typedef enum
{
    YXW_TIA_CHANNEL1 = 0,
    YXW_TIA_CHANNEL2 = 1
}YxwTiaChannel_t;

/**
 *  \brief Data structure to hold tia gain
 */
typedef enum
{
    YXW_TIA_GAIN_MINUS_6DB = 0, /*!<Tia gain is -6db*/
    YXW_TIA_GAIN_PLUS_0DB  = 1, /*!<Tia gain is 0db*/
    YXW_TIA_GAIN_PLUS_6DB  = 2  /*!<Tia gain is -6db*/
}YxwTiaGain_t;

#endif

