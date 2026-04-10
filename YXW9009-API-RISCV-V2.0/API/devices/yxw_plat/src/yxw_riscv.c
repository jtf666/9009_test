/**
* \file yxw_riscv.c
* \brief Contains functions to support Y9009 Riscv Write and Read functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_riscv.h"

#include <stdio.h>
#include <unistd.h>

#include "firmware.h"
#include "yxw_error.h"
#include "yxw_jesd204b.h"
#include "yxw_plat_hal.h"
#include "yxw_radio.h"
#include "yxw_reg_dig.h"

static uint32_t yxw_disExtIntr(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	/*First,store the current interrupt status*/
	halStatus=YXW_spiReadReg(hw_priv,REG_EXT_INTR_ENA,&hw_priv->intr_status);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*it's need disable ext interrupt*/
	/*otherwise it maybe effect spi command*/
	halStatus=YXW_spiWriteReg(hw_priv,REG_EXT_INTR_ENA,0x0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

static uint32_t yxw_enaExtIntr(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	/*second,recover the current interrupt status*/
	halStatus=YXW_spiWriteReg(hw_priv,REG_EXT_INTR_ENA,hw_priv->intr_status);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

uint32_t YXW_clearStatus(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halStatus=YXW_spiWriteReg(hw_priv,DOCMD_STATUS,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}
uint32_t YXW_writeFirmware(YxwDevice_t *hw_priv,uint16_t addr, uint32_t data)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	halStatus=YXW_spiWriteReg(hw_priv,addr,data);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}


/*Read sram process*/
uint32_t YXW_spiReadMem(YxwDevice_t *hw_priv,uint32_t addr,uint32_t *regdata)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	if (addr & 0x20000000)
	{
		YXW_spiWriteField(hw_priv, REG_IP_SRAM_CTRL, 0, 3, 1);
	}
	else
	{
		YXW_spiWriteField(hw_priv, REG_IP_SRAM_CTRL, 1, 3, 1);
	}

	 /*read sram status*/
	halStatus=YXW_spiWriteReg(hw_priv,REG_IP_SRAM_ACC_ADDR,addr);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*2.read sram data*/
	halStatus=YXW_spiReadReg(hw_priv,SRAM_READ_DATA,regdata);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}
/*Write sram process*/
uint32_t YXW_spiWriteMem(YxwDevice_t *hw_priv,uint32_t addr,uint32_t data)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	halStatus=YXW_spiWriteReg(hw_priv,REG_IP_SRAM_ACC_WDATA,data);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Enable write func*/
	addr|=0x80000000;
	//printk("%x = %x\n",addr,data);
	halStatus=YXW_spiWriteReg(hw_priv,REG_IP_SRAM_ACC_ADDR,addr);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}
static uint32_t yxw_spiGetRiscvLog(YxwDevice_t *hw_priv,uint32_t addr,uint32_t *data)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	 /*read sram status*/
	halStatus=YXW_spiWriteReg(hw_priv,REG_IP_SRAM_ACC_ADDR,addr);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*2.read sram data*/
	halStatus=YXW_spiReadReg(hw_priv,SRAM_READ_DATA,data);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	return (uint32_t)yxwRetAct;
}

uint32_t yxw_spiInterrupt(YxwDevice_t *hw_priv,uint32_t writeData)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;
	FUNCTION_ENTER(hw_priv);
	/*request isr*/
	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,20,20);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_clearSpiWaitCmdConfirmStatus(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halStatus=YXW_spiWriteReg(hw_priv,RECV_CMD_CONFIRM,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);
	return yxwRetAct;
}


