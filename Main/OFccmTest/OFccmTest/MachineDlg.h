#pragma once
#include "MyTabSheet.h"

// MachineDlg 对话框

class MachineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MachineDlg)

public:
	MachineDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MachineDlg();

// 对话框数据
	enum { IDD = IDD_MachineSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:	 
	MyTabSheet m_MyTabSheet; 
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedCancel();
	virtual void PostNcDestroy();
	
};
