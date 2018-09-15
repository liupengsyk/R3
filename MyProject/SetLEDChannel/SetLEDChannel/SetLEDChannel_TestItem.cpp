#include "StdAfx.h"
#include "SetLEDChannel_TestItem.h"
#include "ImageProc.h"




#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new SetLEDChannel_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
SetLEDChannel_TestItem::SetLEDChannel_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化
	LEDInitFlag=FALSE;

	//.....
}

SetLEDChannel_TestItem::~SetLEDChannel_TestItem(void)
{

}


int SetLEDChannel_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	CurrentName=GetName();
	LoadOption(); 

	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SetLEDChannel_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码

	CString  GlobalSetting=_T("WBSetting");
	SetName(GlobalSetting);	
	GlobalTime=ReadConfigInt(_T("GlobalTime"), 3);
	SetName(CurrentName);
	imageflag=0;
//	m_pInterface->SetLEDChannel(0,pDlg->m_LEDType);
	return 0;
}
void SetLEDChannel_TestItem::CenterBlockRawInfo(ImageInfo *mySensor ,int Ratio,WBInf &MyWbInf,int index,int BLC)
{
	USHORT *pIn=new USHORT[mySensor->imgwidth*mySensor->imgheight];
	memset(pIn,0,mySensor->imgwidth*mySensor->imgheight);

	for (int k=0;k<mySensor->imgwidth*mySensor->imgheight;k++)
	{
		pIn[k]=(mySensor->Raw_buffer[k]>>2);
	}

	int  PixW   = mySensor->imgwidth;
	int  PixH   =   mySensor->imgheight;
	//calculate rawWidth & rawHeight  , size of ROI 
	int    rawWidth  = PixW / Ratio;    //multiply two is to make sure that width & height is even  because of bayer order
	int    rawHeight = PixH / Ratio; 

	int  StartX = (PixW  - rawWidth ) / 4 * 2;
	int  StartY = (PixH  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

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
	delete []pIn;

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;
	MyWbInf.Gr = int(Gr);
	MyWbInf.Gb = int(Gb);
	MyWbInf.R  = int(R);
	MyWbInf.B  = int(B);
	MyWbInf.RG = int( (float)index*(MyWbInf.R - BLC)*2.0/(float)((Gr + Gb) - 2*BLC) + 0.5  );// Rave/Gave
	MyWbInf.BG = int( (float)index*(MyWbInf.B - BLC)*2.0/(float)((Gr + Gb) - 2*BLC ) + 0.5 );  //Bave/Gave 
	MyWbInf.GG =int( (float)index*(MyWbInf.Gb- BLC)/(float)(MyWbInf.Gr-BLC)+0.5);

}
BOOL SetLEDChannel_TestItem::ChangeLEDLSChannel(int LEDType,HiStarOTP_LampType lamptype)
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
	Sleep(pDlg->m_SleepTime);

    return Result;

}


