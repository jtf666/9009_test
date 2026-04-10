

#include "main.h"
#include <unistd.h>

#define  K7_SPI_CLK_PERIOD_IN_US  2


void  K7_spi_write_reg( uint8_t ctrl_byte )
{
//	uint32_t i=0;
	uint8_t j=0;
	uint32_t spi_3bytes = 0;
//	for( i=0; (bytes_numb-i) >= 3; )
//	{
		//MSB是固定数值0x55，MSB是固定数值0xAA
		//提取出24位的SPI串行数据
		spi_3bytes = 0 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出MSB字节
		spi_3bytes = spi_3bytes | 0x55 ;

		spi_3bytes = spi_3bytes << 8 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出中间字节
		spi_3bytes = spi_3bytes | ctrl_byte ;

		spi_3bytes = spi_3bytes << 8 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出LSB字节
		spi_3bytes = spi_3bytes | 0xAA ;

		//先将SPI信号拉高一段时间
		set_gpio_val(EMIO_NUM_K7_CS, 0);
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		set_gpio_val(EMIO_NUM_K7_MOSI, 0);
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//在cs拉低之前，需要事先将时钟拉低
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//拉低cs
		set_gpio_val(0, EMIO_NUM_K7_CS);

		for(j=0; j<24; j++)
		{
			//拉低时钟
			set_gpio_val(0, EMIO_NUM_K7_SCK);
			//送出数据
			if(spi_3bytes & 0x800000)
			{
				set_gpio_val(EMIO_NUM_K7_MOSI, 0);
			}
			else
			{
				set_gpio_val(0, EMIO_NUM_K7_MOSI);
			}
			//延迟1us
			usleep(K7_SPI_CLK_PERIOD_IN_US/2);

			//拉高时钟
			set_gpio_val(EMIO_NUM_K7_SCK, 0);
			//延迟1us
			usleep(K7_SPI_CLK_PERIOD_IN_US/2);

			//串行数据左移一位
			spi_3bytes <<= 1;
		}

		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);
		//拉高cs
		set_gpio_val(EMIO_NUM_K7_CS, 0);
//	}
}
uint8_t K7_spi_read_reg( void )
{
	uint32_t spi_data = 0;
	uint8_t reg_rd_data =0;
	uint8_t i = 0;

//	uint32_t pin_reg_value = 0;

	//赋值读标志位
//	spi_data = spi_data | 0x01 ;

	//赋值地址位
//	spi_data = spi_data << 15 ;
//	spi_data = spi_data | (reg_addr & 0x7FFF);

	spi_data = 0x33CC ;

//	printf("spi_data is: %x\n\r",spi_data);

	//先将SPI信号拉高一段时间
	set_gpio_val(EMIO_NUM_K7_CS, 0);
	set_gpio_val(EMIO_NUM_K7_SCK, 0);
	set_gpio_val(EMIO_NUM_K7_MOSI, 0);
	usleep(K7_SPI_CLK_PERIOD_IN_US/2);

	//在cs拉低之前，需要事先将时钟拉低
	set_gpio_val(0, EMIO_NUM_K7_SCK);
	usleep(K7_SPI_CLK_PERIOD_IN_US/2);

	//拉低cs
	set_gpio_val(0, EMIO_NUM_K7_CS);

	//送出读标志和地址
	for(i=0; i<16; i++)
	{
		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//送出数据
		if(spi_data & 0x8000)
		{
//			printf("spi_data & 0x8000 is 1 \n\r");
			set_gpio_val(EMIO_NUM_K7_MOSI, 0);
		}
		else
		{
//			printf("spi_data & 0x8000 is 0 \n\r");
			set_gpio_val(0, EMIO_NUM_K7_MOSI);
		}
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//拉高时钟
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//串行数据左移一位
		spi_data <<= 1;
	}

	//读数据
	for(i=0; i<8; i++)
	{
		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//读数据
		reg_rd_data <<= 1 ;

//		pin_reg_value = get_gpio_val(NUM__CPLD_PS_SPI_MISO) ;
//		printf("pin_reg_value is %x\n\r",pin_reg_value);
//		if( pin_reg_value )

		if( get_gpio_val(EMIO_NUM_K7_MISO) )
		{
//			printf("lmk04828 miso is 1\n\r");
			reg_rd_data = reg_rd_data | 0x01;
		}
		else
		{
//			printf("lmk04828 miso is 0\n\r");
		}

		//拉高时钟
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);
	}

	//拉高cs
	set_gpio_val(EMIO_NUM_K7_CS, 0);

