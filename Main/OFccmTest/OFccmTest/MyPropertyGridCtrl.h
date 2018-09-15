#pragma once
#include "ConfigManage.h"
 
// MyPropertyGridCtrl

class MyPropertyGridCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(MyPropertyGridCtrl)

public:
	MyPropertyGridCtrl(int CamID);
	virtual ~MyPropertyGridCtrl();
	void AddSensorTab();
	void OnSensorTabPropertyChanged(CMFCPropertyGridProperty* pProp)const;
	  
	void AddVCMDriver();
	void OnVcmDriverPropertyChanged(CMFCPropertyGridProperty* pProp)const;

	void AddOtpSensor();
	void OnOtpSensorPropertyChanged(CMFCPropertyGridProperty* pProp)const;

	void AddRolongoSetting();
	void OnRolongoPropertyChanged(CMFCPropertyGridProperty* pProp)const;

	 
	void Show();
	void OnPropertyChanged(CMFCPropertyGridProperty* pProp)const;
	void Save();

	CMFCPropertyGridProperty* AddMyProperty(LPCTSTR name, int data, LPCTSTR description, DWORD_PTR dataType);
	CMFCPropertyGridProperty* AddMyProperty(int name, int data, LPCTSTR description, DWORD_PTR dataType); 

	SensorTab* pMySensorTab;
	OtpSensorTab* m_OtpSensor;
	int VCM_SlaveID;
	 

	int CamID;
private:
	void InitHead();
	
protected:
	DECLARE_MESSAGE_MAP()
};


