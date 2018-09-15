#include "StdAfx.h"
#include "AE_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new AE_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
AE_TestItem::AE_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	m_cfgName = CString(lpszName);
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄
	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化
	Count=0;
	SetLEDFlag=TRUE;
	//.....
}

AE_TestItem::~AE_TestItem(void)
{

}


int AE_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	CurrentName=GetName();
	GlobalSetting=_T("WBSetting");
	LoadOption(); 

	
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AE_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
	CString path=m_pInterface->Ctrl_GetCurrentPath();
	path=path+L"SensorTab\\init.ini";
	SetConfigPath(path);	 //设置CONFIG路径	
	SetName(L"ErrorCode");
	SetLEDFlag=TRUE;
	/*
	CString temp = _T("");
	temp.Format(_T("CAM%d_ErrorCode"),CamID);
	WriteConfigInt(temp, pDlg->ErrorCode); */
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(m_cfgName);
	interval=0;
    brightness = 0;
	
	CString  GlobalSetting=_T("WBSetting");
	SetName(GlobalSetting);	
	GlobalTime=ReadConfigInt(_T("GlobalTime"), 3);
	int CurrentTagTime=ReadConfigInt(_T("CurrentTimeTag"),2);
	SetName(CurrentName);
	if (pDlg->m_DNPExp==1)
	{
		if(GlobalTime>=3) return 0;
	}
	
	if (pDlg->m_RemExp==1)
	{
		if (pDlg->m_InitExposure == 0)
		{
			pDlg->m_InitExposure = 0x0100;
		}
		m_pInterface->Sensor_WriteExp(CamID,pDlg->m_InitExposure);
	}

	m_pInterface->Device_WriteI2c(SlaveID,GainReg1,GainVal1,mode,CamID);
	m_pInterface->Device_WriteI2c(SlaveID,GainReg2,GainVal2,mode,CamID);

	
	return 0;
}

BOOL AE_TestItem::ChangeLEDLSChannel(int LEDType,HiStarOTP_LampType lamptype)
{
	BOOL Result=TRUE;       //ChangeLED Channel 前开启Play模式，切换Channel 后等待LEDWAITTIME，计算画面AWB值，进行光源切换是否成功防呆

	switch (lamptype)
	{
	case LAMP_CCT_A:
		m_pInterface->Ctrl_Addlog(CamID,_T("**[切换A光源]**"),COLOR_BLUE,200);
		if(!m_pInterface->SetLEDChannel(CamID,m_LightSource.LAMP_CCT_A.Channel,LEDType,pDlg->m_WaitFlag)) return FALSE;
		break;
	case LAMP_CCT_TL84:
		m_pInterface->Ctrl_Addlog(CamID,_T("**[切换TL84光源]**"),COLOR_BLUE,200);
		if(!m_pInterface->SetLEDChannel(CamID,m_LightSource.LAMP_CCT_TL84.Channel,LEDType,pDlg->m_WaitFlag)) return FALSE;
		break;	
	case LAMP_CCT_D65:
		m_pInterface->Ctrl_Addlog(CamID,_T("**[切换D65光源]**"),COLOR_BLUE,200);
		if(!m_pInterface->SetLEDChannel(CamID,m_LightSource.LAMP_CCT_D65.Channel,LEDType,pDlg->m_WaitFlag)) return FALSE;
		break;	
	default:
		return FALSE;
	}
	Sleep(500);

	return Result;

}

