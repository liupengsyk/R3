#pragma once

#include "resource.h"		// ������
// CSFRROISelect �Ի���

class CSFRROISelect : public CDialog
{
	DECLARE_DYNAMIC(CSFRROISelect)

public:
	CSFRROISelect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSFRROISelect();
	
	CString GetModulePath();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedSelectall();
//	CButtonST //m_moveOffset;
};
