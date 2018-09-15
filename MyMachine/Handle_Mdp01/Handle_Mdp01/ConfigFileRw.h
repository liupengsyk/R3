#pragma once
#include "stdafx.h" 
#include <vector>
using namespace std;

class ConfigFileRw
{
public:
	ConfigFileRw(void);
	~ConfigFileRw(void);

	LPCTSTR GetConfigTitle(); 
	void SetConfigFilePath(CString m_ConfigFilePathName);
private:

	CString m_ConfigFilePathName; 	 
	CString m_ConfigFileTitle;

public:

	/******¶ÁÐ´»ù±¾²Ù×÷******/
	BOOL WriteConfigData(CString sSection, CString sSectionKey, int nValue); 
	BOOL WriteConfigDataHex(CString sSection, CString sSectionKey, long nValue);
	void WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = 0);

	void WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName,LPCTSTR string);

	int ReadConfigData(CString sSection,CString sSectionKey,int nDefault);
	long ReadConfigDataHex(CString sSection,CString sSectionKey,long nDefault);
	
	LPCTSTR ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName,LPCTSTR default);

	void ClearConfig(LPCTSTR AppName);
	void ClearConfig(LPCTSTR AppName, LPCTSTR KeyName);
};

