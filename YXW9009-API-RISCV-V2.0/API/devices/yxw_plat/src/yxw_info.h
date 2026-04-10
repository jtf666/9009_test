/**
* \file yxw_info.h
* \brief Contains Show Yxw20 Chip work status information API.
* \version: Ver 1.0
* \author :
* \date   : 2023/04/20 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#ifndef _YXW_INFO_H_
#define _YXW_INFO_H_
#include "yxw_config_types.h"
#include "yxw_types.h"

#define YXW_INFOPRINT printf

/**
 * \brief show all info
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_InfoShow_rx(YxwDevice_t *hw_priv);
uint32_t YXW_InfoShow_tx(YxwDevice_t *hw_priv);

/**
 * \brief get yxw all Path Gain informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getPathGainInfo(YxwDevice_t *hw_priv);

/**
 * \brief get yxw all Pll informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getPllInfo(YxwDevice_t *hw_priv);

/**
 * \brief get yxw radio module informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getRadioInfo(YxwDevice_t *hw_priv);

/**
 * \brief get yxw agc module informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getAgcInfo(YxwDevice_t *hw_priv);


/**
 * \brief get yxw cals module informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getCalsInfo(YxwDevice_t *hw_priv);

/**
 * \brief get yxw init module informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Init Pointer to the yxw_config.c init structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getInitInfo(YxwDevice_t *hw_priv, YxwInit_t *Init);

/**
 * \brief get yxw Jesd204b module informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getJesd204bInfo(YxwDevice_t *hw_priv);

/**
 * \brief get yxw gpio module informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getGpioInfo(YxwDevice_t *hw_priv);

/**
 * \brief yxw set debug macro DBG_LEVEL_DEBUG can print informations
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_debugInfo(YxwDevice_t *hw_priv);

#endif
