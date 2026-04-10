/**
* \file yxw_tx.c
* \brief Contains functions to support Y9009 Tx data path control
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_tx.h"

#include <stdio.h>
#include <unistd.h>

#include "yxw_error.h"
#include "yxw_plat_hal.h"
#include "yxw_radio.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"
#include "yxw_riscv_types.h"

uint32_t YXW_setTxManualGain(YxwDevice_t *hw_priv,YxwTxChannels_t TxCh, uint8_t gainIndex)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	switch(TxCh)
	{
		case YXW_TX1:
			if ((gainIndex < hw_priv->YxwState.gainIndexes.tx1MinGainIndex) ||
				(gainIndex > hw_priv->YxwState.gainIndexes.tx1MaxGainIndex))
			{
				return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,
						YXW_ERR_ORX1_MAX_GAIN_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
			}
			break;
		case YXW_TX2:
			if ((gainIndex < hw_priv->YxwState.gainIndexes.tx2MinGainIndex) ||
				(gainIndex > hw_priv->YxwState.gainIndexes.tx2MaxGainIndex))
			{
				return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,
						YXW_ERR_ORX2_MAX_GAIN_VALID,yxwRetAct,YXW_ERR_CHECK_PARAM);
			}
			break;
		default:
			break;
	}

	if(TxCh == YXW_TX1)
	{
		yxwRetAct = (YxwRecoveryActions_t)YXW_spiWriteField(hw_priv, REG_GC_06,1,16,16);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct = (YxwRecoveryActions_t)YXW_spiWriteField(hw_priv, REG_GC_06,gainIndex,5,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	if(TxCh == YXW_TX2)
	{
		yxwRetAct = (YxwRecoveryActions_t)YXW_spiWriteField(hw_priv, REG_GC_06,1,17,17);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct = (YxwRecoveryActions_t)YXW_spiWriteField(hw_priv, REG_GC_06,gainIndex,13,8);
	   	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_setTxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t mode)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	/*TxGain only control by api*/
	halError = YXW_spiWriteField(hw_priv,REG_GC_05,mode,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getTxGain(YxwDevice_t *hw_priv,YxwTxChannels_t TxChannel, uint32_t *TxGainIndex)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;
	uint32_t radioStatus = 0;

	halError=YXW_spiReadReg(hw_priv, REGOUT_CALI_DONE_0,&radioStatus);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	switch(TxChannel){
		case YXW_TX1:
			if(TX1ENMASK == (radioStatus&TX1ENMASK))
			{
				halError = YXW_spiReadField(hw_priv, REGOUT_GC_22,TxGainIndex,6,0);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				halError = YXW_spiReadField(hw_priv, REG_GC_06,TxGainIndex,5,0);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			break;
		case YXW_TX2:
			if(TX2ENMASK == (radioStatus&TX2ENMASK))
			{
				halError = YXW_spiReadField(hw_priv, REGOUT_GC_22,TxGainIndex,14,8);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				halError = YXW_spiReadField(hw_priv, REG_GC_06,TxGainIndex,13,8);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
			}
			break;
		default:
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,
									YXW_ERR_TXRXORX_CHANNLE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_setPaProtectionCfg(YxwDevice_t *hw_priv,YxwTxPaProtectCfg_t *txPaProtectCfg)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	/*Cnf the cali data cnt*/
	halError = YXW_spiWriteField(hw_priv,REG_GC_46,txPaProtectCfg->TxCaliCnt,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Cnf the HighThreshold*/
	halError = YXW_spiWriteField(hw_priv,REG_GC_48,txPaProtectCfg->TxPowerThreshold,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Enable PA Protect*/
	halError = YXW_spiWriteField(hw_priv,REG_GC_47,txPaProtectCfg->Enable,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if(0 == txPaProtectCfg->Enable)
	{
		YXW_RevertClearPaProtect(hw_priv);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_getPaProtectErrorFlags(YxwDevice_t *hw_priv, uint32_t *errorFlags)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;

	/* Read back PA Protection Error flags */
	/*errorFlags==1,it indicate Tx1 generate Pa protect*/
	/*errorFlags==2,it indicate Tx2 generate Pa protect*/
	/*errorFlags==3,it indicate Tx1/Tx2 generate Pa protect*/
	halError = YXW_spiReadField(hw_priv, REG_GC_49, errorFlags,30,29);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	*errorFlags = ((*errorFlags) & YXW_TX1TX2);

	if(YXW_TX1 == *errorFlags)
	{
		YXW_DEBUG(DBG_LEVEL_STATUS,"PA_ProtectionGenerated: YXW_TX1\n");
	}
	else if(YXW_TX2 == *errorFlags)
	{
		YXW_DEBUG(DBG_LEVEL_STATUS,"PA_ProtectionGenerated: YXW_TX2\n");
	}
	else if(YXW_TX1TX2 == *errorFlags)
	{
		YXW_DEBUG(DBG_LEVEL_STATUS,"PA_ProtectionGenerated: YXW_TX1 And YXW_TX2\n");
	}

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_RevertClearPaProtect(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;
	uint32_t errorFlag,index;

	FUNCTION_ENTER(hw_priv);
	YXW_getPaProtectErrorFlags(hw_priv,&errorFlag);

	/*revert Paprotect gain*/
	if(YXW_TX1 == (errorFlag&YXW_TX1)){

		halStatus = YXW_spiReadField(hw_priv, REG_GC_49,&index,6,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct=YXW_setTxManualGain(hw_priv,YXW_TX1,index);
		IF_ERR_RETURN(yxwRetAct);
	}
	if(YXW_TX2 == (errorFlag&YXW_TX2)){

		halStatus = YXW_spiReadField(hw_priv, REG_GC_49,&index,14,8);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct=YXW_setTxManualGain(hw_priv,YXW_TX2,index);
		IF_ERR_RETURN(yxwRetAct);
	}

	/*Clear Paprotect status*/
	halStatus=YXW_spiWriteField(hw_priv,REG_GC_49,0x0,30,29);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_clearPaProtectErrorFlags(YxwDevice_t *hw_priv,YxwTxChannels_t txChannel)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	switch(txChannel){
		case YXW_TX1:
			halStatus=YXW_spiWriteField(hw_priv,DOCMD_STATUS,0x0,30,30);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX2:
			halStatus=YXW_spiWriteField(hw_priv,DOCMD_STATUS,0x0,29,29);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX1TX2:
			halStatus=YXW_spiWriteField(hw_priv,DOCMD_STATUS,0x0,30,29);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;

	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


/*This function enable a digital numerically controlled oscillator in the Yxw20*/
/*Digital to create a teset Cw tone on Tx1 or Tx2 RF output*/
uint32_t YXW_setDDsFreqConfig(YxwDevice_t *hw_priv, YxwTxChannels_t txChannel, YxwTxDdsTestToneCfg_t txDDSTone, YxwTxDdsAttenCfg_t txDDSAtten)
{
   	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;
	uint32_t txFltBypassMode;
	sint32_t Freq_word0,Freq_word1;

	FUNCTION_ENTER(hw_priv);
	/*Calculate the DDS Freq Deviation*/
	float para = ((1ULL<<31) / hw_priv->YxwState.clocks.deviceClock_kHz);

	/*if freq > 150 MHZ device clock divide by 2*/
	if(hw_priv->YxwState.clocks.deviceClock_kHz > 150000){
		para = ((1ULL<<32) / hw_priv->YxwState.clocks.deviceClock_kHz);
	}

	halError=YXW_spiReadField(hw_priv, REG_CALI_TXFLT_CTRL_0, &txFltBypassMode,3,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_0,1,11,11);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if(hw_priv->YxwState.txInputRate_kHz == 245760)
	{
		Freq_word0 = para*txDDSTone.txTone1Freq_kHz;
		Freq_word1 = para*txDDSTone.txTone2Freq_kHz;
	}
	else if(hw_priv->YxwState.txInputRate_kHz == 491520)
	{
		Freq_word0 = para/2*txDDSTone.txTone1Freq_kHz;
		Freq_word1 = para/2*txDDSTone.txTone2Freq_kHz;
	}
	else if(hw_priv->YxwState.txInputRate_kHz == 122880)
	{
		Freq_word0 = para*2*txDDSTone.txTone1Freq_kHz;
		Freq_word1 = para*2*txDDSTone.txTone2Freq_kHz;
	}
	else
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,
								YXW_ERR_TXRXORX_CHANNLE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

 	halError=YXW_spiWriteField(hw_priv, REG_CALI_DG_0, txDDSAtten.txTone0Atten, 6, 4);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_DG_0, txDDSAtten.txTone1Atten, 9, 7);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_DG_0, txDDSAtten.txTone2Atten, 12, 10);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_DG_0, txDDSAtten.txTone3Atten, 15, 13);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_DG_2, Freq_word0, 31, 0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_DG_3, Freq_word1, 31, 0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_CTRL_0, 1, 0, 0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	switch(txChannel){
		case YXW_TX1:
			halError=YXW_spiWriteField(hw_priv, REG_CALI_CTRL_0, 0, 4, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX2:
			halError=YXW_spiWriteField(hw_priv, REG_CALI_CTRL_0, 1, 4, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}
	//Enable Nco
	halError=YXW_spiWriteField(hw_priv, REG_CALI_OP_0, 1, 12, 12);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	halError=YXW_spiWriteField(hw_priv, REG_CALI_OP_0, 0 , 12, 12);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_OP_0, 0, 1, 1);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_OP_0, 1, 1, 1);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_DG_0, 1, 1, 0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

uint32_t YXW_setDdsOff(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError= YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halError=YXW_spiWriteField(hw_priv, REG_CALI_CTRL_0, 0x00,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_OP_0, 0x00,1,1);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CALI_DG_0, 0, 1, 0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_0,0,11,11);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

uint32_t YXW_getTxDecPowerFloat(YxwDevice_t *hw_priv, YxwTxChannels_t txChannel, float *txDecPower_dBFS)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;
	uint32_t txDecPower = 0;
	uint32_t radioStatus = 0;
	YxwData DataInfo;
	YxwCmdTx *pMsg;

	halStatus=YXW_spiReadReg(hw_priv, REGOUT_CALI_DONE_0,&radioStatus);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_TX,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//if Txchannel OFF DecPower return 0
	switch(txChannel){
		case YXW_TX1:
			if(TX1ENMASK != (radioStatus&TX1ENMASK))
			{
				*txDecPower_dBFS=(sint16_t)txDecPower;
				return (uint32_t)yxwRetAct;
			}
			break;
		case YXW_TX2:
			if(TX2ENMASK != (radioStatus&TX2ENMASK))
			{
				*txDecPower_dBFS=(sint16_t)txDecPower;
				return (uint32_t)yxwRetAct;
			}
			break;
		default:
			{
				*txDecPower_dBFS=(sint16_t)txDecPower;
				YXW_DEBUG(DBG_LEVEL_RESULT,"txChannel(%d) Parameter is invalid, only 1:YXW_TX1 or 2:YXW_TX2.\n",txChannel);
				return (uint32_t)yxwRetAct;
			}
	}

	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;

	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;

	DataInfo.u.Tx.TxData.value=(HI_GET_TXDACPOWER|((txChannel+2)<<16)); // TX1 : opdata = 3  TX2 : opdata = 4

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiReadReg(hw_priv,SEND_CMD_DATA,&txDecPower);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	*txDecPower_dBFS=(float)((sint32_t)txDecPower/POWER_ACCURACY);

	return (uint32_t)yxwRetAct;
}
