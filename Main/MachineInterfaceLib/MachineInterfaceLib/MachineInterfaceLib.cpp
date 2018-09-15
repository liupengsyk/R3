#include "stdafx.h"
#include "ccmBaseInterface.h"
#include "MachineInterfaceLib.h"

MachineInterface::MachineInterface()
{
	m_hSettingDlg = NULL;
}

MachineInterface::~MachineInterface()
{
	m_hSettingDlg = NULL;
}

int MachineInterface::InitDevice() 
{
	return 0;
}
 
int MachineInterface::LoadSetting()
{
	int ret = SendMessage(m_hSettingDlg,WM_UPDATE_SETTING,0,0);
	return ret;
}
int MachineInterface::SaveSetting()
{
	int ret = SendMessage(m_hSettingDlg,WM_UPDATE_SETTING,1,0);
	return ret;
}
 
int MachineInterface::CameraReturnResult(int CamID,BOOL bResult,int error_type)
{
	return 0;
}

void MachineInterface::AddLog(int CamID,CString info,COLORREF clr,int fontSize)
{
	m_pInterface->Ctrl_Addlog(CamID,info + L"machine",clr,fontSize);
}
 
void MachineInterface::SetName(LPCTSTR name)
{	 
	m_Name = name;
}
LPCTSTR MachineInterface::GetName()
{
	return (LPCTSTR)m_Name;
}
 
void MachineInterface::SetDlgHandle(HWND OptionHwnd)
{
	m_hSettingDlg = OptionHwnd;
}
HWND MachineInterface::GetDlgHandle()
{
	return m_hSettingDlg;
}


void MachineInterface::SetMainHwnd(HWND hMainWndHwnd)
{
	this->hMainWndHwnd = hMainWndHwnd;
}
HWND MachineInterface::GetMainHwnd()
{
	return hMainWndHwnd;
}


int MachineInterface::GetCamID()
{
	return CamID;
}
void MachineInterface::SetCamID(int CamID)
{
	this->CamID = CamID;
}


void MachineInterface::SetConfigPath(LPCTSTR ConfigPathName)
{
	m_configPathName = ConfigPathName;
}

LPCTSTR MachineInterface::GetConfigPath()
{
	return (LPCTSTR)m_configPathName;
}

 void MachineInterface::Config_ClearConfig(LPCTSTR AppName)
 {
	 WritePrivateProfileSection(AppName,0, m_configPathName);
 }
int MachineInterface::ReadConfigInt(LPCTSTR KeyName,int int_default)
{
	return GetPrivateProfileInt(m_Name,KeyName,int_default,m_configPathName);
}
CString MachineInterface::ReadConfigString(LPCTSTR keyName,LPCTSTR strDefault)
{
	CString strTemp;
	GetPrivateProfileString(m_Name,keyName,strDefault,strTemp.GetBuffer(255),255,m_configPathName);
	strTemp.ReleaseBuffer();
	return strTemp;

}
void MachineInterface::WriteConfigInt(LPCTSTR KeyName,int data,int datatype)
{
	CString szTemp;
	switch(datatype)
	{
	case DATA_DEC:
		szTemp.Format(_T("%d"),data);
		break;
	case DATA_HEX_WORD:
		szTemp.Format(_T("0x%04x"),data);
		break;
	case DATA_HEX_BYTE:
		szTemp.Format(_T("0x%02x"),data);
		break;
	case DATA_HEX_DWORD:
		szTemp.Format(_T("0x%x"),data);
		break;
	}
	WritePrivateProfileString(m_Name,KeyName,szTemp,m_configPathName);
}
void MachineInterface::WriteConfigString(LPCTSTR KeyName,LPCTSTR strDefault)
{
	WritePrivateProfileString(m_Name,KeyName,strDefault,m_configPathName);
}

void MachineInterface::SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist)
{
	if(SrcString == _T("")) return;
	strlist->clear();
	CString temp, str;
	int nStart = 0, index = 0;
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //判断是否已','结尾
	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//如果最后一个不是分割字符，再把后面的一段保存再退出
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);

				if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
				strlist->push_back(str);
			}
			break;//如果最后一个就是分割字符，退出
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
		strlist->push_back(str);
		nStart = index + 1;
	}
}