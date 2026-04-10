/**
* \file yxw_info.c
* \brief Contains Show Yxw20 Chip work status information API.
* \version: Ver 1.0
* \author :
* \date   : 2023/04/20 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#include "yxw_info.h"

#include <inttypes.h>

#include "yxw_agc_types.h"
#include "yxw_cals.h"
#include "yxw_error.h"
#include "yxw_gpio.h"
#include "yxw_jesd204b.h"
#include "yxw_plat_hal.h"
#include "yxw_radio.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"
#include "yxw_rx.h"
#include "yxw_tx.h"

uint32_t YXW_InfoShow_rx(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");

	YXWHAL_MDelay(100);

	yxwRetAct=YXW_getRadioInfo(hw_priv);
	IF_ERR_RETURN(yxwRetAct);
	YXWHAL_MDelay(100);

	yxwRetAct=YXW_getPathGainInfo(hw_priv);
	IF_ERR_RETURN(yxwRetAct);
	YXWHAL_MDelay(100);
	yxwRetAct=YXW_getJesd204bInfo(hw_priv);
	IF_ERR_RETURN(yxwRetAct);
	YXWHAL_MDelay(100);
	//yxwRetAct=YXW_getAgcInfo(hw_priv);
	//IF_ERR_RETURN(yxwRetAct);
	//YXWHAL_MDelay(100);

	//yxwRetAct=YXW_getGpioInfo(hw_priv);
	//IF_ERR_RETURN(yxwRetAct);
	//YXWHAL_MDelay(100);
	//yxwRetAct=YXW_getWaitTimeoutBit(hw_priv);
	//IF_ERR_RETURN(yxwRetAct);
	//YXWHAL_MDelay(100);
	//yxwRetAct=YXW_getSdkVersion();
	//IF_ERR_RETURN(yxwRetAct);
	//YXWHAL_MDelay(100);
	//yxwRetAct=YXW_getFirmwareVersion(hw_priv);
    //IF_ERR_RETURN(yxwRetAct);

	return yxwRetAct;
}
uint32_t YXW_InfoShow_tx(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");

	YXWHAL_MDelay(100);

	yxwRetAct=YXW_getRadioInfo(hw_priv);
	IF_ERR_RETURN(yxwRetAct);
	YXWHAL_MDelay(100);

	yxwRetAct=YXW_getPathGainInfo(hw_priv);
	IF_ERR_RETURN(yxwRetAct);
	YXWHAL_MDelay(100);
	yxwRetAct=YXW_getJesd204bInfo(hw_priv);
	IF_ERR_RETURN(yxwRetAct);
	YXWHAL_MDelay(100);
	//yxwRetAct=YXW_getAgcInfo(hw_priv);
	//IF_ERR_RETURN(yxwRetAct);
	//YXWHAL_MDelay(100);

	//yxwRetAct=YXW_getGpioInfo(hw_priv);
	//IF_ERR_RETURN(yxwRetAct);
	//YXWHAL_MDelay(100);
	//yxwRetAct=YXW_getWaitTimeoutBit(hw_priv);
	//IF_ERR_RETURN(yxwRetAct);
	//YXWHAL_MDelay(100);
	//yxwRetAct=YXW_getSdkVersion();
	//IF_ERR_RETURN(yxwRetAct);
	//YXWHAL_MDelay(100);
	//yxwRetAct=YXW_getFirmwareVersion(hw_priv);
    //IF_ERR_RETURN(yxwRetAct);

	return yxwRetAct;
}

uint32_t YXW_getPathGainInfo(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint32_t tx1_index = 0;
	uint32_t tx2_index = 0;
	uint32_t rx1_index = 0;
	uint32_t rx2_index = 0;
	uint32_t orx1_index = 0;
	uint32_t orx2_index = 0;

	YXW_DEBUG(DBG_LEVEL_ALWAYS, "|---------------------PathGainInfoshow---------------------|\t\n");

	yxwRetAct = YXW_getTxGain(hw_priv, YXW_TX1, &tx1_index);
	IF_ERR_RETURN(yxwRetAct);
	yxwRetAct = YXW_getTxGain(hw_priv, YXW_TX2, &tx2_index);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct = YXW_getRxGain(hw_priv, YXW_RX1, &rx1_index);
	IF_ERR_RETURN(yxwRetAct);
	yxwRetAct = YXW_getRxGain(hw_priv, YXW_RX2, &rx2_index);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct = YXW_getObsRxGain(hw_priv, YXW_ORX1, &orx1_index);
	IF_ERR_RETURN(yxwRetAct);
	yxwRetAct = YXW_getObsRxGain(hw_priv, YXW_ORX2, &orx2_index);
	IF_ERR_RETURN(yxwRetAct);

	YXW_DEBUG(DBG_LEVEL_ALWAYS, "GainPath\tTX1\tTX2\tRX1\tRX2\tORX1\tORX2\t\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS, "GainIndex\t%d\t%d\t%d\t%d\t%d\t%d\t\n", tx1_index, tx2_index,
			  rx1_index, rx2_index, orx1_index, orx2_index);

	YXW_DEBUG(DBG_LEVEL_ALWAYS, "\n");

	return (uint32_t)yxwRetAct;
}



uint32_t YXW_getPllInfo(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;
	uint8_t RFPLL_LOCKED = 0;
	uint8_t BBPLL_LOCKED = 0;
	uint8_t AUXPLL_LOCKED = 0;
	uint32_t radioStatus = 0;
	uint64_t rfPllLoFreq_Hz = 0;
	uint64_t auxPllLoFreq_Hz = 0;
	sint8_t *pllStatus[2] = {"\033[1;31;40munlock\033[0m", "\033[1;32;40mlocked\033[0m"};

	halStatus = YXW_spiReadReg(hw_priv, REGOUT_CALI_DONE_0, &radioStatus);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_TX, halStatus, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if ((radioStatus & RFPLLMASK) == RFPLLMASK)
	{
		RFPLL_LOCKED = 1;
	}
	if ((radioStatus & BBPLLMASK) == BBPLLMASK)
	{
		BBPLL_LOCKED = 1;
	}
	if ((radioStatus & AUXPLLMASK) == AUXPLLMASK)
	{
		AUXPLL_LOCKED = 1;
	}

	yxwRetAct = YXW_getPllFrequency(hw_priv, YXW_RF_PLL, &rfPllLoFreq_Hz);
	IF_ERR_RETURN(yxwRetAct);
	yxwRetAct = YXW_getPllFrequency(hw_priv, YXW_AUX_PLL, &auxPllLoFreq_Hz);
	IF_ERR_RETURN(yxwRetAct);

	YXW_DEBUG(DBG_LEVEL_ALWAYS, "|---------------------PLLInfoshow---------------------|\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS, "PLL          RFPLL          AUXPLL           BBPPLL   \n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS, "Status       %s         %s           %s\n",
			  pllStatus[RFPLL_LOCKED], pllStatus[AUXPLL_LOCKED], pllStatus[BBPLL_LOCKED]);
	YXW_DEBUG(DBG_LEVEL_ALWAYS, "Freq         %" PRIu64 "     %" PRIu64 "         -\n", rfPllLoFreq_Hz, auxPllLoFreq_Hz);

	YXW_DEBUG(DBG_LEVEL_ALWAYS, "\n");

	return (uint32_t)yxwRetAct;
}



uint32_t YXW_getRadioInfo(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t errorFlags = 0;

	yxwRetAct=YXW_getRadioState(hw_priv);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_getPllInfo(hw_priv);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_getPaProtectErrorFlags(hw_priv, &errorFlags);
	IF_ERR_RETURN(yxwRetAct);

	return yxwRetAct;
}


uint32_t YXW_getAgcInfo(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError = YXWHAL_OK;
	YxwGainMode_t rx_gain_ctrl_mode = YXW_MGC;
	sint8_t *gainModeStatus[2]   = {"\033[1;31;40mMGC\033[0m" , "\033[1;32;40mAGC\033[0m"};
	YxwAgcCfg_t rxAgcCtrl;
	YXW_Memset(&rxAgcCtrl, 0, sizeof(rxAgcCtrl));

	/*Get Rx Gain Ctrl Mode*/
	halError = YXW_spiReadField(hw_priv, REG_GC_01,&rx_gain_ctrl_mode,0,0);
	yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"|--------------------AgcInfoshow----------------------|\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"RxGainCtrlMode: %s\n", gainModeStatus[rx_gain_ctrl_mode]);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"YxwRX1MaxIndex:  %d, YxwRX2MaxIndex:  %d\n", hw_priv->YxwState.gainIndexes.rx1MaxGainIndex, hw_priv->YxwState.gainIndexes.rx2MaxGainIndex);
	if(YXW_AGC == rx_gain_ctrl_mode)
	{
		{
				sint8_t *isDetectApdOrHpd[2] = {"YES" , "NO"};
				sint8_t *agcDetectMode[2]    = {"PEAK_MODE " , "POWER_MODE"};
				sint8_t *agcClock[4]         = {"500M" , "250M", "125M", "125M"};

				/*set RX AGC Clock Division Ratio*/
				halError = YXW_spiReadField(hw_priv, REG_GC_00, (uint32_t*)&rxAgcCtrl.agcClkDiv, 1, 0);
				yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				/* config agc register */
				halError = YXW_spiReadField(hw_priv, REG_GC_01, (uint32_t*)&rxAgcCtrl.agcPreventGain,1,1);
				yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halError = YXW_spiReadField(hw_priv, REG_GC_01, (uint32_t*)&rxAgcCtrl.agcGainControlMode,2,2);
				yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				/* write AGC peak wait time */
			   	halError = YXW_spiReadField(hw_priv,REG_GC_01,(uint32_t*)&rxAgcCtrl.agcPeakWaitCounter,7,4);
			  	yxwRetAct = YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_AGC,halError,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halError = YXW_spiReadField(hw_priv, REG_GC_01, (uint32_t*)&rxAgcCtrl.agcGainUpdateCounter,22,8);
				yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
				YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcTotal--AgcClock: %s\n", agcClock[rxAgcCtrl.agcClkDiv]);
				YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcTotal--AgcPeakThreshGainControlMode: %s\n", agcDetectMode[rxAgcCtrl.agcGainControlMode]);
				YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcTotal--IsDetectApdOrHpd(PowerMode): %s\n", isDetectApdOrHpd[rxAgcCtrl.agcPreventGain]);
				YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcTotal--AgcPeakWaitCounter: %d\n", rxAgcCtrl.agcPeakWaitCounter);
				YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcTotal--AgcGainUpdateCounter: %d\n", rxAgcCtrl.agcGainUpdateCounter);
		}

		YXW_DEBUG(DBG_LEVEL_ALWAYS,"|----------------AgcPeakMode--APD--Infoshow-----------|\n");
		/* APD Configuration Info*/
		halError = YXW_spiReadField(hw_priv, REG_RXFE3, (uint32_t*)&rxAgcCtrl.agcPeak.apdHighTresh,9,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--HighTresh: %d\n", rxAgcCtrl.agcPeak.apdHighTresh);

		halError = YXW_spiReadField(hw_priv, REG_RXFE3, (uint32_t*)&rxAgcCtrl.agcPeak.apdLowTresh,15,10);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--LowTresh: %d\n", rxAgcCtrl.agcPeak.apdLowTresh);

		halError = YXW_spiReadField(hw_priv, REG_GC_07, (uint32_t*)&rxAgcCtrl.agcPeak.apdAttackMode,0,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--AttackMode: %d\n", rxAgcCtrl.agcPeak.apdAttackMode);

		halError = YXW_spiReadField(hw_priv, REG_GC_07, (uint32_t*)&rxAgcCtrl.agcPeak.apdMeasurementDuration,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--MeasurementDuration: %d\n", rxAgcCtrl.agcPeak.apdMeasurementDuration);

		halError = YXW_spiReadField(hw_priv, REG_GC_07, (uint32_t*)&rxAgcCtrl.agcPeak.apdGainStepAttack,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--GainStepAttack: %d\n", rxAgcCtrl.agcPeak.apdGainStepAttack);

		halError = YXW_spiReadField(hw_priv, REG_GC_07, (uint32_t*)&rxAgcCtrl.agcPeak.apdGainStepRecovery,19,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--GainStepRecovery: %d\n", rxAgcCtrl.agcPeak.apdGainStepRecovery);

		halError = YXW_spiReadField(hw_priv, REG_GC_08, (uint32_t*)&rxAgcCtrl.agcPeak.apdImmdOverNum,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--ImmdOverNum: %d\n", rxAgcCtrl.agcPeak.apdImmdOverNum);

		halError = YXW_spiReadField(hw_priv, REG_GC_08, (uint32_t*)&rxAgcCtrl.agcPeak.apdPeriOverNum,19,10);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--PeriOverNum: %d\n", rxAgcCtrl.agcPeak.apdPeriOverNum);

		halError = YXW_spiReadField(hw_priv, REG_GC_08, (uint32_t*)&rxAgcCtrl.agcPeak.apdUnderNum,29,20);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--APD--UnderNum: %d\n", rxAgcCtrl.agcPeak.apdUnderNum);


		YXW_DEBUG(DBG_LEVEL_ALWAYS,"|----------------AgcPeakMode--HB2--Infoshow-----------|\n");
		halError = YXW_spiReadField(hw_priv, REG_GC_09, (uint32_t*)&rxAgcCtrl.agcPeak.hb2MeasurementDuration,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--MeasurementDuration: %d\n", rxAgcCtrl.agcPeak.hb2MeasurementDuration);

		halError = YXW_spiReadField(hw_priv, REG_GC_09, (uint32_t*)&rxAgcCtrl.agcPeak.hb2RecoveryMode,5,5);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--RecoveryMode: %d\n", rxAgcCtrl.agcPeak.hb2RecoveryMode);

		halError = YXW_spiReadField(hw_priv, REG_GC_09, (uint32_t*)&rxAgcCtrl.agcPeak.hb2AttackMode,6,6);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--AttackMode: %d\n", rxAgcCtrl.agcPeak.hb2AttackMode);

		halError = YXW_spiReadField(hw_priv, REG_GC_10, (uint32_t*)&rxAgcCtrl.agcPeak.hb2GainStepHighRecovery,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--GainStepHighRecovery: %d\n", rxAgcCtrl.agcPeak.hb2GainStepHighRecovery);

		halError = YXW_spiReadField(hw_priv, REG_GC_10, (uint32_t*)&rxAgcCtrl.agcPeak.hb2GainStepLowRecovery,11,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--GainStepLowRecovery: %d\n", rxAgcCtrl.agcPeak.hb2GainStepLowRecovery);

		halError = YXW_spiReadField(hw_priv, REG_GC_10, (uint32_t*)&rxAgcCtrl.agcPeak.hb2GainStepMidRecovery,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--GainStepMidRecovery: %d\n", rxAgcCtrl.agcPeak.hb2GainStepMidRecovery);

		halError = YXW_spiReadField(hw_priv, REG_GC_10, (uint32_t*)&rxAgcCtrl.agcPeak.hb2GainStepAttack,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--GainStepAttack: %d\n", rxAgcCtrl.agcPeak.hb2GainStepAttack);

		halError = YXW_spiReadField(hw_priv, REG_GC_11, (uint32_t*)&rxAgcCtrl.agcPeak.hb2OverThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--OverThresh: %d\n", rxAgcCtrl.agcPeak.hb2OverThresh);

		halError = YXW_spiReadField(hw_priv, REG_GC_11, (uint32_t*)&rxAgcCtrl.agcPeak.hb2UnderRangeHighThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--UnderRangeHighThresh: %d\n", rxAgcCtrl.agcPeak.hb2UnderRangeHighThresh);

		halError = YXW_spiReadField(hw_priv, REG_GC_12, (uint32_t*)&rxAgcCtrl.agcPeak.hb2UnderRangeLowThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--UnderRangeLowThresh: %d\n", rxAgcCtrl.agcPeak.hb2UnderRangeLowThresh);

		halError = YXW_spiReadField(hw_priv, REG_GC_12, (uint32_t*)&rxAgcCtrl.agcPeak.hb2UnderRangeMidThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--UnderRangeMidThresh: %d\n", rxAgcCtrl.agcPeak.hb2UnderRangeMidThresh);

		halError = YXW_spiReadField(hw_priv, REG_GC_13, (uint32_t*)&rxAgcCtrl.agcPeak.hb2OverloadThreshCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--OverloadThreshCnt: %d\n", rxAgcCtrl.agcPeak.hb2OverloadThreshCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_13, (uint32_t*)&rxAgcCtrl.agcPeak.hb2UnderRangeHighThreshCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--UnderRangeHighThreshCnt: %d\n", rxAgcCtrl.agcPeak.hb2UnderRangeHighThreshCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_14, (uint32_t*)&rxAgcCtrl.agcPeak.hb2UnderRangeLowThreshCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--UnderRangeLowThreshCnt: %d\n", rxAgcCtrl.agcPeak.hb2UnderRangeLowThreshCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_14, (uint32_t*)&rxAgcCtrl.agcPeak.hb2UnderRangeMidThreshCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--UnderRangeMidThreshCnt: %d\n", rxAgcCtrl.agcPeak.hb2UnderRangeMidThreshCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_15, (uint32_t*)&rxAgcCtrl.agcPeak.hb2PeriExceededCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--PeriExceededCnt: %d\n", rxAgcCtrl.agcPeak.hb2PeriExceededCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_15, (uint32_t*)&rxAgcCtrl.agcPeak.hb2PeriUnderExceededCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--PeriUnderExceededCnt: %d\n", rxAgcCtrl.agcPeak.hb2PeriUnderExceededCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_16, (uint32_t*)&rxAgcCtrl.agcPeak.hb2ImmdExceededCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--ImmdExceededCnt: %d\n", rxAgcCtrl.agcPeak.hb2ImmdExceededCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_16, (uint32_t*)&rxAgcCtrl.agcPeak.hb2ImmdUnderHigCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--ImmdUnderHigCnt: %d\n", rxAgcCtrl.agcPeak.hb2ImmdUnderHigCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_17, (uint32_t*)&rxAgcCtrl.agcPeak.hb2ImmdUnderLowCnt,9,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--ImmdUnderLowCnt: %d\n", rxAgcCtrl.agcPeak.hb2ImmdUnderLowCnt);

		halError = YXW_spiReadField(hw_priv, REG_GC_17, (uint32_t*)&rxAgcCtrl.agcPeak.hb2ImmdUnderMidCnt,25,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"AgcPeak--HB2--ImmdUnderMidCnt: %d\n", rxAgcCtrl.agcPeak.hb2ImmdUnderMidCnt);


		YXW_DEBUG(DBG_LEVEL_ALWAYS,"|-------------------AgcPowerInfoshow------------------|\n");
		halError = YXW_spiReadField(hw_priv, REG_GC_18, (uint32_t*)&rxAgcCtrl.agcPower.powerInMux,9,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--InMux: %d\n", rxAgcCtrl.agcPower.powerInMux);

		halError = YXW_spiReadField(hw_priv, REG_GC_18, (uint32_t*)&rxAgcCtrl.agcPower.powerMeasurementDuration,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--MeasurementDuration: %d\n", rxAgcCtrl.agcPower.powerMeasurementDuration);

		halError = YXW_spiReadField(hw_priv, REG_GC_19,(uint32_t*) &rxAgcCtrl.agcPower.powerAttackHigStep,3,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--AttackHigStep: %d\n", rxAgcCtrl.agcPower.powerAttackHigStep);

		halError = YXW_spiReadField(hw_priv, REG_GC_19, (uint32_t*)&rxAgcCtrl.agcPower.powerAttackLowStep,7,4);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--AttackLowStep: %d\n", rxAgcCtrl.agcPower.powerAttackLowStep);

		halError = YXW_spiReadField(hw_priv, REG_GC_19, (uint32_t*)&rxAgcCtrl.agcPower.powerRecoveryHigStep,11,8);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--RecoveryHigStep: %d\n", rxAgcCtrl.agcPower.powerRecoveryHigStep);

		halError = YXW_spiReadField(hw_priv, REG_GC_19, (uint32_t*)&rxAgcCtrl.agcPower.powerRecoveryLowStep,15,12);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--RecoveryLowStep: %d\n", rxAgcCtrl.agcPower.powerRecoveryLowStep);

		halError = YXW_spiReadField(hw_priv, REG_GC_20, (uint32_t*)&rxAgcCtrl.agcPower.powerOverHigThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--OverHigThresh: %d\n", rxAgcCtrl.agcPower.powerOverHigThresh);

		halError = YXW_spiReadField(hw_priv, REG_GC_20, (uint32_t*)&rxAgcCtrl.agcPower.powerOverLowThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--OverLowThresh: %d\n", rxAgcCtrl.agcPower.powerOverLowThresh);

		halError = YXW_spiReadField(hw_priv, REG_GC_21, (uint32_t*)&rxAgcCtrl.agcPower.powerUnderHigThresh,13,0);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--UnderHigThresh: %d\n", rxAgcCtrl.agcPower.powerUnderHigThresh);

		halError = YXW_spiReadField(hw_priv, REG_GC_21, (uint32_t*)&rxAgcCtrl.agcPower.powerUnderLowThresh,29,16);
		yxwRetAct = YXW_ApiErrHandler(hw_priv, YXW_ERRHDL_HAL_AGC, halError, yxwRetAct, YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"agcPower--UnderLowThresh: %d\n", rxAgcCtrl.agcPower.powerUnderLowThresh);
	}

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getCalsInfo(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError = YXWHAL_OK;
	uint32_t regValue=0;
	uint32_t userMode = 0;

	yxwRetAct = YXW_GetRiscvTraceEvent(hw_priv);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct = YXW_getInitCalsStatus(hw_priv);
	IF_ERR_RETURN(yxwRetAct);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"|-----------------CalsRegisterInfoshow----------------|\n");

	halError=YXW_spiReadReg(hw_priv,REG_CALI_CTRL_0,&regValue);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_INITCALS,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"0x%x=0x%x\n", REG_CALI_CTRL_0, regValue);

	halError=YXW_spiReadReg(hw_priv,REG_CALI_RXIQ0_CTRL_0,&regValue);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_INITCALS,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"0x%x=0x%x\n", REG_CALI_RXIQ0_CTRL_0, regValue);

	halError=YXW_spiReadReg(hw_priv,REG_EXT_INTR_ENA,&regValue);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_INITCALS,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"0x%x=0x%x\n", REG_EXT_INTR_ENA, regValue);

	halError=YXW_spiReadReg(hw_priv,REG_CALI_TXFLT_CTRL_0,&regValue);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_INITCALS,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"0x%x=0x%x\n", REG_CALI_TXFLT_CTRL_0, regValue);

	halError=YXW_spiReadReg(hw_priv,REG_CLK_SWAP_1,&regValue);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_INITCALS,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"0x%x=0x%x\n", REG_CLK_SWAP_1, regValue);

	halError=YXW_spiReadReg(hw_priv,REG_CH_ENA,&regValue);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_INITCALS,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"0x%x=0x%x\n", REG_CH_ENA, regValue);

	yxwRetAct = YXW_getWaitTimeoutBit(hw_priv);
	IF_ERR_RETURN(yxwRetAct);

	return yxwRetAct;
}


uint32_t YXW_getInitInfo(YxwDevice_t *hw_priv, YxwInit_t *Init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwInfo_t *pStateInfo = &hw_priv->YxwState;

	if(NULL == Init)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"%s,%d,Init point is NULL!\n",__FUNCTION__,__LINE__);
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
					YXW_ERR_DEVICE_INIT_NULL_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	/********************************************************************/
	/***************  TX Config related information *********************/
	/********************************************************************/
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"|----------------InitTXConfigInfoshow-----------------|\n");

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"TX_PROFILE_VALID=%d\n",(pStateInfo->profilesValid & TX_PROFILE_VALID));
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"txProfile.numFirCoefs=%d\n",Init->tx.txProfile.txFir.numFirCoefs);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"tx1MaxGainIndex=%d\n",Init->tx.txGainCtrl.tx1MaxGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"tx1MinGainIndex=%d\n",Init->tx.txGainCtrl.tx1MinGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"tx2MaxGainIndex=%d\n",Init->tx.txGainCtrl.tx2MaxGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"tx2MinGainIndex=%d\n",Init->tx.txGainCtrl.tx2MinGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set txChannels=%d\n",(Init->tx.txChannels & 3));
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set tx1GainIndex=%d\n",Init->tx.txGainCtrl.tx1GainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set tx2GainIndex=%d\n",Init->tx.txGainCtrl.tx2GainIndex);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set txgainMode=%d\n",Init->tx.txGainCtrl.txgainMode);
	if(YXW_MGC == Init->tx.txGainCtrl.txgainMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set txgainMode is: YXW_MGC\n");
	}
	else if(YXW_AGC == Init->tx.txGainCtrl.txgainMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set txgainMode is: YXW_AGC\n");
	}
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");


	/********************************************************************/
	/***************  RX Config related information *********************/
	/********************************************************************/
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"|----------------InitRXConfigInfoshow-----------------|\n");

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"RX_PROFILE_VALID=%d\n",(pStateInfo->profilesValid & RX_PROFILE_VALID));
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"rxProfile.numFirCoefs=%d\n",Init->rx.rxProfile.rxFir.numFirCoefs);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"rx1MaxGainIndex=%d\n",Init->rx.rxGainCtrl.rx1MaxGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"rx1MinGainIndex=%d\n",Init->rx.rxGainCtrl.rx1MinGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"rx2MaxGainIndex=%d\n",Init->rx.rxGainCtrl.rx2MaxGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"rx2MinGainIndex=%d\n",Init->rx.rxGainCtrl.rx2MinGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");


	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set rxChannels=%d\n",(Init->rx.rxChannels & 3));
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set rx1GainIndex=%d\n",Init->rx.rxGainCtrl.rx1GainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set rx2GainIndex=%d\n",Init->rx.rxGainCtrl.rx2GainIndex);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set rxgainMode=%d\n",Init->rx.rxGainCtrl.rxgainMode);
	if(YXW_MGC == Init->rx.rxGainCtrl.rxgainMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set rxgainMode is: YXW_MGC\n");
	}
	else if(YXW_AGC == Init->rx.rxGainCtrl.rxgainMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set rxgainMode is: YXW_AGC\n");
	}
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set RX YxwCtrlWay=%d\n",Init->rx.rxGainCtrl.rxmgcMode);
	if(YXW_APIMODE == Init->rx.rxGainCtrl.rxmgcMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set RX Gaincontrol ways is spi\n");
	}
	else if(YXW_GPIOMODE == Init->rx.rxGainCtrl.rxmgcMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set RX Gaincontrol ways is gpio\n");
	}
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");


	/********************************************************************/
	/***************  ORX Config related information ********************/
	/********************************************************************/
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"|----------------InitORXConfigInfoshow-----------------|\n");

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"ORX_PROFILE_VALID=%d\n",(pStateInfo->profilesValid & ORX_PROFILE_VALID));
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"orxProfile.numFirCoefs=%d\n",Init->obsRx.orxProfile.rxFir.numFirCoefs);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"orx1MaxGainIndex=%d\n",Init->obsRx.orxGainCtrl.orx1MaxGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"orx1MinGainIndex=%d\n",Init->obsRx.orxGainCtrl.orx1MinGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"orx2MaxGainIndex=%d\n",Init->obsRx.orxGainCtrl.orx2MaxGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"orx2MinGainIndex=%d\n",Init->obsRx.orxGainCtrl.orx2MinGainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set orxChannels=%d\n",(Init->obsRx.obsRxChannelsEnable & 3));
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set orx1GainIndex=%d\n",Init->obsRx.orxGainCtrl.orx1GainIndex);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set orx2GainIndex=%d\n",Init->obsRx.orxGainCtrl.orx2GainIndex);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set orxgainMode=%d\n",Init->obsRx.orxGainCtrl.orxgainMode);
	if(YXW_MGC == Init->obsRx.orxGainCtrl.orxgainMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set orxgainMode is: YXW_MGC\n");
	}
	else if(YXW_AGC == Init->obsRx.orxGainCtrl.orxgainMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set orxgainMode is: YXW_AGC\n");
	}

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set ORX YxwCtrlWay=%d\n",Init->obsRx.orxGainCtrl.orxmgcMode);
	if(YXW_APIMODE == Init->obsRx.orxGainCtrl.orxmgcMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set ORX Gaincontrol ways is spi\n");
	}
	else if(YXW_GPIOMODE == Init->obsRx.orxGainCtrl.orxmgcMode)
	{
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"yxw_config set ORX Gaincontrol ways is gpio\n");
	}

	return yxwRetAct;
}


