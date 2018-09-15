// GeneralSettingPage.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "GeneralSettingPage.h"
#include "afxdialogex.h"


// GeneralSettingPage 对话框

IMPLEMENT_DYNAMIC(GeneralSettingPage, CMFCPropertyPage)

GeneralSettingPage::GeneralSettingPage(int CamID)
	: CMFCPropertyPage(GeneralSettingPage::IDD)	 
	, strTestMode(_T(""))
	, m_strIP(_T(""))
	, m_iPort(0)
	, m_bSaveReportEn(FALSE)
	,sReportPath(L"")
{ 
	this->CamID = CamID;
	SocketEn = FALSE; 
	bUseLocalExp = FALSE;

}

GeneralSettingPage::~GeneralSettingPage()
{

}

void GeneralSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	 

	DDX_Text(pDX, IDC_bTestMode, strTestMode);

	DDX_Check(pDX, IDC_bForceShow, bForceShow); 

	DDX_Text(pDX, IDC_strIP, m_strIP);
	DDX_Text(pDX, IDC_iPort, m_iPort);
	DDX_Check(pDX, IDC_SocektEn, SocketEn);


	DDX_Check(pDX, IDC_CHECK2, m_bSaveReportEn);

	DDX_Check(pDX,IDC_bUseLocalExp,bUseLocalExp); 

	DDX_Text(pDX,IDC_DestListCOB,DevName);

	DDX_Text(pDX,IDC_EDIT_REPORT_PATH,sReportPath);
	
	 
}


BEGIN_MESSAGE_MAP(GeneralSettingPage, CMFCPropertyPage) 	 
	ON_BN_CLICKED(IDC_UpDataReportPath, &GeneralSettingPage::OnBnClickedUpdatareportpath)
END_MESSAGE_MAP()


// GeneralSettingPage 消息处理程序
BOOL GeneralSettingPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
   
	GetbCheckIni();

	CComboBox *pTestModeSel =(CComboBox*)GetDlgItem(IDC_bTestMode);
	pTestModeSel->AddString(_T("手动模式"));	
	pTestModeSel->AddString(_T("自动模式"));
	pTestModeSel->AddString(_T("机器模式"));
	pTestModeSel->AddString(_T("AA模式"));

	pTestModeSel = NULL;

	CComboBox *pDevList =(CComboBox*)GetDlgItem(IDC_DestListCOB);
	vector<CString> devList = pCcmCtrl->pDevice->GetDevList();
	for (UINT i=0;i<devList.size();i++)
	{
		pDevList->AddString(devList[i]);	
	}	 
	pDevList = NULL;
	return TRUE;
}

 

void GeneralSettingPage::SavePage()
{ 
	UpdateData(TRUE);
	 
	 
	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->m_TestMode = GetTestMode(strTestMode);

	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->RemoteIP  = m_strIP;
	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->iPort = m_iPort  ;
	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->SocketEn = SocketEn  ;
 
	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->SaveReport_EnLocal = m_bSaveReportEn  ;
	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->bUseLocalExp = bUseLocalExp ;
	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->bForceShow = bForceShow ; 
 
	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->DevName = DevName;

	pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->sReportPath = sReportPath;
	 
}
 
 
void GeneralSettingPage::GetbCheckIni()
{  
	  
	 strTestMode = GetTestMode(  pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->m_TestMode);
	 m_strIP = pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->RemoteIP ;
	 m_iPort = pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->iPort ;
	 SocketEn = pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->SocketEn ;

	 m_bSaveReportEn = pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->SaveReport_EnLocal ;
	 bUseLocalExp = pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->bUseLocalExp  ;
	 bForceShow = pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->bForceShow  ;
	 
	 DevName = pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->DevName;

	 sReportPath = pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->sReportPath;
	 
	
	  
	 UpdateData(FALSE); //fase 为向dlg传递数据
}
 
void GeneralSettingPage::OnBnClickedUpdatareportpath()
{
	// TODO: 在此添加控件通知处理程序代码
	pCcmCtrl->Cam[CamID].pConfig->pConfigFileRw->ClearConfig(_T("GENERAL_SETTING"),_T("sReportPath"));
}
//后续使用map进行改善
CString GeneralSettingPage::GetTestMode(int m_TestMode)
{
	CString str;
	switch(m_TestMode)
	{
	case 0 :
		str =_T("手动模式");
		break;
	case 1:
		str = _T("自动模式");
		break;
	case 2 :
		str = _T("机器模式");
		break;
	case 3 :
		str = _T("AA模式");
		break;
	default :
		str = _T("手动模式");
		break;
	}
	return str;
}

int GeneralSettingPage::GetTestMode(CString strTestMode)
{  
	
	if(strTestMode ==_T("自动模式"))
	{
		return 1;
	}
	else if(strTestMode == _T("机器模式"))
	{
		return 2;
	}
	else if(strTestMode == _T("AA模式"))
	{
		return 3;
	}
	else
	{
		return 0;
	}
	 
}