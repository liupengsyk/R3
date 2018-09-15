#include "StdAfx.h"
#include "ccmBaseInterface.h"
#include "Mud952.h"
#include <map>
using namespace std;

#pragma comment(lib,"dtccm2_mud952/dtccm2.lib")

Mud952::Mud952(void)
{
	DevicesTol = 0;
	isOpened = FALSE;
	for(int i=0;i<4;i++)
	{
		m_nDevID[i] = -1;
		devVec[i] = L"";
	
	}
	bOpenShortFinish[0] = FALSE;
	bOpenShortFinish[1] = FALSE;
}


Mud952::~Mud952(void)
{
	for(int i=0;i<4;i++)
	{
		m_nDevID[i] = -1;
		devVec[i] = L"";
	}
	
}

void Mud952::SetCamTab(int CamID)
{
	LoadSensorGenaralTab(CamID); 
	LoadSensorParalist(CamID); 
}

void Mud952::SetGpioPinLevel(int gpioNum,BOOL bH,int CamID)
{
	::SetGpioPinLevel(gpioNum,bH,m_nDevID[CamID]);
}
void Mud952::SensorEnable(int gpioNum,BOOL bH,int CamID)
{
	//::SetGpioPinLevel(gpioNum,bH,m_nDevID[CamID]);
	//gpioNum=1:pwdn 2:reset 3:pwdn&reset
	//bH=0:低平 1：高pin
	if (bH==1)
	{
		if((m_nDevID[CamID]>=0)&&(m_nDevID[CamID]<500))
		::SensorEnable(gpioNum,1,m_nDevID[CamID]);
		else
			{
				CString str;
				str.Format(_T("SensorEnable失败，DevID异常,DevID=%d"),m_nDevID[CamID]);
				AddLog(CamID,str,COLOR_RED,200);
		}
	}
	else
	{
		if((m_nDevID[CamID]>=0)&&(m_nDevID[CamID]<500))
		::SensorEnable(gpioNum^3,1,m_nDevID[CamID]);
		else
		{
			CString str;
			str.Format(_T("SensorEnable失败，DevID异常,DevID=%d"),m_nDevID[CamID]);
			AddLog(CamID,str,COLOR_RED,200);
		}
	}
}
/*
vector<CString> Mud952::GetDevList()
{
	char *DeviceName[4]={NULL};
	
	int DeviceNum;
	vector<CString> DevNameList;
	EnumerateDevice(DeviceName,4,&DeviceNum);
	for(int i=0;i<DeviceNum;i++)
	{
		CString st(DeviceName[i]);
		DevNameList.push_back(st);
		if(DeviceName[i]!=NULL)
		GlobalFree(DeviceName[i]);
	}

	return DevNameList;
	}

*/
vector<CString> Mud952::GetDevList()
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

		CString Tempstr1(TempDeviceSN);
		CloseDevice(i);
		m_nDevID[i]=-1;
		CString st(DeviceName[i]);
		st=Tempstr1+_T("_")+st;
		DevNameList.push_back(st);
		if(DeviceName[i]!=NULL)
			GlobalFree(DeviceName[i]);

	}
	return DevNameList;
}
	 
