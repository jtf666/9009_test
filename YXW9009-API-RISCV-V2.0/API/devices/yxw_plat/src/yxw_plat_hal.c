/**
* \file yxw_plat_hal.c
* \brief Contains Y9009 API Hardware Abstraction Layer (HAL) functions
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#include "yxw_plat_hal.h"

#include "yxw_hal.h"

// yxwHalErr_t YXW_spiOpen(YxwDevice_t *hw_priv,uint32_t DeviceId)
// {
// 	yxwHalErr_t halStatus = YXWHAL_OK;
// 	yxwHalHandle SpiHandle=NULL;

// 	YXWHAL_SpiOpen(DeviceId,&SpiHandle);
// 	hw_priv->devHalInfo=(void*)SpiHandle;
// 	if(hw_priv->devHalInfo==NULL)
// 	{
// 		YXW_DEBUG(DBG_LEVEL_ERR,"hw_priv->devHalInfo is NULL\n");
// 	}
// 	return halStatus;
// }
// yxwHalErr_t YXW_spiClose(YxwDevice_t *hw_priv)
// {
// 	yxwHalErr_t halStatus = YXWHAL_OK;

// 	halStatus =YXWHAL_SpiClose(hw_priv->devHalInfo);
// 	return halStatus;
// }

yxwHalErr_t YXW_spiReadReg(YxwDevice_t *hw_priv, uint16_t addr, uint32_t *data)
{
	yxwHalErr_t halStatus = YXWHAL_OK;

	/*customer read interface*/
	halStatus=YXWHAL_SpiReadReg(hw_priv->devHalInfo,addr, data);
	YXWHAL_UDelay(20);
	return halStatus;
}
yxwHalErr_t YXW_spiWriteReg(YxwDevice_t *hw_priv, uint16_t addr,uint32_t data)
{
	yxwHalErr_t halStatus = YXWHAL_OK;

	/*customer write interface*/
	halStatus=YXWHAL_SpiWriteReg(hw_priv->devHalInfo,addr, data);
	return halStatus;
}
yxwHalErr_t YXW_spiReadField(YxwDevice_t *hw_priv,uint16_t addr,uint32_t *fieldVal,uint8_t endBit,uint8_t startBit)
{
	yxwHalErr_t halStatus = YXWHAL_OK;
	uint32_t regdata=0;
	uint32_t regmask=0;

	regmask = ~((1<<startBit) -1);
	regmask &= ((1<<endBit) -1)|(1<<endBit);
	halStatus=YXW_spiReadReg(hw_priv,addr,&regdata);
	regdata &= regmask;
	regdata >>=  startBit;
	*fieldVal = regdata;
	return halStatus;
}
yxwHalErr_t YXW_spiWriteField(YxwDevice_t *hw_priv,uint16_t addr,uint32_t fieldVal,uint8_t endBit,uint8_t startBit)
{
	yxwHalErr_t halStatus = YXWHAL_OK;
	uint32_t regdata=0;
	uint32_t regmask=0;

	halStatus=YXW_spiReadReg(hw_priv,addr,&regdata);

	regmask = ~((1<<startBit) -1);
	regmask &= ((1<<endBit) -1)|(1<<endBit);
	regdata &= ~regmask;
	regdata |= (fieldVal <<startBit)&regmask;
	halStatus=YXW_spiWriteReg(hw_priv,addr,regdata);
	return halStatus;
}

yxwHalErr_t YXW_mdelay(uint32_t mdelay_time)
{
	yxwHalErr_t halStatus = YXWHAL_OK;

	YXWHAL_MDelay(mdelay_time);
	return halStatus;
}

yxwHalErr_t YXW_WriteToLog(YxwDevice_t *hw_priv,yxwLogLevel_t loglevel,uint32_t errorCode,const char *comment)
{
	yxwHalErr_t halStatus = YXWHAL_OK;

	halStatus=YXWHAL_WriteToLog(hw_priv->devHalInfo,loglevel,errorCode,comment);
	return halStatus;

}
yxwHalErr_t YXW_Memset(void* Addr,uint32_t Data,uint32_t len)
{
	yxwHalErr_t halStatus = YXWHAL_OK;

	memset(Addr,Data,len);
	return halStatus;
}
yxwHalErr_t YXW_Memcpy(void* dst,void *src,uint32_t len)
{
	yxwHalErr_t halStatus = YXWHAL_OK;

	memcpy(dst,src,len);
	return halStatus;

}
yxwHalErr_t YXW_resetHw(uint32_t DeviceId)
{
	yxwHalErr_t halStatus = YXWHAL_OK;

	//halStatus = YXWHAL_ResetHw(DeviceId);
	return halStatus;
}

/*Api to control pin enable, When the Tx/RX channel enable mode is GPIO mode*/
yxwHalErr_t YXW_Tx1EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode)
{
	yxwHalErr_t halStatus = YXWHAL_OK;
	halStatus = YXWHAL_Tx1EnableCtrl(hw_priv->devHalInfo, mode);
	return halStatus;
}

yxwHalErr_t YXW_Tx2EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode)
{
	yxwHalErr_t halStatus = YXWHAL_OK;
	halStatus = YXWHAL_Tx2EnableCtrl(hw_priv->devHalInfo, mode);
	return halStatus;
}

yxwHalErr_t YXW_Rx1EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode)
{
	yxwHalErr_t halStatus = YXWHAL_OK;
	halStatus = YXWHAL_Rx1EnableCtrl(hw_priv->devHalInfo, mode);
	return halStatus;
}

yxwHalErr_t YXW_Rx2EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode)
{
	yxwHalErr_t halStatus = YXWHAL_OK;
	halStatus = YXWHAL_Rx2EnableCtrl(hw_priv->devHalInfo, mode);
	return halStatus;
}

yxwHalErr_t YXW_ORx1EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode)
{
	yxwHalErr_t halStatus = YXWHAL_OK;
	halStatus = YXWHAL_ORx1EnableCtrl(hw_priv->devHalInfo, mode);
	return halStatus;
}

yxwHalErr_t YXW_ORx2EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode)
{
	yxwHalErr_t halStatus = YXWHAL_OK;
	halStatus = YXWHAL_ORx2EnableCtrl(hw_priv->devHalInfo, mode);
	return halStatus;
}


