#pragma once


// OptionDlg 对话框
#include "resource.h"

class ApplyWB_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, ApplyWB_TestItem* pTestItem);   // 标准构造函数
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
	ApplyWB_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int m_RHigh;
	int m_RLow;
	int m_Ratio;
	int m_RGHigh;
	int m_RGLow;
	int m_TypicalB;
	int m_TypicalGb;
	int m_TypicalGG;
	int m_TypicalGr;
	int m_TypicalR;
	int m_TypicalRG;
	int m_TypicalBG;
	int m_ROIWidth;
	int m_BHigh;
	int m_BLow;
	int m_BGHigh;
	int m_BGLow;
	int m_BLC;
	BOOL m_Raw8_EN;
	BOOL m_Raw10_EN;
	BOOL m_Channel_EN;
	BOOL m_Gain_EN;
	int m_GbHigh;
	int m_GbLow;
	int m_GGHigh;
	int m_GGLow;
	int m_GrHigh;
	int m_GrLow;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	int m_OTP_Distance;
	BOOL m_SaveImage;
	int m_BGIndex;
	int m_BIndex;
	int m_GbIndex;
	int m_GGIndex;
	int m_GrIndex;
	int m_RGIndex;
	int m_RIndex;
	int m_OTP_BeforeDistance;
	BOOL m_GG_GainEn;
	BOOL m_flow;
	int m_count;
	BOOL m_QTITool;
};
