/**
* \file yxw_rx.c
* \brief Contains functions to support Y9009 Rx and Observation Rx data path
*        control
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_error.h"
#include "yxw_rx.h"

#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_gpio.h"
#include "yxw_plat_hal.h"
#include "yxw_radio.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"


static uint32_t yxw_setRxGainCtrlPin (YxwDevice_t *hw_priv, YxwRxGainCtrlPinInfo_t *rxGainCtrlPinInfo)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	/*gpioOutEn -->1 output*/
	/*gpioOutEn -->0 input*/
	uint32_t gpioOutEn=0;
	/*gpioCtrl -->1 Ctrl by spi*/
	/*gpioCtrl -->0 Ctrl by riscv*/
	uint32_t gpioCtrl=1;

	FUNCTION_ENTER(hw_priv);
	if((rxGainCtrlPinInfo->rxGainIncPin >= YXW_GPIO_00)&&(rxGainCtrlPinInfo->rxGainIncPin <= YXW_GPIO_09))
	{
		yxwRetAct=YXW_setGpioSourceCtrl(hw_priv,gpioCtrl,rxGainCtrlPinInfo->rxGainIncPin);
		IF_ERR_RETURN(yxwRetAct);
	}

	yxwRetAct=YXW_setGpioDirection(hw_priv,gpioOutEn,rxGainCtrlPinInfo->rxGainIncPin);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioFuncSel(hw_priv,rxGainCtrlPinInfo->rxGainIncPinFunc,rxGainCtrlPinInfo->rxGainIncPin);
	IF_ERR_RETURN(yxwRetAct);

	if((rxGainCtrlPinInfo->rxGainIncPin >= YXW_GPIO_00)&&(rxGainCtrlPinInfo->rxGainIncPin <= YXW_GPIO_09))
	{
		yxwRetAct=YXW_setGpioSourceCtrl(hw_priv,gpioCtrl,rxGainCtrlPinInfo->rxGainDecPin);
		IF_ERR_RETURN(yxwRetAct);
	}

	yxwRetAct=YXW_setGpioDirection(hw_priv,gpioOutEn,rxGainCtrlPinInfo->rxGainDecPin);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioFuncSel(hw_priv,rxGainCtrlPinInfo->rxGainDecPinFunc,rxGainCtrlPinInfo->rxGainDecPin);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t  YXW_setRxGainCtrlPin(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel, YxwRxGainCtrlPin_t *rxGainCtrlPin)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;
	uint32_t MAX_STEP =0x7;

	FUNCTION_ENTER(hw_priv);
	switch(rxChannel){
		case YXW_RX1:
			yxwRetAct = yxw_setRxGainCtrlPin(hw_priv, &rxGainCtrlPin->rx1GainCtrlPinInfo);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX2:
			yxwRetAct = yxw_setRxGainCtrlPin(hw_priv, &rxGainCtrlPin->rx2GainCtrlPinInfo);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX1RX2:
			yxwRetAct = yxw_setRxGainCtrlPin(hw_priv, &rxGainCtrlPin->rx1GainCtrlPinInfo);
			IF_ERR_RETURN(yxwRetAct);
			yxwRetAct = yxw_setRxGainCtrlPin(hw_priv, &rxGainCtrlPin->rx2GainCtrlPinInfo);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}
	/*Error check for gain index*/
	if(rxGainCtrlPin->decStep >MAX_STEP)
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
				YXW_ERR_RX_GAINPIN_CTRL_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}
	if(rxGainCtrlPin->incStep>MAX_STEP){

		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
				YXW_ERR_RX_GAINPIN_CTRL_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}

	halstatus = YXW_spiWriteField(hw_priv,REG_GC_02,(rxGainCtrlPin->decStep-1),11,8);
	halstatus = YXW_spiWriteField(hw_priv,REG_GC_02,(rxGainCtrlPin->incStep-1),15,12);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_setRxManualGain(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel, uint8_t gainIndex)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	if (rxChannel == YXW_RX1)
	{
		if ((gainIndex < hw_priv->YxwState.gainIndexes.rx1MinGainIndex) ||
			(gainIndex > hw_priv->YxwState.gainIndexes.rx1MaxGainIndex))
		{
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
					YXW_ERR_RX1_MAX_GAIN_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
		}
	}
	else if (rxChannel == YXW_RX2)
	{
		if ((gainIndex < hw_priv->YxwState.gainIndexes.rx2MinGainIndex) ||
			(gainIndex > hw_priv->YxwState.gainIndexes.rx2MaxGainIndex))
		{
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
					YXW_ERR_RX2_MAX_GAIN_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
		}
	}
	else
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
				YXW_ERR_TXRXORX_CHANNLE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if (rxChannel == YXW_RX1)
	{
		halError = YXW_spiWriteField(hw_priv, REG_GC_02,1,24,24);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_02,gainIndex,6,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	else
	{
		halError = YXW_spiWriteField(hw_priv, REG_GC_02,1,25,25);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_02,gainIndex,23,17);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getRxGain(YxwDevice_t *hw_priv,YxwRxChannels_t rxChannel, uint32_t *rxGainIndex)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;
	uint32_t radioStatus = 0;

	halError=YXW_spiReadReg(hw_priv, REGOUT_CALI_DONE_0,&radioStatus);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	switch(rxChannel){
		case YXW_RX1:
			if(RX1ENMASK == (radioStatus&RX1ENMASK))
			{
				halError = YXW_spiReadField(hw_priv, REGOUT_GC_21,rxGainIndex,6,0);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				halError = YXW_spiReadField(hw_priv, REG_GC_02,rxGainIndex,6,0);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			break;
		case YXW_RX2:
			if(RX2ENMASK == (radioStatus&RX2ENMASK))
			{
				halError = YXW_spiReadField(hw_priv, REGOUT_GC_21,rxGainIndex,14,8);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				halError = YXW_spiReadField(hw_priv, REG_GC_02,rxGainIndex,23,17);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			break;
		default:
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
					YXW_ERR_TXRXORX_CHANNLE,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_setRxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t mode,YxwCtrlWay_t ctrlWay)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halError = YXW_spiWriteField(hw_priv,REG_GC_01,mode,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*0: API mode; index is from register
	 1: Pin ctrl mode; index is controlled by GPIO*/
	halError = YXW_spiWriteField(hw_priv,REG_GC_02,ctrlWay,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getRxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t *mode,YxwCtrlWay_t *ctrlWay)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	halError = YXW_spiReadField(hw_priv,REG_GC_01,mode,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*0: API mode; index is from register
	 1: Pin ctrl mode; index is controlled by GPIO*/
	halError = YXW_spiReadField(hw_priv,REG_GC_02,ctrlWay,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}


uint32_t  YXW_setObsRxGainCtrlPin(YxwDevice_t *hw_priv, YxwObsRxChannels_t orxChannel, YxwRxGainCtrlPin_t *rxGainCtrlPin)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;
	uint32_t MAX_STEP =0x7;

	FUNCTION_ENTER(hw_priv);
	switch(orxChannel){
		case YXW_ORX1:
			yxwRetAct = yxw_setRxGainCtrlPin(hw_priv, &rxGainCtrlPin->rx1GainCtrlPinInfo);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX2:
			yxwRetAct = yxw_setRxGainCtrlPin(hw_priv, &rxGainCtrlPin->rx2GainCtrlPinInfo);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX1ORX2:
			yxwRetAct = yxw_setRxGainCtrlPin(hw_priv, &rxGainCtrlPin->rx1GainCtrlPinInfo);
			IF_ERR_RETURN(yxwRetAct);
			yxwRetAct = yxw_setRxGainCtrlPin(hw_priv, &rxGainCtrlPin->rx2GainCtrlPinInfo);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}

	/*Error check for gain index*/
	if(rxGainCtrlPin->decStep >MAX_STEP)
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
				YXW_ERR_RX_GAINPIN_CTRL_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}
	if(rxGainCtrlPin->incStep>MAX_STEP){

		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
				YXW_ERR_RX_GAINPIN_CTRL_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}

	halstatus = YXW_spiWriteField(hw_priv,REG_GC_04,(rxGainCtrlPin->decStep-1),11,8);
	halstatus = YXW_spiWriteField(hw_priv,REG_GC_04,(rxGainCtrlPin->incStep-1),15,12);

	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_setObsRxManualGain(YxwDevice_t *hw_priv,YxwObsRxChannels_t obsRxCh, uint8_t gainIndex)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	switch(obsRxCh)
	{
		case YXW_ORX1:
			if ((gainIndex < hw_priv->YxwState.gainIndexes.orx1MinGainIndex) ||
				(gainIndex > hw_priv->YxwState.gainIndexes.orx1MaxGainIndex))
			{
				return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
						YXW_ERR_ORX1_MAX_GAIN_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
			}
			break;
		case YXW_ORX2:
			if ((gainIndex < hw_priv->YxwState.gainIndexes.orx2MinGainIndex) ||
				(gainIndex > hw_priv->YxwState.gainIndexes.orx2MaxGainIndex))
			{
				return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
						YXW_ERR_ORX2_MAX_GAIN_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
			}
			break;
		default:
			break;
	}

	if(obsRxCh == YXW_ORX1)
	{
		halError = YXW_spiWriteField(hw_priv, REG_GC_04,1,24,24);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_04,gainIndex,6,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	if(obsRxCh == YXW_ORX2)
	{
		halError = YXW_spiWriteField(hw_priv, REG_GC_04,1,25,25);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_04,gainIndex,23,17);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getObsRxGain(YxwDevice_t *hw_priv,YxwObsRxChannels_t obsRxChannel, uint32_t *obsRxGainIndex)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;
	uint32_t radioStatus = 0;

	halError=YXW_spiReadReg(hw_priv, REGOUT_CALI_DONE_0,&radioStatus);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	switch(obsRxChannel){
		case YXW_ORX1:
			if(ORX1ENMASK == (radioStatus&ORX1ENMASK))
			{
				halError = YXW_spiReadField(hw_priv,REGOUT_GC_21,obsRxGainIndex,22,16);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				halError = YXW_spiReadField(hw_priv,REG_GC_04,obsRxGainIndex,6,0);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			break;
		case YXW_ORX2:
			if(ORX2ENMASK == (radioStatus&ORX2ENMASK))
			{
				halError = YXW_spiReadField(hw_priv,REGOUT_GC_21,obsRxGainIndex,30,24);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				halError = YXW_spiReadField(hw_priv,REG_GC_04,obsRxGainIndex,23,17);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			break;
		default:
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
					YXW_ERR_TXRXORX_CHANNLE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_setObsRxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t mode,YxwCtrlWay_t ctrlWay)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halError = YXW_spiWriteField(hw_priv,REG_GC_03,mode,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*0: API mode; index is from register
	 1: Pin ctrl mode; index is controlled by GPIO*/
	halError = YXW_spiWriteField(hw_priv,REG_GC_04,ctrlWay,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getObsRxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t *mode,YxwCtrlWay_t *ctrlWay)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	halError = YXW_spiReadField(hw_priv,REG_GC_03,mode,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*0: API mode; index is from register
	 1: Pin ctrl mode; index is controlled by GPIO*/
	halError = YXW_spiReadField(hw_priv,REG_GC_04,ctrlWay,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}


/*!< GPIO Select :{monitor_valid_data[7:0], 8d0}*/
static uint32_t YXW_setGpioFunForSlicerPosSel(YxwDevice_t *hw_priv, YxwGpioPinSel_t GpioNum,YxwGpioPinFunc_t GpioFunc)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, YXW_SPI_CTRL,GpioNum);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioDirection(hw_priv, YXW_GPIO_DIR_OUT,GpioNum);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioFuncSel(hw_priv, GpioFunc,GpioNum);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_setRxSlicerEnable(YxwDevice_t *hw_priv,YxwRxChannels_t rxChannel,YxwRxSlicerPin_t *rxSlierPin)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	YxwData DataInfo;
	YxwCmdTx *pMsg;

	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;

	FUNCTION_ENTER(hw_priv);
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;

	switch(rxChannel){

		case YXW_RX1:
			yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,rxSlierPin->rx1SlicerPinInfo.rx1SlicerPin_0,rxSlierPin->rxSlicerPinFunc);
			IF_ERR_RETURN(yxwRetAct);

			yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,rxSlierPin->rx1SlicerPinInfo.rx1SlicerPin_1,rxSlierPin->rxSlicerPinFunc);
			IF_ERR_RETURN(yxwRetAct);

			yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,rxSlierPin->rx1SlicerPinInfo.rx1SlicerPin_2,rxSlierPin->rxSlicerPinFunc);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX2:
			yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,rxSlierPin->rx2SlicerPinInfo.rx2SlicerPin_0,rxSlierPin->rxSlicerPinFunc);
			IF_ERR_RETURN(yxwRetAct);

			yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,rxSlierPin->rx2SlicerPinInfo.rx2SlicerPin_1,rxSlierPin->rxSlicerPinFunc);
			IF_ERR_RETURN(yxwRetAct);

			yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,rxSlierPin->rx2SlicerPinInfo.rx2SlicerPin_2,rxSlierPin->rxSlicerPinFunc);
			IF_ERR_RETURN(yxwRetAct);
			break;
			default:
				break;
	}

	if(rxSlierPin->EnableSlicer == YXW_RXSLICER_EN){
		/*Send cmd to riscv change gaintable to normal digital gain mode*/
		DataInfo.u.Tx.TxData.value=(HI_ENABLE_SLICER|(YXW_RXSLICER_EN<<16));
	}else{
		/*Send cmd to riscv change gaintable to normal slicer gain mode*/
		DataInfo.u.Tx.TxData.value=(HI_ENABLE_SLICER|(YXW_RXSLICER_DIS<<16));
	}
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;

}

uint32_t YXW_setOrxSlicerEnable(YxwDevice_t *hw_priv,YxwOrxSlicerCfg_t *orxSlierPin)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwCmdTx *pMsg;

	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;

	FUNCTION_ENTER(hw_priv);
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;

	yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,orxSlierPin->orxSlicerPinInfo.orxSlicerPin_0,orxSlierPin->orxSlicerPinFunc);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,orxSlierPin->orxSlicerPinInfo.orxSlicerPin_1,orxSlierPin->orxSlicerPinFunc);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioFunForSlicerPosSel(hw_priv,orxSlierPin->orxSlicerPinInfo.orxSlicerPin_2,orxSlierPin->orxSlicerPinFunc);
	IF_ERR_RETURN(yxwRetAct);

	if(orxSlierPin->EnableSlicer == YXW_ORXSLICER_EN){
		/*Send cmd to riscv change gaintable to normal digital gain mode*/
		DataInfo.u.Tx.TxData.value=(HI_ENABLE_ORX_SLICER|(YXW_ORXSLICER_EN<<16));
	}else{
		/*Send cmd to riscv change gaintable to normal slicer gain mode*/
		DataInfo.u.Tx.TxData.value=(HI_ENABLE_ORX_SLICER|(YXW_ORXSLICER_DIS<<16));
	}
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_setORxPinFunction(YxwDevice_t *hw_priv,YxwObsRxChannels_t obsRxChannel)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	switch(obsRxChannel){
		case YXW_ORX1:
			yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, YXW_SPI_CTRL, YXW_GPIO_05);
			IF_ERR_RETURN(yxwRetAct);

			yxwRetAct=YXW_setGpioDirection(hw_priv,YXW_GPIO_DIR_IN, YXW_GPIO_05);
			IF_ERR_RETURN(yxwRetAct);

			yxwRetAct=YXW_setGpioFuncSel(hw_priv,YXW_SEL_4, YXW_GPIO_05);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX2:
			yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, YXW_SPI_CTRL, YXW_GPIO_06);
			IF_ERR_RETURN(yxwRetAct);

			yxwRetAct=YXW_setGpioDirection(hw_priv,YXW_GPIO_DIR_IN, YXW_GPIO_06);
			IF_ERR_RETURN(yxwRetAct);

			yxwRetAct=YXW_setGpioFuncSel(hw_priv,YXW_SEL_4, YXW_GPIO_06);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,
					YXW_ERR_TXRXORX_CHANNLE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}
	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getRxDecPowerFloat(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel, float *rxDecPower_dBFS)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;

	uint32_t rxDecPower = 0;

	YxwData DataInfo;
	YxwCmdTx *pMsg;

	uint32_t radioStatus = 0;

	halStatus=YXW_spiReadReg(hw_priv, REGOUT_CALI_DONE_0,&radioStatus);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	switch(rxChannel){
		case YXW_RX1:
			if(RX1ENMASK != (radioStatus&RX1ENMASK))
			{
				*rxDecPower_dBFS=(sint16_t)rxDecPower;
				return (uint32_t)yxwRetAct;
			}
			break;
		case YXW_RX2:
			if(RX2ENMASK != (radioStatus&RX2ENMASK))
			{
				*rxDecPower_dBFS=(sint16_t)rxDecPower;
				return (uint32_t)yxwRetAct;
			}
			break;
		default:
			{
				*rxDecPower_dBFS=(sint16_t)rxDecPower;
				YXW_DEBUG(DBG_LEVEL_RESULT,"rxChannel(%d) Parameter is invalid, only 1:YXW_RX1 or 2:YXW_RX2.\n",rxChannel);
				return (uint32_t)yxwRetAct;
			}

	}


	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;

	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;

	DataInfo.u.Tx.TxData.value=(HI_GET_DACPOWER|(rxChannel<<16));
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiReadReg(hw_priv,SEND_CMD_DATA,&rxDecPower);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	*rxDecPower_dBFS=(float)((sint32_t)rxDecPower/POWER_ACCURACY);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getORxDecPowerFloat(YxwDevice_t *hw_priv, YxwObsRxChannels_t orxChannel, float *orxDecPower_dBFS)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;

	uint32_t orxDecPower = 0;

	YxwData DataInfo;
	YxwCmdTx *pMsg;

	uint32_t radioStatus = 0;

	YXW_Memset(&DataInfo,0,sizeof(YxwData));

	halStatus=YXW_spiReadReg(hw_priv, REGOUT_CALI_DONE_0,&radioStatus);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	switch(orxChannel){
		case YXW_ORX1:
			if(ORX1ENMASK != (radioStatus&ORX1ENMASK))
			{
				*orxDecPower_dBFS=(sint32_t)orxDecPower;
				return (uint32_t)yxwRetAct;
			}
			break;
		default:
			{
				*orxDecPower_dBFS=(sint32_t)orxDecPower;
				YXW_DEBUG(DBG_LEVEL_ERR,"orxChannel(%d) Parameter is invalid, only 1:YXW_ORX1.\n",orxChannel);
				return (uint32_t)yxwRetAct;
			}

	}
	pMsg=&DataInfo.u.Tx;

	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;

	DataInfo.u.Tx.TxData.value=(HI_GET_ORXDACPOWER|(orxChannel<<16));
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiReadReg(hw_priv,SEND_CMD_DATA,&orxDecPower);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	*orxDecPower_dBFS=(float)((sint32_t)orxDecPower/POWER_ACCURACY);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_programObsRxGainTable(YxwDevice_t *hw_priv, YxwGainTab_t * gainTablePtr)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus = YXWHAL_OK;
	uint32_t i = 0;

	FUNCTION_ENTER(hw_priv);
	uint8_t numGainIndexesInTable = 0;

	if(gainTablePtr == NULL)
	{
		return yxwRetAct;
	}

	numGainIndexesInTable = gainTablePtr->sizeofTb;

	halstatus = YXW_spiWriteField(hw_priv,REG_GC_00,0,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);


	for(i = 0; i < numGainIndexesInTable; i++)
	{
		yxwRetAct = YXW_spiWriteMem(hw_priv, REG_ORX0_GAIN_SRAM(i), (gainTablePtr->anaGainTB[numGainIndexesInTable-1-i]<<11)|gainTablePtr->digGainTB[numGainIndexesInTable-1-i]);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct = YXW_spiWriteMem(hw_priv, REG_ORX1_GAIN_SRAM(i), (gainTablePtr->anaGainTB[numGainIndexesInTable-1-i]<<11)|gainTablePtr->digGainTB[numGainIndexesInTable-1-i]);
		IF_ERR_RETURN(yxwRetAct);
	}

	halstatus = YXW_spiWriteField(hw_priv,REG_GC_00,1,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	hw_priv->YxwState.gainIndexes.orx1MaxGainIndex = numGainIndexesInTable - 1;
	hw_priv->YxwState.gainIndexes.orx2MaxGainIndex = numGainIndexesInTable - 1;
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

uint32_t YXW_setRxTiaGain(YxwDevice_t *hw_priv, YxwTiaChannel_t channel, YxwTiaGain_t gain)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint16_t addr = channel == YXW_TIA_CHANNEL1 ? REG_RXFE1 : REG_RXFE9;
	FUNCTION_ENTER(hw_priv);
	if(gain == YXW_TIA_GAIN_MINUS_6DB) //-6db
		yxwRetAct = YXW_spiWriteField(hw_priv, addr, 0xF, 4, 1);
	else if(gain == YXW_TIA_GAIN_PLUS_0DB) //0db
		yxwRetAct = YXW_spiWriteField(hw_priv, addr, 0x3, 4, 1);
	else if(gain == YXW_TIA_GAIN_PLUS_6DB)//+6db
		yxwRetAct = YXW_spiWriteField(hw_priv, addr, 0x2, 4, 1);

	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return yxwRetAct;
}

