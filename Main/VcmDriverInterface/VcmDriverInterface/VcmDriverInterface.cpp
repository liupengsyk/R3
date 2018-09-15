// VcmDriverInterface.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ccmBaseInterface.h"
#include "VcmDriverInterface.h"
#include "Driver_S5k4ec.h"
#include "Driver_DW9807.h"
#include "Driver_DW9804.h"
#include "Driver_DW9800.h"
#include "Driver_HI541.h"
#include "Driver_LC898212.h"
#include "Driver_LC898212XD.h"
#include "Driver_Max14650.h"
#include "Driver_AK7371.h"
#include "Driver_DW9714.h"
#include "Driver_BU64295296297GWZ.h"
#include "LC898217_E4_E6.h"
#include "LC898217_E8_EA.h"
#include "Driver_DW9767.h"
#include "Driver_AXD898123XC.h"
#include "DriverIC_DW9718.h"
#include "Driver_LC898219.h"
#include "Driver_AK7374.h"
#include "Driver_DW9763.h"
#include "Driver_DW9719.h"
#include "Driver_WV511A.h"
#include "Driver_BU64748GWZ.h"
#include "Driver_LC898214XC_shicoh.h"
#ifndef _AFXDLL
#define _AFXDLL
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;
CString GetVcmDriverList()
{
	CString VcmDriverList = _T("S5K4EC,DW9767,DW9807,DW9804,DW9800,HI541,LC898212,LC898212XD,Max14650,AK7371,DW9714,BU64295296297GWZ,LC898217_E4_E6,LC898217_E8_EA,Driver_AXD898123XC,\
							   DW9718,LC898219,AK7374,DW9763,DW9719,WV511A,BU64748GWZ,LC898214_Shicoh");
	return VcmDriverList;
}

CVcmDriverInterface* GetVcmDriver(ccmBaseInterface* m_pInterface,CString DriveName,int SlaveID,int CamID)
{
	CVcmDriverInterface* pDriver;
	if (DriveName == _T("S5K4EC"))
	{
		pDriver = new Driver_S5k4ec();
	}
	else if (DriveName == _T("DW9767"))
	{
		pDriver = new Driver_DW9767();
	}
	else if (DriveName == _T("DW9807"))
	{
		pDriver = new Driver_DW9807();
	}
	else if (DriveName == _T("DW9804"))
	{
		pDriver = new Driver_DW9804();
	}
	else if (DriveName == _T("DW9800"))
	{
		pDriver = new Driver_DW9800();
	}
	else if (DriveName == _T("HI541"))
	{
		pDriver = new Driver_HI541();
	}
	else if (DriveName == _T("LC898212"))
	{
		pDriver = new Driver_LC898212();
	}
	else if (DriveName == _T("LC898212XD"))
	{
		pDriver = new DriverIC_LC898212XD();
	}
	else if (DriveName == _T("Max14650"))
	{
		pDriver = new Driver_Max14650();
	}
	else if (DriveName == _T("AK7371"))
	{
		pDriver = new Driver_AK7371();
	} 	
	else if (DriveName == _T("DW9714"))
	{
		pDriver = new Driver_DW9714();
	} 	//
	else if (DriveName == _T("BU64295296297GWZ"))
	{
		pDriver = new Driver_BU64295296297GWZ();
	}
	else if (DriveName == _T("LC898217_E4_E6"))
	{
		pDriver = new LC898217_E4_E6();
	}
	else if (DriveName == _T("LC898217_E8_EA"))
	{
		pDriver = new LC898217_E8_EA();
	}
	else if (DriveName == _T("Driver_AXD898123XC"))
	{
		pDriver = new Driver_AXD898123XC();
	} 
	else if (DriveName == _T("DW9718"))
	{
		pDriver = new DriverIC_DW9718();
	}
	else if (_T("LC898219") == DriveName)
	{
		pDriver = new Driver_LC898219();
	}
	else if (_T("AK7374") == DriveName)
	{
		pDriver = new Driver_AK7374();
	}
	else if (_T("DW9763") == DriveName)
	{
		pDriver = new Driver_DW9763();
	}
	else if (_T("DW9719") == DriveName)
	{
		pDriver = new Driver_DW9719();
	}
	else if (_T("WV511A") == DriveName)
	{
		pDriver = new Driver_WV511A();
	}
	else if (_T("BU64748GWZ") == DriveName)
	{
		pDriver = new Driver_BU64748GWZ();
	}
	else if (_T("LC898214_Shicoh") == DriveName)
	{
		pDriver = new Driver_LC898214XC_shicoh();
	}
	else
	{		
		return NULL;  
	}
	pDriver->SetInterface(m_pInterface,SlaveID,CamID);//SetInterface(ccmBaseInterface* m_pInterface)
	return pDriver;
 
}


CVcmDriverInterface::CVcmDriverInterface(void)
{

}

CVcmDriverInterface::~CVcmDriverInterface(void)
{

}

int CVcmDriverInterface::Init()
{
	return 0;
}

int CVcmDriverInterface::Standby()
{
	return 0;
}
void CVcmDriverInterface::SetInterface(ccmBaseInterface* m_pInterface,int SlaveID,int CamID)
{
	this->SlaveID=SlaveID;
	pCcmCtr = m_pInterface;
	this->CamID = CamID;

}
 
int CVcmDriverInterface::I2cWrite(UCHAR uAddr, USHORT uReg, USHORT Value,BYTE byMode)
{
	return pCcmCtr->Device_WriteI2c( uAddr,  uReg, Value, byMode, CamID/* =DEFAULT_DEV_ID */);
}

int CVcmDriverInterface::I2cRead(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode)
{
	return pCcmCtr->Device_ReadI2c( uAddr,  uReg, pValue, byMode, CamID/* =DEFAULT_DEV_ID */);
}
 
int CVcmDriverInterface::Config_ReadConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int default,int CamID)
{
	return pCcmCtr->Config_ReadConfigInt( AppName,  KeyName,  default, CamID);
}
CString CVcmDriverInterface::Config_ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR default,int CamID)
{
	return pCcmCtr->Config_ReadConfigString( AppName,  KeyName,  default, CamID);
}
 
void CVcmDriverInterface::Config_WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int default,int datatype,int CamID)
{
	pCcmCtr->Config_WriteConfigInt( AppName,  KeyName,  default,datatype, CamID);
}
void CVcmDriverInterface::Config_WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR default,int CamID)
{
	pCcmCtr->Config_WriteConfigString(AppName,KeyName,default,CamID);
}

void CVcmDriverInterface::Addlog(CString strInfo,COLORREF color,int Size)
{
	return pCcmCtr->Ctrl_Addlog( CamID,  strInfo,  color, Size);
}

int CVcmDriverInterface::AutoFocus()
{
	return 0;
}

int CVcmDriverInterface::I2cReadBlock(UCHAR uAddr, USHORT uReg,BYTE regSize, BYTE *pValue,USHORT uSize)
{
	return pCcmCtr->Device_ReadI2cBlock(uAddr,uReg,regSize,pValue,uSize,0,0,CamID);
}
int CVcmDriverInterface::I2cWriteBlock(UCHAR uAddr, USHORT uReg,BYTE regSize, BYTE *pValue,USHORT uSize)
{
	return pCcmCtr->Device_WriteI2cBlock(uAddr,uReg,regSize,pValue,uSize,0,CamID);
}