int  Mud952::GetKitTypeDeviceInfo(int CamID)
{
	if (CamID>1)
	{
		return -1;
	}
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
			CString Tempstr2(TempDeviceSN);
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

// int  Mud952::GetKitTypeDeviceInfo(int CamID)
// {  
// 	CString str;
// 	 
// 	char *DtDeviceName[4]={NULL};
//  	int DeviceNum; 
// 	EnumerateDevice(DtDeviceName,4,&DeviceNum);
//  	int iDevIDA = 0;
// 
// 	if (DeviceNum==4)//一次只打开一个
// 	{
// 		if (devName.GetLength()<3)
// 		{ 
// 			str.Format(_T("没有指定度信:%s，请重新设定度信"),devName);
// 			AddLog(CamID,str,COLOR_RED,200); 
// 			return -1;
// 		}
// 
// 		if(m_nDevID[CamID]>=0)
// 			::CloseDevice(m_nDevID[CamID]);
// 
// 		BOOL  devFind= FALSE;
// 		for (int i=0; i<DeviceNum; i++)
// 		{
// 			CString tempDev(DtDeviceName[i]); 
//  			if (tempDev== devName)
// 			{	
// 				devFind = TRUE;
// 				if (::OpenDevice(DtDeviceName[i],&iDevIDA) != DT_ERROR_OK)	
// 				{ 
// 					str.Format(_T("Open %s failed"),tempDev);
// 					AddLog(CamID,str,COLOR_RED,200);
// 					if(DtDeviceName[i]!=NULL)
// 						GlobalFree(DtDeviceName[i]);
// 					return -1;
// 				} 			  		
// 				char ReadBufA[32];
// 			    int iRet;
/// 
// 				memset(ReadBufA,0,32);
// 
// 				iRet = ReadUserDefinedName(ReadBufA,32,&iDevIDA);	// 读取长度根据写入长度设置
// 				  
// 				CString UD_DevName(ReadBufA);
// 
// 				UD_DevName.MakeUpper();
// 				// 名称对比，绑定设备ID和用户名，读取到的设备名称和用户选择的设备名称一致
// 				if (UD_DevName.Find(_T("CAM0"))!=-1)		// 若读到空名字
// 				{		
// 					if (iDevIDA!=0)
// 					{
// 						m_nDevID[1] = 0;
// 					}
// 					m_nDevID[0] = iDevIDA;
// 					devVec[0] = UD_DevName;
// 					CString str;
// 					str.Format(_T("devID:%d,name:%s"),iDevIDA,tempDev);
// 					AddLog(CamID,str,COLOR_BLUE,200); 
// 				}
// 
// 				else if(UD_DevName.Find(_T("CAM1"))!=-1)
// 				{
//  					if (iDevIDA!=1)
// 					{
// 						m_nDevID[0] = 1;
// 					}
// 					m_nDevID[1] = iDevIDA;
// 					devVec[1] = UD_DevName;
// 					CString str;
// 					str.Format(_T("devID:%d,name:%s"),iDevIDA,tempDev);
// 					AddLog(1,str,COLOR_BLUE,200); 
// 				} 
// 				else 
// 				{
// 					CString str;
// 					str.Format(_T("请对度信命名_devID:%d,name:%s"),iDevIDA,tempDev);
// 					AddLog(CamID,str,COLOR_RED,200); 
// 
// 				} 
// 			}
// 			if(DtDeviceName[i]!=NULL)
// 				GlobalFree(DtDeviceName[i]);
// 		}
// 		if (!devFind)
// 		{
// 			CString str;
// 			str.Format(_T("没有找到指定度信:%s，请重新设定度信"),devName);
// 			AddLog(CamID,str,COLOR_RED,200); 
// 			return -1;
// 		}
// 		isOpened = TRUE;
// 	}
// 	else if(DeviceNum==2)
// 	{
// 		//防止第二次被打开
// 		if (isOpened )
// 		{
// 			return 1;
// 		}
// 		isOpened = TRUE;
// 		for (int i=0;i<2;i++)
// 		{
// 			if(m_nDevID[i]>=0)
// 				::CloseDevice(m_nDevID[i]);// 一次性打开
// 		}
// 
// 		for (int i=0; i<DeviceNum; i++)
// 		{
// 			CString tempDev(DtDeviceName[i]); 
// 			//AddLog(i,tempDev,COLOR_BLUE,200);
// 
// 			iDevIDA = 0;
// 			if (::OpenDevice(DtDeviceName[i],&iDevIDA) != DT_ERROR_OK)		
// 			{ 
// 				str.Format(_T("Open %s failed"),tempDev);
// 				AddLog(i,str,COLOR_RED,200);
// 				return -1;
// 			}
// 
// 			str.Format(_T("Open %s pass,iDevIDA:%d,i:%d"),tempDev,m_nDevID[CamID],i);
//             AddLog(CamID,str,COLOR_BLUE,200);
// 
// 			char ReadBufA[32];
// 		    int iRet;
/// 			 
// 			memset(ReadBufA,0,32);
// 
// 			iRet = ReadUserDefinedName(ReadBufA,32,&iDevIDA);	// 读取长度根据写入长度设置
// 			 CString UD_DevName(ReadBufA);
// 
// 			UD_DevName.MakeUpper();
// 			// 名称对比，绑定设备ID和用户名，读取到的设备名称和用户选择的设备名称一致
// 			if (UD_DevName.Find(_T("CAM0"))!=-1)		// 若读到空名字
// 			{	
// 				if (iDevIDA!=0)
// 				{
// 					m_nDevID[1] = 0;
// 				}
// 				m_nDevID[0] = iDevIDA;
// 				devVec[0] = UD_DevName;
// 				CString str;
// 				str.Format(_T("devID:%d,name:%s"),iDevIDA,tempDev);
// 				AddLog(CamID,str,COLOR_BLUE,200); 
// 			}
// 
// 			else if(UD_DevName.Find(_T("CAM1"))!=-1)
// 			{
// 				/*devMap_NameId.insert(pair<CString,int>(DevName,iDevIDA)); */
// 				if (iDevIDA!=1)
// 				{
// 					m_nDevID[0] = 1;
// 				}
// 				m_nDevID[1] = iDevIDA;
// 				devVec[1] = UD_DevName;
// 				CString str;
// 				str.Format(_T("devID:%d,name:%s"),iDevIDA,tempDev);
// 				AddLog(1,str,COLOR_BLUE,200); 
// 			} 
// 			else 
// 			{
// 				CString str;
// 				str.Format(_T("请对度信命名_devID:%d,name:%s"),iDevIDA,tempDev);
// 				AddLog(CamID,str,COLOR_RED,200); 
// 			} 
// 
// 			GlobalFree(DtDeviceName[i]);
// 		}
// 	}
// 	
// 	else if (DeviceNum==1)
// 	{
// 		isOpened = TRUE;
// 	 		if(m_nDevID[CamID]>=0)
// 				::CloseDevice(m_nDevID[CamID]);// 一次性打开
// 	  		if (::OpenDevice(DtDeviceName[0],&iDevIDA,m_nDevID[CamID]) != DT_ERROR_OK)
// 			{ 
// 				str.Format(_T("Open %s failed"),DtDeviceName[0]);
// 				AddLog(CamID,str,COLOR_RED,200);
// 				return -1;
// 			} 
// 			GlobalFree(DtDeviceName[0]);
// 		return TRUE;
// 	}
// 	else
// 	{
// 		AddLog(CamID,L"度信连接异常",COLOR_RED,200);
// 		isOpened = FALSE;
// 	} 
// 	return isOpened;
// }


int  Mud952::OpenMUD952(int CamID)
{  
	//DWORD Ver[4];
	//CString DtDeviceName = m_wndDevList[CamID];
	//if(DtDeviceName.GetLength()<2)
	//{
	//	return -1;
	//}
	//int DevID  = m_nDevID[CamID];
	//char buff[1024]={0};
	//int bufflen=1024; 
	//int len = DtDeviceName.GetLength();
	//WideCharToMultiByte(CP_ACP,0,DtDeviceName,len,buff,len,NULL,NULL);//T2A,W2A

	//if (DevID >= 0)
	//	CloseDevice(DevID);

	//if (::OpenDevice(buff,&DevID,0) == DT_ERROR_OK)
	//{
	//	m_nDevID[CamID] = DevID;
	//	TRACE("Open device successful\n");
	//	if (GetLibVersion(Ver,DevID) != DT_ERROR_OK)
	//		return -1;
	//	TRACE("LIB Version:%d.%d.%d.%d\n",Ver[0],Ver[1],Ver[2],Ver[3]);

	//	if (GetFwVersion(0,Ver,DevID) != DT_ERROR_OK)
	//		return -1;
	//	TRACE("FW1 Version:%d.%d.%d.%d\n",Ver[0],Ver[1],Ver[2],Ver[3]);

	//	if (GetFwVersion(1,Ver,DevID) != DT_ERROR_OK)
	//		return -1;
	//	TRACE("FW2 Version:%d.%d.%d.%d\n",Ver[0],Ver[1],Ver[2],Ver[3]);

	//	if (GetFwVersion(2,Ver,DevID) != DT_ERROR_OK)
	//		return -1;
	//	TRACE("FW3 Version:%d.%d.%d.%d\n",Ver[0],Ver[1],Ver[2],Ver[3]);
	//	return 1;
	//}


	//TRACE("Open device failed!\n");
	return 1;
}

int  Mud952::CloseDevice(int CamID)
{
//	SetMipiEnable(FALSE, m_nDevID[CamID]);
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
	PmuSetVoltage(Power, Volt, 5, m_nDevID[CamID]);
	Sleep(50);
	EnableSoftPin(FALSE, m_nDevID[CamID]);
	EnableGpio(FALSE, m_nDevID[CamID]);

	//DtPowerClose(3);
	if (m_nDevID[CamID] >= 0&&m_nDevID[CamID]<500)
	{
		::CloseDevice(m_nDevID[CamID]);
		m_nDevID[CamID]=-1;
	}
	m_nDevID[CamID]=-1;
	isOpened = FALSE;
	return 0;
}
int Mud952::OpenDevice(CString devName)
{
	return 1;
}

int  Mud952::OpenVideo(int CamID)
{ 
	int DevID = m_nDevID[CamID];
	DeviceSensorTab CurrentCam = CamSensorTab[CamID];

	UCHAR ImgFormat = D_RAW8;

	CString str; 
	int iRet = TRUE;

	AddLog(CamID,_T("OpenVideo Start"),COLOR_GREEN);
	TRACE("SensorTest Start\r\n"); 
	CString sTemp ; 
	// 设置通道
	ImgFormat =CurrentCam.type;

	SetMipiEnable(TRUE,DevID);
	SetSoftPinPullUp(IO_NOPULL,DevID); 
	SetSoftPin(0,DevID);
	SetSoftPinPullUp(IO_PULLUP,DevID); 
	EnableGpio(TRUE,DevID);

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
	Volt[0] = 0; // 2.8V
	Current[0] = 500; // 500mA
	Rise[0] = 100;
	OnOff[0] = TRUE;
	Range[0] = CURRENT_RANGE_MA;

	Power[1] = POWER_DOVDD;
	Volt[1] = 0; // 1.8V
	Current[1] = 500; // 500mA
	Rise[1] = 100;
	OnOff[1] = TRUE;
	Range[1] = CURRENT_RANGE_MA;	

	Power[2] = POWER_DVDD;
	Volt[2] = 0;// 1.8V
	Current[2] =  500;// 500mA
	Rise[2] = 100;
	OnOff[2] = TRUE;
	Range[2] = CURRENT_RANGE_MA;

	Power[3] = POWER_AFVCC;
	Volt[3] = 0; // 2.8V
	Current[3] = 500; // 500mA
	Rise[3] = 100;
	OnOff[3] = TRUE;
	Range[3] = CURRENT_RANGE_MA;

	Power[4] = POWER_VPP;
	Volt[4] = 0; // 2.8V
	Current[4] = 500; // 500mA
	Rise[4] = 100;
	OnOff[4] = TRUE;
	Range[4] = CURRENT_RANGE_MA;
	CString log;
	// 设置电压值
	if (::PmuSetVoltage(Power,Volt,5,DevID) != DT_ERROR_OK)
	{
		//	msg("初始化电源失败!\r\n");
		log.Format(L"name:%s id:%d log:",devVec[CamID],DevID,_T("初始化电源失败0!"));
		AddLog(CamID,log,COLOR_RED);
		return DT_ERROR_FAILED;
	}

	// 开启电压
	if (::PmuSetOnOff(Power,OnOff,5,DevID) != DT_ERROR_OK)
	{
		log.Format(L"name:%s id:%d log:",devVec[CamID],DevID,_T("开启电源失败1!"));
		AddLog(CamID,log,COLOR_RED); 
		return DT_ERROR_FAILED;
	}
	Volt[0] = (int)(CurrentCam.avdd *100); // 2.8V

	Volt[1] = (int)(CurrentCam.dovdd *100); // 1.8V 
	Volt[2] = (int)(CurrentCam.dvdd *100);// 1.8V 
	Volt[3] = (int)(CurrentCam.af_vdd *100); // 2.8V 	 
	Volt[4] = (int)(CurrentCam.vpp *100); // 2.8V 
	 
	// 100mv/ms
	if (::PmuSetRise(Power, Rise, 5, DevID) != DT_ERROR_OK)
	{
		/*msg("设置电压斜率失败!\r\n");
		AddLog(0,_T("设置电压斜率失败!"),COLOR_RED);*/
		//return DT_ERROR_FAILED;
	}

	// 设置电压值
	if (::PmuSetVoltage(Power,Volt,5,DevID) != DT_ERROR_OK)
	{
		//	msg("初始化电源失败!\r\n"); 
		log.Format(L"name:%s id:%d log:",devVec[CamID],DevID,_T("初始化电源失败2!"));
		AddLog(CamID,log,COLOR_RED); 
		return DT_ERROR_FAILED;
	}

	// 开启电压
	if (::PmuSetOnOff(Power,OnOff,5,DevID) != DT_ERROR_OK)
	{
		//	msg("开启电源失败!\r\n"); 
		log.Format(L"name:%s id:%d log:",devVec[CamID],DevID,_T("开启电源失败!3"));
		AddLog(CamID,log,COLOR_RED); 

		return DT_ERROR_FAILED;
	}
	Sleep(500);

	// 设置每路限流值
	if (::PmuSetOcpCurrentLimit(Power, Current, 5,DevID) != DT_ERROR_OK)
	{
		log.Format(L"name:%s id:%d log:",devVec[CamID],DevID,_T("初始化电源限流失败!4"));
		AddLog(CamID,log,COLOR_RED); 

		//	msg("初始化电源限流失败!\r\n");
		//AddLog(0,_T("初始化电源限流失败!"),COLOR_RED);
		return DT_ERROR_FAILED;
	}
	if(CamSensorTab[CamID].bTestStandyCurrent)
	{
		// 设置量程

		ResetSensorI2cBus(DevID);
		::SensorEnable(CamSensorTab[CamID].pin^3,TRUE,DevID);
		SetSensorClock(FALSE,(USHORT)(CamSensorTab[CamID].mclk *10),DevID);
		
		if (::PmuSetCurrentRange(Power,RangeUA,5,DevID) != DT_ERROR_OK)
		{
			log.Format(L"name:%s id:%d log:",devVec[CamID],DevID,_T("设置uA量程失败!5"));
			AddLog(CamID,log,COLOR_RED); 
			return DT_ERROR_FAILED;
		}

		//LING Add for DriverIC standby
		DriverICStandby(CamID);

		Sleep(2500);
		if (0 == CamID)
		{
			PmuGetCurrent(iStandyCurrent_Left,5,0,CamID);
		}
		else
		{
			PmuGetCurrent(iStandyCurrent_Right,5,0,CamID);
		}
	
	}

	// 设置mA量程
	if (::PmuSetCurrentRange(Power,Range,5,DevID) != DT_ERROR_OK)  
	{
		log.Format(L"name:%s id:%d log:",devVec[CamID],DevID,_T("设置mA量程失败2 !5"));
		AddLog(CamID,log,COLOR_RED); 
		return DT_ERROR_FAILED;
	}

	//Sleep(10);
	if (SetSensorClock(TRUE,(USHORT)(CurrentCam.mclk *10),DevID) != DT_ERROR_OK) // 20MHz
	{
		log.Format(L"name:%s id:%d log:",devVec[CamID],DevID,_T("设置量设置SENSOR时钟失败程失败!!6"));
		AddLog(CamID,log,COLOR_RED);  
		return FALSE;
	}

	//开启IO上拉电阻
	SetSoftPinPullUp(IO_PULLUP,DevID); 
	EnableGpio(TRUE,DevID);
	Sleep(10);
	SetSensorI2cRate(TRUE,DevID);
	SetSensorI2cRapid(FALSE, DevID);
	SetSensorI2cAckWait(500,DevID);

	Sleep(50); 	

	/* 设置reset pin,pwdn pin*/

	SensorEnable(CurrentCam.pin^RESET_H,TRUE,CamID);
	Sleep(20);

	SensorEnable(CurrentCam.pin,TRUE,CamID);
	Sleep(50);

	
	int Flag = InitSensor(CurrentCam.SlaveID,CurrentCam.ParaList,CurrentCam.ParaListSize,CurrentCam.mode,DevID);
	if ( Flag != DT_ERROR_OK)
	{
		AddLog(CamID,_T("sensor初始化失败"),COLOR_RED);
		return DT_ERROR_FAILED;
	} 
	AddLog(CamID,_T("sensor初始化成功"),COLOR_GREEN);
	Sleep(20);

	if (ImgFormat == D_YUV || ImgFormat == D_YUV_MTK_S || ImgFormat == D_YUV_SPI)
		SetYUV422Format(CurrentCam.outformat,DevID);
	else
		SetRawFormat(CurrentCam.outformat,DevID);

	/* 获得图像数据 */
	Size = GetByteOffset(CurrentCam.width,ImgFormat,DevID)*CurrentCam.height;


	USHORT roi_x0 = CurrentCam.width >> 2;
	USHORT roi_y0 = CurrentCam.height >> 2;
	USHORT roi_hb = 0;
	USHORT roi_vb = 0;
	USHORT roi_hnum = 1;
	USHORT roi_vnum = 1;

	USHORT fifo_div = 2;	
	roi_x0 = 0;
	roi_y0 = 0;
	roi_hb = 0;
	roi_vb = 0;
	roi_hnum = 1;
	roi_vnum = 1;
	//初始化设备
	InitRoi(0+CurrentCam.iXRoi, 0+CurrentCam.iYRoi, CurrentCam.width, CurrentCam.height , 0, 0, 1, 1, CurrentCam.type, TRUE, DevID);	//以像素为单位，全分辨率显示关闭ROI使能
	SetSensorPort(CurrentCam.port, CurrentCam.width, CurrentCam.height, DevID);
	SetMipiClkPhase(0,DevID);
	SetMipiEnable(TRUE,DevID);//LP or HS 
	Sleep(10);

	// 计算A通道实际采集数据量
	CalculateGrabSize(&uGrabSize,DevID);  
	// 打开视频
	::OpenVideo(uGrabSize,DevID); 
	// 设置要抓取的通道

	Sleep(20);

	if (InitDisplay(GetCamDisplayHwnd(CamID),CurrentCam.width,CurrentCam.height,ImgFormat,CHANNEL_A,NULL,DevID) != DT_ERROR_OK)
	{
		AddLog(CamID,_T("InitDisplay失败"),COLOR_RED);
		return DT_ERROR_FAILED;
	}
	InitIsp(CurrentCam.width, CurrentCam.height, CurrentCam.type, CHANNEL_A , DevID);

	SetGamma(100, DevID);  //Gamma  :default 100 is no gamma change....
	SetContrast(100), DevID; //Contrast :default 100 is no contrast change...
	SetSaturation(128, DevID);//Saturation: default 128 is no saturation change...
	SetDigitalGain(1.0, 1.0, 1.0, DevID); //AWB digital gian R G B

	return 1;  

}

int  Mud952::GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int CamID)
{
	if((m_nDevID[CamID]>=0)&&(m_nDevID[CamID]<500)) return ::GrabFrame(pInBuffer,uBufferSize,pGrabSize,pInfo, m_nDevID[CamID]);
	else 
	{
       AddLog(CamID,_T("DeviceID异常"),COLOR_RED,200); 
	   return 0;
	}
}

