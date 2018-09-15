// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DEVICEINTERFACE_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DEVICEINTERFACE_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#pragma once
#ifdef DEVICEINTERFACE_EXPORTS
#define DEVICEINTERFACE_API __declspec(dllexport)
#else
#define DEVICEINTERFACE_API __declspec(dllimport)
#endif
#include "OFccmDef.h"
// �����Ǵ� DeviceInterface.dll ������
class ccmBaseInterface;

typedef struct sFrameInfo
{
	BYTE	byChannel;		
	USHORT	uWidth;	
	USHORT	uHeight;	
	UINT	uDataSize;	
	UINT64	uiTimeStamp;
}FrameInfo;
 
typedef struct DeviceSensorTab
{ 
	/// @brief SENSOR���
	USHORT width;          ///<SENSOR���
	/// @brief SENSOR�߶�
	USHORT height;         ///<SENSOR�߶�
	/// @brief SENSOR��������
	BYTE type;             ///<SENSOR��������
	/// @brief SENSOR��RESET��PWDN��������
	BYTE pin;              ///<SENSOR��RESET��PWDN��������
	/// @brief SENSOR��������ַ
	BYTE SlaveID;          ///<SENSOR��������ַ
	/// @brief SENSOR��I2Cģʽ
	BYTE mode;						 ///<SENSOR��I2Cģʽ
	/// @brief SENSOR��־�Ĵ���1.
	USHORT FlagReg;				 ///<SENSOR��־�Ĵ���1.
	/// @brief SENSOR��־�Ĵ���1��ֵ
	USHORT FlagData;			 ///<SENSOR��־�Ĵ���1��ֵ
	/// @brief SENSOR��־�Ĵ���1������ֵ
	USHORT FlagMask;			 ///<SENSOR��־�Ĵ���1������ֵ
	/// @brief SENSOR��־�Ĵ���2.
	USHORT FlagReg1;			 ///<SENSOR��־�Ĵ���2.
	/// @brief SENSOR��־�Ĵ���2��ֵ
	USHORT FlagData1;			 ///<SENSOR��־�Ĵ���2��ֵ
	/// @brief SENSOR��־�Ĵ���2������ֵ
	USHORT FlagMask1;			 ///<SENSOR��־�Ĵ���2������ֵ
	/// @brief SENSOR������
	//char name[64];				///<SENSOR������
	CString name;
	/// @brief ��ʼ��SENSOR���ݱ�
	USHORT* ParaList;			///<��ʼ��SENSOR���ݱ�
	/// @brief ��ʼ��SENSOR���ݱ��С����λ�ֽ�
	USHORT  ParaListSize; ///<��ʼ��SENSOR���ݱ��С����λ�ֽ�

	/// @brief SENSOR����Sleepģʽ�Ĳ�����
	USHORT* SleepParaList;	///<SENSOR����Sleepģʽ�Ĳ�����
	/// @brief SENSOR����Sleepģʽ�Ĳ������С����λ�ֽ�
	USHORT  SleepParaListSize;///<SENSOR����Sleepģʽ�Ĳ������С����λ�ֽ�

	/// @brief SENSOR������ݸ�ʽ��YUV//0:YCbYCr;	//1:YCrYCb;	//2:CbYCrY;	//3:CrYCbY.
	BYTE outformat;         ///<SENSOR������ݸ�ʽ��YUV//0:YCbYCr;	//1:YCrYCb;	//2:CbYCrY;	//3:CrYCbY.
	/// @brief SENSOR������ʱ��MCLK��0:12M; 1:24M; 2:48M.
	int mclk;               ///<SENSOR������ʱ��MCLK��0:12M; 1:24M; 2:48M.
	/// @brief SENSOR��AVDD��ѹֵ
	float avdd;
	/// @brief SENSOR��DOVDD��ѹֵ
	float dovdd;
	/// @brief SENSOR��DVDD��ѹֵ		
	float dvdd;

//	BYTE af_vdd;
	float af_vdd;
//	BYTE vpp;
	float vpp;
	BYTE sensorpwdn;
	BYTE sensorreset;
	/// @brief SENSOR�����ݽӿ�����
	BYTE port; 							///<SENSOR�����ݽӿ�����
	USHORT Ext0;
	USHORT Ext1;
	USHORT Ext2; 
	int CamID;
	BOOL bTestStandyCurrent;
	int iXRoi;
	int iYRoi;
	 
	DeviceSensorTab()
	{
		width=0;
		height=0;
		type=0;
		pin=0;
		SlaveID=0;
		mode=0;
		FlagReg=0;
		FlagData=0;
		FlagMask=0;
		FlagReg1=0;
		FlagData1=0;
		FlagMask1=0;
		//memset(name,0,sizeof(name));

		ParaList=NULL;
		ParaListSize=0;
		SleepParaList=NULL;
		SleepParaListSize=0;

		outformat=0;
		mclk=0;               //0:12M; 1:24M; 2:48M.
		avdd = 0.0;               // 
		dovdd = 0.0;              //
		dvdd = 0.0; 
		af_vdd = 0.0;
		vpp = 0.0;
		port=0; 	
		Ext0=0;
		Ext1=0;
		Ext2=0;  
		CamID = 0;
		bTestStandyCurrent=FALSE;
		iXRoi = 0;
		iYRoi = 0;
	}
}DeviceSensorTab, *pDeviceSensorTab;

