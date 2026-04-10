/**
* \file yxw_hopping.h
* \brief Contains Y9009 hopping frequence related function prototypes for
 *        yxw_hopping.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#ifndef _YXW_HOPPING_H_
#define _YXW_HOPPING_H_

#include "yxw_gpio.h"
#include "yxw_hopping_types.h"
#include "yxw_types.h"

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param YxwHoppingInfo_t hoplist info
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_initHopping(YxwDevice_t *hw_priv, YxwHoppingInfo_t* config);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param index hoplist freq
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_triggerHopping_Spi(YxwDevice_t *hw_priv, uint32_t index);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param pinNum hop indicate gpio, it can support gpio 0-9
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_configHopping_Pin(YxwDevice_t *hw_priv, YxwGpioPinSel_t pinNum);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_deinitHopping(YxwDevice_t *hw_priv);

#endif
