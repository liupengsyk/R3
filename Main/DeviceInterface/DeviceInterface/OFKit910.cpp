#include "StdAfx.h"
#include "ccmBaseInterface.h"
#include "DeviceInterface.h"
#include "OFKit910.h"

#pragma comment(lib,"dtccm2_mud952/dtccm2.lib")
//#pragma comment(lib,"DTCCM2_SDK/dtccm2.lib")

OFKit910::OFKit910()
{ 
	for(int i=0;i<4;i++)
	{
		m_nDevID[i] = -1;
	}
	isOpened = FALSE;
}

OFKit910::~OFKit910(void)
{

} 

void OFKit910::SetCamTab(int CamID)
{
	LoadSensorGenaralTab(CamID); 
	LoadSensorParalist(CamID); 
}

void OFKit910::SetGpioPinLevel(int gpioNum,BOOL bH,int CamID)
{
	::SetGpioPinLevel(gpioNum,bH,m_nDevID[CamID]);
}
void OFKit910::SensorEnable(int gpioNum,BOOL bH,int CamID)
{
	//::SetGpioPinLevel(gpioNum,bH,m_nDevID[CamID]);
	//gpioNum=1:pwdn 2:reset 3:pwdn&reset
	//bH=0:低平 1：高pin
	if (bH==1)
	{
		if((m_nDevID[CamID]>=0)&&(m_nDevID[CamID]<4))
		::SensorEnable(gpioNum,TRUE,m_nDevID[CamID]);
	}
	else
	{
		if((m_nDevID[CamID]>=0)||(m_nDevID[CamID]<4))
		::SensorEnable(gpioNum^3,TRUE,m_nDevID[CamID]);
	}
}
BOOL OFKit910::InitiaSensor(int CamID,CString SettingPath)
{

	if (SettingPath.GetLength()>4)
	{
       ;//添加New 点亮设定代码
	}
	/* 设置reset pin,pwdn pin*/
	::CloseVideo(m_nDevID[CamID]);

	if (SetSensorClock(TRUE,(USHORT)(CamSensorTab[CamID].mclk *10),m_nDevID[CamID]) != DT_ERROR_OK) // 20MHz
	{
		AddLog(CamID,L"设置量设置SENSOR时钟失败程失败!",COLOR_RED);  
		return DT_ERROR_FAILED;
	}

	::SensorEnable(CamSensorTab[CamID].pin^RESET_H,TRUE,m_nDevID[CamID]);
	Sleep(20);
	::SensorEnable(CamSensorTab[CamID].pin,TRUE,m_nDevID[CamID]);
	Sleep(50);

	if (InitSensor(CamSensorTab[CamID].SlaveID,CamSensorTab[CamID].ParaList,CamSensorTab[CamID].ParaListSize,CamSensorTab[CamID].mode,m_nDevID[CamID]) != DT_ERROR_OK)
	{
		AddLog(CamID,_T("sensor初始化失败"),COLOR_RED);
		return DT_ERROR_FAILED;
	}

	if (CamSensorTab[CamID].outformat == D_YUV || CamSensorTab[CamID].outformat == D_YUV_MTK_S || CamSensorTab[CamID].outformat == D_YUV_SPI)
		SetYUV422Format(CamSensorTab[0].outformat,m_nDevID[CamID]);
	else
		SetRawFormat(CamSensorTab[CamID].outformat,m_nDevID[CamID]);

	/* 获得图像数据 */
	Size = GetByteOffset(CamSensorTab[CamID].width,CamSensorTab[CamID].outformat,m_nDevID[CamID])*CamSensorTab[CamID].height;


	// A通道ROI
	SetRoiEx(0+CamSensorTab[CamID].iXRoi, 0+CamSensorTab[CamID].iYRoi, CamSensorTab[CamID].width, CamSensorTab[CamID].height, 0, 0 , 1, 1, 1,m_nDevID[CamID]);
	SetMipiClkPhase(0,m_nDevID[CamID]);	
	Sleep(10);
	// 计算A通道实际采集数据量
	CalculateGrabSize(&uGrabSize,m_nDevID[CamID]); 
	// 打开视频
	::OpenVideo(Size,m_nDevID[CamID]); 
	// 设置要抓取的通道

	//step 8补上gamma 对比度 饱和度测试
	SetGamma(100);  //Gamma  :default 100 is no gamma change....
	SetContrast(100); //Contrast :default 100 is no contrast change...
	SetSaturation(128);//Saturation: defualt 128 is no saturation change...
	SetDigitalGain(1.0, 1.0, 1.0); //AWB digital gian R G B

}

