#include "StdAfx.h"
#include "AF_SFR_TestItem.h"
#include "ImageProc.h"
#include "CurveFit2.h"
#include "SFR_Process.h"
#include "float.h"
#include "ofSFR.h"

#pragma comment(lib,"SFR_LENOVO.lib")
#pragma comment(lib,"CurveFit.lib")
#pragma comment(lib,"BoardChartAutoROI.lib")
#pragma comment(lib,"ofSFR.lib")

#include <windows.h>    
#include <psapi.h>    
#pragma comment(lib,"psapi.lib")  

const int SMALLRANGE = 165;
void AF_SFR_TestItem::SFR_Fov_6()
{
	//m_dSFRValue_6[MAXROICOUNT] = {0.0};
	int startsfr3f = 0;
	int endsfr3f = 0;
	int startsfr4f = 0;
	int endsfr4f = 0;
	int startsfr5f = 0;
	int endsfr5f = 0;
	int startsfr6f = 0;
	int endsfr6f  = 0;
	int startsfr8f = 0;
	int endsfr8f  = 0;

	int roiCnt = 1;
	memset(m_dSFRValue_6,0,sizeof(float)*MAXROICOUNT);

	if (pDlg->m_bTestF3 == 1)
	{
		startsfr3f = roiCnt + 1;
		roiCnt = endsfr3f = roiCnt + 8;
	}
	else
	{
		startsfr3f = 0;
		endsfr3f = 0;
	}

	if (pDlg->m_bTestF4 == 1)
	{
		startsfr4f = roiCnt + 1;
		roiCnt = endsfr4f = roiCnt + 8;
	}
	else
	{
		startsfr4f = 0;
		endsfr4f = 0;
	}

	if (pDlg->m_bTestF5 == 1)
	{
		startsfr5f = roiCnt + 1;
		roiCnt = endsfr5f = roiCnt + 8;
	}
	else
	{
		startsfr5f = 0;
		endsfr5f = 0;
	}

	if (pDlg->m_bTestF6 == 1)
	{
		startsfr6f = roiCnt + 1;
		roiCnt = endsfr6f = roiCnt + 8;
	}
	else
	{
		startsfr6f = 0;
		endsfr6f = 0;
	}

	if (pDlg->m_bTestF8 == 1)
	{
		startsfr8f = roiCnt + 1;
		roiCnt = endsfr8f = roiCnt + 8;
	}
	else
	{
		startsfr8f = 0;
		endsfr8f = 0;
	}



	if (pDlg->m_bTestF6)
	{
		if(m_iSFRROICount >= endsfr6f)
		{
			m_dSFRValue_6[0] = m_dSFRValue[0];
			m_dSFRValue_6[1] = m_dSFRValue[1];
						//if(m_iSFRROICount
			for (int index = startsfr6f; index <= endsfr6f; index++)
			{
				m_dSFRValue_6[index - startsfr6f + 2] = m_dSFRValue[index];
			}

			++ m_iMiniStepIndex_6;
			AppendAfBurnFile(_T(""),m_iMiniStepIndex_6,m_iCode,m_dSFRValue_6);
			
		}


	}
	//for(int i = 0; i < 16; i ++)

	
}
int GetBestCode1(CString afMode, AF_STEP_DATA stepData, int stepCode, int* startCode, int* endCode, float* bestAv)
{
	int ret = 0;

	float maxAV = 0.0f;
	int peakCode = 0, peakCnt = 0;
	if (afMode == _T("BIG_STEP_MODE"))
	{
		for (int i = 0; i<stepData.bigStepCnt; i++)
		{
			if (stepData.bigStepData[i].av > maxAV)
			{
				maxAV = stepData.bigStepData[i].av;
				peakCode = stepData.bigStepData[i].code;
				peakCnt = i;
			}
		}

		if (peakCode == stepData.bigStepData[0].code)
		{
			int tmpCode = stepData.bigStepData[0].code - stepCode;
			if (tmpCode < 0)
				tmpCode = 0;
			*startCode = tmpCode;
			*endCode = stepData.bigStepData[0].code + stepCode;
			*bestAv = stepData.bigStepData[0].av;
			return stepData.bigStepData[0].code;
		}

		if (peakCode == stepData.bigStepData[stepData.bigStepCnt - 1].code)
		{
			int tmpCode = stepData.bigStepData[stepData.bigStepCnt - 1].code - stepCode;
			if (tmpCode < 0)
				tmpCode = 0;
			*startCode = tmpCode;
			*endCode = stepData.bigStepData[stepData.bigStepCnt - 1].code + stepCode;
			*bestAv = stepData.bigStepData[stepData.bigStepCnt - 1].av;
			return stepData.bigStepData[stepData.bigStepCnt - 1].code;
		}
	}
	else
	{
		for (int i = 0; i<stepData.miniStepCnt; i++)
		{
			if (stepData.miniStepData[i].av > maxAV)
			{
				maxAV = stepData.miniStepData[i].av;
				peakCode = stepData.miniStepData[i].code;
				peakCnt = i;
			}
		}

		if (peakCode == stepData.miniStepData[0].code)
		{
			int tmpCode = stepData.miniStepData[0].code - stepCode;
			if (tmpCode < 0)
				tmpCode = 0;
			*startCode = tmpCode;
			*endCode = stepData.miniStepData[0].code + stepCode;
			*bestAv = stepData.miniStepData[0].av;
			return stepData.miniStepData[0].code;
		}

		if (peakCode == stepData.miniStepData[stepData.miniStepCnt - 1].code)
		{
			int tmpCode = stepData.miniStepData[stepData.miniStepCnt - 1].code - stepCode;
			if (tmpCode < 0)
				tmpCode = 0;
			*startCode = tmpCode;
			*endCode = stepData.miniStepData[stepData.miniStepCnt - 1].code + stepCode;
			*bestAv = stepData.miniStepData[stepData.miniStepCnt - 1].av;
			return stepData.miniStepData[stepData.miniStepCnt - 1].code;
		}
	}

	if (peakCnt < 1)
		peakCnt = 1;

	float y1 = 0.0f, y2 = 0.0f, y3 = 0.0f;
	int x1 = 0, x2 = 0, x3 = 0;
	if (afMode == _T("BIG_STEP_MODE"))
	{
		y1 = stepData.bigStepData[peakCnt - 1].av;
		y2 = stepData.bigStepData[peakCnt].av;
		y3 = stepData.bigStepData[peakCnt + 1].av;

		x1 = stepData.bigStepData[peakCnt - 1].code;
		x2 = stepData.bigStepData[peakCnt].code;
		x3 = stepData.bigStepData[peakCnt + 1].code;
	}
	else
	{
		y1 = stepData.miniStepData[peakCnt - 1].av;
		y2 = stepData.miniStepData[peakCnt].av;
		y3 = stepData.miniStepData[peakCnt + 1].av;

		x1 = stepData.miniStepData[peakCnt - 1].code;
		x2 = stepData.miniStepData[peakCnt].code;
		x3 = stepData.miniStepData[peakCnt + 1].code;
	}

	float a = ((y1 - y2) * (x2 - x3) - (y2 - y3) * (x1 - x2)) / ((x1 * x1 - x2 * x2) * (x2 - x3) - (x2 * x2 - x3 * x3) * (x1 - x2));
	float b = ((y1 - y2) * (x2 - x3) - a * (x1 * x1 - x2 * x2) * (x2 - x3)) / ((x1 - x2) * (x2 - x3));
	float c = y1 - a * x1 * x1 - b * x1;

	ret = (int)(b * (-1.0f) / (2 * a));
	*bestAv = (4 * a * c - b * b) / (4 * a);

	if (afMode == _T("BIG_STEP_MODE"))
	{
		if (ret < stepData.bigStepData[0].code)
			ret = stepData.bigStepData[0].code;
	}
	else
	{
		if (ret < stepData.miniStepData[0].code)
			ret = stepData.miniStepData[0].code;
	}

	if (afMode == _T("BIG_STEP_MODE"))
	{
		for (int i = 0; i<stepData.bigStepCnt - 1; i++)
		{
			if ((stepData.bigStepData[i].code <= ret) && (stepData.bigStepData[i + 1].code >= ret))
			{
				if ((stepData.bigStepData[i].code + stepCode / 3) > ret)
				{
					*startCode = stepData.bigStepData[i - 1].code;
					*endCode = stepData.bigStepData[i + 1].code;
				}
				else if ((stepData.bigStepData[i + 1].code - stepCode / 3) < ret)
				{
					*startCode = stepData.bigStepData[i].code;
					*endCode = stepData.bigStepData[i + 2].code;
				}
				else
				{
					*startCode = stepData.bigStepData[i].code;
					*endCode = stepData.bigStepData[i + 1].code;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i<stepData.miniStepCnt - 1; i++)
		{
			if ((stepData.miniStepData[i].code <= ret) && (stepData.miniStepData[i + 1].code >= ret))
			{
				if ((stepData.miniStepData[i].code + stepCode / 3) > ret)
				{
					*startCode = stepData.miniStepData[i - 1].code;
					*endCode = stepData.miniStepData[i + 1].code;
				}
				else if ((stepData.miniStepData[i + 1].code - stepCode / 3) < ret)
				{
					*startCode = stepData.miniStepData[i].code;
					*endCode = stepData.miniStepData[i + 2].code;
				}
				else
				{
					*startCode = stepData.miniStepData[i].code;
					*endCode = stepData.miniStepData[i + 1].code;
				}
			}
		}
	}

	return ret;
}

#define  LOGHEIGHT 200
#define  SFRVCOLOR RGB(0,255,0)
#define  SFRHCOLOR RGB(0,210,255)
enum OUTFORMAT_RGB
{
	OUTFORMAT_RGGB = 0,///<RGGB输出格式
	OUTFORMAT_GRBG,///<GRBG输出格式
	OUTFORMAT_GBRG,///<GBRG输出格式
	OUTFORMAT_BGGR,///<BGGR输出格式
};

enum AFTEST_MODE
{
	NORMAL_MODE = 0,
	FAST_MODE,
	FQC_MODE,
};

#define ENGINEMODE _T("EngineMode_En")
#define MIDLLEVALUE_EN _T("MiddleValue_En")
#define SHAKE_EN  _T("Shake_En")
#define STARTCODE _T("StartCode")
#define ENDCODE _T("EndCode")
#define STEP1 _T("Step1")
#define STEP2 _T("Step2")
#define STEP3 _T("Step3")
#define SKIPFRAME _T("SkipFrame")
#define CTC8 _T("CTC8")
#define CTC6 _T("CTC6")
#define CTC5 _T("CTC5")
#define CTC8H _T("CTC8H")
#define CTC8V _T("CTC8V")
#define CTC6H _T("CTC6H")
#define CTC6V _T("CTC6V")
#define CTC5H _T("CTC5H")
#define CTC5V _T("CTC5V")
#define SHAKESKIPFRAME _T("ShakeSkipFrame")
#define SHAKECODE _T("ShakeCode")
#define SHAKECOUNT _T("ShakeCount")
#define  PEAKMIN _T("PeakMin")
#define PEAKMAX _T("PeakMax")
#define SFRSPEC _T("SFRSpec")
#define STATIONNAME _T("StationName")
#define SFRBURNRATE _T("SFRBurnRate")
#define ROIWIDTH _T("ConctrastROIWidth")
#define ROIHeight _T("ContrastROIHeight")
#define ROISTARTX _T("ROIStartX")
#define ROISTARTY _T("ROIStartY")


#define MAX_ROI_NUMBER 25

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */

		bool Save(char* FileName,unsigned char* Data,int Width,int Height)	//	块Bmp郎
		{
			bool res = true;

			if ((Width == 0)||(Height == 0))
			{
				return false;
			}
			int ScanLineSize = 0;
			double ModValue = int(fmod(double(Width)*3,4));
			if (ModValue == 0)
			{
				ScanLineSize = (Width)*3;
			}
			else
			{
				ScanLineSize = (Width)*3+int((ModValue == 0)? (0):(4-ModValue));
			}

			unsigned char* pBitmapBits = new unsigned char[ScanLineSize*(Height)];
			memset(pBitmapBits,0,ScanLineSize*(Height));

			for (int i=0;i<Height;i++)
			{
				for (int j=0;j<Width;j++)
				{
					pBitmapBits[(Height-1-i)*ScanLineSize+j*3+2] = Data[(i*Width+j)*3+2];
					pBitmapBits[(Height-1-i)*ScanLineSize+j*3+1] = Data[(i*Width+j)*3+1];
					pBitmapBits[(Height-1-i)*ScanLineSize+j*3+0] = Data[(i*Width+j)*3+0];
				}
			}
			LONG lWidth = Width;
			LONG lHeight = Height;
			WORD wBitsPerPixel = 24;
			BITMAPINFOHEADER bmpInfoHeader = {0};
			// Set the size
			bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
			// Bit count
			bmpInfoHeader.biBitCount = wBitsPerPixel;
			// Use all colors
			bmpInfoHeader.biClrImportant = 0;
			// Use as many colors according to bits per pixel
			bmpInfoHeader.biClrUsed = 0;
			// Store as Uncompressed
			bmpInfoHeader.biCompression = BI_RGB;
			// Set the height in pixels
			bmpInfoHeader.biHeight = lHeight;
			// Width of the Image in pixels
			bmpInfoHeader.biWidth = lWidth;
			// Default number of planes
			bmpInfoHeader.biPlanes = 1;
			// Calculate the image size in bytes
			bmpInfoHeader.biSizeImage = ScanLineSize*(Height); 

			BITMAPFILEHEADER bfh = {0};
			// This value should be values of BM letters i.e 0⊙4D42
			// 0⊙4D = M 0⊙42 = B storing in reverse order to match with endian
			bfh.bfType = 0x4D42;
			/* or
			bfh.bfType = ˉBˇ+(ˇMˇ << 8);
			// <<8 used to shift ˉMˇ to end
			*/
			// Offset to the RGBQUAD
			bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
			// Total size of image including size of headers
			bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;
			// Create the file in disk to write
			HANDLE hFile = CreateFileA(FileName,GENERIC_WRITE, 0,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL); 

			if( !hFile ) // return if error opening file
			{
				res = false;
				SAFE_DELETE_ARRAY(pBitmapBits);
				//OfMSGBOX(TEXT("Some error, Can't open this file"));

				return res;
			} 

			DWORD dwWritten = 0;
			// Write the File header
			WriteFile( hFile, &bfh, sizeof(bfh), &dwWritten , NULL );
			// Write the bitmap info header
			WriteFile( hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwWritten, NULL );
			// Write the RGB Data
			WriteFile( hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &dwWritten, NULL );
			// Close the file handle
			CloseHandle( hFile );

			SAFE_DELETE_ARRAY(pBitmapBits);

			return res;	
		}

		BOOL bSaveRawFile( char* sfilename, BYTE *pBuffer, DWORD BufSize)
		{	

			if(BufSize<=0) return FALSE;

			HFILE rawFile;
			DWORD dwSizeImage = BufSize;

			rawFile = _lcreat( sfilename, 0 );
			if( rawFile < 0 )
				return FALSE;

			UINT len;
			len = _lwrite( rawFile, (LPSTR)pBuffer, dwSizeImage );

			_lclose( rawFile );

			return TRUE;	
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
	OutputDebugStringA(Budout);
	va_end(ap);
	char FileContent[16384]={0};
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);

	char TimeTemp[64]={0};
	char FileNameTemp[64]={0};
	CreateDirectoryA("TempLog",NULL);
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
extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new AF_SFR_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
AF_SFR_TestItem::AF_SFR_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	m_cfgName = CString(lpszName);
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化



	//.....
}

AF_SFR_TestItem::~AF_SFR_TestItem(void)
{

}


int AF_SFR_TestItem::InitItem()
{
	BOOL Bres = SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);
	if(!Bres)
	{
		AfxMessageBox(_T("内存初始化失败!!!"));
	}
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption();	
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AF_SFR_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码

	//SetProcessWorkingSetSize(GetCurrentProcess(),-1,-1);//内存初始化
	memset(m_dSFRValue_6,0,sizeof(float)*MAXROICOUNT);
	m_iMiniStepIndex_6 = 0;
	bRoiFail = false;
	CString path=m_pInterface->Ctrl_GetCurrentPath();
	path=path+L"ofset.ini";

	SetConfigPath(path);	 //设置CONFIG路径	

	SetName(L"OTPCheck");
	m_Options.m_FQCCode = ReadConfigInt(pDlg->m_UseCode_Name, 0);
	path=m_pInterface->Ctrl_GetCurrentPath();
	if (CamID==0)
	{
		path=path+L"ofset_Cam0.ini";
	} 
	else
	{
		path=path+L"ofset_Cam1.ini";
	}
	SetConfigPath(path);	 //设置CONFIG路径
	SetName(L"SFRROIPosition");
	WriteConfigInt(_T("ROI_Size"), pDlg->m_ROI_Size);  
	if (pDlg->m_bTestCenter)
	{
		WriteConfigInt(_T("H_B1"), 1);       WriteConfigInt(_T("V_B1"), 1);
	}
	else
	{
		WriteConfigInt(_T("H_B1"), 0);       WriteConfigInt(_T("V_B1"), 0);
	}

	if (pDlg->m_bTestF3)
	{
		WriteConfigInt(_T("H_B2"), 1);       WriteConfigInt(_T("V_B2"), 1);
		WriteConfigInt(_T("H_B3"), 1);       WriteConfigInt(_T("V_B3"), 1);
		WriteConfigInt(_T("H_B4"), 1);       WriteConfigInt(_T("V_B4"), 1);
		WriteConfigInt(_T("H_B5"), 1);       WriteConfigInt(_T("V_B5"), 1);
	}
	else
	{
		WriteConfigInt(_T("H_B2"), 0);       WriteConfigInt(_T("V_B2"), 0);
		WriteConfigInt(_T("H_B3"), 0);       WriteConfigInt(_T("V_B3"), 0);
		WriteConfigInt(_T("H_B4"), 0);       WriteConfigInt(_T("V_B4"), 0);
		WriteConfigInt(_T("H_B5"), 0);       WriteConfigInt(_T("V_B5"), 0);
	}

	if (pDlg->m_bTestF4)
	{
		WriteConfigInt(_T("H_B6"), 1);       WriteConfigInt(_T("V_B6"), 1);
		WriteConfigInt(_T("H_B7"), 1);       WriteConfigInt(_T("V_B7"), 1);
		WriteConfigInt(_T("H_B8"), 1);       WriteConfigInt(_T("V_B8"), 1);
		WriteConfigInt(_T("H_B9"), 1);       WriteConfigInt(_T("V_B9"), 1);
	}
	else
	{
		WriteConfigInt(_T("H_B6"), 0);       WriteConfigInt(_T("V_B6"), 0);
		WriteConfigInt(_T("H_B7"), 0);       WriteConfigInt(_T("V_B7"), 0);
		WriteConfigInt(_T("H_B8"), 0);       WriteConfigInt(_T("V_B8"), 0);
		WriteConfigInt(_T("H_B9"), 0);       WriteConfigInt(_T("V_B9"), 0);
	}

	if (pDlg->m_bTestF5)
	{
		WriteConfigInt(_T("H_B10"), 1);       WriteConfigInt(_T("V_B10"), 1);
		WriteConfigInt(_T("H_B11"), 1);       WriteConfigInt(_T("V_B11"), 1);
		WriteConfigInt(_T("H_B12"), 1);       WriteConfigInt(_T("V_B12"), 1);
		WriteConfigInt(_T("H_B13"), 1);       WriteConfigInt(_T("V_B13"), 1);
	}
	else
	{
		WriteConfigInt(_T("H_B10"), 0);       WriteConfigInt(_T("V_B10"), 0);
		WriteConfigInt(_T("H_B11"), 0);       WriteConfigInt(_T("V_B11"), 0);
		WriteConfigInt(_T("H_B12"), 0);       WriteConfigInt(_T("V_B12"), 0);
		WriteConfigInt(_T("H_B13"), 0);       WriteConfigInt(_T("V_B13"), 0);
	}

	if (pDlg->m_bTestF6)
	{
		WriteConfigInt(_T("H_B14"), 1);       WriteConfigInt(_T("V_B14"), 1);
		WriteConfigInt(_T("H_B15"), 1);       WriteConfigInt(_T("V_B15"), 1);
		WriteConfigInt(_T("H_B16"), 1);       WriteConfigInt(_T("V_B16"), 1);
		WriteConfigInt(_T("H_B17"), 1);       WriteConfigInt(_T("V_B17"), 1);
	}
	else
	{
		WriteConfigInt(_T("H_B14"), 0);       WriteConfigInt(_T("V_B14"), 0);
		WriteConfigInt(_T("H_B15"), 0);       WriteConfigInt(_T("V_B15"), 0);
		WriteConfigInt(_T("H_B16"), 0);       WriteConfigInt(_T("V_B16"), 0);
		WriteConfigInt(_T("H_B17"), 0);       WriteConfigInt(_T("V_B17"), 0);
	}

	if (pDlg->m_bTestF8)
	{
		WriteConfigInt(_T("H_B22"), 1);       WriteConfigInt(_T("V_B22"), 1);
		WriteConfigInt(_T("H_B23"), 1);       WriteConfigInt(_T("V_B23"), 1);
		WriteConfigInt(_T("H_B24"), 1);       WriteConfigInt(_T("V_B24"), 1);
		WriteConfigInt(_T("H_B25"), 1);       WriteConfigInt(_T("V_B25"), 1);
	}
	else
	{
		WriteConfigInt(_T("H_B22"), 0);       WriteConfigInt(_T("V_B22"), 0);
		WriteConfigInt(_T("H_B23"), 0);       WriteConfigInt(_T("V_B23"), 0);
		WriteConfigInt(_T("H_B24"), 0);       WriteConfigInt(_T("V_B24"), 0);
		WriteConfigInt(_T("H_B25"), 0);       WriteConfigInt(_T("V_B25"), 0);
	}

   //m_pInterface->Ctrl_SetCamErrorCode(CamID,m_Options.ErrorCode);
   //DebugInfo("ErrorCode = %d",m_Options.ErrorCode);

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(m_cfgName);

	//VCM初始化...
	m_pInterface->VcmDr_InitAF(CamID);
	m_pInterface->VcmDr_WriteAF_Code(0,CamID); //CODE归0

	memset(SFRAll,0,sizeof(double)*10*MAXROICOUNT);
	memset(m_stROIPos,0,sizeof(ROIPosInfo)*MAXROICOUNT);
	memset(m_dSFRValue,0,sizeof(float)*MAXROICOUNT);
	memset(m_stAFData,0,sizeof(AFData)*1024);
	memset(m_stSpec,0,sizeof(SpecInfo)*MAXROICOUNT);
	memset(m_stContrastData,0,sizeof(ContrastData)*100);
	memset(m_stMTFData,0,sizeof(MTFData)*100);
	memset(m_dSFRValue_Fact,0,sizeof(m_dSFRValue_Fact));
	memset(m_dSFRValue_Offset,0,sizeof(m_dSFRValue_Offset));
	m_bResult = FALSE;
	m_iSFRROICount = 0;
	m_bROIConfigureError = FALSE;
	m_bGotAllROI = false;
	m_iROICount = 0;
	m_fCTC8 = 0;
	m_fCTC8_H = 0;
	m_fCTC8_V = 0;
	m_fCTC6 = 0;
	m_fCTC6_H = 0;
	m_fCTC6_V = 0;
	m_fCTC5 = 0;
	m_fCTC5_H = 0;
	m_fCTC5_V = 0;
	m_iPeakDAC = 0;
	m_iPos = 0;
	m_nJumpPos = -1;
	m_iSkipFrame = 0;
	m_iShakeTime = 0;

	m_Options.m_iAFMethod = ReadConfigInt(_T("AFMethod"),0);
	//m_iStep = m_Options.m_iStep1;
	m_iStep = (m_Options.m_iEndCode - m_Options.m_iStartCode) / m_Options.m_iStep1;//6
	if (pDlg->m_AFTestMode == NORMAL_MODE)
	{
		m_iCode = m_Options.m_iStartCode;
	}
	else if (pDlg->m_AFTestMode == FAST_MODE)
	{
		m_Options.m_iStartCode =m_Options.m_FQCCode - pDlg->m_StartOffset;
		m_Options.m_iEndCode = m_Options.m_FQCCode + pDlg->m_EndOffset;
		m_iCode = m_Options.m_iStartCode;
		m_iStep = pDlg->m_FastMode_Step;
	}
	else if (pDlg->m_AFTestMode == FQC_MODE)
	{
		m_Options.m_iStartCode =m_Options.m_FQCCode - pDlg->m_StartOffset;
		m_Options.m_iEndCode = m_Options.m_FQCCode + pDlg->m_EndOffset;
		m_iCode = m_Options.m_FQCCode;
		m_iStep = pDlg->m_FastMode_Step;
	}

	m_iEndCode = m_Options.m_iEndCode;
	m_iPeroid = 0;
	m_bAFGoing = TRUE;
	m_iLastPos = 0;

	m_iFinalAutoROITime = 0;
	m_iFinalSFRTest = 0;
	m_bFocusing = TRUE;
	m_bFinalCheck = FALSE;
	fqcFirstTime = TRUE;
	iTestTimes = 0;
	memset(&m_ofDestDataOut,0,sizeof(OFAutoROIOut));
	AnalysisROISpec();

	m_iROIFailCount = 0;

	m_pInterface->Device_WriteI2c(0x20,0x301b, 0xf0, 3, CamID);
	m_pInterface->Device_WriteI2c(0x20,0x4d00, 0x03, 3, CamID);
	m_pInterface->Device_WriteI2c(0x20,0x4d01, 0xe2, 3, CamID);
	m_pInterface->Device_WriteI2c(0x20,0x4d02, 0xbb, 3, CamID);
	m_pInterface->Device_WriteI2c(0x20,0x4d03, 0x93, 3, CamID);
	m_pInterface->Device_WriteI2c(0x20,0x4d04, 0x48, 3, CamID);
	m_pInterface->Device_WriteI2c(0x20,0x4d05, 0x68, 3, CamID);

	return 0;
}


