/**
* \file yxw_error.h
* \brief Contains Y9009 API error information functions
 *        These functions are public to the customer for getting more details on
 *        errors and debugging.
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_plat_hal.h"
#include "yxw_error.h"

#include "yxw_plat_hal.h"
#include "yxw_plat_hal.h"
#include "yxw_plat_hal.h"


static const char* yxw_getErrorMessage(uint32_t errSrc, uint32_t errCode)
{
	switch(errSrc){
		case YXW_ERRHDL_HAL_INIT:
			if(errCode==YXWHAL_OK){
				return "INIT->SPI_OK";
			}else{
				return "INIT->SPI_FAIL";
			}
		case YXW_ERRHDL_HAL_AGC:
			if(errCode==YXWHAL_OK){
				return "AGC->SPI_OK";
			}else{
				return "AGC->SPI_FAIL";
			}
		case YXW_ERRHDL_HAL_JES204B:
			if(errCode==YXWHAL_OK){
				return "JESD->SPI_OK";
			}else{
				return "JESD->SPI_FAIL";
			}
		case YXW_ERRHDL_HAL_OTP:
			if(errCode==YXWHAL_OK){
				return "OTP->SPI_OK";
			}else{
				return "OTP->SPI_FAIL";
			}
		case YXW_ERRHDL_HAL_RADIO:
			if(errCode==YXWHAL_OK){
				return "RADIO->SPI_OK";
			}else{
				return "RADIO->SPI_FAIL";
			}
		case YXW_ERRHDL_HAL_RISCV:
			if(errCode==YXWHAL_OK){
				return "RISCV->SPI_OK";
			}else{
				return "RISCV->SPI_FAIL";
			}
		case YXW_ERRHDL_HAL_RX:
			if(errCode==YXWHAL_OK){
				return "RX->SPI_OK";
			}else{
				return "RX->SPI_FAIL";
			}
		case YXW_ERRHDL_HAL_TX:
			if(errCode==YXWHAL_OK){
				return "TX->SPI_OK";
			}else{
				return "TX->SPI_FAIL";
			}
		case YXW_ERRHDL_HAL_GPIO:
			if(errCode==YXWHAL_OK){
				return "GPIO->SPI_OK";
			}else{
				return "GPIO->SPI_FAIL";
			}
			default:
				break;
	}
	return NULL;
}

YxwRecoveryActions_t YXW_ApiErrHandler(YxwDevice_t *hw_priv, YxwErrHdls_t errHdl,uint32_t detErr, YxwRecoveryActions_t retVal,YxwRecoveryActions_t recAction)
{
	if(detErr!=YXW_ERR_OK){
		switch (errHdl)
		{
			case YXW_ERRHDL_HAL_INIT:
				/*Ignore Error from log as lower priority than error being handled*/
				YXW_WriteToLog(hw_priv, YXWHAL_LOG_ERR, detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_INIT, detErr));
				hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_INIT;
				return recAction;
			case YXW_ERRHDL_HAL_AGC:
				YXW_WriteToLog(hw_priv,YXWHAL_LOG_ERR,detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_AGC,detErr));
				hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_AGC;
				break;
			case YXW_ERRHDL_HAL_JES204B:
				 YXW_WriteToLog(hw_priv, YXWHAL_LOG_ERR, detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_JES204B, detErr));
				 hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_JES204B;
				 break;
			case YXW_ERRHDL_HAL_OTP:
				YXW_WriteToLog(hw_priv, YXWHAL_LOG_ERR, detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_OTP, detErr));
				hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_OTP;
				break;
			case YXW_ERRHDL_HAL_RADIO:
				YXW_WriteToLog(hw_priv, YXWHAL_LOG_ERR, detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_RADIO, detErr));
				hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_OTP;
				break;
			case YXW_ERRHDL_HAL_RISCV:
				YXW_WriteToLog(hw_priv, YXWHAL_LOG_ERR, detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_RISCV, detErr));
				hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_OTP;
				break;
			case YXW_ERRHDL_HAL_RX:
				YXW_WriteToLog(hw_priv, YXWHAL_LOG_ERR, detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_RX, detErr));
				hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_OTP;
				break;
			case YXW_ERRHDL_HAL_TX:
				YXW_WriteToLog(hw_priv, YXWHAL_LOG_ERR, detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_TX, detErr));
				hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_OTP;
				break;
			case YXW_ERRHDL_HAL_GPIO:
				YXW_WriteToLog(hw_priv, YXWHAL_LOG_ERR, detErr,yxw_getErrorMessage(YXW_ERRHDL_HAL_GPIO, detErr));
				hw_priv->YxwState.errSource = YXW_ERRHDL_HAL_OTP;
				break;
			 default:
			   break;
		}
	}
	hw_priv->YxwState.errCode = detErr;
	return retVal;
}
