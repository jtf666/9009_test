/**
* \file yxw_otp.c
* \brief Knowledge of the otp on yxw transfer device
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_error.h"
#include "yxw_otp.h"

#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_plat_hal.h"


/*Read OTP Mode.The Highest Clk is 20MHZ,High and Low level still 20ns at less */
uint32_t YXW_OtpRead(YxwDevice_t *hw_priv,uint32_t OtpAddr,uint32_t *Data)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t OptCmd=0;
	uint32_t status=0;
	uint32_t uiTry = WIAT_TIMEOUT;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	/*construct otp_cmd   [17:8]->otpaddr */
	OptCmd|=((OtpAddr&0x3FF)<<8);
	/*construct otp_cmd   [6]->PAS_EN */
	//OptCmd|=(PAS_EN<<6);
	/*construct otp_cmd   [5]->TEST_ROW_VALID */
	//OptCmd|=(TEST_ROW_VALID<<5);
	/*construct otp_cmd   [4]->CELL_VALID */
	//OptCmd|=(CELL_VALID<<4);
	/*construct otp_cmd   [3]->CMD_VALID */
	OptCmd|=(CMD_VALID<<3);
	/*construct otp_cmd   [2:0]->OTP_CMD_VLD */
	OptCmd|=(YXW_READ_MODE<<0);

	/*First Write otp cmd*/
	while(uiTry > 0){
		YXW_mdelay(1);
		halStatus = YXW_spiReadReg(hw_priv,YXW_REG_OTPOUT_STA_ADDR,&status);
		yxwRetAct = YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_OTP,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		if((status&0x40)!=0x40){
			break;
		}
		YXW_DEBUG(DBG_LEVEL_RESULT,"OtpRead status=%x\n",status);
		uiTry--;
	}

	if(uiTry <= 0){
		YXW_DEBUG(DBG_LEVEL_ERR,"OtpRead Timeout!\n\n");
		yxwRetAct=YXW_ERR_TIMEOUT;
		return (uint32_t)yxwRetAct;
	}

	halStatus = YXW_spiWriteReg(hw_priv,YXW_REG_OTPIN_CMD_ADDR,OptCmd);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_OTP,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*second Read otp data*/
	halStatus = YXW_spiReadReg(hw_priv,YXW_REG_OTPOUT_DATA_ADDR,Data);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_OTP,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return 0;

}