uint32_t YXW_getJesd204bInfo(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint8_t txSyncStatus = 0;
	uint8_t rxSyncStatus = 0;
	uint8_t orxSyncStatus = 0;
	uint32_t regAddr = 0;
	uint32_t regValue = 0;
	uint32_t TypeStatus = 0;
	sint8_t *syncStatus[2] = {"\033[1;32;40mYES\033[0m", "\033[1;31;40mNO \033[0m"};
	sint8_t *enableStatus[2] = {"\033[1;31;40mOFF\033[0m", "\033[1;32;40mON \033[0m"};

	YXW_DEBUG(DBG_LEVEL_ALWAYS, "|-------------------Jesd204bInfoshow------------------|\n");

	yxwRetAct = YXW_getSyncStatus(hw_priv, &TypeStatus);
	IF_ERR_RETURN(yxwRetAct);

	if (TX_DEFRAMER_NOT_SYNC == (TypeStatus & TX_DEFRAMER_NOT_SYNC))
	{
		txSyncStatus++;
	}

	if (RX_FRAMER_NOT_SYNC == (TypeStatus & RX_FRAMER_NOT_SYNC))
	{
		rxSyncStatus++;
	}

	if (ORX_FRAMER_NOT_SYNC == (TypeStatus & ORX_FRAMER_NOT_SYNC))
	{
		orxSyncStatus++;
	}

	regAddr = JESD204B_TX_ENABLE_REG;
	yxwRetAct = YXW_spiReadMem(hw_priv, regAddr, &regValue);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS, "List         Satus          Sync\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS, "TX           %s            %s\n",enableStatus[regValue], syncStatus[txSyncStatus]);

	regAddr = JESD204B_RX_ENABLE_REG;
	yxwRetAct = YXW_spiReadMem(hw_priv, regAddr, &regValue);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS, "RX           %s            %s\n",enableStatus[regValue], syncStatus[rxSyncStatus]);

	regAddr = JESD204B_ORX_ENABLE_REG;
	yxwRetAct = YXW_spiReadMem(hw_priv, regAddr, &regValue);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS, "ORX          %s            %s\n",enableStatus[regValue], syncStatus[rxSyncStatus]);

	yxwRetAct = YXW_GetJes204bSysrefControlStatus(hw_priv);
	IF_ERR_RETURN(yxwRetAct);

	return yxwRetAct;
}