vector<CString> OFKit910::GetDevList()
{
	char *DeviceName[4]={0};
	int DeviceNum;
	vector<CString> DevNameList;
	EnumerateDevice(DeviceName,4,&DeviceNum);
	for(int i=0;i<DeviceNum;i++)
	{
		if(m_nDevID[i]>=0)
		{
			AfxMessageBox(_T("请先关闭模组,再进入"));
			return DevNameList;
		}
		if(::OpenDevice(DeviceName[i],&m_nDevID[i])!= DT_ERROR_OK)
		{
			AddLog(0,_T("Open Device failed."),RGB(255,0,0),200);
			continue;
		}

		BYTE  TempDeviceSN[512];
		int RetLen=0;
		GetDeviceSN(TempDeviceSN,512,&RetLen,m_nDevID[i]);

		CString Tempstr1;
		CString Tempstr2=_T("");
		for (int j=0;j<8;j++)
		{
			Tempstr1.Format(_T("%02X"),TempDeviceSN[j]);
			Tempstr2=Tempstr2+Tempstr1;

		}
		CloseDevice(m_nDevID[i]);
		m_nDevID[i]=-1;
		CString st(DeviceName[i]);
		st=Tempstr2+_T("_")+st;
		DevNameList.push_back(st);
		if(DeviceName[i]!=NULL)
			GlobalFree(DeviceName[i]);

	}
	return DevNameList;
}
int  OFKit910::GetKitTypeDeviceInfo(int CamID)
{
	if (devName.GetLength()<3)
	{
		CString str;
		str.Format(_T("没有指定度信:%s，请重新设定度信"),devName);
		AddLog(CamID,str,COLOR_RED,200); 
		return -1;
	}
	BOOL devFind = FALSE;
	char *DeviceName[4]={0};
	int DeviceNum;
	 
	for(int i=0;i<4;i++)
	{
		if (m_nDevID[CamID] >= 0)
			::CloseDevice(m_nDevID[CamID]);
	}

	EnumerateDevice(DeviceName,4,&DeviceNum);
	 
	isOpened = TRUE;
    DevicesTol = DeviceNum;

	int index=devName.Find(_T("_"));
	CString CurrentSN=devName.Left(index);
	devName=devName.Mid(index+1);
	
	for (int i=0; i<DeviceNum; i++)
	{ 
		CString st(DeviceName[i]); 
		if (st==devName)
		{
			CString Tempstr1;
			devFind = TRUE;
			if (::OpenDevice(DeviceName[i],&m_nDevID[CamID]) == DT_ERROR_OK)
			{
				Tempstr1.Format(_T("打开设备成功，DevID：%d"),m_nDevID[CamID]);
				AddLog(CamID,Tempstr1,COLOR_BLUE,200); 
			}
			BYTE  TempDeviceSN[512];
			int RetLen=0;
			GetDeviceSN(TempDeviceSN,512,&RetLen,m_nDevID[CamID]);
			CString Tempstr2=_T("");
			for (int j=0;j<8;j++) 
			{
				Tempstr1.Format(_T("%02X"),TempDeviceSN[j]);
				Tempstr2=Tempstr2+Tempstr1;
			}
			if (Tempstr2!=CurrentSN)
			{
				AddLog(CamID,_T("没有找到SN：")+CurrentSN+_T("当前SN:")+Tempstr2,RGB(255,0,0),200);
				devFind=FALSE;
			}
			else AddLog(CamID,_T("找到SN：")+ CurrentSN,RGB(0,255,0),200);
		} 

		if (DeviceName[i] != NULL)
		{
			TRACE("Found device:%s\n",DeviceName[i]);
			 
			m_wndDevList[i] = st;			
			GlobalFree(DeviceName[i]);
		}
	}

	if (!devFind)
	{
		CString str;
		str.Format(_T("没有找到指定度信:%s，请重新设定度信"),devName);
		AddLog(CamID,str,COLOR_RED,200); 
		return -1;
	}
	 
	return DeviceNum;

}
 
