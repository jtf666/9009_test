

#define  ZLYXW_PLATFORM
//#define  ZYNQ_PLATFORM

#ifdef ZLYXW_PLATFORM
#include <stdio.h>
//#include <sleep.h>
#include <unistd.h>
#include "yxw_error.h"
#include <linux/types.h>
#include "../../../../../main.h"
//#include <xspips.h>
//#include <xgpiops.h>
//#include "yxw_error.h"
//#include "spi.h"
//#include "spi_extra.h"
//#include "platform_config.h"
#include "yxw_hal.h"
//#include "DataType.h"

#define LOGLEVEL_CNT  6


//static XGpioPs_Config*  gpio_config;
//static XGpioPs			gpio_instance;

/*delay interface provided by the platform */
void YXWHAL_UDelay(uint32_t usecs)
{
	usleep(usecs);
}

void YXWHAL_MDelay(uint32_t msecs)
{
	usleep(msecs*1000);
}

/*global interface for YXWHAL inited */
// yxwHalErr_t YXWHAL_Init(void)
// {
// 	gpio_config = XGpioPs_LookupConfig(GPIO_DEVICE_ID);
// 	XGpioPs_CfgInitialize(&gpio_instance, gpio_config, gpio_config->BaseAddr);
// 	return YXWHAL_OK;
// }

/*
typedef enum {
	YXWHAL_GPIORXEN0 = 0,
	YXWHAL_GPIORXEN1 = 1,
	YXWHAL_GPIOTXEN0 = 2,
	YXWHAL_GPIOTXEN1 = 3,
	YXWHAL_GPIOINT	= 4,
} yxwHalGpio_t;

#define RF1_RESET_B         (GPIO_OFFSET + 0)
#define TX1_ENABLE   	    (GPIO_OFFSET + 1)
#define TX2_ENABLE   	    (GPIO_OFFSET + 2)
#define RX1_ENABLE   	    (GPIO_OFFSET + 3)
#define RX2_ENABLE   	    (GPIO_OFFSET + 4)
#define TRX_SW       	    (GPIO_OFFSET + 5)
#define FDDTDD_SW           (GPIO_OFFSET + 6)
#define CPRI_ENABLE         (GPIO_OFFSET + 7)
#define RF1_REF_SELECT0     (GPIO_OFFSET + 8)
#define RF1_REF_SELECT1     (GPIO_OFFSET + 9)
#define RF2_RESET_B         (GPIO_OFFSET + 10)
#define RF2_REF_SELECT0     (GPIO_OFFSET + 11)
#define RF2_REF_SELECT1     (GPIO_OFFSET + 12)
#define LMK_SYNC            (GPIO_OFFSET + 13)
#define TEST                (GPIO_OFFSET + 14)
*/

