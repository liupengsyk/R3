// BPTCalibration.h : main header file for the BPTCALIBRATION DLL
//

#if !defined(AFX_BPTCALIBRATION_H__345B609F_E34F_42CE_99A0_13FFCA79D1BB__INCLUDED_)
#define AFX_BPTCALIBRATION_H__345B609F_E34F_42CE_99A0_13FFCA79D1BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBPTCalibrationApp
// See BPTCalibration.cpp for the implementation of this class
//

class CBPTCalibrationApp : public CWinApp
{
public:
	CBPTCalibrationApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBPTCalibrationApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBPTCalibrationApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BPTCALIBRATION_H__345B609F_E34F_42CE_99A0_13FFCA79D1BB__INCLUDED_)