int  OFKit910::CloseDevice(int CamID)
{
//	SetMipiEnable(FALSE);
	//buffer
	CloseVideo(m_nDevID[CamID]);
	ResetSensorI2cBus(m_nDevID[CamID]);
	SensorEnable(CamSensorTab[CamID].pin ^ 3, 1, CamID);
	Sleep(20);
	SetSensorClock(FALSE, CamSensorTab[CamID].mclk * 10, m_nDevID[CamID]);
	SENSOR_POWER Power[10];
	int Volt[10];
	BOOL OnOff[10];


	Power[0] = POWER_AVDD;
	Volt[0] = 0;
	OnOff[0] = FALSE;


	Power[1] = POWER_DOVDD;
	Volt[1] = 0	;	
	OnOff[1] =FALSE;


	Power[2] = POWER_DVDD;
	Volt[2] = 0;
	OnOff[2] =FALSE;

	Power[3] = POWER_AFVCC;
	Volt[3] = 0; 
	OnOff[3] = FALSE;


	Power[4] = POWER_VPP;
	Volt[4] = 0; 
	OnOff[4] = FALSE;

	SetSoftPinPullUp(IO_NOPULL, m_nDevID[CamID]);
	//设置5路电压值
	PmuSetVoltage(Power,Volt,5,m_nDevID[CamID]);			 
	Sleep(50);
// 	EnableSoftPin(FALSE, m_nDevID[CamID]);  //度信建议910不执行这两个函数
// 	EnableGpio(FALSE, m_nDevID[CamID]);

// 	//设置5路电压值
// 	PmuSetVoltage(Power,Volt,5,m_nDevID[CamID]);			 
// 	//设置电压开关
// 	PmuSetOnOff(Power,OnOff,5,m_nDevID[CamID]) ;
// 	//使能柔性接口
// 	EnableSoftPin(FALSE,m_nDevID[CamID]);
// 	EnableGpio(FALSE,m_nDevID[CamID]);
// 

// 	CloseVideo(m_nDevID[CamID]);

	if (m_nDevID[CamID] >= 0&&m_nDevID[CamID]<500)
	{
		::CloseDevice(m_nDevID[CamID]);
		m_nDevID[CamID]=-1;
	}
	isOpened = FALSE;
	return 0;
}

  int OFKit910::OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) 
{
	BOOL bFlag  = FALSE;
	CString str;
	str.Format(_T("Cam %d 工装启动中OS"),CamID);
	AddLog(CamID,str,COLOR_BLUE,200); 
	
	SENSOR_POWER Power[10];
	int Volt[10];
	BOOL OnOff[10];

	Power[0] = POWER_AVDD;
	Volt[0] = 1500; // 2.8V
	OnOff[0] = FALSE;

	Power[1] = POWER_DOVDD;
	Volt[1] = 1500; // 1.8V
	OnOff[1] = FALSE;

	Power[2] = POWER_DVDD;
	Volt[2] = 1500;// 1.2V
	OnOff[2] = FALSE;

	Power[3] = POWER_AFVCC;
	Volt[3] = 1500; // 2.8V
	OnOff[3] = FALSE;

	Power[4] = POWER_VPP;
	Volt[4] = 1500; 
	OnOff[4] = FALSE;
	//设置5路电压值

	PmuSetVoltage(Power, Volt, 5, m_nDevID[CamID]);
	PmuSetOnOff(Power, OnOff, 5, m_nDevID[CamID]);

	USHORT m_OpenStd = 0x1ff-50;  //0x400 equal to 3V...
	USHORT m_ShortStd = 50;

	int HighLimit[45] = {0};
	int LowLimit[45] = {0};

	for(int i = 0; i < 45; i++)
	{
		HighLimit[i] = (int)(m_OpenStd * 1.5f * 1000000/512) ; //转变为uV信号	
		LowLimit[i] =  (int)(m_ShortStd *1.5f * 1000000/512) ;
	}
	OS_Config((int)(1.5f*1000*1000), HighLimit, LowLimit, 45, 2*1000, 1*1000,0);

	//OS 测试，进行该PIN为low的测试。
	//command bit7 :enalble os ; bit3:high test; bit2:low test...
	ULONG PinMask_L = pOSTab->PinMask_L; 
	ULONG PinMask_H = pOSTab->PinMask_H;  

	UCHAR IoMask[10] = {0};
	IoMask[0] = (UCHAR)((PinMask_L) & 0xff);
	IoMask[1] = (UCHAR)((PinMask_L>>8) & 0xff);
	IoMask[2] = (UCHAR)((PinMask_L>>16) & 0xff);
	IoMask[3] = (UCHAR)((PinMask_L>>24) & 0xff);

	IoMask[4] = (UCHAR)((PinMask_H) & 0xff);
	IoMask[5] = (UCHAR)((PinMask_H>>8) & 0xff);
	IoMask[6] = (UCHAR)((PinMask_H>>16) & 0xff);
	IoMask[7] = (UCHAR)((PinMask_H>>24) & 0xff);
	LC_OS_CommandConfig(0x80 | 0x04, IoMask, 10,0);

	int VoltageH[45] = {0};
	int VoltageL[45] = {0};
	UCHAR Result[45] = {0};

	int nRet = OS_Read(VoltageH, VoltageL, Result, 0, 1, 45,0);
	
	if( 1 == nRet)
	{
		for (int i=0; i<45; i++)
		{
			OSValueArray[i] = VoltageL[i];
		}
	}
	else
	{  
		/*CString str;
		str.
(_T("开短路错误%d，请重试"), nRet);
		AfxMessageBox(str);*/
		return -3;
	}
	return 1;
}
int  OFKit910::OpenVideo(int CamID)
{
	//OpenDevice(m_wndDevList[0]);

#if 1
	UCHAR ImgFormat = D_RAW8;

	UCHAR PinDef[26] = {PIN_NC};

	CString str; 
	int iRet = TRUE;

	TRACE("SensorTest Start\r\n"); 
	CString sTemp ; 
	// 设置通道
	ImgFormat = CamSensorTab[CamID].type;
 
	SetMipiEnable(TRUE);
	SetSoftPinPullUp(IO_NOPULL,m_nDevID[CamID]);
	SetSoftPin(CamSensorTab[CamID].port,m_nDevID[CamID]);		 
	EnableSoftPin(TRUE,m_nDevID[CamID]);
	EnableGpio(TRUE,m_nDevID[CamID]);

	// 电源设置
	// 初始化SENOSR电源

	SENSOR_POWER Power[5];
	int Volt[5];
	int Rise[5];
	BOOL OnOff[5];
	int Current[5];
	CURRENT_RANGE Range[5];
	CURRENT_RANGE RangeUA[5];
	for (int i=0;i<5;i++)
	{
		RangeUA[i] = CURRENT_RANGE_UA;
	}


	Power[0] = POWER_AVDD;
	Volt[0] = (int)(CamSensorTab[CamID].avdd *100); // 2.8V
	Current[0] = 300; // 500mA
	Rise[0] = 100;
	OnOff[0] = TRUE;
	Range[0] = CURRENT_RANGE_MA;

	Power[1] = POWER_DOVDD;
	Volt[1] = (int)(CamSensorTab[CamID].dovdd *100); // 1.8V
	Current[1] = 300; // 500mA
	Rise[1] = 100;
	OnOff[1] = TRUE;
	Range[1] = CURRENT_RANGE_MA;	

	Power[2] = POWER_DVDD;
	Volt[2] = (int)(CamSensorTab[CamID].dvdd *100);// 1.8V
	Current[2] =  300;// 500mA
	Rise[2] = 100;
	OnOff[2] = TRUE;
	Range[2] = CURRENT_RANGE_MA;

	Power[3] = POWER_AFVCC;
	Volt[3] = (int)(CamSensorTab[CamID].af_vdd *100); // 2.8V
	Current[3] = 300; // 500mA
	Rise[3] = 100;
	OnOff[3] = TRUE;
	Range[3] = CURRENT_RANGE_MA;

	Power[4] = POWER_VPP;
	Volt[4] = (int)(CamSensorTab[CamID].vpp *100); // 2.8V
	Current[4] = 300; // 500mA
	Rise[4] = 100;
	OnOff[4] = TRUE;
	Range[4] = CURRENT_RANGE_MA;

// 	// 100mv/ms
// 	if (::PmuSetRise(Power, Rise, 5, m_nDevID[CamID]) != DT_ERROR_OK)
// 	{
// 		//msg("设置电压斜率失败!\r\n");
// 		AddLog(0,_T("设置电压斜率失败!"),COLOR_RED);
// 		//return DT_ERROR_FAILED;
// 	}

	// 设置电压值
	if (::PmuSetVoltage(Power,Volt,5,m_nDevID[CamID]) != DT_ERROR_OK)
	{
		//	msg("初始化电源失败!\r\n");
		AddLog(CamID,_T("No Demokit!"),COLOR_RED);
		return DT_ERROR_FAILED;
	}
	// 开启电压
	if (::PmuSetOnOff(Power,OnOff,5,m_nDevID[CamID]) != DT_ERROR_OK)
	{
		//	msg("开启电源失败!\r\n");
		AddLog(CamID,_T("开启电源失败!"),COLOR_RED);
		return DT_ERROR_FAILED;
	}
	// 设置量程
	if(CamSensorTab[CamID].bTestStandyCurrent)
	{
		ResetSensorI2cBus(m_nDevID[CamID]);
		::SensorEnable(CamSensorTab[CamID].pin^3,TRUE,m_nDevID[CamID]);
		SetSensorClock(FALSE,(USHORT)(CamSensorTab[CamID].mclk *10),m_nDevID[CamID]);
		if(PmuSetCurrentRange(Power,RangeUA,5,m_nDevID[CamID]) != DT_ERROR_OK)
		{
			AddLog(CamID,_T("设置电流uA量程失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}
		DriverICStandby(CamID);
		Sleep(1500);
		PmuGetCurrent(iStandyCurrent,5,0,CamID);//待机电流，uA
	}

	if(PmuSetCurrentRange(Power,Range,5,m_nDevID[CamID]) != DT_ERROR_OK)
	{
		AddLog(CamID,_T("设置电流mA量程失败!"),COLOR_RED);
		return DT_ERROR_FAILED;
	}
	
	
	// 设置每路限流值
// 	if (::PmuSetOcpCurrentLimit(Power, Current, 5,m_nDevID[CamID]) != DT_ERROR_OK)
// 	{
// 		//	msg("初始化电源限流失败!\r\n");
// 		AddLog(CamID,_T("初始化电源限流失败!"),COLOR_RED);
// 		//return DT_ERROR_FAILED;
// 	}

	if (SetSensorClock(TRUE,(USHORT)(CamSensorTab[CamID].mclk *10),m_nDevID[CamID]) != DT_ERROR_OK) // 20MHz
	{
		//msg("设置SENSOR时钟失败!\r\n");
		AddLog(CamID,_T("设置量设置SENSOR时钟失败 !"),COLOR_RED);
		//DtClosePower(Channel);
		return FALSE;
	}

	//开启IO上拉电阻
	SetSoftPinPullUp(IO_PULLUP,m_nDevID[CamID]); 
	
	if (InitDevice(GetCamDisplayHwnd(CamID),CamSensorTab[CamID].width,CamSensorTab[CamID].height,0,ImgFormat,CHANNEL_A,NULL,m_nDevID[CamID]) != DT_ERROR_OK)
	{

		return DT_ERROR_FAILED;
	}
	Sleep(10);
	SetSensorI2cRate(I2C_400K,m_nDevID[CamID]);
	ResetSensorI2cBus(m_nDevID[CamID]);
	/* 设置reset pin,pwdn pin*/
	::SensorEnable(CamSensorTab[CamID].pin^RESET_H,TRUE,m_nDevID[CamID]);
	Sleep(20);
	::SensorEnable(CamSensorTab[CamID].pin,TRUE,m_nDevID[CamID]);
	Sleep(150);
	if (InitSensor(CamSensorTab[CamID].SlaveID,CamSensorTab[CamID].ParaList,CamSensorTab[CamID].ParaListSize,CamSensorTab[CamID].mode,m_nDevID[CamID]) != DT_ERROR_OK)
 	{
 		AddLog(CamID,_T("sensor初始化失败"),COLOR_RED);
 		return DT_ERROR_FAILED;
 	}

	if (ImgFormat == D_YUV || ImgFormat == D_YUV_MTK_S || ImgFormat == D_YUV_SPI)
		SetYUV422Format(CamSensorTab[0].outformat,m_nDevID[CamID]);
	else
		SetRawFormat(CamSensorTab[CamID].outformat,m_nDevID[CamID]);

	/* 获得图像数据 */
	Size = GetByteOffset(CamSensorTab[CamID].width,ImgFormat,m_nDevID[CamID])*CamSensorTab[CamID].height;


	// A通道ROI
	SetRoiEx(0+CamSensorTab[CamID].iXRoi, 0+CamSensorTab[CamID].iYRoi, CamSensorTab[CamID].width, CamSensorTab[CamID].height, 0, 0 , 1, 1, 1,m_nDevID[CamID]);
	SetMipiClkPhase(0,m_nDevID[CamID]);	
	Sleep(10);
	// 计算A通道实际采集数据量
	CalculateGrabSize(&uGrabSize,m_nDevID[CamID]); 


	// 打开视频
	::OpenVideo(Size,m_nDevID[CamID]); 
	// 设置要抓取的通道

	//step 8补上gamma 对比度 饱和度测试
	SetGamma(100);  //Gamma  :default 100 is no gamma change....
	SetContrast(100); //Contrast :default 100 is no contrast change...
	SetSaturation(128);//Saturation: defualt 128 is no saturation change...
	SetDigitalGain(1.0, 1.0, 1.0); //AWB digital gian R G B

#endif
	 

	return 1;
}

int OFKit910::GetGrabSize()
{
	if(uGrabSize<Size)
	{
		return Size;
	}
	else
	{
		return uGrabSize;
	}
}
int  OFKit910::GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int CamID)
{
	if((m_nDevID[CamID]>=0)&&(m_nDevID[CamID]<500)) return ::GrabFrame(pInBuffer,uBufferSize,pGrabSize,pInfo, m_nDevID[CamID]);
	else 
	{
		AddLog(CamID,_T("DeviceID异常"),COLOR_RED,200); 
		return 0;
	}
}

int  OFKit910::OpenDevice(CString DeviceName) 
{
	return 1;
}

int OFKit910::WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID,int CamID)
{
	return ::WriteSensorReg(  uAddr,uReg,uValue,byMode,m_nDevID[CamID]);
}
int OFKit910::ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID,int CamID)
{
	return ::ReadSensorReg(  uAddr,uReg,pValue, byMode,m_nDevID[CamID]);
}
int  OFKit910::WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID,int CamID )
{ 

	return ::WriteSensorI2c( uDevAddr, uRegAddr, uRegAddrSize,pData, uSize, m_nDevID[CamID]);
}

