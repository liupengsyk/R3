#pragma once
#include "MyTabSheet.h"

// MachineDlg �Ի���

class MachineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MachineDlg)

public:
	MachineDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MachineDlg();

// �Ի�������
	enum { IDD = IDD_MachineSetting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:	 
	MyTabSheet m_MyTabSheet; 
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedCancel();
	virtual void PostNcDestroy();
	
};
