#pragma once

#include "deviceinterface.h"
#include "dtccm2_mud952\imagekit.h"
#include "dtccm2_mud952\dtccm2.h"
#include <vector>
using namespace std;


#define  GPIO_NUMBER 76

class CDevice_UH920 :
	public CDeviceInterface
{
public:
	CDevice_UH920(void);
	~CDevice_UH920(void);

	virtual int  GetKitTypeDeviceInfo(int CamID) ; 

	virtual int  OpenDevice(CString DeviceName) ;
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
	virtual BOOL InitiaSensor(int CamID,CString SettingPath);

private:
	BYTE m_uSensorPort;
	BYTE m_iChanSel;
	CString strDevice;
	BYTE Channel;

	int m_nDevID;

	int DtPowerOn(BYTE byChannel);
	int DtPowerClose(BYTE byChannel);
	int SetMclk(BYTE byChannel);
	int SetSensorPwdnReset(BYTE byChannel);
	int SensorFind(BYTE byChannel);
	int SensorInit(BYTE byChannel);
	int DeviceInit(BYTE byChannel);
	int OpenVideo(BYTE byChannel);
	int DtSensorTest(); 
	int DtSensorTest0(int Camera); 
	int DtSensorTest1(int Camera);

	ULONG uGrabSize;
	 
	int SizeA;
	int SizeB;

private:
	float m_fOSVoltage;
	float m_fOSLimitHigh;
	float m_fOSLimitLow;
	float m_fOSPowerCurrent;
	float m_fOSGpioCurrent;

	int DtClosePower(int byChannel);

	 
	BOOL m_bOSCommand;
	BOOL m_bLCCommand;
	BOOL m_bAPort;
	BOOL m_bBPort;
	BOOL m_bOSHigh;
	BOOL m_bOSLow;
	BOOL m_bLCHigh;
	BOOL m_bLCLow;

	 

	BOOL m_bAMipi;
	BOOL m_bAOtherIO;
	BOOL m_bBMipi;
	BOOL m_bBOtherIO;

	vector<CString> m_listOS;
	vector<CString> m_wndDevList;

	ULONG Size;

	int DevicesTol;

	BOOL isOpen;

};

