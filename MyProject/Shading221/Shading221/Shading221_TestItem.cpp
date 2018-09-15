#include "StdAfx.h"
#include "Shading221_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new Shading221_TestItem(pInterface, lpszName);
}


/******************************************************************
º¯ÊýÃû:    ¹¹Ôìº¯Êý
º¯Êý¹¦ÄÜ:  ¶ÔÏóÉú³ÉµÄÊ±ºò³õÊ¼±ØÐëÍê³ÉµÄÅäÖÃ
*******************************************************************/
Shading221_TestItem::Shading221_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

Shading221_TestItem::~Shading221_TestItem(void)
{

}


int Shading221_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //ÉèÖÃCONFIGÂ·¾¶	
	LoadOption(); 

	CString ConfigPath = m_pInterface->Config_GetConfigPathName(CamID);
	wchar_t section[1024] = {0};
	GetPrivateProfileString(_T("GENERAL_SETTING"),_T("sReportPath"),_T("0.0"),section, sizeof(section),ConfigPath);
	ReportPath.Format(section);
	ReportPath += _T("\\");
//	CreateDirectory(ReportPath,NULL);

	CString tempReport = ReportPath;

	int len = tempReport.GetLength();
	vector <CString> _StrVec;
	CString tempStr;
	int num;

	do 
	{
		num = tempReport.Find('\\');		
		tempStr = tempReport.Left(num+1);
		_StrVec.push_back(tempStr);
		tempReport = tempReport.Right(len-num-1);
		len = tempReport.GetLength();
	} while (len > 0);

	int size = _StrVec.size();
	tempReport = _T("");
	for (int i=0; i<size; i++)
	{
		tempReport += _StrVec.at(i);
		CreateDirectory(tempReport,NULL);
	}

	return 0;
}


/******************************************************************
º¯ÊýÃû:    Initialize
º¯Êý¹¦ÄÜ:  ÓÃÓÚ²âÊÔÇ°³õÊ¼»¯Ò»Ð©±äÁ¿£¬×´Ì¬£¬·ÖÅä¿Õ¼äµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int Shading221_TestItem::Initialize()
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
int Shading221_TestItem::Testing()
{
   //TODO:ÔÚ´ËÌí¼Ó²âÊÔÏî´úÂë
	SensorID = m_pInterface->Sensor_GetCamFuseID(CamID);
	if (Qualcomm_LSCCal() != TRUE)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("获取Shading数据NG!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("获取Shading数据NG!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	if (ColorShading_Test() != TRUE)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Color Shading Test fail!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Color Shading Test fail!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	if (Y_Shading_Test() != TRUE)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Y Shading Test fail!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Y Shading Test fail!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	m_pInterface->Ctrl_Addlog(CamID,_T("Shading Test Pass!"),COLOR_BLUE,200);
	SetResult(RESULT_PASS);
   return 0;
}


/******************************************************************
º¯ÊýÃû:    Finalize
º¯Êý¹¦ÄÜ:  ÓÃÓÚInitializeÉêÃ÷µÄ±äÁ¿»Ö¸´Ô­Ê¼Öµ£¬ÉêÇëµÄÄÚ´æ¿Õ¼äÊÍ·ÅµÈ
·µ»ØÖµ£º   0 - Íê³É   ·Ç0 - Î´Íê³É
*******************************************************************/
int Shading221_TestItem::Finalize()
{
	//TODO:ÔÚ´ËÌí¼Ó»ØÊÕ½áÊø´úÂë


	return 0;
}


