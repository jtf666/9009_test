/**
* \file yxw_cals.c
* \brief Contains functions to support Yxw init and tracking calibrations
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_error.h"
#include "yxw_cals.h"

#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_jesd204b.h"
#include "yxw_plat_hal.h"
#include "yxw_radio.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"

uint32_t YXW_setCalsChannel(YxwDevice_t *hw_priv, YxwCalsChannel_t channels)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	FUNCTION_ENTER(hw_priv);

	if(channels == YXW_CALS_CHANNEL_1)
		yxwRetAct = YXW_spiWriteField(hw_priv, REG_CALI_RXIQ0_CTRL_0, 1, 21, 20);
	else if(channels == YXW_CALS_CHANNEL_2)
		yxwRetAct = YXW_spiWriteField(hw_priv, REG_CALI_RXIQ0_CTRL_0, 2, 21, 20);
	else if(channels == YXW_CALS_CHANNEL_12)
		yxwRetAct = YXW_spiWriteField(hw_priv, REG_CALI_RXIQ0_CTRL_0, 0, 21, 20);

	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getCalsChannel(YxwDevice_t *hw_priv, YxwCalsChannel_t *channels)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint32_t value;
	FUNCTION_ENTER(hw_priv);

	yxwRetAct = YXW_spiReadField(hw_priv, REG_CALI_RXIQ0_CTRL_0, &value, 21, 20);
	IF_ERR_RETURN(yxwRetAct);
	if(!channels)
		return YXW_ERR_NULL_POINTER;

	if(value == 0)
		*channels = YXW_CALS_CHANNEL_12;
	else if(value == 1)
		*channels = YXW_CALS_CHANNEL_1;
	else if(value == 2)
		*channels = YXW_CALS_CHANNEL_2;

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}
uint32_t YXW_runInitCals(YxwDevice_t *hw_priv,YxwInitCalibrations_t initCalMask)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwCmdTx *pMsg;

	YXW_Memset(&DataInfo,0,sizeof(YxwData));

	FUNCTION_ENTER(hw_priv);
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_RUNN_INIT_OPCODE;
	pMsg->TxData.value=initCalMask;
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	hw_priv->currentCalMask = initCalMask;
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_waitInitCalsDone(YxwDevice_t *hw_priv,uint32_t timeout)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t count_print = 0;
	uint32_t data;
	uint32_t i;

	FUNCTION_ENTER(hw_priv);
	/*Wait for initial calibration is already donw*/
	while (1)
	{
		YXW_mdelay(timeout);
		yxwRetAct=YXW_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(yxwRetAct);
		if (hw_priv->initcal_done == 1)
		{
			YXW_clearStatus(hw_priv);
			hw_priv->initcal_done=0;
			break;
		}

		count_print++;
		if(0 == (count_print%3))
		{
			YXW_DEBUG(DBG_LEVEL_STATUS,"..........\n");

			if(count_print > YXW_CALI_WIAT_TIMEOUT)
			{
				YXW_setWaitTimeoutBit(hw_priv, ACTION_SET_TIMEOUT_BIT, WAIT_CALS_DONE_TIMEOUT);
				yxwRetAct=YXW_ERR_TIMEOUT;
				YXW_DEBUG(DBG_LEVEL_STATUS,"waitCalsDone Timeout!\n");
				return (uint32_t)yxwRetAct;
			}
		}
	}

	YXW_setWaitTimeoutBit(hw_priv, ACTION_CLEAR_TIMEOUT_BIT, WAIT_CALS_DONE_TIMEOUT);
	YXW_DEBUG(DBG_LEVEL_STATUS,"waitCalsDone OK!\n");
	for(i = 0; i < 8; i++)
	{
		YXW_spiReadReg(hw_priv,0x10c + i,&data);
		YXW_spiWriteReg(hw_priv,0x150 + i,data);
	}
	YXW_spiWriteField(hw_priv,REG_CALI_ORXIQ0_CTRL_0,1,4,4);
	YXWHAL_UDelay(100);
	YXW_spiWriteField(hw_priv,REG_CALI_ORXIQ0_CTRL_0,0,4,4);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_waitInitCalsOutRingDone(YxwDevice_t *hw_priv,uint32_t timeout)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t count_print = 0;

	FUNCTION_ENTER(hw_priv);
	/*Wait for initial calibration is already donw*/
	while (1)
	{
		YXW_mdelay(timeout);
		yxwRetAct=YXW_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(yxwRetAct);
		if (hw_priv->initcal_done == 1)
		{
			YXW_clearStatus(hw_priv);
			hw_priv->initcal_done=0;
			break;
		}

		count_print++;
		if(0 == (count_print%3))
		{
			YXW_DEBUG(DBG_LEVEL_STATUS,"..........\n");

			if(count_print > YXW_CALI_OUT_RING_WIAT_TIMEOUT)
			{
				YXW_setWaitTimeoutBit(hw_priv, ACTION_SET_TIMEOUT_BIT, WAIT_CALS_OUT_RING_DONE_TIMEOUT);
				yxwRetAct=YXW_ERR_TIMEOUT;
				YXW_DEBUG(DBG_LEVEL_STATUS,"waitExtCalsDone Timeout!\n");
				return (uint32_t)yxwRetAct;
			}
		}
	}

	YXW_setWaitTimeoutBit(hw_priv, ACTION_CLEAR_TIMEOUT_BIT, WAIT_CALS_OUT_RING_DONE_TIMEOUT);
	YXW_DEBUG(DBG_LEVEL_STATUS,"waitExtCalsDone OK!\n");
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


