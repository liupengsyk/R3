// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SaveImg.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "SaveImg_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, SaveImg_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_strFileName(_T(""))	
{
	m_pTestItem = pTestItem; 
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_FILENAME,m_strFileName);
	DDX_Text(pDX,IDC_FILEPATH,m_pTestItem->FilePath);
	DDX_Text(pDX,IDC_COMBOIMGTYPE,m_pTestItem->ImgType);
 
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
	CComboBox* imgType = (CComboBox*) GetDlgItem(IDC_COMBOIMGTYPE);
	imgType->AddString(_T(".jpg"));	
	imgType->AddString(_T(".bmp"));
	imgType->AddString(_T(".raw"));
	imgType->AddString(_T(".raw8"));
	imgType->AddString(_T("HwHDC"));
	imgType->AddString(_T("HwMMI")); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}