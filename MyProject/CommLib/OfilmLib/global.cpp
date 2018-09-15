#include   <afx.h>
#define   _AFXDLL
#ifndef _DLLCLASS
#define _DLLCLASS
#endif

#include "stdafx.h"
#include "global.h"
#include "DllDialog.h"
#include "DNPIndexDlg.h"
#include "UnlockDlg.h"
#include "MesLogin.h"
#include "dtusb.h"
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <windows.h>
#include <MATH.H>
#include "MD5Checksum.h"
#include "Syunew3D.h"
#include "winsock2.h" 
#include <Nb30.h>   
#include  <wininet.h>
#include <IO.H>
#include <VECTOR>
CMesLogin m_Login;
HINTERNET hSession;
HINTERNET hConnection;
#define RELEASE(a) (((a)!=0) ? (delete[] (a),(a) = 0):((a)=0))

#pragma comment(lib,"Syunew3D.lib")
#pragma comment( lib, "Wininet.lib" )
#pragma comment(lib,"version.lib")
#pragma comment (lib, "DTUsb.lib")
#pragma comment(lib,"ws2_32.lib")   
#pragma comment(lib,"netapi32.lib") 

char TestDataSave[16384];

BOOL FtpConnect()
{
	hSession =InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	
	CString strHostName="192.168.58.65";
	CString strUser="CCM-SW";
	CString strPwd="liangwan100zl";
	
	hConnection = InternetConnect(hSession,strHostName,INTERNET_DEFAULT_FTP_PORT,strUser,strPwd,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
	if (hConnection==0)
	{
		return FALSE;
	}
	return TRUE;
	
}
void GetLocalIp(char *szIp)
{
	WSADATA wsaData;
	char szHostName[256]; 
	char *szTmp;
	PHOSTENT HostInfo; 
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 ) 
	{  
		if( gethostname (szHostName, sizeof(szHostName)) == 0) 
		{
			if((HostInfo = gethostbyname(szHostName)) != NULL) 
			{ 
				szTmp = inet_ntoa (*(struct in_addr *)*HostInfo->h_addr_list); 
				
			} 
		} 
		WSACleanup();         
	} 	
	strcpy(szIp,szTmp);
}
void GetModulePath(char *FilePath)
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	char ModulePath[128] = {0};
	char path[128]={0};
	GetModuleFileName(NULL, ModulePath, 128);
	len = strlen(ModulePath);
	for (i = len - 1; i >= 0; i--)
	{
		if (ModulePath[i] == '\\')
		{
			ModulePath[i + 1] = 0;
			flag = true;
			strcpy(path, ModulePath);	
			break;
		}
	}
	
	if (!flag)
	{
		strcpy(path, "");
		
	}
	
	strcpy(FilePath,path);
	
	return;

}

void GetLocalMac(char *szMac)
{
	NCB ncb;     
	
	typedef struct _ASTAT_    
	{
		ADAPTER_STATUS   adapt; 
		NAME_BUFFER   NameBuff   [30];     
	}ASTAT, *PASTAT;
	ASTAT Adapter;   
	
	typedef struct _LANA_ENUM   
	{
		UCHAR length; 
		UCHAR lana[MAX_LANA];   
	}LANA_ENUM;     
	LANA_ENUM lana_enum;   
	
	//   取得网卡信息列表     
	UCHAR uRetCode;     
	memset(&ncb, 0, sizeof(ncb));     
	memset(&lana_enum, 0, sizeof(lana_enum));     
	
	ncb.ncb_command = NCBENUM;     
	ncb.ncb_buffer = (unsigned char *)&lana_enum; 
	ncb.ncb_length = sizeof(LANA_ENUM);   
	
	uRetCode = Netbios(&ncb);     
	if(uRetCode != NRC_GOODRET)     
		return ;     
	
	
	for(int lana=0; lana<lana_enum.length; lana++)     
	{
		ncb.ncb_command = NCBRESET;   
		ncb.ncb_lana_num = lana_enum.lana[lana]; 
		uRetCode = Netbios(&ncb);   
	} 
	if(uRetCode != NRC_GOODRET)
		return ;     
    
	
	memset(&ncb, 0, sizeof(ncb)); 
	ncb.ncb_command = NCBASTAT;    
	ncb.ncb_lana_num = lana_enum.lana[0];     
	strcpy((char*)ncb.ncb_callname, "*");     
	ncb.ncb_buffer = (unsigned char *)&Adapter; 
	ncb.ncb_length = sizeof(Adapter);
	
	uRetCode = Netbios(&ncb); 
	
	if(uRetCode != NRC_GOODRET)   
		return ;   
	
	sprintf(szMac,"%02X-%02X-%02X-%02X-%02X-%02X",     
		Adapter.adapt.adapter_address[0],     
		Adapter.adapt.adapter_address[1],     
		Adapter.adapt.adapter_address[2],     
		Adapter.adapt.adapter_address[3],     
		Adapter.adapt.adapter_address[4],     
		Adapter.adapt.adapter_address[5] 
		); 
}
int ShowInputPwbBox(char *cWndText,char *cShowText,char *ReturnText) 
{
	CUnlockDlg InputDlg;
	InputDlg.cWndText=cWndText;
	InputDlg.cShowText=cShowText;
	InputDlg.ReturnText=ReturnText;
	InputDlg.DoModal();
	return InputDlg.Res;
}

int  GetTimeTag()
{
	int Hour;
	time_t nowtime;
	time(&nowtime);
	tm *_time;
	
	_time=localtime(&nowtime);
	Hour = (_time->tm_yday)*24 + _time->tm_hour;
	return Hour;
}


void  DebugInfo(char * format,...)
{	
	char buf[2048];	
	va_list ap;
    va_start(ap, format);
	
	int len=vsprintf(buf,format,ap);
    buf[len]=0;
	char Budout[4096];
	strcpy(Budout,"Ofilm->");
	strcat(Budout,buf);
	OutputDebugString(Budout);
	va_end(ap);
	char FileContent[16384]={0};
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);
	
	char TimeTemp[64]={0};
	char FileNameTemp[64]={0};
	CreateDirectory("TempLog",NULL);
	sprintf(FileNameTemp,"TempLog\\DebugInfo_%04d%02d%02d_%02d.log",NowTime.wYear,NowTime.wMonth,NowTime.wDay,NowTime.wHour);
	
	sprintf(TimeTemp,"%04d-%02d-%02d %02d:%02d:%03d",NowTime.wYear,NowTime.wMonth,NowTime.wDay,NowTime.wHour,NowTime.wMinute,NowTime.wMilliseconds);
	FILE *pFIle=fopen(FileNameTemp,"a");
	strcpy(FileContent,TimeTemp);
	strcat(FileContent,"   ");
	strcat(FileContent,buf);
	strcat(FileContent,"\n");
	fwrite(FileContent,1,strlen(FileContent),pFIle);
	fclose(pFIle);
}

bool fSleep(unsigned short uiMillsecond)
{
	DWORD uiStartTick	=	GetTickCount();
	while(true)
	{
		DWORD uiNowTick	=	GetTickCount();
		if ((uiNowTick-uiStartTick) >= uiMillsecond)
		{
			break;
		}
		
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return true;
}

struct TestInfoHeader
{
	char SenserID[128];
	char ModuleID[128];
	char Result[128];
	int	 StationID;
	char StationName[128];
	int  LineID;
	int	 RouteSeq;//途程中的序号
};

bool GetFileVersionInfoLib(char *FileName,char *VersionInfo,int *dwSize)
{
	DWORD dwLen = 0;
	char* lpData=NULL;
	BOOL bSuccess = FALSE;

	dwLen = GetFileVersionInfoSize(FileName,0);
	if (0 == dwLen)
	{
		strcpy(VersionInfo,"获得版本信息失败.");
		return false;
	}

    lpData =new char [dwLen+1];

	bSuccess = GetFileVersionInfo(FileName, 0, dwLen, lpData);
	if (!bSuccess)
	{
		strcpy(VersionInfo,"获得版本信息失败.");

		delete lpData;

		return false;
	}

    LPVOID lpBuffer = NULL;
    UINT uLen = 0;

	bSuccess = VerQueryValue(lpData,TEXT("\\StringFileInfo\\080404b0\\FileVersion"),&lpBuffer,&uLen); 
			  //0804中文
              //04b0即1252,ANSI
              //可以从ResourceView中的Version中BlockHeader中看到
              //可以测试的属性
              /*
                CompanyName 
                FileDescription 
                FileVersion 
                InternalName 
                LegalCopyright
                OriginalFilename
                ProductName 
                ProductVersion 
                Comments
                LegalTrademarks 
                PrivateBuild 
                SpecialBuild 
              */              

	if (!bSuccess)
	{
		VersionInfo="获得版本信息失败.";
		delete lpData;
		return false;
	}

	for (int i= 0; i<=uLen; i++)
	{
        VersionInfo[i] = ((char *)lpBuffer)[i];
	}
	*dwSize=strlen(VersionInfo);
	delete [] lpData;      
	return true;

}
bool RegReadKey(char *SubReg,char *SubKeyName,char *keyValue)
{
	HKEY hKey=0;
	char *SubRegName=new char[512];
	LPBYTE lpData= NULL;
	DWORD dwBytes=0;
	strcpy(SubRegName,"Software\\Ofilm\\");
	if (strlen(SubReg))
	{
		strcat(SubRegName,SubReg);
	}
	::RegOpenKey(HKEY_CURRENT_USER,SubRegName,&hKey);
	if (hKey==0)
	{
		RELEASE(SubRegName);
		return false;
	}
	::RegQueryValueEx(hKey,SubKeyName,NULL,NULL,NULL,&dwBytes);
	if (dwBytes>0)
	{
		lpData = new BYTE[dwBytes];
	}
	::RegQueryValueEx(hKey,SubKeyName,NULL,NULL,lpData,&dwBytes);
	::RegCloseKey(hKey);
	if (lpData==NULL)
	{
		strcpy(keyValue,"");
	}
	else
	{
		strcpy(keyValue,(char *)lpData);
	}
	RELEASE(lpData);
	RELEASE(SubRegName);
	return true;
}

bool RegSetKey(char *SubReg,char *SubKeyName,char *keyValue)
{
	char szKeyValueBuf[512] = {0};
	strncpy(szKeyValueBuf, keyValue, sizeof(szKeyValueBuf)-1);
	HKEY hKey=0;
	char *SubRegName=new char[512];
	strcpy(SubRegName,"Software\\Ofilm\\");
	if (strlen(SubReg))
	{
		strcat(SubRegName,SubReg);
	}
	::RegOpenKey(HKEY_CURRENT_USER,SubRegName,&hKey);
	if (hKey==0)
	{
		::RegCreateKey(HKEY_CURRENT_USER,SubRegName,&hKey);
	}
	
	::RegSetValueEx(hKey,SubKeyName,0,REG_SZ,(CONST BYTE*)szKeyValueBuf,sizeof(szKeyValueBuf));
	::RegCloseKey(hKey);
	RELEASE(SubRegName);
	return true;
}
bool GetFileUpdateTime(char *FilePath,char *ReturnValue)
{
	WIN32_FIND_DATA File_Info;
	memset(&File_Info,0,sizeof(File_Info));
	HANDLE FileHandle=::FindFirstFile(FilePath,&File_Info);
	if(INVALID_HANDLE_VALUE==FileHandle)
	{
		return false;
	}
	FILETIME FileTime=File_Info.ftLastWriteTime;
	sprintf(ReturnValue,"%x%x",FileTime.dwHighDateTime,FileTime.dwLowDateTime);
	return true;
}
void GetNowTime(char *m_sTime)
{
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);
	sprintf(m_sTime,"%04d-%02d-%02d %02d:%02d:%02d",NowTime.wYear,NowTime.wMonth,NowTime.wDay,
		NowTime.wHour,NowTime.wMinute,NowTime.wSecond);
	return ;
}
void GetNowDate(char *m_sDate)
{
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);
	sprintf(m_sDate,"%04d%02d%02d",NowTime.wYear,NowTime.wMonth,NowTime.wDay);
	return;
}
void GetNowTimeT(char *m_sTime)
{
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);
	sprintf(m_sTime,"%04d%02d%02d%02d%02d%02d",NowTime.wYear,NowTime.wMonth,NowTime.wDay,
		NowTime.wHour,NowTime.wMinute,NowTime.wSecond);
	return ;
}

void DoMutateChar(BYTE* c, int mask, BOOL encr)
{
    if (*c == '\r' || *c == '\n')
	{
		return;
	}
	
    *c ^= mask & 0xF;
}

void DoEncrypt(char *sCont,int seed, BOOL encr)
{
	int   j = 0;
	BYTE* temp = NULL;
	
    srand(seed);
	temp = (BYTE*)sCont;
	
    for ( j = 0; j <strlen(sCont); j++)
	{
        DoMutateChar(&temp[j], rand(),encr);
	}
}
CString GetCPUNum()
{
	//获取CPU序列号
	unsigned long s1,s2;     
	char sel;   
	sel='1';   
	CString CpuID,CPUID1,CPUID2;   
	__asm{   
		mov eax,01h   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,eax   
	}   
	CPUID1.Format("%08X%08X",s1,s2);   
	__asm{   
		mov eax,03h   
			xor ecx,ecx   
			xor edx,edx   
			cpuid   
			mov s1,edx   
			mov s2,ecx   
	}   
	CPUID2.Format("%08X%08X",s1,s2);   
	CpuID=CPUID1+CPUID2;
	return CpuID.Mid(5,3);
}
bool GetPwdInfo(char *tBuffer)
{
	char CptPWDInfo[512]={0};
	
	char CptName[512]={0};
	DWORD Len=512;
	GetComputerName(CptName,&Len);
	strcpy(CptPWDInfo,CptName);
	
	OSVERSIONINFO mOS;
	mOS.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&mOS);
	char VersionInfo[512]={0};
	sprintf(VersionInfo,"%d%d%d%d",mOS.dwMajorVersion,mOS.dwMinorVersion,mOS.dwBuildNumber,mOS.dwPlatformId);;
	strcat(CptPWDInfo,VersionInfo);
	
	
	MEMORYSTATUS mMemo;
	char TempMe[512]={0};
	mMemo.dwLength=sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&mMemo);
	sprintf(TempMe,"%d",mMemo.dwTotalPhys);
	strcat(CptPWDInfo,TempMe);
	
	
	CString Temp=GetCPUNum();
	char CPUID[512]={0};
	strcpy(CPUID,Temp.GetBuffer(Temp.GetLength()));
	strcat(CptPWDInfo,CPUID);
	
	strcpy(tBuffer,CptPWDInfo);
	return true;
}

