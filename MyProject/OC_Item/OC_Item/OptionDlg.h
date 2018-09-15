#pragma once


// OptionDlg 对话框
#include "resource.h"

class OC_Item_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, OC_Item_TestItem* pTestItem);   // 标准构造函数
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
	OC_Item_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int m_OCSpecX_hig;
	int m_OCSpecY_hig;
	BOOL m_Continue;
	BOOL m_OCSpec_Shift;
	int m_OCSpec_x_AfterShift;
	int m_OCSpec_y_AfterShift;
};
