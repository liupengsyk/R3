#pragma once


// OptionDlg �Ի���
#include "resource.h"

class FPN_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, FPN_TestItem* pTestItem);   // ��׼���캯��
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
	FPN_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int m_RowMaxSpec;
	BOOL m_bSaveImage;
	int m_ColMaxSpec;
	int m_DefultExposure;
	int m_DefultGain;
	int ErrorCode;
	int m_SinglePixelMax;
	int m_PixelCountPercent;
	int m_DiffRowPercent;
	int m_DiffColPercent;
};
