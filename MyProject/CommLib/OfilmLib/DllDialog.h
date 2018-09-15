#if !defined(AFX_DLLDIALOG_H__CEA4C6AF_245D_48A6_B11A_A5521EAD7C4E__INCLUDED_)
#define AFX_DLLDIALOG_H__CEA4C6AF_245D_48A6_B11A_A5521EAD7C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DllDialog.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDllDialog dialog

class CDllDialog : public CDialog
{
// Construction
public:
	CDllDialog(CWnd* pParent = NULL);   // standard constructor
	CDllDialog(CWnd* pParent,int Res,DWORD xScale,DWORD yScale,DWORD WScale,DWORD HSale,char *sPass,char *sFail,char *Other);   // standard constructor
	
	CBrush m_brush;


	int m_Res;
	CString m_strTemp;

	char m_sPASS[128];
	char m_sFAIL[128];
	char m_Other[128];
	
	DWORD XScale;
	DWORD YScale;
	DWORD WidthScale;
	DWORD HeightScale;	
	
	void PassMessage(CString str, int Res);
// Dialog Data
	//{{AFX_DATA(CDllDialog)
	enum { IDD = IDD_SHOW_DIALOG};
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDllDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLDIALOG_H__CEA4C6AF_245D_48A6_B11A_A5521EAD7C4E__INCLUDED_)
