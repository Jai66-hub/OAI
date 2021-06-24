/**
 *	\file		  DP_RFSAPR_Cmd_Processor.c
 *	\date		  26-FEB-2016 
 *  \author		  Jayaram D 
 *  \brief		  This file is used to Handle all the RFSAPR commands received from RFSAPR GUI controller application(Host PC).
 *	\copyright	  (C)2016 - Data Patterns (India) Pvt. Ltd. 
 *				  #19, Arya Gowder Road 
 *				  West Mambalam, Chennai, INDIA - 600033 
 *				  Phone : +91 44 2483 7460 
 *				  Fax   : +91 44 2484 8703 
 *				  Email : support@datapatterns.co.in 
 *				  Web	: www.datapatternsindia.com 
 */

/*==========================================================================================*/

#include ".././INCLUDE/DP_RFSAPR_Main.h"

#include <netinet/tcp.h>

/*==========================================================================================*/
/*										GLOBAL HANDLE										*/
/*==========================================================================================*/
extern S_RFSAPR_GLOBAL_HANDLE g_RFSAPR_Handle;
/*------------------------------------------------------------------------------------------*/


/*! 
 *	\function		DP_RFSAPR_ThreadRFSAPRServer()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function start the RFSAPR Server Task Thread.
 *	\param[in]		None 
 *	\param[out]		None
 *	\return			DP_RFSAPR_SUCCESS
 */
short DP_RFSAPR_ThreadRFSAPRServer() 
{	
	return dp_rfsapr_create_thread(&g_RFSAPR_Handle.g_hRFSAPRServerTaskId,(void *) &DP_RFSAPR_ThreadRFSAPRServerTask, "RFSAPR_Server",DP_RFSAPR_SOCKET_TASK_PRIORITY, DP_RFSAPR_SKIP_HYPER_THREAD_OPTIMIZATION);
}

/*! 
 *	\function		DP_RFSAPR_OnlineStatusServer()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function start the RFSAPR Online status.
 *	\param[in]		None 
 *	\param[out]		None
 *	\return			DP_RFSAPR_SUCCESS
 */
short DP_RFSAPR_OnlineStatusServer()
{

		return dp_rfsapr_create_thread(&g_RFSAPR_Handle.g_hOnlineStsTaskId,
					(void *) &DP_RFSAPR_TaskOnlineSts, "DP_RFSAPR_TaskOnlineSts",
					DP_RFSAPR_SOCKET_TASK_PRIORITY, DP_RFSAPR_SKIP_HYPER_THREAD_OPTIMIZATION);

	return DP_RFSAPR_SUCCESS;
		
}		


/*! 
 *	\function		DP_RFSAPR_TaskOnlineSts()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function start the RFSAPR Online Monitoring Server application and handles the client GUI Online Status requests.
 *	\param[in]		None 
 *	\param[out]		None
 *	\return			DP_RFSAPR_SUCCESS
 */
short DP_RFSAPR_TaskOnlineSts() 
{   
	int iResult = 0;

	short sRetVal = DP_RFSAPR_SUCCESS;

	struct sockaddr_in saSockAddr;
	struct sockaddr_in saReceiveAddr;

	socklen_t iReceiveAddrSize = sizeof(saReceiveAddr);
	
	GET_TCPRXCOMM_HANDLE(pSCommandFromRFSAPR);
	
	char cRXData;
	int iflag = 1;

	int iSendBufferSize = RFSAPR_ONLINE_MAX_TCPMSG_SIZE;	

	unsigned long ulRxMessageLen = 0;
	unsigned short usMessageId = 0;
	unsigned long	ulRxByteCount = 0;
	unsigned long	ulMessageLength = 0;	
	int iSocReuse = 0;	

	int iKeepAliveTime				= DP_RFSAPR_KEEPALIVE_TIME;
	int iKeepAliveCount				= DP_RFSAPR_KEEPALIVE_COUNT;
	int iKeepAliveInterval			= DP_RFSAPR_KEEPALIVE_INTVL;

	S_RFSAPR_TEMPERATURE_MEASUREMENTS  sReadTempDisp;
	unsigned long ulTxPacketLength 	= 0;
	unsigned int uiElapseTime 	= 0;
	unsigned int uiETIEvtCnt	= 0;
	int iSendResult				= 0;


	g_RFSAPR_Handle.bOnlineClientStatus = 0; 

	g_RFSAPR_Handle.ucOnlineTCPHeader[0]	=	0xAA;
	g_RFSAPR_Handle.ucOnlineTCPHeader[1]	=	0xAB;
	g_RFSAPR_Handle.ucOnlineTCPHeader[2]	=	0xBA;
	g_RFSAPR_Handle.ucOnlineTCPHeader[3]	=	0xBB;

	g_RFSAPR_Handle.ucOnlineTCPFooter[0]	=	0xCC;
	g_RFSAPR_Handle.ucOnlineTCPFooter[1]	=	0xCD;
	g_RFSAPR_Handle.ucOnlineTCPFooter[2]	=	0xDC;
	g_RFSAPR_Handle.ucOnlineTCPFooter[3]	=	0xDD;

	g_RFSAPR_Handle.ucOnlineRxCmdState	=	RFSAPR_RX_HEADER_STATE;

	g_RFSAPR_Handle.usOnlineRXMessageStatus = RFSAPR_CMD_NOTRECED;

	/****************************************************/

	memset(&saSockAddr,0,sizeof(saSockAddr));
	saSockAddr.sin_family		=	AF_INET;
	saSockAddr.sin_addr.s_addr	=	INADDR_ANY; 
	saSockAddr.sin_port			=	htons(DEFAULT_ONLINE_PORT);

	/* Create a SOCKET for connecting to server*/
	g_RFSAPR_Handle.sktOnlnLsnSocket = dp_rfsapr_socket(saSockAddr.sin_family, SOCK_STREAM, 0);
	if (g_RFSAPR_Handle.sktOnlnLsnSocket == INVALID_SOCKET) 
	{
		dp_rfsapr_printf("ERROR\t: Listen socket creation failed\r\n");        
		return 1;
	}
	/*To Avoid Listen socket bind failed error. Socket port reuse option is added*/
	iSocReuse = 1; /*SOC_REUSE_ENABLE*/

	iResult = dp_rfsapr_setsockopt( g_RFSAPR_Handle.sktOnlnLsnSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&iSocReuse, sizeof(iSocReuse));
	if (iResult < 0)
	{
		dp_rfsapr_printf("ERROR\t: SO_REUSEADDR\r\n ");
	}

	/* Set-up the TCP listening socket*/
	iResult = dp_rfsapr_bind( g_RFSAPR_Handle.sktOnlnLsnSocket, (struct sockaddr*)&saSockAddr, (int)sizeof(saSockAddr));
	if (iResult == SOCKET_ERROR) 
	{
		dp_rfsapr_printf("ERROR\t: Listen socket bind failed\r\n");
		close(g_RFSAPR_Handle.sktOnlnLsnSocket);	
		return 1;
	}

	while(1)
	{		
		iResult = dp_rfsapr_listen(g_RFSAPR_Handle.sktOnlnLsnSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("ERROR\t: Listen Failed\r\n");
			close(g_RFSAPR_Handle.sktOnlnLsnSocket);	
			return 1;
		}

		/* Accept a client socket*/
		g_RFSAPR_Handle.sktOnlnCltSocket = dp_rfsapr_accept(g_RFSAPR_Handle.sktOnlnLsnSocket,(struct sockaddr*)&saReceiveAddr,&iReceiveAddrSize);
		if (g_RFSAPR_Handle.sktOnlnCltSocket == INVALID_SOCKET) 
		{

			dp_rfsapr_printf("ERROR\t: Accept Failed\r\n");
			close(g_RFSAPR_Handle.sktOnlnLsnSocket);

			return 1;
		}

		dp_rfsapr_printf("INFO\t: Online Monitoring Connection Established\r\n");

		/* Send buffer size */
		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktOnlnCltSocket,SOL_SOCKET,SO_SNDBUF, (char *)&iSendBufferSize,sizeof (iSendBufferSize));
		if (iResult < 0)
		{
			dp_rfsapr_printf("ERROR\t: SO_SNDBUF \r\n");
		}

		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktOnlnCltSocket,           /* socket affected */
				IPPROTO_TCP,							/* set option at TCP level */
				0x0001,								/* TCP_NODELAY = 0x0001 name of option */
				(char *) &iflag,						/* the cast is historical crust */
				sizeof(int));							/* length of option value */

		if (iResult < 0)
		{
			dp_rfsapr_printf("ERROR\t: TCP_NODELAY \r\n");
		}

		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktOnlnCltSocket, SOL_SOCKET, SO_KEEPALIVE,(char *)&iflag,sizeof(iflag));	
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("ERROR\t: SO_KEEPALIVE \r\n");
		}		

		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktOnlnCltSocket, IPPROTO_TCP, TCP_KEEPIDLE, (char *)&iKeepAliveTime, sizeof(iKeepAliveTime));
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("\n\rERROR\t: KEEPALIVE_TIME");
		}

		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktOnlnCltSocket, IPPROTO_TCP, TCP_KEEPCNT, (char *)&iKeepAliveCount, sizeof(iKeepAliveCount));
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("\n\rERROR\t: KEEPALIVE_INTVL");
		}
		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktOnlnCltSocket, IPPROTO_TCP, TCP_KEEPINTVL, (char *)&iKeepAliveInterval, sizeof(iKeepAliveInterval));
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("\n\rERROR\t: KEEPALIVE_INTVL");
		}

		/*Reset the Flag*/
		g_RFSAPR_Handle.usOnlineSockErrExitFlag = 0;

		g_RFSAPR_Handle.ulOnlineSocketState	=	1;		

		/* Receive Client commands */
		while(1)
		{

			switch(g_RFSAPR_Handle.ucOnlineRxCmdState)
			{
			case RFSAPR_RX_HEADER_STATE:

				DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktOnlnCltSocket,SO_RCVTIMEO,RFSAPR_SOCKET_TIME_OUT_VALUE);				

				iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktOnlnCltSocket, &cRXData, RFSAPR_RX_BYTE_COUNT, 0);					

				if (iResult == RFSAPR_RX_BYTE_COUNT)
				{	

					if((unsigned char)cRXData == g_RFSAPR_Handle.ucOnlineTCPHeader[g_RFSAPR_Handle.usOnlineCmdIndex])
					{
						g_RFSAPR_Handle.usOnlineCmdIndex++;						
					}	
					else if((unsigned char)cRXData == g_RFSAPR_Handle.ucOnlineTCPHeader[0])
					{
						g_RFSAPR_Handle.usOnlineCmdIndex = 1;						
					}
					else
					{
						g_RFSAPR_Handle.usOnlineCmdIndex = 0;						
					}

					if(g_RFSAPR_Handle.usOnlineCmdIndex)
					{
						g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.ucTCPHeader[g_RFSAPR_Handle.usOnlineCmdIndex-1] = (unsigned char)cRXData;
					}

					if(g_RFSAPR_Handle.usOnlineCmdIndex >= RFSAPR_HEADER_SIZE)
					{
						g_RFSAPR_Handle.ucOnlineRxCmdState = RFSAPR_RX_MSGLEN_STATE;
						g_RFSAPR_Handle.usOnlineCmdIndex = 1;
					}
				}

				break;

			case RFSAPR_RX_MSGLEN_STATE:

				ulRxMessageLen = 0;

				DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktOnlnCltSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);

				iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktOnlnCltSocket, (char *)&ulRxMessageLen, RFSAPR_MSGLEN_SIZE, 0);

				if (iResult == RFSAPR_MSGLEN_SIZE)
				{

					g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.ulMessageLength = htonl(ulRxMessageLen);
					g_RFSAPR_Handle.ucOnlineRxCmdState = RFSAPR_RX_MSGID_STATE;						
				}
				break;

			case RFSAPR_RX_MSGID_STATE:

				usMessageId = 0;

				DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktOnlnCltSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);

				iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktOnlnCltSocket, (char *)&usMessageId, sizeof(usMessageId), 0);			

				if (iResult == sizeof(usMessageId))
				{

					if(g_RFSAPR_Handle.usOnlineCmdIndex)
					{
						g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usGroupId  = htons(usMessageId);
						g_RFSAPR_Handle.usOnlineCmdIndex = 0;
					}
					else
					{
						g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usUnitId  = htons(usMessageId);
						g_RFSAPR_Handle.usOnlineCmdIndex = 0;


						if(g_RFSAPR_Handle.ucOnlinePrintLogState	==	RFSAPR_ENABLE_PRINT_LOGS)
						{
							dp_rfsapr_printf("------------------------------------------------------------------\r\n"); 
							dp_rfsapr_printf("[%d-%d] Received Bytes : %d\r\n ", g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usGroupId, g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usUnitId, g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.ulMessageLength);
							dp_rfsapr_printf("------------------------------------------------------------------\r\n"); 
						}

						if(g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.ulMessageLength != 0)
						{
							g_RFSAPR_Handle.ucOnlineRxCmdState = RFSAPR_RX_MSGDATA_STATE;
						}
						else
						{
							g_RFSAPR_Handle.ucOnlineRxCmdState = RFSAPR_RX_FOOTER_STATE;
						}
					}
				}
				break;

			case RFSAPR_RX_MSGDATA_STATE:

				DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktOnlnCltSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);

				ulRxByteCount = 0;
				ulMessageLength	=	g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.ulMessageLength;

				while(ulMessageLength > 0)
				{
					if(ulMessageLength > RFSAPR_MAX_RX_TCPMSG_SIZE)
					{
						iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktOnlnCltSocket, &g_RFSAPR_Handle.cOnlineReceiveBuffer[ulRxByteCount], RFSAPR_ONLINE_MAX_TCPMSG_SIZE, 0);					

					}
					else
					{
						iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktOnlnCltSocket, &g_RFSAPR_Handle.cOnlineReceiveBuffer[ulRxByteCount], ulMessageLength, 0);					
					}

					ulMessageLength = ulMessageLength - iResult;
					ulRxByteCount = ulRxByteCount + iResult;
				}

				g_RFSAPR_Handle.cOnlineReceiveBuffer[ulRxByteCount] = '\0';

				if(ulRxByteCount == g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.ulMessageLength)
				{
					memcpy(&g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.SCommandFromRFSAPR.UCmdRFSAPRMessage.ucRFSAPRRxData,(unsigned char*)g_RFSAPR_Handle.cOnlineReceiveBuffer,g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.ulMessageLength);

					g_RFSAPR_Handle.ucOnlineRxCmdState = RFSAPR_RX_FOOTER_STATE;
					g_RFSAPR_Handle.usOnlineCmdIndex = 0;
				}
				else
				{
					g_RFSAPR_Handle.usOnlineRXMessageStatus = RFSAPR_CMD_NOTRECED;
					g_RFSAPR_Handle.ucOnlineRxCmdState = RFSAPR_RX_HEADER_STATE;
				}

				break;

			case RFSAPR_RX_FOOTER_STATE:

				DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktOnlnCltSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);
				iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktOnlnCltSocket, &cRXData, RFSAPR_RX_BYTE_COUNT, 0);
				DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktOnlnCltSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);

				if (iResult == RFSAPR_RX_BYTE_COUNT)
				{
					if((unsigned char)cRXData == g_RFSAPR_Handle.ucOnlineTCPFooter[g_RFSAPR_Handle.usOnlineCmdIndex])
					{
						g_RFSAPR_Handle.usOnlineCmdIndex++;						
					}	
					else if((unsigned char)cRXData == g_RFSAPR_Handle.ucOnlineTCPFooter[0])
					{
						g_RFSAPR_Handle.usOnlineCmdIndex = 1;						
					}
					else
					{
						g_RFSAPR_Handle.usOnlineCmdIndex = 0;						
					}

					if(g_RFSAPR_Handle.usOnlineCmdIndex)
					{
						g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.ucTCPFooter[g_RFSAPR_Handle.usOnlineCmdIndex-1] = (unsigned char)cRXData;
					}

					if(g_RFSAPR_Handle.usOnlineCmdIndex >= RFSAPR_FOOTER_SIZE)
					{
						g_RFSAPR_Handle.ucOnlineRxCmdState = RFSAPR_RX_HEADER_STATE;
						g_RFSAPR_Handle.usOnlineRXMessageStatus = RFSAPR_CMD_RECEIVED;
						g_RFSAPR_Handle.usOnlineCmdIndex = 0;
					}
				}		

				break;

			default:
				break;
			}

			if(iResult == 0)
			{
				break;
			}

			if(g_RFSAPR_Handle.usOnlineRXMessageStatus == RFSAPR_CMD_RECEIVED)
			{
				if(g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usUnitId == RFSAPR_CMD_READ_SENSOR_DATA)
				{

					//dp_rfsapr_printf("Temp sensor\n");
					
					memset(&sReadTempDisp, 0, sizeof(S_RFSAPR_TEMPERATURE_MEASUREMENTS));

					sRetVal = DP_RFSAPR_Get_ElapsedTime(&uiElapseTime, &uiETIEvtCnt);
					if(sRetVal)
					{
						dp_rfsapr_printf("Unable to Read the Elapsed Time Value\r\n");				
					}			

					sRetVal = DP_RFSAPR_Temperature_Sensor_Read(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_NB_Voltage,&sReadTempDisp);	
					if(sRetVal)
					{
						dp_rfsapr_printf("Unable to Read the Temperature Sensor Values\r\n");				
					}

					sReadTempDisp.uiElapseTime = uiElapseTime;
					sReadTempDisp.uiETIEvtCnt = uiETIEvtCnt;

					DP_RFSAPR_ComposeOnlineResponse(sRetVal,sizeof(sReadTempDisp));

					g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usGroupId = g_RFSAPR_Handle.S_RFSAPR_OnlineRxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usGroupId;
					g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_SENSOR_DATA;

					memcpy(g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.UResRFSAPRMessage.ucRFSAPRTxData, &sReadTempDisp, sizeof(sReadTempDisp));

					sRetVal = DP_RFSAPR_FrameTCP_OnlineTXBuffer(&g_RFSAPR_Handle.ulOnlineTxPacketLength);

					ulTxPacketLength = 0;

					while(ulTxPacketLength < g_RFSAPR_Handle.ulOnlineTxPacketLength)
					{
						if((g_RFSAPR_Handle.ulOnlineTxPacketLength -  ulTxPacketLength) > RFSAPR_MAX_TCP_STACK_SIZE)
						{
							iSendResult = dp_rfsapr_send( g_RFSAPR_Handle.sktOnlnCltSocket, (g_RFSAPR_Handle.cOnlineTransmitBuffer + ulTxPacketLength), RFSAPR_MAX_TCP_STACK_SIZE, 0 );						
						}
						else
						{
							iSendResult = dp_rfsapr_send( g_RFSAPR_Handle.sktOnlnCltSocket, (g_RFSAPR_Handle.cOnlineTransmitBuffer + ulTxPacketLength), (g_RFSAPR_Handle.ulOnlineTxPacketLength -  ulTxPacketLength), 0 );
						}

						if (iSendResult == SOCKET_ERROR) 
						{						
							break;
						}
						else
						{
							ulTxPacketLength += iSendResult;

						}
					}

				}


				if (iSendResult == SOCKET_ERROR) 
				{	
					dp_rfsapr_printf("ERROR\t: Client Disconnected. Data Send Failed \r\n");

					dp_rfsapr_printf("ERROR\t: Send failed : %d\r\n",sRetVal);
					close(g_RFSAPR_Handle.sktOnlnCltSocket);
					break;
				}

				g_RFSAPR_Handle.usOnlineRXMessageStatus =	RFSAPR_CMD_NOTRECED;
			}	

			/* In vxworks if the client connection is closed then the recv function returns either 0 mostly or ECONNRESET and
			 * it is not breaking in while (1) and not going to listen. To avoid this following condition is added. 
			 */
			if(errno == ECONNRESET)
			{
				break;
			}
		}	

	}


	/* No longer need server socket*/

	dp_rfsapr_close(g_RFSAPR_Handle.sktOnlnLsnSocket);


	/* shut-down the connection since we're done*/
	iResult = dp_rfsapr_shutdown(g_RFSAPR_Handle.sktOnlnCltSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) 
	{
		dp_rfsapr_printf("ERROR\t: Shutdown Failed...\r\n");
		dp_rfsapr_close(g_RFSAPR_Handle.sktOnlnCltSocket);

		return 1;
	}	


	/*clean-up*/

	dp_rfsapr_close(g_RFSAPR_Handle.sktOnlnCltSocket);

	dp_rfsapr_thread_exit(&g_RFSAPR_Handle.g_hOnlineStsTaskId);

	return DP_RFSAPR_SUCCESS;
}

/*! 
 *	\function		DP_RFSAPR_ThreadRFSAPRServerTask()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function start the RFSAPR Server application and handles the client GUI requests.
 *	\param[in]		None 
 *	\param[out]		None
 *	\return			DP_RFSAPR_SUCCESS
 */
short DP_RFSAPR_ThreadRFSAPRServerTask() 
{   
    int iResult = 0;
	short sRetVal = DP_RFSAPR_SUCCESS;
	struct sockaddr_in saSockAddr;
	struct sockaddr_in saReceiveAddr;
	
	socklen_t iReceiveAddrSize = sizeof(saReceiveAddr);
		
	char cRXData;
	int iflag = 1;
	
	int iSendBufferSize = RFSAPR_MAX_TCPMSG_SIZE;	
	
	/*int RcvBuffSize =  RFSAPR_MAX_TCP_STACK_SIZE;*/
	int iRcvBuffSize =  RFSAPR_MAX_TCP_STACK_SIZE;

	unsigned long ulRxMessageLen = 0;
	unsigned short usMessageId = 0;
	unsigned long	ulRxByteCount = 0;
	unsigned long	ulMessageLength = 0;	

	int iSocReuse = 0;	

	int iKeepAliveTime				= DP_RFSAPR_KEEPALIVE_TIME;
	int iKeepAliveCount				= DP_RFSAPR_KEEPALIVE_COUNT;
	int iKeepAliveInterval			= DP_RFSAPR_KEEPALIVE_INTVL;
		
	/****************************************************/
	/*			Update the global variables				*/
	/****************************************************/

	g_RFSAPR_Handle.ucTCPHeader[0]	=	0xAA;
	g_RFSAPR_Handle.ucTCPHeader[1]	=	0xAB;
	g_RFSAPR_Handle.ucTCPHeader[2]	=	0xBA;
	g_RFSAPR_Handle.ucTCPHeader[3]	=	0xBB;

	g_RFSAPR_Handle.ucTCPFooter[0]	=	0xCC;
	g_RFSAPR_Handle.ucTCPFooter[1]	=	0xCD;
	g_RFSAPR_Handle.ucTCPFooter[2]	=	0xDC;
	g_RFSAPR_Handle.ucTCPFooter[3]	=	0xDD;

	g_RFSAPR_Handle.ucRxCmdState	=	RFSAPR_RX_HEADER_STATE;

	g_RFSAPR_Handle.usRXMessageStatus = RFSAPR_CMD_NOTRECED;

	/****************************************************/
	
	memset(&saSockAddr,0,sizeof(saSockAddr));
	saSockAddr.sin_family		=	AF_INET;
	saSockAddr.sin_addr.s_addr	=	INADDR_ANY; 
	saSockAddr.sin_port			=	htons(DEFAULT_PORT);

  
	/* Create a SOCKET for connecting to server*/
	g_RFSAPR_Handle.sktListenSocket = dp_rfsapr_socket(saSockAddr.sin_family, SOCK_STREAM, 0);
	if (g_RFSAPR_Handle.sktListenSocket == INVALID_SOCKET) 
	{
			dp_rfsapr_printf("ERROR\t: Listen socket creation failed\r\n");
		return 1;
	}

	/*To Avoid Listen socket bind failed error. Socket port reuse option is added*/
	iSocReuse = 1; /*SOC_REUSE_ENABLE*/

	iResult = dp_rfsapr_setsockopt( g_RFSAPR_Handle.sktListenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&iSocReuse, sizeof(iSocReuse));
	if (iResult < 0)
	{
		dp_rfsapr_printf("ERROR\t: SO_REUSEADDR\r\n ");
	}

	/* Set-up the TCP listening socket*/
	iResult = dp_rfsapr_bind( g_RFSAPR_Handle.sktListenSocket, (struct sockaddr*)&saSockAddr, (int)sizeof(saSockAddr));
	if (iResult == SOCKET_ERROR) 
	{
			dp_rfsapr_printf("ERROR\t: Listen socket bind failed\r\n");
			close(g_RFSAPR_Handle.sktListenSocket);
		return 1;
	}

	while(1)
	{	
		iResult = dp_rfsapr_listen(g_RFSAPR_Handle.sktListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) 
		{
				dp_rfsapr_printf("ERROR\t: Listen Failed\r\n");
				close(g_RFSAPR_Handle.sktListenSocket);
			
			return 1;
		}

		/* Accept a client socket*/
		g_RFSAPR_Handle.sktClientSocket = dp_rfsapr_accept(g_RFSAPR_Handle.sktListenSocket,(struct sockaddr*)&saReceiveAddr,&iReceiveAddrSize);
		if (g_RFSAPR_Handle.sktClientSocket == INVALID_SOCKET) 
		{
				dp_rfsapr_printf("ERROR\t: Accept Failed\r\n");
				close(g_RFSAPR_Handle.sktListenSocket);
        			
			return 1;
		}

		dp_rfsapr_printf("INFO\t: RFSAPR Type 1 Test Application Server Connection Established\r\n");

		/* Send buffer size */
		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktClientSocket,SOL_SOCKET,SO_SNDBUF, (char *)&iSendBufferSize,sizeof (iSendBufferSize));
		if (iResult < 0)
		{
			dp_rfsapr_printf("ERROR\t: SO_SNDBUF \r\n");
		}
		
		/* Send buffer size */
		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktClientSocket,SOL_SOCKET,SO_RCVBUF, (char *)&iRcvBuffSize,sizeof (iRcvBuffSize));
		if (iResult < 0)
		{
			dp_rfsapr_printf("ERROR\t: SO_RCVBUF \r\n");
		}

