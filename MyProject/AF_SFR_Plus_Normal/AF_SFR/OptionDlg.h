#pragma once


// OptionDlg 对话框
#include "resource.h"

class AF_SFR_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, AF_SFR_TestItem* pTestItem);   // 标准构造函数
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
	AF_SFR_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bTestCenter;
	BOOL m_bTestF3;
	BOOL m_bTestF4;
	BOOL m_bTestF5;
	BOOL m_bTestF6;
	BOOL m_bTestF8;

	int m_AFTestMode;
	int m_StartOffset;
	int m_EndOffset;
	int m_FastMode_Step;
	CString m_UseCode_Name;
	BOOL m_bSaveImage;
	int m_ROI_Size;
	float m_SpcShift_0F;
	float m_SpcShift_3F;
	float m_SpcShift_4F;
	float m_SpcShift_5F;
	float m_SpcShift_6F;
	float m_SpcShift_8F;
	BOOL m_SpcShift_En_0F;
	BOOL m_SpcShift_En_3F;
	BOOL m_SpcShift_En_4F;
	BOOL m_SpcShift_En_5F;
	BOOL m_SpcShift_En_6F;
	BOOL m_SpcShift_En_8F;
	int m_CodeDiffMax;
	int m_CodeDiffMin;
	CString m_InfinityStationName;
	CString m_MacroStationName;
	BOOL m_CodeDiffCheckEn;
	float m_SpcShift_5F_CTC;
	float m_SpcShift_6F_CTC;
	float m_SpcShift_8F_CTC;
	BOOL m_SpcShift_CTC_En_5F;
	BOOL m_SpcShift_CTC_En_6F;
	BOOL m_SpcShift_CTC_En_8F;
	
	float m_sfrFreq;
	BOOL m_cornerDiffRelative;
	BOOL m_cornerDiffAbsolute;

	CString m_smallRange;
	BOOL m_bCTCLimit;
	short m_ContrastStepCount;
	BOOL m_CheckPeakCode;
};
