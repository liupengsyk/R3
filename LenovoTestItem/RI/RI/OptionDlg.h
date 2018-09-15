#pragma once


// OptionDlg 对话框
#include "resource.h"

typedef struct 
{
	float RI_Min_R;
	float RI_Min_Gr;
	float RI_Min_Gb;
	float RI_Min_B;
}_RI_MIN_SPEC_;

class RI_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, RI_TestItem* pTestItem);   // 标准构造函数
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
	RI_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	_RI_MIN_SPEC_ m_RiMinSpec;
};