/******************************************************************
º¯ÊýÃû:    LoadOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ´ÓÅäÖÃÎÄ¼þ¶ÁÈ¡²ÎÊýµ½±äÁ¿£¬ºÍ¸³Öµµ½¶Ô»°¿òµÄ°ó¶¨±äÁ¿ (ÎÄ¼þ´æ´¢Öµ->±äÁ¿¡ª>¿Ø¼þµÄÖµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int Shading221_TestItem::LoadOption()
{
	//TODO: ÔÚ´ËÌí¼Ó¶ÁÈ¡´úÂë 

//	pDlg->m_Cam0=ReadConfigInt(_T("m_Cam0"),1);
	pDlg->m_BLC = ReadConfigInt(_T("BLC"),16);
	pDlg->m_RGMAX = ReadConfigInt(_T("RGMAX"),100);
	pDlg->m_RGMin = ReadConfigInt(_T("RGMIN"),100);
	pDlg->m_BGMAX = ReadConfigInt(_T("BGMAX"),100);
	pDlg->m_BGMin = ReadConfigInt(_T("BGMIN"),100);
	pDlg->m_YShading_Diff = ReadConfigInt(_T("YShading_Diff"),0);
	pDlg->m_YShading_Max  = ReadConfigInt(_T("YShading_Max"),100);
	pDlg->m_YShading_Min  = ReadConfigInt(_T("YShading_Min"),0);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
º¯ÊýÃû:    SaveOption
º¯Êý¹¦ÄÜ:  ÓÃÓÚ½«±äÁ¿ÊýÖµ»ò¶Ô»°¿òµÄ°ó¶¨±äÁ¿ÊýÖµ±£´æµ½ÅäÖÃÎÄ¼þÖÐ (¿Ø¼þµÄÖµ¡ª>±äÁ¿->ÎÄ¼þ´æ´¢Öµ)
·µ»ØÖµ£º   0 - ³É¹¦   ·Ç0 - ²»³É¹¦
*******************************************************************/
int Shading221_TestItem::SaveOption()
{
//	WriteConfigInt(_T("m_Cam0"),pDlg->m_Cam0);

	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: ÔÚ´ËÌí¼Ó´æ´¢´úÂë	   

	WriteConfigInt(_T("BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("RGMAX"),pDlg->m_RGMAX);
	WriteConfigInt(_T("RGMIN"),pDlg->m_RGMin);
	WriteConfigInt(_T("BGMAX"),pDlg->m_BGMAX);
	WriteConfigInt(_T("BGMIN"),pDlg->m_BGMin);
	WriteConfigInt(_T("YShading_Diff"),pDlg->m_YShading_Diff);
	WriteConfigInt(_T("YShading_Max"),pDlg->m_YShading_Max);
	WriteConfigInt(_T("YShading_Min"),pDlg->m_YShading_Min);

	//........................
	return 0;
}


/******************************************************************
º¯ÊýÃû:    GetReportHeaders
º¯Êý¹¦ÄÜ:  ±£´æÐèÒª±£´æµÄ²âÊÔÏîÄ¿Ãû×ÖÖÖÀà
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR Shading221_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header += _T("RGRef_Max,RGRef_Min,BGRef_Max,BGRef_Min,Y_Shading_Block[0],Y_Shading_Block[1],Y_Shading_Block[2],Y_Shading_Block[3],Y_Shading_Diff");
	return header;
}


/******************************************************************
º¯ÊýÃû:    GetReportLowerLimits
º¯Êý¹¦ÄÜ:  ±£´æ²âÊÔ±ê×¼µÄÏÂÏÞÖµ
·µ»ØÖµ£º   ×Ö·û´®Ö¸Õë
*******************************************************************/
LPCTSTR Shading221_TestItem::GetReportLowLimit()
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
LPCTSTR Shading221_TestItem::GetReportHightLimit()
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
LPCTSTR Shading221_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");

	CString temp;
	temp.Format(_T("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f"),RGRef_Max,RGRef_Min,BGRef_Max,BGRef_Min,Y_Shading_Corner[0],Y_Shading_Corner[1],Y_Shading_Corner[2],Y_Shading_Corner[3],Y_Shading_Diff);
	Content += temp;

	return Content;
}

void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}

