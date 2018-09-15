// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "OFccmTestDlg.h"
#include "ClientSocket.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}
void CClientSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	  
	char buff[1024]={0};
	int bufflen=1024;
	int nLen = CSocket::Receive(buff,bufflen);
	p_Main->PostMessage (WM_UPDATEMSG,(WPARAM)buff,(LPARAM)nLen);
	CSocket::OnReceive(nErrorCode);
}
void CClientSocket::OnClose(int nErrorCode)
{
	char buff[1024]={0};
	int bufflen=1024;
	strcpy(buff,"H02000");
	p_Main->PostMessage (WM_UPDATEMSG,(WPARAM)buff,6);
	CSocket::OnClose(nErrorCode);
}
// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions
