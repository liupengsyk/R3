#if !defined(AFX_PODSPECDLG_H__9DFEE226_5152_405F_9453_C61EC31E41FD__INCLUDED_)
#define AFX_PODSPECDLG_H__9DFEE226_5152_405F_9453_C61EC31E41FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PODSpecDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// PODSpecDlg dialog

class PODSpecDlg : public CDialog
{
// Construction
public:
	PODSpecDlg(CWnd* pParent = NULL);   // standard constructor
    CString m_szSetup;
// Dialog Data
	//{{AFX_DATA(PODSpecDlg)
	enum { IDD = IDD_PODSPECDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PODSpecDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PODSpecDlg)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PODSPECDLG_H__9DFEE226_5152_405F_9453_C61EC31E41FD__INCLUDED_)