BOOL Shading221_TestItem::Qualcomm_LSCCal(void)
{
	ImageInfo *MyImageInfo;
	MyImageInfo=m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == MyImageInfo->Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		return FALSE;
	}
	int width=MyImageInfo->imgwidth;
	int height=MyImageInfo->imgheight;

	LPBYTE MyRaw8Buff=new BYTE[width*height];
	if (NULL == MyRaw8Buff)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("MyRaw8Buff buffer 申请失败!"),COLOR_RED,200);
		return FALSE;
	}
	memset(MyRaw8Buff,0,width*height);
	Raw10toRaw8(MyImageInfo->Raw_buffer,MyRaw8Buff,width*height);
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7);

	HINSTANCE hDll;   //DLL句柄 
	CString m_szDirectory;
	m_szDirectory=m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\");
	SetCurrentDirectory(m_szDirectory);
	m_szDirectory=m_szDirectory+_T("Qualcomm\\");
	SetCurrentDirectory(m_szDirectory);
	CString DllPath;
	DllPath=m_szDirectory+_T("LSCCalibrationDll.dll");
	hDll = LoadLibrary(DllPath);
	if (NULL==hDll)
	{
		SAFE_DELETE_ARRAY(MyRaw8Buff);
		FreeLibrary(hDll);
		m_pInterface->Ctrl_Addlog(CamID,_T("LSCCalibrationDll.dll丢失,请将文件放回程式目录!"),COLOR_RED,200);
		return FALSE;
	}

	lpFun LensCorrection = (lpFun)GetProcAddress(hDll,"LensCorrectionLib"); 
	if (NULL==LensCorrection)
	{
		SAFE_DELETE_ARRAY(MyRaw8Buff);
		FreeLibrary(hDll);
		m_pInterface->Ctrl_Addlog(CamID,_T("DLL中函数寻找失败!"),COLOR_RED,200);
		return FALSE;
	}
	int CFA_Pattern ;

	if ((MyImageInfo->RawFormat == 0) ||
		(MyImageInfo->RawFormat == 1))
	{
		CFA_Pattern =MyImageInfo->RawFormat;
	}
	if (MyImageInfo->RawFormat == 2)
	{
		CFA_Pattern = 3;
	}
	if (MyImageInfo->RawFormat == 3)
	{
		CFA_Pattern = 2;
	}
	int flag = LensCorrection(MyRaw8Buff,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,0);
	FreeLibrary(hDll);
	SAFE_DELETE_ARRAY(MyRaw8Buff);
	return TRUE;
}


