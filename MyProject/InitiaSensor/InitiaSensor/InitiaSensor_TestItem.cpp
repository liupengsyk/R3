#include "StdAfx.h"
#include "InitiaSensor_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new InitiaSensor_TestItem(pInterface, lpszName);
}


/******************************************************************
º¯ÊýÃû:    ¹¹Ôìº¯Êý
º¯Êý¹¦ÄÜ:  ¶ÔÏóÉú³ÉµÄÊ±ºò³õÊ¼±ØÐëÍê³ÉµÄÅäÖÃ
*******************************************************************/
InitiaSensor_TestItem::InitiaSensor_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

InitiaSensor_TestItem::~InitiaSensor_TestItem(void)
{

}


int InitiaSensor_TestItem::InitItem()
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
int InitiaSensor_TestItem::Initialize()
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
int InitiaSensor_TestItem::Testing()
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
	CString CurrentSetting=m_pInterface->Ctrl_GetCurrentPath()+ _T("SensorTab\\")+SettingPath;
	
	if (pDlg->m_Cam0)
		m_pInterface->Ctrl_PauseGrabFrame(0);
	if (pDlg->m_Cam1)
		m_pInterface->Ctrl_PauseGrabFrame(1);
	Sleep(800);
	if (pDlg->m_Cam0)
	{
		int _CamID = pDlg->m_isChangeSize;
		if (m_pInterface->InitiaSensor(/*0*/_CamID,SettingPath) == 0)
		{
			m_pInterface->Ctrl_ResumeGrabFrame(0);
			Sleep(1000);
			m_pInterface->Ctrl_Addlog(CamID,_T("Cam0 Initial sensor fail!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Cam0 Initial sensor fail!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
		
		m_pInterface->Ctrl_ResumeGrabFrame(0);
    }
	if (pDlg->m_Cam1)
	{
		int _CamID = 16 + pDlg->m_isChangeSize;
		if (m_pInterface->InitiaSensor(/*1*/_CamID,SettingPath1) == 0)
		{
			m_pInterface->Ctrl_ResumeGrabFrame(1);
			Sleep(1000);
			m_pInterface->Ctrl_Addlog(CamID,_T("Cam1 Initial sensor fail!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Cam1 Initial sensor fail!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
		
		m_pInterface->Ctrl_ResumeGrabFrame(1);
	}
	Sleep(200);
	m_pInterface->Ctrl_Addlog(CamID,_T("initial sensor pass!"),COLOR_BLUE,200);
	SetResult(RESULT_PASS);
    return 0;
}


/******************************************************************
º¯ÊýÃû:    Finalize
º¯Êý¹¦ÄÜ:  ÓÃÓÚInitializeÉêÃ÷µÄ±äÁ¿»Ö¸´Ô­Ê¼Öµ£¬ÉêÇëµÄÄÚ´æ¿Õ¼äÊÍ·ÅµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int InitiaSensor_TestItem::Finalize()
{
	//TODO:ÔÚ´ËÌí¼Ó»ØÊÕ½áÊø´úÂë
	return 0;
}


/******************************************************************
º¯ÊýÃû:    LoadOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡²ÎÊýµ½±äÁ¿£¬ºÍ¸³Öµµ½¶Ô»°¿òµÄ°ó¶¨±äÁ¿ (ÎÄ¼þ´æ´¢Öµ->±äÁ¿¡ª>¿Ø¼þµÄÖµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int InitiaSensor_TestItem::LoadOption()
{
	//TODO: ÔÚ´ËÌí¼Ó¶ÁÈ¡´úÂë 
	SettingPath=ReadConfigString(_T("SettingPath"),_T(""));
	pDlg->GetDlgItem(IDC_COMBO1)->SetWindowText(SettingPath);
	SettingPath1=ReadConfigString(_T("SettingPath1"),_T(""));
	pDlg->GetDlgItem(IDC_COMBO2)->SetWindowText(SettingPath1);

	pDlg->m_isChangeSize = ReadConfigInt(_T("isChangeSize"),0);

	pDlg->m_Cam0=ReadConfigInt(_T("m_Cam0"),1);
	pDlg->m_Cam1=ReadConfigInt(_T("m_Cam1"),0);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
º¯ÊýÃû:    SaveOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ½«±äÁ¿ÊýÖµ»ò¶Ô»°¿òµÄ°ó¶¨±äÁ¿ÊýÖµ±£´æµ½ÅäÖÃÎÄ¼þÖÐ (¿Ø¼þµÄÖµ¡ª>±äÁ¿->ÎÄ¼þ´æ´¢Öµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int InitiaSensor_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: ÔÚ´ËÌí¼Ó´æ´¢´úÂë	   
	pDlg->GetDlgItem(IDC_COMBO1)->GetWindowText(SettingPath);
	WriteConfigString(_T("SettingPath"),SettingPath);

	pDlg->GetDlgItem(IDC_COMBO2)->GetWindowText(SettingPath1);
	WriteConfigString(_T("SettingPath1"),SettingPath1);
	WriteConfigInt(_T("m_Cam0"),pDlg->m_Cam0);
	WriteConfigInt(_T("m_Cam1"),pDlg->m_Cam1);
	WriteConfigInt(_T("isChangeSize"),pDlg->m_isChangeSize);

	//........................
	return 0;
}


/******************************************************************
º¯ÊýÃû:    GetReportHeaders
º¯Êý¹¦ÄÜ:  ±£´æÐèÒª±£´æµÄ²âÊÔÏîÄ¿Ãû×ÖÖÖÀà
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR InitiaSensor_TestItem::GetReportHeads()
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
LPCTSTR InitiaSensor_TestItem::GetReportLowLimit()
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
LPCTSTR InitiaSensor_TestItem::GetReportHightLimit()
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
LPCTSTR InitiaSensor_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}
void InitiaSensor_TestItem::GetvecSensorlist()
{
	CString  SensorTabPath= m_pInterface->Ctrl_GetCurrentPath()+ _T("SensorTab\\");
	m_pInterface->GetDirAllFile(SensorTabPath,pDlg->vecSensorlist); 
}