/*		
		iResult = dp_rfsapr_getsockopt(g_RFSAPR_Handle.sktClientSocket,SOL_SOCKET,SO_RCVBUF, (char *)&RcvBuffSize,&BufferSize);
		if (iResult < 0)
		{
			dp_rfsapr_printf("ERROR\t: SO_RCVBUF \r\n");
			dp_rfsapr_printf("Set Buffer size :%d , %X , \r\n", BufferSize , BufferSize);
		}
		else
		{
			dp_rfsapr_printf("Set Buffer size :%d , %X , \r\n", BufferSize , BufferSize);			
		}*/
		
        iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktClientSocket,           /* socket affected */
                             IPPROTO_TCP,							/* set option at TCP level */
                             0x0001,								/* TCP_NODELAY = 0x0001 name of option */
                             (char *) &iflag,						/* the cast is historical crust */
                             sizeof(int));							/* length of option value */

		 if (iResult < 0)
		 {
			dp_rfsapr_printf("ERROR\t: TCP_NODELAY \r\n");
		 }

		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktClientSocket, SOL_SOCKET, SO_KEEPALIVE,(char *)&iflag,sizeof(iflag));	
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("ERROR\t: SO_KEEPALIVE \r\n");
		}

		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktClientSocket, IPPROTO_TCP, TCP_KEEPIDLE, (char *)&iKeepAliveTime, sizeof(iKeepAliveTime));
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("\n\rERROR\t: KEEPALIVE_TIME");
		}

		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktClientSocket, IPPROTO_TCP, TCP_KEEPCNT, (char *)&iKeepAliveCount, sizeof(iKeepAliveCount));
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("\n\rERROR\t: KEEPALIVE_INTVL");
		}
		iResult = dp_rfsapr_setsockopt(g_RFSAPR_Handle.sktClientSocket, IPPROTO_TCP, TCP_KEEPINTVL, (char *)&iKeepAliveInterval, sizeof(iKeepAliveInterval));
		if (iResult == SOCKET_ERROR) 
		{
			dp_rfsapr_printf("\n\rERROR\t: KEEPALIVE_INTVL");
		}
		/*Reset the Flag*/
		g_RFSAPR_Handle.usSockErrExitFlag = 0;

		g_RFSAPR_Handle.ulSocketState	=	1;		
	
		/* Receive Client commands */
		while(1)
		{

			switch(g_RFSAPR_Handle.ucRxCmdState)
			{
				case RFSAPR_RX_HEADER_STATE:
					
					DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktClientSocket,SO_RCVTIMEO,RFSAPR_SOCKET_TIME_OUT_VALUE);				

					iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktClientSocket, &cRXData, RFSAPR_RX_BYTE_COUNT, 0);	
					
					if (iResult == RFSAPR_RX_BYTE_COUNT)
					{	
						
						if((unsigned char)cRXData == g_RFSAPR_Handle.ucTCPHeader[g_RFSAPR_Handle.usCmdIndex])
						{
							g_RFSAPR_Handle.usCmdIndex++;						
						}	
						else if((unsigned char)cRXData == g_RFSAPR_Handle.ucTCPHeader[0])
						{
							g_RFSAPR_Handle.usCmdIndex = 1;						
						}
						else
						{
							g_RFSAPR_Handle.usCmdIndex = 0;						
						}

						if(g_RFSAPR_Handle.usCmdIndex)
						{
							g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ucTCPHeader[g_RFSAPR_Handle.usCmdIndex-1] = (unsigned char)cRXData;
						}

						if(g_RFSAPR_Handle.usCmdIndex >= RFSAPR_HEADER_SIZE)
						{
							g_RFSAPR_Handle.ucRxCmdState = RFSAPR_RX_MSGLEN_STATE;
							g_RFSAPR_Handle.usCmdIndex = 1;
						}
					}
					
					break;

				case RFSAPR_RX_MSGLEN_STATE:

					ulRxMessageLen = 0;

					DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktClientSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);

					iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktClientSocket, (char *)&ulRxMessageLen, RFSAPR_MSGLEN_SIZE, 0);

					if (iResult == RFSAPR_MSGLEN_SIZE)
					{
						
						g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength = htonl(ulRxMessageLen);
						g_RFSAPR_Handle.ucRxCmdState = RFSAPR_RX_MSGID_STATE;						
					}
					break;

				case RFSAPR_RX_MSGID_STATE:

					usMessageId = 0;

					DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktClientSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);
					
					iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktClientSocket, (char *)&usMessageId, sizeof(usMessageId), 0);			
										
					if (iResult == sizeof(usMessageId))
					{

						if(g_RFSAPR_Handle.usCmdIndex)
						{
							g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usGroupId  = htons(usMessageId);
							g_RFSAPR_Handle.usCmdIndex = 0;
						}
						else
						{
							g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usUnitId  = htons(usMessageId);
							g_RFSAPR_Handle.usCmdIndex = 0;

							
						if(g_RFSAPR_Handle.ucPrintLogState	==	RFSAPR_ENABLE_PRINT_LOGS)
						{
							dp_rfsapr_printf("------------------------------------------------------------------\r\n"); 
							dp_rfsapr_printf("[%d-%d] Received Bytes : %d\r\n ", g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usGroupId, g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usUnitId, g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength);
							dp_rfsapr_printf("------------------------------------------------------------------\r\n"); 
						}
						
							if(g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength != 0)
							{
								g_RFSAPR_Handle.ucRxCmdState = RFSAPR_RX_MSGDATA_STATE;
							}
							else
							{
								g_RFSAPR_Handle.ucRxCmdState = RFSAPR_RX_FOOTER_STATE;
							}
						}
					}
										
					break;

				case RFSAPR_RX_MSGDATA_STATE:

					DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktClientSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);

					ulRxByteCount = 0;
					ulMessageLength	=	g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength;

					while(ulMessageLength > 0)
					{
						if(ulMessageLength > RFSAPR_MAX_RX_TCPMSG_SIZE)
						{
							iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktClientSocket, &g_RFSAPR_Handle.cReceiveBuffer[ulRxByteCount], RFSAPR_MAX_RX_TCPMSG_SIZE, 0);					
						}
						else
						{
							iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktClientSocket, &g_RFSAPR_Handle.cReceiveBuffer[ulRxByteCount], ulMessageLength, 0);
						}

						ulMessageLength = ulMessageLength - iResult;
						ulRxByteCount = ulRxByteCount + iResult;
					}

					g_RFSAPR_Handle.cReceiveBuffer[ulRxByteCount] = '\0';

					if(ulRxByteCount == g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength)
					{
						memcpy(&g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.UCmdRFSAPRMessage.ucRFSAPRRxData,(unsigned char*)g_RFSAPR_Handle.cReceiveBuffer,g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength);
						
					/*  for(int iDataIdx = 0; iDataIdx < ulRxByteCount; iDataIdx++)
						{
							printf("Idx : %d, Data : 0x%X - 0x%X,\n", iDataIdx, g_RFSAPR_Handle.cReceiveBuffer[iDataIdx], g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.UCmdRFSAPRMessage.ucRFSAPRRxData[iDataIdx]);
						} */
						
						
						/*if(ulRxByteCount == 262155)
						{*/
							/*iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktClientSocket, &g_RFSAPR_Handle.cReceiveBuffer[0], 4, 0);*/
							/*if(iResult > 0)
							{
								printf("%x, %x, %x, %x, %x\r\n",g_RFSAPR_Handle.cReceiveBuffer[ulRxByteCount-1], g_RFSAPR_Handle.cReceiveBuffer[0], g_RFSAPR_Handle.cReceiveBuffer[1], g_RFSAPR_Handle.cReceiveBuffer[2], g_RFSAPR_Handle.cReceiveBuffer[3]);
							}
						}*/
							
						g_RFSAPR_Handle.ucRxCmdState = RFSAPR_RX_FOOTER_STATE;
						g_RFSAPR_Handle.usCmdIndex = 0;
					}
					else
					{
						g_RFSAPR_Handle.usRXMessageStatus = RFSAPR_CMD_NOTRECED;
						g_RFSAPR_Handle.ucRxCmdState = RFSAPR_RX_HEADER_STATE;
					}
					
					
					break;
				
				case RFSAPR_RX_FOOTER_STATE:

					DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktClientSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);
					iResult = dp_rfsapr_recv(g_RFSAPR_Handle.sktClientSocket, &cRXData, RFSAPR_RX_BYTE_COUNT, 0);
					DP_RFSAPR_SetTimeOut(g_RFSAPR_Handle.sktClientSocket, SO_RCVTIMEO, RFSAPR_SOCKET_TIME_OUT_VALUE);
					
					if (iResult == RFSAPR_RX_BYTE_COUNT)
					{
						/*printf("Idx : %d : Data :%X\r\n", g_RFSAPR_Handle.usCmdIndex, cRXData);*/
						if((unsigned char)cRXData == g_RFSAPR_Handle.ucTCPFooter[g_RFSAPR_Handle.usCmdIndex])
						{
							g_RFSAPR_Handle.usCmdIndex++;						
						}	
						else if((unsigned char)cRXData == g_RFSAPR_Handle.ucTCPFooter[0])
						{
							g_RFSAPR_Handle.usCmdIndex = 1;						
						}
						else
						{
							g_RFSAPR_Handle.usCmdIndex = 0;						
						}

						if(g_RFSAPR_Handle.usCmdIndex)
						{
							g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ucTCPFooter[g_RFSAPR_Handle.usCmdIndex-1] = (unsigned char)cRXData;
						}

						if(g_RFSAPR_Handle.usCmdIndex >= RFSAPR_FOOTER_SIZE)
						{
							g_RFSAPR_Handle.ucRxCmdState = RFSAPR_RX_HEADER_STATE;
							g_RFSAPR_Handle.usRXMessageStatus = RFSAPR_CMD_RECEIVED;
							g_RFSAPR_Handle.usCmdIndex = 0;
						}
					}
						
					/*dp_rfsapr_printf("Rx Footer Cmd State : %d, Msg Status : %d, Cmd Idx = %d, Rx msg Len : %d : %d, %d \r\n",g_RFSAPR_Handle.ucRxCmdState, g_RFSAPR_Handle.usRXMessageStatus, g_RFSAPR_Handle.usCmdIndex, g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength, ulMessageLength, ulRxByteCount );*/
					break;

				default:
					break;
			}
			
			if(g_RFSAPR_Handle.usRXMessageStatus == RFSAPR_CMD_RECEIVED)
			{
				
				dp_rfsapr_printf("------------------------------------------------------------------\r\n"); 
				dp_rfsapr_printf("Recv Group ID : %d, Unit ID : %d, length : %d \r\n", g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usGroupId, g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usUnitId, g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength);
				dp_rfsapr_printf("------------------------------------------------------------------\r\n"); 
									
				sRetVal = DP_RFSAPR_CommandProcessor(g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usGroupId, g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.SCommandFromRFSAPR.SRFSAPRCmdIds.usUnitId);

				//g_RFSAPR_Handle.ucPrintLogState	=	RFSAPR_ENABLE_PRINT_LOGS;

				if(g_RFSAPR_Handle.ucPrintLogState	==	RFSAPR_ENABLE_PRINT_LOGS)
				{
					dp_rfsapr_printf("------------------------------------------------------------------\r\n"); 
					dp_rfsapr_printf("[%d-%d] Send Bytes : %d\r\n", htons(g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usGroupId), htons(g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usUnitId), g_RFSAPR_Handle.ulTxPacketLength);
					dp_rfsapr_printf("------------------------------------------------------------------\r\n"); 
				}

				if(sRetVal == RFSAPR_GOTO_LISTEN_STATE)
				{
					break;
				}				

				if (sRetVal == SOCKET_ERROR) 
				{
					dp_rfsapr_printf("ERROR\t: Client Disconnected. Data Send Failed \r\n");
					
						dp_rfsapr_printf("ERROR\t: Send failed : %d\r\n",sRetVal);
						close(g_RFSAPR_Handle.sktClientSocket);
					break;
				}
				
				g_RFSAPR_Handle.usRXMessageStatus =	RFSAPR_CMD_NOTRECED;
			}	
				

		 /*
		  * In vxworks if the client connection is closed then the recv function returns either 0 mostly or ECONNRESET and
		  * it is not breaking in while (1) and not going to listen. To avoid this following condition is added.
		 */
			if(iResult == 0 || errno == ECONNRESET)
			{
				break;
			}
			else if((iResult < 0) && (errno != EWOULDBLOCK))
			{
				perror("Error Msg : ");
				printf("Clinet socket disconnected \n"); /* added */
				g_RFSAPR_Handle.usSockErrExitFlag = 1; /* added */
				break;	 /* Socket conn error "break" added */			
			}
		}/* Inner While loop */	
		
		
		/** Added to close the socket and break the thread if no connection is there */
		if(g_RFSAPR_Handle.usSockErrExitFlag == 1)
		{
			break;	 /* Socket conn error "break" added */
		}
		
		
	}	/** Outer While loop **/	
	
	/* No longer need server socket*/

	dp_rfsapr_close(g_RFSAPR_Handle.sktListenSocket);
	

    /* shut-down the connection since we're done*/
    iResult = dp_rfsapr_shutdown(g_RFSAPR_Handle.sktClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) 
	{
			dp_rfsapr_printf("ERROR\t: Shutdown Failed...\r\n");
			dp_rfsapr_close(g_RFSAPR_Handle.sktClientSocket);
        
        return 1;
    }	

    
	/*clean-up*/
	
		dp_rfsapr_close(g_RFSAPR_Handle.sktClientSocket);

		dp_rfsapr_thread_exit(&g_RFSAPR_Handle.g_hRFSAPRServerTaskId);


	return DP_RFSAPR_SUCCESS;
}

/*! 
 *	\function		DP_RFSAPR_CommandProcessor()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function calls respective routines for the commands received from RFSAPR GUI.
 *	\param[in]		usCmdGroupID
 *	\param[in]		usCmdUnitID
 *	\param[out]		None
 *	\return			sRetVal
 */

short DP_RFSAPR_CommandProcessor(unsigned short usCmdGroupID, unsigned short usCmdUnitID)
{
	short sRetVal = DP_RFSAPR_SUCCESS, sRetVal_EndianConv = DP_RFSAPR_SUCCESS;	

	int iSendResult = 0;
	U8BIT  ucChNo 				= DP_RFSAPR_INIT_VALUE;
	S8BIT  carrErrMsg[100];	
	unsigned long ulResMessageSize = 0;
	unsigned long ulTxPacketLength = 0, ulRxMsgLength = 0;	

	/* Following buffers are declared as static, to avoid the stack overflow */
	static S_DP_VPX_1502_CONFIG_TRF_PLL				S_Config_TRF_PLL;
	static S_DP_VPX_1502_CONFIG_LMK_PLL			    S_Config_LMK_PLL;
	static S_DP_VPX_1502_CONFIG_ADC					S_ConfigADC;	
	static S_DP_VPX_1502_CONFIG_LOG_ADC				S_Config_Log_ADC; 
	static	S_FPGA_SEQUENCE_WRITE					S_FPGA_SequenceWrite;
	static S_DP_VPX_1013_CONFIG_LMX_PLL				S_NB_Config_LMX_PLL;
	static S_DP_VPX_1013_CONFIG_LMK_PLL			    S_NB_Config_LMK_PLL;
	static S_DP_VPX_1013_CONFIG_ADC					S_NB_ConfigADC;	
	
	memset(&S_Config_TRF_PLL,0,sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL));
	memset(&S_Config_LMK_PLL,0,sizeof(S_DP_VPX_1502_CONFIG_LMK_PLL));
	memset(&S_ConfigADC,0,sizeof(S_DP_VPX_1502_CONFIG_ADC));
	memset(&S_Config_Log_ADC,0,sizeof(S_DP_VPX_1502_CONFIG_LOG_ADC));
	memset(&S_FPGA_SequenceWrite,0,sizeof(S_FPGA_SEQUENCE_WRITE));
	memset(&S_NB_Config_LMX_PLL,0,sizeof(S_DP_VPX_1013_CONFIG_LMX_PLL));
	memset(&S_NB_Config_LMK_PLL,0,sizeof(S_DP_VPX_1013_CONFIG_LMK_PLL));
	memset(&S_NB_ConfigADC,0,sizeof(S_DP_VPX_1013_CONFIG_ADC));	
	
	


	/*	Getting Objects for Global TCP Rx Command structure and TCP Tx Response structure	*/
	GET_TCPCOMM_HANDLE(pSCommandFromRFSAPR, pSResponseToRFSAPR);	

	ulRxMsgLength = g_RFSAPR_Handle.S_RFSAPR_RxTcpPacket.ulMessageLength;

	pSResponseToRFSAPR->SRFSAPRResIds.usGroupId	=	usCmdGroupID;
	pSResponseToRFSAPR->SRFSAPRResIds.usUnitId	=	usCmdUnitID;

	g_RFSAPR_Handle.ucPrintLogState = RFSAPR_ENABLE_PRINT_LOGS;

	printf("In Commnd Proccsr : Received GRp ID : %d ,Received Unit ID : %d \n",usCmdGroupID,usCmdUnitID);
	
	/*Endian Swap*/
	sRetVal = DP_RFSAPR_EndianSwap(usCmdGroupID,usCmdUnitID,pSCommandFromRFSAPR->UCmdRFSAPRMessage.ucRFSAPRRxData);
	
	if(sRetVal == DP_RFSAPR_SUCCESS)
	{	
		switch(usCmdGroupID)
		{
			case RFSAPR_COMM_CMD_GROUP0:
			{					
				switch(usCmdUnitID)
				{			
					case RFSAPR_CMD_VERSION_DETAILS:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_VERSION_DETAILS;

						if(ulRxMsgLength != 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							sRetVal = DP_RFSAPR_VerionDetails();
							if(sRetVal == DP_RFSAPR_SUCCESS)
							{
								ulResMessageSize = sizeof(pSResponseToRFSAPR->UResRFSAPRMessage.S_ResVersionDetails);
							}
						}
					
						break;

					case RFSAPR_CMD_FPGA_READ:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FPGA_READ;

						if(ulRxMsgLength != sizeof(S_CMD_FPGA_READ))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
#ifdef DP_RFSAPR_DRX_DEBUG_MSG				
							dp_rfsapr_printf("INFO\t: Read FPGA Command Received\r\n");
							dp_rfsapr_printf("INFO\t: Board No\t\t: %d\r\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadFPGA.ucBoardNo + 1));
							dp_rfsapr_printf("INFO\t: Bar No\t\t: %d\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadFPGA.ucBarSelection);
							dp_rfsapr_printf("INFO\t: FPGA Offset\t: 0x%X\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadFPGA.ulFPGAOffset);
#endif						
					
#ifndef WIN32
							sRetVal = DP_RFSAPR_DRx_ReadLong(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadFPGA.ucBoardNo, /*Board No*/
															pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadFPGA.ucBarSelection,/** Bar Selection*/
															pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadFPGA.ulFPGAOffset,/* FPGA Offset*/
															&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResFPGAData.ulFPGAData); /* FPGA Read Data*/
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("\n\rERROR\t: DP_RFSAPR_DRx_ReadLong...\r\n");							
								break;
							}
#endif

							ulResMessageSize = sizeof(S_RES_FPGA_READ);
						}						
						break;

					case RFSAPR_CMD_FPGA_WRITE:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FPGA_WRITE;

						if(ulRxMsgLength != sizeof(S_CMD_FPGA_WRITE))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{	
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: Write FPGA Command Received\r\n");
							dp_rfsapr_printf("INFO\t: Board No\t\t: %d\r\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ucBoardNo + 1));
							dp_rfsapr_printf("INFO\t: Bar No\t\t: %d\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ucBarSelection);
							dp_rfsapr_printf("INFO\t: FPGA Offset\t: 0x%X \r\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ulFPGAOffset);
							dp_rfsapr_printf("INFO\t: Write Data\t: %ld (0x%X) \r\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ulFPGAData, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ulFPGAData);
#endif					
					
#ifndef WIN32
							sRetVal = DP_RFSAPR_DRx_WriteLong(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ucBoardNo, /*Board No*/
															  pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ucBarSelection,/** Bar Selection*/
															  pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ulFPGAOffset,/* FPGA Offset*/
															  pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WriteFPGA.ulFPGAData); /* FPGA Write Data*/
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DRx_WriteLong...\r\n");
							}
#endif
						}
						break;
						
					case RFSAPR_CMD_NB_FPGA_READ:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_NB_FPGA_READ;

						if(ulRxMsgLength != sizeof(S_CMD_NB_FPGA_READ))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
#ifdef DP_RFSAPR_DRX_DEBUG_MSG				
							dp_rfsapr_printf("INFO\t: Read NB FPGA Command Received\r\n");
							dp_rfsapr_printf("INFO\t: Board No\t\t: %d\r\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBReadFPGA.ucBoardNo + 1));
							dp_rfsapr_printf("INFO\t: Bar No\t\t: %d\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBReadFPGA.ucBarSelection);
							dp_rfsapr_printf("INFO\t: FPGA Offset\t: 0x%X\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBReadFPGA.ulFPGAOffset);
#endif						
					
#ifndef WIN32
							sRetVal = DP_RFSAPR_DPVPX1013_DRx_ReadLong(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBReadFPGA.ucBoardNo, /*Board No*/
															pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBReadFPGA.ucBarSelection,/** Bar Selection*/
															pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBReadFPGA.ulFPGAOffset,/* FPGA Offset*/
															&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBFPGAData.ulFPGAData); /* FPGA Read Data*/
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("\n\rERROR\t: DP_RFSAPR_DPVPX1013_DRx_ReadLong...\r\n");							
								break;
							}
#endif

							ulResMessageSize = sizeof(S_RES_NB_FPGA_READ);
						}						
						break;

					case RFSAPR_CMD_NB_FPGA_WRITE:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_NB_FPGA_WRITE;

						if(ulRxMsgLength != sizeof(S_CMD_NB_FPGA_WRITE))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{					
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: Write NB FPGA Command Received\r\n");
							dp_rfsapr_printf("INFO\t: Board No\t\t: %d\r\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ucBoardNo + 1));
							dp_rfsapr_printf("INFO\t: Bar No\t\t: %d\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ucBarSelection);
							dp_rfsapr_printf("INFO\t: FPGA Offset\t: 0x%X \r\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ulFPGAOffset);
							dp_rfsapr_printf("INFO\t: Write Data\t: %ld (0x%X) \r\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ulFPGAData, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ulFPGAData);
#endif					
						/*	DP_RFSAPR_DPVPX1013_DRx_WriteLong*/
#ifndef WIN32
							sRetVal = DP_RFSAPR_DPVPX1013_DRx_WriteLong(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ucBoardNo, /*Board No*/
															  pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ucBarSelection,/** Bar Selection*/
															  pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ulFPGAOffset,/* FPGA Offset*/
															  pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NBWriteFPGA.ulFPGAData); /* FPGA Write Data*/
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1013_DRx_WriteLong...\r\n");
							}
#endif
						}
						break;
						
					case RFSAPR_CMD_WRITE_FPGA_ADC_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_FPGA_ADC_DATA;

						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_ADC))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							dp_rfsapr_printf("INFO\t: Write ADC Command Received \r\n");
							memcpy(&S_ConfigADC, &pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ConfigADC, sizeof(S_DP_VPX_1502_CONFIG_ADC));


		
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: BoardNumber\t: %d\r\n", (S_ConfigADC.ucBoardNumber + 1));
#ifndef DP_RFSAPR_ENBLE_DRX_CONFIG_PANEL_NEW
							dp_rfsapr_printf("INFO\t: ADC Selection\t: %d\r\n", (S_ConfigADC.ucADCSelection + 1));
#endif
#endif
							sRetVal = DP_RFSAPR_DP_VPX_1502_ADC_Config(&S_ConfigADC); 
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1502_ADC_Config...\r\n");
							}
							else
							{
								dp_rfsapr_printf("INFO\t: ADC Configuration Success for %d ADC Selection...\r\n", (S_ConfigADC.ucADCSelection + 1));
								if(S_ConfigADC.ucBoardNumber < DP_RFSAPR_DRX_MAX_BOARD_COUNT)
								{
									memcpy(&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_DPVPX1502_ADCConfigData[S_ConfigADC.ucBoardNumber], &S_ConfigADC, sizeof(S_ConfigADC));

									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_FPGA_DRX_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.S_DPVPX1502_Configurations, (unsigned long)sizeof(g_RFSAPR_Handle.S_DPVPX1502_Configurations));
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: ADC Configuration store to file\r\n");
									}
									else
									{
										dp_rfsapr_printf("SUCCESS\t: ADC Configuration store to file\r\n");
									}
								}
								else
								{
									dp_rfsapr_printf("ERROR\t: Invalid board number\r\n");
								}
							}
						}

						break;
						
					case RFSAPR_CMD_WRITE_NB_FPGA_ADC_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_NB_FPGA_ADC_DATA;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("Received RFSAPR_CMD_WRITE_NB_FPGA_ADC_DATA \n");
#endif						

						if(ulRxMsgLength != sizeof(S_DP_VPX_1013_CONFIG_ADC))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							dp_rfsapr_printf("INFO\t: Write NB ADC Command Received \r\n");
							memcpy(&S_NB_ConfigADC, &pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_ConfigADC, sizeof(S_DP_VPX_1013_CONFIG_ADC));


		
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: BoardNumber\t: %d\r\n", (S_ConfigADC.ucBoardNumber + 1));
#ifndef DP_RFSAPR_ENBLE_DRX_CONFIG_PANEL_NEW
							dp_rfsapr_printf("INFO\t: ADC Selection\t: %d\r\n", (S_ConfigADC.ucADCSelection + 1));
#endif
#endif
							sRetVal = DP_RFSAPR_DPVPX1013_ADC_Config(&S_NB_ConfigADC); 
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1013_ADC_Config...\r\n");
							}
							else
							{
 								
								dp_rfsapr_printf("INFO\t: NB ADC Configuration Success for %d ADC Selection...\r\n", (S_NB_ConfigADC.ucADCSelection + 1));
								if(S_NB_ConfigADC.ucBoardNumber < DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT)
								{
									memcpy(&g_RFSAPR_Handle.S_DPVPX1013_Configurations.S_DPVPX1013_ADCConfigData[S_NB_ConfigADC.ucBoardNumber], &S_NB_ConfigADC, sizeof(S_NB_ConfigADC));

									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_FPGA_DRX_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.S_DPVPX1502_Configurations, (unsigned long)sizeof(g_RFSAPR_Handle.S_DPVPX1502_Configurations));
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: NB ADC Configuration store to file\r\n");
									}
									else
									{
										dp_rfsapr_printf("SUCCESS\t: NB ADC Configuration store to file\r\n");
									}
								}
								else
								{
									dp_rfsapr_printf("ERROR\t: Invalid board number\r\n");
								}
								
							}
						}

						break;						

					case RFSAPR_CMD_READ_FPGA_REGISTERS_ADC_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_FPGA_REGISTERS_ADC_DATA;															   

						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_ADC))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
						 
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: Read ADC Register Command Received\r\n");
#endif
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ConfigADC.ucBoardNumber >= DP_RFSAPR_DRX_MAX_BOARD_COUNT)
							{
								sRetVal =  DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ConfigADC.ucADCSelection >= DP_RFSAPR_DRX_ADC_CH_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_ADC_CHN_NUM;
								break;
							}					

							memcpy(&S_ConfigADC,&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_DPVPX1502_ADCConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ConfigADC.ucBoardNumber],sizeof(S_DP_VPX_1502_CONFIG_ADC));
			
