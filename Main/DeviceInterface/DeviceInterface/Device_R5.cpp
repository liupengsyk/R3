#include "StdAfx.h"
#include "Device_R5.h"

#ifdef _DEBUG
#pragma comment(lib, "RolongoSDK/x86/Debug/HisFX3Platform32D.lib")
#else
#pragma comment(lib, "RolongoSDK/x86/Release/HisFX3Platform32.lib")
#endif
#define TestPINCount 25
CDevice_R5::CDevice_R5(void)
{
	isOpened = FALSE;
	R5_CamID=0;
	OpenCamNum=0;
	CloseCamNum=0;
}


CDevice_R5::~CDevice_R5(void)
{
	int iresult	=classPlatform.HisFX3CloseDevice();
	if(iresult!=0)
	{
		CString str;
		GetHisFX3ErrorInfo(iresult,str);
		str=_T("Stop Preview fail  ")+str;
		AddLog(R5_CamID,str,COLOR_RED);
	}
}

void CDevice_R5::SetCamTab(int CamID)
{
	R5_CamID=CamID;
	LoadSensorGenaralTab(CamID); 
	LoadSensorParalist(CamID); 
	LoadSensorSetting(CamID);
}
void CDevice_R5::SetGpioPinLevel(int gpioNum,BOOL bH,int CamID)
{
	return;
}
void CDevice_R5::SensorEnable(int gpioNum,BOOL bH,int CamID)
{
	//gpioNum=1:pwdn 2:reset 3:pwdn&reset
	//bH=0:��ƽ 1����pin
	if((gpioNum&0x02)==2)
	{
		if(CamID==0)
		    classPlatform.HisFX3PullReset(bH);
		else
			classPlatform.HisFX3PullReset_S2(bH);
	}
	if((gpioNum&0x01)==1)
	{
		if(CamID==0)
		    classPlatform.HisFX3PullPWND(bH);
		else
			classPlatform.HisFX3PullPWND_S2(bH);

	}
}
int CDevice_R5::GetGrabSize()
{
	int Grabsize=0;
	switch(stPreviewConfig[R5_CamID].ucDataFormat)
	{
	case HisBaylor8_BGGR:
	case HisBaylor8_GBRG:
	case HisBaylor8_RGGB:
	case HisBaylor8_GRBG:
		Grabsize=stPreviewConfig[R5_CamID].iWidth*stPreviewConfig[R5_CamID].iHeight;
		break;
	case HisBaylor10_BGGR:
	case HisBaylor10_GBRG:
	case HisBaylor10_RGGB:
	case HisBaylor10_GRBG:
		Grabsize=stPreviewConfig[R5_CamID].iWidth*stPreviewConfig[R5_CamID].iHeight*2;
		break;
	}

	return Grabsize;
}
vector<CString> CDevice_R5::GetDevList()
{
	vector<CString> DevNameList;
	int iresult=0;
	if(!classPlatform.HisFX3isopen())
	{
		iresult = classPlatform.HisFX3OpenDevice();
		if (iresult!=0)
		{
			CString str;
			GetHisFX3ErrorInfo(iresult,str);
			str=_T("Open device Fail")+str;
			AddLog(R5_CamID,str,COLOR_RED); 
			return DevNameList;
		}

	}
	isOpened = TRUE;
	if(classPlatform.HisFX3isopen()&&!iresult)
	{
		unsigned int uiHWSerialNow[4];
		classPlatform.HisFX3GetHardwareKey(uiHWSerialNow);
		char cDeviceID[512]={0};
		CString str;
		str.Format(_T("%u.%u.%u.%u"),uiHWSerialNow[0],uiHWSerialNow[1],uiHWSerialNow[2],uiHWSerialNow[3]);
		DevNameList.push_back(str);
		classPlatform.HisFX3CloseDevice();
	}
	return DevNameList;
}
int CDevice_R5::PositiveOpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) 
{
	if(!classPlatform.HisFX3isopen())
	{
		int iresult = classPlatform.HisFX3OpenDevice();
		if (iresult!=0)
		{
			CString str;
			GetHisFX3ErrorInfo(iresult,str);
			str=_T("Open device Fail")+str;
			AddLog(CamID,str,COLOR_RED); 
			return -1;
		}
	}
	std::vector<_HisFX3OS_Positive_Item> vectorPositive;
	//����22��//
	_HisFX3OS_Positive_Item stItem;
	stItem.bTest =true; //DOVDD
	stItem.flSpecMin	=pOSTab->Pin[0].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[0].OpenLimit;	
	strcpy(stItem.strName,"DOVDD");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //DVDD
	stItem.flSpecMin	=pOSTab->Pin[1].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[1].OpenLimit;	
	strcpy(stItem.strName,"DVDD");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //AVDD
	stItem.flSpecMin	=pOSTab->Pin[2].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[2].OpenLimit;	
	strcpy(stItem.strName,"AVDD");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //VPP
	stItem.flSpecMin	=pOSTab->Pin[3].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[3].OpenLimit;	
	strcpy(stItem.strName,"OTP");  //VPP
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //AFVDD
	stItem.flSpecMin	=pOSTab->Pin[4].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[4].OpenLimit;	
	strcpy(stItem.strName,"AF-VCC");
	vectorPositive.push_back(stItem);

	stItem.bTest =true; //CLK_P
	stItem.flSpecMin	=pOSTab->Pin[9].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[9].OpenLimit;	
	strcpy(stItem.strName,"MCP");  
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //CLK_N
	stItem.flSpecMin	=pOSTab->Pin[10].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[10].OpenLimit;	
	strcpy(stItem.strName,"MCN");
	vectorPositive.push_back(stItem);
	int i=0;
	for (i=0;i<4;i++)
	{
		stItem.bTest =true; //D0P
		stItem.flSpecMin	=pOSTab->Pin[11+i*2].ShortLimit;	
		stItem.flSpecMax	=pOSTab->Pin[11+i*2].OpenLimit;	
		sprintf(stItem.strName,"LAN%d-P",i);
		vectorPositive.push_back(stItem);
		stItem.bTest =true; //D0P
		stItem.flSpecMin	=pOSTab->Pin[12+i*2].ShortLimit;	
		stItem.flSpecMax	=pOSTab->Pin[12+i*2].OpenLimit;	
		sprintf(stItem.strName,"LAN%d-N",i);
		vectorPositive.push_back(stItem);
	}
	stItem.bTest =true; //MCLK
	stItem.flSpecMin	=pOSTab->Pin[36].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[36].OpenLimit;	
	sprintf(stItem.strName,"MCLK");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //PWDN
	stItem.flSpecMin	=pOSTab->Pin[37].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[37].OpenLimit;	
	sprintf(stItem.strName,"PWDN");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //RST
	stItem.flSpecMin	=pOSTab->Pin[38].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[38].OpenLimit;	
	sprintf(stItem.strName,"RST");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //SCL
	stItem.flSpecMin	=pOSTab->Pin[39].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[39].OpenLimit;	
	sprintf(stItem.strName,"SCL");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //SDA
	stItem.flSpecMin	=pOSTab->Pin[40].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[40].OpenLimit;	
	sprintf(stItem.strName,"SDA");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //IO2
	stItem.flSpecMin	=pOSTab->Pin[43].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[43].OpenLimit;	
	sprintf(stItem.strName,"IO_OUT0");
	vectorPositive.push_back(stItem);
	stItem.bTest =true; //IO3
	stItem.flSpecMin	=pOSTab->Pin[44].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[43].OpenLimit;	
	sprintf(stItem.strName,"IO_OUT1");
	vectorPositive.push_back(stItem);
	int iresult=classPlatform.HisFX3OSPositiveTest(CamID,vectorPositive, 4);
	OSValueArray[0]=vectorPositive[0].flVoltValue*1000;
	OSValueArray[1]=vectorPositive[1].flVoltValue*1000;
	OSValueArray[2]=vectorPositive[2].flVoltValue*1000;
	OSValueArray[3]=vectorPositive[3].flVoltValue*1000;
	OSValueArray[4]=vectorPositive[4].flVoltValue*1000;

	OSValueArray[9]=vectorPositive[5].flVoltValue*1000;
	OSValueArray[10]=vectorPositive[6].flVoltValue*1000;
	for (i=0;i<8;i++)
	{
		OSValueArray[11+i]=vectorPositive[7+i].flVoltValue*1000;
	}
	OSValueArray[36]=vectorPositive[15].flVoltValue*1000;
	OSValueArray[37]=vectorPositive[16].flVoltValue*1000;
	OSValueArray[38]=vectorPositive[17].flVoltValue*1000;
	OSValueArray[39]=vectorPositive[18].flVoltValue*1000;
	OSValueArray[40]=vectorPositive[19].flVoltValue*1000;
	OSValueArray[43]=vectorPositive[20].flVoltValue*1000;
	OSValueArray[44]=vectorPositive[21].flVoltValue*1000;
	if(iresult)
	{
		AddLog(CamID,_T("����O/S����NG"),COLOR_RED,200);
		return -1;
	}
	return 1;
}
int CDevice_R5::NegtiveOpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) 
{
	if(!classPlatform.HisFX3isopen())
	{
		int iresult = classPlatform.HisFX3OpenDevice();
		if (iresult!=0)
		{
			CString str;
			GetHisFX3ErrorInfo(iresult,str);
			str=_T("Open device Fail")+str;
			AddLog(CamID,str,COLOR_RED); 
			return -1;
		}
	}
	std::vector<_HisFX3OS_Negtive_Item> vectorNegtive;
	//����17��//
	_HisFX3OS_Negtive_Item stItem;
	stItem.bTest =true; //CLK_P
	stItem.flSpecMin	=pOSTab->Pin[9].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[9].OpenLimit;	
	strcpy(stItem.strName,"MCP");
	vectorNegtive.push_back(stItem);
	stItem.bTest =true; //CLK_N
	stItem.flSpecMin	=pOSTab->Pin[10].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[10].OpenLimit;	
	strcpy(stItem.strName,"MCN");
	vectorNegtive.push_back(stItem);
	int i=0;
	for (i=0;i<4;i++)
	{
		stItem.bTest =true; //D0P
		stItem.flSpecMin	=pOSTab->Pin[11+i*2].ShortLimit;	
		stItem.flSpecMax	=pOSTab->Pin[11+i*2].OpenLimit;	
		sprintf(stItem.strName,"LAN%d-P",i);
		vectorNegtive.push_back(stItem);
		stItem.bTest =true; //D0P
		stItem.flSpecMin	=pOSTab->Pin[12+i*2].ShortLimit;	
		stItem.flSpecMax	=pOSTab->Pin[12+i*2].OpenLimit;	
		sprintf(stItem.strName,"LAN%d-N",i);
		vectorNegtive.push_back(stItem);
	}
	stItem.bTest =true; //MCLK
	stItem.flSpecMin	=pOSTab->Pin[36].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[36].OpenLimit;	
	sprintf(stItem.strName,"MCLK");
	vectorNegtive.push_back(stItem);
	stItem.bTest =true; //PWDN
	stItem.flSpecMin	=pOSTab->Pin[37].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[37].OpenLimit;	
	sprintf(stItem.strName,"PWDN");
	vectorNegtive.push_back(stItem);
	stItem.bTest =true; //RST
	stItem.flSpecMin	=pOSTab->Pin[38].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[38].OpenLimit;	
	sprintf(stItem.strName,"RST");
	vectorNegtive.push_back(stItem);
	stItem.bTest =true; //SCL
	stItem.flSpecMin	=pOSTab->Pin[39].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[39].OpenLimit;	
	sprintf(stItem.strName,"SCL");
	vectorNegtive.push_back(stItem);
	stItem.bTest =true; //SDA
	stItem.flSpecMin	=pOSTab->Pin[40].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[40].OpenLimit;	
	sprintf(stItem.strName,"SDA");
	vectorNegtive.push_back(stItem);
	stItem.bTest =true; //IO2
	stItem.flSpecMin	=pOSTab->Pin[43].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[43].OpenLimit;	
	sprintf(stItem.strName,"IO_OOUT0");
	vectorNegtive.push_back(stItem);
	stItem.bTest =true; //IO3
	stItem.flSpecMin	=pOSTab->Pin[44].ShortLimit;	
	stItem.flSpecMax	=pOSTab->Pin[43].OpenLimit;	
	sprintf(stItem.strName,"IO_OUT1");
	vectorNegtive.push_back(stItem);
	int iresult=classPlatform.HisFX3OSNegtiveTest(0,vectorNegtive, 4);
	OSValueArray[9]=vectorNegtive[0].flVoltValue*1000;
	OSValueArray[10]=vectorNegtive[1].flVoltValue*1000;
	for (i=0;i<8;i++)
	{
		OSValueArray[11+i]=vectorNegtive[2+i].flVoltValue*1000;
	}
	OSValueArray[36]=vectorNegtive[10].flVoltValue*1000;
	OSValueArray[37]=vectorNegtive[11].flVoltValue*1000;
	OSValueArray[38]=vectorNegtive[12].flVoltValue*1000;
	OSValueArray[39]=vectorNegtive[13].flVoltValue*1000;
	OSValueArray[40]=vectorNegtive[14].flVoltValue*1000;
	OSValueArray[43]=vectorNegtive[15].flVoltValue*1000;
	OSValueArray[44]=vectorNegtive[16].flVoltValue*1000;
	if(iresult)
	{
		AddLog(CamID,_T("����O/S����NG"),COLOR_RED,200);
		return -1;
	}
	return 1;
}
int CDevice_R5::SensorpinOpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) 
{
	return 1;
}
int CDevice_R5::OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID) 
{
	//�������////
	if (pOSTab->OS_en)
	{
		return NegtiveOpenShortTest(pOSTab,OSValueArray,CamID);
	}
	//����������//
	//�������////
	if (pOSTab->PositiveOS_en)
	{
		return PositiveOpenShortTest(pOSTab,OSValueArray,CamID);
	}
	//����������//

	return 1;
}
int CDevice_R5::GetKitTypeDeviceInfo(int CamID)
{
	if(CamID==1) return 1;
	if (devName.GetLength()<3)
	{
		CString str;
		str.Format(_T("û��ָ������:%s���������趨����"),devName);
		AddLog(CamID,str,COLOR_RED,200); 
		return -1;
	}
	isOpened = TRUE;
	unsigned int uiHWSerialNow[4];
	int index=devName.Find(_T("."));
	CString temp1=devName.Left(index);
	CString temp2=devName.Mid(index+1);
	USES_CONVERSION;
	uiHWSerialNow[0]=strtoul(T2A(temp1),NULL,10);
	index=temp2.Find(_T("."));
	temp1=temp2.Left(index);
	temp2=temp2.Mid(index+1);
	uiHWSerialNow[1]=strtoul(T2A(temp1),NULL,10);
	index=temp2.Find(_T("."));
	temp1=temp2.Left(index);
	temp2=temp2.Mid(index+1);
	uiHWSerialNow[2]=strtoul(T2A(temp1),NULL,10);
	uiHWSerialNow[3]=strtoul(T2A(temp2),NULL,10);
	char cPlatformName[512]={0};
	int iresult = classPlatform.HisFX3OpenDevice(0,0,cPlatformName,0xFF,uiHWSerialNow);
	//Open Device
	if (iresult!=0)
	{
		CString str;
		GetHisFX3ErrorInfo(iresult,str);
		str=_T("Open device Fail")+str;
		AddLog(CamID,str,COLOR_RED);
		return -1;
	}
	return 1;
}
int  CDevice_R5::OpenDevice(CString DeviceName)
{

	return 1;
}
int CDevice_R5::OpenVideo(int CamID)
{
	CString str;
	int iresult = 0;
	if(CamID==0)
		iresult =classPlatform.HisFX3StartPreview(&stPreviewConfig[CamID]);
	else
	   iresult =classPlatform.HisFX3StartPreview_S2(&stPreviewConfig[CamID]);
	if(iresult){
		GetHisFX3ErrorInfo(iresult,str);
		str=_T("preview fail")+str;
		AddLog(CamID,str,COLOR_RED);
		return FALSE;
	}
	OpenCamNum++;
	return 1;
}
void  CDevice_R5::GetHisFX3ErrorInfo(int errorcode, CString &errormsg)
{
	switch (errorcode){
	case HisFX3Error_Construct:
		errormsg.Format(_T("Error Code=0x%04X,���󣺹���CHisFX3Platform��ʧ��"),errorcode);
		break;
	case HisFX3Error_CloseUSB3:
		errormsg.Format(_T("Error Code=0x%04X,���󣺹ر�3.0ƽ̨ʧ��"),errorcode);
		break;
	case HisFX3Error_Timeout:
		errormsg.Format(_T("Error Code=0x%04X,���󣺲�����ʱ"),errorcode);
		break;
	case HisFX3Error_Parameter:
		errormsg.Format(_T("Error Code=0x%04X,���󣺴������������Ҫ��"),errorcode);
		break;
	case HisFX3Error_IsNotOpen:
		errormsg.Format(_T("Error Code=0x%04X,����3.0�豸û�п��������ȿ����豸"),errorcode);
		break;
	case HisCCMError_NotPreivew:
		errormsg.Format(_T("Error Code=0x%04X,���󣺻�û�п�ʼͼ��Ԥ�����"),errorcode);
		break;
	case HisFX3Error_IsNotPreview:
		errormsg.Format(_T("Error Code=0x%04X,���󣺻�û�п�ʼͼ��Ԥ�����"),errorcode);
		break;
	case HisFX3Error_MallocBuffer:
		errormsg.Format(_T("Error Code=0x%04X,���������ڴ�ռ�ʧ��"),errorcode);
		break;
	case HisFX3Error_USBDataOut:
		errormsg.Format(_T("Error Code=0x%04X,����3.0���ݴ�����󣬽�������ƽ̨"),errorcode);
		break;
	case HisFX3Error_Hardware:
		errormsg.Format(_T("Error Code=0x%04X,����3.0ƽ̨Ӳ������������"),errorcode);
		break;
	case HisFX3Error_NotSuperSpeed:
		errormsg.Format(_T("Error Code=0x%04X,����3.0ƽ̨���ǳ����ٶȣ�����������������߻�������"),errorcode);
		break;
	case HisFX3Error_CreateThread:
		errormsg.Format(_T("Error Code=0x%04X,���󣺴����߳�ʧ�ܣ����windowsϵͳ��Դ���趨"),errorcode);
		break;
	case HisFX3Error_License:
		errormsg.Format(_T("Error Code=0x%04X,����License����"),errorcode);
		break;
	case HisFX3Error_NoRespond:
		errormsg.Format(_T("Error Code=0x%04X,�����豸IICû����Ӧ"),errorcode);
		break;
	case HisFX3Error_WirteIIC:
		errormsg.Format(_T("Error Code=0x%04X,����д��IICʧ��"),errorcode);
		break;
	case HisFX3Error_Check:
		errormsg.Format(_T("Error Code=0x%04X,����У��ʧ��"),errorcode);
		break;
	case HisFX3Error_AlgData:
		errormsg.Format(_T("Error Code=0x%04X,���󣺵����㷨�����ݲ�����Ҫ��"),errorcode);
		break;
	case HisFX3Error_CreateEvent:
		errormsg.Format(_T("Error Code=0x%04X,���󣺴����¼�ʧ��"),errorcode);
		break;
	case HisCCMError_NoFrame:
		errormsg.Format(_T("Error Code=0x%04X,����û��֡���"),errorcode);
		break;
	case HisCCMError_Result:
		errormsg.Format(_T("Error Code=0x%04X,���󣺽���������"),errorcode);
		break;
	case HisCCMError_Database:
		errormsg.Format(_T("Error Code=0x%04X,����sqlite���ݿ�����"),errorcode);
		break;
	case HisCCMError_opConflic:
		errormsg.Format(_T("Error Code=0x%04X,���󣺲������ڳ�ͻ"),errorcode);
		break;
	case HisCCMError_luxtooLow:
		errormsg.Format(_T("Error Code=0x%04X,���󣺻������ȹ���"),errorcode);
		break;
	case HisCCMError_luxtooHigh:
		errormsg.Format(_T("Error Code=0x%04X,���󣺻������ȹ���"),errorcode);
		break;
	case HisCCMError_readIIC:
		errormsg.Format(_T("Error Code=0x%04X,���󣺶�ȡI2C"),errorcode);
		break;
	case HisCCMError_OpenFile:
		errormsg.Format(_T("Error Code=0x%04X,���󣺴��ļ�ʧ��"),errorcode);
		break;
	case HisCCMError_FileAttribute:
		errormsg.Format(_T("Error Code=0x%04X,���󣺻�ȡ�ļ�����ʧ�ܣ��������Բ�ƥ��"),errorcode);
		break;
	case HisCCMError_ReadFile:
		errormsg.Format(_T("Error Code=0x%04X,���󣺶�ȡ�ļ�ʧ��"),errorcode);
		break;
	case HisCCMError_WriteFile:
		errormsg.Format(_T("Error Code=0x%04X,����д���ļ�ʧ��"),errorcode);
		break;
	case HisFX3Error_ShortTestAbort:
		errormsg.Format(_T("Error Code=0x%04X,���󣺶�·�����쳣"),errorcode);
		break;
	case	HisFX3Error_NegOpenTestAbort:
		errormsg.Format(_T("Error Code=0x%04X,���󣺸��򿪶�·�����쳣"),errorcode);
		break;
	case	HisFX3Error_PosOpenTestAbort:
		errormsg.Format(_T("Error Code=0x%04X,�������򿪶�·�����쳣"),errorcode);
		break;	
	case	HisFX3Error_ImgBufConvert:
		errormsg.Format(_T("Error Code=0x%04X,����ͼ��ת��ʧ��"),errorcode);
		break;			
	case	HisFX3Error_NoHardware:
		errormsg.Format(_T("Error Code=0x%04X,�����Ҳ���װ��Ӳ��"),errorcode);
		break;				
	case	HisFX3Error_Connect:
		errormsg.Format(_T("Error Code=0x%04X,����װ�������쳣"),errorcode);
		break;						
	case	HisFX3Error_GrabFrame:
		errormsg.Format(_T("Error Code=0x%04X,����ͼ������쳣"),errorcode);
		break;					
	case	HisFX3Error_OSAbort	:
		errormsg.Format(_T("Error Code=0x%04X,���󣺿���·�����쳣"),errorcode);
		break;					
	case	HisFX3Error_VersionNotMatch:
		errormsg.Format(_T("Error Code=0x%04X,��������汾��ƥ��"),errorcode);
		break;					
	case	HisFX3Error_NoGrabber:
		errormsg.Format(_T("Error Code=0x%04X,����û������װ��"),errorcode);
		break;					
	case	HisFX3Error_GrabberClosing:
		errormsg.Format(_T("Error Code=0x%04X,���󣺲������ڳ�ͻ"),errorcode);
		break; 		
	case HisFX3Error_Other:
	default:
		errormsg.Format(_T("Error Code=0x%04X,��������0x%X"),errorcode);
		break;
	}
}
int CDevice_R5::CloseDevice(int CamID)
{
	int iresult=0;
	if(CamID==0)
	  iresult=classPlatform.HisFX3StopPreview();
	else
		iresult=classPlatform.HisFX3StopPreview_S2();
	if(iresult!=0)
	{
		CString str;
		GetHisFX3ErrorInfo(iresult,str);
		str=_T("Stop Preview fail  ")+str;
		AddLog(CamID,str,COLOR_RED);
	}
	isOpened = FALSE;
	return 0;
}
int  CDevice_R5::GrabFrame(BYTE *pInBuffer, ULONG uBufferSize,ULONG *pGrabSize,FrameInfo *pInfo,int CamID)
{
	unsigned __int64 ui64FrameIndex;
	int iresult	=0;
	if(CamID==0)
	    iresult	=classPlatform.HisFX3GrabFrame(&pInBuffer, uBufferSize, ui64FrameIndex);
	else 
		iresult	=classPlatform.HisFX3GrabFrame_S2(&pInBuffer, uBufferSize, ui64FrameIndex);   
	*pGrabSize=uBufferSize;

	if (iresult!=0)
	{
		CString str;
		GetHisFX3ErrorInfo(iresult,str);
		str=_T("Open device Fail")+str;
		AddLog(CamID,str,COLOR_RED); 
	}
	return Turnresult(iresult);
}
int CDevice_R5::WriteSensorReg(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int iDevID,int CamID)
{
	if(uReg==0xffff) 
	{
		if(CamID==0)
		    classPlatform.HisFX3WriteIIC(0x00, 0x0000, uValue,0x1608);
		else
			classPlatform.HisFX3WriteIIC_S2(0x00, 0x0000, uValue,0x1608);

	}
	if(uReg==0xfffe) return 1;
	USHORT mode=GetMode(byMode);
	int iresult=0;
	if(CamID==0)
	  iresult=classPlatform.HisFX3WriteIIC(uAddr, uReg, uValue,mode);
	else
	  iresult=classPlatform.HisFX3WriteIIC_S2(uAddr, uReg, uValue,mode);
	return Turnresult(iresult);
}
int CDevice_R5::ReadSensorReg(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int iDevID,int CamID)
{
	USHORT mode=GetMode(byMode);
	unsigned long long uiData=0;
	int iresult=0;
	if (CamID==0)
	{
		iresult=classPlatform.HisFX3ReadI2C(uAddr, uReg, &uiData,mode);
	}
	else
	   iresult=classPlatform.HisFX3ReadI2C_S2(uAddr, uReg, &uiData,mode);
	*pValue=uiData;
	return Turnresult(iresult);
}
int  CDevice_R5::WriteI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID,int CamID )
{ 
	int iresult=0;
	if (CamID==0)
	{
		iresult=classPlatform.HisFX3PageWriteIIC(uDevAddr, uRegAddr, uRegAddrSize,pData,uSize);
	}
	else
	   iresult=classPlatform.HisFX3PageWriteIIC_S2(uDevAddr, uRegAddr, uRegAddrSize,pData,uSize);
	return Turnresult(iresult);
}

