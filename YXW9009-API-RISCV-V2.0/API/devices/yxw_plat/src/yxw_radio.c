/**
* \file yxw_radio.c
* \brief Contains functions to support Yxw radio control and pin control
 *        functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_error.h"
#include "yxw_radio.h"

#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_error.h"
#include "yxw_plat_hal.h"
#include "yxw_reg_ana.h"
#include "yxw_reg_dig.h"
#include "yxw_riscv.h"


uint32_t YXW_setPllFrequency(YxwDevice_t *hw_priv,uint32_t name,uint64_t pllLoFreq_Hz)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwMem *pMsg;

	FUNCTION_ENTER(hw_priv);
	YXW_Memset(&DataInfo,0,sizeof(YxwData));

	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_WRITE_MEM_OPCODE;
     YXW_spiWriteReg(hw_priv,0x17,0xe2f87bd8);
	if(name == YXW_RF_PLL)
	{
		pMsg->Msg.Id=HI_MSG_TO_RFFRQ_MSGID;

		pMsg->Msg.Lenth=HI_MSG_TO_RFFRQ_MSGLEN;
		/*Write RfPllFreq to SRAM*/
		/*long byte array with parsed bytes from rfPllLoFrequency_Hz word */
		pMsg->Msg.data[0] = (uint8_t)(pllLoFreq_Hz & 0xFF);
		pMsg->Msg.data[1] = (uint8_t)((pllLoFreq_Hz >> 8) & 0xFF);
		pMsg->Msg.data[2] = (uint8_t)((pllLoFreq_Hz >> 16) & 0xFF);
		pMsg->Msg.data[3] = (uint8_t)((pllLoFreq_Hz >> 24) & 0xFF);
		pMsg->Msg.data[4] = (uint8_t)((pllLoFreq_Hz >> 32) & 0xFF);
		pMsg->Msg.data[5] = (uint8_t)((pllLoFreq_Hz >> 40) & 0xFF);
		pMsg->Msg.data[6] = (uint8_t)((pllLoFreq_Hz >> 48) & 0xFF);
		pMsg->Msg.data[7] = (uint8_t)((pllLoFreq_Hz >> 56) & 0xFF);
	}
	else if(name == YXW_AUX_PLL)
	{
		if(pllLoFreq_Hz >= 6000000000)
		{
			YXW_DEBUG(DBG_LEVEL_ERR,"auxpllFreq_Hz is error,exceed 4G!\n");
			return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,
						YXW_ERR_INV_NULL_INIT_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
		}
		pMsg->Msg.Id=HI_MSG_TO_AUXFRQ_MSGID;
		pMsg->Msg.Lenth=HI_MSG_AUXFRQ_MSGLEN;
		/*Write AUXPllFreq to SRAM*/
		/*long byte array with parsed bytes from rfPllLoFrequency_Hz word */
		pMsg->Msg.data[0] = (uint8_t)(pllLoFreq_Hz & 0xFF);
		pMsg->Msg.data[1] = (uint8_t)((pllLoFreq_Hz >> 8) & 0xFF);
		pMsg->Msg.data[2] = (uint8_t)((pllLoFreq_Hz >> 16) & 0xFF);
		pMsg->Msg.data[3] = (uint8_t)((pllLoFreq_Hz >> 24) & 0xFF);
		pMsg->Msg.data[4] = (uint8_t)((pllLoFreq_Hz >> 32) & 0xFF);
		pMsg->Msg.data[5] = (uint8_t)((pllLoFreq_Hz >> 40) & 0xFF);
		pMsg->Msg.data[6] = (uint8_t)((pllLoFreq_Hz >> 48) & 0xFF);
		pMsg->Msg.data[7] = (uint8_t)((pllLoFreq_Hz >> 56) & 0xFF);
	}
	/*Construct the cmdto fw*/
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getPllFrequency(YxwDevice_t *hw_priv,uint32_t name,uint64_t *pllLoFreq_Hz)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;
	uint32_t freq_low_addr	 = 0;
	uint32_t freq_high_addr  = 0;
	uint32_t freq_low_value  = 0;
	uint32_t freq_high_value = 0;
	uint8_t *freq_data = 0;

	if (NULL == pllLoFreq_Hz)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"pllLoFreq_Hz point is NULL!\n");
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,
					YXW_ERR_INV_NULL_INIT_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}
	*pllLoFreq_Hz = 0;

	if(name == YXW_RF_PLL)
	{
		freq_low_addr  = REG_GC_39;
		freq_high_addr = REG_GC_40;
	}
	else if(name == YXW_AUX_PLL)
	{
		freq_low_addr  = REG_GC_41;
		freq_high_addr = REG_GC_42;
	}

	halStatus=YXW_spiReadReg(hw_priv,freq_low_addr,&freq_low_value);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiReadReg(hw_priv,freq_high_addr,&freq_high_value);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);



	/*Write RfPllFreq to SRAM*/
	/*long byte array with parsed bytes from rfPllLoFrequency_Hz word */

	freq_data = (uint8_t *)&freq_low_value;
	*pllLoFreq_Hz =  ((uint64_t)(freq_data[0]& 0xFF))<<0;
	*pllLoFreq_Hz |= ((uint64_t)(freq_data[1]& 0xFF))<<8;
	*pllLoFreq_Hz |= ((uint64_t)(freq_data[2]& 0xFF))<<16;
	*pllLoFreq_Hz |= ((uint64_t)(freq_data[3]& 0xFF))<<24;

	freq_data = (uint8_t *)&freq_high_value;
	*pllLoFreq_Hz |= ((uint64_t)(freq_data[0]& 0xFF))<<32;
	*pllLoFreq_Hz |= ((uint64_t)(freq_data[1]& 0xFF))<<40;
	*pllLoFreq_Hz |= ((uint64_t)(freq_data[2]& 0xFF))<<48;
	*pllLoFreq_Hz |= ((uint64_t)(freq_data[3]& 0xFF))<<56;

	return (uint32_t)yxwRetAct;
}



