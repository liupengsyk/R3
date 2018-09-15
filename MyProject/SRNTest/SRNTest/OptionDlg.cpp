// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SRNTest.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "SRNTest_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, SRNTest_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;

	m_srnPara.errCode = _T("16");
	m_srnPara.errInfo = _T("Test Failed");

	m_srnPara.specPPmax = _T("30");
	m_srnPara.frmDelayCnt = _T("5");

	m_srnPara.analogGain = _T("0x00");
	m_srnPara.exposureTime = _T("0x00");
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_SRN_ERROR_CODE, m_srnPara.errCode);
	DDX_Text(pDX, IDC_EDIT_SRN_ERROR_INFO, m_srnPara.errInfo);
	DDX_Text(pDX, IDC_EDIT_SRN_SPEC_PPMAX, m_srnPara.specPPmax);
	DDX_Text(pDX, IDC_EDIT_SRN_FRAME_DELAY_COUNT, m_srnPara.frmDelayCnt);
	DDX_Text(pDX, IDC_EDIT_SRN_ANALOG_GAIN_VALUE, m_srnPara.analogGain);
	DDX_Text(pDX, IDC_EDIT_SRN_EXPOSURE_TIME_VALUE, m_srnPara.exposureTime);
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