#pragma once

#include "resource.h"		// 主符号
// CSFRROISelect 对话框

class CSFRROISelect : public CDialog
{
	DECLARE_DYNAMIC(CSFRROISelect)

public:
	CSFRROISelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSFRROISelect();
	
	CString GetModulePath();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSelectall();
//	CButtonST //m_moveOffset;
};
