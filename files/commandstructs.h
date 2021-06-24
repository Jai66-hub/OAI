
#ifndef  _DP_RFSAPR_COMMONSTRUCT_H_
#define  _DP_RFSAPR_COMMONSTRUCT_H_

#pragma pack(push, 1)

#include "Macros.h"
#include "dp_types.h"
#include "dpxmc5028_pro.h"

/* Group ID's for the TCP packet */

#define RFSAPR_COMM_CMD_GROUP0				100	/* Group ID- 0  for Board details  */
#define RFSAPR_COMM_CMD_GROUP1				1	/* Group ID- 1  for Arinc communication */
#define RFSAPR_COMM_CMD_GROUP2				2	/* Group ID- 2  for 1553b communication */
#define RFSAPR_COMM_CMD_GROUP3				3	/* Group ID- 3  for Codec register communication */
#define RFSAPR_COMM_CMD_GROUP4				4	/* Group ID- 4  for FLASH configurations */
#define RFSAPR_COMM_CMD_GROUP5				5	/* Group ID- 5  for FPGA configurations */
#define RFSAPR_COMM_CMD_GROUP6				6	

/* INVALID unit ID , Group ID , Msg Size ID Acknowledgment for Response */

#define RFSAPR_INVALID_UNIT_ID 				100
#define RFSAPR_INVALID_GROUP_ID				101
#define RFSAPR_INVALID_COMMAND_MSG				102	
#define NO_ERROR_ACK						0
#define PARAM_ERROR_ACK						1

/*============================================= RFSAPR Type 1 UNIT ID's =====================================================*/

/*!	Group 0 Commands (RFSAPR_COMM_CMD_GROUP0)	*/

#define	RFSAPR_CMD_VERSION_DETAILS					1 /*! To request for Get System version details */
#define	RFSAPR_RES_VERSION_DETAILS					2 /*! Response command to Get System version details */
												
#define	RFSAPR_CMD_FPGA_READ						15 /*! Command Read the FPGA Read*/
#define	RFSAPR_RES_FPGA_READ						16 /*! Response command Read the FPGA Rea */	
												
#define	RFSAPR_CMD_FPGA_WRITE						17 /*! Command to FPGA Write */
#define	RFSAPR_RES_FPGA_WRITE						18 /*! Response command  FPGA Write*/
												
#define	RFSAPR_CMD_WRITE_FPGA_ADC_DATA				21 /*! Command to ADC Configurations*/
#define	RFSAPR_RES_WRITE_FPGA_ADC_DATA				22 /*! Response command ADC Configurations*/
												
#define	RFSAPR_CMD_READ_FPGA_ADC_CONFIG			23 /*! Command to to ADC Configurations*/
#define	RFSAPR_RES_READ_FPGA_ADC_CONFIG			24 /*! Response command to ADC Configurations*/	
												
#define	RFSAPR_CMD_WRITE_LMK_PLL_CONFIG			27 /*! Command to Config PLL*/
#define	RFSAPR_RES_WRITE_LMK_PLL_CONFIG			28 /*! Response to Config PLL*/

#define	RFSAPR_CMD_READ_LMK_PLL_CONFIG				29 /*! Command to Config PLL*/
#define	RFSAPR_RES_READ_LMK_PLL_CONFIG				30 /*! Response to Config PLL*/

#define	RFSAPR_CMD_WRITE_TRF_PLL_CONFIG			31 /*! Command to Config PLL*/
#define	RFSAPR_RES_WRITE_TRF_PLL_CONFIG			32 /*! Response to Config PLL*/		

#define	RFSAPR_CMD_READ_TRF_PLL_CONFIG				33 /*! Command to Config PLL*/
#define	RFSAPR_RES_READ_TRF_PLL_CONFIG				34 /*! Response to Config PLL*/		
												
#define	RFSAPR_CMD_FPGA_FLASH_ERASE				35 /*! Command to write the FPGA Flash Programming*/
#define	RFSAPR_RES_FPGA_FLASH_ERASE				36 /*! Response command Read the Flash Programming*/
												
#define	RFSAPR_CMD_FPGA_FLASH_WRITE				37 /*! Command to write the FPGA Flash Programming*/
#define	RFSAPR_RES_FPGA_FLASH_WRITE				38 /*! Response command Read the Flash Programming*/
												
#define	RFSAPR_CMD_FPGA_FLASH_READ 				39 /*! Command to write the FPGA Flash Programming*/
#define	RFSAPR_RES_FPGA_FLASH_READ	     			40 /*! Response command Read the Flash Programming*/

#define RFSAPR_CMD_FPGA_WRITE_LOG_ADC_CONFIG		51	/*!  command to Initialize LOG ADC Calibrations*/
#define	RFSAPR_RES_FPGA_WRITE_LOG_ADC_CONFIG 		52 /*!  Response Initialize LOG ADC Calibrations */	

#define RFSAPR_CMD_FPGA_READ_LOG_ADC_CONFIG		53	/*!  command to Initialize LOG ADC Calibrations*/
#define	RFSAPR_RES_FPGA_READ_LOG_ADC_CONFIG 		54 /*!  Response Initialize LOG ADC Calibrations */		

#define RFSAPR_CMD_PBIT							57	/*!  command to PBIT*/
#define	RFSAPR_RES_PBIT			 				58 /*!  Response pBIT*/	

#define RFSAPR_CMD_READ_ADC_FFT_DATA				61	/*!  command to read adc data*/
#define	RFSAPR_RES_READ_ADC_FFT_DATA 				62 /*!  response to read adc data*/

#define	RFSAPR_CMD_READ_FPGA_REGISTERS_ADC_DATA		63 /*! Command to to ADC Configurations*/
#define	RFSAPR_RES_READ_FPGA_REGISTERS_ADC_DATA    64 /*! Response command to ADC Configurations*/	

#define	RFSAPR_CMD_READ_FPGA_REGISTERS_LOG_ADC_DATA 65 /*! Command to to ADC Configurations*/
#define	RFSAPR_RES_READ_FPGA_REGISTERS_LOG_ADC_DATA 66 /*! Response command to ADC Configurations*/	

#define	RFSAPR_CMD_READ_LMK_REGISTERS_PLL_DATA		 67 /*! Command to Config PLL*/
#define	RFSAPR_RES_READ_LMK_REGISTERS_PLL_DATA		 68 /*! Response to Config PLL*/

#define	RFSAPR_CMD_READ_TRF_REGISTERS_PLL_DATA		 69 /*! Command to Config PLL*/
#define	RFSAPR_RES_READ_TRF_REGISTERS_PLL_DATA		 70 /*! Response to Config PLL*/

#define	RFSAPR_CMD_OFFLINE_FILE_PATH_CONFIG		71 /*! Command to Offline File path config*/
#define	RFSAPR_RES_OFFLINE_FILE_PATH_CONFIG		72 /*! Response to Offline File path config*/


#define	RFSAPR_CMD_SET_RFSAPR_SYSTEM_TIME				73 /*! Command to Offline File path config*/
#define	RFSAPR_RES_SET_RFSAPR_SYSTEM_TIME				74 /*! Response to Offline File path config*/

#define	RFSAPR_CMD_WRITE_PROGRAMMABLE_THRESHOLD	75 /*! Command to Offline File path config*/
#define	RFSAPR_RES_WRITE_PROGRAMMABLE_THRESHOLD	76 /*! Response to Offline File path config*/

#define	RFSAPR_CMD_READ_PROGRAMMABLE_THRESHOLD		77 /*! Command to Offline File path config*/
#define	RFSAPR_RES_READ_PROGRAMMABLE_THRESHOLD		78 /*! Response to Offline File path config*/
  
#define	RFSAPR_CMD_LOG_ADC_PATTERN_ENABLE			79 /*! Command to Offline File path config*/
#define	RFSAPR_RES_LOG_ADC_PATTERN_ENABLE		    80 /*! Response to Offline File path config*/

#define	RFSAPR_CMD_LOG_ADC_PATTERN_DISABLE			81 /*! Command to Offline File path config*/
#define	RFSAPR_RES_LOG_ADC_PATTERN_DISABLE		    82 /*! Response to Offline File path config*/

#define RFSAPR_CMD_RF_ATTENUATOR_CONTROL			83	/*!  command to control attenuator*/
#define	RFSAPR_RES_RF_ATTENUATOR_CONTROL 			84 /*!  Response to control attenuator */	

#define RFSAPR_CMD_UPDATE_RF_FRONTEND_CALIB		85	/*!  command to update the calibrate the RF Frontend*/
#define	RFSAPR_RES_UPDATE_RF_FRONTEND_CALIB 		86 /*!  Response for update the calibrate the RF Frontend*/

#define RFSAPR_CMD_READ_RF_FRONTEND_CALIB			87	/*!  command to read the calibrate the RF Frontend*/
#define	RFSAPR_RES_READ_RF_FRONTEND_CALIB 			88 /*!  Response for read the calibrate the RF Frontend*/

#define RFSAPR_CMD_WRITE_FPGA_SEQUENCE_CONFIG		95	/*!  FPGA sequence write configuration command*/
#define RFSAPR_RES_WRITE_FPGA_SEQUENCE_CONFIG		96	/*!  FPGA sequence write configuration response*/

#define RFSAPR_CMD_READ_FPGA_SEQUENCE_CONFIG		97	/*!  FPGA sequence read configuration command*/
#define RFSAPR_RES_READ_FPGA_SEQUENCE_CONFIG		98	/*!  FPGA sequence read configuration response*/

#define RFSAPR_CMD_READ_SENSOR_DATA				103	/*!  FPGA sequence read configuration command*/
#define RFSAPR_RES_READ_SENSOR_DATA				104	/*!  FPGA sequence read configuration response*/

#define	RFSAPR_CMD_LOG_ADC_CALIBRATION				105 /*! Command to calibrate LOG ADC */
#define	RFSAPR_RES_LOG_ADC_CALIBRATION				106 /*! Response command for LOG ADC Calibrarion */

#define	RFSAPR_CMD_READ_LOG_ADC_CALIB_DATA			107 /*! Command to read LOG ADC calibration data */
#define	RFSAPR_RES_READ_LOG_ADC_CALIB_DATA			108 /*! Response command to read LOG ADC Calibrarion data */

#define	RFSAPR_CMD_RT_SUBBANDCONFIGURATION			109 /*! Command to read LOG ADC calibration data */
#define	RFSAPR_RES_RT_SUBBANDCONFIGURATION			110 /*! Response command to read LOG ADC Calibrarion data */

#define RFSAPR_CMD_READ_ADC_FIFO_DATA				115
#define RFSAPR_RES_READ_ADC_FIFO_DATA				116

#define RFSAPR_CMD_READ_LOG_ADC_FIFO_DATA			117
#define RFSAPR_RES_READ_LOG_ADC_FIFO_DATA			118

#define RFSAPR_CMD_READ_BOARD_DET_STATUS			119
#define RFSAPR_RES_READ_BOARD_DET_STATUS			120

#define RFSAPR_CMD_WRITE_NB_LMK_PLL_CONFIG			121
#define RFSAPR_RES_WRITE_NB_LMK_PLL_CONFIG			122

#define RFSAPR_CMD_READ_NB_LMK_PLL_CONFIG			123
#define RFSAPR_RES_READ_NB_LMK_PLL_CONFIG			124

#define RFSAPR_CMD_WRITE_NB_LMX_PLL_CONFIG			125
#define RFSAPR_RES_WRITE_NB_LMX_PLL_CONFIG			126

#define RFSAPR_CMD_READ_NB_LMX_PLL_CONFIG			127
#define RFSAPR_RES_READ_NB_LMX_PLL_CONFIG			128

#define RFSAPR_CMD_READ_NB_FPGA_ADC_CONFIG			129
#define RFSAPR_RES_READ_NB_FPGA_ADC_CONFIG			130

#define RFSAPR_CMD_WRITE_NB_FPGA_ADC_DATA			131
#define RFSAPR_RES_WRITE_NB_FPGA_ADC_DATA			132

#define RFSAPR_CMD_CONFIG_NARROWBAND_BDC			133
#define RFSAPR_RES_CONFIG_NARROWBAND_BDC			134

#define RFSAPR_CMD_READ_NB_BDC_HW_VERSION			135
#define RFSAPR_RES_READ_NB_BDC_HW_VERSION			136

#define	RFSAPR_CMD_NB_FPGA_READ					137 /*! Command Read the FPGA Read*/
#define	RFSAPR_RES_NB_FPGA_READ					138 /*! Response command Read the FPGA Rea */	
												
#define	RFSAPR_CMD_NB_FPGA_WRITE					139 /*! Command to FPGA Write */
#define	RFSAPR_RES_NB_FPGA_WRITE					140 /*! Response command  FPGA Write*/

#define RFSAPR_CMD_READ_NB_ADC_FIFO_DATA			141
#define RFSAPR_RES_READ_NB_ADC_FIFO_DATA			142

#define RFSAPR_CMD_READ_NB_ADC_FFT_DATA				143	/*!  command to read NB ADC data */
#define	RFSAPR_RES_READ_NB_ADC_FFT_DATA 				144 /*! response to read NB ADC data */

#define RFSAPR_CMD_NB_ADC_FIFO_RESET				145
#define RFSAPR_RES_NB_ADC_FIFO_RESET				146

#define RFSAPR_CMD_READ_NB_READ_VOLTAGE			147	
#define RFSAPR_RES_READ_NB_READ_VOLTAGE			148	

#define RFSAPR_CMD_READ_BLANKING_SIGNAL_INFO			149
#define RFSAPR_RES_READ_BLANKING_SIGNAL_INFO			150

#define RFSAPR_CMD_FLASH_SINGLE_WRITE		165
#define RFSAPR_RES_FLASH_SINGLE_WRITE		166

#define RFSAPR_CMD_FLASH_FILE_WRITE			167
#define RFSAPR_RES_FLASH_FILE_WRITE			168

#define	RFSAPR_CMD_FLASH_SINGLE_READ			169
#define	RFSAPR_RES_FLASH_SINGLE_READ			170

#define	RFSAPR_CMD_FLASH_FILE_READ			171
#define	RFSAPR_RES_FLASH_FILE_READ			172

#define RFSAPR_CMD_FLASH_ERASE				173
#define RFSAPR_RES_FLASH_ERASE				174

#define RFSAPR_CMD_FLASH_MEM_TEST			175
#define RFSAPR_RES_FLASH_MEM_TEST			176

#define RFSAPR_CMD_FLASH_READ_ID				177
#define RFSAPR_RES_FLASH_READ_ID				178


#define RFSAPR_CMD_CONFIG_LOG_VIDEO_CH_READ_DATA  		184
#define RFSAPR_RES_CONFIG_LOG_VIDEO_CH_READ_DATA	    185

#define RFSAPR_CMD_READ_LOG_VIDEO_CH_DATA 186
#define RFSAPR_RES_READ_LOG_VIDEO_CH_DATA 187

#define	RFSAPR_CMD_DRX_AURORA_GT_CORE_RESET			210
#define	RFSAPR_RES_DRX_AURORA_GT_CORE_RESET			211

#define	RFSAPR_CMD_DRX_AURORA_FIFO_RESET				212
#define	RFSAPR_RES_DRX_AURORA_FIFO_RESET				213

#define	RFSAPR_CMD_DRX_AURORA_READ_LINK_STATUS		214
#define	RFSAPR_RES_DRX_AURORA_READ_LINK_STATUS		215

#define	RFSAPR_CMD_DRX_AURORA_READ_ERROR_STATUS		216
#define	RFSAPR_RES_DRX_AURORA_READ_ERROR_STATUS		217

#define	RFSAPR_CMD_DRX_AURORA_READ_FIFO_COUNT		218
#define	RFSAPR_RES_DRX_AURORA_READ_FIFO_COUNT		219

#define	RFSAPR_CMD_DRX_AURORA_READ_RX_FIFO_DATA		220
#define	RFSAPR_RES_DRX_AURORA_READ_RX_FIFO_DATA		221

#define	RFSAPR_CMD_DRX_AURORA_WRITE_TX_FIFO_DATA		222
#define	RFSAPR_RES_DRX_AURORA_WRITE_TX_FIFO_DATA		223

#define RFSAPR_CMD_DRX_AURORA_ENABLE_TX_CHANNEL		224
#define RFSAPR_RES_DRX_AURORA_ENABLE_TX_CHANNEL		225

#define RFSAPR_CMD_DRX_AURORA_ENBDIS_LOOPBACK		226
#define RFSAPR_RES_DRX_AURORA_ENBDIS_LOOPBACK		227

#define	RFSAPR_CMD_READ_ALL_BAND_DATA				228
#define	RFSAPR_RES_READ_ALL_BAND_DATA				229

#define	RFSAPR_CMD_READ_LOG_ADC_ALL_BAND_DATA		230
#define	RFSAPR_RES_READ_LOG_ADC_ALL_BAND_DATA		231

#define	RFSAPR_CMD_UPLOAD_TO_TARGET_LOG_ADC_DATA	232
#define	RFSAPR_RES_UPLOAD_TO_TARGET_LOG_ADC_DATA	233

#define	RFSAPR_CMD_DOWNLOAD_TO_TARGET_LOG_ADC_DATA	234
#define	RFSAPR_RES_DOWNLOAD_TO_TARGET_LOG_ADC_DATA	235

#define RFSAPR_CMD_SBC_PROCESSOR_TEST   		236
#define RFSAPR_RES_SBC_PROCESSOR_TEST   		237

