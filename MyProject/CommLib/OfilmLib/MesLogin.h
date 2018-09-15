#if !defined(AFX_MESLOGIN_H__F7009FDF_E3C5_4B2A_A985_8C62D85846C1__INCLUDED_)
#define AFX_MESLOGIN_H__F7009FDF_E3C5_4B2A_A985_8C62D85846C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MesLogin.h : header file
//
#include  <wininet.h>
/////////////////////////////////////////////////////////////////////////////
// CMesLogin dialog
typedef struct _MesInfo
{
	char SN[256];
	char Name[256];
}MesInfo;

class CMesLogin : public CDialog
{
// Construction
public:
	CMesLogin(CWnd* pParent = NULL);   // standard constructor
	CString m_ConfigFile;
	CBitmap m_bmpBackground;
	char *LineNO;
	char *StationNo;
	char *Operator;
	char *Machine_NO;
	char *Module_NO;

	MesInfo StationInfo[512];
	MesInfo StationLineInfo[512];
	MesInfo ServerAddr[512];
	MesInfo ModuleNOInfo[512];

	int isMustTest[256];

	bool GetSCPRNO(char *ResturnData);
	bool GetModuleName(char *ResturnData);
	bool GetStationNO(char *ReturnData);

	CFont FontTxt;

	BOOL FtpConnect();
	HINTERNET hConnection;
	HINTERNET hSession;

// Dialog Data
	//{{AFX_DATA(CMesLogin)
	enum { IDD = IDD_LOGIN };
	CBitmapButton	m_BtnClose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMesLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMesLogin)
	virtual BOOL OnInitDialog();
	afx_msg void OnLogin();
	afx_msg void OnDropdownLineno();
	afx_msg void OnDropdownStationno();
	afx_msg void OnSelchangeLineno();
	afx_msg void OnSelchangeStationno();
	afx_msg void OnDropdownModuleid();
	afx_msg void OnPaint();
	afx_msg void OnBtnclose();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDropdownSeraddr();
	afx_msg void OnSelchangeSeraddr();
	afx_msg void OnSelchangeModuleid();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESLOGIN_H__F7009FDF_E3C5_4B2A_A985_8C62D85846C1__INCLUDED_)