uint32_t YXW_spiWaitCmdConfirm(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;
	uint32_t readata;
	uint32_t uiTry = SPI_RESPONSE_WIAT_TIMEOUT;

	FUNCTION_ENTER(hw_priv);
	while(uiTry > 0){
		YXW_mdelay(100);
		halStatus=YXW_spiReadReg(hw_priv,RECV_CMD_CONFIRM,&readata);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		
		IF_ERR_RETURN(yxwRetAct);
		if(readata==HI_STATUS_SUCCESS){
			/*Clear Confirm From Chip Status*/
			YXW_clearSpiWaitCmdConfirmStatus(hw_priv);
			break;
		}
		uiTry--;
	}

	if(uiTry <= 0){
			
		YXW_setWaitTimeoutBit(hw_priv, ACTION_SET_TIMEOUT_BIT, WAIT_SPI_CMD_DONE_TIMEOUT);
		YXW_DEBUG(DBG_LEVEL_STATUS,"Timeout!\n\n");
		yxwRetAct=YXW_ERR_TIMEOUT;
		return (uint32_t)yxwRetAct;
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_getFirmwareVersion(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwMem *pMsg;

	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;

	pMsg->Cmd=YXW_READ_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_READFIRDY_MSGID;
	pMsg->Msg.Lenth=sizeof(YxwCaps_t);
	pMsg->Msg.addr=YXW_ADDR_FW_VERSION;
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_READ_CMD);
	IF_ERR_RETURN(yxwRetAct);

	YXW_Memset(&hw_priv->caps,0x0,sizeof(YxwCaps_t));
	YXW_Memcpy(&hw_priv->caps,(YxwCaps_t*)&pMsg->Msg.data[0],sizeof(YxwCaps_t));

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"*************Firmware BUILD INFO********\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"Compiler is :Jason\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"Current Firmware version is :1024\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"ChipId is :1206\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"Build Time is :2024_10_25\n");
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"****************************************\n\n");

	return (uint32_t)yxwRetAct;
}
static short yxw_calCheckSum(YxwDevice_t *hw_priv, void *data,int len)
{
	unsigned int acc;
	unsigned char src;
	char *octetptr;
	unsigned char ucXor=MAGIC;
	acc=0;

	FUNCTION_ENTER(hw_priv);
	octetptr=(char*)data;
	while(len>0){
		src=(*octetptr^ucXor);
		octetptr++;
		acc+=src;
		len--;
	}
	FUNCTION_EXIT(hw_priv);
	return acc;
}

static uint32_t yxw_downLoadFirmware(YxwDevice_t *hw_priv, uint8_t *data,uint32_t size,uint16_t addr)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;
	uint32_t DatLen = 0;
	uint32_t Data;

	FUNCTION_ENTER(hw_priv);
	for (DatLen = 0; DatLen < size ;DatLen += 4) {
		/*Little endian */
		Data=((data[DatLen]<<24)|data[DatLen+1]<<16|data[DatLen+2]<<8|data[DatLen+3]);
		yxwRetAct = YXW_writeFirmware(hw_priv,addr,Data);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
	}
	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}
uint32_t yxw_downLoadItcmBefore(YxwDevice_t *hw_priv,int writeData)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,4,4);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t yxw_downLoadDtcmBefore(YxwDevice_t *hw_priv,int writeData,int dtcmOffset)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,4,4);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,dtcmOffset,3,1);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus = YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,!writeData,8,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus = YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,!writeData,4,4);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
static uint32_t yxw_getFirmwareInfo(YxwDevice_t *hw_priv,struct firmware_Y9009 *fw_Y9009)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;
	uint16_t chcksum=0;
	uint32_t value=0;

	FUNCTION_ENTER(hw_priv);
	fw_Y9009->hdr.itcmLen=(fw_code[8]<<24|fw_code[9]<<16|fw_code[10]<<8|fw_code[11]);
	fw_Y9009->hdr.itcmAddr=(fw_code[0]<<24|fw_code[1]<<16|fw_code[2]<<8|fw_code[3]);
	fw_Y9009->fw_iccm=&fw_code[fw_Y9009->hdr.itcmAddr];

	chcksum+=yxw_calCheckSum(hw_priv, fw_Y9009->fw_iccm,fw_Y9009->hdr.itcmLen);
	fw_Y9009->hdr.dtcmLen=(fw_code[24]<<24|fw_code[25]<<16|fw_code[26]<<8|fw_code[27]);
	fw_Y9009->hdr.dtcmAddr=(fw_code[16]<<24|fw_code[17]<<16|fw_code[18]<<8|fw_code[19]);
	fw_Y9009->fw_dccm=&fw_code[fw_Y9009->hdr.dtcmAddr];
	chcksum+=yxw_calCheckSum(hw_priv, fw_Y9009->fw_dccm,fw_Y9009->hdr.dtcmLen);

	value=(MAGIC<<16|chcksum);
	halStatus=YXW_spiWriteReg(hw_priv,REG_IP_CHKSUM,value);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	/*Enalbe Checksum */
	halStatus=YXW_spiWriteField(hw_priv, REG_IP_SRAM_CTRL, 1, 16, 16);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	YXW_DEBUG(DBG_LEVEL_STATUS,"ITCM & DTCM len  %d  %d \r\n",(int)fw_Y9009->hdr.itcmLen,(int)fw_Y9009->hdr.dtcmLen);
	YXW_DEBUG(DBG_LEVEL_STATUS,"ITCM & DTCM Addr  %d  %d \r\n",(int)fw_Y9009->hdr.itcmAddr,(int)fw_Y9009->hdr.dtcmAddr);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}
static uint32_t yxw_downLoadItcm(YxwDevice_t *hw_priv,struct firmware_Y9009 *fw_Y9009)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halStatus = yxw_downLoadFirmware(hw_priv,fw_Y9009->fw_iccm,fw_Y9009->hdr.itcmLen,DOWNLOAD_ITCM_ADDR);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

