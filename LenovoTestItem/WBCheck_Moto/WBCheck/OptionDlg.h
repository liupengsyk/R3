#pragma once


// OptionDlg ¶Ô»°¿ò
#include "resource.h"

typedef enum
{
	MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC,
	MOTO_LIGHT_SRC_CAL_MODE_STATIC,

	MOTO_LIGHT_SRC_CAL_MODE_MAX
}_MOTO_LIGHT_SRC_CAL_MODE_;

class WBCheck_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, WBCheck_TestItem* pTestItem);   // ±ê×¼¹¹Ôìº¯Êý
	virtual ~OptionDlg();

// ¶Ô»°¿òÊý¾Ý
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	WBCheck_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	CString m_SlaveID;
	CString R_StartAdd;
	CString Gr_StartAdd;
	CString Gb_StartAdd;
	CString B_StartAdd;
	CString RG_StartAdd;
	CString BG_StartAdd;
	CString GG_StartAdd;
	int R_min;
	int R_max;
	int Gr_min;
	int Gr_max;
	int Gb_min;
	int Gb_max;
	int B_min;
	int B_max;
	int RG_min;
	int RG_max;
	int BG_min;
	int BG_max;
	int GG_min;
	int GG_max;
	CString ByteCount;
	CString A_Multi;
	CString B_Multi;
	//CString Station;
	BOOL SingleChannelEnable;
	BOOL GainEnable;
	int m_ROI_H;
	int m_ROI_V;
	int m_BLC;
	int m_Multiple;
	int m_TypicalB;
	int m_TypicalGb;
	int m_TypicalGG;
	int m_TypicalGr;
	int m_TypicalR;
	int m_TypicalRG;
	int m_TypicalBG;
	int m_distance;
	BOOL m_Raw8_EN;
	int page;
	BOOL m_GG_GainEn;
	int m_before_distance;
	BOOL m_flow;
	int m_count;
//	BOOL m_m_QTITool;
	BOOL m_QTITool;

	BOOL m_IsGoldenForCopy;
	int m_GoldenCopy_ErrH;
	int m_GoldenCopy_ErrL;
};
