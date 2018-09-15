// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DualParallelCamCheck.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "DualParallelCamCheck_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, DualParallelCamCheck_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_LeftCut = 0;
	m_BottomCut = 0;
	m_RightCut = 0;
	m_TopCut = 0;
	m_CutMode = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX); 


	DDX_Check(pDX,IDC_bContinue,m_pTestItem->bContinue); 
	DDX_Check(pDX,IDC_bOnAA,m_pTestItem->bOnAA);

	DDX_Text(pDX,IDC_pixel_size_um0,m_pTestItem->CamCal.CamInfo[0].baseInfo.strPixelSize);
	DDX_Text(pDX,IDC_focal_length_mm0,m_pTestItem->CamCal.CamInfo[0].baseInfo.strFocusLength);
	//DDX_Text(pDX,IDC_Cam0Enable,m_pTestItem->CamCal.CamInfo[1].bEnable);

	DDX_Text(pDX,IDC_pixel_size_um1,m_pTestItem->CamCal.CamInfo[1].baseInfo.strPixelSize);
	DDX_Text(pDX,IDC_focal_length_mm1,m_pTestItem->CamCal.CamInfo[1].baseInfo.strFocusLength); 
	//DDX_Text(pDX,IDC_Cam1Enable,m_pTestItem->CamCal.CamInfo[1].bEnable); 

	DDX_Text(pDX,IDC_RatatioDif,m_pTestItem->CamCal.m_SpecData.strRotationSpc);	
	DDX_Text(pDX,IDC_TitleDif_x,m_pTestItem->CamCal.m_SpecData.strTiltXSpc); 
	DDX_Text(pDX,IDC_TitleDif_y,m_pTestItem->CamCal.m_SpecData.strTiltYSpc); 


	DDX_Text(pDX,IDC_Macro_chart_distance_mm,m_pTestItem->CamCal.Macro_PointChartInfo.ObjectLength);	
	DDX_Text(pDX,IDC_Macrochart_PointNum,m_pTestItem->CamCal.Macro_PointChartInfo.PointNum); 
	DDX_Text(pDX,IDC_Macrochart_PointSize_mm,m_pTestItem->CamCal.Macro_PointChartInfo.PointSize);	
	DDX_Text(pDX,IDC_Macrochart_pitch_mm,m_pTestItem->CamCal.Macro_PointChartInfo.PointPitch); 

	DDX_Text(pDX,IDC_Infinitychart_distance_mm,m_pTestItem->CamCal.Infinity_PointChartInfo.ObjectLength);	
	DDX_Text(pDX,IDC_Infinitychart_PointNum,m_pTestItem->CamCal.Infinity_PointChartInfo.PointNum); 
	DDX_Text(pDX,IDC_Infinitychart_PointSize_mm,m_pTestItem->CamCal.Infinity_PointChartInfo.PointSize);	
	DDX_Text(pDX,IDC_Infinitychart_PointPitch_mm,m_pTestItem->CamCal.Infinity_PointChartInfo.PointPitch); 

	DDX_Text(pDX, IDC_EDIT_OFFSET_TX, offsetTx);
	DDX_Text(pDX, IDC_EDIT_OFFSET_TY, offsetTy);
	DDX_Text(pDX, IDC_EDIT_OFFSET_TZ, offsetTz);
	DDX_Text(pDX, IDC_LEFTCUT, m_LeftCut);
	DDX_Text(pDX, IDC_BOTTOMCUT, m_BottomCut);
	DDX_Text(pDX, IDC_RIGHTCUT, m_RightCut);
	DDX_Text(pDX, IDC_TOPCUT, m_TopCut);
	DDX_Radio(pDX, IDC_RADIO1, m_CutMode);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
END_MESSAGE_MAP()


// OptionDlg 消息处理程序
void OptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

//对话框窗口数据更新的响应函数
LRESULT OptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	//TURE 控件的值—>变量    
	//FALSE 变量—>控件的值
	BOOL	bSaveAndValidate = (BOOL) wParam; 

	int ret = UpdateData(bSaveAndValidate);

	// TODO: Add your specialized code here
	if(bSaveAndValidate)
	{

	}
	else
	{

	}

	return ret;
}

//主程序按下ctrl+数字键的响应消息，通过此消息接受外界指令。
LRESULT OptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

//响应模组关闭消息
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}