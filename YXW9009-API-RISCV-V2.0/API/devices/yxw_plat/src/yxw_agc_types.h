/**
* \file Yxw_agc_types.h
* \brief Contains Y9009 API AGC data types
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef _YXW_AGC_TYPES_H_
#define _YXW_AGC_TYPES_H_
#include "yxw_data_types.h"

/**
 *  \brief Data structure to hold AGC peak settings
 *  The evaluation software GUI for the product can be used to generate a structure with suggested settings.
 */
typedef struct
{
	uint8_t     apdEnableMeasurement;               /*!< Enable the Rx apd measurement block. (0/1) */
	uint8_t     apdAttackMode;                		/*!< for agc mode, 0: periodically attack, 1: immediately attack */
	uint8_t     apdMeasurementDuration;             /*!< APD update time(the time = 2^8*(apdMeasurementDuration+1) clocks) apdMeasurementDuration valid range 0-7*/
	uint16_t    apdImmdOverNum;                     /*!< mmediately attack mode, overrange */
	uint16_t    apdPeriOverNum;                     /*!< periodically attack mode, overrange*/
	uint16_t    apdUnderNum;                        /*!< underrange valid range is 0 to 1023 */
	uint16_t    apdLowTresh;
	uint16_t    apdHighTresh;
	uint8_t     apdGainStepAttack;                  /*!< AGC APD peak detect attack gain step. Valid range is 2.4.5.6.8.9.10.11.12.13.14 */
	uint8_t     apdGainStepRecovery;                /*!< AGC APD gain index step size for recovery. Valid range is 2.4.5.6.8.9.10.11.12.13.14*/

	uint8_t     hb2EnableMeasurement;               /*!< Enable or disables the HB2 measurement block. */
	uint8_t     hb2MeasurementDuration;             /*!< AGC HB2 update time(the time = 2^8*(hb2MeasurementDuration+1) clocks) valid range 0-15*/
	uint8_t     hb2RecoveryMode;                    /*!< for agc mode, 0: periodically recovery, 1: immediately recovery */
	uint8_t     hb2AttackMode;                      /*!< for agc mode, 0: periodically attack, 1: immediately attack */
	uint16_t    hb2OverThresh;                      /*!< AGC HB2 output high threshold. Valid range from  0 to 16383 */
	uint16_t    hb2UnderRangeLowThresh;             /*!< AGC HB2 output low threshold. Valid range from  0 to 16383 */
	uint16_t    hb2UnderRangeMidThresh;             /*!< AGC HB2 output low threshold for 2nd interval for multiple time constant AGC mode. Valid range from  0 to 16383 */
	uint16_t    hb2UnderRangeHighThresh;            /*!< AGC HB2 output low threshold for 3rd interval for multiple time constant AGC mode. Valid range from  0 to 16383 */
	uint16_t    hb2OverloadThreshCnt;               /*!< Sets the number of actual overloads required to trigger the overload signal in agcGainUpdateCounter time. Valid range from 1 to 1023 */
	uint16_t    hb2UnderRangeHighThreshCnt;         /*!< Sets the number of actual Under Range High threshold count to trigger the overload signal in hpd cnt time. Valid range from 1 to 1023 */
	uint16_t    hb2UnderRangeMidThreshCnt;          /*!< Sets the number of actual Under Range Mid threshold  count to trigger the overload signal in hpd cnt time. Valid range from 1 to 1023 */
	uint16_t    hb2UnderRangeLowThreshCnt;          /*!< Sets the number of actual Under Range Low threshold  count to trigger the overload signal in hpd cnt time. Valid range from 1 to 1023 */
	uint16_t	hb2PeriExceededCnt;					/*!< Sets the number of actual periodically over Range overange num Valid range from 1 to 1023 */
	uint16_t	hb2PeriUnderExceededCnt;			/*!< Sets the number of actual periodically Under Range overange num Valid range from 1 to 1023 */
	uint16_t	hb2ImmdExceededCnt;					/*!< Sets the number of actual immediately over Range overange num Valid range from 1 to 1023 */
	uint16_t	hb2ImmdUnderHigCnt;					/*!< Sets the number of actual immediately underHigh Range overange num Valid range from 1 to 1023 */
	uint16_t	hb2ImmdUnderLowCnt;					/*!< Sets the number of actual immediately underlow Range overange num Valid range from 1 to 1023 */
	uint16_t	hb2ImmdUnderMidCnt;					/*!< Sets the number of actual immediately underMid Range overange num Valid range from 1 to 1023 */
	uint8_t     hb2GainStepHighRecovery;            /*!< AGC HB2 gain index step size. Valid range from  0 to 15 */
	uint8_t     hb2GainStepLowRecovery;             /*!< AGC HB2 gain index step size, when the HB2 Low Overrange interval 2 triggers. Valid range from 2.4.5.6.8.9.10.11.12.13.14 */
	uint8_t     hb2GainStepMidRecovery;             /*!< AGC HB2 gain index step size, when the HB2 Low Overrange interval 3 triggers. Valid range from 2.4.5.6.8.9.10.11.12.13.14 */
	uint8_t     hb2GainStepAttack;                  /*!< AGC HB2 output attack gain step. Valid range from 2.4.5.6.8.9.10.11.12.13.14 */
} YxwAgcPeak_t;

