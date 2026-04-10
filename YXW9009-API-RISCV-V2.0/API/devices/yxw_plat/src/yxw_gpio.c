/**
* \file yxw_gpio.c
* \brief Y9009 GPIO functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_error.h"
#include "yxw_gpio.h"

#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_plat_hal.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"
#include "yxw_riscv_types.h"
#include "yxw_tx.h"


/** The paramter Value-->1-->high**/
/** otherwise 	 Value-->0-->low **/
/** gpioNum from 0->9 **/
uint32_t  YXW_setGpioValue(YxwDevice_t *hw_priv, YxwGpioPinSel_t gpioNum, uint32_t value)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;

	FUNCTION_ENTER(hw_priv);
	if((gpioNum < YXW_GPIO_00)||(gpioNum > YXW_GPIO_09))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_GPIO_NUM_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/*gpioCtrl -->0 Ctrl by riscv, 1 Ctrl by spi*/
	YXW_setGpioSourceCtrl(hw_priv, 0, gpioNum);
	IF_ERR_RETURN(yxwRetAct);

	/*YXW_SEL_5 -->set gpio functiong to gpio*/
	YXW_setGpioFuncSel(hw_priv, YXW_SEL_5, gpioNum);
	IF_ERR_RETURN(yxwRetAct);

	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	DataInfo.CmdType=0xff;
	DataInfo.u.Tx.Cmd=YXW_SET_OPCODE;
	DataInfo.u.Tx.TxData.value=HI_SET_GPIO_VALUE|(value<<31|gpioNum<<16);
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_waitGetGpioValueDone(YxwDevice_t *hw_priv, uint32_t timeout)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t uiTry = WIAT_TIMEOUT;

	FUNCTION_ENTER(hw_priv);
	/*Wait Get Gpio Value Done*/
	while(uiTry > 0){
		YXW_mdelay(timeout);
		yxwRetAct=YXW_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(yxwRetAct);
		if(hw_priv->gpio_get_value_done==1){
			YXW_clearStatus(hw_priv);
			hw_priv->gpio_get_value_done=0;
			break;
		}
		uiTry--;
	}
	if(uiTry <= 0){
		YXW_DEBUG(DBG_LEVEL_RESULT,"waitGetGpioValue Timeout!\n");
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


/** The paramter  gpioNum from 0->9 **/
/** value output value **/
uint32_t  YXW_getGpioValue(YxwDevice_t *hw_priv, YxwGpioPinSel_t gpioNum, uint32_t *value)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;

	if((gpioNum < YXW_GPIO_00)||(gpioNum > YXW_GPIO_09))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_GPIO_NUM_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/*gpioCtrl -->0 Ctrl by riscv, 1 Ctrl by spi*/
	YXW_setGpioSourceCtrl(hw_priv, 0, gpioNum);
	IF_ERR_RETURN(yxwRetAct);

	/*YXW_SEL_5 -->set gpio functiong to gpio*/
	YXW_setGpioFuncSel(hw_priv, YXW_SEL_5, gpioNum);
	IF_ERR_RETURN(yxwRetAct);

	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	DataInfo.CmdType=0xff;
	DataInfo.u.Tx.Cmd=YXW_SET_OPCODE;
	DataInfo.u.Tx.TxData.value=HI_GET_GPIO_VALUE|gpioNum<<16;
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_waitGetGpioValueDone(hw_priv, 100);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=(YxwRecoveryActions_t)YXW_spiReadField(hw_priv,REG_GPIO_CTRL_0,value,31,31);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}

