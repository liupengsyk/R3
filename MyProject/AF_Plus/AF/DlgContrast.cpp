// DlgContrast.cpp : 实现文件
//

#include "stdafx.h"
#include "AF.h"
#include "DlgContrast.h"
#include "afxdialogex.h"


// CDlgContrast 对话框

IMPLEMENT_DYNAMIC(CDlgContrast, CDialogEx)

CDlgContrast::CDlgContrast(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgContrast::IDD, pParent)
{

}

CDlgContrast::~CDlgContrast()
{
}

void CDlgContrast::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgContrast, CDialogEx)
END_MESSAGE_MAP()


// CDlgContrast 消息处理程序
