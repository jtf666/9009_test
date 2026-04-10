/**
* \file yxw_riscv_types.h
* \brief Contains Yxw API Riscv data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#ifndef __YXW_RISCV_TYPES_H__
#define __YXW_RISCV_TYPES_H__

#include "yxw_data_types.h"

#define TX_PROFILE_VALID    			0x01
#define RX_PROFILE_VALID    			0x02
#define ORX_PROFILE_VALID   			0x04
/*Drvier commandType*/
#define YXW_ABORT_OPCODE				0x0
#define YXW_RUNN_INIT_OPCODE			0x1
#define YXW_RADIO_ON_OPCODE				0x2
#define YXW_WRITE_MEM_OPCODE			0x3
#define YXW_READ_MEM_OPCODE				0x4
#define	YXW_SET_OPCODE					0x5
#define YXW_GET_OPCODE					0x6
#define YXW_GET_MEMORY_DATA 		    0x7



#define PHY_INIT_CALI_INIT              0x11
#define RFPLL_FREQ_CALI_INIT	        0x22
#define ENABLE_SYSREF_INIT  	        0x33
#define ENABLE_DBG_PRINT2SPI  	        0x34
#define HI_MSG_TO_JESD204B_MSGID  		0x36
#define RESET_DEVICE_MSGID  		   	0x37
#define HI_MSG_ENABLE_LOG_MSGID  		0x38
#define HI_SET_GPIO_VALUE  		        0x3A
#define HI_GET_GPIO_VALUE  		        0x3B
#define HI_ENABLE_SLICER  		        0x3C
#define HI_ENABLE_PLLTC_TRACK			0x3D
#define HI_ENABLE_FRAMER_A  		    0x3E
#define HI_ENABLE_FRAMER_B  		    0x3F
#define HI_ENABLE_DFRAMER  		        0x40
#define HI_GET_DACPOWER 		        0x41
#define HI_GET_ORXDACPOWER 		        0x42
#define HI_GET_PRBS15_SYSTEM_TX 		0x43
#define HI_SET_PRBS15_SYSTEM_RX_ORX     0x44
#define HI_SET_AGC_MAX_VALUE     		0x46
#define HI_SET_RF_PLL_LOOP_FILTER     	0x47
#define HI_GET_TXDACPOWER				0x48
#define HI_SET_HOPPINGCONFIG            0x49
#define HI_SET_HOPPINGINDEX             0x4A
#define HI_ENABLE_ORX_SLICER  		    0x4B
#define HI_SET_HOPPINGLIST              0x4C
#define HI_SET_GPIO_INTR                0x4D
#define HI_SET_LANE_SEQ                 0x4E
#define HI_GET_LANE_SEQ                 0x4F
#define HI_SET_SPECIAL_MODE             0x50

#define RECV_CMD_CONFIRM  				  REG_RSV
#define SEND_CMD_TYPE  					  REG_RSV_1
#define SEND_CMD_DATA					  REG_RSV_2
#define DOCMD_STATUS					  REG_RSV_3

#define DBG_PRINT_BUF_SIZE_MAX 			 256

#define DOWNLOAD_ITCM_ADDR				 (0x1000)
#define DOWNLOAD_ITCM_SIZE				 (64*1024)
#define DOWNLOAD_DTCM_ADDR				 (0x1001)
#define DOWNLOAD_DTCM_SIZE				 (64*1024)
#define DOWNLOAD_MEM_ADDR				 (0x1003)
#define DOWNLOAD_MEM_SIZE				 (64*1024)

#define BOOT_RDY						0x42
#define HI_STATUS_SUCCESS	   			0x88

#define HI_STATUS_SUCCESS_BIT	   		0
#define HI_STATUS_INVALID_DATA_BIT      1
#define HI_STATUS_FIRMWARE_READY_BIT	2
#define RFPLL_CALI_DONE_BIT				3
#define PHY_INIT_CALI_DONE_BIT			4
#define PHY_INIT_PROFILE_DONE_BIT		5
#define DBG_2SPI_DONE_BIT				6
#define JESD204B_DONE_BIT				7
#define GPIO_GET_VALUE_DONE_BIT         8

#define WDT_ERROR_BIT					31
#define PAPROTECT_ERROR_BIT				30
#define PAPROTECT_ERROR_CH_BIT			29

#define MAGIC							0xa5

#define RESET_BY_ISR  	   				0x0
#define RESET_BY_RESET     				0x1

#define IOCTL_WRITE_CMD 				(0x1)
#define IOCTL_READ_CMD 					(0x2)
#define YXW_ADDR_DATA_REGION                      0x10030000
#define YXW_ADDR_FW_VERSION                       0x10032000

#define YXW_ADDR_CALISTA_ADDR                     0x1003f010

#define HI_MSG_TO_PROFILE_MSGID  		 		0x1
#define HI_MSG_TO_PROFILE_MSGLEN		 		68
#define PROFILE_MSG_ADDR				 		YXW_ADDR_DATA_REGION

#define HI_MSG_TO_RFFRQ_MSGID  			 		0x2
#define HI_MSG_TO_RFFRQ_MSGLEN		 	 		0x8
#define RFFRQ_MSG_ADDR				 			(PROFILE_MSG_ADDR+HI_MSG_TO_PROFILE_MSGLEN) //last addr offset

#define HI_MSG_TO_AUXFRQ_MSGID  				0x6
#define HI_MSG_AUXFRQ_MSGLEN         	 		0x8
#define AUXFRQ_MSG_ADDR				 			(RFFRQ_MSG_ADDR+HI_MSG_TO_RFFRQ_MSGLEN) //last addr offset