bool GetFileMD5(char *m_FilePath,char *DataMD5)
{
	char TmpData[512]={0};
	CFile mFile;
	if(mFile.Open(m_FilePath,CFile::modeRead)==0)
	{
	//	mFile.Close();
		return false;
	}
	
	CString strMD5 = CMD5Checksum::GetMD5(mFile);
	mFile.Close();
	strcpy(DataMD5,strMD5.GetBuffer(strMD5.GetLength()));
	strupr(DataMD5);
	return true;
}

bool GetUSBKeyID(char *sUSBID)
{
	char Path[128]={0};
	int a=FindPort(0,Path);
	if (a!=0)
	{
		return false;
	}
	else
	{
		DWORD ID1;
		DWORD ID2;
		GetID(&ID1,&ID2,Path);
		sprintf(sUSBID,"%X%X",ID1,ID2);
		return true;
	}
}
CMsgBox::CMsgBox()
{
	this->XScale=3;
	this->YScale=3;
	this->WidthScale=4;
	this->HeightScale=4;
	strcpy(this->m_sPASS,"PASS");
	strcpy(this->m_sFAIL,"Fail");
	strcpy(this->m_Other,"OFilm");
}
void CMsgBox::ShowMsgBox(char *sMsg,int Res) 
{
	CString m_sMsg(sMsg);
	CDllDialog dllDialog(NULL,Res,XScale,YScale,WidthScale,HeightScale,m_sPASS,m_sFAIL,m_Other);
	dllDialog.m_strTemp=m_sMsg;
	dllDialog.m_Res= Res;
	dllDialog.DoModal();	
}
bool ReadBMPtoBuffer(char *BmpFileName,BYTE *RGBBuffer, int &Width,int &Height)
{
	HANDLE hFile = CreateFile(BmpFileName,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if( hFile == INVALID_HANDLE_VALUE ) 
	{
		return false;
	} 
	BITMAPINFOHEADER bmpInfoHeader = {0};
	BITMAPFILEHEADER bfh = {0};
	
	DWORD nBytesRead;
	
	BOOL bResult1 = ReadFile(hFile, &bfh, sizeof(bfh), &nBytesRead, NULL) ;
	BOOL bResult2 = ReadFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &nBytesRead, NULL );
	
	Width = bmpInfoHeader.biWidth;
	Height = bmpInfoHeader.biHeight;
	
	int ScanLineSize = 0;
	int _tmpV = (Width)*3;
	double ModValue = int(fmod(double(Width)*3,4));
	if (ModValue == 0)
	{
		ScanLineSize = (Width)*3;
	}
	else
	{
		ScanLineSize = (Width)*3+int((ModValue == 0)? (0):(4-ModValue));
	}
	
	if (bmpInfoHeader.biSizeImage!=ScanLineSize*Height)
	{
		bmpInfoHeader.biSizeImage = ScanLineSize*Height;
	}
	
	BYTE* pBitmapBits = new BYTE[bmpInfoHeader.biSizeImage];
	memset(pBitmapBits,0,sizeof(BYTE)*bmpInfoHeader.biSizeImage);
	
	BOOL bResult3 = ReadFile(hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &nBytesRead, NULL );

	CloseHandle(hFile);
	memcpy(RGBBuffer,pBitmapBits,sizeof(BYTE)*Width*Height*3);
	return true;
}
bool SaveBuffertoBmp(BYTE *pBuffer,char *BmpFileName, int width, int height)
{
	int				 OffBits;
	HFILE			 bmpFile;
	BITMAPFILEHEADER bmpHeader;  
	BITMAPINFO		 bmpInfo;
	
	OffBits           = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	DWORD dwSizeImage = width * height * 3;  
	
	
	bmpHeader.bfType      = ( (WORD)('M' << 8) | 'B');
	bmpHeader.bfSize      = OffBits + dwSizeImage;
	bmpHeader.bfReserved1 = 0;
	bmpHeader.bfReserved2 = 0;
	bmpHeader.bfOffBits   = OffBits;
	
	bmpInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth         = width;
    bmpInfo.bmiHeader.biHeight        = height;
    bmpInfo.bmiHeader.biPlanes        = 1;
    bmpInfo.bmiHeader.biBitCount      = 24;
    bmpInfo.bmiHeader.biCompression   = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage     = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed       = 0;
    bmpInfo.bmiHeader.biClrImportant  = 0;
	
	bmpFile = _lcreat(BmpFileName, FALSE); 
	if (bmpFile < 0) 
	{
		return false;
	}
	
	UINT len = 0;
	len = _lwrite(bmpFile, (LPSTR)&bmpHeader, sizeof(BITMAPFILEHEADER));
	len = _lwrite(bmpFile, (LPSTR)&bmpInfo,   sizeof(BITMAPINFOHEADER));
	len = _lwrite(bmpFile, (LPSTR)pBuffer,    bmpHeader.bfSize - sizeof(bmpHeader) - sizeof(bmpInfo) + 4);  //+4 is for exact filesize
	_lclose(bmpFile);
	
	return true;
}

bool GetBmpSize(char *BmpFileName,int &Width,int &Height)
{
	HANDLE hFile = CreateFile(BmpFileName,GENERIC_READ, FILE_SHARE_READ,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if( hFile == INVALID_HANDLE_VALUE ) 
	{
		return false;
	} 
	BITMAPINFOHEADER bmpInfoHeader = {0};
	BITMAPFILEHEADER bfh = {0};
	
	DWORD nBytesRead;
	
	BOOL bResult1 = ReadFile(hFile, &bfh, sizeof(bfh), &nBytesRead, NULL) ;
	BOOL bResult2 = ReadFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &nBytesRead, NULL );
	
	Width = bmpInfoHeader.biWidth;
	Height = bmpInfoHeader.biHeight;

	CloseHandle(hFile);

	return true;

}

CMesSystem::CMesSystem()
{
	memset(Run_Card,0,512);
	memset(Barcode_SN,0,512);
	memset(Sensor_ID,0,512);
	memset(TestVersion,0,512);
	memset(TestResult,0,512);
	memset(ErrorCode,0,512);

	memset(LineNO,0,512);
	memset(StationNo,0,512);
	memset(Operator,0,512);
	memset(Machine_NO,0,512);
	memset(ComputerIP,0,512);
	memset(TestTime,0,512);
	memset(TestData,0,16384);
	strcpy(TestData,"");
	memset(TestDataSave,0,16384);
	strcpy(TestDataSave,"");
}

CMesSystem::~CMesSystem()
{

}


bool CMesSystem::ShowLoginDlg()
{
	m_Login.LineNO=LineNO;
	m_Login.StationNo=StationNo;
	m_Login.Machine_NO=Machine_NO;
	m_Login.Operator=Operator;
	m_Login.Module_NO=TestVersion;
  	int Res=m_Login.DoModal();
	if (Res==1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

struct CheckTestInfo
{
	CString name;
	int spec;
	int NGNum;
	CheckTestInfo()
	{
		name = _T("");
		spec = 0;
		NGNum = 0;
	}
};

bool GetLastTestInfo(char* SensorID,BOOL& bExist,void* RetunInfo,char* ReturnMsg)
{
	bExist = FALSE;
	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ReturnMsg,"创建连接失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return false;
	}
	
	_RecordsetPtr m_pLiuRecordset;
	hr = m_pLiuRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
	{
		strcpy(ReturnMsg,"创建选择集实例失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return false;
	}

	
	char sCommand[2048]={0};
	
	Fields *fields=NULL;
/*	strcpy(sCommand,"select  stationid, station_name  FROM sys_station");*/
	sprintf(sCommand,"select distinct a.*,b.station_name,c.route_seq \
from g_test_header a, sys_station b ,sys_route_detail c, \
sys_route_model d \
where a.sensor_id=N'%s' and \
a.stationid = c.station_id and a.model_id = d.model_id and \
c.station_id=b.stationid and c.routeid = d.routeid",SensorID);

	OutputDebugString(sCommand);
	try
	{
		m_pLiuRecordset->Open(sCommand,m_pLiuConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);			
	}
	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		if (m_pLiuRecordset->State)
		{
			m_pLiuRecordset->Close();
		}
		if (m_pLiuRecordset)
		{
			m_pLiuRecordset.Release();
			m_pLiuRecordset = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	_variant_t vValue;
	CString TempData;
	int index = 0;
	while(!m_pLiuRecordset->LiuEOF)
	{
		try
		{	
			bExist = TRUE;
			TestInfoHeader Data;
			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			strcpy(Data.SenserID,TempData);

			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)1));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			TempData.MakeUpper();
			strcpy(Data.ModuleID,TempData);

			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)2));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			strcpy(Data.Result,TempData);

			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)3));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			Data.StationID = atoi(TempData);

			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)4));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			Data.LineID = atoi(TempData);

			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)7));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			strcpy(Data.StationName,TempData);

			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)8));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			Data.RouteSeq = atoi(TempData);
			memcpy(RetunInfo,&Data,sizeof(TestInfoHeader));

			index++;
			m_pLiuRecordset->MoveNext();
		}
		catch(_com_error &e)
		{
			strcpy(ReturnMsg,e.Description());
			OutputDebugString(ReturnMsg);
			if (m_pLiuConnection->State)
			{
				m_pLiuConnection->Close();
			}
			if (m_pLiuConnection)
			{
				m_pLiuConnection.Release();
				m_pLiuConnection = NULL;
			}
			if (m_pLiuRecordset->State)
			{
				m_pLiuRecordset->Close();
			}
			if (m_pLiuRecordset)
			{
				m_pLiuRecordset.Release();
				m_pLiuRecordset = NULL;
			}
			CoUninitialize();
			return false;
		}
	}
	
	if (m_pLiuRecordset->State)
	{
		m_pLiuRecordset->Close();
	}


	ZeroMemory(sCommand,2048);
	sprintf(sCommand,"select msg from g_test_detail where sensor_id=N'%s' and result=N'FAIL'",SensorID);
	
	OutputDebugString(sCommand);
	try
	{
		m_pLiuRecordset->Open(sCommand,m_pLiuConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);			
	}
	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		if (m_pLiuRecordset->State)
		{
			m_pLiuRecordset->Close();
		}
		if (m_pLiuRecordset)
		{
			m_pLiuRecordset.Release();
			m_pLiuRecordset = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	char Path[512] = {0};
	GetModulePath(Path);
	strcat(Path,"FtpSetting.dll");
	
	vector<CheckTestInfo> CheckTest;
	CString TempName[100] = {""};
	int TempFileNum[100] = {0};

	char CheckFail_Name[512] = {0};
	GetPrivateProfileString("MESCHECK","CheckFail_Name","",CheckFail_Name,512,Path);
	WritePrivateProfileString("MESCHECK","CheckFail_Name",CheckFail_Name,Path);
	char *CheckTest_Name = NULL;
	char *delim = ",";
	CheckTest_Name = strtok(CheckFail_Name,delim);
	int i=0;
	while(CheckTest_Name)
	{
		TempName[i++] = CheckTest_Name;
		CheckTest_Name = strtok(NULL,delim);
		if (i>=99)
			break;
	}
	
	char CheckFail_MaxFailNumber[512] = {0};	
	GetPrivateProfileString("MESCHECK","CheckFail_MaxFailNumber","",CheckFail_MaxFailNumber,512,Path);
	WritePrivateProfileString("MESCHECK","CheckFail_MaxFailNumber",CheckFail_MaxFailNumber,Path);
	char *MaxFailNumber = strtok(CheckFail_MaxFailNumber,delim);
	int j=0;
	while(MaxFailNumber)
	{
		TempFileNum[j++] = atoi(MaxFailNumber);
		MaxFailNumber = strtok(NULL,delim);
		if (i>=99)
			break;
	}

	for (i=0;i<100;i++)
	{
		if (TempName[i].IsEmpty()!=TRUE)
		{
			CheckTestInfo temp;
			temp.name = TempName[i];
			temp.spec = TempFileNum[i];
			temp.NGNum = 0;
			CheckTest.push_back(temp);
		}
		else
			break;
	}

	vValue;
	TempData;
	while(!m_pLiuRecordset->LiuEOF)
	{
		try
		{	
			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			TempData.MakeLower();

			for (UINT index=0; index<CheckTest.size();index++)
			{
				CString Fail1 = CheckTest[index].name+":NG";
				CString Fail2 = CheckTest[index].name+":Fail";
				CString Fail3 = CheckTest[index].name+":False";
				Fail1.MakeLower();
				Fail2.MakeLower();
				Fail3.MakeLower();
				if ((TempData.Find(Fail1,0) != -1)||(TempData.Find(Fail2,0) != -1)||(TempData.Find(Fail3,0) != -1))
				{
					CheckTest[index].NGNum++;
				}
				if (CheckTest[index].spec>0 && CheckTest[index].NGNum>=CheckTest[index].spec)
				{
					sprintf(ReturnMsg,_T("查询到:%s测试失败了%d次, 限制测试%d次;"),CheckTest[index].name,CheckTest[index].NGNum,CheckTest[index].spec);
					if (m_pLiuConnection->State)
					{
						m_pLiuConnection->Close();
					}
					if (m_pLiuConnection)
					{
						m_pLiuConnection.Release();
						m_pLiuConnection = NULL;
					}
					if (m_pLiuRecordset->State)
					{
						m_pLiuRecordset->Close();
					}
					if (m_pLiuRecordset)
					{
						m_pLiuRecordset.Release();
						m_pLiuRecordset = NULL;
					}
					CoUninitialize();
					return false;
				}
			}		
			m_pLiuRecordset->MoveNext();
		}
		catch(_com_error &e)
		{
			strcpy(ReturnMsg,e.Description());
			OutputDebugString(ReturnMsg);
			if (m_pLiuConnection->State)
			{
				m_pLiuConnection->Close();
			}
			if (m_pLiuConnection)
			{
				m_pLiuConnection.Release();
				m_pLiuConnection = NULL;
			}
			if (m_pLiuRecordset->State)
			{
				m_pLiuRecordset->Close();
			}
			if (m_pLiuRecordset)
			{
				m_pLiuRecordset.Release();
				m_pLiuRecordset = NULL;
			}
			CoUninitialize();
			return false;
		}
	}

	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	if (m_pLiuRecordset->State)
	{
		m_pLiuRecordset->Close();
	}
	if (m_pLiuRecordset)
	{
		m_pLiuRecordset.Release();
		m_pLiuRecordset = NULL;
	}
	CoUninitialize();
	strcpy(ReturnMsg,"OK");
	return true;
}

