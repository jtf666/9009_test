/**
* \file yxw_otp_types.h
* \brief Contains Y9009 API Otp data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_OTP_TYPES_H__
#define __YXW_OTP_TYPES_H__

#define YXW_REG_OTPIN_CMD_ADDR    	0x208
#define YXW_REG_OTPIN_DATA_ADDR		0x209

#define YXW_REG_OTPOUT_DATA_ADDR		0x44c
#define YXW_REG_OTPOUT_STA_ADDR		0x44d

/*REG_OTPIN_CMD_ADDR ->bit[6]---otp_pas_en */
typedef enum 
{
	YXW_PAS_DIS  = 0x0,
	YXW_PAS_EN   = 0x1
}YxwOtpPasEn_t;

/*REG_OTPIN_CMD_ADDR ->bit[5]---otp_ptr_vld */
typedef enum
{
	YXW_TEST_ROW_INVALID	= 0x0,
	YXW_TEST_ROW_VALID	= 0x1
}YxwOtpPtrVld_t;

/*REG_OTPIN_CMD_ADDR ->bit[4]---otp_pas_vld */
typedef enum
{
	YXW_CELL_INVALID		= 0x0,
	YXW_CELL_VALID		= 0x1
}YxwOtpPasVld_t;

/*REG_OTPIN_CMD_ADDR ->bit[3]---otp_cmd_vld */
typedef enum
{
	CMD_INVALID	 = 0x0,
	CMD_VALID	 = 0x1
}YxwOtpCmdVld_t;

/*REG_OTPIN_CMD_ADDR ->bit[2:0]---otp_cmd */
typedef enum
{
	YXW_DEEP_STANDBY_MODE	= 0x0,
	YXW_STANDBY_MODE			= 0x1,
	YXW_READ_MODE			= 0x2,
	YXW_PROG_MODE			= 0x3,
	YXW_INIT_MARGIN_READ_MODE = 0x4,
	YXW_PGM_MARGIN_READ_MODE	 = 0x5,
}YxwOtpCmd_t;

#endif