#define HI_MSG_TO_READFIRDY_MSGID  		 		0x3
#define HI_MSG_TO_READFIRDY_MSGLEN		 		20

#define HI_MSG_TO_FILTER_MSGID  		        0x4
#define HI_MSG_TO_FILTER_GAINDB_NUMFIR_LEN		8
#define HI_MSG_TO_TXFILTER_COEFS_LEN	      	80
#define HI_MSG_TO_RX_ORX_FILTER_COEFS_LEN	  	72
#define HI_MSG_TO_TXFILTER_MSGLEN		        (HI_MSG_TO_FILTER_GAINDB_NUMFIR_LEN+HI_MSG_TO_TXFILTER_COEFS_LEN*2)
#define HI_MSG_TO_RX_ORXFILTER_MSGLEN	     	(HI_MSG_TO_FILTER_GAINDB_NUMFIR_LEN+HI_MSG_TO_RX_ORX_FILTER_COEFS_LEN*2)
#define FILTER_MSG_ADDR				 			(AUXFRQ_MSG_ADDR+HI_MSG_AUXFRQ_MSGLEN)//last addr offset
#define HI_MSG_TO_TXFILTER_TOATL_MSGLEN	        (HI_MSG_TO_TXFILTER_MSGLEN+(HI_MSG_TO_RX_ORXFILTER_MSGLEN*2))


#define HI_MSG_TO_CALSTATUS_MSGID  		 		0x5
#define HI_MSG_TO_CALSTATUS_MSGLEN       		72

#define HI_MSG_TO_J204B_MSGID        0x7
#define HI_MSG_TO_J204B_MSGLEN       256
#define J204B_MSG_ADDR				 (FILTER_MSG_ADDR+HI_MSG_TO_TXFILTER_TOATL_MSGLEN) //last addr offset

#define HI_MSG_TO_HOPFREQ_MSGID                 0x8
#define HI_MSG_TO_HOPFREQ_MSGLEN                16
#define HOPFREQ_MSG_ADDR                        0x1003B798

#define CALIBRITION_SRAM_ADDR  (0x10039788+8)  //ECO version add chip_id function
#define CALIBRITION_SRAM_LEN   (2050*4)
#define CALI_EVENTID_CNT	   (14)       //14 calibration events

#define WIAT_TIMEOUT                (120)  //wiat_timeout=120s
#define SPI_RESPONSE_WIAT_TIMEOUT   (1200)  //spi_Response_wiat_timeout=120s

#define SAMPLE_MAX_LENGTH           (16*1024)


#define ACTION_SET_TIMEOUT_BIT   (1)
#define ACTION_CLEAR_TIMEOUT_BIT (0)

#define WAIT_FIRMWARE_READY_TIMEOUT      BIT(0)
#define WAIT_PROFILE_DONE_TIMEOUT        BIT(1)
#define WAIT_PLLS_LOCK_DONE_TIMEOUT      BIT(2)
#define WAIT_JES204B_CONFIG_DONE_TIMEOUT BIT(3)
#define WAIT_CALS_DONE_TIMEOUT           BIT(4)
#define WAIT_CALS_OUT_RING_DONE_TIMEOUT  BIT(5)
#define WAIT_SPI_CMD_DONE_TIMEOUT  	     BIT(6)
#define WAIT_JES204B_SYNC_TIMEOUT   	 BIT(7)

#define POWER_ACCURACY					(1000.0F)

typedef struct __TxData
{
	uint32_t value;
	uint32_t addr;
	uint32_t lo;
}YxwTxData;

typedef struct CmdTxType
{
	uint32_t Cmd;
	YxwTxData TxData;
}YxwCmdTx;

typedef struct __RxData
{
	uint32_t value;
	uint32_t addr;
	uint32_t lo;
}YxwRxData;

typedef struct CmdRxType
{
	uint32_t Cmd;
	YxwRxData RxData;
}YxwCmdRx;

typedef struct _YxwMsg
{
	uint32_t addr;
	uint16_t Lenth;
	uint16_t Id;
	uint32_t offset;
	uint8_t data[CALIBRITION_SRAM_LEN];
}YxwMsg;

typedef struct YxwMemMsg
{
	uint32_t Cmd;
	YxwMsg Msg;
}YxwMem;

typedef struct
{
  uint32_t StartBit;
  uint32_t EndBit;
  uint16_t RegAddr;
  uint32_t RegValue;
  long long RegDelayTime;
} YxwCmdReg;

typedef struct DataType
{
	uint32_t CmdType;
	union
	{
   		YxwCmdReg Reg;
		YxwCmdTx Tx;
		YxwCmdRx Rx;
		YxwMem Mmsg;
	}u;
}YxwData;

struct firmware_headr
{
	uint32_t itcmAddrOffset;
	uint32_t itcmAddr;
	uint32_t itcmLen;
	uint32_t itcmBlockSize;
	uint32_t dtcmAddrOffset;
	uint32_t dtcmAddr;
	uint32_t dtcmLen;
	uint32_t dtcmBlockSize;
};

struct firmware_Y9009
{
	struct firmware_headr hdr;
	uint8_t *fw_dccm;
	uint8_t *fw_iccm;
};

#define ELOG_LEN  1024

struct elog_id
{
	unsigned short 	id;
	unsigned short	valid;
	unsigned int  data;
};

struct elog_s
{
	unsigned short start;
	unsigned short index;
	unsigned int len;
	struct elog_id e_id[ELOG_LEN];
};

struct build_info
{
	char git_info[64];
	char submiter[64];
	char summit_date[64];
	char compiler[64];
	char compile_time[64];
};
#endif
