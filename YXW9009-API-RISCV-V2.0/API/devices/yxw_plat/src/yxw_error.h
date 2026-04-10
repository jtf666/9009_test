/**
* \file yxw_error.h
* \brief Contains Y9009 API error handling function prototypes and data types for Y9009_error.c
 *        These functions are public to the customer for getting more details on
 *        errors and debugging.
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#ifndef __YXW_ERROR_H__
#define __YXW_ERROR_H__

#include "yxw_error_types.h"
#include "yxw_types.h"
/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param errHdl Error Handler type
 * \param detErr Error detected to be processed by handler
 * \param retVal current Recovery Action
 * \param RecAction new Recovery action to be returned should err handle determin an error
 * \retval uint32 value representing the latest recovery action following processing of detected error
 */
YxwRecoveryActions_t YXW_ApiErrHandler(YxwDevice_t *hw_priv, YxwErrHdls_t errHdl,
    uint32_t detErr, YxwRecoveryActions_t retVal, YxwRecoveryActions_t recAction);
#endif
