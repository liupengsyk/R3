// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DParticle.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "DParticle_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, DParticle_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, nLeftSL(0)
	, nRightSL(0)
	, nTopSL(0)
	, nBottomSL(0)
	, nAreaWidth(0)
	, nAreaHeight(0)
	, nDeadUnit(0)
	, nDeadSpec(0)
	, nDefultExposure(0)
	, nDefultGain(0)
	, nDeadType(0)
	, bSaveImage(FALSE)
	, ErrorCode(0)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LeftSL, nLeftSL);
	DDX_Text(pDX, IDC_RightSL, nRightSL);
	DDX_Text(pDX, IDC_TopSL, nTopSL);
	DDX_Text(pDX, IDC_BottomSL, nBottomSL);
	DDX_Text(pDX, IDC_AreaWidth, nAreaWidth);
	DDX_Text(pDX, IDC_AreaHeight, nAreaHeight);
	DDX_Text(pDX, IDC_DeadUnit, nDeadUnit);
	DDX_Text(pDX, IDC_DeadSpec, nDeadSpec);
	DDX_Text(pDX, IDC_DefultExposure, nDefultExposure);
	DDX_Text(pDX, IDC_DefultGain, nDefultGain);
	DDX_Text(pDX, IDC_DeadType, nDeadType);
	DDX_Check(pDX, IDC_SaveImage, bSaveImage);
	DDX_Text(pDX, IDC_ERROR_CODE, ErrorCode);
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