
/****< Insert User Includes Here >***/


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>  // 提供 gettimeofday() 的声明
#include <sys/socket.h>
#include <sys/un.h>
#include "main.h"
#include "k7_spi.h"
#include <linux/types.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <ctype.h>
#define CONFIG_FILE "/work/_config/9009_ip_config.ini"
#define CONFIG_FILE_PARAMETER "/tmp/9009_parameter_config.ini"

// 默认ip配置内容
const char *DEFAULT_IP_CONFIG =
    "[IP]\n"
    "ip_addr=192.168.88.6\n"
    "ip_port=12346\n"
    "\n"
    "[AP]\n"
    "ip_addr=192.168.88.111\n"
    "ip_port=12345\n"
	"\n"
	"[CP]\n"
    "ip_addr=192.168.88.2\n";


// 默认参数配置内容
const char *DEFAULT_PARAM_CONFIG =
    "[RX]\n"
    "freq=3000\n"
    "att_in=127\n"
    "att_out=0\n"
    "is_init_ok=0\n"
	"data_filter=60\n"
	"analog_filter=3600\n"
	"gain_control_mode=0\n"
    "\n"

	"[TX]\n"
    "freq=3000\n"
    "att_in=0\n"
    "att_out=0\n"
    "is_init_ok=0\n"
	"analog_filter=215\n"
    "carrier_switch=0\n";

#define LOCAL_IP   "192.168.88.2"       // eth0 IP
#define LOCAL_IP1   "192.168.88.2" 
#define DEST_IP    "192.168.88.111"
#define DEST_IP1    "192.168.88.6"
#define DEST_PORT 12345        // 替换成目标程序监听端口
#define DEST_PORT1 12346
#define MESSAGE     "Hello from client!"
#define BUF_SIZE 1024

#define MESSAGE_RX_initing  "MESSAGE_9009_RX_initing..."
#define MESSAGE_RX_init_failed "MESSAGE_9009_RX_init_failed!!!"
#define MESSAGE_RX_init_successfully "MESSAGE_9009_RX_init_successfully!!!"

#define MESSAGE_TX_initing  "MESSAGE_9009_TX_initing..."
#define MESSAGE_TX_init_failed "MESSAGE_9009_TX_init_failed!!!"
#define MESSAGE_TX_init_successfully "MESSAGE_9009_TX_init_successfully!!!"

//#define MESSAGE_9009_init_successfully "MESSAGE_9009_init_successfully!!!" 
//#define MESSAGE_9009_set_successfully "MESSAGE_9009_set_successfully!!!" 
#define TX_MAX_GAIN 4.35
#define MAX_COMMAND_LENGTH 100
#define DELAY_TIME_MS 100
#define DELAY_TIME_MS1 1000
int sockfd,sockfd1;
struct sockaddr_in local_addr, servaddr;
struct sockaddr_in local_addr1, servaddr1;  
unsigned char init_9009_success[] = {0x7e, 0x0,0x01, 0x7e};  //  7e 01 01 7e   (1) 01 init (2) 01 succ  00 failed
unsigned char init_9009_failed[] = {0x7e, 0x0,0x00, 0x7e};
unsigned char set_9009_success[] = {0x7e, 0x02, 0x01, 0x7e};   //  7e 02 01 7e  (1) 02 set  (2) 01 succ 00 faild
unsigned char set_9009_failed[] = {0x7e, 0x02, 0x00, 0x7e};
unsigned char set_9009_rx_freq_success[] = {0x7e, 0x01,0x01,0x7e};   
unsigned char set_9009_rx_freq_failed[] = {0x7e,0x1,0x00, 0x7e};
unsigned char set_9009_tx_freq_success[] = {0x7e, 0x02,0x01,0x7e};   
unsigned char set_9009_tx_freq_failed[] = {0x7e,0x02,0x00, 0x7e};
unsigned char set_9009_tx_att_success[] = {0x7e,0x03, 0x01, 0x7e};  
unsigned char set_9009_tx_att_failed[] = {0x7e, 0x03,0x00, 0x7e};
unsigned char set_9009_tx_attout_success[] = {0x7e,0x04,0x01, 0x7e};   
unsigned char set_9009_tx_attout_failed[] = {0x7e,0x04,0x00, 0x7e};
unsigned char set_9009_tx_zaibo_success[] = {0x7e, 0x05,0x01, 0x7e};   
unsigned char set_9009_tx_zaibo_failed[] = {0x7e,0x05,0x00, 0x7e};


unsigned char set_9009_orx_firBW_success[] = {0x7e, 0x06,0x01, 0x7e};   
unsigned char set_9009_orx_firBW_failed[] = {0x7e,0x06,0x00, 0x7e};
unsigned char set_9009_rx_att_success[] = {0x7e, 0x07,0x01, 0x7e};   
unsigned char set_9009_rx_att_failed[] = {0x7e,0x07,0x00, 0x7e};
unsigned char set_9009_tx_attattout_success[] = {0x7e, 0x08,0x01, 0x7e};   
unsigned char set_9009_tx_attattout_failed[] = {0x7e,0x08,0x00, 0x7e};
//config spi
static const char *spiDevice1 = "/dev/spidev2.0";  //tx
static const char *spiDevice2 = "/dev/spidev3.0";  //rx
static uint32_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;
unsigned char receive_data1[6];
unsigned char receive_data2[6];
int fd1,fd2;
struct spi_ioc_transfer spi1;
struct spi_ioc_transfer spi2;
int prgCtrl;

uint8_t k7_spi_ctrl =0;
uint64_t  freq_9009_tx_Hz = 3000000000ULL ;
uint64_t  freq_9009_tx_Hz_new = 3000000000ULL ;
uint64_t  freq_9009_rx_Hz = 3000000000ULL ;
uint64_t  freq_9009_rx_Hz_new = 3000000000ULL ;
uint8_t  att_9009_tx_db = 0 ;
uint8_t  att_9009_tx_db_new = 0 ;
uint8_t  att_9009_rx_db = 0 ;
uint8_t  att_9009_rx_db_new = 0 ;
uint8_t read_curr_rx_gain_index,read_curr_tx_gain_index;
uint32_t rxGainIndex;
//config 9009
uint8_t adrv9009_select;
YxwDevice_t g_hw_priv_rx; 
YxwDevice_t g_hw_priv_tx; 
YxwDevice_t *hw_priv_tx=NULL;
YxwDevice_t *hw_priv_rx=NULL;
uint32_t yxwAction = YXWHAL_OK;

/**********************************************************/
/**********************************************************/
/********** Talise Data Structure Initializations ********/
/**********************************************************/
/**********************************************************/

/**********************************************************/
/**********************************************************/
/********** Talise Data Structure Initializations ********/
/**********************************************************/
/**********************************************************/


// typedef struct {
//     uint32_t gain_dB;
//     uint32_t numFirCoefs;
//     uint16_t coefs[128];  // 假设最大支持128个系数
// } YxwFir_t;

// typedef struct
// {
//     sint16_t gain_dB;         
//     uint32_t numFirCoefs;    
//     sint16_t *coefs;
// } YxwFir_t;

sint16_t orxfir_60[72]={25,26,2,-39,-75,-73,-17,78,160,166,62,-121,-289,-324,-162,155,469,579,356,-154,
    -711,-979,-712,73,1038,1637,1390,185,-1554,-2945,-2987,-1020,2896,7881,12492,15259,
    15259,12492,7881,2896,-1020,-2987,-2945,-1554,185,1390,1637,1038,73,-712,-979,-711,-154,
    356,579,469,155,-162,-324,-289,-121,62,166,160,78,-17,-73,-75,-39,2,26,25};
sint16_t orxfir_40[72]={-9,-31,-55,-72,-68,-37,24,101,174,214,194,101,-57,-246,-410,-487,-427,
    -211,132,526,856,999,859,405,-304,-1118,-1811,-2132,-1859,-864,845,3106,
    5621,8005,9868,10889,10889,9868,8005,5621,3106,845,-864,-1859,-2132,-1811,-1118,-304,405,
    859,999,856,526,132,-211,-427,-487,-410,-246,-57,101,194,214,174,101,24,-37,-68,-72,-55,-31,-9};
sint16_t orxfir_48[72]={103, 17, -70, -117, -40, 107, 154, 6, -207, -229, 28, 317, 280, -125, -486, -337, 267, 685, 352, -503, -946, -325, 853, 1266, 202, -1406, -1698, 80, 2338, 2363, -754, -4360, -3884, 3070, 13812, 21879, 21879, 13812, 3070, -3884, -4360, -754, 2363, 2338, 80, -1698, -1406, 202, 1266, 853, -325, -946, -503, 352, 685, 267, -337, -486, -125, 280, 317, 28, -229, -207, 6, 154, 107, -40, -117, -70, 17, 103};
sint16_t orxfir_24[72]={53, 81, 83, 48, -23, -112, -184, -201, -140, -3, 177, 334, 400, 321, 94, -227, -537, -710, -645, -311, 224, 800, 1202, 1231, 782, -95, -1180, -2116, -2498, -1990, -439, 2056, 5135, 8246, 10773, 12189, 12189, 10773, 8246, 5135, 2056, -439, -1990, -2498, -2116, -1180, -95, 782, 1231, 1202, 800, 224, -311, -645, -710, -537, -227, 94, 321, 400, 334, 177, -3, -140, -201, -184, -112, -23, 48, 83, 81, 53};
sint16_t orxfir_12[72]={6, -23, -58, -95, -127, -147, -145, -116, -57, 30, 139, 256, 366, 446, 479, 445, 335, 147, -109, -410, -724, -1006, -1208, -1282, -1185, -887, -372, 353, 1260, 2302, 3413, 4514, 5524, 6364, 6966, 7279, 7279, 6966, 6364, 5524, 4514, 3413, 2302, 1260, 353, -372, -887, -1185, -1282, -1208, -1006, -724, -410, -109, 147, 335, 445, 479, 446, 366, 256, 139, 30, -57, -116, -145, -147, -127, -95, -58, -23, 6};
sint16_t orxfir_6[72]={43, 67, 92, 116, 135, 147, 148, 134, 102, 51, -19, -107, -210, -322, -435, -540, -627, -685, -701, -666, -570, -406, -170, 138, 515, 952, 1439, 1961, 2499, 3034, 3544, 4008, 4407, 4723, 4942, 5054, 5054, 4942, 4723, 4407, 4008, 3544, 3034, 2499, 1961, 1439, 952, 515, 138, -170, -406, -570, -666, -701, -685, -627, -540, -435, -322, -210, -107, -19, 51, 102, 134, 148, 147, 135, 116, 92, 67, 43};
sint16_t orxfir_3[72]={-38, -61, -87, -117, -150, -185, -219, -252, -281, -304, -318, -319, -307, -278, -229, -159, -67, 50, 190, 355, 541, 749, 974, 1214, 1464, 1720, 1977, 2229, 2471, 2697, 2902, 3081, 3231, 3346, 3425, 3464, 3464, 3425, 3346, 3231, 3081, 2902, 2697, 2471, 2229, 1977, 1720, 1464, 1214, 974, 749, 541, 355, 190, 50, -67, -159, -229, -278, -307, -319, -318, -304, -281, -252, -219, -185, -150, -117, -87, -61, -38};
sint16_t orxfir_1d5[72]={-36, -55, -78, -104, -132, -162, -191, -217, -240, -256, -263, -259, -242, -209, -157, -86, 6, 120, 257, 415, 593, 790, 1003, 1229, 1465, 1705, 1945, 2181, 2407, 2618, 2809, 2976, 3115, 3222, 3295, 3332, 3332, 3295, 3222, 3115, 2976, 2809, 2618, 2407, 2181, 1945, 1705, 1465, 1229, 1003, 790, 593, 415, 257, 120, 6, -86, -157, -209, -242, -259, -263, -256, -240, -217, -191, -162, -132, -104, -78, -55, -36};
sint16_t orxfir_d750[72]={26, 40, 57, 79, 104, 133, 166, 204, 247, 294, 346, 403, 464, 529, 598, 671, 746, 824, 905, 986, 1068, 1151, 1232, 1312, 1389, 1464, 1534, 1600, 1661, 1715, 1763, 1804, 1838, 1863, 1880, 1888, 1888, 1880, 1863, 1838, 1804, 1763, 1715, 1661, 1600, 1534, 1464, 1389, 1312, 1232, 1151, 1068, 986, 905, 824, 746, 671, 598, 529, 464, 403, 346, 294, 247, 204, 166, 133, 104, 79, 57, 40, 26};


