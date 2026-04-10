/**
* \file yxw_hopping.c
* \brief Y9009 hopping frequency functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_cals.h"
#include "yxw_hopping.h"

#include "yxw_cals.h"
#include "yxw_cals.h"
#include "yxw_cals.h"
#include "yxw_error.h"
#include "yxw_plat_hal.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"
#include "yxw_riscv_types.h"

static uint32_t isHoppingListChanged(uint32_t* freqList0, uint32_t* freqList1, uint8_t freqNum)
{
	if (freqNum > HOPPING_LIST_MAX)
	{
		return 0;
	}
	if (freqList0 == NULL || freqList1 == NULL)
	{
		return 0;
	}

	return memcmp(freqList0, freqList1, freqNum * sizeof(freqList0[0]));
}

static uint32_t writeHoppingList(YxwDevice_t *hw_priv, uint32_t* freqList, uint8_t freqNum)
{
	if (hw_priv == NULL)
	{
		return YXW_ERR_NULL_POINTER;
	}
	if (freqList == NULL)
	{
		return YXW_ERR_NULL_POINTER;
	}
	if (freqNum > HOPPING_LIST_MAX)
	{
		return YXW_ERR_CHECK_PARAM;
	}
	FUNCTION_ENTER(hw_priv);

	uint32_t i = 0;
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	YxwData DataInfo;
	YxwMem *pMsg;
	YXW_Memset(&DataInfo, 0, sizeof(YxwData));
	pMsg = &DataInfo.u.Mmsg;
	DataInfo.CmdType = 0xff;
	pMsg->Cmd = YXW_WRITE_MEM_OPCODE;
	pMsg->Msg.Id = HI_MSG_TO_HOPFREQ_MSGID;
	pMsg->Msg.Lenth = HI_MSG_TO_HOPFREQ_MSGLEN;
	for (i = 0; i < freqNum; i++)
	{
		uint64_t pllLoFreq_Hz = freqList[i] * 1000000ULL;
		/*Write RfPllFreq to SRAM*/
		/*long byte array with parsed bytes from rfPllLoFrequency_Hz word */
		pMsg->Msg.data[0] = (uint8_t)(pllLoFreq_Hz & 0xFF);
		pMsg->Msg.data[1] = (uint8_t)((pllLoFreq_Hz >> 8) & 0xFF);
		pMsg->Msg.data[2] = (uint8_t)((pllLoFreq_Hz >> 16) & 0xFF);
		pMsg->Msg.data[3] = (uint8_t)((pllLoFreq_Hz >> 24) & 0xFF);
		pMsg->Msg.data[4] = (uint8_t)((pllLoFreq_Hz >> 32) & 0xFF);
		pMsg->Msg.data[5] = (uint8_t)((pllLoFreq_Hz >> 40) & 0xFF);
		pMsg->Msg.data[6] = (uint8_t)((pllLoFreq_Hz >> 48) & 0xFF);
		pMsg->Msg.data[7] = (uint8_t)((pllLoFreq_Hz >> 56) & 0xFF);
		memcpy(&(pMsg->Msg.data[8]), pMsg->Msg.data, 8);

		pMsg->Msg.offset = i * sizeof(HOPPING_FREQ);
		/*Construct the cmdto fw*/
		yxwRetAct = YXW_sendRiscvCmd(hw_priv, (void*)&DataInfo, IOCTL_WRITE_CMD);
		IF_ERR_RETURN(yxwRetAct);
	}

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

static uint32_t writeHoppingConfig(YxwDevice_t *hw_priv, uint32_t item, uint32_t val)
{
	if (hw_priv == NULL)
	{
		return YXW_ERR_NULL_POINTER;
	}
	FUNCTION_ENTER(hw_priv);

	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	YxwData DataInfo = { 0 };
	YxwCmdTx *pMsg = NULL;
	DataInfo.CmdType = 0xff;
	pMsg = &DataInfo.u.Tx;
	pMsg->Cmd = YXW_SET_OPCODE;
	pMsg->TxData.value = item | (val << 16);
	yxwRetAct = YXW_sendRiscvCmd(hw_priv, (void*)&DataInfo, IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

static uint32_t YXW_setGpioFunForHopPosSel(YxwDevice_t *hw_priv, YxwGpioPinSel_t GpioNum,YxwGpioPinFunc_t GpioFunc)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	yxwRetAct=YXW_setGpioSourceCtrl(hw_priv, YXW_SPI_CTRL,GpioNum);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioDirection(hw_priv, YXW_GPIO_DIR_IN,GpioNum);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioFuncSel(hw_priv, GpioFunc,GpioNum);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_setGpioIntr(hw_priv, GpioNum, 1);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_initHopping(YxwDevice_t *hw_priv, YxwHoppingInfo_t* config)
{
	if (hw_priv == NULL)
	{
		return YXW_ERR_NULL_POINTER;
	}
	if (config == NULL)
	{
		return YXW_ERR_NULL_POINTER;
	}
	FUNCTION_ENTER(hw_priv);

	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	yxwHalErr_t halstatus = YXWHAL_OK;
	uint32_t isListChanged = 0;

	if (isHoppingListChanged(hw_priv->YxwState.hoppingInfo.List, config->List, config->ListNum))
	{
		memcpy(hw_priv->YxwState.hoppingInfo.List, config->List, config->ListNum * sizeof(config->List[0]));
		hw_priv->YxwState.hoppingInfo.ListNum = config->ListNum;
		yxwRetAct = writeHoppingList(hw_priv, hw_priv->YxwState.hoppingInfo.List, hw_priv->YxwState.hoppingInfo.ListNum);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct = writeHoppingConfig(hw_priv, HI_SET_HOPPINGLIST, hw_priv->YxwState.hoppingInfo.ListNum);
		IF_ERR_RETURN(yxwRetAct);
		isListChanged = 1;
	}

	halstatus=YXW_spiWriteField(hw_priv, REG_CALI_RXIQ0_CTRL_0, 0x1, 15, 15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_HOPPING, halstatus, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if (isListChanged)
	{
		uint32_t initCalType =0;

		yxwRetAct = YXW_runInitCals(hw_priv, initCalType);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct = YXW_waitInitCalsDone(hw_priv, 2000);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct = YXW_getInitCalsStatus(hw_priv);
		yxwRetAct = YXW_NO_ACTION;
		IF_ERR_RETURN(yxwRetAct);
	}

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_triggerHopping_Spi(YxwDevice_t *hw_priv, uint32_t index)
{
	return writeHoppingConfig(hw_priv, HI_SET_HOPPINGINDEX, index);
}

uint32_t YXW_configHopping_Pin(YxwDevice_t *hw_priv, YxwGpioPinSel_t pinNum)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	hw_priv->YxwState.hoppingInfo.triggerPin = pinNum;

	yxwRetAct=YXW_setGpioFunForHopPosSel(hw_priv,pinNum,YXW_SEL_5);
	IF_ERR_RETURN(yxwRetAct);

 	yxwRetAct=writeHoppingConfig(hw_priv, HI_SET_HOPPINGCONFIG, pinNum);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

uint32_t YXW_deinitHopping(YxwDevice_t *hw_priv)
{
	if (hw_priv == NULL)
	{
		return YXW_ERR_NULL_POINTER;
	}
	FUNCTION_ENTER(hw_priv);

	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	halstatus=YXW_spiWriteField(hw_priv, REG_CALI_RXIQ0_CTRL_0, 0x0, 15, 15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_HOPPING, halstatus, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

