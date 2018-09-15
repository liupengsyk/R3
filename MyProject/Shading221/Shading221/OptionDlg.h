#pragma once


// OptionDlg ¶Ô»°¿ò
#include "resource.h"
#include <vector>
using namespace std;
class Shading221_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, Shading221_TestItem* pTestItem);   // ±ê×¼¹¹Ôìº¯Êý
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
	Shading221_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()

public:
	int m_BLC;
	int m_BGMAX;
	int m_BGMin;
	int m_RGMAX;
	int m_RGMin;
	int m_YShading_Diff;
	int m_YShading_Max;
	int m_YShading_Min;
};
