// MyErrorCode.cpp : implementation file
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyErrorCode.h"
#include "afxdialogex.h"


// MyErrorCode dialog

IMPLEMENT_DYNAMIC(MyErrorCode, CPropertyPage)

MyErrorCode::MyErrorCode(int CamID)
	: CPropertyPage(MyErrorCode::IDD)
{
	this->CamID=CamID;
}

MyErrorCode::~MyErrorCode()
{
}

void MyErrorCode::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, ErrorCode_en);

	DDX_Text(pDX, IDC_strSocket0, strSocket[0]);
	DDX_Text(pDX, IDC_strSocket1, strSocket[1]);
	DDX_Text(pDX, IDC_strSocket2, strSocket[2]);
	DDX_Text(pDX, IDC_strSocket3, strSocket[3]);
	DDX_Text(pDX, IDC_strSocket4, strSocket[4]);
	DDX_Text(pDX, IDC_strSocket5, strSocket[5]);
	DDX_Text(pDX, IDC_strSocket6, strSocket[6]);
	DDX_Text(pDX, IDC_strSocket7, strSocket[7]);
	DDX_Text(pDX, IDC_strSocket8, strSocket[8]);
	DDX_Text(pDX, IDC_strSocket9, strSocket[9]);
	DDX_Text(pDX, IDC_strSocket10, strSocket[10]);
	DDX_Text(pDX, IDC_strSocket11, strSocket[11]);
	DDX_Text(pDX, IDC_strSocket12, strSocket[12]);
	DDX_Text(pDX, IDC_strSocket13, strSocket[13]);
	DDX_Text(pDX, IDC_strSocket14, strSocket[14]);

	DDX_Text(pDX, IDC_strTestID0, strTestID[0]);
	DDX_Text(pDX, IDC_strTestID1, strTestID[1]);
	DDX_Text(pDX, IDC_strTestID2, strTestID[2]);
	DDX_Text(pDX, IDC_strTestID3, strTestID[3]);
	DDX_Text(pDX, IDC_strTestID4, strTestID[4]);
	DDX_Text(pDX, IDC_strTestID5, strTestID[5]);
	DDX_Text(pDX, IDC_strTestID6, strTestID[6]);
	DDX_Text(pDX, IDC_strTestID7, strTestID[7]);
	DDX_Text(pDX, IDC_strTestID8, strTestID[8]);
	DDX_Text(pDX, IDC_strTestID9, strTestID[9]);
	DDX_Text(pDX, IDC_strTestID10, strTestID[10]);
	DDX_Text(pDX, IDC_strTestID11, strTestID[11]);
	DDX_Text(pDX, IDC_strTestID12, strTestID[12]);
	DDX_Text(pDX, IDC_strTestID13, strTestID[13]);
	DDX_Text(pDX, IDC_strTestID14, strTestID[14]);
}


BEGIN_MESSAGE_MAP(MyErrorCode, CPropertyPage)
END_MESSAGE_MAP()

void MyErrorCode::SavePage()
{
	UpdateData(TRUE);
	pCcmCtrl->Cam[CamID].pConfig->GetErrorCodeTab()->ErrorCode_en=ErrorCode_en;
	for(int i=0;i<15;i++)
	{
		pCcmCtrl->Cam[CamID].pConfig->GetErrorCodeTab()->strSocket[i]=strSocket[i];
		pCcmCtrl->Cam[CamID].pConfig->GetErrorCodeTab()->strTestID[i]=strTestID[i];

	}
}
// MyErrorCode message handlers


BOOL MyErrorCode::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	ErrorCode_en=pCcmCtrl->Cam[CamID].pConfig->GetErrorCodeTab()->ErrorCode_en;
	for(int i=0;i<15;i++)
	{
		strSocket[i]=pCcmCtrl->Cam[CamID].pConfig->GetErrorCodeTab()->strSocket[i];
		strTestID[i]=pCcmCtrl->Cam[CamID].pConfig->GetErrorCodeTab()->strTestID[i];

	}
	UpdateData(FALSE);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
