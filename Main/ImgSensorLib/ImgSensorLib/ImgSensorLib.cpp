// ImgSensorLib.cpp : 定义 DLL 应用程序的导出函数。
//
#pragma once
#include "stdafx.h"
#include "ImgSensorLib.h"
#include "ccmBaseInterface.h"
//SensorLib
#include "IMX_258_0AQH5.h"
#include "IMX_214.h"
#include "IMX_278.h"
#include "S5K5E2.h"
#include "S5K5E8.h"
#include "OV8865.h"
#include "OV8835.h"
#include "S5K3L8XXM3_FGX9.h"
#include "S5K3P3.h"
#include "HI_259.h"
#include "IMX_386.h"
#include "IMX_298.h"
#include "IMX_230.h"
#include "S5K4H8_F1X9.h"
#include "OV13853.h"
#include "S5K3P3SP06.h"
#include "IMX_398.h"
#include "IMX_350.h"
#include "IMX_376.h"
#include "OV13855.h"
#include "S5K2P7SX03_FGX9.h"
#include "OV12A10.h"
#include "OV16880.h"
#include "AR1335.h"
#include "HI_846.h"
#include "IMX_286.h"
#include "IMX_486.h"
#include "IMX_319.h"
#include "OV5695.h"
#include "OV13880.h"
#include "S5K3M3.h"
#include "GC2375.h"
#include "OV12870.h"
#include "S5K3M2xxM3.h"
#include "OV5670.h"
#include "IMX_362.h"
#include "S5K3P8SP03-FGX9.h"
#include "S5K3L2.h"
#include "OV16B10.h"
#include "IMX_371.h"
#include "OV16885.h"
#include "OV20880.h"
#include "S5K4H7YX.h"
#include "S5K2L7SA.h"
#include "SP2509.h"
#include "S5K3P9SP04-FGX9.h"
#include "OV13358.h"
#include "GC5025A.h"
#include "HI_556.h"
#include "IMX_268.h"
#include "HI_1333.h"
#include "IMX_363.h"
#include "OV8856.h"
#include "S5K2L8SX.h"
#include "S5K5E9.h"
#include "IMX_519.h"
#include "S5K2X7SP03.h"
#include "OV5675.h"
#include "IMX_351.h"
#include "IMX_380.h"
#include "IMX_498.h"
#include "IMX_550.h"
#include "IMX_600.h"
#include "OV08A10.h"
#include "OV13A40.h"
#include "OV24A1B.h"
#include "GC5025.h"
#include "S5K2X7SX.h"
#include "S5K3T1.h"
#include "S5K3L6XX03_FGX9.h"
#include "IMX_576.h"
#include "OV02A10.h"
#include "IMX_476.h"
#include "IMX_355.h"

#pragma comment(lib,"Lib_lsc_OV/lc4.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 using namespace std;
  
CString GetImgSensorList()
{
	CString ImgSensorList = _T("IMX_258_0AQH5,IMX_214,IMX_278,S5K5E2,S5K5E8,OV8865,OV8835,S5K3L8XXM3_FGX9,S5K3P3,HI_259,IMX_386,IMX_298,IMX_230,S5K2L7SA,S5K4H8_F1X9,\
OV13853,S5K3P3SP06,IMX_398,IMX_350,IMX_376,OV13855,S5K2P7SX03_FGX9,OV12A10,OV16880,AR1335,HI846,IMX_286,IMX_486,IMX_319,OV5695,OV13880,\
S5K3M3,GC2375,OV12870,S5K3M2xxM3,OV5670,IMX_362,S5K3P8SP03-FGX,S5K3P9SP04-FGX,S5K3L2,OV16B10,IMX_371,OV16885,OV20880,S5K4H7YX,SP2509_MIPI_1600_1200_72M_RAW10,OV13358,\
GC5025A,HI_556,IMX_268,HI_1333,IMX_363,OV8856,S5K2L8SX,S5K5E9,IMX_519,S5K2X7SP03,OV5675,IMX_351,IMX_380,IMX_498,IMX_550,IMX_600,OV08A10,OV13A40,OV24A1B,GC5025,\
S5K2X7SX,S5K3T1,S5K3L6XX03_FGX9,IMX_576,OV02A10,IMX_476,IMX_355");
	return ImgSensorList;
}

