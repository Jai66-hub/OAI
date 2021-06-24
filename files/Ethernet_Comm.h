// UWES_Ethernet_Comm.h: interface for the UWES_Ethernet_Comm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UWES_ETHERNET_COMM_H__74A24C04_16B3_4440_AEC0_1DFC7092FCA0__INCLUDED_)
#define AFX_UWES_ETHERNET_COMM_H__74A24C04_16B3_4440_AEC0_1DFC7092FCA0__INCLUDED_


// Include microsoft communication control header file
#include <winsock2.h>


/**********************************************************************************************************/

short RFSAPR_ETH_Connect(DP_RFSAPR_ETH_COMM *out_ETH_Comm);

short RFSAPR_ETH_DisConnect(DP_RFSAPR_ETH_COMM *out_ETH_Comm);
short RFSAPR_ETH_CriticalSectionInit();


#endif // !defined(AFX_UWES_ETHERNET_COMM_H__74A24C04_16B3_4440_AEC0_1DFC7092FCA0__INCLUDED_)