int  OFKit910::ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop,int iDevID,int CamID )
{
	return ::ReadSensorI2c( uDevAddr, uRegAddr, uRegAddrSize,pData, uSize, bNoStop, m_nDevID[CamID]);
}
int OFKit910::PmuGetStandyCurrent(int Current[],int iCount,int iDevID,int CamID)
{
	for (int i=0;i<iCount;i++)
	{
		Current[i]=iStandyCurrent[i];
	}
	return 1;
}
int OFKit910::PmuGetCurrent(int Current[],int iCount,int iDevID,int CamID) 
{
	SENSOR_POWER Power[5];

	Power[0] = POWER_AVDD;
	Power[1] = POWER_DOVDD;
	Power[2] = POWER_DVDD;
	Power[3] = POWER_AFVCC;
	Power[4] = POWER_VPP;
	return ::PmuGetCurrent( Power, Current, iCount, m_nDevID[CamID]);
}
int OFKit910::PmuGetVoltage(int Voltage[],int iCount,int iDevID,int CamID) 
{
	SENSOR_POWER Power[5]; 
	Power[0] = POWER_AVDD;
	Power[1] = POWER_DOVDD;
	Power[2] = POWER_DVDD;
	Power[3] = POWER_AFVCC;
	Power[4] = POWER_VPP;



	return ::PmuGetVoltage(  Power, Voltage, iCount, m_nDevID[CamID]);
}