/**
 * \brief Data structure to hold AGC power settings
 *  The evaluation software GUI for the product can be used to generate a structure with suggested settings.
 */
typedef struct
{
	uint8_t     powerEnableMeasurement;                 /*!< Enable the Rx power measurement block. (0/1) */
	uint8_t     powerInMux;                             /*!< 00: hb1   01: bbdc  10: rfir */
	uint8_t     powerMeasurementDuration;               /*!< Average power measurement duration = 2^(7+powerMeasurementDuration) clocks. Valid range from 0 to 15 */
	uint16_t 	powerAttackHigStep;						/*!< AGC pmd output attack gain step. Valid range from 2.4.5.6.8.9.10.11.12.13.14 */
	uint16_t    powerAttackLowStep;						/*!< AGC pmd output attack gain step. Valid range from 2.4.5.6.8.9.10.11.12.13.14 */
	uint16_t    powerRecoveryHigStep;					/*!< AGC pmd output recovery gain step. Valid range from 2.4.5.6.8.9.10.11.12.13.14 */
	uint16_t    powerRecoveryLowStep;					/*!< AGC pmd output recovery gain step. Valid range from 2.4.5.6.8.9.10.11.12.13.14 */
	uint16_t    powerOverHigThresh;						/*!< AGC pmd output over high threshold. Valid range from  0 to 16383 */
	uint16_t    powerOverLowThresh;						/*!< AGC pmd output over low threshold. Valid range from  0 to 16383 */
	uint16_t    powerUnderHigThresh;					/*!< AGC pmd output under High threshold. Valid range from  0 to 16383 */
	uint16_t    powerUnderLowThresh;					/*!< AGC pmd output under low threshold. Valid range from  0 to 16383 */
} YxwAgcPower_t;

/**
 * \brief Data structure to hold all AGC configuration settings for initialization
 *  The evaluation software GUI for the product can be used to generate a structure with suggested settings.
 */
typedef struct
{
	uint8_t     agcClkDiv;                          /*!< AGC clock (500M) division.(default:no division,1:divide by 2, 2:divide by 4) */
	uint8_t     agcPeakWaitCounter;                 /*!< AGC peak wait time. Valid range is from 0 to 15 (time=16* (agcPeakWaitTime+1) clocks) */
	uint32_t    agcGainUpdateCounter;               /*!< AGC gain update time (time = 128*( agcGainUpdateCounter+1)) */
	uint8_t     agcPreventGain;                     /*!< Prevent gain index from incrementing if peak thresholds are being exceeded */
	uint8_t     agcGainControlMode;                 /*!< Enable gain change based only on the signal peak threshold over-ranges. Power based AGC changes are disabled in this mode.Default value is 0 */

	YxwAgcPower_t agcPower;
	YxwAgcPeak_t agcPeak;

} YxwAgcCfg_t;
#endif

