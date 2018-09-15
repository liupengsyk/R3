// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AF_MoveToPos.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "AF_MoveToPos_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, AF_MoveToPos_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_ModuleEFL = _T("");
	m_InfDistance = _T("");
	m_InfRegHigh = _T("");
	m_InfRegLow = _T("");
	m_MacroDistance = _T("");
	m_MacroRegHigh = _T("");
	m_MacroRegLow = _T("");
	m_MoveToPos = _T("");
	m_OtherCode = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_AF_MoveCode, m_MoveToPosChecked);
	DDX_Text(pDX, IDC_EDIT_EFL, m_ModuleEFL);
	DDX_Text(pDX, IDC_EDIT_InfDistance, m_InfDistance);
	DDX_Text(pDX, IDC_EDIT_InfRegHigh, m_InfRegHigh);
	DDX_Text(pDX, IDC_EDIT_InfRegLow, m_InfRegLow);
	DDX_Text(pDX, IDC_EDIT_MacroDistance, m_MacroDistance);
	DDX_Text(pDX, IDC_EDIT_MacroRegHigh, m_MacroRegHigh);
	DDX_Text(pDX, IDC_EDIT_MacroRegLow, m_MacroRegLow);
	DDX_Check(pDX, IDC_InfinityCode, m_InfinityPosChecked);
	DDX_Check(pDX, IDC_MacroCode, m_MacroPosChecked);
	DDX_Check(pDX, IDC_MiddleCode, m_MiddlePosChecked);
	DDX_Text(pDX, IDC_MovePos, m_MoveToPos);
	DDX_Check(pDX, IDC_SaveAFCode, m_SaveAFCode);
	DDX_Text(pDX, IDC_EDIT_OtherCode, m_OtherCode);
	DDX_Check(pDX, IDC_OtherPos, m_OtherCode_En);
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