void AF_SFR_TestItem::AppendAfBurnFile(CString strAlgo, int cnt, int code, float roiVal[])
{
	/*int lu = _ttoi(pDlg->m_afPara.throuthFocusLU);
	int ru = _ttoi(pDlg->m_afPara.throuthFocusRU);
	int ld = _ttoi(pDlg->m_afPara.throuthFocusLD);
	int rd = _ttoi(pDlg->m_afPara.throuthFocusRD);*/

	CString filePath = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile.ini");
	
	{
		CString strsatationname(m_Options.m_StationName);
		CString key = _T(""), val = _T("");
		key.Format(_T("Position_%d"), cnt);
		val.Format(_T("%d"), code);
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("CEN_H%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[0]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("CEN_V%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[1]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("LU6_H%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[2]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("LU6_V%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[3]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("RU6_H%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[4]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);
		key.Format(_T("RU6_V%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[5]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

#if 0
		key.Format(_T("LD6_H%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[6]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("LD6_V%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[7]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("RD6_H%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[8]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("RD6_V%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[9]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);
#else
		key.Format(_T("RD6_H%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[6]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("RD6_V%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[7]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("LD6_H%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[8]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);

		key.Format(_T("LD6_V%d"), cnt);
		val.Format(_T("%d"), (int)((roiVal[9]) * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(strsatationname, key, val, filePath);
		
#endif
	}
	
}

/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AF_SFR_TestItem::Testing()
{
   //TODO:在此添加测试项代码
		 bResult = true; 
	if (TRUE == m_bROIConfigureError)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ROI Configure Error..."),COLOR_RED,220);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("ROI Configure Error"));
		m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
		bResult = false; 
		SetResult(RESULT_FAIL);
		return 0;
	}

	OfMarker stROIInfo[300] = {0};

	if (m_nJumpPos == -1)     //Shake
	{
		//Shake VCM --> 去除迟滞
		if (m_Options.m_bShakeEn && m_Options.m_iShakeCount > 0)
		{
			if (m_iSkipFrame < m_Options.m_iShakeSkipFrame)
			{
				m_iSkipFrame++;
				return 1;
			}
			if (m_iShakeTime % 2 == 0)
			{
				m_pInterface->VcmDr_WriteAF_Code(m_Options.m_iShakeDAC,CamID);
				m_iSkipFrame = 0;
			}else
			{
				m_pInterface->VcmDr_WriteAF_Code(0,CamID);
				m_iSkipFrame = 0;				
			}
			m_iShakeTime++;
			if (m_iShakeTime == m_Options.m_iShakeCount * 2)
			{
				m_iSkipFrame = 0;	
				if (pDlg->m_AFTestMode == FQC_MODE)
				{
					int  nCode = 0;
					CString temp = _T("");
					for (int i = 0; i< 5; i++)
					{
						nCode = (m_iCode-(4-i)*m_iStep);
						temp.Format(_T("Move to Code:%d"),nCode);
						m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_ORANGE,LOGHEIGHT);
						m_pInterface->VcmDr_WriteAF_Code(nCode,CamID);
					}
					m_nJumpPos =2 ;
				}
				else
				{
					m_pInterface->VcmDr_WriteAF_Code(m_iCode,CamID);	
				    m_nJumpPos++;
				}
			}
		}
		else
		{
			m_iSkipFrame = 0;	
			if (pDlg->m_AFTestMode == FQC_MODE)
			{
				int  nCode = 0;
				CString temp = _T("");
				for (int i = 0; i< 5; i++)
				{
					nCode = (m_iCode-(4-i)*m_iStep);
					temp.Format(_T("Move to Code:%d"),nCode);
					m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_ORANGE,LOGHEIGHT);
					m_pInterface->VcmDr_WriteAF_Code(nCode,CamID);
				}   
				m_nJumpPos =2 ;
			}
			else
			{
				m_pInterface->VcmDr_WriteAF_Code(m_iCode,CamID);	
				m_nJumpPos++;
			}
		}		
		return 1;
	}
	else if (m_nJumpPos == 0)     //对焦
	{
		if (m_iSkipFrame < m_Options.m_iSkipFrame)
		{	
			m_iSkipFrame++;
			if (m_Options.m_iAFMethod == 0 || m_iPeroid ==2 )
			{
				if (m_ofDestDataOut.ofROInumber != 0)
				{
					DrawText(m_stROIInfo,m_iROICount,m_iBlockCount);
				}
			}else
			{
				DrawContrastInfo();
			}
	
			return 1;
		}
		m_iSkipFrame = 0;

		CString temp;
		img  = *(m_pInterface->Ctrl_GetImgInfo(CamID));
		if (m_Options.m_iAFMethod == 0 || m_iPeroid == 2)
		{
			memset(m_stROIInfo,0,sizeof(OfMarker)*300);
			memset(&m_ofDestDataOut,0,sizeof(OFAutoROIOut));
			BOOL bAutoROI = AutoROI(m_ofDestDataOut);
			m_iBlockCount = m_ofDestDataOut.ofROInumber;

			if (bAutoROI)
			{
				m_iROIFailCount = 0;
				BOOL bSFRResult =-1;
				//if(bFlagCenterROI)
				{
					bSFRResult = SFRTest(m_ofDestDataOut,m_stROIInfo,m_iROICount);
				}
				if (!bSFRResult)
				{
					CString temp;
					temp.Format(_T("SFR calculate Fail At Code:%d"),m_iCode);
					m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,LOGHEIGHT);
				}
				else
				{
					CString temp;
					temp.Format(_T("Code=%d   CEN_H=%.4f  CEN_V=%.4f"),m_iCode,m_dSFRValue[0],m_dSFRValue[1]);
					m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
					DrawText(m_stROIInfo,m_iROICount,m_iBlockCount);
					//Save SFR with code
					SaveSFRPosInfo();
                       
					if(pDlg->m_bCheckThrough)
					{
						if(m_iPeroid == 2)
						{
							SFR_Fov_6();
							//保存小步搜索的SFR
							//
						}
						/*if(m_iPeroid == 2 && bRoiFail == true)
						{
							m_pInterface->Ctrl_Addlog(CamID,_T("小步走ROI失败！"),COLOR_RED,200);
							m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Auto ROI Fail"));
							m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
							SetResult(RESULT_FAIL);
							return 0;
						}*/
					}
				}
			}
			else
			{
				temp.Format(_T("Auto ROI Fail At Code:%d"),m_iCode);
				m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,LOGHEIGHT);
				m_iROIFailCount++;
				
				if (m_iROIFailCount >= 5)
				{
					bResult = false; 
					m_pInterface->Ctrl_Addlog(CamID,_T("Auto ROI Fail 5次"),COLOR_RED,200);
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Auto ROI Fail 5次"));
					m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
					SetResult(RESULT_FAIL);
					return 0;
				}

				if(pDlg->m_bCheckThrough && m_iPeroid == 2)//判断是否是小步走
				{
					//bRoiFail = true;
					bResult = false; 
					m_pInterface->Ctrl_Addlog(CamID,_T("小步走ROI失败！"),COLOR_RED,200);
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Auto ROI Fail"));
					m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
					SetResult(RESULT_FAIL);
					return 0;
				}
			}
		}
		else if (m_Options.m_iAFMethod == 1)
		{
			//Contrast AF
			m_stContrastData[m_iPos].DAC = m_iCode;
			m_stContrastData[m_iPos].iContrastValue = ContrastMeasure();
			CString info;
			info.Format(_T("Code: %d     Contrast: %d"),m_iCode,m_stContrastData[m_iPos].iContrastValue);
			m_pInterface->Ctrl_Addlog(CamID,info,COLOR_BLUE,200);
			//Show Contrast Info
			m_iPos++;
		}
		else
		{
			//MTF
			m_stMTFData[m_iPos].DAC = m_iCode;
			m_stMTFData[m_iPos].dMTF = MTFTest();
			CString info;
			info.Format(_T("Code: %d     MTF: %.2f"),m_iCode,m_stMTFData[m_iPos].dMTF);
			m_pInterface->Ctrl_Addlog(CamID,info,COLOR_BLUE,200);
			//Show Contrast Info
			m_iPos++;
		}

	
		DesideNextCode();

		BOOL bStop = FALSE;
		int temp_m_iCode = 0;
		if(pDlg->m_bCheckThrough && m_iPeroid == 2)//ThroughFocus分小步搜索
		{
			temp_m_iCode = m_iCode+m_iStep;
		}
		else 
		{
			temp_m_iCode = m_iCode;
		}
		if (temp_m_iCode > m_iEndCode)
		//if (m_iCode > m_iEndCode)
		{
			bStop = TRUE;
		}
		if (bStop)
		{
			//if(!FindPeak(m_iPeakDAC,m_iLastPos,m_iStep))
			//{
			//	m_pInterface->Ctrl_Addlog(CamID,_T("找不到符合范围的peak点"),COLOR_RED,200);
			//	m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("找不到符合范围的peak点"));
			//	m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
			//	SetResult(RESULT_FAIL);
			//	return 0;
			//}

			AF_STEP_DATA afStepData;
			afStepData.bigStepCnt = 0;
			for (int i=0; i<256; i++)
			{
				afStepData.bigStepData[i].code = 0;
				afStepData.bigStepData[i].av = 0.0f;
			}
			afStepData.miniStepCnt = 0;
			for (int i=0; i<256; i++)
			{
				afStepData.miniStepData[i].code = 0;
				afStepData.miniStepData[i].av = 0.0f;
			}

			if (m_Options.m_iAFMethod == 0) // SFR AF
			{
				if (m_iPeroid == 0)
				{
					afStepData.bigStepCnt = m_iPos;
					for (int i=0; i<afStepData.bigStepCnt; i++)
					{
						afStepData.bigStepData[i].code = m_stAFData[i].DAC;
						afStepData.bigStepData[i].av = (float) (m_stAFData[i].SFR[0] + m_stAFData[i].SFR[1]);
					}
				}
				else
				{
					afStepData.miniStepCnt = m_iPos - m_iLastPos;
					for (int i=0; i<afStepData.miniStepCnt; i++)
					{
						afStepData.miniStepData[i].code = m_stAFData[m_iLastPos + i].DAC;
						afStepData.miniStepData[i].av = (float) (m_stAFData[m_iLastPos + i].SFR[0] + m_stAFData[m_iLastPos + i].SFR[1]);
					}
				}
			}
			else if (m_Options.m_iAFMethod == 1) 
			{
				if (m_iPeroid == 0)
				{
					afStepData.bigStepCnt = m_iPos;
					for (int i=0; i<afStepData.bigStepCnt; i++)
					{
						afStepData.bigStepData[i].code = m_stContrastData[i].DAC;
						afStepData.bigStepData[i].av = (float)(m_stContrastData[i].iContrastValue);
					}
				}
				else
				{
					afStepData.miniStepCnt = m_iPos - m_iLastPos;
					for (int i=0; i<afStepData.miniStepCnt; i++)
					{
						afStepData.miniStepData[i].code = m_stContrastData[m_iLastPos + i].DAC;
						afStepData.miniStepData[i].av = (float)(m_stContrastData[m_iLastPos + i].iContrastValue);
					}
				}
			}
			else
			{
				if (m_iPeroid == 0)
				{
					afStepData.bigStepCnt = m_iPos;
					for (int i=0; i<afStepData.bigStepCnt; i++)
					{
						afStepData.bigStepData[i].code = m_stMTFData[i].DAC;
						afStepData.bigStepData[i].av = (float)(m_stMTFData[i].dMTF);
					}
				}
				else
				{
					afStepData.miniStepCnt = m_iPos - m_iLastPos;
					for (int i=0; i<afStepData.miniStepCnt; i++)
					{
						afStepData.miniStepData[i].code = m_stMTFData[m_iLastPos + i].DAC;
						afStepData.miniStepData[i].av = (float)(m_stMTFData[m_iLastPos + i].dMTF);
					}
				}
			}
			
			float afBestAv = 0.0f;
			if (m_iPeroid == 0)
				m_iPeakDAC = GetBestCode(_T("BIG_STEP_MODE"), afStepData, m_iStep, &m_iCode, &m_iEndCode, &afBestAv);
			else
				m_iPeakDAC = GetBestCode(_T("MINI_STEP_MODE"), afStepData, m_iStep, &m_iCode, &m_iEndCode, &afBestAv);

			temp.Format(_T("m_iPeakDAC : %d, %f"), m_iPeakDAC, afBestAv);
			m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, LOGHEIGHT);

          if(pDlg->m_bCheckThrough)
		  {
		  	//分大小步
			if(m_iPeroid == 0)
			{
				int codeguding = pDlg->m_nCodeCount;//SMALLRANGE;//(m_iEndCode - m_iCode);
				m_iStep =  codeguding/pDlg->m_nCnt;//14
				int tempminstep_code = _wtoi(pDlg->m_smallRange);
				int umlimt = m_iPeakDAC-tempminstep_code;//85
				int Tumlimit = m_iPeakDAC+tempminstep_code;
				if(umlimt < 0)
				{
					temp.Format(_T("小于0，m_iPeakDAC : %d, 范围：%d"), m_iPeakDAC, tempminstep_code);
					m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, LOGHEIGHT);
					umlimt = 0;//m_iCode
					Tumlimit = pDlg->m_nCodeCount;//SMALLRANGE;
				}
				//add by hjin 2017-09-12
				/*
				else if(umlimt > 1023)
				{
					temp.Format(_T("大于1023，m_iPeakDAC : %d, 范围：%d"), m_iPeakDAC, tempminstep_code);
					m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, LOGHEIGHT);
					umlimt = 1023;
					Tumlimit = pDlg->m_nCodeCount;
				}
				*/
				m_iCode= umlimt;		//;
				m_iEndCode=	Tumlimit;	//;
				m_bFocusing = FALSE;
			}
		  }
		  else 
		  {
		  	m_iStep = (m_iEndCode - m_iCode) / m_Options.m_iStep2;//6;
		  }
			
			if(pDlg->m_AFTestMode == NORMAL_MODE)
			{
				if (m_iPeroid == 0)
				{
					m_pInterface->VcmDr_WriteAF_Code(m_iCode,CamID);	
					SaveAllData();
					//m_iStep = m_Options.m_iStep2;
					m_iLastPos = m_iPos;
					m_iPeroid = 2;
					m_Options.m_iAFMethod = 0;
					temp.Format(_T("Step 2: Code Range[%d - %d] Step=%d"),m_iCode, m_iEndCode, m_iStep);
					m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_ORANGE,LOGHEIGHT);
				}else if (m_iPeroid == 1)
				{
					m_pInterface->VcmDr_WriteAF_Code(m_iCode,CamID);	
					SaveAllData();
					m_iStep = m_Options.m_iStep3;
					m_iLastPos = m_iPos;
					m_iPeroid = 2;
					temp.Format(_T("Step 3: Code Range[%d - %d] Step=%d"),m_iCode,m_iEndCode,m_iStep);
					m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_ORANGE,LOGHEIGHT);
				}else
				{
					SaveAllData();
					m_nJumpPos=1; 
					m_bFocusing = FALSE;	//-->转入Check
					m_iSkipFrame = 0;
					return 1;
				}
			}
	        else
			{
				SaveAllData();
				m_nJumpPos=1; 
				m_bFocusing = FALSE;	//-->转入Check
				m_iSkipFrame = 0;
				return 1;
			}
			return 1;
		}	
		else
		{
	        m_pInterface->VcmDr_WriteAF_Code(m_iCode,CamID);	
		}
		return 1;
	}

    else if (m_nJumpPos == 1)		//VCM退到Peak
	{
		if (m_iSkipFrame == 0)
		{
			CString temp;
#if 1
		   int  nCode = 0;
		   for (int i = 0; i< 5; i++)
		   {
			   nCode = (m_iPeakDAC-(4-i)*m_iStep);
			   temp.Format(_T("Move to Peak:%d"),nCode);
			   m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_ORANGE,LOGHEIGHT);
			   m_pInterface->VcmDr_WriteAF_Code(nCode,CamID);
			   Sleep(20);
		   }
#else
		   temp.Format(_T("Move to Peak:%d"),m_iPeakDAC);
		   m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_ORANGE,LOGHEIGHT);
		   m_pInterface->VcmDr_WriteAF_Code(m_iPeakDAC,CamID);
		   Sleep(300);
#endif
			m_iSkipFrame++;
		}
		else if (m_iSkipFrame < (m_Options.m_iSkipFrame+1))
		{
			m_iSkipFrame++;
			return 1;
		}
		else
		{
			m_nJumpPos = 2;
			m_iSkipFrame = 0;
			return 1;
		}
		return 1;
	}
	else if (m_nJumpPos == 2)	    //Peak位置SFR计算
	{
		
		if (!m_bFinalCheck)	//Check时，仅做一次Auto ROI
		{	
			Sleep(500);
			if ((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime))
			{		
				img  = *(m_pInterface->Ctrl_GetImgInfo(CamID));
				m_bFocusing = FALSE;

				if (m_iSkipFrame < (m_Options.m_iSkipFrame+1))
				{	
					m_iSkipFrame++;
					return 1;
				}
				m_iSkipFrame = 0;
			}

			memset(m_stROIInfo,0,sizeof(OfMarker)*120);
			memset(&m_ofDestDataOut,0,sizeof(OFAutoROIOut));
			img  = *(m_pInterface->Ctrl_GetImgInfo(CamID));
			BOOL ret = AutoROI(m_ofDestDataOut);
	
			if (ret)
			{
				m_iBlockCount = m_ofDestDataOut.ofROInumber;
				m_bFinalCheck = TRUE;
			}
			else
			{
				m_iFinalAutoROITime++;

				if (m_iFinalAutoROITime == 5)
				{
					if (pDlg->m_AFTestMode == FQC_MODE)
					{
						m_nJumpPos = 0;
						m_pInterface->Ctrl_Addlog(CamID,_T("AutoROI 5th Fail...Start AutoFocus..."),COLOR_RED,LOGHEIGHT);
						return 1;
					}
					else
					{
						bResult = false; 
						m_pInterface->Ctrl_Addlog(CamID,_T("Auto ROI 5th Fail..."),COLOR_RED,LOGHEIGHT);
						m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("连续Auto ROI 5次失败..."));
						m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
						SetResult(RESULT_FAIL);
						return 0;
					}
				}else
					return 1;
			}
		}
		
		img  = *(m_pInterface->Ctrl_GetImgInfo(CamID));
		BOOL bSFRResult = -1;
		//if(bFlagCenterROI == FALSE)
		{
		   bSFRResult = SFRTest(m_ofDestDataOut,stROIInfo,m_iROICount);
		}
		
		if (!bSFRResult)
		{
			m_iFinalSFRTest++;
			CString temp;
			temp.Format(_T("SFR calculate Fail At Code:%d"),m_iPeakDAC);
	
			if (m_iFinalSFRTest > 10)
			{
				bResult = false; 
					m_pInterface->Ctrl_Addlog(CamID,_T("连续10次 计算peak点SFR失败..."),COLOR_RED,LOGHEIGHT);
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("连续10次 计算peak点SFR失败..."));
					m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
					SetResult(RESULT_FAIL);
					return 0;
			}
			else
			{
				    m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_YELLOW,LOGHEIGHT);
				    return 1;
			}
		}


		DrawText(stROIInfo,m_iROICount,m_iBlockCount);
		if (!m_Options.m_bMidValue)
		{
			m_nJumpPos = 3;
			return 1;
		}
		//取中值
		for (int i = 0; i < m_iSFRROICount; i++)
		{
			SFRAll[iTestTimes][i] = m_dSFRValue[i];
		}		
		iTestTimes++;
		if (iTestTimes == m_Options.m_Mid_TestTime)
		{
			SaveCheckData();
			//************************中值
			//从大到小排序
			for(int ROIIndex=0;ROIIndex<m_iSFRROICount;ROIIndex++)
			{
				for(int TestTimeIndex=0;TestTimeIndex< iTestTimes-1; TestTimeIndex++)
				{
					for(int index=TestTimeIndex+1;index<iTestTimes;index++)
					{
						if(SFRAll[TestTimeIndex][ROIIndex]>SFRAll[index][ROIIndex])
						{
							float SFRTemp=SFRAll[TestTimeIndex][ROIIndex];
							SFRAll[TestTimeIndex][ROIIndex]=SFRAll[index][ROIIndex];
							SFRAll[index][ROIIndex]=SFRTemp;
						}
					}
				}
			}
			//排序完成
			if (pDlg->m_AFTestMode == FQC_MODE)
			{
				for(int i=0;i<m_iSFRROICount;i++) 
				{
					m_dSFRValue[i]=SFRAll[0][i];
					m_dSFRValue_Fact[i] = SFRAll[0][i];
					m_dSFRValue_Offset[i] = SFRAll[0][i];

				}
			}
			else
			{
				if ((m_Options.m_Mid_TestTime%2)==1)
				{
					for(int i=0;i<m_iSFRROICount;i++) 
					{
						m_dSFRValue[i]=SFRAll[(m_Options.m_Mid_TestTime-1)/2][i];
						m_dSFRValue_Fact[i] = SFRAll[(m_Options.m_Mid_TestTime-1)/2][i];
						m_dSFRValue_Offset[i] = SFRAll[(m_Options.m_Mid_TestTime-1)/2][i];
					}
				}
				else
				{
					for(int i=0;i<m_iSFRROICount;i++) 
					{
						m_dSFRValue[i]=(float)(0.5*(SFRAll[(m_Options.m_Mid_TestTime/2)-1][i]+SFRAll[m_Options.m_Mid_TestTime/2][i]));
						m_dSFRValue_Fact[i] = SFRAll[(m_Options.m_Mid_TestTime-1)/2][i];
						m_dSFRValue_Offset[i] = SFRAll[(m_Options.m_Mid_TestTime-1)/2][i];
					}
				}
			}
			
			m_nJumpPos = 3;
			CString temp;
			temp.Format(_T("CEN_H=%.4f  CEN_V=%.4f"),m_dSFRValue[0],m_dSFRValue[1]);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
		}
		return 1;
	}
	else if (m_nJumpPos == 3)	//卡规格
	{
		m_bResult = CheckFinalSpec();
		if(!m_bResult)
		{
			bResult = false;
		}
		
		//m_bResult=true;//todo

		DrawText(stROIInfo,m_iROICount,m_iBlockCount);
		//保存补偿后的SFR值
		if(pDlg->m_BSFRSaveDataOffset)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("pDlg->m_BSaveSFR_Acut,Save Offset SFR Data"),COLOR_BLUE,220);
			if(bResult)
			{
				SaveSFRData_Offset(true);//保存补偿后数据
			}
			else
			{
				SaveSFRData_Offset(false);//保存补偿后数据
			}

		}
		//保存实际的SFR值
		if(pDlg->m_BSaveSFR_Acut)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("pDlg->m_BSaveSFR_Acut,Save Actually SFR Data"),COLOR_BLUE,220);
			if(bResult)
			{
				SaveData_InFact(true);//保存实际数据
			}
			else
			{
				SaveData_InFact(false);//保存实际数据
			}
		}

		SaveFinalData();
		BOOL ret = SaveSFR();
		if (FALSE == ret)
		{
			bResult = false; 
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Sensor温度异常"));
			m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
			SetResult(RESULT_FAIL);
			return 0;
		}
		BOOL bResult_PeakCode_Diff = Peak_Code_Check();
		if(!bResult_PeakCode_Diff)
		{
			bResult = false; 
			m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
			SetResult(RESULT_FAIL);
			return 0;
		}
		if (m_bResult)
		{
			if (pDlg->m_CodeDiffCheckEn)
			{
				m_bResult = FALSE;
				m_bResult = CodeDiffCheck();
				if (m_bResult)
				{
					SetResult(RESULT_PASS);
					return 0;
				}
				else
				{
					if ((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime))
					{
						m_nJumpPos = 0;
						iTestTimes = 0;
						m_bFocusing = TRUE;
						m_bFinalCheck = FALSE;
						m_iCode = m_Options.m_iStartCode ;
						m_ofDestDataOut.ofROInumber = 0;
						//m_pInterface->Ctrl_Addlog(CamID,_T("fqc 1th Fail...Start AutoFocus..."),COLOR_RED,LOGHEIGHT);
						m_pInterface->VcmDr_WriteAF_Code(m_iCode,CamID);
						fqcFirstTime = FALSE;
						return 1;
					}
					else
					{
						//m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Failed."));
						m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
						SetResult(RESULT_FAIL);
						bResult = false; 
						return 0;
					}
				}
			}
			else 	
			{
					SetResult(RESULT_PASS);
					return 0;
			}
		}
		else
		{
			if ((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime))
			{
				m_nJumpPos = 0;
                iTestTimes = 0;
				m_bFocusing = TRUE;
				m_bFinalCheck = FALSE;
				m_iCode = m_Options.m_iStartCode ;
				m_ofDestDataOut.ofROInumber = 0;
				m_pInterface->Ctrl_Addlog(CamID,_T("fqc 1th Fail...Start AutoFocus..."),COLOR_RED,LOGHEIGHT);
				m_pInterface->VcmDr_WriteAF_Code(m_iCode,CamID);
				fqcFirstTime = FALSE;
				return 1;
			}
			else
			{
				//m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Failed."));
				m_pInterface->Ctrl_SetCamErrorCode(CamID, m_Options.ErrorCode);
				SetResult(RESULT_FAIL);
				bResult = false; 
				return 0;
			}
		}
		
	}

   //SetResult(RESULT_PASS);
   //m_pInterface->Ctrl_Addlog(CamID, _T("AF SFR Test PASS ."), COLOR_GREEN, 200);
   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AF_SFR_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	BOOL Bres = EmptyWorkingSet(GetCurrentProcess()); 
	if(!Bres)
	{
		AfxMessageBox(_T("清理内存失败！"));
	}

	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AF_SFR_TestItem::LoadOption()
{
	pDlg->m_sfrFreq = ReadConfigInt(_T("SFR_TEST_FREQ"), 4);
	pDlg->m_sfrOther_Freq = ReadConfigInt(_T("SFR_TEST_OTHER_FREQ"), 8);

	pDlg->m_cornerDiffRelative = ReadConfigInt(_T("SFR_CORNER_DIFF_RELATIVE"), 0);
	pDlg->m_cornerDiffAbsolute = ReadConfigInt(_T("SFR_CORNER_DIFF_ABSOLUTE"), 1);
	pDlg->m_smallRange = ReadConfigString(_T("SFR_SMALL_RANGE"), _T("85"));
	//leger add
	pDlg->m_SpcShift_En_0F = ReadConfigInt(_T("SpcShift_En_0F"),0);
	pDlg->m_SpcShift_En_3F = ReadConfigInt(_T("SpcShift_En_3F"),0);
	pDlg->m_SpcShift_En_4F = ReadConfigInt(_T("SpcShift_En_4F"),0);
	pDlg->m_SpcShift_En_5F = ReadConfigInt(_T("SpcShift_En_5F"),0);
	pDlg->m_SpcShift_En_6F = ReadConfigInt(_T("SpcShift_En_6F"),0);
	pDlg->m_SpcShift_En_8F = ReadConfigInt(_T("SpcShift_En_8F"),0);
	pDlg->m_SpcShift_CTC_En_5F = ReadConfigInt(_T("SpcShift_CTC_En_5F"),0);
	pDlg->m_SpcShift_CTC_En_6F = ReadConfigInt(_T("SpcShift_CTC_En_6F"),0);
	pDlg->m_SpcShift_CTC_En_8F = ReadConfigInt(_T("SpcShift_CTC_En_8F"),0);
	
	pDlg->m_bCheckThrough = ReadConfigInt(_T("CheckThrough"),0);

	pDlg->m_StartOffset =  ReadConfigInt(_T("StartOffset"),0);
	pDlg->m_EndOffset =  ReadConfigInt(_T("EndOffset"),0);
	pDlg->m_bTestCenter = ReadConfigInt(_T("bTestCenter"),0);
	pDlg->m_bTestF3 = ReadConfigInt(_T("bTestF3"),0);
	pDlg->m_bTestF4 = ReadConfigInt(_T("bTestF4"),0);
	pDlg->m_bTestF5 = ReadConfigInt(_T("bTestF5"),0);
	pDlg->m_bTestF6 = ReadConfigInt(_T("bTestF6"),0);
	pDlg->m_bTestF8 = ReadConfigInt(_T("bTestF8"),0);
	pDlg->m_AFTestMode = ReadConfigInt(_T("AFTestMode"),0);
	pDlg->m_FastMode_Step = ReadConfigInt(_T("FastMode_Step"),0);
	pDlg->m_UseCode_Name = 	ReadConfigString(_T("UseCode_Name"),_T("0"));	
	pDlg->m_bSaveImage =  ReadConfigInt(_T("SaveImage"),0);
	pDlg->m_ROI_Size =  ReadConfigInt(_T("ROISize"),96); 

	pDlg->m_CodeDiffCheckEn = ReadConfigInt(_T("CodeDiffCheckEn"),0);
	pDlg->m_CodeDiffMax = ReadConfigInt(_T("CodeDiffMax"),0);
	pDlg->m_CodeDiffMin = ReadConfigInt(_T("CodeDiffMin"),0);
	pDlg->m_MacroStationName = ReadConfigString(_T("MacroStationName"),_T("station1"));
	pDlg->m_InfinityStationName = ReadConfigString(_T("InfinityStationName"),_T("station2"));

	CString temp;

	m_Options.ErrorCode = ReadConfigInt(_T("ErrorCode"),16);
	temp = 	ReadConfigString(_T("CenH_Spec"),_T("0"));	
	m_Options.m_CenH_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("CenV_Spec"),_T("0"));	
	m_Options.m_CenV_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F3H_Spec"),_T("0"));	
	m_Options.m_F3H_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F3V_Spec"),_T("0"));	
	m_Options.m_F3V_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F4H_Spec"),_T("0"));	
	m_Options.m_F4H_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F4V_Spec"),_T("0"));	
	m_Options.m_F4V_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F5H_Spec"),_T("0"));	
	m_Options.m_F5H_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F5V_Spec"),_T("0"));	
	m_Options.m_F5V_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F6H_Spec"),_T("0"));	
	m_Options.m_F6H_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F6V_Spec"),_T("0"));	
	m_Options.m_F6V_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F8H_Spec"),_T("0"));	
	m_Options.m_F8H_Spec = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("F8V_Spec"),_T("0"));	
	m_Options.m_F8V_Spec = (float)_tcstod(temp,0);

	temp = 	ReadConfigString(_T("SpecShift_0F"),_T("0"));	
	pDlg->m_SpcShift_0F = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_3F"),_T("0"));	
	pDlg->m_SpcShift_3F = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_4F"),_T("0"));	
	pDlg->m_SpcShift_4F = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_5F"),_T("0"));	
	pDlg->m_SpcShift_5F = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_6F"),_T("0"));	
	pDlg->m_SpcShift_6F = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_8F"),_T("0"));	
	pDlg->m_SpcShift_8F = (float)_tcstod(temp,0);

	temp = 	ReadConfigString(_T("SpecShift_5F_CTC"),_T("0"));	
	pDlg->m_SpcShift_5F_CTC = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_6F_CTC"),_T("0"));	
	pDlg->m_SpcShift_6F_CTC = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_8F_CTC"),_T("0"));	
	pDlg->m_SpcShift_8F_CTC = (float)_tcstod(temp,0);

	m_Options.m_Mid_TestTime =  ReadConfigInt(_T("MidTestTime"),0);
	if ((m_Options.m_Mid_TestTime<1)||(m_Options.m_Mid_TestTime>8))
	{
		m_Options.m_Mid_TestTime = 1;
	}
	//TODO: 在此添加读取代码 
	m_Options.bEngineerMode = ReadConfigInt(ENGINEMODE,0);
	m_Options.m_bMidValue = ReadConfigInt(MIDLLEVALUE_EN,1);
	m_Options.m_bShakeEn = ReadConfigInt(SHAKE_EN,1);
	m_Options.m_iStartCode = ReadConfigInt(STARTCODE,100);
	m_Options.m_iEndCode = ReadConfigInt(ENDCODE,500);
	m_Options.m_iStep1 = ReadConfigInt(STEP1,6);
	m_Options.m_iStep2 = ReadConfigInt(STEP2,6);
	m_Options.m_iStep3 = ReadConfigInt(STEP3,5);
	m_Options.m_iSkipFrame = ReadConfigInt(SKIPFRAME,2);
	temp = 	ReadConfigString(CTC8,_T("0"));	
	m_Options.m_fCTC8 = (float)_tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC8);
	temp = 	ReadConfigString(CTC6,_T("0"));
	m_Options.m_fCTC6 = (float)_tcstod(temp,0);
	//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC6);
	temp = 	ReadConfigString(CTC5,_T("0"));
	m_Options.m_fCTC5 = (float)_tcstod(temp,0);
	//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC5);
	temp = 	ReadConfigString(CTC8H,_T("0"));
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC8_H);
	m_Options.m_fCTC8_H = (float)_tcstod(temp,0);
	temp = 	ReadConfigString(CTC8V,_T("0"));
	m_Options.m_fCTC8_V = (float)_tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC8_V);
	temp = 	ReadConfigString(CTC6H,_T("0"));
	m_Options.m_fCTC6_H = (float)_tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC6_H);
	temp = 	ReadConfigString(CTC6V,_T("0"));
	m_Options.m_fCTC6_V = (float)_tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC6_V);
	temp = 	ReadConfigString(CTC5H,_T("0"));
	m_Options.m_fCTC5_H = (float)_tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC5_H);
	temp = 	ReadConfigString(CTC5V,_T("0"));
	m_Options.m_fCTC5_V = (float)_tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC5_V);
	m_Options.m_iShakeCount = ReadConfigInt(SHAKECOUNT,0);
	m_Options.m_iShakeDAC = ReadConfigInt(SHAKECODE,1000);
	m_Options.m_iShakeSkipFrame = ReadConfigInt(SHAKESKIPFRAME,3);
	m_Options.m_iPeakMax = ReadConfigInt(PEAKMAX,1023);
	m_Options.m_iPeakMin = ReadConfigInt(PEAKMIN,0);
