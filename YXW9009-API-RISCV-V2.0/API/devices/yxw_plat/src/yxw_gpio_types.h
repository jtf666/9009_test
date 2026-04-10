/**
* \file yxw_gpio_types.h
* \brief Contains functions to allow control of the General Purpose IO functions on the Y9009 device
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/
#ifndef YXW_GPIO_TYPES_H_
#define YXW_GPIO_TYPES_H_
#include "yxw_data_types.h"

/**
 *  \brief Enum to select desired low voltage GPIO pin used by the API
 */
typedef enum
{
	YXW_GPIO_00 = 0x0,  			/*!<low voltage gpio 0*/
	YXW_GPIO_01 = 0x1,		 	/*!<low voltage gpio 1*/
	YXW_GPIO_02 = 0x2,		 	/*!<low voltage gpio 2*/
	YXW_GPIO_03 = 0x3,		 	/*!<low voltage gpio 3*/
	YXW_GPIO_04 = 0x4,		 	/*!<low voltage gpio 4*/
	YXW_GPIO_05 = 0x5,		 	/*!<low voltage gpio 5*/
	YXW_GPIO_06 = 0x6,			/*!<low voltage gpio 6*/
	YXW_GPIO_07 = 0x7,			/*!<low voltage gpio 7*/
	YXW_GPIO_08 = 0x8,		 	/*!<low voltage gpio 8*/
	YXW_GPIO_09 = 0x9,		 	/*!<low voltage gpio 9*/
	YXW_GPIO_10 = 0xa,		 	/*!<low voltage gpio 10*/
	YXW_GPIO_11 = 0xb,		 	/*!<low voltage gpio 11*/
	YXW_GPIO_12 = 0xc,		 	/*!<low voltage gpio 12*/
	YXW_GPIO_13 = 0xd,		 	/*!<low voltage gpio 13*/
	YXW_GPIO_14 = 0xe,		 	/*!<low voltage gpio 14*/
	YXW_GPIO_15 = 0xf,		 	/*!<low voltage gpio 15*/
	YXW_GPIO_16 = 0x10,		 	/*!<low voltage gpio 16*/
	YXW_GPIO_17 = 0x11,		 	/*!<low voltage gpio 17-->boot0,user can not be used*/
	YXW_GPIO_18 = 0x12,		 	/*!<low voltage gpio 18-->boot1，user can not be used*/
	YXW_GPIO_INVALID = 0xff  	/*!<valid low voltage gpio */
} YxwGpioPinSel_t;

typedef enum
{
	YXW_SEL_0	= 0x0,			/*!<gpio function select 0,More info pls see gpio doc*/
	YXW_SEL_1	= 0x1,			/*!<gpio function select 1*/
	YXW_SEL_2	= 0x2,			/*!<gpio function select 2*/
	YXW_SEL_3	= 0x3,			/*!<gpio function select 3*/
	YXW_SEL_4	= 0x4,			/*!<gpio function select 4*/
	YXW_SEL_5	= 0x5,			/*!<gpio function select 5*/
	YXW_SEL_6	= 0x6,			/*!<gpio function select 6*/
	YXW_SEL_7	= 0x7,			/*!<gpio function select 7*/
} YxwGpioPinFunc_t;

typedef enum
{
	YXW_RISCV_CTRL = 0x0,			/*!<gpio is controled by riscv*/
	YXW_SPI_CTRL = 0x1		/*!<gpio is controled by spi*/

}YxwGpioCtrlMode_t;

typedef enum
{
	YXW_GPIO_DIR_IN = 0x0,			/*!<gpio dir is IN*/
	YXW_GPIO_DIR_OUT = 0x1			/*!<gpio dir is OUT*/

}YxwGpioDir_t;

/**
 * \brief Enumeration for 10bit AuxDAC voltage for center DAC code (code 512).
 */
typedef enum
{
	YXW_AUXDACVREF_1V 	= 0x0, 	/*!<AuxDac reference at 1V*/
	YXW_AUXDACVREF_1P5V  = 0x1,	/*!<AuxDac reference at 1.5V*/
	YXW_AUXDACVREF_2V 	= 0x2, 	/*!<AuxDac reference at 2V*/
	YXW_AUXDACVREF_2P5V 	= 0x3 	/*!<AuxDac reference at 2.5V*/
} YxwAuxDacVref_t;

/**
 * \brief Enumeration for AuxDAC resolution modes.
 */
typedef enum
{
	YXW_AUXDACRES_12BIT = 0x0, 	/*!<12it DAC resolution for a subset of the output voltage range centered aroung VREF*/
	YXW_AUXDACRES_11BIT = 0x1, 	/*!<11it DAC resolution for a subset of the output voltage range centered aroung VREF*/
	YXW_AUXDACRES_10BIT = 0x2, 	/*!<10it DAC resolution for 100mv to 3.3v VREF*/
} YxwAuxDacResolution_t;

/**
 * \brief Data structure to hold the auxiliary DAC settings
 */
