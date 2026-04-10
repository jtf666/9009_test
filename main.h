
#ifndef _MAIN_
//#define _MAIN_
#define rx_fn
#define tx_fn
//#define k7_sks_mgc
//#define config_intface
//#define commond


#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_plat_hal.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_init.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_jesd204b.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_radio.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_cals.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_error.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_riscv.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_init.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_reg_dig.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_reg_ana.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_cals_types.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_rx.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_tx.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_hal.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_gpio.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_gpio_types.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_config.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_info.h"
#include "YXW9009-API-RISCV-V2.0/API/devices/yxw_plat/src/yxw_agc.h"


typedef int bool;
#define true 1
#define false 0
//extern YxwDevice_t g_hw_priv; 
int open_prg_dev(char grP, char grD, char grR);
int set_gpio_val(unsigned int maskSet, unsigned int maskClr);
unsigned int get_gpio_val(unsigned int inMask);
void my_usleep(unsigned int delay);
extern void  adrv9009_spi_write_reg( unsigned char * txbuf,  uint32_t bytes_numb );
extern void adrv9009_spi_read_reg( unsigned char * buf );
uint32_t  change_9009_rf_freq_rx(double nfreq);
uint32_t  change_9009_rf_freq_tx(double nfreq);
uint32_t  set_tx_att(uint8_t tx_att_0d1dB);
uint32_t  set_rx_att(uint8_t rx_att_0d1dB);
uint32_t  change_9009_rf_freq_rx1(double nfreq);
uint32_t  change_9009_rf_freq_tx1(double nfreq);
void  set_tx_att1(uint8_t tx_att_0d1dB);
void  set_rx_att1(uint8_t rx_att_0d1dB);
uint32_t set_9009_tx_outside_att(uint8_t dsa_att_value);
void get_rx_gain_and_mgc();
uint32_t set_9009_tx_attattout(double tx_att);
uint32_t set_1orx_power_low(YxwDevice_t *hw_priv);
uint32_t set_1rx_moni_power_low(YxwDevice_t *hw_priv);
uint32_t set_1t_power_low(YxwDevice_t *hw_priv);
uint32_t set_orx_firBW(YxwDevice_t *hw_priv,double firBW);
uint32_t set_orx_firBW_reg(YxwDevice_t *hw_priv,double firBW);
uint32_t set_zaibo_switch(YxwDevice_t *hw_priv,uint16_t zaiboswitch);
int tcp_client_send_msg(const char *ip,int port,const char *message);
int udp_client_init(const char *ip, int port); 
int udp_client_init_v2(); 
void udp_client_close();
int udp_client_init_v2_1(); 
int udp_client_init_v3();
void udp_client_close1();
int udp_client_send_data(const unsigned char *data, size_t len);
int udp_client_send_data1(const unsigned char *data, size_t len);
unsigned long get_current_ms(); 
int double_equals(double a, double b, double epsilon);
void autoreadreg(YxwDevice_t *hw_priv);
void create_default_9009_ip_config();
void create_default_9009_parameter_config() ;
bool is_number(const char *s);




/**
 * @brief 从配置文件读取值（自动判断数字或字符串）
 * 
 * @param section   Section 名
 * @param key       Key 名
 * @param out_str   输出字符串缓冲区
 * @param str_size  缓冲区大小
 * @param out_int   如果是数字，会解析存入这里
 * @param is_int    返回值：true=数字，false=字符串
 * @return true     找到
 * @return false    未找到
 */
bool get_ini_value(const char *section, const char *key,
                   char *out_str, size_t str_size,
                   int *out_int, bool *is_int);

/**
 * @brief 设置配置文件的值（自动判断数字或字符串）
 * 
 * @param section   Section 名
 * @param key       Key 名
 * @param new_value 新值（字符串形式）
 * @return true     成功
 * @return false    失败
 */
bool set_ini_value(const char *section, const char *key,
                   const char *new_value);


bool set_ini_value_double(const char *section, const char *key, double new_value);


//uint32_t set_9009_tx_outside_att(uint8_t dsa_att_value);
#define  EMIO_CX3E04_RESET_B  1<<5 // 54  // 0
#define  EMIO_CX3E04_CS_N     1<<6// 55  // 1
#define  EMIO_CX3E04_SCK       1<<8//56  // 2
#define  EMIO_CX3E04_MOSI      1<<7//57  // 3

#define  EMIO_U23_RX_9009_RST_N  1<<15// 58  // 4  F10
//#define  EMIO_U23_RX_9009_MOSI   1<<5// 59  // 5
//#define  EMIO_U23_RX_9009_MISO    1<<6//60  // 6
//#define  EMIO_U23_RX_9009_SCK   1<<7//  61  // 7
//#define  EMIO_U23_RX_9009_CS_N   1<<8 //62  // 8

// #define  EMIO_K7_UZ3_RX_CS      1<<9//  63  // 9/ AD25
// #define  EMIO_K7_UZ3_RX_CLK       1<<10//64  // 10  AE26
// #define  EMIO_K7_UZ3_RX_MOSI    1<<11//  65  // 11  AD30
// #define  EMIO_K7_UZ3_RX_MISO      1<<12//66  // 12  AE30

#define  EMIO_NUM_K7_CS   1<<11// 65  // 11
#define  EMIO_NUM_K7_SCK  1<<12// 66  // 12
#define  EMIO_NUM_K7_MOSI 1<<13// 67  // 13
#define  EMIO_NUM_K7_MISO 1<<14// 68  // 14

#define  EMIO_U37_TX_9009_RST_N  1<<0// 67  // 13  C7
//#define  EMIO_U37_TX_9009_MOSI   1<<14// 68  // 14
//#define  EMIO_U37_TX_9009_MISO   1<<15// 69  // 15
//#define  EMIO_U37_TX_9009_SCK    1<<16// 70  // 16
//#define  EMIO_U37_TX_9009_CS_N   1<<17// 71  // 17

// #define  EMIO_K7_UZ1_TX_CS       1<<18// 72  // 18   AB27
// #define  EMIO_K7_UZ1_TX_CLK     1<<19 // 73  // 19   AC27
// #define  EMIO_K7_UZ1_TX_MOSI     1<<20// 74  // 20   AJ30
// #define  EMIO_K7_UZ1_TX_MISO     1<<21// 75  // 21   AK30
//9009 载波以及收发放大器开关 4120
#define  EMIO_NUM_ZAIBO_SWITCH   1<<0
#define  EMIO_NUM_RX_FDQ_SWITCH   1<<1
#define  EMIO_NUM_TX_FDQ_SWITCH   1<<2



#endif

