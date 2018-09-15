// OtpSensorInterface.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "OtpSensorInterface.h"
#include "ccmBaseInterface.h"
#include "OTP_HI544.h"
#include "OTP_DW9807.h"
#include "OTP_S5k4ec.h"
#include "EEPROM_GT24C16.h"
#include "EEPROM_GT24C32.h"
#include "OTP_S5K4H8_F1X9.h"
#include "EEPROM_GT24C128_256.h"
#include "OTP_S5k5E8.h"
#include "OTP_DW9767.h"
#include "OTP_GC5025A.h"
#include "OTP_Hi556.h"
#include "OTP_DW9763_WrtProtect.h"
#include "OTP_S5K4H7YX.h"
#include "OTP_OV8856.h"
#include "EEPROM_BL24SA64.h"
#include "OTP_OV5675.h"
#include "EEPROM_M24C64S.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}


CString GetOtpSensorList()
{
	CString VcmDriverList = _T("OTP_HI544,OTP_DW9807,OTP_S5k4ec,EEPROM_GT24C16,EEPROM_GT24C32,S5K4H8_F1X9,GT24C128_256,S5K5E8,OTP_DW9767,\
OTP_GC5025A,OTP_HI556,OTP_DW9763_WrtProtect,OTP_S5K4H7YX,OTP_OV8856,EEPROM_BL24SA64,OTP_OV5675,EEPROM_M24C64S");
	return VcmDriverList;
}

COtpSensorInterface* GetOtpSensor(ccmBaseInterface* m_pInterface,CString OtpSensorName,int SlaveID,int CamID)
{
	COtpSensorInterface* pOtpSensor;
	if (OtpSensorName == _T("OTP_HI544"))
	{
		pOtpSensor = new OTP_HI544();
	}else if (OtpSensorName == _T("OTP_DW9807"))
	{
		pOtpSensor = new OTP_DW9807();
	}else if (OtpSensorName == _T("OTP_S5k4ec"))
	{
		pOtpSensor = new OTP_S5k4ec();
	}else if (OtpSensorName == _T("EEPROM_GT24C16"))
	{
		pOtpSensor = new EEPROM_GT24C16();
	}else if (OtpSensorName == _T("EEPROM_GT24C32"))
	{
		pOtpSensor = new EEPROM_GT24C32();
	}
	else if (OtpSensorName == _T("S5K4H8_F1X9"))
	{
		pOtpSensor = new OTP_S5K4H8_F1X9();
	}
	else if (OtpSensorName == L"GT24C128_256")
	{
		pOtpSensor = new EEPROM_GT24C128_256();
	}
	else if(OtpSensorName == L"S5K5E8")
	{
		pOtpSensor = new OTP_S5k5E8();
	}
	else if (OtpSensorName == _T("OTP_DW9767"))
	{
		pOtpSensor = new OTP_DW9767();
	}
	else if (OtpSensorName == _T("OTP_GC5025A"))
	{
		pOtpSensor = new OTP_GC5025A();
	}
	else if (OtpSensorName == _T("OTP_HI556"))
	{
		pOtpSensor = new OTP_Hi556();
	}
	else if (OtpSensorName == _T("OTP_DW9763_WrtProtect"))
	{
		pOtpSensor = new OTP_DW9763_WrtProtect();
	}
	else if (OtpSensorName == _T("OTP_S5K4H7YX"))
	{
		pOtpSensor = new OTP_S5K4H7YX();
	}
	else if (OtpSensorName == _T("OTP_OV8856"))
	{
		pOtpSensor = new OTP_OV8856();
	}
	else if (OtpSensorName == _T("EEPROM_BL24SA64"))
	{
		pOtpSensor = new EEPROM_BL24SA64();
	}
	else if (OtpSensorName == _T("OTP_OV5675"))
	{
		pOtpSensor = new OTP_OV5675();
	}
	else if (OtpSensorName == _T("EEPROM_M24C64S"))
	{
		pOtpSensor = new EEPROM_M24C64S();
	}
	else
	{
		return NULL;
	}
	pOtpSensor->SetInterface(m_pInterface,SlaveID,CamID);//SetInterface(ccmBaseInterface* m_pInterface)
	return pOtpSensor;

}

COtpSensorInterface::COtpSensorInterface(void)
{

}

COtpSensorInterface::~COtpSensorInterface(void)
{

}
void COtpSensorInterface::SetInterface(ccmBaseInterface* m_pInterface,int SlaveID,int CamID)
{
	pCcmCtr = m_pInterface;
	this->SlaveID =SlaveID;
	this->CamID = CamID;
}

int COtpSensorInterface::I2cWrite(USHORT uReg, USHORT Value,BYTE byMode/* =DEFAULT_DEV_ID */)
{
	return pCcmCtr->Device_WriteI2c( SlaveID,  uReg, Value, byMode, CamID/* =DEFAULT_DEV_ID */);
}

int COtpSensorInterface::I2cRead(USHORT uReg, USHORT *pValue,BYTE byMode/* =DEFAULT_DEV_ID */)
{
	return pCcmCtr->Device_ReadI2c( SlaveID,  uReg, pValue, byMode, CamID/* =DEFAULT_DEV_ID */);
}

void COtpSensorInterface::AddLog(int reg,int val)
{
	CString otpREG_Value;
	otpREG_Value.Format(_T("reg:%0x,val:%0x"),reg,val);
	return pCcmCtr->Ctrl_Addlog(CamID,otpREG_Value,COLOR_BLUE,200);
}
int COtpSensorInterface::I2cReadBlock(USHORT uReg,UCHAR uRegSize, BYTE *pValue,USHORT Dlen)
{
	return pCcmCtr->Device_ReadI2cBlock(SlaveID,uReg,uRegSize,pValue,Dlen,0,0,CamID);
}
int COtpSensorInterface::I2cWriteBlock(USHORT uReg,UCHAR uRegSize, BYTE *pValue,USHORT Dlen)
{
	return pCcmCtr->Device_WriteI2cBlock(SlaveID,uReg,uRegSize,pValue,Dlen,0,CamID);
}

int COtpSensorInterface::SendWaitAck(void)
{
	int maxbusycnt = 100;    //200us*100 = 8ms
	int ret = 0;
	do
	{
		//ret = FX2I2CMultiBytesWrite(I2cDevAddr,0,0,NULL,0,nDevID);
		ret =  I2cWriteBlock(0,0,NULL, 0);
		if (ret == 1)
		{
			break;
		}
	}while(maxbusycnt--);
	return ret;
}
