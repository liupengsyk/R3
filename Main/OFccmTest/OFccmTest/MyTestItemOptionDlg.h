#pragma once
#include "afxcmn.h"
#include "MyTabSheet.h"

// MyTestItemOptionDlg �Ի���

class MyTestItemOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyTestItemOptionDlg)

public:
	MyTestItemOptionDlg(int CamID,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyTestItemOptionDlg();

// �Ի�������
	enum { IDD = IDD_MyTestItemOptionDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
