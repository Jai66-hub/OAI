// RFSAPR_TCPSocComm.cpp: implementation of the CRFSAPR_TCPSocComm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DP_ATE_8333_300.h"
#include "RFSAPR_TCPSocComm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRFSAPR_TCPSocComm::CRFSAPR_TCPSocComm()
{


}



CRFSAPR_TCPSocComm::~CRFSAPR_TCPSocComm()
{

}

/**************************************************************************
// Name						: RFSAPR_TCPCmdTrans
// Author					: E Srinivasu
// Global Variables affected: Nil
// Created Date				: 26 Nov 2017
// Revision Date			:
// Reason for Revising		:
// Description				: The routine used to send the RFSAPR Command to
							  RFSAPR System and receive the Response from RFSAPR System.
**************************************************************************/

short CRFSAPR_TCPSocComm::RFSAPR_TCPCmdTrans(SOCKET in_sktConnectSocket, 
							LPVOID pInputParam, 
							unsigned long in_ulInputParamLen, 
							LPVOID pOutputParam, 
							unsigned long *pOutputParamLen, 
							unsigned long in_ulTimeOut)
{
	
	MAINAPP(pMainApp);
	
	static S_RFSAPR_RX_TCP_PACKET	SRFSAPR_TxTcpPacket;
	static S_RFSAPR_TX_TCP_PACKET	SRFSAPR_RxTcpPacket;	
	
	static S_COMMAND_FROM_RFSAPR	m_SCommandFromRFSAPR;
	
	unsigned long ulTxPacketLength = RFSAPR_ATE_ZERO;
	unsigned short usid = 0;
	
	
	S_COMMAND_FROM_RFSAPR *pucInParam;
	S_RESPONSE_TO_RFSAPR  *pucOutParam;
	
	int iResult = RFSAPR_ATE_ZERO;
	short sRetVal = RFSAPR_ATE_FAILURE;
	DWORD dwWaitResult  = 0;
	
	static char cTransmitBuffer[RFSAPR_MAX_TCPMSG_SIZE]; 
	
	/************************************************************************************************************************/
	// Copy the Param Data to the message
	
	/******** Waiting for mutex **********/
#if 1	
	dwWaitResult = WaitForSingleObject(
		pMainApp->m_objMutexOnlineMonitor,    // Handle To Mutex
												120000); /** Max time to wait 1 Min , since time taken by online monitoring operation
	for reading and upating GUI **/
#endif	
	/************************************************************************************************************************/
	
	
	if(dwWaitResult == WAIT_OBJECT_0)
	{
		
		
		pucInParam = (S_COMMAND_FROM_RFSAPR *)pInputParam;
		
		usid =  pucInParam->m_SRFSAPRCmdIds.m_usGroupId;
		
		memset(&SRFSAPR_RxTcpPacket, 0, sizeof(SRFSAPR_RxTcpPacket));

#if 0
		SYSTEMTIME st1, st2;
		MAINAPP(pMainApp);
		GetLocalTime(&st1);

		sRetVal = RFSAPR_TCPRFSAPRResRev(in_sktConnectSocket, &SRFSAPR_RxTcpPacket,1);
		GetLocalTime(&st2);
		CString strTime = "";
		strTime.Format("%02d:%02d:%02d.%03d - %02d:%02d:%02d.%03d ", st1.wHour, st1.wMinute, st1.wSecond, st1.wMilliseconds, st2.wHour, st2.wMinute, st2.wSecond, st2.wMilliseconds);
		pMainApp->PrintLog(strTime, DP_RFSAPR_PRINTLOG_DEFAULT);
#endif
		memcpy(&m_SCommandFromRFSAPR.m_SRFSAPRCmdIds, &pucInParam->m_SRFSAPRCmdIds, DP_RFSAPR_MSGIDS_SIZE );
		memcpy(m_SCommandFromRFSAPR.m_UCmdRFSAPRMessage.m_ucRFSAPRRxData, pucInParam->m_UCmdRFSAPRMessage.m_ucRFSAPRRxData, in_ulInputParamLen);
		
		if(m_SCommandFromRFSAPR.m_SRFSAPRCmdIds.m_usUnitId) /* Group ID and Unit ID 0 for receiving Reports from WBSPU */
		{
			
			SRFSAPR_TxTcpPacket.m_ucTCPHeader[0]	=	0xAA;
			SRFSAPR_TxTcpPacket.m_ucTCPHeader[1]	=	0xAB;
			SRFSAPR_TxTcpPacket.m_ucTCPHeader[2]	=	0xBA;
			SRFSAPR_TxTcpPacket.m_ucTCPHeader[3]	=	0xBB;
			
			SRFSAPR_TxTcpPacket.m_ucTCPFooter[0]	=	0xCC;
			SRFSAPR_TxTcpPacket.m_ucTCPFooter[1]	=	0xCD;
			SRFSAPR_TxTcpPacket.m_ucTCPFooter[2]	=	0xDC;
			SRFSAPR_TxTcpPacket.m_ucTCPFooter[3]	=	0xDD;
			
			// Validate the input parameter length
			if(in_ulInputParamLen > RFSAPR_MAX_TCPMSG_SIZE)
			{
				ReleaseMutex(pMainApp->m_objMutexOnlineMonitor);
				return DP_RFSAPR_APP_INVALID_MSG_DATA_LENGTH;
			}
			
			if(in_ulTimeOut > DP_RFSAPR_CMD_MAX_TIMEOUT)
			{
				ReleaseMutex(pMainApp->m_objMutexOnlineMonitor);
				return DP_RFSAPR_APP_INVALID_TIMEOUT_VAL;
			}
			
			if(in_ulInputParamLen)
			{
				if(pInputParam == NULL)
				{
					ReleaseMutex(pMainApp->m_objMutexOnlineMonitor);
					return DP_RFSAPR_APP_NULL_PTR_ACCESS;
				}
			}
			
			SRFSAPR_TxTcpPacket.m_SCommandFromRFSAPR.m_SRFSAPRCmdIds.m_usGroupId = htons(m_SCommandFromRFSAPR.m_SRFSAPRCmdIds.m_usGroupId);
			
			SRFSAPR_TxTcpPacket.m_SCommandFromRFSAPR.m_SRFSAPRCmdIds.m_usUnitId = htons(m_SCommandFromRFSAPR.m_SRFSAPRCmdIds.m_usUnitId);
			
			SRFSAPR_TxTcpPacket.m_ulMessageLength = in_ulInputParamLen;
			
			memcpy(SRFSAPR_TxTcpPacket.m_SCommandFromRFSAPR.m_UCmdRFSAPRMessage.m_ucRFSAPRRxData,m_SCommandFromRFSAPR.m_UCmdRFSAPRMessage.m_ucRFSAPRRxData, in_ulInputParamLen);
			
			RFSAPR_FrameTCP_TXBuffer(cTransmitBuffer,&SRFSAPR_TxTcpPacket,&ulTxPacketLength);
			
			//EnterCriticalSection(&pMainApp->g_ObjETHCriticalSection);
			
			if(ulTxPacketLength > RFSAPR_MAX_TCP_STACK_SIZE)
			{
				unsigned long ulTxBufferCount = RFSAPR_ATE_ZERO; 
				unsigned long ulTransmitSize  = RFSAPR_ATE_ZERO;
				
				while(ulTxBufferCount < ulTxPacketLength)
				{
					
					if((ulTxBufferCount + RFSAPR_MAX_TCP_STACK_SIZE) <=  ulTxPacketLength)
					{
						ulTransmitSize = RFSAPR_MAX_TCP_STACK_SIZE;
						iResult = send( in_sktConnectSocket, &cTransmitBuffer[ulTxBufferCount], ulTransmitSize, 0 );					
					}
					else
					{
						ulTransmitSize = (ulTxPacketLength - ulTxBufferCount);
						iResult = send( in_sktConnectSocket, &cTransmitBuffer[ulTxBufferCount], ulTransmitSize, 0 );					
					}
					
					ulTxBufferCount += ulTransmitSize;
					if (iResult == SOCKET_ERROR) 
					{
						LeaveCriticalSection(&pMainApp->g_ObjETHCriticalSection);
						ReleaseMutex(pMainApp->m_objMutexOnlineMonitor);
						//	printf("send failed with error: %d\n", WSAGetLastError());
						closesocket(in_sktConnectSocket);
						/*WSACleanup(); // commented - since it terminates use of all the winsock dll  
						Mysql server connection also terminating and which disables database logging ( doubt - how they are interlinked ) */		
						return DP_RFSAPR_APP_ETH_TIMEOUT;
					}
				}
				
			}
			else
			{
				iResult = send( in_sktConnectSocket, cTransmitBuffer, ulTxPacketLength, 0 );
				if (iResult == SOCKET_ERROR) 
				{
					//	printf("send failed with error: %d\n", WSAGetLastError());
					ReleaseMutex(pMainApp->m_objMutexOnlineMonitor);
					closesocket(in_sktConnectSocket);
					//		WSACleanup();
					return DP_RFSAPR_APP_ETH_TIMEOUT;
				}
			}
		}
			
		sRetVal = RFSAPR_TCPRFSAPRResRev(in_sktConnectSocket, &SRFSAPR_RxTcpPacket,in_ulTimeOut);
		
		//LeaveCriticalSection(&pMainApp->g_ObjETHCriticalSection);
		if(sRetVal == RFSAPR_ATE_SUCCESS)
		{
			sRetVal = SRFSAPR_RxTcpPacket.m_sMsgStatus;
			
			*pOutputParamLen = SRFSAPR_RxTcpPacket.m_ulMessageLength;		
			
			//Copy the Param Data to the message
			pucOutParam = (S_RESPONSE_TO_RFSAPR *)pOutputParam;
			
			/*	CString strTemp;
			
			  strTemp.Format("SRFSAPR_RxTcpPacket.m_SResponseToRFSAPR.m_SRFSAPRResIds.m_usUnitId :%d",SRFSAPR_RxTcpPacket.m_SResponseToRFSAPR.m_SRFSAPRResIds.m_usUnitId);
			AfxMessageBox(strTemp); */
			
			memcpy(&pucOutParam->m_SRFSAPRResIds, &SRFSAPR_RxTcpPacket.m_SResponseToRFSAPR.m_SRFSAPRResIds, (DP_RFSAPR_MSGIDS_SIZE));
			memcpy(&pucOutParam->m_UResRFSAPRMessage.m_ucRFSAPRTxData, &SRFSAPR_RxTcpPacket.m_SResponseToRFSAPR.m_UResRFSAPRMessage.m_ucRFSAPRTxData,(*pOutputParamLen));
			/*	strTemp.Format("pucOutParam->m_SRFSAPRResIds.m_usUnitId :%d",pucOutParam->m_SRFSAPRResIds.m_usUnitId);
			AfxMessageBox(strTemp); */
			
		}
#if 1
		// Release Ownership Of The Mutex Object
		if(!ReleaseMutex(pMainApp->m_objMutexOnlineMonitor)) 
		{ 
			// Handle Error
			pMainApp->PrintLog("Unable to release the mutex for command transmission" ,2);
		}

		
	}
	else
	{
		pMainApp->PrintLog("Thread mutex synchronization is in progress - command processor" ,2);		
	}
#endif	
	
	return sRetVal;
	
}

