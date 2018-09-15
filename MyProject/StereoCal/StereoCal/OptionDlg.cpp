// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StereoCal.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "StereoCal_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, StereoCal_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{

}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	DDX_Text(pDX,IDC_COMBO_Platform,m_pTestItem->Platform);
	DDX_Text(pDX,IDC_EDIT_CFG_NAME,m_pTestItem->ConfigName);
	DDX_Text(pDX,IDC_EDIT_LF,m_pTestItem->leftFarImgPath);
	DDX_Text(pDX,IDC_EDIT_LN,m_pTestItem->leftNearImgPath);
	DDX_Text(pDX,IDC_EDIT_RN,m_pTestItem->rightNearImgPath);
	DDX_Text(pDX,IDC_EDIT_RF,m_pTestItem->rightFarImgPath);
	DDX_Text(pDX,IDC_EDIT_MainCamID,m_pTestItem->MainCamID);

	DDX_Text(pDX,IDC_EDIT_CFG_DIR,m_pTestItem->cfg_dir);

	DDX_Text(pDX,IDC_EDIT_IMAGE_DIR,m_pTestItem->image_dir);

	//

	DDX_Text(pDX,IDC_EDITCAM0EFL,m_pTestItem->m_CamInfo[0].CamEFL);
	DDX_Text(pDX,IDC_EDITCAM1EFL,m_pTestItem->m_CamInfo[1].CamEFL);
	DDX_Text(pDX,IDC_EDITCAM0PIXEL,m_pTestItem->m_CamInfo[0].CamPixelSize);
	DDX_Text(pDX,IDC_EDITCAM1PIXEL,m_pTestItem->m_CamInfo[1].CamPixelSize);

	DDX_Text(pDX,IDC_EDITDOTPITCH,m_pTestItem->m_ChartInfo.objectPitch_x);
	DDX_Text(pDX,IDC_EDITNEARCHARTDIST,m_pTestItem->m_ChartInfo.NearChartDist);
	DDX_Text(pDX,IDC_EDITFARCHARTDIST,m_pTestItem->m_ChartInfo.FarChartDist);
	


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
	CComboBox * Platform = (CComboBox *)GetDlgItem(IDC_COMBO_Platform);
	Platform->AddString(_T("Mtk"));
	Platform->AddString(_T("Qualcomm")); 
	//Ofilm
	Platform->AddString(_T("Ofilm"));  
	Platform->AddString(_T("HwHdc")); 
	Platform->AddString(_T("HwMMI")); 
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}