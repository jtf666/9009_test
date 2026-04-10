/**
* \file yxw_radio.h
* \brief Contains Y9009 related function prototypes for yxw_radio.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_RADIOCTRL_H__
#define __YXW_RADIOCTRL_H__

#include "yxw_radio_types.h"
#include "yxw_rx_types.h"
#include "yxw_tx_types.h"
#include "yxw_types.h"

typedef enum
{
	LO_RF,
	LO_AUX,
	LO_MAX
} YxwLO_t;

/**
 * \brief Sets the Rx,Tx And Orx radio on by spi control mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel Enable Rx channle num
 * \param orxChannel Enable ORx channle num
 * \param txChannel Enable Tx channle num
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setRadioCtrlSpiMode(YxwDevice_t *hw_priv,
	YxwRxChannels_t rxChannel, YxwObsRxChannels_t orxChannel, YxwTxChannels_t txChannel);

/**
 * \brief Gets the Rx、Tx And Orx radio on by spi control mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel Enable Rx channle num
 * \param orxChannel Enable ORx channle num
 * \param txChannel Enable Tx channle num
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getRadioCtrlSpiMode(YxwDevice_t *hw_priv,
	YxwRxChannels_t *rxChannel, YxwObsRxChannels_t *orxChannel, YxwTxChannels_t *txChannel);

/**
 * \brief Sets the Rx、Tx And Orx radio on by pin control mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel Enable Rx channle num
 * \param orxChannel Enable ORx channle num
 * \param txChannel Enable Tx channle num
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setRadioCtrlPinMode(YxwDevice_t *hw_priv,
	YxwRxChannels_t rxChannel, YxwObsRxChannels_t orxChannel, YxwTxChannels_t txChannel);

/**
 * \brief get radio ctrl mode
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param radioCtrlMode Pointer to yxw radio ctrl mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getRadioCtrlMode(YxwDevice_t *hw_priv, YxwRadioCtlMode_t* radioCtrlMode);

/**
 * \brief Set channel seq
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param channelSeq channel seq
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setChannelSeq(YxwDevice_t *hw_priv, YxwChannelSeq_t channelSeq);

/**
 * \brief Get channel seq
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param channelSeq channel seq
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getChannelSeq(YxwDevice_t *hw_priv, YxwChannelSeq_t *channelSeq);

/**
 * \brief Sets the RF PLL local oscillator frequency
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param name of the pll to configure baseed on the enumerated types in yxwRfPllName_t
 * \param pllLoFreq_Hz Desired RF LO frequenct in Hz
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setPllFrequency(YxwDevice_t *hw_priv, uint32_t name,uint64_t pllLoFreq_Hz);

/**
 * \brief Gets the RF PLL local oscillator frequency
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param name of the pll to configure baseed on the enumerated types in yxwRfPllName_t
 * \param pllLoFreq_Hz Desired RF LO frequenct in Hz
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getPllFrequency(YxwDevice_t *hw_priv, uint32_t name, uint64_t *pllLoFreq_Hz);

/**
 * \brief check if the plls are locked
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait for locked time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_waitPllsLockDone(YxwDevice_t *hw_priv, uint32_t timeout);

/**
 * \brief Set the yxw20 radio state to the radio on state
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Mode spi mode or pin mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_radioOn(YxwDevice_t * hw_priv, YxwRadioCtlMode_t Mode);

/**
 * \brief Set the yxw20 radio state to the radio off state
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Mode spi mode or pin mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_radioOff(YxwDevice_t *hw_priv, YxwRadioCtlMode_t Mode);

/**
 * \brief Get the yxw20 radio to the radio  state
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Mode spi mode or pin mode
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getRadioState(YxwDevice_t *device);

/**
 * \brief set Pll Temperature Track Enable
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param enable 0:disable,1:enable
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setPllTemperatureTrackEnable(YxwDevice_t *hw_priv, uint32_t enable);

uint32_t YXW_pllmode(YxwDevice_t *hw_priv, YxwInit_t *Init);

uint32_t YXW_orx_rx(YxwDevice_t *hw_priv);

uint32_t YXW_rx0A_orx0D(YxwDevice_t *hw_priv);
#endif

