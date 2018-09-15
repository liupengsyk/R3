// BPTDemo.h : main header file for the BPTDEMO application
//

#if !defined(AFX_BPTDEMO_H__0D0E2E64_2766_4455_84F1_4F4C520F99E6__INCLUDED_)
#define AFX_BPTDEMO_H__0D0E2E64_2766_4455_84F1_4F4C520F99E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBPTDemoApp:
// See BPTDemo.cpp for the implementation of this class
//

class CBPTDemoApp : public CWinApp
{
public:
	CBPTDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBPTDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBPTDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BPTDEMO_H__0D0E2E64_2766_4455_84F1_4F4C520F99E6__INCLUDED_)