static uint32_t yxw_downLoadDtcm(YxwDevice_t *hw_priv,struct firmware_Y9009 *fw_Y9009)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	yxwRetAct = yxw_downLoadFirmware(hw_priv,fw_Y9009->fw_dccm,fw_Y9009->hdr.dtcmLen,DOWNLOAD_DTCM_ADDR);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

static uint32_t yxw_downLoadAfter(YxwDevice_t *hw_priv,int writeData)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus=YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halStatus = YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,8,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus = YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,writeData,12,12);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiResetChip(hw_priv,RESET_BY_RESET);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_loadFirmware(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	struct firmware_Y9009 fw_Y9009;

	FUNCTION_ENTER(hw_priv);
	YXW_Memset(&fw_Y9009,0,sizeof(struct firmware_Y9009));

	yxwRetAct=yxw_getFirmwareInfo(hw_priv,&fw_Y9009);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct = yxw_downLoadDtcmBefore(hw_priv,1,0);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct = yxw_downLoadItcm(hw_priv,&fw_Y9009);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct = yxw_downLoadDtcmBefore(hw_priv,1,2);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct = yxw_downLoadDtcm(hw_priv,&fw_Y9009);
	IF_ERR_RETURN(yxwRetAct);

	yxwRetAct = yxw_downLoadAfter(hw_priv,1);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	YXW_DEBUG(DBG_LEVEL_STATUS,"FIRMWARE DOWNLOAD FINISH\n");
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_writeRiscvMem(YxwDevice_t *hw_priv,uint8_t *data,uint32_t addr,uint32_t size,uint32_t msgId,uint32_t offset)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t DatLen = 0;
	uint32_t Data;

	switch (msgId){
		case HI_MSG_TO_FILTER_MSGID:
			addr = FILTER_MSG_ADDR+offset;
			break;
		case HI_MSG_TO_PROFILE_MSGID:
			addr=PROFILE_MSG_ADDR;
			break;
		case HI_MSG_TO_RFFRQ_MSGID:
			addr=RFFRQ_MSG_ADDR;
			break;
		case HI_MSG_TO_AUXFRQ_MSGID:
			addr=AUXFRQ_MSG_ADDR;
			break;
		case HI_MSG_TO_J204B_MSGID:
			addr=J204B_MSG_ADDR;
			break;
		case HI_MSG_TO_HOPFREQ_MSGID:
			addr=HOPFREQ_MSG_ADDR+offset;
			break;
		default:
			break;
	}
	for(DatLen=0;DatLen<size;DatLen+=4){
		Data=((data[DatLen])|data[DatLen+1]<<8|data[DatLen+2]<<16|data[DatLen+3]<<24);
		yxwRetAct=YXW_spiWriteMem(hw_priv,addr,Data);
		IF_ERR_RETURN(yxwRetAct);
		addr+=4;
	}

	return (uint32_t)yxwRetAct;
}
uint32_t YXW_readRiscvMem(YxwDevice_t *hw_priv,uint32_t *data,uint32_t addr,uint32_t size)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t index;

	for(index=0;index<size;index+=4){
		yxwRetAct=YXW_spiReadMem(hw_priv,addr,data);
		IF_ERR_RETURN(yxwRetAct);
		addr+=4;
		data++;
	}

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_waitSpiInterruptResponseCompleted(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	yxwRetAct=yxw_spiInterrupt(hw_priv,1);
	IF_ERR_RETURN(yxwRetAct);

	/*Read Confirm From Chip*/
	yxwRetAct=YXW_spiWaitCmdConfirm(hw_priv);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

uint32_t YXW_sendRiscvCmd(YxwDevice_t *hw_priv,void *UserData, unsigned int Command)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;
	int MemData=0;
	YxwMsg *pMmsg=NULL;

	YxwData *DataInfo = (YxwData*)UserData;
	if(IOCTL_WRITE_CMD==Command)
	{
		/*Disable externer interrupt*/
		yxw_disExtIntr(hw_priv);
		switch(DataInfo->u.Tx.Cmd){
			case YXW_ABORT_OPCODE:
			case YXW_RUNN_INIT_OPCODE:
			case YXW_RADIO_ON_OPCODE:
				{
					halStatus=YXW_spiWriteReg(hw_priv,SEND_CMD_TYPE,DataInfo->u.Tx.Cmd);
					yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
					IF_ERR_RETURN(yxwRetAct);

					halStatus=YXW_spiWriteReg(hw_priv,SEND_CMD_DATA,DataInfo->u.Tx.TxData.value);
					yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
					IF_ERR_RETURN(yxwRetAct);
				}
				break;
			case YXW_WRITE_MEM_OPCODE:
				pMmsg=&DataInfo->u.Mmsg.Msg;
				MemData=(pMmsg->Id<<16|pMmsg->Lenth);
				halStatus=YXW_spiWriteReg(hw_priv,SEND_CMD_TYPE,DataInfo->u.Mmsg.Cmd);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halStatus=YXW_spiWriteReg(hw_priv,SEND_CMD_DATA,MemData);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halStatus=YXW_writeRiscvMem(hw_priv,&pMmsg->data[0],DOWNLOAD_DTCM_ADDR,pMmsg->Lenth,pMmsg->Id,pMmsg->offset);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
				break;
			case YXW_SET_OPCODE:
				halStatus=YXW_spiWriteReg(hw_priv,SEND_CMD_TYPE,DataInfo->u.Tx.Cmd);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halStatus=YXW_spiWriteReg(hw_priv,SEND_CMD_DATA,DataInfo->u.Tx.TxData.value);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);
				break;
		}

		yxwRetAct=YXW_waitSpiInterruptResponseCompleted(hw_priv);
		IF_ERR_RETURN(yxwRetAct);

	}
	else if(IOCTL_READ_CMD == Command)
	{
		switch(DataInfo->u.Tx.Cmd){
			case YXW_GET_OPCODE:
				/*Disable externer interrupt*/
				yxw_disExtIntr(hw_priv);
				halStatus=YXW_spiWriteReg(hw_priv,SEND_CMD_TYPE,DataInfo->u.Tx.Cmd);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				halStatus=YXW_spiWriteReg(hw_priv,SEND_CMD_DATA,DataInfo->u.Rx.RxData.value);
				yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
				IF_ERR_RETURN(yxwRetAct);

				yxwRetAct=YXW_waitSpiInterruptResponseCompleted(hw_priv);
				IF_ERR_RETURN(yxwRetAct);
				break;
			case YXW_READ_MEM_OPCODE:
				pMmsg=&DataInfo->u.Mmsg.Msg;
				MemData=(pMmsg->Id<<16|pMmsg->Lenth);
				yxwRetAct=YXW_readRiscvMem(hw_priv,(uint32_t *)(&pMmsg->data[0]),pMmsg->addr,pMmsg->Lenth);
				IF_ERR_RETURN(yxwRetAct);
				break;
			case YXW_GET_MEMORY_DATA:
				yxwRetAct=YXW_readRiscvMem(hw_priv,(uint32_t *)((size_t)DataInfo->u.Rx.RxData.lo),DataInfo->u.Rx.RxData.addr,DataInfo->u.Rx.RxData.value);
				IF_ERR_RETURN(yxwRetAct);
				break;
			default:
				break;
		}
	}

	return (uint32_t)yxwRetAct;
}


