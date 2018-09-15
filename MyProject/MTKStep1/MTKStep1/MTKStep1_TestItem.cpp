#include "StdAfx.h"
#include "MTKStep1_TestItem.h"
#include "ImageProc.h"
#include <direct.h>

#if 0
#include "T_PDAF/MTK/2.0.1.1023/pd_cali_dll.h"
#pragma comment(lib,"T_PDAF/MTK/2.0.1.1023/pd_cali_dll.lib")
#endif

#include "T_PDAF/MTK/3.0.1.0322/pd_cali_dll.h"
#pragma comment(lib,"T_PDAF/MTK/3.0.1.0322/pd_cali_dll.lib")

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new MTKStep1_TestItem(pInterface, lpszName);
}


/******************************************************************
º¯ÊýÃû:    ¹¹Ôìº¯Êý
º¯Êý¹¦ÄÜ:  ¶ÔÏóÉú³ÉµÄÊ±ºò³õÊ¼±ØÐëÍê³ÉµÄÅäÖÃ
*******************************************************************/
MTKStep1_TestItem::MTKStep1_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

MTKStep1_TestItem::~MTKStep1_TestItem(void)
{

}


int MTKStep1_TestItem::InitItem()
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
int MTKStep1_TestItem::Initialize()
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
int MTKStep1_TestItem::Testing()
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
	CString filename=_T("");
	CString filepath=_T("");
	filepath=m_pInterface->Ctrl_GetCurrentPath()+_T("T_PDAF\\");
	
	filename.Format(_T("Site%d_CamID%d_"),m_pInterface->GetSiteNo(),CamID);
	filename=filename+_T("Step1_PDAF.raw");
	ImageInfo img=*m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("抓帧1失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	m_pInterface->Ctrl_SaveImage(CamID,1,filepath,filename,3);

	if (pDlg->MTKNDA)
	{
		CString SNnumber;
		CString Modulefilename=_T("");
		SNnumber=m_pInterface->Ctrl_GetCamCode();
		Modulefilename=filepath+SNnumber+_T("\\");
		CreateDirectory(Modulefilename,NULL);
		filename.Format(_T("Site%d_CamID%d_"),m_pInterface->GetSiteNo(),CamID);
		filename=filename+_T("5100k.raw.raw");
		m_pInterface->Ctrl_SaveImage(CamID,1,Modulefilename,filename,3);
		m_pInterface->Ctrl_Addlog(CamID,_T("5100K Raw图保存成功!"),RGB(0,255,0),200);	
	}

	int max_size=0;
	int width=0;
	int height=0;
	CString settingfilepath;
	settingfilepath.Format(_T("OV16885_v3_mirror_normal_flip_normal_mtk3.0.0_1.ini"));  //待修改
	settingfilepath=filepath+_T("MTK\\3.0.1.0322\\")+settingfilepath;
// 	char Myfilename[1024]={0};
// 	ImageProc::ConvertWideToChar(settingfilepath,Myfilename);
	USES_CONVERSION;
	char *settingfilepath_C = T2A(settingfilepath);
	int errornum =pd_cali_init(settingfilepath_C);
	if (errornum!=0)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Setting init failed!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Setting init failed!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	errornum=pd_cali_get_max_output_size(max_size);
	if (errornum!=0)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Get maxsize failed!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Get maxsize failed!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	errornum= pd_cali_get_raw_width_height(width,height);
	if (errornum!=0)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("pd_cali_get_raw_width_height运算出错!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("pd_cali_get_raw_width_height运算出错!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	int output_size1 = -1;
	char *output1 = NULL;
	output1=new char[max_size];
	if (NULL == output1)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("output1 buffer 申请失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("output1 buffer 申请失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	memset(output1,0,max_size); 
	errornum=pd_cali_proc1(img.Raw_buffer,output1,max_size,output_size1);
	if (errornum!=0)
	{
		SAFE_DELETE_ARRAY(output1);
		m_pInterface->Ctrl_Addlog(CamID,_T("PDAF Step1计算失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("PDAF Step1计算失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}	
	m_pInterface->Ctrl_Addlog(CamID,_T("PDAF Step1运算Pass!"),COLOR_BLUE,200);

	BYTE *PDAFoutput=NULL;
	PDAFoutput=new BYTE[output_size1];
	if (NULL == PDAFoutput)
	{
		SAFE_DELETE_ARRAY(output1);
		m_pInterface->Ctrl_Addlog(CamID,_T("PDAFoutput buffer 申请失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("PDAFoutput buffer 申请失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	memset(PDAFoutput,0,output_size1);
	BYTE Step11Eeprom[496]={0};
	vector <BYTE> Step1Data;
	Step1Data.clear();

	for (int k=0;k<output_size1;k++)
	{
		PDAFoutput[k]=output1[k];
		Step11Eeprom[k]=(BYTE)output1[k];
		Step1Data.push_back(PDAFoutput[k]);
	}
	m_pInterface->Ctrl_Addlog(CamID,_T("Verify开始"),COLOR_BLUE,200);

	SAFE_DELETE_ARRAY(output1);
	SAFE_DELETE_ARRAY(PDAFoutput);
	
	errornum =pd_cali_init(settingfilepath_C);
	if (errornum!=0)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Setting init failed!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Setting init failed!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	errornum=pd_cali_verify_proc1(Step11Eeprom);
	if (errornum!=0)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Verify 失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Verify 失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Verify PASS!"),COLOR_BLUE,200);
	}
	int Site=m_pInterface->GetSiteNo();
	CString FileName;
	FileName.Format(_T("MTKStep1_%d.bin"), Site);
	if(!m_pInterface->Otp_WriteToOtpBoard(CamID,FileName,Step1Data,1))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Bin文件生成失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Bin文件生成失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	SetResult(RESULT_PASS);
   return 0;
}


/******************************************************************
º¯ÊýÃû:    Finalize
º¯Êý¹¦ÄÜ:  ÓÃÓÚInitializeÉêÃ÷µÄ±äÁ¿»Ö¸´Ô­Ê¼Öµ£¬ÉêÇëµÄÄÚ´æ¿Õ¼äÊÍ·ÅµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int MTKStep1_TestItem::Finalize()
{
	//TODO:ÔÚ´ËÌí¼Ó»ØÊÕ½áÊø´úÂë


	return 0;
}


/******************************************************************
º¯ÊýÃû:    LoadOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡²ÎÊýµ½±äÁ¿£¬ºÍ¸³Öµµ½¶Ô»°¿òµÄ°ó¶¨±äÁ¿ (ÎÄ¼þ´æ´¢Öµ->±äÁ¿¡ª>¿Ø¼þµÄÖµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int MTKStep1_TestItem::LoadOption()
{
	//TODO: ÔÚ´ËÌí¼Ó¶ÁÈ¡´úÂë 



	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
º¯ÊýÃû:    SaveOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ½«±äÁ¿ÊýÖµ»ò¶Ô»°¿òµÄ°ó¶¨±äÁ¿ÊýÖµ±£´æµ½ÅäÖÃÎÄ¼þÖÐ (¿Ø¼þµÄÖµ¡ª>±äÁ¿->ÎÄ¼þ´æ´¢Öµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int MTKStep1_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: ÔÚ´ËÌí¼Ó´æ´¢´úÂë	   



	//........................
	return 0;
}


/******************************************************************
º¯ÊýÃû:    GetReportHeaders
º¯Êý¹¦ÄÜ:  ±£´æÐèÒª±£´æµÄ²âÊÔÏîÄ¿Ãû×ÖÖÖÀà
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR MTKStep1_TestItem::GetReportHeads()
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
LPCTSTR MTKStep1_TestItem::GetReportLowLimit()
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
LPCTSTR MTKStep1_TestItem::GetReportHightLimit()
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
LPCTSTR MTKStep1_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}
BOOL MTKStep1_TestItem::SaveStep1Bin(BYTE *GainBin)
{
	USES_CONVERSION;
	CString FilePath;
	FilePath.Format(_T("otpdataManager\\Cam%d\\"),CamID);
	FilePath=m_pInterface->Ctrl_GetCurrentPath()+FilePath;

	CString FileName;
	_mkdir(T2A(FilePath));
	FileName.Format(_T("MTKStep1_%d.bin"), CamID);
	FileName = FilePath + FileName;
	//删除旧文件
	if(PathFileExists(FileName))
	{	
		if (!DeleteFile(FileName))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("无法删除旧AWB文件!"),COLOR_RED,200);
			return FALSE;
		}
	}
	FILE *fp;
	fp=fopen(T2A(FileName),"wb");
	if (fp!=NULL)
	{
		int n=fwrite(GainBin,1,496,fp);
		fclose(fp);
		if(n!=496)
		{
			return FALSE;
		}
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("创建bin失败！"),COLOR_RED,200);
		return FALSE;
	}
	return TRUE;
}

