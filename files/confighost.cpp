// confighost.cpp : implementation file
//

#include "stdafx.h"
#include "dp_ate_8333_300.h"
#include "confighost.h"
#include "Ethernet_Comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define _DEBUGGING_

/////////////////////////////////////////////////////////////////////////////
// CConfigHost
IMPLEMENT_DYNCREATE(CConfigHost, CFormView)

CConfigHost::CConfigHost()
	: CFormView(CConfigHost::IDD)
{
	//{{AFX_DATA_INIT(CConfigHost)
	m_strPortNo = _T("");
	m_iRdoEth1 = 0;
	m_Chkfan1 = FALSE;
	m_Chkfan2 = FALSE;
	m_ChkfanOnOff = FALSE;
	//}}AFX_DATA_INIT
	VERIFY(objTitleFont.CreateFont(
	30,                        // nHeight
	0,                         // nWidth
	0,                         // nEscapement
	0,                         // nOrientation
	FW_BOLD,                 // nWeight
	FALSE,                     // bItalic
	FALSE,                     // bUnderline
	0,                         // cStrikeOut
	ANSI_CHARSET,              // nCharSet
	OUT_DEFAULT_PRECIS,        // nOutPrecision
	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	DEFAULT_QUALITY,           // nQuality
	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	_T("Arial")));                // lpszFacename

	bInitFlag = 1;

}

CConfigHost::~CConfigHost()
{
}

void CConfigHost::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigHost)
	DDX_Control(pDX, IDC_LED_SG_DET_STS, m_chkSGDetSts);
	DDX_Control(pDX, IDC_LED_557_DET_STS, m_Chk557DetSts);
	DDX_Control(pDX, IDC_CHK_ETNCONN_STS, m_EthConnSts);
	DDX_Control(pDX, IDC_LED_5028_STS, m_5028Brd_Sts);
	DDX_Control(pDX, IDC_LED_1502_2, m_1502Brd2_Sts);
	DDX_Control(pDX, IDC_LED_1502_1, m_1502Brd1_Sts);
	DDX_Control(pDX, IDC_LED_1013_STS, m_1013_Brd_Sts);
	DDX_Control(pDX, IDC_IPADD_TARGET2, m_CtrlIPAddrs2);
	DDX_Control(pDX, IDC_IPADD_TARGET1, m_CtrlIPAddrs1);
	DDX_Text(pDX, IDC_EDT_PORT_NO, m_strPortNo);
	DDX_Radio(pDX, IDC_RDO_ETHERNET_SEL, m_iRdoEth1);
	DDX_Check(pDX, IDC_CHK_FAN_1, m_Chkfan1);
	DDX_Check(pDX, IDC_CHK_FAN_2, m_Chkfan2);
	DDX_Check(pDX, IDC_CHK_FAN_ONOFF, m_ChkfanOnOff);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_LST_PBIT_STATUS, m_listPBITSts);
}


BEGIN_MESSAGE_MAP(CConfigHost, CFormView)
	//{{AFX_MSG_MAP(CConfigHost)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHK_FAN_ONOFF, OnChkFanOnoff)
	ON_BN_CLICKED(IDC_BTN_FAN_ON_OFF, OnBtnFanOnOff)
	ON_BN_CLICKED(IDC_BTN_SYS_PWRON, OnBtnSysPwron)
	ON_BN_CLICKED(IDC_BTN_SYS_PWROFF, OnBtnSysPwroff)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADD_TARGET2, OnFieldchangedIpaddTarget2)
	//}}AFX_MSG_MAP


	ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED,	 IDC_LST_PBIT_STATUS, OnReportValueChanged)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigHost diagnostics

#ifdef _DEBUG
void CConfigHost::AssertValid() const
{
	CFormView::AssertValid();
}

void CConfigHost::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConfigHost message handlers

