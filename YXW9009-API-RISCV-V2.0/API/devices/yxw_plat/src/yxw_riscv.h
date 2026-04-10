/**
* \file yxw_riscv.h
* \brief Contains Y9009 related function prototypes for yxw_riscv.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_RISCV_H__
#define __YXW_RISCV_H__

#include "yxw_radio_types.h"
#include "yxw_riscv_types.h"
#include "yxw_types.h"


/**
 * \brief reset y9009 by spi
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param RestType can be interrupt or spi register
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_spiResetChip(YxwDevice_t *hw_priv, uint8_t RestType);

/**
 * \brief download firmware
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_loadFirmware(YxwDevice_t *hw_priv);

/**
 * \brief download firmware function
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param addr firmware itcm/dtcm memory
 * \param data firmware itcm/dtcm data
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_writeFirmware(YxwDevice_t *hw_priv, uint16_t addr, uint32_t data);

/**
 * \brief write riscv memory data
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param regdata wirte memory data
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_spiWriteMem(YxwDevice_t *hw_priv, uint32_t addr, uint32_t data);

/**
 * \brief read riscv memory data
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param regdata store memory data
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_spiReadMem(YxwDevice_t *hw_priv, uint32_t addr, uint32_t *regdata);

/**
 * \brief write the initail profile
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Init pointer the initial profile
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_writeRiscvProfile(YxwDevice_t *priv, YxwInit_t *Init);

/**
 * \brief read the initail profile
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Init pointer the initial profile
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_readRiscvProfile(YxwDevice_t *priv, YxwInit_t *Init);

/**
 * \brief get the riscvtraceEvent
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_GetRiscvTraceEvent(YxwDevice_t *hw_priv);

/**
 * \brief get the riscvlog
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param addr the riscvlog store
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint8_t * YXW_spiGetRiscvLog(YxwDevice_t *hw_priv, uint32_t addr);

/**
 * \brief enable riscv log
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param debug 1 enable.othewise 0 is disable
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_enableRiscvLog(YxwDevice_t *hw_priv, uint32_t debug);

/**
 * \brief Assert spi interrupt to riscv
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param writeData 1, 1 means assert spi interrupt otherwise 0 cannot assert spi interrupt to riscv
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t yxw_spiInterrupt(YxwDevice_t *hw_priv,uint32_t writeData);

/**
 * \brief wait for command confirm status
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_spiWaitCmdConfirm(YxwDevice_t *hw_priv);

/**
 * \brief get riscvcmd  statuc
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_spiGetCmdStatus(YxwDevice_t *hw_priv);

/**
\brief setcommad to risv
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Command set command to riscv
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_sendRiscvCmd(YxwDevice_t *priv,void *UserData, unsigned int Command);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getBootStatus(YxwDevice_t *hw_priv);

/**
 *\brief wait fot boot already bring up
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  YXW_waitFirmwareReady(YxwDevice_t *hw_priv, uint32_t timeout);

/**
 * \brief wait for profile write done
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  YXW_waitProfileDone(YxwDevice_t *hw_priv, uint32_t timeout);

/**
 * \brief clear the status
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */

uint32_t YXW_clearStatus(YxwDevice_t *hw_priv);

/**
 * \brief get the sdkversion message
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getSdkVersion(void);

/**
 * \brief get the firmware version message
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getFirmwareVersion(YxwDevice_t *hw_priv);

/**
 * \brief get after reset reg97 value
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_recoverCpu(YxwDevice_t *hw_priv);

/**
 * \brief set Wait Timeout Bit
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setWaitTimeoutBit(YxwDevice_t *hw_priv, uint32_t action, uint32_t timeout_bit);

/**
 * \brief get Wait Timeout Bit
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getWaitTimeoutBit(YxwDevice_t *hw_priv);
#endif