//	printf("K7 SPI reg_rd_data is %x \n\r" , reg_rd_data );

	return reg_rd_data ;
}

void  K7_spi_write_tx_dsa_att( uint8_t dsa_att_value )
{
//	u32 i=0;
	uint8_t j=0;
	uint32_t spi_3bytes = 0;

	uint32_t k7_cs_num;
	uint32_t k7_sck_num;
	uint32_t k7_mosi_num;
	uint32_t k7_miso_num;


		k7_cs_num   = EMIO_NUM_K7_CS ;
		k7_sck_num  = EMIO_NUM_K7_SCK ;
		k7_mosi_num = EMIO_NUM_K7_MOSI ;
		k7_miso_num = EMIO_NUM_K7_MISO ;
	


//	for( i=0; (bytes_numb-i) >= 3; )
//	{
		//MSB是固定数值0x55，MSB是固定数值0xAA
		//提取出24位的SPI串行数据
		spi_3bytes = 0 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出MSB字节
		spi_3bytes = spi_3bytes | 0x33 ;

		spi_3bytes = spi_3bytes << 8 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出中间字节
		spi_3bytes = spi_3bytes | dsa_att_value ;

		spi_3bytes = spi_3bytes << 8 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出LSB字节
		spi_3bytes = spi_3bytes | 0xCC ;

		//先将SPI信号拉高一段时间
		set_gpio_val(k7_cs_num, 0);
		set_gpio_val(k7_sck_num, 0);
		set_gpio_val(k7_mosi_num, 0);
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//在cs拉低之前，需要事先将时钟拉低
		set_gpio_val(0,k7_sck_num);
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//拉低cs
		set_gpio_val(0,k7_cs_num);

		for(j=0; j<24; j++)
		{
			//拉低时钟
			set_gpio_val(0,k7_sck_num);
			//送出数据
			if(spi_3bytes & 0x800000)
			{
				set_gpio_val(k7_mosi_num, 0);
			}
			else
			{
				set_gpio_val(0,k7_mosi_num);
			}
			//延迟1us
			usleep(K7_SPI_CLK_PERIOD_IN_US/2);

			//拉高时钟
			set_gpio_val(k7_sck_num, 0);
			//延迟1us
			usleep(K7_SPI_CLK_PERIOD_IN_US/2);

			//串行数据左移一位
			spi_3bytes <<= 1;
		}

		//拉低时钟
		set_gpio_val(0,k7_sck_num);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);
		//拉高cs
		set_gpio_val(k7_cs_num, 0);
//	}
}
void  K7_spi_write_reg_sks_agc( uint8_t ctrl_byte )  //一秒返回一次rx agc 到k7
{
//	uint32_t i=0;
	uint8_t j=0;
	uint32_t spi_3bytes = 0;

//	for( i=0; (bytes_numb-i) >= 3; )
//	{
		//MSB是固定数值0x55，MSB是固定数值0xAA
		//提取出24位的SPI串行数据
		spi_3bytes = 0 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出MSB字节
		spi_3bytes = spi_3bytes | 0x55 ;

		spi_3bytes = spi_3bytes << 8 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出中间字节
		spi_3bytes = spi_3bytes | ctrl_byte ;

		spi_3bytes = spi_3bytes << 8 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出LSB字节
		spi_3bytes = spi_3bytes | 0xBB ;

		//先将SPI信号拉高一段时间
		set_gpio_val(EMIO_NUM_K7_CS, 0);
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		set_gpio_val(EMIO_NUM_K7_MOSI, 0);
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//在cs拉低之前，需要事先将时钟拉低
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//拉低cs
		set_gpio_val(0, EMIO_NUM_K7_CS);

		for(j=0; j<24; j++)
		{
			//拉低时钟
			set_gpio_val(0, EMIO_NUM_K7_SCK);
			//送出数据
			if(spi_3bytes & 0x800000)
			{
				set_gpio_val(EMIO_NUM_K7_MOSI, 0);
			}
			else
			{
				set_gpio_val(0, EMIO_NUM_K7_MOSI);
			}
			//延迟1us
			usleep(K7_SPI_CLK_PERIOD_IN_US/2);

			//拉高时钟
			set_gpio_val(EMIO_NUM_K7_SCK, 0);
			//延迟1us
			usleep(K7_SPI_CLK_PERIOD_IN_US/2);

			//串行数据左移一位
			spi_3bytes <<= 1;
		}

		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);
		//拉高cs
		set_gpio_val(EMIO_NUM_K7_CS, 0);
