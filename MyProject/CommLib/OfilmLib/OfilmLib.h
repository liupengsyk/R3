// OfilmLib.h : main header file for the REGULARDLL DLL
//

#if !defined(AFX_REGULARDLL_H__3E9CB22B_588B_4388_B778_B3416ADB79B3__INCLUDED_)
#define AFX_REGULARDLL_H__3E9CB22B_588B_4388_B778_B3416ADB79B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COfilmLibApp
// See OfilmLib.cpp for the implementation of this class
//

class COfilmLibApp : public CWinApp
{
public:
	COfilmLibApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COfilmLibApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COfilmLibApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGULARDLL_H__3E9CB22B_588B_4388_B778_B3416ADB79B3__INCLUDED_)