#ifndef WIN32
						
							/*sRetVal = DP_RFSAPR_CmdRFSAPRMessage_Read_ADC_ConfigReg(&S_ConfigADC);*/
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{							
								dp_rfsapr_printf("ERROR\t: ADC Config...\r\n");
								break;
							}
#endif

							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResADC_Data,&S_ConfigADC,sizeof(S_DP_VPX_1502_CONFIG_ADC));
							ulResMessageSize = sizeof( S_DP_VPX_1502_CONFIG_ADC);
						}

						break;
						

					case RFSAPR_CMD_WRITE_LMK_PLL_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_LMK_PLL_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Write LMK PLL Command Received \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_LMK_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							memcpy(&S_Config_LMK_PLL,&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_LMK_PLL,sizeof(S_DP_VPX_1502_CONFIG_LMK_PLL));
		
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: BoardNumber\t: %d\r\n", (S_Config_LMK_PLL.ucBoardNumber + 1));
#ifndef DP_RFSAPR_ENBLE_DRX_CONFIG_PANEL_NEW
							dp_rfsapr_printf("INFO\t: PLL Selection\t: %d\r\n", (S_Config_LMK_PLL.ucPLLSelection + 1));
#endif
#endif

							sRetVal = DP_RFSAPR_DP_VPX_1502_LMK_PLL_Config(&S_Config_LMK_PLL);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1502_LMK_PLL_Config...\r\n");
							}
							else
							{
								if(S_Config_LMK_PLL.ucBoardNumber < DP_RFSAPR_DRX_MAX_BOARD_COUNT)
								{
									memcpy(&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_LMK_PLLConfigData[S_Config_LMK_PLL.ucBoardNumber], &S_Config_LMK_PLL, sizeof(S_Config_LMK_PLL));

									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_FPGA_DRX_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.S_DPVPX1502_Configurations, (unsigned long)sizeof(g_RFSAPR_Handle.S_DPVPX1502_Configurations));
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t:LMK ADC Configuration store to file Err Code : %d \r\n", sRetVal);
									}
									else
									{
										dp_rfsapr_printf("INFO\t: LMK PLL Configuration Success ..\r\n" );
									}
								}
								else
								{
									dp_rfsapr_printf("ERROR\t:Invalid board number\r\n");
								}
							}
						}

						break;
						
					case RFSAPR_CMD_WRITE_NB_LMK_PLL_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_NB_LMK_PLL_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Write NB LMK PLL Command Received \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_DP_VPX_1013_CONFIG_LMK_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							memcpy(&S_NB_Config_LMK_PLL,&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_Config_LMK_PLL,sizeof(S_DP_VPX_1013_CONFIG_LMK_PLL));
		
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: BoardNumber\t: %d\r\n", (S_NB_Config_LMK_PLL.ucBoardNumber + 1));
#ifndef DP_RFSAPR_ENBLE_DRX_CONFIG_PANEL_NEW
							dp_rfsapr_printf("INFO\t: PLL Selection\t: %d\r\n", (S_NB_Config_LMK_PLL.ucPLLSelection + 1));
#endif
#endif

							sRetVal = DP_RFSAPR_DP_VPX_1013_LMK_PLL_Config(&S_NB_Config_LMK_PLL);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1013_LMK_PLL_Config...\r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: DP_RFSAPR_DP_VPX_1013_LMK_PLL_Config Completed... : %d\r\n", sRetVal);
								if(S_NB_Config_LMK_PLL.ucBoardNumber < DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT)
								{
									memcpy(&g_RFSAPR_Handle.S_DPVPX1013_Configurations.S_LMK_PLLConfigData[S_Config_LMK_PLL.ucBoardNumber], &S_NB_Config_LMK_PLL, sizeof(S_NB_Config_LMK_PLL));

									/* Inside this function need to be updated for 1013 */
									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_FPGA_DRX_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.S_DPVPX1502_Configurations, (unsigned long)sizeof(g_RFSAPR_Handle.S_DPVPX1502_Configurations));
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t:LMK ADC Configuration store to file Err code : %d\r\n", sRetVal);
									}
									else
									{
										dp_rfsapr_printf("INFO\t: LMK PLL Configuration Success ..\r\n" );
									}
								}
								else
								{
									dp_rfsapr_printf("ERROR\t:Invalid board number\r\n");
								}
								
							}
						}
						break;						
						
					case RFSAPR_CMD_READ_LMK_PLL_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_LMK_PLL_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read LMK PLL Command Received\r\n");						
#endif
						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_LMK_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_LMK_PLL.ucBoardNumber >= DP_RFSAPR_DRX_MAX_BOARD_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_LMK_PLL.ucPLLSelection >= DP_RFSAPR_MAX_PLL_CHANNEL_NUMBER)
							{
								sRetVal = DP_RFSAPR_INVALID_PLL_NUM;
								break;
							}

							ulResMessageSize = sizeof( S_DP_VPX_1502_CONFIG_LMK_PLL);
							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Config_LMK_PLL, &g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_LMK_PLLConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_LMK_PLL.ucBoardNumber],sizeof(S_DP_VPX_1502_CONFIG_LMK_PLL));
						}

						

						break;
						
					case RFSAPR_CMD_READ_NB_LMK_PLL_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NB_LMK_PLL_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read NB LMK PLL Command Received\r\n");						
#endif
						if(ulRxMsgLength != sizeof(S_DP_VPX_1013_CONFIG_LMK_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_Config_LMK_PLL.ucBoardNumber >= DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_Config_LMK_PLL.ucPLLSelection >= DP_RFSAPR_MAX_PLL_CHANNEL_NUMBER)
							{
								sRetVal = DP_RFSAPR_INVALID_PLL_NUM;
								break;
							}

							ulResMessageSize = sizeof(S_DP_VPX_1013_CONFIG_LMK_PLL);
							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Config_NB_LMK_PLL, &g_RFSAPR_Handle.S_DPVPX1013_Configurations.S_LMK_PLLConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_LMK_PLL.ucBoardNumber],sizeof(S_DP_VPX_1013_CONFIG_LMK_PLL));
						}

						break;		
					
					case RFSAPR_CMD_READ_LMK_REGISTERS_PLL_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_LMK_REGISTERS_PLL_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG						
						dp_rfsapr_printf("INFO\t: Read Registers LMK PLL Command Received\r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_LMK_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{

							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_LMK_PLL.ucBoardNumber >= DP_RFSAPR_DRX_MAX_BOARD_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_LMK_PLL.ucPLLSelection >= DP_RFSAPR_MAX_PLL_CHANNEL_NUMBER)
							{
								sRetVal = DP_RFSAPR_INVALID_PLL_NUM;
								break;
							}

							memcpy(&S_Config_LMK_PLL,&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_LMK_PLLConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_LMK_PLL.ucBoardNumber],sizeof(S_DP_VPX_1502_CONFIG_LMK_PLL));

#ifndef WIN32
							/*sRetVal = DP_RFSAPR_CmdRFSAPRMessage_Read_LMK_PLL_Data(&S_Config_LMK_PLL);*/
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Read LMK PLL...\r\n");
								break;
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: Read LMK PLL...\r\n");
							}
#endif
							ulResMessageSize = sizeof( S_DP_VPX_1502_CONFIG_LMK_PLL);
							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Config_LMK_PLL,&S_Config_LMK_PLL,sizeof(S_DP_VPX_1502_CONFIG_LMK_PLL));
						}

						break;

					case RFSAPR_CMD_WRITE_TRF_PLL_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_TRF_PLL_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG				
						dp_rfsapr_printf("INFO\t: Write TRF PLL Command Received \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							memcpy(&S_Config_TRF_PLL ,&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_TRF_PLL,sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL));
		
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: BoardNumber\t: %d\r\n", (S_Config_TRF_PLL.ucBoardNumber + 1));
#ifndef DP_RFSAPR_ENBLE_DRX_CONFIG_PANEL_NEW
							dp_rfsapr_printf("INFO\t: PLL Selection\t: %d\r\n", (S_Config_TRF_PLL.ucPLLSelection + 1));
#endif
#endif
						
							sRetVal = DP_RFSAPR_DP_VPX_1502_TRF_PLL_Config(&S_Config_TRF_PLL);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1502_TRF_PLL_Config...\r\n");
							}
							else
							{
								if(S_Config_TRF_PLL.ucBoardNumber < DP_RFSAPR_DRX_MAX_BOARD_COUNT)
								{
									memcpy(&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_TRF_PLLConfigData[S_Config_TRF_PLL.ucBoardNumber], &S_Config_TRF_PLL, sizeof(S_Config_TRF_PLL));

									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_FPGA_DRX_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.S_DPVPX1502_Configurations, (unsigned long)sizeof(g_RFSAPR_Handle.S_DPVPX1502_Configurations));
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t:TRF ADC Configuration store to file\r\n");
									}
									else
									{
										dp_rfsapr_printf("INFO\t: TRF PLL Configuration Success ..\r\n" );
									}
								}
								else
								{
									dp_rfsapr_printf("ERROR\t: Invalid board number..\r\n" );
								}
							}							
						}

						break;
						
					case RFSAPR_CMD_WRITE_NB_LMX_PLL_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_NB_LMX_PLL_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG				
						dp_rfsapr_printf("INFO\t: Write LMX PLL Command Received \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_DP_VPX_1013_CONFIG_LMX_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							memcpy(&S_NB_Config_LMX_PLL ,&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_Config_TRF_PLL,sizeof(S_DP_VPX_1013_CONFIG_LMX_PLL));
		
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: BoardNumber\t: %d\r\n", (S_NB_Config_LMX_PLL.ucBoardNumber));
#ifndef DP_RFSAPR_ENBLE_DRX_CONFIG_PANEL_NEW
							dp_rfsapr_printf("INFO\t: PLL Selection\t: %d\r\n", (S_NB_Config_LMX_PLL.ucPLLSelection + 1));
#endif
#endif
							dp_rfsapr_printf("LMX PLL Data Count: %d\r\n", (S_NB_Config_LMX_PLL.ulDataCount));	
						
							sRetVal = DP_RFSAPR_DP_VPX_1013_LMX_PLL_Config(&S_NB_Config_LMX_PLL);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1013_TRF_PLL_Config...\r\n");
							}
							else
							{
 
								if(S_NB_Config_LMX_PLL.ucBoardNumber < DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT)
								{
									memcpy(&g_RFSAPR_Handle.S_DPVPX1013_Configurations.S_LMX_PLLConfigData[S_NB_Config_LMX_PLL.ucBoardNumber], &S_NB_Config_LMX_PLL, sizeof(S_NB_Config_LMX_PLL));

									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_FPGA_DRX_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.S_DPVPX1502_Configurations, (unsigned long)sizeof(g_RFSAPR_Handle.S_DPVPX1502_Configurations));
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t:TRF ADC Configuration store to file\r\n");
									}
									else
									{
										dp_rfsapr_printf("INFO\t: TRF PLL Configuration Success ..\r\n" );
									}
								}
								else
								{
									dp_rfsapr_printf("ERROR\t: Invalid board number..\r\n" );
								}
								
							}							
						}

						break;						

				case RFSAPR_CMD_READ_TRF_PLL_CONFIG:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_TRF_PLL_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read TRF PLL Command Received\r\n");						
#endif

						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_TRF_PLL.ucBoardNumber >= DP_RFSAPR_DRX_MAX_BOARD_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_TRF_PLL.ucPLLSelection >= DP_RFSAPR_MAX_PLL_CHANNEL_NUMBER)
							{
								sRetVal = DP_RFSAPR_INVALID_PLL_NUM;
								break;
							}						
								
							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Config_TRF_PLL,&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_TRF_PLLConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_TRF_PLL.ucBoardNumber],sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL));
							ulResMessageSize = sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL);
						}

						break;
						
				case RFSAPR_CMD_READ_NB_LMX_PLL_CONFIG:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NB_LMX_PLL_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read NB TRF PLL Command Received\r\n");						
#endif

						if(ulRxMsgLength != sizeof(S_DP_VPX_1013_CONFIG_LMX_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_Config_TRF_PLL.ucBoardNumber >= DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_Config_TRF_PLL.ucPLLSelection >= DP_RFSAPR_MAX_PLL_CHANNEL_NUMBER)
							{
								sRetVal = DP_RFSAPR_INVALID_PLL_NUM;
								break;
							}						
								
							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Config_NB_TRF_PLL,&g_RFSAPR_Handle.S_DPVPX1013_Configurations.S_LMX_PLLConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_Config_TRF_PLL.ucBoardNumber],sizeof(S_DP_VPX_1013_CONFIG_LMX_PLL));
							ulResMessageSize = sizeof(S_DP_VPX_1013_CONFIG_LMX_PLL);
						}

						break;			
					
				case RFSAPR_CMD_FLASH_SINGLE_WRITE:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FLASH_SINGLE_WRITE;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WB_FLASH_SINGLE_WRITE Command Received\r\n");						
#endif
						
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Single_Write.m_ucBoardSel == 1)
							{							
								sRetVal = DP_RFSAPR_DPVPX1502_WB_Flash_Single_Write(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Single_Write);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1502_WB_Flash_Single_Write...\r\n");
								}
							}
							else
							{
								sRetVal = DP_RFSAPR_DPVPX1013_WB_Flash_Single_Write(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Single_Write);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1013_WB_Flash_Single_Write...\r\n");
								}																
							}
							
						}
						break;						

				case RFSAPR_CMD_FLASH_FILE_WRITE:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FLASH_FILE_WRITE;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WB_FLASH_FILE_WRITE Command Received\r\n");						
#endif
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Page_Write.m_ucBoardSel == 1)
						{
							sRetVal = DP_RFSAPR_DPVPX1502_WB_Flash_Page_Write(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Page_Write);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1502_WB_Flash_Page_Write...\r\n");
							}
						}
						else
						{
							sRetVal = DP_RFSAPR_DPVPX1013_WB_Flash_Page_Write(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Page_Write);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1013_WB_Flash_Page_Write...\r\n");
							}
						}							
					}					
					break;		
					
				case RFSAPR_CMD_FLASH_SINGLE_READ:
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FLASH_SINGLE_READ;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WB_FLASH_SINGLE_READ Command Received\r\n");						
#endif
						
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Single_Read.m_ucBoardSel == 1)
							{							
								sRetVal = DP_RFSAPR_DPVPX1502_WB_Flash_Single_Read(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Single_Read,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_WB_Flash_Single_Read);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1502_WB_Flash_Single_Read...\r\n");
								}
							}
							else
							{							
								sRetVal = DP_RFSAPR_DPVPX1013_WB_Flash_Single_Read(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Single_Read,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_WB_Flash_Single_Read);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1013_WB_Flash_Single_Read...\r\n");
								}
							}
								
						}
						
						ulResMessageSize = sizeof(S_RFSAPR_RES_FLASH_SINGLE_READ);	
						
						break;						
				case RFSAPR_CMD_FLASH_FILE_READ:	

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WB_FLASH_FILE_READ Command Received\r\n");						
#endif
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FLASH_FILE_READ;	


					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Page_Read.m_ucBoardSel == 1)
						{						
							sRetVal = DP_RFSAPR_DPVPX1502_WB_Flash_Page_Read(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Page_Read,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_WB_Flash_Page_Read);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1502_WB_Flash_Read...\r\n");
							}
						}
						else
						{						
							sRetVal = DP_RFSAPR_DPVPX1013_WB_Flash_Page_Read(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_Page_Read,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_WB_Flash_Page_Read);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1013_WB_Flash_Page_Read...\r\n");
							}
						}						
					}
					
					ulResMessageSize = sizeof(S_RFSAPR_RES_FLASH_PAGE_FILE_READ);
					
					break;		
						
				case RFSAPR_CMD_FLASH_ERASE:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FLASH_ERASE;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WB_FLASH_ERASE Command Received\r\n");						
#endif

					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_erase.m_ucBoardSel == 1)
						{						
							sRetVal = DP_RFSAPR_DP_VPX_1502_WB_Flash_Erase(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_erase);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1502_WB_Flash_Erase...\r\n");
							}
						}
						else
						{						
							sRetVal = DP_RFSAPR_DP_VPX_1013_WB_Flash_Erase(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_erase);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1013_WB_Flash_Erase...\r\n");
							}
						}
						
					}
					
					break;									

				case RFSAPR_CMD_FLASH_MEM_TEST:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FLASH_MEM_TEST;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WB_FLASH_MEM_TEST Command Received\r\n");						
#endif

					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
					/*	sRetVal = DP_RFSAPR_DP_VPX_1502_WB_Flash_MemTest(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_MemTest);*/
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1502_WB_Flash_MemTest...\r\n");
						}
					}
					
					break;														

				case RFSAPR_CMD_FLASH_READ_ID:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FLASH_READ_ID;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WB_FLASH_READ_ID Command Received\r\n");						
#endif

					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
					/*	sRetVal = DP_RFSAPR_DP_VPX_1502_WB_Read_Flash_ID(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_Flash_ReadID,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_WB_Flash_ReadID);*/
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1502_WB_Read_Flash_ID...\r\n");
						}
					}					
					break;
					
				case RFSAPR_CMD_SEND_FLASH_DATA_SIZE:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SEND_FLASH_DATA_SIZE;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_SEND_FLASH_DATA_SIZE Command Received\r\n");						
#endif

					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*sRetVal = DP_RFSAPR_Receive_and_allocate_Flash_Size_Mem(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdFlashDataSize.m_ulFlashDataSize);*/
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: DP_RFSAPR_Receive_and_allocate_Flash_Size_Mem...\r\n");
						}
					}					
					break;
					
				case RFSAPR_CMD_SEND_FLASH_CONFIG_DATA:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SEND_FLASH_CONFIG_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_SEND_FLASH_CONFIG_DATA Command Received\r\n");						
#endif

					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*sRetVal = DP_RFSAPR_Receive_Flash_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdConfigFlashData);*/
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: DP_RFSAPR_Receive_Flash_Data ...\r\n");
						}
					}					
					break;
					
				case RFSAPR_CMD_WRITE_CONFIG_DATA_TO_FLASH:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_CONFIG_DATA_TO_FLASH;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					/*dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WRITE_CONFIG_DATA_TO_FLASH Command Received\r\n");*/						
#endif

					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						
						sRetVal = DP_RFSAPR_Configure_Flash_Data(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdConfigFlashData);
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: DP_RFSAPR_Receive_and_Configure_Flash_Data ...\r\n");
						}
					}					
					break;		
				case RFSAPR_CMD_READ_FLASH_CONFIG_COMPLETION_STS:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_FLASH_CONFIG_COMPLETION_STS;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_FLASH_CONFIG_COMPLETION_STS Command Received\r\n");						
#endif

					if(ulRxMsgLength != 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{						
						pSResponseToRFSAPR->UResRFSAPRMessage.m_S_Res_FlashConfigSts.m_cFlashConfigSts =  g_RFSAPR_Handle.g_cFlashConfigSts;
					}					
					break;				
					
				case RFSAPR_CMD_READ_FLASH_CONFIG_DATA:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_FLASH_CONFIG_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_FLASH_CONFIG_DATA Command Received\r\n");						
#endif

					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{						
						
						sRetVal = DP_RFSAPR_Read_Flash_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdReadFlashData , &pSResponseToRFSAPR->UResRFSAPRMessage.m_S_ResFlashDataRead);
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: DP_RFSAPR_Receive_and_Configure_Flash_Data ...\r\n");
						}
					}
					
					ulResMessageSize = sizeof(S_RFSAPR_RES_FLASH_READ_DATA);	
					
					break;						
					
					
				case RFSAPR_CMD_WRITE_FPGA_SEQUENCE_CONFIG:

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_FPGA_SEQUENCE_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG				
						dp_rfsapr_printf("INFO\t: Write FPGA Sequence Command Received \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_FPGA_SEQUENCE_WRITE))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							memcpy(&S_FPGA_SequenceWrite, &pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGA_SequenceWrite,sizeof(S_FPGA_SEQUENCE_WRITE));
		
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: Board Number\t: %d\r\n", (S_FPGA_SequenceWrite.ucBoardNumber + 1));
#endif
							sRetVal = DP_RFSAPR_DP_VPX_1502_FPGA_Sequence_Config(&S_FPGA_SequenceWrite);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DP_VPX_1502_FPGA_Sequence_Config...\r\n");
							}
							else
							{
 								
								if(S_FPGA_SequenceWrite.ucBoardNumber < DP_RFSAPR_DRX_MAX_BOARD_COUNT)
								{
									memcpy(&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_FPGA_SequenceWrite[S_FPGA_SequenceWrite.ucBoardNumber], &S_FPGA_SequenceWrite, sizeof(S_FPGA_SEQUENCE_WRITE));

									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_FPGA_DRX_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.S_DPVPX1502_Configurations, (unsigned long)sizeof(g_RFSAPR_Handle.S_DPVPX1502_Configurations));
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t:FPGA sequence configuration store to file\r\n");
									}
									else
									{
										dp_rfsapr_printf("SUCCESS\t: FPGA sequence configuration.\r\n" );
									}
								}
								else
								{
									dp_rfsapr_printf("ERROR\t:Invalid board number\r\n");
								}
								
							}							
						}

						break;

				case RFSAPR_CMD_READ_FPGA_SEQUENCE_CONFIG:				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_FPGA_SEQUENCE_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read FPGA Sequence Command Received\r\n");						
#endif
						if(ulRxMsgLength != sizeof(S_FPGA_SEQUENCE_WRITE))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGA_SequenceWrite.ucBoardNumber >= DP_RFSAPR_DRX_MAX_BOARD_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
						
							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_FPGA_SequenceWrite, &g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_FPGA_SequenceWrite[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGA_SequenceWrite.ucBoardNumber],sizeof(S_FPGA_SEQUENCE_WRITE));
							ulResMessageSize = sizeof(S_FPGA_SEQUENCE_WRITE);
						}
						break;
				case RFSAPR_CMD_READ_TRF_REGISTERS_PLL_DATA:

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_TRF_REGISTERS_PLL_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG					
						dp_rfsapr_printf("INFO\t: Read Registers TRF PLL Command Received\r\n");
#endif						

						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_TRF_PLL.ucBoardNumber >= DP_RFSAPR_DRX_MAX_BOARD_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_TRF_PLL.ucPLLSelection >= DP_RFSAPR_MAX_PLL_CHANNEL_NUMBER)
							{
								sRetVal = DP_RFSAPR_INVALID_PLL_NUM;
								break;
							}

							memcpy(&S_Config_TRF_PLL,&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_TRF_PLLConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_TRF_PLL.ucBoardNumber],sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL));
						
#ifndef WIN32
							/*sRetVal = DP_RFSAPR_CmdRFSAPRMessage_Read_TRF_PLL_Data(&S_Config_TRF_PLL);*/
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{							
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_CmdRFSAPRMessage_Read_TRF_PLL_Data...\r\n");
								break;
							}
#endif

							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Config_TRF_PLL,&S_Config_TRF_PLL,sizeof(S_DP_VPX_1502_CONFIG_TRF_PLL));
							ulResMessageSize = sizeof( S_DP_VPX_1502_CONFIG_TRF_PLL);
						}
						
						break;
		

					case RFSAPR_CMD_CHECK_BOARD_NUMBERS:
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CHECK_BOARD_NUMBERS;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG						
						dp_rfsapr_printf("INFO\t: Check Board Numbers Command Received\r\n\r\n");						
#endif

						if(ulRxMsgLength != sizeof(S_CHECK_BOARD_STATUS))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
