#pragma once
#include "resource.h"

// RawOptionDlg 对话框

class RawOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RawOptionDlg)

public:
	RawOptionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RawOptionDlg();

// 对话框数据
	enum { IDD = IDD_RawOptionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_Raw_Width,m_Raw_Height,RawBits; 
	int RawOutForamt,m_ShowType,m_FileLength;
	CString strRawOutForamt,strm_ShowType;
	 
	afx_msg void OnBnClickedOk();
};