BOOL Shading221_TestItem::ReadBinData(USHORT * LSC_Ratio)
{
	HANDLE hFile;
	int Timeout=100;
	CString eeprom_slim;

	eeprom_slim=m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\Qualcomm\\ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt");

	while (Timeout!=0)
	{
		hFile = CreateFile(eeprom_slim,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
		if( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD nBytesRead;
			unsigned char BIN[2000] = {0};
			ReadFile(hFile, BIN, 2000, &nBytesRead, NULL );
			CloseHandle( hFile );

			if (nBytesRead != 1768)
			{
				CString ErrorMsg;
				ErrorMsg.Format(_T("eeprom文件大小不为%d Byte!"),1768);
				m_pInterface->Ctrl_Addlog(CamID,ErrorMsg,COLOR_RED,200);
				return FALSE;
			}
			for (int i = 0;i < 1768; i++)
			{
				LSC_Ratio[i]=BIN[i];
			}
			break;
		}
		Timeout--;
	}
	return TRUE;
}


BOOL Shading221_TestItem::ColorShading_Test()
{
	USHORT LSC_Ratio[2000];
	memset(LSC_Ratio,0,2000*sizeof(USHORT));
	if (ReadBinData(LSC_Ratio) != TRUE)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("获取LSC\\Qualcomm\\ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt失败!"),COLOR_RED,200);
		return FALSE;
	}	

	CString TempMsg = _T("");
	int tempBuf[17*13][4] = {0};
	int cnt= 0;

	for( int i = 0; i < 221; i++ )
	{
		tempBuf[i][0] = LSC_Ratio[i*2]+(LSC_Ratio[i*2+1]<<8) ;
		tempBuf[i][1] = LSC_Ratio[i*2+442]+(LSC_Ratio[i*2+1+442]<<8) ;
		tempBuf[i][2] = LSC_Ratio[i*2+884]+(LSC_Ratio[i*2+1+884]<<8) ;
		tempBuf[i][3] = LSC_Ratio[i*2+1326]+(LSC_Ratio[i*2+1+1326]<<8) ;
	}

	float CenterRG=(float)tempBuf[111][0]*2/(tempBuf[111][1]+tempBuf[111][2]);
	float CenterBG=(float)tempBuf[111][3]*2/(tempBuf[111][1]+tempBuf[111][2]);

	float tempRG[221]={0};
	float tempBG[221]={0};
	for(int i = 0; i < 17*13; i++ )
	{			
		tempRG[i]=(float)tempBuf[i][0]*2/(float)(tempBuf[i][1]+tempBuf[i][2])/CenterRG * 100.0;
		tempBG[i]=(float)tempBuf[i][3]*2/(float)(tempBuf[i][1]+tempBuf[i][2])/CenterBG * 100.0;
	}

	RGRef_Max = 0.0;
	RGRef_Min  = 10000.0;
	BGRef_Max = 0.0;
	BGRef_Min  = 10000.0;

	for (int i=0; i<221; i++)
	{
		if (tempRG[i] > RGRef_Max)  RGRef_Max = tempRG[i];
		if (tempRG[i] < RGRef_Min)  RGRef_Min = tempRG[i];
		if (tempBG[i] > BGRef_Max)  BGRef_Max = tempRG[i];
		if (tempBG[i] < BGRef_Min)  BGRef_Min = tempRG[i];
	}

	BOOL TestResult = TRUE;
	for(int  i = 0; i < 221; i++ )
	{
		if (tempRG[i]>pDlg->m_RGMAX||tempRG[i]<pDlg->m_RGMin)
		{
			TempMsg.Format(_T("第%d Block RG超规格!"),i);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TempMsg.Format(_T("RGref为%0.3f "),tempRG[i]);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TestResult = FALSE;
			break;
		}

		if (tempBG[i]>pDlg->m_BGMAX||tempBG[i]<pDlg->m_BGMin)
		{
			TempMsg.Format(_T("第%d Block BG超规格!"),i);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TempMsg.Format(_T("BGref为%0.3f "),tempBG[i]);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TestResult = FALSE;
			break;
		}
	}

	SaveColorShadingData(tempRG,tempBG,TestResult);
	return TestResult;
}


void Shading221_TestItem::SaveColorShadingData(float * Color_RG , float * Color_BG , BOOL Result)
{
	USES_CONVERSION;

	int i = 0;
	FILE  *fp  = NULL;
	CTime time = CTime::GetCurrentTime();

	CString RecordTime = _T("");

	CFile MyFile;
	long fileSizeTemp=0;

	CString FileName;
	FileName.Format(_T("ColorShading_Test_Site_A%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay()); ;
	FileName = ReportPath + FileName;

	m_pInterface->Ctrl_Addlog(CamID,FileName,COLOR_BLUE,200);

	fp=freopen(CT2A(FileName),"at",stdout);
	if(fp==NULL)  
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Color shading文件打开失败，数据保存失败!"),COLOR_RED,200);
		return;
	}

	fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);
	if(fileSizeTemp==0)
	{
		printf("FUSE ID\tTest Time\t");
		for (i = 0; i < 221; i++)
		{
			printf( "%s%s%d\t", "RG_","Block", i);
		}	
		for (i = 0; i < 221; i++)
		{
			printf( "%s%s%d\t", "BG_","Block", i);
		}
		printf("Result\t");
	}

	printf("\n");
	printf(CT2A(SensorID));
	printf("#\t");

	printf(CT2A(RecordTime));
	printf("\t");

	for (i = 0; i < 221; i++)
	{
		printf( "%.4f\t",Color_RG[i]);
	}	
	for (i = 0; i < 221; i++)
	{
		printf( "%.4f\t",Color_BG[i]);
	}	
	if (Result)
	{
		printf("%s", "PASS");
	}
	else printf("%s", "NG");
	printf("\t");
	fclose(fp); 
}


