
#ifndef	__DP_RFSAPR_ETHERNET_COMM_H__
#define __DP_RFSAPR_ETHERNET_COMM_H__


#include "commandstructs.h"
#include "dp_types.h"


/*! General Commands of RFSAPR communication */
#define RFSAPR_TERMINATE_CLIENT		999
#define	RFSAPR_CMD_RFSAPR_DISCONNECT		999		/*	(CL->EQS) To Disconnect the RFSAPR Communication */

/**********************************************************************************************************/

#define	RFSAPR_MAX_TCP_STACK_SIZE			(1*(1024*1024)) 

#define	RFSAPR_MAX_TCPMSG_SIZE				(12 * (1024 * 1024)) //12MB
#define	RFSAPR_TCPMSG_BUFFER_SIZE			(RFSAPR_MAX_TCPMSG_SIZE + 1024)

#define	DP_RFSAPR_HEADER_SIZE				4
#define	DP_RFSAPR_MSGLEN_SIZE				sizeof(unsigned long)
#define	DP_RFSAPR_FOOTER_SIZE				4
#define	DP_RFSAPR_MSGIDS_SIZE				4
#define	DP_RFSAPR_MSGSTS_SIZE				2
#define	DP_RFSAPR_MSG_RESERVED_SIZE		2
										
#define DP_RFSAPR_RX_HEADER_STATE			0
#define DP_RFSAPR_RX_MSGSTS_STATE			1
#define DP_RFSAPR_RX_MSGLEN_STATE			2
#define DP_RFSAPR_RX_MSGID_STATE			3
#define DP_RFSAPR_RX_MSGDATA_STATE			4
#define DP_RFSAPR_RX_FOOTER_STATE			5
										
#define DP_RFSAPR_RX_BYTE_COUNT				1
										
#define	DP_RFSAPR_CMD_RECEIVED				1
#define	DP_RFSAPR_CMD_NOTRECED				0
								
#define DP_RFSAPR_DEFAULT_PORT				10030

#define RFSAPR_GOTO_LISTEN_STATE	-1000


/************ Macros for settings socket options for ethernet disconnection detection *********************/

#define DP_RFSAPR_KEEPALIVE_TIME						10
#define DP_RFSAPR_KEEPALIVE_INTVL						1
#define DP_RFSAPR_KEEPALIVE_COUNT						5
#define DP_RFSAPR_TCP_TIMEOUT_VALUE_MS					10000
#define TCP_USER_TIMEOUT							18 /* Need to define only if socket.h does not have this macro */



///////////////////////////////* Communication packet structure for the Ethernet *///////////////////////////////////