//	m_Options.strSpecROIs = ReadConfigString(SFRSPEC,_T(""));
	m_Options.m_StationName = ReadConfigString(STATIONNAME,_T("station1"));
	m_Options.m_iSFRBurnRate = ReadConfigInt(SFRBURNRATE,100);


	m_Options.m_iAFMethod = ReadConfigInt(_T("AFMethod"),0);
	m_Options.m_iContrastROIStartX = ReadConfigInt(ROISTARTX,0);
	m_Options.m_iContrastROIStartY = ReadConfigInt(ROISTARTY,0);
	m_Options.m_iContrastROIWidth = ReadConfigInt(ROIWIDTH,400);
	m_Options.m_iContrastROIHeight = ReadConfigInt(ROIHeight,400);

	pDlg->m_nCnt = ReadConfigInt(_T("m_nCnt"),14);

	pDlg->m_nCodeCount = ReadConfigInt(_T("m_nCodeCount"),165);

	pDlg->m_nPeakCodeDiff = ReadConfigInt(_T("m_nPeakCodeDiff"),10);

	pDlg->m_BPeakCodeDiffEN = ReadConfigInt(_T("m_BPeakCodeDiffEN"),0);
	pDlg->m_BMacroDiffEn = ReadConfigInt(_T("m_BMacroDiffEn"),0);
	
	pDlg->m_nMacroCodeDiff = ReadConfigInt(_T("m_nMacroCodeDiff"),10);
	pDlg->m_BLC = ReadConfigInt(_T("m_BLC"), 0);
	pDlg->m_BSaveSFR_Acut = ReadConfigInt(_T("m_BSaveSFR_Acut"), 1);
	pDlg->m_BSFRSaveDataOffset = ReadConfigInt(_T("m_BSFRSaveDataOffset"), 1);	
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AF_SFR_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("SFR_TEST_FREQ"), pDlg->m_sfrFreq);
	WriteConfigInt(_T("SFR_TEST_OTHER_FREQ"), pDlg->m_sfrOther_Freq);

	WriteConfigInt(_T("SFR_CORNER_DIFF_RELATIVE"), pDlg->m_cornerDiffRelative);
	WriteConfigInt(_T("SFR_CORNER_DIFF_ABSOLUTE"), pDlg->m_cornerDiffAbsolute);
	WriteConfigString(_T("SFR_SMALL_RANGE"), pDlg->m_smallRange);
	//leger add
	WriteConfigInt(_T("ROISize"),pDlg->m_ROI_Size);
	WriteConfigInt(_T("StartOffset"),pDlg->m_StartOffset);
	WriteConfigInt(_T("EndOffset"),pDlg->m_EndOffset);
	WriteConfigInt(_T("bTestCenter"),pDlg->m_bTestCenter);
	WriteConfigInt(_T("bTestF3"),pDlg->m_bTestF3);
	WriteConfigInt(_T("bTestF4"),pDlg->m_bTestF4);
	WriteConfigInt(_T("bTestF5"),pDlg->m_bTestF5);
	WriteConfigInt(_T("bTestF6"),pDlg->m_bTestF6);
	WriteConfigInt(_T("bTestF8"),pDlg->m_bTestF8);
	
	WriteConfigInt(_T("CheckThrough"),pDlg->m_bCheckThrough);

	WriteConfigInt(_T("ErrorCode"),m_Options.ErrorCode);
	WriteConfigInt(_T("AFTestMode"),pDlg->m_AFTestMode);
	WriteConfigInt(_T("FastMode_Step"),pDlg->m_FastMode_Step);
	WriteConfigString(_T("UseCode_Name"),pDlg->m_UseCode_Name);
	WriteConfigInt(_T("SaveImage"),pDlg->m_bSaveImage);	

	WriteConfigInt(_T("CodeDiffCheckEn"),pDlg->m_CodeDiffCheckEn);
	WriteConfigInt(_T("CodeDiffMax"),pDlg->m_CodeDiffMax);
	WriteConfigInt(_T("CodeDiffMin"),pDlg->m_CodeDiffMin);
	WriteConfigString(_T("MacroStationName"),pDlg->m_MacroStationName);
	WriteConfigString(_T("InfinityStationName"),pDlg->m_InfinityStationName);

	CString temp;

	temp.Format(_T("%.6f"),m_Options.m_CenH_Spec);
	WriteConfigString(_T("CenH_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_CenV_Spec);
	WriteConfigString(_T("CenV_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F3H_Spec);
	WriteConfigString(_T("F3H_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F3V_Spec);
	WriteConfigString(_T("F3V_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F4H_Spec);
	WriteConfigString(_T("F4H_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F4V_Spec);
	WriteConfigString(_T("F4V_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F5H_Spec);
	WriteConfigString(_T("F5H_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F5V_Spec);
	WriteConfigString(_T("F5V_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F6H_Spec);
	WriteConfigString(_T("F6H_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F6V_Spec);
	WriteConfigString(_T("F6V_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F8H_Spec);
	WriteConfigString(_T("F8H_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F8V_Spec);
	WriteConfigString(_T("F8V_Spec"),temp);
	WriteConfigInt(_T("MidTestTime"),m_Options.m_Mid_TestTime);

	temp.Format(_T("%.6f"),pDlg->m_SpcShift_0F);
	WriteConfigString(_T("SpecShift_0F"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_3F);
	WriteConfigString(_T("SpecShift_3F"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_4F);
	WriteConfigString(_T("SpecShift_4F"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_5F);
	WriteConfigString(_T("SpecShift_5F"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_6F);
	WriteConfigString(_T("SpecShift_6F"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_8F);
	WriteConfigString(_T("SpecShift_8F"),temp);

	WriteConfigInt(_T("SpcShift_En_0F"),pDlg->m_SpcShift_En_0F);
	WriteConfigInt(_T("SpcShift_En_3F"),pDlg->m_SpcShift_En_3F);
	WriteConfigInt(_T("SpcShift_En_4F"),pDlg->m_SpcShift_En_4F);
	WriteConfigInt(_T("SpcShift_En_5F"),pDlg->m_SpcShift_En_5F);
	WriteConfigInt(_T("SpcShift_En_6F"),pDlg->m_SpcShift_En_6F);
	WriteConfigInt(_T("SpcShift_En_8F"),pDlg->m_SpcShift_En_8F);
	WriteConfigInt(_T("SpcShift_CTC_En_5F"),pDlg->m_SpcShift_CTC_En_5F);
	WriteConfigInt(_T("SpcShift_CTC_En_6F"),pDlg->m_SpcShift_CTC_En_6F);
	WriteConfigInt(_T("SpcShift_CTC_En_8F"),pDlg->m_SpcShift_CTC_En_8F);

	//old 
	WriteConfigInt(ENGINEMODE,m_Options.bEngineerMode);
	WriteConfigInt(MIDLLEVALUE_EN,m_Options.m_bMidValue);
	WriteConfigInt(SHAKE_EN,m_Options.m_bShakeEn);
	WriteConfigInt(STARTCODE,m_Options.m_iStartCode);
	WriteConfigInt(ENDCODE,m_Options.m_iEndCode);
	WriteConfigInt(STEP1,m_Options.m_iStep1);
	WriteConfigInt(STEP2,m_Options.m_iStep2);
	WriteConfigInt(STEP3,m_Options.m_iStep3);
	WriteConfigInt(SKIPFRAME,m_Options.m_iSkipFrame);

	temp.Format(_T("%.6f"),m_Options.m_fCTC8);
	WriteConfigString(CTC8,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC6);
	WriteConfigString(CTC6,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC5);
	WriteConfigString(CTC5,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC8_H);
	WriteConfigString(CTC8H,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC8_V);
	WriteConfigString(CTC8V,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC6_H);
	WriteConfigString(CTC6H,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC6_V);
	WriteConfigString(CTC6V,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC5_H);
	WriteConfigString(CTC5H,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC5_V);
	WriteConfigString(CTC5V,temp);

	temp.Format(_T("%.6f"),pDlg->m_SpcShift_5F_CTC);
	WriteConfigString(_T("SpecShift_5F_CTC"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_6F_CTC);
	WriteConfigString(_T("SpecShift_6F_CTC"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_8F_CTC);
	WriteConfigString(_T("SpecShift_8F_CTC"),temp);

	WriteConfigInt(SHAKECOUNT,m_Options.m_iShakeCount);
	WriteConfigInt(SHAKECODE,m_Options.m_iShakeDAC);
	WriteConfigInt(SHAKESKIPFRAME,m_Options.m_iShakeSkipFrame);
	WriteConfigInt(PEAKMAX,m_Options.m_iPeakMax);
	WriteConfigInt(PEAKMIN,m_Options.m_iPeakMin);
	//WriteConfigString(SFRSPEC,m_Options.strSpecROIs);
	WriteConfigString(STATIONNAME,m_Options.m_StationName);
	WriteConfigInt(SFRBURNRATE,m_Options.m_iSFRBurnRate);

	WriteConfigInt(_T("AFMethod"),m_Options.m_iAFMethod);
	WriteConfigInt(ROISTARTX,m_Options.m_iContrastROIStartX);
	WriteConfigInt(ROISTARTY,m_Options.m_iContrastROIStartY);
	WriteConfigInt(ROIWIDTH,m_Options.m_iContrastROIWidth);
	WriteConfigInt(ROIHeight,m_Options.m_iContrastROIHeight);

	WriteConfigInt(_T("m_nCnt"),pDlg->m_nCnt);
	
	WriteConfigInt(_T("m_nCodeCount"),pDlg->m_nCodeCount);
	
	WriteConfigInt(_T("m_nPeakCodeDiff"),pDlg->m_nPeakCodeDiff);
	WriteConfigInt(_T("m_BPeakCodeDiffEN"),pDlg->m_BPeakCodeDiffEN);
	
	WriteConfigInt(_T("m_BMacroDiffEn"),pDlg->m_BMacroDiffEn);
	WriteConfigInt(_T("m_nMacroCodeDiff"),pDlg->m_nMacroCodeDiff);
	WriteConfigInt(_T("m_BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("m_BSaveSFR_Acut"), pDlg->m_BSaveSFR_Acut);

	WriteConfigInt(_T("m_BSFRSaveDataOffset"), pDlg->m_BSFRSaveDataOffset);	
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_SFR_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header = _T("DAC,");
	for (int i = 0; i < m_iSFRROICount; i++)
	{
		header += m_stSpec[i].ROIName;
		header += _T(",");
	}
	header += _T("CTC8,CTC6,CTC5,CTC8_H,CTC8_V,CTC6_H,CTC6_V,CTC5_H,CTC5_V");
	return header;	
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_SFR_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_SFR_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_SFR_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	Content.Format(_T("%d,"),m_iPeakDAC);

	CString temp;
	for (int index = 0; index < m_iSFRROICount; index++)
	{
		temp.Format(_T("%f,"),m_dSFRValue[index]);
		Content += temp;
	}
	temp.Format(_T("%f,%f,%f,%f,%f,%f,%f,%f,%f"),m_fCTC8,m_fCTC6,m_fCTC5,m_fCTC8_H,m_fCTC8_V,m_fCTC6_H,m_fCTC6_V,m_fCTC5_H,m_fCTC5_V);
	Content += temp;
	return Content;
}

//m_BPeakCodeDiffEN
//检查规格差异
BOOL AF_SFR_TestItem::Peak_Code_Check()
{
	USHORT InfCode_EEPRom = 0;
	USHORT MacroCode_EEPRom = 0;
	CString str = _T("");
	if(pDlg->m_BPeakCodeDiffEN)//远焦
	{
		//读取之前已经写入的PeakCode值
		/*USHORT HighVal = 0;
		USHORT LowVal = 0;
		m_pInterface->Otp_OtpRead(0x0027, 0x0027, &HighVal, CamID);
		Sleep(10);
		m_pInterface->Otp_OtpRead(0x0028, 0x0028, &LowVal, CamID);
		Sleep(10);
		InfCode_EEPRom = ((HighVal & 0xFF) << 8 | (LowVal & 0xFF));
		InfCode_EEPRom = InfCode_EEPRom >> 6;*/
		CString SFRFile=m_pInterface->Ctrl_GetCurrentPath() +_T("//SFRBurnFile.ini");//SFR数据保存文件
		int tempPeakCode = GetPrivateProfileInt(m_Options.m_StationName,_T("Peak"),0,SFRFile);

		SFRFile = m_pInterface->Ctrl_GetCurrentPath() + _T("//SFRBurnFile_PeakCodeCheck.ini");
		InfCode_EEPRom = GetPrivateProfileInt(m_Options.m_StationName,_T("Peak"),0,SFRFile);
		int tempPeakCodeDiffer = abs(tempPeakCode - InfCode_EEPRom);
		if(tempPeakCodeDiffer > pDlg->m_nPeakCodeDiff)
		{
			str.Format(_T("从SFRBurnFile_PeakCodeCheck.ini远焦值：%d, 从SFRBurnFile.ini远焦值：%d，两者差值：%d，界面设定值：%d"), InfCode_EEPRom, tempPeakCode, tempPeakCodeDiffer,pDlg->m_nPeakCodeDiff);
			//SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);
			return FALSE;
		}
	}
	if(pDlg->m_BMacroDiffEn)//近焦
	{
		/*USHORT HighVal_Macro = 0;
		USHORT LowVal_Macro = 0;
		m_pInterface->Otp_OtpRead(0x0029, 0x0029, &HighVal_Macro, CamID);
		Sleep(10);
		m_pInterface->Otp_OtpRead(0x002A, 0x002A, &LowVal_Macro, CamID);
		Sleep(10);
		MacroCode_EEPRom = ((HighVal_Macro & 0xFF) << 8 | (LowVal_Macro & 0xFF));
		MacroCode_EEPRom = MacroCode_EEPRom >> 6;*/

		CString SFRFile=m_pInterface->Ctrl_GetCurrentPath() +_T("//SFRBurnFile.ini");//SFR数据保存文件
		int tempPeakCode = GetPrivateProfileInt(m_Options.m_StationName,_T("Peak"),0,SFRFile);
		SFRFile = m_pInterface->Ctrl_GetCurrentPath() + _T("//SFRBurnFile_PeakCodeCheck.ini");
		MacroCode_EEPRom = GetPrivateProfileInt(m_Options.m_StationName,_T("Peak"),0,SFRFile);

		int tempPeakCodeDiffer = abs(tempPeakCode - MacroCode_EEPRom);
		if(tempPeakCodeDiffer > pDlg->m_nMacroCodeDiff)//m_nMacroCodeDiff
		{
			str.Format(_T("从SFRBurnFile_PeakCodeCheck.ini取的近焦值：%d, 从SFRBurnFile.ini取的远焦值：%d，两者差值：%d，界面设定值：%d"), MacroCode_EEPRom, tempPeakCode, tempPeakCodeDiffer,pDlg->m_nMacroCodeDiff);
			//SetResult(RESULT_FAIL);
			m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);
			return FALSE;
		}

	}
	return TRUE;
	 //
}
BOOL AF_SFR_TestItem::CheckFinalSpec()
{
	//卡规格
	BOOL bPass = TRUE;

	if (m_iPeakDAC<m_Options.m_iPeakMin || m_iPeakDAC > m_Options.m_iPeakMax)
	{
		bPass = FALSE;
		m_errMsg = m_cfgName;
		m_errMsg += _T(" Peak点超范围");
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
		m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);		
	}

	double dMax8 = 0;
	double dMin8 = 1;
	double dMax6 = 0;
	double dMin6 = 1;
	double dMax5 = 0;
	double dMin5 = 1;

	double dMax8_H = 0;
	double dMin8_H = 1;
	double dMax8_V = 0;
	double dMin8_V = 1;

	double dMax6_H = 0;
	double dMin6_H = 1;				
	double dMax6_V = 0;
	double dMin6_V = 1;

	double dMax5_H = 0;
	double dMin5_H = 1;
	double dMax5_V = 0;
	double dMin5_V = 1;

	int startsfr0f = 0;
	int endsfr0f = 1;

	int startsfr3f = 0;
	int endsfr3f = 0;
	int startsfr4f = 0;
	int endsfr4f = 0;
	int startsfr5f = 0;
	int endsfr5f = 0;
	int startsfr6f = 0;
	int endsfr6f  = 0;
	int startsfr8f = 0;
	int endsfr8f  = 0;

	int roiCnt = 1;
	if (pDlg->m_bTestF3 == 1)
	{
		startsfr3f = roiCnt + 1;
		roiCnt = endsfr3f = roiCnt + 8;
	}
	else
	{
		startsfr3f = 0;
		endsfr3f = 0;
	}

	if (pDlg->m_bTestF4 == 1)
	{
		startsfr4f = roiCnt + 1;
		roiCnt = endsfr4f = roiCnt + 8;
	}
	else
	{
		startsfr4f = 0;
		endsfr4f = 0;
	}

	if (pDlg->m_bTestF5 == 1)
	{
		startsfr5f = roiCnt + 1;
		roiCnt = endsfr5f = roiCnt + 8;
	}
	else
	{
		startsfr5f = 0;
		endsfr5f = 0;
	}

	if (pDlg->m_bTestF6 == 1)
	{
		startsfr6f = roiCnt + 1;
		roiCnt = endsfr6f = roiCnt + 8;
	}
	else
	{
		startsfr6f = 0;
		endsfr6f = 0;
	}

	if (pDlg->m_bTestF8 == 1)
	{
		startsfr8f = roiCnt + 1;
		roiCnt = endsfr8f = roiCnt + 8;
	}
	else
	{
		startsfr8f = 0;
		endsfr8f = 0;
	}
#if 0
	if (((pDlg->m_bTestCenter+pDlg->m_bTestF6+pDlg->m_bTestF8)==3)&&(pDlg->m_bTestF4==0))
	{
		startsfr4f = 0;
		endsfr4f = 0;
		startsfr5f = 0;
		endsfr5f = 0;
		startsfr6f = 2;
		endsfr6f  = 9;
		startsfr8f = 10;
		endsfr8f  = 17;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF8)==3)&&(pDlg->m_bTestF6==0))
	{
		startsfr4f = 2;
		endsfr4f = 9;
		startsfr5f = 0;
		endsfr5f = 0;
		startsfr6f = 0;
		endsfr6f  = 0;
		startsfr8f = 10;
		endsfr8f  = 17;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF6)==3)&&(pDlg->m_bTestF8==0))
	{
		startsfr4f = 2;
		endsfr4f = 9;
		startsfr5f = 0;
		endsfr5f = 0;
		startsfr6f = 10;
		endsfr6f  = 17;
		startsfr8f = 0;
		endsfr8f  = 0;
	}
	else if ((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF6+pDlg->m_bTestF8)==4)
	{
		startsfr4f = 2;
		endsfr4f = 9;
		startsfr5f = 0;
		endsfr5f = 0;
		startsfr6f = 10;
		endsfr6f  = 17;
		startsfr8f = 18;
		endsfr8f  = 25;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF8)==2)&&((pDlg->m_bTestF4+pDlg->m_bTestF6)==0))
	{
		startsfr4f = 0;
		endsfr4f = 0;
		startsfr5f = 0;
		endsfr5f = 0;
		startsfr6f = 0;
		endsfr6f  = 0;
		startsfr8f = 2;
		endsfr8f  = 9;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF6)==2)&&((pDlg->m_bTestF4+pDlg->m_bTestF8)==0))
	{
		startsfr4f = 0;
		endsfr4f = 0;
		startsfr5f = 0;
		endsfr5f = 0;
		startsfr6f = 2;
		endsfr6f  = 9;
		startsfr8f = 0;
		endsfr8f  = 0;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF4)==2)&&((pDlg->m_bTestF6+pDlg->m_bTestF8)==0))
	{
		startsfr4f = 2;
		endsfr4f = 9;
		startsfr5f = 0;
		endsfr5f = 0;
		startsfr6f = 0;
		endsfr6f  = 0;
		startsfr8f = 0;
		endsfr8f  = 0;
	}
	else
	{
		startsfr4f = 0;
		endsfr4f = 0;
		startsfr5f = 0;
		endsfr5f = 0;
		startsfr6f = 0;
		endsfr6f  = 0;
		startsfr8f = 0;
		endsfr8f  = 0;
	}
#endif
	srand(time(NULL));
	for (int index = 0; index < m_iSFRROICount; index++)
	{
		if (pDlg->m_bTestCenter && pDlg->m_SpcShift_En_0F && index>= startsfr0f && index <= endsfr0f)
		{
			if (pDlg->m_SpcShift_0F > 0.2)
			{
				m_errMsg = m_cfgName;
				m_errMsg += _T(" 0视场Spec Shift过大!");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				return FALSE;
			}

			float SFRValue_Diff = m_stSpec[index].SFRSpec - m_dSFRValue[index];
			if (SFRValue_Diff > pDlg->m_SpcShift_0F)
			{
				bPass = FALSE;
				m_errMsg = m_cfgName;
				if (!((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime)))
				{
					m_errMsg += _T(" 单点不良");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				}
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_0F)
			{
				//m_dSFRValue[index] += pDlg->m_SpcShift_0F;
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_0F;//??????
				//SFRValue_Diff = SFRValue_Diff+0.01;
				double value_offset = 0.0;
				int res = 0;
				//srand(time(NULL));
				res = rand();
				double ff = (res%4000)*0.00001;
				double value_offset_temp = ff;
				m_dSFRValue_Offset[index] += SFRValue_Diff;
				m_dSFRValue_Offset[index] = m_dSFRValue_Offset[index] + value_offset_temp;
				m_dSFRValue[index] = m_dSFRValue[index] + value_offset_temp;

				
			}
		}

		else if (pDlg->m_bTestF3 && pDlg->m_SpcShift_En_3F && index >=  startsfr3f && index <= endsfr3f)
		{
			if (pDlg->m_SpcShift_3F > 0.2)
			{
				m_errMsg = m_cfgName;
				m_errMsg += _T(" 3视场Spec Shift过大!");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				return FALSE;
			}

			float SFRValue_Diff = m_stSpec[index].SFRSpec - m_dSFRValue[index];
			if (SFRValue_Diff > pDlg->m_SpcShift_3F)
			{
				bPass = FALSE;
				m_errMsg = m_cfgName;
				if (!((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime)))
				{
					m_errMsg += _T(" 单点不良");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				}
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_3F)
			{
				//m_dSFRValue[index] += pDlg->m_SpcShift_3F;
					m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_0F;//??????
				//SFRValue_Diff = SFRValue_Diff+0.01;
				double value_offset = 0.0;
				int res = 0;
				//srand(time(NULL));
				res = rand();
				double ff = (res%4000)*0.00001;
				double value_offset_temp = ff;
				m_dSFRValue_Offset[index] += SFRValue_Diff;
				m_dSFRValue_Offset[index] = m_dSFRValue_Offset[index] + value_offset_temp;
				m_dSFRValue[index] = m_dSFRValue[index] + value_offset_temp;			
				
			}
		}

		else if (pDlg->m_bTestF4 && pDlg->m_SpcShift_En_4F && index >=  startsfr4f && index <= endsfr4f)
		{
			if (pDlg->m_SpcShift_4F > 0.2)
			{
				m_errMsg = m_cfgName;
				m_errMsg += _T(" 4视场Spec Shift过大!");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				return FALSE;
			}

			float SFRValue_Diff = m_stSpec[index].SFRSpec - m_dSFRValue[index];
			if (SFRValue_Diff > pDlg->m_SpcShift_4F)
			{
				bPass = FALSE;
				m_errMsg = m_cfgName;
				if (!((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime)))
				{
					m_errMsg += _T(" 单点不良");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				}
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_4F)
			{
				//m_dSFRValue[index] += pDlg->m_SpcShift_4F;
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_0F;//??????
				//SFRValue_Diff = SFRValue_Diff+0.01;
				double value_offset = 0.0;
				int res = 0;
				//srand(time(NULL));
				res = rand();
				double ff = (res%4000)*0.00001;
				double value_offset_temp = ff;
				m_dSFRValue_Offset[index] += SFRValue_Diff;
				m_dSFRValue_Offset[index] = m_dSFRValue_Offset[index] + value_offset_temp;
				m_dSFRValue[index] = m_dSFRValue[index] + value_offset_temp;				
			}
		}

		else if (pDlg->m_bTestF5 && pDlg->m_SpcShift_En_5F && index >=  startsfr5f && index <= endsfr5f)
		{
			if (pDlg->m_SpcShift_5F > 0.2)
			{
				m_errMsg = m_cfgName;
				m_errMsg += _T(" 5视场Spec Shift过大!");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				return FALSE;
			}

			float SFRValue_Diff = m_stSpec[index].SFRSpec - m_dSFRValue[index];
			if (SFRValue_Diff > pDlg->m_SpcShift_5F)
			{
				bPass = FALSE;
				m_errMsg = m_cfgName;
				if (!((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime)))
				{
					m_errMsg += _T(" 单点不良");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				}
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_5F)
			{
				//m_dSFRValue[index] += pDlg->m_SpcShift_5F;
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_0F;//??????
				//SFRValue_Diff = SFRValue_Diff+0.01;
				double value_offset = 0.0;
				int res = 0;
				//srand(time(NULL));
				res = rand();
				double ff = (res%4000)*0.00001;
				double value_offset_temp = ff;
				m_dSFRValue_Offset[index] += SFRValue_Diff;
				m_dSFRValue_Offset[index] = m_dSFRValue_Offset[index] + value_offset_temp;
				m_dSFRValue[index] = m_dSFRValue[index] + value_offset_temp;				
			}
		}

		else if (pDlg->m_bTestF6 && pDlg->m_SpcShift_En_6F && index >=  startsfr6f && index <= endsfr6f)
		{
			if (pDlg->m_SpcShift_6F > 0.2)
			{
				m_errMsg = m_cfgName;
				m_errMsg += _T(" 6视场Spec Shift过大!");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				return FALSE;
			}

			float SFRValue_Diff = m_stSpec[index].SFRSpec - m_dSFRValue[index];
			if (SFRValue_Diff > pDlg->m_SpcShift_6F)
			{
				bPass = FALSE;
				m_errMsg = m_cfgName;
				if (!((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime)))
				{
					m_errMsg += _T(" 单点不良");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				}
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_6F)
			{
				//m_dSFRValue[index] += pDlg->m_SpcShift_6F;
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_0F;//??????
				//SFRValue_Diff = SFRValue_Diff+0.01;
				double value_offset = 0.0;
				int res = 0;
				//srand(time(NULL));
				res = rand();
				double ff = (res%4000)*0.00001;
				double value_offset_temp = ff;
				m_dSFRValue_Offset[index] += SFRValue_Diff;
				m_dSFRValue_Offset[index] = m_dSFRValue_Offset[index] + value_offset_temp;
				m_dSFRValue[index] = m_dSFRValue[index] + value_offset_temp;			
			}
		}

		else if (pDlg->m_bTestF8 && pDlg->m_SpcShift_En_8F && index >=  startsfr8f && index <= endsfr8f)
		{
			if (pDlg->m_SpcShift_8F > 0.2)
			{
				m_errMsg = m_cfgName;
				m_errMsg += _T(" 8视场Spec Shift过大!");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				return FALSE;
			}

			float SFRValue_Diff = m_stSpec[index].SFRSpec - m_dSFRValue[index];
			if (SFRValue_Diff > pDlg->m_SpcShift_8F)
			{
				bPass = FALSE;
				m_errMsg = m_cfgName;
				if (!((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime)))
				{
					m_errMsg += _T(" 单点不良");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				}
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_8F)
			{
				//m_dSFRValue[index] += pDlg->m_SpcShift_8F;
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_0F;//??????
				//SFRValue_Diff = SFRValue_Diff+0.01;
				double value_offset = 0.0;
				int res = 0;
				//srand(time(NULL));
				res = rand();
				double ff = (res%4000)*0.00001;
				double value_offset_temp = ff;
				m_dSFRValue_Offset[index] += SFRValue_Diff;
				m_dSFRValue_Offset[index] = m_dSFRValue_Offset[index] + value_offset_temp;
				m_dSFRValue[index] = m_dSFRValue[index] + value_offset_temp;				
			}
		}
		else
		{
			if (m_dSFRValue[index] < m_stSpec[index].SFRSpec)
			{
				bPass = FALSE;
				m_errMsg = m_cfgName;
				if (!((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime)))
				{
					m_errMsg += _T(" 单点不良");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				}
			}
		}
	}

	int Index_Max_H_8F = 0;
	int Index_Max_V_8F = 0;
	int Index_Min_H_8F = 0;
	int Index_Min_V_8F = 0;
	int Index_Max_8F = 0;
	int Index_Min_8F = 0;

	int Index_Max_H_6F = 0;
	int Index_Max_V_6F = 0;
	int Index_Min_H_6F = 0;
	int Index_Min_V_6F = 0;
	int Index_Max_6F = 0;
	int Index_Min_6F = 0;

	int Index_Max_H_5F = 0;
	int Index_Max_V_5F = 0;
	int Index_Min_H_5F = 0;
	int Index_Min_V_5F = 0;
	int Index_Max_5F = 0;
	int Index_Min_5F = 0;

	BOOL bCTC_Pass = TRUE;
	BOOL DataChange = FALSE;
	BOOL bPass_CTC_5F = TRUE;
	BOOL bPass_CTC_5F_H = TRUE;
	BOOL bPass_CTC_5F_V = TRUE;

	BOOL bPass_CTC_6F = TRUE;
	BOOL bPass_CTC_6F_H = TRUE;
	BOOL bPass_CTC_6F_V = TRUE;

	BOOL bPass_CTC_8F = TRUE;
	BOOL bPass_CTC_8F_H = TRUE;
	BOOL bPass_CTC_8F_V = TRUE;

	BOOL Threshhold = FALSE;
	BYTE *SFR_Shift_Cnt = new BYTE[m_iSFRROICount];
	if (SFR_Shift_Cnt == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("SFR_Shift_Cnt 内存分配失败"),COLOR_BLUE,200);
		return FALSE;
	}
	memset(SFR_Shift_Cnt,0,m_iSFRROICount);
	do 
	{
		DataChange = FALSE;
		bPass_CTC_5F = TRUE;
		BOOL bPass_CTC_5F_H = TRUE;
		BOOL bPass_CTC_5F_V = TRUE;

		bPass_CTC_6F = TRUE;
		BOOL bPass_CTC_6F_H = TRUE;
		BOOL bPass_CTC_6F_V = TRUE;

		bPass_CTC_8F = TRUE;
		BOOL bPass_CTC_8F_H = TRUE;
		BOOL bPass_CTC_8F_V = TRUE;

		float dMax8 = 0;
		float dMin8 = 1 ;
		float dMax6 = 0;
		float dMin6 = 1 ;
		float dMax5 = 0;
		float dMin5 = 1 ;

		float dMax8_H = 0;
		float dMin8_H = 1 ;
		float dMax8_V = 0;
		float dMin8_V = 1 ;

		float dMax6_H = 0;
		float dMin6_H = 1 ;				
		float dMax6_V = 0;
		float dMin6_V = 1 ;

		float dMax5_H = 0;
		float dMin5_H = 1 ;				
		float dMax5_V = 0;
		float dMin5_V = 1 ;
		double dMax8_fact = 0;
		double dMin8_fact = 1 ;
		double dMax6_fact = 0;
		double dMin6_fact = 1 ;
		double dMax5_fact = 0;
		double dMin5_fact = 1 ;

		double dMax8_H_fact = 0;
		double dMin8_H_fact = 1 ;
		double dMax8_V_fact = 0;
		double dMin8_V_fact = 1 ;

		double dMax6_H_fact = 0;
		double dMin6_H_fact = 1 ;				
		double dMax6_V_fact = 0;
		double dMin6_V_fact = 1;

		double dMax5_H_fact = 0;
		double dMin5_H_fact = 1 ;				
		double dMax5_V_fact = 0;
		double dMin5_V_fact = 1;



		//0.8F
		for (int index = startsfr8f; index <= endsfr8f ; index++)
		{
			double SFRtemp = m_dSFRValue[index];
			double SFRtemp_fact = m_dSFRValue_Fact[index];

			if (SFRtemp > dMax8)	
			{	
				dMax8 = (float)SFRtemp; 
				Index_Max_8F = index;
			}
			if (SFRtemp < dMin8)	
			{	
				dMin8 = (float)SFRtemp;
				Index_Min_8F = index;
			}
			if (SFRtemp_fact > dMax8_fact)	
			{	
				dMax8_fact =SFRtemp_fact; 
			}
			if (SFRtemp_fact < dMin8_fact)	
			{	
				dMin8_fact = SFRtemp_fact;
			}
		}
		//0.8视场分方向
		for (int index = startsfr8f; index < endsfr8f ; index+=2)
		{
			double SFRtemp_H = m_dSFRValue[index];
			double SFRtemp_V = m_dSFRValue[index+1];
			double SFRtemp_H_fact = m_dSFRValue_Fact[index];
			double SFRtemp_V_fact = m_dSFRValue_Fact[index+1];

			if (SFRtemp_H > dMax8_H)	
			{	
				dMax8_H = (float)SFRtemp_H;
				Index_Max_H_8F = index;
			}
			if (SFRtemp_H < dMin8_H)
			{	
				dMin8_H = (float)SFRtemp_H;	
				Index_Min_H_8F = index;
			}
			if (SFRtemp_V > dMax8_V)
			{	
				dMax8_V = (float)SFRtemp_V; 
				Index_Max_V_8F = index+1;
			}
			if (SFRtemp_V < dMin8_V)	
			{	
				dMin8_V = (float)SFRtemp_V;	
				Index_Min_V_8F = index+1;
			}
			if (SFRtemp_H_fact > dMax8_H_fact)	
			{	
				dMax8_H_fact =SFRtemp_H_fact; 
			}
			if (SFRtemp_H_fact < dMin8_H_fact)	
			{	
				dMin8_H_fact = SFRtemp_H_fact;
			}
			if (SFRtemp_V_fact > dMax8_V_fact)	
			{	
				dMax8_V_fact =SFRtemp_V_fact; 
			}
			if (SFRtemp_V_fact < dMin8_V_fact)	
			{	
				dMin8_V_fact = SFRtemp_V_fact;
			}


		}
		//0.6视场
		for (int index =startsfr6f; index <= endsfr6f ; index++)
		{
			double SFRtemp = m_dSFRValue[index];
			double SFRtemp_fact = m_dSFRValue_Fact[index];


			if (SFRtemp > dMax6)	
			{	
				dMax6 = (float)SFRtemp;
				Index_Max_6F = index;
			}
			if (SFRtemp < dMin6)	
			{	
				dMin6 = (float)SFRtemp;	
				Index_Min_6F = index;
			}
			if (SFRtemp_fact > dMax6_fact)	
			{	
				dMax6_fact =SFRtemp_fact; 
			}
			if (SFRtemp_fact < dMin6_fact)	
			{	
				dMin6_fact = SFRtemp_fact;
			}
		}
		//0.6视场分方向
		for (int index = startsfr6f; index < endsfr6f ; index+=2)
		{
			double SFRtemp_H = m_dSFRValue[index];
			double SFRtemp_V = m_dSFRValue[index+1];
			double SFRtemp_H_fact = m_dSFRValue_Fact[index];
			double SFRtemp_V_fact = m_dSFRValue_Fact[index+1];

			if (SFRtemp_H > dMax6_H)	
			{	
				dMax6_H = (float)SFRtemp_H; 
				Index_Max_H_6F = index;
			}
			if (SFRtemp_H < dMin6_H)	
			{
				dMin6_H = (float)SFRtemp_H;	
				Index_Min_H_6F = index;
			}
			if (SFRtemp_V > dMax6_V)	
			{
				dMax6_V = (float)SFRtemp_V; 
				Index_Max_V_6F = index+1;
			}
			if (SFRtemp_V < dMin6_V)	
			{	
				dMin6_V = (float)SFRtemp_V;	
				Index_Min_V_6F = index+1;
			}
			if (SFRtemp_H_fact > dMax6_H_fact)	
			{	
				dMax6_H_fact =SFRtemp_H_fact; 
			}
			if (SFRtemp_H_fact < dMin6_H_fact)	
			{	
				dMin6_H_fact = SFRtemp_H_fact;
			}
			if (SFRtemp_V_fact > dMax6_V_fact)	
			{	
				dMax6_V_fact =SFRtemp_V_fact; 
			}
			if (SFRtemp_V_fact < dMin6_V_fact)	
			{	
				dMin6_V_fact = SFRtemp_V_fact;
			}
		}
		//0.5视场
		for (int index =startsfr5f; index <= endsfr5f ; index++)
		{
			double SFRtemp = m_dSFRValue[index];
			double SFRtemp_fact = m_dSFRValue_Fact[index];

			if (SFRtemp > dMax5)	
			{	
				dMax5 = (float)SFRtemp;
				Index_Max_5F = index;
			}
			if (SFRtemp < dMin5)	
			{	
				dMin5 = (float)SFRtemp;	
				Index_Min_5F = index;
			}
			if (SFRtemp_fact > dMax5_fact)	
			{	
				dMax5_fact = (float)SFRtemp_fact;
				
			}
			if (SFRtemp_fact < dMin5_fact)	
			{	
				dMin5_fact = (float)SFRtemp_fact;	
				
			}
		}
		//0.5视场分方向
		for (int index = startsfr5f; index < endsfr5f ; index+=2)
		{
			float SFRtemp_H = m_dSFRValue[index];
			float SFRtemp_V = m_dSFRValue[index+1];
			float SFRtemp_H_fact = m_dSFRValue_Fact[index];
			float SFRtemp_V_fact = m_dSFRValue_Fact[index+1];
			if (SFRtemp_H > dMax5_H)	
			{	
				dMax5_H =SFRtemp_H; 
				Index_Max_H_5F = index;
			}
			if (SFRtemp_H < dMin5_H)	
			{
				dMin5_H = SFRtemp_H;	
				Index_Min_H_5F = index;
			}
			if (SFRtemp_V > dMax5_V)	
			{
				dMax5_V =SFRtemp_V; 
				Index_Max_V_5F = index+1;
			}
			if (SFRtemp_V < dMin5_V)	
			{	
				dMin5_V = SFRtemp_V;	
				Index_Min_V_5F = index+1;
			}
			if (SFRtemp_H_fact > dMax5_H_fact)	
			{	
				dMax5_H_fact =SFRtemp_H_fact; 
			}
			if (SFRtemp_H_fact < dMin5_H_fact)	
			{
				dMin5_H_fact = SFRtemp_H_fact;	
			}
			if (SFRtemp_V_fact > dMax5_V_fact)	
			{
				dMax5_V_fact =SFRtemp_V_fact; 

			}
			if (SFRtemp_V_fact < dMin5_V_fact)	
			{	
				dMin5_V_fact = SFRtemp_V_fact;	

			}
		}
		if (dMax8 == 0)	{	dMax8 = 1;	}
		if (dMax6== 0)	{	dMax6 = 1;	}
		if (dMax5== 0)	{	dMax5 = 1;	}
		if (dMax8_H == 0)	{	dMax8_H = 1;	}
		if (dMax6_H == 0)	{	dMax6_H = 1;	}
		if (dMax5_H == 0)	{	dMax5_H = 1;	}
		if (dMax8_V == 0)	{	dMax8_V = 1;	}
		if (dMax6_V == 0)	{	dMax6_V = 1;	}
		if (dMax5_V == 0)	{	dMax5_V = 1;	}

		if (dMax8_fact == 0)	{	dMax8_fact = 1;	}
		if (dMax6_fact== 0)	{	dMax6_fact = 1;	}
		if (dMax8_H_fact == 0)	{	dMax8_H_fact = 1;	}
		if (dMax6_H_fact == 0)	{	dMax6_H_fact = 1;	}
		if (dMax8_V_fact == 0)	{	dMax8_V_fact = 1;	}
		if (dMax6_V_fact == 0)	{	dMax6_V_fact = 1;	}
		if (dMax5_V_fact == 0)	{	dMax5_V_fact = 1;	}
		if (pDlg->m_cornerDiffRelative == TRUE)
		{
			m_fCTC5 = (dMax5 - dMin5)/dMax5 * 100;
			m_fCTC6 = (dMax6 - dMin6)/dMax6 * 100;
			m_fCTC8 = (dMax8 - dMin8)/dMax8 * 100;
			m_fCTC5_H = (dMax5_H - dMin5_H)/dMax5_H * 100;
			m_fCTC6_H = (dMax6_H - dMin6_H)/dMax6_H * 100;
			m_fCTC8_H = (dMax8_H - dMin8_H)/dMax8_H * 100;
			m_fCTC5_V = (dMax5_V - dMin5_V)/dMax5_V * 100;
			m_fCTC6_V = (dMax6_V - dMin6_V)/dMax6_V * 100;
			m_fCTC8_V = (dMax8_V - dMin8_V)/dMax8_V * 100;

			m_fCTC5_Fact = (dMax5_fact - dMin5_fact)/dMax5_fact * 100;
			m_fCTC6_Fact = (dMax6_fact - dMin6_fact)/dMax6_fact * 100;
			m_fCTC8_Fact = (dMax8_fact - dMin8_fact)/dMax8_fact * 100;
			m_fCTC5_H_Fact = (dMax5_H_fact - dMin5_H_fact)/dMax5_H_fact * 100;
			m_fCTC6_H_Fact = (dMax6_H_fact - dMin6_H_fact)/dMax6_H_fact * 100;
			m_fCTC8_H_Fact = (dMax8_H_fact - dMin8_H_fact)/dMax8_H_fact * 100;
			m_fCTC5_V_Fact = (dMax5_V_fact - dMin5_V_fact)/dMax5_V_fact * 100;
			m_fCTC6_V_Fact = (dMax6_V_fact - dMin6_V_fact)/dMax6_V_fact * 100;
			m_fCTC8_V_Fact = (dMax8_V_fact - dMin8_V_fact)/dMax8_V_fact * 100;
		}
		else
		{
			m_fCTC5 = (dMax5 - dMin5) * 100;
			m_fCTC6 = (dMax6 - dMin6) * 100;
			m_fCTC8 = (dMax8 - dMin8) * 100;
			m_fCTC5_H = (dMax5_H - dMin5_H) * 100;
			m_fCTC6_H = (dMax6_H - dMin6_H) * 100;
			m_fCTC8_H = (dMax8_H - dMin8_H) * 100;
			m_fCTC5_V = (dMax5_V - dMin5_V) * 100;
			m_fCTC6_V = (dMax6_V - dMin6_V) * 100;
			m_fCTC8_V = (dMax8_V - dMin8_V) * 100;

			m_fCTC5_Fact = (dMax5_fact - dMin5_fact) * 100;
			m_fCTC6_Fact = (dMax6_fact - dMin6_fact) * 100;
			m_fCTC8_Fact = (dMax8_fact - dMin8_fact) * 100;
			m_fCTC5_H_Fact = (dMax5_H_fact - dMin5_H_fact) * 100;
			m_fCTC6_H_Fact = (dMax6_H_fact - dMin6_H_fact) * 100;
			m_fCTC8_H_Fact = (dMax8_H_fact - dMin8_H_fact) * 100;
			m_fCTC5_V_Fact = (dMax5_V_fact - dMin5_V_fact) * 100;
			m_fCTC6_V_Fact = (dMax6_V_fact - dMin6_V_fact) * 100;
			m_fCTC8_V_Fact = (dMax8_V_fact - dMin8_V_fact) * 100;
		}

		if (m_fCTC8 > m_Options.m_fCTC8)
		{	
			bPass_CTC_8F = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF8 && pDlg->m_SpcShift_CTC_En_8F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_8F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T(" 0.8视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}
				float _8F_Diff = dMax8 - m_dSFRValue[Index_Min_8F] - (float)0.01;
				if (_8F_Diff > 0)
				{
					if (SFR_Shift_Cnt[Index_Min_8F]==0)
					{
						if (_8F_Diff >= pDlg->m_SpcShift_8F_CTC)
						{
							if (
								(pDlg->m_cornerDiffRelative&&((dMax8-(m_dSFRValue[Index_Min_8F]+pDlg->m_SpcShift_8F_CTC))/dMax8*100)<m_Options.m_fCTC8)
								||(!pDlg->m_cornerDiffRelative&&((dMax8-(m_dSFRValue[Index_Min_8F]+pDlg->m_SpcShift_8F_CTC))*100)<m_Options.m_fCTC8)
								)
							m_dSFRValue[Index_Min_8F] += pDlg->m_SpcShift_8F_CTC;
							else
							{
								Threshhold = TRUE;
								bCTC_Pass = FALSE;
							}
						}
						else
						{
							m_dSFRValue[Index_Min_8F] += _8F_Diff;
						}
						m_dSFRValue_Offset[Index_Min_8F] = m_dSFRValue[Index_Min_8F];
						DataChange = TRUE;
					}
					SFR_Shift_Cnt[Index_Min_8F]++;

				}
				else break;

			}
		}

		if (m_fCTC6 > m_Options.m_fCTC6 )
		{	
			bPass_CTC_6F = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF6 && pDlg->m_SpcShift_CTC_En_6F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_6F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T(" 0.6视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}
				float _6F_Diff = dMax6 - m_dSFRValue[Index_Min_6F] - (float)0.01;
				if (_6F_Diff > 0)
				{
					if ( SFR_Shift_Cnt[Index_Min_6F] == 0)
					{
						if (_6F_Diff >= pDlg->m_SpcShift_6F_CTC)
						{
							if (
								(pDlg->m_cornerDiffRelative&&((dMax6-(m_dSFRValue[Index_Min_6F]+pDlg->m_SpcShift_6F_CTC))/dMax6*100)<m_Options.m_fCTC6)
								||(!pDlg->m_cornerDiffRelative&&((dMax6-(m_dSFRValue[Index_Min_6F]+pDlg->m_SpcShift_6F_CTC))*100)<m_Options.m_fCTC6)
								)
							m_dSFRValue[Index_Min_6F] += pDlg->m_SpcShift_6F_CTC;
							else
							{
								Threshhold = TRUE;
								bCTC_Pass = FALSE;
							}
						}
						else
						{
							m_dSFRValue[Index_Min_6F] += _6F_Diff;
						}
						m_dSFRValue_Offset[Index_Min_6F] = m_dSFRValue[Index_Min_6F];
						DataChange = TRUE;
					}
					SFR_Shift_Cnt[Index_Min_6F]++;
				}
				else break;

			}
		}

		if (m_fCTC5 > m_Options.m_fCTC5 )
		{	
			bPass_CTC_5F = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF5 && pDlg->m_SpcShift_CTC_En_5F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_5F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T(" 0.5视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}
				float _5F_Diff = dMax5 - m_dSFRValue[Index_Min_5F] - (float)0.01;
				if (_5F_Diff > 0)
				{
					if ( SFR_Shift_Cnt[Index_Min_5F] == 0)
					{
						if (_5F_Diff >= pDlg->m_SpcShift_5F_CTC)
						{
							if (
								(pDlg->m_cornerDiffRelative&&((dMax5-(m_dSFRValue[Index_Min_5F]+pDlg->m_SpcShift_5F_CTC))/dMax5*100)<m_Options.m_fCTC5)
								||(!pDlg->m_cornerDiffRelative&&((dMax5-(m_dSFRValue[Index_Min_5F]+pDlg->m_SpcShift_5F_CTC))*100)<m_Options.m_fCTC5)
								)
							m_dSFRValue[Index_Min_5F] += pDlg->m_SpcShift_5F_CTC;
							else
							{
								Threshhold = TRUE;
								bCTC_Pass = FALSE;
							}
						}
						else
						{
							m_dSFRValue[Index_Min_5F] += _5F_Diff;
						}
						m_dSFRValue_Offset[Index_Min_5F] = m_dSFRValue[Index_Min_5F];
						DataChange = TRUE;
					}
					SFR_Shift_Cnt[Index_Min_5F]++;
				}
				else break;

			}
		}

		if (m_fCTC5_H > m_Options.m_fCTC5_H )
		{	
			bPass_CTC_5F_H = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF5 && pDlg->m_SpcShift_CTC_En_5F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_5F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T(" 0.5视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}
				if ((Index_Min_H_5F != Index_Min_5F) || (Index_Min_H_5F == Index_Min_5F && bPass_CTC_5F==TRUE) )
				{
					float _5F_H_Diff = dMax5_H - m_dSFRValue[Index_Min_H_5F] - (float)0.01;
					if (_5F_H_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_H_5F]==0)
						{
							if (_5F_H_Diff >= pDlg->m_SpcShift_5F_CTC)
							{
								if (
									(pDlg->m_cornerDiffRelative&&((dMax5_H-(m_dSFRValue[Index_Min_H_5F]+pDlg->m_SpcShift_5F_CTC))/dMax5_H*100)<m_Options.m_fCTC5_H)
									||(!pDlg->m_cornerDiffRelative&&((dMax5_H-(m_dSFRValue[Index_Min_H_5F]+pDlg->m_SpcShift_5F_CTC))*100)<m_Options.m_fCTC5_H)
									)
								m_dSFRValue[Index_Min_H_5F] += pDlg->m_SpcShift_5F_CTC;
								else
								{
									Threshhold = TRUE;
									bCTC_Pass = FALSE;
								}
							}
							else
							{
								m_dSFRValue[Index_Min_H_5F] += _5F_H_Diff;
							}
							m_dSFRValue_Offset[Index_Min_H_5F] = m_dSFRValue[Index_Min_H_5F];
							DataChange = TRUE;
						}
						SFR_Shift_Cnt[Index_Min_H_5F]++;
					}
					else break;
				}
			}
		}

		if (m_fCTC5_V > m_Options.m_fCTC5_V )
		{	
			bPass_CTC_5F_V = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF5 && pDlg->m_SpcShift_CTC_En_5F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_5F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T(" 0.5视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}
				if ((Index_Min_V_5F != Index_Min_5F) || (Index_Min_V_5F == Index_Min_5F && bPass_CTC_5F == TRUE))
				{
					float _5F_V_Diff = (float)(dMax5_V - m_dSFRValue[Index_Min_V_5F] - 0.01);
					if (_5F_V_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_V_5F]==0)
						{
							if (_5F_V_Diff >= pDlg->m_SpcShift_5F_CTC)
							{
								if (
									(pDlg->m_cornerDiffRelative&&((dMax5_V-(m_dSFRValue[Index_Min_V_5F]+pDlg->m_SpcShift_5F_CTC))/dMax5_V*100)<m_Options.m_fCTC5_V)
									||(!pDlg->m_cornerDiffRelative&&((dMax5_V-(m_dSFRValue[Index_Min_V_5F]+pDlg->m_SpcShift_5F_CTC))*100)<m_Options.m_fCTC5_V)
									)
									m_dSFRValue[Index_Min_V_5F] += pDlg->m_SpcShift_5F_CTC;
								else
								{
									Threshhold = TRUE;
									bCTC_Pass = FALSE;
								}
							}
							else
							{
								m_dSFRValue[Index_Min_V_5F] += _5F_V_Diff;
							}
							m_dSFRValue_Offset[Index_Min_V_5F] = m_dSFRValue[Index_Min_V_5F];
							DataChange = TRUE;
						}
						SFR_Shift_Cnt[Index_Min_V_5F]++;
					}
					else break;

				}

			}
		}

		if (m_fCTC6_H > m_Options.m_fCTC6_H )
		{	
			bPass_CTC_6F_H = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF6 && pDlg->m_SpcShift_CTC_En_6F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_6F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T(" 0.6视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}
				if ((Index_Min_H_6F != Index_Min_6F) || (Index_Min_H_6F == Index_Min_6F && bPass_CTC_6F==TRUE) )
				{
					float _6F_H_Diff = dMax6_H - m_dSFRValue[Index_Min_H_6F] - (float)0.01;
					if (_6F_H_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_H_6F]==0)
						{
							if (_6F_H_Diff >= pDlg->m_SpcShift_6F_CTC)
							{
								if (
									(pDlg->m_cornerDiffRelative&&((dMax6_H-(m_dSFRValue[Index_Min_H_6F]+pDlg->m_SpcShift_6F_CTC))/dMax6_H*100)<m_Options.m_fCTC6_H)
									||(!pDlg->m_cornerDiffRelative&&((dMax6_H-(m_dSFRValue[Index_Min_H_6F]+pDlg->m_SpcShift_6F_CTC))*100)<m_Options.m_fCTC6_H)
									)
								m_dSFRValue[Index_Min_H_6F] += pDlg->m_SpcShift_6F_CTC;
								else
								{
									Threshhold = TRUE;
									bCTC_Pass = FALSE;
								}
							}
							else
							{
								m_dSFRValue[Index_Min_H_6F] += _6F_H_Diff;
							}
							m_dSFRValue_Offset[Index_Min_H_6F] = m_dSFRValue[Index_Min_H_6F];
							DataChange = TRUE;
						}
						SFR_Shift_Cnt[Index_Min_H_6F]++;
					}
					else break;
				}
			}
		}

		if (m_fCTC6_V > m_Options.m_fCTC6_V )
		{	
			bPass_CTC_6F_V = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF6 && pDlg->m_SpcShift_CTC_En_6F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_6F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T(" 0.6视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}
				if ((Index_Min_V_6F != Index_Min_6F) || (Index_Min_V_6F == Index_Min_6F && bPass_CTC_6F == TRUE))
				{
					float _6F_V_Diff = (float)(dMax6_V - m_dSFRValue[Index_Min_V_6F] - 0.01);
					if (_6F_V_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_V_6F]==0)
						{
							if (_6F_V_Diff >= pDlg->m_SpcShift_6F_CTC)
							{
								if (
									(pDlg->m_cornerDiffRelative&&((dMax6_V-(m_dSFRValue[Index_Min_V_6F]+pDlg->m_SpcShift_6F_CTC))/dMax6_V*100)<m_Options.m_fCTC6_V)
									||(!pDlg->m_cornerDiffRelative&&((dMax6_V-(m_dSFRValue[Index_Min_V_6F]+pDlg->m_SpcShift_6F_CTC))*100)<m_Options.m_fCTC6_V)
									)
								m_dSFRValue[Index_Min_V_6F] += pDlg->m_SpcShift_6F_CTC;
								else
								{
									Threshhold = TRUE;
									bCTC_Pass = FALSE;
								}
							}
							else
							{
								m_dSFRValue[Index_Min_V_6F] += _6F_V_Diff;
							}
							m_dSFRValue_Offset[Index_Min_V_6F] = m_dSFRValue[Index_Min_V_6F];
							DataChange = TRUE;
						}
						SFR_Shift_Cnt[Index_Min_V_6F]++;
					}
					else break;

				}

			}
		}

		if (m_fCTC8_H > m_Options.m_fCTC8_H )
		{	
			bPass_CTC_8F_H = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF8 && pDlg->m_SpcShift_CTC_En_8F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_8F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T(" 0.8视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}

				if ((Index_Min_H_8F != Index_Min_8F) || (Index_Min_H_8F == Index_Min_8F && bPass_CTC_8F == TRUE) )
				{
					float _8F_H_Diff = (float)(dMax8_H - m_dSFRValue[Index_Min_H_8F] - 0.01);
					if (_8F_H_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_H_8F]==0)
						{
							if (_8F_H_Diff >= pDlg->m_SpcShift_8F_CTC)
							{
								if (
									(pDlg->m_cornerDiffRelative&&((dMax8_H-(m_dSFRValue[Index_Min_H_8F]+pDlg->m_SpcShift_8F_CTC))/dMax8_H*100)<m_Options.m_fCTC8_H)
									||(!pDlg->m_cornerDiffRelative&&((dMax8_H-(m_dSFRValue[Index_Min_H_8F]+pDlg->m_SpcShift_8F_CTC))*100)<m_Options.m_fCTC8_H)
									)
								m_dSFRValue[Index_Min_H_8F] += pDlg->m_SpcShift_8F_CTC;
								else
								{
									Threshhold = TRUE;
									bCTC_Pass = FALSE;
								}
							}
							else
							{
								m_dSFRValue[Index_Min_H_8F] += _8F_H_Diff;
							}
							m_dSFRValue_Offset[Index_Min_H_8F] = m_dSFRValue[Index_Min_H_8F];
							DataChange = TRUE;
						}
						SFR_Shift_Cnt[Index_Min_H_8F]++;
					}
					else break;

				}

			}
		}

		if (m_fCTC8_V > m_Options.m_fCTC8_V )
		{	
			bPass_CTC_8F_V = FALSE; 
			if (Threshhold == TRUE)
			{
				break;
			}
			if ( pDlg->m_bTestF8 && pDlg->m_SpcShift_CTC_En_8F && bPass == TRUE)
			{
				if (pDlg->m_SpcShift_8F_CTC > 0.2)
				{
					m_errMsg = m_cfgName;
					m_errMsg += _T("0.8视场CTC Spec Shift过大!");
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
					m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
					SAFE_DELETE_ARRAY(SFR_Shift_Cnt);
					return FALSE;
				}
				if ((Index_Min_V_8F != Index_Min_8F) || (Index_Min_V_8F == Index_Min_8F && bPass_CTC_8F == TRUE))
				{
					float _8F_V_Diff = (float)(dMax8_V - m_dSFRValue[Index_Min_V_8F] - 0.01);
					if (_8F_V_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_V_8F]==0)
						{
							if (_8F_V_Diff >= pDlg->m_SpcShift_8F_CTC)
							{
								if (
									(pDlg->m_cornerDiffRelative&&((dMax8_V-(m_dSFRValue[Index_Min_V_8F]+pDlg->m_SpcShift_8F_CTC))/dMax8_V*100)<m_Options.m_fCTC8_V)
									||(!pDlg->m_cornerDiffRelative&&((dMax8_V-(m_dSFRValue[Index_Min_V_8F]+pDlg->m_SpcShift_8F_CTC))*100)<m_Options.m_fCTC8_V)
									)
								m_dSFRValue[Index_Min_V_8F] += pDlg->m_SpcShift_8F_CTC;
								else
								{
									Threshhold = TRUE;
									bCTC_Pass = FALSE;
								}
							}
							else
							{
								m_dSFRValue[Index_Min_V_8F] += _8F_V_Diff;
							}
							m_dSFRValue_Offset[Index_Min_V_8F] = m_dSFRValue[Index_Min_V_8F];
							DataChange = TRUE;
						}
						SFR_Shift_Cnt[Index_Min_V_8F]++;
					}
					else break;
				}
			}
		}

		if (pDlg->m_SpcShift_CTC_En_8F)
		{
			for (int index =startsfr8f; index <= endsfr8f ; index++)
			{
				if (SFR_Shift_Cnt[index]>1)
				{
					Threshhold = TRUE;
				}
			}
		}
		else
		{
			if (bPass_CTC_8F == FALSE)
			{
				Threshhold = TRUE;
			}
		}

		if (pDlg->m_SpcShift_CTC_En_6F)
		{
			for (int index =startsfr6f; index <= endsfr6f ; index++)
			{
				if (SFR_Shift_Cnt[index]>1)
				{
					Threshhold = TRUE;
				}
			}
		}
		else
		{
			if (bPass_CTC_6F == FALSE)
			{
				Threshhold = TRUE;
			}
		}

		if (pDlg->m_SpcShift_CTC_En_5F)
		{
			for (int index =startsfr5f; index <= endsfr5f ; index++)
			{
				if (SFR_Shift_Cnt[index]>1)
				{
					Threshhold = TRUE;
				}
			}
		}
		else
		{
			if (bPass_CTC_5F == FALSE)
			{
				Threshhold = TRUE;
			}
		}
		if (bPass_CTC_5F == FALSE || bPass_CTC_5F_H == FALSE || bPass_CTC_6F == FALSE || bPass_CTC_6F_H == FALSE || bPass_CTC_6F_V == FALSE || bPass_CTC_8F == FALSE || bPass_CTC_8F_H == FALSE || bPass_CTC_8F_V == FALSE)
		{
			bCTC_Pass = FALSE;
		}
		else
		{
			bCTC_Pass = TRUE;
		}

	} while ((DataChange == TRUE) ||(Threshhold == FALSE && bCTC_Pass==FALSE && bPass == TRUE && ((pDlg->m_SpcShift_CTC_En_8F&&pDlg->m_SpcShift_8F_CTC>0) || (pDlg->m_SpcShift_CTC_En_6F&&pDlg->m_SpcShift_6F_CTC>0 || (pDlg->m_SpcShift_CTC_En_5F&&pDlg->m_SpcShift_5F_CTC>0)))));

	SAFE_DELETE_ARRAY(SFR_Shift_Cnt);

	if (bCTC_Pass == FALSE)
	{
		if ((pDlg->m_AFTestMode == FQC_MODE)&&(fqcFirstTime))
		{
			;
		}
		else
		{
			m_errMsg = m_cfgName;
			m_errMsg += _T(" CTC NG");
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
			m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
		}
		bPass = FALSE;
	}	

	CString temp;
	temp.Format(_T("CTC8:%.4f"),m_fCTC8);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC6:%.4f"),m_fCTC6);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC5:%.4f"),m_fCTC5);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);

	temp.Format(_T("CTC8_H:%.4f"),m_fCTC8_H);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC6_H:%.4f"),m_fCTC6_H);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC5_H:%.4f"),m_fCTC5_H);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);

	temp.Format(_T("CTC8_V:%.4f"),m_fCTC8_V);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC6_V:%.4f"),m_fCTC6_V);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC5_V:%.4f"),m_fCTC5_V);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);

	for (int index = 0; index < m_iSFRROICount; index++)
	{
		if (m_dSFRValue[index] > 0.99)
		{
			CString Temp;
			Temp.Format(_T("Block_%d  SFR数据 大于0.99!"),index);
			m_errMsg = m_cfgName;
			m_errMsg += Temp;
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
			m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
			bPass = FALSE;
		}	
	}

	return bPass;
}
BOOL AF_SFR_TestItem::SaveAllData()
{
	SetName(L"GENERAL_SETTING");
	CString strReportPath = ReadConfigString(_T("sReportPath"),_T("0"));	
	SetName(m_cfgName);

	//////////////////////////////////////////////////////////////////////////
	FILE* fp = NULL;
	SYSTEMTIME st;
	GetLocalTime(&st);
	_wmkdir(strReportPath);

	//strReportPath += _T("\\SFR_Course_Data");
	strReportPath+=_T("\\");
	strReportPath+=GetName();
	_wmkdir(strReportPath);
	
	CString strTimeString ;
	strTimeString.Format(_T("%04d%02d%02d"),st.wYear,st.wMonth,st.wDay);
	CString strDataFileName = strReportPath + _T("\\") + _T("AutoFocus_Data_") + strTimeString + _T(".csv");
	m_pInterface->Ctrl_Addlog(CamID,strDataFileName,COLOR_BLUE,200);
	fp = _wfopen(strDataFileName,_T("a"));
	if (fp == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("打开文档失败，无法保存数据"),COLOR_RED,LOGHEIGHT);
		return FALSE;
	}
	fclose(fp);
	CStdioFile DataFile(strDataFileName, CFile::modeWrite | CFile::typeText);
	if (DataFile.GetLength() == 0)
	{
		CString strHeader = _T("SensorID,Time,DAC,");
		if (m_Options.m_iAFMethod == 0 )
		{
			if (m_Options.bEngineerMode == FALSE)
			{
				for (int i = 0; i < 2; i++)
				{
					strHeader += m_stSpec[i].ROIName;
					strHeader += _T(",");
				}
			}else
			{
				for (int i = 0; i < m_iSFRROICount; i++)
				{
					strHeader += m_stSpec[i].ROIName;
					strHeader += _T(",");
				}
			}
		}else if (m_Options.m_iAFMethod == 1)
		{
			strHeader += _T("ContrastValue");
		}else
		{
			strHeader += _T("MTF");
		}

		strHeader += _T("\n");
		DataFile.WriteString(strHeader);
	}
	DataFile.SeekToEnd();
	CString strContent = _T("");
	strContent.Format(_T("%s,%02d:%02d:%02d,\n"),m_pInterface->Sensor_GetCamFuseID(CamID),
		st.wHour,st.wMinute,st.wSecond);
	DataFile.WriteString(strContent);
	if (m_Options.m_iAFMethod == 0 || m_iPeroid == 2)
	{
		for (int i = m_iLastPos; i < m_iPos; i++)
		{
			strContent.Format(_T(",,%d,%f,%f,"),
				m_stAFData[i].DAC,m_stAFData[i].SFR[0],m_stAFData[i].SFR[1]);	
			if (m_Options.bEngineerMode)
			{
				CString temp;
				for (int index = 2; index < m_iSFRROICount; index++)
				{
					temp.Format(_T("%f,"),m_stAFData[i].SFR[index]);
					strContent += temp;
				}
			}
			strContent += _T("\n");
			DataFile.WriteString(strContent);
		}
	}else if (m_Options.m_iAFMethod == 1 )
	{		
		for (int i = m_iLastPos; i < m_iPos; i++)
		{
			strContent = _T("");
			strContent.Format(_T(",,%d,%d\n"),m_stContrastData[i].DAC,m_stContrastData[i].iContrastValue);
			DataFile.WriteString(strContent);
		}
	}else
	{
		for (int i = m_iLastPos; i < m_iPos; i++)
		{
			strContent = _T("");
			strContent.Format(_T(",,%d,%.2f\n"),m_stMTFData[i].DAC,m_stMTFData[i].dMTF);
			DataFile.WriteString(strContent);
		}
	}

	DataFile.Close();
	return TRUE;
}

