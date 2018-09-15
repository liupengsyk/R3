// MyTestPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyTestPanel.h"
#include "afxdialogex.h"


// MyTestPanel 对话框

IMPLEMENT_DYNAMIC(MyTestPanel, CDialogEx)

MyTestPanel::MyTestPanel(int CamID,CWnd* pParent /*=NULL*/)
	: CDialogEx(MyTestPanel::IDD, pParent)
{
	this->CamID = CamID;
}

MyTestPanel::~MyTestPanel()
{

}

void MyTestPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MyTestPanel, CDialogEx)
END_MESSAGE_MAP()



// MyTestPanel 消息处理程序

BOOL MyTestPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetBackgroundColor(RGB(250,250,232));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void MyTestPanel::EnableButton(BOOL bOn)
{
	pCcmCtrl->Cam[CamID].pTestCtrl->EnableButton(bOn);	

}
 
void MyTestPanel::ResetAllButtonImage(void)
{
	pCcmCtrl->Cam[CamID].pTestCtrl->ResetALLButtonImage();
}
