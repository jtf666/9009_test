/**
* \file yxw_otp.h
* \brief Y9009 Otp header file
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef YXW_OTP_H__
#define YXW_OTP_H__

#include "yxw_otp_types.h"
#include "yxw_riscv_types.h"
#include "yxw_types.h"

/*operation otp function*/
/**
 * \brief otp read function
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param OtpAddr optddr that read from otp
 * \param Data Read Otp data
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_OtpRead(YxwDevice_t *hw_priv, uint32_t OtpAddr, uint32_t *Data);
#endif
