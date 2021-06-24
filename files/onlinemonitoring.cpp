// onlinemonitoring.cpp : implementation file
//

#include "stdafx.h"
#include "dp_ate_8333_300.h"
#include "onlinemonitoring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int COnlineMonitoring::m_iFile = 0;

/////////////////////////////////////////////////////////////////////////////
// COnlineMonitoring

IMPLEMENT_DYNCREATE(COnlineMonitoring, CFormView)

COnlineMonitoring::COnlineMonitoring()
	: CFormView(COnlineMonitoring::IDD)
{
	//{{AFX_DATA_INIT(COnlineMonitoring)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COnlineMonitoring::~COnlineMonitoring()
{			
	if(m_fpDataFile)
	fclose(m_fpDataFile);
}

void COnlineMonitoring::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COnlineMonitoring)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CUST_VI_MON, m_LstCtlSBCSts);
}


BEGIN_MESSAGE_MAP(COnlineMonitoring, CFormView)
	//{{AFX_MSG_MAP(COnlineMonitoring)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_USER_UPDATE_ONLINE_STS,UpdateOnlineSts)
	ON_MESSAGE(WM_UPDATE_ONLINEMON,StartOnlineMonThread)	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnlineMonitoring diagnostics

#ifdef _DEBUG
void COnlineMonitoring::AssertValid() const
{
	CFormView::AssertValid();
}

void COnlineMonitoring::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COnlineMonitoring message handlers

void COnlineMonitoring::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	MAINAPP(pMainApp);

	SYSTEMTIME sSysCurrTimeDate;

	GetLocalTime(&sSysCurrTimeDate);

	pMainApp->m_pobjOnlineMon = this;
	
	CString strTemp = "";
	CString strFilePath = "";
	CXTPReportRecord* pRecord = NULL;
	CXTPReportColumn* pColumn = NULL;//GetColumns()->GetVisibleAt(1);
	CXTPReportRecordItemText* pItem = NULL;

	strFilePath.Format("%sOnlineStatus\\OnlineTempStatus_%d-%d-%d_%d_%d_%d.csv ",pMainApp->sGlobalInfo.sFilePath.m_strOutputFilePath,sSysCurrTimeDate.wDay,sSysCurrTimeDate.wMonth, sSysCurrTimeDate.wYear, sSysCurrTimeDate.wHour, sSysCurrTimeDate.wMinute, sSysCurrTimeDate.wSecond);
	m_fpDataFile = fopen(strFilePath,"w");
	if(m_fpDataFile)
	{
		fprintf(m_fpDataFile, "Date & Time,Processor-Local(°C),CPU-Die(°C),ETI(HHHH:MM:SS),Event Count,SBC Ethernet-1,SBC Ethernet-2,WB DRx1 FPGA-Local(°C),WB DRx1 FPGA-Die(°C),WB DRx1 ADC-Local(°C),WB DRx1 ADC-Die_1(°C),WB DRx1 ADC-Die_2(°C),WB DRx1 ADC-Die_3(°C),WB DRx1 ADC-Die_4(°C),WB DRx2 FPGA-Local(°C),WB DRx2 FPGA-Die(°C),WB DRx2 ADC-Local(°C),WB DRx2 ADC-Die_1(°C),WB DRx2 ADC-Die_2(°C),WB DRx2 ADC-Die_3(°C),WB DRx2 ADC-Die_4(°C),NB DRx FPGA-Local(°C),NB DRx FPGA-Die(°C),NB DRx ADC-Local(°C),NB DRx ADC-Die_1(°C),VX1,VX2,VX3,VX4,VX5,VP1,VP2,VP3,VP4,VH,NBDS(°C),NBDC-1(°C),NBDC-2(°C),FAN-1(rpm),FAN-2(rpm)\n");
	}


	// AddColumn(ColumnNo, Parameter name, CellWidth, AutoSizeEnable, ColumnIcon, SortOption, VisibleEnable);
	m_LstCtlSBCSts.AddColumn(new CXTPReportColumn(0, _T("Parameters"),115, TRUE, XTP_REPORT_NOICON, FALSE, TRUE));
	m_LstCtlSBCSts.AddColumn(new CXTPReportColumn(1, _T("Readings"),190, TRUE, XTP_REPORT_NOICON, FALSE, TRUE));	
	m_LstCtlSBCSts.GetPaintManager()->SetGridStyle(TRUE, xtpReportGridSolid);

	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("Processor - Local (°C)", "-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("CPU - Die (°C)", "-") );		
//	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("ETI(HHHH:MM:SS)","-"));
	//m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("Event Count","-"));
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("WB DRx1 FPGA-Local (°C)","-"));	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("WB DRx1 FPGA-Die (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("WB DRx1 ADC-Local (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("WB DRx1 ADC-Die (°C)","-") );	

	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("WB DRx2 FPGA-Local (°C)","-"));	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("WB DRx2 FPGA-Die (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("WB DRx2 ADC-Local (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("WB DRx2 ADC-Die (°C)","-") );	

	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("NB DRx  FPGA-Local (°C)","-"));	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("NB DRx  FPGA-Die (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("NB DRx  ADC-Local (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("NB DRx  ADC-Die (°C)","-") );	

	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("PSU Local(°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("PSU Remote (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("NBDS (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("NBDC-1 (°C)","-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("NBDC-2 (°C)","-") );
	
	/* Newly Added ***/
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("FAN Speed(rpm)", "-") );	
	m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("IO Matrix (°C)","-") );
	/*m_LstCtlSBCSts.AddRecord(new CMessageRecordEx("FAN-2 Speed(rpm)", "-") );	*/
	/**************/


	/****************/


	m_LstCtlSBCSts.SelectionEnable(false);
	m_LstCtlSBCSts.Populate();

	Sleep(100);
/*
	strTemp.Format(" Link Down");
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(4)->GetItem(1));
	pItem->SetValue(strTemp);

	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(5)->GetItem(1));
	pItem->SetValue(strTemp);
*/
	UpdateData(false);

	/*AfxBeginThread(OnlineStsRead, this, THREAD_PRIORITY_NORMAL, 0, 0);*/
}


