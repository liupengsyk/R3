#pragma once
#include "MyCameraSettingPage.h"
#include "MyCurrentSetting.h"
#include "GeneralSettingPage.h"
#include "MyErrorCode.h"


// MyPropertySheet

class MyPropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(MyPropertySheet)
	
public:
	MyPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,int CamID =0);
	MyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0,int CamID =0);
	virtual ~MyPropertySheet();
	MyCameraSettingPage* pCameraSetPage;
	MyCurrentSetting* pMyCurrentSetting;
	GeneralSettingPage* pGeneralSettingPage;
	MyErrorCode *pMyErrorCodeSettingPage;
	virtual BOOL OnInitDialog();

	afx_msg void OnApplyNow();
	afx_msg void OnIdOk();
	int CamID;

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
};