void CConfigHost::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	MAINAPP(pMainApp);

	
	bInitFlag = 0;	
	unsigned int uiTotRec = 0;
	
	CXTPReportRecords* pRecs	= NULL;
	CXTPReportRecord*  pRec		= NULL;
	
	CXTPReportRecordItemText* pItem = NULL;

	static bool bInitPBITDisp = true; 
	GetDlgItem(IDC_BTN_SYS_PWROFF)->EnableWindow(false);
	
	/*if(pMainApp->m_bPBITflag == true)*/
	if(bInitPBITDisp)
	{
	/*	pMainApp->m_bPBITflag = false;*/

		m_listPBITSts.AddColumn(new CXTPReportColumn(0, _T("S.No "),50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE));
		m_listPBITSts.AddColumn(new CXTPReportColumn(1, _T("Test"),210, FALSE, XTP_REPORT_NOICON, FALSE, TRUE));
		m_listPBITSts.AddColumn(new CXTPReportColumn(2, _T("Status"),55, FALSE, XTP_REPORT_NOICON, FALSE, TRUE));
		
		m_listPBITSts.FocusSubItems(TRUE);
		m_listPBITSts.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);
		// Diable the column remove option
		m_listPBITSts.GetReportHeader()->AllowColumnRemove(FALSE);
		// Fixed report column
		m_listPBITSts.GetReportHeader()->AllowColumnReorder(FALSE);
		
		//m_listPBITSts.GetColumns()->GetAt(1)->SetEditable(FALSE);	
		//m_listPBITSts.AllowEdit(true);
		
		m_listPBITSts.AddRecord(new CMessageRecord("1","MIL-STD-1553B", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("2","ARINC 429", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("3","Processor ALU Test", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("4","MRAM Test", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("5","SDRAM Test", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("6","Flash Test", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("7","UART Test", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("8","USB DOM", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("9","SATA DOM", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("10","WBDRX-1 FPGA Scratch Pad Test", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("11","WBDRX-2 FPGA Scratch Pad Test", "NA"));
		m_listPBITSts.AddRecord(new CMessageRecord("12","NBDRX FPGA Scratch Pad Test", "NA"));

		m_listPBITSts.SelectionEnable(false);
		m_listPBITSts.Populate();
	}
	else
	{
		bInitPBITDisp = true;
	}
	
	
	GetDlgItem(IDC_STC_TITLE)->SetFont(&objTitleFont);

	
	m_strPortNo.Format("%d", DP_RFSAPR_DEFAULT_PORT_NUM);
	
	//Provide Default Address for the Control 1
	
	if((pMainApp->g_RFSAPR_Eth_Config.ucAddr[0] == 0) && (pMainApp->g_RFSAPR_Eth_Config.ucAddr[1] == 0) && (pMainApp->g_RFSAPR_Eth_Config.ucAddr[2] == 0) && (pMainApp->g_RFSAPR_Eth_Config.ucAddr[3] == 0))
	{		
		m_CtrlIPAddrs1.SetAddress(DP_RFSAPR_DEFAULT_IP_ADDRESS_1, DP_RFSAPR_DEFAULT_IP_ADDRESS_2, DP_RFSAPR_DEFAULT_IP_ADDRESS_3, DP_RFSAPR_DEFAULT_IP_ADDRESS_4);
	}
	else
	{
		m_CtrlIPAddrs1.SetAddress(pMainApp->g_RFSAPR_Eth_Config.ucAddr[0], pMainApp->g_RFSAPR_Eth_Config.ucAddr[1], 
			pMainApp->g_RFSAPR_Eth_Config.ucAddr[2], pMainApp->g_RFSAPR_Eth_Config.ucAddr[3]);
	}
	
	//Provide Default Address for the Control 2
	
	if((pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[0] == 0) && (pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[1] == 0) && (pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[2] == 0) && (pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[3] == 0))
	{		
		m_CtrlIPAddrs2.SetAddress(DP_RFSAPR_ETH2_DEFAULT_IP_ADDRESS_1, DP_RFSAPR_ETH2_DEFAULT_IP_ADDRESS_2, DP_RFSAPR_ETH2_DEFAULT_IP_ADDRESS_3, DP_RFSAPR_ETH2_DEFAULT_IP_ADDRESS_4);
	}
	else
	{
		m_CtrlIPAddrs2.SetAddress(pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[0], pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[1], 
			pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[2], pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[3]);
	}
	
	
	LED_ControlsInit();

	m_chkSGDetSts.SetLedStatesNumber(MAX_LED_STATES);
	m_chkSGDetSts.SetIcon(LED_NONE,IDI_ICON_NONE_LED_NEW);
	m_chkSGDetSts.SetIcon(LED_SUCCESS,IDI_ICON_GREEN_LED_NEW);
	m_chkSGDetSts.SetIcon(LED_FAILURE,IDI_ICON_RED_LED_NEW);
	m_chkSGDetSts.SetLedState(LED_NONE);

	m_Chk557DetSts.SetLedStatesNumber(MAX_LED_STATES);
	m_Chk557DetSts.SetIcon(LED_NONE,IDI_ICON_NONE_LED_NEW);
	m_Chk557DetSts.SetIcon(LED_SUCCESS,IDI_ICON_GREEN_LED_NEW);
	m_Chk557DetSts.SetIcon(LED_FAILURE,IDI_ICON_RED_LED_NEW);
	m_Chk557DetSts.SetLedState(LED_NONE);

	/*Ethenet connection Status*/
	if(pMainApp->g_RFSAPR_Eth_Config.bISConfigured == true )
	{
		m_EthConnSts.SetLedState(LED_SUCCESS);
	}
	else
	{
		m_EthConnSts.SetLedState(LED_FAILURE);
	}
	
	if(pMainApp->g_b5028BrdDetcSts == 1)
	{
		m_5028Brd_Sts.SetLedState(LED_SUCCESS);
	}
	else
	{
		m_5028Brd_Sts.SetLedState(LED_FAILURE);
	}
	if(pMainApp->g_b1013BrdSts == 1)
	{
		m_1013_Brd_Sts.SetLedState(LED_SUCCESS);
	}
	else
	{
		m_1013_Brd_Sts.SetLedState(LED_FAILURE);
	}
	
	if(pMainApp->g_b1502Brd2Sts == 1)
	{
		m_1502Brd2_Sts.SetLedState(LED_SUCCESS);			
		
	}
	else
	{		
		m_1502Brd2_Sts.SetLedState(LED_FAILURE);
	}
	
	if(pMainApp->g_b1502Brd1Sts == 1)
	{
		m_1502Brd1_Sts.SetLedState(LED_SUCCESS);
	}
	else
	{
		m_1502Brd1_Sts.SetLedState(LED_FAILURE);
	}

	if(pMainApp->sGlobalInfo.sCOTSInstInfo.m_DPPci557flag == 1)
	{
		m_Chk557DetSts.SetLedState(LED_SUCCESS);
	}
	else
	{
		m_Chk557DetSts.SetLedState(LED_FAILURE);
	}

	if(pMainApp->sGlobalInfo.sCOTSInstInfo.m_bSigGen[0] == 1)
	{
		m_chkSGDetSts.SetLedState(LED_SUCCESS);
	}
	else
	{
		m_chkSGDetSts.SetLedState(LED_FAILURE);
	}

	
	if(pMainApp->m_bIsEthWASStartUp == true || 	pMainApp->m_bISOnlineWASStartUp == true )
	{
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(false);
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(true);
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(false);
	}
	
	/*** added for PBIT Test sts */
//#ifdef _DEBUGGING_	
	uiTotRec	= m_listPBITSts.GetRows()->GetCount();
	pRecs		= m_listPBITSts.GetRecords();

	for(UCHAR ucTestCaseCnt = 0;ucTestCaseCnt < uiTotRec; ucTestCaseCnt++)
	{
		pRec = pRecs->GetAt(ucTestCaseCnt);
		//		 =  pRec->GetItem(1)->GetCaption(m_listPBITSts.GetColumns()->GetAt(2));
		
		pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_listPBITSts.GetRecords()->GetAt(ucTestCaseCnt)->GetItem(2));
		
		if(pMainApp->g_PBITTestSts[ucTestCaseCnt])
		{
			pItem->SetCaption("PASS");
			pItem->SetBackgroundColor(RGB(0,128,0));
		}
		else
		{
			pItem->SetCaption("FAIL");
			pItem->SetBackgroundColor(RGB(255,0,0));
		}
		pItem = NULL;		
	}
	
	m_listPBITSts.Populate();
//#endif

	UpdateData(false);
	
}

void CConfigHost::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	tagRECT rcCorrection;	
	tagRECT rcBasePrevWnd;
		
	if(!bInitFlag)
	{
		UpdateData(true);
		//Base FrameControl
		//	Alignment
		//variable m_Align_mode -> holds the Mode of Anchoring (Horizontal/Vertical/Both)
		Anchor_BaseControl(IDC_STC_HOST_GROUP,BOTH_ANCHOR,rcCorrection);
		
		//Anchors all others Controls in the Windows wrt the BaseFrameControl Alignment
		AlignControl(IDC_STC_HOST_GROUP,rcBasePrevWnd,rcCorrection);
		Invalidate();	
		
	}
	
}



void CConfigHost::OnBtnConnect() 
{

	MAINAPP(pMainApp);
	MAINFRAM(pMainFrm);
	unsigned char ucTempflag = 0;
	S_RFSAPR_HW_TEMP_CALIB_DATA sRFSAPRTempCalibData;

	memset(&sRFSAPRTempCalibData, 0 , sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA));
	
	pMainApp->m_pobjOnlineMon->SendMessage(WM_UPDATE_ONLINEMON, 0, 0); /* Starting the Onlinemontoring */
	

	CString strExeName		= "";
	CString strTemp			= "";
	CString strHelpFilePath = "";
	CString strExeFilePath	= "";
	CString strActLog		= "";
	DWORD dwChkSum			= 0;
	DWORD dwErrorCode		= 0;

	unsigned int uiTrgtChksum	  = 0;
	unsigned int uiTrgtVersion	  = 0;

	unsigned long ulRxMessageSize = 0;
	char arrcData[MAX_ERR_STRING_LEN]			  = {0};
	char szErrStr[256]			  = {0};

	short	sRetVal		= RFSAPR_ATE_ZERO;
	int		iIoctlRet	= RFSAPR_ATE_ZERO;
	UCHAR	ucMonSelEth = RFSAPR_ATE_ZERO;
	

	struct tcp_keepalive keepalive_opts;
	keepalive_opts.onoff = 1;
	keepalive_opts.keepalivetime = 5;
	keepalive_opts.keepaliveinterval = 1;

	DP_RFSAPR_ETH_COMM	SEthComm1;
	DP_RFSAPR_ETH_COMM	SEthComm2;

	memset(&SEthComm1, 0, sizeof(DP_RFSAPR_ETH_COMM));
	memset(&SEthComm2, 0, sizeof(DP_RFSAPR_ETH_COMM));

	CCrc32Dynamic *pobCrc32Dynamic = NULL;


//	pMainFrm->ChangeRibbonBarView(false);
	UpdateData(true);

	if(m_CtrlIPAddrs1.IsBlank() || m_CtrlIPAddrs2.IsBlank())
	{
		AfxMessageBox("Either of the IP Controls is Empty! Provide IP Address.");//,MB_ICONWARNING);
		return ;
	}

	m_CtrlIPAddrs1.GetAddress(SEthComm1.ucAddr[0], SEthComm1.ucAddr[1], SEthComm1.ucAddr[2], SEthComm1.ucAddr[3]);
	
	m_strPortNo = "10030";
	
	SEthComm1.uiPortNumber = atoi(m_strPortNo);
	
	iIoctlRet = WSAIoctl(pMainApp->g_RFSAPR_Eth_Config.uiConnectedSocket, SIO_KEEPALIVE_VALS, &keepalive_opts, sizeof(keepalive_opts), NULL, 0, NULL, NULL, NULL);

	sRetVal = RFSAPR_ETH_Connect(&SEthComm1);
	
	if(sRetVal == RFSAPR_ATE_SUCCESS)
	{
		strActLog.Format("Ethernet Communication Established!");
		pMainApp->PrintLog(strActLog, 0);
		SEthComm1.bISConfigured = true;
		pMainApp->m_bIsEthWASStartUp = true;	
		memcpy(&pMainApp->g_RFSAPR_Eth_Config, &SEthComm1, sizeof(DP_RFSAPR_ETH_COMM));		

	/* This function shall set the RFSAPR System Date & Time in UTC Timezone
			 To be removed after implementing the timestorage in SBC */
		SetRFSAPRTimeandDate(); 	
		Sleep(300);

		EnaDisControls(true);

	}
	else
	{
		GetErrorMsg(sRetVal, arrcData);
		pMainApp->PrintLog(arrcData,2);		
		AfxMessageBox("Ethernet connection failed!", MB_ICONWARNING);		
		EnaDisControls(false);
	}


	if(m_iRdoEth1) //Ethernet 2 is selected for Monitoring
	{		
		// Get IP Address From Control
		m_CtrlIPAddrs2.GetAddress(SEthComm2.ucAddr[0], SEthComm2.ucAddr[1], SEthComm2.ucAddr[2], SEthComm2.ucAddr[3]);

	}
	else
	{
		memcpy(&SEthComm2,&SEthComm1,sizeof(DP_RFSAPR_ETH_COMM));
	}
		
	
	//Port Number for Online Monitoring
	SEthComm2.uiPortNumber = 10031;

	iIoctlRet = WSAIoctl(pMainApp->g_RFSAPR_OnlineEth_Config.uiConnectedSocket, SIO_KEEPALIVE_VALS, &keepalive_opts, sizeof(keepalive_opts), NULL, 0, NULL, NULL, NULL);

	sRetVal = pMainApp->m_pObjTCPOnlineSockComm->RFSAPR_ETH_Connect(&SEthComm2);

	if(sRetVal == RFSAPR_ATE_SUCCESS)
	{
		pMainApp->PrintLog("Online Ethernet Communication Established!", 0);
		pMainApp->g_RFSAPR_OnlineEth_Config.bISConfigured = true;
		pMainApp->m_bISOnlineWASStartUp = true;
		SEthComm2.bISConnected = true;
		SEthComm2.bISConfigured = true;
		memcpy(&pMainApp->g_RFSAPR_OnlineEth_Config, &SEthComm2, sizeof(SEthComm2));
		pMainApp->m_pobjOnlineMon->SendMessage(WM_UPDATE_ONLINEMON, 0, 0); /* Starting the Onlinemontoring */
	}
	else
	{
		GetErrorMsg(sRetVal, arrcData);
		pMainApp->PrintLog(arrcData,2);
		strActLog.Format("Online Ethernet connection failed!");
		AfxMessageBox(strActLog, MB_ICONWARNING);
		return;
	}
	
	/*Added by Srinivasu*/
	/*Host and Target Application Version & Checksum Details*/

	/************************Host Application************************************/
	strHelpFilePath = AfxGetApp()->m_pszHelpFilePath;
	strExeName = AfxGetApp()->m_pszExeName;
	strHelpFilePath = strHelpFilePath.Left (strHelpFilePath.ReverseFind ('\\'));
	strExeFilePath.Format("%s\\%s.exe",strHelpFilePath,strExeName);

	strTemp = "";

	pobCrc32Dynamic = new CCrc32Dynamic;

	pobCrc32Dynamic->Init();
	dwErrorCode = pobCrc32Dynamic->FileCrc32Win32(strExeFilePath,dwChkSum);

	pobCrc32Dynamic->Free();
	/*Host Application Checksum*/
	strTemp.Format("0x%X", dwChkSum);
	pMainApp->m_gstrAppChkSum = strTemp;
	/*Host Application Version*/
	strTemp.Format("%dV%0.2d",  APP_MAJOR_VER, APP_MINOR_VER);
	pMainApp->m_gstrAppVer = strTemp;

	/**************************************************************************/

	/************************Target Application********************************/
	/* Assign Group ID and Unit ID */
		pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0; 
		pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId  = RFSAPR_CMD_READ_BOARD_DET_STATUS;
		
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_Read_Brd_Sts.usWBDRxBrdNo = 0;
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_Read_Brd_Sts.usXMC5028BrdNo = 0;
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_Read_Brd_Sts.usNBDRxBrdNo = 0;
			
		/*Send Ethernet Command for read RFSAPR TYPE-1 App checksum and version details*/
		sRetVal = pMainApp->objTCPSocComm->RFSAPR_TCPCmdTrans(pMainApp->sktConnectSocket,&pMainApp->m_SCmdFromRFSAPRAtp,sizeof(S_CMD_BOARD_DET_STATUS),&pMainApp->m_SResToRFSAPRAtp,&ulRxMessageSize,100);
		
		if(sRetVal == RFSAPR_ATE_SUCCESS)
		{
			if((pMainApp->m_SResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->m_SResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId = RFSAPR_RES_READ_BOARD_DET_STATUS))
			{													
#ifdef ENDIAN_SWAP			
				sRetVal = EndianSwap(&pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts);
				if(sRetVal)
				{
					GetErrorMsg(sRetVal, arrcData);
					pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
					AfxMessageBox(arrcData,MB_ICONERROR);		
				}
#endif	
			}
			else
			{
				strActLog.Format("Invalid Unit ID or Group ID for reading RFSAPR TYPE-1 Application checksum and version details");
				pMainApp->PrintLog(strActLog,1);
			}
		}			
		else
		{
			strActLog.Format("Unable to for reading RFSAPR TYPE-1 Application checksum and version details");
			AfxMessageBox(strActLog,MB_ICONERROR);
			pMainApp->PrintLog(strActLog,DP_RFSAPR_PRINTLOG_FAILURES);
		}
	
	uiTrgtChksum = pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts.ulTrgtChkSum;
	uiTrgtVersion = pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts.ulTrgtVrsn;

	/*Read RFSAPR TYPE-1 Application checksum details */
	strTemp.Format("0x%X",uiTrgtChksum);
	pMainApp->m_gstrTarAppChkSum =  strTemp;
	/*Read RFSAPR TYPE-1 Application version details */
	strTemp.Format("%dV%0.2d",  APP_MAJOR_VER,uiTrgtVersion);
	pMainApp->m_gstrTarAppVer =  strTemp;
	/**************************************************************************/
	
	/*Set RTC*/

	COleDateTime	m_dtCurrDateTime;
	m_dtCurrDateTime = COleDateTime::GetCurrentTime();

		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SBC_RTC_Test.m_usYear		= m_dtCurrDateTime.GetYear();
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SBC_RTC_Test.m_ucMonth	= m_dtCurrDateTime.GetMonth() - 1;
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SBC_RTC_Test.m_ucDay		= m_dtCurrDateTime.GetDay();
		
		/* Set the TIME*/
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SBC_RTC_Test.m_ucHour		= m_dtCurrDateTime.GetHour();
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SBC_RTC_Test.m_ucMinutes  = m_dtCurrDateTime.GetMinute();
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SBC_RTC_Test.m_ucSeconds  = m_dtCurrDateTime.GetSecond();
		pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SBC_RTC_Test.m_ucWeekDay  = m_dtCurrDateTime.GetDayOfWeek()-1;
		
		/*Send an ethernet command to Set SBC RTC in RFSAPR Type 1 unit */
		pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0; 
		pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId  = RFSAPR_CMD_SBC_SET_RTC;
		
#ifdef ENDIAN_SWAP			
		sRetVal = EndianSwap(&pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SBC_RTC_Test);
		if(sRetVal)
		{
			strActLog.Format("Set SBC RTC Endian Error"); 
			pMainApp->PrintLog(strActLog, 1);
			AfxMessageBox(strActLog,MB_ICONERROR);
			return;
		}
#endif
		
#ifndef _8333_DEBUG_
		
		sRetVal = pMainApp->objTCPSocComm->RFSAPR_TCPCmdTrans(pMainApp->sktConnectSocket,&pMainApp->m_SCmdFromRFSAPRAtp,sizeof(S_RFSAPR_CMD_SBC_SET_RTC),&pMainApp->m_SResToRFSAPRAtp,&ulRxMessageSize,3000);
		
		if(sRetVal == RFSAPR_SUCCESS)
		{
			if((pMainApp->m_SResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->m_SResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId = RFSAPR_RES_SBC_SET_RTC))
			{
				strActLog.Format("Successfully Set the SBC RTC in RFSAPR Type 1 unit");
				pMainApp->PrintLog(strActLog,0);
				
			}
			else
			{
				strActLog.Format("Invalid Response Group/Unit ID for Set SDB RTC  ");
				pMainApp->PrintLog(strActLog,1);
			}
		}			
		else
		{
			strActLog.Format("Unable to Set SBC RTC in RFSAPR Type 1 unit");  
			AfxMessageBox(strActLog,MB_ICONERROR);
			pMainApp->PrintLog(strActLog,1);
			return;
		}
		
#endif

	UpdateData(false);


	/******** Added for the Temperature based calibration attenuation applying ******/	
#if 1

	ucTempflag = pMainApp->g_TemperatureSelflag;

	if(ucTempflag != 1)
	{
		
		for(UCHAR ucAntloop = 0 ; ucAntloop < 2; ucAntloop++)
		{
			sRetVal = Read_and_apply_TempCalibData(ucAntloop, ucTempflag);
			if(sRetVal)
			{
				strActLog.Format("Unable to Apply the temperatue based Attenuation");  
				pMainApp->PrintLog(strActLog,1);
			}	
		}
		
	}

#endif	

	/*****************************************/	
}

void CConfigHost::OnBtnDisconnect() 
{
	// TODO: Add your control notification handler code here
	MAINAPP(pMainApp);

	unsigned int uiTotRec = 0;
	CXTPReportRecords* pRecs	= NULL;
	CXTPReportRecord*  pRec		= NULL;
	
	CXTPReportRecordItemText* pItem = NULL;

	RFSAPR_ETH_DisConnect(&pMainApp->g_RFSAPR_Eth_Config);
	
	pMainApp->PrintLog("Ethernet Connection Closed..", 0);
	
	pMainApp->g_RFSAPR_Eth_Config.bISConfigured = false;

	pMainApp->m_pObjTCPOnlineSockComm->RFSAPR_ETH_DisConnect(&pMainApp->g_RFSAPR_OnlineEth_Config);
	
	pMainApp->PrintLog("Online Monitoring Ethernet Connection Closed..", 0);
	
	pMainApp->g_RFSAPR_OnlineEth_Config.bISConfigured = false;

	pMainApp->m_bIsEthWASStartUp = false;	
	pMainApp->m_bISOnlineWASStartUp = false;

	for(UCHAR uloop = 0; uloop < (sizeof(pMainApp->g_PBITTestSts)/sizeof(pMainApp->g_PBITTestSts[0])); uloop++)
	{
		pMainApp->g_PBITTestSts[uloop] = 0;
	}
	uiTotRec	= m_listPBITSts.GetRows()->GetCount();
	pRecs		= m_listPBITSts.GetRecords();

	for(UCHAR ucTestCaseCnt = 0;ucTestCaseCnt < uiTotRec; ucTestCaseCnt++)
	{
		pRec = pRecs->GetAt(ucTestCaseCnt);		
		pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_listPBITSts.GetRecords()->GetAt(ucTestCaseCnt)->GetItem(2));
		pItem->SetCaption("FAIL");
		pItem->SetBackgroundColor(RGB(255,0,0));
		pItem = NULL;		
	}

	m_listPBITSts.Populate();
	LED_ControlsInit();

	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(true);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(false);
	
	UpdateData(false);	
}


void CConfigHost::SetRFSAPRTimeandDate() 
{
	MAINAPP(pMainApp);


	short sRetVal = 0;
	CString strActLog = "";

	unsigned long ulRxMessageSize = 0;
	unsigned long ulTxMessageSize = 0;	
	unsigned char uc1502Brds	  =0;
	char arrcData[MAX_ERR_STRING_LEN] = {0};

	S_RFSAPR_1553B_TEST_STS	s1553BSelfTstSts;

	memset(&s1553BSelfTstSts ,0 ,sizeof(S_RFSAPR_1553B_TEST_STS));

	pMainApp->sktConnectSocket = pMainApp->g_RFSAPR_Eth_Config.uiConnectedSocket;

	if(pMainApp->g_RFSAPR_Eth_Config.bISConfigured == false) //Socket Connection
	{
		GetErrorMsg(DP_RFSAPR_APP_NO_COMM_LNK, arrcData);
		pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
		AfxMessageBox(arrcData,MB_ICONERROR);
		return;
	}

	//Set Group Id and Unit Id for Version
	pMainApp->SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0;
	pMainApp->SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId =  RFSAPR_CMD_SET_RFSAPR_SYSTEM_TIME;
#if 0
	time_t UTC_Time;
	time(&UTC_Time);
	memcpy(&pMainApp->SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SetRFSAPR_System_Time.GmtTime, &UTC_Time, sizeof(UTC_Time));

	ulTxMessageSize = sizeof(S_DP_SET_RFSAPR_SYSTEM_TIME);	

#ifdef ENDIAN_SWAP			
	sRetVal = EndianSwap(&pMainApp->SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_SetRFSAPR_System_Time);
	if(sRetVal)
	{
		GetErrorMsg(sRetVal, arrcData);
		pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
		AfxMessageBox(arrcData,MB_ICONERROR);
		return;
	}
#endif
	
	sRetVal = pMainApp->objTCPSocComm->RFSAPR_TCPCmdTrans(pMainApp->sktConnectSocket, &pMainApp->SCmdFromRFSAPRAtp, ulTxMessageSize, &pMainApp->SResToRFSAPRAtp, &ulRxMessageSize, DP_RFSAPR_CMD_DEFAULT_TIMEOUT_MS);
	if(sRetVal == RFSAPR_ATE_SUCCESS)
	{
		if((pMainApp->SResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->SResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId == RFSAPR_RES_SET_RFSAPR_SYSTEM_TIME))
		{
			pMainApp->PrintLog("RFSAPR (TYPE - 1) system date/time configuration success", DP_RFSAPR_PRINTLOG_SUCCESS);
		}
		else
		{
			pMainApp->PrintLog("Invalid Group/Unit ID of Response", DP_RFSAPR_PRINTLOG_FAILURES);
			pMainApp->PrintLog("RFSAPR (TYPE - 1) system date/time configuration failure", DP_RFSAPR_PRINTLOG_FAILURES);
			return ;					
		}
	}
	else
	{
		GetErrorMsg(sRetVal, arrcData);
		pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
		pMainApp->PrintLog("RFSAPR system date/time configuration failure", DP_RFSAPR_PRINTLOG_FAILURES);
		return ;
	}	


#endif

/** For Getting the PBIT Status  */
	
	/*Set Group Id and Unit Id for Version*/
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0;
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId =  RFSAPR_CMD_PBIT;	

	ulTxMessageSize = 0;	

	sRetVal = pMainApp->objTCPSocComm->RFSAPR_TCPCmdTrans(pMainApp->sktConnectSocket, &pMainApp->m_SCmdFromRFSAPRAtp, ulTxMessageSize, &pMainApp->SResToRFSAPRAtp, &ulRxMessageSize, DP_RFSAPR_CMD_MAX_TIMEOUT);
	if(sRetVal == RFSAPR_ATE_SUCCESS)
	{	
		if((pMainApp->SResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->SResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId == RFSAPR_RES_PBIT))
		{
#ifdef ENDIAN_SWAP			
		sRetVal = EndianSwap(&pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status);
		if(sRetVal)
		{
			GetErrorMsg(sRetVal, arrcData);
			pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
			AfxMessageBox(arrcData,MB_ICONERROR);		
		}
			
#endif				
			pMainApp->g_bPBITStatus = 1;
			pMainApp->PrintLog("PBIT SUCCESS",DP_RFSAPR_PRINTLOG_SUCCESS);			
		}
		else
		{
			pMainApp->g_bPBITStatus = 0;
			pMainApp->PrintLog("ERROR : Invalid Group/Unit ID for RFSAPR_RES_PBIT", DP_RFSAPR_PRINTLOG_FAILURES);
			return ;
			
		}
	}
	else
	{
		pMainApp->g_bPBITStatus = 0;
		GetErrorMsg(sRetVal, arrcData);
		AfxMessageBox(arrcData,MB_ICONERROR);
		return;
	}


	/* For updating the XTP report control */
	UpdateGUI(); 


	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_1553B_Arinc.s1553BSelfTstSts.m_u8LpbkBusAPass && pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_1553B_Arinc.s1553BSelfTstSts.m_u8LpbkBusBPass && pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_1553B_Arinc.s1553BSelfTstSts.m_u8RAMTestPass )
	{
		pMainApp->g_b1553BselfTest = 1;		
	}
	else
	{
		pMainApp->g_b1553BselfTest = 0;
	}


	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_1553B_Arinc.m_u8Arincselftest == 1 )
	{
		pMainApp->g_bArincSelfTest = 1;		
	}
	else
	{
		pMainApp->g_bArincSelfTest = 0;
	}

/*** End **/
	

/*** This command is for detecting the board status present in the target system*/

	/* Assign Group ID and Unit ID */
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0; 
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId  = RFSAPR_CMD_READ_BOARD_DET_STATUS;
	
#ifdef ENDIAN_SWAP			
	sRetVal = EndianSwap(&pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_Read_Brd_Sts);
	if(sRetVal)
	{
		GetErrorMsg(sRetVal, arrcData);
		pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
		AfxMessageBox(arrcData,MB_ICONERROR);
		return;
	}
#endif

	//Send Ethernet Command for ADC Data from RFSAPR target
	sRetVal = pMainApp->objTCPSocComm->RFSAPR_TCPCmdTrans(pMainApp->sktConnectSocket,&pMainApp->m_SCmdFromRFSAPRAtp,sizeof(S_CMD_BOARD_DET_STATUS),&pMainApp->m_SResToRFSAPRAtp,&ulRxMessageSize,DP_RFSAPR_CMD_DEFAULT_TIMEOUT_MS);
	if(sRetVal == RFSAPR_ATE_SUCCESS)
	{
		if((pMainApp->m_SResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->m_SResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId = RFSAPR_RES_READ_BOARD_DET_STATUS))
		{													
#ifdef ENDIAN_SWAP			
			sRetVal = EndianSwap(&pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts);
			if(sRetVal)
			{
				GetErrorMsg(sRetVal,arrcData);
				pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
				AfxMessageBox(arrcData,MB_ICONERROR);		
			}
#endif	
			pMainApp->g_bBrdDetctnSts = 1;
		}
		else
		{
			pMainApp->g_bBrdDetctnSts = 0;
			strActLog.Format("Invalid Unit ID or Group ID");
			pMainApp->PrintLog(strActLog,1);
		}
	}			
	else
	{
		pMainApp->g_bBrdDetctnSts = 0;
		strActLog.Format("Unable to read the Board detection Status from target");
		AfxMessageBox(strActLog,MB_ICONERROR);
		pMainApp->PrintLog(strActLog,DP_RFSAPR_PRINTLOG_FAILURES);
	}

	pMainApp->sGlobalInfo.sTrgtBrdSts.uc1013Sts = pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts.usNBDRxBrdSts;
	pMainApp->sGlobalInfo.sTrgtBrdSts.uc5028Sts = pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts.usXMC5028Sts;

	if(pMainApp->sGlobalInfo.sTrgtBrdSts.uc5028Sts > 0)
	{
		pMainApp->g_b5028BrdDetcSts = 1;
		m_5028Brd_Sts.SetLedState(LED_SUCCESS);
	}
	else
	{
		pMainApp->g_b5028BrdDetcSts = 0;
		m_5028Brd_Sts.SetLedState(LED_FAILURE);
	}


	if(pMainApp->sGlobalInfo.sTrgtBrdSts.uc1013Sts == 1)
	{
		pMainApp->g_b1013BrdSts = 1;
		m_1013_Brd_Sts.SetLedState(LED_SUCCESS);
	}
	else
	{
		pMainApp->g_b1013BrdSts = 0;
		m_1013_Brd_Sts.SetLedState(LED_FAILURE);
	}

//	uc1502Brds = pMainApp->sGlobalInfo.sTrgtBrdSts.uc1502Sts;
	uc1502Brds = pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts.usWBDRxBrdSts[0] || pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts.usWBDRxBrdSts[1];
	if(uc1502Brds == 0)
	{
		pMainApp->g_b1502Brd1Sts = 0;
		pMainApp->g_b1502Brd2Sts = 0;
		m_1502Brd1_Sts.SetLedState(LED_FAILURE);
		m_1502Brd2_Sts.SetLedState(LED_FAILURE);
	}
	else
	{
		if(pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts.usWBDRxBrdSts[0])
		{
				m_1502Brd1_Sts.SetLedState(LED_SUCCESS);
				pMainApp->g_b1502Brd1Sts = 1;
		}	

		if(pMainApp->m_SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Read_Brd_Sts.usWBDRxBrdSts[1])
		{
				m_1502Brd2_Sts.SetLedState(LED_SUCCESS);
				pMainApp->g_b1502Brd2Sts = 1;
		}	
	}
	/*Ethenet connection Status*/
	if(pMainApp->g_RFSAPR_Eth_Config.bISConfigured == true )
	{
		m_EthConnSts.SetLedState(LED_SUCCESS);
	}
	else
	{
		m_EthConnSts.SetLedState(LED_FAILURE);
	}

}

void CConfigHost::EnaDisControls(bool in_validate)
{

	if(in_validate == true)
	{
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(false);
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(true);
		GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(false);
	}

}



void CConfigHost::LED_ControlsInit()
{

	MAINAPP(pMainApp);

	m_EthConnSts.SetLedStatesNumber(MAX_LED_STATES);
	m_EthConnSts.SetIcon(LED_NONE,IDI_ICON_NONE_LED_NEW);
	m_EthConnSts.SetIcon(LED_SUCCESS,IDI_ICON_GREEN_LED_NEW);
	m_EthConnSts.SetIcon(LED_FAILURE,IDI_ICON_RED_LED_NEW);
	m_EthConnSts.SetLedState(LED_NONE);

	m_5028Brd_Sts.SetLedStatesNumber(MAX_LED_STATES);
	m_5028Brd_Sts.SetIcon(LED_NONE,IDI_ICON_NONE_LED_NEW);
	m_5028Brd_Sts.SetIcon(LED_SUCCESS,IDI_ICON_GREEN_LED_NEW);
	m_5028Brd_Sts.SetIcon(LED_FAILURE,IDI_ICON_RED_LED_NEW);
	m_5028Brd_Sts.SetLedState(LED_NONE);

	m_1013_Brd_Sts.SetLedStatesNumber(MAX_LED_STATES);
	m_1013_Brd_Sts.SetIcon(LED_NONE,IDI_ICON_NONE_LED_NEW);
	m_1013_Brd_Sts.SetIcon(LED_SUCCESS,IDI_ICON_GREEN_LED_NEW);
	m_1013_Brd_Sts.SetIcon(LED_FAILURE,IDI_ICON_RED_LED_NEW);
	m_1013_Brd_Sts.SetLedState(LED_NONE);

	m_1502Brd1_Sts.SetLedStatesNumber(MAX_LED_STATES);
	m_1502Brd1_Sts.SetIcon(LED_NONE,IDI_ICON_NONE_LED_NEW);
	m_1502Brd1_Sts.SetIcon(LED_SUCCESS,IDI_ICON_GREEN_LED_NEW);
	m_1502Brd1_Sts.SetIcon(LED_FAILURE,IDI_ICON_RED_LED_NEW);
	m_1502Brd1_Sts.SetLedState(LED_NONE);

	m_1502Brd2_Sts.SetLedStatesNumber(MAX_LED_STATES);
	m_1502Brd2_Sts.SetIcon(LED_NONE,IDI_ICON_NONE_LED_NEW);
	m_1502Brd2_Sts.SetIcon(LED_SUCCESS,IDI_ICON_GREEN_LED_NEW);
	m_1502Brd2_Sts.SetIcon(LED_FAILURE,IDI_ICON_RED_LED_NEW);
	m_1502Brd2_Sts.SetLedState(LED_NONE);

}

HBRUSH CConfigHost::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STC_TITLE)
   {
		pDC->SetTextColor(RGB(226, 33, 188));
	    pDC->SetBkMode(TRANSPARENT);
   }

	// TODO: Return a different brush if the default is not desired
	return hbr;
}