#ifdef DP_RFSAPR_DRX_DEBUG_MSG						
							dp_rfsapr_printf("INFO\t: Received Board Number\t\t: %d\r\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_CheckAvailableBoardNum.ucBoardNumber + 1));
							dp_rfsapr_printf("INFO\t: Available number of boards\t: %d\r\n", g_RFSAPR_Handle.usNoOfBoards);
#endif
							/*sRetVal = DP_RFSAPR_CheckBoard_Status(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_CheckAvailableBoardNum);*/
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_CheckBoard_Status...\r\n");
							}						
							ulResMessageSize = sizeof(S_CHECK_BOARD_STATUS);
						}												

						break;


					case RFSAPR_CMD_FPGA_WRITE_LOG_ADC_CONFIG:
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FPGA_WRITE_LOG_ADC_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Log ADC Config Command Received \r\n");
#endif
						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_LOG_ADC))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: Log ADC Config Command Received \r\n");
							dp_rfsapr_printf("INFO\t: Board Number\t\t: %d\tLog ADC Selection\t: %d\r\n\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGALogADCConfig.ucBoardNumber + 1), (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGALogADCConfig.ucLogADCSelection + 1));
#endif
							memcpy(&S_Config_Log_ADC, &pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGALogADCConfig, sizeof(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGALogADCConfig));

							sRetVal = DP_RFSAPR_LOG_ADC_SPI_Config(S_Config_Log_ADC.ucBoardNumber, &S_Config_Log_ADC);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_Write_Log_ADC_Data...\r\n");
							}
							else
							{
								if(S_Config_Log_ADC.ucBoardNumber < DP_RFSAPR_DRX_MAX_BOARD_COUNT)
								{
									memcpy(&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_LOG_ADC_ConfigData[S_Config_Log_ADC.ucBoardNumber], &S_Config_Log_ADC, sizeof(S_Config_Log_ADC));
									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_FPGA_DRX_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.S_DPVPX1502_Configurations, (unsigned long)sizeof(g_RFSAPR_Handle.S_DPVPX1502_Configurations));
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: Log ADC Configuration store to file\r\n");
									}
									
									dp_rfsapr_printf("SUCCESS\t: Log ADC Configuration store to file\r\n");
								}
								else
								{
									dp_rfsapr_printf("ERROR\t: Invalid board number\r\n");
								}
							}
						}

						break;

										
					case RFSAPR_CMD_PBIT:
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						printf("INFO\t: Perform PBIT Command and Return status  \r\n");
#endif
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_PBIT;
						sRetVal =  DP_RFSAPR_Perform_PBIT(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_PBIT_Status);

						ulResMessageSize = sizeof(S_RFSAPR_PERFORM_PBIT_TEST);
						
						dp_rfsapr_i2cuart_config(1);  /*Commented By Srinu @08082020, Here its working*/
						
						dp_rfsapr_i2cuart_config(2);
						
						break;

					case RFSAPR_CMD_READ_BOARD_DET_STATUS:
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Perform Board Detection Command Received  \r\n");
#endif
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_BOARD_DET_STATUS;
						if(ulRxMsgLength != sizeof(S_CMD_BOARD_DET_STATUS))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							
							DP_RFSAPR_Detect_Board_Status(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_Brd_Sts,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_Brd_Sts);
							
							ulResMessageSize = sizeof(S_RES_BOARD_DET_STATUS);
						}
						break;
						
					case RFSAPR_CMD_READ_ADC_FFT_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_ADC_FFT_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read ADC Data Command Received \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_CMD_FFT_DATA_AQU))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							DP_RFSAPR_DPVPX1502_ReadADC_FIFO_Data(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FFT_ADC_Info ,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_FFT_ADC_Data);
						
							ulResMessageSize = sizeof(S_RFSAPR_RES_ADC_ACQUIS_TEST);
						}
						
						break;

					case RFSAPR_CMD_READ_NB_ADC_FFT_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NB_ADC_FFT_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read NB ADC Data Command Received \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_CMD_NB_FFT_DATA_AQU))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{

							DP_RFSAPR_DPVPX1013_ReadADC_FIFO_Data(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_NB_ADC_Data_Acq ,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_FFT_ADC_Data);

							ulResMessageSize = sizeof(S_RFSAPR_RES_NB_ADC_ACQUIS_TEST);
						}
						
						break;						
						
					case RFSAPR_CMD_READ_ADC_FIFO_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_ADC_FIFO_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read RF ADC FIFO Data Command Received\r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_CMD_ADC_FIFO_DATA_AQU))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							DP_RFSAPR_Read_ADC_FIFOData(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_ADC_FIFO_Data, &pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ADC_FIFO_Data);

							ulResMessageSize = sizeof(S_RFSAPR_RES_ADC_FIFO_DATA);
						}
						
						break;	

					case RFSAPR_CMD_READ_ALL_BAND_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_ALL_BAND_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read All Band Data Command Received...\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_READ_ALL_BAND_DATA))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							DP_RFSAPR_Read_All_Band_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadAllBandData, &pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_AllBandReadData);
							
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_ALL_BAND_DATA);
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Response Message Size : %d\r\n", ulResMessageSize);
#endif							
						}
						break;
						
					case RFSAPR_CMD_WB_ACQUIRE_ALL_BAND_DMA_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WB_ACQUIRE_ALL_BAND_DMA_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: DMA Read All Band Data Command Received...\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_READ_ALL_BAND_DMA_DATA))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							DP_RFSAPR_DMA_Read_All_Band_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_All_Band_DataRead, &pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_AllBandReadData);
							
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_ALL_BAND_DATA);
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Response Message Size : %d\r\n", ulResMessageSize);
#endif							
						}
						break;						
						
					case RFSAPR_CMD_READ_LOG_ADC_ALL_BAND_DATA:

							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_LOG_ADC_ALL_BAND_DATA;

	#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: Read Log ADC All Band Data Command Received...\r\n");
	#endif
							if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_READ_ALL_BAND_DATA))
							{
								sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
							}
							else
							{
								sRetVal = DP_RFSAPR_Read_LogADC_All_Band_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadAllBandData, &pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_AllBandReadData);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{ 
									dp_rfsapr_printf("ERROR\t: Read Log ADC All Band Data Command\r\n");
								}
								else
								{
									dp_rfsapr_printf("SUCCESS\t: Read Log ADC All Band Data Command\r\n");
								}
								
								ulResMessageSize = sizeof(S_RFSAPR_RES_READ_ALL_BAND_DATA);
	#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: Response Message Size : %d\r\n", ulResMessageSize);
	#endif							
							}
							break;
							

					case RFSAPR_CMD_UPLOAD_TO_TARGET_LOG_ADC_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_UPLOAD_TO_TARGET_LOG_ADC_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Log ADC Calibration Data upload to target Command Received...\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_LOG_ADC_CALIB_DATA))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
						//	DP_RFSAPR_LogADCCalibData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_LogADCCalibData);
							
							/*Store to bin file */
							sRetVal = DP_RFSAPR_AccessBinFile((DP_RFSAPR_ANT_1_LOG_ADC_CALIB_CONFIG_BIN_FILE + pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_LogADCCalibData.m_ucAntennaSel), DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_LogADCCalibData, (unsigned long)sizeof(S_RFSAPR_CMD_LOG_ADC_CALIB_DATA));
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{ 
								dp_rfsapr_printf("ERROR\t: Log ADC Calibration Data store to file\r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: Log ADC Calibration Data store to file\r\n");
							}
							
							ulResMessageSize = sizeof(S_RFSAPR_CMD_LOG_ADC_CALIB_DATA);
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Response Message Size : %d\r\n", ulResMessageSize);
#endif							
						}
						break;	
						
					case RFSAPR_CMD_1502_READ_DMA_DATA:
		
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1502_READ_DMA_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_1502_READ_DMA_DATA Command Received...\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_READ_DMA_DATA))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							sRetVal = RFSAPR_Read_1502_DMA_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1502_ReadDMAData ,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_1502_ReadDMAData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{ 
								dp_rfsapr_printf("ERROR\t: RFSAPR_CMD_1502_READ_DMA_DATA Command\r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: RFSAPR_CMD_1502_READ_DMA_DATA Command\r\n");
							}
							
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_DMA_DATA);
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Response Message Size : %d\r\n", ulResMessageSize);
#endif							
						}
						break;				
						
					case RFSAPR_CMD_1502_READ_LOG_ADC_AVG_VOLT:
		
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1502_READ_LOG_ADC_AVG_VOLT;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_1502_READ_LOG_ADC_AVG_VOLT Command Received...\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_READ_LOG_ADC_VOLT))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							sRetVal = RFSAPR_Read_Log_ADC_Avg_Voltage(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadLogADCAvgVolt ,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_1502_ReadLogADCAvgVolt);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{ 
								dp_rfsapr_printf("ERROR\t: RFSAPR_CMD_1502_READ_LOG_ADC_AVG_VOLT Command\r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: RFSAPR_CMD_1502_READ_LOG_ADC_AVG_VOLT Command\r\n");
							}
							
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_LOG_ADC_VOLT);
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Response Message Size : %d\r\n", ulResMessageSize);
#endif							
						}
						break;							

					case RFSAPR_CMD_NB_ADC_FIFO_RESET:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_NB_ADC_FIFO_RESET;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t:  RFSAPR_CMD_NB_ADC_FIFO_RESET Command Received.. Latest \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_CMD_NB_ADC_FIFO_RESET))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							DP_RFSAPR_DPVPX1013_ADC_FIFO_Reset(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_NB_ADC_FIFO_Reset);

							ulResMessageSize = 0;
						}
						
						break;																	

					case RFSAPR_CMD_READ_NB_ADC_FIFO_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NB_ADC_FIFO_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t:  Read NB RF ADC FIFO Data Command Received.. Latest \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_CMD_NB_ADC_FIFO_DATA_AQU))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							DP_RFSAPR_Read_NB_ADC_FIFOData(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_NB_ADC_FIFO_Data, &pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_NB_ADC_FIFO_Data);

							ulResMessageSize = sizeof(S_RFSAPR_RES_NB_ADC_FIFO_DATA);
						}
						
						break;						
						
						
					case RFSAPR_CMD_READ_LOG_ADC_FIFO_DATA:
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_LOG_ADC_FIFO_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t:  Read LOG ADC FIFO Data Command Received.. Latest \r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_CMD_LOG_ADC_FIFO_DATA_AQU))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							DP_RFSAPR_Read_LOG_ADC_FIFOData(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_Log_ADC_FIFO_Data, &pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Log_ADC_FIFO_Data);

							ulResMessageSize = sizeof(S_RFSAPR_RES_LOG_ADC_FIFO_DATA);
						}
						
						break;
						/*Cmd to write the Threshold value*/
					case RFSAPR_CMD_WRITE_LOG_ADC_THRESHOLD_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_LOG_ADC_THRESHOLD_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t:  Write RFSAPR_CMD_WRITE_LOG_ADC_THRESHOLD_DATA Command Received.. \r\n");
#endif

						if(ulRxMsgLength != sizeof(S_CMD_LOG_ADC_THRESHOLD_DATA))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							DP_RFSAPR_Write_LOG_ADC_Threshold(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Write_Log_ADC_Threshold_Data);

							ulResMessageSize = 0;
						}

						break;


						/* Cmd for Log ADC pulse ON Time, OFF Time and amplitude (Manual debugging in Host application)*/	
					case RFSAPR_CMD_CONFIG_LOG_VIDEO_CH_READ_DATA:

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIG_LOG_VIDEO_CH_READ_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t:  Read Log ADC pulse ON Time, OFF Time and amplitude Command Received.. \r\n");
#endif
											
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_CONFIGURE_LOG_VIDEO_CHANNEL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							dp_rfsapr_printf("INFO\t:  DP_RFSAPR_Read_LOG_ADC_PulseData Fun.. \r\n");
							DP_RFSAPR_Read_LOG_ADC_PulseData(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_sCmd_Config_Log_Video_Ch, &pSResponseToRFSAPR->UResRFSAPRMessage.m_sRes_Read_Log_Video_Pulse);

							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_LOG_VIDEO_PULSE);
						}
											
					break;
						
					case RFSAPR_CMD_READ_FPGA_ADC_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_FPGA_ADC_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read ADC Command Received\r\n");
#endif

						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_ADC))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ConfigADC.ucBoardNumber >= DP_RFSAPR_DRX_MAX_BOARD_COUNT)
							{
								sRetVal =  DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ConfigADC.ucADCSelection >= DP_RFSAPR_DRX_ADC_CH_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_ADC_CHN_NUM;
								break;
							}

							memcpy(&S_ConfigADC,&g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_DPVPX1502_ADCConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ConfigADC.ucBoardNumber],sizeof(S_DP_VPX_1502_CONFIG_ADC));

							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResADC_Data,&S_ConfigADC,sizeof(S_DP_VPX_1502_CONFIG_ADC));
							ulResMessageSize = sizeof( S_DP_VPX_1502_CONFIG_ADC);
						}						
						break;
					case RFSAPR_CMD_READ_NB_FPGA_ADC_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NB_FPGA_ADC_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read NB ADC Command Received\r\n");
#endif

						if(ulRxMsgLength != sizeof(S_DP_VPX_1013_CONFIG_ADC))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_ConfigADC.ucBoardNumber >= DP_RFSAPR_NB_DRX_MAX_BOARD_COUNT)
							{
								sRetVal =  DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_ConfigADC.ucADCSelection >= DP_RFSAPR_DRX_ADC_CH_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_ADC_CHN_NUM;
								break;
							}

							memcpy(&S_NB_ConfigADC,&g_RFSAPR_Handle.S_DPVPX1013_Configurations.S_DPVPX1013_ADCConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_NB_ConfigADC.ucBoardNumber],sizeof(S_DP_VPX_1013_CONFIG_ADC));

							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_NB_ADC_Data,&S_NB_ConfigADC,sizeof(S_DP_VPX_1013_CONFIG_ADC));
							ulResMessageSize = sizeof( S_DP_VPX_1013_CONFIG_ADC);
						}						
						break;						
						
					case RFSAPR_CMD_FPGA_READ_LOG_ADC_CONFIG:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FPGA_READ_LOG_ADC_CONFIG;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read Log ADC Configuration Command Received\r\n");						
#endif
						
						if(ulRxMsgLength != sizeof(S_DP_VPX_1502_CONFIG_ADC))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGALogADCConfig.ucBoardNumber >= DP_RFSAPR_DRX_MAX_BOARD_COUNT)
							{
								sRetVal =  DP_RFSAPR_INVALID_BOARD_NUM;
								break;
							}
								
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGALogADCConfig.ucLogADCSelection >= DP_RFSAPR_MAX_LOG_ADC_CH_COUNT)
							{
								sRetVal = DP_RFSAPR_INVALID_LOG_ADC_CH_NUM;
								break;
							}					

							memcpy(&S_Config_Log_ADC, &g_RFSAPR_Handle.S_DPVPX1502_Configurations.S_LOG_ADC_ConfigData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_FPGALogADCConfig.ucBoardNumber],sizeof(S_DP_VPX_1502_CONFIG_LOG_ADC));
						
							memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResLOGADC_Data, &S_Config_Log_ADC, sizeof(S_DP_VPX_1502_CONFIG_LOG_ADC));
							ulResMessageSize = sizeof( S_DP_VPX_1502_CONFIG_LOG_ADC);
						}						
						break;

						
					case RFSAPR_CMD_SET_RFSAPR_SYSTEM_TIME:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SET_RFSAPR_SYSTEM_TIME;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR System date/time configure command received\r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_DP_SET_RFSAPR_SYSTEM_TIME))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
#ifdef __vxworks
							sRetVal = 0;
#endif
						}

						break;
																	
					case RFSAPR_CMD_RF_ATTENUATOR_CONTROL:

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RF Attenuation Control Command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_CMD_RF_ATTENUATION_CONTROL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{	
					
							sRetVal = DP_RFSAPR_RF_Attenuation_Control(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfAttenuationCtrl);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_RF_Attenaution_Control \r\n");
							}

						}
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_RF_ATTENUATOR_CONTROL;
						
						dp_rfsapr_printf("INFO\t: In RF Attenuation Control Command Received Unit ID : %d\r\n",pSResponseToRFSAPR->SRFSAPRResIds.usUnitId);
						
						break;
						
					case RFSAPR_CMD_UPDATE_RF_FRONTEND_CALIB:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_UPDATE_RF_FRONTEND_CALIB;
						
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RF Calibration data update command received : %d\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfCalibData.ulAntennaNumber);
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE))
						{
							dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							for(int i = 0; i<17;i++)
							{
								printf("INFO\t: Before DP_RFSAPR_Update_RF_CalibData - RF_Attenuation_dB : %f, Composite Attenuation : %f\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfCalibData.arr_fRF_Attenuation_dB[i], pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfCalibData.arr_fCompositeAttenuation_dB[i]);
							}
							
							for(int j = 0; j<5 ; j++)
							{
								printf("INFO\t: Before DP_RFSAPR_Update_RF_CalibData - BDC Attenuation data : %f\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfCalibData.arr_fRF_Attenuation_dB[17+j]);
							}	
							
							/*Apply in DRX*/
							sRetVal = DP_RFSAPR_All_RF_Attenuation_Control(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfCalibData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_All_RF_Attenuation_Control \r\n");
							}
							
							/*Store to bin file */
							sRetVal = DP_RFSAPR_AccessBinFile((DP_RFSAPR_ANT_1_RF_ADC_ATTEN_CONFIG_BIN_FILE + pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfCalibData.ulAntennaNumber), DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfCalibData, (unsigned long)sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE));
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: RF Front end Attenuator Configuration store to file\r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: RF Front end Attenuator Data store to file\r\n");
							}
						}
						break;

					case RFSAPR_CMD_READ_RF_FRONTEND_CALIB:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_RF_FRONTEND_CALIB;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: RF Calibration data read command received\r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							sRetVal = DP_RFSAPR_AccessBinFile((DP_RFSAPR_ANT_1_RF_ADC_ATTEN_CONFIG_BIN_FILE + pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RfCalibData.ulAntennaNumber), DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_RfGainCalibTable, (unsigned long)sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE));
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: RF Front end Attenuator Configuration read from file\r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: RF Front end Attenuator Data read from file\r\n");
							}
							
							
							ulResMessageSize = sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE);
						}

						break;

					case RFSAPR_CMD_UPDATE_GAIN_SW_CALIB_DATA:
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_UPDATE_GAIN_SW_CALIB_DATA;
					
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: Auto Gain Software Calibration Data Update Command Received... \r\n");
#endif
					if(ulRxMsgLength != sizeof(S_RFSAPR_GAIN_SW_CALIB_DATA))
					{
						dp_rfsapr_printf("ERROR\t: ulRxMsgLength : %d and sizeof(S_RFSAPR_GAIN_SW_CALIB_DATA) = %d\r\n", ulRxMsgLength,sizeof(S_RFSAPR_GAIN_SW_CALIB_DATA));
						
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						memcpy((void *)&g_RFSAPR_Handle.m_sSWCalibData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RFSAPR_GainSwCalibData.m_ucAntennaSel][pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RFSAPR_GainSwCalibData.m_ucBandSel], (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RFSAPR_GainSwCalibData.m_sSWcalibData, sizeof(S_SW_CALIB_DATA));
												
					//	dp_rfsapr_printf("INFO\t: ...In  g_RFSAPR_Handle.m_sSWCalibData[%d][%d].m_usFreqSpotCnt = %d\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RFSAPR_GainSwCalibData.m_ucAntennaSel, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_RFSAPR_GainSwCalibData.m_ucBandSel,g_RFSAPR_Handle.m_sSWCalibData[1][3].m_usFreqSpotCnt);
												
					/*	for(int iTemp = 0; iTemp < g_RFSAPR_Handle.m_sSWCalibData[1][3].m_usFreqSpotCnt ; iTemp++)
						{
							dp_rfsapr_printf("INFO\t: ...Freq = %lf and Data = %lf\r\n", g_RFSAPR_Handle.m_sSWCalibData[1][3].m_dFreq[iTemp], g_RFSAPR_Handle.m_sSWCalibData[1][3].m_dSWCalib[iTemp]);
						} */
						
						/* Store to bin file */
						sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_SW_GAINCALIB_DATA_CONFIG_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.m_sSWCalibData, (unsigned long)sizeof(g_RFSAPR_Handle.m_sSWCalibData));
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Auto Gain Software Calibration Data Configuration store to file\r\n");
						}
						else
						{
							dp_rfsapr_printf("SUCCESS\t: Auto Gain Software Calibration Data Configuration Store to file\r\n");
						}
					}
					break;
					
					case RFSAPR_CMD_STORE_TEMP_HW_BASED_CALIB:
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_STORE_TEMP_HW_BASED_CALIB;
					
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: Temp HW Gain Calibration data constants Update Command Received... \r\n");
#endif
					if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_STORE_HW_CALIB_CONST))
					{
						dp_rfsapr_printf("ERROR\t: ulRxMsgLength : %d and size of(S_RFSAPR_CMD_STORE_HW_CALIB_CONST) = %d\r\n", ulRxMsgLength,sizeof(S_RFSAPR_CMD_STORE_HW_CALIB_CONST));
						
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						
						memcpy((void *)&g_RFSAPR_Handle.m_sTempHWCalConst[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdStoreHWCalibConst.m_ucAntNo], (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdStoreHWCalibConst, sizeof(S_RFSAPR_CMD_STORE_HW_CALIB_CONST));						
																								
					/*	for(int iTemp = 0; iTemp < g_RFSAPR_Handle.m_sSWCalibData[1][3].m_usFreqSpotCnt ; iTemp++)
						{
							dp_rfsapr_printf("INFO\t: ...Freq = %lf and Data = %lf\r\n", g_RFSAPR_Handle.m_sSWCalibData[1][3].m_dFreq[iTemp], g_RFSAPR_Handle.m_sSWCalibData[1][3].m_dSWCalib[iTemp]);
						} */
						
						/* Store to bin file */
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdStoreHWCalibConst.m_ucAntNo == 0)
						{						
							sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT1_TEMP_HW_CALIB_DATA_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.m_sTempHWCalConst[0], (unsigned long)sizeof(S_RFSAPR_CMD_STORE_HW_CALIB_CONST));
						}
						else
						{
							sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT2_TEMP_HW_CALIB_DATA_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&g_RFSAPR_Handle.m_sTempHWCalConst[1], (unsigned long)sizeof(S_RFSAPR_CMD_STORE_HW_CALIB_CONST));													
						}						
						
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Temp HW Gain Calibration constants Configuration not stored to file\r\n");
						}
						else
						{
							dp_rfsapr_printf("SUCCESS\t: Auto Temp HW Gain Calibration constants Configuration Stored to file\r\n");
						}
						
					}
					break;

					case RFSAPR_CMD_READ_TEMP_HW_BASED_CALIB:
											
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_TEMP_HW_BASED_CALIB;
					
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: Temp HW Gain Calibration Data Command Received... \r\n");
#endif
					if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_READ_HW_CALIB_CONST))
					{
						dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdReadHWCalibConst.m_ucAntNo == 0)
						{
							sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT1_TEMP_HW_CALIB_DATA_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResTempHWCalibRead, (unsigned long)sizeof(S_RFSAPR_RES_READ_HW_CALIB_CONST));
						}
						else
						{
							sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT2_TEMP_HW_CALIB_DATA_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResTempHWCalibRead, (unsigned long)sizeof(S_RFSAPR_RES_READ_HW_CALIB_CONST));
						}
						
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Read Temp HW Gain calibration data for ANT - %d from file failed\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdReadHWCalibConst.m_ucAntNo+1);
						}
						else
						{
							dp_rfsapr_printf("SUCCESS\t: Read Temp HW Gain calibration data for ANT - %d from file\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdReadHWCalibConst.m_ucAntNo+1);
						}
						
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_HW_CALIB_CONST);
						
					}
					break;					

					case RFSAPR_CMD_READ_GAIN_SW_CALIB_DATA:
											
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_GAIN_SW_CALIB_DATA;
					
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: Read Software Gain Calibration Data Command Received... \r\n");
#endif
					if(ulRxMsgLength != sizeof(S_RFSAPR_GAIN_SW_CALIB_DATA))
					{
						dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_SW_GAINCALIB_DATA_CONFIG_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&g_RFSAPR_Handle.m_sSWCalibData, (unsigned long)sizeof(g_RFSAPR_Handle.m_sSWCalibData));
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Read Software calibration data from file\r\n");
						}
						else
						{
							dp_rfsapr_printf("SUCCESS\t: Read Software calibration data from file\r\n");
						}
						
						pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_GainSwCalibData.m_ucAntennaSel	= pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_CmdReadSwCalibData.m_ucAntennaSel;
						pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_GainSwCalibData.m_ucBandSel		= pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_CmdReadSwCalibData.m_ucBandSel;
						memcpy((void *)&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_GainSwCalibData.m_sSWcalibData, (void *)&g_RFSAPR_Handle.m_sSWCalibData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_CmdReadSwCalibData.m_ucAntennaSel][pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_CmdReadSwCalibData.m_ucBandSel], sizeof(S_SW_CALIB_DATA));
					}
					break;
					
					case RFSAPR_CMD_READ_ALL_SW_GAIN_CALIB_DATA:
										
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_ALL_SW_GAIN_CALIB_DATA;
					
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: Read All band Software Gain Calibration Data Command Received... \r\n");
#endif
					if(ulRxMsgLength != 0)
					{
						dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						
					//	sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_SW_GAINCALIB_DATA_CONFIG_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&g_RFSAPR_Handle.m_sSWCalibData, (unsigned long)sizeof(g_RFSAPR_Handle.m_sSWCalibData));
						sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_SW_GAINCALIB_DATA_CONFIG_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ReadAllGainSwCalibData, (unsigned long)sizeof(g_RFSAPR_Handle.m_sSWCalibData));
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Read All band Software calibration data from file\r\n");
						}
						else
						{
							dp_rfsapr_printf("SUCCESS\t: Read All band Software calibration data from file\r\n");
						}
					
					//	memcpy(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ReadAllGainSwCalibData, g_RFSAPR_Handle.m_sSWCalibData, sizeof(g_RFSAPR_Handle.m_sSWCalibData));
					//	dp_rfsapr_printf("INFO\t: ...g_RFSAPR_Handle.m_sSWCalibData.m_usFreqSpotCnt = %d and pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ReadAllGainSwCalibData[1][3].m_sSWcalibData.m_usFreqSpotCnt = %d\r\n",g_RFSAPR_Handle.m_sSWCalibData[1][3].m_usFreqSpotCnt, pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ReadAllGainSwCalibData[1][3].m_sSWcalibData.m_usFreqSpotCnt);
					//	dp_rfsapr_printf("INFO\t: ...g_RFSAPR_Handle.m_sSWCalibData.m_usFreqSpotCnt = %d and pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ReadAllGainSwCalibData[1][3].m_sSWcalibData.m_usFreqSpotCnt = %d\r\n",g_RFSAPR_Handle.m_sSWCalibData[1][3].m_usFreqSpotCnt, pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ReadAllGainSwCalibData[1][3].m_usFreqSpotCnt);
						
						ulResMessageSize = sizeof(pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ReadAllGainSwCalibData);
						
						printf("Size of response message :%d \n",ulResMessageSize);
					}
					break;
					/*************************************IOMATRIX CONFIG MANUAL MODE*******************************/
				
					case RFSAPR_CMD_IO_RF_CONFIG_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_IO_RF_CONFIG_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: IO Matrix RF Path Selection command received  ... \r\n");
#endif
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							printf("INFO\t: Brd Selection :%d \r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdIORFConfig.m_ucBrdSel);
							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdIORFConfig.m_ucBrdSel == 0) 
							{
								printf("INFO\t: DP_RFSAPR_WB_IO_Matrix_RF_SW_Path_Config\r\n");
								sRetVal = DP_RFSAPR_WB_IO_Matrix_RF_SW_Path_Config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdIORFConfig.m_ucPathSel);
							}
							else if (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdIORFConfig.m_ucBrdSel == 1)
							{
								printf("INFO\t: DP_RFSAPR_NB_IO_Matrix_RF_SW_Path_Config\r\n");
								sRetVal = DP_RFSAPR_NB_IO_Matrix_RF_SW_Path_Config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdIORFConfig.m_ucPathSel);
							}
							else 
							{
								dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
								dp_rfsapr_printf("ERROR\t: Invalid Band Selection \r\n");
							}
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: IO Matrix Configuration not changed \r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: IO Matrix Configuration has been changed\r\n");
							}

							ulResMessageSize = 0;						
						}
						break;	
					case RFSAPR_CMD_IO_ATTENUATION_CONFIG_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_IO_ATTENUATION_CONFIG_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: IO Matrix Attenuation Selection command received  ... \r\n");