BOOL AF_SFR_TestItem::SaveFinalData()
{
	SetName(L"GENERAL_SETTING");
	CString strReportPath = ReadConfigString(_T("sReportPath"),_T("0"));	
	SetName(m_cfgName);
	_wmkdir(strReportPath);

	FILE* fp = NULL;
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString DLL_Name = GetName();

	CString strTimeString ;
	strTimeString.Format(_T("%04d%02d%02d"),st.wYear,st.wMonth,st.wDay);

	CString strDataFileName = strReportPath + _T("\\") + DLL_Name+ _T("_FinalSFRData_") + strTimeString + _T(".csv");

	fp = _wfopen(strDataFileName,_T("a"));
	if (fp == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("打开文档失败，无法保存数据"),COLOR_RED,LOGHEIGHT);
		return FALSE;
	}
	fclose(fp);
	CStdioFile DataFile(strDataFileName, CFile::modeWrite | CFile::typeText);
	if (DataFile.GetLength() == 0)
	{
		CString strHeader = _T("SensorID,Time,Result,DAC,");
		for (int i = 0; i < m_iSFRROICount; i++)
		{
			strHeader += m_stSpec[i].ROIName;
			strHeader += _T(",");
		}
		strHeader+= _T("CTC8,CTC6,CTC5,CTC8_H,CTC8_V,CTC6_H,CTC6_V,CTC5_H,CTC5_V,");
		strHeader += _T("\n");
		DataFile.WriteString(strHeader);
	}
	DataFile.SeekToEnd();
	CString strContent = _T("");
	strContent.Format(_T("%s,%02d:%02d:%02d,%s,%d,"),m_pInterface->Sensor_GetCamFuseID(CamID),
		st.wHour,st.wMinute,st.wSecond,m_bResult?_T("PASS"):_T("FAIL"),m_iPeakDAC);
	CString temp;
	for (int index = 0; index < m_iSFRROICount; index++)
	{
		temp.Format(_T("%f,"),m_dSFRValue[index]);
		strContent += temp;
	}
	temp.Format(_T("%f,%f,%f,%f,%f,%f,%f,%f,%f"),m_fCTC8,m_fCTC6,m_fCTC5,m_fCTC8_H,m_fCTC8_V,m_fCTC6_H,m_fCTC6_V,m_fCTC5_H,m_fCTC5_V);
	strContent += temp;
	strContent += _T("\n");
	DataFile.WriteString(strContent);
	DataFile.Close();

	return TRUE;
}

