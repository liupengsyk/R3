// MachineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MachineDlg.h"
#include "MachineInterfaceLib.h"
#include "afxdialogex.h"


// MachineDlg 对话框

IMPLEMENT_DYNAMIC(MachineDlg, CDialogEx)

MachineDlg::MachineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MachineDlg::IDD, pParent)
{
	 
}

MachineDlg::~MachineDlg()
{

}

void MachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MachineSetTab, m_MyTabSheet);
}


BEGIN_MESSAGE_MAP(MachineDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MachineDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// MachineDlg 消息处理程序
BOOL MachineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_MyTabSheet.Init(); 

	//加载machine的属性页
	MachineInterface* pMachine = pCcmCtrl->pMachineCtrl->p_machine;

	LPCTSTR name = pMachine->GetName();
	HWND    hDlg = pMachine->GetDlgHandle();
	if(hDlg!=NULL)
	{
		m_MyTabSheet.AddPage(name,hDlg);

		m_MyTabSheet.SetCurSel(0);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void MachineDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int ret = pCcmCtrl->pMachineCtrl->SaveMachineSetting();
	if (ret>0)
	{

	}
	CDialogEx::OnOK();
}

void MachineDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	/*	delete this;*/
	CDialogEx::PostNcDestroy();
}