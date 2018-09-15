#include "StdAfx.h"
#include "GetDNPIndex_Moto_TestItem.h"
#include "ImageProc.h"

#include <direct.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new GetDNPIndex_Moto_TestItem(pInterface, lpszName);
}


/******************************************************************
º¯ÊýÃû:    ¹¹Ôìº¯Êý
º¯Êý¹¦ÄÜ:  ¶ÔÏóÉú³ÉµÄÊ±ºò³õÊ¼±ØÐëÍê³ÉµÄÅäÖÃ
*******************************************************************/
GetDNPIndex_Moto_TestItem::GetDNPIndex_Moto_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

	memset((void *)(&m_CurSum), 0x00, sizeof(m_CurSum));
	memset((void *)(&m_CurVal), 0x00, sizeof(m_CurVal));
	//memset((void *)(&m_TypVal), 0x00, sizeof(m_TypVal));
}

GetDNPIndex_Moto_TestItem::~GetDNPIndex_Moto_TestItem(void)
{

}


int GetDNPIndex_Moto_TestItem::InitItem()
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
int GetDNPIndex_Moto_TestItem::Initialize()
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

void GetDNPIndex_Moto_TestItem::SetAutoLightStatus(int nsel)
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
#if 0
int GetDNPIndex_Moto_TestItem::Testing()
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
 	SetName(GlobalSetting);	
 	GlobalTime = ReadConfigInt(_T("GlobalTime"), 3);
 	int CurrentTagTime = ReadConfigInt(_T("CurrentTimeTag"), 2);
 	SetName(CurrentName);

	int NowColorNum = 0;	//色温数[单色温、两色温、三色温]
	if (pDlg->m_count == 0)
	{
		NowColorNum = 1;
	}
	if (pDlg->m_count == 3)
	{
		NowColorNum = 2;
	}
	if (pDlg->m_count == 6)
	{
		NowColorNum = 3;
	}
	if (pDlg->m_count == 9)
	{
		NowColorNum = 4;
	}

	/**
	*	点击主窗口菜单[光源系数校准]->[打开光源校准]，会在ini里面赋值"GlobalTime"为[0]，从而让
	*	此处能够运行下去.
	*	--Added by LiHai--20171221
	**/
	CString AFXmsg;
	if ((GlobalTime >= (pDlg->m_count + 3)) || (GlobalTime < pDlg->m_count))
	{
		return 0;
	}

	/** 光源点检防呆 **/
	if (FALSE == GetLightSource())
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("GetLightSource() Fail\r\n"), COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("GetLightSource() Fail\r\n"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	if (SetLEDFlag)
	{
		if (GlobalTime == pDlg->m_count)
		{
			AFXmsg = _T("请放入") + pDlg->m_ColorName + _T("色温模组进行校正!");
			AfxMessageBox(AFXmsg);
			GlobalTime2 = 0;//初始化
		}
	}
	
	int iTimeTag = GetTimeTag();
	if (GlobalTime2 != 0)//猜测:是否是说某次光源点检持续的时间超过两个小时，就重新复位点检模组数，需要重新开始点检--比如总共需要点检5颗模组，而此时先点检了三颗，然后有事就耽误了两个小时，最后接着点检，这时就需要重新开始点检--Added by LiHai--20171221
	{
		if (abs(iTimeTag - CurrentTagTime) > 1)//时间过长不能矫正
		{
			GlobalTime = 0;
		}
	}

	CString Caltime;
	Caltime.Format(_T("第%d颗模组开始校正.."), GlobalTime2 + 1);
	m_pInterface->Ctrl_Addlog(CamID, Caltime, COLOR_BLUE, 200);
	CString CurrentFuseID = m_pInterface->Sensor_GetCamFuseID(CamID);
	m_pInterface->Ctrl_Addlog(CamID, CurrentFuseID, COLOR_BLUE, 200);
	int i=0;
	for (i=0;i<20;i++)
	{
		if(!CurrentFuseID.Compare(pDlg->FuseID[i]))
		{
			break;
		}
	}
	if (i>=20)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("非注册模组!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("非注册模组!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	
	for (i = 0; i < GlobalTime2; i++)//已经校准了"GlobalTime2"颗Golden模组--Added by LiHai--20171221
	{
		if (CurrentFuseID == MyfuseID[i])//判断此模组是否已经点检过，防止一颗模组多次点检--Added by LiHai--20171221
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("该模组已注册!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("该模组已注册!"));
// 			SetResult(RESULT_FAIL);
// 			return 0;
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
		return 0;
	}
	if (pDlg->m_QTITool)
	{
		if (!GetAWBData(MyImageInfo[0], MyWBInf[GlobalTime2]))
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("获取AWB失败!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("获取AWB失败!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	else
	{
	   CenterBlockRawInfo(MyImageInfo, pDlg->m_ROI_H, MyWBInf[GlobalTime2], pDlg->m_Multiple, pDlg->m_BLC);
	}
	if (pDlg->SingleChannelEnable)
	{
		BOOL WBResult = TRUE;
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
			return 0;
		}
	}
	if (pDlg->GainEnable)
	{
		BOOL WBResult=TRUE;
		CString temp;
#if 0
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
			return 0;
		}

		float m_OTPDistance=0;
		if (pDlg->m_GG_GainEn)
		{
			m_OTPDistance=(float)sqrt(pow((MyWBInf[GlobalTime2].RG-MyWBInf[GlobalTime2].OTP_RG)*1.0/MyWBInf[GlobalTime2].OTP_RG,2) + pow((MyWBInf[GlobalTime2].BG-MyWBInf[GlobalTime2].OTP_BG)*1.0/MyWBInf[GlobalTime2].OTP_BG,2)+pow((MyWBInf[GlobalTime2].GG-MyWBInf[GlobalTime2].OTP_GG)*1.0/MyWBInf[GlobalTime2].OTP_GG,2));
		}
		else
		{
		    m_OTPDistance=(float)sqrt(pow((MyWBInf[GlobalTime2].RG-MyWBInf[GlobalTime2].OTP_RG)*1.0/MyWBInf[GlobalTime2].OTP_RG,2) + pow((MyWBInf[GlobalTime2].BG-MyWBInf[GlobalTime2].OTP_BG)*1.0/MyWBInf[GlobalTime2].OTP_BG,2));
		}
		temp.Format(_T("Distance=%f"),m_OTPDistance*100);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		if (m_OTPDistance * 1000 > pDlg->m_DNPDistance)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Distance 超出范围！"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Distance 超出范围!"));
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
			return 0;
		}
#else
		temp.Format(_T("当前影像 R_index=%d <%d,%d>"),MyWBInf[GlobalTime2].R_index,pDlg->R_index_min,pDlg->R_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 Gr_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr_index,pDlg->Gr_index_min,pDlg->Gr_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 Gb_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb_index,pDlg->Gb_index_min,pDlg->Gb_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 B_index=%d <%d,%d>"),MyWBInf[GlobalTime2].B_index,pDlg->B_index_min,pDlg->B_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 RG=%d <%d,%d>"),MyWBInf[GlobalTime2].RG,pDlg->RG_min,pDlg->RG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 BG=%d <%d,%d>"),MyWBInf[GlobalTime2].BG,pDlg->BG_min,pDlg->BG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 GG=%d <%d,%d>"),MyWBInf[GlobalTime2].GG,pDlg->GG_min,pDlg->GG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		if ((MyWBInf[GlobalTime2].R > 1023)
			|| (MyWBInf[GlobalTime2].Gr > 1023)
			|| (MyWBInf[GlobalTime2].Gb > 1023)
			|| (MyWBInf[GlobalTime2].B > 1023)
			)
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("R或Gr或Gb或B值超出范围,请重新测试"), COLOR_RED, 200);
			return 0;
		}
		m_CurVal.m_R = MyWBInf[GlobalTime2].R;
		m_CurVal.m_Gr = MyWBInf[GlobalTime2].Gr;
		m_CurVal.m_Gb = MyWBInf[GlobalTime2].Gb;
		m_CurVal.m_B = MyWBInf[GlobalTime2].B;
		m_CurVal.m_RGain = MyWBInf[GlobalTime2].RG;
		m_CurVal.m_BGain = MyWBInf[GlobalTime2].BG;
		m_CurVal.m_GGain = MyWBInf[GlobalTime2].GG;

		temp.Format(_T("Typical_R = %d"), pDlg->m_TypVal.m_R);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
		temp.Format(_T("Typical_Gr = %d"), pDlg->m_TypVal.m_Gr);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
		temp.Format(_T("Typical_Gb = %d"), pDlg->m_TypVal.m_Gb);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
		temp.Format(_T("Typical_B = %d"), pDlg->m_TypVal.m_B);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
		temp.Format(_T("Typical_RG = %d"), pDlg->m_TypVal.m_RGain);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
		temp.Format(_T("Typical_BG = %d"), pDlg->m_TypVal.m_BGain);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
		temp.Format(_T("Typical_GG = %d"), pDlg->m_TypVal.m_GGain);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);

		/** 计算公差 **/
		float ErrorValue = sqrt(pow((float)(m_CurVal.m_RGain - pDlg->m_TypVal.m_RGain), 2) + pow((float)(m_CurVal.m_BGain - pDlg->m_TypVal.m_BGain), 2));
		if (ErrorValue > pDlg->m_CurrentDiff)
		{
			temp.Format(_T("Error:%.4f > Spec:%.4f  该模组Error超范围!"), ErrorValue, pDlg->m_CurrentDiff);
			m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
			return 0;
		}
		temp.Format(_T("Error:%.4f > Spec:%.4f"), ErrorValue, pDlg->m_CurrentDiff);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);

		m_CurSum.m_R  += m_CurVal.m_R;
		m_CurSum.m_Gr += m_CurVal.m_Gr;
		m_CurSum.m_Gb += m_CurVal.m_Gb;
		m_CurSum.m_B  += m_CurVal.m_B;

		m_CurSum.m_RGain += m_CurVal.m_RGain;
		m_CurSum.m_BGain += m_CurVal.m_BGain;
		m_CurSum.m_GGain += m_CurVal.m_GGain;

		CTime time = CTime::GetCurrentTime();
		DnpInfo[GlobalTime2].FuseId = CurrentFuseID;
		DnpInfo[GlobalTime2].month = time.GetMonth();
		DnpInfo[GlobalTime2].day = time.GetDay();
		DnpInfo[GlobalTime2].hour = time.GetHour();
		DnpInfo[GlobalTime2].min = time.GetMinute();
		DnpInfo[GlobalTime2].sec = time.GetSecond();

		DnpInfo[GlobalTime2].R_ave = m_CurVal.m_R;
		DnpInfo[GlobalTime2].Gr_ave = m_CurVal.m_Gr;
		DnpInfo[GlobalTime2].Gb_ave = m_CurVal.m_Gb;
		DnpInfo[GlobalTime2].B_ave = m_CurVal.m_B;
		DnpInfo[GlobalTime2].RG = m_CurVal.m_RGain;
		DnpInfo[GlobalTime2].BG = m_CurVal.m_BGain;
		DnpInfo[GlobalTime2].GG = m_CurVal.m_GGain;
		DnpInfo[GlobalTime2].Error = ErrorValue;
#endif
	}
	GlobalTime++;
	GlobalTime2++;
	CString temp;
