#pragma once
#include "resource.h"

typedef struct _MTF_ROI_PARA
{
	BOOL enable;
	CString cx;
	CString cy;
	CString limit1;
	CString limit2;

	CRect rc;
	float mtfVal;
	BOOL result;
} MTF_ROI_PARA, *PMTF_ROI_PARA;

typedef struct _MTF_PARA
{
	MTF_ROI_PARA roiPara[16];
	CString roiWidth;
	CString roiHeight;
	CString cornerDiffSpec;

	float cornerDiff;
	int   bUseRaw8Test;
} MTF_PARA, *PMTF_PARA;

// CDlgMtf 对话框

class CDlgMtf : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMtf)

public:
	CDlgMtf(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMtf();

// 对话框数据
	enum { IDD = IDD_DIALOG_MTF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	MTF_PARA m_mtfPara;
	afx_msg void OnBnClickedOk();
};
