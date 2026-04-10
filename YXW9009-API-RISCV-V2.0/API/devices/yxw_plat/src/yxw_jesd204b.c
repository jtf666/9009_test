/**
* \file yxw_jesd204.c
* \brief Contains functions to support Yxw JESD204b data interface
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_error.h"
#include "yxw_jesd204b.h"

#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_plat_hal.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"


uint32_t YXW_writeJes204bConfig(YxwDevice_t *hw_priv,YxwInit_t *Init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwMem *pMsg;
	YxwJesdSettings_t jesd204Config;
	YxwJesdSettings_t *pjesd204Config = &jesd204Config;

	FUNCTION_ENTER(hw_priv);
	pMsg=&DataInfo.u.Mmsg;
	memset(&DataInfo,0,sizeof(DataInfo));

	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_WRITE_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_J204B_MSGID;
	pMsg->Msg.Lenth=sizeof(YxwJesdSettings_t);
	pMsg->Msg.addr=J204B_MSG_ADDR;

	pjesd204Config = (YxwJesdSettings_t *)pMsg->Msg.data;
	memcpy(pjesd204Config, &Init->jesd204Settings, sizeof(YxwJesdSettings_t));

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_waitJes204bConfigDone(YxwDevice_t *hw_priv, uint32_t timeout)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t uiTry = WIAT_TIMEOUT;

	FUNCTION_ENTER(hw_priv);
	/*Wait for JESD204B interface synchronization*/
	while (uiTry > 0)
	{
		YXW_mdelay(timeout);
		yxwRetAct=YXW_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(yxwRetAct);
		if (hw_priv->j204b_done == 1)
		{
			YXW_clearStatus(hw_priv);
			hw_priv->j204b_done=0;
			break;
		}
		uiTry--;
	}

	if (uiTry <= 0)
	{

		YXW_setWaitTimeoutBit(hw_priv, ACTION_SET_TIMEOUT_BIT, WAIT_JES204B_CONFIG_DONE_TIMEOUT);
		yxwRetAct=YXW_ERR_TIMEOUT;
		YXW_DEBUG(DBG_LEVEL_STATUS,"waitJesd204Done Timeout!\n");
		return (uint32_t)yxwRetAct;
	}

	YXW_setWaitTimeoutBit(hw_priv, ACTION_CLEAR_TIMEOUT_BIT, WAIT_JES204B_CONFIG_DONE_TIMEOUT);
	YXW_DEBUG(DBG_LEVEL_STATUS,"waitJesd204Done OK!\n");
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_GetJes204bSysrefControlStatus(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halError=YXWHAL_OK;
	uint32_t fieldVal = 0;
	char *jes204bSysrefStatus[2] = {"ON" , "OFF"};

	halError=YXW_spiReadField(hw_priv, REG_SYSREF0,&fieldVal,15,15);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_JES204B,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	YXW_DEBUG(DBG_LEVEL_RESULT,"YxwJesd204bSysrefStatus: %s\n", jes204bSysrefStatus[fieldVal]);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_SetPrbs15_SystemTx(YxwDevice_t *hw_priv,uint32_t enable)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	YxwData DataInfo;
	YxwCmdTx *pMsg=NULL;
	DataInfo.CmdType=0xff;
	pMsg=&DataInfo.u.Tx;
	pMsg->Cmd=YXW_SET_OPCODE;
	pMsg->TxData.value=(((enable &0xffff)<<16)|(HI_GET_PRBS15_SYSTEM_TX));

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	return yxwRetAct;
}