#if 0
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
			temp.Format(_T("%d %d %d %d %d %d %d %d %d %d %d %d %d"),i,MyWBInf[i-1].R,MyWBInf[i-1].Gr,MyWBInf[i-1].Gb,MyWBInf[i-1].B,MyWBInf->OTP_R,MyWBInf->OTP_Gr,MyWBInf->OTP_Gb,MyWBInf->OTP_B,MyWBInf[i-1].R_index,MyWBInf[i-1].Gr_index,MyWBInf[i-1].Gb_index,MyWBInf[i-1].B_index);
			if (GlobalTime2==pDlg->m_ModuleNum)
			{
				MyWBInf[GlobalTime2].R_index+=MyWBInf[i-1].R_index;
				MyWBInf[GlobalTime2].Gr_index+=MyWBInf[i-1].Gr_index;
				MyWBInf[GlobalTime2].Gb_index+=MyWBInf[i-1].Gb_index;
				MyWBInf[GlobalTime2].B_index+=MyWBInf[i-1].B_index;
				if (i==1)
				{
					MyWBInf[GlobalTime2].R_index=(float)MyWBInf[GlobalTime2].R_index/3.0+0.5;
					MyWBInf[GlobalTime2].Gr_index=(float)MyWBInf[GlobalTime2].Gr_index/3.0+0.5;
					MyWBInf[GlobalTime2].Gb_index=(float)MyWBInf[GlobalTime2].Gb_index/3.0+0.5;
					MyWBInf[GlobalTime2].B_index=(float)MyWBInf[GlobalTime2].B_index/3.0+0.5;
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
					MyWBInf[GlobalTime2].RG_index=(float)MyWBInf[GlobalTime2].RG_index/3.0+0.5;
					MyWBInf[GlobalTime2].BG_index=(float)MyWBInf[GlobalTime2].BG_index/3.0+0.5;
					MyWBInf[GlobalTime2].GG_index=(float)MyWBInf[GlobalTime2].GG_index/3.0+0.5;
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
					MyWBInf[GlobalTime2].R_index=(float)MyWBInf[GlobalTime2].R_index/3.0+0.5;
					MyWBInf[GlobalTime2].Gr_index=(float)MyWBInf[GlobalTime2].Gr_index/3.0+0.5;
					MyWBInf[GlobalTime2].Gb_index=(float)MyWBInf[GlobalTime2].Gb_index/3.0+0.5;
					MyWBInf[GlobalTime2].B_index=(float)MyWBInf[GlobalTime2].B_index/3.0+0.5;
					MyWBInf[GlobalTime2].RG_index=(float)MyWBInf[GlobalTime2].RG_index/3.0+0.5;
					MyWBInf[GlobalTime2].BG_index=(float)MyWBInf[GlobalTime2].BG_index/3.0+0.5;
					MyWBInf[GlobalTime2].GG_index=(float)MyWBInf[GlobalTime2].GG_index/3.0+0.5;
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
#endif
	///导入规格//
	if (GlobalTime2 == pDlg->m_ModuleNum)
	{
#if 0
		if(WBResult==1||WBResult==2)
		{
			//GlobalTime--;
			CString templog=_T("index平均值不在范围内!");
			if(WBResult==2) templog=_T("index error不在范围内!");
			m_pInterface->Ctrl_Addlog(CamID,templog,COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,templog);
			SetResult(RESULT_FAIL);
			memset(&MyWBInf[GlobalTime2],0,sizeof(WBInf));
			return 0;
		}
#else
		m_CurVal.m_R  = int(m_CurSum.m_R * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_Gr = int(m_CurSum.m_Gr * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_Gb = int(m_CurSum.m_Gb * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_B  = int(m_CurSum.m_B * 1.0 / GlobalTime2 + 0.5);

		/*
		*	接刘月琴通知，客户要求以下三个GAIN值的计算公式由一开始的各自的平均值改为用通道的平均值计算。
		*	--Added by LiHai--20171110
		*/
	#if 0
		m_CurVal.m_RGain = int(m_CurSum.m_RGain * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_BGain = int(m_CurSum.m_BGain * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_GGain = int(m_CurSum.m_GGain * 1.0 / GlobalTime2 + 0.5);
	#else
		m_CurVal.m_RGain = int((m_CurVal.m_R - pDlg->m_BLC) * pDlg->m_Multiple / (m_CurVal.m_Gr + m_CurVal.m_Gb - 2 * pDlg->m_BLC) * 2 + 0.5);
		m_CurVal.m_BGain = int((m_CurVal.m_B - pDlg->m_BLC) * pDlg->m_Multiple / (m_CurVal.m_Gr + m_CurVal.m_Gb - 2 * pDlg->m_BLC) * 2 + 0.5);
		m_CurVal.m_GGain = int(m_CurVal.m_Gr * pDlg->m_Multiple / m_CurVal.m_Gb + 0.5);
	#endif

		/*
		*	接刘月琴通知，客户要求以下三个GAIN值的计算公式由一开始的各自的平均值改为用通道的平均值计算。
		*	注意和上面的计算公式相比，此处不能四舍五入。
		*	--Added by LiHai--20171118
		*/
#if 0
		float RGNew_ave =float(m_CurSum.m_RGain * 1.0 / GlobalTime2);
		float BGNew_ave =float(m_CurSum.m_BGain * 1.0 / GlobalTime2);
#else
		float RGNew_ave = (m_CurVal.m_R - pDlg->m_BLC) * pDlg->m_Multiple / (m_CurVal.m_Gr + m_CurVal.m_Gb - 2 * pDlg->m_BLC) * 2;
		float BGNew_ave = (m_CurVal.m_B - pDlg->m_BLC) * pDlg->m_Multiple / (m_CurVal.m_Gr + m_CurVal.m_Gb - 2 * pDlg->m_BLC) * 2;
#endif
		float Error_ave = sqrt(pow((float)(RGNew_ave - pDlg->m_TypVal.m_RGain), 2) + pow((float)(BGNew_ave - pDlg->m_TypVal.m_BGain), 2));
		/**********************************************************************/
		/*                         动态点检算法   : add by Gary               */
		/**********************************************************************/	
		if (Error_ave < pDlg->m_GoldenDiff)
		{ 
			m_CurVal  = pDlg->m_TypVal;
			temp.Format(_T("Error_ave:%f < Spec:%f，使用Typical数据为Golden数据"), Error_ave, pDlg->m_GoldenDiff);
		}
		else
		{
			temp.Format(_T("Error_ave:%f > Spec:%f，使用Current AWB数据为Golden数据"), Error_ave, pDlg->m_GoldenDiff);
		}
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);

		SaveMotoDynamic_DNPData();

		RegSetKey(_T("LightSource"), _T(""), _T(""));    //将LightSource数据清空
		RegSetKey(_T("LightSource"), _T("5100k_Ev") ,_T(""));
		RegSetKey(_T("LightSource"), _T("5100k_u"), _T(""));
		RegSetKey(_T("LightSource"), _T("5100k_v"), _T(""));
#endif
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
		WriteConfigInt(_T("CalTimeTag"), iTimeTag);//此处把当前时间戳写入ini文件，PreProcess测试项里面会读取这个时间戳，所以GetDNPIndex_Moto必须在PreProcess之前--Added by LiHai--20171221
		CString suffix=_T("");
		if (pDlg->m_count!=0)
		{
			suffix.Format(_T("_%d"),pDlg->m_count);
		}

#if 0
		WriteConfigInt(_T("R_index")+suffix,MyWBInf[GlobalTime2].R_index);
		WriteConfigInt(_T("Gr_index")+suffix,MyWBInf[GlobalTime2].Gr_index);
		WriteConfigInt(_T("Gb_index")+suffix,MyWBInf[GlobalTime2].Gb_index);
		WriteConfigInt(_T("B_index")+suffix,MyWBInf[GlobalTime2].B_index);
		WriteConfigInt(_T("RG_index")+suffix,MyWBInf[GlobalTime2].RG_index);
		WriteConfigInt(_T("BG_index")+suffix,MyWBInf[GlobalTime2].BG_index);
		WriteConfigInt(_T("GG_index")+suffix,MyWBInf[GlobalTime2].GG_index);
#endif
		SetName(CurrentName);
		if (NowColorNum == pDlg->m_ColorNum)
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("光源校正完毕，开始测试"));
		}
		else if (NowColorNum < pDlg->m_ColorNum)
		{
			AFXmsg = pDlg->m_ColorName + _T("色温校正完毕，请换下一种色温模组");
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,AFXmsg);
		}
		SetResult(RESULT_NORMAL);
		memset(&MyWBInf[GlobalTime2], 0, sizeof(WBInf));
	}
	else
	{
		temp.Format(_T("第%d颗模组校正完成"),GlobalTime2);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,temp);
		SetResult(RESULT_NORMAL);
	}
	SetName(GlobalSetting);
	WriteConfigInt(_T("CurrentTimeTag"),iTimeTag);
	WriteConfigInt(_T("GlobalTime"),GlobalTime);
	SetName(CurrentName);
	m_pInterface->UnlocktheFile(CamID);//解锁加密狗
	MyfuseID[GlobalTime2-1]=CurrentFuseID;

   return 0;
}
#endif

int GetDNPIndex_Moto_TestItem::Testing()
{
	/**
	*	Moto光源点检方式特点和注意事项：
	*	1、静态点检：
			a、获取图片PIC；
			b、得到图片PIC按要求抓的框的单通道平均值，是个浮点数，RawR、RawGr、RawGb、RawB
			c、把[b]中的四个单通道值减去BLC后四舍五入取整，得到新的单通道值:RawR2、RawGr2、RawGb2、RawB2
			d、把[c]中的单通道值按照16384的公式计算对应的Gain值，结果四舍五入后取整，例如:RGain=INT(16384.0 * RawR2/((RawGr2 + RawGb2)/2.0) + 0.5 )
			e、使用EEPROM中的OTP_Golden的值除以c中的四个单通道值，四舍五入后取整，得到四个index光源系数。例如:Rindex=INT(1.0*R_OTP/RawR2 + 0.5)
			f、使用[e]中得到的四个单通道的光源系数index值，然后把每个通道都除以Gr_index值，刘月琴的解释是防止在ApplyAWB的测试项中校准单通道数据时出现G通道值超过曝光值的问题。
	*	2、动态点检：
			a、获取图片PIC；
			b、得到图片PIC按要求抓的框的单通道平均值，是个浮点数，RawR、RawGr、RawGb、RawB
			c、把[b]中的四个单通道值四舍五入取整(注意和静态相比此处不减BLC)，得到新的单通道值:RawR2、RawGr2、RawGb2、RawB2
			d、把[c]中的单通道值按照16384的公式计算对应的Gain值，结果四舍五入后取整，例如:RGain=INT(16384.0 * RawR2/((RawGr2 + RawGb2)/2.0) + 0.5 )
			e、这个新的Gain值和EEPROM中的OTP_Golden值进行方差比较，如果小于0.005，后面ApplyAWB中使用EEPROM中的OTP_Golden值进行校准，如果>=0.005，使用
				[d]中计算出来的Gain值作为后面ApplyAWB中校准的Golden值。
	**/

	switch (pDlg->m_MotoLightSrcMode)//动态点检
	{
	case MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC:
		{
			return MotoDynamicLightSrcCalibration();
		}
		break;

	case MOTO_LIGHT_SRC_CAL_MODE_STATIC:
		{
			return MotoStaticLightSrcCalibration();
		}
		break;

	default:
		break;
	}
	return 0;
}


int GetDNPIndex_Moto_TestItem::MotoStaticLightSrcCalibration(void)
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
 	SetName(GlobalSetting);	
 	GlobalTime = ReadConfigInt(_T("GlobalTime"), 3);
 	int CurrentTagTime = ReadConfigInt(_T("CurrentTimeTag"), 2);
 	SetName(CurrentName);

	int NowColorNum = 1;	//色温数[单色温、两色温、三色温]
	if (pDlg->m_count == 0)
	{
		NowColorNum = 1;
	}
	if (pDlg->m_count == 3)
	{
		NowColorNum = 2;
	}
	if (pDlg->m_count == 6)
	{
		NowColorNum = 3;
	}
	if (pDlg->m_count == 9)
	{
		NowColorNum = 4;
	}

	/**
	*	点击主窗口菜单[光源系数校准]->[打开光源校准]，会在ini里面赋值"GlobalTime"为[0]，从而让
	*	此处能够运行下去.
	*	--Added by LiHai--20171221
	**/
	CString AFXmsg;
	//if ((GlobalTime >= (pDlg->m_count + pDlg->m_ModuleNum)) || (GlobalTime < pDlg->m_count))//3修改成pDlg->m_ModuleNum
	if (GlobalTime > (pDlg->m_ModuleNum*pDlg->m_ColorNum))
	{
		GlobalTime2 = 0; //下次光源校正时从新计数，不然数组会越界--LYC 20180511
		return 0;
	}

	/** 光源点检防呆 **/
	if (FALSE == GetLightSource())
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("GetLightSource() Fail\r\n"), COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("GetLightSource() Fail\r\n"));
		SetResult(RESULT_FAIL);
		SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
		return 0;
	}

	//if (SetLEDFlag)
	{
		if (GlobalTime == pDlg->m_count)
		{
			AFXmsg = _T("请放入") + pDlg->m_ColorName + _T("色温模组进行校正!");
			AfxMessageBox(AFXmsg);
			GlobalTime2 = 0;//初始化
		}
	}
	
	int iTimeTag = GetTimeTag();
	if (GlobalTime2 != 0)//猜测:是否是说某次光源点检持续的时间超过两个小时，就重新复位点检模组数，需要重新开始点检--比如总共需要点检5颗模组，而此时先点检了三颗，然后有事就耽误了两个小时，最后接着点检，这时就需要重新开始点检--Added by LiHai--20171221
	{
		if (abs(iTimeTag - CurrentTagTime) > 1)//时间过长不能矫正
		{
			GlobalTime = 0;
			GlobalTime2 = 0;    //每隔一时重新校准--LYC 20180511
		}
	}

	CString Caltime;
	Caltime.Format(_T("第%d颗模组开始校正.."), GlobalTime2 + 1);
	m_pInterface->Ctrl_Addlog(CamID, Caltime, COLOR_BLUE, 200);
	CString CurrentFuseID = m_pInterface->Sensor_GetCamFuseID(CamID);
	m_pInterface->Ctrl_Addlog(CamID, CurrentFuseID, COLOR_BLUE, 200);
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
	
	for (i = 0; i < GlobalTime2; i++)//已经校准了"GlobalTime2"颗Golden模组--Added by LiHai--20171221
	{
		if (CurrentFuseID == MyfuseID[i])//判断此模组是否已经点检过，防止一颗模组多次点检--Added by LiHai--20171221
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("该模组已注册!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("该模组已注册!"));
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
		if (!GetAWBData(MyImageInfo[0], MyWBInf[GlobalTime2]))
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("获取AWB失败!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("获取AWB失败!"));
			SetResult(RESULT_FAIL);
			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;
		}
	}
	else
	{
	   CenterBlockRawInfo(MyImageInfo, pDlg->m_ROI_H, MyWBInf[GlobalTime2], pDlg->m_Multiple, pDlg->m_BLC);
	}
	if (pDlg->SingleChannelEnable)
	{
		BOOL WBResult = TRUE;
		CString temp;
		temp.Format(_T("当前影像 R=%d <%d,%d>"),MyWBInf[GlobalTime2].R,pDlg->R_min,pDlg->R_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gr=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr,pDlg->Gr_min,pDlg->Gr_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gb=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb,pDlg->Gb_min,pDlg->Gb_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 B=%d <%d,%d>"),MyWBInf[GlobalTime2].B,pDlg->B_min,pDlg->B_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
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
		MyWBInf[GlobalTime2].R_index=(MyWBInf[GlobalTime2].OTP_R*10000.0/(float)MyWBInf[GlobalTime2].R+0.5);
		MyWBInf[GlobalTime2].Gr_index=(MyWBInf[GlobalTime2].OTP_Gr*10000.0/(float)MyWBInf[GlobalTime2].Gr+0.5);
		MyWBInf[GlobalTime2].Gb_index=(MyWBInf[GlobalTime2].OTP_Gb*10000.0/(float)MyWBInf[GlobalTime2].Gb+0.5);
		MyWBInf[GlobalTime2].B_index=(MyWBInf[GlobalTime2].OTP_B*10000.0/(float)MyWBInf[GlobalTime2].B+0.5);
		/** 再次除以GrIndex,，刘月琴的解释:防止以后使用系数校准后的数据越过曝光值--Added by LiHai--20180507 **/
		MyWBInf[GlobalTime2].R_index=(MyWBInf[GlobalTime2].R_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].Gb_index=(MyWBInf[GlobalTime2].Gb_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].B_index=(MyWBInf[GlobalTime2].B_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].Gr_index=(MyWBInf[GlobalTime2].Gr_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		temp.Format(_T("当前影像 R_index=%d <%d,%d>"),MyWBInf[GlobalTime2].R_index,pDlg->R_index_min,pDlg->R_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gr_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr_index,pDlg->Gr_index_min,pDlg->Gr_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gb_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb_index,pDlg->Gb_index_min,pDlg->Gb_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 B_index=%d <%d,%d>"),MyWBInf[GlobalTime2].B_index,pDlg->B_index_min,pDlg->B_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
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
		{
		    m_OTPDistance=(float)sqrt(pow((MyWBInf[GlobalTime2].RG-MyWBInf[GlobalTime2].OTP_RG)*1.0/MyWBInf[GlobalTime2].OTP_RG,2) + pow((MyWBInf[GlobalTime2].BG-MyWBInf[GlobalTime2].OTP_BG)*1.0/MyWBInf[GlobalTime2].OTP_BG,2));
		}
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
			temp.Format(_T("%d %d %d %d %d %d %d %d %d %d %d %d %d"),
						i,
						MyWBInf[i-1].R,
						MyWBInf[i-1].Gr,
						MyWBInf[i-1].Gb,
						MyWBInf[i-1].B,
						MyWBInf[i-1].OTP_R,//修改MyWBInf->OTP_R 20180517
						MyWBInf[i-1].OTP_Gr,
						MyWBInf[i-1].OTP_Gb,
						MyWBInf[i-1].OTP_B,
						MyWBInf[i-1].R_index,
						MyWBInf[i-1].Gr_index,
						MyWBInf[i-1].Gb_index,
						MyWBInf[i-1].B_index
						);
			if (GlobalTime2==pDlg->m_ModuleNum)
			{
				MyWBInf[GlobalTime2].R_index+=MyWBInf[i-1].R_index;
				MyWBInf[GlobalTime2].Gr_index+=MyWBInf[i-1].Gr_index;
				MyWBInf[GlobalTime2].Gb_index+=MyWBInf[i-1].Gb_index;
				MyWBInf[GlobalTime2].B_index+=MyWBInf[i-1].B_index;
				if (i==1)
				{
					MyWBInf[GlobalTime2].R_index=(float)MyWBInf[GlobalTime2].R_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].Gr_index=(float)MyWBInf[GlobalTime2].Gr_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].Gb_index=(float)MyWBInf[GlobalTime2].Gb_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].B_index=(float)MyWBInf[GlobalTime2].B_index * 1.0/GlobalTime2+0.5;
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
			temp.Format(_T("%d %d %d %d %d %d %d %d %d %d"),
						i,
						MyWBInf[i-1].RG,
						MyWBInf[i-1].BG,
						MyWBInf[i-1].GG,
						MyWBInf[i-1].OTP_RG,
						MyWBInf[i-1].OTP_BG,
						MyWBInf[i-1].OTP_GG,
						MyWBInf[i-1].RG_index,
						MyWBInf[i-1].BG_index,
						MyWBInf[i-1].GG_index
						);
			if (GlobalTime2==pDlg->m_ModuleNum)
			{
				MyWBInf[GlobalTime2].RG_index+=MyWBInf[i-1].RG_index;
				MyWBInf[GlobalTime2].BG_index+=MyWBInf[i-1].BG_index;
				MyWBInf[GlobalTime2].GG_index+=MyWBInf[i-1].GG_index;
				if (i==1)
				{
					MyWBInf[GlobalTime2].RG_index=(float)MyWBInf[GlobalTime2].RG_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].BG_index=(float)MyWBInf[GlobalTime2].BG_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].GG_index=(float)MyWBInf[GlobalTime2].GG_index * 1.0/GlobalTime2+0.5;
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
					MyWBInf[GlobalTime2].R_index=(float)MyWBInf[GlobalTime2].R_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].Gr_index=(float)MyWBInf[GlobalTime2].Gr_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].Gb_index=(float)MyWBInf[GlobalTime2].Gb_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].B_index=(float)MyWBInf[GlobalTime2].B_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].RG_index=(float)MyWBInf[GlobalTime2].RG_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].BG_index=(float)MyWBInf[GlobalTime2].BG_index * 1.0/GlobalTime2+0.5;
					MyWBInf[GlobalTime2].GG_index=(float)MyWBInf[GlobalTime2].GG_index * 1.0/GlobalTime2+0.5;
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
			//"序号 R Gr Gb B OTP_R OTP_Gr OTP_Gb OTP_B R_index Gr_index Gb_index B_index RG BG GG OTP_RG OTP_BG OTP_GG RG_index BG_index GG_index"
			temp.Format(_T("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d"),
						i,
						MyWBInf[i-1].R,
						MyWBInf[i-1].Gr,
						MyWBInf[i-1].Gb,
						MyWBInf[i-1].B,
						MyWBInf[i-1].OTP_R,
						MyWBInf[i-1].OTP_Gr,
						MyWBInf[i-1].OTP_Gb,
						MyWBInf[i-1].OTP_B,
						MyWBInf[i-1].R_index,
						MyWBInf[i-1].Gr_index,
						MyWBInf[i-1].Gb_index,
						MyWBInf[i-1].B_index,
						MyWBInf[i-1].RG,
						MyWBInf[i-1].BG,
						MyWBInf[i-1].GG,
						MyWBInf[i-1].OTP_RG,
						MyWBInf[i-1].OTP_BG,
						MyWBInf[i-1].OTP_GG,
						MyWBInf[i-1].RG_index,
						MyWBInf[i-1].BG_index,
						MyWBInf[i-1].GG_index
						);
		}
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	int WBResult=0;
	if (pDlg->SingleChannelEnable==1&&pDlg->GainEnable==0)
	{
		if (GlobalTime2==pDlg->m_ModuleNum)
		{
			temp.Format(_T("Aevr -- -- -- -- -- -- -- -- %d %d %d %d"),MyWBInf[GlobalTime2].R_index,MyWBInf[GlobalTime2].Gr_index,MyWBInf[GlobalTime2].Gb_index,MyWBInf[GlobalTime2].B_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
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
		temp.Format(_T("序号 RG BG GG OTP_RG OTP_BG OTP_GG RG_index BG_index GG_index"));
	}
	else
	{
		if (GlobalTime2==pDlg->m_ModuleNum)
		{
			temp.Format(_T("Aevr -- -- -- -- -- -- -- -- %d %d %d %d -- -- -- -- -- -- %d %d %d"),MyWBInf[GlobalTime2].R_index,MyWBInf[GlobalTime2].Gr_index,MyWBInf[GlobalTime2].Gb_index,MyWBInf[GlobalTime2].B_index,MyWBInf[GlobalTime2].RG_index,MyWBInf[GlobalTime2].BG_index,MyWBInf[GlobalTime2].GG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
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
	m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);

	///导入规格//
	if (GlobalTime2 == pDlg->m_ModuleNum)
	{
		if(WBResult==1||WBResult==2)
		{
			//GlobalTime--;
			CString templog=_T("index平均值不在范围内!");
			if(WBResult==2) templog=_T("index error不在范围内!");
			m_pInterface->Ctrl_Addlog(CamID,templog,COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,templog);
			SetResult(RESULT_FAIL);
			memset(&MyWBInf[GlobalTime2],0,sizeof(WBInf));
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
		WriteConfigInt(_T("CalTimeTag"), iTimeTag);//此处把当前时间戳写入ini文件，PreProcess测试项里面会读取这个时间戳，所以GetDNPIndex_Moto必须在PreProcess之前--Added by LiHai--20171221
		CString suffix=_T("");
		if (pDlg->m_count!=0)
		{
			suffix.Format(_T("_%d"),pDlg->m_count);
		}

		if (pDlg->SingleChannelEnable)
		{
			WriteConfigInt(_T("R_index")+suffix,MyWBInf[GlobalTime2].R_index);
			WriteConfigInt(_T("Gr_index")+suffix,MyWBInf[GlobalTime2].Gr_index);
			WriteConfigInt(_T("Gb_index")+suffix,MyWBInf[GlobalTime2].Gb_index);
			WriteConfigInt(_T("B_index")+suffix,MyWBInf[GlobalTime2].B_index);
		}

		if (pDlg->GainEnable)
		{
			WriteConfigInt(_T("RG_index")+suffix,MyWBInf[GlobalTime2].RG_index);
			WriteConfigInt(_T("BG_index")+suffix,MyWBInf[GlobalTime2].BG_index);
			WriteConfigInt(_T("GG_index")+suffix,MyWBInf[GlobalTime2].GG_index);
		}

		//LightSource
		WriteConfigInt(_T("Ev_5000k"), m_HighCCT_Ev);
		WriteConfigInt(_T("u_5000k"), m_HighCCT_u);
		WriteConfigInt(_T("v_5000k"), m_HighCCT_v);

		SetName(CurrentName);
		if (NowColorNum == pDlg->m_ColorNum)
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("光源校正完毕，开始测试"));
			GlobalTime = 12;//点检完成后设置GlobalTime,下一个模组直接测试不点检 20180517
		}
		else if (NowColorNum < pDlg->m_ColorNum)
		{
			AFXmsg = pDlg->m_ColorName + _T("色温校正完毕，请换下一种色温模组");
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,AFXmsg);
		}
		SetResult(RESULT_NORMAL);
		memset(&MyWBInf[GlobalTime2], 0, sizeof(WBInf));
		SetAutoLightStatus(AUTOLIGHT_SAVECOEFFEE_SUCESS);  //导入成功
	}
	else
	{
		temp.Format(_T("第%d颗模组校正完成"),GlobalTime2);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,temp);
		SetResult(RESULT_NORMAL);
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

int GetDNPIndex_Moto_TestItem::MotoDynamicLightSrcCalibration(void)
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
 	SetName(GlobalSetting);	
 	GlobalTime = ReadConfigInt(_T("GlobalTime"), 3);
 	int CurrentTagTime = ReadConfigInt(_T("CurrentTimeTag"), 2);
 	SetName(CurrentName);

	int NowColorNum = 0;	//色温数[单色温、两色温、三色温]
	if (pDlg->m_count == 0)
	{
		NowColorNum = 1;
	}
	if (pDlg->m_count == 3)
	{
		NowColorNum = 2;
	}
	if (pDlg->m_count == 6)
	{
		NowColorNum = 3;
	}
	if (pDlg->m_count == 9)
	{
		NowColorNum = 4;
	}

	/**
	*	点击主窗口菜单[光源系数校准]->[打开光源校准]，会在ini里面赋值"GlobalTime"为[0]，从而让
	*	此处能够运行下去.
	*	--Added by LiHai--20171221
	**/
	CString AFXmsg;
	//if ((GlobalTime >= (pDlg->m_count + pDlg->m_ModuleNum)) || (GlobalTime < pDlg->m_count))//3修改成pDlg->m_ModuleNum
	if (GlobalTime > (pDlg->m_ModuleNum*pDlg->m_ColorNum))
	{
		GlobalTime2 = 0; //下次光源校正时从新计数，不然数组会越界
		return 0;
	}

	/** 光源点检防呆 **/
	if (FALSE == GetLightSource())
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("GetLightSource() Fail\r\n"), COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("GetLightSource() Fail\r\n"));
		SetResult(RESULT_FAIL);
		SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
		return 0;
	}

	//if (SetLEDFlag)
	{
		if (GlobalTime == pDlg->m_count)
		{
			AFXmsg = _T("请放入") + pDlg->m_ColorName + _T("色温模组进行校正!");
			AfxMessageBox(AFXmsg);
			GlobalTime2 = 0;//初始化
		}
	}
	
	int iTimeTag = GetTimeTag();
	if (GlobalTime2 != 0)//猜测:是否是说某次光源点检持续的时间超过两个小时，就重新复位点检模组数，需要重新开始点检--比如总共需要点检5颗模组，而此时先点检了三颗，然后有事就耽误了两个小时，最后接着点检，这时就需要重新开始点检--Added by LiHai--20171221
	{
		if (abs(iTimeTag - CurrentTagTime) > 1)//时间过长不能矫正
		{
			GlobalTime = 0;
			GlobalTime2 = 0;    //每隔一时重新校准
		}
	}

	CString Caltime;
	Caltime.Format(_T("第%d颗模组开始校正.."), GlobalTime2 + 1);
	m_pInterface->Ctrl_Addlog(CamID, Caltime, COLOR_BLUE, 200);
	CString CurrentFuseID = m_pInterface->Sensor_GetCamFuseID(CamID);
	m_pInterface->Ctrl_Addlog(CamID, CurrentFuseID, COLOR_BLUE, 200);
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
	
	for (i = 0; i < GlobalTime2; i++)//已经校准了"GlobalTime2"颗Golden模组--Added by LiHai--20171221
	{
		if (CurrentFuseID == MyfuseID[i])//判断此模组是否已经点检过，防止一颗模组多次点检--Added by LiHai--20171221
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("该模组已注册!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("该模组已注册!"));
 			SetResult(RESULT_FAIL);
			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
 			return 0;
		}
	}

	//GetOTPData(MyWBInf[GlobalTime2]);//动态点检不需要模组内烧录的数据 LYC 20180515
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
		if (!GetAWBData(MyImageInfo[0], MyWBInf[GlobalTime2]))
		{
			m_pInterface->Ctrl_Addlog(CamID, _T("获取AWB失败!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("获取AWB失败!"));
			SetResult(RESULT_FAIL);
			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			return 0;
		}
	}
	else
	{
	   CenterBlockRawInfo(MyImageInfo, pDlg->m_ROI_H, MyWBInf[GlobalTime2], pDlg->m_Multiple, pDlg->m_BLC);
	}
	if (pDlg->SingleChannelEnable)
	{
		BOOL WBResult = TRUE;
		CString temp;
		temp.Format(_T("当前影像 R=%d <%d,%d>"),MyWBInf[GlobalTime2].R,pDlg->R_min,pDlg->R_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gr=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr,pDlg->Gr_min,pDlg->Gr_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gb=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb,pDlg->Gb_min,pDlg->Gb_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 B=%d <%d,%d>"),MyWBInf[GlobalTime2].B,pDlg->B_min,pDlg->B_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
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
		MyWBInf[GlobalTime2].R_index=(MyWBInf[GlobalTime2].OTP_R*10000.0/(float)MyWBInf[GlobalTime2].R+0.5);
		MyWBInf[GlobalTime2].Gr_index=(MyWBInf[GlobalTime2].OTP_Gr*10000.0/(float)MyWBInf[GlobalTime2].Gr+0.5);
		MyWBInf[GlobalTime2].Gb_index=(MyWBInf[GlobalTime2].OTP_Gb*10000.0/(float)MyWBInf[GlobalTime2].Gb+0.5);
		MyWBInf[GlobalTime2].B_index=(MyWBInf[GlobalTime2].OTP_B*10000.0/(float)MyWBInf[GlobalTime2].B+0.5);
		MyWBInf[GlobalTime2].R_index=(MyWBInf[GlobalTime2].R_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].Gb_index=(MyWBInf[GlobalTime2].Gb_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].B_index=(MyWBInf[GlobalTime2].B_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		MyWBInf[GlobalTime2].Gr_index=(MyWBInf[GlobalTime2].Gr_index*10000.0/(float)MyWBInf[GlobalTime2].Gr_index+0.5);
		temp.Format(_T("当前影像 R_index=%d <%d,%d>"),MyWBInf[GlobalTime2].R_index,pDlg->R_index_min,pDlg->R_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gr_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr_index,pDlg->Gr_index_min,pDlg->Gr_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gb_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb_index,pDlg->Gb_index_min,pDlg->Gb_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 B_index=%d <%d,%d>"),MyWBInf[GlobalTime2].B_index,pDlg->B_index_min,pDlg->B_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
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

		temp.Format(_T("当前影像 R=%d <%d,%d>"),MyWBInf[GlobalTime2].R,pDlg->R_min,pDlg->R_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gr=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr,pDlg->Gr_min,pDlg->Gr_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 Gb=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb,pDlg->Gb_min,pDlg->Gb_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 B=%d <%d,%d>"),MyWBInf[GlobalTime2].B,pDlg->B_min,pDlg->B_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
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

		/*temp.Format(_T("当前影像 R_index=%d <%d,%d>"),MyWBInf[GlobalTime2].R_index,pDlg->R_index_min,pDlg->R_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 Gr_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gr_index,pDlg->Gr_index_min,pDlg->Gr_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 Gb_index=%d <%d,%d>"),MyWBInf[GlobalTime2].Gb_index,pDlg->Gb_index_min,pDlg->Gb_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前影像 B_index=%d <%d,%d>"),MyWBInf[GlobalTime2].B_index,pDlg->B_index_min,pDlg->B_index_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);*/
		temp.Format(_T("当前影像 RG=%d <%d,%d>"),MyWBInf[GlobalTime2].RG,pDlg->RG_min,pDlg->RG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 BG=%d <%d,%d>"),MyWBInf[GlobalTime2].BG,pDlg->BG_min,pDlg->BG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前影像 GG=%d <%d,%d>"),MyWBInf[GlobalTime2].GG,pDlg->GG_min,pDlg->GG_max);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		if ((MyWBInf[GlobalTime2].R > 1023)
			|| (MyWBInf[GlobalTime2].Gr > 1023)
			|| (MyWBInf[GlobalTime2].Gb > 1023)
			|| (MyWBInf[GlobalTime2].B > 1023)
			)
		{
			SetResult(RESULT_FAIL);
			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			m_pInterface->Ctrl_Addlog(CamID, _T("R或Gr或Gb或B值超出范围,请重新测试"), COLOR_RED, 200);
			return 0;
		}
		m_CurVal.m_R = MyWBInf[GlobalTime2].R;
		m_CurVal.m_Gr = MyWBInf[GlobalTime2].Gr;
		m_CurVal.m_Gb = MyWBInf[GlobalTime2].Gb;
		m_CurVal.m_B = MyWBInf[GlobalTime2].B;
		m_CurVal.m_RGain = MyWBInf[GlobalTime2].RG;
		m_CurVal.m_BGain = MyWBInf[GlobalTime2].BG;
		m_CurVal.m_GGain = MyWBInf[GlobalTime2].GG;

		temp.Format(_T("Typical_R = %d"), pDlg->m_TypVal.m_R);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);
		temp.Format(_T("Typical_Gr = %d"), pDlg->m_TypVal.m_Gr);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);
		temp.Format(_T("Typical_Gb = %d"), pDlg->m_TypVal.m_Gb);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);
		temp.Format(_T("Typical_B = %d"), pDlg->m_TypVal.m_B);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);
		temp.Format(_T("Typical_RG = %d"), pDlg->m_TypVal.m_RGain);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);
		temp.Format(_T("Typical_BG = %d"), pDlg->m_TypVal.m_BGain);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);
		temp.Format(_T("Typical_GG = %d"), pDlg->m_TypVal.m_GGain);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);

		/** 计算公差 **/
		float ErrorValue = sqrt(pow((float)(m_CurVal.m_RGain - pDlg->m_TypVal.m_RGain), 2) + pow((float)(m_CurVal.m_BGain - pDlg->m_TypVal.m_BGain), 2));
		if (ErrorValue > pDlg->m_CurrentDiff)
		{
			SetResult(RESULT_FAIL);
			SetAutoLightStatus(AUTOLIGHT_GOLDEN_FAIL);  //获取GOLDEDN值失败
			temp.Format(_T("Error:%.4f > Spec:%.4f  该模组Error超范围!"), ErrorValue, pDlg->m_CurrentDiff);
			m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
			return 0;
		}
		temp.Format(_T("Error:%.4f > Spec:%.4f"), ErrorValue, pDlg->m_CurrentDiff);
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);

		m_CurSum.m_R  += m_CurVal.m_R;
		m_CurSum.m_Gr += m_CurVal.m_Gr;
		m_CurSum.m_Gb += m_CurVal.m_Gb;
		m_CurSum.m_B  += m_CurVal.m_B;

		m_CurSum.m_RGain += m_CurVal.m_RGain;
		m_CurSum.m_BGain += m_CurVal.m_BGain;
		m_CurSum.m_GGain += m_CurVal.m_GGain;

		CTime time = CTime::GetCurrentTime();
		DnpInfo[GlobalTime2].FuseId = CurrentFuseID;
		DnpInfo[GlobalTime2].month = time.GetMonth();
		DnpInfo[GlobalTime2].day = time.GetDay();
		DnpInfo[GlobalTime2].hour = time.GetHour();
		DnpInfo[GlobalTime2].min = time.GetMinute();
		DnpInfo[GlobalTime2].sec = time.GetSecond();

		DnpInfo[GlobalTime2].R_ave = m_CurVal.m_R;
		DnpInfo[GlobalTime2].Gr_ave = m_CurVal.m_Gr;
		DnpInfo[GlobalTime2].Gb_ave = m_CurVal.m_Gb;
		DnpInfo[GlobalTime2].B_ave = m_CurVal.m_B;
		DnpInfo[GlobalTime2].RG = m_CurVal.m_RGain;
		DnpInfo[GlobalTime2].BG = m_CurVal.m_BGain;
		DnpInfo[GlobalTime2].GG = m_CurVal.m_GGain;
		DnpInfo[GlobalTime2].Error = ErrorValue;
	}
	GlobalTime++;
	GlobalTime2++;
	CString temp;

	///导入规格//
	if (GlobalTime2 == pDlg->m_ModuleNum)
	{
		m_CurVal.m_R  = int(m_CurSum.m_R * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_Gr = int(m_CurSum.m_Gr * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_Gb = int(m_CurSum.m_Gb * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_B  = int(m_CurSum.m_B * 1.0 / GlobalTime2 + 0.5);

		/*
		*	接刘月琴通知，客户要求以下三个GAIN值的计算公式由一开始的各自的平均值改为用通道的平均值计算。
		*	--Added by LiHai--20171110
		*/
	#if 0
		m_CurVal.m_RGain = int(m_CurSum.m_RGain * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_BGain = int(m_CurSum.m_BGain * 1.0 / GlobalTime2 + 0.5);
		m_CurVal.m_GGain = int(m_CurSum.m_GGain * 1.0 / GlobalTime2 + 0.5);
	#else
		m_CurVal.m_RGain = int( (float)pDlg->m_Multiple*(m_CurVal.m_R - pDlg->m_BLC)/((float)(m_CurVal.m_Gr + m_CurVal.m_Gb - 2*pDlg->m_BLC)/2.0) + 0.5 );
		m_CurVal.m_BGain = int( (float)pDlg->m_Multiple*(m_CurVal.m_B - pDlg->m_BLC)/((float)(m_CurVal.m_Gr + m_CurVal.m_Gb - 2*pDlg->m_BLC)/2.0) + 0.5 );//int((m_CurVal.m_B - pDlg->m_BLC) * pDlg->m_Multiple / (m_CurVal.m_Gr + m_CurVal.m_Gb - 2 * pDlg->m_BLC) * 2 + 0.5);
		m_CurVal.m_GGain = int( (float)pDlg->m_Multiple*(m_CurVal.m_Gr)/(float)(m_CurVal.m_Gb)+0.5);//int(m_CurVal.m_Gr * pDlg->m_Multiple / m_CurVal.m_Gb + 0.5);
	#endif

		/*
		*	接刘月琴通知，客户要求以下三个GAIN值的计算公式由一开始的各自的平均值改为用通道的平均值计算。
		*	注意和上面的计算公式相比，此处不能四舍五入。
		*	--Added by LiHai--20171118
		*/
#if 0
		float RGNew_ave =float(m_CurSum.m_RGain * 1.0 / GlobalTime2);
		float BGNew_ave =float(m_CurSum.m_BGain * 1.0 / GlobalTime2);
#else
		float RGNew_ave = ((float)(m_CurVal.m_R - pDlg->m_BLC) * pDlg->m_Multiple / (float)(m_CurVal.m_Gr + m_CurVal.m_Gb - 2 * pDlg->m_BLC) * 2);
		float BGNew_ave = ((float)(m_CurVal.m_B - pDlg->m_BLC) * pDlg->m_Multiple / (float)(m_CurVal.m_Gr + m_CurVal.m_Gb - 2 * pDlg->m_BLC) * 2);
#endif
		float Error_ave = sqrt(pow((float)(RGNew_ave - pDlg->m_TypVal.m_RGain), 2) + pow((float)(BGNew_ave - pDlg->m_TypVal.m_BGain), 2));
		/**********************************************************************/
		/*                         动态点检算法   : add by Gary               */
		/**********************************************************************/	
		if (Error_ave < pDlg->m_GoldenDiff)
		{ 
			m_CurVal  = pDlg->m_TypVal;
			temp.Format(_T("Error_ave:%f < Spec:%f，使用Typical数据为Golden数据"), Error_ave, pDlg->m_GoldenDiff);
		}
		else
		{
			temp.Format(_T("Error_ave:%f > Spec:%f，使用Current AWB数据为Golden数据"), Error_ave, pDlg->m_GoldenDiff);
		}
		m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);

		SaveMotoDynamic_DNPData();

		//RegSetKey(_T("LightSource"), _T(""), _T(""));    //将LightSource数据清空
		//RegSetKey(_T("LightSource"), _T("5100k_Ev") ,_T(""));
		//RegSetKey(_T("LightSource"), _T("5100k_u"), _T(""));
		//RegSetKey(_T("LightSource"), _T("5100k_v"), _T(""));

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
		WriteConfigInt(_T("CalTimeTag"), iTimeTag);//此处把当前时间戳写入ini文件，PreProcess测试项里面会读取这个时间戳，所以GetDNPIndex_Moto必须在PreProcess之前--Added by LiHai--20171221
		CString suffix=_T("");
		if (pDlg->m_count!=0)
		{
			suffix.Format(_T("_%d"),pDlg->m_count);
		}

		SetName(CurrentName);
		if (NowColorNum == pDlg->m_ColorNum)
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("光源校正完毕，开始测试"));
			GlobalTime = 12;//点检完成后设置GlobalTime,下一个模组直接测试不点检 20180517
		}
		else if (NowColorNum < pDlg->m_ColorNum)
		{
			AFXmsg = pDlg->m_ColorName + _T("色温校正完毕，请换下一种色温模组");
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,AFXmsg);
		}
		
		SetResult(RESULT_NORMAL);
		memset(&MyWBInf[GlobalTime2], 0, sizeof(WBInf));
		SetAutoLightStatus(AUTOLIGHT_SAVECOEFFEE_SUCESS);  //导入成功
	}
	else
	{
		temp.Format(_T("第%d颗模组校正完成"),GlobalTime2);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,temp);
		SetResult(RESULT_NORMAL);
		SetAutoLightStatus(AUTOLIGHT_GOLDEN_SUCESS);  //获取GOLDEDN值成功
	}
	SetName(GlobalSetting);
	WriteConfigInt(_T("CurrentTimeTag"),iTimeTag);
	WriteConfigInt(_T("GlobalTime"),GlobalTime);
	SetName(CurrentName);
	m_pInterface->UnlocktheFile(CamID);//解锁加密狗

	MyfuseID[GlobalTime2-1]=CurrentFuseID;
	//SetResult(RESULT_PASS);
	return 0;
}

