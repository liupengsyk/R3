#pragma once
#include "deviceinterface.h"
#include "RolongoSDK\HisFX3Platform.h"
class CDevice_R3 :
	public CDeviceInterface
{
public:
	CDevice_R3(void);
	~CDevice_R3(void);
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
	CHisFX3Platform classPlatform[2];
	void GetHisFX3ErrorInfo(int errorcode, CString &errormsg);
	_HisFX3_PreviewStruct stPreviewConfig[2];
	BOOL isOpened;
	bool bDeviceOpened;
	int Turnresult(int iresult);
	BOOL LoadSensorSetting(int CamID);
	USHORT GetMode(int mode);

	int PositiveOpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID);
	int NegtiveOpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID); 
	int SensorpinOpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID);
	int R3_CamID;
	int iStandyCurrent[5];
};

