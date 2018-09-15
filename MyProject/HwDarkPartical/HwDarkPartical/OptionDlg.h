#pragma once


// OptionDlg �Ի���
#include "resource.h"

class HwDarkPartical_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, HwDarkPartical_TestItem* pTestItem);   // ��׼���캯��
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
	HwDarkPartical_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_SaveImgRaw10;
	float m_QuantitySpec;
	short m_DnDiff;
	BOOL m_SaveDarkParticalInfo;
	CString m_AnalogGain;
	CString m_InitExposure;
	int m_SensorType;
	short m_FrameDelay;
	BOOL m_CheckGain;
};