void GetDNPIndex_Moto_TestItem::CenterBlockRawInfo(ImageInfo *mySensor ,int Ratio,WBInf &MyWbInf,int Multiple,int BLC)
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
	int rawWidth  = PixW / pDlg->m_ROI_H/*Ratio*/;    //multiply two is to make sure that width & height is even  because of bayer order
	int rawHeight = PixH / pDlg->m_ROI_V/*Ratio*/;

	//联想手机曝光和计算AWB数据都是取的【中间128*128像素区域】来计算的--Added by LiHai--20171213
	if ((pDlg->m_ROI_H > 100) && (pDlg->m_ROI_V > 100))
	{
		rawWidth = pDlg->m_ROI_H;
		rawHeight = pDlg->m_ROI_V;
	}

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

	float AA = 0.0;
	if (pDlg->m_flow)
	{
		AA = 0.5;
	}
	CString str;
	str.Format(_T("m_MotoLightSrcMode = %d,GAverFlag = %d"), pDlg->m_MotoLightSrcMode,GAverFlag);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

	if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == pDlg->m_MotoLightSrcMode)//动态点检--单通道不减BLC，但是Gain的计算需要先减BLC然后按16384的公式计算--Added by LiHai--20180507
	{
		MyWbInf.Gr = int(Gr + AA);//-BLC;
		MyWbInf.Gb = int(Gb + AA);//-BLC;
		MyWbInf.R  = int(R + AA);//-BLC;
		MyWbInf.B  = int(B + AA);//-BLC;
		int RawG   = int((Gr + Gb/* - 2*BLC*/) / 2.0 + AA);
		
		if(GAverFlag==1)
		{
			MyWbInf.RG = int( (float)Multiple*(MyWbInf.R - BLC)/((float)(MyWbInf.Gr + MyWbInf.Gb - 2*BLC)/2.0) + AA  );// Rave/Gave
			MyWbInf.BG = int( (float)Multiple*(MyWbInf.B - BLC)/((float)(MyWbInf.Gr + MyWbInf.Gb - 2*BLC)/2.0) + AA );  //Bave/Gave 
			MyWbInf.GG =int( (float)Multiple*(MyWbInf.Gr)/(float)(MyWbInf.Gb)+AA);//LYC 修改计算GGain值Gr/Gb 20180511

			//MyWbInf.RG = int( (float)Multiple*(MyWbInf.R - BLC)/(float)(RawG - BLC) + AA  );// Rave/Gave
			//MyWbInf.BG = int( (float)Multiple*(MyWbInf.B - BLC)/(float)(RawG - BLC) + AA );  //Bave/Gave 
			//MyWbInf.GG = int( (float)Multiple*(MyWbInf.Gb - BLC)/(float)(MyWbInf.Gr - BLC)+AA);
		}
		else if (GAverFlag==2)
		{
			MyWbInf.RG = int( (float)Multiple*(MyWbInf.R - BLC)/(float)(MyWbInf.Gr - BLC) + AA );// Rave/Gave
			MyWbInf.BG = int( (float)Multiple*(MyWbInf.B - BLC)/(float)(MyWbInf.Gr - BLC) + AA );  //Bave/Gave 
			MyWbInf.GG = int( (float)Multiple*(MyWbInf.Gb - BLC)/(float)(MyWbInf.Gr - BLC) + AA);
		}
		else if (GAverFlag==3)
		{
			MyWbInf.RG = int( (float)Multiple*(MyWbInf.R - BLC)/(float)(MyWbInf.Gb - BLC) + AA );// Rave/Gave
			MyWbInf.BG = int( (float)Multiple*(MyWbInf.B - BLC)/(float)(MyWbInf.Gb - BLC) + AA );  //Bave/Gave 
			MyWbInf.GG = int( (float)Multiple*(MyWbInf.Gr - BLC)/(float)(MyWbInf.Gb - BLC) + AA);
		}
	}
	else	/** 联想静态点检的单通道数据需要减BLC，Gain也是需要先减BLC然后按16384的公式计算--Added by LiHai--20180104 **/
	{
		MyWbInf.Gr = int(Gr+AA)-BLC;//added 四舍五入20180515
		MyWbInf.Gb = int(Gb+AA)-BLC;
		MyWbInf.R  = int(R+AA)-BLC;
		MyWbInf.B  = int(B+AA)-BLC;
		int RawG   = int((Gr + Gb - 2 * BLC) / 2.0 + 0.5);

		if(GAverFlag==1)
		{
			MyWbInf.RG = int( (float)Multiple*(MyWbInf.R)/((float)(MyWbInf.Gr + MyWbInf.Gb)/2.0) + AA );// Rave/Gave
			MyWbInf.BG = int( (float)Multiple*(MyWbInf.B)/((float)(MyWbInf.Gr + MyWbInf.Gb)/2.0) + AA );  //Bave/Gave 
			MyWbInf.GG = int( (float)Multiple*(MyWbInf.Gr)/(float)(MyWbInf.Gb)+AA);//LYC 修改计算GGain值Gr/Gb 20180511
			//MyWbInf.GG = int( (float)Multiple*(MyWbInf.Gb)/(float)(MyWbInf.Gr)+AA);
		}
		else if (GAverFlag==2)
		{
			MyWbInf.RG = int( (float)Multiple*(MyWbInf.R)/(float)(MyWbInf.Gr) + AA );// Rave/Gave
			MyWbInf.BG = int( (float)Multiple*(MyWbInf.B)/(float)(MyWbInf.Gr) + AA );  //Bave/Gave 
			MyWbInf.GG = int( (float)Multiple*(MyWbInf.Gb)/(float)(MyWbInf.Gr)+ AA);
		}
		else if (GAverFlag==3)
		{
			MyWbInf.RG = int( (float)Multiple*(MyWbInf.R)/(float)(MyWbInf.Gb) + AA );// Rave/Gave
			MyWbInf.BG = int( (float)Multiple*(MyWbInf.B)/(float)(MyWbInf.Gb) + AA );  //Bave/Gave 
			MyWbInf.GG = int( (float)Multiple*(MyWbInf.Gr)/(float)(MyWbInf.Gb)+ AA);
		}
	}
}


