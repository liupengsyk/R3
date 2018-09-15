#pragma once
#include "stdafx.h"
#include "imagekit.h"
#include "OFccmDef.h"
#include "ConfigFileRw.h"
 
#include <vector>
using namespace std;
 
class ConfigManage
{
public:
	ConfigManage(void);
	~ConfigManage(void);
private:
	void InitPath(int CamNum=0);
public:
	BOOL bIsLoad; 
	CString GetConfigFilePath();	
	void SetConfigFilePath(CString m_ConfigFilePathName);

private:

	CString m_ConfigFilePathName;
 

	CString strCurrentPath;
	CString DefaultReportPath;
	CString m_ConfigFileTitle;
public:
	ConfigFileRw* pConfigFileRw;
	
public:
	//*******Get****//
	SensorTab* GetSensorTab();
	 
	OSTab* GetOSTab();
	GeneralSettingTab* GetGeneralTab();
//	CurrentTab* GetWorkingCurrentTab();
	CurrentTab* GetStandbyCurrentTab();
	ErrorCodeTab* GetErrorCodeTab();
	vector<CString>* GetDllNamelist(); 
	CString GetVcmDr();
	CString VcmDriver;	
	int VCM_SlaveID;
	OtpSensorTab* GetOtpSensorTab();

private:
	/******基本结构体对象******/
	SensorTab* pMySensorTab;
	vector<CString> m_DllNamelist;
	 
	OSTab m_OStab;
	GeneralSettingTab m_GeneralTab;
	ErrorCodeTab m_ErrorCodeTab;
//	CurrentTab m_WorkingCurrentTab;
	CurrentTab m_StandbyCurrentTab;
	 
	OtpSensorTab m_OtpSensorTab;
public:
	/******各对象的初始化与保存******/
	BOOL LoadConfig(int CamID);
	BOOL SaveConfig();
	
	BOOL LoadSensorGenaralTab();
	BOOL SaveSensorGenaralTab();
	 

	BOOL LoadSensorParalist();
//	BOOL SaveSensorParalist(LPCTSTR AppName,USHORT* paralist,USHORT paralistsize);

	int LoadDllName();
	int WriteDllName();

	BOOL LoadVcmDriver();
	int WriteVcmDriver(CString vcm_driver);

	BOOL LoadOtpSensor();
	int WriteOtpSensor();	 

	int LoadOSTab();
	int WriteOSTab();

	/*int LoadStaticCurrentTab(CurrentTab m_CurrentTab);
	int WriteStaticTabCurrentTab(CurrentTab &m_CurrentTab);*/

	int LoadCurrentTab(CurrentTab &m_CurrentTab,CString strCurrentTab);
	int WriteCurrentTab(CurrentTab m_CurrentTab,CString strCurrentTab);
 

	int LoadGeneralSetting();
	int WriteGeneralSetting(void); 

	int LoadErrorCodeSetting();
	int WriteErrorCodeSetting();

	USHORT*    m_pSensorPara;
	  
public:
	int GetLastExp();
	void SaveExp(int exp);

	int GetLastAgain();
	void SaveAgain(int exp);

		 
	
};