CImgSensorLib* GetImgSensor(ccmBaseInterface* m_pInterface,CString ImgSensorName,int SlaveID,int CamID)
{
	CImgSensorLib* pImgSensor;
	if (_T("IMX_258_0AQH5") == ImgSensorName)
	{
		pImgSensor = new IMX_258_0AQH5();
	}
	else if (_T("IMX_214") == ImgSensorName)
	{
		pImgSensor = new IMX_214();
	} 
	else if (_T("IMX_278") == ImgSensorName)
	{
		pImgSensor = new IMX_278();
	}
	else if (_T("S5K5E2") == ImgSensorName)
	{
		pImgSensor = new S5K5E2();
	}
	else if (_T("S5K5E8") == ImgSensorName)
	{
		pImgSensor = new S5K5E8();
	}
	else if (_T("OV8865") == ImgSensorName)
	{
		pImgSensor = new OV8865();
	}
	else if (_T("S5K3L8XXM3_FGX9") == ImgSensorName)
	{
		pImgSensor = new S5K3L8XXM3_FGX9();
	} 
	else if (_T("S5K3P3") == ImgSensorName)
	{
		pImgSensor = new S5K3P3();
	} 
	else if (_T("HI_259") == ImgSensorName)
	{
		pImgSensor = new HI_259();
	} 
	else if (_T("IMX_386") == ImgSensorName)
	{
		pImgSensor = new IMX_386();
	} 
	else if (_T("IMX_298") == ImgSensorName)
	{
		pImgSensor = new IMX_298();
	} 
	else if (_T("IMX_230") == ImgSensorName)
	{
		pImgSensor = new IMX_230();
	}
	else if (_T("S5K4H8_F1X9") == ImgSensorName)
	{
		pImgSensor = new S5K4H8_F1X9();
	}
	else if(_T("OV13853") == ImgSensorName)
	{
		pImgSensor=new OV13853();
	}
	else if(_T("S5K3P3SP06") == ImgSensorName)
	{
		pImgSensor=new S5K3P3SP06();
	}
	else if(_T("IMX_398") == ImgSensorName)
	{
		pImgSensor=new IMX_398();
	}
	else if(_T("IMX_350") == ImgSensorName)
	{
		pImgSensor=new IMX_350();
	}
	else if (_T("IMX_376") == ImgSensorName)
	{
		pImgSensor=new IMX_376();
	}
	else if (_T("OV13855") == ImgSensorName)
	{
		pImgSensor = new OV13855();
	}
	else if (_T("S5K2P7SX03_FGX9") == ImgSensorName)
	{
		pImgSensor = new S5K2P7SX03_FGX9();
	}
	else if (_T("OV12A10") == ImgSensorName)
	{
		pImgSensor = new OV12A10();
	}
	else if (_T("OV16880") == ImgSensorName)
	{
		pImgSensor = new OV16880();
	}
	else if (_T("AR1335") == ImgSensorName)
	{
		pImgSensor = new AR1335();
	}
	else if (_T("HI846") == ImgSensorName)
	{
		pImgSensor = new HI_846();
	}
	else if (_T("IMX_286") == ImgSensorName)
	{
		pImgSensor = new IMX_286();
	}
	else if (_T("IMX_486") == ImgSensorName)
	{
		pImgSensor = new IMX_486();
	}
	else if (_T("IMX_319") == ImgSensorName)
	{
		pImgSensor = new IMX_319();
	}
	else if (_T("OV5695") == ImgSensorName)
	{
		pImgSensor = new OV5695();
	}
	else if (_T("OV13880") == ImgSensorName)
	{
		pImgSensor = new OV13880();
	}
	else if (_T("S5K3M3") == ImgSensorName)
	{
		pImgSensor = new S5K3M3();
	}
	else if (_T("GC2375") == ImgSensorName)
	{
		pImgSensor = new GC2375();
	}
	else if (_T("OV12870") == ImgSensorName)
	{
		pImgSensor = new OV12870();
	}
	else if (_T("S5K3M2xxM3") == ImgSensorName)
	{
		pImgSensor = new S5K3M2xxM3();
	}
	else if (_T("OV5670") == ImgSensorName)
	{
		pImgSensor = new OV5670();
	}
	else if (_T("IMX_362") == ImgSensorName)
	{
		pImgSensor = new IMX_362();
	}
	else if (_T("S5K3P8SP03-FGX") == ImgSensorName)
	{
		pImgSensor = new S5K3P8SP03_FGX9();
	}
	else if (_T("S5K3P9SP04-FGX") == ImgSensorName)
	{
		pImgSensor = new S5K3P9SP04_FGX9();
	}
	else if (_T("S5K3L2") == ImgSensorName)
	{
		pImgSensor = new S5K3L2();
	}
	else if (_T("OV16B10") == ImgSensorName)
	{
		pImgSensor = new OV16B10();
	}
	else if (_T("IMX_371") == ImgSensorName)
	{
		pImgSensor = new IMX_371();
	}
	else if (_T("OV16885") == ImgSensorName)
	{
		pImgSensor = new OV16885();
	}
	else if (_T("OV20880") == ImgSensorName)
	{
		pImgSensor = new OV20880();
	}
	else if (_T("S5K4H7YX") == ImgSensorName)
	{
		pImgSensor = new S5K4H7YX();
	}
	else if (_T("S5K2L7SA") == ImgSensorName)
	{
		pImgSensor = new S5K2L7SA();
	}
	else if( _T("SP2509_MIPI_1600_1200_72M_RAW10" == ImgSensorName))
	{
		pImgSensor = new SP2509();
	}
	else if (_T("OV13358") == ImgSensorName)
	{
		pImgSensor = new OV13358();
	}
	else if (_T("GC5025A") == ImgSensorName)
	{
		pImgSensor = new GC5025A();
	}
	else if (_T("HI_556") == ImgSensorName)
	{
		pImgSensor = new HI_556();
	}
	else if (_T("IMX_268") == ImgSensorName)
	{
		pImgSensor = new IMX_268();
	}
	else if (_T("HI_1333") == ImgSensorName)
	{
		pImgSensor = new HI1333();
	}
	else if (_T("IMX_363") == ImgSensorName)
	{
		pImgSensor = new IMX_363();
	}
	else if (_T("OV8856") == ImgSensorName)
	{
		pImgSensor = new OV8856();
	}
	else if (_T("S5K2L8SX") == ImgSensorName)
	{
		pImgSensor = new S5K2L8SX();
	}
	else if (_T("S5K5E9") == ImgSensorName)
	{
		pImgSensor = new S5K5E9();
	}
	else if (_T("IMX_519") == ImgSensorName)
	{
		pImgSensor = new IMX_519();
	}
	else if (_T("S5K2X7SP03") == ImgSensorName)
	{
		pImgSensor = new S5K2X7SP();
	}
	else if (_T("OV5675") == ImgSensorName)
	{
		pImgSensor = new OV5675();
	}
	else if (_T("IMX_351") == ImgSensorName)
	{
		pImgSensor = new IMX_351();
	}
	else if (_T("IMX_380") == ImgSensorName)
	{
		pImgSensor = new IMX_380();
	}
	else if (_T("IMX_498") == ImgSensorName)
	{
		pImgSensor = new IMX_498();
	}
	else if (_T("IMX_550") == ImgSensorName)
	{
		pImgSensor = new IMX_550();
	}
	else if (_T("IMX_600") == ImgSensorName)
	{
		pImgSensor = new IMX_600();
	}
	else if (_T("OV08A10") == ImgSensorName)
	{
		pImgSensor = new OV08A10();
	}
	else if (_T("OV13A40") == ImgSensorName)
	{
		pImgSensor = new OV13A40();
	}
	else if (_T("OV24A1B") == ImgSensorName)
	{
		pImgSensor = new OV24A1B();
	}
	else if (_T("GC5025") == ImgSensorName)
	{
		pImgSensor = new GC5025();
	}
	else if (_T("S5K2X7SX") == ImgSensorName)
	{
		pImgSensor = new S5K2X7SX();
	}
	else if (_T("S5K3T1") == ImgSensorName)
	{
		pImgSensor = new S5K3T1();
	}
	else if (_T("S5K3L6XX03_FGX9") == ImgSensorName)
	{
		pImgSensor = new S5K3L6XX03_FGX9();
	}
	else if (_T("IMX_576") == ImgSensorName)
	{
		pImgSensor = new IMX_576();
	}
	else if (_T("OV02A10") == ImgSensorName)
	{
		pImgSensor = new OV02A10();
	}
	else if (_T("IMX_476") == ImgSensorName)
	{
		pImgSensor = new IMX_476();
	}
	else if (_T("IMX_355") == ImgSensorName)
	{
		pImgSensor = new IMX_355();
	}
	else 
	{
		return NULL;
	}
	pImgSensor->SetInterface(m_pInterface,SlaveID,CamID);
	return pImgSensor;
}

