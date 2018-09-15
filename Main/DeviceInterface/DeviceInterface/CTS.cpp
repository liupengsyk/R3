#include "StdAfx.h"
#include "CTS.h"
//#include "CTS_SDK/cts.h"

#pragma comment(lib,"CTS_SDK/ctscam.lib")


#define CURRENT_LARGERANGE 	0
#define CURRENT_MIDDLERANGE 	1
#define CURRENT_SMALLRANGE 	2
#define CURRENTRANGENUM 		3

#define CURRENT_CHANGERANGE_DELAY 0
#define CURRENT_READCONTINUE_DELAY 1

CCTS::CCTS(void)
{
	CTS_CamID=0;
	bCamInit=0;
	m_bDualCamMode = 0;
	DelayTime=3000;
}


CCTS::~CCTS(void)
{
	CTSSensor.vecRegSettings.clear();
	if (bCamInit)
	{
		Cam_UnInit();
		CloseDevice(CTS_CamID);
		StopPreview(CTS_CamID);
	}
}

 vector<CString> CCTS::GetDevList()
 {
	 vector<CString> DevNameList;
	 if (bCamInit==0)
	 {
		 bCamInit = Cam_Init();
	 }
	 
	 if(bCamInit==0) return DevNameList;
	 
	 DWORD nDevCount = 0;
	 DEVINFO	stDevInfo[DEVNUM];

	 memset(stDevInfo, 0, sizeof(DEVINFO) * DEVNUM);
	 BYTE byCamMode = 1; //2: check dual cam, 0: single cam or normal multi cam; 1: array cam or ordered-multi cam

	 BOOL  bRes= Cam_EnumAllDevInfo(stDevInfo, nDevCount, byCamMode);

	 m_bDualCamMode = FALSE;
	 if(bRes > 0)
	 {
		 CString str;
		 CString str1;
		 for(int i=0;i<nDevCount;i++)
		 {
			 str.Format(_T("%08x"),stDevInfo[i].nDevID);
			 str1.Format(_T("%08x"),stDevInfo[i].nHWVer);
			 str=str+_T("_")+str1;
			 DevNameList.push_back(str);

		 }
		 if(bRes == 2) //dual cam
		 {
			m_bDualCamMode = TRUE;
		 }
		 else
		 {
		 }
	 }
	 else
	 {
		 return DevNameList;
	 }

	 return DevNameList;
 }
 int  CCTS::GetKitTypeDeviceInfo(int CamID)
 {
	 if (devName.GetLength()<3)
	 {
		 CString str;
		 str.Format(_T("没有指定设备:%s，请重新设定设备"),devName);
		 AddLog(CamID,str,COLOR_RED,200); 
		 return -1;
	 }
	 SetCamTab(CamID);
	 if (bCamInit==FALSE)
	 {
		 if (Cam_Init())
		 {
			 bCamInit=TRUE;
		 }
		 else
		 {
			 AddLog(CamID,_T("设备初始化失败！"),COLOR_RED,200);
			 return -1;
		 }
	 }


	 if(!m_MyCTS[CamID].bSensorInit)
	 {
		 int index=devName.Find(_T("_"));
		 CString CurrentSN=devName.Left(index);
		 devName=devName.Mid(index+1);

		 int dev;
		 swscanf(CurrentSN, _T("%x"), &m_MyCTS[CamID].devInst);

		 DWORD nRes = Cam_SetConfigExFromAPIDT(m_MyCTS[CamID].devInst);

		 nRes = (nRes == 0);
		 if(nRes == 1)
		 {
			 m_MyCTS[CamID].bSensorInit = 1;
			 nRes &= StartPreview(CamID);
		 }
		 else
			 m_MyCTS[CamID].bSensorInit = 0;

		 return nRes;
	 }
	 else
		 return 1;

 }

 int  CCTS::OpenDevice(CString DeviceName)
 {

	 return 1;
 }

 int  CCTS::CloseDevice(int CamID)
 {
	 BOOL bRes = 0;
	 if(m_MyCTS[CamID].bVideoPlay)
	 {
		 bRes = StopPlay(CamID); // stop lay

		// StopPreview(CamID);
		 
	 }

	 return bRes;
 }

 int  CCTS::OpenVideo(int CamID)
 {
	 BOOL bRes = 1;

	 if(!m_MyCTS[CamID].bBuildPreview)
	 {
		 bRes &= StartPreview(CamID);
	 }

	 if(m_MyCTS[CamID].bBuildPreview)
	 {
		 bRes &= StartPlay(CamID);
	 }

	 return bRes;
 }

 int  CCTS::CloseVideo(int CamID)
 {
	 BOOL bRes;
	 bRes = StopPlay(CamID);

	 return bRes;
 }


 BOOL CCTS::StartPreview(int CamID)
 {
	 BOOL bRes;

	 HWND hwndPreview=NULL;
	 DWORD devInst = m_MyCTS[CamID].devInst;

	 WORD nImageWidth = Cam_GetSensorOutputWidth(devInst); //;
	 WORD nImageHeight = Cam_GetSensorOutputHeight(devInst);//;

	 hwndPreview=GetCamDisplayHwnd(CamID);
	 BYTE dataformat = 0; // 0:RAW8,1: YUV422, 2:MJPG, 3:MIPIRAW10,4:DVPRAW10,5:MIPIRAW12			
	 BYTE displayformat = 1; //0: RAW8, 1:RGB24
	 BYTE BayerOrder = 1; // 0:RG_GB, 1:GR_BG, 2:GB_RG, 3:BG_GR

	 dataformat = Cam_GetSensorDataFormat(devInst);
	 BayerOrder = Cam_GetSensorPixelOrder(devInst);

	 bRes = Cam_BuildPreviewGraph(devInst, hwndPreview, nImageWidth, nImageHeight, dataformat, displayformat, BayerOrder);

	 if(bRes == 0)
		 m_MyCTS[CamID].bBuildPreview = 1;
	 else
		 m_MyCTS[CamID].bBuildPreview = 0;

	 return m_MyCTS[CamID].bBuildPreview;
 }

 void CCTS::StopPreview(int CamID)
 {	
	 Cam_StopPreviewGraph(m_MyCTS[CamID].devInst); //stop preview
	 m_MyCTS[CamID].bBuildPreview = 0;
 }

 BOOL CCTS::StartPlay(int CamID)
 {
	 BOOL bRes = 0;
	 if(m_MyCTS[CamID].bBuildPreview == 1)
	 {
		 bRes = Cam_MediaControl(m_MyCTS[CamID].devInst, MEDIACTRL_RUN); //run

		 m_MyCTS[CamID].bVideoPlay = bRes;
	 }

	 return bRes;
 }

 BOOL CCTS::StopPlay(int CamID)
 {
	 BOOL bRes = 0;	

	 if(m_MyCTS[CamID].bBuildPreview == 1)
	 {
		 bRes = Cam_MediaControl(m_MyCTS[CamID].devInst, MEDIACTRL_STOP); //stop
		 m_MyCTS[CamID].bVideoPlay = 0;
	 }
	 return bRes;

 }