bool DeleteLocalMesData(char *FuseID,char *ReturnMsg)
{
	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ReturnMsg,"创建连接失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return false;
	}
	
	bool RES = false;
	
	try
	{
		char sCommand[2048] = {0};
		sprintf(sCommand,"	insert into g_test_header_rework(sensor_id,model_id,result,stationid,lineid,remark,create_time) select sensor_id,model_id,result,stationid,lineid,remark,create_time from g_test_header where sensor_id = N'%s';\
							insert into g_binding_rework(id,runcard,serial_number,sensor_id,lineid,remark,create_time) select id,runcard,serial_number,sensor_id,lineid,remark,create_time from g_binding where sensor_id = N'%s';\
							insert into g_test_detail_rework(sensor_id,model_id,result,stationid,station_ip,station_no,user_id,lineid,remark,msg,create_time) select sensor_id,model_id,result,stationid,station_ip,station_no,user_id,lineid,remark,msg,create_time from g_test_detail where sensor_id = N'%s';\
							delete from g_test_header where sensor_id = N'%s';\
							delete from g_binding where sensor_id = N'%s';\
							delete from g_test_detail where sensor_id = N'%s';",FuseID,FuseID,FuseID,FuseID,FuseID,FuseID);	

    	OutputDebugString(sCommand);
		_variant_t ra;
		m_pLiuConnection->Execute(sCommand,&ra,adCmdText);
		CString TempData;
		TempData=((LPCSTR)(_bstr_t)ra);
		TempData.TrimLeft();
		TempData.TrimRight();
		OutputDebugString(TempData);
		int count=atoi(TempData);
		if (count >= 0)
		{
			OutputDebugString("删除成功");
			RES = true;
		}
	}catch(_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		
		CoUninitialize();
		return false;
	}
	
	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	
	CoUninitialize();
	strcpy(ReturnMsg,"OK");
	return RES;
}

bool CMesSystem::DeleteMesData(char *ReturnMsg)
{
	if (strlen(Sensor_ID)==0)
	{
		strcpy(ReturnMsg,"MES delete Fail,SensorID不能都为空!");		
		return false;
	}

    bool ret = DeleteLocalMesData(Sensor_ID,ReturnMsg);

    return ret;
}


bool MesReCheck(char *Sensor_ID,char *StationNo,char *TestVersion,char *ReturnMsg)
{	
	TestInfoHeader mesData;
	BOOL bExist = FALSE;
	WritePrivateProfileString("MESCheck","CheckResult","0",m_Login.m_ConfigFile);
	//数据库中最后一条测试数据为本站测试数据
	bool ret = GetLastTestInfo(Sensor_ID,bExist,&mesData,ReturnMsg);
	OutputDebugStringA(ReturnMsg);
	
	if (!ret)
	{
		strcpy(ReturnMsg,"获取Mes流程数据失败");
		return false;
	}
	if (bExist)
	{
		char model_no_c[256] = "";
		char model_no_d[256] = "";
		
		if(strlen(TestVersion)>7)   	
			memcpy(model_no_c,TestVersion,7);
		else
			strcpy(model_no_c,strupr(TestVersion));
		
		if(strlen(mesData.ModuleID)>7)   	
			memcpy(model_no_d,mesData.ModuleID,7);
		else
			strcpy(model_no_d,strupr(mesData.ModuleID));
		
		
        if (atoi(StationNo) == mesData.StationID && strcmp(model_no_c,model_no_d) == 0)
		{
			//数据库中最后一条测试数据是本站测试数据
			strcpy(ReturnMsg,"OK");
			WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
			return true;
		}
		else
		{
			
			strcpy(ReturnMsg,"Mes复检错误,Mes上传失败");
			return false;				
		}
	}
	else
	{
		sprintf(ReturnMsg,"Mes复检错误,Mes上传失败");
		return false;
	}
	
	WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
	return true;
}

bool CMesSystem::MesCheck(char *ReturnMsg)
{	
	char Path[512] = {0};
	GetModulePath(Path);
	strcat(Path,"FtpSetting.dll");

	char MesStation[512] = {0};
	GetPrivateProfileString("MESCHECK","MesStation","",MesStation,512,Path);

	char Temp[512] = {0};
	GetPrivateProfileString("MESSetting","StaionNoPos",NULL,Temp,512,m_Login.m_ConfigFile);
	
	if (strlen(MesStation)>0);
	else
	{
		strcpy(ReturnMsg,"MES 站位未设置!");	
		return false;
	}
	CString MESSET=MesStation;
	CString MESSELECT=Temp;
	if (MESSELECT!=MESSET)
	{
		strcpy(ReturnMsg,"MES 站位选择错误!");	
		return false;
	}
	char MesStation2[512] = {0};
	GetPrivateProfileString("MESCHECK","MesLine","",MesStation2,512,Path);
	char Temp2[512] = {0};
	GetPrivateProfileString("MESSetting","ModuleNoPos",NULL,Temp2,512,m_Login.m_ConfigFile);
	CString MESSELECT2=Temp2;
	CString MESSET2=MesStation2;
	if (MESSELECT2!=MESSET2)
	{
		strcpy(ReturnMsg,"MES 机种选择错误!");	
		return false;
	}
	
	WritePrivateProfileString("MESCheck","CheckResult","0",m_Login.m_ConfigFile);

	memset(TestData,0,16384);
	memset(TestDataSave,0,16384*sizeof(char));

	if ((strlen(Run_Card)==0)&&(strlen(Barcode_SN)==0)&&(strlen(Sensor_ID)==0))
	{
		strcpy(ReturnMsg,"MES Check Fail,RunCard-Barcode-SensorID不能都为空!");
		return false;
	}

#if 1

	int _Ascii[128] = {0};     //Sensor ID防呆
	memset(_Ascii,0,128*sizeof(int));
	int tempVal = 0;
	int Len = strlen(Sensor_ID);
	for (int i=0; i<Len; i++)
	{
		tempVal = Sensor_ID[i];
		if ((tempVal>=48 && tempVal<=57) || 
			(tempVal>=65 && tempVal<=90) ||
			(tempVal>=97 && tempVal<=122))
		{
			_Ascii[tempVal]++;
		}
	}
	
	int CharCnt = 0;
	for (i=0; i<128; i++)
	{
		if (_Ascii[i]>0)
		{
			CharCnt++;
		}
	}
	
	int CharNum = 0;
	CharNum = GetPrivateProfileInt("MESCHECK","CharNum",0,Path);
	
	if (CharCnt < CharNum)
	{
		strcpy(ReturnMsg,"SensorID错误,请确认是否正常读取!");	
		return false;
	}

#endif

	char tempContent[512] = {0};
	string mesContent;

	sprintf(tempContent,"Run_Card:%s;\n",Run_Card);
	mesContent.append(tempContent);

	sprintf(tempContent,"Barcode_SN:%s;\n",Barcode_SN);
	mesContent.append(tempContent);

	sprintf(tempContent,"Sensor_ID:%s;\n",Sensor_ID);
	mesContent.append(tempContent);

	GetPrivateProfileString("MESSetting","LineNoPos",NULL,ReturnMsg,512,m_Login.m_ConfigFile);
	sprintf(tempContent,"LineNO:%s;\n",ReturnMsg);
	mesContent.append(tempContent);

	GetPrivateProfileString("MESSetting","ModuleNoPos",NULL,ReturnMsg,512,m_Login.m_ConfigFile);
	strcpy(TestVersion,ReturnMsg);
	sprintf(tempContent,"ModuleNo:%s;\n",ReturnMsg);
	mesContent.append(tempContent);

	GetPrivateProfileString("MESSetting","StaionNoPos",NULL,ReturnMsg,512,m_Login.m_ConfigFile);
	sprintf(tempContent,"StationNo:%s;\n",ReturnMsg);
	mesContent.append(tempContent);
	
	sprintf(tempContent,"Operator:%s;\n",Operator);
	mesContent.append(tempContent);

	sprintf(tempContent,"Machine_NO:%s;\n",Machine_NO);
	mesContent.append(tempContent);
	
	GetLocalMac(ComputerIP);
	sprintf(tempContent,"ComputerIP:%s;\n",ComputerIP);
	mesContent.append(tempContent);
	
	GetNowTime(TestTime);
	sprintf(tempContent,"TestTime:%s;\n",TestTime);
	mesContent.append(tempContent);

	char TimeTemp[64]={0};
	char FileNameTemp[64]={0};
	CreateDirectory("LocalMes",NULL);
	GetNowTimeT(TimeTemp);
	sprintf(FileNameTemp,"LocalMes\\%s_MesCheck_%s.txt",Sensor_ID,TimeTemp);
	FILE *pFIle=fopen(FileNameTemp,"a");
	fputs(mesContent.c_str(),pFIle);

	fclose(pFIle);
	
	int routeSeq = GetPrivateProfileInt("MESSetting","StationRouteID",1,m_Login.m_ConfigFile);

	TestInfoHeader mesData;
	BOOL bExist = FALSE;
	//数据库中最后一条测试数据为本站测试数据
	bool ret = GetLastTestInfo(Sensor_ID,bExist,&mesData,ReturnMsg);
	OutputDebugStringA(ReturnMsg);

	if (!ret)
	{
		return false;
	}
	if (bExist)
	{
		char model_no_c[256] = "";
		char model_no_d[256] = "";

		if(strlen(TestVersion)>7)   	
			memcpy(model_no_c,TestVersion,7);
		else
		    strcpy(model_no_c,strupr(TestVersion));

		if(strlen(mesData.ModuleID)>7)   	
			memcpy(model_no_d,mesData.ModuleID,7);
		else
			strcpy(model_no_d,strupr(mesData.ModuleID));


		
		int RouteNum = GetPrivateProfileInt("StationInfo","RouteNum",10,m_Login.m_ConfigFile);
		int MesRouteID = 0;
		int PreMustTestStationIDOnMes = 0;
		char tempChar[512] = {0};

		int CurrentStationNo = 0;
		CurrentStationNo = GetPrivateProfileInt("MESSetting","StaionNo",1,m_Login.m_ConfigFile);
		
		int CurrentStationRouteID = 0;

		BOOL bStationIDFind = FALSE;

 		int jj = 0;
		for (jj=0; jj<RouteNum; jj++)
		{
			sprintf(tempChar,"Station[%d].RouteID",jj);
			MesRouteID = GetPrivateProfileInt("StationInfo",tempChar,1,m_Login.m_ConfigFile);			
			sprintf(tempChar,"Station[%d].RouteID = %d",jj,MesRouteID);
			OutputDebugStringA(tempChar);
			
			if (MesRouteID == mesData.StationID)
			{
				bStationIDFind = TRUE;
				PreMustTestStationIDOnMes = jj;
				itoa(jj,tempChar,10);
				OutputDebugStringA("MES测试上传的最后一个站位序号为:");
				OutputDebugStringA(tempChar);
			}
			else if(MesRouteID == CurrentStationNo)
			{
				CurrentStationRouteID = jj;
				sprintf(tempChar,"Station[%d].Name",jj);
				GetPrivateProfileString("StationInfo",tempChar,NULL,ReturnMsg,512,m_Login.m_ConfigFile);
				CString CurrentStationName = ReturnMsg;
				if (MESSELECT != CurrentStationName)
				{
					sprintf(ReturnMsg,"MESetting StationNoPos设置和StationNo设置不匹配!");
			        return false;
				}
			}
		}
		
		if(bStationIDFind != TRUE)
		{
			sprintf(ReturnMsg,"上一站StationID和本地MesSetting匹配错误!");
			return false;
		}



		
		if(strcmp(model_no_c,model_no_d) != 0)
		{
			sprintf(ReturnMsg,"混料:该模组在系统里对应的机种为 %s",mesData.ModuleID);
			return false;
		}
        else if (atoi(StationNo) == mesData.StationID && strcmp(model_no_c,model_no_d) == 0)
		{
			//数据库中最后一条测试数据是本站测试数据
			strcpy(ReturnMsg,"OK");
			WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
			return true;
		}
		else
		{
			//数据库中最后一条测试数据不是本站测试数据
			char preStationName[512] = {0};
			char temptemp[512] = {0};
            int  preStationNo = 0;
			int  isMustTest = 0; 
	
			if (routeSeq == 1)
			{
				if (mesData.RouteSeq > routeSeq)
				{
					// 数据库中最后一个测试站大于本站	
					sprintf(ReturnMsg,"已过站：上一站测试站别为%s",mesData.StationName);
					OutputDebugStringA(ReturnMsg);
					return false;
				}
				else
				{
					strcpy(ReturnMsg,"OK");
					WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
					return true;
				}
			}
			else if (routeSeq > 1)
			{
				sprintf(temptemp,"Station[%d].RouteID",(routeSeq-2));
				preStationNo = GetPrivateProfileInt("StationInfo",temptemp,1,m_Login.m_ConfigFile);
				sprintf(temptemp,"Station[%d].Name",(routeSeq-2));
				GetPrivateProfileString("StationInfo",temptemp,NULL,preStationName,512,m_Login.m_ConfigFile);
				sprintf(temptemp,"Station[%d].IsMust",(routeSeq-2));
				isMustTest = GetPrivateProfileInt("StationInfo",temptemp,1,m_Login.m_ConfigFile);
			}

			itoa(mesData.StationID,temptemp,10);
			OutputDebugStringA("已测试的最后一个站位RouteID为:");
			OutputDebugStringA(temptemp);

			if (preStationNo == mesData.StationID)
			{	
				// 数据库中最后一条测试数据是前一站的测试数据	
				strupr(mesData.Result);
				if (strcmp(mesData.Result,"PASS") == 0)
				{
					// 前一站测试Fail
					strcpy(ReturnMsg,"OK");
					WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
					return true;
				}else
				{
					// 前一站测试Fail
					sprintf(ReturnMsg,"上一站: %s 测试FAIL",mesData.StationName);
					return false;
				}
			}
			else
			{
				// 数据库中最后一条测试数据不是前一站的测试数据	
				itoa(mesData.RouteSeq,temptemp,10);
				OutputDebugStringA("上一站在途程中的序号为:");
				OutputDebugStringA(temptemp);

				itoa(routeSeq-1,temptemp,10);
				OutputDebugStringA("本站在途程中的序号为:");
				OutputDebugStringA(temptemp);

				if (mesData.RouteSeq > routeSeq)
				{
					strupr(mesData.Result);
					OutputDebugStringA("上一站测试结果为:");
					OutputDebugStringA(mesData.Result);

				    if (strcmp(mesData.Result,"PASS") < 0)
                    {
						if (((mesData.RouteSeq-1)==routeSeq)||((mesData.RouteSeq-2)==routeSeq))
						{
							strcpy(ReturnMsg,"OK");
							WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
					        return true;
						}
						else
						{
							// 数据库中最后一个测试站大于本站	
							sprintf(ReturnMsg,"已过站：上一站测试站别为%s",mesData.StationName);
					        return false;
						}
					} 
					else
					{
						// 数据库中最后一个测试站大于本站	
						sprintf(ReturnMsg,"已过站：上一站测试站别为%s",mesData.StationName);
					    return false;
					}

				}
				else if ((mesData.RouteSeq < routeSeq)&&isMustTest)
				{
					// 数据库中最后一个测试站小于本站
					// 前一站必须测试
					sprintf(ReturnMsg,"漏站:该产品上一站测试站别为%s",mesData.StationName);
					return false;
                }
				else
				{
			    	// 前一站不是必测站
				    strupr(mesData.Result);
					if (strcmp(mesData.Result,"PASS") != 0)
					{
						// 数据库中最后的测试结果是Fail
						sprintf(ReturnMsg," %s 站位测试FAIL",mesData.StationName);
						return false;
					}
					else
					{
						// 检测该机种流程单中 在该站位之前的最后一个必测站是哪个
						
						
						
						int preMustStation = routeSeq-1;
						do 
						{
							sprintf(temptemp,"Station[%d].IsMust",preMustStation-1);
							isMustTest = GetPrivateProfileInt("StationInfo",temptemp,1,m_Login.m_ConfigFile);
						 
      						if (isMustTest)
							{
								break;
							}
							else
							{
								preMustStation--;
							}

						} while (preMustStation>=0);

						sprintf(temptemp,"Station[%d].IsMust = 1",preMustStation-1);
						OutputDebugStringA("本站前一个必测站位序号为:");
						OutputDebugStringA(temptemp);

						if (preMustStation < 0)
						{
							strcpy(ReturnMsg,"OK");
							WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
					        return true;
						}	
						
						else if (preMustStation > mesData.RouteSeq ||
					       	(preMustStation-1) > PreMustTestStationIDOnMes)
						{
							sprintf(ReturnMsg,"漏站:该产品上一站测试站别为%s",mesData.StationName);
						    return false;
						}
						else
						{
							strcpy(ReturnMsg,"OK");
							WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
					        return true;
						}
					}
				}
			}				
		}
	}
	else
	{
		if (routeSeq == 1)
		{
			strcpy(ReturnMsg,"OK");
			return true;
		}

		sprintf(ReturnMsg,"漏测: 该模组未测试过");
		return false;
	}
	
	memset(TestData,0,16384*sizeof(char));
	WritePrivateProfileString("MESCheck","CheckResult","1",m_Login.m_ConfigFile);
	return true;
}