int  Mud952::WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID,int CamID )
{  
	return ::WriteSensorI2c( uDevAddr, uRegAddr, uRegAddrSize,pData, uSize, m_nDevID[CamID]); 
}

int  Mud952::ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop,int iDevID,int CamID )
{

	return ::ReadSensorI2c( uDevAddr, uRegAddr, uRegAddrSize,pData, uSize, TRUE, m_nDevID[CamID]);

}


int Mud952::WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID,int CamID)
{		 
	return ::WriteSensorReg(  uAddr,uReg,uValue,byMode,m_nDevID[CamID]);
}

int Mud952::ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID,int CamID)
{

	return ::ReadSensorReg(  uAddr,uReg,pValue, byMode,m_nDevID[CamID]);
}

int Mud952::OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) 
{
	//return DTOSTest(pOSTab,OSValueArray, CamID);
	BOOL bFlag  = FALSE;
	CString str;
	str.Format(_T("Cam %d 工装启动中OS"),CamID);
	AddLog(CamID,str,COLOR_BLUE,200); 
	//bFlag = OpenMUD952(CamID);

	//上电
	//设置电压，电流
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
	OS_Config((int)(1.5f*1000*1000), HighLimit, LowLimit, 45, 2*1000, 1*1000,m_nDevID[CamID]);

	//OS 测试，进行该PIN为low的测试。
	//command bit7 :enalble os ; bit3:high test; bit2:low test...
	ULONG PinMask_L = pOSTab->PinMask_L; 
	ULONG PinMask_H = pOSTab->PinMask_H;  

	UCHAR IoMask[10] = {0};

	LC_OS_CommandConfig(0x40, IoMask, 10, m_nDevID[0]);
	LC_OS_CommandConfig(0x40, IoMask, 10, m_nDevID[1]);

	/*LC_OS_CommandConfig(0x40, IoMask, 10, m_iDevIDA);
	LC_OS_CommandConfig(0x40, IoMask, 10, m_iDevIDB);*/

	IoMask[0] = (UCHAR)((PinMask_L) & 0xff);
	IoMask[1] = (UCHAR)((PinMask_L>>8) & 0xff);
	IoMask[2] = (UCHAR)((PinMask_L>>16) & 0xff);
	IoMask[3] = (UCHAR)((PinMask_L>>24) & 0xff);

	IoMask[4] = (UCHAR)((PinMask_H) & 0xff);
	IoMask[5] = (UCHAR)((PinMask_H>>8) & 0xff);
	IoMask[6] = (UCHAR)((PinMask_H>>16) & 0xff);
	IoMask[7] = (UCHAR)((PinMask_H>>24) & 0xff);
	LC_OS_CommandConfig(0x80 | 0x08 | 0x04, IoMask, 10,m_nDevID[CamID]);

	int VoltageH[45] = {0};
	int VoltageL[45] = {0};
	UCHAR Result[45] = {0};


	int nRet = OS_Read(VoltageH, VoltageL, Result, 0, 1, 45,m_nDevID[CamID]);

	if( 1 == nRet)
	{
		for (int i=0; i<45; i++)
		{
			OSValueArray[i] = VoltageL[i];
		}
	}
	else
	{  
		//CString str;
		//str.Format(_T("开短路错误%d，请重试"), nRet);
		//AfxMessageBox(str);
		return -3;
	}
	return 1;
}

