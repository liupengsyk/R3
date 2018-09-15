// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ApplyWB_TestItem.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "ApplyWB_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

	OptionDlg::OptionDlg(CWnd* pParent, ApplyWB_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_RHigh = 0;
	m_RLow = 0;
	m_Ratio = 0;
	m_RGHigh = 0;
	m_RGLow = 0;
	m_TypicalB = 0;
	m_TypicalGb = 0;
	m_TypicalGG = 0;
	m_TypicalGr = 0;
	m_TypicalR = 0;
	m_TypicalRG = 0;
	m_TypicalBG = 0;
	m_ROIWidth = 0;
	m_BHigh = 0;
	m_BLow = 0;
	m_BGHigh = 0;
	m_BGLow = 0;
	m_BLC = 0;
	m_GbHigh = 0;
	m_GbLow = 0;
	m_GGHigh = 0;
	m_GGLow = 0;
	m_GrHigh = 0;
	m_GrLow = 0;
	m_OTP_Distance = 0;
	m_BGIndex = 0;
	m_BIndex = 0;
	m_GbIndex = 0;
	m_GGIndex = 0;
	m_GrIndex = 0;
	m_RGIndex = 0;
	m_RIndex = 0;
	m_OTP_BeforeDistance = 0;
	m_count = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_R_High, m_RHigh);
	DDX_Text(pDX, IDC_R_Low, m_RLow);
	DDX_Text(pDX, IDC_Ratio, m_Ratio);
	DDX_Text(pDX, IDC_RG_High, m_RGHigh);
	DDX_Text(pDX, IDC_RG_Low, m_RGLow);
	DDX_Text(pDX, IDC_Typical_B, m_TypicalB);
	DDX_Text(pDX, IDC_Typical_Gb, m_TypicalGb);
	DDX_Text(pDX, IDC_Typical_GG, m_TypicalGG);
	DDX_Text(pDX, IDC_Typical_Gr, m_TypicalGr);
	DDX_Text(pDX, IDC_Typical_R, m_TypicalR);
	DDX_Text(pDX, IDC_Typical_RG, m_TypicalRG);
	DDX_Text(pDX, IDC_TypicalBG, m_TypicalBG);
	DDX_Text(pDX, IDC_Width, m_ROIWidth);
	DDX_Text(pDX, IDC_B_High, m_BHigh);
	DDX_Text(pDX, IDC_B_Low, m_BLow);
	DDX_Text(pDX, IDC_BG_High, m_BGHigh);
	DDX_Text(pDX, IDC_BG_Low, m_BGLow);
	DDX_Text(pDX, IDC_BLC, m_BLC);
	DDX_Check(pDX, IDC_CHECK1, m_Raw8_EN);
	DDX_Check(pDX, IDC_CHECK2, m_Raw10_EN);
	DDX_Check(pDX, IDC_CHECK3, m_Channel_EN);
	DDX_Check(pDX, IDC_CHECK4, m_Gain_EN);
	DDX_Text(pDX, IDC_Gb_High, m_GbHigh);
	DDX_Text(pDX, IDC_Gb_Low, m_GbLow);
	DDX_Text(pDX, IDC_GG_High, m_GGHigh);
	DDX_Text(pDX, IDC_GG_Low, m_GGLow);
	DDX_Text(pDX, IDC_Gr_High, m_GrHigh);
	DDX_Text(pDX, IDC_Gr_low, m_GrLow);
	DDX_Text(pDX, IDC_OTP_Distance, m_OTP_Distance);
	DDX_Check(pDX, IDC_SaveImage, m_SaveImage);
	DDX_Text(pDX, IDC_BGINDEX, m_BGIndex);
	DDX_Text(pDX, IDC_BINDEX, m_BIndex);
	DDX_Text(pDX, IDC_GbINDEX, m_GbIndex);
	DDX_Text(pDX, IDC_GGINDEX, m_GGIndex);
	DDX_Text(pDX, IDC_GrINDEX, m_GrIndex);
	DDX_Text(pDX, IDC_RGINXE, m_RGIndex);
	DDX_Text(pDX, IDC_RINDEX, m_RIndex);
	DDX_Text(pDX, IDC_OTP_BeforeDistance, m_OTP_BeforeDistance);
	DDX_Check(pDX, IDC_CHECK5, m_GG_GainEn);
	DDX_Check(pDX, IDC_CHECK6, m_flow);
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDX_Check(pDX, IDC_CHECK7, m_QTITool);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_CHECK1, &OptionDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &OptionDlg::OnBnClickedCheck2)
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
		// 		CString CurrentName=m_pTestItem->GetName();
		// 		m_pTestItem->SetName(_T("WBSetting"));
		// 
		// 
		// 		//Typical Gain
		// 		m_TypicalRG  = ReadConfigInt(_T("Typical_RG"),200);
		// 		m_TypicalBG  = ReadConfigInt(_T("Typical_BG"),200);
		// 		m_TypicalGG  = ReadConfigInt(_T("Typical_GG"),200);
		// 
		// 		m_AWBSpec.Typical_RG  = pDlg->m_TypicalRG ;
		// 		m_AWBSpec.Typical_BG = pDlg->m_TypicalBG;
		// 		m_AWBSpec.Typical_GG  = pDlg->m_TypicalGG  ;
		// 		//Typical Channel
		// 		pDlg->m_TypicalR  = ReadConfigInt(_T("Typical_R"),200);
		// 		pDlg->m_TypicalGr  = ReadConfigInt(_T("Typical_Gr"),200);
		// 		pDlg->m_TypicalGb  = ReadConfigInt(_T("Typical_Gb"),200);
		// 		pDlg->m_TypicalB  = ReadConfigInt(_T("Typical_B"),200);
		// 		m_AWBSpec.Typical_R  = pDlg->m_TypicalR ;
		// 		m_AWBSpec.Typical_Gr  = pDlg->m_TypicalGr;
		// 		m_AWBSpec.Typical_Gb  = pDlg->m_TypicalGb  ;
		// 		m_AWBSpec.Typical_B  = pDlg->m_TypicalB ;
		// 
		// 		//Channel Range
		// 		pDlg->m_RLow=ReadConfigInt(_T("R_min"),30);
		// 		pDlg->m_RHigh=ReadConfigInt(_T("R_max"),220);
		// 		pDlg->m_GrLow=ReadConfigInt(_T("Gr_min"),30);
		// 		pDlg->m_GrHigh=ReadConfigInt(_T("Gr_max"),220);
		// 		pDlg->m_GbLow=ReadConfigInt(_T("Gb_min"),30);
		// 		pDlg->m_GbHigh=ReadConfigInt(_T("Gb_max"),220);
		// 		pDlg->m_BLow=ReadConfigInt(_T("B_min"),30);
		// 		pDlg->m_BHigh=ReadConfigInt(_T("B_max"),220);
		// 
		// 		m_AWBSpec.RHigh=pDlg->m_RHigh;
		// 		m_AWBSpec.RLow=pDlg->m_RLow;
		// 		m_AWBSpec.GrHigh=pDlg->m_GrHigh;
		// 		m_AWBSpec.GrLow=pDlg->m_GrLow;
		// 		m_AWBSpec.GbHigh=pDlg->m_GbHigh;
		// 		m_AWBSpec.GbLow=pDlg->m_GbLow;
		// 		m_AWBSpec.BHigh=pDlg->m_BHigh;
		// 		m_AWBSpec.BLow=pDlg->m_BLow;
		// 
		// 
		// 		//Gain Range
		// 		pDlg->m_RGLow=ReadConfigInt(_T("RG_min"),220);
		// 		pDlg->m_RGHigh=ReadConfigInt(_T("RG_max"),1000);
		// 		pDlg->m_BGLow=ReadConfigInt(_T("BG_min"),220);
		// 		pDlg->m_BGHigh=ReadConfigInt(_T("BG_max"),1000);
		// 		pDlg->m_GGLow=ReadConfigInt(_T("GG_min"),220);
		// 		pDlg->m_GGHigh=ReadConfigInt(_T("GG_max"),1000);
		// 
		// 
		// 		m_AWBSpec.RGainHigh=pDlg->m_RGHigh;
		// 		m_AWBSpec.RGainlow=pDlg->m_RGLow;
		// 		m_AWBSpec.BGainHigh=pDlg->m_BGHigh;
		// 		m_AWBSpec.BGainlow=pDlg->m_BGLow;
		// 		m_AWBSpec.GGainHigh=pDlg->m_GGHigh;
		// 		m_AWBSpec.GGainlow=pDlg->m_GGLow;
		// 
		// 
		// 		pDlg->m_Raw8_EN  = ReadConfigInt(_T("Raw8_EN"),1);
		// 		pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
		// 		pDlg->m_ROIWidth=ReadConfigInt(_T("Ratio"),5);
		// 		pDlg->m_Ratio=ReadConfigInt(_T("index"),512);
		// 
		// 		SetName(CurrentName);
		// 		pDlg->m_OTP_Distance  = ReadConfigInt(_T("OTP_Distance"),1);
		// 		pDlg->m_Channel_EN  = ReadConfigInt(_T("Channel_EN"),200);
		// 		pDlg->m_Gain_EN  = ReadConfigInt(_T("Gain_EN"),200);
		// 		pDlg->m_Raw10_EN  = ReadConfigInt(_T("Raw10_EN"),1);
		// 		pDlg->m_SaveImage  = ReadConfigInt(_T("SaveImage"),200);
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

void OptionDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Raw8_EN=TRUE;
	m_Raw10_EN=FALSE;
	UpdateData(FALSE);
}


void OptionDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Raw8_EN=FALSE;
	m_Raw10_EN=TRUE;
	UpdateData(FALSE);
}
