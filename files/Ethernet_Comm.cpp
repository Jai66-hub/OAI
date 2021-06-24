
#include "stdafx.h"
#include "dp_ate_8333_300.h"
#include "Ethernet_Comm.h"
#include "ErrorCodes.h"
#include <winsock2.h>



/*************************************************************************************************
* Name						:	RFSAPR_ETH_CriticalSectionInit()
* Author					:	Vinoth V
* Global Variables affected	:	Nil
* Created Date				:	Oct 20, 2015
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:	For Initializing Critical Secion
***************************************************************************************************/
short RFSAPR_ETH_CriticalSectionInit()
{
	MAINAPP(pAppHandle);
	// Initialize Critical Section
	InitializeCriticalSection(&pAppHandle->g_ObjETHCriticalSection);

	return DP_RFSAPR_SUCCESS;
}


/*************************************************************************************************
* Name						:	RFSAPR_ETH_Connect()
* Author					:	Vinoth V
* Global Variables affected	:	Nil
* Created Date				:	Oct 20, 2015
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:	For Connecting the Ethernet Port
***************************************************************************************************/
short RFSAPR_ETH_Connect(DP_RFSAPR_ETH_COMM *out_ETH_Comm)
{
	int iRetVal = 0;
	CString strTempPortNo = "", strTempIPAddr = "", csTemp = "";	
	SOCKET sktConnectSocket;
	sockaddr_in saSockAddr;
	WSADATA wsaData;
	fd_set Write, Err;
	MAINAPP(pAppHandle);
	TIMEVAL Timeout;
    Timeout.tv_sec = 2; /* Timeout in Sec */
    Timeout.tv_usec = 0; /* Timeout in MilliSec */
	
	if(out_ETH_Comm == NULL)
	{
		return DP_RFSAPR_APP_NULL_PTR_ACCESS; 
	}	

	FD_ZERO(&Write);
    FD_ZERO(&Err);
	
	/* Fills a block of memory with zeros */
	ZeroMemory(&saSockAddr,sizeof(sockaddr_in));
	
	/* Initialize Winsock */
    iRetVal = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iRetVal != DP_RFSAPR_SUCCESS) 
	{        
		pAppHandle->PrintLog("Windows Socket Startup failed",2);	
		out_ETH_Comm->bISConnected = false;
        return DP_RFSAPR_APP_SOCKET_START_FAIL;
    }
	else
	{
		pAppHandle->m_bIsEthWASStartUp = true;
	}

	
	
	strTempPortNo.Format("%d",out_ETH_Comm->uiPortNumber);
	strTempIPAddr.Format("%d.%d.%d.%d",out_ETH_Comm->ucAddr[0],out_ETH_Comm->ucAddr[1],out_ETH_Comm->ucAddr[2],out_ETH_Comm->ucAddr[3]);
	
	saSockAddr.sin_family = AF_INET;
	saSockAddr.sin_port = htons((unsigned short)atoi(strTempPortNo));
	saSockAddr.sin_addr.s_addr=inet_addr(strTempIPAddr);	
	
	/* Create a SOCKET for connecting to server */
	sktConnectSocket = socket(saSockAddr.sin_family, SOCK_STREAM, 0);
	if (sktConnectSocket == INVALID_SOCKET) 
	{      
		pAppHandle->PrintLog("Socket Creation failed",2);
        WSACleanup();
		out_ETH_Comm->bISConnected = false;
        return DP_RFSAPR_APP_SOCKET_CREATE_FAIL;
    }

	//set the socket in non-blocking
    unsigned long iMode = 1;
    int iResult = ioctlsocket(sktConnectSocket, FIONBIO, &iMode);
    if (iResult != NO_ERROR)
    {	
		csTemp.Format("ioctlsocket failed with error: %ld\n", iResult);
        pAppHandle->PrintLog(csTemp, 2);
    }	
		
    /* Connect to server.*/    
    if (connect(sktConnectSocket,(struct sockaddr*)&saSockAddr, (int)sizeof(sockaddr_in)) == false) 
	{
		closesocket(sktConnectSocket);
		sktConnectSocket = INVALID_SOCKET;     
    }
	
	if(sktConnectSocket == INVALID_SOCKET) 
	{		
		WSACleanup();
		out_ETH_Comm->bISConnected = false;
		closesocket(sktConnectSocket);
		sktConnectSocket = INVALID_SOCKET;		
		return DP_RFSAPR_APP_SOCKET_CREATE_FAIL;
    }    

	// restart the socket mode
    iMode = 0;
    iResult = ioctlsocket(sktConnectSocket, FIONBIO, &iMode);
    if (iResult != NO_ERROR)
    {	
		csTemp.Format("ioctlsocket failed with error: %ld\n", iResult);
        pAppHandle->PrintLog(csTemp, 2);
		out_ETH_Comm->bISConnected = false;
		closesocket(sktConnectSocket);
		sktConnectSocket = INVALID_SOCKET;		
		return DP_RFSAPR_APP_SOCKET_CREATE_FAIL;
    }

	FD_SET(sktConnectSocket, &Write);
    FD_SET(sktConnectSocket, &Err);
 
    // check if the socket is ready
    select(0,NULL,&Write,&Err,&Timeout);			
    if(FD_ISSET(sktConnectSocket, &Write) == 0) 
    {	
        out_ETH_Comm->bISConnected = false;
		closesocket(sktConnectSocket);
		sktConnectSocket = INVALID_SOCKET;		
		return DP_RFSAPR_APP_SOCKET_CREATE_FAIL;
    }
	
	out_ETH_Comm->uiConnectedSocket = sktConnectSocket;
	out_ETH_Comm->bISConnected = true;
	
	return DP_RFSAPR_SUCCESS;
}


/*************************************************************************************************
* Name						:	RFSAPR_ETH_DisConnect()
* Author					:	Vinoth V
* Global Variables affected	:	Nil
* Created Date				:	Oct 20, 2015
* Revision Date				:	NA
* Reason for Revising		:	NA
* Description				:	For DisConnecting the Ethernet Port
***************************************************************************************************/
short RFSAPR_ETH_DisConnect(DP_RFSAPR_ETH_COMM *out_ETH_Comm)
{	
	int iRetVal = 0;
	
	MAINAPP(pAppHandle);
	if(out_ETH_Comm == NULL)
	{
		return DP_RFSAPR_FAILURE; 
	}	
	
	iRetVal = shutdown(out_ETH_Comm->uiConnectedSocket, SD_BOTH); //Shutdown the both transmit and receive operation
	if(iRetVal != DP_RFSAPR_SUCCESS) 
	{
		pAppHandle->PrintLog("Connection Shutdown failure",2);        
	}
	
	iRetVal = closesocket(out_ETH_Comm->uiConnectedSocket);
	if(iRetVal != DP_RFSAPR_SUCCESS) 
	{
		pAppHandle->PrintLog("Socket close failure",2);        
	}
	
	pAppHandle->g_bPBITStatus = 0;
	pAppHandle->g_bBrdDetctnSts = 0;
	
	
/*	WSACleanup();

	pAppHandle->m_bIsEthWASStartUp = false;*/
	
	out_ETH_Comm->bISConnected = false;	

	return DP_RFSAPR_SUCCESS;	
}
