/*Gpio interface provided by the platform */
/*

yxwHalHandle YXWHAL_GpioOpenExt(yxwHalDevId_t devid, int gpioid)
{
    return (yxwHalHandle)gpioid;
}

void YXWHAL_GpioClose(yxwHalHandle handle)
{

}

yxwHalErr_t YXWHAL_GpioSetDirection(yxwHalHandle handle, u8 isOutput)
{
	if(isOutput)
	{
		XGpioPs_SetDirectionPin(&gpio_instance, (uint32_t)handle, 1);
		XGpioPs_SetOutputEnablePin(&gpio_instance, (uint32_t)handle, 1);
	}
	else
	{
		XGpioPs_SetDirectionPin(&gpio_instance, (uint32_t)handle, 0);
		XGpioPs_SetOutputEnablePin(&gpio_instance, (uint32_t)handle, 0);
	}
	return YXWHAL_OK;
}

yxwHalErr_t YXWHAL_GpioGetDirection(yxwHalHandle handle, u8* isOutput)
{
    return YXWHAL_GPIO_FAIL;
}

yxwHalErr_t YXWHAL_GpioSetOutput(yxwHalHandle handle, u8 value)
{
	XGpioPs_SetDirectionPin(&gpio_instance, (uint32_t)handle, 1);
	XGpioPs_SetOutputEnablePin(&gpio_instance, (uint32_t)handle, 1);
	XGpioPs_WritePin(&gpio_instance,(uint32_t)handle,value);
	return YXWHAL_OK;
}

yxwHalErr_t YXWHAL_GpioSet(yxwHalHandle handle, u8 value)
{
	XGpioPs_WritePin(&gpio_instance,(uint32_t)handle,value);
    return YXWHAL_OK;
}

yxwHalErr_t YXWHAL_GpioGet(yxwHalHandle handle, u8* value)
{
    return YXWHAL_GPIO_FAIL;
}



yxwHalErr_t YXWHAL_SpiOpen(yxwHalDevId_t devid,yxwHalHandle* data)
{
	yxwHalErr_t halError = YXWHAL_OK;

	if(devid == YXWHAL_DEVID0)
	{
		*data = YXWHAL_SpiOpenExt(SPI0_DEVICE_ID,SPI_CHIPSEL0,5000000);
		//*data = NULL;
	}
	else if(devid == YXWHAL_DEVID1)
	{
		*data = NULL;
	}

	if (NULL == *data)
		halError = YXWHAL_SPI_FAIL;

	return halError;
}

yxwHalHandle YXWHAL_SpiOpenExt(uint32_t spidevid, uint8_t spichipsel, uint32_t spispeedhz)
{
    struct xil_spi_init_param spi_dev_param;
    struct spi_init_param spi_init_param;
    yxwHalHandle spihandle = NULL;
    int32_t error = 0;


    spi_dev_param.type          = SPI_PS;
    spi_dev_param.flags         = 0;
	spi_dev_param.device_id     = spidevid;

    spi_init_param.max_speed_hz = spispeedhz;
	spi_init_param.mode         = SPI_MODE_0;
	spi_init_param.chip_select  = spichipsel;
	spi_init_param.platform_ops = &xil_platform_ops;
	spi_init_param.extra        = &spi_dev_param;

	error = spi_init((struct spi_desc **)&spihandle, &spi_init_param);

    if (error != 0)
    	return NULL;

    return (spihandle);
}

yxwHalErr_t YXWHAL_SpiClose(yxwHalHandle devHalInfo)
{
	if(devHalInfo != NULL)
	{
		if (spi_remove(devHalInfo) != 0)
			return YXWHAL_SPI_FAIL;
	}

    return YXWHAL_OK;
}
*/
yxwHalErr_t YXWHAL_SpiWriteReg(yxwHalHandle devHalInfo, uint16_t addr, uint32_t data)
{
	yxwHalErr_t halError = YXWHAL_OK;
	uint8_t buf[6];

	// if(devHalInfo == NULL)
	// 	return YXWHAL_SPI_FAIL;

	buf[0] = (uint8_t) ((addr >> 8) & 0x7f);
	buf[1] = (uint8_t) (addr & 0xff);
	buf[2] = (data>>24) & 0xFF;
	buf[3] = (data>>16) & 0xFF;
	buf[4] = (data>>8) & 0xFF;
	buf[5] = (data) & 0xFF;

		//return YXWHAL_SPI_FAIL;
	// if (spi_write_and_read(devHalInfo, buf, 6) != 0)
	// 	return YXWHAL_SPI_FAIL;
     adrv9009_spi_write_reg(buf, 6);
    return halError;
}

yxwHalErr_t YXWHAL_SpiReadReg(yxwHalHandle devHalInfo, uint16_t addr, uint32_t* data)
{
	yxwHalErr_t halError = YXWHAL_OK;
	uint8_t buf[6];

	//if(devHalInfo == NULL)
	//	return YXWHAL_SPI_FAIL;

	buf[0] = (uint8_t) ((addr >> 8) | 0x80);
	buf[1] = (uint8_t) (addr & 0xff);
	buf[2] = (uint8_t) 0x00;
	buf[3] = (uint8_t) 0x00;
	buf[4] = (uint8_t) 0x00;
	buf[5] = (uint8_t) 0x00;

	// if (spi_write_and_read(devHalInfo, buf, 6) != 0)
	// 	return YXWHAL_SPI_FAIL;
  adrv9009_spi_read_reg( buf );
	*data = ((buf[2]<<24)|(buf[3]<<16)|(buf[4]<<8)|buf[5]);

    return halError;
}

yxwHalErr_t YXWHAL_setLogLevel(yxwHalHandle devHalInfo, uint16_t logLevel)
{
    yxwHalErr_t retVal = YXWHAL_OK;

    YxwDevice_t *devHalData = (YxwDevice_t *)devHalInfo;

    if(devHalInfo == NULL)
    {
        //return (YXWHAL_GEN_SW);
    }

    //devHalData->logLevel = logLevel;

    return retVal;
}