/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AE_TestItem::Testing()
{
   //TODO:在此添加测试项代码


	if (pDlg->m_DNPExp==1)
	{
		if((GlobalTime<pDlg->m_count)||(GlobalTime>=(pDlg->m_count+3))) return 0;
	}

	//切换光源
	if (pDlg->SetLedEN)
	{
		if(SetLEDFlag)
		{
			if (!ChangeLEDLSChannel(pDlg->m_LEDType,HiStarOTP_LampType(pDlg->LEDChannel)))
			{
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Fail!"));
				SetResult(RESULT_FAIL);
				Sleep(500);
				return 0;
			}
			Sleep(1000);
			SetLEDFlag=FALSE;
			return 1;
		}
	}

	if(interval<intervalSpec)
	{
		interval++;
		return 1;
	}

	interval = 0;

	int target = (pDlg->m_LowLimit+pDlg->m_HighLimit)/2;
	float factor =1.0;
	CString temp;
	int i=0;
	 {
		interval=0;
		ImageInfo *MyImageInfo = NULL;		
		MyImageInfo = m_pInterface->Ctrl_GetImgInfo(CamID);
		if (MyImageInfo->Raw_buffer == NULL)
		{
			Count =0;
			m_pInterface->Ctrl_Addlog(CamID,_T("grab image fail!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("grab image fail!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
		if (pDlg->m_10bit)
		{
			if (pDlg->m_is4Cell)
				CenterBlockRawInfo4Cell(MyImageInfo->Raw_buffer,MyImageInfo,pDlg->m_Ratio,pDlg->m_Ratio2,brightness,pDlg->m_WhiteEnable);
			else if (pDlg->m_is2PD)
				CenterBlockRawInfo2PD(MyImageInfo->Raw_buffer,MyImageInfo,pDlg->m_Ratio,pDlg->m_Ratio2,brightness,pDlg->m_WhiteEnable);
			else
				CenterBlockRawInfo(MyImageInfo->Raw_buffer,MyImageInfo,pDlg->m_Ratio,pDlg->m_Ratio2,brightness,pDlg->m_WhiteEnable);
		}
		else
		{
			USHORT* MyRaw8Buff=new USHORT[MyImageInfo->imgwidth*MyImageInfo->imgheight];
			if (NULL == MyRaw8Buff)
			{
				Count =0;
				m_pInterface->Ctrl_Addlog(CamID,_T("MyRaw8Buff buffer 申请失败!!"),COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("MyRaw8Buff buffer 申请失败!"));
				SetResult(RESULT_FAIL);
				return 0;
			}
			memset(MyRaw8Buff,0,MyImageInfo->imgwidth*MyImageInfo->imgheight);
			for(int i=0;i<MyImageInfo->imgwidth*MyImageInfo->imgheight;i++)
			{
				MyRaw8Buff[i]=(MyImageInfo->Raw_buffer[i]>>2);
			}
			if (pDlg->m_is4Cell)
				CenterBlockRawInfo4Cell(MyRaw8Buff,MyImageInfo,pDlg->m_Ratio,pDlg->m_Ratio2,brightness,pDlg->m_WhiteEnable);
			else if (pDlg->m_is2PD)
				CenterBlockRawInfo2PD(MyRaw8Buff,MyImageInfo,pDlg->m_Ratio,pDlg->m_Ratio2,brightness,pDlg->m_WhiteEnable);
			else
				CenterBlockRawInfo(MyRaw8Buff,MyImageInfo,pDlg->m_Ratio,pDlg->m_Ratio2,brightness,pDlg->m_WhiteEnable);
		
			SAFE_DELETE_ARRAY(MyRaw8Buff);
		}
		
		temp.Format(_T("brightness=%f <%d , %d>"),brightness,pDlg->m_LowLimit,pDlg->m_HighLimit);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		ShowROIInfo();
		if ((brightness<pDlg->m_LowLimit || brightness>pDlg->m_HighLimit))
		{
			if(brightness>pDlg->m_BLC)
			{
				factor = 1.0* (target-pDlg->m_BLC) / (brightness-pDlg->m_BLC);
				if (pDlg->m_10bit)
				{
					if(brightness>=1023)//Raw10
					{
						factor =factor/2;
					}
				}
				else
				{
					if(brightness>=255)//Raw8
					{
						factor =factor/2;
					}
				}
				
			}
			else if(brightness!=0)
			{
				factor = 1.0*(target)/brightness;
			}
			else
			{
				factor = target;
			}
			
			USHORT hig;
			USHORT low;
			//m_pInterface->Sensor_ReadExp(CamID,exposure);
			m_pInterface->Device_ReadI2c(SlaveID,AddressH,&hig,mode,CamID);
			m_pInterface->Device_ReadI2c(SlaveID,AddressL,&low,mode,CamID);
			exposure=factor*(hig*256+low);
			hig=exposure>>8;
			low=exposure%256;
			m_pInterface->Device_WriteI2c(SlaveID,AddressH,hig,mode,CamID);
			m_pInterface->Device_WriteI2c(SlaveID,AddressL,low,mode,CamID);
				
			Sleep(10);			
		}
		else
		{
			m_pInterface->Sensor_ReadExp(CamID,exposure);
			WriteConfigInt(_T("InitExposure"),exposure);
			m_pInterface->UnlocktheFile(CamID);//解锁加密狗
			pDlg->m_InitExposure = exposure;
			Count = 0;
			temp.Format(_T("Auto exposure OK! \nCenter brightness=%f"),brightness);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			SetResult(RESULT_PASS);		
			return 0;
		}		
	}
     Count++;
	if (Count >pDlg->m_excount)
	{
		Count=0;
		m_pInterface->Ctrl_Addlog(CamID,_T("Auto expoure fail!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("自动曝光失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	return 1;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AE_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	SetLEDFlag=TRUE;
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AE_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_count=ReadConfigInt(_T("m_count"),12);
	pDlg->m_WhiteEnable=ReadConfigInt(_T("WhiteEnable"),0);
	pDlg->m_White128Enable = ReadConfigInt(_T("White128Enable"),0);
	pDlg->m_HighLimit=ReadConfigInt(_T("HighLimit"),180);
	pDlg->m_LowLimit=ReadConfigInt(_T("LowLimit"),160);
	pDlg->m_Ratio=ReadConfigInt(_T("Ratio"),5);
	pDlg->m_Ratio2=ReadConfigInt(_T("Ratio2"),5);
	pDlg->m_10bit=ReadConfigInt(_T("m_10bit"),0);
	pDlg->m_is4Cell=ReadConfigInt(_T("m_is4Cell"),0);
	pDlg->m_is2PD = ReadConfigInt(_T("is2PD"),0);
	pDlg->m_StartX = ReadConfigInt(_T("StartX"),50);
	pDlg->m_StartY = ReadConfigInt(_T("StartY"),50);

	pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
	pDlg->m_excount=ReadConfigInt(_T("count"),20);
	pDlg->m_SlaveID=ReadConfigString(_T("SlaveID"),_T("0x20"));
	pDlg->m_InitExposure=ReadConfigInt(_T("InitExposure"),0x2080);
	pDlg->m_aeTargetString = ReadConfigString(_T("AE_TARGET_STRING"), _T("G_Target"));
	pDlg->ErrorCode = ReadConfigInt(_T("ErrorCode"),0);
	pDlg->m_mode=ReadConfigInt(_T("mode"),3);
	pDlg->m_mode_Gain = ReadConfigInt(_T("mode_Gain"),3);
	pDlg->m_AddressH=ReadConfigString(_T("AddressH"),_T("0x0202"));
	pDlg->m_AddressL=ReadConfigString(_T("AddressL"),_T("0x0203"));
	pDlg->m_GainReg1 = ReadConfigString(_T("GainReg1"),_T("0x0204"));
	pDlg->m_GainReg2 = ReadConfigString(_T("GainReg2"),_T("0x0205"));
	pDlg->m_GainVal1 = ReadConfigString(_T("GainVal1"),_T("0x00"));
	pDlg->m_GainVal2 = ReadConfigString(_T("GainVal2"),_T("0x20"));
	
	intervalSpec = ReadConfigInt(_T("intervalSpec"),5);
	AddressH = ReadConfigInt(_T("AddressH"),0x0202);
	AddressL=ReadConfigInt(_T("AddressL"),0x0203);

	GainReg1 = ReadConfigInt(_T("GainReg1"),0x0204);
	GainReg2 = ReadConfigInt(_T("GainReg2"),0x0205);
	GainVal1 = ReadConfigInt(_T("GainVal1"),0x00);
	GainVal2 = ReadConfigInt(_T("GainVal2"),0x20);
	mode=ReadConfigInt(_T("mode"),3);
	mode_Gain = ReadConfigInt(_T("mode_Gain"),3);
	
	SlaveID=ReadConfigInt(_T("SlaveID"),32);

	pDlg->m_RemExp=ReadConfigInt(_T("RemExp"),1);
	pDlg->m_DNPExp=ReadConfigInt(_T("DNPExp"),0);

	pDlg->LEDChannel=ReadConfigInt(_T("LED_Channel"),1);
	pDlg->ALight_Channel=ReadConfigInt(_T("ALight_Channel"),1);
	pDlg->TL84LightChannel=ReadConfigInt(_T("TL84LightChannel"),2);
	pDlg->D65LightChannel=ReadConfigInt(_T("D65LightChannel"),3);
	pDlg->m_LEDType=ReadConfigInt(_T("LED_Type"), 0);
	pDlg->m_WaitFlag=ReadConfigInt(_T("m_WaitFlag"), 0);
	pDlg->SetLedEN=ReadConfigInt(_T("SetLedEN"), 0);

	m_LightSource.LAMP_CCT_A.Channel=pDlg->ALight_Channel;
	m_LightSource.LAMP_CCT_TL84.Channel=pDlg->TL84LightChannel;
	m_LightSource.LAMP_CCT_D65.Channel=pDlg->D65LightChannel;
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AE_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	  
	WriteConfigInt(_T("m_count"),pDlg->m_count);
	WriteConfigInt(_T("WhiteEnable"),pDlg->m_WhiteEnable);
	WriteConfigInt(_T("White128Enable"),pDlg->m_White128Enable);

	WriteConfigInt(_T("RemExp"),pDlg->m_RemExp);
	WriteConfigInt(_T("DNPExp"),pDlg->m_DNPExp);

	WriteConfigInt(_T("HighLimit"),pDlg->m_HighLimit);
	WriteConfigInt(_T("LowLimit"),pDlg->m_LowLimit);
	WriteConfigInt(_T("Ratio"),pDlg->m_Ratio);
	WriteConfigInt(_T("BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("count"),pDlg->m_excount);
	WriteConfigInt(_T("InitExposure"),pDlg->m_InitExposure);
	WriteConfigString(_T("SlaveID"),pDlg->m_SlaveID);
	WriteConfigString(_T("AE_TARGET_STRING"), pDlg->m_aeTargetString);
	WriteConfigInt(_T("ErrorCode"),pDlg->ErrorCode);
	WriteConfigInt(_T("mode"),pDlg->m_mode);
	WriteConfigInt(_T("mode_Gain"),pDlg->m_mode_Gain);
	WriteConfigString(_T("AddressH"), pDlg->m_AddressH);
	WriteConfigString(_T("AddressL"),pDlg->m_AddressL);

	WriteConfigString(_T("GainReg1"), pDlg->m_GainReg1);
	WriteConfigString(_T("GainReg2"),pDlg->m_GainReg2);
	WriteConfigString(_T("GainVal1"), pDlg->m_GainVal1);
	WriteConfigString(_T("GainVal2"),pDlg->m_GainVal2);

	WriteConfigInt(_T("intervalSpec"),intervalSpec);

	WriteConfigInt(_T("m_10bit"),pDlg->m_10bit);
	WriteConfigInt(_T("m_is4Cell"),pDlg->m_is4Cell);
	WriteConfigInt(_T("is2PD"),pDlg->m_is2PD);
	WriteConfigInt(_T("Ratio2"),pDlg->m_Ratio2);
	WriteConfigInt(_T("StartX"),pDlg->m_StartX);
	WriteConfigInt(_T("StartY"),pDlg->m_StartY);

	//........................
	//切换光源
	WriteConfigInt(_T("LED_Channel"), pDlg->LEDChannel);
	WriteConfigInt(_T("LED_Type"), pDlg->m_LEDType);
	WriteConfigInt(_T("ALight_Channel"), pDlg->ALight_Channel);
	WriteConfigInt(_T("TL84LightChannel"), pDlg->TL84LightChannel);
	WriteConfigInt(_T("D65LightChannel"), pDlg->D65LightChannel);
	WriteConfigInt(_T("m_WaitFlag"), pDlg->m_WaitFlag);
	WriteConfigInt(_T("SetLedEN"), pDlg->SetLedEN);
	LoadOption();
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR AE_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header = _T("brightness");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR AE_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	LowerLimit.Format(_T("%d"), pDlg->m_LowLimit);
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR AE_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	UpperLimit.Format(_T("%d"), pDlg->m_HighLimit);
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR AE_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	Content.Format(_T("%.2f"), brightness);
	return Content;
}

void AE_TestItem::ShowROIInfo()
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//得到系统默认字体
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//初始化
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);

	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	int cx1, cy1, cx2, cy2;
	int px1, py1, px2, py2;
	
	CurrentColor = COLOR_YELLOW;
	CPen pen(PS_SOLID,2,CurrentColor);
	pDisplayWndMemDC->SelectObject(&pen);

	cx1 = (1 - 1.0/pDlg->m_Ratio)*pDlg->m_StartX*ImgWidth/100;
	cy1 =  (1 - 1.0/pDlg->m_Ratio2)*pDlg->m_StartY*ImgHeight/100;
	px1 = (int)(cx1* Xratio);
	py1 = (int)(cy1* Yratio);

	cx2 = (1 + 1.0/pDlg->m_Ratio)*pDlg->m_StartX*ImgWidth/100;
	cy2 = (1 + 1.0/pDlg->m_Ratio2)*pDlg->m_StartY*ImgHeight/100;
	px2 = (int)(cx2* Xratio);
	py2 = (int)(cy2* Yratio);
	
	pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);

	CString str = _T("");
	if ((brightness>pDlg->m_HighLimit)||(brightness<pDlg->m_LowLimit))
	{
		CurrentColor = COLOR_RED;
	}
	else
	{
		CurrentColor = COLOR_BLUE;
	}

	pDisplayWndMemDC->SetTextColor(CurrentColor);
	str.Format(_T("brightness = %.2f"), brightness);
	pDisplayWndMemDC->TextOut(pDlg->m_StartX*DisplayWndRect.Width()/100,pDlg->m_StartY*DisplayWndRect.Height()/100,str);

	m_pInterface->Ctrl_ShowMemDC(CamID);
	return ;
}

void AE_TestItem::CenterBlockRawInfo(USHORT *pIn,ImageInfo *mySensor ,int Ratio,int Ratio2,float &RawG,BOOL WhiteEnable)
{
	int  PixW   = mySensor->imgwidth;
	int  PixH   =   mySensor->imgheight;

	//calculate rawWidth & rawHeight  , size of ROI 
	int    rawWidth  = 0;    //multiply two is to make sure that width & height is even  because of bayer order
	int    rawHeight = 0;

	if (Ratio>100 && Ratio2>100)
	{
		rawWidth = Ratio;
		rawHeight = Ratio2;
	}
	else
	{
		rawWidth = PixW/Ratio;
		rawHeight = PixH/Ratio2;
	}

	int  StartX = PixW*pDlg->m_StartX/100 - rawWidth/2;// (PixW  - rawWidth ) / 4 * 2;
	int  StartY = PixH*pDlg->m_StartY/100 - rawHeight/2;// (PixH  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

	StartX = StartX - StartX%4;
	StartY = StartY - StartY%4;

	EndX = EndX - EndX%4;
	EndY = EndY - EndY%4;

	BYTE outformat=mySensor->RawFormat;

	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1  = 0, Pos2 = 0;

	int  SumGr = 0;
	double	Gr = 0.0;

	int  SumGb = 0;
	double Gb = 0.0;

	int  SumR  = 0;
	double R  = 0.0;

	int  SumB  = 0;
	double B  = 0.0;

	if (pDlg->m_White128Enable)
	{
		RawG = 128;
		SumGr=0;
		SumGb=0;
		Count=0;
		if ( outformat== 0)//RGGB
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					int AveG  = int((pIn[Pos1 + 1]+pIn[Pos2]) / 2.0); 
					if(AveG>RawG)
					{
						SumGr += pIn[Pos1 + 1];
						SumGb += pIn[Pos2];
						Count++;
					}

				}
			}
		}

		if (outformat == 1)//GRBG
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					int AveG  = int((pIn[Pos1]+pIn[Pos2 + 1]) / 2.0); 
					if(AveG>RawG)
					{
						SumGr += pIn[Pos1];
						SumGb += pIn[Pos2 + 1];
						Count++;
					}
				}
			}
		}

		if (outformat == 2)//GBRG
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG  = int((pIn[Pos1]+pIn[Pos2 + 1]) / 2.0); 
					if(AveG>RawG)
					{
						SumGr += pIn[Pos2 + 1];
						SumGb += pIn[Pos1];
						Count++;
					}
				}
			}
		}

		if (outformat == 3)//BGGR
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG  = int((pIn[Pos1 + 1]+pIn[Pos2]) / 2.0); 
					if(AveG>RawG)
					{
						SumGr += pIn[Pos1 + 1];
						SumGb +=pIn[Pos2];
						Count++;
					}
				}
			}
		}
		if(Count==0) Count=1;
		Gr = 1.0*SumGr / Count;
		Gb = 1.0*SumGb / Count;
		RawG  = int((Gr + Gb) / 2.0); 

		OutputDebugString(_T("测试大于128的pixel..."));
		return;
	}

	if ( outformat== 0)//RGGB
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumR  += pIn[Pos1];
				SumGr += pIn[Pos1 + 1];
				SumGb += pIn[Pos2];
				SumB  += pIn[Pos2 + 1];
				Count++;
			}
		}
	}

	if (outformat == 1)//GRBG
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += pIn[Pos1];
				SumR  += pIn[Pos1 + 1];
				SumB  += pIn[Pos2];
				SumGb += pIn[Pos2 + 1]; 

				Count++;
			}
		}
	}

	if (outformat == 2)//GBRG
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGb += pIn[Pos1];
				SumB  += pIn[Pos1 + 1];
				SumR  += pIn[Pos2];
				SumGr += pIn[Pos2 + 1];

				Count++;
			}
		}
	}
	
	if (outformat == 3)//BGGR
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumB  += pIn[Pos1];
				SumGb += pIn[Pos1 + 1];
				SumGr += pIn[Pos2];
				SumR  += pIn[Pos2 + 1];

				Count++;
			}
		}
	}

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;

	if (pDlg->m_aeTargetString == _T("R_Target"))
		RawG = R;
	else if (pDlg->m_aeTargetString == _T("B_Target"))
		RawG = B;
	else if (pDlg->m_aeTargetString == _T("Gr_Target"))
		RawG = Gr;
	else if (pDlg->m_aeTargetString == _T("Gb_Target"))
		RawG = Gb;
	else if (pDlg->m_aeTargetString == _T("Y_Target"))
		RawG = (R * 299 + ((Gr + Gb) / 2 + 0.5) * 587 + B * 114) / 1000;
	else
		RawG  = (Gr + Gb) / 2.0; 

	OutputDebugString(_T("测试所有pixel平均值..."));

	if (WhiteEnable)
	{
		SumGr=0;
		SumGb=0;
		Count=0;
		if ( outformat== 0)//RGGB
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					int AveG  = int((pIn[Pos1 + 1]+pIn[Pos2]) / 2.0); 
					if(AveG>RawG)
					{
						SumGr += pIn[Pos1 + 1];
						SumGb += pIn[Pos2];
						Count++;
					}
					
				}
			}
		}

		if (outformat == 1)//GRBG
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					int AveG  = int((pIn[Pos1]+pIn[Pos2 + 1]) / 2.0); 
					if(AveG>RawG)
					{
						SumGr += pIn[Pos1];
						SumGb += pIn[Pos2 + 1];
						Count++;
					}
				}
			}
		}

		if (outformat == 2)//GBRG
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG  = int((pIn[Pos1]+pIn[Pos2 + 1]) / 2.0); 
					if(AveG>RawG)
					{
						SumGr += pIn[Pos2 + 1];
						SumGb += pIn[Pos1];
						Count++;
					}
				}
			}
		}

		if (outformat == 3)//BGGR
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG  = int((pIn[Pos1 + 1]+pIn[Pos2]) / 2.0); 
					if(AveG>RawG)
					{
						SumGr += pIn[Pos1 + 1];
						SumGb +=pIn[Pos2];
						Count++;
					}
				}
			}
		}
		if(Count==0) Count=1;
		Gr = 1.0*SumGr / Count;
		Gb = 1.0*SumGb / Count;
		RawG  = int((Gr + Gb) / 2.0); 

		OutputDebugString(_T("测试大于平均值的pixel..."));
	}
}

