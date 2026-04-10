/**
* \file yxw_rx.h
* \brief Contains Y9009 receive related function prototypes for
 *        yxw_rx.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_RX_H__
#define __YXW_RX_H__

#include "yxw_rx_types.h"
#include "yxw_types.h"

/**
 * \brief Set the Rx Tia gain
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param gain rx tia gain
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setRxTiaGain(YxwDevice_t *hw_priv, YxwTiaChannel_t channel, YxwTiaGain_t gain);

/**
 * \brief Get the Rx Tia gain
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param gain rx tia gain
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getRxTiaGain(YxwDevice_t *hw_priv, YxwTiaChannel_t channel, YxwTiaGain_t* gain);

/**
 * \brief Set the Orx Tia gain
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param gain orx tia gain
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setOrxTiaGain(YxwDevice_t *hw_priv, YxwTiaChannel_t channel, YxwTiaGain_t gain);

/**
 * \brief Get the Orx Tia gain
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param gain orx tia gain
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getOrxTiaGain(YxwDevice_t *hw_priv, YxwTiaChannel_t channel, YxwTiaGain_t* gain);

/**
 * \brief Sets the Rx1/Rx2 Manual Gain Index
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel select the channel to update the gainindex
 * \param gainindex Desired Rx1/Rx2 gain index depending on rxchannle parameter
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setRxManualGain(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel, uint8_t gainIndex);

/**
 * \brief get the Rx1/Rx2 Manual Gain Index
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel select the channel to update the gainindex
 * \param gainindex Desired Rx1/Rx2 gain index depending on rxchannle parameter
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getRxGain(YxwDevice_t *hw_priv,YxwRxChannels_t rxChannel, uint32_t *rxGainIndex);

/**
 * \brief configure gain steps and the gpio inputs for manual Rx gain control
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel select the channel to update the gainindex
 * \param rxGainCtrlPin poPointer to Y9009RxGainCtrlPin_t structure that configures the mannual rx gain pin control
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  YXW_setRxGainCtrlPin(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel, YxwRxGainCtrlPin_t *rxGainCtrlPin);

/**
 * \brief configures the rx gain control mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setRxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t mode,YxwCtrlWay_t ctrlWay);

/**
 * \brief get rx gain control mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getRxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t *mode,YxwCtrlWay_t *ctrlWay);

/**
 * \brief programs the gain table settings for either Rx1\Rx2\
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param numGainIndexesInTable configure table
 * \param orxChannel  Desired orxchannle
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_programObsRxGainTable(YxwDevice_t *hw_priv, YxwGainTab_t * gainTablePtr);

/**
 * \brief Sets the ORx1/ORx2 Manual Gain Index
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel select the channel to update the gainindex
 * \param gainindex Desired Rx1/Rx2 gain index depending on rxchannle parameter
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setObsRxManualGain(YxwDevice_t *hw_priv,YxwObsRxChannels_t obsRxCh, uint8_t gainIndex);
/**
 * \brief configure gain steps and the gpio inputs for manual ORx gain control
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param orxChannel select the channel to update the gainindex
 * \param rxGainCtrlPin poPointer to Y9009RxGainCtrlPin_t structure that configures the mannual rx gain pin control
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  YXW_setObsRxGainCtrlPin(YxwDevice_t *hw_priv, YxwObsRxChannels_t orxChannel, YxwRxGainCtrlPin_t *rxGainCtrlPin);

/**
 * \brief configures the orx gain control mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setObsRxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t mode,YxwCtrlWay_t ctrlWay);

/**
 * \brief get orx gain control mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param mode Desired gain mode to be configured MGC or AGC
 * \param ctrlway apimode or gpio mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getObsRxGainControlMode(YxwDevice_t *hw_priv, YxwGainMode_t *mode,YxwCtrlWay_t *ctrlWay);

/**
 * \brief get obsrx gain index
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param obsRxChannel
 * \param obsRxGainIndex
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getObsRxGain(YxwDevice_t *hw_priv,YxwObsRxChannels_t obsRxChannel, uint32_t *obsRxGainIndex);

/**
 * \brief set rxslicer
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel
 * \param rxSlierPin
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setRxSlicerEnable(YxwDevice_t *hw_priv,YxwRxChannels_t rxChannel,YxwRxSlicerPin_t *rxSlierPin);

/**
 * \brief set orxslicer
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param orxSlierPin
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setOrxSlicerEnable(YxwDevice_t *hw_priv,YxwOrxSlicerCfg_t *orxSlierPin);
/**
 * \brief set orx pin function
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param obsRxChannel

 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setORxPinFunction(YxwDevice_t *hw_priv,YxwObsRxChannels_t obsRxChannel);

/**
 * \brief get the rx dec power
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \ret RET_SUCCESS means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getRxDecPowerFloat(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel, float *rxDecPower_dBFS);

/**
 * \brief get the orx dec power
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \ret RET_SUCCESS means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getORxDecPowerFloat(YxwDevice_t *hw_priv, YxwObsRxChannels_t orxChannel, float *orxDecPower_dBFS);
#endif

