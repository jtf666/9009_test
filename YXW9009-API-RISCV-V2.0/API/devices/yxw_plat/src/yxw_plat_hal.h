/**
* \file yxw_plat_hal.h
* \brief Contains prototypes and macro definitions for Private Y9009 HAL wrapper
 *        functions implemented in yxw_plat_hal.c
* \version: Ver 1.0
* \author :
* \date   : 2021/11/15 18:17
*
* Copyright 2021- Zkyxw Inc.
*/

#ifndef __YXW_PLAT_HAL_H__
#define __YXW_PLAT_HAL_H__

#include "yxw_info.h"
#include "yxw_plat_hal_types.h"
#include "yxw_types.h"
#include "yxw_config.h"
#include "../../../../../main.h"
/**
-1  - 姒涙ǹ顓诲鍝勫煑閹垫挸宓
 0  - 闂堟瑩绮幍鎾冲祪閿涘矂鍏樻稉宥嗗ⅵ閸楋拷
 1  - 閼锋潙鎳￠幍鎾冲祪(韫囧懘銆忕憰浣瑰ⅵ閸楁壆娈戦幍鎾冲祪閿涘苯瀵橀幏顒勬晩鐠囶垰瀵橀崥顐㈠毐閺佹澘鎮曠悰灞炬殶閿涘苯鍤遍弫棰佸▏閻€劑鏁婄拠顖ょ礉閸忋儱寮敍姘扁敄閹稿洭鎷￠妴浣界翻閸忋儴瀵栭崶鎾晩鐠囶垽绱濆鍌氱埗閿涙碍鐗庨崙鍡涙晩鐠囶垽绱濈搾鍛)
 2  - 缂佹挻鐏夐幍鎾冲祪(閸栧懎鎯堢拋锛勭暬閺佹澘锟借偐绮ㄩ弸婊愮礉鏉╂柨娲栭崐鍏煎ⅵ閸楀府绱扮紒鎾寸亯缁狙冨焼閻ㄥ嫭澧﹂崡锟)
 3  - 閻樿埖锟戒焦澧﹂崡锟(閸栧懎鎯堟稉顓㈡？鐠侊紕鐣绘潻鍥┾柤閿涘苯鐦庣涙ê娅掗悩鑸碉拷渚婄窗閸戣姤鏆熸潻鍥┾柤缁狙冨焼)
 4  - debug閹垫挸宓(閸欘垯浜掗弰顖欐崲娴ｆ洟娓剁憰浣圭叀閻娈戦敍灞剧槷婵″倷鍞惍浣虹椽閸愭瑨绻冪粙瀣╄厬閿涘矁顓绘稉鐑樼厙娴滄稑褰查懗鎴掔窗鐎佃壈鍤х紒鎾寸亯闁挎瑨顕ら惃鍕ⅵ閸楀府绱濇担鍡樻Ц娑撳秴绗囬張娑橀挬鐢憡澧﹂崡锟)

-1 FORCE
 0 NONE
 1 ERR:XXX
 2 RESULT:XXX
 3 STATUS:XXX
 4 DEBUG:XXX

**/                    
#define DBG_LEVEL_ALWAYS   -1
#define DBG_LEVEL_NONE		0
#define DBG_LEVEL_ERR		1
#define DBG_LEVEL_RESULT	2
#define DBG_LEVEL_STATUS	3
#define DBG_LEVEL_DEBUG		4

 #define YXW_DEBUG(debug, ...)                                            \
 	do {                                                                \
		if ( DBG_LEVEL_ALWAYS == debug) {                               \
				YXWHAL_Printf(__VA_ARGS__);                              \
			}                                                           \
		else if ( (Y9009Init_tx.Loglevel)>= (debug)) {                      \
				if(debug == DBG_LEVEL_ERR)                              \
					YXWHAL_Printf("\033[1;31;40m[ERR]\033[0m"),          \
					YXWHAL_Printf(__VA_ARGS__);                          \
				else if(debug == DBG_LEVEL_RESULT)                      \
					YXWHAL_Printf("[RESULT]"),YXWHAL_Printf(__VA_ARGS__); \
				else if(debug == DBG_LEVEL_STATUS)                      \
					YXWHAL_Printf("[STATUS]"),YXWHAL_Printf(__VA_ARGS__); \
				else if(debug == DBG_LEVEL_DEBUG)                       \
					YXWHAL_Printf("[DEBUG]"),YXWHAL_Printf(__VA_ARGS__);  \
			}                                                           \
 	} while(0)

#define IF_ERR_RETURN(a)                                     \
	if(a != YXW_NO_ACTION) {                                  \
		YXW_DEBUG(DBG_LEVEL_ERR,"%s,%d,(%d)SPI CONF ERROR\n", \
			__FUNCTION__,__LINE__,a);                        \
	  	return a;                                            \
	}

#define FUNCTION_ENTER(hw_priv) 	                                 \
	do {                                                             \
		YXW_DEBUG(DBG_LEVEL_DEBUG,"Enter:%s,%d\n",__func__,__LINE__); \
	}while(0)

#define FUNCTION_EXIT(hw_priv) 		                                 \
	do {                                                             \
		YXW_DEBUG(DBG_LEVEL_DEBUG,"Exit:%s,%d\n",__func__,__LINE__);  \
	}while(0)


/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_spiOpen(YxwDevice_t *hw_priv, uint32_t DeviceId);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_spiClose(YxwDevice_t *hw_priv);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_spiReadReg(YxwDevice_t *hw_priv, uint16_t addr, uint32_t *data);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_spiWriteReg(YxwDevice_t *hw_priv, uint16_t addr, uint32_t data);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_spiReadField(YxwDevice_t *hw_priv, uint16_t addr, uint32_t *fieldVal,
	uint8_t endBit, uint8_t statBit);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_spiWriteField(YxwDevice_t *hw_priv, uint16_t addr, uint32_t fieldVal,
	uint8_t endBit, uint8_t statBit);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_mdelay(uint32_t mdelay_time);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_WriteToLog(YxwDevice_t *hw_priv, yxwLogLevel_t loglevel,
	uint32_t errorCode, const char *comment);

yxwHalErr_t YXW_Memset(void* Addr, uint32_t Data, uint32_t len);

yxwHalErr_t YXW_Memcpy(void* dst, void *src, uint32_t len);

/**
 * \brief
 *
 * \param DeviceId Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_resetHw(uint32_t DeviceId);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_Tx1EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */

yxwHalErr_t YXW_Tx2EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode);
/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_Rx1EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_Rx2EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */
yxwHalErr_t YXW_ORx1EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode);

/**
 * \brief
 *
 * \param device Pointer to the Y9009 data structure containing settings
 * \param
 *
 * \retval YXWHAL_OK means function completed successfully, otherwise it will fail
 */

yxwHalErr_t YXW_ORx2EnableCtrl(YxwDevice_t *hw_priv, YxwHalChannelEnableMode_t mode);

#endif