uint32_t YXW_writeRiscvProfile(YxwDevice_t *hw_priv,YxwInit_t *Init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t i = 0,j=0;
	yxwInfo_t *pStateInfo;
	YxwData DataInfo;
	YxwMem *pMsg;

	FUNCTION_ENTER(hw_priv);
	pStateInfo= &hw_priv->YxwState;
	YXW_Memset(&DataInfo,0,sizeof(YxwData));

	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_WRITE_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_PROFILE_MSGID;
	pMsg->Msg.Lenth=HI_MSG_TO_PROFILE_MSGLEN;
	for (i = 0; i < 4; i++)
	{
		pMsg->Msg.data[i] = (uint8_t)(((Init->clocks.deviceClock_kHz) >> (i * 8)) & 0x000000FF);
	}
	for (i = 4; i < 8; i++)
	{
		pMsg->Msg.data[i] = (uint8_t)(((Init->clocks.rfpll_frequency_Hz) >> ((i - 4) * 8)) & 0x000000FF);
	}
	pMsg->Msg.data[8] = ((Init->clocks.rfPllUseExternalLo > 0) ? 1 : 0);

	pMsg->Msg.data[9] = (Init->clocks.rfPllPhaseSyncMode & 0x03);
	pMsg->Msg.data[10]=0;//reserved
	pMsg->Msg.data[11]=0;//reserved
	if (pStateInfo->profilesValid & TX_PROFILE_VALID)
	{
		/* start of Tx profile data */
		pMsg->Msg.data[12] = Init->tx.txProfile.txFirBypass;
		pMsg->Msg.data[13] = Init->tx.txProfile.txFirInterpolation;
		pMsg->Msg.data[14] = Init->tx.txProfile.thb1Interpolation;
		pMsg->Msg.data[15] = Init->tx.txProfile.thb2Interpolation;
		pMsg->Msg.data[16] = Init->tx.txProfile.thb3Interpolation;
		pMsg->Msg.data[17]=0;//reserved
		pMsg->Msg.data[18]=0;//reserved
		pMsg->Msg.data[19]=0;//reserved
		for (i = 20; i < 24; i++)
		{
			pMsg->Msg.data[i] = (uint8_t)(((Init->tx.txProfile.txInputRate_kHz) >> ((i - 20) * 8)) & 0x000000FF);
		}

		for (i = 24; i < 28; i++)
		{
			pMsg->Msg.data[i] = (uint8_t)(((Init->tx.txProfile.primarySigBandwidth_Hz) >> ((i - 24) * 8)) & 0x000000FF);
		}

		for (i = 28; i < 32; i++)
		{
			pMsg->Msg.data[i] = (uint8_t)(((Init->tx.txProfile.rfBandwidth_Hz) >> ((i - 28) * 8)) & 0x000000FF);
		}
		for (i = 32; i < 36; i++)
		{
			pMsg->Msg.data[i] = (uint8_t)(((Init->tx.txProfile.txBbf3dBCorner_kHz) >> ((i - 32) * 8)) & 0x000000FF);
		}
	}else{
		/* clear pMsg->Msg.data[12] to pMsg->Msg.data[32] */
		for (j = 12; j < 36; j++)
		{
			pMsg->Msg.data[j] = 0;
		}
	}
	if (pStateInfo->profilesValid & RX_PROFILE_VALID)
	{
		/* start of Rx profile data */
		pMsg->Msg.data[36] = Init->rx.rxProfile.rxFirBypass;
		pMsg->Msg.data[37] = Init->rx.rxProfile.rxFirDecimation;
		pMsg->Msg.data[38] = Init->rx.rxProfile.rhb1Decimation;
		pMsg->Msg.data[39]=0;//pad

		for (i = 40; i < 44; i++)
		{
			pMsg->Msg.data[i] = (uint8_t)(((Init->rx.rxProfile.rxOutputRate_kHz) >> ((i - 40) * 8)) & 0x000000FF);
		}

		for (i = 44; i < 48; i++)
		{
			/* sig bw placeholder */
			pMsg->Msg.data[i] = (uint8_t)(((Init->rx.rxProfile.rfBandwidth_Hz) >> ((i - 44) * 8)) & 0x000000FF);
		}
		for (i = 48; i < 52; i++)
		{
			pMsg->Msg.data[i] = (uint8_t)(((Init->rx.rxProfile.rxBbf3dBCorner_kHz) >> ((i - 48) * 8)) & 0x000000FF);
		}
	}
	else
	{
		/* clear pMsg->Msg.data[32] to pMsg->Msg.data[48] */
		for (j = 36; j < 52; j++)
		{
			pMsg->Msg.data[j] = 0;
		}
	}
	if (pStateInfo->profilesValid & ORX_PROFILE_VALID)
	{
		/* start of ObsRx profile data */
		pMsg->Msg.data[52] = Init->obsRx.orxProfile.orxFirBypass;
		pMsg->Msg.data[53] = Init->obsRx.orxProfile.orxFirDecimation;
		pMsg->Msg.data[54] = 0;//pad
		pMsg->Msg.data[55] = 0;//pad

		for (i = 56; i < 60; i++)
		{
			pMsg->Msg.data[i] = (uint8_t)(((Init->obsRx.orxProfile.orxOutputRate_kHz) >> ((i - 56) * 8)) & 0x000000FF);
		}

		for (i = 60; i < 64; i++)
		{
			/* sig bw placeholder */
			pMsg->Msg.data[i] = (uint8_t)(((Init->obsRx.orxProfile.rfBandwidth_Hz) >> ((i - 60) * 8)) & 0x000000FF);
		}
		for (i = 64; i < 68; i++)
		{
			pMsg->Msg.data[i] = (uint8_t)(((Init->obsRx.orxProfile.rxBbf3dBCorner_kHz) >> ((i - 64) * 8)) & 0x000000FF);
		}
	}
	else
	{
		/* clear pMsg->Msg.data[48] to pMsg->Msg.data[64] */
		for (j = 52; j < 68; j++)
		{
			pMsg->Msg.data[j] = 0;
		}
	}
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	FUNCTION_EXIT(hw_priv);
	return (uint32_t)yxwRetAct;
}