static sint8_t *gpioInputFuncSel_00[8]   = {"-","-","-","-","rx0_gain_ctrl_gpio_0","gpio_ext_port[0]","auxadcdata[0]","-"};
static sint8_t *gpioInputFuncSel_01[8]   = {"uart_sin","uart_sin","trstn","uart_sin","rx0_gain_ctrl_gpio_1","gpio_ext_port[1]","auxadcdata[1]","-"};
static sint8_t *gpioInputFuncSel_02[8]   = {"gpio_ext_port[2]","tck","tms","tck","-","gpio_ext_port[2]","auxadcdata[2]","-"};
static sint8_t *gpioInputFuncSel_03[8]   = {"gpio_ext_port[3]","trstn","tdi","trstn","rx1_gain_ctrl_gpio_0","gpio_ext_port[3]","auxadcdata[3]","-"};
static sint8_t *gpioInputFuncSel_04[8]   = {"-","tms","-","tms","rx1_gain_ctrl_gpio_1","gpio_ext_port[4]","auxadcdata[4]","-"};
static sint8_t *gpioInputFuncSel_05[8]   = {"-","tdi","-","tdi","orx_enable_0","gpio_ext_port[5]","auxadcdata[5]","-"};
static sint8_t *gpioInputFuncSel_06[8]   = {"-","gpio_ext_port[6]","-","-","orx_enable_1","gpio_ext_port[6]","auxadcdata[6]","-"};
static sint8_t *gpioInputFuncSel_07[8]   = {"mspi_sdi0","gpio_ext_port[7]","mspi_sdi0","-","uart_sin","gpio_ext_port[7]","auxadcdata[7]","-"};
static sint8_t *gpioInputFuncSel_08[8]   = {"-","rx0_gain_ctrl_gpio_0","rx0_gain_ctrl_gpio_0","-","slicer_in_tx0[0]","gpio_ext_port[8]","rx0_gain_ctrl_gpio_0","-"};
static sint8_t *gpioInputFuncSel_09[8]   = {"-","rx0_gain_ctrl_gpio_1","rx0_gain_ctrl_gpio_1","-","slicer_in_tx0[1]","gpio_ext_port[9]","rx0_gain_ctrl_gpio_1","-"};
static sint8_t *gpioInputFuncSel_10[8]   = {"-","rx1_gain_ctrl_gpio_0","rx1_gain_ctrl_gpio_0","-","slicer_in_tx0[2]","rx0_gain_ctrl_gpio_0","auxadcdata[8]","-"};
static sint8_t *gpioInputFuncSel_11[8]   = {"-","rx1_gain_ctrl_gpio_1","rx1_gain_ctrl_gpio_1","-","slicer_in_tx1[0]","rx0_gain_ctrl_gpio_1","auxadcdata[9]","-"};
static sint8_t *gpioInputFuncSel_12[8]   = {"rx0_gain_ctrl_gpio_0","-","-","-","slicer_in_tx1[1]","-","auxadcdata[10]","-"};
static sint8_t *gpioInputFuncSel_13[8]   = {"rx0_gain_ctrl_gpio_1","-","-","-","slicer_in_tx1[2]","rx1_gain_ctrl_gpio_0","auxadcdata[11]","-"};
static sint8_t *gpioInputFuncSel_14[8]   = {"rx1_gain_ctrl_gpio_0","-","-","-","-","rx1_gain_ctrl_gpio_1","rx1_gain_ctrl_gpio_0","-"};
static sint8_t *gpioInputFuncSel_15[8]   = {"rx1_gain_ctrl_gpio_1","-","-","-","-","orx_enable_0","rx1_gain_ctrl_gpio_1","-"};