void CConfigHost::EnbDisControls(bool in_bflag)
{	

	GetDlgItem(IDC_EDT_PORT_NO)->EnableWindow(in_bflag);
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(in_bflag);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(in_bflag);
	GetDlgItem(IDC_RDO_ETHERNET_SEL)->EnableWindow(in_bflag);
	GetDlgItem(IDC_RDO_SECOND_ETH)->EnableWindow(in_bflag);	
}


void CConfigHost::OnReportValueChanged(NMHDR * pNotifyStruct, LRESULT * result)
{

	UpdateData(true);
	CXTPReportRecords* pRecs = NULL;
	CXTPReportRecord*  pRec  = NULL;
	CString csCaption = "";
	USHORT usCycleCnt = 0;
	
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
	
	if (!pItemNotify->pRow || !pItemNotify->pColumn)
	{
		return;
	}

	
}


void CConfigHost::UpdateGUI()
{
	
	MAINAPP(pMainApp);


	unsigned int	uiTotRec		= 0;
	unsigned char	ucTestCaseCnt	= 0;
	unsigned char   *puTestSts		= 0;

	CXTPReportRecords* pRecs	= NULL;
	CXTPReportRecord*  pRec		= NULL;
	
	CXTPReportRecordItemText* pItem = NULL;
	uiTotRec	= m_listPBITSts.GetRows()->GetCount();
	pRecs		= m_listPBITSts.GetRecords();

	puTestSts = (PUCHAR)malloc(sizeof(UCHAR) * uiTotRec);

	memset(puTestSts , 0, sizeof(UCHAR) * uiTotRec);


	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_1553B_Arinc.s1553BSelfTstSts.m_u8LpbkBusAPass && pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_1553B_Arinc.s1553BSelfTstSts.m_u8LpbkBusBPass && pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_1553B_Arinc.s1553BSelfTstSts.m_u8RAMTestPass )
	{
		puTestSts[0] = 1;
		pMainApp->g_PBITTestSts[0] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[0] = 0;
	}

	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_1553B_Arinc.m_u8Arincselftest == 1)
	{
		puTestSts[1] = 1;		
		pMainApp->g_PBITTestSts[1] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[1] = 0;
	}

	/* Processor test status */
	if((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucProcsALUTest & 1) &&  ((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucProcsALUTest >> 1) & 1) && \
		((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucProcsALUTest >> 2) & 1))
	{
		puTestSts[2] = 1;
		pMainApp->g_PBITTestSts[2] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[2] = 0;
	}

	/* MRAM test status */
	if((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucMRAMTest & 1) &&  ((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucMRAMTest >> 1) & 1))
	{
		puTestSts[3] = 1;
		pMainApp->g_PBITTestSts[3] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[3] = 0;
	}

	/* SDRAM test status */
	if((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucSDRAMTest & 1) &&  ((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucSDRAMTest >> 1) & 1) && \
		((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucSDRAMTest >> 2) & 1))
	{
		puTestSts[4] = 1;
		pMainApp->g_PBITTestSts[4] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[4] = 0;
	}

	/* Processor Flash Test test status */
	if((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucFlashTest & 1) &&  ((pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucFlashTest >> 1) & 1))
	{
		puTestSts[5] = 1;
		pMainApp->g_PBITTestSts[5] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[5] = 0;
	}

	/* UART Test */
	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucUARTTest == 7)
	{
		puTestSts[6] = 1;
		pMainApp->g_PBITTestSts[6] = 1;
	}
	else
	{	
		pMainApp->g_PBITTestSts[6] = 0;
	}

	/* Processor USB test */
	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucUSBDOMTest == 1)
	{
		puTestSts[7] = 1;
		pMainApp->g_PBITTestSts[7] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[7] = 0;
	}

	/* Processor Sata Test */
	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.S_Processor_Test.m_ucSATADOMTest == 1)
	{
		puTestSts[8] = 1;
		pMainApp->g_PBITTestSts[8] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[8] = 0;
	}


	  /*Added for the FPGA scratch pad test */
	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.m_ucWBScratchPadSts[0] == 1)
	{
		puTestSts[9] = 1;
		pMainApp->g_PBITTestSts[9] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[9] = 0;
	} 

	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.m_ucWBScratchPadSts[1] == 1)
	{
		puTestSts[10] = 1;
		pMainApp->g_PBITTestSts[10] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[10] = 0;
	} 

	if(pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_PBIT_Status.m_ucNBScratchPadSts == 1)
	{
		puTestSts[11] = 1;
		pMainApp->g_PBITTestSts[11] = 1;
	}
	else
	{
		pMainApp->g_PBITTestSts[11] = 0;
	} 



	/* Update XTP report */
	
	//check for invalid cycle number
	for(ucTestCaseCnt = 0;ucTestCaseCnt < uiTotRec; ucTestCaseCnt++)
	{
		pRec = pRecs->GetAt(ucTestCaseCnt);
//		 =  pRec->GetItem(1)->GetCaption(m_listPBITSts.GetColumns()->GetAt(2));
		
		pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_listPBITSts.GetRecords()->GetAt(ucTestCaseCnt)->GetItem(2));
		
		if(puTestSts[ucTestCaseCnt])
		{
			pItem->SetCaption("PASS");
			pItem->SetBackgroundColor(RGB(0,128,0));
		}
		else
		{
			pItem->SetCaption("FAIL");
			pItem->SetBackgroundColor(RGB(255,0,0));
		}

		pItem = NULL;
		
	}

	m_listPBITSts.Populate();

	if(puTestSts)
	{
		free(puTestSts);
		puTestSts = NULL;
	}

	return;

}


