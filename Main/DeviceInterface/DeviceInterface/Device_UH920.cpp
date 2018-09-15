#include "StdAfx.h"
#include "ccmBaseInterface.h"
#include "Device_UH920.h"
#pragma comment(lib,"dtccm2_mud952/dtccm2.lib")

CDevice_UH920::CDevice_UH920(void)
{

}


CDevice_UH920::~CDevice_UH920(void)
{

}


/* PIN的ID号和名称结构体 */
typedef struct DevPinInfo_s
{
	int PinID;
	char PinName[10];

}DevPinInfo_t;

/* UH920的PIN对应的名称和ID号 */
/* 按76个PIN来获取结果，选取需要的PIN结果值 */
DevPinInfo_t info[56]={
	{0,"A-AFVCC"},
	{1,"B-AFVCC"},
	{2,"A-DVDD"},
	{3,"B-DVDD"},
	{4,"A-DOVDD"},
	{5,"B-DOVDD"},
	{6,"A-AVDD"},
	{7,"B-AVDD"},
	{8,"A-VPP"},
	{9,"B-VPP"},
	{12,"A-GND4"},
	{13,"B-GND4"},
	{14,"A-GND3"},
	{15,"B-GND3"},
	{16,"A-GND2"},
	{17,"B-GND2"},
	{18,"A-GND1"},
	{19,"B-GND1"},
	{20,"A-SCKP"},
	{21,"A-SCKN"},
	{22,"A-D0P"},
	{23,"A-D0N"},
	{24,"A-D1P"},
	{25,"A-D1N"},
	{26,"A-D2P"},
	{27,"A-D2N"},
	{28,"A-D3P"},
	{29,"A-D3N"},
	{30,"B-SCKP"},
	{31,"B-SCKN"},
	{32,"B-D0P"},
	{33,"B-D0N"},
	{34,"B-D1P"},
	{35,"B-D1N"},
	{36,"B-D2P"},
	{37,"B-D2N"},
	{38,"B-D3P"},
	{39,"B-D3N"},
	{40,"B-MCLK"},
	{41,"B-PWDN"},
	{42,"B-RESET"},
	{43,"B-SDA"},
	{44,"B-SCL"},
	{45,"B-PO2"},
	{46,"B-PO1"},
	{47,"B-PO3"},
	{48,"B-PO4"},
	{67,"A-PO4"},
	{68,"A-PO3"},
	{69,"A-PO1"},
	{70,"A-PO2"},
	{71,"A-SCL"},
	{72,"A-SDA"},
	{73,"A-RESET"},
	{74,"A-PWDN"},
	{75,"A-MCLK"}
};
void CDevice_UH920::SetCamTab(int CamID)
{
	LoadSensorGenaralTab(CamID); 
	LoadSensorParalist(CamID); 
}

  void CDevice_UH920::SetGpioPinLevel(int gpioNum,BOOL bH,int CamID)
{
	::SetGpioPinLevel(gpioNum,bH,0);
}
  void CDevice_UH920::SensorEnable(int gpioNum,BOOL bH,int CamID)
  {
	  //::SetGpioPinLevel(gpioNum,bH,m_nDevID[CamID]);
	  //gpioNum=1:pwdn 2:reset 3:pwdn&reset
	  //bH=0:低平 1：高pin
	  if (bH==1)
	  {
		  ::SensorEnable(gpioNum,1,0);
	  }
	  else
	  {
		  ::SensorEnable(gpioNum^3,1,0);
	  }
  }
int  CDevice_UH920::GetKitTypeDeviceInfo(int CamID)
{
	CString m_DevName[8];
	char *DeviceName[8]={0};
	int DeviceNum;


	EnumerateDevice(DeviceName,4,&DeviceNum);

	if(DeviceNum==DevicesTol)//如果已经均有配置cam 则不执行打开动作
	{
		return DeviceNum;
	}
	m_wndDevList.clear();
	DevicesTol = DeviceNum;

	for (int i=0; i<DeviceNum; i++)
	{
		m_DevName[i].Empty();

		if (DeviceName[i] != NULL)
		{
			TRACE("Found device:%s\n",DeviceName[i]);

			CString st(DeviceName[i]); 
			m_wndDevList.push_back(st);			
			GlobalFree(DeviceName[i]);
		}
	}
	if(m_wndDevList.size()>0)
	{
		return 1;
	}
	return 0;
}
int CDevice_UH920::OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) 
{
	BOOL bFlag  = FALSE;
	CString str;
	str.Format(_T("Cam %d 工装启动中OS"),CamID);
	AddLog(CamID,str,COLOR_BLUE,200); 
	bFlag = OpenDevice(m_wndDevList[0]);
	if(!bFlag)
	{
		AddLog(CamID,_T("工装未连接"),COLOR_RED,220);
		return -1;
	}
	USHORT m_OpenStd = 0x1ff-50;  //0x400 equal to 3V...
	USHORT m_ShortStd = 50;

	int HighLimit[45] = {0};
	int LowLimit[45] = {0};

	for(int i = 0; i < 45; i++)
	{
		HighLimit[i] = (int)(m_OpenStd * 1.5f * 1000000/512) ; //转变为uV信号	
		LowLimit[i] =  (int)(m_ShortStd *1.5f * 1000000/512) ;
	}
	OS_Config((int)(1.5f*1000*1000), HighLimit, LowLimit, 45, 20*1000, 1*1000,0);

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

	if(CamID==0)
	{
		m_bAPort = 1;
		m_bBPort = 0;
	}
	else
	{
		m_bAPort = 0;
		m_bBPort = 1;
	}

	m_bLCHigh = 1;
	m_bLCLow = 0;
	m_bOSCommand = 1;
	m_bLCCommand = 0;
	m_bOSHigh = 1;
	m_bOSLow = 0;
	UCHAR Command =  (m_bOSCommand << 7) + (m_bLCCommand << 6) + (m_bAPort << 5) + (m_bBPort << 4) + (m_bOSHigh << 3) + (m_bOSLow << 2) + (m_bLCHigh << 1) + m_bLCLow;


	//LC_OS_CommandConfig(0x80 | 0x04, IoMask, 10,0);
	LC_OS_CommandConfig(Command, IoMask, 10,0);

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
		str.Format(_T("开短路错误%d，请重试"), nRet);
		AfxMessageBox(str);*/
		return -3;
	}
	return 1;
}
int  CDevice_UH920::OpenDevice(CString DeviceName)
{    
	if (DeviceName != _T(""))
	{
		DWORD Ver[4];

		char buff[1024]={0};
		int bufflen=1024; 
		int len = DeviceName.GetLength();
		WideCharToMultiByte(CP_ACP,0,DeviceName,len,buff,len,NULL,NULL);//T2A,W2A

		if (m_nDevID >= 0)
			::CloseDevice(m_nDevID);

		if (::OpenDevice(buff,&m_nDevID,0) == DT_ERROR_OK)
		{
			TRACE("Open device successful\n");
			if (GetLibVersion(Ver,m_nDevID) != DT_ERROR_OK)
				return -1;
			TRACE("LIB Version:%d.%d.%d.%d\n",Ver[0],Ver[1],Ver[2],Ver[3]);

			if (GetFwVersion(0,Ver,m_nDevID) != DT_ERROR_OK)
				return -1;
			TRACE("FW1 Version:%d.%d.%d.%d\n",Ver[0],Ver[1],Ver[2],Ver[3]);

			if (GetFwVersion(1,Ver,m_nDevID) != DT_ERROR_OK)
				return -1;
			TRACE("FW2 Version:%d.%d.%d.%d\n",Ver[0],Ver[1],Ver[2],Ver[3]);

			if (GetFwVersion(2,Ver,m_nDevID) != DT_ERROR_OK)
				return -1;
			TRACE("FW3 Version:%d.%d.%d.%d\n",Ver[0],Ver[1],Ver[2],Ver[3]);
			return -1;
		}
	}

	TRACE("Open device failed!\n");
	return 1;
}

