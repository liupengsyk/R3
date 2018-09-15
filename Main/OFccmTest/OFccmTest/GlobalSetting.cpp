#include "StdAfx.h"
#include "GlobalSetting.h"


GlobalSetting::GlobalSetting(void)
{
	DebugMode = 0;
	bOnLButtonUp = FALSE;
	ButtonUpTestItem = 0;
	bIsLoad = FALSE;
	bFocus = FALSE;
	bAutoMachine = TRUE;
}


GlobalSetting::~GlobalSetting(void)
{
}

CString GlobalSetting::GetCamConfigPath(int CamID)
{
	CString m_ConfigFilePathName;
	CString strItem;
	switch(CamID)
	{
	case 0:
		strItem = _T("SensorTab\\")+CAM0_SensorInitFile;
		break;
	case 1:
		strItem = _T("SensorTab\\")+CAM1_SensorInitFile;
		break;	 
	}
	
	strCurrentPath = GetCurrentPath();
	m_ConfigFilePathName = strCurrentPath + strItem; 

	return m_ConfigFilePathName ;
}

CString GlobalSetting::GetCurrentPath()
{
	return strCurrentPath;
}
CString GlobalSetting::GetIniPath()
{
	return strInitFileName;
}


void GlobalSetting::SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist)
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
void GlobalSetting::pathInit()
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];
	GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);
	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	strCurrentPath = (CString(sDrive) + CString(sDir));

	if ( strCurrentPath.Right(1) != _T('\\') )
		strCurrentPath += _T("\\"); 

	strInitFileName = strCurrentPath +  _T("\\SensorTab") _T("\\init.ini");
}

void GlobalSetting::LoadSetting()
{
	pathInit();

	errorFramLimit=ReadConfigData(_T("Global"), _T("ErrorFrameLimit"), 30);
	DeviceTypeSel = ReadConfigString(_T("Global"), _T("DeviceTypeSel"), _T("UV910"));
	CamTotalNum = ReadConfigData(_T("Global"), _T("CamTotalNum"), 1);
	CamType = ReadConfigString(_T("Global"), _T("CamType"), _T("Normal"));
	bUsedCamCode = ReadConfigData(_T("Global"), _T("bUsedCamCode"),1 ); 
	DebugMode = ReadConfigData(_T("Global"), _T("DebugMode"),0);

	MenueDeviID = ReadConfigData(_T("Global"), _T("MenueDeviID"), 0);
	Cam0DeviID = ReadConfigData(_T("Global"), _T("Cam0DeviID"), 0);
	Cam1DeviID = ReadConfigData(_T("Global"), _T("Cam1DeviID"),1);

	OffLineTest = ReadConfigData(_T("Global"), _T("OffLineTest"),0);
	Cam0ImgPath = ReadConfigString(_T("InitSet"), _T("Cam0ImgPath"),0);
	Cam1ImgPath = ReadConfigString(_T("InitSet"), _T("Cam1ImgPath"),0);
 


	CAM0_SensorInitFile = ReadConfigString(_T("InitSet"), _T("CAM0_SensorInitFile"),0);
	CAM1_SensorInitFile = ReadConfigString(_T("InitSet"), _T("CAM1_SensorInitFile"),0);
	 

	StationName = ReadConfigString(_T("Global"), _T("StationName"),_T("OFccmTest"));

	bMachine = ReadConfigData(_T("Global"), _T("bMachine"),0);
	MachineName = ReadConfigString(_T("Global"), _T("MachineName"),0);
	machinePath =  GetCurrentPath()+ _T("Machine\\");

	bLogSave = ReadConfigData(_T("Global"), _T("bLogSave"),0);

	bOnLButtonUp = ReadConfigData(_T("Global"), _T("bOnLButtonUp"),0);
	ButtonUpTestItem = ReadConfigData(_T("Global"), _T("ButtonUpTestItem"),1);

	bFocus = ReadConfigData(_T("Global"), _T("bFocus"),0);
	bAutoMachine = ReadConfigData(_T("Global"),_T("bAutoMachine"),1);

	m_Raw_Width = ReadConfigData(_T("RawInfo"), _T("m_Raw_Width"),1);
	m_Raw_Height = ReadConfigData(_T("RawInfo"), _T("m_Raw_Height"),1);
	RawBits = ReadConfigData(_T("RawInfo"), _T("RawBits"),1);
	RawOutForamt = ReadConfigData(_T("RawInfo"), _T("RawOutForamt"),1);
	m_RawShowType = ReadConfigData(_T("RawInfo"), _T("m_RawShowType"),1);
	m_FileLength = ReadConfigData(_T("RawInfo"), _T("m_FileLength"),1);
	bRawImg = ReadConfigData(_T("RawInfo"), _T("bRawImg"),1);

	siteNo = ReadConfigData(_T("Global"), _T("siteNo"),0);
	m_MySite = ReadConfigString(_T("Global"), _T("MysiteNo"),_T(""));
	bSite = ReadConfigData(_T("Global"), _T("bSite"),0);

	DisplayInterval = ReadConfigData(_T("Global"), _T("DisplayInterval"),1);

	showLog = ReadConfigData( L"Global",L"showLog",0);

	bLEDEnable=ReadConfigData(_T("Global"), _T("bLEDEnable"),0);
	m_com=ReadConfigData(_T("Global"), _T("COM"),0);
	m_iDualAutoTestWaitPeerMaxTime = ReadConfigData(_T("Global"), _T("DualAutoTestWaitPeerMaxTime"),1000*30);//默认最大等待30s
	  //DisplayInterval
	bIsLoad  = TRUE;
}
void GlobalSetting::SaveSetting()
{
	WriteConfigString(_T("Global"), _T("DeviceTypeSel"), DeviceTypeSel);
	WriteConfigData(_T("Global"), _T("CamTotalNum"), CamTotalNum);
	WriteConfigString(_T("Global"), _T("CamType"), CamType);
	WriteConfigData(_T("Global"), _T("bUsedCamCode"), bUsedCamCode);
 
	WriteConfigData(_T("Global"), _T("DebugMode"),DebugMode);
	WriteConfigData(_T("Global"), _T("MenueDeviID"), MenueDeviID);
	WriteConfigData(_T("Global"), _T("Cam0DeviID"), Cam0DeviID);
	WriteConfigData(_T("Global"), _T("Cam1DeviID"),Cam1DeviID);
	WriteConfigData(_T("Global"), _T("OffLineTest"),OffLineTest);
	WriteConfigData(_T("Global"), _T("bSite"),bSite);
	WriteConfigString(_T("InitSet"), _T("Cam0ImgPath"),Cam0ImgPath);
	WriteConfigString(_T("InitSet"), _T("Cam1ImgPath"),Cam1ImgPath);
	 

 

	WriteConfigString(_T("InitSet"), _T("CAM0_SensorInitFile"),CAM0_SensorInitFile);
	WriteConfigString(_T("InitSet"), _T("CAM1_SensorInitFile"),CAM1_SensorInitFile);
	 
	 
	WriteConfigString(_T("Global"), _T("StationName"),StationName);

	WriteConfigData(_T("Global"), _T("bMachine"),bMachine);
	WriteConfigString(_T("Global"), _T("MachineName"),MachineName);	 
	WriteConfigInt(_T("Global"), _T("bLogSave"),bLogSave);	 

	WriteConfigInt(_T("Global"), _T("bOnLButtonUp"),bOnLButtonUp);	 
	WriteConfigInt(_T("Global"), _T("ButtonUpTestItem"),ButtonUpTestItem); 

	WriteConfigInt(_T("Global"), _T("bFocus"),bFocus);
	WriteConfigInt(_T("Global"),_T("bAutoMachine"),bAutoMachine);

	WriteConfigInt(_T("RawInfo"), _T("m_Raw_Width"),m_Raw_Width);
    WriteConfigInt(_T("RawInfo"), _T("m_Raw_Height"),m_Raw_Height);
	WriteConfigInt(_T("RawInfo"), _T("RawBits"),RawBits);
    WriteConfigInt(_T("RawInfo"), _T("RawOutForamt"),RawOutForamt);
	WriteConfigInt(_T("RawInfo"), _T("m_RawShowType"),m_RawShowType);
    WriteConfigInt(_T("RawInfo"), _T("m_FileLength"),m_FileLength);
	WriteConfigInt(_T("RawInfo"), _T("bRawImg"),bRawImg);

	WriteConfigInt(_T("Global"), _T("DisplayInterval"),DisplayInterval);
	WriteConfigInt(_T("Global"), _T("siteNo"),siteNo);
	WriteConfigString(_T("Global"), _T("MysiteNo"),m_MySite);
	WriteConfigData( L"Global",L"showLog",showLog);

	WriteConfigInt(_T("Global"), _T("bLEDEnable"),bLEDEnable);
	WriteConfigInt(_T("Global"), _T("COM"),m_com);

	bIsLoad = FALSE;
}