BOOL AF_SFR_TestItem::SaveCheckData()
{
	SetName(L"GENERAL_SETTING");
	CString strReportPath = ReadConfigString(_T("sReportPath"),_T("0"));	
	SetName(m_cfgName);
	//////////////////////////////////////////////////////////////////////////
	FILE* fp = NULL;
	SYSTEMTIME st;
	GetLocalTime(&st);
	_wmkdir(strReportPath);

	strReportPath += _T("\\SFR_Course_Data");
	_wmkdir(strReportPath);

	CString strTimeString ;
	strTimeString.Format(_T("%04d%02d%02d"),st.wYear,st.wMonth,st.wDay);
	CString strDataFileName = strReportPath + _T("\\") + _T("FinalTest_Data_") + strTimeString + _T(".csv");

	fp = _wfopen(strDataFileName,_T("a"));
	if (fp == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("打开文档失败，无法保存数据"),COLOR_RED,LOGHEIGHT);
		return FALSE;
	}
	fclose(fp);
	CStdioFile DataFile(strDataFileName, CFile::modeWrite | CFile::typeText);
	if (DataFile.GetLength() == 0)
	{
		CString strHeader = _T("SensorID,Time,PeakDAC,");
		for (int i = 0; i < m_iSFRROICount; i++)
		{
			strHeader += m_stSpec[i].ROIName;
			strHeader += _T(",");
		}
		strHeader += _T("\n");
		DataFile.WriteString(strHeader);
	}
	DataFile.SeekToEnd();
	CString strContent = _T("");
	strContent.Format(_T("%s,%02d:%02d:%02d"),m_pInterface->Sensor_GetCamFuseID(CamID),
		st.wHour,st.wMinute,st.wSecond);
	DataFile.WriteString(strContent);
	for (int i = 0; i < m_Options.m_Mid_TestTime; i++)
	{
		strContent.Format(_T(",%d,"),m_iPeakDAC);	
		CString temp;
		for (int index = 0; index < m_iSFRROICount; index++)
		{
			temp.Format(_T("%f,"),SFRAll[i][index]);
			strContent += temp;
		}
		strContent += _T("\n");
		DataFile.WriteString(strContent);
	}
	DataFile.Close();
	return TRUE;
}
BOOL AF_SFR_TestItem::SaveSFRPosInfo()
{
	m_stAFData[m_iPos].DAC = m_iCode;
	m_stAFData[m_iPos].SFR[0] = m_dSFRValue[0];
	m_stAFData[m_iPos].SFR[1] = m_dSFRValue[1];
	if (m_Options.bEngineerMode)
	{
		for (int i = 2; i < m_iSFRROICount; i++)
		{
			m_stAFData[m_iPos].SFR[i] = m_dSFRValue[i];
		}
	}
	m_iPos++;
	return TRUE;
}
void AF_SFR_TestItem::DesideNextCode()
{
	m_iCode += m_iStep;	

}