void AE_TestItem::CenterBlockRawInfo4Cell(USHORT *pIn,ImageInfo *mySensor ,int Ratio,int Ratio2,float &RawG,BOOL WhiteEnable)
{
	int  PixW   = mySensor->imgwidth;
	int  PixH   =   mySensor->imgheight;

	//calculate rawWidth & rawHeight  , size of ROI 
	int    rawWidth  = 0;    //multiply two is to make sure that width & height is even  because of bayer order
	int    rawHeight = 0;

	if (Ratio>100 && Ratio2>100)
	{
		rawWidth = Ratio;
		rawHeight = Ratio2;
	}
	else
	{
		rawWidth = PixW/Ratio;
		rawHeight = PixH/Ratio2;
	}

	int  StartX = PixW*pDlg->m_StartX/100 - rawWidth/2;// (PixW  - rawWidth ) / 4 * 2;
	int  StartY = PixH*pDlg->m_StartY/100 - rawHeight/2;// (PixH  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

	StartX = StartX - StartX%4;
	StartY = StartY - StartY%4;

	EndX = EndX - EndX%4;
	EndY = EndY - EndY%4;

	BYTE outformat=mySensor->RawFormat;

	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1  = 0, Pos2 = 0, Pos3 = 0, Pos4 = 0;

	int  SumGr = 0;
	double	Gr = 0.0;

	int  SumGb = 0;
	double Gb = 0.0;

	int  SumR  = 0;
	double R  = 0.0;

	int  SumB  = 0;
	double B  = 0.0;

	if (pDlg->m_White128Enable)
	{
		RawG = 128;
		SumGr=0;
		SumGb=0;
		Count=0;
		if ( outformat== 0)//RGGB
		{
			for (y = StartY; y < EndY; y += 4)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					Pos3 = x + PixW * (y + 2);
					Pos4 = x + PixW * (y + 3);
					int  AveG=int((pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3]+pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/8.0);
					if(AveG>RawG)
					{
						SumGr += (pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3])/4;
						SumGb += (pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/4;
						Count++;
					}

				}
			}
		}

		if (outformat == 1)//GRBG
		{
			for (y = StartY; y < EndY; y += 4)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					Pos3 = x + PixW * (y + 2);
					Pos4 = x + PixW * (y + 3);
					int AveG  = int((pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1]+pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3]) / 8.0); 
					if(AveG>RawG)
					{
						SumGr  += (pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1])/4;
						SumGb  += (pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3])/4;
						Count++;
					}
				}
			}
		}

		if (outformat == 2)//GBRG
		{
			for (y = StartY; y < EndY; y += 4)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					Pos3 = x + PixW * (y + 2);
					Pos4 = x + PixW * (y + 3);
					int AveG  = int((pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1]+pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3]) / 8.0); 
					if(AveG>RawG)
					{
						SumGb  += (pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1])/4;
						SumGr  += (pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3])/4;
						Count++;
					}
				}
			}
		}

		if (outformat == 3)//BGGR
		{
			for (y = StartY; y < EndY; y += 4)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					Pos3 = x + PixW * (y + 2);
					Pos4 = x + PixW * (y + 3);

					int  AveG=int((pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3]+pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/8.0);
					if(AveG>RawG)
					{
						SumGb += (pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3])/4;
						SumGr += (pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/4;
						Count++;
					}
				}
			}
		}
		if(Count==0) Count=1;
		Gr = 1.0*SumGr / Count;
		Gb = 1.0*SumGb / Count;
		RawG  = int((Gr + Gb) / 2.0); 

		OutputDebugString(_T("测试大于128的pixel..."));
		return;
	}


	if ( outformat== 0)
	{
		for (y = StartY; y < EndY; y += 4)
		{
			for (x = StartX; x < EndX; x += 4)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);
				Pos3 = x + PixW * (y + 2);
				Pos4 = x + PixW * (y + 3);

				SumR  += (pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1])/4;
				SumGr += (pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3])/4;
				SumGb += (pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/4;
				SumB  += (pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3])/4;
				Count++;
			}
		}
	}

	if (outformat == 1)
	{
		for (y = StartY; y < EndY; y += 4)
		{
			for (x = StartX; x < EndX; x += 4)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);
				Pos3 = x + PixW * (y + 2);
				Pos4 = x + PixW * (y + 3);

				SumGr  += (pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1])/4;
				SumR += (pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3])/4;
				SumB += (pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/4;
				SumGb  += (pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3])/4;

				Count++;
			}
		}
	}

	if (outformat == 2)
	{
		for (y = StartY; y < EndY; y += 4)
		{
			for (x = StartX; x < EndX; x += 4)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);
				Pos3 = x + PixW * (y + 2);
				Pos4 = x + PixW * (y + 3);

				SumGb  += (pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1])/4;
				SumB += (pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3])/4;
				SumR += (pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/4;
				SumGr  += (pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3])/4;

				Count++;
			}
		}
	}
	//for example :OV8865   outformat  =OUTFORMAT_BGGR
	if (outformat == 3)
	{
		for (y = StartY; y < EndY; y += 4)
		{
			for (x = StartX; x < EndX; x += 4)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);
				Pos3 = x + PixW * (y + 2);
				Pos4 = x + PixW * (y + 3);

				SumB  += (pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1])/4;
				SumGb += (pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3])/4;
				SumGr += (pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/4;
				SumR  += (pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3])/4;

				Count++;
				//so,OV8865 run to here
			}
		}
	}

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;

	if (pDlg->m_aeTargetString == _T("R_Target"))
		RawG = R;
	else if (pDlg->m_aeTargetString == _T("B_Target"))
		RawG = B;
	else if (pDlg->m_aeTargetString == _T("Gr_Target"))
		RawG = Gr;
	else if (pDlg->m_aeTargetString == _T("Gb_Target"))
		RawG = Gb;
	else if (pDlg->m_aeTargetString == _T("Y_Target"))
		RawG = (R * 299 + ((Gr + Gb) / 2 + 0.5) * 587 + B * 114) / 1000;
	else
		RawG  = (Gr + Gb) / 2.0; 

	OutputDebugString(_T("测试所有pixel平均值..."));

	if (WhiteEnable)
	{
		SumGr=0;
		SumGb=0;
		Count=0;
		if ( outformat== 0)//RGGB
		{
			for (y = StartY; y < EndY; y += 4)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					Pos3 = x + PixW * (y + 2);
					Pos4 = x + PixW * (y + 3);
					int  AveG=int((pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3]+pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/8.0);
					if(AveG>RawG)
					{
						SumGr += (pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3])/4;
						SumGb += (pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/4;
						Count++;
					}

				}
			}
		}

		if (outformat == 1)//GRBG
		{
			for (y = StartY; y < EndY; y += 4)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					Pos3 = x + PixW * (y + 2);
					Pos4 = x + PixW * (y + 3);
					int AveG  = int((pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1]+pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3]) / 8.0); 
					if(AveG>RawG)
					{
						SumGr  += (pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1])/4;
						SumGb  += (pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3])/4;
						Count++;
					}
				}
			}
		}

		if (outformat == 2)//GBRG
		{
			for (y = StartY; y < EndY; y += 4)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					Pos3 = x + PixW * (y + 2);
					Pos4 = x + PixW * (y + 3);
					int AveG  = int((pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1]+pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3]) / 8.0); 
					if(AveG>RawG)
					{
						SumGb  += (pIn[Pos1]+pIn[Pos1+1]+pIn[Pos2]+pIn[Pos2+1])/4;
						SumGr  += (pIn[Pos3+2]+pIn[Pos3+3]+pIn[Pos4+2]+pIn[Pos4+3])/4;
						Count++;
					}
				}
			}
		}

		if (outformat == 3)//BGGR
		{
			for (y = StartY; y < EndY; y += 4)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					Pos3 = x + PixW * (y + 2);
					Pos4 = x + PixW * (y + 3);

					int  AveG=int((pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3]+pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/8.0);
					if(AveG>RawG)
					{
						SumGb += (pIn[Pos1 + 2]+pIn[Pos1+3]+pIn[Pos2+2]+pIn[Pos2+3])/4;
						SumGr += (pIn[Pos3]+pIn[Pos3+1]+pIn[Pos4]+pIn[Pos4+1])/4;
						Count++;
					}
				}
			}
		}
		if(Count==0) Count=1;
		Gr = 1.0*SumGr / Count;
		Gb = 1.0*SumGb / Count;
		RawG  = int((Gr + Gb) / 2.0); 

		OutputDebugString(_T("测试大于平均值的pixel..."));
	}
}

