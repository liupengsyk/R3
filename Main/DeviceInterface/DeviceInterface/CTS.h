#pragma once
#include "deviceinterface.h"
#include "CTS_SDK/cts.h"


typedef struct _CTSSensorTab
{
	float DVDD;
	float AVDD;
	float DOVDD;
	float AFVDD;
	float PWR4;
	float PWR5;

	BYTE SENSOR_POWERDOWN_ACTIVE;
	BYTE SENSOR_RESET_ACTIVE;
	DWORD OPERATION_SEQUENCE;
	BYTE SEQUENCE_DELAY;
	float SENSOR_IO_LEVEL;
	BYTE I2C_SPEED;
	BYTE SENSOR_I2C_ID;
	BYTE SENSOR_REG_ADDR_WIDTH;
	BYTE SENSOR_REG_DATA_WIDTH;
	float SENSOR_FRAME_RATE;
	float SENSOR_EXTCLK;
	WORD SENSOR_OUTPUT_WIDTH;
	WORD SENSOR_OUTPUT_HEIGHT;
	
	BYTE USB3_BANDWIDTH;
	
	BYTE DATA_PATH;
	BYTE SENSOR_OUTPUT_FORMAT;
	BYTE MIPI_LANE_NUMBER;
	BYTE MIPI_T_hs_zero_MASK;

	WORD DSP_SAMPLE_START_PIX_X;
	WORD DSP_SAMPLE_START_PIX_Y;
	WORD DSP_OUTPUT_WIDTH;
	WORD DSP_OUTPUT_HEIGHT;

	BYTE BAYER_PIX_ORDER;
	BYTE YUV_PIX_ORDER;
	BYTE SENSOR_PCLK_SAMPLE_EDGE;

	BYTE SENSOR_HSYNC_POLARITY;
	BYTE SENSOR_VSYNC_POLARITY;

	BYTE MIPI_AUTO_SKEW_CALIBRATION;
	BYTE MIPI_CLK_DATA0_SKEW_CALIBRATION;
	BYTE MIPI_CLK_DATA1_SKEW_CALIBRATION;
	BYTE MIPI_CLK_DATA2_SKEW_CALIBRATION;
	BYTE MIPI_CLK_DATA3_SKEW_CALIBRATION;

	
	vector<tagREG> vecRegSettings;


	_CTSSensorTab()
	{
		DVDD = 0;
		AVDD = 0;
		DOVDD = 0;
		AFVDD = 0;
		PWR4 = 0;
		PWR5 = 0;

		SENSOR_POWERDOWN_ACTIVE = 0;
		SENSOR_RESET_ACTIVE = 0;
		OPERATION_SEQUENCE = 0;
		SEQUENCE_DELAY = 0;
		SENSOR_IO_LEVEL = 0;
		I2C_SPEED = 0;
		SENSOR_I2C_ID = 0;
		SENSOR_REG_ADDR_WIDTH = 0;
		SENSOR_REG_DATA_WIDTH = 0;
		SENSOR_FRAME_RATE = 0;
		SENSOR_EXTCLK = 0;
		SENSOR_OUTPUT_WIDTH = 0;
		SENSOR_OUTPUT_HEIGHT = 0;
		
		USB3_BANDWIDTH = 0;
		
		DATA_PATH = 0;
		SENSOR_OUTPUT_FORMAT = 0;
		MIPI_LANE_NUMBER = 0;
		MIPI_T_hs_zero_MASK = 0;

		DSP_SAMPLE_START_PIX_X = 0;
		DSP_SAMPLE_START_PIX_Y = 0;
		DSP_OUTPUT_WIDTH = 0;
		DSP_OUTPUT_HEIGHT = 0;
		
		YUV_PIX_ORDER = 0;
		SENSOR_PCLK_SAMPLE_EDGE = 0;

		SENSOR_HSYNC_POLARITY = 0;
		SENSOR_VSYNC_POLARITY = 0;

		MIPI_AUTO_SKEW_CALIBRATION = 0;
		MIPI_CLK_DATA0_SKEW_CALIBRATION = 0;
		MIPI_CLK_DATA1_SKEW_CALIBRATION = 0;
		MIPI_CLK_DATA2_SKEW_CALIBRATION = 0;
		MIPI_CLK_DATA3_SKEW_CALIBRATION = 0;

	}
	
}CTSSensorTab;


class CCTS :
	public CDeviceInterface
{
public:
	CCTS(void);
	~CCTS(void);
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
	BOOL LoadSensorSetting(int CamID);

public:
	BOOL bCamInit;
	BOOL m_bDualCamMode;
	typedef struct _CTSInfo
	{
		BOOL bSensorInit;
		BOOL bBuildPreview;
		BOOL bVideoPlay;
		int devInst;

		_CTSInfo()
		{
			bSensorInit = 0;
			bBuildPreview = 0;
			bVideoPlay=0;
			devInst = 0;
		};

	}CTSInfo;
	CTSInfo m_MyCTS[2];


	CTSSensorTab CTSSensor;
	int CTS_CamID;
	int DelayTime;

protected:
	 BOOL ParseDTINItoCTSStruct(int CamID);
	 DWORD Cam_SetConfigExFromAPIDT(DWORD devInst);
	 BOOL StartPreview(int CamID);
	 void StopPreview(int CamID);
	 BOOL StartPlay(int CamID);
	 BOOL StopPlay(int CamID);
	 void OS_CTSToDT(int *OSValueArray, LOGINFO OsLogInfo);



	 BOOL InitOS(int CamID);
	 BOOL OS_SetConfigFromAPI(int CamID);
	 BOOL OS_Test(int CamID, LOGINFO &OsLogInfo);
	 int  CloseVideo(int CamID);
	 void OS_CTSToDT_DVP(int *OSValueArray, LOGINFO OsLogInfo);
};