/******************************************************************
º¯ÊýÃû:    Finalize
º¯Êý¹¦ÄÜ:  ÓÃÓÚInitializeÉêÃ÷µÄ±äÁ¿»Ö¸´Ô­Ê¼Öµ£¬ÉêÇëµÄÄÚ´æ¿Õ¼äÊÍ·ÅµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int GetDNPIndex_Moto_TestItem::Finalize()
{
	//TODO:ÔÚ´ËÌí¼Ó»ØÊÕ½áÊø´úÂë


	return 0;
}


/******************************************************************
º¯ÊýÃû:    LoadOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡²ÎÊýµ½±äÁ¿£¬ºÍ¸³Öµµ½¶Ô»°¿òµÄ°ó¶¨±äÁ¿ (ÎÄ¼þ´æ´¢Öµ->±äÁ¿¡ª>¿Ø¼þµÄÖµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int GetDNPIndex_Moto_TestItem::LoadOption()
{
	pDlg->m_count=ReadConfigInt(_T("count"),0);
	CString suffix=_T("");
	if (pDlg->m_count!=0)
	{
		suffix.Format(_T("_%d"),pDlg->m_count);
	}
	SetName(GlobalSetting);
	//TODO: ÔÚ´ËÌí¼Ó¶ÁÈ¡´úÂë 
	pDlg->m_CalDiff=ReadConfigInt(_T("CalDiff"),12);
	pDlg->m_QTITool=ReadConfigInt(_T("QTITool"),1);
	pDlg->m_flow=ReadConfigInt(_T("flow"),1);
	pDlg->m_DNPDistance = ReadConfigInt(_T("DNPDistance")+suffix,100);
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
		pDlg->FuseID[i] = ReadConfigString(temp, _T("0000000000"));
	}
	pDlg->m_SlaveID= ReadConfigString(_T("SlaveID"), _T("0x42"));
	pDlg->R_StartAdd= ReadConfigString(_T("R_StartAdd")+suffix, _T("0x0000"));
	pDlg->Gr_StartAdd= ReadConfigString(_T("Gr_StartAdd")+suffix, _T("0x0000"));
	pDlg->Gb_StartAdd= ReadConfigString(_T("Gb_StartAdd")+suffix, _T("0x0000"));
	pDlg->B_StartAdd= ReadConfigString(_T("B_StartAdd")+suffix, _T("0x0000"));
	pDlg->RG_StartAdd= ReadConfigString(_T("RG_StartAdd")+suffix, _T("0x002F"));
	pDlg->BG_StartAdd= ReadConfigString(_T("BG_StartAdd")+suffix, _T("0x0031"));
	pDlg->GG_StartAdd= ReadConfigString(_T("GG_StartAdd")+suffix, _T("0x0033"));
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
	pDlg->Station=ReadConfigString(_T("TestStation"), _T("OHP0792"));
	pDlg->SingleChannelEnable=ReadConfigInt(_T("SingleChannelEnable"),0);
	pDlg->GainEnable=ReadConfigInt(_T("GainEnable"),0);
	pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
	pDlg->m_Multiple=ReadConfigInt(_T("Multiple"),512);
	pDlg->m_ROI_H=ReadConfigInt(_T("ROI_H"),5);
	pDlg->m_ROI_V=ReadConfigInt(_T("ROI_V"),5);
	m_SlaveID= ReadConfigInt(_T("SlaveID"), 0x42);
	R_StartAdd=ReadConfigInt(_T("R_StartAdd")+suffix, 0x0000);
	Gr_StartAdd=ReadConfigInt(_T("Gr_StartAdd")+suffix, 0x0000);
	Gb_StartAdd=ReadConfigInt(_T("Gb_StartAdd")+suffix, 0x0000);
	B_StartAdd=ReadConfigInt(_T("B_StartAdd")+suffix, 0x0000);
	RG_StartAdd=ReadConfigInt(_T("RG_StartAdd")+suffix, 0x002F);
	BG_StartAdd=ReadConfigInt(_T("BG_StartAdd")+suffix, 0x0031);
	GG_StartAdd=ReadConfigInt(_T("GG_StartAdd")+suffix, 0x0033);
	ByteCount=ReadConfigInt(_T("ByteCount"), 1);
	A_Multi=ReadConfigInt(_T("A_Multi"), 1);
    B_Multi=ReadConfigInt(_T("B_Multi"), 0);
	pDlg->m_Raw8_EN  = ReadConfigInt(_T("Raw8_EN"),1);
	pDlg->page=ReadConfigInt(_T("page")+suffix,1);

	pDlg->m_ColorNum  = ReadConfigInt(_T("m_ColorNum"),1);
	pDlg->m_ModuleNum  = ReadConfigInt(_T("m_ModuleNum"),3);

	/** Moto动态光源点检数据获取--在ApplyAWB.dll中也要使用，所以需要放在WBSetting主键下面--Added by LiHai--20180505 **/
	pDlg->m_MotoLightSrcMode = ReadConfigInt(_T("MotoLightSrcMode"), 0);
	if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == pDlg->m_MotoLightSrcMode)//动态点检
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO4))->SetCheck(1);
		pDlg->MotoLightSrcCfg(TRUE);

		/**
		*	Moto动态光源点检，Golden值是可能变动的，所以卡控范围也是动态的，范围为当前Golden值加减0.05%。
		*	只要RGain、BGain，GGain由于Gr和Gb相差不大基本相等，所以卡控范围不用动态计算。
		*	--Added by LiHai--20180505
		**/
		/** Step 1:读取当前Golden值 **/
		int tmpGoldenRG  = ReadConfigInt(_T("Golden_RG") + suffix, 200);//ReadConfigInt(_T("Typical_RG")+suffix,200);
		int tmpGoldenBG  = ReadConfigInt(_T("Golden_BG") + suffix, 200);//ReadConfigInt(_T("Typical_BG")+suffix,200);
		//pDlg->m_GoldenGG  = ReadConfigInt(_T("Golden_GG")+suffix,200);//ReadConfigInt(_T("Typical_GG")+suffix,200);

		/** Step 2:读取当前上下浮动比例 **/
		CString TempStr = ReadConfigString(_T("RGainHigh_diff"),_T("0.0"));
		float tmpRGainHigh_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("RGainlow_diff"),_T("0.0"));
		float tmpRGainlow_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("BGainHigh_diff"),_T("0.0"));
		float tmpBGainHigh_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("BGainlow_diff"),_T("0.0"));
		float tmpBGainLow_diff = atof(CT2A(TempStr));

		/** Step 3:计算上下浮动范围 **/
		pDlg->RG_min = (int)(tmpGoldenRG + tmpRGainlow_diff * 16384.0 + 0.5);	//注意本身有负号，所以用【加】
		pDlg->RG_max = (int)(tmpGoldenRG + tmpRGainHigh_diff * 16384.0 + 0.5);
		pDlg->BG_min = (int)(tmpGoldenBG + tmpBGainLow_diff * 16384.0 + 0.5);   //注意本身有负号，所以用【加】
		pDlg->BG_max = (int)(tmpGoldenBG + tmpBGainHigh_diff * 16384.0 + 0.5);
		
		//pDlg->RG_min = tmpGoldenRG * (1 + tmpRGainlow_diff);	//注意本身有负号，所以用【加】
		//pDlg->RG_max = tmpGoldenRG * (1 + tmpRGainHigh_diff);
		//pDlg->BG_min = tmpGoldenBG * (1 + tmpBGainLow_diff);	//注意本身有负号，所以用【加】
		//pDlg->BG_max = tmpGoldenBG * (1 + tmpBGainHigh_diff);
	}
	else if (MOTO_LIGHT_SRC_CAL_MODE_STATIC == pDlg->m_MotoLightSrcMode)//静态点检
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO5))->SetCheck(1);
		pDlg->MotoLightSrcCfg(FALSE);
	}
	
	//切换光源
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(CurrentName);
	pDlg->m_ColorName=ReadConfigString(_T("m_ColorName"), 0);
	
	/** Moto动态光源点检数据获取 **/
	pDlg->m_TypVal.m_R = ReadConfigInt(_T("Typical_R"), 128);
	pDlg->m_TypVal.m_Gr = ReadConfigInt(_T("Typical_Gr"), 128);
	pDlg->m_TypVal.m_Gb = ReadConfigInt(_T("Typical_Gb"), 128);
	pDlg->m_TypVal.m_B = ReadConfigInt(_T("Typical_B"), 128);
	pDlg->m_TypVal.m_RGain = ReadConfigInt(_T("Typical_RG"), 128);
	pDlg->m_TypVal.m_BGain = ReadConfigInt(_T("Typical_BG"), 128);
	pDlg->m_TypVal.m_GGain = ReadConfigInt(_T("Typical_GG"), 128);

	CString str;
	str = ReadConfigString(_T("GoldenDiff"), _T("5.0"));
	pDlg->m_GoldenDiff = _tstof(str);
	str = ReadConfigString(_T("CurrentDiff"), _T("5.0"));
	pDlg->m_CurrentDiff = _tstof(str);

	pDlg->m_Ev_HighCCT_Min = ReadConfigInt(_T("Ev_HighCCT_Min"), 128);
	pDlg->m_Ev_HighCCT_Max = ReadConfigInt(_T("Ev_HighCCT_Max"), 128);
	pDlg->m_U_HighCCT_Min = ReadConfigInt(_T("U_HighCCT_Min"), 128);
	pDlg->m_U_HighCCT_Max = ReadConfigInt(_T("U_HighCCT_Max"), 128);
	pDlg->m_V_HighCCT_Min = ReadConfigInt(_T("V_HighCCT_Min"), 128);
	pDlg->m_V_HighCCT_Max = ReadConfigInt(_T("V_HighCCT_Max"), 128);

	str = ReadConfigString(_T("Tolerance_HighCCT_Max"), _T("5.0"));
	pDlg->m_Tolerance_HighCCT_Max = _tstof(str);

	pDlg->m_U_Standard_HighCCT = ReadConfigInt(_T("U_Standard_HighCCT"), 10000);
	pDlg->m_V_Standard_HighCCT = ReadConfigInt(_T("V_Standard_HighCCT"), 10000);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
