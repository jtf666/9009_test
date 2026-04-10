/**
* \file yxw_init.c
* \brief Y9009 Init functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_init.h"

#include "yxw_error.h"
#include "yxw_plat_hal.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"
#include "yxw_rx.h"
#include "yxw_tx.h"


// uint32_t YXW_openHw(YxwDevice_t *hw_priv,uint32_t DeviceId)
// {
// 	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
// 	yxwHalErr_t halStatus = YXWHAL_OK;

// 	FUNCTION_ENTER(hw_priv);
// 	/*inital Spi device*/
// 	halStatus=YXW_spiOpen(hw_priv,DeviceId);

// 	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halStatus,yxwRetAct,YXW_ERR_INIT_PLAT);
// 	IF_ERR_RETURN(yxwRetAct);

// 	FUNCTION_EXIT(hw_priv);

// 	return (uint32_t)yxwRetAct;
// }
// uint32_t YXW_closeHw(YxwDevice_t *hw_priv)
// {
// 	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
// 	yxwHalErr_t halStatus = YXWHAL_OK;

// 	halStatus=YXW_spiClose(hw_priv);
// 	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halStatus,yxwRetAct,YXW_ERR_INIT_PLAT);
// 	IF_ERR_RETURN(yxwRetAct);
// 	/*Release hw_priv ressource*/

// 	return (uint32_t)yxwRetAct;
// }