uint32_t YXW_readRiscvProfile(YxwDevice_t *hw_priv,YxwInit_t *Init)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	uint32_t i = 0;
	yxwInfo_t *pStateInfo;
	YxwData DataInfo;
	YxwMem *pMsg;
	pStateInfo=&hw_priv->YxwState;
	FUNCTION_ENTER(hw_priv);

	pMsg=&DataInfo.u.Mmsg;

	DataInfo.CmdType=0xff;
	pMsg->Cmd=YXW_READ_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_PROFILE_MSGID;
	pMsg->Msg.Lenth=HI_MSG_TO_PROFILE_MSGLEN;
	pMsg->Msg.addr=PROFILE_MSG_ADDR;
	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_READ_CMD);
	IF_ERR_RETURN(yxwRetAct);

	YXW_Memset(Init, 0, sizeof(YxwInit_t));
	for (i = 0; i < 4; i++)
	{
		Init->clocks.deviceClock_kHz |= (pMsg->Msg.data[i]<<(i * 8));
	}


	for (i = 4; i < 8; i++)
	{
		Init->clocks.rfpll_frequency_Hz |= (pMsg->Msg.data[i]) << ((i - 4) * 8);
	}

	Init->clocks.rfPllUseExternalLo = pMsg->Msg.data[8];
	Init->clocks.rfPllPhaseSyncMode = (pMsg->Msg.data[9] & 0x03);


	//pMsg->Msg.data[10]=0;//reserved
	//pMsg->Msg.data[11]=0;//reserved
	if (pStateInfo->profilesValid & TX_PROFILE_VALID)
	{
		// start of Tx profile data
		Init->tx.txProfile.txFirBypass = pMsg->Msg.data[12];
		Init->tx.txProfile.txFirInterpolation = pMsg->Msg.data[13];
		Init->tx.txProfile.thb1Interpolation = pMsg->Msg.data[14];
		Init->tx.txProfile.thb2Interpolation = pMsg->Msg.data[15];
		Init->tx.txProfile.thb3Interpolation = pMsg->Msg.data[16];
		//pMsg->Msg.data[17]=0;//reserved
		//pMsg->Msg.data[18]=0;//reserved
		//pMsg->Msg.data[19]=0;//reserved
		for (i = 20; i < 24; i++)
		{
			Init->tx.txProfile.txInputRate_kHz |= (pMsg->Msg.data[i]) << ((i - 20) * 8);
		}

		for (i = 24; i < 28; i++)
		{
			Init->tx.txProfile.primarySigBandwidth_Hz |= (pMsg->Msg.data[i]) << ((i - 24) * 8);
		}

		for (i = 28; i < 32; i++)
		{
			Init->tx.txProfile.rfBandwidth_Hz |= (pMsg->Msg.data[i]) << ((i - 28) * 8);
		}

		for (i = 32; i < 36; i++)
		{
			Init->tx.txProfile.txBbf3dBCorner_kHz |= (pMsg->Msg.data[i]) << ((i - 32) * 8);
		}


	}
	if (pStateInfo->profilesValid & RX_PROFILE_VALID)
	{
		// start of Rx profile data
		Init->rx.rxProfile.rxFirBypass = pMsg->Msg.data[36];
		Init->rx.rxProfile.rxFirDecimation = pMsg->Msg.data[37];
		Init->rx.rxProfile.rhb1Decimation = pMsg->Msg.data[38];
		//pMsg->Msg.data[39]=0;//pad

		for (i = 40; i < 44; i++)
		{
			Init->rx.rxProfile.rxOutputRate_kHz |= (pMsg->Msg.data[i]) << ((i - 40) * 8);
		}

		for (i = 44; i < 48; i++)
		{
			// sig bw placeholder
			Init->rx.rxProfile.rfBandwidth_Hz |= (pMsg->Msg.data[i]) << ((i - 44) * 8);
		}

		for (i = 48; i < 52; i++)
		{
			Init->rx.rxProfile.rxBbf3dBCorner_kHz |= (pMsg->Msg.data[i]) << ((i - 48) * 8);
		}
	}

	if (pStateInfo->profilesValid & ORX_PROFILE_VALID)
	{
		// start of ObsRx profile data
		Init->obsRx.orxProfile.orxFirBypass = pMsg->Msg.data[52];
		Init->obsRx.orxProfile.orxFirDecimation = pMsg->Msg.data[53];
		//pMsg->Msg.data[50] = 0;//pad
		//pMsg->Msg.data[51] = 0;//pad

		for (i = 56; i < 60; i++)
		{
			Init->obsRx.orxProfile.orxOutputRate_kHz |= (pMsg->Msg.data[i]) << ((i - 56) * 8);
		}

		for (i = 60; i < 64; i++)
		{
			/// sig bw placeholder
			Init->obsRx.orxProfile.rfBandwidth_Hz |= (pMsg->Msg.data[i]) << ((i - 60) * 8);
		}
		for (i = 64; i < 68; i++)
		{
			Init->obsRx.orxProfile.rxBbf3dBCorner_kHz |= (pMsg->Msg.data[i]) << ((i - 64) * 8);
		}
	}
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t  YXW_waitFirmwareReady(YxwDevice_t *hw_priv, uint32_t timeout)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t uiTry = WIAT_TIMEOUT;

	FUNCTION_ENTER(hw_priv);
	/*Wait for Riscv is already running*/
	while(uiTry > 0){
		YXW_mdelay(timeout);
		yxwRetAct=YXW_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(yxwRetAct);
		if(hw_priv->caps.firmwareReady==1){
			YXW_clearStatus(hw_priv);
			hw_priv->caps.firmwareReady=0;
			break;
		}
		uiTry--;
	}
	if(uiTry <= 0){
		YXW_setWaitTimeoutBit(hw_priv, ACTION_SET_TIMEOUT_BIT, WAIT_FIRMWARE_READY_TIMEOUT);
		yxwRetAct=YXW_ERR_TIMEOUT;
		YXW_DEBUG(DBG_LEVEL_STATUS,"waitFirmwareReady TimeOut!\n");
		return (uint32_t)yxwRetAct;
	}
	YXW_setWaitTimeoutBit(hw_priv, ACTION_CLEAR_TIMEOUT_BIT, WAIT_FIRMWARE_READY_TIMEOUT);
	YXW_DEBUG(DBG_LEVEL_STATUS,"waitFirmwareReady OK!\n");
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}
uint32_t  YXW_waitProfileDone(YxwDevice_t *hw_priv, uint32_t timeout)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	uint32_t uiTry = WIAT_TIMEOUT;

	FUNCTION_ENTER(hw_priv);
	/*Wait for profile is already done*/
	while(uiTry > 0){
		YXW_mdelay(timeout);
		yxwRetAct=YXW_spiGetCmdStatus(hw_priv);
		IF_ERR_RETURN(yxwRetAct);
		if(hw_priv->profile_done==1){
			YXW_clearStatus(hw_priv);
			hw_priv->profile_done=0;
			break;
		}
		uiTry--;
	}

	if(uiTry <= 0){
		YXW_setWaitTimeoutBit(hw_priv, ACTION_SET_TIMEOUT_BIT, WAIT_PROFILE_DONE_TIMEOUT);
		yxwRetAct=YXW_ERR_TIMEOUT;
		YXW_DEBUG(DBG_LEVEL_STATUS,"waitProfile TimeOut!\n");
		return (uint32_t)yxwRetAct;
	}
	YXW_setWaitTimeoutBit(hw_priv, ACTION_CLEAR_TIMEOUT_BIT, WAIT_PROFILE_DONE_TIMEOUT);
	YXW_DEBUG(DBG_LEVEL_STATUS,"waitProfile OK!\n");
	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_GetRiscvTraceEvent(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	YxwData DataInfo;
	YxwMem *pMsg = NULL;
	struct elog_s *elog = NULL;
	unsigned short newTraceEventId;
	uint32_t elogCnt;
	pMsg=&DataInfo.u.Mmsg;
	DataInfo.CmdType=0xff;
	YXW_DEBUG(DBG_LEVEL_ALWAYS,"|-------------GetRiscvTraceEventInfoshow--------------|\n");


	pMsg->Cmd=YXW_READ_MEM_OPCODE;
	pMsg->Msg.Id=HI_MSG_TO_CALSTATUS_MSGID;
	pMsg->Msg.Lenth=CALIBRITION_SRAM_LEN;/*CalibrationInfo Length*/
	pMsg->Msg.addr=CALIBRITION_SRAM_ADDR;/*CalibrationInfo ram*/

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_READ_CMD);
	if(yxwRetAct!=YXW_NO_ACTION){
		return YXW_ERR_CHECK_PARAM;
	}

	elog = (struct elog_s *)(pMsg->Msg.data);

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"TraceEvent	Id\n");
	for (newTraceEventId = elog->index-1, elogCnt = 0; elogCnt < ELOG_LEN; elogCnt++)
	{
		if(newTraceEventId >= ELOG_LEN)
			break;

		if (elog->e_id[newTraceEventId].valid)
		{
			YXW_DEBUG(DBG_LEVEL_ALWAYS,"TraceEventId %d  = 0x%x\n",newTraceEventId,((elog->e_id[newTraceEventId].id<<24)|(elog->e_id[newTraceEventId].data & 0xffff)));
			YXW_DEBUG(DBG_LEVEL_ALWAYS,"TraceEventId %d  = 0x%x\n",newTraceEventId,((elog->e_id[newTraceEventId].data & 0xffffffff)));
		}

		if(0 == newTraceEventId)
		{
			newTraceEventId = elog->len-1;
		}
		else
		{
			newTraceEventId--;
		}
	}

	return yxwRetAct;
}

