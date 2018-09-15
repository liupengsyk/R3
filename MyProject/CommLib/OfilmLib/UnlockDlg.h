// UnlockDlg.h : header file
//

#if !defined(AFX_UNLOCKDLG_H__75E43198_BCAE_41AD_9D0B_B4EF6074C719__INCLUDED_)
#define AFX_UNLOCKDLG_H__75E43198_BCAE_41AD_9D0B_B4EF6074C719__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUnlockDlg dialog

class CUnlockDlg : public CDialog
{
// Construction
public:
	CUnlockDlg(CWnd* pParent = NULL);	// standard constructor
	char *cWndText;
	char *cShowText;
	char *ReturnText;
	int Res;
// Dialog Data
	//{{AFX_DATA(CUnlockDlg)
	enum { IDD = IDD_UNLOCK_DIALOG };
	CEdit	m_edit;
	CString	m_PWD;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnlockDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CFont m_Font;
	CFont l_Font;
	CFont n_Font;
	// Generated message map functions
	//{{AFX_MSG(CUnlockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNLOCKDLG_H__75E43198_BCAE_41AD_9D0B_B4EF6074C719__INCLUDED_)