int OFKit910::SetSoftPin(int SensorPort ,int iDevID/* = 0 */)
{
	BYTE m_pinDef[26];
	memset(m_pinDef,0,26*sizeof(BYTE));
	if (SensorPort == 0 )
	{
		m_pinDef[0] = PIN_NC;
		m_pinDef[1] = PIN_D0;
		m_pinDef[2] = PIN_D2;
		m_pinDef[3] = PIN_D1;
		m_pinDef[4] = PIN_D3;
		m_pinDef[5] = PIN_D4;
		m_pinDef[6] = PIN_D5;
		m_pinDef[7] = PIN_D6;
		m_pinDef[8] = PIN_D7;
		m_pinDef[9] = PIN_D8;
		m_pinDef[10] = PIN_D9;
		m_pinDef[11] = PIN_NC;
		m_pinDef[12] = PIN_PCLK;
		m_pinDef[13] = PIN_HSYNC;
		m_pinDef[14] = PIN_VSYNC;
		m_pinDef[15] = PIN_NC;
		m_pinDef[16] = PIN_NC;
		m_pinDef[17] = PIN_MCLK;
		m_pinDef[18] = PIN_PWDN;
		m_pinDef[19] = PIN_RESET;
		m_pinDef[20] =  PIN_SCL;
		m_pinDef[21] = PIN_SDA; //PIN_SDA_18
		m_pinDef[22] = GPIO1;
		m_pinDef[23] = GPIO2;
		m_pinDef[24] = GPIO3;
		m_pinDef[25] = GPIO4;
	} 
	else if( SensorPort == 1)
	{
		m_pinDef[0]=PIN_PWDN2;
		m_pinDef[1]=PIN_D0;
		m_pinDef[2]=PIN_D2;
		m_pinDef[3]=PIN_D1;
		m_pinDef[4]=PIN_D3;
		m_pinDef[5]=PIN_D4;
		m_pinDef[6]=PIN_D5;
		m_pinDef[7]=PIN_D6;
		m_pinDef[8]=PIN_D7;
		m_pinDef[9]=PIN_D8;
		m_pinDef[10]=PIN_D9;
		m_pinDef[11]=PIN_NC;
		m_pinDef[12]=PIN_PCLK;
		m_pinDef[13]=PIN_HSYNC;
		m_pinDef[14]=PIN_VSYNC;
		m_pinDef[15]=PIN_NC;
		m_pinDef[16]=PIN_NC;
		m_pinDef[17]=PIN_NC;
		m_pinDef[18]=PIN_NC;
		m_pinDef[19]=PIN_NC;
		m_pinDef[20]=PIN_MCLK;
		m_pinDef[21]=PIN_NC;
		m_pinDef[22]=PIN_RESET;
		m_pinDef[23]=PIN_PWDN;
		m_pinDef[24]=PIN_SDA;
		m_pinDef[25]=PIN_SCL;

	}

	return ::SetSoftPin( m_pinDef, iDevID/* = 0 */);
}