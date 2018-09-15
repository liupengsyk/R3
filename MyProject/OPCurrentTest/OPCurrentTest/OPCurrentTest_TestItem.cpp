#include "StdAfx.h"
#include "OPCurrentTest_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new OPCurrentTest_TestItem(pInterface, lpszName);
}


/******************************************************************
º¯ÊýÃû:    ¹¹Ôìº¯Êý
º¯Êý¹¦ÄÜ:  ¶ÔÏóÉú³ÉµÄÊ±ºò³õÊ¼±ØÐëÍê³ÉµÄÅäÖÃ
*******************************************************************/
OPCurrentTest_TestItem::OPCurrentTest_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

	m_cVoltageAVDD = 0;
	m_cVoltageDOVDD = 0;
	m_cVoltageDVDD = 0;
	m_fPower = 0.0f;
	//.....
}

OPCurrentTest_TestItem::~OPCurrentTest_TestItem(void)
{

}


int OPCurrentTest_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //ÉèÖÃCONFIGÂ·¾¶	
	LoadOption(); 
	return 0;
}


/******************************************************************
º¯ÊýÃû:    Initialize
º¯Êý¹¦ÄÜ:  ÓÃÓÚ²âÊÔÇ°³õÊ¼»¯Ò»Ð©±äÁ¿£¬×´Ì¬£¬·ÖÅä¿Õ¼äµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int OPCurrentTest_TestItem::Initialize()
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
int OPCurrentTest_TestItem::Testing()
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
	//0: POWER_AVDD;
	//1: POWER_DOVDD;
	//2: POWER_DVDD;
	//3: POWER_AFVCC;
	//4: POWER_VPP;
	m_pInterface->Ctrl_PauseGrabFrame(CamID);
	Sleep(300);
   int OPCurrent[5]={0};
   /*fOPCurrent[5]={0.0};*/
   memset(fOPCurrent,0,sizeof(float)*5);
   m_pInterface->Ctrl_GetCamCurrent(OPCurrent,CamID);//获取当前电流

   for (int i=0;i<5;i++)
   {
	   fOPCurrent[i]=OPCurrent[i]/1000000.0;//获取到的值，每个都除以1000000
   }

   CString strTemp;
   if (fOPCurrent[0]>pDlg->m_iavmax||fOPCurrent[0]<pDlg->m_iavmin)//可能读取异常，给两次机会
   {
	   m_pInterface->Ctrl_GetCamCurrent(OPCurrent,CamID);
	   for (int i=0;i<5;i++)
	   {
		   fOPCurrent[i]=OPCurrent[i]/1000000.0;
	   }
   }


   m_fPower = (fOPCurrent[0] * m_cVoltageAVDD + fOPCurrent[1] * m_cVoltageDOVDD + fOPCurrent[2] * m_cVoltageDVDD) / 10.0f;
   CString strLog;
   strLog.Format(_T("工作AVDD电流值为 %f mA\n")
	   _T("工作DOVDD电流值为 %f mA\n")
	   _T("工作DVDD电流值为 %f mA\n")
	   _T("工作AF电流值为 %f mA\n")
	   _T("工作功耗值为 %f mw\n"),
	   fOPCurrent[0],
	   fOPCurrent[1],
	   fOPCurrent[2],
	   fOPCurrent[3],
	   m_fPower);
   m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);

   BOOL bCheckSucc = TRUE;

   if (fOPCurrent[0]>pDlg->m_iavmax||fOPCurrent[0]<pDlg->m_iavmin)//判断模拟电源
   {
	   strTemp.Format(_T("AVDD电流超范围:%f(%f,%f)"),fOPCurrent[0],pDlg->m_iavmin,pDlg->m_iavmax);
	   m_pInterface->Ctrl_Addlog(CamID,strTemp,COLOR_RED,200);
	   bCheckSucc = FALSE;
   }

   if (fOPCurrent[1]>pDlg->m_idomax||fOPCurrent[1]<pDlg->m_idomin)//数字电压
   {
	   strTemp.Format(_T("DOVDD电流超范围:%f(%f,%f)"),fOPCurrent[1],pDlg->m_idomin,pDlg->m_idomax);
	   m_pInterface->Ctrl_Addlog(CamID,strTemp,COLOR_RED,200);
	   bCheckSucc = FALSE;
   }
   if (fOPCurrent[2]>pDlg->m_idvmax||fOPCurrent[2]<pDlg->m_idvmin)
   {
	   strTemp.Format(_T("DVDD电流超范围:%f(%f,%f)"),fOPCurrent[2],pDlg->m_idvmin,pDlg->m_idvmax);
	   m_pInterface->Ctrl_Addlog(CamID,strTemp,COLOR_RED,200);
	   bCheckSucc = FALSE;
   }
   if (fOPCurrent[3]>pDlg->m_iafmax||fOPCurrent[3]<pDlg->m_iafmin)
   {
	   strTemp.Format(_T("AFVDD电流超范围:%f(%f,%f)"),fOPCurrent[3],pDlg->m_iafmin,pDlg->m_iafmax);
	   m_pInterface->Ctrl_Addlog(CamID,strTemp,COLOR_RED,200);
	   bCheckSucc = FALSE;
   }

   if (m_fPower>pDlg->m_iPowerMax||m_fPower <pDlg->m_iPowerMin)
   {
	   strTemp.Format(_T("功耗超范围:%f(%f,%f)"),m_fPower,pDlg->m_iPowerMin,pDlg->m_iPowerMax);
	   m_pInterface->Ctrl_Addlog(CamID,strTemp,COLOR_RED,200);
	   bCheckSucc = FALSE;
   }
   if (bCheckSucc)
   {
	   SetResult(RESULT_PASS);
   }
   else
   {
	   SetResult(RESULT_FAIL);
   }

   m_pInterface->Ctrl_ResumeGrabFrame(CamID);
   Sleep(300);
   return 0;
}