BOOL Shading221_TestItem::Y_Shading_Test(void)
{
	CString TempMsg = _T("");
	int i=0;
	float R_Channel[221] = {0};
	float Gr_Channel[221] = {0};
	float Gb_channel[221] = {0};
	float B_Channel[221] = {0};
	if (ReadDataFromTXT(R_Channel,Gr_Channel,Gb_channel,B_Channel) != TRUE)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("获取YShading数据失败!"),COLOR_RED,200);
		return FALSE;
	}
	
	float Y_Data[221] = {0.0};
	float Y_Shading[221] = {0};

	for (i=0; i<221; i++)
	{
		Y_Data[i] = ((9798)*float(R_Channel[i]) + (19235)*float((Gr_Channel[i]+Gb_channel[i])/2.0) + (3735)*float(B_Channel[i]))/32768 ;
	}

	for (i=0; i<221; i++)
	{
		Y_Shading[i] = Y_Data[i]/Y_Data[110]*100;
	}

	BOOL TestResult = TRUE; 

	for(int  i = 0; i < 221; i++ )
	{
		if (Y_Shading[i]<pDlg->m_YShading_Min)
		{
			TempMsg.Format(_T("第%d Block YShading超规格!"),i);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TempMsg.Format(_T("YShading为%0.3f"),Y_Shading[i]);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TestResult = FALSE;
			break;
		}
	}

	Y_Shading_Corner[0] = Y_Shading[0];
	Y_Shading_Corner[1] = Y_Shading[16];
	Y_Shading_Corner[2] = Y_Shading[204];
	Y_Shading_Corner[3] = Y_Shading[220];

	for (i=0; i<4; i++)
	{
		if (Y_Shading_Corner[i] > pDlg->m_YShading_Max ||
			 Y_Shading_Corner[i] < pDlg->m_YShading_Min)
		{
			TempMsg.Format(_T("Corner YShading Block[%d]超规格!"),i);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TempMsg.Format(_T("Corner YShading[%d]为%0.3f"),i,Y_Shading_Corner[i]);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			TestResult = FALSE;
			break;
		}
	}

	float Y_Shading_Max=0.0;
	float y_Shading_Min = 1000.0;
	for (i=0; i<4; i++)
	{
		if (Y_Shading_Corner[i] > Y_Shading_Max)
		{
			Y_Shading_Max = Y_Shading_Corner[i];
		}
		if (Y_Shading_Corner[i] < y_Shading_Min)
		{
			y_Shading_Min = Y_Shading_Corner[i];
		}
	}

	TempMsg.Format(_T("Y Shading Max = %.3f, Y Shading Min = %.3f"),Y_Shading_Max,y_Shading_Min);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	Y_Shading_Diff = Y_Shading_Max - y_Shading_Min;
	TempMsg.Format(_T("Y Shading Diff = %.3f, Spec = %d"),Y_Shading_Diff,pDlg->m_YShading_Diff);

	if (Y_Shading_Diff > pDlg->m_YShading_Diff)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Y Shading Diff NG!"),COLOR_RED,200);
		TestResult = FALSE;
	}

	SaveYShadingData(Y_Shading,TestResult);

	return TestResult;
}