CImgSensorLib::CImgSensorLib(void)
{

}

CImgSensorLib::~CImgSensorLib(void)
{

}

void CImgSensorLib::SetInterface(ccmBaseInterface* m_pInterface,int SlaveID,int CamID)
{
	pCcmCtr = m_pInterface;
	this->SlaveID =SlaveID;
	this->CamID = CamID;
}

int CImgSensorLib::I2cWrite(USHORT uReg, USHORT Value,BYTE byMode/* =DEFAULT_DEV_ID */)
{
	return pCcmCtr->Device_WriteI2c( SlaveID,  uReg, Value, byMode, CamID/* =DEFAULT_DEV_ID */);
}

int CImgSensorLib::I2cRead( USHORT uReg, USHORT *pValue,BYTE byMode/* =DEFAULT_DEV_ID */)
{
	return pCcmCtr->Device_ReadI2c( SlaveID,  uReg, pValue, byMode, CamID/* =DEFAULT_DEV_ID */);
}

 
int CImgSensorLib::I2cRead(USHORT slaveID,USHORT uReg, USHORT *pValue,BYTE byMode)
{
	return pCcmCtr->Device_ReadI2c( slaveID,  uReg, pValue, byMode, CamID/* =DEFAULT_DEV_ID */);
}
int CImgSensorLib::GetCurentPath(CString &path)
{
	path = pCcmCtr->Ctrl_GetCurrentPath();
	return 1;
}