int ofCheckBarcode(char* ModuleName, char* Runcard,char* Barcode,char* FuseID,BOOL &isFail, char* ReturnMsg)
{
	isFail = FALSE;

	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ReturnMsg,"创建连接失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();		
		return -1;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return -1;
	}
		_RecordsetPtr m_pLiuRecordset;
	hr = m_pLiuRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
	{
		strcpy(ReturnMsg,"创建选择集实例失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return -1;
	}
	
	try
	{
		char sCommand[5000] = {0};
	
		sprintf(sCommand,"select distinct c.route_seq,b.station_name,a.result from g_test_header a,sys_station b,sys_route_detail c,sys_route_model d \
where a.sensor_id=N'%s' and a.stationid = c.station_id and c.routeid = d.routeid and a.model_id = d.model_id and a.stationid = b.stationid;\
select model_no from G_BarCodeData where barcode =N'%s';\
select distinct model_no from G_RunCardQty where runcard=N'%s';\
select sensor_id FROM g_binding where serial_number=N'%s' group by sensor_id;\
select distinct serial_number,runcard from g_binding where sensor_id=N'%s';",FuseID,Barcode,Runcard,Barcode,FuseID);
		
		m_pLiuRecordset->Open(sCommand,m_pLiuConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		OutputDebugString(sCommand);	
	}

	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		if (m_pLiuRecordset->State)
		{
			m_pLiuRecordset->Close();
		}
		if (m_pLiuRecordset)
		{
			m_pLiuRecordset.Release();
			m_pLiuRecordset = NULL;
		}
		CoUninitialize();
		return -1;
	}
	// -1 : 数据库中有数据，且与传入的Runcard不一致 或 数据库中的数据不止一条
	// 0 : 数据库中无绑定数据，可正常绑定
	// 1: 已绑定相同Runcard
	int  retValue = 1;
	_variant_t vValue;
	CString TempData;
	CString LastResult;
	int index = 0;

	while(!m_pLiuRecordset->LiuEOF)
	{
		try
		{	
			if (retValue == 1)
			{
				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
				TempData = ((LPCSTR)(_bstr_t)vValue);
				TempData.TrimLeft();
				TempData.TrimRight();
				int nCount = atoi(TempData);
				

				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)1));
				TempData = ((LPCSTR)(_bstr_t)vValue);
				TempData.TrimLeft();
				TempData.TrimRight();

				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)2));
				LastResult = ((LPCSTR)(_bstr_t)vValue);
				LastResult.TrimLeft();
				LastResult.TrimRight();
				LastResult.MakeUpper();
				
				if ((nCount > 1)&&(LastResult.Find("PASS")!=(-1)))   //记得改回来
				{
					sprintf(ReturnMsg,"已过站,上一站为:%s",TempData);
					OutputDebugString(ReturnMsg);
					retValue = -1;
				}
				else if (nCount > 1)
				{
					sprintf(ReturnMsg,"已过站,上一站:%s测试Fail",TempData);
					OutputDebugString(ReturnMsg);
					isFail = TRUE;
					retValue = 1;
				}
				else if ((nCount == 1)&&(LastResult.Find("FAIL")!=(-1)))
				{
					sprintf(ReturnMsg,"%s测试Fail",TempData);
					OutputDebugString(ReturnMsg);
					isFail = TRUE;
					retValue = 1;
				}
				else 
				{
					retValue = 1;
				}
			}
			
			index++;
			m_pLiuRecordset->MoveNext();
		}
		catch(_com_error &e)
		{
			strcpy(ReturnMsg,e.Description());
			OutputDebugString(ReturnMsg);
			if (m_pLiuConnection->State)
			{
				m_pLiuConnection->Close();
			}
			if (m_pLiuConnection)
			{
				m_pLiuConnection.Release();
				m_pLiuConnection = NULL;
			}
			if (m_pLiuRecordset->State)
			{
				m_pLiuRecordset->Close();
			}
			if (m_pLiuRecordset)
			{
				m_pLiuRecordset.Release();
				m_pLiuRecordset = NULL;
			}
			CoUninitialize();
			return -1;
		}
	}
	
    if (index == 0)
    {
		retValue = 1;
    }
	
	if (retValue == 1)
	{
		index = 0;
		
		m_pLiuRecordset = m_pLiuRecordset->NextRecordset(NULL);

		while(!m_pLiuRecordset->LiuEOF)
		{
			try
			{	
				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
				TempData = ((LPCSTR)(_bstr_t)vValue);
				TempData.TrimLeft();
				TempData.TrimRight();
				TempData.MakeUpper();
				char model_no_c[512] = {0};
				
				if(strlen(ModuleName)>7)   	
					memcpy(model_no_c,ModuleName,7);
				else
				    strcpy(model_no_c,strupr(ModuleName));

				if (TempData.Find(model_no_c) < 0)
				{
					sprintf(ReturnMsg,"该Barcode在途程上对应的机种为%s",TempData);
					OutputDebugString(ReturnMsg);
					retValue = -1;
				}
				else
				{ 
// 					vValue = m_pLiuRecordset->GetCollect(_variant_t((long)1));
// 					TempData = ((LPCSTR)(_bstr_t)vValue);
// 					TempData.TrimLeft();
// 					TempData.TrimRight();
// 					TempData.MakeUpper();
// 					char Runcard_c[512] = {0};
// 					strcpy(Runcard_c,strupr(Runcard));
// 					if (strcmp(Runcard_c,TempData.GetBuffer(TempData.GetLength())) == 0)
// 					{
						retValue = 1;
// 					}
// 					else
// 					{
// 						strcpy(ReturnMsg,"该Barcode与Runcard不对应，请重新扫描Runcard");
// 						retValue = -1;
// 					}
				}

				index++;
				m_pLiuRecordset->MoveNext();
			}
			catch(_com_error &e)
			{
				strcpy(ReturnMsg,e.Description());
				OutputDebugString(ReturnMsg);
				if (m_pLiuConnection->State)
				{
					m_pLiuConnection->Close();
				}
				if (m_pLiuConnection)
				{
					m_pLiuConnection.Release();
					m_pLiuConnection = NULL;
				}
				if (m_pLiuRecordset->State)
				{
					m_pLiuRecordset->Close();
				}
				if (m_pLiuRecordset)
				{
					m_pLiuRecordset.Release();
					m_pLiuRecordset = NULL;
				}
				CoUninitialize();
				return -1;
			}
		}

		if(index == 0)
		{
			strcpy(ReturnMsg,"数据库没有这个Barcode的信息");
			retValue = -1;
		}
	}

	if (retValue == 1)
	{
		index = 0;
		
		m_pLiuRecordset = m_pLiuRecordset->NextRecordset(NULL);
		
		while(!m_pLiuRecordset->LiuEOF)
		{
			try
			{	
				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
				TempData = ((LPCSTR)(_bstr_t)vValue);
				TempData.TrimLeft();
				TempData.TrimRight();
				TempData.MakeUpper();
				char model_no_c[512] = {0};
				
				if(strlen(ModuleName)>7)   	
					memcpy(model_no_c,ModuleName,7);
				else
					strcpy(model_no_c,strupr(ModuleName));
				
				if (TempData.Find(model_no_c) < 0)
				{
					sprintf(ReturnMsg,"该Runcard在途程上对应的机种为%s",TempData);
					OutputDebugString(ReturnMsg);
					retValue = -1;
				}
				else
				{ 
					retValue = 1;
				}
				
				index++;
				m_pLiuRecordset->MoveNext();
			}
			catch(_com_error &e)
			{
				strcpy(ReturnMsg,e.Description());
				OutputDebugString(ReturnMsg);
				if (m_pLiuConnection->State)
				{
					m_pLiuConnection->Close();
				}
				if (m_pLiuConnection)
				{
					m_pLiuConnection.Release();
					m_pLiuConnection = NULL;
				}
				if (m_pLiuRecordset->State)
				{
					m_pLiuRecordset->Close();
				}
				if (m_pLiuRecordset)
				{
					m_pLiuRecordset.Release();
					m_pLiuRecordset = NULL;
				}
				CoUninitialize();
				return -1;
			}
		}
		
		if(index == 0)
		{
			strcpy(ReturnMsg,"数据库没有这个Runcard的信息");
			retValue = -1;
		}
	}

	if (retValue == 1)
	{
		index = 0;

		m_pLiuRecordset = m_pLiuRecordset->NextRecordset(NULL);

		while(!m_pLiuRecordset->LiuEOF)
		{
			try
			{	
				if (retValue == 1)
				{
					vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
					TempData = ((LPCSTR)(_bstr_t)vValue);
					TempData.TrimLeft();
					TempData.TrimRight();
					TempData.MakeUpper();
					char sensor_id_c[512] = {0};
					strcpy(sensor_id_c,strupr(FuseID));
					if (strcmp(sensor_id_c,TempData.GetBuffer(TempData.GetLength())) != 0)
					{
						sprintf(ReturnMsg,"该Barcode已绑定其他模组!");
						OutputDebugString(ReturnMsg);
						retValue = -1;
					}
					else
					{
						retValue = 1;
					}
				}

				index++;
				m_pLiuRecordset->MoveNext();
			}
			catch(_com_error &e)
			{
				strcpy(ReturnMsg,e.Description());
				OutputDebugString(ReturnMsg);
				if (m_pLiuConnection->State)
				{
					m_pLiuConnection->Close();
				}
				if (m_pLiuConnection)
				{
					m_pLiuConnection.Release();
					m_pLiuConnection = NULL;
				}
				if (m_pLiuRecordset->State)
				{
					m_pLiuRecordset->Close();
				}
				if (m_pLiuRecordset)
				{
					m_pLiuRecordset.Release();
					m_pLiuRecordset = NULL;
				}
				CoUninitialize();
				return -1;
			}
		}
 
		if (index == 0)
		{
			retValue = 1;
		}
	}

	if (retValue == 1)
	{
		index = 0;
		
		m_pLiuRecordset = m_pLiuRecordset->NextRecordset(NULL);
		
		while(!m_pLiuRecordset->LiuEOF)
		{
			try
			{	
				if (retValue == 1)
				{
					vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
					TempData = ((LPCSTR)(_bstr_t)vValue);
					TempData.TrimLeft();
					TempData.TrimRight();
					TempData.MakeUpper();
					char barcode_id_c[512] = {0};
					strcpy(barcode_id_c,strupr(Barcode));
					if (strcmp(barcode_id_c,TempData.GetBuffer(TempData.GetLength())) != 0)
					{
						sprintf(ReturnMsg,"该模组已绑定其他Barcode!");
						OutputDebugString(ReturnMsg);
						retValue = -1;
					}
					else
					{
						if(isFail)
						{
							retValue = 1;
						}
						else
						{
							vValue = m_pLiuRecordset->GetCollect(_variant_t((long)1));
							TempData = ((LPCSTR)(_bstr_t)vValue);
							TempData.TrimLeft();
							TempData.TrimRight();
							TempData.MakeUpper();
							char runcard_id_c[512] = {0};
							strcpy(runcard_id_c,strupr(Runcard));
							if (strcmp(runcard_id_c,TempData.GetBuffer(TempData.GetLength())) != 0)
							{
								sprintf(ReturnMsg,"该模组已绑定其他Runcard!");
								OutputDebugString(ReturnMsg);
								retValue = -1;
							}
							else
							{
								retValue = 1;
							}
						}
				
					}
				}
			
				index++;
				m_pLiuRecordset->MoveNext();
			}
			catch(_com_error &e)
			{
				strcpy(ReturnMsg,e.Description());
				OutputDebugString(ReturnMsg);
				if (m_pLiuConnection->State)
				{
					m_pLiuConnection->Close();
				}
				if (m_pLiuConnection)
				{
					m_pLiuConnection.Release();
					m_pLiuConnection = NULL;
				}
				if (m_pLiuRecordset->State)
				{
					m_pLiuRecordset->Close();
				}
				if (m_pLiuRecordset)
				{
					m_pLiuRecordset.Release();
					m_pLiuRecordset = NULL;
				}
				CoUninitialize();
				return -1;
			}
		}
		
		if (index == 0)
		{
			strcpy(ReturnMsg,"OK");
			retValue = 1;
		}
	}

	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	if (m_pLiuRecordset->State)
	{
		m_pLiuRecordset->Close();
	}
	if (m_pLiuRecordset)
	{
		m_pLiuRecordset.Release();
		m_pLiuRecordset = NULL;
	}
	CoUninitialize();
	return retValue;
}