const char* yxw_getInitCalsErrMsg(uint32_t errSrc, uint32_t errCode)
{
	if (errSrc == YXW_ERRHDL_INITCALS)
	{
		switch (errCode)
		{
		case RX1_DC_OFFSET_ERROR:
			return "RX1_DC_OFFSET Error Status ";
		case RX2_DC_OFFSET_ERROR:
			return "RX2 DC OFFSET Error Status";
		case RX1_QEC_INIT_ERROR:
			return "RX1 QEC INIT Error Status";
		case RX2_QEC_INIT_ERROR:
			return "RX2 QEC INIT Error Status";
		case ORX1_QEC_INIT_ERROR:
			return "ORX1 QEC INIT Error Status";
		case ORX2_QEC_INIT_ERROR:
			return "ORX2 QEC INIT Error Status";
		case TX1_LO_LEAKAGE_INTERNAL_ERROR:
			return "TX1 LO LEAKAGE INTERNAL Error Status";
		case TX2_LO_LEAKAGE_INTERNAL_ERROR:
			return "TX2 LO LEAKAGE INTERNAL Error Status";
		case TX1_LO_LEAKAGE_EXTERNAL_ERROR:
			return "TX1 LO LEAKAGE EXTERNAL Error Status";
		case TX2_LO_LEAKAGE_EXTERNAL_ERROR:
			return "TX2 LO LEAKAGE EXTERNAL Error Status";
		case TX1_QEC_INIT_ERROR:
			return "TX1 QEC INIT Error Status";
		case TX2_QEC_INIT_ERROR:
			return "TX2 QEC INIT Error Status";
		case YXW_BBPLL_LOCK_ERROR:
			return "BB PLL Calibration Error Status";
		case YXW_RFPLL_LOCK_ERROR:
			return "RF PLL Calibration Error Status";
		case YXW_AUXPLL_LOCK_ERROR:
			return "AUX PLL Calibration Error Status";
		case YXW_CALI_STATS_OK:
			return "YXW Calibration Status Ok";
		default:
			return "Unknown Init Calibration Error\n";
		}
	}
	return "Unknown Init Calibration Error\n";
}

uint32_t  YXW_getInitCalsStatus(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwMem *pMsg = NULL;
	struct elog_s *elog = NULL;
	unsigned short newCaLiEId;
	uint32_t i,elogCnt, isSendCaliId;
	uint32_t szCaLiEventId[CALI_EVENTID_CNT]={0x50, 0x10, 0x11, 0x12, 0x13, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x30, 0x31, 0x32};

	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;

	pMsg->Cmd=YXW_READ_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_CALSTATUS_MSGID;
	pMsg->Msg.Lenth=CALIBRITION_SRAM_LEN;/*CalibrationInfo Length*/
	pMsg->Msg.addr=CALIBRITION_SRAM_ADDR;/*CalibrationInfo ram*/

	yxwRetAct = YXW_sendRiscvCmd(hw_priv, (void *)&DataInfo, IOCTL_READ_CMD);
	if (yxwRetAct != YXW_NO_ACTION)
	{
		return YXW_ERR_CHECK_PARAM;
	}

	elog = (struct elog_s *)(pMsg->Msg.data);
	isSendCaliId = 0;
	for (newCaLiEId = elog->index - 1, elogCnt = 0; !isSendCaliId; newCaLiEId--, elogCnt++)
	{
		if (newCaLiEId >= elog->len)
		{
			newCaLiEId = elog->len - 1;
		}

		if (elog->e_id[newCaLiEId].valid != 1)
			continue;

		for (i = 0; i < CALI_EVENTID_CNT; i++)
		{
			if (elog->e_id[newCaLiEId].id == szCaLiEventId[i])
			{
				uint32_t errCode = ((elog->e_id[newCaLiEId].id<<24)|(elog->e_id[newCaLiEId].data & 0xffff));
				YXW_DEBUG(DBG_LEVEL_RESULT,"Calibration Results:%s\n",yxw_getInitCalsErrMsg(YXW_ERRHDL_INITCALS, errCode));
				isSendCaliId = 1;

				if(YXW_CALI_STATS_OK != errCode)
				{
					yxwRetAct=YXW_ERR_INIT_CALS;
				}
				break;
			}
		}

		if (elogCnt > 1024)
		{
			break;
		}
	}

	return yxwRetAct;
}

