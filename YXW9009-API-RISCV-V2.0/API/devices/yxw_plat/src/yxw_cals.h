/**
* \file yxw_cals.h
* \brief Contains Y9009 calibration related function prototypes for
 *        yxw_cals.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#ifndef _YXW_CALS_H_
#define _YXW_CALS_H_

#include "yxw_cals_types.h"
#include "yxw_types.h"

/**
 * \brief Set Init Cals channel
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param channels Calibrated channels.
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setCalsChannel(YxwDevice_t *hw_priv, YxwCalsChannel_t channels);

/**
 * \brief Set Init Cals channel
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param channels Calibrated channels.
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getCalsChannel(YxwDevice_t *hw_priv, YxwCalsChannel_t *channels);

/**
 * \brief Run Init Cals
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param initCalMask Calibrated Mask.
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_runInitCals(YxwDevice_t *hw_priv, YxwInitCalibrations_t initCalMask);

/**
 * \brief Wait Init Cals Done
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_waitInitCalsDone(YxwDevice_t *hw_priv, uint32_t timeout);

/**
 * \brief Wait Init Cals Out Ring Done
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_waitInitCalsOutRingDone(YxwDevice_t *hw_priv, uint32_t timeout);

/**
 * \brief Get Init Cals Status
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval The first address of the string is returned correctly, and null is returned in case of error.
 */
uint32_t YXW_getInitCalsStatus(YxwDevice_t *hw_priv);

#endif