uint32_t YXW_getRxTiaGain(YxwDevice_t *hw_priv, YxwTiaChannel_t channel, YxwTiaGain_t* gain)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint16_t addr = channel == YXW_TIA_CHANNEL1 ? REG_RXFE1 : REG_RXFE9;
	uint32_t value;
	FUNCTION_ENTER(hw_priv);

	yxwRetAct = YXW_spiReadField(hw_priv, addr, &value, 4, 1);
	IF_ERR_RETURN(yxwRetAct);

	if(!gain)
		return YXW_ERR_NULL_POINTER;

	if(value == 0xF)
		*gain = YXW_TIA_GAIN_MINUS_6DB;
	else if(value == 0x3)
		*gain = YXW_TIA_GAIN_PLUS_0DB;
	else if(value == 0x2)
		*gain = YXW_TIA_GAIN_PLUS_6DB;

	FUNCTION_EXIT(hw_priv);
	return yxwRetAct;
}

uint32_t YXW_setOrxTiaGain(YxwDevice_t *hw_priv, YxwTiaChannel_t channel, YxwTiaGain_t gain)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint16_t addr = channel == YXW_TIA_CHANNEL1 ? REG_RXFE1 : REG_RXFE9;
	FUNCTION_ENTER(hw_priv);

	if(gain == YXW_TIA_GAIN_MINUS_6DB) //-6db
		yxwRetAct = YXW_spiWriteField(hw_priv, addr, 0xF, 4, 1);
	else if(gain == YXW_TIA_GAIN_PLUS_0DB) //0db
		yxwRetAct = YXW_spiWriteField(hw_priv, addr, 0x3, 4, 1);
	else if(gain == YXW_TIA_GAIN_PLUS_6DB)//+6db
		yxwRetAct = YXW_spiWriteField(hw_priv, addr, 0x2, 4, 1);

	FUNCTION_EXIT(hw_priv);
	return yxwRetAct;
}

uint32_t YXW_getOrxTiaGain(YxwDevice_t *hw_priv, YxwTiaChannel_t channel, YxwTiaGain_t* gain)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint16_t addr = channel == YXW_TIA_CHANNEL1 ? REG_RXFE1 : REG_RXFE9;
	uint32_t value;
	FUNCTION_ENTER(hw_priv);

	yxwRetAct = YXW_spiReadField(hw_priv, addr, &value, 4, 1);
	IF_ERR_RETURN(yxwRetAct);

	if(!gain)
		return YXW_ERR_NULL_POINTER;

	if(value == 0xF)
		*gain = YXW_TIA_GAIN_MINUS_6DB;
	else if(value == 0x3)
		*gain = YXW_TIA_GAIN_PLUS_0DB;
	else if(value == 0x2)
		*gain = YXW_TIA_GAIN_PLUS_6DB;

	FUNCTION_EXIT(hw_priv);
	return yxwRetAct;
}