float AF_SFR_TestItem::GetSFRValue(unsigned char *_InImageBuffer,ROIInfo roiinfo,int ROISize,wchar_t *stringinfo,int index,int blockIndex,int select_freque)
{
	int Width = img.imgwidth;
	int Height = img.imgheight;
	float sfrvalue = 0.0;
	if(roiinfo.LT.x < 0 || roiinfo.LT.y < 0 || roiinfo.RB.x > Width || roiinfo.RB.y > Height)
	{
		return sfrvalue;
	}
	////截取ROI,存在ROIP10里
	BYTE* ROIP10 = new BYTE[ROISize*ROISize*2];
	if (ROIP10 == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ROIP10内存分配失败"),COLOR_BLUE,200);
		return 0;
	}
	memset(ROIP10,0,ROISize*ROISize*2);
	PCutImageBuffer(_InImageBuffer,Width,Height,2,1,roiinfo.LT.x,roiinfo.LT.y,ROISize,ROISize,ROIP10);	

	// Channel Balance
	BYTE* BlockBuffer = new BYTE[ROISize*ROISize*2];
    if (BlockBuffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("BlockBuffer内存分配失败"),COLOR_BLUE,200);
		SAFE_DELETE_ARRAY(ROIP10);
		return 0;
	}
	memset(BlockBuffer,0,ROISize*ROISize*2);
	int outformat = m_pInterface->Ctrl_GetOutPutFormat(CamID);
	GetBayerType(m_pInterface->Ctrl_GetOutPutFormat(CamID),roiinfo.LT.x,roiinfo.LT.y,outformat);
	RawToGrayscaleImage_Raw10(ROIP10,outformat,ROISize,ROISize,BlockBuffer);
	
	SAFE_DELETE_ARRAY(ROIP10);	
	// Save ROI Images
	if (m_bFinalCheck&&pDlg->m_bSaveImage)
	{  
		char filename[1024]  = {0};
		CString temppath = _T("");
		CString tempname = _T("");
		temppath = GetModulePath();
		temppath += _T("\\\\");
		temppath += _T("SFR_Image");
		CreateDirectory(temppath,NULL);
		temppath += _T("\\\\");
		temppath += m_pInterface->Sensor_GetCamFuseID(CamID);
		tempname.Format(_T("_%s_ROI%d_%s.raw"),GetName(),blockIndex,stringinfo);
		temppath += tempname;

		WideCharToMultiByte(CP_ACP,0,temppath,(temppath.GetLength()+1), filename, 1024, NULL, NULL);
		//		sprintf(filename,"%s",temppath.GetBuffer(temppath.GetLength()));
		bSaveRawFile(filename,BlockBuffer,ROISize*ROISize*2);
	}
	USHORT *pROIBuffer = new USHORT[ROISize*ROISize];
    if (pROIBuffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("pROIBuffer内存分配失败"),COLOR_BLUE,200);
		SAFE_DELETE_ARRAY(BlockBuffer);
		return 0;
	}
	memset(pROIBuffer,0,ROISize*ROISize*sizeof(USHORT));
	for (int y=0; y < ROISize;y++)
	{
		for (int x=0; x<ROISize;x++)
		{
			pROIBuffer[y*ROISize+x] = BlockBuffer[(y*ROISize+x)*2] | (BlockBuffer[(y*ROISize+x)*2 + 1] << 8);
		}
	}
	SAFE_DELETE_ARRAY(BlockBuffer);

	int grayLT = pROIBuffer[0];
	int grayRT = pROIBuffer[ROISize - 1];
	int grayLB = pROIBuffer[ROISize * (ROISize - 1)];
	int grayRB = pROIBuffer[ROISize * ROISize - 1];
	if ((abs(grayLT - grayRB) < 100) || (abs(grayRT - grayLB) < 100))
	{
		CString s = _T("");
		s.Format(_T("ROI image error.%d %d %d %d"), grayLT, grayRT, grayLB, grayRB);
		m_pInterface->Ctrl_Addlog(CamID, s, COLOR_BLUE, 200);
	    SAFE_DELETE_ARRAY(pROIBuffer);
		sfrvalue = 0.0f;
		return sfrvalue;
	}

	CSFRData* sfrArray = new CSFRData[ROISize];
	if (sfrArray == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("sfrArray内存分配失败"),COLOR_BLUE,200);
		SAFE_DELETE_ARRAY(pROIBuffer);
		return 0;
	}
	//if(pDlg->m_bTestCenter)
	
	double freq = 1.0f / (select_freque * 2);
	
	sfrvalue = (float)SFRMatbmpTest(pROIBuffer,freq,sfrArray,ROISize,ROISize);
	if(sfrvalue>1) {sfrvalue=1;}
	if(sfrvalue<0.007) {sfrvalue=0.01f;}		
	if(_isnan(sfrvalue))  {sfrvalue = 1000;}

	SAFE_DELETE_ARRAY(sfrArray);
	SAFE_DELETE_ARRAY(pROIBuffer);
	//计算每个BlockBuffer的SFRValue
	/*
	OutputDebugString(_T("++++++++++Start GetSFR_ofilm+++++++"));			

		_sfrresult sfrresult;
	memset(&sfrresult,0,sizeof(_sfrresult));
	int SFRerr = GetSFR_ofilm(sfrvalue, &sfrresult, 4.0 , BlockBuffer,bayer10, ROISize, ROISize);

	if(sfrvalue>1) {sfrvalue=1;}
	if(sfrvalue<0.007) {sfrvalue=0.01;}		

	SAFE_DELETE_ARRAY(BlockBuffer);

	if(SFRerr){
	CString tempStr;
	tempStr.Format(_T("%s"),sfrresult.errtip);
	m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_RED,220);
	sfrvalue = 1000;
	}
	*/			

	return sfrvalue;
}

bool AF_SFR_TestItem::PCutImageBuffer(unsigned char *_InImageBuffer,int _Width,int _Height,UINT _ByteDepth,UINT _ColorChannel, int _mStartX,int _mStartY,int _CutWidth,int _CutHeight,unsigned char *_OutImageBuffer)
{		
	bool bRes = true;

	if (((_CutWidth>_Width)||(_CutHeight>_Height))||((_CutWidth<=0)||(_CutHeight<=0)))
	{
		bRes = false;
		return bRes;
	}
	if ((_Width == _CutWidth)&&(_Height == _CutHeight))
	{
		memcpy(_OutImageBuffer,_InImageBuffer,sizeof(unsigned char)*_Width*_Height*_ByteDepth*_ColorChannel);
		return bRes;
	}
	//此算法针对原图为各个channel交错排列的情况，即RGBRGBRGB.....			
	for (int i=0;i<_CutHeight;i++)
	{
		memcpy( _OutImageBuffer+i*_CutWidth*_ByteDepth*_ColorChannel , 
			_InImageBuffer+((_mStartY+i)*_Width+_mStartX)*_ByteDepth*_ColorChannel,	
			_CutWidth*_ByteDepth*_ColorChannel*sizeof(unsigned char));
	}	

	return bRes;
}

void AF_SFR_TestItem::GetBayerType(int iImageType,int iStartX,int iStartY,int& outformat)
{	
	outformat=m_pInterface->Ctrl_GetOutPutFormat(CamID);		

	//RAW Buffer RGB排列
	if ((iStartX%2==0) &&(iStartY%2==1))
	{		
		switch(outformat)
		{
		case 	OUTFORMAT_RGGB:
			outformat = OUTFORMAT_GBRG;
			break;
		case 	OUTFORMAT_GRBG:
			outformat = OUTFORMAT_BGGR;
			break;
		case 	OUTFORMAT_GBRG:
			outformat = OUTFORMAT_RGGB;
			break;
		case 	OUTFORMAT_BGGR:
			outformat = OUTFORMAT_GRBG;
			break;
		}
	}else if ((iStartX%2 == 1) && (iStartY%2 == 0))
	{
		switch(outformat)
		{
		case 	OUTFORMAT_RGGB:
			outformat = OUTFORMAT_GRBG;
			break;
		case 	OUTFORMAT_GRBG:
			outformat = OUTFORMAT_RGGB;
			break;
		case 	OUTFORMAT_GBRG:
			outformat = OUTFORMAT_BGGR;
			break;
		case 	OUTFORMAT_BGGR:
			outformat = OUTFORMAT_GBRG;
			break;
		}
	}else if ((iStartX%2==1) && (iStartY%2==1))
	{
		switch(outformat)
		{
		case 	OUTFORMAT_RGGB:
			outformat = OUTFORMAT_BGGR;
			break;
		case 	OUTFORMAT_GRBG:
			outformat = OUTFORMAT_GBRG;
			break;
		case 	OUTFORMAT_GBRG:
			outformat = OUTFORMAT_GRBG;
			break;
		case 	OUTFORMAT_BGGR:
			outformat = OUTFORMAT_RGGB;
			break;
		}
	}
}

void AF_SFR_TestItem::RawToGrayscaleImage_Raw10(unsigned char *m_pBuff,int outformat,int m_width,int m_height,unsigned char *pOutRaw10)
{
	USHORT* m_pBuff2 = new USHORT[m_width*m_height];
	USHORT* pOut     = new USHORT[m_width*m_height];
    if (m_pBuff2 == NULL || pOut == NULL)
	{
		SAFE_DELETE_ARRAY(m_pBuff2);
		SAFE_DELETE_ARRAY(pOut);
	}
	memset(m_pBuff2,0,sizeof(USHORT)*m_width*m_height);//memset(BMPBuffer,0,sizeof(BYTE)*img.imgwidth*img.imgheight*3);//初始化
	memset(pOut,0,sizeof(USHORT)*m_width*m_height);
	int i =0;
	for( i= 0; i < m_height; i++ )
	{
		for( int j = 0; j < m_width; j++ )
		{
			m_pBuff2[i*m_width+j] = (m_pBuff[(i*m_width+j)*2+1]<<8)+m_pBuff[(i*m_width+j)*2];
		}
	}

	int  PixW   = m_width;
	int  PixH   = m_height;

	int  StartX = 0;
	int  StartY = 0;

	int  EndX   = PixW;
	int  EndY   = PixH;

	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1  = 0, Pos2 = 0;

	long  SumGr = 0;
	double	Gr = 0.0;

	long  SumGb = 0;
	double Gb = 0.0;

	long  SumR  = 0;
	double R  = 0.0;

	long  SumB  = 0;
	double B  = 0.0;

	//计算Gr Avg
	if ( outformat== OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += m_pBuff2[Pos1 + 1];
				Count++;
			}
		}
	}
	if (outformat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += m_pBuff2[Pos1];
				Count++;
			}
		}
	}

	if (outformat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += m_pBuff2[Pos2 + 1];
				Count++;
			}
		}
	}

	if (outformat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += m_pBuff2[Pos2];
				Count++;
			}
		}
	}
	Gr = SumGr/Count;	
	SumGr =0;
	Count =0;

	//------------------------------------------
	if ( outformat== OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				if (m_pBuff2[Pos1 + 1] >= Gr)
				{
					SumR  += m_pBuff2[Pos1];
					SumGr += m_pBuff2[Pos1 + 1];
					SumGb += m_pBuff2[Pos2];
					SumB  += m_pBuff2[Pos2 + 1];
					Count++;
				}
			}
		}
	}
	if (outformat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				if (m_pBuff2[Pos1] >= Gr)
				{
					SumGr += m_pBuff2[Pos1];
					SumR  += m_pBuff2[Pos1 + 1];
					SumB  += m_pBuff2[Pos2];
					SumGb += m_pBuff2[Pos2 + 1]; 

					Count++;
				}
			}
		}
	}

	if (outformat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				if (m_pBuff2[Pos2+1] >= Gr)
				{
					SumGb += m_pBuff2[Pos1];
					SumB  += m_pBuff2[Pos1 + 1];
					SumR  += m_pBuff2[Pos2];
					SumGr += m_pBuff2[Pos2 + 1];

					Count++;
				}
			}
		}
	}

	if (outformat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				if (m_pBuff2[Pos2] >= Gr)
				{
					SumB  += m_pBuff2[Pos1];
					SumGb += m_pBuff2[Pos1 + 1];
					SumGr += m_pBuff2[Pos2];
					SumR  += m_pBuff2[Pos2 + 1];

					Count++;
				}
			}
		}
	}

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;

	//double G_Ave = (0.5*(Gr+Gb));

	double Gr_R = (Gr / R);
	double Gr_Gb = (Gr/Gb);
	double Gr_B = (Gr/B);

	if ( outformat== OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				pOut[Pos1]    = ((int)(m_pBuff2[Pos1] * Gr_R)>1023?1023:(int)(m_pBuff2[Pos1] * Gr_R));
				pOut[Pos1 + 1]= m_pBuff2[Pos1 + 1];
				pOut[Pos2]    = ((int)(m_pBuff2[Pos2] * Gr_Gb)>1023?1023:(int)(m_pBuff2[Pos2] * Gr_Gb));
				pOut[Pos2 + 1]= ((int)(m_pBuff2[Pos2 + 1] * Gr_B)>1023?1023:(int)(m_pBuff2[Pos2 + 1] * Gr_B));			
			}
		}
	}
	if (outformat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				pOut[Pos1]    = m_pBuff2[Pos1];
				pOut[Pos1 + 1]=((int)(m_pBuff2[Pos1 + 1] * Gr_R)>1023?1023:(int)(m_pBuff2[Pos1 + 1] * Gr_R));
				pOut[Pos2]    =((int)(m_pBuff2[Pos2] * Gr_B)>1023?1023:(int)(m_pBuff2[Pos2] * Gr_B));
				pOut[Pos2 + 1]=((int)(m_pBuff2[Pos2 + 1] * Gr_Gb)>1023?1023:(int)(m_pBuff2[Pos2 + 1] * Gr_Gb));

			}
		}
	}

	if (outformat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				pOut[Pos1]    = ((int)(m_pBuff2[Pos1]*Gr_Gb)>1023?1023:(int)(m_pBuff2[Pos1]*Gr_Gb));
				pOut[Pos1 + 1]= ((int)(m_pBuff2[Pos1 + 1]*Gr_B)>1023?1023:(int)(m_pBuff2[Pos1 + 1]*Gr_B));
				pOut[Pos2]    = ((int)(m_pBuff2[Pos2]*Gr_R)>1023?1023:(int)(m_pBuff2[Pos2]*Gr_R));
				pOut[Pos2 + 1]= m_pBuff2[Pos2 + 1];
			}
		}
	}

	if (outformat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				pOut[Pos1]    =((int)( m_pBuff2[Pos1] * Gr_B)>1023?1023:(int)(m_pBuff2[Pos1] * Gr_B));
				pOut[Pos1 + 1]=((int)( m_pBuff2[Pos1 + 1] * Gr_Gb)>1023?1023:(int)(m_pBuff2[Pos1 + 1] * Gr_Gb));
				pOut[Pos2]    = m_pBuff2[Pos2] ;
				pOut[Pos2 + 1]=((int)( m_pBuff2[Pos2 + 1] * Gr_R)>1023?1023:(int)(m_pBuff2[Pos2 + 1] * Gr_R));
			}
		}
	}

	USHORT nCount = 0;
	for( i = 0; i < m_height; i++ )
	{
		for( int j = 0; j < m_width; j++ )
		{
			pOutRaw10[nCount++] = (pOut[i*m_width+j])&0xff;
			pOutRaw10[nCount++] = (pOut[i*m_width+j]>>8)&0x03;
		}
	}
	delete []m_pBuff2;
    m_pBuff2 = NULL;
	delete []pOut;
    pOut = NULL;
}

BOOL AF_SFR_TestItem::bSaveBmpFile(CString sfilename, BYTE *pBuffer, UINT width, UINT height)
{
	int				 OffBits;
	HFILE			 bmpFile;
	BITMAPFILEHEADER bmpHeader;  // Header for Bitmap file
	BITMAPINFO		 bmpInfo;

	OffBits           = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	DWORD dwSizeImage = width * height * 3;  //IMAGESIZE_X*IMAGESIZE_Y*3;

	// VertFlipBuf(pBuffer, width * 3, height); //to correct the vertical flip problem.

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

	char temp[512] = {0};
	WideCharToMultiByte(CP_ACP,0,sfilename,(sfilename.GetLength()+1), temp, 512, NULL, NULL);
	bmpFile = _lcreat((LPCSTR)temp, FALSE); 
	if (bmpFile < 0) 
	{
		return FALSE;
	}

	UINT len = 0;
	len = _lwrite(bmpFile, (LPSTR)&bmpHeader, sizeof(BITMAPFILEHEADER));

	len = _lwrite(bmpFile, (LPSTR)&bmpInfo,   sizeof(BITMAPINFOHEADER));
	len = _lwrite(bmpFile, (LPSTR)pBuffer,    bmpHeader.bfSize - sizeof(bmpHeader) - sizeof(bmpInfo) + 4);  //+4 is for exact filesize
	_lclose(bmpFile);

	return TRUE;
}


