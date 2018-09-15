// oSRNtest.h : main header file for the OSRNTEST DLL
//

#if !defined(AFX_OSRNTEST_H__6C5685CF_DCAB_47EC_AF05_8D53E68C17D6__INCLUDED_)
#define AFX_OSRNTEST_H__6C5685CF_DCAB_47EC_AF05_8D53E68C17D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COSRNtestApp
// See oSRNtest.cpp for the implementation of this class
//

class COSRNtestApp : public CWinApp
{
public:
	COSRNtestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COSRNtestApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COSRNtestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#ifndef oRAW_NOISE
#define oRAW_NOISE  extern "C" _declspec(dllexport)

oRAW_NOISE float RowNoiseTest(BYTE* Raw0, BYTE *Raw1, BYTE *Raw2, BYTE *Raw3, int nWidth, int nheigth, int BLC);
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OSRNTEST_H__6C5685CF_DCAB_47EC_AF05_8D53E68C17D6__INCLUDED_)
