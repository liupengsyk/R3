// CamCodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "CamCodeDlg.h"
#include "afxdialogex.h"


// CamCodeDlg 对话框

IMPLEMENT_DYNAMIC(CamCodeDlg, CDialogEx)

CamCodeDlg::CamCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CamCodeDlg::IDD, pParent)
{
	CamCode =_T("");
}

CamCodeDlg::~CamCodeDlg()
{

}

void CamCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_CamCode,CamCode);
	DDX_Text(pDX,IDC_CamCode,CamRunChard);
	
}


BEGIN_MESSAGE_MAP(CamCodeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CamCodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CamCodeDlg 消息处理程序
BOOL CamCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_CamCode)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CamCodeDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