void AE_TestItem::CenterBlockRawInfo2PD(USHORT * pIn, ImageInfo * mySensor, int Ratio, int Ratio2, float & RawG, bool WhiteEnable)
{
	int  PixW   = mySensor->imgwidth;
	int  PixH   =   mySensor->imgheight;

	//calculate rawWidth & rawHeight  , size of ROI 
	int    rawWidth  = 0;    //multiply two is to make sure that width & height is even  because of bayer order
	int    rawHeight = 0;

	if (Ratio>100 && Ratio2>100)
	{
		rawWidth = Ratio;
		rawHeight = Ratio2;
	}
	else
	{
		rawWidth = PixW/Ratio;
		rawHeight = PixH/Ratio2;
	}

	int  StartX = PixW*pDlg->m_StartX/100 - rawWidth/2;// (PixW  - rawWidth ) / 4 * 2;
	int  StartY = PixH*pDlg->m_StartY/100 - rawHeight/2;// (PixH  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

	StartX = StartX - StartX%4;
	StartY = StartY - StartY%4;

	EndX = EndX - EndX%4;
	EndY = EndY - EndY%4;

	BYTE outformat=mySensor->RawFormat;

	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1  = 0, Pos2 = 0, Pos3 = 0, Pos4 = 0;

	int  SumGr = 0;
	double	Gr = 0.0;

	int  SumGb = 0;
	double Gb = 0.0;

	int  SumR  = 0;
	double R  = 0.0;

	int  SumB  = 0;
	double B  = 0.0;

	int pos = 0;

 	USHORT *pData = new USHORT[rawWidth*rawHeight];
	if (NULL == pData)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("pData Buffer 申请失败!"),COLOR_RED,200);
		return;
	}
	memset(pData,0,rawWidth*rawHeight*sizeof(USHORT));

	for (y=StartY; y<EndY; y++)
	{
		for (x=StartX; x<EndX; x++)
		{
			pos = x+PixW*y;
			pData[Count++] = pIn[pos];
		}
	}

	PixW   = rawWidth;
	PixH   = rawHeight;
	StartX = 0;
	StartY = 0;

	EndX = StartX + rawWidth-1;
	EndY = StartY + rawHeight-1;
	Count = 0;

	if (pDlg->m_White128Enable)
	{
		RawG = 128;
		SumGr=0;
		SumGb=0;
		Count=0;
		if ( outformat== 0)//RGGB
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG = (pData[Pos1 + 2] + pData[Pos1 + 3]+pData[Pos2]+pData[Pos2+1])/4.0;
					if (AveG > RawG)
					{
		//				SumR  += (pData[Pos1]+pData[Pos1 + 1]);
						SumGr += (pData[Pos1 + 2] + pData[Pos1 + 3])/2.0;
						SumGb += (pData[Pos2]+pData[Pos2+1])/2.0;
		//				SumB  += (pData[Pos2 + 2]+pData[Pos2 + 3]);
						Count++;
					}
				}
			}
		}

		if (outformat == 1)//GRBG
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					int AveG = (pData[Pos1]+pData[Pos1+1]+pData[Pos2 + 2]+pData[Pos2+3])/4.0;
					if (AveG > RawG)
					{
						SumGr += (pData[Pos1]+pData[Pos1+1])/2.0;
			//			SumR  += (pData[Pos1 + 2]+pData[Pos1+3]);
			//			SumB  += (pData[Pos2]+pData[Pos2+1]);
						SumGb += (pData[Pos2 + 2]+pData[Pos2+3])/2.0; 

						Count++;
					}
				}
			}
		}

		if (outformat == 2)//GBRG
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG = (pData[Pos1]+ pData[Pos2 + 1])/2.0;
					if (AveG > RawG)
					{
						SumGb += pData[Pos1];
			//			SumB  += pData[Pos1 + 1];
			//			SumR  += pData[Pos2];
						SumGr += pData[Pos2 + 1];

						Count++;
					}
				}
			}
		}

		if (outformat == 3)//BGGR
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG = (pData[Pos1 + 1] + pData[Pos2])/2.0;
					if (AveG > RawG)
					{
		//				SumB  += pData[Pos1];
						SumGb += pData[Pos1 + 1];
						SumGr += pData[Pos2];
		//				SumR  += pData[Pos2 + 1];

						Count++;
					}
				}
			}
		}
		if(Count==0) Count=1;
		Gr = 1.0*SumGr / Count;
		Gb = 1.0*SumGb / Count;
		RawG  = int((Gr + Gb) / 2.0); 

		SAFE_DELETE_ARRAY(pData);

		OutputDebugString(_T("测试大于128的pixel..."));
		return;
	}


	if ( outformat== 0)//RGGB
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 4)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumR  += (pData[Pos1]+pData[Pos1 + 1])/2.0;
				SumGr += (pData[Pos1 + 2] + pData[Pos1 + 3])/2.0;
				SumGb += (pData[Pos2]+pData[Pos2+1])/2.0;
				SumB  += (pData[Pos2 + 2]+pData[Pos2 + 3])/2.0;
				Count++;
			}
		}
	}

	if (outformat == 1)//GRBG
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 4)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);
			
				SumGr += (pData[Pos1]+pData[Pos1+1])/2.0;
				SumR  += (pData[Pos1 + 2]+pData[Pos1+3])/2.0;
				SumB  += (pData[Pos2]+pData[Pos2+1])/2.0;
				SumGb += (pData[Pos2 + 2]+pData[Pos2+3])/2.0; 

				Count++;
			}
		}
	}

	if (outformat == 2)//GBRG
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGb += pData[Pos1];
				SumB  += pData[Pos1 + 1];
				SumR  += pData[Pos2];
				SumGr += pData[Pos2 + 1];

				Count++;
			}
		}
	}

	if (outformat == 3)//BGGR
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumB  += pData[Pos1];
				SumGb += pData[Pos1 + 1];
				SumGr += pData[Pos2];
				SumR  += pData[Pos2 + 1];

				Count++;
			}
		}
	}

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;

	if (pDlg->m_aeTargetString == _T("R_Target"))
		RawG = R;
	else if (pDlg->m_aeTargetString == _T("B_Target"))
		RawG = B;
	else if (pDlg->m_aeTargetString == _T("Gr_Target"))
		RawG = Gr;
	else if (pDlg->m_aeTargetString == _T("Gb_Target"))
		RawG = Gb;
	else if (pDlg->m_aeTargetString == _T("Y_Target"))
		RawG = (R * 299 + ((Gr + Gb) / 2 + 0.5) * 587 + B * 114) / 1000;
	else
		RawG  = (Gr + Gb) / 2.0; 

	OutputDebugString(_T("测试所有pixel平均值..."));

	if (WhiteEnable)
	{
		SumGr=0;
		SumGb=0;
		Count=0;
		
		if ( outformat== 0)//RGGB
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG = (pData[Pos1 + 2] + pData[Pos1 + 3]+pData[Pos2]+pData[Pos2+1])/4.0;
					if (AveG > RawG)
					{
						SumGr += (pData[Pos1 + 2] + pData[Pos1 + 3])/2.0;
						SumGb += (pData[Pos2]+pData[Pos2+1])/2.0;

						Count++;
					}
				}
			}
		}

		if (outformat == 1)//GRBG
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 4)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);
					int AveG = (pData[Pos1]+pData[Pos1+1]+pData[Pos2 + 2]+pData[Pos2+3])/4.0;
					if (AveG > RawG)
					{
						SumGr += (pData[Pos1]+pData[Pos1+1])/2.0;
						SumGb += (pData[Pos2 + 2]+pData[Pos2+3])/2.0; 

						Count++;
					}
				}
			}
		}

		if (outformat == 2)//GBRG
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG = (pData[Pos1]+ pData[Pos2 + 1])/2.0;
					if (AveG > RawG)
					{
						SumGb += pData[Pos1];
						SumGr += pData[Pos2 + 1];

						Count++;
					}
				}
			}
		}

		if (outformat == 3)//BGGR
		{
			for (y = StartY; y < EndY; y += 2)
			{
				for (x = StartX; x < EndX; x += 2)
				{
					Pos1 = x + PixW * y;
					Pos2 = x + PixW * (y + 1);

					int AveG = (pData[Pos1 + 1] + pData[Pos2])/2.0;
					if (AveG > RawG)
					{
						SumGb += pData[Pos1 + 1];
						SumGr += pData[Pos2];

						Count++;
					}
				}
			}
		}
		
		if(Count==0) Count=1;
		Gr = 1.0*SumGr / Count;
		Gb = 1.0*SumGb / Count;
		RawG  = int((Gr + Gb) / 2.0); 

		OutputDebugString(_T("测试大于平均值的pixel..."));
	}
	SAFE_DELETE_ARRAY(pData);
}