int ofCheckBing(char* ModuleName, char* Runcard,char* SerialNO,char* SensorID,BOOL &isFail,char* ReturnMsg)
{
	isFail = FALSE;

	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ReturnMsg,"创建连接失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();		
		return -1;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return -1;
	}
		_RecordsetPtr m_pLiuRecordset;
	hr = m_pLiuRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
	{
		strcpy(ReturnMsg,"创建选择集实例失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return -1;
	}
	
	Fields *fields=NULL;
	try
	{
		char sCommand[3000] = {0};
		sprintf(sCommand,"select distinct c.route_seq,b.station_name,a.result from g_test_header a,sys_station b,sys_route_detail c,sys_route_model d \
where a.sensor_id=N'%s' and a.stationid = c.station_id and c.routeid = d.routeid and a.model_id = d.model_id and a.stationid = b.stationid;\
select distinct model_no from G_RunCardQty where runcard =N'%s';\
select count(*),runcard FROM g_binding  where sensor_id=N'%s' group by runcard ",SensorID,Runcard,SensorID);

// sprintf(sCommand,"select distinct c.route_seq,b.station_name from g_test_header a,sys_station b,sys_route_detail c,sys_route_model d \
// where a.sensor_id=N'%s' and a.stationid = c.station_id and c.routeid = d.routeid and a.model_id = d.model_id and a.stationid = b.stationid;\
// select model_no from G_BarCodeData where runcard =N'%s';\
// select count(*),runcard FROM g_binding  where sensor_id=N'%s' group by runcard ",SensorID,SensorID,Runcard,SensorID);
		
		m_pLiuRecordset->Open(sCommand,m_pLiuConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		OutputDebugString(sCommand);	
	}

	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		if (m_pLiuRecordset->State)
		{
			m_pLiuRecordset->Close();
		}
		if (m_pLiuRecordset)
		{
			m_pLiuRecordset.Release();
			m_pLiuRecordset = NULL;
		}
		CoUninitialize();
		return -1;
	}
	// -1 : 数据库中有数据，且与传入的Runcard不一致 或 数据库中的数据不止一条
	// 0 : 数据库中无绑定数据，可正常绑定
	// 1: 已绑定相同Runcard
	int  retValue = 1;
	_variant_t vValue;
	CString TempData;
	CString LastResult;
	int index = 0;

	while(!m_pLiuRecordset->LiuEOF)
	{
		try
		{	
			if (retValue == 1)
			{
				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
				TempData = ((LPCSTR)(_bstr_t)vValue);
				TempData.TrimLeft();
				TempData.TrimRight();
				int nCount = atoi(TempData);
				
				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)1));
				TempData = ((LPCSTR)(_bstr_t)vValue);
				TempData.TrimLeft();
				TempData.TrimRight();
				
				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)2));
				LastResult = ((LPCSTR)(_bstr_t)vValue);
				LastResult.TrimLeft();
				LastResult.TrimRight();
				LastResult.MakeUpper();
				
				if ((nCount > 1)&&(LastResult.Find("PASS")!=(-1)))   //记得改回来
				{
					sprintf(ReturnMsg,"已过站,上一站为:%s",TempData);
					OutputDebugString(ReturnMsg);
					retValue = -1;
				}
				else if (nCount > 1)
				{
					sprintf(ReturnMsg,"已过站,上一站:%s测试Fail",TempData);
					OutputDebugString(ReturnMsg);
					isFail = TRUE;
					retValue = 1;
				}
				else if ((nCount == 1)&&(LastResult.Find("FAIL")!=(-1)))
				{
					sprintf(ReturnMsg,"%s测试Fail",TempData);
					OutputDebugString(ReturnMsg);
					isFail = TRUE;
					retValue = 1;
				}
				else
				{
					retValue = 1;
				}
			}
		
			index++;
			m_pLiuRecordset->MoveNext();
		}
		catch(_com_error &e)
		{
			strcpy(ReturnMsg,e.Description());
			OutputDebugString(ReturnMsg);
			if (m_pLiuConnection->State)
			{
				m_pLiuConnection->Close();
			}
			if (m_pLiuConnection)
			{
				m_pLiuConnection.Release();
				m_pLiuConnection = NULL;
			}
			if (m_pLiuRecordset->State)
			{
				m_pLiuRecordset->Close();
			}
			if (m_pLiuRecordset)
			{
				m_pLiuRecordset.Release();
				m_pLiuRecordset = NULL;
			}
			CoUninitialize();
			return -1;
		}
	}
	
    if (index == 0)
    {
		retValue = 1;
    }

	if (retValue == 1)
	{
		index = 0;
		
		m_pLiuRecordset = m_pLiuRecordset->NextRecordset(NULL);

       	while(!m_pLiuRecordset->LiuEOF)
		{
			try
			{	
				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
				TempData = ((LPCSTR)(_bstr_t)vValue);
				TempData.TrimLeft();
				TempData.TrimRight();

				char model_no_c[512] = {0};
				if(strlen(ModuleName)>7)   	
					memcpy(model_no_c,ModuleName,7);
				else
					strcpy(model_no_c,strupr(ModuleName));
				
				if (TempData.Find(model_no_c) < 0)   //记得改回来
				{
					sprintf(ReturnMsg,"该Runcard在途程上对应的机种为%s",TempData);
					OutputDebugString(ReturnMsg);
					retValue = -1;
				}
				else
				{
					retValue = 1;
				}
				
				index++;
				m_pLiuRecordset->MoveNext();
			}
			catch(_com_error &e)
			{
				strcpy(ReturnMsg,e.Description());
				OutputDebugString(ReturnMsg);
				if (m_pLiuConnection->State)
				{
					m_pLiuConnection->Close();
				}
				if (m_pLiuConnection)
				{
					m_pLiuConnection.Release();
					m_pLiuConnection = NULL;
				}
				if (m_pLiuRecordset->State)
				{
					m_pLiuRecordset->Close();
				}
				if (m_pLiuRecordset)
				{
					m_pLiuRecordset.Release();
					m_pLiuRecordset = NULL;
				}
				CoUninitialize();
				return -1;
			}
		}
		
		if (index == 0)
		{
			sprintf(ReturnMsg,"数据库里没有该Runcard的信息");
			OutputDebugString(ReturnMsg);
			retValue = -1;
		}
	}

	if (retValue == 1)
	{
		index = 0;
		
		m_pLiuRecordset = m_pLiuRecordset->NextRecordset(NULL);
		
		while(!m_pLiuRecordset->LiuEOF)
		{
			try
			{	
				if (retValue == 1)
				{
					vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
					TempData = ((LPCSTR)(_bstr_t)vValue);
					TempData.TrimLeft();
					TempData.TrimRight();
					int count = atoi(TempData.GetBuffer(TempData.GetLength()));
				//	if (count > 1)
				//	{
				//		strcpy(ReturnMsg,"该模组已绑定不止一个Runcard");
				//		retValue = -1;
				//	}
				//	else
				//	{
				//		retValue = 1;
				//	}
				}

				if (retValue == 1)
				{
					if (isFail)
					{
						retValue = 1;
					}
					else
					{
						vValue = m_pLiuRecordset->GetCollect(_variant_t((long)1));
						TempData = ((LPCSTR)(_bstr_t)vValue);
						TempData.TrimLeft();
						TempData.TrimRight();
						TempData.MakeUpper();
						char Runcard_c[512] = {0};
						strcpy(Runcard_c,strupr(Runcard));
						if (strcmp(Runcard_c,TempData.GetBuffer(TempData.GetLength())) == 0)
						{
							retValue = 1;
						}
						else
						{
							strcpy(ReturnMsg,"该模组已绑定其他RunCard");
							retValue = -1;
						}
					}
				}
				
				index++;
				m_pLiuRecordset->MoveNext();
			}
			catch(_com_error &e)
			{
				strcpy(ReturnMsg,e.Description());
				OutputDebugString(ReturnMsg);
				if (m_pLiuConnection->State)
				{
					m_pLiuConnection->Close();
				}
				if (m_pLiuConnection)
				{
					m_pLiuConnection.Release();
					m_pLiuConnection = NULL;
				}
				if (m_pLiuRecordset->State)
				{
					m_pLiuRecordset->Close();
				}
				if (m_pLiuRecordset)
				{
					m_pLiuRecordset.Release();
					m_pLiuRecordset = NULL;
				}
				CoUninitialize();
				return -1;
			}
		}
		
		if (index == 0)
		{
			strcpy(ReturnMsg,"OK");
			retValue = 1;
		} 
	}

	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	if (m_pLiuRecordset->State)
	{
		m_pLiuRecordset->Close();
	}
	if (m_pLiuRecordset)
	{
		m_pLiuRecordset.Release();
		m_pLiuRecordset = NULL;
	}
	CoUninitialize();

	return retValue;
}
bool ModuleBing(char* Runcard,char* SerialNO,char* SensorID,int LineNO,BOOL isFail,char* ReturnMsg)
{
	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ReturnMsg,"创建连接失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();		
		return false;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return false;
	}
	bool RES = false;
	try
	{
		char sCommand[502400] = {0};
		if (isFail)
		{
            sprintf(sCommand,"insert into g_binding_rework(id,runcard,serial_number,sensor_id,lineid,remark,create_time) \
select id,runcard,serial_number,sensor_id,lineid,remark,create_time from g_binding where sensor_id=N'%s';\
insert into g_test_detail_rework(sensor_id,model_id,result,stationid,station_ip,station_no,user_id,lineid,remark,msg,create_time) \
select sensor_id,model_id,result,stationid,station_ip,station_no,user_id,lineid,remark,msg,create_time from g_test_detail where sensor_id=N'%s';\
insert into g_test_header_rework(sensor_id ,model_id,result,stationid,lineid,remark,create_time) \
select sensor_id ,model_id,result,stationid,lineid,remark,create_time from g_test_header where sensor_id=N'%s';\
delete from g_binding where sensor_id=N'%s';delete from g_test_detail where sensor_id=N'%s';\
delete from g_test_header where sensor_id=N'%s';insert into g_binding(runcard,serial_number,sensor_id,lineid) \
values(N'%s',N'%s',N'%s',%d)",SensorID,SensorID,SensorID,SensorID,SensorID,SensorID,Runcard,SerialNO,SensorID,LineNO);
		}
	    else
		{
	    	sprintf(sCommand,"insert into g_binding(runcard,serial_number,sensor_id,lineid) \
values(N'%s',N'%s',N'%s',%d)",Runcard,SerialNO,SensorID,LineNO);
		}

		_variant_t ra;
		m_pLiuConnection->Execute(sCommand,&ra,adCmdText);
		CString TempData;
		TempData=((LPCSTR)(_bstr_t)ra);
		TempData.TrimLeft();
		TempData.TrimRight();
		OutputDebugString(TempData);
		int count=atoi(TempData);
		if (count > 0)
		{
			OutputDebugString("插入成功");
			RES = true;
		}
	}catch(_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		
		CoUninitialize();
		return false;
	}

	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	
	CoUninitialize();
	strcpy(ReturnMsg,"OK");
	return RES;
}

