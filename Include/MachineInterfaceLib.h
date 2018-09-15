#pragma once
#define  WM_UPDATE_SETTING WM_USER+9000
#define WM_KEY_CTRLNUM WM_USER+9001

//#ifndef _AFXDLL
//#define _AFXDLL
//#endif

#include "stdafx.h"
#include "OFccmDef.h"
using namespace std;
class ccmBaseInterface;


class MachineInterface
{
public:
	MachineInterface(void);
	~MachineInterface(void);

	virtual int InitDevice() = 0;
	 
	virtual int LoadSetting();
	virtual int SaveSetting();
	 

	virtual int CameraReturnResult(int CamID,BOOL bResult,int error_type=0);
	 
public:


	int ReadConfigInt(LPCTSTR KeyName,int int_default);
	CString ReadConfigString(LPCTSTR keyName,LPCTSTR strDefault);
	void WriteConfigInt(LPCTSTR KeyName,int int_default,int datatype = DATA_DEC);
	void WriteConfigString(LPCTSTR KeyName,LPCTSTR strDefault);
	void  Config_ClearConfig(LPCTSTR AppName);

	ccmBaseInterface  *m_pInterface;
protected:
	void AddLog(int CamID,CString info,COLORREF clr,int fontSize);

public:
	void SetName(LPCTSTR name);
	LPCTSTR GetName();

	void SetDlgHandle(HWND OptionHwnd);	
	HWND GetDlgHandle();

	void SetCamID(int CamID);
	int GetCamID();

	void SetConfigPath(LPCTSTR ConfigPathName);
	LPCTSTR GetConfigPath();

	void SetMainHwnd(HWND hMainWndHwnd);
	HWND GetMainHwnd();

private:
	CString m_Name;
	CString m_configPathName;
	int     m_Type;
	HWND  m_hSettingDlg;
	HWND  hMainWndHwnd;
	int CamID;
	 
public: 
	void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);
	
};