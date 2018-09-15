#include "StdAfx.h"
#include "WBCheck_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new WBCheck_TestItem(pInterface, lpszName);
}


/******************************************************************
º¯ÊýÃû:    ¹¹Ôìº¯Êý
º¯Êý¹¦ÄÜ:  ¶ÔÏóÉú³ÉµÄÊ±ºò³õÊ¼±ØÐëÍê³ÉµÄÅäÖÃ
*******************************************************************/
WBCheck_TestItem::WBCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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
	flag=0;

	//.....
}

WBCheck_TestItem::~WBCheck_TestItem(void)
{

}


int WBCheck_TestItem::InitItem()
{
	CamID = GetCamID();
	CurrentName=GetName();
	GlobalSetting=_T("WBSetting");
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //ÉèÖÃCONFIGÂ·¾¶	
	LoadOption(); 
	return 0;
}


/******************************************************************
º¯ÊýÃû:    Initialize
º¯Êý¹¦ÄÜ:  ÓÃÓÚ²âÊÔÇ°³õÊ¼»¯Ò»Ð©±äÁ¿£¬×´Ì¬£¬·ÖÅä¿Õ¼äµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int WBCheck_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:ÔÚ´ËÌí¼Ó³õÊ¼»¯´úÂë
	AfterBG=0;
	AfterGG=0;
	AfterR=0;
	AfterGr=0;
	AfterGb=0;
	AfterB=0;
	AfterRG_fin=0;
	AfterBG_fin=0;
	AfterGG_fin=0;
	AfterR_fin=0;
	AfterGr_fin=0;
	AfterGb_fin=0;
	AfterB_fin=0;
	Afterdistance=0.0;//保存数据
	return 0;
}



/******************************************************************
º¯ÊýÃû:    Testing
º¯Êý¹¦ÄÜ:  Íê³ÉÄ³Ò»Ïî²âÊÔ¹¦ÄÜ´úÂë·ÅÓë´Ë
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int WBCheck_TestItem::Testing()
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
	if (flag==0)
	{
		flag=BeforeAWB();
		return flag;
	}
	else
	{
		if(flag>=3)
		{
			flag=0;
			int iresult=AfterAWB();
			if (iresult==0)
			{
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Pass!"));
				SetResult(RESULT_PASS);
			}
		}
		else
		{
			flag++;
			return 1;
		}
		
	}
	

   return 0;
}


/******************************************************************
º¯ÊýÃû:    Finalize
º¯Êý¹¦ÄÜ:  ÓÃÓÚInitializeÉêÃ÷µÄ±äÁ¿»Ö¸´Ô­Ê¼Öµ£¬ÉêÇëµÄÄÚ´æ¿Õ¼äÊÍ·ÅµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int WBCheck_TestItem::Finalize()
{
	//TODO:ÔÚ´ËÌí¼Ó»ØÊÕ½áÊø´úÂë


	return 0;
}


/******************************************************************
º¯ÊýÃû:    LoadOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡²ÎÊýµ½±äÁ¿£¬ºÍ¸³Öµµ½¶Ô»°¿òµÄ°ó¶¨±äÁ¿ (ÎÄ¼þ´æ´¢Öµ->±äÁ¿¡ª>¿Ø¼þµÄÖµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int WBCheck_TestItem::LoadOption()
{
	//TODO: ÔÚ´ËÌí¼Ó¶ÁÈ¡´úÂë 
	pDlg->m_count=ReadConfigInt(_T("count"),12);
	CString suffix=_T("");
	if (pDlg->m_count!=0)
		suffix.Format(_T("_%d"),pDlg->m_count);
	SetName(GlobalSetting);	
	pDlg->m_QTITool=ReadConfigInt(_T("QTITool"),0);
	pDlg->m_flow=ReadConfigInt(_T("flow"),1);
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
	pDlg->m_before_distance=ReadConfigInt(_T("before_distance_check")+suffix, 50);
	pDlg->m_GG_GainEn=ReadConfigInt(_T("GG_GainEn"), 0);
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
	pDlg->RG_min=ReadConfigInt(_T("RG_min")+suffix,220);
	pDlg->RG_max=ReadConfigInt(_T("RG_max")+suffix,1000);
	pDlg->BG_min=ReadConfigInt(_T("BG_min")+suffix,220);
	pDlg->BG_max=ReadConfigInt(_T("BG_max")+suffix,1000);
	pDlg->GG_min=ReadConfigInt(_T("GG_min")+suffix,220);
	pDlg->GG_max=ReadConfigInt(_T("GG_max")+suffix,1000);
	pDlg->ByteCount=ReadConfigString(_T("ByteCount"), _T("1"));
	pDlg->A_Multi=ReadConfigString(_T("A_Multi"), _T("1"));
	pDlg->B_Multi=ReadConfigString(_T("B_Multi"), _T("0"));
	pDlg->Station=ReadConfigString(_T("Station"), _T("OHWBA06"));
	pDlg->SingleChannelEnable=ReadConfigInt(_T("SingleChannelEnable"),1);
	pDlg->GainEnable=ReadConfigInt(_T("GainEnable"),0);
	pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
	pDlg->m_Ratio=ReadConfigInt(_T("Ratio"),5);
	pDlg->m_index=ReadConfigInt(_T("index"),512);
	pDlg->page=ReadConfigInt(_T("page")+suffix,1);
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
	pDlg->m_TypicalR  = ReadConfigInt(_T("Typical_R")+suffix,200);
	pDlg->m_TypicalGr  = ReadConfigInt(_T("Typical_Gr")+suffix,200);
	pDlg->m_TypicalGb  = ReadConfigInt(_T("Typical_Gb")+suffix,200);
	pDlg->m_TypicalB  = ReadConfigInt(_T("Typical_B")+suffix,200);

	//Typical Gain
	pDlg->m_TypicalRG  = ReadConfigInt(_T("Typical_RG")+suffix,200);
	pDlg->m_TypicalBG  = ReadConfigInt(_T("Typical_BG")+suffix,200);
	pDlg->m_TypicalGG  = ReadConfigInt(_T("Typical_GG")+suffix,200);
	pDlg->m_Raw8_EN  = ReadConfigInt(_T("Raw8_EN"),1);
	pDlg->m_distance  = ReadConfigInt(_T("distance_check")+suffix,1);
	R_index=ReadConfigInt(_T("R_index")+suffix,10000);
	Gr_index=ReadConfigInt(_T("Gr_index")+suffix,10000);
	Gb_index=ReadConfigInt(_T("Gb_index")+suffix,10000);
	B_index=ReadConfigInt(_T("B_index")+suffix,10000);
	RG_index=ReadConfigInt(_T("RG_index")+suffix,10000);
	BG_index=ReadConfigInt(_T("BG_index")+suffix,10000);
    GG_index=ReadConfigInt(_T("GG_index")+suffix,10000);

	SetName(CurrentName);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
º¯ÊýÃû:    SaveOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ½«±äÁ¿ÊýÖµ»ò¶Ô»°¿òµÄ°ó¶¨±äÁ¿ÊýÖµ±£´æµ½ÅäÖÃÎÄ¼þÖÐ (¿Ø¼þµÄÖµ¡ª>±äÁ¿->ÎÄ¼þ´æ´¢Öµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int WBCheck_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: ÔÚ´ËÌí¼Ó´æ´¢´úÂë
	CString suffix=_T("");
	if (pDlg->m_count!=0)
		suffix.Format(_T("_%d"),pDlg->m_count);
	WriteConfigInt(_T("count"),pDlg->m_count);
	SetName(GlobalSetting);
	WriteConfigInt(_T("QTITool"),pDlg->m_QTITool);
	WriteConfigInt(_T("flow"),pDlg->m_flow);
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
	WriteConfigString(_T("ByteCount"), pDlg->ByteCount);
	WriteConfigString(_T("A_Multi"), pDlg->A_Multi);
	WriteConfigString(_T("B_Multi"), pDlg->B_Multi);
	WriteConfigString(_T("Station"), pDlg->Station);
	WriteConfigInt(_T("SingleChannelEnable"),pDlg->SingleChannelEnable);
	WriteConfigInt(_T("GainEnable"),pDlg->GainEnable);
	WriteConfigInt(_T("Ratio"),pDlg->m_Ratio);
	WriteConfigInt(_T("BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("index"),pDlg->m_index);
	WriteConfigInt(_T("Typical_R")+suffix, pDlg->m_TypicalR);
	WriteConfigInt(_T("Typical_Gr")+suffix, pDlg->m_TypicalGr);
	WriteConfigInt(_T("Typical_Gb")+suffix, pDlg->m_TypicalGb);
	WriteConfigInt(_T("Typical_B")+suffix, pDlg->m_TypicalB);

	//Typical Gain
	WriteConfigInt(_T("Typical_RG")+suffix, pDlg->m_TypicalRG);
	WriteConfigInt(_T("Typical_BG")+suffix, pDlg->m_TypicalBG);
	WriteConfigInt(_T("Typical_GG")+suffix, pDlg->m_TypicalGG);
	WriteConfigInt(_T("Raw8_EN"), pDlg->m_Raw8_EN);
	WriteConfigInt(_T("distance_check")+suffix, pDlg->m_distance);
	WriteConfigInt(_T("before_distance_check")+suffix, pDlg->m_before_distance);
	WriteConfigInt(_T("GG_GainEn"), pDlg->m_GG_GainEn);
	WriteConfigInt(_T("page")+suffix, pDlg->page);
	SetName(CurrentName);

	//........................
	return 0;
}


/******************************************************************
º¯ÊýÃû:    GetReportHeaders
º¯Êý¹¦ÄÜ:  ±£´æÐèÒª±£´æµÄ²âÊÔÏîÄ¿Ãû×ÖÖÖÀà
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR WBCheck_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");

	header += _T("APPLY_R,APPLY_Gr,APPLY_Gb,APPLY_B,");

	if(pDlg->GainEnable)
	{
		header += _T("APPLY_RGain,APPLY_BGain,APPLY_GGain,");
		header += _T("APPLY_RGain_fin,APPLY_BGain_fin,APPLY_GGain_fin,Typical_RG,Typical_BG,Typical_GG,RGIndex,BGIndex,GGIndex,");
	}

	if(pDlg->SingleChannelEnable)
	{
		header += _T("APPLY_R_fin,APPLY_Gr_fin,APPLY_Gb_fin,APPLY_B_fin,");
		header += _T("APPLY_RGain_fin,APPLY_BGain_fin,APPLY_GGain_fin,Typical_R,Typical_Gr,Typical_Gb,Typical_B,RIndex,GrIndex,GbIndex,BIndex,");
	}
	header += _T("AfterDistance,");
	return header;
}


/******************************************************************
º¯ÊýÃû:    GetReportLowerLimits
º¯Êý¹¦ÄÜ:  ±£´æ²âÊÔ±ê×¼µÄÏÂÏÞÖµ
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR WBCheck_TestItem::GetReportLowLimit()
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
LPCTSTR WBCheck_TestItem::GetReportHightLimit()
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
LPCTSTR WBCheck_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	CString temp;
	temp.Format(_T("%d,%d,%d,%d,"),AfterR,AfterGr,AfterGb,AfterB);
	Content+=temp;


	if(pDlg->GainEnable)
	{
		temp.Format(_T("%d,%d,%d,"),AfterRG,AfterBG,AfterGG);
		Content+=temp;
		temp.Format(_T("%d,%d,%d,"),AfterRG_fin,AfterBG_fin,AfterGG_fin);
		Content+=temp;
		temp.Format(_T("%d,%d,%d,%d,%d,%d,"),pDlg->m_TypicalRG,pDlg->m_TypicalBG,pDlg->m_TypicalGG,RG_index, BG_index,GG_index);
		Content+=temp;
	}

	if(pDlg->SingleChannelEnable)
	{
		temp.Format(_T("%d,%d,%d,%d,"),AfterR_fin,AfterGr_fin,AfterGb_fin,AfterB_fin);
		Content+=temp;
		temp.Format(_T("%d,%d,%d,"),AfterRG_fin,AfterBG_fin,AfterGG_fin);
		Content+=temp;
		temp.Format(_T("%d,%d,%d,%d,%d,%d,%d,%d,"),pDlg->m_TypicalR,pDlg->m_TypicalGr,pDlg->m_TypicalGb,pDlg->m_TypicalB,R_index,Gr_index,Gb_index,B_index);
		Content+=temp;
	}
	temp.Format(_T("%.2f"),Afterdistance);
	Content+=temp;
	return Content;
}
int WBCheck_TestItem::BeforeAWB()
{
	USHORT OTP_R=0;
	USHORT OTP_Gr=0;
	USHORT OTP_Gb=0;
	USHORT OTP_B=0;
	USHORT OTP_RG=0;
	USHORT OTP_BG=0;
	USHORT OTP_GG=0;
	Typical_RG=0;
	Typical_BG=0;
	Typical_GG=0;

	

	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}

	if (ByteCount==1)
	{
		if (pDlg->SingleChannelEnable)
		{
			m_pInterface->Otp_OtpRead(R_StartAdd,R_StartAdd,&OTP_R,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(Gr_StartAdd,Gr_StartAdd,&OTP_Gr,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(Gb_StartAdd,Gb_StartAdd,&OTP_Gb,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(B_StartAdd,B_StartAdd,&OTP_B,CamID,pDlg->page);

			if (GAverFlag == 1)
			{
				OTP_RG = 2.0*pDlg->m_index*OTP_R / (OTP_Gr+OTP_Gb)+AA;
				OTP_BG = 2.0*pDlg->m_index*OTP_B / (OTP_Gr+OTP_Gb)+AA;
				OTP_GG = 1.0*pDlg->m_index*OTP_Gb / OTP_Gr+AA;

				Typical_RG = 2.0*pDlg->m_index*pDlg->m_TypicalR/(pDlg->m_TypicalGr+pDlg->m_TypicalGb)+AA;
				Typical_BG = 2.0*pDlg->m_index*pDlg->m_TypicalB/(pDlg->m_TypicalGr+pDlg->m_TypicalGb)+AA;
				Typical_GG = 1.0*pDlg->m_index*pDlg->m_TypicalGb/pDlg->m_TypicalGr + AA;
			}
			else if (GAverFlag==2)
			{
				OTP_RG = 1.0*pDlg->m_index*OTP_R / OTP_Gr+AA;
				OTP_BG = 1.0*pDlg->m_index*OTP_B / OTP_Gr+AA;
				OTP_GG = 1.0*pDlg->m_index*OTP_Gb / OTP_Gr+AA;

				Typical_RG = 1.0*pDlg->m_index*pDlg->m_TypicalR/pDlg->m_TypicalGr+AA;
				Typical_BG = 1.0*pDlg->m_index*pDlg->m_TypicalB/pDlg->m_TypicalGr+AA;
				Typical_GG = 1.0*pDlg->m_index*pDlg->m_TypicalGb/pDlg->m_TypicalGr+AA;
			}
			else if (GAverFlag==3)
			{
				OTP_RG = 1.0*pDlg->m_index*OTP_R / OTP_Gb+AA;
				OTP_BG = 1.0*pDlg->m_index*OTP_B / OTP_Gb+AA;
				OTP_GG = 1.0*pDlg->m_index*OTP_Gr / OTP_Gb+AA;

				Typical_RG = 1.0*pDlg->m_index*pDlg->m_TypicalR/pDlg->m_TypicalGb+AA;
				Typical_BG = 1.0*pDlg->m_index*pDlg->m_TypicalB/pDlg->m_TypicalGb+AA;
				Typical_GG = 1.0*pDlg->m_index*pDlg->m_TypicalGr/pDlg->m_TypicalGb+AA;
			}
		}
		if (pDlg->GainEnable)
		{
			m_pInterface->Otp_OtpRead(RG_StartAdd,RG_StartAdd,&OTP_RG,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(BG_StartAdd,BG_StartAdd,&OTP_BG,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(GG_StartAdd,GG_StartAdd,&OTP_GG,CamID,pDlg->page);

			Typical_RG=pDlg->m_TypicalRG;
			Typical_BG=pDlg->m_TypicalBG;
			Typical_GG=pDlg->m_TypicalGG;
		}

	}
	else if (ByteCount==2)
	{
		if (pDlg->SingleChannelEnable)
		{
			USHORT OTPData[2]={0};
			m_pInterface->Otp_OtpRead(R_StartAdd,R_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_R=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(Gr_StartAdd,Gr_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_Gr=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(Gb_StartAdd,Gb_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_Gb=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(B_StartAdd,B_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_B=OTPData[0]*A_Multi+OTPData[1]*B_Multi;

			if (GAverFlag == 1)
			{
				OTP_RG = 2.0*pDlg->m_index*OTP_R / (OTP_Gr+OTP_Gb)+AA;
				OTP_BG = 2.0*pDlg->m_index*OTP_B / (OTP_Gr+OTP_Gb)+AA;
				OTP_GG = 1.0*pDlg->m_index*OTP_Gb / OTP_Gr+AA;

				Typical_RG = 2.0*pDlg->m_index*pDlg->m_TypicalR/(pDlg->m_TypicalGr+pDlg->m_TypicalGb)+AA;
				Typical_BG = 2.0*pDlg->m_index*pDlg->m_TypicalB/(pDlg->m_TypicalGr+pDlg->m_TypicalGb)+AA;
				Typical_GG = 1.0*pDlg->m_index*pDlg->m_TypicalGb/pDlg->m_TypicalGr + AA;
			}
			else if (GAverFlag==2)
			{
				OTP_RG = 1.0*pDlg->m_index*OTP_R / OTP_Gr+AA;
				OTP_BG = 1.0*pDlg->m_index*OTP_B / OTP_Gr+AA;
				OTP_GG = 1.0*pDlg->m_index*OTP_Gb / OTP_Gr+AA;

				Typical_RG = 1.0*pDlg->m_index*pDlg->m_TypicalR/pDlg->m_TypicalGr+AA;
				Typical_BG = 1.0*pDlg->m_index*pDlg->m_TypicalB/pDlg->m_TypicalGr+AA;
				Typical_GG = 1.0*pDlg->m_index*pDlg->m_TypicalGb/pDlg->m_TypicalGr+AA;
			}
			else if (GAverFlag==3)
			{
				OTP_RG = 1.0*pDlg->m_index*OTP_R / OTP_Gb+AA;
				OTP_BG = 1.0*pDlg->m_index*OTP_B / OTP_Gb+AA;
				OTP_GG = 1.0*pDlg->m_index*OTP_Gr / OTP_Gb+AA;

				Typical_RG = 1.0*pDlg->m_index*pDlg->m_TypicalR/pDlg->m_TypicalGb+AA;
				Typical_BG = 1.0*pDlg->m_index*pDlg->m_TypicalB/pDlg->m_TypicalGb+AA;
				Typical_GG = 1.0*pDlg->m_index*pDlg->m_TypicalGr/pDlg->m_TypicalGb+AA;
			}
		}
		if (pDlg->GainEnable)
		{
			USHORT OTPData[2]={0};
			m_pInterface->Otp_OtpRead(RG_StartAdd,RG_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_RG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(BG_StartAdd,BG_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_BG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;

			m_pInterface->Otp_OtpRead(GG_StartAdd,GG_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_GG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;

			if ((OTP_RG<=0)||(OTP_BG<=0))
			{
				CString strTemp;
				strTemp.Format(_T("RG,BG异常，不能为0，RG=%d BG=%d!"),OTP_RG,OTP_BG);
				m_pInterface->Ctrl_Addlog(CamID,strTemp,COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,strTemp);
				SetResult(RESULT_FAIL);
				return 0;
			}

			Typical_RG=pDlg->m_TypicalRG;
			Typical_BG=pDlg->m_TypicalBG;
			Typical_GG=pDlg->m_TypicalGG;
		}
	}
	CString temp;
	BOOL WBResult=TRUE;
	if (pDlg->SingleChannelEnable)
	{
		temp.Format(_T("当前模组 OTP_R=%d"),OTP_R);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前模组 OTP_Gr=%d"),OTP_Gr);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前模组 OTP_Gb=%d"),OTP_Gb);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		temp.Format(_T("当前模组 OTP_B=%d"),OTP_B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
		if (OTP_R>pDlg->R_max||OTP_R<pDlg->R_min) WBResult=FALSE;
		if (OTP_Gr>pDlg->Gr_max||OTP_Gr<pDlg->Gr_min) WBResult=FALSE;
		if (OTP_Gb>pDlg->Gb_max||OTP_Gb<pDlg->Gb_min) WBResult=FALSE;
		if (OTP_B>pDlg->B_max||OTP_B<pDlg->B_min) WBResult=FALSE;
		if (WBResult!=TRUE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("单通道R Gr Gb B超规格!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("单通道R Gr Gb B超规格!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	if (pDlg->GainEnable)
	{
		temp.Format(_T("当前模组 OTP_RG=%d"),OTP_RG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_BG=%d"),OTP_BG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_GG=%d"),OTP_GG); 
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		if (OTP_RG>pDlg->RG_max||OTP_RG<pDlg->RG_min) WBResult=FALSE;
		if (OTP_BG>pDlg->BG_max||OTP_BG<pDlg->BG_min) WBResult=FALSE;
		if (OTP_GG>pDlg->GG_max||OTP_GG<pDlg->GG_min) WBResult=FALSE;
		if (WBResult!=TRUE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Gain值超规格!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Gain值超规格!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
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
	if (pDlg->m_QTITool)
	{
		if(!GetAWBData(MyImageInfo[0],MyWBInf))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	else
	{
		CenterBlockRawInfo(MyImageInfo,pDlg->m_Ratio,MyWBInf,pDlg->m_index,pDlg->m_BLC);
	}

	temp.Format(_T("Typical RG=%d"),Typical_RG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

	temp.Format(_T("Typical BG=%d"),Typical_BG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

	temp.Format(_T("补偿前影像 R=%d"),MyWBInf.R);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 Gr=%d"),MyWBInf.Gr);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 Gb=%d"),MyWBInf.Gb);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 B=%d"),MyWBInf.B);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 RG=%d"),MyWBInf.RG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 BG=%d"),MyWBInf.BG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 GG=%d"),MyWBInf.GG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

	if (pDlg->SingleChannelEnable)
	{
		MyWBInf.R=((float)MyWBInf.R*R_index/10000.0+0.5);
		MyWBInf.Gr=((float)MyWBInf.Gr*Gr_index/10000.0+0.5);
		MyWBInf.Gb=((float)MyWBInf.Gb*Gb_index/10000.0+0.5);
		MyWBInf.B=((float)MyWBInf.B*B_index/10000.0+0.5);
		temp.Format(_T("x系数 R=%d index=%d"),MyWBInf.R,R_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 Gr=%d index=%d"),MyWBInf.Gr,Gr_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 Gb=%d index=%d"),MyWBInf.Gb,Gb_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 B=%d index=%d"),MyWBInf.B,B_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		if (GAverFlag == 1)
		{
			MyWBInf.RG=MyWBInf.R*pDlg->m_index*2.0/(MyWBInf.Gr+MyWBInf.Gb)+AA;
			MyWBInf.BG=MyWBInf.B*pDlg->m_index*2.0/(MyWBInf.Gr+MyWBInf.Gb)+AA;
		}
		else if (GAverFlag == 2)
		{
			MyWBInf.RG=MyWBInf.R*pDlg->m_index*1.0/MyWBInf.Gr+AA;
			MyWBInf.BG=MyWBInf.B*pDlg->m_index*1.0/MyWBInf.Gr+AA;
		}
		else if (GAverFlag == 3)
		{
			MyWBInf.RG=MyWBInf.R*pDlg->m_index*1.0/MyWBInf.Gb+AA;
			MyWBInf.BG=MyWBInf.B*pDlg->m_index*1.0/MyWBInf.Gb+AA;
		}
				
		temp.Format(_T("RG=%d"),MyWBInf.RG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("BG=%d"),MyWBInf.BG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	if (pDlg->GainEnable)
	{
		MyWBInf.RG=((float)MyWBInf.RG*RG_index/10000.0+0.5);
		MyWBInf.BG=((float)MyWBInf.BG*BG_index/10000.0+0.5);
		MyWBInf.GG=((float)MyWBInf.GG*GG_index/10000.0+0.5);
		temp.Format(_T("x系数 RG=%d index=%d"),MyWBInf.RG,RG_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 BG=%d index=%d"),MyWBInf.BG,BG_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 GG=%d index=%d"),MyWBInf.GG,GG_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	float distance=0;
	if (pDlg->m_GG_GainEn)
	{
		distance=(float)sqrt(pow((MyWBInf.RG-Typical_RG)*1.0/Typical_RG,2) + pow((MyWBInf.BG-Typical_BG)*1.0/Typical_BG,2)+ pow((MyWBInf.GG-Typical_GG)*1.0/Typical_GG,2));
	}
	else
	distance=(float)sqrt(pow((MyWBInf.RG-Typical_RG)*1.0/Typical_RG,2) + pow((MyWBInf.BG-Typical_BG)*1.0/Typical_BG,2));
	temp.Format(_T("补偿前Distance=%.2f"),distance*100);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
	m_pInterface->Ctrl_PauseGrabFrame(CamID);
	Sleep(500);
	m_pInterface->Sensor_ApplyAWBGain(CamID,OTP_RG,OTP_BG,Typical_RG,Typical_BG);
	m_pInterface->Ctrl_ResumeGrabFrame(CamID);
	Sleep(500);
	return 1;
}
int WBCheck_TestItem::AfterAWB()
{
	ImageInfo *MyImageInfo = NULL;		
	MyImageInfo = m_pInterface->Ctrl_GetImgInfo(CamID);
	if (MyImageInfo->Raw_buffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("grab image fail!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("grab image fail!"));
		SetResult(RESULT_FAIL);
		return -1;
	}
	WBInf MyWBInf;
	memset(&MyWBInf,0,sizeof(WBInf));
	CString temp;
	if (pDlg->m_QTITool)
	{
		if(!GetAWBData(MyImageInfo[0],MyWBInf))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	else
	{
		CenterBlockRawInfo(MyImageInfo,pDlg->m_Ratio,MyWBInf,pDlg->m_index,pDlg->m_BLC);
	}
	temp.Format(_T("补偿后影像 R=%d"),MyWBInf.R);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 Gr=%d"),MyWBInf.Gr);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 Gb=%d"),MyWBInf.Gb);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 B=%d"),MyWBInf.B);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 RG=%d"),MyWBInf.RG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 BG=%d"),MyWBInf.BG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 GG=%d"),MyWBInf.GG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	AfterR=MyWBInf.R;
	AfterGr=MyWBInf.Gr;
	AfterGb=MyWBInf.Gb;
	AfterB=MyWBInf.B;
	AfterRG=MyWBInf.RG;
	AfterBG=MyWBInf.BG;
	AfterGG=MyWBInf.GG;

	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}

	if (pDlg->SingleChannelEnable)
	{
		MyWBInf.R=((float)MyWBInf.R*R_index/10000.0+0.5);
		MyWBInf.Gr=((float)MyWBInf.Gr*Gr_index/10000.0+0.5);
		MyWBInf.Gb=((float)MyWBInf.Gb*Gb_index/10000.0+0.5);
		MyWBInf.B=((float)MyWBInf.B*B_index/10000.0+0.5);
		temp.Format(_T("x系数 R=%d index=%d"),MyWBInf.R,R_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 Gr=%d index=%d"),MyWBInf.Gr,Gr_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 Gb=%d index=%d"),MyWBInf.Gb,Gb_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 B=%d index=%d"),MyWBInf.B,B_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		AfterR_fin=MyWBInf.R;
		AfterGr_fin=MyWBInf.Gr;
		AfterGb_fin=MyWBInf.Gb;
		AfterB_fin=MyWBInf.B;

		if (GAverFlag == 1)
		{
			AfterRG_fin=MyWBInf.R*pDlg->m_index*2.0/(MyWBInf.Gr+MyWBInf.Gb)+AA;
			AfterBG_fin=MyWBInf.B*pDlg->m_index*2.0/(MyWBInf.Gr+MyWBInf.Gb)+AA;
			AfterGG_fin=MyWBInf.Gb*pDlg->m_index*1.0/MyWBInf.Gr+AA;
		}
		else if (GAverFlag == 2)
		{
			AfterRG_fin=MyWBInf.R*pDlg->m_index*1.0/MyWBInf.Gr+AA;
			AfterBG_fin=MyWBInf.B*pDlg->m_index*1.0/MyWBInf.Gr+AA;
			AfterGG_fin=MyWBInf.Gb*pDlg->m_index*1.0/MyWBInf.Gr+AA;
		}
		else if (GAverFlag == 3)
		{
			AfterRG_fin=MyWBInf.R*pDlg->m_index*1.0/MyWBInf.Gb+AA;
			AfterBG_fin=MyWBInf.B*pDlg->m_index*1.0/MyWBInf.Gb+AA;
			AfterGG_fin=MyWBInf.Gr*pDlg->m_index*1.0/MyWBInf.Gb+AA;
		}

		temp.Format(_T("RG=%d"),AfterRG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("BG=%d"),AfterBG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	if (pDlg->GainEnable)
	{
		MyWBInf.RG=((float)MyWBInf.RG*RG_index/10000.0+0.5);
		MyWBInf.BG=((float)MyWBInf.BG*BG_index/10000.0+0.5);
		MyWBInf.GG=((float)MyWBInf.GG*GG_index/10000.0+0.5);
		temp.Format(_T("x系数 RG=%d index=%d"),MyWBInf.RG,RG_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 BG=%d index=%d"),MyWBInf.BG,BG_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("x系数 GG=%d index=%d"),MyWBInf.GG,GG_index);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		AfterRG_fin=MyWBInf.RG;
		AfterBG_fin=MyWBInf.BG;
		AfterGG_fin=MyWBInf.GG;
	}
	temp.Format(_T("Typical_RG=%d  Typical_BG=%d"),Typical_RG,Typical_BG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	float distance=0;
	if (pDlg->m_GG_GainEn)
	{
		distance=(float)sqrt(pow((AfterRG_fin-Typical_RG)*1.0/Typical_RG,2) + pow((AfterBG_fin-Typical_BG)*1.0/Typical_BG,2)+ pow((AfterGG-Typical_GG)*1.0/Typical_GG,2));
	}
	else
	    distance=(float)sqrt(pow((AfterRG_fin-Typical_RG)*1.0/Typical_RG,2) + pow((AfterBG_fin-Typical_BG)*1.0/Typical_BG,2));
	temp.Format(_T("补偿后Distance=%.2f"),distance*100);
	Afterdistance=distance*100;
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,200);
	if (distance*1000>pDlg->m_distance)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Distance 超范围!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Distance 超范围!"));
		SetResult(RESULT_FAIL);
		return -1;
	}
	return 0;
}
void WBCheck_TestItem::CenterBlockRawInfo(ImageInfo *mySensor ,int Ratio,WBInf &MyWbInf,int index,int BLC)
{
	USHORT *pIn=new USHORT[mySensor->imgwidth*mySensor->imgheight];
	if (NULL == pIn)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("pIn buffer 申请失败!"),COLOR_RED,200);
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
		memcpy(pIn,mySensor->Raw_buffer,mySensor->imgwidth*mySensor->imgheight*sizeof(USHORT));
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
}
void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}
BOOL WBCheck_TestItem::GetAWBData(ImageInfo img,WBInf &MyWbInf)
{
	char AWB_LSC_File[1024]={0};
	char ONLY_LSC_File[1024]={0};
	char ONLY_AWB_File[1024]={0};
	int R,Gr,Gb,B;
	CString ToolPath;
	ToolPath.Format(_T("AWBData\\Cam%d\\"),CamID);
	ToolPath=m_pInterface->Ctrl_GetCurrentPath()+ToolPath;
	SetCurrentDirectory(ToolPath);

	CString sfilename=ToolPath+_T("AWB_LSC_CALIBRATION_DATA.txt");
	USES_CONVERSION;
	sprintf(AWB_LSC_File,W2A(ToolPath.GetBuffer()));
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB_LSC_CALIBRATION_DATA.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt");
	sprintf(ONLY_LSC_File,W2A(ToolPath.GetBuffer()));
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt");
	sprintf(ONLY_AWB_File,W2A(ToolPath.GetBuffer()));
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}

	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7/*,char * AWB_LSC_Path,char * ONLY_LSC_Path,char * ONLY_AWB_Path*/);

	HINSTANCE hDll;   //DLL句柄 
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
	CFile file1;
	BOOL ret = file1.Open(ToolPath+_T("Raw8.raw"), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if (ret)
	{
		file1.Write((void*)Raw8buffer, width * height * sizeof(BYTE) );
		file1.Close();
	}
	
	LensCorrection(Raw8buffer,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,0/*,AWB_LSC_File,ONLY_LSC_File,ONLY_AWB_File*/);
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
	int index=pDlg->m_index;
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
		MyWbInf.GG =int( (float)index*(MyWbInf.Gb)/(float)(MyWbInf.Gr) + AA);
	}
	else if (GAverFlag==3)
	{
		MyWbInf.RG = int( (float)index*(MyWbInf.R)/(float)(MyWbInf.Gb) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)index*(MyWbInf.B)/(float)(MyWbInf.Gb) + AA ); //Bave/Gave 
		MyWbInf.GG =int( (float)index*(MyWbInf.Gr)/(float)(MyWbInf.Gb) + AA);
	}

	sfilename=ToolPath+_T("AWB_LSC_CALIBRATION_DATA.txt");
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
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}

	return TRUE;
}

