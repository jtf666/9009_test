/**
* \file yxw_jesd204.h
* \brief Contains Y9009 JESD204b data path related function prototypes for
 *        yxw_jesd204.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef _YXW_JESD204B_H_
#define _YXW_JESD204B_H_

#include "yxw_jesd204b_types.h"
#include "yxw_types.h"

/**
 * \brief write Jes204b config
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param Init pointer the initial profile
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_writeJes204bConfig(YxwDevice_t *priv, YxwInit_t *Init);

/**
 * \brief wait for Jes204b config done
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_waitJes204bConfigDone(YxwDevice_t *hw_priv, uint32_t timeout);

/**
 * \brief Get Jes204b sysref control configure
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param mode Desired Jes204b Sysref ON or OFF
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_GetJes204bSysrefControlStatus(YxwDevice_t *priv);

/**
 * \brief set prbs15 system tx enable/disabled
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_SetPrbs15_SystemTx(YxwDevice_t *hw_priv, uint32_t enable);

/**
 * \brief get prbs15 system tx info(Include enable and statistics info)
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_GetPrbs15_SystemTxInfo(YxwDevice_t *hw_priv, uint32_t *enable);

/**
 * \brief set prbs15 system rx/orx enable/disabled
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_SetPrbs15_SystemRxORx(YxwDevice_t *hw_priv, uint32_t enable);

/**
 * \brief get prbs15 system rx/orx enable status
 *
 * \param device Pointer to the Y9009 data structure containing settings
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_GetPrbs15_SystemRxORx_EnableStatus(YxwDevice_t *hw_priv,
	uint32_t *rx_enable, uint32_t *orx_enable);

/**
 * \brief wait for Jes204b framerLink enable
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_enableFramerLink(YxwDevice_t *hw_priv, YxwFramerSel_t framerSel, uint32_t enable);

/**
 * \brief wait for Jes204b deframerLink enable
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_enableDeframerLink(YxwDevice_t *hw_priv, YxwDeframerSel_t deframerSel, uint32_t enable);

/**
 * \brief enable sysref to framer
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_enableSysRefToFramer(YxwDevice_t *hw_priv, YxwInit_t *Init);

/**
 * \brief get 204b sync status
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param TypeStatus Pointer to 204b sync status
 * Through three macros: TX_DEFRAMER_NOT_SYNC, RX_FRAMER_NOT_SYNC, ORX_FRAMER_NOT_SYNC
 * Determine Whether TX/RX/ORX Is Sync,Example RX is Sync as follows:
 * if(RX_FRAMER_NOT_SYNC == ((TypeStatus)&RX_FRAMER_NOT_SYNC))
 * {
 *    //RX Not Sync.
 * }
 * else
 * {
 * 	  //RX Is Sync.
 * }
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t  YXW_getSyncStatus(YxwDevice_t * hw_priv, uint32_t *TypeStatus);

/**
 * \brief waitSync204b
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param timeout wait time //ms
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_waitSync204b(YxwDevice_t *hw_priv, uint32_t timeout);

/**
 * \brief yxw Spi Get Debug Data
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param astore_addr setDebug addr of Data
 * \param sample_point Desired Jes204b sample point
 * \param iq_sel < 0;I,1:Q>
 * \param sample_rate < 1:250Mhz, 2:500Mhz >
 * \param length setDebug length of Data
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_spiGetJesdDebugData(YxwDevice_t *hw_priv, uint32_t store_addr,
	uint32_t sample_point, uint32_t sample_rate, uint32_t iq_sel, uint32_t length);

/**
 * \brief set Y9009 lane sequence num
 * \param device Pointer to the Y9009 data structure containing settings
 * \param tx path lane sequence num
 * \param rx/orx path lane sequence num
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setLaneSequence(YxwDevice_t *hw_priv,
	YxwJesdLaneSeq_t TxlaneSeq, YxwJesdLaneSeq_t RxORxlaneSeq);

/**
 * \brief get Y9009 lane sequence num
 * \param device Pointer to the Y9009 data structure containing settings
 * \param tx path lane sequence num,if TxLaneSeq ==0x0.it means the jesd204b laneid seq is
	 0-1-2-3,TxLaneSeq==0x1,it means the jesd204b laneid seq is 0-1-3-2,in
	 detail refer to enum YxwJesdLaneSeq_t
 * \param rx/orx path lane sequence num,it the same to TxLaneSeq describe
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getLaneSequence(YxwDevice_t *hw_priv,
	YxwJesdLaneSeq_t *TxLaneSeq, YxwJesdLaneSeq_t *RxOrxLaneSeq);

/**
 * \brief set Y9009 byte sequence mask
 * \param device Pointer to the Y9009 data structure containing settings
 * \param tx path byte sequence mask
 * \param rx path byte sequence mask
 * \param orx path byte sequence mask
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_setByteSequence(YxwDevice_t *hw_priv, YxwJesdByteSeq_t tx,
	YxwJesdByteSeq_t rx, YxwJesdByteSeq_t orx);

/**
 * \brief get Y9009 byte sequence mask
 * \param device Pointer to the Y9009 data structure containing settings
 * \param tx path byte sequence mask
 * \param rx path byte sequence mask
 * \param orx path byte sequence mask
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
uint32_t YXW_getByteSequence(YxwDevice_t *hw_priv, YxwJesdByteSeq_t *tx,
	YxwJesdByteSeq_t *rx, YxwJesdByteSeq_t *orx);

#endif //__Y9009_JESD204B_H__