uint32_t YXW_enableRiscvLog(YxwDevice_t *hw_priv,uint32_t debug)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	YxwData DataInfo;
	YxwCmdTx *pMsg=NULL;
	DataInfo.CmdType=0xff;
	pMsg=&DataInfo.u.Tx;

	FUNCTION_ENTER(hw_priv);
	pMsg->Cmd=YXW_SET_OPCODE;

	pMsg->TxData.value=(((debug &0xffff)<<16)|(HI_MSG_ENABLE_LOG_MSGID));

	yxwRetAct=YXW_sendRiscvCmd(hw_priv,(void*)&DataInfo,IOCTL_WRITE_CMD);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}



uint32_t YXW_spiGetCmdStatus(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;
	uint32_t ReaData;

	FUNCTION_ENTER(hw_priv);
	halStatus=YXW_spiReadReg(hw_priv,DOCMD_STATUS,&ReaData);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	if(ReaData&BIT(HI_STATUS_FIRMWARE_READY_BIT))
	{
		yxwRetAct=YXW_getFirmwareVersion(hw_priv);
		hw_priv->caps.firmwareReady=1;
	}
	if(ReaData&BIT(RFPLL_CALI_DONE_BIT))
	{
		hw_priv->rfpll_done=1;
	}
	if(ReaData&BIT(PHY_INIT_CALI_DONE_BIT))
	{
		hw_priv->initcal_done=1;
	}
	if(ReaData&BIT(PHY_INIT_PROFILE_DONE_BIT))
	{
		hw_priv->profile_done=1;
	}
	if(ReaData&BIT(JESD204B_DONE_BIT))
	{
		hw_priv->j204b_done=1;
	}
	if(ReaData&BIT(DBG_2SPI_DONE_BIT))
	{
		hw_priv->dbg_done=1;
	}
	if(ReaData&BIT(GPIO_GET_VALUE_DONE_BIT))
	{
		hw_priv->gpio_get_value_done=1;
	}

	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_spiResetChip(YxwDevice_t *hw_priv,uint8_t RestType)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	yxwRetAct=yxw_spiInterrupt(hw_priv,RestType);
	IF_ERR_RETURN(yxwRetAct);
	return (uint32_t)yxwRetAct;
}
uint32_t YXW_getBootStatus(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halstatus = YXWHAL_OK;
	uint32_t regValue=0;
	uint32_t uiTry = WIAT_TIMEOUT;

	FUNCTION_ENTER(hw_priv);
	/*Wait for Boot Ready*/
	printf("BZ_getBootStatus_after/r/n");
	while(uiTry > 0){
		halstatus=YXW_spiReadReg(hw_priv,REG_IP_SRAM_CTRL,&regValue);
		printf("0x%x ", regValue);
		yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halstatus,yxwRetAct,YXW_ERR_RESET_SPI);
		IF_ERR_RETURN(yxwRetAct);
		if(regValue==BOOT_RDY){
			YXW_DEBUG(DBG_LEVEL_STATUS,"Boot OK\n\n");
			break;
		}
		uiTry--;
		YXW_mdelay(1000);
	}
	if(uiTry <= 0){
		YXW_DEBUG(DBG_LEVEL_STATUS,"Boot Timeout!\n\n");
		yxwRetAct=YXW_ERR_TIMEOUT;
		return (uint32_t)yxwRetAct;
	}

	FUNCTION_EXIT(hw_priv);

	return (uint32_t)yxwRetAct;
}

