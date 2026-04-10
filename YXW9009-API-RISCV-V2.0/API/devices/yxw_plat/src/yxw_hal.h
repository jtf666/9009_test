#ifndef __YXW_HAL_H__
#define __YXW_HAL_H__

#include <stdint.h>

typedef enum
{
	YXWHAL_OK 		= 0x0,
	YXWHAL_SPI_FAIL	= 0x1,
	YXWHAL_GPIO_FAIL	= 0x2,
	YXWHAL_INV_AGC_CLK_PARM_FAIL = 0x3,
	YXWHAL_INV_AGC_RX_GAIN_UPDATE_TIME_PARM_FAIL=0x4,
	YXWHAL_PARA_ERR =0x5,
	YXWHAL_FILE_ERR =0x6,
	YXWHAL_TIME_OUT
} yxwHalErr_t;

typedef enum
{
	YXWHAL_LOG_NONE = 0x0,
	YXWHAL_LOG_MSG  = 0x1,
	YXWHAL_LOG_WARN = 0x2,
	YXWHAL_LOG_ERR  = 0x4,
	YXWHAL_LOG_SPI  = 0x8,
	YXWHAL_LOG_ALL  = 0xF
} yxwLogLevel_t;

typedef enum {
	YXWHAL_DEVID0	= 0x0,
	YXWHAL_DEVID1	= 0x1,
	YXWHAL_DEVID2	= 0x2,
	YXWHAL_DEVID3	= 0x3,
	YXWHAL_DEVID4	= 0x4,
	YXWHAL_DEVID5	= 0x5,
	YXWHAL_DEVCNTMAX
}yxwHalDevId_t;

typedef enum
{
	YXWHAL_CHANNEL_DISABLE = 0x0,  /*!<Tx/RX channel disable*/
	YXWHAL_CHANNEL_ENABLE  = 0x01  /*!<Tx/RX channel enable*/
} YxwHalChannelEnableMode_t;

typedef struct yxwHalHandle_t * yxwHalHandle;

/*Spi operation,necessarily*/
// yxwHalErr_t YXWHAL_SpiOpen(yxwHalDevId_t devid, yxwHalHandle *handle);
// yxwHalHandle YXWHAL_SpiOpenExt(uint32_t spidevid, uint8_t spichipsel, uint32_t spispeedhz);
// yxwHalErr_t YXWHAL_SpiClose(yxwHalHandle handle);
yxwHalErr_t YXWHAL_SpiWriteReg(yxwHalHandle handle, uint16_t addr, uint32_t data);
yxwHalErr_t YXWHAL_SpiReadReg(yxwHalHandle handle, uint16_t addr, uint32_t* data);

/*External pin reset,Not necessarily*/
// yxwHalErr_t YXWHAL_ResetHw(yxwHalDevId_t devid);

// uint32_t   YXWHAL_DeviceCount(void);

/*Write log to file,Not necessarily*/
yxwHalErr_t YXWHAL_SetLogLevel(yxwHalHandle devHalInfo, uint16_t halLogLevel);
yxwHalErr_t YXWHAL_WriteToLog(yxwHalHandle handle,
	uint32_t logLevel, uint32_t errorCode, const char *comment);

/*Delay Api,necessarily*/
void YXWHAL_MDelay(uint32_t msecs);
void YXWHAL_UDelay(uint32_t usecs);

/*Api to control pin enable, When the Tx/RX channel enable mode is GPIO mode,Not necessarily*/
yxwHalErr_t YXWHAL_Tx1EnableCtrl(yxwHalHandle handle, YxwHalChannelEnableMode_t mode);
yxwHalErr_t YXWHAL_Tx2EnableCtrl(yxwHalHandle handle, YxwHalChannelEnableMode_t mode);
yxwHalErr_t YXWHAL_Rx1EnableCtrl(yxwHalHandle handle, YxwHalChannelEnableMode_t mode);
yxwHalErr_t YXWHAL_Rx2EnableCtrl(yxwHalHandle handle, YxwHalChannelEnableMode_t mode);
yxwHalErr_t YXWHAL_ORx1EnableCtrl(yxwHalHandle handle, YxwHalChannelEnableMode_t mode);
yxwHalErr_t YXWHAL_ORx1TxSel(yxwHalHandle handle, uint32_t mask);
yxwHalErr_t YXWHAL_ORx2EnableCtrl(yxwHalHandle handle, YxwHalChannelEnableMode_t mode);

/*Api to control MGC mode by gpio,Not necessarily*/
yxwHalErr_t YXWHAL_Rx1GainCtrlInc(yxwHalHandle handle);
yxwHalErr_t YXWHAL_Rx1GainCtrlDec(yxwHalHandle handle);
yxwHalErr_t YXWHAL_Rx2GainCtrlInc(yxwHalHandle handle);
yxwHalErr_t YXWHAL_Rx2GainCtrlDec(yxwHalHandle handle);

#endif