void COnlineMonitoring::StartOnlineMonThread(WPARAM wParam, LPARAM lParam)
{
	MAINAPP(pMainApp);
	if(pMainApp->g_RFSAPR_OnlineEth_Config.bISConfigured == true)
	{
		AfxBeginThread(OnlineStsRead, this, THREAD_PRIORITY_NORMAL, 0, 0);
	}
	return;
}

void COnlineMonitoring::UpdateOnlineSts(WPARAM wParam, LPARAM lParam)
{
	MAINAPP(pMainApp);
	CString strTemp		  = "";
	CString strElsapTime  = "";	
	CString strEth1Sts    = "";	
	CString strEth2Sts	  = "";	
	CString strFilePath   = "";
	CString strDatetym    = "";
	int iHrs = 0;
	int iMns = 0;
	int iSec = 0;
	int iIdx = 0;
	ULONG ulFan1SpeedRPM = 0;
	ULONG ulFan2SpeedRPM = 0;
	static int iFile =0 ;

	SYSTEMTIME	Systym;
	CString strIP1 = "";
	CString strIP2 = "";
	S_RFSAPR_TEMPERATURE_MEASUREMENTS	sRFSAPRLocal;
/*	S_RFSAPR_RES_READ_NB_VOLTAGE	s_RFSAPRNBVol;	*/

	memcpy(&sRFSAPRLocal,&pMainApp->SOnlineResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Temperature_Sensor,sizeof(S_RFSAPR_TEMPERATURE_MEASUREMENTS));		
	
	CXTPReportRecord* pRecord = NULL;
	CXTPReportColumn* pColumn = NULL;//GetColumns()->GetVisibleAt(1);
	CXTPReportRecordItemText* pItem = NULL;
	
	/**Updation for the Processor temperature **/
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	strTemp.Format("%0.1f",sRFSAPRLocal.fProcessorLocalTemperature );
	pItem->SetValue(strTemp);
	
	/**Updation for the CPU Die temperature **/
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	strTemp.Format("%0.1f",sRFSAPRLocal.fCPU_DieTemperature );
	pItem->SetValue(strTemp);

	/*Elasped Date & Time*/
	if(sRFSAPRLocal.uiETIElapsedTime >= (60 * 60))
	{
		iHrs = sRFSAPRLocal.uiETIElapsedTime / (60 * 60);
		sRFSAPRLocal.uiETIElapsedTime -= (iHrs * (60 * 60));
	}
	if(sRFSAPRLocal.uiETIElapsedTime >= 60)
	{
		iMns  = sRFSAPRLocal.uiETIElapsedTime / 60;
		sRFSAPRLocal.uiETIElapsedTime -= (iMns * 60);
	}
	if(sRFSAPRLocal.uiETIElapsedTime < 60)
	{
		iSec = sRFSAPRLocal.uiETIElapsedTime;
	} 

	strElsapTime.Format("%02d:%02d:%02d",iHrs,iMns,iSec);
	
	/***********************Ethernet-1 Status Check***********************/
	CPing objPingTest1;
	CPingReply objPingReply1;

	strIP1.Format("%d.%d.%d.%d", pMainApp->g_RFSAPR_Eth_Config.ucAddr[0], pMainApp->g_RFSAPR_Eth_Config.ucAddr[1], pMainApp->g_RFSAPR_Eth_Config.ucAddr[2], pMainApp->g_RFSAPR_Eth_Config.ucAddr[3]);

	/*Ethernet Connection Check*/
	for(UCHAR ucCheckCnt1 = 0; ucCheckCnt1 < 2;ucCheckCnt1++)
	{

		if(objPingTest1.Ping(strIP1, objPingReply1, 128, 5000, 32))
		{
			pMainApp->m_usEthSts1 = 0;
		}
		else
		{
			/*To Monitor for Continuous three Times*/
			pMainApp->m_usEthSts1++;
		}
		
	}/*End of Connection Check*/

	/*Set status based on the Eternet Status Count*/
	/*pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));*/

	if(pMainApp->m_usEthSts1 >= 6)
	{
		strTemp.Format("Link Down" );
		strEth1Sts.Format("Link Down" );		
		/*pItem->SetBackgroundColor(RGB(255, 110, 110));*/
		
	}
	else
	{
		strTemp.Format("Link Up" );
		strEth1Sts.Format("Link Up" );
		/*pItem->SetBackgroundColor(RGB(175, 235, 175));*/
	}
	
	/*pItem->SetValue(strTemp);

	/*Ethernet Status 2*/

	CPing objPingTest2;
	CPingReply objPingReply2;

	strIP2.Format("%d.%d.%d.%d", pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[0], pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[1], pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[2], pMainApp->g_RFSAPR_OnlineEth_Config.ucAddr[3]);

	/*Ethernet Connection Check*/
	for(UCHAR ucCheckCnt2 = 0; ucCheckCnt2 < 2;ucCheckCnt2++)
	{

		if(objPingTest2.Ping(strIP2, objPingReply2, 128, 5000, 32))
		{
			pMainApp->m_usEthSts2 = 0;
		}
		else
		{
			/*To Monitor for Continuous three Times*/
			pMainApp->m_usEthSts2++;
		}
		
	}/*End of Connection Check*/

/*	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));*/
	/*Set status based on the Eternet Status Count*/
	if(pMainApp->m_usEthSts2 >= 6)
	{
		strTemp.Format("Link Down" );
		strEth2Sts.Format("Link Down" );
		/*pItem->SetBackgroundColor(RGB(255, 110, 110));*/
	}
	else
	{
		strTemp.Format("Link Up" );
		strEth2Sts.Format("Link Up" );
		/*pItem->SetBackgroundColor(RGB(175, 235, 175));*/
	}


	/*Updation for Ethernet -2 link status */
	/*pItem->SetValue(strTemp);*/

	/*****************************************************************************/
	/** Updation of the WB and NB Drx table **/
	
	for(UCHAR ucloop =0; ucloop < 2;ucloop++)
	{		
		/***Updation of WB FPGA Local temperature */
		pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
		
		strTemp.Format("%.2f",sRFSAPRLocal.m_DRX_Temperature[ucloop].fFPGA_Local_Temperature);
		
		pItem->SetValue(strTemp);
		
		/** Updation of the WB ADC Die temp **/
		pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
		
		strTemp.Format("%.1f",sRFSAPRLocal.m_DRX_Temperature[ucloop].fFPGA_Die_Temperature);

		
		pItem->SetValue(strTemp);
		
		/** Updation of the WB ADC Local temp **/
		pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
		
		strTemp.Format("%.1f",sRFSAPRLocal.m_DRX_Temperature[ucloop].fADC_LocalTemperature);

		
		pItem->SetValue(strTemp);
		
		/** Updation of the WB ADC-Die Channel-1 Temp **/
		pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText,m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
		
		strTemp.Format("CH1:%.1f, CH2:%.1f, CH3:%.1f, CH4:%.1f", sRFSAPRLocal.m_DRX_Temperature[ucloop].arr_fADC_Die_Temperature[0],\
			sRFSAPRLocal.m_DRX_Temperature[ucloop].arr_fADC_Die_Temperature[1],\
			sRFSAPRLocal.m_DRX_Temperature[ucloop].arr_fADC_Die_Temperature[2],\
			sRFSAPRLocal.m_DRX_Temperature[ucloop].arr_fADC_Die_Temperature[3]);

	
		pItem->SetValue(strTemp);
		
	}
	
	/***Updation of NB FPGA Local temperature */
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	
	strTemp.Format("%.1f",sRFSAPRLocal.m_NBDRxTemp.fFPGA_Local_Temperature);

	
	pItem->SetValue(strTemp);
	
	/** Updation of the NB ADC Die temp **/
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	
	strTemp.Format("%.1f",sRFSAPRLocal.m_NBDRxTemp.fFPGA_Die_Temperature);

	
	pItem->SetValue(strTemp);
	
	/** Updation of the NB ADC Local temp **/
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	
	strTemp.Format("%.1f",sRFSAPRLocal.m_NBDRxTemp.fADC_LocalTemperature);

	
	pItem->SetValue(strTemp);
	
	/** Updation of the NB ADC-Die Channel-1 Temp **/
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	
	strTemp.Format("CH1:%.1f", sRFSAPRLocal.m_NBDRxTemp.arr_fADC_Die_Temperature[0]);
	/*strTemp.Format("CH1:%.1f, CH2:%.1f", sRFSAPRLocal.m_NBDRxTemp.arr_fADC_Die_Temperature[0],\
		sRFSAPRLocal.m_NBDRxTemp.arr_fADC_Die_Temperature[1]);*/
	pItem->SetValue(strTemp);

	/* Added for PSU Temp */
	/*TODO*/
	/*Implement for Power Supply(PSU) Modules*/
	
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	
	strTemp.Format("%.2f",sRFSAPRLocal.m_PSUVoltage.m_flocalTemp);
		
	pItem->SetValue(strTemp);


	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	
	strTemp.Format("%.2f",sRFSAPRLocal.m_PSUVoltage.m_fRemoteTemp);
		
	pItem->SetValue(strTemp);

	ULONG ulReadTemp1 = 0;
	ULONG ulReadTemp2 = 0;

	/*Updating NBDS Temperature*/ 
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	strTemp.Format("%0.2f",sRFSAPRLocal.m_sNBRFModTemp.m_fNBDSTemp);
	double dNBFSTemp = atof(strTemp);
	pItem->SetValue(strTemp);

	ulReadTemp1 = 0;
	ulReadTemp2 = 0;

	/*Updating NBDC - 1 Temperature*/ 
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	strTemp.Format("%0.2f",sRFSAPRLocal.m_sNBRFModTemp.m_fNBBDC1Temp);
	double dNBDC1Temp = atof(strTemp);
	pItem->SetValue(strTemp);

	ulReadTemp1 = 0;
	ulReadTemp2 = 0;

	/*Updating NBDC - 2 Temperature*/ 
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	strTemp = "";
	strTemp.Format("%0.2f",sRFSAPRLocal.m_sNBRFModTemp.m_fNBBDC2Temp);
	double dNBDC2Temp = atof(strTemp);
	pItem->SetValue(strTemp);



	
	/* FAN-1 Speed */ 
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	
	/*** ON + OFF time = T 
		RPM = 60/2T; */

	/*Commented by srinivasu @ 08092020 Reason :: Divide by Zero: Execption*/
	
	if(sRFSAPRLocal.m_sReadFANOnOFFTime.m_ulFan2OnTime +  sRFSAPRLocal.m_sReadFANOnOFFTime.m_ulFan2OFFTime )
	{
		ulFan1SpeedRPM = ( 60.0 / (( 2.0 * (sRFSAPRLocal.m_sReadFANOnOFFTime.m_ulFan2OnTime +  sRFSAPRLocal.m_sReadFANOnOFFTime.m_ulFan2OFFTime)*0.33*(10e-6))));
	}

	strTemp.Format("%d",ulFan1SpeedRPM);

	pItem->SetValue(strTemp);

	/* FAN-2 Speed */ 

/*	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));

	/*** ON + OFF time = T 
		RPM = 60/2T; */


	/*ulFan2SpeedRPM = ( 60 / (( 2 * (sRFSAPRLocal.m_sReadFANOnOFFTime.m_ulFan1OnTime +  sRFSAPRLocal.m_sReadFANOnOFFTime.m_ulFan1OFFTime))));*/
	
	/*strTemp.Format("%d",ulFan2SpeedRPM);

	pItem->SetValue(strTemp);*/

	
	/*Updating IO Matrix Temperature*/ 
	pItem = DYNAMIC_DOWNCAST(CXTPReportRecordItemText, m_LstCtlSBCSts.GetRecords()->GetAt(iIdx++)->GetItem(1));
	strTemp = "";
	strTemp.Format("%0.2f",sRFSAPRLocal.fIOMatrixTemp);
	double dIOmatrixTemp = atof(strTemp);
	pItem->SetValue(strTemp);
	
	/** End ***/

	GetLocalTime(&Systym);

	strDatetym.Format("%d/%d/%d - %d:%d:%d",Systym.wDay,Systym.wMonth,Systym.wYear,Systym.wHour,Systym.wMinute,Systym.wSecond);

	if(m_fpDataFile)
	{/*
		fprintf(m_fpDataFile,"%s,%f,%f,%s,%d,%s,%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f\n",strDatetym,sRFSAPRLocal.fProcessorLocalTemperature,sRFSAPRLocal.fCPU_DieTemperature,strElsapTime,sRFSAPRLocal.uiETIEvtCnt,\
				strEth1Sts,strEth2Sts,sRFSAPRLocal.m_DRX_Temperature[0].fFPGA_Local_Temperature,sRFSAPRLocal.m_DRX_Temperature[0].fFPGA_Die_Temperature,sRFSAPRLocal.m_DRX_Temperature[0].fADC_LocalTemperature,sRFSAPRLocal.m_DRX_Temperature[0].arr_fADC_Die_Temperature[0],
				sRFSAPRLocal.m_DRX_Temperature[0].arr_fADC_Die_Temperature[1],sRFSAPRLocal.m_DRX_Temperature[0].arr_fADC_Die_Temperature[2],sRFSAPRLocal.m_DRX_Temperature[0].arr_fADC_Die_Temperature[3],
				sRFSAPRLocal.m_DRX_Temperature[1].fFPGA_Local_Temperature,sRFSAPRLocal.m_DRX_Temperature[1].fFPGA_Die_Temperature,sRFSAPRLocal.m_DRX_Temperature[1].fADC_LocalTemperature,sRFSAPRLocal.m_DRX_Temperature[1].arr_fADC_Die_Temperature[0],
				sRFSAPRLocal.m_DRX_Temperature[1].arr_fADC_Die_Temperature[1],sRFSAPRLocal.m_DRX_Temperature[1].arr_fADC_Die_Temperature[2],sRFSAPRLocal.m_DRX_Temperature[1].arr_fADC_Die_Temperature[3],sRFSAPRLocal.m_NBDRxTemp.fFPGA_Local_Temperature, 
				sRFSAPRLocal.m_NBDRxTemp.fFPGA_Die_Temperature ,sRFSAPRLocal.m_NBDRxTemp.fADC_LocalTemperature,sRFSAPRLocal.m_NBDRxTemp.arr_fADC_Die_Temperature[0],\
				sRFSAPRLocal.m_NBVoltage.fVX1,sRFSAPRLocal.m_NBVoltage.fVX2,sRFSAPRLocal.m_NBVoltage.fVX3,sRFSAPRLocal.m_NBVoltage.fVX4,sRFSAPRLocal.m_NBVoltage.fVX5,\
				sRFSAPRLocal.m_NBVoltage.fVP1,sRFSAPRLocal.m_NBVoltage.fVP2,sRFSAPRLocal.m_NBVoltage.fVP3,sRFSAPRLocal.m_NBVoltage.fVP4,sRFSAPRLocal.m_NBVoltage.fVH,\
				sRFSAPRLocal.m_PSUVoltage.m_flocalTemp,sRFSAPRLocal.m_PSUVoltage.m_fRemoteTemp,dNBFSTemp,dNBDC1Temp,dNBDC2Temp,ulFan1SpeedRPM,dIOmatrixTemp);
*/
				fprintf(m_fpDataFile,"%s,%f,%f,%s,%d,%s,%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d,%f\n",strDatetym,sRFSAPRLocal.fProcessorLocalTemperature,sRFSAPRLocal.fCPU_DieTemperature,strElsapTime,sRFSAPRLocal.uiETIEvtCnt,\
				strEth1Sts,strEth2Sts,sRFSAPRLocal.m_DRX_Temperature[0].fFPGA_Local_Temperature,sRFSAPRLocal.m_DRX_Temperature[0].fFPGA_Die_Temperature,sRFSAPRLocal.m_DRX_Temperature[0].fADC_LocalTemperature,sRFSAPRLocal.m_DRX_Temperature[0].arr_fADC_Die_Temperature[0],
				sRFSAPRLocal.m_DRX_Temperature[0].arr_fADC_Die_Temperature[1],sRFSAPRLocal.m_DRX_Temperature[0].arr_fADC_Die_Temperature[2],sRFSAPRLocal.m_DRX_Temperature[0].arr_fADC_Die_Temperature[3],
				sRFSAPRLocal.m_DRX_Temperature[1].fFPGA_Local_Temperature,sRFSAPRLocal.m_DRX_Temperature[1].fFPGA_Die_Temperature,sRFSAPRLocal.m_DRX_Temperature[1].fADC_LocalTemperature,sRFSAPRLocal.m_DRX_Temperature[1].arr_fADC_Die_Temperature[0],
				sRFSAPRLocal.m_DRX_Temperature[1].arr_fADC_Die_Temperature[1],sRFSAPRLocal.m_DRX_Temperature[1].arr_fADC_Die_Temperature[2],sRFSAPRLocal.m_DRX_Temperature[1].arr_fADC_Die_Temperature[3],sRFSAPRLocal.m_NBDRxTemp.fFPGA_Local_Temperature, 
				sRFSAPRLocal.m_NBDRxTemp.fFPGA_Die_Temperature ,sRFSAPRLocal.m_NBDRxTemp.fADC_LocalTemperature,sRFSAPRLocal.m_NBDRxTemp.arr_fADC_Die_Temperature[0],\
				sRFSAPRLocal.m_NBVoltage.fVX1,sRFSAPRLocal.m_NBVoltage.fVX2,sRFSAPRLocal.m_NBVoltage.fVX3,sRFSAPRLocal.m_NBVoltage.fVX4,sRFSAPRLocal.m_NBVoltage.fVX5,\
				sRFSAPRLocal.m_NBVoltage.fVP1,sRFSAPRLocal.m_NBVoltage.fVP2,sRFSAPRLocal.m_NBVoltage.fVP3,sRFSAPRLocal.m_NBVoltage.fVP4,sRFSAPRLocal.m_NBVoltage.fVH,\
				dNBFSTemp,dNBDC1Temp,dNBDC2Temp,ulFan1SpeedRPM,dIOmatrixTemp);
	}

	
	m_LstCtlSBCSts.RedrawControl();		
	
	// Refresh the control.
	//m_LstCtlSBCSts.Populate();
	
}


