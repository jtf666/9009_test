/**
* \file yxw_radio_types.h
* \brief Contains Y9009 API Radio Control data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_RADIO_TYPES_H__
#define __YXW_RADIO_TYPES_H__

typedef enum
{
	YXW_PIN_MODE = 0x1,   /*!<radio ctrl by pin mode*/
	YXW_SPI_MODE = 0x2,	 /*!<radio ctrl by spi mode*/
	YXW_ERR_MODE = 0x3	 /*!<radio ctrl err mode, not spi or pin mode*/
} YxwRadioCtlMode_t;

typedef enum
{
	YXW_OFF_Mask 			= 0x0,  /*!<Get radio Off state mask*/
	YXW_BIT0_Mask 		= 0x1,	/*!<Get radio On state mask*/
	YXW_BIT1_Mask 		= 0x2,	/*!<Get radio On state mask*/
	YXW_BIT0BIT1_Mask 	= 0x3	/*!<Get radio On state mask*/
}YxwBitMask_t;

/**
 *  \brief Enum of PLL selections
 */
typedef enum
{
	YXW_CLK_PLL = 0x0,                /*!< Selects CLK PLL for Rx and Tx */
	YXW_RF_PLL  = 0x1,                /*!< Selects RF PLL for Rx and Tx */
	YXW_AUX_PLL = 0x2                 /*!< Selects AUX PLL for Rx and tx*/
} YxwRfPllName_t;

/**
 *  \brief Enum of Channel seq
 */
typedef enum
{
	YXW_CHANNEL_SEQ_NONE      = 0x0, /*! Channel Seq no chage*/
	YXW_CHANNEL_SEQ_ORX_TO_RX = 0x1, /*! ORx frontend to RX backend*/
	YXW_CHANNEL_SEQ_RX_TO_ORX = 0x2  /*! Rx frontend to ORX backend*/
}YxwChannelSeq_t;

/**
 *  \brief Enum of RADIO STATUS selections
 */
typedef enum
{
	RFPLLMASK =  0x80,              /*!< Get RF pll Status MASK */
	BBPLLMASK  = 0x100,             /*!< Get BB pll Status MASK */
	AUXPLLMASK = 0x200,             /*!< Get AUX pll Status MASK */
	TX1ENMASK =  0x400,				/*!< Get TX0 EN  Status MASK */
	TX2ENMASK =  0x800,				/*!< Get TX1 EN  Status MASK */
	RX1ENMASK =  0x1000,			/*!< Get RX0 EN  Status MASK */
	RX2ENMASK =  0x2000,			/*!< Get TX1 EN  Status MASK */
	ORX1ENMASK = 0x4000,			/*!< Get ORX0 EN  Status MASK */
	ORX2ENMASK = 0x8000				/*!< Get ORX1 EN Status MASK */
} YxwChnStatus_t;

typedef enum
{
	TX1_INTR_OPEN  = 0x80,              /*!< Get Tx1 Enabled Interupt Status MASK */
	TX2_INTR_OPEN  = 0x100,             /*!< Get Tx2 Enabled Interupt Status MASK */
	RX1_INTR_OPEN  = 0x200,             /*!< Get Rx1 Enabled Interupt Status MASK */
	RX2_INTR_OPEN  = 0x400,				/*!< Get Rx2 Enabled Interupt Status MASK */
	TX1_INTR_CLOSE = 0x800,				/*!< Get Tx1 Disabled Interupt Status MASK */
	TX2_INTR_CLOSE = 0x1000,			/*!< Get Tx2 Disabled Interupt Status MASK */
	RX1_INTR_CLOSE = 0x2000,			/*!< Get Rx1 Disabled Interupt Status MASK */
	RX2_INTR_CLOSE = 0x4000,			/*!< Get Rx2 Disabled Interupt Status MASK */
	ORX_INTR_OPEN_CLOSE = 0x8000		/*!< Get ORX Enabled And Disabled Interupt Status MASK */
} YxwInteruptStatus_t;

typedef enum
{
	TDD_MODE = 0x0,   /*!< user tdd mode*/
	FDD_MODE = 0x1	  /*!< user fdd mode*/
} YxwModeSel_t;

typedef enum
{
	DPD_DIS = 0x0,   /*!< not support dpd*/
	DPD_EN = 0x1,   /*!< support dpd*/
} YxwDpdEn_t;

#define YXW_RADIO_PLL_WIAT_TIMEOUT (60)   // timeout = 60s

#endif //__YXW_RADIO_TYPES_H__

