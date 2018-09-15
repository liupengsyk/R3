#pragma once

#include "DlgMtf.h"
#include "DlgContrast.h"
#include "DlgSfr.h"
#include "DlgMotoCharSfr.h"
#include "DlgSquareChartSfr.h"

typedef struct _AE_PARA
{
	BOOL enable;
	BOOL overAverage;
	BOOL over128;

	CString channel;
	CString roi;
	CString blc;
	
	CString expoNum;
	CString expoInitVal;

	CString target;
	CString range;
} AE_PARA, *PAE_PARA;

typedef struct _AF_PARA
{
	CString errCode;
	CString errInfo;

	CString avAlgorithm;
	CString frmDelay;

	BOOL enable;
	CString startCode;
	CString endCode;
	CString afAvSpec;
	
	BOOL sfrFullMode;
	BOOL sfrLastMode;
	BOOL sfrBmpMode;
	BOOL sfrDemosaic;
	BOOL sfrRawBalance;

	CString stationName;

	CString throuthFocusLU;
	CString throuthFocusRU;
	CString throuthFocusLD;
	CString throuthFocusRD;
	BOOL throuthFocusEnable;
	BOOL throuthFocusEnginerModeEnable;

	BOOL finalChkEnable;
	BOOL finalChkAfEn;
	CString finalChkCodeName;
	CString finalChkAfRange;
	CString finalChkCodeDiffSpec;

	AE_PARA ae;
} AF_PARA, *PAF_PARA;

// OptionDlg 对话框
#include "resource.h"
#include "afxcmn.h"

class AF_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, AF_TestItem* pTestItem);   // 标准构造函数
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
	AF_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	AF_PARA m_afPara;
	CTabCtrl m_tabCtrl;

	CDlgMtf m_dlgMtf;
	CDlgSfr m_dlgSfr;
	CDlgContrast m_dlgContrast;
	CDlgMotoCharSfr m_dlgMotoChartSfr;
	CDlgSquareChartSfr m_dlgSquareChartSfr;
	afx_msg void OnBnClickedButtonMtfPara();
	afx_msg void OnBnClickedCheckAfAeEnable();
	void AE_UI_Update();
	afx_msg void OnBnClickedCheckAfThroughFocusEnable();
	void ThroughFocusUI_Update();
	afx_msg void OnBnClickedCheckAfEnable();
	void AF_UI_Update();
	afx_msg void OnBnClickedCheckFinalChkEnable();
	void FinalCheck_UI_Update();
	afx_msg void OnBnClickedButtonSfrSquareChartPara();
	afx_msg void OnBnClickedButtonSfrXchartPara();
	afx_msg void OnBnClickedButtonSfrMotoChartPara();

	BOOL m_bUseFCelltoByear;
};
