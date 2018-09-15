// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "POG.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "POG_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, POG_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;

	m_pogCamera0saveImage = TRUE;
	m_pogCamera0saveLog = TRUE;
	m_pogCamera0medianA = "25";
	m_pogCamera0medianB = "5";
	m_pogCamera0axStart = "50";
	m_pogCamera0ayStart = "50";
	m_pogCamera0offset = "7";
	m_pogCamera0cornerDiff = "150";
	m_pogCamera0edgeLimit = "7";
	m_pogCamera0threshold = "20";
	m_pogCamera0pixelLimit = "20";
	m_pogCamera0axEnd = "350";
	m_pogCamera0ayEnd = "250";
	m_pogCamera0intensity = "6";
	m_pogCamera0rangeSpc = "150";
	m_pogCamera0edgePixelLimit = "9";

	m_pogCamera0Enable = FALSE;

	m_pogCamera1saveImage = TRUE;
	m_pogCamera1saveLog = TRUE;
	m_pogCamera1medianA = "25";
	m_pogCamera1medianB = "5";
	m_pogCamera1axStart = "50";
	m_pogCamera1ayStart = "50";
	m_pogCamera1offset = "7";
	m_pogCamera1cornerDiff = "150";
	m_pogCamera1edgeLimit = "7";
	m_pogCamera1threshold = "20";
	m_pogCamera1pixelLimit = "20";
	m_pogCamera1axEnd = "350";
	m_pogCamera1ayEnd = "250";
	m_pogCamera1intensity = "6";
	m_pogCamera1rangeSpc = "150";
	m_pogCamera1edgePixelLimit = "9";
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_POG_CAMERA0_SAVE_IMAGE, m_pogCamera0saveImage);
	DDX_Check(pDX, IDC_CHECK_POG_CAMERA0_SAVE_LOG, m_pogCamera0saveLog);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_MEDIAN_A, m_pogCamera0medianA);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_MEDIAN_B, m_pogCamera0medianB);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_AX_START, m_pogCamera0axStart);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_AY_START, m_pogCamera0ayStart);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_OFFSET, m_pogCamera0offset);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_CORNER_DIFF, m_pogCamera0cornerDiff);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_EDGE_LIMIT, m_pogCamera0edgeLimit);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_THRESHOLD, m_pogCamera0threshold);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_PIXEL_LIMIT, m_pogCamera0pixelLimit);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_AX_END, m_pogCamera0axEnd);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_AY_END, m_pogCamera0ayEnd);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_INTENSITY, m_pogCamera0intensity);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_RANGE_SPC, m_pogCamera0rangeSpc);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA0_EDGE_PIXEL_LIMIT, m_pogCamera0edgePixelLimit);

	DDX_Check(pDX, IDC_CHECK_POG_CAMERA0_ENABLE, m_pogCamera0Enable);

	DDX_Check(pDX, IDC_CHECK_POG_CAMERA1_SAVE_IMAGE, m_pogCamera1saveImage);
	DDX_Check(pDX, IDC_CHECK_POG_CAMERA1_SAVE_LOG, m_pogCamera1saveLog);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_MEDIAN_A, m_pogCamera1medianA);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_MEDIAN_B, m_pogCamera1medianB);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_AX_START, m_pogCamera1axStart);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_AY_START, m_pogCamera1ayStart);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_OFFSET, m_pogCamera1offset);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_CORNER_DIFF, m_pogCamera1cornerDiff);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_EDGE_LIMIT, m_pogCamera1edgeLimit);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_THRESHOLD, m_pogCamera1threshold);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_PIXEL_LIMIT, m_pogCamera1pixelLimit);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_AX_END, m_pogCamera1axEnd);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_AY_END, m_pogCamera1ayEnd);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_INTENSITY, m_pogCamera1intensity);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_RANGE_SPC, m_pogCamera1rangeSpc);
	DDX_Text(pDX, IDC_EDIT_POG_CAMERA1_EDGE_PIXEL_LIMIT, m_pogCamera1edgePixelLimit);
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