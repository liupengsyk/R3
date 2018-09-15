#pragma once
#include "afxcmn.h"

typedef struct _SQUARE_CHART_SFR_ROI_PARA
{
	BOOL enable;
	CString cX;
	CString cY;
	CString freq;
	CString limitH;
	CString limitV;
	CString sfrOffsetH;
	CString sfrOffsetV;
	CString fov;

	CRect rcH;
	CRect rcV;
	float sfrValH;
	float sfrValV;
	BOOL resultH;
	BOOL resultV;
	CString memo;
} SQUARE_CHART_SFR_ROI_PARA, *PSQUARE_CHART_SFR_ROI_PARA;

typedef struct _SQUARE_CHART_CORNER_DIFF_PARA
{
	BOOL enable;
	CString spec;
	CString specH;
	CString specV;
	CString roiNumLU;
	CString roiNumRU;
	CString roiNumLD;
	CString roiNumRD;
	BOOL diffAbsolute;
	CString diffOffset;
	CString diffOffsetH;
	CString diffOffsetV;

	float diffVal;
	float diffValH;
	float diffValV;
} SQUARE_CHART_CORNER_DIFF_PARA, *PSQUARE_CHART_CORNER_DIFF_PARA;

typedef struct _SQUARE_CHART_SFR_PARA
{
	int roiCnt;
	SQUARE_CHART_SFR_ROI_PARA roiPara[64];
	CString imgWidth;
	CString imgHeight;
	CString roiWidth;
	CString roiHeight;

	CString roiMaskWidth;
	CString roiMaskHeight;
	CString roiMaskPhi;

	SQUARE_CHART_CORNER_DIFF_PARA cornerDiffPara;
} SQUARE_CHART_SFR_PARA, *PSQUARE_CHART_SFR_PARA;

// CDlgSquareChartSfr 对话框

class CDlgSquareChartSfr : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSquareChartSfr)

public:
	CDlgSquareChartSfr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSquareChartSfr();

// 对话框数据
	enum { IDD = IDD_DIALOG_SQUARE_CHART_SFR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listSquareChartRoiPara;
	virtual BOOL OnInitDialog();
	virtual void PreSubclassWindow();
public:
	SQUARE_CHART_SFR_PARA m_squareChartSfrPara;
	CString m_addParaByFov;
	BOOL m_roiEnable;
	CString m_pX;
	CString m_pY;
	CString m_freq;
	CString m_sfrOffsetH;
	CString m_sfrOffsetV;
	CString m_specH;
	CString m_specV;
	CString m_memo;
	int m_selectedItemNum;
	afx_msg void OnBnClickedButtonSquareCharSfrAddRoiPara();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickListSquareChartRoiPara(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSquareChartRoiPara(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateListControl(void);
	afx_msg void OnBnClickedButtonSquareCharSfrDelAllRoiPara();
	afx_msg void OnBnClickedButtonSquareCharSfrDelRoiPara();
	afx_msg void OnBnClickedCheckSquareCharCornerDiffEnable();
	void Update_Corner_Diff_UI();
	afx_msg void OnBnClickedButtonSquareCharSfrUpdateRoiPara();
};