// 
//  int  CCTS::GetKitTypeDeviceInfo(int CamID)
//  {
// 
// 	 if (devName.GetLength()<3)
// 	{
// 		CString str;
// 		str.Format(_T("没有指定设备:%s，请重新设定设备"),devName);
// 		AddLog(CamID,str,COLOR_RED,200); 
// 		return -1;
// 	}
// 	if (m_MyCTS[CamID].bCamInit==FALSE)
// 	{
// 		SetCamTab(CamID);
// 		if (Cam_Init())
// 		{
// 			m_MyCTS[CamID].bCamInit=TRUE;
// 		}
// 		else
// 		{
// 			AddLog(CamID,_T("设备初始化失败！"),COLOR_RED,200);
// 			return -1;
// 		}
// 	}
// 
// 	
// 	if(!m_MyCTS[CamID].bSensorInit)
// 	{
// 		int index=devName.Find(_T("_"));
// 		CString CurrentSN=devName.Left(index);
// 		devName=devName.Mid(index+1);
// 
// 		int dev;
// 		swscanf(CurrentSN, _T("%x"), &dev);
// 
// 		DWORD devInst;
// 		devInst = dev;
// 		m_MyCTS[CamID].devInst = dev; 
// 		DWORD nRes = Cam_SetConfigExFromAPIDT(devInst);
// 
// 		if(nRes == 0)//成功
// 		{
// 			m_MyCTS[CamID].bSensorInit = 1;
// 
// 			HWND hwnd = NULL;
// 
// 			WORD nImageWidth = Cam_GetSensorOutputWidth(devInst); //;
// 			WORD nImageHeight = Cam_GetSensorOutputHeight(devInst);//;
// 
// 			BYTE dataformat = 3; // 0:RAW8,1: YUV422, 2:MJPG, 3:MIPIRAW10,4:DVPRAW10,5:MIPIRAW12	
// 			dataformat=Cam_GetSensorDataFormat(devInst);
// 			BYTE displayformat = 1; //0: RAW8, 1:RGB24
// 			BYTE BayerOrder = CamSensorTab[CamID].outformat; // 0:RG_GB, 1:GR_BG, 2:GB_RG, 3:BG_GR
// 			
// 			nRes = Cam_BuildPreviewGraph(devInst, hwnd, nImageWidth, nImageHeight, dataformat, displayformat, BayerOrder);
// 
// 			if(nRes == 0)//成功
// 				m_MyCTS[CamID].bBuildPreview = 1;
// 			else
// 				m_MyCTS[CamID].bBuildPreview = 0;
// 			
// 		}
// 		else
// 			m_MyCTS[CamID].bSensorInit = 0;
// 		
// 		return m_MyCTS[CamID].bSensorInit ;
// 	}
// 	else
// 		return 1;
// 	
// 	
// 	
//  }
// 
//  int  CCTS::OpenDevice(CString DeviceName)
//  {
// 	 return 1;
//  }
// 
//  int  CCTS::CloseDevice(int CamID)
//  {
// 	 BOOL bRes;
// 	if(m_MyCTS[CamID].bBuildPreview)
// 	{
// 		bRes = Cam_MediaControl(m_MyCTS[CamID].devInst, MEDIACTRL_STOP); //stop
// 		m_MyCTS[CamID].bVideoPlay = 0;
// 	}
// 	else
// 		bRes = 0;
// 
// 	return bRes;
//  }
// 
//  int  CCTS::OpenVideo(int CamID)
//  {
//  	BOOL bRes;
// 	if(m_MyCTS[CamID].bBuildPreview)
// 	{
// 		bRes = Cam_MediaControl(m_MyCTS[CamID].devInst, MEDIACTRL_RUN); //run
// 		m_MyCTS[CamID].bVideoPlay = bRes;
// 	}
// 	else
// 		bRes = 0;
// 
// 	return bRes;
//  }
int CCTS::PmuGetStandyCurrent(int Current[],int iCount,int iDevID,int CamID)
{
	return 1;
}
 int CCTS::PmuGetCurrent(int Current[],int iCount,int iDevID,int CamID)
 {
	 BOOL bRes;
	 BOOL bAllValid = TRUE;
	 BOOL bCur[PWRNUM];
	 BYTE byValid[PWRNUM];
	 int nValue[PWRNUM];
	 BYTE prerange;
	 bRes &= Cam_GetCurrentRange(m_MyCTS[CamID].devInst, prerange);
	 if(bRes && prerange != CURRENT_LARGERANGE)
	 {
		 bRes &= Cam_SetCurrentRange(m_MyCTS[CamID].devInst, CURRENT_LARGERANGE);
		 //Sleep(CURRENT_CHANGERANGE_DELAY);
	 }
	 memset(bCur, 0, sizeof(BOOL) * PWRNUM);
	 memset(byValid, 0, sizeof(BYTE) * PWRNUM);
	 memset(nValue, 0, sizeof(int) * PWRNUM);

	 bCur[0] = 1; //DVDD
	 bCur[1] = 1; //AVDD
	 bCur[2] = 1; //DOVDD

	 if(iCount > 3)
	 {
		 bCur[3] = 1; // AFVDD
	 }

	 bRes = Cam_GetPowerCurrentEx(m_MyCTS[CamID].devInst, bCur, byValid, nValue);

	 if(bRes)
	 {
			 for(int i = 0; i < iCount; i++)
			  {
				  if(byValid[i])
				  {
					  if(i==0)
				    Current[2] = nValue[i]; // nA
					  if(i==1)
					Current[0] = nValue[i]; // nA
					  if(i==2)
					Current[1] = nValue[i]; // nA
					  if(i==3)
					Current[3] = nValue[i]; // nA

				  }
			 }
	 }
	 return bRes && bAllValid;
 }
 int CCTS::PmuGetVoltage(int Voltage[],int iCount,int iDevID,int CamID)
 {
	 BOOL bCur[PWRNUM];
	 int nValue[PWRNUM];
	 memset(bCur, 0, sizeof(BOOL) * PWRNUM);
	 memset(nValue, 0, sizeof(int) * PWRNUM);

	 bCur[0] = 1; //DVDD
	 bCur[1] = 1; //AVDD
	 bCur[2] = 1; //DOVDD

	 if(iCount > 3)
	 {
		 bCur[3] = 1; // AFVDD
	 }
	 BOOL bRes = Cam_GetPowerVoltage(m_MyCTS[CamID].devInst, bCur, nValue);

	 float pucDOVDD, pucAVDD,pucDVDD,pucAF,pucVFuse;

	 pucDVDD = nValue[0];
	 pucAVDD = nValue[1];
	 pucDOVDD = nValue[2];
	 pucAF = nValue[3];
	 pucVFuse = 0;

	 //int iresult	=classPlatform[CamID].HisFX3GetVolt(&pucDOVDD,&pucAVDD,&pucDVDD,&pucAF,&pucVFuse);
	 Voltage[0]=pucAVDD;
	 Voltage[1]=pucDOVDD;
	 Voltage[2]=pucDVDD;
	 Voltage[3]=pucAF;
	 Voltage[4]=pucVFuse;
	 return 1;
 }

 int CCTS::OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID)
 {
	 BOOL bRes;
	 LOGINFO OsLogInfo;
	 bRes = OS_Test(CamID, OsLogInfo);

	 if(CamSensorTab[CamID].port == 0)	
		 OS_CTSToDT(OSValueArray, OsLogInfo);//MIPI
	 else
		 OS_CTSToDT_DVP(OSValueArray, OsLogInfo);//DVP

	 return bRes;
 }

 int CCTS::GetGrabSize()
 {
	 int Grabsize=0;
	 switch(CamSensorTab[0].type)
	 {
	 case 1:
		 Grabsize=CamSensorTab[0].width*CamSensorTab[0].height;
		 break;
	 case 0:
		 Grabsize=CamSensorTab[0].width*CamSensorTab[0].height*2;
		 break;
	 }

	 return Grabsize;
 }
 int  CCTS::GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int CamID)
 {
 	BOOL bRes;
 	long bufsize = 0;
 	bRes = Cam_GrabFrameOrg(m_MyCTS[CamID].devInst, pInBuffer, &bufsize);
	
	return bRes;
 }

 int CCTS::WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID,int CamID)
 {
	 BOOL bRes;
	 BYTE byCTSMode=0;
	 int count = 0;
	 BYTE data[2];
	 if (byMode>=1)
	 {
		 byCTSMode=byMode-1;
	 }
	 if(byCTSMode == 1 || byCTSMode == 3)
	{
		data[0] = (uValue >> 8) & 0xff;
		data[1] = (uValue >> 0) & 0xff;
		count = 2;
	 }
	 else
	 {
		data[0] = (uValue >> 0) & 0xff;
		count = 1;
	 }	
	 


	 bRes = Cam_WriteNormalI2CEx(m_MyCTS[CamID].devInst, uAddr, uReg, count, data, byCTSMode);

	 return bRes;
 }
 int CCTS::ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID,int CamID)
 {
	 BOOL bRes;
	 BYTE byCTSMode=0;
	 int count = 0;
	 BYTE data[2] = {0};
	 if (byMode>=1)
	 {
		 byCTSMode=byMode-1;
	 }
	 if(byCTSMode == 1 || byCTSMode == 3)
	 {
		 count = 2;
	 }
	 else
	 {
		 count = 1;
	 }	

	 bRes = Cam_ReadNormalI2CEx(m_MyCTS[CamID].devInst, uAddr, uReg, count, data, byCTSMode);

	 if(byCTSMode == 1 || byCTSMode == 3)
	 {
		*pValue = (data[0] << 8) + data[1];
	 }
	 else
	 {
		 *pValue = data[0];
	 }	

	 return bRes;
 }

 int  CCTS::WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID,int CamID)
 {
	 BOOL bRes=TRUE;
	 BYTE byMode = 0;
	 if(uRegAddrSize == 1)
		 byMode = 0;
	 else if(uRegAddrSize == 2)
		 byMode = 2;
//	bRes &= Cam_WriteNormalI2CExWithDelay(m_MyCTS[CamID].devInst, uDevAddr, uRegAddr, uSize, pData, byMode,DelayTime);
	bRes &= Cam_WriteNormalI2CExWithDelay(m_MyCTS[CamID].devInst, uDevAddr, uRegAddr, uSize, pData, byMode,32, DelayTime, 10000);


	 return bRes;
 }
 int  CCTS::ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop,int iDevID,int CamID)
 {
	 BOOL bRes;
	 BYTE byMode = 0;
	 if(uRegAddrSize == 1)
		 byMode = 0;
	 else if(uRegAddrSize == 2)
		 byMode = 2;
	 bRes = Cam_ReadNormalI2CEx(m_MyCTS[CamID].devInst, uDevAddr, uRegAddr, uSize, pData, byMode);
	 return bRes;
 }

 void CCTS::SetCamTab(int CamID)
 {
	  CTS_CamID=CamID;
	 LoadSensorGenaralTab(CamID); 
	 LoadSensorParalist(CamID); 
	 LoadSensorSetting(CamID);

	 ParseDTINItoCTSStruct(CamID);
 }

 void CCTS::SetGpioPinLevel(int gpioNum,BOOL bH,int CamID)
 {

 }
 void CCTS::SensorEnable(int pin,BOOL bH,int CamID)
 {

 }
 BOOL CCTS::InitiaSensor(int CamID,CString SettingPath)
 {
	 if (CloseVideo(CamID)!=1)
	 {
		 AddLog(CamID,_T("初始化失败:关闭失败！"),COLOR_RED,200);
		 return 0;
	 }
	 Sleep(10);
	 if (OpenVideo(CamID)!=1)
	 {
		 AddLog(CamID,_T("初始化失败：点亮失败！"),COLOR_RED,200);
		 return 0;
	 }
	 
	 
	 return 1;
 }
 BOOL CCTS::LoadSensorSetting(int CamID)
 {
	 strAppName=_T("CTS");
	 //cts parameters
	 DelayTime=ReadConfigData(_T("DelayTime"), 3000, CamID); 
	 CTSSensor.OPERATION_SEQUENCE	 =ReadConfigData(_T("OPERATION_SEQUENCE"), 0, CamID); 
	 CTSSensor.SEQUENCE_DELAY		 = ReadConfigData(_T("SEQUENCE_DELAY"), 0, CamID);
	 //CString str = ReadConfigString(_T("SENSOR_IO_LEVEL"), _T("0"), CamID);

	 //swscanf(str, _T("%f"), &CTSSensor.SENSOR_IO_LEVEL);	

	 CTSSensor.SENSOR_IO_LEVEL = 1.8;
	 
	 CTSSensor.I2C_SPEED			 = ReadConfigData(_T("I2C_SPEED"), 0, CamID);
	 CTSSensor.SENSOR_FRAME_RATE	 = ReadConfigData(_T("SENSOR_FRAME_RATE"), 0, CamID);
	 CTSSensor.USB3_BANDWIDTH		 = ReadConfigData(_T("USB3_BANDWIDTH"), 0, CamID);
		 
	 CTSSensor.DATA_PATH			 = ReadConfigData(_T("DATA_PATH"), 0, CamID);
	 CTSSensor.SENSOR_OUTPUT_FORMAT  = ReadConfigData(_T("SENSOR_OUTPUT_FORMAT"), 0, CamID);
	 CTSSensor.MIPI_LANE_NUMBER	 	 = ReadConfigData(_T("MIPI_LANE_NUMBER"),0, CamID);
	 CTSSensor.MIPI_T_hs_zero_MASK   = ReadConfigData(_T("MIPI_T_hs_zero_MASK"), 0, CamID);
	 strAppName=_T("sensor");
	 return 1;
 }

 
 BOOL CCTS::ParseDTINItoCTSStruct(int CamID)
 {
	 BOOL bRes = TRUE;

	 //pwr
	 CTSSensor.DVDD = CamSensorTab[CamID].dvdd / 10.0;
	 CTSSensor.AVDD = CamSensorTab[CamID].avdd / 10.0;
	 CTSSensor.DOVDD = CamSensorTab[CamID].dovdd / 10.0;
	 CTSSensor.AFVDD = CamSensorTab[CamID].af_vdd / 10.0;
 
	 CTSSensor.SENSOR_POWERDOWN_ACTIVE	 = !(CamSensorTab[CamID].pin & 0x01);
	 CTSSensor.SENSOR_RESET_ACTIVE		 = !((CamSensorTab[CamID].pin >> 1) & 0x01);

	 if(CamSensorTab[CamID].mode <= 1)
	 {
		 CTSSensor.SENSOR_REG_ADDR_WIDTH = 1;
		 CTSSensor.SENSOR_REG_DATA_WIDTH = 1;
	 }
	 else if(CamSensorTab[CamID].mode == 2)
	 {
		 CTSSensor.SENSOR_REG_ADDR_WIDTH = 1;
		 CTSSensor.SENSOR_REG_DATA_WIDTH = 2;
	 }
	 else if(CamSensorTab[CamID].mode == 3)
	 {
		 CTSSensor.SENSOR_REG_ADDR_WIDTH = 2;
		 CTSSensor.SENSOR_REG_DATA_WIDTH = 1;
	 }
	 else if(CamSensorTab[CamID].mode == 4)
	 {
		 CTSSensor.SENSOR_REG_ADDR_WIDTH = 2;
		 CTSSensor.SENSOR_REG_DATA_WIDTH = 2;
	 }

	 CTSSensor.SENSOR_EXTCLK			 = CamSensorTab[CamID].mclk;
	 CTSSensor.SENSOR_OUTPUT_WIDTH		 = CamSensorTab[CamID].width;
	 CTSSensor.SENSOR_OUTPUT_HEIGHT 	 = CamSensorTab[CamID].height;

	 CTSSensor.SENSOR_I2C_ID				= CamSensorTab[CamID].SlaveID;
	 
	 CTSSensor.BAYER_PIX_ORDER			 = CamSensorTab[CamID].outformat;
 
	 CTSSensor.DSP_SAMPLE_START_PIX_X	 = 0; //reserved
	 CTSSensor.DSP_SAMPLE_START_PIX_Y	 = 0; //reserved
	 CTSSensor.DSP_OUTPUT_WIDTH 		 = 0; //reserved
	 CTSSensor.DSP_OUTPUT_HEIGHT		 = 0; //reserved
	 
	 CTSSensor.YUV_PIX_ORDER			 = 0; //reserved
	 CTSSensor.SENSOR_PCLK_SAMPLE_EDGE	 = 0; //reserved
 
	 CTSSensor.SENSOR_HSYNC_POLARITY	 = 0; //reserved
	 CTSSensor.SENSOR_VSYNC_POLARITY	 = 0; //reserved
 
	 CTSSensor.MIPI_AUTO_SKEW_CALIBRATION	 = 0; //reserved
	 CTSSensor.MIPI_CLK_DATA0_SKEW_CALIBRATION	 = 0xf; //reserved
	 CTSSensor.MIPI_CLK_DATA1_SKEW_CALIBRATION	 = 0xf; //reserved
	 CTSSensor.MIPI_CLK_DATA2_SKEW_CALIBRATION	 = 0xf; //reserved
	 CTSSensor.MIPI_CLK_DATA3_SKEW_CALIBRATION	 = 0xf; //reserved
 
	 tagREG reg;
	 CTSSensor.vecRegSettings.clear();
	 for(int i = 0; i < CamSensorTab[CamID].ParaListSize; i += 2)
	 {
		 reg.addr = CamSensorTab[CamID].ParaList[i + 0];
		 reg.data = CamSensorTab[CamID].ParaList[i + 1];
 
		 CTSSensor.vecRegSettings.push_back(reg);
	 }
 
 
	 return TRUE;
 
 }

 
 
 DWORD CCTS::Cam_SetConfigExFromAPIDT(DWORD devInst)
 {
 	BOOL bRes = TRUE;
	 // parse parameter from ini file

	 float fValue;
	 WORD wValue, wData[2];
	 DWORD dwValue;  
	 BYTE data[5];
	 int datapath;
	 float fPwr[6];
	 BYTE byAddrWidth, byDataWidth;
 

 
	 // step 1
	 //init all first
	 bRes &= Cam_SetIniDefault(devInst);
 
	 //step 2
 
	 //step 2.1
	 //[SENSOR_POWER&SEQUENCE]	 
	 fPwr[0] = CTSSensor.DVDD;
	 fPwr[1] = CTSSensor.AVDD;
	 fPwr[2] = CTSSensor.DOVDD;
	 fPwr[3] = CTSSensor.AFVDD;
	 fPwr[4] = CTSSensor.PWR4;
	 fPwr[5] = CTSSensor.PWR5;
	 bRes &= Cam_SetIniPower(devInst, fPwr[0], fPwr[1], fPwr[2], fPwr[3], fPwr[4], fPwr[5]);
	 
	 data[0] = CTSSensor.SENSOR_POWERDOWN_ACTIVE;
	 data[1] = CTSSensor.SENSOR_RESET_ACTIVE;
	 bRes &= Cam_SetIniSnrPwdnResetActive(devInst, data[0], data[1]);
 
	 dwValue = CTSSensor.OPERATION_SEQUENCE;
	 bRes &= Cam_SetIniOperationSequence(devInst, dwValue);
		 
	 data[0] = CTSSensor.SEQUENCE_DELAY;
	 bRes &= Cam_SetIniSequenceDelay(devInst, data[0]);
 
	 fValue = CTSSensor.SENSOR_IO_LEVEL;
	 bRes &= Cam_SetIniSnrIoLevel(devInst, fValue);
 
	 //[SENSOR_I2C]
	 data[0] = CTSSensor.I2C_SPEED;
	 bRes &= Cam_SetIniI2cSpeed(devInst, data[0]);
 
	 data[0] = CTSSensor.SENSOR_I2C_ID;
	 bRes &= Cam_SetIniSnrI2cId(devInst, data[0]);
	 
	 data[0] = CTSSensor.SENSOR_REG_ADDR_WIDTH;
	 data[1] = CTSSensor.SENSOR_REG_DATA_WIDTH;
	 bRes &= Cam_SetIniSnrRegAddrDataWidth(devInst, data[0], data[1]);
	 byAddrWidth = data[0];
	 byDataWidth = data[1];
 
	 //[SENSOR_PARAMETER]
	 fValue = CTSSensor.SENSOR_FRAME_RATE;
	 bRes &= Cam_SetIniSnrFps(devInst, fValue);
 
	 fValue = CTSSensor.SENSOR_EXTCLK;
	 bRes &= Cam_SetIniSnrExtClk(devInst, fValue);
		 
	 dwValue = CTSSensor.SENSOR_OUTPUT_WIDTH;
	 bRes &= Cam_SetIniSnrOutputWidth(devInst, dwValue);
	 dwValue = CTSSensor.SENSOR_OUTPUT_HEIGHT;
	 bRes &= Cam_SetIniSnrOutputHeight(devInst, dwValue);
 
	 //[USB_PARAMETER]
	 data[0] = CTSSensor.USB3_BANDWIDTH;
	 bRes &= Cam_SetIniUsb3Bandwidth(devInst, data[0]);
 
	 //[DATA_PATH_SELECTION]
	 datapath = CTSSensor.DATA_PATH;
	 /*   path: SensorOutput   SystemOutput
			1	 RAW			 YUV422 	 ==>MIPI
			2	 RAW			 RAW		 ==>MIPI
			3	 YUV422 		 YUV422 	 ==>MIPI
			4	 MJPG			 MJPG		 ==>MIPI
			
			5	 RAW			 YUV422 	 ==>DVP
			6	 RAW			 RAW		 ==>DVP
			7	 YUV422 		 YUV422 	 ==>DVP
			8	 MJPG			 MJPG		 ==>DVP
	 */
	 
	 data[0] = CTSSensor.SENSOR_OUTPUT_FORMAT;
	 bRes &= Cam_SetIniSnrOutputFormat(devInst, data[0]);
 
	 data[0] = CTSSensor.MIPI_LANE_NUMBER;
	 bRes &= Cam_SetIniMipiLaneNumber(devInst, data[0]);
 
	 data[0] = CTSSensor.MIPI_T_hs_zero_MASK;
	 bRes &= Cam_SetIniMipiTHsZeroMask(devInst, data[0]);
 
	 data[0] = CTSSensor.BAYER_PIX_ORDER;
	 bRes &= Cam_SetIniBayerPixOrder(devInst, data[0]);
 
	 data[0] = CTSSensor.DSP_SAMPLE_START_PIX_X;
	 bRes &= Cam_SetIniDspSampleStartPixX(devInst, data[0]);
 
	 data[0] = CTSSensor.DSP_SAMPLE_START_PIX_Y;
	 bRes &= Cam_SetIniDspSampleStartPixY(devInst, data[0]);
 
	 wValue = CTSSensor.DSP_OUTPUT_WIDTH;
	 bRes &= Cam_SetIniDspOutputWidth(devInst, wValue);
 
	 wValue = CTSSensor.DSP_OUTPUT_HEIGHT;
	 bRes &= Cam_SetIniDspOutputHeight(devInst, wValue);
		 
	 data[0] = CTSSensor.YUV_PIX_ORDER;
	 bRes &= Cam_SetIniYuvPixOrder(devInst, data[0]);	 
 
	 data[0] = CTSSensor.SENSOR_PCLK_SAMPLE_EDGE;
	 bRes &= Cam_SetIniSnrPclkSampleEdge(devInst, data[0]);  
 
	 data[0] = CTSSensor.SENSOR_HSYNC_POLARITY;
	 bRes &= Cam_SetIniSnrHsyncPolarity(devInst, data[0]);	 
 
	 data[0] = CTSSensor.SENSOR_VSYNC_POLARITY;
	 bRes &= Cam_SetIniSnrVsyncPolarity(devInst, data[0]);	 
 
	 //step 2.2
	 bRes &= Cam_SetIniDataPath(devInst, datapath);
 
	 //[PCB_MIPI_TRACE_SKEW_CALIBRATION]
	 data[0] = CTSSensor.MIPI_AUTO_SKEW_CALIBRATION;
	 data[1] = CTSSensor.MIPI_CLK_DATA0_SKEW_CALIBRATION;
	 data[2] = CTSSensor.MIPI_CLK_DATA1_SKEW_CALIBRATION;
	 data[3] = CTSSensor.MIPI_CLK_DATA2_SKEW_CALIBRATION;
	 data[4] = CTSSensor.MIPI_CLK_DATA3_SKEW_CALIBRATION;
	 bRes &= Cam_SetIniMipiAutoSkewCalibration(devInst, data[0], data[1], data[2], data[3], data[4]);
 
	 //[SENSOR_SETTING]
	 int length = CTSSensor.vecRegSettings.size();
	 tagREG *RegSettings;
	 RegSettings = new tagREG[length];
 
	 for(int i = 0; i < length; i++)
	 {
		 RegSettings[i].addr = CTSSensor.vecRegSettings[i].addr;
		 RegSettings[i].data = CTSSensor.vecRegSettings[i].data;
	 }
	 
	 bRes &= Cam_SetIniSnrRegSettings(devInst, RegSettings, length, byAddrWidth, byDataWidth);
 
	 delete [] RegSettings;

 	 if(bRes)
 	 {
		 //step 3
		 return Cam_SetConfigEx(devInst, NULL);
 	 }
	 else
	 	return FALSE;
 }

 BOOL CCTS::InitOS(int CamID)
 {
	 BOOL bRes = TRUE;
	 CString strOsFile;
	 //strOsFile = GetOpenShortConfigPath() + m_strOSFile[DevIdx] + _T(".ini");

	 //char *strFile = CStringToChar(strOsFile);

	 //#ifdef OSSETCONFIGFROMAPI


	 bRes &= OS_SetConfigFromAPI(CamID);	 
	 //#else
	 // bRes &= OS_SetConfig(g_vecCTSDev[DevIdx]->devinfo.nDevID, strFile);
	 //
	 //#endif


	 return bRes;
 }

 BOOL CCTS::OS_SetConfigFromAPI(int CamID)
 {
	 BOOL bRes = TRUE;

	 BOOL bDVPorMIPI = 0; // 0: MIPI, 1: DVP

	 if(CamSensorTab[CamID].port == 0)
		 bDVPorMIPI= 0;
	 else
		 bDVPorMIPI = 1;

	 bRes &= OS_SetIniDVPorMIPI(m_MyCTS[CamID].devInst, bDVPorMIPI);
	 bRes &= OS_SetIniShortPairEnable(m_MyCTS[CamID].devInst, 0);
	 float fpwrCap = 4.7;
	 bRes &= OS_SetIniPWRCapacitance(m_MyCTS[CamID].devInst, fpwrCap);

	 bRes &= OS_SetConfig(m_MyCTS[CamID].devInst, NULL);

	 return bRes;
 }




 BOOL CCTS::OS_Test(int CamID, LOGINFO &OsLogInfo)
 {

	 BOOL bRes = 0;
	 memset(&OsLogInfo, 0, sizeof(LOGINFO));

	 BOOL bOSInit = InitOS(CamID);
	 if(bOSInit)
	 {
		 bRes = OS_TestScan(m_MyCTS[CamID].devInst, &OsLogInfo);

	 }
	 return bRes;
 }
 void CCTS::OS_CTSToDT(int *OSValueArray, LOGINFO OsLogInfo)
 {
	 typedef struct _PINMAP
	 {
		 int dt;
		 int cts;
	 }PINMAP;

	 PINMAP pinmap_MIPI[DTPIN_MAX] = 
	 {
		 {0, 2}, // dovdd
		 {1, 0}, // dvdd
		 {2, 1}, // avdd
		 {3, 25}, // vpp
		 {4, 3}, // afvdd

		 {5, -1}, // gnd1
		 {6, -1}, // gnd2
		 {7, -1}, // gnd3
		 {8, -1}, // gnd4

		 {9, 22}, // clk_p
		 {10, 23}, // clk_n

		 {11, 5}, // d0p
		 {12, 7}, // d0n

		 {13, 9}, // d1p
		 {14, 11}, // d1n

		 {15, 13}, // d2p
		 {16, 15}, // d2n

		 {17, 19}, // d3p
		 {18, 21}, // d3n

		 {19, -1}, // NC1
		 {10, -1}, // D0
		 {21, -1}, // D1
		 {22, -1}, // D2
		 {23, -1}, // D3
		 {24, -1}, // D4
		 {25, -1}, // D5
		 {26, -1}, // D6
		 {27, -1}, // D7
		 {28, -1}, // D8
		 {29, -1}, // D9
		 {30, -1}, // NC2
		 {31, -1}, // PCLK
		 {32, -1}, // HSYNC
		 {33, -1}, // VSYNC
		 {34, -1}, // NC3
		 {35, -1}, // NC4

		 {36, 24}, // MCLK
		 {37, 8}, // PWDN
		 {38, 10}, // RESET
		 {39, 4}, // SCL
		 {40, 6}, // SDA
		 {41, 12}, // GPIO1
		 {42, 14}, // GPIO2
		 {43, 18}, // GPIO3
		 {44, 10}, // GPIO4
	 };

	 int id, ic;
	 for(id = 0; id < DTPIN_MAX; id++)
	 {
		 ic = pinmap_MIPI[id].cts;
		 if(ic >= 0)
			 OSValueArray[id] = OsLogInfo.pin[ic].voltISink*1000000;
		 else
			 OSValueArray[id] = 0;
	 }

	 return;
 }
 void CCTS::OS_CTSToDT_DVP(int *OSValueArray, LOGINFO OsLogInfo)
 {
	 typedef struct _PINMAP
	 {
		 int dt;
		 int cts;
	 }PINMAP;


	 PINMAP pinmap_DVP[DTPIN_MAX] = 
	 {
		 {0, 2}, // dovdd
		 {1, 0}, // dvdd
		 {2, 1}, // avdd
		 {3, 34}, // vpp
		 {4, 3}, // afvdd

		 {5, -1}, // gnd1
		 {6, -1}, // gnd2
		 {7, -1}, // gnd3
		 {8, -1}, // gnd4

		 {9, -1}, // clk_p
		 {10, -1}, // clk_n

		 {11, -1}, // d0p
		 {12, -1}, // d0n

		 {13, -1}, // d1p
		 {14, -1}, // d1n

		 {15, -1}, // d2p
		 {16, -1}, // d2n

		 {17, -1}, // d3p
		 {18, -1}, // d3n

		 {19, 5}, // SDA
		 {20, 4}, // SCL
		 {21, 7}, // VSYNC
		 {22, 39}, // RESET
		 {23, 20}, // PWDN
		 {24, 6}, // HSYNC
		 {25, 38}, // MCLK

		 {26, 25}, // D9
		 {27, 24}, // D8
		 {28, 17}, // D7
		 {29, 16}, // D6
		 {30, 21}, // PCLK
		 {31, 15}, // D5
		 {32, 14}, // D4
		 {33, 13}, // D3
		 {34, 12}, // D2
		 {35, 11}, // D1
		 {36, 10}, // D0

		 {37, -1}, // NC1
		 {38, -1}, // NC2
		 
	 
		 {39, -1}, // NC3
		 {40, -1}, // NC4

		 {41, 8}, // GPIO1
		 {42, 9}, // GPIO2
		 {43, 36}, // GPIO3
		 {44, 37}, // GPIO4
	 };

	 int id, ic;
	 for(id = 0; id < DTPIN_MAX; id++)
	 {
		 ic = pinmap_DVP[id].cts;
		 if(ic >= 0)
			 OSValueArray[id] = OsLogInfo.pin[ic].voltISink*1000000;
		 else
			 OSValueArray[id] = 0;
	 }

	 return;
 }
 