class DEVICEINTERFACE_API CDeviceInterface {
public:
	CDeviceInterface(void);
	~CDeviceInterface(void); 

protected:
	ccmBaseInterface* pCcmCtr;
public:
	 void SetInterface(ccmBaseInterface* m_pInterface);

	 CString strAppName;
	
	/***********Interface from  base*********/ 
public: 
	void AddLog(int CamID,CString Info,COLORREF CurrentColor,int InfoSize = 200);
	CString ReadConfigString(CString KeyName,CString defaultValue,int CamID); 
	int ReadConfigData(CString KeyName,int defaultValue,int CamID); 
	void WriteConfigString(CString KeyName,CString defaultValue,int CamID); 
	void WriteConfigData(CString KeyName,int defaultValue,int CamID);
	HWND GetCamDisplayHwnd(int CamID);
public:
	BOOL LoadSensorParalist(int CamID);
	BOOL LoadSensorGenaralTab(int CamID);
	DeviceSensorTab CamSensorTab[4];
	void DriverICStandby(int CamID);
	
public: 
	virtual void SetDevName(CString devName);
	virtual vector<CString> GetDevList();
	virtual int  GetKitTypeDeviceInfo(int CamID) = 0;
	virtual int  OpenDevice(CString DeviceName) = 0;
	virtual int  CloseDevice(int CamID= 0) = 0;
	virtual int  OpenVideo(int CamID= 0) = 0;

	virtual int PmuGetCurrent(int Current[],int iCount,int iDevID=0,int CamID=0) =0;
	virtual int PmuGetVoltage(int Voltage[],int iCount,int iDevID=0,int CamID=0) = 0;
	virtual int PmuGetStandyCurrent(int Current[],int iCount,int iDevID=0,int CamID=0) =0;

	virtual int OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) = 0;

	virtual int GetGrabSize()=0;
	virtual int  GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int iDevID=0) = 0;
	virtual void SetCamTab(int CamID) = 0;
	//gpioNum=1:pwdn 2:reset 3:pwdn&reset
	//bH=0:��ƽ 1����pin
	virtual void SetGpioPinLevel(int gpioNum,BOOL bH,int CamID) = 0;
	virtual void SensorEnable(int pin,BOOL bH,int CamID)=0;
	virtual BOOL InitiaSensor(int CamID,CString SettingPath)=0;

	/// @brief ��SESNOR�Ĵ���,I2CͨѶģʽbyI2cMode������ֵ��I2CMODE���塣
