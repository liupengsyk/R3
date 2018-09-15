#include "StdAfx.h"
#include "OtpCheck_API.h"


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */

// #define TESTSTATION _T("OJLJP15")
// #define STATIONCHECKITEM _T("OJLOJP15TPCheck")
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{	if(p) { delete [] (p);					(p)=NULL; }	}
#endif

CString OtpCheckAPI::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	return ret;
}



BOOL RegReadKey(wchar_t *SubReg,wchar_t *SubKeyName,wchar_t *keyValue)
{
	HKEY hKey=0;
	wchar_t *SubRegName=new wchar_t[512];
	LPBYTE lpData= NULL;
	DWORD dwBytes=0;
	wcscpy(SubRegName,_T("Software\\Ofilm\\"));
	if (wcslen(SubReg))
	{
		wcscat(SubRegName,SubReg);
	}

	::RegOpenKey(HKEY_CURRENT_USER,SubRegName,&hKey);
	if (hKey==0)
	{
		free(SubRegName);
		return false;
	}
	//	 ::RegQueryValueEx(hKey,SubRegName,NULL,NULL,NULL,&dwBytes);
	::RegQueryValueEx(hKey,SubKeyName,NULL,NULL,NULL,&dwBytes);
	if (dwBytes>0)
	{
		lpData = new BYTE[dwBytes];
	}
	//	 ::RegQueryValueEx(hKey,SubRegName,NULL,NULL,lpData,&dwBytes);
	::RegQueryValueEx(hKey,SubKeyName,NULL,NULL,lpData,&dwBytes);
	::RegCloseKey(hKey);
	if (lpData==NULL)
	{
		wcscpy(keyValue,_T(""));
	}
	else
	{
		wcscpy(keyValue,(wchar_t *)lpData);
	}
	free(SubRegName);
	return true;
}

BOOL RegSetKey(wchar_t *SubReg,wchar_t *SubKeyName,wchar_t *keyValue)
{
	HKEY hKey=0;
	wchar_t *SubRegName=new wchar_t[512];
	wcscpy(SubRegName,_T("Software\\Ofilm\\"));
	if (wcslen(SubReg))
	{
		wcscat(SubRegName,SubReg);
	}
	::RegOpenKey(HKEY_CURRENT_USER,SubRegName,&hKey);
	if (hKey==0)
	{
		::RegCreateKey(HKEY_CURRENT_USER,SubRegName,&hKey);
	}

	::RegSetValueEx(hKey,SubKeyName,0,REG_SZ,(CONST BYTE*)keyValue,512);
	::RegCloseKey(hKey);
	free(SubRegName);
	return true;
}

int GetCRC16(BYTE *Data, int Len)
{
	int CRC[16] = {0};
	int DoInvert = 0,i=0,j=0;
	int mycrc16x = 0;
	for (i=0; i<Len; i++)
	{
		int hexonly = Data[i];
		for (j=7; j>=0; j--)
		{
			DoInvert = hexonly & ((int)pow(2.0,j));
			DoInvert = DoInvert / (pow(2.0,j));
			DoInvert = DoInvert ^ CRC[15];

			CRC[15] = CRC[14] ^ DoInvert;
			CRC[14] = CRC[13];
			CRC[13] = CRC[12];
			CRC[12] = CRC[11];
			CRC[11] = CRC[10];
			CRC[10] = CRC[9];
			CRC[9] = CRC[8];
			CRC[8] = CRC[7];
			CRC[7] = CRC[6];
			CRC[6] = CRC[5];
			CRC[5] = CRC[4];
			CRC[4] = CRC[3];
			CRC[3] = CRC[2];
			CRC[2] = CRC[1] ^ DoInvert;
			CRC[1] = CRC[0];
			CRC[0] = DoInvert;
		}
	}

	for (i=0; i<16; i++)
	{
		mycrc16x = mycrc16x + (CRC[i]*pow(2.0,i));
	}
	return mycrc16x;
}

int OtpCheckAPI::Testing( wchar_t *ErrorMsg,BYTE *InputOTPData,int iInputOtpSize, int Flag,CString StationName)
{
	if (iInputOtpSize <= 0)
	{
		Catstring(ErrorMsg, _T("输入buffer size小于等于0!"), iInputOtpSize);
		return -1;
	}
	m_OTPSize = iInputOtpSize;

	//CString TESTSTATION = _T("");
	CString strFilePath = GetModulePath()+ _T("\\SetOTPCheckSpec.ini");
	if (Flag == 1)
	{
		TESTSTATION.Format(MASTER_MODULE_NAME);
		STATIONCHECKITEM.Format(MASTER_MODULE_STATION_CHK_ITEM_NAME); 

		OnInitialSpec_Project1( strFilePath, m_GetOTPCheckSpec);
		if (m_GetOTPCheckSpec.m_ProjectName1.Find(TESTSTATION) == -1)
		{
			TCHAR szLog[512] = {0};

			wsprintf(szLog, _T("\n 请调用对应机种的OTPcheck.dll!\n 仅适用于%s机种!"), TESTSTATION);
			Catstring(ErrorMsg, szLog);
			return -2;
		}

		//主摄里有副摄数据，所以也要检查副摄数据
		//TESTSTATION_1.Format(SLAVER_MODULE_NAME);
		//STATIONCHECKITEM_1.Format(SLAVER_MODULE_STATION_CHK_ITEM_NAME); 

		//OnInitialSpec_Project2( strFilePath, m_GetOTPCheckSpec1);
		//if (m_GetOTPCheckSpec1.m_ProjectName1.Find(TESTSTATION_1) == -1)
		//{
		//	TCHAR szLog[512] = {0};

		//	wsprintf(szLog, _T("\n 请调用对应机种的OTPcheck.dll!\n 仅适用于%s机种!"), TESTSTATION_1);
		//	Catstring(ErrorMsg, szLog);
		//	return -2;
		//}
	}
	else
	{
		TESTSTATION_1.Format(SLAVER_MODULE_NAME);
		STATIONCHECKITEM_1.Format(SLAVER_MODULE_STATION_CHK_ITEM_NAME); 

		OnInitialSpec_Project2( strFilePath, m_GetOTPCheckSpec1);
		if (m_GetOTPCheckSpec1.m_ProjectName1.Find(TESTSTATION_1) == -1)
		{
			TCHAR szLog[512] = {0};

			wsprintf(szLog, _T("\n 请调用对应机种的OTPcheck.dll!\n 仅适用于%s机种!"), TESTSTATION_1);
			Catstring(ErrorMsg, szLog);
			return -2;
		}
	}

    return OTPcheckImp(ErrorMsg,InputOTPData, Flag,StationName);
}


void OtpCheckAPI::Catstring(wchar_t *Msg, const wchar_t *format, ...)
{
	wchar_t buf[1024] = {0};
	va_list ap;
	va_start(ap, format);
	int len=vswprintf(buf,format,ap);
	buf[len]=0;
	va_end(ap);
	CString data;
	data.Format(buf);
	wcscat(Msg,buf);

	return;
}

BOOL OtpCheckAPI::OnInitialSpec_Project1(const CString &strFilePath,GetTestSpec &m_GetOTPCheckSpec)
{
	
	CString strProjectName1 = _T("Project1_OTPCheck");
	wchar_t szTmp[2048];
	
	int iBufSize = 2048;

	memset(szTmp, 0, sizeof(szTmp));
	 GetPrivateProfileString(strProjectName1,_T("ProjectName1"), _T("XXXX"), szTmp, iBufSize, strFilePath);	  //获取机种名
	m_GetOTPCheckSpec.m_ProjectName1 = szTmp;

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1,_T("TestStation"), _T("终检"), szTmp, iBufSize, strFilePath);	  //获取机种名
	m_GetOTPCheckSpec.m_TestStation = szTmp;

	int nPos = 0;
	nPos = (m_GetOTPCheckSpec.m_TestStation).Find('/');
	m_GetOTPCheckSpec.m_TestStation = (nPos < 0)?m_GetOTPCheckSpec.m_TestStation:m_GetOTPCheckSpec.m_TestStation.Left(nPos-1);

	m_GetOTPCheckSpec.m_Project1Check_En	=	GetPrivateProfileInt(strProjectName1,_T("Project1Check_En"),	0, strFilePath);	
	m_GetOTPCheckSpec.m_OTPCheckAll_En		=	GetPrivateProfileInt(strProjectName1,_T("OTPCheckAll_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_ModuleInfoCheck_En	=	GetPrivateProfileInt(strProjectName1,_T("ModuleInfoCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec.m_OCCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("OCCheck_En"),			0, strFilePath);
	m_GetOTPCheckSpec.m_AWBLSCCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("AWBLSCCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_AFCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("AFCheck_En"),			0, strFilePath);
	m_GetOTPCheckSpec.m_SFRCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("SFRCheck_En"),			0, strFilePath);
	m_GetOTPCheckSpec.m_SPCCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("SPCCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_PDAFCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("PDAFCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_DualCamCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("DualCamCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec.m_AECCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("AECCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_CrossTalkCheck_En	=	GetPrivateProfileInt(strProjectName1,_T("CrossTalkCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec.m_ArcsoftCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("ArcsoftCheck_En"),	0, strFilePath);							 
	m_GetOTPCheckSpec.m_LRCCheck_En			=   GetPrivateProfileInt(strProjectName1,_T("LRCCheck_En"),        0, strFilePath);											 
	m_GetOTPCheckSpec.m_OffLineCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("OffLineCheck_En"),	0, strFilePath);
															  
	m_GetOTPCheckSpec.m_SaveOTPCheckData	=	GetPrivateProfileInt(strProjectName1,_T("SaveOTPCheckData"),	1, strFilePath);
	
	m_GetOTPCheckSpec.m_EqualCountSpec		=	GetPrivateProfileInt(strProjectName1,_T("EqualCountSpec"),		0, strFilePath);
	m_GetOTPCheckSpec.m_HisiMasterFlag		=	GetPrivateProfileInt(strProjectName1,_T("HisiMasterFlag"),		0, strFilePath);
	
	// BasicInfo
	m_GetOTPCheckSpec.m_ModulInfoFlag		=	GetPrivateProfileInt(strProjectName1,_T("ModulInfoFlag"),		0, strFilePath);
	m_GetOTPCheckSpec.m_EepromTableRevision	=	GetPrivateProfileInt(strProjectName1,_T("EepromTableRevision"),	0, strFilePath);
	m_GetOTPCheckSpec.m_CalHwVersion		=	GetPrivateProfileInt(strProjectName1,_T("CalHwVersion"),		0, strFilePath);
	m_GetOTPCheckSpec.m_CalSwVersion		=	GetPrivateProfileInt(strProjectName1,_T("CalSwVersion"),		0, strFilePath);
	m_GetOTPCheckSpec.m_PartNumber1			=	GetPrivateProfileInt(strProjectName1,_T("MotorolaPartNumber1"),	0, strFilePath);
	m_GetOTPCheckSpec.m_PartNumber2			=	GetPrivateProfileInt(strProjectName1,_T("MotorolaPartNumber2"),	0, strFilePath);
	m_GetOTPCheckSpec.m_PartNumber3			=	GetPrivateProfileInt(strProjectName1,_T("MotorolaPartNumber3"),	0, strFilePath);
	m_GetOTPCheckSpec.m_PartNumber4			=	GetPrivateProfileInt(strProjectName1,_T("MotorolaPartNumber4"),	0, strFilePath);
	m_GetOTPCheckSpec.m_ActuatorID			=	GetPrivateProfileInt(strProjectName1,_T("ActuatorID"),			0, strFilePath);
	m_GetOTPCheckSpec.m_LensID				=	GetPrivateProfileInt(strProjectName1,_T("LensID"),				0, strFilePath);
	m_GetOTPCheckSpec.m_ManufacturerID		=	GetPrivateProfileInt(strProjectName1,_T("ManufacturerID"),		0, strFilePath);
	m_GetOTPCheckSpec.m_FactoryID			=	GetPrivateProfileInt(strProjectName1,_T("FactoryID"),			0, strFilePath);
	m_GetOTPCheckSpec.m_ManufactureLine		=	GetPrivateProfileInt(strProjectName1,_T("ManufactureLine"),		0, strFilePath);
	//m_GetOTPCheckSpec.m_Year				=	GetPrivateProfileInt(strProjectName1,_T("Year"),				0, strFilePath);
	//m_GetOTPCheckSpec.m_Month				=	GetPrivateProfileInt(strProjectName1,_T("Month"),				0, strFilePath);
	//m_GetOTPCheckSpec.m_Day					=	GetPrivateProfileInt(strProjectName1,_T("Day"),					0, strFilePath);	

	// AWB Data
	m_GetOTPCheckSpec.m_MaxCIE_Ev_Source	=	GetPrivateProfileInt(strProjectName1,_T("MaxCIE_Ev_Source"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinCIE_Ev_Source	=	GetPrivateProfileInt(strProjectName1,_T("MinCIE_Ev_Source"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxCIE_U_Source		=	GetPrivateProfileInt(strProjectName1,_T("MaxCIE_U_Source"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinCIE_U_Source		=	GetPrivateProfileInt(strProjectName1,_T("MinCIE_U_Source"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxCIE_V_Source		=	GetPrivateProfileInt(strProjectName1,_T("MaxCIE_V_Source"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinCIE_V_Source		=	GetPrivateProfileInt(strProjectName1,_T("MinCIE_V_Source"),		0, strFilePath);										
	m_GetOTPCheckSpec.m_u_standard_Source	=	GetPrivateProfileInt(strProjectName1,_T("u_standard_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_v_standard_Source	=	GetPrivateProfileInt(strProjectName1,_T("v_standard_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxTolerance_Source	=	GetPrivateProfileInt(strProjectName1,_T("MaxTolerance_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MinTolerance_Source	=	GetPrivateProfileInt(strProjectName1,_T("MinTolerance_Source"),	0, strFilePath);

	m_GetOTPCheckSpec.m_MaxRValue			=	GetPrivateProfileInt(strProjectName1,_T("MaxR_Source"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MinRValue			=	GetPrivateProfileInt(strProjectName1,_T("MinR_Source"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGrValue			=	GetPrivateProfileInt(strProjectName1,_T("MaxGr_Source"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGrValue			=	GetPrivateProfileInt(strProjectName1,_T("MinGr_Source"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxGbValue			=	GetPrivateProfileInt(strProjectName1,_T("MaxGb_Source"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGbValue			=	GetPrivateProfileInt(strProjectName1,_T("MinGb_Source"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MaxBValue			=	GetPrivateProfileInt(strProjectName1,_T("MaxB_Source"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MinBValue			=	GetPrivateProfileInt(strProjectName1,_T("MinB_Source"),			0, strFilePath);					
	
	m_GetOTPCheckSpec.m_MaxRGain			=	GetPrivateProfileInt(strProjectName1,_T("MaxRGain_Source"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinRGain			=	GetPrivateProfileInt(strProjectName1,_T("MinRGain_Source"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxBGain			=	GetPrivateProfileInt(strProjectName1,_T("MaxBGain_Source"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinBGain			=	GetPrivateProfileInt(strProjectName1,_T("MinBGain_Source"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxGGGain			=	GetPrivateProfileInt(strProjectName1,_T("MaxGGain_Source"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGGGain			=	GetPrivateProfileInt(strProjectName1,_T("MinGGain_Source"),		0, strFilePath);										
	
	m_GetOTPCheckSpec.m_ReCal_RG_diff		=	GetPrivateProfileInt(strProjectName1,_T("ReCal_RG_diff"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_ReCal_BG_diff		=	GetPrivateProfileInt(strProjectName1,_T("ReCal_BG_diff"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_ReCal_GG_diff		=	GetPrivateProfileInt(strProjectName1,_T("ReCal_GG_diff"),		0, strFilePath);

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("MaxRGRatio_UUT_diff"),_T("0.0"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_MaxRGRatio_UUT_diff	= _wtof(szTmp);
	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("MinRGRatio_UUT_diff"),_T("0.0"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_MinRGRatio_UUT_diff	= _wtof(szTmp);
	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("MaxBGRatio_UUT_diff"),_T("0.0"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_MaxBGRatio_UUT_diff	= _wtof(szTmp);
	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("MinBGRatio_UUT_diff"),_T("0.0"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_MinBGRatio_UUT_diff	= _wtof(szTmp);

	m_GetOTPCheckSpec.m_MaxRGolden		=	GetPrivateProfileInt(strProjectName1,_T("MaxRGolden_Source"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinRGolden		=	GetPrivateProfileInt(strProjectName1,_T("MinRGolden_Source"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MaxGrGolden		=	GetPrivateProfileInt(strProjectName1,_T("MaxGrGolden_Source"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGrGolden		=	GetPrivateProfileInt(strProjectName1,_T("MinGrGolden_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGbGolden		=	GetPrivateProfileInt(strProjectName1,_T("MaxGbGolden_Source"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGbGolden		=	GetPrivateProfileInt(strProjectName1,_T("MinGbGolden_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxBGolden		=	GetPrivateProfileInt(strProjectName1,_T("MaxBGolden_Source"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinBGolden		=	GetPrivateProfileInt(strProjectName1,_T("MinBGolden_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxRGainGolden	=	GetPrivateProfileInt(strProjectName1,_T("MaxRGainGolden_Source"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinRGainGolden	=	GetPrivateProfileInt(strProjectName1,_T("MinRGainGolden_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxBGainGolden	=	GetPrivateProfileInt(strProjectName1,_T("MaxBGainGolden_Source"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinBGainGolden	=	GetPrivateProfileInt(strProjectName1,_T("MinBGainGolden_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGGainGolden	=	GetPrivateProfileInt(strProjectName1,_T("MaxGGainGolden_Source"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGGainGolden	=	GetPrivateProfileInt(strProjectName1,_T("MinGGainGolden_Source"),	0, strFilePath);
	m_GetOTPCheckSpec.m_BLC				=	GetPrivateProfileInt(strProjectName1,_T("BLC"),					0, strFilePath);					
	m_GetOTPCheckSpec.m_Multiple		=	GetPrivateProfileInt(strProjectName1,_T("Multiple"),		16384, strFilePath);


	//OC Info
	m_GetOTPCheckSpec.m_PixelHeight		=	GetPrivateProfileInt(strProjectName1,_T("PixelHeight"),		0, strFilePath);
	m_GetOTPCheckSpec.m_PixelWidth		=	GetPrivateProfileInt(strProjectName1,_T("PixelWidth"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MaxOCDistance	=	GetPrivateProfileInt(strProjectName1,_T("MaxOCDistance"),	0, strFilePath);
	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("PixelSize"),_T("1.2"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_PixelSize	= _wtof(szTmp);

	// AF Info
	m_GetOTPCheckSpec.m_MaxInfinityCode		=	GetPrivateProfileInt(strProjectName1,_T("MaxInfinityCode"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MinInfinityCode		=	GetPrivateProfileInt(strProjectName1,_T("MinInfinityCode"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxMacroCode		=	GetPrivateProfileInt(strProjectName1,_T("MaxMacroCode"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MinMacroCode		=	GetPrivateProfileInt(strProjectName1,_T("MinMacroCode"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxCodeDiff			=	GetPrivateProfileInt(strProjectName1,_T("MaxCodeDiff"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MinCodeDiff			=	GetPrivateProfileInt(strProjectName1,_T("MinCodeDiff"),		0, strFilePath);
	m_GetOTPCheckSpec.m_AFInfinityPeak		=	GetPrivateProfileInt(strProjectName1,_T("AFInfinityPeak"),	0, strFilePath);
	m_GetOTPCheckSpec.m_AFMacroPeak			=	GetPrivateProfileInt(strProjectName1,_T("AFMacroPeak"),		0, strFilePath);

	// SFR Macro Distance
	m_GetOTPCheckSpec.m_Posture_Macro			=	GetPrivateProfileInt(strProjectName1,_T("Posture_Macro"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MinCTPosition_Macro		=	GetPrivateProfileInt(strProjectName1,_T("MinCTPosition_Macro"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxCTPosition_Macro		=	GetPrivateProfileInt(strProjectName1,_T("MaxCTPosition_Macro"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MinCenter_Macro			=	GetPrivateProfileInt(strProjectName1,_T("MinCenter_Macro"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MinField4_Macro			=	GetPrivateProfileInt(strProjectName1,_T("MinField4_Macro"),		0, strFilePath);	//Macro 0.4视场
	m_GetOTPCheckSpec.m_MinField6_Macro			=	GetPrivateProfileInt(strProjectName1,_T("MinField6_Macro"),		0, strFilePath);	//Macro 0.6视场
	m_GetOTPCheckSpec.m_MinField8_Macro			=	GetPrivateProfileInt(strProjectName1,_T("MinField8_Macro"),		0, strFilePath);	//Macro 0.8视场
	m_GetOTPCheckSpec.m_MaxField6CTC_Macro		=	GetPrivateProfileInt(strProjectName1,_T("MaxField6CTC_Macro"),	0, strFilePath);

	// SFR Mid Distance
	m_GetOTPCheckSpec.m_Posture_Mid				=	GetPrivateProfileInt(strProjectName1,_T("Posture_Mid"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinCTPosition_Mid		=	GetPrivateProfileInt(strProjectName1,_T("MinCTPosition_Mid"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxCTPosition_Mid		=	GetPrivateProfileInt(strProjectName1,_T("MaxCTPosition_Mid"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MinCenter_Mid			=	GetPrivateProfileInt(strProjectName1,_T("MinCenter_Mid"),		0, strFilePath);	//Mid 中心视场
	m_GetOTPCheckSpec.m_MinField4_Mid			=	GetPrivateProfileInt(strProjectName1,_T("MinField4_Mid"),		0, strFilePath);	//Mid 0.4视场
	m_GetOTPCheckSpec.m_MinField6_Mid			=	GetPrivateProfileInt(strProjectName1,_T("MinField6_Mid"),		0, strFilePath);	//Mid 0.6视场
	m_GetOTPCheckSpec.m_MinField8_Mid			=	GetPrivateProfileInt(strProjectName1,_T("MinField8_Mid"),		0, strFilePath);	//Mid 0.8视场
	m_GetOTPCheckSpec.m_MaxField6CTC_Mid		=	GetPrivateProfileInt(strProjectName1,_T("MaxField6CTC_Mid"),	0, strFilePath);

	// SFR Infinity Distance
	m_GetOTPCheckSpec.m_Posture_Infinity			=	GetPrivateProfileInt(strProjectName1,_T("Posture_Infinity"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MinCTPosition_Infinity		=	GetPrivateProfileInt(strProjectName1,_T("MinCTPosition_Infinity"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxCTPosition_Infinity		=	GetPrivateProfileInt(strProjectName1,_T("MaxCTPosition_Infinity"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MinCenter_Infinity			=	GetPrivateProfileInt(strProjectName1,_T("MinCenter_Infinity"),		0, strFilePath);	//Infinity 中心视场
	m_GetOTPCheckSpec.m_MinField4_Infinity			=	GetPrivateProfileInt(strProjectName1,_T("MinField4_Infinity"),		0, strFilePath);	//Infinity 0.4视场
	m_GetOTPCheckSpec.m_MinField6_Infinity			=	GetPrivateProfileInt(strProjectName1,_T("MinField6_Infinity"),		0, strFilePath);	//Infinity 0.6视场
	m_GetOTPCheckSpec.m_MinField8_Infinity			=	GetPrivateProfileInt(strProjectName1,_T("MinField8_Infinity"),		0, strFilePath);	//Infinity 0.8视场
	m_GetOTPCheckSpec.m_MaxField6CTC_Infinity		=	GetPrivateProfileInt(strProjectName1,_T("MaxField6CTC_Infinity"),	0, strFilePath);

	// LSC
	m_GetOTPCheckSpec.m_LSCCurveCheck		=	GetPrivateProfileInt(strProjectName1,_T("LSCCurveCheck"),		0, strFilePath);
	m_GetOTPCheckSpec.m_LSC_R_Max			=	GetPrivateProfileInt(strProjectName1,_T("LSC_R_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec.m_LSC_R_Min			=	GetPrivateProfileInt(strProjectName1,_T("LSC_R_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSC_Gr_Max			=	GetPrivateProfileInt(strProjectName1,_T("LSC_Gr_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec.m_LSC_Gr_Min			=	GetPrivateProfileInt(strProjectName1,_T("LSC_Gr_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSC_Gb_Max			=	GetPrivateProfileInt(strProjectName1,_T("LSC_Gb_Max"),			0, strFilePath);			    
	m_GetOTPCheckSpec.m_LSC_Gb_Min			=	GetPrivateProfileInt(strProjectName1,_T("LSC_Gb_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSC_B_Max			=	GetPrivateProfileInt(strProjectName1,_T("LSC_B_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec.m_LSC_B_Min			=	GetPrivateProfileInt(strProjectName1,_T("LSC_B_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSCDistanceSpec		=	GetPrivateProfileInt(strProjectName1,_T("LSCDistanceSpec"),		0, strFilePath);	
	m_GetOTPCheckSpec.m_SaveLSCALLDataEn	=	GetPrivateProfileInt(strProjectName1,_T("SaveLSCALLDataEn"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_SaveLSCOKDataEn		=	GetPrivateProfileInt(strProjectName1,_T("SaveLSCOKDataEn"),		0, strFilePath);
	m_GetOTPCheckSpec.m_CenterValueEqualcount =	GetPrivateProfileInt(strProjectName1,_T("CenterValueEqualcount"),0, strFilePath);		
	m_GetOTPCheckSpec.m_Curvediff			=	GetPrivateProfileInt(strProjectName1,_T("Curvediff"),			0, strFilePath);								
	m_GetOTPCheckSpec.m_ErrorCount			=	GetPrivateProfileInt(strProjectName1,_T("ErrorCount"),			0, strFilePath); 							
	m_GetOTPCheckSpec.m_TotalErrorCount		=	GetPrivateProfileInt(strProjectName1,_T("TotalErrorCount"),		0, strFilePath);			

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("edg_center_LimitRatio"),_T("1.01"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_edg_center_LimitRatio	= _wtof(szTmp);

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("edg_center_HighRatio"),_T("1.01"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_edg_center_HighRatio  = _wtof(szTmp);

	m_GetOTPCheckSpec.m_MaxSymColDiff		=	GetPrivateProfileInt(strProjectName1,_T("MaxSymColDiff"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxSymRowDiff		=	GetPrivateProfileInt(strProjectName1,_T("MaxSymRowDiff"),		0, strFilePath);

	m_GetOTPCheckSpec.m_ToleranceValueError	=	GetPrivateProfileInt(strProjectName1,_T("ToleranceValueError"),	0, strFilePath);			
	m_GetOTPCheckSpec.m_ToleranceCountError	=	GetPrivateProfileInt(strProjectName1,_T("ToleranceCountError"),	0, strFilePath);


	m_GetOTPCheckSpec.m_MaxFluoreLightSource_RG	=	GetPrivateProfileInt(strProjectName1,_T("MaxFluoreLightSource_RG"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinFluoreLightSource_RG	=	GetPrivateProfileInt(strProjectName1,_T("MinFluoreLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxFluoreLightSource_BG	=	GetPrivateProfileInt(strProjectName1,_T("MaxFluoreLightSource_BG"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinFluoreLightSource_BG	=	GetPrivateProfileInt(strProjectName1,_T("MinFluoreLightSource_BG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_LSCFluore_Flag			=	GetPrivateProfileInt(strProjectName1,_T("LSCFluore_Flag"),				0, strFilePath);

	m_GetOTPCheckSpec.m_SNFlag				=	GetPrivateProfileInt(strProjectName1,_T("SNFlag"),				    0, strFilePath);
	m_GetOTPCheckSpec.m_LSCFlag				=	GetPrivateProfileInt(strProjectName1,_T("LSCFlag"),				0, strFilePath);						
	m_GetOTPCheckSpec.m_LSCSize				=	GetPrivateProfileInt(strProjectName1,_T("LSCSize"),				0, strFilePath);		     
	m_GetOTPCheckSpec.m_LSCDataEqualCountSpec	=	GetPrivateProfileInt(strProjectName1,_T("LSCDataEqualCountSpec"),  0, strFilePath);		
				
			
		
	
	

	return TRUE;
}


BOOL OtpCheckAPI::OnInitialSpec_Project2(const CString &strFilePath, GetTestSpec &m_GetOTPCheckSpec1)
{
	CString strProjectName2 = _T("Project2_OTPCheck");
	wchar_t szTmp[2048];
	int iBufSize = 2048;

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName2, _T("ProjectName2"), _T("XXXX"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec1.m_ProjectName1  = szTmp;

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName2,_T("TestStation"), _T("终检"), szTmp, iBufSize, strFilePath);	  //获取机种名
	m_GetOTPCheckSpec1.m_TestStation = szTmp;

	int nPos = 0;
	nPos = (m_GetOTPCheckSpec1.m_TestStation).Find('/');
	m_GetOTPCheckSpec1.m_TestStation = (nPos < 0)?m_GetOTPCheckSpec1.m_TestStation:m_GetOTPCheckSpec1.m_TestStation.Left(nPos-1);
				   
	m_GetOTPCheckSpec1.m_Project1Check_En		=	GetPrivateProfileInt(strProjectName2,_T("Project1Check_En"),	0, strFilePath);	
	m_GetOTPCheckSpec1.m_OTPCheckAll_En		=	GetPrivateProfileInt(strProjectName2,_T("OTPCheckAll_En"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_ModuleInfoCheck_En	=	GetPrivateProfileInt(strProjectName2,_T("ModuleInfoCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_AWBLSCCheck_En		=	GetPrivateProfileInt(strProjectName2,_T("AWBLSCCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_AFCheck_En			=	GetPrivateProfileInt(strProjectName2,_T("AFCheck_En"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_PDAFCheck_En			=	GetPrivateProfileInt(strProjectName2,_T("PDAFCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_HDCCheck_En			=	GetPrivateProfileInt(strProjectName2,_T("HDCCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_OISCheck_En			=	GetPrivateProfileInt(strProjectName2,_T("OISCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_OCCheck_En			=	GetPrivateProfileInt(strProjectName2,_T("OCCheck_En"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_SFRCheck_En			=	GetPrivateProfileInt(strProjectName2,_T("SFRCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_AECCheck_En			=	GetPrivateProfileInt(strProjectName2,_T("AECCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_SPCCheck_En			=	GetPrivateProfileInt(strProjectName2,_T("SPCCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_CrossTalkCheck_En	=	GetPrivateProfileInt(strProjectName2,_T("CrossTalkCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_ArcsoftCheck_En		=	GetPrivateProfileInt(strProjectName2,_T("ArcsoftCheck_En"),	0, strFilePath);	
	m_GetOTPCheckSpec1.m_SegmentInfoCheck_En	=	GetPrivateProfileInt(strProjectName2,_T("SegmentInfoCheck_En"),0, strFilePath);
	m_GetOTPCheckSpec1.m_DualCamCheck_En		=	GetPrivateProfileInt(strProjectName2,_T("DualCamCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_OthersCheck_En		=	GetPrivateProfileInt(strProjectName2,_T("OthersCheck_En"),		0, strFilePath);	
				   
	m_GetOTPCheckSpec1.m_OffLineCheck_En		=	GetPrivateProfileInt(strProjectName2,_T("OffLineCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_AFSyncCheck_En      =   GetPrivateProfileInt(strProjectName2,_T("AFSyncCheck_En"),      0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_SaveOTPCheckData		=	GetPrivateProfileInt(strProjectName2,_T("SaveOTPCheckData"),	1, strFilePath);
	m_GetOTPCheckSpec1.m_LSCCurveCheck		=	GetPrivateProfileInt(strProjectName2,_T("LSCCurveCheck"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_EqualCountSpec		=	GetPrivateProfileInt(strProjectName2,_T("EqualCountSpec"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_HisiMasterFlag		=	GetPrivateProfileInt(strProjectName2,_T("HisiMasterFlag"),		0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_ModulInfoFlag		=	GetPrivateProfileInt(strProjectName2,_T("ModulInfoFlag"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_ModuleID				=	GetPrivateProfileInt(strProjectName2,_T("ModuleID"),			0, strFilePath);      			
	m_GetOTPCheckSpec1.m_LensID				=	GetPrivateProfileInt(strProjectName2,_T("LensID"),				0, strFilePath);	      			
	m_GetOTPCheckSpec1.m_ActuatorID				=	GetPrivateProfileInt(strProjectName2,_T("VCMID"),				0, strFilePath);							
	m_GetOTPCheckSpec1.m_DriverIcID			=	GetPrivateProfileInt(strProjectName2,_T("DriverIcID"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_LSCToolVersion		=	GetPrivateProfileInt(strProjectName2,_T("LSCToolVersion"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_PDAFToolVersion		=	GetPrivateProfileInt(strProjectName2,_T("PDAFToolVersion"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_ColorTemperatureID	=	GetPrivateProfileInt(strProjectName2,_T("ColorTemperatureID"),	0, strFilePath);	
	m_GetOTPCheckSpec1.m_FirstPixel			=	GetPrivateProfileInt(strProjectName2,_T("FirstPixel"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MirrorFlip			=	GetPrivateProfileInt(strProjectName2,_T("MirrorFlip"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_SensorNameID			=	GetPrivateProfileInt(strProjectName2,_T("SensorNameID"),		0, strFilePath);				
	m_GetOTPCheckSpec1.m_ManufacturerID			=	GetPrivateProfileInt(strProjectName2,_T("SupplierCode"),		0, strFilePath);				
	m_GetOTPCheckSpec1.m_ModuleCode			=	GetPrivateProfileInt(strProjectName2,_T("ModuleCode"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_Year					=	GetPrivateProfileInt(strProjectName2,_T("Year"),				0, strFilePath);								
	m_GetOTPCheckSpec1.m_Month				=	GetPrivateProfileInt(strProjectName2,_T("Month"),				0, strFilePath);						
	m_GetOTPCheckSpec1.m_Day					=	GetPrivateProfileInt(strProjectName2,_T("Day"),				0, strFilePath);							
	m_GetOTPCheckSpec1.m_LightSourceFlag		=	GetPrivateProfileInt(strProjectName2,_T("LightSourceFlag"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_AForFF_Flag			=	GetPrivateProfileInt(strProjectName2,_T("AForFF_Flag"),		0, strFilePath);				
	m_GetOTPCheckSpec1.m_IRBGID				=	GetPrivateProfileInt(strProjectName2,_T("IRBGID"),				0, strFilePath); 
	m_GetOTPCheckSpec1.m_PlatformID			=	GetPrivateProfileInt(strProjectName2,_T("PlatformID"),			0, strFilePath); 
	m_GetOTPCheckSpec1.m_DualCameraFlag		=	GetPrivateProfileInt(strProjectName2,_T("DualCameraFlag"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_FnumberID			=	GetPrivateProfileInt(strProjectName2,_T("FnumberID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_FactoryID			=	GetPrivateProfileInt(strProjectName2,_T("FactoryID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_CalibrationVersion	=	GetPrivateProfileInt(strProjectName2,_T("CalibrationVersion"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_OISID				=	GetPrivateProfileInt(strProjectName2,_T("OISID"),				0, strFilePath);	
	m_GetOTPCheckSpec1.m_ModuleVersion		=	GetPrivateProfileInt(strProjectName2,_T("ModuleVersion"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_SoftVersion			=	GetPrivateProfileInt(strProjectName2,_T("SoftVersion"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_AWBLSCStation		=	GetPrivateProfileInt(strProjectName2,_T("AWBLSCStation"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_PDAFStation			=	GetPrivateProfileInt(strProjectName2,_T("PDAFStation"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_AFStation			=	GetPrivateProfileInt(strProjectName2,_T("AFStation"),			0, strFilePath);				
	m_GetOTPCheckSpec1.m_EepromTableRevision				=	GetPrivateProfileInt(strProjectName2,_T("EEPROMID"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_ProgramName			=	GetPrivateProfileInt(strProjectName2,_T("ProgramName"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_SensorSlaveID		=	GetPrivateProfileInt(strProjectName2,_T("SensorSlaveID"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_EEPROMSlaveID		=	GetPrivateProfileInt(strProjectName2,_T("EEPROMSlaveID"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_VCMSlaveID			=	GetPrivateProfileInt(strProjectName2,_T("VCMSlaveID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_SensorStatus			=	GetPrivateProfileInt(strProjectName2,_T("SensorStatus"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_CheckSN_En			=	GetPrivateProfileInt(strProjectName2,_T("CheckSN_En"),			0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_VendorID				=	GetPrivateProfileInt(strProjectName2,_T("VendorID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_OISFWVER				=	GetPrivateProfileInt(strProjectName2,_T("OISFWVER"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_ProductID				=	GetPrivateProfileInt(strProjectName2,_T("ProductID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MapVersionID			=	GetPrivateProfileInt(strProjectName2,_T("MapVersionID"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MapHistoryID			=	GetPrivateProfileInt(strProjectName2,_T("MapHistoryID"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_EepromorOTP			=	GetPrivateProfileInt(strProjectName2,_T("EepromorOTP"),		0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_GyroID					=	GetPrivateProfileInt(strProjectName2,_T("GyroID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_OisDriverIcID			=	GetPrivateProfileInt(strProjectName2,_T("OisDriverIcID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_FpcPcbVersion			=	GetPrivateProfileInt(strProjectName2,_T("FpcPcbVersion"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_ManufactureLine		=	GetPrivateProfileInt(strProjectName2,_T("ManufactureLine"),			0, strFilePath);

	m_GetOTPCheckSpec1.m_SegmentInfoFlag		=	GetPrivateProfileInt(strProjectName2,_T("SegmentInfoFlag"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_SegmentCount			=	GetPrivateProfileInt(strProjectName2,_T("SegmentCount"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_SegmentAFOffset		=	GetPrivateProfileInt(strProjectName2,_T("SegmentAFOffset"),	0, strFilePath);				
	m_GetOTPCheckSpec1.m_SegmentAWBOffset		=	GetPrivateProfileInt(strProjectName2,_T("SegmentAWBOffset"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_SegmentLSCOffset		=	GetPrivateProfileInt(strProjectName2,_T("SegmentLSCOffset"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_SegmentAECOffset		=	GetPrivateProfileInt(strProjectName2,_T("SegmentAECOffset"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_SegmentPDAFOffset	=	GetPrivateProfileInt(strProjectName2,_T("SegmentPDAFOffset"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_SegmentOISOffset		=	GetPrivateProfileInt(strProjectName2,_T("SegmentOISOffset"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_SegmentDualCamOffset	=	GetPrivateProfileInt(strProjectName2,_T("SegmentDualCamOffset"),0, strFilePath);
	m_GetOTPCheckSpec1.m_SegmentChecksumOffset=	GetPrivateProfileInt(strProjectName2,_T("SegmentChecksumOffset"),0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_SegmentAFTagID		=	GetPrivateProfileInt(strProjectName2,_T("SegmentAFTagID"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_AFTypeNumber			=	GetPrivateProfileInt(strProjectName2,_T("AFTypeNumber"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxInfinityCode		=	GetPrivateProfileInt(strProjectName2,_T("MaxInfinityCode"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinInfinityCode		=	GetPrivateProfileInt(strProjectName2,_T("MinInfinityCode"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxMacroCode			=	GetPrivateProfileInt(strProjectName2,_T("MaxMacroCode"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinMacroCode			=	GetPrivateProfileInt(strProjectName2,_T("MinMacroCode"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxPostureDifA		=	GetPrivateProfileInt(strProjectName2,_T("MaxPostureDifA"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinPostureDifA		=	GetPrivateProfileInt(strProjectName2,_T("MinPostureDifA"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxPostureDifB		=	GetPrivateProfileInt(strProjectName2,_T("MaxPostureDifB"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinPostureDifB		=	GetPrivateProfileInt(strProjectName2,_T("MinPostureDifB"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxPostureDifC		=	GetPrivateProfileInt(strProjectName2,_T("MaxPostureDifC"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinPostureDifC		=	GetPrivateProfileInt(strProjectName2,_T("MinPostureDifC"),		0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_SegmentAWBTagID		=	GetPrivateProfileInt(strProjectName2,_T("SegmentAWBTagID"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_AWBTypeNumber		=	GetPrivateProfileInt(strProjectName2,_T("AWBTypeNumber"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_AWBMirror			=	GetPrivateProfileInt(strProjectName2,_T("AWBMirror"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_AWBFilp				=	GetPrivateProfileInt(strProjectName2,_T("AWBFilp"),			0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_SegmentLSCTagID		=	GetPrivateProfileInt(strProjectName2,_T("SegmentLSCTagID"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_LSCTypeNumber		=	GetPrivateProfileInt(strProjectName2,_T("LSCTypeNumber"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_LSCMirror			=	GetPrivateProfileInt(strProjectName2,_T("LSCMirror"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_LSCFilp				=	GetPrivateProfileInt(strProjectName2,_T("LSCFilp"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_LSCVersionNumber		=	GetPrivateProfileInt(strProjectName2,_T("LSCVersionNumber"),	0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_SegmentAECTagID		=	GetPrivateProfileInt(strProjectName2,_T("SegmentAECTagID"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_AECFlag				=	GetPrivateProfileInt(strProjectName2,_T("AECFlag"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_AECVersionNumber		=	GetPrivateProfileInt(strProjectName2,_T("AECVersionNumber"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxBrightnessRatio	=	GetPrivateProfileInt(strProjectName2,_T("MaxBrightnessRatio"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinBrightnessRatio	=	GetPrivateProfileInt(strProjectName2,_T("MinBrightnessRatio"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MainGain				=	GetPrivateProfileInt(strProjectName2,_T("MainGain"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MainExposureTime		=	GetPrivateProfileInt(strProjectName2,_T("MainExposureTime"),	0, strFilePath);										
	m_GetOTPCheckSpec1.m_SubGain				=	GetPrivateProfileInt(strProjectName2,_T("SubGain"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_SubExposureTime		=	GetPrivateProfileInt(strProjectName2,_T("SubExposureTime"),	0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_SegmentPDAFTagID		=	GetPrivateProfileInt(strProjectName2,_T("SegmentPDAFTagID"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_PDAFTypeNumber		=	GetPrivateProfileInt(strProjectName2,_T("PDAFTypeNumber"),		0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_SegmentDualCamTagID	=	GetPrivateProfileInt(strProjectName2,_T("SegmentDualCamTagID"),0, strFilePath);						
	m_GetOTPCheckSpec1.m_DualCamFlag			=	GetPrivateProfileInt(strProjectName2,_T("DualCamFlag"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_DualCamVersionNumber	=	GetPrivateProfileInt(strProjectName2,_T("DualCamVersionNumber"),0, strFilePath);						
	m_GetOTPCheckSpec1.m_SegmentCheckSumTagID	=	GetPrivateProfileInt(strProjectName2,_T("SegmentCheckSumTagID"),0, strFilePath);
	m_GetOTPCheckSpec1.m_AWBFlag				=	GetPrivateProfileInt(strProjectName2,_T("AWBFlag"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxRGain				=	GetPrivateProfileInt(strProjectName2,_T("MaxRGain"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinRGain				=	GetPrivateProfileInt(strProjectName2,_T("MinRGain"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxBGain				=	GetPrivateProfileInt(strProjectName2,_T("MaxBGain"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinBGain				=	GetPrivateProfileInt(strProjectName2,_T("MinBGain"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxGGGain			=	GetPrivateProfileInt(strProjectName2,_T("MaxGGGain"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGGGain			=	GetPrivateProfileInt(strProjectName2,_T("MinGGGain"),			0, strFilePath);										
	m_GetOTPCheckSpec1.m_MaxRBGain			=	GetPrivateProfileInt(strProjectName2,_T("MaxRBGain"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MinRBGain			=	GetPrivateProfileInt(strProjectName2,_T("MinRBGain"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_RG			=	GetPrivateProfileInt(strProjectName2,_T("Typical_RG"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_Typical_BG			=	GetPrivateProfileInt(strProjectName2,_T("Typical_BG"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_Typical_GG			=	GetPrivateProfileInt(strProjectName2,_T("Typical_GG"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MaxRValue			=	GetPrivateProfileInt(strProjectName2,_T("MaxRValue"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinRValue			=	GetPrivateProfileInt(strProjectName2,_T("MinRValue"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MaxBValue			=	GetPrivateProfileInt(strProjectName2,_T("MaxBValue"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinBValue			=	GetPrivateProfileInt(strProjectName2,_T("MinBValue"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MaxGrValue			=	GetPrivateProfileInt(strProjectName2,_T("MaxGrValue"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGrValue			=	GetPrivateProfileInt(strProjectName2,_T("MinGrValue"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxGbValue			=	GetPrivateProfileInt(strProjectName2,_T("MaxGbValue"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGbValue			=	GetPrivateProfileInt(strProjectName2,_T("MinGbValue"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_Typical_R			=	GetPrivateProfileInt(strProjectName2,_T("Typical_R"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_Typical_B			=	GetPrivateProfileInt(strProjectName2,_T("Typical_B"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_Typical_Gr			=	GetPrivateProfileInt(strProjectName2,_T("Typical_Gr"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_Typical_Gb			=	GetPrivateProfileInt(strProjectName2,_T("Typical_Gb"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MaxISO				=	GetPrivateProfileInt(strProjectName2,_T("MaxISO"),				0, strFilePath);
	m_GetOTPCheckSpec1.m_MinISO				=	GetPrivateProfileInt(strProjectName2,_T("MinISO"),				0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxRGain_HighCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxRGain_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinRGain_HighCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinRGain_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxBGain_HighCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxBGain_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinBGain_HighCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinBGain_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxGGGain_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("MaxGGGain_HighCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGGGain_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("MinGGGain_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_RG_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_RG_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_Typical_BG_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_BG_HighCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_Typical_GG_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_GG_HighCCT"),	0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxRGain_Fluore		=	GetPrivateProfileInt(strProjectName2,_T("MaxRGain_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinRGain_Fluore		=	GetPrivateProfileInt(strProjectName2,_T("MinRGain_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxBGain_Fluore		=	GetPrivateProfileInt(strProjectName2,_T("MaxBGain_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinBGain_Fluore		=	GetPrivateProfileInt(strProjectName2,_T("MinBGain_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxGGGain_Fluore		=	GetPrivateProfileInt(strProjectName2,_T("MaxGGGain_Fluore"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGGGain_Fluore		=	GetPrivateProfileInt(strProjectName2,_T("MinGGGain_Fluore"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_RG_Fluore	=	GetPrivateProfileInt(strProjectName2,_T("Typical_RG_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_Typical_BG_Fluore	=	GetPrivateProfileInt(strProjectName2,_T("Typical_BG_Fluore"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_Typical_GG_Fluore	=	GetPrivateProfileInt(strProjectName2,_T("Typical_GG_Fluore"),	0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxRGain_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxRGain_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinRGain_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinRGain_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxBGain_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxBGain_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinBGain_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinBGain_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxGGGain_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxGGGain_LowCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGGGain_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinGGGain_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_RG_LowCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_RG_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_Typical_BG_LowCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_BG_LowCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_Typical_GG_LowCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_GG_LowCCT"),	0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxR_HighCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxR_HighCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinR_HighCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinR_HighCCT"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGr_HighCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxGr_HighCCT"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGr_HighCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinGr_HighCCT"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGb_HighCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxGb_HighCCT"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGb_HighCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinGb_HighCCT"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxB_HighCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxB_HighCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinB_HighCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinB_HighCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_Typical_R_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_R_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_Gr_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_Gr_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_Gb_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_Gb_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_B_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_B_HighCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_AWBHighCCT_Flag		=	GetPrivateProfileInt(strProjectName2,_T("AWBHighCCT_Flag"),	0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxRGRatio_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("MaxRGRatio_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinRGRatio_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("MinRGRatio_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxBGRatio_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("MaxBGRatio_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinBGRatio_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("MinBGRatio_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGGRatio_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("MaxGGRatio_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinGGRatio_HighCCT	=	GetPrivateProfileInt(strProjectName2,_T("MinGGRatio_HighCCT"),	0, strFilePath);

	m_GetOTPCheckSpec1.m_MaxRGRatio_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxRGRatio_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinRGRatio_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinRGRatio_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxBGRatio_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxBGRatio_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinBGRatio_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinBGRatio_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGGRatio_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MaxGGRatio_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinGGRatio_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("MinGGRatio_LowCCT"),	0, strFilePath);



	m_GetOTPCheckSpec1.m_MaxR_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxR_LowCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinR_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinR_LowCCT"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGr_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxGr_LowCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGr_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinGr_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGb_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxGb_LowCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGb_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinGb_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxB_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxB_LowCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinB_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinB_LowCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_Typical_R_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("Typical_R_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_Gr_LowCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_Gr_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_Gb_LowCCT	=	GetPrivateProfileInt(strProjectName2,_T("Typical_Gb_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_B_LowCCT		=	GetPrivateProfileInt(strProjectName2,_T("Typical_B_LowCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_AWBLowCCT_Flag		=	GetPrivateProfileInt(strProjectName2,_T("AWBLowCCT_Flag"),		0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxR_Fluore			=	GetPrivateProfileInt(strProjectName2,_T("MaxR_Fluore"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinR_Fluore			=	GetPrivateProfileInt(strProjectName2,_T("MinR_Fluore"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGr_Fluore			=	GetPrivateProfileInt(strProjectName2,_T("MaxGr_Fluore"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGr_Fluore			=	GetPrivateProfileInt(strProjectName2,_T("MinGr_Fluore"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGb_Fluore			=	GetPrivateProfileInt(strProjectName2,_T("MaxGb_Fluore"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGb_Fluore			=	GetPrivateProfileInt(strProjectName2,_T("MinGb_Fluore"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxB_Fluore			=	GetPrivateProfileInt(strProjectName2,_T("MaxB_Fluore"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinB_Fluore			=	GetPrivateProfileInt(strProjectName2,_T("MinB_Fluore"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_Typical_R_Fluore		=	GetPrivateProfileInt(strProjectName2,_T("Typical_R_Fluore"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_Gr_Fluore	=	GetPrivateProfileInt(strProjectName2,_T("Typical_Gr_Fluore"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_Gb_Fluore	=	GetPrivateProfileInt(strProjectName2,_T("Typical_Gb_Fluore"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_Typical_B_Fluore		=	GetPrivateProfileInt(strProjectName2,_T("Typical_B_Fluore"),	0, strFilePath);					
	m_GetOTPCheckSpec1.m_AWBFluore_Flag		=	GetPrivateProfileInt(strProjectName2,_T("AWBFluore_Flag"),		0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxHighCCTLightSource_RG	=	GetPrivateProfileInt(strProjectName2,_T("MaxHighCCTLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinHighCCTLightSource_RG	=	GetPrivateProfileInt(strProjectName2,_T("MinHighCCTLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxHighCCTLightSource_BG	=	GetPrivateProfileInt(strProjectName2,_T("MaxHighCCTLightSource_BG"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinHighCCTLightSource_BG	=	GetPrivateProfileInt(strProjectName2,_T("MinHighCCTLightSource_BG"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_LSCHighCCT_Flag			=	GetPrivateProfileInt(strProjectName2,_T("LSCHighCCT_Flag"),			0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxLowCCTLightSource_RG	=	GetPrivateProfileInt(strProjectName2,_T("MaxLowCCTLightSource_RG"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinLowCCTLightSource_RG	=	GetPrivateProfileInt(strProjectName2,_T("MinLowCCTLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxLowCCTLightSource_BG	=	GetPrivateProfileInt(strProjectName2,_T("MaxLowCCTLightSource_BG"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinLowCCTLightSource_BG	=	GetPrivateProfileInt(strProjectName2,_T("MinLowCCTLightSource_BG"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_LSCLowCCT_Flag			=	GetPrivateProfileInt(strProjectName2,_T("LSCLowCCT_Flag"),				0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_MaxFluoreLightSource_RG	=	GetPrivateProfileInt(strProjectName2,_T("MaxFluoreLightSource_RG"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinFluoreLightSource_RG	=	GetPrivateProfileInt(strProjectName2,_T("MinFluoreLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxFluoreLightSource_BG	=	GetPrivateProfileInt(strProjectName2,_T("MaxFluoreLightSource_BG"),	0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinFluoreLightSource_BG	=	GetPrivateProfileInt(strProjectName2,_T("MinFluoreLightSource_BG"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_LSCFluore_Flag			=	GetPrivateProfileInt(strProjectName2,_T("LSCFluore_Flag"),				0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_SNFlag				=	GetPrivateProfileInt(strProjectName2,_T("SNFlag"),				    0, strFilePath);
	m_GetOTPCheckSpec1.m_LSCFlag				=	GetPrivateProfileInt(strProjectName2,_T("LSCFlag"),				0, strFilePath);				
	m_GetOTPCheckSpec1.m_LSCSize				=	GetPrivateProfileInt(strProjectName2,_T("LSCSize"),				0, strFilePath);		     
	m_GetOTPCheckSpec1.m_LSCDataEqualCountSpec=	GetPrivateProfileInt(strProjectName2,_T("LSCDataEqualCountSpec"),  0, strFilePath);		
	m_GetOTPCheckSpec1.m_ToleranceValueError	=	GetPrivateProfileInt(strProjectName2,_T("ToleranceValueError"),	0, strFilePath);			
	m_GetOTPCheckSpec1.m_ToleranceCountError	=	GetPrivateProfileInt(strProjectName2,_T("ToleranceCountError"),	0, strFilePath);			
	m_GetOTPCheckSpec1.m_SaveLSCALLDataEn		=	GetPrivateProfileInt(strProjectName2,_T("SaveLSCALLDataEn"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_SaveLSCOKDataEn		=	GetPrivateProfileInt(strProjectName2,_T("SaveLSCOKDataEn"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_LSC_R_Max			=	GetPrivateProfileInt(strProjectName2,_T("LSC_R_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec1.m_LSC_R_Min			=	GetPrivateProfileInt(strProjectName2,_T("LSC_R_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_LSC_Gr_Max			=	GetPrivateProfileInt(strProjectName2,_T("LSC_Gr_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec1.m_LSC_Gr_Min			=	GetPrivateProfileInt(strProjectName2,_T("LSC_Gr_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_LSC_Gb_Max			=	GetPrivateProfileInt(strProjectName2,_T("LSC_Gb_Max"),			0, strFilePath);			    
	m_GetOTPCheckSpec1.m_LSC_Gb_Min			=	GetPrivateProfileInt(strProjectName2,_T("LSC_Gb_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_LSC_B_Max			=	GetPrivateProfileInt(strProjectName2,_T("LSC_B_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec1.m_LSC_B_Min			=	GetPrivateProfileInt(strProjectName2,_T("LSC_B_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_LSCDistanceSpec		=	GetPrivateProfileInt(strProjectName2,_T("LSCDistanceSpec"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_CenterValueEqualcount =	GetPrivateProfileInt(strProjectName2,_T("CenterValueEqualcount"),0, strFilePath);		
	m_GetOTPCheckSpec1.m_Curvediff			=	GetPrivateProfileInt(strProjectName2,_T("Curvediff"),			0, strFilePath);								
	m_GetOTPCheckSpec1.m_ErrorCount			=	GetPrivateProfileInt(strProjectName2,_T("ErrorCount"),			0, strFilePath); 							
	m_GetOTPCheckSpec1.m_TotalErrorCount		=	GetPrivateProfileInt(strProjectName2,_T("TotalErrorCount"),	0, strFilePath);			
				   
				   
	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName2, _T("edg_center_LimitRatio"),_T("1.01"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec1.m_edg_center_LimitRatio  = _wtof(szTmp);

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName2, _T("edg_center_HighRatio"),_T("1.01"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec1.m_edg_center_HighRatio  = _wtof(szTmp);
				   
	m_GetOTPCheckSpec1.m_MaxSymColDiff		=	GetPrivateProfileInt(strProjectName2,_T("MaxSymColDiff"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MaxSymRowDiff		=	GetPrivateProfileInt(strProjectName2,_T("MaxSymRowDiff"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_AFFlag				=	GetPrivateProfileInt(strProjectName2,_T("AFFlag"),				0, strFilePath);						
	m_GetOTPCheckSpec1.m_AFCalDirection		=	GetPrivateProfileInt(strProjectName2,_T("AFCalDirection"),		0, strFilePath);	  
	m_GetOTPCheckSpec1.m_MaxAFCode1000cm		=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode1000cm"),	0, strFilePath);	
	m_GetOTPCheckSpec1.m_MinAFCode1000cm		=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode1000cm"),	0, strFilePath);	
	m_GetOTPCheckSpec1.m_MaxAFCode500cm		=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode500cm"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_MinAFCode500cm		=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode500cm"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_MaxAFCode10cm		=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode10cm"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_MinAFCode10cm		=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode10cm"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxAFCode12cm		=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode12cm"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_MinAFCode12cm		=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode12cm"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxAFCode8cm			=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode8cm"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_MinAFCode8cm			=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode8cm"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxAFCode7cm			=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode7cm"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_MinAFCode7cm			=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode7cm"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxCodeDiff			=	GetPrivateProfileInt(strProjectName2,_T("MaxCodeDiff"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinCodeDiff			=	GetPrivateProfileInt(strProjectName2,_T("MinCodeDiff"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MaxStartCode			=	GetPrivateProfileInt(strProjectName2,_T("MaxStartCode"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinStartCode			=	GetPrivateProfileInt(strProjectName2,_T("MinStartCode"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxAFStartCurrent		=	GetPrivateProfileInt(strProjectName2,_T("MaxAFStartCurrent"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinAFStartCurrent		=	GetPrivateProfileInt(strProjectName2,_T("MinAFStartCurrent"),	0, strFilePath);
	
	m_GetOTPCheckSpec1.m_MinCenter_Macro			=	GetPrivateProfileInt(strProjectName2,_T("MinCenter_7m"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MinField4_Macro			=	GetPrivateProfileInt(strProjectName2,_T("MinField4_7m"),		0, strFilePath);		//7M 0.4视场
	m_GetOTPCheckSpec1.m_MinField6_Macro			=	GetPrivateProfileInt(strProjectName2,_T("MinField6_7m"),		0, strFilePath);		//7M 0.6视场
	m_GetOTPCheckSpec1.m_MinField8_Macro			=	GetPrivateProfileInt(strProjectName2,_T("MinField8_7m"),		0, strFilePath);		//7M 0.8视场

	m_GetOTPCheckSpec1.m_MinCenter_Mid			=	GetPrivateProfileInt(strProjectName2,_T("MinCenter_8cm"),		0, strFilePath);		//8cM 中心视场
	m_GetOTPCheckSpec1.m_MinField4_Mid			=	GetPrivateProfileInt(strProjectName2,_T("MinField4_8cm"),		0, strFilePath);		//8cM 0.4视场
	m_GetOTPCheckSpec1.m_MinField6_Mid			=	GetPrivateProfileInt(strProjectName2,_T("MinField6_8cm"),		0, strFilePath);		//8cM 0.6视场
	m_GetOTPCheckSpec1.m_MinField8_Mid			=	GetPrivateProfileInt(strProjectName2,_T("MinField8_8cm"),		0, strFilePath);		//8cM 0.8视场


	m_GetOTPCheckSpec1.m_PDAFFlag				=	GetPrivateProfileInt(strProjectName2,_T("PDAFFlag"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_PDAFStep1Flag		=	GetPrivateProfileInt(strProjectName2,_T("PDAFStep1Flag"),		0, strFilePath);	
	m_GetOTPCheckSpec1.m_PDAFStep1Size		=	GetPrivateProfileInt(strProjectName2,_T("PDAFStep1Size"),		0, strFilePath);	  
	m_GetOTPCheckSpec1.m_PDAFStep2Flag		=	GetPrivateProfileInt(strProjectName2,_T("PDAFStep2Flag"),		0, strFilePath);	  
	m_GetOTPCheckSpec1.m_PDAFStep2Size		=	GetPrivateProfileInt(strProjectName2,_T("PDAFStep2Size"),		0, strFilePath);	  
	m_GetOTPCheckSpec1.m_PDAFVersionNum		=	GetPrivateProfileInt(strProjectName2,_T("PDAFVersionNum"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_GainMapFlag			=	GetPrivateProfileInt(strProjectName2,_T("GainMapFlag"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_GainMapWidth			=	GetPrivateProfileInt(strProjectName2,_T("GainMapWidth"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_GainMapHeight		=	GetPrivateProfileInt(strProjectName2,_T("GainMapHeight"),		0, strFilePath);		
	m_GetOTPCheckSpec1.m_MaxGainMap			=	GetPrivateProfileInt(strProjectName2,_T("MaxGainMap"),			0, strFilePath);				
	m_GetOTPCheckSpec1.m_MinGainMap			=	GetPrivateProfileInt(strProjectName2,_T("MinGainMap"),			0, strFilePath);				
	m_GetOTPCheckSpec1.m_DCCFlag				=	GetPrivateProfileInt(strProjectName2,_T("DCCFlag"),			0, strFilePath);	 				
	m_GetOTPCheckSpec1.m_DCCFormat			=	GetPrivateProfileInt(strProjectName2,_T("DCCFormat"),			0, strFilePath);				
	m_GetOTPCheckSpec1.m_DCCMapWidth			=	GetPrivateProfileInt(strProjectName2,_T("DCCMapWidth"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_DCCMapHeight			=	GetPrivateProfileInt(strProjectName2,_T("DCCMapHeight"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_MaxDCC				=	GetPrivateProfileInt(strProjectName2,_T("MaxDCC"),				0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinDCC				=	GetPrivateProfileInt(strProjectName2,_T("MinDCC"),				0, strFilePath);						
	m_GetOTPCheckSpec1.m_OISWideTag			=	GetPrivateProfileInt(strProjectName2,_T("OISWideTag"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_OISVersion			=	GetPrivateProfileInt(strProjectName2,_T("OISVersion"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_OCFlag				=	GetPrivateProfileInt(strProjectName2,_T("OCFlag"),				0, strFilePath);		  	
	m_GetOTPCheckSpec1.m_MaxOCX				=	GetPrivateProfileInt(strProjectName2,_T("MaxOCX"),				0, strFilePath);	  	  
	m_GetOTPCheckSpec1.m_MinOCX				=	GetPrivateProfileInt(strProjectName2,_T("MinOCX"),				0, strFilePath);	  		
	m_GetOTPCheckSpec1.m_MaxOCY				=	GetPrivateProfileInt(strProjectName2,_T("MaxOCY"),				0, strFilePath);	  		
	m_GetOTPCheckSpec1.m_MinOCY				=	GetPrivateProfileInt(strProjectName2,_T("MinOCY"),				0, strFilePath);	  	  
	m_GetOTPCheckSpec1.m_MaxShiftX			=	GetPrivateProfileInt(strProjectName2,_T("MaxShiftX"),			0, strFilePath);	 	
	m_GetOTPCheckSpec1.m_MinShiftX			=	GetPrivateProfileInt(strProjectName2,_T("MinShiftX"),			0, strFilePath);	  
	m_GetOTPCheckSpec1.m_MaxShiftY			=	GetPrivateProfileInt(strProjectName2,_T("MaxShiftY"),			0, strFilePath);	 	
	m_GetOTPCheckSpec1.m_MinShiftY			=	GetPrivateProfileInt(strProjectName2,_T("MinShiftY"),			0, strFilePath);	 	
	m_GetOTPCheckSpec1.m_MaxTiltX				=	GetPrivateProfileInt(strProjectName2,_T("MaxTiltX"),			0, strFilePath);	  	
	m_GetOTPCheckSpec1.m_MinTiltX				=	GetPrivateProfileInt(strProjectName2,_T("MinTiltX"),			0, strFilePath);	 		
	m_GetOTPCheckSpec1.m_MaxTiltY				=	GetPrivateProfileInt(strProjectName2,_T("MaxTiltY"),			0, strFilePath);			
	m_GetOTPCheckSpec1.m_MinTiltY				=	GetPrivateProfileInt(strProjectName2,_T("MinTiltY"),			0, strFilePath);			
	m_GetOTPCheckSpec1.m_MaxRotation			=	GetPrivateProfileInt(strProjectName2,_T("MaxRotation"),		0, strFilePath);	
	m_GetOTPCheckSpec1.m_MinRotation			=	GetPrivateProfileInt(strProjectName2,_T("MinRotation"),		0, strFilePath);	
	m_GetOTPCheckSpec1.m_MaxSPC				=	GetPrivateProfileInt(strProjectName2,_T("MaxSPC"),				0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinSPC				=	GetPrivateProfileInt(strProjectName2,_T("MinSPC"),				0, strFilePath);					
	m_GetOTPCheckSpec1.m_BurnedAFUpDownPosture =	GetPrivateProfileInt(strProjectName2,_T("BurnedAFUpDownPosture"),0, strFilePath);		
	m_GetOTPCheckSpec1.m_AFUpDownPostureFlag	=	GetPrivateProfileInt(strProjectName2,_T("AFUpDownPostureFlag"),0, strFilePath);	
	m_GetOTPCheckSpec1.m_MaxAFCode8cmUp		=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode8cmUp"),		0, strFilePath);				
	m_GetOTPCheckSpec1.m_MinAFCode8cmUp		=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode8cmUp"),		0, strFilePath);				
	m_GetOTPCheckSpec1.m_MaxAFCode1000cmUp	=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode1000cmUp"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_MinAFCode1000cmUp	=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode1000cmUp"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_MaxCodeDiffUp		=	GetPrivateProfileInt(strProjectName2,_T("MaxCodeDiffUp"),		0, strFilePath);				
	m_GetOTPCheckSpec1.m_MinCodeDiffUp		=	GetPrivateProfileInt(strProjectName2,_T("MinCodeDiffUp"),		0, strFilePath);				
	m_GetOTPCheckSpec1.m_MaxAFCode8cmDown		=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode8cmDown"),	0, strFilePath);			
	m_GetOTPCheckSpec1.m_MinAFCode8cmDown		=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode8cmDown"),	0, strFilePath);			
	m_GetOTPCheckSpec1.m_MaxAFCode1000cmDown	=	GetPrivateProfileInt(strProjectName2,_T("MaxAFCode1000cmDown"),0, strFilePath);	
	m_GetOTPCheckSpec1.m_MinAFCode1000cmDown	=	GetPrivateProfileInt(strProjectName2,_T("MinAFCode1000cmDown"),0, strFilePath); 
	m_GetOTPCheckSpec1.m_MaxCodeDiffDown		=	GetPrivateProfileInt(strProjectName2,_T("MaxCodeDiffDown"),	0, strFilePath);			
	m_GetOTPCheckSpec1.m_MinCodeDiffDown		=	GetPrivateProfileInt(strProjectName2,_T("MinCodeDiffDown"),	0, strFilePath);			
	m_GetOTPCheckSpec1.m_CrossTalkFlag		=	GetPrivateProfileInt(strProjectName2,_T("CrossTalkFlag"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_ArcsoftFlag			=	GetPrivateProfileInt(strProjectName2,_T("ArcsoftFlag"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxAVDD				=	GetPrivateProfileInt(strProjectName2,_T("MaxAVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MinAVDD				=	GetPrivateProfileInt(strProjectName2,_T("MinAVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxDVDD				=	GetPrivateProfileInt(strProjectName2,_T("MaxDVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MinDVDD				=	GetPrivateProfileInt(strProjectName2,_T("MinDVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxIOVDD				=	GetPrivateProfileInt(strProjectName2,_T("MaxIOVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MinIOVDD				=	GetPrivateProfileInt(strProjectName2,_T("MinIOVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxAFVDD				=	GetPrivateProfileInt(strProjectName2,_T("MaxAFVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MinAFVDD				=	GetPrivateProfileInt(strProjectName2,_T("MinAFVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxDRVAVDD			=	GetPrivateProfileInt(strProjectName2,_T("MaxDRVAVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MinDRVAVDD			=	GetPrivateProfileInt(strProjectName2,_T("MinDRVAVDD"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxAVDD18			=	GetPrivateProfileInt(strProjectName2,_T("MaxAVDD18"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MinAVDD18			=	GetPrivateProfileInt(strProjectName2,_T("MinAVDD18"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxAVDD28			=	GetPrivateProfileInt(strProjectName2,_T("MaxAVDD28"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MinAVDD28			=	GetPrivateProfileInt(strProjectName2,_T("MinAVDD28"),			0, strFilePath);
	return TRUE;
}


BOOL OtpCheckAPI::SaveLSCDataSum_LG(float * LSCValue, wchar_t * Errormsg, int flag)
{
	/************************************************************************/
	/*						创建保存数据的文件夹							*/
	/************************************************************************/
	CString saveLSCDataSumPath;
	CTime time = CTime::GetCurrentTime(); 											

	CString saveDataPath = _T("D:\\Data\\") + m_StationName + ("\\");
	CreateDirectory(saveDataPath,NULL);
	CString tempYear = _T(""),tempMonth = _T(""),tempDay = _T("");
	tempYear.Format(_T("%d"),time.GetYear());
	tempMonth.Format(_T("%d"),time.GetMonth());
	tempDay.Format(_T("%d"),time.GetDay());
	saveDataPath = saveDataPath + tempYear + tempMonth + tempDay +_T("\\");
	CreateDirectory(saveDataPath,NULL);
	if (flag == 0)
	{
		saveLSCDataSumPath = saveDataPath + _T("AnalyzeSum_MasterLSC.xls");
	}
	else
	{
		saveLSCDataSumPath = saveDataPath + _T("AnalyzeSum_SlaveLSC.xls");
	}

	/************************************************************************/
	/*							创建表格文件							    */
	/************************************************************************/
	CString FileHeaderSum;
	CString FileContentSum;
	CString tempHeaderSum;
	CString tempContentSum;
	FILE *fp =_wfopen(saveLSCDataSumPath, _T("a"));
	if (fp == NULL)
	{
		Catstring(Errormsg,_T("Save LSC Data Sum File Failed!"));
		return FALSE;
	}
	fclose(fp);

	tempHeaderSum.Format(_T("\t\t\t\t\t\t\t\tAnalyze LSC Data Sum\n Fuse ID\tTest Time\t R-个数\t Gr-个数\t Gb-个数\t B-个数\tR-Max误差\tGr-Max误差\tGb-Max误差\tB-Max误差\tR-每行最多个数\tGr-每行最多个数\tGb-每行最多个数\tB-每行最多个数\tR-总个数\tGr-总个数\tGb-总个数\tB-总个数\tR-RatioMin\tR-RatioMax\tGr-RatioMin\tGr-RatioMax\tGb-RatioMin\tGb-RatioMax\tB-RatioMin\tB-RatioMax\tR-SymColMax差值\tGr-SymColMax差值\tGb-SymColMax差值\tB-SymColMax差值\tR-SymRowMax差值\tGr-SymRowMax差值\tGb-SymRowMax差值\tB-SymRowMax差值"));
	FileHeaderSum += tempHeaderSum;
	tempContentSum.Format(_T("%s# \t%04d%02d%02d%02d%02d%02d# \t "),m_FuseID,time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	FileContentSum += tempContentSum;
	for (int i=0;i<32;i++)
	{
		tempContentSum.Format(_T("%.2f\t"),LSCValue[i]);
		FileContentSum += tempContentSum;
	}

	CStdioFile MyFileSum(saveLSCDataSumPath, CFile::modeWrite | CFile::typeText);
	int	ResSum=MyFileSum.GetLength();
	if (!ResSum)
	{
		MyFileSum.WriteString(FileHeaderSum);
		MyFileSum.WriteString(_T("\n"));
	}
	else
	{
		MyFileSum.SeekToEnd();
		MyFileSum.WriteString(_T("\n"));
	}
	MyFileSum.WriteString(FileContentSum);
	MyFileSum.Close();
	printf("Save LSC Data Sum File in %s",saveLSCDataSumPath);

	return TRUE;
}


BOOL OtpCheckAPI::SaveLSCData_LG(CString channelName , USHORT * channelData, int CenterValueEqualcount, int ErrorCountMax, int TotalErrorCount, int ErrorValueMax, float * FourthCalculateRatio, int * SymColDiff, int * SymRowDiff, wchar_t * Errormsg, int flag)
{
	/************************************************************************/
	/*						创建保存数据的文件夹							*/
	/************************************************************************/
	CString saveLSCDataPath;
	CTime time = CTime::GetCurrentTime(); 	

	CString saveDataPath = _T("D:\\Data\\") + m_StationName + _T("\\");
	CreateDirectory(saveDataPath,NULL);
	CString tempYear = _T(""),tempMonth = _T(""),tempDay = _T("");
	tempYear.Format(_T("%d"),time.GetYear());
	tempMonth.Format(_T("%d"),time.GetMonth());
	tempDay.Format(_T("%d"),time.GetDay());
	saveDataPath = saveDataPath + tempYear + tempMonth + tempDay + _T("\\");
	CreateDirectory(saveDataPath,NULL);
	if (flag == 0)
	{
		saveLSCDataPath = saveDataPath + m_FuseID + _T("_MasterLSC.xls");
	}
	else
	{
		saveLSCDataPath = saveDataPath + m_FuseID  + _T("_SlaveLSC.xls");
	}

	/************************************************************************/
	/*							创建表格文件							    */
	/************************************************************************/
	int i= 0,k = 0,j = 0;
	CString FileHeader;
	CString FileContent;
	CString tempHeader;
	CString tempContent;
	FILE *fp =_wfopen(saveLSCDataPath, _T("a"));
	if (fp == NULL)
	{
		Catstring(Errormsg,_T("Save LSC Data File Failed!"));
		return FALSE;
	}
	fclose(fp);

	FileHeader.Format(_T("\t\t\t\t\t\t\t\tAnalyze LSC Data"));

	FileContent.Format(_T("Test time:\t%04d%02d%02d%02d%02d%02d# \t\t FuseID:\t%s# \n"),time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond(),m_FuseID);

	tempContent.Format(_T("%s:\n"),channelName);
	FileContent+=tempContent;

	tempContent.Format(_T("\t"));
	FileContent+=tempContent;
	//块状打印LSC数据（17*13）
	for (int i=1;i<18;i++)
	{			
		tempContent.Format(_T("   BT%d\t"),i);
		FileContent+=tempContent;
		if (i>16)
		{
			tempContent.Format(_T("\n"));
			FileContent+=tempContent;
		}
	}

	for(int  m = 0; m<13; m++)
	{	
		tempContent.Format(_T("   BL%d\t"),m+1);
		FileContent+=tempContent;
		for (int n = 0;n<17; n++)
		{	
			tempContent.Format(_T("   %d\t"),channelData[m*17+n]);
			FileContent+=tempContent;
		}
		tempContent.Format(_T("\r"));
		FileContent+=tempContent;
	}

	tempContent.Format(_T("\r************************************************************"));
	FileContent+=tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;

	//保存①中心3*3范围内存在几个最大值
	CString temp1 =_T("①中心3*3范围内存在的最大值个数");
	int m_CenterValueEqualcount  = m_GetOTPCheckSpec.m_CenterValueEqualcount;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;
	tempContent.Format(_T("%s:%d (Spec < %d)\t"),temp1,CenterValueEqualcount,m_CenterValueEqualcount);
	FileContent += tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;
	tempContent.Format(_T("\r************************************************************"));
	FileContent+=tempContent;


	//保存②每行相邻的递减或递增误差值
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;
	temp1 =_T("②每行相邻的递减或递增最大误差值");
	int channel_firstDiff[208]	 = {0};

	int m_CurveDiff			= m_GetOTPCheckSpec.m_Curvediff;
	int m_ErrorCount		= m_GetOTPCheckSpec.m_ErrorCount;
	int m_TotalErrorCount	= m_GetOTPCheckSpec.m_TotalErrorCount;

	for (k = 0; k < 13; k++)
	{
		for (int i = 0; i<16; i++)
		{
			channel_firstDiff[k*16+i]  = channelData[k*17+i+1]  - channelData[k*17+i] ;
		}
	}

	tempContent.Format(_T("\r"));
	FileContent+=tempContent;
	tempContent.Format(_T("%s:%d (Spec < %d)\n  每行最多存在误差个数:%d (Spec < %d)\n  每Channel存在误差个数:%d (Spec < %d)"),temp1,ErrorValueMax,m_CurveDiff,ErrorCountMax,m_ErrorCount,TotalErrorCount,m_TotalErrorCount);
	FileContent += tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;
	for (k = 0; k<13; k++)
	{
		for (int i = 0;i<16; i++)
		{
			tempContent.Format(_T("%d\t"),channel_firstDiff[k*16+i]);
			FileContent+=tempContent;
		}
		tempContent.Format(_T("\r"));
		FileContent+=tempContent;
	}
	tempContent.Format(_T("\r************************************************************"));
	FileContent+=tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;


	//保存③第1列，2列，16列，17列与中心9列的比值
	temp1 =_T("③第1列，2列，16列，17列与中心9列的比值");
	char Temp_GetData[128] = {0};
	float edg_center_LimitRatio = 0,  edg_center_HighRatio = 0;
	float FourthCalculateRatioMax = 0,FourthCalculateRatioMin = 255;

	edg_center_LimitRatio = m_GetOTPCheckSpec.m_edg_center_LimitRatio;
	edg_center_HighRatio  = m_GetOTPCheckSpec.m_edg_center_HighRatio;

	for (int i=0;i<52;i++)
	{
		FourthCalculateRatioMax = max(FourthCalculateRatioMax,FourthCalculateRatio[i]);
		FourthCalculateRatioMin = min(FourthCalculateRatioMin,FourthCalculateRatio[i]);
	}

	tempContent.Format(_T("\r"));
	FileContent+=tempContent;
	tempContent.Format(_T("%s:Min:%.2f,Max:%.2f (Spec %.2f ~ %.2f)"),temp1,FourthCalculateRatioMin,FourthCalculateRatioMax,edg_center_LimitRatio,edg_center_HighRatio);
	FileContent += tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;

	for (k = 0; k<13; k++)
	{
		for (int i = 0;i<4; i++)
		{
			tempContent.Format(_T("%.2f\t"),FourthCalculateRatio[k*4+i]);
			FileContent+=tempContent;
		}
		tempContent.Format(_T("\r"));
		FileContent+=tempContent;
	}
	tempContent.Format(_T("\r************************************************************"));
	FileContent+=tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;
	tempContent.Format(_T("\n"));
	FileContent+=tempContent;

	//保存④.1对称列差值小于某个数值（如25）**************************/
	temp1 = _T("④1.对称列的差值");
	int m_MaxSymColDiff = m_GetOTPCheckSpec.m_MaxSymColDiff;
	int SymColDiffMax = 0;
	for (int i=0;i<104;i++)
	{
		SymColDiffMax = max(SymColDiffMax,SymColDiff[i]);
	}
	tempContent.Format(_T("%s:Max:%d (Spec < %d)"),temp1,SymColDiffMax,m_MaxSymColDiff);
	FileContent += tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;

	for (int i=0;i<13;i++)
	{
		for (j=0;j<8;j++)
		{
			tempContent.Format(_T("%d\t"),SymColDiff[i*8+j]);
			FileContent+=tempContent;
		}
		tempContent.Format(_T("\r"));
		FileContent+=tempContent;
	}


	tempContent.Format(_T("\r************************************************************"));
	FileContent+=tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;

	//保存④.2对称行差值小于某个数值（如25）**************************/
	temp1 = _T("④2.对称行的差值");
	int m_MaxSymRowDiff = m_GetOTPCheckSpec.m_MaxSymRowDiff;
	int SymRowDiffMax = 0;
	for (int i=0;i<102;i++)
	{
		SymRowDiffMax = max(SymRowDiffMax,SymRowDiff[i]);
	}
	tempContent.Format(_T("%s:Max:%d (Spec < %d)"),temp1,SymRowDiffMax,m_MaxSymRowDiff);
	FileContent += tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;

	for (int i=0;i<6;i++)
	{
		for (j=0;j<17;j++)
		{
			tempContent.Format(_T("%d\t"),SymRowDiff[17*i+j]);
			FileContent+=tempContent;
		}
		tempContent.Format(_T("\r"));
		FileContent+=tempContent;
	}


	tempContent.Format(_T("\r************************************************************"));
	FileContent+=tempContent;
	tempContent.Format(_T("\r"));
	FileContent+=tempContent;

	CStdioFile MyFile(saveLSCDataPath, CFile::modeWrite | CFile::typeText);
	int	Res=MyFile.GetLength();
	if (!Res)
	{
		MyFile.WriteString(FileHeader);
		MyFile.WriteString(_T("\n"));
	}
	else
	{
		MyFile.SeekToEnd();
		MyFile.WriteString(_T("\n"));
	}
	MyFile.WriteString(FileContent);
	MyFile.Close();
	printf("LSC Data analyze file save in %s",saveLSCDataPath);

	return TRUE;
}


BOOL OtpCheckAPI::CheckLSCData_LG(USHORT *RchannelData,USHORT *GrchannelData,USHORT *GbchannelData,USHORT *BchannelData,int centerValue, wchar_t * Errormsg,int flag)
{

	BOOL lscCheckResult = TRUE;
	int RCenterValueEqualcount = 0, GrCenterValueEqualcount = 0, GbCenterValueEqualcount = 0, BCenterValueEqualcount = 0;
	int i = 0,j = 0,k = 0;

	/***********************①中心范围9个点中必最大值centerValue，且管控个数**************************/
	int m_CenterValueEqualcount  = m_GetOTPCheckSpec.m_CenterValueEqualcount;
	int m_SaveLSCDataEn			 = m_GetOTPCheckSpec.m_SaveLSCALLDataEn;
	int m_SaveLSCOKDataEn		 = m_GetOTPCheckSpec.m_SaveLSCOKDataEn;
	for (int i=5;i<8;i++)
	{
		for (j=7;j<10;j++)
		{
			if (RchannelData[i*17+j] == centerValue)
			{
				RCenterValueEqualcount++;
			}
			if (GrchannelData[i*17+j] == centerValue)
			{
				GrCenterValueEqualcount++;
			}
			if (GbchannelData[i*17+j] == centerValue)
			{
				GbCenterValueEqualcount++;
			}
			if (BchannelData[i*17+j] == centerValue)
			{
				BCenterValueEqualcount++;
			}
		}
	}
	if (RCenterValueEqualcount  < 1 || RCenterValueEqualcount  > m_CenterValueEqualcount)
	{

		if (!m_SaveLSCDataEn)
		{
			Catstring(Errormsg,_T("R channel have (%d !< %d) CenterValue = %d "),RCenterValueEqualcount,m_CenterValueEqualcount,centerValue);
			return FALSE;
		} 
		else 
			lscCheckResult = FALSE;

	}
	if (GrCenterValueEqualcount  < 1 || GrCenterValueEqualcount  > m_CenterValueEqualcount)
	{
		if (!m_SaveLSCDataEn)
		{
			Catstring(Errormsg,_T("Gr channel have (%d !< %d) CenterValue = %d "),GrCenterValueEqualcount,m_CenterValueEqualcount,centerValue);
			return FALSE;
		} 
		else 
			lscCheckResult = FALSE;
	}
	if (GbCenterValueEqualcount  < 1 || GbCenterValueEqualcount  > m_CenterValueEqualcount)
	{

		if (!m_SaveLSCDataEn)
		{
			Catstring(Errormsg,_T("Gb channel have (%d !< %d) CenterValue = %d "),GbCenterValueEqualcount,m_CenterValueEqualcount,centerValue);
			return FALSE;
		} 
		else 
			lscCheckResult = FALSE;
	}
	if (BCenterValueEqualcount  < 1 || BCenterValueEqualcount  > m_CenterValueEqualcount)
	{

		if (!m_SaveLSCDataEn)
		{
			Catstring(Errormsg,_T("B channel have (%d !< %d) CenterValue = %d "),BCenterValueEqualcount,m_CenterValueEqualcount,centerValue);
			return FALSE;
		} 
		else 
			lscCheckResult = FALSE;
	}

	/***********************②从第1列到第9列依次递增，从第9列到第17列依次递减**************************/
	int m_Curvediff			= m_GetOTPCheckSpec.m_Curvediff;
	int m_EerrorCout		= m_GetOTPCheckSpec.m_ErrorCount;
	int m_TotoalErrorCout	= m_GetOTPCheckSpec.m_TotalErrorCount;

	int m_RChannelErrorCount	= 0 ,m_RChannelTotoalErrorCout	= 0 ,m_RChannelErrorCountMax	 = 0;
	int m_GrChannelErrorCount	= 0 ,m_GrChannelTotoalErrorCout	= 0 ,m_GrChannelErrorCountMax	 = 0;
	int m_GbChannelErrorCount	= 0 ,m_GbChannelTotoalErrorCout	= 0 ,m_GbChannelErrorCountMax	 = 0;
	int m_BChannelErrorCount	= 0 ,m_BChannelTotoalErrorCout	= 0 ,m_BChannelErrorCountMax	 = 0;

	int m_RChannelErrorValue[208]  = {0};
	int m_GrChannelErrorValue[208] = {0};
	int m_GbChannelErrorValue[208] = {0};
	int m_BChannelErrorValue[208]  = {0};

	int m_RChannelErrorValueMax = 0;
	int m_GrChannelErrorValueMax = 0;
	int m_GbChannelErrorValueMax = 0;
	int m_BChannelErrorValueMax = 0;

	for (k = 0; k<13; k++)
	{	
		m_RChannelErrorCount  = 0;
		m_GrChannelErrorCount = 0;
		m_GbChannelErrorCount = 0;
		m_BChannelErrorCount  = 0;

		for (int i=8,j=9;i>0,j<17;i--,j++)
		{
			//R Channel
			if (RchannelData[k*17+i-1] > RchannelData[k*17+i])
			{
				m_RChannelTotoalErrorCout ++;
				m_RChannelErrorCount ++;
				if((RchannelData[k*17+i-1]>(RchannelData[k*17+i]+m_Curvediff)))
				{
					Catstring(Errormsg,_T("R_LSC[%d,%d]:%d !< %d=R_LSC[%d,%d]"),k+1,i,RchannelData[k*17+i-1],RchannelData[k*17+i],k+1,i+1);
					if (!m_SaveLSCDataEn)
					{
						return FALSE;
					} 
					else 
						lscCheckResult = FALSE;
				}
				m_RChannelErrorValue[m_RChannelTotoalErrorCout] = RchannelData[k*17+i-1] - RchannelData[k*17+i];
			}
			if (RchannelData[k*17+j-1] < RchannelData[k*17+j])
			{
				m_RChannelTotoalErrorCout ++;
				m_RChannelErrorCount ++;
				if(((m_Curvediff+RchannelData[k*17+j-1])<RchannelData[k*17+j]))
				{	

					if (!m_SaveLSCDataEn)
					{
						Catstring(Errormsg,_T("R_LSC[%d,%d]:%d !> %d=R_LSC[%d,%d]"),k+1,j,RchannelData[k*17+j-1],RchannelData[k*17+j],k+1,j+1);
						return FALSE;
					} 
					else 
						lscCheckResult = FALSE;	
				}
				m_RChannelErrorValue[m_RChannelTotoalErrorCout] = RchannelData[k*17+j] - RchannelData[k*17+j-1];
			}
			m_RChannelErrorValueMax = max(m_RChannelErrorValueMax,m_RChannelErrorValue[m_RChannelTotoalErrorCout]);
			m_RChannelErrorCountMax = max(m_RChannelErrorCountMax,m_RChannelErrorCount);

			//Gr Channel**************************************

			if (GrchannelData[k*17+i-1] > GrchannelData[k*17+i])
			{
				m_GrChannelTotoalErrorCout ++;
				m_GrChannelErrorCount ++;
				if((GrchannelData[k*17+i-1]>(GrchannelData[k*17+i]+m_Curvediff)))
				{	

					if (!m_SaveLSCDataEn)
					{
						Catstring(Errormsg,_T("Gr_LSC[%d,%d]:%d !< %d=Gr_LSC[%d,%d]"),k+1,i,GrchannelData[k*17+i-1],GrchannelData[k*17+i],k+1,i+1);
						return FALSE;
					} 
					else 
						lscCheckResult = FALSE;
				}
				m_GrChannelErrorValue[m_GrChannelTotoalErrorCout] = GrchannelData[k*17+i-1] - GrchannelData[k*17+i];
			}

			if (GrchannelData[k*17+j-1] < GrchannelData[k*17+j])
			{
				m_GrChannelTotoalErrorCout ++;
				m_GrChannelErrorCount ++;
				if(((m_Curvediff+GrchannelData[k*17+j-1])<GrchannelData[k*17+j]))
				{	

					if (!m_SaveLSCDataEn)
					{
						Catstring(Errormsg,_T("Gr_LSC[%d,%d]:%d !> %d=Gr_LSC[%d,%d]"),k+1,j,GrchannelData[k*17+j-1],GrchannelData[k*17+j],k+1,j+1);
						return FALSE;
					} 
					else 
						lscCheckResult = FALSE;
				}
				m_GrChannelErrorValue[m_GrChannelTotoalErrorCout] = GrchannelData[k*17+j] - GrchannelData[k*17+j-1];
			}
			m_GrChannelErrorValueMax = max(m_GrChannelErrorValueMax,m_GrChannelErrorValue[m_GrChannelTotoalErrorCout]);
			m_GrChannelErrorCountMax = max(m_GrChannelErrorCountMax,m_GrChannelErrorCount);


			//Gb Channel*********************************************
			if (GbchannelData[k*17+i-1] > GbchannelData[k*17+i])
			{
				m_GbChannelTotoalErrorCout ++;
				m_GbChannelErrorCount ++;
				if((GbchannelData[k*17+i-1]>(GbchannelData[k*17+i]+m_Curvediff)))
				{

					if (!m_SaveLSCDataEn)
					{
						Catstring(Errormsg,_T("Gb_LSC[%d,%d]:%d !< %d=Gb_LSC[%d,%d]"),k+1,i,GbchannelData[k*17+i-1],GbchannelData[k*17+i],k+1,i+1);
						return FALSE;
					} 
					else 
						lscCheckResult = FALSE;
				}
				m_GbChannelErrorValue[m_GbChannelTotoalErrorCout] = GbchannelData[k*17+i-1] - GbchannelData[k*17+i];
			}

			if (GbchannelData[k*17+j-1] < GbchannelData[k*17+j])
			{
				m_GbChannelTotoalErrorCout ++;
				m_GbChannelErrorCount ++;
				if(((m_Curvediff+GbchannelData[k*17+j-1])<GbchannelData[k*17+j]))
				{

					if (!m_SaveLSCDataEn)
					{
						Catstring(Errormsg,_T("Gb_LSC[%d,%d]:%d !> %d=Gb_LSC[%d,%d]"),k+1,j,GbchannelData[k*17+j-1],GbchannelData[k*17+j],k+1,j+1);
						return FALSE;
					} 
					else 
						lscCheckResult = FALSE;
				}
				m_GbChannelErrorValue[m_GbChannelTotoalErrorCout] = GbchannelData[k*17+j] - GbchannelData[k*17+j-1];
			}
			m_GbChannelErrorValueMax = max(m_GbChannelErrorValueMax,m_GbChannelErrorValue[m_GbChannelTotoalErrorCout]);
			m_GbChannelErrorCountMax = max(m_GbChannelErrorCountMax,m_GbChannelErrorCount);


			//B Channel***********************************
			if (BchannelData[k*17+i-1] > BchannelData[k*17+i])
			{
				m_BChannelTotoalErrorCout ++;
				m_BChannelErrorCount ++;
				if((BchannelData[k*17+i-1]>(BchannelData[k*17+i]+m_Curvediff)))
				{

					if (!m_SaveLSCDataEn)
					{
						Catstring(Errormsg,_T("B_LSC[%d,%d]:%d !< %d=B_LSC[%d,%d]"),k+1,i,BchannelData[k*17+i-1],BchannelData[k*17+i],k+1,i+1);
						return FALSE;
					} 
					else 
						lscCheckResult = FALSE;
				}
				m_BChannelErrorValue[m_BChannelTotoalErrorCout] = BchannelData[k*17+i-1] - BchannelData[k*17+i];
			}

			if (BchannelData[k*17+j-1] <BchannelData[k*17+j])
			{
				m_BChannelTotoalErrorCout ++;
				m_BChannelErrorCount ++;
				if(((m_Curvediff+BchannelData[k*17+j-1])<BchannelData[k*17+j]))
				{

					if (!m_SaveLSCDataEn)
					{
						Catstring(Errormsg,_T("B_LSC[%d,%d]:%d !> %d=B_LSC[%d,%d]"),k+1,j,BchannelData[k*17+j-1],BchannelData[k*17+j],k+1,j+1);
						return FALSE;
					} 
					else 
						lscCheckResult = FALSE;
				}
				m_BChannelErrorValue[m_BChannelTotoalErrorCout] = BchannelData[k*17+j] - BchannelData[k*17+j-1];
			}
			m_BChannelErrorValueMax = max(m_BChannelErrorValueMax,m_BChannelErrorValue[m_BChannelTotoalErrorCout]);
			m_BChannelErrorCountMax = max(m_BChannelErrorCountMax,m_BChannelErrorCount);
		}
		//卡控每条曲线错误数是否超规
		if (m_RChannelErrorCount > m_EerrorCout) 
		{

			if (!m_SaveLSCDataEn)
			{
				Catstring(Errormsg,_T("RChannelErrorCount:%d !< %d ErrorCount \n Curve Test NG"),m_RChannelErrorCount,m_EerrorCout);
				return FALSE;
			} 
			else 
				lscCheckResult = FALSE;
		}
		if (m_GrChannelErrorCount > m_EerrorCout) 
		{

			if (!m_SaveLSCDataEn)
			{
				Catstring(Errormsg,_T("GrChannelErrorCount:%d !< %d ErrorCount \n Curve Test NG"),m_GrChannelErrorCount,m_EerrorCout);
				return FALSE;
			} 
			else 
				lscCheckResult = FALSE;
		}
		if (m_GbChannelErrorCount > m_EerrorCout) 
		{

			if (!m_SaveLSCDataEn)
			{
				Catstring(Errormsg,_T("GbChannelErrorCount:%d !< %d ErrorCount \n Curve Test NG"),m_GbChannelErrorCount,m_EerrorCout);
				return FALSE;
			} 
			else 
				lscCheckResult = FALSE;
		}
		if (m_BChannelErrorCount > m_EerrorCout) 
		{

			if (!m_SaveLSCDataEn)
			{
				Catstring(Errormsg,_T("BChannelErrorCount:%d !< %d ErrorCount \n Curve Test NG"),m_BChannelErrorCount,m_EerrorCout);
				return FALSE;
			} 
			else 
				lscCheckResult = FALSE;
		}
	}

	// 卡控每条channel的总的趋势错误数是否超规
	if (m_RChannelTotoalErrorCout > m_TotoalErrorCout) 
	{

		if (!m_SaveLSCDataEn)
		{
			Catstring(Errormsg,_T("RChannelTotoalErrorCout:%d !< %d TotoalErrorCout \n Curve Test NG"),m_RChannelTotoalErrorCout,m_TotoalErrorCout);
			return FALSE;
		} 
		else 
			lscCheckResult = FALSE;
	}
	if (m_GrChannelTotoalErrorCout > m_TotoalErrorCout) 
	{

		if (!m_SaveLSCDataEn)
		{
			Catstring(Errormsg,_T("GrChannelTotoalErrorCout:%d !< %d TotoalErrorCout \n Curve Test NG"),m_GrChannelTotoalErrorCout,m_TotoalErrorCout);
			return FALSE;
		} 
		else 
			lscCheckResult = FALSE;
	}
	if (m_GbChannelTotoalErrorCout > m_TotoalErrorCout) 
	{

		if (!m_SaveLSCDataEn)
		{
			Catstring(Errormsg,_T("GbChannelTotoalErrorCout:%d !< %d TotoalErrorCoutt \n Curve Test NG"),m_GbChannelTotoalErrorCout,m_TotoalErrorCout);
			return FALSE;
		} 
		else 
			lscCheckResult = FALSE;
	}
	if (m_BChannelTotoalErrorCout > m_TotoalErrorCout) 
	{

		if (!m_SaveLSCDataEn)
		{
			Catstring(Errormsg,_T("BChannelTotoalErrorCout:%d !< %d TotoalErrorCout \n Curve Test NG"),m_BChannelTotoalErrorCout,m_TotoalErrorCout);
			return FALSE;
		} 
		else 
			lscCheckResult = FALSE;
	}

	/***********************③第1列，2列，16列，17列与中心9列的比值在一定的范围内(如：0.4~1)**************************/
	char Temp_GetData[128] = {0};
	float edg_center_LimitRatio = 0,  edg_center_HighRatio = 0;

	edg_center_LimitRatio = m_GetOTPCheckSpec.m_edg_center_LimitRatio;
	edg_center_HighRatio  = m_GetOTPCheckSpec.m_edg_center_HighRatio;

	float edg_center_Ratio[13][4]			= {0.0};

	float RFourthCalculateRatio[52]			= {0.0};
	float GrFourthCalculateRatio[52]		= {0.0};
	float GbFourthCalculateRatio[52]		= {0.0};
	float BFourthCalculateRatio[52]			= {0.0};

	for (k = 0; k<13; k++)
	{
		//R channel
		edg_center_Ratio[k][0]		= float(RchannelData[k*17])   /float(RchannelData[k*17+8]);
		edg_center_Ratio[k][1] 		= float(RchannelData[k*17+1]) /float(RchannelData[k*17+8]);
		edg_center_Ratio[k][2] 		= float(RchannelData[k*17+15])/float(RchannelData[k*17+8]);
		edg_center_Ratio[k][3]		= float(RchannelData[k*17+16])/float(RchannelData[k*17+8]);

		for (int i = 0; i< 4 ; i++)
		{
			if (edg_center_Ratio[k][i] < edg_center_LimitRatio || edg_center_Ratio[k][i] > edg_center_HighRatio)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("R channel edg_center_Ratio = %.4f is not in （%.4f ~ %.4f）"),edg_center_Ratio[k][i],edg_center_LimitRatio,edg_center_HighRatio);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
		}

		//获取Rchannel存储数据：
		RFourthCalculateRatio[k]			= edg_center_Ratio[k][0];
		RFourthCalculateRatio[k+13]			= edg_center_Ratio[k][1];
		RFourthCalculateRatio[k+26]			= edg_center_Ratio[k][2];
		RFourthCalculateRatio[k+39]			= edg_center_Ratio[k][3];

		//Gr channel
		edg_center_Ratio[k][0]		= float(GrchannelData[k*17])   /float(GrchannelData[k*17+8]);
		edg_center_Ratio[k][1] 		= float(GrchannelData[k*17+1]) /float(GrchannelData[k*17+8]);
		edg_center_Ratio[k][2] 		= float(GrchannelData[k*17+15])/float(GrchannelData[k*17+8]);
		edg_center_Ratio[k][3]		= float(GrchannelData[k*17+16])/float(GrchannelData[k*17+8]);

		for (int i = 0; i< 4 ; i++)
		{
			if (edg_center_Ratio[k][i] < edg_center_LimitRatio || edg_center_Ratio[k][i] > edg_center_HighRatio)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("Gr channel edg_center_Ratio = %.4f is not in （%.4f ~ %.4f）"),edg_center_Ratio[k][i],edg_center_LimitRatio,edg_center_HighRatio);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
		}

		//获取GRchannel存储数据：
		GrFourthCalculateRatio[k]				= edg_center_Ratio[k][0];
		GrFourthCalculateRatio[k+13]			= edg_center_Ratio[k][1];
		GrFourthCalculateRatio[k+26]			= edg_center_Ratio[k][2];
		GrFourthCalculateRatio[k+39]			= edg_center_Ratio[k][3];

		//Gb channel
		edg_center_Ratio[k][0]		= float(GbchannelData[k*17])   /float(GbchannelData[k*17+8]);
		edg_center_Ratio[k][1] 		= float(GbchannelData[k*17+1]) /float(GbchannelData[k*17+8]);
		edg_center_Ratio[k][2] 		= float(GbchannelData[k*17+15])/float(GbchannelData[k*17+8]);
		edg_center_Ratio[k][3]		= float(GbchannelData[k*17+16])/float(GbchannelData[k*17+8]);

		for (int i = 0; i< 4 ; i++)
		{
			if (edg_center_Ratio[k][i] < edg_center_LimitRatio || edg_center_Ratio[k][i] > edg_center_HighRatio)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("Gb channel edg_center_Ratio = %.4f is not in （%.4f ~ %.4f）"),edg_center_Ratio[k][i],edg_center_LimitRatio,edg_center_HighRatio);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
		}

		//获取Gbchannel存储数据：
		GbFourthCalculateRatio[k]				= edg_center_Ratio[k][0];
		GbFourthCalculateRatio[k+13]			= edg_center_Ratio[k][1];
		GbFourthCalculateRatio[k+26]			= edg_center_Ratio[k][2];
		GbFourthCalculateRatio[k+39]			= edg_center_Ratio[k][3];


		//B channel
		edg_center_Ratio[k][0]		= float(BchannelData[k*17])   /float(BchannelData[k*17+8]);
		edg_center_Ratio[k][1] 		= float(BchannelData[k*17+1]) /float(BchannelData[k*17+8]);
		edg_center_Ratio[k][2] 		= float(BchannelData[k*17+15])/float(BchannelData[k*17+8]);
		edg_center_Ratio[k][3]		= float(BchannelData[k*17+16])/float(BchannelData[k*17+8]);

		for (int i = 0; i< 4 ; i++)
		{
			if (edg_center_Ratio[k][i] < edg_center_LimitRatio || edg_center_Ratio[k][i] > edg_center_HighRatio)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("B channel edg_center_Ratio = %.4f is not in （%.4f ~ %.4f）"),edg_center_Ratio[k][i],edg_center_LimitRatio,edg_center_HighRatio);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
		}

		//获取Gbchannel存储数据：
		BFourthCalculateRatio[k]			= edg_center_Ratio[k][0];
		BFourthCalculateRatio[k+13]			= edg_center_Ratio[k][1];
		BFourthCalculateRatio[k+26]			= edg_center_Ratio[k][2];
		BFourthCalculateRatio[k+39]			= edg_center_Ratio[k][3];
	}	

	//获取各通道最大最小比值
	float RFourthCalculateRatioMax = 0.0,RFourthCalculateRatioMin = 1.0;
	float GrFourthCalculateRatioMax= 0.0,GrFourthCalculateRatioMin= 1.0;
	float GbFourthCalculateRatioMax= 0.0,GbFourthCalculateRatioMin= 1.0;
	float BFourthCalculateRatioMax = 0.0,BFourthCalculateRatioMin = 1.0;
	for (int i = 0; i < 52; i++)
	{
		RFourthCalculateRatioMax = max(RFourthCalculateRatioMax,RFourthCalculateRatio[i]);
		RFourthCalculateRatioMin = min(RFourthCalculateRatioMin,RFourthCalculateRatio[i]);

		GrFourthCalculateRatioMax = max(GrFourthCalculateRatioMax,GrFourthCalculateRatio[i]);
		GrFourthCalculateRatioMin = min(GrFourthCalculateRatioMin,GrFourthCalculateRatio[i]);

		GbFourthCalculateRatioMax = max(GbFourthCalculateRatioMax,GbFourthCalculateRatio[i]);
		GbFourthCalculateRatioMin = min(GbFourthCalculateRatioMin,GbFourthCalculateRatio[i]);

		BFourthCalculateRatioMax = max(BFourthCalculateRatioMax,BFourthCalculateRatio[i]);
		BFourthCalculateRatioMin = min(BFourthCalculateRatioMin,BFourthCalculateRatio[i]);
	}

	/***********************④.1对称列差值小于某个数值（如25）**************************/	
	int m_MaxSymColDiff = m_GetOTPCheckSpec.m_MaxSymColDiff;
	//对称列差值
	int RSymColDiff[8*13]  = {0};
	int GrSymColDiff[8*13] = {0};
	int GbSymColDiff[8*13] = {0};
	int BSymColDiff[8*13]  = {0};

	for (int i=0;i<13;i++)
	{
		for (j=0;j<8;j++)
		{
			//R channel
			RSymColDiff[8*i+j]  = abs(RchannelData[i*17+j]  - RchannelData[i*17+16-j] );
			if (RSymColDiff[8*i+j] > m_MaxSymColDiff)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("******R Channel NG******* "));
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,j+1,RchannelData[i*17+j]);
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,17-j,RchannelData[i*17+16-j]);
					Catstring(Errormsg,_T("The Diff is %d !< %d"),RSymColDiff[8*i+j],m_MaxSymColDiff);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
			//Gr channel
			GrSymColDiff[8*i+j] = abs(GrchannelData[i*17+j] - GrchannelData[i*17+16-j]);
			if (GrSymColDiff[8*i+j] > m_MaxSymColDiff)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("******Gr Channel NG******* "));
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,j+1,GrchannelData[i*17+j]);
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,17-j,GrchannelData[i*17+16-j]);
					Catstring(Errormsg,_T("The Diff is %d !< %d"),GrSymColDiff[8*i+j],m_MaxSymColDiff);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
			//Gb channel
			GbSymColDiff[8*i+j] = abs(GbchannelData[i*17+j] - GbchannelData[i*17+16-j]);
			if (GbSymColDiff[8*i+j] > m_MaxSymColDiff)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("******Gb Channel NG******* "));
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,j+1,GbchannelData[i*17+j]);
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,17-j,GbchannelData[i*17+16-j]);
					Catstring(Errormsg,_T("The Diff is %d !< %d"),GbSymColDiff[8*i+j],m_MaxSymColDiff);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
			//B channel
			BSymColDiff[8*i+j]  = abs(BchannelData[i*17+j]  - BchannelData[i*17+16-j] );
			if (BSymColDiff[8*i+j] > m_MaxSymColDiff)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("******B Channel NG******* "));
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,j+1,BchannelData[i*17+j]);
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,17-j,BchannelData[i*17+16-j]);
					Catstring(Errormsg,_T("The Diff is %d !< %d"),BSymColDiff[8*i+j],m_MaxSymColDiff);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
		}
	}

	//获取对称列最大最小差值
	int RSymColDiffMax = 0;
	int GrSymColDiffMax= 0;
	int GbSymColDiffMax= 0;
	int BSymColDiffMax = 0;

	for (int i = 0; i < 104; i++)
	{
		RSymColDiffMax = max(RSymColDiffMax,RSymColDiff[i]);

		GrSymColDiffMax = max(GrSymColDiffMax,GrSymColDiff[i]);

		GbSymColDiffMax = max(GbSymColDiffMax,GbSymColDiff[i]);

		BSymColDiffMax = max(BSymColDiffMax,BSymColDiff[i]);
	}

	/***********************④.2对称行差值小于某个数值（如25）**************************/	
	//对称行差值
	int m_MaxSymRowDiff = m_GetOTPCheckSpec.m_MaxSymRowDiff;
	int RSymRowDiff[17*6]  = {0};
	int GrSymRowDiff[17*6] = {0};
	int GbSymRowDiff[17*6] = {0};
	int BSymRowDiff[17*6]  = {0};

	for (int i=0;i<6;i++)
	{
		for (j=0;j<17;j++)
		{
			//R channel
			RSymRowDiff[17*i+j]  = abs(RchannelData[i*17+j]  - RchannelData[(12-i)*17+j] );
			if (RSymRowDiff[17*i+j] > m_MaxSymRowDiff)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("******R Channel NG******* "));
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,j+1,RchannelData[i*17+j]);
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),13-i,j+1,RchannelData[(12-i)*17+j]);
					Catstring(Errormsg,_T("The Diff is %d !< %d"),RSymRowDiff[17*i+j],m_MaxSymRowDiff);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
			//Gr channel
			GrSymRowDiff[17*i+j]  = abs(GrchannelData[i*17+j]  - GrchannelData[(12-i)*17+j] );
			if (GrSymRowDiff[17*i+j] > m_MaxSymRowDiff)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("******Gr Channel NG******* "));
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,j+1,GrchannelData[i*17+j]);
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),13-i,j+1,GrchannelData[(12-i)*17+j]);
					Catstring(Errormsg,_T("The Diff is %d !< %d"),GrSymRowDiff[17*i+j],m_MaxSymRowDiff);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
			//Gb channel
			GbSymRowDiff[17*i+j]  = abs(GbchannelData[i*17+j]  - GbchannelData[(12-i)*17+j] );
			if (GbSymRowDiff[17*i+j] > m_MaxSymRowDiff)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("******Gb Channel NG******* "));
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,j+1,GbchannelData[i*17+j]);
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),13-i,j+1,GbchannelData[(12-i)*17+j]);
					Catstring(Errormsg,_T("The Diff is %d !< %d"),GbSymRowDiff[17*i+j],m_MaxSymRowDiff);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
			//B channel
			BSymRowDiff[17*i+j]  = abs(BchannelData[i*17+j]  - BchannelData[(12-i)*17+j] );
			if (BSymRowDiff[17*i+j] > m_MaxSymRowDiff)
			{

				if (!m_SaveLSCDataEn)
				{
					Catstring(Errormsg,_T("******B Channel NG******* "));
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),i+1,j+1,BchannelData[i*17+j]);
					Catstring(Errormsg,_T("Line %d  ~ Column %d data is %d "),13-i,j+1,BchannelData[(12-i)*17+j]);
					Catstring(Errormsg,_T("The Diff is %d !< %d"),BSymRowDiff[17*i+j],m_MaxSymRowDiff);
					return FALSE;
				} 
				else 
					lscCheckResult = FALSE;
			}
		}
	}

	//获取对称行最大最小差值
	int RSymRowDiffMax = 0;
	int GrSymRowDiffMax= 0;
	int GbSymRowDiffMax= 0;
	int BSymRowDiffMax = 0;

	for (int i = 0; i < 102; i++)
	{
		RSymRowDiffMax = max(RSymRowDiffMax,RSymRowDiff[i]);

		GrSymRowDiffMax= max(GrSymRowDiffMax,GrSymRowDiff[i]);

		GbSymRowDiffMax= max(GbSymRowDiffMax,GbSymRowDiff[i]);

		BSymRowDiffMax = max(BSymRowDiffMax,BSymRowDiff[i]);
	}

	if (m_SaveLSCDataEn || m_SaveLSCOKDataEn)
	{		
		BOOL lscSaveResult_R = TRUE,lscSaveResult_Gr = TRUE,lscSaveResult_Gb= TRUE,lscSaveResult_B = TRUE,lscSaveResult_Sum = TRUE;
		if (flag == 0)
		{
			lscSaveResult_R = SaveLSCData_LG(_T("RChannel"),RchannelData,RCenterValueEqualcount,m_RChannelErrorCountMax,m_RChannelTotoalErrorCout,m_RChannelErrorValueMax,RFourthCalculateRatio,RSymColDiff,RSymRowDiff,Errormsg,0);
			lscSaveResult_Gr= SaveLSCData_LG(_T("GrChannel"),GrchannelData,GrCenterValueEqualcount,m_GrChannelErrorCountMax,m_GrChannelTotoalErrorCout,m_GrChannelErrorValueMax,GrFourthCalculateRatio,GrSymColDiff,GrSymRowDiff,Errormsg,0);
			lscSaveResult_Gb= SaveLSCData_LG(_T("GbChannel"),GbchannelData,GbCenterValueEqualcount,m_GbChannelErrorCountMax,m_GbChannelTotoalErrorCout,m_GbChannelErrorValueMax,GbFourthCalculateRatio,GbSymColDiff,GbSymRowDiff,Errormsg,0);
			lscSaveResult_B = SaveLSCData_LG(_T("BChannel"),BchannelData,BCenterValueEqualcount,m_BChannelErrorCountMax,m_BChannelTotoalErrorCout,m_BChannelErrorValueMax,BFourthCalculateRatio,BSymColDiff,BSymRowDiff,Errormsg,0);
		}
		else
		{
			lscSaveResult_R = SaveLSCData_LG(_T("RChannel"),RchannelData,RCenterValueEqualcount,m_RChannelErrorCountMax,m_RChannelTotoalErrorCout,m_RChannelErrorValueMax,RFourthCalculateRatio,RSymColDiff,RSymRowDiff,Errormsg,1);
			lscSaveResult_Gr= SaveLSCData_LG(_T("GrChannel"),GrchannelData,GrCenterValueEqualcount,m_GrChannelErrorCountMax,m_GrChannelTotoalErrorCout,m_GrChannelErrorValueMax,GrFourthCalculateRatio,GrSymColDiff,GrSymRowDiff,Errormsg,1);
			lscSaveResult_Gb= SaveLSCData_LG(_T("GbChannel"),GbchannelData,GbCenterValueEqualcount,m_GbChannelErrorCountMax,m_GbChannelTotoalErrorCout,m_GbChannelErrorValueMax,GbFourthCalculateRatio,GbSymColDiff,GbSymRowDiff,Errormsg,1);
			lscSaveResult_B = SaveLSCData_LG(_T("BChannel"),BchannelData,BCenterValueEqualcount,m_BChannelErrorCountMax,m_BChannelTotoalErrorCout,m_BChannelErrorValueMax,BFourthCalculateRatio,BSymColDiff,BSymRowDiff,Errormsg,1);
		}


		float LSCValue[32] = {0};
		LSCValue[0] = RCenterValueEqualcount;
		LSCValue[1] = GrCenterValueEqualcount;
		LSCValue[2] = GbCenterValueEqualcount;
		LSCValue[3] = BCenterValueEqualcount;
		LSCValue[4] = m_RChannelErrorValueMax;
		LSCValue[5] = m_GrChannelErrorValueMax;
		LSCValue[6] = m_GbChannelErrorValueMax;
		LSCValue[7] = m_BChannelErrorValueMax;
		LSCValue[8] = m_RChannelErrorCountMax;
		LSCValue[9] = m_GrChannelErrorCountMax;
		LSCValue[10]= m_GbChannelErrorCountMax;
		LSCValue[11]= m_BChannelErrorCountMax;
		LSCValue[12]= m_RChannelTotoalErrorCout;
		LSCValue[13]= m_GrChannelTotoalErrorCout;
		LSCValue[14]= m_GbChannelTotoalErrorCout;
		LSCValue[15]= m_BChannelTotoalErrorCout;
		LSCValue[16]= RFourthCalculateRatioMin;
		LSCValue[17]= RFourthCalculateRatioMax;
		LSCValue[18]= GrFourthCalculateRatioMin;
		LSCValue[19]= GrFourthCalculateRatioMax;
		LSCValue[20]= GbFourthCalculateRatioMin;
		LSCValue[21]= GbFourthCalculateRatioMax;
		LSCValue[22]= BFourthCalculateRatioMin;
		LSCValue[23]= BFourthCalculateRatioMax;
		LSCValue[24]= RSymColDiffMax;
		LSCValue[25]= GrSymColDiffMax;
		LSCValue[26]= GbSymColDiffMax;
		LSCValue[27]= BSymColDiffMax;
		LSCValue[28]= RSymRowDiffMax;
		LSCValue[29]= GrSymRowDiffMax;
		LSCValue[30]= GbSymRowDiffMax;
		LSCValue[31]= BSymRowDiffMax;

		if (flag == 0)
		{
			lscSaveResult_Sum = SaveLSCDataSum_LG(LSCValue,Errormsg,0);
		}
		else
		{
			lscSaveResult_Sum = SaveLSCDataSum_LG(LSCValue,Errormsg,1);
		}


		if (lscSaveResult_R == FALSE || lscSaveResult_Gr == FALSE || lscSaveResult_Gb == FALSE || lscSaveResult_B == FALSE || lscSaveResult_Sum == FALSE)
		{
			Catstring(Errormsg,_T("LSC data file save failed!"));
			lscCheckResult = FALSE;
		}

		if (lscCheckResult == FALSE)
		{
			Catstring(Errormsg,_T("LSC data Check failed!"));
			return FALSE;
		}
	} 

	return TRUE;
}

BOOL OtpCheckAPI::LensVerificationLib(CString str,USHORT *Data,wchar_t *sErrorMsg,BOOL isProject1,BOOL isWide)
{
	int nWidthCount = 17;
	int nHeightCount= 13;

	int i,j,k;
	int nCount = 0;
	int nValue = 0;

	int	ToleranceValueError = 10;
	int	ToleranceCountError = 10;

	if (isProject1)
	{
		ToleranceValueError = m_GetOTPCheckSpec.m_ToleranceValueError;
		ToleranceCountError = m_GetOTPCheckSpec.m_ToleranceCountError;
	}
	else
	{
		ToleranceValueError = m_GetOTPCheckSpec1.m_ToleranceValueError;
		ToleranceCountError = m_GetOTPCheckSpec1.m_ToleranceCountError;
	}

	if (isWide)
	{
		if (Data[110] != 1023)
		{
			Catstring(sErrorMsg,str +_T(":中心Block LSC数据不为1023!"));
			return FALSE;
		}
	}

	for (int i=(nWidthCount/2),j=(nWidthCount/2);i>0,j<(nWidthCount-1);i--,j++)
	{
		for (k=0;k<nHeightCount;k++)
		{
			if((Data[k*nWidthCount+i-1]- ToleranceValueError)> Data[k*nWidthCount+i])
			{
				nCount++;
				if ((Data[k*nWidthCount+i-1]-Data[k*nWidthCount+i])>nValue)
				{
					nValue = Data[k*nWidthCount+i-1]-Data[k*nWidthCount+i];
				}
			}
			if((Data[k*nWidthCount+j+1]- ToleranceValueError)> Data[k*nWidthCount+j])
			{
				nCount++;
				if ((Data[k*nWidthCount+j+1]-Data[k*nWidthCount+j])>nValue)
				{
					nValue = Data[k*nWidthCount+j+1]-Data[k*nWidthCount+j];
				}
			}
		}
	}

	for (int i=(nHeightCount/2),j=(nHeightCount/2);i>0,j<(nHeightCount-1);i--,j++)
	{
		for (k=0;k<nWidthCount;k++)
		{
			if((Data[(i-1)*nWidthCount+k]-ToleranceValueError)> Data[i*nWidthCount+k])
			{
				nCount++;
				if ((Data[(i-1)*nWidthCount+k]-Data[i*nWidthCount+k])>nValue)
				{
					nValue = Data[(i-1)*nWidthCount+k]-Data[i*nWidthCount+k];
				}
			}
			if((Data[(j+1)*nWidthCount+k]-ToleranceValueError)> Data[j*nWidthCount+k])
			{
				nCount++;
				if ((Data[(j+1)*nWidthCount+k]-Data[j*nWidthCount+k])>nValue)
				{
					nValue = Data[(j+1)*nWidthCount+k]-Data[j*nWidthCount+k];
				}
			}
		}
	}

	if (nCount > ToleranceCountError)
	{
		Catstring(sErrorMsg,str+_T(":LSC Error Count %d"),nCount);
		Catstring(sErrorMsg,str+_T(":LSC Error Value %d"),nValue);
		Catstring(sErrorMsg,str +_T("Channel 数据分布曲线异常!"));
		return FALSE;
	}
	else
	{
		//Catstring(sErrorMsg,str+_T("Channel LSC 数据正确\n"));
	}

	return TRUE;
}
BOOL OtpCheckAPI::LSCDataCheck(USHORT *rch,USHORT *grch,USHORT *gbch,USHORT *bch,wchar_t * ErrorMsg)
{
	////////////////////////////////////////////////
	FILE *fp = NULL;

	CString sfilename = _T("");
	CString sTmp      = _T("");
	CString sfileext  = _T("");
	CString sfilter   = _T("Ini File(*.txt)|*.txt");

	CFileFind ff;

	CString SensorINI = GetModulePath() + _T("\\Standard.ini");  // 设定参数文档路径

	sfilename.Format(SensorINI);

	fp = _wfopen(sfilename, _T("a"));
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeRead | CFile::typeText);  

	int StandardBuf[17*13][4] = {0};

	for( int i = 0; i < 221; i++ )
	{
		MyFile.ReadString( sTmp);
		StandardBuf[i][0]=_wtoi( sTmp);

		MyFile.ReadString( sTmp);
		StandardBuf[i][1]=_wtoi( sTmp);

		MyFile.ReadString( sTmp);
		StandardBuf[i][2]=_wtoi( sTmp);
		MyFile.ReadString( sTmp);
		StandardBuf[i][3]=_wtoi( sTmp);
	}

	MyFile.Close();

	int tempBuf[17*13][4] = {0};

	int cnt= 0;
	
	int LSC_R_Max = m_GetOTPCheckSpec.m_LSC_R_Max;
	int LSC_Gr_Max= m_GetOTPCheckSpec.m_LSC_Gr_Max;
	int LSC_Gb_Max= m_GetOTPCheckSpec.m_LSC_Gb_Max;
	int LSC_B_Max = m_GetOTPCheckSpec.m_LSC_B_Max;
	int LSC_R_Min  =m_GetOTPCheckSpec.m_LSC_R_Min;
	int LSC_Gr_Min =m_GetOTPCheckSpec.m_LSC_Gr_Min;
	int LSC_Gb_Min =m_GetOTPCheckSpec.m_LSC_Gb_Min;
	int LSC_B_Min  =m_GetOTPCheckSpec.m_LSC_B_Min;		
	int LSCDistanceSpec = m_GetOTPCheckSpec.m_LSCDistanceSpec;

	int LSCData_R_Max,LSCData_Gr_Max,LSCData_Gb_Max,LSCData_B_Max;
	LSCData_R_Max=LSCData_Gr_Max=LSCData_Gb_Max=LSCData_B_Max=0;
	int LSCData_R_Min,LSCData_Gr_Min,LSCData_Gb_Min,LSCData_B_Min;
	LSCData_R_Min=LSCData_Gr_Min=LSCData_Gb_Min=LSCData_B_Min=10000;

	for(  int i = 0; i < 221; i++ )
	{
		tempBuf[i][0] = rch[i];
		tempBuf[i][1] = grch[i];
		tempBuf[i][2] = gbch[i];
		tempBuf[i][3] = bch[i];

		if (tempBuf[i][0]<LSCData_R_Min)
		{
			LSCData_R_Min=tempBuf[i][0];
		}
		if (tempBuf[i][0]>LSCData_R_Max)
		{
			LSCData_R_Max=tempBuf[i][0];
		}
		if (tempBuf[i][1]<LSCData_Gr_Min)
		{
			LSCData_Gr_Min=tempBuf[i][1];
		}
		if (tempBuf[i][1]>LSCData_Gr_Max)
		{
			LSCData_Gr_Max=tempBuf[i][1];
		}
		if (tempBuf[i][2]<LSCData_Gb_Min)
		{
			LSCData_Gb_Min=tempBuf[i][2];
		}
		if (tempBuf[i][2]>LSCData_Gb_Max)
		{
			LSCData_Gb_Max=tempBuf[i][2];
		}
		if(tempBuf[i][3]<LSCData_B_Min)
		{
			LSCData_B_Min=tempBuf[i][3];
		}
		if (tempBuf[i][3]>LSCData_B_Max)
		{
			LSCData_B_Max=tempBuf[i][3];
		}
	}
	///////////////Save the Data///////////
	FILE  *fpn  = NULL;
	CString FileName = _T("");

	FileName.Format(_T("\\LSCData.txt"));
	FileName = GetModulePath() + FileName;

	fpn=_wfreopen(FileName,_T("w"),stdout);
	if(fpn==NULL)  
	{
		//m_pInterface->Ctrl_Addlog(CamID,_T("Open LSCData.txt fail!"),COLOR_RED,200);
		return 0;
	}

	int tempMax = 0;
	int tempDistance = 0;

	for (int i = 0; i < 17*13; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			tempDistance = abs(tempBuf[i][j] - StandardBuf[i][j]);

			if( tempDistance > tempMax )
			{
				tempMax = tempDistance;
			}		 

		}

	}
	for(  int i = 0; i < 221; i++ )
	{
		printf("%d\n",tempBuf[i][0]);
		printf("%d\n",tempBuf[i][1]);
		printf("%d\n",tempBuf[i][2]);
		printf("%d\n",tempBuf[i][3]);
	}

	printf("%d %d",LSCData_R_Min,LSCData_R_Max);
	printf("\n");
	printf("%d %d",LSCData_Gr_Min,LSCData_Gr_Max);
	printf("\n");
	printf("%d %d",LSCData_Gb_Min,LSCData_Gb_Max);
	printf("\n");
	printf("%d %d",LSCData_B_Min,LSCData_B_Max);
	printf("\n");
	printf("LSCDistanceSpec为：%d",tempMax);
	printf("\n\n");

	fclose(fpn); 		
	/////////////////////////////////////
	if (LSCData_R_Max>LSC_R_Max||LSCData_R_Min<LSC_R_Min)
	{
		wcscpy(ErrorMsg,_T("R不在范围!"));		
		return 0;
	}
	if (LSCData_Gr_Max>LSC_Gr_Max||LSCData_Gr_Min<LSC_Gr_Min)
	{
		wcscpy(ErrorMsg,_T("Gr不在范围!"));		
		return 0;
	}
	if (LSCData_Gb_Max>LSC_Gb_Max||LSCData_Gb_Min<LSC_Gb_Min)
	{
		wcscpy(ErrorMsg,_T("Gb不在范围!"));		
		return 0;
	}
	if (LSCData_B_Max>LSC_B_Max||LSCData_B_Min<LSC_B_Min)
	{
		wcscpy(ErrorMsg,_T("B不在范围!"));		
		return 0;
	}	

	if( tempMax > LSCDistanceSpec )
	{
		wcscpy(ErrorMsg,_T("Distance不在范围!"));				
		return 0;
	} 

	return TRUE;
}


BOOL OtpCheckAPI::LSCDataCheck_Project1(USHORT *rch,USHORT *grch,USHORT *gbch,USHORT *bch,wchar_t *ErrorMsg)
{
	////////////////////////////////////////////////
	FILE *fp = NULL;

	CString sfilename = _T("");
	CString sTmp      = _T("");
	CString sfileext  = _T("");
	CString sfilter   = _T("Ini File(*.txt)|*.txt");	
	CFileFind ff;

	CString SensorINI = GetModulePath() + _T("\\Standard.ini");  // 设定参数文档路径 （单体使用）	
	sfilename.Format(SensorINI);

	fp = _wfopen(sfilename, _T("a"));
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeRead | CFile::typeText);  

	int StandardBuf[17*13][4] = {0};

	for( int i = 0; i < 221; i++ )
	{
		MyFile.ReadString( sTmp);
		StandardBuf[i][0]=_wtoi( sTmp);

		MyFile.ReadString( sTmp);
		StandardBuf[i][1]=_wtoi( sTmp);

		MyFile.ReadString( sTmp);
		StandardBuf[i][2]=_wtoi( sTmp);
		MyFile.ReadString( sTmp);
		StandardBuf[i][3]=_wtoi( sTmp);
	}

	MyFile.Close();

	int tempBuf[17*13][4] = {0};		
	int cnt= 0;

	int LSCData_R_Max,LSCData_Gr_Max,LSCData_Gb_Max,LSCData_B_Max;
	LSCData_R_Max=LSCData_Gr_Max=LSCData_Gb_Max=LSCData_B_Max=0;
	int LSCData_R_Min,LSCData_Gr_Min,LSCData_Gb_Min,LSCData_B_Min;
	LSCData_R_Min=LSCData_Gr_Min=LSCData_Gb_Min=LSCData_B_Min=10000;
	
	for(  int i = 0; i < 221; i++ )
	{
		tempBuf[i][0] = rch[i];
		tempBuf[i][1] = grch[i];
		tempBuf[i][2] = gbch[i];
		tempBuf[i][3] = bch[i];

		if (tempBuf[i][0]<LSCData_R_Min)
		{
			LSCData_R_Min=tempBuf[i][0];
		}
		if (tempBuf[i][0]>LSCData_R_Max)
		{
			LSCData_R_Max=tempBuf[i][0];
		}
		if (tempBuf[i][1]<LSCData_Gr_Min)
		{
			LSCData_Gr_Min=tempBuf[i][1];
		}
		if (tempBuf[i][1]>LSCData_Gr_Max)
		{
			LSCData_Gr_Max=tempBuf[i][1];
		}
		if (tempBuf[i][2]<LSCData_Gb_Min)
		{
			LSCData_Gb_Min=tempBuf[i][2];
		}
		if (tempBuf[i][2]>LSCData_Gb_Max)
		{
			LSCData_Gb_Max=tempBuf[i][2];
		}
		if(tempBuf[i][3]<LSCData_B_Min)
		{
			LSCData_B_Min=tempBuf[i][3];
		}
		if (tempBuf[i][3]>LSCData_B_Max)
		{
			LSCData_B_Max=tempBuf[i][3];
		}
	}
	///////////////Save the Data///////////
	FILE  *fpn  = NULL;
	CString FileName = _T("");

	FileName.Format(_T("\\LSCData1.txt"));
	FileName = GetModulePath() + FileName;

	fpn=_wfreopen(FileName,_T("w"),stdout);
	if(fpn==NULL)  
	{
		Catstring(ErrorMsg,_T("Open LSCData.txt file fail!"));
		return 0;
	}

	int tempMax = 0;
	int tempDistance = 0;

	for (int i = 0; i < 17*13; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			tempDistance = abs(tempBuf[i][j] - StandardBuf[i][j]);

			if( tempDistance > tempMax )
			{
				tempMax = tempDistance;
			}		 				
		}			
	}
	for(  int i = 0; i < 221; i++ )
	{
		printf("%d\n",tempBuf[i][0]);
		printf("%d\n",tempBuf[i][1]);
		printf("%d\n",tempBuf[i][2]);
		printf("%d\n",tempBuf[i][3]);
	}

	printf("%d %d",LSCData_R_Min,LSCData_R_Max);
	printf("\n");
	printf("%d %d",LSCData_Gr_Min,LSCData_Gr_Max);
	printf("\n");
	printf("%d %d",LSCData_Gb_Min,LSCData_Gb_Max);
	printf("\n");
	printf("%d %d",LSCData_B_Min,LSCData_B_Max);
	printf("\n");
	printf("LSCDistanceSpec为：%d",tempMax);
	printf("\n\n");

	fclose(fpn); 		
	/////////////////////////////////////
	if ((LSCData_R_Max > m_GetOTPCheckSpec.m_LSC_R_Max) || (LSCData_R_Min < m_GetOTPCheckSpec.m_LSC_R_Min))
	{
		Catstring(ErrorMsg,_T("LSC R_Min为%d,R_Max为%d,不在<%d,%d>范围内!"),LSCData_R_Min,LSCData_R_Max,m_GetOTPCheckSpec.m_LSC_R_Min,m_GetOTPCheckSpec.m_LSC_R_Max);
		return 0;
	}
	if ((LSCData_Gr_Max > m_GetOTPCheckSpec.m_LSC_Gr_Max) || (LSCData_Gr_Min < m_GetOTPCheckSpec.m_LSC_Gr_Min))
	{
		Catstring(ErrorMsg,_T("LSC Gr_Min为%d,Gr_Max为%d,不在<%d,%d>范围内!"),LSCData_Gr_Min,LSCData_Gr_Max,m_GetOTPCheckSpec.m_LSC_Gr_Min,m_GetOTPCheckSpec.m_LSC_Gr_Max);
		return 0;
	}
	if ((LSCData_Gb_Max > m_GetOTPCheckSpec.m_LSC_Gb_Max) || (LSCData_Gb_Min < m_GetOTPCheckSpec.m_LSC_Gb_Min))
	{
		Catstring(ErrorMsg,_T("LSC Gb_Min为%d,Gb_Max为%d,不在<%d,%d>范围内!"),LSCData_Gb_Min,LSCData_Gb_Max,m_GetOTPCheckSpec.m_LSC_Gb_Min,m_GetOTPCheckSpec.m_LSC_Gb_Max);
		return 0;
	}
	if ((LSCData_B_Max > m_GetOTPCheckSpec.m_LSC_B_Max) || (LSCData_B_Min < m_GetOTPCheckSpec.m_LSC_B_Min))
	{
		Catstring(ErrorMsg,_T("LSC B_Min为%d,B_Max为%d,不在<%d,%d>范围内!"),LSCData_B_Min,LSCData_B_Max,m_GetOTPCheckSpec.m_LSC_B_Min,m_GetOTPCheckSpec.m_LSC_B_Max);
		return 0;
	}			
	if(tempMax > m_GetOTPCheckSpec.m_LSCDistanceSpec)
	{
		Catstring(ErrorMsg,_T("LSC Distance为%d !< %d,不在范围!"),tempMax,m_GetOTPCheckSpec.m_LSCDistanceSpec);				
		return 0;
	} 

	return 1;
}

BOOL OtpCheckAPI::LSCDataCheck_Project2(USHORT *rch,USHORT *grch,USHORT *gbch,USHORT *bch,wchar_t *ErrorMsg)
{
	////////////////////////////////////////////////
	FILE *fp = NULL;

	CString sfilename = _T("");
	CString sTmp      = _T("");
	CString sfileext  = _T("");
	CString sfilter   = _T("Ini File(*.txt)|*.txt");	
	CFileFind ff;

	CString   SensorINI = GetModulePath() + _T("\\Standard_Slave.ini");  // 设定参数文档路径

	sfilename.Format(SensorINI);

	fp = _wfopen(sfilename, _T("a"));
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeRead | CFile::typeText);  

	int StandardBuf[17*13][4] = {0};

	for( int i = 0; i < 221; i++ )
	{
		MyFile.ReadString( sTmp);
		StandardBuf[i][0]=_wtoi( sTmp);

		MyFile.ReadString( sTmp);
		StandardBuf[i][1]=_wtoi( sTmp);

		MyFile.ReadString( sTmp);
		StandardBuf[i][2]=_wtoi( sTmp);
		MyFile.ReadString( sTmp);
		StandardBuf[i][3]=_wtoi( sTmp);
	}

	MyFile.Close();

	int tempBuf[17*13][4] = {0};		
	int cnt= 0;

	int LSCData_R_Max,LSCData_Gr_Max,LSCData_Gb_Max,LSCData_B_Max;
	LSCData_R_Max=LSCData_Gr_Max=LSCData_Gb_Max=LSCData_B_Max=0;
	int LSCData_R_Min,LSCData_Gr_Min,LSCData_Gb_Min,LSCData_B_Min;
	LSCData_R_Min=LSCData_Gr_Min=LSCData_Gb_Min=LSCData_B_Min=10000;

	for(  int i = 0; i < 221; i++ )
	{
		tempBuf[i][0] = rch[i];
		tempBuf[i][1] = grch[i];
		tempBuf[i][2] = gbch[i];
		tempBuf[i][3] = bch[i];

		if (tempBuf[i][0]<LSCData_R_Min)
		{
			LSCData_R_Min=tempBuf[i][0];
		}
		if (tempBuf[i][0]>LSCData_R_Max)
		{
			LSCData_R_Max=tempBuf[i][0];
		}
		if (tempBuf[i][1]<LSCData_Gr_Min)
		{
			LSCData_Gr_Min=tempBuf[i][1];
		}
		if (tempBuf[i][1]>LSCData_Gr_Max)
		{
			LSCData_Gr_Max=tempBuf[i][1];
		}
		if (tempBuf[i][2]<LSCData_Gb_Min)
		{
			LSCData_Gb_Min=tempBuf[i][2];
		}
		if (tempBuf[i][2]>LSCData_Gb_Max)
		{
			LSCData_Gb_Max=tempBuf[i][2];
		}
		if(tempBuf[i][3]<LSCData_B_Min)
		{
			LSCData_B_Min=tempBuf[i][3];
		}
		if (tempBuf[i][3]>LSCData_B_Max)
		{
			LSCData_B_Max=tempBuf[i][3];
		}
	}
	///////////////Save the Data///////////
	FILE  *fpn  = NULL;
	CString FileName = _T("");

	FileName.Format(_T("\\LSCData2.txt"));
	FileName = GetModulePath() + FileName;

	fpn=_wfreopen(FileName,_T("w"),stdout);
	if(fpn==NULL)  
	{
		wcscpy(ErrorMsg,_T("open LSCData.txt file fail!"));
		return 0;
	}

	int tempMax = 0;
	int tempDistance = 0;

	for (int i = 0; i < 17*13; i++ )
	{
		for( int j = 0; j < 4; j++ )
		{
			tempDistance = abs(tempBuf[i][j] - StandardBuf[i][j]);

			if( tempDistance > tempMax )
			{
				tempMax = tempDistance;
			}		 				
		}			
	}
	for(  int i = 0; i < 221; i++ )
	{
		printf("%d\n",tempBuf[i][0]);
		printf("%d\n",tempBuf[i][1]);
		printf("%d\n",tempBuf[i][2]);
		printf("%d\n",tempBuf[i][3]);
	}

	printf("%d %d",LSCData_R_Min,LSCData_R_Max);
	printf("\n");
	printf("%d %d",LSCData_Gr_Min,LSCData_Gr_Max);
	printf("\n");
	printf("%d %d",LSCData_Gb_Min,LSCData_Gb_Max);
	printf("\n");
	printf("%d %d",LSCData_B_Min,LSCData_B_Max);
	printf("\n");
	printf("LSCDistanceSpec为：%d",tempMax);
	printf("\n\n");

	fclose(fpn); 		
	/////////////////////////////////////
	if ((LSCData_R_Max > m_GetOTPCheckSpec1.m_LSC_R_Max) || (LSCData_R_Min < m_GetOTPCheckSpec1.m_LSC_R_Min))
	{
		Catstring(ErrorMsg,_T("LSC R_Min为%d,R_Max为%d,不在<%d,%d>范围内!"),LSCData_R_Min,LSCData_R_Max,m_GetOTPCheckSpec1.m_LSC_R_Min,m_GetOTPCheckSpec1.m_LSC_R_Max);
		return 0;
	}
	if ((LSCData_Gr_Max > m_GetOTPCheckSpec1.m_LSC_Gr_Max) || (LSCData_Gr_Min < m_GetOTPCheckSpec1.m_LSC_Gr_Min))
	{
		Catstring(ErrorMsg,_T("LSC Gr_Min为%d,Gr_Max为%d,不在<%d,%d>范围内!"),LSCData_Gr_Min,LSCData_Gr_Max,m_GetOTPCheckSpec1.m_LSC_Gr_Min,m_GetOTPCheckSpec1.m_LSC_Gr_Max);
		return 0;
	}
	if ((LSCData_Gb_Max > m_GetOTPCheckSpec1.m_LSC_Gb_Max) || (LSCData_Gb_Min < m_GetOTPCheckSpec1.m_LSC_Gb_Min))
	{
		Catstring(ErrorMsg,_T("LSC Gb_Min为%d,Gb_Max为%d,不在<%d,%d>范围内!"),LSCData_Gb_Min,LSCData_Gb_Max,m_GetOTPCheckSpec1.m_LSC_Gb_Min,m_GetOTPCheckSpec1.m_LSC_Gb_Max);
		return 0;
	}
	if ((LSCData_B_Max > m_GetOTPCheckSpec1.m_LSC_B_Max) || (LSCData_B_Min < m_GetOTPCheckSpec1.m_LSC_B_Min))
	{
		Catstring(ErrorMsg,_T("LSC B_Min为%d,B_Max为%d,不在<%d,%d>范围内!"),LSCData_B_Min,LSCData_B_Max,m_GetOTPCheckSpec1.m_LSC_B_Min,m_GetOTPCheckSpec1.m_LSC_B_Max);
		return 0;
	}			
	if(tempMax > m_GetOTPCheckSpec1.m_LSCDistanceSpec)
	{
		Catstring(ErrorMsg,_T("LSC Distance为%d !< %d,不在范围!"),tempMax,m_GetOTPCheckSpec1.m_LSCDistanceSpec);				
		return 0;
	}  

	return 1;
}

int OtpCheckAPI::SaveLSCData(USHORT *rch,USHORT *grch,USHORT *gbch,USHORT *bch)
{
	remove("OTP_LSC_INFO.txt");

	FILE *file;
	file = fopen("OTP_LSC_INFO.txt","w");
	if(!file)
	{
		perror("open file failed!");				
	}
	else 
	{
		fprintf(file,"\nR Channel");
		for (int mm = 0; mm < 221; mm ++)
		{
			if((mm%17)==0)
			{
				fprintf(file,"\n");
			}
			fprintf(file,"%d\t",rch[mm]);
		}

		fprintf(file,"\nGR Channel");
		for (int mm = 0; mm < 221; mm ++)
		{
			if((mm%17)==0)
			{
				fprintf(file,"\n");
			}
			fprintf(file,"%d\t",grch[mm]);
		}

		fprintf(file,"\nGB Channel");
		for (int mm = 0; mm < 221; mm ++)
		{
			if((mm%17)==0)
			{
				fprintf(file,"\n");
			}
			fprintf(file,"%d\t",gbch[mm]);
		}

		fprintf(file,"\nB Channel");
		for (int mm = 0; mm < 221; mm ++)
		{
			if((mm%17)==0)
			{
				fprintf(file,"\n");
			}
			fprintf(file,"%d\t",bch[mm]);
		}
		fclose(file);	
	}
	return TRUE;
}

BOOL OtpCheckAPI::CheckModuleEqualValue(wchar_t *moduleKey,wchar_t *TestItemCountName,wchar_t *TestItemRefName,int testValue,int TestItemCountSpec,wchar_t* ErrorMsg)
{
	wchar_t section[64]	= {0};
	int	 TestItemRef	= 0;
	int	 TestItemCount	= 0;

	//获取TestValue的对比值
	RegReadKey(moduleKey,TestItemRefName,section);
	TestItemRef = _wtoi(section);

	//获取TestItemCount 值，用于判断是否超规
	RegReadKey(moduleKey,TestItemCountName,section);
	TestItemCount = _wtoi(section);

	if ( testValue == TestItemRef)
	{
		TestItemCount ++;

		if (TestItemCount > TestItemCountSpec)
		{
			Catstring(ErrorMsg,_T("%s:%d !<= %d,many module data Equal"),TestItemCountName,TestItemCount,TestItemCountSpec);
			TestItemCount = 0;

			_itow(TestItemCount,section,10);
			RegSetKey(moduleKey,TestItemCountName,section);
			return FALSE;
		}
		else
		{	
			_itow(TestItemCount,section,10);
			RegSetKey(moduleKey,TestItemCountName,section);
		}
	}
	else
	{
		TestItemCount = 0;
		_itow(TestItemCount,section,10);
		RegSetKey(moduleKey,TestItemCountName,section);

		TestItemRef = testValue;
		_itow(TestItemRef,section,10);
		RegSetKey(moduleKey,TestItemRefName,section);
	}
	return TRUE;
}

BOOL OtpCheckAPI::CheckSegmentInfo_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int  m_value = 0;
	if (OTPReadData[0x0025] != m_GetOTPCheckSpec.m_SegmentInfoFlag)
	{
		Catstring(ErrorMsg, _T("m_SegmentInfoFlag 0x%02X != 0x%02X"),OTPReadData[0x0025],m_GetOTPCheckSpec.m_SegmentInfoFlag);
		return FALSE;
	}
	if (OTPReadData[0x0026] != m_GetOTPCheckSpec.m_SegmentCount)
	{
		Catstring(ErrorMsg, _T("m_SegmentCount 0x%02X != 0x%02X"),OTPReadData[0x0026],m_GetOTPCheckSpec.m_SegmentCount);
		return FALSE;
	}
	m_value = ((OTPReadData[0x0027]<<8)&0xFF00) + OTPReadData[0x0028];
	if (m_value != m_GetOTPCheckSpec.m_SegmentAFOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentAFOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentAFOffset);
		return FALSE;
	}
	m_value = ((OTPReadData[0x0029]<<8)&0xFF00) + OTPReadData[0x002A];
	if (m_value != m_GetOTPCheckSpec.m_SegmentAWBOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentAWBOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentAWBOffset);
		return FALSE;
	}
	m_value = ((OTPReadData[0x002B]<<8)&0xFF00) + OTPReadData[0x002C];
	if (m_value != m_GetOTPCheckSpec.m_SegmentLSCOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentLSCOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentLSCOffset);
		return FALSE;
	}
	m_value = ((OTPReadData[0x002D]<<8)&0xFF00) + OTPReadData[0x002E];
	if (m_value != m_GetOTPCheckSpec.m_SegmentAECOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentAECOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentAECOffset);
		return FALSE;
	}
	m_value = ((OTPReadData[0x002F]<<8)&0xFF00) + OTPReadData[0x0030];
	if (m_value != m_GetOTPCheckSpec.m_SegmentPDAFOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentPDAFOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentPDAFOffset);
		return FALSE;
	}	
	m_value = ((OTPReadData[0x0031]<<8)&0xFF00) + OTPReadData[0x0032];
	if (m_value != m_GetOTPCheckSpec.m_SegmentOISOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentOISOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentOISOffset);
		return FALSE;	
	}
	m_value = ((OTPReadData[0x0033]<<8)&0xFF00) + OTPReadData[0x0034];
	if (m_value != m_GetOTPCheckSpec.m_SegmentDualCamOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentDualCamOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentDualCamOffset);
		return FALSE;		
	}
	m_value = ((OTPReadData[0x003D]<<8)&0xFF00) + OTPReadData[0x003E];
	if (m_value != m_GetOTPCheckSpec.m_SegmentChecksumOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentChecksumOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentChecksumOffset);
		return FALSE;			
	}

	//check module info reserve
	for (int i = 0x0035; i < 0x003D; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg, _T("Module reserve 0x%04X is 0x%02X != 0xFF"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	//check module info Check sum
	int m_ModuleInfoSum = 0;
	for (int i = 0x0026; i < 0x003F; i++)
	{
		m_ModuleInfoSum += OTPReadData[i];
	}
	m_ModuleInfoSum = (m_ModuleInfoSum%255) +1 ;
	if (m_ModuleInfoSum != OTPReadData[0x003F])
	{
		Catstring(ErrorMsg, _T("SegmentInfo CheckSum 0x%02X != 0x%02X"),OTPReadData[0x003F],m_ModuleInfoSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckSegmentInfo_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int  m_value = 0;
	if (OTPReadData[0x0025] != m_GetOTPCheckSpec1.m_SegmentInfoFlag)
	{
		Catstring(ErrorMsg, _T("m_SegmentInfoFlag 0x%02X != 0x%02X"),OTPReadData[0x0025],m_GetOTPCheckSpec1.m_SegmentInfoFlag);
		return FALSE;
	}
	if (OTPReadData[0x0026] != m_GetOTPCheckSpec1.m_SegmentCount)
	{
		Catstring(ErrorMsg, _T("m_SegmentCount 0x%02X != 0x%02X"),OTPReadData[0x0026],m_GetOTPCheckSpec1.m_SegmentCount);
		return FALSE;
	}
	m_value = ((OTPReadData[0x0027]<<8)&0xFF00) + OTPReadData[0x0028];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentAFOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentAFOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentAFOffset);
		return FALSE;
	}
	m_value = ((OTPReadData[0x0029]<<8)&0xFF00) + OTPReadData[0x002A];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentAWBOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentAWBOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentAWBOffset);
		return FALSE;
	}
	m_value = ((OTPReadData[0x002B]<<8)&0xFF00) + OTPReadData[0x002C];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentLSCOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentLSCOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentLSCOffset);
		return FALSE;
	}
	m_value = ((OTPReadData[0x002D]<<8)&0xFF00) + OTPReadData[0x002E];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentAECOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentAECOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentAECOffset);
		return FALSE;
	}
	m_value = ((OTPReadData[0x002F]<<8)&0xFF00) + OTPReadData[0x0030];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentPDAFOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentPDAFOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentPDAFOffset);
		return FALSE;
	}	
	m_value = ((OTPReadData[0x0031]<<8)&0xFF00) + OTPReadData[0x0032];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentOISOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentOISOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentOISOffset);
		return FALSE;	
	}
	m_value = ((OTPReadData[0x0033]<<8)&0xFF00) + OTPReadData[0x0034];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentDualCamOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentDualCamOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentDualCamOffset);
		return FALSE;		
	}
	m_value = ((OTPReadData[0x003D]<<8)&0xFF00) + OTPReadData[0x003E];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentChecksumOffset)
	{
		Catstring(ErrorMsg, _T("m_SegmentChecksumOffset 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentChecksumOffset);
		return FALSE;			
	}

	//check module info reserve
	for (int i = 0x0035; i < 0x003D; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg, _T("Module reserve 0x%04X is 0x%02X != 0xFF"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	//check module info Check sum
	int m_ModuleInfoSum = 0;
	for (int i = 0x0026; i < 0x003F; i++)
	{
		m_ModuleInfoSum += OTPReadData[i];
	}
	m_ModuleInfoSum = (m_ModuleInfoSum%255) +1 ;
	if (m_ModuleInfoSum != OTPReadData[0x003F])
	{
		Catstring(ErrorMsg, _T("SegmentInfo CheckSum 0x%02X != 0x%02X"),OTPReadData[0x003F],m_ModuleInfoSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckModuleInfo_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	if (OTPReadData[Info_StartAdd] != m_GetOTPCheckSpec.m_EepromTableRevision)
	{
		Catstring(ErrorMsg,_T("BasicInfo EEPROM Table Revision is 0x%02X != 0x%02X"),OTPReadData[0x0001],m_GetOTPCheckSpec.m_EepromTableRevision);
		return FALSE;
	}
	if (OTPReadData[Info_StartAdd+1] != m_GetOTPCheckSpec.m_CalHwVersion)
	{
		Catstring(ErrorMsg,_T("BasicInfo CalHwVersion is 0x%02X != 0x%02X"),OTPReadData[Info_StartAdd+1],m_GetOTPCheckSpec.m_CalHwVersion);
		return FALSE;
	}
	if (OTPReadData[Info_StartAdd+2] != m_GetOTPCheckSpec.m_CalSwVersion)
	{
		Catstring(ErrorMsg,_T("BasicInfo CalSwVersion is 0x%02X != 0x%02X"),OTPReadData[Info_StartAdd+2],m_GetOTPCheckSpec.m_CalSwVersion);
		return FALSE;
	}
	if (((OTPReadData[Info_StartAdd+3]<<8) + (OTPReadData[Info_StartAdd+4])) != m_GetOTPCheckSpec.m_PartNumber1)
	{
		Catstring(ErrorMsg,_T("BasicInfo MotorolaPartNumber1 is 0x%04X != 0x%04X"),(OTPReadData[Info_StartAdd+3]<<8) + (OTPReadData[Info_StartAdd+4]),m_GetOTPCheckSpec.m_PartNumber1);
		return FALSE;
	}
	if (((OTPReadData[Info_StartAdd+5]<<8) + (OTPReadData[Info_StartAdd+6])) != m_GetOTPCheckSpec.m_PartNumber2)
	{
		Catstring(ErrorMsg,_T("BasicInfo MotorolaPartNumber2 is 0x%04X != 0x%04X"),(OTPReadData[Info_StartAdd+5]<<8) + (OTPReadData[Info_StartAdd+6]),m_GetOTPCheckSpec.m_PartNumber2);
		return FALSE;
	}
	if (((OTPReadData[Info_StartAdd+7]<<8) + (OTPReadData[Info_StartAdd+8])) != m_GetOTPCheckSpec.m_PartNumber3)
	{
		Catstring(ErrorMsg,_T("BasicInfo MotorolaPartNumber3 is 0x%04X != 0x%04X"),(OTPReadData[Info_StartAdd+7]<<8) + (OTPReadData[Info_StartAdd+8]),m_GetOTPCheckSpec.m_PartNumber3);
		return FALSE;
	}
	if (((OTPReadData[Info_StartAdd+9]<<8) + (OTPReadData[Info_StartAdd+10])) != m_GetOTPCheckSpec.m_PartNumber4)
	{
		Catstring(ErrorMsg,_T("BasicInfo MotorolaPartNumber4 is 0x%04X != 0x%04X"),(OTPReadData[Info_StartAdd+9]<<8) + (OTPReadData[Info_StartAdd+10]),m_GetOTPCheckSpec.m_PartNumber4);
		return FALSE;
	}

	if (OTPReadData[Info_StartAdd+11] != m_GetOTPCheckSpec.m_ActuatorID)
	{
		Catstring(ErrorMsg,_T("BasicInfo Actuator ID is 0x%02X != 0x%02X"),OTPReadData[Info_StartAdd+11],m_GetOTPCheckSpec.m_ActuatorID);
		return FALSE;
	}
	if (OTPReadData[Info_StartAdd+12] != m_GetOTPCheckSpec.m_LensID)
	{
		Catstring(ErrorMsg,_T("BasicInfo Lens ID is 0x%02X != 0x%02X"),OTPReadData[Info_StartAdd+12],m_GetOTPCheckSpec.m_LensID);
		return FALSE;
	}
	if (((OTPReadData[Info_StartAdd+13]<<8) + (OTPReadData[Info_StartAdd+14])) != m_GetOTPCheckSpec.m_ManufacturerID)
	{
		Catstring(ErrorMsg,_T("BasicInfo ManufacturerID is 0x%04X != 0x%04X"),(OTPReadData[Info_StartAdd+13]<<8) + (OTPReadData[Info_StartAdd+14]),m_GetOTPCheckSpec.m_ManufacturerID);
		return FALSE;
	}
	if (((OTPReadData[Info_StartAdd+15]<<8) + (OTPReadData[Info_StartAdd+16])) != m_GetOTPCheckSpec.m_FactoryID)
	{
		Catstring(ErrorMsg,_T("BasicInfo Factory is 0x%02X != 0x%02X"),(OTPReadData[Info_StartAdd+15]<<8) + (OTPReadData[Info_StartAdd+16]),m_GetOTPCheckSpec.m_FactoryID);
		return FALSE;
	}
	if (OTPReadData[Info_StartAdd+17] != m_GetOTPCheckSpec.m_ManufactureLine)
	{
		Catstring(ErrorMsg,_T("BasicInfo ManufactureLine is 0x%02X != 0x%02X"),OTPReadData[Info_StartAdd+17],m_GetOTPCheckSpec.m_ManufactureLine);
		return FALSE;
	}

	if (OTPReadData[Info_StartAdd+18]>22 || OTPReadData[Info_StartAdd+18]<17)
	{
		Catstring(ErrorMsg, _T("BasicInfo Year %d not in<17,22>"),OTPReadData[Info_StartAdd+18]);
		return FALSE;
	}
	if (OTPReadData[Info_StartAdd+19]>12 || OTPReadData[Info_StartAdd+19]<1)
	{
		Catstring(ErrorMsg, _T("BasicInfo Month %d not in<1,12>"),OTPReadData[Info_StartAdd+19]);
		return FALSE;
	}
	if (OTPReadData[Info_StartAdd+20]>31 || OTPReadData[Info_StartAdd+20]<1)
	{
		Catstring(ErrorMsg, _T("BasicInfo Day %d not in<1,31>"),OTPReadData[Info_StartAdd+20]);
		return FALSE;
	}

	int m_SameNum = 0;
	for (int i = (Info_StartAdd+21);i < (Info_StartAdd+37);i++)
	{
		if ((OTPReadData[i] == 0x00) || (OTPReadData[i] == 0xFF))
		{
			m_SameNum ++;
		}
	}
	if (m_SameNum > 15)
	{
		Catstring(ErrorMsg,_T("BasicInfo Serial Number all is 0xFF/0x00."));
		return FALSE;
	}

	int CRC = GetCRC16(&OTPReadData[Info_StartAdd],Info_EndAdd - Info_StartAdd - 1); 
	if (((OTPReadData[Info_EndAdd - 1]<<8) + (OTPReadData[Info_EndAdd])) != CRC)
	{
		Catstring(ErrorMsg,_T("BasicInfo CheckSum is 0x%04X != 0x%04X"),(OTPReadData[Info_EndAdd - 1]<<8) + (OTPReadData[Info_EndAdd]),CRC);
		return FALSE;
	}
	return TRUE;
}

BOOL OtpCheckAPI::CheckModuleInfo_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (OTPReadData[0x0000] != m_GetOTPCheckSpec1.m_ModulInfoFlag)
	{
		Catstring(ErrorMsg,_T("Flag of Base Information is 0x%02X != 0x%02X"),OTPReadData[0x0000],m_GetOTPCheckSpec1.m_ModulInfoFlag);
		return FALSE;
	}

	if (OTPReadData[0x0001] != m_GetOTPCheckSpec1.m_EepromTableRevision)
	{
		Catstring(ErrorMsg,_T("Data Storage Location is 0x%02X != 0x%02X"),OTPReadData[0x0001],m_GetOTPCheckSpec1.m_EepromTableRevision);
		return FALSE;
	}

	if ((OTPReadData[0x0002] != 'E') || (OTPReadData[0x0003] != 'M') || (OTPReadData[0x0004] != 'C')
		|| (OTPReadData[0x0005] != 'A') || (OTPReadData[0x0006] != 'M') || (OTPReadData[0x0007] != '0')
		|| (OTPReadData[0x0008] != '0') || (OTPReadData[0x0009] != '2') || (OTPReadData[0x000A] != '0')
		|| (OTPReadData[0x000B] != 'A') || (OTPReadData[0x000C] != '0') || (OTPReadData[0x000D] != '1'))
	{
		Catstring(ErrorMsg,_T("Part Number of Smartisan Fail"));
		return FALSE;
	}

	if (OTPReadData[0x000E] != m_GetOTPCheckSpec1.m_ManufacturerID)
	{
		Catstring(ErrorMsg,_T("Supplier ID is 0x%02X != 0x%02X"),OTPReadData[0x000E],m_GetOTPCheckSpec1.m_ManufacturerID);
		return FALSE;
	}

	if (OTPReadData[0x000F] != m_GetOTPCheckSpec1.m_SensorNameID)
	{
		Catstring(ErrorMsg,_T("Sensor ID is 0x%02X != 0x%02X"),OTPReadData[0x000F],m_GetOTPCheckSpec1.m_SensorNameID);
		return FALSE;
	}

	if (OTPReadData[0x0010] != m_GetOTPCheckSpec1.m_LensID)
	{
		Catstring(ErrorMsg,_T("Lens ID is 0x%02X != 0x%02X"),OTPReadData[0x0010],m_GetOTPCheckSpec1.m_LensID);
		return FALSE;
	}

	if (OTPReadData[0x0011] != m_GetOTPCheckSpec1.m_ActuatorID)
	{
		Catstring(ErrorMsg,_T("VCM ID is 0x%02X != 0x%02X"),OTPReadData[0x0011],m_GetOTPCheckSpec1.m_ActuatorID);
		return FALSE;
	}

	if (OTPReadData[0x0012] != m_GetOTPCheckSpec1.m_DriverIcID)
	{
		Catstring(ErrorMsg,_T("Driver IC ID is 0x%02X != 0x%02X"),OTPReadData[0x0012],m_GetOTPCheckSpec1.m_DriverIcID);
		return FALSE;
	}

	if (OTPReadData[0x0013] != m_GetOTPCheckSpec1.m_IRBGID)
	{
		Catstring(ErrorMsg,_T("IR/BG ID is 0x%02X != 0x%02X"),OTPReadData[0x0013],m_GetOTPCheckSpec1.m_IRBGID);
		return FALSE;
	}

	if (OTPReadData[0x0014] != m_GetOTPCheckSpec1.m_GyroID)
	{
		Catstring(ErrorMsg,_T("Gyro ID is 0x%02X != 0x%02X"),OTPReadData[0x0014],m_GetOTPCheckSpec1.m_GyroID);
		return FALSE;
	}

	if (OTPReadData[0x0015] != m_GetOTPCheckSpec1.m_ColorTemperatureID)
	{
		Catstring(ErrorMsg,_T("Color Temperature ID is 0x%02X != 0x%02X"),OTPReadData[0x0015],m_GetOTPCheckSpec1.m_ColorTemperatureID);
		return FALSE;
	}

	if (OTPReadData[0x0016] != m_GetOTPCheckSpec1.m_OisDriverIcID)
	{
		Catstring(ErrorMsg,_T("OIS Driver IC ID is 0x%02X != 0x%02X"),OTPReadData[0x0016],m_GetOTPCheckSpec1.m_OisDriverIcID);
		return FALSE;
	}

	if (OTPReadData[0x0017] != m_GetOTPCheckSpec1.m_FpcPcbVersion)
	{
		Catstring(ErrorMsg,_T("FPC/PCB Version is 0x%02X != 0x%02X"),OTPReadData[0x0017],m_GetOTPCheckSpec1.m_FpcPcbVersion);
		return FALSE;
	}

	if (OTPReadData[0x0018] != m_GetOTPCheckSpec1.m_CalibrationVersion)
	{
		Catstring(ErrorMsg,_T("Calibration Version is 0x%02X != 0x%02X"),OTPReadData[0x0018],m_GetOTPCheckSpec1.m_CalibrationVersion);
		return FALSE;
	}

	if (OTPReadData[0x0019] != m_GetOTPCheckSpec1.m_SoftVersion)
	{
		Catstring(ErrorMsg,_T("OIS firmware version is 0x%02X != 0x%02X"),OTPReadData[0x0019],m_GetOTPCheckSpec1.m_SoftVersion);
		return FALSE;
	}

	if (OTPReadData[0x002A] != m_GetOTPCheckSpec1.m_ModuleVersion)
	{
		Catstring(ErrorMsg,_T("Camera building stage is 0x%02X != 0x%02X"),OTPReadData[0x002A],m_GetOTPCheckSpec1.m_ModuleVersion);
		return FALSE;
	}

	if (OTPReadData[0x002B] != m_GetOTPCheckSpec1.m_FactoryID)
	{
		Catstring(ErrorMsg,_T("Production factory is 0x%02X != 0x%02X"),OTPReadData[0x002B],m_GetOTPCheckSpec1.m_FactoryID);
		return FALSE;
	}

	if (OTPReadData[0x002C] != m_GetOTPCheckSpec1.m_ManufactureLine)
	{
		Catstring(ErrorMsg,_T("Production line is 0x%02X != 0x%02X"),OTPReadData[0x002C],m_GetOTPCheckSpec1.m_ManufactureLine);
		return FALSE;
	}

	if (OTPReadData[0x002D]>22 || OTPReadData[0x002D]<17)
	{
		Catstring(ErrorMsg, _T("BasicInfo Year %d not in<17,22>"),OTPReadData[0x002D]);
		return FALSE;
	}
	if (OTPReadData[0x002E]>12 || OTPReadData[0x002E]<1)
	{
		Catstring(ErrorMsg, _T("BasicInfo Month %d not in<1,12>"),OTPReadData[0x002E]);
		return FALSE;
	}
	if (OTPReadData[0x002F]>31 || OTPReadData[0x002F]<1)
	{
		Catstring(ErrorMsg, _T("BasicInfo Day %d not in<1,31>"),OTPReadData[0x0014]);
		return FALSE;
	}

	if ((OTPReadData[0x0032] != 0xff) || (OTPReadData[0x0033] != 0xff))
	{
		Catstring(ErrorMsg,_T("Reserved is 0x%02X 0x%02X!= 0x%02X"),OTPReadData[0x0032],OTPReadData[0x0032],0xff);
		return FALSE;
	}

	int BasicCheckSum = 0;
	for (i = 0x0000; i < 0x0034; i++)
	{
		BasicCheckSum = BasicCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x0034] != ((BasicCheckSum >> 8) & 0xff)) || (OTPReadData[0x0035] != (BasicCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n Basic Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x0034],OTPReadData[0x0035],BasicCheckSum);
		return FALSE;
	}

	return TRUE;
#if 0
	//check module info
	if (OTPReadData[0] == 0xff && OTPReadData[1] == 0xff)
	{
		Catstring(ErrorMsg,_T("OTP漏烧录!"));
		return FALSE;
	}
	if (OTPReadData[0x0000] != m_GetOTPCheckSpec1.m_ModulInfoFlag)
	{
		Catstring(ErrorMsg, _T("Module InfoFlag 0x%02X != 0x%02X"),OTPReadData[0x0000],m_GetOTPCheckSpec1.m_ModulInfoFlag);
		return FALSE;
	}
	if (OTPReadData[0x0001] != m_GetOTPCheckSpec1.m_VendorID)
	{
		Catstring(ErrorMsg, _T("m_ModuleID 0x%02X != 0x%02X"),OTPReadData[0x0001],m_GetOTPCheckSpec1.m_VendorID);
		return FALSE;
	}
	if (OTPReadData[0x0002]>21 || OTPReadData[0x0002]<17)
	{
		Catstring(ErrorMsg, _T("Year %d not in<17,21>"),OTPReadData[0x0002]);
		return FALSE;
	}
	if (OTPReadData[0x0003]>12 || OTPReadData[0x0003]<1)
	{
		Catstring(ErrorMsg, _T("Month %d not in<1,12>"),OTPReadData[0x0003]);
		return FALSE;
	}
	if (OTPReadData[0x0004]>31 || OTPReadData[0x0004]<1)
	{
		Catstring(ErrorMsg, _T("Day %d not in<1,31>"),OTPReadData[0x0004]);
		return FALSE;
	}
	if (OTPReadData[0x0005]>23 || OTPReadData[0x0005]<0)
	{
		Catstring(ErrorMsg, _T("Hour %d not in<0,23>"),OTPReadData[0x0005]);
		return FALSE;
	}
	if (OTPReadData[0x0006]>59 || OTPReadData[0x0006]<0)
	{
		Catstring(ErrorMsg, _T("Minute %d not in<0,59>"),OTPReadData[0x0006]);
		return FALSE;
	}
	if (OTPReadData[0x0007]>59 || OTPReadData[0x0007]<0)
	{
		Catstring(ErrorMsg, _T("Second %d not in<0,59>"),OTPReadData[0x0007]);
		return FALSE;
	}
	if (OTPReadData[0x0008] != m_GetOTPCheckSpec1.m_LensID)
	{
		Catstring(ErrorMsg, _T("m_LensID 0x%02X != 0x%02X"),OTPReadData[0x0008],m_GetOTPCheckSpec1.m_LensID);
		return FALSE;
	}
	if (OTPReadData[0x0009] != m_GetOTPCheckSpec1.m_VCMID)
	{
		Catstring(ErrorMsg, _T("m_VCMID 0x%02X != 0x%02X"),OTPReadData[0x0009],m_GetOTPCheckSpec1.m_VCMID);
		return FALSE;
	}
	if (OTPReadData[0x000A] != m_GetOTPCheckSpec1.m_DriverIcID)
	{
		Catstring(ErrorMsg, _T("m_DriverIcID 0x%02X != 0x%02X"),OTPReadData[0x000A],m_GetOTPCheckSpec1.m_DriverIcID);
	}
	if (OTPReadData[0x000B] != m_GetOTPCheckSpec1.m_OISFWVER)
	{
		Catstring(ErrorMsg, _T("m_OISFWVER 0x%02X != 0x%02X"),OTPReadData[0x000B],m_GetOTPCheckSpec1.m_OISFWVER);
		return FALSE;
	}
	if (OTPReadData[0x000C] != m_GetOTPCheckSpec1.m_SensorNameID)
	{
		Catstring(ErrorMsg, _T("m_SensorNameID 0x%02X != 0x%02X"),OTPReadData[0x000C],m_GetOTPCheckSpec1.m_SensorNameID);
		return FALSE;
	}
	if (OTPReadData[0x000D] != m_GetOTPCheckSpec1.m_ProductID)
	{
		Catstring(ErrorMsg, _T("m_ProductID 0x%02X != 0x%02X"),OTPReadData[0x000D],m_GetOTPCheckSpec1.m_ProductID);
		return FALSE;
	}
	if (OTPReadData[0x000E] != m_GetOTPCheckSpec1.m_MapVersionID)
	{
		Catstring(ErrorMsg, _T("m_MapVersionID 0x%02X != 0x%02X"),OTPReadData[0x000E],m_GetOTPCheckSpec1.m_MapVersionID);
		return FALSE;
	}
	if (OTPReadData[0x000F] != m_GetOTPCheckSpec1.m_MapHistoryID)
	{
		Catstring(ErrorMsg, _T("m_MapHistoryID 0x%02X != 0x%02X"),OTPReadData[0x000F],m_GetOTPCheckSpec1.m_MapHistoryID);
		return FALSE;
	}

	int temp=0;
	for (int i=0;i<16;i++)
	{
		if ((OTPReadData[0x0010+i] == 0) || (OTPReadData[0x0010+i] == 0xFF))
		{
			temp++;
		}
	}
	if (temp>15)
	{
		Catstring(ErrorMsg, _T("Module Serial No.全为0或0xFF,未烧录Fuse ID!"));
		return FALSE;
	}

	//check module info reserve
	for (int i = 0x0020; i < 0x0024; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg, _T("Module reserve 0x%04X is 0x%02X != 0xFF"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	//check module info Check sum
	int m_ModuleInfoSum = 0;
	for (int i = 0x0001; i < 0x0024; i++)
	{
		m_ModuleInfoSum += OTPReadData[i];
	}
	m_ModuleInfoSum = (m_ModuleInfoSum%255) +1 ;
	if (m_ModuleInfoSum != OTPReadData[0x0024])
	{
		Catstring(ErrorMsg, _T("ModuleInfo CheckSum 0x%02X != 0x%02X"),OTPReadData[0x0024],m_ModuleInfoSum);
		return FALSE;
	}

	return TRUE;
#endif
}

BOOL OtpCheckAPI::CheckModuleInfo_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (OTPReadData[0x0600] != m_GetOTPCheckSpec1.m_ModulInfoFlag)
	{
		Catstring(ErrorMsg,_T("Flag of Base Information is 0x%02X != 0x%02X"),OTPReadData[0x0600],m_GetOTPCheckSpec1.m_ModulInfoFlag);
		return FALSE;
	}

	if (OTPReadData[0x0601] != m_GetOTPCheckSpec1.m_EepromTableRevision)
	{
		Catstring(ErrorMsg,_T("Data Storage Location is 0x%02X != 0x%02X"),OTPReadData[0x0601],m_GetOTPCheckSpec1.m_EepromTableRevision);
		return FALSE;
	}

	if ((OTPReadData[0x0602] != 'E') || (OTPReadData[0x0603] != 'M') || (OTPReadData[0x0604] != 'C')
		|| (OTPReadData[0x0605] != 'A') || (OTPReadData[0x0606] != 'M') || (OTPReadData[0x0607] != '0')
		|| (OTPReadData[0x0608] != '0') || (OTPReadData[0x0609] != '2') || (OTPReadData[0x060A] != '0')
		|| (OTPReadData[0x060B] != 'A') || (OTPReadData[0x060C] != '0') || (OTPReadData[0x060D] != '1'))
	{
		Catstring(ErrorMsg,_T("Part Number of Smartisan Fail"));
		return FALSE;
	}

	if (OTPReadData[0x060E] != m_GetOTPCheckSpec1.m_ManufacturerID)
	{
		Catstring(ErrorMsg,_T("Supplier ID is 0x%02X != 0x%02X"),OTPReadData[0x060E],m_GetOTPCheckSpec1.m_ManufacturerID);
		return FALSE;
	}

	if (OTPReadData[0x060F] != m_GetOTPCheckSpec1.m_SensorNameID)
	{
		Catstring(ErrorMsg,_T("Sensor ID is 0x%02X != 0x%02X"),OTPReadData[0x060F],m_GetOTPCheckSpec1.m_SensorNameID);
		return FALSE;
	}

	if (OTPReadData[0x0610] != m_GetOTPCheckSpec1.m_LensID)
	{
		Catstring(ErrorMsg,_T("Lens ID is 0x%02X != 0x%02X"),OTPReadData[0x0610],m_GetOTPCheckSpec1.m_LensID);
		return FALSE;
	}

	if (OTPReadData[0x0611] != m_GetOTPCheckSpec1.m_ActuatorID)
	{
		Catstring(ErrorMsg,_T("VCM ID is 0x%02X != 0x%02X"),OTPReadData[0x0611],m_GetOTPCheckSpec1.m_ActuatorID);
		return FALSE;
	}

	if (OTPReadData[0x0612] != m_GetOTPCheckSpec1.m_DriverIcID)
	{
		Catstring(ErrorMsg,_T("Driver IC ID is 0x%02X != 0x%02X"),OTPReadData[0x0612],m_GetOTPCheckSpec1.m_DriverIcID);
		return FALSE;
	}

	if (OTPReadData[0x0613] != m_GetOTPCheckSpec1.m_IRBGID)
	{
		Catstring(ErrorMsg,_T("IR/BG ID is 0x%02X != 0x%02X"),OTPReadData[0x0613],m_GetOTPCheckSpec1.m_IRBGID);
		return FALSE;
	}

	if (OTPReadData[0x0614] != m_GetOTPCheckSpec1.m_GyroID)
	{
		Catstring(ErrorMsg,_T("Gyro ID is 0x%02X != 0x%02X"),OTPReadData[0x0614],m_GetOTPCheckSpec1.m_GyroID);
		return FALSE;
	}

	if (OTPReadData[0x0615] != m_GetOTPCheckSpec1.m_ColorTemperatureID)
	{
		Catstring(ErrorMsg,_T("Color Temperature ID is 0x%02X != 0x%02X"),OTPReadData[0x0615],m_GetOTPCheckSpec1.m_ColorTemperatureID);
		return FALSE;
	}

	if (OTPReadData[0x0616] != m_GetOTPCheckSpec1.m_OisDriverIcID)
	{
		Catstring(ErrorMsg,_T("OIS Driver IC ID is 0x%02X != 0x%02X"),OTPReadData[0x0616],m_GetOTPCheckSpec1.m_OisDriverIcID);
		return FALSE;
	}

	if (OTPReadData[0x0617] != m_GetOTPCheckSpec1.m_FpcPcbVersion)
	{
		Catstring(ErrorMsg,_T("FPC/PCB Version is 0x%02X != 0x%02X"),OTPReadData[0x0617],m_GetOTPCheckSpec1.m_FpcPcbVersion);
		return FALSE;
	}

	if (OTPReadData[0x0618] != m_GetOTPCheckSpec1.m_CalibrationVersion)
	{
		Catstring(ErrorMsg,_T("Calibration Version is 0x%02X != 0x%02X"),OTPReadData[0x0618],m_GetOTPCheckSpec1.m_CalibrationVersion);
		return FALSE;
	}

	if (OTPReadData[0x0619] != m_GetOTPCheckSpec1.m_SoftVersion)
	{
		Catstring(ErrorMsg,_T("OIS firmware version is 0x%02X != 0x%02X"),OTPReadData[0x0619],m_GetOTPCheckSpec1.m_SoftVersion);
		return FALSE;
	}

	if (OTPReadData[0x062A] != m_GetOTPCheckSpec1.m_ModuleVersion)
	{
		Catstring(ErrorMsg,_T("Camera building stage is 0x%02X != 0x%02X"),OTPReadData[0x062A],m_GetOTPCheckSpec1.m_ModuleVersion);
		return FALSE;
	}

	if (OTPReadData[0x062B] != m_GetOTPCheckSpec1.m_FactoryID)
	{
		Catstring(ErrorMsg,_T("Production factory is 0x%02X != 0x%02X"),OTPReadData[0x062B],m_GetOTPCheckSpec1.m_FactoryID);
		return FALSE;
	}

	if (OTPReadData[0x062C] != m_GetOTPCheckSpec1.m_ManufactureLine)
	{
		Catstring(ErrorMsg,_T("Production line is 0x%02X != 0x%02X"),OTPReadData[0x062C],m_GetOTPCheckSpec1.m_ManufactureLine);
		return FALSE;
	}

	if (OTPReadData[0x062D]>22 || OTPReadData[0x062D]<17)
	{
		Catstring(ErrorMsg, _T("BasicInfo Year %d not in<17,22>"),OTPReadData[0x062D]);
		return FALSE;
	}
	if (OTPReadData[0x062E]>12 || OTPReadData[0x062E]<1)
	{
		Catstring(ErrorMsg, _T("BasicInfo Month %d not in<1,12>"),OTPReadData[0x062E]);
		return FALSE;
	}
	if (OTPReadData[0x062F]>31 || OTPReadData[0x062F]<1)
	{
		Catstring(ErrorMsg, _T("BasicInfo Day %d not in<1,31>"),OTPReadData[0x062F]);
		return FALSE;
	}

	if ((OTPReadData[0x0632] != 0xff) || (OTPReadData[0x0632] != 0xff))
	{
		Catstring(ErrorMsg,_T("Reserved is 0x%02X 0x%02X!= 0x%02X"),OTPReadData[0x0632],OTPReadData[0x0032],0xff);
		return FALSE;
	}

	int BasicCheckSum = 0;
	for (i = 0x0600; i < 0x0634; i++)
	{
		BasicCheckSum = BasicCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x0634] != ((BasicCheckSum >> 8) & 0xff)) || (OTPReadData[0x0635] != (BasicCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n Basic Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x0634],OTPReadData[0x0635],BasicCheckSum);
		return FALSE;
	}

	return TRUE;
}
BOOL OtpCheckAPI::CheckAFCode_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;

	int m_AFInfCode			= ((OTPReadData[0x063A]<<8)&0xFF00) + (OTPReadData[0x063B]&0xFF);
	int m_AFMacroCode		= ((OTPReadData[0x063C]<<8)&0xFF00) + (OTPReadData[0x063D]&0xFF);
	int m_AFCodeDiff		= m_AFMacroCode - m_AFInfCode;

	if (OTPReadData[0x0636] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of AF Calibration is 0x%02X != 0x01"),OTPReadData[0x0636]);
		return FALSE;
	}

	if (OTPReadData[0x0637] != 0xFF)
	{
		Catstring(ErrorMsg,_T("af Equip number is 0x%02X != 0xFF"),OTPReadData[0x0637]);
		return FALSE;
	}
	if ((OTPReadData[0x0638] != 0x00) || (OTPReadData[0x0639] != 0x00))
	{
		Catstring(ErrorMsg,_T("AF Start current is 0x%02X,0x%02X != 0x00"),OTPReadData[0x0638],OTPReadData[0x0639]);
		return FALSE;
	}
	for (int i = 0x063E;i< 0x0642;i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("AF Reserved Verify 0x%04X is 0x%02X != 0xFF"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	if ((m_AFMacroCode > m_GetOTPCheckSpec1.m_MaxMacroCode) || (m_AFMacroCode < m_GetOTPCheckSpec1.m_MinMacroCode))
	{
		Catstring(ErrorMsg, _T("m_MacroCode %d not in<%d,%d>"),m_AFMacroCode,m_GetOTPCheckSpec1.m_MinMacroCode,m_GetOTPCheckSpec1.m_MaxMacroCode);
		return FALSE;
	}
	if ((m_AFInfCode > m_GetOTPCheckSpec1.m_MaxInfinityCode) ||( m_AFInfCode < m_GetOTPCheckSpec1.m_MinInfinityCode))
	{
		Catstring(ErrorMsg, _T("m_InfinityCode %d not in<%d,%d>"),m_AFInfCode,m_GetOTPCheckSpec1.m_MinInfinityCode,m_GetOTPCheckSpec1.m_MaxInfinityCode);
		return FALSE;
	}
	if ((m_AFCodeDiff > m_GetOTPCheckSpec1.m_MaxCodeDiff) || (m_AFCodeDiff < m_GetOTPCheckSpec1.m_MinCodeDiff))
	{
		Catstring(ErrorMsg, _T("AFCodeDiff %d not in<%d,%d>"),m_AFCodeDiff,m_GetOTPCheckSpec1.m_MinCodeDiff,m_GetOTPCheckSpec1.m_MaxCodeDiff);
		return FALSE;
	}

	int AFCheckSum = 0;//GetCRC16(&OTPReadData[0x0027],24);
	for (i = 0x0636; i < 0x0642; i++)
	{
		AFCheckSum = AFCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x0642] != ((AFCheckSum >> 8) & 0xff)) || (OTPReadData[0x0643] != (AFCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n AF Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x0642],OTPReadData[0x0643],AFCheckSum);
		return FALSE;
	}
	return TRUE;
}

BOOL OtpCheckAPI::CheckAWB_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	float m_CIE_Ev_Source = (OTPReadData[AWB_StartAdd]<<8) + (OTPReadData[AWB_StartAdd+1]);
	float m_CIE_U_Source  = (OTPReadData[AWB_StartAdd+2]<<8) + (OTPReadData[AWB_StartAdd+3]);
	float m_CIE_V_Source  = (OTPReadData[AWB_StartAdd+4]<<8) + (OTPReadData[AWB_StartAdd+5]);

	double Tolerance_Source = sqrt(pow((m_CIE_U_Source-m_GetOTPCheckSpec.m_u_standard_Source),2)+pow((m_CIE_V_Source-m_GetOTPCheckSpec.m_v_standard_Source),2));

	if ((m_CIE_Ev_Source > m_GetOTPCheckSpec.m_MaxCIE_Ev_Source) || (m_CIE_Ev_Source < m_GetOTPCheckSpec.m_MinCIE_Ev_Source))
	{
		Catstring(ErrorMsg,_T("m_CIE_Ev_Source is %.2f ,not in <%d,%d>!"),m_CIE_Ev_Source,m_GetOTPCheckSpec.m_MinCIE_Ev_Source,m_GetOTPCheckSpec.m_MaxCIE_Ev_Source);
		return FALSE;
	}
	if ((m_CIE_U_Source > m_GetOTPCheckSpec.m_MaxCIE_U_Source) || (m_CIE_U_Source < m_GetOTPCheckSpec.m_MinCIE_U_Source))
	{
		Catstring(ErrorMsg,_T("m_CIE_U_Source is %.2f ,not in <%d,%d>!"),m_CIE_U_Source,m_GetOTPCheckSpec.m_MinCIE_U_Source,m_GetOTPCheckSpec.m_MaxCIE_U_Source);
		return FALSE;
	}
	if ((m_CIE_V_Source > m_GetOTPCheckSpec.m_MaxCIE_V_Source) || (m_CIE_V_Source < m_GetOTPCheckSpec.m_MinCIE_V_Source))
	{
		Catstring(ErrorMsg,_T("m_CIE_V_Source is %.2f ,not in <%d,%d>!"),m_CIE_V_Source,m_GetOTPCheckSpec.m_MinCIE_V_Source,m_GetOTPCheckSpec.m_MaxCIE_V_Source);
		return FALSE;
	}
	if ((Tolerance_Source > m_GetOTPCheckSpec.m_MaxTolerance_Source) || (Tolerance_Source < m_GetOTPCheckSpec.m_MinTolerance_Source))
	{
		Catstring(ErrorMsg,_T("Tolerance_Source is %.2f ,not in <%d,%d>!"),Tolerance_Source,m_GetOTPCheckSpec.m_MinTolerance_Source,m_GetOTPCheckSpec.m_MaxTolerance_Source);
		return FALSE;
	}

	int RGolden_Source		= (OTPReadData[AWB_StartAdd+6]<<8) + (OTPReadData[AWB_StartAdd+7]);
	int GrGolden_Source		= (OTPReadData[AWB_StartAdd+8]<<8) + (OTPReadData[AWB_StartAdd+9]);
	int GbGolden_Source		= (OTPReadData[AWB_StartAdd+10]<<8) + (OTPReadData[AWB_StartAdd+11]);
	int BGolden_Source		= (OTPReadData[AWB_StartAdd+12]<<8) + (OTPReadData[AWB_StartAdd+13]);
	int RGainGolden_Source	= (OTPReadData[AWB_StartAdd+14]<<8) + (OTPReadData[AWB_StartAdd+15]);
	int BGainGolden_Source	= (OTPReadData[AWB_StartAdd+16]<<8) + (OTPReadData[AWB_StartAdd+17]);
	int GGainGolden_Source	= (OTPReadData[AWB_StartAdd+18]<<8) + (OTPReadData[AWB_StartAdd+19]);

	float C_RGainGolden_Source = (1.0*(RGolden_Source)/(0.5*(GrGolden_Source+GbGolden_Source)))*m_GetOTPCheckSpec.m_Multiple;
	float C_BGainGolden_Source = (1.0*(BGolden_Source)/(0.5*(GrGolden_Source+GbGolden_Source)))*m_GetOTPCheckSpec.m_Multiple;
	float C_GGainGolden_Source = (1.0*(GrGolden_Source)/(GbGolden_Source))*m_GetOTPCheckSpec.m_Multiple;
	int C_ReCal_RG_diff = abs(RGainGolden_Source - C_RGainGolden_Source);
	int C_ReCal_BG_diff = abs(BGainGolden_Source - C_BGainGolden_Source);
	int C_ReCal_GG_diff = abs(GGainGolden_Source - C_GGainGolden_Source);
	if (C_ReCal_RG_diff > m_GetOTPCheckSpec.m_ReCal_RG_diff)
	{
		Catstring(ErrorMsg,_T("AWB ReCal_RG_diff is %d !< %d "),C_ReCal_RG_diff,m_GetOTPCheckSpec.m_ReCal_RG_diff);
		return FALSE;
	}
	if (C_ReCal_BG_diff > m_GetOTPCheckSpec.m_ReCal_BG_diff)
	{
		Catstring(ErrorMsg,_T("AWB ReCal_BG_diff is %d !< %d "),C_ReCal_BG_diff,m_GetOTPCheckSpec.m_ReCal_BG_diff);
		return FALSE;
	}
	if (C_ReCal_GG_diff > m_GetOTPCheckSpec.m_ReCal_GG_diff)
	{
		Catstring(ErrorMsg,_T("AWB ReCal_GG_diff is %d !< %d "),C_ReCal_GG_diff,m_GetOTPCheckSpec.m_ReCal_GG_diff);
		return FALSE;
	}

	int R_Source1		= (OTPReadData[AWB_StartAdd+20]<<8) + (OTPReadData[AWB_StartAdd+21]);
	int Gr_Source1		= (OTPReadData[AWB_StartAdd+22]<<8) + (OTPReadData[AWB_StartAdd+23]);
	int Gb_Source1		= (OTPReadData[AWB_StartAdd+24]<<8) + (OTPReadData[AWB_StartAdd+25]);
	int B_Source1		= (OTPReadData[AWB_StartAdd+26]<<8) + (OTPReadData[AWB_StartAdd+27]);
	int RGain_Source1	= (OTPReadData[AWB_StartAdd+28]<<8) + (OTPReadData[AWB_StartAdd+29]);
	int BGain_Source1	= (OTPReadData[AWB_StartAdd+30]<<8) + (OTPReadData[AWB_StartAdd+31]);
	int GGain_Source1	= (OTPReadData[AWB_StartAdd+32]<<8) + (OTPReadData[AWB_StartAdd+33]);

	if ((R_Source1 > m_GetOTPCheckSpec.m_MaxRValue) || (R_Source1 < m_GetOTPCheckSpec.m_MinRValue))
	{
		Catstring(ErrorMsg,_T("AWB R is %d ,not in <%d,%d>!"),R_Source1,m_GetOTPCheckSpec.m_MinRValue,m_GetOTPCheckSpec.m_MaxRValue);
		return FALSE;
	}
	if ((Gr_Source1 > m_GetOTPCheckSpec.m_MaxGrValue) || (Gr_Source1 < m_GetOTPCheckSpec.m_MinGrValue))
	{
		Catstring(ErrorMsg,_T("AWB Gr is %d ,not in <%d,%d>!"),Gr_Source1,m_GetOTPCheckSpec.m_MinGrValue,m_GetOTPCheckSpec.m_MaxGrValue);
		return FALSE;
	}
	if ((Gb_Source1 > m_GetOTPCheckSpec.m_MaxGbValue) || (Gb_Source1 < m_GetOTPCheckSpec.m_MinGbValue))
	{
		Catstring(ErrorMsg,_T("AWB Gb is %d ,not in <%d,%d>!"),Gb_Source1,m_GetOTPCheckSpec.m_MinGbValue,m_GetOTPCheckSpec.m_MaxGbValue);
		return FALSE;
	}
	if ((B_Source1 > m_GetOTPCheckSpec.m_MaxBValue) || (B_Source1 < m_GetOTPCheckSpec.m_MinBValue))
	{
		Catstring(ErrorMsg,_T("AWB B is %d ,not in <%d,%d>!"),B_Source1,m_GetOTPCheckSpec.m_MinBValue,m_GetOTPCheckSpec.m_MaxBValue);
		return FALSE;
	}
	

	//float RGainHigh = (RGainGolden_Source + (m_GetOTPCheckSpec.m_MaxRGRatio_UUT_diff*m_GetOTPCheckSpec.m_Multiple));//动态点检使用
	//float RGainlow  = (RGainGolden_Source + (m_GetOTPCheckSpec.m_MinRGRatio_UUT_diff*m_GetOTPCheckSpec.m_Multiple));
	//float BGainHigh = (BGainGolden_Source + (m_GetOTPCheckSpec.m_MaxBGRatio_UUT_diff*m_GetOTPCheckSpec.m_Multiple));
	//float BGainlow  = (BGainGolden_Source + (m_GetOTPCheckSpec.m_MinBGRatio_UUT_diff*m_GetOTPCheckSpec.m_Multiple));

	if( (RGain_Source1 > m_GetOTPCheckSpec.m_MaxRGain) || ( RGain_Source1 < m_GetOTPCheckSpec.m_MinRGain ))
	{
		Catstring(ErrorMsg,_T("AWB RGain is %d ,not in <%d,%d>!"),RGain_Source1,m_GetOTPCheckSpec.m_MinRGain, m_GetOTPCheckSpec.m_MaxRGain);
		return FALSE;
	}	
	if( (BGain_Source1 > m_GetOTPCheckSpec.m_MaxBGain) || ( BGain_Source1 < m_GetOTPCheckSpec.m_MinBGain ) )
	{
		Catstring(ErrorMsg,_T("AWB BGain is %d ,not in <%d,%d>!"),BGain_Source1,m_GetOTPCheckSpec.m_MinBGain,m_GetOTPCheckSpec.m_MaxBGain);
		return FALSE;
	}
	if ((GGain_Source1 > m_GetOTPCheckSpec.m_MaxGGGain) || (GGain_Source1 < m_GetOTPCheckSpec.m_MinGGGain))
	{
		Catstring(ErrorMsg,_T("AWB GGain is %d ,not in <%d,%d>!"),GGain_Source1,m_GetOTPCheckSpec.m_MinGGGain,m_GetOTPCheckSpec.m_MaxGGGain);
		return FALSE;
	}


	if ((RGolden_Source > m_GetOTPCheckSpec.m_MaxRGolden) || (RGolden_Source < m_GetOTPCheckSpec.m_MinRGolden))
	{
		Catstring(ErrorMsg,_T("AWB Golden_R is %d ,not in <%d,%d>!"),RGolden_Source,m_GetOTPCheckSpec.m_MinRGolden,m_GetOTPCheckSpec.m_MaxRGolden);
		return FALSE;
	}
	if ((GrGolden_Source > m_GetOTPCheckSpec.m_MaxGrGolden) || (GrGolden_Source < m_GetOTPCheckSpec.m_MinGrGolden))
	{
		Catstring(ErrorMsg,_T("AWB Golden_Gr is %d ,not in <%d,%d>!"),GrGolden_Source,m_GetOTPCheckSpec.m_MinGrGolden,m_GetOTPCheckSpec.m_MaxGrGolden);
		return FALSE;
	}
	if ((GbGolden_Source > m_GetOTPCheckSpec.m_MaxGbGolden) || (GbGolden_Source < m_GetOTPCheckSpec.m_MinGbGolden))
	{
		Catstring(ErrorMsg,_T("AWB Golden_Gb is %d ,not in <%d,%d>!"),GbGolden_Source,m_GetOTPCheckSpec.m_MinGbGolden,m_GetOTPCheckSpec.m_MaxGbGolden);
		return FALSE;
	}
	if ((BGolden_Source > m_GetOTPCheckSpec.m_MaxBGolden) || (BGolden_Source < m_GetOTPCheckSpec.m_MinBGolden))
	{
		Catstring(ErrorMsg,_T("AWB Golden_B is %d ,not in <%d,%d>!"),BGolden_Source,m_GetOTPCheckSpec.m_MinBGolden,m_GetOTPCheckSpec.m_MaxBGolden);
		return FALSE;
	}
	if ((RGainGolden_Source > m_GetOTPCheckSpec.m_MaxRGainGolden) || (RGainGolden_Source < m_GetOTPCheckSpec.m_MinRGainGolden))
	{
		Catstring(ErrorMsg,_T("AWB Golden_RGain is %d ,not in <%d,%d>!"),RGainGolden_Source,m_GetOTPCheckSpec.m_MinRGainGolden,m_GetOTPCheckSpec.m_MaxRGainGolden);
		return FALSE;
	}
	if ((BGainGolden_Source > m_GetOTPCheckSpec.m_MaxBGainGolden) || (BGainGolden_Source < m_GetOTPCheckSpec.m_MinBGainGolden))
	{
		Catstring(ErrorMsg,_T("AWB Golden_BGain is %d ,not in <%d,%d>!"),BGainGolden_Source,m_GetOTPCheckSpec.m_MinBGainGolden,m_GetOTPCheckSpec.m_MaxBGainGolden);
		return FALSE;
	}
	if ((GGainGolden_Source > m_GetOTPCheckSpec.m_MaxGGainGolden) || (GGainGolden_Source < m_GetOTPCheckSpec.m_MinGGainGolden))
	{
		Catstring(ErrorMsg,_T("AWB Golden_GGain is %d ,not in <%d,%d>!"),GGainGolden_Source,m_GetOTPCheckSpec.m_MinGGainGolden,m_GetOTPCheckSpec.m_MaxGGainGolden);
		return FALSE;
	}

	int CRC = GetCRC16(&OTPReadData[AWB_StartAdd],AWB_EndAdd - AWB_StartAdd - 1); 
	if (((OTPReadData[AWB_EndAdd-1]<<8) + (OTPReadData[AWB_EndAdd])) != CRC)
	{
		Catstring(ErrorMsg,_T("AWB CheckSum is 0x%04X != 0x%04X"),(OTPReadData[AWB_EndAdd-1]<<8) + (OTPReadData[AWB_EndAdd]),CRC);
		return FALSE;
	}
	return TRUE;

}

BOOL OtpCheckAPI::CheckAWB_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (OTPReadData[0x0044] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of WB Calibration is 0x%02X != 0x01"),OTPReadData[0x0044]);
		return FALSE;
	}

	if (OTPReadData[0x0045] != 0xFF)
	{
		Catstring(ErrorMsg,_T("WB Equip number is 0x%02X != 0xFF"),OTPReadData[0x0045]);
		return FALSE;
	}

	int RG_Ratio_5100k = (OTPReadData[0x0046] << 8) + OTPReadData[0x0047];
	int BG_Ratio_5100k = (OTPReadData[0x0048] << 8) + OTPReadData[0x0049];
	int GG_Ratio_5100k = (OTPReadData[0x004A] << 8) + OTPReadData[0x004B];

	int RG_Gain_5100k = (OTPReadData[0x004C] << 8) + OTPReadData[0x004D];
	int BG_Gain_5100k = (OTPReadData[0x004E] << 8) + OTPReadData[0x004F];
	int GG_Gain_5100k = (OTPReadData[0x0050] << 8) + OTPReadData[0x0051];

	int RG_Ratio_4100k = (OTPReadData[0x0052] << 8) + OTPReadData[0x0053];
	int BG_Ratio_4100k = (OTPReadData[0x0054] << 8) + OTPReadData[0x0055];
	int GG_Ratio_4100k = (OTPReadData[0x0056] << 8) + OTPReadData[0x0057];

	int RG_Gain_4100k = (OTPReadData[0x0058] << 8) + OTPReadData[0x0059];
	int BG_Gain_4100k = (OTPReadData[0x005A] << 8) + OTPReadData[0x005B];
	int GG_Gain_4100k = (OTPReadData[0x005C] << 8) + OTPReadData[0x005D];

	int RG_Ratio_3100k = (OTPReadData[0x005E] << 8) + OTPReadData[0x005F];
	int BG_Ratio_3100k = (OTPReadData[0x0060] << 8) + OTPReadData[0x0061];
	int GG_Ratio_3100k = (OTPReadData[0x0062] << 8) + OTPReadData[0x0063];

	int RG_Gain_3100k = (OTPReadData[0x0064] << 8) + OTPReadData[0x0065];
	int BG_Gain_3100k = (OTPReadData[0x0066] << 8) + OTPReadData[0x0067];
	int GG_Gain_3100k = (OTPReadData[0x0068] << 8) + OTPReadData[0x0069];

	if ((RG_Ratio_5100k != RG_Ratio_4100k) || (BG_Ratio_5100k != BG_Ratio_4100k) || (GG_Ratio_5100k != GG_Ratio_4100k) 
		|| (RG_Gain_5100k != RG_Gain_4100k) || (BG_Gain_5100k != BG_Gain_4100k) || (GG_Gain_5100k != GG_Gain_4100k))
	{
		Catstring(ErrorMsg,_T("5100K RG Ratio is ,not in <%d,%d>!"),RG_Ratio_5100k,RG_Ratio_4100k);
		Catstring(ErrorMsg,_T("5100K BG Ratio is ,not in <%d,%d>!"),BG_Ratio_5100k,BG_Ratio_4100k);
		Catstring(ErrorMsg,_T("5100K GG Ratio is ,not in <%d,%d>!"),GG_Ratio_5100k,GG_Ratio_4100k);

		Catstring(ErrorMsg,_T("5100K RG Gain is ,not in <%d,%d>!"),RG_Gain_5100k,RG_Gain_4100k);
		Catstring(ErrorMsg,_T("5100K BG Gain is ,not in <%d,%d>!"),BG_Gain_5100k,BG_Gain_4100k);
		Catstring(ErrorMsg,_T("5100K GG Gain is ,not in <%d,%d>!"),GG_Gain_5100k,GG_Gain_5100k);

		return FALSE;
	}

	//5100K
	if( (RG_Ratio_5100k > m_GetOTPCheckSpec1.m_MaxRGRatio_HighCCT) || ( RG_Ratio_5100k < m_GetOTPCheckSpec1.m_MinRGRatio_HighCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB RG Ratio(HighCCT)=%d,not in<%d,%d>"),RG_Ratio_5100k,m_GetOTPCheckSpec1.m_MinRGRatio_HighCCT,m_GetOTPCheckSpec1.m_MaxRGRatio_HighCCT);
		Catstring(ErrorMsg,_T("AWB RG Ratio(HighCCT) 值超范围!"));
		return FALSE;
	} 

	if( (BG_Ratio_5100k > m_GetOTPCheckSpec1.m_MaxBGRatio_HighCCT) || ( BG_Ratio_5100k < m_GetOTPCheckSpec1.m_MinBGRatio_HighCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB BG Ratio(HighCCT)=%d,not in<%d,%d>"),BG_Ratio_5100k,m_GetOTPCheckSpec1.m_MinBGRatio_HighCCT,m_GetOTPCheckSpec1.m_MaxBGRatio_HighCCT);
		Catstring(ErrorMsg,_T("AWB BG Ratio(HighCCT) 值超范围!"));
		return FALSE;
	} 

	if( (GG_Ratio_5100k > m_GetOTPCheckSpec1.m_MaxGGRatio_HighCCT) || ( GG_Ratio_5100k < m_GetOTPCheckSpec1.m_MinGGRatio_HighCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB GG Ratio(HighCCT)=%d,not in<%d,%d>"),GG_Ratio_5100k,m_GetOTPCheckSpec1.m_MinGGRatio_HighCCT,m_GetOTPCheckSpec1.m_MaxGGRatio_HighCCT);
		Catstring(ErrorMsg,_T("AWB GG Ratio(HighCCT) 值超范围!"));
		return FALSE;
	} 

	if( (RG_Gain_5100k > m_GetOTPCheckSpec1.m_MaxRGain_HighCCT) || ( RG_Gain_5100k < m_GetOTPCheckSpec1.m_MinRGain_HighCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB RG GAIN (HighCCT)=%d,not in<%d,%d>"),RG_Gain_5100k,m_GetOTPCheckSpec1.m_MinRGain_HighCCT,m_GetOTPCheckSpec1.m_MaxRGain_HighCCT);
		Catstring(ErrorMsg,_T("AWB RG GAIN (HighCCT) 值超范围!"));
		return FALSE;
	} 

	if( (BG_Gain_5100k > m_GetOTPCheckSpec1.m_MaxBGain_HighCCT) || ( BG_Gain_5100k < m_GetOTPCheckSpec1.m_MinBGain_HighCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB BG GAIN (HighCCT)=%d,not in<%d,%d>"),BG_Gain_5100k,m_GetOTPCheckSpec1.m_MinBGain_HighCCT,m_GetOTPCheckSpec1.m_MaxBGain_HighCCT);
		Catstring(ErrorMsg,_T("AWB BG GAIN (HighCCT) 值超范围!"));
		return FALSE;
	} 

	if( (GG_Gain_5100k > m_GetOTPCheckSpec1.m_MaxGGGain_HighCCT) || ( GG_Gain_5100k < m_GetOTPCheckSpec1.m_MinGGGain_HighCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB GG GAIN (HighCCT)=%d,not in<%d,%d>"),GG_Gain_5100k,m_GetOTPCheckSpec1.m_MinGGGain_HighCCT,m_GetOTPCheckSpec1.m_MaxGGGain_HighCCT);
		Catstring(ErrorMsg,_T("AWB GG GAIN (HighCCT) 值超范围!"));
		return FALSE;
	} 

	//3100K
	if( (RG_Ratio_3100k > m_GetOTPCheckSpec1.m_MaxRGRatio_LowCCT) || ( RG_Ratio_3100k < m_GetOTPCheckSpec1.m_MinRGRatio_LowCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB RG Ratio(LowCCT)=%d,not in<%d,%d>"),RG_Ratio_3100k,m_GetOTPCheckSpec1.m_MinRGRatio_LowCCT,m_GetOTPCheckSpec1.m_MaxRGRatio_LowCCT);
		Catstring(ErrorMsg,_T("AWB RG Ratio(LowCCT) 值超范围!"));
		return FALSE;
	} 

	if( (BG_Ratio_3100k > m_GetOTPCheckSpec1.m_MaxBGRatio_LowCCT) || ( BG_Ratio_3100k < m_GetOTPCheckSpec1.m_MinBGRatio_LowCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB BG Ratio(LowCCT)=%d,not in<%d,%d>"),BG_Ratio_3100k,m_GetOTPCheckSpec1.m_MinBGRatio_LowCCT,m_GetOTPCheckSpec1.m_MaxBGRatio_LowCCT);
		Catstring(ErrorMsg,_T("AWB BG Ratio(LowCCT) 值超范围!"));
		return FALSE;
	} 

	if( (GG_Ratio_3100k > m_GetOTPCheckSpec1.m_MaxGGRatio_LowCCT) || ( GG_Ratio_3100k < m_GetOTPCheckSpec1.m_MinGGRatio_LowCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB GG Ratio(LowCCT)=%d,not in<%d,%d>"),GG_Ratio_3100k,m_GetOTPCheckSpec1.m_MinGGRatio_LowCCT,m_GetOTPCheckSpec1.m_MaxGGRatio_LowCCT);
		Catstring(ErrorMsg,_T("AWB GG Ratio(LowCCT) 值超范围!"));
		return FALSE;
	} 

	if( (RG_Gain_3100k > m_GetOTPCheckSpec1.m_MaxRGain_LowCCT) || ( RG_Gain_3100k < m_GetOTPCheckSpec1.m_MinRGain_LowCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB RG GAIN (LowCCT)=%d,not in<%d,%d>"),RG_Gain_3100k,m_GetOTPCheckSpec1.m_MinRGain_LowCCT,m_GetOTPCheckSpec1.m_MaxRGain_LowCCT);
		Catstring(ErrorMsg,_T("AWB RG GAIN (LowCCT) 值超范围!"));
		return FALSE;
	} 

	if( (BG_Gain_3100k > m_GetOTPCheckSpec1.m_MaxBGain_LowCCT) || ( BG_Gain_3100k < m_GetOTPCheckSpec1.m_MinBGain_LowCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB BG GAIN (LowCCT)=%d,not in<%d,%d>"),BG_Gain_3100k,m_GetOTPCheckSpec1.m_MinBGain_LowCCT,m_GetOTPCheckSpec1.m_MaxBGain_LowCCT);
		Catstring(ErrorMsg,_T("AWB BG GAIN (LowCCT) 值超范围!"));
		return FALSE;
	} 

	if( (GG_Gain_3100k > m_GetOTPCheckSpec1.m_MaxGGGain_LowCCT) || ( GG_Gain_3100k < m_GetOTPCheckSpec1.m_MinGGGain_LowCCT ) )
	{
		Catstring(ErrorMsg,_T("AWB GG GAIN (LowCCT)=%d,not in<%d,%d>"),GG_Gain_3100k,m_GetOTPCheckSpec1.m_MinGGGain_LowCCT,m_GetOTPCheckSpec1.m_MaxGGGain_LowCCT);
		Catstring(ErrorMsg,_T("AWB GG GAIN (LowCCT) 值超范围!"));
		return FALSE;
	} 

	int AwbCheckSum = 0;
	for (i = 0x0044; i < 0x006A; i++)
	{
		AwbCheckSum = AwbCheckSum + OTPReadData[i];
	}

	if ((OTPReadData[0x006A] != ((AwbCheckSum >> 8) & 0xff)) || (OTPReadData[0x006B] != (AwbCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n AF Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x006A],OTPReadData[0x006B],AwbCheckSum);
		return FALSE;
	}
	//m_pInterface->Ctrl_Addlog(CamID,_T("WB Check Pass!"),COLOR_BLUE,200);
	return TRUE;
#if 0
	//check AWB
	int m_value = 0;
	int RGain		= ((OTPReadData[0x005A]<<8)&0xFF00) +OTPReadData[0x005B];
	int BGain		= ((OTPReadData[0x005C]<<8)&0xFF00) +OTPReadData[0x005D];
	int GGGain		= ((OTPReadData[0x005E]<<8)&0xFF00) +OTPReadData[0x005F];
	int RBGain		= int(1000.0*float(RGain)/float(BGain));

	//  check module have equal value 
	int m_RGainjudgevalue = CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("RGainEqualCount"),_T("RGainEqualRef"),RGain,m_GetOTPCheckSpec1.m_EqualCountSpec,ErrorMsg);
	if (!m_RGainjudgevalue)
	{
		return FALSE;
	}
	int m_BGainjudgevalue = CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("BGainEqualCount"),_T("BGainEqualRef"),BGain,m_GetOTPCheckSpec1.m_EqualCountSpec,ErrorMsg);
	if (!m_BGainjudgevalue)
	{
		return FALSE;
	}
	int m_GGainjudgevalue = CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("GGainEqualCount"),_T("GGainEqualRef"),GGGain,m_GetOTPCheckSpec1.m_EqualCountSpec,ErrorMsg);
	if (!m_GGainjudgevalue)
	{
		return FALSE;
	}	

	m_value = ((OTPReadData[0x0054]<<8)&0xFF00) +OTPReadData[0x0055];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentAWBTagID)
	{
		Catstring(ErrorMsg, _T("m_SegmentAWBTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentAWBTagID);
		return FALSE;
	}
	if (OTPReadData[0x0056] != m_GetOTPCheckSpec1.m_AWBFlag)
	{
		Catstring(ErrorMsg, _T("AWBFlag 0x%02X != 0x%02X"),OTPReadData[0x0056],m_GetOTPCheckSpec1.m_AWBFlag);
		return FALSE;
	}
	if (OTPReadData[0x0057] != m_GetOTPCheckSpec1.m_AWBTypeNumber)
	{
		Catstring(ErrorMsg, _T("AWBTypeNumber 0x%02X != 0x%02X"),OTPReadData[0x0057],m_GetOTPCheckSpec1.m_AWBTypeNumber);
		return FALSE;
	}
	if (OTPReadData[0x0058] != m_GetOTPCheckSpec1.m_AWBMirror)
	{
		Catstring(ErrorMsg, _T("m_AWBMirror 0x%02X != 0x%02X"),OTPReadData[0x0058],m_GetOTPCheckSpec1.m_AWBMirror);
		return FALSE;
	}
	if (OTPReadData[0x0059] != m_GetOTPCheckSpec1.m_AWBFilp)
	{
		Catstring(ErrorMsg, _T("m_AWBFilp 0x%02X != 0x%02X"),OTPReadData[0x0059],m_GetOTPCheckSpec1.m_AWBFilp);
		return FALSE;
	}

	if ((RGain > m_GetOTPCheckSpec1.m_MaxRGain) || (RGain < m_GetOTPCheckSpec1.m_MinRGain))
	{
		Catstring(ErrorMsg, _T("RGain %d not in<%d,%d>"),RGain,m_GetOTPCheckSpec1.m_MinRGain,m_GetOTPCheckSpec1.m_MaxRGain);
		return FALSE;
	}
	if ((BGain > m_GetOTPCheckSpec1.m_MaxBGain) || (BGain < m_GetOTPCheckSpec1.m_MinBGain))
	{
		Catstring(ErrorMsg, _T("BGain %d not in<%d,%d>"),BGain,m_GetOTPCheckSpec1.m_MinBGain,m_GetOTPCheckSpec1.m_MaxBGain);
		return FALSE;
	}
	if ((GGGain > m_GetOTPCheckSpec1.m_MaxGGGain) || (GGGain < m_GetOTPCheckSpec1.m_MinGGGain))
	{
		Catstring(ErrorMsg, _T("GGGain %d not in<%d,%d>"),GGGain,m_GetOTPCheckSpec1.m_MinGGGain,m_GetOTPCheckSpec1.m_MaxGGGain);
		return FALSE;
	}	
	if ((RBGain > m_GetOTPCheckSpec1.m_MaxRBGain) || (RBGain < m_GetOTPCheckSpec1.m_MinRBGain))
	{
		Catstring(ErrorMsg, _T("RBGain %d not in<%d,%d>"),RBGain,m_GetOTPCheckSpec1.m_MinRBGain,m_GetOTPCheckSpec1.m_MaxRBGain);
		return FALSE;
	}

	//check AWB checksum
	int m_AWBSum = 0;
	for (int i = 0x0057; i < 0x0060; i++)
	{
		m_AWBSum += OTPReadData[i];
	}
	m_AWBSum = (m_AWBSum%255) +1;
	if (m_AWBSum != OTPReadData[0x0060])
	{
		Catstring(ErrorMsg,_T("AWB CheckSum 0x%02X != 0x%02X"),OTPReadData[0x0060],m_AWBSum);
		return FALSE;
	}		

	return TRUE;
#endif
}

BOOL OtpCheckAPI::CheckHisiLSC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//华为专用
	if (m_GetOTPCheckSpec.m_HisiMasterFlag)
	{
		//卡控Hisi LSC数据不允许存在多个连续相等值
		int m_DataEqualRef = OTPReadData[0x0038];
		int m_DataEqualCount = 0;	
		int m_LSCCheckSum = 0;
		for (int i = 0x0038;i < 0x0AE9;i++)
		{
			m_LSCCheckSum += OTPReadData[i];

			if (OTPReadData[i] == m_DataEqualRef)
			{
				m_DataEqualCount++;
				if (m_DataEqualCount > 5)
				{
					Catstring(ErrorMsg,_T("Hisi LSC数据存在%d个连续相等的值，规格为5个!"),m_DataEqualCount);
					m_DataEqualCount = 0;
					return FALSE;
				}
			}
			else
			{
				m_DataEqualCount = 0;
				m_DataEqualRef = OTPReadData[i];
			}
		}
		//Check LSCCheckSum
		m_LSCCheckSum = (m_LSCCheckSum % 65535);
		if (((m_LSCCheckSum>>8)&0xFF) != OTPReadData[0x0AE9])
		{
			Catstring(ErrorMsg,_T("LSC CheckSum 0x0AE9 is 0x%02X != 0x%02X"),OTPReadData[0x0AE9],(m_LSCCheckSum>>8)&0xFF);
			return FALSE;
		}
		if ((m_LSCCheckSum&0xFF) != OTPReadData[0x0AEA])
		{
			Catstring(ErrorMsg,_T("LSC CheckSum 0x0AEA is 0x%02X != 0x%02X"),OTPReadData[0x0AEA],m_LSCCheckSum&0xFF);
			return FALSE;
		}
	}
	else
	{
		int m_LSCCheckSum = 0;
		for (int i = 0x0038;i < 0x0AE9;i++)
		{
			if ((OTPReadData[i] != 0x00) || (OTPReadData[i] != 0xFF))
			{
				Catstring(ErrorMsg,_T("No MasterModule,LSC data should be 0x00/0xFF."));
				return FALSE;
			}
			m_LSCCheckSum += OTPReadData[i];
		}
		m_LSCCheckSum = (m_LSCCheckSum % 65535);
		if (((m_LSCCheckSum>>8)&0xFF) != OTPReadData[0x0AE9])
		{
			Catstring(ErrorMsg,_T("LSC CheckSum 0x0AE9 is 0x%02X != 0x%02X"),OTPReadData[0x0AE9],(m_LSCCheckSum>>8)&0xFF);
			return FALSE;
		}
		if ((m_LSCCheckSum&0xFF) != OTPReadData[0x0AEA])
		{
			Catstring(ErrorMsg,_T("LSC CheckSum 0x0AEA is 0x%02X != 0x%02X"),OTPReadData[0x0AEA],m_LSCCheckSum&0xFF);
			return FALSE;
		}
	}

	if ((OTPReadData[0x0AEB] < 17) || (OTPReadData[0x0AEB] > 21) ||
		(OTPReadData[0x0AEC] < 1)  || (OTPReadData[0x0AEC] > 12) ||
		(OTPReadData[0x0AED] < 1)  || (OTPReadData[0x0AED] > 31) ||
		(OTPReadData[0x0AEE] < 0)  || (OTPReadData[0x0AEE] > 23))
	{
		Catstring(ErrorMsg, _T("LSC Station Time Info NG."));
		return FALSE;
	}
	//check LSC Station Info Check sum
	int m_LSCInfoSum  = 0;
	for (int i = 0x0AEB; i < 0x0AF1; i++)
	{
		m_LSCInfoSum += OTPReadData[i];
	}
	m_LSCInfoSum = (m_LSCInfoSum)%65535;
	if (((m_LSCInfoSum>>8)&0xFF) != OTPReadData[0x0AF1])
	{
		Catstring(ErrorMsg, _T("LSC Station Info CheckSum 0x0AF1 is 0x%02X != 0x%02X"),OTPReadData[0x0AF1],(m_LSCInfoSum>>8)&0xFF);
		return FALSE;
	}
	if ((m_LSCInfoSum&0xFF) != OTPReadData[0x0AF2])
	{
		Catstring(ErrorMsg, _T("LSC Station Info CheckSum 0x0AF2 is 0x%02X != 0x%02X"),OTPReadData[0x0AF2],m_LSCInfoSum&0xFF);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckHisiLSC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//华为专用
	if (m_GetOTPCheckSpec1.m_HisiMasterFlag)
	{
		//卡控Hisi LSC数据不允许存在多个连续相等值
		int m_DataEqualRef = OTPReadData[0x0038];
		int m_DataEqualCount = 0;	
		int m_LSCCheckSum = 0;
		for (int i = 0x0038;i < 0x0AE9;i++)
		{
			m_LSCCheckSum += OTPReadData[i];

			if (OTPReadData[i] == m_DataEqualRef)
			{
				m_DataEqualCount++;
				if (m_DataEqualCount > 5)
				{
					Catstring(ErrorMsg,_T("Hisi LSC数据存在%d个连续相等的值，规格为5个!"),m_DataEqualCount);
					m_DataEqualCount = 0;
					return FALSE;
				}
			}
			else
			{
				m_DataEqualCount = 0;
				m_DataEqualRef = OTPReadData[i];
			}
		}
		//Check LSCCheckSum
		m_LSCCheckSum = (m_LSCCheckSum % 65535);
		if (((m_LSCCheckSum>>8)&0xFF) != OTPReadData[0x0AE9])
		{
			Catstring(ErrorMsg,_T("LSC CheckSum 0x0AE9 is 0x%02X != 0x%02X"),OTPReadData[0x0AE9],(m_LSCCheckSum>>8)&0xFF);
			return FALSE;
		}
		if ((m_LSCCheckSum&0xFF) != OTPReadData[0x0AEA])
		{
			Catstring(ErrorMsg,_T("LSC CheckSum 0x0AEA is 0x%02X != 0x%02X"),OTPReadData[0x0AEA],m_LSCCheckSum&0xFF);
			return FALSE;
		}
	}
	else
	{
		int m_LSCCheckSum = 0;
		for (int i = 0x0038;i < 0x0AE9;i++)
		{
			if ((OTPReadData[i] != 0x00) || (OTPReadData[i] != 0xFF))
			{
				Catstring(ErrorMsg,_T("No MasterModule,LSC data should be 0x00/0xFF."));
				return FALSE;
			}
			m_LSCCheckSum += OTPReadData[i];
		}
		m_LSCCheckSum = (m_LSCCheckSum % 65535);
		if (((m_LSCCheckSum>>8)&0xFF) != OTPReadData[0x0AE9])
		{
			Catstring(ErrorMsg,_T("LSC CheckSum 0x0AE9 is 0x%02X != 0x%02X"),OTPReadData[0x0AE9],(m_LSCCheckSum>>8)&0xFF);
			return FALSE;
		}
		if ((m_LSCCheckSum&0xFF) != OTPReadData[0x0AEA])
		{
			Catstring(ErrorMsg,_T("LSC CheckSum 0x0AEA is 0x%02X != 0x%02X"),OTPReadData[0x0AEA],m_LSCCheckSum&0xFF);
			return FALSE;
		}
	}

	if ((OTPReadData[0x0AEB] < 17) || (OTPReadData[0x0AEB] > 21) ||
		(OTPReadData[0x0AEC] < 1)  || (OTPReadData[0x0AEC] > 12) ||
		(OTPReadData[0x0AED] < 1)  || (OTPReadData[0x0AED] > 31) ||
		(OTPReadData[0x0AEE] < 0)  || (OTPReadData[0x0AEE] > 23))
	{
		Catstring(ErrorMsg, _T("LSC Station Time Info NG."));
		return FALSE;
	}
	//check LSC Station Info Check sum
	int m_LSCInfoSum  = 0;
	for (int i = 0x0AEB; i < 0x0AF1; i++)
	{
		m_LSCInfoSum += OTPReadData[i];
	}
	m_LSCInfoSum = (m_LSCInfoSum)%65535;
	if (((m_LSCInfoSum>>8)&0xFF) != OTPReadData[0x0AF1])
	{
		Catstring(ErrorMsg, _T("LSC Station Info CheckSum 0x0AF1 is 0x%02X != 0x%02X"),OTPReadData[0x0AF1],(m_LSCInfoSum>>8)&0xFF);
		return FALSE;
	}
	if ((m_LSCInfoSum&0xFF) != OTPReadData[0x0AF2])
	{
		Catstring(ErrorMsg, _T("LSC Station Info CheckSum 0x0AF2 is 0x%02X != 0x%02X"),OTPReadData[0x0AF2],m_LSCInfoSum&0xFF);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckHisiHDC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check HDC Check sum
	int m_HDCInfoSum  = 0;
	for (int i = 0x0AF3; i < 0x0D4B; i++)
	{
		m_HDCInfoSum += OTPReadData[i];
	}
	m_HDCInfoSum = (m_HDCInfoSum)%65535;
	if (((m_HDCInfoSum>>8)&0xFF) != OTPReadData[0x0D4B])
	{
		Catstring(ErrorMsg, _T("HDC CheckSum 0x0D4B is 0x%02X != 0x%02X"),OTPReadData[0x0D4B],(m_HDCInfoSum>>8)&0xFF);
		return FALSE;
	}
	if ((m_HDCInfoSum&0xFF) != OTPReadData[0x0D4C])
	{
		Catstring(ErrorMsg, _T("HDC CheckSum 0x0D4C is 0x%02X != 0x%02X"),OTPReadData[0x0D4C],m_HDCInfoSum&0xFF);
		return FALSE;
	}

	if ((OTPReadData[0x0D4D] < 17) || (OTPReadData[0x0D4D] > 21) ||
		(OTPReadData[0x0D4E] < 1)  || (OTPReadData[0x0D4E] > 12) ||
		(OTPReadData[0x0D4F] < 1)  || (OTPReadData[0x0D4F] > 31) ||
		(OTPReadData[0x0D50] < 0)  || (OTPReadData[0x0D50] > 23))
	{
		Catstring(ErrorMsg, _T("HDC Station Time Info NG."));
		return FALSE;
	}

	int m_HDCInfoCheckSum = 0;
	for (int i = 0x0D4D;i < 0x0D53;i++)
	{
		m_HDCInfoCheckSum += OTPReadData[i];
	}
	m_HDCInfoCheckSum = (m_HDCInfoCheckSum%65535);
	if (((m_HDCInfoCheckSum>>8)&0xFF) != OTPReadData[0x0D53])
	{
		Catstring(ErrorMsg, _T("HDC Info CheckSum 0x0D53 is 0x%02X != 0x%02X"),OTPReadData[0x0D53],(m_HDCInfoCheckSum>>8)&0xFF);
		return FALSE;
	}
	if ((m_HDCInfoCheckSum&0xFF) != OTPReadData[0x0D54])
	{
		Catstring(ErrorMsg, _T("HDC Info CheckSum 0x0D54 is 0x%02X != 0x%02X"),OTPReadData[0x0D54],m_HDCInfoCheckSum&0xFF);
		return FALSE;
	}

	//check Total Checksum
	int m_TotalCheckSum = 0;
	for (int i = 0x0000;i < 0x0D55;i++)
	{
		m_TotalCheckSum += OTPReadData[i];
	}
	m_TotalCheckSum = (m_TotalCheckSum%65535);
	if (((m_TotalCheckSum>>8)&0xFF) != OTPReadData[0x0D55])
	{
		Catstring(ErrorMsg, _T("Total CheckSum 0x0D55 is 0x%02X != 0x%02X"),OTPReadData[0x0D55],(m_TotalCheckSum>>8)&0xFF);
		return FALSE;
	}
	if ((m_TotalCheckSum&0xFF) != OTPReadData[0x0D56])
	{
		Catstring(ErrorMsg, _T("Total CheckSum 0x0D56 is 0x%02X != 0x%02X"),OTPReadData[0x0D56],m_TotalCheckSum&0xFF);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommLSC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	USHORT LSC_RChannel[221]  = {0};
	USHORT LSC_GrChannel[221] = {0};
	USHORT LSC_GbChannel[221] = {0};
	USHORT LSC_BChannel[221]  = {0};
	int i = 0;
	int j = 0;

#if LSC_MODE == LSC_MODE_2
	int R_max, Gr_max, Gb_max, B_max;

	R_max = (OTPReadData[LSC_EndAdd-9] << 8) | OTPReadData[LSC_EndAdd-8];
	Gr_max = (OTPReadData[LSC_EndAdd-7] << 8) | OTPReadData[LSC_EndAdd-6];
	Gb_max = (OTPReadData[LSC_EndAdd-5] << 8) | OTPReadData[LSC_EndAdd-4];
	B_max = (OTPReadData[LSC_EndAdd-3] << 8) | OTPReadData[LSC_EndAdd-2];
#endif

	for (i = 0; i < 221; i++)
	{
	#if LSC_MODE == LSC_MODE_1
		LSC_RChannel[i]  = ((OTPReadData[LSC_StartAdd+i*2]<<8)&0xFF00) + OTPReadData[LSC_StartAdd+i*2+1];
		LSC_GrChannel[i] = ((OTPReadData[LSC_StartAdd+i*2+442]<<8)&0xFF00)+(OTPReadData[LSC_StartAdd+i*2+443]);
		LSC_GbChannel[i] = ((OTPReadData[LSC_StartAdd+i*2+884]<<8)&0xFF00)+(OTPReadData[LSC_StartAdd+i*2+885]);
		LSC_BChannel[i]  = ((OTPReadData[LSC_StartAdd+i*2+1326]<<8)&0xFF00)+(OTPReadData[LSC_StartAdd+i*2+1327]);
	#else
		int Tmp[4] = {0};

		Tmp[0]  = ((OTPReadData[LSC_StartAdd+i]<<8)&0xFF00)+(OTPReadData[LSC_StartAdd+i+1]);
		Tmp[1]  = ((OTPReadData[LSC_StartAdd+i+442]<<8)&0xFF00)+(OTPReadData[LSC_StartAdd+i+443]);
		Tmp[2]	= ((OTPReadData[LSC_StartAdd+i+884]<<8)&0xFF00)+(OTPReadData[LSC_StartAdd+i+885]);
		Tmp[3]  = ((OTPReadData[LSC_StartAdd+i+1326]<<8)&0xFF00)+(OTPReadData[LSC_StartAdd+i+1327]);

		LSC_RChannel[i]  = (int)( (float)(Tmp[0] * R_max) / 1023.0 );
		LSC_GrChannel[i] = (int)( (float)(Tmp[1] * Gr_max) / 1023.0 );
		LSC_GbChannel[i] = (int)( (float)(Tmp[2] * Gb_max) / 1023.0 );
		LSC_BChannel[i]  = (int)( (float)(Tmp[3] * B_max) / 1023.0 );
	#endif
		//j = j + 5;
	}

	//以下函数中，若模组为单体使用："Standard.ini";双摄主摄使用"Standard_Master.ini"，副摄使用"Standard_Slave.ini";
	if(!LSCDataCheck_Project1(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel,ErrorMsg))
	{
		Catstring(ErrorMsg,_T("LSC Data Check FAIL!"));
		return FALSE;
	}

	if (m_GetOTPCheckSpec.m_LSCCurveCheck == 1)
	{
		if (!SaveLSCData(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel))
		{
			Catstring(ErrorMsg,_T("Save LSC Data fail!"));
			return FALSE;
		}
		BOOL m_judgement = CheckLSCData_LG(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel,1023, ErrorMsg,0);
		if (!m_judgement)
		{
			return FALSE;
		}  
	}
	else if (m_GetOTPCheckSpec.m_LSCCurveCheck == 2)
	{
		if (!SaveLSCData(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel))
		{
			Catstring(ErrorMsg,_T("Save LSC Data fail!"));
			return FALSE;
		}
		if (!LensVerificationLib(_T("R"), LSC_RChannel,ErrorMsg,TRUE,TRUE))   return FALSE;
		if (!LensVerificationLib(_T("Gr"),LSC_GrChannel,ErrorMsg,TRUE,TRUE))   return FALSE;
		if (!LensVerificationLib(_T("Gb"),LSC_GbChannel,ErrorMsg,TRUE,TRUE))   return FALSE;
		if (!LensVerificationLib(_T("B "),LSC_BChannel, ErrorMsg,TRUE,TRUE))   return FALSE;
	}


	// check LSC checksum
	int CRC = GetCRC16(&OTPReadData[LSC_StartAdd],LSC_EndAdd - LSC_StartAdd - 1); 
	if (((OTPReadData[LSC_EndAdd-1]<<8) + (OTPReadData[LSC_EndAdd])) != CRC)
	{
		Catstring(ErrorMsg,_T("LSC CheckSum is 0x%04X != 0x%04X"),(OTPReadData[LSC_EndAdd-1]<<8) + (OTPReadData[LSC_EndAdd]),CRC);
		return FALSE;
	}
	return TRUE;
#if 0
	USHORT LSC_RChannel[221] = {0};
	USHORT LSC_GrChannel[221] = {0};
	USHORT LSC_GbChannel[221] = {0};
	USHORT LSC_BChannel[221] = {0};

	int m_value = ((OTPReadData[0x0080]<<8)&0xFF00) +OTPReadData[0x0081];
	if (m_value != m_GetOTPCheckSpec.m_SegmentLSCTagID)
	{
		Catstring(ErrorMsg, _T("m_SegmentLSCTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentLSCTagID);
		return FALSE;
	}
	if (OTPReadData[0x0082] != m_GetOTPCheckSpec.m_LSCFlag)
	{
		Catstring(ErrorMsg, _T("m_LSCFlag 0x%02X != 0x%02X"),OTPReadData[0x0082],m_GetOTPCheckSpec.m_LSCFlag);
		return FALSE;
	}
	if (OTPReadData[0x0083] != m_GetOTPCheckSpec.m_LSCTypeNumber)
	{
		Catstring(ErrorMsg, _T("m_LSCVersionNumber 0x%02X != 0x%02X"),OTPReadData[0x0083],m_GetOTPCheckSpec.m_LSCTypeNumber);
		return FALSE;
	}
	if (OTPReadData[0x0084] != m_GetOTPCheckSpec.m_LSCMirror)
	{
		Catstring(ErrorMsg, _T("LSC Mirror 0x%02X != 0x%02X"),OTPReadData[0x0084],m_GetOTPCheckSpec.m_LSCMirror);
		return FALSE;
	}
	if (OTPReadData[0x0085] != m_GetOTPCheckSpec.m_LSCFilp)
	{
		Catstring(ErrorMsg, _T("LSC Filp 0x%02X != 0x%02X"),OTPReadData[0x0085],m_GetOTPCheckSpec.m_LSCFilp);
		return FALSE;
	}

	for (int mm = 0; mm < 221; mm++)
	{
		LSC_RChannel[mm]  = OTPReadData[0x86+5*mm]+((OTPReadData[0x8A+5*mm]&0xC0)<<2);
		LSC_GrChannel[mm] = OTPReadData[0x87+5*mm]+((OTPReadData[0x8A+5*mm]&0x30)<<4);
		LSC_GbChannel[mm] = OTPReadData[0x88+5*mm]+((OTPReadData[0x8A+5*mm]&0x0C)<<6);
		LSC_BChannel[mm]  = OTPReadData[0x89+5*mm]+((OTPReadData[0x8A+5*mm]&0x03)<<8);
	}

	//以下函数中，若模组为单体使用："Standard.ini";双摄主摄使用"Standard_Master.ini"，副摄使用"Standard_Slave.ini";
	if(!LSCDataCheck_Project1(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel,ErrorMsg))
	{
		Catstring(ErrorMsg,_T("LSC Data Check FAIL!"));
		return FALSE;
	}

	if (m_GetOTPCheckSpec.m_LSCCurveCheck == 1)
	{
		if (!SaveLSCData(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel))
		{
			Catstring(ErrorMsg,_T("Save LSC fail"));
			return FALSE;
		}
		BOOL m_judgement = CheckLSCData_LG(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel,1023, ErrorMsg,0);
		if (!m_judgement)
		{
			return FALSE;
		}  
	}
	else if (m_GetOTPCheckSpec.m_LSCCurveCheck == 2)
	{
		if (!SaveLSCData(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel))
		{
			Catstring(ErrorMsg,_T("Save LSC Data fail"));
			return FALSE;
		}
		if (!LensVerificationLib(_T("R"), LSC_RChannel, ErrorMsg,TRUE,TRUE))   return FALSE;
		if (!LensVerificationLib(_T("Gr"),LSC_GrChannel,ErrorMsg,TRUE,TRUE))   return FALSE;
		if (!LensVerificationLib(_T("Gb"),LSC_GbChannel,ErrorMsg,TRUE,TRUE))   return FALSE;
		if (!LensVerificationLib(_T("B"),LSC_BChannel, ErrorMsg,TRUE,TRUE))   return FALSE;
	}

	// check LSC checksum
	int m_LSCSum = 0;
	for (int i = 0x0083; i<0x04D7; i++)
	{
		m_LSCSum += OTPReadData[i];
	}
	m_LSCSum = (m_LSCSum%255) + 1;
	if (m_LSCSum != OTPReadData[0x04D7])
	{
		Catstring(ErrorMsg,_T("LSC CheckSum 0x%02X != 0x%02X"),OTPReadData[0x04D7],m_LSCSum);
		return FALSE;
	}

	return TRUE;
#endif
}

BOOL OtpCheckAPI::CheckQualcommLSC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	USHORT LSC_RChannel[221]  = {0};
	USHORT LSC_GrChannel[221] = {0};
	USHORT LSC_GbChannel[221] = {0};
	USHORT LSC_BChannel[221]  = {0};

#if LSC_MODE == LSC_MODE_2
	int R_max, Gr_max, Gb_max, B_max;

	R_max = (OTPReadData[0x04BF] << 8) | OTPReadData[0x4C0];
	Gr_max = (OTPReadData[0x04C1] << 8) | OTPReadData[0x4C2];
	Gb_max = (OTPReadData[0x04C3] << 8) | OTPReadData[0x4C4];
	B_max = (OTPReadData[0x04C5] << 8) | OTPReadData[0x4C6];
#endif

	int i = 0;
	int j = 0;
	for (i = 0; i < 221; i++)
	{
#if LSC_MODE == LSC_MODE_1
		LSC_RChannel[i]  = OTPReadData[0x006E + j] + ((OTPReadData[(0x006E + j) + 4] & 0x03) << 8);//((OTPReadData[0x001D+8*i]<<8)&0xFF00)+(OTPReadData[0x001E+8*i]);
		LSC_GrChannel[i] = OTPReadData[(0x006E + j)+1] + ((OTPReadData[(0x006E + j) + 4] & 0x0c) << 6);//((OTPReadData[0x001F+8*i]<<8)&0xFF00)+(OTPReadData[0x0020+8*i]);
		LSC_GbChannel[i] = OTPReadData[(0x006E + j)+2] + ((OTPReadData[(0x006E + j) + 4] & 0x30) << 4);//((OTPReadData[0x0021+8*i]<<8)&0xFF00)+(OTPReadData[0x0022+8*i]);
		LSC_BChannel[i]  = OTPReadData[(0x006E + j)+3] + ((OTPReadData[(0x006E + j) + 4] & 0xc0) << 2);//((OTPReadData[0x0023+8*i]<<8)&0xFF00)+(OTPReadData[0x0024+8*i]);
#else
		int Tmp[4] = {0};

		Tmp[0]  = OTPReadData[0x006E + j] + ((OTPReadData[(0x006E + j) + 4] & 0x03) << 8);//((OTPReadData[0x001D+8*i]<<8)&0xFF00)+(OTPReadData[0x001E+8*i]);
		Tmp[1]  = OTPReadData[(0x006E + j)+1] + ((OTPReadData[(0x006E + j) + 4] & 0x0c) << 6);//((OTPReadData[0x001F+8*i]<<8)&0xFF00)+(OTPReadData[0x0020+8*i]);
		Tmp[2] = OTPReadData[(0x006E + j)+2] + ((OTPReadData[(0x006E + j) + 4] & 0x30) << 4);//((OTPReadData[0x0021+8*i]<<8)&0xFF00)+(OTPReadData[0x0022+8*i]);
		Tmp[3]  = OTPReadData[(0x006E + j)+3] + ((OTPReadData[(0x006E + j) + 4] & 0xc0) << 2);//((OTPReadData[0x0023+8*i]<<8)&0xFF00)+(OTPReadData[0x0024+8*i]);

		LSC_RChannel[i]  = (int)( (float)(Tmp[0] * R_max) / 1023.0 );
		LSC_GrChannel[i] = (int)( (float)(Tmp[1] * Gr_max) / 1023.0 );
		LSC_GbChannel[i] = (int)( (float)(Tmp[2] * Gb_max) / 1023.0 );
		LSC_BChannel[i]  = (int)( (float)(Tmp[3] * B_max) / 1023.0 );
#endif
		j = j + 5;
	}

	if(!LSCDataCheck_Project2(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel,ErrorMsg))
	{
		Catstring(ErrorMsg,_T("LSC Data Check FAIL!"));
		return FALSE;
	}

	if (m_GetOTPCheckSpec1.m_LSCCurveCheck == 1)
	{
		if (!SaveLSCData(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel))
		{
			Catstring(ErrorMsg,_T("Save LSC fail"));
			return FALSE;
		}
		BOOL m_judgement = CheckLSCData_LG(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel,1023, ErrorMsg,1);
		if (!m_judgement)
		{
			return FALSE;
		}  
	}
	else if (m_GetOTPCheckSpec1.m_LSCCurveCheck == 2)
	{
		if (!SaveLSCData(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel))
		{
			Catstring(ErrorMsg,_T("Save LSC Data fail"));
			return FALSE;
		}
		if (!LensVerificationLib(_T("R"), LSC_RChannel, ErrorMsg,FALSE,FALSE))   return FALSE;
		if (!LensVerificationLib(_T("Gr"),LSC_GrChannel,ErrorMsg,FALSE,FALSE))   return FALSE;
		if (!LensVerificationLib(_T("Gb"),LSC_GbChannel,ErrorMsg,FALSE,FALSE))   return FALSE;
		if (!LensVerificationLib(_T("B"),LSC_BChannel, ErrorMsg,FALSE,FALSE))   return FALSE;
	}

	if (OTPReadData[0x006C] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of LSC Calibration is 0x%02X != 0x01"),OTPReadData[0x006C]);
		return FALSE;
	}

	if (OTPReadData[0x006D] != 0xFF)
	{
		Catstring(ErrorMsg,_T("LSC Equip number is 0x%02X != 0xFF"),OTPReadData[0x006D]);
		return FALSE;
	}

	// check LSC checksum
	int m_LSCSum = 0;
	for (i = 0x006C; i < 0x04CB; i++)
	{
		m_LSCSum += OTPReadData[i];
	}

	if ((OTPReadData[0x04CB] != ((m_LSCSum >> 8) & 0xff)) || (OTPReadData[0x04CC] != (m_LSCSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n LSC Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x04CB],OTPReadData[0x04CC],m_LSCSum);
		return FALSE;
	}

	return TRUE;
}
BOOL OtpCheckAPI::CheckQualcommLSC_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	USHORT LSC_RChannel[221]  = {0};
	USHORT LSC_GrChannel[221] = {0};
	USHORT LSC_GbChannel[221] = {0};
	USHORT LSC_BChannel[221]  = {0};

#if LSC_MODE == LSC_MODE_2
	int R_max, Gr_max, Gb_max, B_max;

	R_max = (OTPReadData[0x0ABF] << 8) | OTPReadData[0xAC0];
	Gr_max = (OTPReadData[0x0AC1] << 8) | OTPReadData[0xAC2];
	Gb_max = (OTPReadData[0x0AC3] << 8) | OTPReadData[0xAC4];
	B_max = (OTPReadData[0x0AC5] << 8) | OTPReadData[0xAC6];
#endif

	int i = 0;
	int j = 0;
	for (i = 0; i < 221; i++)
	{
	#if LSC_MODE == LSC_MODE_1
		LSC_RChannel[i]  = OTPReadData[0x066E + j] + ((OTPReadData[(0x066E + j) + 4] & 0x03) << 8);//((OTPReadData[0x001D+8*i]<<8)&0xFF00)+(OTPReadData[0x001E+8*i]);
		LSC_GrChannel[i] = OTPReadData[(0x066E + j)+1] + ((OTPReadData[(0x066E + j) + 4] & 0x0c) << 6);//((OTPReadData[0x001F+8*i]<<8)&0xFF00)+(OTPReadData[0x0020+8*i]);
		LSC_GbChannel[i] = OTPReadData[(0x066E + j)+2] + ((OTPReadData[(0x066E + j) + 4] & 0x30) << 4);//((OTPReadData[0x0021+8*i]<<8)&0xFF00)+(OTPReadData[0x0022+8*i]);
		LSC_BChannel[i]  = OTPReadData[(0x066E + j)+3] + ((OTPReadData[(0x066E + j) + 4] & 0xc0) << 2);//((OTPReadData[0x0023+8*i]<<8)&0xFF00)+(OTPReadData[0x0024+8*i]);
	#else
		int Tmp[4] = {0};

		Tmp[0]  = OTPReadData[0x066E + j] + ((OTPReadData[(0x066E + j) + 4] & 0x03) << 8);//((OTPReadData[0x001D+8*i]<<8)&0xFF00)+(OTPReadData[0x001E+8*i]);
		Tmp[1]  = OTPReadData[(0x066E + j)+1] + ((OTPReadData[(0x066E + j) + 4] & 0x0c) << 6);//((OTPReadData[0x001F+8*i]<<8)&0xFF00)+(OTPReadData[0x0020+8*i]);
		Tmp[2] = OTPReadData[(0x066E + j)+2] + ((OTPReadData[(0x066E + j) + 4] & 0x30) << 4);//((OTPReadData[0x0021+8*i]<<8)&0xFF00)+(OTPReadData[0x0022+8*i]);
		Tmp[3]  = OTPReadData[(0x066E + j)+3] + ((OTPReadData[(0x066E + j) + 4] & 0xc0) << 2);//((OTPReadData[0x0023+8*i]<<8)&0xFF00)+(OTPReadData[0x0024+8*i]);

		LSC_RChannel[i]  = (int)( (float)(Tmp[0] * R_max) / 1023.0 );
		LSC_GrChannel[i] = (int)( (float)(Tmp[1] * Gr_max) / 1023.0 );
		LSC_GbChannel[i] = (int)( (float)(Tmp[2] * Gb_max) / 1023.0 );
		LSC_BChannel[i]  = (int)( (float)(Tmp[3] * B_max) / 1023.0 );
	#endif
		j = j + 5;
	}

	if(!LSCDataCheck_Project2(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel,ErrorMsg))
	{
		Catstring(ErrorMsg,_T("LSC Data Check FAIL!"));
		return FALSE;
	}

	if (m_GetOTPCheckSpec1.m_LSCCurveCheck == 1)
	{
		if (!SaveLSCData(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel))
		{
			Catstring(ErrorMsg,_T("Save LSC fail"));
			return FALSE;
		}
		BOOL m_judgement = CheckLSCData_LG(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel,1023, ErrorMsg,1);
		if (!m_judgement)
		{
			return FALSE;
		}  
	}
	else if (m_GetOTPCheckSpec1.m_LSCCurveCheck == 2)
	{
		if (!SaveLSCData(LSC_RChannel,LSC_GrChannel,LSC_GbChannel,LSC_BChannel))
		{
			Catstring(ErrorMsg,_T("Save LSC Data fail"));
			return FALSE;
		}
		if (!LensVerificationLib(_T("R"), LSC_RChannel, ErrorMsg,FALSE,FALSE))   return FALSE;
		if (!LensVerificationLib(_T("Gr"),LSC_GrChannel,ErrorMsg,FALSE,FALSE))   return FALSE;
		if (!LensVerificationLib(_T("Gb"),LSC_GbChannel,ErrorMsg,FALSE,FALSE))   return FALSE;
		if (!LensVerificationLib(_T("B"),LSC_BChannel, ErrorMsg,FALSE,FALSE))   return FALSE;
	}

	if (OTPReadData[0x066C] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of LSC Calibration is 0x%02X != 0x01"),OTPReadData[0x066C]);
		return FALSE;
	}

	if (OTPReadData[0x066D] != 0xFF)
	{
		Catstring(ErrorMsg,_T("LSC Equip number is 0x%02X != 0xFF"),OTPReadData[0x066D]);
		return FALSE;
	}

	// check LSC checksum
	int m_LSCSum = 0;
	for (i = 0x066C; i < 0x0ACB; i++)
	{
		m_LSCSum += OTPReadData[i];
	}

	if ((OTPReadData[0x0ACB] != ((m_LSCSum >> 8) & 0xff)) || (OTPReadData[0x0ACC] != (m_LSCSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n LSC Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x0ACB],OTPReadData[0x0ACC],m_LSCSum);
		return FALSE;
	}

	return TRUE;
}
BOOL OtpCheckAPI::CheckMTKLSC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	if (OTPReadData[0x3F] != m_GetOTPCheckSpec.m_LSCFlag)
	{
		Catstring(ErrorMsg, _T("m_LSClag: %d != %d"),OTPReadData[0x003F],m_GetOTPCheckSpec.m_LSCFlag);
		return FALSE;
	}

	//卡控MTK LSC 数据是否存在连续相等值			
	int m_LSCDataEqualRef = OTPReadData[0x0040];
	int m_LSCDataEqualCount = 0;
	int LSCChecksum = 0;
	for (int i = 0x0040;i < 0x078C;i++)
	{
		LSCChecksum += OTPReadData[i];
		if (OTPReadData[i] == m_LSCDataEqualRef)
		{
			m_LSCDataEqualCount++;
			if (m_LSCDataEqualCount > 10)
			{
				Catstring(ErrorMsg,_T("MTK LSC数据存在%d个连续相等的值，规格为10个!"),m_LSCDataEqualCount);
				m_LSCDataEqualCount = 0;
				return FALSE;
			}
		}
		else
		{
			m_LSCDataEqualCount = 0;
			m_LSCDataEqualRef = OTPReadData[i];
		}
	}

	//check LSC checksum
	LSCChecksum = (LSCChecksum%255);
	if (OTPReadData[0x078C] != LSCChecksum)
	{
		Catstring(ErrorMsg,_T("LSC Checksum 0x078C is 0x%02X != 0x%02X"),OTPReadData[0x078C],LSCChecksum);
		return FALSE;
	}	

	return TRUE;
}

BOOL OtpCheckAPI::CheckMTKLSC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	if (OTPReadData[0x3F] != m_GetOTPCheckSpec1.m_LSCFlag)
	{
		Catstring(ErrorMsg, _T("m_LSClag: %d != %d"),OTPReadData[0x003F],m_GetOTPCheckSpec1.m_LSCFlag);
		return FALSE;
	}

	//卡控MTK LSC 数据是否存在连续相等值			
	int m_LSCDataEqualRef = OTPReadData[0x0040];
	int m_LSCDataEqualCount = 0;
	int LSCChecksum = 0;
	for (int i = 0x0040;i < 0x078C;i++)
	{
		LSCChecksum += OTPReadData[i];
		if (OTPReadData[i] == m_LSCDataEqualRef)
		{
			m_LSCDataEqualCount++;
			if (m_LSCDataEqualCount > 10)
			{
				Catstring(ErrorMsg,_T("MTK LSC数据存在%d个连续相等的值，规格为10个!"),m_LSCDataEqualCount);
				m_LSCDataEqualCount = 0;
				return FALSE;
			}
		}
		else
		{
			m_LSCDataEqualCount = 0;
			m_LSCDataEqualRef = OTPReadData[i];
		}
	}

	//check LSC checksum
	LSCChecksum = (LSCChecksum%255);
	if (OTPReadData[0x078C] != LSCChecksum)
	{
		Catstring(ErrorMsg,_T("LSC Checksum 0x078C is 0x%02X != 0x%02X"),OTPReadData[0x078C],LSCChecksum);
		return FALSE;
	}	

	return TRUE;
}


BOOL OtpCheckAPI::CheckAFCode_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check AF
	int i = 0;
	//if (OTPReadData[0x0036] != 0x01)
	//{
	//	Catstring(ErrorMsg,_T("Flag of AF Calibration错误!"));
	//	return FALSE;
	//}

	//if (OTPReadData[0x0037] != 0xff)
	//{
	//	Catstring(ErrorMsg,_T("AF Equip number错误!"));
	//	return FALSE;
	//}

	//if ((OTPReadData[0x0038] != 0x00) || (OTPReadData[0x0039] != 0x00))
	//{
	//	Catstring(ErrorMsg,_T("AF Start current错误!"));
	//	return FALSE;
	//}
	//for (i = 0x003E; i < 0x0042; i++)
	//{
	//	if (OTPReadData[i] != 0xff)
	//	{
	//		Catstring(ErrorMsg,_T("AF Reserve)错误!"));
	//		return FALSE;
	//	}
	//}

	int Infinity  = (((OTPReadData[AF_StartAdd]<<8)&0xFF00) + (OTPReadData[AF_StartAdd+1]&0xFF)) / 64;// OTPReadData[0x003B] + (OTPReadData[0x003A]*256);
	int MacroCode = (((OTPReadData[AF_StartAdd+2]<<8)&0xFF00) + (OTPReadData[AF_StartAdd+3]&0xFF)) / 64;//OTPReadData[0x003D] + (OTPReadData[0x003C]*256);
	

	int m_AFCodeDiff		= MacroCode - Infinity;

	wchar_t InfinitySection[64] ={0};
	_itow(Infinity,InfinitySection,10);
	RegSetKey(_T("F_Auto"),_T("Three_in_one_InfCode"),InfinitySection);

	wchar_t MacroSection[64] ={0};
	_itow(MacroCode,MacroSection,10);
	RegSetKey(_T("F_Auto"),_T("Three_in_one_MacroCode"),MacroSection);

	if ((MacroCode > m_GetOTPCheckSpec.m_MaxMacroCode) || (MacroCode < m_GetOTPCheckSpec.m_MinMacroCode))
	{
		Catstring(ErrorMsg, _T("m_MacroCode %d not in<%d,%d>"),MacroCode,m_GetOTPCheckSpec.m_MinMacroCode,m_GetOTPCheckSpec.m_MaxMacroCode);
		return FALSE;
	}
	if ((Infinity > m_GetOTPCheckSpec.m_MaxInfinityCode) ||( Infinity < m_GetOTPCheckSpec.m_MinInfinityCode))
	{
		Catstring(ErrorMsg, _T("m_InfinityCode %d not in<%d,%d>"),Infinity,m_GetOTPCheckSpec.m_MinInfinityCode,m_GetOTPCheckSpec.m_MaxInfinityCode);
		return FALSE;
	}
	if ((m_AFCodeDiff > m_GetOTPCheckSpec.m_MaxCodeDiff) || (m_AFCodeDiff < m_GetOTPCheckSpec.m_MinCodeDiff))
	{
		Catstring(ErrorMsg, _T("AFCodeDiff %d not in<%d,%d>"),m_AFCodeDiff,m_GetOTPCheckSpec.m_MinCodeDiff,m_GetOTPCheckSpec.m_MaxCodeDiff);
		return FALSE;
	}

	int AFInfinityPeak		= (((OTPReadData[AF_StartAdd+4]<<8)&0xFF00) + (OTPReadData[AF_StartAdd+5]&0xFF)) / 64;
	int AFMacroPeak			= (((OTPReadData[AF_StartAdd+6]<<8)&0xFF00) + (OTPReadData[AF_StartAdd+7]&0xFF)) / 64;
	if (AFInfinityPeak != m_GetOTPCheckSpec.m_AFInfinityPeak)
	{
		Catstring(ErrorMsg,_T("AFInfinityPeak %d != %d"),AFInfinityPeak,m_GetOTPCheckSpec.m_AFInfinityPeak);
		return FALSE;
	}
	if (AFMacroPeak != m_GetOTPCheckSpec.m_AFMacroPeak)
	{
		Catstring(ErrorMsg,_T("AFMacroPeak %d != %d"),AFMacroPeak,m_GetOTPCheckSpec.m_AFMacroPeak);
		return FALSE;
	}

/*	long _CHECKSUM = 0;
	for (int mm = 0x0036; mm < 0x0042; mm++)
	{
		_CHECKSUM += OTPReadData[mm];
	}

	if ((OTPReadData[0x0042] != ((_CHECKSUM >> 8) & 0xff)) || (OTPReadData[0x0043] != (_CHECKSUM & 0xff)))
	{
		Catstring(ErrorMsg,_T("Checksum OF AF Error!"));
		return FALSE;
	} */ 
	int AFCRC = GetCRC16(&OTPReadData[AF_StartAdd],AF_EndAdd - AF_StartAdd - 1); 
	if (((OTPReadData[AF_EndAdd - 1]<<8) + (OTPReadData[AF_EndAdd])) != AFCRC)
	{
		Catstring(ErrorMsg,_T("AF Data CheckSum is 0x%04X != 0x%04X"),(OTPReadData[AF_EndAdd - 1]<<8) + (OTPReadData[AF_EndAdd]),AFCRC);
		return FALSE;
	}
	//m_pInterface->Ctrl_Addlog(CamID,_T("AF Check Pass!"),COLOR_BLUE,200);

	return TRUE;
}
BOOL OtpCheckAPI::CheckSFRMacro_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
// 	m_GetOTPCheckSpec.m_MinCenter_7m			=	ReadConfigInt(_T("MinCenter_7m"),		0);
// 	m_GetOTPCheckSpec.m_MinField4_7m			=	ReadConfigInt(_T("MinField4_7m"),		0);		//7M 0.4视场
// 	m_GetOTPCheckSpec.m_MinField6_7m			=	ReadConfigInt(_T("MinField6_7m"),		0);		//7M 0.6视场
// 	m_GetOTPCheckSpec.m_MinField8_7m			=	ReadConfigInt(_T("MinField8_7m"),		0);		//7M 0.8视场
// 
// 	m_GetOTPCheckSpec.m_MinCenter_8cm			=	ReadConfigInt(_T("MinCenter_8cm"),		0);		//8cM 中心视场
// 	m_GetOTPCheckSpec.m_MinField4_8cm			=	ReadConfigInt(_T("MinField4_8cm"),		0);		//8cM 0.4视场
// 	m_GetOTPCheckSpec.m_MinField6_8cm			=	ReadConfigInt(_T("MinField6_8cm"),		0);		//8cM 0.6视场
// 	m_GetOTPCheckSpec.m_MinField8_8cm			=	ReadConfigInt(_T("MinField8_8cm"),		0);		//8cM 0.8视场
	int i = 0;

	int b_CTPosition_Macro = (((OTPReadData[SFR_Macro_StartAdd+1]<<8)&0xff00) + (OTPReadData[SFR_Macro_StartAdd+2]&0xff)) / 64;

	if (OTPReadData[SFR_Macro_StartAdd] != m_GetOTPCheckSpec.m_Posture_Macro)
	{
		Catstring(ErrorMsg,_T("Distance1 Measurement Posture 0x%02X != 0x%02X"),OTPReadData[SFR_Macro_StartAdd],m_GetOTPCheckSpec.m_Posture_Macro);
		return FALSE;
	}
	if ((b_CTPosition_Macro < m_GetOTPCheckSpec.m_MinCTPosition_Macro) || (b_CTPosition_Macro > m_GetOTPCheckSpec.m_MaxCTPosition_Macro))
	{
		Catstring(ErrorMsg,_T("Distance1 Center SFR Peak Position %d !<%d,%d>"),b_CTPosition_Macro,m_GetOTPCheckSpec.m_MinCTPosition_Macro,m_GetOTPCheckSpec.m_MaxCTPosition_Macro);
		return FALSE;
	}

	if (OTPReadData[SFR_Macro_StartAdd+3] < m_GetOTPCheckSpec.m_MinCenter_Macro)
	{
		Catstring(ErrorMsg,_T("SFR Center_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+3],m_GetOTPCheckSpec.m_MinCenter_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+4] < m_GetOTPCheckSpec.m_MinCenter_Macro)
	{
		Catstring(ErrorMsg,_T("SFR Center_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+4],m_GetOTPCheckSpec.m_MinCenter_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+5] < m_GetOTPCheckSpec.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UL_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+5],m_GetOTPCheckSpec.m_MinField4_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+6] < m_GetOTPCheckSpec.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UL_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+6],m_GetOTPCheckSpec.m_MinField4_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+7] < m_GetOTPCheckSpec.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UR_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+7],m_GetOTPCheckSpec.m_MinField4_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+8] < m_GetOTPCheckSpec.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UR_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+8],m_GetOTPCheckSpec.m_MinField4_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+9] < m_GetOTPCheckSpec.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LR_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+9],m_GetOTPCheckSpec.m_MinField4_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+10] < m_GetOTPCheckSpec.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LR_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+10],m_GetOTPCheckSpec.m_MinField4_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+11] < m_GetOTPCheckSpec.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LL_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+11],m_GetOTPCheckSpec.m_MinField4_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+12] < m_GetOTPCheckSpec.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LL_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+12],m_GetOTPCheckSpec.m_MinField4_Macro);
		return FALSE;
	}

	if (OTPReadData[SFR_Macro_StartAdd+13] < m_GetOTPCheckSpec.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UL_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+13],m_GetOTPCheckSpec.m_MinField6_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+14] < m_GetOTPCheckSpec.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UL_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+14],m_GetOTPCheckSpec.m_MinField6_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+15] < m_GetOTPCheckSpec.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UR_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+15],m_GetOTPCheckSpec.m_MinField6_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+16] < m_GetOTPCheckSpec.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UR_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+16],m_GetOTPCheckSpec.m_MinField6_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+17] < m_GetOTPCheckSpec.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LR_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+17],m_GetOTPCheckSpec.m_MinField6_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+18] < m_GetOTPCheckSpec.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LR_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+18],m_GetOTPCheckSpec.m_MinField6_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+19] < m_GetOTPCheckSpec.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LL_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+19],m_GetOTPCheckSpec.m_MinField6_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+20] < m_GetOTPCheckSpec.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LL_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+20],m_GetOTPCheckSpec.m_MinField6_Macro);
		return FALSE;
	}

	if (OTPReadData[SFR_Macro_StartAdd+21] < m_GetOTPCheckSpec.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UL_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+21],m_GetOTPCheckSpec.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+22] < m_GetOTPCheckSpec.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UL_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+22],m_GetOTPCheckSpec.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+23] < m_GetOTPCheckSpec.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UR_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+23],m_GetOTPCheckSpec.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+24] < m_GetOTPCheckSpec.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UR_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+24],m_GetOTPCheckSpec.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+25] < m_GetOTPCheckSpec.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LR_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+25],m_GetOTPCheckSpec.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+26] < m_GetOTPCheckSpec.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LR_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+26],m_GetOTPCheckSpec.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+27] < m_GetOTPCheckSpec.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LL_Macro_H is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+27],m_GetOTPCheckSpec.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+28] < m_GetOTPCheckSpec.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LL_Macro_V is %d !> %d"),OTPReadData[SFR_Macro_StartAdd+28],m_GetOTPCheckSpec.m_MinField8_Macro);
		return FALSE;
	}

	if (OTPReadData[SFR_Macro_StartAdd+29] > m_GetOTPCheckSpec.m_MaxField6CTC_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_CTC_Macro_H is %d !< %d"),OTPReadData[SFR_Macro_StartAdd+29],m_GetOTPCheckSpec.m_MaxField6CTC_Macro);
		return FALSE;
	}
	if (OTPReadData[SFR_Macro_StartAdd+30] > m_GetOTPCheckSpec.m_MaxField6CTC_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_CTC_Macro_V is %d !< %d"),OTPReadData[SFR_Macro_StartAdd+30],m_GetOTPCheckSpec.m_MaxField6CTC_Macro);
		return FALSE;
	}

	return TRUE;
}
BOOL OtpCheckAPI::CheckSFR7M_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (((OTPReadData[0x04CF] << 8) + OTPReadData[0x04D0]) < m_GetOTPCheckSpec1.m_MinCenter_Macro)
	{
		Catstring(ErrorMsg,_T("SFR Center_7m_H is %d !> %d"),((OTPReadData[0x04CF] << 8) + OTPReadData[0x04D0]),m_GetOTPCheckSpec1.m_MinCenter_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04D1] << 8) + OTPReadData[0x04D2]) < m_GetOTPCheckSpec1.m_MinCenter_Macro)
	{
		Catstring(ErrorMsg,_T("SFR Center_7m_V is %d !> %d"),((OTPReadData[0x04D1] << 8) + OTPReadData[0x04D2]),m_GetOTPCheckSpec1.m_MinCenter_Macro);
		return FALSE;
	}

	if (((OTPReadData[0x04D3] << 8) + OTPReadData[0x04D4]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TL_7m_H is %d !> %d"),((OTPReadData[0x04D3] << 8) + OTPReadData[0x04D4]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04D5] << 8) + OTPReadData[0x04D6]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TL_7m_V is %d !> %d"),((OTPReadData[0x04D5] << 8) + OTPReadData[0x04D6]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04D7] << 8) + OTPReadData[0x04D8]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TR_7m_H is %d !> %d"),((OTPReadData[0x04D7] << 8) + OTPReadData[0x04D8]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04D9] << 8) + OTPReadData[0x04DA]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TR_7m_V is %d !> %d"),((OTPReadData[0x04D9] << 8) + OTPReadData[0x04DA]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04DB] << 8) + OTPReadData[0x04DC]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BL_7m_H is %d !> %d"),((OTPReadData[0x04DB] << 8) + OTPReadData[0x04DC]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04DD] << 8) + OTPReadData[0x04DE]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BL_7m_V is %d !> %d"),((OTPReadData[0x04DD] << 8) + OTPReadData[0x04DE]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04DF] << 8) + OTPReadData[0x04E0]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BR_7m_H is %d !> %d"),((OTPReadData[0x04DF] << 8) + OTPReadData[0x04E0]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04E1] << 8) + OTPReadData[0x04E2]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BR_7m_V is %d !> %d"),((OTPReadData[0x04E1] << 8) + OTPReadData[0x04E2]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}

	if (((OTPReadData[0x04E3] << 8) + OTPReadData[0x04E4]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TL_7m_H is %d !> %d"),((OTPReadData[0x04E3] << 8) + OTPReadData[0x04E4]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04E5] << 8) + OTPReadData[0x04E6]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TL_7m_V is %d !> %d"),((OTPReadData[0x04E5] << 8) + OTPReadData[0x04E6]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04E7] << 8) + OTPReadData[0x04E8]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TR_7m_H is %d !> %d"),((OTPReadData[0x04E7] << 8) + OTPReadData[0x04E8]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04E9] << 8) + OTPReadData[0x04EA]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TR_7m_V is %d !> %d"),((OTPReadData[0x04E9] << 8) + OTPReadData[0x04EA]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04EB] << 8) + OTPReadData[0x04EC]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BL_7m_H is %d !> %d"),((OTPReadData[0x04EB] << 8) + OTPReadData[0x04EC]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04ED] << 8) + OTPReadData[0x04EE]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BL_10cm_V is %d !> %d"),((OTPReadData[0x04ED] << 8) + OTPReadData[0x04EE]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04EF] << 8) + OTPReadData[0x04F0]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BR_7m_H is %d !> %d"),((OTPReadData[0x04EF] << 8) + OTPReadData[0x04F0]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04F1] << 8) + OTPReadData[0x04F2]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BR_7m_V is %d !> %d"),((OTPReadData[0x04F1] << 8) + OTPReadData[0x04F2]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}

	if (((OTPReadData[0x04F3] << 8) + OTPReadData[0x04F4]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TL_7m_H is %d !> %d"),((OTPReadData[0x04F3] << 8) + OTPReadData[0x04F4]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04F5] << 8) + OTPReadData[0x04F6]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TL_7m_V is %d !> %d"),((OTPReadData[0x04F5] << 8) + OTPReadData[0x04F6]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04F7] << 8) + OTPReadData[0x04F8]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TR_7m_H is %d !> %d"),((OTPReadData[0x04F7] << 8) + OTPReadData[0x04F8]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04F9] << 8) + OTPReadData[0x04FA]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TR_7m_V is %d !> %d"),((OTPReadData[0x04F9] << 8) + OTPReadData[0x04FA]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04FB] << 8) + OTPReadData[0x04FC]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_7m_H is %d !> %d"),((OTPReadData[0x04FB] << 8) + OTPReadData[0x04FC]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04FD] << 8) + OTPReadData[0x04FE]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_7m_V is %d !> %d"),((OTPReadData[0x04FD] << 8) + OTPReadData[0x04FE]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x04FF] << 8) + OTPReadData[0x0500]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BR_7m_H is %d !> %d"),((OTPReadData[0x04FF] << 8) + OTPReadData[0x0500]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0501] << 8) + OTPReadData[0x0502]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BR_7m_V is %d !> %d"),((OTPReadData[0x0501] << 8) + OTPReadData[0x0502]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[0x04CD] != 0x01)
	{
		Catstring(ErrorMsg,_T("7M Flag of SFR is 0x%02X != 0x01"),OTPReadData[0x04CD]);
		return FALSE;
	}

	if (OTPReadData[0x04CE] != 0xFF)
	{
		Catstring(ErrorMsg,_T("7m sfr Equip number is 0x%02X != 0xFF"),OTPReadData[0x04CE]);
		return FALSE;
	}
	int SfrCheckSum = 0;
	for (i = 0x04CD; i < 0x0503; i++)
	{
		SfrCheckSum = SfrCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x0503] != ((SfrCheckSum >> 8) & 0xff)) || (OTPReadData[0x0504] != (SfrCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n 7M AF Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x0503],OTPReadData[0x0504],SfrCheckSum);
		return FALSE;
	}
	//m_pInterface->Ctrl_Addlog(CamID,_T("MONO 7CM Check Pass!"),COLOR_BLUE,200);
	return TRUE;
}
BOOL OtpCheckAPI::CheckSFR7M_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (((OTPReadData[0x0ACF] << 8) + OTPReadData[0x0AD0]) < m_GetOTPCheckSpec1.m_MinCenter_Macro)
	{
		Catstring(ErrorMsg,_T("SFR Center_7m_H is %d !> %d"),((OTPReadData[0x0ACF] << 8) + OTPReadData[0x0AD0]),m_GetOTPCheckSpec1.m_MinCenter_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AD1] << 8) + OTPReadData[0x0AD2]) < m_GetOTPCheckSpec1.m_MinCenter_Macro)
	{
		Catstring(ErrorMsg,_T("SFR Center_7m_V is %d !> %d"),((OTPReadData[0x0AD1] << 8) + OTPReadData[0x0AD2]),m_GetOTPCheckSpec1.m_MinCenter_Macro);
		return FALSE;
	}

	if (((OTPReadData[0x0AD3] << 8) + OTPReadData[0x0AD4]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TL_7m_H is %d !> %d"),((OTPReadData[0x0AD3] << 8) + OTPReadData[0x0AD4]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AD5] << 8) + OTPReadData[0x0AD6]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TL_7m_V is %d !> %d"),((OTPReadData[0x0AD5] << 8) + OTPReadData[0x0AD6]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AD7] << 8) + OTPReadData[0x0AD8]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TR_7m_H is %d !> %d"),((OTPReadData[0x0AD7] << 8) + OTPReadData[0x0AD8]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AD9] << 8) + OTPReadData[0x0ADA]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TR_7m_V is %d !> %d"),((OTPReadData[0x0AD9] << 8) + OTPReadData[0x0ADA]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0ADB] << 8) + OTPReadData[0x0ADC]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BL_7m_H is %d !> %d"),((OTPReadData[0x0ADB] << 8) + OTPReadData[0x0ADC]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0ADD] << 8) + OTPReadData[0x0ADE]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BL_7m_V is %d !> %d"),((OTPReadData[0x0ADD] << 8) + OTPReadData[0x0ADE]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0ADF] << 8) + OTPReadData[0x0AE0]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BR_7m_H is %d !> %d"),((OTPReadData[0x0ADF] << 8) + OTPReadData[0x0AE0]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AE1] << 8) + OTPReadData[0x0AE2]) < m_GetOTPCheckSpec1.m_MinField4_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BR_7m_V is %d !> %d"),((OTPReadData[0x0AE1] << 8) + OTPReadData[0x0AE2]),m_GetOTPCheckSpec1.m_MinField4_Macro);
		return FALSE;
	}

	if (((OTPReadData[0x0AE3] << 8) + OTPReadData[0x0AE4]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TL_7m_H is %d !> %d"),((OTPReadData[0x0AE3] << 8) + OTPReadData[0x0AE4]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AE5] << 8) + OTPReadData[0x0AE6]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TL_7m_V is %d !> %d"),((OTPReadData[0x0AE5] << 8) + OTPReadData[0x0AE6]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AE7] << 8) + OTPReadData[0x0AE8]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TR_7m_H is %d !> %d"),((OTPReadData[0x0AE7] << 8) + OTPReadData[0x0AE8]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AE9] << 8) + OTPReadData[0x0AEA]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TR_7m_V is %d !> %d"),((OTPReadData[0x0AE9] << 8) + OTPReadData[0x0AEA]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AEB] << 8) + OTPReadData[0x0AEC]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BL_7m_H is %d !> %d"),((OTPReadData[0x0AEB] << 8) + OTPReadData[0x0AEC]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AED] << 8) + OTPReadData[0x0AEE]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BL_10cm_V is %d !> %d"),((OTPReadData[0x0AED] << 8) + OTPReadData[0x0AEE]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AEF] << 8) + OTPReadData[0x0AF0]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BR_7m_H is %d !> %d"),((OTPReadData[0x0AEF] << 8) + OTPReadData[0x0AF0]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AF1] << 8) + OTPReadData[0x0AF2]) < m_GetOTPCheckSpec1.m_MinField6_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BR_7m_V is %d !> %d"),((OTPReadData[0x0AF1] << 8) + OTPReadData[0x0AF2]),m_GetOTPCheckSpec1.m_MinField6_Macro);
		return FALSE;
	}

	if (((OTPReadData[0x0AF3] << 8) + OTPReadData[0x0AF4]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TL_7m_H is %d !> %d"),((OTPReadData[0x0AF3] << 8) + OTPReadData[0x0AF4]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AF5] << 8) + OTPReadData[0x0AF6]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TL_7m_V is %d !> %d"),((OTPReadData[0x0AF5] << 8) + OTPReadData[0x0AF6]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AF7] << 8) + OTPReadData[0x0AF8]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TR_7m_H is %d !> %d"),((OTPReadData[0x0AF7] << 8) + OTPReadData[0x0AF8]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AF9] << 8) + OTPReadData[0x0AFA]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TR_7m_V is %d !> %d"),((OTPReadData[0x0AF9] << 8) + OTPReadData[0x0AFA]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AFB] << 8) + OTPReadData[0x0AFC]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_7m_H is %d !> %d"),((OTPReadData[0x0AFB] << 8) + OTPReadData[0x0AFC]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AFD] << 8) + OTPReadData[0x0AFE]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_7m_V is %d !> %d"),((OTPReadData[0x0AFD] << 8) + OTPReadData[0x0AFE]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0AFF] << 8) + OTPReadData[0x0B00]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BR_7m_H is %d !> %d"),((OTPReadData[0x0AFF] << 8) + OTPReadData[0x0B00]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (((OTPReadData[0x0B01] << 8) + OTPReadData[0x0B02]) < m_GetOTPCheckSpec1.m_MinField8_Macro)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BR_7m_V is %d !> %d"),((OTPReadData[0x0B01] << 8) + OTPReadData[0x0B02]),m_GetOTPCheckSpec1.m_MinField8_Macro);
		return FALSE;
	}
	if (OTPReadData[0x0ACD] != 0x01)
	{
		Catstring(ErrorMsg,_T("7M Flag of SFR is 0x%02X != 0x01"),OTPReadData[0x04CD]);
		return FALSE;
	}

	if (OTPReadData[0x0ACE] != 0xFF)
	{
		Catstring(ErrorMsg,_T("7m sfr Equip number is 0x%02X != 0xFF"),OTPReadData[0x04CE]);
		return FALSE;
	}
	int SfrCheckSum = 0;
	for (i = 0x0ACD; i < 0x0B03; i++)
	{
		SfrCheckSum = SfrCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x0B03] != ((SfrCheckSum >> 8) & 0xff)) || (OTPReadData[0x0B04] != (SfrCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n 7M AF Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x0B03],OTPReadData[0x0B04],SfrCheckSum);
		return FALSE;
	}

	return TRUE;
}
BOOL OtpCheckAPI::CheckSFRMid_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int b_CTPosition_Mid = (((OTPReadData[SFR_Mid_StartAdd+1]<<8)&0xff00) + (OTPReadData[SFR_Mid_StartAdd+2]&0xff)) / 64;

	if (OTPReadData[SFR_Mid_StartAdd] != m_GetOTPCheckSpec.m_Posture_Mid)
	{
		Catstring(ErrorMsg,_T("Distance2 Measurement Posture 0x%02X != 0x%02X"),OTPReadData[SFR_Mid_StartAdd],m_GetOTPCheckSpec.m_Posture_Mid);
		return FALSE;
	}
	if ((b_CTPosition_Mid < m_GetOTPCheckSpec.m_MinCTPosition_Mid) || (b_CTPosition_Mid > m_GetOTPCheckSpec.m_MaxCTPosition_Mid))
	{
		Catstring(ErrorMsg,_T("Distance2 Center SFR Peak Position %d !<%d,%d>"),b_CTPosition_Mid,m_GetOTPCheckSpec.m_MinCTPosition_Mid,m_GetOTPCheckSpec.m_MaxCTPosition_Mid);
		return FALSE;
	}

	if (OTPReadData[SFR_Mid_StartAdd+3] < m_GetOTPCheckSpec.m_MinCenter_Mid)
	{
		Catstring(ErrorMsg,_T("SFR Center_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+3],m_GetOTPCheckSpec.m_MinCenter_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+4] < m_GetOTPCheckSpec.m_MinCenter_Mid)
	{
		Catstring(ErrorMsg,_T("SFR Center_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+4],m_GetOTPCheckSpec.m_MinCenter_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+5] < m_GetOTPCheckSpec.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UL_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+5],m_GetOTPCheckSpec.m_MinField4_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+6] < m_GetOTPCheckSpec.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UL_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+6],m_GetOTPCheckSpec.m_MinField4_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+7] < m_GetOTPCheckSpec.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UR_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+7],m_GetOTPCheckSpec.m_MinField4_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+8] < m_GetOTPCheckSpec.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UR_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+8],m_GetOTPCheckSpec.m_MinField4_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+9] < m_GetOTPCheckSpec.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LR_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+9],m_GetOTPCheckSpec.m_MinField4_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+10] < m_GetOTPCheckSpec.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LR_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+10],m_GetOTPCheckSpec.m_MinField4_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+11] < m_GetOTPCheckSpec.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LL_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+11],m_GetOTPCheckSpec.m_MinField4_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+12] < m_GetOTPCheckSpec.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LL_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+12],m_GetOTPCheckSpec.m_MinField4_Mid);
		return FALSE;
	}

	if (OTPReadData[SFR_Mid_StartAdd+13] < m_GetOTPCheckSpec.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UL_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+13],m_GetOTPCheckSpec.m_MinField6_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+14] < m_GetOTPCheckSpec.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UL_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+14],m_GetOTPCheckSpec.m_MinField6_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+15] < m_GetOTPCheckSpec.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UR_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+15],m_GetOTPCheckSpec.m_MinField6_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+16] < m_GetOTPCheckSpec.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UR_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+16],m_GetOTPCheckSpec.m_MinField6_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+17] < m_GetOTPCheckSpec.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LR_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+17],m_GetOTPCheckSpec.m_MinField6_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+18] < m_GetOTPCheckSpec.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LR_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+18],m_GetOTPCheckSpec.m_MinField6_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+19] < m_GetOTPCheckSpec.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LL_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+19],m_GetOTPCheckSpec.m_MinField6_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+20] < m_GetOTPCheckSpec.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LL_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+20],m_GetOTPCheckSpec.m_MinField6_Mid);
		return FALSE;
	}

	if (OTPReadData[SFR_Mid_StartAdd+21] < m_GetOTPCheckSpec.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UL_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+21],m_GetOTPCheckSpec.m_MinField8_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+22] < m_GetOTPCheckSpec.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UL_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+22],m_GetOTPCheckSpec.m_MinField8_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+23] < m_GetOTPCheckSpec.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UR_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+23],m_GetOTPCheckSpec.m_MinField8_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+24] < m_GetOTPCheckSpec.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UR_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+24],m_GetOTPCheckSpec.m_MinField8_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+25] < m_GetOTPCheckSpec.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LR_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+25],m_GetOTPCheckSpec.m_MinField8_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+26] < m_GetOTPCheckSpec.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LR_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+26],m_GetOTPCheckSpec.m_MinField8_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+27] < m_GetOTPCheckSpec.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LL_Mid_H is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+27],m_GetOTPCheckSpec.m_MinField8_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+28] < m_GetOTPCheckSpec.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LL_Mid_V is %d !> %d"),OTPReadData[SFR_Mid_StartAdd+28],m_GetOTPCheckSpec.m_MinField8_Mid);
		return FALSE;
	}

	if (OTPReadData[SFR_Mid_StartAdd+29] > m_GetOTPCheckSpec.m_MaxField6CTC_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_CTC_Mid_H is %d !< %d"),OTPReadData[SFR_Mid_StartAdd+29],m_GetOTPCheckSpec.m_MaxField6CTC_Mid);
		return FALSE;
	}
	if (OTPReadData[SFR_Mid_StartAdd+30] > m_GetOTPCheckSpec.m_MaxField6CTC_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_CTC_Mid_V is %d !< %d"),OTPReadData[SFR_Mid_StartAdd+30],m_GetOTPCheckSpec.m_MaxField6CTC_Mid);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckSFR8CM_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (((OTPReadData[0x0506] << 8) + OTPReadData[0x0507]) < m_GetOTPCheckSpec1.m_MinCenter_Mid)
	{
		Catstring(ErrorMsg,_T("SFR Center_8cm_H is %d !> %d"),((OTPReadData[0x0506] << 8) + OTPReadData[0x0507]),m_GetOTPCheckSpec1.m_MinCenter_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0508] << 8) + OTPReadData[0x0509]) < m_GetOTPCheckSpec1.m_MinCenter_Mid)
	{
		Catstring(ErrorMsg,_T("SFR Center_8cm_V is %d !> %d"),((OTPReadData[0x0508] << 8) + OTPReadData[0x0509]),m_GetOTPCheckSpec1.m_MinCenter_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x050A] << 8) + OTPReadData[0x050B]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TL_8cm_H is %d !> %d"),((OTPReadData[0x050A] << 8) + OTPReadData[0x050B]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x050C] << 8) + OTPReadData[0x050D]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TL_8cm_V is %d !> %d"),((OTPReadData[0x050C] << 8) + OTPReadData[0x050D]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x050E] << 8) + OTPReadData[0x050F]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TR_8cm_H is %d !> %d"),((OTPReadData[0x050E] << 8) + OTPReadData[0x050F]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0510] << 8) + OTPReadData[0x0511]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TR_8cm_V is %d !> %d"),((OTPReadData[0x0510] << 8) + OTPReadData[0x0511]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0512] << 8) + OTPReadData[0x0513]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BL_8cm_H is %d !> %d"),((OTPReadData[0x0512] << 8) + OTPReadData[0x0513]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0514] << 8) + OTPReadData[0x0515]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BL_8cm_V is %d !> %d"),((OTPReadData[0x0514] << 8) + OTPReadData[0x0515]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0516] << 8) + OTPReadData[0x0517]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BR_8cm_H is %d !> %d"),((OTPReadData[0x0516] << 8) + OTPReadData[0x0517]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0518] << 8) + OTPReadData[0x0519]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BR_8cm_V is %d !> %d"),((OTPReadData[0x0518] << 8) + OTPReadData[0x0519]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}

	if (((OTPReadData[0x051A] << 8) + OTPReadData[0x051B]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TL_8cm_H is %d !> %d"),((OTPReadData[0x051A] << 8) + OTPReadData[0x051B]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x051C] << 8) + OTPReadData[0x051D]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TL_8cm_V is %d !> %d"),((OTPReadData[0x051C] << 8) + OTPReadData[0x051D]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x051E] << 8) + OTPReadData[0x051F]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TR_8cm_H is %d !> %d"),((OTPReadData[0x051E] << 8) + OTPReadData[0x051F]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0520] << 8) + OTPReadData[0x0521]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TR_1000cm_V is %d !> %d"),((OTPReadData[0x0520] << 8) + OTPReadData[0x0521]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0522] << 8) + OTPReadData[0x0523]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BL_1000cm_H is %d !> %d"),((OTPReadData[0x0522] << 8) + OTPReadData[0x0523]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0524] << 8) + OTPReadData[0x0525]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BL_8cm_V is %d !> %d"),((OTPReadData[0x0524] << 8) + OTPReadData[0x0525]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0526] << 8) + OTPReadData[0x0527]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BR_8cm_H is %d !> %d"),((OTPReadData[0x0526] << 8) + OTPReadData[0x0527]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0528] << 8) + OTPReadData[0x0529]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BR_8cm_V is %d !> %d"),((OTPReadData[0x0528] << 8) + OTPReadData[0x0529]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}

	if (((OTPReadData[0x052A] << 8) + OTPReadData[0x052B]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TL_8cm_H is %d !> %d"),((OTPReadData[0x052A] << 8) + OTPReadData[0x052B]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x052C] << 8) + OTPReadData[0x052D]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TL_8cm_V is %d !> %d"),((OTPReadData[0x052C] << 8) + OTPReadData[0x052D]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x052E] << 8) + OTPReadData[0x052F]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TR_8cm_H is %d !> %d"),((OTPReadData[0x052E] << 8) + OTPReadData[0x052F]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0530] << 8) + OTPReadData[0x0531]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TR_8cm_V is %d !> %d"),((OTPReadData[0x0530] << 8) + OTPReadData[0x0531]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0532] << 8) + OTPReadData[0x0533]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_8cm_H is %d !> %d"),((OTPReadData[0x0532] << 8) + OTPReadData[0x0533]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0534] << 8) + OTPReadData[0x0535]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_8cm_V is %d !> %d"),((OTPReadData[0x0534] << 8) + OTPReadData[0x0535]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0536] << 8) + OTPReadData[0x0537]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_8cm_H is %d !> %d"),((OTPReadData[0x0536] << 8) + OTPReadData[0x0537]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0538] << 8) + OTPReadData[0x0539]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BR_8cm_V is %d !> %d"),((OTPReadData[0x0538] << 8) + OTPReadData[0x0539]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}

	if (OTPReadData[0x0505] != 0x01)
	{
		Catstring(ErrorMsg,_T("8CM Flag of SFR is 0x%02X != 0x01"),OTPReadData[0x0505]);
		return FALSE;
	}

	int SfrCheckSum = 0;
	for (i = 0x0505; i < 0x053A; i++)
	{
		SfrCheckSum = SfrCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x053A] != ((SfrCheckSum >> 8) & 0xff)) || (OTPReadData[0x053B] != (SfrCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n 8CM AF Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x0503],OTPReadData[0x0504],SfrCheckSum);
		return FALSE;
	}
	//m_pInterface->Ctrl_Addlog(CamID,_T("MONO 8CM Check Pass!"),COLOR_BLUE,200);
	return TRUE;
}

BOOL OtpCheckAPI::CheckSFR8CM_Project3(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (((OTPReadData[0x0B06] << 8) + OTPReadData[0x0B07]) < m_GetOTPCheckSpec1.m_MinCenter_Mid)
	{
		Catstring(ErrorMsg,_T("SFR Center_8cm_H is %d !> %d"),((OTPReadData[0x0B06] << 8) + OTPReadData[0x0B07]),m_GetOTPCheckSpec1.m_MinCenter_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B08] << 8) + OTPReadData[0x0B09]) < m_GetOTPCheckSpec1.m_MinCenter_Mid)
	{
		Catstring(ErrorMsg,_T("SFR Center_8cm_V is %d !> %d"),((OTPReadData[0x0B08] << 8) + OTPReadData[0x0B09]),m_GetOTPCheckSpec1.m_MinCenter_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B0A] << 8) + OTPReadData[0x0B0B]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TL_8cm_H is %d !> %d"),((OTPReadData[0x0B0A] << 8) + OTPReadData[0x0B0B]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B0C] << 8) + OTPReadData[0x0B0D]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TL_8cm_V is %d !> %d"),((OTPReadData[0x0B0C] << 8) + OTPReadData[0x0B0D]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B0E] << 8) + OTPReadData[0x0B0F]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TR_8cm_H is %d !> %d"),((OTPReadData[0x0B0E] << 8) + OTPReadData[0x0B0F]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B10] << 8) + OTPReadData[0x0B11]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_TR_8cm_V is %d !> %d"),((OTPReadData[0x0B10] << 8) + OTPReadData[0x0B11]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B12] << 8) + OTPReadData[0x0B13]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BL_8cm_H is %d !> %d"),((OTPReadData[0x0B12] << 8) + OTPReadData[0x0B13]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B14] << 8) + OTPReadData[0x0B15]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BL_8cm_V is %d !> %d"),((OTPReadData[0x0B14] << 8) + OTPReadData[0x0B15]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B16] << 8) + OTPReadData[0x0B17]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BR_8cm_H is %d !> %d"),((OTPReadData[0x0B16] << 8) + OTPReadData[0x0B17]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B18] << 8) + OTPReadData[0x0B19]) < m_GetOTPCheckSpec1.m_MinField4_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_BR_8cm_V is %d !> %d"),((OTPReadData[0x0B18] << 8) + OTPReadData[0x0B19]),m_GetOTPCheckSpec1.m_MinField4_Mid);
		return FALSE;
	}

	if (((OTPReadData[0x0B1A] << 8) + OTPReadData[0x0B1B]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TL_8cm_H is %d !> %d"),((OTPReadData[0x0B1A] << 8) + OTPReadData[0x0B1B]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B1C] << 8) + OTPReadData[0x0B1D]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TL_8cm_V is %d !> %d"),((OTPReadData[0x0B1C] << 8) + OTPReadData[0x0B1D]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B1E] << 8) + OTPReadData[0x0B1F]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TR_8cm_H is %d !> %d"),((OTPReadData[0x0B1E] << 8) + OTPReadData[0x0B1F]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B20] << 8) + OTPReadData[0x0B21]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_TR_1000cm_V is %d !> %d"),((OTPReadData[0x0B20] << 8) + OTPReadData[0x0B21]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B22] << 8) + OTPReadData[0x0B23]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BL_1000cm_H is %d !> %d"),((OTPReadData[0x0B22] << 8) + OTPReadData[0x0B23]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B24] << 8) + OTPReadData[0x0B25]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BL_8cm_V is %d !> %d"),((OTPReadData[0x0B24] << 8) + OTPReadData[0x0B25]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B26] << 8) + OTPReadData[0x0B27]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BR_8cm_H is %d !> %d"),((OTPReadData[0x0B26] << 8) + OTPReadData[0x0B27]),m_GetOTPCheckSpec1.m_MinField6_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B28] << 8) + OTPReadData[0x0B29]) < m_GetOTPCheckSpec1.m_MinField6_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_BR_8cm_V is %d !> %d",((OTPReadData[0x0B28] << 8) + OTPReadData[0x0B29]),m_GetOTPCheckSpec1.m_MinField6_Mid));
		return FALSE;
	}

	if (((OTPReadData[0x0B2A] << 8) + OTPReadData[0x0B2B]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TL_8cm_H is %d !> %d"),((OTPReadData[0x0B2A] << 8) + OTPReadData[0x0B2B]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B2C] << 8) + OTPReadData[0x0B2D]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TL_8cm_V is %d !> %d"),((OTPReadData[0x0B2C] << 8) + OTPReadData[0x0B2D]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B2E] << 8) + OTPReadData[0x0B2F]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TR_8cm_H is %d !> %d"),((OTPReadData[0x0B2E] << 8) + OTPReadData[0x0B2F]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B30] << 8) + OTPReadData[0x0B31]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_TR_8cm_V is %d !> %d"),((OTPReadData[0x0B30] << 8) + OTPReadData[0x0B31]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B32] << 8) + OTPReadData[0x0B33]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_8cm_H is %d !> %d"),((OTPReadData[0x0B32] << 8) + OTPReadData[0x0B33]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B34] << 8) + OTPReadData[0x0B35]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_8cm_V is %d !> %d"),((OTPReadData[0x0B34] << 8) + OTPReadData[0x0B35]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B36] << 8) + OTPReadData[0x0B37]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BL_8cm_H is %d !> %d"),((OTPReadData[0x0B36] << 8) + OTPReadData[0x0B37]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}
	if (((OTPReadData[0x0B38] << 8) + OTPReadData[0x0B39]) < m_GetOTPCheckSpec1.m_MinField8_Mid)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_BR_8cm_V is %d !> %d"),((OTPReadData[0x0B38] << 8) + OTPReadData[0x0B39]),m_GetOTPCheckSpec1.m_MinField8_Mid);
		return FALSE;
	}

	if (OTPReadData[0x0B05] != 0x01)
	{
		Catstring(ErrorMsg,_T("8CM Flag of SFR is 0x%02X != 0x01"),OTPReadData[0x0B05]);
		return FALSE;
	}


	int SfrCheckSum = 0;
	for (i = 0x0B05; i < 0x0B3A; i++)
	{
		SfrCheckSum = SfrCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x0B3A] != ((SfrCheckSum >> 8) & 0xff)) || (OTPReadData[0x0B3B] != (SfrCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n 8CM AF Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x0B3A],OTPReadData[0x0B3B],SfrCheckSum);
		return FALSE;
	}
	for (i = 0x0B3C; i < 0x0D00; i++)
	{
		if (OTPReadData[i] != 0xff)
		{
			Catstring(ErrorMsg,_T("OTPReadData 0x%02X != 0xff"),OTPReadData[i]);
			return FALSE;
		}
	}
	return TRUE;
}
BOOL OtpCheckAPI::CheckSFRInfinity_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int b_CTPosition_Infinity = (((OTPReadData[SFR_Inf_StartAdd+1]<<8)&0xff00) + (OTPReadData[SFR_Inf_StartAdd+2]&0xff)) / 64;

	if (OTPReadData[SFR_Inf_StartAdd] != m_GetOTPCheckSpec.m_Posture_Infinity)
	{
		Catstring(ErrorMsg,_T("Distance3 Measurement Posture 0x%02X != 0x%02X"),OTPReadData[SFR_Inf_StartAdd],m_GetOTPCheckSpec.m_Posture_Infinity);
		return FALSE;
	}
	if ((b_CTPosition_Infinity < m_GetOTPCheckSpec.m_MinCTPosition_Infinity) || (b_CTPosition_Infinity > m_GetOTPCheckSpec.m_MaxCTPosition_Infinity))
	{
		Catstring(ErrorMsg,_T("Distance3 Center SFR Peak Position %d !<%d,%d>"),b_CTPosition_Infinity,m_GetOTPCheckSpec.m_MinCTPosition_Infinity,m_GetOTPCheckSpec.m_MaxCTPosition_Infinity);
		return FALSE;
	}

	if (OTPReadData[SFR_Inf_StartAdd+3] < m_GetOTPCheckSpec.m_MinCenter_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR Center_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+3],m_GetOTPCheckSpec.m_MinCenter_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+4] < m_GetOTPCheckSpec.m_MinCenter_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR Center_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+4],m_GetOTPCheckSpec.m_MinCenter_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+5] < m_GetOTPCheckSpec.m_MinField4_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UL_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+5],m_GetOTPCheckSpec.m_MinField4_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+6] < m_GetOTPCheckSpec.m_MinField4_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UL_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+6],m_GetOTPCheckSpec.m_MinField4_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+7] < m_GetOTPCheckSpec.m_MinField4_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UR_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+7],m_GetOTPCheckSpec.m_MinField4_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+8] < m_GetOTPCheckSpec.m_MinField4_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_UR_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+8],m_GetOTPCheckSpec.m_MinField4_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+9] < m_GetOTPCheckSpec.m_MinField4_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LR_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+9],m_GetOTPCheckSpec.m_MinField4_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+10] < m_GetOTPCheckSpec.m_MinField4_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LR_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+10],m_GetOTPCheckSpec.m_MinField4_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+11] < m_GetOTPCheckSpec.m_MinField4_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LL_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+11],m_GetOTPCheckSpec.m_MinField4_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+12] < m_GetOTPCheckSpec.m_MinField4_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.4_Field_LL_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+12],m_GetOTPCheckSpec.m_MinField4_Infinity);
		return FALSE;
	}

	if (OTPReadData[SFR_Inf_StartAdd+13] < m_GetOTPCheckSpec.m_MinField6_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UL_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+13],m_GetOTPCheckSpec.m_MinField6_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+14] < m_GetOTPCheckSpec.m_MinField6_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UL_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+14],m_GetOTPCheckSpec.m_MinField6_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+15] < m_GetOTPCheckSpec.m_MinField6_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UR_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+15],m_GetOTPCheckSpec.m_MinField6_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+16] < m_GetOTPCheckSpec.m_MinField6_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_UR_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+16],m_GetOTPCheckSpec.m_MinField6_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+17] < m_GetOTPCheckSpec.m_MinField6_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LR_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+17],m_GetOTPCheckSpec.m_MinField6_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+18] < m_GetOTPCheckSpec.m_MinField6_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LR_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+18],m_GetOTPCheckSpec.m_MinField6_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+19] < m_GetOTPCheckSpec.m_MinField6_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LL_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+19],m_GetOTPCheckSpec.m_MinField6_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+20] < m_GetOTPCheckSpec.m_MinField6_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_LL_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+20],m_GetOTPCheckSpec.m_MinField6_Infinity);
		return FALSE;
	}

	if (OTPReadData[SFR_Inf_StartAdd+21] < m_GetOTPCheckSpec.m_MinField8_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UL_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+21],m_GetOTPCheckSpec.m_MinField8_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+22] < m_GetOTPCheckSpec.m_MinField8_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UL_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+22],m_GetOTPCheckSpec.m_MinField8_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+23] < m_GetOTPCheckSpec.m_MinField8_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UR_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+23],m_GetOTPCheckSpec.m_MinField8_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+24] < m_GetOTPCheckSpec.m_MinField8_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_UR_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+24],m_GetOTPCheckSpec.m_MinField8_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+25] < m_GetOTPCheckSpec.m_MinField8_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LR_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+25],m_GetOTPCheckSpec.m_MinField8_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+26] < m_GetOTPCheckSpec.m_MinField8_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LR_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+26],m_GetOTPCheckSpec.m_MinField8_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+27] < m_GetOTPCheckSpec.m_MinField8_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LL_Infinity_H is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+27],m_GetOTPCheckSpec.m_MinField8_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+28] < m_GetOTPCheckSpec.m_MinField8_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.8_Field_LL_Infinity_V is %d !> %d"),OTPReadData[SFR_Inf_StartAdd+28],m_GetOTPCheckSpec.m_MinField8_Infinity);
		return FALSE;
	}

	if (OTPReadData[SFR_Inf_StartAdd+29] > m_GetOTPCheckSpec.m_MaxField6CTC_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_CTC_Infinity_H is %d !< %d"),OTPReadData[SFR_Inf_StartAdd+29],m_GetOTPCheckSpec.m_MaxField6CTC_Infinity);
		return FALSE;
	}
	if (OTPReadData[SFR_Inf_StartAdd+30] > m_GetOTPCheckSpec.m_MaxField6CTC_Infinity)
	{
		Catstring(ErrorMsg,_T("SFR 0.6_Field_CTC_Infinity_V is %d !< %d"),OTPReadData[SFR_Inf_StartAdd+30],m_GetOTPCheckSpec.m_MaxField6CTC_Infinity);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckAFCode_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check AF	
	int i = 0;
	if (OTPReadData[0x0036] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of AF Calibration错误!"));
		return FALSE;
	}

	if (OTPReadData[0x0037] != 0xff)
	{
		Catstring(ErrorMsg,_T("AF Equip number错误!"));
		return FALSE;
	}

	if ((OTPReadData[0x0038] != 0x00) || (OTPReadData[0x0039] != 0x00))
	{
		Catstring(ErrorMsg,_T("AF Start current错误!"));
		return FALSE;
	}
	for (i = 0x003E; i < 0x0042; i++)
	{
		if (OTPReadData[i] != 0xff)
		{
			Catstring(ErrorMsg,_T("AF Reserve)错误!"));
			return FALSE;
		}
	}

	int MacroCode =  OTPReadData[0x003D] + (OTPReadData[0x003C]*256);
	int Infinity  =  OTPReadData[0x003B] + (OTPReadData[0x003A]*256);

	int m_AFCodeDiff		= MacroCode - Infinity;


	if ((MacroCode > m_GetOTPCheckSpec1.m_MaxMacroCode) || (MacroCode < m_GetOTPCheckSpec1.m_MinMacroCode))
	{
		Catstring(ErrorMsg, _T("m_MacroCode %d not in<%d,%d>"),MacroCode,m_GetOTPCheckSpec1.m_MinMacroCode,m_GetOTPCheckSpec1.m_MaxMacroCode);
		return FALSE;
	}
	if ((Infinity > m_GetOTPCheckSpec1.m_MaxInfinityCode) ||( Infinity < m_GetOTPCheckSpec1.m_MinInfinityCode))
	{
		Catstring(ErrorMsg, _T("m_InfinityCode %d not in<%d,%d>"),Infinity,m_GetOTPCheckSpec1.m_MinInfinityCode,m_GetOTPCheckSpec1.m_MaxInfinityCode);
		return FALSE;
	}
	if ((m_AFCodeDiff > m_GetOTPCheckSpec1.m_MaxCodeDiff) || (m_AFCodeDiff < m_GetOTPCheckSpec1.m_MinCodeDiff))
	{
		Catstring(ErrorMsg, _T("AFCodeDiff %d not in<%d,%d>"),m_AFCodeDiff,m_GetOTPCheckSpec1.m_MinCodeDiff,m_GetOTPCheckSpec1.m_MaxCodeDiff);
		return FALSE;
	}

	long _CHECKSUM = 0;
	for (int mm = 0x0036; mm < 0x0042; mm++)
	{
		_CHECKSUM += OTPReadData[mm];
	}

	if ((OTPReadData[0x0042] != ((_CHECKSUM >> 8) & 0xff)) || (OTPReadData[0x0043] != (_CHECKSUM & 0xff)))
	{
		Catstring(ErrorMsg,_T("Checksum OF AF Error!"));
		return FALSE;
	}  
	//m_pInterface->Ctrl_Addlog(CamID,_T("MONO AF Check Pass!"),COLOR_BLUE,200);
	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommSPC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	// check Gain map	
	int bPDAFVersionNum			= ((OTPReadData[0x0E74]<<8)&0xFF00) + OTPReadData[0x0E75];
	int bGainMapWidth			= ((OTPReadData[0x0E76]<<8)&0xFF00) + OTPReadData[0x0E77];
	int bGainMapHeight			= ((OTPReadData[0x0E78]<<8)&0xFF00) + OTPReadData[0x0E79];

	int m_value = ((OTPReadData[0x0E70]<<8)&0xFF00) +OTPReadData[0x0E71];
	if (m_value != m_GetOTPCheckSpec.m_SegmentPDAFTagID)
	{
		Catstring(ErrorMsg, _T("m_SegmentPDAFTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentPDAFTagID);
		return FALSE;
	}
	if (OTPReadData[0x0E72] != m_GetOTPCheckSpec.m_PDAFFlag)
	{
		Catstring(ErrorMsg, _T("m_PDAFFlag 0x%02X != 0x%02X"),OTPReadData[0x0E72],m_GetOTPCheckSpec.m_PDAFFlag);
		return FALSE;
	}

	if (OTPReadData[0x0E73] != m_GetOTPCheckSpec.m_PDAFTypeNumber)
	{
		Catstring(ErrorMsg, _T("m_PDAFTypeNumber 0x%02X != 0x%02X"),OTPReadData[0x0E73],m_GetOTPCheckSpec.m_PDAFTypeNumber);
		return FALSE;
	}
	if (bPDAFVersionNum != m_GetOTPCheckSpec.m_PDAFVersionNum)
	{
		Catstring(ErrorMsg, _T("PDAFVersionNumber: 0x%02X != 0x%02X"),bPDAFVersionNum,m_GetOTPCheckSpec.m_PDAFVersionNum);
		return FALSE;
	}
	if (bGainMapWidth != m_GetOTPCheckSpec.m_GainMapWidth)
	{
		Catstring(ErrorMsg, _T("GainMapWidth: 0x%02X != 0x%02X"),bGainMapWidth,m_GetOTPCheckSpec.m_GainMapWidth);
		return FALSE;
	}	
	if (bGainMapHeight != m_GetOTPCheckSpec.m_GainMapHeight)
	{
		Catstring(ErrorMsg, _T("GainMapHeight: 0x%02X != 0x%02X"),bGainMapHeight,m_GetOTPCheckSpec.m_GainMapHeight);
		return FALSE;
	}	

	int m_GainMap = 0;
	for (int i = 0x0E7A;i < 0x11EE;i += 2)
	{
		m_GainMap = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];
		if ((m_GainMap > m_GetOTPCheckSpec.m_MaxGainMap) || (m_GainMap < m_GetOTPCheckSpec.m_MinGainMap))
		{
			Catstring(ErrorMsg,_T("GainMap[0x%04X~0x%04X]: %d,not in <%d,%d>"),i,i+1,m_GainMap,m_GetOTPCheckSpec.m_MinGainMap,m_GetOTPCheckSpec.m_MaxGainMap);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommSPC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	// check Gain map	
	int bPDAFVersionNum			= ((OTPReadData[0x0E74]<<8)&0xFF00) + OTPReadData[0x0E75];
	int bGainMapWidth			= ((OTPReadData[0x0E76]<<8)&0xFF00) + OTPReadData[0x0E77];
	int bGainMapHeight			= ((OTPReadData[0x0E78]<<8)&0xFF00) + OTPReadData[0x0E79];

	int m_value = ((OTPReadData[0x0E70]<<8)&0xFF00) +OTPReadData[0x0E71];
	if (m_value != m_GetOTPCheckSpec1.m_SegmentPDAFTagID)
	{
		Catstring(ErrorMsg, _T("m_SegmentPDAFTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec1.m_SegmentPDAFTagID);
		return FALSE;
	}
	if (OTPReadData[0x0E72] != m_GetOTPCheckSpec1.m_PDAFFlag)
	{
		Catstring(ErrorMsg, _T("m_PDAFFlag 0x%02X != 0x%02X"),OTPReadData[0x0E72],m_GetOTPCheckSpec1.m_PDAFFlag);
		return FALSE;
	}

	if (OTPReadData[0x0E73] != m_GetOTPCheckSpec1.m_PDAFTypeNumber)
	{
		Catstring(ErrorMsg, _T("m_PDAFTypeNumber 0x%02X != 0x%02X"),OTPReadData[0x0E73],m_GetOTPCheckSpec1.m_PDAFTypeNumber);
		return FALSE;
	}
	if (bPDAFVersionNum != m_GetOTPCheckSpec1.m_PDAFVersionNum)
	{
		Catstring(ErrorMsg, _T("PDAFVersionNumber: 0x%02X != 0x%02X"),bPDAFVersionNum,m_GetOTPCheckSpec1.m_PDAFVersionNum);
		return FALSE;
	}
	if (bGainMapWidth != m_GetOTPCheckSpec1.m_GainMapWidth)
	{
		Catstring(ErrorMsg, _T("GainMapWidth: 0x%02X != 0x%02X"),bGainMapWidth,m_GetOTPCheckSpec1.m_GainMapWidth);
		return FALSE;
	}	
	if (bGainMapHeight != m_GetOTPCheckSpec1.m_GainMapHeight)
	{
		Catstring(ErrorMsg, _T("GainMapHeight: 0x%02X != 0x%02X"),bGainMapHeight,m_GetOTPCheckSpec1.m_GainMapHeight);
		return FALSE;
	}	

	int m_GainMap = 0;
	for (int i = 0x0E7A;i < 0x11EE;i += 2)
	{
		m_GainMap = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];
		if ((m_GainMap > m_GetOTPCheckSpec1.m_MaxGainMap) || (m_GainMap < m_GetOTPCheckSpec1.m_MinGainMap))
		{
			Catstring(ErrorMsg,_T("GainMap[0x%04X~0x%04X]: %d,not in <%d,%d>"),i,i+1,m_GainMap,m_GetOTPCheckSpec1.m_MinGainMap,m_GetOTPCheckSpec1.m_MaxGainMap);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckSonySPC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	// check Gain map	
	if (OTPReadData[0x0AE4] != 0x01)
	{
		Catstring(ErrorMsg, _T("Flag of SPC Error: 0x%02X != 0x01"), OTPReadData[0x0AE4]);
		return FALSE;
	}

	if (OTPReadData[0x0AE5] != 0xFF)
	{
		Catstring(ErrorMsg, _T("Equip Number Error: 0x%02X != 0xFF"), OTPReadData[0x0AE5]);
		return FALSE;
	}

	//int m_GainMap = 0;
	for (int i = 0/*0x0AE6*/; i < 88; i++)
	{
		//m_GainMap = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];
		if ((OTPReadData[i + 0x0AE6] > m_GetOTPCheckSpec1.m_MaxSPC) || (OTPReadData[i + 0x0AE6] < m_GetOTPCheckSpec1.m_MinSPC))
		{
			Catstring(ErrorMsg,_T("SPC GainMap[0x%04X]: %d,not in <%d,%d>"), i + 0x0AE6, OTPReadData[i + 0x0AE6], m_GetOTPCheckSpec1.m_MinSPC, m_GetOTPCheckSpec1.m_MaxSPC);
			return FALSE;
		}

		if ((OTPReadData[i + 0x0B3E] > m_GetOTPCheckSpec1.m_MaxSPC) || (OTPReadData[i + 0x0B3E] < m_GetOTPCheckSpec1.m_MinSPC))
		{
			Catstring(ErrorMsg,_T("SPC GainMap[0x%04X]: %d,not in <%d,%d>"), i + 0x0B3E, OTPReadData[i + 0x0B3E], m_GetOTPCheckSpec1.m_MinSPC, m_GetOTPCheckSpec1.m_MaxSPC);
			return FALSE;
		}
	}

	int i = 0;
	int m_ChkSum = 0;
	for (i = 0/*0x0AE6*/; i < (2 * 88 + 2); i++)	//最大值加2的原因是有一个Flag字节和一个Equip Number字节
	{
		m_ChkSum += OTPReadData[i + 0x0AE4];
	}
	int m_ChkSum2 = 0;
	m_ChkSum2 = OTPReadData[i + 0x0AE4];
	m_ChkSum2 <<= 8;
	m_ChkSum2 |= OTPReadData[i + 1 + 0x0AE4];
	if (m_ChkSum2 != m_ChkSum)
	{
		Catstring(ErrorMsg, _T("SPC Gain Map CheckSum Error--Cal Val:0x%02X != Saved Val:0x%02X"), m_ChkSum, m_ChkSum2);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckHisiSPC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//卡控SPC数据不允许存在多个连续相等值
	int m_SPCDataEqualRef = OTPReadData[0x0AF3];
	int m_SPCDataEqualCount = 0;	
	for (int i = 0x0AF3;i < 0x0B71;i++)
	{
		if (OTPReadData[i] == m_SPCDataEqualRef)
		{
			m_SPCDataEqualCount++;
			if (m_SPCDataEqualCount > 5)
			{
				Catstring(ErrorMsg,_T("SPC数据存在%d个连续相等的值，规格为5个!"),m_SPCDataEqualCount);
				m_SPCDataEqualCount = 0;
				return FALSE;
			}
		}
		else
		{
			m_SPCDataEqualCount = 0;
			m_SPCDataEqualRef = OTPReadData[i];
		}
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckArcsoft_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	if (OTPReadData[0x078D] != m_GetOTPCheckSpec.m_ArcsoftFlag)
	{
		Catstring(ErrorMsg,_T("Arcsoft Flag 0x078D is 0x%02X != 0x%02X"),OTPReadData[0x078D],m_GetOTPCheckSpec.m_ArcsoftFlag);
		return FALSE;
	}

	//Check Arcsoft checksum
	int ArcsoftChecksum = 0;
	for (int i = 0x078D;i < 0x0F8E;i++)
	{
		ArcsoftChecksum += OTPReadData[i];
	}
	ArcsoftChecksum = (ArcsoftChecksum%255);

	if (ArcsoftChecksum != OTPReadData[0x0F8E])
	{
		Catstring(ErrorMsg, _T("Arcsoft CheckSum 0x0F8E is 0x%02X != 0x%02X"),OTPReadData[0x0F8E],ArcsoftChecksum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckCrossTalk_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	/*********************************************************************/
	/*                   CrossTalk check						         */
	/*********************************************************************/
	//if (OTPReadData[0x072A] != m_GetOTPCheckSpec.m_CrossTalkFlag)
	//{
	//	Catstring(ErrorMsg,_T("m_CrossTalkFlag is 0x%02X != 0x%02X"),OTPReadData[0x072A],m_GetOTPCheckSpec.m_CrossTalkFlag);
	//	return FALSE;
	//}
	
	// check CrossTalk checksum
	int CRC = GetCRC16(&OTPReadData[CTalk_StartAdd], CTalk_EndAdd - CTalk_StartAdd - 1); 
	if (((OTPReadData[CTalk_EndAdd-1]<<8) + (OTPReadData[CTalk_EndAdd])) != CRC)
	{
		Catstring(ErrorMsg,_T("CrossTalk CheckSum is 0x%04X != 0x%04X"),(OTPReadData[CTalk_EndAdd-1]<<8) + (OTPReadData[CTalk_EndAdd]),CRC);
		return FALSE;
	}
	return TRUE;
}

BOOL OtpCheckAPI::CheckOthers(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	/*********************************************************************/
	/*                   Altek check									 */
	/*********************************************************************/
	for (int i = 0x1142;i < 0x14F6;i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("Altek [0x%04X]: 0x%02X != 0xFF"),i,OTPReadData[i]);
			Catstring(ErrorMsg,_T("Altek reserved data should be 0xFF!"));
			Catstring(ErrorMsg,_T("Altek reserved 烧录错误!"));
			return FALSE;
		}
	}

	/*********************************************************************/
	/*                   IQ Master check								 */
	/*********************************************************************/
	int m_MasterTag = 0,m_MasterStatus = 0,m_MasterVersion = 0,m_MasterColorTemp = 0,m_MasterRmean = 0,
		m_MasterGmean = 0,m_MasterBmean = 0,m_MainTrunkVersion = 0,m_SettingVersion = 0;
	m_MasterTag			= ((OTPReadData[0x14F6]<<8)&0xFF00) + (OTPReadData[0x14F7]&0xFF);
	m_MasterStatus		=   OTPReadData[0x14F8];
	m_MasterVersion		=   OTPReadData[0x14F9];
	m_MasterColorTemp	= ((OTPReadData[0x14FA]<<8)&0xFF00) + (OTPReadData[0x14FB]&0xFF);
	m_MasterRmean		= ((OTPReadData[0x14FC]<<8)&0xFF00) + (OTPReadData[0x14FD]&0xFF);
	m_MasterGmean		= ((OTPReadData[0x14FE]<<8)&0xFF00) + (OTPReadData[0x14FF]&0xFF);
	m_MasterBmean		= ((OTPReadData[0x1500]<<8)&0xFF00) + (OTPReadData[0x1501]&0xFF);
	m_MainTrunkVersion  = ((OTPReadData[0x1502]<<8)&0xFF00) + (OTPReadData[0x1503]&0xFF);
	m_SettingVersion	= ((OTPReadData[0x1504]<<8)&0xFF00) + (OTPReadData[0x1505]&0xFF);

	//待添加卡控

	//Check IQMaster checksum
	long IQMasterChecksum = 0;
	for (int i = 0x14F6;i < 0x1527;i++)
	{
		IQMasterChecksum += OTPReadData[i];
	}
	IQMasterChecksum = (IQMasterChecksum%255);
	if (OTPReadData[0x1527] != IQMasterChecksum)
	{
		Catstring(ErrorMsg,_T("IQMaster Checksum 0x1527 is 0x%02X != 0x%02X"),OTPReadData[0x1527],IQMasterChecksum);
		Catstring(ErrorMsg,_T("IQMaster 烧录错误!"));
		return FALSE;
	}

	/*********************************************************************/
	/*						 IQ Slave check								 */
	/*********************************************************************/
	int m_SlaveTag = 0,m_SlaveStatus = 0,m_SlaveVersion = 0,m_SlaveColorTemp = 0,m_SlaveRmean = 0,
		m_SlaveGmean = 0,m_SlaveBmean = 0;
	m_SlaveTag			= ((OTPReadData[0x1528]<<8)&0xFF00) + (OTPReadData[0x1529]&0xFF);
	m_SlaveStatus		=   OTPReadData[0x152A];
	m_SlaveVersion		=   OTPReadData[0x152B];
	m_SlaveColorTemp	= ((OTPReadData[0x152C]<<8)&0xFF00) + (OTPReadData[0x152D]&0xFF);
	m_SlaveRmean		= ((OTPReadData[0x152E]<<8)&0xFF00) + (OTPReadData[0x152F]&0xFF);
	m_SlaveGmean		= ((OTPReadData[0x1530]<<8)&0xFF00) + (OTPReadData[0x1531]&0xFF);
	m_SlaveBmean		= ((OTPReadData[0x1532]<<8)&0xFF00) + (OTPReadData[0x1533]&0xFF);

	//待添加卡控

	//Check reserved
	for (int i = 0x1534;i < 0x1559;i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("IQ Slave Reserved 0x%04X is 0x%02X != 0xFF"));
			return FALSE;
		}
	}

	//Check IQSlave checksum
	long IQSlaveChecksum = 0;
	for (int i = 0x1528;i < 0x1559;i++)
	{
		IQSlaveChecksum += OTPReadData[i];
	}
	IQSlaveChecksum = (IQSlaveChecksum%255);
	if (OTPReadData[0x1559] != IQSlaveChecksum)
	{
		Catstring(ErrorMsg,_T("IQSlave Checksum 0x1559 is 0x%02X != 0x%02X"),OTPReadData[0x1559],IQSlaveChecksum);
		Catstring(ErrorMsg,_T("IQSlave 烧录错误!"));
		return FALSE;
	}

	/*********************************************************************/
	/*					Manufacturing check								 */
	/*********************************************************************/
	//待添加卡控

	/*********************************************************************/
	/*						2D check									 */
	/*********************************************************************/
	//待添加卡控

	/*********************************************************************/
	/*						3D check									 */
	/*********************************************************************/
	//待添加卡控


	//Check TotalCheckSum
	long TotalCheckSum = 0;
	for (int i = 0x00;i < 0x1FFE;i++)
	{
		TotalCheckSum += OTPReadData[i];
	}
	if (OTPReadData[0x1FFE] != ((TotalCheckSum>>8)%256))
	{
		Catstring(ErrorMsg,_T("TotalCheckSum 0x1FFE is 0x%02X != 0x%02X"),OTPReadData[0x1FFE],(TotalCheckSum>>8)%256);
		Catstring(ErrorMsg,_T("TotalCheckSum 烧录错误!"));
		return FALSE;
	}
	if (OTPReadData[0x1FFF] != (TotalCheckSum%256))
	{
		Catstring(ErrorMsg,_T("TotalCheckSum 0x1FFF is 0x%02X != 0x%02X"),OTPReadData[0x1FFF],TotalCheckSum%256);
		Catstring(ErrorMsg,_T("TotalCheckSum 烧录错误!"));
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckAEC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int bBrightnessRatio	= ((OTPReadData[0x1202]<<8)&0xFF00) + OTPReadData[0x1203];
	int bMainGain			= ((OTPReadData[0x1208]<<8)&0xFF00) + OTPReadData[0x1209];
	int bMainExposureTime	= ((OTPReadData[0x120A]<<8)&0xFF00) + OTPReadData[0x120B];
	int bSubGain			= ((OTPReadData[0x1204]<<8)&0xFF00) + OTPReadData[0x1205];
	int bSubExposureTime	= ((OTPReadData[0x1206]<<8)&0xFF00) + OTPReadData[0x1207];
	int i = 0;
	if (OTPReadData[0x1200] != 0x01)
	{
		Catstring(ErrorMsg,_T("AEC Flag is 0x%02X != 0x01"),OTPReadData[0x1200]);
		return FALSE;
	}

	if (OTPReadData[0x1201] != 0xFF)
	{
		Catstring(ErrorMsg,_T("AEC Equip number is 0x%02X != 0xFF"),OTPReadData[0x1201]);
		return FALSE;
	}

	if ((bBrightnessRatio > m_GetOTPCheckSpec.m_MaxBrightnessRatio) || (bBrightnessRatio < m_GetOTPCheckSpec.m_MinBrightnessRatio))
	{
		Catstring(ErrorMsg, _T("BrightnessRatio: %d,not in <%d,%d>"),bBrightnessRatio,m_GetOTPCheckSpec.m_MinBrightnessRatio,m_GetOTPCheckSpec.m_MaxBrightnessRatio);
		return FALSE;
	}
	if (bMainGain != m_GetOTPCheckSpec.m_MainGain)
	{
		Catstring(ErrorMsg, _T("MainGain %d != %d"),bMainGain,m_GetOTPCheckSpec.m_MainGain);
		return FALSE;
	}
	if (bMainExposureTime != m_GetOTPCheckSpec.m_MainExposureTime)
	{
		Catstring(ErrorMsg, _T("MainExposureTime %d != %d"),bMainExposureTime,m_GetOTPCheckSpec.m_MainExposureTime);
		return FALSE;
	}
	if (bSubGain != m_GetOTPCheckSpec.m_SubGain)
	{
		Catstring(ErrorMsg, _T("SubGain %d != %d"),bSubGain,m_GetOTPCheckSpec.m_SubGain);
		return FALSE;
	}
	if (bSubExposureTime != m_GetOTPCheckSpec.m_SubExposureTime)
	{
		Catstring(ErrorMsg, _T("SubExposureTime %d != %d"),bSubExposureTime,m_GetOTPCheckSpec.m_SubExposureTime);
		return FALSE;
	}
// 	if (OTPReadData[0x0E6E] != 0xFF)
// 	{
// 		Catstring(ErrorMsg, _T("AEC Reserved 0x0E6E is 0x%02X != 0xFF"),OTPReadData[0x0E6E]);
// 		return FALSE;
// 	}

	// check AEC checksum
	int AECCheckSum = 0;//GetCRC16(&OTPReadData[0x0027],24);
	for (i = 0x1200; i < 0x120C; i++)
	{
		AECCheckSum = AECCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x120C] != ((AECCheckSum >> 8) & 0xff)) || (OTPReadData[0x120D] != (AECCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n AEC Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x120C],OTPReadData[0x120D],AECCheckSum);
		return FALSE;
	}
	//m_pInterface->Ctrl_Addlog(CamID,_T("AEC Check Pass!"),COLOR_BLUE,200);
	return TRUE;
}

BOOL OtpCheckAPI::CheckAEC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	// check reserve
	for (int i = 0x1255; i < 0x18ed; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("AEC reserve[%d]: %d != 0xFF!"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	// check SlaveCamera checksum
	int m_SlaveCameraSum  = 0;
	for (int i = 0; i<0x18fe; i++)
	{
		m_SlaveCameraSum += OTPReadData[i];
	}
	m_SlaveCameraSum = (m_SlaveCameraSum%255) + 1;
	if (m_SlaveCameraSum != OTPReadData[0x18fe])
	{
		Catstring(ErrorMsg,_T("SlaveCamera total CheckSum %d != %d"),m_SlaveCameraSum,OTPReadData[0x18fe]);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckOIS_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (OTPReadData[0x1B00] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of OIS Calibration is 0x%02X != 0x01"),OTPReadData[0x1B00]);
		return FALSE;
	}

	int OISCheckSum = 0;//GetCRC16(&OTPReadData[0x0027],24);
	for (i = 0x1B00; i < 0x1B27; i++)
	{
		OISCheckSum = OISCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x1B27] != ((OISCheckSum >> 8) & 0xff)) || (OTPReadData[0x1B28] != (OISCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n OIS Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x1B27],OTPReadData[0x1B28],OISCheckSum);
		return FALSE;
	}
	//m_pInterface->Ctrl_Addlog(CamID,_T("OIS Check Pass!"),COLOR_BLUE,200);
	return TRUE;
}

BOOL OtpCheckAPI::CheckAFSync_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (OTPReadData[0x1D00] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of AFSync Error---0x%02X != 0x01"), OTPReadData[0x1D00]);
		return FALSE;
	}

	if ((OTPReadData[0x1D01] != 0x00) || (OTPReadData[0x1D02] != 0x01))
	{
		Catstring(ErrorMsg,_T("Version Number of AFSync Error---0x%02X%02X != 0x0001"), OTPReadData[0x1D01], OTPReadData[0x1D02]);
		return FALSE;
	}

	if ((OTPReadData[0x1D03] != 0x00) || (OTPReadData[0x1D04] != 0x03))
	{
		Catstring(ErrorMsg,_T("Number of Distance of AFSync Error---0x%02X%02X != 0x0001"), OTPReadData[0x1D03], OTPReadData[0x1D04]);
		return FALSE;
	}

	int AFSyncCheckSum = 0;//GetCRC16(&OTPReadData[0x0027],24);
	for (i = 0x1D00; i < 0x1D11; i++)
	{
		AFSyncCheckSum += OTPReadData[i];
	}
	if ((OTPReadData[0x1D11] != ((AFSyncCheckSum >> 8) & 0xFF))
		|| (OTPReadData[0x1D12] != (AFSyncCheckSum  & 0xFF))
		)
	{
		Catstring(ErrorMsg,_T("\n OIS Data CheckSum is 0x%04X 0x%04X != 0x%04X"), OTPReadData[0x1D11], OTPReadData[0x1D12], AFSyncCheckSum & 0xFFFF);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckAFSync_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	if (OTPReadData[0x5AB] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of AFSync Error---0x%02X != 0x01"), OTPReadData[0x5AB]);
		return FALSE;
	}

	if ((OTPReadData[0x5AC] != 0x00) || (OTPReadData[0x5AD] != 0x01))
	{
		Catstring(ErrorMsg,_T("Version Number of AFSync Error---0x%02X%02X != 0x0001"), OTPReadData[0x5AC], OTPReadData[0x5AD]);
		return FALSE;
	}

	if ((OTPReadData[0x5AE] != 0x00) || (OTPReadData[0x5AF] != 0x03))
	{
		Catstring(ErrorMsg,_T("Number of Distance of AFSync Error---0x%02X%02X != 0x0001"), OTPReadData[0x5AE], OTPReadData[0x5AF]);
		return FALSE;
	}

	int AFSyncCheckSum = 0;//GetCRC16(&OTPReadData[0x0027],24);
	for (i = 0x5AB; i < 0x5BC; i++)
	{
		AFSyncCheckSum += OTPReadData[i];
	}
	if ((OTPReadData[0x5BC] != ((AFSyncCheckSum >> 8) & 0xFF))
		|| (OTPReadData[0x5BD] != (AFSyncCheckSum  & 0xFF))
		)
	{
		Catstring(ErrorMsg,_T("\n OIS Data CheckSum is 0x%04X 0x%04X != 0x%04X"), OTPReadData[0x5BC], OTPReadData[0x5BD], AFSyncCheckSum & 0xFFFF);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckTotalCheckSum_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int m_TotalSum  = 0;
	for (int i = 0x00; i < 0x0A46; i++)
	{
		m_TotalSum += OTPReadData[i];
	}
	m_TotalSum = (m_TotalSum%255);
	if (m_TotalSum != ((OTPReadData[0x0A46]<<8) + OTPReadData[0x0A47]))
	{
		Catstring(ErrorMsg,_T("TotalCheckSum 0x0A46+0x0A47 is 0x%04X != 0x%04X"),(OTPReadData[0x0A46]<<8) + OTPReadData[0x0A47],m_TotalSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckDualCamera_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	for (i = 0x1A1C; i<0x1B00; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("DualCamera reserve[0x%04X]: %d != 0xFF!"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	if (OTPReadData[0x120E] != 0x01)
	{
		Catstring(ErrorMsg,_T("Dual Camera calibration Flag is 0x%02X != 0x01"),OTPReadData[0x1200]);
		return FALSE;
	}

	if (OTPReadData[0x120F] != 0xFF)
	{
		Catstring(ErrorMsg,_T("Dual Equip number is 0x%02X != 0xFF"),OTPReadData[0x1201]);
		return FALSE;
	}

	int DualCheckSum = 0;//GetCRC16(&OTPReadData[0x0027],24);
	for (i = 0x120E; i < 0x1A10; i++)
	{
		DualCheckSum += OTPReadData[i];
	}
	for (i = 0x1A12; i < 0x1A16; i++)
	{
		DualCheckSum += OTPReadData[i];
	}

	if ((OTPReadData[0x1A10] != ((DualCheckSum >> 8) & 0xff)) || (OTPReadData[0x1A11] != (DualCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n Dual Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x1A10],OTPReadData[0x1A11],DualCheckSum&0xFFFF);
		return FALSE;
	}

	long TotalCheckSum = 0;
	for (i = 0x0000; i < 0x05AB; i++)
	{
		TotalCheckSum += OTPReadData[i];
	}

	for (i = 0x0600; i < 0x0BAB; i++)
	{
		TotalCheckSum += OTPReadData[i];
	}

	for (i = 0x0D00; i < 0x11FC; i++)
	{
		TotalCheckSum += OTPReadData[i];
	}

	for (i = 0x1200; i < 0x1A16; i++)	//和锤子科技的王亮核实过，Total CheckSum只加到0x1A15处，RotationX、RotationY、RotationZ三个数据的六个字节不加入计算--Added by LiHai--20180203
	{
		TotalCheckSum += OTPReadData[i];
	}

	for (i = 0x1B00; i < 0x1B29; i++)
	{
		TotalCheckSum += OTPReadData[i];
	}

	if ((OTPReadData[0x05AB] != ((TotalCheckSum >> 8) & 0xff)) || (OTPReadData[0x05AC] != (TotalCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n Total Data CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x05AB],OTPReadData[0x05AC],TotalCheckSum&0xFFFF);
		return FALSE;
	}
	//m_pInterface->Ctrl_Addlog(CamID,_T("Dual Check Pass!"),COLOR_BLUE,200);
	return TRUE;
#if 0
	int m_value = ((OTPReadData[0x1360]<<8)&0xFF00) +OTPReadData[0x1361];
	if (m_value != m_GetOTPCheckSpec.m_SegmentDualCamTagID)
	{
		Catstring(ErrorMsg, _T("SegmentDualCamTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentDualCamTagID);
		return FALSE;
	}
	if (OTPReadData[0x1362] != m_GetOTPCheckSpec.m_DualCamFlag)
	{
		Catstring(ErrorMsg, _T("DualCamFlag 0x%02X != 0x%02X"),OTPReadData[0x1362],m_GetOTPCheckSpec.m_DualCamFlag);
		return FALSE;
	}
	if (OTPReadData[0x1363] != m_GetOTPCheckSpec.m_DualCamVersionNumber)
	{
		Catstring(ErrorMsg, _T("DualCamVersionNumber: 0x%02X != 0x%02X"),OTPReadData[0x1363],m_GetOTPCheckSpec.m_DualCamVersionNumber);
		return FALSE;
	}

	// check DualCam checksum
	int m_DualCamSum = 0;
	for (int i = 0x1363; i < 0x18EC; i++)
	{
		m_DualCamSum += OTPReadData[i];
	}
	m_DualCamSum = (m_DualCamSum%255) +1;
	if (m_DualCamSum != OTPReadData[0x18EC])
	{
		Catstring(ErrorMsg,_T("DualCam CheckSum 0x%02X != 0x%02X"),OTPReadData[0x18EC],m_DualCamSum);
		return FALSE;
	}

	// check reserve
	for (int i = 0x153A; i < 0x18EC; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("DualCamera reserve[0x%04X]: 0x%02X != 0xFF!"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	// check Total checksum
	int m_TotalSum = 0;
	for (int i = 0x0000; i < 0x1902; i++)
	{
		m_TotalSum += OTPReadData[i];
	}
	m_TotalSum = (m_TotalSum%255) +1;
	if (m_TotalSum != OTPReadData[0x1902])
	{
		Catstring(ErrorMsg,_T("Total CheckSum 0x%02X != 0x%02X"),OTPReadData[0x1902],m_TotalSum);
		return FALSE;
	}

	return TRUE;
#endif
}

BOOL OtpCheckAPI::CheckHisiPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//华为Hisi专用
	//卡控DCC数据不允许存在多个连续相等值
	int m_DCCDataEqualRef = OTPReadData[0x0B71];
	int m_DCCDataEqualCount = 0;	
	for (int i = 0x0B71;i < 0x0BD1;i++)
	{
		if (OTPReadData[i] == m_DCCDataEqualRef)
		{
			m_DCCDataEqualCount++;
			if (m_DCCDataEqualCount > 5)
			{
				Catstring(ErrorMsg,_T("DCC数据存在%d个连续相等的值，规格为5个!"),m_DCCDataEqualCount);
				m_DCCDataEqualCount = 0;
				return FALSE;
			}
		}
		else
		{
			m_DCCDataEqualCount = 0;
			m_DCCDataEqualRef = OTPReadData[i];
		}
	}

	//PDAF Checksum
	int m_PDAFCheckSum = 0;
	for (int i = 0x0AF3;i < 0x0BD1;i++)
	{
		m_PDAFCheckSum += OTPReadData[i];
	}
	m_PDAFCheckSum = (m_PDAFCheckSum%65535);
	if (((m_PDAFCheckSum>>8)&0xFF) != OTPReadData[0x0BD1])
	{
		Catstring(ErrorMsg, _T("PDAF CheckSum 0x0BD1 is 0x%02X != 0x%02X"),OTPReadData[0x0BD1],(m_PDAFCheckSum>>8)&0xFF);
		return FALSE;
	}
	if ((m_PDAFCheckSum&0xFF) != OTPReadData[0x0BD2])
	{
		Catstring(ErrorMsg, _T("PDAF CheckSum 0x0BD2 is 0x%02X != 0x%02X"),OTPReadData[0x0BD2],m_PDAFCheckSum&0xFF);
		return FALSE;
	}

	if ((OTPReadData[0x0BD3] < 17) || (OTPReadData[0x0BD3] > 21) ||
		(OTPReadData[0x0BD4] < 1)  || (OTPReadData[0x0BD4] > 12) ||
		(OTPReadData[0x0BD5] < 1)  || (OTPReadData[0x0BD5] > 31) ||
		(OTPReadData[0x0BD6] < 0)  || (OTPReadData[0x0BD6] > 23))
	{
		Catstring(ErrorMsg, _T("PDAF Station Time Info NG."));
		return FALSE;
	}

	int m_PDAFInfoCheckSum = 0;
	for (int i = 0x0BD3;i < 0x0BD9;i++)
	{
		m_PDAFInfoCheckSum += OTPReadData[i];
	}
	m_PDAFInfoCheckSum = (m_PDAFInfoCheckSum%65535);
	if (((m_PDAFInfoCheckSum>>8)&0xFF) != OTPReadData[0x0BD9])
	{
		Catstring(ErrorMsg, _T("PDAF CheckSum 0x0BD9 is 0x%02X != 0x%02X"),OTPReadData[0x0BD9],(m_PDAFInfoCheckSum>>8)&0xFF);
		return FALSE;
	}
	if ((m_PDAFInfoCheckSum&0xFF) != OTPReadData[0x0BDA])
	{
		Catstring(ErrorMsg, _T("PDAF CheckSum 0x0BDA is 0x%02X != 0x%02X"),OTPReadData[0x0BDA],m_PDAFInfoCheckSum&0xFF);
		return FALSE;
	}

	//check Total Checksum
	int m_TotalCheckSum = 0;
	for (int i = 0x0000;i < 0x0BE9;i++)
	{
		m_TotalCheckSum += OTPReadData[i];
	}
	m_TotalCheckSum = (m_TotalCheckSum%65535);
	if (((m_TotalCheckSum>>8)&0xFF) != OTPReadData[0x0BE9])
	{
		Catstring(ErrorMsg, _T("Total CheckSum 0x0BE9 is 0x%02X != 0x%02X"),OTPReadData[0x0BE9],(m_TotalCheckSum>>8)&0xFF);
		return FALSE;
	}
	if ((m_TotalCheckSum&0xFF) != OTPReadData[0x0BEA])
	{
		Catstring(ErrorMsg, _T("Total CheckSum 0x0BEA is 0x%02X != 0x%02X"),OTPReadData[0x0BEA],m_TotalCheckSum&0xFF);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckSonyPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	// check DCC map
	int bDCCFormat				= ((OTPReadData[0x0B07]<<8)&0xFF00) + OTPReadData[0x0B06];
	int bDCCMapWidth			= ((OTPReadData[0x0B09]<<8)&0xFF00) + OTPReadData[0x0B08];
	int bDCCMapHeight			= ((OTPReadData[0x0B0B]<<8)&0xFF00) + OTPReadData[0x0B0A];

	if (bDCCFormat != m_GetOTPCheckSpec.m_DCCFormat)
	{
		Catstring(ErrorMsg, _T("DCCFormat: %d != %d"),bDCCFormat,m_GetOTPCheckSpec.m_DCCFormat);
		return FALSE;
	}
	if (bDCCMapWidth != m_GetOTPCheckSpec.m_DCCMapWidth)
	{
		Catstring(ErrorMsg, _T("DCCMapWidth: %d != %d"),bDCCMapWidth,m_GetOTPCheckSpec.m_DCCMapWidth);
		return FALSE;
	}
	if (bDCCMapHeight != m_GetOTPCheckSpec.m_DCCMapHeight)
	{
		Catstring(ErrorMsg, _T("DCCMapHeight: %d != %d"),bDCCMapHeight,m_GetOTPCheckSpec.m_DCCMapHeight);
		return FALSE;
	}

	int m_DCCMap = 0;
	for (int i = 0x0B0C;i < 0x0B6C;i += 2)
	{
		m_DCCMap = ((OTPReadData[i+1]<<8)&0xFF00) + (OTPReadData[i]);
		if (m_DCCMap > m_GetOTPCheckSpec.m_MaxDCC || m_DCCMap < m_GetOTPCheckSpec.m_MinDCC)
		{
			Catstring(ErrorMsg,_T("DCCMap 0x%04X is %d not in <%d, %d>"),i,m_DCCMap,m_GetOTPCheckSpec.m_MinDCC,m_GetOTPCheckSpec.m_MaxDCC);
			return FALSE;
		}
	}

	if (OTPReadData[0x0B6C] != m_GetOTPCheckSpec.m_GainMapFlag)
	{
		Catstring(ErrorMsg, _T("GainMap Flag 0x0B6C is 0x%02X != 0x%02X"),OTPReadData[0x0B6C],m_GetOTPCheckSpec.m_GainMapFlag);
		return FALSE;
	}

	// check GainMap checksum
	int m_GainMapSum = 0;
	for (int i = 0x0700; i < 0x0B06; i++)
	{
		m_GainMapSum += OTPReadData[i];
	}
	m_GainMapSum = (m_GainMapSum%255);
	if (m_GainMapSum != OTPReadData[0x0B6D])
	{
		Catstring(ErrorMsg,_T("GainMap CheckSum 0x0B6D is 0x%02X != 0x%02X"),OTPReadData[0x0B6D],m_GainMapSum);
		return FALSE;
	}

	if (OTPReadData[0x0B6E] != m_GetOTPCheckSpec.m_DCCFlag)
	{
		Catstring(ErrorMsg, _T("DCC Flag 0x0B6E is 0x%02X != 0x%02X"),OTPReadData[0x0B6E],m_GetOTPCheckSpec.m_DCCFlag);
		return FALSE;
	}

	// check PDAF checksum
	int m_DCCSum = 0;
	for (int i = 0x0B06; i < 0x0B6C; i++)
	{
		m_DCCSum += OTPReadData[i];
	}
	m_DCCSum = (m_DCCSum%255);
	if (m_DCCSum != OTPReadData[0x0B6F])
	{
		Catstring(ErrorMsg,_T("PDAF DCC CheckSum 0x0B6F is 0x%02X != 0x%02X"),OTPReadData[0x0B6F],m_DCCSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckSonyPDAF_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check DCC(不烧录)
	if (OTPReadData[0x0B98] != 0x01)
	{
		Catstring(ErrorMsg, _T("Flag of SPC Error: 0x%02X != 0x01"), OTPReadData[0x0B98]);
		return FALSE;
	}

	if (OTPReadData[0x0B99] != 0xFF)
	{
		Catstring(ErrorMsg, _T("Equip Number Error: 0x%02X != 0xFF"), OTPReadData[0x0B99]);
		return FALSE;
	}

	int i = 0;
	int m_DCC = 0;
	for (i = 0x0B9A; i < 0x0BFA; i += 2)
	{
		m_DCC = ((OTPReadData[i] << 8) & 0xFF00) + OTPReadData[i + 1];
		if ((m_DCC > m_GetOTPCheckSpec1.m_MaxDCC) || (m_DCC < m_GetOTPCheckSpec1.m_MinDCC))
		{
			Catstring(ErrorMsg,_T("Sony DCC[0x%04X~0x%04X]: %d, not in <%d,%d>"), i + MAP_OFFSET, i + MAP_OFFSET + 1, m_DCC, m_GetOTPCheckSpec1.m_MinDCC, m_GetOTPCheckSpec1.m_MaxDCC);
			return FALSE;
		}
	}

	int m_ChkSum = 0;
	for (i = 0/*0x0AE6*/; i < 96 + 2; i++)	//最大值加2的原因是有一个Flag字节和一个Equip Number字节
	{
		m_ChkSum += OTPReadData[i + 0x0B98];
	}
	int m_ChkSum2 = 0;
	m_ChkSum2 = OTPReadData[i + 0x0B98];
	m_ChkSum2 <<= 8;
	m_ChkSum2 |= OTPReadData[i + 0x0B98 + 1];
	if (m_ChkSum2 != m_ChkSum)
	{
		Catstring(ErrorMsg, _T("SPC DCC CheckSum Error--i:0x%02X 0x%02X 0x%02X Cal Val:0x%02X != Saved Val:0x%02X"), i + 0x0B98, OTPReadData[i + 0x0B98],OTPReadData[i + 0x0B98 + 1], m_ChkSum, m_ChkSum2);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int i = 0;
	int bVersionNumber		= ((OTPReadData[0x0D02]<<8)&0xFF00) + OTPReadData[0x0D03];
	int bGainMapWidth		= ((OTPReadData[0x0D04]<<8)&0xFF00) + OTPReadData[0x0D05];
	int bGainMapHeight		= ((OTPReadData[0x0D06]<<8)&0xFF00) + OTPReadData[0x0D07];

	unsigned int m_GainMap	= 0;	

	// check Gain map
	if (bVersionNumber != m_GetOTPCheckSpec.m_PDAFVersionNum)
	{
		Catstring(ErrorMsg, _T("m_PDAFVersionNum: 0x%04X != 0x%04X"),bVersionNumber,m_GetOTPCheckSpec.m_PDAFVersionNum);
		return FALSE;
	}
	if (bGainMapWidth != m_GetOTPCheckSpec.m_GainMapWidth)
	{
		Catstring(ErrorMsg, _T("GainMapWidth: 0x%04X != 0x%04X"),bGainMapWidth,m_GetOTPCheckSpec.m_GainMapWidth);
		return FALSE;
	}	
	if (bGainMapHeight != m_GetOTPCheckSpec.m_GainMapHeight)
	{
		Catstring(ErrorMsg, _T("GainMapHeight: 0x%04X != 0x%04X"),bGainMapHeight,m_GetOTPCheckSpec.m_GainMapHeight);
		return FALSE;
	}	

	for (i = 0x0D08;i < 0x107C;i += 2)
	{
		m_GainMap  = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];
		if (m_GainMap > m_GetOTPCheckSpec.m_MaxGainMap)
		{
			Catstring(ErrorMsg,_T("GainMap[0x%04X~0x%04X]: %d !< %d"),i,i+1,m_GainMap,m_GetOTPCheckSpec.m_MaxGainMap);
			return FALSE;
		}
		if (m_GainMap < m_GetOTPCheckSpec.m_MinGainMap)
		{
			Catstring(ErrorMsg,_T("GainMap[0x%04X~0x%04X]: %d !> %d"),i,i+1,m_GainMap,m_GetOTPCheckSpec.m_MinGainMap);
			return FALSE;
		}
	}

	int bDCCFormat				= ((OTPReadData[0x107C]<<8)&0xFF00) + OTPReadData[0x107D];
	int bDCCMapWidth			= ((OTPReadData[0x107E]<<8)&0xFF00) + OTPReadData[0x107F];
	int bDCCMapHeight			= ((OTPReadData[0x1080]<<8)&0xFF00) + OTPReadData[0x1081];	

	unsigned int m_DCCMap				= 0;	

	if (OTPReadData[0x0D00] != 0x01)
	{
		Catstring(ErrorMsg,_T("Flag of PDAF Calibration is 0x%02X != 0x01"),OTPReadData[0x0D00]);
		return FALSE;
	}

	if (OTPReadData[0x0D01] != 0xFF)
	{
		Catstring(ErrorMsg,_T("PDAF Equip number is 0x%02X != 0xFF"),OTPReadData[0x0D01]);
		return FALSE;
	}

	// check DCC map	
	if (bDCCFormat != m_GetOTPCheckSpec.m_DCCFormat)
	{
		Catstring(ErrorMsg, _T("DCCFormat: 0x%04X != 0x%04X"),bDCCFormat,m_GetOTPCheckSpec.m_DCCFormat);
		return FALSE;
	}
	if (bDCCMapWidth != m_GetOTPCheckSpec.m_DCCMapWidth)
	{
		Catstring(ErrorMsg, _T("DCCMapWidth: 0x%04X != 0x%04X"),bDCCMapWidth,m_GetOTPCheckSpec.m_DCCMapWidth);
		return FALSE;
	}
	if (bDCCMapHeight != m_GetOTPCheckSpec.m_DCCMapHeight)
	{
		Catstring(ErrorMsg, _T("DCCMapHeight: 0x%04X != 0x%04X"),bDCCMapHeight,m_GetOTPCheckSpec.m_DCCMapHeight);
		return FALSE;
	}	

	for (i = 0x1082;i < 0x10E2;i += 2)
	{
		m_DCCMap = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];

		if (m_DCCMap > m_GetOTPCheckSpec.m_MaxDCC)
		{
			Catstring(ErrorMsg,_T("DCCMap[0x%04X~0x%04X]: %d !< %d"),i,i+1,m_DCCMap,m_GetOTPCheckSpec.m_MaxDCC);
			return FALSE;
		}
		if (m_DCCMap < m_GetOTPCheckSpec.m_MinDCC)
		{
			Catstring(ErrorMsg,_T("DCCMap[0x%04X~0x%04X]: %d !> %d"),i,i+1,m_DCCMap,m_GetOTPCheckSpec.m_MinDCC);
			return FALSE;
		}
	}


	int DCCCheckSum = 0;
	for (i = 0x0D00; i < 0x10E2; i++)
	{
		DCCCheckSum = DCCCheckSum + OTPReadData[i];
	}
	if ((OTPReadData[0x10E2] != ((DCCCheckSum >> 8) & 0xff)) || (OTPReadData[0x10E3] != (DCCCheckSum  & 0xff)))
	{
		Catstring(ErrorMsg,_T("\n Total CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x10E2],OTPReadData[0x10E3],DCCCheckSum);
		return FALSE;
	}
	/*m_pInterface->Ctrl_Addlog(CamID,_T("PDAF Check Pass!"),COLOR_BLUE,200);
	m_pInterface->Ctrl_Addlog(CamID,_T("PDAF Check Pass!"),COLOR_BLUE,200);
	m_pInterface->Ctrl_Addlog(CamID,_T("PDAF Check Pass!"),COLOR_BLUE,200);
	m_pInterface->Ctrl_Addlog(CamID,_T("PDAF Check Pass!"),COLOR_BLUE,200);*/
	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommPDAF_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int bDCCFormat				= ((OTPReadData[0x11EE]<<8)&0xFF00) + OTPReadData[0x11EF];
	int bDCCMapWidth			= ((OTPReadData[0x11F0]<<8)&0xFF00) + OTPReadData[0x11F1];
	int bDCCMapHeight			= ((OTPReadData[0x11F2]<<8)&0xFF00) + OTPReadData[0x11F3];

	if (bDCCFormat != m_GetOTPCheckSpec1.m_DCCFormat)
	{
		Catstring(ErrorMsg, _T("DCCFormat: 0x%02X != 0x%02X"),bDCCFormat,m_GetOTPCheckSpec1.m_DCCFormat);
		return FALSE;
	}
	if (bDCCMapWidth != m_GetOTPCheckSpec1.m_DCCMapWidth)
	{
		Catstring(ErrorMsg, _T("DCCMapWidth: 0x%02X != 0x%02X"),bDCCMapWidth,m_GetOTPCheckSpec1.m_DCCMapWidth);
		return FALSE;
	}
	if (bDCCMapHeight != m_GetOTPCheckSpec1.m_DCCMapHeight)
	{
		Catstring(ErrorMsg, _T("DCCMapHeight: 0x%02X != 0x%02X"),bDCCMapHeight,m_GetOTPCheckSpec1.m_DCCMapHeight);
		return FALSE;
	}

	// check DCC map
	unsigned int m_DCCMap = 0;
	for (int i =0x11F4;i<0x1254;i += 2)
	{
		m_DCCMap = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];

		if ((m_DCCMap > m_GetOTPCheckSpec1.m_MaxDCC) || (m_DCCMap < m_GetOTPCheckSpec1.m_MinDCC))
		{
			Catstring(ErrorMsg,_T("DCCMap[0x%04X~0x%04X]: %d,not in <%d,%d>"),i,i+1,m_DCCMap,m_GetOTPCheckSpec1.m_MinDCC,m_GetOTPCheckSpec1.m_MaxDCC);
			return FALSE;
		}
	}		

	// check PDAF checksum
	int m_PDAFSum = 0;
	for (int i = 0x0E73; i < 0x1254; i++)
	{
		m_PDAFSum += OTPReadData[i];
	}
	m_PDAFSum = (m_PDAFSum%255) +1;
	if (m_PDAFSum != OTPReadData[0x1254])
	{
		Catstring(ErrorMsg,_T("PDAF CheckSum 0x%02X != 0x%02X"),OTPReadData[0x1254],m_PDAFSum);
		return FALSE;
	}

	// check reserve
	for (int i = 0x1255; i<0x18ED; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("PDAF reserve[0x%04X]: 0x%02X != 0xFF!"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckOC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	int OCX_R		= (OTPReadData[OC_StartAdd]<<8) + (OTPReadData[OC_StartAdd+1]);
	int OCY_R		= (OTPReadData[OC_StartAdd+2]<<8) + (OTPReadData[OC_StartAdd+3]);

	int OCX_Gr		= (OTPReadData[OC_StartAdd+4]<<8) + (OTPReadData[OC_StartAdd+5]);
	int OCY_Gr		= (OTPReadData[OC_StartAdd+6]<<8) + (OTPReadData[OC_StartAdd+7]);

	int OCX_Gb		= (OTPReadData[OC_StartAdd+8]<<8) + (OTPReadData[OC_StartAdd+9]);
	int OCY_Gb		= (OTPReadData[OC_StartAdd+10]<<8) + (OTPReadData[OC_StartAdd+11]);

	int OCX_B		= (OTPReadData[OC_StartAdd+12]<<8) + (OTPReadData[OC_StartAdd+13]);	
	int OCY_B		= (OTPReadData[OC_StartAdd+14]<<8) + (OTPReadData[OC_StartAdd+15]);

	float Center_Height = m_GetOTPCheckSpec.m_PixelHeight*0.5;
	float Center_Width  = m_GetOTPCheckSpec.m_PixelWidth *0.5;

	double OCDistance_R  = (1.0)*(sqrt(pow((Center_Height-OCX_R),2) +pow((Center_Width-OCY_R),2)))  * m_GetOTPCheckSpec.m_PixelSize;
	double OCDistance_Gr = (1.0)*(sqrt(pow((Center_Height-OCX_Gr),2)+pow((Center_Width-OCY_Gr),2))) * m_GetOTPCheckSpec.m_PixelSize;
	double OCDistance_Gb = (1.0)*(sqrt(pow((Center_Height-OCX_Gb),2)+pow((Center_Width-OCY_Gb),2))) * m_GetOTPCheckSpec.m_PixelSize;
	double OCDistance_B  = (1.0)*(sqrt(pow((Center_Height-OCX_B),2) +pow((Center_Width-OCY_B),2)))  * m_GetOTPCheckSpec.m_PixelSize;

	if (OCDistance_R > m_GetOTPCheckSpec.m_MaxOCDistance)
	{
		Catstring(ErrorMsg,_T("OC OCDistance_R is %.2f !< %d"),OCDistance_R,m_GetOTPCheckSpec.m_MaxOCDistance);
		return FALSE;
	}
	if (OCDistance_Gr > m_GetOTPCheckSpec.m_MaxOCDistance)
	{
		Catstring(ErrorMsg,_T("OC OCDistance_Gr is %.2f !< %d"),OCDistance_Gr,m_GetOTPCheckSpec.m_MaxOCDistance);
		return FALSE;
	}
	if (OCDistance_Gb > m_GetOTPCheckSpec.m_MaxOCDistance)
	{
		Catstring(ErrorMsg,_T("OC OCDistance_Gb is %.2f !< %d"),OCDistance_Gb,m_GetOTPCheckSpec.m_MaxOCDistance);
		return FALSE;
	}
	if (OCDistance_B > m_GetOTPCheckSpec.m_MaxOCDistance)
	{
		Catstring(ErrorMsg,_T("OC OCDistance_B is %.2f !< %d"),OCDistance_B,m_GetOTPCheckSpec.m_MaxOCDistance);
		return FALSE;
	}

	//Check Cal1 Data Checksum(CRC16)
	int CalCRC = GetCRC16(&OTPReadData[OC_StartAdd],OC_EndAdd - OC_StartAdd - 1); 
	if (((OTPReadData[OC_EndAdd - 1]<<8) + (OTPReadData[OC_EndAdd])) != CalCRC)
	{
		Catstring(ErrorMsg,_T("\n OC Data CheckSum is 0x%04X != 0x%04X"),(OTPReadData[OC_EndAdd-1]<<8) + (OTPReadData[OC_EndAdd]),CalCRC);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckMTKPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	////////////////////////////////PDAF Check//////////////////////////////////////////
	//No DriverIC
	for (int i = 0x0900;i < 0x0E68;i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("PDAF [0x%04X]: 0x%02X != 0xFF"),i,OTPReadData[i]);
			Catstring(ErrorMsg,_T("No DriverIC,PDAF data should be 0xFF!"));
			return FALSE;
		}
	}	

	return TRUE;

	// 	 long PdafChecksum =0;
	// 	if(OTPReadData[0x0776] != m_GetOTPCheckSpec.m_PDAFStep1Flag)
	// 	{
	// 		Catstring(ErrorMsg, "m_PDAFStep1Flag: %d != %d",m_GetOTPCheckSpec.m_PDAFStep1Flag,OTPReadData[0x0776]);
	// 		return FALSE;
	// 	}
	// 
	// 	if(OTPReadData[0x0968] != m_GetOTPCheckSpec.m_PDAFStep2Flag)
	// 	{
	// 		Catstring(ErrorMsg, "m_PDAFStep1Flag: %d != %d",m_GetOTPCheckSpec.m_PDAFStep2Flag,OTPReadData[0x0968]);
	// 		return FALSE;
	// 	}
	// 
	// 
	// 	for ( i=0x0777; i<0x0967;i++)
	// 	{
	// 		PdafChecksum += OTPReadData[i];
	// 	}
	// 	PdafChecksum = (PdafChecksum%255) +1;
	// 	if (OTPReadData[0x0967] != PdafChecksum)
	// 	{
	// 		Catstring(ErrorMsg, "PDAF Step1 checksum: %d != %d",PdafChecksum,OTPReadData[0x0967]);
	// 		return FALSE;
	// 	}	
	// 	
	// 	PdafChecksum=0;
	// 	for ( i=0x0969; i<0x0CF5;i++)
	// 	{
	// 		PdafChecksum += OTPReadData[i];
	// 	}
	// 	PdafChecksum = (PdafChecksum%255) +1;
	// 	if (OTPReadData[0x0CF5] != PdafChecksum)
	// 	{
	// 		Catstring(ErrorMsg, "PDAF Step2 checksum: %d != %d",PdafChecksum,OTPReadData[0x0CF5]);
	// 		return FALSE;
	// 	}
	// 	
	// 	BYTE PDAFCheckData[1404]={0};
	// 	for ( i=0;i<496;i++)
	// 	{
	// 		PDAFCheckData[i]=OTPReadData[i+0x0777];
	// 	}
	// 	for ( i=496;i<1404;i++)
	// 	{
	// 		PDAFCheckData[i]=OTPReadData[i+0x0969-496];
	// 	}
	// 	CString settingfilepath = "";
	// 	//打开这些PD函数需要打开头文件中的.h文件
	// //	settingfilepath.Format("setting_sample(3L8_2M8_4208_3120)_MirrorFlip_DLL_3.0.0_20160913.ini");
	// //	settingfilepath=GetModulePath()+settingfilepath;
	// 	int errornum = 0;
	// 	//	errornum =pd_cali_init(settingfilepath.GetBuffer(settingfilepath.GetLength()));
	// 	if (errornum!=0)
	// 	{
	// 		Catstring(ErrorMsg,"Setting init failed!");
	// 		return FALSE;
	// 	}
	// //	errornum=pd_cali_verify(PDAFCheckData);
	// 	if (errornum!=0)
	// 	{
	// 		Catstring(ErrorMsg,"EEPRom PDAF数据不正确!");
	// 		return FALSE;
	// 	}
	// 	return TRUE;
}

BOOL OtpCheckAPI::GetDataFromTxt(BYTE *OTPData,int Flag)
{
	FILE *fp = NULL;

	CString sfilename = _T("");
	CString sTmp      = _T("");
	CString sfileext  = _T("");
	CString sfilter   = _T("Ini File(*.txt)|*.txt");
	CString SensorINI = _T("");
	CFileFind ff;
	if (Flag==1)
	{
		SensorINI = GetModulePath() + _T("\\readbackvalue1.txt"); // 设定参数文档路径
	}
	else if (Flag==2)
	{
		SensorINI = GetModulePath() + _T("\\readbackvalue2.txt"); // 设定参数文档路径
	}

	sfilename.Format(SensorINI);

	fp = _wfopen(sfilename, _T("a"));
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeRead | CFile::typeText);  


	char temp[0x1B000] = {0};
	char temp1[20] = {0};
	USHORT tempdata = 0;
	MyFile.Read(temp,0x1B000);
	for (int k=0;k<m_OTPSize;k++)
	{		
		strncpy(temp1,temp+12*k+9,2); //13*k+10
		sscanf(temp1,"%02x",&tempdata);			
		OTPData[k] = tempdata;
	}
	MyFile.Close();
	return TRUE;
}
BOOL OtpCheckAPI::SaveOTPData(BYTE *OTPReadData,wchar_t *ErrorMsg,int Flag)
{
	CString path=_T("");
	wchar_t spath[258] = {0};
	path = GetModulePath();
	CString sTmp = _T("");

	/*********************************************************************/
	/*                           save ReadOTPData.bin                   */
	/*********************************************************************/

	if (Flag==1) //主摄保存数据1，副摄保存数据2
	{
		wsprintf(spath,_T("%s\\ReadOTPData1.bin"),path);
	}
	else if (Flag==2)
	{
		wsprintf(spath,_T("%s\\ReadOTPData2.bin"),path);
	}

	FILE *fp = _wfopen(spath,_T("wb"));
	if (fp == NULL)
	{
		Catstring(ErrorMsg, _T("\n 打开ReadOTPData.bin文件失败!"));
		return FALSE;
	}
	fwrite(OTPReadData,1,m_OTPSize,fp);
	fclose(fp);

	/*********************************************************************/
	/*                           save ReadOTPData.ini                   */
	/*********************************************************************/
	memset(spath,0,258);
	if (Flag==1) //主摄保存数据1，副摄保存数据2
	{
		wsprintf(spath,_T("%s\\ReadOTPData1.ini"),path);
	}
	else if (Flag==2)
	{
		wsprintf(spath,_T("%s\\ReadOTPData2.ini"),path);
	}

	fp = NULL;
	fp = _wfopen(spath,_T("a"));
	if (fp == NULL)
	{
		Catstring(ErrorMsg, _T("\n 打开ReadOTPData.ini文件失败!"));
		return FALSE;
	}
	fclose(fp);

	CStdioFile MyFile(spath, CFile::modeWrite | CFile::typeText);  
	Sleep(20);
	for (int k=0;k<m_OTPSize;k++)
	{
		sTmp.Format(_T("0x%04X\t0x%02X\n"),k,OTPReadData[k]);
		MyFile.WriteString(sTmp);

	}
	MyFile.Close();
	return TRUE;
}

int OtpCheckAPI::OTPcheckImp(wchar_t* ErrorMsg,BYTE *InputOTPData, int Flag,CString StationName)
{
	int i = 0;
	BYTE *OTPReadData = new BYTE[m_OTPSize];
	if (NULL == OTPReadData)
	{
		//m_pInterface->Ctrl_Addlog(CamID,_T("申请OTPReadData buffer失败!"),COLOR_RED,200);
		Catstring(ErrorMsg, _T("\n申请OTPReadData buffer失败!"));
		SAFE_DELETE_ARRAY(OTPReadData);
		return FALSE;
	}
	memset(OTPReadData,0,m_OTPSize);
	BOOL m_OTPCheckResult = FALSE;
	wchar_t szLog[128] = {0};
	/*********************************************************************/
	/*					获取及保存项目1Eeprom数据						 */
	/*********************************************************************/
	if (Flag == 1)
	{
        /*TESTSTATION.Format(MASTER_MODULE_NAME);
        STATIONCHECKITEM.Format(MASTER_MODULE_STATION_CHK_ITEM_NAME); 

		if (m_GetOTPCheckSpec.m_ProjectName1.Find(TESTSTATION) == -1)
		{
			SAFE_DELETE_ARRAY(OTPReadData);
			wsprintf(szLog, _T("\n 请调用对应机种的OTPcheck.dll!\n 仅适用于%s机种!"), TESTSTATION);
			Catstring(ErrorMsg, szLog);
			return FALSE;
		}*/

		if (m_GetOTPCheckSpec.m_OffLineCheck_En)
		{
			GetDataFromTxt(OTPReadData,1);
		}
		else
		{
			memcpy(OTPReadData,InputOTPData,m_OTPSize);
		}

		if (m_GetOTPCheckSpec.m_SaveOTPCheckData)
		{
			if (!SaveOTPData(OTPReadData,ErrorMsg,1))
			{
				wsprintf(szLog, _T("\n %s保存Eeprom数据失败!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}			
		}
		/*********************************************************************/
		/*					获取项目1测试站位信息							 */
		/*********************************************************************/
		if (((StationName).Find(_T("终检"))) != -1)//m_GetOTPCheckSpec.m_TestStation//修改根据点亮设定即界面选择的站名进行匹配相应的测试项 LYC 20180525
		{
			m_GetOTPCheckSpec.m_AFCheck_En		 = 0;
			m_GetOTPCheckSpec.m_SFRCheck_En= 1;
			m_GetOTPCheckSpec.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec.m_OCCheck_En= 1;
			m_GetOTPCheckSpec.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec.m_CrossTalkCheck_En = 1;
			m_GetOTPCheckSpec.m_PDAFCheck_En	= 0;
			m_GetOTPCheckSpec.m_AECCheck_En		 = 0;
			m_GetOTPCheckSpec.m_DualCamCheck_En	 = 0;
			Catstring(ErrorMsg,_T("***测试终检站位数据***\n"),m_GetOTPCheckSpec.m_SFRCheck_En);
		}
		else if (((StationName).Find(_T("标定"))) != -1)
		{
			m_GetOTPCheckSpec.m_AFCheck_En		 = 0;
			m_GetOTPCheckSpec.m_SFRCheck_En= 1;
			m_GetOTPCheckSpec.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec.m_OCCheck_En= 1;
			m_GetOTPCheckSpec.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec.m_PDAFCheck_En	 = 0;
			m_GetOTPCheckSpec.m_AECCheck_En		 = 0;
			m_GetOTPCheckSpec.m_DualCamCheck_En	 = 0;
			Catstring(ErrorMsg,_T("***测试标定站位数据***\n"),m_GetOTPCheckSpec.m_SFRCheck_En);
		}
		else if (((StationName).Find(_T("AEC"))) != -1)
		{
			m_GetOTPCheckSpec.m_AFCheck_En		 = 0;
			m_GetOTPCheckSpec.m_SFRCheck_En= 1;
			m_GetOTPCheckSpec.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec.m_OCCheck_En= 1;
			m_GetOTPCheckSpec.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec.m_PDAFCheck_En	 = 0;
			m_GetOTPCheckSpec.m_AECCheck_En		 = 0;
			Catstring(ErrorMsg,_T("***测试AEC站位数据***\n"),m_GetOTPCheckSpec.m_SFRCheck_En);
		}
		else if (((StationName).Find(_T("PDAF"))) != -1)
		{
			m_GetOTPCheckSpec.m_AFCheck_En		 = 0;
			m_GetOTPCheckSpec.m_SFRCheck_En= 1;
			m_GetOTPCheckSpec.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec.m_OCCheck_En		 = 1;
			m_GetOTPCheckSpec.m_PDAFCheck_En	 = 0;
			Catstring(ErrorMsg,_T("***测试PDAF站位数据***\n"),m_GetOTPCheckSpec.m_SFRCheck_En);
		}
		else if (((StationName).Find(_T("OTP"))) != -1)
		{
			m_GetOTPCheckSpec.m_AFCheck_En = 0;
			m_GetOTPCheckSpec.m_SFRCheck_En = 1;
			m_GetOTPCheckSpec.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec.m_OCCheck_En = 1;
			m_GetOTPCheckSpec.m_AWBLSCCheck_En = 1;
			m_GetOTPCheckSpec.m_CrossTalkCheck_En = 1;
			Catstring(ErrorMsg,_T("***测试OTP站位数据***\n"),m_GetOTPCheckSpec.m_SFRCheck_En);
		}
		else if (((StationName).Find(_T("SFR"))) != -1)
		{
			m_GetOTPCheckSpec.m_AFCheck_En  = 0;
			m_GetOTPCheckSpec.m_SFRCheck_En = 1;
			Catstring(ErrorMsg,_T("***测试SFR站位数据***\n"),m_GetOTPCheckSpec.m_SFRCheck_En);
		}
		else
		{
			SAFE_DELETE_ARRAY(OTPReadData);
			wsprintf(szLog, _T("\n 无%s试站位，%s站位名填写错误!"), StationName, TESTSTATION);
			Catstring(ErrorMsg, szLog);
			return FALSE;
		}
		wchar_t DebugInfo_en[5120]=_T("");
		Catstring(DebugInfo_en, _T("AFCheck_En = %d\n"),m_GetOTPCheckSpec.m_AFCheck_En);
		Catstring(DebugInfo_en, _T("SFRCheck_En = %d\n"),m_GetOTPCheckSpec.m_SFRCheck_En);
		Catstring(DebugInfo_en, _T("ModuleInfoCheck_En = %d\n"),m_GetOTPCheckSpec.m_ModuleInfoCheck_En);
		Catstring(DebugInfo_en, _T("OCCheck_En = %d\n"),m_GetOTPCheckSpec.m_OCCheck_En);
		Catstring(DebugInfo_en, _T("AWBLSCCheck_En = %d\n"),m_GetOTPCheckSpec.m_AWBLSCCheck_En);
		Catstring(DebugInfo_en, _T("CrossTalkCheck_En = %d\n"),m_GetOTPCheckSpec.m_CrossTalkCheck_En);
		//Catstring(DebugInfo_en, _T("PDAFCheck_En = %d\n"),m_GetOTPCheckSpec.m_PDAFCheck_En);
		//Catstring(DebugInfo_en, _T("AECCheck_En = %d\n"),m_GetOTPCheckSpec.m_AECCheck_En);
		//Catstring(DebugInfo_en, _T("DualCamCheck_En = %d\n"),m_GetOTPCheckSpec.m_DualCamCheck_En);
		//Catstring(DebugInfo_en, _T("OISCheck_En = %d\n"),m_GetOTPCheckSpec.m_OISCheck_En);	
		//Catstring(DebugInfo_en, _T("AFSyncCheck_En = %d\n"),m_GetOTPCheckSpec.m_AFSyncCheck_En);
		//m_pInterface->Ctrl_Addlog(CamID,DebugInfo_en,COLOR_BLUE,220);
		/*********************************************************************/
		/*							Check AF				                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec.m_AFCheck_En == 1)
		{
			m_OTPCheckResult = CheckAFCode_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s AF烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}
		}
		if (m_GetOTPCheckSpec.m_SFRCheck_En == 1)
		{
			m_OTPCheckResult = CheckSFRMacro_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s Macro SFR 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckSFRMid_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s Middle SFR 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckSFRInfinity_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s Infinity SFR 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}
			int SFRCRC = GetCRC16(&OTPReadData[SFR_StartAdd],SFR_EndAdd - SFR_StartAdd -1); 
			if (((OTPReadData[SFR_EndAdd-1]<<8) + (OTPReadData[SFR_EndAdd])) != SFRCRC)
			{
				Catstring(ErrorMsg,_T("\nSFR Data CheckSum is 0x%04X != 0x%04X"),(OTPReadData[SFR_EndAdd-1]<<8) + (OTPReadData[SFR_EndAdd]),SFRCRC);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*              Check module Information			                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec.m_ModuleInfoCheck_En == 1)
		{
			m_OTPCheckResult = CheckModuleInfo_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s ModuleInfo烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*              Check OC							                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec.m_OCCheck_En == 1)
		{
			m_OTPCheckResult = CheckOC_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s OC烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*                       AWB & LSC check				             */
		/*********************************************************************/
		if (m_GetOTPCheckSpec.m_AWBLSCCheck_En == 1)
		{
			m_OTPCheckResult = CheckAWB_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s AWB烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}

			
			m_OTPCheckResult = CheckQualcommLSC_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s LSC烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*					CrossTalk check							         */
		/*********************************************************************/	
		if (m_GetOTPCheckSpec.m_CrossTalkCheck_En == 1)
		{
			m_OTPCheckResult = CheckCrossTalk_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s CrossTalk烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}


#if 0
		/*********************************************************************/
		/*						SPC check							         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec.m_SPCCheck_En == 1)
		{
			m_OTPCheckResult = CheckQualcommSPC_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog,_T("\n %s SPC烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}
#endif

		/*********************************************************************/
		/*						PDAF check							         */
		/*********************************************************************/	
		if (m_GetOTPCheckSpec.m_PDAFCheck_En == 1)
		{
			m_OTPCheckResult = CheckQualcommPDAF_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s PDAF烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}
			
		/*********************************************************************/
		/*						AEC check							         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec.m_AECCheck_En == 1)
		{
			m_OTPCheckResult = CheckAEC_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s AEC烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*						OIS check							         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec.m_OISCheck_En == 1)
		{
			m_OTPCheckResult = CheckOIS_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s OIS烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*						DualCam check						         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec.m_DualCamCheck_En == 1)
		{
			m_OTPCheckResult = CheckDualCamera_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s DualCam烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}

			m_OTPCheckResult = CheckModuleInfo_Project3(OTPReadData,ErrorMsg);//副摄部分拷贝过来的数据--Added by LiHai--20180118
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s mono ModuleInfo 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckAFCode_Project3(OTPReadData,ErrorMsg);//副摄部分拷贝过来的数据--Added by LiHai--20180118
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s mono AF 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckAWB_Project2(OTPReadData + MAP_OFFSET, ErrorMsg);//副摄部分拷贝过来的数据,借用副摄的AWBCheck函数，所以需要偏移0x0600--Added by LiHai--20180118
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s mono AWB 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckQualcommLSC_Project3(OTPReadData,ErrorMsg);//副摄部分拷贝过来的数据--Added by LiHai--20180118
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s mono LSC 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckSFR7M_Project3(OTPReadData,ErrorMsg);//副摄部分拷贝过来的数据--Added by LiHai--20180118
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s mono 7M SFR 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckSFR8CM_Project3(OTPReadData,ErrorMsg);//副摄部分拷贝过来的数据--Added by LiHai--20180118
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s mono 8CM SFR 烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			/*********************************************************************/
			/*						SPC check							         */
			/*********************************************************************/		
			m_OTPCheckResult = CheckSonySPC_Project2(OTPReadData + MAP_OFFSET,ErrorMsg);//副摄部分拷贝过来的数据,借用副摄的AWBCheck函数，所以需要偏移0x0600--Added by LiHai--20180118
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog,_T("\n %s Mono SPC烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			/*********************************************************************/
			/*						PDAF check							         */
			/*********************************************************************/		
			m_OTPCheckResult = CheckSonyPDAF_Project2(OTPReadData + MAP_OFFSET,ErrorMsg);//副摄部分拷贝过来的数据,借用副摄的AWBCheck函数，所以需要偏移0x0600--Added by LiHai--20180118
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s Mono PDAF烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			/*********************************************************************/
			/*						AF Sync check							         */
			/*********************************************************************/		
			m_OTPCheckResult = CheckAFSync_Project2(OTPReadData + MAP_OFFSET, ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s AFSync烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}

			//m_pInterface->Ctrl_Addlog(CamID,_T("MONO Check Pass!"),COLOR_BLUE,200);
		}

		/*********************************************************************/
		/*						AF Sync check							         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec.m_AFSyncCheck_En == 1)
		{
			m_OTPCheckResult = CheckAFSync_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s AFSync烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}
	}
	else if (Flag == 2)
	{
		/*TESTSTATION_1.Format(SLAVER_MODULE_NAME);
		STATIONCHECKITEM.Format(SLAVER_MODULE_STATION_CHK_ITEM_NAME); 

		if (m_GetOTPCheckSpec1.m_ProjectName1.Find(TESTSTATION_1) == -1)
		{
			SAFE_DELETE_ARRAY(OTPReadData);
			wsprintf(szLog, _T("\n 请调用对应机种的OTPcheck.dll!\n 仅适用于%s机种!"), TESTSTATION_1);
			Catstring(ErrorMsg, szLog);
			return FALSE;
		}*/

		if (m_GetOTPCheckSpec1.m_OffLineCheck_En)
		{
			GetDataFromTxt(OTPReadData, Flag);
		}
		else
		{
			memcpy(OTPReadData, InputOTPData + MAP_OFFSET, m_OTPSize - MAP_OFFSET);	//偏移0x0600的原因是副摄MAP表中数据是从EEPROM中的0x0600开始的--Added by LiHai--20180118
		}

		if (m_GetOTPCheckSpec1.m_SaveOTPCheckData)
		{
			if (!SaveOTPData(OTPReadData, ErrorMsg, Flag))
			{
				wsprintf(szLog, _T("\n %s保存Eeprom数据失败!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}			
		}
#if 0
		/*********************************************************************/
		/*					获取项目1测试站位信息							 */
		/*********************************************************************/
		if (((m_GetOTPCheckSpec1.m_TestStation).Find(_T("OTP"))) != -1)
		{
			m_GetOTPCheckSpec1.m_AFCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec1.m_SegmentInfoCheck_En= 1;
			m_GetOTPCheckSpec1.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec1.m_SPCCheck_En		 = 1;
		}
		else if (((m_GetOTPCheckSpec1.m_TestStation).Find(_T("PDAF"))) != -1)
		{
			m_GetOTPCheckSpec1.m_AFCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec1.m_SegmentInfoCheck_En= 1;
			m_GetOTPCheckSpec1.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec1.m_SPCCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_PDAFCheck_En		 = 1;
		}
		else if (((m_GetOTPCheckSpec1.m_TestStation).Find(_T("AF"))) != -1)
		{
			m_GetOTPCheckSpec1.m_AFCheck_En		 = 1;
		}
		else if (((m_GetOTPCheckSpec1.m_TestStation).Find(_T("终检"))) != -1)
		{
			m_GetOTPCheckSpec1.m_AFCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec1.m_SegmentInfoCheck_En= 1;
			m_GetOTPCheckSpec1.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec1.m_SPCCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_PDAFCheck_En		 = 1;
		}
		else
		{
			SAFE_DELETE_ARRAY(OTPReadData);
			wsprintf(szLog, _T("\n 无%s试站位，%s站位名填写错误!"), m_GetOTPCheckSpec1.m_TestStation, TESTSTATION);
			Catstring(ErrorMsg, szLog);
			return FALSE;
		}
#endif
		wchar_t DebugInfo_en[5120]=_T("");
		Catstring(DebugInfo_en, _T("AFCheck_En = %d\n"),m_GetOTPCheckSpec1.m_AFCheck_En);
		Catstring(DebugInfo_en, _T("ModuleInfoCheck_En = %d\n"),m_GetOTPCheckSpec1.m_ModuleInfoCheck_En);
		Catstring(DebugInfo_en, _T("AWBLSCCheck_En = %d\n"),m_GetOTPCheckSpec1.m_AWBLSCCheck_En);
		Catstring(DebugInfo_en, _T("PDAFCheck_En = %d\n"),m_GetOTPCheckSpec1.m_PDAFCheck_En);
		Catstring(DebugInfo_en, _T("AECCheck_En = %d\n"),m_GetOTPCheckSpec1.m_AECCheck_En);
		Catstring(DebugInfo_en, _T("OISCheck_En = %d\n"),m_GetOTPCheckSpec1.m_OISCheck_En);
		Catstring(DebugInfo_en, _T("DualCamCheck_En = %d\n"),m_GetOTPCheckSpec1.m_DualCamCheck_En);
		Catstring(DebugInfo_en, _T("AFSyncCheck_En = %d\n"),m_GetOTPCheckSpec1.m_AFSyncCheck_En);
		//m_pInterface->Ctrl_Addlog(CamID,DebugInfo_en,COLOR_BLUE,220);
		/*********************************************************************/
		/*							Check AF				                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec1.m_AFCheck_En == 1)
		{
			m_OTPCheckResult = CheckAFCode_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s AF烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckSFR7M_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s 7M SFR 烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}

			m_OTPCheckResult = CheckSFR8CM_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s 8CM SFR 烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*              Check module Information			                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec1.m_ModuleInfoCheck_En == 1)
		{
			m_OTPCheckResult = CheckModuleInfo_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s ModuleInfo烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}
#if 0
		/*********************************************************************/
		/*              Check Segment Information			                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec1.m_SegmentInfoCheck_En == 1)
		{
			m_OTPCheckResult = CheckSegmentInfo_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s SegmentInfo烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}
		}
#endif
		/*********************************************************************/
		/*                       AWB & LSC check				             */
		/*********************************************************************/
		if (m_GetOTPCheckSpec1.m_AWBLSCCheck_En == 1)
		{
			m_OTPCheckResult = CheckAWB_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s AWB烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}

			m_OTPCheckResult = CheckQualcommLSC_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s LSC烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
			/*int TotalCheckSum = 0;
			for (i = 0; i < 0x05AB; i++)
			{
				TotalCheckSum = TotalCheckSum + OTPReadData[i];
			}
			if ((OTPReadData[0x05AB] != ((TotalCheckSum >> 8) & 0xff)) || (OTPReadData[0x05AC] != (TotalCheckSum  & 0xff)))
			{
				Catstring(ErrorMsg,_T("\n Total CheckSum is 0x%04X 0x%04X != 0x%04X"),OTPReadData[0x05AB],OTPReadData[0x05AC],TotalCheckSum);
				return FALSE;
			}*/
		}

		/*********************************************************************/
		/*						SPC check							         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec1.m_SPCCheck_En == 1)
		{
			m_OTPCheckResult = CheckSonySPC_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog,_T("\n %s SPC烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*						PDAF check							         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec1.m_PDAFCheck_En == 1)
		{
			m_OTPCheckResult = CheckSonyPDAF_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s PDAF烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}

#if 0
		/*********************************************************************/
		/*						AEC check							         */
		/*********************************************************************/		
		if (GetOTPCheckSpec1.m_AECCheck_En == 1)
		{
			m_OTPCheckResult = CheckAEC_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				SAFE_DELETE_ARRAY(OTPReadData);
				wsprintf(szLog, _T("\n %s AEC烧录错误!"), TESTSTATION_1);
				Catstring(ErrorMsg, szLog);
				return FALSE;
			}
		}
#endif

		/*********************************************************************/
		/*						AF Sync check							         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec1.m_AFSyncCheck_En == 1)
		{
			m_OTPCheckResult = CheckAFSync_Project2(OTPReadData, ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s AFSync烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}
	}
	
	SAFE_DELETE_ARRAY(OTPReadData);
	return TRUE;
}