void CConfigHost::OnDestroy() 
{
	CFormView::OnDestroy();
	
	GetDlgItem(IDC_RDO_ETHERNET_SEL)->SetFocus();
	
}

void CConfigHost::OnChkFanOnoff() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);

	if(m_ChkfanOnOff)
	{
		GetDlgItem(IDC_BTN_FAN_ON_OFF)->SetWindowText("ON");
	}	
	else
	{
		GetDlgItem(IDC_BTN_FAN_ON_OFF)->SetWindowText("OFF");
	}
	
}

void CConfigHost::OnBtnFanOnOff() 
{
	// TODO: Add your control notification handler code here

	MAINAPP(pMainApp);

	short sRetVal = 0;
	int iretVal = 0;
	CString	strActLog		= "";
	UCHAR	ucFanSel		= 0;
	ULONG	ulRxMessageSize = 0;


	UpdateData(true);

	
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0; 
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId  = RFSAPR_CMD_SYSTEM_FAN_ON_OFF_CTRL;

	
	ucFanSel = (((m_Chkfan2 & 0x1) << 1) | (m_Chkfan1));

	if(ucFanSel == 0 )
	{
		AfxMessageBox("Select atleast one fan");
		return;
	}


	pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.m_SCmdFanOnOffCtrl.m_ucFanSel = ucFanSel; 
	pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.m_SCmdFanOnOffCtrl.m_ucFanOnOffsel = !m_ChkfanOnOff; 

		
#ifndef _8333_DEBUG_
	
	sRetVal = pMainApp->objTCPSocComm->RFSAPR_TCPCmdTrans(pMainApp->sktConnectSocket,&pMainApp->m_SCmdFromRFSAPRAtp,sizeof(S_RFSAPR_CMD_FAN_ON_OFF_CONTROL),&pMainApp->m_SResToRFSAPRAtp,&ulRxMessageSize,1000);
	
	if(sRetVal == RFSAPR_SUCCESS)
	{
		if((pMainApp->m_SResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->m_SResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId = RFSAPR_RES_SYSTEM_FAN_ON_OFF_CTRL))
		{
			strActLog.Format("System FAN On/Off Success");
			pMainApp->PrintLog(strActLog,0);
			
		}
		else
		{
			strActLog.Format("System FAN On/Off Success");
			pMainApp->PrintLog(strActLog,1);
		}
	}			
	else
	{
		strActLog.Format("Unable to Control System FAN On/Off");  
		AfxMessageBox(strActLog,MB_ICONERROR);
		pMainApp->PrintLog(strActLog,1);
		return ;
	}
	
#endif
	
	UpdateData(true);
	
}