uint32_t YXW_GetPrbs15_SystemTxInfo(YxwDevice_t *hw_priv,uint32_t *enable)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t regValue = 0;
	uint32_t lane_recv_data_count = 0;
	uint32_t lane_error_count = 0;

	yxwRetAct=YXW_spiReadMem(hw_priv, 0x25006134, &regValue);
	IF_ERR_RETURN(yxwRetAct);

	*enable = regValue&0x0f;

	if((*enable)&(0x01<<0))
	{
		//lane0
		yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_LAN0_PRBS15_RCVALLCONT, &lane_recv_data_count);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_LAN0_PRBS15_RCVERRCONT, &lane_error_count);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_RESULT,"lane0_recv_data_count=%u, lane0_error_count=%u\n", lane_recv_data_count, lane_error_count);
	}

	if((*enable)&(0x01<<1))
	{
		//lane1
		yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_LAN1_PRBS15_RCVALLCONT, &lane_recv_data_count);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_LAN1_PRBS15_RCVERRCONT, &lane_error_count);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_RESULT,"lane1_recv_data_count=%u, lane1_error_count=%u\n", lane_recv_data_count, lane_error_count);
	}

	if((*enable)&(0x01<<2))
	{
		//lane2
		yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_LAN2_PRBS15_RCVALLCONT, &lane_recv_data_count);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_LAN2_PRBS15_RCVERRCONT, &lane_error_count);
		IF_ERR_RETURN(yxwRetAct);
		YXW_DEBUG(DBG_LEVEL_RESULT,"lane2_recv_data_count=%u, lane2_error_count=%u\n", lane_recv_data_count, lane_error_count);
	}

	if((*enable)&(0x01<<3))
	{
		//lane3
		yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_LAN3_PRBS15_RCVALLCONT, &lane_recv_data_count);
		IF_ERR_RETURN(yxwRetAct);
		yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_LAN3_PRBS15_RCVERRCONT, &lane_error_count);
		IF_ERR_RETURN(yxwRetAct);

		YXW_DEBUG(DBG_LEVEL_RESULT,"lane3_recv_data_count=%u, lane3_error_count=%u\n", lane_recv_data_count, lane_error_count);
	}

	return yxwRetAct;
}



uint32_t YXW_SetPrbs15_SystemRxORx(YxwDevice_t *hw_priv,uint32_t enable)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	YxwData DataInfo;
	YxwCmdTx *pMsg=NULL;
	DataInfo.CmdType=0xff;
	pMsg=&DataInfo.u.Tx;
	pMsg->Cmd=YXW_SET_OPCODE;
	pMsg->TxData.value=(((enable &0xffff)<<16)|(HI_SET_PRBS15_SYSTEM_RX_ORX));

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	return yxwRetAct;
}