short COnlineMonitoring::ConnecttoServer(DP_RFSAPR_ETH_COMM *out_ETH_Comm,bool in_bStartStop)
{
	
	short sRetVal = 0;
	CString strTempIPAddress = "";
	CString strTempPortnumber = "";

	MAINAPP(pMainApp);
	//Fills a block of memory with zeros
	ZeroMemory(&saSockAddr,sizeof(sockaddr_in));
	
	if(in_bStartStop)
	{
		
		if(pMainApp->sktOnlineStsSocket)
		{
			closesocket(pMainApp->sktOnlineStsSocket);
			WSACleanup();
			pMainApp->sktOnlineStsSocket = 0;	

		}
		
		// Initialize Winsock
		sRetVal = WSAStartup(MAKEWORD(2,2),&pMainApp->wsaData);
		
		if (sRetVal != RFSAPR_ATE_SUCCESS) 
		{        
			pMainApp->PrintLog("WSAStartup failed",2);		
			out_ETH_Comm->bIsOnlineStsSocCntd = false; // added 
			return RFSAPR_ATE_FAILURE;
		}

		strTempPortnumber.Format("%u",out_ETH_Comm->uiOnlinesocPortNo);
		strTempIPAddress.Format("%d.%d.%d.%d",out_ETH_Comm->ucAddr[0],out_ETH_Comm->ucAddr[1],out_ETH_Comm->ucAddr[2],out_ETH_Comm->ucAddr[3]);

		
		// Create a SOCKET for connecting to server
		pMainApp->sktOnlineStsSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (pMainApp->sktOnlineStsSocket == INVALID_SOCKET) 
		{      
			pMainApp->PrintLog("Online Status Socket Creation failed",2);
			WSACleanup();
			return RFSAPR_ATE_FAILURE;
		}
		
		saSockAddr.sin_family = AF_INET;
		saSockAddr.sin_port = htons((unsigned short)atoi(strTempPortnumber));
		saSockAddr.sin_addr.s_addr=inet_addr(strTempIPAddress);
		
		
		struct timeval tvTimeOutValue;
		
	//	tvTimeOutValue.tv_sec = 1000;
		tvTimeOutValue.tv_sec = 10;
		
		setsockopt(pMainApp->sktOnlineStsSocket, SOL_SOCKET, SO_RCVTIMEO,(const char *)(struct timeval *)&tvTimeOutValue,sizeof(struct timeval));
		
		// Connect to server.
		sRetVal = connect(pMainApp->sktOnlineStsSocket,(struct sockaddr*)&saSockAddr, (int)sizeof(sockaddr_in));
		if (sRetVal == SOCKET_ERROR) 
		{
			closesocket(pMainApp->sktOnlineStsSocket);
			pMainApp->sktOnlineStsSocket = INVALID_SOCKET;     
			out_ETH_Comm->bIsOnlineStsSocCntd = false; //added 
			pMainApp->PrintLog("Unable to connect to Online status server!",2);
			WSACleanup();
			return RFSAPR_ATE_FAILURE;
			
		}

		pMainApp->m_bOnlineStatusFlag = true;
		
		
		
	}
	else
	{
		pMainApp->m_bOnlineStatusFlag = false;
		out_ETH_Comm->bIsOnlineStsSocCntd = false; //added 
		closesocket(pMainApp->sktOnlineStsSocket);
		// Do the clean up
		WSACleanup();
	}
	
	return RFSAPR_ATE_SUCCESS;
	

}