void CConfigHost::OnBtnSysPwron() 
{
	short sRetVal		= 0;
	CString StrErrMsg	= "";
	CString strActLog	= "";
	MAINAPP(pMainApp)
	GetDlgItem(IDC_BTN_SYS_PWRON)->EnableWindow(false);
	GetDlgItem(IDC_BTN_SYS_PWROFF)->EnableWindow(true);
	UpdateData(false);
	/*Remote POwer ON the RFSAPR SYSTEm */
	sRetVal = pMainApp->obj3096Wrapper.SetSWStrobe(1 /*3096 Board Number*/, 2 /*Group Number*/);
	if(sRetVal)
	{
		pMainApp->obj3096Wrapper.GetErrMsg(sRetVal, StrErrMsg);
		AfxMessageBox(StrErrMsg);
		strActLog.Format("Unable to remote power ON the RFSAPR TYPE-1 UNIT");
		pMainApp->PrintLog(strActLog,2);
		return;
	}
	
}

void CConfigHost::OnBtnSysPwroff() 
{
	short sRrtVal = 0;
		GetDlgItem(IDC_BTN_SYS_PWRON)->EnableWindow(true);
	GetDlgItem(IDC_BTN_SYS_PWROFF)->EnableWindow(false);;
	/*TBD with Shanmugham sir*/
	
	
}