/** The paramter gpioOutEn-->1-->output**/
/** otherwise gpioOutEn-->0-->input **/
/** gpioNum from 0->18 **/
uint32_t  YXW_setGpioDirection(YxwDevice_t *hw_priv, YxwGpioDir_t gpioOutEn, YxwGpioPinSel_t gpioNum)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;
	uint32_t GpioMap_RegBit;

	FUNCTION_ENTER(hw_priv);
	if((gpioNum >= YXW_GPIO_00)&&(gpioNum <= YXW_GPIO_09))
	{
		GpioMap_RegBit=gpioNum;
	}
	else if((gpioNum >= YXW_GPIO_10)&&(gpioNum <= YXW_GPIO_16))
	{
		GpioMap_RegBit=gpioNum + YXW_GPIO_10;
	}
	else
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_GPIO_NUM_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/* Y9009 SPI regs to set GPIO OE direction */
	halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_0,gpioOutEn,GpioMap_RegBit,GpioMap_RegBit);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t  YXW_getGpioDirection(YxwDevice_t *hw_priv, YxwGpioDir_t *gpioOutEn, YxwGpioPinSel_t gpioNum)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;
	uint32_t GpioMap_RegBit;

	if((gpioNum >= YXW_GPIO_00)&&(gpioNum <= YXW_GPIO_09))
	{
		GpioMap_RegBit=gpioNum;
	}
	else if((gpioNum >= YXW_GPIO_10)&&(gpioNum <= YXW_GPIO_16))
	{
		GpioMap_RegBit=gpioNum + YXW_GPIO_10;
	}
	else
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
				YXW_ERR_GPIO_NUM_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/* Y9009 SPI regs to set GPIO OE direction */
	halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_0,(uint32_t*)gpioOutEn,GpioMap_RegBit,GpioMap_RegBit);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}
/** The paramter gpioCtrlMode-->1--> spi**/
/** otherwise gpioNum-->0-->riscv **/
/** gpioNum from 0->18 **/

uint32_t  YXW_setGpioSourceCtrl(YxwDevice_t *hw_priv, YxwGpioCtrlMode_t gpioCtrlMode, YxwGpioPinSel_t gpioNum)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;
	uint32_t GpioMap_RegBit;

	FUNCTION_ENTER(hw_priv);
	if((gpioNum >= YXW_GPIO_00)&&(gpioNum <= YXW_GPIO_09))
	{
		GpioMap_RegBit=(gpioNum+10);

		/* Y9009 SPI regs to set GPIO Ctrl */
		halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_0,(uint32_t)gpioCtrlMode,GpioMap_RegBit,GpioMap_RegBit);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}else if((gpioNum >= YXW_GPIO_10)&&(gpioNum <= YXW_GPIO_16)){
		//Gpio10-Gpio16 cant be controled by riscv,it is controled by spi default
	}else{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_GPIO_NUM_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t  YXW_getGpioSourceCtrl(YxwDevice_t *hw_priv, YxwGpioCtrlMode_t *gpioCtrlMode, YxwGpioPinSel_t gpioNum)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;
	uint32_t GpioMap_RegBit;

	GpioMap_RegBit=(gpioNum+10);
	/* Y9009 SPI regs to get GPIO Ctrl */
	halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_0,(uint32_t*)gpioCtrlMode,GpioMap_RegBit,GpioMap_RegBit);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}

