#pragma once


// OptionDlg ¶Ô»°¿ò
#include "resource.h"
#include <vector>
using namespace std;
class InitiaSensor_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, InitiaSensor_TestItem* pTestItem);   // ±ê×¼¹¹Ôìº¯Êý
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
	vector <CString> vecSensorlist;
	void GetvecSensorlist();
	InitiaSensor_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
//	BOOL m_Cam0;
//	BOOL m_Cam1;
	BOOL m_Cam0;
	BOOL m_Cam1;
	BOOL m_isChangeSize;
};