short CConfigHost::Read_and_apply_TempCalibData(unsigned char in_ucAntloop,unsigned char in_ucTempflag)
{
	short sRetVal = 0;	
	MAINAPP(pMainApp);
	unsigned long ulTxMessageSize = 0, ulRxMessageSize = 0;
	char arrcData[MAX_ERR_STRING_LEN] = {0};
	unsigned char ucSuccessflag = 0;

	S_RFSAPR_HW_TEMP_CALIB_DATA		sRFSAPRTempCalibData;
	S_RFSAPR_RF_GAIN_CALIB_TABLE	sRFSAPRGainTable;

	memset(&sRFSAPRTempCalibData , 0 ,sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA));
	memset(&sRFSAPRGainTable , 0 ,sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE));


	if(pMainApp->g_RFSAPR_Eth_Config.bISConfigured == false) //Socket Connection
	{
		GetErrorMsg(DP_RFSAPR_APP_NO_COMM_LNK, arrcData);
		pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
		AfxMessageBox(arrcData,MB_ICONERROR);
		return DP_RFSAPR_FAILURE;
	}

	pMainApp->sktConnectSocket = pMainApp->g_RFSAPR_Eth_Config.uiConnectedSocket;

	/*Set Group Id and Unit Id for Version*/
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0;
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId  =  RFSAPR_CMD_READ_WBRF_TEMP_CALIB;

	pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.m_S_Cmd_ReadWBRFCalib.m_ucAntNum = in_ucAntloop;

	ulTxMessageSize = sizeof(S_RFSAPR_CMD_READ_WBRF_TEMP_CALIB_DATA);

	sRetVal = pMainApp->objTCPSocComm->RFSAPR_TCPCmdTrans(pMainApp->sktConnectSocket, &pMainApp->m_SCmdFromRFSAPRAtp, ulTxMessageSize, &pMainApp->SResToRFSAPRAtp, &ulRxMessageSize, DP_RFSAPR_CMD_DEFAULT_TIMEOUT_MS);
	if(sRetVal == DP_RFSAPR_SUCCESS)
	{
		if((pMainApp->SResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->SResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId == RFSAPR_RES_READ_WBRF_TEMP_CALIB))
		{
#ifdef ENDIAN_SWAP	
			sRetVal = EndianSwap(&pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.m_S_Res_ReadWBRFCalib);
			if(sRetVal)
			{
				GetErrorMsg(sRetVal, arrcData);
				pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
				return DP_RFSAPR_FAILURE;
			}
#endif
			ucSuccessflag = 1;	
			
			/*memcpy(&pMainApp->g_SRFSAPRHWTempCalibTable, &pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.m_S_Res_ReadWBRFCalib, sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA));*/
			memcpy(&sRFSAPRTempCalibData, &pMainApp->SResToRFSAPRAtp.m_UResRFSAPRMessage.m_S_Res_ReadWBRFCalib, sizeof(S_RFSAPR_HW_TEMP_CALIB_DATA));
			
		}
		else
		{
			pMainApp->PrintLog("Invalid Ver. Group/Unit ID of Resp", DP_RFSAPR_PRINTLOG_FAILURES);
			return DP_RFSAPR_FAILURE;
		}
	}
	else
	{
		pMainApp->PrintLog("RF Frontend Temperature based calibration constants read failure", DP_RFSAPR_PRINTLOG_FAILURES);
		return DP_RFSAPR_FAILURE;
	}

	if(ucSuccessflag)
	{

		memcpy(&sRFSAPRGainTable,&sRFSAPRTempCalibData.m_sRFSAPRcalibdata[in_ucTempflag], sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE)); 

		sRFSAPRGainTable.ulBoardNumber = in_ucAntloop;
			
		sRetVal = Apply_Attenuation(sRFSAPRGainTable);
		if(sRetVal)
		{
			pMainApp->PrintLog("Failed to Apply Attenuation", DP_RFSAPR_PRINTLOG_FAILURES);
			return DP_RFSAPR_FAILURE;
		}
		else
		{
		//	pMainApp->PrintLog("Successfuly Applied Attenuation", DP_RFSAPR_PRINTLOG_SUCCESS);
		}
	}

	return sRetVal;
}