typedef struct
{
	uint16_t             auxDacEnables;         /*!< Aux DAC enable bit for each DAC, where the first ten bits correspond to the 10-bit DACs, and the next consecutive two bits enable the 12-bit DACs */
	YxwAuxDacVref_t       auxDacVref[10];        /*!< Aux DAC voltage reference value for each of the 10-bit DACs */
	YxwAuxDacResolution_t auxDacResolution[10];  /*!< Aux DAC slope (resolution of voltage change per AuxDAC code) - only applies to 10bit DACs (0-9) */
	uint16_t             auxDacValues[12];      /*!< Aux DAC values for each 10-bit DAC correspond to the first 10 array elements, the next consecutive array elements correspond to the two 12-bit DAC values */
} YxwAuxDac_t;

/**
 * \brief General Purpose interrupt mask types
 */
typedef enum
{
	YXW_GP_MASK_WATCHDOG_TIMEOUT        = 0x80000000,   /*!< Watchdog GP Interrupt mask bit */
	YXW_GP_MASK_PA_PROTECTION_TX1_ERROR = 0x40000000,   /*!< Tx2 PA protection error GP Interrupt mask bit */
	YXW_GP_MASK_PA_PROTECTION_TX2_ERROR = 0x20000000,   /*!< Tx1 PA protection error GP Interrupt mask bit */
	YXW_GP_MASK_CALIBRATION_ERROR   	   = 0x10000000,   /*!<calibration error GP Interrupt mask bit */
	YXW_GP_MASK_FORCE_INTERRUPT     	   = 0x08000000,   /*!<force GP Interrupt mask bit */
	YXW_GP_MASK_JESD_DEFRMER_IRQ        = 0x04000000,   /*!< JESD204B Deframer IRQ error GP Interrupt mask bit */
	YXW_GP_MASK_BB_SYNTH_UNLOCK         = 0x02000000,   /*!< BB clock PLL non-lock error GP Interrupt mask bit */
	YXW_GP_MASK_AUX_SYNTH_UNLOCK        = 0x01000000,   /*!< Auxiliary PLL non-lock error GP Interrupt mask bit */
	YXW_GP_MASK_RF_SYNTH_UNLOCK         = 0x00800000    /*!< RF PLL non-lock error GP Interrupt mask bit */
}YxwGpIntMask_t;

/**
 * \brief Talise Aux ADC Channel types
 */
typedef enum
{
	YXW_AUXADC_CH0 = 0x0,  /*!< Select Aux ADC Channel 0 for sampling and conversion*/
	YXW_AUXADC_CH1 = 0x1,  /*!< Select Aux ADC Channel 1 for sampling and conversion*/
	YXW_AUXADC_CH2 = 0x2,  /*!< Select Aux ADC Channel 2 for sampling and conversion*/
	YXW_AUXADC_CH3 = 0x3   /*!< Select Aux ADC Channel 3 for sampling and conversion*/
} YxwAuxAdcChannels_t;

/**
 * \brief Aux ADC modes for sampling
 */
typedef enum
{
	YXW_AUXADC_NONPIN_MODE = 0x0,  /*!< Select Aux ADC sampling and conversion in Non-Pin mode*/
	YXW_AUXADC_PIN_MODE    = 0x1   /*!< Select Aux ADC sampling and conversion in Pin mode*/
} YxwAuxAdcModes_t;

/**
 * \brief Aux ADC configuration structure
 */
typedef struct
{
	YxwAuxAdcChannels_t auxAdcChannelSel;  	/*!< Selects the channel which is supposed to sample AuxADC input for A/D conversion */
	YxwAuxAdcModes_t    auxAdcMode;        	/*!< Selects mode to latch and store conversion results */
	uint32_t           numSamples;       	/*!< No. of A/D conversions to be performed in range 1 - 1000 */
	uint32_t           samplingPeriod_us; 	/*!< Sampling interval time in microseconds (Mininum 1us,Maxnum 8us) NOTE: Valid only for non pin mode. Ignored for pin mode. */
	uint32_t           auxadcGpioSel; 		/*!< auxadc gpiosel can use gpio2-9 & gpio10-13*/
} YxwAuxAdcConfig_t;

/**
 * \brief Aux ADC conversion result structure
 */
typedef struct
{
	uint32_t auxAdcCodeAvg;     /*!< 12-bit Average of AuxADC A/D conversion samples */
	uint32_t  completeIndicator; /*!< Flag to indicate if a scheduled AuxADC conversion completed. 1 - AuxADC Conversion Complete, 0 - AuxADC Conversion Incomplete */
} YxwAuxAdcResult_t;

/**
 * \brief Talise gpio monitor pos sel types(Used to select the position of 8-bit monitor signal on GPIO)
 */
typedef enum
{
	YXW_GPIO_MONITOR_POS_SEL_D0 = 0x0,  /*!< GPIO Select :{8d0, monitor_valid_data[7:0]}*///(8d0 and 4d0 means invalid bit data)
	YXW_GPIO_MONITOR_POS_SEL_D1 = 0x1,  /*!< GPIO Select :{monitor_valid_data[7:4], 4d0, 4d0, monitor_valid_data[3:0]}*/
	YXW_GPIO_MONITOR_POS_SEL_D2 = 0x2,  /*!< GPIO Select :{4d0, monitor_valid_data[3:0], monitor_valid_data[7:4], 4d0}*/
	YXW_GPIO_MONITOR_POS_SEL_D3 = 0x3   /*!< GPIO Select :{monitor_valid_data[7:0], 8d0}*/
} YxwGpioMonitorPosSel_t;

#endif
