// DeviceInterface.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ccmBaseInterface.h"
#include "DeviceInterface.h" 
#include "OFKit910.h"
#include "Device_UH920.h"
#include "Mud952.h"
//#include "Device_R3.h"
//#include "Device_R5.h" 
//#include "CTS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

CDeviceInterface* GetDeviceType(ccmBaseInterface* m_pInterface,CString DeviceName)
{
	CDeviceInterface* pDInterface;
	if (DeviceName==_T("UV910"))
	{
		pDInterface = new OFKit910();
	}
	else if (DeviceName==_T("UH920"))
	{
		pDInterface = new CDevice_UH920();
	}
	else if (DeviceName==_T("MUD952"))
	{
		pDInterface = new Mud952();
	}
// 	else if (DeviceName==_T("R3"))
// 	{
// 		pDInterface = new CDevice_R3();
// 	}
// 	else if (DeviceName==_T("R5"))
// 	{
// 		pDInterface = new CDevice_R5();
// 	}
// 	else if (DeviceName==_T("CTS"))
// 	{
// 		pDInterface = new CCTS();
// 	}
	pDInterface->SetInterface(m_pInterface);

	return pDInterface;
}

  void CDeviceInterface::SetDevName(CString devName)
{
	this->devName = devName;
}

 vector<CString> CDeviceInterface::GetDevList()
{
	vector<CString> devName;
	return devName;
}
CDeviceInterface::CDeviceInterface(void)
{
	for(int i=0;i<4;i++)
	{
		CamSensorTab[i].ParaList = new USHORT [8192*4];   //(USHORT*) malloc(8192*sizeof(USHORT));
		CamSensorTab[i].SleepParaList = new USHORT [2048];   //(USHORT*) malloc(1024*2);
	}	
}

CDeviceInterface::~CDeviceInterface(void)
{
	for(int i=0;i<4;i++)
	{
		SAFE_DELETE_ARRAY(CamSensorTab[i].ParaList);   //(USHORT*) malloc(8192*sizeof(USHORT));
		SAFE_DELETE_ARRAY(CamSensorTab[i].SleepParaList);  
	}
}