int Mud952::DTOSTest(OSTab *pOSTab, int* OSValueArray,int CamID)
{
	int iTryTimes = 0;
	while(iTryTimes<60 && (m_nDevID[0]<0 ||m_nDevID[1] < 0)) ////等待两个模组的设备都打开了
	{
		Sleep(50);
		iTryTimes++;
	}
	bOpenShortFinish[CamID%2] = FALSE;


	//设置电压，电流
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
	//20130703 changed....
	//os test....
	//开短路测试是否通过的结果存到Result中,对应值非0代表NG。
	UCHAR Result[45] = {0};
	//传递给设备的IOMask值。
	//IoMask[0]  PIN7:0;
	//IoMask[1]  PIN15:8;
	//IoMask[2]  PIN23:16;
	//IoMask[3]  PIN31:24;
	//IoMask[4]  ....
	//
	UCHAR IoMask[10] = {0};

	if (1)
	{
		// 如果共地，断开测试板A，断开测试板B
		// IoMask设置为0
		LC_OS_CommandConfig(0x40, IoMask, 10, m_nDevID[0]);
		LC_OS_CommandConfig(0x40, IoMask, 10, m_nDevID[1]);
	}


	//这里对应你要设置的PIN。
	//m_PinMask[31:0] PIN31:0 enable or not. 1:enalbe
	//m_PinMaskH[44:32] PIN44:32 enable or not...
	ULONG m_PinMask = 0xffffffff;// pOSTab->PinMask_L;//0xffffffff;  //you can setup this...
	ULONG m_PinMaskH = 0x01fff;//pOSTab->PinMask_H;//0x01fff;

	//Here　is value...
	USHORT m_OpenStd = 0x1ff-50;  //0x400 equal to 3V...
	USHORT m_ShortStd = 50;

	int HighLimit[45] = {0};
	int LowLimit[45] = {0};
	int VoltageH[45] = {0};
	int VoltageL[45] = {0};

	//这里完成一个转换。其实这里可以每一个PIN都设立一个标准。。

	for(int i = 0; i < 45; i++)
	{
		HighLimit[i] = (int)(m_OpenStd *1.5f *1000000/512) ; //转变为uV信号	
		LowLimit[i] =  (int)(m_ShortStd *1.5f*1000000/512) ;
	}
	IoMask[0] = (UCHAR)((m_PinMask) & 0xff);
	IoMask[1] = (UCHAR)((m_PinMask>>8) & 0xff);
	IoMask[2] = (UCHAR)((m_PinMask>>16) & 0xff);
	IoMask[3] = (UCHAR)((m_PinMask>>24) & 0xff);

	IoMask[4] = (UCHAR)((m_PinMaskH) & 0xff);
	IoMask[5] = (UCHAR)((m_PinMaskH>>8) & 0xff);
	IoMask[6] = (UCHAR)((m_PinMaskH>>16) & 0xff);
	IoMask[7] = (UCHAR)((m_PinMaskH>>24) & 0xff);


	//dtkdemo.exe use 20*1000 uA for os test. so maybe some different.
	OS_Config((int)(1.5f*1000*1000), HighLimit, LowLimit, 45, 2*1000, 1*1000, m_nDevID[CamID]);	
	//OS 测试，进行该PIN为low的测试。
	//command bit7 :enalble os ; bit3:high test; bit2:low test...
	LC_OS_CommandConfig(0x80|0x08 | 0x04, IoMask, 10, m_nDevID[CamID]);
	//读取OS测试的结果
	int iPin = 0;
	CString stmp;
	int nRet = OS_Read(VoltageH, VoltageL, Result, 1, 1, 45 , m_nDevID[CamID]);
	bOpenShortFinish[CamID%2] = TRUE;
	if(nRet==1)
	{
		for(int i=0;i<45;i++)
		{
			if((VoltageL[i] != 0xffff*1000  ) /*&& Result[i]*/)
			{
				if (i < 20 || i > 36)	// 取MU950硬件上有的管脚
				{

					iPin++;
					stmp.Format(_T("%02d, Vol=%.3f (v)\n"), iPin, VoltageL[i]/1000000.0f);	
					/*pDlg->m_listOS.AddString(stmp);
					sOSInfo += stmp;
					*/
					stmp.Format(_T("%02d, Vol=%.3f (v)\n"), iPin, VoltageH[i]/1000000.0f);	
					/*pDlg->m_listOS.AddString(stmp);
					sOSInfo += stmp;


					*/
					//OSValueArray[i] = VoltageH[i];
				}
			}
		}
		for (int i=0; i<45; i++)
		{
			OSValueArray[i] = VoltageL[i];
		//	OSValueArray[i] = VoltageH[i];
		}
	}
	else
	{  
		//CString str;
		/*str.Format(_T("开短路错误%d，请重试"), nRet);
		AfxMessageBox(str);*/

		//重置状态，防止下次进来时读取错误
		bOpenShortFinish[0] = FALSE;//reset
		bOpenShortFinish[1] = FALSE;//reset
		return -3;
	} 

	int iTryTimes2 = 0;
	while((bOpenShortFinish[0] != TRUE || bOpenShortFinish[1] != TRUE) && iTryTimes2<60) //等待两个模组同时做完
	{
		Sleep(50);
		iTryTimes2++;
	}

	
	if (1)
	{
		// IoMask设置为0
		for(int i=0; i<10; i++)
		{
			IoMask[i] = 0;
		}
		// OS测试结束，连通测试板	（共地情况）
		
		LC_OS_CommandConfig(0x20, IoMask, 10, m_nDevID[0]);
		LC_OS_CommandConfig(0x20, IoMask, 10, m_nDevID[1]);
	}

	//重置状态，防止下次进来时读取错误
	bOpenShortFinish[0] = FALSE;//reset
	bOpenShortFinish[1] = FALSE;//reset

	return 1;
}
int Mud952::PmuGetStandyCurrent(int Current[],int iCount,int iDevID,int CamID)
{
// 	for (int i=0;i<iCount;i++)
// 	{
// 		Current[i]=iStandyCurrent[i];
// 	}

	//LING 增加双摄上传待机电流数据
	if (0 == CamID)
	{
		for (int i = 0; i < iCount; i++)
		{
			Current[i] = iStandyCurrent_Left[i];
		}
	}
	else
	{
		for (int i = 0; i < iCount; i++)
		{
			Current[i] = iStandyCurrent_Right[i];
		}
	}
	return 1;
}
int Mud952::PmuGetCurrent(int Current[],int iCount,int iDevID,int CamID)
{
	SENSOR_POWER Power[5];

	Power[0] = POWER_AVDD;
	Power[1] = POWER_DOVDD;
	Power[2] = POWER_DVDD;
	Power[3] = POWER_AFVCC;
	Power[4] = POWER_VPP; 
	return ::PmuGetCurrent( Power, Current, iCount, m_nDevID[CamID]);
}
int Mud952::PmuGetVoltage(int Voltage[],int iCount,int iDevID,int CamID)
{
	SENSOR_POWER Power[5];

	Power[0] = POWER_AVDD;
	Power[1] = POWER_DOVDD;
	Power[2] = POWER_DVDD;
	Power[3] = POWER_AFVCC;
	Power[4] = POWER_VPP;

	return ::PmuGetVoltage(Power,Voltage, iCount, m_nDevID[CamID]);
}