unsigned int *prgmap_addr_ctrl;
unsigned int *_prgmap_addr_ctrl;
unsigned int pinReg0, pinReg1, pinMask;
int open_prg_dev(char grP, char grD, char grR)
{
	prgCtrl = open("/dev/mem", O_RDWR | O_SYNC);
	if (prgCtrl < 0) {
		fprintf(stderr, "Cannot open GPIO ...\n");
		return -1;
	}
	unsigned int devPin[2] = {0x41210000, 0x41210000};  //{0x41200000, 0x41210000};
 	prgmap_addr_ctrl = (int*)mmap(NULL, 256 ,PROT_READ|PROT_WRITE,MAP_SHARED, prgCtrl, devPin[grP]); //EMIO: 0xe000a000 ;;; axi-gpio :: 0x41210000=gpio1021.. or 0x41200000=gpio1018..);

	if (prgmap_addr_ctrl == MAP_FAILED) {
		fprintf(stderr, "Cannot control GPIO ...\n");
		return -1;
	}

	pinReg0 = *((volatile unsigned int*)prgmap_addr_ctrl);
	pinReg1 = *((volatile unsigned int*)prgmap_addr_ctrl+1);
	return 0;
}
//direct io axi-gpio
int set_gpio_val(unsigned int maskSet, unsigned int maskClr)
{
	pinMask = maskSet | maskClr;
	if(pinReg1 & pinMask)
	{
		pinReg1 |= pinMask;
		pinReg1 ^= pinMask;
	//fprintf(stderr, "write reg1 ... ...\n");
		*((volatile unsigned int*)prgmap_addr_ctrl+1) = pinReg1;
	}
	pinReg0 |= pinMask;
	pinReg0 ^= maskClr;
	//fprintf(stderr, "write reg0 ... %08x...\n", pinReg0);
	*((volatile unsigned int*)prgmap_addr_ctrl+0) = pinReg0;
	return 0;
}
unsigned int get_gpio_val(unsigned int inMask)
{
	if((pinReg1 & inMask) != inMask)
	{
		pinReg1 |= inMask;
		*((volatile unsigned int*)prgmap_addr_ctrl+1) = pinReg1;
		//fprintf(stderr, "write reg1 ... ...%08x\n", pinReg1);
	}
	pinReg0 = *((volatile unsigned int*)prgmap_addr_ctrl+0);
	return pinReg0 & inMask;
}
//config gpio 4120
unsigned int _pinReg0, _pinReg1, _pinMask;
int _open_prg_dev(char grP, char grD, char grR)
{

	unsigned int devPin[2] = {0x41200000, 0x41200000};  //{0x41200000, 0x41210000};
 	_prgmap_addr_ctrl = (int*)mmap(NULL, 256 ,PROT_READ|PROT_WRITE,MAP_SHARED, prgCtrl, devPin[grP]); //EMIO: 0xe000a000 ;;; axi-gpio :: 0x41210000=gpio1021.. or 0x41200000=gpio1018..);

	if (_prgmap_addr_ctrl == MAP_FAILED) {
		fprintf(stderr, "Cannot control GPIO ...\n");
		return -1;
	}

	_pinReg0 = *((volatile unsigned int*)_prgmap_addr_ctrl);
	_pinReg1 = *((volatile unsigned int*)_prgmap_addr_ctrl+1);
	return 0;
}
//direct io axi-gpio
int _set_gpio_val(unsigned int maskSet, unsigned int maskClr)
{
	_pinMask = maskSet | maskClr;
	if(_pinReg1 & _pinMask)
	{
		_pinReg1 |= _pinMask;
		_pinReg1 ^= _pinMask;
	//fprintf(stderr, "write reg1 ... ...\n");
		*((volatile unsigned int*)_prgmap_addr_ctrl+1) = _pinReg1;
	}
	_pinReg0 |= _pinMask;
	_pinReg0 ^= maskClr;
	//fprintf(stderr, "write reg0 ... %08x...\n", pinReg0);
	*((volatile unsigned int*)_prgmap_addr_ctrl+0) = _pinReg0;
	return 0;
}
unsigned int _get_gpio_val(unsigned int inMask)
{
	if((_pinReg1 & inMask) != inMask)
	{
		_pinReg1 |= inMask;
		*((volatile unsigned int*)_prgmap_addr_ctrl+1) = _pinReg1;
		//fprintf(stderr, "write reg1 ... ...%08x\n", pinReg1);
	}
	_pinReg0 = *((volatile unsigned int*)_prgmap_addr_ctrl+0);
	return _pinReg0 & inMask;
}
void adrv9009_spi_write_reg( unsigned char * txbuf,  uint32_t bytes_numb )
{    
	//printf("\r\nbegin spi write is begin\r\n");
	// printf("begin spi write\r\n");
	memset(receive_data1,0,6);
	memset(receive_data2,0,6); 
    if( adrv9009_select == 0 )  //RX
	{
     spi2.tx_buf = (unsigned long)txbuf;
	 spi2.rx_buf = (unsigned long)receive_data2;
     //spi2.len = sizeof(bytes_numb);
	 spi2.len = sizeof(receive_data2);
		if (ioctl(fd2, SPI_IOC_MESSAGE(1), &spi2) < 0) {
			perror("SPI transfer for device 2 failed\r\n");
			//return -1;
		}
	// printf("\r\nTransmit from device tx 2: \r\n");
    }
    else
    {
     spi1.tx_buf = (unsigned long)txbuf;
	 spi1.rx_buf = (unsigned long)receive_data1;
    // spi1.len = sizeof(bytes_numb); 
	 spi1.len = sizeof(receive_data1);
	 //printf("0x%x ", txbuf[0]);
		if (ioctl(fd1, SPI_IOC_MESSAGE(1), &spi1) < 0) {
			perror("SPI transfer for device 1 failed\r\n");
			//return -1;
		} 
	//printf("\r\nTransmit from device tx 1: \r\n");
    }
	//printf("\r\nbegin spi write is over\r\n");
}
void adrv9009_spi_read_reg( unsigned char * buf )
{   
	// printf("\r\nbegin spi read is write\r\n");
	memset(receive_data1,0,6);
	memset(receive_data2,0,6); 
	unsigned char send_data[6];
	memset(send_data,0,6);
	unsigned char reg_rd_data =0;
/*
	spi_2bytes = 0 ;
	spi_2bytes = spi_2bytes | buf[0] ;  //提取出MSB字节

	spi_2bytes = spi_2bytes << 8 ;
	spi_2bytes = spi_2bytes | buf[1] ;  //提取出中间字节
  */  
	send_data[0] = buf[0];
	send_data[1] = buf[1];

	 //printf("\r\nbegin spi read is write  spi before========");
	// for (int i = 0; i < sizeof(send_data); ++i) {
    //    printf("0x%x ", send_data[i]);
		
   // }
    if( adrv9009_select == 0 )  //RX
	{  
		//spi2.tx_buf = (unsigned long)send_data;
		spi2.tx_buf = (unsigned long)buf;
		spi2.rx_buf = (unsigned long)receive_data2;
		spi2.len = sizeof(receive_data2);
		if (ioctl(fd2, SPI_IOC_MESSAGE(1), &spi2) < 0) {
			perror("SPI transfer for device 2 failed\r\n");
			//return -1;
		}
		 usleep(10);
			//printf("\r\nReceived from device rx 2: \r\n");
			buf[2]          = receive_data2[2];
			buf[3]          = receive_data2[3];
			buf[4]          = receive_data2[4];
			buf[5]          = receive_data2[5];
		//	printf("\r\nbegin spi read is read--------------------------------");
		//	for (int i = 0; i < sizeof(receive_data2); ++i) {
		//	printf("0x%x ", receive_data2[i]);
			
		//}
		
		//printf("\n");
    }
    else
    {   
		spi1.tx_buf = (unsigned long)buf;
		spi1.rx_buf = (unsigned long)receive_data1;
		spi1.len = sizeof(receive_data1); 
		if (ioctl(fd1, SPI_IOC_MESSAGE(1), &spi1) < 0) {
			perror("SPI transfer for device 1 failed\r\n");
			//return -1;
		} 
		 usleep(10);
		//printf("\r\nReceived from device rx 1:\r\n ");
			   buf[2]          = receive_data1[2];
		       buf[3]          = receive_data1[3];
			   buf[4]          = receive_data1[4];
			   buf[5]          = receive_data1[5];
	//	printf("\r\nbegin spi read is read-----------------------------------");
	//	for (int i = 0; i < sizeof(receive_data1); ++i) {
		//	printf("0x%x ", receive_data1[i]);
	//	}
	//	printf("\n");
		
    }
	//printf("\r\nbegin spi read is over\r\n");
}
void my_usleep(unsigned int delay)
{
    delay = delay * 800;
    while(delay>0){
        delay--;
    }
}
yxwHalErr_t YXW_resetHw_my(YxwDevice_t *hw_priv)
{
    yxwHalErr_t halStatus = YXWHAL_OK;
	if( adrv9009_select == 0 )  //U23 RX
	{
		set_gpio_val(EMIO_U23_RX_9009_RST_N, 0);
		my_usleep(200000);
		set_gpio_val(0, EMIO_U23_RX_9009_RST_N);
		my_usleep(200000);
		set_gpio_val(EMIO_U23_RX_9009_RST_N, 0);
		my_usleep(200000);
	}
	else //U37 TX
	{
		set_gpio_val(EMIO_U37_TX_9009_RST_N, 0);
		my_usleep(200000);
		set_gpio_val(0, EMIO_U37_TX_9009_RST_N);
		my_usleep(200000);
		set_gpio_val(EMIO_U37_TX_9009_RST_N, 0);
		my_usleep(200000);
	}

	return halStatus;
}
void executeCommand(const char *command) {
    printf("执行命令: %s\n", command);
    system(command);
}
int yxw_rx(void)
{   
	printf("yxw rx init ...\n\r"); 
    K7_spi_write_9009_rx_agc_restart(0x1f);
	uint8_t i;
	adrv9009_select = 0 ;
	YXW_Memset(&g_hw_priv_rx,0,sizeof(YxwDevice_t));
	hw_priv_rx=&g_hw_priv_rx;
    YXW_resetHw_my(hw_priv_rx);
	//printf("reset hw success\n\r");

	 yxwAction = YXW_resetDevice(hw_priv_rx);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_resetDevice() failed\n\r");
		return  -1;
    }
	yxwAction = YXW_getBootStatus(hw_priv_rx);
    if (yxwAction != YXWHAL_OK)
    {
      printf("error: YXW_getBootStatus() failed\n\r");
		return  -1;  
    }
	//printf("YXW_getBootStatus\n\r");
		/*DownLoad Y9009 Firmware*/
	yxwAction = YXW_loadFirmware(hw_priv_rx);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_loadFirmware() failed\n\r");
		return  -1; 
    }
	//printf("YXW_loadFirmware\n\r");
	/*Wait Write Riscv Profile To Y9009 device Done */
	yxwAction = YXW_waitFirmwareReady(hw_priv_rx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_waitFirmwareReady() failed\n\r");
		//return  -1; 
    }
	//printf("YXW_waitFirmwareReady\n\r");
	
	/*Write Riscv initial configure To y9009 device */
    yxwAction = YXW_initialize(hw_priv_rx, &Y9009Init_rx);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_initialize() failed\n\r");
		return  -1; 
    }
   // printf("YXW_initialize\n\r");
	/*Write Riscv Profile To Y9009 device*/
	yxwAction = YXW_writeRiscvProfile(hw_priv_rx,&Y9009Init_rx);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_writeRiscvProfile() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_writeRiscvProfile\n\r");
	/*Wait Riscv Profile To Y9009 device Done */
	yxwAction = YXW_waitProfileDone(hw_priv_rx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {   
		YXW_GetRiscvTraceEvent(hw_priv_rx);
        printf("error: YXW_waitProfileDone() failed\n\r");
		return  -1; 
    }
	//printf("YXW_waitProfileDone\n\r");	
	
	/***********************************************/
    /**3.Set RF AND AUX PLL Frequencies, Default 5.5GHz ****/
    /***********************************************/
	
    yxwAction = YXW_setPllFrequency(hw_priv_rx,YXW_RF_PLL, freq_9009_rx_Hz);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_setRFPllFrequency() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_setRFPllFrequency\n\r");	
	/*Wait RF PLL Frequencies Set Done */
	yxwAction = YXW_waitPllsLockDone(hw_priv_rx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_waitRFPllsLockDone() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_waitRFPllsLockDone\n\r");	

	yxwAction = YXW_setPllFrequency(hw_priv_rx,YXW_AUX_PLL, freq_9009_rx_Hz);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_setAUXPllFrequency() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_setAUXPllFrequency\n\r");	
	
	/*Wait RF PLL Frequencies Set Done */
	yxwAction = YXW_waitPllsLockDone(hw_priv_rx,DELAY_TIME_MS); 
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_waitAUXPllsLockDone() failed\n\r");
		return  -1; 
    }
	//printf("YXW_waitAUXPllsLockDone\n\r");	
	
    /****************************************************/
    /**** 4.Run Y9009 ARM Initialization Calibrations ***/
    /****************************************************/
	YXW_spiWriteField(hw_priv_rx,0xa9,1,11,11);
	YXW_spiWriteField(hw_priv_rx,0xa9,1,20,20);
     yxwAction = YXW_runInitCals(hw_priv_rx, Y9009Init_rx.initcaltype);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_runInitCals() failed\n\r");
		return  -1; 
    }
	//printf("YXW_runInitCals\n\r");	
    yxwAction = YXW_waitInitCalsDone(hw_priv_rx, 1000);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_waitInitCalsDone() failed\n\r");
		return  -1; 
    }
	// printf("YXW_waitInitCalsDone\n\r");
    /***************************************************/
    /**** 5.Enable  Y9009 JESD204B  ***/
    /***************************************************/	
	/*Write JESD204B Config To Y9009 device*/
	yxwAction = YXW_writeJes204bConfig(hw_priv_rx,&Y9009Init_rx);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_writeJes204bConfig() failed\n\r");
		return  -1; 
    }	
	
	//printf("YXW_writeJes204bConfig\n\r");	
	/*Wait Write JESD204B Config To Y9009 device Done */

    YXW_spiWriteReg(hw_priv_rx,0x230,0xeee48e4);

	yxwAction = YXW_waitJes204bConfigDone(hw_priv_rx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_waitJes204bConfigDone() failed\n\r");
		//return  -1; 
    }
	//printf("YXW_waitJes204bConfigDone\n\r");
// 	uint32_t mem,mem1,mem2,mem3,mem4,mem5,mem6;	

//   yxwAction=YXW_spiReadMem(hw_priv_rx,0x25006050,&mem);
//   printf("0x25006050  0 0x%X\r\n",mem); 
	YXW_spiWriteField(hw_priv_rx,0x20d,1,4,4);
	YXW_spiWriteField(hw_priv_rx,0x20d,1,0,0);
    /*************************************************/
    /**** 6.Enable SYSREF to Y9009 JESD204B Framer ***/

  
    yxwAction = YXW_enableSysRefToFramer(hw_priv_rx, &Y9009Init_rx);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_enableSysRefToFramer() failed\n\r");
		return  -1; 
    }
	//printf("YXW_enableSysRefToFramer\n\r");
    YXW_spiWriteField(hw_priv_rx,REG_SYSREF0,0x1,15,15);
	/*==============================
	 此时9009已经稳定地向FPGA发送K码，在这之前9009的SerDes输出还不正常，导致FPGA SerDes输入数据路径内部的BUFFER溢出。
	所以此时需要复位FPGA SerDes数据路径。仅需要复位GT_CHANNEL，不能复位CPLL和QPLL。
	JESD204_PHY IP核的 t/rx_reset_gt端口仅复位各个串行通道，但不复位CPLL和QPLL。
	==============================*/
//	printf("************** k7_spi_ctrl is %x\n\r" , k7_spi_ctrl );
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT3_RX_JESD204_CORE_RESET );
	usleep(10000);
	K7_spi_write_reg( k7_spi_ctrl |=  BIT3_RX_JESD204_CORE_RESET );
	usleep(10000);
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT3_RX_JESD204_CORE_RESET );
	usleep(10000);


	/*==============================
	打开 JESD204_RX 对SYSREF的使能
	==============================*/
	K7_spi_write_reg( k7_spi_ctrl |=  BIT4_RX_FPGA_SYSREF_EN );
	usleep(10);


	/*** < User Sends SYSREF Here > ***/

	for(i=0; i<1; i++)
	{
		
		usleep(50 * 1000);
	}

	/*==============================
	关闭 JESD204_RX 和 JESD204_TX 对SYSREF的使能
	==============================*/
		K7_spi_write_reg( k7_spi_ctrl &= ~BIT4_RX_FPGA_SYSREF_EN );
	usleep(10);

	/*==============================
	复位JESD204 IP核
	==============================*/
