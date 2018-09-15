// CreateExTestItem.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "CreateExTestItem.h"
#include "afxdialogex.h"


// CreateExTestItem 对话框

IMPLEMENT_DYNAMIC(CreateExTestItem, CDialogEx)

CreateExTestItem::CreateExTestItem(CString ExTestItemName,CWnd* pParent /*=NULL*/)
	: CDialogEx(CreateExTestItem::IDD, pParent)
{
	bUsed = TRUE;
	this->ExTestItemName = ExTestItemName;
}

CreateExTestItem::~CreateExTestItem()
{

}

void CreateExTestItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_ExTestItemName,ExTestItemName);
}


BEGIN_MESSAGE_MAP(CreateExTestItem, CDialogEx)
	ON_BN_CLICKED(IDOK, &CreateExTestItem::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CreateExTestItem::OnBnClickedCancel)
END_MESSAGE_MAP()


// CreateExTestItem 消息处理程序


void CreateExTestItem::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	bUsed = TRUE;
	CDialogEx::OnOK();
}


void CreateExTestItem::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	bUsed = FALSE;
	CDialogEx::OnCancel();
}
