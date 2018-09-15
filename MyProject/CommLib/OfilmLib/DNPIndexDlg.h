#if !defined(AFX_DNPINDEXDLG_H__91A24D58_07D8_476F_9250_43EC06AEEA2D__INCLUDED_)
#define AFX_DNPINDEXDLG_H__91A24D58_07D8_476F_9250_43EC06AEEA2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DNPIndexDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDNPIndexDlg dialog

class CDNPIndexDlg : public CDialog
{
// Construction
public:
	int m_CameraCnt;
	void InsertTestDataIntoForm( int nIndex );

	USHORT m_RGIndex;
	USHORT m_BGIndex;
	USHORT m_GGIndex;

	float m_RGCurr;
	float m_GGCurr;
	float m_BGCurr;

	int m_RGIndexSpec;
	int m_BGIndexSpec;
	int m_GGIndexSpec;

	CWnd* m_pParent;
	CDNPIndexDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDNPIndexDlg)
	enum { IDD = IDD_DIALOG_DNPINDEX };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDNPIndexDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_Setup;

	// Generated message map functions
	//{{AFX_MSG(CDNPIndexDlg)
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnAverage();
	afx_msg void OnWriteintoSpec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DNPINDEXDLG_H__91A24D58_07D8_476F_9250_43EC06AEEA2D__INCLUDED_)
