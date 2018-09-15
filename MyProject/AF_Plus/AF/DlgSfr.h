#pragma once
#include "resource.h"
#include "afxcmn.h"

typedef struct _X_CHART_SFR_ROI_PARA
{
	BOOL topEn;
	BOOL bottomEn;
	BOOL leftEn;
	BOOL rightEn;
	CString cX;
	CString cY;
	CString width;
	CString height;
	CString freq;
	CString limitH;
	CString limitV;
	CString sfrOffsetH;
	CString sfrOffsetV;
	CString fov;

	CRect rcTop;
	CRect rcBottom;
	CRect rcLeft;
	CRect rcRight;
	float sfrValTop;
	float sfrValBottom;
	float sfrValLeft;
	float sfrValRight;
	BOOL resultTop;
	BOOL resultBottom;
	BOOL resultLeft;
	BOOL resultRight;
	CString memo;
} X_CHART_SFR_ROI_PARA, *PX_CHART_SFR_ROI_PARA;

typedef struct _X_CHART_CORNER_DIFF_PARA
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
	BOOL diffMinValueEn;
} X_CHART_CORNER_DIFF_PARA, *PX_CHART_CORNER_DIFF_PARA;

typedef struct _X_CHART_SFR_PARA
{
	int roiCnt;
	X_CHART_SFR_ROI_PARA roiPara[64];
	CString imgWidth;
	CString imgHeight;
	CString roiWidth;
	CString roiHeight;

	CString roiMaskWidth;
	CString roiMaskHeight;

	X_CHART_CORNER_DIFF_PARA cornerDiffPara;
} X_CHART_SFR_PARA, *PX_CHART_SFR_PARA;

// CDlgSfr 对话框

class CDlgSfr : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSfr)

public:
	CDlgSfr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSfr();

// 对话框数据
	enum { IDD = IDD_DIALOG_SFR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listXChartRoiPara;
	virtual BOOL OnInitDialog();
	virtual void PreSubclassWindow();
public:
	X_CHART_SFR_PARA m_xChartSfrPara;
	CString m_addParaByFov;
	BOOL m_roiTopEn;
	BOOL m_roiBottomEn;
	BOOL m_roiLeftEn;
	BOOL m_roiRightEn;
	CString m_pX;
	CString m_pY;
	CString m_freq;
	CString m_sfrOffsetH;
	CString m_sfrOffsetV;
	CString m_specH;
	CString m_specV;
	CString m_memo;
	int m_selectedItemNum;
	afx_msg void OnBnClickedButtonXcharSfrAddRoiPara();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickListXchartRoiPara(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListXchartRoiPara(NMHDR *pNMHDR, LRESULT *pResult);
	void UpdateListControl(void);
	afx_msg void OnBnClickedButtonXcharSfrDelAllRoiPara();
	afx_msg void OnBnClickedButtonXcharSfrDelRoiPara();
	afx_msg void OnBnClickedCheckXcharCornerDiffEnable();
	void Update_Corner_Diff_UI();
	afx_msg void OnBnClickedButtonXcharSfrUpdateRoiPara();
};
