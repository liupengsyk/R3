#include "StdAfx.h"
#include "FF_SFR_TestItem.h"
#include "ImageProc.h"
#include "CurveFit2.h"
#include "SFR_Process.h"
#include "float.h"
#pragma comment(lib,"SFR_LENOVO.lib")
#pragma comment(lib,"CurveFit.lib")
#pragma comment(lib,"BoardChartAutoROI.lib")


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
#define SKIPFRAME _T("SkipFrame")
#define CTC8 _T("CTC8")
#define CTC6 _T("CTC6")
#define CTC8H _T("CTC8H")
#define CTC8V _T("CTC8V")
#define CTC6H _T("CTC6H")
#define CTC6V _T("CTC6V")
#define SHAKESKIPFRAME _T("ShakeSkipFrame")
#define SFRSPEC _T("SFRSpec")
#define STATIONNAME _T("StationName")
#define SFRBURNRATE _T("SFRBurnRate")

#define MAX_ROI_NUMBER 13

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

	return new FF_SFR_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
FF_SFR_TestItem::FF_SFR_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

FF_SFR_TestItem::~FF_SFR_TestItem(void)
{

}


int FF_SFR_TestItem::InitItem()
{
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
int FF_SFR_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	m_pInterface->Ctrl_Addlog(CamID,_T("FF Test Initialize"),COLOR_RED,220);
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
	CString path=m_pInterface->Ctrl_GetCurrentPath();
	path=path+L"ofset.ini";

	SetConfigPath(path);	 //设置CONFIG路径	

//	SetName(L"OTPCheck");

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

	if (pDlg->m_bTestF4)
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

	if (pDlg->m_bTestF6)
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

	if (pDlg->m_bTestF8)
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

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(m_cfgName);
	memset(SFRAll,0,sizeof(double)*10*MAXROICOUNT);
	memset(m_stROIPos,0,sizeof(ROIPosInfo)*MAXROICOUNT);
	memset(m_dSFRValue,0,sizeof(float)*MAXROICOUNT);
	memset(m_dSFR40CM_Data,0,sizeof(int)*MAXROICOUNT);
	memset(m_dSFR60CM_Data,0,sizeof(int)*MAXROICOUNT);
	memset(m_dSFR120CM_Data,0,sizeof(int)*MAXROICOUNT);
	
	memset(m_dSFRValue_Fact,0,sizeof(m_dSFRValue_Fact));
	memset(m_dSFRValue_Offset,0,sizeof(m_dSFRValue_Offset));
	memset(m_stSpec,0,sizeof(SpecInfo)*MAXROICOUNT);
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
	m_nJumpPos = -1;
	m_iSkipFrame = 0;

	m_iFinalAutoROITime = 0;
	m_iFinalSFRTest = 0;
	m_bFocusing = FALSE;
	m_bFinalCheck = FALSE;
	fqcFirstTime = TRUE;
	iTestTimes = 0;
	nTestTime = 0;
	memset(&m_ofDestDataOut,0,sizeof(OFAutoROIOut));
	AnalysisROISpec();
	return 0;
}


/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int FF_SFR_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	m_pInterface->Ctrl_Addlog(CamID,_T("FF Testing"),COLOR_BLUE,220);
	 bResult = true; 
	CString errorInfo;
	if (TRUE == m_bROIConfigureError)
	{
		errorInfo = m_cfgName + _T(" ROI Configure Error...");
		m_pInterface->Ctrl_Addlog(CamID,errorInfo,COLOR_RED,220);
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,errorInfo);
		bResult = false; 
		return 0;
	}
	OfMarker stROIInfo[300] = {0};
	if (m_nJumpPos == -1)     //跳帧
	{
		    m_pInterface->Ctrl_Addlog(CamID,_T("m_nJumpPos == -1"),COLOR_BLUE,220);
			if (m_iSkipFrame < m_Options.m_iShakeSkipFrame)
			{
				m_iSkipFrame++;
				return 1;
			}

			m_iSkipFrame = 0;

		    m_nJumpPos =1 ;

		return 1;
	}
	else if (m_nJumpPos == 1)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("m_nJumpPos == 1"),COLOR_BLUE,220);
		//Sleep(450);
		img  = *(m_pInterface->Ctrl_GetImgInfo(CamID));
		m_pInterface->Ctrl_Addlog(CamID,_T("img  = *(m_pInterface->Ctrl_GetImgInfo(CamID))"),COLOR_BLUE,220);
		if (!m_bFinalCheck)	
		{	
			Sleep(550);
			memset(m_stROIInfo,0,sizeof(OfMarker)*120);
			memset(&m_ofDestDataOut,0,sizeof(OFAutoROIOut));
			m_pInterface->Ctrl_Addlog(CamID,_T("BOOL ret = AutoROI(m_ofDestDataOut)"),COLOR_BLUE,220);
			BOOL ret = AutoROI(m_ofDestDataOut);
			Sleep(600);
			CString strtempret(_T("AutoROI返回值：%d"),ret);
			m_pInterface->Ctrl_Addlog(CamID,strtempret,COLOR_BLUE,220);
			if (ret)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("AutoROI Sucesss"),COLOR_BLUE,220);
				m_iBlockCount = m_ofDestDataOut.ofROInumber;
				m_bFinalCheck = TRUE;
			}
			else
			{
				m_iFinalAutoROITime++;
				m_pInterface->Ctrl_Addlog(CamID,_T("AutoROI Failed"),COLOR_RED,220);
				if (m_iFinalAutoROITime == 5)
				{
					errorInfo = m_cfgName + _T("连续Auto ROI 5次失败...");
					m_pInterface->Ctrl_Addlog(CamID,errorInfo,COLOR_RED,LOGHEIGHT);
						
					m_pInterface->Ctrl_SetCamErrorInfo(CamID,errorInfo);
					SetResult(RESULT_FAIL);
					m_pInterface->Ctrl_SetCamErrorCode(CamID,m_Options.ErrorCode);
				//	DebugInfo("ErrorCode = %d",m_Options.ErrorCode);
					bResult = false; 
					return 0;
				}
			}
		}

		/*OfMarker stROIInfo[120] = {0};*/
		m_pInterface->Ctrl_Addlog(CamID,_T("SFRTest 开始计算"),COLOR_BLUE,220);
		BOOL bSFRResult = SFRTest(m_ofDestDataOut,stROIInfo,m_iROICount);
		if (!bSFRResult)
		{
			m_iFinalSFRTest++;
			CString temp;
	        m_pInterface->Ctrl_Addlog(CamID,_T("SFRTest Sucesss"),COLOR_BLUE,220);
			if (m_iFinalSFRTest > 5)
			{
				errorInfo = m_cfgName + _T("连续5次 计算SFR失败...");
				m_pInterface->Ctrl_Addlog(CamID,errorInfo,COLOR_RED,LOGHEIGHT);
					
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,errorInfo);
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetCamErrorCode(CamID,m_Options.ErrorCode);
				//DebugInfo("ErrorCode = %d",m_Options.ErrorCode);
				bResult = false; 
				return 0;
			}
			else
			{
				    m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_YELLOW,LOGHEIGHT);
				    return 1;
			}
		}
		m_pInterface->Ctrl_Addlog(CamID,_T("DrawText(stROIInfo,m_iROICount,m_iBlockCount)"),COLOR_BLUE,220);
		DrawText(stROIInfo,m_iROICount,m_iBlockCount);
		if (!m_Options.m_bMidValue)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("!m_Options.m_bMidValue"),COLOR_RED,220);
			m_nJumpPos = 2;
			return 1;
		}
		//取中值
		m_pInterface->Ctrl_Addlog(CamID,_T("取中值"),COLOR_BLUE,220);
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
							double SFRTemp=SFRAll[TestTimeIndex][ROIIndex];
							SFRAll[TestTimeIndex][ROIIndex]=SFRAll[index][ROIIndex];
							SFRAll[index][ROIIndex]=SFRTemp;
						}
					}
				}
			}
			//排序完成
			m_pInterface->Ctrl_Addlog(CamID,_T("排序完成"),COLOR_BLUE,220);
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
					m_dSFRValue[i] = 0.5*(SFRAll[(m_Options.m_Mid_TestTime/2)-1][i]+SFRAll[m_Options.m_Mid_TestTime/2][i]);
					m_dSFRValue_Fact[i] = 0.5*(SFRAll[(m_Options.m_Mid_TestTime/2)-1][i]+SFRAll[m_Options.m_Mid_TestTime/2][i]);
					m_dSFRValue_Offset[i] = 0.5*(SFRAll[(m_Options.m_Mid_TestTime/2)-1][i]+SFRAll[m_Options.m_Mid_TestTime/2][i]);
				}
			}
			
			m_nJumpPos = 2;
	
		}
		return 1;
	}
	else if (m_nJumpPos == 2)	//卡规格
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("m_nJumpPos == 2"),COLOR_BLUE,220);
		Sleep(500);
		m_bResult = CheckFinalSpec();

		DrawText(stROIInfo,m_iROICount,m_iBlockCount);

		CString temp;
		temp.Format(_T("CEN_H=%.4f  CEN_V=%.4f"),m_dSFRValue[0],m_dSFRValue[1]);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
		temp.Format(_T("isSFRBurn = %d"),pDlg->m_isSFRBurn);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);

		if(!m_bResult)
		{
			bResult = false;
		}
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
		if (pDlg->m_isSFRBurn)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("pDlg->m_isSFRBurn,SaveSFR()"),COLOR_BLUE,220);
			SaveSFR();
			Save40CMSFR();
			Save60CMSFR();
			Save120CMSFR();
		}
		if (m_bResult)
		{
			BOOL BSFRSpec = SFRSpecLimit();//数据卡控
			if(BSFRSpec)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("SFR数据卡控失败);"),COLOR_RED,220);
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetCamErrorCode(CamID,m_Options.ErrorCode);
				bResult = false; 
				return 0;
			}
			m_pInterface->Ctrl_Addlog(CamID,_T("SetResult(RESULT_PASS);"),COLOR_BLUE,220);
			SetResult(RESULT_PASS);
		}else
		{
			if (pDlg->m_EnableSecondCheck==1 && nTestTime==0)
			{
				Initialize();
				nTestTime = 1;
				m_iSkipFrame = 0;
				m_nJumpPos =-1 ;
				m_pInterface->Ctrl_Addlog(CamID,_T("开始第二次 SFR Check"),COLOR_BLUE,200);
				return 1;
			} 
			else
			{
				
				m_pInterface->Ctrl_Addlog(CamID,_T("SetResult(RESULT_FAIL);"),COLOR_RED,220);
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetCamErrorCode(CamID,m_Options.ErrorCode);
				//DebugInfo("ErrorCode = %d",m_Options.ErrorCode);
				//bResult = false; 
			}
				
		}
		m_pInterface->Ctrl_Addlog(CamID,_T("return 0;"),COLOR_BLUE,220);
		return 0;
	}
   m_pInterface->Ctrl_Addlog(CamID,_T("函数return 0;"),COLOR_BLUE,220);
   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int FF_SFR_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int FF_SFR_TestItem::LoadOption()
{
	//leger add
	pDlg->m_EnableSecondCheck = ReadConfigInt(_T("EnableSecondCheck"),1);

	pDlg->m_SpcShiftEn_0F = ReadConfigInt(_T("SpcShift_En_0F"),0);
	pDlg->m_SpcShiftEn_4F = ReadConfigInt(_T("SpcShift_En_4F"),0);
	pDlg->m_SpcShiftEn_6F = ReadConfigInt(_T("SpcShift_En_6F"),0);
	pDlg->m_SpcShiftEn_8F = ReadConfigInt(_T("SpcShift_En_8F"),0);
	pDlg->m_SpcShift_CTC_En_6F = ReadConfigInt(_T("SpcShift_CTC_En_6F"),0);
	pDlg->m_SpcShift_CTC_En_8F = ReadConfigInt(_T("SpcShift_CTC_En_8F"),0);

	pDlg->m_bTestCenter = ReadConfigInt(_T("bTestCenter"),0);
	pDlg->m_bTestF4 = ReadConfigInt(_T("bTestF4"),0);
	pDlg->m_bTestF6 = ReadConfigInt(_T("bTestF6"),0);
	pDlg->m_bTestF8 = ReadConfigInt(_T("bTestF8"),0);

	pDlg->m_isSFRBurn = ReadConfigInt(_T("bSFRBurnEn"),0);


	pDlg->m_bSaveImage =  ReadConfigInt(_T("SaveImage"),0);
	pDlg->m_ROI_Size =  ReadConfigInt(_T("ROISize"),96); 
	CString temp;

	temp = 	ReadConfigString(_T("SpecShift_0F"),_T("0"));	
	pDlg->m_SpcShift_0F = _tcstod(temp,0);

	m_Options.ErrorCode = ReadConfigInt(_T("ErrorCode"),0);
	temp = 	ReadConfigString(_T("CenH_Spec"),_T("0"));	
	m_Options.m_CenH_Spec = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("CenV_Spec"),_T("0"));	
	m_Options.m_CenV_Spec = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("F4H_Spec"),_T("0"));	
	m_Options.m_F4H_Spec = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("F4V_Spec"),_T("0"));	
	m_Options.m_F4V_Spec = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("F6H_Spec"),_T("0"));	
	m_Options.m_F6H_Spec = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("F6V_Spec"),_T("0"));	
	m_Options.m_F6V_Spec = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("F8H_Spec"),_T("0"));	
	m_Options.m_F8H_Spec = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("F8V_Spec"),_T("0"));	
	m_Options.m_F8V_Spec = _tcstod(temp,0);

	temp = 	ReadConfigString(_T("SpecShift_0F"),_T("0"));	
	pDlg->m_SpcShift_0F = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_4F"),_T("0"));	
	pDlg->m_SpcShift_4F = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_6F"),_T("0"));	
	pDlg->m_SpcShift_6F = _tcstod(temp,0);
	temp = 	ReadConfigString(_T("SpecShift_8F"),_T("0"));	
	pDlg->m_SpcShift_8F = _tcstod(temp,0);

	temp = 	ReadConfigString(_T("SpecShift_6F_CTC"),_T("0"));	
	pDlg->m_SpcShift_6F_CTC = _tcstod(temp,0);

	temp = 	ReadConfigString(_T("SpecShift_8F_CTC"),_T("0"));	
	pDlg->m_SpcShift_8F_CTC = _tcstod(temp,0);

	m_Options.m_Mid_TestTime =  ReadConfigInt(_T("MidTestTime"),0);
	if ((m_Options.m_Mid_TestTime<1)||(m_Options.m_Mid_TestTime>8))
	{
		m_Options.m_Mid_TestTime = 1;
	}
	//TODO: 在此添加读取代码 
	m_Options.bEngineerMode = ReadConfigInt(ENGINEMODE,0);
	m_Options.m_bMidValue = ReadConfigInt(MIDLLEVALUE_EN,1);

	temp = 	ReadConfigString(CTC8,_T("0"));	
	m_Options.m_fCTC8 = _tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC8);
	temp = 	ReadConfigString(CTC6,_T("0"));
	m_Options.m_fCTC6 = _tcstod(temp,0);
	//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC6);
	temp = 	ReadConfigString(CTC8H,_T("0"));
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC8_H);
	m_Options.m_fCTC8_H = _tcstod(temp,0);
	temp = 	ReadConfigString(CTC8V,_T("0"));
	m_Options.m_fCTC8_V = _tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC8_V);
	temp = 	ReadConfigString(CTC6H,_T("0"));
	m_Options.m_fCTC6_H = _tcstod(temp,0);
