/**
* \file Yxw_jesd204_types.h
* \brief Contains Y9009 API JESD data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef _YXW_JESD204B_TYPE_H_
#define _YXW_JESD204B_TYPE_H_
#include "yxw_data_types.h"

#define WIAT_204B_COUNT       (20)  //wiat 204b count
#define RESET_204B_TIME       (100)  //ms

#define TX_DEFRAMER_NOT_SYNC  (0x1)
#define RX_FRAMER_NOT_SYNC    (0x10)
#define ORX_FRAMER_NOT_SYNC   (0x100)

#define JESD204B_TX_ENABLE_REG    (0x25006050)
#define JESD204B_RX_ENABLE_REG    (0x25007050)
#define JESD204B_ORX_ENABLE_REG   (0x25008050)

#define JESD204B_LAN0_PRBS15_RCVALLCONT   (0x25006200)
#define JESD204B_LAN0_PRBS15_RCVERRCONT   (0x25006140)

#define JESD204B_LAN1_PRBS15_RCVALLCONT   (0x25006204)
#define JESD204B_LAN1_PRBS15_RCVERRCONT   (0x25006144)

#define JESD204B_LAN2_PRBS15_RCVALLCONT   (0x25006208)
#define JESD204B_LAN2_PRBS15_RCVERRCONT   (0x25006148)

#define JESD204B_LAN3_PRBS15_RCVALLCONT   (0x2500620c)
#define JESD204B_LAN3_PRBS15_RCVERRCONT   (0x2500614c)

#define JESD204B_RX_PRBS15_ENABLE         (0x25007134)
#define JESD204B_ORX_PRBS15_ENABLE        (0x25008134)

#define SERDES_CLK_10G   (10)
#define SERDES_CLK_5G    (5)

#define JESD_DEFRAMER_ADDR 	0x25006050
#define JESD_FRAMER_A_ADDR 	0x25007050
#define JESD_FRAMER_B_ADDR 	0x25008050
#define JESD_LANE_SEQ       REG_GC_45

#define ENABLE 			0x1
#define DISABLE			0x0

typedef struct {
	uint32_t lid;
	uint32_t subClass;
	uint32_t cf;
	uint32_t cs;
	uint32_t f;
	uint32_t k;
	uint32_t l;
	uint32_t n;
	uint32_t m;
	uint32_t ntotal;
	uint32_t s;
	uint32_t hd;
	uint32_t rxCoreInterruptMask;
	uint32_t rxIlasAdnMiscDetails12c;
	uint32_t scr;
	uint32_t enableLinkTest;
	uint32_t rxFeatureDisable;
} YxwTxConfig_t;

typedef struct
{
	uint32_t lid;
	uint32_t subClass;
	uint32_t cf;
	uint32_t cs;
	uint32_t f;
	uint32_t k;
	uint32_t l;
	uint32_t n;
	uint32_t m;
	uint32_t ntotal;
	uint32_t s;
	uint32_t hd;
	uint32_t txIlasDelay;
	uint32_t rxIlasAdnMiscDetails12c;
	uint32_t scr;
	uint32_t coreInterruptMask;
	uint32_t tailbits;
	uint32_t enableLinkTest;
	uint32_t txFeatureDisable;
} YxwRxOrXConfig_t;

typedef struct
{
	uint32_t		serdesClk;
	YxwTxConfig_t	txConfig;
	YxwRxOrXConfig_t	rxConfig;
	YxwRxOrXConfig_t	orxConfig;
}YxwJesdSettings_t;

typedef enum
{
	YXW_FRAMER_A = 0,       /*!< Framer A selection */
	YXW_FRAMER_B,           /*!< Framer B selection */
	YXW_FRAMER_A_AND_B      /*!< Used for cases where Orx uses one framer, Rx uses the second framer */
} YxwFramerSel_t;

typedef enum
{
	YXW_DEFRAMER = 0,    /*!< Deframer selection */
} YxwDeframerSel_t;

typedef enum
{
	YXW_JES204B_SYSREF_ON = 0x1,   /*!<Jes204b sysref enable*/
	YXW_JES204B_SYSREF_OFF = 0x2	  /*!<Jes204b sysref disable*/
} YxwJes204bSysrefControlMode_t;

