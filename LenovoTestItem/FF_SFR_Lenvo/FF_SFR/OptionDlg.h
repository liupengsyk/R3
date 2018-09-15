#pragma once


// OptionDlg 对话框
#include "resource.h"

class FF_SFR_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, FF_SFR_TestItem* pTestItem);   // 标准构造函数
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
	FF_SFR_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bTestCenter;
	BOOL m_bTestF4;
	BOOL m_bTestF6;
	BOOL m_bTestF8;
	BOOL m_bSaveImage;
	int m_ROI_Size;
	BOOL m_isSFRBurn;
//	BOOL m_SpecShiftEn_0F;
	float m_SpcShift_0F;
	float m_SpcShift_4F;
	float m_SpcShift_6F;
	float m_SpcShift_8F;
	BOOL m_SpcShiftEn_0F;
	BOOL m_SpcShiftEn_4F;
	BOOL m_SpcShiftEn_6F;
	BOOL m_SpcShiftEn_8F;
	float m_SpcShift_6F_CTC;
	float m_SpcShift_8F_CTC;
	BOOL m_SpcShift_CTC_En_6F;
	BOOL m_SpcShift_CTC_En_8F;
	BOOL m_EnableSecondCheck;
	int m_sfrFreq;
	BOOL m_BchekSFRSpec;
	BOOL m_BSaveSFR_Acut;
	BOOL m_BSFRSaveDataOffset;
	BOOL m_B40cmSFRCheckData;
	BOOL m_B60cmSFRCheckData;
	BOOL m_B120cmSFRCheckData;
	CString m_StrSpecSFR_FIRST;
	CString m_StrSpecSFR_SECOND;
	int m_BLC;
};
