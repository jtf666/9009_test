/**
* \file yxw_tx.h
* \brief Contains Y9009 transmit related function prototypes for
 *        yxw_tx.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_TX_H__
#define __YXW_TX_H__

#include "yxw_tx_types.h"
#include "yxw_types.h"

/**
 * \brief Sets the Tx1/Tx2 Manual Gain Index
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param txChannel select the channel to update the gainindex
 * \param gainindex Desired Rx1/Rx2 gain index depending on rxchannle parameter
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setTxManualGain(YxwDevice_t *hw_priv,YxwTxChannels_t txChannel, uint8_t gainIndex);
/**
 * \brief configures the rx gain control mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setTxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t mode);

/**
 * \brief get Tx gain index
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param TxChannel
 * \param TxGainIndex
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getTxGain(YxwDevice_t *hw_priv,YxwTxChannels_t TxChannel, uint32_t *TxGainIndex);

/**
 * \brief set PA protect config
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param txPaProtectCfg
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setPaProtectionCfg(YxwDevice_t *hw_priv,YxwTxPaProtectCfg_t *txPaProtectCfg);

/**
 * \brief  this function for get pa protect errorf flags
 *
 * \param hw_priv Pointer to the Y9009 data structure containing settings
 * \param get errorFlags flag
 *
 * \retval
 */
uint32_t YXW_getPaProtectErrorFlags(YxwDevice_t *hw_priv,uint32_t *errorFlags);

/**
 * \brief  this function for revert clear paprotectflag
 *
 * \param hw_priv Pointer to the Y9009 data structure containing settings
 * \param txChannel clear the channle flag
 *
 * \retval
 */
uint32_t YXW_RevertClearPaProtect(YxwDevice_t *hw_priv);

/**
 * \brief  this function for clear paprotectflag
 *
 * \param hw_priv Pointer to the Y9009 data structure containing settings
 * \param txChannel clear the channle flag
 *
 * \retval
 */
uint32_t YXW_clearPaProtectErrorFlags(YxwDevice_t *hw_priv,YxwTxChannels_t txChannel);
/**
 * \brief  this function for set DDS config
 *
 * \param hw_priv Pointer to the Y9009 data structure containing settings
 * \param txChannel TX channel select of DDS tone
 * \param txDDSTone DDS tone freq
 * \param txDDSAtten DDS tone attenuation
 *
 * \retval
 */
uint32_t YXW_setDDsFreqConfig(YxwDevice_t *hw_priv, YxwTxChannels_t txChannel, YxwTxDdsTestToneCfg_t txDDSTone, YxwTxDdsAttenCfg_t txDDSAtten);
/**
 * \brief  this function for set DDS disable
 *
 * \param hw_priv Pointer to the Y9009 data structure containing settings
 *
 * \retval
 */
uint32_t YXW_setDdsOff(YxwDevice_t *hw_priv);
/**
 * \brief get the tx dec power
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \ret RET_SUCCESS means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getTxDecPowerFloat(YxwDevice_t *hw_priv, YxwTxChannels_t txChannel, float *txDecPower_dBFS);
#endif