int  CDevice_UH920::CloseDevice(int CamID)
{
	Channel = 3;
	if (Channel & CHANNEL_A)
	{
		//设置reset pin,pwdn pin
		//SensorEnable(CamSensorTab[0].pin,FALSE,m_nDevID);
		SetChannelSel(CHANNEL_B,m_nDevID);
		{
			ResetSensorI2cBus(m_nDevID);
			CloseVideo(m_nDevID);
			SensorEnable(3 ^ 2, 1, m_nDevID);       //reset sensor....
			Sleep(20);
			SensorEnable(3 ^ 1, 1, m_nDevID);       //power down sensor....
			Sleep(2);
			SetSensorClock(0, 24*10, m_nDevID);
			//close the IO power...
			SetSoftPinPullUp(IO_NOPULL, m_nDevID); 
			//close reset and power down pin...
			SensorEnable(0, 1, m_nDevID);

			//关闭电压
			SENSOR_POWER Power[10];
			BOOL OnOff[10] = {0};	
			int Volt[10] = {0};

			Power[0] = POWER_AVDD;
			Power[1] = POWER_DOVDD;
			Power[2] = POWER_DVDD;
			Power[3] = POWER_AFVCC;
			Power[4] = POWER_VPP;
			Volt[0] = 0;
			Volt[1] = 0;
			Volt[2] = 0;
			Volt[3] = 0;
			Volt[4] = 0;
			OnOff[0] = 0;
			OnOff[1] = 0;
			OnOff[2] = 0;
			OnOff[3] = 0;
			OnOff[4] = 0;
			//close the power switch...
			//if necessory, you can set the power voltage to 0
			PmuSetVoltage(Power, Volt, 5, m_nDevID);
			Sleep(50);
			PmuSetOnOff(Power,OnOff,5,m_nDevID);
		}
	}

	if (Channel & CHANNEL_B)
	{
		// 设置reset pin,pwdn pi
		//SensorEnable(CamSensorTab[1].pin,FALSE,m_nDevID);
		SetChannelSel(CHANNEL_B,m_nDevID);
		CloseVideo(m_nDevID);
		ResetSensorI2cBus(m_nDevID);
		SensorEnable(3 ^ 2, 1, m_nDevID);       //reset sensor....
		Sleep(20);
		SensorEnable(3 ^ 1, 1, m_nDevID);       //power down sensor....
		Sleep(2);
		SetSensorClock(0, 24*10, m_nDevID);
		//close the IO power...
		SetSoftPinPullUp(IO_NOPULL, m_nDevID); 
		//close reset and power down pin...
		SensorEnable(0, 1, m_nDevID);

		//关闭电压
		SENSOR_POWER Power[10];
		BOOL OnOff[10] = {0};	
		int Volt[10] = {0};

		Power[0] = POWER_AVDD_B;
		Power[1] = POWER_DOVDD_B;
		Power[2] = POWER_DVDD_B;
		Power[3] = POWER_AFVCC_B;
		Power[4] = POWER_VPP_B;
		Volt[0] = 0;
		Volt[1] = 0;
		Volt[2] = 0;
		Volt[3] = 0;
		Volt[4] = 0;
		OnOff[0] = 0;
		OnOff[1] = 0;
		OnOff[2] = 0;
		OnOff[3] = 0;
		OnOff[4] = 0;
		//close the power switch...
		//if necessory, you can set the power voltage to 0
		PmuSetVoltage(Power, Volt, 5, m_nDevID);
		Sleep(50);
		PmuSetOnOff(Power,OnOff,5,m_nDevID);
	}
	return 0;
}

int  CDevice_UH920::OpenVideo(int CamID)
{ 

	if(m_wndDevList.size()<1)
	{
		if(GetKitTypeDeviceInfo(CamID)<1)
			return -1;
	}
	OpenDevice(m_wndDevList[0]);
	//return DtSensorTest0(CamID); 
	  return  DtSensorTest1(CamID);
}

int  CDevice_UH920::GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int iDevID)
{
	return ::GrabFrame(pInBuffer,uBufferSize,pGrabSize,pInfo, m_nDevID);

}

int  CDevice_UH920::WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID,int CamID )
{
	if(CamID==0||CamID==2)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);
	}
	else
	{
		SetChannelSel(CHANNEL_B,m_nDevID);
	}

	return WriteSensorI2c( uDevAddr, uRegAddr, uRegAddrSize,pData, uSize, m_nDevID);

}

int  CDevice_UH920::ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop,int iDevID,int CamID )
{
	if(CamID==0||CamID==2)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);
	}
	else
	{
		SetChannelSel(CHANNEL_B,m_nDevID);
	}

	return ReadSensorI2c( uDevAddr, uRegAddr, uRegAddrSize,pData, uSize, bNoStop, m_nDevID);

}

int CDevice_UH920::DtSensorTest()
{ 
	OpenDevice(m_wndDevList[0]);
	m_iChanSel = 2;
	int nFlag = 0;
	TRACE("SensorTest Start\r\n");

	m_uSensorPort = SENSOR_PORT_MIPI; 

	if (m_iChanSel == 1)
	{
		Channel = CHANNEL_B;
	}
	else if (m_iChanSel == 2)
	{
		Channel = CHANNEL_AB;
	}

	// 设置IO上拉
	SetSoftPinPullUp(TRUE,m_nDevID);

	// 电源设置
	// 初始化SENOSR电源
	nFlag = DtPowerOn( Channel);
	if(nFlag!=1) return -1; 
	// 400Kbps I2C
	nFlag = SetSensorI2cRate(TRUE,m_nDevID);
	if(nFlag!=1) return -1;

	nFlag = SetMclk(Channel);
	if(nFlag!=1) return -1;

	nFlag = SetSensorPwdnReset(Channel);
	if(nFlag!=1) return -1;
	nFlag = SensorFind(Channel); 
	if(nFlag!=1) return -1;
	nFlag = SensorInit( Channel);
	if(nFlag!=1) return -1;
	nFlag = DeviceInit(Channel); 
	if(nFlag!=1) return -1;
	nFlag =OpenVideo(  Channel);
	if(nFlag!=1) return -1;
	return 1;

}

