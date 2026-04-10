/**
* \file Yxw_error_types.h
* \brief Contains Y9009 data types for API Error messaging
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
/* Copyright 2020 Zealy Copyright.*/


#ifndef YXW_ERROR_TYPES_H_
#define YXW_ERROR_TYPES_H_

/**
 *  \brief Enum of possible sources of error codes.
 */
typedef enum
{
	YXW_ERR_OK						= 0x0,
	YXW_ERR_INV_NULL_INIT_PARAM		= 0x1,
	YXW_ERR_DEVICE_INIT_NULL_PARAM	= 0x2,
	YXW_ERR_VERTXPFILE_TXINPUTRATE	= 0x3,
	YXW_ERR_VERTXPFILE_RFBAND		= 0x4,
	YXW_ERR_VERTXPFILE_THB1			= 0x5,
	YXW_ERR_VERTXPFILE_THB2			= 0x6,
	YXW_ERR_VERTXPFILE_THB3			= 0x7,
	YXW_ERR_VERTXPFILE_TXFIR			= 0x8,
	YXW_ERR_VERTXPFILE_TXFIR1		= 0x9,
	YXW_ERR_VERTRXPFILE_RXINPUTRATE  = 0xA,
	YXW_ERR_VERXPFILE_RFBAND			= 0xB,
	YXW_ERR_VERRXPFILE_THB1			= 0xC,
	YXW_ERR_VERRXPFILE_RXFIR1		= 0xD,
	YXW_ERR_VERTORXPFILE_RXINPUTRATE	= 0xE,
	YXW_ERR_VEORXPFILE_RFBAND		= 0xF,
	YXW_ERR_VERORXPFILE_FIR			= 0x10,
	YXW_ERR_VERORXPFILE_FIR1			= 0x11,
	YXW_ERR_CLOCK_RFPLL_PHASE		= 0x12,
	YXW_ERR_TXRXORX_CHANNLE			= 0x13,
	YXW_ERR_TXRXORX_PRFILE_VALID		= 0x14,
	YXW_ERR_TX1_MAX_GAIN_VALID		= 0x15,
	YXW_ERR_TX2_MAX_GAIN_VALID		= 0x16,
	YXW_ERR_RX_GAINPIN_CTRL_VALID	= 0x17,
	YXW_ERR_RX1_MAX_GAIN_VALID		= 0x18,
	YXW_ERR_RX2_MAX_GAIN_VALID		= 0x19,
	YXW_ERR_ORX1_MAX_GAIN_VALID		= 0x1A,
	YXW_ERR_ORX2_MAX_GAIN_VALID		= 0x1B,
	YXW_ERR_GPIO_NUM_VALID		    = 0x1C,
	YXW_ERR_GPIO_MONITOR_PARAM_VALID = 0x1D
}YxwErr_t;

/**
 *  \brief Private Enum to list of available error handlers
 */
typedef enum
{
	YXW_ERRHDL_HAL_INIT	  = 0x0,
	YXW_ERRHDL_HAL_AGC	  = 0x1,
	YXW_ERRHDL_HAL_JES204B = 0x2,
	YXW_ERRHDL_HAL_OTP	  = 0x3,
	YXW_ERRHDL_HAL_RADIO   = 0x4,
	YXW_ERRHDL_HAL_RISCV   = 0x5,
	YXW_ERRHDL_HAL_RX 	  = 0x6,
	YXW_ERRHDL_HAL_TX	  = 0x7,
	YXW_ERRHDL_HAL_GPIO    = 0x8,
	YXW_ERRHDL_INITCALS    = 0x9,
	YXW_ERRHDL_HAL_INFO    = 0xa,
	YXW_ERRHDL_HAL_HOPPING = 0xb,
} YxwErrHdls_t;

/**
 *  \brief Enumerated list of Y9009 Recovery Actions used as return values
 *         APIs.
 */
typedef enum
{
	YXW_NO_ACTION 		= 0x0,
	YXW_ERR_INIT_PLAT	= 0x1,
	YXW_ERR_RESET_RISCV	= 0x2,
	YXW_ERR_INIT_CALS	= 0x3,
	YXW_ERR_RESET_SPI	= 0x4,
	YXW_ERR_RESET_GPIO	= 0x5,
	YXW_ERR_CHECK_PARAM	= 0x6,
	YXW_ERR_TIMEOUT	    = 0x7,
	YXW_ERR_NULL_POINTER = 0x8,
}YxwRecoveryActions_t;

#endif /* Y9009_ERROR_TYPES_H_ */
