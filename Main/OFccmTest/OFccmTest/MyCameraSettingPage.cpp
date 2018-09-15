// MyCaneraSettingPage.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyCameraSettingPage.h"
#include "afxdialogex.h"


// MyCaneraSettingPage 对话框

IMPLEMENT_DYNAMIC(MyCameraSettingPage, CMFCPropertyPage)

MyCameraSettingPage::MyCameraSettingPage(int CamID)
	: CMFCPropertyPage(MyCameraSettingPage::IDD)
{
	this->CamID = CamID;
	m_OSList = new MyOSListCtrl(CamID);
	pProGridCtrl = new MyPropertyGridCtrl(CamID);
}

MyCameraSettingPage::~MyCameraSettingPage()
{
	SAFE_DELETE_OBJECT(m_OSList);
	SAFE_DELETE_OBJECT(pProGridCtrl);
}

void MyCameraSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OS_List, *m_OSList);	 
	DDX_Check(pDX, IDC_OSCheck, m_CheckOS);
	DDX_Check(pDX, IDC_OSCheck2, m_CheckPositiveOS);
	DDX_Check(pDX, IDC_OSCheck3, m_CheckSensorpinOS);
}


BEGIN_MESSAGE_MAP(MyCameraSettingPage, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_OSCheck, &MyCameraSettingPage::OnBnClickedCheckOs)
	ON_BN_CLICKED(IDC_OSCheck2, &MyCameraSettingPage::OnBnClickedCheckOs)
	ON_BN_CLICKED(IDC_OSCheck3, &MyCameraSettingPage::OnBnClickedCheckOs)
END_MESSAGE_MAP()

BOOL MyCameraSettingPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();    
	 
	CRect rect;
	 
	m_CheckOS = pCcmCtrl->Cam[CamID].pConfig->GetOSTab()->OS_en;
	m_CheckPositiveOS=pCcmCtrl->Cam[CamID].pConfig->GetOSTab()->PositiveOS_en;
	m_CheckSensorpinOS=pCcmCtrl->Cam[CamID].pConfig->GetOSTab()->SensorpinOS_en;
	GetDlgItem(IDC_GridShow)->GetClientRect(&rect);
	GetDlgItem(IDC_GridShow)->MapWindowPoints(this,&rect);	
	pProGridCtrl->Create(WS_CHILD|WS_BORDER|WS_TABSTOP|WS_VISIBLE,rect,this,2212);
	pProGridCtrl->Show();

	UpdateData(FALSE);

	return TRUE;
}

 
void MyCameraSettingPage::SavePage()
{
	pProGridCtrl->Save();
	m_OSList->Save();
	pCcmCtrl->Cam[CamID].pConfig->GetOSTab()->OS_en = m_CheckOS;
	pCcmCtrl->Cam[CamID].pConfig->GetOSTab()->PositiveOS_en=m_CheckPositiveOS;
	pCcmCtrl->Cam[CamID].pConfig->GetOSTab()->SensorpinOS_en=m_CheckSensorpinOS;
}

void MyCameraSettingPage::OnBnClickedCheckOs()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_CheckOS||m_CheckPositiveOS||m_CheckSensorpinOS)
	{
		m_OSList->EnableWindow(TRUE);
	}
	else
	{
		m_OSList->EnableWindow(FALSE);
	}
}
void MyCameraSettingPage::ApplyPage()
{

}
void MyCameraSettingPage::InitPage()
{

}


BOOL MyCameraSettingPage::OnSetActive()
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
