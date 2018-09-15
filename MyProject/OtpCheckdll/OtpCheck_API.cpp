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

int OtpCheckAPI::Testing( wchar_t *ErrorMsg,BYTE *InputOTPData,int iInputOtpSize, int Flag,CString StationName)
{
	if (iInputOtpSize <= 0)
	{
		Catstring(ErrorMsg, _T("输入buffer size小于等于0!"), iInputOtpSize);
		return -1;
	}
	m_OTPSize = iInputOtpSize;

	CString TESTSTATION = _T("");
	CString strFilePath = GetModulePath()+ _T("SetOTPCheckSpec.ini");
	if (Flag == 1)
	{
		TESTSTATION.Format(_T("OHS0657_Main"));
		STATIONCHECKITEM.Format(_T("OHS0657OTPCheck_Main")); 
		OnInitialSpec_Project1( strFilePath, m_GetOTPCheckSpec);

		if (m_GetOTPCheckSpec.m_ProjectName1.Find(TESTSTATION) == -1)
		{
			Catstring(ErrorMsg, _T("项目名错误!"));
			return -2;
		}
	}
	else
	{
		TESTSTATION.Format(_T("OHS0657_Slave"));
		STATIONCHECKITEM.Format(_T("OHS0657OTPCheck_Slave")); 
		OnInitialSpec_Project2( strFilePath, m_GetOTPCheckSpec1);

		if (m_GetOTPCheckSpec1.m_ProjectName1.Find(TESTSTATION) == -1)
		{
			Catstring(ErrorMsg, _T("项目名错误!"));
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
	GetPrivateProfileString(strProjectName1,_T("TestStation"), _T("Image"), szTmp, iBufSize, strFilePath);	  //获取机种名
	m_GetOTPCheckSpec.m_TestStation = szTmp;

	int nPos = 0;
	nPos = (m_GetOTPCheckSpec.m_TestStation).Find('/');
	m_GetOTPCheckSpec.m_TestStation = (nPos < 0)?m_GetOTPCheckSpec.m_TestStation:m_GetOTPCheckSpec.m_TestStation.Left(nPos-1);

	m_GetOTPCheckSpec.m_Project1Check_En		=	GetPrivateProfileInt(strProjectName1,_T("Project1Check_En"),	0, strFilePath);	
	m_GetOTPCheckSpec.m_OTPCheckAll_En		=	GetPrivateProfileInt(strProjectName1,_T("OTPCheckAll_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_ModuleInfoCheck_En	=	GetPrivateProfileInt(strProjectName1,_T("ModuleInfoCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec.m_AWBLSCCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("AWBLSCCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_AFCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("AFCheck_En"),			0, strFilePath);
	m_GetOTPCheckSpec.m_PDAFCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("PDAFCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_HDCCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("HDCCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_OISCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("OISCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_OCCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("OCCheck_En"),			0, strFilePath);
	m_GetOTPCheckSpec.m_SFRCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("SFRCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_AECCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("AECCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_SPCCheck_En			=	GetPrivateProfileInt(strProjectName1,_T("SPCCheck_En"),		0, strFilePath);
	m_GetOTPCheckSpec.m_CrossTalkCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("CrossTalkCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec.m_ArcsoftCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("ArcsoftCheck_En"),	0, strFilePath);	
	m_GetOTPCheckSpec.m_SegmentInfoCheck_En	=	GetPrivateProfileInt(strProjectName1,_T("SegmentInfoCheck_En"),0, strFilePath);
	m_GetOTPCheckSpec.m_DualCamCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("DualCamCheck_En"),	0, strFilePath);
	m_GetOTPCheckSpec.m_OthersCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("OthersCheck_En"),		0, strFilePath);	
	m_GetOTPCheckSpec.m_LRCCheck_En			=   GetPrivateProfileInt(strProjectName1,_T("LRCCheck_En"),        0, strFilePath);											 
	m_GetOTPCheckSpec.m_OffLineCheck_En		=	GetPrivateProfileInt(strProjectName1,_T("OffLineCheck_En"),	0, strFilePath);
    m_GetOTPCheckSpec.m_OVPDAFStep_En         =   GetPrivateProfileInt(strProjectName1,_T("OVPDAFStep_En"),      0, strFilePath);
															  
	m_GetOTPCheckSpec.m_SaveOTPCheckData		=	GetPrivateProfileInt(strProjectName1,_T("SaveOTPCheckData"),	1, strFilePath);
	m_GetOTPCheckSpec.m_LSCCurveCheck			=	GetPrivateProfileInt(strProjectName1,_T("LSCCurveCheck"),		0, strFilePath);
	m_GetOTPCheckSpec.m_EqualCountSpec		=	GetPrivateProfileInt(strProjectName1,_T("EqualCountSpec"),		0, strFilePath);
	m_GetOTPCheckSpec.m_HisiMasterFlag		=	GetPrivateProfileInt(strProjectName1,_T("HisiMasterFlag"),		0, strFilePath);
															 
	m_GetOTPCheckSpec.m_ModulInfoFlag			=	GetPrivateProfileInt(strProjectName1,_T("ModulInfoFlag"),		0, strFilePath);			
	m_GetOTPCheckSpec.m_ModuleID				=	GetPrivateProfileInt(strProjectName1,_T("ModuleID"),			0, strFilePath);      			
	m_GetOTPCheckSpec.m_LensID				=	GetPrivateProfileInt(strProjectName1,_T("LensID"),				0, strFilePath);	      			
	m_GetOTPCheckSpec.m_VCMID					=	GetPrivateProfileInt(strProjectName1,_T("VCMID"),				0, strFilePath);							
	m_GetOTPCheckSpec.m_DriverIcID			=	GetPrivateProfileInt(strProjectName1,_T("DriverIcID"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSCToolVersion		=	GetPrivateProfileInt(strProjectName1,_T("LSCToolVersion"),		0, strFilePath);			
	m_GetOTPCheckSpec.m_PDAFToolVersion		=	GetPrivateProfileInt(strProjectName1,_T("PDAFToolVersion"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_ColorTemperatureID	=	GetPrivateProfileInt(strProjectName1,_T("ColorTemperatureID"),	0, strFilePath);	
	m_GetOTPCheckSpec.m_FirstPixel			=	GetPrivateProfileInt(strProjectName1,_T("FirstPixel"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MirrorFlip			=	GetPrivateProfileInt(strProjectName1,_T("MirrorFlip"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_SensorNameID			=	GetPrivateProfileInt(strProjectName1,_T("SensorNameID"),		0, strFilePath);				
	m_GetOTPCheckSpec.m_SupplierCode			=	GetPrivateProfileInt(strProjectName1,_T("SupplierCode"),		0, strFilePath);				
	m_GetOTPCheckSpec.m_ModuleCode			=	GetPrivateProfileInt(strProjectName1,_T("ModuleCode"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_Year					=	GetPrivateProfileInt(strProjectName1,_T("Year"),				0, strFilePath);								
	m_GetOTPCheckSpec.m_Month					=	GetPrivateProfileInt(strProjectName1,_T("Month"),				0, strFilePath);						
	m_GetOTPCheckSpec.m_Day					=	GetPrivateProfileInt(strProjectName1,_T("Day"),				0, strFilePath);							
	m_GetOTPCheckSpec.m_LightSourceFlag		=	GetPrivateProfileInt(strProjectName1,_T("LightSourceFlag"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_AForFF_Flag			=	GetPrivateProfileInt(strProjectName1,_T("AForFF_Flag"),		0, strFilePath);				
	m_GetOTPCheckSpec.m_IRBGID				=	GetPrivateProfileInt(strProjectName1,_T("IRBGID"),				0, strFilePath); 
	m_GetOTPCheckSpec.m_PlatformID			=	GetPrivateProfileInt(strProjectName1,_T("PlatformID"),			0, strFilePath); 
	m_GetOTPCheckSpec.m_DualCameraFlag		=	GetPrivateProfileInt(strProjectName1,_T("DualCameraFlag"),		0, strFilePath);
	m_GetOTPCheckSpec.m_FnumberID				=	GetPrivateProfileInt(strProjectName1,_T("FnumberID"),			0, strFilePath);
	m_GetOTPCheckSpec.m_FactoryID				=	GetPrivateProfileInt(strProjectName1,_T("FactoryID"),			0, strFilePath);
	m_GetOTPCheckSpec.m_CalibrationVersion	=	GetPrivateProfileInt(strProjectName1,_T("CalibrationVersion"),	0, strFilePath);
	m_GetOTPCheckSpec.m_OISID					=	GetPrivateProfileInt(strProjectName1,_T("OISID"),				0, strFilePath);	
	m_GetOTPCheckSpec.m_ModuleVersion			=	GetPrivateProfileInt(strProjectName1,_T("ModuleVersion"),		0, strFilePath);		
	m_GetOTPCheckSpec.m_SoftVersion			=	GetPrivateProfileInt(strProjectName1,_T("SoftVersion"),		0, strFilePath);			
	m_GetOTPCheckSpec.m_AWBLSCStation			=	GetPrivateProfileInt(strProjectName1,_T("AWBLSCStation"),		0, strFilePath);		
//	m_GetOTPCheckSpec.m_PDAFStation			=	GetPrivateProfileInt(strProjectName1,_T("PDAFStation"),		0, strFilePath);			
//	m_GetOTPCheckSpec.m_AFStation				=	GetPrivateProfileInt(strProjectName1,_T("AFStation"),			0, strFilePath);				
	m_GetOTPCheckSpec.m_EEPROMID				=	GetPrivateProfileInt(strProjectName1,_T("EEPROMID"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_ProgramName			=	GetPrivateProfileInt(strProjectName1,_T("ProgramName"),		0, strFilePath);
	m_GetOTPCheckSpec.m_SensorSlaveID			=	GetPrivateProfileInt(strProjectName1,_T("SensorSlaveID"),		0, strFilePath);
	m_GetOTPCheckSpec.m_EEPROMSlaveID			=	GetPrivateProfileInt(strProjectName1,_T("EEPROMSlaveID"),		0, strFilePath);
	m_GetOTPCheckSpec.m_VCMSlaveID			=	GetPrivateProfileInt(strProjectName1,_T("VCMSlaveID"),			0, strFilePath);
	m_GetOTPCheckSpec.m_SensorStatus			=	GetPrivateProfileInt(strProjectName1,_T("SensorStatus"),		0, strFilePath);
	m_GetOTPCheckSpec.m_CheckSN_En			=	GetPrivateProfileInt(strProjectName1,_T("CheckSN_En"),			0, strFilePath);
    m_GetOTPCheckSpec.m_OVPDAFStep            =   GetPrivateProfileInt(strProjectName1,_T("OVPDAFStep"),         0, strFilePath);
															  
	m_GetOTPCheckSpec.m_VendorID				=	GetPrivateProfileInt(strProjectName1,_T("VendorID"),			0, strFilePath);
	m_GetOTPCheckSpec.m_OISFWVER				=	GetPrivateProfileInt(strProjectName1,_T("OISFWVER"),			0, strFilePath);
	m_GetOTPCheckSpec.m_ProductID				=	GetPrivateProfileInt(strProjectName1,_T("ProductID"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MapVersionID			=	GetPrivateProfileInt(strProjectName1,_T("MapVersionID"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MapHistoryID			=	GetPrivateProfileInt(strProjectName1,_T("MapHistoryID"),		0, strFilePath);
	m_GetOTPCheckSpec.m_EepromorOTP			=	GetPrivateProfileInt(strProjectName1,_T("EepromorOTP"),		0, strFilePath);
															  
	m_GetOTPCheckSpec.m_SegmentInfoFlag		=	GetPrivateProfileInt(strProjectName1,_T("SegmentInfoFlag"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_SegmentCount			=	GetPrivateProfileInt(strProjectName1,_T("SegmentCount"),		0, strFilePath);			
	m_GetOTPCheckSpec.m_SegmentAFOffset		=	GetPrivateProfileInt(strProjectName1,_T("SegmentAFOffset"),	0, strFilePath);				
	m_GetOTPCheckSpec.m_SegmentAWBOffset		=	GetPrivateProfileInt(strProjectName1,_T("SegmentAWBOffset"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_SegmentLSCOffset		=	GetPrivateProfileInt(strProjectName1,_T("SegmentLSCOffset"),	0, strFilePath);
	m_GetOTPCheckSpec.m_SegmentAECOffset		=	GetPrivateProfileInt(strProjectName1,_T("SegmentAECOffset"),	0, strFilePath);
	m_GetOTPCheckSpec.m_SegmentPDAFOffset		=	GetPrivateProfileInt(strProjectName1,_T("SegmentPDAFOffset"),	0, strFilePath);
	m_GetOTPCheckSpec.m_SegmentOISOffset		=	GetPrivateProfileInt(strProjectName1,_T("SegmentOISOffset"),	0, strFilePath);
	m_GetOTPCheckSpec.m_SegmentDualCamOffset	=	GetPrivateProfileInt(strProjectName1,_T("SegmentDualCamOffset"),0, strFilePath);
	m_GetOTPCheckSpec.m_SegmentChecksumOffset	=	GetPrivateProfileInt(strProjectName1,_T("SegmentChecksumOffset"),0, strFilePath);
															 
	m_GetOTPCheckSpec.m_SegmentAFTagID		=	GetPrivateProfileInt(strProjectName1,_T("SegmentAFTagID"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_AFTypeNumber			=	GetPrivateProfileInt(strProjectName1,_T("AFTypeNumber"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxInfinityCode		=	GetPrivateProfileInt(strProjectName1,_T("MaxInfinityCode"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinInfinityCode		=	GetPrivateProfileInt(strProjectName1,_T("MinInfinityCode"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxMacroCode			=	GetPrivateProfileInt(strProjectName1,_T("MaxMacroCode"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinMacroCode			=	GetPrivateProfileInt(strProjectName1,_T("MinMacroCode"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxPostureDifA		=	GetPrivateProfileInt(strProjectName1,_T("MaxPostureDifA"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinPostureDifA		=	GetPrivateProfileInt(strProjectName1,_T("MinPostureDifA"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxPostureDifB		=	GetPrivateProfileInt(strProjectName1,_T("MaxPostureDifB"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinPostureDifB		=	GetPrivateProfileInt(strProjectName1,_T("MinPostureDifB"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxPostureDifC		=	GetPrivateProfileInt(strProjectName1,_T("MaxPostureDifC"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinPostureDifC		=	GetPrivateProfileInt(strProjectName1,_T("MinPostureDifC"),		0, strFilePath);
															  
	m_GetOTPCheckSpec.m_SegmentAWBTagID		=	GetPrivateProfileInt(strProjectName1,_T("SegmentAWBTagID"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_AWBTypeNumber			=	GetPrivateProfileInt(strProjectName1,_T("AWBTypeNumber"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_AWBMirror				=	GetPrivateProfileInt(strProjectName1,_T("AWBMirror"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_AWBFilp				=	GetPrivateProfileInt(strProjectName1,_T("AWBFilp"),			0, strFilePath);
															  
	m_GetOTPCheckSpec.m_SegmentLSCTagID		=	GetPrivateProfileInt(strProjectName1,_T("SegmentLSCTagID"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_LSCTypeNumber			=	GetPrivateProfileInt(strProjectName1,_T("LSCTypeNumber"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_LSCMirror				=	GetPrivateProfileInt(strProjectName1,_T("LSCMirror"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_LSCFilp				=	GetPrivateProfileInt(strProjectName1,_T("LSCFilp"),			0, strFilePath);
	m_GetOTPCheckSpec.m_LSCVersionNumber		=	GetPrivateProfileInt(strProjectName1,_T("LSCVersionNumber"),	0, strFilePath);
    m_GetOTPCheckSpec.m_LSCHighByte   		=	GetPrivateProfileInt(strProjectName1,_T("LSCHighByte"),    	0, strFilePath);
    m_GetOTPCheckSpec.m_LSCLowByte    		=	GetPrivateProfileInt(strProjectName1,_T("LSCLowByte"),     	0, strFilePath);

															  
	m_GetOTPCheckSpec.m_SegmentAECTagID		=	GetPrivateProfileInt(strProjectName1,_T("SegmentAECTagID"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_AECFlag				=	GetPrivateProfileInt(strProjectName1,_T("AECFlag"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_AECVersionNumber		=	GetPrivateProfileInt(strProjectName1,_T("AECVersionNumber"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxBrightnessRatio	=	GetPrivateProfileInt(strProjectName1,_T("MaxBrightnessRatio"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinBrightnessRatio	=	GetPrivateProfileInt(strProjectName1,_T("MinBrightnessRatio"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MainGain				=	GetPrivateProfileInt(strProjectName1,_T("MainGain"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MainExposureTime		=	GetPrivateProfileInt(strProjectName1,_T("MainExposureTime"),	0, strFilePath);										
	m_GetOTPCheckSpec.m_SubGain				=	GetPrivateProfileInt(strProjectName1,_T("SubGain"),			0, strFilePath);
	m_GetOTPCheckSpec.m_SubExposureTime		=	GetPrivateProfileInt(strProjectName1,_T("SubExposureTime"),	0, strFilePath);
															  
	m_GetOTPCheckSpec.m_SegmentPDAFTagID		=	GetPrivateProfileInt(strProjectName1,_T("SegmentPDAFTagID"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_PDAFTypeNumber		=	GetPrivateProfileInt(strProjectName1,_T("PDAFTypeNumber"),		0, strFilePath);
															  
	m_GetOTPCheckSpec.m_SegmentDualCamTagID	=	GetPrivateProfileInt(strProjectName1,_T("SegmentDualCamTagID"),0, strFilePath);						
	m_GetOTPCheckSpec.m_DualCamFlag			=	GetPrivateProfileInt(strProjectName1,_T("DualCamFlag"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_DualCamVersionNumber	=	GetPrivateProfileInt(strProjectName1,_T("DualCamVersionNumber"),0, strFilePath);						
	m_GetOTPCheckSpec.m_SegmentCheckSumTagID	=	GetPrivateProfileInt(strProjectName1,_T("SegmentCheckSumTagID"),0, strFilePath);
	m_GetOTPCheckSpec.m_AWBFlag				=	GetPrivateProfileInt(strProjectName1,_T("AWBFlag"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxRGain				=	GetPrivateProfileInt(strProjectName1,_T("MaxRGain"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_MinRGain				=	GetPrivateProfileInt(strProjectName1,_T("MinRGain"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxBGain				=	GetPrivateProfileInt(strProjectName1,_T("MaxBGain"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_MinBGain				=	GetPrivateProfileInt(strProjectName1,_T("MinBGain"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxGGGain				=	GetPrivateProfileInt(strProjectName1,_T("MaxGGGain"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGGGain				=	GetPrivateProfileInt(strProjectName1,_T("MinGGGain"),			0, strFilePath);										
	m_GetOTPCheckSpec.m_MaxRBGain				=	GetPrivateProfileInt(strProjectName1,_T("MaxRBGain"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinRBGain				=	GetPrivateProfileInt(strProjectName1,_T("MinRBGain"),			0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_RG			=	GetPrivateProfileInt(strProjectName1,_T("Typical_RG"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_Typical_BG			=	GetPrivateProfileInt(strProjectName1,_T("Typical_BG"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_Typical_GG			=	GetPrivateProfileInt(strProjectName1,_T("Typical_GG"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MaxRValue				=	GetPrivateProfileInt(strProjectName1,_T("MaxRValue"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MinRValue				=	GetPrivateProfileInt(strProjectName1,_T("MinRValue"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MaxBValue				=	GetPrivateProfileInt(strProjectName1,_T("MaxBValue"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MinBValue				=	GetPrivateProfileInt(strProjectName1,_T("MinBValue"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MaxGrValue			=	GetPrivateProfileInt(strProjectName1,_T("MaxGrValue"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGrValue			=	GetPrivateProfileInt(strProjectName1,_T("MinGrValue"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxGbValue			=	GetPrivateProfileInt(strProjectName1,_T("MaxGbValue"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGbValue			=	GetPrivateProfileInt(strProjectName1,_T("MinGbValue"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_Typical_R				=	GetPrivateProfileInt(strProjectName1,_T("Typical_R"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_Typical_B				=	GetPrivateProfileInt(strProjectName1,_T("Typical_B"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_Typical_Gr			=	GetPrivateProfileInt(strProjectName1,_T("Typical_Gr"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_Typical_Gb			=	GetPrivateProfileInt(strProjectName1,_T("Typical_Gb"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_MaxISO				=	GetPrivateProfileInt(strProjectName1,_T("MaxISO"),				0, strFilePath);
	m_GetOTPCheckSpec.m_MinISO				=	GetPrivateProfileInt(strProjectName1,_T("MinISO"),				0, strFilePath);
			
	m_GetOTPCheckSpec.m_MaxRGain_HighCCT		=	GetPrivateProfileInt(strProjectName1,_T("MaxRGain_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinRGain_HighCCT		=	GetPrivateProfileInt(strProjectName1,_T("MinRGain_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxBGain_HighCCT		=	GetPrivateProfileInt(strProjectName1,_T("MaxBGain_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinBGain_HighCCT		=	GetPrivateProfileInt(strProjectName1,_T("MinBGain_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxGGGain_HighCCT		=	GetPrivateProfileInt(strProjectName1,_T("MaxGGGain_HighCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGGGain_HighCCT		=	GetPrivateProfileInt(strProjectName1,_T("MinGGGain_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_RG_HighCCT	=	GetPrivateProfileInt(strProjectName1,_T("Typical_RG_HighCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_Typical_BG_HighCCT	=	GetPrivateProfileInt(strProjectName1,_T("Typical_BG_HighCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_Typical_GG_HighCCT	=	GetPrivateProfileInt(strProjectName1,_T("Typical_GG_HighCCT"),	0, strFilePath);
															 
	m_GetOTPCheckSpec.m_MaxRGain_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("MaxRGain_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinRGain_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("MinRGain_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxBGain_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("MaxBGain_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinBGain_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("MinBGain_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxGGGain_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("MaxGGGain_Fluore"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGGGain_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("MinGGGain_Fluore"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_RG_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("Typical_RG_Fluore"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_Typical_BG_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("Typical_BG_Fluore"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_Typical_GG_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("Typical_GG_Fluore"),	0, strFilePath);
															
	m_GetOTPCheckSpec.m_MaxRGain_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("MaxRGain_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinRGain_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("MinRGain_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxBGain_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("MaxBGain_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinBGain_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("MinBGain_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxGGGain_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("MaxGGGain_LowCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGGGain_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("MinGGGain_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_RG_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_RG_LowCCT"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_Typical_BG_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_BG_LowCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_Typical_GG_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_GG_LowCCT"),	0, strFilePath);
															
	m_GetOTPCheckSpec.m_MaxR_HighCCT			=	GetPrivateProfileInt(strProjectName1,_T("MaxR_HighCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinR_HighCCT			=	GetPrivateProfileInt(strProjectName1,_T("MinR_HighCCT"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGr_HighCCT			=	GetPrivateProfileInt(strProjectName1,_T("MaxGr_HighCCT"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGr_HighCCT			=	GetPrivateProfileInt(strProjectName1,_T("MinGr_HighCCT"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGb_HighCCT			=	GetPrivateProfileInt(strProjectName1,_T("MaxGb_HighCCT"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGb_HighCCT			=	GetPrivateProfileInt(strProjectName1,_T("MinGb_HighCCT"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxB_HighCCT			=	GetPrivateProfileInt(strProjectName1,_T("MaxB_HighCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinB_HighCCT			=	GetPrivateProfileInt(strProjectName1,_T("MinB_HighCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_Typical_R_HighCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_R_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_Gr_HighCCT	=	GetPrivateProfileInt(strProjectName1,_T("Typical_Gr_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_Gb_HighCCT	=	GetPrivateProfileInt(strProjectName1,_T("Typical_Gb_HighCCT"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_B_HighCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_B_HighCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_AWBHighCCT_Flag		=	GetPrivateProfileInt(strProjectName1,_T("AWBHighCCT_Flag"),	0, strFilePath);
															 
	m_GetOTPCheckSpec.m_MaxR_LowCCT			=	GetPrivateProfileInt(strProjectName1,_T("MaxR_LowCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinR_LowCCT			=	GetPrivateProfileInt(strProjectName1,_T("MinR_LowCCT"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGr_LowCCT			=	GetPrivateProfileInt(strProjectName1,_T("MaxGr_LowCCT"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGr_LowCCT			=	GetPrivateProfileInt(strProjectName1,_T("MinGr_LowCCT"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGb_LowCCT			=	GetPrivateProfileInt(strProjectName1,_T("MaxGb_LowCCT"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGb_LowCCT			=	GetPrivateProfileInt(strProjectName1,_T("MinGb_LowCCT"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxB_LowCCT			=	GetPrivateProfileInt(strProjectName1,_T("MaxB_LowCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinB_LowCCT			=	GetPrivateProfileInt(strProjectName1,_T("MinB_LowCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_Typical_R_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_R_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_Gr_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_Gr_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_Gb_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_Gb_LowCCT"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_B_LowCCT		=	GetPrivateProfileInt(strProjectName1,_T("Typical_B_LowCCT"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_AWBLowCCT_Flag		=	GetPrivateProfileInt(strProjectName1,_T("AWBLowCCT_Flag"),		0, strFilePath);
															
	m_GetOTPCheckSpec.m_MaxR_Fluore			=	GetPrivateProfileInt(strProjectName1,_T("MaxR_Fluore"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinR_Fluore			=	GetPrivateProfileInt(strProjectName1,_T("MinR_Fluore"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGr_Fluore			=	GetPrivateProfileInt(strProjectName1,_T("MaxGr_Fluore"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGr_Fluore			=	GetPrivateProfileInt(strProjectName1,_T("MinGr_Fluore"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxGb_Fluore			=	GetPrivateProfileInt(strProjectName1,_T("MaxGb_Fluore"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinGb_Fluore			=	GetPrivateProfileInt(strProjectName1,_T("MinGb_Fluore"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxB_Fluore			=	GetPrivateProfileInt(strProjectName1,_T("MaxB_Fluore"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MinB_Fluore			=	GetPrivateProfileInt(strProjectName1,_T("MinB_Fluore"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_Typical_R_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("Typical_R_Fluore"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_Gr_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("Typical_Gr_Fluore"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_Gb_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("Typical_Gb_Fluore"),	0, strFilePath);
	m_GetOTPCheckSpec.m_Typical_B_Fluore		=	GetPrivateProfileInt(strProjectName1,_T("Typical_B_Fluore"),	0, strFilePath);					
	m_GetOTPCheckSpec.m_AWBFluore_Flag		=	GetPrivateProfileInt(strProjectName1,_T("AWBFluore_Flag"),		0, strFilePath);

	m_GetOTPCheckSpec.m_MaxHighCCTLightSource_RG	=	GetPrivateProfileInt(strProjectName1,_T("MaxHighCCTLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MinHighCCTLightSource_RG	=	GetPrivateProfileInt(strProjectName1,_T("MinHighCCTLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxHighCCTLightSource_BG	=	GetPrivateProfileInt(strProjectName1,_T("MaxHighCCTLightSource_BG"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinHighCCTLightSource_BG	=	GetPrivateProfileInt(strProjectName1,_T("MinHighCCTLightSource_BG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_LSCHighCCT_Flag			=	GetPrivateProfileInt(strProjectName1,_T("LSCHighCCT_Flag"),			0, strFilePath);
																  
	m_GetOTPCheckSpec.m_MaxLowCCTLightSource_RG	=	GetPrivateProfileInt(strProjectName1,_T("MaxLowCCTLightSource_RG"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinLowCCTLightSource_RG	=	GetPrivateProfileInt(strProjectName1,_T("MinLowCCTLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxLowCCTLightSource_BG	=	GetPrivateProfileInt(strProjectName1,_T("MaxLowCCTLightSource_BG"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinLowCCTLightSource_BG	=	GetPrivateProfileInt(strProjectName1,_T("MinLowCCTLightSource_BG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_LSCLowCCT_Flag			=	GetPrivateProfileInt(strProjectName1,_T("LSCLowCCT_Flag"),				0, strFilePath);
																  
	m_GetOTPCheckSpec.m_MaxFluoreLightSource_RG	=	GetPrivateProfileInt(strProjectName1,_T("MaxFluoreLightSource_RG"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinFluoreLightSource_RG	=	GetPrivateProfileInt(strProjectName1,_T("MinFluoreLightSource_RG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MaxFluoreLightSource_BG	=	GetPrivateProfileInt(strProjectName1,_T("MaxFluoreLightSource_BG"),	0, strFilePath);						
	m_GetOTPCheckSpec.m_MinFluoreLightSource_BG	=	GetPrivateProfileInt(strProjectName1,_T("MinFluoreLightSource_BG"),	0, strFilePath);
	m_GetOTPCheckSpec.m_LSCFluore_Flag			=	GetPrivateProfileInt(strProjectName1,_T("LSCFluore_Flag"),				0, strFilePath);

	m_GetOTPCheckSpec.m_SNFlag				=	GetPrivateProfileInt(strProjectName1,_T("SNFlag"),				    0, strFilePath);
	m_GetOTPCheckSpec.m_LSCFlag				=	GetPrivateProfileInt(strProjectName1,_T("LSCFlag"),				0, strFilePath);						
	m_GetOTPCheckSpec.m_LSCSize				=	GetPrivateProfileInt(strProjectName1,_T("LSCSize"),				0, strFilePath);		     
	m_GetOTPCheckSpec.m_LSCDataEqualCountSpec	=	GetPrivateProfileInt(strProjectName1,_T("LSCDataEqualCountSpec"),  0, strFilePath);		
	m_GetOTPCheckSpec.m_ToleranceValueError	=	GetPrivateProfileInt(strProjectName1,_T("ToleranceValueError"),	0, strFilePath);			
	m_GetOTPCheckSpec.m_ToleranceCountError	=	GetPrivateProfileInt(strProjectName1,_T("ToleranceCountError"),	0, strFilePath);			
	m_GetOTPCheckSpec.m_SaveLSCALLDataEn		=	GetPrivateProfileInt(strProjectName1,_T("SaveLSCALLDataEn"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_SaveLSCOKDataEn		=	GetPrivateProfileInt(strProjectName1,_T("SaveLSCOKDataEn"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_LSC_R_Max				=	GetPrivateProfileInt(strProjectName1,_T("LSC_R_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec.m_LSC_R_Min				=	GetPrivateProfileInt(strProjectName1,_T("LSC_R_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSC_Gr_Max			=	GetPrivateProfileInt(strProjectName1,_T("LSC_Gr_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec.m_LSC_Gr_Min			=	GetPrivateProfileInt(strProjectName1,_T("LSC_Gr_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSC_Gb_Max			=	GetPrivateProfileInt(strProjectName1,_T("LSC_Gb_Max"),			0, strFilePath);			    
	m_GetOTPCheckSpec.m_LSC_Gb_Min			=	GetPrivateProfileInt(strProjectName1,_T("LSC_Gb_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSC_B_Max				=	GetPrivateProfileInt(strProjectName1,_T("LSC_B_Max"),			0, strFilePath);				  
	m_GetOTPCheckSpec.m_LSC_B_Min				=	GetPrivateProfileInt(strProjectName1,_T("LSC_B_Min"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_LSCDistanceSpec		=	GetPrivateProfileInt(strProjectName1,_T("LSCDistanceSpec"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_CenterValueEqualcount =	GetPrivateProfileInt(strProjectName1,_T("CenterValueEqualcount"),0, strFilePath);		
	m_GetOTPCheckSpec.m_Curvediff				=	GetPrivateProfileInt(strProjectName1,_T("Curvediff"),			0, strFilePath);								
	m_GetOTPCheckSpec.m_ErrorCount			=	GetPrivateProfileInt(strProjectName1,_T("ErrorCount"),			0, strFilePath); 							
	m_GetOTPCheckSpec.m_TotalErrorCount		=	GetPrivateProfileInt(strProjectName1,_T("TotalErrorCount"),	0, strFilePath);			

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("edg_center_LimitRatio"),_T("1.01"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_edg_center_LimitRatio	= _wtof(szTmp);

	memset(szTmp, 0, sizeof(szTmp));
	GetPrivateProfileString(strProjectName1, _T("edg_center_HighRatio"),_T("1.01"), szTmp, iBufSize, strFilePath);
	m_GetOTPCheckSpec.m_edg_center_HighRatio  = _wtof(szTmp);

	m_GetOTPCheckSpec.m_MaxSymColDiff			=	GetPrivateProfileInt(strProjectName1,_T("MaxSymColDiff"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_MaxSymRowDiff			=	GetPrivateProfileInt(strProjectName1,_T("MaxSymRowDiff"),		0, strFilePath);						
	m_GetOTPCheckSpec.m_AFFlag				=	GetPrivateProfileInt(strProjectName1,_T("AFFlag"),				0, strFilePath);						
	m_GetOTPCheckSpec.m_AFCalDirection		=	GetPrivateProfileInt(strProjectName1,_T("AFCalDirection"),		0, strFilePath);	  
	m_GetOTPCheckSpec.m_MaxAFCode1000cm		=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode1000cm"),	0, strFilePath);	
	m_GetOTPCheckSpec.m_MinAFCode1000cm		=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode1000cm"),	0, strFilePath);	
	m_GetOTPCheckSpec.m_MaxAFCode500cm		=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode500cm"),		0, strFilePath);		
	m_GetOTPCheckSpec.m_MinAFCode500cm		=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode500cm"),		0, strFilePath);		
	m_GetOTPCheckSpec.m_MaxAFCode10cm			=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode10cm"),		0, strFilePath);		
	m_GetOTPCheckSpec.m_MinAFCode10cm			=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode10cm"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxAFCode12cm			=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode12cm"),		0, strFilePath);		
	m_GetOTPCheckSpec.m_MinAFCode12cm			=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode12cm"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxAFCode8cm			=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode8cm"),		0, strFilePath);		
	m_GetOTPCheckSpec.m_MinAFCode8cm			=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode8cm"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxAFCode7cm			=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode7cm"),		0, strFilePath);		
	m_GetOTPCheckSpec.m_MinAFCode7cm			=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode7cm"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxCodeDiff			=	GetPrivateProfileInt(strProjectName1,_T("MaxCodeDiff"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinCodeDiff			=	GetPrivateProfileInt(strProjectName1,_T("MinCodeDiff"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MaxStartCode			=	GetPrivateProfileInt(strProjectName1,_T("MaxStartCode"),		0, strFilePath);					
	m_GetOTPCheckSpec.m_MinStartCode			=	GetPrivateProfileInt(strProjectName1,_T("MinStartCode"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxAFStartCurrent		=	GetPrivateProfileInt(strProjectName1,_T("MaxAFStartCurrent"),	0, strFilePath);
	m_GetOTPCheckSpec.m_MinAFStartCurrent		=	GetPrivateProfileInt(strProjectName1,_T("MinAFStartCurrent"),	0, strFilePath);
															 
	m_GetOTPCheckSpec.m_PDAFFlag				=	GetPrivateProfileInt(strProjectName1,_T("PDAFFlag"),			0, strFilePath);						
	m_GetOTPCheckSpec.m_PDAFStep1Flag			=	GetPrivateProfileInt(strProjectName1,_T("PDAFStep1Flag"),		0, strFilePath);	
	m_GetOTPCheckSpec.m_PDAFStep1Size			=	GetPrivateProfileInt(strProjectName1,_T("PDAFStep1Size"),		0, strFilePath);	  
	m_GetOTPCheckSpec.m_PDAFStep2Flag			=	GetPrivateProfileInt(strProjectName1,_T("PDAFStep2Flag"),		0, strFilePath);	  
	m_GetOTPCheckSpec.m_PDAFStep2Size			=	GetPrivateProfileInt(strProjectName1,_T("PDAFStep2Size"),		0, strFilePath);	  
	m_GetOTPCheckSpec.m_PDAFVersionNum		=	GetPrivateProfileInt(strProjectName1,_T("PDAFVersionNum"),		0, strFilePath);
	m_GetOTPCheckSpec.m_GainMapFlag			=	GetPrivateProfileInt(strProjectName1,_T("GainMapFlag"),		0, strFilePath);
	m_GetOTPCheckSpec.m_GainMapWidth			=	GetPrivateProfileInt(strProjectName1,_T("GainMapWidth"),		0, strFilePath);			
	m_GetOTPCheckSpec.m_GainMapHeight			=	GetPrivateProfileInt(strProjectName1,_T("GainMapHeight"),		0, strFilePath);		
	m_GetOTPCheckSpec.m_MaxGainMap			=	GetPrivateProfileInt(strProjectName1,_T("MaxGainMap"),			0, strFilePath);				
	m_GetOTPCheckSpec.m_MinGainMap			=	GetPrivateProfileInt(strProjectName1,_T("MinGainMap"),			0, strFilePath);				
	m_GetOTPCheckSpec.m_DCCFlag				=	GetPrivateProfileInt(strProjectName1,_T("DCCFlag"),			0, strFilePath);	 				
	m_GetOTPCheckSpec.m_DCCFormat				=	GetPrivateProfileInt(strProjectName1,_T("DCCFormat"),			0, strFilePath);				
	m_GetOTPCheckSpec.m_DCCMapWidth			=	GetPrivateProfileInt(strProjectName1,_T("DCCMapWidth"),		0, strFilePath);			
	m_GetOTPCheckSpec.m_DCCMapHeight			=	GetPrivateProfileInt(strProjectName1,_T("DCCMapHeight"),		0, strFilePath);			
	m_GetOTPCheckSpec.m_MaxDCC				=	GetPrivateProfileInt(strProjectName1,_T("MaxDCC"),				0, strFilePath);						
	m_GetOTPCheckSpec.m_MinDCC				=	GetPrivateProfileInt(strProjectName1,_T("MinDCC"),				0, strFilePath);						
	m_GetOTPCheckSpec.m_OISWideTag			=	GetPrivateProfileInt(strProjectName1,_T("OISWideTag"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_OISVersion			=	GetPrivateProfileInt(strProjectName1,_T("OISVersion"),			0, strFilePath);					
	m_GetOTPCheckSpec.m_OCFlag				=	GetPrivateProfileInt(strProjectName1,_T("OCFlag"),				0, strFilePath);		  	
	m_GetOTPCheckSpec.m_MaxOCX				=	GetPrivateProfileInt(strProjectName1,_T("MaxOCX"),				0, strFilePath);	  	  
	m_GetOTPCheckSpec.m_MinOCX				=	GetPrivateProfileInt(strProjectName1,_T("MinOCX"),				0, strFilePath);	  		
	m_GetOTPCheckSpec.m_MaxOCY				=	GetPrivateProfileInt(strProjectName1,_T("MaxOCY"),				0, strFilePath);	  		
	m_GetOTPCheckSpec.m_MinOCY				=	GetPrivateProfileInt(strProjectName1,_T("MinOCY"),				0, strFilePath);	  	  
	m_GetOTPCheckSpec.m_MaxShiftX				=	GetPrivateProfileInt(strProjectName1,_T("MaxShiftX"),			0, strFilePath);	 	
	m_GetOTPCheckSpec.m_MinShiftX				=	GetPrivateProfileInt(strProjectName1,_T("MinShiftX"),			0, strFilePath);	  
	m_GetOTPCheckSpec.m_MaxShiftY				=	GetPrivateProfileInt(strProjectName1,_T("MaxShiftY"),			0, strFilePath);	 	
	m_GetOTPCheckSpec.m_MinShiftY				=	GetPrivateProfileInt(strProjectName1,_T("MinShiftY"),			0, strFilePath);	 	
	m_GetOTPCheckSpec.m_MaxTiltX				=	GetPrivateProfileInt(strProjectName1,_T("MaxTiltX"),			0, strFilePath);	  	
	m_GetOTPCheckSpec.m_MinTiltX				=	GetPrivateProfileInt(strProjectName1,_T("MinTiltX"),			0, strFilePath);	 		
	m_GetOTPCheckSpec.m_MaxTiltY				=	GetPrivateProfileInt(strProjectName1,_T("MaxTiltY"),			0, strFilePath);			
	m_GetOTPCheckSpec.m_MinTiltY				=	GetPrivateProfileInt(strProjectName1,_T("MinTiltY"),			0, strFilePath);			
	m_GetOTPCheckSpec.m_MaxRotation			=	GetPrivateProfileInt(strProjectName1,_T("MaxRotation"),		0, strFilePath);	
	m_GetOTPCheckSpec.m_MinRotation			=	GetPrivateProfileInt(strProjectName1,_T("MinRotation"),		0, strFilePath);	
	m_GetOTPCheckSpec.m_MaxSPC				=	GetPrivateProfileInt(strProjectName1,_T("MaxSPC"),				0, strFilePath);					
	m_GetOTPCheckSpec.m_MinSPC				=	GetPrivateProfileInt(strProjectName1,_T("MinSPC"),				0, strFilePath);					
	m_GetOTPCheckSpec.m_BurnedAFUpDownPosture =	GetPrivateProfileInt(strProjectName1,_T("BurnedAFUpDownPosture"),0, strFilePath);		
	m_GetOTPCheckSpec.m_AFUpDownPostureFlag	=	GetPrivateProfileInt(strProjectName1,_T("AFUpDownPostureFlag"),0, strFilePath);	
	m_GetOTPCheckSpec.m_MaxAFCode8cmUp		=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode8cmUp"),		0, strFilePath);				
	m_GetOTPCheckSpec.m_MinAFCode8cmUp		=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode8cmUp"),		0, strFilePath);				
	m_GetOTPCheckSpec.m_MaxAFCode1000cmUp		=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode1000cmUp"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_MinAFCode1000cmUp		=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode1000cmUp"),	0, strFilePath);		
	m_GetOTPCheckSpec.m_MaxCodeDiffUp			=	GetPrivateProfileInt(strProjectName1,_T("MaxCodeDiffUp"),		0, strFilePath);				
	m_GetOTPCheckSpec.m_MinCodeDiffUp			=	GetPrivateProfileInt(strProjectName1,_T("MinCodeDiffUp"),		0, strFilePath);				
	m_GetOTPCheckSpec.m_MaxAFCode8cmDown		=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode8cmDown"),	0, strFilePath);			
	m_GetOTPCheckSpec.m_MinAFCode8cmDown		=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode8cmDown"),	0, strFilePath);			
	m_GetOTPCheckSpec.m_MaxAFCode1000cmDown	=	GetPrivateProfileInt(strProjectName1,_T("MaxAFCode1000cmDown"),0, strFilePath);	
	m_GetOTPCheckSpec.m_MinAFCode1000cmDown	=	GetPrivateProfileInt(strProjectName1,_T("MinAFCode1000cmDown"),0, strFilePath); 
	m_GetOTPCheckSpec.m_MaxCodeDiffDown		=	GetPrivateProfileInt(strProjectName1,_T("MaxCodeDiffDown"),	0, strFilePath);			
	m_GetOTPCheckSpec.m_MinCodeDiffDown		=	GetPrivateProfileInt(strProjectName1,_T("MinCodeDiffDown"),	0, strFilePath);			
	m_GetOTPCheckSpec.m_CrossTalkFlag			=	GetPrivateProfileInt(strProjectName1,_T("CrossTalkFlag"),		0, strFilePath);
	m_GetOTPCheckSpec.m_ArcsoftFlag			=	GetPrivateProfileInt(strProjectName1,_T("ArcsoftFlag"),		0, strFilePath);
	m_GetOTPCheckSpec.m_MaxAVDD				=	GetPrivateProfileInt(strProjectName1,_T("MaxAVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinAVDD				=	GetPrivateProfileInt(strProjectName1,_T("MinAVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MaxDVDD				=	GetPrivateProfileInt(strProjectName1,_T("MaxDVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinDVDD				=	GetPrivateProfileInt(strProjectName1,_T("MinDVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MaxIOVDD				=	GetPrivateProfileInt(strProjectName1,_T("MaxIOVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinIOVDD				=	GetPrivateProfileInt(strProjectName1,_T("MinIOVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MaxAFVDD				=	GetPrivateProfileInt(strProjectName1,_T("MaxAFVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinAFVDD				=	GetPrivateProfileInt(strProjectName1,_T("MinAFVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MaxDRVAVDD			=	GetPrivateProfileInt(strProjectName1,_T("MaxDRVAVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinDRVAVDD			=	GetPrivateProfileInt(strProjectName1,_T("MinDRVAVDD"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MaxAVDD18				=	GetPrivateProfileInt(strProjectName1,_T("MaxAVDD18"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinAVDD18				=	GetPrivateProfileInt(strProjectName1,_T("MinAVDD18"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MaxAVDD28				=	GetPrivateProfileInt(strProjectName1,_T("MaxAVDD28"),			0, strFilePath);
	m_GetOTPCheckSpec.m_MinAVDD28				=	GetPrivateProfileInt(strProjectName1,_T("MinAVDD28"),			0, strFilePath);
    m_GetOTPCheckSpec.m_LRCFlag               =   GetPrivateProfileInt(strProjectName1,_T("LRCFLAG"),            0, strFilePath);
    m_GetOTPCheckSpec.m_GainMapHighByte       =   GetPrivateProfileInt(strProjectName1,_T("GainMapHighByte"),    0, strFilePath);
    m_GetOTPCheckSpec.m_GainMapLowByte        =   GetPrivateProfileInt(strProjectName1,_T("GainMapLowByte"),     0, strFilePath);
    m_GetOTPCheckSpec.m_DCCMapHighByte        =   GetPrivateProfileInt(strProjectName1,_T("DCCMapHighByte"),     0, strFilePath);
    m_GetOTPCheckSpec.m_DCCMapLowByte         =   GetPrivateProfileInt(strProjectName1,_T("DCCMapLowByte"),      0, strFilePath);
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
	GetPrivateProfileString(strProjectName2,_T("TestStation"), _T("Image"), szTmp, iBufSize, strFilePath);	  //获取机种名
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
				   
	m_GetOTPCheckSpec1.m_SaveOTPCheckData		=	GetPrivateProfileInt(strProjectName2,_T("SaveOTPCheckData"),	1, strFilePath);
	m_GetOTPCheckSpec1.m_LSCCurveCheck		=	GetPrivateProfileInt(strProjectName2,_T("LSCCurveCheck"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_EqualCountSpec		=	GetPrivateProfileInt(strProjectName2,_T("EqualCountSpec"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_HisiMasterFlag		=	GetPrivateProfileInt(strProjectName2,_T("HisiMasterFlag"),		0, strFilePath);
				   
	m_GetOTPCheckSpec1.m_ModulInfoFlag		=	GetPrivateProfileInt(strProjectName2,_T("ModulInfoFlag"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_ModuleID				=	GetPrivateProfileInt(strProjectName2,_T("ModuleID"),			0, strFilePath);      			
	m_GetOTPCheckSpec1.m_LensID				=	GetPrivateProfileInt(strProjectName2,_T("LensID"),				0, strFilePath);	      			
	m_GetOTPCheckSpec1.m_VCMID				=	GetPrivateProfileInt(strProjectName2,_T("VCMID"),				0, strFilePath);							
	m_GetOTPCheckSpec1.m_DriverIcID			=	GetPrivateProfileInt(strProjectName2,_T("DriverIcID"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_LSCToolVersion		=	GetPrivateProfileInt(strProjectName2,_T("LSCToolVersion"),		0, strFilePath);			
	m_GetOTPCheckSpec1.m_PDAFToolVersion		=	GetPrivateProfileInt(strProjectName2,_T("PDAFToolVersion"),	0, strFilePath);		
	m_GetOTPCheckSpec1.m_ColorTemperatureID	=	GetPrivateProfileInt(strProjectName2,_T("ColorTemperatureID"),	0, strFilePath);	
	m_GetOTPCheckSpec1.m_FirstPixel			=	GetPrivateProfileInt(strProjectName2,_T("FirstPixel"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_MirrorFlip			=	GetPrivateProfileInt(strProjectName2,_T("MirrorFlip"),			0, strFilePath);					
	m_GetOTPCheckSpec1.m_SensorNameID			=	GetPrivateProfileInt(strProjectName2,_T("SensorNameID"),		0, strFilePath);				
	m_GetOTPCheckSpec1.m_SupplierCode			=	GetPrivateProfileInt(strProjectName2,_T("SupplierCode"),		0, strFilePath);				
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
//	m_GetOTPCheckSpec1.m_PDAFStation			=	GetPrivateProfileInt(strProjectName2,_T("PDAFStation"),		0, strFilePath);			
//	m_GetOTPCheckSpec1.m_AFStation			=	GetPrivateProfileInt(strProjectName2,_T("AFStation"),			0, strFilePath);				
	m_GetOTPCheckSpec1.m_EEPROMID				=	GetPrivateProfileInt(strProjectName2,_T("EEPROMID"),			0, strFilePath);						
	m_GetOTPCheckSpec1.m_ProgramName			=	GetPrivateProfileInt(strProjectName2,_T("ProgramName"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_SensorSlaveID		=	GetPrivateProfileInt(strProjectName2,_T("SensorSlaveID"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_EEPROMSlaveID		=	GetPrivateProfileInt(strProjectName2,_T("EEPROMSlaveID"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_VCMSlaveID			=	GetPrivateProfileInt(strProjectName2,_T("VCMSlaveID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_SensorStatus			=	GetPrivateProfileInt(strProjectName2,_T("SensorStatus"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_CheckSN_En			=	GetPrivateProfileInt(strProjectName2,_T("CheckSN_En"),			0, strFilePath);
   
				   
	m_GetOTPCheckSpec1.m_VendorID				=	GetPrivateProfileInt(strProjectName2,_T("VendorID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_OISFWVER				=	GetPrivateProfileInt(strProjectName2,_T("OISFWVER"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_ProductID			=	GetPrivateProfileInt(strProjectName2,_T("ProductID"),			0, strFilePath);
	m_GetOTPCheckSpec1.m_MapVersionID			=	GetPrivateProfileInt(strProjectName2,_T("MapVersionID"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MapHistoryID			=	GetPrivateProfileInt(strProjectName2,_T("MapHistoryID"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_EepromorOTP			=	GetPrivateProfileInt(strProjectName2,_T("EepromorOTP"),		0, strFilePath);
				   
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
				   
	m_GetOTPCheckSpec1.m_MaxR_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxR_LowCCT"),		0, strFilePath);						
	m_GetOTPCheckSpec1.m_MinR_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinR_LowCCT"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGr_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxGr_LowCCT"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGr_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinGr_LowCCT"),		0, strFilePath);
	m_GetOTPCheckSpec1.m_MaxGb_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MaxGb_LowCCT"),		0, strFilePath);					
	m_GetOTPCheckSpec1.m_MinGb_LowCCT			=	GetPrivateProfileInt(strProjectName2,_T("MinGb_LowCCT"),		0, strFilePath);
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
	m_GetOTPCheckSpec1.m_MaxAFStartCurrent	=	GetPrivateProfileInt(strProjectName2,_T("MaxAFStartCurrent"),	0, strFilePath);
	m_GetOTPCheckSpec1.m_MinAFStartCurrent	=	GetPrivateProfileInt(strProjectName2,_T("MinAFStartCurrent"),	0, strFilePath);
    
				   
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

BOOL OtpCheckAPI::LensVerificationLib(CString str,USHORT *Data,wchar_t *sErrorMsg,BOOL isWide)
{
	int nWidthCount = 17;
	int nHeightCount= 13;

	int i,j,k;
	int nCount = 0;
	int nValue = 0;

	int	ToleranceValueError = m_GetOTPCheckSpec.m_ToleranceValueError;
	int	ToleranceCountError = m_GetOTPCheckSpec.m_ToleranceCountError;

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
//     if (m_GetOTPCheckSpec.m_ProjectName1.Find(TESTSTATION) == -1)
//     {
// 
//         m_pInterface->Ctrl_Addlog(CamID,_T("项目名错误!"),COLOR_RED,200);
//         m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("OTP Check Fail!"));
//         SetResult(RESULT_FAIL);
//         return 0;
// 
//     }
	else
	{
       //m_pInterface->Ctrl_Addlog(CamID,_T("Channel LSC 数据正确!"),COLOR_BLUE,200);
        //return FALSE;
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

	CString SensorINI = GetModulePath() + _T("Standard.ini");  // 设定参数文档路径

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

	FileName.Format(_T("LSCData.txt"));
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

	CString SensorINI = GetModulePath() + _T("Standard.ini");  // 设定参数文档路径 （单体使用）	
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

	FileName.Format(_T("LSCData1.txt"));
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

	CString   SensorINI = GetModulePath() + _T("Standard_Slave.ini");  // 设定参数文档路径

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

	FileName.Format(_T("LSCData2.txt"));
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

BOOL OtpCheckAPI::CheckModuleInfo_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check module info
	if (OTPReadData[0x0000] != m_GetOTPCheckSpec.m_ModulInfoFlag)
	{
		Catstring(ErrorMsg, _T("Module InfoFlag 0x%02X != 0x%02X"),OTPReadData[0x0000],m_GetOTPCheckSpec.m_ModulInfoFlag);
		return FALSE;
	}
	
	if (OTPReadData[0x0001]>21 || OTPReadData[0x0001]<17)
	{
		Catstring(ErrorMsg, _T("Year %d not in<17,21>"),OTPReadData[0x0001]);
		return FALSE;
	}
	if (OTPReadData[0x0002]>12 || OTPReadData[0x0002]<1)
	{
		Catstring(ErrorMsg, _T("Month %d not in<1,12>"),OTPReadData[0x0002]);
		return FALSE;
	}
	if (OTPReadData[0x0003]>31 || OTPReadData[0x0003]<1)
	{
		Catstring(ErrorMsg, _T("Day %d not in<1,31>"),OTPReadData[0x0003]);
		return FALSE;
	}                                                        
    if (OTPReadData[0x0004] != m_GetOTPCheckSpec.m_ModuleID)                                                                                                                                      
    {
        Catstring(ErrorMsg, _T("m_ModuleID 0x%02X != 0x%02X"),OTPReadData[0x0004],m_GetOTPCheckSpec.m_ModuleID);
        return FALSE;
    }
    if (OTPReadData[0x0005] != m_GetOTPCheckSpec.m_MirrorFlip)
    {
        Catstring(ErrorMsg, _T("MirrorFlip 0x%02X != 0x%02X"),OTPReadData[0x0005],m_GetOTPCheckSpec.m_MirrorFlip);
        return FALSE;
    }
    if (OTPReadData[0x0006] != m_GetOTPCheckSpec.m_FirstPixel)
    {
        Catstring(ErrorMsg, _T("FirstPixel 0x%02X != 0x%02X"),OTPReadData[0x0006],m_GetOTPCheckSpec.m_FirstPixel);
        return FALSE;
    }
// 	if (OTPReadData[0x0005]>23 || OTPReadData[0x0005]<0)
// 	{
// 		Catstring(ErrorMsg, _T("Hour %d not in<0,23>"),OTPReadData[0x0005]);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x0006]>59 || OTPReadData[0x0006]<0)
// 	{
// 		Catstring(ErrorMsg, _T("Minute %d not in<0,59>"),OTPReadData[0x0006]);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x0007]>59 || OTPReadData[0x0007]<0)
// 	{
// 		Catstring(ErrorMsg, _T("Second %d not in<0,59>"),OTPReadData[0x0007]);
// 		return FALSE;
// 	}
	if (OTPReadData[0x0007] != m_GetOTPCheckSpec.m_LensID)             
	{
		Catstring(ErrorMsg, _T("m_LensID 0x%02X != 0x%02X"),OTPReadData[0x0007],m_GetOTPCheckSpec.m_LensID);
		return FALSE;
	}
	if (OTPReadData[0x0008] != m_GetOTPCheckSpec.m_VCMID)
	{
		Catstring(ErrorMsg, _T("m_VCMID 0x%02X != 0x%02X"),OTPReadData[0x0008],m_GetOTPCheckSpec.m_VCMID);
		return FALSE;
	}
	if (OTPReadData[0x0009] != m_GetOTPCheckSpec.m_DriverIcID)
	{
		Catstring(ErrorMsg, _T("m_DriverIcID 0x%02X != 0x%02X"),OTPReadData[0x0009],m_GetOTPCheckSpec.m_DriverIcID);
        return FALSE;
    }   
    if (OTPReadData[0x000A] != m_GetOTPCheckSpec.m_IRBGID)
    {
        Catstring(ErrorMsg, _T("m_IR/BGID 0x%02X != 0x%02X"),OTPReadData[0x000A],m_GetOTPCheckSpec.m_IRBGID);
        return FALSE;
    }
    if (OTPReadData[0x000B] != m_GetOTPCheckSpec.m_ColorTemperatureID)
    {
        Catstring(ErrorMsg, _T("m_ColorTemperatureID 0x%02X != 0x%02X"),OTPReadData[0x000B],m_GetOTPCheckSpec.m_ColorTemperatureID);
        return FALSE;
    }
    if (OTPReadData[0x000C] != m_GetOTPCheckSpec.m_AForFF_Flag)
    {
        Catstring(ErrorMsg, _T("m_AForFF Flag 0x%02X != 0x%02X"),OTPReadData[0x000C],m_GetOTPCheckSpec.m_AForFF_Flag);
        return FALSE;
    }
    if (OTPReadData[0x000D] != m_GetOTPCheckSpec.m_LightSourceFlag)
    {
        Catstring(ErrorMsg, _T("m_LightSourceFlag 0x%02X != 0x%02X"),OTPReadData[0x000D],m_GetOTPCheckSpec.m_LightSourceFlag);
        return FALSE;
    }
// 	if (OTPReadData[0x000B] != m_GetOTPCheckSpec.m_OISFWVER)
// 	{
// 		Catstring(ErrorMsg, _T("m_OISFWVER 0x%02X != 0x%02X"),OTPReadData[0x000B],m_GetOTPCheckSpec.m_OISFWVER);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x000C] != m_GetOTPCheckSpec.m_SensorNameID)
// 	{
// 		Catstring(ErrorMsg, _T("m_SensorNameID 0x%02X != 0x%02X"),OTPReadData[0x000C],m_GetOTPCheckSpec.m_SensorNameID);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x000D] != m_GetOTPCheckSpec.m_ProductID)
// 	{
// 		Catstring(ErrorMsg, _T("m_ProductID 0x%02X != 0x%02X"),OTPReadData[0x000D],m_GetOTPCheckSpec.m_ProductID);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x000E] != m_GetOTPCheckSpec.m_MapVersionID)
// 	{
// 		Catstring(ErrorMsg, _T("m_MapVersionID 0x%02X != 0x%02X"),OTPReadData[0x000E],m_GetOTPCheckSpec.m_MapVersionID);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x000F] != m_GetOTPCheckSpec.m_MapHistoryID)
// 	{
// 		Catstring(ErrorMsg, _T("m_MapHistoryID 0x%02X != 0x%02X"),OTPReadData[0x000F],m_GetOTPCheckSpec.m_MapHistoryID);
// 		return FALSE;
// 	}

// 	int temp=0;
// 	for (int i=0;i<16;i++)
// 	{
// 		if ((OTPReadData[0x0010+i] == 0) || (OTPReadData[0x0010+i] == 0xFF))
// 		{
// 			temp++;
// 		}
// 	}
// 	if (temp>15)
// 	{
// 		Catstring(ErrorMsg, _T("Module Serial No.全为0或0xFF,未烧录Fuse ID!"));
// 		return FALSE;
// 	}

	//check module info reserve
	for (int i = 0x000E; i < 0x001F; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg, _T("Module reserve 0x%04X is 0x%02X != 0xFF"),i,OTPReadData[i]);
			return FALSE;
		}
	}
// 	if (OTPReadData[0x0023] != m_GetOTPCheckSpec.m_EepromorOTP)
// 	{
// 		Catstring(ErrorMsg, _T("m_EepromorOTP 0x%02X != 0x%02X"),OTPReadData[0x0023],m_GetOTPCheckSpec.m_EepromorOTP);
// 		return FALSE;
// 	}

	//check module info Check sum
	int m_ModuleInfoSum = 0;
	for (int i = 0x0001; i < 0x001F; i++)
	{
		m_ModuleInfoSum += OTPReadData[i];
	}
	m_ModuleInfoSum = (m_ModuleInfoSum%0xFF);
	if (m_ModuleInfoSum != OTPReadData[0x001F])
	{
		Catstring(ErrorMsg, _T("ModuleInfo CheckSum 0x%02X != 0x%02X"),OTPReadData[0x001F],m_ModuleInfoSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckModuleInfo_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check module info
	if (OTPReadData[0x0000] != m_GetOTPCheckSpec1.m_ModulInfoFlag)
	{
		Catstring(ErrorMsg, _T("ModuleInfoFlag 0x%02X != 0x%02X"),OTPReadData[0x0000],m_GetOTPCheckSpec1.m_ModulInfoFlag);
		return FALSE;
	}
	if (OTPReadData[0x0001] != m_GetOTPCheckSpec1.m_ModuleID)
	{
		Catstring(ErrorMsg, _T("ModuleID 0x%02X != 0x%02X"),OTPReadData[0x0001],m_GetOTPCheckSpec1.m_ModuleID);
		return FALSE;
	}
	if (OTPReadData[0x0002] != m_GetOTPCheckSpec1.m_FirstPixel)
	{
		Catstring(ErrorMsg, _T("FirstPixel 0x%02X != 0x%02X"),OTPReadData[0x0002],m_GetOTPCheckSpec1.m_FirstPixel);
		return FALSE;
	}
	if (OTPReadData[0x0003] != m_GetOTPCheckSpec1.m_MirrorFlip)
	{
		Catstring(ErrorMsg, _T("MirrorFlip 0x%02X != 0x%02X"),OTPReadData[0x0003],m_GetOTPCheckSpec1.m_MirrorFlip);
		return FALSE;
	}
	if (OTPReadData[0x0004] != 0x00)
	{
		Catstring(ErrorMsg, _T("Reserved 0x0004 is 0x%02X != 0x00"),OTPReadData[0x0004]);
		return FALSE;
	}
	if (OTPReadData[0x0005] != m_GetOTPCheckSpec1.m_LSCToolVersion)
	{
		Catstring(ErrorMsg, _T("LSCToolVersion 0x%02X != 0x%02X"),OTPReadData[0x0005],m_GetOTPCheckSpec1.m_LSCToolVersion);
		return FALSE;
	}
	if (OTPReadData[0x0006] != m_GetOTPCheckSpec1.m_PDAFToolVersion)
	{
		Catstring(ErrorMsg, _T("PDAFToolVersion 0x%02X != 0x%02X"),OTPReadData[0x0006],m_GetOTPCheckSpec1.m_PDAFToolVersion);
		return FALSE;
	}
	if (OTPReadData[0x0007] != m_GetOTPCheckSpec1.m_EEPROMID)
	{
		Catstring(ErrorMsg, _T("EEPROMID 0x%02X != 0x%02X"),OTPReadData[0x0007],m_GetOTPCheckSpec1.m_EEPROMID);
		return FALSE;
	}
	if (OTPReadData[0x0008] != m_GetOTPCheckSpec1.m_LensID)
	{
		Catstring(ErrorMsg, _T("LensID 0x%02X != 0x%02X"),OTPReadData[0x0008],m_GetOTPCheckSpec1.m_LensID);
		return FALSE;
	}
	if (OTPReadData[0x0009] != m_GetOTPCheckSpec1.m_VCMID)
	{
		Catstring(ErrorMsg, _T("VCMID 0x%02X != 0x%02X"),OTPReadData[0x0009],m_GetOTPCheckSpec1.m_VCMID);
		return FALSE;
	}
	if (OTPReadData[0x000A]>22 || OTPReadData[0x000A]<17)
	{
		Catstring(ErrorMsg, _T("Year %d not in<17,22>"),OTPReadData[0x000A]);
		return FALSE;
	}
	if (OTPReadData[0x000B]>12 || OTPReadData[0x000B]<1)
	{
		Catstring(ErrorMsg, _T("Month %d not in<1,12>"),OTPReadData[0x000B]);
		return FALSE;
	}
	if (OTPReadData[0x000C]>31 || OTPReadData[0x000C]<1)
	{
		Catstring(ErrorMsg, _T("Day %d not in<1,31>"),OTPReadData[0x000C]);
		return FALSE;
	}
	for (int i = 0x000D;i < 0x0013;i++)
	{
		if (OTPReadData[i] != 0x00)
		{
			Catstring(ErrorMsg, _T("ModuleInfo  Reserved 0x%04X is 0x%02X != 0x00"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	//check module info Check sum
	int m_ModuleInfoSum = 0;
	for (int i = 0x0000; i < 0x0013; i++)
	{
		m_ModuleInfoSum += OTPReadData[i];
	}
	m_ModuleInfoSum = (m_ModuleInfoSum%255) + 1;
	if (OTPReadData[0x0013] != 0x00)
	{
		Catstring(ErrorMsg, _T("ModuleInfo CheckSum 0x0013 is 0x%02X != 0x00"),OTPReadData[0x0013]);
		return FALSE;
	}
	if (m_ModuleInfoSum != OTPReadData[0x0014])
	{
		Catstring(ErrorMsg, _T("ModuleInfo CheckSum 0x0014 is 0x%02X != 0x%02X"),OTPReadData[0x0014],m_ModuleInfoSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckAWB_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check AWB  S5k2x7
	int m_value = 0;
	int RGain		= ((OTPReadData[0x0031]<<8)&0xFF00) +OTPReadData[0x0032];
	int BGain		= ((OTPReadData[0x0033]<<8)&0xFF00) +OTPReadData[0x0034];
	int GGGain		= ((OTPReadData[0x0035]<<8)&0xFF00) +OTPReadData[0x0036];
	//int RBGain		= int(1000.0*float(RGain)/float(BGain));
    int Typical_RG	= ((OTPReadData[0x0037]<<8)&0xFF00) +OTPReadData[0x0038];				
    int Typical_BG	= ((OTPReadData[0x0039]<<8)&0xFF00) +OTPReadData[0x003A];					
    int Typical_GG	= ((OTPReadData[0x003B]<<8)&0xFF00) +OTPReadData[0x003C];	
    

	//  check module have equal value 
	int m_RGainjudgevalue = CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("RGainEqualCount"),_T("RGainEqualRef"),RGain,m_GetOTPCheckSpec.m_EqualCountSpec,ErrorMsg);
	if (!m_RGainjudgevalue)
	{
		return FALSE;
	}
	int m_BGainjudgevalue = CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("BGainEqualCount"),_T("BGainEqualRef"),BGain,m_GetOTPCheckSpec.m_EqualCountSpec,ErrorMsg);
	if (!m_BGainjudgevalue)
	{
		return FALSE;
	}
	int m_GGainjudgevalue = CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("GGainEqualCount"),_T("GGainEqualRef"),GGGain,m_GetOTPCheckSpec.m_EqualCountSpec,ErrorMsg);
	if (!m_GGainjudgevalue)
	{
		return FALSE;
	}	

	if (OTPReadData[0x0030] != m_GetOTPCheckSpec.m_AWBFlag)
	{
		Catstring(ErrorMsg, _T("AWBFlag 0x%02X != 0x%02X"),OTPReadData[0x0030],m_GetOTPCheckSpec.m_AWBFlag);
		return FALSE;
	}

	if ((RGain > m_GetOTPCheckSpec.m_MaxRGain) || (RGain < m_GetOTPCheckSpec.m_MinRGain))
	{
		Catstring(ErrorMsg, _T("RGain %d not in<%d,%d>"),RGain,m_GetOTPCheckSpec.m_MinRGain,m_GetOTPCheckSpec.m_MaxRGain);
		return FALSE;
	}
	if ((BGain > m_GetOTPCheckSpec.m_MaxBGain) || (BGain < m_GetOTPCheckSpec.m_MinBGain))
	{
		Catstring(ErrorMsg, _T("BGain %d not in<%d,%d>"),BGain,m_GetOTPCheckSpec.m_MinBGain,m_GetOTPCheckSpec.m_MaxBGain);
		return FALSE;
	}
	if ((GGGain > m_GetOTPCheckSpec.m_MaxGGGain) || (GGGain < m_GetOTPCheckSpec.m_MinGGGain))
	{
		Catstring(ErrorMsg, _T("GGGain %d not in<%d,%d>"),GGGain,m_GetOTPCheckSpec.m_MinGGGain,m_GetOTPCheckSpec.m_MaxGGGain);
		return FALSE;
	}	
   
    if (Typical_RG!= m_GetOTPCheckSpec.m_Typical_RG)
    {
        Catstring(ErrorMsg,_T("Typical_RG %d != %d"),Typical_RG,m_GetOTPCheckSpec.m_Typical_RG);
        return FALSE;
    }
    if (Typical_BG != m_GetOTPCheckSpec.m_Typical_BG)
    {
        Catstring(ErrorMsg,_T("Typical_BG %d != %d"),Typical_BG,m_GetOTPCheckSpec.m_Typical_BG);
        return FALSE;
    }
    if (Typical_GG != m_GetOTPCheckSpec.m_Typical_GG)
    {
        Catstring(ErrorMsg,_T("Typical_GG %d != %d"),Typical_GG,m_GetOTPCheckSpec.m_Typical_GG);
        return FALSE;
    }
   
    if (RGain > GGGain||Typical_RG > Typical_GG)
    {
        Catstring(ErrorMsg, _T("Typical Setting Fail,RG > GG"));
        return FALSE;
    }

    if (BGain > GGGain||Typical_BG > Typical_GG)
    {

        Catstring(ErrorMsg, _T("Typical Setting Fail,BG > GG"));
        return FALSE;
    }

	//check module info reserve	
	for (int i = 0x003D; i < 0x003F; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg, _T("AWB reserve 0x%04X is 0x%02X != 0xFF"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	//check AWB checksum
	int m_AWBSum = 0;
	for( int i = 0x0031; i < 0x003F; i++)
	{
		m_AWBSum += OTPReadData[i];
	}
	m_AWBSum = (m_AWBSum%0xFF);
    if (m_AWBSum != OTPReadData[0x003F])
	{
		Catstring(ErrorMsg,_T("AWB CheckSum 0x%02X != 0x%02X"),OTPReadData[0x003F],m_AWBSum);
		return FALSE;
	}		

	return TRUE;
} 

BOOL OtpCheckAPI::CheckAWB_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
    //IMX499
   
    int RGain		= ((OTPReadData[0x1001]<<8)&0xFF00) +OTPReadData[0x1002];
    int BGain    	= ((OTPReadData[0x1003]<<8)&0xFF00) +OTPReadData[0x1004];
    int GGGain   	= ((OTPReadData[0x1005]<<8)&0xFF00) +OTPReadData[0x1006];
    int RBGain		= int(1000.0*float(RGain)/float(BGain));
    int Typical_RG	= ((OTPReadData[0x1007]<<8)&0xFF00) +OTPReadData[0x1008];				
    int Typical_BG	= ((OTPReadData[0x1009]<<8)&0xFF00) +OTPReadData[0x100A];					
    int Typical_GG	= ((OTPReadData[0x100B]<<8)&0xFF00) +OTPReadData[0x100C];	


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

    if (OTPReadData[0x1000] != m_GetOTPCheckSpec1.m_AWBFlag)
    {
        Catstring(ErrorMsg, _T("AWBFlag 0x%02X != 0x%02X"),OTPReadData[0x1000],m_GetOTPCheckSpec1.m_AWBFlag);
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

    if (Typical_RG!= m_GetOTPCheckSpec1.m_Typical_RG)
    {
        Catstring(ErrorMsg, _T("Typical_RG %d != %d"),Typical_RG,m_GetOTPCheckSpec1.m_Typical_RG);
        return FALSE;
    }
    if (Typical_BG != m_GetOTPCheckSpec1.m_Typical_BG)
    {
        Catstring(ErrorMsg,_T("Typical_BG %d != %d"),Typical_BG,m_GetOTPCheckSpec1.m_Typical_BG);
        return FALSE;
    }
    if (Typical_GG != m_GetOTPCheckSpec1.m_Typical_GG)
    {
        Catstring(ErrorMsg,_T("Typical_GG %d != %d"),Typical_GG,m_GetOTPCheckSpec1.m_Typical_GG);
        return FALSE;
    }
    if (RGain > BGain)
    {
        if (Typical_RG < Typical_BG)
        {
            Catstring(ErrorMsg,_T("Typical Setting Fail,Typical_RG !> Typical_BG"));
            return FALSE;
        }	
    }
    else
    {
        if (Typical_RG > Typical_BG)
        {
            Catstring(ErrorMsg,_T("Typical Setting Fail,Typical_RG !< Typical_BG"));
            return FALSE;
        }
    }

    //check AWB checksum
    int m_AWBSum = 0;
    for (int i = 0x1001; i < 0x100D; i++)
    {
        m_AWBSum += OTPReadData[i];
    }
    m_AWBSum = (m_AWBSum%256);
    if (m_AWBSum != OTPReadData[0x100D])
    {
        Catstring(ErrorMsg,_T("AWB CheckSum 0x%02X != 0x%02X"),OTPReadData[0x100D],m_AWBSum);
        return FALSE;
    }

	return TRUE;
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
	USHORT LSC_RChannel[221] = {0};
	USHORT LSC_GrChannel[221] = {0};
	USHORT LSC_GbChannel[221] = {0};
	USHORT LSC_BChannel[221] = {0};

// 	int m_value = ((OTPReadData[0x0080]<<8)&0xFF00) +OTPReadData[0x0081];
// 	if (m_value != m_GetOTPCheckSpec.m_SegmentLSCTagID)
// 	{
// 		Catstring(ErrorMsg, _T("m_SegmentLSCTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentLSCTagID);
// 		return FALSE;
// 	}
	if (OTPReadData[0x0040] != m_GetOTPCheckSpec.m_LSCFlag)
	{
		Catstring(ErrorMsg, _T("m_LSCFlag 0x%02X != 0x%02X"),OTPReadData[0x0040],m_GetOTPCheckSpec.m_LSCFlag);
		return FALSE;
	}
    if (OTPReadData[0x0041] != m_GetOTPCheckSpec.m_LSCHighByte)
    {
        Catstring(ErrorMsg,_T("m_LSCHighByte 0x%02X != 0x%02X"),OTPReadData[0x0041],m_GetOTPCheckSpec.m_LSCHighByte);
        return FALSE;
    }
    if (OTPReadData[0x0042] != m_GetOTPCheckSpec.m_LSCLowByte)
    {
        Catstring(ErrorMsg,_T("m_LSCLowByte 0x%02X  != 0x%02X"),OTPReadData[0x0042],m_GetOTPCheckSpec.m_LSCLowByte);
        return FALSE;
    }

// 	if (OTPReadData[0x0083] != m_GetOTPCheckSpec.m_LSCTypeNumber)
// 	{
// 		Catstring(ErrorMsg, _T("m_LSCTypeNumber 0x%02X != 0x%02X"),OTPReadData[0x0083],m_GetOTPCheckSpec.m_LSCTypeNumber);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x0084] != m_GetOTPCheckSpec.m_LSCVersionNumber)
// 	{
// 		Catstring(ErrorMsg, _T("m_LSCVersionNumber 0x%02X != 0x%02X"),OTPReadData[0x0084],m_GetOTPCheckSpec.m_LSCVersionNumber);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x0085] != m_GetOTPCheckSpec.m_LSCMirror)
// 	{
// 		Catstring(ErrorMsg, _T("LSC Mirror 0x%02X != 0x%02X"),OTPReadData[0x0085],m_GetOTPCheckSpec.m_LSCMirror);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x0086] != m_GetOTPCheckSpec.m_LSCFilp)
// 	{
// 		Catstring(ErrorMsg, _T("LSC Filp 0x%02X != 0x%02X"),OTPReadData[0x0086],m_GetOTPCheckSpec.m_LSCFilp);
// 		return FALSE;
// 	}

	for (int i = 0; i < 221; i ++)
	{
		LSC_RChannel[i]  = ((OTPReadData[0x0043+2*i]<<8)&0xFF00)+(OTPReadData[0x0044+2*i]);
		LSC_GrChannel[i] = ((OTPReadData[0x01FD+2*i]<<8)&0xFF00)+(OTPReadData[0x01FE+2*i]);
		LSC_GbChannel[i] = ((OTPReadData[0x03B7+2*i]<<8)&0xFF00)+(OTPReadData[0x03B8+2*i]);
		LSC_BChannel[i]  = ((OTPReadData[0x0571+2*i]<<8)&0xFF00)+(OTPReadData[0x0572+2*i]);
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
		if (!LensVerificationLib(_T("R"), LSC_RChannel, ErrorMsg))   return FALSE;
		if (!LensVerificationLib(_T("Gr"),LSC_GrChannel,ErrorMsg))   return FALSE;
		if (!LensVerificationLib(_T("Gb"),LSC_GbChannel,ErrorMsg))   return FALSE;
		if (!LensVerificationLib(_T("B"),LSC_BChannel, ErrorMsg))   return FALSE;
	}

// 	check module info reserve	
// 		for (int i = 0x0770; i < 0x0E60; i++)
// 		{
// 			if (OTPReadData[i] != 0xFF)
// 			{
// 				Catstring(ErrorMsg, _T("LSC reserve 0x%04X is 0x%02X != 0xFF"),i,OTPReadData[i]);
// 				return FALSE;
// 			}
// 		}

    //check LSC reserve	
    for (int i = 0x072B; i < 0x08FF; i++)
    {
        if (OTPReadData[i] != 0xFF)
        {
            Catstring(ErrorMsg, _T("AWB reserve 0x%04X is 0x%02X != 0xFF"),i,OTPReadData[i]);
            return FALSE;
        }
    }

	// check LSC checksum
	int m_LSCSum = 0;
	for (int i = 0x0041; i<0x08FF; i++)
	{
		m_LSCSum += OTPReadData[i];
	}
	m_LSCSum = (m_LSCSum%0xFF);
	if (m_LSCSum != OTPReadData[0x08FF])
	{
		Catstring(ErrorMsg,_T("LSC CheckSum 0x%02X != 0x%02X"),OTPReadData[0x08FF],m_LSCSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommLSC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
    //IMX499

    if (OTPReadData[0x1016] != m_GetOTPCheckSpec1.m_LSCFlag)
    {
        Catstring(ErrorMsg, _T("m_LSCFlag 0x%02X != 0x%02X"),OTPReadData[0x1016],m_GetOTPCheckSpec1.m_LSCFlag);
        return FALSE;
    }
	USHORT LSC_RChannel[221]  = {0};
	USHORT LSC_GrChannel[221] = {0};
	USHORT LSC_GbChannel[221] = {0};
	USHORT LSC_BChannel[221]  = {0};

	for (int i = 0; i < 221; i++)
	{
		LSC_RChannel[i]  = ((OTPReadData[0x1017+8*i]<<8)&0xFF00)+(OTPReadData[0x1018+8*i]);
		LSC_GrChannel[i] = ((OTPReadData[0x1019+8*i]<<8)&0xFF00)+(OTPReadData[0x101A+8*i]);
		LSC_GbChannel[i] = ((OTPReadData[0x101B+8*i]<<8)&0xFF00)+(OTPReadData[0x101C+8*i]);
		LSC_BChannel[i]  = ((OTPReadData[0x101D+8*i]<<8)&0xFF00)+(OTPReadData[0x101E+8*i]);
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
		if (!LensVerificationLib(_T("R"), LSC_RChannel, ErrorMsg))   return FALSE;
		if (!LensVerificationLib(_T("Gr"),LSC_GrChannel,ErrorMsg))   return FALSE;
		if (!LensVerificationLib(_T("Gb"),LSC_GbChannel,ErrorMsg))   return FALSE;
		if (!LensVerificationLib(_T("B"),LSC_BChannel, ErrorMsg))   return FALSE;
	}

	// check LSC checksum
	int m_LSCSum = 0;
	for (int i = 0x1017; i < 0x16FF; i++)
	{
		m_LSCSum += OTPReadData[i];
	}
	m_LSCSum = (m_LSCSum%256);
// 	if (OTPReadData[0x0705] != 0x00)
// 	{
// 		Catstring(ErrorMsg, _T("LSC CheckSum 0x0705 is 0x%02X != 0x00"),OTPReadData[0x0705]);
// 		return FALSE;
// 	}
	if (m_LSCSum != OTPReadData[0x16FF])
	{
		Catstring(ErrorMsg,_T("LSC CheckSum 0x16FF is 0x%02X != 0x%02X"),OTPReadData[0x16FF],m_LSCSum);
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
    if (OTPReadData[0x0020] != m_GetOTPCheckSpec.m_AFFlag)
    {
        Catstring(ErrorMsg, _T("AFFlag: 0x%02X != 0x%02X"),OTPReadData[0x0020],m_GetOTPCheckSpec.m_AFFlag);
        return FALSE;
    }
    if (OTPReadData[0x0021] != m_GetOTPCheckSpec.m_AFCalDirection)
    {
        Catstring(ErrorMsg,_T("AF Calibration Direction 0x%02X != 0x%02X"),OTPReadData[0x0021],m_GetOTPCheckSpec.m_AFCalDirection);
        return FALSE;
    }
//	int b_SegmentAFTagID	= ((OTPReadData[0x0040]<<8)&0xFF00) + (OTPReadData[0x0041]&0xFF);
	int m_InfinityCode		= ((OTPReadData[0x0022]<<8)&0xFF00) + (OTPReadData[0x0023]&0xFF);
	int m_MacroCode			= ((OTPReadData[0x0024]<<8)&0xFF00) + (OTPReadData[0x0025]&0xFF);
// 	int m_PostDifA			= ((OTPReadData[0x004C]<<8)&0xFF00) + (OTPReadData[0x004D]&0xFF);
// 	int m_PostDifB			= ((OTPReadData[0x004E]<<8)&0xFF00) + (OTPReadData[0x004F]&0xFF);
// 	int m_PostDifC			= ((OTPReadData[0x0050]<<8)&0xFF00) + (OTPReadData[0x0051]&0xFF);
// 
 	int m_AFCodeDiff		= m_MacroCode - m_InfinityCode;     

	wchar_t InfinitySection[64] ={0};
	_itow(m_InfinityCode,InfinitySection,10);
	RegSetKey(_T("F_Auto"),_T("Three_in_one_InfCode"),InfinitySection);

	wchar_t MacroSection[64] ={0};
	_itow(m_MacroCode,MacroSection,10);
	RegSetKey(_T("F_Auto"),_T("Three_in_one_MacroCode"),MacroSection);

	//  check module have equal value 
	int m_AFCode8cmjudgevalue	= CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("MacroCodeEqualCount"),_T("MacroCodeEqualRef"),m_MacroCode,m_GetOTPCheckSpec.m_EqualCountSpec,ErrorMsg);
	if (!m_AFCode8cmjudgevalue)
	{
		return FALSE;
	}
	int m_AFCode500cmjudgevalue= CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("InfinityCodeEqualCount"),_T("InfinityCodeEqualRef"),m_InfinityCode,m_GetOTPCheckSpec.m_EqualCountSpec,ErrorMsg);
	if (!m_AFCode500cmjudgevalue)
	{
		return FALSE;
	}                                     

	if ((m_MacroCode > m_GetOTPCheckSpec.m_MaxMacroCode) || (m_MacroCode < m_GetOTPCheckSpec.m_MinMacroCode))
	{
		Catstring(ErrorMsg, _T("m_MacroCode %d not in<%d,%d>"),m_MacroCode,m_GetOTPCheckSpec.m_MinMacroCode,m_GetOTPCheckSpec.m_MaxMacroCode);
		return FALSE;
	}
	if ((m_InfinityCode > m_GetOTPCheckSpec.m_MaxInfinityCode) ||( m_InfinityCode < m_GetOTPCheckSpec.m_MinInfinityCode))
	{
		Catstring(ErrorMsg, _T("m_InfinityCode %d not in<%d,%d>"),m_InfinityCode,m_GetOTPCheckSpec.m_MinInfinityCode,m_GetOTPCheckSpec.m_MaxInfinityCode);
		return FALSE;
	}
	if ((m_AFCodeDiff > m_GetOTPCheckSpec.m_MaxCodeDiff) || (m_AFCodeDiff < m_GetOTPCheckSpec.m_MinCodeDiff))
	{
		Catstring(ErrorMsg, _T("AFCodeDiff %d not in<%d,%d>"),m_AFCodeDiff,m_GetOTPCheckSpec.m_MinCodeDiff,m_GetOTPCheckSpec.m_MaxCodeDiff);
		return FALSE;
	}

	//check AF reserved
	for (int i = 0x0026 ; i < 0x002F; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("AF Reserve 0x%04X is 0x%02X != 0xFF!"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	//check AF checksum
	int m_AFSum = 0;
	for (int i = 0x0021; i < 0x002F; i++)
	{
		m_AFSum += OTPReadData[i];
	}
	m_AFSum = (m_AFSum%0xFF);
	if (m_AFSum != OTPReadData[0x002F])
	{
		Catstring(ErrorMsg,_T("AF CheckSum 0x%02X != 0x%02X"),OTPReadData[0x002F],m_AFSum);
		return FALSE;
	}
	return TRUE;
}

BOOL OtpCheckAPI::CheckAFCode_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check AF	 IMX499
	int m_AFCode500cm		= ((OTPReadData[0x1011]<<8)&0xFF00) + (OTPReadData[0x1012]&0xFF);
	int m_AFCode8cm			= ((OTPReadData[0x100F]<<8)&0xFF00) + (OTPReadData[0x1010]&0xFF);
	int m_AFCodeDiff		= m_AFCode8cm - m_AFCode500cm;

	wchar_t InfinitySection[64] ={0};
	_itow(m_AFCode500cm,InfinitySection,10);
	RegSetKey(_T("F_Auto"),_T("Three_in_one_InfCode"),InfinitySection);

	wchar_t MacroSection[64] ={0};
	_itow(m_AFCode8cm,MacroSection,10);
	RegSetKey(_T("F_Auto"),_T("Three_in_one_MacroCode"),MacroSection);

	//  check module have equal value 
	int m_AFCode8cmjudgevalue	= CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("AFCode8cmEqualCount"),_T("AFCode8cmEqualRef"),m_AFCode8cm,m_GetOTPCheckSpec1.m_EqualCountSpec,ErrorMsg);
	if (!m_AFCode8cmjudgevalue)
	{
		return FALSE;
	}
	int m_AFCode500cmjudgevalue= CheckModuleEqualValue(STATIONCHECKITEM.GetBuffer(),_T("AFCode500cmEqualCount"),_T("AFCode500cmEqualRef"),m_AFCode500cm,m_GetOTPCheckSpec1.m_EqualCountSpec,ErrorMsg);
	if (!m_AFCode500cmjudgevalue)
	{
		return FALSE;
	}

	if (m_AFCode8cm > m_GetOTPCheckSpec1.m_MaxAFCode8cm || m_AFCode8cm < m_GetOTPCheckSpec1.m_MinAFCode8cm)
	{
		Catstring(ErrorMsg, _T("AFCode8cm %d not in<%d,%d>"),m_AFCode8cm,m_GetOTPCheckSpec1.m_MinAFCode8cm,m_GetOTPCheckSpec1.m_MaxAFCode8cm);
		return FALSE;
	}
	if (m_AFCode500cm > m_GetOTPCheckSpec1.m_MaxAFCode500cm || m_AFCode500cm < m_GetOTPCheckSpec1.m_MinAFCode500cm)
	{
		Catstring(ErrorMsg, _T("AFCode500cm %d not in<%d,%d>"),m_AFCode500cm,m_GetOTPCheckSpec1.m_MinAFCode500cm,m_GetOTPCheckSpec1.m_MaxAFCode500cm);
		return FALSE;
	}

    if (OTPReadData[0x100E] != m_GetOTPCheckSpec1.m_AFFlag)
    {
        Catstring(ErrorMsg, _T("AFFlag: 0x%02X != 0x%02X"),OTPReadData[0x100E],m_GetOTPCheckSpec1.m_AFFlag);
        return FALSE;
    }
    //check AF reserved
    for (int i = 0x1013 ; i < 0x1015; i++)
    {
        if (OTPReadData[i] != 0xFF)
        {
            Catstring(ErrorMsg,_T("AF Reserve 0x%04X is 0x%02X != 0xFF!"),i,OTPReadData[i]);
            return FALSE;
        }
    }

	//check AF checksum
	int m_AFSum = 0;
	for (int i = 0x100F; i < 0x1015; i++)
	{
		m_AFSum += OTPReadData[i];
	}
	m_AFSum = (m_AFSum%256);

	if (m_AFSum != OTPReadData[0x1015])
	{
		Catstring(ErrorMsg,_T("AF CheckSum 0x1015 is 0x%02X != 0x%02X"),OTPReadData[0x1015],m_AFSum);
		return FALSE;
	}

	return TRUE;
    }

BOOL OtpCheckAPI::CheckQualcommSPC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	// check Gain map
    if (OTPReadData[0x0904] != m_GetOTPCheckSpec.m_GainMapFlag)
    {
        Catstring(ErrorMsg, _T("m_ GainMapFlag 0x%02X != 0x%02X"),OTPReadData[0x0904],m_GetOTPCheckSpec.m_GainMapFlag);
        return FALSE;
    }

    if (OTPReadData[0x0905] != m_GetOTPCheckSpec.m_GainMapHighByte)
    {
        Catstring(ErrorMsg, _T("m_ GainMapHighByte 0x%02X != 0x%02X"),OTPReadData[0x0905],m_GetOTPCheckSpec.m_GainMapHighByte);
        return FALSE;
    }

    if (OTPReadData[0x0906] != m_GetOTPCheckSpec.m_GainMapLowByte)
    {
        Catstring(ErrorMsg, _T("m_ GainMapLowByte 0x%02X != 0x%02X"),OTPReadData[0x0906],m_GetOTPCheckSpec.m_GainMapLowByte);
        return FALSE;
    }
    
    if (((OTPReadData[0x0900])+ (OTPReadData[0x0901]<<8) + (OTPReadData[0x0902]<<16) +(OTPReadData[0x0903]<<24)) != m_GetOTPCheckSpec.m_PDAFToolVersion)
    {
        Catstring(ErrorMsg, _T("PDAFToolVersionis 0x%04X != 0x%04X"),((OTPReadData[0x0900])+ (OTPReadData[0x0901]<<8) + (OTPReadData[0x0902]<<16) +(OTPReadData[0x0903]<<24)),m_GetOTPCheckSpec.m_PDAFToolVersion);
        return FALSE;
    }
   
  

	int bPDAFVersionNum			= ((OTPReadData[0x0907]<<8)&0xFF00) + OTPReadData[0x0908];
	int bGainMapWidth			= ((OTPReadData[0x0909]<<8)&0xFF00) + OTPReadData[0x090A];
	int bGainMapHeight			= ((OTPReadData[0x090B]<<8)&0xFF00) + OTPReadData[0x090C];

// 	int m_value = ((OTPReadData[0x0E70]<<8)&0xFF00) +OTPReadData[0x0E71];
// 	if (m_value != m_GetOTPCheckSpec.m_SegmentPDAFTagID)
// 	{
// 		Catstring(ErrorMsg, _T("m_SegmentPDAFTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentPDAFTagID);
// 		return FALSE;
// 	}
// 	if (OTPReadData[0x070D] != m_GetOTPCheckSpec.m_PDAFFlag)
// 	{
// 		Catstring(ErrorMsg, _T("m_PDAFFlag 0x%02X != 0x%02X"),OTPReadData[0x070D],m_GetOTPCheckSpec.m_PDAFFlag);
// 		return FALSE;
// 	}

// 	if (OTPReadData[0x0E73] != m_GetOTPCheckSpec.m_PDAFTypeNumber)
// 	{
// 		Catstring(ErrorMsg, _T("m_PDAFTypeNumber 0x%02X != 0x%02X"),OTPReadData[0x0E73],m_GetOTPCheckSpec.m_PDAFTypeNumber);
// 		return FALSE;
// 	}
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
	for (int i = 0x090D;i < 0x0C81;i += 2)
	{
		m_GainMap = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];
		if ((m_GainMap > m_GetOTPCheckSpec.m_MaxGainMap) || (m_GainMap < m_GetOTPCheckSpec.m_MinGainMap))
		{
			Catstring(ErrorMsg,_T("GainMap[0x%04X~0x%04X]: %d,not in <%d,%d>"),i,i+1,m_GainMap,m_GetOTPCheckSpec.m_MinGainMap,m_GetOTPCheckSpec.m_MaxGainMap);
			return FALSE;
		}
	}

    // check PDAF checksum
    int m_PDAFSum  = 0;
    int i;
    for (i = 0x0905; i < 0x0C81; i++)
    {
        m_PDAFSum += OTPReadData[i];
    }
    m_PDAFSum = (m_PDAFSum%0xFF);
    if (m_PDAFSum != OTPReadData[0x0C81])
    {
        Catstring(ErrorMsg,_T("PDAF CheckSum 0x0C81 is 0x%02X != 0x%02X"),OTPReadData[0x0C81],m_PDAFSum);
        return FALSE;
    }

	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommSPC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
    // check Gain map	IMX499
    int bPDAFVersionNum1		= ((OTPReadData[0x1701]<<8)&0xFF00) + OTPReadData[0x1702];
    int bGainMapWidth1			= ((OTPReadData[0x1703]<<8)&0xFF00) + OTPReadData[0x1704];
    int bGainMapHeight1			= ((OTPReadData[0x1705]<<8)&0xFF00) + OTPReadData[0x1706];  

    if (OTPReadData[0x1700] != m_GetOTPCheckSpec1.m_PDAFFlag)
    {
        Catstring(ErrorMsg, _T("m_PDAFFlag 0x%02X != 0x%02X"),OTPReadData[0x1700],m_GetOTPCheckSpec1.m_PDAFFlag);
        return FALSE;
    }
    if (bPDAFVersionNum1 != m_GetOTPCheckSpec1.m_PDAFVersionNum)
    {
        Catstring(ErrorMsg, _T("PDAFVersionNumber: 0x%02X != 0x%02X"),bPDAFVersionNum1,m_GetOTPCheckSpec1.m_PDAFVersionNum);
        return FALSE;
    }
    if (bGainMapWidth1 != m_GetOTPCheckSpec1.m_GainMapWidth)
    {
        Catstring(ErrorMsg, _T("GainMapWidth: 0x%02X != 0x%02X"),bGainMapWidth1,m_GetOTPCheckSpec1.m_GainMapWidth);
        return FALSE;
    }	
    if (bGainMapHeight1 != m_GetOTPCheckSpec1.m_GainMapHeight)
    {
        Catstring(ErrorMsg, _T("GainMapHeight: 0x%02X != 0x%02X"),bGainMapHeight1,m_GetOTPCheckSpec1.m_GainMapHeight);
        return FALSE;
    }	

    int m_GainMap1 = 0;
    for (int i = 0x1707;i < 0x1A7B;i += 2)
    {
        m_GainMap1 = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];
        if ((m_GainMap1 > m_GetOTPCheckSpec1.m_MaxGainMap) || (m_GainMap1 < m_GetOTPCheckSpec1.m_MinGainMap))
        {
            Catstring(ErrorMsg,_T("GainMap1[0x%04X~0x%04X]: %d,not in <%d,%d>"),i,i+1,m_GainMap1,m_GetOTPCheckSpec1.m_MinGainMap,m_GetOTPCheckSpec1.m_MaxGainMap);
            return FALSE;
        }
    }
                                                                 
    // check PDAF checksum
    int m_PDAFSum1  = 0;
    int i;
    for (i = 0x1701; i < 0x1A7B; i++)
    {                                                  
        m_PDAFSum1 += OTPReadData[i];
    }
    m_PDAFSum1 = (m_PDAFSum1%256);
    if (m_PDAFSum1 != OTPReadData[0x1A7B])
    {
        Catstring(ErrorMsg,_T("PDAF CheckSum1 0x1A7B is 0x%02X != 0x%02X"),OTPReadData[0x1A7B],m_PDAFSum1);
        return FALSE;
    }

    return TRUE;

}

BOOL OtpCheckAPI::CheckSonySPC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check SPC(不烧录)
	for (int i = 0x070D; i < 0x076F; i++)
	{
		if (OTPReadData[i] != 0x00)
		{
			Catstring(ErrorMsg,_T("OGC0677不烧录SPC值，0x%04X值为0x%02X != 0x00!"),i,OTPReadData[i]);
			return FALSE;
		}
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
	if (OTPReadData[0x2000] != m_GetOTPCheckSpec.m_ArcsoftFlag)
	{
		Catstring(ErrorMsg,_T("Arcsoft Flag 0x2000 is 0x%02X != 0x%02X"),OTPReadData[0x2000],m_GetOTPCheckSpec.m_ArcsoftFlag);
		return FALSE;
	}
    int i;
    for (i = 0x2001;i < 0x2801;i++)
    {


        if (OTPReadData[i] == OTPReadData[0x2001])
        {
            Catstring(ErrorMsg,_T("Calibration Data all equality"));
            return FALSE;
        }                                                                       

        if (OTPReadData[i] != OTPReadData[0x2001])
            break;  
    }
    if(i>0x2800)
    {
        Catstring(ErrorMsg,_T("Calibration Data all equality"));
        return FALSE;
    }   
                                                            
	//Check Arcsoft checksum
	int ArcsoftChecksum = 0;
	for (int i = 0x2001;i < 0x2801;i++)
	{
		ArcsoftChecksum += OTPReadData[i];
	}
	ArcsoftChecksum = (ArcsoftChecksum%256);

	if (ArcsoftChecksum != OTPReadData[0x2801])
	{
		Catstring(ErrorMsg, _T("Arcsoft CheckSum 0x2801 is 0x%02X != 0x%02X"),OTPReadData[0x2801],ArcsoftChecksum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckLRC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
    if (OTPReadData[0x1AE4] != m_GetOTPCheckSpec.m_LRCFlag)
    {
        Catstring(ErrorMsg,_T("LRC Flag 0x2000 is 0x%02X != 0x%02X"),OTPReadData[0x1AE4],m_GetOTPCheckSpec.m_LRCFlag);
        return FALSE;
    }
    int i;
    for (i = 0x1AE5;i < 0x1B71;i++)
    {


        if (OTPReadData[i] == OTPReadData[0x1AE5])
        {
            Catstring(ErrorMsg,_T("Calibration Data all equality"));
            return FALSE;
        }                                                                       

        if (OTPReadData[i] != OTPReadData[0x1AE5])
            break;  
    }
    if(i>0x1B70)
    {
        Catstring(ErrorMsg,_T("Calibration Data all equality"));
        return FALSE;
    }   

    //Check Arcsoft checksum
    int LRCChecksum = 0;
    for (int i = 0x1AE5;i < 0x1B71;i++)
    {
       LRCChecksum += OTPReadData[i];
    }
    LRCChecksum = (LRCChecksum%256);

    if (LRCChecksum != OTPReadData[0x1B71])
    {
        Catstring(ErrorMsg, _T("LRC CheckSum 0x2801 is 0x%02X != 0x%02X"),OTPReadData[0x1B71],LRCChecksum);
        return FALSE;
    }

    //LRC reserve
    for (int i = 0x1B72; i<0x2000; i++)
           {
              if (OTPReadData[i] != 0xFF)
                {
                  Catstring(ErrorMsg,_T("LRC reserve[0x%04X]: 0x%02X != 0xFF!"),i,OTPReadData[i]);
                 return FALSE;
              }
            }

    return TRUE;
}

BOOL OtpCheckAPI::CheckCrossTalk_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	/*********************************************************************/
	/*                   CrossTalk check						         */
	/*********************************************************************/
	if (OTPReadData[0x072A] != m_GetOTPCheckSpec.m_CrossTalkFlag)
	{
		Catstring(ErrorMsg,_T("m_CrossTalkFlag is 0x%02X != 0x%02X"),OTPReadData[0x072A],m_GetOTPCheckSpec.m_CrossTalkFlag);
		return FALSE;
	}
	
	//Check CrossTalk checksum
	long CrossTalkChecksum = 0;
	for (int i = 0x072B;i < 0x0983;i++)
	{
		CrossTalkChecksum += OTPReadData[i];
	}
	CrossTalkChecksum = (CrossTalkChecksum%255) + 1;

	if (CrossTalkChecksum != OTPReadData[0x0983])
	{
		Catstring(ErrorMsg, _T("CrossTalk CheckSum 0x0983 is 0x%02X != 0x%02X"),OTPReadData[0x0983],CrossTalkChecksum);
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
	int bBrightnessRatio	= ((OTPReadData[0x2803]<<8)&0xFF00) + OTPReadData[0x2804];
	int bMainGain			= ((OTPReadData[0x2805]<<8)&0xFF00) + OTPReadData[0x2806];
	int bMainExposureTime	= ((OTPReadData[0x2807]<<8)&0xFF00) + OTPReadData[0x2808];
	int bSubGain			= ((OTPReadData[0x2809]<<8)&0xFF00) + OTPReadData[0x280A];
	int bSubExposureTime	= ((OTPReadData[0x280B]<<8)&0xFF00) + OTPReadData[0x280C];

// 	int m_value = ((OTPReadData[0x0E60]<<8)&0xFF00) +OTPReadData[0x0E61];
// 	if (m_value != m_GetOTPCheckSpec.m_SegmentAECTagID)
// 	{
// 		Catstring(ErrorMsg, _T("m_SegmentAECTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentAECTagID);
// 		return FALSE;
// 	}
	if (OTPReadData[0x2802] != m_GetOTPCheckSpec.m_AECFlag)
	{
		Catstring(ErrorMsg, _T("m_AECFlag 0x%02X != 0x%02X"),OTPReadData[0x2802],m_GetOTPCheckSpec.m_AECFlag);
		return FALSE;
	}
// 	if (OTPReadData[0x0E63] != m_GetOTPCheckSpec.m_AECVersionNumber)
// 	{
// 		Catstring(ErrorMsg, _T("AECVersionNumber: 0x%02X != 0x%02X"),OTPReadData[0x0E63],m_GetOTPCheckSpec.m_AECVersionNumber);
// 		return FALSE;
// 	}
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
	int m_AECSum = 0;
	for (int i = 0x2803; i < 0x280D; i++)
	{
		m_AECSum += OTPReadData[i];
	}
	m_AECSum = (m_AECSum%256);
	if (m_AECSum != OTPReadData[0x280D])
	{
		Catstring(ErrorMsg,_T("AEC CheckSum 0x%02X != 0x%02X"),OTPReadData[0x280D],m_AECSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckAEC_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	// check reserve
	for (int i = 0x1F72; i < 0x1F92; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("AEC reserve[%d]: %d != 0xFF!"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	// check SlaveCamera checksum
	int m_SlaveCameraSum  = 0;
	int m_SlaveCameraSum1 = 0;
	int m_SlaveCameraSum2 = 0;
	for (int i = 0x1739; i<0x1F92; i++)
	{
		m_SlaveCameraSum += OTPReadData[i];
	}
	m_SlaveCameraSum1 = ((m_SlaveCameraSum%1023) + 1)&0xFF;
	if (m_SlaveCameraSum1 != OTPReadData[0x1F92])
	{
		Catstring(ErrorMsg,_T("SlaveCamera CheckSum1 %d != %d"),m_SlaveCameraSum1,OTPReadData[0x1F92]);
		return FALSE;
	}
	m_SlaveCameraSum2 = ((m_SlaveCameraSum%1023) + 1) >> 8;
	if (m_SlaveCameraSum2 != OTPReadData[0x1F93])
	{
		Catstring(ErrorMsg,_T("SlaveCamera CheckSum2 %d != %d"),m_SlaveCameraSum2,OTPReadData[0x1F93]);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckOIS_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//No OIS
	for (int i = 0x1330; i < 0x1360; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("OIS Reserved [0x%04X]: 0x%02X != 0xFF"),i,OTPReadData[i]);
			return FALSE;
		}
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
	for (int i = 0x1363; i < 0x1B64; i++)
	{
		m_DualCamSum += OTPReadData[i];
	}
	m_DualCamSum = (m_DualCamSum%255) +1;
	if (m_DualCamSum != OTPReadData[0x1B64])
	{
		Catstring(ErrorMsg,_T("DualCam CheckSum 0x%02X != 0x%02X"),OTPReadData[0x1B64],m_DualCamSum);
		return FALSE;
	}

	// check reserve
	for (int i = 0x1B65; i < 0x1BF3; i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("DualCamera reserve[0x%04X]: 0x%02X != 0xFF!"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	m_value = ((OTPReadData[0x1BF3]<<8)&0xFF00) +OTPReadData[0x1BF4];
	if (m_value != m_GetOTPCheckSpec.m_SegmentCheckSumTagID)
	{
		Catstring(ErrorMsg, _T("SegmentCheckSumTagID 0x%04X != 0x%04X"),m_value,m_GetOTPCheckSpec.m_SegmentCheckSumTagID);
		return FALSE;
	}

	// check Total checksum
	int m_TotalSum = 0;
	for (int i = 0x0000; i < 0x1BF5; i++)
	{
		m_TotalSum += OTPReadData[i];
	}
	m_TotalSum = (m_TotalSum%255) +1;
	if (m_TotalSum != OTPReadData[0x1BF5])
	{
		Catstring(ErrorMsg,_T("Total CheckSum 0x%02X != 0x%02X"),OTPReadData[0x1BF5],m_TotalSum);
		return FALSE;
	}

	return TRUE;
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
    
    if (OTPReadData[0x0C82] != m_GetOTPCheckSpec.m_DCCFlag)
    {
        Catstring(ErrorMsg, _T("DCC Flag 0x0C82 is 0x%02X != 0x%02X"),OTPReadData[0x0C82],m_GetOTPCheckSpec.m_DCCFlag);
        return FALSE;
    }

    if (OTPReadData[0x0C83] != m_GetOTPCheckSpec.m_DCCMapHighByte)
    {
        Catstring(ErrorMsg, _T("DCCMapHighByte 0x%02X != 0x%02X"),OTPReadData[0x0C84],m_GetOTPCheckSpec.m_DCCMapHighByte);
        return FALSE;
    }
    if (OTPReadData[0x0C84] != m_GetOTPCheckSpec.m_DCCMapLowByte)
    {
        Catstring(ErrorMsg, _T("DCCMapLowByte 0x%02X  != 0x%02X"),OTPReadData[0x0C84],m_GetOTPCheckSpec.m_DCCMapLowByte);
        return FALSE;
    }

	int bDCCFormat				= ((OTPReadData[0x0C85]<<8)&0xFF00) + OTPReadData[0x0C86];
	int bDCCMapWidth			= ((OTPReadData[0x0C87]<<8)&0xFF00) + OTPReadData[0x0C88];
	int bDCCMapHeight			= ((OTPReadData[0x0C89]<<8)&0xFF00) + OTPReadData[0x0C8A];

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
	for (int i = 0x0C8B;i < 0x0CEB;i += 2)
	{
		m_DCCMap = ((OTPReadData[i+1]<<8)&0xFF00) + (OTPReadData[i]);
		if (m_DCCMap > m_GetOTPCheckSpec.m_MaxDCC || m_DCCMap < m_GetOTPCheckSpec.m_MinDCC)
		{
			Catstring(ErrorMsg,_T("DCCMap 0x%04X is %d not in <%d, %d>"),i,m_DCCMap,m_GetOTPCheckSpec.m_MinDCC,m_GetOTPCheckSpec.m_MaxDCC);
			return FALSE;
		}
	}

// 	if (OTPReadData[0x0B6C] != m_GetOTPCheckSpec.m_GainMapFlag)
// 	{
// 		Catstring(ErrorMsg, _T("GainMap Flag 0x0B6C is 0x%02X != 0x%02X"),OTPReadData[0x0B6C],m_GetOTPCheckSpec.m_GainMapFlag);
// 		return FALSE;
// 	}

	// check GainMap checksum
// 	int m_GainMapSum = 0;
// 	for (int i = 0x0700; i < 0x0B06; i++)
// 	{
// 		m_GainMapSum += OTPReadData[i];
// 	}
// 	m_GainMapSum = (m_GainMapSum%255);
// 	if (m_GainMapSum != OTPReadData[0x0B6D])
// 	{
// 		Catstring(ErrorMsg,_T("GainMap CheckSum 0x0B6D is 0x%02X != 0x%02X"),OTPReadData[0x0B6D],m_GainMapSum);
// 		return FALSE;
// 	}

	

	// check PDAF checksum
	int m_DCCSum = 0;
	for (int i = 0x0C83; i < 0x0CEB; i++)
	{
		m_DCCSum += OTPReadData[i];
	}
	m_DCCSum = (m_DCCSum%0xFF);
	if (m_DCCSum != OTPReadData[0x0CEB])
	{
		Catstring(ErrorMsg,_T(" DCC CheckSum 0x0CEB is 0x%02X != 0x%02X"),OTPReadData[0x0CEB],m_DCCSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckSonyPDAF_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//check DCC(不烧录)
	for (int i = 0x076F; i < 0x07D1; i++)
	{
		if (OTPReadData[i] != 0x00)
		{
			Catstring(ErrorMsg,_T("OGC0677不烧录DCC值，0x%04X值为0x%02X != 0x00!"),i,OTPReadData[i]);
			return FALSE;
		}
	}

	//Check Total CheckSum
	int m_TotalCheckSum = 0;
	for (int i = 0x0000; i < 0x07D1; i++)
	{
		m_TotalCheckSum += OTPReadData[i];
	}
	m_TotalCheckSum = (m_TotalCheckSum%255) + 1;
	if (OTPReadData[0x07D1] != 0x00)
	{
		Catstring(ErrorMsg, _T("Total CheckSum 0x07D1 is 0x%02X != 0x00"),OTPReadData[0x07D1]);
		return FALSE;
	}
	if (m_TotalCheckSum != OTPReadData[0x07D2])
	{
		Catstring(ErrorMsg,_T("Total CheckSum 0x07D2 is 0x%02X != 0x%02X"),OTPReadData[0x07D2],m_TotalCheckSum);
		return FALSE;
	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommPDAF_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
    //S5k2x7
	int bDCCFormat				= ((OTPReadData[0x0C85]<<8)&0xFF00) + OTPReadData[0x0C86];
	int bDCCMapWidth			= ((OTPReadData[0x0C87]<<8)&0xFF00) + OTPReadData[0x0C88];
	int bDCCMapHeight			= ((OTPReadData[0x0C89]<<8)&0xFF00) + OTPReadData[0x0C8A];

    if(OTPReadData[0x0C82] != m_GetOTPCheckSpec.m_DCCFlag)
    {
        Catstring(ErrorMsg, _T("m_DCCFlag: %d != %d"),m_GetOTPCheckSpec.m_DCCFlag,OTPReadData[0x0C82]);
        return FALSE;
    }
    if(OTPReadData[0x0C83] != m_GetOTPCheckSpec.m_DCCMapHighByte)
    {
        Catstring(ErrorMsg, _T("m_DCCMapHighByte: %d != %d"),m_GetOTPCheckSpec.m_DCCMapHighByte,OTPReadData[0x0C83]);
        return FALSE;
    } 
    if(OTPReadData[0x0C84] != m_GetOTPCheckSpec.m_DCCMapLowByte)
    {
        Catstring(ErrorMsg, _T("m_DCCMapLowByte: %d != %d"),m_GetOTPCheckSpec.m_DCCMapLowByte,OTPReadData[0x0C84]);
        return FALSE;
    }
	if (bDCCFormat != m_GetOTPCheckSpec.m_DCCFormat)
	{
		Catstring(ErrorMsg, _T("DCCFormat: 0x%02X != 0x%02X"),bDCCFormat,m_GetOTPCheckSpec.m_DCCFormat);
		return FALSE;
	}
	if (bDCCMapWidth != m_GetOTPCheckSpec.m_DCCMapWidth)
	{
		Catstring(ErrorMsg, _T("DCCMapWidth: 0x%02X != 0x%02X"),bDCCMapWidth,m_GetOTPCheckSpec.m_DCCMapWidth);
		return FALSE;
	}
	if (bDCCMapHeight != m_GetOTPCheckSpec.m_DCCMapHeight)
	{
		Catstring(ErrorMsg, _T("DCCMapHeight: 0x%02X != 0x%02X"),bDCCMapHeight,m_GetOTPCheckSpec.m_DCCMapHeight);
		return FALSE;
	}

	// check DCC map
	unsigned int m_DCCMap = 0;
	for (int i =0x0C8B;i<0x0CEB;i += 2)
	{
		m_DCCMap = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];

		if ((m_DCCMap > m_GetOTPCheckSpec.m_MaxDCC) || (m_DCCMap < m_GetOTPCheckSpec.m_MinDCC))
		{
			Catstring(ErrorMsg,_T("DCCMap[0x%04X~0x%04X]: %d,not in <%d,%d>"),i,i+1,m_DCCMap,m_GetOTPCheckSpec.m_MinDCC,m_GetOTPCheckSpec.m_MaxDCC);
			return FALSE;
		}
	}		

	// check DCC checksum
	
    int m_DCCSum = 0;
    for (int i = 0x0C83; i < 0x0CEB; i++)
    {
        m_DCCSum += OTPReadData[i];
    }
    m_DCCSum = (m_DCCSum%0xFF);
    if (m_DCCSum != OTPReadData[0x0CEB])
    {
        Catstring(ErrorMsg,_T(" DCC CheckSum 0x0CEB is 0x%02X != 0x%02X"),OTPReadData[0x0CEB],m_DCCSum);
        return FALSE;
    }

	// check reserve
// 	for (int i = 0x0C83; i<0x1000; i++)
// 	{
// 		if (OTPReadData[i] != 0xFF)
// 		{
// 			Catstring(ErrorMsg,_T("PDAF reserve[0x%04X]: 0x%02X != 0xFF!"),i,OTPReadData[i]);
// 			return FALSE;
// 		}
// 	}

	return TRUE;
}

BOOL OtpCheckAPI::CheckQualcommPDAF_Project2(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
    //IMX499
    int bDCCFormat1				= ((OTPReadData[0x1A7D]<<8)&0xFF00) + OTPReadData[0x1A7E];
    int bDCCMapWidth1			= ((OTPReadData[0x1A7F]<<8)&0xFF00) + OTPReadData[0x1A80];
    int bDCCMapHeight1			= ((OTPReadData[0x1A81]<<8)&0xFF00) + OTPReadData[0x1A82];

    if(OTPReadData[0x1A7C] != m_GetOTPCheckSpec1.m_DCCFlag)
    {
        Catstring(ErrorMsg, _T("m_DCCFlag: %d != %d"),m_GetOTPCheckSpec1.m_DCCFlag,OTPReadData[0x1A7C]);
        return FALSE;
    }
    if (bDCCFormat1 != m_GetOTPCheckSpec1.m_DCCFormat)
    {
        Catstring(ErrorMsg, _T("DCCFormat1: 0x%02X != 0x%02X"),bDCCFormat1,m_GetOTPCheckSpec1.m_DCCFormat);
        return FALSE;
    }
    if (bDCCMapWidth1 != m_GetOTPCheckSpec1.m_DCCMapWidth)
    {
        Catstring(ErrorMsg, _T("DCCMapWidth1: 0x%02X != 0x%02X"),bDCCMapWidth1,m_GetOTPCheckSpec1.m_DCCMapWidth);
        return FALSE;
    }
    if (bDCCMapHeight1 != m_GetOTPCheckSpec1.m_DCCMapHeight)
    {
        Catstring(ErrorMsg, _T("DCCMapHeight1: 0x%02X != 0x%02X"),bDCCMapHeight1,m_GetOTPCheckSpec1.m_DCCMapHeight);
        return FALSE;
    }

    // check DCC map
    unsigned int m_DCCMap1 = 0;
    for (int i =0x1A83;i<0x1AE3;i += 2)
    {
        m_DCCMap1 = ((OTPReadData[i]<<8)&0xFF00) + OTPReadData[i+1];

        if ((m_DCCMap1 > m_GetOTPCheckSpec1.m_MaxDCC) || (m_DCCMap1 < m_GetOTPCheckSpec1.m_MinDCC))
        {
            Catstring(ErrorMsg,_T("DCCMap[0x%04X~0x%04X]: %d,not in <%d,%d>"),i,i+1,m_DCCMap1,m_GetOTPCheckSpec1.m_MinDCC,m_GetOTPCheckSpec1.m_MaxDCC);
            return FALSE;
        }
    }		

    // check DCC checksum
    int m_DCCSum1 = 0;
    for (int i = 0x1A7D; i < 0x1AE3; i++)
    {
        m_DCCSum1 += OTPReadData[i];
    }
    m_DCCSum1 = (m_DCCSum1%256);
    if (m_DCCSum1 != OTPReadData[0x1AE3])
    {
        Catstring(ErrorMsg,_T("DCC CheckSum1 0x%02X != 0x%02X"),OTPReadData[0x1AE3],m_DCCSum1);
        return FALSE;
    }

//     // check reserve
//     for (int i = 0x0AF1; i<0x1000; i++)
//     {
//         if (OTPReadData[i] != 0xFF)
//         {
//             Catstring(ErrorMsg,_T("PDAF reserve[0x%04X]: 0x%02X != 0xFF!"),i,OTPReadData[i]);
//             return FALSE;
//         }
//     }

    return TRUE;

}

BOOL OtpCheckAPI::CheckOC_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
	//No OC
	for (int i = 0x0591;i < 0x05A7;i++)
	{
		if (OTPReadData[i] != 0xFF)
		{
			Catstring(ErrorMsg,_T("No Burned OC,OC data should be 0xFF!"));
			return FALSE;
		}
	}	

	return TRUE;
}

BOOL OtpCheckAPI::CheckOVPDAFStep_Project1(BYTE *OTPReadData, wchar_t *ErrorMsg)
{
    if (OTPReadData[0x0CEC] != m_GetOTPCheckSpec.m_OVPDAFStep)
    {
        Catstring(ErrorMsg, _T("OV PDAF Step1 Flag 0x%02X != 0x%02X"),OTPReadData[0x0CEC],m_GetOTPCheckSpec.m_OVPDAFStep);


        return FALSE;
    }
   
    int temp=0;
    for (int i=0;i<826;i++)
    {
    	if ((OTPReadData[0x0CED+i] == 0) || (OTPReadData[0x0CED+i] == 0xFF))
    	{
       		temp++;
       	}
    }
    if (temp>825)
    {
     	Catstring(ErrorMsg, _T("OV PDAF Step1全为0或0xFF"));
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
		SensorINI = GetModulePath() + _T("readbackvalue1.txt"); // 设定参数文档路径
	}
	else if (Flag==2)
	{
		SensorINI = GetModulePath() + _T("readbackvalue2.txt"); // 设定参数文档路径
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
     //S5k2x7
		/*********************************************************************/
		/*					获取项目1测试站位信息							 */
		/*********************************************************************/
        
        if ((StationName.Find(_T("Image"))) != -1)
        {
            m_GetOTPCheckSpec.m_AFCheck_En		 = 1;
            m_GetOTPCheckSpec.m_ModuleInfoCheck_En = 1;
            //	m_GetOTPCheckSpec.m_SegmentInfoCheck_En= 1;
            m_GetOTPCheckSpec.m_AWBLSCCheck_En	 = 1;
            m_GetOTPCheckSpec.m_SPCCheck_En		 = 1;
            m_GetOTPCheckSpec.m_PDAFCheck_En		 = 1;
            m_GetOTPCheckSpec.m_OVPDAFStep_En      = 1;
        }
        else  if ((StationName.Find(_T("PDAF"))) != -1)
        {
            m_GetOTPCheckSpec.m_AFCheck_En		 = 1;
            m_GetOTPCheckSpec.m_ModuleInfoCheck_En = 1;
            //  m_GetOTPCheckSpec.m_SegmentInfoCheck_En= 1;
            m_GetOTPCheckSpec.m_AWBLSCCheck_En	 = 1;
            m_GetOTPCheckSpec.m_SPCCheck_En		 = 1;
            m_GetOTPCheckSpec.m_PDAFCheck_En		 = 1;
            m_GetOTPCheckSpec.m_OVPDAFStep_En      = 1;

        }
		else if ((StationName.Find(_T("OTP"))) != -1)
		{
			m_GetOTPCheckSpec.m_AFCheck_En		 = 1;
			m_GetOTPCheckSpec.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec.m_SPCCheck_En		 = 1;
            m_GetOTPCheckSpec.m_OVPDAFStep_En      = 1;
		}
        else if((StationName.Find(_T("AF"))) != -1)
            {
                m_GetOTPCheckSpec.m_AFCheck_En		 = 1;
            }
		else
		{
			wsprintf(szLog, _T("\n 无%s试站位，%s站位名填写错误!"), StationName, TESTSTATION);
			Catstring(ErrorMsg, szLog);
			SAFE_DELETE_ARRAY(OTPReadData);
			return FALSE;
		}

		/*********************************************************************/
		/*              Check module Information			                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec.m_ModuleInfoCheck_En == 1)
		{
			m_OTPCheckResult = CheckModuleInfo_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s ModuleInfo烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}
        /*********************************************************************/
        /*							Check AF				                 */
        /*********************************************************************/
        if (m_GetOTPCheckSpec.m_AFCheck_En == 1)
        {
            m_OTPCheckResult = CheckAFCode_Project1(OTPReadData,ErrorMsg);
            if (!m_OTPCheckResult)
            {
                wsprintf(szLog, _T("\n %s AF烧录错误!"), TESTSTATION);
                Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
                return FALSE;
            }
        }

		/*********************************************************************/
		/*              Check Segment Information			                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec.m_SegmentInfoCheck_En == 1)
		{
			m_OTPCheckResult = CheckSegmentInfo_Project1(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s SegmentInfo烧录错误!"), TESTSTATION);
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
        /*							Check OV PDAF STEP				                 */
        /*********************************************************************/
        if (m_GetOTPCheckSpec.m_OVPDAFStep_En == 1)
        {
            m_OTPCheckResult = CheckOVPDAFStep_Project1(OTPReadData,ErrorMsg);
            if (!m_OTPCheckResult)
            {
                wsprintf(szLog, _T("\n %s OV PDAF STEP 全部为0或0xFF!"), TESTSTATION);
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
        /*						LRC check							         */
        /*********************************************************************/		
        if (m_GetOTPCheckSpec.m_LRCCheck_En == 1)
        {
            m_OTPCheckResult = CheckLRC_Project1(OTPReadData,ErrorMsg);
            if (!m_OTPCheckResult)
            {
                wsprintf(szLog, _T("\n %s LRC烧录错误!"), TESTSTATION);
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
		}
	}

	else if (Flag == 2)
	{	
		if (m_GetOTPCheckSpec1.m_OffLineCheck_En)
		{
			GetDataFromTxt(OTPReadData,1);
		}
		else
		{
			memcpy(OTPReadData,InputOTPData,m_OTPSize);
		}

		if (m_GetOTPCheckSpec1.m_SaveOTPCheckData)
		{
			if (!SaveOTPData(OTPReadData,ErrorMsg,1))
			{
				wsprintf(szLog, _T("\n %s保存Eeprom数据失败!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}			
		}
		//S5k2x7
		/*********************************************************************/
		/*					获取项目1测试站位信息							 */
		/*********************************************************************/

		if ((StationName.Find(_T("Image"))) != -1)
		{
			m_GetOTPCheckSpec1.m_AFCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_ModuleInfoCheck_En = 1;
			//	m_GetOTPCheckSpec.m_SegmentInfoCheck_En= 1;
			m_GetOTPCheckSpec1.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec1.m_SPCCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_PDAFCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_OVPDAFStep_En      = 1;
		}
		else  if ((StationName.Find(_T("PDAF"))) != -1)
		{
			m_GetOTPCheckSpec1.m_AFCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_ModuleInfoCheck_En = 1;
			//  m_GetOTPCheckSpec.m_SegmentInfoCheck_En= 1;
			m_GetOTPCheckSpec1.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec1.m_SPCCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_PDAFCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_OVPDAFStep_En      = 1;

		}
		else if ((StationName.Find(_T("OTP"))) != -1)
		{
			m_GetOTPCheckSpec1.m_AFCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_ModuleInfoCheck_En = 1;
			m_GetOTPCheckSpec1.m_AWBLSCCheck_En	 = 1;
			m_GetOTPCheckSpec1.m_SPCCheck_En		 = 1;
			m_GetOTPCheckSpec1.m_OVPDAFStep_En      = 1;
		}
		else if((StationName.Find(_T("AF"))) != -1)
		{
			m_GetOTPCheckSpec1.m_AFCheck_En		 = 1;
		}
		else
		{
			wsprintf(szLog, _T("\n 无%s试站位，%s站位名填写错误!"), StationName, TESTSTATION);
			Catstring(ErrorMsg, szLog);
			SAFE_DELETE_ARRAY(OTPReadData);
			return FALSE;
		}

		/*********************************************************************/
		/*              Check module Information			                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec1.m_ModuleInfoCheck_En == 1)
		{
			m_OTPCheckResult = CheckModuleInfo_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s ModuleInfo烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}
		/*********************************************************************/
		/*							Check AF				                 */
		/*********************************************************************/
		if (m_GetOTPCheckSpec1.m_AFCheck_En == 1)
		{
			m_OTPCheckResult = CheckAFCode_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s AF烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*                       AWB & LSC check				             */
		/*********************************************************************/
		if (m_GetOTPCheckSpec1.m_AWBLSCCheck_En == 1)
		{
			m_OTPCheckResult = CheckAWB_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s AWB烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}

			m_OTPCheckResult = CheckQualcommLSC_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s LSC烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}

		/*********************************************************************/
		/*						SPC check							         */
		/*********************************************************************/		
		if (m_GetOTPCheckSpec1.m_SPCCheck_En == 1)
		{
			m_OTPCheckResult = CheckQualcommSPC_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog,_T("\n %s SPC烧录错误!"), TESTSTATION);
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
			m_OTPCheckResult = CheckQualcommPDAF_Project2(OTPReadData,ErrorMsg);
			if (!m_OTPCheckResult)
			{
				wsprintf(szLog, _T("\n %s PDAF烧录错误!"), TESTSTATION);
				Catstring(ErrorMsg, szLog);
				SAFE_DELETE_ARRAY(OTPReadData);
				return FALSE;
			}
		}
	}
	else
	{
		wsprintf(szLog, _T("主副摄设置错误!"));
		Catstring(ErrorMsg, szLog);
		SAFE_DELETE_ARRAY(OTPReadData);
		return FALSE;
    }

	return TRUE;
}