#define RFSAPR_CMD_SBC_SDRAM_TEST   			238
#define RFSAPR_RES_SBC_SDRAM_TEST   			239

#define RFSAPR_CMD_SBC_MRAM_TEST   				240
#define RFSAPR_RES_SBC_MRAM_TEST   				241

#define RFSAPR_CMD_SBC_TIMER_TEST   			242
#define RFSAPR_RES_SBC_TIMER_TEST   			243

#define RFSAPR_CMD_SBC_WDT_TEST   			    244
#define RFSAPR_RES_SBC_WDT_TEST   				245

#define RFSAPR_CMD_SBC_UART_TEST   			    246
#define RFSAPR_RES_SBC_UART_TEST   				247

#define RFSAPR_CMD_SBC_DMA_TEST   			    248
#define RFSAPR_RES_SBC_DMA_TEST   				249

#define RFSAPR_CMD_SBC_COUNTER_TEST   			250
#define RFSAPR_RES_SBC_COUNTER_TEST   			251

#define RFSAPR_CMD_SBC_USB_TEST   				252
#define RFSAPR_RES_SBC_USB_TEST   				253

#define RFSAPR_CMD_SBC_SET_RTC   				254
#define RFSAPR_RES_SBC_SET_RTC   				255

#define RFSAPR_CMD_SBC_GET_RTC   				256
#define RFSAPR_RES_SBC_GET_RTC   				257

#define RFSAPR_CMD_SBC_EEPROM_TEST   			258
#define RFSAPR_RES_SBC_EEPROM_TEST   			259

#define RFSAPR_CMD_SBC_SATA_TEST   				260
#define RFSAPR_RES_SBC_SATA_TEST   				261

#define RFSAPR_CMD_SBC_ALTIVEC_TEST             262
#define RFSAPR_RES_SBC_ALTIVEC_TEST             263

#define RFSAPR_CMD_NB_DMA_CONFIGURE				264
#define RFSAPR_RES_NB_DMA_CONFIGURE				265

#define RFSAPR_CMD_NB_ACQUIRE_DMA_DATA			266
#define RFSAPR_RES_NB_ACQUIRE_DMA_DATA			267

#define RFSAPR_CMD_UART_PORT_CONFIGURE			268
#define RFSAPR_RES_UART_PORT_CONFIGURE			269

#define RFSAPR_CMD_UART_WRITE_TX_DATA			270
#define RFSAPR_RES_UART_WRITE_TX_DATA			271

#define RFSAPR_CMD_UART_READ_RX_DATA			272
#define RFSAPR_RES_UART_READ_RX_DATA			273

#define RFSAPR_CMD_CLOSE_UART					274
#define RFSAPR_RES_CLOSE_UART					275

#define RFSAPR_CMD_WB_ACQUIRE_DMA_DATA			276
#define RFSAPR_RES_WB_ACQUIRE_DMA_DATA			277

#define RFSAPR_CMD_WB_ACQUIRE_ALL_BAND_DMA_DATA		278
#define RFSAPR_RES_WB_ACQUIRE_ALL_BAND_DMA_DATA		279

#define RFSAPR_CMD_1502_READ_DMA_DATA				280
#define RFSAPR_RES_1502_READ_DMA_DATA				281

#define RFSAPR_CMD_1502_READ_LOG_ADC_AVG_VOLT			282
#define RFSAPR_RES_1502_READ_LOG_ADC_AVG_VOLT			283

#define RFSAPR_CMD_FEARS422_WRITE_DATA						284
#define RFSAPR_RES_FEARS422_WRITE_DATA						285

#define RFSAPR_CMD_READ_WBDRX_FEARS422						286
#define RFSAPR_RES_READ_WBDRX_FEARS422						287

#define RFSAPR_CMD_READ_NBDRX_FEARS422						288
#define RFSAPR_RES_READ_NBDRX_FEARS422						289
	
#define RFSAPR_CMD_WRITE_AUTS_VIDEO_TTL         			290
#define RFSAPR_RES_WRITE_AUTS_VIDEO_TTL         			291

#define RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_BLANKING   		292
#define RFSAPR_RES_WRITE_NBDRX_LOOK_THRU_BLANKING   		293

#define RFSAPR_CMD_READ_WBDRX_LOOK_THRU_BLANKING   			294
#define RFSAPR_RES_READ_WBDRX_LOOK_THRU_BLANKING   			295

#define RFSAPR_CMD_READ_NBDRX_LOOK_THRU_BLANKING   			296
#define RFSAPR_RES_READ_NBDRX_LOOK_THRU_BLANKING   			297

#define RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_RADAR      		298
#define RFSAPR_RES_WRITE_NBDRX_LOOK_THRU_RADAR      		299

#define RFSAPR_CMD_READ_WBDRX_LOOK_THRU_RADAR      			300
#define RFSAPR_RES_READ_WBDRX_LOOK_THRU_RADAR      			301

#define RFSAPR_CMD_READ_NBDRX_LOOK_THRU_RADAR      			302
#define RFSAPR_RES_READ_NBDRX_LOOK_THRU_RADAR      			303

#define RFSAPR_CMD_READ_WBDRX_BLANKING_SIGNAL_PULSE   		304
#define RFSAPR_RES_READ_WBDRX_BLANKING_SIGNAL_PULSE   		305

#define RFSAPR_CMD_READ_NBDRX_BLANKING_SIGNAL_PULSE   		306
#define RFSAPR_RES_READ_NBDRX_BLANKING_SIGNAL_PULSE   		307

#define RFSAPR_CMD_WRITE_OPEN_GND_SIGNAL_STATUS      		308
#define RFSAPR_RES_WRITE_OPEN_GND_SIGNAL_STATUS      		309

#define RFSAPR_CMD_READ_WBDRX_OPEN_GND_SIGNAL_STATUS    	310
#define RFSAPR_RES_READ_WBDRX_OPEN_GND_SIGNAL_STATUS    	311

#define RFSAPR_CMD_READ_NBDRX_OPEN_GND_SIGNAL_STATUS      	312
#define RFSAPR_RES_READ_NBDRX_OPEN_GND_SIGNAL_STATUS      	313

#define RFSAPR_CMD_READ_IOEXPANDER_OPEN_GND_SIGNAL_STATUS  	314
#define RFSAPR_RES_READ_IOEXPANDER_OPEN_GND_SIGNAL_STATUS  	315

#define RFSAPR_CMD_READ_XMC_OPEN_GND_SIGNAL_STATUS  		316
#define RFSAPR_RES_READ_XMC_OPEN_GND_SIGNAL_STATUS  		317

#define RFSAPR_CMD_SEND_FLASH_DATA_SIZE				  		318
#define RFSAPR_RES_SEND_FLASH_DATA_SIZE				  		319

#define RFSAPR_CMD_SEND_FLASH_CONFIG_DATA			  		320
#define RFSAPR_RES_SEND_FLASH_CONFIG_DATA			  		321

#define RFSAPR_CMD_WRITE_CONFIG_DATA_TO_FLASH		  		322
#define RFSAPR_RES_WRITE_CONFIG_DATA_TO_FLASH		  		323

#define RFSAPR_CMD_READ_FLASH_CONFIG_COMPLETION_STS	  		324
#define RFSAPR_RES_READ_FLASH_CONFIG_COMPLETION_STS			325

#define RFSAPR_CMD_READ_FLASH_CONFIG_FILE_SIZE_FRM_TRGT		326
#define RFSAPR_RES_READ_FLASH_CONFIG_FILE_SIZE_FRM_TRGT		327

#define RFSAPR_CMD_READ_FLASH_CONFIG_DATA			  		328
#define RFSAPR_RES_READ_FLASH_CONFIG_DATA					329

#define RFSAPR_CMD_UPDATE_GAIN_SW_CALIB_DATA	    330	/*! Command to update the Gain Software calibration */
#define	RFSAPR_RES_UPDATE_GAIN_SW_CALIB_DATA 	    331	/*! Response for update the Gain Software calibration */

#define RFSAPR_CMD_READ_GAIN_SW_CALIB_DATA		    332	/*! Command to update the Gain Software calibration */
#define	RFSAPR_RES_READ_GAIN_SW_CALIB_DATA 		    333	/*! Response for update the Gain Software calibration */

#define	RFSAPR_CMD_READ_ALL_SW_GAIN_CALIB_DATA		334
#define	RFSAPR_RES_READ_ALL_SW_GAIN_CALIB_DATA		335

#define RFSAPR_CMD_JTAG_SELECTION					336
#define RFSAPR_RES_JTAG_SELECTION					337

#define RFSAPR_CMD_SBC_JTAG_SEL			  		339
#define RFSAPR_RES_SBC_JTAG_SEL					340

#define RFSAPR_CMD_IO_RF_CONFIG_DATA			  		341
#define RFSAPR_RES_IO_RF_CONFIG_DATA					342

#define RFSAPR_CMD_IO_ATTENUATION_CONFIG_DATA			  		343
#define RFSAPR_RES_IO_ATTENUATION_CONFIG_DATA					344

#define RFSAPR_CMD_IO_BITE_CONFIG_DATA			  		345
#define RFSAPR_RES_IO_BITE_CONFIG_DATA					346

#define RFSAPR_CMD_NBDC_IOMATRIX_UART_WRITE_DATA	363
#define RFSAPR_RES_NBDC_IOMATRIX_UART_WRITE_DATA	364

#define RFSAPR_CMD_NBDC_IOMATRIX_UART_READ_DATA		365
#define RFSAPR_RES_NBDC_IOMATRIX_UART_READ_DATA		366

#define RFSAPR_CMD_SBC_UART_PORT_CONFIGURE		367
#define RFSAPR_RES_SBC_UART_PORT_CONFIGURE		368

#define RFSAPR_CMD_SBC_UART_WRITE_TX_DATA		369
#define RFSAPR_RES_SBC_UART_WRITE_TX_DATA		370

#define RFSAPR_CMD_SBC_CLOSE_UART				371
#define RFSAPR_RES_SBC_CLOSE_UART				372

#define RFSAPR_CMD_CONFIGURE_NB_BDS_AND_BDC		373
#define RFSAPR_RES_CONFIGURE_NB_BDS_AND_BDC		374

#define RFSAPR_CMD_STORE_TEMP_HW_BASED_CALIB		375
#define RFSAPR_RES_STORE_TEMP_HW_BASED_CALIB		376

#define RFSAPR_CMD_READ_TEMP_HW_BASED_CALIB			377
#define RFSAPR_RES_READ_TEMP_HW_BASED_CALIB			378

#define RFSAPR_CMD_SET_FEA_CTRL_OP					379
#define RFSAPR_RES_SET_FEA_CTRL_OP					380

#define RFSAPR_CMD_CONFIGURE_NBDC		381
#define RFSAPR_RES_CONFIGURE_NBDC		382

#define RFSAPR_CMD_CONFIGURE_MISSION_ENABLE		383
#define RFSAPR_RES_CONFIGURE_MISSION_ENABLE		384

#define RFSAPR_CMD_CONFIGURE_NBFS_CONFIG		385
#define RFSAPR_RES_CONFIGURE_NBFS_CONFIG		386

#define RFSAPR_CMD_DIFF_BLANK_READ					387
#define RFSAPR_RES_DIFF_BLANK_READ					388

#define RFSAPR_CMD_SYSTEM_FAN_ON_OFF_CTRL		389
#define RFSAPR_RES_SYSTEM_FAN_ON_OFF_CTRL		390

#define RFSAPR_CMD_UPDATE_LOG_ADC_CALIB				391
#define RFSAPR_RES_UPDATE_LOG_ADC_CALIB				392

#define RFSAPR_CMD_READ_LOG_ADC_CALIB				393
#define RFSAPR_RES_READ_LOG_ADC_CALIB				394

#define RFSAPR_CMD_CONFIG_DRX_SINENDED_DEBOUNCETIME	394
#define RFSAPR_RES_CONFIG_DRX_SINENDED_DEBOUNCETIME	395

#define RFSAPR_CMD_STORE_NBBDC_CALIB_FILE			396
#define RFSAPR_RES_STORE_NBBDC_CALIB_FILE			397

#define RFSAPR_CMD_READ_NBBDC_CALIB_FILE			398
#define RFSAPR_RES_READ_NBBDC_CALIB_FILE			399

#define RFSAPR_CMD_WRITE_LOG_ADC_THRESHOLD_DATA		400
#define RFSAPR_RES_WRITE_LOG_ADC_THRESHOLD_DATA		401

#define RFSAPR_CMD_UPDATE_WBRF_TEMP_CALIB			402
#define RFSAPR_RES_UPDATE_WBRF_TEMP_CALIB			403

#define RFSAPR_CMD_READ_WBRF_TEMP_CALIB			404
#define RFSAPR_RES_READ_WBRF_TEMP_CALIB			405

#define RFSAPR_CMD_READ_BLANKING_STS		406
#define RFSAPR_RES_READ_BLANKING_STS			407


#define RFSAPR_CMD_OPTICAL_CONFIG				282
#define RFSAPR_RES_OPTICAL_CONFIG				283

#define RFSAPR_CMD_OPTICAL_STARTSTOP_COMM		284
#define RFSAPR_RES_OPTICAL_STARTSTOP_COMM		285

#define RFSAPR_CMD_OPTICAL_SEND_RCVD_DATA		286
#define RFSAPR_RES_OPTICAL_SEND_RCVD_DATA		287

#define RFSAPR_CMD_OPTICAL_STARTSTOP_COMM_1		288
#define RFSAPR_RES_OPTICAL_STARTSTOP_COMM_1		289

#define RFSAPR_CMD_OPTICAL_SEND_TX_DATA			290
#define RFSAPR_RES_OPTICAL_SEND_TX_DATA			291

#define RFSAPR_CMD_READ_FEA_VOLTAGE				292
#define RFSAPR_RES_READ_FEA_VOLTAGE				293

#define RFSAPR_CMD_WRITE_FEARS485_DISCRETE_OUTPUT	294
#define RFSAPR_RES_WRITE_FEARS485_DISCRETE_OUTPUT	295