//	}
}


uint8_t K7_spi_read_reg_sks_agc_tx( void )  // 将收到的tx agc 显示出来 目前先打印出来 后续做接口
{
	uint32_t spi_data = 0;
	uint8_t reg_rd_data =0;
	uint8_t i = 0;

//	uint32_t pin_reg_value = 0;

	//赋值读标志位
//	spi_data = spi_data | 0x01 ;

	//赋值地址位
//	spi_data = spi_data << 15 ;
//	spi_data = spi_data | (reg_addr & 0x7FFF);

	spi_data = 0x00AA ;

//	printf("spi_data is: %x\n\r",spi_data);

	//先将SPI信号拉高一段时间
	set_gpio_val(EMIO_NUM_K7_CS, 0);
	set_gpio_val(EMIO_NUM_K7_SCK, 0);
	set_gpio_val(EMIO_NUM_K7_MOSI, 0);
	usleep(K7_SPI_CLK_PERIOD_IN_US/2);

	//在cs拉低之前，需要事先将时钟拉低
	set_gpio_val(0, EMIO_NUM_K7_SCK);
	usleep(K7_SPI_CLK_PERIOD_IN_US/2);

	//拉低cs
	set_gpio_val(0, EMIO_NUM_K7_CS);

	//送出读标志和地址
	for(i=0; i<16; i++)
	{
		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//送出数据
		if(spi_data & 0x8000)
		{
//			printf("spi_data & 0x8000 is 1 \n\r");
			set_gpio_val(EMIO_NUM_K7_MOSI, 0);
		}
		else
		{
//			printf("spi_data & 0x8000 is 0 \n\r");
			set_gpio_val(0, EMIO_NUM_K7_MOSI);
		}
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//拉高时钟
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//串行数据左移一位
		spi_data <<= 1;
	}

	//读数据
	for(i=0; i<8; i++)
	{
		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//读数据
		reg_rd_data <<= 1 ;

//		pin_reg_value = get_gpio_val(NUM__CPLD_PS_SPI_MISO) ;
//		printf("pin_reg_value is %x\n\r",pin_reg_value);
//		if( pin_reg_value )

		if( get_gpio_val(EMIO_NUM_K7_MISO) )
		{
//			printf("lmk04828 miso is 1\n\r");
			reg_rd_data = reg_rd_data | 0x01;
		}
		else
		{
//			printf("lmk04828 miso is 0\n\r");
		}

		//拉高时钟
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);
	}

	//拉高cs
	set_gpio_val(EMIO_NUM_K7_CS, 0);

