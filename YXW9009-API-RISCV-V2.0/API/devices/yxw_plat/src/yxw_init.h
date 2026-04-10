/**
* \file yxw_init.h
* \brief Y9009 Init header file
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_INIT_H__
#define __YXW_INIT_H__

#include "yxw_init_types.h"
#include "yxw_types.h"

/**
 * \brief open hardware
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t YXW_openHw(YxwDevice_t *hw_priv, uint32_t DeviceId);

/**
 * \brief close hardware
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_closeHw(YxwDevice_t *hw_priv);

/**
 * \brief reset info in yxw20
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_resetDevice(YxwDevice_t *hw_priv);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_programFir(YxwDevice_t *hw_priv, YxwFir_t *filter, int FirSel);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Init Pointer to the yxw_config.c init structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_initialize(YxwDevice_t *hw_priv, YxwInit_t *Init);

#endif
