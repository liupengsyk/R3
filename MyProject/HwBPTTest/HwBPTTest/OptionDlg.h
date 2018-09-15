#pragma once


// OptionDlg �Ի���
#include "resource.h"

class HwBPTTest_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, HwBPTTest_TestItem* pTestItem);   // ��׼���캯��
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
	HwBPTTest_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadotpdata();

public:
// 	int DCC_Hig;
// 	int DCC_Low;

	int m_VerInfo;
	int m_StaInfo;

	CString m_ExValue;
	CString m_GainValue;
	afx_msg void OnEnChangeGain();

	CString m_bptErrCode;
	CString m_bptErrInfo;
	CString m_bptFrmDelayNum;

	CString m_bptBurnStartAddr;
	CString m_bptBurnEndAddr;
	int m_SensorSelection;
	BOOL m_SavePic;
	BOOL m_SaveData;
};