int  CDevice_R5::ReadI2c(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop,int iDevID,int CamID )
{
	int iresult=0;
	if (CamID==0)
	{
	   iresult=classPlatform.HisFX3PageReadIIC(uDevAddr, uRegAddr, uRegAddrSize,pData,uSize);
	}
	else
      iresult=classPlatform.HisFX3PageReadIIC_S2(uDevAddr, uRegAddr, uRegAddrSize,pData,uSize);
	return Turnresult(iresult);
}
int CDevice_R5::PmuGetStandyCurrent(int Current[],int iCount,int iDevID,int CamID)
{
	for (int i=0;i<iCount;i++)
	{
		Current[i]=iStandyCurrent[i];
	}
	return 1;
}
int CDevice_R5::PmuGetCurrent(int Current[],int iCount,int iDevID,int CamID) 
{
	float flCurrentAVDD, flCurrentDOVDD, flCurrentDVDD, flCurrentAF, flCurrentVFuse,flCurrentplow;

	int iresult=0;
	if (CamID==0)
	{
	   iresult	=classPlatform.HisFX3GetWorkCurrent(flCurrentAVDD, flCurrentDVDD, flCurrentDOVDD, flCurrentAF, flCurrentVFuse,flCurrentplow);
	}
	else
		iresult	=classPlatform.HisFX3GetWorkCurrent_S2(flCurrentAVDD, flCurrentDVDD, flCurrentDOVDD, flCurrentAF, flCurrentVFuse,flCurrentplow);
	Current[0]=flCurrentAVDD;
	Current[1]=flCurrentDOVDD;
	Current[2]=flCurrentDVDD;
	Current[3]=flCurrentAF;
	Current[4]=flCurrentVFuse;
	return Turnresult(iresult);
}
int CDevice_R5::PmuGetVoltage(int Voltage[],int iCount,int iDevID,int CamID) 
{
	float pucDOVDD, pucAVDD,pucDVDD,pucAF,pucVFuse;
	int iresult=0;
	if (CamID==0)
	{
	    iresult	=classPlatform.HisFX3GetVolt(&pucDOVDD,&pucAVDD,&pucDVDD,&pucAF,&pucVFuse);
	}
	else
		iresult	=classPlatform.HisFX3GetVolt_S2(&pucDOVDD,&pucAVDD,&pucDVDD,&pucAF,&pucVFuse);
	Voltage[0]=pucAVDD;
	Voltage[1]=pucDOVDD;
	Voltage[2]=pucDVDD;
	Voltage[3]=pucAF;
	Voltage[4]=pucVFuse;
	return Turnresult(iresult);
}
int CDevice_R5::Turnresult(int iresult)
{
	int flag=0;
	if (iresult!=0) flag=0;
	else flag=1;
	return flag;
}
BOOL CDevice_R5::LoadSensorSetting(int CamID)
{
	CString strname = ReadConfigString(_T("SensorName"),0, CamID);
	USES_CONVERSION;
	strcpy(stPreviewConfig[CamID].strSensorName,T2A(strname));
	stPreviewConfig[CamID].iWidth   = ReadConfigData(_T("width"),0, CamID);
	stPreviewConfig[CamID].iHeight  = ReadConfigData(_T("height"),0, CamID);
	int iType  = ReadConfigData(_T("type"),2, CamID);

	int iImageTransFormart = ReadConfigData( _T("port"), 0, CamID);
	int iResetPWDNPIN=ReadConfigData( _T("pin"), 0, CamID);
	if(iImageTransFormart==0) //MIPI
	{
		stPreviewConfig[CamID].bDVP_VS_ActiveHigh=false;
		stPreviewConfig[CamID].bDVP_HS_ActiveHigh=false;
		if((iResetPWDNPIN&0x01)==0x01) stPreviewConfig[CamID].bPWND_ActiveLow=true;
		else stPreviewConfig[CamID].bPWND_ActiveLow=false;
		if((iResetPWDNPIN&0x02)==0x02) stPreviewConfig[CamID].bReset_ActiveLow=true;
		else stPreviewConfig[CamID].bReset_ActiveLow=false;
	}
	else if(iImageTransFormart==1) //DVP Parallel
	{
		stPreviewConfig[CamID].bReset_ActiveLow=false;
		stPreviewConfig[CamID].bPWND_ActiveLow=false;
		if((iResetPWDNPIN&0x01)==0x01) stPreviewConfig[CamID].bDVP_VS_ActiveHigh=true;
		else stPreviewConfig[CamID].bDVP_VS_ActiveHigh=false;
		if((iResetPWDNPIN&0x02)==0x02) stPreviewConfig[CamID].bDVP_HS_ActiveHigh=true;
		else stPreviewConfig[CamID].bDVP_HS_ActiveHigh=false;
	}
	else 
	{
		return FALSE;
	}
	stPreviewConfig[CamID].ucSlave  = ReadConfigData( _T("SlaveID"), 0, CamID);
	int i2CMode     = ReadConfigData( _T("mode"), 0, CamID);
	if(i2CMode ==0 || i2CMode==1)stPreviewConfig[CamID].usI2CMode=0x0808;
	else if(i2CMode==2)stPreviewConfig[CamID].usI2CMode=0x0816;
	else if(i2CMode==3)stPreviewConfig[CamID].usI2CMode=0x1608;
	else if(i2CMode==4)stPreviewConfig[CamID].usI2CMode=0x1616;

	int iOutFormat= ReadConfigData( _T("outformat"), 0x00, CamID);
	if(iOutFormat==0) 
	{ 
		stPreviewConfig[CamID].ucDataFormat=HisBaylor8_RGGB; 
		if(iType==6 || iType==0)
			stPreviewConfig[CamID].ucDataFormat=HisBaylor10_RGGB; 
	}
	else if(iOutFormat==1)
	{
		stPreviewConfig[CamID].ucDataFormat=HisBaylor8_GRBG;
		if(iType==6 || iType==0)
			stPreviewConfig[CamID].ucDataFormat=HisBaylor10_GRBG; 
	}
	else if(iOutFormat==2)
	{
		stPreviewConfig[CamID].ucDataFormat=HisBaylor8_GBRG; 
		if(iType==6 || iType==0)
			stPreviewConfig[CamID].ucDataFormat=HisBaylor10_GBRG; 
	}
	else if(iOutFormat==3)
	{ 
		stPreviewConfig[CamID].ucDataFormat=HisBaylor8_BGGR;
		if(iType==6 || iType==0)
			stPreviewConfig[CamID].ucDataFormat=HisBaylor10_BGGR; 
	}
	switch (stPreviewConfig[CamID].ucDataFormat){
	case HisBaylor8_BGGR:
	case HisBaylor8_RGGB:
	case HisBaylor8_GRBG:
	case HisBaylor8_GBRG:
		stPreviewConfig[CamID].uiFrameByteCount	=	stPreviewConfig[CamID].iWidth *stPreviewConfig[CamID].iHeight;
		break;
	case HisBaylor10_BGGR:
	case HisBaylor10_RGGB:
	case HisBaylor10_GRBG:
	case HisBaylor10_GBRG:
		stPreviewConfig[CamID].uiFrameByteCount	=	stPreviewConfig[CamID].iWidth *stPreviewConfig[CamID].iHeight *10/8;
		break;
	case HisYUV8_422_YUYV:
	case HisYUV8_422_UYVY:
	case HisYUV8_422_YVYU:
	case HisYUV8_422_VYUY:
		stPreviewConfig[CamID].uiFrameByteCount	=	stPreviewConfig[CamID].iWidth *stPreviewConfig[CamID].iHeight *2;
		break;
	case HisRGB_RGB24:
		stPreviewConfig[CamID].uiFrameByteCount	=	stPreviewConfig[CamID].iWidth *stPreviewConfig[CamID].iHeight *3;
		break;
	default:
		stPreviewConfig[CamID].uiFrameByteCount	=	0;
		break;
	}
	stPreviewConfig[CamID].flMCLK     = ReadConfigData( _T("mclk"), 0x01, CamID);

	stPreviewConfig[CamID].ucVolt_AVDD     = ReadConfigData( _T("avdd"), 0x00, CamID);
	stPreviewConfig[CamID].ucVolt_IODD=stPreviewConfig[CamID].ucVolt_DOVDD   = ReadConfigData( _T("dovdd"), 0x00, CamID);
	stPreviewConfig[CamID].ucVolt_DVDD    = ReadConfigData( _T("dvdd"), 0x00, CamID);
	stPreviewConfig[CamID].ucVolt_AF    = ReadConfigData( _T("afvdd"), 0x00, CamID);
	stPreviewConfig[CamID].ucVolt_VFuse = ReadConfigData(_T("vpp"),0x00, CamID);
	stPreviewConfig[CamID].uiDummyBottom	=	stPreviewConfig[CamID].uiDummyLeft = stPreviewConfig[CamID].uiDummyRight =stPreviewConfig[CamID].uiDummyTop = 0;
	stPreviewConfig[CamID].bUseDDR=true;
	stPreviewConfig[CamID].bOutputXSVS=FALSE;   //LVDS Interface using
	stPreviewConfig[CamID].bDebug=TRUE;

	strAppName=_T("RolongoPlatform_Set");
	stPreviewConfig[CamID].usI2CSpeed=ReadConfigData( _T("HW_I2C_Speed"), 400, CamID);
	stPreviewConfig[CamID].usI2CIntervalTime=ReadConfigData( _T("HW_I2C_IntervalTime"), 200, CamID);
	stPreviewConfig[CamID].ucCommProtocal=ReadConfigData( _T("HW_I2C_CommProtocal"), 0x00, CamID);
	stPreviewConfig[CamID].usI2CCount	=ReadConfigData( _T("HW_I2C_Count"), 1024, CamID);
	if(stPreviewConfig[CamID].usI2CCount<=0)
	{
		AddLog(CamID,_T("HW_I2C_Count"),COLOR_RED,200);
		stPreviewConfig[CamID].usI2CCount=1024;
	}
	stPreviewConfig[CamID].sensorFactory=(_HisFX3_ImageSenor_Factory)ReadConfigData( _T("HW_PowerupSequence"), 0x00, CamID);
	stPreviewConfig[CamID].ucMIPILane=ReadConfigData( _T("Image_MIPI_Lane"), 0x4, CamID);
	strAppName=_T("sensor");
	if(stPreviewConfig[CamID].sensorFactory== 0xFF || stPreviewConfig[CamID].ucMIPILane==0xFF)
	{
		return FALSE;
	}
	HisReleaseNewB(stPreviewConfig[CamID].pucSlave);
	HisReleaseNewB(stPreviewConfig[CamID].puiReg);
	HisReleaseNewB(stPreviewConfig[CamID].pui64Data);
	HisReleaseNewB(stPreviewConfig[CamID].pusType);

	stPreviewConfig[CamID].pucSlave		=	new unsigned char[stPreviewConfig[CamID].usI2CCount];
	stPreviewConfig[CamID].puiReg			=	new unsigned int[stPreviewConfig[CamID].usI2CCount];
	stPreviewConfig[CamID].pui64Data	=	new unsigned __int64[stPreviewConfig[CamID].usI2CCount];
	stPreviewConfig[CamID].pusType		=	new unsigned short[stPreviewConfig[CamID].usI2CCount];
	int count=0;
	int mode=stPreviewConfig[CamID].usI2CMode;
	for (int i=0;i<CamSensorTab[CamID].ParaListSize;i+=2)
	{
		if (CamSensorTab[CamID].ParaList[i]==0xffff)
		{
			stPreviewConfig[CamID].pucSlave[i/2]=0x00;
			stPreviewConfig[CamID].puiReg[i/2]=0x0000;
			stPreviewConfig[CamID].pui64Data[i/2]=CamSensorTab[CamID].ParaList[i+1];
			stPreviewConfig[CamID].pusType[i/2]=0x1608;
		}
		else if (CamSensorTab[CamID].ParaList[i]==0xfffe)
		{
			if(CamSensorTab[CamID].ParaList[i+1]==3) mode=0x1608;
			if(CamSensorTab[CamID].ParaList[i+1]==4) mode=0x1616;
			continue;
			count++;
		}
		else
		{
			stPreviewConfig[CamID].pucSlave[i/2]=stPreviewConfig[CamID].ucSlave;
			stPreviewConfig[CamID].puiReg[i/2]=CamSensorTab[CamID].ParaList[i];
			stPreviewConfig[CamID].pui64Data[i/2]=CamSensorTab[CamID].ParaList[i+1];
			stPreviewConfig[CamID].pusType[i/2]=mode;
		}

	}			
	stPreviewConfig[CamID].usI2CCount=CamSensorTab[CamID].ParaListSize/2-count;
	return TRUE;
}
USHORT CDevice_R5::GetMode(int mode)
{
	USHORT temp=3;
	if (mode==0)
	{
		temp=0x0808;
	}
	if (mode==1||mode==3)
	{
		temp=0x1608;
	}
	else
		temp=0x1616;
	return temp;
}
BOOL CDevice_R5::InitiaSensor(int CamID,CString SettingPath)
{
	if (SettingPath.GetLength()>4)
	{
		;//���New �����趨����
	}
	return TRUE;
}
