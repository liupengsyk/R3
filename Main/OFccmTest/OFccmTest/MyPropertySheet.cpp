// MyPropertySheet.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyPropertySheet.h"


// MyPropertySheet

IMPLEMENT_DYNAMIC(MyPropertySheet, CMFCPropertySheet)

MyPropertySheet::MyPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage,int CamID)
	:CMFCPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	this->CamID = CamID;
}

MyPropertySheet::MyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage,int CamID)
	:CMFCPropertySheet(pszCaption, pParentWnd, iSelectPage)
{ 	
	this->CamID = CamID;
	pCameraSetPage = new MyCameraSettingPage(CamID);	
	AddPage(pCameraSetPage);

	pMyCurrentSetting = new MyCurrentSetting(CamID);
	AddPage(pMyCurrentSetting);

	pGeneralSettingPage = new GeneralSettingPage(CamID);
	AddPage(pGeneralSettingPage);

	pMyErrorCodeSettingPage=new MyErrorCode(CamID);
	AddPage(pMyErrorCodeSettingPage);
}

MyPropertySheet::~MyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(MyPropertySheet, CPropertySheet)
	ON_COMMAND(ID_APPLY_NOW,&MyPropertySheet::OnApplyNow)
	ON_COMMAND(IDOK,&MyPropertySheet::OnIdOk)
//	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

BOOL MyPropertySheet::OnInitDialog()
{
 
	BOOL bResult = CMFCPropertySheet::OnInitDialog();
	

	GetDlgItem(IDHELP)->ShowWindow(FALSE);
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	GetDlgItem(ID_APPLY_NOW)->SetWindowTextW(_T("保存并退出"));
	SetActivePage(pMyCurrentSetting);
	SetActivePage(pCameraSetPage);
	SetActivePage(pMyErrorCodeSettingPage);
	SetActivePage(pGeneralSettingPage);
	return bResult;
}
// MyPropertySheet 消息处理程序
 
void MyPropertySheet::OnApplyNow()
{
	pCameraSetPage->SavePage();
	pMyCurrentSetting->SavePage();
	pGeneralSettingPage->SavePage();
	pMyErrorCodeSettingPage->SavePage();
	pCcmCtrl->Cam[CamID].pConfig->SaveConfig();
	pCcmCtrl->UnlocktheFile(CamID);
	CMFCPropertySheet::EndDialog(IDOK);
}

void MyPropertySheet::OnIdOk()
{
	pCameraSetPage->SavePage();
	pMyCurrentSetting->SavePage();
	pGeneralSettingPage->SavePage();
	pMyErrorCodeSettingPage->SavePage();
	pCcmCtrl->UnlocktheFile(CamID);
	CMFCPropertySheet::EndDialog(IDOK);
}