int Mud952::GetGrabSize()
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

int Mud952::SetSoftPin(int SensorPort ,int iDevID/* = 0 */)
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

int Mud952::InitCamera(int CamID)
{
	//first kill the data buffer...
	DeviceSensorTab CurrentCam = CamSensorTab[CamID];
	int devID = m_nDevID[CamID];
	//关闭IO上拉电阻
	SetSoftPinPullUp(IO_NOPULL, devID);
	/*first set sensor working condition....*/
	{
		//first set pin definition...
		{
			BYTE  pinDef[40] = {0};
			if(CurrentCam.port == PORT_MIPI || CurrentCam.port == PORT_HISPI)
			{
				pinDef[0] = 20;
				pinDef[1] = 0;
				pinDef[2] = 2;
				pinDef[3] = 1;
				pinDef[4] = 3;
				pinDef[5] = 4;
				pinDef[6] = 5;
				pinDef[7] = 6;
				pinDef[8] = 7;
				pinDef[9] = 8;
				pinDef[10] = 9;
				pinDef[11] = 20;
				pinDef[12] = 10;
				pinDef[13] = 11;
				pinDef[14] = 12;
				pinDef[15] = 20;
				pinDef[16] = 20;
				pinDef[17] = 13;
				pinDef[18] = 15;
				pinDef[19] = 14;
				pinDef[20] = 19;
				pinDef[21] = 18;
				pinDef[22] = 20;
				pinDef[23] = 16;
				pinDef[24] = 20;
				pinDef[25] = 20;

			}
			else  //standard parallel..
			{
				pinDef[0]=16;
				pinDef[1]=0;
				pinDef[2]=2;
				pinDef[3]=1;
				pinDef[4]=3;
				pinDef[5]=4;
				pinDef[6]=5;
				pinDef[7]=6;
				pinDef[8]=7;
				pinDef[9]=8;
				pinDef[10]=9;
				pinDef[11]=20;
				pinDef[12]=10;
				pinDef[13]=11;
				pinDef[14]=12;
				pinDef[15]=20;
				pinDef[16]=20;
				pinDef[17]=20;
				pinDef[18]=20;
				pinDef[19]=20;
				pinDef[20]=13;
				pinDef[21]=20;
				pinDef[22]=14;
				pinDef[23]=15;
				pinDef[24]=18;
				pinDef[25]=19;
			}
			//配置柔性接口
			::SetSoftPin(pinDef,devID);
		}

		//使能柔性接口
		EnableSoftPin(TRUE,devID);
		EnableGpio(TRUE,devID);
		//set voltage and mclk.....

		//设置电压，电流
		SENSOR_POWER Power[10];
		int Volt[10];
		int Current[10];
		BOOL OnOff[10];
		CURRENT_RANGE range[5];
		float fMclk;

		Power[0] = POWER_AVDD;
		Current[0] = 300; // 300mA
		OnOff[0] = TRUE;
		range[0] = CURRENT_RANGE_MA;

		Power[1] = POWER_DOVDD;
		Current[1] = 300; // 300mA
		OnOff[1] = TRUE;
		range[1] = CURRENT_RANGE_MA;	

		Power[2] = POWER_DVDD;
		Current[2] =  300;// 300mA
		OnOff[2] = TRUE;
		range[2] = CURRENT_RANGE_MA;

		Power[3] = POWER_AFVCC;
		Current[3] = 300; // 300mA
		OnOff[3] = TRUE;
		range[3] = CURRENT_RANGE_MA;

		Power[4] = POWER_VPP;
		Current[4] = 300; // 300mA
		OnOff[4] = FALSE;
		range[4] = CURRENT_RANGE_MA;

		//if (byChannel == CHANNEL_A)
		{
			Volt[0] = (int)(CurrentCam.avdd * 100); 
			Volt[1] = (int)(CurrentCam.dovdd * 100); 
			Volt[2] = (int)(CurrentCam.dvdd * 100); 
			Volt[3] = (int)(2.8 * 1000); 
			Volt[4] = (int)(2.8 * 1000); 
			fMclk = CurrentCam.mclk*10;
		}



		//设置5路电压值
		if (PmuSetVoltage(Power,Volt,5,devID) != DT_ERROR_OK)
		{
			//msg("Set Voltage Failed!");
			return DT_ERROR_FAILED;
		}

		//设置电压开关
		if (PmuSetOnOff(Power,OnOff,5,devID) != DT_ERROR_OK)
		{
			//msg("Open Voltage Failed!");
			return DT_ERROR_FAILED;
		}

		// 设置量程
		if (PmuSetCurrentRange(Power,range,5,devID) != DT_ERROR_OK)
		{
			//msg("Set Range Failed!");
		}

		//设置电流
		if (PmuSetOcpCurrentLimit(Power,Current,5,devID) != DT_ERROR_OK)
		{
			//msg("Set OcpCurrentLimit Failed!");
		}


		if (SetSensorClock(TRUE,(USHORT)(fMclk*10), devID) != DT_ERROR_OK)
		{
			//AfxMessageBox("Set Mclk Failed!");
			return DT_ERROR_FAILED;
		}

		//开启IO上拉电阻
		SetSoftPinPullUp(IO_PULLUP, devID);
	}
	Sleep(10);
	//i2C init....

	//设置SENSOR I2C的速率为400Kbps,允许从设备为Streching mode（从设备端可以拉低scl和sda电平来表示busy）
	SetSensorI2cRate(I2C_400K, devID);
	SetSensorI2cRapid(TRUE, devID);

	//check sensor is on line or not ,if on line,init sensor to work....
	SensorEnable(CurrentCam.pin ^ 0x02, 1, CamID); //reset
	Sleep(20);
	SensorEnable(CurrentCam.pin, 1, CamID);
	Sleep(50);

	//check sensoris on line...
	//if(SensorIsMe(&CurrentCam, CHANNEL_A, 0,m_nDevID) != DT_ERROR_OK)
	//{
	//	//AfxMessageBox("Sensor is not ok!");
	//	return DT_ERROR_FAILED;			
	//}
	//init sensor....
	if(InitSensor(CurrentCam.SlaveID,
		CurrentCam.ParaList,
		CurrentCam.ParaListSize,
		CurrentCam.mode,devID) != DT_ERROR_OK)
	{
		//AfxMessageBox("Init Sensor Failed!");
		return DT_ERROR_FAILED;			
	}

	if(CurrentCam.type == D_YUV || CurrentCam.type == D_YUV_SPI || CurrentCam.type == D_YUV_MTK_S)
		SetYUV422Format(CurrentCam.outformat, devID);
	else
		SetRawFormat(CurrentCam.outformat, devID);

	//if (byChannel == CHANNEL_A)
	//{
	//	m_PreviewWidthA = CurrentCam.width;
	//	m_PreviewHeightA = CurrentCam.height;//& 0xfffe;
	//}
	//
	//if (byChannel == CHANNEL_B)
	//{
	//	m_PreviewWidthB = CurrentCam.width;
	//	m_PreviewHeightB = CurrentCam.height;//& 0xfffe;
	//}

	USHORT roi_x0 = CurrentCam.width >> 2;
	USHORT roi_y0 = CurrentCam.height >> 2;
	USHORT roi_hb = 0;
	USHORT roi_vb = 0;
	USHORT roi_hnum = 1;
	USHORT roi_vnum = 1;

	USHORT fifo_div = 2;	
	roi_x0 = 0;
	roi_y0 = 0;
	roi_hb = 0;
	roi_vb = 0;
	roi_hnum = 1;
	roi_vnum = 1;
	//初始化设备
	InitRoi(0, 0, CurrentCam.width, CurrentCam.height , 0, 0, 1, 1, CurrentCam.type, TRUE, devID);	//以像素为单位，全分辨率显示关闭ROI使能
	SetSensorPort(CurrentCam.port, CurrentCam.width, CurrentCam.height, devID);
	return DT_ERROR_OK;
}
BOOL Mud952::InitiaSensor(int CamID,CString SettingPath)
{
	if (SettingPath.GetLength()>4)
	{
		;//添加New 点亮设定代码
	}
	DeviceSensorTab CurrentCam = CamSensorTab[CamID];
	/* 设置reset pin,pwdn pin*/
	int DevID=m_nDevID[CamID];
	::CloseVideo(DevID);

	if (SetSensorClock(TRUE,(USHORT)(CurrentCam.mclk *10),DevID) != DT_ERROR_OK) // 20MHz
	{
		AddLog(CamID,L"设置量设置SENSOR时钟失败程失败!",COLOR_RED);  
		return DT_ERROR_FAILED;
	}

	::SensorEnable(0, 1,m_nDevID[CamID]); 
	Sleep(20); 
	::SensorEnable(3, 1,m_nDevID[CamID]); 
	Sleep(50);
	int Flag = InitSensor(CurrentCam.SlaveID,CurrentCam.ParaList,CurrentCam.ParaListSize,CurrentCam.mode,DevID);
	if ( Flag != DT_ERROR_OK)
	{
		AddLog(CamID,_T("sensor初始化失败"),COLOR_RED);
		return DT_ERROR_FAILED;
	} 

	Sleep(100);

	if (CamSensorTab[CamID].type == D_YUV || CamSensorTab[CamID].type == D_YUV_MTK_S || CamSensorTab[CamID].type == D_YUV_SPI)
		SetYUV422Format(CurrentCam.outformat,DevID);
	else
		SetRawFormat(CurrentCam.outformat,DevID);

	/* 获得图像数据 */
	Size = GetByteOffset(CurrentCam.width,CamSensorTab[CamID].type,DevID)*CurrentCam.height;


	USHORT roi_x0 = CurrentCam.width >> 2;
	USHORT roi_y0 = CurrentCam.height >> 2;
	USHORT roi_hb = 0;
	USHORT roi_vb = 0;
	USHORT roi_hnum = 1;
	USHORT roi_vnum = 1;

	USHORT fifo_div = 2;	
	roi_x0 = 0;
	roi_y0 = 0;
	roi_hb = 0;
	roi_vb = 0;
	roi_hnum = 1;
	roi_vnum = 1;
	//初始化设备
	InitRoi(0+CurrentCam.iXRoi, 0+CurrentCam.iYRoi, CurrentCam.width, CurrentCam.height , 0, 0, 1, 1, CurrentCam.type, TRUE, DevID);	//以像素为单位，全分辨率显示关闭ROI使能
	SetSensorPort(CurrentCam.port, CurrentCam.width, CurrentCam.height, DevID);

	if (InitDisplay(GetCamDisplayHwnd(CamID),CurrentCam.width,CurrentCam.height,CamSensorTab[CamID].type,CHANNEL_A,NULL,DevID) != DT_ERROR_OK)
	{

		return DT_ERROR_FAILED;
	}
	InitIsp(CurrentCam.width, CurrentCam.height, CurrentCam.type, CHANNEL_A , DevID);

	// 计算A通道实际采集数据量
	CalculateGrabSize(&uGrabSize,DevID);  
	// 打开视频
	::OpenVideo(uGrabSize,DevID); 
	// 设置要抓取的通道

	return TRUE;
}