short CConfigHost::Apply_Attenuation(S_RFSAPR_RF_GAIN_CALIB_TABLE in_sRFSAPRGainTable)
{
	MAINAPP(pMainApp);
	short sRetVal = 0;
	unsigned long ulTxMessageSize = 0, ulRxMessageSize = 0;
	char arrcData[MAX_ERR_STRING_LEN] = {0};

	if(pMainApp->g_RFSAPR_Eth_Config.bISConfigured == false) //Socket Connection
	{
		GetErrorMsg(DP_RFSAPR_APP_NO_COMM_LNK, arrcData);
		pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
		AfxMessageBox(arrcData,MB_ICONERROR);
		return DP_RFSAPR_FAILURE;
	}

	pMainApp->sktConnectSocket = pMainApp->g_RFSAPR_Eth_Config.uiConnectedSocket;

	/*Set Group Id and Unit Id for Version*/
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0;
	pMainApp->m_SCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId  = RFSAPR_CMD_UPDATE_RF_FRONTEND_CALIB;

	memcpy(&pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_RFSAPRGainTable, &in_sRFSAPRGainTable, sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE));

#ifdef ENDIAN_SWAP			
	sRetVal = EndianSwap(&pMainApp->m_SCmdFromRFSAPRAtp.m_UCmdRFSAPRMessage.S_Cmd_RFSAPRGainTable);
	if(sRetVal)
	{
		GetErrorMsg(sRetVal, arrcData);
		pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
		return sRetVal;
	}
