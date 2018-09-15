// MyTestItemOptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyTestItemOptionDlg.h"
#include "afxdialogex.h"
#include "MyTestItem.h"


// MyTestItemOptionDlg 对话框

IMPLEMENT_DYNAMIC(MyTestItemOptionDlg, CDialogEx)

MyTestItemOptionDlg::MyTestItemOptionDlg(int CamID,CWnd* pParent /*=NULL*/)
	: CDialogEx(MyTestItemOptionDlg::IDD, pParent)
{
	this->CamID = CamID;
}

MyTestItemOptionDlg::~MyTestItemOptionDlg()
{

}

void MyTestItemOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OptionDlgTab, m_MyTabSheet);
}

 

BEGIN_MESSAGE_MAP(MyTestItemOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_APPLY_SETTING, &MyTestItemOptionDlg::OnBnClickedApplySetting)	 
	ON_BN_CLICKED(IDCANCEL, &MyTestItemOptionDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGING, IDC_OptionDlgTab, &MyTestItemOptionDlg::OnSelchangingOptiondlgtab)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// InspectionOptionDlg 消息处理程序


BOOL MyTestItemOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_MyTabSheet.Init(); 

	//加载DLL的属性页
	vector<MyTestItem>* pArray = pCcmCtrl->Cam[CamID].pTestCtrl->GetMyTestItemArray();

	vector<MyTestItem>::iterator it;
	for(it = pArray->begin(); it != pArray->end(); it++)
	{
		LPCTSTR name = it->pItem->GetName();
		HWND    hDlg = it->pItem->GetOptionDialogHandle();
		m_MyTabSheet.AddPage(name,hDlg);
	}
	m_MyTabSheet.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
 

void MyTestItemOptionDlg::OnBnClickedApplySetting()
{
	// TODO: 在此添加控件通知处理程序代码
	int ret = pCcmCtrl->Cam[CamID].pTestCtrl->SaveAllOption();
	int i=m_MyTabSheet.GetCurSel();
	pCcmCtrl->Cam[CamID].pTestCtrl->VecTestItem[i].pItem->SaveOption();
	if (ret>0)
	{

	}
	CDialogEx::OnOK();
}


 

void MyTestItemOptionDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	

	CDialogEx::OnCancel();
}


void MyTestItemOptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	/*	delete this;*/
	CDialogEx::PostNcDestroy();
}

void MyTestItemOptionDlg::OnSelchangingOptiondlgtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int i=m_MyTabSheet.GetCurSel();
	int ret = pCcmCtrl->Cam[CamID].pTestCtrl->VecTestItem[i].pItem->SaveOption();
	pCcmCtrl->Cam[CamID].pTestCtrl->LoadAllOption();

	if (ret>0)
	{

	}
	*pResult = 0;
}
