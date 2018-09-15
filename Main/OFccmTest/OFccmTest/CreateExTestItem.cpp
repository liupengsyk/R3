// CreateExTestItem.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "CreateExTestItem.h"
#include "afxdialogex.h"


// CreateExTestItem �Ի���

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


// CreateExTestItem ��Ϣ�������


void CreateExTestItem::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bUsed = TRUE;
	CDialogEx::OnOK();
}


void CreateExTestItem::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bUsed = FALSE;
	CDialogEx::OnCancel();
}