uint32_t YXW_resetDevice(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus= YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);

	/*BBPLL_REG_RESETB_AAFC=1*/
	/*BBPLL_REG_RESETB_GEN=1*/
	/*BBPLL_REG_RESETB_BB=1*/
	/*BBPLL_REG_RESETB_RF=1*/
	/*BBPLL_REG_RESETB_1GHZ=1*/
	halstatus=YXW_spiWriteField(hw_priv,REG_BBPLL6,1,15,15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_BBPLL6,0x0F,31,28);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*RFPLL_RESETB_SDM=1*/
	/*RFPLL_RESETB_AAC_AFC=1*/
	/*RFPLL_RESETB_SDMCTRL=1*/
	/*RFPLL_RESETB_RFSYNC=1*/
	halstatus=YXW_spiWriteField(hw_priv,REG_RFPLL10,0x0F,3,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*AUXPLL_RESETB_SDM=1*/
	/*AUXPLL_RESETB_AAC_AFC=1*/
	/*AUXPLL_RESETB_SDMCTRL=1*/
	/*AUXPLL_RESETB_RFSYNC=1*/
	halstatus=YXW_spiWriteField(hw_priv,REG_AUXPLL10,0x0F,3,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//BBPLL_REF_DIV_RSTN = 0
	halstatus=YXW_spiWriteField(hw_priv,REG_BBPLL0,0,15,15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//BBPLL_REF_DIV_RSTN = 1
	halstatus=YXW_spiWriteField(hw_priv,REG_BBPLL0,1,15,15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//RFPLL_REF_DIV_RSTN = 0
	halstatus=YXW_spiWriteField(hw_priv,REG_RFPLL0,0,15,15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//RFPLL_REF_DIV_RSTN = 1
	halstatus=YXW_spiWriteField(hw_priv,REG_RFPLL0,1,15,15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	//AUXPLL_REF_DIV_RSTN = 0
	halstatus=YXW_spiWriteField(hw_priv,REG_AUXPLL0,0,15,15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//AUXPLL_REF_DIV_RSTN = 1
	halstatus=YXW_spiWriteField(hw_priv,REG_AUXPLL0,1,15,15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//BBPLL_EN_CLK_500M = 0
	halstatus=YXW_spiWriteField(hw_priv,REG_BBPLL4,0,21,21);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//BBPLL_EN_CLK_500M = 1
	halstatus=YXW_spiWriteField(hw_priv,REG_BBPLL4,1,21,21);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//BBPLL_EN_CLK_SARADC = 0
	halstatus=YXW_spiWriteField(hw_priv,REG_BBPLL4,0,20,20);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	//BBPLL_EN_CLK_SARADC = 1
	halstatus=YXW_spiWriteField(hw_priv,REG_BBPLL4,1,20,20);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_RFPLL11,1,10,10);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_AUXPLL11,1,10,10);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_SYNC_0,1,29,29);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*apd_reset_ext=1*/
	halstatus=YXW_spiWriteField(hw_priv,REG_GC_50,1,8,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*RX1_PDET_PD_0=1*/
	/*RX1_PDET_PD_0=0*/
	halstatus=YXW_spiWriteField(hw_priv,REG_RXFE0,1,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_RXFE0,0,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*RX2_PDET_PD_0=1*/
	/*RX2_PDET_PD_0=0*/
	halstatus=YXW_spiWriteField(hw_priv,REG_RXFE8,1,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_RXFE8,0,16,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	/*apd_reset_ext=0*/
	halstatus=YXW_spiWriteField(hw_priv,REG_GC_50,0,8,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_0,0,2,2);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_0,1,2,2);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

static uint32_t yxw_verifyRxProfile(YxwDevice_t *hw_priv, YxwRxProfile_t *rxProfile, uint32_t *rxHsDigClk_kHz)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	FUNCTION_ENTER(hw_priv);

	*rxHsDigClk_kHz = 0;

	/********************************/
	/* Check for a valid Rx profile */
	/********************************/
	if ((rxProfile->rxOutputRate_kHz < 61440) ||
		(rxProfile->rxOutputRate_kHz > 245760))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
				YXW_ERR_VERTRXPFILE_RXINPUTRATE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((rxProfile->rfBandwidth_Hz < 50000000) ||
		(rxProfile->rfBandwidth_Hz > 200000000))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
				YXW_ERR_VERXPFILE_RFBAND,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((rxProfile->rhb1Decimation != 1) &&
		(rxProfile->rhb1Decimation != 2))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERRXPFILE_THB1,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}
	if ((rxProfile->rxFirDecimation != 1) &&
		(rxProfile->rxFirDecimation != 2) &&
		(rxProfile->rxFirDecimation != 4))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERRXPFILE_RXFIR1,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((rxProfile->rxFir.coefs == NULL) &&
		(rxProfile->rxFirDecimation != 1))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERRXPFILE_RXFIR1,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	*rxHsDigClk_kHz = ( rxProfile->rxOutputRate_kHz * rxProfile->rxFirDecimation *
						rxProfile->rhb1Decimation);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

static yxwHalErr_t yxw_verifyObsRxprofile(YxwDevice_t *hw_priv,YxwORxProfile_t *orxProfile, uint32_t *orxHsDigClk_kHz)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	*orxHsDigClk_kHz = 0;
	FUNCTION_ENTER(hw_priv);

	/********************************/
	/* Check for a valid ORx profile */
	/********************************/
	if ((orxProfile->orxOutputRate_kHz < 245760) ||
		(orxProfile->orxOutputRate_kHz > 491520))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
				YXW_ERR_VERTORXPFILE_RXINPUTRATE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((orxProfile->rfBandwidth_Hz < 50000000) ||
		(orxProfile->rfBandwidth_Hz > 450000000))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
				YXW_ERR_VEORXPFILE_RFBAND,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((orxProfile->orxFirDecimation != 1) &&
		(orxProfile->orxFirDecimation != 2))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERORXPFILE_FIR,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((orxProfile->rxFir.coefs == NULL) &&
		(orxProfile->orxFirDecimation != 1))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERORXPFILE_FIR1,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	*orxHsDigClk_kHz = (orxProfile->orxOutputRate_kHz * orxProfile->orxFirDecimation);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}
/**
* Verify the TxProfile Valid,If the parameter is valid.
* Return YXW_NO_ACTION,otherwise return YXW_ERR_CHECK_PARAM
*/
static uint32_t yxw_verifyTxProfile(YxwDevice_t *hw_priv,YxwTxProfile_t *txProfile,
													uint32_t *txHsDigClk_kHz)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	FUNCTION_ENTER(hw_priv);

	*txHsDigClk_kHz = 0;
	/********************************/
	/* Check for a valid Tx profile */
	/********************************/

	if ((txProfile->txInputRate_kHz < 122880) ||
		(txProfile->txInputRate_kHz > 491520))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
				YXW_ERR_VERTXPFILE_TXINPUTRATE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((txProfile->rfBandwidth_Hz < 50000000) ||
		(txProfile->rfBandwidth_Hz > 450000000))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERTXPFILE_RFBAND,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((txProfile->thb1Interpolation != 1) &&
		(txProfile->thb1Interpolation != 2))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERTXPFILE_THB1,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((txProfile->thb2Interpolation != 1) &&
		(txProfile->thb2Interpolation != 2))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERTXPFILE_THB2,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((txProfile->thb3Interpolation != 1) &&
		(txProfile->thb3Interpolation != 2))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERTXPFILE_THB3,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((txProfile->txFirInterpolation != 1) &&
		(txProfile->txFirInterpolation != 2) &&
		(txProfile->txFirInterpolation != 4))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERTXPFILE_TXFIR,yxwRetAct,YXW_ERR_CHECK_PARAM);

	}

	if ((txProfile->txFir.coefs == NULL) &&
		(txProfile->txFirInterpolation != 1))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
			YXW_ERR_VERTXPFILE_TXFIR1,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	*txHsDigClk_kHz = (txProfile->txInputRate_kHz * txProfile->txFirInterpolation *
					   txProfile->thb1Interpolation * txProfile->thb2Interpolation *
					   txProfile->thb3Interpolation);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}
uint32_t YXW_verifyProfiles(YxwDevice_t *hw_priv, YxwInit_t *init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t rxHsDigClk_kHz = 0;
	uint32_t orxHsDigClk_kHz = 0;
	uint32_t txHsDigClk_kHz = 0;
	YxwRxProfile_t *rxProfile  = NULL;
	YxwTxProfile_t *txProfile = NULL;
	YxwORxProfile_t *orxProfile  = NULL;

	FUNCTION_ENTER(hw_priv);
	if(init==NULL){
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
					YXW_ERR_DEVICE_INIT_NULL_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	hw_priv->YxwState.gainIndexes.tx1MaxGainIndex = init->tx.txGainCtrl.tx1MaxGainIndex;
	hw_priv->YxwState.gainIndexes.tx1MinGainIndex = init->tx.txGainCtrl.tx1MinGainIndex;
	hw_priv->YxwState.gainIndexes.tx2MaxGainIndex = init->tx.txGainCtrl.tx2MaxGainIndex;
	hw_priv->YxwState.gainIndexes.tx2MinGainIndex = init->tx.txGainCtrl.tx2MinGainIndex;

	hw_priv->YxwState.gainIndexes.rx1MaxGainIndex = init->rx.rxGainCtrl.rx1MaxGainIndex;
	hw_priv->YxwState.gainIndexes.rx1MinGainIndex = init->rx.rxGainCtrl.rx1MinGainIndex;
	hw_priv->YxwState.gainIndexes.rx2MaxGainIndex = init->rx.rxGainCtrl.rx2MaxGainIndex;
	hw_priv->YxwState.gainIndexes.rx2MinGainIndex = init->rx.rxGainCtrl.rx2MinGainIndex;

	hw_priv->YxwState.gainIndexes.orx1MaxGainIndex = init->obsRx.orxGainCtrl.orx1MaxGainIndex;
	hw_priv->YxwState.gainIndexes.orx1MinGainIndex = init->obsRx.orxGainCtrl.orx1MinGainIndex;
	hw_priv->YxwState.gainIndexes.orx2MaxGainIndex = init->obsRx.orxGainCtrl.orx2MaxGainIndex;
	hw_priv->YxwState.gainIndexes.orx2MinGainIndex = init->obsRx.orxGainCtrl.orx2MinGainIndex;

	hw_priv->YxwState.profilesValid = 0;

	txProfile = &init->tx.txProfile;
	yxwRetAct = yxw_verifyTxProfile(hw_priv, txProfile, &txHsDigClk_kHz);
	IF_ERR_RETURN(yxwRetAct);

	rxProfile = &init->rx.rxProfile;
	yxwRetAct = yxw_verifyRxProfile(hw_priv, rxProfile, &rxHsDigClk_kHz);
	IF_ERR_RETURN(yxwRetAct);

	orxProfile = &init->obsRx.orxProfile;
	yxwRetAct = (YxwRecoveryActions_t)yxw_verifyObsRxprofile(hw_priv, orxProfile, &orxHsDigClk_kHz);
	IF_ERR_RETURN(yxwRetAct);

	if ((init->clocks.rfPllPhaseSyncMode != YXW_RFPLLMCS_NOSYNC)
		&& (init->clocks.rfPllPhaseSyncMode != YXW_RFPLLMCS_INIT_AND_SYNC)
		&& (init->clocks.rfPllPhaseSyncMode != YXW_RFPLLMCS_INIT_AND_1TRACK)
		&& (init->clocks.rfPllPhaseSyncMode != YXW_RFPLLMCS_INIT_AND_CONTTRACK))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_INIT,
				YXW_ERR_CLOCK_RFPLL_PHASE,yxwRetAct,YXW_ERR_RESET_SPI);
	}
	/* Verify Tx profile is valid */
	if (txHsDigClk_kHz > 0)
	{
		hw_priv->YxwState.profilesValid|= TX_PROFILE_VALID;
	}
	/* Verify Rx profile is valid */
	if (rxHsDigClk_kHz > 0)
	{
		hw_priv->YxwState.profilesValid |= RX_PROFILE_VALID;
	}
	/* Verify ORx profile is valid */
	if (orxHsDigClk_kHz > 0)
	{
		 hw_priv->YxwState.profilesValid |= ORX_PROFILE_VALID;
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_programFir(YxwDevice_t *hw_priv,YxwFir_t *filter, int FirSel)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t i = 0,j=0, curpos = 0;
	uint32_t len;
	YxwData DataInfo ={0};
	YxwMem *pMsg=NULL;
	FUNCTION_ENTER(hw_priv);

	pMsg=&DataInfo.u.Mmsg;

	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_WRITE_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_FILTER_MSGID;

	for (i = 0; i < 4; i++)
	{
		pMsg->Msg.data[i] = (uint8_t)(((filter->gain_dB) >> (i * 8)) & 0x000000FF);
	}
	for (i = 4; i < 8; i++)
	{
		pMsg->Msg.data[i] = (uint8_t)(((filter->numFirCoefs) >> ((i - 4) * 8)) & 0x000000FF);
	}


	if(FirSel == YXW_TX_FILTER)
	{
		pMsg->Msg.offset=0;
		pMsg->Msg.Lenth=HI_MSG_TO_TXFILTER_MSGLEN;
		len = HI_MSG_TO_TXFILTER_COEFS_LEN;
	}
	else if(FirSel == YXW_RX_FILTER)
	{
		pMsg->Msg.offset=HI_MSG_TO_TXFILTER_MSGLEN;
		pMsg->Msg.Lenth=HI_MSG_TO_RX_ORXFILTER_MSGLEN;
		len = HI_MSG_TO_RX_ORX_FILTER_COEFS_LEN;
	}else{
		pMsg->Msg.offset=HI_MSG_TO_TXFILTER_MSGLEN+HI_MSG_TO_RX_ORXFILTER_MSGLEN;
		pMsg->Msg.Lenth=HI_MSG_TO_RX_ORXFILTER_MSGLEN;
		len = HI_MSG_TO_RX_ORX_FILTER_COEFS_LEN;
	}
	curpos = i;
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < 2; j++)
		{
			pMsg->Msg.data[curpos] = (uint8_t)(((filter->coefs[i]) >> (j * 8)) & 0x000000FF);
			curpos++;
		}
	}
	yxwRetAct = YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_initialize(YxwDevice_t *hw_priv,YxwInit_t *Init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus = YXWHAL_OK;

	yxwInfo_t *pStateInfo = &hw_priv->YxwState;
	pStateInfo->devState=YXW_STATE_INITIALIZED;

	FUNCTION_ENTER(hw_priv);

	yxwRetAct=YXW_enableRiscvLog(hw_priv,Init->Loglevel);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_verifyProfiles(hw_priv,Init);
	IF_ERR_RETURN(yxwRetAct);

	/* Program FIR filters */
	if (pStateInfo->profilesValid & TX_PROFILE_VALID){
		if (Init->tx.txProfile.txFir.numFirCoefs > 0)
		{
			yxwRetAct =YXW_programFir(hw_priv,&Init->tx.txProfile.txFir,YXW_TX_FILTER);
			IF_ERR_RETURN(yxwRetAct);
		}
		yxwRetAct = YXW_setTxManualGain(hw_priv, YXW_TX1, Init->tx.txGainCtrl.tx1GainIndex);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct = YXW_setTxManualGain(hw_priv, YXW_TX2, Init->tx.txGainCtrl.tx2GainIndex);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct=YXW_setTxGainControlMode(hw_priv,Init->tx.txGainCtrl.txgainMode);
		IF_ERR_RETURN(yxwRetAct);
	}
	if (pStateInfo->profilesValid & RX_PROFILE_VALID)
	{
		if (Init->rx.rxProfile.rxFir.numFirCoefs > 0)
		{
			yxwRetAct = YXW_programFir(hw_priv,&Init->rx.rxProfile.rxFir,YXW_RX_FILTER);
			IF_ERR_RETURN(yxwRetAct);
		}

		yxwRetAct =YXW_setRxManualGain(hw_priv, YXW_RX1, Init->rx.rxGainCtrl.rx1GainIndex);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct = YXW_setRxManualGain(hw_priv, YXW_RX2, Init->rx.rxGainCtrl.rx2GainIndex);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct=YXW_setRxGainControlMode(hw_priv,Init->rx.rxGainCtrl.rxgainMode,Init->rx.rxGainCtrl.rxmgcMode);
		IF_ERR_RETURN(yxwRetAct);
	}

	if (pStateInfo->profilesValid & ORX_PROFILE_VALID)
	{
		if (Init->obsRx.orxProfile.rxFir.numFirCoefs > 0)
		{
			/* if pointer to orx rxFIR is valid */
			yxwRetAct = YXW_programFir(hw_priv,&Init->obsRx.orxProfile.rxFir,YXW_ORX_FILTER);
			IF_ERR_RETURN(yxwRetAct);
		}
		/*Update orx gaintable*/
		yxwRetAct=YXW_programObsRxGainTable(hw_priv,Init->obsRx.TableP);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct = YXW_setObsRxManualGain(hw_priv, YXW_ORX1, Init->obsRx.orxGainCtrl.orx1GainIndex);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct = YXW_setObsRxManualGain(hw_priv, YXW_ORX2, Init->obsRx.orxGainCtrl.orx2GainIndex);
		IF_ERR_RETURN(yxwRetAct);

		yxwRetAct=YXW_setObsRxGainControlMode(hw_priv,Init->obsRx.orxGainCtrl.orxgainMode,Init->obsRx.orxGainCtrl.orxmgcMode);
		IF_ERR_RETURN(yxwRetAct);
	}

	yxwRetAct = YXW_spiWriteField(hw_priv, REG_RXFE1, 0xF, 4, 1);//RX Tia gain: -6db
	IF_ERR_RETURN(yxwRetAct);
	yxwRetAct = YXW_spiWriteField(hw_priv, REG_RXFE9, 0xF, 4, 1);//ORX Tia gain: -6db
	IF_ERR_RETURN(yxwRetAct);

	pStateInfo->clocks.deviceClock_kHz=Init->clocks.deviceClock_kHz;
	pStateInfo->supportChannels =  (Init->tx.txChannels & 3);
	pStateInfo->supportChannels |= ((Init->rx.rxChannels & 3) << 2);
	pStateInfo->supportChannels |= ((Init->obsRx.obsRxChannelsEnable & 3) << 4);
	pStateInfo->txInputRate_kHz = Init->tx.txProfile.txInputRate_kHz;

	pStateInfo->rxBandwidth_Hz = Init->rx.rxProfile.rfBandwidth_Hz;
	pStateInfo->txBandwidth_Hz = Init->tx.txProfile.rfBandwidth_Hz;
	pStateInfo->orxBandwidth_Hz = Init->obsRx.orxProfile.rfBandwidth_Hz;
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)halstatus;
}