int CDevice_UH920::DtPowerOn(BYTE byChannel)
{
	SENSOR_POWER Power[5];
	int Volt[5];
	int Rise[5];
	BOOL OnOff[5];
	int Current[5];
	CURRENT_RANGE Range[5];

	if (byChannel & CHANNEL_A)
	{
		Power[0] = POWER_AVDD;
		Volt[0] = (int)(CamSensorTab[0].avdd  *100); // 2.8V
		Current[0] = 500; // 500mA
		Rise[0] = 100;
		OnOff[0] = TRUE;
		Range[0] = CURRENT_RANGE_MA;

		Power[1] = POWER_DOVDD;
		Volt[1] = (int)(CamSensorTab[0].dovdd *100); // 1.8V
		Current[1] = 500; // 500mA
		Rise[1] = 100;
		OnOff[1] = TRUE;
		Range[1] = CURRENT_RANGE_MA;	

		Power[2] = POWER_DVDD;
		Volt[2] = (int)(CamSensorTab[0].dvdd *100);// 1.8V
		Current[2] =  500;// 500mA
		Rise[2] = 100;
		OnOff[2] = TRUE;
		Range[2] = CURRENT_RANGE_MA;

		Power[3] = POWER_AFVCC;
		Volt[3] = (int)(CamSensorTab[0].af_vdd *100); // 2.8V
		Current[3] = 500; // 500mA
		Rise[3] = 100;
		OnOff[3] = TRUE;
		Range[3] = CURRENT_RANGE_MA;

		Power[4] = POWER_VPP;
		Volt[4] = (int)(CamSensorTab[0].vpp *100); // 2.8V
		Current[4] = 500; // 500mA
		Rise[4] = 100;
		OnOff[4] = FALSE;
		Range[4] = CURRENT_RANGE_MA;

		// 100mv/ms
		if (::PmuSetRise(Power, Rise, 5, m_nDevID) != DT_ERROR_OK)
		{
			TRACE("设置电压斜率失败!\r\n");
			return DT_ERROR_FAILED;
		}

		// 设置电压值
		if (::PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 开启电压
		if (::PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 设置每路限流值
		if (::PmuSetOcpCurrentLimit(Power, Current, 5,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("初始化电源限流失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 设置量程
		if (::PmuSetCurrentRange(Power,Range,5,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("设置量程失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
	}

	if (byChannel & CHANNEL_B)
	{
		Power[0] = POWER_AVDD_B;
		Volt[0] = (int)(CamSensorTab[1].avdd *100); // 2.8V
		Current[0] = 500; // 500mA
		Rise[0] = 100;
		OnOff[0] = TRUE;
		Range[0] = CURRENT_RANGE_MA;


		Power[1] = POWER_DOVDD_B;
		Volt[1] = (int)(CamSensorTab[1].dovdd *100); // 1.8V
		Current[1] = 500; // 500mA
		Rise[1] = 100;
		OnOff[1] = TRUE;
		Range[1] = CURRENT_RANGE_MA;

		Power[2] = POWER_DVDD_B;
		Volt[2] = (int)(CamSensorTab[1].dvdd *100);// 1.8V
		Current[2] =  500;// 500mA
		Rise[2] = 100;
		OnOff[2] = TRUE;
		Range[2] = CURRENT_RANGE_MA;

		Power[3] = POWER_AFVCC_B;
		Volt[3] = (int)(CamSensorTab[1].af_vdd *100); // 2.8V
		Current[3] = 500; // 500mA
		Rise[3] = 100;
		OnOff[3] = TRUE;
		Range[3] = CURRENT_RANGE_MA;

		Power[4] = POWER_VPP_B;
		Volt[4] = (int)(CamSensorTab[1].vpp *100); // 2.8V
		Current[4] = 500; // 500mA
		Rise[4] = 100;
		OnOff[4] = FALSE;
		Range[4] = CURRENT_RANGE_MA;

		// 100mv/ms
		if (PmuSetRise(Power, Rise, 5, m_nDevID) != DT_ERROR_OK)
		{
			TRACE("设置电压斜率失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 设置电压值
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 开启电压
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 设置每路限流值
		if (PmuSetOcpCurrentLimit(Power, Current, 5,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("初始化电源限流失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 设置量程
		if (PmuSetCurrentRange(Power,Range,5,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("设置量程失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
	}
	return 1;
}

int CDevice_UH920::DtPowerClose(BYTE byChannel)
{
	SENSOR_POWER Power[5];
	int Volt[5];
	BOOL OnOff[5];

	if (byChannel & CHANNEL_A)
	{
		Power[0] = POWER_AVDD;
		Volt[0] = 0; 
		OnOff[0] = FALSE;

		Power[1] = POWER_DOVDD;
		Volt[1] = 0; 
		OnOff[1] = FALSE;

		Power[2] = POWER_DVDD;
		Volt[2] = 0;
		OnOff[2] = FALSE;

		Power[3] = POWER_AFVCC;
		Volt[3] = 0; 
		OnOff[3] = FALSE;

		Power[4] = POWER_VPP;
		Volt[4] = 0;
		OnOff[4] = FALSE;

		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			return DT_ERROR_FAILED;
		}

		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//return DT_ERROR_FAILED;
		}
	}

	if (byChannel & CHANNEL_B)
	{
		Power[0] = POWER_AVDD_B;
		Volt[0] = 0;
		OnOff[0] = FALSE;

		Power[1] = POWER_DOVDD_B;
		Volt[1] = 0;
		OnOff[1] = FALSE;

		Power[2] = POWER_DVDD_B;
		Volt[2] = 0;
		OnOff[2] = FALSE;

		Power[3] = POWER_AFVCC_B;
		Volt[3] = 0;
		OnOff[3] = FALSE;

		Power[4] = POWER_VPP_B;
		Volt[4] = 0;
		OnOff[4] = FALSE;

		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			return DT_ERROR_FAILED;
		}

		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//return DT_ERROR_FAILED;
		}
	}

	return DT_ERROR_OK;
}

int CDevice_UH920::SetMclk(BYTE byChannel)
{
	if (byChannel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);

		if (SetSensorClock(TRUE,(USHORT)(CamSensorTab[0].mclk *10),m_nDevID) != DT_ERROR_OK) // 20MHz
		{
			TRACE("设置SENSOR时钟失败!\r\n");
			DtPowerClose(byChannel);
			return -1;
		}
	}

	if (byChannel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		if (SetSensorClock(TRUE,(USHORT)(CamSensorTab[1].mclk *10),m_nDevID) != DT_ERROR_OK) // 20MHz
		{
			TRACE("设置SENSOR时钟失败!\r\n");
			DtPowerClose(byChannel);
			return -1;
		}
	}
	return 1;

}
int CDevice_UH920::SetSensorPwdnReset(BYTE byChannel)
{
	BYTE Pwdn2=0;
	BYTE Pwdn1=0;
	BYTE Reset=0;
	Sleep(50);
	// A通道sensor的reset，pwdn设置
	if(byChannel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);	// 设置当前为A通道
		/* 设置reset pin,pwdn pin*/
		SensorEnable(CamSensorTab[0].pin^RESET_H,TRUE,m_nDevID);
		Sleep(50);
		SensorEnable(CamSensorTab[0].pin,TRUE,m_nDevID);
		Sleep(50);

		Pwdn2 = CamSensorTab[0].pin & PWDN_H ?  PWDN2_L : PWDN2_H;   //pwdn2 neg to pwdn1
		Pwdn1 = CamSensorTab[0].pin & PWDN_H ?  PWDN_H : PWDN_L;     //pwdn1
		Reset = CamSensorTab[0].pin & RESET_H ?  RESET_H : RESET_L;  //reset

		CamSensorTab[0].pin = Pwdn2 | Pwdn1 | Reset;
		//as the State said.
		SensorEnable(CamSensorTab[0].pin,1,m_nDevID); //reset
	}

	// B通道sensor的reset，pwdn设置
	if(byChannel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);	// 设置当前为B通道
		Pwdn2=0;
		Pwdn1=0;
		Reset=0;
		/* 设置reset pin,pwdn pin*/
		SensorEnable(CamSensorTab[1].pin^RESET_H,TRUE,m_nDevID);
		Sleep(500);
		SensorEnable(CamSensorTab[1].pin,TRUE,m_nDevID);
		Sleep(500);

		Pwdn2 = CamSensorTab[1].pin & PWDN_H ?  PWDN2_L : PWDN2_H;   //pwdn2 neg to pwdn1
		Pwdn1 = CamSensorTab[1].pin & PWDN_H ?  PWDN_H : PWDN_L;     //pwdn1
		Reset = CamSensorTab[1].pin & RESET_H ?  RESET_H : RESET_L;  //reset

		CamSensorTab[1].pin = Pwdn2 | Pwdn1 | Reset;
		//as the State said.
		SensorEnable(CamSensorTab[1].pin,1,m_nDevID); //reset
	}

	return 1;
}

int CDevice_UH920::SensorFind(BYTE byChannel)
{
	int nFlag = 0;
	if (byChannel & CHANNEL_A)
	{
		// 检测A通道SESNOR是否存在
		USHORT Flag = 0;
		nFlag= ReadSensorReg(CamSensorTab[0].SlaveID,CamSensorTab[0].FlagReg,&Flag,CamSensorTab[0].mode,m_nDevID,0); 

		if (nFlag!= 1 )
		{
			CString str;
			str.Format(_T("IIC nFlag = %d"),nFlag);
			AddLog(CamSensorTab[0].CamID,str,COLOR_RED,200);

			return -2;//i2c 通信失败
		}	 
		else if ((Flag&CamSensorTab[0].FlagMask) != (CamSensorTab[0].FlagData & CamSensorTab[0].FlagMask))
		{
			CString str;
			str.Format(_T("IIC nFlag = %d"),nFlag);
			AddLog(CamSensorTab[0].CamID,str,COLOR_RED,200);

			return -3;//没有找到指定sensor；
		}
	}

	if (byChannel & CHANNEL_B)
	{
		// 检测A通道SESNOR是否存在
		USHORT Flag = 0;
		nFlag= ReadSensorReg(CamSensorTab[1].SlaveID,CamSensorTab[1].FlagReg,&Flag,CamSensorTab[1].mode,m_nDevID,1); 

		if (nFlag!= 1)
		{
			CString str;
			str.Format(_T("i2c 通信失败 IIC nFlag = %d"),nFlag);
			AddLog(CamSensorTab[1].CamID,str,COLOR_RED,200);

			return -2;//i2c 通信失败
		}	 
		else if ((Flag&CamSensorTab[1].FlagMask) != (CamSensorTab[1].FlagData & CamSensorTab[1].FlagMask))
		{
			CString str;
			str.Format(_T("没有找到指定sensor IIC nFlag = %d"),nFlag);
			AddLog(CamSensorTab[1].CamID,str,COLOR_RED,200);

			return -3;//没有找到指定sensor；
		}

	}
	Sleep(50);
	return 1;
} 

int CDevice_UH920::SensorInit(BYTE byChannel)
{
	// 初始化A通道sensor
	if (byChannel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);	

		if (InitSensor(CamSensorTab[0].SlaveID,CamSensorTab[0].ParaList,CamSensorTab[0].ParaListSize,CamSensorTab[0].mode,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("初始化A通道SENSOR失败!\r\n");
			DtPowerClose(Channel);
			return DT_ERROR_FAILED;
		}
	}

	Sleep(50);

	// 初始化B通道sensor
	if (byChannel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		if (InitSensor(CamSensorTab[1].SlaveID,CamSensorTab[1].ParaList,CamSensorTab[1].ParaListSize,CamSensorTab[1].mode,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("初始化B通道SENSOR失败!\r\n");
			DtPowerClose(Channel);
			return DT_ERROR_FAILED;
		}
	}
	return 1;
}

int CDevice_UH920::DeviceInit(BYTE byChannel)
{
	if (byChannel & CHANNEL_A)
	{
		if (InitDevice(NULL,CamSensorTab[0].width,CamSensorTab[0].height,m_uSensorPort,CamSensorTab[0].outformat,CHANNEL_A,NULL,m_nDevID) != DT_ERROR_OK)
		{
			TRACE("初始化设备失败!\r\n");
			DtPowerClose(byChannel);
			return DT_ERROR_FAILED;
		}

		if (CamSensorTab[0].outformat == D_YUV || CamSensorTab[0].outformat == D_YUV_MTK_S || CamSensorTab[0].outformat == D_YUV_SPI)
			SetYUV422Format(CamSensorTab[0].outformat,m_nDevID);
		else
			SetRawFormat(CamSensorTab[0].outformat,m_nDevID);

		/* 获得图像数据 */
		SizeA = GetByteOffset(CamSensorTab[0].width,CamSensorTab[0].outformat,m_nDevID)*CamSensorTab[0].height;
		TRACE("Image SizeA=%d(%08X) bytes\r\n",SizeA,SizeA);
	}

	if (byChannel & CHANNEL_B)
	{
		if (InitDevice(NULL,CamSensorTab[1].width,CamSensorTab[1].height,m_uSensorPort,CamSensorTab[1].outformat,CHANNEL_B,NULL,m_nDevID) != DT_ERROR_OK)
		{  
			DtPowerClose(byChannel);
			return DT_ERROR_FAILED;
		}
		if (CamSensorTab[1].outformat == D_YUV || CamSensorTab[1].outformat == D_YUV_MTK_S || CamSensorTab[1].outformat == D_YUV_SPI)
			SetYUV422Format(CamSensorTab[1].outformat,m_nDevID);
		else
			SetRawFormat(CamSensorTab[1].outformat,m_nDevID);

		/* 获得图像数据 */
		SizeB = GetByteOffset(CamSensorTab[1].width,CamSensorTab[1].outformat,m_nDevID)*CamSensorTab[1].height;

	}
	return 1;
}

int CDevice_UH920::GetGrabSize()
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
int CDevice_UH920::OpenVideo(BYTE byChannel)
{ 
	if (byChannel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);

		// A通道ROI
		SetRoiEx(0, 0, CamSensorTab[0].width, CamSensorTab[0].height, 3, 3 , 1, 1, 1,m_nDevID);
		// 计算A通道实际采集数据量
		CalculateGrabSize(&uGrabSize,m_nDevID);
		TRACE("CalculateGrabSizeA:%u\r\n",uGrabSize);

	}

	if (byChannel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		// B通道ROI
		SetRoiEx(0, 0, CamSensorTab[1].width, CamSensorTab[1].height, 3, 3 , 1, 1, 1,m_nDevID);
		// 计算B通道实际采集数据量
		CalculateGrabSize(&uGrabSize,m_nDevID);
		TRACE("CalculateGrabSizeB:%u\r\n",uGrabSize);

	}
	int Size = SizeA>SizeB?SizeA:SizeB;
	// 打开视频
	::OpenVideo(Size,m_nDevID);

	// 设置要抓取的通道
	SetGrabChannelEnable(Channel);

	return 1;

}
int CDevice_UH920::WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID,int CamID)
{
	if(CamID==0)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);
	}
	else
	{
		SetChannelSel(CHANNEL_B,m_nDevID);
	}
	return ::WriteSensorReg(  uAddr,uReg,uValue,byMode,m_nDevID);
}

int CDevice_UH920::ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID,int CamID)
{
	if(CamID==0)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);
	}
	else
	{
		SetChannelSel(CHANNEL_B,m_nDevID);
	}
	return ::ReadSensorReg(  uAddr,uReg,pValue, byMode,m_nDevID);
}
int CDevice_UH920::PmuGetStandyCurrent(int Current[],int iCount,int iDevID,int CamID)
{
	return 1;
}
int CDevice_UH920::PmuGetCurrent(int Current[],int iCount,int iDevID,int CamID)
{
	SENSOR_POWER Power[5];
	if(CamID==0)
	{
		Power[0] = POWER_AVDD;
		Power[1] = POWER_DOVDD;
		Power[2] = POWER_DVDD;
		Power[3] = POWER_AFVCC;
		Power[4] = POWER_VPP;
		SetChannelSel(CHANNEL_A,m_nDevID);
	}
	else
	{
		Power[0] = POWER_AVDD_B;
		Power[1] = POWER_DOVDD_B;
		Power[2] = POWER_DVDD_B;
		Power[3] = POWER_AFVCC_B;
		Power[4] = POWER_VPP_B;
		SetChannelSel(CHANNEL_B,m_nDevID);
	}
	return ::PmuGetCurrent( Power, Current, iCount, m_nDevID);
}
int CDevice_UH920::PmuGetVoltage(int Voltage[],int iCount,int iDevID,int CamID)
{
	SENSOR_POWER Power[5];
	if(CamID==0)
	{
		Power[0] = POWER_AVDD;
		Power[1] = POWER_DOVDD;
		Power[2] = POWER_DVDD;
		Power[3] = POWER_AFVCC;
		Power[4] = POWER_VPP;
		SetChannelSel(CHANNEL_A,m_nDevID);
	}
	else
	{
		Power[0] = POWER_AVDD_B;
		Power[1] = POWER_DOVDD_B;
		Power[2] = POWER_DVDD_B;
		Power[3] = POWER_AFVCC_B;
		Power[4] = POWER_VPP_B;
		SetChannelSel(CHANNEL_B,m_nDevID);
	}

	return ::PmuGetVoltage(  Power, Voltage, iCount, m_nDevID);
}
int CDevice_UH920::DtSensorTest0(int Camera)
{  

	UCHAR PinDef[26] = {PIN_NC};

	CString str;

	BYTE Channel = CHANNEL_A;
	int iRet = TRUE;
	//CString strDevice="";

	TRACE("SensorTest Start\r\n");

	m_uSensorPort = SENSOR_PORT_MIPI;


	CString sTemp ;
	m_iChanSel  =Camera;
	if (m_iChanSel == 1)
	{
		Channel = CHANNEL_B;
	}
	else if (m_iChanSel == 2)
	{
		Channel = CHANNEL_AB;
	}

	// 设置通道

	UCHAR ImgFormatA = CamSensorTab[0].type;
	UCHAR  ImgFormatB = CamSensorTab[1].type;


	// 设置IO上拉
	//SetSoftPinPullUp(TRUE,m_nDevID);

	// 电源设置
	// 初始化SENOSR电源

	// 电源设置
	// 初始化SENOSR电源

	SENSOR_POWER Power[5];
	int Volt[5] = {0};
	int Rise[5] = {0};
	BOOL OnOff[5] = {0};
	int Current[5] = {0};
	CURRENT_RANGE Range[5] ;


	if (Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);	
		SensorEnable(0, 1, m_nDevID);
		SetSoftPinPullUp(IO_NOPULL, m_nDevID);
		SetSensorClock(FALSE,(USHORT)(12*10), m_nDevID); 

		Power[0] = POWER_AVDD;

		Current[0] = 500; // 500mA
		Rise[0] = 100;
		OnOff[0] = TRUE;
		Range[0] = CURRENT_RANGE_MA;

		Power[1] = POWER_DOVDD;

		Current[1] = 500; // 500mA
		Rise[1] = 100;
		OnOff[1] = TRUE;
		Range[1] = CURRENT_RANGE_MA;	

		Power[2] = POWER_DVDD;

		Current[2] =  500;// 500mA
		Rise[2] = 100;
		OnOff[2] = TRUE;
		Range[2] = CURRENT_RANGE_MA;

		Power[3] = POWER_AFVCC;

		Current[3] = 500; // 500mA
		Rise[3] = 100;
		OnOff[3] = TRUE;
		Range[3] = CURRENT_RANGE_MA;

		Power[4] = POWER_VPP;

		Current[4] = 500; // 500mA
		Rise[4] = 100;
		OnOff[4] = FALSE;
		Range[4] = CURRENT_RANGE_MA;


		//先要对电压设定成0V，再打开电源开关，再把电压设定到预设值　2015.12.24


		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}

		Sleep(10);
		// 100mv/ms
		OnOff[0]=1;
		OnOff[1]=1;
		OnOff[2]=1;
		OnOff[3]=1;
		OnOff[4]=1;
		// 开启电压 
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		} 
		Sleep(2);
		// 100mv/ms
		if (PmuSetRise(Power, Rise, 5, m_nDevID) != DT_ERROR_OK)
		{
			//msg("设置电压斜率失败!\r\n");
			AddLog(0,_T("设置电压斜率失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}

		Sleep(2);

		// 开启电压
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//	msg("开启电源失败!\r\n");
			AddLog(0,_T("开启电源失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}

		Volt[0] = (int)(CamSensorTab[0].avdd *100); // 2.8V
		Sleep(2);
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2); 
		Volt[2] = (int)(CamSensorTab[0].dvdd *100);// 1.8V
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);




		Volt[1] = (int)(CamSensorTab[0].dovdd *100); // 1.8V
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}

		Volt[3] = (int)(CamSensorTab[0].af_vdd *100); // 2.8V
		Volt[4] = (int)(CamSensorTab[0].vpp *100); // 2.8V

		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}


		Sleep(2);
		OnOff[0]=TRUE;//AVDD_ON
		//////////////////////////////////////////////////////////////////     

		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		OnOff[2]=TRUE;//AVDD_ON 
		//OnOff={1,1,1,1,1};
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		OnOff[3]=TRUE;

		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}


		// 设置每路限流值
		if (PmuSetOcpCurrentLimit(Power, Current, 5,m_nDevID) != DT_ERROR_OK)
		{
			//	msg("初始化电源限流失败!\r\n");
			AddLog(0,_T("初始化电源限流失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}

		// 设置量程
		if (PmuSetCurrentRange(Power,Range,5,m_nDevID) != DT_ERROR_OK)
		{
			//	msg("设置量程失败!\r\n");
			AddLog(0,_T("设置量程失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		SetSoftPinPullUp(IO_PULLUP, m_nDevID);
	}

	if (Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);	
		SensorEnable(0, 1, m_nDevID);
		SetSoftPinPullUp(IO_NOPULL, m_nDevID);
		SetSensorClock(FALSE,(USHORT)(12*10), m_nDevID); //add 20160607

		Power[0] = POWER_AVDD_B;

		Current[0] = 500; // 500mA
		Rise[0] = 100;
		OnOff[0] = TRUE;
		Range[0] = CURRENT_RANGE_MA;


		Power[1] = POWER_DOVDD_B;

		Current[1] = 500; // 500mA
		Rise[1] = 100;
		OnOff[1] = TRUE;
		Range[1] = CURRENT_RANGE_MA;

		Power[2] = POWER_DVDD_B;

		Current[2] =  500;// 500mA
		Rise[2] = 100;
		OnOff[2] = TRUE;
		Range[2] = CURRENT_RANGE_MA;

		Power[3] = POWER_AFVCC_B;

		Current[3] = 500; // 500mA
		Rise[3] = 100;
		OnOff[3] = TRUE;
		Range[3] = CURRENT_RANGE_MA;

		Power[4] = POWER_VPP_B;

		Current[4] = 500; // 500mA
		Rise[4] = 100;
		OnOff[4] = FALSE;
		Range[4] = CURRENT_RANGE_MA;


		//先要对电压设定成0V，再打开电源开关，再把电压设定到预设值　2015.12.30 B通道


		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}

		Sleep(10);
		// 100mv/ms
		OnOff[0]=1;
		OnOff[1]=1;
		OnOff[2]=1;
		OnOff[3]=1;
		OnOff[4]=1;
		// 开启电压 
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		} 
		Sleep(2);
		// 100mv/ms
		if (PmuSetRise(Power, Rise, 5, m_nDevID) != DT_ERROR_OK)
		{
			//msg("设置电压斜率失败!\r\n");
			AddLog(0,_T("设置电压斜率失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}

		Sleep(2);

		// 开启电压
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//	msg("开启电源失败!\r\n");
			AddLog(0,_T("开启电源失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}

		Volt[0] = (int)(CamSensorTab[1].avdd *100); // 2.8V
		Sleep(2);
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2); 
		Volt[2] = (int)(CamSensorTab[1].dvdd *100);// 1.8V
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2); 

		Volt[1] = (int)(CamSensorTab[1].dovdd *100); // 1.8V
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}

		Volt[3] = (int)(CamSensorTab[1].af_vdd *100); // 2.8V
		Volt[4] = (int)(CamSensorTab[1].vpp *100); // 2.8V

		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}


		Sleep(2);
		OnOff[0]=TRUE;//AVDD_ON
		//////////////////////////////////////////////////////////////////     

		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		OnOff[2]=TRUE;//AVDD_ON 
		//OnOff={1,1,1,1,1};
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		OnOff[3]=TRUE;

		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		} 


		// 设置每路限流值
		if (PmuSetOcpCurrentLimit(Power, Current, 5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源限流失败!\r\n");
			AddLog(1,_T("初始化电源限流失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}

		// 设置量程
		if (PmuSetCurrentRange(Power,Range,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("设置量程失败!\r\n");
			AddLog(1,_T("设置量程失败!"),COLOR_RED);
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		SetSoftPinPullUp(IO_PULLUP, m_nDevID);
	}

	// 400Kbps I2C true  100k false;
	SetSensorI2cRate(TRUE,m_nDevID);
	SetSensorI2cRapid(1, m_nDevID);

	if (Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);

		if (SetSensorClock(TRUE,(USHORT)(CamSensorTab[0].mclk *10),m_nDevID) != DT_ERROR_OK) // 20MHz
		{
			//msg("设置SENSOR时钟失败!\r\n");
			AddLog(0,_T("设置量设置SENSOR时钟失败程失败!"),COLOR_RED);
			//DtClosePower(Channel);
			return FALSE;
		}
	}

	if (Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		if (SetSensorClock(TRUE,(USHORT)(CamSensorTab[1].mclk *10),m_nDevID) != DT_ERROR_OK) // 20MHz
		{
			//msg("设置SENSOR时钟失败!\r\n");
			AddLog(1,_T("设置量设置SENSOR时钟失败程失败!"),COLOR_RED);
			//DtClosePower(Channel);
			return FALSE;
		}
	}


	BYTE Pwdn2=0;
	BYTE Pwdn1=0;
	BYTE Reset=0;
	Sleep(50);
	// A通道sensor的reset，pwdn设置
	if(Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);	// 设置当前为A通道
		/* 设置reset pin,pwdn pin*/
		SensorEnable(CamSensorTab[0].pin^RESET_H,TRUE,m_nDevID);
		Sleep(50);
		SensorEnable(CamSensorTab[0].pin,TRUE,m_nDevID);
		Sleep(50);

		Pwdn2 = CamSensorTab[0].pin & PWDN_H ?  PWDN2_L : PWDN2_H;   //pwdn2 neg to pwdn1
		Pwdn1 = CamSensorTab[0].pin & PWDN_H ?  PWDN_H : PWDN_L;     //pwdn1
		Reset = CamSensorTab[0].pin & RESET_H ?  RESET_H : RESET_L;  //reset

		CamSensorTab[0].pin = Pwdn2 | Pwdn1 | Reset;
		//as the State said.
		SensorEnable(CamSensorTab[0].pin,1,m_nDevID); //reset
	}

	// B通道sensor的reset，pwdn设置
	if(Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);	// 设置当前为B通道
		Pwdn2=0;
		Pwdn1=0;
		Reset=0;
		/* 设置reset pin,pwdn pin*/
		SensorEnable(CamSensorTab[1].pin^RESET_H,TRUE,m_nDevID);
		Sleep(100);
		SensorEnable(CamSensorTab[1].pin,TRUE,m_nDevID);
		Sleep(100);

		Pwdn2 = CamSensorTab[1].pin & PWDN_H ?  PWDN2_L : PWDN2_H;   //pwdn2 neg to pwdn1
		Pwdn1 = CamSensorTab[1].pin & PWDN_H ?  PWDN_H : PWDN_L;     //pwdn1
		Reset = CamSensorTab[1].pin & RESET_H ?  RESET_H : RESET_L;  //reset


		CamSensorTab[1].pin = Pwdn2 | Pwdn1 | Reset;
		//as the State said.
		SensorEnable(CamSensorTab[1].pin,1,m_nDevID); //reset
	}


	Sleep(50);

	// 初始化A通道sensor
	if (Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);	

		if (InitSensor(CamSensorTab[0].SlaveID,CamSensorTab[0].ParaList,CamSensorTab[0].ParaListSize,CamSensorTab[0].mode,m_nDevID) != DT_ERROR_OK)
		{
			AddLog(0,_T("sensorA初始化失败"),COLOR_RED);
			return DT_ERROR_FAILED;
		}
	}

	Sleep(50);

	// 初始化B通道sensor
	if (Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		if (InitSensor(CamSensorTab[1].SlaveID,CamSensorTab[1].ParaList,CamSensorTab[1].ParaListSize,CamSensorTab[1].mode,m_nDevID) != DT_ERROR_OK)
		{
			AddLog(1,_T("sensorB初始化失败"),COLOR_RED);
			return DT_ERROR_FAILED;
		}
	}

	if (Channel & CHANNEL_A)
	{
		if (InitDevice(GetCamDisplayHwnd(0),CamSensorTab[0].width,CamSensorTab[0].height,m_uSensorPort,ImgFormatA,CHANNEL_A,NULL,m_nDevID) != DT_ERROR_OK)
		{ 
			return DT_ERROR_FAILED;
		}

		if (ImgFormatA == D_YUV || ImgFormatA == D_YUV_MTK_S || ImgFormatA == D_YUV_SPI)
			SetYUV422Format(CamSensorTab[0].outformat,m_nDevID);
		else
			SetRawFormat(ImgFormatA,m_nDevID);

		/* 获得图像数据 */
		SizeA = GetByteOffset(CamSensorTab[0].width,ImgFormatA,m_nDevID)*CamSensorTab[0].height;

	}

	if (Channel & CHANNEL_B)
	{
		if (InitDevice(NULL,CamSensorTab[1].width,CamSensorTab[1].height,m_uSensorPort,ImgFormatB,CHANNEL_B,GetCamDisplayHwnd(1),m_nDevID) != DT_ERROR_OK)
		{

			return DT_ERROR_FAILED;
		}
		if (ImgFormatB == D_YUV || ImgFormatB == D_YUV_MTK_S || ImgFormatB == D_YUV_SPI)
			SetYUV422Format(CamSensorTab[1].outformat,m_nDevID);
		else
			SetRawFormat(ImgFormatB ,m_nDevID);

		/* 获得图像数据 */
		SizeB = GetByteOffset(CamSensorTab[1].width,ImgFormatB,m_nDevID)*CamSensorTab[1].height;

	} 

	UINT FrmNum = 0;





	if (Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);

		// A通道ROI
		SetRoiEx(0, 0, CamSensorTab[0].width, CamSensorTab[0].height, 3, 3 , 1, 1, 1,m_nDevID);
		// 计算A通道实际采集数据量
		CalculateGrabSize(&uGrabSize,m_nDevID); 
		SizeA = uGrabSize;

	}

	if (Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		// B通道ROI
		SetRoiEx(0, 0, CamSensorTab[1].width, CamSensorTab[1].height, 3, 3 , 1,1, 1,m_nDevID);
		// 计算B通道实际采集数据量
		CalculateGrabSize(&uGrabSize,m_nDevID); 

		SizeB = uGrabSize;

	}
	Size = (SizeA >= SizeB) ? SizeA : SizeB;
	// 打开视频
	::OpenVideo(Size,m_nDevID); 
	// 设置要抓取的通道
	SetGrabChannelEnable(Channel);
	return 1;
}

int CDevice_UH920::DtSensorTest1(int Camera)
{
	 
	UCHAR ImgFormatA = CamSensorTab[0].type;
	UCHAR ImgFormatB = CamSensorTab[1].type;
	UCHAR PinDef[26] = {PIN_NC};
	 
	ULONG MipiFreq = 0;
	UINT EccErr = 0;
	UINT CrcErr = 0;
	CString str;

	BYTE Channel = CHANNEL_A;
	int iRet = TRUE;
	CString strDevice=L"";

	TRACE("SensorTest Start\r\n");

	m_uSensorPort = SENSOR_PORT_MIPI;

	CString sTemp ;
	m_iChanSel = Camera;
	if (m_iChanSel == 0)
	{
		Channel = CHANNEL_A;
	}
	if (m_iChanSel == 1)
	{
		Channel = CHANNEL_B;
	}
	else if (m_iChanSel == 2)
	{
		Channel = CHANNEL_AB;
	}

	// 设置通道
	ImgFormatA = CamSensorTab[0].type;
	ImgFormatB = CamSensorTab[1].type;
	// 设置IO上拉
	//SetSoftPinPullUp(TRUE,m_nDevID);

	// 电源设置
	// 初始化SENOSR电源

	SENSOR_POWER Power[5];
	int Volt[5];
	int Rise[5];
	BOOL OnOff[5];
	int Current[5];
	CURRENT_RANGE Range[5];
	//修改了上电部分，主要是上电前关闭时钟20160607
	if (Channel & CHANNEL_A)
	{ 
		//*20160607*//
		SetChannelSel(CHANNEL_A,m_nDevID);	
		SensorEnable(0, 1, m_nDevID);
		SetSoftPinPullUp(IO_NOPULL, m_nDevID);
		SetSensorClock(FALSE,(USHORT)(CamSensorTab[0].mclk*10), m_nDevID); 
		Sleep(50);
		//////////////////////////////////////////////////////////////////////////
		Power[0] = POWER_AVDD;
		Volt[0] = 0; // 2.8V
		Current[0] = 500; // 500mA
		Rise[0] = 500;
		OnOff[0] = FALSE;
		//OnOff[0] = TRUE;
		Range[0] = CURRENT_RANGE_MA;
		Power[1] = POWER_DOVDD;
		Volt[1] = 0; // 1.8V
		Current[1] = 500; // 500mA
		Rise[1] = 500;
		OnOff[1] = FALSE;
		Range[1] = CURRENT_RANGE_MA;	
		Power[2] = POWER_DVDD;
		Volt[2] = 0;// 1.8V
		Current[2] =  500;// 500mA
		Rise[2] = 500;
		OnOff[2] = FALSE;
		Range[2] = CURRENT_RANGE_MA;
		Power[3] = POWER_AFVCC;
		Volt[3] = 0; // 2.8V
		Current[3] = 500; // 500mA
		Rise[3] = 1000;
		OnOff[3] = FALSE;
		Range[3] = CURRENT_RANGE_MA;
		Power[4] = POWER_VPP;
		Volt[4] = 0; // 2.8V
		Current[4] = 500; // 500mA
		Rise[4] = 1000;
		OnOff[4] = FALSE;//FLASE 
		Range[4] = CURRENT_RANGE_MA;
		//先要对电压设定成0V，再打开电源开关，再把电压设定到预设值　2015.12.24
		Volt[0] = Volt[1] = Volt[2] = Volt[3] = Volt[4] = 0;
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(10);
		// 100mv/ms
		OnOff[0]=1;
		OnOff[1]=1;
		OnOff[2]=1;
		OnOff[3]=1;
		OnOff[4]=1;
		// 开启电压 开关
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		if (PmuSetRise(Power, Rise, 5, m_nDevID) != DT_ERROR_OK)
		{
			//msg("设置电压斜率失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		Volt[0] = (int)(CamSensorTab[0].avdd * 100);//avdd
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		Volt[2] = (int)(CamSensorTab[0].dvdd * 100);//dovdd
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		Volt[1] = (int)(CamSensorTab[0].dovdd * 100);//dvdd
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		Volt[3] = (int)(CamSensorTab[0].af_vdd*100); //afvcc
		Volt[4] = (int)(CamSensorTab[0].vpp*100); // vpp
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 设置每路限流值
		if (PmuSetOcpCurrentLimit(Power, Current, 5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源限流失败!\r\n");
			return DT_ERROR_FAILED;
		}
		// 设置量程
		if (PmuSetCurrentRange(Power,Range,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("设置量程失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		SetSoftPinPullUp(IO_PULLUP, m_nDevID);
	}

	if (Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);	
		SensorEnable(0, 1, m_nDevID);
		SetSoftPinPullUp(IO_NOPULL, m_nDevID);
		SetSensorClock(FALSE,(USHORT)(CamSensorTab[1].mclk*10), m_nDevID); //add 20160607
		Sleep(50);
		Power[0] = POWER_AVDD_B;
		Volt[0] = 0; // 2.8V
		Current[0] = 500; // 500mA
		Rise[0] = 500;
		OnOff[0] = FALSE;
		Range[0] = CURRENT_RANGE_MA;
		Power[1] = POWER_DOVDD_B;
		Volt[1] = 0; // 1.8V
		Current[1] = 500; // 500mA
		Rise[1] = 500;
		OnOff[1] = FALSE;
		Range[1] = CURRENT_RANGE_MA;
		Power[2] = POWER_DVDD_B;
		Volt[2] = 0;// 1.8V
		Current[2] =  500;// 500mA
		Rise[2] = 500;
		OnOff[2] = FALSE;
		Range[2] = CURRENT_RANGE_MA;
		Power[3] = POWER_AFVCC_B;
		Volt[3] = 0; // 2.8V
		Current[3] = 500; // 500mA
		Rise[3] = 500;
		OnOff[3] = FALSE;
		Range[3] = CURRENT_RANGE_MA;
		Power[4] = POWER_VPP_B;
		Volt[4] = 0; // 2.8V
		Current[4] = 500; // 500mA
		Rise[4] = 500;// 500mv/ms
		OnOff[4] = FALSE;
		Range[4] = CURRENT_RANGE_MA;
		Volt[0] = Volt[1] = Volt[2] = Volt[3] = Volt[4] = 0;
		// 设置电压值
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(10);
		OnOff[0]=TRUE;
		OnOff[1]=TRUE;
		OnOff[2]=TRUE;
		OnOff[3]=TRUE;
		OnOff[4]=TRUE;
		// 开启电压 / set power on;
		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("开启电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		// 500mv/ms
		if (PmuSetRise(Power, Rise, 5, m_nDevID) != DT_ERROR_OK)
		{
			//msg("设置电压斜率失败!\r\n");
			return DT_ERROR_FAILED;
		}
		//设定每一路电压值 set  power value
		Sleep(2);
		Volt[0] = (int)(CamSensorTab[1].avdd * 100);//avdd
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		Volt[2] = (int)(CamSensorTab[1].dvdd * 100);//dvdd
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		Volt[1] = (int)(CamSensorTab[1].dovdd * 100);//dovdd
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		Volt[3] = (int)(CamSensorTab[1].af_vdd*100); // 2.8V * 1000);
		Volt[4] = (int)(CamSensorTab[1].vpp*100);
		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		// 设置每路限流值
		if (PmuSetOcpCurrentLimit(Power, Current, 5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化电源限流失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(2);
		// 设置量程
		if (PmuSetCurrentRange(Power,Range,5,m_nDevID) != DT_ERROR_OK)
		{
			//msg("设置量程失败!\r\n");
			return DT_ERROR_FAILED;
		}
		Sleep(50);
		SetSoftPinPullUp(IO_PULLUP, m_nDevID);
	}

	// 400Kbps I2C
	SetSensorI2cRate(TRUE,m_nDevID);

	SetSensorI2cRapid(1, m_nDevID);//

	//SetI2CInterval(0, m_nDevID); 
	if (Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);

		if (SetSensorClock(TRUE,(USHORT)(CamSensorTab[0].mclk*10),m_nDevID) != DT_ERROR_OK) // 20MHz
		{
			//msg("设置SENSOR时钟失败!\r\n");
			DtClosePower(Channel);
			return FALSE;
		}
	}

	if (Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		if (SetSensorClock(TRUE,(USHORT)(CamSensorTab[1].mclk*10),m_nDevID) != DT_ERROR_OK) // 20MHz
		{
			//msg("设置SENSOR时钟失败!\r\n");
			DtClosePower(Channel);
			return FALSE;
		}
	}

	BYTE Pwdn2=0;
	BYTE Pwdn1=0;
	BYTE Reset=0;
	Sleep(50);
	// A通道sensor的reset，pwdn设置
	if(Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);	// 设置当前为A通道
		/* 设置reset pin,pwdn pin*/

		SensorEnable(CamSensorTab[0].pin^RESET_H,TRUE,m_nDevID);
		Sleep(50);
		SensorEnable(CamSensorTab[0].pin,TRUE,m_nDevID);
		Sleep(50);

		Pwdn2 = CamSensorTab[0].pin & PWDN_H ?  PWDN2_L : PWDN2_H;   //pwdn2 neg to pwdn1
		Pwdn1 = CamSensorTab[0].pin & PWDN_H ?  PWDN_H : PWDN_L;     //pwdn1
		Reset = CamSensorTab[0].pin & RESET_H ?  RESET_H : RESET_L;  //reset

		CamSensorTab[0].pin = Pwdn2 | Pwdn1 | Reset;
		//as the State said.
		SensorEnable(CamSensorTab[0].pin,1,m_nDevID); //reset


	}

	// B通道sensor的reset，pwdn设置
	if(Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);	// 设置当前为B通道
		Pwdn2=0;
		Pwdn1=0;
		Reset=0;
		//设置reset pin,pwdn pin
		SensorEnable(CamSensorTab[1].pin^RESET_H,TRUE,m_nDevID);
		Sleep(100);
		SensorEnable(CamSensorTab[1].pin,TRUE,m_nDevID);
		Sleep(100);

		Pwdn2 = CamSensorTab[1].pin & PWDN_H ?  PWDN2_L : PWDN2_H;   //pwdn2 neg to pwdn1
		Pwdn1 = CamSensorTab[1].pin & PWDN_H ?  PWDN_H : PWDN_L;     //pwdn1
		Reset = CamSensorTab[1].pin & RESET_H ?  RESET_H : RESET_L;  //reset


		CamSensorTab[1].pin = Pwdn2 | Pwdn1 | Reset;
		//as the State said.
		SensorEnable(CamSensorTab[1].pin,1,m_nDevID); //reset
	}

	Sleep(50);

	// 初始化A通道sensor
	if (Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);	

		if (InitSensor(CamSensorTab[0].SlaveID,CamSensorTab[0].ParaList,CamSensorTab[0].ParaListSize,CamSensorTab[0].mode,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化A通道SENSOR失败!\r\n");
			DtClosePower(Channel);
			return DT_ERROR_FAILED;
		}
	}

	Sleep(50);

	// 初始化B通道sensor
	if (Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		if (InitSensor(CamSensorTab[1].SlaveID,CamSensorTab[1].ParaList,CamSensorTab[1].ParaListSize,CamSensorTab[1].mode,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化B通道SENSOR失败!\r\n");
			DtClosePower(Channel);
			return DT_ERROR_FAILED;
		}
	}

	if (Channel & CHANNEL_A)
	{
		ImgFormatA = CamSensorTab[0].type;
		if (InitDevice(NULL,CamSensorTab[0].width,CamSensorTab[0].height,m_uSensorPort,ImgFormatA,CHANNEL_A,NULL,m_nDevID) != DT_ERROR_OK)

		{
			//msg("初始化设备失败!\r\n");
			DtClosePower(Channel);
			return DT_ERROR_FAILED;
		}

		if (ImgFormatA == D_YUV || ImgFormatA == D_YUV_MTK_S || ImgFormatA == D_YUV_SPI)
			SetYUV422Format(CamSensorTab[0].outformat,m_nDevID);
		else
			SetRawFormat(CamSensorTab[0].outformat,m_nDevID);

		/* 获得图像数据 */
		SizeA = GetByteOffset(CamSensorTab[0].width,ImgFormatA,m_nDevID)*CamSensorTab[0].height;
		//msg("Image SizeA=%d(%08X) bytes\r\n",SizeA,SizeA);
	}

	if (Channel & CHANNEL_B)
	{
		ImgFormatB = CamSensorTab[1].type;
		if (InitDevice(NULL,CamSensorTab[1].width,CamSensorTab[1].height,m_uSensorPort,ImgFormatB,CHANNEL_B,NULL,m_nDevID) != DT_ERROR_OK)
		{
			//msg("初始化设备失败!\r\n");
			DtClosePower(Channel);
			return DT_ERROR_FAILED;
		}
		if (ImgFormatB == D_YUV || ImgFormatB == D_YUV_MTK_S || ImgFormatB == D_YUV_SPI)
			SetYUV422Format(CamSensorTab[1].outformat,m_nDevID);
		else
			SetRawFormat(CamSensorTab[1].outformat,m_nDevID);

		/* 获得图像数据 */
		SizeB = GetByteOffset(CamSensorTab[1].width,ImgFormatB,m_nDevID)*CamSensorTab[1].height;
		//msg("Image SizeB=%d(%08X) bytes\r\n",SizeB,SizeB);
	}


	if (Channel & CHANNEL_A)
	{
		SetChannelSel(CHANNEL_A,m_nDevID);
		// A通道ROI
		SetRoiEx(0, 0, CamSensorTab[0].width, CamSensorTab[0].height, 3, 3 , 1, 1, 1,m_nDevID);
		// 计算A通道实际采集数据量
		CalculateGrabSize(&uGrabSize,m_nDevID);
		SizeA = uGrabSize;//// 2016.01.07
		//msg("CalculateGrabSizeA:%u\r\n",uGrabSize);

	}

	if (Channel & CHANNEL_B)
	{
		SetChannelSel(CHANNEL_B,m_nDevID);

		// B通道ROI
		SetRoiEx(0, 0, CamSensorTab[1].width, CamSensorTab[1].height, 3, 3 , 1, 1, 1,m_nDevID);
		// 计算B通道实际采集数据量
		CalculateGrabSize(&uGrabSize,m_nDevID);
		SizeB = uGrabSize;// 2016.01.07
		//msg("CalculateGrabSizeB:%u\r\n",uGrabSize); 
	}

	// 打开视频
	Size = (SizeA >= SizeB) ? SizeA : SizeB;
	//Size = SizeA>SizeB?SizeA:SizeB;
	::OpenVideo(Size,m_nDevID);
	// CString strtemp;
	//strtemp.Format("%d",Openvalue);
	//AfxMessageBox(strtemp);

	// 设置要抓取的通道
	SetGrabChannelEnable(Channel);

	return 1;
}

int CDevice_UH920::DtClosePower(int byChannel)
{
	SENSOR_POWER Power[5];
	int Volt[5];
	BOOL OnOff[5];

	if (byChannel & CHANNEL_A)
	{
		Power[0] = POWER_AVDD;
		Volt[0] = 0; 
		OnOff[0] = FALSE;

		Power[1] = POWER_DOVDD;
		Volt[1] = 0; 
		OnOff[1] = FALSE;

		Power[2] = POWER_DVDD;
		Volt[2] = 0;
		OnOff[2] = FALSE;

		Power[3] = POWER_AFVCC;
		Volt[3] = 0; 
		OnOff[3] = FALSE;

		Power[4] = POWER_VPP;
		Volt[4] = 0;
		OnOff[4] = FALSE;

		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			return DT_ERROR_FAILED;
		}

		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			return DT_ERROR_FAILED;
		}
	}

	if (byChannel & CHANNEL_B)
	{
		Power[0] = POWER_AVDD_B;
		Volt[0] = 0;
		OnOff[0] = FALSE;

		Power[1] = POWER_DOVDD_B;
		Volt[1] = 0;
		OnOff[1] = FALSE;

		Power[2] = POWER_DVDD_B;
		Volt[2] = 0;
		OnOff[2] = FALSE;

		Power[3] = POWER_AFVCC_B;
		Volt[3] = 0;
		OnOff[3] = FALSE;

		Power[4] = POWER_VPP_B;
		Volt[4] = 0;
		OnOff[4] = FALSE;

		if (PmuSetVoltage(Power,Volt,5,m_nDevID) != DT_ERROR_OK)
		{
			return DT_ERROR_FAILED;
		}

		if (PmuSetOnOff(Power,OnOff,5,m_nDevID) != DT_ERROR_OK)
		{
			return DT_ERROR_FAILED;
		}
	}

	return DT_ERROR_OK;
}
BOOL CDevice_UH920::InitiaSensor(int CamID,CString SettingPath)
{
	if (SettingPath.GetLength()>4)
	{
		;//添加New 点亮设定代码
	}
	return TRUE;
}