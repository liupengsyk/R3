#if !defined(AFX_CLIENTSOCKET_H__034F0A33_CC1C_4D98_BFB3_C8F825052043__INCLUDED_)
#define AFX_CLIENTSOCKET_H__034F0A33_CC1C_4D98_BFB3_C8F825052043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//
#include <WinSock2.h>
#include <WinSock.h>

#define WM_UPDATEMSG WM_USER+100
/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();
	CWnd *p_Main;

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL
	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__034F0A33_CC1C_4D98_BFB3_C8F825052043__INCLUDED_)