UINT OnlineStsRead(LPVOID lParam)
{
	MAINAPP(pMainApp)
		
	char cRecvData[1024]	= {NULL};
	CString  str = "";
	
	COnlineMonitoring *pobjOnlineSts = (COnlineMonitoring*)lParam;
	
	unsigned long ulRxMessageSize = 0;
	unsigned long ulTxMessageSize = 0;
	short sRetVal = 0;
	char arrcData[MAX_ERR_STRING_LEN] = {0};

	while(1)
	{

		if(pMainApp->g_RFSAPR_OnlineEth_Config.bISConfigured == false) //Socket Connection
		{			
			break;
		}

#if 1
		Sleep(5000); /* To Update the GUI for every 2 seconds */

		/*Set Group Id and Unit Id for Version*/
		pMainApp->SOnlineCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usGroupId = RFSAPR_COMM_CMD_GROUP0;
		pMainApp->SOnlineCmdFromRFSAPRAtp.m_SRFSAPRCmdIds.m_usUnitId	= RFSAPR_CMD_READ_SENSOR_DATA;
		
 		ulTxMessageSize = 0;

		sRetVal = pMainApp->m_pObjTCPOnlineSockComm->RFSAPR_TCPCmdTrans(pMainApp->g_RFSAPR_OnlineEth_Config.uiConnectedSocket, &pMainApp->SOnlineCmdFromRFSAPRAtp, sizeof(S_RFSAPR_IDS), &pMainApp->SOnlineResToRFSAPRAtp, &ulRxMessageSize, DP_RFSAPR_CMD_DEFAULT_TIMEOUT_MS);
		if(sRetVal == DP_RFSAPR_SUCCESS)
		{
			if((pMainApp->SOnlineResToRFSAPRAtp.m_SRFSAPRResIds.m_usGroupId == RFSAPR_COMM_CMD_GROUP0) && (pMainApp->SOnlineResToRFSAPRAtp.m_SRFSAPRResIds.m_usUnitId == RFSAPR_RES_READ_SENSOR_DATA))
			{
				
#ifdef ENDIAN_SWAP			
				sRetVal = EndianSwap(&pMainApp->SOnlineResToRFSAPRAtp.m_UResRFSAPRMessage.S_Res_Temperature_Sensor);
				if(sRetVal)
				{
					GetErrorMsg(sRetVal, arrcData);
					pMainApp->PrintLog(arrcData, DP_RFSAPR_PRINTLOG_FAILURES);
					AfxMessageBox(arrcData,MB_ICONERROR);		
				}
#endif			
				pobjOnlineSts->SendMessage(WM_USER_UPDATE_ONLINE_STS, 0, 0);
					
			}
			else
			{
				pMainApp->g_ucSensorDataParsing_Flag  = 0;
				pMainApp->PrintLog("Invalid Group/Unit ID of Resp", DP_RFSAPR_PRINTLOG_FAILURES);			
			}
		}
		else
		{
			pMainApp->g_ucSensorDataParsing_Flag  = 0;
			//pMainApp->PrintLog("Sensor data parsing failed!", DP_RFSAPR_PRINTLOG_FAILURES);
		//	strcpy(out_Msg, "Sensor data parsing failed!");
		}

#endif

	}
	return RFSAPR_ATE_SUCCESS;	
}
