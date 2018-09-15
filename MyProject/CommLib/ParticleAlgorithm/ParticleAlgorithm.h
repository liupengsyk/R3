// ParticleAlgorithm.h : main header file for the PARTICLEALGORITHM DLL
//

#if !defined(AFX_PARTICLEALGORITHM_H__B1F136D4_DF28_430D_802A_76DF7F6EBC62__INCLUDED_)
#define AFX_PARTICLEALGORITHM_H__B1F136D4_DF28_430D_802A_76DF7F6EBC62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CParticleAlgorithmApp
// See ParticleAlgorithm.cpp for the implementation of this class
//

class CParticleAlgorithmApp : public CWinApp
{
public:
	CParticleAlgorithmApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParticleAlgorithmApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CParticleAlgorithmApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARTICLEALGORITHM_H__B1F136D4_DF28_430D_802A_76DF7F6EBC62__INCLUDED_)
