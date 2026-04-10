/**
* \file yxw_hopping_types.h
* \brief Contains Y9009 hopping frequency data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#ifndef _YXW_HOPPING_TYPES_H_
#define _YXW_HOPPING_TYPES_H_
#include "yxw_data_types.h"

/**
 * \brief Data structure to hold hopping frequency settings
 */
#define HOPPING_LIST_MAX		32
typedef struct
{
	uint8_t triggerPin;
	uint8_t reserved0;
	uint8_t reserved1;
	uint8_t ListNum;
	uint32_t List[HOPPING_LIST_MAX];
} YxwHoppingInfo_t;

typedef struct
{
	unsigned int rfpll[2];
	unsigned int auxpll[2];
	unsigned int data[195];
} HOPPING_FREQ;

#endif
