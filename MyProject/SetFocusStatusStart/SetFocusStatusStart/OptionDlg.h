#pragma once


// OptionDlg �Ի���
#include "resource.h"

class SetFocusStatusStart_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, SetFocusStatusStart_TestItem* pTestItem);   // ��׼���캯��
	virtual ~OptionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);

	//int m_MoveCode;
	//BOOL m_bAFMoveCode;
	//BOOL m_bInfinityCode;
	//BOOL m_bMacroCode;

	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	SetFocusStatusStart_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
};