#endif
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							sRetVal = DP_RFSAPR_IO_Matrix_Attenuation_config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdIOAttenuationConfig);
							
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: IO Matrix Configuration not changed \r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: IO Matrix Configuration has been changed\r\n");
							}

							ulResMessageSize = 0;						
						}
						break;	
					case RFSAPR_CMD_IO_BITE_CONFIG_DATA:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_IO_BITE_CONFIG_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: IO Matrix BITE Source Configuration command recieved   ... \r\n");
#endif
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							sRetVal = DP_RFSAPR_IO_Matrix_BITE_Freq_Config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdIOBITEConfig.m_ucFreqSel);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: IO Matrix BITE Source Configuration Changed\r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: IO Matrix BITE Source Configuration not Changed \r\n");
							}

							ulResMessageSize = 0;						
						}
						break;	
					/********************IO MATRIX CONFIG END ************************/	
					
					case RFSAPR_CMD_NBDC_IOMATRIX_UART_WRITE_DATA:
				

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_NBDC_IOMATRIX_UART_WRITE_DATA;
	
	#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: NBDC & IO Matrix UART write data command recieved   ... \r\n");
	#endif
					if(ulRxMsgLength == 0)
					{
						dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						dp_rfsapr_printf("INFO\t: Else\r\n");
						
				
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTWRConfig.m_ucModuleSel == 1) /*IO Matrix*/
						{
							/*Configure the UART 10 port in DP-XMC-5028 Card*/
							sRetVal = DP_RFSAPR_IOMatrix_RS422_Wrapper_Open(10 /*Port Number*/, 115200/*Buad Rate*/,  8/*Data Length*/, 2 /*Odd Parity*/,1 /*Stop Bits*/);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART port configured failed in XMC Boadrd\r\n");
							}
													
							ulResMessageSize = 0;
						}
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTWRConfig.m_ucModuleSel == 2) /*NB BDC*/
						{
							dp_rfsapr_printf("INFO\t: NB Modules\r\n");
							
								/*Configure the 12C/UART bridge in NBDRx Module*/
								sRetVal = dp_rfsapr_i2cuart_config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTWRConfig.m_ucNBDCUARTCh);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: UART port not configured\r\n");
								}
								/*Write data in NBCD thru UART communication*/
							/*	sRetVal = DP_RFSAPR_NBDC_RS422_Wrapper_WriteData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTWRConfig.m_ucNBDCUARTCh, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTWRConfig.m_ulCmdWrd, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTWRConfig.m_uiData,100);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: UART port not configured\r\n");
								}*/	
							ulResMessageSize = 0;
						}
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ucModuleSel == 3)/*NBDS*/
						{
							/*Write data in NBDS thru I2C communication*/
							sRetVal = DP_RFSAPR_NBDS_I2C_Wrapper_WriteData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTWRConfig.m_ulCmdWrd, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTWRConfig.m_uiData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t:I2C data write failed for NBDS\r\n");
							}
							
							ulResMessageSize = 0;
						}
					}
					break;
				
					case RFSAPR_CMD_NBDC_IOMATRIX_UART_READ_DATA:

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_NBDC_IOMATRIX_UART_READ_DATA;
	
	#ifdef DP_RFSAPR_DRX_DEBUG_MSG
					dp_rfsapr_printf("INFO\t: NBDC & IO Matrix UART read data command recieved   ... \r\n");
	#endif
					if(ulRxMsgLength == 0)
					{
						dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ucModuleSel == 1) /*IO Matrix*/
						{
							/*Read data from IO Matrix thru UART communication*/
							sRetVal = DP_RFSAPR_IOMatrix_RS422_Wrapper_ReadData(10/*UART port number*/,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ulCmdWrd, 5 ,/*Ack Delay*/ 50,/*Resp Ack Delay*/ &pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBDCIOMatrixUARTRdConfig.m_ulReadUARTData4Bytes);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART data read failed for IO Matrix\r\n");
							}
							/*Close the UART port*/
							/*sRetVal = DP_RFSAPR_IOMatrix_RS422_Wrapper_Close(10);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART port not closed\r\n");
							}*/
							ulResMessageSize = sizeof(S_RFSAPR_RES_NBDC_IOMATRIX_UART_READ_DATA);
						}
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ucModuleSel == 2)/*NB BDC*/
						{
							memset(&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBDCIOMatrixUARTRdConfig,0,sizeof(pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBDCIOMatrixUARTRdConfig));
							/*Read data from NBCD & IO Matrix thru UART communication*/
							sRetVal = DP_RFSAPR_NBDC_UART_Wrapper_ReadData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ucNBDCUARTCh/*UART port number*/,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ulCmdWrd, 100 ,/*Ack Delay*/ 100,/*Resp Ack Delay*/ &pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBDCIOMatrixUARTRdConfig);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t:UART data read failed for NBDC\r\n");
							}
							/*dp_rfsapr_printf("INFO\t: Response UART Read Data(4 Bytes) :  %X \r\n", pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBDCIOMatrixUARTRdConfig.m_ulReadUARTData4Bytes);
							dp_rfsapr_printf("INFO\t: Response UART Read Data(1 Byte) :  %X \r\n", pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBDCIOMatrixUARTRdConfig.m_ulReadUARTData1Byte);*/
							ulResMessageSize = sizeof(S_RFSAPR_RES_NBDC_IOMATRIX_UART_READ_DATA);
						}
						if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ucModuleSel == 3)/*NBDS*/
							{
								/*Read data from NBDS module thru I2C communication*/
								//sRetVal = DP_RFSAPR_NBDS_I2C_Wrapper_ReadData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ulCmdWrd, &pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBDCIOMatrixUARTRdConfig.m_ulReadUARTData4Bytes);
								sRetVal = DP_RFSAPR_NBDC_i2c_Read(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCIOMatrixUARTRdConfig.m_ulCmdWrd, &pSResponseToRFSAPR->UResRFSAPRMessage.S_ResNBDCIOMatrixUARTRdConfig.m_ulReadUARTData4Bytes);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t:I2C data read failed for NBDS\r\n");
								}
								
								ulResMessageSize = sizeof(S_RFSAPR_RES_NBDC_IOMATRIX_UART_READ_DATA);
							}
					}
					break;

						/************************JTAG SEL**************/
					case RFSAPR_CMD_JTAG_SELECTION:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_JTAG_SELECTION;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: JTAG Selection command received  ... \r\n");
#endif
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{

							sRetVal = DP_RFSAPR_JTAG_Selection(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_JTAG_Selection.m_ucIntExtSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_JTAG_Selection.m_ucBrdSwtch);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: JTAG Selection has not changed \r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: JTAG Selection has been changed\r\n");
							}

							ulResMessageSize = 0;						
						}
						break;	

					case RFSAPR_CMD_SBC_JTAG_SEL:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_JTAG_SELECTION;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: JTAG Selection command received  ... \r\n");
#endif
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{

							sRetVal = DP_RFSAPR_SBC_JTAG_Selection(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdSBCJtagSel.m_ucBrdNo);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: JTAG Selection has not changed \r\n");
							}
							else
							{
								dp_rfsapr_printf("SUCCESS\t: JTAG Selection has been changed\r\n");
							}

							ulResMessageSize = 0;						
						}
						break;	

					/************************************JTAG SEL END*******************************/
					 case RFSAPR_CMD_READ_SENSOR_DATA:

						 pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_SENSOR_DATA;

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: Read Sensor Data Command Received\r\n");
#endif

						if(ulRxMsgLength != 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{							
							DP_RFSAPR_Temperature_Sensor_Read(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_NB_Voltage,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Temperature_Sensor);
							
							ulResMessageSize = sizeof(S_RFSAPR_TEMPERATURE_MEASUREMENTS);
						}
						
						break;
						
						case RFSAPR_CMD_READ_NB_READ_VOLTAGE:
						
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NB_READ_VOLTAGE;
							
	#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_NB_READ_VOLTAGE command Received\r\n");
	#endif

							if(ulRxMsgLength == 0)
							{
								sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
							}
							else
							{
								
								sRetVal = DP_RFSAPR_NB_Voltage_Read(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_NB_Voltage,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_NB_Voltage);
								if(sRetVal)
								{								
									ulResMessageSize = 0;
								}
								else
								{
									ulResMessageSize = sizeof(S_RFSAPR_RES_READ_NB_VOLTAGE);
								}											
							}
							
					break;						
						
					case RFSAPR_CMD_CONFIG_NARROWBAND_BDC:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIG_NARROWBAND_BDC;

						printf("NB BDC Command Received \r\n");
						
						
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						printf("INFO\t: RFSAPR_CMD_CONFIG_NARROWBAND_BDC command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_CONFIG_NB_BDC))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{
							
							if((pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdNBBDCConfig.m_ucModSel >= 1) && (pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdNBBDCConfig.m_ucModSel <= 3))
							{
								sRetVal = DP_RFSAPR_NB_BDC_and_Synthesizer_config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdNBBDCConfig);
							}
							else
							{
								sRetVal = DP_RFSAPR_NB_SynthesizerBDC_config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdNBBDCConfig);								
							}
							
							ulResMessageSize = 0;
								
							
						}							

						break;
						
					case RFSAPR_CMD_CONFIGURE_NB_BDS_AND_BDC:
					
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIGURE_NB_BDS_AND_BDC;

						printf("NB BDC and BDS Configure Command Received \r\n");
						
						
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						printf("INFO\t: RFSAPR_CMD_CONFIGURE_NB_BDS_AND_BDC command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_CONFIGURE_NBBDC_RFBUS))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_Configure_NB_BDC_RFBUS(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_CmdNBRFBusConfig);						
							ulResMessageSize = 0;							
						}							

						break;
						
						/********************Added By sapthagiri For NB Boards*********************/
												case RFSAPR_CMD_CONFIGURE_NBDC: 

												pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIGURE_NBDC;

												printf("NB BDC Command Received \r\n");
												
												
						#ifdef DP_RFSAPR_DRX_DEBUG_MSG
												printf("INFO\t: RFSAPR_CMD_CONFIG_NARROWBAND_BDC command Received\r\n");
						#endif
												/*if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_NBBDC_CONFIG))*/
												if(ulRxMsgLength == 0)
												{
													sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
												}
												else	
												{
													
												
														sRetVal = DP_RFSAPR_NB_BDC_config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCConfig);
													
													
													ulResMessageSize = 0;
														
													
												}							

												break;
												
												case RFSAPR_CMD_CONFIGURE_MISSION_ENABLE:

												pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIGURE_MISSION_ENABLE;

												printf("NB BDC Configure Mission Command Received \r\n");
												
												
						#ifdef DP_RFSAPR_DRX_DEBUG_MSG
												printf("INFO\t: RFSAPR_CMD_CONFIG_NARROWBAND_BDC Mission command Received\r\n");
						#endif
												/*if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_MISSION_ENABLE))*/
												if(ulRxMsgLength == 0)
												{
													sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
												}
												else	
												{
													
													
														sRetVal = DP_RFSAPR_NB_Mission_Enable(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCMissionEnableConfig);								
												
													
													ulResMessageSize = 0;
														
													
												}							

												break;
												
												case RFSAPR_CMD_CONFIGURE_NBFS_CONFIG:

												pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIGURE_NBFS_CONFIG;

												printf("NBFS Command Received \r\n");
												
												
						#ifdef DP_RFSAPR_DRX_DEBUG_MSG
												printf("INFO\t: RFSAPR_CMD_CONFIG_NARROWBAND_FS command Received\r\n");
						#endif
												/*if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_NBFS))*/
												if(ulRxMsgLength == 0)
												{
													sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
												}
												else	
												{
													
													sRetVal = DP_RFSAPR_NB_SynthesizerFS_config(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdNBDCNBFSConfig);								
							
													ulResMessageSize = 0;
																				
												}							

												break;
												/**************************************************************************/
					
						
					case RFSAPR_CMD_DRX_AURORA_GT_CORE_RESET:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_GT_CORE_RESET;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_GT_CORE_RESET command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_GT_CORE_RESET))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_Reset_GT_Core(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_GT_core_Rst.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_GT_core_Rst.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_GT_core_Rst.m_ucChNo);
							ulResMessageSize = 0;
						}						
						break;		
					case RFSAPR_CMD_DRX_AURORA_FIFO_RESET:
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_FIFO_RESET;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_FIFO_RESET command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_FIFO_RESET))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_FIFO_Reset(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_FIFO_Rst.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_FIFO_Rst.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_FIFO_Rst.m_ucChNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_FIFO_Rst.m_ucTxReset,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_FIFO_Rst.m_ucRxReset);
							ulResMessageSize = 0;
						}						
						break;	
					case RFSAPR_CMD_DRX_AURORA_READ_LINK_STATUS:
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_READ_LINK_STATUS;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_READ_LINK_STATUS command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_READ_LINK_STATUS))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_Read_link_Status(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Link_Sts.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Link_Sts.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Link_Sts.m_ucChNo,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Aurora_Rd_Link_Sts);							
							ulResMessageSize = sizeof(S_RFSAPR_RES_DRX_AURORA_READ_LINK_STATUS);
						}						
						break;			
					case RFSAPR_CMD_DRX_AURORA_READ_ERROR_STATUS:
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_READ_ERROR_STATUS;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_READ_ERROR_STATUS command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_READ_ERROR_STATUS))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_Read_Error_Status(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Error_Sts.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Error_Sts.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Error_Sts.m_ucChNo,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Aurora_Rd_Error_Sts);							
							ulResMessageSize = sizeof(S_RFSAPR_RES_DRX_AURORA_READ_ERROR_STATUS);
						}						
						break;							
					case RFSAPR_CMD_DRX_AURORA_READ_FIFO_COUNT:
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_READ_FIFO_COUNT;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_READ_FIFO_COUNT command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_READ_FIFO_COUNT))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_Read_FIFO_Cnt(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_FIFO_Cnt.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_FIFO_Cnt.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_FIFO_Cnt.m_ucChNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_FIFO_Cnt.m_ucTxRxSel,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Aurora_Rd_FIFO_Cnt);														
							ulResMessageSize = sizeof(S_RFSAPR_RES_DRX_AURORA_READ_FIFO_COUNT);
						}						
						break;		
					case RFSAPR_CMD_DRX_AURORA_READ_RX_FIFO_DATA:
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_READ_RX_FIFO_DATA;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_READ_RX_FIFO_DATA command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_READ_RX_FIFO_DATA))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_Read_Rx_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Rx_FIFO_Data.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Rx_FIFO_Data.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Rx_FIFO_Data.m_ucChNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Rd_Rx_FIFO_Data.m_usRxDataCnt,pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Aurora_Rd_FIFO_Data.m_usRxRdData);
							ulResMessageSize = sizeof(S_RFSAPR_RES_DRX_AURORA_READ_RX_FIFO_DATA);
						}						
						break;	
					case RFSAPR_CMD_DRX_AURORA_WRITE_TX_FIFO_DATA:
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_WRITE_TX_FIFO_DATA;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_WRITE_TX_FIFO_DATA command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_WRITE_TX_FIFO_DATA))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_Write_Tx_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Wr_Tx_FIFO_Data.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Wr_Tx_FIFO_Data.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Wr_Tx_FIFO_Data.m_ucChNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Wr_Tx_FIFO_Data.m_usTxDataCnt,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Wr_Tx_FIFO_Data.m_usTxData);
							ulResMessageSize = 0;
						}						
						break;
					case RFSAPR_CMD_DRX_AURORA_ENABLE_TX_CHANNEL:
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_ENABLE_TX_CHANNEL;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_ENABLE_TX_CHANNEL command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_ENABLE_TX_CHANNEL))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_Enable_Tx(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Enable_Tx_Channel.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Enable_Tx_Channel.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Enable_Tx_Channel.m_ucChNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Enable_Tx_Channel.m_ucTxEnbDis,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_Enable_Tx_Channel.m_ucLoopbackEnbDis);
							ulResMessageSize = 0;
						}						
						break;	
						
					case RFSAPR_CMD_DRX_AURORA_ENBDIS_LOOPBACK:
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DRX_AURORA_ENBDIS_LOOPBACK;
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_DRX_AURORA_ENBDIS_LOOPBACK command Received\r\n");
#endif
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DRX_AURORA_ENBDIS_LOOPBACK))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else	
						{							
							sRetVal = DP_RFSAPR_DRX_AURORA_EnbDis_IntLoopBack(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_EnbDis_Loopback.m_ucBoardSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_EnbDis_Loopback.m_ucBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_EnbDis_Loopback.m_ucChNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Aurora_EnbDis_Loopback.m_ucLoopBackEnbDis);
							ulResMessageSize = 0;
						}						
						break;															