/******************************************************************
º¯ÊýÃû:    Finalize
º¯Êý¹¦ÄÜ:  ÓÃÓÚInitializeÉêÃ÷µÄ±äÁ¿»Ö¸´Ô­Ê¼Öµ£¬ÉêÇëµÄÄÚ´æ¿Õ¼äÊÍ·ÅµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int OPCurrentTest_TestItem::Finalize()
{
	//TODO:ÔÚ´ËÌí¼Ó»ØÊÕ½áÊø´úÂë


	return 0;
}


/******************************************************************
º¯ÊýÃû:    LoadOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡²ÎÊýµ½±äÁ¿£¬ºÍ¸³Öµµ½¶Ô»°¿òµÄ°ó¶¨±äÁ¿ (ÎÄ¼þ´æ´¢Öµ->±äÁ¿¡ª>¿Ø¼þµÄÖµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int OPCurrentTest_TestItem::LoadOption()
{
	//TODO: ÔÚ´ËÌí¼Ó¶ÁÈ¡´úÂë 

	CString strTemp;
	strTemp=ReadConfigString(_T("IavMax"),_T("50"));
	pDlg->m_iavmax=_wtof(strTemp);
	strTemp=ReadConfigString(_T("IavMin"),_T("0"));
	pDlg->m_iavmin=_wtof(strTemp);

	strTemp=ReadConfigString(_T("IdoMax"),_T("50"));
	pDlg->m_idomax=_wtof(strTemp);
	strTemp=ReadConfigString(_T("IdoMin"),_T("0"));
	pDlg->m_idomin=_wtof(strTemp);

	strTemp=ReadConfigString(_T("IdvMax"),_T("50"));
	pDlg->m_idvmax=_wtof(strTemp);
	strTemp=ReadConfigString(_T("IdvMin"),_T("0"));
	pDlg->m_idvmin=_wtof(strTemp);

	strTemp=ReadConfigString(_T("IafMax"),_T("50"));
	pDlg->m_iafmax=_wtof(strTemp);
	strTemp=ReadConfigString(_T("IafMin"),_T("0"));
	pDlg->m_iafmin=_wtof(strTemp);

	strTemp=ReadConfigString(_T("IpowerMax"),_T("50"));
	pDlg->m_iPowerMax=_wtof(strTemp);
	strTemp=ReadConfigString(_T("IpowerMin"),_T("0"));
	pDlg->m_iPowerMin=_wtof(strTemp);

	m_cVoltageAVDD = GetPrivateProfileInt(_T("Sensor"),_T("avdd"), 0,m_pInterface->Config_GetConfigPathName(CamID));
	m_cVoltageDOVDD = GetPrivateProfileInt(_T("Sensor"),_T("dovdd"), 0,m_pInterface->Config_GetConfigPathName(CamID));
	m_cVoltageDVDD = GetPrivateProfileInt(_T("Sensor"),_T("dvdd"), 0,m_pInterface->Config_GetConfigPathName(CamID));


	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
º¯ÊýÃû:    SaveOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ½«±äÁ¿ÊýÖµ»ò¶Ô»°¿òµÄ°ó¶¨±äÁ¿ÊýÖµ±£´æµ½ÅäÖÃÎÄ¼þÖÐ (¿Ø¼þµÄÖµ¡ª>±äÁ¿->ÎÄ¼þ´æ´¢Öµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int OPCurrentTest_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: ÔÚ´ËÌí¼Ó´æ´¢´úÂë	   

	CString strTemp;
	strTemp.Format(_T("%f"),pDlg->m_iavmax);
	WriteConfigString(_T("IavMax"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_iavmin);
	WriteConfigString(_T("IavMin"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_idomax);
	WriteConfigString(_T("IdoMax"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_idomin);
	WriteConfigString(_T("IdoMin"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_idvmax);
	WriteConfigString(_T("IdvMax"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_idvmin);
	WriteConfigString(_T("IdvMin"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_iafmax);
	WriteConfigString(_T("IafMax"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_iafmin);
	WriteConfigString(_T("IafMin"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_iPowerMax);
	WriteConfigString(_T("IpowerMax"),strTemp);

	strTemp.Format(_T("%f"),pDlg->m_iPowerMin);
	WriteConfigString(_T("IpowerMin"),strTemp);
	//........................
	return 0;
}


/******************************************************************
º¯ÊýÃû:    GetReportHeaders
º¯Êý¹¦ÄÜ:  ±£´æÐèÒª±£´æµÄ²âÊÔÏîÄ¿Ãû×ÖÖÖÀà
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR OPCurrentTest_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header+=_T("AVDD,DOVDD,DVDD,AFVCC,VPP,POWER");
	return header;
}


/******************************************************************
º¯ÊýÃû:    GetReportLowerLimits
º¯Êý¹¦ÄÜ:  ±£´æ²âÊÔ±ê×¼µÄÏÂÏÞÖµ
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR OPCurrentTest_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	CString temp;
	temp.Format(_T("%f,%f,%f,%f,,%f"),pDlg->m_iavmin,pDlg->m_idomin,pDlg->m_idvmin,pDlg->m_iafmin,pDlg->m_iPowerMin);
	LowerLimit+=temp;
	return LowerLimit;
}


/******************************************************************
º¯ÊýÃû:    GetReportUpperLimits
º¯Êý¹¦ÄÜ:  ±£´æ²âÊÔ±ê×¼µÄÉÏÏÞÖµ
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR OPCurrentTest_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	CString temp;
	temp.Format(_T("%f,%f,%f,%f,,%f"),pDlg->m_iavmax,pDlg->m_idomax,pDlg->m_idvmax,pDlg->m_iafmax,pDlg->m_iPowerMax);
	UpperLimit+=temp;
	return UpperLimit;
}


/******************************************************************
º¯ÊýÃû:    GetReportContents
º¯Êý¹¦ÄÜ:  ±£´æ¶ÔÓ¦²âÊÔÃû×ÖÖÖÀàµÄ²âÊÔµÄÊý¾Ý
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR OPCurrentTest_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	CString temp;
	temp.Format(_T("%f,%f,%f,%f,%f,%f"),fOPCurrent[0],fOPCurrent[1],fOPCurrent[2],fOPCurrent[3],fOPCurrent[4],m_fPower);
	Content+=temp;
	return Content;
}