/*********************************** Structures for DP-VPX-0930 SBC Test Cases **************************************/
/*** Response structures for DP-VPX-0930 SBC processor test ***/
typedef struct 
{	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_PROCESSOR_TEST;


/*** Command structures for DP-VPX-0930 SBC SDRAM test ***/
typedef struct {
	
	U8BIT m_ucSDRAMTestType;								/** 1 - DATA BUS TEST, 2 - ADDRESS BUS TEST, 3- DEVICE TEST**/

}S_RFSAPR_CMD_SBC_SDRAM_TEST;

/*** Response structures for DP-VPX-0930 SBC SDRAM test ***/
typedef struct {
	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_SDRAM_TEST;

/*** Command structures for DP-VPX-0930 SBC MRAM test ***/
typedef struct {
	
	U8BIT m_ucMRAMTestType;								/** 1 - MRAM DATA RETENSION TEST, 2 - MRAM CHECKSUM TEST **/

}S_RFSAPR_CMD_SBC_MRAM_TEST;

/*** Response structures for DP-VPX-0930 SBC MRAM test ***/
typedef struct
{
	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_MRAM_TEST;
	
/*** Command structures for DP-VPX-0930 SBC Timer test ***/
typedef struct 
{
	
	U8BIT   m_ucTimerNumber; 								/** 1 -> Timer-1 Test, 2 -> Timer-2 Test, 3 -> Timer-3 Test, 4 -> Timer-4 Test  **/
	U32BIT  m_uiTimerTimeout;								/**  1msec to 60Sec **/
	U16BIT	m_usTimerRefTimeOut;							/* ((m_uiTimerTimeout / 1000) + 1) **/  /*NOTE :: Wait time must be greater than Timer base count value*/
		
}S_RFSAPR_CMD_SBC_TIMER_TEST;	


/*** Response structures for DP-VPX-0930 SBC Timer test ***/
typedef struct 
{
	U32BIT m_ulTimerIntCnt;
	U8BIT  m_ucResult;
	
}S_RFSAPR_RES_SBC_TIMER_TEST;

/*** Command structures for DP-VPX-0930 SBC WDT test ***/
typedef struct {
	
	U8BIT   m_ucWDT_TestType; 						/**  1 - WDT_RST_TEST, 2 - WDT_INT_TEST , 3 - WDT_DIS, 4 - WDT_GEN_TRIGGER, 5 - WDT_GET_INT_COUNT, 6 - WDT_CLR_INT_COUNT**/
	U8BIT   m_ucWDT_Mode; 						/**  1 - AVIONICS_MODE, 2 - STANDARD_MODE **/
	U8BIT	m_ucIsTrigReq;							/*Trigger Required or not after Delay time out*/
	U16BIT  m_usWDTMinTimeout; 											/**  Minimum Count Range -> MIN :1 , MAX: (40000 - 1) **/
	U16BIT	m_usWDTMaxTimeout;											/**  Maximum Count Range -> MIN : (m_usWDTMinCnt + 1), MAX : 40000 **/  /*NOTE :: WDT maximun count should be greater than minimun counte*/		
	U16BIT  m_usTriggerTimeout; 
	U32BIT	m_ulRefTimeOut;		/*Maximum delay need to wait before trigger and WDT disable in terms of ms*/

}S_RFSAPR_CMD_SBC_WDT_TEST;

/*** Response structures for DP-VPX-0930 SBC WDT test ***/
typedef struct 
{
	U32BIT m_ulWDTIntCnt;	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_WDT_TEST;

/*** Command structures for DP-VPX-0930 SBC RS422-UART test ***/
typedef struct {
	
	U8BIT   m_ucPortNumber; 										/** COM2, COM3 and COM4 **/
	U32BIT  m_uiBaudRate;											/**  4800/9600/19200/38400/57600/115200  **/
	U8BIT	m_ucParity;												/**  0 - Even Parity , 1 - Odd Enable **/
	U8BIT	m_ucDataLength;											/* 5 to 8*/
	U8BIT	m_ucStopBits;											/* 0 - 1 Bit, 1 - 1.5/2 Bits */
	U32BIT	m_uiDataCnt;
		
}S_RFSAPR_CMD_SBC_UART_TEST;


/*** Response structures for DP-VPX-0930 SBC RS422-UART test ***/
typedef struct {
	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_UART_TEST;

/*** Command structures for DP-VPX-0930 SBC DMA test ***/
typedef struct {
	
	unsigned long m_ulSrcAddress;
	unsigned long m_ulDesAddress;
	unsigned long m_ulDataSize ;											 /** Datasize in bytes(in Hex) (MIN : 1BYTE ; MAX : 40MB **/
	
}S_RFSAPR_CMD_SBC_DMA_TEST;

/*** Response structures for DP-VPX-0930 SBC DMA test ***/
typedef struct 
{
	U8BIT m_ucResult;
	unsigned long m_ulSrcAddress;
	unsigned long m_ulDesAddress;
	unsigned long m_ulDataSize ;			/** Data size in bytes(in Hex) ((MIN : 1BYTE ; MAX : 64 * 1024 * 1024 ) - 1 **/
	unsigned long m_ulTransferTime;			/*DMA Data Transfer Time in Seconds*/
	
	
} S_RFSAPR_RES_SBC_DMA_TEST;

/*** Command structures for DP-VPX-0930 SBC Counter test ***/
typedef struct {
	
	U8BIT   m_ucCounterNumber; 								/** 1 -> Counter-1 Test, 2 -> Counter-2 Test, 3 -> Counter-3 Test, 4 -> Counter-4 Test  **/
	U8BIT   m_ucCounterMode;								/** 1 - One Shot Mode, 2 - Continuous Mode,  **/
	U16BIT  m_usCountTimeOut; 								/** Counter Count Value MIN : 1 ms, MAX : 40000 ms) **/
	U16BIT	m_usCounterRefTimeOut;							/** Counter Wait Time MIN : (m_usCountTimeOut / 1000),  MAX : 40 **/  /*NOTE :: Counter Wait time must be greater than count value*/
		
} S_RFSAPR_CMD_SBC_COUNTER_TEST;

/*** Response structures for DP-VPX-0930 SBC Counter test ***/
typedef struct {
	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_COUNTER_TEST;

/*** Command structures for DP-VPX-0930 SBC USB test ***/
typedef struct {
	
	char m_szUSBPartiName[64];						/**  USB partion name Ex: bd0**/
	char m_szUSBDataFileName[64];; 							/** Data File Name**/
	U32BIT m_u32DataSize;
} S_RFSAPR_CMD_SBC_USB_TEST;

/*** Response structures for DP-VPX-0930 SBC USB test ***/
typedef struct {
	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_USB_TEST;

/*** Command structures for DP-VPX-0930 SBC Set RTC  ***/
typedef struct 
{
	U16BIT  m_usYear; 								/** MIN : 1970, MAX : 2068"**/
	U8BIT   m_ucMonth;								/** MIN : 1, MAX : 12 **/
	U8BIT  	m_ucDay; 								/** MIN : 1, MAX : 31 **/ /*NOTE:: No.of Days is depending on the month */
	U8BIT   m_ucHour;								/** MIN : 0, MAX : 23 **/
	U8BIT   m_ucMinutes;							/** MIN : 0, MAX : 59 **/
	U8BIT   m_ucSeconds;							/** MIN : 0, MAX : 59 **/
	U8BIT	m_ucWeekDay;							/*Sunday = 0,...Saturday = 6*/
}S_RFSAPR_CMD_SBC_SET_RTC;

/*** Response structures for DP-VPX-0930 SBC RTC test ***/
typedef struct 
{
	U8BIT	m_ucResult;							/*Set RTC Status*/
}S_RFSAPR_RES_SBC_SET_RTC;

/*** Response structures for DP-VPX-0930 SBC Get RTC ***/
typedef struct 
{
	U16BIT  m_usYear; 								/** MIN : 1970, MAX : 2068"**/
	U8BIT   m_ucMonth;								/** MIN : 1, MAX : 12 **/
	U8BIT  	m_ucDay; 								/** MIN : 1, MAX : 31 **/ /*NOTE:: No.of Days is depending on the month */
	U8BIT   m_ucHour;								/** MIN : 0, MAX : 23 **/
	U8BIT   m_ucMinutes;							/** MIN : 0, MAX : 59 **/
	U8BIT   m_ucSeconds;							/** MIN : 0, MAX : 59 **/
	U8BIT	m_ucWeekDay;							/*Sunday = 0,...Saturday = 6*/

}S_RFSAPR_RES_SBC_GET_RTC;


/*** Command structures for DP-VPX-0930 SBC EEPROM test ***/
typedef struct {
	
	U8BIT m_ucEEPROMTestType;						/** 1 - DATA RETENTION TEST, 2 - EEPROM CHECKSUM TEST**/
	
}S_RFSAPR_CMD_SBC_EEPROM_TEST;

/*** Response structures for DP-VPX-0930 SBC EEPROM test ***/
typedef struct {
	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_EEPROM_TEST;

/*** Command structures for DP-VPX-0930 SBC SATA test ***/
typedef struct {
	
	char  m_szSATADevName[64];						/**  SATA Device name  Ex: **/
	char  m_szSATADataFileName[64];; 						/**  Data **/
	U32BIT m_u32DataSize;
}S_RFSAPR_CMD_SBC_SATA_TEST;

/*** Response structures for DP-VPX-0930 SBC SATA test ***/
typedef struct {
	
	U8BIT m_ucResult;
	
}S_RFSAPR_RES_SBC_SATA_TEST;

typedef union
{
	U8BIT m_arru8Operand[16];
	U16BIT m_arru16Operand[8];
	U32BIT m_arru32Operand[4];
}U_VECTOR_DATA;

/*** Command structures for DP-VPX-0930 SBC Altivec test ***/
 typedef struct 
 {
	 U8BIT m_u8DataType;
	 U8BIT m_u8TestType;
	 U_VECTOR_DATA m_UVectorOperand1;
	 U_VECTOR_DATA m_UVectorOperand2;
 }S_RFSAPR_CMD_ALTIVEC_TEST;

 /*** Response structures for DP-VPX-0930 SBC Altivec test ***/
 typedef struct 
 {
	 U8BIT m_u8DataType;
	 U8BIT m_u8TestType;
	 U_VECTOR_DATA m_UVectorOperand1;
	 U_VECTOR_DATA m_UVectorOperand2;
	 U_VECTOR_DATA m_UVectorResult;
 }S_RFSAPR_RES_ALTIVEC_TEST;

/***************** Commnads for the SATA-DOM Test *******************/

#define RFSAPR_CMD_PERFORM_SATADOM_WRITE_TEST		20
#define RFSAPR_RES_PERFORM_SATADOM_WRITE_TEST		21

#define RFSAPR_CMD_PERFORM_SATADOM_READ_TEST		22
#define RFSAPR_RES_PERFORM_SATADOM_READ_TEST		23

#define ACK_SATA_FILE_CREATE_ERROR				28
#define ACK_SATA_FILE_READ_ERROR				29

typedef struct 
{
	U16BIT	m_usData[1024];
	U16BIT  m_usDataCount;
	S8BIT	m_szSATADevicename[30];	
}S_RFSAPR_CMD_PERFORM_SATADOM_WRITE_TEST;

typedef struct 
{
	S8BIT	m_szSATADevicename[30];	
}S_RFSAPR_CMD_PERFORM_SATADOM_READ_TEST;

typedef struct 
{
	U16BIT	m_usData[1024];
}S_RFSAPR_RES_PERFORM_SATADOM_READ_TEST;


#define RFSAPR_CMD_PERFORM_USB_WRITE_TEST		25
#define RFSAPR_RES_PERFORM_USB_WRITE_TEST		26

#define RFSAPR_CMD_PERFORM_USB_READ_TEST		27
#define RFSAPR_RES_PERFORM_USB_READ_TEST		28

#define ACK_USB_FILE_CREATE_ERROR			30
#define ACK_USB_FILE_READ_ERROR				31

typedef struct 
{
	U16BIT	m_usData[1024];
	U16BIT  m_usDataCount;
	S8BIT	m_szUSBDevicename[30];	
}S_RFSAPR_CMD_PERFORM_USB_WRITE_TEST;

typedef struct 
{
	S8BIT	m_szUSBDevicename[30];	
}S_RFSAPR_CMD_PERFORM_USB_READ_TEST;

typedef struct 
{
	U16BIT	m_usData[1024];
}S_RFSAPR_RES_PERFORM_USB_READ_TEST;

#define RFSAPR_CMD_UART_TX_CONFIGURE				32  // Configure the Tx of UART
#define RFSAPR_RES_UART_TX_CONFIGURE				32

#define RFSAPR_CMD_UART_RX_CONFIGURE				34	// Configure the Rx of UART
#define RFSAPR_RES_UART_RX_CONFIGURE				35

#define RFSAPR_CMD_UART_START_RX_AND_RECEIVE		36	//To start the Rx channel and Rcv the data 
#define RFSAPR_RES_UART_START_RX_AND_RECEIVE		37

#define RFSAPR_CMD_UART_SEND_DATA_TO_HOST			38	//Receive Channel no. and send the respective data to host through Ethernet.
#define RFSAPR_RES_UART_SEND_DATA_TO_HOST			39		

#define RFSAPR_CMD_UART_TRANMIT_DATA				40	//Transmit data through channel 
#define RFSAPR_RES_UART_TRANMIT_DATA				41

#define RFSAPR_CMD_UART_RCV_SEND_DATA_FROM_HOST	42	//Receive data from host thru ethernet and send back it through channel.
#define RFSAPR_RES_UART_RCV_SEND_DATA_FROM_HOST	43

typedef struct 
{
   U32BIT 	m_ulTxBaudrate;
   U8BIT 	m_ucTxPortNo;
   U8BIT 	m_ucTxDataLen;
   U8BIT 	m_ucTxParity;
   U8BIT 	m_ucTxStopBit;
   
}S_RFSAPR_CMD_UART_TX_CONFIG;  
  
typedef struct 
{
   U32BIT 	m_ulRxBaudrate;
   U8BIT 	m_ucRxPortNo;
   U8BIT 	m_ucRxDataLen;
   U8BIT 	m_ucRxParity;
   U8BIT 	m_ucRxStopBit;
   
}S_RFSAPR_CMD_UART_RX_CONFIG;

typedef struct
{	
	U8BIT	m_ucRxChNo;
	U32BIT  m_ulRxWaitDelay;
	U16BIT	m_usTxDatacnt;

}S_RFSAPR_CMD_UART_START_RECEIVING;

typedef struct 
{
	U8BIT 	m_TxChNo;
	U16BIT	m_usTxDataCnt;
	U8BIT	m_ucRxChNo;

}S_RFSAPR_CMD_UART_TRANSMIT_DATA;

typedef struct 
{
	U8BIT	m_ucRxChNo;	
}S_RFSAPR_CMD_UART_SEND_DATA_TO_HOST;

typedef struct 
{
	U16BIT	m_usData[256];
}S_RFSAPR_RES_UART_SEND_DATA_TO_HOST;

typedef struct 
{
	U16BIT  m_usTxDataCnt;
	U32BIT	m_usData[256];	
}S_RFSAPR_CMD_UART_SEND_DATA_FRM_HOST;

#define ACK_RFSAPR_UART_TX_CONFIG_ERROR			65
#define ACK_RFSAPR_UART_RX_CONFIG_ERROR			66
#define ACK_RFSAPR_UART_RX_START_ERROR				67
#define ACK_RFSAPR_UART_TX_START_ERROR				68
#define ACK_RFSAPR_UART_RX_READ_DATA_ERROR			69
#define ACK_RFSAPR_UART_TX_DATA_ERROR				70
#define ACK_RFSAPR_UART_NO_RECEIVER_DATA_ERROR		71

#define RFSAPR_CMD_GETBOARDDETAILS			1
#define RFSAPR_RES_GETBOARDDETAILS			2

#define RFSAPR_CMD_GETDEVICE_LOCATION		3
#define RFSAPR_RES_GETDEVICE_LOCATION		4

#define RFSAPR_CMD_TOTALBOARDS				5
#define RFSAPR_RES_TOTALBOARDS				6


////////////////////*   Structures for the above commands *//////////////////////////////////
typedef struct
{
	U8BIT	m_ucTot5028Brds;
	U8BIT	m_ucTot1502Brds;
	U8BIT	m_ucTot1013Brds;	
}S_RFSAPR_CMD_BOARD_DETAILS;

typedef struct 
{
	U32BIT	m_uiDeviceId;
	U32BIT  m_uiBoardVersion;
}S_RFSAPR_RES_DEVICE_DETAILS;

#define RFSAPR_CMD_ARINC_429_TX_CONFIGURE					11 // Command to Configure the ARINC429 Tranmitter 
#define RFSAPR_RES_ARINC_429_TX_CONFIGURE					12 // Response for above command

#define RFSAPR_CMD_ARINC_429_RX_CONFIGURE					13 //Command to Configure the ARINC429 Receiver 
#define RFSAPR_RES_ARINC_429_RX_CONFIGURE					14 //Response for above command

#define RFSAPR_CMD_ARINC_429_LABEL_CONFIGURE				15 //Command to configure Rx Label
#define RFSAPR_RES_ARINC_429_LABEL_CONFIGURE				16 //response

#define RFSAPR_CMD_START_ARINC_RX_AND_RECEIVE				17 //Command to start and receive data 
#define RFSAPR_RES_START_ARINC_RX_AND_RECEIVE				18 // Response 

#define RFSAPR_CMD_SEND_ARINC429_DATA_TO_HOST				19 // Command to send Received data to host (through Ethernet)
#define RFSAPR_RES_SEND_ARINC429_DATA_TO_HOST				20 // response 

#define RFSAPR_CMD_TRANSMIT_ARINC429_DATA					21 //Command to Transmit data through Channel 
#define RFSAPR_RES_TRANSMIT_ARINC429_DATA					22 // Response 

#define RFSAPR_CMD_ARINC_RCV_AND_SEND_DATA_FROM_HOST		23 //Command to Receive data from host and send it back through channel
#define RFSAPR_RES_ARINC_RCV_AND_SEND_DATA_FROM_HOST		24 //Response 

#define ARINC429_TX_CONFIG_ERROR_ACK				2
#define ARINC429_RX_CONFIG_ERROR_ACK				3
#define ARINC429_RX_START_ERROR						4
#define ARINC429_RX_WORD_READ_ERROR					5
#define ARINC429_RX_FIFO_COUNT_ERROR				6
#define ARINC429_TX_START_ERROR						8
#define ARINC429_TX_DATA_ERROR						10	
#define ARINC429_LABEL_CONFIG_ERROR					11
#define ARINC429_LABEL_MATCH_ERROR					12
#define ARINC429_RX_CHANNEL_SELECT_ERROR			13	
#define ARINC429_TX_CHANNEL_SELECT_ERROR			14	
#define ARINC429_TX_ECHO_ERROR						15
#define ARINC429_CLEAR_FIFO_ERROR					16
#define ARINC429_NO_RECEIVER_DATA_ERROR				17

#define ACK_ARINC429_TXRX_CONFIG_SUCCESS			16
#define ACK_ARINC429_LABEL_CONFIG_SUCCESS			17

typedef struct 
{
	U8BIT 	m_ucTxchno;
	U8BIT 	m_ucWordLen;
	U8BIT 	m_ucDataRate;
	U8BIT 	m_ucDataFormat;
	U8BIT 	m_ucSDIEnb;
	U32BIT 	m_uiPollintervaltime; // Only if Pollmode is Selected or else 0 
	U8BIT 	m_ucParity;		// 0- dis , 1 - enb
	U8BIT 	m_ucParitySel;	// 0- odd , 1- even 
	U16BIT 	m_usTxLabelNo;
	U8BIT 	m_ucTxSDIValue;     // Enter Value between (0 - 3)	
	U8BIT 	m_ucTxSSMValue; 
	U8BIT 	m_ucMode;    // 0- interrupt  , 1- polling	
	U8BIT 	m_ucEchoEnDisable;  //Enable  Disable echo back testoEnDisablen
	U16BIT	m_usFIFOProgThreshold;

}S_RFSAPR_CMD_ARINC429_TX_CONFIGURE;

typedef struct 
{
	U8BIT 	m_ucWordLen;
	U8BIT 	m_ucDataRate;
	U8BIT 	m_ucDataFormat;
	U8BIT 	m_ucSDIEnb;
	U32BIT 	m_uiPollintervaltime; // Only if Pollmode is Selected or else 0 
	U8BIT 	m_ucMode;    // 1- interrupt  , 0- polling	
	U8BIT 	m_ucRxChno;
	U8BIT 	m_ucRxSDIValue;       // Enter Value between (0 - 3)
	U8BIT 	m_ucRxSSMValue;
	U8BIT 	m_ucRxEnbDIsLabelMatch;	
	U16BIT	m_usFIFOProgThreshold;

}S_RFSAPR_CMD_ARINC429_RX_CONFIGURE;

typedef struct 
{

	U8BIT	m_LablMatchEnbDis;
	U8BIT	m_ucLabelEnbDis[256];
	U8BIT	m_arrucLabelvalues[256];
	U16BIT	m_usLabelCount;
	U8BIT	m_ucRxChNo;

}S_RFSAPR_CMD_ARINC429_LABEL_CONFIG;

typedef struct
{	
	U8BIT		m_ucRxChNo;
	U32BIT		m_uiRxWaitDelay; /* In sec */
	U16BIT		m_usRxDatacnt; /* how much data to receive */

}S_RFSAPR_CMD_ARINC429_START_RX_DATA;

typedef struct 
{
	U8BIT 	m_TxChNo;
	U16BIT	m_usTxDataCnt;
	U8BIT	m_ucRxChNo;

}S_RFSAPR_CMD_ARINC429_START_TX_DATA;


typedef struct
{
	U32BIT m_u32Parity;		/*!< Parity value */
    U32BIT m_u32SSM;		/*!< SSM */
    U32BIT m_u32Data;		/*!< Data */
    U32BIT m_u32SDI;		/*!< SDI */
    U32BIT m_u32LabelNo;	/*!< Label number */	
	
}S_RES_ARINC429_32BIT_WORD;

typedef struct
{

	U8BIT	m_ucTxChNo;
    U16BIT  m_usTxDataCnt;
	U32BIT  m_u32ARINCData[1024];

}S_RFSAPR_CMD_ARINC429_TX_DATA_FRM_HOST;

typedef struct
{
	U8BIT	m_ucRxChNo;
}S_RFSAPR_CMD_ARINC429_RX_DATA_TO_HOST;

typedef struct
{
	U16BIT		m_usRcvdDataCnt;
	S_RES_ARINC429_32BIT_WORD	 m_sARINCData[1024];

}S_RFSAPR_RES_ARINC429_RX_DATA_TO_HOST;

#define RFSAPR_CMD_DPXMC5028_1553B_BC_CONFIGURE			61
#define RFSAPR_RES_DPXMC5028_1553B_BC_CONFIGURE			62	

#define RFSAPR_CMD_1553B_BC_DEFINE_MESSAGES_AND_FRAME		63
#define RFSAPR_RES_1553B_BC_DEFINE_MESSAGES_AND_FRAME		64

#define RFSAPR_CMD_1553B_START_BC_RECEIVE_MSGS				65
#define RFSAPR_RES_1553B_START_BC_RECEIVE_MSGS				66

#define RFSAPR_CMD_1553B_BC_STOP							67
#define RFSAPR_RES_1553B_BC_STOP							68

#define RFSAPR_CMD_DPXMC5028_1553B_RT_CONFIGURE			69	
#define RFSAPR_RES_DPXMC5028_1553B_RT_CONFIGURE			70	

#define RFSAPR_CMD_1553B_START_RT							71
#define RFSAPR_RES_1553B_START_RT							72	

#define RFSAPR_CMD_1553B_RECEIVE_RT_MESSAGES 				73
#define RFSAPR_RES_1553B_RECEIVE_RT_MESSAGES 				74

#define RFSAPR_CMD_1553B_STOP_RT_RECEIVING					75
#define RFSAPR_RES_1553B_STOP_RT_RECEIVING					76

#define RFSAPR_CMD_DPXMC5028_1553B_MT_CONFIGURE			77	
#define RFSAPR_RES_DPXMC5028_1553B_MT_CONFIGURE			78	

#define RFSAPR_CMD_1553B_START_MONITOR_MT					79
#define RFSAPR_RES_1553B_START_MONITOR_MT					80

#define RFSAPR_CMD_1553B_STOP_MT_MONITORING				81
#define RFSAPR_RES_1553B_STOP_MT_MONITORING				82

#define RFSAPR_CMD_1553B_SEND_RT_DATA_TO_HOST				83
#define RFSAPR_RES_1553B_SEND_RT_DATA_TO_HOST				84

#define RFSAPR_CMD_1553B_SEND_MT_DATA_TO_HOST				85
#define RFSAPR_RES_1553B_SEND_MT_DATA_TO_HOST				86

#define RFSAPR_CMD_1553B_SEND_BC_DATA_TO_HOST				87
#define RFSAPR_RES_1553B_SEND_BC_DATA_TO_HOST				88

#define RFSAPR_CMD_1553B_SEND_SELFTEST_STATUS				89
#define RFSAPR_RES_1553B_SEND_SELFTEST_STATUS				90

#define RFSAPR_CMD_1553B_START_BC_TRANSMIT					91
#define RFSAPR_RES_1553B_START_BC_TRANSMIT					92

#define RFSAPR_CMD_1553B_CLOSE_TERMINALS					93
#define RFSAPR_RES_1553B_CLOSE_TERMINALS					94

#define RFSAPR_CMD_1553B_BC_RT_QT_TEST						95
#define RFSAPR_RES_1553B_BC_RT_QT_TEST						96

#define RFSAPR_CMD_1553B_SEND_QT_BCTORT_DATA_TO_HOST		97
#define RFSAPR_RES_1553B_SEND_QT_BCTORT_DATA_TO_HOST		98

#define RFSAPR_CMD_READ_MT_DATA								99
#define RFSAPR_RES_READ_MT_DATA								100

#define ACK_1553B_BC_CONFIG_ERROR						19
#define ACK_1553B_BC_START_ERROR						20
#define ACK_1553B_BC_CONFIGURE_SUCCESS					21
#define ACK_1553B_BC_DEF_MSG_ERROR						22
#define ACK_1553B_BC_FRAME_DEF_ERROR					23
#define ACK_1553B_BC_FRAME_AUTOREP_ERROR				24
#define ACK_1553B_BC_READ_INTSTS_ERROR					25
#define ACK_1553B_BC_READ_MSG_ERROR						26
#define ACK_1553B_BC_STOP_ERROR							27
#define ACK_1553B_RT_CONFIG_ERROR						28
#define ACK_1553B_RT_START_ERROR						29
#define ACK_1553B_RT_CHANNEL_SELECT_ERROR				30
#define ACK_1553B_BOARD_MST_RESET_ERROR					31
#define ACK_1553B_RT_BUFF_ALLOC_ERROR					32
#define ACK_1553B_RT_READ_INTSTS_ERROR					33
#define ACK_1553B_RT_READ_TIMEOUT_ERROR					34
#define ACK_1553B_RT_READ_MSG_ERROR 					35
#define ACK_1553B_RT_WRITE_SA_ERROR						36
#define ACK_1553B_MT_CONFIG_ERROR						37
#define ACK_1553B_MT_START_ERROR						38
#define ACK_1553B_MT_FILTER_ERROR						39
#define ACK_1553B_MT_DEF_FILTER_ERROR					40
#define ACK_1553B_MT_STOP_ERROR							41
#define ACK_1553B_MT_READ_BUFF_MSG_ERROR				42
#define ACK_1553B_RT_STOP_ERROR							43
#define ACK_1553B_ENBDIS_INT_ERROR						44	
#define ACK_1553B_ATTACH_INT_ERROR						45
#define ACK_1553B_SEND_RT_DATA_TO_HOST_ERROR			46	
#define ACK_1553B_BC_READ_TIMEOUT_ERROR					47
#define ACK_1553B_SEND_BC_DATA_TO_HOST_ERROR			48

typedef struct
{
	U8BIT	m_ucTermSel; /* 1- BC , 2- RT , 3- MT */
	U8BIT	m_ucChnlSel; /* 1 or 2 */
}S_RFSAPR_CMD_1553B_TERM_CLOSE;

typedef struct 
{

	U16BIT 	m_usLpbkWord;
	U8BIT 	m_ucLpbkAnlg    : 1;          // To Enable Analog Loop back Test
	U8BIT 	m_ucLpbkBusBEn  : 1;          // To Enable Loopback Test on Bus B
	U8BIT 	m_ucLpbkBusAEn  : 1;          // To Enable Loopback Test on Bus A
	U8BIT 	m_ucRAMTstFFail : 1;          // To force RAM Test to fail (To Evaluate validity of RAM Test Logic)
	U8BIT 	m_ucRAMTstType  : 3;          /* To choose type of RAM Test (1-6)	*/
	U8BIT 	m_ucRAMTstEn    : 1;          // To Enable Built-In RAM Test

}S_RFSAPR_CMD_1553B_SELFTEST_CTRL;

typedef struct 
{

	U16BIT	 m_usLpbkWord;
	U8BIT 	 m_ucUnused : 5;
	U8BIT 	 m_ucRAMTestPass  : 1;
	U8BIT 	 m_ucLpbkBusBPass : 1;
	U8BIT	 m_ucLpbkBusAPass : 1;

}S_RFSAPR_RES_1553B_SELFTEST_STS;

typedef struct
{
	U16BIT	m_usNoofMessages;
	U16BIT	m_usMsgIdlist[256];
	U16BIT 	m_usRepeatcnt;
	U16BIT 	m_usFrametime;
	U8BIT 	m_ucEnbDisAutorep;

}S_RFSAPR_CMD_1553B_BC_FRAME_DEF;

typedef struct 
{
	U16BIT m_usRTAddr;
	U16BIT m_usTxRx;
	U16BIT m_usRTSA;
	U16BIT m_usWCorMC;
}S_RFSAPR_1553b_CMD_WRD;

typedef struct 
{

	S_RFSAPR_1553b_CMD_WRD  m_cmd1, m_cmd2;
	U16BIT m_usData[32];

	U16BIT m_usPatternData;
	U16BIT m_usCounterData;
	U16BIT m_usControlwrd;

	U16BIT m_usTTNM;
	U8BIT  m_ucMsdID;
	U8BIT m_ucDataSel; //(0- cntr data , 1- pattern data)
	U8BIT m_ucBus;
	U8BIT m_ucMsgType;//(1-BC-RT ,2-RT-BC,3-BMCWOD,4-BMCWD,5-MCWND,6-MCWDR,7-BRDCST,8-RTRT,9-BRTRT)
	U8BIT m_ucModeSel;// Mode code selection(0- Modecode without data,1-Mode code withData RX, 2-Mode code withData TX )
	U8BIT m_ucEnaSelfTest;	

}S_RFSAPR_CMD_1553B_BC_MSG_DEF;

typedef struct 
{
	U16BIT  m_usMaxMsgs;		// Total BC messages can be defined, including DM msgs

	U8BIT 	m_ucRespTOSel;	/*  0 - 17 micro seconds
				 *  1 - 22 micro seconds
				 *  2 - 58 micro seconds
				 *  3 - 138 micro seconds
				 */
	U8BIT 	m_ucTTResSel;	/*  0 - 16 bit resolution*  1 - 32 bit resolution*/


	U8BIT 	m_ucRetryCount;		/*  0 - Retry Never
					 *  1 - Single Retry
					 *  2 - Double Retry
					 */
	U8BIT 	m_ucRetry1Bus;		/*  0 - Retry on same bus
					 *  1 - Retry on alternate bus
					 */
	U8BIT 	m_ucRetry2Bus;		/*  0 - Retry on same bus
					 *  1 - Retry on alternate bus
					 */

	U8BIT	m_ucChannelSel;

}S_RFSAPR_CMD_1553B_BC_CONFIG;

typedef struct 
{	
	U16BIT	m_usNoofMsgs;
	U8BIT	m_ucChNo;
	U8BIT	m_ucMsgFormat;
	
	S_RFSAPR_CMD_1553B_BC_MSG_DEF		m_S_BCMsgDef;
	S_RFSAPR_CMD_1553B_BC_FRAME_DEF	m_S_BCFrameInfo;
	
}S_RFSAPR_CMD_1553B_BC_MESSAGE_AND_FRAME;

typedef struct{
	
	U8BIT	m_ucBCChNo;	
}S_RFSAPR_CMD_1553B_BC_START_STOP;


typedef struct
{
	U16BIT	m_usBCRunCnt;
	U8BIT	m_ucBCChNo;	
}S_RFSAPR_CMD_1553B_BC_START_RECEIVE;


typedef struct
{
	SDPXMC5028_1553B_BC_MSG 	SBCMsg[128];
	
}S_RFSAPR_RES_1553B_SEND_BC_DATA_TO_HOST;

typedef struct 
{
	U16BIT 		m_usCtrlWord;
	U16BIT 		m_usCmdWord1;
	U16BIT 		m_usCmdWord2;
	U16BIT 		m_usRTStsWord1;
	U16BIT 		m_usRTStsWord2;
	U16BIT 		m_usBlkStsWord;
	U16BIT 		m_usTimeTag;
	U16BIT 		m_usLpbkWord;
	U16BIT 		m_usTTROCnt;	
	U16BIT		m_usReadCount;
	U16BIT		m_usRemCount;	
	U16BIT 		m_usData[32];
	U16BIT 		m_usMsgAddr; //(used only for the RFSAPR_CMD_1553B_BC_READ_MSG)


}S_RFSAPR_RES_1553B_BC_BUFFER_MSG;

typedef struct 
{
	U8BIT m_ucSA;			// RT Sub Address Number (1-30)
	U8BIT m_ucRxReq;			// Whether Rx Buffer is required for this SA
	U8BIT m_ucTxReq;			// Whether Tx Buffer is required for this SA
	U8BIT m_ucUnused;
}S_RFSAPR_CMD_1553B_RT_SAINFO;

typedef struct 
{
	U16BIT 	 					m_usRTSubAdrsData;
	U16BIT 	 					m_usBusASelWord;
	U16BIT	 					m_usBusBSelWord;
	U8BIT 						m_ucRTchnno;
	U8BIT 						m_ucRTAddr;				// RT Address for this channel (0-30)
	U8BIT 						m_ucMaxSAUsage;			// Maximum SAs going to be used
	S_RFSAPR_CMD_1553B_RT_SAINFO	m_SSAInfo[32];	// Information about SAs
	U8BIT				 		m_ucTOSel; /*  0 - 57 micro seconds
							    *  1 - 62 micro seconds
							    *  2 - 100 micro seconds
							    *  3 - 180 micro seconds
							    */
}S_RFSAPR_CMD_1553B_RT_CONFIG;

typedef struct 
{
	U16BIT	m_ucRTchnno;
	U16BIT	m_ucRTAddrs;

}S_RFSAPR_CMD_1553B_RT_COMMON;
typedef struct 
{
	U16BIT	m_usNoofMsgs;
	U8BIT	m_ucRTchnno;	/* RT channel no. */
	U8BIT	m_ucRTAddrs;	/* RT address 	*/
}S_RFSAPR_CMD_1553B_RT_RCV_MSGS;


typedef struct 
{
	U8BIT				ucNoRcvdMsgs;
	SDPXMC5028_1553B_RT_MSG		m_SRTMsg[128];
}S_RFSAPR_RES_RT_MESSAGE;

typedef struct
{
	U8BIT 	m_ucRTchnno ;
	U8BIT 	m_ucRTSA;
	U16BIT 	m_usTxBuffersize;
	U16BIT 	m_usRxBuffersize;
}S_RFSAPR_CMD_1553B_RT_ALLOCBUF;

typedef struct
{
	U8BIT 	m_ucRTchnno;
	U8BIT 	m_ucRTSA;
	U8BIT 	m_ucMode;//(0 - append after previous msg  , 1- write newly at current address)
	U8BIT   m_ucDataCount;			// Number of data in this array 0 -32
	U16BIT 	m_usData[32];			// 16-bit data array	

}S_RFSAPR_CMD_1553B_WRITEDATA_RTSA;

typedef struct
{
	U8BIT 	m_ucRTchnno;
	U8BIT 	m_ucModecode;//(0- 31)
	U16BIT 	m_usModedata;//(0 - append after previous msg  , 1- write newly at current address)

}S_RFSAPR_CMD_1553B_WRITE_MODEDATA;

typedef struct
{
	U8BIT 	m_ucRTchnno;
	U16BIT	m_usBuffersize;

}S_RFSAPR_CMD_1553B_READ_BUFF;

typedef struct
{
	U16BIT 	m_usReadcount;
	U16BIT 	m_usRemcount;
	U16BIT 	m_usMsgInfo;				// Message Information Word
	U16BIT 	m_usTimeTag;				// Time Tag Word
	U16BIT 	m_usCmdWord;				// Message Command Word
	U16BIT 	m_usDescAddr;			// Descriptor Table Addr
	U16BIT 	m_usData[32];			// Message Data
	U8BIT 	m_ucIsSnglRxMsg;	

}S_RFSAPR_RES_1553B_READ_BUFF;

typedef struct
{
	U8BIT m_ucRTAddr;
	U8BIT m_ucTxorRx;//( 0 - Tx , 1- Rx , 2 - both )
	U32BIT m_uiSubAddSelectn;

} S_RFSAPR_CMD_1553B_MT_FILTER;

typedef struct 
{
	U16BIT	m_usChnID;			// 16 bit Channel ID (IMT only)
	U16BIT	m_usMTBuffsize;		// 1-512
	U16BIT  m_IMTPacketCnt;   // 2 - 256 
	U16BIT  m_IMTMsgCnt;    // 1 - 256 
	U8BIT 	m_ucSMTorIMT;		// SMT or IMT configuration// 0 - SMT// 1 - IMT
	U8BIT 	m_ucTOSel;			/*  0 - 18 & 61 micro seconds
						 *  1 - 23 & 66 micro seconds
						 *  2 - 82 & 122 micro seconds
						 *  3 - 140 & 180 micro seconds*/                                         
	U8BIT 	m_ucTTPnt;			/*  0 - Tag Time @ Last bit of last word of a msg
						 *  1 - Tag Time @ First bit of first word of a msg
						 *  2 - Tag Time @ Last bit of first word of a msg
						 *  3 - Never Tag Time (always 0)*/                                         
	U8BIT 	m_ucTimeTagRes;		// Time Tag Resolution (SMT Only)
	// 0 - 16 bit Time Tag
	// 1 - 48 bit Time Tag
	U8BIT 	m_ucDataTypeVer;	/*  0 - IRIG-106-09*  1 - IRIG-106-04*  2 - IRIG-106-05*  3 - IRIG-106-07*/                                         
	U8BIT	m_ucPktHdrTrlrReq;		// EnaDis Automatic Header and Trailer generation
	// 0 - Disable Auto Hdr and Trlr generation
	// 1 - Enable Auto Hdr and Trlr generation
	U8BIT   m_ucMonitorAll;		//0 -Monitor All RT messages 1 - particular RT address 

	U8BIT	m_ucMTChNo;

	U8BIT	m_ucNoofMsgs;

	S_RFSAPR_CMD_1553B_MT_FILTER		m_SCmd_MTFilter;


}S_RFSAPR_CMD_1553B_MT_CONFIG;


typedef struct
{
	U8BIT	m_ucMTChNo;
}S_RFSAPR_CMD_STOP_MT;

typedef struct 
{
	SDPXMC5028_1553B_SMT_MSG	m_SSMTMsg[128];
	U16BIT m_u16Noofmsgs;

}S_RFSAPR_RES_SMT_MESSAGE;

typedef struct 
{
	U8BIT	m_ucMTChNo;
	
	U8BIT	m_ucMTRecrdtype;   /* 0 - Start recording on CMD SYNC
				      1 - Start recording on CMD or DATA SYNC
				      2 - Start recording on CMD SYNC + 2 data bit
				      3 - Start recording on CMD or DATA SYNC + 2 data bit*/

}S_RFSAPR_CMD_1553B_MT_START_MONITOR;

typedef struct 
{
	unsigned char m_ucChhnlNo;

}S_RFSAPR_CMD_MT_READ_DATA;

typedef struct
{
	U8BIT m_ucRTAddr;//(0 - 31)
	U8BIT m_ucRTSubAddr;//(0 - 31)
	U8BIT m_ucTxorRx;//(0 -Tx , 1 - Rx)

}S_RFSAPR_CMD_1553B_MT_FILTER_RTSA;

typedef	struct
{

	U8BIT 						m_ucRTchnno;			// (1 - 2)
	U8BIT 						m_ucRTAddr;				// RT Address for this channel (0-30)
	U16BIT	m_usNoofMsgs; /* No of msgs to receive */

}S_RFSAPR_CMD_QT_1553B_BC_RT_DATA_OUT;

typedef	struct
{
	U16BIT usarrMsgData[32];
	U16BIT usMsgCnt;
	U16BIT usCmdwrd;
	U16BIT usMsgInfo;

}S_RFSAPR_RES_QT_1553B_BC_RT_DATA_OUT;

/**//////////// Group 3  (Audio codec related)///////////////////////////////////////////////////**/

#define  MAX_AUDIO_DATA			1048576	/** 1MB **/	


#define 	RFSAPR_CMD_WRITE_AUDIO_DATA		33
#define 	RFSAPR_RES_WRITE_AUDIO_DATA		34

typedef struct {
	
	U8BIT m_ucAudioNo;
	U8BIT m_arrucAudioData[4096]; /* 4k data */
	U32BIT m_uiAudioSize;
	U32BIT m_uiPktID;

} S_RFSAPR_CMD_WRITE_AUDIO_DATA;

#define 	RFSAPR_CMD_STORE_AUDIO_DATA		1
#define 	RFSAPR_RES_STORE_AUDIO_DATA		2

typedef struct
{	
	U8BIT	m_ucAudioNo;
	U32BIT	m_uiAudioSize;

}S_RFSAPR_CMD_STORE_AUDIO_DATA;


#define 	RFSAPR_CMD_READ_AUDIO_DATA		3
#define 	RFSAPR_RES_READ_AUDIO_DATA		4

typedef struct{
	U8BIT	m_ucAudioNo;
}S_RFSAPR_CMD_READ_AUDIO_DATA;

typedef struct{

	U8BIT	m_ucAudioNo;
	U32BIT	m_uiAudioSize;
	U8BIT	m_arrucAudioData[MAX_AUDIO_DATA];		/** 1 MB of data **/

}S_RFSAPR_RES_READ_AUDIO_DATA;


#define 	RFSAPR_CMD_ERASE_AUDIO_DATA		5
#define 	RFSAPR_RES_ERASE_AUDIO_DATA		6


typedef struct{
	U8BIT	m_ucAudioNo;
}S_RFSAPR_CMD_ERASE_AUDIO_DATA;


#define 	RFSAPR_CMD_PLAY_FLASH_AUDIO		7
#define 	RFSAPR_RES_PLAY_FLASH_AUDIO		8


typedef struct{

	U8BIT	m_ucAudioNo;
	U8BIT	m_ucMode;		/* 1- Signle play mode , 2 -Continuous play mode , 3 - repeat mode */
	U32BIT	m_uiDelay;		/** (Min - 0 to 700 sec) ***/
	U16BIT	m_usRepeatCount;		

}S_RFSAPR_CMD_PLAY_FLASH_AUDIO;


#define 	RFSAPR_CMD_STOP_AUDIO_PLAY		9
#define 	RFSAPR_RES_STOP_AUDIO_PLAY		10

/*** Configure the Audio Volume command ***/

#define 	RFSAPR_CMD_CONFIGURE_AUDIO_VOL		11
#define 	RFSAPR_RES_CONFIGURE_AUDIO_VOL		12

typedef struct{

	short	m_cLeftVol;
	short	m_cRightVol;		
	U8BIT	m_ucVolCtrlEnbDis;		
	U8BIT	m_ucMuteUnmute;		/** ( 00- Both left and Right mute, 01 - L-Mute , R- Unmute , 10- L- Unmute , R- Mute , 11 - Both unmute ) **/

}S_RFSAPR_CMD_CONFIGURE_AUDIO_VOL;


/*** command to mute or unmute the audio***/


#define RFSAPR_CMD_AUDIO_MUTE_UNMUTE    13
#define RFSAPR_RES_AUDIO_MUTE_UNMUTE    14

/*structure for mute or unmute the audio */
typedef struct
{
	U8BIT m_ucMuteUnmute;

}S_RFSAPR_CMD_AUDIO_MUTE_UNMUTE;

/*** command to know the audio playing status ***/

#define RFSAPR_CMD_AUDIO_STATUS			15
#define RFSAPR_RES_AUDIO_STATUS			16

/*structure for reading audio playing status */
typedef struct
{
	U8BIT m_ucStatus;

}S_RFSAPR_RES_AUDIO_STATUS;

/*** command to store audio codec default configuration in flash ***/

#define RFSAPR_CMD_STORE_AUDIO_CODEC_DEFAULT_CONFIG_FLASH  		17
#define RFSAPR_RES_STORE_AUDIO_CODEC_DEFAULT_CONFIG_FLASH  		18


/*Store default configurations in flash*/
typedef struct
{
	U32BIT m_arruiData[64];

}S_RFSAPR_CMD_AUDIO_CODEC_DEFAULT_CONFIG_FLASH;

/*** command to Read audio codec default configurations from flash ***/

#define RFSAPR_CMD_READ_AUDIO_CODEC_DEFAULT_CONFIG_FLASH  		19
#define RFSAPR_RES_READ_AUDIO_CODEC_DEFAULT_CONFIG_FLASH  		20


/*Read audio codec default configurations from flash*/
typedef struct
{
	U32BIT m_arruiData[64];

}S_RFSAPR_RES_READ_AUDIO_CODEC_DEFAULT_CONFIG_FLASH;

/*** command to Configure audio Codec  ***/

#define RFSAPR_CMD_CONFIG_AUDIO_CODEC		21
#define RFSAPR_RES_CONFIG_AUDIO_CODEC		22

/* Configure audio codec*/
typedef struct
{
	U8BIT	m_ucConfigCnt;
	U32BIT 	m_arruiData[64][2];

}S_RFSAPR_CMD_CONFIG_AUDIO_CODEC;

/*** command to Read Configured  audio Codec  ***/

#define RFSAPR_CMD_READ_CONFIG_AUDIO_CODEC			23	
#define RFSAPR_RES_READ_CONFIG_AUDIO_CODEC			24


/* Read configure audio codec*/
typedef struct
{
	
	U8BIT	m_ucConfigCnt;
	U32BIT 	m_arruiData[64][2];

}S_RFSAPR_RES_READ_CONFIG_AUDIO_CODEC;

#define RFSAPR_CMD_WRITE_CODEC_PLL_CONFIG			25	
#define RFSAPR_RES_WRITE_CODEC_PLL_CONFIG			26

typedef struct
{
	U16BIT	m_usLSBData;
	U32BIT	m_uiMSBData;	
}S_RFSAPR_CMD_WRITE_CODEC_PLL_CONFIG;


#define RFSAPR_CMD_READ_CODEC_PLL_CONFIG			27	
#define RFSAPR_RES_READ_CODEC_PLL_CONFIG			28


typedef struct
{
	U16BIT	m_usLSBData;
	U32BIT	m_uiMSBData;	
}S_RFSAPR_RES_READ_CODEC_PLL_CONFIG;

/////////////////////// GROUP 4 ///////////////////////////////////////////////////////
#define RFSAPR_CMD_FLASH_WORD_READ				1
#define RFSAPR_RES_FLASH_WORD_READ				2

#define RFSAPR_CMD_FLASH_WORD_WRITE			3
#define RFSAPR_RES_FLASH_WORD_WRITE			4

#define RFSAPR_CMD_FLASH_PAGE_WRITE			5
#define RFSAPR_RES_FLASH_PAGE_WRITE			6

#define RFSAPR_CMD_FLASH_PAGE_READ				7
#define RFSAPR_RES_FLASH_PAGE_READ				8

#define RFSAPR_CMD_FLASH_ERASE_SECTOR			9
#define RFSAPR_RES_FLASH_ERASE_SECTOR			10

#define RFSAPR_CMD_FLASH_ERASE_CHIP			11
#define RFSAPR_RES_FLASH_ERASE_CHIP			12

#define RFSAPR_CMD_FLASH_AUDIO_PLAY			13
#define RFSAPR_RES_FLASH_AUDIO_PLAY			14

#define RFSAPR_CMD_FLASH_AUDIO_STOP			15
#define RFSAPR_RES_FLASH_AUDIO_STOP			16


#define ACK_FLASH_AUDIO_PLAY_ERROR			50
#define ACK_FLASH_AUDIO_STOP_ERROR			52

typedef struct
{
	U8BIT 	m_ucFlashno; //(1-2)
	U32BIT 	m_uiFlashAddr;
	U16BIT 	m_usFlashdata;

} S_RFSAPR_CMD_FLASH_DATA_WRITE;


typedef struct
{
	U8BIT 	m_ucFlashno; //(1-2)
	U32BIT 	m_uiFlashAddr;	
} S_RFSAPR_CMD_FLASH_DATA_READ;

typedef struct
{
	U16BIT 	m_usFlashdata;	
} S_RFSAPR_RES_FLASH_DATA_READ;


typedef struct
{
	U8BIT 	m_ucFlashno; //(1-2)
	U32BIT 	m_uiFlashAddr;
	U16BIT 	m_usFlashdata;

} S_RFSAPR_CMD_FLASH_PAGE_WRITE;


typedef struct
{
	U8BIT 	m_ucFlashno; //(1-2)
	U32BIT 	m_uiFlashAddr;	
} S_RFSAPR_CMD_FLASH_PAGE_READ;

typedef struct
{
	U16BIT 	m_usFlashdata;	
} S_RFSAPR_RES_FLASH_PAGE_READ;


typedef struct
{
	U8BIT 	m_ucFlashno; //(1-2)
	U32BIT 	m_uiFlashSectorNo; //( 1- 500)
	U16BIT 	m_usNoofsectors;//( 1- 500)

} S_RFSAPR_CMD_FLASH_SECTOR_ERASE;


typedef struct{
	U8BIT m_ucFlashno;
}S_RFSAPR_CMD_FLASH_CHIP_ERASE;


typedef struct 
{
	U8BIT	m_ucPlayModeSel;
	U8BIT	m_ucFLASHNo;
	U32BIT	m_uiDelayCount;
	U16BIT	m_usRepeatCount;
	U32BIT	m_uiGetVolume;
	U32BIT	m_uiStartAddr;
	U32BIT	m_uiStopAddr;

}S_RFSAPR_CMD_FLASH_AUDIO_PLAY_CONFIG;

//////////////////////// GROUP 5 ////////////////////////////////
#define RFSAPR_CMD_FPGA_DATA_WRITE			1
#define RFSAPR_RES_FPGA_DATA_WRITE			2

#define RFSAPR_CMD_FPGA_DATA_READ			3
#define RFSAPR_RES_FPGA_DATA_READ			4

#define RFSAPR_CMD_REGISTER_DATA_WRITE		5
#define RFSAPR_RES_REGISTER_DATA_WRITE		6

#define RFSAPR_CMD_REGISTER_DATA_READ		7
#define RFSAPR_RES_REGISTER_DATA_READ		8

#define RFSAPR_CMD_READ_ADC_RAW_DATA		9
#define RFSAPR_RES_READ_ADC_RAW_DATA		10

#define RFSAPR_CMD_WRITE_LMX_PLL_CONFIG	13
#define RFSAPR_RES_WRITE_LMX_PLL_CONFIG	14

#define RFSAPR_CMD_WRITE_RF_ADC_CONFIG		15
#define RFSAPR_RES_WRITE_RF_ADC_CONFIG		16

#define RFSAPR_CMD_WRITE_IF_ADC_CONFIG		17
#define RFSAPR_RES_WRITE_IF_ADC_CONFIG		18

typedef struct{
	U32BIT 	m_uiFpgadata;
}S_RFSAPR_CMD_FPGA_DATA_WRITE;


typedef struct
{
	U32BIT 	m_uiFpgadata;
}S_RFSAPR_RES_FPGA_DATA_READ;

typedef struct{
	U32BIT 	m_uiRegOffset;
	U32BIT 	m_uiRegdata;
}S_RFSAPR_CMD_REG_DATA_WRITE;


typedef struct
{
	U32BIT 	m_uiRegOffset;
}S_RFSAPR_CMD_REG_DATA_READ;

typedef struct
{
	U32BIT 	m_uiRegdata;
}S_RFSAPR_RES_REG_DATA_READ;

typedef struct
{
	unsigned char ucBoardNumber;
	unsigned char ucADCNumber;
	unsigned char ucReserved[2];

}S_CMD_FFT_DATA_AQU;

typedef struct 
{
	
	short arrsADCData[DP_RFSAPR_DRX_ADC_FIFO_LEN];
	
}S_RFSAPR_RES_ADC_ACQUIS_TEST;

typedef struct
{
	unsigned char ucADCNumber;
	unsigned char ucBoardNumber;
	unsigned char ucFIFOSelctn;  /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	
}S_CMD_ADC_FIFO_DATA_AQU;

typedef struct
{
	unsigned char m_ucAntennaSel; /* 0 - Antenna 1 , 1 - Antenna 2 */
	unsigned char m_ucRFADCFIFOCnt; /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	unsigned char m_ucLogADCFIFOCnt; /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	unsigned char m_ucTempSel; /* 0 - Low , 1- Ambient , 2- High*/
	
}S_RFSAPR_CMD_READ_ALL_BAND_DATA;

typedef struct 
{
	unsigned char m_ucBandNo;
	double m_dReadVolt;
	double m_dReadPower;
}S_RFSAPR_READ_VOLT;

typedef struct
{
	unsigned char m_ucAntennaSel; /* 0 - Antenna 1 , 1 - Antenna 2 */
	unsigned long ulFIFOCnt;	
//	unsigned char m_ucFIFOCnt; /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	S_RFSAPR_READ_VOLT m_arrSReadVolt[17];
	short m_arrsData[4][DP_RFSAPR_ADC_FIFO_LEN_16K];
	
}S_RFSAPR_RES_READ_ALL_BAND_DATA;


typedef struct
{
	unsigned char m_ucAntennaSel; /* 0 - Antenna 1 , 1 - Antenna 2 */
	unsigned long ulFIFOCnt;	
//	unsigned char m_ucFIFOCnt; /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	S_RFSAPR_READ_VOLT m_arrSReadVolt[17];
	short m_arrsData[4][DP_RFSAPR_ADC_FIFO_LEN_2K];
	
}S_RFSAPR_RES_READ_ALL_BAND_DATA_2K;

typedef struct
{
	unsigned char m_ucAntennaSel; /* 0 - Antenna 1 , 1 - Antenna 2 */
	unsigned long ulFIFOCnt;	
//	unsigned char m_ucFIFOCnt; /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	S_RFSAPR_READ_VOLT m_arrSReadVolt[17];
	short m_arrsData[4][DP_RFSAPR_ADC_FIFO_LEN_4K];
	
}S_RFSAPR_RES_READ_ALL_BAND_DATA_4K;

typedef struct
{
	unsigned char m_ucAntennaSel; /* 0 - Antenna 1 , 1 - Antenna 2 */
	unsigned long ulFIFOCnt;	
//	unsigned char m_ucFIFOCnt; /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	S_RFSAPR_READ_VOLT m_arrSReadVolt[17];
	short m_arrsData[4][DP_RFSAPR_ADC_FIFO_LEN_8K];
	
}S_RFSAPR_RES_READ_ALL_BAND_DATA_8K;

typedef struct
{	
	short arrsADCData[DP_RFSAPR_ADC_FIFO_LEN_32K+100];
	unsigned long ulFIFOCnt;		/* 0 - 2k , 1- 4k , 2- 8k , 3-16k */ 
	
}S_RFSAPR_RES_ADC_FIFO_DATA; 

typedef struct
{
	unsigned short m_usFreqSpotCnt;
	double m_dFreq[5120];
	double m_dSWCalib[5120];
}S_SW_CALIB_DATA;

typedef struct
{
	UCHAR m_ucBandSel;
	UCHAR m_ucAntennaSel;
	S_SW_CALIB_DATA m_sSWcalibData;
}S_RFSAPR_GAIN_SW_CALIB_DATA;

typedef struct
{
	UCHAR m_ucBandSel;
	UCHAR m_ucAntennaSel;
}S_RFSAPR_CMD_READ_SW_CALIB_DATA;

typedef struct
{
	unsigned short m_usFreqSpotCnt;
	double m_dFreq[345];
	double m_dSWCalib[345];
}S_NB_SW_CALIB_DATA;

typedef struct
{
	UCHAR m_ucAntennaSel;
	S_SW_CALIB_DATA m_sSWcalibData;
}S_RFSAPR_NB_GAIN_SW_CALIB_DATA;

/**LOG ADC **/
typedef struct
{	
	unsigned char ucAntNumber;
	unsigned char ucADCNumber;
	unsigned char ucBoardNumber;
	unsigned char ucFIFOSelctn;  /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	
}S_CMD_LOG_ADC_FIFO_DATA_AQU;

typedef struct
{	
	short arrsADCData[DP_RFSAPR_ADC_FIFO_LEN_16K];
	unsigned long ulFIFOCnt;		/* 0 - 2k , 1- 4k , 2- 8k , 3-16k */ 
	
}S_RFSAPR_RES_LOG_ADC_FIFO_DATA; 

typedef struct
{
	unsigned char ucADCNumber;
	unsigned char ucBoardNumber;
	unsigned int  uiThreshold;
	unsigned char ucAntNumber;
	
}S_CMD_LOG_ADC_THRESHOLD_DATA_AQU;


typedef struct
{
	unsigned char ucADCNumber;
	unsigned char ucBoardNumber;
	unsigned char ucAntNumber;
	
}S_CMD_LOG_ADC_PUL_DATA_AQU;

typedef struct
{		
	unsigned long ulPulseOnTime;
	unsigned long ulPulseOffTime;
	float		  fPulseMag;
}S_RFSAPR_RES_LOG_ADC_PUL_DATA;


typedef struct 
{
	unsigned long ulAddress;
	unsigned long ulData;
}S_NB_ADC_PLL_ADDR_DATA;

typedef struct 
{
	unsigned char	ucBoardNo;
	S_NB_ADC_PLL_ADDR_DATA	sNBADCPLL[250];	
	unsigned long	ulDataCnt;
}S_CMD_NB_DRX_LMK_PLL_CONFIG;


typedef struct 
{
	unsigned char	ucBoardNo;
	S_NB_ADC_PLL_ADDR_DATA	sNBADCPLL[250];	
	unsigned long	ulDataCnt;
}S_CMD_NB_DRX_LMX_PLL_CONFIG;	

typedef struct
{
	unsigned char	ucBoardNo;
	unsigned char	ucChipSel;
	S_NB_ADC_PLL_ADDR_DATA	sNBADCPLL[250];	
	unsigned long	ulDataCnt;
}S_CMD_NB_DRX_RF_ADC_CONFIG;

typedef struct
{
	unsigned char	ucBoardNo;
	S_NB_ADC_PLL_ADDR_DATA	sNBADCPLL[250];	
	unsigned long	ulDataCnt;
}S_CMD_NB_DRX_IF_ADC_CONFIG;

typedef struct
{
	unsigned char ucBoardNumber;
	unsigned char ucADCNumber;
	unsigned char ucReserved[2];

}S_CMD_NB_FFT_DATA_AQU;

typedef struct 
{
	
	short arrsADCData[DP_RFSAPR_DRX_ADC_FIFO_LEN];
	
}S_RFSAPR_RES_NB_ADC_ACQUIS_TEST;


typedef struct
{
	unsigned char ucADCNumber;
	unsigned char ucBoardNumber;
	unsigned char ucFIFOSelctn;  /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
	
}S_CMD_NB_ADC_FIFO_DATA_AQU;


typedef struct 
{	
	short arrsADCData[DP_RFSAPR_ADC_FIFO_LEN_16K];
	unsigned long ulFIFOCnt;		/* 0 - 2k , 1- 4k , 2- 8k , 3-16k */ 
	
}S_RFSAPR_RES_NB_ADC_FIFO_DATA;

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4214)
#endif

#ifdef WIN32
#pragma warning(pop)
#endif

typedef struct
{
	unsigned char ucBarSelection;
	unsigned char ucBoardNo;
	unsigned char ucReserved[2];	
	unsigned long ulFPGAOffset;
	unsigned long ulFPGAData;

}S_CMD_FPGA_WRITE;


typedef struct
{
	unsigned char ucBarSelection;
	unsigned char ucBoardNo;
	unsigned char ucReserved[2];
	unsigned long ulFPGAOffset;

}S_CMD_FPGA_READ;


typedef struct
{
	unsigned long ulAddress;
	unsigned long ulData;

}S_ADC_PLL_DATA;

typedef struct
{	

	S_ADC_PLL_DATA  S_PLL_Config[512];
	unsigned long   ulDataCount;
	unsigned char   ucBoardNumber;	
	unsigned char   ucPLLSelection;
	unsigned char   ucPLL_Type;	
	unsigned char   ucPLLClockSelection;

}S_DP_VPX_1502_CONFIG_TRF_PLL;

typedef struct
{	

	S_ADC_PLL_DATA  S_PLL_Config[512];
	unsigned long   ulDataCount;

	unsigned char   ucBoardNumber;	
	unsigned char   ucPLLSelection;
	unsigned char   ucPLL_Type;	
	unsigned char	ucReserved;

}S_DP_VPX_1502_CONFIG_LMK_PLL;


typedef struct
{

	S_ADC_PLL_DATA S_ADC_Config[512];
	unsigned long  ulDataCount;

	unsigned char  ucBoardNumber;	
	unsigned char  ucADCSelection;
	unsigned char  ucClockFreqMHz;
	unsigned char  ucReserved;

}S_DP_VPX_1502_CONFIG_ADC;

typedef struct
{
	unsigned char	ucAntenna_Sel;
	unsigned char	ucFreqBand_Sel;
	unsigned char	ucBDCFreqBand_Sel;
	unsigned char	ucReserved;
	float fRF_ADC_Attn_dB;
	float fComposite_Attn_dB;
	float fBDC_1_2GHzAttn_dB;
	float fBDC_1_6GHzAttn_dB;

}S_CMD_RF_ATTENUATION_CONTROL;

typedef struct
{
	unsigned long ulBoardNumber;
	unsigned long ulFlashAddress;
	unsigned long ulDataCountToRead;

}S_FPGA_FLASH_DATA_READ_REQ;

typedef struct
{
	unsigned long ulBoardNumber;
	unsigned long ulFlashAddress;	
	unsigned long ulDataCount;
	unsigned char ucFlashData[DP_RFSAPR_FPGA_FLASH_SECTOR_ERASE_SIZE];

}S_FPGA_FLASH_DATA;

typedef struct
{
	unsigned char ucBoardNo;
	unsigned char ucADC_ChannelNum;
	unsigned long ulFPGAOffset;
	unsigned long ulFPGAData;
	unsigned char ucReserved[2];

}S_CMD_FPGA_LOG_ADC_WRITE;

typedef struct
{

	S_ADC_PLL_DATA				S_LOG_ADC_Config[512];
	unsigned long				ulDataCount;

	unsigned char				ucBoardNumber;
	unsigned char				ucLogADCSelection;
	unsigned char				ucReserved[2]; 

}S_DP_VPX_1502_CONFIG_LOG_ADC;

/**********************	Perform IBIT ****************************/
typedef struct
{
	unsigned char ulAvailableDisk1SpaceResult;
	unsigned char ulAvailableDisk2SpaceResult;
	unsigned char ulAvailableDisk3SpaceResult;
	unsigned char ulAvailableDisk4SpaceResult;


	double	dAvailableDiscSpace;
	double  dTotDiskSpace;

}S_RFSAPR_PERFORM_SELF_TEST;

/**********************	Perform IBIT ****************************/

typedef struct 
{
	unsigned char					ucExtTempFault;
	unsigned char					ucLocalTempFault;
	unsigned char					ucReserved[2];

}DP_RFSAPR_DP_VPX_0930_TEMPERATURE_TEST;

typedef struct 
{
    unsigned char m_u8RAMTestPass;
    unsigned char m_u8LpbkBusBPass;
    unsigned char m_u8LpbkBusAPass;
    unsigned short m_u16LpbkWord;

}S_RFSAPR_1553B_TEST_STS;

typedef struct
{
	unsigned char	m_u8Arincselftest;
    S_RFSAPR_1553B_TEST_STS	s1553BSelfTstSts;

}S_RFSAPR_1553B_ARINC_SELFTEST;

typedef struct{
	
	unsigned char m_ucProcsALUTest;
	unsigned char m_ucSDRAMTest;
	unsigned char m_ucFlashTest;
	unsigned char m_ucMRAMTest;
	unsigned char m_ucUARTTest;
	unsigned char m_ucUSBDOMTest;
	unsigned char m_ucSATADOMTest;
	
}DP_RFSAPR_DPVPX0930_TEST;


typedef struct {

	unsigned char ucAvailableUSBSpaceResult;
	unsigned char ucAvailableSATASpaceResult;

	double dAvailableUSBDiskSpace;
	double dTotUSBDiskSpace;
	double dAvailableSATADiskSpace;
	double dTotSATADiskSpace;
	
} DP_RFSAPR_MEM_SIZE_TEST;


typedef struct 
{
	DP_RFSAPR_DP_VPX_0930_TEMPERATURE_TEST			S_Temp_Test;
	S_RFSAPR_1553B_ARINC_SELFTEST					S_1553B_Arinc;
	DP_RFSAPR_MEM_SIZE_TEST						S_MemTest;
	DP_RFSAPR_DPVPX0930_TEST 					S_Processor_Test;
	unsigned char 							m_ucWBScratchPadSts[2];
	unsigned char 							m_ucNBScratchPadSts;

}S_RFSAPR_PERFORM_PBIT_TEST;

typedef struct
{
	S_RFSAPR_ADC_AMP_CALIB_TABLE_ENGG		S_ADC_Amp_CalibTable_EnggUnits[DP_RFSAPR_RF_ATTN_ANTENNA_CH_COUNT];
	S_RFSAPR_ADC_AMP_CALIB_TABLE			S_ADC_Amp_CalibTable[DP_RFSAPR_DRX_MAX_BOARD_COUNT][DP_RFSAPR_DRX_ADC_CH_COUNT][DP_RFSAPR_MAX_FOLD_BACK_FREQ_BANDS];

}S_ADC_AMP_CALIB_RECORD;

typedef struct
{
	S_ADC_PLL_DATA S_FPGA_Data[512];
	unsigned long  ulDataCount;
	unsigned char  ucBoardNumber;
	unsigned char  ucReserved[3];
	
}S_FPGA_SEQUENCE_WRITE;

typedef struct
{	
	S_DP_VPX_1502_CONFIG_ADC		SADCConfigData[DP_RFSAPR_MAX_FPGA_BOARD_NUMBER][DP_RFSAPR_MAX_ADC_CHANNEL_COUNT];
	S_DP_VPX_1502_CONFIG_LOG_ADC	SLOG_ADCConfigData[DP_RFSAPR_MAX_FPGA_BOARD_NUMBER][DP_RFSAPR_MAX_LOG_ADC_CHANNEL_COUNT];
	S_DP_VPX_1502_CONFIG_TRF_PLL	STRF_PLLConfigData[DP_RFSAPR_MAX_FPGA_BOARD_NUMBER][DP_RFSAPR_MAX_PLL_CHANNEL_NUMBER];
	S_DP_VPX_1502_CONFIG_LMK_PLL	SLMK_PLLConfigData[DP_RFSAPR_MAX_FPGA_BOARD_NUMBER][DP_RFSAPR_MAX_PLL_CHANNEL_NUMBER];

}S_FPGA_CONFIGURATIONS;


typedef struct
{
	unsigned char ucBoardNo;
	unsigned char ucADC_ChannelNum;
	unsigned char arrucReserved[2];

}S_CMD_LOG_ADC_PATTERN_EN_DIS;

typedef struct
{
	unsigned char ucBandSelection[DP_RFSAPR_RF_ATTN_SUB_BAND_COUNT];
	unsigned char ucReserved[3];

}S_CMD_RT_SUB_SCAN_BAND_CONFIGURATION;

typedef struct
{
	unsigned short usWBDRxBrdNo;
	unsigned short usXMC5028BrdNo;
	unsigned short usNBDRxBrdNo;

}S_CMD_BOARD_DET_STATUS;

typedef struct
{
	unsigned short usWBDRxBrdSts[2];
	unsigned short usXMC5028Sts;
	unsigned short usNBDRxBrdSts;
	unsigned long  ulTrgtVrsn;
	unsigned long  ulTrgtChkSum;

}S_RES_BOARD_DET_STATUS;

typedef struct
{
	unsigned char ucBarSelection;
	unsigned char ucBoardNo;
	unsigned char ucReserved[2];	
	unsigned long ulFPGAOffset;
	unsigned long ulFPGAData;

}S_CMD_NB_FPGA_WRITE;


typedef struct
{
	unsigned char ucBarSelection;
	unsigned char ucBoardNo;
	unsigned char ucReserved[2];
	unsigned long ulFPGAOffset;

}S_CMD_NB_FPGA_READ;

typedef struct
{
	unsigned long ulFPGAData;

}S_RES_NB_FPGA_READ;

typedef struct
{

	S_ADC_PLL_DATA				S_ADC_Config[512];
	unsigned long				ulDataCount;

	unsigned char				ucBoardNumber;	
	unsigned char				ucADCSelection;
	unsigned char				ucClockFreqMHz;
	unsigned char				ucReserved;

}S_DP_VPX_1013_CONFIG_ADC;


typedef struct
{
	unsigned char ucBoardNumber;	
	unsigned char m_ucAdcSel;

}S_CMD_NB_ADC_FIFO_RESET;

typedef struct
{	
	/*S_ADC_PLL_DATA				S_PLL_Config[DP_RFSAPR_MAX_NUM_SEQ_CONFIG_DATA];*/
	S_ADC_PLL_DATA				S_PLL_Config[512];
	unsigned long				ulDataCount;								
	unsigned char				ucBoardNumber;
	unsigned char				ucPLLSelection;
	unsigned char				ucPLL_Type;	
	unsigned char				ucPLLClockSelection;

}S_DP_VPX_1013_CONFIG_LMX_PLL;

typedef struct
{	
	/*S_ADC_PLL_DATA				S_PLL_Config[DP_RFSAPR_MAX_NUM_SEQ_CONFIG_DATA];*/
	S_ADC_PLL_DATA				S_PLL_Config[512];
	unsigned long				ulDataCount;								
	unsigned char				ucBoardNumber;
	unsigned char				ucPLLSelection;
	unsigned char				ucPLL_Type;	
	unsigned char				ucReserved;

}S_DP_VPX_1013_CONFIG_LMK_PLL;


typedef struct
{	
	S_DP_VPX_1013_CONFIG_ADC					S_DPVPX1013_ADCConfigData[DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT];

	S_DP_VPX_1013_CONFIG_LMK_PLL				S_LMK_PLLConfigData[DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT];

	S_DP_VPX_1013_CONFIG_LMX_PLL				S_TRF_PLLConfigData[DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT];

	S_FPGA_SEQUENCE_WRITE						S_FPGA_SequenceWrite[DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT];

}S_DP_VPX_1013_CONFIGURATIONS;


typedef struct
{
	unsigned char  m_ucModSel;
	unsigned char  ucDeviceCode;
	unsigned short usConfigData;

}S_RFSAPR_CMD_CONFIG_NB_BDC;


/** Added for the Blankning signal aquisition **/

typedef struct 
{
	unsigned long m_ulOntime;
	unsigned long m_ulOfftime;
	double 		 m_dValue;
	
}S_RFSAPR_RES_BLANKING_SIG_OP;


/*********** Flash related structures ***********/

typedef struct
{
	unsigned char 	m_ucBoardSel;
	unsigned char 	m_ucBoardNo;
	unsigned char	m_ucFlashSel;	/* 1- Storage flash , 2 - configuration flash */
	unsigned long 	m_ulFlashAddrs;
	unsigned char 	m_ucData;
	
}S_RFSAPR_CMD_FLASH_SINGLE_WRITE;

typedef struct
{
	unsigned char 	m_ucBoardSel;
	unsigned char 	m_ucBoardNo;
	unsigned char	m_ucFlashSel;	/* 1- Storage flash , 2 - configuration flash */
	unsigned long	m_ulPageNo;	/** Min - 1 , Max - 256 **/
	unsigned char 	m_ucarrFlashData[512];
	
}S_RFSAPR_CMD_FLASH_PAGE_FILE_WRITE;


typedef struct
{
	unsigned char 	m_ucBoardSel;
	unsigned char 	m_ucBoardNo;
	unsigned char 	m_ucFlashSel; /* 1- Storage flash , 2 - configuration flash */
	unsigned long	m_ulFlashAddr;	
	
}S_RFSAPR_CMD_FLASH_SINGLE_READ;

typedef struct
{
	unsigned char 	m_ucBoardSel;
	unsigned char 	m_ucBoardNo;
	unsigned char 	m_ucFlashSel; /* 1- Storage flash , 2 - configuration flash */
	unsigned long	m_ulPageNo;	/** Min - 1 , Max - 256 **/
	
}S_RFSAPR_CMD_FLASH_PAGE_FILE_READ;

/*** Response structure **/
typedef struct
{
	unsigned char 	m_ucData;/** only if Page write is used */
	
}S_RFSAPR_RES_FLASH_SINGLE_READ;


typedef struct
{
	unsigned char 	m_ucarrFlashData[512];/** only if Page write is used */
	
}S_RFSAPR_RES_FLASH_PAGE_FILE_READ;


typedef struct
{
	unsigned char 	m_ucBoardSel;
	unsigned char 	m_ucBoardNo;
	unsigned char 	m_ucFlashsel; /* 1 - Configuration , 2 - storage flash */
	unsigned char 	m_ucErasetype; /** 1- Sector Erase, 2 - Chip Erase ***/
	unsigned short	m_usSectorNo;	/** Min - 1 , Max - 256 **/
	
}S_RFSAPR_CMD_FLASH_ERASE;


/*********** NB Flash related structures ***********/

typedef struct
{
	unsigned char 	m_ucBoardNo;
	unsigned char 	m_ucFlashWRType; /** 1- Single Write 2- Page Write ***/
	unsigned char	m_ucPageNo;	/** Min - 1 , Max - 256 **/
	unsigned long 	m_ulTimeOut;
	unsigned int 	m_uiarrFlashData[512];
	
}S_RFSAPR_CMD_NB_FLASH_PAGE_FILE_WRITE;

typedef struct
{
	unsigned char 	m_ucBoardNo;
	unsigned char 	m_ucFlashRDType; /** 1- Single Read 2- Page Read ***/
	unsigned int	m_uiFlashAddr;	/** Min - 1 , Max - 256 **/
	unsigned long 	m_ulTimeOut;
	
}S_RFSAPR_CMD_NB_FLASH_PAGE_FILE_READ;

typedef struct
{
	unsigned int 	m_uiarrFlashData[512];/** only if Page write is used */
	
}S_RFSAPR_RES_NB_FLASH_PAGE_FILE_READ;

typedef struct
{
	unsigned char 	m_ucBoardNo;
	unsigned char 	m_ucErasetype; /** 1- Sector Erase 2- Bulk erase ***/
	unsigned char	m_ucSectorNo;	/** Min - 1 , Max - 256 **/
	unsigned long 	m_ulTimeOut;
	
	
}S_RFSAPR_CMD_NB_FLASH_ERASE;

typedef struct 
{
	U8BIT 	m_ucTxChno;
	U8BIT 	m_ucRxChno;
	UINT	m_uiDataCnt;
	UINT	m_uiRxWaitDelay;

}S_RFSAPR_CMD_OPTICAL_CONFIG;


typedef struct
{
	U8BIT 	m_ucChno;
	U8BIT	m_ucStartstop;
	UINT	m_uiDatacnt;

}S_RFSAPR_CMD_OPTICAL_STARTSTOP_COMM;

typedef struct
{
	U8BIT 	m_ucChno;
	USHORT	m_usDataCnt;
	USHORT	m_usData[1024];

}S_RFSAPR_RES_OPTICAL_RCVD_DATA;

typedef struct
{

	U8BIT	m_ucTxChNo;
    U16BIT  m_usTxDataCnt;
	U16BIT 	m_usarrData[2048];

}S_RFSAPR_CMD_OPTICAL_TX_DATA_FRM_HOST;

typedef struct
{
	UCHAR m_ucRFSAPR_Type1_FEA_Channel;
	UCHAR m_ucRFSAPR_Type1_FEA_Channel_Config;

}S_RFSAPR_CMD_READ_FEA_VOLTAGE;

typedef struct
{
	UCHAR	m_ucRFSAPRType1FEARS485DiscretChno;
	USHORT	m_usRFSAPRType1EnbDisDiscrete;
}S_RFSAPR_CMD_WRITE_FEARS485_DISCRETE_OUTPUT;


typedef struct {
	UCHAR m_ucADCChannel;		/* WB DRx Brd Number	  (Min - 0, Max - 1) */
	UCHAR ucBoardNumber;		/* Log ADC Channel Number (Min - 1, Max - 17) */
	
} S_RFSAPR_CMD_CONFIGURE_LOG_VIDEO_CHANNEL;

typedef struct
{
	float  m_fPulseAmplitude;					/* Read Pulse Amplitude from Log ADC */
	float  m_fPulsePeriod;						/* Read Pulse Period from Log ADC */
	float  m_fPulseWidth;						/* Read Pulse width from Log ADC */
	
}S_RFSAPR_RES_READ_LOG_VIDEO_PULSE_DATA;


typedef struct {
	
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;			/* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;				/* Chnl No.  -(1 - 3 )	*/
}S_RFSAPR_CMD_DRX_AURORA_GT_CORE_RESET;

typedef struct {
	
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;            /* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;		        /* Chnl No.  -(1 - 3 )	*/
	unsigned char	m_ucTxReset;
	unsigned char	m_ucRxReset;
}S_RFSAPR_CMD_DRX_AURORA_FIFO_RESET;

typedef struct {
		
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;            /* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;				/* Chnl No.  -(1 - 3 )	*/
}S_RFSAPR_CMD_DRX_AURORA_READ_LINK_STATUS;

typedef struct {
	
	unsigned char	m_ucAuroraRxSts; 				
	unsigned char	m_ucLaneUpSts; 		    
	unsigned char	m_ucChnUpSts; 		    
}S_RFSAPR_RES_DRX_AURORA_READ_LINK_STATUS;

typedef struct {
	
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;            /* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;				/* Chnl No.  -(1 - 3 )	*/
}S_RFSAPR_CMD_DRX_AURORA_READ_ERROR_STATUS;

typedef struct {
	
	unsigned char	m_ucHWErrSts; 				
	unsigned char	m_ucSWErrSts; 		    
	unsigned char	m_ucFramingErrSts; 		
}S_RFSAPR_RES_DRX_AURORA_READ_ERROR_STATUS;

typedef struct {
	
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;            /* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;		        /* Chnl No.  -(1 - 3 )	*/
	unsigned char	m_ucTxRxSel;			/* 0 - Tx FIFO , 1 - Rx FIFO , 2 - b0th */		
}S_RFSAPR_CMD_DRX_AURORA_READ_FIFO_COUNT;

typedef struct {
	
	unsigned long	m_ulReadTxFIFOCnt;		
	unsigned long	m_ulReadRxFIFOCnt;
}S_RFSAPR_RES_DRX_AURORA_READ_FIFO_COUNT;


typedef struct {
	
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;            /* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;		        /* Chnl No.  -(1 - 3 )	*/
	unsigned short	m_usRxDataCnt;	
}S_RFSAPR_CMD_DRX_AURORA_READ_RX_FIFO_DATA;


typedef struct {
	
	unsigned short	m_usRxRdData[1024];	
}S_RFSAPR_RES_DRX_AURORA_READ_RX_FIFO_DATA;


typedef struct {
	
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;            /* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;		        /* Chnl No.  -(1 - 3 )	*/
	unsigned short	m_usTxDataCnt;	
	unsigned short	m_usTxData[1024];			
}S_RFSAPR_CMD_DRX_AURORA_WRITE_TX_FIFO_DATA;


typedef struct 
{
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;            /* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;		        /* Chnl No.  -(1 - 3 )	*/
	unsigned char	m_ucTxEnbDis;
	unsigned char	m_ucLoopbackEnbDis;
}S_RFSAPR_CMD_DRX_AURORA_ENABLE_TX_CHANNEL;

typedef struct 
{
		
	unsigned char	m_ucBoardSel;			/* 1 - WB Sel ,  2- NB Sel */
	unsigned char	m_ucBoardNo;            /* WB - (1 - 2) , NB - 1 */
	unsigned char	m_ucChNo;	            /* Chnl No.  -(1 - 3 )	*/
	unsigned char	m_ucLoopBackEnbDis;
	
}S_RFSAPR_CMD_DRX_AURORA_ENBDIS_LOOPBACK;

typedef struct 
{
	unsigned char	m_ucBrdNo;
	unsigned long	m_ulDataLimit;	
	unsigned long	m_ulDelay;
	
}S_RFSAPR_CMD_NB_DRX_DMA_DATA;

typedef struct 
{
	unsigned char  m_ucarrDMAData[1* 512*1024];	
	unsigned long  m_ulReadCnt;
	
}S_RFSAPR_RES_NB_DRX_DMA_DATA;

typedef struct 
{
	unsigned char	m_ucBrdNo;
	unsigned char	m_ucCntrADCDatasel;
	unsigned long	m_ulDataLimit;		
	unsigned long	m_ulDelay;
	
}S_RFSAPR_CMD_WB_DRX_DMA_DATA;

typedef struct 
{
	unsigned char	m_ucarr1502DMAData[1* 512*1024];	
	unsigned long	m_ulReadCnt;
	
}S_RFSAPR_RES_WB_DRX_DMA_DATA;

typedef struct
{
	unsigned char m_ucAntennaSel; /* 0 - Antenna 1 , 1 - Antenna 2 */
	unsigned long m_ulDatalimit; /* Max 16K */
	unsigned long m_ulDelay; /* in msec */
	
}S_RFSAPR_CMD_READ_ALL_BAND_DMA_DATA;

typedef struct 
{
	unsigned char m_DRxBrdNO; /* 0 - Board-1 , 1 - Board-2 */
	unsigned long m_ulDatalimit; /* Max 8K */
	unsigned long m_ulDelay; /* in msec */		
}S_RFSAPR_CMD_READ_DMA_DATA;

typedef struct 
{
	unsigned char m_DRxBrdNO; /* 0 - Board-1 , 1 - Board-2 */
	unsigned long m_ulReadDataCnt; 
	unsigned char m_arrucReadData[128*1024]; /* Max 128 K */		
}S_RFSAPR_RES_READ_DMA_DATA;

typedef struct
{
	unsigned char m_ucAntennaSel; /* 0 - Antenna 1 , 1 - Antenna 2 */
	unsigned char ucFIFOSelctn; /* 0 - 2k , 1- 4k , 2- 8k , 3-16k */
}S_RFSAPR_CMD_READ_LOG_ADC_VOLT;


typedef struct
{
	unsigned char m_ucAntennaSel; /* 0 - Antenna 1 , 1 - Antenna 2 */
	S_RFSAPR_READ_VOLT m_arrSReadVolt[17];
	
}S_RFSAPR_RES_READ_LOG_ADC_VOLT;

typedef struct 
{
 	 U8BIT 	m_u8PortNumber;		/*!< Port Number */
 	 U32BIT m_u32BaudRate;		/*!< Baud Rate */
 	 U8BIT 	m_u8DataLength;		/*!< Data Length */
 	 U8BIT 	m_u8Parity;			/*!< Parity */
 	 U8BIT 	m_u8StopBits;		/*!< Stop Bits */
}S_RFSAPR_CMD_UART_PORT_CONFIGURE;

/**
*	\struct	S_RFSAPR_CMD_UART_WRITE_TX_DATA
*	\brief	This structure contains members to write the data in UART Tx port
*/
typedef struct 
{
 	 U8BIT 	m_u8PortNumber;		/*!< Port Number */
 	 U16BIT m_u16TxDataCount;	/*!< Tx Data Count */
 	 U8BIT	m_u8TxData[1024];	/*!< Array of Data to be written into UART Tx */
}S_RFSAPR_CMD_UART_WRITE_TX_DATA;

/**
*	\struct	S_RFSAPR_CMD_UART_READ_RX_DATA
*	\brief	This structure contains members to read the data from UART Rx port
*/
typedef struct 
{
 	 U8BIT 	m_u8PortNumber;		/*!< Port Number */
 	 U16BIT m_u16RxDataCount;	/*!< Rx Data Count */
}S_RFSAPR_CMD_UART_READ_RX_DATA;

/**
*	\struct	S_RFSAPR_RES_UART_READ_RX_DATA
*	\brief	This structure contains members to read the data from UART Rx port
*/
typedef struct 
{
 	 U8BIT 	m_u8PortNumber;		/*!< Port Number */
 	 U16BIT m_u16RxDataCount;	/*!< Rx Data Count */
 	 U8BIT	m_u8RxData[1024];	/*!< Array of Data to be read from UART Rx */
}S_RFSAPR_RES_UART_READ_RX_DATA;

/**
*	\struct	S_RFSAPR_CMD_UART_CLOSE_PORT
*	\brief	This structure contains members to close the UART port
*/
typedef struct 
{
 	 U8BIT 	m_u8PortNumber;		/*!< Port Number */

}S_RFSAPR_CMD_UART_CLOSE_PORT;

/*** Response structures for ARINC429 QT test ***/
typedef struct {
	
	U8BIT m_ucArincQTRes;
	
}S_RFSAPR_RES_ARINC429_QT_TEST;

/**
*\struct 	S_RFSAPR_CMD_FEA_WRITE_TX_DATA
*\brief 	This structure contains members to configure the Transceivers,TTL output Channels and write the data in TTL Channel(DP-XMC-5028)  
*/
typedef struct 
{
	 U8BIT 	m_u8TxTransceiverBitConfig;	 /*!< Select No.of Transceivers  */
	 U8BIT 	m_u8TxTransceiverNo[3];		/*!< Tx Transceiver Number */
	 U8BIT 	m_u8TxTransceiverDirection;	/*!< Select Transceivers direction as Output or Input  */
	 U8BIT 	m_u8TxGPOChSel[3];			/*!< Tx GPO Channel Selection */
	 U8BIT 	m_u8TxChData[3];			/*!< Tx Channel Data */

}S_RFSAPR_CMD_FEA_WRITE_TX_DATA;

/**
*\struct 	S_RFSAPR_CMD_READ_FEARS422_ALL_DRX_RX
*\brief 	This structure contains DRx board number    
*/
typedef struct 
{
	U8BIT 	m_u8DRxBrdNo;		/*!< DRx Board Number */
	
}S_RFSAPR_CMD_READ_FEARS422_ALL_DRX_RX;

/**
*\struct 	S_RFSAPR_RES_READ_FEARS422_ALL_DRX_RX
*\brief 	This structure contains the FEA RS422 read status from DRx modules     
*/
typedef struct 
{
	U8BIT 	m_u8FEAReadSts;		/*!< Read WB DRx1 Status*/

}S_RFSAPR_RES_READ_FEARS422_ALL_DRX_RX;

/**
*\struct 	S_RFSAPR_CMD_WRITE_AUTS_VIDEO_TTL
*\brief 	This structure contains members to write pulse ON count and pulse OFF count into WB DRX1 module    
*/
typedef struct 
{
	U8BIT 	m_u8DRxBrdNo;		/*!<  DRx Board Number */
	U32BIT 	m_u32PulseONCount;	/*!<  Pulse ON Count*/
	U32BIT 	m_u32PulseOFFCount;	/*!<  Pulse OFF Count*/

}S_RFSAPR_CMD_WRITE_AUTS_VIDEO_TTL;;

/**
*\struct 	S_RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_BLANKING
*\brief 	This structure contains members to write data into NB DRX module    
*/
typedef struct 
{
	U8BIT 	m_u8NBDRx;		/*!< NB DRx  Board Number */
	U8BIT 	m_u8WriteData;	/*!< Data Write into WB DRx-1*/

}S_RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_BLANKING;

/**
*\struct 	S_RFSAPR_CMD_READ_ALLDRX_LOOK_THRU_BLANKING
*\brief 	This structure contains members to WB DRX1, WB DRX2 and NB DRX module board number   
*/
typedef struct 
{
	U8BIT 	m_u8DRxBrdNo;		/*!<  DRx Board Number */
	
}S_RFSAPR_CMD_READ_ALLDRX_LOOK_THRU_BLANKING;

/**
*\struct 	S_RFSAPR_RES_READ_ALLDRX_LOOK_THRU_BLANKING
*\brief 	This structure contains members to read look through blanking status from WB DRX1, WB DRX2 and NB DRX module    
*/
typedef struct 
{
	U8BIT m_ucRdBlankingJPod1Sts;
	U8BIT m_ucRdBlankingJPod2Sts;
	
}S_RFSAPR_RES_READ_ALLDRX_LOOK_THRU_BLANKING;

/**
*\struct 	S_RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_RADAR
*\brief 	This structure contains members to write data into NB DRX module   
*/
typedef struct 
{
	U8BIT 	m_u8NBDRx;		/*!< NB DRx  Board Number */
	U8BIT 	m_u8WriteData;	/*!< Data Write into WB DRx-1*/

}S_RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_RADAR;

/**
*\struct 	S_RFSAPR_CMD_READ_ALLDRX_LOOK_THRU_RADAR
*\brief 	This structure contains members to read data from WB DRX1, WB DRX2 and NB DRX module    
*/
typedef struct 
{
	U8BIT 	m_u8DRxBrdNo;		/*!< DRx Board Number */

}S_RFSAPR_CMD_READ_ALLDRX_LOOK_THRU_RADAR;

/**
*\struct 	S_RFSAPR_RES_READ_ALLDRX_LOOK_THRU_RADAR
*\brief 	This structure contains members to read data from WB DRX1, WB DRX2 and NB DRX module    
*/
typedef struct 
{
	U8BIT 	m_u8ReadlookThrRadarSts;	/*!< Data read from DRx module*/

}S_RFSAPR_RES_READ_ALLDRX_LOOK_THRU_RADAR;


/**
*\struct 	S_RFSAPR_CMD_READ_BLANKING_SIGNAL_PULSE
*\brief 	This structure contains members to configure the balnking channel in WB DRX1, WB DRX2 and NB DRX modules    
*/
typedef struct 
{
	U8BIT 	m_u8DRxBoardNo;		/*!< DRx's Board Number */
	U8BIT 	m_u8ChSel;		    /*!< DRx's Channel Selection */

}S_RFSAPR_CMD_READ_BLANKING_SIGNAL_PULSE;

/**
*\struct 	S_RFSAPR_RES_READ_BLANKING_SIGNAL_PULSE
*\brief 	This structure contains members to read pulse width and pulse period from WB DRX1, WB DRX2 and NB DRX modules    
*/
typedef struct 
{
	float 	m_fDRxReadPulsWidth;	/*!< Read Pulse Width Time from DRx*/
	float 	m_fDRxReadPulsePeriod;	 /*!< Read Pulse Period Time from DRx*/
	
}S_RFSAPR_RES_READ_BLANKING_SIGNAL_PULSE;

/**
*\struct 	S_RFSAPR_CMD_WRITE_OPEN_GND_CONTROLS_QT_TEST
*\brief 	This structure contains members to hold the Write SerVOut data in DRx's module
*/
typedef struct 
{
	U8BIT	m_u8DRxBrdNo; 		/*!< DRx's Board Number */
	U8BIT	m_u8SerOutDRxData; /*!< Write SerVOut data in DRx's module */

}S_RFSAPR_CMD_WRITE_OPEN_GND_CONTROLS_QT_TEST;

/**
*\struct 	S_RFSAPR_CMD_READ_OPEN_GND_CONTROLS_QT_TEST
*\brief 	This structure contains members to hold the DRx's modules broad numbers
*/
typedef struct 
{
	U8BIT	m_u8DRxBrdNo; 		/*!< DRx's Board Number */

}S_RFSAPR_CMD_READ_OPEN_GND_CONTROLS_DRX_QT_TEST;

/**
*\struct 	S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_QT_TEST
*\brief 	This structure contains members to hold the read open ground controls from DRx's modules
*/
typedef struct 
{
	U8BIT  m_u8ReadOpnGndSts;  /*!< It contains the System Recovety Status, Reset Status and Memory Erase status */
	
}S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_DRX_QT_TEST;

/**
*\struct 	S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_IOEXP_QT_TEST
*\brief 	This structure contains members to hold the read open ground controls from DRx's modules
*/
typedef struct 
{
	U8BIT  m_u8ReadIOExpOpnGndDisrtCntlSts1;  /*!< It contains Open Gnd Discrete Control signals status */
	U8BIT  m_u8ReadIOExpOpnGndDisrtCntlSts2;	/*!< It contains Open Gnd Discrete Control signals status */
	
}S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_IOEXP_QT_TEST;

/**
*\struct 	S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_XMC_QT_TEST
*\brief 	This structure contains members to hold the read open ground controls from DRx's modules
*/
typedef struct 
{
	U8BIT  m_u8ReadXMCOpnGndSts;	/*!< It contains the System Recovety Status*/
	
}S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_XMC_QT_TEST;

typedef struct
{
	unsigned long m_ulJPod1RdPulOnCnt;
	unsigned long m_ulJPod1RdPulOffCnt;
	unsigned long m_ulJPod2RdPulOnCnt;
	unsigned long m_ulJPod2RdPulOffCnt;

}S_RFSAPR_RES_READ_BLANKING_PULSE_ON_OFF_CNT;

/**
*\struct 	S_RFSAPR_WB_RES_READ_DIFF_BLANKING_PULSE_ON_OFF
*\brief 	This structure contains members to hold the read Differential blanking signals from DRx's modules
*/
typedef struct
{
	unsigned long m_ulRADARRdPulOnCnt;
	unsigned long m_ulRADARRdPulOffCnt;
	unsigned long m_ulStrobeRdPulOnCnt;
	unsigned long m_ulStrobeRdPulOffCnt;
	unsigned long m_ulDataLinkRdPulOnCnt;
	unsigned long m_ulDataLinkRdPulOffCnt;

}S_RFSAPR_RES_READ_DIFF_BLANKING_PULSE_ON_OFF;

/**
*\struct 	S_RFSAPR_RES_READ_DIFF_BLANKING_IO_DATA
*\brief 	This structure contains members to hold the read Differential blanking signals IO status from DRx's modules
*/
typedef struct
{
	unsigned char m_ucRADARRdSts;
	unsigned char m_ucStrobeRdSts;
	unsigned char m_ucDataLinkRdSts;

}S_RFSAPR_RES_READ_DIFF_BLANKING_IO_DATA;

typedef struct 
{
	unsigned char	m_ucBrdSel;	/*  1- WB , 2 - NB */
	unsigned char	m_ucBrdNo;	/*  WB: 0 - Board-1 ,1 - Board-2 , NB: 0 - Board-1 */
	unsigned long	m_RFPathSelData;	
	
}S_RFSAPR_CMD_CONFIG_IOMATRIX_RFPATH;

typedef struct 
{
	unsigned char	m_ucBrdSel;	/*  1- WB , 2 - NB */
	unsigned char	m_ucBrdNo;	/*  WB: 0 - Board-1 ,1 - Board-2 , NB: 0 - Board-1 */
	unsigned long	m_ulAttenData;	
	
}S_RFSAPR_CMD_CONFIG_IOMATRIX_ATTENUATION;


typedef struct 
{
	unsigned char	m_ucBrdSel;	/*  1- WB , 2 - NB */
	unsigned char	m_ucBrdNo;	/*  WB: 0 - Board-1 ,1 - Board-2 , NB: 0 - Board-1 */
	unsigned long	m_ulFreqData;	
	
}S_RFSAPR_CMD_CONFIG_IOMATRIX_BITE_FREQ;


typedef struct 
{
	unsigned long	m_ulFlashDataSize; /* in Bytes  */

}S_RFSAPR_CMD_SEND_FLASH_DATA_SIZE;


typedef struct 
{
	U8BIT	m_ucStart;
	U8BIT	m_arrucFlashData[MAX_FLASH_DATA];	/** 1 MB of data **/

}S_RFSAPR_CMD_SEND_FLASH_CONFIG_DATA;


typedef struct 
{
	unsigned char	m_ucBrdSel;	/*  1- WB , 2 - NB */
	unsigned char	m_FlashSel;
	unsigned char	m_ucBrdNo;	/*  WB: 0 - Board-1 ,1 - Board-2 ,NB: 0 - Board-1 */
	unsigned long	m_ulFlashAddrs;
	unsigned long	m_ulDataCnt; 
	unsigned char	m_arrucFlashData[FLASH_SECTOR_HALF_SIZE];	/** 128 Kbytes of data **/

}S_RFSAPR_CMD_FLASH_CONFIG_DATA;

typedef struct 
{
	unsigned char m_ucBrdSel;	/*  1- WB , 2 - NB */
	unsigned char m_ucBrdNo;	/*  WB: 0 - Board-1 ,1 - Board-2 ,NB: 0 - Board-1 */

}S_RFSAPR_CMD_WRITE_CONFIG_DATA_TO_FLASH;


typedef struct 
{
	char m_cFlashConfigSts; /* Flash configuration status  1- Still in process , 0 - Successfully configured , -1 - Failed due to error  */

}S_RFSAPR_RES_READ_FLASH_CONFIG_COMPLETION_STS;

typedef struct 
{
	unsigned char	m_ucBrdSel;	/*  1- WB , 2 - NB */
	unsigned char	m_ucBrdNo;	/*  WB: 0 - Board-1 ,1 - Board-2 , NB: 0 - Board-1 */
	unsigned char	m_ucFlashNo; /* 1 - storage flash , 2 - configuaration flash	*/
	unsigned char	m_ucFlashReadType;	/* 1 - Read the complete flash data , 2 - read particular sectors data*/
	unsigned long	m_ulFlashAdrss;
	unsigned long	m_ulNoofBytstoRead;	

}S_RFSAPR_CMD_FLASH_READ_DATA;

typedef struct 
{
	U8BIT			m_arrucFlashData[1048576];	/** 1 MB of Data **/
}S_RFSAPR_RES_FLASH_READ_DATA;


typedef struct 
{
	unsigned char	m_ucBrdSel;	/*  1- WB , 2 - NB */
	unsigned char	m_ucPathSel;
}S_RFSAPR_CMD_RF_IOMATRIX_CONFIG;

typedef struct 
{
	unsigned char	m_ucDCASel;	/*  1- DCA-1 , 2 - DCA-2 */
	unsigned char	m_ucDCAData;
	
}S_RFSAPR_CMD_ATTENUATION_IOMATRIX_CONFIG;


typedef struct 
{
	unsigned char m_ucFreqSel;	
}S_RFSAPR_CMD_BITE_IOMATRIX_CONFIG;

typedef struct 
{
	unsigned char	m_ucModuleSel;		/*! 1 - IO Matrix, 2 - NBDC-1 & NBBC -2 */
	unsigned char	m_ucNBDCUARTCh;		/*! 1 - UART CH 1 for NBDC1 , 2 - UART CH 2 for NBDC2*/
	unsigned int	m_uiData;
	unsigned long	m_ulCmdWrd;		/*! Command packet Command Data (0 - 255)*/
	
}S_RFSAPR_CMD_NBDC_IOMATRIX_UART_WRITE_DATA;

typedef struct 
{
	unsigned char	m_ucModuleSel;		/*! 1 - IO Matrix, 2 - NBDC-1 & NBBC -2 */
	unsigned long	m_ulCmdWrd;		/*! Command packet Command Data (0 - 255)*/
	unsigned char	m_ucNBDCUARTCh;		/*! 1 - UART CH 1 for NBDC1 , 2 - UART CH 2 for NBDC2*/
	
}S_RFSAPR_CMD_NBDC_IOMATRIX_UART_READ_DATA;

typedef struct 
{
	unsigned long	m_ulReadUARTData4Bytes;
	unsigned long	m_ulReadUARTData1Byte;
	
}S_RFSAPR_RES_NBDC_IOMATRIX_UART_READ_DATA;

typedef struct 
{
	unsigned char	m_ucBrdNo;	/*  WB: 0 - Board-1 ,1 - Board-2 , NB: 0 - Board-1 */
	
}S_RFSAPR_CMD_SBC_JTAG_SEL;

typedef struct
{
	unsigned char	m_ucModulesel;
	unsigned char	m_ucMsnNum;
	unsigned short	m_usFreqBWData;
	unsigned short	m_usRFDCA;
	unsigned short	m_usIFDCA;
	unsigned short	m_usPhasft;
	unsigned short	m_usPathandLo;	
	
}S_RFSAPR_CMD_CONFIGURE_NBBDC_RFBUS;

typedef struct 
{
	UCHAR m_ucNBDCSel;
	UCHAR m_ucMissionNo;
	UCHAR m_ucFreqBWUpdate;
	USHORT m_usFrequency;
	USHORT m_usBandwidth;
	UCHAR m_ucRFAttenUpdate;
	float m_fRFAtten;
	UCHAR m_ucIFAttenUpdate;
	float m_fIFAtten;
	UCHAR m_ucPhaseUpdate;
	float m_fPhaseShift;
	UCHAR m_ucLOPathUpdate;
	UCHAR m_ucPathsel;
	UCHAR m_ucLOSel;

}S_RFSAPR_CMD_NBBDC_CONFIG;



typedef struct 
{
	UCHAR ucModuleSel;
	UCHAR ucMissionNo;
}S_RFSAPR_CMD_MISSION_ENABLE;

typedef struct 
{
	UCHAR m_ucMissionNo;
	USHORT m_usFrequency;

}S_RFSAPR_CMD_NBFS;

typedef struct 
{
	unsigned long	m_ulReadData;
	
}S_RFSAPR_RES_NBFS;

typedef struct 
{
	unsigned long	m_ulReadData;
	
}S_RFSAPR_RES_NBBDC_CONFIG;

typedef struct 
{
	unsigned long	m_ulReadData;
	
}S_RFSAPR_RES_MISSION_ENABLE;


typedef struct
{

	unsigned char m_ucAntNo;
	double	m_dPolyconst[DP_RFSAPR_RF_ATTN_BAND_COUNT+DP_RFSAPR_RF_FREQ_BLOCK_ATTN_BAND_COUNT][3];
	
}S_RFSAPR_CMD_STORE_HW_CALIB_CONST;

typedef struct
{
	unsigned char m_ucAntNo;	
}S_RFSAPR_CMD_READ_HW_CALIB_CONST;

typedef struct
{

	unsigned char m_ucAntNo;
	double	m_dPolyconst[(DP_RFSAPR_RF_ATTN_BAND_COUNT+DP_RFSAPR_RF_FREQ_BLOCK_ATTN_BAND_COUNT)][3];
	
}S_RFSAPR_RES_READ_HW_CALIB_CONST;

/**
*\struct 	S_RFSAPR_CMD_SET_FEA_CTRL_OP
*\brief 	This structure contains members to set FEA control output
*/
typedef struct 
{
	 U16BIT 	m_u16FEACtrlData;		/*!< FEA Output Data */

}S_RFSAPR_CMD_SET_FEA_CTRL_OP;

/**
*\struct 	S_RFSAPR_CMD_DIFF_BLANK_READ
*\brief 	This structure contains members to Read differential blanking signal
*/
typedef struct 
{
	 U8BIT 	m_u8WBNBSel;		/*!<Wide Band or Narrow Band Selection */
	 U8BIT 	m_u8DRXNo;		/*!< DRx Number */

}S_RFSAPR_CMD_DIFF_BLANK_READ;

typedef struct 
{
	 U16BIT m_u16ReadIPData;		/*!< Read Data */
}S_RFSAPR_RES_DIFF_BLANK_READ;

/******* Added for the FAN On/Off control *********/
typedef struct
{
	unsigned char m_ucFanSel;		/* 1 - fan 1 , 2 - fan2 , 3 - Both*/	
	unsigned char m_ucFanOnOffsel; /* 1- On , 0 - Off */
	
}S_RFSAPR_CMD_FAN_ON_OFF_CONTROL;

/**
*\struct 	S_RFSAPR_CMD_CONFIG_DRX_SINENDED_DEBOUNCETIME
*\brief 	This structure contains members to configure the DRx modules Debounce time
*/
typedef struct 
{
	 U8BIT 	m_u8WBNBSel;	/*!<Wide Band or Narrow Band Selection */
	 U8BIT 	m_u8DRXNo;		/*!< DRx Number */
	 U16BIT m_u16DebounceTime;		/*!< Read Data */

}S_RFSAPR_CMD_CONFIG_DRX_SINENDED_DEBOUNCETIME;

typedef struct 
{
	unsigned char	m_ucAntNum;
	
}S_RFSAPR_CMD_READ_LOG_ADC_CALIB_DATA;

typedef struct
{

	float							m_arrfFreq_MHZ[DP_RFSAPR_RF_ATTN_BAND_COUNT];
	float							m_arrdCalcA[DP_RFSAPR_RF_ATTN_BAND_COUNT];
	float							m_arrdCalcB[DP_RFSAPR_RF_ATTN_BAND_COUNT];
	float							m_arrdCalcC[DP_RFSAPR_RF_ATTN_BAND_COUNT];
	unsigned char					m_ucAntNum;

}S_RFSAPR_LOG_ADC_MEASD_TABLE;


typedef struct 
{
	unsigned char					m_ucAntNum;
	S_RFSAPR_LOG_ADC_MEASD_TABLE	m_sRFSAPRLogCalibdata[3];

}S_RFSAPR_HW_LOG_TEMP_CALIB_DATA;

typedef struct 
{
	unsigned char					m_ucAntNum;

}S_RFSAPR_CMD_READ_LOGADC_TEMP_CALIB_DATA;


typedef struct
{
	unsigned char	m_ucAntennaNo;
}S_RFSAPR_CMD_NARROW_BAND_CALIB_DATA_READ;

typedef struct
{
	unsigned char	m_ucAntennaNo;
	float			m_farrCalibData[360];
}S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA;


typedef struct 
{
	unsigned char					m_ucAntNum;
	unsigned char					m_ucTempSel;
	S_RFSAPR_RF_GAIN_CALIB_TABLE	m_sRFSAPRcalibdata[3];

}S_RFSAPR_HW_TEMP_CALIB_DATA;

typedef struct 
{
	unsigned char					m_ucAntNum;

}S_RFSAPR_CMD_READ_WBRF_TEMP_CALIB_DATA;


typedef struct 
{
	unsigned long		m_WB1RDSts;
	unsigned long		m_WB2RDSts;
	unsigned long		m_NB1RDSts;

}S_RFSAPR_RES_READ_BLANKING_STS;


#pragma pack(pop)

#endif