/************************************** DP-VPX-0930 SBC Test Cases*****************************************************/						
					/***SBC Processor Test**/
						
					case RFSAPR_CMD_SBC_PROCESSOR_TEST:
					
						dp_rfsapr_printf("INFO\t: SBC Processor Test Command Received \r\n");
							
						/* Perform Processor Test*/
						sRetVal = DPVPX0930_ATP_Processor_Test();
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: DPVPX0930_ATP_Processor_Test Failed...\r\n");
						}
						
						/* Result is send back to ATE*/
						pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Processor_Test.m_ucResult = sRetVal;
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_PROCESSOR_TEST;
						
						ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_PROCESSOR_TEST);		
													
						break;
							
					/*SBC SDRAM Test*/
						
					case RFSAPR_CMD_SBC_SDRAM_TEST :
						
						dp_rfsapr_printf("INFO\t:SBC SDRAM Test Command Received \r\n");
							
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform SDRAM test only one test at a time (viz. Databus , Address bus and Device test) */
							sRetVal = DPVPX0930_ATP_SDRAM_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_SDRAM_Test);
							dp_rfsapr_printf("INFO\t: Return Value :%d \n",sRetVal);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: SDRAM Test Type :%d Failed \r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_SDRAM_Test.m_ucSDRAMTestType);
							}
							
							/* Result is send back to ATE*/
							pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_SDRAM_Test.m_ucResult = sRetVal;
					
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_SDRAM_TEST;
					
							ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_SDRAM_TEST);
						}
						
						break;
						
					case RFSAPR_CMD_SBC_MRAM_TEST :
						dp_rfsapr_printf("INFO\t:SBC MRAM Test Command Received\r\n");
																			
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
						    sRetVal = DPVPX0930_ATP_MRAM_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_MRAM_Test);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: MRAM Test Type :%d Failed \r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_MRAM_Test.m_ucMRAMTestType);
							}
							
							/* Result is send back to ATE*/
							pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_MRAM_Test.m_ucResult = sRetVal;

							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_MRAM_TEST;

							ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_MRAM_TEST);
						}
												
						break;
						
					case RFSAPR_CMD_SBC_TIMER_TEST :
						
						dp_rfsapr_printf("INFO\t:SBC Timer Test Command Received\r\n");
																									
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
								sRetVal = DPVPX0930_ATP_Timer_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Timer_Test, &pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Timer_Test.m_ulTimerIntCnt);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: Timer :%d Failed \r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Timer_Test.m_ucTimerNumber);
								}
								
								/* Result is send back to ATE*/
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Timer_Test.m_ucResult = sRetVal;
																
								pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_TIMER_TEST;
																
								ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_TIMER_TEST);
							
						}
																		
						break;
						
					case RFSAPR_CMD_SBC_UART_TEST :
														
						dp_rfsapr_printf("INFO\t: SBC UART Test Command Received \r\n");
																													
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \r\n",ulRxMsgLength);
						}
						else
						{
								sRetVal = DPVPX0930_ATP_UART_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_UART_Test);
								
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: UART CH - %d Test Failed\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_UART_Test.m_ucPortNumber);
								}
								
								/* Result is send back to ATE*/
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_UART_Test.m_ucResult = sRetVal;

								pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_UART_TEST;

								ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_UART_TEST);
								
						}
																												
						break;
						
					case RFSAPR_CMD_SBC_COUNTER_TEST :
							
						dp_rfsapr_printf("INFO\t: SBC Counter Test Command Received \r\n");
																																		
						  if(ulRxMsgLength == 0)
						  {
							  dp_rfsapr_printf("ERROR\t: Rx message size :%d \r\n",ulRxMsgLength);
						  }
						  else
						  {
				/*			   sRetVal = DPVPX0930_ATP_Counter_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Counter_Test); */
							   if(sRetVal != DP_RFSAPR_SUCCESS)
							   {
								   dp_rfsapr_printf("ERROR\t: Counter - %d Test Failed\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Counter_Test.m_ucCounterNumber);
							   }
							   /* Result is send back to ATE*/
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Counter_Test.m_ucResult = sRetVal;
																																
								pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_COUNTER_TEST;
																																
								ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_COUNTER_TEST);
						  }
																																	
					    break;
					    
					case RFSAPR_CMD_SBC_USB_TEST :
																								
						dp_rfsapr_printf("INFO\t: SBC USB Test Command Received \r\n");
																																							
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						}
						else
						{
							sRetVal = DPVPX0930_ATP_USB_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_USB_Test);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: USB Test Failed\r\n");
							}
							
							/* Result is send back to ATE*/
							pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_USB_Test.m_ucResult = sRetVal;
																																				
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_USB_TEST;
																																				
							ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_USB_TEST);
								 
						}
						 break;
						 
					case RFSAPR_CMD_SBC_SATA_TEST :
																													
						dp_rfsapr_printf("INFO\t: SBC SATA Test Command Received \r\n");
																																												
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						}
						else
						{
							sRetVal = DPVPX0930_ATP_SATA_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_SATA_Test);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: SATA Test Failed\r\n");
							}
							
							/* Result is send back to ATE*/
							pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_SATA_Test.m_ucResult = sRetVal;
																																									
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_SATA_TEST;
																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_SATA_TEST);							 
						}
						break;
						 
					case RFSAPR_CMD_SBC_SET_RTC :
																																		
						dp_rfsapr_printf("INFO\t:SBC Set RTC Command Received \r\n");
																																																	
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t:Rx message size :%d \n",ulRxMsgLength);
						}
						else
						{
							struct tm setRTCDateTime ;
							memset(&setRTCDateTime, 0, sizeof(struct tm));
							
							setRTCDateTime.tm_year 	= (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_usYear - 1900);
							setRTCDateTime.tm_mon 	= pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucMonth;
							setRTCDateTime.tm_mday 	= pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucDay;
							setRTCDateTime.tm_hour 	= pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucHour;
							setRTCDateTime.tm_min 	= pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucMinutes;
							setRTCDateTime.tm_sec 	= pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucSeconds;
							setRTCDateTime.tm_wday 	= pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucWeekDay;
							
							sRetVal = vxbRtcSet(&setRTCDateTime);
							
							if(sRetVal)
							{
								dp_rfsapr_printf("ERROR\t: RTC Set failed\n");
							}
							else
							{
								dp_rfsapr_printf("INFO\t: RTC Set Date Time : %02d-%02d-%04d %02d:%02d:%02d\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucDay,\
										pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucMonth, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_usYear,\
										pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucHour, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucMinutes,\
										pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Set_RTC.m_ucSeconds);
							}
							
							/* Result is send back to ATE*/
							pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Set_RTC.m_ucResult = sRetVal;
							dp_rfsapr_printf("INFO\t: RTC Set : %d, %d",pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Set_RTC.m_ucResult,sRetVal);
																																														
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_SET_RTC;
																																														
							ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_SET_RTC);
							
						}
						break;
						
					case RFSAPR_CMD_SBC_GET_RTC :
						
						dp_rfsapr_printf("INFO\t:SBC Get RTC Command Received \r\n");
																																																					
						if(ulRxMsgLength != 0)
						{
							dp_rfsapr_printf("ERROR\t:Rx message size :%d \n",ulRxMsgLength);
						}
						else
						{
								struct tm getRTCDateTime ;
								memset(&getRTCDateTime, 0, sizeof(struct tm));
								
								sRetVal = vxbRtcGet(&getRTCDateTime);
																
								if(sRetVal == ERROR)
								{
									dp_rfsapr_printf("ERROR\t: RTC Get failed\n");
								}
								else
								{
										dp_rfsapr_printf("INFO\t: RTC Get Date Time : %02d-%02d-%04d %02d:%02d:%02d\n", getRTCDateTime.tm_mday,\
												getRTCDateTime.tm_mon, getRTCDateTime.tm_year, getRTCDateTime.tm_hour, getRTCDateTime.tm_min, getRTCDateTime.tm_sec);
								}
								
								
								/* Result is send back to ATE*/
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_usYear    = (unsigned short)(getRTCDateTime.tm_year + 1900);
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucMonth   = (unsigned char)getRTCDateTime.tm_mon;
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucDay     = (unsigned char)getRTCDateTime.tm_mday;
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucHour    = (unsigned char)getRTCDateTime.tm_hour;
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucMinutes = (unsigned char)getRTCDateTime.tm_min;
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucSeconds = (unsigned char)getRTCDateTime.tm_sec;
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucWeekDay = (unsigned char)getRTCDateTime.tm_wday;
							
								
								dp_rfsapr_printf("INFO\t: RTC Get Date Time : %02d-%02d-%04d %02d:%02d:%02d\n", pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucDay,\
										pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucMonth, pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_usYear, pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucHour, pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucMinutes, pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Get_RTC.m_ucSeconds);
															
								pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_GET_RTC;
																																															
								ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_GET_RTC);
						}
												
						 break;
						 
					case RFSAPR_CMD_SBC_DMA_TEST :
																																							
						dp_rfsapr_printf("INFO\t: SBC DMA Test Command Received \r\n");
																																																						
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d\r\n",ulRxMsgLength);
						}
						else
						{
							sRetVal = DPVPX0930_ATP_DMA_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_DMA_Test,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_DMA_Test);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: DMA Test Failed\r\n");
							}

							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_DMA_TEST;

							ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_DMA_TEST);
																							 
						}
											
						break;
					
					case RFSAPR_CMD_SBC_EEPROM_TEST :
																																												
						dp_rfsapr_printf("INFO\t: SBC EEPROM Test Command Received \r\n");
																																																											
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						}
						else
						{
								sRetVal = DPVPX0930_ATP_EEPROM_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_EEPROM_Test);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: EEPROM Test Type : %d failed\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_EEPROM_Test.m_ucEEPROMTestType);
								}
										
								/* Result is send back to ATE*/
								pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_EEPROM_Test.m_ucResult = sRetVal;
																																																								
								pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_EEPROM_TEST;
																																																								
								ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_EEPROM_TEST);
						}
																
						break;
						
					case RFSAPR_CMD_SBC_WDT_TEST :
																																																	
						dp_rfsapr_printf("INFO\t: SBC WDT Test Command Received \r\n");
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						}
						else
						{
							sRetVal = DPVPX0930_ATP_WDT_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_WDT_Test,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_WDT_Test.m_ulWDTIntCnt);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: WDT Test Type : %d failed\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_WDT_Test.m_ucWDT_TestType);

							}
							
							/* Result is send back to ATE*/
							pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_WDT_Test.m_ucResult = sRetVal;
																																																													
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_WDT_TEST;
																																																													
							ulResMessageSize = sizeof(S_RFSAPR_RES_SBC_WDT_TEST);
						}
																					
						break;
						
					case RFSAPR_CMD_SBC_ALTIVEC_TEST :
							

						dp_rfsapr_printf("INFO\t: SBC Altivec Test Command Received \r\n");
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						}
						else
						{
							sRetVal = AltivecTest(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Altivec_Test,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_SBC_Altivec_Test);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Altivec Test Type : %d failed\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Altivec_Test.m_u8TestType);
								dp_rfsapr_printf("ERROR\t: Altivec Data Type : %d failed\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_SBC_Altivec_Test.m_u8DataType);

							}
																																																									
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_ALTIVEC_TEST;
																																																													
							ulResMessageSize = sizeof(S_RFSAPR_RES_ALTIVEC_TEST);

						}

																					
						break;
					case RFSAPR_CMD_NB_DMA_CONFIGURE :
							
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_NB_DMA_CONFIGURE Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_NB_DMA_CONFIGURE;
																																																																
						if(ulRxMsgLength != 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							sRetVal = DP_RFSAPR_DPVPX1013_ConfigureDMA();
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: NB DMA Configuration failed\n");
							}																																																									
							ulResMessageSize = 0;

						}
						break;
						
					case RFSAPR_CMD_NB_ACQUIRE_DMA_DATA :
							
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_NB_ACQUIRE_DMA_DATA Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_NB_ACQUIRE_DMA_DATA;
																																																																
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_NB_DRX_DMA_DATA))
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Reset DMA FIFO*/
							sRetVal = DP_RFSAPR_DPVPX1013_DMA_Reset(0x0);
							if (sRetVal != DP_RFSAPR_SUCCESS) 
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DMA_Reset Failure \r\n");
								return sRetVal;
							}
							
							/*Configure DMA*/
							sRetVal = DP_RFSAPR_DPVPX1013_DMA_Configure(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_DMA_DataRead.m_ucBrdNo, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_DMA_DataRead.m_ulDataLimit , g_RFSAPR_Handle.S_1013DMAHandle.ulPhyAddr[0][DP_RFSAPR_DMA_PING]);
							if (sRetVal != DP_RFSAPR_SUCCESS) 
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1013_DMA_Configure Failure \r\n");
								return sRetVal;
							}
							
							/*Start Acquisition*/
							sRetVal = DP_RFSAPR_DPVPX1013_DMA_StartStop_Data_Acquisition(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_DMA_DataRead.m_ucBrdNo, 0x1);
							if (sRetVal != DP_RFSAPR_SUCCESS) 
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1013_DMA_StartStop_Data_Acquisition Failure \r\n");
								return sRetVal;
							}
							
							/*Delay*/
							sysUsDelay(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_DMA_DataRead.m_ulDelay * 1000);
							
							/*Stop Acquisition*/
							sRetVal = DP_RFSAPR_DPVPX1013_DMA_StartStop_Data_Acquisition(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_DMA_DataRead.m_ucBrdNo, 0x0);
							if (sRetVal != DP_RFSAPR_SUCCESS) 
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DPVPX1013_DMA_StartStop_Data_Acquisition Failure \r\n");
								return sRetVal;
							}
										
							/*Read DMA Data*/
							
							sRetVal = DP_RFSAPR_DPVPX1013_ReadDMAData(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_NB_DRx_DMA_Data);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: NB DMA Read Data failed\n");
							}	
							
							ulResMessageSize = sizeof(S_RFSAPR_RES_NB_DRX_DMA_DATA);

						}
						break;
					case RFSAPR_CMD_WB_ACQUIRE_DMA_DATA :
							
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WB_ACQUIRE_DMA_DATA Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WB_ACQUIRE_DMA_DATA;
																																																																
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_WB_DRX_DMA_DATA))
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Reset DMA FIFO*/
							sRetVal = DP_RFSAPR_DMA_Reset(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ucBrdNo);
							if (sRetVal != DP_RFSAPR_SUCCESS) 
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DMA_Reset Failure \r\n");
								return sRetVal;
							}
							
							/*Configure DMA*/
							sRetVal = DP_RFSAPR_DMA_Configure(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ucBrdNo, pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ucCntrADCDatasel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ulDataLimit,g_RFSAPR_Handle.S_DMAHandle.ulPhyAddr[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ucBrdNo][DP_RFSAPR_DMA_PING]);
							if (sRetVal != DP_RFSAPR_SUCCESS) 
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DMA_Configure Failure \r\n");
								return sRetVal;
							}
							
							/*Start Acquisition*/
							sRetVal = DP_RFSAPR_DMA_StartStop_Data_Acquisition(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ucBrdNo, 0x1);
							if (sRetVal != DP_RFSAPR_SUCCESS) 
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DMA_StartStop_Data_Acquisition Failure \r\n");
								return sRetVal;
							}
							
							/*Delay*/
							sysUsDelay(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ulDelay * 1000);
							
							/*Stop Acquisition*/
							sRetVal = DP_RFSAPR_DMA_StartStop_Data_Acquisition(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ucBrdNo, 0x0);
							if (sRetVal != DP_RFSAPR_SUCCESS) 
							{
								dp_rfsapr_printf("ERROR\t: DP_RFSAPR_DMA_StartStop_Data_Acquisition Failure \r\n");
								return sRetVal;
							}
										
							/*Read DMA Data*/
							
							sRetVal = DP_RFSAPR_ReadDMAData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_WB_DMA_DataRead.m_ucBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_WB_DRx_DMA_Data);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: WB DMA Read Data failed\n");
							}	
							
							ulResMessageSize = sizeof(S_RFSAPR_RES_NB_DRX_DMA_DATA);

						}
						break;						
					case RFSAPR_CMD_UART_PORT_CONFIGURE :
																
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_UART_PORT_CONFIGURE Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_UART_PORT_CONFIGURE;
																																																															
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Configure the UART*/
							sRetVal = DP_RFSAPR_UART_Configuration(pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_cmdUARTPortConfigure);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART COnfiguration failed\n");
							}																																																									
							ulResMessageSize = 0;

						}
						break;
					case RFSAPR_CMD_UART_WRITE_TX_DATA :
																					
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_UART_WRITE_TX_DATA Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_UART_WRITE_TX_DATA;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Write the Data in UART Tx*/
							sRetVal = DP_RFSAPR_UART_Write_TxData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_cmdUARTWriteTxData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART Tx Data Write failed\n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;
						
					case RFSAPR_CMD_UART_READ_RX_DATA :
																										
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_UART_READ_RX_DATA Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_UART_READ_RX_DATA;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Write the Data in UART Tx*/
							sRetVal = DP_RFSAPR_UART_Read_RxData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_cmdUARTReadRxData, &pSResponseToRFSAPR->UResRFSAPRMessage.S_ResUARTReadRxData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART Rx Data Read failed\n");
							}																																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_UART_READ_RX_DATA);
	
						}
						break;
					case RFSAPR_CMD_CLOSE_UART :
																															
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_CLOSE_UART Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CLOSE_UART;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Write the Data in UART Tx*/
							sRetVal = DP_RFSAPR_Close_UART_Port(pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_cmdUARTClosePort);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART COM Port Closed failed\n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;

					case RFSAPR_CMD_FEARS422_WRITE_DATA:
																																				
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_FEARS422_WRITE_DATA Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_FEARS422_WRITE_DATA;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Write the Data in DP-XMC-5028 FEA RS422 Tx*/
							sRetVal = DP_XMC_5028_FEA_Write_TxData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdFEARS422WriteData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to write data into DP_XMC_5028_FEA \n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;
					case RFSAPR_CMD_READ_WBDRX_FEARS422:
																																									
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_WBDRX_FEARS422 Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_WBDRX_FEARS422;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read data from WB DRx module*/
							sRetVal = DP_RFSAPR_DPVPX1502_Read_FEARS422_InputSts(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdFEARS422ReadAllDRx.m_u8DRxBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadDiifBlankgIOSts);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to read Blanking IO status from WB DRx Module \n");
							}																																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_DIFF_BLANKING_IO_DATA);
	
						}
						break;
					case RFSAPR_CMD_READ_NBDRX_FEARS422:
																																														
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_NBDRX_FEARS422 Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NBDRX_FEARS422;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read data from NB DRx module*/
							sRetVal = DP_RFSAPR_DPVPX1013_Read_FEARS422_InputSts(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdFEARS422ReadAllDRx.m_u8DRxBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadDiifBlankgIOSts);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to read Blanking IO status from NB DRx Module \n");
							}																																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_DIFF_BLANKING_IO_DATA);
	
						}
						break;
						
					case RFSAPR_CMD_WRITE_AUTS_VIDEO_TTL:
																																														
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WRITE_AUTS_VIDEO_TTL Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_AUTS_VIDEO_TTL;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read Data from All DRx Modules*/
							sRetVal = DP_RFSAPR_DPVPX1502_Write_AUTS_Video_TTL_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWtiteAUTSVideoTTLData.m_u8DRxBrdNo, pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWtiteAUTSVideoTTLData.m_u32PulseONCount, pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWtiteAUTSVideoTTLData.m_u32PulseOFFCount);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to write AUTS Video TTL Data into WB DRx1 Module\n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;
						
					case RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_BLANKING:
																																																			
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_BLANKING Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_NBDRX_LOOK_THRU_BLANKING;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Write Data in NB DRx Module*/
							sRetVal = DP_RFSAPR_DPVPX1013_Write_Look_Thrgh_Blanking_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWriteNBDRxLookThruBlanking.m_u8NBDRx, pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWriteNBDRxLookThruBlanking.m_u8WriteData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to Look throgh blanking write Data into NB DRx Module\n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;
						
					case RFSAPR_CMD_READ_WBDRX_LOOK_THRU_BLANKING:
																																																								
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_WBDRX_LOOK_THRU_BLANKING Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_WBDRX_LOOK_THRU_BLANKING;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read look through blanking data from WB DRx Modules*/
							sRetVal = DP_RFSAPR_DPVPX1502_Read_Look_Thrgh_Blanking_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadAllDRxLookThruBlanking.m_u8DRxBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadBlankgPulseOnOffcnt);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to read Look through blanking data from WB DRx modules\n");
							}																																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_BLANKING_PULSE_ON_OFF_CNT);
	
						}
						break;
					case RFSAPR_CMD_READ_NBDRX_LOOK_THRU_BLANKING:
																																																													
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_NBDRX_LOOK_THRU_BLANKING Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NBDRX_LOOK_THRU_BLANKING;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read look through blanking data from NB DRx Module*/
							sRetVal = DP_RFSAPR_DPVPX1013_Read_Look_Thrgh_Blanking_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadAllDRxLookThruBlanking.m_u8DRxBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadBlankgPulseOnOffcnt);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to read Look through blanking data from NB DRx module\n");
							}																																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_BLANKING_PULSE_ON_OFF_CNT);
	
						}
						break;
						
					case RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_RADAR:
																																																								
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WRITE_NBDRX_LOOK_THRU_RADAR Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_NBDRX_LOOK_THRU_RADAR;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Write look through radar data in NB DRx Module*/
							sRetVal = DP_RFSAPR_DPVPX1013_Write_Look_Thrgh_Radar_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWriteNBDRxLookThruRadar.m_u8NBDRx, pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWriteNBDRxLookThruRadar.m_u8WriteData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to Look through radar write Data into NB DRx Module\n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;
						
					case RFSAPR_CMD_READ_WBDRX_LOOK_THRU_RADAR:
																																																								
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_WBDRX_LOOK_THRU_RADAR Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_WBDRX_LOOK_THRU_RADAR;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read look through radar data from WB DRx Module*/
							sRetVal = DP_RFSAPR_DPVPX1502_Read_Look_Thrgh_Radar_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadAllDRxLookThruRadar.m_u8DRxBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadAllDRxLookThruRadar.m_u8ReadlookThrRadarSts);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to read Look throgh radar Data from WB DRx Modules\n");
							}																																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_ALLDRX_LOOK_THRU_RADAR);
	
						}
						break;
						
					case RFSAPR_CMD_READ_NBDRX_LOOK_THRU_RADAR:
																																																													
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_WBDRX_LOOK_THRU_RADAR Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NBDRX_LOOK_THRU_RADAR;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read look through radar data from NB DRx Module*/
							sRetVal = DP_RFSAPR_DPVPX1013_Read_Look_Thrgh_Radar_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadAllDRxLookThruRadar.m_u8DRxBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadAllDRxLookThruRadar.m_u8ReadlookThrRadarSts);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to read Look throgh radar Data from NB DRx Module\n");
							}																																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_ALLDRX_LOOK_THRU_RADAR);
	
						}
						break;
					case RFSAPR_CMD_READ_WBDRX_BLANKING_SIGNAL_PULSE:
																																																													
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_WBDRX_BLANKING_SIGNAL_PULSE Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_WBDRX_BLANKING_SIGNAL_PULSE;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read Pulse ON and OFF time from All DRx Modules*/
							sRetVal = DP_RFSAPR_DPVPX1502_Read_Blanking_Signal(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadBlankingPulse.m_u8DRxBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadBlankingPulse.m_u8ChSel,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadBlankingPulse.m_fDRxReadPulsWidth, &pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadBlankingPulse.m_fDRxReadPulsePeriod);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to read blanking signal pulse width and pulse period from WB DRx Modules\n");
							}																																																									
							ulResMessageSize = sizeof(S_RFSAPR_RES_READ_BLANKING_SIGNAL_PULSE);
	
						}
						break;
						
					case RFSAPR_CMD_READ_NBDRX_BLANKING_SIGNAL_PULSE:
																																																																		
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_NBDRX_BLANKING_SIGNAL_PULSE Command Received \r\n");
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NBDRX_BLANKING_SIGNAL_PULSE;
																																																															
					if(ulRxMsgLength == 0)
					{
						dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*Read Pulse width and pulse period from NB DRx Module*/
						sRetVal = DP_RFSAPR_DPVPX1013_Read_Blanking_Signal(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadBlankingPulse.m_u8DRxBoardNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadBlankingPulse.m_u8ChSel,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadBlankingPulse.m_fDRxReadPulsWidth, &pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadBlankingPulse.m_fDRxReadPulsePeriod);
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Unable to read blanking signal pulse width and pulse period from NB DRx Module\n");
						}																																																									
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_BLANKING_SIGNAL_PULSE);

					}
					break;
					
					case RFSAPR_CMD_CONFIG_DRX_SINENDED_DEBOUNCETIME:
																																																													
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_CONFIG_DRX_SINENDED_DEBOUNCETIME Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIG_DRX_SINENDED_DEBOUNCETIME;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Read Pulse ON and OFF time from All DRx Modules*/
							sRetVal = DP_RFSAPR_Config_DRxDebounceTime(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdConfigDRxDebounceTime.m_ucWBNBSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdConfigDRxDebounceTime.m_ucDRXNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdConfigDRxDebounceTime.m_usDebounceTime);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Unable to configure deboune time in DRx Modules for read pulse parameters\n");
							}																																																									
							ulResMessageSize = 0;
					
						}
						break;
						
					case RFSAPR_CMD_WRITE_OPEN_GND_SIGNAL_STATUS:
																																																																		
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_WRITE_OPEN_GND_SIGNAL_STATUS Command Received \r\n");
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_OPEN_GND_SIGNAL_STATUS;
																																																															
					if(ulRxMsgLength == 0)
					{
						dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*Write ServOP data in WB DRX1*/
						sRetVal = DP_RFSAPR_DPVPX1502_Write_Open_Gnd_Signal_Sts(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWriteOpenGndSignalSts.m_u8DRxBrdNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWriteOpenGndSignalSts.m_u8SerOutDRxData);
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Unable to write serv out data in WB DRx module\n");
						}
						
						ulResMessageSize = 0;
					}
					break;
					
					case RFSAPR_CMD_READ_WBDRX_OPEN_GND_SIGNAL_STATUS:
																																																																							
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_WBDRX_OPEN_GND_SIGNAL_STATUS Command Received \r\n");
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_WBDRX_OPEN_GND_SIGNAL_STATUS;
																																																															
					if(ulRxMsgLength == 0)
					{
						dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*Read System Recovry status for WB DRx Modules*/
						sRetVal = DP_RFSAPR_DPVPX1502_Read_Open_Gnd_Signal_Sts(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadOpenGndSignalSts.m_u8DRxBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadOpenGndSignalSts.m_u8ReadOpnGndSts);
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Unable to read system recovery status from WB DRx module - %d\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadOpenGndSignalSts.m_u8DRxBrdNo);
						}
						
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_DRX_QT_TEST);
					}
					break;
					
					case RFSAPR_CMD_READ_NBDRX_OPEN_GND_SIGNAL_STATUS:
																																																																												
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_NBDRX_OPEN_GND_SIGNAL_STATUS Command Received \r\n");
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NBDRX_OPEN_GND_SIGNAL_STATUS;
																																																															
					if(ulRxMsgLength == 0)
					{
						dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*Read Open Gnd Signal Status from NB DRx Module*/
						sRetVal = DP_RFSAPR_DPVPX1013_Read_Open_Gnd_Signal_Sts(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadOpenGndSignalSts.m_u8DRxBrdNo,&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadOpenGndSignalSts.m_u8ReadOpnGndSts);
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Unable to read system recovery status from NB DRx module - %d\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdReadOpenGndSignalSts.m_u8DRxBrdNo);
						}
						
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_DRX_QT_TEST);
					}
					break;
					case RFSAPR_CMD_READ_IOEXPANDER_OPEN_GND_SIGNAL_STATUS:
																																																																																	
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_IOEXPANDER_OPEN_GND_SIGNAL_STATUS Command Received \r\n");
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_IOEXPANDER_OPEN_GND_SIGNAL_STATUS;
																																																															
					if(ulRxMsgLength != 0)
					{
						dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*Read Open Gnd Signal Status from IO Expander */
						sRetVal = DP_RFSAPR_IOEXPANDER_Read_Open_Gnd_SignalSts(&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadIOExpOpenGndSignalSts);
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Unable to read data from XMC IO Expander\n");
						}
						
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_IOEXP_QT_TEST);
					}
					break;
					case RFSAPR_CMD_READ_XMC_OPEN_GND_SIGNAL_STATUS:
																																																																																						
					dp_rfsapr_printf("INFO\t: RFSAPR_CMD_READ_XMC_OPEN_GND_SIGNAL_STATUS Command Received \r\n");
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_XMC_OPEN_GND_SIGNAL_STATUS;
																																																															
					if(ulRxMsgLength != 0)
					{
						dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*Read System Recovery Status from XMC Module*/
						sRetVal = DP_RFSAPR_DPXMC5028_Read_Open_Gnd_SignalSts(&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResReadXMCOpenGndSignalSts);
						if(sRetVal != DP_RFSAPR_SUCCESS)
						{
							dp_rfsapr_printf("ERROR\t: Unable to read System Recovery Status from XMC module\n");
						}
						
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_OPEN_GND_CONTROLS_XMC_QT_TEST);
					}
					break;
					
					case RFSAPR_CMD_SBC_UART_PORT_CONFIGURE :
																
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_UART_PORT_CONFIGURE Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_UART_PORT_CONFIGURE;
																																																															
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Configure the UART*/
							sRetVal = DPVPX0930_UART_Configuration(pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_cmdUARTPortConfigure);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART COnfiguration failed\n");
							}																																																									
							ulResMessageSize = 0;

						}
						break;
					case RFSAPR_CMD_SBC_UART_WRITE_TX_DATA :
																					
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_UART_WRITE_TX_DATA Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_UART_WRITE_TX_DATA;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Write the Data in UART Tx*/
							sRetVal = DPVPX0930_UART_Write_TxData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_cmdUARTWriteTxData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART Tx Data Write failed\n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;

					case RFSAPR_CMD_SBC_CLOSE_UART :
																															
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_CLOSE_UART Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SBC_CLOSE_UART;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Close SBC UART Port*/
							sRetVal = DPVPX0930_Close_UART_Port(pSCommandFromRFSAPR->UCmdRFSAPRMessage.s_cmdUARTClosePort);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: UART COM Port Closed failed\n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;
						
						case RFSAPR_CMD_SET_FEA_CTRL_OP :
																															
						dp_rfsapr_printf("INFO\t: RFSAPR_CMD_SET_FEA_CTRL_OP Command Received \r\n");
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SET_FEA_CTRL_OP;
																																																																
						if(ulRxMsgLength == 0)
						{
							dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/*Close SBC UART Port*/
							sRetVal = SetFEACtrlOutput(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_cmdSetFEACtrlOutput.m_u16FEACtrlData);
							if(sRetVal != DP_RFSAPR_SUCCESS)
							{
								dp_rfsapr_printf("ERROR\t: Set FEA Control output failed\n");
							}																																																									
							ulResMessageSize = 0;
	
						}
						break;
						
						case RFSAPR_CMD_SYSTEM_FAN_ON_OFF_CTRL :
																																
							dp_rfsapr_printf("INFO\t: RFSAPR_CMD_SYSTEM_FAN_ON_OFF_CTRL Command Received \r\n");
							
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SYSTEM_FAN_ON_OFF_CTRL;
																																																																	
							if(ulRxMsgLength == 0)
							{
								dp_rfsapr_printf("ERROR\t: Rx message size :%d \n",ulRxMsgLength);
								sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
							}
							else
							{
								/*Control FAN On/Off */
								sRetVal = DP_RFSAPR_FANONOFFControl(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdFANOnOffControl.m_ucFanSel,pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdFANOnOffControl.m_ucFanOnOffsel);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: Control FAN On/Off failed\n");
								}																																																									
								ulResMessageSize = 0;
		
							}
							break;						
						case RFSAPR_CMD_UPDATE_LOG_ADC_CALIB:

							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_UPDATE_LOG_ADC_CALIB;
							
		#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: Log ADC Calibration data update command received : %d\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdHWlogAdcTempData.m_ucAntNum);
		#endif
							/*if(ulRxMsgLength != sizeof(S_RFSAPR_LOG_ADC_MEASD_TABLE))*/
							if(ulRxMsgLength != sizeof(S_RFSAPR_HW_LOG_TEMP_CALIB_DATA))							
							{
								dp_rfsapr_printf("ERROR\t: ulRxMsgLength\r\n");
								sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
							}
							else
							{
								
								dp_rfsapr_printf("INFO\t: Log ADC Channel : %d\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdHWlogAdcTempData.m_ucAntNum);

								/*Store to bin file */
								/*sRetVal = DP_RFSAPR_AccessBinFile((DP_RFSAPR_ANT1_LOG_ADC_CALIB_DATA_BIN_FILE + pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdUpdateLOGADCCalib.m_ucAntNum), DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdUpdateLOGADCCalib, (unsigned long)sizeof(S_RFSAPR_LOG_ADC_MEASD_TABLE));*/								
								sRetVal = DP_RFSAPR_AccessBinFile((DP_ANT1_WB_HW_LOGADC_TEMP_CALIB_CONST_BIN_FILE + pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdHWlogAdcTempData.m_ucAntNum), DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdHWlogAdcTempData, (unsigned long)sizeof(S_RFSAPR_HW_LOG_TEMP_CALIB_DATA));		
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: Log ADC Calibration constants failed stored in file\r\n");
								}
								else
								{
									dp_rfsapr_printf("ERROR\t: Log ADC Calibration constants successfully stored in file\r\n");
								}

								/* update the global structure for the same */
								/*memcpy(&g_RFSAPR_Handle.m_SLogADCCalibConsts[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdUpdateLOGADCCalib.m_ucAntNum],&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdUpdateLOGADCCalib,sizeof(S_RFSAPR_LOG_ADC_MEASD_TABLE));*/
								memcpy(&g_RFSAPR_Handle.m_sHWlogADCCalData[pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdHWlogAdcTempData.m_ucAntNum],&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdHWlogAdcTempData,sizeof(S_RFSAPR_HW_LOG_TEMP_CALIB_DATA));
								
							}
							break;		
						case RFSAPR_CMD_READ_LOG_ADC_CALIB:

							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_LOG_ADC_CALIB;

	#ifdef DP_RFSAPR_DRX_DEBUG_MSG
								dp_rfsapr_printf("INFO\t: Log ADC Calibration data read command received\r\n");
	#endif							
							if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_READ_LOG_ADC_CALIB_DATA))
							/*if(ulRxMsgLength != sizeof(S_RFSAPR_HW_LOG_TEMP_CALIB_DATA))*/							
							{
								sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
							}
							else
							{
								
								dp_rfsapr_printf("INFO\t: Log ADC Channel : %d\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadLogADCCalibData.m_ucAntNum);
								
								/*memset(&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResReadLOGADCCalibData,0, sizeof(S_RFSAPR_LOG_ADC_MEASD_TABLE));*/
								memset(&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResHWlogAdcTempData,0, sizeof(S_RFSAPR_HW_LOG_TEMP_CALIB_DATA));
								
								/*sRetVal = DP_RFSAPR_AccessBinFile((DP_RFSAPR_ANT1_LOG_ADC_CALIB_DATA_BIN_FILE + pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadLogADCCalibData.m_ucAntNum), DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResReadLOGADCCalibData, (unsigned long)sizeof(S_RFSAPR_LOG_ADC_MEASD_TABLE));*/
								sRetVal = DP_RFSAPR_AccessBinFile((DP_ANT1_WB_HW_LOGADC_TEMP_CALIB_CONST_BIN_FILE + pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ReadLogADCCalibData.m_ucAntNum), DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResHWlogAdcTempData, (unsigned long)sizeof(S_RFSAPR_HW_LOG_TEMP_CALIB_DATA));								
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: Log ADC Calibration constants data read from file failed\r\n");
								}
								else
								{
									dp_rfsapr_printf("SUCCESS\t: Log ADC Calibration constants data Successfully read from file\r\n");
								}
															
								/*ulResMessageSize = sizeof(S_RFSAPR_LOG_ADC_MEASD_TABLE);*/
								ulResMessageSize = sizeof(S_RFSAPR_HW_LOG_TEMP_CALIB_DATA);								
							}

							break;
						case RFSAPR_CMD_DIFF_BLANK_READ:

						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DIFF_BLANK_READ;
						