//	swscanf_s(temp.GetBuffer(temp.GetLength()),_T("%.3f"),&m_Options.m_fCTC6_H);
	temp = 	ReadConfigString(CTC6V,_T("0"));
	m_Options.m_fCTC6_V = _tcstod(temp,0);

	m_Options.m_iShakeSkipFrame = ReadConfigInt(SHAKESKIPFRAME,3);

//	m_Options.strSpecROIs = ReadConfigString(SFRSPEC,_T(""));
	m_Options.m_StationName = ReadConfigString(STATIONNAME,_T(""));
	m_Options.m_iSFRBurnRate = ReadConfigInt(SFRBURNRATE,100);

	pDlg->m_sfrFreq = ReadConfigInt(_T("SFR_TEST_FREQ"), 4);

	pDlg->m_BchekSFRSpec = ReadConfigInt(_T("m_BchekSFRSpec"), 0);

	
	pDlg->m_BSaveSFR_Acut = ReadConfigInt(_T("m_BSaveSFR_Acut"), 1);
	pDlg->m_BSFRSaveDataOffset = ReadConfigInt(_T("m_BSFRSaveDataOffset"), 1);

	pDlg->m_B40cmSFRCheckData = ReadConfigInt(_T("m_B40cmSFRCheckData"), 0);
	pDlg->m_B60cmSFRCheckData = ReadConfigInt(_T("m_B60cmSFRCheckData"), 0);
	pDlg->m_B120cmSFRCheckData = ReadConfigInt(_T("m_B120cmSFRCheckData"), 0);
	//........................
	pDlg->m_StrSpecSFR_FIRST = 	ReadConfigString(_T("m_StrSpecSFR_FIRST"),_T("8"));
	pDlg->m_StrSpecSFR_SECOND = ReadConfigString(_T("m_StrSpecSFR_SECOND"),_T("8"));
	pDlg->m_BLC = ReadConfigInt(_T("m_BLC"), 0);
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int FF_SFR_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("ROISize"),pDlg->m_ROI_Size);
	WriteConfigInt(_T("bTestCenter"),pDlg->m_bTestCenter);
	WriteConfigInt(_T("bTestF4"),pDlg->m_bTestF4);
	WriteConfigInt(_T("bTestF6"),pDlg->m_bTestF6);
	WriteConfigInt(_T("bTestF8"),pDlg->m_bTestF8);
	WriteConfigInt(_T("ErrorCode"),m_Options.ErrorCode);

	WriteConfigInt(_T("bSFRBurnEn"),pDlg->m_isSFRBurn);
	WriteConfigInt(_T("EnableSecondCheck"),pDlg->m_EnableSecondCheck);

	WriteConfigInt(_T("SaveImage"),pDlg->m_bSaveImage);	

	CString temp;

	temp.Format(_T("%.6f"),pDlg->m_SpcShift_0F);
	WriteConfigString(_T("SpecShift_0F"),temp);

	temp.Format(_T("%.6f"),m_Options.m_CenH_Spec);
	WriteConfigString(_T("CenH_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_CenV_Spec);
	WriteConfigString(_T("CenV_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F4H_Spec);
	WriteConfigString(_T("F4H_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F4V_Spec);
	WriteConfigString(_T("F4V_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F6H_Spec);
	WriteConfigString(_T("F6H_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F6V_Spec);
	WriteConfigString(_T("F6V_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F8H_Spec);
	WriteConfigString(_T("F8H_Spec"),temp);
	temp.Format(_T("%.6f"),m_Options.m_F8V_Spec);
	WriteConfigString(_T("F8V_Spec"),temp);
	WriteConfigInt(_T("MidTestTime"),m_Options.m_Mid_TestTime);
	
	//old 
	WriteConfigInt(ENGINEMODE,m_Options.bEngineerMode);
	WriteConfigInt(MIDLLEVALUE_EN,m_Options.m_bMidValue);

	temp.Format(_T("%.6f"),m_Options.m_fCTC8);
	WriteConfigString(CTC8,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC6);
	WriteConfigString(CTC6,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC8_H);
	WriteConfigString(CTC8H,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC8_V);
	WriteConfigString(CTC8V,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC6_H);
	WriteConfigString(CTC6H,temp);
	temp.Format(_T("%.6f"),m_Options.m_fCTC6_V);
	WriteConfigString(CTC6V,temp);

	temp.Format(_T("%.6f"),pDlg->m_SpcShift_0F);
	WriteConfigString(_T("SpecShift_0F"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_4F);
	WriteConfigString(_T("SpecShift_4F"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_6F);
	WriteConfigString(_T("SpecShift_6F"),temp);
	temp.Format(_T("%.6f"),pDlg->m_SpcShift_8F);
	WriteConfigString(_T("SpecShift_8F"),temp);

	temp.Format(_T("%.6f"),pDlg->m_SpcShift_6F_CTC);
	WriteConfigString(_T("SpecShift_6F_CTC"),temp);

	temp.Format(_T("%.6f"),pDlg->m_SpcShift_8F_CTC);
	WriteConfigString(_T("SpecShift_8F_CTC"),temp);

	WriteConfigInt(_T("SpcShift_En_0F"),pDlg->m_SpcShiftEn_0F);
	WriteConfigInt(_T("SpcShift_En_4F"),pDlg->m_SpcShiftEn_4F);
	WriteConfigInt(_T("SpcShift_En_6F"),pDlg->m_SpcShiftEn_6F);
	WriteConfigInt(_T("SpcShift_En_8F"),pDlg->m_SpcShiftEn_8F);
	WriteConfigInt(_T("SpcShift_CTC_En_6F"),pDlg->m_SpcShift_CTC_En_6F);
	WriteConfigInt(_T("SpcShift_CTC_En_8F"),pDlg->m_SpcShift_CTC_En_8F);

	WriteConfigInt(SHAKESKIPFRAME,m_Options.m_iShakeSkipFrame);

	//WriteConfigString(SFRSPEC,m_Options.strSpecROIs);
	WriteConfigString(STATIONNAME,m_Options.m_StationName);
	WriteConfigInt(SFRBURNRATE,m_Options.m_iSFRBurnRate);

	WriteConfigInt(_T("SFR_TEST_FREQ"), pDlg->m_sfrFreq);
	WriteConfigInt(_T("m_BchekSFRSpec"), pDlg->m_BchekSFRSpec);
	
	WriteConfigInt(_T("m_BSaveSFR_Acut"), pDlg->m_BSaveSFR_Acut);
	
	WriteConfigInt(_T("m_BSFRSaveDataOffset"), pDlg->m_BSFRSaveDataOffset);
	WriteConfigInt(_T("m_B40cmSFRCheckData"), pDlg->m_B40cmSFRCheckData);
	WriteConfigInt(_T("m_B60cmSFRCheckData"), pDlg->m_B60cmSFRCheckData);
	WriteConfigInt(_T("m_B120cmSFRCheckData"), pDlg->m_B120cmSFRCheckData);

	
	WriteConfigString(_T("m_StrSpecSFR_FIRST"),pDlg->m_StrSpecSFR_FIRST);
	WriteConfigString(_T("m_StrSpecSFR_SECOND"),pDlg->m_StrSpecSFR_SECOND);
	WriteConfigInt(_T("m_BLC"),pDlg->m_BLC);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR FF_SFR_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	for (int i = 0; i < m_iSFRROICount; i++)
	{
		header += m_stSpec[i].ROIName;
		header += _T(",");
	}
	header += _T("CTC8,CTC6,CTC8_H,CTC8_V,CTC6_H,CTC6_V");
	return header;	
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR FF_SFR_TestItem::GetReportLowLimit()
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
LPCTSTR FF_SFR_TestItem::GetReportHightLimit()
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
LPCTSTR FF_SFR_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");

	CString temp;
	for (int index = 0; index < m_iSFRROICount; index++)
	{
		temp.Format(_T("%f,"),m_dSFRValue[index]);
		Content += temp;
	}
	temp.Format(_T("%f,%f,%f,%f,%f,%f"),m_fCTC8,m_fCTC6,m_fCTC8_H,m_fCTC8_V,m_fCTC6_H,m_fCTC6_V);
	Content += temp;
	return Content;
}

// BOOL FF_SFR_TestItem::CheckFinalSpec()
// {
// 	//卡规格
// 	BOOL bPass = TRUE;		
// 	for (int index = 0; index < m_iSFRROICount; index++)
// 	{
// 		if (pDlg->m_SpecShiftEn_0F)
// 		{
// 			if (pDlg->m_SpcShift_0F > 0.03)
// 			{
// 				bPass = FALSE;
// 				m_errMsg = m_cfgName;
// 				m_errMsg += _T(" SFR Spec shift超过3个点!");
// 				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
// 				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
// 				return bPass;
// 			}
// 			float SFRValue_Diff = m_stSpec[index].SFRSpec - m_dSFRValue[index];
// 			if (SFRValue_Diff > pDlg->m_SpcShift_0F)
// 			{
// 				bPass = FALSE;
// 				m_errMsg = m_cfgName;
// 				m_errMsg += _T(" 单点不良");
// 				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
// 				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
// 				return bPass;
// 			}
// 			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_0F)
// 			{
// 				m_dSFRValue[index] += pDlg->m_SpcShift_0F;
// 			}
// 			else
// 			{
// 				;
// 			}
// 		}
// 
// 		else
// 		{
// 			if (m_dSFRValue[index] < m_stSpec[index].SFRSpec)
// 			{
// 				bPass = FALSE;
// 				m_errMsg = m_cfgName;
// 				m_errMsg += _T(" SFR单点不良");
// 				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
// 				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
// 				return bPass;
// 			}
// 		}
// 	}
// 
// 	//卡CN	
// 	//0~1 CEN                2~9    0.6F              10~17   0.8F
// 	double dMax8 = 0;
// 	double dMin8 = 1 ;
// 	double dMax6 = 0;
// 	double dMin6 = 1 ;
// 
// 	double dMax8_H = 0;
// 	double dMin8_H = 1 ;
// 	double dMax8_V = 0;
// 	double dMin8_V = 1 ;
// 
// 	double dMax6_H = 0;
// 	double dMin6_H = 1 ;				
// 	double dMax6_V = 0;
// 	double dMin6_V = 1 ;
// 
// 	int startsfr6f = 0;
// 	int endsfr6f  = 0;
// 	int startsfr8f = 0;
// 	int endsfr8f  = 0;
// 
// 	if (((pDlg->m_bTestCenter+pDlg->m_bTestF6+pDlg->m_bTestF8)==3)&&(pDlg->m_bTestF4==0))
// 	{
// 		startsfr6f = 2;
// 		endsfr6f  = 9;
// 		startsfr8f = 10;
// 		endsfr8f  = 17;
// 	}
// 	else if ((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF6+pDlg->m_bTestF8)==4)
// 	{
// 		startsfr6f = 10;
// 		endsfr6f  = 17;
// 		startsfr8f = 18;
// 		endsfr8f  = 25;
// 	}
// 	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF8)==2)&&((pDlg->m_bTestF4+pDlg->m_bTestF6)==0))
// 	{
// 		startsfr6f = 0;
// 		endsfr6f  = 0;
// 		startsfr8f = 2;
// 		endsfr8f  = 9;
// 	}
// 	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF6)==2)&&((pDlg->m_bTestF4+pDlg->m_bTestF8)==0))
// 	{
// 		startsfr6f = 2;
// 		endsfr6f  = 9;
// 		startsfr8f = 0;
// 		endsfr8f  = 0;
// 	}
// 	else
// 	{
// 		startsfr6f = 0;
// 		endsfr6f  = 0;
// 		startsfr8f = 0;
// 		endsfr8f  = 0;
// 	}
// 	//0.8F
// 	for (int index = startsfr8f; index <= endsfr8f ; index++)
// 	{
// 		double SFRtemp = m_dSFRValue[index];
// 		if (SFRtemp > dMax8)	{	dMax8 =SFRtemp;    }
// 		if (SFRtemp < dMin8)	{	dMin8 = SFRtemp;	}
// 	}
// 	//0.8视场分方向
// 	for (int index = startsfr8f; index < endsfr8f ; index+=2)
// 	{
// 		double SFRtemp_H = m_dSFRValue[index];
// 		double SFRtemp_V = m_dSFRValue[index+1];
// 		if (SFRtemp_H > dMax8_H)	{	dMax8_H =SFRtemp_H;  }
// 		if (SFRtemp_H < dMin8_H)	{	dMin8_H = SFRtemp_H;	}
// 		if (SFRtemp_V > dMax8_V)	{	dMax8_V =SFRtemp_V;  }
// 		if (SFRtemp_V < dMin8_V)	{	dMin8_V = SFRtemp_V;	}
// 	}
// 	//0.6视场
// 	for (int index =startsfr6f; index <= endsfr6f ; index++)
// 	{
// 		double SFRtemp = m_dSFRValue[index];
// 		if (SFRtemp > dMax6)	{	dMax6 =SFRtemp;	}
// 		if (SFRtemp < dMin6)	{	dMin6 = SFRtemp;	}
// 	}
// 	//0.6视场分方向
// 	for (int index = startsfr6f; index < endsfr6f ; index+=2)
// 	{
// 		double SFRtemp_H = m_dSFRValue[index];
// 		double SFRtemp_V = m_dSFRValue[index+1];
// 		if (SFRtemp_H > dMax6_H)	{	dMax6_H =SFRtemp_H;  }
// 		if (SFRtemp_H < dMin6_H)	{	dMin6_H = SFRtemp_H;	}
// 		if (SFRtemp_V > dMax6_V)	{	dMax6_V =SFRtemp_V;  }
// 		if (SFRtemp_V < dMin6_V)	{	dMin6_V = SFRtemp_V;	}
// 	}
// 	if (dMax8 == 0)	{	dMax8 = 1;	}
// 	if (dMax6== 0)	{	dMax6 = 1;	}
// 	if (dMax8_H == 0)	{	dMax8_H = 1;	}
// 	if (dMax6_H == 0)	{	dMax6_H = 1;	}
// 	if (dMax8_V == 0)	{	dMax8_V = 1;	}
// 	if (dMax6_V == 0)	{	dMax6_V = 1;	}
// 
// 	m_fCTC6 = (dMax6 - dMin6)/dMax6 * 100;
// 	m_fCTC8 = (dMax8 - dMin8)/dMax8 * 100;
// 	m_fCTC6_H = (dMax6_H - dMin6_H)/dMax6_H * 100;
// 	m_fCTC8_H = (dMax8_H - dMin8_H)/dMax8_H * 100;
// 	m_fCTC6_V = (dMax6_V - dMin6_V)/dMax6_V * 100;
// 	m_fCTC8_V = (dMax8_V - dMin8_V)/dMax8_V * 100;			
// 
// 	CString temp;
// 	temp.Format(_T("CTC8:%.4f"),m_fCTC8);
// 	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
// 	temp.Format(_T("CTC6:%.4f"),m_fCTC6);
// 	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
// 
// 	temp.Format(_T("CTC8_H:%.4f"),m_fCTC8_H);
// 	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
// 	temp.Format(_T("CTC6_H:%.4f"),m_fCTC6_H);
// 	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
// 
// 	temp.Format(_T("CTC8_V:%.4f"),m_fCTC8_V);
// 	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
// 	temp.Format(_T("CTC6_V:%.4f"),m_fCTC6_V);
// 	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
// 
// 	if ((m_fCTC8 > m_Options.m_fCTC8) || (m_fCTC6 > m_Options.m_fCTC6) ||
// 		(m_fCTC8_H > m_Options.m_fCTC8_H) || (m_fCTC6_H > m_Options.m_fCTC6_H) || 
// 		(m_fCTC8_V > m_Options.m_fCTC8_V) || (m_fCTC6_V > m_Options.m_fCTC6_V) )
// 	{	
// 		//swprintf_s(this->Res.errMsg,512,L"%s CTC NG",this->Param.StationName);
// 		m_errMsg = m_cfgName;
// 		m_errMsg += _T(" CTC NG");
// 		m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
// 		m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
// 		bPass = FALSE;
// 	}
// 	//////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 	return bPass;
// }

BOOL FF_SFR_TestItem::CheckFinalSpec()
{
	//卡规格
	BOOL bPass = TRUE;
	int startsfr0f = 0;
	int endsfr0f = 1;

	int startsfr4f = 0;
	int endsfr4f = 0;
	int startsfr6f = 0;
	int endsfr6f  = 0;
	int startsfr8f = 0;
	int endsfr8f  = 0;

	if (((pDlg->m_bTestCenter+pDlg->m_bTestF6+pDlg->m_bTestF8)==3)&&(pDlg->m_bTestF4==0))
	{
		startsfr4f = 0;
		endsfr4f = 0;
		startsfr6f = 2;
		endsfr6f  = 9;
		startsfr8f = 10;
		endsfr8f  = 17;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF8)==3)&&(pDlg->m_bTestF6==0))
	{
		startsfr4f = 2;
		endsfr4f = 9;
		startsfr6f = 0;
		endsfr6f  = 0;
		startsfr8f = 10;
		endsfr8f  = 17;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF6)==3)&&(pDlg->m_bTestF8==0))
	{
		startsfr4f = 2;
		endsfr4f = 9;
		startsfr6f = 10;
		endsfr6f  = 17;
		startsfr8f = 0;
		endsfr8f  = 0;
	}
	else if ((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF6+pDlg->m_bTestF8)==4)
	{
		startsfr4f = 2;
		endsfr4f = 9;
		startsfr6f = 10;
		endsfr6f  = 17;
		startsfr8f = 18;
		endsfr8f  = 25;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF8)==2)&&((pDlg->m_bTestF4+pDlg->m_bTestF6)==0))
	{
		startsfr4f = 0;
		endsfr4f = 0;
		startsfr6f = 0;
		endsfr6f  = 0;
		startsfr8f = 2;
		endsfr8f  = 9;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF6)==2)&&((pDlg->m_bTestF4+pDlg->m_bTestF8)==0))
	{
		startsfr4f = 0;
		endsfr4f = 0;
		startsfr6f = 2;
		endsfr6f  = 9;
		startsfr8f = 0;
		endsfr8f  = 0;
	}
	else if (((pDlg->m_bTestCenter+pDlg->m_bTestF4)==2)&&((pDlg->m_bTestF6+pDlg->m_bTestF8)==0))
	{
		startsfr4f = 2;
		endsfr4f = 9;
		startsfr6f = 0;
		endsfr6f  = 0;
		startsfr8f = 0;
		endsfr8f  = 0;
	}

	else
	{
		startsfr4f = 0;
		endsfr4f = 0;
		startsfr6f = 0;
		endsfr6f  = 0;
		startsfr8f = 0;
		endsfr8f  = 0;
	}

	for (int index = 0; index < m_iSFRROICount; index++)
	{
		if (pDlg->m_bTestCenter && pDlg->m_SpcShiftEn_0F && index>= startsfr0f && index <= endsfr0f)
		{
			if (pDlg->m_SpcShift_0F > 0.2)
			{
				m_errMsg = m_cfgName;
				m_errMsg += _T(" 0视场Spec Shift过大!");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
				return FALSE;
			}
			float SFRValue_Diff = m_stSpec[index].SFRSpec - m_dSFRValue[index];//m_stSpec[index].SFRSpec
			if (SFRValue_Diff > pDlg->m_SpcShift_0F)
			{
				bPass = FALSE;
				m_errMsg = m_cfgName;
				m_errMsg += _T(" 单点不良");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_0F)
			{
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_0F;//中心视场补偿
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

		else if (pDlg->m_bTestF4 && pDlg->m_SpcShiftEn_4F && index >=  startsfr4f && index <= endsfr4f)
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
				m_errMsg += _T(" 单点不良");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_4F)
			{
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_4F;//0.4视场补偿
				//m_dSFRValue_Offset[index] += SFRValue_Diff;
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

		else if (pDlg->m_bTestF6 && pDlg->m_SpcShiftEn_6F && index >=  startsfr6f && index <= endsfr6f)
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
				m_errMsg += _T(" 单点不良");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_6F)
			{
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_6F;//0.6视场
				//m_dSFRValue_Offset[index] += SFRValue_Diff;
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

		else if (pDlg->m_bTestF8 && pDlg->m_SpcShiftEn_8F && index >=  startsfr8f && index <= endsfr8f)
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
				m_errMsg += _T(" 单点不良");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
			}
			else if (SFRValue_Diff > 0 && SFRValue_Diff <= pDlg->m_SpcShift_8F)
			{
				m_dSFRValue[index] += SFRValue_Diff;//pDlg->m_SpcShift_8F;
				//m_dSFRValue_Offset[index] += SFRValue_Diff;
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
				m_errMsg += _T(" 单点不良");
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
				m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
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

	BOOL bCTC_Pass = TRUE;
	BOOL DataChange = FALSE;
	BOOL bPass_CTC_6F = TRUE;
	BOOL bPass_CTC_6F_H = TRUE;
	BOOL bPass_CTC_6F_V = TRUE;

	BOOL bPass_CTC_8F = TRUE;
	BOOL bPass_CTC_8F_H = TRUE;
	BOOL bPass_CTC_8F_V = TRUE;

	BOOL Threshhold = FALSE;
	BYTE *SFR_Shift_Cnt = new BYTE[m_iSFRROICount];
	memset(SFR_Shift_Cnt,0,m_iSFRROICount);
	do 
	{
		DataChange = FALSE;
		bPass_CTC_6F = TRUE;
		BOOL bPass_CTC_6F_H = TRUE;
		BOOL bPass_CTC_6F_V = TRUE;
		bPass_CTC_8F = TRUE;
		BOOL bPass_CTC_8F_H = TRUE;
		BOOL bPass_CTC_8F_V = TRUE;

		double dMax8 = 0;
		double dMin8 = 1 ;
		double dMax6 = 0;
		double dMin6 = 1 ;

		double dMax8_H = 0;
		double dMin8_H = 1 ;
		double dMax8_V = 0;
		double dMin8_V = 1 ;

		double dMax6_H = 0;
		double dMin6_H = 1 ;				
		double dMax6_V = 0;
		double dMin6_V = 1 ;

		double dMax8_fact = 0;
		double dMin8_fact = 1 ;
		double dMax6_fact = 0;
		double dMin6_fact = 1 ;

		double dMax8_H_fact = 0;
		double dMin8_H_fact = 1 ;
		double dMax8_V_fact = 0;
		double dMin8_V_fact = 1 ;

		double dMax6_H_fact = 0;
		double dMin6_H_fact = 1 ;				
		double dMax6_V_fact = 0;
		double dMin6_V_fact = 1;


		//0.8F
		for (int index = startsfr8f; index <= endsfr8f ; index++)
		{
			double SFRtemp = m_dSFRValue[index];
			double SFRtemp_fact = m_dSFRValue_Fact[index];
			if (SFRtemp > dMax8)	
			{	
				dMax8 =SFRtemp; 
				Index_Max_8F = index;
			}
			if (SFRtemp < dMin8)	
			{	
				dMin8 = SFRtemp;
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
				dMax8_H =SFRtemp_H;
				Index_Max_H_8F = index;
			}
			if (SFRtemp_H < dMin8_H)
			{	
				dMin8_H = SFRtemp_H;	
				Index_Min_H_8F = index;
			}
			if (SFRtemp_V > dMax8_V)
			{	
				dMax8_V =SFRtemp_V; 
				Index_Max_V_8F = index+1;
			}
			if (SFRtemp_V < dMin8_V)	
			{	
				dMin8_V = SFRtemp_V;	
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
				dMax6 =SFRtemp;
				Index_Max_6F = index;
			}
			if (SFRtemp < dMin6)	
			{	
				dMin6 = SFRtemp;	
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
				dMax6_H =SFRtemp_H; 
				Index_Max_H_6F = index;
			}
			if (SFRtemp_H < dMin6_H)	
			{
				dMin6_H = SFRtemp_H;	
				Index_Min_H_6F = index;
			}
			if (SFRtemp_V > dMax6_V)	
			{
				dMax6_V =SFRtemp_V; 
				Index_Max_V_6F = index+1;
			}
			if (SFRtemp_V < dMin6_V)	
			{	
				dMin6_V = SFRtemp_V;	
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
		if (dMax8 == 0)	{	dMax8 = 1;	}
		if (dMax6== 0)	{	dMax6 = 1;	}
		if (dMax8_H == 0)	{	dMax8_H = 1;	}
		if (dMax6_H == 0)	{	dMax6_H = 1;	}
		if (dMax8_V == 0)	{	dMax8_V = 1;	}
		if (dMax6_V == 0)	{	dMax6_V = 1;	}

		if (dMax8_fact == 0)	{	dMax8_fact = 1;	}
		if (dMax6_fact== 0)	{	dMax6_fact = 1;	}
		if (dMax8_H_fact == 0)	{	dMax8_H_fact = 1;	}
		if (dMax6_H_fact == 0)	{	dMax6_H_fact = 1;	}
		if (dMax8_V_fact == 0)	{	dMax8_V_fact = 1;	}
		if (dMax6_V_fact == 0)	{	dMax6_V_fact = 1;	}

		m_fCTC6 = (dMax6 - dMin6)/dMax6 * 100;
		m_fCTC8 = (dMax8 - dMin8)/dMax8 * 100;
		m_fCTC6_H = (dMax6_H - dMin6_H)/dMax6_H * 100;
		m_fCTC8_H = (dMax8_H - dMin8_H)/dMax8_H * 100;
		m_fCTC6_V = (dMax6_V - dMin6_V)/dMax6_V * 100;
		m_fCTC8_V = (dMax8_V - dMin8_V)/dMax8_V * 100;			

		m_fCTC6_Fact = (dMax6_fact - dMin6_fact)/dMax6_fact * 100;
		m_fCTC8_Fact = (dMax8_fact - dMin8_fact)/dMax8_fact * 100;
		m_fCTC6_H_Fact = (dMax6_H_fact - dMin6_H_fact)/dMax6_H_fact * 100;
		m_fCTC8_H_Fact = (dMax8_H_fact - dMin8_H_fact)/dMax8_H_fact * 100;
		m_fCTC6_V_Fact = (dMax6_V_fact - dMin6_V_fact)/dMax6_V_fact * 100;
		m_fCTC8_V_Fact = (dMax8_V_fact - dMin8_V_fact)/dMax8_V_fact * 100;

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
					return FALSE;
				}
				float _8F_Diff = dMax8 - m_dSFRValue[Index_Min_8F] - 0.01;
				if (_8F_Diff > 0)
				{
					if (SFR_Shift_Cnt[Index_Min_8F]==0)
					{
						if (_8F_Diff >= pDlg->m_SpcShift_8F_CTC)
						{
							if ((dMax8-(m_dSFRValue[Index_Min_8F]+pDlg->m_SpcShift_8F_CTC))/dMax8*100<m_Options.m_fCTC8)
							{
								m_dSFRValue[Index_Min_8F] += pDlg->m_SpcShift_8F_CTC;	
							}
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
					return FALSE;
				}
				float _6F_Diff = dMax6 - m_dSFRValue[Index_Min_6F] - 0.01;
				if (_6F_Diff > 0)
				{
					if ( SFR_Shift_Cnt[Index_Min_6F] == 0)
					{
						if (_6F_Diff >= pDlg->m_SpcShift_6F_CTC)
						{
							if ((dMax6-(m_dSFRValue[Index_Min_6F]+pDlg->m_SpcShift_6F_CTC))/dMax6*100<m_Options.m_fCTC6)
							{
								m_dSFRValue[Index_Min_6F] += pDlg->m_SpcShift_6F_CTC;
							}
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
					return FALSE;
				}
				if ((Index_Min_H_6F != Index_Min_6F) || (Index_Min_H_6F == Index_Min_6F && bPass_CTC_6F==TRUE) )
				{
					float _6F_H_Diff = dMax6_H - m_dSFRValue[Index_Min_H_6F] - 0.01;
					if (_6F_H_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_H_6F]==0)
						{
							if (_6F_H_Diff >= pDlg->m_SpcShift_6F_CTC)
							{
								if ((dMax6_H-(m_dSFRValue[Index_Min_H_6F]+pDlg->m_SpcShift_6F_CTC))/dMax6_H*100<m_Options.m_fCTC6_H)
								{
									m_dSFRValue[Index_Min_H_6F] += pDlg->m_SpcShift_6F_CTC;
								}
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
					return FALSE;
				}
				if ((Index_Min_V_6F != Index_Min_6F) || (Index_Min_V_6F == Index_Min_6F && bPass_CTC_6F == TRUE))
				{
					float _6F_V_Diff = dMax6_V - m_dSFRValue[Index_Min_V_6F] - 0.01;
					if (_6F_V_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_V_6F]==0)
						{
							if (_6F_V_Diff >= pDlg->m_SpcShift_6F_CTC)
							{
								if (((dMax6_V-(m_dSFRValue[Index_Min_V_6F]+pDlg->m_SpcShift_6F_CTC))/dMax6_V*100)<m_Options.m_fCTC6_V)
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
					return FALSE;
				}
				
				if ((Index_Min_H_8F != Index_Min_8F) || (Index_Min_H_8F == Index_Min_8F && bPass_CTC_8F == TRUE) )
				{
					float _8F_H_Diff = dMax8_H - m_dSFRValue[Index_Min_H_8F] - 0.01;
					if (_8F_H_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_H_8F]==0)
						{
							if (_8F_H_Diff >= pDlg->m_SpcShift_8F_CTC)
							{
								if (((dMax8_H-(m_dSFRValue[Index_Min_H_8F]+pDlg->m_SpcShift_8F_CTC))/dMax8_H*100)<m_Options.m_fCTC8_H)
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
					return FALSE;
				}
				if ((Index_Min_V_8F != Index_Min_8F) || (Index_Min_V_8F == Index_Min_8F && bPass_CTC_8F == TRUE))
				{
					float _8F_V_Diff = dMax8_V - m_dSFRValue[Index_Min_V_8F] - 0.01;
					if (_8F_V_Diff > 0)
					{
						if (SFR_Shift_Cnt[Index_Min_V_8F]==0)
						{
							if (_8F_V_Diff >= pDlg->m_SpcShift_8F_CTC)
							{
								if (((dMax8_V-(m_dSFRValue[Index_Min_V_8F]+pDlg->m_SpcShift_8F_CTC))/dMax8_V*100)<m_Options.m_fCTC8_V)
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
		if (bPass_CTC_6F == FALSE || bPass_CTC_6F_H == FALSE || bPass_CTC_6F_V == FALSE || bPass_CTC_8F == FALSE || bPass_CTC_8F_H == FALSE || bPass_CTC_8F_V == FALSE)
		{
			bCTC_Pass = FALSE;
		}
		else
		{
			bCTC_Pass = TRUE;
		}

	} while ((DataChange == TRUE) ||(Threshhold == FALSE && bCTC_Pass==FALSE && bPass == TRUE && ((pDlg->m_SpcShift_CTC_En_8F&&pDlg->m_SpcShift_8F_CTC>0) || (pDlg->m_SpcShift_CTC_En_6F&&pDlg->m_SpcShift_6F_CTC>0))));
	
	SAFE_DELETE_ARRAY(SFR_Shift_Cnt);

	if (bCTC_Pass == FALSE)
	{
		m_errMsg = m_cfgName;
		m_errMsg += _T(" CTC NG");
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,m_errMsg);
		m_pInterface->Ctrl_Addlog(CamID,m_errMsg,COLOR_RED,LOGHEIGHT);
		bPass = FALSE;
	}	

	CString temp;
	temp.Format(_T("CTC8:%.4f"),m_fCTC8);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC6:%.4f"),m_fCTC6);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);

	temp.Format(_T("CTC8_H:%.4f"),m_fCTC8_H);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC6_H:%.4f"),m_fCTC6_H);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);

	temp.Format(_T("CTC8_V:%.4f"),m_fCTC8_V);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,LOGHEIGHT);
	temp.Format(_T("CTC6_V:%.4f"),m_fCTC6_V);
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

BOOL FF_SFR_TestItem::SaveCheckData()
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
		CString strHeader = _T("SensorID,Time,");
		for (int i = 0; i < m_iSFRROICount; i++)
		{
			strHeader += m_stSpec[i].ROIName;
			strHeader += _T(",");
		}
		strHeader += _T("\n");
		DataFile.WriteString(strHeader);
	}
	DataFile.SeekToEnd();
	CString TTT = m_pInterface->Sensor_GetCamFuseID(CamID);
	CString strContent = _T("");
	strContent.Format(_T("%s,%02d:%02d:%02d,\n"),m_pInterface->Sensor_GetCamFuseID(CamID),
		st.wHour,st.wMinute,st.wSecond);
	DataFile.WriteString(strContent);
	
	for (int i = 0; i < m_Options.m_Mid_TestTime; i++)
	{
		strContent.Format(_T(",,"));
		CString temp;
		
		for (int index = 0; index < m_iSFRROICount; index++)
		{
			temp.Format(_T("%.4f,"),SFRAll[i][index]);
			strContent += temp;
		}
		strContent += _T("\n");
		DataFile.WriteString(strContent);
	}
	DataFile.Close();
	return TRUE;
}

float FF_SFR_TestItem::GetSFRValue(unsigned char *_InImageBuffer,ROIInfo roiinfo,int ROISize,wchar_t *stringinfo,int mm)
{
	int Width = img.imgwidth;
	int Height = img.imgheight;
	float sfrvalue = 0.0;
	////截取ROI,存在ROIP10里
	BYTE* ROIP10 = new BYTE[ROISize*ROISize*2];
	memset(ROIP10,0,ROISize*ROISize*2);
	PCutImageBuffer(_InImageBuffer,Width,Height,2,1,roiinfo.LT.x,roiinfo.LT.y,ROISize,ROISize,ROIP10);	

	// Channel Balance
	BYTE* BlockBuffer = new BYTE[ROISize*ROISize*2];
	memset(BlockBuffer,0,ROISize*ROISize*2);
	int outformat = m_pInterface->Ctrl_GetOutPutFormat(CamID);
	GetBayerType(m_pInterface->Ctrl_GetOutPutFormat(CamID),roiinfo.LT.x,roiinfo.LT.y,outformat);
	RawToGrayscaleImage_Raw10(ROIP10,outformat,ROISize,ROISize,BlockBuffer);
	// Save ROI Images
	SAFE_DELETE_ARRAY(ROIP10);	
	USHORT *pROIBuffer = new USHORT[ROISize*ROISize];
	memset(pROIBuffer,0,ROISize*ROISize*sizeof(USHORT));
	for (int y=0; y < ROISize;y++)
	{
		for (int x=0; x<ROISize;x++)
		{
			pROIBuffer[y*ROISize+x] = BlockBuffer[(y*ROISize+x)*2] | (BlockBuffer[(y*ROISize+x)*2 + 1] << 8);
		}
	}
	
	CSFRData* sfrArray = new CSFRData[ROISize];
	double freq = 1.0f / (pDlg->m_sfrFreq * 2);
    sfrvalue = SFRMatbmpTest(pROIBuffer,freq,sfrArray,ROISize,ROISize);//0.125
	if(sfrvalue>1) {sfrvalue=1;}
	if(sfrvalue<0.007) {sfrvalue=0.01;}		
	if(_isnan(sfrvalue))  {sfrvalue = 1000;}

	SAFE_DELETE_ARRAY(BlockBuffer);
	SAFE_DELETE_ARRAY(sfrArray);
    SAFE_DELETE_ARRAY(pROIBuffer);

	return sfrvalue;
}

bool FF_SFR_TestItem::PCutImageBuffer(unsigned char *_InImageBuffer,int _Width,int _Height,UINT _ByteDepth,UINT _ColorChannel, int _mStartX,int _mStartY,int _CutWidth,int _CutHeight,unsigned char *_OutImageBuffer)
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

void FF_SFR_TestItem::GetBayerType(int iImageType,int iStartX,int iStartY,int& outformat)
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

void FF_SFR_TestItem::RawToGrayscaleImage_Raw10(unsigned char *m_pBuff,int outformat,int m_width,int m_height,unsigned char *pOutRaw10)
{
	USHORT* m_pBuff2 = new USHORT[m_width*m_height];
	USHORT* pOut     = new USHORT[m_width*m_height];
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
	delete []pOut;
}

BOOL FF_SFR_TestItem::bSaveBmpFile(CString sfilename, BYTE *pBuffer, UINT width, UINT height)
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


BOOL FF_SFR_TestItem::AnalysisROISpec()
{			
	char temp[MAXSTRLEN] = {0};
	int index = 0;

	if ((pDlg->m_bTestCenter+pDlg->m_bTestF4+pDlg->m_bTestF6+pDlg->m_bTestF8) == 0)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("没有选择要测试的ROI"),COLOR_RED,220);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("没有选择要测试的ROI"));
		m_bROIConfigureError = true;
		return FALSE;
	}

	if (pDlg->m_bTestCenter)
	{
		wcscpy(m_stSpec[index].ROIName,_T("CEN_H"));
		m_stSpec[index].SFRSpec = m_Options.m_CenH_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("CEN_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_CenV_Spec;
		index++;
	}
	
	if (pDlg->m_bTestF4)
	{
		//左上
		wcscpy(m_stSpec[index].ROIName,_T("TL4_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F4H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("TL4_V"));
		m_stSpec[index].SFRSpec =   m_Options.m_F4V_Spec;
		index++;
		//右上
		wcscpy(m_stSpec[index].ROIName,_T("TR4_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F4H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("TR4_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F4V_Spec;
		index++;
		//右下
		wcscpy(m_stSpec[index].ROIName,_T("BR4_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F4H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("BR4_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F4V_Spec;
		index++;
		//左下
		wcscpy(m_stSpec[index].ROIName,_T("BL4_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F4H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("BL4_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F4V_Spec;
		index++;
	}

	if (pDlg->m_bTestF6)
	{
		//左上
		wcscpy(m_stSpec[index].ROIName,_T("TL6_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F6H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("TL6_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F6V_Spec;
		index++;
		//右上
		wcscpy(m_stSpec[index].ROIName,_T("TR6_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F6H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("TR6_V"));
		m_stSpec[index].SFRSpec =   m_Options.m_F6V_Spec;
		index++;
		//右下
		wcscpy(m_stSpec[index].ROIName,_T("BR6_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F6H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("BR6_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F6V_Spec;
		index++;
		//左下
		wcscpy(m_stSpec[index].ROIName,_T("BL6_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F6H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("BL6_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F6V_Spec;
		index++;
	}

	if (pDlg->m_bTestF8)
	{
		//左上
		wcscpy(m_stSpec[index].ROIName,_T("TL8_H"));
		m_stSpec[index].SFRSpec =  m_Options.m_F8H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("TL8_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F8V_Spec;
		index++;
		//右上
		wcscpy(m_stSpec[index].ROIName,_T("TR8_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F8H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("TR8_V"));
		m_stSpec[index].SFRSpec = m_Options.m_F8V_Spec;
		index++;
		//右下
		wcscpy(m_stSpec[index].ROIName,_T("BR8_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F8H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("BR8_V"));
		m_stSpec[index].SFRSpec =  m_Options.m_F8V_Spec;
		index++;
		//左下
		wcscpy(m_stSpec[index].ROIName,_T("BL8_H"));
		m_stSpec[index].SFRSpec = m_Options.m_F8H_Spec;
		index++;
		wcscpy(m_stSpec[index].ROIName,_T("BL8_V"));
		m_stSpec[index].SFRSpec =   m_Options.m_F8V_Spec;
		index++;
	}

	m_iSFRROICount = index;

	return TRUE;
}

void FF_SFR_TestItem::StringSplit( const wstring & str, vector<wstring> & tokens, const wstring & delimiters)  
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

BOOL FF_SFR_TestItem::AutoROI(OFAutoROIOut &ofDestDataOut)
{	
	//使用BMP图来mark点和ROI
	BYTE* BMPBuffer=new BYTE[img.imgwidth*img.imgheight*3];
	if (BMPBuffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("内存分配失败"),COLOR_BLUE,200);
		return FALSE;
	}
	memset(BMPBuffer,0,img.imgwidth*img.imgheight*3);
	memcpy(BMPBuffer,img.RGB24_buffer,img.imgwidth*img.imgheight*3);	
	OFAutoROIIn ofSrcDataIn;
	ofSrcDataIn.ofSrcBuffer=BMPBuffer;
	ofSrcDataIn.ofHeight = img.imgheight;
	ofSrcDataIn.ofWidth = img.imgwidth;
	ofSrcDataIn.ofImageType = BMP_IMAGE;
	ofSrcDataIn.ofOutFormat  = OUTFORMAT_FLIP_;
	char ErrorMsg[512]={0};
	BOOL bAutoROI = FALSE;
	//bSaveBmpFile(_T("D:\\AutoROI.bmp"),BMPBuffer,img.imgwidth,img.imgheight);
	
	if (m_bFocusing)
	{
		if (m_Options.bEngineerMode)
		{
			bAutoROI = fnAutoROISelect(&ofSrcDataIn, &ofDestDataOut,ErrorMsg);
		}
		else
		{
			bAutoROI = fnAutoROICenter(&ofSrcDataIn, &ofDestDataOut,ErrorMsg);
		}
	}else
	{
		bAutoROI = fnAutoROISelect(&ofSrcDataIn, &ofDestDataOut,ErrorMsg);
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
BOOL FF_SFR_TestItem::SFRTest(OFAutoROIOut ofDestDataOut,OfMarker* _ROIInfo,int &iROICount)
{	
	//**************************计算SFR********************************//
	int iROINum = ofDestDataOut.ofROInumber;
//	DebugInfo("------------------ROINum=%d------------------",iROINum);
	BYTE* P10Buffer = new BYTE[img.imgwidth*img.imgheight*2];	
	memset(P10Buffer,0,img.imgwidth*img.imgheight*2);	
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
		tempname.Format(_T("_%d.raw"),(iTestTimes+1));
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
	for (int i = 0; i < iROINum; i++)
	{
		if ((ofDestDataOut.ofROIposition[i]._y1+ofDestDataOut.ofROIposition[i]._y2) == 0)
		{
			_sfrC = ofDestDataOut.ofROIposition[i]._C;
			_sfrD = ofDestDataOut.ofROIposition[i]._D;

			if (_sfrC+_sfrD)
			{
				nCount++;

				sfrvalue1 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH1,ofDestDataOut.ofROISize,L"H1",nCount);
				sfrvalue2 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH2,ofDestDataOut.ofROISize,L"H2",nCount);

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

				m_dSFRValue[nCount-1] = (sfrvalue1*_sfrD+sfrvalue2*_sfrC)/(_sfrC+_sfrD);
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
			sfrvalue1 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH1,ofDestDataOut.ofROISize,L"H1",nCount);
			sfrvalue2 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH2,ofDestDataOut.ofROISize,L"H2",nCount);
			sfrvalue3 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH3,ofDestDataOut.ofROISize,L"H3",nCount);				
			sfrvalue4 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIH4,ofDestDataOut.ofROISize,L"H4",nCount);

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

			m_dSFRValue[nCount-1] = ((sfrvalue1*_sfrD+sfrvalue2*_sfrC)*_sfrY2/(_sfrC+_sfrD)+(sfrvalue3*_sfrD+sfrvalue4*_sfrC)*_sfrY1/(_sfrC+_sfrD))/(_sfrY1+_sfrY2);
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

				sfrvalue1 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV1,ofDestDataOut.ofROISize,L"V1",nCount);
				sfrvalue2 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV2,ofDestDataOut.ofROISize,L"V2",nCount);

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
				m_dSFRValue[nCount-1] = (sfrvalue1*_sfrB+sfrvalue2*_sfrA)/(_sfrA+_sfrB);

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
			sfrvalue1 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV1,ofDestDataOut.ofROISize,L"V1",nCount);
			sfrvalue2 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV2,ofDestDataOut.ofROISize,L"V2",nCount);
			sfrvalue3 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV3,ofDestDataOut.ofROISize,L"V3",nCount);				
			sfrvalue4 = GetSFRValue(P10Buffer,ofDestDataOut.ofROIposition[i].ROIV4,ofDestDataOut.ofROISize,L"V4",nCount);

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

			m_dSFRValue[nCount-1] = ((sfrvalue1*_sfrB+sfrvalue2*_sfrA)*_sfrX2/(_sfrA+_sfrB)+(sfrvalue3*_sfrB+sfrvalue4*_sfrA)*_sfrX1/(_sfrA+_sfrB))/(_sfrX1+_sfrX2);
			//DebugInfo("this->Res.SFR[%d] = %.2f",nCount-1,m_dSFRValue[nCount-1] );
			//DebugInfo("SFR_V1 = %.2f, SFR_V2 = %.2f, SFR_V3 = %.2f, SFR_V4 = %.2f",sfrvalue1,sfrvalue2,sfrvalue3,sfrvalue4 );
			//DebugInfo("SFR_A = %.2f, SFR_B = %.2f, SFR_X1 = %.2f, SFR_X2 = %.2f",_sfrA,_sfrB,_sfrX1,_sfrX2);
		}
	}
	SAFE_DELETE_ARRAY(P10Buffer);
	iROICount = blockCount;

	return TRUE;
}

BOOL FF_SFR_TestItem::DrawText(OfMarker* pStROIInfo,int iROICount,int iBlockCount)
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

	COLORREF CurrentColor;
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
	memset(stROIPos,0,sizeof(OfMarker)*MAX_ROI_NUMBER);

	int nCount = 0;
	if (pDlg->m_bTestCenter)
	{
		stROIPos[nCount].x      = (int)(0.5 * ImgWidth);
		stROIPos[nCount++].y      = (int)(0.5 * ImgHeight);
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
			pDisplayWndMemDC->TextOut((cx1 - 160)*Xratio,(cy1 - 105)*Yratio,str);
		}else
		{
			pDisplayWndMemDC->TextOut((cx1 - 160)*Xratio,(cy2 + 5 )*Yratio,str);
		}

		CPen pen1(PS_SOLID,2,COLOR_YELLOW);
	    pDisplayWndMemDC->SelectObject(&pen1);
	    pDisplayWndMemDC->MoveTo( (int)((stROIPos[i].x - 4)*Xratio),((stROIPos[i].y - 4)*Yratio));
	    pDisplayWndMemDC->LineTo((int)((stROIPos[i].x + 4)*Xratio),((stROIPos[i].y + 4)*Yratio));
		pDisplayWndMemDC->MoveTo( (int)((stROIPos[i].x - 4)*Xratio),((stROIPos[i].y + 4)*Yratio));
		pDisplayWndMemDC->LineTo((int)((stROIPos[i].x + 4)*Xratio),((stROIPos[i].y - 4)*Yratio));
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

void FF_SFR_TestItem::Convert2Char(char* cString,int iMaxLength,CString strSting)
{
	memset(cString,0,iMaxLength);
	int length = strSting.GetLength();
	WideCharToMultiByte(CP_ACP, 0, strSting.GetBuffer(length), length, cString, 1024, 0, 0);
}


void FF_SFR_TestItem::SaveData_InFact(bool bResult)
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
		sfilename.Format(_T("%s\\Cam%dSFRData_Actually%s.csv"),strLog,CamID,localtime.Mid(0,8));
		//stSpec.Format(_T("%s\\Cam%dSpecData_%s.csv"),strLog,CamID,localtime.Mid(0,8));
		break;
	}
	char fileName[1024] = {0};	
	Convert2Char(fileName,1024,sfilename);
	fp = fopen(fileName, "a");
	if(fp)
	fclose(fp);

	//CStdioFile MyFile(sfilename, CFile::modeWrite | CFile::typeText); 
	CStdioFile MyFile;
	if(MyFile.Open(sfilename, CFile::modeWrite | CFile::typeText))
	{
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
	if(pDlg->m_B40cmSFRCheckData)
	{
		strSFRStationName = _T("SFR_Data_40CM");
	}
	if(pDlg->m_B60cmSFRCheckData)
	{
		strSFRStationName = _T("SFR_Data_60CM");
	}
	if(pDlg->m_B120cmSFRCheckData)
	{
		strSFRStationName = _T("SFR_Data_120CM");
	}
	//表中内容
	char clocaltime[512] = {0};
	ImageProc::ConvertWideToChar(localtime,clocaltime);
	char sensorID[512] = {0};
	ImageProc::ConvertWideToChar(m_pInterface->Sensor_GetCamFuseID(CamID),sensorID);
	m_pInterface->Ctrl_Addlog(CamID,m_pInterface->Sensor_GetCamFuseID(CamID),COLOR_BLUE,200);

	char str_StationName[512] = {0};
	ImageProc::ConvertWideToChar(strSFRStationName,str_StationName);
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
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("open file fail"),COLOR_BLUE,200);
	}
}

void FF_SFR_TestItem::SaveSFRData_Offset(bool bResult)
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
		sfilename.Format(_T("%s\\Cam%dSFRData_Offset%s.csv"),strLog,CamID,localtime.Mid(0,8));
		//stSpec.Format(_T("%s\\Cam%dSpecData_%s.csv"),strLog,CamID,localtime.Mid(0,8));
		break;
	}
	char fileName[1024] = {0};	
	Convert2Char(fileName,1024,sfilename);
	fp = fopen(fileName, "a");
	if(fp)
	fclose(fp);

	CStdioFile MyFile;  
	if(MyFile.Open(sfilename, CFile::modeWrite | CFile::typeText))
	{
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
	CString strSFRStationName(_T(""));
	if(pDlg->m_B40cmSFRCheckData)
	{
		strSFRStationName = _T("SFR_Data_40CM");
	}
	if(pDlg->m_B60cmSFRCheckData)
	{
		strSFRStationName = _T("SFR_Data_60CM");
	}
	if(pDlg->m_B120cmSFRCheckData)
	{
		strSFRStationName = _T("SFR_Data_120CM");
	}
	//表中内容
	char clocaltime[512] = {0};
	ImageProc::ConvertWideToChar(localtime,clocaltime);
	char sensorID[512] = {0};
	CString strtempFuseid(_T(""));
	ImageProc::ConvertWideToChar(m_pInterface->Sensor_GetCamFuseID(CamID),sensorID);
	m_pInterface->Ctrl_Addlog(CamID,m_pInterface->Sensor_GetCamFuseID(CamID),COLOR_BLUE,200);

	char str_StationName[512] = {0};
	ImageProc::ConvertWideToChar(strSFRStationName,str_StationName);

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
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("open file fail"),COLOR_BLUE,200);
	}
}

//保存40CM 的SFR
void FF_SFR_TestItem::Save40CMSFR()
{
	if(pDlg->m_B40cmSFRCheckData)
	{
		CString strCurrentPath = m_pInterface->Config_GetExeDir();
		CString strFileName = strCurrentPath + _T("SFRBurnFile_40CM.ini");
		FILE* fp = NULL;
		fp = _wfopen(strFileName,_T("a"));
		if (fp == NULL)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("创建SFRBurnFile_40CM.ini文件失败"),COLOR_RED,LOGHEIGHT);
			return ;
		}
		fclose(fp);
		/*m_pInterface->Ctrl_Addlog(CamID,_T("SFRBurnFile.ini保存数据进行中！"),COLOR_BLUE,300);*/
		CString temp;
		//WritePrivateProfileString(m_Options.m_StationName,_T("Posture"),_T("1"),strFileName);
		WritePrivateProfileString(m_Options.m_StationName,_T("Peak"),temp,strFileName);
		for (int i =0;i < m_iSFRROICount; i++)
		{
			temp.Format(_T("%d"),(int)(m_dSFRValue[i] * m_Options.m_iSFRBurnRate + 0.5));
			WritePrivateProfileString(m_Options.m_StationName,m_stSpec[i].ROIName,temp,strFileName);
		}
	}
	
}
//保存60CM 的SFR
void FF_SFR_TestItem::Save60CMSFR()
{
	if(pDlg->m_B60cmSFRCheckData)
	{
		CString strCurrentPath = m_pInterface->Config_GetExeDir();
		CString strFileName = strCurrentPath + _T("SFRBurnFile_60CM.ini");
		FILE* fp = NULL;
		fp = _wfopen(strFileName,_T("a"));
		if (fp == NULL)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("创建SFRBurnFile_60CM.ini文件失败"),COLOR_RED,LOGHEIGHT);
			return ;
		}
		fclose(fp);
		/*m_pInterface->Ctrl_Addlog(CamID,_T("SFRBurnFile.ini保存数据进行中！"),COLOR_BLUE,300);*/
		CString temp;
		//WritePrivateProfileString(m_Options.m_StationName,_T("Posture"),_T("1"),strFileName);
		WritePrivateProfileString(m_Options.m_StationName,_T("Peak"),temp,strFileName);
		for (int i =0;i < m_iSFRROICount; i++)
		{
			temp.Format(_T("%d"),(int)(m_dSFRValue[i] * m_Options.m_iSFRBurnRate + 0.5));
			WritePrivateProfileString(m_Options.m_StationName,m_stSpec[i].ROIName,temp,strFileName);
		}
	}
}
//保存120CM 的SFR
void FF_SFR_TestItem::Save120CMSFR()
{
	if(pDlg->m_B120cmSFRCheckData)
	{
		CString strCurrentPath = m_pInterface->Config_GetExeDir();
		CString strFileName = strCurrentPath + _T("SFRBurnFile_120CM.ini");
		FILE* fp = NULL;
		fp = _wfopen(strFileName,_T("a"));
		if (fp == NULL)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("创建SFRBurnFile_120CM.ini文件失败"),COLOR_RED,LOGHEIGHT);
			return ;
		}
		fclose(fp);
		/*m_pInterface->Ctrl_Addlog(CamID,_T("SFRBurnFile.ini保存数据进行中！"),COLOR_BLUE,300);*/
		CString temp;
		//WritePrivateProfileString(m_Options.m_StationName,_T("Posture"),_T("1"),strFileName);
		WritePrivateProfileString(m_Options.m_StationName,_T("Peak"),temp,strFileName);
		for (int i =0;i < m_iSFRROICount; i++)
		{
			temp.Format(_T("%d"),(int)(m_dSFRValue[i] * m_Options.m_iSFRBurnRate + 0.5));
			WritePrivateProfileString(m_Options.m_StationName,m_stSpec[i].ROIName,temp,strFileName);
		}
	}
}

void FF_SFR_TestItem::GetSFR40CM()
{
	int n = 0;
	//取出40CM的SFR值
	CString strFileName = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile_40CM.ini");
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("CEN_H"),0,strFileName);// //中心水平方向写入固定值
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("CEN_V"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL4_H"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL4_V"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR4_H"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR4_V"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR4_H"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR4_V"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL4_H"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL4_V"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL6_H"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL6_V"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR6_H"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR6_V"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR6_H"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR6_V"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL6_H"),0,strFileName);
	m_dSFR40CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL6_V"),0,strFileName);
}

void FF_SFR_TestItem::GetSFR60CM()
{
	int n = 0;
	//取出40CM的SFR值
	CString strFileName = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile_60CM.ini");
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("CEN_H"),0,strFileName);// //中心水平方向写入固定值
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("CEN_V"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL4_H"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL4_V"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR4_H"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR4_V"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR4_H"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR4_V"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL4_H"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL4_V"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL6_H"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL6_V"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR6_H"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR6_V"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR6_H"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR6_V"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL6_H"),0,strFileName);
	m_dSFR60CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL6_V"),0,strFileName);
}

void FF_SFR_TestItem::GetSFR120CM()
{
	int n = 0;
	//取出40CM的SFR值
	CString strFileName = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile_120CM.ini");
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("CEN_H"),0,strFileName);// //中心水平方向写入固定值
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("CEN_V"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL4_H"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL4_V"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR4_H"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR4_V"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR4_H"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR4_V"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL4_H"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL4_V"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL6_H"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TL6_V"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR6_H"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("TR6_V"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR6_H"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BR6_V"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL6_H"),0,strFileName);
	m_dSFR120CM_Data[n++] = GetPrivateProfileInt(m_Options.m_StationName,_T("BL6_V"),0,strFileName);
}
//卡控SFR规格
BOOL FF_SFR_TestItem::SFRSpecLimit()
{
	bool bflag_60 = false;
	bool bflag_120 = false;
	CString strlog(_T(""));
	if(pDlg->m_BchekSFRSpec)
	{
		//GetSFR40CM();
		int n = 0;
		//取出40CM的SFR值
		CString strFileName = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile_40CM.ini");
		m_dSFR40CM_Data[n++] = GetPrivateProfileInt(_T("Station1"),_T("CEN_H"),0,strFileName);// //中心水平方向写入固定值
		m_dSFR40CM_Data[n++] = GetPrivateProfileInt(_T("Station1"),_T("CEN_V"),0,strFileName);
		for(int ik = 0; ik <2; ik ++)
		{
			strlog.Format(_T("数据--40CM SFR Data:%d"),m_dSFR40CM_Data[ik]);
			m_pInterface->Ctrl_Addlog(CamID,strlog,COLOR_BLUE,220);
		}
	   
		//取出60CM的SFR值
		//GetSFR60CM();
		n = 0;
		 strFileName = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile_60CM.ini");
		m_dSFR60CM_Data[n++] = GetPrivateProfileInt(_T("Station2"),_T("CEN_H"),0,strFileName);// //中心水平方向写入固定值
		m_dSFR60CM_Data[n++] = GetPrivateProfileInt(_T("Station2"),_T("CEN_V"),0,strFileName);
		for(int ik = 0; ik <2; ik ++)
		{
			strlog.Format(_T("数据--60CM SFR Data:%d"),m_dSFR60CM_Data[ik]);
			m_pInterface->Ctrl_Addlog(CamID,strlog,COLOR_BLUE,220);
		}
		//取出120CM的SFR值
		//GetSFR120CM();
		n = 0;
		 strFileName = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRBurnFile_120CM.ini");
		m_dSFR120CM_Data[n++] = GetPrivateProfileInt(_T("Station3"),_T("CEN_H"),0,strFileName);// //中心水平方向写入固定值
		m_dSFR120CM_Data[n++] = GetPrivateProfileInt(_T("Station3"),_T("CEN_V"),0,strFileName);
		for(int ik = 0; ik <2; ik ++)
		{
			strlog.Format(_T("数据--120CM SFR Data:%d"),m_dSFR120CM_Data[ik]);
			m_pInterface->Ctrl_Addlog(CamID,strlog,COLOR_BLUE,220);
		}
		//check 60CM的数据
		int temp60 = _ttoi(pDlg->m_StrSpecSFR_FIRST);
		//if(pDlg->m_bTestCenter)
		{
			for (int i =0;i < 2; i++)
			{
				if((m_dSFR40CM_Data[i] != 0) &&  (m_dSFR60CM_Data[i] != 0))
				{
					int Diff60 = m_dSFR40CM_Data[i]-temp60;
					if(m_dSFR60CM_Data[i] <= Diff60)
					{
						strlog.Format(_T("SFR数据：i:%d,m_dSFR40CM_Data:%d,m_dSFR60CM_Data:%d"),i,m_dSFR40CM_Data[i],m_dSFR60CM_Data[i]);
						m_pInterface->Ctrl_Addlog(CamID,strlog,COLOR_RED,220);
						bflag_60 = true;
					}
				}

			}
		}
		
		//check120cm数据
		int temp120 = _ttoi(pDlg->m_StrSpecSFR_SECOND);
		//if(pDlg->m_bTestCenter)
		{
			for (int i =0;i < 2; i++)
			{
				if((m_dSFR40CM_Data[i] != 0) &&  (m_dSFR120CM_Data[i] != 0))
				{
					int Diff120 = m_dSFR120CM_Data[i]-temp120;
					if(m_dSFR40CM_Data[i] <= Diff120)
					{
						strlog.Format(_T("SFR数据：i:%d,m_dSFR40CM_Data:%d,m_dSFR120CM_Data:%d"),i,m_dSFR40CM_Data[i],m_dSFR120CM_Data[i]);
						m_pInterface->Ctrl_Addlog(CamID,strlog,COLOR_RED,220);
						bflag_120 = true;
					}
				}
			}
		}
		
		if(bflag_120 || bflag_60)
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL FF_SFR_TestItem::SaveSFR()
{
	CString strCurrentPath = m_pInterface->Config_GetExeDir();
	CString strFileName = strCurrentPath + _T("SFRBurnFile.ini");
	FILE* fp = NULL;
	fp = _wfopen(strFileName,_T("a"));
	if (fp == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("创建SFRBurnFile.ini文件失败"),COLOR_RED,LOGHEIGHT);
		return FALSE;
	}
	fclose(fp);
	/*m_pInterface->Ctrl_Addlog(CamID,_T("SFRBurnFile.ini保存数据进行中！"),COLOR_BLUE,300);*/
	CString temp;
	WritePrivateProfileString(m_Options.m_StationName,_T("Posture"),_T("1"),strFileName);
	WritePrivateProfileString(m_Options.m_StationName,_T("Peak"),temp,strFileName);
	for (int i =0;i < m_iSFRROICount; i++)
	{
		temp.Format(_T("%d"),(int)(m_dSFRValue[i] * m_Options.m_iSFRBurnRate + 0.5));
		WritePrivateProfileString(m_Options.m_StationName,m_stSpec[i].ROIName,temp,strFileName);
	}
	////////////////////////// CTC ///////////////////////////////////
	temp.Format(_T("%d"),(int)(m_fCTC8 + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC8"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC6 + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC6"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC8_H + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC8_H"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC8_V + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC8_V"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC6_H + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC6_H"),temp,strFileName);
	temp.Format(_T("%d"),(int)(m_fCTC6_V + 0.5));
	WritePrivateProfileString(m_Options.m_StationName,_T("CTC6_V"),temp,strFileName);

	return TRUE;
}

CString FF_SFR_TestItem::GetModulePath()
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