/** The paramter gpioFuncSel-->1--> spi**/
/** otherwise gpioNum-->0-->riscv **/
/** gpioNum from 0->18 **/
uint32_t  YXW_setGpioFuncSel(YxwDevice_t *hw_priv, YxwGpioPinFunc_t gpioFuncSel, YxwGpioPinSel_t gpioNum)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	switch(gpioNum){
		case YXW_GPIO_00:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,2,0);
			break;
		case YXW_GPIO_01:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,5,3);
			break;
		case YXW_GPIO_02:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,8,6);
			break;
		case YXW_GPIO_03:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,11,9);
			break;
		case YXW_GPIO_04:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,14,12);
			break;
		case YXW_GPIO_05:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,17,15);
			break;
		case YXW_GPIO_06:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,20,18);
			break;
		case YXW_GPIO_07:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,23,21);
			break;
		case YXW_GPIO_08:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,26,24);
			break;
		case YXW_GPIO_09:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_1,(uint32_t)gpioFuncSel,29,27);
			break;
		case YXW_GPIO_10:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,2,0);
			break;
		case YXW_GPIO_11:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,5,3);
			break;
		case YXW_GPIO_12:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,8,6);
			break;
		case YXW_GPIO_13:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,11,9);
			break;
		case YXW_GPIO_14:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,14,12);
			break;
		case YXW_GPIO_15:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,17,15);
			break;
		case YXW_GPIO_16:
			halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_CTRL_2,(uint32_t)gpioFuncSel,20,18);
			break;
		default:
			break;
	}

	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t  YXW_getGpioFuncSel(YxwDevice_t *hw_priv, YxwGpioPinFunc_t *gpioFuncSel, YxwGpioPinSel_t gpioNum)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	switch(gpioNum){
		case YXW_GPIO_00:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,2,0);
			break;
		case YXW_GPIO_01:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,5,3);
			break;
		case YXW_GPIO_02:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,8,6);
			break;
		case YXW_GPIO_03:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,11,9);
			break;
		case YXW_GPIO_04:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,14,12);
			break;
		case YXW_GPIO_05:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,17,15);
			break;
		case YXW_GPIO_06:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,20,18);
			break;
		case YXW_GPIO_07:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,23,21);
			break;
		case YXW_GPIO_08:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,26,24);
			break;
		case YXW_GPIO_09:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_1,(uint32_t*)gpioFuncSel,29,27);
			break;
		case YXW_GPIO_10:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,2,0);
			break;
		case YXW_GPIO_11:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,5,3);
			break;
		case YXW_GPIO_12:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,8,6);
			break;
		case YXW_GPIO_13:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,11,9);
			break;
		case YXW_GPIO_14:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,14,12);
			break;
		case YXW_GPIO_15:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,17,15);
			break;
		case YXW_GPIO_16:
			halstatus = YXW_spiReadField(hw_priv,REG_GPIO_CTRL_2,(uint32_t*)gpioFuncSel,20,18);
			break;
		default:
			break;
	}
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}

