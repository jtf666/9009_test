/**
* \file yxw_agc.h
* \brief Contains Yxw AGC function prototypes for yxw_agc.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef _YXW_AGC_H_
#define _YXW_AGC_H_

#include "yxw_agc_types.h"
#include "yxw_types.h"

/**
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxAgcCtrl Pointer to AGC data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setupRxAgc(YxwDevice_t *hw_priv, YxwAgcCfg_t *rxAgcCtrl);

/**
 * \param device Pointer to the Y9009 data structure containing settings
 * \param orxAgcCtrl Pointer to AGC data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setupOrxAgc(YxwDevice_t *hw_priv, YxwAgcCfg_t *orxAgcCtrl);

/***
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel, 1:YXW_RX1, 2:YXW_RX2
 * \param maxValue agc max index
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
 uint32_t YXW_SetAgcMaxIndex(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel, uint32_t maxIndex);

/***
 * \param device Pointer to the Y9009 data structure containing settings
 * \param rxChannel, 1:YXW_RX1, 2:YXW_RX2
 * \param maxValue agc max index
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
 uint32_t YXW_GetAgcMaxIndex(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel, uint32_t *maxIndex);

 uint32_t YXW_lp(YxwDevice_t *hw_priv, YxwInit_t *Init);
#endif

