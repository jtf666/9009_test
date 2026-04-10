
#include"yxw_config.h"
#define bandwidth60M
//#define bandwidthdefM
//MODE 1
//tx:245 rx:245 orx:245
// if .tx.numFirCoefs = 20/40/80[491/245/122], the first 20/40/80 valid elements of the array are effective
#if 0
sint16_t yxwTxFirCoefs[80]={-19,-7,57,11,-112,-36,208,81,-349,-170,
                                                        556,328,-855,-610,1322,1155,-2206,-2533,5134,14425,
                                                        14425,5134,-2533,-2206,1155,1322,-610,-855,328,556,
                                                        -170,-349,81,208,-36,-112,11,57,-7,-19,
                                                        0,0,0,0,0,0,0,0,0,0,
                                                        0,0,0,0,0,0,0,0,0,0,
                                                        0,0,0,0,0,0,0,0,0,0,
                                                        0,0,0,0,0,0,0,0,0,0
														};//fix=32768"

sint16_t yxwTxFirCoefs[80]={78,3,111,5,-194,-30,347,93,-583,-217,928,450,-1394,-845,2022,1525,
-2846,-2715,3943,5091,-4883,-9105,8164,31422,31422,8164,-9105,-4883,5091,3943,-2715,-2846,
1525,2022,-845,-1394,450,928,-217,-583,93,347,-30,-194,5,111,3,-78,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#endif
sint16_t yxwTxFirCoefs[80]={-585,297,636,384,-186,-657,-703,-264,422,947,950,326,-676,
-1534,-1667,-703,1321,3925,6340,7789,7789,6340,3925,1321,-703,-1667,-1534,
-676,326,950,947,422,-264,-703,-657,-186,384,636,297,-585,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//fix=32768"														
// if .rx.numFirCoefs = 48/72[245/122], the first 48/72 valid elements of the array are effective
sint16_t yxwRxFirCoefs[72]={-78,3,111,5,-194,-30,347,93,-583,-217,928,450,-1394,-845,2022,1525,
-2846,-2715,3943,5091,-4883,-9105,8164,31422,31422,8164,-9105,-4883,5091,3943,-2715,-2846,
1525,2022,-845,-1394,450,928,-217,-583,93,347,-30,-194,5,111,3,-78,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//tap48,fix=65536
// if .orx.numFirCoefs = 72[491/245], the first 72 valid elements of the array are effective
#ifdef bandwidthdefM
sint16_t yxwObsrxFirCoefs[72]= {-2,7,-1,-12,1,21,2,-35,-8,53,21,-76,-45,103,82,-130,
					-138,155,217,-170,-324,168,464,-137,-645,62,879,85,-1195,
					-354,1667,884,-2545,-2245,5459,14118,14118,5459,-2245,-2545,
					884,1667,-354,-1195,85,879,62,-645,-137,464,168,-324,-170,217,
					155,-138,-130,82,103,-45,-76,21,53,-8,-35,2,21,1,-12,-1,7,-2};//tap72,fix=32768
#endif
#ifdef bandwidth60M
sint16_t yxwObsrxFirCoefs[72]={25,26,2,-39,-75,-73,-17,78,160,166,62,-121,-289,-324,-162,155,469,579,356,-154,
    -711,-979,-712,73,1038,1637,1390,185,-1554,-2945,-2987,-1020,2896,7881,12492,15259,
    15259,12492,7881,2896,-1020,-2987,-2945,-1554,185,1390,1637,1038,73,-712,-979,-711,-154,
    356,579,469,155,-162,-324,-289,-121,62,166,160,78,-17,-73,-75,-39,2,26,25};
#endif
YxwGainTab_t RxOrxGainTab_0_5_STEP =
{
	.sizeofTb = 64,
	.anaGainTB =
	{
		0x0,0x0,0x19,0x19,0x33,0x33,0x4b,0x4b,0x62,0x62,0x75,
		0x75,0x85,0x85,0x94,0x94,0xa3,0xa3,0xae,0xae,0xb8,0xb8,
		0xc3,0xc3,0xc9,0xc9,0xcf,0xcf,0xd5,0xd5,0xda,0xda,0xe0,
		0xe0,0xe4,0xe4,0xe7,0xe7,0xea,0xea,0xec,0xec,0xee,0xee,
		0xf0,0xf0,0xf2,0xf2,0xf3,0xf3,0xf4,0xf4,0xf5,0xf5,0xf6,
		0xf6,0xf7,0xf7,0xf8,0xf8,0xf9,0xf9,0xfa,0xfa
	},
	.digGainTB =
	{
		0x3,0x7f9,0x1,0x7f7,0x7fe,0x7f4,0x7fd,0x7f3,0x7fd,0x7f3,
		0x7fe,0x7f4,0x7fe,0x7f4,0x7ff,0x7f5,0x7ff,0x7f5,0x0,0x7f6,
		0x2,0x7f8,0x5,0x7fa,0x2,0x7f8,0x2,0x7f8,0x4,0x7fa,
		0x6,0x7fc,0x3,0x7f9,0x6,0x7fc,0x6,0x7fc,0x8,0x7fe,
		0x4,0x7fa,0x3,0x7f9,0x4,0x7fa,0x8,0x7fe,0x3,0x7f9,
		0x7fd,0x7f3,0x7fb,0x7f1,0x7f9,0x7ef,0x7fb,0x7f1,0x7fd,0x7f3,
		0x6,0x7fc,0x11,0x7
	},
};
YxwInit_t Y9009Init_rx=
{
	.Loglevel =1,		 	/*The DebugLevel:0-disable,1-err,2-Result,3-Status,4-Debug,Default is status*/  
    .mode = 0, /*PLL mode:0-TX/RX/ORX=RFPLL;  1-TX=RFPLL,RX=RFPLL,ORX=AuxPLL;  2-TX1+ORX1ARX1D=RFPLL,TX2+ORX2ARX2D=AuxPLL*/
	.initcaltype= RX_QEC_INIT| ORX_DC_OFFSET,
    /*Rxsettings*/
    .rx=
    {
        .rxProfile=
        {
            .rxFir=
            {
                .gain_dB=-12,                 /*The setting(in DB) for the gain block within the Rx FIR(18,12,6,0,-6,-12-18,-24) 0=FIX(16384)*/
                .numFirCoefs=48,            /*The number softapstobeusedintheRxFir(24,48,72)*/
                .coefs=&yxwRxFirCoefs[0],
            },
            .rxFirBypass=1,                 /*The RX FIR bypass enable (0,1)*/
            .rxFirDecimation=1,             /*The Rx FIR decimation (1,2,4) */
            .rhb1Decimation=2,              /*The Rx Halfband1 Decimation setting(1,2) */
            .rxOutputRate_kHz=245760,       /*The I/Q data rate at input to the Rx output data rate in kHz range 245.76/122.88/62.44Mhz*/
            .rfBandwidth_Hz=200000000,      /*The Rx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz=220000,     /*The Rx BBF 3dB corner in kHz  <1.1 * rxrfBandwidth_Hz/1000>*/
        },
        .rxGainCtrl=
        {
            .rxgainMode=YXW_MGC,             /*The MGC mode setting(0,1)*/
            .rxmgcMode=YXW_GPIOMODE,          /*The Ways for mgcMode(0,1) */
            .rx1GainIndex=127,              /*The rx1GainIndex setting(0,127) */
            .rx2GainIndex=127,              /*The rx2GainIndex setting(0,127)*/
            .rx1MaxGainIndex=127,           /*The Rx1 GainMax index 127*/
            .rx1MinGainIndex=0,             /*The Rx1 GainMix index 0;*/
            .rx2MaxGainIndex=127,           /*The Rx2 GainMax index 127*/
            .rx2MinGainIndex=0              /*The Rx2 GainMix index 0;*/
        },
        .rxGainCtrlPinInfo=
        {
            .incStep=1,                     /*Increment in gain index applied when the increment gain pin is pulsed(1-16) */
            .decStep=1,                     /*Decrement in gain index applied when the increment gain pin is pulsed(1-16) */
            .rx1GainCtrlPinInfo=
            {
                .rxGainIncPin=YXW_GPIO_00,   /*GPIO used for the rx1 Increment gain input*/
                .rxGainDecPin=YXW_GPIO_01,   /*GPIO used for the rx1 Decrement gain input*/
                .rxGainIncPinFunc=YXW_SEL_4, /*GPIO function select 0-7: make GPIO function rx1 gain Increment function */
                .rxGainDecPinFunc=YXW_SEL_4, /*GPIO function select 0-7:*make GPIO function rx1 gain Decrement function */
            },
            .rx2GainCtrlPinInfo=
            {
                .rxGainIncPin=YXW_GPIO_03,   /*GPIO used for the rx2 Increment gain input*/
                .rxGainDecPin=YXW_GPIO_04,   /*GPIO used for the rx2 Decrement gain input*/
                .rxGainIncPinFunc=YXW_SEL_4, /*GPIO function select 0-7: make GPIO function rx2 gain Increment function */
                .rxGainDecPinFunc=YXW_SEL_4, /*GPIO function select 0-7:*make GPIO function rx2 gain Decrement function */
            },
        },
        .rxSlicerPinInfo=
        {
            .EnableSlicer=YXW_RXSLICER_EN,   /*Default No Digital gain compensation slicer output*/
            .rx1SlicerPinInfo=
            {
                .rx1SlicerPin_0=YXW_GPIO_07, /*GPIO used for the rx1 Slicer pin output*/
                .rx1SlicerPin_1=YXW_GPIO_08, /*GPIO used for the rx1 Slicer pin output*/
                .rx1SlicerPin_2=YXW_GPIO_09, /*GPIO used for the rx1 Slicer pin output*/
            },
            .rx2SlicerPinInfo=
            {
                .rx2SlicerPin_0=YXW_GPIO_10, /*GPIO used for the rx2 Slicer pin output*/
                .rx2SlicerPin_1=YXW_GPIO_11, /*GPIO used for the rx2 Slicer pin output*/
                .rx2SlicerPin_2=YXW_GPIO_12, /*GPIO used for the rx2 Slicer pin output*/
            },
            .rxSlicerPinFunc=YXW_SEL_3,      /*GPIO function select 0-7: make GPIO function slicer function */
        },
        .rxChannels=YXW_RX1,               /*Select The enable rx channel*/
    },

    /*Tx settings*/
    .tx=
    {
        .txProfile=
        {
            .txFir=
            {
                .gain_dB=0,                 /* The setting(in DB) for the gain block within the Tx FIR(18,12,6,0,-6,-12-18,-24) 0=FIX(16384) */
                .numFirCoefs=40,            /* The numbers of taps to be used in the Tx Fir (20,40,80) */
                .coefs=&yxwTxFirCoefs[0],
            },
            .txFirBypass=1,                 /* The TX FIR bypass enable (0-enable,1-disable)*/
            .txFirInterpolation=1,          /* The Tx FIR filter interpolation setting(1-diable,2,4)     */
            .thb1Interpolation=2,           /* The Tx Halfband1 filter interpolation setting(1-disable,2-enable) */
            .thb2Interpolation=2,           /* The Tx Halfband2 filter interpolation settingg(1-disable,2-enable)*/
            .thb3Interpolation=2,           /* The Tx Halfband3 filter interpolation setting(1-disable,2-enable) */
            .txInputRate_kHz=245760,        /* The I/Q data rate at input to the TxFir rate in kHz range 491.52/245.76/122.88Mhz*/
            .rfBandwidth_Hz=200000000,      /* The Tx RF passband bandwidth for the profile */
            .txBbf3dBCorner_kHz=220000,     /*  The Tx BBF 3dB corner in kHz  <1.1 * txrfBandwidth_Hz/1000>*/
        },
		.txChannels = YXW_TXOFF,         /* Select The enable tx channel */
        .txGainCtrl=
        {
            .txgainMode=YXW_MGC,             /* The MGC mode setting(0)*/
            .txmgcMode=YXW_APIMODE,          /* The Ways for mgcMode(0) */
            .tx1GainIndex=0,                /* The tx1GainIndex setting(0,63) */
            .tx2GainIndex=0,                /* The tx2GainIndex setting(0,63) */
            .tx1MaxGainIndex=63,            /* The Tx1 GainMax index 63*/
            .tx1MinGainIndex=0,             /* The Tx1 GainMin index 0*/
            .tx2MaxGainIndex=63,            /* The Tx2 GainMax index 63*/
            .tx2MinGainIndex=0,             /* The Tx2 GainMin index 0*/
        },
    },

    /*ObsRx settings*/
    .obsRx=
    {
        .orxProfile=
        {
            .rxFir=
            {
                .gain_dB=-6,                 /* The setting(in DB) for the gain block within the Orx FIR(18,12,6,0,-6,-12-18,-24)  0=FIX(16384)*/
                .numFirCoefs=72,            /* The numbers of taps to be used in the Orx Fir 72 */
                .coefs=&yxwObsrxFirCoefs[0],
            },
            .orxFirBypass=0,                /* The ORX FIR bypass enable (0,1)*/
            .orxFirDecimation=2,            /* The ORx FIR decimation (1,2) */
            .orxOutputRate_kHz=245760,      /* The I/Q data rate at input to the ORX data rate in kHz range 491.52/245.76Mhz*/
            .rfBandwidth_Hz=200000000,      /* The ORx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz=220000,     /* The ORx BBF 3dB corner in kHz  <1.1 * orxrfBandwidth_Hz/1000>*/
        },
        .TableP=NULL, 	/*The OrxGain Table,Default is NULL(0.25db stepsize)*/
        .orxGainCtrl=
        {
            .orxgainMode=YXW_MGC,            /* The MGC mode setting(0,1)*/
            .orxmgcMode=YXW_APIMODE,         /* The Ways for mgcMode(0,1) */
            .orx1GainIndex=127,             /* The orx1GainIndex setting(0,127) */
            .orx2GainIndex=127,             /* The orx2GainIndex setting(0,127) */
            .orx1MaxGainIndex=127,          /* The Orx1 GainMax index 127*/
            .orx1MinGainIndex=0,            /* The Orx1 GainMax index 0*/
            .orx2MaxGainIndex=127,          /* The Orx2 GainMax index 127*/
            .orx2MinGainIndex=0,            /* The Orx2 GainMax index 0*/
        },
        .orxSlicerPinInfo = 
		{
			.EnableSlicer = YXW_ORXSLICER_EN,		/*Default No Digital gain compensation slicer output*/
			.orxSlicerPinInfo = 
			{
				.orxSlicerPin_0   = YXW_GPIO_13,  /*GPIO used for the orx1 Slicer pin output*/
				.orxSlicerPin_1   = YXW_GPIO_14,  /*GPIO used for the orx1 Slicer pin output*/
				.orxSlicerPin_2   = YXW_GPIO_15,  /*GPIO used for the orx1 Slicer pin output*/
			},			
			.orxSlicerPinFunc = YXW_SEL_3,  		 /*GPIO function select 0-7: make GPIO function slicer function */
		},
        .obsRxChannelsEnable=YXW_ORX1,     /* The desired ObsRx Channels to enable during initialization */
    },

    /*Digital Clock settings*/
	 .clocks =
	{
		.deviceClock_kHz    = 76800,  			    /* device reference clock frequency in kHz */
		.rfpll_frequency_Hz  = 4000000000ULL,  	    /* RFPll frequency in Hz */
		.auxpll_frequency_Hz  = 4000000000ULL,      /* AuxPll frequency in Hz */
		.rfPllUseExternalLo = 0,         			/* 1= Use external LO for RF PLL, 0 = use internal LO generation for RF PLL */
		.rfPllPhaseSyncMode = YXW_RFPLLMCS_NOSYNC  	/* RFPLL MCS (Phase sync) mode */
	},

	/* JESD204B settings */
		.jesd204Settings =
		{
			.serdesClk = SERDES_CLK_10G, /* JESD204B  Transmission Speed (10G:real speed 9.8304G, 5G:real speed 4.9152G)*/

			/* tx Config settings */
			.txConfig =
			{
				.lid						 = 0x0a418820,/* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
				.subClass					 = 1,         /* Subclass mode of operation (0, 1, or 2) */
				.cf 						 = 0,         /* number of control words per converter in 1 frame period */
				.cs 						 = 0,         /* number of control bits in each converter sample data */
				.f							 = 4,         /* F (number of bytes per JESD204 Frame) */
				.k							 = 32,        /* the number of frames in each multi frame. It is recommended to set it to 32 */
				.l							 = 1,         /* number of channels (l can be set to 1 ,2 or 4) */
				.n							 = 16,        /* converter resolution (n can be set to 16) */
				.m							 = 2,         /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
				.ntotal 					 = 16,        /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
				.s							 = 1,         /* number of samples sent by each converter per frame (s can be set to 1) */
				.hd 						 = 0x0,       /* High Density Format(0:High Density format not used,1:High Density format used)*/
				.rxCoreInterruptMask 	     = 0xFFFD,    /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
				.rxIlasAdnMiscDetails12c     = 0x0F000080,/* Register to hold details of ILAS length and RBD information */
				.scr						 = 0x0,       /* Scrambling enabled */
				.enableLinkTest 			 = 0x0,       /* Enable of link layer test(0:No link layer test performed.1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).
																					  2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2).3:No link layer test performed)*/
				.rxFeatureDisable 		     = 0x10,	  /*Disable features in the Rx module for compatibility with other devices.*/
			},
			/* rx Config settings */
			.rxConfig =
			{
				.lid						 = 0x0a418820,/* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
	   			.subClass					 = 1,         /* Subclass mode of operation (0, 1, or 2) */
	   			.cf 						 = 0,         /* number of control words per converter in 1 frame period */
	   			.cs 						 = 0,         /* number of control bits in each converter sample data */
	   			.f							 = 4,         /* F (number of bytes per JESD204 Frame) */
	   			.k							 = 32,        /* the number of frames in each multi frame. It is recommended to set it to 32 */
	   			.l							 = 1,         /* number of channels (l can be set to 1 ,2 or 4) */
	   			.n							 = 16,        /* converter resolution (n can be set to 16) */
	   			.m							 = 2,         /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
	   			.ntotal 					 = 16,        /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
	   			.s							 = 1,         /* number of samples sent by each converter per frame (s can be set to 1) */
	   			.hd 						 = 0x0,       /* High Density Format(0:High Density format not used,1:High Density format used) Currently not supported*/
				.txIlasDelay				 = 0xc,      /* Defines the delay from initial sample requests on the TX sample interface until the link layer requests the first frame from the transport layer.
															The resolution is clk_character cycles. A rough formula is:F=1 - delay must be set to 4,F=2 - delay must be set to 6,F=3 - delay must be set to 8
															F>3 - delay must be 8 + F,For F>3 the value may need fine tuning, especially if the ratio between clk_character and clk_sample is not 1:1.
															It should work in most cases, though.The user is encouraged to verify the setting in simulations for specific cases */
				.rxIlasAdnMiscDetails12c     = 0x0F000080,/* Register to hold details of ILAS length and other link parameters */
				.scr						 = 0x0,       /* Scrambling enabled */
				.coreInterruptMask		     = 0xd,       /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
				.tailbits					 = 0x0,       /* Control insertion of tail bits (0:Insert'0',1:Insert '1'2:Insert tail bit generated from an LFSR (or other construct),
				                                                                             that ensures low DC content,3:Invalid (Insert'0')) */
				.enableLinkTest 			 = 0x0,       /*Enable of link layer test (0:No link layer test performed,1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).
																                       2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2),3:No link layer test performed)*/
				.txFeatureDisable 		     = 0x10,      /* Disable features in the Rx module for compatibility with other devices */
			},
			/* orx Config settings */
			.orxConfig =
			{
				.lid						 = 0x0a418820,/* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
				.subClass					 = 1,         /* Subclass mode of operation (0, 1, or 2) */
				.cf 						 = 0,         /* number of control words per converter in 1 frame period */
				.cs 						 = 0,         /* number of control bits in each converter sample data */
				.f							 = 4,         /* F (number of bytes per JESD204 Frame) */
				.k							 = 32,        /* the number of frames in each multi frame. It is recommended to set it to 32 */
				.l							 = 1,         /* number of channels (l can be set to 1 ,2 or 4) */
				.n							 = 16,        /* converter resolution (n can be set to 16) */
				.m							 = 2,         /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
				.ntotal 					 = 16,        /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
				.s							 = 1,         /* number of samples sent by each converter per frame (s can be set to 1) */
				.hd 						 = 0x0,       /* High Density Format(0:High Density format not used,1:High Density format used) Currently not supported*/
				.txIlasDelay				 = 0xc,      /* Defines the delay from initial sample requests on the TX sample interface until the link layer requests the first frame from the transport layer.
															The resolution is clk_character cycles. A rough formula is:F=1 - delay must be set to 4,F=2 - delay must be set to 6,F=3 - delay must be set to 8,
															F>3 - delay must be 8 + F,For F>3 the value may need fine tuning, especially if the ratio between clk_character and clk_sample is not 1:1.
															It should work in most cases, though.The user is encouraged to verify the setting in simulations for specific cases */
				.rxIlasAdnMiscDetails12c     = 0x0F000080,/* Register to hold details of ILAS length and other link parameters */
				.scr						 = 0x0,       /* Scrambling enabled */
				.coreInterruptMask		     = 0xd,       /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
				.tailbits					 = 0x0,       /* Control insertion of tail bits (0:Insert'0',1:Insert '1',2:Insert tail bit generated from an LFSR (or other construct),
				                                                                     that ensures low DC content,3:Invalid (Insert'0')) */
				.enableLinkTest 			 = 0x0,       /*Enable of link layer test (0:No link layer test performed,1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).
																                       2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2),3:No link layer test performed)*/
				.txFeatureDisable 		     = 0x10,      /* Disable features in the Rx module for compatibility with other devices */
		   },
		},
    /* AGC settings */ 
    .agcSettings = 
    {
        .agcClkDiv                    = 1,          /*AGC clock select:0: No division 1: Divide by 2 2: Divide by 4 */
        .agcPeakWaitCounter           = 15,         /* AGC peak wait time. Valid range is from 0 to 15 (time=16* (agcPeakWaitTime+1) clocks) */
        .agcGainUpdateCounter         = 0x3FFF,     /* AGC gain update time (time = 128*( agcGainUpdateCounter+1))  */
        .agcPreventGain               = 0,          /* Prevent gain index from incrementing if peak thresholds are being exceeded:
                                                        0: power mode,check apd/hpd result 1: power mode ,not check apd/hpd result*/
        .agcGainControlMode           = 0,          /* Enable gain change based only on the signal peak threshold over-ranges:0: Peak 1: Power */

        /* AGC power settings */
        .agcPower =
        {
            .powerEnableMeasurement   = 0,      /* Enable the Rx power measurement block:0:Enable 1:disable */
            .powerInMux               = 1,      /* 00: hb1   01: bbdc  10: rfir  */
            .powerMeasurementDuration = 7,      /* Average power measurement duration = 2^(7+powerMeasurementDuration) clocks.
                                                    Valid range from 0 to 15 */
            .powerAttackHigStep       = 2,      /* AGC pmd output attack gain step.Valid range from 0 to 15 */
            .powerAttackLowStep       = 2,      /* AGC pmd output attack gain step.Valid range from 0 to 15 */
            .powerRecoveryHigStep     = 2,      /* AGC pmd output recovery gain step. Valid range from 0 to 15 */
            .powerRecoveryLowStep     = 2,      /* AGC pmd output recovery gain step. Valid range from 0 to 15 */
            .powerOverHigThresh       = 1456,   /* AGC pmd output over high threshold. Valid range from  0 to 8191 */
            .powerOverLowThresh       = 730,    /* AGC pmd output over low threshold. Valid range from  0 to 8191  */
            .powerUnderHigThresh      = 1456,   /* AGC pmd output under High threshold. Valid range from  0 to 8191 */
            .powerUnderLowThresh      = 730     /* AGC pmd output under low threshold. Valid range from  0 to 8191 */
        },

        /* AGC peak settings */
        .agcPeak =
        {
            .apdEnableMeasurement       = 1,    /* Enable the Rx apd measurement block. (0/1) */
            .apdAttackMode              = 0,    /* for agc mode, 0: periodically attack, 1: immediately attack */
            .apdMeasurementDuration     = 0,    /* APD update time(the time = 2^8*(apdMeasurementDuration+1) clocks) apdMeasurementDuration
                                                   Valid range 0-7 */
            .apdImmdOverNum             = 0,    /* mmediately attack mode, overrange */
            .apdPeriOverNum             = 7,    /* periodically attack mode, overrange */
            .apdUnderNum                = 450,  /* underrange valid range is 0 to 1023  */
            .apdLowTresh                = 16,   /* APD low thresh */
            .apdHighTresh               = 24,   /* APD high thresh */
            .apdGainStepAttack          = 2,    /* AGC APD peak detect attack gain step. Valid range from 0 to 15 */
            .apdGainStepRecovery        = 2,    /* AGC APD gain index step size for recovery. Valid range from 0 to 15 */

            .hb2EnableMeasurement       = 1,    /* Enable or disables the HB2 measurement block. */
            .hb2MeasurementDuration     = 0,    /* AGC HB2 update time(the time = 2^8*(hb2MeasurementDuration+1) clocks).
                                                   valid range 0-15. */
            .hb2RecoveryMode            = 0,    /* for agc mode, 0: periodically recovery, 1: immediately recovery */
            .hb2AttackMode              = 0,    /* for agc mode, 0: periodically attack, 1: immediately attack */
            .hb2OverThresh              = 4195, /* AGC HB2 output high threshold. Valid range from  0 to 8191 */
            .hb2UnderRangeHighThresh    = 2907, /* AGC HB2 output low threshold. Valid range from  0 to 8191 */
            .hb2UnderRangeMidThresh     = 0,    /* AGC HB2 output low threshold for 2nd interval for multiple time constant AGC mode.
                                                   Valid range from  0 to 8191. */
            .hb2UnderRangeLowThresh     = 0,    /* AGC HB2 output low threshold for 3rd interval for multiple time constant AGC mode.
                                                   Valid range from  0 to 8191. */
            .hb2OverloadThreshCnt       = 1,    /* Sets the number of actual overloads required to trigger the overload signal in agcGainUpdateCounter time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeHighThreshCnt = 1,    /* Sets the number of actual Under Range High threshold count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeMidThreshCnt  = 1,    /* Sets the number of actual Under Range Mid threshold  count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeLowThreshCnt  = 1,    /* Sets the number of actual Under Range Low threshold  count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2PeriExceededCnt         = 7,    /* Sets the number of actual periodically over Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2PeriUnderExceededCnt    = 450,  /* Sets the number of actual periodically Under Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdExceededCnt         = 1,    /* Sets the number of actual immediately over Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdUnderHigCnt         = 1,    /* Sets the number of actual immediately underHigh Range overange num.
                                                   Valid range from 1. */
            .hb2ImmdUnderLowCnt         = 1,    /* Sets the number of actual immediately underlow Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdUnderMidCnt         = 1,    /* Sets theimmediately underMid Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2GainStepHighRecovery    = 2,    /* AGC HB2 gain index step size. Valid range from  0 to 15. */
            .hb2GainStepLowRecovery     = 2,    /* AGC HB2 gain index step size, when the HB2 Low Overrange interval 2 triggers.
                                                   Valid range from  0 to 15. */
            .hb2GainStepMidRecovery     = 2,    /* AGC HB2 gain index step size, when the HB2 Low Overrange interval 3 triggers.
                                                   Valid range from  0 to 15. */
            .hb2GainStepAttack          = 2     /* AGC HB2 output attack gain step. Valid range from  0 to 15. */
        },
    },
};
#if 0
YxwInit_t Y9009Init_rx=
{
	.Loglevel = 1,		 	/*The DebugLevel:0-disable,1-err,2-Result,3-Status,4-Debug,Default is status*/
    .mode = 0, /*PLL mode:0-TX/RX/ORX=RFPLL;  1-TX=RFPLL,RX=RFPLL,ORX=AuxPLL;  2-TX1+ORX1ARX1D=RFPLL,TX2+ORX2ARX2D=AuxPLL;  3-TX1+RX1AORX1D=RFPLL,TX2+RX2AORX2D=RFPLL*/
	.initcaltype=RX_DC_OFFSET|RX_QEC_INIT,
	//.initcaltype=DC_OFFSET|RX_QEC_INIT|ORX_QEC_INIT|TX_QEC_INIT|TX_LO_LEAKAGE_INTERNAL,
    /*Rxsettings*/
    .rx=
    {
        .rxProfile=
        {
            .rxFir=
            {
                .gain_dB=0,                 /*The setting(in DB) for the gain block within the Rx FIR(18,12,6,0,-6,-12-18,-24) 0=FIX(16384)*/
                .numFirCoefs=24,            /*The number softapstobeusedintheRxFir(24,48,72)*/
                .coefs=&yxwRxFirCoefs[0],
            },
            .rxFirBypass=1,                 /*The RX FIR bypass enable (0,1)*/
            .rxFirDecimation=1,             /*The Rx FIR decimation (1,2,4) */
            .rhb1Decimation=2,              /*The Rx Halfband1 Decimation setting(1,2) */
            .rxOutputRate_kHz=245760,       /*The I/Q data rate at input to the Rx output data rate in kHz range 245.76/122.88/62.44Mhz*/
            .rfBandwidth_Hz=200000000,      /*The Rx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz=220000,     /*The Rx BBF 3dB corner in kHz  <1.1 * rxrfBandwidth_Hz/1000>*/
        },
        .rxGainCtrl=
        {
            .rxgainMode=YXW_MGC,             /*The MGC mode setting(0,1)*/
            .rxmgcMode=YXW_GPIOMODE,          /*The Ways for mgcMode(0,1) */
            .rx1GainIndex=127,              /*The rx1GainIndex setting(0,127) */
            .rx2GainIndex=127,              /*The rx2GainIndex setting(0,127)*/
            .rx1MaxGainIndex=127,           /*The Rx1 GainMax index 127*/
            .rx1MinGainIndex=0,             /*The Rx1 GainMix index 0;*/
            .rx2MaxGainIndex=127,           /*The Rx2 GainMax index 127*/
            .rx2MinGainIndex=0              /*The Rx2 GainMix index 0;*/
        },
        .rxGainCtrlPinInfo=
        {
            .incStep=1,                     /*Increment in gain index applied when the increment gain pin is pulsed(1-16) */
            .decStep=1,                     /*Decrement in gain index applied when the increment gain pin is pulsed(1-16) */
            .rx1GainCtrlPinInfo=
            {
                .rxGainIncPin=YXW_GPIO_00,   /*GPIO used for the rx1 Increment gain input*/
                .rxGainDecPin=YXW_GPIO_01,   /*GPIO used for the rx1 Decrement gain input*/
                .rxGainIncPinFunc=YXW_SEL_4, /*GPIO function select 0-7: make GPIO function rx1 gain Increment function */
                .rxGainDecPinFunc=YXW_SEL_4, /*GPIO function select 0-7:*make GPIO function rx1 gain Decrement function */
            },
            .rx2GainCtrlPinInfo=
            {
                .rxGainIncPin=YXW_GPIO_03,   /*GPIO used for the rx2 Increment gain input*/
                .rxGainDecPin=YXW_GPIO_04,   /*GPIO used for the rx2 Decrement gain input*/
                .rxGainIncPinFunc=YXW_SEL_4, /*GPIO function select 0-7: make GPIO function rx2 gain Increment function */
                .rxGainDecPinFunc=YXW_SEL_4, /*GPIO function select 0-7:*make GPIO function rx2 gain Decrement function */
            },
        },
        .rxSlicerPinInfo=
        {
            .EnableSlicer=YXW_RXSLICER_EN,   /*Default No Digital gain compensation slicer output*/
            .rx1SlicerPinInfo=
            {
                .rx1SlicerPin_0=YXW_GPIO_07, /*GPIO used for the rx1 Slicer pin output*/
                .rx1SlicerPin_1=YXW_GPIO_08, /*GPIO used for the rx1 Slicer pin output*/
                .rx1SlicerPin_2=YXW_GPIO_09, /*GPIO used for the rx1 Slicer pin output*/
            },
            .rx2SlicerPinInfo=
            {
                .rx2SlicerPin_0=YXW_GPIO_10, /*GPIO used for the rx2 Slicer pin output*/
                .rx2SlicerPin_1=YXW_GPIO_11, /*GPIO used for the rx2 Slicer pin output*/
                .rx2SlicerPin_2=YXW_GPIO_12, /*GPIO used for the rx2 Slicer pin output*/
            },
            .rxSlicerPinFunc=YXW_SEL_3,      /*GPIO function select 0-7: make GPIO function slicer function */
        },
        .rxChannels=YXW_RX1,               /*Select The enable rx channel*/
    },

    /*Tx settings*/
    .tx=
    {
        .txProfile=
        {
            .txFir=
            {
                .gain_dB=0,                 /* The setting(in DB) for the gain block within the Tx FIR(18,12,6,0,-6,-12-18,-24) 0=FIX(16384) */
                .numFirCoefs=40,            /* The numbers of taps to be used in the Tx Fir (20,40,80) */
                .coefs=&yxwTxFirCoefs[0],
            },
            .txFirBypass=1,                 /* The TX FIR bypass enable (0-enable,1-disable)*/
            .txFirInterpolation=1,          /* The Tx FIR filter interpolation setting(1-diable,2,4)     */
            .thb1Interpolation=2,           /* The Tx Halfband1 filter interpolation setting(1-disable,2-enable) */
            .thb2Interpolation=2,           /* The Tx Halfband2 filter interpolation settingg(1-disable,2-enable)*/
            .thb3Interpolation=2,           /* The Tx Halfband3 filter interpolation setting(1-disable,2-enable) */
            .txInputRate_kHz=245760,        /* The I/Q data rate at input to the TxFir rate in kHz range 491.52/245.76/122.88Mhz*/
            .rfBandwidth_Hz=200000000,      /* The Tx RF passband bandwidth for the profile */
            .txBbf3dBCorner_kHz=220000,     /*  The Tx BBF 3dB corner in kHz  <1.1 * txrfBandwidth_Hz/1000>*/
        },
		.txChannels = YXW_TXOFF,         /* Select The enable tx channel */
        .txGainCtrl=
        {
            .txgainMode=YXW_MGC,             /* The MGC mode setting(0)*/
            .txmgcMode=YXW_APIMODE,          /* The Ways for mgcMode(0) */
            .tx1GainIndex=0,                /* The tx1GainIndex setting(0,63) */
            .tx2GainIndex=0,                /* The tx2GainIndex setting(0,63) */
            .tx1MaxGainIndex=63,            /* The Tx1 GainMax index 63*/
            .tx1MinGainIndex=0,             /* The Tx1 GainMin index 0*/
            .tx2MaxGainIndex=63,            /* The Tx2 GainMax index 63*/
            .tx2MinGainIndex=0,             /* The Tx2 GainMin index 0*/
        },
    },

    /*ObsRx settings*/
    .obsRx=
    {
        .orxProfile=
        {
            .rxFir=
            {
                .gain_dB=-12,                 /* The setting(in DB) for the gain block within the Orx FIR(18,12,6,0,-6,-12-18,-24)  0=FIX(16384)*/
                .numFirCoefs=72,            /* The numbers of taps to be used in the Orx Fir 72 */
                .coefs=&yxwObsrxFirCoefs[0],
            },
            .orxFirBypass=0,                /* The ORX FIR bypass enable (0,1)*/
            .orxFirDecimation=2,            /* The ORx FIR decimation (1,2) */
            .orxOutputRate_kHz=245760,      /* The I/Q data rate at input to the ORX data rate in kHz range 491.52/245.76Mhz*/
            .rfBandwidth_Hz=200000000,      /* The ORx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz=220000,     /* The ORx BBF 3dB corner in kHz  <1.1 * orxrfBandwidth_Hz/1000>*/
        },
        .TableP=NULL, 	/*The OrxGain Table,Default is NULL(0.25db stepsize)*/
        .orxGainCtrl=
        {
            .orxgainMode=YXW_MGC,            /* The MGC mode setting(0,1)*/
            .orxmgcMode=YXW_APIMODE,         /* The Ways for mgcMode(0,1) */
            .orx1GainIndex=127,             /* The orx1GainIndex setting(0,127) */
            .orx2GainIndex=127,             /* The orx2GainIndex setting(0,127) */
            .orx1MaxGainIndex=127,          /* The Orx1 GainMax index 127*/
            .orx1MinGainIndex=0,            /* The Orx1 GainMax index 0*/
            .orx2MaxGainIndex=127,          /* The Orx2 GainMax index 127*/
            .orx2MinGainIndex=0,            /* The Orx2 GainMax index 0*/
        },
        .orxSlicerPinInfo = 
		{
			.EnableSlicer = YXW_ORXSLICER_EN,		/*Default No Digital gain compensation slicer output*/
			.orxSlicerPinInfo = 
			{
				.orxSlicerPin_0   = YXW_GPIO_13,  /*GPIO used for the orx1 Slicer pin output*/
				.orxSlicerPin_1   = YXW_GPIO_14,  /*GPIO used for the orx1 Slicer pin output*/
				.orxSlicerPin_2   = YXW_GPIO_15,  /*GPIO used for the orx1 Slicer pin output*/
			},			
			.orxSlicerPinFunc = YXW_SEL_3,  		 /*GPIO function select 0-7: make GPIO function slicer function */
		},
        .obsRxChannelsEnable=YXW_ORXOFF,     /* The desired ObsRx Channels to enable during initialization */
    },

    /*Digital Clock settings*/
	 .clocks =
	{
		.deviceClock_kHz    = 76800,  			    /* device reference clock frequency in kHz */
		.rfpll_frequency_Hz  = 2000000000ULL,  	    /* RFPll frequency in Hz */
		.auxpll_frequency_Hz  = 2000000000ULL,      /* AuxPll frequency in Hz */
		.rfPllUseExternalLo = 0,         			/* 1= Use external LO for RF PLL, 0 = use internal LO generation for RF PLL */
		.rfPllPhaseSyncMode = YXW_RFPLLMCS_NOSYNC  	/* RFPLL MCS (Phase sync) mode */
	},

    /*JESD204B settings*/
    .jesd204Settings=
    {
        .serdesClk=SERDES_CLK_10G,          /* JESD204B  Transmission Speed (10G:real speed 9.8304G, 5G:real speed 4.9152G)*/
        /*tx Config settings*/
        .txConfig=
        {
            .lid=0xA418820,                    /* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
            .subClass=1,                        /* Subclass mode of operation (0, 1, or 2) */
            .cf=0,                              /* number of control words per converter in 1 frame period */
            .cs=0,                              /* number of control bits in each converter sample data */
            .f=4,                               /* F (number of bytes per JESD204 Frame) */
            .k=32,                              /* the number of frames in each multi frame. It is recommended to set it to 32 */
            .l=1,                               /* number of channels (l can be set to 1 ,2 or 4) */
            .n=16,                              /* converter resolution (n can be set to 16) */
            .m=2,                               /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
            .ntotal=16,                         /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
            .s=1,                               /* number of samples sent by each converter per frame (s can be set to 1) */
            .hd=0x0,                            /* High Density Format(0:High Density format not used,1:High Density format used)*/
            .rxCoreInterruptMask=0xFFFD,        /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
            .rxIlasAdnMiscDetails12c=0xF000080,/* Register to hold details of ILAS length and RBD information */
            .scr=0x0,                           /* Scrambling enabled */
            .enableLinkTest=0x0,                /* Enable of link layer test(0:No link layer test performed.1:Transmit continuous sequence of /K28.5/characters (LINKTEST1). 2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2).3:No link layer test performed)*/
            .rxFeatureDisable=0x10,             /*Disable features in the Rx module for compatibility with other devices.*/
        },
        /*rx Config settings*/
        .rxConfig=
        {
            .lid=0xA418820,                    /* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
            .subClass=1,                        /* Subclass mode of operation (0, 1, or 2) */
            .cf=0,                              /* number of control words per converter in 1 frame period */
            .cs=0,                              /* number of control bits in each converter sample data */
            .f=4,                               /* F (number of bytes per JESD204 Frame) */
            .k=32,                              /* the number of frames in each multi frame. It is recommended to set it to 32 */
            .l=1,                               /* number of channels (l can be set to 1 ,2 or 4) */
            .n=16,                              /* converter resolution (n can be set to 16) */
            .m=2,                               /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
            .ntotal=16,                         /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
            .s=1,                               /* number of samples sent by each converter per frame (s can be set to 1) */
            .hd=0x0,                            /* High Density Format(0:High Density format not used,1:High Density format used) Currently not supported*/
            .txIlasDelay=0xC,                  /* Defines the delay from initial sample requests on the TX sample interface until the link layer requests the first frame from the transport layer. The resolution is clk_character cycles. A rough formula is:F=1 - delay must be set to 4,F=2- delay must be set to 6,F=3 - delay must be set to 8,F>3 - delay must be 8 + F,For F>3 the value may need fine tuning, especially if the ratio between clk_character and clk_sample is not 1:1. It should work in most cases, though.The user is encouraged to verify the setting in simulations for specific cases */
            .rxIlasAdnMiscDetails12c=0xF000080,/* Register to hold details of ILAS length and other link parameters */
            .scr=0x0,                           /* Scrambling enabled */
            .coreInterruptMask=0xD,             /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
            .tailbits=0x0,                      /* Control insertion of tail bits (0:Insert '0',1:Insert'1',2:Insert tail bit generated from an LFSR (or other construct), that ensures low DC content,3:Invalid (Insert '0')) */
            .enableLinkTest=0x0,                /*Enable of link layer test (0:No link layer test performed,1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2),3:No link layer test performed)*/
            .txFeatureDisable=0x10,             /* Disable features in the Rx module for compatibility with other devices */
        },
        /*orx Config settings*/
        .orxConfig=
        {
            .lid=0xA418820,                    /* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
            .subClass=1,                        /* Subclass mode of operation (0, 1, or 2) */
            .cf=0,                              /* number of control words per converter in 1 frame period */
            .cs=0,                              /* number of control bits in each converter sample data */
            .f=4,                               /* F (number of bytes per JESD204 Frame) */
            .k=32,                              /* the number of frames in each multi frame. It is recommended to set it to 32 */
            .l=1,                               /* number of channels (l can be set to 1 ,2 or 4) */
            .n=16,                              /* converter resolution (n can be set to 16) */
            .m=2,                               /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
            .ntotal=16,                         /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
            .s=1,                               /* number of samples sent by each converter per frame (s can be set to 1) */
            .hd=0x0,                            /* High Density Format(0:High Density format not used,1:High Density format used) Currently not supported*/
            .txIlasDelay=0xC,                  /* Defines the delay from initial sample requests on the TX sample interface until the link layer requests the first frame from the transport layer. The resolution is clk_character cycles. A rough formula is:F=1 - delay must be set to 4,F=2 - delay must be set to 6,F=3 - delay must be set to 8,F>3 - delay must be 8 + F,For F>3 the value may need fine tuning, especially if the ratio between clk_character and clk_sample is not 1:1. It should work in most cases, though.The user is encouraged to verify the setting in simulations for specific cases */
            .rxIlasAdnMiscDetails12c=0xF000080,/* Register to hold details of ILAS length and other link parameters */
            .scr=0x0,                           /* Scrambling enabled */
            .coreInterruptMask=0xD,             /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
            .tailbits=0x0,                      /* Control insertion of tail bits (0:Insert'0',1:Insert'1',2:Insert tail bit generated from an LFSR (or other construct), that ensures low DC content,3:Invalid (Insert'0')) */
            .enableLinkTest=0x0,                /* Enable of link layer test (0:No link layer test performed,1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2),3:No link layer test performed)*/
            .txFeatureDisable=0x10,             /* Disable features in the Rx module for compatibility with other devices */
        },
    }
	,
    /* AGC settings */ 
    .agcSettings = 
    {
        .agcClkDiv                    = 1,          /*AGC clock select:0: No division 1: Divide by 2 2: Divide by 4 */
        .agcPeakWaitCounter           = 15,         /* AGC peak wait time. Valid range is from 0 to 15 (time=16* (agcPeakWaitTime+1) clocks) */
        .agcGainUpdateCounter         = 0x3FFF,     /* AGC gain update time (time = 128*( agcGainUpdateCounter+1))  */
        .agcPreventGain               = 0,          /* Prevent gain index from incrementing if peak thresholds are being exceeded:
                                                        0: power mode,check apd/hpd result 1: power mode ,not check apd/hpd result*/
        .agcGainControlMode           = 0,          /* Enable gain change based only on the signal peak threshold over-ranges:0: Peak 1: Power */

        /* AGC power settings */
        .agcPower = 
        {
            .powerEnableMeasurement   = 0,      /* Enable the Rx power measurement block:0:Enable 1:disable */
            .powerInMux               = 1,      /* 00: hb1   01: bbdc  10: rfir  */
            .powerMeasurementDuration = 7,      /* Average power measurement duration = 2^(7+powerMeasurementDuration) clocks.
                                                    Valid range from 0 to 15 */
            .powerAttackHigStep       = 2,      /* AGC pmd output attack gain step.Valid range from 0 to 15 */
            .powerAttackLowStep       = 2,      /* AGC pmd output attack gain step.Valid range from 0 to 15 */
            .powerRecoveryHigStep     = 2,      /* AGC pmd output recovery gain step. Valid range from 0 to 15 */
            .powerRecoveryLowStep     = 2,      /* AGC pmd output recovery gain step. Valid range from 0 to 15 */
            .powerOverHigThresh       = 1456,   /* AGC pmd output over high threshold. Valid range from  0 to 8191 */
            .powerOverLowThresh       = 730,    /* AGC pmd output over low threshold. Valid range from  0 to 8191  */
            .powerUnderHigThresh      = 1456,   /* AGC pmd output under High threshold. Valid range from  0 to 8191 */
            .powerUnderLowThresh      = 730     /* AGC pmd output under low threshold. Valid range from  0 to 8191 */
        },

        /* AGC peak settings */
        .agcPeak = 
        {
            .apdEnableMeasurement       = 1,    /* Enable the Rx apd measurement block. (0/1) */
            .apdAttackMode              = 0,    /* for agc mode, 0: periodically attack, 1: immediately attack */
            .apdMeasurementDuration     = 0,    /* APD update time(the time = 2^8*(apdMeasurementDuration+1) clocks) apdMeasurementDuration
                                                   Valid range 0-7 */
            .apdImmdOverNum             = 0,    /* mmediately attack mode, overrange */
            .apdPeriOverNum             = 7,    /* periodically attack mode, overrange */
            .apdUnderNum                = 450,  /* underrange valid range is 0 to 1023  */
            .apdLowTresh                = 16,   /* APD low thresh */
            .apdHighTresh               = 24,   /* APD high thresh */
            .apdGainStepAttack          = 2,    /* AGC APD peak detect attack gain step. Valid range from 0 to 15 */
            .apdGainStepRecovery        = 2,    /* AGC APD gain index step size for recovery. Valid range from 0 to 15 */
            
            .hb2EnableMeasurement       = 1,    /* Enable or disables the HB2 measurement block. */
            .hb2MeasurementDuration     = 0,    /* AGC HB2 update time(the time = 2^8*(hb2MeasurementDuration+1) clocks).
                                                   valid range 0-15. */
            .hb2RecoveryMode            = 0,    /* for agc mode, 0: periodically recovery, 1: immediately recovery */
            .hb2AttackMode              = 0,    /* for agc mode, 0: periodically attack, 1: immediately attack */
            .hb2OverThresh              = 4195, /* AGC HB2 output high threshold. Valid range from  0 to 8191 */
            .hb2UnderRangeHighThresh    = 2907, /* AGC HB2 output low threshold. Valid range from  0 to 8191 */
            .hb2UnderRangeMidThresh     = 0,    /* AGC HB2 output low threshold for 2nd interval for multiple time constant AGC mode.
                                                   Valid range from  0 to 8191. */
            .hb2UnderRangeLowThresh     = 0,    /* AGC HB2 output low threshold for 3rd interval for multiple time constant AGC mode.
                                                   Valid range from  0 to 8191. */
            .hb2OverloadThreshCnt       = 1,    /* Sets the number of actual overloads required to trigger the overload signal in agcGainUpdateCounter time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeHighThreshCnt = 1,    /* Sets the number of actual Under Range High threshold count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeMidThreshCnt  = 1,    /* Sets the number of actual Under Range Mid threshold  count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeLowThreshCnt  = 1,    /* Sets the number of actual Under Range Low threshold  count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2PeriExceededCnt         = 7,    /* Sets the number of actual periodically over Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2PeriUnderExceededCnt    = 450,  /* Sets the number of actual periodically Under Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdExceededCnt         = 1,    /* Sets the number of actual immediately over Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdUnderHigCnt         = 1,    /* Sets the number of actual immediately underHigh Range overange num.
                                                   Valid range from 1. */
            .hb2ImmdUnderLowCnt         = 1,    /* Sets the number of actual immediately underlow Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdUnderMidCnt         = 1,    /* Sets theimmediately underMid Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2GainStepHighRecovery    = 2,    /* AGC HB2 gain index step size. Valid range from  0 to 15. */
            .hb2GainStepLowRecovery     = 2,    /* AGC HB2 gain index step size, when the HB2 Low Overrange interval 2 triggers.
                                                   Valid range from  0 to 15. */
            .hb2GainStepMidRecovery     = 2,    /* AGC HB2 gain index step size, when the HB2 Low Overrange interval 3 triggers.
                                                   Valid range from  0 to 15. */
            .hb2GainStepAttack          = 2     /* AGC HB2 output attack gain step. Valid range from  0 to 15. */
        }, 
    },    
};
#endif
YxwInit_t Y9009Init_tx=
{
	.Loglevel = 3,		 	/*The DebugLevel:0-disable,1-err,2-Result,3-Status,4-Debug,Default is status*/
    .mode = 0, /*PLL mode:0-TX/RX/ORX=RFPLL;  1-TX=RFPLL,RX=RFPLL,ORX=AuxPLL;  2-TX1+ORX1ARX1D=RFPLL,TX2+ORX2ARX2D=AuxPLL;  3-TX1+RX1AORX1D=RFPLL,TX2+RX2AORX2D=RFPLL*/
	.initcaltype=TX_QEC_INIT | TX_LO_LEAKAGE_INTERNAL,
	//.initcaltype=DC_OFFSET|RX_QEC_INIT|ORX_QEC_INIT|TX_QEC_INIT|TX_LO_LEAKAGE_INTERNAL,
    /*Rxsettings*/
    .rx=
    {
        .rxProfile=
        {
            .rxFir=
            {
                .gain_dB=-12,                 /*The setting(in DB) for the gain block within the Rx FIR(18,12,6,0,-6,-12-18,-24) 0=FIX(16384)*/
                .numFirCoefs=48,            /*The number softapstobeusedintheRxFir(24,48,72)*/
                .coefs=&yxwRxFirCoefs[0],
            },
            .rxFirBypass=1,                 /*The RX FIR bypass enable (0,1)*/
            .rxFirDecimation=1,             /*The Rx FIR decimation (1,2,4) */
            .rhb1Decimation=2,              /*The Rx Halfband1 Decimation setting(1,2) */
            .rxOutputRate_kHz=245760,       /*The I/Q data rate at input to the Rx output data rate in kHz range 245.76/122.88/62.44Mhz*/
            .rfBandwidth_Hz=200000000,      /*The Rx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz=220000,     /*The Rx BBF 3dB corner in kHz  <1.1 * rxrfBandwidth_Hz/1000>*/
        },
        .rxGainCtrl=
        {
            .rxgainMode=YXW_MGC,             /*The MGC mode setting(0,1)*/
            .rxmgcMode=YXW_APIMODE,          /*The Ways for mgcMode(0,1) */
            .rx1GainIndex=127,              /*The rx1GainIndex setting(0,127) */
            .rx2GainIndex=127,              /*The rx2GainIndex setting(0,127)*/
            .rx1MaxGainIndex=127,           /*The Rx1 GainMax index 127*/
            .rx1MinGainIndex=0,             /*The Rx1 GainMix index 0;*/
            .rx2MaxGainIndex=127,           /*The Rx2 GainMax index 127*/
            .rx2MinGainIndex=0              /*The Rx2 GainMix index 0;*/
        },
        .rxGainCtrlPinInfo=
        {
            .incStep=3,                     /*Increment in gain index applied when the increment gain pin is pulsed(1-16) */
            .decStep=3,                     /*Decrement in gain index applied when the increment gain pin is pulsed(1-16) */
            .rx1GainCtrlPinInfo=
            {
                .rxGainIncPin=YXW_GPIO_08,   /*GPIO used for the rx1 Increment gain input*/
                .rxGainDecPin=YXW_GPIO_09,   /*GPIO used for the rx1 Decrement gain input*/
                .rxGainIncPinFunc=YXW_SEL_1, /*GPIO function select 0-7: make GPIO function rx1 gain Increment function */
                .rxGainDecPinFunc=YXW_SEL_1, /*GPIO function select 0-7:*make GPIO function rx1 gain Decrement function */
            },
            .rx2GainCtrlPinInfo=
            {
                .rxGainIncPin=YXW_GPIO_10,   /*GPIO used for the rx2 Increment gain input*/
                .rxGainDecPin=YXW_GPIO_11,   /*GPIO used for the rx2 Decrement gain input*/
                .rxGainIncPinFunc=YXW_SEL_1, /*GPIO function select 0-7: make GPIO function rx2 gain Increment function */
                .rxGainDecPinFunc=YXW_SEL_1, /*GPIO function select 0-7:*make GPIO function rx2 gain Decrement function */
            },
        },
        .rxSlicerPinInfo=
        {
            .EnableSlicer=YXW_RXSLICER_EN,   /*Default No Digital gain compensation slicer output*/
            .rx1SlicerPinInfo=
            {
                .rx1SlicerPin_0=YXW_GPIO_07, /*GPIO used for the rx1 Slicer pin output*/
                .rx1SlicerPin_1=YXW_GPIO_08, /*GPIO used for the rx1 Slicer pin output*/
                .rx1SlicerPin_2=YXW_GPIO_09, /*GPIO used for the rx1 Slicer pin output*/
            },
            .rx2SlicerPinInfo=
            {
                .rx2SlicerPin_0=YXW_GPIO_10, /*GPIO used for the rx2 Slicer pin output*/
                .rx2SlicerPin_1=YXW_GPIO_11, /*GPIO used for the rx2 Slicer pin output*/
                .rx2SlicerPin_2=YXW_GPIO_12, /*GPIO used for the rx2 Slicer pin output*/
            },
            .rxSlicerPinFunc=YXW_SEL_3,      /*GPIO function select 0-7: make GPIO function slicer function */
        },
        .rxChannels=YXW_RXOFF,               /*Select The enable rx channel*/
    },

    /*Tx settings*/
    .tx=
    {
        .txProfile=
        {
            .txFir=
            {
                .gain_dB=0,                 /* The setting(in DB) for the gain block within the Tx FIR(18,12,6,0,-6,-12-18,-24) 0=FIX(16384) */
                .numFirCoefs=40,            /* The numbers of taps to be used in the Tx Fir (20,40,80) */
                .coefs=&yxwTxFirCoefs[0],
            },
            .txFirBypass=0,                 /* The TX FIR bypass enable (0-enable,1-disable)*/
            .txFirInterpolation=1,          /* The Tx FIR filter interpolation setting(1-diable,2,4)     */
            .thb1Interpolation=2,           /* The Tx Halfband1 filter interpolation setting(1-disable,2-enable) */
            .thb2Interpolation=2,           /* The Tx Halfband2 filter interpolation settingg(1-disable,2-enable)*/
            .thb3Interpolation=2,           /* The Tx Halfband3 filter interpolation setting(1-disable,2-enable) */
            .txInputRate_kHz=245760,        /* The I/Q data rate at input to the TxFir rate in kHz range 491.52/245.76/122.88Mhz*/
            .rfBandwidth_Hz=200000000,      /* The Tx RF passband bandwidth for the profile */
            .txBbf3dBCorner_kHz=220000,     /*  The Tx BBF 3dB corner in kHz  <1.1 * txrfBandwidth_Hz/1000>*/
        },
		.txChannels = YXW_TX1,         /* Select The enable tx channel */
        .txGainCtrl=
        {
            .txgainMode=YXW_MGC,             /* The MGC mode setting(0)*/
            .txmgcMode=YXW_APIMODE,          /* The Ways for mgcMode(0) */
            .tx1GainIndex=0,                /* The tx1GainIndex setting(0,63) */
            .tx2GainIndex=0,                /* The tx2GainIndex setting(0,63) */
            .tx1MaxGainIndex=63,            /* The Tx1 GainMax index 63*/
            .tx1MinGainIndex=0,             /* The Tx1 GainMin index 0*/
            .tx2MaxGainIndex=63,            /* The Tx2 GainMax index 63*/
            .tx2MinGainIndex=0,             /* The Tx2 GainMin index 0*/
        },
    },

    /*ObsRx settings*/
    .obsRx=
    {
        .orxProfile=
        {
            .rxFir=
            {
                .gain_dB=-6,                 /* The setting(in DB) for the gain block within the Orx FIR(18,12,6,0,-6,-12-18,-24)  0=FIX(16384)*/
                .numFirCoefs=72,            /* The numbers of taps to be used in the Orx Fir 72 */
                .coefs=&yxwObsrxFirCoefs[0],
            },
            .orxFirBypass=0,                /* The ORX FIR bypass enable (0,1)*/
            .orxFirDecimation=2,            /* The ORx FIR decimation (1,2) */
            .orxOutputRate_kHz=245760,      /* The I/Q data rate at input to the ORX data rate in kHz range 491.52/245.76Mhz*/
            .rfBandwidth_Hz=200000000,      /* The ORx RF passband bandwidth for the profile */
            .rxBbf3dBCorner_kHz=220000,     /* The ORx BBF 3dB corner in kHz  <1.1 * orxrfBandwidth_Hz/1000>*/
        },
        .TableP=NULL, 	/*The OrxGain Table,Default is NULL(0.25db stepsize)*/
        .orxGainCtrl=
        {
            .orxgainMode=YXW_MGC,            /* The MGC mode setting(0,1)*/
            .orxmgcMode=YXW_APIMODE,         /* The Ways for mgcMode(0,1) */
            .orx1GainIndex=127,             /* The orx1GainIndex setting(0,127) */
            .orx2GainIndex=127,             /* The orx2GainIndex setting(0,127) */
            .orx1MaxGainIndex=127,          /* The Orx1 GainMax index 127*/
            .orx1MinGainIndex=0,            /* The Orx1 GainMax index 0*/
            .orx2MaxGainIndex=127,          /* The Orx2 GainMax index 127*/
            .orx2MinGainIndex=0,            /* The Orx2 GainMax index 0*/
        },
        .orxSlicerPinInfo = 
		{
			.EnableSlicer = YXW_ORXSLICER_EN,		/*Default No Digital gain compensation slicer output*/
			.orxSlicerPinInfo = 
			{
				.orxSlicerPin_0   = YXW_GPIO_13,  /*GPIO used for the orx1 Slicer pin output*/
				.orxSlicerPin_1   = YXW_GPIO_14,  /*GPIO used for the orx1 Slicer pin output*/
				.orxSlicerPin_2   = YXW_GPIO_15,  /*GPIO used for the orx1 Slicer pin output*/
			},			
			.orxSlicerPinFunc = YXW_SEL_3,  		 /*GPIO function select 0-7: make GPIO function slicer function */
		},
        .obsRxChannelsEnable=YXW_ORXOFF,     /* The desired ObsRx Channels to enable during initialization */
    },

    /*Digital Clock settings*/
	 .clocks =
	{
		.deviceClock_kHz    = 76800,  			    /* device reference clock frequency in kHz */
		.rfpll_frequency_Hz  = 2000000000ULL,  	    /* RFPll frequency in Hz */
		.auxpll_frequency_Hz  = 2000000000ULL,      /* AuxPll frequency in Hz */
		.rfPllUseExternalLo = 0,         			/* 1= Use external LO for RF PLL, 0 = use internal LO generation for RF PLL */
		.rfPllPhaseSyncMode = YXW_RFPLLMCS_NOSYNC  	/* RFPLL MCS (Phase sync) mode */
	},

    /*JESD204B settings*/
    .jesd204Settings=
    {
        .serdesClk=SERDES_CLK_10G,          /* JESD204B  Transmission Speed (10G:real speed 9.8304G, 5G:real speed 4.9152G)*/
        /*tx Config settings*/
        .txConfig=
        {
            .lid=0xA418820,                    /* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
            .subClass=1,                        /* Subclass mode of operation (0, 1, or 2) */
            .cf=0,                              /* number of control words per converter in 1 frame period */
            .cs=0,                              /* number of control bits in each converter sample data */
            .f=4,                               /* F (number of bytes per JESD204 Frame) */
            .k=32,                              /* the number of frames in each multi frame. It is recommended to set it to 32 */
            .l=1,                               /* number of channels (l can be set to 1 ,2 or 4) */
            .n=16,                              /* converter resolution (n can be set to 16) */
            .m=2,                               /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
            .ntotal=16,                         /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
            .s=1,                               /* number of samples sent by each converter per frame (s can be set to 1) */
            .hd=0x0,                            /* High Density Format(0:High Density format not used,1:High Density format used)*/
            .rxCoreInterruptMask=0xFFFD,        /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
            .rxIlasAdnMiscDetails12c=0xF000080,/* Register to hold details of ILAS length and RBD information */
            .scr=0x0,                           /* Scrambling enabled */
            .enableLinkTest=0x0,                /* Enable of link layer test(0:No link layer test performed.1:Transmit continuous sequence of /K28.5/characters (LINKTEST1). 2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2).3:No link layer test performed)*/
            .rxFeatureDisable=0x10,             /*Disable features in the Rx module for compatibility with other devices.*/
        },
        /*rx Config settings*/
        .rxConfig=
        {
            .lid=0xA418820,                    /* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
            .subClass=1,                        /* Subclass mode of operation (0, 1, or 2) */
            .cf=0,                              /* number of control words per converter in 1 frame period */
            .cs=0,                              /* number of control bits in each converter sample data */
            .f=4,                               /* F (number of bytes per JESD204 Frame) */
            .k=32,                              /* the number of frames in each multi frame. It is recommended to set it to 32 */
            .l=1,                               /* number of channels (l can be set to 1 ,2 or 4) */
            .n=16,                              /* converter resolution (n can be set to 16) */
            .m=2,                               /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
            .ntotal=16,                         /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
            .s=1,                               /* number of samples sent by each converter per frame (s can be set to 1) */
            .hd=0x0,                            /* High Density Format(0:High Density format not used,1:High Density format used) Currently not supported*/
            .txIlasDelay=0xC,                  /* Defines the delay from initial sample requests on the TX sample interface until the link layer requests the first frame from the transport layer. The resolution is clk_character cycles. A rough formula is:F=1 - delay must be set to 4,F=2- delay must be set to 6,F=3 - delay must be set to 8,F>3 - delay must be 8 + F,For F>3 the value may need fine tuning, especially if the ratio between clk_character and clk_sample is not 1:1. It should work in most cases, though.The user is encouraged to verify the setting in simulations for specific cases */
            .rxIlasAdnMiscDetails12c=0xF000080,/* Register to hold details of ILAS length and other link parameters */
            .scr=0x0,                           /* Scrambling enabled */
            .coreInterruptMask=0xD,             /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
            .tailbits=0x0,                      /* Control insertion of tail bits (0:Insert '0',1:Insert'1',2:Insert tail bit generated from an LFSR (or other construct), that ensures low DC content,3:Invalid (Insert '0')) */
            .enableLinkTest=0x0,                /*Enable of link layer test (0:No link layer test performed,1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2),3:No link layer test performed)*/
            .txFeatureDisable=0x10,             /* Disable features in the Rx module for compatibility with other devices */
        },
        /*orx Config settings*/
        .orxConfig=
        {
            .lid=0xA418820,                    /* Lane ID for Lane0 to 3 (LID0 [0:4], LID1 [5:9], LID2 [10:14], LID3 [15:19])*/
            .subClass=1,                        /* Subclass mode of operation (0, 1, or 2) */
            .cf=0,                              /* number of control words per converter in 1 frame period */
            .cs=0,                              /* number of control bits in each converter sample data */
            .f=4,                               /* F (number of bytes per JESD204 Frame) */
            .k=32,                              /* the number of frames in each multi frame. It is recommended to set it to 32 */
            .l=1,                               /* number of channels (l can be set to 1 ,2 or 4) */
            .n=16,                              /* converter resolution (n can be set to 16) */
            .m=2,                               /* Number of converters (ADC) (M can be set to 0, 2 or 4) */
            .ntotal=16,                         /* bit width of each sample data (n' can be set to 16), N'= n + control and pseudo data bits */
            .s=1,                               /* number of samples sent by each converter per frame (s can be set to 1) */
            .hd=0x0,                            /* High Density Format(0:High Density format not used,1:High Density format used) Currently not supported*/
            .txIlasDelay=0xC,                  /* Defines the delay from initial sample requests on the TX sample interface until the link layer requests the first frame from the transport layer. The resolution is clk_character cycles. A rough formula is:F=1 - delay must be set to 4,F=2 - delay must be set to 6,F=3 - delay must be set to 8,F>3 - delay must be 8 + F,For F>3 the value may need fine tuning, especially if the ratio between clk_character and clk_sample is not 1:1. It should work in most cases, though.The user is encouraged to verify the setting in simulations for specific cases */
            .rxIlasAdnMiscDetails12c=0xF000080,/* Register to hold details of ILAS length and other link parameters */
            .scr=0x0,                           /* Scrambling enabled */
            .coreInterruptMask=0xD,             /* Register to hold interrupt mask bits (0:indicates the interrupt source is unmasked.1: indicates the interrupt source is masked)*/
            .tailbits=0x0,                      /* Control insertion of tail bits (0:Insert'0',1:Insert'1',2:Insert tail bit generated from an LFSR (or other construct), that ensures low DC content,3:Invalid (Insert'0')) */
            .enableLinkTest=0x0,                /* Enable of link layer test (0:No link layer test performed,1:Transmit continuous sequence of /K28.5/characters (LINKTEST1).2:Transmit code group sequence, followed by repeated lane alignment sequence (LINKTEST2),3:No link layer test performed)*/
            .txFeatureDisable=0x10,             /* Disable features in the Rx module for compatibility with other devices */
        },
    }
	,
    /* AGC settings */ 
    .agcSettings = 
    {
        .agcClkDiv                    = 1,          /*AGC clock select:0: No division 1: Divide by 2 2: Divide by 4 */
        .agcPeakWaitCounter           = 15,         /* AGC peak wait time. Valid range is from 0 to 15 (time=16* (agcPeakWaitTime+1) clocks) */
        .agcGainUpdateCounter         = 0x3FFF,     /* AGC gain update time (time = 128*( agcGainUpdateCounter+1))  */
        .agcPreventGain               = 0,          /* Prevent gain index from incrementing if peak thresholds are being exceeded:
                                                        0: power mode,check apd/hpd result 1: power mode ,not check apd/hpd result*/
        .agcGainControlMode           = 0,          /* Enable gain change based only on the signal peak threshold over-ranges:0: Peak 1: Power */

        /* AGC power settings */
        .agcPower = 
        {
            .powerEnableMeasurement   = 0,      /* Enable the Rx power measurement block:0:Enable 1:disable */
            .powerInMux               = 1,      /* 00: hb1   01: bbdc  10: rfir  */
            .powerMeasurementDuration = 7,      /* Average power measurement duration = 2^(7+powerMeasurementDuration) clocks.
                                                    Valid range from 0 to 15 */
            .powerAttackHigStep       = 2,      /* AGC pmd output attack gain step.Valid range from 0 to 15 */
            .powerAttackLowStep       = 2,      /* AGC pmd output attack gain step.Valid range from 0 to 15 */
            .powerRecoveryHigStep     = 2,      /* AGC pmd output recovery gain step. Valid range from 0 to 15 */
            .powerRecoveryLowStep     = 2,      /* AGC pmd output recovery gain step. Valid range from 0 to 15 */
            .powerOverHigThresh       = 1456,   /* AGC pmd output over high threshold. Valid range from  0 to 8191 */
            .powerOverLowThresh       = 730,    /* AGC pmd output over low threshold. Valid range from  0 to 8191  */
            .powerUnderHigThresh      = 1456,   /* AGC pmd output under High threshold. Valid range from  0 to 8191 */
            .powerUnderLowThresh      = 730     /* AGC pmd output under low threshold. Valid range from  0 to 8191 */
        },

        /* AGC peak settings */
        .agcPeak = 
        {
            .apdEnableMeasurement       = 1,    /* Enable the Rx apd measurement block. (0/1) */
            .apdAttackMode              = 0,    /* for agc mode, 0: periodically attack, 1: immediately attack */
            .apdMeasurementDuration     = 0,    /* APD update time(the time = 2^8*(apdMeasurementDuration+1) clocks) apdMeasurementDuration
                                                   Valid range 0-7 */
            .apdImmdOverNum             = 0,    /* mmediately attack mode, overrange */
            .apdPeriOverNum             = 7,    /* periodically attack mode, overrange */
            .apdUnderNum                = 450,  /* underrange valid range is 0 to 1023  */
            .apdLowTresh                = 16,   /* APD low thresh */
            .apdHighTresh               = 24,   /* APD high thresh */
            .apdGainStepAttack          = 2,    /* AGC APD peak detect attack gain step. Valid range from 0 to 15 */
            .apdGainStepRecovery        = 2,    /* AGC APD gain index step size for recovery. Valid range from 0 to 15 */
            
            .hb2EnableMeasurement       = 1,    /* Enable or disables the HB2 measurement block. */
            .hb2MeasurementDuration     = 0,    /* AGC HB2 update time(the time = 2^8*(hb2MeasurementDuration+1) clocks).
                                                   valid range 0-15. */
            .hb2RecoveryMode            = 0,    /* for agc mode, 0: periodically recovery, 1: immediately recovery */
            .hb2AttackMode              = 0,    /* for agc mode, 0: periodically attack, 1: immediately attack */
            .hb2OverThresh              = 4195, /* AGC HB2 output high threshold. Valid range from  0 to 8191 */
            .hb2UnderRangeHighThresh    = 2907, /* AGC HB2 output low threshold. Valid range from  0 to 8191 */
            .hb2UnderRangeMidThresh     = 0,    /* AGC HB2 output low threshold for 2nd interval for multiple time constant AGC mode.
                                                   Valid range from  0 to 8191. */
            .hb2UnderRangeLowThresh     = 0,    /* AGC HB2 output low threshold for 3rd interval for multiple time constant AGC mode.
                                                   Valid range from  0 to 8191. */
            .hb2OverloadThreshCnt       = 1,    /* Sets the number of actual overloads required to trigger the overload signal in agcGainUpdateCounter time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeHighThreshCnt = 1,    /* Sets the number of actual Under Range High threshold count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeMidThreshCnt  = 1,    /* Sets the number of actual Under Range Mid threshold  count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2UnderRangeLowThreshCnt  = 1,    /* Sets the number of actual Under Range Low threshold  count to trigger the overload signal in hpd cnt time.
                                                   Valid range from 1 to 1023. */
            .hb2PeriExceededCnt         = 7,    /* Sets the number of actual periodically over Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2PeriUnderExceededCnt    = 450,  /* Sets the number of actual periodically Under Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdExceededCnt         = 1,    /* Sets the number of actual immediately over Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdUnderHigCnt         = 1,    /* Sets the number of actual immediately underHigh Range overange num.
                                                   Valid range from 1. */
            .hb2ImmdUnderLowCnt         = 1,    /* Sets the number of actual immediately underlow Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2ImmdUnderMidCnt         = 1,    /* Sets theimmediately underMid Range overange num.
                                                   Valid range from 1 to 1023. */
            .hb2GainStepHighRecovery    = 2,    /* AGC HB2 gain index step size. Valid range from  0 to 15. */
            .hb2GainStepLowRecovery     = 2,    /* AGC HB2 gain index step size, when the HB2 Low Overrange interval 2 triggers.
                                                   Valid range from  0 to 15. */
            .hb2GainStepMidRecovery     = 2,    /* AGC HB2 gain index step size, when the HB2 Low Overrange interval 3 triggers.
                                                   Valid range from  0 to 15. */
            .hb2GainStepAttack          = 2     /* AGC HB2 output attack gain step. Valid range from  0 to 15. */
        }, 
    },    
};