BOOL AF_SFR_TestItem::AnalysisROISpec()
{			
	char temp[MAXSTRLEN] = {0};
	int index = 0;

	if ((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF5+pDlg->m_bTestF6+pDlg->m_bTestF8) == 0)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("没有选择要测试的ROI"),COLOR_RED,220);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("没有选择要测试的ROI"));
		m_bROIConfigureError = true;
		return FALSE;
	}

	if (pDlg->m_bTestCenter)
	{
		wcscpy_s(m_stSpec[index].ROIName,512,_T("CEN_H"));
		m_stSpec[index].SFRSpec = m_Options.m_CenH_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("CEN_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_CenH_Spec;
		index++;
	}

	if (pDlg->m_bTestF3)
	{
		//左上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL3_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F3H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL3_V"));
		m_stSpec[index].SFRSpec =   m_Options.m_F3V_Spec;
		index++;
		//右上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR3_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F3H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR3_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F3V_Spec;
		index++;
		//右下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR3_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F3H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR3_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F3V_Spec;
		index++;
		//左下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL3_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F3H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL3_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F3V_Spec;
		index++;
	}
	
	if (pDlg->m_bTestF4)
	{
		//左上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL4_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F4H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL4_V"));
		m_stSpec[index].SFRSpec =   m_Options.m_F4V_Spec;
		index++;
		//右上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR4_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F4H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR4_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F4V_Spec;
		index++;
		//右下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR4_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F4H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR4_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F4V_Spec;
		index++;
		//左下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL4_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F4H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL4_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F4V_Spec;
		index++;
	}

	if (pDlg->m_bTestF5)
	{
		//左上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL5_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F5H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL5_V"));
		m_stSpec[index].SFRSpec =   m_Options.m_F5V_Spec;
		index++;
		//右上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR5_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F5H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR5_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F5V_Spec;
		index++;
		//右下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR5_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F5H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR5_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F5V_Spec;
		index++;
		//左下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL5_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F5H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL5_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F5V_Spec;
		index++;
	}

	if (pDlg->m_bTestF6)
	{
		//左上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL6_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F6H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL6_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F6V_Spec;
		index++;
		//右上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR6_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F6H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR6_V"));
		m_stSpec[index].SFRSpec =   m_Options.m_F6V_Spec;
		index++;
		//右下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR6_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F6H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR6_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F6V_Spec;
		index++;
		//左下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL6_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F6H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL6_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F6V_Spec;
		index++;
	}

	if (pDlg->m_bTestF8)
	{
		//左上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL8_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F8H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TL8_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F8V_Spec;
		index++;
		//右上
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR8_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F8H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("TR8_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F8V_Spec;
		index++;
		//右下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR8_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F8H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BR8_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F8V_Spec;
		index++;
		//左下
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL8_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F8H_Spec;
		index++;
		wcscpy_s(m_stSpec[index].ROIName,512,_T("BL8_V"));
		m_stSpec[index].SFRSpec =   m_Options.m_F8V_Spec;
		index++;
	}

	m_iSFRROICount = index;

	return TRUE;
}

void AF_SFR_TestItem::StringSplit( const wstring & str, vector<wstring> & tokens, const wstring & delimiters)  
{  
	// Skip delimiters at beginning. 
	wstring::size_type lastPos = str.find_first_not_of(delimiters, 0);  
	// Find first "non-delimiter".  
	wstring::size_type pos = str.find_first_of(delimiters, lastPos);  
	while (wstring::npos != pos || wstring::npos != lastPos)  
	{  
		// Found a token, add it to the vector.  
		tokens.push_back(str.substr(lastPos, pos - lastPos));  
		// Skip delimiters.   
		lastPos = str.find_first_not_of(delimiters, pos);  
		// Find next "non-delimiter"  
		pos = str.find_first_of(delimiters, lastPos);  
	}   
}  

BOOL AF_SFR_TestItem::FindPeak(int& iPeakDAC,int StartIndex,int step)
{
	char templog[512] = {0};
	if ((step == m_Options.m_iStep3)||(pDlg->m_AFTestMode))
	{
		double code[512];
		double sfrvalue[512];
		for (int i=0; i<512; i++)
		{
			code[i] = 0.0f;
			sfrvalue[i] = 0.0f;
		}

		int nCount = 0;
		if (m_Options.m_iAFMethod == 0 || m_iPeroid == 2)
		{
			for(int index = StartIndex; index < m_iPos; index++)
			{
				double SFRTemp = (m_stAFData[index].SFR[0] + m_stAFData[index].SFR[1]);
				code[nCount] = m_stAFData[index].DAC;
				sfrvalue[nCount] = SFRTemp;
				nCount++;
			}

			double maxCode = GetPeakCodeByAfValue(sfrvalue, code);
#if 0
			double x0,x1,x2,maxCode;
			maxCode = fnCurveFit2(code, sfrvalue, &x0, &x1, &x2, nCount); 
#endif
			//DebugInfo("最小二乘法拟合：MaxCode = %d",maxCode);
			//DebugInfo("y = %.7fx^2+%.7fx+%.3f",x2,x1,x0);
			//DebugInfo("Min Code = %d，Max Code = %d",m_stAFData[StartIndex].DAC, m_stAFData[StartIndex+nCount-1].DAC);
			if ((maxCode>m_stAFData[StartIndex].DAC)&&(maxCode<m_stAFData[StartIndex+nCount-1].DAC))
			{
				iPeakDAC = int(maxCode+0.5);
				//DebugInfo("最小二乘法拟合 ok, iPeakDAC = %d",iPeakDAC);
				return TRUE;
			}
		}else if (m_Options.m_iAFMethod == 1)
		{	//Contrast
			for (int index = StartIndex; index < m_iPos; index++)
			{
				code[nCount] = m_stContrastData[index].DAC;
				sfrvalue[nCount] = (double)m_stContrastData[index].iContrastValue;
				nCount++;
			}

			double maxCode = GetPeakCodeByAfValue(sfrvalue, code);
#if 0
			double x0,x1,x2,maxCode;
			maxCode = fnCurveFit2(code, sfrvalue, &x0, &x1, &x2, nCount); 
#endif
			//DebugInfo("最小二乘法拟合：MaxCode = %d",maxCode);
			//DebugInfo("y = %.7fx^2+%.7fx+%.3f",x2,x1,x0);
			//DebugInfo("Min Code = %d，Max Code = %d",m_stAFData[StartIndex].DAC, m_stAFData[StartIndex+nCount-1].DAC);
			if ((maxCode>m_stContrastData[StartIndex].DAC)&&(maxCode<m_stContrastData[StartIndex+nCount-1].DAC))
			{
				iPeakDAC = int(maxCode+0.5);
				//DebugInfo("最小二乘法拟合 ok, iPeakDAC = %d",iPeakDAC);
				return TRUE;
			}
		}else 
		{	//mtf
			for (int index = StartIndex; index < m_iPos; index++)
			{
				code[nCount] = m_stMTFData[index].DAC;
				sfrvalue[nCount] = m_stMTFData[index].dMTF;
				nCount++;
			}

			double maxCode = GetPeakCodeByAfValue(sfrvalue, code);
#if 0
			double x0,x1,x2,maxCode;
			maxCode = fnCurveFit2(code, sfrvalue, &x0, &x1, &x2, nCount); 
#endif
			//DebugInfo("最小二乘法拟合：MaxCode = %d",maxCode);
			//DebugInfo("y = %.7fx^2+%.7fx+%.3f",x2,x1,x0);
			//DebugInfo("Min Code = %d，Max Code = %d",m_stAFData[StartIndex].DAC, m_stAFData[StartIndex+nCount-1].DAC);
			if ((maxCode>m_stMTFData[StartIndex].DAC)&&(maxCode<m_stMTFData[StartIndex+nCount-1].DAC))
			{
				iPeakDAC = int(maxCode+0.5);
				//DebugInfo("最小二乘法拟合 ok, iPeakDAC = %d",iPeakDAC);
				return TRUE;
			}
		}
	}


	if (m_Options.m_iAFMethod == 0 || m_iPeroid == 2) // SFR AF
	{
		iPeakDAC = m_stAFData[StartIndex].DAC;
		float fMaxSFR1 = (float) (m_stAFData[StartIndex].SFR[0] + m_stAFData[StartIndex].SFR[1]);
		float fMaxSFR2 = fMaxSFR1;
		float fMaxSFR3 = fMaxSFR1;
		int DAC1=m_stAFData[StartIndex].DAC;
		int DAC2=DAC1;
		int DAC3 = DAC1;
#if 0
		//找最大的前3个数据
		for(int index = StartIndex; index < m_iPos; index++)
		{
			float SFRTemp = (float)(m_stAFData[index].SFR[0] + m_stAFData[index].SFR[1]);
			if(SFRTemp>fMaxSFR1)  { 					
				DAC3=DAC2;
				fMaxSFR3=fMaxSFR2;
				DAC2=DAC1;
				fMaxSFR2=fMaxSFR1;
				fMaxSFR1=SFRTemp;
				DAC1=m_stAFData[index].DAC;
			}else if (SFRTemp>fMaxSFR2)  { 
				DAC3=DAC2;
				fMaxSFR3=fMaxSFR2;
				fMaxSFR2=SFRTemp;
				DAC2=m_stAFData[index].DAC;					
			}else if (SFRTemp>fMaxSFR3)  { 
				fMaxSFR3=SFRTemp;
				DAC3=m_stAFData[index].DAC;
			}
		}

		iPeakDAC = int((fMaxSFR3*DAC3 + fMaxSFR2*DAC2 + fMaxSFR1*DAC1)/(fMaxSFR1+fMaxSFR2+fMaxSFR3)+0.5);
#else
		fMaxSFR1 = 0;
		int peakCnt = 0;
		for(int index = StartIndex; index < m_iPos; index++)
		{
			if (fMaxSFR1 < (m_stAFData[index].SFR[0] + m_stAFData[index].SFR[1]))
			{
				fMaxSFR1 = (m_stAFData[index].SFR[0] + m_stAFData[index].SFR[1]);
				iPeakDAC = m_stAFData[index].DAC;
				peakCnt = index;
			}
		}

		if (iPeakDAC == m_stAFData[StartIndex].DAC)
			iPeakDAC = m_stAFData[StartIndex].DAC;
		else if (iPeakDAC == m_stAFData[m_iPos-1].DAC)
			iPeakDAC = m_stAFData[m_iPos-1].DAC;
		else
		{
			float y1 = m_stAFData[peakCnt - 1].SFR[0] + m_stAFData[peakCnt - 1].SFR[1], y2 = m_stAFData[peakCnt].SFR[0] + m_stAFData[peakCnt].SFR[1], y3 = m_stAFData[peakCnt + 1].SFR[0] + m_stAFData[peakCnt + 1].SFR[1];
            int x1 = m_stAFData[peakCnt - 1].DAC, x2 = m_stAFData[peakCnt].DAC, x3 = m_stAFData[peakCnt + 1].DAC;

            float a = ((y1 - y2) * (x2 - x3) - (y2 - y3) * (x1 - x2)) / ((x1 * x1 - x2 * x2) * (x2 - x3) - (x2 * x2 - x3 * x3) * (x1 - x2));

            iPeakDAC = (int)((((y1 - y2) * (x2 - x3) - a * (x1 * x1 - x2 * x2) * (x2 - x3)) / ((x1 - x2) * (x2 - x3))) * (-1.0f) / (2 * a));

		}
			
#endif
		//DebugInfo("DAC1=%d,DAC2=%d,DAC3=%d",DAC1,DAC2,DAC3);
		//DebugInfo("PeakDAC=%d",iPeakDAC);
	}
	else if (m_Options.m_iAFMethod == 1) //Contrast AF
	{
		iPeakDAC = m_stContrastData[StartIndex].DAC;
		ULONG64 fMaxSFR1 = m_stContrastData[StartIndex].iContrastValue;
		ULONG64 fMaxSFR2 = fMaxSFR1;
		ULONG64 fMaxSFR3 = fMaxSFR1;
		int DAC1=m_stContrastData[StartIndex].DAC;
		int DAC2=DAC1;
		int DAC3 = DAC1;
#if 0
		//找最大的前3个数据
		for(int index = StartIndex; index < m_iPos; index++)
		{
			ULONG64 SFRTemp = m_stContrastData[index].iContrastValue;
			if(SFRTemp>fMaxSFR1)  { 					
				DAC3=DAC2;
				fMaxSFR3=fMaxSFR2;
				DAC2=DAC1;
				fMaxSFR2=fMaxSFR1;
				fMaxSFR1=SFRTemp;
				DAC1=m_stContrastData[index].DAC;
			}else if (SFRTemp>fMaxSFR2)  { 
				DAC3=DAC2;
				fMaxSFR3=fMaxSFR2;
				fMaxSFR2=SFRTemp;
				DAC2=m_stContrastData[index].DAC;					
			}else if (SFRTemp>fMaxSFR3)  { 
				fMaxSFR3=SFRTemp;
				DAC3=m_stContrastData[index].DAC;
			}
		}

		iPeakDAC = int((fMaxSFR3*DAC3 + fMaxSFR2*DAC2 + fMaxSFR1*DAC1)/(fMaxSFR1+fMaxSFR2+fMaxSFR3)+0.5);
#else
		fMaxSFR1 = 0;
		int peakCnt = 0;
		for(int index = StartIndex; index < m_iPos; index++)
		{
			if (fMaxSFR1 < m_stContrastData[index].iContrastValue)
			{
				fMaxSFR1 = m_stContrastData[index].iContrastValue;
				iPeakDAC = m_stContrastData[index].DAC;
				peakCnt = index;
			}
		}

		if (iPeakDAC == m_stContrastData[StartIndex].DAC)
			iPeakDAC = m_stContrastData[StartIndex].DAC;
		else if (iPeakDAC == m_stContrastData[m_iPos-1].DAC)
			iPeakDAC = m_stContrastData[m_iPos-1].DAC;
		else
		{
			float y1 = m_stContrastData[peakCnt - 1].iContrastValue, y2 = m_stContrastData[peakCnt].iContrastValue, y3 = m_stContrastData[peakCnt + 1].iContrastValue;
            int x1 = m_stContrastData[peakCnt - 1].DAC, x2 = m_stContrastData[peakCnt].DAC, x3 = m_stContrastData[peakCnt + 1].DAC;

            float a = ((y1 - y2) * (x2 - x3) - (y2 - y3) * (x1 - x2)) / ((x1 * x1 - x2 * x2) * (x2 - x3) - (x2 * x2 - x3 * x3) * (x1 - x2));

            iPeakDAC = (int)((((y1 - y2) * (x2 - x3) - a * (x1 * x1 - x2 * x2) * (x2 - x3)) / ((x1 - x2) * (x2 - x3))) * (-1.0f) / (2 * a));
		}
#endif
		//DebugInfo("DAC1=%d,DAC2=%d,DAC3=%d",DAC1,DAC2,DAC3);
		//DebugInfo("PeakDAC=%d",iPeakDAC);
	}else
	{
		iPeakDAC = m_stMTFData[StartIndex].DAC;
		double fMaxSFR1 = m_stMTFData[StartIndex].dMTF;
		double fMaxSFR2 = fMaxSFR1;
		double fMaxSFR3 = fMaxSFR1;
		int DAC1=m_stMTFData[StartIndex].DAC;
		int DAC2=DAC1;
		int DAC3 = DAC1;
#if 0
		//找最大的前3个数据
		for(int index = StartIndex; index < m_iPos; index++)
		{
			double SFRTemp = m_stMTFData[index].dMTF;
			if(SFRTemp>fMaxSFR1)  { 					
				DAC3=DAC2;
				fMaxSFR3=fMaxSFR2;
				DAC2=DAC1;
				fMaxSFR2=fMaxSFR1;
				fMaxSFR1=SFRTemp;
				DAC1=m_stMTFData[index].DAC;
			}else if (SFRTemp>fMaxSFR2)  { 
				DAC3=DAC2;
				fMaxSFR3=fMaxSFR2;
				fMaxSFR2=SFRTemp;
				DAC2=m_stMTFData[index].DAC;					
			}else if (SFRTemp>fMaxSFR3)  { 
				fMaxSFR3=SFRTemp;
				DAC3=m_stMTFData[index].DAC;
			}
		}

		iPeakDAC = int((fMaxSFR3*DAC3 + fMaxSFR2*DAC2 + fMaxSFR1*DAC1)/(fMaxSFR1+fMaxSFR2+fMaxSFR3)+0.5);
#else
		fMaxSFR1 = 0.0f;		
		for(int index = StartIndex; index < m_iPos; index++)
		{
			if (fMaxSFR1 < m_stMTFData[index].dMTF)
			{
				fMaxSFR1 = m_stMTFData[index].dMTF;
				iPeakDAC = m_stMTFData[index].DAC;
			}
		}
#endif
		//DebugInfo("DAC1=%d,DAC2=%d,DAC3=%d",DAC1,DAC2,DAC3);
		//DebugInfo("PeakDAC=%d",iPeakDAC);
	}



	CString temp;
	temp.Format(_T("Peak Code = %d"),iPeakDAC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,LOGHEIGHT);

	if (iPeakDAC < m_stAFData[StartIndex].DAC || iPeakDAC > m_stAFData[m_iPos-1].DAC)
	{
		return FALSE;
	}else
	{
		return TRUE;
	}	
}

BOOL AF_SFR_TestItem::AutoROI(OFAutoROIOut &ofDestDataOut)
{	
	//使用BMP图来mark点和ROI
	BYTE* BMPBuffer=new BYTE[img.imgwidth*img.imgheight*3];
    if (BMPBuffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("内存分配失败"),COLOR_BLUE,200);
		return 0;
	}
    memset(BMPBuffer,0,sizeof(BYTE)*img.imgwidth*img.imgheight*3);//初始化
	memcpy(BMPBuffer,img.RGB24_buffer,img.imgwidth*img.imgheight*3);//	
	OFAutoROIIn ofSrcDataIn;
	ofSrcDataIn.ofSrcBuffer=BMPBuffer;
	ofSrcDataIn.ofHeight = img.imgheight;
	ofSrcDataIn.ofWidth = img.imgwidth;
	ofSrcDataIn.ofImageType = BMP_IMAGE;
	ofSrcDataIn.ofOutFormat  = OUTFORMAT_FLIP_;
    ofSrcDataIn.CamID = CamID;
	char ErrorMsg[512]={0};
	BOOL bAutoROI = FALSE;
	//bSaveBmpFile(_T("D:\\AutoROI.bmp"),BMPBuffer,img.imgwidth,img.imgheight);
	bFlagCenterROI = -1;
	if (m_bFocusing)
	{
		if (m_Options.bEngineerMode)
		{
			bAutoROI = fnAutoROISelect(&ofSrcDataIn, &ofDestDataOut,ErrorMsg);
		}
		else
		{
			bAutoROI = fnAutoROICenter(&ofSrcDataIn, &ofDestDataOut,ErrorMsg);
			bFlagCenterROI = TRUE;
		}
	}else
	{
		bAutoROI = fnAutoROISelect(&ofSrcDataIn, &ofDestDataOut,ErrorMsg);
		bFlagCenterROI = FALSE;
	}

	
	if (!bAutoROI) 
	{
		CString temp;
		temp.Format(_T("%s"),CStringW(ErrorMsg));
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_RED,LOGHEIGHT);
	}
	SAFE_DELETE_ARRAY(BMPBuffer);
	return bAutoROI;
}
BOOL AF_SFR_TestItem::SFRTest(OFAutoROIOut ofDestDataOut,OfMarker* _ROIInfo,int &iROICount)
{	
	//**************************计算SFR********************************//
	int iROINum = ofDestDataOut.ofROInumber;
	int select_freque = 0;
	
	//DebugInfo("------------------ROINum=%d------------------",iROINum);
	BYTE* P10Buffer = new BYTE[img.imgwidth*img.imgheight*2];	
    if (P10Buffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("P10Buffer内存分配失败"),COLOR_BLUE,200);
		return 0;
	}
	memset(P10Buffer,0,img.imgwidth*img.imgheight*2);	
#if 0
	BYTE *pRaw8Buf = new BYTE[img.imgwidth*img.imgheight*2];
	if (NULL == pRaw8Buf)
	{
		CString Msg;
		Msg.Format(_T("%s malloc memory error!"),GetName());
		m_pInterface->Ctrl_Addlog(CamID,Msg, COLOR_RED,500);	
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,Msg);
		SetResult(RESULT_FAIL);
		return 0;
	}
	ImageProc::RawToRaw8(img.Raw_buffer,pRaw8Buf, img.imgwidth, img.imgheight, img.RawBits);

	memcpy(P10Buffer,img.Raw_buffer,img.imgwidth*img.imgheight*2);
	memcpy(P10Buffer,pRaw8Buf,img.imgwidth*img.imgheight*2);
	SAFE_DELETE_ARRAY(pRaw8Buf);
#endif		
    if (pDlg->m_BLC > 0)
	{
		USHORT* Raw10_buffer =  new USHORT[img.imgwidth*img.imgheight];
		if  (Raw10_buffer)
		{
			for ( int i=0;i<img.imgwidth*img.imgheight;i++ )
			{
				Raw10_buffer[i] = (img.Raw_buffer[i] < (USHORT)pDlg->m_BLC)?0:(img.Raw_buffer[i] - pDlg->m_BLC);
			}
			memcpy(P10Buffer,Raw10_buffer,img.imgwidth*img.imgheight*2);
			SAFE_DELETE_ARRAY(Raw10_buffer);
		}
		else
		{
			memcpy(P10Buffer,img.Raw_buffer,img.imgwidth*img.imgheight*2);
		}
	}
	else
	{
		memcpy(P10Buffer,img.Raw_buffer,img.imgwidth*img.imgheight*2);
	}

	if (m_bFinalCheck&&pDlg->m_bSaveImage)
	{  
		char filename[1024]  = {0};
		CString temppath = _T("");
		CString tempname = _T("");
		temppath = GetModulePath();
		temppath += _T("\\\\");
		temppath += _T("SFR_Image");
		CreateDirectory(temppath,NULL);
		temppath += _T("\\\\");
		tempname.Format(_T("%s_%d.raw"),GetName(),(iTestTimes+1));
		temppath += m_pInterface->Sensor_GetCamFuseID(CamID);
		temppath += tempname;

		WideCharToMultiByte(CP_ACP,0,temppath,(temppath.GetLength()+1), filename, 1024, NULL, NULL);
		//		sprintf(filename,"%s",temppath.GetBuffer(temppath.GetLength()));
		bSaveRawFile(filename,P10Buffer,img.imgwidth*img.imgheight*2);
	}

	float sfrvalue1,sfrvalue2,sfrvalue3,sfrvalue4;
	double _sfrA,_sfrB,_sfrC,_sfrD,_sfrX1,_sfrX2,_sfrY1,_sfrY2;
	int nCount = 0;
	int blockCount = 0;
	CString logInfo;
	for (int i = 0; i < iROINum; i++)
	{
		if((i == 0) && (pDlg->m_bTestCenter))
		{
			select_freque = pDlg->m_sfrFreq;//;//中心视场频率1/4,
		}
		else
		{
			select_freque = pDlg->m_sfrOther_Freq;//
		}

		if ((ofDestDataOut.ofROIposition[i]._y1+ofDestDataOut.ofROIposition[i]._y2) == 0)
		{
			_sfrC = ofDestDataOut.ofROIposition[i]._C;
			_sfrD = ofDestDataOut.ofROIposition[i]._D;

			if (_sfrC+_sfrD)
			{
				nCount++;

				sfrvalue1 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH1,ofDestDataOut.ofROISize,L"H1",nCount,i,select_freque);
				sfrvalue2 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH2,ofDestDataOut.ofROISize,L"H2",nCount,i,select_freque);

				if (m_bFinalCheck&&pDlg->m_bSaveImage)
				{
					logInfo.Format(_T("[ROI%d] : H1=%.6f  H2=%.6f  _C=%.6f  _D=%.6f"),i,sfrvalue1,sfrvalue2,
						_sfrC,_sfrD);
					m_pInterface->Ctrl_Addlog(CamID,logInfo,COLOR_BLUE,200);
				}
				
				_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIH1.LT.x;
				_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIH1.LT.y;
				_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
				_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
				_ROIInfo[blockCount].Color = SFRHCOLOR;
				blockCount++;

				_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIH2.LT.x;
				_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIH2.LT.y;
				_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
				_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
				_ROIInfo[blockCount].Color =SFRHCOLOR;
				blockCount++;

				if ((sfrvalue1+sfrvalue2)>999)
				{
					SAFE_DELETE_ARRAY(P10Buffer);
					return FALSE;
				}

				m_dSFRValue[nCount-1] = (float)((sfrvalue1*_sfrD+sfrvalue2*_sfrC)/(_sfrC+_sfrD));
				//DebugInfo("this->Res.SFR[%d] = %.2f",nCount-1,m_dSFRValue[nCount-1] );
				//DebugInfo("SFR_H1 = %.2f, SFR_H2 = %.2f",sfrvalue1,sfrvalue2 );
				//DebugInfo("SFR_C = %.2f, SFR_D = %.2f",_sfrC,_sfrD);
			}          
		}
		else 
		{
			nCount++;

			_sfrC = ofDestDataOut.ofROIposition[i]._C;
			_sfrD = ofDestDataOut.ofROIposition[i]._D;
			_sfrY1 = ofDestDataOut.ofROIposition[i]._y1;
			_sfrY2 = ofDestDataOut.ofROIposition[i]._y2;
			sfrvalue1 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH1,ofDestDataOut.ofROISize,L"H1",nCount,i,select_freque);
			sfrvalue2 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH2,ofDestDataOut.ofROISize,L"H2",nCount,i,select_freque);
			sfrvalue3 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH3,ofDestDataOut.ofROISize,L"H3",nCount,i,select_freque);				
			sfrvalue4 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH4,ofDestDataOut.ofROISize,L"H4",nCount,i,select_freque);


			if (m_bFinalCheck&&pDlg->m_bSaveImage)
			{
				logInfo.Format(_T("[ROI%d] : H1=%.6f  H2=%.6f  H3=%.6f  H4=%.6f\n  _C=%.6f  _D=%.6f  _Y1=%.6f _Y2=%.6f"),
					i,sfrvalue1,sfrvalue2,sfrvalue3,sfrvalue4,_sfrC,_sfrD,_sfrY1,_sfrY2);
				m_pInterface->Ctrl_Addlog(CamID,logInfo,COLOR_BLUE,200);
			}

			_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIH1.LT.x;
			_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIH1.LT.y;
			_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].Color =SFRHCOLOR;
			//			swprintf_s(tempString,L"%.2f", sfrvalue1);
			//			_DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5,SFRHCOLOR);
			blockCount++;

			_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIH2.LT.x;
			_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIH2.LT.y;
			_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].Color = SFRHCOLOR;
			//		swprintf_s(tempString,L"%.2f", sfrvalue2);
			//		_DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5, SFRHCOLOR);
			blockCount++;

			_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIH3.LT.x;
			_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIH3.LT.y;
			_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].Color = SFRHCOLOR;
			//		swprintf_s(tempString,L"%.2f", sfrvalue3);
			//	     _DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5, SFRHCOLOR);
			blockCount++;

			_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIH4.LT.x;
			_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIH4.LT.y;
			_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].Color = SFRHCOLOR;
			//		swprintf_s(tempString,L"%.2f", sfrvalue4);
			//		_DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5, SFRHCOLOR);
			blockCount++;

			if ((sfrvalue1+sfrvalue2+sfrvalue3+sfrvalue4)>999)
			{
				SAFE_DELETE_ARRAY(P10Buffer);
				return FALSE;
			}

			m_dSFRValue[nCount-1] = (float)(((sfrvalue1*_sfrD+sfrvalue2*_sfrC)*_sfrY2/(_sfrC+_sfrD)+(sfrvalue3*_sfrD+sfrvalue4*_sfrC)*_sfrY1/(_sfrC+_sfrD))/(_sfrY1+_sfrY2));
			//DebugInfo("this->Res.SFR[%d] = %.2f",nCount,m_dSFRValue[nCount-1] );
			//DebugInfo("SFR_H1 = %.2f, SFR_H2 = %.2f, SFR_H3 = %.2f, SFR_H4 = %.2f",sfrvalue1,sfrvalue2,sfrvalue3,sfrvalue4 );
			//DebugInfo("SFR_C = %.2f, SFR_D = %.2f, SFR_Y1 = %.2f, SFR_Y2 = %.2f",_sfrC,_sfrD,_sfrY1,_sfrY2);
		}

		if ((ofDestDataOut.ofROIposition[i]._x1+ofDestDataOut.ofROIposition[i]._x2) == 0)
		{
			_sfrA = ofDestDataOut.ofROIposition[i]._A;
			_sfrB = ofDestDataOut.ofROIposition[i]._B;

			if (_sfrA+_sfrB)
			{
				nCount++;

				sfrvalue1 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV1,ofDestDataOut.ofROISize,L"V1",nCount,i,select_freque);
				sfrvalue2 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV2,ofDestDataOut.ofROISize,L"V2",nCount,i,select_freque);
				if (m_bFinalCheck&&pDlg->m_bSaveImage)
				{
					logInfo.Format(_T("[ROI%d] : V1=%.6f  V2=%.6f  _A=%.6f  _B=%.6f"),i,sfrvalue1,sfrvalue2,
						_sfrA,_sfrB);
					m_pInterface->Ctrl_Addlog(CamID,logInfo,COLOR_BLUE,200);
				}
				_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIV1.LT.x;
				_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIV1.LT.y;
				_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
				_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
				_ROIInfo[blockCount].Color = SFRVCOLOR;
				//			  swprintf_s(tempString,L"%.2f", sfrvalue1);
				//			  _DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5,SFRVCOLOR);
				blockCount++;

				_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIV2.LT.x;
				_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIV2.LT.y;
				_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
				_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
				_ROIInfo[blockCount].Color =  SFRVCOLOR;
				//			swprintf_s(tempString,L"%.2f", sfrvalue2);
				//			_DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5,SFRVCOLOR);
				blockCount++;

				if ((sfrvalue1+sfrvalue2)>999)
				{
					SAFE_DELETE_ARRAY(P10Buffer);
					return FALSE;
				}
				m_dSFRValue[nCount-1] = (float)((sfrvalue1*_sfrB+sfrvalue2*_sfrA)/(_sfrA+_sfrB));

				//DebugInfo("this->Res.SFR[%d] = %.2f",nCount-1,m_dSFRValue[nCount-1] );
				//DebugInfo("SFR_V1 = %.2f, SFR_V2 = %.2f",sfrvalue1,sfrvalue2 );
				//DebugInfo("SFR_A = %.2f, SFR_B = %.2f",_sfrA,_sfrB );
			}          
		}
		else 
		{
			nCount++;

			_sfrA = ofDestDataOut.ofROIposition[i]._A;
			_sfrB = ofDestDataOut.ofROIposition[i]._B;
			_sfrX1 = ofDestDataOut.ofROIposition[i]._x1;
			_sfrX2 = ofDestDataOut.ofROIposition[i]._x2;
			sfrvalue1 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV1,ofDestDataOut.ofROISize,L"V1",nCount,i,select_freque);
			sfrvalue2 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV2,ofDestDataOut.ofROISize,L"V2",nCount,i,select_freque);
			sfrvalue3 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV3,ofDestDataOut.ofROISize,L"V3",nCount,i,select_freque);				
			sfrvalue4 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV4,ofDestDataOut.ofROISize,L"V4",nCount,i,select_freque);
			if (m_bFinalCheck&&pDlg->m_bSaveImage)
			{
				logInfo.Format(_T("[ROI%d] : V1=%.6f  V2=%.6f  V3=%.6f  V4=%.6f\n  _A=%.6f  _B=%.6f  _X1=%.6f _X2=%.6f"),
					i,sfrvalue1,sfrvalue2,sfrvalue3,sfrvalue4,_sfrA,_sfrB,_sfrX1,_sfrX2);
				m_pInterface->Ctrl_Addlog(CamID,logInfo,COLOR_BLUE,200);
			}
			_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIV1.LT.x;
			_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIV1.LT.y;
			_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].Color = SFRVCOLOR;
			//			swprintf_s(tempString,L"%.2f", sfrvalue1);
			//			_DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5,SFRVCOLOR);
			blockCount++;

			_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIV2.LT.x;
			_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIV2.LT.y;
			_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].Color = SFRVCOLOR;
			//			swprintf_s(tempString,L"%.2f", sfrvalue2);
			//			_DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5,SFRVCOLOR);
			blockCount++;

			_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIV3.LT.x;
			_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIV3.LT.y;
			_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].Color =  SFRVCOLOR;
			//			swprintf_s(tempString,L"%.2f", sfrvalue3);
			//			_DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5,SFRVCOLOR);
			blockCount++;

			_ROIInfo[blockCount].x = ofDestDataOut.ofROIposition[i].ROIV4.LT.x;
			_ROIInfo[blockCount].y = ofDestDataOut.ofROIposition[i].ROIV4.LT.y;
			_ROIInfo[blockCount].width = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].height = ofDestDataOut.ofROISize;
			_ROIInfo[blockCount].Color =SFRVCOLOR ;
			//			swprintf_s(tempString,L"%.2f", sfrvalue4);
			//			_DrawString[blockCount] = ToFont(tempString,80,_ROIInfo[blockCount].x-4,_ROIInfo[blockCount].y + _ROIInfo[blockCount].height+ 5,SFRVCOLOR);
			blockCount++;

			if ((sfrvalue1+sfrvalue2+sfrvalue3+sfrvalue4)>999)
			{
				SAFE_DELETE_ARRAY(P10Buffer);
				return FALSE;
			}

			m_dSFRValue[nCount-1] = (float)(((sfrvalue1*_sfrB+sfrvalue2*_sfrA)*_sfrX2/(_sfrA+_sfrB)+(sfrvalue3*_sfrB+sfrvalue4*_sfrA)*_sfrX1/(_sfrA+_sfrB))/(_sfrX1+_sfrX2));
			//DebugInfo("this->Res.SFR[%d] = %.2f",nCount-1,m_dSFRValue[nCount-1] );
			//DebugInfo("SFR_V1 = %.2f, SFR_V2 = %.2f, SFR_V3 = %.2f, SFR_V4 = %.2f",sfrvalue1,sfrvalue2,sfrvalue3,sfrvalue4 );
			//DebugInfo("SFR_A = %.2f, SFR_B = %.2f, SFR_X1 = %.2f, SFR_X2 = %.2f",_sfrA,_sfrB,_sfrX1,_sfrX2);
		}
	}
	SAFE_DELETE_ARRAY(P10Buffer);
	iROICount = blockCount;

	return TRUE;
}