/**************************************************************************
// Name						: RFSAPR_TCPRFSAPRResRev
// Author					: 
// Global Variables affected:
// Created Date			: 26 Nov 2017
// Revision Date			:
// Reason for Revising	:
// Description				: The routine used to receive the Response from RFSAPR System.
								
**************************************************************************/
short CRFSAPR_TCPSocComm::RFSAPR_TCPRFSAPRResRev(SOCKET in_sktConnectSocket, S_RFSAPR_TX_TCP_PACKET	*in_SRFSAPR_RxTcpPacket, unsigned long in_ulTimeOut)
{
	unsigned short usRXMessageStatus	= DP_RFSAPR_CMD_NOTRECED;
	unsigned char ucRxCmdState			= DP_RFSAPR_RX_HEADER_STATE;
	char cRXData                     = 0;
	unsigned short usCmdIndex        = 0;

	unsigned char ucTCPHeader[DP_RFSAPR_HEADER_SIZE];
	unsigned char ucTCPFooter[DP_RFSAPR_HEADER_SIZE];
 	char arrcRXMegLen[DP_RFSAPR_MSGLEN_SIZE];
	static char	cReceiveBuffer[RFSAPR_MAX_TCPMSG_SIZE];	

	short sMsgStatus = 0;
	short sReserved = 0;
	unsigned long ulMessageLen = 0;
	unsigned short usMessageId = 0;

	SYSTEMTIME st1, st2;
		MAINAPP(pMainApp);

	int iResult;
	CString str = "";

	unsigned long ulMessageWordCounter = 0;

	ucTCPHeader[0]	=	0xEE;
	ucTCPHeader[1]	=	0xEF;
	ucTCPHeader[2]	=	0xFE;
	ucTCPHeader[3]	=	0xFF;

	ucTCPFooter[0]	=	0xFF;
	ucTCPFooter[1]	=	0xFE;
	ucTCPFooter[2]	=	0xEF;
	ucTCPFooter[3]	=	0xEE;
	
	 //in_ulTimeOut
	
	while(usRXMessageStatus != DP_RFSAPR_CMD_RECEIVED)
	{		
		cRXData = 0;
		
		switch(ucRxCmdState)
		{
			case DP_RFSAPR_RX_HEADER_STATE:

				RFSAPR_SetTimeOut(in_sktConnectSocket,in_ulTimeOut);
			
				iResult = recv(in_sktConnectSocket, &cRXData, DP_RFSAPR_RX_BYTE_COUNT, 0);
				
				RFSAPR_SetTimeOut(in_sktConnectSocket,0);
				
				if (iResult == DP_RFSAPR_RX_BYTE_COUNT)
				{
							
					if((unsigned char)cRXData == ucTCPHeader[usCmdIndex])
					{
						usCmdIndex++;						
					}	
					else if((unsigned char)cRXData == ucTCPHeader[0])
					{
						usCmdIndex = 1;						
					}
					else
					{
						usCmdIndex = 0;						
					}

					if(usCmdIndex)
					{
						in_SRFSAPR_RxTcpPacket->m_ucTCPHeader[usCmdIndex-1] = (unsigned char)cRXData;
					}

					if(usCmdIndex >= DP_RFSAPR_HEADER_SIZE)
					{
						ucRxCmdState = DP_RFSAPR_RX_MSGSTS_STATE;
						usCmdIndex = 1;
					}
				}
				else
				{
					//printf("\n RFSAPR ARE RES - TIME OUT - HEADER\n");
					return DP_RFSAPR_APP_ETH_TIMEOUT;
				}
				iResult = 0;

				break;

			case DP_RFSAPR_RX_MSGSTS_STATE:

				sMsgStatus = 0;
				RFSAPR_SetTimeOut(in_sktConnectSocket,in_ulTimeOut);
				iResult = recv(in_sktConnectSocket, (char *)&sMsgStatus, sizeof(sMsgStatus), 0);
				RFSAPR_SetTimeOut(in_sktConnectSocket,0);
				
				if (iResult == sizeof(sMsgStatus))
				{
					in_SRFSAPR_RxTcpPacket->m_sMsgStatus = htons(sMsgStatus);
					ucRxCmdState = DP_RFSAPR_RX_MSGLEN_STATE;
					usCmdIndex   = 1;					
				}
				else
				{
					//printf("\n RFSAPR ARE RES - TIME OUT - STATUS\n");
					return DP_RFSAPR_APP_ETH_TIMEOUT;
				}

				RFSAPR_SetTimeOut(in_sktConnectSocket,in_ulTimeOut);
				iResult = recv(in_sktConnectSocket, (char *)&sReserved, sizeof(sReserved), 0);
				RFSAPR_SetTimeOut(in_sktConnectSocket,0);

				if (iResult == sizeof(sReserved))
				{
					in_SRFSAPR_RxTcpPacket->m_sReserved = htons(sReserved);
					ucRxCmdState = DP_RFSAPR_RX_MSGLEN_STATE;
					usCmdIndex = 1;					
				}
				else
				{
					//printf("\n RFSAPR ARE RES - TIME OUT - STATUS\n");
					return DP_RFSAPR_APP_ETH_TIMEOUT;
				}

				iResult = 0;
				break;



			case DP_RFSAPR_RX_MSGLEN_STATE:

				memset(arrcRXMegLen,0,DP_RFSAPR_MSGLEN_SIZE);
				
				ulMessageLen = 0;
				RFSAPR_SetTimeOut(in_sktConnectSocket,in_ulTimeOut);
				iResult = recv(in_sktConnectSocket, (char *)&ulMessageLen, DP_RFSAPR_MSGLEN_SIZE, 0);
				RFSAPR_SetTimeOut(in_sktConnectSocket,0);

				

				if (iResult == DP_RFSAPR_MSGLEN_SIZE)
				{
					in_SRFSAPR_RxTcpPacket->m_ulMessageLength = 0;
					in_SRFSAPR_RxTcpPacket->m_ulMessageLength = htonl(ulMessageLen);
					ucRxCmdState = DP_RFSAPR_RX_MSGID_STATE;							
					usCmdIndex = 1;
					
				}
				else
				{
					//printf("\n RFSAPR ARE RES - TIME OUT - MSGLEN\n");
					return DP_RFSAPR_APP_ETH_TIMEOUT;
				}
				iResult = 0;
				break;

			case DP_RFSAPR_RX_MSGID_STATE:

				usMessageId = 0;
				RFSAPR_SetTimeOut(in_sktConnectSocket,in_ulTimeOut);
				iResult = recv(in_sktConnectSocket, (char *)&usMessageId, sizeof(usMessageId), 0);
				RFSAPR_SetTimeOut(in_sktConnectSocket,0);
				

				if (iResult == sizeof(usMessageId))
				{
					if(usCmdIndex)
					{
						in_SRFSAPR_RxTcpPacket->m_SResponseToRFSAPR.m_SRFSAPRResIds.m_usGroupId = htons(usMessageId);
						usCmdIndex = 0;
					}
					else
					{
						in_SRFSAPR_RxTcpPacket->m_SResponseToRFSAPR.m_SRFSAPRResIds.m_usUnitId = htons(usMessageId);
						usCmdIndex = 0;

						if(in_SRFSAPR_RxTcpPacket->m_ulMessageLength)
						{
							ucRxCmdState = DP_RFSAPR_RX_MSGDATA_STATE;
						}
						else
						{
							ucRxCmdState = DP_RFSAPR_RX_FOOTER_STATE;
						}
					}				
				}
				else
				{
					//printf(" RFSAPR ATE RES - TIME OUT - MSGID\n");
					return DP_RFSAPR_APP_ETH_TIMEOUT;
				}
				break;

				

			case DP_RFSAPR_RX_MSGDATA_STATE:

				RFSAPR_SetTimeOut(in_sktConnectSocket,in_ulTimeOut);
				memset(&cReceiveBuffer, 0, sizeof(char) * in_SRFSAPR_RxTcpPacket->m_ulMessageLength);

				iResult = 0;
				ulMessageWordCounter = 0;				
				while(iResult != SOCKET_ERROR)
				{
					iResult = recv(in_sktConnectSocket, &cReceiveBuffer[ulMessageWordCounter], (in_SRFSAPR_RxTcpPacket->m_ulMessageLength - ulMessageWordCounter), 0);
					 
					

					if(iResult > 0)
					{
						ulMessageWordCounter += iResult;
						if(ulMessageWordCounter >= in_SRFSAPR_RxTcpPacket->m_ulMessageLength)						
						{
							break;
						}
					}
				}
				RFSAPR_SetTimeOut(in_sktConnectSocket,0);

				if(iResult == SOCKET_ERROR)
				{
					usRXMessageStatus = DP_RFSAPR_CMD_NOTRECED;
					ucRxCmdState = DP_RFSAPR_RX_HEADER_STATE;
					return DP_RFSAPR_APP_ETH_TIMEOUT;
					
				}

				cReceiveBuffer[ulMessageWordCounter] = '\0';

				if((unsigned long)ulMessageWordCounter == in_SRFSAPR_RxTcpPacket->m_ulMessageLength)
				{
					memcpy(&in_SRFSAPR_RxTcpPacket->m_SResponseToRFSAPR.m_UResRFSAPRMessage.m_ucRFSAPRTxData,(unsigned char*)cReceiveBuffer,in_SRFSAPR_RxTcpPacket->m_ulMessageLength);
					memset(cReceiveBuffer,0,in_SRFSAPR_RxTcpPacket->m_ulMessageLength);
					ucRxCmdState = DP_RFSAPR_RX_FOOTER_STATE;
					usCmdIndex = 0;
				}
				else
				{
					OutputDebugString("Recv Timeout");
					//printf("\n RFSAPR ATE RES - TIME OUT - MSGDATA\n");
					usRXMessageStatus = DP_RFSAPR_CMD_NOTRECED;
					ucRxCmdState = DP_RFSAPR_RX_HEADER_STATE;
					return DP_RFSAPR_APP_ETH_TIMEOUT;
				}
				iResult = 0;
				break;
			
			case DP_RFSAPR_RX_FOOTER_STATE:

				RFSAPR_SetTimeOut(in_sktConnectSocket,in_ulTimeOut);
				iResult = recv(in_sktConnectSocket, &cRXData, DP_RFSAPR_RX_BYTE_COUNT, 0);
				RFSAPR_SetTimeOut(in_sktConnectSocket,0);
				
				
				if (iResult == DP_RFSAPR_RX_BYTE_COUNT)
				{
		
					//printf("Bytes Received: %d\n", iResult);
					
					if((unsigned char)cRXData == ucTCPFooter[usCmdIndex])
					{
						usCmdIndex++;						
					}	
					else if((unsigned char)cRXData == ucTCPFooter[0])
					{
						usCmdIndex = 1;						
					}
					else
					{
						usCmdIndex = 0;						
					}

					if(usCmdIndex)
					{
						in_SRFSAPR_RxTcpPacket->m_ucTCPFooter[usCmdIndex-1] = (unsigned char)cRXData;
					}

					if(usCmdIndex >= DP_RFSAPR_FOOTER_SIZE)
					{
						ucRxCmdState = DP_RFSAPR_RX_HEADER_STATE;
						usRXMessageStatus = DP_RFSAPR_CMD_RECEIVED;
						usCmdIndex = 0;
					}
				}
				else
				{
					//printf("\n RFSAPR ATE RES - TIME OUT - FOOTER\n");
					return DP_RFSAPR_APP_ETH_TIMEOUT;
				}
				iResult = 0;
				break;

			default:
				break;
		}
	
	}

	return RFSAPR_ATE_SUCCESS;
}


