#include "StdAfx.h"
#include "GetDNPIndex_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new GetDNPIndex_TestItem(pInterface, lpszName);
}


/******************************************************************
º¯ÊýÃû:    ¹¹Ôìº¯Êý
º¯Êý¹¦ÄÜ:  ¶ÔÏóÉú³ÉµÄÊ±ºò³õÊ¼±ØÐëÍê³ÉµÄÅäÖÃ
*******************************************************************/
GetDNPIndex_TestItem::GetDNPIndex_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetName(lpszName);	                             //ÉèÖÃ²âÊÔÏîÄ¿µÄÃû×Ö
	pDlg = new OptionDlg(NULL,this);                 //Éú³É¶Ô»°¿ò¶ÔÏó
	                                   //¶ÁÈ¡²ÎÊý
	pDlg->Create(OptionDlg::IDD,NULL);               //´´½¨¶Ô»°¿ò´°¿Ú
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //µÃµ½¶Ô»°¿ò¾ä±ú

	//TODO: ÉèÖÃ²âÊÔÏîÄ¿µÄÀàÐÍ 
	SetType(TYPE_IMAGE);

	//TODO: ÆäËûÀà³ÉÔ±¹¹Ôì³õÊ¼»¯
	memset(MyWBInf,0,4*sizeof(WBInf));
	GlobalTime=100;
	GlobalTime2=0;

	//.....
}

GetDNPIndex_TestItem::~GetDNPIndex_TestItem(void)
{

}


int GetDNPIndex_TestItem::InitItem()
{
	CamID = GetCamID();
	CurrentName=GetName();
	GlobalSetting=_T("WBSetting");
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //ÉèÖÃCONFIGÂ·¾¶	
	LoadOption(); 	
	SetName(GlobalSetting);	
	WriteConfigInt(_T("GlobalTime"), 100);
	SetName(CurrentName);

	return 0;
}