BOOL AF_SFR_TestItem::DrawText(OfMarker* pStROIInfo,int iROICount,int iBlockCount)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//得到系统默认字体
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//初始化
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	//COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	//Small SFR ROI
	for (int i = 0; i < iROICount; i++)
	{
		CPen pen(PS_SOLID,2,pStROIInfo[i].Color);
		pDisplayWndMemDC->SelectObject(&pen);
		int cx1,cy1, cx2, cy2;
		cx1 = pStROIInfo[i].x;
		cx2 = pStROIInfo[i].x + pStROIInfo[i].width;
		int px1 = (int)(cx1 * Xratio);
		int px2 = (int)(cx2 * Xratio);

		cy1 = pStROIInfo[i].y ;
		cy2 = pStROIInfo[i].y + pStROIInfo[i].height;
		int py1 = (int) (cy1 * Yratio);
		int py2 = (int) (cy2 * Yratio);

		pDisplayWndMemDC->Rectangle(px1,py1,px2,py2);
	}
	//Big SFR Block
	OfMarker* stROIPos = new OfMarker[MAX_ROI_NUMBER];
    if (stROIPos == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("DrawText new buffer fail"),COLOR_RED,200);
		return FALSE;
	}
	memset(stROIPos,0,sizeof(OfMarker)*MAX_ROI_NUMBER);

	int nCount = 0;
	if (pDlg->m_bTestCenter)
	{
		stROIPos[nCount].x      = (int)(0.5 * ImgWidth);
		stROIPos[nCount++].y      = (int)(0.5 * ImgHeight);
	}

	if (pDlg->m_bTestF3)
	{
		stROIPos[nCount].x      = (int)(0.35 * ImgWidth);
		stROIPos[nCount++].y =  (int)(0.35 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.65 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.35 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.65 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.65 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.35 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.65 * ImgHeight);
	}

	if (pDlg->m_bTestF4)
	{
		stROIPos[nCount].x      = (int)(0.3 * ImgWidth);
		stROIPos[nCount++].y =  (int)(0.3 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.7 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.3 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.7 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.7 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.3 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.7 * ImgHeight);
	}

	if (pDlg->m_bTestF5)
	{
		stROIPos[nCount].x      = (int)(0.25 * ImgWidth);
		stROIPos[nCount++].y =  (int)(0.25 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.75 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.25 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.75 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.75 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.25 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.75 * ImgHeight);
	}

	if (pDlg->m_bTestF6)
	{
		stROIPos[nCount].x      = (int)(0.2 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.2 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.8 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.2 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.8 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.8 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.2 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.8 * ImgHeight);
	}

	if (pDlg->m_bTestF8)
	{
		stROIPos[nCount].x      = (int)(0.1 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.1 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.9 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.1 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.9 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.9 * ImgHeight);
		stROIPos[nCount].x      = (int)(0.1 * ImgWidth);
		stROIPos[nCount++].y = (int)(0.9 * ImgHeight);
	}
	
	for (int i =0; i < iBlockCount; i++)
	{
		int x = stROIPos[i].x - 240;
		int y = stROIPos[i].y - 230;
		int sx = 480;
		int sy = 460;

		int cx1,cy1, cx2, cy2;
		cx1 = x;
		cx2 = x + sx;
		int px1 = (int)(cx1 * Xratio);
		int px2 = (int)(cx2 * Xratio);

		cy1 = y;
		cy2 = y + sy;
		int py1 = (int) (cy1 * Yratio);
		int py2 = (int) (cy2 * Yratio);

		COLORREF color;
		if (m_dSFRValue[i*2] >= m_stSpec[i*2].SFRSpec &&
			m_dSFRValue[i*2 + 1] >= m_stSpec[i*2 + 1].SFRSpec)
		{
			color = RGB(0,0,255);
		}else
		{
			color = RGB(255,0,0);
		}
		CPen pen(PS_SOLID,2,color);
		pDisplayWndMemDC->SelectObject(&pen);
		pDisplayWndMemDC->Rectangle(px1,py1,px2,py2);

		CString str = _T("");
		str.Format(_T("H:%.2f/%.2f  V:%.2f/%.2f"),m_dSFRValue[i*2],	m_stSpec[i*2].SFRSpec,
			m_dSFRValue[i*2 + 1],m_stSpec[i*2 + 1].SFRSpec);
		pDisplayWndMemDC->SetTextColor(color);
		if (i == 7 || i== 8)
		{
			pDisplayWndMemDC->TextOut((int)((cx1 - 160)*Xratio),(int)((cy1 - 105)*Yratio),str);
		}else
		{
			pDisplayWndMemDC->TextOut((int)((cx1 - 160)*Xratio),(int)((cy2 + 5 )*Yratio),str);
		}

		CPen pen1(PS_SOLID,2,COLOR_YELLOW);
	    pDisplayWndMemDC->SelectObject(&pen1);
	    pDisplayWndMemDC->MoveTo( (int)((stROIPos[i].x - 4)*Xratio),(int)((stROIPos[i].y - 4)*Yratio));
	    pDisplayWndMemDC->LineTo((int)((stROIPos[i].x + 4)*Xratio),(int)((stROIPos[i].y + 4)*Yratio));
		pDisplayWndMemDC->MoveTo( (int)((stROIPos[i].x - 4)*Xratio),(int)((stROIPos[i].y + 4)*Yratio));
		pDisplayWndMemDC->LineTo((int)((stROIPos[i].x + 4)*Xratio),(int)((stROIPos[i].y - 4)*Yratio));
	}

	CPen pen(PS_SOLID,2,COLOR_YELLOW);
	pDisplayWndMemDC->SelectObject(&pen);
	pDisplayWndMemDC->MoveTo(0,(int)(0.5 * ImgHeight*Yratio));
	pDisplayWndMemDC->LineTo((int)(ImgWidth*Xratio),(int)(0.5 * ImgHeight*Yratio));

	pDisplayWndMemDC->MoveTo((int)(ImgWidth*0.5*Xratio),0);
	pDisplayWndMemDC->LineTo((int)(ImgWidth*0.5*Xratio),(int)(ImgHeight*Yratio));

	m_pInterface->Ctrl_ShowMemDC(CamID);

	SAFE_DELETE_ARRAY(stROIPos);
	return TRUE;
}


BOOL AF_SFR_TestItem::SaveSFR()
{
	CString strCurrentPath = m_pInterface->Config_GetExeDir();
	CString strFileName(_T(""));
	if(pDlg->m_BPeakCodeDiffEN || pDlg->m_BMacroDiffEn)//远焦
	{
		strFileName = strCurrentPath + _T("SFRBurnFile_PeakCodeCheck.ini");
	}
	else
	{
		strFileName = strCurrentPath + _T("SFRBurnFile.ini");
	}
	
	FILE* fp = NULL;
	fp = _wfopen(strFileName,_T("a"));
	if (fp == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("创建SFRBurnFile.ini文件失败"),COLOR_RED,LOGHEIGHT);
		return FALSE;
	}
	fclose(fp);

	CString temp;
	WritePrivateProfileString(m_Options.m_StationName,_T("MeasureMentPosture"),_T("72"),strFileName);
	temp.Format(_T("%d"),m_iPeakDAC);
	WritePrivateProfileString(m_Options.m_StationName,_T("Peak"),temp,strFileName);
	for (int i =0;i < m_iSFRROICount; i++)
	{
		temp.Format(_T("%d"),(int)(m_dSFRValue[i] * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(m_Options.m_StationName,m_stSpec[i].ROIName,temp,strFileName);
	}

/*	temp.Format(_T("%d"),(int)(m_dSFRValue[0] * m_Options.m_iSFRBurnRate + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CEN_H"),temp,strFileName);

	temp.Format(_T("%d"),(int)(m_dSFRValue[1] * m_Options.m_iSFRBurnRate + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CEN_V"),temp,strFileName);*/

	////////////////////////// CTC ///////////////////////////////////
	
	temp.Format(_T("%d"),(int)(m_fCTC8 + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC8"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC6 + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC6"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC5 + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC5"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC8_H + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC8_H"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC8_V + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC8_V"),temp,strFileName);	
	temp.Format(_T("%d"),(int)(m_fCTC6_H + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC6_H"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC6_V + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC6_V"),temp,strFileName);	
	temp.Format(_T("%d"),(int)(m_fCTC5_H + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC5_H"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC5_V + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC5_V"),temp,strFileName);
	

	CString strLog = _T("");
	int temperature = GetTemperature();
	temp.Format(_T("%d"),temperature);
	WritePrivateProfileString(m_Options.m_StationName,_T("Temperature"),temp,strFileName);

	strLog.Format(_T("Temperature = %d"), temperature);
	m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);

	return TRUE;
}

CString AF_SFR_TestItem::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	ret.Replace(_T("\\"),_T("\\\\"));

	return ret;
}

BOOL AF_SFR_TestItem::CodeDiffCheck(void)
{
	CString strCurrentPath = m_pInterface->Config_GetExeDir();
	CString strFileName = strCurrentPath + _T("SFRBurnFile.ini");

	int MacroCode = 0,InfinityCode = 0;

	wchar_t Section[256] = {0};
	GetPrivateProfileString(pDlg->m_MacroStationName,_T("Peak"),_T("0"),Section,256,strFileName);
	MacroCode = (int)_tcstod(Section,0);

	GetPrivateProfileString(pDlg->m_InfinityStationName,_T("Peak"),_T("1000"),Section,256,strFileName);
	InfinityCode =(int) _tcstod(Section,0);

	int CodeDiff = MacroCode - InfinityCode;
	if (CodeDiff < 0 ||
		CodeDiff < pDlg->m_CodeDiffMin ||
		CodeDiff > pDlg->m_CodeDiffMax)
	{
		m_errMsg = m_cfgName;
		m_errMsg += _T(" Code Diff超范围!");
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
		m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);		
		return FALSE;
	}
	else
	{
		CString Temp = _T("");
		Temp.Format(_T("Code Diff = %d"),CodeDiff);
		m_pInterface->Ctrl_Addlog(CamID,Temp,COLOR_BLUE,LOGHEIGHT);
		return TRUE;
	}
}

int AF_SFR_TestItem::GetTemperature()
{
	/*m_pInterface->Device_WriteI2c(0x6C,0x4D12, 0x00, 3, CamID);
	m_pInterface->Device_WriteI2c(0x6C,0x4D12, 0x01, 3, CamID);

	USHORT Integer = 0;
	m_pInterface->Device_ReadI2c(0x6C,0x4D13, &Integer, 3, CamID);
	USHORT Decimal = 0;
	m_pInterface->Device_ReadI2c(0x6C,0x4D14, &Decimal, 3, CamID);

	USHORT temp = ((Integer << 8) & 0xFF00) | (Decimal & 0xFF);
	USHORT negVal = (((Integer & 0x1F) << 8) & 0xFF00) | (Decimal & 0xFF);
	if(temp <= 0xC000) return temp / 256;
	else return (192 - (negVal / 256));*/

	m_pInterface->Device_WriteI2c(0x20,0x4D12, 0x00, 3, CamID);
	m_pInterface->Device_WriteI2c(0x20,0x4D12, 0x01, 3, CamID);

	USHORT Integer = 0;
	m_pInterface->Device_ReadI2c(0x20,0x4D13, &Integer, 3, CamID);
	USHORT Decimal = 0;
	m_pInterface->Device_ReadI2c(0x20,0x4D14, &Decimal, 3, CamID);

	USHORT temp = ((Integer << 8) & 0xFF00) | (Decimal & 0xFF);
	USHORT negVal = (((Integer & 0x1F) << 8) & 0xFF00) | (Decimal & 0xFF);
	if(temp <= 0xC000) return temp / 256;
	else return (192 - (negVal / 256));
}

int AF_SFR_TestItem::GetTemperature(int* temperature)
{
#if 1
	USHORT temp = 0;
	UCHAR slaveId = 0x34;
	if (CamID == 0)
		slaveId = 0x34;
	else
		slaveId = 0x34;
	m_pInterface->Device_WriteI2c(slaveId, 0x0138, 0x01, 3, CamID);
	Sleep(2000);
	m_pInterface->Device_ReadI2c(slaveId, 0x013A, &temp, 3, CamID);
	m_pInterface->Device_WriteI2c(slaveId, 0x0138, 0x00, 3, CamID);

	if (temp > 76)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Sensor温度异常"),COLOR_RED,200);
		return 0;
	}
	*temperature = temp;
#else
	USHORT temp = 0;
	UCHAR slaveId = 0x6C;
	if (CamID == 0)
		slaveId = 0x6C;
	else
		slaveId = 0x6C;
	m_pInterface->Device_WriteI2c(slaveId, 0x4D12, 0x01, 3, 0);
	Sleep(100);
	m_pInterface->Device_ReadI2c(slaveId, 0x4D13, &temp, 4, 0);

	int senT = 0;
	if (temp <= 0xC000)
		senT = (int)(temp / 256 + 0.5);
	else
		senT = (int)(192 - temp / 256 + 0.5);

	CString strS = _T("");
	strS.Format(_T("Sensor T = %d"), senT);
	m_pInterface->Ctrl_Addlog(0, strS, COLOR_BLUE, 200);

	*temperature = senT;
#endif
	
	return 1;
}

int AF_SFR_TestItem::GetTemperature_Driver(USHORT* temperature)
{
#if 0
	UCHAR slaveId = 0;
	if (CamID == 0)
		slaveId = 0xE6;
	else
		slaveId = 0xE6;
	m_pInterface->Device_WriteI2c(slaveId, 0x0101, 0x00, 3, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0110, 0xEC, 3, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0111, 0xFE, 3, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0123, 0x8B, 3, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0124, 0x40, 3, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0125, 0xFF, 3, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0126, 0x40, 3, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0154, 0x1B00,4, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0156, 0x0000,4, CamID);
	Sleep(20);
	m_pInterface->Device_WriteI2c(slaveId, 0x0166, 0xA800,4, CamID);
	Sleep(2000);
	if (CamID == 0)
		slaveId = 0xE4;	//Register
	else
		slaveId = 0xE4;
	m_pInterface->Device_WriteI2c(slaveId, 0x00E0, 0x01, 3, CamID);
	Sleep(20);

	USHORT high_Byte=0;
	USHORT Low_Byte=0;
	m_pInterface->Device_ReadI2c(slaveId, 0x00B6, &high_Byte, 3, CamID);
	Sleep(20);
	m_pInterface->Device_ReadI2c(slaveId, 0x00B7, &Low_Byte, 3, CamID);
	Sleep(20);
	int temp = ((high_Byte << 4) & 0x0FF0) + ((Low_Byte >> 4) & 0x000F);
	float temperature_f = 0;
	temperature_f = (float)((float)(temp - 0xFF4) / 4096.0 * 2.8 /(-3.73)+25);
	CString logstr;
	logstr.Format(_T("temperature_float = %f"),temperature_f);
	m_pInterface->Ctrl_Addlog(CamID,logstr,COLOR_BLUE,200);
	*temperature = int(temperature_f + 0.5);
#else
	*temperature = 0;
#endif

	return 1;
}

double AF_SFR_TestItem::MTFTest()
{
	BYTE* Y_buffer = new BYTE[img.imgwidth*img.imgheight];
	if (Y_buffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("MTFTest new Y buffer fail"),COLOR_RED,200);
		return 0;
	}
    memset(Y_buffer,0,sizeof(BYTE)*img.imgwidth*img.imgheight);//初始化
	ImageProc::RGB24toY(img.RGB24_buffer,Y_buffer,img.imgwidth,img.imgheight);
	CRect AFRect;
	AFRect.left = m_Options.m_iContrastROIStartX;
	AFRect.top = m_Options.m_iContrastROIStartY;
	AFRect.right = AFRect.left + m_Options.m_iContrastROIWidth;
	AFRect.bottom = AFRect.top + m_Options.m_iContrastROIHeight;
	double mtfValue = ImageProc::GetMTF(Y_buffer,img.imgwidth,img.imgheight,AFRect)*100;

	SAFE_DELETE_ARRAY(Y_buffer);
	return mtfValue;
}
ULONG64 AF_SFR_TestItem::ContrastMeasure()
{
	ULONG64 iContrastValue = 0;
	int iImgW = img.imgwidth;
	int iImgH = img.imgheight;

	int iStartX = m_Options.m_iContrastROIStartX;
	int iStartY = m_Options.m_iContrastROIStartY;
	for (int y = 0; y < m_Options.m_iContrastROIHeight; y++)
	{
		for (int x = 0; x < m_Options.m_iContrastROIWidth; x++)
		{
			double top = abs(img.RGB24_buffer[((iStartY + y - 1 )*img.imgwidth + iStartX)*3 + 1] - 
						img.RGB24_buffer[((iStartY + y )*img.imgwidth + iStartX)*3 + 1]);
			double bottom = abs(img.RGB24_buffer[((iStartY + y + 1 )*img.imgwidth + iStartX)*3 + 1] - 
				img.RGB24_buffer[((iStartY + y )*img.imgwidth + iStartX)*3 + 1]);
			double left = abs(img.RGB24_buffer[((iStartY + y  )*img.imgwidth + iStartX - 1)*3 + 1] - 
				img.RGB24_buffer[((iStartY + y )*img.imgwidth + iStartX)*3 + 1]);
			double right = abs(img.RGB24_buffer[((iStartY + y  )*img.imgwidth + iStartX + 1)*3 + 1] - 
				img.RGB24_buffer[((iStartY + y )*img.imgwidth + iStartX)*3 + 1]);

			iContrastValue += (ULONG64)(pow(top,2) + pow(bottom,2) + pow(left,2) + pow(right,2));
		}
	}
	return iContrastValue;
}

void AF_SFR_TestItem::DrawContrastInfo()
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//得到系统默认字体
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//初始化
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	//COLORREF CurrentColor;
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	int px1 = (int)(m_Options.m_iContrastROIStartX * Xratio);
	int py1 = (int)(m_Options.m_iContrastROIStartY * Yratio);
	int px2 = (int)((m_Options.m_iContrastROIStartX + m_Options.m_iContrastROIWidth)*Xratio);
	int py2 = (int)((m_Options.m_iContrastROIStartY + m_Options.m_iContrastROIHeight)*Yratio);
	pDisplayWndMemDC->Rectangle(px1,py1,px2,py2);
	m_pInterface->Ctrl_ShowMemDC(CamID);
}

int AF_SFR_TestItem::GetPeakCodeByAfValue(double* afVal, double* afCode)
{
	int ret = 0;

	double maxVal = 0.0f;
	for (int i = 0; i<512; i++)
	{
		if (afVal[i] > maxVal)
		{
			maxVal = afVal[i];
			ret = (int)afCode[i];
		}
	}

	return ret;
}
void AF_SFR_TestItem::Convert2Char(char* cString,int iMaxLength,CString strSting)
{
	memset(cString,0,iMaxLength);
	int length = strSting.GetLength();
	WideCharToMultiByte(CP_ACP, 0, strSting.GetBuffer(length), length, cString, 1024, 0, 0);
}
void AF_SFR_TestItem::SaveData_InFact(bool bResult)
{

	FILE *fp = NULL;

	CString sfilename = _T("");
	CString stSpec = _T("");
	CString strLog = _T("");

	strLog = m_pInterface->Ctrl_GetCurrentPath() + _T("SFR Actually");
	CreateDirectory(strLog,NULL);
	CString localtime = m_pInterface->Ctrl_GetCurrentLocalTime();

	switch(0)
	{
	case 0:
		sfilename.Format(_T("%s\\%s_Cam%d_SFRData_Actually%s.csv"),strLog,m_cfgName,CamID,localtime.Mid(0,8));
		//stSpec.Format(_T("%s\\Cam%dSpecData_%s.csv"),strLog,CamID,localtime.Mid(0,8));
		break;
	}
	char fileName[1024] = {0};	
	Convert2Char(fileName,1024,sfilename);
	fp = fopen(fileName, "a");
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeWrite | CFile::typeText);  
	char strTemp[1024] = {0};

	CString header(_T(""));
	for (int i = 0; i < m_iSFRROICount; i++)
	{
		header += m_stSpec[i].ROIName;
		header += _T(",");
	}
	header += _T("CTC8,CTC6,CTC8_H,CTC8_V,CTC6_H,CTC6_V");
	char TempHeader[1024] = {0};	
	Convert2Char(TempHeader,1024,header);


	//表头
	if (MyFile.GetLength() == 0)
	{
		sprintf_s(strTemp,512,"SN,time,Distance,Result,");
		char tmp[512] = {0};
		strcat_s(strTemp,512,TempHeader);
		strcat_s(strTemp,"\r\n");
		MyFile.Write(strTemp,strlen(strTemp));
	}
	MyFile.SeekToEnd();
#if 0
	CString StrData(_T(""));
	for (int i =0;i < m_iSFRROICount; i++)
	{
		StrData.Format(_T("%d"),(int)(m_dSFRValue_Fact[i] * m_Options.m_iSFRBurnRate + 0.5));
		//WritePrivateProfileString(m_Options.m_StationName,m_stSpec[i].ROIName,temp,strFileName);
	}
#endif
	CString strSFRStationName(_T(""));

	//表中内容
	char clocaltime[512] = {0};
	ImageProc::ConvertWideToChar(localtime,clocaltime);
	char sensorID[512] = {0};
	ImageProc::ConvertWideToChar(m_pInterface->Sensor_GetCamFuseID(CamID),sensorID);
	m_pInterface->Ctrl_Addlog(CamID,m_pInterface->Sensor_GetCamFuseID(CamID),COLOR_BLUE,200);

	char str_StationName[512] = {0};
	ImageProc::ConvertWideToChar(m_cfgName,str_StationName);
	sprintf_s(strTemp,512,"%s,%s,%s,%s,",sensorID,clocaltime,str_StationName,(bResult==TRUE)?"PASS":"FAIL");

	CString Content(_T(""));
	CString temp;
	for (int index = 0; index < m_iSFRROICount; index++)
	{
		temp.Format(_T("%f,"),m_dSFRValue_Fact[index]);
		Content += temp;
	}
	temp.Format(_T("%f,%f,%f,%f,%f,%f"),m_fCTC8_Fact,m_fCTC6_Fact,m_fCTC8_H_Fact,m_fCTC8_V_Fact,m_fCTC6_H_Fact,m_fCTC6_V_Fact);
	Content += temp;
	char TempContent[1024] = {0};
	Convert2Char(TempContent,1024,Content);

	char tmpString[512] = {0};
	//sprintf_s(tmpString,512,"%d,%d,%d,%.3f,%.3f,%d,%.3f,%d,%.3f   %s",m_InfsubLas,m_LassubMac,m_distri,m_Maxdiff,m_uslope,m_Infcode,m_Infheight,m_Maccode,m_Macheight, strErrorInfo);
	//sprintf_s(tmpString,512,"%d,%d,%d,%.3f,%.3f,%d,%.3f,%d,%.3f",m_InfsubLas,m_LassubMac,m_distri,m_Maxdiff,m_uslope,m_Infcode,m_Infheight,m_Maccode,m_Macheight);
	strcat_s(strTemp,TempContent);//FuseId,时间，内容
	strcat_s(strTemp,"\n");

	MyFile.Write(strTemp,strlen(strTemp));
	MyFile.Close();   

}

void AF_SFR_TestItem::SaveSFRData_Offset(bool bResult)
{

	FILE *fp = NULL;

	CString sfilename = _T("");
	CString stSpec = _T("");
	CString strLog = _T("");

	strLog = m_pInterface->Ctrl_GetCurrentPath() + _T("SFR OFFSET");
	CreateDirectory(strLog,NULL);
	CString localtime = m_pInterface->Ctrl_GetCurrentLocalTime();

	switch(0)
	{
	case 0:
		sfilename.Format(_T("%s\\%s_Cam%d_SFRData_Offset%s.csv"),strLog,m_cfgName,CamID,localtime.Mid(0,8));
		//stSpec.Format(_T("%s\\Cam%dSpecData_%s.csv"),strLog,CamID,localtime.Mid(0,8));
		break;
	}
	char fileName[1024] = {0};	
	Convert2Char(fileName,1024,sfilename);
	fp = fopen(fileName, "a");
	fclose(fp);

	CStdioFile MyFile(sfilename, CFile::modeWrite | CFile::typeText);  
	char strTemp[1024] = {0};

	CString header(_T(""));
	for (int i = 0; i < m_iSFRROICount; i++)
	{
		header += m_stSpec[i].ROIName;
		header += _T(",");
	}
	header += _T("CTC8,CTC6,CTC8_H,CTC8_V,CTC6_H,CTC6_V");
	char TempHeader[1024] = {0};	
	Convert2Char(TempHeader,1024,header);
	//表头
	if (MyFile.GetLength() == 0)
	{
		sprintf_s(strTemp,512,"SN,time,Distance,Result,");
		char tmp[512] = {0};
		strcat_s(strTemp,512,TempHeader);
		strcat_s(strTemp,"\r\n");
		MyFile.Write(strTemp,strlen(strTemp));
	}
	MyFile.SeekToEnd();

#if 0
	CString StrData(_T(""));
	for (int i =0;i < m_iSFRROICount; i++)
	{
		StrData.Format(_T("%d"),(int)(m_dSFRValue_Offset[i] * m_Options.m_iSFRBurnRate + 0.5));
		//WritePrivateProfileString(m_Options.m_StationName,m_stSpec[i].ROIName,temp,strFileName);
	}
#endif
	//表中内容
	char clocaltime[512] = {0};
	ImageProc::ConvertWideToChar(localtime,clocaltime);
	char sensorID[512] = {0};
	CString strtempFuseid(_T(""));
	ImageProc::ConvertWideToChar(m_pInterface->Sensor_GetCamFuseID(CamID),sensorID);
	m_pInterface->Ctrl_Addlog(CamID,m_pInterface->Sensor_GetCamFuseID(CamID),COLOR_BLUE,200);

	char str_StationName[512] = {0};
	ImageProc::ConvertWideToChar(m_cfgName,str_StationName);

	sprintf_s(strTemp,512,"%s,%s,%s,%s,",sensorID,clocaltime,str_StationName,(bResult==TRUE)?"PASS":"FAIL");//sensorID

	//sprintf_s(strTemp,512,"%s,%s,%s,",sensorID,clocaltime,);

	CString Content(_T(""));
	CString temp;
	for (int index = 0; index < m_iSFRROICount; index++)
	{
		temp.Format(_T("%f,"),m_dSFRValue_Offset[index]);
		Content += temp;
	}
	temp.Format(_T("%f,%f,%f,%f,%f,%f"),m_fCTC8,m_fCTC6,m_fCTC8_H,m_fCTC8_V,m_fCTC6_H,m_fCTC6_V);
	Content += temp;
	char TempContent[1024] = {0};
	Convert2Char(TempContent,1024,Content);

	char tmpString[512] = {0};
	//sprintf_s(tmpString,512,"%d,%d,%d,%.3f,%.3f,%d,%.3f,%d,%.3f   %s",m_InfsubLas,m_LassubMac,m_distri,m_Maxdiff,m_uslope,m_Infcode,m_Infheight,m_Maccode,m_Macheight, strErrorInfo);
	//sprintf_s(tmpString,512,"%d,%d,%d,%.3f,%.3f,%d,%.3f,%d,%.3f",m_InfsubLas,m_LassubMac,m_distri,m_Maxdiff,m_uslope,m_Infcode,m_Infheight,m_Maccode,m_Macheight);
	strcat_s(strTemp,TempContent);//FuseId,时间，内容
	strcat_s(strTemp,"\n");

	MyFile.Write(strTemp,strlen(strTemp));
	MyFile.Close();   

}


