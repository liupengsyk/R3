// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "L476F00_OTP.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "L476F00_OTP_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, L476F00_OTP_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_EEPROMRevision(0)
	, m_HWVersion(0)
	, m_SWVersion(0)
//	, m_DriverICID(0)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_ModuleCode, m_ModuleCode);
	DDX_Text(pDX, IDC_HWVersion, m_HWVersion);
	DDX_Text(pDX, IDC_SWVersion, m_SWVersion);

	DDX_Text(pDX, IDC_EEPROMRevision, m_EEPROMRevision);
	DDX_Text(pDX, IDC_ActuatorID, m_ActuatorID);
	DDX_Text(pDX, IDC_LensID, m_LensID);
	DDX_Text(pDX, IDC_ManuID, m_ManufactureID);
	DDX_Text(pDX, IDC_FactoryID, m_FactoryID);
	DDX_Text(pDX, IDC_ManufactureLine, m_ManufactureLine);
	DDX_Text(pDX, IDC_LPN1, m_LPN1);
	DDX_Text(pDX, IDC_LPN2, m_LPN2);
	DDX_Text(pDX, IDC_LPN3, m_LPN3);
	DDX_Text(pDX, IDC_LPN4, m_LPN4);
	DDX_Text(pDX, IDC_LPN5, m_LPN5);
	DDX_Text(pDX, IDC_LPN6, m_LPN6);
	DDX_Text(pDX, IDC_LPN7, m_LPN7);
	DDX_Text(pDX, IDC_LPN8, m_LPN8);
	//DDX_Text(pDX, IDC_OIS_VERSION, m_OISfirmwareversion);
	//DDX_Text(pDX, IDC_CAMERA_STAGE, m_Camerabuildingstage);
	//DDX_Text(pDX, IDC_PRODUCTION_FACTORY, m_Productionfactory);
	//DDX_Text(pDX, IDC_PRODUCTION_LINE, m_Productionline);




//	DDX_Text(pDX, IDC_DriverICID, m_DriverICID);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_BUTTON1, &OptionDlg::OnBnClickedButton1)
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

void OptionDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pTestItem->Dump();
}