bool UpdateTestDetail(TestInfoHeader header,char* Operator,char* MachineNo,char* Msg,char* MAC,char* ReturnMsg)
{
	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ReturnMsg,"创建连接失败");
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();		
		return false;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return false;
	}

	bool RES = false;
	try
	{
		char sCommand[25000] = {0};
		char temp[512] = {0};
		strcpy(temp,strupr(header.Result));

		sprintf(sCommand,"declare @infoCount int \
select @infoCount=count(*) from g_test_header where sensor_id = N'%s'	\
if @infoCount > 0 \
begin \
update g_test_header  \
set model_id=N'%s' , \
result=N'%s',	\
stationid=%d ,	\
create_time=getdate(),\
lineid=%d 	\
where sensor_id = N'%s'	\
end	\
else	\
begin	\
insert into g_test_header	\
(sensor_id	\
,model_id	\
,result	\
,stationid	\
,lineid)	\
values(N'%s',N'%s',N'%s',%d,%d)	\
end;\
if @@rowcount = 1 \
begin \
insert into g_test_detail	(sensor_id, \
model_id, \
result, \
stationid, \
station_ip, \
station_no, \
user_id, \
lineid, \
msg\
) values(N'%s',N'%s',N'%s',%d,N'%s',N'%s',N'%s',%d,N'%s') \
end;",\
header.SenserID,header.ModuleID,strupr(header.Result),header.StationID,header.LineID,\
header.SenserID,header.SenserID,header.ModuleID,strupr(header.Result),header.StationID,header.LineID\
,header.SenserID,header.ModuleID,temp,header.StationID,MAC,MachineNo,Operator,header.LineID,Msg);
		OutputDebugString(sCommand);
		_variant_t ra;
		m_pLiuConnection->Execute(sCommand,&ra,adCmdText);
		CString TempData;
		TempData=((LPCSTR)(_bstr_t)ra);

		TempData.TrimLeft();
		TempData.TrimRight();
		OutputDebugString(TempData);
		int count=atoi(TempData);
		if (count == 1)
		{
			strcpy(ReturnMsg,"OK");
			OutputDebugString("插入成功");
			RES = true;
		}
		else
		{
         	strcpy(ReturnMsg,"插入数据失败!");
			RES = FALSE;
		}
	}catch(_com_error &e)
	{
		strcpy(ReturnMsg,e.Description());
		OutputDebugString(ReturnMsg);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		
		CoUninitialize();
		return false;
	}

	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	
	CoUninitialize();

	return RES;
}

bool CMesSystem::MesUpdateData(char *ResurnMsg)
{

		char Path[512] = {0};
		GetModulePath(Path);
		strcat(Path,"FtpSetting.dll");
		
		char MesStation[512] = {0};
		GetPrivateProfileString("MESCHECK","MesStation","",MesStation,512,Path);
		
		char Temp[512] = {0};
		GetPrivateProfileString("MESSetting","StaionNoPos","",Temp,512,m_Login.m_ConfigFile);
		
		DebugInfo("----------路径:----------");
		DebugInfo(Path);
		DebugInfo("----------MesStation:----------");
		DebugInfo(MesStation);
		DebugInfo("----------StaionNoPos:----------");
		DebugInfo(Temp);

		if (strlen(MesStation)>0);
		else
		{
			strcpy(ResurnMsg,"MES 站位未设置!");	
		return false;
		}
		CString MESSET=MesStation;
		CString MESSELECT=Temp;
		if (MESSELECT!=MESSET)
		{
			strcpy(ResurnMsg,"MES 站位选择错误!");	
		return false;
		}
		char MesStation2[512] = {0};
		GetPrivateProfileString("MESCHECK","MesLine","",MesStation2,512,Path);
		char Temp2[512] = {0};
		GetPrivateProfileString("MESSetting","ModuleNoPos",NULL,Temp2,512,m_Login.m_ConfigFile);
		CString MESSELECT2=Temp2;
		CString MESSET2=MesStation2;
		if (MESSELECT2!=MESSET2)
		{
			strcpy(ResurnMsg,"MES 机种选择错误!");	
		return false;
		}

		//插入数据验证
		char temp[512] = {0};
		if (strlen(Sensor_ID) == 0)
		{
			strcpy(ResurnMsg,"MES Update Fail,SensorID不能为空");	
		return false;
		}

		string mesContent;
		char tempContent[512];

		sprintf(tempContent,"Run_Card:%s;\n",Run_Card);
		mesContent.append(tempContent);
		
		sprintf(tempContent,"Barcode_SN:%s;\n",Barcode_SN);
		mesContent.append(tempContent);
		
		sprintf(tempContent,"Sensor_ID:%s;\n",Sensor_ID);
		mesContent.append(tempContent);
		
		GetPrivateProfileString("MESSetting","LineNoPos",NULL,ResurnMsg,512,m_Login.m_ConfigFile);
		sprintf(tempContent,"LineNO:%d %s;\n",atoi(LineNO),ResurnMsg);
		mesContent.append(tempContent);
		
		GetPrivateProfileString("MESSetting","ModuleNoPos",NULL,ResurnMsg,512,m_Login.m_ConfigFile);
		strcpy(TestVersion,ResurnMsg);
		sprintf(tempContent,"ModuleNo:%s;\n",ResurnMsg);
		mesContent.append(tempContent);
		
		GetPrivateProfileString("MESSetting","StaionNoPos",NULL,ResurnMsg,512,m_Login.m_ConfigFile);
		sprintf(tempContent,"StationNo:%d %s;\n",atoi(StationNo),ResurnMsg);
		mesContent.append(tempContent);
		
		sprintf(tempContent,"Operator:%s;\n",Operator);
		mesContent.append(tempContent);
		
		sprintf(tempContent,"Machine_NO:%s;\n",Machine_NO);
		mesContent.append(tempContent);
		
		GetLocalMac(ComputerIP);
		sprintf(tempContent,"ComputerIP:%s;\n",ComputerIP);
		mesContent.append(tempContent);
		
		GetNowTime(TestTime);
		sprintf(tempContent,"TestTime:%s;\n\n",TestTime);
		mesContent.append(tempContent);

		mesContent.append(TestDataSave);
		
		sprintf(tempContent,"\nTestResult:%s;\n",TestResult);
		mesContent.append(tempContent);
		
		sprintf(tempContent,"ErrorCode:%s;\n",ErrorCode);
		mesContent.append(tempContent);
		
		char TimeTemp[64]={0};
		char FileNameTemp[1024]={0};
		CreateDirectory("LocalMes",NULL);
		GetNowTimeT(TimeTemp);
		sprintf(FileNameTemp,"LocalMes\\%s_MesUpdate_%s.txt",Sensor_ID,TimeTemp);
		FILE *pFIle=fopen(FileNameTemp,"a");
		fputs(mesContent.c_str(),pFIle);
		
		fclose(pFIle);

		strcpy(temp,strupr(TestResult));
		OutputDebugString(temp);
		if (!(strcmp(temp,"PASS") == 0 || strcmp(temp,"FAIL") == 0))
		{
			strcpy(ResurnMsg,"MES Update Fail,测试结果只能是PASS或FAIL");
		    return false;
		}

		bool ret = TRUE;

		int routeSeq = GetPrivateProfileInt("MESSetting","StationRouteID",1,m_Login.m_ConfigFile);
		int MESChecckResult = GetPrivateProfileInt("MESCheck","CheckResult",1,m_Login.m_ConfigFile);

	//	int UseBarcode = GetPrivateProfileInt("MESSetting","UseBarcode",1,m_Login.m_ConfigFile);


		int UseBarcode = GetPrivateProfileInt("MESCHECK","UseBarcode",1,Path); /*GetPrivateProfileInt("MESSetting","UseBarcode",1,m_Login.m_ConfigFile)*/;

	//     if ((routeSeq>1)&&(MESChecckResult==0))
	//     {
	// 		strcpy(ResurnMsg,"OK");
	// 	    return true;
	//     }

		if ((routeSeq>1)&&(MESChecckResult==0))
		{
			strcpy(ResurnMsg,"Check Result fail!");
		    return false;
		}

		if (routeSeq == 1)
		{
			if (strlen(Run_Card)==0)
			{
				strcpy(ResurnMsg,"MES Banding Fail,RunCard不能为空!");
			    return false;
			}

			int flag = 0;
			BOOL isFail = FALSE;
			if (UseBarcode)
			{
				if (strlen(Barcode_SN)==0)
				{
					strcpy(ResurnMsg,"MES Banding Fail,Barcode不能为空!");
				    return false;
				}

				flag = ofCheckBarcode(TestVersion,Run_Card,Barcode_SN,Sensor_ID,isFail,ResurnMsg);
 
				if (flag == -1)
				{
				    return false;
				}
			}
			else
			{
				if (strlen(Barcode_SN)!=0)
				{
					strcpy(ResurnMsg,"该机种的途程显示不需要绑定Barcode!");
				    return false;
				}
				else
				{
            		flag = ofCheckBing(TestVersion,Run_Card,Barcode_SN,Sensor_ID,isFail,ResurnMsg);

					if (flag == -1)
					{
					    return false;
					}
				}
			}

			ret = ModuleBing(Run_Card,Barcode_SN,Sensor_ID,atoi(LineNO),isFail,ResurnMsg);
			if (!ret)
			{
			    return false;
			}  
		}

		//插入测试数据头表&测试数据详细信息表
		char MAC[512] = {0};
		GetLocalMac(MAC);

		TestInfoHeader header;
		header.LineID = GetPrivateProfileInt("MESSetting","LineNo",1,m_Login.m_ConfigFile);
		header.StationID = GetPrivateProfileInt("MESSetting","StaionNo",1,m_Login.m_ConfigFile);
		strcpy(header.ModuleID ,TestVersion);
		strcpy(header.Result,strupr(TestResult));
		strcpy(header.SenserID,Sensor_ID);

		ret = UpdateTestDetail(header,Operator,Machine_NO,TestData,MAC,ResurnMsg);
		if (!ret)
		{
		    return false;
		}

		ret = MesReCheck(Sensor_ID,StationNo,TestVersion,ResurnMsg);
		if (!ret)
		{
		    return false;
		}

	memset(TestData,0,16384);
	memset(Run_Card,0,512);
	memset(Barcode_SN,0,512);
	memset(Sensor_ID,0,512);
	memset(TestVersion,0,512);
	memset(TestResult,0,512);
	memset(ErrorCode,0,512);
	memset(TestDataSave,0,16384*sizeof(char));

	strcpy(ResurnMsg,"OK");
	return true;
}

void CMesSystem::ResetDataAfterMesUpdateMaybeFail()
{
	memset(TestData,0,16384);
	memset(Run_Card,0,512);
	memset(Barcode_SN,0,512);
	memset(Sensor_ID,0,512);
	memset(TestVersion,0,512);
	memset(TestResult,0,512);
	memset(ErrorCode,0,512);
	memset(TestDataSave,0,16384*sizeof(char));
}

bool CMesSystem::AddTestItem(char *Name,char *Result)
{
	strcat(TestData,Name);
	strcat(TestData,":");
	strcat(TestData,Result);
	strcat(TestData,";");

	strcat(TestDataSave,Name);
	strcat(TestDataSave,":");
	strcat(TestDataSave,Result);
	strcat(TestDataSave,";\n");

	return true;
}

bool CMesSystem::AddParameter(char *Name,char *Value)
{
	strcat(TestData,Name);
	strcat(TestData,":");
	strcat(TestData,Value);
	strcat(TestData,";");

	strcat(TestDataSave,Name);
	strcat(TestDataSave,":");
	strcat(TestDataSave,Value);
	strcat(TestDataSave,";\n");

	return true;
}

bool CMesSystem::BeginSoap(char *UserName,char *Method,char *sType,char *Data,char *ResturnData)
{
	return TRUE;
}
bool GetBarcode(char *RuncardSN,char Barcode[][32])
{
	CoInitialize(NULL);
	ISoapConnectorPtr SoapConnector;
	ISoapSerializerPtr Serializer;
	ISoapReaderPtr Reader;

	HRESULT hr;
	try
	{
		//创建SoapConnector类的对象
		SoapConnector.CreateInstance(__uuidof(HttpConnector30)); 
		
		//指定Web服务的地址
		SoapConnector->Property ["EndPointURL"] ="http://192.168.58.66:1123/pdaservice.asmx";
		
		//与Web服务连接
		hr=SoapConnector->Connect();
		if(FAILED(hr)) 
		{
			return false;
		}
		
		//指定Web服务完成的操作
		SoapConnector->Property ["SoapAction"] = "http://tempuri.org/GetBarCode";
		
		//准备发送消息给Web服务
		SoapConnector->BeginMessage();
		// 创建SoapSerializer对象
		Serializer.CreateInstance(__uuidof(SoapSerializer30)); 
		
		// 将serializer连接到connector的输入字符串
		Serializer->Init(_variant_t((IUnknown*)SoapConnector->InputStream)); 
		
		// 创建SOAP消息
		Serializer->StartEnvelope("","","");
		
		Serializer->StartBody(""); 
		
		Serializer->StartElement("GetBarCode","http://tempuri.org/","","");
		
		Serializer->StartElement("sc_frcno","http://tempuri.org/","",""); 
		Serializer->WriteString((_bstr_t)RuncardSN);
		Serializer->EndElement();
		
		Serializer->EndElement();
		
		Serializer->EndBody(); 
		Serializer->EndEnvelope();   
		
		//消息真正地发给Web服务
		hr=SoapConnector->EndMessage(); 
		if(FAILED(hr)) return false;
		
		// 读取响应
		Reader.CreateInstance(__uuidof(SoapReader30)); 
		
		// 将reader联接到connector的输出字符串
		Reader->Load(_variant_t((IUnknown*)SoapConnector->OutputStream), _T("")); 
		
		CString TempLog;		
		TempLog.Format("%s",(const char *)Reader->RpcResult->text);
		int n=0;
		do 
		{
			CString TmpNO=TempLog.Mid(0,9);
			if (TmpNO.IsEmpty())
			{
				break;
			}
			strcpy(Barcode[n],TmpNO.GetBuffer(TmpNO.GetLength()));
			TempLog.Delete(0,9);	
			n++;
			
		} while (TRUE);
		
		return true;
	}
	catch (_com_error e)
	{
		return false;
	}
	
	
}
bool GetPwd(char *PwdName,char *Pwd,char *ErrDes)
{
	//FTP下载模块相关
	char m_ConfigFile[512]={0};
	GetModulePath(m_ConfigFile);
	strcat(m_ConfigFile,"Config.ini");

	BOOL Result=FtpGetFile(hConnection,"\\System\\Setting.ini",m_ConfigFile,FALSE,FILE_ATTRIBUTE_HIDDEN, FTP_TRANSFER_TYPE_BINARY + INTERNET_FLAG_RESYNCHRONIZE, 0);
	if (!Result)
	{
		if (hConnection != NULL)
			InternetCloseHandle(hConnection);
		if (hSession != NULL)
			InternetCloseHandle(hSession);
		Result=FtpConnect();
		if (Result)
		{
			Result=FtpGetFile(hConnection,"\\System\\Setting.ini",m_ConfigFile,FALSE,FILE_ATTRIBUTE_HIDDEN, FTP_TRANSFER_TYPE_BINARY + INTERNET_FLAG_RESYNCHRONIZE, 0);
		}
		else
		{
			return false;
		}
	}
	GetPrivateProfileString("Main",PwdName,NULL,Pwd,512,m_ConfigFile);
}