void CDeviceInterface::SetInterface(ccmBaseInterface* m_pInterface)
{
	pCcmCtr = m_pInterface;
	strAppName = _T("sensor");
}
void CDeviceInterface::DriverICStandby(int CamID)
{
	pCcmCtr->VcmDr_WriteAF_Code(0xFFFF, CamID);
}
 
  CString CDeviceInterface::ReadConfigString(CString KeyName,CString defaultValue,int CamID)
  {
	  return pCcmCtr->Config_ReadConfigString(strAppName,KeyName,defaultValue,CamID);
  }
  int CDeviceInterface::ReadConfigData(CString KeyName,int defaultValue,int CamID)
  {
	  return pCcmCtr->Config_ReadConfigInt(strAppName,KeyName,defaultValue,CamID);
  }
  void CDeviceInterface::AddLog(int CamID,CString Info,COLORREF CurrentColor,int InfoSize)
  {
	  pCcmCtr->Ctrl_Addlog(CamID,L"Dev"+ Info,CurrentColor,InfoSize);
  }

  void CDeviceInterface::WriteConfigString(CString KeyName,CString defaultValue,int CamID)
  {
	 pCcmCtr->Config_WriteConfigString(strAppName,KeyName,defaultValue,CamID);
  }
  void CDeviceInterface::WriteConfigData(CString KeyName,int defaultValue,int CamID)
  {
	  pCcmCtr->Config_WriteConfigInt(strAppName,KeyName,defaultValue,CamID);
  }

  HWND CDeviceInterface::GetCamDisplayHwnd(int CamID)
  {
	  return pCcmCtr->Ctrl_GethDisplayWnd(CamID);
  }

  BOOL CDeviceInterface::LoadSensorGenaralTab(int CamID)
{
	CamSensorTab[CamID].name = ReadConfigString(_T("SensorName"),0, CamID); 
	CamSensorTab[CamID].width   = ReadConfigData(_T("width"),0, CamID);
	CamSensorTab[CamID].height  = ReadConfigData(_T("height"),0, CamID);
	CamSensorTab[CamID].type  = ReadConfigData(_T("type"),2, CamID);

	CamSensorTab[CamID].port = ReadConfigData( _T("port"), 0, CamID);
	CamSensorTab[CamID].sensorpwdn = ReadConfigData( _T("pwdn"), 0, CamID);
	CamSensorTab[CamID].sensorreset = ReadConfigData( _T("reset"), 0, CamID);
	CamSensorTab[CamID].pin= ReadConfigData( _T("pin"), 0, CamID);
	//CamSensorTab[CamID].pin = CamSensorTab[CamID].sensorpwdn<<1|CamSensorTab[CamID].sensorreset;

	CamSensorTab[CamID].SlaveID  = ReadConfigData( _T("SlaveID"), 0, CamID);
	CamSensorTab[CamID].mode     = ReadConfigData( _T("mode"), 0, CamID);
	CamSensorTab[CamID].FlagReg  = ReadConfigData( _T("FlagReg"), 0, CamID);
	CamSensorTab[CamID].FlagMask = ReadConfigData( _T("FlagMask"), 0xff, CamID);
	CamSensorTab[CamID].FlagData = ReadConfigData( _T("FlagData"), 0, CamID);

	CamSensorTab[CamID].FlagReg1  = ReadConfigData( _T("FlagReg1"), 0, CamID);
	CamSensorTab[CamID].FlagMask1 = ReadConfigData(_T("FlagMask1"), 0x0, CamID);
	CamSensorTab[CamID].FlagData1 = ReadConfigData( _T("FlagData1"), 0, CamID);

	CamSensorTab[CamID].outformat= ReadConfigData( _T("outformat"), 0x00, CamID);
	CamSensorTab[CamID].mclk     = ReadConfigData( _T("mclk"), 0x01, CamID);

// 	CamSensorTab[CamID].avdd   = ReadConfigData( _T("avdd"), 0x00, CamID);
// 	CamSensorTab[CamID].dovdd  = ReadConfigData( _T("dovdd"), 0x00, CamID);
// 	CamSensorTab[CamID].dvdd   = ReadConfigData( _T("dvdd"), 0x00, CamID);
// 	CamSensorTab[CamID].af_vdd = ReadConfigData( _T("afvdd"), 0x00, CamID);
// 	CamSensorTab[CamID].vpp    = ReadConfigData(_T("vpp"),0x00, CamID);


	CString Temp = L"";
	Temp = ReadConfigString(L"avdd", L"0.0", CamID);
	CamSensorTab[CamID].avdd = _ttoi(Temp);
	Temp = ReadConfigString(L"dovdd", L"0.0", CamID);
	CamSensorTab[CamID].dovdd = _ttoi(Temp);
	Temp = ReadConfigString(L"dvdd", L"0.0", CamID);
	CamSensorTab[CamID].dvdd = _ttoi(Temp);
	Temp = ReadConfigString(L"afvdd", L"0.0", CamID);
	CamSensorTab[CamID].af_vdd = _ttoi(Temp);
	Temp = ReadConfigString(L"vpp", L"0.0", CamID);
	CamSensorTab[CamID].vpp = _ttoi(Temp);

	CamSensorTab[CamID].iXRoi = ReadConfigData( _T("x_roi"), 0, CamID);
	CamSensorTab[CamID].iYRoi = ReadConfigData( _T("y_roi"), 0, CamID);

	return TRUE;
}

  BOOL CDeviceInterface::LoadSensorParalist(int CamID)
{
	CStdioFile file;
	CString m_ConfigFilePathName = pCcmCtr->Config_GetConfigPathName(CamID);
	if (!file.Open(m_ConfigFilePathName, CFile::modeRead | CFile::typeText))
	{
		return FALSE;
	}
	CString szLine = _T("");
	int addr =0, reg=0, value=0;
	BYTE i2cmode = 0;
	USHORT *pParaList= CamSensorTab[CamID].ParaList;
	USHORT *pSleepParaList = CamSensorTab[CamID].SleepParaList; 

	USHORT ParaListSize=0;
	USHORT SleepParaListSize = 0;
	 

	CString sReg, sVal;
	CString strTmp[2];
	int tmp = 0;
	strTmp[0] = "[ParaList]";
	strTmp[1] = "[SleepParaList]";
	 

	for(int i = 0; i <2; i++)
	{
		strTmp[i].MakeLower();
		strTmp[i].TrimLeft();
		strTmp[i].TrimRight();	
	}
	int state = -1;
	while(file.ReadString(szLine))
	{
		CString Textout;
		//寻找注释符号或者']',如果有这样的，只取前面的，
		tmp = szLine.FindOneOf(_T("//")); 
		if( tmp == 0)
		{
			continue;
		}
		else if(tmp > 0)
		{
			szLine = szLine.Left(tmp);
		}
		tmp = szLine.FindOneOf(_T("]")); 
		if( tmp == 0)
		{
			continue;
		}
		else if(tmp > 0)
		{
			szLine = szLine.Left(tmp+1);
		}
		szLine.MakeLower();
		szLine.TrimLeft();
		szLine.TrimRight();		

		if(szLine == strTmp[0]) 
		{
			state = 0;
			ParaListSize = 0;
			continue;
		}
		else if(szLine == strTmp[1])
		{
			state = 1;
			SleepParaListSize = 0;
			continue;
		}
		 

		if(szLine.IsEmpty())
			continue;
		if(szLine.Left(1) == _T(","))
			continue;
		if(szLine.Left(1) == _T(";"))
			continue;
		if(szLine.Left(1) == _T("/"))
			continue;

		if(szLine.Left(1) == _T("["))
		{
			state = -1;
			continue;
		}


		AfxExtractSubString(sReg, szLine, 0, ',');
		AfxExtractSubString(sVal, szLine, 1, ',');
		sReg.TrimLeft();   
		sReg.TrimRight();
		sVal.TrimRight();  
		sVal.TrimLeft();

		if(!swscanf(sReg, _T("0x%x"), &reg)) //读取键值对数据	
			swscanf(sReg, _T("%d"), &reg);

		if(!swscanf(sVal, _T("0x%x"), &value)) //读取键值对数据	
			swscanf(sVal, _T("%d"), &value);

		if(state == 0)
		{
			*(pParaList+ParaListSize) = reg;
			*(pParaList+ParaListSize+1) = value;
			ParaListSize += 2;
		}
		else if(state == 1)
		{
			*(pSleepParaList+SleepParaListSize) = reg;
			*(pSleepParaList+SleepParaListSize+1) = value;
			SleepParaListSize += 2;			
		} 
	}
	file.Close();

	if(ParaListSize)
	{
		CamSensorTab[CamID].ParaListSize  = ParaListSize;// + 2; //datasize...
		 
		if(SleepParaListSize)
		{
			CamSensorTab[CamID].SleepParaListSize  = SleepParaListSize;			 
		} 
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}
 