uint32_t YXW_recoverCpu(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	yxwHalErr_t halStatus = YXWHAL_OK;

	FUNCTION_ENTER(hw_priv);
	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,0x1,7,7);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_IP_SRAM_CTRL,0x0,7,7);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_0,0x0,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_CLK_SWAP_0,0x1,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);

	halStatus=YXW_spiWriteField(hw_priv,REG_CALI_CTRL_0,0x0,0,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RISCV,halStatus,yxwRetAct,YXW_ERR_RESET_SPI);
	IF_ERR_RETURN(yxwRetAct);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}


uint32_t YXW_getSdkVersion(void)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	struct build_info build;
	YXW_Memset(&build, 0, sizeof(struct build_info));




	YXWHAL_Printf("*************SDK BUILD INFO********\n");
	YXWHAL_Printf("version:yxw9009_01\n");
	YXWHAL_Printf("submit_date:2024_10_25\n");
	YXWHAL_Printf("compiler:jason\n");
	YXWHAL_Printf("compile_date:2024_10_25\n");
	YXWHAL_Printf("****************************************\n\n");
	return yxwRetAct;
}


uint32_t YXW_setWaitTimeoutBit(YxwDevice_t *hw_priv, uint32_t action, uint32_t timeout_bit)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	FUNCTION_ENTER(hw_priv);
	switch(action){
		case ACTION_SET_TIMEOUT_BIT:
			hw_priv->waitTimeout |= timeout_bit;
			break;
		case ACTION_CLEAR_TIMEOUT_BIT:
			hw_priv->waitTimeout &= ~timeout_bit;
			break;
		default:
			YXW_DEBUG(DBG_LEVEL_ERR,"No action find.\n");
			break;
	}
	yxw_enaExtIntr(hw_priv);
	FUNCTION_EXIT(hw_priv);

	return yxwRetAct;
}

uint32_t YXW_getWaitTimeoutBit(YxwDevice_t *hw_priv)
{
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;

	YXW_DEBUG(DBG_LEVEL_ALWAYS,"statusBit=0x%x\n",hw_priv->waitTimeout);

	return yxwRetAct;
}