BOOL GlobalSetting::WriteConfigData(CString sSection, CString sSectionKey, int nValue)
{
	CString  sValue;
	sValue.Format(_T("%d"),nValue);
	return WritePrivateProfileString(sSection,sSectionKey,sValue,strInitFileName);
}

BOOL GlobalSetting::WriteConfigDataHex(CString sSection, CString sSectionKey, long nValue)
{
	CString sTmp;
	sTmp.Format(_T("0x%x"), nValue);
	return WritePrivateProfileString(sSection,sSectionKey,sTmp,strInitFileName);
}

void GlobalSetting::WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype)
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
	WritePrivateProfileString(AppName,KeyName,szTemp,strInitFileName);
}

void GlobalSetting::WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName,LPCTSTR string)
{
	WritePrivateProfileString(AppName,KeyName,string,strInitFileName);
}
int GlobalSetting::ReadConfigData(CString sSection,CString sSectionKey,int nDefault)
{
	return GetPrivateProfileInt(sSection,sSectionKey,nDefault,strInitFileName);
}

long GlobalSetting::ReadConfigDataHex(CString sSection,CString sSectionKey,long nDefault)
{
	CString sTemp;
	GetPrivateProfileString(sSection,sSectionKey,_T(""), sTemp.GetBuffer(MAX_PATH), MAX_PATH,strInitFileName);
	sTemp.ReleaseBuffer();
	char* pAddr = (char*)sTemp.GetBuffer(MAX_PATH); 
	long x = sTemp.IsEmpty() ? nDefault : strtoul( pAddr, NULL, 16); 
	sTemp.ReleaseBuffer();
	return x;
}

LPCTSTR GlobalSetting::ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName,LPCTSTR default)
{
	static CString szTemp;
	GetPrivateProfileString(AppName, KeyName, default, szTemp.GetBuffer(255), 255, strInitFileName);
	szTemp.ReleaseBuffer();
	return szTemp;
}

 
CString GlobalSetting::GetOffLineTestPath(int CamID)
{
	CString CamImgPath;
	switch(CamID)
	{
	case 0:
		CamImgPath = Cam0ImgPath;
		break;
	case 1:
		CamImgPath = Cam0ImgPath;
		break;
	case 2:
		CamImgPath = Cam0ImgPath;
		break;
	case 3:
		CamImgPath = Cam0ImgPath;
		break;
	default:
		break;
	}
	return CamImgPath;
}