º¯ÊýÃû:    SaveOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ½«±äÁ¿ÊýÖµ»ò¶Ô»°¿òµÄ°ó¶¨±äÁ¿ÊýÖµ±£´æµ½ÅäÖÃÎÄ¼þÖÐ (¿Ø¼þµÄÖµ¡ª>±äÁ¿->ÎÄ¼þ´æ´¢Öµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int GetDNPIndex_Moto_TestItem::SaveOption()
{
	if (!TestItemInterface::SaveOption())
	{
		return -1;
	}

	WriteConfigInt(_T("count"), pDlg->m_count);
	
	CString suffix=_T("");
	if (pDlg->m_count != 0)
	{
		suffix.Format(_T("_%d"), pDlg->m_count);
	}
	SetName(GlobalSetting);
	//TODO: ÔÚ´ËÌí¼Ó´æ´¢´úÂë	
	WriteConfigInt(_T("CalDiff"), pDlg->m_CalDiff);
	WriteConfigInt(_T("QTITool"), pDlg->m_QTITool);
	WriteConfigInt(_T("flow"), pDlg->m_flow);
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

	/** Moto动态光源点检数据获取--在ApplyAWB.dll中也要使用，所以需要放在WBSetting主键下面--Added by LiHai--20180505 **/
	pDlg->m_MotoLightSrcMode = ReadConfigInt(_T("MotoLightSrcMode"), 0);
	if (MOTO_LIGHT_SRC_CAL_MODE_STATIC == pDlg->m_MotoLightSrcMode)//只有静态点检时才需要保存如下值，动态时不能保存，否则会把[Golde值正负5%]的值写入，这样会覆盖静态的值--Added by LiHai--20180505
	{
		WriteConfigInt(_T("RG_min")+suffix,pDlg->RG_min);
		WriteConfigInt(_T("RG_max")+suffix,pDlg->RG_max);
		WriteConfigInt(_T("BG_min")+suffix,pDlg->BG_min);
		WriteConfigInt(_T("BG_max")+suffix,pDlg->BG_max);
	}
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
	WriteConfigInt(_T("ROI_H"),pDlg->m_ROI_H);
	WriteConfigInt(_T("ROI_V"),pDlg->m_ROI_V);
	WriteConfigInt(_T("BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("Multiple"),pDlg->m_Multiple);
	WriteConfigInt(_T("Raw8_EN"), pDlg->m_Raw8_EN);
	WriteConfigInt(_T("page")+suffix, pDlg->page);

	WriteConfigInt(_T("m_ColorNum"), pDlg->m_ColorNum);
	WriteConfigInt(_T("m_ModuleNum"), pDlg->m_ModuleNum);

	/** Moto动态光源点检数据获取--在ApplyAWB.dll中也要使用，所以需要放在WBSetting主键下面--Added by LiHai--20180505 **/
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO4))->GetCheck())
	{
		pDlg->m_MotoLightSrcMode = MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC;
	}
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO5))->GetCheck())
	{
		pDlg->m_MotoLightSrcMode = MOTO_LIGHT_SRC_CAL_MODE_STATIC;
	}
	WriteConfigInt(_T("MotoLightSrcMode"), pDlg->m_MotoLightSrcMode);

	//切换光源
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));
	SetName(CurrentName);
	WriteConfigString(_T("m_ColorName"), pDlg->m_ColorName);

	WriteConfigInt(_T("Typical_R"), pDlg->m_TypVal.m_R);
	WriteConfigInt(_T("Typical_Gr"), pDlg->m_TypVal.m_Gr);
	WriteConfigInt(_T("Typical_Gb"), pDlg->m_TypVal.m_Gb);
	WriteConfigInt(_T("Typical_B"), pDlg->m_TypVal.m_B);
	WriteConfigInt(_T("Typical_RG"), pDlg->m_TypVal.m_RGain);
	WriteConfigInt(_T("Typical_BG"), pDlg->m_TypVal.m_BGain);
	WriteConfigInt(_T("Typical_GG"), pDlg->m_TypVal.m_GGain);

	CString str;
	str.Format(_T("%f"), pDlg->m_GoldenDiff);
	WriteConfigString(_T("GoldenDiff"), str.GetBuffer(str.GetLength()));

	str.Format(_T("%f"), pDlg->m_CurrentDiff);
	WriteConfigString(_T("CurrentDiff"), str.GetBuffer(str.GetLength()));

	WriteConfigInt(_T("Ev_HighCCT_Min"), pDlg->m_Ev_HighCCT_Min);
	WriteConfigInt(_T("Ev_HighCCT_Max"), pDlg->m_Ev_HighCCT_Max);
	WriteConfigInt(_T("U_HighCCT_Min"), pDlg->m_U_HighCCT_Min);
	WriteConfigInt(_T("U_HighCCT_Max"), pDlg->m_U_HighCCT_Max);
	WriteConfigInt(_T("V_HighCCT_Min"), pDlg->m_V_HighCCT_Min);
	WriteConfigInt(_T("V_HighCCT_Max"), pDlg->m_V_HighCCT_Max);

	str.Format(_T("%f"), pDlg->m_Tolerance_HighCCT_Max);
	WriteConfigString(_T("Tolerance_HighCCT_Max"), str.GetBuffer(str.GetLength()));

	WriteConfigInt(_T("U_Standard_HighCCT"), pDlg->m_U_Standard_HighCCT);
	WriteConfigInt(_T("V_Standard_HighCCT"), pDlg->m_V_Standard_HighCCT);

	LoadOption();
	//........................
	return 0;
}


