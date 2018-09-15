#pragma once


// OptionDlg ¶Ô»°¿ò
#include "resource.h"

class Pre_Process_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, Pre_Process_TestItem* pTestItem);   // ±ê×¼¹¹Ôìº¯Êý
	virtual ~OptionDlg();

// ¶Ô»°¿òÊý¾Ý
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	Pre_Process_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int CalDiff;
	CString TestStation;
	CString ProductAdd;
	CString ProductID;
	BOOL m_ProductEnable;
	CString ProductAdd1;
	CString ProductID1;
	BOOL m_ProductEnable1;
	int CalTimeTag;
	CString m_SlaveID;
};
