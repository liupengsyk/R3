#pragma once


// OptionDlg 对话框
#include "resource.h"

class SetLEDChannel_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, SetLEDChannel_TestItem* pTestItem);   // 标准构造函数
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
	SetLEDChannel_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
//	CString m_BLC;
	int m_BLC;
	int LEDChannel;
	int m_LEDCOM;
	int m_SleepTime;
	int ALight_Channel;
	int TL84LightChannel;
	int D65LightChannel;
	BOOL m_CheckEN;
	int Checknumber;
	int m_ARGHigh;
	int m_ARGLow;
	int m_ABGHigh;
	int m_ABGLow;
	int m_TLRGHigh;
	int m_TLRGLow;
	int m_TLBGHigh;
	int m_TLBGLow;
	int m_DRGHigh;
	int m_DRGLow;
	int m_DBGHigh;
	int m_DBGLow;
	int m_LEDType;
	BOOL m_WaitFlag;
};