/** gpioNum from 0->9 **/
/** enable from 0->1 **/
/** By default,  GPIO interrupt is triggered by falling edge **/
uint32_t YXW_setGpioIntr(YxwDevice_t *hw_priv, YxwGpioPinSel_t gpioNum, uint32_t enable)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	YxwData DataInfo;

	FUNCTION_ENTER(hw_priv);
	if ((gpioNum < YXW_GPIO_00) || (gpioNum > YXW_GPIO_09))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_GPIO,
			YXW_ERR_GPIO_NUM_VALID, yxwRetAct, YXW_ERR_CHECK_PARAM);
	}
	if (enable != 0 && enable != 1)
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_GPIO,
			YXW_ERR_INV_NULL_INIT_PARAM, yxwRetAct, YXW_ERR_CHECK_PARAM);
	}

	YXW_Memset(&DataInfo, 0, sizeof(YxwData));
	DataInfo.CmdType = 0xff;
	DataInfo.u.Tx.Cmd = YXW_SET_OPCODE;
	DataInfo.u.Tx.TxData.value = HI_SET_GPIO_INTR | (enable << 31 | gpioNum << 16);
	yxwRetAct = YXW_sendRiscvCmd(hw_priv, (void*)&DataInfo, IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t  YXW_setGpio3v3Oe(YxwDevice_t *hw_priv, uint32_t gpio3v3OutEn, YxwGpioPinSel_t gpioNum)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	uint32_t GpioMap5_0=0;
	uint32_t GpioMap11_6=16;

	FUNCTION_ENTER(hw_priv);
	/* Y9009 SPI regs to set GPIO 3V3 OE direction */
	/*Gpio11-6 output enable*/
	if(gpioNum>=6){
		halstatus = YXW_spiWriteField(hw_priv,REG_MISC_TOP2,gpio3v3OutEn,GpioMap5_0+gpioNum,GpioMap5_0+gpioNum);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	/*Gpio5-0 output enable*/
	if(gpioNum<6){
		halstatus = YXW_spiWriteField(hw_priv,REG_MISC_TOP4,gpio3v3OutEn,GpioMap11_6+gpioNum,GpioMap11_6+gpioNum);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t  YXW_getGpio3v3Oe(YxwDevice_t *hw_priv, uint32_t *gpio3v3OutEn)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	uint32_t Gpio5_0Mask=0;
	uint32_t Gpio11_6Mask=0;
	/* Y9009 SPI regs to get GPIO 3V3 OE direction */

	/*Gpio11-6 output enable*/
	halstatus = YXW_spiReadField(hw_priv,REG_MISC_TOP2,&Gpio5_0Mask,5,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Gpio5-0 output enable*/
	halstatus = YXW_spiReadField(hw_priv,REG_MISC_TOP4,&Gpio11_6Mask,21,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	*gpio3v3OutEn=(Gpio5_0Mask|Gpio11_6Mask<<6);

	return (uint32_t)yxwRetAct;
}

uint32_t  YXW_setGpio3v3Ie(YxwDevice_t *hw_priv, uint32_t gpio3v3InEn, YxwGpioPinSel_t gpioNum)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;
	uint32_t GpioMap5_0=0;
	uint32_t GpioMap11_6=24;

	FUNCTION_ENTER(hw_priv);
	/*Gpio11-6 input enable*/
	if(gpioNum>=6){
		halstatus = YXW_spiWriteField(hw_priv,REG_MISC_TOP2,gpio3v3InEn,GpioMap5_0+gpioNum,GpioMap5_0+gpioNum);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	/*Gpio5-0 input enable*/
	if(gpioNum<6){
		halstatus = YXW_spiWriteField(hw_priv,REG_MISC_TOP4,gpio3v3InEn,GpioMap11_6+gpioNum,GpioMap11_6+gpioNum);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getGpIntStatus(YxwDevice_t *hw_priv,YxwGpIntMask_t *gpIntStatus)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	halstatus = YXW_spiReadReg(hw_priv,DOCMD_STATUS,(uint32_t *)gpIntStatus);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}

uint32_t  YXW_getGpio3v3Ie(YxwDevice_t *hw_priv, uint32_t *gpio3v3InEn)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	uint32_t Gpio5_0Mask=0;
	uint32_t Gpio11_6Mask=0;
	/* Y9009 SPI regs to get GPIO 3V3 IE direction */

	/*Gpio11-6 input enable*/
	halstatus = YXW_spiReadField(hw_priv,REG_MISC_TOP2,&Gpio5_0Mask,13,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Gpio5-0 input enable*/
	halstatus = YXW_spiReadField(hw_priv,REG_MISC_TOP4,&Gpio11_6Mask,29,24);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	*gpio3v3InEn=(Gpio5_0Mask|Gpio11_6Mask<<6);

	return (uint32_t)yxwRetAct;

}

uint32_t YXW_setupAuxDacs(YxwDevice_t *hw_priv,YxwGpioPinSel_t Gpio3V3Num, YxwAuxDacVref_t YxwAuxDacVref, YxwAuxDacResolution_t YxwAuxDacResolution)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	uint32_t auxDacCnf = 0x00;
	uint32_t auxDacGPIOCnf = 0x00;
	uint32_t auxDacPowerCnf = 0x00;
	uint32_t auxDacModeCnf = 0x00;
	FUNCTION_ENTER(hw_priv);

	if(Gpio3V3Num<=5)
	{
		//for(i=0;i<5;i++)
		{
			halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP4,&auxDacGPIOCnf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*gpio input mode disable*/
			auxDacGPIOCnf&=~BIT(Gpio3V3Num+24);
			/*gpio output mode disable*/
			auxDacGPIOCnf&=~BIT(Gpio3V3Num+16);

			halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP2,&auxDacPowerCnf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*auxdac top bias powerup*/
			auxDacPowerCnf&=~BIT(25);
			/*auxdac powerup*/
			auxDacPowerCnf&=~BIT(Gpio3V3Num+16);

			halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP3,&auxDacModeCnf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*vref set to 1-2.5v seting*/
			auxDacModeCnf &=~(0x03<<(Gpio3V3Num*2));
			auxDacModeCnf|=((YxwAuxDacVref&0x03)<<(Gpio3V3Num*2));
			/*auxdac rgain/10-12bit mode*/
			auxDacModeCnf &=~BIT(Gpio3V3Num+16);
			auxDacModeCnf |=((YxwAuxDacResolution&0x01)<<(Gpio3V3Num+16));
			/*auxdac ictl/10-12bit mode*/
			auxDacModeCnf &=~BIT(Gpio3V3Num+22);
			auxDacModeCnf |=(((YxwAuxDacResolution&0x02)>>1)<<(Gpio3V3Num+22));

			if(Gpio3V3Num<5)
			{
				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP4,auxDacGPIOCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP2,auxDacPowerCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);


				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP3,auxDacModeCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

			}else{
				halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP4,&auxDacCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				auxDacCnf &=~0x03;
				auxDacCnf|=(YxwAuxDacVref&0x03);

				auxDacCnf &=~BIT(12);
				auxDacCnf |=((YxwAuxDacResolution&0x01)<<(12));

				auxDacCnf &=~BIT(5);
				auxDacCnf |=(((YxwAuxDacResolution&0x02)>>1)<<(5));
				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP4,auxDacCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
		}
	}else{
		//for(i=0;i<5;i++)
		{
			halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP2,&auxDacGPIOCnf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			/*gpio input mode disable*/
			auxDacGPIOCnf&=~BIT(Gpio3V3Num+2);
			/*gpio output mode disable*/
			auxDacGPIOCnf&=~BIT(Gpio3V3Num-6);

			halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP0,&auxDacPowerCnf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			/*auxdac top bias powerup*/
			auxDacPowerCnf&=~BIT(9);
			/*auxdac powerup*/
			auxDacPowerCnf&=~BIT(Gpio3V3Num-6);

			halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP1,&auxDacModeCnf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			/*vref set to 1-2.5v seting*/
			auxDacModeCnf &=~(0x03<<((Gpio3V3Num*2)+4));
			auxDacModeCnf|=((YxwAuxDacVref&0x03)<<((Gpio3V3Num*2)+4));
			/*auxdac rgain/10-12bit mode*/
			auxDacModeCnf &=~BIT(Gpio3V3Num-6);
			auxDacModeCnf |=((YxwAuxDacResolution&0x01)<<(Gpio3V3Num-6));
			/*auxdac ictl/10-12bit mode*/
			auxDacModeCnf &=~BIT(Gpio3V3Num);
			auxDacModeCnf |=(((YxwAuxDacResolution&0x02)>>1)<<(Gpio3V3Num));

			if(Gpio3V3Num<11)
			{
				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP2,auxDacGPIOCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP0,auxDacPowerCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);


				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP1,auxDacModeCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

			}else{

				halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP2,&auxDacGPIOCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				/*gpio input mode disable*/
				auxDacGPIOCnf&=~BIT(13);
				/*gpio output mode disable*/
				auxDacGPIOCnf&=~BIT(5);
				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP2,auxDacGPIOCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halstatus = YXW_spiReadReg(hw_priv,REG_MISC_TOP1,&auxDacModeCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				/*vref set to 1-2.5v seting*/
				auxDacModeCnf &=~(0x03<<16);
				auxDacModeCnf|=((YxwAuxDacVref&0x03)<<16);
				/*auxdac rgain/10-12bit mode*/
				auxDacModeCnf &=~BIT(20);
				auxDacModeCnf |=((YxwAuxDacResolution&0x01)<<(20));
				/*auxdac ictl/10-12bit mode*/
				auxDacModeCnf &=~BIT(21);
				auxDacModeCnf |=(((YxwAuxDacResolution&0x02)>>1)<<(21));
				/*auxdac powerup*/
				auxDacModeCnf &=~BIT(28);
				halstatus = YXW_spiWriteReg(hw_priv,REG_MISC_TOP1,auxDacModeCnf);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
		}

	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_writeAuxDac(YxwDevice_t *hw_priv,YxwGpioPinSel_t Gpio3V3Num,uint16_t auxDacCode)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	switch(Gpio3V3Num){
		case YXW_GPIO_00:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_2,auxDacCode,9,0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_01:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_2,auxDacCode,19,10);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_02:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_2,auxDacCode,29,20);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_03:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_3,auxDacCode,9,0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_04:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_3,auxDacCode,19,10);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_05:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_3,auxDacCode,29,20);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_06:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_0,auxDacCode,9,0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_07:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_0,auxDacCode,19,10);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_08:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_0,auxDacCode,29,20);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_09:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_1,auxDacCode,9,0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_10:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_1,auxDacCode,19,10);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_11:
			halstatus = YXW_spiWriteField(hw_priv,REG_AUXDA_1,auxDacCode,29,20);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
		}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


