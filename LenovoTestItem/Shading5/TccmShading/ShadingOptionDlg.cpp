// ShadingOptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TccmShading.h"
#include "ShadingOptionDlg.h"
#include "afxdialogex.h"
#include "ccmBaseInterface.h"
#include "TestItemInterface.h"
#include "Inspection_Shading.h"

// ShadingOptionDlg 对话框

IMPLEMENT_DYNAMIC(ShadingOptionDlg, CDialog)

ShadingOptionDlg::ShadingOptionDlg(CWnd* pParent,Inspection_Shading* pInspection )
	: CDialog(ShadingOptionDlg::IDD, pParent)
	, BLC(0)
{
	m_pInspection = pInspection;
	m_Shading0_x = 0;
	m_Shading0_y = 0;
	m_Shading1_x = 0;
	m_Shading1_y = 0;
	m_Shading2_x = 0;
	m_Shading2_y = 0;
	m_Shading3_x = 0;
	m_Shading3_y = 0;
	m_Shading4_x = 0;
	m_Shading4_y = 0;

	ROI_height = 10;
	ROI_width = 10;


	m_Continue = FALSE;
	m_YDiff = 0;
	m_YMax = 0;
	m_Ymin = 0;

	m_BGMin = 0;
	m_BGMax = 0;
	m_RGMax = 0;
	m_RGMin = 0;

	m_bSavShadingDataPerBlock = FALSE;
}

ShadingOptionDlg::~ShadingOptionDlg()
{
}

void ShadingOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SHADING0_x, m_pInspection->sShading.ROI[0].cx);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[0].cx, 0, 100);
	DDX_Text(pDX, IDC_SHADING0_y, m_pInspection->sShading.ROI[0].cy);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[0].cy, 0, 100);

	DDX_Text(pDX, IDC_SHADING1_x, m_pInspection->sShading.ROI[1].cx);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[1].cx, 0, 100);
	DDX_Text(pDX, IDC_SHADING1_y, m_pInspection->sShading.ROI[1].cy);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[1].cy, 0, 100);

	DDX_Text(pDX, IDC_SHADING2_x, m_pInspection->sShading.ROI[2].cx);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[2].cx, 0, 100);
	DDX_Text(pDX, IDC_SHADING2_y, m_pInspection->sShading.ROI[2].cy);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[2].cy, 0, 100);

	DDX_Text(pDX, IDC_SHADING3_x, m_pInspection->sShading.ROI[3].cx);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[3].cx, 0, 100);
	DDX_Text(pDX, IDC_SHADING3_y, m_pInspection->sShading.ROI[3].cy);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[3].cy, 0, 100);

	DDX_Text(pDX, IDC_SHADING4_x, m_pInspection->sShading.ROI[4].cx);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[4].cx, 0, 100);
	DDX_Text(pDX, IDC_SHADING4_y, m_pInspection->sShading.ROI[4].cy);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI[4].cy, 0, 100);




	DDX_Text(pDX, IDC_SHADING_HEIGHT, m_pInspection->sShading.ROI_height);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI_height, 1, 100);
	DDX_Text(pDX, IDC_SHADING_WIDTH, m_pInspection->sShading.ROI_width);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading.ROI_width, 1, 100);




	DDX_Check(pDX, IDC_CONTINUE, m_Continue);
	DDX_Check(pDX, IDC_CHECK3, m_bSavShadingDataPerBlock);

	DDX_Text(pDX, IDC_EDIT1, BLC);
	DDX_Text(pDX, IDC_YDiff, m_YDiff);
	DDX_Text(pDX, IDC_Ymax, m_YMax);
	DDX_Text(pDX, IDC_YMin, m_Ymin);

	DDX_Text(pDX, IDC_BGMin, m_BGMin);
	DDX_Text(pDX, IDC_BGMax, m_BGMax);
	DDX_Text(pDX, IDC_RGMax, m_RGMax);
	DDX_Text(pDX, IDC_RGMin, m_RGMin);
}


BEGIN_MESSAGE_MAP(ShadingOptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &ShadingOptionDlg::OnUpdateData)
END_MESSAGE_MAP()


// ShadingOptionDlg 消息处理程序
LRESULT ShadingOptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	BOOL	bSaveAndValidate = (BOOL) wParam;

	UpdateData(bSaveAndValidate);

	// TODO: Add your specialized code here
	if(bSaveAndValidate)
	{
	}
	else
	{
	}

	return 0;
}

void ShadingOptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

BOOL ShadingOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