//	printf("K7 SPI reg_rd_data is %x \n\r" , reg_rd_data );

	return reg_rd_data ;
}
uint8_t K7_spi_read_reg_sks_agc_rx( void )   // 将收到的rx agc 显示出来 目前先打印出来 后续做接口
{
	uint32_t spi_data = 0;
	uint8_t reg_rd_data =0;
	uint8_t i = 0;

//	uint32_t pin_reg_value = 0;

	//赋值读标志位
//	spi_data = spi_data | 0x01 ;

	//赋值地址位
//	spi_data = spi_data << 15 ;
//	spi_data = spi_data | (reg_addr & 0x7FFF);

	spi_data = 0x11AA ;

//	printf("spi_data is: %x\n\r",spi_data);

	//先将SPI信号拉高一段时间
	set_gpio_val(EMIO_NUM_K7_CS, 0);
	set_gpio_val(EMIO_NUM_K7_SCK, 0);
	set_gpio_val(EMIO_NUM_K7_MOSI, 0);
	usleep(K7_SPI_CLK_PERIOD_IN_US/2);

	//在cs拉低之前，需要事先将时钟拉低
	set_gpio_val(0, EMIO_NUM_K7_SCK);
	usleep(K7_SPI_CLK_PERIOD_IN_US/2);

	//拉低cs
	set_gpio_val(0, EMIO_NUM_K7_CS);

	//送出读标志和地址
	for(i=0; i<16; i++)
	{
		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//送出数据
		if(spi_data & 0x8000)
		{
//			printf("spi_data & 0x8000 is 1 \n\r");
			set_gpio_val(EMIO_NUM_K7_MOSI, 0);
		}
		else
		{
//			printf("spi_data & 0x8000 is 0 \n\r");
			set_gpio_val(0, EMIO_NUM_K7_MOSI);
		}
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//拉高时钟
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//串行数据左移一位
		spi_data <<= 1;
	}

	//读数据
	for(i=0; i<8; i++)
	{
		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//读数据
		reg_rd_data <<= 1 ;

//		pin_reg_value = get_gpio_val(NUM__CPLD_PS_SPI_MISO) ;
//		printf("pin_reg_value is %x\n\r",pin_reg_value);
//		if( pin_reg_value )

		if( get_gpio_val(EMIO_NUM_K7_MISO) )
		{
//			printf("lmk04828 miso is 1\n\r");
			reg_rd_data = reg_rd_data | 0x01;
		}
		else
		{
//			printf("lmk04828 miso is 0\n\r");
		}

		//拉高时钟
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);
	}

	//拉高cs
	set_gpio_val(EMIO_NUM_K7_CS, 0);

printf(" read_sks_data is %d \n\r" , reg_rd_data );
set_ini_value_double("RX", "att_out", reg_rd_data);

	return reg_rd_data ;
}
void  K7_spi_write_9009_rx_agc_restart( uint8_t ctrl_byte )
{
//	uint32_t i=0;
	uint8_t j=0;
	uint32_t spi_3bytes = 0;
//	for( i=0; (bytes_numb-i) >= 3; )
//	{
		//MSB是固定数值0x55，MSB是固定数值0xAA
		//提取出24位的SPI串行数据
		spi_3bytes = 0 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出MSB字节
		spi_3bytes = spi_3bytes | 0x55 ;

		spi_3bytes = spi_3bytes << 8 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出中间字节
		spi_3bytes = spi_3bytes | ctrl_byte ;

		spi_3bytes = spi_3bytes << 8 ;
//		spi_3bytes = spi_3bytes | txbuf[i++] ;  //提取出LSB字节
		spi_3bytes = spi_3bytes | 0xCC ;

		//先将SPI信号拉高一段时间
		set_gpio_val(EMIO_NUM_K7_CS, 0);
		set_gpio_val(EMIO_NUM_K7_SCK, 0);
		set_gpio_val(EMIO_NUM_K7_MOSI, 0);
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//在cs拉低之前，需要事先将时钟拉低
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);

		//拉低cs
		set_gpio_val(0, EMIO_NUM_K7_CS);

		for(j=0; j<24; j++)
		{
			//拉低时钟
			set_gpio_val(0, EMIO_NUM_K7_SCK);
			//送出数据
			if(spi_3bytes & 0x800000)
			{
				set_gpio_val(EMIO_NUM_K7_MOSI, 0);
			}
			else
			{
				set_gpio_val(0, EMIO_NUM_K7_MOSI);
			}
			//延迟1us
			usleep(K7_SPI_CLK_PERIOD_IN_US/2);

			//拉高时钟
			set_gpio_val(EMIO_NUM_K7_SCK, 0);
			//延迟1us
			usleep(K7_SPI_CLK_PERIOD_IN_US/2);

			//串行数据左移一位
			spi_3bytes <<= 1;
		}

		//拉低时钟
		set_gpio_val(0, EMIO_NUM_K7_SCK);
		//延迟1us
		usleep(K7_SPI_CLK_PERIOD_IN_US/2);
		//拉高cs
		set_gpio_val(EMIO_NUM_K7_CS, 0);
//	}
}