uint32_t YXW_GetPrbs15_SystemRxORx_EnableStatus(YxwDevice_t *hw_priv,uint32_t *rx_enable,uint32_t *orx_enable)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t regValue = 0;

	yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_RX_PRBS15_ENABLE, &regValue);
	IF_ERR_RETURN(yxwRetAct);
	*rx_enable = regValue&0x03;

	yxwRetAct=YXW_spiReadMem(hw_priv, JESD204B_ORX_PRBS15_ENABLE, &regValue);
	IF_ERR_RETURN(yxwRetAct);
	*orx_enable = regValue&0x03;


	return yxwRetAct;
}
uint32_t YXW_spiGetJesdDebugData(YxwDevice_t *hw_priv, uint32_t store_addr, uint32_t sample_point, uint32_t sample_rate,uint32_t iq_sel,uint32_t length)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;
	uint32_t debug_data = 0;
	uint32_t i;

	if(length > SAMPLE_MAX_LENGTH)
	{
		length = SAMPLE_MAX_LENGTH;
	}

	/*Disable jesdb Err intr*/
	halStatus=YXW_spiWriteField(hw_priv,REG_EXT_INTR_ENA,0,18,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,iq_sel,26,26);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_0,1,17,17);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if(sample_rate==0) //125Mhz
	{
		halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,0,18,17);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	if(sample_rate==1){ //245Mhz
		halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,1,18,17);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	if(sample_rate==2){ //491Mhz
		halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,2,18,17);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}

	/*2.disable store sram addr*/
	halStatus=YXW_spiWriteField(hw_priv,REG_PC_BASE_ADDR,0,31,31);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if(sample_point <= 32){
		halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,sample_point,31,27);
		halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,0,23,23);
	}
	else
	{
		halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,0,31,27);
		halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,1,23,23);
	}
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*1,set debug_cntcnt*/
	halStatus=YXW_spiWriteReg(hw_priv,REG_PC_COUNT,length);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*2.set debug data store sram addr*/
	halStatus=YXW_spiWriteField(hw_priv,REG_PC_BASE_ADDR,store_addr,28,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*2.Enable store sram addr*/
	halStatus=YXW_spiWriteField(hw_priv,REG_PC_BASE_ADDR,1,31,31);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);


	halStatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_0,0,17,17);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_EXT_INTR_ENA,0x7,18,16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	YXW_DEBUG(DBG_LEVEL_RESULT,"store_addr=0x%x, length=%d\n\n", store_addr, length);
	for(i = 0; i < length; i++)
	{
		YXW_spiReadMem(hw_priv, store_addr, &debug_data);
		if(0 == i%4) YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n%08x: ", ((i/4)*16));

		YXW_DEBUG(DBG_LEVEL_ALWAYS,"%08x ", debug_data);
		store_addr += 4;
	}
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_enableFramerLink(YxwDevice_t *hw_priv,YxwFramerSel_t framerSel,uint32_t enable)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwCmdTx *pMsg=NULL;

	FUNCTION_ENTER(hw_priv);
	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;

	hw_priv->framer_status |= (enable << framerSel);

	if ((framerSel != YXW_FRAMER_A) && (framerSel != YXW_FRAMER_B) && (framerSel != YXW_FRAMER_A_AND_B))
	{
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_JES204B,
				YXW_ERR_VERTORXPFILE_RXINPUTRATE,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}

	if ((framerSel == YXW_FRAMER_A) ||
		(framerSel == YXW_FRAMER_A_AND_B))
	{
		pMsg->TxData.value=(HI_ENABLE_FRAMER_A|(enable<<16));
	}

	if ((framerSel == YXW_FRAMER_B) ||
		(framerSel == YXW_FRAMER_A_AND_B))
	{
		pMsg->TxData.value=(HI_ENABLE_FRAMER_B|(enable<<16));
	}

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_enableDeframerLink(YxwDevice_t *hw_priv,YxwDeframerSel_t deframerSel,uint32_t enable)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwCmdTx *pMsg=NULL;

	FUNCTION_ENTER(hw_priv);

	hw_priv->deframer_status |= enable;

	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;
	pMsg->TxData.value=(HI_ENABLE_DFRAMER|(enable<<16));

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_enableSysRefToFramer(YxwDevice_t *hw_priv,YxwInit_t *Init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwCmdTx *pMsg=NULL;

	FUNCTION_ENTER(hw_priv);
	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;
	pMsg->TxData.value=ENABLE_SYSREF_INIT;
	if(Init->clocks.rfPllPhaseSyncMode){
		yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t  YXW_getSyncStatus(YxwDevice_t * hw_priv,uint32_t *TypeStatus)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint32_t RegData = 0;


	*TypeStatus = 0;
	yxwRetAct = YXW_spiReadField(hw_priv, REGOUT_J204BRX00, &RegData, 31, 0);
	IF_ERR_RETURN(yxwRetAct);


	if (yxwRetAct != YXW_NO_ACTION)
	{
		return YXW_ERR_CHECK_PARAM;
	}

	if (0x03 != (RegData & 0x03))
	{
		*TypeStatus = TX_DEFRAMER_NOT_SYNC; // TX DeFramer
	}
	if (0x04 != (RegData & 0x04))
	{
		*TypeStatus |= RX_FRAMER_NOT_SYNC; // RX Framer
	}
	if (0x08 != (RegData & 0x08))
	{
		*TypeStatus |= ORX_FRAMER_NOT_SYNC; // ORX Framer
	}


	return yxwRetAct;
}

uint32_t YXW_waitSync204b(YxwDevice_t *hw_priv, uint32_t timeout)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t TypeStatus = 0;
	uint32_t uiTry = 0;

	FUNCTION_ENTER(hw_priv);
	/*Wait for JESD204B sync*/
	{
		while(uiTry < WIAT_204B_COUNT)
		{
			YXW_mdelay(timeout);
			yxwRetAct = YXW_getSyncStatus(hw_priv, &TypeStatus);
			IF_ERR_RETURN(yxwRetAct);

			if((hw_priv->framer_status >> YXW_FRAMER_A)	&&
				(RX_FRAMER_NOT_SYNC == ((TypeStatus)&RX_FRAMER_NOT_SYNC)))
			{
				yxwRetAct = YXW_enableFramerLink(hw_priv, YXW_FRAMER_A, 0);
				IF_ERR_RETURN(yxwRetAct);

				YXW_mdelay(RESET_204B_TIME);
				yxwRetAct = YXW_enableFramerLink(hw_priv, YXW_FRAMER_A, 1);
				IF_ERR_RETURN(yxwRetAct);
			}
			else if((hw_priv->framer_status >> YXW_FRAMER_B)	&&
			(ORX_FRAMER_NOT_SYNC == ((TypeStatus)&ORX_FRAMER_NOT_SYNC)))
			{
				yxwRetAct = YXW_enableFramerLink(hw_priv, YXW_FRAMER_B, 0);
				IF_ERR_RETURN(yxwRetAct);

				YXW_mdelay(RESET_204B_TIME);
				yxwRetAct = YXW_enableFramerLink(hw_priv, YXW_FRAMER_B, 1);
				IF_ERR_RETURN(yxwRetAct);
			}
			else if((hw_priv->deframer_status)	&&
					(TX_DEFRAMER_NOT_SYNC == ((TypeStatus)&TX_DEFRAMER_NOT_SYNC)))
			{
				yxwRetAct = YXW_enableDeframerLink(hw_priv, YXW_DEFRAMER, 0);
				IF_ERR_RETURN(yxwRetAct);

				YXW_mdelay(RESET_204B_TIME);
				yxwRetAct = YXW_enableDeframerLink(hw_priv, YXW_DEFRAMER, 1);
				IF_ERR_RETURN(yxwRetAct);
			}
			else
			{
				YXW_DEBUG(DBG_LEVEL_STATUS,"wait 204b rx and orx sync ok.(uiTry=%d)\n",uiTry);
				break;
			}
			uiTry++;
		}

		if (uiTry >= WIAT_204B_COUNT)
		{
			YXW_setWaitTimeoutBit(hw_priv, ACTION_SET_TIMEOUT_BIT, WAIT_JES204B_SYNC_TIMEOUT);
			YXW_DEBUG(DBG_LEVEL_STATUS,"waitSync204 Timeout!\n\n");
			yxwRetAct=YXW_NO_ACTION;
			return (uint32_t)yxwRetAct;
		}
	}
	YXW_setWaitTimeoutBit(hw_priv, ACTION_CLEAR_TIMEOUT_BIT, WAIT_JES204B_SYNC_TIMEOUT);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;

}
uint32_t YXW_setLaneSequence(YxwDevice_t *hw_priv, YxwJesdLaneSeq_t TxlaneSeq,YxwJesdLaneSeq_t RxORxlaneSeq)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwCmdTx *pMsg=NULL;

	FUNCTION_ENTER(hw_priv);
	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;
	pMsg->TxData.value=(HI_SET_LANE_SEQ|(TxlaneSeq<<16|RxORxlaneSeq<<24));

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;

}
uint32_t YXW_getLaneSequence(YxwDevice_t *hw_priv,YxwJesdLaneSeq_t *TxLaneSeq,YxwJesdLaneSeq_t *RxOrxLaneSeq)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwCmdTx *pMsg=NULL;

	FUNCTION_ENTER(hw_priv);
	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;
	pMsg->TxData.value=(HI_GET_LANE_SEQ);

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_spiReadField(hw_priv,JESD_LANE_SEQ,(uint32_t*)TxLaneSeq,15,0);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct=YXW_spiReadField(hw_priv,JESD_LANE_SEQ,(uint32_t*)RxOrxLaneSeq,31,16);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_setByteSequence(YxwDevice_t *hw_priv, YxwJesdByteSeq_t tx,
   YxwJesdByteSeq_t rx, YxwJesdByteSeq_t orx)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
	uint32_t mask = tx | (rx << 4) | (orx << 8);

	FUNCTION_ENTER(hw_priv);

	yxwRetAct=YXW_spiWriteField(hw_priv, REG_J_SEL_4, mask, 12, 0);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getByteSequence(YxwDevice_t *hw_priv, YxwJesdByteSeq_t *tx,
   YxwJesdByteSeq_t *rx, YxwJesdByteSeq_t *orx)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t mask;

	FUNCTION_ENTER(hw_priv);

	yxwRetAct = YXW_spiReadField(hw_priv, REG_J_SEL_4, &mask, 12, 0);
	IF_ERR_RETURN(yxwRetAct);

	if(tx)
		*tx = mask & 0xF;
	if(rx)
		*rx = (mask >> 4) & 0xF;
	if(orx)
		*orx = (mask >> 8) & 0xF;
	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}