yxwHalErr_t YXWHAL_WriteToLog(yxwHalHandle handle,
	uint32_t logLevel, uint32_t errorCode, const char *comment)
{
	return YXWHAL_OK;
}



/*Api to control pin enable, When the Tx/RX channel enable mode is GPIO mode*/
yxwHalErr_t YXWHAL_Tx1EnableCtrl(yxwHalHandle devHalInfo, YxwHalChannelEnableMode_t mode) //pin mode实锟斤拷锟斤拷锟斤拷锟斤拷锟矫伙拷实锟街ｏ拷锟斤拷fpga锟皆硷拷锟斤拷锟斤拷锟斤拷锟斤拷pin锟斤拷锟斤拷锟斤拷tx1锟斤拷tx2
{
    yxwHalErr_t retVal = YXWHAL_OK;
	switch (mode)
	{
		case YXWHAL_CHANNEL_ENABLE:
			/* GPIO control enable Tx1, When the Tx/RX channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x1, 2, 2);
			break;
		case YXWHAL_CHANNEL_DISABLE:
			/* GPIO control disable Tx1, When the Tx/RX channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x0, 2, 2);
			break;
		default:
			break;
	}

    return (retVal);
}

yxwHalErr_t YXWHAL_Tx2EnableCtrl(yxwHalHandle devHalInfo, YxwHalChannelEnableMode_t mode)
{
    yxwHalErr_t retVal = YXWHAL_OK;
	switch (mode)
	{
		case YXWHAL_CHANNEL_ENABLE:
			/* GPIO control enable Tx2, When the Tx/RX channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x1, 3, 3);
			break;
		case YXWHAL_CHANNEL_DISABLE:
			/* GPIO control disable Tx2, When the Tx/RX channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x0, 3, 3);
			break;
		default:
			break;
	}

    return (retVal);
}


yxwHalErr_t YXWHAL_Rx1EnableCtrl(yxwHalHandle devHalInfo, YxwHalChannelEnableMode_t mode)
{
    yxwHalErr_t retVal = YXWHAL_OK;
	switch (mode)
	{
		case YXWHAL_CHANNEL_ENABLE:
			/* GPIO control enable Rx1, When the Tx/RX channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x1, 0, 0);
			//Xil_ModifyReg(AHBADDR(5), 0x3,4,6);
			break;
		case YXWHAL_CHANNEL_DISABLE:
			/* GPIO control disable Rx1, When the Tx/RX channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x0, 0, 0);
			break;
		default:
			break;
	}

    return (retVal);
}


yxwHalErr_t YXWHAL_Rx2EnableCtrl(yxwHalHandle devHalInfo, YxwHalChannelEnableMode_t mode)
{
    yxwHalErr_t retVal = YXWHAL_OK;
	switch (mode)
	{
		case YXWHAL_CHANNEL_ENABLE:
			/* GPIO control enable Rx2, When the Tx/RX channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x1, 1, 1);
			break;
		case YXWHAL_CHANNEL_DISABLE:
			/* GPIO control disable Rx2, When the Tx/RX channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x0, 1, 1);
			break;
		default:
			break;
	}

    return (retVal);
}

yxwHalErr_t YXWHAL_ORx1EnableCtrl(yxwHalHandle devHalInfo, YxwHalChannelEnableMode_t mode)
{
    yxwHalErr_t retVal = YXWHAL_OK;

	switch (mode)
	{
		case YXWHAL_CHANNEL_ENABLE:
			/* GPIO control enable ORx1, When the Tx/RX/ORx channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x0, 0, 0);
			break;
		case YXWHAL_CHANNEL_DISABLE:
			/* GPIO control disable ORx1, When the Tx/RX/ORx channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x1, 0, 0);
			break;
		default:
			break;
	}

    return (retVal);
}


yxwHalErr_t YXWHAL_ORx2EnableCtrl(yxwHalHandle devHalInfo, YxwHalChannelEnableMode_t mode)
{
    yxwHalErr_t retVal = YXWHAL_OK;

	switch (mode)
	{
		case YXWHAL_CHANNEL_ENABLE:
			/* GPIO control enable ORx2, When the Tx/RX/ORx channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x0, 1, 1);
			break;
		case YXWHAL_CHANNEL_DISABLE:
			/* GPIO control disable ORx2, When the Tx/RX/ORx channel enable mode is GPIO mode*/
	//		Xil_ModifyReg(AHBADDR(5), 0x1, 1, 1);
			break;
		default:
			break;
	}

    return (retVal);
}

#endif