/*	K7_spi_write_reg( k7_spi_ctrl |=  BIT3_JESD204_CORE_RESET );
	usleep(10);
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT3_JESD204_CORE_RESET );
	usleep(10000); */




    /* Function to turn radio on, Enables transmitters and receivers */
    yxwAction = YXW_radioOn(hw_priv_rx,YXW_SPI_MODE);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_radioOn() failed\n\r");
		return  -1; 
    }

	//printf("YXW_radioOn\n\r");	

	
	//0x25006050 0   rx sync hign

	// /*Low Power Mode To Y9009 device*/
	yxwAction = YXW_lp(hw_priv_rx, &Y9009Init_rx);
	if (yxwAction != YXWHAL_OK)
	{
		printf("error: YXW_lp() failed\n\r");
		return  -1; 
	}
	/*PLL Mode select To Y9009 device*/
	yxwAction = YXW_pllmode(hw_priv_rx, &Y9009Init_rx);
	if (yxwAction != YXWHAL_OK)
	{
		printf("error: YXW_pllmode() failed\n\r");
		return  -1; 
	}
	YXW_spiWriteField(hw_priv_rx, 0x43, 0x10882bf, 31, 0);
	YXW_spiWriteField(hw_priv_rx, 0x44, 0x10082bf, 31, 0);
	//yxwAction=YXW_rx0A_orx0D(hw_priv_rx);
  if (yxwAction != YXWHAL_OK)
	{
		printf("error: YXW_rx0A_orx0D() failed\n\r");
		return  -1; 
	}
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT3_RX_JESD204_CORE_RESET );
	usleep(10000);
	K7_spi_write_reg( k7_spi_ctrl |=  BIT3_RX_JESD204_CORE_RESET );
	usleep(10000);
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT3_RX_JESD204_CORE_RESET );
	usleep(10000);

	K7_spi_write_reg( k7_spi_ctrl |=  BIT4_RX_FPGA_SYSREF_EN );
	usleep(10);


	/*** < User Sends SYSREF Here > ***/

	for(i=0; i<1; i++)
	{
		
		usleep(50 * 1000);
	}

	/*==============================
	关闭 JESD204_RX 和 JESD204_TX 对SYSREF的使能
	==============================*/
		K7_spi_write_reg( k7_spi_ctrl &= ~BIT4_RX_FPGA_SYSREF_EN );
	usleep(10);

   yxwAction = YXW_setRxGainCtrlPin(hw_priv_rx,0x3,&Y9009Init_rx.rx.rxGainCtrlPinInfo);
     if (yxwAction != YXWHAL_OK)
     {
         printf("error: BZ_InfoShow_rx() failed\n\r");
 		return  -1; 
     }
     
	 yxwAction=YXW_spiWriteMem(hw_priv_rx,0x25006050,0);
	 YXW_spiWriteField(hw_priv_rx,0x3e,0x2000,31,0);
	 YXW_spiWriteField(hw_priv_rx,0x82,3600,11,0);
	 YXW_spiWriteField(hw_priv_rx,0x83,3600,27,16);
	// YXW_GetRiscvTraceEvent(hw_priv_rx);
	// yxwAction = YXW_waitSync204b(hw_priv_rx, 1000);
	// if (yxwAction != YXWHAL_OK)
    // {
    //     printf("error: YXW_waitSync204b() failed\n\r");
	// 	return  -1; 
    // }
	
	// printf("YXW_waitSync204b\n\r");	
	//   yxwAction = YXW_InfoShow_rx(hw_priv_rx);
    // if (yxwAction != YXWHAL_OK)
    // {
    //     printf("error: YXW_InfoShow_rx() failed\n\r");
	// 	return  -1; 
    // }
   
    
       K7_spi_write_9009_rx_agc_restart(0xff);
	  K7_spi_write_9009_rx_agc_restart(0x00);
	
	
 
}
int yxw_tx(void)
{    
    printf("yxw tx init ...\n\r"); 
	_set_gpio_val(0,EMIO_NUM_ZAIBO_SWITCH);
     uint8_t i;
	adrv9009_select = 1 ;
	YXW_Memset(&g_hw_priv_tx,0,sizeof(YxwDevice_t));
	hw_priv_tx=&g_hw_priv_tx;
    YXW_resetHw_my(hw_priv_tx);
	//printf("reset hw success\n\r");	

	 yxwAction = YXW_resetDevice(hw_priv_tx);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_resetDevice() failed\n\r");
		return  -1;
    }

	yxwAction = YXW_getBootStatus(hw_priv_tx);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_getBootStatus() failed\n\r");
		return  -1;  
    }
	//printf("YXW_getBootStatus\n\r");

	/*DownLoad Y9009 Firmware*/
	yxwAction = YXW_loadFirmware(hw_priv_tx);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_loadFirmware() failed\n\r");
		return  -1; 
    }
	//printf("YXW_loadFirmware\n\r");
	
	/*Wait Write Riscv Profile To Y9009 device Done */
	yxwAction = YXW_waitFirmwareReady(hw_priv_tx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_waitFirmwareReady() failed\n\r");
		return  -1; 
    }
	//printf("YXW_waitFirmwareReady\n\r");
	
	/*Write Riscv initial configure To y9009 device */
    yxwAction = YXW_initialize(hw_priv_tx, &Y9009Init_tx);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_initialize() failed\n\r");
		return  -1; 
    }
   // printf("YXW_initialize\n\r");
	
	/*Write Riscv Profile To Y9009 device*/
	yxwAction = YXW_writeRiscvProfile(hw_priv_tx,&Y9009Init_tx);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_writeRiscvProfile() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_writeRiscvProfile\n\r");
	
	/*Wait Riscv Profile To Y9009 device Done */
	yxwAction = YXW_waitProfileDone(hw_priv_tx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_waitProfileDone() failed\n\r");
		return  -1; 
    }
	//printf("YXW_waitProfileDone\n\r");	
	
	/***********************************************/
    /**3.Set RF AND AUX PLL Frequencies, Default 5.5GHz ****/
    /***********************************************/
    yxwAction = YXW_setPllFrequency(hw_priv_tx,YXW_RF_PLL, freq_9009_tx_Hz);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_setRFPllFrequency() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_setRFPllFrequency\n\r");	
	
	/*Wait RF PLL Frequencies Set Done */
	yxwAction = YXW_waitPllsLockDone(hw_priv_tx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_waitRFPllsLockDone() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_waitRFPllsLockDone\n\r");	

	yxwAction = YXW_setPllFrequency(hw_priv_tx,YXW_AUX_PLL, freq_9009_tx_Hz);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_setAUXPllFrequency() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_setAUXPllFrequency\n\r");	
	
	/*Wait RF PLL Frequencies Set Done */
	yxwAction = YXW_waitPllsLockDone(hw_priv_tx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_waitAUXPllsLockDone() failed\n\r");
		return  -1; 
    }
	//printf("YXW_waitAUXPllsLockDone\n\r");	
		
    /****************************************************/
    /**** 4.Run Y9009 ARM Initialization Calibrations ***/
    /****************************************************/
    YXW_setCalsChannel(hw_priv_tx,YXW_CALS_CHANNEL_1);

    yxwAction = YXW_runInitCals(hw_priv_tx, Y9009Init_tx.initcaltype);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_runInitCals() failed\n\r");
		return  -1; 
    }
	//printf("YXW_runInitCals\n\r");	
	
    yxwAction = YXW_waitInitCalsDone(hw_priv_tx, 1000);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_waitInitCalsDone() failed\n\r");
		return  -1; 
    }
	//printf("YXW_waitInitCalsDone\n\r");
    /***************************************************/
    /**** 5.Enable  Y9009 JESD204B  ***/
    /***************************************************/
	/*Write JESD204B Config To Y9009 device*/
	yxwAction = YXW_writeJes204bConfig(hw_priv_tx,&Y9009Init_tx);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_writeJes204bConfig() failed\n\r");
		return  -1; 
    }	
	//printf("YXW_writeJes204bConfig\n\r");	
	
	/*Wait Write JESD204B Config To Y9009 device Done */
	yxwAction = YXW_waitJes204bConfigDone(hw_priv_tx,DELAY_TIME_MS);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_waitJes204bConfigDone() failed\n\r");
		return  -1; 
    }
	//printf("YXW_waitJes204bConfigDone\n\r");	

	YXW_spiWriteField(hw_priv_tx,0x20d,1,15,15);
	YXW_spiWriteField(hw_priv_tx,0x20d,1,13,13);

    /*************************************************/
    /**** 6.Enable SYSREF to Y9009 JESD204B Framer ***/
    /*************************************************/

    yxwAction = YXW_enableSysRefToFramer(hw_priv_tx, &Y9009Init_tx);
    if (yxwAction != YXWHAL_OK)
    {
        printf("error: YXW_enableSysRefToFramer() failed\n\r");
		return  -1; 
    }
	  YXW_spiWriteField(hw_priv_tx,REG_SYSREF0,0x1,15,15);
	//printf("YXW_enableSysRefToFramer\n\r");
	//    yxwAction = YXW_radioOn(hw_priv_tx,YXW_SPI_MODE);	
    // return 0;
	/*==============================
	 此时9009已经稳定地向FPGA发送K码，在这之前9009的SerDes输出还不正常，导致FPGA SerDes输入数据路径内部的BUFFER溢出。
	所以此时需要复位FPGA SerDes数据路径。仅需要复位GT_CHANNEL，不能复位CPLL和QPLL。
	JESD204_PHY IP核的 t/rx_reset_gt端口仅复位各个串行通道，但不复位CPLL和QPLL。
	==============================*/
//	printf("************** k7_spi_ctrl is %x\n\r" , k7_spi_ctrl );
	// K7_spi_write_reg( k7_spi_ctrl &= ~BIT6_TX_JESD204_CORE_RESET );
	// usleep(10000);
	// K7_spi_write_reg( k7_spi_ctrl |=  BIT6_TX_JESD204_CORE_RESET );
	// usleep(10000);
	// K7_spi_write_reg( k7_spi_ctrl &= ~BIT6_TX_JESD204_CORE_RESET );
	// usleep(10000);


	/*==============================
	打开 JESD204_RX 和 JESD204_TX 对SYSREF的使能
	==============================*/
	K7_spi_write_reg( k7_spi_ctrl |=  BIT7_TX_FPGA_SYSREF_EN );
	usleep(10);


	/*** < User Sends SYSREF Here > ***/

	for(i=0; i<1; i++)
	{
		
		usleep(50 * 1000);
	}
	/*==============================
	关闭 JESD204_RX 对SYSREF的使能
	==============================*/
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT7_TX_FPGA_SYSREF_EN );
	usleep(10);
	

    /* Function to turn radio on, Enables transmitters and receivers */
	   yxwAction = YXW_radioOn(hw_priv_tx,YXW_SPI_MODE);
    if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_radioOn() failed\n\r");
		return  -1; 
    }

	// /*PLL Mode select To Y9009 device*/

	
	/*Low Power Mode To Y9009 device*/
	yxwAction = YXW_lp(hw_priv_tx, &Y9009Init_tx);
	if (yxwAction != YXWHAL_OK)
	{
		printf("error: YXW_lp() failed\n\r");
		return  -1; 
	 }
		yxwAction = YXW_pllmode(hw_priv_tx, &Y9009Init_tx);
	if (yxwAction != YXWHAL_OK)
	{
		printf("error: YXW_pllmode() failed\n\r");
		return  -1; 
	}
	yxwAction = YXW_waitSync204b(hw_priv_tx, 1000);
	if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_waitSync204b() failed\n\r");
		return  -1; 
    }
	//printf("YXW_waitSync204b\n\r");
	YXW_spiWriteReg(hw_priv_tx,0x231,0x40000100);	
	YXW_spiWriteField(hw_priv_tx,0x37,1,15,15);
    YXW_spiWriteField(hw_priv_tx, 0x1a,0x1102, 31, 0);
    // printf("\n\rInfoShow_tx\n\r");
	// yxwAction = YXW_InfoShow_tx(hw_priv_tx);
    // if (yxwAction != YXWHAL_OK)
    // {
    //    printf("error: YXW_InfoShow_tx() failed\n\r");
	// 	return  -1; 
    // }
		yxwAction = YXW_setRadioCtrlPinMode(hw_priv_tx, YXW_RXOFF,YXW_ORXOFF,YXW_TX1);
	  if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_setRadioCtrlPinMode() failed\n\r");
		return  -1; 
    }
	

    
	

}
int main(int argc,char *argv[])
{	
	printf("\n\r \n\r \n\r \n\r \n\r \n\r \n\r");
	printf("---------------------- xyg terminal new sdk 250918 tx lvbo cp guochan 9009\n\r");
	printf("---------------------- build time 260415  22\n\r");
	int run_rx_flag = 0;
    int run_tx_flag = 0;
	
	if(argc == 1)
	{
    run_rx_flag = 1;
	run_tx_flag = 1;

	}
	else if(argc == 2){
     if(strcmp(argv[1], "0") == 0)
	 {
         run_rx_flag = 1;
	     run_tx_flag = 0;
		 printf("---Initialize only rx!\n\r");
	 }
	  if(strcmp(argv[1], "1") == 0)
	 {
		 run_rx_flag = 0;
	     run_tx_flag = 1;
		 printf("---Initialize only tx!\n\r");
	 }
	}

	memset(&spi1, 0, sizeof(struct spi_ioc_transfer));
	memset(&spi2, 0, sizeof(struct spi_ioc_transfer));
	 // 打开 SPI 设备
  if (run_tx_flag)
    {
    fd1 = open(spiDevice1, O_RDWR);
    if (fd1 < 0) {
        perror("Could not open SPI device1 tx");
        return -1;
     }
	}
 if (run_rx_flag)
    {
	  fd2 = open(spiDevice2, O_RDWR);
    if (fd2 < 0) {
        perror("Could not open SPI device2 rx");
        return -1;
     }
    }
	 // 配置 SPI 参数（第一个设备）
    uint8_t mode1 = SPI_MODE_0;
    ioctl(fd1, SPI_IOC_WR_MODE, &mode1);

    uint8_t bits1 = 8;
    ioctl(fd1, SPI_IOC_WR_BITS_PER_WORD, &bits1);

    uint32_t speed1 = 5000000;
    ioctl(fd1, SPI_IOC_WR_MAX_SPEED_HZ, &speed1);

    // 配置 SPI 参数（第二个设备）
    uint8_t mode2 = SPI_MODE_0;
    ioctl(fd2, SPI_IOC_WR_MODE, &mode2);

    uint8_t bits2 = 8;
    ioctl(fd2, SPI_IOC_WR_BITS_PER_WORD, &bits2);

    uint32_t speed2 = 5000000;  // 第二个设备的速度为 1 MHz
    ioctl(fd2, SPI_IOC_WR_MAX_SPEED_HZ, &speed2);


    spi1.delay_usecs = 0;
    spi1.speed_hz = speed1;
    spi1.bits_per_word = bits1;

   
    spi2.delay_usecs = 0;
    spi2.speed_hz = speed2;
    spi2.bits_per_word = bits2;
	
	
   // printf("open spi success\n\r");

	
	int16_t ADRV_temperature = 0 ;
	uint8_t  print_adrv_temp_flag = 0 ;
	
	k7_spi_ctrl = 0 ;

	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "/tmp/socket_9009m");
	unlink("/tmp/socket_9009m");

		// Create a unix domain socket_9009.
	int fd;
	if((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		printf( "Receiver: Cannot create socket_9009m !!!\n" );
		return -1;
	}

	// Bind the socket_9009 to the address.
	if(bind(fd, (struct sockaddr *)&address, sizeof(address)) != 0) {
		printf("Receiver: Cannot bind socket_9009m\n");
		return -1;
	}
   // tcp
   #if 0
	int sock;
    struct sockaddr_in server_addr;

    // 创建套接字
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return 1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return 1;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        return 1;
    }
	#endif
	//udp
	// system("ifconfig lo up");
	system(" touch /tmp/9009_parameter_config.ini");
      create_default_9009_ip_config();
	  system(" rm /tmp/9009_parameter_config.ini");
	  create_default_9009_parameter_config();
    //  if (udp_client_init_v2() != 0)
    //      return 1;
	// if (udp_client_init_v2_1() != 0)
    //      return 1;
	if (udp_client_init_v3() != 0)
          return 1;
//
	printf("SOCKET START ... ... \n");
	
    open_prg_dev(0, 0, 0);
	_open_prg_dev(0, 0, 0);
	uint8_t K7_up_flag_UZ3 = 0 ;
	uint8_t K7_up_flag_UZ1 = 0 ;
	uint8_t K7_up_flag_old_UZ3 = 0 ;
	uint8_t K7_up_flag_old_UZ1 = 0 ;
	int rx_successful_flag = 0;
    int tx_successful_flag = 0;
	unsigned long last_send_time = 0;
    unsigned long now;
	

	while(1)
	{   
	
		
		sleep(1);

	
	
		K7_up_flag_old_UZ3 = K7_up_flag_UZ3 ;
		if( K7_spi_read_reg() == 0x5C )
		{
			K7_up_flag_UZ3 = 1 ;
				
		}
		else
		{
			K7_up_flag_UZ3 = 0 ;
			printf("k7 detection failed!!! \n\r");	
		}
		if( (K7_up_flag_old_UZ3 == 0) && (K7_up_flag_UZ3 == 1) )
		{   
			printf("----------------------\n\r");
			printf("k7 detection successful! \n\r");
			printf("----------------------\n\r");			
			rx_successful_flag = 0;
			tx_successful_flag = 0;	
		//	printf("----------------------K7 UZ3 RX restart detected.\n\r");
		//	printf("----------------------9009 U23 RX need init.\n\r");
		//	usleep(100000);
		
		
#ifdef rx_fn  
       if(run_rx_flag == 1)
    {         
				adrv9009_select = 0 ;  //9009 U23 RX 对应 K7 UZ3 RX
			/*==============================
			GT参考时钟锁定稳定之后，要先释放GT参考时钟的时钟使能。GT_REFCLK_BUFG作为了GT_CHANNEL的T/RXUSRCLK。
			然后释放JESD204_PHY IP核的 t/rx_sys_reset，释放之后，它开始复位GT内部的PLL和各个串行通道。
			==============================*/

				 _set_gpio_val(0,EMIO_NUM_RX_FDQ_SWITCH);
				K7_spi_write_reg( k7_spi_ctrl &= ~BIT0_RX_GT_REFCLK_EN );
				

				
			//   K7_spi_write_reg( k7_spi_ctrl |= BIT2_TX_GT_REFCLK_EN ); //K7需要对它反相    
				usleep(10000);

				//此时检查K7 ILA的状态，如果所有ILA都能触发出来，说明时钟都正常工作起来了

				K7_spi_write_reg( k7_spi_ctrl &= ~BIT1_RX_JESD204_PHY_SYS_RESET );
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl |=  BIT1_RX_JESD204_PHY_SYS_RESET );				
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl &= ~BIT1_RX_JESD204_PHY_SYS_RESET );				
				usleep(10000);

			/*==============================
			ADRV9009初始化
			==============================*/
			//先将ORX反馈路径的驱动源连接到50欧姆电阻到地
		//	set_gpio_val(0, EMIO_NUM_SW_CTL);
		//	set_gpio_val(0, EMIO_NUM_SW_EN);

			// flag_9009_init_success =0;
			// cnt_9009_init_success =0;
			  // 发送消息
//    send(sock, MESSAGE_RX_initing, strlen(MESSAGE_RX_initing), 0);
//    printf("Message sent: %s\n", MESSAGE_RX_initing);
			yxwAction = yxw_rx();
			//YXW_spiWriteReg(hw_priv_tx,0x231,0x40000100);
			if (yxwAction != YXWHAL_OK)
			{
					printf("----------------------ADRV9009 U23_RX Initialization failed!\n\r");
					 set_ini_value_double("RX", "is_init_ok", 0);
				//	send(sock, MESSAGE_RX_init_failed, strlen(MESSAGE_RX_init_failed), 0);
                 //   printf("Message sent: %s\n", MESSAGE_RX_init_failed);
				 rx_successful_flag = 0;
			}
			else
			{
				printf("----------------------ADRV9009 U23_RX Initialization successfully!\n\r");
				 set_ini_value_double("RX", "is_init_ok", 1);
				//   send(sock, MESSAGE_RX_init_successfully, strlen(MESSAGE_RX_init_successfully), 0);
                 //  printf("Message sent: %s\n", MESSAGE_RX_init_successfully);
				 rx_successful_flag = 1;
			}
			
			_set_gpio_val(EMIO_NUM_RX_FDQ_SWITCH,0);
		  K7_spi_write_reg( k7_spi_ctrl |= BIT0_RX_GT_REFCLK_EN );
			
	}
		
			
#endif

