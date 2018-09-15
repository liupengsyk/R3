#include "StdAfx.h"
#include "ConfigFileRw.h"


ConfigFileRw::ConfigFileRw(void)
{
}


ConfigFileRw::~ConfigFileRw(void)
{
}

LPCTSTR ConfigFileRw::GetConfigTitle()
{
	int start = m_ConfigFilePathName.ReverseFind(_T('\\')) + 1;
	int end   = m_ConfigFilePathName.ReverseFind(_T('.'));
	m_ConfigFileTitle = m_ConfigFilePathName.Mid(start, end - start);
	return LPCTSTR(m_ConfigFileTitle);
}

void ConfigFileRw::SetConfigFilePath(CString m_ConfigFilePathName)
{
	this->m_ConfigFilePathName = m_ConfigFilePathName;
}

BOOL ConfigFileRw::WriteConfigData(CString sSection, CString sSectionKey, int nValue)
{
	CString  sValue;
	sValue.Format(_T("%d"),nValue);
	return WritePrivateProfileString(sSection,sSectionKey,sValue,m_ConfigFilePathName);
}
  
BOOL ConfigFileRw::WriteConfigDataHex(CString sSection, CString sSectionKey, long nValue)
{
	CString sTmp;
	sTmp.Format(_T("0x%x"), nValue);
	return WritePrivateProfileString(sSection,sSectionKey,sTmp,m_ConfigFilePathName);
}

void ConfigFileRw::WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype)
{
	CString szTemp;
	szTemp.Format(_T("%d"), data); 
	WritePrivateProfileString(AppName, KeyName, szTemp, m_ConfigFilePathName);
}

void ConfigFileRw::WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName,LPCTSTR string)
{
	WritePrivateProfileString(AppName,KeyName,string,m_ConfigFilePathName);
}
int ConfigFileRw::ReadConfigData(CString sSection,CString sSectionKey,int nDefault)
{
	return GetPrivateProfileInt(sSection,sSectionKey,nDefault,m_ConfigFilePathName);
}
 
long ConfigFileRw::ReadConfigDataHex(CString sSection,CString sSectionKey,long nDefault)
{
	CString sTemp;
	GetPrivateProfileString(sSection,sSectionKey,_T(""), sTemp.GetBuffer(MAX_PATH), MAX_PATH,m_ConfigFilePathName);
	sTemp.ReleaseBuffer();
	char* pAddr = (char*)sTemp.GetBuffer(MAX_PATH); 
	long x = sTemp.IsEmpty() ? nDefault : strtoul( pAddr, NULL, 16); 
	sTemp.ReleaseBuffer();
	return x;
}
 
LPCTSTR ConfigFileRw::ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName,LPCTSTR default)
{
	static CString szTemp;
	GetPrivateProfileString(AppName, KeyName, default, szTemp.GetBuffer(255), 255, m_ConfigFilePathName);
	szTemp.ReleaseBuffer();
	return szTemp;
}

void ConfigFileRw::ClearConfig(LPCTSTR AppName )
{
	WritePrivateProfileSection(AppName,0, m_ConfigFilePathName);
}

void ConfigFileRw::ClearConfig(LPCTSTR AppName, LPCTSTR KeyName)
{
	WritePrivateProfileString(AppName, KeyName, 0, m_ConfigFilePathName);
}