/** gpio monitor pos sel types(Used to select the position of 8-bit monitor signal on GPIO)**/
/** GPIO Select :{ [15:12],[11:8],[7:4],[3:0] } **/
/** 8d0 and 4d0 means invalid bit data **/

/*!< GPIO Select :{8d0, monitor_valid_data[7:0]}*/
static uint32_t YXW_setGpioFunForMonitorPosSelD0(YxwDevice_t *hw_priv, uint32_t monitor_ena)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	int i = 0;

	FUNCTION_ENTER(hw_priv);
	for(i = 0; i < YXW_GPIO_08; i++)
	{
		if(monitor_ena&(1<<i))
		{
			yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, 1, i);
			IF_ERR_RETURN(yxwRetAct);
			yxwRetAct=YXW_setGpioDirection(hw_priv, 1, i);
			IF_ERR_RETURN(yxwRetAct);
			yxwRetAct=YXW_setGpioFuncSel(hw_priv, YXW_SEL_7, i);
			IF_ERR_RETURN(yxwRetAct);
		}
	}

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

/*!< GPIO Select :{monitor_valid_data[7:4], 4d0, 4d0, monitor_valid_data[3:0]}*/
static uint32_t YXW_setGpioFunForMonitorPosSelD1(YxwDevice_t *hw_priv, uint32_t monitor_ena)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	int i = 0;

	FUNCTION_ENTER(hw_priv);
	for(i = 0; i < YXW_GPIO_08; i++)
	{
		if(monitor_ena&(1<<i))
		{
			if(i <= YXW_GPIO_03)
			{
				yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, 1, i);
				IF_ERR_RETURN(yxwRetAct);
				yxwRetAct=YXW_setGpioDirection(hw_priv, 1, i);
				IF_ERR_RETURN(yxwRetAct);
				yxwRetAct=YXW_setGpioFuncSel(hw_priv, YXW_SEL_7, i);
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, 1, (i+YXW_GPIO_08));
				IF_ERR_RETURN(yxwRetAct);
				yxwRetAct=YXW_setGpioDirection(hw_priv, 1, (i+YXW_GPIO_08));
				IF_ERR_RETURN(yxwRetAct);
				yxwRetAct=YXW_setGpioFuncSel(hw_priv, YXW_SEL_7, (i+YXW_GPIO_08));
				IF_ERR_RETURN(yxwRetAct);
			}
		}
	}

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