#ifdef tx_fn  
    if(run_tx_flag == 1)  
  {    
		adrv9009_select = 1 ;  //9009 U37 TX 对应 K7 UZ1 TX
	
		
			/*==============================
			GT参考时钟锁定稳定之后，要先释放GT参考时钟的时钟使能。GT_REFCLK_BUFG作为了GT_CHANNEL的T/RXUSRCLK。
			然后释放JESD204_PHY IP核的 t/rx_sys_reset，释放之后，它开始复位GT内部的PLL和各个串行通道。
			==============================*/
				_set_gpio_val(0,EMIO_NUM_TX_FDQ_SWITCH);
			    K7_spi_write_reg( k7_spi_ctrl &= ~BIT2_TX_GT_REFCLK_EN );
				//K7_spi_write_reg( k7_spi_ctrl &= ~BIT2_TX_GT_REFCLK_EN );
				usleep(10000);

				//此时检查K7 ILA的状态，如果所有ILA都能触发出来，说明时钟都正常工作起来了

				K7_spi_write_reg( k7_spi_ctrl &= ~BIT5_TX_JESD204_PHY_SYS_RESET );
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl |=  BIT5_TX_JESD204_PHY_SYS_RESET );
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl &= ~BIT5_TX_JESD204_PHY_SYS_RESET );
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl &= ~BIT6_TX_JESD204_CORE_RESET );
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl |=  BIT6_TX_JESD204_CORE_RESET );
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl &= ~BIT6_TX_JESD204_CORE_RESET );
				usleep(10000);
			/*==============================
			ADRV9009初始化
			==============================*/
			//先将ORX反馈路径的驱动源连接到50欧姆电阻到地
		//	set_gpio_val(0, EMIO_NUM_SW_CTL);
		//	set_gpio_val(0, EMIO_NUM_SW_EN);
			
		//	yxw_tx();
			//YXW_spiWriteReg(hw_priv_tx,0x231,0x40000100);
			// send(sock, MESSAGE_TX_initing, strlen(MESSAGE_TX_initing), 0);
          //  printf("Message sent: %s\n", MESSAGE_TX_initing);
          //  _set_gpio_val(0,EMIO_NUM_ZAIBO_SWITCH);
			yxwAction = yxw_tx();
			//YXW_spiWriteReg(hw_priv_tx,0x231,0x40000100);
			if (yxwAction != YXWHAL_OK)
			{
				printf("----------------------ADRV9009 U37_TX Initialization failed!\n\r");
				set_ini_value_double("TX", "is_init_ok", 0);
			//	send(sock, MESSAGE_TX_init_failed, strlen(MESSAGE_TX_init_failed), 0);
             //   printf("Message sent: %s\n", MESSAGE_TX_init_failed);
			    tx_successful_flag = 0;
			}
			else
			{
				printf("----------------------ADRV9009 U37_TX Initialization successfully!\n\r");
				set_ini_value_double("TX", "is_init_ok", 1);
			//	send(sock, MESSAGE_TX_init_successfully, strlen(MESSAGE_TX_init_successfully), 0);
             //   printf("Message sent: %s\n", MESSAGE_TX_init_successfully);
			   tx_successful_flag = 1;
			}
			_set_gpio_val(EMIO_NUM_TX_FDQ_SWITCH,0);
			K7_spi_write_reg( k7_spi_ctrl |= BIT2_TX_GT_REFCLK_EN );
  }		
	#endif
	   //    adrv9009_select = 0 ;	
	// 	   printf("\n\rInfoShow_rx\n\r");	
	// 	   yxwAction = YXW_InfoShow_rx(hw_priv_rx);
    // if (yxwAction != YXWHAL_OK)
    // {
    //     printf("error: YXW_InfoShow_rx() failed\n\r");
	// 	return  -1; 
    // }
      //     adrv9009_select = 1 ;
	//  printf("\n\rInfoShow_tx\n\r");
	// yxwAction = YXW_InfoShow_tx(hw_priv_tx);
    // if (yxwAction != YXWHAL_OK)
    // {
    //    printf("error: YXW_InfoShow_tx() failed\n\r");
	// 	return  -1; 
    // }
	
	 
		  		
			
		} 
              now = get_current_ms();
			   if (now - last_send_time >= 2000)
		  {
				
//#if 0
               	uint32_t fieldVal = 0; 
				adrv9009_select = 1 ;  
              YXW_spiReadField(hw_priv_tx, 0x42b,&fieldVal,0,0);			  
			  if(!fieldVal)  
			   {  
				printf("!!!read_tx_0x42b 0x%x \n",fieldVal);
				system("echo 1 > /work/again_tx.txt");
				  tx_successful_flag = 0;
                  set_ini_value_double("TX", "is_init_ok", 0);
				_set_gpio_val(0,EMIO_NUM_TX_FDQ_SWITCH);
			    K7_spi_write_reg( k7_spi_ctrl &= ~BIT2_TX_GT_REFCLK_EN );
				usleep(10000);


				K7_spi_write_reg( k7_spi_ctrl &= ~BIT5_TX_JESD204_PHY_SYS_RESET );
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl |=  BIT5_TX_JESD204_PHY_SYS_RESET );
				usleep(10000);
				K7_spi_write_reg( k7_spi_ctrl &= ~BIT5_TX_JESD204_PHY_SYS_RESET );
				usleep(10000);

				yxwAction = yxw_tx();
				if (yxwAction != YXWHAL_OK)
				{
					printf("----------------------ADRV9009 again U37_TX Initialization failed!\n\r");
					set_ini_value_double("TX", "is_init_ok", 0);

					tx_successful_flag = 0;
				}
				else
				{
					printf("----------------------ADRV9009 again U37_TX Initialization successfully!\n\r");
					set_ini_value_double("TX", "is_init_ok", 1);
				    tx_successful_flag = 1;
				}
				_set_gpio_val(EMIO_NUM_TX_FDQ_SWITCH,0);
				K7_spi_write_reg( k7_spi_ctrl |= BIT2_TX_GT_REFCLK_EN );
			   }
	//     #endif
			  	if(rx_successful_flag == 1 && tx_successful_flag == 1)
					{  
					//   send(sock, MESSAGE_9009_init_successfully, strlen(MESSAGE_9009_init_successfully), 0);          
					// tcp_client_send_msg("127.0.0.1",12345,MESSAGE_9009_init_successfully);
					// udp_client_send(MESSAGE_9009_init_successfully);
					udp_client_send_data(init_9009_success, sizeof(init_9009_success));
					udp_client_send_data1(init_9009_success, sizeof(init_9009_success));
					//  printf("###Message sent: %s\n", MESSAGE_9009_init_successfully);	
					}
					else{
						udp_client_send_data(init_9009_failed, sizeof(init_9009_failed));
						udp_client_send_data1(init_9009_failed, sizeof(init_9009_failed));

					}
					last_send_time = now;
			}
	          char dev,autoreg,debugdata,info,cal,rx_gain_mode;
	         char paras[128];
	        double nfreq, natt,nattout,attattout,data_bw,moni_bw;
	       char *pptr, *vptr, *ppsave, *vpsave;
	       uint16_t iatt,iattout,iattattout,rxgainmmgc,readsksgain;
           uint32_t rr_data,wr_data,rt_data,wt_data,r_mem_val,w_mem_val,r_mem_addr,w_mem_addr,debugdata_point;   //rr   1_r :read  2_r:rev
	       uint16_t rr_addr,wr_addr,rt_addr,wt_addr,off,zaibo;
		   YxwTxDdsTestToneCfg_t config;
           YxwTxDdsAttenCfg_t ddsAtten;
           memset(paras, '\0', sizeof(paras));
			//printf("!!!!!!!wait paras ...  ... \n");
			if(recvfrom(fd, paras, sizeof(paras), MSG_DONTWAIT, NULL, NULL) > 0) {
				printf( "9009m got new paras: %s\n\r" , paras);
				pptr = strtok_r(paras, " ", &ppsave);


			
				while(pptr != NULL)
				{  
				
					vptr = strtok_r(pptr, "=", &vpsave);
					if(strcmp(vptr, "DEV")==0)
					{
						vptr = strtok_r(NULL, "=", &vpsave);
						dev = strcmp(vptr, "0");

						//printf("--------dev: %d\n", dev);
					}
					if(strcmp(vptr, "FREQ")==0)
					{   
						
						//printf("set freq: ... ... ....");
						vptr = strtok_r(NULL, "=", &vpsave);
						nfreq = atof(vptr);
						printf("dev: %d\n", dev);
						if(dev) { 
								if (run_tx_flag)	{
								yxwAction = change_9009_rf_freq_tx(nfreq);
								set_ini_value_double("TX", "freq", nfreq);
											if (yxwAction != YXWHAL_OK)
											{
											udp_client_send_data(set_9009_tx_freq_failed, sizeof(set_9009_tx_freq_failed));
											}
										else{
											udp_client_send_data(set_9009_tx_freq_success, sizeof(set_9009_tx_freq_success));
										}
						        	}else 	printf("---Not Current Channel!!!\n");							
								}
						else {  
							   if (run_rx_flag) {
									yxwAction = change_9009_rf_freq_rx(nfreq);
									set_ini_value_double("RX", "freq", nfreq);
											if (yxwAction != YXWHAL_OK)
												{
												udp_client_send_data(set_9009_rx_freq_failed, sizeof(set_9009_rx_freq_failed));
												}
											else{
												udp_client_send_data(set_9009_rx_freq_success, sizeof(set_9009_rx_freq_success));
											}
								}else 	printf("---Not Current Channel!!!\n");
						}
						//printf("val :: %s\n", vptr);
					}
					if(strcmp(vptr, "ATT")==0)
					{
						vptr = strtok_r(NULL, "=", &vpsave);
						natt = atof(vptr);
						iatt = (int)(natt);
						printf("dev: %d\n", dev);
						if(dev) 
						  {  
							if (run_tx_flag){
							yxwAction = set_tx_att(iatt);
							set_ini_value_double("TX", "att_in", iatt);
							  if (yxwAction != YXWHAL_OK)
									{
                                      udp_client_send_data(set_9009_tx_att_failed, sizeof(set_9009_tx_att_failed));
									}
								else{
									udp_client_send_data(set_9009_tx_att_success, sizeof(set_9009_tx_att_success));
								   }
							}else 	printf("---Not Current Channel!!!\n");
						  }
						else
						{  
							if (run_rx_flag){
							yxwAction = set_rx_att(iatt);
							set_ini_value_double("RX", "att_in", iatt);
						  if (yxwAction != YXWHAL_OK)
									{
                                      udp_client_send_data(set_9009_rx_att_failed, sizeof(set_9009_rx_att_failed));
									}
								else{
									udp_client_send_data(set_9009_rx_att_success, sizeof(set_9009_rx_att_success));
								   }
							}else 	printf("---Not Current Channel!!!\n");
						
						}
					//printf("val :: %d\n", iatt);
					}
					if(strcmp(vptr, "ATTOUT")==0)
					{  
						vptr = strtok_r(NULL, "=", &vpsave);
						nattout = atof(vptr);
						printf("dev: %d\n", dev);
						if(dev) 
						//set_tx_att_rx(iatt);
						//else
						 {
							if (run_tx_flag)	{
								yxwAction = set_9009_tx_outside_att(nattout);
								set_ini_value_double("TX", "att_out", nattout);
									if (yxwAction != YXWHAL_OK)
											{
											udp_client_send_data(set_9009_tx_attout_failed, sizeof(set_9009_tx_attout_failed));
											}
										else{
											udp_client_send_data(set_9009_tx_attout_success, sizeof(set_9009_tx_attout_success));
										}
								}else 	printf("---Not Current Channel!!!\n");
						}
						else
						   printf("---RX att_out not control!!!\n");

					//printf("val :: %d\n", iatt);
					}
						if(strcmp(vptr, "ATTATTOUT")==0)
					{
					  
					
					   	vptr = strtok_r(NULL, "=", &vpsave);
						attattout = atof(vptr);
						//iatt = (int)(natt*10);
						iattattout = (int)(attattout);
						//printf("set att: ... ... ....%s\n", vptr);
						printf("dev: %d\n", dev);
						if(dev) 
						{
							if (run_tx_flag){
								yxwAction =set_9009_tx_attattout(attattout);
							if (yxwAction != YXWHAL_OK)
										{
										udp_client_send_data(set_9009_tx_attattout_failed, sizeof(set_9009_tx_attattout_failed));
										}
									else{
										udp_client_send_data(set_9009_tx_attattout_success, sizeof(set_9009_tx_attattout_success));
									}
							}else 	printf("---Not Current Channel!!!\n");
						}
						else
						{ 
							if (run_rx_flag){
                          yxwAction = set_rx_att(iatt);
						     if (yxwAction != YXWHAL_OK)
									{
                                      udp_client_send_data(set_9009_rx_att_failed, sizeof(set_9009_rx_att_failed));
									}
								else{
									udp_client_send_data(set_9009_rx_att_success, sizeof(set_9009_rx_att_success));
								   }
							}else 	printf("---Not Current Channel!!!\n");
						}
						
					}
					if(strcmp(vptr, "RXGAINMGC")==0)
					{
						if (run_rx_flag){
						vptr = strtok_r(NULL, "=", &vpsave);
						rxgainmmgc = atof(vptr);
						get_rx_gain_and_mgc();
                        }else 	printf("---Not Current Channel!!!\n");
					
					}
							if(strcmp(vptr, "READSKSGAIN")==0)
					{   
						if (run_rx_flag){
						vptr = strtok_r(NULL, "=", &vpsave);
						readsksgain = atof(vptr);
						K7_spi_read_reg_sks_agc_rx();
                        }else 	printf("---Not Current Channel!!!\n");
					
					}
					if(strcmp(vptr, "SETDATABW")==0)
					{  
						if (run_rx_flag){
						adrv9009_select = 0 ; 
						vptr = strtok_r(NULL, "=", &vpsave);
						data_bw = atof(vptr);
	                yxwAction = set_orx_firBW_reg(hw_priv_rx,data_bw);
					 set_ini_value_double("RX", "data_filter", data_bw);
					
					     if (yxwAction != YXWHAL_OK)
									{
                                      udp_client_send_data(set_9009_orx_firBW_failed, sizeof(set_9009_orx_firBW_failed));
									}
								else{
									udp_client_send_data(set_9009_orx_firBW_success, sizeof(set_9009_orx_firBW_success));
									printf("set_data_bw %.2fM ok\n",data_bw);
								   }
					//printf("val :: %d\n", iatt);
					  }else 	printf("---Not Current Channel!!!\n");
					}
	// 					uint32_t mem;
	//  yxwAction=YXW_spiWriteMem(hw_priv_rx,0x25006050,0);
					if(strcmp(vptr, "RTREG")==0)
					{   
					if (run_tx_flag) {
						vptr = strtok_r(NULL, "=", &vpsave);
						rt_addr = atof(vptr);
						adrv9009_select = 1 ; 
					YXW_spiReadReg(hw_priv_tx,rt_addr,&rt_data);
					printf("read_tx_reg 0x%x value = 0x%x\n",rt_addr,rt_data);

                      }else 	printf("---Not Current Channel!!!\n");
					//printf("val :: %d\n", iatt);
					}
					if(strcmp(vptr, "WTREG")==0)
					{  
						if (run_tx_flag) { 
						 //uint16_t w_addr;
						vptr = strtok_r(NULL, "=", &vpsave);
						wt_addr = atof(vptr);
						}else 	printf("---Not Current Channel!!!\n");
					}
						if(strcmp(vptr, "WTVAL")==0)
					{   
						if (run_tx_flag) { 
						adrv9009_select = 1 ; 
						vptr = strtok_r(NULL, "=", &vpsave);
						wt_data = atof(vptr);
						if(wt_addr) 
	               YXW_spiWriteReg(hw_priv_tx,wt_addr,wt_data);
					printf("write_tx_reg 0x%x value = 0x%x\n",wt_addr,wt_data);
					//printf("val :: %d\n", iatt);
					  }else 	printf("---Not Current Channel!!!\n");
					}
						if(strcmp(vptr, "RMEM")==0)
					{   
					    if (run_rx_flag) { 
						vptr = strtok_r(NULL, "=", &vpsave);
						r_mem_addr = atof(vptr);
						adrv9009_select = 0 ; 
						printf("111\n");
					YXW_spiReadMem(hw_priv_rx,r_mem_addr,&r_mem_val);
					printf("read_rx_mem 0x%x value = 0x%x\n",r_mem_addr,r_mem_val);
                       }else 	printf("---Not Current Channel!!!\n");

					//printf("val :: %d\n", iatt);
					}
					if(strcmp(vptr, "WMEM")==0)
					{   
						if (run_rx_flag) { 
						 //uint16_t w_addr;
						vptr = strtok_r(NULL, "=", &vpsave);
						w_mem_addr = atof(vptr);
					 }else 	printf("---Not Current Channel!!!\n");
					}
						if(strcmp(vptr, "WMEMVAL")==0)
					{   
						if (run_rx_flag) { 
						adrv9009_select = 0 ; 
						vptr = strtok_r(NULL, "=", &vpsave);
						w_mem_val = atof(vptr);
						if(w_mem_addr) 
	               YXW_spiWriteMem(hw_priv_rx,w_mem_addr,w_mem_val);
					printf("write_rx_mem 0x%x value = 0x%x\n",w_mem_addr,w_mem_val);
					//printf("val :: %d\n", iatt);
					}else 	printf("---Not Current Channel!!!\n");
					
					}
						if(strcmp(vptr, "RRREG")==0)
					{   
					  if (run_rx_flag) { 
						vptr = strtok_r(NULL, "=", &vpsave);
						rr_addr = atof(vptr);
					
						adrv9009_select = 0 ; 
					YXW_spiReadReg(hw_priv_rx,rr_addr,&rr_data);
					printf("read_rx_reg 0x%x value = 0x%x\n",rr_addr,rr_data);
					 }else 	printf("---Not Current Channel!!!\n");
					}

				if(strcmp(vptr, "WRREG")==0)
					{  
						  if (run_rx_flag) {  
						 //uint16_t w_addr;
						vptr = strtok_r(NULL, "=", &vpsave);
						wr_addr = atof(vptr);
					   }else 	printf("---Not Current Channel!!!\n");
					}
						if(strcmp(vptr, "WRVAL")==0)
					{  
						  if (run_rx_flag) {  
						adrv9009_select = 0 ; 
						vptr = strtok_r(NULL, "=", &vpsave);
						wr_data = atof(vptr);
						if(wr_addr) 
	               YXW_spiWriteReg(hw_priv_rx,wr_addr,wr_data);
					printf("write_rx_reg 0x%x value = 0x%x\n",wr_addr,wr_data);
					//printf("val :: %d\n", iatt);
					 }else 	printf("---Not Current Channel!!!\n");
					}
                   if(strcmp(vptr, "DDST")==0)
					{   
					    adrv9009_select = 1 ; 
					
						vptr = strtok_r(NULL, "=", &vpsave);
						config.txTone1Freq_kHz = atof(vptr);
						config.txTone2Freq_kHz = atof(vptr);
		
					}
						if(strcmp(vptr, "DDSA")==0)
					{   
					    adrv9009_select = 1 ; 

						vptr = strtok_r(NULL, "=", &vpsave);
						ddsAtten.txTone0Atten = atof(vptr);
						ddsAtten.txTone1Atten = atof(vptr);
						ddsAtten.txTone2Atten = atof(vptr);
						ddsAtten.txTone3Atten = atof(vptr);
						
					 YXW_setDDsFreqConfig(hw_priv_tx,YXW_TX1, config, ddsAtten);
					 printf("setDDS SUCCESS\n");


					}

					if(strcmp(vptr, "DDSO")==0)
					{   
					    adrv9009_select = 1 ; 

						vptr = strtok_r(NULL, "=", &vpsave);
						off = atof(vptr);

					 YXW_setDdsOff(hw_priv_tx);
					 printf("setDDS OFF\n");

					}

					if(strcmp(vptr, "ZBSWITCH")==0)
					{   
					    if (run_tx_flag){
						adrv9009_select = 1 ; 

						vptr = strtok_r(NULL, "=", &vpsave);
						zaibo = atof(vptr);
						{
                           yxwAction = set_zaibo_switch(hw_priv_tx,zaibo);
						   set_ini_value_double("TX", "carrier_switch", zaibo);
						      if (yxwAction != YXWHAL_OK)
									{
                                      udp_client_send_data(set_9009_tx_zaibo_failed, sizeof(set_9009_tx_zaibo_failed));
									}
								else{
									udp_client_send_data(set_9009_tx_zaibo_success, sizeof(set_9009_tx_zaibo_success));
								   }
						}
                       }else 	printf("---Not Current Channel!!!\n");
                     
					}

					if(strcmp(vptr, "AUTOREG")==0)
					{   
						  vptr = strtok_r(NULL, "=", &vpsave);	
						 autoreg = strcmp(vptr, "0"); 
					     if(autoreg) {
						   adrv9009_select = 1 ; 
						   printf("autoreg tx\r\n");						   					 
                           autoreadreg(hw_priv_tx);
						 }else{
							adrv9009_select = 0 ; 
							 printf("autoreg rx\r\n");
                           autoreadreg(hw_priv_rx);
						 }
					}
						if(strcmp(vptr, "DEBUGDATA")==0)
					{   
					          vptr = strtok_r(NULL, "=", &vpsave);	
						     // debugdata = strcmp(vptr, "0");
							 debugdata_point = atof(vptr); 
					   if(dev) {
								adrv9009_select = 1 ; 
								 printf("debugdata tx\r\n");
								YXW_spiGetJesdDebugData(hw_priv_tx,0x10080000,debugdata_point,1,0,4096);
							} 
					   else{
								adrv9009_select = 0 ; 
								 printf("debugdata rx\r\n");
								YXW_spiGetJesdDebugData(hw_priv_rx,0x10080000,debugdata_point,1,0,4096);
					     }
					}
						if(strcmp(vptr, "SETMONIBW")==0)
					{
					if(dev)
					{   
						if (run_tx_flag) {
							adrv9009_select = 1 ; 
							vptr = strtok_r(NULL, "=", &vpsave);
							moni_bw = atof(vptr);
							set_ini_value_double("TX", "analog_filter", moni_bw);
							yxwHalErr_t halError = YXWHAL_OK;
							YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
						halError = YXW_spiWriteField(hw_priv_tx,0x7a,moni_bw,9,1);
						yxwRetAct = YXW_ApiErrHandler(hw_priv_tx,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
						halError = YXW_spiWriteField(hw_priv_tx,0x7a,moni_bw,25,17);
						yxwRetAct = YXW_ApiErrHandler(hw_priv_tx,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
						}else 	printf("---Not Current Channel!!!\n");
					}
					else{
                         
						if (run_rx_flag) {
							adrv9009_select = 0 ; 
							vptr = strtok_r(NULL, "=", &vpsave);
							moni_bw = atof(vptr);
							set_ini_value_double("RX", "analog_filter", moni_bw);
							yxwHalErr_t halError = YXWHAL_OK;
							YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;
						halError = YXW_spiWriteField(hw_priv_rx,0x82,moni_bw,11,0);
						yxwRetAct = YXW_ApiErrHandler(hw_priv_rx,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
						halError = YXW_spiWriteField(hw_priv_rx,0x83,moni_bw,27,16);
						yxwRetAct = YXW_ApiErrHandler(hw_priv_rx,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
						}else 	printf("---Not Current Channel!!!\n");

					    }
					}
							if(strcmp(vptr, "INFO")==0)
					{   
					      	 vptr = strtok_r(NULL, "=", &vpsave);	
						      info = strcmp(vptr, "0"); 
					    if(info) {
								adrv9009_select = 1 ; 
								 printf("info tx\r\n");
								 yxwAction = YXW_InfoShow_tx(hw_priv_tx);
							} 
					   else{
								adrv9009_select = 0 ; 
								 printf("info rx\r\n");
								 yxwAction = YXW_InfoShow_rx(hw_priv_rx);
					     }
					}

									if(strcmp(vptr, "CAL")==0)
					{   
					      	 vptr = strtok_r(NULL, "=", &vpsave);	
						      cal = strcmp(vptr, "0"); 
					    if(cal) {
								adrv9009_select = 1 ; 
								 printf("cal tx\r\n");
								 yxwAction = YXW_runInitCals(hw_priv_tx, Y9009Init_tx.initcaltype);
			                     yxwAction = YXW_waitInitCalsDone(hw_priv_tx, DELAY_TIME_MS);
							} 
					   else{
								adrv9009_select = 0 ; 
								 printf("cal rx\r\n");
								 YXW_spiWriteMem(hw_priv_rx,0x25006050,0);
								yxwAction = YXW_runInitCals(hw_priv_rx, 0x1000);
								yxwAction = YXW_waitInitCalsDone(hw_priv_rx, DELAY_TIME_MS);
					     }
					}
                    
                     				if(strcmp(vptr, "RXGAINMODE")==0)
					{    
						if (run_rx_flag){
						      vptr = strtok_r(NULL, "=", &vpsave);	
						      rx_gain_mode = strcmp(vptr, "0"); 
							  set_ini_value_double("RX", "gain_control_mode", rx_gain_mode);
							if(rx_gain_mode) 
							YXW_setRxGainControlMode(hw_priv_rx, 0,YXW_APIMODE );
							
							else
							{    K7_spi_write_9009_rx_agc_restart(0x1f); 
							  YXW_setRxManualGain(hw_priv_rx,YXW_RX1,127);
                              	YXW_setRxGainControlMode(hw_priv_rx, 0,YXW_GPIOMODE );
								sleep(1);
								 yxwAction = YXW_setRxGainCtrlPin(hw_priv_rx,0x3,&Y9009Init_rx.rx.rxGainCtrlPinInfo);
							    K7_spi_write_9009_rx_agc_restart(0xff);
	  							K7_spi_write_9009_rx_agc_restart(0x00);
							}
					    }	else 	printf("---Not Current Channel!!!\n");
					}
					pptr = strtok_r(NULL, " ", &ppsave);
				}
                     // send(sock, MESSAGE_9009_set_successfully, strlen(MESSAGE_9009_set_successfully), 0);
			// tcp_client_send_msg("127.0.0.1",12345,MESSAGE_9009_set_successfully); 
			// udp_client_send(MESSAGE_9009_set_successfully);
			// udp_client_send_data(set_9009_success, sizeof(set_9009_success));
			 //  printf("###Message sent: %s\n", MESSAGE_9009_set_successfully); 
				usleep(50000);
			}

		
	}
     udp_client_close();

	while(1)
	{


	}


}

uint32_t  change_9009_rf_freq_rx1(double nfreq)
{

 adrv9009_select = 0 ;
  freq_9009_rx_Hz_new = nfreq * 1000000;
  printf("rx: the old RF frequency is %llu Hz\n\r" , freq_9009_rx_Hz );
	printf("rx: the new RF frequency is %llu Hz\n\r" , freq_9009_rx_Hz_new );
  	if( freq_9009_rx_Hz_new == freq_9009_rx_Hz )
	{
		printf("rx: new frequency is same as the old, do not need to change!\n\r");
		return YXWHAL_OK;
	}
  
  	 _set_gpio_val(0,EMIO_NUM_RX_FDQ_SWITCH);
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT0_RX_GT_REFCLK_EN );
	uint64_t  freq_rx_difference =0ULL;
	printf("------------ enter change_9009_rf_freq_rx() ------------\n\r");
   
	{
       printf("rx: start change adrv9009 RF frequency...\n\r");
	
		yxwAction = YXW_radioOff(hw_priv_rx,YXW_SPI_MODE);
			if (yxwAction != YXWHAL_OK) {
				 
				
			}

		/***********************************************/
  
    yxwAction = YXW_setPllFrequency(hw_priv_rx,YXW_RF_PLL, freq_9009_rx_Hz_new);
    if (yxwAction != YXWHAL_OK)
    {
     
    }	
	//printf("YXW_setPllFrequency\n\r");	
	yxwAction = YXW_waitPllsLockDone(hw_priv_rx,300);
    if (yxwAction != YXWHAL_OK)
    {
        
    }	
	//printf("YXW_waitPllsLockDone\n\r");	

	yxwAction = YXW_setPllFrequency(hw_priv_rx,YXW_AUX_PLL, freq_9009_rx_Hz_new);
    if (yxwAction != YXWHAL_OK)
    {
        
    }	
	//printf("YXW_setPllFrequency\n\r");	
	
	/*Wait RF PLL Frequencies Set Done */
	yxwAction = YXW_waitPllsLockDone(hw_priv_rx,3000);
    if (yxwAction != YXWHAL_OK)
    {
      
    }
	//printf("YXW_waitPllsLockDone\n\r");	

	//最后打开RF开关
		yxwAction = YXW_radioOn(hw_priv_rx,YXW_SPI_MODE);
		if (yxwAction != YXWHAL_OK) {
			 
		}
	}
	//计算新旧两个频率之间的差值
		if( freq_9009_rx_Hz_new > freq_9009_rx_Hz )
		{
			freq_rx_difference = freq_9009_rx_Hz_new - freq_9009_rx_Hz ;
		}
		else
		{
			freq_rx_difference = freq_9009_rx_Hz - freq_9009_rx_Hz_new ;
		}
		if(freq_rx_difference < 100000000ULL)
		{

		}
		else{
			yxwAction = YXW_runInitCals(hw_priv_rx, Y9009Init_rx.initcaltype);
			if (yxwAction != YXWHAL_OK)
			{
				
			}
			//printf("YXW_runInitCals\n\r");	
			yxwAction = YXW_waitInitCalsDone(hw_priv_rx, 2000);
			if (yxwAction != YXWHAL_OK)
			{
				
			}
		}

		freq_9009_rx_Hz = freq_9009_rx_Hz_new ;

		printf("rx: adrv9009 RF frequency changed successful!\n\r");
			
		_set_gpio_val(EMIO_NUM_RX_FDQ_SWITCH,0);
		K7_spi_write_reg( k7_spi_ctrl |= BIT0_RX_GT_REFCLK_EN );	
		return YXWHAL_OK;

}
uint32_t  change_9009_rf_freq_tx1(double nfreq)
{
    adrv9009_select = 1 ;
	    freq_9009_tx_Hz_new = nfreq * 1000000;
	printf("tx: the old RF frequency is %llu Hz\n\r" , freq_9009_tx_Hz );
	printf("tx: the new RF frequency is %llu Hz\n\r" , freq_9009_tx_Hz_new );
	 		if( freq_9009_tx_Hz_new == freq_9009_tx_Hz )
	{
			printf("tx: new frequency is same as the old, do not need to change!\n\r");
			return YXWHAL_OK;
	}
	_set_gpio_val(0,EMIO_NUM_TX_FDQ_SWITCH);
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT2_TX_GT_REFCLK_EN );
	uint64_t  freq_tx_difference =0ULL;
	printf("------------ enter change_9009_rf_freq_tx() ------------\n\r");

           	
		   printf("tx: start change adrv9009 RF frequency...\n\r");
			yxwAction = YXW_radioOff(hw_priv_tx,YXW_SPI_MODE);
			if (yxwAction != YXWHAL_OK) {
				
				
			 }
		yxwAction = YXW_setPllFrequency(hw_priv_tx,YXW_RF_PLL, freq_9009_tx_Hz_new);
		if (yxwAction != YXWHAL_OK)
		{
			
		}	
		yxwAction = YXW_waitPllsLockDone(hw_priv_tx,300);
		if (yxwAction != YXWHAL_OK)
		{
			
		}	

		yxwAction = YXW_setPllFrequency(hw_priv_tx,YXW_AUX_PLL, freq_9009_tx_Hz_new);
		if (yxwAction != YXWHAL_OK)
		{
			
		}	
	
		yxwAction = YXW_waitPllsLockDone(hw_priv_tx,3000);
		if (yxwAction != YXWHAL_OK)
		{
			
		}

		yxwAction = YXW_radioOn(hw_priv_tx,YXW_PIN_MODE);
		if (yxwAction != YXWHAL_OK) {
			
		}
		
    	//计算新旧两个频率之间的差值
		if( freq_9009_tx_Hz_new > freq_9009_tx_Hz )
		{
			freq_tx_difference = freq_9009_tx_Hz_new - freq_9009_tx_Hz ;
		}
		else
		{
			freq_tx_difference = freq_9009_tx_Hz - freq_9009_tx_Hz_new ;
		}
		if(freq_tx_difference < 100000000ULL)
		{

		}
		else{
			
	
		yxwAction = YXW_runInitCals(hw_priv_tx, Y9009Init_tx.initcaltype);
		if (yxwAction != YXWHAL_OK)
		{
			
		}
		//printf("YXW_runInitCals\n\r");	
		yxwAction = YXW_waitInitCalsDone(hw_priv_tx, 2000);
		if (yxwAction != YXWHAL_OK)
		{
			
		}

		}
	 

		freq_9009_tx_Hz = freq_9009_tx_Hz_new ;

		printf("tx: adrv9009 RF frequency changed successful!\n\r");
	
			
        _set_gpio_val(EMIO_NUM_TX_FDQ_SWITCH,0);
	    K7_spi_write_reg( k7_spi_ctrl |= BIT2_TX_GT_REFCLK_EN );
		return YXWHAL_OK;
		
	//}
	

}
uint32_t  set_tx_att(uint8_t tx_att_0d1dB)
{   
	adrv9009_select = 1 ;
	printf("9009 tx att is 0.5db step, value max is 63;\n\r");
	// printf("att value is expressed in 0.1dB\n\r");
	// printf("valid tx att range is 0~41.9dB\n\r");
	
	if( tx_att_0d1dB > 63 )
	{
		printf("out of range!!! limited to 63!!!\n\r");
		tx_att_0d1dB = 63;
	}
	// else
	// {
	// 	tx_att_0d1dB = tx_att_0d1dB * 100 ;
	// }
    printf("tx :new att value is %6u\n\r",tx_att_0d1dB);
	//printf("tx :new att value (mdb) is %8u\n\r",tx_att_0d1dB);
	//yxwAction = YXW_radioOff(hw_priv_tx,YXW_SPI_MODE);
	yxwAction = YXW_setTxManualGain(hw_priv_tx,YXW_TX1,tx_att_0d1dB);
	if (yxwAction != YXWHAL_OK)
	{
		
	}
    //  yxwAction = YXW_radioOn(hw_priv_tx,YXW_SPI_MODE);
	// 	yxwAction = YXW_runInitCals(hw_priv_tx, 0x00005000);
	// 	if (yxwAction != YXWHAL_OK)
	// 	{
		
	// 	}
	// 	printf("YXW_runInitCals\n\r");	
	// 	yxwAction = YXW_waitInitCalsDone(hw_priv_tx, 100);
	// 	if (yxwAction != YXWHAL_OK)
	// 	{
			
	// 	}
	// 		yxwAction = YXW_lp(hw_priv_rx, &Y9009Init_tx);
	// 	if (yxwAction != YXWHAL_OK)
	// 	{
	// 		printf("error: YXW_lp() failed\n\r");
	// 		return  -1; 
	// 	}

		
	// 	//yxwAction = YXW_InfoShow_tx(hw_priv_tx);
    //     yxwAction = YXW_setRadioCtrlPinMode(hw_priv_tx, YXW_RXOFF,YXW_ORXOFF,YXW_TX1);
	
   
	printf("tx: adrv9009 set_tx_att changed successful!\n\r");
	//K7_spi_write_reg_sks_agc( tx_att_0d1dB);
    return YXWHAL_OK;

}
uint32_t  set_rx_att(uint8_t rx_att_0d1dB)
{   
	uint32_t ff = 0;
	adrv9009_select = 0 ;
	printf("9009 rx att is 0.25db step, value max is 127;\n\r");
	YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
	// printf("att value is expressed in 0.1dB\n\r");
	// printf("valid rx att range is 0~41.9dB\n\r");
	printf("rx :new att value is %6u\n\r",rx_att_0d1dB);
	// if( rx_att_0d1dB > 419 )
	// {
	// 	printf("out of range!!! limited to 41.9dB!!!\n\r");
	// 	rx_att_0d1dB = 41900;
	// }
	// else
	// {
	// 	rx_att_0d1dB = rx_att_0d1dB * 100 ;
	// }

	//printf("tx :new att value (mdb) is %8u\n\r",rx_att_0d1dB);
	YXW_setRxManualGain(hw_priv_rx,YXW_RX1,rx_att_0d1dB);
	printf("rx: adrv9009 set_rx_att changed successful!\n\r");
	return YXWHAL_OK;

}
uint32_t set_9009_tx_outside_att(uint8_t dsa_att_value)
{
	printf("9009 tx out att is 0.25db step, value max is 127;\n\r");
	if(dsa_att_value > 127)
	{
		dsa_att_value = 127 ;
	}
//	K7_spi_write_tx_dsa_att( dsa_att_value );
	K7_spi_write_reg_sks_agc( dsa_att_value);
	printf("set_9009_tx_outside_att successful;\n\r");
	return YXWHAL_OK;
}
void  set_tx_att1(uint8_t tx_att_0d1dB)
{   
	adrv9009_select = 1 ;
	uint8_t att_tx_difference = 0;
	if(tx_att_0d1dB > 125)
	{
		tx_att_0d1dB = 125;
	}
	att_9009_tx_db_new = tx_att_0d1dB;
	YXW_setTxManualGain(hw_priv_tx,YXW_TX1,tx_att_0d1dB);
	printf("tx :old att value is %6u\n\r",att_9009_tx_db);
	printf("tx :new att value is %6u\n\r",att_9009_tx_db_new);
	
	if(att_9009_tx_db > att_9009_tx_db_new) {
		att_tx_difference = att_9009_tx_db - att_9009_tx_db_new;
	} 
	else{
       att_tx_difference = att_9009_tx_db_new - att_9009_tx_db;

	}
	printf("tx: att_difference is %6u db\n\r",att_tx_difference );

	if(att_tx_difference < 12){
	//printf("tx: att_difference is %6u db within range\n\r",att_tx_difference );
	}
	else{
      YXW_spiWriteField(hw_priv_tx,0xa9,1,13,13);
		yxwAction = YXW_runInitCals(hw_priv_tx, Y9009Init_tx.initcaltype);
		if (yxwAction != YXWHAL_OK)
		{
			/*** <what to do based on Y9009 recovery action returned > ***/
		}
		yxwAction = YXW_waitInitCalsDone(hw_priv_tx, 1000);
		if (yxwAction != YXWHAL_OK)
		{
			/*** <what to do based on Y9009 recovery action returned > ***/
		}
	}
    att_9009_tx_db = att_9009_tx_db_new ;
	printf("tx: adrv9009 set_tx_att changed successful!\n\r");
}
void  set_rx_att1(uint8_t rx_att_0d1dB)
{   

	adrv9009_select = 0 ;
	uint8_t att_rx_difference = 0;
	if(rx_att_0d1dB > 125)
	{
		rx_att_0d1dB = 125;
	}
	YXW_setRxManualGain(hw_priv_rx,YXW_RX1,rx_att_0d1dB);   
	att_9009_rx_db_new = rx_att_0d1dB;
	printf("rx :old att value is %6u\n\r",att_9009_rx_db);
	printf("rx :new att value is %6u\n\r",att_9009_rx_db_new);
	
	if(att_9009_rx_db > att_9009_rx_db_new) {
		att_rx_difference = att_9009_rx_db - att_9009_rx_db_new;
	} 
	else{
       att_rx_difference = att_9009_rx_db_new - att_9009_rx_db;

	}
	printf("rx: att_difference is %6u db\n\r",att_rx_difference );

	if(att_rx_difference < 12){
	//printf("rx: att_difference is %6u db within range\n\r",att_rx_difference );
	}
	else{
      YXW_spiWriteField(hw_priv_rx,0xa9,1,13,13);
		yxwAction = YXW_runInitCals(hw_priv_rx, Y9009Init_rx.initcaltype);
		if (yxwAction != YXWHAL_OK)
		{
			/*** <what to do based on Y9009 recovery action returned > ***/
		}
		yxwAction = YXW_waitInitCalsDone(hw_priv_rx, 1000);
		if (yxwAction != YXWHAL_OK)
		{
			/*** <what to do based on Y9009 recovery action returned > ***/
		}
	}
    att_9009_rx_db = att_9009_rx_db_new ;
	printf("rx: adrv9009 set_rx_att changed successful!\n\r");

}
uint32_t  change_9009_rf_freq_rx(double nfreq)
{   
 
    adrv9009_select = 0 ;
	yxwAction=YXW_spiWriteMem(hw_priv_rx,0x25006050,0);
		uint32_t regValue;
    freq_9009_rx_Hz_new = nfreq * 1000000;
    printf("rx: the old RF frequency is %llu Hz\n\r" , freq_9009_rx_Hz );
	printf("rx: the new RF frequency is %llu Hz\n\r" , freq_9009_rx_Hz_new );
  	if( freq_9009_rx_Hz_new == freq_9009_rx_Hz )
	{
		printf("rx: new frequency is same as the old, do not need to change!\n\r");
		return YXWHAL_OK;
	}
	  K7_spi_write_9009_rx_agc_restart(0x1f);
    //  yxw_rx_bak();
    //  return 0;
  	 _set_gpio_val(0,EMIO_NUM_RX_FDQ_SWITCH);
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT0_RX_GT_REFCLK_EN );
	uint64_t  freq_rx_difference =0ULL;
	printf("------------ enter change_9009_rf_freq_rx() ------------\n\r");
    //freq_9009_rx_Hz_new = nfreq * 1000000;
	// printf("rx: the old RF frequency is %llu Hz\n\r" , freq_9009_rx_Hz );
	// printf("rx: the new RF frequency is %llu Hz\n\r" , freq_9009_rx_Hz_new );
	// if( freq_9009_rx_Hz_new == freq_9009_rx_Hz )
	// {
	// 	printf("rx: new frequency is same as the old, do not need to change!\n\r");
	// 	_set_gpio_val(EMIO_NUM_RX_FDQ_SWITCH,0);
	// 	K7_spi_write_reg( k7_spi_ctrl |= BIT0_RX_GT_REFCLK_EN );
	// 	return YXWHAL_OK;
	// }
    // else
	{
       printf("rx: start change adrv9009 RF frequency...\n\r");
		// Function to turn radio on, Disables transmitters and receivers
		yxwAction = YXW_radioOff(hw_priv_rx,YXW_SPI_MODE);
			if (yxwAction != YXWHAL_OK) {
				 
				
			}
		// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL2, 0xc0352640);
		// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL3, 0x4c1022b0);
		// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL7, 0x41af);
		// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL_LO, 0x20821);
	//     YXW_spiReadMem(hw_priv_rx, 0x20000000, &regValue);
	
		/***********************************************/
    /**3.Set RF AND AUX PLL Frequencies, Default 5.5GHz ****/
    /***********************************************/
	//yxwAction = YXW_setPllFrequency(hw_priv_rx,YXW_RF_PLL, YxwY9009Init_rx.clocks.rfpll_frequency_Hz);
    yxwAction = YXW_setPllFrequency(hw_priv_rx,YXW_RF_PLL, freq_9009_rx_Hz_new);
    if (yxwAction != YXWHAL_OK)
    {
      
    }	
	
	// //printf("YXW_set;llFrequency\n\r");	
	// /*Wait RF PLL Frequencies Set Done */
	yxwAction = YXW_waitPllsLockDone(hw_priv_rx,100);
    if (yxwAction != YXWHAL_OK)
    {
        
    }

	//printf("YXW_waitPllsLockDone\n\r");	

    //yxwAction = YXW_setPllFrequency(hw_priv_rx,YXW_AUX_PLL, YxwY9009Init_rx.clocks.auxpll_frequency_Hz);
	// yxwAction = YXW_setPllFrequency(hw_priv_rx,YXW_AUX_PLL, freq_9009_rx_Hz_new);
    // if (yxwAction != YXWHAL_OK)
    // {
       
    // }	
	//printf("YXW_setPllFrequency\n\r");	
	
	/*Wait RF PLL Frequencies Set Done */
	// yxwAction = YXW_waitPllsLockDone(hw_priv_rx,100);
    // if (yxwAction != YXWHAL_OK)
    // {
      
    // }
	// //printf("YXW_waitPllsLockDone\n\r");
	// YXW_setPllFrequency(hw_priv_rx,YXW_RF_PLL, freq_9009_rx_Hz_new);	

    	yxwAction = YXW_radioOn(hw_priv_rx,YXW_SPI_MODE);
		if (yxwAction != YXWHAL_OK) {
			
		}
	}
	//计算新旧两个频率之间的差值
		// if( freq_9009_rx_Hz_new > freq_9009_rx_Hz )
		// {
		// 	freq_rx_difference = freq_9009_rx_Hz_new - freq_9009_rx_Hz ;
		// }
		// else
		// {
		// 	freq_rx_difference = freq_9009_rx_Hz - freq_9009_rx_Hz_new ;
		// }
		// if(freq_rx_difference < 100000000ULL)
		// {

		// }
		// else
		{
			// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL2, 0xc0352640);
			// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL3, 0x4c1022b0);
			// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL7, 0x41af);
			// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL_LO, 0x20821);
		// 	yxwAction = YXW_radioOn(hw_priv_rx,YXW_SPI_MODE);
		// if (yxwAction != YXWHAL_OK) {
			
		// }

			yxwAction = YXW_runInitCals(hw_priv_rx, Y9009Init_rx.initcaltype);
			if (yxwAction != YXWHAL_OK)
			{
			
			}

			printf("YXW_runInitCals\n\r");	
			yxwAction = YXW_waitInitCalsDone(hw_priv_rx, DELAY_TIME_MS);
			if (yxwAction != YXWHAL_OK)
			{
				
			}
			 yxwAction = YXW_lp(hw_priv_rx, &Y9009Init_rx);
			if (yxwAction != YXWHAL_OK)
			{
				printf("error: YXW_lp() failed\n\r");
				return  -1; 
			}
			//	yxwAction = YXW_pllmode(hw_priv_rx, &Y9009Init_rx);
			//yxwAction=YXW_rx0A_orx0D(hw_priv_rx);
		// 	if (yxwAction != YXWHAL_OK)
		// {
		// 	printf("error: YXW_rx0A_orx0D() failed\n\r");
		// 	return  -1; 
		// }
		  YXW_spiWriteField(hw_priv_rx, 0x43, 0x10882bf, 31, 0);
		  YXW_spiWriteField(hw_priv_rx, 0x44, 0x10082bf, 31, 0);
		}

		freq_9009_rx_Hz = freq_9009_rx_Hz_new ;
	//最后打开RF开关
	
		printf("rx: adrv9009 RF frequency changed successful!\n\r");
		K7_spi_write_9009_rx_agc_restart(0xff);
	   K7_spi_write_9009_rx_agc_restart(0x00);	
		_set_gpio_val(EMIO_NUM_RX_FDQ_SWITCH,0);
		K7_spi_write_reg( k7_spi_ctrl |= BIT0_RX_GT_REFCLK_EN );	
		return YXWHAL_OK;

}
uint32_t  change_9009_rf_freq_tx(double nfreq)
{
    
     adrv9009_select = 1 ;
	// _set_gpio_val(0,EMIO_NUM_ZAIBO_SWITCH);
	    freq_9009_tx_Hz_new = nfreq * 1000000;
	printf("tx: the old RF frequency is %llu Hz\n\r" , freq_9009_tx_Hz );
	printf("tx: the new RF frequency is %llu Hz\n\r" , freq_9009_tx_Hz_new );
	 		if( freq_9009_tx_Hz_new == freq_9009_tx_Hz )
	{
			printf("tx: new frequency is same as the old, do not need to change!\n\r");
			return YXWHAL_OK;
	}
	_set_gpio_val(0,EMIO_NUM_TX_FDQ_SWITCH);
	K7_spi_write_reg( k7_spi_ctrl &= ~BIT2_TX_GT_REFCLK_EN );
	uint64_t  freq_tx_difference =0ULL;
	printf("------------ enter change_9009_rf_freq_tx() ------------\n\r");
 
	// 	if( freq_9009_tx_Hz_new == freq_9009_tx_Hz )
	// {
	// 		printf("tx: new frequency is same as the old, do not need to change!\n\r");
	// 		//return YXWHAL_OK;
	// }

	//else if( freq_9009_tx_Hz_new != freq_9009_tx_Hz ){  

         
           	
		   printf("tx: start change adrv9009 RF frequency...\n\r");
			yxwAction = YXW_radioOff(hw_priv_tx,YXW_SPI_MODE);
			if (yxwAction != YXWHAL_OK) {
				 IF_ERR_RETURN(0x1);
				
			 }
		// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL2, 0xc0352640);
		// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL3, 0x4c1022b0);
		// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL7, 0x41af);
		// YXW_spiWriteReg(hw_priv_rx, REG_AUXPLL_LO, 0x20821);
		yxwAction = YXW_setPllFrequency(hw_priv_tx,YXW_RF_PLL, freq_9009_tx_Hz_new);
		if (yxwAction != YXWHAL_OK)
		{
			
		}	
		yxwAction = YXW_waitPllsLockDone(hw_priv_tx,100);
		if (yxwAction != YXWHAL_OK)
		{
			
		}
		//关闭业务信号路径	
		YXW_spiWriteField(hw_priv_tx, 0xb9, 0x1, 31, 0);
		YXW_spiWriteField(hw_priv_tx, 0xbd, 0x10008ff, 31, 0);
		YXW_spiWriteField(hw_priv_tx, 0xa2, 0xfff0, 31, 0);
		//printf("YXW_waitPllsLockDone\n\r");	

		//yxwAction = YXW_setPllFrequency(hw_priv_tx,YXW_AUX_PLL, YxwY9009Init_tx.clocks.auxpll_frequency_Hz);
		// yxwAction = YXW_setPllFrequency(hw_priv_tx,YXW_AUX_PLL, freq_9009_tx_Hz_new);
		// if (yxwAction != YXWHAL_OK)
		// {
		
		// }	
		// //printf("YXW_setPllFrequency\n\r");	
		
		// /*Wait RF PLL Frequencies Set Done */
		// yxwAction = YXW_waitPllsLockDone(hw_priv_tx,100);
		// if (yxwAction != YXWHAL_OK)
		// {
			
		// }
	
		//printf("YXW_waitPllsLockDone\n\r");	
		//最后打开RF开关
		//yxwAction = YXW_radioOn(hw_priv_tx,YXW_PIN_MODE);
		yxwAction = YXW_radioOn(hw_priv_tx,YXW_SPI_MODE);
		if (yxwAction != YXWHAL_OK) {
			
		}
		//YXW_spiWriteField(hw_priv_tx, 0xb9, 0x801, 31, 0);
		//YXW_spiWriteField(hw_priv_tx, 0xa2, 0xfff1, 31, 0);
    	//计算新旧两个频率之间的差值
		// if( freq_9009_tx_Hz_new > freq_9009_tx_Hz )
		// {
		// 	freq_tx_difference = freq_9009_tx_Hz_new - freq_9009_tx_Hz ;
		// }
		// else
		// {
		// 	freq_tx_difference = freq_9009_tx_Hz - freq_9009_tx_Hz_new ;
		// }
		// if(freq_tx_difference < 100000000ULL)
		// {

		// }
		// else
		{
			
	
		yxwAction = YXW_runInitCals(hw_priv_tx, 0x00001000);
		if (yxwAction != YXWHAL_OK)
		{
		
		}
		printf("YXW_runInitCals\n\r");	
		yxwAction = YXW_waitInitCalsDone(hw_priv_tx, 100);
		if (yxwAction != YXWHAL_OK)
		{
			
		}
		//YXW_spiWriteField(hw_priv_tx, REG_CALI_DG_0, 1, 1, 0);
		yxwAction = YXW_lp(hw_priv_tx, &Y9009Init_tx);
		if (yxwAction != YXWHAL_OK)
		{
			printf("error: YXW_lp() failed\n\r");
			return  -1; 
		}
		
		}
		//打开业务路径
		YXW_spiWriteField(hw_priv_tx, 0xb9, 0x0, 31, 0);
		if(freq_9009_tx_Hz_new == 3000000000ULL)
		YXW_spiWriteField(hw_priv_tx, 0x1a,0x1102, 31, 0);
		if(freq_9009_tx_Hz_new == 3200000000ULL)
		YXW_spiWriteField(hw_priv_tx, 0x1a,0x1102, 31, 0);
		if(freq_9009_tx_Hz_new == 3600000000ULL)
		YXW_spiWriteField(hw_priv_tx, 0x1a,0x1102, 31, 0);
		//yxwAction = YXW_InfoShow_tx(hw_priv_tx);
        yxwAction = YXW_setRadioCtrlPinMode(hw_priv_tx, YXW_RXOFF,YXW_ORXOFF,YXW_TX1);
	  if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_setRadioCtrlPinMode() failed\n\r");
		return  -1; 
    }
		freq_9009_tx_Hz = freq_9009_tx_Hz_new ;

		printf("tx: adrv9009 RF frequency changed successful!\n\r");
	
		
        _set_gpio_val(EMIO_NUM_TX_FDQ_SWITCH,0);
	    K7_spi_write_reg( k7_spi_ctrl |= BIT2_TX_GT_REFCLK_EN );
		//_set_gpio_val(EMIO_NUM_ZAIBO_SWITCH,0);	
		return YXWHAL_OK;
		
	//}
	

}
void get_rx_gain_and_mgc()
{
	//    sleep(1);
	// read_curr_rx_gain_index= K7_spi_read_reg_sks_agc_rx();	 
	//printf("rx gain: read current table index is %d\n\r" , read_curr_rx_gain_index );	
	//read_curr_tx_gain_index= K7_spi_read_reg_sks_agc_tx();
	//printf("tx gain: read current table index is %d\n\r" , read_curr_tx_gain_index );	

	adrv9009_select = 0 ;
	yxwAction = YXW_getRxGain(hw_priv_rx,YXW_RX1, &rxGainIndex);
	set_ini_value_double("RX", "att_in", rxGainIndex);	
	if (yxwAction != YXWHAL_OK)
    {
       printf("error: YXW_getRxGain() failed\n\r");
		//return  -1; 
    }
    printf("rx mgc: %d\n\r", rxGainIndex);
	

}
uint32_t set_9009_tx_attattout(double tx_att)
{  
    double iatt = (TX_MAX_GAIN - tx_att);

  if((iatt  <= 31.75) && (iatt >= 0))

  {  
	int att = (iatt *4) ;
	printf("att:%d\r\n",att);
    set_9009_tx_outside_att(att);
	 set_tx_att(0);
  }
  if (iatt > 31.75 && iatt <= 60)
  { 
	int att1 = (iatt - 31.75) * 2 ;
	printf("att1:%d\r\n",att1);
    set_9009_tx_outside_att(127);
    set_tx_att(att1);
   return YXWHAL_OK;

  }

}
uint32_t set_1orx_power_low(YxwDevice_t *hw_priv)
{   
	yxwHalErr_t halError = YXWHAL_OK;
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;

	halError=YXW_spiWriteField(hw_priv,0x42,1,2,1);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4a,1,2,1);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x50,0xF3F0F,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x3E,0x80732000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x44,0x3882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x41,0xc0123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x45,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x46,0x80732000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x43,0x10082bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4C,0x3882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4B,0x10882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x49,0xc0123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x82,0x40d7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4A,0xC00C,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4D,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x85,0xC0d7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x1C6,0x10000000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x18,0xC0752650,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x19,0x44702630,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x38,0x27FFFF,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	return (uint32_t)yxwRetAct;

}
uint32_t set_1rx_moni_power_low(YxwDevice_t *hw_priv)
{   
	yxwHalErr_t halError = YXWHAL_OK;
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;

	halError=YXW_spiWriteField(hw_priv,0x46,0x80732000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x43,0x10082bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4c,0x3882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x49,0xc0123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x50,0xc000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x43,0x10082bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x45,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x82,0xC2e7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x83,0xC2e70000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4b,0x10882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x49,0xC0123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4A,0xC00C,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4D,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x85,0xC2e7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x86,0xC2e70000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x50,0xf7f0f,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x51,0xffbf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x1C6,0x10000000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);


	halError=YXW_spiWriteField(hw_priv,0x24,0xc0752650,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	halError=YXW_spiWriteField(hw_priv,0x25,0xc27026b0,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	halError=YXW_spiWriteField(hw_priv,0x29,0x41bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	halError=YXW_spiWriteField(hw_priv,0xe,0x2e821,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	halError=YXW_spiWriteField(hw_priv,0x41,0x40123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	halError=YXW_spiWriteField(hw_priv,0x42,0x4000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	halError=YXW_spiWriteField(hw_priv,0xa9,0x3100801,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	return (uint32_t)yxwRetAct;

}
uint32_t set_1t_power_low(YxwDevice_t *hw_priv)

{
  yxwHalErr_t halError = YXWHAL_OK;
	YxwRecoveryActions_t yxwRetAct = YXW_NO_ACTION;

	halError=YXW_spiWriteField(hw_priv,0x42,1,2,1);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4A,1,2,1);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x50,0x3D505,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x3E,0x80732000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x44,0x3882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x41,0xc0123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x45,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x46,0x80732000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x43,0x10882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4C,0x3882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4B,0x10882bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x49,0xc0123ffe,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x82,0xC0d7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4A,0xC00C,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x4D,0xf3d08e00,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x85,0xC0d7,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x24,0xC0752650,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x25,0xC27026B0,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x29,0x41bf,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0xE,0x2E821,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);

	halError=YXW_spiWriteField(hw_priv,0x1C6,0x1000000,31,0);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	return (uint32_t)yxwRetAct;

}
uint32_t set_orx_firBW(YxwDevice_t *hw_priv,double firBW)
{  
	YxwFir_t my_filter;
    my_filter.gain_dB = -12;  // 假设为 0 dB 增益
    my_filter.numFirCoefs = 72;

	adrv9009_select = 0 ; 
  YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
  yxwHalErr_t halError = YXWHAL_OK;
  if(firBW == 40)
  {
     for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_40[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
  else if(firBW == 60)
  {
     for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_60[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
   else if(firBW == 48)
  {   
    for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_48[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
   else if(firBW == 24)
  {
      for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_24[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
   else if(firBW == 12)
  {
     for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_12[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
   else if(firBW == 6)
  {
      for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_6[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
   else if(firBW == 3)
  {
      for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_3[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
   else if(double_equals(firBW, 1.5, 1e-6))
  {
      for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_1d5[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
   else if(double_equals(firBW, 0.75, 1e-6))
  {
      for (int i = 0; i < 72; i++) {
        my_filter.coefs[i] = orxfir_d750[i]; 
    }
    yxwRetAct = YXW_programFir(hw_priv, &my_filter, YXW_ORX_FILTER);
     IF_ERR_RETURN(yxwRetAct);
  }
  else
  {
     printf("invalid BW\n");
	 IF_ERR_RETURN(0x1);
  }
   
    //printf("debug1\n");
	halError=YXW_spiWriteField(hw_priv,0xB0,1,8,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
       	uint32_t readdata = 0,readdata3 = 0;
	//  printf("yxw_rx1 0x%X\n\r",readdata);
	halError=YXW_spiWriteField(hw_priv,0xB0,0,8,8);
	yxwRetAct=YXW_ApiErrHandler(hw_priv,YXW_ERRHDL_HAL_RADIO,halError,yxwRetAct,YXW_ERR_RESET_SPI);
	//  printf("yxw_rx2 0x%X\n\r",readdata3);
	return (uint32_t)yxwRetAct;

}

uint32_t set_orx_firBW_reg(YxwDevice_t *hw_priv,double firBW)
{  
	

	adrv9009_select = 0 ; 
  YxwRecoveryActions_t yxwRetAct=YXW_NO_ACTION;
  yxwHalErr_t halError = YXWHAL_OK;
  if(firBW == 40)
  {
     
  }
  else if(firBW == 60)
  {
   
  }
   else if(firBW == 48)
  {   
YXW_spiWriteField(hw_priv, 0x1F6, 0x28000F, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F7, 0x3B003B, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F8, 0xFFE2001D, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F9, 0xFF62FF99, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FA, 0xFFA1FF5D, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FB, 0xCB0028, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FC, 0x161014A, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FD, 0xFFE400E6, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FE, 0xFDA0FEA4, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FF, 0xFE1AFD57, 31, 0);
YXW_spiWriteField(hw_priv, 0x200, 0x229FFDE, 31, 0);
YXW_spiWriteField(hw_priv, 0x201, 0x4F00427, 31, 0);
YXW_spiWriteField(hw_priv, 0x202, 0xD503DB, 31, 0);
YXW_spiWriteField(hw_priv, 0x203, 0xF86AFC8F, 31, 0);
YXW_spiWriteField(hw_priv, 0x204, 0xF75EF627, 31, 0);
YXW_spiWriteField(hw_priv, 0x205, 0x6B9FCF5, 31, 0);
YXW_spiWriteField(hw_priv, 0x206, 0x203E1344, 31, 0);
YXW_spiWriteField(hw_priv, 0x207, 0x30FA2AF1, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5152, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5052, 31, 0);
  }
   else if(firBW == 24)
  {
YXW_spiWriteField(hw_priv, 0x1F6, 0xFFC5FFE2, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F7, 0xFF89FFA5, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F8, 0xFF72FF75, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F9, 0xFFB3FF85, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FA, 0x5AFFFB, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FB, 0x13400C7, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FC, 0x1CA0191, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FD, 0x19101CF, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FE, 0x3C010A, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FF, 0xFE0CFF35, 31, 0);
YXW_spiWriteField(hw_priv, 0x200, 0xFBE6FCE4, 31, 0);
YXW_spiWriteField(hw_priv, 0x201, 0xFB1CFB3F, 31, 0);
YXW_spiWriteField(hw_priv, 0x202, 0xFCEAFBA2, 31, 0);
YXW_spiWriteField(hw_priv, 0x203, 0x1D9FF00, 31, 0);
YXW_spiWriteField(hw_priv, 0x204, 0x952055A, 31, 0);
YXW_spiWriteField(hw_priv, 0x205, 0x11A50D84, 31, 0);
YXW_spiWriteField(hw_priv, 0x206, 0x188A156B, 31, 0);
YXW_spiWriteField(hw_priv, 0x207, 0x1BEF1AC5, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5152, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5052, 31, 0);
     
  }
   else if(firBW == 12)
  {
YXW_spiWriteField(hw_priv, 0x1F6, 0x2E001B, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F7, 0x5A0043, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F8, 0x800070, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F9, 0x850089, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FA, 0x4C0072, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FB, 0xFFC70013, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FC, 0xFEFDFF68, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FD, 0xFE1CFE8B, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FE, 0xFD6EFDB8, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FF, 0xFD54FD48, 31, 0);
YXW_spiWriteField(hw_priv, 0x200, 0xFE29FD9C, 31, 0);
YXW_spiWriteField(hw_priv, 0x201, 0x28FF02, 31, 0);
YXW_spiWriteField(hw_priv, 0x202, 0x351019A, 31, 0);
YXW_spiWriteField(hw_priv, 0x203, 0x75D0542, 31, 0);
YXW_spiWriteField(hw_priv, 0x204, 0xBC30990, 31, 0);
YXW_spiWriteField(hw_priv, 0x205, 0xFD10DE1, 31, 0);
YXW_spiWriteField(hw_priv, 0x206, 0x12D2117D, 31, 0);
YXW_spiWriteField(hw_priv, 0x207, 0x143713BE, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5152, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5052, 31, 0);
  } 
   else if(firBW == 6)
  {
YXW_spiWriteField(hw_priv, 0x1F6, 0xFFD8FFE8, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F7, 0xFFA9FFC2, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F8, 0xFF6AFF8B, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F9, 0xFF25FF48, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FA, 0xFEE7FF04, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FB, 0xFEC4FED1, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FC, 0xFED5FEC5, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FD, 0xFF30FEF8, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FE, 0xFFE9FF80, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FF, 0x10B006D, 31, 0);
YXW_spiWriteField(hw_priv, 0x200, 0x29301C3, 31, 0);
YXW_spiWriteField(hw_priv, 0x201, 0x4720379, 31, 0);
YXW_spiWriteField(hw_priv, 0x202, 0x6890579, 31, 0);
YXW_spiWriteField(hw_priv, 0x203, 0x8AD079C, 31, 0);
YXW_spiWriteField(hw_priv, 0x204, 0xAAD09B4, 31, 0);
YXW_spiWriteField(hw_priv, 0x205, 0xC560B8F, 31, 0);
YXW_spiWriteField(hw_priv, 0x206, 0xD7C0CFC, 31, 0);
YXW_spiWriteField(hw_priv, 0x207, 0xE010DD4, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5152, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5052, 31, 0);
  }
   else if(firBW == 3)
  {
YXW_spiWriteField(hw_priv, 0x1F6, 0xFFFCFFF7, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F7, 0xE0003, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F8, 0x2F001C, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F9, 0x640047, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FA, 0xB00087, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FB, 0x11600E0, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FC, 0x1970153, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FD, 0x23201E1, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FE, 0x2E40288, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FF, 0x3A80344, 31, 0);
YXW_spiWriteField(hw_priv, 0x200, 0x478040F, 31, 0);
YXW_spiWriteField(hw_priv, 0x201, 0x54B04E1, 31, 0);
YXW_spiWriteField(hw_priv, 0x202, 0x61705B2, 31, 0);
YXW_spiWriteField(hw_priv, 0x203, 0x6D20677, 31, 0);
YXW_spiWriteField(hw_priv, 0x204, 0x7730726, 31, 0);
YXW_spiWriteField(hw_priv, 0x205, 0x7F107B7, 31, 0);
YXW_spiWriteField(hw_priv, 0x206, 0x8440820, 31, 0);
YXW_spiWriteField(hw_priv, 0x207, 0x869085D, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5152, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5052, 31, 0);
  }
   else if(double_equals(firBW, 1.5, 1e-6))
  {
YXW_spiWriteField(hw_priv, 0x1F6, 0x2B001C, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F7, 0x52003C, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F8, 0x89006B, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F9, 0xD100AB, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FA, 0x12B00FC, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FB, 0x197015F, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FC, 0x21501D4, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FD, 0x2A2025A, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FE, 0x33B02ED, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FF, 0x3DC038B, 31, 0);
YXW_spiWriteField(hw_priv, 0x200, 0x47F042D, 31, 0);
YXW_spiWriteField(hw_priv, 0x201, 0x51E04CF, 31, 0);
YXW_spiWriteField(hw_priv, 0x202, 0x5B5056B, 31, 0);
YXW_spiWriteField(hw_priv, 0x203, 0x63C05FA, 31, 0);
YXW_spiWriteField(hw_priv, 0x204, 0x6AD0678, 31, 0);
YXW_spiWriteField(hw_priv, 0x205, 0x70506DD, 31, 0);
YXW_spiWriteField(hw_priv, 0x206, 0x73F0726, 31, 0);
YXW_spiWriteField(hw_priv, 0x207, 0x7580750, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5152, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5052, 31, 0);
     
  }
   else if(double_equals(firBW, 0.75, 1e-6))
  {
YXW_spiWriteField(hw_priv, 0x1F6, 0x2B001C, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F7, 0x52003D, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F8, 0x89006C, 31, 0);
YXW_spiWriteField(hw_priv, 0x1F9, 0xD100AB, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FA, 0x12B00FB, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FB, 0x196015E, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FC, 0x21401D3, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FD, 0x2A00258, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FE, 0x33702EA, 31, 0);
YXW_spiWriteField(hw_priv, 0x1FF, 0x3D70386, 31, 0);
YXW_spiWriteField(hw_priv, 0x200, 0x4790428, 31, 0);
YXW_spiWriteField(hw_priv, 0x201, 0x51704C9, 31, 0);
YXW_spiWriteField(hw_priv, 0x202, 0x5AC0563, 31, 0);
YXW_spiWriteField(hw_priv, 0x203, 0x63205F2, 31, 0);
YXW_spiWriteField(hw_priv, 0x204, 0x6A3066E, 31, 0);
YXW_spiWriteField(hw_priv, 0x205, 0x6FA06D2, 31, 0);
YXW_spiWriteField(hw_priv, 0x206, 0x734071B, 31, 0);
YXW_spiWriteField(hw_priv, 0x207, 0x74D0744, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5152, 31, 0);
YXW_spiWriteField(hw_priv, 0xB0, 0x5052, 31, 0);

  }
  else
  {
     printf("invalid BW\n");
	// IF_ERR_RETURN(0x1);
	return -1;
  }
   

	return (uint32_t)yxwRetAct;

}
uint32_t set_zaibo_switch(YxwDevice_t *hw_priv,uint16_t zaiboswitch)
{   


    adrv9009_select = 1 ; 

	// yxwAction = YXW_setRadioCtrlPinMode(hw_priv, YXW_RXOFF,YXW_ORXOFF,YXW_TX1);
	//   if (yxwAction != YXWHAL_OK)
    // {
    //    printf("error: YXW_setRadioCtrlPinMode() failed\n\r");
	// 	return  -1; 
    // }
   if(zaiboswitch)
	{  
						
		_set_gpio_val(EMIO_NUM_ZAIBO_SWITCH,0);	
		printf("set tx zaibo ON\n");      }					
	else {   
		_set_gpio_val(0,EMIO_NUM_ZAIBO_SWITCH);	
        printf("set tx zaibo OFF\n");
	}

	// 	yxwAction = YXW_setRadioCtrlSpiMode(hw_priv, YXW_RXOFF,YXW_ORXOFF,YXW_TX1);
	//   if (yxwAction != YXWHAL_OK)
    // {
    //    printf("error: YXW_setRadioCtrlSpiMode() failed\n\r");
	// 	return  -1; 
    // }
  return 0;

}

int tcp_client_send_msg(const char *ip,int port,const char *message) {
    
  char buffer[1024] = {0};
   int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("client socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return -1;
    }
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("send");
        return -1;
    }

    int len = read(sock, buffer, sizeof(buffer));
    if (len > 0) {
        buffer[len] = '\0';
        printf("Server reply: %s\n", buffer);
    } else {
        perror("read");
        return -1;
    }
    close(sock);
    return 0;
}



// 初始化函数：创建 socket 并设置服务端地址
int udp_client_init(const char *ip, int port) {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);

    return 0;
}
int udp_client_init_v2() {

     char cp_addr[64], ap_addr[64],port_addr[64];
     int cp_val,ap_val,ap_port;
    bool is_int;
	get_ini_value("CP", "ip_addr", cp_addr, sizeof(cp_addr), &cp_val, &is_int);
	get_ini_value("AP", "ip_addr", ap_addr, sizeof(ap_addr), &ap_val, &is_int);
	get_ini_value("AP", "ip_port", port_addr, sizeof(port_addr), &ap_port, &is_int);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }

        // 明确绑定本地网卡 IP（eth0）
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = 0;  // 自动分配端口
   // inet_pton(AF_INET, LOCAL_IP, &local_addr.sin_addr);
	 inet_pton(AF_INET, cp_addr, &local_addr.sin_addr);

    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }

    // 设置目标地址
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
   // servaddr.sin_port = htons(DEST_PORT);//
	  servaddr.sin_port = htons(ap_port);
   // inet_pton(AF_INET, DEST_IP, &servaddr.sin_addr);
	inet_pton(AF_INET, ap_addr, &servaddr.sin_addr);

    return 0;
}

int udp_client_init_v2_1() {

	     char cp_addr1[64], ip_addr[64],port_addr1[64];
     int cp_val1,ip_val1,ap_port1;
    bool is_int1;
	get_ini_value("CP", "ip_addr", cp_addr1, sizeof(cp_addr1), &cp_val1, &is_int1);
	get_ini_value("IP", "ip_addr", ip_addr, sizeof(ip_addr), &ip_val1, &is_int1);
	get_ini_value("IP", "ip_port", port_addr1, sizeof(port_addr1), &ap_port1, &is_int1);

    sockfd1 = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd1 < 0) {
        perror("socket creation failed1");
        return -1;
    }

        // 明确绑定本地网卡 IP（eth0）
    memset(&local_addr1, 0, sizeof(local_addr1));
    local_addr1.sin_family = AF_INET;
    local_addr1.sin_port = 0;  // 自动分配端口
   // inet_pton(AF_INET, LOCAL_IP1, &local_addr.sin_addr);
	    inet_pton(AF_INET, cp_addr1, &local_addr.sin_addr);

    if (bind(sockfd1, (struct sockaddr *)&local_addr1, sizeof(local_addr1)) < 0) {
        perror("bind1");
        close(sockfd1);
        return 1;
    }

    // 设置目标地址
    memset(&servaddr1, 0, sizeof(servaddr1));
    servaddr1.sin_family = AF_INET;
   // servaddr1.sin_port = htons(DEST_PORT1);
     servaddr1.sin_port = htons(ap_port1);
   // inet_pton(AF_INET, DEST_IP1, &servaddr1.sin_addr);
	inet_pton(AF_INET, ip_addr, &servaddr1.sin_addr);

    return 0;
}
int udp_client_init_v3() {

        char cp_addr[64], ap_ip_addr[64],ap_port[64],ip_ip_addr[64],ip_port[64];
     int cp_val,ap_ip_val,ap_port_val,ip_ip_val,ip_port_val;
    bool is_int1;
	get_ini_value("CP", "ip_addr", cp_addr, sizeof(cp_addr), &cp_val, &is_int1);
	get_ini_value("AP", "ip_addr", ap_ip_addr, sizeof(ap_ip_addr), &ap_ip_val, &is_int1);
	get_ini_value("AP", "ip_port", ap_port, sizeof(ap_port), &ap_port_val, &is_int1);
    get_ini_value("IP", "ip_addr", ip_ip_addr, sizeof(ip_ip_addr), &ip_ip_val, &is_int1);
	get_ini_value("IP", "ip_port", ip_port, sizeof(ip_port), &ip_port_val, &is_int1);	

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }

        // 明确绑定本地网卡 IP（eth0）
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = 0;  // 自动分配端口
	 inet_pton(AF_INET, cp_addr, &local_addr.sin_addr);

    if (bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return 1;
    }
      // 设置ap目标地址
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(ap_port_val);
	inet_pton(AF_INET, ap_ip_addr, &servaddr.sin_addr);
    
	     // 设置ip目标地址
    memset(&servaddr1, 0, sizeof(servaddr1));
    servaddr1.sin_family = AF_INET;
     servaddr1.sin_port = htons(ip_port_val);
	inet_pton(AF_INET, ip_ip_addr, &servaddr1.sin_addr);

    return 0;
}
// 发送数据函数：将消息发送给服务端，并接收响应
int udp_client_send(const char *msg) {
   // char buffer[BUF_SIZE];
    socklen_t len = sizeof(servaddr);
    //sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&servaddr, len);
  ssize_t sent = sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr *)&servaddr, len);
 //printf("Client:  send cnt %d\r\n", sent );
 printf("Client:  send %s\r\n", msg );
    if (sent < 0) {
		printf("Client: Cannot send %s", msg );
        perror("sendto failed");
		udp_client_close();
        return -1;
	}
    // int n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0, (struct sockaddr *)&servaddr, &len);
    // if (n < 0) {
    //     perror("recvfrom failed");
    //   //  return -1;
    // }

  // buffer[n] = '\0';
  // printf("Server replied: %s\n", buffer);
    return 0;
}

// 关闭 socket
void udp_client_close() {
    close(sockfd);
}
void udp_client_close1() {
    close(sockfd1);
}
int udp_client_send_data(const unsigned char *data, size_t len) {
    socklen_t addrlen = sizeof(servaddr);

   // 打印即将发送的数据内容
    // printf("Sending data (%ld bytes): ", len);
    // for (size_t i = 0; i < len; ++i) {
    //     printf("0x%02X ", data[i]);
    // }
    // printf("\n");

    ssize_t sent = sendto(sockfd, data, len, 0, (struct sockaddr *)&servaddr, addrlen);
    if (sent < 0) {
        perror("sendto failed");
        return -1;
    }

   // printf("Sent %ld bytes successfully.\n", sent);
    return 0;
}
int udp_client_send_data1(const unsigned char *data, size_t len) {
    socklen_t addrlen = sizeof(servaddr1);

    //打印即将发送的数据内容
    // printf("Sending1 data (%ld bytes): ", len);
    // for (size_t i = 0; i < len; ++i) {
    //     printf("0x%02X ", data[i]);
    // }
    // printf("\n");

    ssize_t sent = sendto(sockfd, data, len, 0, (struct sockaddr *)&servaddr1, addrlen);
    if (sent < 0) {
        perror("sendto failed1");
        return -1;
    }

   // printf("Sent %ld bytes successfully.\n", sent);
    return 0;
}
unsigned long get_current_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int double_equals(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

void autoreadreg(YxwDevice_t *hw_priv)
{
	uint32_t data;
     adrv9009_select = 1 ;
	/*for(int i=0;i<560;i++)
	{
		YXW_spiReadReg(&g_hw_priv,i,&data);
		printf("force TB.CHIP_TOP.U_digital_top.ahb2ip.SPI.sys_data_reg[%d]=32'h%08x;\n",i,data);
	}*/
	printf("**************Analog Regsiter***************\n");
	for(int i=0;i<=150;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_Analog_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	for(int i=524;i<=559;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_Analog_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	for(int i=1067;i<=1099;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_Analog_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	printf("\n");
	printf("\n");
	printf("\n");
	printf("**************Digtal Regsiter***************\n");
	for(int i=151;i<=493;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_digital_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	for(int i=502;i<=523;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_digital_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	for(int i=560;i<=570;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_digital_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	for(int i=1024;i<=1034;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_digital_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	for(int i=1038;i<=1066;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_digital_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	for(int i=1100;i<=1120;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_digital_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
	for(int i=2048;i<=3583;i++)
		{
			YXW_spiReadReg(hw_priv,i,&data);
			printf("force TB.CHIP_TOP.U_digital_top.ahb2ip.SPI.sys_data_reg[0x%x]=32'h%08x;\n",i,data);
		}
}

// 如果文件不存在，创建默认配置
void create_default_9009_ip_config() {
    FILE *file = fopen(CONFIG_FILE, "r");
    if (file) {
        fclose(file);  // 文件已存在，无需创建
        return;
    }

    file = fopen(CONFIG_FILE, "w");
    if (!file) {
        perror("Failed to create config file");
        exit(1);
    }

    fputs(DEFAULT_IP_CONFIG, file);
    fclose(file);
    printf("Created default ip config file: %s\n", CONFIG_FILE);
}
// 如果文件不存在，创建默认配置
void create_default_9009_parameter_config() {
    FILE *file = fopen(CONFIG_FILE_PARAMETER, "r");
    if (file) {
        fclose(file);  // 文件已存在，无需创建
        return;
    }

    file = fopen(CONFIG_FILE_PARAMETER, "w");
    if (!file) {
        perror("Failed to create config file");
        exit(1);
    }

    fputs(DEFAULT_PARAM_CONFIG, file);
    fclose(file);
   // printf("Created default parameter config file: %s\n", CONFIG_FILE_PARAMETER);
}
// 判断是否是纯数字
bool is_number(const char *s) {
    if (!s || *s == '\0') return false;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return false;
        s++;
    }
    return true;
}

bool get_ini_value(const char *section, const char *key,
                   char *out_str, size_t str_size,
                   int *out_int, bool *is_int) {
    FILE *file = fopen(CONFIG_FILE, "r");
    if (!file) {
        perror("Failed to open config file");
        return false;
    }

    char line[256];
    bool in_section = false;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '[' && line[strlen(line)-1] == ']') {
            char current_section[64];
            strncpy(current_section, line + 1, strlen(line) - 2);
            current_section[strlen(line)-2] = '\0';
            in_section = (strcmp(current_section, section) == 0);
            continue;
        }

        if (in_section) {
            char *delim = strchr(line, '=');
            if (!delim) continue;

            *delim = '\0';
            char *current_key = line;
            char *current_val = delim + 1;

            if (strcmp(current_key, key) == 0) {
                if (out_str) {
                    strncpy(out_str, current_val, str_size - 1);
                    out_str[str_size - 1] = '\0';
                }

                if (out_int && is_int) {
                    if (is_number(current_val)) {
                        *out_int = atoi(current_val);
                        *is_int = true;
                    } else {
                        *is_int = false;
                    }
                }

                fclose(file);
                return true;
            }
        }
    }

    fclose(file);
    return false;
}

bool set_ini_value(const char *section, const char *key,
                   const char *new_value) {
    FILE *file = fopen(CONFIG_FILE_PARAMETER, "r");
    if (!file) {
        perror("Failed to open config file");
        return false;
    }

    char lines[200][256];  // 假设文件最多 200 行
    int line_count = 0;
    bool in_section = false;
    bool found = false;

    while (fgets(lines[line_count], sizeof(lines[0]), file)) {
        lines[line_count][strcspn(lines[line_count], "\n")] = '\0';

        if (lines[line_count][0] == '[' && lines[line_count][strlen(lines[line_count])-1] == ']') {
            char current_section[64];
            strncpy(current_section, lines[line_count] + 1, strlen(lines[line_count]) - 2);
            current_section[strlen(lines[line_count]) - 2] = '\0';
            in_section = (strcmp(current_section, section) == 0);
        }

        if (in_section && !found) {
            char *delim = strchr(lines[line_count], '=');
            if (delim) {
                char current_key[64];
                strncpy(current_key, lines[line_count], delim - lines[line_count]);
                current_key[delim - lines[line_count]] = '\0';

                if (strcmp(current_key, key) == 0) {
                    snprintf(lines[line_count], sizeof(lines[0]), "%s=%s", key, new_value);
                    found = true;
                }
            }
        }

        line_count++;
    }

    fclose(file);

    if (!found) {
        printf("Key '%s' not found in section '[%s]'\n", key, section);
        return false;
    }

    file = fopen(CONFIG_FILE, "w");
    if (!file) {
        perror("Failed to write config file");
        return false;
    }

    for (int i = 0; i < line_count; i++) {
        fprintf(file, "%s\n", lines[i]);
    }

    fclose(file);

    if (is_number(new_value)) {
        printf("Updated [%s] %s=%d\n", section, key, atoi(new_value));
    } else {
        printf("Updated [%s] %s=%s\n", section, key, new_value);
    }

    return true;
}




bool set_ini_value_double(const char *section, const char *key, double new_value) {
    FILE *file = fopen(CONFIG_FILE_PARAMETER, "r");
    if (!file) {
        perror("Failed to open config file");
        return false;
    }

    char lines[200][256];  // 假设文件最多 200 行
    int line_count = 0;
    bool in_section = false;
    bool found = false;

    while (fgets(lines[line_count], sizeof(lines[0]), file)) {
        lines[line_count][strcspn(lines[line_count], "\n")] = '\0';

        // 判断是否进入目标 section
        if (lines[line_count][0] == '[' &&
            lines[line_count][strlen(lines[line_count]) - 1] == ']') {
            char current_section[64];
            strncpy(current_section, lines[line_count] + 1,
                    strlen(lines[line_count]) - 2);
            current_section[strlen(lines[line_count]) - 2] = '\0';
            in_section = (strcmp(current_section, section) == 0);
        }

        // 如果在目标 section，查找 key
        if (in_section && !found) {
            char *delim = strchr(lines[line_count], '=');
            if (delim) {
                char current_key[64];
                strncpy(current_key, lines[line_count], delim - lines[line_count]);
                current_key[delim - lines[line_count]] = '\0';

                if (strcmp(current_key, key) == 0) {
                    // 用 double 格式化输出，保留 6 位小数（可调节）
                    snprintf(lines[line_count], sizeof(lines[0]),
                             "%s=%.2f", key, new_value);
                    found = true;
                }
            }
        }

        line_count++;
    }
    fclose(file);

    if (!found) {
        printf("Key '%s' not found in section '[%s]'\n", key, section);
        return false;
    }

    // 重新写回配置文件
    file = fopen(CONFIG_FILE_PARAMETER, "w");
    if (!file) {
        perror("Failed to write config file");
        return false;
    }
    for (int i = 0; i < line_count; i++) {
        fprintf(file, "%s\n", lines[i]);
    }
    fclose(file);

    printf("Updated [%s] %s=%.2f\n", section, key, new_value);
    return true;
}
