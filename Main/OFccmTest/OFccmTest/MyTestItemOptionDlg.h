#pragma once
#include "afxcmn.h"
#include "MyTabSheet.h"

// MyTestItemOptionDlg 对话框

class MyTestItemOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyTestItemOptionDlg)

public:
	MyTestItemOptionDlg(int CamID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyTestItemOptionDlg();

// 对话框数据
	enum { IDD = IDD_MyTestItemOptionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MyTabSheet m_MyTabSheet; 
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedApplySetting();	 
	afx_msg void OnBnClickedCancel();
	int CamID;
	virtual void PostNcDestroy();
	afx_msg void OnSelchangingOptiondlgtab(NMHDR *pNMHDR, LRESULT *pResult);
};
