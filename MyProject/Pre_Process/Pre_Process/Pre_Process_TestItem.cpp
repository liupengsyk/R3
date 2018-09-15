#include "StdAfx.h"
#include "Pre_Process_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new Pre_Process_TestItem(pInterface, lpszName);
}


/******************************************************************
º¯ÊýÃû:    ¹¹Ôìº¯Êý
º¯Êý¹¦ÄÜ:  ¶ÔÏóÉú³ÉµÄÊ±ºò³õÊ¼±ØÐëÍê³ÉµÄÅäÖÃ
*******************************************************************/
Pre_Process_TestItem::Pre_Process_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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


	//.....
}

Pre_Process_TestItem::~Pre_Process_TestItem(void)
{

}


int Pre_Process_TestItem::InitItem()
{
	CamID = GetCamID();
	CurrentName=GetName();
	GlobalSetting=_T("WBSetting");
	SetName(GlobalSetting);
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //ÉèÖÃCONFIGÂ·¾¶	
	LoadOption(); 
	SetName(CurrentName);
	return 0;
}


/******************************************************************
º¯ÊýÃû:    Initialize
º¯Êý¹¦ÄÜ:  ÓÃÓÚ²âÊÔÇ°³õÊ¼»¯Ò»Ð©±äÁ¿£¬×´Ì¬£¬·ÖÅä¿Õ¼äµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int Pre_Process_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:ÔÚ´ËÌí¼Ó³õÊ¼»¯´úÂë
 
	return 0;
}



/******************************************************************
º¯ÊýÃû:    Testing
º¯Êý¹¦ÄÜ:  Íê³ÉÄ³Ò»Ïî²âÊÔ¹¦ÄÜ´úÂë·ÅÓë´Ë
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int Pre_Process_TestItem::Testing()
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë 
	//机种防呆
	wchar_t wcstation[32]={0};
	swprintf(wcstation,_T("CamID%d_Site%d"),CamID,m_pInterface->GetSiteNo());
	wchar_t mystation[32]={0};
	RegReadKey(_T(""),wcstation,mystation);
	CString TestStation(mystation);
	if (TestStation!=pDlg->TestStation)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("机种不对，请切换机种!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("机种不对，请切换机种!"));
		SetResult(RESULT_FAIL);
		return 0;
 	}
	CString CurrentFuseID=m_pInterface->Sensor_GetCamFuseID(CamID);
	m_pInterface->Ctrl_Addlog(CamID,CurrentFuseID,COLOR_BLUE,200);
	for (int i=0;i<20;i++)
	{
		if(!CurrentFuseID.Compare(FuseID[i]))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Golden模组不能进行烧录!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Golden模组不能进行烧录!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	int iNowTimeTag=GetTimeTag();
	if(abs(iNowTimeTag-CalTimeTag)>pDlg->CalDiff)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("DNP光箱已超过校验有效期,请拿GoldenSample重新校验!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("DNP光箱已超过校验有效期,请拿GoldenSample重新校验!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	if (pDlg->m_ProductEnable)
	{
		m_pInterface->Ctrl_PauseGrabFrame(CamID);
		Sleep(300);
		USHORT temp;
//        m_pInterface->ReadReg(CamID,ProductAdd,ProductAdd,&temp);
		m_pInterface->Device_ReadI2c(SlaveID,ProductAdd,&temp,3,CamID);
		if(temp!=ProductID)
		{
			Sleep(10);
			m_pInterface->ReadReg(CamID,ProductAdd,ProductAdd,&temp);
		}
		if(temp!=ProductID)
		{
			CString Tempmsg;
			Tempmsg.Format(_T("Sensor版本号不对%d != %d"),temp,ProductID);
			m_pInterface->Ctrl_Addlog(CamID,Tempmsg,COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Sensor版本号不对!"));
			SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			Sleep(300); 
			return 0;
		}
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(300);
	}
	if (pDlg->m_ProductEnable1)
	{
		USHORT temp;
//		m_pInterface->ReadReg(CamID,ProductAdd1,ProductAdd1,&temp);
		m_pInterface->Device_ReadI2c(SlaveID,ProductAdd1,&temp,3,CamID);
		if(temp!=ProductID1)
		{
			CString Tempmsg;
			Tempmsg.Format(_T("Sensor版本号不对%d != %d"),temp,ProductID1);
			m_pInterface->Ctrl_Addlog(CamID,Tempmsg,COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Sensor版本号不对!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
   return 0;
}


/******************************************************************
º¯ÊýÃû:    Finalize
º¯Êý¹¦ÄÜ:  ÓÃÓÚInitializeÉêÃ÷µÄ±äÁ¿»Ö¸´Ô­Ê¼Öµ£¬ÉêÇëµÄÄÚ´æ¿Õ¼äÊÍ·ÅµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int Pre_Process_TestItem::Finalize()
{
	//TODO:ÔÚ´ËÌí¼Ó»ØÊÕ½áÊø´úÂë


	return 0;
}


/******************************************************************
º¯ÊýÃû:    LoadOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡²ÎÊýµ½±äÁ¿£¬ºÍ¸³Öµµ½¶Ô»°¿òµÄ°ó¶¨±äÁ¿ (ÎÄ¼þ´æ´¢Öµ->±äÁ¿¡ª>¿Ø¼þµÄÖµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int Pre_Process_TestItem::LoadOption()
{
	//TODO: ÔÚ´ËÌí¼Ó¶ÁÈ¡´úÂë 
	SetName(GlobalSetting);
	for (int i=0;i<20;i++)
	{
		CString temp;
		temp.Format(_T("FuseID_%d"),i+1);
		FuseID[i] = ReadConfigString(temp, _T(""));
	}
	pDlg->ProductID=ReadConfigString(_T("ProductID"), _T(""));
	pDlg->ProductID1=ReadConfigString(_T("ProductID1"), _T(""));
	pDlg->ProductAdd=ReadConfigString(_T("ProductAdd"), _T("0x0202"));
	pDlg->ProductAdd1=ReadConfigString(_T("ProductAdd1"), _T("0x0202"));
	pDlg->m_ProductEnable=ReadConfigInt(_T("ProductEnable"),1);
	pDlg->m_ProductEnable1=ReadConfigInt(_T("ProductEnable1"),1);
	pDlg->TestStation=ReadConfigString(_T("TestStation"), _T(""));
	pDlg->m_SlaveID=ReadConfigString(_T("ProductSlaveID"), _T("0x20"));
	pDlg->CalDiff=ReadConfigInt(_T("CalDiff"),12);
	ProductAdd=ReadConfigInt(_T("ProductAdd"),514);
	ProductID=ReadConfigInt(_T("ProductID"),12);
	ProductAdd1=ReadConfigInt(_T("ProductAdd1"),514);
	ProductID1=ReadConfigInt(_T("ProductID1"),12);
	SlaveID=ReadConfigInt(_T("ProductSlaveID"),32);
	CalTimeTag=ReadConfigInt(_T("CalTimeTag"),12);
	SetName(CurrentName);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
º¯ÊýÃû:    SaveOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ½«±äÁ¿ÊýÖµ»ò¶Ô»°¿òµÄ°ó¶¨±äÁ¿ÊýÖµ±£´æµ½ÅäÖÃÎÄ¼þÖÐ (¿Ø¼þµÄÖµ¡ª>±äÁ¿->ÎÄ¼þ´æ´¢Öµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int Pre_Process_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	SetName(GlobalSetting);
	//TODO: ÔÚ´ËÌí¼Ó´æ´¢´úÂë	   
	WriteConfigString(_T("ProductID"), pDlg->ProductID);
	WriteConfigString(_T("ProductAdd"), pDlg->ProductAdd);
	WriteConfigString(_T("ProductID1"), pDlg->ProductID1);
	WriteConfigString(_T("ProductAdd1"), pDlg->ProductAdd1);
	WriteConfigString(_T("TestStation"), pDlg->TestStation);
	WriteConfigString(_T("ProductSlaveID"), pDlg->m_SlaveID);
	WriteConfigInt(_T("ProductEnable"),pDlg->m_ProductEnable);
	WriteConfigInt(_T("ProductEnable1"),pDlg->m_ProductEnable1);
	WriteConfigInt(_T("CalDiff"),pDlg->CalDiff);
	SetName(CurrentName);


	//........................
	return 0;
}


/******************************************************************
º¯ÊýÃû:    GetReportHeaders
º¯Êý¹¦ÄÜ:  ±£´æÐèÒª±£´æµÄ²âÊÔÏîÄ¿Ãû×ÖÖÖÀà
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR Pre_Process_TestItem::GetReportHeads()
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
LPCTSTR Pre_Process_TestItem::GetReportLowLimit()
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
LPCTSTR Pre_Process_TestItem::GetReportHightLimit()
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
LPCTSTR Pre_Process_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

