#pragma once
#include "resource.h"

// CDlgContrast �Ի���

class CDlgContrast : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgContrast)

public:
	CDlgContrast(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgContrast();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONTRAST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
