#pragma once


// OptionDlg 对话框
#include "resource.h"
#include "afxcmn.h"
#include "TestItemInterface.h"
#include "afxwin.h"

const int MAX_SAMPLE_NUM = 30;

class AFCalibration_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, AFCalibration_TestItem* pTestItem);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();

	AFCalibration_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()

public:
	void LoadPara();
	void SavePara();

public:

    int m_nModuleNum;
	int m_nIntervalTime;
    CString m_strStation;
    CString m_strModule;
    CString m_strStdPath;
    CString m_strTestPath;
    CComboBox m_crtlStationNum;
    int m_nStationNum;
};