typedef union
{
	
	//structures for the Group 1 (ARINC_429) 		
	S_RFSAPR_CMD_ARINC429_TX_CONFIGURE			m_SCmdARINC429TxConfig;
	S_RFSAPR_CMD_ARINC429_RX_CONFIGURE			m_SCmdARINC429RxConfig;
	S_RFSAPR_CMD_ARINC429_RX_DATA_TO_HOST		m_SCmdARINC429HostData;
	S_RFSAPR_CMD_ARINC429_START_RX_DATA		m_SCmdARINC429Rxdata;
	S_RFSAPR_CMD_ARINC429_LABEL_CONFIG			m_SCmdARINC429LabelConfig;
	S_RFSAPR_CMD_ARINC429_START_TX_DATA		m_SCmdARINC429TxData;
	S_RFSAPR_CMD_ARINC429_TX_DATA_FRM_HOST		m_SCmdARINC429DataFrmHost;
		
	///structure for the Group 2 ( 1553B) 	
	S_RFSAPR_CMD_1553B_BC_CONFIG				m_SCmd1553bBCConfig;
	S_RFSAPR_CMD_1553B_RT_CONFIG				m_SCmd1553BRTConfig;
	S_RFSAPR_CMD_1553B_MT_CONFIG				m_SCmd1553bMTConfig;
	S_RFSAPR_CMD_1553B_RT_COMMON				m_SCmd1553bRTCmn;
	S_RFSAPR_CMD_1553B_RT_RCV_MSGS				m_SCmd1553bRTRcvMsgs;
	S_RFSAPR_CMD_1553B_MT_START_MONITOR		m_SCmd1553BMTStart;
	S_RFSAPR_CMD_1553B_BC_MESSAGE_AND_FRAME	m_SCmd1553bBCFrmMsgs;//new
	S_RFSAPR_CMD_1553B_BC_START_STOP			m_SCmd1553bBCStrtStop;//new	
	S_RFSAPR_CMD_1553B_BC_START_RECEIVE		m_SCmd1553bBCStrtRcv;
	S_RFSAPR_CMD_STOP_MT						m_SCmd1553bMTStop;
	S_RFSAPR_CMD_1553B_TERM_CLOSE				m_SCmd1553BTermClose;


	S_RFSAPR_CMD_MT_READ_DATA					m_SCmd1553BMTReadData;

	/*** for QT test **/
	S_RFSAPR_CMD_QT_1553B_BC_RT_DATA_OUT		m_SCmd1553BBCtoRTQTtest;
	
	// Structure for the Group 4 (Flash )
	S_RFSAPR_CMD_FLASH_DATA_WRITE				m_SCmdFlashDatawrite;
	S_RFSAPR_CMD_FLASH_DATA_READ				m_SCmdFlashDataRead;
	S_RFSAPR_CMD_FLASH_PAGE_WRITE				m_SCmdFlashPageWrite;
	S_RFSAPR_CMD_FLASH_PAGE_READ				m_SCmdFlashPageRead;
	S_RFSAPR_CMD_FLASH_SECTOR_ERASE			m_SCmdFlashSectorErase;
	S_RFSAPR_CMD_FLASH_CHIP_ERASE				m_SCmdFlashChipErase;
	S_RFSAPR_CMD_FLASH_AUDIO_PLAY_CONFIG		m_SCmdFlashAudioPlay;
	
	// Structure for Group 5 (FPGA )	
	S_RFSAPR_CMD_FPGA_DATA_WRITE				m_SCmdFPGAdataWrite;
	S_RFSAPR_CMD_REG_DATA_WRITE				m_SCmdRegDataWrite;
	S_RFSAPR_CMD_REG_DATA_READ					m_SCmdRegDataRead;

	S_CMD_FPGA_READ					S_Cmd_FPGARead;
	S_CMD_FPGA_WRITE				S_Cmd_FPGAWrite;

/** for 1013 */
	S_CMD_NB_FPGA_READ				S_Cmd_NBFPGARead;
	S_CMD_NB_FPGA_WRITE				S_Cmd_NBFPGAWrite;
	
	//SATA
	S_RFSAPR_CMD_PERFORM_SATADOM_WRITE_TEST	m_SCmdSATAWriteTst;
	S_RFSAPR_CMD_PERFORM_SATADOM_READ_TEST		m_SCmdSATARDTest;
	//USB
	S_RFSAPR_CMD_PERFORM_USB_WRITE_TEST		m_SCmdUSBWRTst;
	S_RFSAPR_CMD_PERFORM_USB_READ_TEST			m_SCmdUSBRDTst;
	//UART	
	S_RFSAPR_CMD_UART_START_RECEIVING			m_SCmdUARTStrtRcv;
	S_RFSAPR_CMD_UART_TX_CONFIG				m_SCmdUARTTxConfig;
	S_RFSAPR_CMD_UART_RX_CONFIG				m_SCmdUARTRxConfig;
	S_RFSAPR_CMD_UART_TRANSMIT_DATA			m_SCmdUARTTxData;
	S_RFSAPR_CMD_UART_SEND_DATA_TO_HOST		m_SCmdUARTDatatoHost;
	S_RFSAPR_CMD_UART_SEND_DATA_FRM_HOST		m_SCmdUARTDataFrmHost;
	
	/*ADC FFT*/
	S_CMD_FFT_DATA_AQU						S_Cmd_FFT_DataAqusition;

	/*Added for WB ADC FIFO Readng */
	S_CMD_ADC_FIFO_DATA_AQU					S_Cmd_Read_ADC_FIFO_Data;

	S_CMD_LOG_ADC_FIFO_DATA_AQU				S_Cmd_Read_Log_ADC_FIFO_Data;

	S_CMD_LOG_ADC_THRESHOLD_DATA_AQU		S_Cmd_Read_Log_ADC_THRESHOLD_Data;

//	S_CMD_RF_LOG_ADC_FIFO_DATA_AQU			S_Cmd_Read_RF_Log_ADC_FIFO_Data;
	S_RFSAPR_CMD_READ_ALL_BAND_DATA			S_Cmd_Read_All_Band_Data;

	/*Added for NB ADC FIFO Readng */
	S_CMD_NB_ADC_FIFO_DATA_AQU				S_Cmd_Read_NB_ADC_FIFO_Data;

	S_RFSAPR_CMD_READ_NB_VOLTAGE			S_Cmd_Read_NB_Voltage;	

	S_CMD_NB_FFT_DATA_AQU					S_Cmd_Read_NB_ADC_Data_Acq;

/**Board Status */
	S_CMD_BOARD_DET_STATUS					S_Cmd_Read_Brd_Sts;

	/*ADC PLL Config*/
	S_CMD_NB_DRX_LMK_PLL_CONFIG				m_SCmdNBDRxLMKPLLConfig;
	S_CMD_NB_DRX_LMX_PLL_CONFIG				m_SCmdNBDRxLMXPLLConfig;
	
	S_CMD_NB_DRX_RF_ADC_CONFIG				m_SCmdNBDRxRFADCConfig;
	S_CMD_NB_DRX_IF_ADC_CONFIG				m_SCmdNBDRxIFADCConfig;

	S_RFSAPR_RF_GAIN_CALIB_TABLE		S_Cmd_RFSAPRGainTable;
	
	S_RFSAPR_HW_TEMP_CALIB_DATA					m_S_Cmd_RFSAPRTempCalib;
	S_RFSAPR_CMD_READ_WBRF_TEMP_CALIB_DATA		m_S_Cmd_ReadWBRFCalib;		

	/*** Added for LOG ADC */

	S_RFSAPR_LOG_ADC_MEASD_TABLE			S_CmdLogADCConstSlopeTab;
	S_RFSAPR_CMD_READ_LOG_ADC_CALIB_DATA	S_CmdReadLogADCData;	

	S_RFSAPR_HW_LOG_TEMP_CALIB_DATA			S_CmdHwLogADCTempData;

	/********************/


	/******** Added from kaushik *****/

	S_RFSAPR_GAIN_SW_CALIB_DATA						S_Cmd_RFSAPR_GainSwCalibData;

	/*********************/
	S_DP_SET_RFSAPR_SYSTEM_TIME				S_Cmd_SetRFSAPR_System_Time;

	S_DP_VPX_1502_CONFIG_LOG_ADC			S_Cmd_FPGA_LogADCWrite;
	S_DP_VPX_1502_CONFIG_ADC				S_Cmd_FPGA_ADCWrite;	
	S_DP_VPX_1502_CONFIG_TRF_PLL			S_Cmd_FPGA_TRF_PLLWrite;
	S_DP_VPX_1502_CONFIG_LMK_PLL			S_Cmd_FPGA_LMK_PLLWrite;
	S_FPGA_SEQUENCE_WRITE					S_Cmd_FPGA_SequenceWrite;

/* Added for the 1013 */
	S_DP_VPX_1013_CONFIG_LMX_PLL			S_Cmd_NB_FPGA_LMX_PLLWrite;
	S_DP_VPX_1013_CONFIG_LMK_PLL			S_Cmd_NB_FPGA_LMK_PLLWrite;
	S_DP_VPX_1013_CONFIG_ADC				S_Cmd_NB_FPGA_ADCWrite;	
	S_FPGA_SEQUENCE_WRITE					S_Cmd_NB_FPGA_SequenceWrite;

	S_CMD_NB_ADC_FIFO_RESET					S_Cmd_Read_NB_ADC_FIFO_Reset;
/*End */

/*** Audio Codec related ***********/
	S_RFSAPR_CMD_READ_AUDIO_DATA					S_Cmd_Read_Audio_Codec_Data;
	S_RFSAPR_CMD_ERASE_AUDIO_DATA					S_Cmd_Erase_Audio_Codec_Data;
	S_RFSAPR_CMD_PLAY_FLASH_AUDIO					S_Cmd_Play_Flash_Audio;
	S_RFSAPR_CMD_CONFIGURE_AUDIO_VOL				S_Cmd_Config_Audio_vol;
	S_RFSAPR_CMD_AUDIO_MUTE_UNMUTE					S_Cmd_Config_Audio_MuteUnmute;
	S_RFSAPR_CMD_AUDIO_CODEC_DEFAULT_CONFIG_FLASH	S_Cmd_Store_Default_Codec_Config;
	S_RFSAPR_CMD_CONFIG_AUDIO_CODEC					S_Cmd_Config_Audio_Codec;
	S_RFSAPR_CMD_STORE_AUDIO_DATA					S_Cmd_Store_Audio_Data;
	S_RFSAPR_CMD_WRITE_AUDIO_DATA					S_Cmd_Write_Audio_Data;	

	S_RFSAPR_CMD_WRITE_CODEC_PLL_CONFIG				S_Cmd_Store_Codec_PLL_Config;

/********** NB Flash Related ******/
	S_RFSAPR_CMD_NB_FLASH_PAGE_FILE_WRITE			S_Cmd_NB_Flash_Pg_Write;
	S_RFSAPR_CMD_NB_FLASH_PAGE_FILE_READ			S_Cmd_NB_Flash_Pg_Read;
	S_RFSAPR_CMD_NB_FLASH_ERASE						S_Cmd_NB_Flash_Erase;

/** WB Flahh related */
	S_RFSAPR_CMD_FLASH_SINGLE_WRITE				S_Cmd_WB_Flash_Single_Write;	
	S_RFSAPR_CMD_FLASH_SINGLE_READ				S_Cmd_WB_Flash_Single_Read;
	S_RFSAPR_CMD_FLASH_PAGE_FILE_WRITE			S_Cmd_WB_Flash_Pg_Write;
	S_RFSAPR_CMD_FLASH_PAGE_FILE_READ			S_Cmd_WB_Flash_Pg_Read;
	S_RFSAPR_CMD_FLASH_ERASE						S_Cmd_WB_Flash_Erase;


	/**************	DRX	Aurora related *********************/
	
	S_RFSAPR_CMD_DRX_AURORA_GT_CORE_RESET			S_Cmd_Aurora_GT_core_Rst;
	S_RFSAPR_CMD_DRX_AURORA_FIFO_RESET				S_Cmd_Aurora_FIFO_Rst;;
	S_RFSAPR_CMD_DRX_AURORA_READ_LINK_STATUS		S_Cmd_Aurora_Rd_Link_Sts;
	S_RFSAPR_CMD_DRX_AURORA_READ_ERROR_STATUS		S_Cmd_Aurora_Rd_Error_Sts;
	S_RFSAPR_CMD_DRX_AURORA_READ_FIFO_COUNT			S_Cmd_Aurora_Rd_FIFO_Cnt;
	S_RFSAPR_CMD_DRX_AURORA_READ_RX_FIFO_DATA		S_Cmd_Aurora_Rd_Rx_FIFO_Data;
	S_RFSAPR_CMD_DRX_AURORA_WRITE_TX_FIFO_DATA		S_Cmd_Aurora_Wr_Tx_FIFO_Data;
	S_RFSAPR_CMD_DRX_AURORA_ENABLE_TX_CHANNEL		S_Cmd_Aurora_Enable_Tx_Channel;
	S_RFSAPR_CMD_DRX_AURORA_ENBDIS_LOOPBACK			S_Cmd_Aurora_EnbDis_loopback;

	/****************************************************/


/***** Optical Related  ***********/

	S_RFSAPR_CMD_OPTICAL_CONFIG						m_SCmd_RFSAPR_Opticaltest;
	S_RFSAPR_CMD_OPTICAL_STARTSTOP_COMM				m_SCmd_OpticlStrtStop;
	S_RFSAPR_CMD_OPTICAL_TX_DATA_FRM_HOST			m_sCmd_RFSAPR_TX_Data_Eth_Frm_Host;

	/* Read RFSAPR FEA +12V and -12V Voltage*/

	S_RFSAPR_CMD_READ_FEA_VOLTAGE                   m_sCmd_Read_RFSAPR_FEA_Voltage;

	/* Write FEA RS485 discrete output data in RFSAPR Type 1 unit*/
	S_RFSAPR_CMD_WRITE_FEARS485_DISCRETE_OUTPUT     m_sCmd_Write_FEARS485_Discrete_OPData;

	S_RFSAPR_CMD_CONFIGURE_LOG_VIDEO_CHANNEL         m_sCmd_Config_Log_Video_Ch;


	/* For Attenuation Control*/
	S_CMD_RF_ATTENUATION_CONTROL			        m_sCmd_RfAttenuationCtrl;
	/* SBC SDRAM Test*/
	S_RFSAPR_CMD_SBC_SDRAM_TEST					S_Cmd_SBC_SDRAM_Test;
	/* SBC MRAM Test*/
	S_RFSAPR_CMD_SBC_MRAM_TEST					S_Cmd_SBC_MRAM_Test;
	/* SBC Timer Test*/
	S_RFSAPR_CMD_SBC_TIMER_TEST					S_Cmd_SBC_Timer_Test;
	S_RFSAPR_CMD_SBC_UART_TEST					S_Cmd_SBC_UART_Test;
	S_RFSAPR_CMD_SBC_COUNTER_TEST				S_Cmd_SBC_Counter_Test;
	S_RFSAPR_CMD_SBC_USB_TEST					S_Cmd_SBC_USB_Test;
	S_RFSAPR_CMD_SBC_SATA_TEST					S_Cmd_SBC_SATA_Test;
	S_RFSAPR_CMD_SBC_SET_RTC 					S_Cmd_SBC_RTC_Test;
	S_RFSAPR_CMD_SBC_DMA_TEST					S_Cmd_SBC_DMA_Test;
	S_RFSAPR_CMD_SBC_EEPROM_TEST				S_Cmd_SBC_EEPROM_Test;
	S_RFSAPR_CMD_SBC_WDT_TEST					S_Cmd_SBC_WDT_Test;
	S_RFSAPR_CMD_ALTIVEC_TEST					S_Cmd_SBC_Altivec_Test;


/***** NB DMA **********/
	S_RFSAPR_CMD_NB_DRX_DMA_DATA				S_Cmd_NB_DMA_Read;
/*** End **********/

/***** WB DMA **********/
	S_RFSAPR_CMD_WB_DRX_DMA_DATA				S_Cmd_WB_DMA_Read;

	S_RFSAPR_CMD_READ_ALL_BAND_DMA_DATA			S_Cmd_WB_All_Band_DMA_Read;


/****** 1502 DMA Seperate command ****/
	
	S_RFSAPR_CMD_READ_DMA_DATA					S_Cmd_1502_ReadDMAData;
/*****************/
	
/********** 1502 Log ADC volt *******/
	S_RFSAPR_CMD_READ_LOG_ADC_VOLT				S_Cmd_ReadLogADCAvgVolt;
/**************/
	

/*** End **********/

/************* 5028 UART *********/

	S_RFSAPR_CMD_UART_PORT_CONFIGURE			S_cmdUARTPortConfigure;	
	S_RFSAPR_CMD_UART_WRITE_TX_DATA				S_cmdUARTWriteTxData;	
	S_RFSAPR_CMD_UART_READ_RX_DATA				S_cmdUARTReadRxData;	
	S_RFSAPR_CMD_UART_CLOSE_PORT				S_cmdUARTClosePort;
	S_RFSAPR_CMD_UART_CLOSE_PORT				m_S_cmdUARTClosePort;

/********************************/

/****************** Qt test cases *********************/


	/*XMC QT Loop back Test*/
	S_RFSAPR_CMD_UART_PORT_CONFIGURE			S_Cmd_UART_QT_TX_Config;
	S_RFSAPR_CMD_UART_PORT_CONFIGURE			S_Cmd_UART_QT_RX_Config;


	S_RFSAPR_CMD_UART_PORT_CONFIGURE			m_SCmdSBCUARTConfig;
	S_RFSAPR_CMD_UART_WRITE_TX_DATA				m_SCmdSBCUART_Write_Data;
	S_RFSAPR_CMD_FEA_WRITE_TX_DATA				m_SCmdFEARS422WriteData;
	S_RFSAPR_CMD_READ_FEARS422_ALL_DRX_RX		m_SCmdFEARS422ReadAllDRx;

	/*AUTS Video TTL*/
	S_RFSAPR_CMD_WRITE_AUTS_VIDEO_TTL           m_SCmdWriteAUTSVideoTTLData;

	S_RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_BLANKING m_SCmdWriteNBDRxLookThruBlanking;
	S_RFSAPR_CMD_READ_ALLDRX_LOOK_THRU_BLANKING m_SCmdReadAllDRxLookThruBlanking;

	S_RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_RADAR	m_SCmdWriteNBDRxLookThruRadar;
	S_RFSAPR_CMD_READ_ALLDRX_LOOK_THRU_RADAR	m_SCmdReadAllDRxLookThruRadar;

	S_RFSAPR_CMD_READ_BLANKING_SIGNAL_PULSE		m_SCmdReadBlankingPulse;

	S_RFSAPR_CMD_WRITE_OPEN_GND_CONTROLS_QT_TEST    m_SCmdWriteOpenGndSignalSts;
	S_RFSAPR_CMD_READ_OPEN_GND_CONTROLS_DRX_QT_TEST m_SCmdReadOpenGndSignalSts;

	/***********************/

	/****IO MATRIX CONFIG******************************************************/
	S_RFSAPR_CMD_RF_IOMATRIX_CONFIG					m_S_CmdIORFConfig;
	S_RFSAPR_CMD_ATTENUATION_IOMATRIX_CONFIG		m_S_CmdIOAttenuationConfig;
	S_RFSAPR_CMD_BITE_IOMATRIX_CONFIG				m_S_CmdIOBiteConfig;
	/***************************************************************************/


/** Added for the NB BDC */
	S_RFSAPR_CMD_CONFIG_NB_BDC					S_Cmd_ConfigNBBDC;
	S_CMD_LOG_ADC_PATTERN_EN_DIS				S_Cmd_Pattern_En_Dis;
	S_CMD_RT_SUB_SCAN_BAND_CONFIGURATION		S_Cmd_RT_SubBandScanning;
/*********************************************/

/*************** Added for the Flash configurations */

	S_RFSAPR_CMD_SEND_FLASH_DATA_SIZE				S_CmdFlashDataSize;
	/*S_RFSAPR_CMD_SEND_FLASH_CONFIG_DATA			S_CmdFlashDataConfig;*/
	S_RFSAPR_CMD_FLASH_CONFIG_DATA					S_CmdFlashDataConfig;
	
	S_RFSAPR_CMD_WRITE_CONFIG_DATA_TO_FLASH			S_CmdWriteDatatoFlash;
 	S_RFSAPR_CMD_FLASH_READ_DATA					S_CmdReadFlashData;
/**********************************************************/
/****IO MATRIX CONFIG******************************************************/
	
	S_RFSAPR_CMD_RF_IOMATRIX_CONFIG					S_CmdIORFConfig;
	S_RFSAPR_CMD_ATTENUATION_IOMATRIX_CONFIG		S_CmdIOAttenuationConfig;
	S_RFSAPR_CMD_BITE_IOMATRIX_CONFIG				S_CmdIOBiteConfig;
/***************************************************************************/

	/*******************************SBC JTAG SEL********************************/
	S_RFSAPR_CMD_SBC_JTAG_SEL						S_CmdSBCJtagSel;
/***************************************************************************/

	S_RFSAPR_CMD_NBDC_IOMATRIX_UART_WRITE_DATA		S_CmdNBDCIOMatrixUARTWRConfig;
	S_RFSAPR_CMD_NBDC_IOMATRIX_UART_READ_DATA		S_CmdNBDCIOMatrixUARTRdConfig;

/******************************************************************************/
	S_RFSAPR_CMD_CONFIGURE_NBBDC_RFBUS				S_CmdNBRFBUSConfig;
/*****************************************************************************/

	/******* Added **************/

	/********************/
	S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA	S_CmdNBCalibData;
	S_RFSAPR_CMD_NARROW_BAND_CALIB_DATA_READ		S_CmdNBCalibDataRead;

/***********************/

	S_RFSAPR_CMD_STORE_HW_CALIB_CONST				S_CmdStrHWCalibConst;
	S_RFSAPR_CMD_READ_HW_CALIB_CONST				S_CmdReadHWCalibConst;
	S_RFSAPR_CMD_SET_FEA_CTRL_OP					S_CmdSetFEACtrlOP;
	S_RFSAPR_CMD_DIFF_BLANK_READ					S_CmdDiffBlankRead;
	/** Added for FAN On/Off control *****/
	S_RFSAPR_CMD_FAN_ON_OFF_CONTROL					m_SCmdFanOnOffCtrl;
	/***********************************/
	S_RFSAPR_CMD_CONFIG_DRX_SINENDED_DEBOUNCETIME	m_SCmdConfigDRxDebounceTime;	

	U8BIT			                        m_ucRFSAPRRxData[RFSAPR_MAX_TCPMSG_SIZE]; 
	S_CMD_LOG_ADC_PUL_DATA_AQU				S_Cmd_Read_Log_ADC_PUL_Data;

	S_RFSAPR_CMD_NBBDC_CONFIG					S_CmdNBDCConfig;

	S_RFSAPR_CMD_MISSION_ENABLE					S_CmdNBDCMissionEnableConfig;

	S_RFSAPR_CMD_NBFS					S_CmdNBDCNBFSConfig;

}U_CMD_RFSAPRMESSAGE;