/**************************************************************************
// Name						: RFSAPR_FrameTCP_TXBuffer
// Author					: 
// Global Variables affected:
// Created Date				: 26 Nov 2017
// Revision Date			:
// Reason for Revising		:
// Description				: The routine used frame the RFSAPR
							  communication buffer.
**************************************************************************/
short CRFSAPR_TCPSocComm::RFSAPR_FrameTCP_TXBuffer(char *cTransmitBuffer, S_RFSAPR_RX_TCP_PACKET	*SRFSAPR_TxTcpPacket, unsigned long *ulTxPacketLength)
{

	unsigned long ulBufferIndex   = RFSAPR_ATE_ZERO;
	unsigned long ulMessageLength = RFSAPR_ATE_ZERO;
	

	
	ulMessageLength = SRFSAPR_TxTcpPacket->m_ulMessageLength;	

	SRFSAPR_TxTcpPacket->m_ulMessageLength = htonl(SRFSAPR_TxTcpPacket->m_ulMessageLength);

	memcpy(&cTransmitBuffer[ulBufferIndex],&SRFSAPR_TxTcpPacket->m_ucTCPHeader,DP_RFSAPR_HEADER_SIZE);
		
	ulBufferIndex  = ulBufferIndex + DP_RFSAPR_HEADER_SIZE;

	memcpy(&cTransmitBuffer[ulBufferIndex],&SRFSAPR_TxTcpPacket->m_ulMessageLength,DP_RFSAPR_MSGLEN_SIZE);

	ulBufferIndex  = ulBufferIndex + DP_RFSAPR_MSGLEN_SIZE;

	memcpy(&cTransmitBuffer[ulBufferIndex],&SRFSAPR_TxTcpPacket->m_SCommandFromRFSAPR.m_SRFSAPRCmdIds,sizeof(S_RFSAPR_IDS));

	ulBufferIndex  = ulBufferIndex + sizeof(S_RFSAPR_IDS);

	memcpy(&cTransmitBuffer[ulBufferIndex],&SRFSAPR_TxTcpPacket->m_SCommandFromRFSAPR.m_UCmdRFSAPRMessage.m_ucRFSAPRRxData,ulMessageLength);

	ulBufferIndex  = ulBufferIndex + ulMessageLength;


	memcpy(&cTransmitBuffer[ulBufferIndex],&SRFSAPR_TxTcpPacket->m_ucTCPFooter,DP_RFSAPR_FOOTER_SIZE);	

	ulBufferIndex  = ulBufferIndex + DP_RFSAPR_FOOTER_SIZE;

	*ulTxPacketLength = ulBufferIndex;

	return RFSAPR_ATE_SUCCESS;
}