void DrawResult(HWND hParent,int Resource,COLORREF BKColor,int FontSize,char *Msg)
{	
	HBRUSH hBrush,OldBrush;
	HFONT hFont,OldFont;
	RECT rect;
	HWND hDlg=::GetDlgItem(hParent,Resource);
	::GetClientRect(hDlg,&rect);
	
	HDC hDC=::GetDC(hDlg);
	
	hBrush=CreateSolidBrush(BKColor);
	OldBrush=(HBRUSH)SelectObject(hDC,(HBRUSH)hBrush);
	
	::FillRect(hDC,&rect,hBrush);
	
	hFont=CreateFont(FontSize, 0,0,0,0,0,0,0,ANSI_CHARSET,3,2,1, VARIABLE_PITCH | FF_ROMAN, "宋体");
	OldFont = (HFONT)SelectObject(hDC, hFont);
	int bkmode = SetBkMode(hDC,TRANSPARENT);
	
	int len=strlen(Msg);
	char TmpMsg[1024]={0};
	
	strcpy(TmpMsg,Msg);
	len=strlen(TmpMsg);
	if (FontSize<=0)
	{
		FontSize=60;
	}		
	//////////////////////////////////////
	CString str = "";
	str.Format("%s",TmpMsg);
	
	int chinanes = 0;
	int num = 0;	
	int LineLen = 0;
	int count = 0;
	
	int LineNum=rect.right/FontSize*2;
	int StartY=rect.bottom/3;
	char sMsg0[100]={0};
	char sMsg1[100]={0};
	char sMsg2[100]={0};
	char sMsg3[100]={0};
	char sMsg4[100]={0};
	char sMsg5[100]={0};
	int j = 0;
	int sum0 = 0;
	int sum1 = 0;
	int sum2 = 0;
	int sum3 = 0;
	int sum4 = 0;
	int sum5 = 0;
	
	//判断需显示字符串长度是否大于屏幕的宽
	if (len>LineNum)
	{
		//遍历屏幕一行的每个字节，计算出中文个数与数字和标点的个数
		for (j=0;j<LineNum-1;j++)
		{
			if(str[j] & 0x80)//判断最后一个字是不是中文
			{
				chinanes++;
				j++;
			}
			else
			{
				num++;
			}		
			
		}
		//单行总字节数
		sum0 = chinanes*2+num;	

		for (j=0;j<sum0;j++)
		{
			sMsg0[j]=TmpMsg[j];
		}
		LineLen=strlen(sMsg0);
		//显示单行的字符串信息
		TextOut(hDC,5,StartY,sMsg0,LineLen);
		//除去第一行剩下的字符串字节数
		len = len - sum0;
		//以下做了五次判断均与第一行的判断相同，由于每行的中文个数与字母和标点的个数不确定，所以这里我是单行判断
		//总显示的行数为六行。
		if (len > LineNum-1)
		{
			chinanes = 0;
			num = 0;
			for (j=sum0;j<LineNum-1+sum0;j++)
			{
				if(str[j] & 0x80)//判断最后一个字是不是中文
				{
					chinanes++;
					j++;
				}
				else
				{
					num++;
				}		
				
			}
			sum1 = chinanes*2+num+sum0;	
			for (j=sum0;j<sum1;j++)
			{
				sMsg1[j-sum0]=TmpMsg[j];
			}
			LineLen=strlen(sMsg1);
			TextOut(hDC,5,StartY+FontSize,sMsg1,LineLen);
			len = len - sum1+sum0;
			
			if (len>LineNum-1)
			{
				chinanes = 0;
				num = 0;	
				for (j=sum1;j<LineNum-1+sum1;j++)
				{
					if(str[j] & 0x80)//判断最后一个字是不是中文
					{
						chinanes++;
						j++;
					}
					else
					{
						num++;
					}							
				}
				sum2 = chinanes*2+num+sum1;	
				for (j=sum1;j<sum2;j++)
				{
					sMsg2[j-sum1]=TmpMsg[j];
				}
				LineLen=strlen(sMsg2);
				TextOut(hDC,5,StartY+FontSize*2,sMsg2,LineLen);
				len = len - sum2 + sum1;
				
				if (len > LineNum-1)
				{
					chinanes = 0;
					num = 0;	
					for (j=sum2;j<LineNum-1+sum2;j++)
					{
						if(str[j] & 0x80)//判断最后一个字是不是中文
						{
							chinanes++;
							j++;
						}
						else
						{
							num++;
						}		
						
					}
					sum3 = chinanes*2+num+sum2;	
					for (j=sum2;j<sum3;j++)
					{
						sMsg3[j-sum2]=TmpMsg[j];
					}
					LineLen=strlen(sMsg3);
					TextOut(hDC,5,StartY+FontSize*3,sMsg3,LineLen);
					
					len = len - sum3 + sum2;
					
					if (len > LineNum-1)
					{
						chinanes = 0;
						num = 0;	
						for (j=sum3;j<LineNum-1+sum3;j++)
						{
							if(str[j] & 0x80)//判断最后一个字是不是中文
							{
								chinanes++;
								j++;
							}
							else
							{
								num++;
							}		
							
						}
						sum4 = chinanes*2+num+sum3;	
						for (j=sum3;j<sum4;j++)
						{
							sMsg4[j-sum3]=TmpMsg[j];
						}
						LineLen=strlen(sMsg4);
						TextOut(hDC,5,StartY+FontSize*4,sMsg4,LineLen);
						
						len = len - sum4 + sum3;
						if (len > LineNum-1)
						{
							chinanes = 0;
							num = 0;	
							for (j=sum4;j<LineNum-1+sum4;j++)
							{
								if(str[j] & 0x80)//判断最后一个字是不是中文
								{
									chinanes++;
									j++;
								}
								else
								{
									num++;
								}		
								
							}
							sum5 = chinanes*2+num+sum4;	
							for (j=sum4;j<sum5;j++)
							{
								sMsg5[j-sum4]=TmpMsg[j];
							}
							LineLen=strlen(sMsg5);
							TextOut(hDC,5,StartY+FontSize*5,sMsg5,LineLen);
							len = len - sum5 + sum4;
						}
						else
						{
							chinanes = 0;
							num = 0;	
							for (j=sum4;j<len+sum4;j++)
							{
								if(str[j] & 0x80)//判断最后一个字是不是中文
								{
									chinanes++;
									j++;
								}
								else
								{
									num++;
								}		
								
							}
							sum5 = chinanes*2+num+sum4;	
							for (j=sum4;j<sum5;j++)
							{
								sMsg5[j-sum4]=TmpMsg[j];
							}
							LineLen=strlen(sMsg5);
							TextOut(hDC,5,StartY+FontSize*5,sMsg5,LineLen);
						}
					}
					else
					{
						chinanes = 0;
						num = 0;	
						for (j=sum3;j<len+sum3;j++)
						{
							if(str[j] & 0x80)//判断最后一个字是不是中文
							{
								chinanes++;
								j++;
							}
							else
							{
								num++;
							}		
							
						}
						sum4 = chinanes*2+num+sum3;	
						for (j=sum3;j<sum4;j++)
						{
							sMsg4[j-sum3]=TmpMsg[j];
						}
						LineLen=strlen(sMsg4);
						TextOut(hDC,5,StartY+FontSize*4,sMsg4,LineLen);
					}
				}
				else
				{
					chinanes = 0;
					num = 0;	
					for (j=sum2;j<len+sum2;j++)
					{
						if(str[j] & 0x80)//判断最后一个字是不是中文
						{
							chinanes++;
							j++;
						}
						else
						{
							num++;
						}		
						
					}
					sum3 = chinanes*2+num+sum2;	
					for (j=sum2;j<sum3;j++)
					{
						sMsg3[j-sum2]=TmpMsg[j];
					}
					LineLen=strlen(sMsg3);
					TextOut(hDC,5,StartY+FontSize*3,sMsg3,LineLen);
				}
			}	
			else
			{
				chinanes = 0;
				num = 0;	
				for (j=sum1;j<len+sum1;j++)
				{
					if(str[j] & 0x80)//判断最后一个字是不是中文
					{
						chinanes++;
						j++;
					}
					else
					{
						num++;
					}		
					
				}
				sum2 = chinanes*2+num+sum1;	
				for (j=sum1;j<sum2;j++)
				{
					sMsg2[j-sum1]=TmpMsg[j];
				}
				LineLen=strlen(sMsg2);
				TextOut(hDC,5,StartY+FontSize*2,sMsg2,LineLen);
			}
		}
		else
		{
			chinanes = 0;
			num = 0;
			for (j=sum0;j<len+sum0;j++)
			{
				if(str[j] & 0x80)//判断最后一个字是不是中文
				{
					chinanes++;
					j++;
				}
				else
				{
					num++;
				}		
				
			}
			sum1 = chinanes*2+num+sum0;	
			for (j=sum0;j<sum1;j++)
			{
				sMsg1[j-sum0]=TmpMsg[j];
			}
			LineLen=strlen(sMsg1);
			TextOut(hDC,5,StartY+FontSize,sMsg1,LineLen);
		}
	}
	else
	{
		for (j=0;j<len;j++)
		{
			if(str[j] & 0x80)//判断最后一个字是不是中文
			{
				chinanes++;
				j++;
			}
			else
			{
				num++;
			}		
			
		}
		sum0 = chinanes*2+num;	
		for (j=0;j<sum0;j++)
		{
			sMsg0[j]=TmpMsg[j];
		}
		LineLen=strlen(sMsg0);
		TextOut(hDC,5,StartY,sMsg0,LineLen);	
	}
	
	
	SetBkMode(hDC,bkmode);
	SelectObject(hDC, (HFONT)OldFont);
	SelectObject(hDC, (HBRUSH)OldBrush);
	
	DeleteObject(hFont);
	DeleteObject(hBrush);
	::ReleaseDC(hDlg, hDC);
	
	return;		
}

int CheckBarCode(char *Barcode,char *ErrorMsg)
{
	CoInitialize(NULL);
	ISoapConnectorPtr SoapConnector;
	ISoapSerializerPtr Serializer;
	ISoapReaderPtr Reader;
	if (strlen(Barcode)==0)
	{
		strcpy(ErrorMsg,"Barcode为空");
		return -1;
	}
	
	HRESULT hr;
	try
	{
		//创建SoapConnector类的对象
		SoapConnector.CreateInstance(__uuidof(HttpConnector30)); 
		
		//指定Web服务的地址
		SoapConnector->Property ["EndPointURL"] ="http://192.168.58.66:1123/pdaservice.asmx";
		
		//与Web服务连接
		hr=SoapConnector->Connect();
		if(FAILED(hr)) 
		{
			strcpy(ErrorMsg,"Connect Error");
			return -1;
		}
		
		//指定Web服务完成的操作
		SoapConnector->Property ["SoapAction"] = "http://tempuri.org/CheckBarCode";
		
		//准备发送消息给Web服务
		SoapConnector->BeginMessage();
		// 创建SoapSerializer对象
		Serializer.CreateInstance(__uuidof(SoapSerializer30)); 
		
		// 将serializer连接到connector的输入字符串
		Serializer->Init(_variant_t((IUnknown*)SoapConnector->InputStream)); 
		
		// 创建SOAP消息
		Serializer->StartEnvelope("","","");
		
		Serializer->StartBody(""); 
		
		Serializer->StartElement("CheckBarCode","http://tempuri.org/","","");
		
		Serializer->StartElement("BarCode","http://tempuri.org/","",""); 
		Serializer->WriteString((_bstr_t)Barcode);
		Serializer->EndElement();
		
		Serializer->EndElement();
		
		Serializer->EndBody(); 
		Serializer->EndEnvelope();   
		
		//消息真正地发给Web服务
		hr=SoapConnector->EndMessage(); 
		if(FAILED(hr)) return -1;
		
		// 读取响应
		Reader.CreateInstance(__uuidof(SoapReader30)); 
		
		// 将reader联接到connector的输出字符串
		Reader->Load(_variant_t((IUnknown*)SoapConnector->OutputStream), _T("")); 
		
		CString TempLog;		
		TempLog.Format("%s",(const char *)Reader->RpcResult->text);
		
		if (TempLog.Find("True")!=-1)
		{
			CString Tempbar=TempLog.Right(TempLog.GetLength()-4);
			if (Tempbar==Barcode)
			{
				return 1;
			}
			else
			{
				strcpy(ErrorMsg,"系统返回Barcode错误");
				return -1;
			}
			
		}
		else if (TempLog.Find("False")!=-1)
		{
			CString TempMsg;
			TempMsg=TempLog.Right(TempLog.GetLength()-5);
			strcpy(ErrorMsg,TempMsg.GetBuffer(TempMsg.GetLength()));
			return 0;
		}
		else
		{
			strcpy(ErrorMsg,TempLog.GetBuffer(TempLog.GetLength()));
			return -1;
		}
		
	}
	catch (_com_error e)
	{
		return -1;
	}	
}

