// MyCurrentSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyCurrentSetting.h"



// MyCurrentSetting

IMPLEMENT_DYNAMIC(MyCurrentSetting, CMFCPropertyPage)

MyCurrentSetting::MyCurrentSetting(int CamID)
  : CMFCPropertyPage(MyCurrentSetting::IDD)
  , m_dbStandByPowerMax(0)
  , m_dbStandByPowerMin(0)
{
	this->CamID = CamID;
}

MyCurrentSetting::~MyCurrentSetting()
{

}
 
// MyCurrentSetting 消息处理程序

void MyCurrentSetting::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	//	DDX_Control(pDX, IDC_WorkingCurrent, m_WorkingCurrentList);	 
	//	DDX_Check(pDX, IDC_WorkingCheck, m_CheckWorkingCurrent);

	DDX_Control(pDX, IDC_StandbyCurrent, m_StandbyCurentList);	 
	DDX_Check(pDX, IDC_StandbyCheck, m_CheckStandbyCurent);
	DDX_Text(pDX, IDC_EDIT_STANDBY_POWER_MAX, m_dbStandByPowerMax);
	DDX_Text(pDX, IDC_EDIT_STANDBY_POWER_MIN, m_dbStandByPowerMin);
}


BEGIN_MESSAGE_MAP(MyCurrentSetting, CMFCPropertyPage)
// 	ON_BN_CLICKED(IDC_WorkingCheck, &MyCurrentSetting::OnBnClickedCheckWorking)
	ON_BN_CLICKED(IDC_StandbyCheck, &MyCurrentSetting::OnBnClickedCheckStandby)
END_MESSAGE_MAP()

BOOL MyCurrentSetting::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();
	 
 //	WorkingCurrent = pCcmCtrl->Cam[CamID].pConfig->GetWorkingCurrentTab();	 
	StandbyCurrent = pCcmCtrl->Cam[CamID].pConfig->GetStandbyCurrentTab(); 

 //	m_CheckWorkingCurrent= WorkingCurrent->Test_en;	 
	m_CheckStandbyCurent= StandbyCurrent->Test_en;
	m_dbStandByPowerMax = StandbyCurrent->dbStandByPowerMax;
	m_dbStandByPowerMin = StandbyCurrent->dbStandByPowerMin;

	
 //	m_WorkingCurrentList.ShowCurrentTab(WorkingCurrent);
	m_StandbyCurentList.ShowCurrentTab(StandbyCurrent);
	UpdateData(FALSE);
	return TRUE;
}
 
void MyCurrentSetting::SavePage()
{ 
	UpdateData(TRUE);
 
//	m_WorkingCurrentList.Save(*WorkingCurrent);
//	WorkingCurrent->Test_en = m_CheckWorkingCurrent;
//	*(pCcmCtrl->Cam[CamID].pConfig->GetWorkingCurrentTab()) = *WorkingCurrent;

	m_StandbyCurentList.Save(*StandbyCurrent);
	StandbyCurrent->Test_en = m_CheckStandbyCurent;
	StandbyCurrent->dbStandByPowerMax = m_dbStandByPowerMax;
	StandbyCurrent->dbStandByPowerMin = m_dbStandByPowerMin;
	*(pCcmCtrl->Cam[CamID].pConfig->GetStandbyCurrentTab()) = *StandbyCurrent;
}

 
// void MyCurrentSetting::OnBnClickedCheckWorking()
// {
// 	UpdateData(TRUE);
// 	if (m_CheckWorkingCurrent)
// 	{
// 		m_WorkingCurrentList.EnableWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_WorkingCurrentList.EnableWindow(FALSE);
// 	}
// }
void MyCurrentSetting::OnBnClickedCheckStandby()
{
	UpdateData(TRUE);
	if (m_CheckStandbyCurent)
	{
		m_StandbyCurentList.EnableWindow(TRUE);
	}
	else
	{
		m_StandbyCurentList.EnableWindow(FALSE);
	}
}


BOOL MyCurrentSetting::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if (pCcmCtrl->IsUSBlock())
	{
		EnableWindow(TRUE);
		GetDlgItem(IDC_strShow)->ShowWindow(SW_HIDE);
	}
	else 
	{
		EnableWindow(FALSE);
		GetDlgItem(IDC_strShow)->ShowWindow(SW_SHOW);
	}

	return CMFCPropertyPage::OnSetActive();
}
