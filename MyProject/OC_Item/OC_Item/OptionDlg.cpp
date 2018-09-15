// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OC_Item.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "OC_Item_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, OC_Item_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_Continue(FALSE)
{
	m_pTestItem = pTestItem;
	m_OCSpecX_hig = 0;
	m_OCSpecY_hig = 0;
	m_OCSpec_x_AfterShift = 0;
	m_OCSpec_y_AfterShift = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_OCSpecX_hig);
	DDX_Text(pDX, IDC_EDIT3, m_OCSpecY_hig);
	DDX_Check(pDX, IDC_CHECK1, m_Continue);
	DDX_Check(pDX, IDC_CHECK_OCShift, m_OCSpec_Shift);
	DDX_Text(pDX, IDC_EDIT5, m_OCSpec_x_AfterShift);
	DDX_Text(pDX, IDC_EDIT6, m_OCSpec_y_AfterShift);
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
		m_OCSpecX_hig=m_pTestItem->ReadConfigInt(_T("OCSpecX_hig"),100);
		m_OCSpecY_hig=m_pTestItem->ReadConfigInt(_T("OCSpecY_hig"),100);
		m_Continue=m_pTestItem->ReadConfigInt(_T("m_Continue"),0);
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