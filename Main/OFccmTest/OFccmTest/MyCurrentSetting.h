#pragma once
#include "MyCurrentListCtrl.h"

// MyCurrentSetting

class MyCurrentSetting : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(MyCurrentSetting)

public:
	MyCurrentSetting(int CamID);
	virtual ~MyCurrentSetting();

	// 对话框数据
	enum { IDD = IDD_CurrentTest };

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg BOOL OnInitDialog();

	 
public:

	void SavePage();
	void ApplyPage();
	 

	   
//	BOOL m_CheckWorkingCurrent;
//	MyCurrentListCtrl m_WorkingCurrentList;
//	CurrentTab* WorkingCurrent;

	BOOL m_CheckStandbyCurent;
	MyCurrentListCtrl m_StandbyCurentList;
	CurrentTab* StandbyCurrent;
	 
//	afx_msg void OnBnClickedCheckWorking();	
	afx_msg void OnBnClickedCheckStandby();
	int CamID;
	 
	
	virtual BOOL OnSetActive();
	double m_dbStandByPowerMax;
	double m_dbStandByPowerMin;
};


