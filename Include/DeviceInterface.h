// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 DEVICEINTERFACE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// DEVICEINTERFACE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once
#ifdef DEVICEINTERFACE_EXPORTS
#define DEVICEINTERFACE_API __declspec(dllexport)
#else
#define DEVICEINTERFACE_API __declspec(dllimport)
#endif
#include "OFccmDef.h"
// 此类是从 DeviceInterface.dll 导出的
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
	/// @brief SENSOR宽度
	USHORT width;          ///<SENSOR宽度
	/// @brief SENSOR高度
	USHORT height;         ///<SENSOR高度
	/// @brief SENSOR数据类型
	BYTE type;             ///<SENSOR数据类型
	/// @brief SENSOR的RESET和PWDN引脚设置
	BYTE pin;              ///<SENSOR的RESET和PWDN引脚设置
	/// @brief SENSOR的器件地址
	BYTE SlaveID;          ///<SENSOR的器件地址
	/// @brief SENSOR的I2C模式
	BYTE mode;						 ///<SENSOR的I2C模式
	/// @brief SENSOR标志寄存器1.
	USHORT FlagReg;				 ///<SENSOR标志寄存器1.
	/// @brief SENSOR标志寄存器1的值
	USHORT FlagData;			 ///<SENSOR标志寄存器1的值
	/// @brief SENSOR标志寄存器1的掩码值
	USHORT FlagMask;			 ///<SENSOR标志寄存器1的掩码值
	/// @brief SENSOR标志寄存器2.
	USHORT FlagReg1;			 ///<SENSOR标志寄存器2.
	/// @brief SENSOR标志寄存器2的值
	USHORT FlagData1;			 ///<SENSOR标志寄存器2的值
	/// @brief SENSOR标志寄存器2的掩码值
	USHORT FlagMask1;			 ///<SENSOR标志寄存器2的掩码值
	/// @brief SENSOR的名称
	//char name[64];				///<SENSOR的名称
	CString name;
	/// @brief 初始化SENSOR数据表
	USHORT* ParaList;			///<初始化SENSOR数据表
	/// @brief 初始化SENSOR数据表大小，单位字节
	USHORT  ParaListSize; ///<初始化SENSOR数据表大小，单位字节

	/// @brief SENSOR进入Sleep模式的参数表
	USHORT* SleepParaList;	///<SENSOR进入Sleep模式的参数表
	/// @brief SENSOR进入Sleep模式的参数表大小，单位字节
	USHORT  SleepParaListSize;///<SENSOR进入Sleep模式的参数表大小，单位字节

	/// @brief SENSOR输出数据格式，YUV//0:YCbYCr;	//1:YCrYCb;	//2:CbYCrY;	//3:CrYCbY.
	BYTE outformat;         ///<SENSOR输出数据格式，YUV//0:YCbYCr;	//1:YCrYCb;	//2:CbYCrY;	//3:CrYCbY.
	/// @brief SENSOR的输入时钟MCLK，0:12M; 1:24M; 2:48M.
	int mclk;               ///<SENSOR的输入时钟MCLK，0:12M; 1:24M; 2:48M.
	/// @brief SENSOR的AVDD电压值
	float avdd;
	/// @brief SENSOR的DOVDD电压值
	float dovdd;
	/// @brief SENSOR的DVDD电压值		
	float dvdd;

//	BYTE af_vdd;
	float af_vdd;
//	BYTE vpp;
	float vpp;
	BYTE sensorpwdn;
	BYTE sensorreset;
	/// @brief SENSOR的数据接口类型
	BYTE port; 							///<SENSOR的数据接口类型
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
	//bH=0:低平 1：高pin
	virtual void SetGpioPinLevel(int gpioNum,BOOL bH,int CamID) = 0;
	virtual void SensorEnable(int pin,BOOL bH,int CamID)=0;
	virtual BOOL InitiaSensor(int CamID,CString SettingPath)=0;

	/// @brief 读SESNOR寄存器,I2C通讯模式byI2cMode的设置值见I2CMODE定义。
