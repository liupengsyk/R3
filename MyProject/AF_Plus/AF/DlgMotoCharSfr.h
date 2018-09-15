#pragma once
#include "resource.h"
#include "afxcmn.h"

typedef struct _MOTO_CHART_SFR_ROI_PARA
{
	BOOL enable;
	CString width;
	CString height;
	CString freq;
	CString limit;
	CString sfrOffset;
	CString fov;

	CRect rc;
	float sfrVal;
	BOOL result;
	CString memo;
} MOTO_CHART_SFR_ROI_PARA, *PMOTO_CHART_SFR_ROI_PARA;

typedef struct _MOTO_CHART_CORNER_DIFF_PARA
{
	BOOL enable;
	CString spec;
	CString roiNumLU;
	CString roiNumRU;
	CString roiNumLD;
	CString roiNumRD;
	BOOL diffAbsolute;
	CString diffOffset;

	float diffVal;
} MOTO_CHART_CORNER_DIFF_PARA, *PMOTO_CHART_CORNER_DIFF_PARA;

typedef struct _MOTO_CHART_SFR_PARA
{
	int roiCnt;
	MOTO_CHART_SFR_ROI_PARA roiPara[64];
	CString roiWidth;
	CString roiHeight;

	CString roiMaskWidth;
	CString roiMaskHeight;
	CString roiMaskOffsetX;

	MOTO_CHART_CORNER_DIFF_PARA cornerDiffPara;
} MOTO_CHART_SFR_PARA, *PMOTO_CHART_SFR_PARA;

// CDlgMotoCharSfr 对话框

class CDlgMotoCharSfr : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMotoCharSfr)

public:
	CDlgMotoCharSfr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMotoCharSfr();

// 对话框数据
	enum { IDD = IDD_DIALOG_MOTO_CHART_SFR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listMotoChartRoiPara;
	virtual BOOL OnInitDialog();
	virtual void PreSubclassWindow();
public:
	MOTO_CHART_SFR_PARA m_motoChartSfrPara;
	BOOL m_roiEnable;
	CString m_freq;
	CString m_sfrOffset;
	CString m_spec;
	CString m_memo;
	int m_selectedItemNum;
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickListMotoChartRoiPara(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListMotoChartRoiPara(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateListControl(void);
	afx_msg void OnBnClickedCheckMotoCharCornerDiffEnable();
	void Update_Corner_Diff_UI();
	afx_msg void OnBnClickedButtonMotoCharSfrUpdateRoiPara();
};