/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SetLEDChannel_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	if (imageflag==0)
	{
		if (pDlg->m_CheckEN)
		{
			if (GlobalTime<pDlg->Checknumber || GlobalTime>pDlg->Checknumber+3)
			{
				SetResult(RESULT_PASS);
				return 0;
			}
			else
			{
				if (!ChangeLEDLSChannel(pDlg->m_LEDType,HiStarOTP_LampType(pDlg->LEDChannel)))
				{
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Fail!"));
					SetResult(RESULT_FAIL);
					Sleep(pDlg->m_SleepTime);
					return 0;
				}
				imageflag=1;	
			}
		}
		else
		{

			if (!ChangeLEDLSChannel(pDlg->m_LEDType,HiStarOTP_LampType(pDlg->LEDChannel)))
			{
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Fail!"));
				SetResult(RESULT_FAIL);
				return 0;
			}
			else
			{
				imageflag=1;
				return 1;
			}
			
		}
	}
	else
	{
		if(imageflag>3)
		{
			if (!ImageVerify(HiStarOTP_LampType(pDlg->LEDChannel)))
			{
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Fail!"));
				SetResult(RESULT_FAIL);
				return 0;
			}
			else
			{
				SetResult(RESULT_PASS);
				Sleep(pDlg->m_SleepTime);
				return 0;
			}
		}
		else
			{
				imageflag++;
				return 1;
		}
	
	}
    return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SetLEDChannel_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	//m_pInterface->SetLEDChannel(CamID,6,pDlg->m_LEDType,pDlg->m_WaitFlag);
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int SetLEDChannel_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(CurrentName);
	pDlg->m_BLC=ReadConfigInt(_T("LED_BLC"),16);
	pDlg->LEDChannel=ReadConfigInt(_T("LED_Channel"),1);
	pDlg->m_LEDCOM=ReadConfigInt(_T("LED_Type"),16);
	pDlg->m_SleepTime=ReadConfigInt(_T("LED_SleepTime"),1);
	pDlg->ALight_Channel=ReadConfigInt(_T("ALight_Channel"),1);
	pDlg->TL84LightChannel=ReadConfigInt(_T("TL84LightChannel"),2);
	pDlg->D65LightChannel=ReadConfigInt(_T("D65LightChannel"),3);
	pDlg->m_LEDType=ReadConfigInt(_T("LED_Type"), 0);
	pDlg->m_WaitFlag=ReadConfigInt(_T("m_WaitFlag"), 0);
	m_LightSource.LAMP_CCT_A.Channel=pDlg->ALight_Channel;
	m_LightSource.LAMP_CCT_TL84.Channel=pDlg->TL84LightChannel;
	m_LightSource.LAMP_CCT_D65.Channel=pDlg->D65LightChannel;

	pDlg->m_CheckEN=ReadConfigInt(_T("m_CheckEN"),0);
	pDlg->Checknumber=ReadConfigInt(_T("Checknumber"),3);

	m_LightSource.LAMP_CCT_A.RGainHigh=ReadConfigInt(_T("3100K_RGainHigh"),0);//GetPrivateProfileInt( "OTP","3100K_RGainHigh",  400,   m_szSetup );
	m_LightSource.LAMP_CCT_A.RGainLow=ReadConfigInt(_T("3100K_RGainLow"),0);//GetPrivateProfileInt( "OTP","3100K_RGainLow",  200,   m_szSetup );
	m_LightSource.LAMP_CCT_A.BGainHigh=ReadConfigInt(_T("3100K_BGainHigh"),0);//GetPrivateProfileInt( "OTP","3100K_BGainHigh",  400,   m_szSetup );
	m_LightSource.LAMP_CCT_A.BGainLow=ReadConfigInt(_T("3100K_BGainLow"),0);//GetPrivateProfileInt( "OTP","3100K_BGainLow",  200,   m_szSetup );
	m_LightSource.LAMP_CCT_TL84.RGainHigh=ReadConfigInt(_T("5100K_RGainHigh"),0);//GetPrivateProfileInt( "OTP","5100K_RGainHigh",  300,   m_szSetup );
	m_LightSource.LAMP_CCT_TL84.RGainLow=ReadConfigInt(_T("5100K_RGainLow"),0);//GetPrivateProfileInt( "OTP","5100K_RGainLow",  200,   m_szSetup );
	m_LightSource.LAMP_CCT_TL84.BGainHigh=ReadConfigInt(_T("5100K_BGainHigh"),0);//GetPrivateProfileInt( "OTP","5100K_BGainHigh",  400,   m_szSetup );
	m_LightSource.LAMP_CCT_TL84.BGainLow=ReadConfigInt(_T("5100K_BGainLow"),0);//GetPrivateProfileInt( "OTP","5100K_BGainLow",  300,   m_szSetup );
	m_LightSource.LAMP_CCT_D65.RGainHigh=ReadConfigInt(_T("2800K_RGainHigh"),0);//GetPrivateProfileInt( "OTP","5100K_RGainHigh",  300,   m_szSetup );
	m_LightSource.LAMP_CCT_D65.RGainLow=ReadConfigInt(_T("2800K_RGainLow"),0);//GetPrivateProfileInt( "OTP","5100K_RGainLow",  200,   m_szSetup );
	m_LightSource.LAMP_CCT_D65.BGainHigh=ReadConfigInt(_T("2800K_BGainHigh"),0);//GetPrivateProfileInt( "OTP","5100K_BGainHigh",  400,   m_szSetup );
	m_LightSource.LAMP_CCT_D65.BGainLow=ReadConfigInt(_T("2800K_BGainLow"),0);//GetPrivateProfileInt( "OTP","5100K_BGainLow",  300,   m_szSetup );

	pDlg->m_ARGHigh=m_LightSource.LAMP_CCT_A.RGainHigh;
	pDlg->m_ARGLow=m_LightSource.LAMP_CCT_A.RGainLow;
	pDlg->m_ABGHigh=m_LightSource.LAMP_CCT_A.BGainHigh;
	pDlg->m_ABGLow=m_LightSource.LAMP_CCT_A.BGainLow;
	pDlg->m_TLRGHigh=m_LightSource.LAMP_CCT_TL84.RGainHigh;
	pDlg->m_TLRGLow=m_LightSource.LAMP_CCT_TL84.RGainLow;
	pDlg->m_TLBGHigh=m_LightSource.LAMP_CCT_TL84.BGainHigh;
	pDlg->m_TLBGLow=m_LightSource.LAMP_CCT_TL84.BGainLow;
	pDlg->m_DRGHigh=m_LightSource.LAMP_CCT_D65.RGainHigh;
	pDlg->m_DRGLow=m_LightSource.LAMP_CCT_D65.RGainLow;
	pDlg->m_DBGHigh=m_LightSource.LAMP_CCT_D65.BGainHigh;
	pDlg->m_DBGLow=m_LightSource.LAMP_CCT_D65.BGainLow;

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int SetLEDChannel_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(CurrentName);
	WriteConfigInt(_T("LED_BLC"), pDlg->m_BLC);
	WriteConfigInt(_T("LED_Channel"), pDlg->LEDChannel);
	WriteConfigInt(_T("LED_Type"), pDlg->m_LEDType);
	WriteConfigInt(_T("LED_SleepTime"), pDlg->m_SleepTime);

	WriteConfigInt(_T("ALight_Channel"), pDlg->ALight_Channel);
	WriteConfigInt(_T("TL84LightChannel"), pDlg->TL84LightChannel);
	WriteConfigInt(_T("D65LightChannel"), pDlg->D65LightChannel);

	WriteConfigInt(_T("m_CheckEN"), pDlg->m_CheckEN);
	WriteConfigInt(_T("Checknumber"), pDlg->Checknumber);

	WriteConfigInt(_T("3100K_RGainHigh"), pDlg->m_ARGHigh);
	WriteConfigInt(_T("3100K_RGainLow"), pDlg->m_ARGLow);
	WriteConfigInt(_T("3100K_BGainHigh"), pDlg->m_ABGHigh);
	WriteConfigInt(_T("3100K_BGainLow"), pDlg->m_ABGLow);
	WriteConfigInt(_T("5100K_RGainHigh"), pDlg->m_TLRGHigh);
	WriteConfigInt(_T("5100K_RGainLow"), pDlg->m_TLRGLow);
	WriteConfigInt(_T("5100K_BGainHigh"), pDlg->m_TLBGHigh);
	WriteConfigInt(_T("5100K_BGainLow"), pDlg->m_TLBGLow);
	WriteConfigInt(_T("2800K_RGainHigh"), pDlg->m_DRGHigh);
	WriteConfigInt(_T("2800K_RGainLow"), pDlg->m_DRGLow);
	WriteConfigInt(_T("2800K_BGainHigh"), pDlg->m_DBGHigh);
	WriteConfigInt(_T("2800K_BGainLow"), pDlg->m_DBGLow);
	WriteConfigInt(_T("m_WaitFlag"), pDlg->m_WaitFlag);
	m_LightSource.LAMP_CCT_A.RGainHigh=ReadConfigInt(_T("3100K_RGainHigh"),0);//GetPrivateProfileInt( "OTP","3100K_RGainHigh",  400,   m_szSetup );
	m_LightSource.LAMP_CCT_A.RGainLow=ReadConfigInt(_T("3100K_RGainLow"),0);//GetPrivateProfileInt( "OTP","3100K_RGainLow",  200,   m_szSetup );
	m_LightSource.LAMP_CCT_A.BGainHigh=ReadConfigInt(_T("3100K_BGainHigh"),0);//GetPrivateProfileInt( "OTP","3100K_BGainHigh",  400,   m_szSetup );
	m_LightSource.LAMP_CCT_A.BGainLow=ReadConfigInt(_T("3100K_BGainLow"),0);//GetPrivateProfileInt( "OTP","3100K_BGainLow",  200,   m_szSetup );
	m_LightSource.LAMP_CCT_TL84.RGainHigh=ReadConfigInt(_T("5100K_RGainHigh"),0);//GetPrivateProfileInt( "OTP","5100K_RGainHigh",  300,   m_szSetup );
	m_LightSource.LAMP_CCT_TL84.RGainLow=ReadConfigInt(_T("5100K_RGainLow"),0);//GetPrivateProfileInt( "OTP","5100K_RGainLow",  200,   m_szSetup );
	m_LightSource.LAMP_CCT_TL84.BGainHigh=ReadConfigInt(_T("5100K_BGainHigh"),0);//GetPrivateProfileInt( "OTP","5100K_2BGainHigh",  400,   m_szSetup );
	m_LightSource.LAMP_CCT_TL84.BGainLow=ReadConfigInt(_T("5100K_BGainLow"),0);//GetPrivateProfileInt( "OTP","5100K_BGainLow",  300,   m_szSetup );
	m_LightSource.LAMP_CCT_D65.RGainHigh=ReadConfigInt(_T("2800K_RGainHigh"),0);//GetPrivateProfileInt( "OTP","5100K_RGainHigh",  300,   m_szSetup );
	m_LightSource.LAMP_CCT_D65.RGainLow=ReadConfigInt(_T("2800K_RGainLow"),0);//GetPrivateProfileInt( "OTP","5100K_RGainLow",  200,   m_szSetup );
	m_LightSource.LAMP_CCT_D65.BGainHigh=ReadConfigInt(_T("2800K_BGainHigh"),0);//GetPrivateProfileInt( "OTP","5100K_BGainHigh",  400,   m_szSetup );
	m_LightSource.LAMP_CCT_D65.BGainLow=ReadConfigInt(_T("2800K_BGainLow"),0);//GetPrivateProfileInt( "OTP","5100K_BGainLow",  300,   m_szSetup );

	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR SetLEDChannel_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR SetLEDChannel_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR SetLEDChannel_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR SetLEDChannel_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}
