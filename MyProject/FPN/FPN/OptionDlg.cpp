// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FPN.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "FPN_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, FPN_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_RowMaxSpec(0)
	, m_bSaveImage(FALSE)
	, m_ColMaxSpec(0)
	, m_DefultExposure(0)
	, m_DefultGain(0)
	, ErrorCode(0)
{
	m_pTestItem = pTestItem;
	m_SinglePixelMax = 0;
	m_PixelCountPercent = 0;
	m_DiffRowPercent = 0;
	m_DiffColPercent = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RowMaxSpec, m_RowMaxSpec);
	DDX_Check(pDX, IDC_CHECK1, m_bSaveImage);
	DDX_Text(pDX, IDC_ColMaxSpec, m_ColMaxSpec);
	DDX_Text(pDX, IDC_DefultExposure, m_DefultExposure);
	DDX_Text(pDX, IDC_DefultGain, m_DefultGain);
	DDX_Text(pDX, IDC_ERROR_CODE, ErrorCode);
	DDX_Text(pDX, IDC_SinglePixelMax, m_SinglePixelMax);
	DDX_Text(pDX, IDC_PixelCountPercent, m_PixelCountPercent);
	DDX_Text(pDX, IDC_DiffRowSpec, m_DiffRowPercent);
	DDX_Text(pDX, IDC_DiffColSpec, m_DiffColPercent);
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