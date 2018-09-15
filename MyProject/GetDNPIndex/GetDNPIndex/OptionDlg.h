#pragma once


// OptionDlg ¶Ô»°¿ò
#include "resource.h"

class GetDNPIndex_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, GetDNPIndex_TestItem* pTestItem);   // ±ê×¼¹¹Ôìº¯Êý
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
	GetDNPIndex_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	CString FuseID[20];
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
	int R_index_min;
	int R_index_max;
	int Gr_index_min;
	int Gr_index_max;
	int Gb_index_min;
	int Gb_index_max;
	int B_index_min;
	int B_index_max;
	int RG_index_min;
	int RG_index_max;
	int BG_index_min;
	int BG_index_max;
	int GG_index_min;
	int GG_index_max;
	int index_error;
	CString ByteCount;
	CString A_Multi;
	CString B_Multi;
	CString Station;
	BOOL SingleChannelEnable;
	BOOL GainEnable;
	int m_Ratio;
	int m_BLC;
	int m_index;
	BOOL m_Raw8_EN;
	int page;
	
	int m_DNPDistance;
	BOOL m_GG_GainEn;
	BOOL m_flow;
	int m_count;
	BOOL m_QTITool;

	int m_ColorNum;
	int m_ModuleNum;
	CString m_ColorName;
	BOOL m_isSingleModule;
};