void CImgSensorLib::Addlog(CString Info)
{
	pCcmCtr->Ctrl_Addlog(CamID,Info,COLOR_BLUE,200);
}

int CImgSensorLib::spcCal(USHORT *imgBuf,short *pSPC)
{
	Addlog(_T("spcCal未设定"));
	return 1;
}
int CImgSensorLib::ApplySpc(short *pSPC)
{
	Addlog(_T("pSPC未设定"));
	return 1;
}

int CImgSensorLib::ReadExp( int &exp) {
	Addlog(_T("ReadExp未设定"));
	return 1;
}
int CImgSensorLib::WriteExp( int exp) {
	Addlog(_T("WriteExp未设定"));
	return 1;
}

int CImgSensorLib::ReadGain( USHORT &gain) {
	Addlog(_T("ReadGain未设定"));
	return 1;
}
int CImgSensorLib::WriteGain( USHORT gain) {
	Addlog(_T("WriteGain未设定"));
	return 1;
}

int CImgSensorLib::GetFuseID(CString &fuseID){
	Addlog(_T("GetFuseID未设定"));
	return 1;
}

int CImgSensorLib::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg){
	Addlog(_T("ApplyAWBGain未设定"));
	return 1;
}
int CImgSensorLib::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup){
	Addlog(_T("ApplyLsc未设定"));
	return 1;
}
int CImgSensorLib::GetLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup){
	Addlog(_T("GetLsc未设定"));
	return 1;
}

int CImgSensorLib::GetTemperature(USHORT &temperature){
	Addlog(_T("GetTemperature未设定"));
	return 1;
}

int CImgSensorLib::PDAF(){
	Addlog(_T("PDAF未设定"));
	return 1;
}

int CImgSensorLib::GetOB()
{
	Addlog(_T("OB未设定,返回默认值32"));
	return 32;
}
void CImgSensorLib::SensorEnable(int gpioNum,BOOL bH)
{
	pCcmCtr->SensorEnable(gpioNum,bH,CamID);
}



int CImgSensorLib::SendWaitAck(void)
{
	int maxbusycnt = 100;    //200us*40 = 8ms
	int ret = 0;
	do
	{
		//ret = FX2I2CMultiBytesWrite(I2cDevAddr,0,0,NULL,0,nDevID);
	//	ret =  I2cWrite(0,2,NULL, 0);
		ret = pCcmCtr->Device_WriteI2cBlock(SlaveID,0,0,NULL,0,0,CamID);
		if (ret == 1)
		{
			break;
		}
	}while(maxbusycnt--);
	return ret;
}
