// MyTestPanel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyTestPanel.h"
#include "afxdialogex.h"


// MyTestPanel �Ի���

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



// MyTestPanel ��Ϣ�������

BOOL MyTestPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBackgroundColor(RGB(250,250,232));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void MyTestPanel::EnableButton(BOOL bOn)
{
	pCcmCtrl->Cam[CamID].pTestCtrl->EnableButton(bOn);	

}
 
void MyTestPanel::ResetAllButtonImage(void)
{
	pCcmCtrl->Cam[CamID].pTestCtrl->ResetALLButtonImage();
}
