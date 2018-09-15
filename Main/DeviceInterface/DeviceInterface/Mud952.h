#pragma once
#include "deviceinterface.h"
#include "dtccm2_mud952\imagekit.h"
#include "dtccm2_mud952\dtccm2.h"
#include <vector>
using namespace std;

class Mud952:public CDeviceInterface
{
public:
	Mud952(void);
	~Mud952(void);
public:  
	virtual vector<CString> GetDevList();
	virtual int  GetKitTypeDeviceInfo(int CamID) ; 

	virtual int OpenDevice(CString devName);
	virtual int  OpenMUD952(int CamID= 0) ;
	virtual int  CloseDevice(int CamID= 0) ;
	virtual int  OpenVideo(int CamID= 0) ;

	virtual int PmuGetCurrent(int Current[],int iCount,int iDevID=DEFAULT_DEV_ID,int CamID=0);
	virtual int PmuGetVoltage(int Voltage[],int iCount,int iDevID=DEFAULT_DEV_ID,int CamID=0);
	virtual int PmuGetStandyCurrent(int Current[],int iCount,int iDevID=0,int CamID=0);

	virtual int OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) ;

    virtual int  GetGrabSize();
	virtual int  GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int iDevID=0) ;

	virtual int WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID=0,int CamID=0);	 
	virtual int ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID=0,int CamID=0);

	virtual int  WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID= 0,int CamID = 0) ;
	virtual int  ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID= 0,int CamID = 0) ;
	 
	virtual void SetCamTab(int CamID);

	virtual void SetGpioPinLevel(int gpioNum,BOOL bH,int CamID);
	virtual void SensorEnable(int pin,BOOL bH,int CamID);
	virtual BOOL Mud952::InitiaSensor(int CamID,CString SettingPath);

	int DTOSTest(OSTab *pOSTab, int* OSValueArray,int CamID);
private:
	 	
	/*static*/ int m_nDevID[4];
	CString devVec[4];
	BOOL EnumDevice();

	/* 度信设备名称 */
	CString		m_strDevNameA;
	CString		m_strDevNameB;

	 int Size;
	 ULONG uGrabSize;
	 int SetSoftPin(int SensorPort ,int iDevID/* = 0 */);
	 int InitCamera( int CamID);
	 int DevicesTol;
	 BOOL isOpened;
	 int iStandyCurrent_Left[5];
	 int iStandyCurrent_Right[5];
	 BOOL bOpenShortFinish[2];//仅限共基板时使用

};

