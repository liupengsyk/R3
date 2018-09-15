// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RX_OTPCheck.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "RX_OTPCheck_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, RX_OTPCheck_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_uErrorCode = 0;
	m_BufferSize = _T("2000");
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ProjectName, m_ProjectName);

	DDX_Text(pDX, IDC_ErrorCode, m_uErrorCode);
	DDX_Radio(pDX, IDC_RADIO1, m_bModuleSelection);
	DDX_Text(pDX, IDC_BufferSize, m_BufferSize);
	DDV_MaxChars(pDX, m_BufferSize, 4);

	DDX_Radio(pDX, IDC_AF, m_AFStation);
// 	DDX_Radio(pDX, IDC_OTP, m_OTPStation);
// 	DDX_Radio(pDX, IDC_PDAF, m_PDAFStation);
// 	DDX_Radio(pDX,IDC_AEC,m_AECStation);
// 	DDX_Radio(pDX, IDC_Cal, m_CalStation);
// 	DDX_Radio(pDX, IDC_FinalTest, m_FinalTestStation);
	
	DDX_Check(pDX, IDC_SaveEEPROMPDAF,SaveEEPROMPDAF_En);
    

}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_LoadSetting, &OptionDlg::OnBnClickedLoadsetting)

    ON_BN_CLICKED(IDC_CHECK1, &OptionDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_SaveEEPROMPDAF, &OptionDlg::OnBnClickedSaveeeprompdaf)
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


void OptionDlg::OnBnClickedLoadsetting()
{
	// TODO: 在此添加控件通知处理程序代码
	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == 1)
	{
		m_bModuleSelection = FALSE;
	}
	else
	{
		m_bModuleSelection = TRUE;
	}
	UpdateData();
	m_pTestItem->LoadOption();
}


void OptionDlg::OnBnClickedCheck1()
{
    // TODO: 在此添加控件通知处理程序代码
}


void OptionDlg::OnBnClickedSaveeeprompdaf()
{
    // TODO: 在此添加控件通知处理程序代码
}
