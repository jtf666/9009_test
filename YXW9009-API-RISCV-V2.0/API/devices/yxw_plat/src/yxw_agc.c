/**
* \file yxw_agc.c
* \brief Contains Y9009 API AGC function calls
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_agc.h"

#include "yxw_error.h"
#include "yxw_plat_hal.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"

uint32_t YXW_setupRxAgc(YxwDevice_t *hw_priv, YxwAgcCfg_t *rxAgcCtrl)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
   	yxwHalErr_t halError = YXWHAL_OK;
	uint32_t agcRegister = 0;
	uint32_t value = 0;

	FUNCTION_ENTER(hw_priv);
	/*Disable rx0/rx1 analog gain ctrl external mode*/
	/*And use AGC module*/
	YXW_spiWriteField(hw_priv,REG_GC_50,0x0,3,2);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiReadReg(hw_priv,REG_GC_00,&value);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/* checking for valid YxwAgcCfg_t hw_priv->rx->rxAgcCtrl pointer to determine if it has been initialized */
	if (rxAgcCtrl == NULL)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"Error:rxAgcCtrl parameter is NULL\n");
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}
	/*set RX AGC Clock Division Ratio*/
	halError = YXW_spiWriteField(hw_priv, REG_GC_00, rxAgcCtrl->agcClkDiv, 1, 0);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/* config agc register */
	agcRegister = (rxAgcCtrl->agcPreventGain<<0)|(rxAgcCtrl->agcGainControlMode<<1);
	halError = YXW_spiWriteField(hw_priv, REG_GC_01, agcRegister,2,1);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/* performing AGC peak wait time value check */
	if (rxAgcCtrl->agcPeakWaitCounter > 0x0F)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"Error:rxAgcCtrl parameter agcPeakWaitCounter exceeded\n");
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}
	else
	{
		/* write AGC peak wait time */
	   	halError = YXW_spiWriteField(hw_priv,REG_GC_01,rxAgcCtrl->agcPeakWaitCounter,7,4);
	  	yxwRetAct = YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	if(rxAgcCtrl->agcGainUpdateCounter>0x7FFF)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"Error:rxAgcCtrl parameter agcGainUpdateCounter exceeded\n");
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}else{
		/* performing range check for gain update time */
		halError = YXW_spiWriteField(hw_priv, REG_GC_01, rxAgcCtrl->agcGainUpdateCounter,22,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	/* If peak detect data structure is not included into project */
	if(rxAgcCtrl->agcPeak.apdEnableMeasurement){

		/* APD Configuration */
		halError = YXW_spiWriteField(hw_priv, REG_RXFE3, rxAgcCtrl->agcPeak.apdHighTresh,9,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_RXFE3, rxAgcCtrl->agcPeak.apdLowTresh,15,10);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_RXFE11, rxAgcCtrl->agcPeak.apdHighTresh,9,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_RXFE11, rxAgcCtrl->agcPeak.apdLowTresh,15,10);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_07, 0x0,8,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_07, rxAgcCtrl->agcPeak.apdAttackMode,0,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_07, rxAgcCtrl->agcPeak.apdMeasurementDuration,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_07, rxAgcCtrl->agcPeak.apdGainStepAttack,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_07, rxAgcCtrl->agcPeak.apdGainStepRecovery,19,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_08, rxAgcCtrl->agcPeak.apdImmdOverNum,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_08, rxAgcCtrl->agcPeak.apdPeriOverNum,19,10);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_08, rxAgcCtrl->agcPeak.apdUnderNum,29,20);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	if(rxAgcCtrl->agcPeak.hb2EnableMeasurement){
		/* HB2 Configuration */
		halError = YXW_spiWriteField(hw_priv, REG_GC_09, 0x0,4,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_09, rxAgcCtrl->agcPeak.hb2MeasurementDuration,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_09, rxAgcCtrl->agcPeak.hb2RecoveryMode,5,5);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_09, rxAgcCtrl->agcPeak.hb2AttackMode,6,6);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_10, rxAgcCtrl->agcPeak.hb2GainStepHighRecovery,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_10, rxAgcCtrl->agcPeak.hb2GainStepLowRecovery,11,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_10, rxAgcCtrl->agcPeak.hb2GainStepMidRecovery,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_10, rxAgcCtrl->agcPeak.hb2GainStepAttack,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_11, rxAgcCtrl->agcPeak.hb2OverThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_11, rxAgcCtrl->agcPeak.hb2UnderRangeHighThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_12, rxAgcCtrl->agcPeak.hb2UnderRangeLowThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_12, rxAgcCtrl->agcPeak.hb2UnderRangeMidThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_13, rxAgcCtrl->agcPeak.hb2OverloadThreshCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_13, rxAgcCtrl->agcPeak.hb2UnderRangeHighThreshCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_14, rxAgcCtrl->agcPeak.hb2UnderRangeLowThreshCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_14, rxAgcCtrl->agcPeak.hb2UnderRangeMidThreshCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_15, rxAgcCtrl->agcPeak.hb2PeriExceededCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_15, rxAgcCtrl->agcPeak.hb2PeriUnderExceededCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_16, rxAgcCtrl->agcPeak.hb2ImmdExceededCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_16, rxAgcCtrl->agcPeak.hb2ImmdUnderHigCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_17, rxAgcCtrl->agcPeak.hb2ImmdUnderLowCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_17, rxAgcCtrl->agcPeak.hb2ImmdUnderMidCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	if(rxAgcCtrl->agcPower.powerEnableMeasurement){
		/* Power Configuration register */
		halError = YXW_spiWriteField(hw_priv, REG_GC_18, 0,4,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_18, rxAgcCtrl->agcPower.powerInMux,9,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_18, rxAgcCtrl->agcPower.powerMeasurementDuration,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_19, rxAgcCtrl->agcPower.powerAttackHigStep,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_19, rxAgcCtrl->agcPower.powerAttackLowStep,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_19, rxAgcCtrl->agcPower.powerRecoveryHigStep,11,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_19, rxAgcCtrl->agcPower.powerRecoveryLowStep,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_20, rxAgcCtrl->agcPower.powerOverHigThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_20, rxAgcCtrl->agcPower.powerOverLowThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_21, rxAgcCtrl->agcPower.powerUnderHigThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_21, rxAgcCtrl->agcPower.powerUnderLowThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	/*Enable AGC Mode*/
	halError = YXW_spiWriteField(hw_priv, REG_GC_01,0x1,0,0);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_setupOrxAgc(YxwDevice_t *hw_priv, YxwAgcCfg_t *orxAgcCtrl)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	yxwHalErr_t halError = YXWHAL_OK;
	uint32_t agcRegister = 0;
	uint32_t value = 0;

	FUNCTION_ENTER(hw_priv);
	/*Disable orx0/orx1 analog gain ctrl external mode*/
	/*And use AGC module*/
	YXW_spiWriteField(hw_priv,REG_GC_50,0x0,5,4);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halError=YXW_spiReadReg(hw_priv,REG_GC_00,&value);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/* checking for valid YxwAgcCfg_t* orxAgcCtrl pointer to determine if it has been initialized */
	if (orxAgcCtrl == NULL)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"Error:orxAgcCtrl parameter is NULL\n");
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}
	/*set ORX AGC Clock Division Ratio*/
	halError = YXW_spiWriteField(hw_priv, REG_GC_00, orxAgcCtrl->agcClkDiv, 3, 2);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/* config agc register */
	agcRegister = (orxAgcCtrl->agcPreventGain<<0)|(orxAgcCtrl->agcGainControlMode<<1);
	halError = YXW_spiWriteField(hw_priv, REG_GC_03, agcRegister,2,1);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/* performing AGC peak wait time value check */
	if (orxAgcCtrl->agcPeakWaitCounter > 0x0F)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"Error:orxAgcCtrl parameter agcPeakWaitCounter exceeded\n");
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}
	else
	{
		/* write AGC peak wait time */
		halError = YXW_spiWriteField(hw_priv,REG_GC_03,orxAgcCtrl->agcPeakWaitCounter,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	if(orxAgcCtrl->agcGainUpdateCounter>0x7FFF)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"Error:orxAgcCtrl parameter agcGainUpdateCounter exceeded\n");
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}else{
		/* performing range check for gain update time */
		halError = YXW_spiWriteField(hw_priv, REG_GC_03, orxAgcCtrl->agcGainUpdateCounter,22,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	/* If peak detect data structure is not included into project */
	if(orxAgcCtrl->agcPeak.apdEnableMeasurement){

		/* APD Configuration */
		halError = YXW_spiWriteField(hw_priv, REG_RXFE3, orxAgcCtrl->agcPeak.apdHighTresh,9,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_RXFE3, orxAgcCtrl->agcPeak.apdLowTresh,15,10);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_RXFE11, orxAgcCtrl->agcPeak.apdHighTresh,9,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_RXFE11, orxAgcCtrl->agcPeak.apdLowTresh,15,10);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_22, 0x0,8,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_22, orxAgcCtrl->agcPeak.apdAttackMode,0,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_22, orxAgcCtrl->agcPeak.apdMeasurementDuration,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_22, orxAgcCtrl->agcPeak.apdGainStepAttack,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_22, orxAgcCtrl->agcPeak.apdGainStepRecovery,19,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_23, orxAgcCtrl->agcPeak.apdImmdOverNum,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_23, orxAgcCtrl->agcPeak.apdPeriOverNum,19,10);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_23, orxAgcCtrl->agcPeak.apdUnderNum,29,20);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	if(orxAgcCtrl->agcPeak.hb2EnableMeasurement){
		/* HB2 Configuration */
		halError = YXW_spiWriteField(hw_priv, REG_GC_24, 0x0,4,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_24, orxAgcCtrl->agcPeak.hb2MeasurementDuration,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_24, orxAgcCtrl->agcPeak.hb2RecoveryMode,5,5);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_24, orxAgcCtrl->agcPeak.hb2AttackMode,6,6);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_25, orxAgcCtrl->agcPeak.hb2GainStepHighRecovery,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_25, orxAgcCtrl->agcPeak.hb2GainStepLowRecovery,11,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_25, orxAgcCtrl->agcPeak.hb2GainStepMidRecovery,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_25, orxAgcCtrl->agcPeak.hb2GainStepAttack,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_26, orxAgcCtrl->agcPeak.hb2OverThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_26, orxAgcCtrl->agcPeak.hb2UnderRangeHighThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_27, orxAgcCtrl->agcPeak.hb2UnderRangeLowThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_27, orxAgcCtrl->agcPeak.hb2UnderRangeMidThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_28, orxAgcCtrl->agcPeak.hb2OverloadThreshCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_28, orxAgcCtrl->agcPeak.hb2UnderRangeHighThreshCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_29, orxAgcCtrl->agcPeak.hb2UnderRangeLowThreshCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_29, orxAgcCtrl->agcPeak.hb2UnderRangeMidThreshCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_30, orxAgcCtrl->agcPeak.hb2PeriExceededCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_30, orxAgcCtrl->agcPeak.hb2PeriUnderExceededCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_31, orxAgcCtrl->agcPeak.hb2ImmdExceededCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_31, orxAgcCtrl->agcPeak.hb2ImmdUnderHigCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_32, orxAgcCtrl->agcPeak.hb2ImmdUnderLowCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_32, orxAgcCtrl->agcPeak.hb2ImmdUnderMidCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	if(orxAgcCtrl->agcPower.powerEnableMeasurement){
		/* Power Configuration register */
		halError = YXW_spiWriteField(hw_priv, REG_GC_33, 0,4,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_33, orxAgcCtrl->agcPower.powerInMux,9,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_33, orxAgcCtrl->agcPower.powerMeasurementDuration,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_34, orxAgcCtrl->agcPower.powerAttackHigStep,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_34, orxAgcCtrl->agcPower.powerAttackLowStep,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_34, orxAgcCtrl->agcPower.powerRecoveryHigStep,11,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_34, orxAgcCtrl->agcPower.powerRecoveryLowStep,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_35, orxAgcCtrl->agcPower.powerOverHigThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_35, orxAgcCtrl->agcPower.powerOverLowThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_36, orxAgcCtrl->agcPower.powerUnderHigThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);

		halError = YXW_spiWriteField(hw_priv, REG_GC_36, orxAgcCtrl->agcPower.powerUnderLowThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	/*Enable AGC Mode*/
	halError = YXW_spiWriteField(hw_priv, REG_GC_03,0x1,0,0);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_SetAgcMaxIndex(YxwDevice_t *hw_priv,YxwRxChannels_t rxChannel, uint32_t maxIndex)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError = YXWHAL_OK;
	YxwData DataInfo;
	uint32_t rxValue = 0;
	YxwCmdTx *pMsg=NULL;
	DataInfo.CmdType=0xff;
	pMsg=&DataInfo.u.Tx;
	pMsg->Cmd=YXW_SET_OPCODE;

	FUNCTION_ENTER(hw_priv);
	switch(rxChannel){
		case YXW_RX1:
			rxValue = 0x4;
			hw_priv->YxwState.gainIndexes.rx1MaxGainIndex=maxIndex;
			break;
		case YXW_RX2:
			rxValue = 0x8;
			hw_priv->YxwState.gainIndexes.rx2MaxGainIndex=maxIndex;
			break;
		default:
			YXW_DEBUG(DBG_LEVEL_ERR,"Error:YXW_SetAgcMaxIndex parameter is error.\n");
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	pMsg->TxData.value=(((rxValue &0xff)<<24)|((maxIndex &0xff)<<16)|(HI_SET_AGC_MAX_VALUE));
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

uint32_t YXW_GetAgcMaxIndex(YxwDevice_t *hw_priv,YxwRxChannels_t rxChannel, uint32_t *maxIndex)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError = YXWHAL_OK;

	switch(rxChannel){
		case YXW_RX1:
			*maxIndex=hw_priv->YxwState.gainIndexes.rx1MaxGainIndex;
			break;
		case YXW_RX2:
			*maxIndex=hw_priv->YxwState.gainIndexes.rx2MaxGainIndex;
			break;
		default:
			YXW_DEBUG(DBG_LEVEL_ERR,"Error:YXW_GetAgcMaxIndex parameter is error.\n");
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}
	return yxwRetAct;
}

uint32_t YXW_lp(YxwDevice_t *hw_priv, YxwInit_t *Init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	switch (Init->tx.txChannels)
	{
		case YXW_TXOFF:
			/*digital close TX1/TX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x0, 25, 24);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog close TX1/TX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0xF, 3, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0xF, 11, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0xF, 19, 16);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x3, 1, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX1:
			/*digital Enable TX1*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x1, 25, 24);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable TX1*/
			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x5, 3, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x5, 11, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x3, 19, 16);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x1, 1, 1);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX2:
			/*digital Enable TX1*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x2, 25, 24);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable TX1*/
			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0xA, 3, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0xA, 11, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0xC, 19, 16);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x1, 0, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX1TX2:
			/*digital Enable TX1/TX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x3, 25, 24);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable TX1/TX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 3, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 11, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 19, 16);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_SYNC_1, 0x0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}

	switch(Init->rx.rxChannels)
	{
		case YXW_RXOFF:
			/*digital close RX1/RX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x0, 27, 26);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog close RX1/RX2*/
			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE6, 0x3882bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE3, 0xc0123ffe);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE14, 0x3882bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE11, 0xc0123ffe);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x3, 13, 12);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x3, 5, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX1:
			/*digital Enable RX1*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x0, 27, 27);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable RX1*/
			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE0, 0xe02000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE6, 0x10082bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE3, 0x40123ffe);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE8, 0x80732000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE14, 0x3882bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE11, 0xc0123ffe);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x1, 12, 12);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x1, 5, 5);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX2:
			/*digital Enable RX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x0, 26, 26);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable RX2*/
			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE0, 0x80732000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE6, 0x13882bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE3, 0xc0123ffe);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE8, 0xe02000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE14, 0x10082bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE11, 0x40123ffe);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x1, 13, 13);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x1, 4, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX1RX2:
			/*digital Enable RX1/RX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x3, 27, 26);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable RX1/RX2*/
			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE0, 0xe02000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE6, 0x10082bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE3, 0x40123ffe);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE8, 0xe02000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE14, 0x10082bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE11, 0x40123ffe);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 13, 12);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_SYNC_1, 0x0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}

	switch(Init->obsRx.obsRxChannelsEnable){
		case YXW_ORXOFF:
			/*digital close ORX1/ORX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x0, 28, 28);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog close ORX1/ORX2*/
			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE5, 0x13882bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_1, 0xC0d7);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_2, 0xC0d70000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE4, 0x400c);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE13, 0x13882bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_5, 0xC0d7);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_6, 0xC0d70000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE12, 0x400C);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_BBPLL4, 0x0, 20, 20);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX1:
			/*digital Enable ORX1*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x1, 28, 28);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable ORX1*/
			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE5, 0x10082bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_1, 0x40d7);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_2, 0x40d70000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE4, 0x4000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE13, 0x13882bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_5, 0xC0d7);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_6, 0xC0d70000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE12, 0x400C);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x1, 14, 14);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX2:
			/*digital Enable ORX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x0, 28, 28);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable ORX2*/
			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE5, 0x13882bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_1, 0xC0d7);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_2, 0xC0d70000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE4, 0x400c);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE13, 0x10082bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_5, 0x40d7);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_6, 0x40d70000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE12, 0x4000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x1, 15, 15);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX1ORX2:
			/*digital Enable ORX1/ORX2*/
			halStatus=YXW_spiWriteField(hw_priv, REG_CLK_SWAP_1, 0x1, 28, 28);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*analog Enable ORX1/ORX2*/
			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE5, 0x10082bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_1, 0x40d7);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_2, 0x40d70000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE4, 0x4000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE13, 0x10082bf);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_5, 0x40d7);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXLPF_6, 0x40d70000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteReg(hw_priv, REG_RXFE12, 0x4000);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 15, 14);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halStatus=YXW_spiWriteField(hw_priv, REG_BBPLL4, 0x1, 20, 20);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}
	#if 0
	 if(Init->mode == 0)//close AUXPLL
	
	 {
	 	halStatus=YXW_spiWriteReg(hw_priv, REG_AUXPLL2, 0xc0752650);
	 	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	 	IF_ERR_RETURN(yxwRetAct);

	 	halStatus=YXW_spiWriteReg(hw_priv, REG_AUXPLL3, 0xc27026b0);
	 	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	 	IF_ERR_RETURN(yxwRetAct);

	 	halStatus=YXW_spiWriteReg(hw_priv, REG_AUXPLL7, 0x41bf);
	 	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	 	IF_ERR_RETURN(yxwRetAct);

	 	halStatus=YXW_spiWriteReg(hw_priv, REG_AUXPLL_LO, 0x2e821);
	 	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	 	IF_ERR_RETURN(yxwRetAct);
	 }
#endif
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