/*!< GPIO Select :{4d0, monitor_valid_data[3:0], monitor_valid_data[7:4], 4d0}*/
static uint32_t YXW_setGpioFunForMonitorPosSelD2(YxwDevice_t *hw_priv, uint32_t monitor_ena)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	int i = 0;

	FUNCTION_ENTER(hw_priv);
	for(i = 0; i < YXW_GPIO_08; i++)
	{
		if(monitor_ena&(1<<i))
		{
			if(i <= YXW_GPIO_03)
			{
				yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, 1, (i+YXW_GPIO_08));
				IF_ERR_RETURN(yxwRetAct);
				yxwRetAct=YXW_setGpioDirection(hw_priv, 1, (i+YXW_GPIO_08));
				IF_ERR_RETURN(yxwRetAct);
				yxwRetAct=YXW_setGpioFuncSel(hw_priv, YXW_SEL_7, (i+YXW_GPIO_08));
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, 1, i);
				IF_ERR_RETURN(yxwRetAct);
				yxwRetAct=YXW_setGpioDirection(hw_priv, 1, i);
				IF_ERR_RETURN(yxwRetAct);
				yxwRetAct=YXW_setGpioFuncSel(hw_priv, YXW_SEL_7, i);
				IF_ERR_RETURN(yxwRetAct);
			}
		}
	}

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