#ifdef DP_RFSAPR_DRX_DEBUG_MSG
							dp_rfsapr_printf("INFO\t: DRx Differential Blanking read command received\r\n");
#endif
						
						if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_DIFF_BLANK_READ))
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{

							if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdDiffBlankRead.m_u8WBNBSel == 1)
							{
								sRetVal = DP_RFSAPR_DPVPX1502_ReadDiffBlankSts(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdDiffBlankRead.m_u8DRXNo, &pSResponseToRFSAPR->UResRFSAPRMessage.S_ResDiffBlankRead.m_u16ReadIPData);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: Unable to read differential blanking signal status from WB DRx - %d\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdDiffBlankRead.m_u8DRXNo);
								}
								else
								{
									dp_rfsapr_printf("SUCCESS\t: read differential blanking signal status from WB DRx - %d\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdDiffBlankRead.m_u8DRXNo);
								}
							}
							else
							{
								sRetVal = DP_RFSAPR_DPVPX1013_ReadDiffBlankSts(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdDiffBlankRead.m_u8DRXNo,&pSResponseToRFSAPR->UResRFSAPRMessage.S_ResDiffBlankRead.m_u16ReadIPData);
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: Unable to read differential blanking signal status from NB DRx - %d\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdDiffBlankRead.m_u8DRXNo);
								}
								else
								{
									dp_rfsapr_printf("SUCCESS\t: read differential blanking signal status from NB DRx - %d\r\n",pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_CmdDiffBlankRead.m_u8DRXNo);
								}
							}

							ulResMessageSize = sizeof(S_RFSAPR_RES_DIFF_BLANK_READ);
						}

						break;

						case RFSAPR_CMD_STORE_NBBDC_CALIB_FILE:						

							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_STORE_NBBDC_CALIB_FILE;

							if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA))
							{	
								sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
							}
							else
							{
								dp_rfsapr_printf("INFO\t: Narrow band calib data store command received \r\n");

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
								dp_rfsapr_printf("INFO\t: Antenna Number \t: %d\r\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdNBCalibData.m_ucAntennaNo + 1));
#endif

								if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdNBCalibData.m_ucAntennaNo == 0)
								{
									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT1_NBBDC_CALIB_DATA_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdNBCalibData, (unsigned long)sizeof(S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA));
								}
								else
								{
									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT2_NBBDC_CALIB_DATA_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdNBCalibData, (unsigned long)sizeof(S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA));
								}									
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: NB BDC Calibratiin Configuration data store to file\r\n");
								}
								else
								{
									dp_rfsapr_printf("SUCCESS\t: NB BDC Calibratiin Configuration data store to file\r\n");
								}
							}

							break;			
							
						case RFSAPR_CMD_READ_NBBDC_CALIB_FILE:						

							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_NBBDC_CALIB_FILE;

							if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_NARROW_BAND_CALIB_DATA_READ))
							{	
								sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
							}
							else
							{
								dp_rfsapr_printf("INFO\t: Narrow band calib data read from file command received \r\n");

#ifdef DP_RFSAPR_DRX_DEBUG_MSG
								dp_rfsapr_printf("INFO\t: Antenna Number \t: %d\r\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdNBReadCalibData.m_ucAntennaNo + 1));
#endif

								if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdNBReadCalibData.m_ucAntennaNo == 0)
								{
									
									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT1_NBBDC_CALIB_DATA_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResNBCalibData, (unsigned long)sizeof(S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA));									
								}
								else
								{
									sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT2_NBBDC_CALIB_DATA_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResNBCalibData, (unsigned long)sizeof(S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA));
								}		
								
								if(sRetVal != DP_RFSAPR_SUCCESS)
								{
									dp_rfsapr_printf("ERROR\t: NB BDC Calibratiin Configuration data read from file\r\n");
								}
								else
								{
									dp_rfsapr_printf("SUCCESS\t: NB BDC Calibratiin Configuration data read from file\r\n");
								}
								
								ulResMessageSize = sizeof(S_RFSAPR_CMD_RES_NARROW_BAND_CALIB_DATA);
							}

							break;		
							
						case RFSAPR_CMD_UPDATE_WBRF_TEMP_CALIB:						

								pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_UPDATE_WBRF_TEMP_CALIB;

								if(ulRxMsgLength != sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA))
								{	
									sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
								}
								else
								{
									dp_rfsapr_printf("INFO\t: WB RF HW Temperature calibration store command received \r\n");

	#ifdef DP_RFSAPR_DRX_DEBUG_MSG
									dp_rfsapr_printf("INFO\t: m_ucTempSel : %d ,Antenna Number \t: %d\r\n", pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWBRFTempCalibData.m_ucTempSel,(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWBRFTempCalibData.m_ucAntNum + 1));																
									
	#endif															

									if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWBRFTempCalibData.m_ucAntNum == 0)
									{
										sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT1_WBRF_HW_TEMP_CALIB_DATA_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWBRFTempCalibData, (unsigned long)sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA));
									}
									else
									{
										sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT2_WBRF_HW_TEMP_CALIB_DATA_BIN_FILE, DP_RFSAPR_SAVE_DATA_TO_FILE, (void *)&pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWBRFTempCalibData, (unsigned long)sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA));
									}									
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: WB RF HW Temperature calibration data store to file\r\n");
									}
									else
									{
										dp_rfsapr_printf("SUCCESS\t: WB RF HW Temperature calibration data store to file\r\n");
									}

									/*Apply in DRX*/
									sRetVal = DP_RFSAPR_All_RF_Attenuation_Control(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWBRFTempCalibData.m_sRFSAPRcalibdata[pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmdWBRFTempCalibData.m_ucTempSel]);
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: DP_RFSAPR_All_RF_Attenuation_Control \r\n");
									}									

								}

								break;			
								
							case RFSAPR_CMD_READ_WBRF_TEMP_CALIB:						

								pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_WBRF_TEMP_CALIB;

								if(ulRxMsgLength != sizeof(S_RFSAPR_CMD_READ_WBRF_TEMP_CALIB_DATA))
								{	
									sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
								}
								else
								{
									dp_rfsapr_printf("INFO\t: WB RF HW Temperature calibration data read from file command received \r\n");

	#ifdef DP_RFSAPR_DRX_DEBUG_MSG
									dp_rfsapr_printf("INFO\t: Antenna Number \t: %d\r\n", (pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_Cmd_ReadWBRFCalib.m_ucAntNum + 1));
	#endif

									if(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_S_Cmd_ReadWBRFCalib.m_ucAntNum == 0)
									{
										
										sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT1_WBRF_HW_TEMP_CALIB_DATA_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResWBRFReadCalibData, (unsigned long)sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA));									
									}
									else
									{
										sRetVal = DP_RFSAPR_AccessBinFile(DP_RFSAPR_ANT2_WBRF_HW_TEMP_CALIB_DATA_BIN_FILE, DP_RFSAPR_LOAD_DATA_FROM_FILE, (void *)&pSResponseToRFSAPR->UResRFSAPRMessage.m_SResWBRFReadCalibData, (unsigned long)sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA));
									}		
									
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: WB RF HW Temperature calibration data read from file\r\n");
									}
									else
									{
										dp_rfsapr_printf("SUCCESS\t: WB RF HW Temperature calibration data read from file\r\n");
									}
									
									ulResMessageSize = sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA);
								}

								break;	
								
							case RFSAPR_CMD_READ_BLANKING_STS:						

								pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_BLANKING_STS;

								if(ulRxMsgLength == 0)
								{	
									sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
								}
								else
								{
									
									dp_rfsapr_printf("INFO\t: Read Blanking status \r\n");
									
									sRetVal = DP_RFSAPR_DPVPX1013_DRx_ReadLong(0,DPVPX0930_RFSAPR_PCI_BAR,DP_RFSAPR_DPVPX1013_LOOKTHRU_RD_JAMPOD_RADAR_REG, &pSResponseToRFSAPR->UResRFSAPRMessage.m_SResBlankingSts.m_NB1RDSts);																										
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: Unable to read differential blanking signal status from NB DRx \r\n");
									}
											
									sRetVal = DP_RFSAPR_DRx_ReadLong(0, DPVPX0930_RFSAPR_PCI_BAR,0xB038, &pSResponseToRFSAPR->UResRFSAPRMessage.m_SResBlankingSts.m_WB1RDSts);																										
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: Unable to read differential blanking signal status from WB DRx 1\r\n");
									}
									
									sRetVal = DP_RFSAPR_DRx_ReadLong(1, DPVPX0930_RFSAPR_PCI_BAR,0xB038, &pSResponseToRFSAPR->UResRFSAPRMessage.m_SResBlankingSts.m_WB2RDSts);																										
									if(sRetVal != DP_RFSAPR_SUCCESS)
									{
										dp_rfsapr_printf("ERROR\t: Unable to read differential blanking signal status from WB DRx 2\r\n");
									}									
									
									ulResMessageSize = sizeof(S_RFSAPR_RES_READ_BLANKING_STS);
								}

								break;								
						
						default:
					
					   /*
						*	------------ ERROR ------------					
						*	Z   GroupId - Invalid Unit ID
						*	-------------------------------
						*/
						dp_rfsapr_printf("ERROR\t: INVAILD UNIT ID\r\n");
						sRetVal = DP_RFSAPR_INVALID_UNIT_ID;
					
						break;
				}
				break;	
			}
		case RFSAPR_COMM_CMD_GROUP1:/*TODO ,  Added for Arinc429 communication */
			{
				switch(usCmdUnitID)
				{
					case RFSAPR_CMD_ARINC_429_TX_CONFIGURE:
#if ARINC_DEBUG_PRINTS		
						printf("Received RFSAPR_CMD_ARINC_429_TX_CONFIGURE command \n ");
#endif 	
					
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= ARINC429_Tx_Configure(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ARINCTxConfig);	
							ulResMessageSize = 0;
				
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_ARINC_429_TX_CONFIGURE;			

						break;
					case RFSAPR_CMD_ARINC_429_RX_CONFIGURE:

#if ARINC_DEBUG_PRINTS		
						printf("Received RFSAPR_CMD_ARINC_429_RX_CONFIGURE command \n ");
#endif 	
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= ARINC429_Rx_Configure(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ARINCRxConfig );
							ulResMessageSize = 0;				
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_ARINC_429_RX_CONFIGURE;				

						break;
					case RFSAPR_CMD_START_ARINC_RX_AND_RECEIVE:
						
#if ARINC_DEBUG_PRINTS		
						printf("Received RFSAPR_CMD_START_ARINC_RX_AND_RECEIVE command \n ");
#endif 	

						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= ARINC429_StrtRxChnl_Rxdata(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ARINCRxdata);
							ulResMessageSize = 0;	
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_START_ARINC_RX_AND_RECEIVE;

						break;	
					case RFSAPR_CMD_TRANSMIT_ARINC429_DATA:
						
#if ARINC_DEBUG_PRINTS		
						printf("Received RFSAPR_CMD_TRANSMIT_ARINC429_DATA command \n ");
#endif 	
											
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= ARINC429_TxARINCWords(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ARINCTxData);
							ulResMessageSize = 0;	
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_TRANSMIT_ARINC429_DATA;			

						break;
					case RFSAPR_CMD_ARINC_429_LABEL_CONFIGURE:
						
#if ARINC_DEBUG_PRINTS		
					printf("Received RFSAPR_CMD_ARINC_429_LABEL_CONFIGURE command \n ");
#endif 						
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= ARINC429_Label_Configure(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ARINClabelconfig);
							ulResMessageSize = 0;	
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_ARINC_429_LABEL_CONFIGURE;			

						break;
					case RFSAPR_CMD_SEND_ARINC429_DATA_TO_HOST:
						
#if ARINC_DEBUG_PRINTS		
					printf("Received RFSAPR_CMD_SEND_ARINC429_DATA_TO_HOST command \n ");
#endif 												
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= ARINC429_SendDatatoHost(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ARINCHostData);
							if(sRetVal)
							{						
								ulResMessageSize = 0;	
							}
							else
							{
								ulResMessageSize = sizeof(S_RFSAPR_RES_ARINC429_RX_DATA_TO_HOST);
							}					
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_SEND_ARINC429_DATA_TO_HOST;

						break;
					case RFSAPR_CMD_ARINC_RCV_AND_SEND_DATA_FROM_HOST:
					
#if ARINC_DEBUG_PRINTS		
					printf("Received RFSAPR_CMD_ARINC_RCV_AND_SEND_DATA_FROM_HOST command \n ");
#endif 												
						
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= ARINC429_TxDataFromHost(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_ARINCDataFrmHost);
							ulResMessageSize = 0;	
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_ARINC_RCV_AND_SEND_DATA_FROM_HOST;			

						break;
					case RFSAPR_CMD_ARINC_PERFORM_SELFTEST:
					
						if(ulRxMsgLength != 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= DPXMC5028_Perform_ARINC429_Selftest(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_1553B_ArincTest);
							if(sRetVal)
							{
								ulResMessageSize = 0;			
							}
							else	
							{
								ulResMessageSize = sizeof(S_RFSAPR_1553B_ARINC_SELFTEST);
							}	
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_ARINC_PERFORM_SELFTEST;			

						break;
					case RFSAPR_CMD_ARINC_PERFORM_QT_TEST:
					
						if(ulRxMsgLength != 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= DP_RFSAPR_QT_ARINC429_Test(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_ARINCQTResult.m_ucArincQTRes);
							if(sRetVal)
							{
								ulResMessageSize = 0;			
							}
							else	
							{
								ulResMessageSize = sizeof(S_RFSAPR_RES_ARINC429_QT_TEST_RESULT);
							}	
						}
								
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_ARINC_PERFORM_QT_TEST;			

						break;						
						

					
					default:
							   /*
								*	------------ ERROR ------------					
								*	Z   GroupId - Invalid Unit ID
								*	-------------------------------
								*/
								dp_rfsapr_printf("ERROR\t: INVAILD UNIT ID\r\n");
								sRetVal = DP_RFSAPR_INVALID_UNIT_ID;									
						break;
					}
				break;
				}
		case RFSAPR_COMM_CMD_GROUP2:/*TODO , Group added for the 1553b communication*/
			{
				switch(usCmdUnitID)
				{
				case RFSAPR_CMD_DPXMC5028_1553B_BC_CONFIGURE:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received 1553B BC configure command \n");	
#endif				
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/* Perform Test on each configuration */
						sRetVal	= DPXMC5028_BCTestCommand(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_cmd_1553bBCConfig);				
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DPXMC5028_1553B_BC_CONFIGURE;					

					break;
				case RFSAPR_CMD_1553B_BC_DEFINE_MESSAGES_AND_FRAME:

#ifdef DP_1553B_PRINTS_EN
					printf("Received 1553B BC Define Messages and frame command \n");	
#endif				
					
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/* Perform Test on each configuration */
						sRetVal	= DPXMC5028_Configure_BC_Messages_and_Frame(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1553bBCFrmMsgs);	
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_BC_DEFINE_MESSAGES_AND_FRAME;	

					break;		
				case RFSAPR_CMD_1553B_START_BC_RECEIVE_MSGS:		
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received Command RFSAPR_CMD_1553B_START_BC_RECEIVE_MSGS \n");	
#endif				

					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/* Perform Test on each configuration */
						sRetVal	= DPXMC5028_Start_BC_And_ReceiveMessages(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1553bBCStrtRcv);
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_START_BC_RECEIVE_MSGS;	

					break;
				case RFSAPR_CMD_1553B_BC_STOP:
				
#ifdef DP_1553B_PRINTS_EN
					printf("Received Command RFSAPR_CMD_1553B_BC_STOP \n");	
#endif									

					ucChNo = pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1553bBCStrtStop.m_ucBCChNo;	
#ifdef DP_1553B_PRINTS_EN
					printf("Received Channel No: %d \n",ucChNo);	
#endif				
						if((ucChNo != 1) && (ucChNo !=2))
						{								
				             DP_RFSAPR_LOG_DATA(DP_RFSAPR_LOG_FLAG ,"Received BC channel no. :%d valid input(1-2)\n",ucChNo);												
				             sRetVal = PARAM_ERROR_ACK;
							 break;		
				        }						
						else
						{
#if DPXMC5028_BOARD_PRESENT            
					         sRetVal = DPXMC5028_1553B_BC_Stop(&g_RFSAPR_Handle.m_iDPXMC5028_BrdHandle[ucChNo-1],ucChNo);
#endif
					         if(sRetVal)
					         {
#if DPXMC5028_BOARD_PRESENT	
								DPXMC5028_GetErrorMessage(sRetVal, carrErrMsg,100);
#endif		
								printf("DP-XMC-5028 - DPXMC5028_1553B_BC_Stop Error : %s Error Code : %d \n",carrErrMsg ,sRetVal);
								DP_RFSAPR_LOG_DATA(DP_RFSAPR_LOG_FLAG,"DP-XMC-5028 Driver Error : %s Error Code : %d \n",carrErrMsg ,sRetVal);
								break;						     
					         }

						}
#ifdef DP_1553B_PRINTS_EN
					printf("BC Stopped succesfully in target\n");	
#endif				
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_BC_STOP;	
						
						break;						
				case RFSAPR_CMD_1553B_SEND_BC_DATA_TO_HOST:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received Command RFSAPR_CMD_1553B_SEND_BC_DATA_TO_HOST \n");	
#endif				
						/* Perform Test on each configuration */
						sRetVal	= DPXMC5028_Send_BC_Data_to_Host();			
						ulResMessageSize = sizeof(S_RFSAPR_RES_1553B_SEND_BC_DATA_TO_HOST);		
					
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_SEND_BC_DATA_TO_HOST;
					
					break;					
				case RFSAPR_CMD_DPXMC5028_1553B_RT_CONFIGURE:
				
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_DPXMC5028_1553B_RT_CONFIGURE Command\n");	
#endif									
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= DPXMC5028_RTTestCommand(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_cmd_1553bRTConfig);
							ulResMessageSize = 0;	
						}
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DPXMC5028_1553B_RT_CONFIGURE;					

					break;
				case RFSAPR_CMD_1553B_START_RT:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_1553B_START_RT Command\n");	
#endif									
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{

							ucChNo = (pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1553bRTCmn.m_ucRTchnno);
							
#ifdef DP_1553B_PRINTS_EN
							printf("Received Channel No : %d\n",ucChNo);	
#endif									
							if((ucChNo != 1) && (ucChNo !=2))
							{								
								DP_RFSAPR_LOG_DATA(DP_RFSAPR_LOG_FLAG ,"Received RT channel no. :%d valid input(1-2)\n",ucChNo);												
								sRetVal = PARAM_ERROR_ACK;
								break;
							}
													
#if DPXMC5028_BOARD_PRESENT 	
							sRetVal = DPXMC5028_1553B_RT_Start(&g_RFSAPR_Handle.m_iDPXMC5028_BrdHandle[ucChNo-1],1);
#endif
							if(sRetVal)
							{
#if DPXMC5028_BOARD_PRESENT	
								DPXMC5028_GetErrorMessage(sRetVal, carrErrMsg,100);
#endif		
								printf("DP-XMC-5028 - DPXMC5028_1553B_RT_Start Error : %s Error Code : %d \n",carrErrMsg ,sRetVal);
								DP_RFSAPR_LOG_DATA(DP_RFSAPR_LOG_FLAG,"DP-XMC-5028 Driver Error : %s Error Code : %d \n",carrErrMsg ,sRetVal);
								return sRetVal;
								break;
							}		
#ifdef DP_1553B_PRINTS_EN
							printf("RT started \n");	
#endif									
							ulResMessageSize = 0;		
							pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_START_RT;													
						}
					break;					
				case RFSAPR_CMD_1553B_RECEIVE_RT_MESSAGES:
#ifdef DP_1553B_PRINTS_EN
						printf("Received RFSAPR_CMD_1553B_RECEIVE_RT_MESSAGES Command\n");	
#endif									
						
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= DPXMC5028_1553B_Start_RT_Receive_Msgs(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmd1553bRTRcvMsgs);
							ulResMessageSize = 0;	
						}
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_RECEIVE_RT_MESSAGES;					

					break;		
					
				case RFSAPR_CMD_1553B_BC_RT_QT_TEST:
#ifdef DP_1553B_PRINTS_EN
						printf("Received RFSAPR_CMD_1553B_BC_RT_QT_TEST Command\n");	
#endif									
						
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= DPXMC5028_1553B_QT_BCtoRT_Test(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmd1553BBCtoRTQTtest);
							/*ulResMessageSize = sizeof(S_RFSAPR_RES_QT_1553B_BC_RT_DATA_OUT);*/
							ulResMessageSize = 0;
						}
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_BC_RT_QT_TEST;					

					break;							
					
				case RFSAPR_CMD_1553B_STOP_RT_RECEIVING:
					
#ifdef DP_1553B_PRINTS_EN
						printf("Received RFSAPR_CMD_1553B_STOP_RT_RECEIVING Command\n");	
#endif									
						if(ulRxMsgLength == 0)
						{		
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}						
						else
						{
							ucChNo =pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1553bRTCmn.m_ucRTchnno;
							
#ifdef DP_1553B_PRINTS_EN
						printf("Received Channel No : %d\n" ,ucChNo);	
#endif																
							
							if((ucChNo != 1) && (ucChNo !=2))
							{								
								DP_RFSAPR_LOG_DATA(DP_RFSAPR_LOG_FLAG ,"Received RT channel no. :%d valid input(1-2)\n",ucChNo);												
								sRetVal = PARAM_ERROR_ACK;
								break;
							}
#if DPXMC5028_BOARD_PRESENT             
					         sRetVal = DPXMC5028_1553B_RT_Stop(&g_RFSAPR_Handle.m_iDPXMC5028_BrdHandle[ucChNo-1],1);
#endif
					         if(sRetVal)
					         {
#if DPXMC5028_BOARD_PRESENT	
								DPXMC5028_GetErrorMessage(sRetVal, carrErrMsg,100);
#endif		
								printf("DP-XMC-5028 - DPXMC5028_1553B_RT_Stop Error : %s Error Code : %d \n",carrErrMsg ,sRetVal);
								DP_RFSAPR_LOG_DATA(DP_RFSAPR_LOG_FLAG,"DP-XMC-5028 Driver Error : %s Error Code : %d \n",carrErrMsg ,sRetVal);
				        	    return sRetVal;
								break;
					         }
							 ulResMessageSize = 0;	
						}
						
#ifdef DP_1553B_PRINTS_EN
						printf("RT stopping Success\n");	
#endif																						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_STOP_RT_RECEIVING;											
						
					break;
				case RFSAPR_CMD_1553B_SEND_RT_DATA_TO_HOST:
					
#ifdef DP_1553B_PRINTS_EN
						printf("Received RFSAPR_CMD_1553B_SEND_RT_DATA_TO_HOST Command\n");	
#endif									

						if(ulRxMsgLength == 0)
						{							
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;													
						}
						else
						{	
							sRetVal	= DPXMC5028_Send_RT_Data_to_Host(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1553bRTCmn);
							if(sRetVal)
							{
								ulResMessageSize = 0;			
							}
							else	
							{
								ulResMessageSize = sizeof(S_RFSAPR_RES_RT_MESSAGE);									
							}	
						}
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_SEND_RT_DATA_TO_HOST;
					
					break;	
					
				case RFSAPR_CMD_1553B_SEND_QT_BCTORT_DATA_TO_HOST:
					
#ifdef DP_1553B_PRINTS_EN
						printf("Received RFSAPR_CMD_1553B_SEND_QT_BCTORT_DATA_TO_HOST Command\n");	
#endif									
						if(ulRxMsgLength != 0)
						{							
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;													
						}
						else
						{	
							sRetVal	= DPXMC5028_Send_QT_BCTORT_Data_to_Host(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_1553bBCtoRTQTtest);
							if(sRetVal)
							{
								ulResMessageSize = 0;			
							}
							else	
							{
								ulResMessageSize = sizeof(S_RFSAPR_RES_QT_1553B_BC_RT_DATA_OUT);									
							}	
						}
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_SEND_QT_BCTORT_DATA_TO_HOST;
					
					break;						
				case RFSAPR_CMD_DPXMC5028_1553B_MT_CONFIGURE:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_DPXMC5028_1553B_MT_CONFIGURE Command\n");	
#endif														
				
						if(ulRxMsgLength == 0)
						{
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
						}
						else
						{
							/* Perform Test on each configuration */
							sRetVal	= DPXMC5028_MTTestCommand(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_cmd_1553bMTConfig);
							ulResMessageSize = 0;	
						}
						
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_DPXMC5028_1553B_MT_CONFIGURE;									

					break;	
				case RFSAPR_CMD_1553B_START_MONITOR_MT:
						
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_1553B_START_MONITOR_MT Command\n");	
#endif														
					
					if(ulRxMsgLength == 0)
					{		
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;				
					}
					else
					{	
						sRetVal	= DPXMC5028_Start_MT_Monitor(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1553bMTStart);
						ulResMessageSize = 0;	
					}
					
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_START_MONITOR_MT;														
					
					break;
				case RFSAPR_CMD_1553B_STOP_MT_MONITORING:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_1553B_STOP_MT_MONITORING Command\n");	
#endif																									
											
						if(ulRxMsgLength == 0)
						{	
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;									
						}						
						else
						{	
							ucChNo =pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_1553bMTStop.m_ucMTChNo;
#ifdef DP_1553B_PRINTS_EN
							printf("Received channel No : %d \n",ucChNo);
#endif
							if((ucChNo != 1) && (ucChNo !=2))
							{								
								DP_RFSAPR_LOG_DATA(DP_RFSAPR_LOG_FLAG ,"Received MT channel no. :%d valid input(1-2)\n",ucChNo);												
								sRetVal = PARAM_ERROR_ACK;
								break;
							}							
#if	DPXMC5028_BOARD_PRESENT	
							sRetVal = DPXMC5028_1553B_MT_Stop( &g_RFSAPR_Handle.m_iDPXMC5028_BrdHandle[ucChNo-1]);
#endif
							if(sRetVal)
							{
#if DPXMC5028_BOARD_PRESENT	
								DPXMC5028_GetErrorMessage(sRetVal, carrErrMsg,100);
#endif		
								printf("DP-XMC-5028 - DPXMC5028_1553B_MT_Stop Error : %s Error Code : %d \n",carrErrMsg ,sRetVal);
								DP_RFSAPR_LOG_DATA(DP_RFSAPR_LOG_FLAG,"DP-XMC-5028 Driver Error : %s Error Code : %d \n",carrErrMsg ,sRetVal);
								ulResMessageSize = 0;
								break;
							}
							ulResMessageSize = 0;	
						}
						pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_STOP_MT_MONITORING;
						
					break;					
				case RFSAPR_CMD_1553B_SEND_MT_DATA_TO_HOST:

