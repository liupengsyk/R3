#pragma once


// CamCodeDlg �Ի���

class CamCodeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CamCodeDlg)

public:
	CamCodeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CamCodeDlg();

// �Ի�������
	enum { IDD = IDD_CamCodeDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString CamCode;
	CString CamRunChard;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