/*!< GPIO Select :{monitor_valid_data[7:0], 8d0}*/
static uint32_t YXW_setGpioFunForMonitorPosSelD3(YxwDevice_t *hw_priv, uint32_t monitor_ena)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	int i = 0;

	FUNCTION_ENTER(hw_priv);
	for(i = 0; i < YXW_GPIO_08; i++)
	{
		if(monitor_ena&(1<<i))
		{
			yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, YXW_SPI_CTRL, (i+YXW_GPIO_08));
			IF_ERR_RETURN(yxwRetAct);
			yxwRetAct=(YxwRecoveryActions_t)YXW_setGpioDirection(hw_priv, YXW_GPIO_DIR_OUT, (i+YXW_GPIO_08));
			IF_ERR_RETURN(yxwRetAct);
			yxwRetAct=YXW_setGpioFuncSel(hw_priv, YXW_SEL_7, (i+YXW_GPIO_08));
			IF_ERR_RETURN(yxwRetAct);
		}
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_setGpioMonitorOut(YxwDevice_t *hw_priv,uint32_t monitorIndex,uint32_t monitor_ena,YxwGpioMonitorPosSel_t monitor_pos_sel_type)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	switch (monitor_pos_sel_type)
	{
		case YXW_GPIO_MONITOR_POS_SEL_D0:
			/*!< GPIO Select :{8d0, monitor_valid_data[7:0]}*/
			yxwRetAct=YXW_setGpioFunForMonitorPosSelD0(hw_priv, monitor_ena);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_MONITOR_POS_SEL_D1:
			/*!< GPIO Select :{monitor_valid_data[7:4], 4d0, 4d0, monitor_valid_data[3:0]}*/
			yxwRetAct=YXW_setGpioFunForMonitorPosSelD1(hw_priv, monitor_ena);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_MONITOR_POS_SEL_D2:
			/*!< GPIO Select :{4d0, monitor_valid_data[3:0], monitor_valid_data[7:4], 4d0}*/
			yxwRetAct=YXW_setGpioFunForMonitorPosSelD2(hw_priv, monitor_ena);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_GPIO_MONITOR_POS_SEL_D3:
			/*!< GPIO Select :{monitor_valid_data[7:0], 8d0}*/
			yxwRetAct=YXW_setGpioFunForMonitorPosSelD3(hw_priv, monitor_ena);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,
				YXW_ERR_GPIO_MONITOR_PARAM_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/*  set gpio_monitor_pos_sel,value =0,1,2,3 */
	halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_MODE_2,monitor_pos_sel_type,9,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Enable monitor*/
	halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_MODE_2,monitor_ena,7,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Setup monitorIndex*/
	halstatus = YXW_spiWriteField(hw_priv,REG_GPIO_MODE_2,monitorIndex,15,12);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_getGpioMonitorOut(YxwDevice_t *hw_priv,uint32_t *monitorIndex,uint32_t* monitorEna)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	/*Get monitor out enable*/
	halstatus = YXW_spiReadField(hw_priv,REG_GPIO_MODE_2,monitorEna,7,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Get monitor Index*/
	halstatus = YXW_spiReadField(hw_priv,REG_GPIO_MODE_2,monitorIndex,15,12);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_startAuxAdc(YxwDevice_t *hw_priv,YxwAuxAdcConfig_t *auxAdcConfig)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	static const uint16_t MIN_AUXADC_SAMPLES = 1;
	static const uint16_t MAX_AUXADC_SAMPLES = 1000;
	static const uint16_t MIN_AUXADC_SAMPLING_PERIOD_US = 1;
	static const uint16_t MAX_AUXADC_SAMPLING_PERIOD_US = 10;

	FUNCTION_ENTER(hw_priv);
	/*Null Check auxAdcConfig structure*/
	if (auxAdcConfig == NULL)
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
					YXW_ERR_DEVICE_INIT_NULL_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/*Range check that AuxADC Channel selected is valid*/
	if((auxAdcConfig->auxAdcChannelSel != YXW_AUXADC_CH0) &&
	   (auxAdcConfig->auxAdcChannelSel != YXW_AUXADC_CH1) &&
	   (auxAdcConfig->auxAdcChannelSel != YXW_AUXADC_CH2) &&
	   (auxAdcConfig->auxAdcChannelSel != YXW_AUXADC_CH3))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
					YXW_ERR_DEVICE_INIT_NULL_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/*Range check that AuxADC mode is valid*/
	if((auxAdcConfig->auxAdcMode != YXW_AUXADC_NONPIN_MODE) &&
	   (auxAdcConfig->auxAdcMode != YXW_AUXADC_PIN_MODE))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
					YXW_ERR_DEVICE_INIT_NULL_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/*Range check that number of samples for conversion is within range*/
	if((auxAdcConfig->numSamples < MIN_AUXADC_SAMPLES) ||
	   (auxAdcConfig->numSamples > MAX_AUXADC_SAMPLES))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
					YXW_ERR_DEVICE_INIT_NULL_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/*Range check that sampling period is at least minimum sampling period for a non-pin mode*/
	if((auxAdcConfig->auxAdcMode == YXW_AUXADC_NONPIN_MODE) &&
	   (auxAdcConfig->samplingPeriod_us < MIN_AUXADC_SAMPLING_PERIOD_US) &&
	   (auxAdcConfig->samplingPeriod_us > MAX_AUXADC_SAMPLING_PERIOD_US))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
					YXW_ERR_DEVICE_INIT_NULL_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/*write auxadcMode*/
	halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC_0,auxAdcConfig->auxAdcMode,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*auxadc intnumber,This value use default*/
	halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC_1,0x03,18,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*write auxadc_numbersamples */
	halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC_1,auxAdcConfig->numSamples,9,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Setup auxadc_clkdiv */
	halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC_1,0x03,26,24);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Enable auxadc_clk*/
	halstatus = YXW_spiWriteField(hw_priv,REG_CLK_SWAP_0,0x1,4,4);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus = YXW_spiWriteField(hw_priv,REG_CLK_SWAP_0,0x1,8,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Setup auxadc  powerup*/
	halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC0,0x11002000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Setup auxadc channelsel*/
	halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC0,auxAdcConfig->auxAdcChannelSel,3,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*only use nopin mode*/
	if(auxAdcConfig->auxAdcMode == YXW_AUXADC_NONPIN_MODE)
	{
		/*write auxadc samplingPeriod_us*/
		halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC_1,auxAdcConfig->samplingPeriod_us,13,12);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		/*Enable auxad _start*/
		halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC_0,1,4,4);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC_0,0,4,4);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

	}else{ //pinmode
		/*Setup gpio function to auxadc mode*/
		YXW_setGpioSourceCtrl(hw_priv,1,auxAdcConfig->auxadcGpioSel);
		YXW_setGpioDirection(hw_priv,0,auxAdcConfig->auxadcGpioSel);
		YXW_setGpioFuncSel(hw_priv,YXW_SEL_6,auxAdcConfig->auxadcGpioSel);
		/*setup auxadc gpiosel */
		halstatus = YXW_spiWriteField(hw_priv,REG_AUXADC_1,auxAdcConfig->auxadcGpioSel,23,20);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_readAuxAdc(YxwDevice_t *hw_priv,YxwAuxAdcResult_t *auxAdcConfig)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;
	uint32_t uiTry = WIAT_TIMEOUT;

	FUNCTION_ENTER(hw_priv);
	do{
		halstatus = YXW_spiReadField(hw_priv,REGOUT_AUXADC_0,&auxAdcConfig->completeIndicator,16,16);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		if(auxAdcConfig->completeIndicator){
			break;
		}
		uiTry--;
	}while(uiTry > 0);

	if(uiTry<=0)
	{
		YXW_DEBUG(DBG_LEVEL_RESULT,"readAuxAdc Timeout!\n");
		return (uint32_t)yxwRetAct;
	}

	halstatus = YXW_spiReadField(hw_priv,REGOUT_AUXADC_0,&auxAdcConfig->auxAdcCodeAvg,15,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_GPIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