///
/// @param uAddr����������ַ
/// @param uReg���Ĵ�����ַ
/// @param pValue�������ļĴ�����ֵ
/// @param byMode��I2Cģʽ
///
/// @retval DT_ERROR_OK����ɶ�SENSOR�Ĵ��������ɹ�
/// @retval DT_ERROR_COMM_ERROR��ͨѶ����
/// @retval DT_ERROR_PARAMETER_INVALID��byMode������Ч
/// @retval DT_ERROR_TIME_OUT��ͨѶ��ʱ
/// @retval DT_ERROR_INTERNAL_ERROR���ڲ�����
///
/// @see I2CMODE
	virtual int WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID=0,int CamID=0) = 0;

	/// @brief дSENSOR�Ĵ�����֧�������Ĵ���д�����ݣ����֧��16���Ĵ�������
	///
	/// @param uAddr����������ַ
	/// @param byI2cMode��I2Cģʽ
	/// @param uRegNum��д��Ĵ������������16
	/// @param RegAddr[]���Ĵ�����ַ����
	/// @param RegData[]��д��Ĵ���������
	/// @param RegDelay[]��д��һ��Ĵ�������һ��Ĵ���֮�����ʱ����λus
	///
	/// @retval DT_ERROR_OK�����дSENSOR�Ĵ��������ɹ�
	/// @retval DT_ERROR_COMM_ERROR��ͨѶ����
	/// @retval DT_ERROR_PARAMETER_INVALID��uSize������Ч
	/// @retval DT_ERROR_TIME_OUT��ͨѶ��ʱ
	/// @retval DT_ERROR_INTERNAL_ERROR���ڲ�����
	virtual int ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID=0,int CamID=0) = 0;


	/// @brief дSENSOR�Ĵ�����֧����һ���Ĵ���д��һ�����ݿ飨������255�ֽڣ���
	///
	/// @param uDevAddr����������ַ
	/// @param uRegAddr���Ĵ�����ַ
	/// @param uRegAddrSize���Ĵ�����ַ���ֽ���
	/// @param pData��д��Ĵ��������ݿ�
	/// @param uSize��д��Ĵ��������ݿ���ֽ�����������255�ֽ�(HS300/HS300D/HV910/HV910Dһ�β��ܳ���253�ֽ�)��
	///
	/// @retval DT_ERROR_OK�����дSENSOR�Ĵ���������ɹ�
	/// @retval DT_ERROR_COMM_ERROR��ͨѶ����
	/// @retval DT_ERROR_PARAMETER_INVALID��uSize������Ч
	/// @retval DT_ERROR_TIME_OUT��ͨѶ��ʱ
	/// @retval DT_ERROR_INTERNAL_ERROR���ڲ�����
	virtual int  WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID= 0,int CamID = 0) = 0;

	/// @brief ��SENSOR�Ĵ�����֧����һ���Ĵ�������һ�����ݿ飨������255�ֽڣ���
	///
	/// @param uDevAddr����������ַ
	/// @param uRegAddr���Ĵ�����ַ
	/// @param uRegAddrSize���Ĵ�����ַ���ֽ���
	/// @param pData�������Ĵ�����ֵ
	/// @param uSize�������Ĵ��������ݿ���ֽ�����������255�ֽڣ�
	/// @param bNoStop���Ƿ񷢳�I2C��STOP���һ�������Ĭ��ΪFALSE��bNoStop=TRUE��ʾд�Ĺ��̲�����I2C��ֹͣ���bNoStop=FALSE��I2C��ֹͣ����
	///
	/// @retval DT_ERROR_OK����ɶ�SENSOR�Ĵ���������ɹ�
	/// @retval DT_ERROR_COMM_ERROR��ͨѶ����
	/// @retval DT_ERROR_PARAMETER_INVALID��uSize������Ч
	/// @retval DT_ERROR_TIME_OUT��ͨѶ��ʱ
	/// @retval DT_ERROR_INTERNAL_ERROR���ڲ�����
	virtual int  ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID= 0,int CamID = 0) = 0;

	CString devName;

};

extern DEVICEINTERFACE_API int nDeviceInterface; 

DEVICEINTERFACE_API CDeviceInterface* GetDeviceType(ccmBaseInterface* m_pInterface,CString DeviceName);

DEVICEINTERFACE_API int fnDeviceInterface(void);