/******************************************************************
º¯ÊýÃû:    GetReportHeaders
º¯Êý¹¦ÄÜ:  ±£´æÐèÒª±£´æµÄ²âÊÔÏîÄ¿Ãû×ÖÖÖÀà
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR GetDNPIndex_Moto_TestItem::GetReportHeads()
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
LPCTSTR GetDNPIndex_Moto_TestItem::GetReportLowLimit()
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
LPCTSTR GetDNPIndex_Moto_TestItem::GetReportHightLimit()
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
LPCTSTR GetDNPIndex_Moto_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}
void GetDNPIndex_Moto_TestItem::GetOTPData(WBInf &MyWBInf)
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
		temp.Format(_T("当前模组 OTP_R=%d"),MyWBInf.OTP_R);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_Gr=%d"),MyWBInf.OTP_Gr);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_Gb=%d"),MyWBInf.OTP_Gb);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_B=%d"),MyWBInf.OTP_B);
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

#define LSC_PATH_DEFINE								0
BOOL GetDNPIndex_Moto_TestItem::GetAWBData(ImageInfo img,WBInf &MyWbInf)
{
#if LSC_PATH_DEFINE == 1
	char AWB_LSC_File[1024]={0};
	char ONLY_LSC_File[1024]={0};
	char ONLY_AWB_File[1024]={0};
#endif
	int R,Gr,Gb,B;
	CString ToolPath;
	ToolPath.Format(_T("AWBData\\Cam%d\\"),CamID);
	ToolPath=m_pInterface->Ctrl_GetCurrentPath()+ToolPath;
	SetCurrentDirectory(ToolPath);
	CString sfilename=ToolPath+_T("AWB_LSC_CALIBRATION_DATA.txt");
#if LSC_PATH_DEFINE == 1
	USES_CONVERSION;
	sprintf(AWB_LSC_File,W2A(ToolPath.GetBuffer()));
#endif
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB_LSC_CALIBRATION_DATA.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt");
#if LSC_PATH_DEFINE == 1
	sprintf(ONLY_LSC_File,ToolPath.GetBuffer());
#endif
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}

	sfilename=m_pInterface->Ctrl_GetCurrentPath()+_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt");
#if LSC_PATH_DEFINE == 1
	sprintf(ONLY_AWB_File,ToolPath.GetBuffer());
#endif
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	
#if LSC_PATH_DEFINE == 1
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7,char * AWB_LSC_Path,char * ONLY_LSC_Path,char * ONLY_AWB_Path);
#else
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7);
#endif

	HINSTANCE hDll;   //DLL句柄 

	//SetCurrentDirectory(m_pInterface->Ctrl_GetCurrentPath());		
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

#if LSC_PATH_DEFINE == 1
	LensCorrection(Raw8buffer,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,0,AWB_LSC_File,ONLY_LSC_File,ONLY_AWB_File);
#else
	LensCorrection(Raw8buffer,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,0);
#endif
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

	int Multiple = pDlg->m_Multiple;

	tempStr.Format(_T("Multiple = %d"), Multiple);
	m_pInterface->Ctrl_Addlog(CamID, tempStr, COLOR_BLUE, 200);

	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	if(GAverFlag==1)
	{
		MyWbInf.RG = int( (float)Multiple*2.0*(MyWbInf.R)/(float)((MyWbInf.Gr + MyWbInf.Gb)) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)Multiple*2.0*(MyWbInf.B)/(float)((MyWbInf.Gr + MyWbInf.Gb)) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)Multiple*2.0*(MyWbInf.Gb)/(float)((MyWbInf.Gr+ MyWbInf.Gb))+ AA);
	}
	else if (GAverFlag==2)
	{
		MyWbInf.RG = int( (float)Multiple*(MyWbInf.R)/(float)(MyWbInf.Gr) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)Multiple*(MyWbInf.B)/(float)(MyWbInf.Gr) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)Multiple*(MyWbInf.Gb)/(float)(MyWbInf.Gr)+AA);
	}
	else if (GAverFlag==3)
	{
		MyWbInf.RG = int( (float)Multiple*(MyWbInf.R)/(float)(MyWbInf.Gb) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)Multiple*(MyWbInf.B)/(float)(MyWbInf.Gb) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)Multiple*(MyWbInf.Gr)/(float)(MyWbInf.Gb)+AA);
	}
	return TRUE;
}

/** 
*	对于多色温的情况，目前不好处理各个色温的Ev、x、y的卡控，这点不好参考联想的案子。
*	因此，色温的Ev、x、y值就不判断了，只判断机种名和时间就行了。
*	--Added by LiHai--20171221
**/
BOOL GetDNPIndex_Moto_TestItem::GetLightSource(void) //对应DNP Light Source Index对话框中的[获取光源数据]
{
	m_pInterface->Ctrl_Addlog(CamID,_T("Enter into GetDNPIndex_Moto_TestItem::OnGetLightSource()\r\n"),COLOR_BLUE,200);

	// TODO: Add your control notification handler code here	
 	wchar_t station[256];
	RegReadKey(_T("LightSource"), _T("Time"), station);
	int m_CalTimeTag = _tstoi(station);
	int iNowTimeTag = GetTimeTag();
	/********** 光源点检的时间防呆 ***************/
	if (abs(iNowTimeTag - m_CalTimeTag) > pDlg->m_CalDiff)
	{
		CString strLog;

		strLog.Format(_T("当前时间:%d---距离上次照度计校准光源超过%d小时，请拿照度计重新获取校验"), iNowTimeTag, pDlg->m_CalDiff);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, strLog.GetBuffer(strLog.GetLength()));
		return FALSE;
	}

	wchar_t TestStation[128];
	RegReadKey(_T("LightSource"), _T(""), TestStation);//获取机种名
	CString TestStation_Reg = TestStation;
	/********** 机种防呆 ***************/
	if (TestStation_Reg.IsEmpty() || pDlg->Station.Compare(TestStation_Reg))
	{
		CString str;

		str.Format(_T("机种错误，请重新进行LightSource校准---LightSource TestStation:%s Current Station:%s\r\n"), TestStation_Reg.GetBuffer(TestStation_Reg.GetLength()), pDlg->Station.GetBuffer(pDlg->Station.GetLength()));
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, str.GetBuffer(str.GetLength()));
		return FALSE;
	}

	RegReadKey(_T("LightSource"), _T("5100K_Ev"), station);
	m_HighCCT_Ev = _tstoi(station);

	RegReadKey(_T("LightSource"), _T("5100K_u"), station);
	m_HighCCT_u = _tstol(station);

	RegReadKey(_T("LightSource"), _T("5100K_v"), station);
	m_HighCCT_v = _tstol(station);

	/**********HighCCT卡控***************/
	if ((m_HighCCT_Ev > pDlg->m_Ev_HighCCT_Max) || (m_HighCCT_Ev < pDlg->m_Ev_HighCCT_Min))
	{
		CString str;

		str.Format(_T("Ev_5000k = %d, not in<%d, %d>\r\n"), m_HighCCT_Ev, pDlg->m_Ev_HighCCT_Min, pDlg->m_Ev_HighCCT_Max);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
		return FALSE;
	}
	if ((m_HighCCT_u > pDlg->m_U_HighCCT_Max) || (m_HighCCT_u < pDlg->m_U_HighCCT_Min))
	{
		CString str;

		str.Format(_T("U_5000k = %d, not in<%d, %d>\r\n"), m_HighCCT_u, pDlg->m_U_HighCCT_Min, pDlg->m_U_HighCCT_Max);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
		return FALSE;
	}
	if ((m_HighCCT_v > pDlg->m_V_HighCCT_Max) || (m_HighCCT_v < pDlg->m_V_HighCCT_Min))
	{
		CString str;

		str.Format(_T("V_5000k = %d, not in<%d, %d>\r\n"), m_HighCCT_v, pDlg->m_V_HighCCT_Min, pDlg->m_V_HighCCT_Max);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
		return FALSE;
	}
	float m_tolerance_HighCCT = sqrt(pow((float)(m_HighCCT_u - pDlg->m_U_Standard_HighCCT), 2) + pow((float)(m_HighCCT_v - pDlg->m_V_Standard_HighCCT), 2));
	if (m_tolerance_HighCCT > pDlg->m_Tolerance_HighCCT_Max)
	{
		CString str;

		str.Format(_T("tolerance_5000k=%.4f >%.4f\r\n"), m_tolerance_HighCCT, pDlg->m_Tolerance_HighCCT_Max);
		m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
		return FALSE;
	}

	CString str;
	str.Format(_T("Ev_5000k = %d, in<%d, %d>\r\n"), m_HighCCT_Ev, pDlg->m_Ev_HighCCT_Min, pDlg->m_Ev_HighCCT_Max);
	m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);

	str.Format(_T("U_5000k = %d, in<%d, %d>\r\n"), m_HighCCT_u, pDlg->m_U_HighCCT_Min, pDlg->m_U_HighCCT_Max);
	m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);

	str.Format(_T("V_5000k = %d, in<%d, %d>\r\n"), m_HighCCT_v, pDlg->m_V_HighCCT_Min, pDlg->m_V_HighCCT_Max);
	m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);

	str.Format(_T("tolerance_5000k = %.4f < %.4f\r\n"), m_tolerance_HighCCT, pDlg->m_Tolerance_HighCCT_Max);
	m_pInterface->Ctrl_Addlog(CamID, str.GetBuffer(str.GetLength()), COLOR_BLUE, 200);

	return TRUE;
}