BOOL SetLEDChannel_TestItem::ImageVerify(HiStarOTP_LampType lamptype)
{
	BOOL Result=TRUE;
	ImageInfo *MyImageInfo = NULL;		
	MyImageInfo = m_pInterface->Ctrl_GetImgInfo(CamID);
	if (MyImageInfo->Raw_buffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("grab image fail!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("grab image fail!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	WBInf MyWBInf;
	memset(&MyWBInf,0,sizeof(WBInf));
	CenterBlockRawInfo(MyImageInfo,10,MyWBInf,512,pDlg->m_BLC);

	CString TempMsg;
	if(lamptype==LAMP_CCT_A)
	{
		int OTP_R_3000K=0;
		int OTP_Gr_3000K=0;
		int OTP_Gb_3000K=0;
		int OTP_B_3000K=0;

		OTP_R_3000K=(MyWBInf.R-pDlg->m_BLC)+0.5;
		OTP_Gr_3000K=(MyWBInf.Gr-pDlg->m_BLC)+0.5;
		OTP_Gb_3000K=(MyWBInf.Gb-pDlg->m_BLC)+0.5;
		OTP_B_3000K=(MyWBInf.B-pDlg->m_BLC)+0.5;
		int RG_fin_3000K = int(512.0*OTP_R_3000K *2.0/(OTP_Gb_3000K+OTP_Gr_3000K) + 0.5);
		int BG_fin_3000K = int(512.0*OTP_B_3000K *2.0/(OTP_Gb_3000K+OTP_Gr_3000K) + 0.5);

		if(((1.0*RG_fin_3000K)/BG_fin_3000K)<0.5)   //防止ofset 张RGain BGain卡控失效
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换A光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}
		if((RG_fin_3000K>m_LightSource.LAMP_CCT_A.RGainHigh)||(RG_fin_3000K<m_LightSource.LAMP_CCT_A.RGainLow))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换A光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}

		else if((BG_fin_3000K>m_LightSource.LAMP_CCT_A.BGainHigh)||(BG_fin_3000K<m_LightSource.LAMP_CCT_A.BGainLow))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换A光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换A光源成功!"),COLOR_BLUE,200);
		}
		TempMsg.Format(_T("RGain:%d, BGain:%d, RGain/BGain < 1.5"),RG_fin_3000K,BG_fin_3000K);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	}
	else if(lamptype==LAMP_CCT_TL84)
	{
		int OTP_R_5100K=0;
		int OTP_Gr_5100K=0;
		int OTP_Gb_5100K=0;
		int OTP_B_5100K=0;

		OTP_R_5100K=(MyWBInf.R-pDlg->m_BLC)+0.5;
		OTP_Gr_5100K=(MyWBInf.Gr-pDlg->m_BLC)+0.5;
		OTP_Gb_5100K=(MyWBInf.Gb-pDlg->m_BLC)+0.5;
		OTP_B_5100K=(MyWBInf.B-pDlg->m_BLC)+0.5;
		int RG_fin_5100K = int(512.0*OTP_R_5100K *2.0/(OTP_Gb_5100K+OTP_Gr_5100K) + 0.5);
		int BG_fin_5100K = int(512.0*OTP_B_5100K *2.0/(OTP_Gb_5100K+OTP_Gr_5100K) + 0.5);

		if(((1.0*RG_fin_5100K)/BG_fin_5100K)<0.5)   //防止ofset 张RGain BGain卡控失效
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换TL84光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}
		if((RG_fin_5100K>m_LightSource.LAMP_CCT_TL84.RGainHigh)||(RG_fin_5100K<m_LightSource.LAMP_CCT_TL84.RGainLow))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换TL84光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}

		if((BG_fin_5100K>m_LightSource.LAMP_CCT_TL84.BGainHigh)||(BG_fin_5100K<m_LightSource.LAMP_CCT_TL84.BGainLow))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换TL84光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换TL84光源成功!"),COLOR_BLUE,200);
		}
		TempMsg.Format(_T("RGain:%d, BGain:%d, RGain/BGain < 1.5"),RG_fin_5100K,BG_fin_5100K);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	}
	else if(lamptype==LAMP_CCT_D65)
	{
		int OTP_R_2800K=0;
		int OTP_Gr_2800K=0;
		int OTP_Gb_2800K=0;
		int OTP_B_2800K=0;

		OTP_R_2800K=(MyWBInf.R-pDlg->m_BLC)+0.5;
		OTP_Gr_2800K=(MyWBInf.Gr-pDlg->m_BLC)+0.5;
		OTP_Gb_2800K=(MyWBInf.Gb-pDlg->m_BLC)+0.5;
		OTP_B_2800K=(MyWBInf.B-pDlg->m_BLC)+0.5;
		int RG_fin_2800K = int(512.0*OTP_R_2800K *2.0/(OTP_Gb_2800K+OTP_Gr_2800K) + 0.5);
		int BG_fin_2800K = int(512.0*OTP_B_2800K *2.0/(OTP_Gb_2800K+OTP_Gr_2800K) + 0.5);

		if(((1.0*RG_fin_2800K)/BG_fin_2800K)<0.5)   //防止ofset 张RGain BGain卡控失效
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换D65光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}

		if((RG_fin_2800K>m_LightSource.LAMP_CCT_D65.RGainHigh)||(RG_fin_2800K<m_LightSource.LAMP_CCT_D65.RGainLow))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换D65光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}

		if((BG_fin_2800K>m_LightSource.LAMP_CCT_D65.BGainHigh)||(BG_fin_2800K<m_LightSource.LAMP_CCT_D65.BGainLow))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换D65光源失败!"),COLOR_RED,200);
			Result=FALSE;
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("切换D65光源成功!"),COLOR_BLUE,200);
		}
		TempMsg.Format(_T("RGain:%d, BGain:%d, RGain/BGain < 1.5"),RG_fin_2800K,BG_fin_2800K);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	}

	return Result;
}

