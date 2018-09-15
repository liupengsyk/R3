#pragma once


// OptionDlg 对话框
#include "resource.h"

class RX_OTPCheck_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, RX_OTPCheck_TestItem* pTestItem);   // 标准构造函数
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
	RX_OTPCheck_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	CString m_ProjectName;
	CString m_TestStation;
	afx_msg void OnBnClickedLoadsetting();

	UINT m_uErrorCode;
	BOOL m_bModuleSelection;
	CString m_BufferSize;	
	BOOL m_OTPStation;
	BOOL m_AECStation;
	BOOL m_PDAFStation;
	BOOL m_AFStation;
	BOOL m_CalStation;
	BOOL m_FinalTestStation;
    BOOL SaveEEPROMPDAF_En;
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedSaveeeprompdaf();
};
