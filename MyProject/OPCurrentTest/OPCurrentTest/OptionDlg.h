#pragma once


// OptionDlg ¶Ô»°¿ò
#include "resource.h"

class OPCurrentTest_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, OPCurrentTest_TestItem* pTestItem);   // ±ê×¼¹¹Ôìº¯Êý
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
	OPCurrentTest_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	float m_iafmax;
	float m_iavmax;
	float m_iavmin;
	float m_idomax;
	float m_idomin;
	float m_idvmax;
	float m_idvmin;
	float m_iafmin;
	float m_iPowerMax;
	float m_iPowerMin;
};