/**************************************************************************
// Name						: RFSAPR_SetTimeOut
// Author					:
// Global Variables affected:
// Created Date				: 26 Nov 2017
// Revision Date			:
// Reason for Revising		:
// Description				: The routine used set the Time Out for receive
**************************************************************************/

short CRFSAPR_TCPSocComm::RFSAPR_SetTimeOut(SOCKET in_sktConnectSocket,unsigned long in_ulMilliSeconds)
{
	setsockopt(in_sktConnectSocket, SOL_SOCKET, SO_RCVTIMEO,(const char *)&in_ulMilliSeconds, sizeof(int));

	return RFSAPR_ATE_SUCCESS;
}

/**************************************************************************
// Name						: RFSAPR_ClearTCPRXBuffer
// Author					: 
// Global Variables affected:
// Created Date				: 26 Nov 2017
// Revision Date			:
// Reason for Revising		:
// Description				: The routine used clear the receiver Buffer
**************************************************************************/

short CRFSAPR_TCPSocComm::RFSAPR_ClearTCPRXBuffer(SOCKET in_sktConnectSocket)
{

	char cClearTcpRxBuffer[512];
	int iResult = 0;

	RFSAPR_SetTimeOut(in_sktConnectSocket,1000);
	do
	{
		iResult = recv(in_sktConnectSocket, cClearTcpRxBuffer, 512, 0);

	}while(iResult != -1);


	RFSAPR_SetTimeOut(in_sktConnectSocket,0);

	return RFSAPR_ATE_SUCCESS;
}