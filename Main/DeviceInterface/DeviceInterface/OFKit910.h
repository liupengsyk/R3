#pragma once
#include "deviceinterface.h"
 #include "dtccm2_mud952\imagekit.h"
 #include "dtccm2_mud952\dtccm2.h"
//   #include "DTCCM2_SDK\imagekit.h"
//   #include "DTCCM2_SDK\dtccm2.h"

#include <vector>
using namespace std;
class OFKit910 : public CDeviceInterface
{
public:
	OFKit910();
	~OFKit910(void);

	 virtual vector<CString> GetDevList();

	 virtual int  GetKitTypeDeviceInfo(int CamID) ; 

	 virtual int  OpenDevice(CString DeviceName) ;

	 virtual int  CloseDevice(int CamID= 0) ;

	 virtual int  OpenVideo(int CamID= 0) ;

	 virtual int PmuGetCurrent(int Current[],int iCount,int iDevID=0,int CamID=0) ;
	 virtual int PmuGetVoltage(int Voltage[],int iCount,int iDevID=0,int CamID=0) ;
	 virtual int PmuGetStandyCurrent(int Current[],int iCount,int iDevID=0,int CamID=0);

	 virtual int OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) ;

	 virtual int GetGrabSize();
	 virtual int  GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int CamID=0) ;

	 virtual int WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID=0,int CamID=0);	 
	 virtual int ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID=0,int CamID=0);

	 virtual int  WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID= 0,int CamID = 0) ;
	 virtual int  ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID=0 ,int CamID = 0) ;

	 virtual void SetCamTab(int CamID);

	 virtual void SetGpioPinLevel(int gpioNum,BOOL bH,int CamID);
	 virtual void SensorEnable(int pin,BOOL bH,int CamID);
	 virtual BOOL InitiaSensor(int CamID,CString SettingPath);

private:
	  
	 CString m_wndDevList[4];
	int m_nDevID[4];
	BOOL isOpened;
	int iStandyCurrent[5];
	 
	 int Size;
	 int DevicesTol;
	 ULONG uGrabSize;
	 int SetSoftPin(int SensorPort ,int iDevID = 0 );
	 
};

