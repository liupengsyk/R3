#include "StdAfx.h"
#include "TestItemInterface.h"


TestItemInterface::TestItemInterface(void)
{
	m_Result = RESULT_NULL;
	m_Name = _T("");
	m_configPathName = _T("");
	m_Type = TYPE_NULL;
	m_hOptionDlg = NULL;
	m_bSave = TRUE;   //是否存储的开关	 
	CamID = 0;
	bStopTest = FALSE;
	error_Code = _T("");
}


TestItemInterface::~TestItemInterface(void)
{
	if(m_hOptionDlg)
	{
		::DestroyWindow(m_hOptionDlg);
	}
}
 
int TestItemInterface::InitPrivateMen()
{
	m_Result = RESULT_NULL;
	m_Name = _T("");
	m_configPathName = _T("");
	m_Type = TYPE_NULL;
	m_hOptionDlg = NULL;
	m_bSave = TRUE;   //是否存储的开关	 
	CamID = 0;
	bStopTest = FALSE;
	error_Code = _T("");
	return 0;
}
 int TestItemInterface::InitItem() 
{
	return 0;
}
int TestItemInterface::Initialize()
{
	return 0;
}
int TestItemInterface::Finalize()
{
	return 0;
}
int TestItemInterface::LoadOption()
{
	int ret = SendMessage(m_hOptionDlg,WM_UPDATE_INSPECITON_OPTION,0,0);
	return ret;
}
int TestItemInterface::SaveOption()
{
	int ret = SendMessage(m_hOptionDlg,WM_UPDATE_INSPECITON_OPTION,1,0);
	return ret;
}
void TestItemInterface::SetType(int itype)
{
	m_Type = itype;
}
void TestItemInterface::SetName(LPCTSTR name)
{	 
	m_Name = name;
}

void TestItemInterface::SetErrorCode(CString error_Code)
{
	 this->error_Code = error_Code;
}
LPCTSTR TestItemInterface::GetErrorCode()
{
	return error_Code;
}

void TestItemInterface::SetOptionDlgHandle(HWND OptionHwnd)
{
	m_hOptionDlg = OptionHwnd;
}
void TestItemInterface::SetResult(int Result)
{
	m_Result = Result;
}
void TestItemInterface::SetSave(BOOL bSave)
{
	m_bSave = bSave;
}
int TestItemInterface::GetResult()
{
	return m_Result;
}
HWND TestItemInterface::GetOptionDialogHandle()
{
	return m_hOptionDlg;
}
int TestItemInterface::GetType()
{
	return m_Type;
}
BOOL TestItemInterface::isSaved()
{
	return m_bSave;
}
LPCTSTR TestItemInterface::GetName()
{
	return (LPCTSTR)m_Name;
}

int TestItemInterface::GetCamID()
{
	return CamID;
}
void TestItemInterface::SetCamID(int CamID)
{
	this->CamID = CamID;
}
void TestItemInterface::SetConfigPath(LPCTSTR ConfigPathName)
{
	m_configPathName = ConfigPathName;
}
int TestItemInterface::ReadConfigInt(LPCTSTR KeyName,int int_default)
{
	return GetPrivateProfileInt(m_Name,KeyName,int_default,m_configPathName);
}
CString TestItemInterface::ReadConfigString(LPCTSTR keyName,LPCTSTR strDefault,int length)
{
	CString strTemp;
    GetPrivateProfileString(m_Name,keyName,strDefault,strTemp.GetBuffer(length),length,m_configPathName);
	strTemp.ReleaseBuffer();
	return strTemp;

}
void TestItemInterface::WriteConfigInt(LPCTSTR KeyName,int data,int datatype)
{
	CString szTemp;
	switch(datatype)
	{
	case DATA_DEC:
		szTemp.Format(_T("%d"),data);
		break;
	case DATA_HEX_WORD:
		szTemp.Format(_T("0x%04x"),data);
		break;
	case DATA_HEX_BYTE:
		szTemp.Format(_T("0x%02x"),data);
		break;
	case DATA_HEX_DWORD:
		szTemp.Format(_T("0x%x"),data);
		break;
	}
	WritePrivateProfileString(m_Name,KeyName,szTemp,m_configPathName);
}
void TestItemInterface::WriteConfigString(LPCTSTR KeyName,LPCTSTR strDefault)
{
	WritePrivateProfileString(m_Name,KeyName,strDefault,m_configPathName);
}