uint32_t YXW_setRadioCtrlSpiMode(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel,YxwObsRxChannels_t OrxChannel,YxwTxChannels_t txChannel)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halstatus=YXW_spiWriteField(hw_priv,REG_CH_ENA,0x0,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_1,0x1f,31,24);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv,REG_ENA_EXT_0,0x3f,5,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 31, 0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0, 1, 0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0, 7, 4);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	switch (txChannel)
	{
		case YXW_TXOFF:
			/*close TX1/TX2: TXFE2_LOBUF_PD=1,TXFE1_LOBUF_PD=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0xFFF, 11, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0xF, 19, 16);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX1:
			// Enable TX1
			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x2, 1, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x2, 1, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_TXLPF0, 0x1, 8, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_TXLPF3, 0x0, 8, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x1, 9, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX2:
			// Enable TX2
			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x1, 1, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x1, 1, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_TXLPF0, 0x0, 8, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_TXLPF3, 0x1, 8, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x2, 9, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX1TX2:
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 9, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x3, 1, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x0, 1, 0);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_TXLPF0, 0x1, 8, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			halstatus=YXW_spiWriteField(hw_priv, REG_TXLPF3, 0x1, 8, 8);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}

	switch(rxChannel)
	{
		case YXW_RXOFF:
			/*close RX1/RX2: RXFE2_LOBUF_PD=1,RXFE1_LOBUF_PD=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x3, 13, 12);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			/*close RX1: RX1_RX_ENN2=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE6, 0x7, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			/*close RX2: RX2_RX_ENN2=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE14, 0x7, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX1:
			/*Enable RX1 RXFE1_LOBUF_PD=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x1, 13, 12);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x2, 5, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x2, 5, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE6, 0x0, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE14, 0x7, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX2:
			/*Enable RX2 RXFE2_LOBUF_PD=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x2, 13, 12);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x1, 5, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x1, 5, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE14, 0x0, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE6, 0x7, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX1RX2:
			/*Enable RX1/RX2 RXFE2_LOBUF_PD=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 13, 12);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x3, 5, 4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_SYNC_1, 0x0, 5,4);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE6, 0x0, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE14, 0x0, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}

	switch(OrxChannel){
		case YXW_ORXOFF:
			/*PD SAR1 & SAR2*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x3, 15, 14);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*close ORX1/ORX2*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE5, 0x7, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE13, 0x7, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX1:
			/*PD  SAR2*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x1, 15, 14);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*RX1_ORX_ENN2=0*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE5, 0, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			/*RX2_ORX_ENN2=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE13, 0x7, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x2, 7, 6);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_1,0x0,15,15);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX2:
			/*PD SAR1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x2, 15, 14);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*RX1_ORX_ENN2=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE5, 0x7, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			/*RX2_ORX_ENN2=0*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE13, 0, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x1, 7, 6);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_1,0x1,15,15);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX1ORX2:
			/*PD SAR1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_PD0, 0x0, 15, 14);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			/*RX1_ORX_ENN2=1*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE5, 0x0, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);
			/*RX2_ORX_ENN2=0*/
			halstatus=YXW_spiWriteField(hw_priv, REG_RXFE13, 0x0, 21, 19);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			halstatus=YXW_spiWriteField(hw_priv, REG_RSTN0, 0x3, 7, 6);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
			IF_ERR_RETURN(yxwRetAct);

			break;
		default:
			break;
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_getRadioCtrlSpiMode(YxwDevice_t *hw_priv, YxwRxChannels_t *rxChannel,YxwObsRxChannels_t *OrxChannel,YxwTxChannels_t *txChannel)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;

	uint32_t fieldVal1 = 0;
	uint32_t fieldVal2 = 0;
	halStatus=YXW_spiReadField(hw_priv,REG_PD0,&fieldVal1,9,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	switch (fieldVal1)
	{
		case YXW_BIT0BIT1_Mask:
			/*TXFE1_LOBUF_PD=1*/
			/*TXFE2_LOBUF_PD=1*/
			*txChannel= YXW_TXOFF;
			break;
		case YXW_BIT0_Mask:
			/*TXFE1_LOBUF_PD=0*/
			/*TXFE2_LOBUF_PD=1*/
			*txChannel= YXW_TX1;
			break;
		case YXW_BIT1_Mask:
			/*TXFE1_LOBUF_PD=1*/
			/*TXFE2_LOBUF_PD=0*/
			*txChannel= YXW_TX2;
			break;
		case YXW_OFF_Mask:
			/*TXFE1_LOBUF_PD=0*/
			/*TXFE2_LOBUF_PD=0*/
			*txChannel= YXW_TX1TX2;
			break;
		default:
			break;
	}
	fieldVal1 = 0;
	fieldVal2 = 0;
	halStatus=YXW_spiReadField(hw_priv,REG_RXFE6,&fieldVal1,19,19);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiReadField(hw_priv,REG_RXFE14,&fieldVal2,19,19);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	int fieldVal_mask = ((fieldVal1<<1)|fieldVal2);
	switch(fieldVal_mask)
	{
		case YXW_BIT0BIT1_Mask:
			/*RX1_RX_ENN2=1*/
			/*RX2_RX_ENN2=1*/
			*rxChannel = YXW_RXOFF;
			break;
		case YXW_BIT0_Mask:
			/*RX1_RX_ENN2=0*/
			/*RX2_RX_ENN2=1*/
			*rxChannel = YXW_RX1;
			break;
		case YXW_BIT1_Mask:
			/*RX1_RX_ENN2=1*/
			/*RX2_RX_ENN2=0*/
			*rxChannel = YXW_RX2;
			break;
		case YXW_OFF_Mask:
			/*RX1_RX_ENN2=0*/
			/*RX2_RX_ENN2=0*/
			*rxChannel = YXW_RX1RX2;
			break;
		default:
			break;
	}

	fieldVal1 = 0;
	fieldVal2 = 0;
	halStatus=YXW_spiReadField(hw_priv,REG_RXFE5,&fieldVal1,19,19);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiReadField(hw_priv,REG_RXFE13,&fieldVal2,19,19);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	fieldVal_mask = ((fieldVal1<<1)|fieldVal2);
	switch(fieldVal_mask){
		case YXW_BIT0BIT1_Mask:
			/*RX1_RX_ENN2=1*/
			/*RX2_RX_ENN2=1*/
			*OrxChannel = YXW_ORXOFF;
			break;
		case YXW_BIT0_Mask:
			/*RX1_ORX_ENN2=0*/
			/*RX2_ORX_ENN2=1*/
			*OrxChannel = YXW_ORX1;
			break;
		case YXW_BIT1_Mask:
			/*RX1_ORX_ENN2=1*/
			/*RX2_ORX_ENN2=0*/
			*OrxChannel = YXW_ORX2;
			break;
		case YXW_OFF_Mask:
			/*RX1_ORX_ENN2=0*/
			/*RX2_ORX_ENN2=0*/
			*OrxChannel = YXW_ORX1ORX2;
			break;
		default:
			break;
	}
	return (uint32_t)yxwRetAct;
}
uint32_t YXW_setRadioCtrlPinMode(YxwDevice_t *hw_priv, YxwRxChannels_t rxChannel,YxwObsRxChannels_t OrxChannel,YxwTxChannels_t txChannel)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halStatus=YXW_spiWriteField(hw_priv,REG_CH_ENA,0x21922,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_1,0x53,31,24);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*
	 *RX/TX control mode: pin mode
	 *ext_tx1_ena = 0
	 *ext_tx2_ena = 0
	 *ext_rx1_ena = 0
	 *ext_rx2_ena = 0
	 *ext_orx1_ena = 0
	 *ext_orx2_ena = 0
	 */
	halStatus=YXW_spiWriteField(hw_priv, REG_ENA_EXT_0,0x0,5,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*disable TX/RX channel, by gpio*/
	halStatus = YXW_Tx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
	IF_ERR_RETURN(yxwRetAct);
	halStatus = YXW_Tx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
	IF_ERR_RETURN(yxwRetAct);
	halStatus = YXW_Rx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
	IF_ERR_RETURN(yxwRetAct);
	halStatus = YXW_Rx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
	IF_ERR_RETURN(yxwRetAct);
	halStatus = YXW_ORx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
	IF_ERR_RETURN(yxwRetAct);
	halStatus = YXW_ORx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
	IF_ERR_RETURN(yxwRetAct);

	switch(txChannel) {
		case YXW_TXOFF:
			/*disable TX1/TX2 channel*/
			halStatus = YXW_Tx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_Tx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX1:
			/*enable TX1 channel*/
			halStatus = YXW_Tx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_Tx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX2:
			/*enable TX2 channel*/
			halStatus = YXW_Tx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_Tx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_TX1TX2:
			/*enable TX1/TX2 channel*/
			halStatus = YXW_Tx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_Tx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}

	switch(rxChannel) {
		case YXW_RXOFF:
			/*disable RX1/RX2 channel*/
			halStatus = YXW_Rx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_Rx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX1:
			/*enable RX1 channel*/
			halStatus = YXW_Rx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_Rx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX2:
			/*enable RX2 channel*/
			halStatus = YXW_Rx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_Rx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_RX1RX2:
			/*enable RX1/RX2 channel*/
			halStatus = YXW_Rx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_Rx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}

	switch(OrxChannel) {
		case YXW_ORXOFF:
			/*disable ORX1/ORX2 channel*/
			halStatus = YXW_ORx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_ORx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX1:
			/*disable ORX1 channel*/
			halStatus = YXW_ORx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_ORx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX2:
			/*disable ORX2 channel*/
			halStatus = YXW_ORx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_DISABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_ORx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		case YXW_ORX1ORX2:
			/*enable ORX1/ORX2 channel*/
			halStatus = YXW_ORx1EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			halStatus = YXW_ORx2EnableCtrl(hw_priv, YXWHAL_CHANNEL_ENABLE);
			yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_GPIO);
			IF_ERR_RETURN(yxwRetAct);
			break;
		default:
			break;
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_getRadioCtrlMode(YxwDevice_t *hw_priv,YxwRadioCtlMode_t* radioCtrlMode)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;
	uint32_t regValue=0;

	if (NULL == radioCtrlMode)
	{
		YXW_DEBUG(DBG_LEVEL_ERR,"%s,%d,radioCtrlMode point is NULL!\n",__FUNCTION__,__LINE__);
		return (uint32_t)YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,
					YXW_ERR_DEVICE_INIT_NULL_PARAM,yxwRetAct,YXW_ERR_CHECK_PARAM);
	}
	*radioCtrlMode = YXW_ERR_MODE;

	halStatus=YXW_spiReadField(hw_priv,REG_ENA_EXT_0,&regValue,5,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if(0x0 == regValue)
	{
		*radioCtrlMode = YXW_PIN_MODE;
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"Path Ctrl Mode:\033[1;32;40mPIN Mode.\033[0m\n");
	}
	else if(0x3f == regValue)
	{
		*radioCtrlMode = YXW_SPI_MODE;
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"Path Ctrl Mode:\033[1;32;40mSPI Mode.\033[0m\n");
	}
	else
	{
		*radioCtrlMode = YXW_ERR_MODE;
		YXW_DEBUG(DBG_LEVEL_ALWAYS,"Path Ctrl Mode:\033[1;32;40mERR Mode.\033[0m\n");
	}

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_setChannelSeq(YxwDevice_t *hw_priv, YxwChannelSeq_t channelSeq)
{
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	if(channelSeq == YXW_CHANNEL_SEQ_NONE)
		yxwRetAct = YXW_spiWriteField(hw_priv, REG_CALI_RXIQ0_CTRL_0, 0, 12, 11);
	else if(channelSeq == YXW_CHANNEL_SEQ_ORX_TO_RX)//12 -> 1
		yxwRetAct = YXW_spiWriteField(hw_priv, REG_CALI_RXIQ0_CTRL_0, 2, 12, 11);
	else if(channelSeq == YXW_CHANNEL_SEQ_RX_TO_ORX)//11 -> 1
		yxwRetAct = YXW_spiWriteField(hw_priv, REG_CALI_RXIQ0_CTRL_0, 1, 12, 11);
	IF_ERR_RETURN(yxwRetAct);

	{
		YxwData DataInfo;
		YxwCmdTx *pMsg;
		YXW_Memset(&DataInfo, 0, sizeof(YxwData));

		DataInfo.CmdType = 0xff;
		pMsg=&DataInfo.u.Tx;
		pMsg->Cmd = YXW_SET_OPCODE;
		pMsg->TxData.value=(((channelSeq &0xffff)<<16)|(HI_SET_SPECIAL_MODE));

		yxwRetAct = YXW_sendRiscvCmd(hw_priv, (void*)&DataInfo, IOCTL_WRITE_CMD);
		IF_ERR_RETURN(yxwRetAct);
	}

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_getChannelSeq(YxwDevice_t *hw_priv, YxwChannelSeq_t *channelSeq)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t value;
	FUNCTION_ENTER(hw_priv);

	yxwRetAct = YXW_spiReadField(hw_priv, REG_CALI_RXIQ0_CTRL_0, &value, 12, 11);
	IF_ERR_RETURN(yxwRetAct);

	if(!channelSeq)
		return YXW_ERR_NULL_POINTER;

	if(value == 0)
		*channelSeq = YXW_CHANNEL_SEQ_NONE;
	else if(value == 1)
		*channelSeq = YXW_CHANNEL_SEQ_RX_TO_ORX;
	else if(value == 2)
		*channelSeq = YXW_CHANNEL_SEQ_ORX_TO_RX;

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_radioOn(YxwDevice_t * hw_priv, YxwRadioCtlMode_t Mode)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint8_t txChannel = 0;
	uint8_t rxOrxChannel = 0;
	uint8_t rxChannel=0;

	FUNCTION_ENTER(hw_priv);
	txChannel= hw_priv->YxwState.supportChannels &3;
	rxChannel= (hw_priv->YxwState.supportChannels >>2)&3;
	rxOrxChannel= (hw_priv->YxwState.supportChannels >>4)&3;

	/*Enable channel*/
	if(Mode==YXW_SPI_MODE){
		yxwRetAct=YXW_setRadioCtrlSpiMode(hw_priv,rxChannel,rxOrxChannel,txChannel);
		IF_ERR_RETURN(yxwRetAct);
	}else{
		yxwRetAct=YXW_setRadioCtrlPinMode(hw_priv,rxChannel,rxOrxChannel,txChannel);
		IF_ERR_RETURN(yxwRetAct);
	}

	hw_priv->YxwState.devState=YXW_STATE_RADIOON;
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_radioOff(YxwDevice_t * hw_priv,YxwRadioCtlMode_t Mode)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	/*Disable channel*/
	if(Mode==YXW_SPI_MODE){
		yxwRetAct=YXW_setRadioCtrlSpiMode(hw_priv,YXW_RXOFF,YXW_ORXOFF,YXW_TXOFF);
		IF_ERR_RETURN(yxwRetAct);
	}else{
		yxwRetAct=YXW_setRadioCtrlPinMode(hw_priv,YXW_RXOFF,YXW_ORXOFF,YXW_TXOFF);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_waitPllsLockDone(YxwDevice_t *hw_priv, uint32_t timeout)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t count_print = 0;

	FUNCTION_ENTER(hw_priv);
	/*Wait for pll is already done*/
	while(1){
		YXW_mdelay(timeout);
		yxwRetAct=YXW_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(yxwRetAct);

		if(hw_priv->rfpll_done==1){
			YXW_clearStatus(hw_priv);
			hw_priv->rfpll_done=0;
			break;
		}

		count_print++;
		{
			YXW_DEBUG(DBG_LEVEL_STATUS,"..........\n");
		}
		if(count_print > YXW_RADIO_PLL_WIAT_TIMEOUT)
		{
			YXW_setWaitTimeoutBit(hw_priv, ACTION_SET_TIMEOUT_BIT, WAIT_PLLS_LOCK_DONE_TIMEOUT);
			yxwRetAct=YXW_ERR_TIMEOUT;
			YXW_DEBUG(DBG_LEVEL_STATUS,"waitPllsLockDone TimeOut!\n");
			return (uint32_t)yxwRetAct;
		}
	}

	YXW_setWaitTimeoutBit(hw_priv, ACTION_CLEAR_TIMEOUT_BIT, WAIT_PLLS_LOCK_DONE_TIMEOUT);

	YXW_DEBUG(DBG_LEVEL_STATUS,"waitPllsLockDone OK!\n");
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_getRadioState(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;
	YxwRadioCtlMode_t radioCtrlMode = YXW_ERR_MODE;
	uint32_t radioStatus = 0;
	uint32_t regValue = 0;
	uint8_t tx1Enable = 0;
	uint8_t tx2Enable = 0;
	uint8_t rx1Enable = 0;
	uint8_t rx2Enable = 0;
	uint8_t orx1Enable = 0;
	uint8_t orx2Enable = 0;
	char *pathStatus[2]       = {"\033[1;31;40mOFF\033[0m" , "\033[1;32;40mON \033[0m"};

	halStatus=YXW_spiReadReg(hw_priv, REGOUT_CALI_DONE_0,&radioStatus);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if((radioStatus&TX1ENMASK) == TX1ENMASK)	{	tx1Enable =  1;	}
	if((radioStatus&TX2ENMASK) == TX2ENMASK)	{	tx2Enable =  1;	}
	if((radioStatus&RX1ENMASK) == RX1ENMASK)	{	rx1Enable =  1;	}
	if((radioStatus&RX2ENMASK) == RX2ENMASK)	{	rx2Enable =  1;	}
	if((radioStatus&ORX1ENMASK) == ORX1ENMASK)	{	orx1Enable = 1;	}
	if((radioStatus&ORX2ENMASK) == ORX2ENMASK)	{	orx2Enable = 1;	}

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"|---------------------PathInfoshow--------------------|\n");

	yxwRetAct=YXW_getRadioCtrlMode(hw_priv, &radioCtrlMode);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiReadReg(hw_priv, REG_CLK_SWAP_1,&regValue);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"Donald Trump=\033[1;32;40m0x%x\033[0m\n", (regValue >> 20));

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"Path        TX1    TX2    RX1    RX2     ORX1    ORX2\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"Status      %s    %s    %s    %s     %s     %s\n", pathStatus[tx1Enable], pathStatus[tx2Enable],
		pathStatus[rx1Enable], pathStatus[rx2Enable], pathStatus[orx1Enable], pathStatus[orx2Enable]);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"\n");

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_setPllTemperatureTrackEnable(YxwDevice_t *hw_priv,uint32_t enable){
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo; YxwCmdTx *pMsg;
	YXW_Memset(&DataInfo,0,sizeof(YxwData));
	pMsg=&DataInfo.u.Tx;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_SET_OPCODE;

	FUNCTION_ENTER(hw_priv);
	/*Send cmd to riscv change gaintable to normal digital gain mode*/
	DataInfo.u.Tx.TxData.value=(HI_ENABLE_PLLTC_TRACK|(enable<<16));
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;

}