///
/// @param uAddr：从器件地址
/// @param uReg：寄存器地址
/// @param pValue：读到的寄存器的值
/// @param byMode：I2C模式
///
/// @retval DT_ERROR_OK：完成读SENSOR寄存器操作成功
/// @retval DT_ERROR_COMM_ERROR：通讯错误
/// @retval DT_ERROR_PARAMETER_INVALID：byMode参数无效
/// @retval DT_ERROR_TIME_OUT：通讯超时
/// @retval DT_ERROR_INTERNAL_ERROR：内部错误
///
/// @see I2CMODE
	virtual int WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID=0,int CamID=0) = 0;

	/// @brief 写SENSOR寄存器，支持向多个寄存器写入数据（最大支持16个寄存器）。
	///
	/// @param uAddr：从器件地址
	/// @param byI2cMode：I2C模式
	/// @param uRegNum：写入寄存器个数，最大16
	/// @param RegAddr[]：寄存器地址数组
	/// @param RegData[]：写入寄存器的数据
	/// @param RegDelay[]：写完一组寄存器与下一组寄存器之间的延时，单位us
	///
	/// @retval DT_ERROR_OK：完成写SENSOR寄存器操作成功
	/// @retval DT_ERROR_COMM_ERROR：通讯错误
	/// @retval DT_ERROR_PARAMETER_INVALID：uSize参数无效
	/// @retval DT_ERROR_TIME_OUT：通讯超时
	/// @retval DT_ERROR_INTERNAL_ERROR：内部错误
	virtual int ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID=0,int CamID=0) = 0;


	/// @brief 写SENSOR寄存器，支持向一个寄存器写入一个数据块（不超过255字节）。
	///
	/// @param uDevAddr：从器件地址
	/// @param uRegAddr：寄存器地址
	/// @param uRegAddrSize：寄存器地址的字节数
	/// @param pData：写入寄存器的数据块
	/// @param uSize：写入寄存器的数据块的字节数（不超过255字节(HS300/HS300D/HV910/HV910D一次不能超过253字节)）
	///
	/// @retval DT_ERROR_OK：完成写SENSOR寄存器块操作成功
	/// @retval DT_ERROR_COMM_ERROR：通讯错误
	/// @retval DT_ERROR_PARAMETER_INVALID：uSize参数无效
	/// @retval DT_ERROR_TIME_OUT：通讯超时
	/// @retval DT_ERROR_INTERNAL_ERROR：内部错误
	virtual int  WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID= 0,int CamID = 0) = 0;

	/// @brief 读SENSOR寄存器，支持向一个寄存器读出一个数据块（不超过255字节）。
	///
	/// @param uDevAddr：从器件地址
	/// @param uRegAddr：寄存器地址
	/// @param uRegAddrSize：寄存器地址的字节数
	/// @param pData：读到寄存器的值
	/// @param uSize：读出寄存器的数据块的字节数（不超过255字节）
	/// @param bNoStop：是否发出I2C的STOP命令，一般情况下默认为FALSE，bNoStop=TRUE表示写的过程不会有I2C的停止命令，bNoStop=FALSE有I2C的停止命令
	///
	/// @retval DT_ERROR_OK：完成读SENSOR寄存器块操作成功
	/// @retval DT_ERROR_COMM_ERROR：通讯错误
	/// @retval DT_ERROR_PARAMETER_INVALID：uSize参数无效
	/// @retval DT_ERROR_TIME_OUT：通讯超时
	/// @retval DT_ERROR_INTERNAL_ERROR：内部错误
	virtual int  ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop=FALSE,int iDevID= 0,int CamID = 0) = 0;

	CString devName;

};

extern DEVICEINTERFACE_API int nDeviceInterface; 

DEVICEINTERFACE_API CDeviceInterface* GetDeviceType(ccmBaseInterface* m_pInterface,CString DeviceName);

DEVICEINTERFACE_API int fnDeviceInterface(void);