typedef struct
{
	U16BIT		m_usGroupId;
	U16BIT		m_usUnitId;	
}S_RFSAPR_IDS;

/***************************************Structure for RFSAPR Command******************************************/
typedef struct
{
	S_RFSAPR_IDS				m_SRFSAPRCmdIds;
	U_CMD_RFSAPRMESSAGE		m_UCmdRFSAPRMessage;	

}S_COMMAND_FROM_RFSAPR;

/***********************************Structure for TCP TX Packet********************************************/
typedef struct
{
	U8BIT					m_ucTCPHeader[DP_RFSAPR_HEADER_SIZE];
	U32BIT					m_ulMessageLength;
	S_COMMAND_FROM_RFSAPR	m_SCommandFromRFSAPR;		
	U8BIT					m_ucTCPFooter[DP_RFSAPR_FOOTER_SIZE];

}S_RFSAPR_RX_TCP_PACKET;


/* FOR response*/


typedef union
{

	///RESPONSE structure */////
	S_RFSAPR_RES_DEVICE_DETAILS	            m_SResDeviceDetails;
	
	// for ARINC *///			
	S_RFSAPR_RES_ARINC429_RX_DATA_TO_HOST		m_SResARINC429Hostdata;	
	
	// 1553B Test cases
	S_RFSAPR_RES_1553B_SELFTEST_STS			m_SRes1553bSelftestStatus;
	S_RFSAPR_RES_1553B_BC_BUFFER_MSG			m_SRes1553bBCBufferMsg;	
	S_RFSAPR_RES_1553B_READ_BUFF				m_SRes1553bBCReadBuff;	
	S_RFSAPR_RES_RT_MESSAGE					m_SRes1553bRTMsg;	
	S_RFSAPR_RES_SMT_MESSAGE					m_SRes1553bMTMsg;
	S_RFSAPR_RES_1553B_SEND_BC_DATA_TO_HOST	m_SRes1553bBCMsgSts;

	S_RFSAPR_RES_QT_1553B_BC_RT_DATA_OUT		m_SRes1553BBCtoRTQTtest;
		
	//Flash 
	S_RFSAPR_RES_FLASH_DATA_READ				m_SResFlashDataRead;
	S_RFSAPR_RES_FLASH_PAGE_READ				m_SResFlashPageRead;	

	//FPGA
	S_RFSAPR_RES_FPGA_DATA_READ				m_SResFpgaDataRead;
	S_RFSAPR_RES_REG_DATA_READ					m_SResRegDataRead;	
	S_RES_FPGA_READ							S_ResFPGARead;

	/** for 1013 */
	S_RES_NB_FPGA_READ						S_ResNBFPGARead;	

	//SATA
	S_RFSAPR_RES_PERFORM_SATADOM_READ_TEST		m_SResSATAReadTst;	

	//USB
	S_RFSAPR_RES_PERFORM_USB_READ_TEST			m_SResUSBRDTst;

	//UART
	S_RFSAPR_RES_UART_SEND_DATA_TO_HOST		m_SResUARTDatatohost;

	//ADC Data 
	S_RFSAPR_RES_ADC_ACQUIS_TEST				m_SResADCData;

	S_RFSAPR_RES_ADC_FIFO_DATA					S_Res_ADC_FIFO_Data;

//	S_RFSAPR_RES_RF_LOG_ADC_FIFO_DATA			S_Res_RF_LOG_ADC_FIFO_Data;
	S_RFSAPR_RES_READ_ALL_BAND_DATA			S_Res_Read_All_Band_Data;
	S_RFSAPR_RES_READ_ALL_BAND_DATA_2K		S_Res_Read_All_Band_Data_2k;
	S_RFSAPR_RES_READ_ALL_BAND_DATA_4K		S_Res_Read_All_Band_Data_4k;
	S_RFSAPR_RES_READ_ALL_BAND_DATA_8K		S_Res_Read_All_Band_Data_8k;


	// NB ADC FIFO Read */
	S_RFSAPR_RES_NB_ADC_FIFO_DATA				S_Res_NB_ADC_FIFO_Data;	

	/* NB Volatge data */
	S_RFSAPR_RES_READ_NB_VOLTAGE			S_Res_Read_NB_Voltage; 

	/* To Read the PBIT Status */
	S_RFSAPR_PERFORM_PBIT_TEST					S_Res_PBIT_Status;
	S_RFSAPR_PERFORM_SELF_TEST					S_Res_IBIT;


	S_RFSAPR_RES_LOG_ADC_FIFO_DATA				S_Res_Log_ADC_FIFO_Data;

/* Board Status */

	S_RES_BOARD_DET_STATUS					S_Res_Read_Brd_Sts;

	S_RFSAPR_RES_ADC_ACQUIS_TEST				S_ResADC_FFTData;
	S_RFSAPR_RES_NB_ADC_ACQUIS_TEST			S_ResADC_NB_FFTData;

	S_RFSAPR_LOG_ADC_CALIB_TABLE				S_Res_LOG_ADC_Calib_Data;

	S_RES_RFSAPR_RF_GAIN_CALIB_TABLE		S_ResRFSAPRGainTable;

	/*** Added from kaushik ***/
	S_SW_CALIB_DATA							S_Res_ReadAllGainSwCalibData[2][4];


	S_RFSAPR_TEMPERATURE_MEASUREMENTS			S_Res_Temperature_Sensor;

	S_DP_VPX_1502_CONFIG_ADC				S_ResADC_ReadData;
	S_DP_VPX_1502_CONFIG_LOG_ADC			S_ResLOGADC_Data;
	S_DP_VPX_1502_CONFIG_TRF_PLL			S_ResTRF_PLLReadData;
	S_DP_VPX_1502_CONFIG_LMK_PLL			S_ResLMK_PLLReadData;

/*** Audio Codec ******/

	S_RFSAPR_RES_READ_AUDIO_DATA						S_Res_Read_Audio_codec_Data;
	S_RFSAPR_RES_AUDIO_STATUS							S_Res_Read_Audio_Play_Sts;
	S_RFSAPR_RES_READ_AUDIO_CODEC_DEFAULT_CONFIG_FLASH	S_Res_Read_Codec_Default_Config;
	S_RFSAPR_RES_READ_CONFIG_AUDIO_CODEC				S_Res_Read_Configrd_Audio_Codec;

	S_RFSAPR_RES_READ_CODEC_PLL_CONFIG				S_Res_Read_Codec_PLL_Config;
/**** End ***********/

/********* NB DRX Flash Related ****/
	S_RFSAPR_RES_NB_FLASH_PAGE_FILE_READ			S_Res_NB_Flash_Pg_Read;

/*********** WB Flash **********/
	S_RFSAPR_RES_FLASH_PAGE_FILE_READ			S_Res_WB_Flash_Pg_Read;
	S_RFSAPR_RES_FLASH_SINGLE_READ				S_Res_WB_Flash_Single_Read;


	/********************** DRX Aurora related **********************/
	
	
	S_RFSAPR_RES_DRX_AURORA_READ_LINK_STATUS			S_Res_Aurora_Rd_Link_Sts;
	S_RFSAPR_RES_DRX_AURORA_READ_ERROR_STATUS			S_Res_Aurora_Rd_Error_Sts;
	S_RFSAPR_RES_DRX_AURORA_READ_FIFO_COUNT				S_Res_Aurora_Rd_FIFO_Cnt;
	S_RFSAPR_RES_DRX_AURORA_READ_RX_FIFO_DATA			S_Res_Aurora_Rd_FIFO_Data;
	
	/******************** End ***************************************/

	S_RFSAPR_HW_TEMP_CALIB_DATA						m_S_Res_ReadWBRFCalib;

/*********** For Optical *********/

	S_RFSAPR_RES_OPTICAL_RCVD_DATA					m_SCmd_OpticalRcvdData;

	S_RFSAPR_RES_READ_LOG_VIDEO_PULSE_DATA				m_sRes_Read_Log_Video_Pulse;

		/*DP-VPX-0930 SBC Test*/
	S_RFSAPR_RES_SBC_PROCESSOR_TEST 				S_Res_SBC_Processor_Test;
	S_RFSAPR_RES_SBC_SDRAM_TEST						S_Res_SBC_SDRAM_Test;
	S_RFSAPR_RES_SBC_MRAM_TEST						S_Res_SBC_MRAM_Test;
	S_RFSAPR_RES_SBC_TIMER_TEST						S_Res_SBC_Timer_Test;
	S_RFSAPR_RES_SBC_UART_TEST						S_Res_SBC_UART_Test;
	S_RFSAPR_RES_SBC_COUNTER_TEST					S_Res_SBC_Counter_Test;
	S_RFSAPR_RES_SBC_USB_TEST						S_Res_SBC_USB_Test;
	S_RFSAPR_RES_SBC_SATA_TEST						S_Res_SBC_SATA_Test;
	S_RFSAPR_RES_SBC_SET_RTC						S_Res_SBC_RTC_Test;
	S_RFSAPR_RES_SBC_DMA_TEST						S_Res_SBC_DMA_Test;
	S_RFSAPR_RES_SBC_EEPROM_TEST					S_Res_SBC_EEPROM_Test;
	S_RFSAPR_RES_SBC_WDT_TEST						S_Res_SBC_WDT_Test;
	S_RFSAPR_RES_SBC_GET_RTC						S_Res_SBC_Get_RTC;
	S_RFSAPR_RES_ALTIVEC_TEST						S_Res_SBC_Altivec_Test;


/*** NB DMA **/
	S_RFSAPR_RES_NB_DRX_DMA_DATA					S_Res_NB_DMA_Read;
/***************/

/*** NB DMA **/
	S_RFSAPR_RES_WB_DRX_DMA_DATA					S_Res_WB_DMA_Read;
/***************/

/**** UART ******/
	S_RFSAPR_RES_UART_READ_RX_DATA					S_ResUARTReadRxData;
/**************/

/************ QT test cases ***************/

	S_RFSAPR_RES_ARINC429_QT_TEST						m_SResARINCQTTestRes;

	S_RFSAPR_RES_UART_READ_RX_DATA						m_SResUARTReadRxData;

	S_RFSAPR_RES_READ_FEARS422_ALL_DRX_RX				m_SResReadFEARS422DRx;

	S_RFSAPR_RES_READ_ALLDRX_LOOK_THRU_BLANKING			m_SResReadAllDRxLookThruBlanking;

	S_RFSAPR_RES_READ_ALLDRX_LOOK_THRU_RADAR			m_SResReadAllDRxLookThruRadar;

	S_RFSAPR_RES_READ_BLANKING_SIGNAL_PULSE				m_SResReadBlankingPulse;

	S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_DRX_QT_TEST		m_SResReadOpenGndSignalSts;

	S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_IOEXP_QT_TEST	m_SResReadIOExpOpenGndSignalSts;
	S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_XMC_QT_TEST		m_SResReadXMCOpenGndSignalSts;

/*******************************************/

	S_RFSAPR_RES_READ_BLANKING_PULSE_ON_OFF_CNT			m_SResReadBlankgPulseOnOffcnt;
	S_RFSAPR_RES_READ_DIFF_BLANKING_PULSE_ON_OFF		m_SResReadDiifBlankgPulseOnOffcnt;
	S_RFSAPR_RES_READ_DIFF_BLANKING_IO_DATA				m_SResReadDiifBlankgIOSts;

	/*****************/

/***************For 1013 ***************/
	S_DP_VPX_1013_CONFIG_LMX_PLL			S_ResLMX_NB_PLLReadData;
	S_DP_VPX_1013_CONFIG_LMK_PLL			S_ResLMK_NB_PLLReadData;
	S_DP_VPX_1013_CONFIG_ADC				S_ResADC_NB_ReadData;	
/*****************************************/
	
/*************** Added for the Flash configurations */
	S_RFSAPR_RES_READ_FLASH_CONFIG_COMPLETION_STS	S_Res_FlashConfigSts;
	S_RFSAPR_RES_FLASH_READ_DATA					S_Res_FlashReadData;
/*****************************************************/	

/* Newly added **/

	/********** 1502 DMA ************/
	S_RFSAPR_RES_READ_DMA_DATA						S_Res_1502_ReadDMAData;
	/*****************************/
	
	/************* 1502 Log ADC Avg Volt **/
	S_RFSAPR_RES_READ_LOG_ADC_VOLT					S_Res_1502_ReadLogADCAvgVolt;
	/*****************/	


	S_RFSAPR_RES_LOG_ADC_PUL_DATA					S_Res_LOG_ADC_PUL_DATA;


	S_RFSAPR_RES_NBDC_IOMATRIX_UART_READ_DATA		S_ResNBDCIOMatrixUARTRdConfig;

	S_RFSAPR_RES_READ_HW_CALIB_CONST				S_ResReadHWCalibConst;
	
	S_RFSAPR_RES_DIFF_BLANK_READ					S_ResDiffBlankRead;

	S_RFSAPR_RES_NBFS		S_ResNBFSRead;

	S_RFSAPR_RES_NBBDC_CONFIG	S_ResNBDCConfig;

	S_RFSAPR_RES_MISSION_ENABLE		S_ResNBDCMissionEnable;

	/*** Added for log ADC  ******/

	S_RFSAPR_LOG_ADC_MEASD_TABLE					S_ResLogADCMeasTabData;
	S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA			S_ResNBCalibDataRead;
	/************/

	S_RFSAPR_RES_READ_BLANKING_STS					S_ResBlankingsSts;

	S_RFSAPR_HW_LOG_TEMP_CALIB_DATA					S_ResHwLogADCTempData;



	
	U8BIT			                        m_ucRFSAPRTxData[RFSAPR_MAX_TCPMSG_SIZE];

}U_RES_RFSAPRMESSAGE;

/***************************************Structure for RFSAPR Response*****************************************/
typedef struct
{
	S_RFSAPR_IDS				m_SRFSAPRResIds;
	U_RES_RFSAPRMESSAGE		m_UResRFSAPRMessage;	/*m_UResRFSAPRMessage*/

}S_RESPONSE_TO_RFSAPR;

/***********************************Structure for TCP RX Packet********************************************/
typedef struct
{
	U8BIT					m_ucTCPHeader[DP_RFSAPR_HEADER_SIZE];
	S16BIT					m_sMsgStatus;
	S16BIT					m_sReserved;
	U32BIT					m_ulMessageLength;
	S_RESPONSE_TO_RFSAPR	m_SResponseToRFSAPR;	
	U8BIT					m_ucTCPFooter[DP_RFSAPR_FOOTER_SIZE];

}S_RFSAPR_TX_TCP_PACKET;



#endif 