uint32_t YXW_pllmode(YxwDevice_t *hw_priv, YxwInit_t *Init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	switch(Init->mode)
	{
	 case 0:
		 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK1_1, LO_RF, 0, 0);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK2_1, LO_RF, 0, 0);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE4, LO_RF, 2, 1);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE12, LO_RF, 2, 1);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 break;
	 case 1:
		 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK1_1, LO_RF, 0, 0);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK2_1, LO_RF, 0, 0);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE4, LO_AUX, 2, 1);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE12, LO_AUX, 2, 1);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 break;
	 case 2:
		 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK1_1, LO_RF, 0, 0);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK2_1, LO_AUX, 0, 0);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE4, LO_RF, 2, 1);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE12, LO_AUX, 2, 1);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);

		halStatus=YXW_spiWriteField(hw_priv,0x3E,0x80732000,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x44,0x3882bf,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x41,0xc0123ffe,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x45,0xf3d08e00,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x46,0x80732000,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x43,0x10082bf,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x4C,0x3882bf,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x49,0xC0123ffe,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x50,0xC000,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus = YXW_spiWriteField(hw_priv, 0xA9, 2, 12, 11);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus = YXW_spiWriteField(hw_priv, 0x1C6, 0xf000000, 31, 0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 break;
	 case 3:
		 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK1_1, LO_RF, 0, 0);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK2_1, LO_AUX, 0, 0);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE4, LO_RF, 2, 1);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);
		 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE12, LO_AUX, 2, 1);
		 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 IF_ERR_RETURN(yxwRetAct);

		halStatus=YXW_spiWriteField(hw_priv,0x3E,0x80732000,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x44,0x3882bf,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x41,0xc0123ffe,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x45,0xf3d08e00,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x46,0x80732000,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x43,0x10082bf,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x4C,0x3882bf,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x49,0xC0123ffe,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus=YXW_spiWriteField(hw_priv,0x50,0xC000,31,0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus = YXW_spiWriteField(hw_priv, 0xA9, 2, 12, 11);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

		halStatus = YXW_spiWriteField(hw_priv, 0x1C6, 0xf000000, 31, 0);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		 break;
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_orx_rx(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK1_1, LO_RF, 0, 0);
	 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	 IF_ERR_RETURN(yxwRetAct);
	 halStatus=YXW_spiWriteField(hw_priv, REG_LPBK2_1, LO_RF, 0, 0);
	 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	 IF_ERR_RETURN(yxwRetAct);
	 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE4, LO_AUX, 2, 1);
	 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	 IF_ERR_RETURN(yxwRetAct);
	 halStatus=YXW_spiWriteField(hw_priv, REG_RXFE12, LO_AUX, 2, 1);
	 yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	 IF_ERR_RETURN(yxwRetAct);
	halStatus=YXW_spiWriteField(hw_priv,0x44,0x3882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x4C,0x3882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x50,0xC000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x43,0x10082bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x45 ,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x82 ,0x42e7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x83 ,0x42e70000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x4B,0x10082bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x49,0x40123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x4A,0x4002,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x4D,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x85,0x42e7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x86 ,0x42e70000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x1C6,0xD000000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_rx0A_orx0D(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	
	//关A_RX2
	halStatus=YXW_spiWriteField(hw_priv,0x46,0x80732000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x43,0x10082bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x4C,0x3882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x49,0xc0123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x50,0xC000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	//关A_ORx1/A_ORx2
	halStatus=YXW_spiWriteField(hw_priv,0x43,0x10882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x45,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x82,0xC2e7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x83,0xC2e70000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x4B,0x10882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x49,0xC0123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x4A,0xC00C,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x4D,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x85,0xC2e7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x86,0xC2e70000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	//关A_Tx2_TX1
	halStatus=YXW_spiWriteField(hw_priv,0x50,0xF7F0F,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x51,0xFFBF,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
/*
	halStatus=YXW_spiWriteField(hw_priv,0x46,0x602000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x43,0x82bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x4C,0x13882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x49,0x10123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x45,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x82,0x4107,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x43,0x41070000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x4B,0x13882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x4A,0x4000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x4D,0xf3d08e000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x85,0x4107,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x86,0x41070000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	halStatus=YXW_spiWriteField(hw_priv,0x50,0xF5FFF,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	halStatus=YXW_spiWriteField(hw_priv,0x51,0xFFAC,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
   */
	//关数字ORX+Tx2+Rx2
	halStatus=YXW_spiWriteField(hw_priv,0x1C6,0x14000000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);

	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

