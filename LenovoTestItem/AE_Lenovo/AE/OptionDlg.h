#pragma once


// OptionDlg 对话框
#include "resource.h"

class AE_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, AE_TestItem* pTestItem);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	AE_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int m_HighLimit;
	int m_LowLimit;
	int m_Ratio;
	int m_BLC;
	BOOL m_WhiteEnable;
	BOOL m_White128Enable;

	BOOL m_Ratio2;
	BOOL m_10bit;
	BOOL m_is4Cell;
	BOOL m_is2PD;

	BOOL m_RemExp;
	BOOL m_DNPExp;
//	int m_excuont;
	int m_excount;
	CString m_SlaveID;
	int m_InitExposure;
	CString m_AddressH;
	CString m_AddressL;
	CString	m_GainReg1;
	CString m_GainReg2;
	CString m_GainVal1;
	CString m_GainVal2;
	int m_mode;
	int m_mode_Gain;

	CString m_aeTargetString;
	int ErrorCode;

	int m_count;
	int m_StartX;
	int m_StartY;

	int LEDChannel;
	int m_LEDType;
	int ALight_Channel;
	int TL84LightChannel;
	int D65LightChannel;
	int m_WaitFlag;
	int SetLedEN;
};
