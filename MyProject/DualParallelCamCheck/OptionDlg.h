#pragma once


// OptionDlg �Ի���
#include "resource.h"

class DualParallelCamCheck_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, DualParallelCamCheck_TestItem* pTestItem);   // ��׼���캯��
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
	DualParallelCamCheck_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	CString offsetTx;
	CString offsetTy;
	CString offsetTz;
	int m_LeftCut;
	int m_BottomCut;
	int m_RightCut;
	int m_TopCut;
	int m_CutMode;
};