typedef enum
{
	YXW_LANE_SEQ_0=0x0, /*!<Jesd204b laneid 0-1-2-3>*/
	YXW_LANE_SEQ_1=0x1, /*!<Jesd204b laneid 0-1-3-2>*/
	YXW_LANE_SEQ_2=0x2, /*!<Jesd204b laneid 0-2-1-3>*/
	YXW_LANE_SEQ_3=0x3, /*!<Jesd204b laneid 0-2-3-1>*/
	YXW_LANE_SEQ_4=0x4, /*!<Jesd204b laneid 0-3-1-2>*/
	YXW_LANE_SEQ_5=0x5, /*!<Jesd204b laneid 0-3-2-1>*/

	YXW_LANE_SEQ_6=0x6, /*!<Jesd204b laneid 1-0-2-3>*/
	YXW_LANE_SEQ_7=0x7, /*!<Jesd204b laneid 1-0-3-2>*/
	YXW_LANE_SEQ_8=0x8, /*!<Jesd204b laneid 1-2-0-3>*/
	YXW_LANE_SEQ_9=0x9, /*!<Jesd204b laneid 1-2-3-0>*/
	YXW_LANE_SEQ_10=0xa, /*!<Jesd204b laneid 1-3-0-2>*/
	YXW_LANE_SEQ_11=0xb, /*!<Jesd204b laneid 1-3-2-0>*/

	YXW_LANE_SEQ_12=0xc, /*!<Jesd204b laneid 2-1-0-3>*/
	YXW_LANE_SEQ_13=0xd, /*!<Jesd204b laneid 2-1-3-0>*/
	YXW_LANE_SEQ_14=0xe, /*!<Jesd204b laneid 2-0-1-3>*/
	YXW_LANE_SEQ_15=0xf, /*!<Jesd204b laneid 2-0-3-1>*/
	YXW_LANE_SEQ_16=0x10, /*!<Jesd204b laneid 2-3-1-0>*/
	YXW_LANE_SEQ_17=0x11, /*!<Jesd204b laneid 2-3-0-1>*/

	YXW_LANE_SEQ_18=0x12, /*!<Jesd204b laneid 3-1-2-0>*/
	YXW_LANE_SEQ_19=0x13, /*!<Jesd204b laneid 3-1-0-2>*/
	YXW_LANE_SEQ_20=0x14, /*!<Jesd204b laneid 3-2-1-0>*/
	YXW_LANE_SEQ_21=0x15, /*!<Jesd204b laneid 3-2-0-1>*/
	YXW_LANE_SEQ_22=0x16, /*!<Jesd204b laneid 3-0-1-2>*/
	YXW_LANE_SEQ_23=0x17, /*!<Jesd204b laneid 3-0-2-1>*/
}YxwJesdLaneSeq_t;

typedef enum
{
	YXW_BYTE_SEQ_0  = 0x00, /*!<byte seq: 0x01234567 0x89ABCDEF -> 0x01234567 0x89ABCDEF>*/
	YXW_BYTE_SEQ_1  = 0x01, /*!<byte seq: 0x01234567 0x89ABCDEF -> 0x23016745 0xAB89EFCD>*/
	YXW_BYTE_SEQ_2  = 0x02, /*!<byte seq: 0x01234567 0x89ABCDEF -> 0x45670123 0xCDEF89AB>>*/
	YXW_BYTE_SEQ_3  = YXW_BYTE_SEQ_1 | YXW_BYTE_SEQ_2,
	YXW_BYTE_SEQ_4  = 0x04, /*!<byte seq: 0x01234567 0x89ABCDEF -> 0xCDEF4567 0x89AB0123>>*/
	YXW_BYTE_SEQ_5  = YXW_BYTE_SEQ_1 | YXW_BYTE_SEQ_4,
	YXW_BYTE_SEQ_6  = YXW_BYTE_SEQ_2 | YXW_BYTE_SEQ_4,
	YXW_BYTE_SEQ_7  = YXW_BYTE_SEQ_1 | YXW_BYTE_SEQ_2 | YXW_BYTE_SEQ_4,
	YXW_BYTE_SEQ_8  = 0x08, /*!<byte seq: 0x01234567 0x89ABCDEF -> 0x012389AB 0x45678CDEF>>*/
	YXW_BYTE_SEQ_9  = YXW_BYTE_SEQ_1 | YXW_BYTE_SEQ_8,
	YXW_BYTE_SEQ_A  = YXW_BYTE_SEQ_2 | YXW_BYTE_SEQ_8,
	YXW_BYTE_SEQ_B  = YXW_BYTE_SEQ_1 | YXW_BYTE_SEQ_2 | YXW_BYTE_SEQ_8,
	YXW_BYTE_SEQ_C  = YXW_BYTE_SEQ_4 | YXW_BYTE_SEQ_8,
	YXW_BYTE_SEQ_D  = YXW_BYTE_SEQ_1 | YXW_BYTE_SEQ_4 | YXW_BYTE_SEQ_8,
	YXW_BYTE_SEQ_E  = YXW_BYTE_SEQ_2 | YXW_BYTE_SEQ_4 | YXW_BYTE_SEQ_8,
	YXW_BYTE_SEQ_F  = YXW_BYTE_SEQ_1 | YXW_BYTE_SEQ_2 | YXW_BYTE_SEQ_4 | YXW_BYTE_SEQ_8,
}YxwJesdByteSeq_t;

#endif //__YXW_JESD204B_TYPE_H__

