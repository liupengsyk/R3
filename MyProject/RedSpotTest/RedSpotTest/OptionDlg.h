#pragma once


// OptionDlg 对话框
#include "resource.h"



class RedSpot_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, RedSpot_TestItem* pTestItem);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_REDSPOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	RedSpot_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	CString strExpVal;
	CString strGainVal;
	CString strExposureRegHig;
	CString strExposureRegLow;
	CString strGainRegHig;
	CString strGainRegLow;
	int iManualExDelayFrame;
	BOOL bManualExposure;
	int Redspot_Ymax;
	int Redspot_Rmax;
	int Redspot_Gmax;
	int Redspot_Bmax;
	int XBlockNum;
	int YBlockNum;
};