BOOL Shading221_TestItem::ReadDataFromTXT(float * R_Channel, float * Gr_Channel, float * Gb_Channel, float * B_Channel)
{
	USES_CONVERSION;
	int mycharlength = 0;
	int strlength = 0;
	char mycharbuff[512] = {0};
	CStdioFile file;
	CString Path;
	Path =m_pInterface->Ctrl_GetCurrentPath()+_T("LSC\\Qualcomm\\AWB_LSC_CALIBRATION_DATA.txt");
	int r_count =0;
	int gr_count = 0;
	int gb_count = 0;
	int b_count = 0;
	int channel_count = 0;
	int end_count =0;
	float tempnum;
	int i;
	int q = 0;
	if(file.Open(Path , CFile::modeRead | CFile::typeText ))
	{
		CString strTemp;
		CString a;
		while(file.ReadString(strTemp))
		{
			if (strTemp.Find(_T("}"),0) != -1)
			{
				end_count++;
				continue;
			}
			if (end_count >= 4)
			{
				break;
			}
			if ( strTemp.Find( _T("{"), 0 ) != -1 )
			{	
				channel_count++;
				if ( channel_count == 1 )
				{
					strTemp = strTemp.Mid(38,strTemp.GetLength()-38);
				}
				if ( (channel_count == 2) || ( channel_count == 3 ) )
				{
					strTemp = strTemp.Mid(14,strTemp.GetLength()-14);
				}
				if ( channel_count == 4 )
				{
					strTemp = strTemp.Mid(13,strTemp.GetLength()-13);
				}

				for (i = 0;i < 17; i++)
				{
					switch (channel_count)
					{
					case 1:
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum =atof(T2A(a));;
						mycharlength = 0;
						R_Channel[r_count++] = tempnum;
						break;
					case 2:
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = atof(T2A(a));
						mycharlength = 0;
						Gr_Channel[gr_count++] = tempnum;
						break;
					case 3:
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = atof(T2A(a));
						mycharlength = 0;
						Gb_Channel[gb_count++] = tempnum;
						break;
					case 4:
						q = 0;
						while ( (strTemp[q] != ' ')&&(q<13) )
						{
							mycharlength++;
							q++;
						}
						a = strTemp.Mid(0,mycharlength);
						strlength = strTemp.GetLength();
						if ( (strlength-mycharlength) != 1 )
						{
							strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
						}
						else
						{
							a = strTemp.Mid(0,strlength-1);
						}
						tempnum = atof(T2A(a));
						mycharlength = 0;
						B_Channel[b_count++] = tempnum;
						break;
					}
				}
			}
			else
			{
				strTemp = strTemp.Mid(1,strTemp.GetLength()-1);
				for (i = 0;i < 17; i++)
				{
					int k = 0;
					while ( (strTemp[k] != ' ')&&(k<13) )
					{
						mycharlength++;
						k++;
					}
					a = strTemp.Mid(0,mycharlength);
					strlength = strTemp.GetLength();
					if ( (strlength-mycharlength) != 1 )
					{
						strTemp = strTemp.Mid(mycharlength+2,strlength-(mycharlength+2));
					}
					else
					{
						a = strTemp.Mid(0,strlength-1);
					}


					float tempnum = atof(T2A(a));
					mycharlength = 0;
					switch (channel_count)
					{
					case 1:
						R_Channel[r_count++] = tempnum;
						break;
					case 2:
						Gr_Channel[gr_count++] = tempnum;
						break;
					case 3:
						Gb_Channel[gb_count++] = tempnum;
						break;
					case 4:
						B_Channel[b_count++] = tempnum;
						break;
					}
				}
			}
		}
		file.Close();
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("LSC\\Qualcomm\\AWB_LSC_CALIBRATION_DATA.txt打开失败!"),COLOR_RED,200);
		return FALSE;
    }
	return TRUE;
}


void Shading221_TestItem::SaveYShadingData(float * Y_Shading_Data,BOOL Result)
{
	USES_CONVERSION;

	int i = 0;
	FILE  *fp  = NULL;
	CTime time = CTime::GetCurrentTime();

	CString RecordTime = _T("");

	CFile MyFile;
	long fileSizeTemp=0;

	CString FileName;
	FileName.Format(_T("Y_Shading_Test_Site_A%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay()); ;
	FileName = ReportPath + FileName;

	m_pInterface->Ctrl_Addlog(CamID,FileName,COLOR_BLUE,200);

	fp=freopen(CT2A(FileName),"at",stdout);
	if(fp==NULL)  
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Y shading文件打开失败，数据保存失败!"),COLOR_RED,200);
		return;
	}

	fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);
	if(fileSizeTemp==0)
	{
		printf("FUSE ID\tTest Time\t");
		for (i = 0; i < 221; i++)
		{
			printf( "%s%s%d\t", "Y_Shading_","Block", i);
		}	
		printf("Result\t");
	}

	printf("\n");
	printf(CT2A(SensorID));
	printf("#\t");

	printf(CT2A(RecordTime));
	printf("\t");

	for (i = 0; i < 221; i++)
	{
		printf( "%.4f\t",Y_Shading_Data[i]);
	}	

	if (Result)
	{
		printf("%s", "PASS");
	}
	else printf("%s", "NG");
	printf("\t");
	fclose(fp); 
}