#endif

	ulTxMessageSize = sizeof(S_RFSAPR_RF_GAIN_CALIB_TABLE);

	sRetVal = pMainApp->objTCPSocComm->RFSAPR_TCPCmdTrans(pMainApp->sktConnectSocket, &pMainApp->m_SCmdFromRFSAPRAtp, ulTxMessageSize, &pMainApp->SResToRFSAPRAtp, &ulRxMessageSize, DP_RFSAPR_CMD_ADC_CALIB_TIMEOUT_MS);
	if(sRetVal == DP_RFSAPR_SUCCESS)
	{
		if((pMainApp->SResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->SResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId == RFSAPR_RES_UPDATE_RF_FRONTEND_CALIB))
		{					
			/*memcpy(&pMainApp->g_SRFSAPRGainTable, in_pUploadRFGainValue, sizeof(pMainApp->g_SRFSAPRGainTable));*/
		}
		else
		{
			pMainApp->PrintLog("Invalid Ver. Group/Unit ID of Resp", DP_RFSAPR_PRINTLOG_FAILURES);		
			return DP_RFSAPR_FAILURE;
		}
	}
	else
	{
		return DP_RFSAPR_FAILURE;
	}

	return sRetVal;
}



void CConfigHost::OnFieldchangedIpaddTarget2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
