#pragma once

#define WM_UPDATE_SETTING WM_USER + 0x2000
#define WM_UPDATE_SETTING WM_USER + 0x2001

#include "OFccmDef.h"

class ccmBaseInterface;

class Device_Interface
{
public:
	Device_Interface(void);
	~Device_Interface(void);
	virtual int Device_Load() = 0;
	virtual int Device_Init() = 0;
	virtual int Device_Open() = 0;
	virtual int Device_Close() = 0;
	virtual int Device_GetImg() = 0;
	virtual int Device_ReadIIC() = 0;
	virtual int Device_WriteIIC() = 0;

	virtual int LoadSetting();
	virtual int SaveSetting();

	virtual LPCTSTR GetReportHeads() ;
	virtual LPCTSTR GetReportHightLimit();
	virtual LPCTSTR GetReportLowLimit();

	virtual LPCTSTR GetReportContents();


	void SetName(LPCTSTR name);
	LPCTSTR GetName();

	void SetOptionDlgHandle(HWND OptionHwnd);
	HWND GetOptionDialogHandle();

	void SetResult(int Result);	
	int GetResult();

	int GetCamID();
	void SetCamID(int CamID);

	void SetConfigPath(LPCTSTR ConfigPathName);
	int ReadConfigInt(LPCTSTR KeyName,int int_default);
	CString ReadConfigString(LPCTSTR keyName,LPCTSTR strDefault);
	void WriteConfigInt(LPCTSTR KeyName,int int_default,int datatype = DATA_DEC);
	void WriteConfigString(LPCTSTR KeyName,LPCTSTR strDefault);
	ccmBaseInterface  *m_pInterface;

private:
	int     CamID;
	int     m_Result;
	CString m_Name;
	CString m_configPathName; 
	HWND    m_hOptionDlg;

};

