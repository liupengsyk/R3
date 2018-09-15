#pragma once


// CamCodeDlg 对话框

class CamCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CamCodeDlg)

public:
	CamCodeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CamCodeDlg();

// 对话框数据
	enum { IDD = IDD_CamCodeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString CamCode;
	CString CamRunChard;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