BOOL GetDNPIndex_Moto_TestItem::SaveMotoDynamic_DNPData(void)
{
#if 1	//校准后新的Golden值将在ApplyAWB.dll中使用，所以需要写入wbsetting键值下面--Added by LiHai--20180505
	CString suffix = _T("");
	if (pDlg->m_count != 0)
	{
		suffix.Format(_T("_%d"), pDlg->m_count);
	}

	CString CurrentName = GetName();
	SetName(_T("WBSetting"));

	//Golden Channel
	WriteConfigInt(_T("Golden_R") + suffix, m_CurVal.m_R);
	WriteConfigInt(_T("Golden_Gr") + suffix, m_CurVal.m_Gr);
	WriteConfigInt(_T("Golden_Gb") + suffix, m_CurVal.m_Gb);
	WriteConfigInt(_T("Golden_B") + suffix, m_CurVal.m_B);

	//Golden Gain
	WriteConfigInt(_T("Golden_RG") + suffix, m_CurVal.m_RGain);
	WriteConfigInt(_T("Golden_BG") + suffix, m_CurVal.m_BGain);
	WriteConfigInt(_T("Golden_GG") + suffix, m_CurVal.m_GGain);

	//LightSource
	WriteConfigInt(_T("Ev_5000k"), m_HighCCT_Ev);
	WriteConfigInt(_T("u_5000k"), m_HighCCT_u);
	WriteConfigInt(_T("v_5000k"), m_HighCCT_v);
#else
	USES_CONVERSION;
	CString FilePath;
	CString FileName;

	FilePath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\"), CamID);
	FilePath = m_pInterface->Ctrl_GetCurrentPath() + FilePath;

	int siteNo = m_pInterface->GetSiteNo();
	_mkdir(T2A(FilePath));
	if (pDlg->m_count != 0)
	{
		FileName.Format(_T("DNP_Data_%d_%d.ini"), siteNo, pDlg->m_count);
	}
	else
	{
		FileName.Format(_T("DNP_Data_%d.ini"), siteNo);//写入的路径为【F:\OHP0792\Execute\Debug\otpdataManager】, 还可以使用m_pInterface->Otp_WriteToOtpBoard函数存储
	}
	FileName = FilePath + FileName;
	//删除旧文件
	if (PathFileExists(FileName))
	{
		if (!DeleteFile(FileName))
		{
			CString TempMsg;
			TempMsg.Format(_T("无法删除旧的：%s 文件!"), FileName);
			m_pInterface->Ctrl_Addlog(CamID, TempMsg, COLOR_RED, 200);
			return FALSE;
		}
	}

	char  section[128];

	CTime time = CTime::GetCurrentTime();
	int day   = time.GetDay();
	int hour  = time.GetHour();
	int month = time.GetMonth();

	ofstream outfile(FileName, ios::out|ios::trunc);
	outfile<<"[DNP_Data]"<<endl;

	outfile<<"Ev_5000k="<<itoa(m_HighCCT_Ev, section, 10)<<endl;
	outfile<<"u_5000k="<<ltoa(m_HighCCT_u, section, 10)<<endl;
	outfile<<"v_5000k="<<ltoa(m_HighCCT_v, section, 10)<<endl;

	outfile<<"Month="<<itoa(month, section, 10)<<endl;
	outfile<<"Day="<<itoa(day, section, 10)<<endl;
	outfile<<"Hour="<<itoa(hour, section, 10)<<endl;

	outfile<<"Golden_R="<<itoa(m_CurVal.m_R, section, 10)<<endl;
	outfile<<"Golden_Gr="<<itoa(m_CurVal.m_Gr, section, 10)<<endl;
	outfile<<"Golden_Gb="<<itoa(m_CurVal.m_Gb, section, 10)<<endl;
	outfile<<"Golden_B="<<itoa(m_CurVal.m_B, section, 10)<<endl;

	outfile<<"Golden_RG="<<itoa(m_CurVal.m_RGain, section, 10)<<endl;
	outfile<<"Golden_BG="<<itoa(m_CurVal.m_BGain, section, 10)<<endl;
	outfile<<"Golden_GG="<<itoa(m_CurVal.m_GGain, section, 10)<<endl;

	outfile.close();
#endif
	return TRUE;
}
