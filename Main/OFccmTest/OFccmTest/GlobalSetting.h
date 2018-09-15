#pragma once

#include "stdafx.h"
#include "OFccmDef.h"

#include <vector>
using namespace std;
 
class GlobalSetting
{
public:
	GlobalSetting(void);
	~GlobalSetting(void); 
	void pathInit(); 
	CString GetCamConfigPath(int CamID);

	CString GetCurrentPath();
	CString strCurrentPath;

	CString strInitFileName;
	CString GetIniPath();

	CString CamType;//dual single;
	int CamTotalNum;//1,2,3,4 

	void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);

	BOOL bIsLoad;
	void LoadSetting(); 
	void SaveSetting();


	BOOL WriteConfigData(CString sSection, CString sSectionKey, int nValue); 
	BOOL WriteConfigDataHex(CString sSection, CString sSectionKey, long nValue);
	void WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype = DATA_DEC);

	void WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName,LPCTSTR string);

	int ReadConfigData(CString sSection,CString sSectionKey,int nDefault);
	long ReadConfigDataHex(CString sSection,CString sSectionKey,long nDefault);

	LPCTSTR ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName,LPCTSTR default);

	CString DeviceTypeSel;
	BOOL bUsedCamCode;
	int errorFramLimit;
	 
	int MenueDeviID;
	int Cam0DeviID ;
	int Cam1DeviID ;

	int DebugMode;

	int siteNo;

	CString CAM0_SensorInitFile;
	CString CAM1_SensorInitFile;

	CString StationName;

	BOOL OffLineTest;	 
	CString GetOffLineTestPath(int CamID);
	CString Cam0ImgPath;
	CString Cam1ImgPath;
 

	BOOL bMachine;
	CString MachineName;
	CString machinePath;

	BOOL bLogSave; 
	BOOL bOnLButtonUp;
	BOOL bFocus;

	BOOL bAutoMachine;

	int ButtonUpTestItem;

	int m_Raw_Width,m_Raw_Height,RawBits,RawOutForamt; 
	int m_RawShowType,m_FileLength;
	BOOL bRawImg;

	int DisplayInterval;

	BOOL showLog;

	int bSite;//������ʾλ�ã�ֻ���ģ������Ϊ1�������0:�����ĸ������Ĵ��� 1���ر�
	CString m_MySite;//�Զ��崰��Site��ֻ���ڽ�����ʾ
	BOOL bLEDEnable;
	int  m_com;
	int m_iDualAutoTestWaitPeerMaxTime;//˫���Զ�ģʽ����ʱ����Ҫ�ȴ��Զ�(����/����)�Ĳ��Խ�������ȴ�ʱ�䣬��λms
};