int CheckPKList(char *PKList,char *ErrorMsg)
{
	CoInitialize(NULL);
	ISoapConnectorPtr SoapConnector;
	ISoapSerializerPtr Serializer;
	ISoapReaderPtr Reader;
	
	HRESULT hr;
	try
	{
		//创建SoapConnector类的对象
		SoapConnector.CreateInstance(__uuidof(HttpConnector30)); 
		
		//指定Web服务的地址
		SoapConnector->Property ["EndPointURL"] ="http://192.168.58.66:1123/pdaservice.asmx";
		
		//与Web服务连接
		hr=SoapConnector->Connect();
		if(FAILED(hr)) 
		{
			strcpy(ErrorMsg,"Connect Error");
			return -1;
		}
		
		//指定Web服务完成的操作
		SoapConnector->Property ["SoapAction"] = "http://tempuri.org/CheckPKLIST";
		
		//准备发送消息给Web服务
		SoapConnector->BeginMessage();
		// 创建SoapSerializer对象
		Serializer.CreateInstance(__uuidof(SoapSerializer30)); 
		
		// 将serializer连接到connector的输入字符串
		Serializer->Init(_variant_t((IUnknown*)SoapConnector->InputStream)); 
		
		// 创建SOAP消息
		Serializer->StartEnvelope("","","");
		
		Serializer->StartBody(""); 
		
		Serializer->StartElement("CheckPKLIST","http://tempuri.org/","","");
		
		Serializer->StartElement("pk_flist","http://tempuri.org/","",""); 
		Serializer->WriteString((_bstr_t)PKList);
		Serializer->EndElement();
		
		Serializer->EndElement();
		
		Serializer->EndBody(); 
		Serializer->EndEnvelope();   
		
		//消息真正地发给Web服务
		hr=SoapConnector->EndMessage(); 
		if(FAILED(hr)) return -1;
		
		// 读取响应
		Reader.CreateInstance(__uuidof(SoapReader30)); 
		
		// 将reader联接到connector的输出字符串
		Reader->Load(_variant_t((IUnknown*)SoapConnector->OutputStream), _T("")); 
		
		CString TempLog;		
		TempLog.Format("%s",(const char *)Reader->RpcResult->xml);
		
		int StartPos=TempLog.Find("<IsSuccess>")+11;
		int EndPos=TempLog.Find("</IsSuccess>");
		CString CheckResult=TempLog.Mid(StartPos,EndPos-StartPos);


		StartPos=TempLog.Find("<ErrCode>")+9;
		EndPos=TempLog.Find("</ErrCode>");
		CString ErrorCode=TempLog.Mid(StartPos,EndPos-StartPos);

		StartPos=TempLog.Find("<ErrMsg>")+8;
		EndPos=TempLog.Find("</ErrMsg>");
		CString Msg=TempLog.Mid(StartPos,EndPos-StartPos);
		
		int Result=0;

		if (CheckResult=="PASS")
		{
			Result=1;
		}
		else if (CheckResult=="FAIL")
		{
			Result=0;
		}

		strcpy(ErrorMsg,Msg.GetBuffer(Msg.GetLength()));

		return Result;
		
	}
	catch (_com_error e)
	{
		return -1;
	}	
}

int Pack(char *PKList,char *barcode,char *ErrorMsg)
{
	CoInitialize(NULL);
	ISoapConnectorPtr SoapConnector;
	ISoapSerializerPtr Serializer;
	ISoapReaderPtr Reader;
	
	HRESULT hr;
	try
	{
		//创建SoapConnector类的对象
		SoapConnector.CreateInstance(__uuidof(HttpConnector30)); 
		
		//指定Web服务的地址
		SoapConnector->Property ["EndPointURL"] ="http://192.168.58.66:1123/pdaservice.asmx";
		
		//与Web服务连接
		hr=SoapConnector->Connect();
		if(FAILED(hr)) 
		{
			strcpy(ErrorMsg,"Connect Error");
			return -1;
		}
		
		//指定Web服务完成的操作
		SoapConnector->Property ["SoapAction"] = "http://tempuri.org/Pack";
		
		//准备发送消息给Web服务
		SoapConnector->BeginMessage();
		// 创建SoapSerializer对象
		Serializer.CreateInstance(__uuidof(SoapSerializer30)); 
		
		// 将serializer连接到connector的输入字符串
		Serializer->Init(_variant_t((IUnknown*)SoapConnector->InputStream)); 
		
		// 创建SOAP消息
		Serializer->StartEnvelope("","","");
		
		Serializer->StartBody(""); 
		
		Serializer->StartElement("Pack","http://tempuri.org/","","");
		
		Serializer->StartElement("pk_flist","http://tempuri.org/","",""); 
		Serializer->WriteString((_bstr_t)PKList);
		Serializer->EndElement();

		Serializer->StartElement("scan_data","http://tempuri.org/","",""); 
		Serializer->WriteString((_bstr_t)barcode);
		Serializer->EndElement();
		
		Serializer->EndElement();
		
		Serializer->EndBody(); 
		Serializer->EndEnvelope();   
		
		//消息真正地发给Web服务
		hr=SoapConnector->EndMessage(); 
		if(FAILED(hr)) return -1;
		
		// 读取响应
		Reader.CreateInstance(__uuidof(SoapReader30)); 
		
		// 将reader联接到connector的输出字符串
		Reader->Load(_variant_t((IUnknown*)SoapConnector->OutputStream), _T("")); 
		
		CString TempLog;		
		TempLog.Format("%s",(const char *)Reader->RpcResult->xml);
		
		int StartPos=TempLog.Find("<IsSuccess>")+11;
		int EndPos=TempLog.Find("</IsSuccess>");
		CString CheckResult=TempLog.Mid(StartPos,EndPos-StartPos);
		
		
		StartPos=TempLog.Find("<ErrCode>")+9;
		EndPos=TempLog.Find("</ErrCode>");
		CString ErrorCode=TempLog.Mid(StartPos,EndPos-StartPos);
		
		StartPos=TempLog.Find("<ErrMsg>")+8;
		EndPos=TempLog.Find("</ErrMsg>");
		CString Msg=TempLog.Mid(StartPos,EndPos-StartPos);
		
		int Result=0;
		
		if (CheckResult=="PASS")
		{
			if (Msg=="3")
			{
				Result=1;
			}
			else
			{
				Result=2;
			}
		
		}
		else if (CheckResult=="FAIL")
		{
			Result=0;
		}
		
		strcpy(ErrorMsg,Msg.GetBuffer(Msg.GetLength()));
		
		return Result;
		
	}
	catch (_com_error e)
	{
		return -1;
	}	
}

int GetServerNo()
{
	int ServerNo = 0;
	char Path[512] = {0};
	GetModulePath(Path);
	strcat(Path,"MesSetting.dll");
	ServerNo = GetPrivateProfileInt("MESSetting","ServerNo",3,Path);
	return ServerNo;
}

bool GetModelforBarcode(char *Barcode,char Model[32],char *sErrorMsg)
{
	CoInitialize(NULL);
	ISoapConnectorPtr SoapConnector;
	ISoapSerializerPtr Serializer;
	ISoapReaderPtr Reader;
	
	HRESULT hr;
	try
	{
		//创建SoapConnector类的对象
		SoapConnector.CreateInstance(__uuidof(HttpConnector30)); 
		
		//指定Web服务的地址
		switch(GetServerNo())
		{
		case 0:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.63:1123/pdaservice.asmx";
			break;
		case 1:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.66:1123/pdaservice.asmx";
			break;
		case 2:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.67:1123/pdaservice.asmx";
			break;
		case 3:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.68:1123/pdaservice.asmx";
			break;
		default:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.67:1123/pdaservice.asmx";
			break;				
		}
		//与Web服务连接
		hr=SoapConnector->Connect();
		if(FAILED(hr)) 
		{
			strcpy(sErrorMsg,"服务器连接失败!");
			return false;
		}
		
		//指定Web服务完成的操作
		SoapConnector->Property ["SoapAction"] = "http://tempuri.org/GetModelforBarcode";
		
		//准备发送消息给Web服务
		SoapConnector->BeginMessage();
		// 创建SoapSerializer对象
		Serializer.CreateInstance(__uuidof(SoapSerializer30)); 
		
		// 将serializer连接到connector的输入字符串
		Serializer->Init(_variant_t((IUnknown*)SoapConnector->InputStream)); 
		
		// 创建SOAP消息
		Serializer->StartEnvelope("","","");
		
		Serializer->StartBody(""); 
		
		Serializer->StartElement("GetModelforBarcode","http://tempuri.org/","","");
		
		Serializer->StartElement("Barcode","http://tempuri.org/","",""); 
		Serializer->WriteString((_bstr_t)Barcode);
		Serializer->EndElement();
		
		Serializer->EndElement();
		
		Serializer->EndBody(); 
		Serializer->EndEnvelope();   
		
		//消息真正地发给Web服务
		hr=SoapConnector->EndMessage(); 
		if(FAILED(hr))
		{
			strcpy(sErrorMsg,"Mes端口无响应!");
			return false;
		}
		
		// 读取响应
		Reader.CreateInstance(__uuidof(SoapReader30)); 
		
		// 将reader联接到connector的输出字符串
		Reader->Load(_variant_t((IUnknown*)SoapConnector->OutputStream), _T("")); 
		
		CString TempLog;
		TempLog.Format("%s",(const char *)Reader->RpcResult->text);
		if (TempLog == "<el_no />")
		{
			strcpy(sErrorMsg,"无效的barcode!");
			return false;
		}

		if (TempLog == "soap:Client")
		{
			strcpy(sErrorMsg,"服务器选择错误!");
			return false;
		}
		int namingrule=0;
		if (TempLog.Find("L",0) == 14)
		{
			namingrule =1;
		}
		if (namingrule)
		{
			int length=0;
			length = TempLog.GetLength();	
			TempLog.Delete(0,14);
			int EndPos = TempLog.Find("</el_no></el_no>",0);
			TempLog.Delete(EndPos,16);
		    strcpy(Model,TempLog.GetBuffer(TempLog.GetLength()));
		}
		else
		{
			CString tempModel = TempLog.Mid(14,7);
			strcpy(Model,tempModel.GetBuffer(TempLog.GetLength()));
		}
		
		return true;
 	}
	catch (_com_error e)
	{
		//AfxMessageBox(e.Description());
		return false;
	}	
}

bool GetModelforFuseid(char *Fuseid, char Model[32],char *sErrorMsg)
{
	CoInitialize(NULL);
	ISoapConnectorPtr SoapConnector;
	ISoapSerializerPtr Serializer;
	ISoapReaderPtr Reader;
	
	HRESULT hr;
	try
	{
		//创建SoapConnector类的对象
		SoapConnector.CreateInstance(__uuidof(HttpConnector30)); 
		
		//指定Web服务的地址	
		switch(GetServerNo())
		{
		case 0:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.63:1123/pdaservice.asmx";
			break;
		case 1:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.66:1123/pdaservice.asmx";
			break;
		case 2:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.67:1123/pdaservice.asmx";
			break;
		case 3:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.68:1123/pdaservice.asmx";
			break;
		default:
			SoapConnector->Property ["EndPointURL"] ="http://192.168.58.67:1123/pdaservice.asmx";
			break;				
		}
		
		//与Web服务连接
		hr=SoapConnector->Connect();
		if(FAILED(hr)) 
		{
			strcpy(sErrorMsg,"服务器连接失败!");
			return false;
		}
		
		//指定Web服务完成的操作
		SoapConnector->Property ["SoapAction"] = "http://tempuri.org/GetModelforFuseid";
		
		//准备发送消息给Web服务
		SoapConnector->BeginMessage();
		// 创建SoapSerializer对象
		Serializer.CreateInstance(__uuidof(SoapSerializer30)); 
		
		// 将serializer连接到connector的输入字符串
		Serializer->Init(_variant_t((IUnknown*)SoapConnector->InputStream)); 
		
		// 创建SOAP消息
		Serializer->StartEnvelope("","","");
		
		Serializer->StartBody(""); 
		
		Serializer->StartElement("GetModelforFuseid","http://tempuri.org/","","");
		
		Serializer->StartElement("Barcode","http://tempuri.org/","",""); 
		Serializer->WriteString((_bstr_t)Fuseid);
		Serializer->EndElement();
		
		Serializer->EndElement();
		
		Serializer->EndBody(); 
		Serializer->EndEnvelope();   
		
		//消息真正地发给Web服务
		hr=SoapConnector->EndMessage(); 
		if(FAILED(hr))
		{
			strcpy(sErrorMsg,"Mes端口相应失败!");
			return false;
		}
		
		// 读取响应
		Reader.CreateInstance(__uuidof(SoapReader30)); 
		
		// 将reader联接到connector的输出字符串
		Reader->Load(_variant_t((IUnknown*)SoapConnector->OutputStream), _T("")); 
		
		CString TempLog;
		TempLog.Format("%s",(const char *)Reader->RpcResult->text);

		if (TempLog == "<el_no />")
		{
			strcpy(sErrorMsg,"无效的barcode!");
			return false;
		}
		if (TempLog == "soap:Client")
		{
			strcpy(sErrorMsg,"服务器选择错误!");
			return false;
		}

		int namingrule=0;
		if (TempLog.Find("L",0) == 14)
		{
			namingrule =1;
		}
		if (namingrule)
		{
			int length=0;
			length = TempLog.GetLength();	
			TempLog.Delete(0,14);
			int EndPos = TempLog.Find("</el_no></el_no>",0);
			TempLog.Delete(EndPos,16);
			strcpy(Model,TempLog.GetBuffer(TempLog.GetLength()));
		}
		else
		{
			CString tempModel = TempLog.Mid(14,7);
			strcpy(Model,tempModel.GetBuffer(TempLog.GetLength()));
		}
		return true;
 	}
	catch (_com_error e)
	{
	//	AfxMessageBox(e.Description());
		return false;
	}	
}