/******************************************************************
º¯ÊýÃû:    Initialize
º¯Êý¹¦ÄÜ:  ÓÃÓÚ²âÊÔÇ°³õÊ¼»¯Ò»Ð©±äÁ¿£¬×´Ì¬£¬·ÖÅä¿Õ¼äµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int GetDNPIndex_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:ÔÚ´ËÌí¼Ó³õÊ¼»¯´úÂë

	CString  GlobalSetting=_T("WBSetting");
	SetName(GlobalSetting);	
	GlobalTime=ReadConfigInt(_T("GlobalTime"), 3);
	SetName(CurrentName);
	imageflag=0;
    if (pDlg->m_ColorNum > 4)
    {
        pDlg->m_ColorNum = 4;
    }

    if (pDlg->m_ModuleNum > MAXGODLENNUM)
    {
        pDlg->m_ModuleNum = MAXGODLENNUM;
    }

	return 0;
}


void GetDNPIndex_TestItem::SetAutoLightStatus(int nsel)
{
	SetName(GlobalSetting);
	WriteConfigInt(_T("AUTOLIGHTSTATUS"), nsel);
	SetName(CurrentName);
}


/******************************************************************
º¯ÊýÃû:    Testing
º¯Êý¹¦ÄÜ:  Íê³ÉÄ³Ò»Ïî²âÊÔ¹¦ÄÜ´úÂë·ÅÓë´Ë
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int GetDNPIndex_TestItem::Testing()
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
 	SetName(GlobalSetting);	
 	GlobalTime=ReadConfigInt(_T("GlobalTime"), 3);
 	int CurrentTagTime=ReadConfigInt(_T("CurrentTimeTag"),2);
 	SetName(CurrentName);

	int NowColorNum=1;
	if (pDlg->m_count==0)
		NowColorNum=1;
	if (pDlg->m_count==3)
		NowColorNum=2;
	if (pDlg->m_count==6)
		NowColorNum=3;
	if (pDlg->m_count==9)
		NowColorNum=4;

	CString AFXmsg;
	//if ((GlobalTime>=(pDlg->m_count+3))||(GlobalTime<pDlg->m_count)) return 0;
    if (GlobalTime > (pDlg->m_ModuleNum*pDlg->m_ColorNum))
    {
        GlobalTime2 = 0; //下次光源校正时从新计数，不然数组会越界
        return 0;
    }

//	if (SetLEDFlag)
	{
		if(GlobalTime==pDlg->m_count) 
		{
			AFXmsg=_T("请放入")+pDlg->m_ColorName+_T("色温模组进行校正!");
			m_pInterface->Ctrl_Addlog(CamID,AFXmsg,COLOR_BLUE,200);
//			AfxMessageBox(AFXmsg);
			GlobalTime2=0;//初始化
		}
	}

	int iTimeTag=GetTimeTag();
	if (GlobalTime2!=0)
	{
		if(abs(iTimeTag-CurrentTagTime)>1)//时间过长不能矫正
		{	
            GlobalTime=0;
            GlobalTime2 = 0;    //每隔一时重新校准
        }
	}

	CString Caltime;
	Caltime.Format(_T("第%d颗模组开始校正.."),GlobalTime2+1);
	m_pInterface->Ctrl_Addlog(CamID,Caltime,COLOR_BLUE,200);
	CString CurrentFuseID=m_pInterface->Sensor_GetCamFuseID(CamID);
	m_pInterface->Ctrl_Addlog(CamID,CurrentFuseID,COLOR_BLUE,200);
	int i=0;
	for (i=0;i<MAXGODLENNUM;i++)
	{
		if(!CurrentFuseID.Compare(pDlg->FuseID[i]))
		{
			break;
		}
	}
	if (i>=MAXGODLENNUM)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("非注册模组!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("非注册模组!"));
		SetResult(RESULT_FAIL);

		SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
		return 0;
	}
	
	for (i=0;i<GlobalTime2;i++)
	{
		if (CurrentFuseID==MyfuseID[i])
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("该模组已注册!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("该模组已注册!"));
			SetResult(RESULT_FAIL);

			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;
		}
	}
	
	GetOTPData(MyWBInf[GlobalTime2]);
	ImageInfo *MyImageInfo = NULL;		
	MyImageInfo = m_pInterface->Ctrl_GetImgInfo(CamID);
	if (MyImageInfo->Raw_buffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("grab image fail!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("grab image fail!"));
		SetResult(RESULT_FAIL);
		SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
		return 0;
	}
	if (pDlg->m_QTITool)
	{
		if(!GetAWBData(MyImageInfo[0],MyWBInf[GlobalTime2]))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("获取AWB失败!"));
			SetResult(RESULT_FAIL);

			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;
		}
	}
	else
	{
	   CenterBlockRawInfo(MyImageInfo,pDlg->m_Ratio,MyWBInf[GlobalTime2],pDlg->m_index,pDlg->m_BLC);
	}
	if (pDlg->SingleChannelEnable)
	{
		BOOL WBResult=TRUE;
		CString temp;
		temp.Format(_T("当前影像 R=%d <%d,%d>"),MyWBInf[GlobalTime2].R,pDlg->R_min,pDlg->R_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 Gr=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr,pDlg->Gr_min,pDlg->Gr_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 Gb=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb,pDlg->Gb_min,pDlg->Gb_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 B=%d <%d,%d>"),MyWBInf[GlobalTime2].B,pDlg->B_min,pDlg->B_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		if (MyWBInf[GlobalTime2].R>pDlg->R_max||MyWBInf[GlobalTime2].R<pDlg->R_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].Gr>pDlg->Gr_max||MyWBInf[GlobalTime2].Gr<pDlg->Gr_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].Gb>pDlg->Gb_max||MyWBInf[GlobalTime2].Gb<pDlg->Gb_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].B>pDlg->B_max||MyWBInf[GlobalTime2].B<pDlg->B_min) WBResult=FALSE;
		if (WBResult!=TRUE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("单通道R Gr Gb B超规格!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("单通道R Gr Gb B超规格!"));
			SetResult(RESULT_FAIL);
			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;
		}

		float m_OTPDistance=0;
		if (pDlg->m_GG_GainEn)
		{
			m_OTPDistance=(float)sqrt(pow((MyWBInf[GlobalTime2].RG-MyWBInf[GlobalTime2].OTP_RG)*1.0/MyWBInf[GlobalTime2].OTP_RG,2) + pow((MyWBInf[GlobalTime2].BG-MyWBInf[GlobalTime2].OTP_BG)*1.0/MyWBInf[GlobalTime2].OTP_BG,2)+pow((MyWBInf[GlobalTime2].GG-MyWBInf[GlobalTime2].OTP_GG)*1.0/MyWBInf[GlobalTime2].OTP_GG,2));
		}
		else
			m_OTPDistance=(float)sqrt(pow((MyWBInf[GlobalTime2].RG-MyWBInf[GlobalTime2].OTP_RG)*1.0/MyWBInf[GlobalTime2].OTP_RG,2) + pow((MyWBInf[GlobalTime2].BG-MyWBInf[GlobalTime2].OTP_BG)*1.0/MyWBInf[GlobalTime2].OTP_BG,2));
		temp.Format(_T("Distance=%f"),m_OTPDistance*100);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		if (m_OTPDistance * 1000 > pDlg->m_DNPDistance)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Distance 超出范围！"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Distance 超出范围!"));
			SetResult(RESULT_FAIL);
			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;	
		}

		MyWBInf[GlobalTime2].R_index=(MyWBInf[GlobalTime2].OTP_R*10000.0/(float)MyWBInf[GlobalTime2].R+0.5);
		MyWBInf[GlobalTime2].Gr_index=(MyWBInf[GlobalTime2].OTP_Gr*10000.0/(float)MyWBInf[GlobalTime2].Gr+0.5);
		MyWBInf[GlobalTime2].Gb_index=(MyWBInf[GlobalTime2].OTP_Gb*10000.0/(float)MyWBInf[GlobalTime2].Gb+0.5);
		MyWBInf[GlobalTime2].B_index=(MyWBInf[GlobalTime2].OTP_B*10000.0/(float)MyWBInf[GlobalTime2].B+0.5);
		MyWBInf[GlobalTime2].R_index=(MyWBInf[GlobalTime2].R_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].Gb_index=(MyWBInf[GlobalTime2].Gb_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].B_index=(MyWBInf[GlobalTime2].B_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].Gr_index=(MyWBInf[GlobalTime2].Gr_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		temp.Format(_T("当前影像 R_index=%d <%d,%d>"),MyWBInf[GlobalTime2].R_index,pDlg->R_index_min,pDlg->R_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 Gr_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr_index,pDlg->Gr_index_min,pDlg->Gr_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 Gb_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb_index,pDlg->Gb_index_min,pDlg->Gb_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 B_index=%d <%d,%d>"),MyWBInf[GlobalTime2].B_index,pDlg->B_index_min,pDlg->B_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		if (MyWBInf[GlobalTime2].R_index>pDlg->R_index_max||MyWBInf[GlobalTime2].R_index<pDlg->R_index_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].Gr_index>pDlg->Gr_index_max||MyWBInf[GlobalTime2].Gr_index<pDlg->Gr_index_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].Gb_index>pDlg->Gb_index_max||MyWBInf[GlobalTime2].Gb_index<pDlg->Gb_index_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].B_index>pDlg->B_index_max||MyWBInf[GlobalTime2].B_index<pDlg->B_index_min) WBResult=FALSE;
		if (WBResult!=TRUE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("单通道index超规格!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("单通道index超规格!"));
			SetResult(RESULT_FAIL);

			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;
		}

	}
	if (pDlg->GainEnable)
	{
		BOOL WBResult=TRUE;
		CString temp;
		temp.Format(_T("当前影像 RG=%d <%d,%d>"),MyWBInf[GlobalTime2].RG,pDlg->RG_min,pDlg->RG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 BG=%d <%d,%d>"),MyWBInf[GlobalTime2].BG,pDlg->BG_min,pDlg->BG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 GG=%d <%d,%d>"),MyWBInf[GlobalTime2].GG,pDlg->GG_min,pDlg->GG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		if (MyWBInf[GlobalTime2].RG>pDlg->RG_max||MyWBInf[GlobalTime2].RG<pDlg->RG_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].BG>pDlg->BG_max||MyWBInf[GlobalTime2].BG<pDlg->BG_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].GG>pDlg->GG_max||MyWBInf[GlobalTime2].GG<pDlg->GG_min) WBResult=FALSE;
		if (WBResult!=TRUE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Gain值超规格!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Gain值超规格!"));
			SetResult(RESULT_FAIL);
			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;
		}

		float m_OTPDistance=0;
		if (pDlg->m_GG_GainEn)
		{
			m_OTPDistance=(float)sqrt(pow((MyWBInf[GlobalTime2].RG-MyWBInf[GlobalTime2].OTP_RG)*1.0/MyWBInf[GlobalTime2].OTP_RG,2) + pow((MyWBInf[GlobalTime2].BG-MyWBInf[GlobalTime2].OTP_BG)*1.0/MyWBInf[GlobalTime2].OTP_BG,2)+pow((MyWBInf[GlobalTime2].GG-MyWBInf[GlobalTime2].OTP_GG)*1.0/MyWBInf[GlobalTime2].OTP_GG,2));
		}
		else
			m_OTPDistance=(float)sqrt(pow((MyWBInf[GlobalTime2].RG-MyWBInf[GlobalTime2].OTP_RG)*1.0/MyWBInf[GlobalTime2].OTP_RG,2) + pow((MyWBInf[GlobalTime2].BG-MyWBInf[GlobalTime2].OTP_BG)*1.0/MyWBInf[GlobalTime2].OTP_BG,2));
		temp.Format(_T("Distance=%f"),m_OTPDistance*100);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		if (m_OTPDistance * 1000 > pDlg->m_DNPDistance)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Distance 超出范围！"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Distance 超出范围!"));

			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			SetResult(RESULT_FAIL);
			return 0;	
		}
		MyWBInf[GlobalTime2].RG_index=(MyWBInf[GlobalTime2].OTP_RG*10000.0/(float)MyWBInf[GlobalTime2].RG+0.5);
		MyWBInf[GlobalTime2].BG_index=(MyWBInf[GlobalTime2].OTP_BG*10000.0/(float)MyWBInf[GlobalTime2].BG+0.5);
		MyWBInf[GlobalTime2].GG_index=(MyWBInf[GlobalTime2].OTP_GG*10000.0/(float)MyWBInf[GlobalTime2].GG+0.5);

		temp.Format(_T("当前影像 RG_index=%d <%d,%d>"),MyWBInf[GlobalTime2].RG_index,pDlg->RG_index_min,pDlg->RG_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 BG_index=%d <%d,%d>"),MyWBInf[GlobalTime2].BG_index,pDlg->BG_index_min,pDlg->BG_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 GG_index=%d <%d,%d>"),MyWBInf[GlobalTime2].GG_index,pDlg->GG_index_min,pDlg->GG_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		if (MyWBInf[GlobalTime2].RG_index>pDlg->RG_index_max||MyWBInf[GlobalTime2].RG_index<pDlg->RG_index_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].BG_index>pDlg->BG_index_max||MyWBInf[GlobalTime2].BG_index<pDlg->BG_index_min) WBResult=FALSE;
		if (MyWBInf[GlobalTime2].GG_index>pDlg->GG_index_max||MyWBInf[GlobalTime2].GG_index<pDlg->GG_index_min) WBResult=FALSE;
		if (WBResult!=TRUE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Gain index值超规格!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Gain index值超规格!"));
			SetResult(RESULT_FAIL);

			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;
		}
	}

	GlobalTime++;
	GlobalTime2++;
	CString temp;
	int R_Index_Error_max=0;
	int Gr_Index_Error_max=0;
	int Gb_Index_Error_max=0;
	int B_Index_Error_max=0;
	int RG_Index_Error_max=0;
	int BG_Index_Error_max=0;
	int GG_Index_Error_max=0;

	int R_Index_Error_min=100000;
	int Gr_Index_Error_min=100000;
	int Gb_Index_Error_min=100000;
	int B_Index_Error_min=100000;
	int RG_Index_Error_min=100000;
	int BG_Index_Error_min=100000;
	int GG_Index_Error_min=100000;

	int R_Index_Error=1000;
	int Gr_Index_Error=1000;
	int Gb_Index_Error=1000;
	int B_Index_Error=1000;
	int RG_Index_Error=1000;
	int BG_Index_Error=1000;
	int GG_Index_Error=1000;
	for(int i=GlobalTime2;i>0;i--)
	{
	
		if (pDlg->SingleChannelEnable==1&&pDlg->GainEnable==0)
		{
			temp.Format(_T("%d %d %d %d %d %d %d %d %d %d %d %d %d"),i,MyWBInf[i-1].R,MyWBInf[i-1].Gr,MyWBInf[i-1].Gb,MyWBInf[i-1].B,MyWBInf[i-1].OTP_R,MyWBInf[i-1].OTP_Gr,MyWBInf[i-1].OTP_Gb,MyWBInf[i-1].OTP_B,MyWBInf[i-1].R_index,MyWBInf[i-1].Gr_index,MyWBInf[i-1].Gb_index,MyWBInf[i-1].B_index);
			if (GlobalTime2==pDlg->m_ModuleNum)
			{
				MyWBInf[GlobalTime2].R_index+=MyWBInf[i-1].R_index;
				MyWBInf[GlobalTime2].Gr_index+=MyWBInf[i-1].Gr_index;
				MyWBInf[GlobalTime2].Gb_index+=MyWBInf[i-1].Gb_index;
				MyWBInf[GlobalTime2].B_index+=MyWBInf[i-1].B_index;
				if (i==1)
				{
					MyWBInf[GlobalTime2].R_index=(float)1.0*MyWBInf[GlobalTime2].R_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].Gr_index=(float)1.0*MyWBInf[GlobalTime2].Gr_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].Gb_index=(float)1.0*MyWBInf[GlobalTime2].Gb_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].B_index=(float)1.0*MyWBInf[GlobalTime2].B_index/pDlg->m_ModuleNum+0.5;
				}
				if(R_Index_Error_max<MyWBInf[i-1].R_index) R_Index_Error_max=MyWBInf[i-1].R_index;
				if(Gr_Index_Error_max<MyWBInf[i-1].Gr_index) Gr_Index_Error_max=MyWBInf[i-1].Gr_index;
				if(Gb_Index_Error_max<MyWBInf[i-1].Gb_index) Gb_Index_Error_max=MyWBInf[i-1].Gb_index;
				if(B_Index_Error_max<MyWBInf[i-1].B_index) B_Index_Error_max=MyWBInf[i-1].B_index;
				if(R_Index_Error_min>MyWBInf[i-1].R_index) R_Index_Error_min=MyWBInf[i-1].R_index;
				if(Gr_Index_Error_min>MyWBInf[i-1].Gr_index) Gr_Index_Error_min=MyWBInf[i-1].Gr_index;
				if(Gb_Index_Error_min>MyWBInf[i-1].Gb_index) Gb_Index_Error_min=MyWBInf[i-1].Gb_index;
				if(B_Index_Error_min>MyWBInf[i-1].B_index) B_Index_Error_min=MyWBInf[i-1].B_index;
			}
		}
		else if (pDlg->SingleChannelEnable==0&&pDlg->GainEnable==1)
		{
			temp.Format(_T("%d %d %d %d %d %d %d %d %d %d"),i,MyWBInf[i-1].RG,MyWBInf[i-1].BG,MyWBInf[i-1].GG,MyWBInf[i-1].OTP_RG,MyWBInf[i-1].OTP_BG,MyWBInf[i-1].OTP_GG,MyWBInf[i-1].RG_index,MyWBInf[i-1].BG_index,MyWBInf[i-1].GG_index);
			if (GlobalTime2==pDlg->m_ModuleNum)
			{
				MyWBInf[GlobalTime2].RG_index+=MyWBInf[i-1].RG_index;
				MyWBInf[GlobalTime2].BG_index+=MyWBInf[i-1].BG_index;
				MyWBInf[GlobalTime2].GG_index+=MyWBInf[i-1].GG_index;
				if (i==1)
				{
					MyWBInf[GlobalTime2].RG_index=(float)1.0*MyWBInf[GlobalTime2].RG_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].BG_index=(float)1.0*MyWBInf[GlobalTime2].BG_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].GG_index=(float)1.0*MyWBInf[GlobalTime2].GG_index/pDlg->m_ModuleNum+0.5;
				}
				if(RG_Index_Error_max<MyWBInf[i-1].RG_index) RG_Index_Error_max=MyWBInf[i-1].RG_index;
				if(BG_Index_Error_max<MyWBInf[i-1].BG_index) BG_Index_Error_max=MyWBInf[i-1].BG_index;
				if(GG_Index_Error_max<MyWBInf[i-1].GG_index) GG_Index_Error_max=MyWBInf[i-1].GG_index;
				if(RG_Index_Error_min>MyWBInf[i-1].RG_index) RG_Index_Error_min=MyWBInf[i-1].RG_index;
				if(BG_Index_Error_min>MyWBInf[i-1].BG_index) BG_Index_Error_min=MyWBInf[i-1].BG_index;
				if(GG_Index_Error_min>MyWBInf[i-1].GG_index) GG_Index_Error_min=MyWBInf[i-1].GG_index;
			}
		}
		else
		{
			if (GlobalTime2==pDlg->m_ModuleNum)
			{
				MyWBInf[GlobalTime2].R_index+=MyWBInf[i-1].R_index;
				MyWBInf[GlobalTime2].Gr_index+=MyWBInf[i-1].Gr_index;
				MyWBInf[GlobalTime2].Gb_index+=MyWBInf[i-1].Gb_index;
				MyWBInf[GlobalTime2].B_index+=MyWBInf[i-1].B_index;
				MyWBInf[GlobalTime2].RG_index+=MyWBInf[i-1].RG_index;
				MyWBInf[GlobalTime2].BG_index+=MyWBInf[i-1].BG_index;
				MyWBInf[GlobalTime2].GG_index+=MyWBInf[i-1].GG_index;
				if (i==1)
				{
					MyWBInf[GlobalTime2].R_index=(float)1.0*MyWBInf[GlobalTime2].R_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].Gr_index=(float)1.0*MyWBInf[GlobalTime2].Gr_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].Gb_index=(float)1.0*MyWBInf[GlobalTime2].Gb_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].B_index=(float)1.0*MyWBInf[GlobalTime2].B_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].RG_index=(float)1.0*MyWBInf[GlobalTime2].RG_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].BG_index=(float)1.0*MyWBInf[GlobalTime2].BG_index/pDlg->m_ModuleNum+0.5;
					MyWBInf[GlobalTime2].GG_index=(float)1.0*MyWBInf[GlobalTime2].GG_index/pDlg->m_ModuleNum+0.5;
				}
				if(R_Index_Error_max<MyWBInf[i-1].R_index) R_Index_Error_max=MyWBInf[i-1].R_index;
				if(Gr_Index_Error_max<MyWBInf[i-1].Gr_index) Gr_Index_Error_max=MyWBInf[i-1].Gr_index;
				if(Gb_Index_Error_max<MyWBInf[i-1].Gb_index) Gb_Index_Error_max=MyWBInf[i-1].Gb_index;
				if(B_Index_Error_max<MyWBInf[i-1].B_index) B_Index_Error_max=MyWBInf[i-1].B_index;
				if(R_Index_Error_min>MyWBInf[i-1].R_index) R_Index_Error_min=MyWBInf[i-1].R_index;
				if(Gr_Index_Error_min>MyWBInf[i-1].Gr_index) Gr_Index_Error_min=MyWBInf[i-1].Gr_index;
				if(Gb_Index_Error_min>MyWBInf[i-1].Gb_index) Gb_Index_Error_min=MyWBInf[i-1].Gb_index;
				if(B_Index_Error_min>MyWBInf[i-1].B_index) B_Index_Error_min=MyWBInf[i-1].B_index;
				if(RG_Index_Error_max<MyWBInf[i-1].RG_index) RG_Index_Error_max=MyWBInf[i-1].RG_index;
				if(BG_Index_Error_max<MyWBInf[i-1].BG_index) BG_Index_Error_max=MyWBInf[i-1].BG_index;
				if(GG_Index_Error_max<MyWBInf[i-1].GG_index) GG_Index_Error_max=MyWBInf[i-1].GG_index;
				if(RG_Index_Error_min>MyWBInf[i-1].RG_index) RG_Index_Error_min=MyWBInf[i-1].RG_index;
				if(BG_Index_Error_min>MyWBInf[i-1].BG_index) BG_Index_Error_min=MyWBInf[i-1].BG_index;
				if(GG_Index_Error_min>MyWBInf[i-1].GG_index) GG_Index_Error_min=MyWBInf[i-1].GG_index;
			}
			temp.Format(_T("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"),i,MyWBInf[i-1].R,MyWBInf[i-1].Gr,MyWBInf[i-1].Gb,MyWBInf[i-1].B,MyWBInf[i-1].R_index,MyWBInf[i-1].Gr_index,MyWBInf[i-1].Gb_index,MyWBInf[i-1].B_index,MyWBInf[i-1].RG,MyWBInf[i-1].BG,MyWBInf[i-1].GG,MyWBInf[i-1].OTP_RG,MyWBInf[i-1].OTP_BG,MyWBInf[i-1].OTP_GG,MyWBInf[i-1].RG_index,MyWBInf[i-1].BG_index,MyWBInf[i-1].GG_index);
		}
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
	}
	int WBResult=0;
	if (pDlg->SingleChannelEnable==1&&pDlg->GainEnable==0)
	{
		if (GlobalTime2==pDlg->m_ModuleNum)
		{
			temp.Format(_T("Aevr -- -- -- -- -- -- -- -- %d %d %d %d"),MyWBInf[GlobalTime2].R_index,MyWBInf[GlobalTime2].Gr_index,MyWBInf[GlobalTime2].Gb_index,MyWBInf[GlobalTime2].B_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
			if (MyWBInf[GlobalTime2].R_index>pDlg->R_index_max||MyWBInf[GlobalTime2].R_index<pDlg->R_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].Gr_index>pDlg->Gr_index_max||MyWBInf[GlobalTime2].Gr_index<pDlg->Gr_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].Gb_index>pDlg->Gb_index_max||MyWBInf[GlobalTime2].Gb_index<pDlg->Gb_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].B_index>pDlg->B_index_max||MyWBInf[GlobalTime2].B_index<pDlg->B_index_min) WBResult=1;
			if(abs(R_Index_Error_max-R_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(Gr_Index_Error_max-Gr_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(Gb_Index_Error_max-Gb_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(B_Index_Error_max-B_Index_Error_min)>pDlg->index_error) WBResult=2;

			temp.Format(_T("R_Max = %d,R_Min = %d,R_Error = %d"),R_Index_Error_max,R_Index_Error_min,abs(R_Index_Error_max-R_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("Gr_Max = %d,Gr_Min = %d,Gr_Error = %d"),Gr_Index_Error_max,Gr_Index_Error_min,abs(Gr_Index_Error_max-Gr_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("Gb_Max = %d,Gb_Min = %d,Gb_Error = %d"),Gb_Index_Error_max,Gb_Index_Error_min,abs(Gb_Index_Error_max-Gb_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("B_Max = %d,B_Min = %d,B_Error = %d"),B_Index_Error_max,B_Index_Error_min,abs(B_Index_Error_max-B_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		}	
		temp.Format(_T("序号 R Gr Gb B OTP_R OTP_Gr OTP_Gb OTP_B R_index Gr_index Gb_index B_index"));
	}
	else if (pDlg->SingleChannelEnable==0&&pDlg->GainEnable==1)
	{
		if (GlobalTime2==pDlg->m_ModuleNum)
		{
			temp.Format(_T("Aevr -- -- -- -- -- -- %d %d %d"),MyWBInf[GlobalTime2].RG_index,MyWBInf[GlobalTime2].BG_index,MyWBInf[GlobalTime2].GG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
			if (MyWBInf[GlobalTime2].RG_index>pDlg->RG_index_max||MyWBInf[GlobalTime2].RG_index<pDlg->RG_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].BG_index>pDlg->BG_index_max||MyWBInf[GlobalTime2].BG_index<pDlg->BG_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].GG_index>pDlg->GG_index_max||MyWBInf[GlobalTime2].GG_index<pDlg->GG_index_min) WBResult=1;
			if(abs(RG_Index_Error_max-RG_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(BG_Index_Error_max-BG_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(GG_Index_Error_max-GG_Index_Error_min)>pDlg->index_error) WBResult=2;

			temp.Format(_T("RG_Max = %d,RG_Min = %d,RG_Error = %d"),RG_Index_Error_max,RG_Index_Error_min,abs(RG_Index_Error_max-RG_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("BG_Max = %d,BG_Min = %d,BG_Error = %d"),BG_Index_Error_max,BG_Index_Error_min,abs(BG_Index_Error_max-BG_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("GG_Max = %d,GG_Min = %d,GG_Error = %d"),GG_Index_Error_max,GG_Index_Error_min,abs(GG_Index_Error_max-GG_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		}
		temp.Format(_T("序号 RG BG GG OTP_RG OTP_BG OTP_GG RG_index BG_index GG_index"));		
	}
	else
	{
		if (GlobalTime2==pDlg->m_ModuleNum)
		{
			temp.Format(_T("Aevr -- -- -- -- -- -- -- -- %d %d %d %d -- -- -- -- -- -- %d %d %d"),MyWBInf[GlobalTime2].R_index,MyWBInf[GlobalTime2].Gr_index,MyWBInf[GlobalTime2].Gb_index,MyWBInf[GlobalTime2].B_index,MyWBInf[GlobalTime2].RG_index,MyWBInf[GlobalTime2].BG_index,MyWBInf[GlobalTime2].GG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
			if (MyWBInf[GlobalTime2].R_index>pDlg->R_index_max||MyWBInf[GlobalTime2].R_index<pDlg->R_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].Gr_index>pDlg->Gr_index_max||MyWBInf[GlobalTime2].Gr_index<pDlg->Gr_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].Gb_index>pDlg->Gb_index_max||MyWBInf[GlobalTime2].Gb_index<pDlg->Gb_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].B_index>pDlg->B_index_max||MyWBInf[GlobalTime2].B_index<pDlg->B_index_min) WBResult=1;
			if(abs(R_Index_Error_max-R_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(Gr_Index_Error_max-Gr_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(Gb_Index_Error_max-Gb_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(B_Index_Error_max-B_Index_Error_min)>pDlg->index_error) WBResult=2;

			temp.Format(_T("R_Max = %d,R_Min = %d,R_Error = %d"),R_Index_Error_max,R_Index_Error_min,abs(R_Index_Error_max-R_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("Gr_Max = %d,Gr_Min = %d,Gr_Error = %d"),Gr_Index_Error_max,Gr_Index_Error_min,abs(Gr_Index_Error_max-Gr_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("Gb_Max = %d,Gb_Min = %d,Gb_Error = %d"),Gb_Index_Error_max,Gb_Index_Error_min,abs(Gb_Index_Error_max-Gb_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("B_Max = %d,B_Min = %d,B_Error = %d"),B_Index_Error_max,B_Index_Error_min,abs(B_Index_Error_max-B_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

			if (MyWBInf[GlobalTime2].RG_index>pDlg->RG_index_max||MyWBInf[GlobalTime2].RG_index<pDlg->RG_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].BG_index>pDlg->BG_index_max||MyWBInf[GlobalTime2].BG_index<pDlg->BG_index_min) WBResult=1;
			if (MyWBInf[GlobalTime2].GG_index>pDlg->GG_index_max||MyWBInf[GlobalTime2].GG_index<pDlg->GG_index_min) WBResult=1;
			if(abs(RG_Index_Error_max-RG_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(BG_Index_Error_max-BG_Index_Error_min)>pDlg->index_error) WBResult=2;
			if(abs(GG_Index_Error_max-GG_Index_Error_min)>pDlg->index_error) WBResult=2;

			temp.Format(_T("RG_Max = %d,RG_Min = %d,RG_Error = %d"),RG_Index_Error_max,RG_Index_Error_min,abs(RG_Index_Error_max-RG_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("BG_Max = %d,BG_Min = %d,BG_Error = %d"),BG_Index_Error_max,BG_Index_Error_min,abs(BG_Index_Error_max-BG_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("GG_Max = %d,GG_Min = %d,GG_Error = %d"),GG_Index_Error_max,GG_Index_Error_min,abs(GG_Index_Error_max-GG_Index_Error_min));
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		}
		temp.Format(_T("序号 R Gr Gb B OTP_R OTP_Gr OTP_Gb OTP_B R_index Gr_index Gb_index B_index RG BG GG OTP_RG OTP_BG OTP_GG RG_index BG_index GG_index"));

	}
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
	///导入规格//
	if (GlobalTime2==pDlg->m_ModuleNum)
	{
		if(WBResult==1||WBResult==2)
		{
			//GlobalTime--;
			CString templog=_T("index平均值不在范围内!");
			if(WBResult==2) templog=_T("index error不在范围内!");
			m_pInterface->Ctrl_Addlog(CamID,templog,COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,templog);
			SetResult(RESULT_FAIL);
			//memset(&MyWBInf[GlobalTime2],0,sizeof(WBInf));
            --GlobalTime2;  //失败后计数器减1，统计校验成功的模组

			SetAutoLightStatus(AUTOLIGHT_SAVECOEFFEE_FAIL_AVAGEGODEN);  //获取GOLDEDN平均值失败
			return 0;
		}
		SetName(GlobalSetting);
		iTimeTag=GetTimeTag();
		wchar_t wcstation[128]={0};
		swprintf(wcstation,_T("CamID%d_Site%d"),CamID,m_pInterface->GetSiteNo());
		RegSetKey(_T(""),wcstation,pDlg->Station.GetBuffer(pDlg->Station.GetLength()));
		CTime t=CTime::GetCurrentTime();
		wchar_t wctemp[128]={0};
		swprintf(wctemp,_T("%02d日%02d时%02d分"),t.GetDay(),t.GetHour(),t.GetMinute());
		swprintf(wcstation,_T("CamID%d_Time%d"),CamID,m_pInterface->GetSiteNo());
		RegSetKey(_T(""),wcstation,wctemp);
		WriteConfigInt(_T("CalTimeTag"),iTimeTag);
		CString suffix=_T("");
		if (pDlg->m_count!=0)
			suffix.Format(_T("_%d"),pDlg->m_count);

		WriteConfigInt(_T("R_index")+suffix,MyWBInf[GlobalTime2].R_index);
		WriteConfigInt(_T("Gr_index")+suffix,MyWBInf[GlobalTime2].Gr_index);
		WriteConfigInt(_T("Gb_index")+suffix,MyWBInf[GlobalTime2].Gb_index);
		WriteConfigInt(_T("B_index")+suffix,MyWBInf[GlobalTime2].B_index);
		WriteConfigInt(_T("RG_index")+suffix,MyWBInf[GlobalTime2].RG_index);
		WriteConfigInt(_T("BG_index")+suffix,MyWBInf[GlobalTime2].BG_index);
		WriteConfigInt(_T("GG_index")+suffix,MyWBInf[GlobalTime2].GG_index);
		SetName(CurrentName);
		if (NowColorNum==pDlg->m_ColorNum)
		{
            m_pInterface->Ctrl_Addlog(CamID, _T("光源校正完毕，开始测试"), COLOR_GREEN, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("光源校正完毕CheckLast Pass"));  //此errorinfo不能改，主框架识别该信息做区分刷屏
            SetResult(RESULT_NORMAL); //必须置为RESULT_NORMAL,主框架识别后，在该item测试完后刷pass
		}
		else if (NowColorNum<pDlg->m_ColorNum)
		{
            //Bug：多色温，多个dll,count数小的放在前面，否则下一色温测试不了，直接就刷屏PASS
			AFXmsg=pDlg->m_ColorName+_T("色温校正完毕，换下一色温校准");
			m_pInterface->Ctrl_Addlog(CamID, AFXmsg, COLOR_GREEN, 200);
            SetResult(RESULT_PASS);  
		}

		SetAutoLightStatus(AUTOLIGHT_SAVECOEFFEE_SUCESS);  //导入成功
		//memset(&MyWBInf[GlobalTime2],0,sizeof(WBInf));  //越界了，清零有什么意思，注释掉
	}
	else
	{
		temp.Format(_T("第%d颗模组校正完成"),GlobalTime2);
        m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_GREEN, 200);
        temp += _T("请放入CheckLast模组");
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, temp); //此errorinfo不能改，主框架识别该信息做区分刷屏

        if (NowColorNum < pDlg->m_ColorNum && pDlg->m_isSingleModule)
        {
            SetResult(RESULT_PASS);
        }
        else
        {
		    SetResult(RESULT_NORMAL);   //必须置为RESULT_NORMAL,主框架识别后，在该item测试完后刷pass
        }
		SetAutoLightStatus(AUTOLIGHT_GOLDEN_SUCESS);  //获取GOLDEDN值成功
	}

    SetName(GlobalSetting);
    WriteConfigInt(_T("CurrentTimeTag"),iTimeTag);
    WriteConfigInt(_T("GlobalTime"),GlobalTime);
    SetName(CurrentName);
    m_pInterface->UnlocktheFile(CamID);//解锁加密狗
    MyfuseID[GlobalTime2-1]=CurrentFuseID;
	
    return 0;
}

void GetDNPIndex_TestItem::CenterBlockRawInfo(ImageInfo *mySensor ,int Ratio,WBInf &MyWbInf,int index,int BLC)
{
	USHORT *pIn=new USHORT[mySensor->imgwidth*mySensor->imgheight];
	if (NULL == pIn)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("pIn Buffer 申请失败!"),COLOR_RED,200);
		return;
	}
	memset(pIn,0,mySensor->imgwidth*mySensor->imgheight*sizeof(USHORT));
	if(pDlg->m_Raw8_EN)
	{
		for (int k=0;k<mySensor->imgwidth*mySensor->imgheight;k++)
		{
			pIn[k]=(mySensor->Raw_buffer[k]>>2);
		}
	}
	else
	{
		memcpy(pIn,mySensor->Raw_buffer,mySensor->imgwidth*mySensor->imgheight*2);
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

	SAFE_DELETE_ARRAY(pIn);

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;
	MyWbInf.Gr = int(Gr)-BLC;
	MyWbInf.Gb = int(Gb)-BLC;
	MyWbInf.R  = int(R)-BLC;
	MyWbInf.B  = int(B)-BLC;
	int RawG   = int((Gr + Gb - 2*BLC) / 2.0 + 0.5); 
	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	if(GAverFlag==1)
	{
		MyWbInf.RG = int( (float)index*(MyWbInf.R)/(float)(RawG) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)index*(MyWbInf.B)/(float)(RawG) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)index*(MyWbInf.Gb)/(float)(MyWbInf.Gr)+AA);
	}
	else if (GAverFlag==2)
	{
		MyWbInf.RG = int( (float)index*(MyWbInf.R)/(float)(MyWbInf.Gr) + AA );// Rave/Gave
		MyWbInf.BG = int( (float)index*(MyWbInf.B)/(float)(MyWbInf.Gr) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)index*(MyWbInf.Gb)/(float)(MyWbInf.Gr)+AA);
	}
	else if (GAverFlag==3)
	{
		MyWbInf.RG = int( (float)index*(MyWbInf.R)/(float)(MyWbInf.Gb) + AA );// Rave/Gave
		MyWbInf.BG = int( (float)index*(MyWbInf.B)/(float)(MyWbInf.Gb) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)index*(MyWbInf.Gr)/(float)(MyWbInf.Gb)+AA);
	}
}


/******************************************************************
º¯ÊýÃû:    Finalize
º¯Êý¹¦ÄÜ:  ÓÃÓÚInitializeÉêÃ÷µÄ±äÁ¿»Ö¸´Ô­Ê¼Öµ£¬ÉêÇëµÄÄÚ´æ¿Õ¼äÊÍ·ÅµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int GetDNPIndex_TestItem::Finalize()
{
	//TODO:ÔÚ´ËÌí¼Ó»ØÊÕ½áÊø´úÂë


	return 0;
}


/******************************************************************
º¯ÊýÃû:    LoadOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡²ÎÊýµ½±äÁ¿£¬ºÍ¸³Öµµ½¶Ô»°¿òµÄ°ó¶¨±äÁ¿ (ÎÄ¼þ´æ´¢Öµ->±äÁ¿¡ª>¿Ø¼þµÄÖµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int GetDNPIndex_TestItem::LoadOption()
{
	pDlg->m_count=ReadConfigInt(_T("count"),0);
	CString suffix=_T("");
	if (pDlg->m_count!=0)
		suffix.Format(_T("_%d"),pDlg->m_count);
	SetName(GlobalSetting);
	//TODO: ÔÚ´ËÌí¼Ó¶ÁÈ¡´úÂë 
	pDlg->m_QTITool=ReadConfigInt(_T("QTITool"),0);
	pDlg->m_flow=ReadConfigInt(_T("flow"),1);
	pDlg->m_DNPDistance  = ReadConfigInt(_T("DNPDistance")+suffix,100);
	pDlg->m_GG_GainEn=ReadConfigInt(_T("GG_GainEn"),0);
	GAverFlag=ReadConfigInt(_T("GAverFlag"),1);
	if (GAverFlag==1)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO1))->SetCheck(1);
	}
	else if (GAverFlag==2)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	else if (GAverFlag==3)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO3))->SetCheck(1);
	}
	for (int i=0;i<20;i++)
	{
		CString temp;
		temp.Format(_T("FuseID_%d"),i+1);
		pDlg->FuseID[i] = ReadConfigString(temp+suffix, _T("0000000000"));
	}
	pDlg->m_SlaveID= ReadConfigString(_T("SlaveID"), _T("0x20"));
	pDlg->R_StartAdd= ReadConfigString(_T("R_StartAdd")+suffix, _T("0x0000"));
	pDlg->Gr_StartAdd= ReadConfigString(_T("Gr_StartAdd")+suffix, _T("0x0000"));
	pDlg->Gb_StartAdd= ReadConfigString(_T("Gb_StartAdd")+suffix, _T("0x0000"));
	pDlg->B_StartAdd= ReadConfigString(_T("B_StartAdd")+suffix, _T("0x0000"));
	pDlg->RG_StartAdd= ReadConfigString(_T("RG_StartAdd")+suffix, _T("0x0000"));
	pDlg->BG_StartAdd= ReadConfigString(_T("BG_StartAdd")+suffix, _T("0x0000"));
	pDlg->GG_StartAdd= ReadConfigString(_T("GG_StartAdd")+suffix, _T("0x0000"));
	pDlg->R_min=ReadConfigInt(_T("R_min")+suffix,30);
	pDlg->R_max=ReadConfigInt(_T("R_max")+suffix,220);
	pDlg->Gr_min=ReadConfigInt(_T("Gr_min")+suffix,30);
	pDlg->Gr_max=ReadConfigInt(_T("Gr_max")+suffix,220);
	pDlg->Gb_min=ReadConfigInt(_T("Gb_min")+suffix,30);
	pDlg->Gb_max=ReadConfigInt(_T("Gb_max")+suffix,220);
	pDlg->B_min=ReadConfigInt(_T("B_min")+suffix,30);
	pDlg->B_max=ReadConfigInt(_T("B_max")+suffix,220);
	//pDlg->RG_min=ReadConfigInt(_T("RG_min")+suffix+suffix,220);
	pDlg->RG_min=ReadConfigInt(_T("RG_min")+suffix,220); //YSZ
	pDlg->RG_max=ReadConfigInt(_T("RG_max")+suffix,1000);
	pDlg->BG_min=ReadConfigInt(_T("BG_min")+suffix,220);
	pDlg->BG_max=ReadConfigInt(_T("BG_max")+suffix,1000);
	pDlg->GG_min=ReadConfigInt(_T("GG_min")+suffix,220);
	pDlg->GG_max=ReadConfigInt(_T("GG_max")+suffix,1000);
	pDlg->R_index_min=ReadConfigInt(_T("R_index_min")+suffix,9000);
	pDlg->R_index_max=ReadConfigInt(_T("R_index_max")+suffix,11000);
	pDlg->Gr_index_min=ReadConfigInt(_T("Gr_index_min")+suffix,9000);
	pDlg->Gr_index_max=ReadConfigInt(_T("Gr_index_max")+suffix,11000);
	pDlg->Gb_index_min=ReadConfigInt(_T("Gb_index_min")+suffix,9000);
	pDlg->Gb_index_max=ReadConfigInt(_T("Gb_index_max")+suffix,11000);
	pDlg->B_index_min=ReadConfigInt(_T("B_index_min")+suffix,9000);
	pDlg->B_index_max=ReadConfigInt(_T("B_index_max")+suffix,11000);
	pDlg->RG_index_min=ReadConfigInt(_T("RG_index_min")+suffix,9000);
	pDlg->RG_index_max=ReadConfigInt(_T("RG_index_max")+suffix,11000);
	pDlg->BG_index_min=ReadConfigInt(_T("BG_index_min")+suffix,9000);
	pDlg->BG_index_max=ReadConfigInt(_T("BG_index_max")+suffix,11000);
	pDlg->GG_index_min=ReadConfigInt(_T("GG_index_min")+suffix,9000);
	pDlg->GG_index_max=ReadConfigInt(_T("GG_index_max")+suffix,11000);
	pDlg->index_error=ReadConfigInt(_T("index_error")+suffix,100);

	pDlg->ByteCount=ReadConfigString(_T("ByteCount"), _T("1"));
	pDlg->A_Multi=ReadConfigString(_T("A_Multi"), _T("1"));
	pDlg->B_Multi=ReadConfigString(_T("B_Multi"), _T("0"));
	pDlg->Station=ReadConfigString(_T("TestStation"), _T("OHWBA06"));
	pDlg->SingleChannelEnable=ReadConfigInt(_T("SingleChannelEnable"),1);
	pDlg->GainEnable=ReadConfigInt(_T("GainEnable"),0);
	pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
	pDlg->m_index=ReadConfigInt(_T("index"),512);
	pDlg->m_Ratio=ReadConfigInt(_T("Ratio"),5);
	m_SlaveID= ReadConfigInt(_T("SlaveID"), 0x20);
	R_StartAdd=ReadConfigInt(_T("R_StartAdd")+suffix, 0x0000);
	Gr_StartAdd=ReadConfigInt(_T("Gr_StartAdd")+suffix, 0x0000);
	Gb_StartAdd=ReadConfigInt(_T("Gb_StartAdd")+suffix, 0x0000);
	B_StartAdd=ReadConfigInt(_T("B_StartAdd")+suffix, 0x0000);
	RG_StartAdd=ReadConfigInt(_T("RG_StartAdd")+suffix, 0x0000);
	BG_StartAdd=ReadConfigInt(_T("BG_StartAdd")+suffix, 0x0000);
	GG_StartAdd=ReadConfigInt(_T("GG_StartAdd")+suffix, 0x0000);
	ByteCount=ReadConfigInt(_T("ByteCount"), 1);
	A_Multi=ReadConfigInt(_T("A_Multi"), 1);
    B_Multi=ReadConfigInt(_T("B_Multi"), 0);
	pDlg->m_Raw8_EN  = ReadConfigInt(_T("Raw8_EN"),1);
	pDlg->page=ReadConfigInt(_T("page")+suffix,1);

	pDlg->m_ColorNum  = ReadConfigInt(_T("m_ColorNum"),1);
	pDlg->m_ModuleNum  = ReadConfigInt(_T("m_ModuleNum"),3);
	pDlg->m_isSingleModule = ReadConfigInt(_T("isSingleModule"),0);

	SetName(CurrentName);


	//切换光源
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(CurrentName);
	pDlg->m_ColorName=ReadConfigString(_T("m_ColorName"), 0);
	
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
º¯ÊýÃû:    SaveOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ½«±äÁ¿ÊýÖµ»ò¶Ô»°¿òµÄ°ó¶¨±äÁ¿ÊýÖµ±£´æµ½ÅäÖÃÎÄ¼þÖÐ (¿Ø¼þµÄÖµ¡ª>±äÁ¿->ÎÄ¼þ´æ´¢Öµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int GetDNPIndex_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;

	WriteConfigInt(_T("count"),pDlg->m_count);
	
	CString suffix=_T("");
	if (pDlg->m_count!=0)
		suffix.Format(_T("_%d"),pDlg->m_count);
	SetName(GlobalSetting);
	//TODO: ÔÚ´ËÌí¼Ó´æ´¢´úÂë	
	WriteConfigInt(_T("QTITool"),pDlg->m_QTITool);
	WriteConfigInt(_T("flow"),pDlg->m_flow);
	 WriteConfigInt(_T("DNPDistance")+suffix,pDlg->m_DNPDistance);
	 WriteConfigInt(_T("GG_GainEn"),pDlg->m_GG_GainEn);
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		GAverFlag=1;
	}
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		GAverFlag=2;
	}
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO3))->GetCheck())
	{
		GAverFlag=3;
	}
	WriteConfigInt(_T("GAverFlag"),GAverFlag);
	for (int i=0;i<20;i++)
	{
		CString temp;
		temp.Format(_T("FuseID_%d"),i+1);
		WriteConfigString(temp+suffix,pDlg->FuseID[i]);
	}
	WriteConfigString(_T("SlaveID"), pDlg->m_SlaveID);
	WriteConfigString(_T("R_StartAdd")+suffix, pDlg->R_StartAdd);
	WriteConfigString(_T("Gr_StartAdd")+suffix, pDlg->Gr_StartAdd);
	WriteConfigString(_T("Gb_StartAdd")+suffix, pDlg->Gb_StartAdd);
	WriteConfigString(_T("B_StartAdd")+suffix, pDlg->B_StartAdd);
	WriteConfigString(_T("RG_StartAdd")+suffix, pDlg->RG_StartAdd);
	WriteConfigString(_T("BG_StartAdd")+suffix, pDlg->BG_StartAdd);
	WriteConfigString(_T("GG_StartAdd")+suffix, pDlg->GG_StartAdd);
	WriteConfigInt(_T("R_min")+suffix,pDlg->R_min);
	WriteConfigInt(_T("R_max")+suffix,pDlg->R_max);
	WriteConfigInt(_T("Gr_min")+suffix,pDlg->Gr_min);
	WriteConfigInt(_T("Gr_max")+suffix,pDlg->Gr_max);
	WriteConfigInt(_T("Gb_min")+suffix,pDlg->Gb_min);
	WriteConfigInt(_T("Gb_max")+suffix,pDlg->Gb_max);
	WriteConfigInt(_T("B_min")+suffix,pDlg->B_min);
	WriteConfigInt(_T("B_max")+suffix,pDlg->B_max);
	WriteConfigInt(_T("RG_min")+suffix,pDlg->RG_min);
	WriteConfigInt(_T("RG_max")+suffix,pDlg->RG_max);
	WriteConfigInt(_T("BG_min")+suffix,pDlg->BG_min);
	WriteConfigInt(_T("BG_max")+suffix,pDlg->BG_max);
	WriteConfigInt(_T("GG_min")+suffix,pDlg->GG_min);
	WriteConfigInt(_T("GG_max")+suffix,pDlg->GG_max);
	WriteConfigInt(_T("R_index_min")+suffix,pDlg->R_index_min);
	WriteConfigInt(_T("R_index_max")+suffix,pDlg->R_index_max);
	WriteConfigInt(_T("Gr_index_min")+suffix,pDlg->Gr_index_min);
	WriteConfigInt(_T("Gr_index_max")+suffix,pDlg->Gr_index_max);
	WriteConfigInt(_T("Gb_index_min")+suffix,pDlg->Gb_index_min);
	WriteConfigInt(_T("Gb_index_max")+suffix,pDlg->Gb_index_max);
	WriteConfigInt(_T("B_index_min")+suffix,pDlg->B_index_min);
	WriteConfigInt(_T("B_index_max")+suffix,pDlg->B_index_max);
	WriteConfigInt(_T("RG_index_min")+suffix,pDlg->RG_index_min);
	WriteConfigInt(_T("RG_index_max")+suffix,pDlg->RG_index_max);
	WriteConfigInt(_T("BG_index_min")+suffix,pDlg->BG_index_min);
	WriteConfigInt(_T("BG_index_max")+suffix,pDlg->BG_index_max);
	WriteConfigInt(_T("GG_index_min")+suffix,pDlg->GG_index_min);
	WriteConfigInt(_T("GG_index_max")+suffix,pDlg->GG_index_max);
	WriteConfigInt(_T("index_error")+suffix,pDlg->index_error);
	WriteConfigString(_T("ByteCount"), pDlg->ByteCount);
	WriteConfigString(_T("A_Multi"), pDlg->A_Multi);
	WriteConfigString(_T("B_Multi"), pDlg->B_Multi);
	WriteConfigString(_T("TestStation"), pDlg->Station);
	WriteConfigInt(_T("SingleChannelEnable"),pDlg->SingleChannelEnable);
	WriteConfigInt(_T("GainEnable"),pDlg->GainEnable);
	WriteConfigInt(_T("Ratio"),pDlg->m_Ratio);
	WriteConfigInt(_T("BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("index"),pDlg->m_index);
	WriteConfigInt(_T("Raw8_EN"), pDlg->m_Raw8_EN);
	WriteConfigInt(_T("page")+suffix, pDlg->page);

	WriteConfigInt(_T("m_ColorNum"), pDlg->m_ColorNum);
	WriteConfigInt(_T("m_ModuleNum"), pDlg->m_ModuleNum);
	WriteConfigInt(_T("isSingleModule"),pDlg->m_isSingleModule);

	SetName(CurrentName);

	//切换光源
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(CurrentName);
	WriteConfigString(_T("m_ColorName"), pDlg->m_ColorName);
	LoadOption();
	//........................
	return 0;
}


/******************************************************************
º¯ÊýÃû:    GetReportHeaders
º¯Êý¹¦ÄÜ:  ±£´æÐèÒª±£´æµÄ²âÊÔÏîÄ¿Ãû×ÖÖÖÀà
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR GetDNPIndex_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	return header;
}


/******************************************************************
º¯ÊýÃû:    GetReportLowerLimits
º¯Êý¹¦ÄÜ:  ±£´æ²âÊÔ±ê×¼µÄÏÂÏÞÖµ
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR GetDNPIndex_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
º¯ÊýÃû:    GetReportUpperLimits
º¯Êý¹¦ÄÜ:  ±£´æ²âÊÔ±ê×¼µÄÉÏÏÞÖµ
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR GetDNPIndex_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
º¯ÊýÃû:    GetReportContents
º¯Êý¹¦ÄÜ:  ±£´æ¶ÔÓ¦²âÊÔÃû×ÖÖÖÀàµÄ²âÊÔµÄÊý¾Ý
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR GetDNPIndex_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}
void GetDNPIndex_TestItem::GetOTPData(WBInf &MyWBInf)
{
	if (ByteCount==1)
	{
		if (pDlg->SingleChannelEnable)
		{
			m_pInterface->Otp_OtpRead(R_StartAdd,R_StartAdd,&MyWBInf.OTP_R,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(Gr_StartAdd,Gr_StartAdd,&MyWBInf.OTP_Gr,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(Gb_StartAdd,Gb_StartAdd,&MyWBInf.OTP_Gb,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(B_StartAdd,B_StartAdd,&MyWBInf.OTP_B,CamID,pDlg->page);
		}
		if (pDlg->GainEnable)
		{
			m_pInterface->Otp_OtpRead(RG_StartAdd,RG_StartAdd,&MyWBInf.OTP_RG,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(BG_StartAdd,BG_StartAdd,&MyWBInf.OTP_BG,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(GG_StartAdd,GG_StartAdd,&MyWBInf.OTP_GG,CamID,pDlg->page);
		}

	}
	else if (ByteCount==2)
	{
		if (pDlg->SingleChannelEnable)
		{
			USHORT OTPData[2]={0};
			m_pInterface->Otp_OtpRead(R_StartAdd,R_StartAdd+1,OTPData,CamID,pDlg->page);
			MyWBInf.OTP_R=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(Gr_StartAdd,Gr_StartAdd+1,OTPData,CamID,pDlg->page);
			MyWBInf.OTP_Gr=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(Gb_StartAdd,Gb_StartAdd+1,OTPData,CamID,pDlg->page);
			MyWBInf.OTP_Gb=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(B_StartAdd,B_StartAdd+1,OTPData,CamID,pDlg->page);
			MyWBInf.OTP_B=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
		}
		if (pDlg->GainEnable)
		{
			USHORT OTPData[2]={0};
			m_pInterface->Otp_OtpRead(RG_StartAdd,RG_StartAdd+1,OTPData,CamID,pDlg->page);
			MyWBInf.OTP_RG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(BG_StartAdd,BG_StartAdd+1,OTPData,CamID,pDlg->page);
			MyWBInf.OTP_BG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(GG_StartAdd,GG_StartAdd+1,OTPData,CamID,pDlg->page);
			MyWBInf.OTP_GG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
		}
	}
	CString temp;
	if (pDlg->SingleChannelEnable)
	{
		float AA=0.0;
		if (pDlg->m_flow)
		{
			AA=0.5;
		}
		if(GAverFlag==1)
		{
			MyWBInf.OTP_RG = int( (float)pDlg->m_index*2.0*(MyWBInf.OTP_R)/(float)((MyWBInf.OTP_Gr + MyWBInf.OTP_Gb)) + AA  );// Rave/Gave
			MyWBInf.OTP_BG = int( (float)pDlg->m_index*2.0*(MyWBInf.OTP_B)/(float)((MyWBInf.OTP_Gr + MyWBInf.OTP_Gb)) + AA );  //Bave/Gave 
			MyWBInf.OTP_GG =int( (float)pDlg->m_index*2.0*(MyWBInf.OTP_Gb)/(float)((MyWBInf.OTP_Gr+ MyWBInf.OTP_Gb))+ AA);
		}
		else if (GAverFlag==2)
		{
			MyWBInf.OTP_RG = int( (float)pDlg->m_index*(MyWBInf.OTP_R)/(float)(MyWBInf.OTP_Gr) + AA  );// Rave/Gave
			MyWBInf.OTP_BG = int( (float)pDlg->m_index*(MyWBInf.OTP_B)/(float)(MyWBInf.OTP_Gr) + AA );  //Bave/Gave 
			MyWBInf.OTP_GG =int( (float)pDlg->m_index*(MyWBInf.OTP_Gb)/(float)(MyWBInf.OTP_Gr)+AA);
		}
		else if (GAverFlag==3)
		{
			MyWBInf.OTP_RG = int( (float)pDlg->m_index*(MyWBInf.OTP_R)/(float)(MyWBInf.OTP_Gb) + AA  );// Rave/Gave
			MyWBInf.OTP_BG = int( (float)pDlg->m_index*(MyWBInf.OTP_B)/(float)(MyWBInf.OTP_Gb) + AA );  //Bave/Gave 
			MyWBInf.OTP_GG =int( (float)pDlg->m_index*(MyWBInf.OTP_Gr)/(float)(MyWBInf.OTP_Gb)+AA);
		}

		temp.Format(_T("当前模组 OTP_R=%d"),MyWBInf.OTP_R);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_Gr=%d"),MyWBInf.OTP_Gr);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_Gb=%d"),MyWBInf.OTP_Gb);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_B=%d"),MyWBInf.OTP_B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		temp.Format(_T("当前模组 OTP_RG=%d"),MyWBInf.OTP_RG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_BG=%d"),MyWBInf.OTP_BG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_GG=%d"),MyWBInf.OTP_GG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	if (pDlg->GainEnable)
	{
		temp.Format(_T("当前模组 OTP_RG=%d"),MyWBInf.OTP_RG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_BG=%d"),MyWBInf.OTP_BG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_GG=%d"),MyWBInf.OTP_GG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
}

void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}
BOOL GetDNPIndex_TestItem::GetAWBData(ImageInfo img,WBInf &MyWbInf)
{
	CString ToolPath;
	ToolPath.Format(_T("AWBData\\Cam%d\\"),CamID);
	ToolPath=m_pInterface->Ctrl_GetCurrentPath()+ToolPath;
	SetCurrentDirectory(ToolPath);

	int R,Gr,Gb,B;
	CString sfilename=ToolPath+_T("AWB_LSC_CALIBRATION_DATA.txt");
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB_LSC_CALIBRATION_DATA.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt");
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt");
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}

	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7);

	HINSTANCE hDll;   //DLL句柄 

//	SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());		
	sfilename=ToolPath+_T("LSCCalibrationDll.dll");
	hDll = LoadLibrary(sfilename);
	if (NULL==hDll)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("LSCCalibrationDll.dll丢失,请将文件放回程式目录!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}

	lpFun LensCorrection = (lpFun)GetProcAddress(hDll,"LensCorrectionLib");
	if (NULL==LensCorrection)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("DLL中函数寻找失败!!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}
	int  width   = img.imgwidth;
	int  height   =   img.imgheight;
	BYTE * Raw8buffer=new BYTE[width*height];
	if (NULL == Raw8buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Raw8buffer buffer 申请失败!!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}
	Raw10toRaw8(img.Raw_buffer,Raw8buffer,width*height);

	int CFA_Pattern=0;
	if ((img.RawFormat == 0) ||
		(img.RawFormat == 1))
	{
		CFA_Pattern =img.RawFormat;
	}
	if (img.RawFormat == 2)
	{
		CFA_Pattern = 3;
	}
	if (img.RawFormat == 3)
	{
		CFA_Pattern = 2;
	}

	LensCorrection(Raw8buffer,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,0);

	SAFE_DELETE_ARRAY(Raw8buffer);
	FreeLibrary(hDll);
	CStdioFile file;
	CString Path = ToolPath;
	Path = Path + _T("AWB_LSC_CALIBRATION_DATA.txt");

	if(file.Open(Path , CFile::modeRead | CFile::typeText ))
	{
		CString strTemp;
		CString a;
		while(file.ReadString(strTemp))
		{
			if (strTemp.Find(_T("r_oc_ave"),0) == -1)
			{
				continue;
			}
			else
			{
				float temp=0.00;
				a = strTemp.Mid(10,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				R=(temp+0.5); //R


				file.ReadString(strTemp);
				a = strTemp.Mid(10,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				B=(temp+0.5); //B

				file.ReadString(strTemp);
				a = strTemp.Mid(11,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				Gr=(temp+0.5); //Gr

				file.ReadString(strTemp);
				a = strTemp.Mid(11,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				Gb=(temp+0.5); //Gb
				break;

			}	
		}
		file.Close();
	}
	else {
		m_pInterface->Ctrl_Addlog(CamID,_T("打开AWB_LSC_CALIBRATION_DATA.txt失败!"),COLOR_RED,200);
		return FALSE;
	}
	MyWbInf.Gr = int(Gr);
	MyWbInf.Gb = int(Gb);
	MyWbInf.R  = int(R);
	MyWbInf.B  = int(B);

	CString tempStr;
	tempStr.Format(_T("R = %d"),MyWbInf.R);
	m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_BLUE,200);

	tempStr.Format(_T("Gr = %d"),MyWbInf.Gr);
	m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_BLUE,200);

	tempStr.Format(_T("Gb = %d"),MyWbInf.Gb);
	m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_BLUE,200);

	tempStr.Format(_T("B = %d"),MyWbInf.B);
	m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_BLUE,200);

	int index=pDlg->m_index;

	tempStr.Format(_T("Index = %d"),index);
	m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_BLUE,200);

	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	if(GAverFlag==1)
	{
		MyWbInf.RG = int( (float)index*2.0*(MyWbInf.R)/(float)((MyWbInf.Gr + MyWbInf.Gb)) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)index*2.0*(MyWbInf.B)/(float)((MyWbInf.Gr + MyWbInf.Gb)) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)index*2.0*(MyWbInf.Gb)/(float)((MyWbInf.Gr+ MyWbInf.Gb))+ AA);
	}
	else if (GAverFlag==2)
	{
		MyWbInf.RG = int( (float)index*(MyWbInf.R)/(float)(MyWbInf.Gr) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)index*(MyWbInf.B)/(float)(MyWbInf.Gr) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)index*(MyWbInf.Gb)/(float)(MyWbInf.Gr)+AA);
	}
	else if (GAverFlag==3)
	{
		MyWbInf.RG = int( (float)index*(MyWbInf.R)/(float)(MyWbInf.Gb) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)index*(MyWbInf.B)/(float)(MyWbInf.Gb) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)index*(MyWbInf.Gr)/(float)(MyWbInf.Gb)+AA);
	}
	return TRUE;
}