static sint8_t *gpioOutputFuncSel_00[8]  = {"uart_sout","uart_sout","-","uart_sout","-","gpio_port_dr[0]","-","monitor_signal[0]"};
static sint8_t *gpioOutputFuncSel_01[8]  = {"-","-","-","-","-","gpio_port_dr[1]","-","monitor_signal[1]"};
static sint8_t *gpioOutputFuncSel_02[8]  = {"gpio_port_dr[2]","-","-","-","uart_sout","gpio_port_dr[2]","-","monitor_signal[2]"};
static sint8_t *gpioOutputFuncSel_03[8]  = {"gpio_port_dr[3]","-","-","-","-","gpio_port_dr[3]","-","monitor_signal[3]"};
static sint8_t *gpioOutputFuncSel_04[8]  = {"mspi_sclk","-","mspi_sclk","-","-","gpio_port_dr[4]","-","monitor_signal[4]"};
static sint8_t *gpioOutputFuncSel_05[8]  = {"mspi_csn0","-","mspi_csn0","-","-","gpio_port_dr[5]","-","monitor_signal[5]"};
static sint8_t *gpioOutputFuncSel_06[8]  = {"mspi_sdo0","gpio_port_dr[6]","mspi_sdo0","-","-","gpio_port_dr[6]","-","monitor_signal[6]"};
static sint8_t *gpioOutputFuncSel_07[8]  = {"-","gpio_port_dr[7]","-","slicer_bit_rx0[0]","-","gpio_port_dr[7]","-","monitor_signal[7]"};
static sint8_t *gpioOutputFuncSel_08[8]  = {"-","tdo","tdo","slicer_bit_rx0[1]","-","gpio_port_dr[8]","-","monitor_signal[8]"};
static sint8_t *gpioOutputFuncSel_09[8]  = {"-","tdo_oe","tdo_oe","slicer_bit_rx0[2]","-","gpio_port_dr[9]","-","monitor_signal[9]"};
static sint8_t *gpioOutputFuncSel_10[8]  = {"-","-","-","slicer_bit_rx1[0]","-","-","-","monitor_signal[10]"};
static sint8_t *gpioOutputFuncSel_11[8]  = {"-","-","-","slicer_bit_rx1[1]","-","-","-","monitor_signal[11]"};
static sint8_t *gpioOutputFuncSel_12[8]  = {"-","-","-","slicer_bit_rx1[2]","-","-","-","monitor_signal[12]"};
static sint8_t *gpioOutputFuncSel_13[8]  = {"-","-","-","slicer_bit_orx[0]","-","-","-","monitor_signal[13]"};
static sint8_t *gpioOutputFuncSel_14[8]  = {"-","-","-","slicer_bit_orx[1]","-","-","-","monitor_signal[14]"};
static sint8_t *gpioOutputFuncSel_15[8]  = {"-","-","-","slicer_bit_orx[2]","-","-","-","monitor_signal[15]"};

