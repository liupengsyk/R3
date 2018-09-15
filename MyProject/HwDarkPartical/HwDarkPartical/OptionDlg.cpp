// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HwDarkPartical.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "HwDarkPartical_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, HwDarkPartical_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_QuantitySpec = 0.0f;
	m_DnDiff = 0;
	m_AnalogGain = _T("");
	m_InitExposure = _T("");
	m_SensorType = 0;
	m_FrameDelay = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SaveImageRaw10, m_SaveImgRaw10);
	DDX_Text(pDX, IDC_QuantitySpec, m_QuantitySpec);
	DDX_Text(pDX, IDC_DN_Diff, m_DnDiff);
	DDX_Check(pDX, IDC_SaveDarkParticalInfo, m_SaveDarkParticalInfo);
	DDX_Text(pDX, IDC_AnalogGain, m_AnalogGain);
	DDX_Text(pDX, IDC_InitExposure, m_InitExposure);
	DDX_Radio(pDX, IDC_RADIO1, m_SensorType);
	DDX_Text(pDX, IDC_FrameDelay, m_FrameDelay);
	DDX_Check(pDX, IDC_CheckGain, m_CheckGain);
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