
#ifndef _K7_SPI__
#define _K7_SPI__



extern void  K7_spi_write_reg( uint8_t ctrl_byte );
extern uint8_t K7_spi_read_reg( void );
extern void  K7_spi_write_tx_dsa_att( uint8_t dsa_att_value );
extern void  K7_spi_write_reg_sks_agc( uint8_t ctrl_byte );
extern uint8_t K7_spi_read_reg_sks_agc_tx( void );
extern uint8_t K7_spi_read_reg_sks_agc_rx( void );
extern void  K7_spi_write_9009_rx_agc_restart( uint8_t ctrl_byte );
#define  BIT0_RX_GT_REFCLK_EN         (1<<0)  //K7需要对它反向
#define  BIT1_RX_JESD204_PHY_SYS_RESET  (1<<1)
#define  BIT3_RX_JESD204_CORE_RESET     (1<<3)
#define  BIT4_RX_FPGA_SYSREF_EN       (1<<4)

#define  BIT2_TX_GT_REFCLK_EN         (1<<2)  //K7需要对它反向
#define  BIT5_TX_JESD204_PHY_SYS_RESET  (1<<5)
#define  BIT6_TX_JESD204_CORE_RESET     (1<<6)
#define  BIT7_TX_FPGA_SYSREF_EN       (1<<7)


#endif
