#pragma once


// OptionDlg �Ի���
#include "resource.h"

typedef struct _SRN_PARA
{
	CString errCode;
	CString errInfo;

	CString specPPmax;
	CString frmDelayCnt;

	CString analogGain;
	CString exposureTime;
} SRN_PARA, *PSRN_PARA;

class SRNTest_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, SRNTest_TestItem* pTestItem);   // ��׼���캯��
	virtual ~OptionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	SRNTest_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	SRN_PARA m_srnPara;
};