#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_1553B_SEND_MT_DATA_TO_HOST Command\n");	
#endif																									
						sRetVal	= DPXMC5028_Send_MT_Data_to_Host();
						if(sRetVal)
						{
							ulResMessageSize = 0;			
						}
						else	
						{
							ulResMessageSize = sizeof(S_RFSAPR_RES_SMT_MESSAGE);
						}	

					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_SEND_MT_DATA_TO_HOST;																				

					break;					

				case RFSAPR_CMD_1553B_SEND_SELFTEST_STATUS:
						
						if(ulRxMsgLength != 0)
						{							
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;													
						}
						else
						{	
							sRetVal	= DPXMC5028_Perform_1553B_Selftest(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_1553B_ArincTest);
							if(sRetVal)
							{
								ulResMessageSize = 0;			
							}
							else	
							{
								ulResMessageSize = sizeof(S_RFSAPR_1553B_ARINC_SELFTEST);
							}	
						}
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_1553B_SEND_SELFTEST_STATUS;																				

					break;		

				case RFSAPR_CMD_READ_MT_DATA:
						
						if(ulRxMsgLength == 0)
						{							
							sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;													
						}
						else
						{	
							sRetVal	= DPXMC5028_Receive_MTData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.m_SCmd1553BMTReadData.m_ucChhnlNo);
							if(sRetVal)
							{
								dp_rfsapr_printf("MT Read Data failed \n");
							}
							ulResMessageSize = 0;			

						}
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_MT_DATA;																				

					break;		

				default:
					   /*
						*	------------ ERROR ------------					
						*	Z   GroupId - Invalid Unit ID
						*	-------------------------------
						*/
						dp_rfsapr_printf("ERROR\t: INVAILD UNIT ID\r\n");
						sRetVal = DP_RFSAPR_INVALID_UNIT_ID;									
					break;
				}
				break;
			}
		case RFSAPR_COMM_CMD_GROUP3:/*TODO , Group added for Codec flash Operations*/
			{
				switch(usCmdUnitID)
				{
								
				case RFSAPR_CMD_WRITE_AUDIO_DATA:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_WRITE_AUDIO_DATA command \n");	
#endif							
					
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						
						sRetVal	= WriteAudioData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Write_Audio_Data.m_ucAudioNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Write_Audio_Data.m_uiAudioSize,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Write_Audio_Data.m_arrucAudioData,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Write_Audio_Data.m_uiPktID);									
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_AUDIO_DATA;					

					break;
				
				case RFSAPR_CMD_STORE_AUDIO_DATA:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_STORE_AUDIO_DATA command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= StoreAudioData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Store_Audio_Data.m_ucAudioNo,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Store_Audio_Data.m_uiAudioSize);									
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_STORE_AUDIO_DATA;					

					break;
				case RFSAPR_CMD_READ_AUDIO_DATA:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_READ_AUDIO_DATA command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{
						printf("Rx message size :%lu \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						/*sRetVal	= ReadAudioData(U8BIT in_u8AudioNo, PU32BIT out_u32ReadAudioSize, PU8BIT out_pu8AudioData);*/
						sRetVal	= DPXMC5028_Read_Audio_Codec_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Read_Audio_Codec_Data.m_ucAudioNo,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_Audio_codec_Data); 				
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_AUDIO_DATA);	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_AUDIO_DATA;					

					break;
				case RFSAPR_CMD_ERASE_AUDIO_DATA:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_ERASE_AUDIO_DATA command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= EraseAudioData(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Erase_Audio_Codec_Data.m_ucAudioNo);
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_ERASE_AUDIO_DATA;					

					break;
				case RFSAPR_CMD_PLAY_FLASH_AUDIO:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_PLAY_FLASH_AUDIO command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= DPXMC5028_Play_Flash_Audio(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Play_Flash_Audio);
					
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_PLAY_FLASH_AUDIO;					

					break;
				case RFSAPR_CMD_STOP_AUDIO_PLAY:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_STOP_AUDIO_PLAY command \n");	
#endif													
					if(ulRxMsgLength != 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= StopAudio();
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_STOP_AUDIO_PLAY;					

					break;
				case RFSAPR_CMD_CONFIGURE_AUDIO_VOL:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_CONFIGURE_AUDIO_VOL command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= DPXMC5028_Config_Audio_Vol(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_Audio_vol); 				
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIGURE_AUDIO_VOL;					

					break;
				case RFSAPR_CMD_AUDIO_MUTE_UNMUTE:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_AUDIO_MUTE_UNMUTE command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= MuteAudio(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_Audio_MuteUnmute.m_ucMuteUnmute);
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_AUDIO_MUTE_UNMUTE;					

					break;
				case RFSAPR_CMD_AUDIO_STATUS:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_AUDIO_STATUS command \n");	
#endif													
					if(ulRxMsgLength != 0)
					{
						printf("Rx message size :%lu \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{						
						sRetVal	= DPXMC5028_Audio_Playing_Sts(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_Audio_Play_Sts.m_ucStatus); 				
						ulResMessageSize = sizeof(S_RFSAPR_RES_AUDIO_STATUS);	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_AUDIO_STATUS;					

					break;					
				case RFSAPR_CMD_STORE_AUDIO_CODEC_DEFAULT_CONFIG_FLASH:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_STORE_AUDIO_CODEC_DEFAULT_CONFIG_FLASH command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= LoadPowerOnConfig(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Store_Default_Codec_Config.m_arruiData);
					/*	sRetVal	= DPXMC5028_Store_Codec_Default_Config(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Store_Default_Codec_Config);*/ 				
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_STORE_AUDIO_CODEC_DEFAULT_CONFIG_FLASH;					

					break;		
				case RFSAPR_CMD_READ_AUDIO_CODEC_DEFAULT_CONFIG_FLASH:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_READ_AUDIO_CODEC_DEFAULT_CONFIG_FLASH command \n");	
#endif													
					if(ulRxMsgLength != 0)
					{
						printf("Rx message size :%lu \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						
						sRetVal	= ReadPowerOnConfig(pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_Codec_Default_Config.m_arruiData);
					/*	sRetVal	= DPXMC5028_Read_Codec_Default_config(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_Codec_Default_Config);*/ 				
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_AUDIO_CODEC_DEFAULT_CONFIG_FLASH);	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_AUDIO_CODEC_DEFAULT_CONFIG_FLASH;					

					break;			
				case RFSAPR_CMD_CONFIG_AUDIO_CODEC:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_CONFIG_AUDIO_CODEC command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= LoadAudioCodecConfig(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_Audio_Codec.m_ucConfigCnt,pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_Audio_Codec.m_arruiData);
						/*sRetVal	= DPXMC5028_Config_Audio_Codec(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Config_Audio_Codec);*/ 				
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_CONFIG_AUDIO_CODEC;					

					break;		
				case RFSAPR_CMD_READ_CONFIG_AUDIO_CODEC:
					
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_READ_CONFIG_AUDIO_CODEC command \n");	
#endif													
					if(ulRxMsgLength != 0)
					{
						printf("Rx message size :%lu \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= ReadAudioCodecConfig(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_Configrd_Audio_Codec.m_ucConfigCnt,pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_Configrd_Audio_Codec.m_arruiData); 				
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_CONFIG_AUDIO_CODEC);	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_CONFIG_AUDIO_CODEC;					

					break;	
					
				case RFSAPR_CMD_WRITE_CODEC_PLL_CONFIG:	
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_WRITE_CODEC_PLL_CONFIG command \n");
#endif													
					if(ulRxMsgLength == 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						
						sRetVal	= DPXMC5028_Write_Audio_Codec_PLL_Data(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_Codec_PLL_Data);					
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_CODEC_PLL_CONFIG;					

					break;	

				case RFSAPR_CMD_READ_CODEC_PLL_CONFIG:	
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_READ_CODEC_PLL_CONFIG command \n");	
#endif													
					if(ulRxMsgLength != 0)
					{
						printf("Rx message size :%lu \n",ulRxMsgLength);
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= DPXMC5028_Read_Audio_Codec_PLL_Data(&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_Read_Audio_Codec_PLL_Data); 				
						ulResMessageSize = sizeof(S_RFSAPR_RES_READ_CODEC_PLL_CONFIG);	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_CODEC_PLL_CONFIG;					

					break;	
				case RFSAPR_CMD_WRITE_GPIO_DATA:	
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_WRITE_GPIO_DATA command \n");	
#endif													
					if(ulRxMsgLength == 0)
					{						
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= DPXMC5028_Write_GPIO_Data(&pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_5028_GPIO_WR_Data); 				
						ulResMessageSize = 0;	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_WRITE_GPIO_DATA;					

					break;	
				case RFSAPR_CMD_READ_GPIO_DATA:	
#ifdef DP_1553B_PRINTS_EN
					printf("Received RFSAPR_CMD_READ_GPIO_DATA command \n");	
#endif													
					if(ulRxMsgLength != 0)
					{
						sRetVal = DP_RFSAPR_INVALID_CMD_MSG_SIZE;
					}
					else
					{
						sRetVal	= DPXMC5028_Read_GPIO_Data(pSCommandFromRFSAPR->UCmdRFSAPRMessage.S_Cmd_5028_GPIO_RD_Data.m_ucTRLineNo,&pSResponseToRFSAPR->UResRFSAPRMessage.S_Res_5028_GPIO_RD_Data.m_ucGPIORdData); 				
						ulResMessageSize = sizeof(S_RFSAPR_RES_GPIO_READ_DATA);	
					}
						
					pSResponseToRFSAPR->SRFSAPRResIds.usUnitId = RFSAPR_RES_READ_GPIO_DATA;					

					break;						
					
				default:
					   /*
						*	------------ ERROR ------------					
						*	Z   GroupId - Invalid Unit ID
						*	-------------------------------
						*/
						dp_rfsapr_printf("ERROR\t: INVAILD UNIT ID\r\n");
						sRetVal = DP_RFSAPR_INVALID_UNIT_ID;									
					break;
				}
				break;
			}			
			default:
			{
				   /*
					*	------------ ERROR ------------					
					*	   GroupId - Invalid Group ID
					*	-------------------------------
					*/
					dp_rfsapr_printf("ERROR\t: INVAILD GROUP ID\r\n");
					sRetVal = DP_RFSAPR_INVALID_GROUP_ID;
				
					break;
			}

		}
	
		/*Endian Swap*/
		sRetVal_EndianConv = DP_RFSAPR_EndianSwap(usCmdGroupID,pSResponseToRFSAPR->SRFSAPRResIds.usUnitId, pSResponseToRFSAPR->UResRFSAPRMessage.ucRFSAPRTxData);
	}

	/*Incase of endian conversion failure, indicate it as that some thing is seriously wrong in data structure handling*/
	if(sRetVal_EndianConv)
	{
		sRetVal = sRetVal_EndianConv;
	}
	
/*	printf("Endian Return Value : %d \n",sRetVal);*/

	DP_RFSAPR_ComposeResponse(sRetVal,ulResMessageSize);
	
	sRetVal = DP_RFSAPR_FrameTCP_TXBuffer(&g_RFSAPR_Handle.ulTxPacketLength);

	ulTxPacketLength = 0;

	while(ulTxPacketLength < g_RFSAPR_Handle.ulTxPacketLength)
	{
		if((g_RFSAPR_Handle.ulTxPacketLength -  ulTxPacketLength) > RFSAPR_MAX_TCP_STACK_SIZE)
		{
			iSendResult = dp_rfsapr_send( g_RFSAPR_Handle.sktClientSocket, (g_RFSAPR_Handle.cTransmitBuffer + ulTxPacketLength), RFSAPR_MAX_TCP_STACK_SIZE, 0 );						
		}
		else
		{
			iSendResult = dp_rfsapr_send( g_RFSAPR_Handle.sktClientSocket, (g_RFSAPR_Handle.cTransmitBuffer + ulTxPacketLength), (g_RFSAPR_Handle.ulTxPacketLength -  ulTxPacketLength), 0 );
		}
		
		if (iSendResult == SOCKET_ERROR) 
		{						
			break;
		}
		else
		{
			ulTxPacketLength += iSendResult;

		}
	}
	
	if(iSendResult == SOCKET_ERROR) 
	{
		return ((short)iSendResult);
	}

	return DP_RFSAPR_SUCCESS;
}

#ifdef WIN32
#pragma warning(pop)
#endif
/*! 
 *	\function		DP_RFSAPR_ComposeResponse()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function composes responses to the commands received from RFSAPR
 *	\param[in]		in_sMsgStatus 
 *	\param[in]		in_ulResMessageSize 
 *	\param[out]		None
 *	\return			DP_RFSAPR_SUCCESS
 */
short DP_RFSAPR_ComposeResponse(short in_sMsgStatus,unsigned long in_ulResMessageSize)
{
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPHeader[0]	=	0xEE;
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPHeader[1]	=	0xEF;
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPHeader[2]	=	0xFE;
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPHeader[3]	=	0xFF;
	
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.sMsgStatus		=	htons(in_sMsgStatus);	

	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.sReserved		= 0;	

	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ulMessageLength	=	in_ulResMessageSize;

	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usGroupId = htons(g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usGroupId);
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usUnitId = htons(g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usUnitId);	

	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPFooter[0]	=	0xFF;
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPFooter[1]	=	0xFE;
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPFooter[2]	=	0xEF;
	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPFooter[3]	=	0xEE;	

	return DP_RFSAPR_SUCCESS;
}

/*! 
 *	\function		DP_RFSAPR_FrameTCP_TXBuffer()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function frames the TCP transmission buffer.
 *	\param[in]		None 
 *	\param[out]		out_ulTxPacketLength - Transmit Buffer Length
 *	\return			DP_RFSAPR_SUCCESS
 */
short DP_RFSAPR_FrameTCP_TXBuffer(unsigned long *out_ulTxPacketLength)
{
	unsigned long ulBufferIndex = 0, ulMessageLength = 0;

	if(out_ulTxPacketLength == NULL)
	{
		return DP_RFSAPR_NULL_PTR_ACCESS;
	}

	ulMessageLength = g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ulMessageLength;

	g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ulMessageLength = htonl(g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ulMessageLength);

	memcpy(&g_RFSAPR_Handle.cTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPHeader,RFSAPR_HEADER_SIZE);

	ulBufferIndex = ulBufferIndex + RFSAPR_HEADER_SIZE;

	memcpy(&g_RFSAPR_Handle.cTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.sMsgStatus,RFSAPR_MSGSTS_SIZE);

	ulBufferIndex = ulBufferIndex + RFSAPR_MSGSTS_SIZE;

	memcpy(&g_RFSAPR_Handle.cTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.sReserved,RFSAPR_MSG_RESERVED_SIZE);

	ulBufferIndex = ulBufferIndex + RFSAPR_MSG_RESERVED_SIZE;

	memcpy(&g_RFSAPR_Handle.cTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ulMessageLength,RFSAPR_MSGLEN_SIZE);

	ulBufferIndex = ulBufferIndex + RFSAPR_MSGLEN_SIZE;

	memcpy(&g_RFSAPR_Handle.cTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds,sizeof(S_RFSAPR_IDS));

	ulBufferIndex = ulBufferIndex + sizeof(S_RFSAPR_IDS);

	memcpy(&g_RFSAPR_Handle.cTransmitBuffer[ulBufferIndex], &g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.SResponseToRFSAPR.UResRFSAPRMessage.ucRFSAPRTxData,ulMessageLength);

	ulBufferIndex = ulBufferIndex + ulMessageLength;

	memcpy(&g_RFSAPR_Handle.cTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_TxTcpPacket.ucTCPFooter,RFSAPR_FOOTER_SIZE);	

	ulBufferIndex = ulBufferIndex + RFSAPR_FOOTER_SIZE;

	*out_ulTxPacketLength = ulBufferIndex;

	return DP_RFSAPR_SUCCESS;
}

/*! 
 *	\function		DP_RFSAPR_ComposeResponse()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function composes responses to the commands received from RFSAPR
 *	\param[in]		in_sMsgStatus 
 *	\param[in]		in_ulResMessageSize 
 *	\param[out]		None
 *	\return			DP_RFSAPR_SUCCESS
 */
short DP_RFSAPR_ComposeOnlineResponse(short in_sMsgStatus,unsigned long in_ulResMessageSize)
{
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPHeader[0]	=	0xEE;
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPHeader[1]	=	0xEF;
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPHeader[2]	=	0xFE;
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPHeader[3]	=	0xFF;
	
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.sMsgStatus		=	htons(in_sMsgStatus);	

	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.sReserved		= 0;	

	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ulMessageLength	=	in_ulResMessageSize;

	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usGroupId = htons(g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usGroupId);
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usUnitId = htons(g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds.usUnitId);	

	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPFooter[0]	=	0xFF;
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPFooter[1]	=	0xFE;
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPFooter[2]	=	0xEF;
	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPFooter[3]	=	0xEE;	

	return DP_RFSAPR_SUCCESS;
}

/*! 
 *	\function		DP_RFSAPR_FrameTCP_TXBuffer()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			This function frames the TCP transmission buffer.
 *	\param[in]		None 
 *	\param[out]		out_ulTxPacketLength - Transmit Buffer Length
 *	\return			DP_RFSAPR_SUCCESS
 */
short DP_RFSAPR_FrameTCP_OnlineTXBuffer(unsigned long *out_ulTxPacketLength)
{
	unsigned long ulBufferIndex = 0, ulMessageLength = 0;

	if(out_ulTxPacketLength == NULL)
	{
		return DP_RFSAPR_NULL_PTR_ACCESS;
	}

	ulMessageLength = g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ulMessageLength;

	g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ulMessageLength = htonl(g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ulMessageLength);

	memcpy(&g_RFSAPR_Handle.cOnlineTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPHeader,RFSAPR_HEADER_SIZE);

	ulBufferIndex = ulBufferIndex + RFSAPR_HEADER_SIZE;

	memcpy(&g_RFSAPR_Handle.cOnlineTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.sMsgStatus,RFSAPR_MSGSTS_SIZE);

	ulBufferIndex = ulBufferIndex + RFSAPR_MSGSTS_SIZE;

	memcpy(&g_RFSAPR_Handle.cOnlineTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.sReserved,RFSAPR_MSG_RESERVED_SIZE);

	ulBufferIndex = ulBufferIndex + RFSAPR_MSG_RESERVED_SIZE;

	memcpy(&g_RFSAPR_Handle.cOnlineTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ulMessageLength,RFSAPR_MSGLEN_SIZE);

	ulBufferIndex = ulBufferIndex + RFSAPR_MSGLEN_SIZE;

	memcpy(&g_RFSAPR_Handle.cOnlineTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.SRFSAPRResIds,sizeof(S_RFSAPR_IDS));

	ulBufferIndex = ulBufferIndex + sizeof(S_RFSAPR_IDS);

	memcpy(&g_RFSAPR_Handle.cOnlineTransmitBuffer[ulBufferIndex], &g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.SResponseToRFSAPR.UResRFSAPRMessage.ucRFSAPRTxData,ulMessageLength);

	ulBufferIndex = ulBufferIndex + ulMessageLength;

	memcpy(&g_RFSAPR_Handle.cOnlineTransmitBuffer[ulBufferIndex],&g_RFSAPR_Handle.S_RFSAPR_OnlineTxTcpPacket.ucTCPFooter,RFSAPR_FOOTER_SIZE);	

	ulBufferIndex = ulBufferIndex + RFSAPR_FOOTER_SIZE;

	*out_ulTxPacketLength = ulBufferIndex;

	return DP_RFSAPR_SUCCESS;
}



/*! 
 *	\function		DP_RFSAPR_SetTimeOut()
 *	\author			Jayaram D
 *	\date			26-FEB-2016
 *	\brief			The routine used set the Time Out for RFSAPR
 *	\param[in]		in_sktConnectSocket - UART Socket Handle
 *	\param[in]		in_OptionName		- Timeout Selection for Send/Receive
 *	\param[in]		in_lMilliSeconds	- Timout value in milliseconds 
 *	\param[out]		None
 *	\return			DP_RFSAPR_SUCCESS
 */
#ifdef WIN32
	short DP_RFSAPR_SetTimeOut(SOCKET in_sktConnectSocket,int in_OptionName,long in_lMilliSeconds)
#else//elif linux
	short DP_RFSAPR_SetTimeOut(int in_sktConnectSocket,int in_OptionName,long in_lMilliSeconds)	
#endif
{
	int iResult = 0;

	struct timeval tvTimeOutValue;

#ifdef WIN32
	tvTimeOutValue.tv_sec = in_lMilliSeconds;
	tvTimeOutValue.tv_usec = 0;
#elif linux
	tvTimeOutValue.tv_sec = in_lMilliSeconds;
	tvTimeOutValue.tv_usec = 0;
#else
	tvTimeOutValue.tv_sec = (in_lMilliSeconds / 1000);
	tvTimeOutValue.tv_usec = (in_lMilliSeconds % 1000) * 1000;
#endif

	iResult = dp_rfsapr_setsockopt(in_sktConnectSocket, SOL_SOCKET, in_OptionName,(char *)(struct timeval *)&tvTimeOutValue,sizeof(struct timeval));	
	if (iResult == SOCKET_ERROR) 
	{
		if(in_OptionName == SO_RCVTIMEO)
		{

			if(g_RFSAPR_Handle.ucPrintLogState	==	RFSAPR_ENABLE_PRINT_LOGS)
			{
				dp_rfsapr_printf("ERROR\t: Set Socket Option Failure : SO_RCVTIMEO\r\n");
			}
		}
		else
		{
			if(g_RFSAPR_Handle.ucPrintLogState	==	RFSAPR_ENABLE_PRINT_LOGS)
			{
				dp_rfsapr_printf("ERROR\t: Set Socket Option Failure : SO_SNDTIMEO\r\n");
			}
		}		
	}
	
	return DP_RFSAPR_SUCCESS;
}

#ifdef WIN32
void DP_RFSAPR_PollSocketError_Thread()
#else
void DP_RFSAPR_PollSocketError_Thread(void *cookie)
#endif
{
#ifdef linux
	struct pollfd uffd = {0};	
	int iReturnVal = 0;
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	int err;

	/*Detach the thread*/
	dp_rfsapr_thread_detach();

	dp_rfsapr_printf("INFO\t: Socket poll task started\r\n");

	uffd.fd = g_RFSAPR_Handle.sktClientSocket;

	uffd.events = POLLERR | POLLHUP | POLLNVAL | POLLRDHUP;

	uffd.revents = 0;

	iReturnVal = poll(&uffd, 1, -1);
	/*
	printf("\n iReturnVal = %d \n", iReturnVal);
	printf("\n POLLHUP = %d \n", POLLHUP);
	printf("\n POLLERR = %d \n", POLLERR);
	printf("\n POLLNVAL = %d \n", POLLNVAL);
	printf("\n uffd.revents = %d \n", uffd.revents);
	*/

	if(uffd.revents & POLLHUP)
	{
		dp_rfsapr_printf("ERROR\t: RFSAPR - POLLHUP\r\n");
		g_RFSAPR_Handle.usSockErrExitFlag = 1;
	}
	else if(uffd.revents & POLLERR)
	{
		printf("\nERROR: RFSAPR - POLLERR\r\n");
		g_RFSAPR_Handle.usSockErrExitFlag = 1;
	}
	else if(uffd.revents & POLLNVAL)
	{
		dp_rfsapr_printf("\nERROR\t: RFSAPR - POLLNVAL\r\n");
		g_RFSAPR_Handle.usSockErrExitFlag = 1;
	}
	else if(uffd.revents & POLLRDHUP)
	{
		dp_rfsapr_printf("INFO\t: POLLRDHUP: RFSAPR Client Socket Closed \r\n");
		g_RFSAPR_Handle.usSockErrExitFlag = 1;
	}

	printf("\nExit from socket poll task...\n");

#endif
	return;
}


#ifdef WIN32
void DP_RFSAPR_OnlinePollSocketError_Thread()
#else
void DP_RFSAPR_OnlinePollSocketError_Thread(void *cookie)
#endif
{
#ifdef linux
	struct pollfd uffd = {0};	
	int iReturnVal = 0;
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);
	int err;

	/*Detach the thread*/
	dp_rfsapr_thread_detach();

	dp_rfsapr_printf("INFO\t: Socket poll task started\r\n");

	uffd.fd = SktOnlnCltSocket;

	uffd.events = POLLERR | POLLHUP | POLLNVAL | POLLRDHUP;

	uffd.revents = 0;

	iReturnVal = poll(&uffd, 1, -1);
	/*
	printf("\n iReturnVal = %d \n", iReturnVal);
	printf("\n POLLHUP = %d \n", POLLHUP);
	printf("\n POLLERR = %d \n", POLLERR);
	printf("\n POLLNVAL = %d \n", POLLNVAL);
	printf("\n uffd.revents = %d \n", uffd.revents);
	*/

	if(uffd.revents & POLLHUP)
	{
		dp_rfsapr_printf("ERROR\t: RFSAPR - POLLHUP\r\n");
		g_RFSAPR_Handle.usOnlineSockErrExitFlag = 1;
	}
	else if(uffd.revents & POLLERR)
	{
		printf("\nERROR: RFSAPR - POLLERR\r\n");
		g_RFSAPR_Handle.usOnlineSockErrExitFlag = 1;
	}
	else if(uffd.revents & POLLNVAL)
	{
		dp_rfsapr_printf("\nERROR\t: RFSAPR - POLLNVAL\r\n");
		g_RFSAPR_Handle.usOnlineSockErrExitFlag = 1;
	}
	else if(uffd.revents & POLLRDHUP)
	{
		dp_rfsapr_printf("INFO\t: POLLRDHUP: RFSAPR Client Socket Closed \r\n");
		g_RFSAPR_Handle.usOnlineSockErrExitFlag = 1;
	}

	printf("\nExit from socket poll task...\n");

#endif
	return;
}




/***********************************************************************************************/
