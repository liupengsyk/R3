#pragma once

#include "resource.h"
// ShadingOptionDlg 对话框
class Inspection_Shading;

class ShadingOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(ShadingOptionDlg)

public:
	ShadingOptionDlg(CWnd* pParent, Inspection_Shading* pInspection);   // 标准构造函数
	virtual ~ShadingOptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	Inspection_Shading* m_pInspection;
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	UINT m_Shading0_x;
	UINT m_Shading1_x;
	UINT m_Shading1_y;
	UINT m_Shading0_y;
	UINT m_Shading2_x;
	UINT m_Shading2_y;
	UINT m_Shading3_x;
	UINT m_Shading3_y;
	UINT m_Shading4_x;
	UINT m_Shading4_y;
//	double m_Ydif;
	UINT ROI_height;
	UINT ROI_width;
	double m_BG;
	double m_Delta_Y;
	double m_RG;
	UINT m_exp_tolerance;
	UINT m_exp_target;
	BOOL m_ExposureEn;
	BOOL m_Continue;
	int BLC;
	int m_YDiff;
	int m_YMax;
	int m_Ymin;
//	int m_RGMax;
	int m_BGMin;
	int m_BGMax;
	int m_RGMax;
	int m_RGMin;

	int Y1diffmax;
	int Y1diffmin;

	int Y2diffmax;
	int Y2diffmin;

	int Y3diffmax;
	int Y3diffmin;

	int Y4diffmax;
	int Y4diffmin;

	int shadingTimes;

	int DelayTime;
};
