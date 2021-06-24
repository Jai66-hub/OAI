// RFSAPR_TCPSocComm.h: interface for the CRFSAPR_TCPSocComm class.
//
//////////////////////////////////////////////////////////////////////
/**
 *	\File		  RFSAPR_TCPSocComm.h
 *	\Date		  26 Nov 2017 
 *  \Author		  
 *  \Brief		  This file contains Structure and function prototypes of the Ethernet communication of RFSAPR LRU
 *	\Copyright	  (C)2016 - Data Patterns (India) Pvt. Ltd. 
 *				  #19, Arya Gowder Road 
 *				  West Mambalam, Chennai, INDIA - 600033 
 *				  Phone : +91 44 2483 7460 
 *				  Fax   : +91 44 2484 8703 
 *				  Email : support@datapatterns.co.in 
 *				  Web	: www.datapatternsindia.com 
 */

/*==========================================================================================*/

#ifndef _RFSAPR_TCPSOCCOMM_H_
#define _RFSAPR_TCPSOCCOMM_H_


class CRFSAPR_TCPSocComm
{

public:

	CRFSAPR_TCPSocComm();
	~CRFSAPR_TCPSocComm();

	short RFSAPR_TCPCmdTrans(SOCKET in_sktConnectSocket, 
							   LPVOID pInputParam, 
							   unsigned long in_ulInputParamLen, 
							   LPVOID pOutputParam, 
							   unsigned long *pOutputParamLen, 
							   unsigned long in_ulTimeOut);
	short RFSAPR_TCPRFSAPRResRev(SOCKET in_sktConnectSocket, S_RFSAPR_TX_TCP_PACKET	*in_SRFSAPR_RxTcpPacket, unsigned long in_ulTimeOut);
	short RFSAPR_FrameTCP_TXBuffer(char *cTransmitBuffer, S_RFSAPR_RX_TCP_PACKET	*SRFSAPR_TxTcpPacket, unsigned long *ulTxPacketLength);
	short RFSAPR_SetTimeOut(SOCKET in_sktConnectSocket,unsigned long in_ulMilliSeconds);
	short RFSAPR_ClearTCPRXBuffer(SOCKET in_sktConnectSocket);
public:

};
#endif