void TestItemInterface::Config_ClearConfig(LPCTSTR AppName)
{
	WritePrivateProfileSection(AppName,0,m_configPathName);
}

 LPCTSTR TestItemInterface::GetReportHeads() 
{
	return NULL;
}
 LPCTSTR TestItemInterface::GetReportHightLimit()
{
	return NULL;
}
 LPCTSTR TestItemInterface::GetReportLowLimit()
{
	return NULL;
}
 LPCTSTR TestItemInterface::GetReportContents()
{
	return NULL;
}
  
 BOOL TestItemInterface::GetbStopTest()
 {
	 return bStopTest;
 }
 void TestItemInterface::SetbStopTest(BOOL bStop)
 {
	 this->bStopTest = bStop;
 }

 void TestItemInterface::ConvertToOtpData(USHORT* dataInput,vector<BYTE> *otpDataOut)
{
	USHORT* DataInput = dataInput;
	BYTE bits = m_otpDataFormat.bits;
	BOOL bHigh8 = m_otpDataFormat.bHigh8;
	BOOL bHightFirst=m_otpDataFormat.bHightFirst;
	BOOL unit_byteH_L=m_otpDataFormat.unit_byteH_L;
	int  len1 = m_otpDataFormat.len-1;
	int shift_bit = bits-8;
	BYTE second = 0;
	 
	if(len1==0)
	{
		otpDataOut->push_back((BYTE)DataInput[0]);
		return;
	}

	BYTE* hight8 = new BYTE[len1];
	BYTE* low8 = new BYTE[len1]; 
	
	 

	//数据分割：
	if(bHigh8)//如果是高八位，分别存放高八位和低八位数据
	{ 
		for(int i=0;i<len1;i++)
		{
			hight8[i] = DataInput[i]>>shift_bit; 
			low8[i] = DataInput[i]&(0xff>>(8-shift_bit)); 
		} 
		
	}
	else
	{
		for(int i=0;i<len1;i++)
		{
			hight8[i] = DataInput[i]>>8;
			low8[i] = DataInput[i]&0xff; 
		}
	}

	//数据存放
	if(bHightFirst)
	{
		for(int i=0;i<len1;i++)
		{ 
			otpDataOut->push_back(hight8[i]);
			if(unit_byteH_L)
			{
				second = (second<<shift_bit)|low8[i];//前面数据的低位放在unit byte的高位
			}
			else
			{
				second = (second<<shift_bit)|low8[len1-i-1];//前面数据的低位放在unit byte的低位
			} 
		} 
	}
	else
	{
		for(int i=0;i<len1;i++)
		{  
			otpDataOut->push_back(low8[i]);
			if(unit_byteH_L)
			{
				second = (second<<shift_bit)|hight8[i];//前面数据的低位放在unit byte的高位
			}
			else
			{
				second = (second<<shift_bit)|hight8[len1-i-1];//前面数据的低位放在unit byte的低位
			}
		}
		
		
	}
	 otpDataOut->push_back(second);

	//释放数据
	 
	SAFE_DELETE_ARRAY(hight8);
	SAFE_DELETE_ARRAY(low8);
}

 void TestItemInterface::ConvertToItemData(BYTE* dataInput,vector<USHORT> *ItemDataOut)
 {
	 BYTE* DataInput = dataInput;
	 BYTE bits = m_otpDataFormat.bits;
	 BOOL bHigh8 = m_otpDataFormat.bHigh8;
	 BOOL bHightFirst=m_otpDataFormat.bHightFirst;
	 BOOL unit_byteH_L=m_otpDataFormat.unit_byteH_L;
	 int  ulen = m_otpDataFormat.len-1;
	 int shift_bit = bits-8;
	 BYTE second = 0;

	 if(ulen==0)
	 {
		 ItemDataOut->push_back(dataInput[0]);
		 return ;
	 }

	 BYTE* hight8 = new BYTE[ulen];
	 BYTE* low8 = new BYTE[ulen]; 

	 //取出高低位
	 if(bHightFirst)
	 {
		 for(int i=0;i<ulen;i++)
		 {  
			 hight8[i] = dataInput[i];
			 if(unit_byteH_L)
			 {
				 low8[ulen-i-1] = (dataInput[ulen]>>(shift_bit*i))&(0xff>>(8-shift_bit)); //从低位开始取，
			 }
			 else
			 {
				 low8[i] = (dataInput[ulen]>>(shift_bit*i))&(0xff>>(8-shift_bit));
			 } 
		 } 
	 }
	 else
	 {
		 for(int i=0;i<ulen;i++)
		 {  
			 low8[i] = dataInput[i];
			 if(unit_byteH_L)
			 {
				 hight8[ulen-i-1] = (dataInput[ulen]>>(shift_bit*i))&(0xff>>(8-shift_bit)); //从低位开始取，
			 }
			 else
			 {
				 hight8[i] = (dataInput[ulen]>>(shift_bit*i))&(0xff>>(8-shift_bit)); //从低位开始取，
			 }
		 }
	 }

	 //组成ushort data
	 if(bHigh8)//如果是高八位，分别存放高八位和低八位数据
	 { 
		 USHORT uData = 0;
		 for(int i=0;i<ulen;i++)
		 {
			 uData = (hight8[i]<<shift_bit)|low8[i]; 
			 ItemDataOut->push_back(uData);
		 } 
	 }
	 else
	 {
		 USHORT uData = 0;
		 for(int i=0;i<ulen;i++)
		 {
			 uData = (hight8[i]<<8)|low8[i]; 
			 ItemDataOut->push_back(uData);
		 } 
	 } 
 }
 void TestItemInterface::GetOtp_CheckSum(vector<BYTE> otpData,BYTE Sumtype,USHORT &sum)
{ 
	 LONG Lsum = 0;
	 sum = 0;
	for(UINT i = 0;i<otpData.size();i++)
	{
		Lsum += otpData[i];
	}
	switch(Sumtype)
	{
	case 0:
		sum = Lsum%255+1;
		break;
	case 1:
		sum = Lsum%255;
		break;
	case 2:
		sum = Lsum;
		break;
	case 3:
		sum = Lsum%256;
		break;
	case 4:
		sum = Lsum%65535;
		break;
	}  
}

 void TestItemInterface::SetOtpDataFormat(otpDataFormat m_otpDataFormat)
 {
	 this->m_otpDataFormat = m_otpDataFormat;
 }
 void TestItemInterface::GetOtpDataFormat(otpDataFormat &m_otpDataFormat)
 {
	 m_otpDataFormat = this->m_otpDataFormat;
 }
 BOOL TestItemInterface::RegReadKey(wchar_t *SubReg,wchar_t *SubKeyName,wchar_t *keyValue)
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
		 SAFE_DELETE_ARRAY(SubRegName);
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
	 SAFE_DELETE_ARRAY(lpData);

	 SAFE_DELETE_ARRAY(SubRegName);
	 return true;
 }

 BOOL TestItemInterface::RegSetKey(wchar_t *SubReg,wchar_t *SubKeyName,wchar_t *keyValue)
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
	 SAFE_DELETE_ARRAY(SubRegName);
	 return true;
 }
 int  TestItemInterface::GetTimeTag()
 {
	 int Hour;
	 time_t nowtime;
	 time(&nowtime);
	 tm *_time;

	 _time=localtime(&nowtime);
	 Hour = (_time->tm_yday)*24 + _time->tm_hour;
	 return Hour;
 }
 void TestItemInterface::MIPIP10ToRaw8(USHORT * pIn, BYTE * pOut, long number)
 {
	 int num = 0;
	 for (long i=0; i<number; i += 4)
	 {
		 pOut[num++] = (pIn[i+0]>>2) & 0xff;
		 pOut[num++] = (pIn[i+1]>>2) & 0xff;
		 pOut[num++] = (pIn[i+2]>>2) & 0xff;
		 pOut[num++] = (pIn[i+3]>>2) & 0xff;
	 }
 }