static uint32_t YXW_getGpioFuncSelString(YxwGpioPinSel_t gpioNum, YxwGpioDir_t gpioOutEn, YxwGpioPinFunc_t gpioFuncSel, sint8_t **gpioFuncSelString)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	if (YXW_GPIO_DIR_IN == gpioOutEn)
	{
		switch (gpioNum)
		{
		case YXW_GPIO_00:
			*gpioFuncSelString = gpioInputFuncSel_00[gpioFuncSel];
			break;
		case YXW_GPIO_01:
			*gpioFuncSelString = gpioInputFuncSel_01[gpioFuncSel];
			break;
		case YXW_GPIO_02:
			*gpioFuncSelString = gpioInputFuncSel_02[gpioFuncSel];
			break;
		case YXW_GPIO_03:
			*gpioFuncSelString = gpioInputFuncSel_03[gpioFuncSel];
			break;
		case YXW_GPIO_04:
			*gpioFuncSelString = gpioInputFuncSel_04[gpioFuncSel];
			break;
		case YXW_GPIO_05:
			*gpioFuncSelString = gpioInputFuncSel_05[gpioFuncSel];
			break;
		case YXW_GPIO_06:
			*gpioFuncSelString = gpioInputFuncSel_06[gpioFuncSel];
			break;
		case YXW_GPIO_07:
			*gpioFuncSelString = gpioInputFuncSel_07[gpioFuncSel];
			break;
		case YXW_GPIO_08:
			*gpioFuncSelString = gpioInputFuncSel_08[gpioFuncSel];
			break;
		case YXW_GPIO_09:
			*gpioFuncSelString = gpioInputFuncSel_09[gpioFuncSel];
			break;
		case YXW_GPIO_10:
			*gpioFuncSelString = gpioInputFuncSel_10[gpioFuncSel];
			break;
		case YXW_GPIO_11:
			*gpioFuncSelString = gpioInputFuncSel_11[gpioFuncSel];
			break;
		case YXW_GPIO_12:
			*gpioFuncSelString = gpioInputFuncSel_12[gpioFuncSel];
			break;
		case YXW_GPIO_13:
			*gpioFuncSelString = gpioInputFuncSel_13[gpioFuncSel];
			break;
		case YXW_GPIO_14:
			*gpioFuncSelString = gpioInputFuncSel_14[gpioFuncSel];
			break;
		case YXW_GPIO_15:
			*gpioFuncSelString = gpioInputFuncSel_15[gpioFuncSel];
			break;
		default:
			yxwRetAct = YXW_ERR_CHECK_PARAM;
			break;
		}
	}
	else
	{
		switch (gpioNum)
		{
		case YXW_GPIO_00:
			*gpioFuncSelString = gpioOutputFuncSel_00[gpioFuncSel];
			break;
		case YXW_GPIO_01:
			*gpioFuncSelString = gpioOutputFuncSel_01[gpioFuncSel];
			break;
		case YXW_GPIO_02:
			*gpioFuncSelString = gpioOutputFuncSel_02[gpioFuncSel];
			break;
		case YXW_GPIO_03:
			*gpioFuncSelString = gpioOutputFuncSel_03[gpioFuncSel];
			break;
		case YXW_GPIO_04:
			*gpioFuncSelString = gpioOutputFuncSel_04[gpioFuncSel];
			break;
		case YXW_GPIO_05:
			*gpioFuncSelString = gpioOutputFuncSel_05[gpioFuncSel];
			break;
		case YXW_GPIO_06:
			*gpioFuncSelString = gpioOutputFuncSel_06[gpioFuncSel];
			break;
		case YXW_GPIO_07:
			*gpioFuncSelString = gpioOutputFuncSel_07[gpioFuncSel];
			break;
		case YXW_GPIO_08:
			*gpioFuncSelString = gpioOutputFuncSel_08[gpioFuncSel];
			break;
		case YXW_GPIO_09:
			*gpioFuncSelString = gpioOutputFuncSel_09[gpioFuncSel];
			break;
		case YXW_GPIO_10:
			*gpioFuncSelString = gpioOutputFuncSel_10[gpioFuncSel];
			break;
		case YXW_GPIO_11:
			*gpioFuncSelString = gpioOutputFuncSel_11[gpioFuncSel];
			break;
		case YXW_GPIO_12:
			*gpioFuncSelString = gpioOutputFuncSel_12[gpioFuncSel];
			break;
		case YXW_GPIO_13:
			*gpioFuncSelString = gpioOutputFuncSel_13[gpioFuncSel];
			break;
		case YXW_GPIO_14:
			*gpioFuncSelString = gpioOutputFuncSel_14[gpioFuncSel];
			break;
		case YXW_GPIO_15:
			*gpioFuncSelString = gpioOutputFuncSel_15[gpioFuncSel];
			break;
		default:
			yxwRetAct = YXW_ERR_CHECK_PARAM;
			break;
		}
	}

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_getGpioInfo(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwGpioCtrlMode_t gpioCtrlMode = YXW_SPI_CTRL;
	YxwGpioDir_t gpioOutEn = YXW_GPIO_DIR_IN;
	YxwGpioPinFunc_t gpioFuncSel = YXW_SEL_3;
	sint8_t *gpioFuncSelString = NULL;
	uint32_t gpioNum=YXW_GPIO_00;
	sint8_t *gpioCtrlStatus[2]        = {"RISCV", " SPI "};
	sint8_t *gpioDirectionStatus[2]   = {"INPUT ", "OUTPUT"};

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"|--------------------GpioInfoshow---------------------|\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"List     Ctrl  Direction  Function\n");


	for(gpioNum = 0; gpioNum <= YXW_GPIO_15; gpioNum++)
	{
		yxwRetAct=YXW_getGpioSourceCtrl(hw_priv, &gpioCtrlMode, gpioNum);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct=YXW_getGpioDirection(hw_priv, &gpioOutEn, gpioNum);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct=YXW_getGpioFuncSel(hw_priv, &gpioFuncSel, gpioNum);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct=YXW_getGpioFuncSelString(gpioNum, gpioOutEn, gpioFuncSel, &gpioFuncSelString);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"GPIO_%02d  %s  %s    %s\n", gpioNum, gpioCtrlStatus[gpioCtrlMode], gpioDirectionStatus[gpioOutEn], gpioFuncSelString);
	}

	return yxwRetAct;
}

