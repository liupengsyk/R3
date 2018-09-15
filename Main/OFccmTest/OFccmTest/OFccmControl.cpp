#include "StdAfx.h"
#include "OFccmControl.h"
#include "ImageProc.h"
#include "OFccmTest.h"
#include <locale.h>
#include <vector>
#include <afxpriv.h>
#include"imagehlp.h"
enum 
{
	MESCHECK = 0,
	MESBINDING = 1
};
using namespace std;

GlobalSetting OFccmControl::globalset; 
 int  OFccmControl::OpenTimes = 0;

#pragma comment(lib,"SerialPortLED.lib")
OFccmControl::OFccmControl(HWND hTestDlgWnd,HWND hDisplayWnd[],HWND hPanel[],HWND hLogger[],int CameraNum)
{ 
	// ADDED BY WZX
	bCheckLight = FALSE;
	isClose[0] = FALSE;
	isClose[1] = FALSE;
	isOpen[0] = FALSE;
	isOpen[1] = FALSE;
	ShowResult[0] = 0;
	ShowResult[1] = 0;

	AAstate[0] =0;
	AAstate[1] =0;
	AAstate[2] =0;
	AAstate[3] =0;

	this->hTestDlgWnd = hTestDlgWnd;
	this->CameraNum = CameraNum;
	CreateLogFile();
	Ctrl_AddToLog(_T("CreateLogFile"),0);
	
	for (int i=0;i<CameraNum;i++)
	{
		Cam[i].hDisplayWnd = hDisplayWnd[i];	
		Cam[i].hPanel = hPanel[i];	
		Cam[i].hLogger = hLogger[i];
	}
	 
	ContinueTestTime = 0;
	Ctrl_AddToLog(_T("OFccmControl_ok"),0);
	Rgain = 1.0;
	Bgain = 1.0;
	pConfigFileRw = new ConfigFileRw();
	pMachineCtrl = NULL;
	pDevice = NULL;
	if(globalset.DeviceTypeSel == _T("UV910"))
	{
		pDevice=::GetDeviceType(this,_T("UV910"));//1 - HV910,2-UH920  
	}
	else if(globalset.DeviceTypeSel == _T("UH920"))
	{
		pDevice=::GetDeviceType(this,_T("UH920"));//1 - HV910,2-UH920 
	}
	else if(globalset.DeviceTypeSel == _T("MUD952"))
	{
		pDevice=::GetDeviceType(this,_T("MUD952"));//1 - HV910,2-UH920 
	}
	else if(globalset.DeviceTypeSel == _T("R3"))
	{
		pDevice=::GetDeviceType(this,_T("R3"));//1 - HV910,2-UH920 
	}
	else if(globalset.DeviceTypeSel == _T("R5"))
	{
		pDevice=::GetDeviceType(this,_T("R5"));//1 - HV910,2-UH920 
	}
	else if(globalset.DeviceTypeSel == _T("CTS"))
	{
		pDevice=::GetDeviceType(this,_T("CTS"));//1 - HV910,2-UH920 
	}
	 
	InitializeCriticalSection(&proc_cs);
	InitializeCriticalSection(&mMes_cs);
	  
	hControlerProc = CreateEvent(NULL,TRUE,FALSE,NULL);
	bCCMCtrlInit();
}

void OFccmControl::CreateLogFile()
{
	CString str(_T("LogFile\\"));
	CString logPath;
	logPath = globalset.GetCurrentPath() + str;
	CreateDirectory(logPath,NULL);
	DeleteFileW(logPath);
	for(int i=0;i<CameraNum;i++)
	{ 	
		str.Format(_T("LogFile\\logFile%d"),i); 
		Cam[i].logPath = globalset.GetCurrentPath() + str;
		CStdioFile file;
		
		if(isFileExist(Cam[i].logPath))
		{
			CString sfilename = Ctrl_GetCurrentLocalTime(); 
			CopyFile(Cam[i].logPath, Cam[i].logPath+ sfilename,0);
		}
		file.Open(Cam[i].logPath,CFile::modeCreate);
		file.Close();
	}
}


OFccmControl::~OFccmControl(void)
{
	for(int i=0;i<CameraNum;i++)
	{
		if(!isStandby(i))
		{
			CloseCamera(i,0);
		}
		UnlocktheFile(i);
	}
	bControlThreadOn = FALSE;	
	
	Sleep(300);
	SAFE_DELETE(pConfigFileRw);
	CamBufRelease();
	SAFE_CLOSE_HANDLE(hControlerProc);
	 
}
void OFccmControl::SetCamID(int CamID)
{
	this->CamID = CamID;
}
int OFccmControl::CamBufRelease()
{
	for (int i=0;i<2;i++)
	{
		SAFE_DELETE(Cam[i].pConfig);			  
		SAFE_DELETE(Cam[i].pTestCtrl); 
		SAFE_DELETE(Cam[i].pCameraCtrl);  
		SAFE_DELETE(Cam[i].pImgExplorer); 
		SAFE_DELETE(Cam[i].pVcmDriver); 
		SAFE_DELETE(Cam[i].pOtpSensor); 
		SAFE_DELETE(Cam[i].pImgSensor); 
		SAFE_DELETE(Cam[i].m_log);  
		SAFE_DELETE(Cam[i].DisplayMemryDC);  
		SAFE_DELETE(Cam[i].pServerSocket); 
		SAFE_DELETE(Cam[i].camSocket); 
	
		Cam[i].DisplayMemryDC = NULL;
		Cam[i].pBuf.grab_buffer = NULL;	 
		Cam[i].pBuf.display_buffer = NULL;	
		Cam[i].pBuf.transfer_buffer = NULL;
		Cam[i].pBuf.RGB24_buffer= NULL;
		Cam[i].pBuf.raw_buffer = NULL;
		Cam[i].bTripleBufferDirty = FALSE;
	}

	SAFE_DELETE(pMachineCtrl);
	SAFE_DELETE(pDevice);
	return 1;
}

int OFccmControl::CamInit()
{
	 
	for (int i=0;i<2;i++)
	{
		Cam[i].camSocket = NULL;
		Cam[i].pConfig = NULL;			  
		Cam[i].pTestCtrl = NULL;
		Cam[i].pCameraCtrl = NULL;
	
		Cam[i].pImgExplorer = NULL;
		Cam[i].pVcmDriver  = NULL; 
		Cam[i].pOtpSensor =NULL; 
		Cam[i].pImgSensor =NULL; 
		Cam[i].m_log = NULL;

		Cam[i].m_FrameInfo.FrameCnt = 0;
		Cam[i].m_FrameInfo.m_GrabSize = 0;	 
		Cam[i].m_FrameInfo.PlayCnt = 0;

		Cam[i].bState.bAEmutex = TRUE;
		Cam[i].bState.bCameraOffLine = FALSE;
		Cam[i].bState.bCameraStandby = TRUE;
		Cam[i].bState.bCameraRunSate=0;
		Cam[i].bState.bCameraStarted = FALSE;
		Cam[i].bState.bLoopFlag = FALSE;
		Cam[i].bState.bOffline_LoopFlag = FALSE;

		Cam[i].bState.bGrabThreadRunSate = -1;
		Cam[i].bState.bDisplayThreadRunSate = -1;//Grab、Display线程状态标示

		Cam[i].bTripleBufferDirty = FALSE;

		Cam[i].hThreadCtrl.m_hGrabPause = NULL;
		Cam[i].hThreadCtrl.m_hDisPlayPause = NULL;	
		Cam[i].hThreadCtrl.m_hOfflineThreadDead = NULL;
		Cam[i].hThreadCtrl.m_hCameraThreadEventDead = NULL;
		Cam[i].hThreadCtrl.m_hDisplayThreadEventDead = NULL;

		memset(Cam[i].fWorkingCurrent, 0,  5*sizeof(float));
		Cam[i].bWorkingCurrentFailed = FALSE; 
		 
		InitializeCriticalSection(&Cam[i].m_csLock);
		InitializeCriticalSection(&Cam[i].m_DisPlayLock); 
		Cam[i].DisplayMemryDC = NULL;
		Cam[i].pBuf.grab_buffer = NULL;	 
		Cam[i].pBuf.display_buffer = NULL;	
		Cam[i].pBuf.transfer_buffer = NULL;
		Cam[i].pBuf.RGB24_buffer= NULL;
		Cam[i].pBuf.raw_buffer = NULL; 
		Cam[i].pServerSocket = NULL; 

		Cam[i].MesSelect = MESCHECK;     //Mes 标示状态
		 
	}

	for (int i=0;i<CameraNum;i++)
	{
		Cam[i].pConfig = new ConfigManage();		 
		Cam[i].pConfig->LoadConfig(i);
		Ctrl_AddToLog(_T("GetCurentDeviceNameNum"),i);
		  
		if (globalset.DeviceTypeSel==_T("UH920"))
		{
			Cam[i].channel = i+1; 
		}
		else
		{
			Cam[i].channel =1; 
		}		

		Cam[i].pTestCtrl = new TestItemControl(this,Cam[i].hPanel,i);
		Cam[i].pTestCtrl->m_TestMode = Cam[i].pConfig->GetGeneralTab()->m_TestMode; //TESTMODE_MANUAL =0
		Cam[i].pCameraCtrl = new CCameraControl(i); 
	
		Cam[i].pImgExplorer = new ImageExplorer(Cam[i].hDisplayWnd,this,i);
		Cam[i].pVcmDriver  = NULL;
		if(Cam[i].pConfig->GetVcmDr().GetLength()>1)
		{
			Cam[i].pVcmDriver = ::GetVcmDriver(this,Cam[i].pConfig->GetVcmDr(),Cam[i].pConfig->VCM_SlaveID,i); //Config.VcmDriver  _T("S5K4EC")
		}
	  
		if(Cam[i].pConfig->GetOtpSensorTab()->SensorName.GetLength()>1)
		{
			Cam[i].pOtpSensor = ::GetOtpSensor(this,Cam[i].pConfig->GetOtpSensorTab()->SensorName,
				Cam[i].pConfig->GetOtpSensorTab()->SlaveID,i);  
		} 
		 
		if(Cam[i].pConfig->GetSensorTab()->name.GetLength()>1)
		{
			Cam[i].pImgSensor = ::GetImgSensor(this,Cam[i].pConfig->GetSensorTab()->name,
				Cam [i].pConfig->GetSensorTab()->SlaveID,i);  
		}
		 
		Cam[i].m_log = new LoggerManage(Cam[i].hLogger);

		Cam[i].m_FrameInfo.FrameCnt = 0;
		Cam[i].m_FrameInfo.m_GrabSize = 0;	 
		Cam[i].m_FrameInfo.PlayCnt = 0;

		
		Cam[i].hThreadCtrl.m_hGrabPause = NULL;
		Cam[i].hThreadCtrl.m_hDisPlayPause = NULL;	
		Cam[i].hThreadCtrl.m_hOfflineThreadDead = NULL;
		Cam[i].hThreadCtrl.m_hCameraThreadEventDead = NULL;
		Cam[i].hThreadCtrl.m_hDisplayThreadEventDead = NULL;

		memset(Cam[i].fWorkingCurrent, 0,  5*sizeof(float));
		Cam[i].bWorkingCurrentFailed = FALSE; 

		Cam[i].MesID = 0;
		Cam[i].usedMes = FALSE;
		Cam[i].bSuccessMes=FALSE;
		 

		Cam[i].DisplayMemryDC = new CDC();
		GetBuffer(i);
		bMemDCInit(i);	
		 
		if(Cam[i].pConfig->GetGeneralTab()->SocketEn)
		{ 
			InitSocket(i,TRUE);
		}
		CString str;
		str.Format(_T("Cam %d Init"),i);
		Ctrl_AddToLog(str,i);

		InitCamDualData(i);
		Cam[i].CamPoint = (0,0); 

		CString otpdataManagerPath;
		otpdataManagerPath.Format(_T("otpdataManager\\Cam%d\\"),i);
		otpdataManagerPath =  globalset.GetCurrentPath() + otpdataManagerPath;

		Cam[i].otpMapPath = otpdataManagerPath + _T("otpItemMap");
		if(!isFileExist(Cam[i].otpMapPath)) ::CreateDirectory(Cam[i].otpMapPath,NULL);

		Cam[i].otpByteInPath = otpdataManagerPath + _T("otpInputByte");
		if(!isFileExist(Cam[i].otpByteInPath)) ::CreateDirectory(Cam[i].otpByteInPath,NULL);

		Cam[i].otpCheckDataPath = otpdataManagerPath + _T("otpItemCheckData");
		if(!isFileExist(Cam[i].otpCheckDataPath)) ::CreateDirectory(Cam[i].otpCheckDataPath,NULL);

		Cam[i].otpByteOutPath = otpdataManagerPath + _T("otpOutByte");
		if(!isFileExist(Cam[i].otpByteOutPath)) ::CreateDirectory(Cam[i].otpByteOutPath,NULL);

		Cam[i].otpItemdataPath = otpdataManagerPath + _T("otpItemData");
		if(!isFileExist(Cam[i].otpItemdataPath)) ::CreateDirectory(Cam[i].otpItemdataPath,NULL);
	}
/*
	for (int i=0;i<CameraNum;i++)
	{
		 KillDataBuffer(i);		
	}
 */
	return 0;
}

void OFccmControl::InitCamDualData(int CamID)
{
	DualCamData m_DualCamData;
	m_DualCamData.bResult = FALSE;
	m_DualCamData.dif_Rotation = 0;
	m_DualCamData.dif_TiltX = 0;
	m_DualCamData.dif_TiltY = 0;
	m_DualCamData.Rotation = 0;
	m_DualCamData.TiltX = 0;
	m_DualCamData.TiltY = 0;
	Ctrl_SetDualCamData(CamID, m_DualCamData);


}

BOOL OFccmControl::bCCMCtrlInit()
{	
	CamInit(); 
	InitializeCriticalSection(&m_lockChannel);
	OpCameraNum=0;
	CWinThread* WndThread = AfxBeginThread(ControlThread,this);
	ControlThreadID = WndThread->m_nThreadID;
	WndThread = NULL;
	return TRUE;
}


int OFccmControl::ControlThread()
{
	MSG msg; 
	bTEST = FALSE; 
	bControlThreadOn = TRUE;
	while(bControlThreadOn)
	{  

		if(!PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			Sleep(100); 
			continue;
		}
		else
		{
			switch(msg.message)
			{
			case PLAY_CAMERA:
				{
					int openFlage = OpenCamera(msg.wParam); 
				}
				break;
			case STOP_CAMERA:
				CloseCamera(msg.wParam,0);
				break;
			case PAUSE_CAMERA:
				Ctrl_PauseGrabFrame(msg.wParam);
				break;
			case RESUME_CAMERA:
				Ctrl_ResumeGrabFrame(msg.wParam);
				break;
			case TM_SINGLETEST://单项测试
				{
					Cam[msg.wParam].pTestCtrl->SetTestItem(msg.lParam);//lParam

					break;
				}
				break; 

			}
			//SetEvent(hControlerProc);
		}
		
		
	}
	::AfxEndThread(0);
	return 0;
}



BOOL OFccmControl::GetBuffer(int CamID)
{ 
	int width=Cam[CamID].pConfig->GetSensorTab()->width;
	int height=Cam[CamID].pConfig->GetSensorTab()->height;
	UINT nSize = 0;
	if (width%2 == 0 && height%2 == 0)
	{
		nSize = width * height + 1024;
	}
	if (width%2 == 0 && height%2 == 1)
	{
		nSize = width * (height+1) + 1024;	
	}
	else if (width%2 == 1 && height%2 == 0)
	{
		nSize = (width+1) * height + 1024;
	}
	else if (width%2 == 1 && height%2==1)
	{
		nSize = (width+1) * (height+1) + 1024;
	}
	else
	{
		nSize = width * height + 1024;
	}

	Cam[CamID].pBuf.grab_buffer = new BYTE[nSize*5/4];	 
	Cam[CamID].pBuf.display_buffer = new BYTE[nSize*5/4];	
	Cam[CamID].pBuf.transfer_buffer = new BYTE[nSize*5/4];
	Cam[CamID].pBuf.RGB24_buffer= new BYTE[nSize*3];
	Cam[CamID].pBuf.raw_buffer = new USHORT[nSize];

	if (Cam[CamID].pBuf.grab_buffer  == NULL  || 
		Cam[CamID].pBuf.display_buffer  == NULL  || 
		Cam[CamID].pBuf.transfer_buffer == NULL ||
		Cam[CamID].pBuf.RGB24_buffer == NULL ||
		Cam[CamID].pBuf.raw_buffer == NULL)
	{
		Ctrl_Addlog(CamID,_T("Memory error!"),COLOR_RED);
		return FALSE;
	} 
	return TRUE;
}

void OFccmControl::KillDataBuffer(int CamID)
{
	SAFE_DELETE_ARRAY(Cam[CamID].pBuf.grab_buffer);  
	SAFE_DELETE_ARRAY(Cam[CamID].pBuf.display_buffer); 
	SAFE_DELETE_ARRAY(Cam[CamID].pBuf.transfer_buffer); 
	SAFE_DELETE_ARRAY(Cam[CamID].pBuf.RGB24_buffer); 
 	SAFE_DELETE_ARRAY(Cam[CamID].pBuf.raw_buffer); 

	SAFE_CLOSE_HANDLE(Cam[CamID].hThreadCtrl.m_hGrabPause );
	SAFE_CLOSE_HANDLE(Cam[CamID].hThreadCtrl.m_hDisPlayPause );
	SAFE_CLOSE_HANDLE(Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead );
	SAFE_CLOSE_HANDLE(Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead );
	 
 }

BOOL OFccmControl::bMemDCInit(int CamID)
{ 
	HWND srcHdc = Ctrl_GethDisplayWnd( CamID);
	CDC* destDC = Ctrl_GetDisplayWndMemDC(CamID) ;
	 
	CRect rect;
	::GetWindowRect(srcHdc,&rect);
	CPoint cp = rect.TopLeft();
	::ScreenToClient(hTestDlgWnd,&cp);
	rect.MoveToXY(cp); 
	 
	//创建并重置DC
	CreateDisplayMemDC(CamID);	
	ResetMemdcBackGround(CamID);
	destDC = NULL;
	return TRUE;
}

void OFccmControl::CreateDisplayMemDC(int CamID) // 由 HWND 创建dc
{ 
	//初始化辅组窗口的兼容DC
	CDC* destDC = Ctrl_GetDisplayWndMemDC( CamID);
	HWND srcHdc = Ctrl_GethDisplayWnd( CamID);
	CRect destRect;
	Ctrl_GetDisplayWndRect(&destRect, CamID);

	if (destDC->m_hDC != NULL)  
	{
		destDC->DeleteDC();  
	} 
	CDC*  srcPdc = CWnd::FromHandle(srcHdc)->GetDC();

	CBitmap bitmap;
	destDC->CreateCompatibleDC(srcPdc);//创建dc

	bitmap.CreateCompatibleBitmap(srcPdc,destRect.Width(),destRect.Height());

	destDC->SelectObject(&bitmap);

	CWnd::FromHandle(srcHdc)->ReleaseDC(srcPdc);
	bitmap.DeleteObject(); 
	destDC = NULL;
	srcPdc = NULL;
	 
}

void OFccmControl::ResetMemdcBackGround(int CamID,COLORREF BKcolor,CString strLog,COLORREF strColor)
{ 
	CRect rect;
	Ctrl_GetDisplayWndRect(&rect,CamID);
	int fontSize = rect.Height()*2/3;
	Ctrl_DrawingToMenDC(CamID, BKcolor);  	   
	Ctrl_DrawingToMenDC(CamID,strColor,strLog,fontSize,3,2);

}

int OFccmControl::Ctrl_DrawingToMenDC(int CamID,COLORREF bkColor)
{
	CRect rect;	
	::GetClientRect(Ctrl_GethDisplayWnd(CamID),&rect);
	CDC* pMenDC = Ctrl_GetDisplayWndMemDC(CamID);
	pMenDC->FillRect(&rect,&CBrush(bkColor));
	pMenDC = NULL;
	return 0;
}

int OFccmControl::Ctrl_DrawingToMenDC(int CamID,COLORREF strColor,CString strLog,UINT nSize,UINT cx,UINT cy)
{
	CDC* pMenDC = Ctrl_GetDisplayWndMemDC(CamID);
	//刷新DC背景
	CRect rect;
	CFont ft;
	ft.CreatePointFont(nSize,_T("Swis721 BlkEx BT"));

	::GetClientRect(Ctrl_GethDisplayWnd(CamID),&rect);

	pMenDC->SelectObject(&ft);
	pMenDC->SetBkMode(TRANSPARENT);
	pMenDC->SetTextColor(strColor);
	pMenDC->TextOutW(int(rect.Width()/cx), int (rect.Height()/cy),strLog);
	pMenDC = NULL;

	return 0;
}

int OFccmControl::Ctrl_DrawingToMenDC(int CamID,COLORREF recColor,CRect drawRect,ImageInfo img)
{
	CDC* pMenDC = Ctrl_GetDisplayWndMemDC(CamID);
	//刷新DC背景
	CRect rect;

	CPen pen(PS_SOLID,2,recColor);

	::GetClientRect(Ctrl_GethDisplayWnd(CamID),&rect);

	double Xratio = 1.0*rect.Width()/img.imgwidth;
	double Yratio = 1.0*rect.Height()/img.imgheight;

	pMenDC->SelectObject(&pen);	
	pMenDC->SelectStockObject(NULL_BRUSH);
	pMenDC->SetBkMode(TRANSPARENT);

	drawRect.left   =  LONG(drawRect.left   *  Xratio);
	drawRect.right  =  LONG(drawRect.right  *  Xratio);
	drawRect.top    =  LONG(drawRect.top    *  Yratio);
	drawRect.bottom =  LONG(drawRect.bottom *  Yratio);
	pMenDC->Rectangle(drawRect);
	pMenDC = NULL;
	return 0;
}

int OFccmControl::Ctrl_DrawingToMenDC(int CamID,COLORREF lineColor,CPoint sPoint,CPoint ePoint,ImageInfo img)
{
	CDC* pMenDC = Ctrl_GetDisplayWndMemDC(CamID);
	//刷新DC背景
	CRect rect;

	CPen pen(PS_SOLID,2,lineColor);

	::GetClientRect(Ctrl_GethDisplayWnd(CamID),&rect);
	double Xratio = 1.0*rect.Width()/img.imgwidth;
	double Yratio = 1.0*rect.Height()/img.imgheight;

	pMenDC->SelectObject(&pen);	 

	sPoint.x =  LONG(sPoint.x   *  Xratio);
	sPoint.y =  LONG(sPoint.y  *  Yratio);
	ePoint.x =  LONG(ePoint.x    *  Xratio);
	ePoint.y =  LONG(ePoint.y *  Yratio);
	pMenDC->MoveTo(sPoint);
	pMenDC->LineTo(ePoint);
	pMenDC = NULL;
	return 0;
}

int OFccmControl::Ctrl_DrawingToMenDC(int CamID,COLORREF lineColor,int cx0,int cy0,int cx1,int cy1,ImageInfo img)
{
	CRect rect;
	CDC* pMenDC = Ctrl_GetDisplayWndMemDC(CamID);
	CPen pen(PS_SOLID,2,lineColor);
	::GetClientRect( Ctrl_GethDisplayWnd(CamID),&rect);
	double Xratio = 1.0*rect.Width()/img.imgwidth;
	double Yratio = 1.0*rect.Height()/img.imgheight;

	pMenDC->SelectObject(&pen);	 

	cx0=  LONG(cx0  *  Xratio);
	cy0 =  LONG(cy0  *  Yratio);
	cx1 =  LONG(cx1    *  Xratio);
	cy1 =  LONG(cy1 *  Yratio);
	pMenDC->Arc(cx0,cy0,cx1,cy1,cx0,cy0,cx0,cy0); 
	pMenDC = NULL;
	return 0;
}

int OFccmControl::Ctrl_DrawingToMenDC(int CamID,int Width,int Height,BYTE* SrcImg,int ImgType)
{
	int width  = Width;
	int height = Height;
	//根据比例压缩图像
	int CompressionRatio = GetDynamicCompressRatio(CamID);
	 
	int patcData = 0;
	if((width/CompressionRatio)%4!=0)
	{
		patcData = 4 - (width/CompressionRatio)%4;
	}

	CImage image;

	image.Create(width/CompressionRatio-patcData, -height/CompressionRatio, 24);
	BYTE* pDest = (BYTE *)image.GetBits();
	BYTE* ImageShow_buffer;
	if(ImgType==0) 
	{
		ImageShow_buffer = SrcImg;
	}
	else
	{
		ImageShow_buffer  = new BYTE[width*height*3];
		if(NULL == ImageShow_buffer)
		{
			SAFE_DELETE_ARRAY(ImageShow_buffer);
			pDest = NULL;
			return 0;
		}

		for(int i=0;i<width*height;i++)
		{
			ImageShow_buffer[i*3] = SrcImg[i];
			ImageShow_buffer[i*3+1] = SrcImg[i];
			ImageShow_buffer[i*3+2] = SrcImg[i];
		}
	}

	ImageProc::ZoomOutRGBImage( ImageShow_buffer, width, height, pDest, CompressionRatio);
	//获取当前显示区域大小
	CRect rect;
	::GetClientRect(Cam[CamID].hDisplayWnd,&Cam[CamID].DisplayRect);
	//cimage 画到内存DC
	//计算显示区域

	//DisplayMemryDC->SetStretchBltMode(ConfigManager.GetGeneralTab()->ColorOnColor_En?COLORONCOLOR:HALFTONE);
	Cam[CamID].DisplayMemryDC->SetStretchBltMode(HALFTONE);


	image.Draw(Cam[CamID].DisplayMemryDC->m_hDC, Cam[CamID].DisplayRect);

	Ctrl_ShowMemDC(CamID);
	SAFE_DELETE_ARRAY(ImageShow_buffer);
	pDest = NULL;
	 
	return 0;
}

void OFccmControl::DrawToDisplayMemDc(int CamID)
{
	//PreImageProcess(CamID);
	int Width  = Cam[CamID].pConfig->GetSensorTab()->width;
	int Height = Cam[CamID].pConfig->GetSensorTab()->height; 

	//获取当前显示区域大小
	
	::GetClientRect(Cam[CamID].hDisplayWnd,&Cam[CamID].DisplayRect);
	CRect rect = Cam[CamID].DisplayRect;

	//计算显示区域
	CDC* pDC = Ctrl_GetDisplayWndMemDC(CamID);

	SetStretchBltMode(pDC->m_hDC, HALFTONE);
	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	CPoint ScrImageStartPoint(0,0);; //原图的显示起点（整数）

	CImage ScrImage;
	int width=0;
	int height=0;
	int newHeight=Height-Height%4;
	int newWidth=Width-Width%4;
	if (Cam[CamID].pConfig->GetSensorTab()->width>4000)
	{
		width=(Width-Width%4)/4;
		width = ((width)/4)*4;
		newWidth = width * 4;

		height=(Height-Height%4)/4;
	}
	else
	{
		width=Width;
		height=Height;
	}

	if (!ScrImage.Create(width, -height, 24))
	{
		return;
	}
	BYTE* pDest = (BYTE *)ScrImage.GetBits();

	if (Cam[CamID].pConfig->GetSensorTab()->width>4000)
	{
		int cnt=0;
		for (int j=0;j<newHeight;j+=4)
		{
			for (int i=0;i<newWidth;i+=4)
			{
				pDest[cnt++] = Cam[CamID].pBuf.RGB24_buffer[j*Width*3+i*3];
				pDest[cnt++] = Cam[CamID].pBuf.RGB24_buffer[j*Width*3+i*3+1];
				pDest[cnt++] = Cam[CamID].pBuf.RGB24_buffer[j*Width*3+i*3+2];
			}
		}
	}
	else
	{
		for (int i=0;i<width*height*3;i++)
		{
			pDest[i] = Cam[CamID].pBuf.RGB24_buffer[i];
		}
	}
	

	float ZoomRatio = 0.0;
	//边界
	int a = int( (ScrImage.GetHeight()*ZoomRatio - rect.Height() )/ ZoomRatio);
	int b = int( (ScrImage.GetWidth()*ZoomRatio  - rect.Width() )/ ZoomRatio);

	//判断图像起点否超出边界
	if(ScrImageStartPoint.y>=a)
		ScrImageStartPoint.y=a;
	if(ScrImageStartPoint.x>=b)
		ScrImageStartPoint.x=b;

	if(ScrImageStartPoint.y<=0)
		ScrImageStartPoint.y=0;
	if(ScrImageStartPoint.x<=0)
		ScrImageStartPoint.x=0;

	if ( (a<=0 && b<=0) || (ZoomRatio == 0))
	{
		ScrImage.Draw(pDC->m_hDC,rect);
		ZoomRatio = (float) rect.Width() / ScrImage.GetWidth();
	} 
	else
	{
		int width  = int (rect.Width()/ZoomRatio);
		int height = int (rect.Height()/ZoomRatio) ;

		CRect display_rect(ScrImageStartPoint,CSize(width,height));
		ScrImage.StretchBlt(pDC->m_hDC,rect,display_rect);
		//释放DC		
	} 

	pDC = NULL;
	pDest = NULL;
}

int OFccmControl::Ctrl_ShowMemDC(int CamID)
{
	CDC* pMemDC = Ctrl_GetDisplayWndMemDC(CamID);
	//通过MAP找到对应的窗口句柄
	HWND hWnd = Ctrl_GethDisplayWnd(CamID);

	CDC* pDC = CWnd::FromHandle(hWnd)->GetDC();
	CRect rect;
	::GetClientRect(hWnd,&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),pMemDC,0,0,SRCCOPY);
	//pDC->StretchBlt(0,0,rect.Width(),rect.Height(),pMemDC,0,0,Cam[CamID].m_ImageInfo.imgwidth,Cam[CamID].m_ImageInfo.imgheight,SRCCOPY);

	CWnd::FromHandle(hWnd)->ReleaseDC(pDC);

	pMemDC = NULL;
	pDC = NULL;

	return 0;
}

int OFccmControl::Ctrl_ShowMemDCChart(int CamID)
{
	CDC* pMemDC = Ctrl_GetDisplayWndMemDC(CamID);
	//通过MAP找到对应的窗口句柄
	HWND hWnd = Ctrl_GethDisplayWnd(CamID);

	CDC* pDC = CWnd::FromHandle(hWnd)->GetDC();
	CRect rect;
	::GetClientRect(hWnd,&rect);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),pMemDC,0,0,SRCCOPY);

	CPoint point[4];
	point[0].x = rect.Width()/2;
	point[0].y = 0;
	point[1].x = rect.Width()/2;
	point[1].y = rect.Height();
	point[2].x = 0;
	point[2].y = rect.Height()/2;
	point[3].x = rect.Width();
	point[3].y = rect.Height()/2;

	COLORREF CurrentColor = RGB(255,0,0);
	CPen pen(PS_SOLID,2,CurrentColor);
	pDC->SelectObject(&pen);

	pDC->SelectStockObject(NULL_BRUSH);
	
	pDC->MoveTo(point[0]);
	pDC->LineTo(point[1]);
	pDC->MoveTo(point[2]);
	pDC->LineTo(point[3]);

	int radius = rect.Height()/6;

	point[0].x = rect.Width()/2 - radius;
	point[0].y = rect.Height()/2 - radius;
	point[1].x = rect.Width()/2 + radius;
	point[1].y = rect.Height()/2 + radius;

	pDC->Ellipse(point[0].x,point[0].y,point[1].x,point[1].y);

	point[2].x = rect.Width()/2 - radius*2;
	point[2].y = rect.Height()/2 - radius*2;
	point[3].x = rect.Width()/2 + radius*2;
	point[3].y = rect.Height()/2 + radius*2;

	pDC->Ellipse(point[2].x,point[2].y,point[3].x,point[3].y);

	CWnd::FromHandle(hWnd)->ReleaseDC(pDC);

	pMemDC = NULL;
	pDC = NULL;

	return 0;
}

int OFccmControl::GetDynamicCompressRatio(int CamID)
{ 
	::GetClientRect(Cam[CamID].hDisplayWnd,&Cam[CamID].DisplayRect);

	double ratio = 1.0 * (Cam[CamID].pConfig->GetSensorTab()->width) / (Cam[CamID].DisplayRect.Width());

	int CompressRatio = 2;
	if (ratio <= 2.2)
	{
		CompressRatio = 2;
	}
	else if (ratio <8)
	{
		CompressRatio = 2;
	}
	else 
	{ 
		CompressRatio = (int)ratio/4.0;
		if (CompressRatio%2!=0)
		{
			CompressRatio -= 1;
		}

		CompressRatio = CompressRatio*4;
	}
 
	return CompressRatio;
}

int OFccmControl::Ctrl_CopyBackGroundToMemDC(int CamID)
{
	CDC* pMemDC = Ctrl_GetDisplayWndMemDC( CamID) ;
	//通过MAP找到对应的窗口句柄
	HWND hWnd = Ctrl_GethDisplayWnd(CamID);

	CDC* pDC = CWnd::FromHandle(hWnd)->GetDC();
	CRect rect;
	::GetClientRect(hWnd,&rect);
	pMemDC->BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);
	CWnd::FromHandle(hWnd)->ReleaseDC(pDC);
	pMemDC = NULL;
	pDC = NULL;
	return 0;
}
CDC* OFccmControl::Ctrl_GetDisplayWndMemDC(int CamID)       //得到主显示窗口DC       
{
	return (Cam[CamID].DisplayMemryDC);	 
}
 

void OFccmControl::Ctrl_GetDisplayWndRect(CRect* rect,int CamID)    //得到主显示窗口的大小
{ 
	::GetClientRect(Cam[CamID].hDisplayWnd,rect);
}
HWND OFccmControl::Ctrl_GethDisplayWnd(int CamID)
{
	return Cam[CamID].hDisplayWnd;
}
  
int OFccmControl::Device_WriteI2c(UCHAR uAddr, USHORT uReg, USHORT uValue, BYTE byMode,int CamID)
{ 
	return pDevice->WriteSensorReg( uAddr,  uReg,  uValue,  byMode, 0,CamID);
}
int OFccmControl::Device_ReadI2c(UCHAR uAddr, USHORT uReg, USHORT *pValue,BYTE byMode,int CamID)
{ 
	if(pValue==NULL) return -1; 
	return pDevice->ReadSensorReg( uAddr,  uReg,  pValue,  byMode, 0,CamID);
}


void OFccmControl::Ctrl_ClearLog(int CamID)
{
	Cam[CamID].m_log->ClearLog();
}

void OFccmControl::Ctrl_Addlog(int CamID,CString info ,COLORREF color,int yHeight )
{
	if((CamID+1)>globalset.CamTotalNum)
	{
		CamID = 0;
		info += _T("error");
		color = COLOR_RED;
	}
	Cam[CamID].m_log->Addlog(info,color,yHeight,CamID);
	if(CamID<0) CamID=0;
	Ctrl_AddToLog(info,CamID);
}
void OFccmControl::Ctrl_Addlog(int CamID,int info ,COLORREF color,int yHeight )
{
	CString str;
	str.Format(_T("%d"),info);

	if((CamID+1)>globalset.CamTotalNum)
	{
		CamID = 0;
		str += _T("error");
		color = COLOR_RED;
	}
	if(CamID < 0) CamID = 0;
	Cam[CamID].m_log->Addlog(str,color,yHeight,CamID);
	Ctrl_AddToLog(str,CamID);
}

void OFccmControl::PostMsgToControler(int wp,int CamID,int lp)
{
	EnterCriticalSection(&proc_cs);	
	PostThreadMessage(ControlThreadID,wp,(WPARAM)CamID,lp);	
	Sleep(200);
 	LeaveCriticalSection(&proc_cs);
}
void OFccmControl::OpenCameraMsg(int CamID)
{
	//PostMsgToControler(PLAY_CAMERA,(WPARAM)CamID,0);
	OpenCamera(CamID); 
}

void OFccmControl::CloseCameraMsg(int CamID)
{
	PostMsgToControler(STOP_CAMERA,(WPARAM)CamID,0); 
}

int OFccmControl::Ctrl_ResumeGrabFrame(int CamID)
{
	SetEvent(Cam[CamID].hThreadCtrl.m_hGrabPause);
	return 0;
}
void OFccmControl::ResumeCameraMsg(int CamID)
{
	PostMsgToControler(RESUME_CAMERA,(WPARAM)CamID,0);  
}

int OFccmControl::Ctrl_PauseGrabFrame(int CamID)
{
	ResetEvent(Cam[CamID].hThreadCtrl.m_hGrabPause);

	return 0;
}

void OFccmControl::PauseCameraMsg(int CamID)
{
	PostMsgToControler(PAUSE_CAMERA,(WPARAM)CamID,0);   
}
int OFccmControl::CamStartInit(int CamID)
{
	int flag = 0;
	int i = CamID;
	flag = OSTest(i);
	if(flag!=1)
	{
		Ctrl_Addlog(i,_T("Cam启动失败"),COLOR_RED,200);
		Cam[i].NGTestItemID=-1;
		return -1;
	} 
	pDevice->CamSensorTab[i].bTestStandyCurrent=Cam[CamID].pConfig->GetStandbyCurrentTab()->Test_en;
	pDevice->SetCamTab(i); 
	Cam[i].pCameraCtrl->Init();  
	 
	bWriteExp = FALSE;
	 
	Cam[i].pTestCtrl->TestInit();  
	 
	return 1;
}
int OFccmControl::OpenCamera(int CamID)
{  
	isClose[0] = FALSE;
	isClose[1] = FALSE;
	ShowResult[CamID] = 0;

	OpenTimes++;
	int flag = 0 ;

	
	if (globalset.CamType == _T("Dual") && globalset.DeviceTypeSel.Find(_T("UH920"))!=-1)
	{  
		isOpen[CamID] = TRUE;
		if(!isOpen[0] || !isOpen[1] ) return 0; 
		
		AAstate[0] = 0;
		AAstate[1] = 0;
		AAstate[2] = 0;
		AAstate[3] =0;
		if(globalset.DeviceTypeSel.Find(_T("UH920"))!=-1)
		{ 
			StartDualCamThread();
		}
		else
		{ 
			//int DualCamID = 0;//GetDualCamID(CamID);
			//for(int i=0;i<globalset.CamTotalNum;i++)
			//{
			//	InitCamDualData(i);
			//	StartNormalCamThread(i);
	 	//	}			
		}
		
	}
	else 
	{   
		InitCamDualData(CamID);
		AAstate[CamID] = 0;
		AAstate[2] =0;
		StartNormalCamThread(CamID);  
	} 
	return 0;

}

int OFccmControl::StartDualCamThread()
{
	for (int i=0;i<2;i++)
	{	
		InitCamDualData(i);
		KillDataBuffer(i); //删除缓存
		GetBuffer(i); 
		Cam[i].bState.bLoopFlag = TRUE;
		Cam[i].hThreadCtrl.m_hGrabPause    = CreateEvent(NULL, TRUE, TRUE, NULL);   //暂停显示线程
		Cam[i].hThreadCtrl.m_hDisPlayPause = CreateEvent(NULL, TRUE, TRUE, NULL);
		Cam[i].hThreadCtrl.m_hCameraThreadEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
		Cam[i].hThreadCtrl.m_hDisplayThreadEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);  

		Cam[i].bState.bCameraStarted = TRUE;	 
		Cam[i].bState.bCameraStandby = FALSE;
		Cam[i].bState.bCameraOffLine = FALSE;
	}
	 
	AfxBeginThread(GrabFramThread_Cam,this);
	Sleep(50);
	if (Cam[CamID].bOnlyOpenCamera == TRUE)
	{
		AfxBeginThread(DisplayThread_Cam0OnlyOpen,this);
		Sleep(50);
		AfxBeginThread(DisplayThread_Cam1OnlyOpen,this);
		Sleep(50);
	}
	else
	{
		AfxBeginThread(DisplayThread_Cam0,this);
		Sleep(50);
		AfxBeginThread(DisplayThread_Cam1,this);
		Sleep(50);
	}
	
	return 0;
}

int OFccmControl::StartNormalCamThread(int CamID)
{ 
	CString strConfigFileName=OFccmControl::globalset.GetCamConfigPath(CamID);
	Cam[CamID].pConfig->SetConfigFilePath(strConfigFileName);
	Cam[CamID].pConfig->pConfigFileRw->SetConfigFilePath(strConfigFileName);
	Cam[CamID].pConfig->LoadSensorGenaralTab();
	Cam[CamID].bTripleBufferDirty = FALSE;
	KillDataBuffer(CamID); //删除缓存
	GetBuffer(CamID); 
	Cam[CamID].bState.bLoopFlag = TRUE;
	Cam[CamID].hThreadCtrl.m_hGrabPause    = CreateEvent(NULL, TRUE, TRUE, NULL);   //暂停显示线程
	Cam[CamID].hThreadCtrl.m_hDisPlayPause = CreateEvent(NULL, TRUE, TRUE, NULL);
	Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);
	Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead = CreateEvent(NULL, TRUE, FALSE, NULL);  

	Cam[CamID].bState.bCameraStarted = TRUE;	 
	Cam[CamID].bState.bCameraStandby = FALSE;
	Cam[CamID].bState.bCameraOffLine = FALSE; 

	switch(CamID)
	{
	case 0:
		AfxBeginThread(GrabFramThread_Cam0,this);
		Sleep(50);
		if (Cam[CamID].bOnlyOpenCamera == TRUE)
		{
			AfxBeginThread(DisplayThread_Cam0OnlyOpen,this);
		}
		else 
		{
			AfxBeginThread(DisplayThread_Cam0,this);
		}
		break;
	case 1:
		Sleep(50);
		AfxBeginThread(GrabFramThread_Cam1,this);
		Sleep(50);
		if (Cam[CamID].bOnlyOpenCamera == TRUE)
		{
			AfxBeginThread(DisplayThread_Cam1OnlyOpen,this);
		}
		else
		{
			AfxBeginThread(DisplayThread_Cam1,this);
		}
		
		break;
	} 
	 
	return 0;
}
 
int OFccmControl::CloseCamera_Final(int CamID, BOOL bResult)
{
	int nCnt = 0;
	while(nCnt < 10)
	{
		if ((Cam[CamID].bState.bGrabThreadRunSate == 1)&&(Cam[CamID].bState.bDisplayThreadRunSate == 1))
		{
			break;
		}
		fSleep(300);
		nCnt++;
	}
	//等待抓取线程结束事件
	if( Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead != NULL)
	{
		WaitForSingleObject(Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead, INFINITE); 
		CloseHandle(Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead);
		Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead = NULL;
	}
	//等待显示处理线程结束事件
	if( Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead != NULL )
	{
		WaitForSingleObject(Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead, INFINITE); 
		CloseHandle(Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead);
		Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead = NULL;
	}
	//		SaveWorkingCurrent(CamID); //存储提示工作电流

	Cam[CamID].pCameraCtrl->CloseCamera(CamID);//关闭摄像头
	Sleep(100);	

	//发消息通知各项测试DLL结束
	Cam[CamID].pTestCtrl->PostMessageToItems(WM_CAMERA_STOP);

	Cam[CamID].bState.bCameraStarted = FALSE;

//	if (Cam[CamID].pConfig->GetGeneralTab()->m_TestMode == TESTMODE_MANUAL)
//	{
		UpdateUI(UI_STOP_SUCESS,CamID);
//	}

	ShowTextInfo(_T(""),CamID) ;
	if (Cam[CamID].pConfig->GetGeneralTab()->SocketEn)
	{
		if ((globalset.CamType==L"Dual"))
		{
			DualAutoTestSendMsgMachine(CamID);
		}
		else
		{
			SendMsgMachine(CamID);
		}
	} 
	Ctrl_Addlog(CamID,_T("模组已经关闭"),COLOR_BLUE,220);

	KillDataBuffer(CamID); //删除缓存
 	if((globalset.CamType==L"Dual")&&(Cam[CamID].pConfig->GetGeneralTab()->m_TestMode==TESTMODE_AUTO )) 
 	{
 		if (Cam[0].pTestCtrl->bTestResult==FALSE||Cam[1].pTestCtrl->bTestResult==FALSE)
 			DrawingAndShowResult(CamID,_T("TEST FAIL"),FALSE,0);
 		else
 			DrawingAndShowResult(CamID,_T("TEST PASS"),TRUE,0);
 	}

	if((globalset.CamType==L"Normal")&&(Cam[0].pConfig->GetGeneralTab()->m_TestMode==TESTMODE_AUTO )) 
	{
		if (Cam[0].pTestCtrl->bTestResult==FALSE)
			DrawingAndShowResult(0,_T("TEST FAIL"),FALSE,0);
		else
			DrawingAndShowResult(0,_T("TEST PASS"),TRUE,0);
	}

	return 0;
}
int OFccmControl::CloseCamera(int CamID,BOOL bResult)
{ 
	if (Cam[CamID].bState.bCameraStandby||Cam[CamID].bState.bCameraRunSate==1) 
	{
		return 1;
	}
	 
	AAstate[0] =0;
	AAstate[1] =0;
	AAstate[2] =0;
	
	isOpen[CamID] = FALSE;

	int waitComplete = 100;//3000;
	Cam[CamID].bState.bCameraStandby = TRUE;
	Cam[CamID].bState.bCameraRunSate = 0;
	Cam[CamID].bState.bLoopFlag = FALSE;  //循环标志结束
	SetEvent(Cam[CamID].hThreadCtrl.m_hGrabPause);
    fSleep(200);
	SAFE_CLOSE_HANDLE(Cam[CamID].hThreadCtrl.m_hGrabPause);//关闭暂停现场句柄

	SetEvent(Cam[CamID].hThreadCtrl.m_hDisPlayPause);
    fSleep(200);
	SAFE_CLOSE_HANDLE(Cam[CamID].hThreadCtrl.m_hDisPlayPause);//关闭暂停现场句柄 

	PostMessage (hTestDlgWnd,WM_UPDATEMSG,WM_SOCKET_CLOSECAMERAFINAL,(LPARAM)(CamID));
	return 0;



		//等待抓取线程结束事件
		if( Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead != NULL)
		{
			WaitForSingleObject(Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead, waitComplete); 
			CloseHandle(Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead);
			Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead = NULL;
		}
		//等待显示处理线程结束事件
		if( Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead != NULL )
		{
			WaitForSingleObject(Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead, waitComplete); 
			CloseHandle(Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead);
			Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead = NULL;
		}
//		SaveWorkingCurrent(CamID); //存储提示工作电流

		Cam[CamID].pCameraCtrl->CloseCamera(CamID);//关闭摄像头
		Sleep(200);	

		//发消息通知各项测试DLL结束
		Cam[CamID].pTestCtrl->PostMessageToItems(WM_CAMERA_STOP);

		Cam[CamID].bState.bCameraStarted = FALSE;
		 
		UpdateUI(UI_STOP_SUCESS,CamID); 
		 
		ShowTextInfo(_T(""),CamID) ;
		if (Cam[CamID].pConfig->GetGeneralTab()->SocketEn)
		{
			SendMsgMachine(CamID);
		} 
		Ctrl_Addlog(CamID,_T("模组已经关闭"),COLOR_BLUE,220);

		KillDataBuffer(CamID); //删除缓存

		//UpdateToMes(CamID);
// 	}
// 	
	return 0;
}


UINT OFccmControl::GrabFramThread_Cam(void* param)
{
	OFccmControl* pTccm = static_cast<OFccmControl*>(param);
	pTccm->GrabFramThread();
	pTccm = NULL;
	return 0;
}

UINT OFccmControl::GrabFramThread_Cam0(void* param)
{
	OFccmControl* pTccm = static_cast<OFccmControl*>(param);
	pTccm->GrabFramThread(0);
	pTccm = NULL;
	return 0;
}
UINT OFccmControl::GrabFramThread_Cam1(void* param)
{
	OFccmControl* pTccm = static_cast<OFccmControl*>(param);
	pTccm->GrabFramThread(1);
	pTccm = NULL;
	return 0;
}

UINT OFccmControl::DisplayThread_Cam0(void* param)
{
	OFccmControl* pTccm = static_cast<OFccmControl*>(param);
	pTccm->DisplayThread(0);
	pTccm = NULL;
	return 0;
}

UINT OFccmControl::DisplayThread_Cam0OnlyOpen(void* param)
{
	OFccmControl* pTccm = static_cast<OFccmControl*>(param);
	pTccm->DisplayThreadOnly(0);
	pTccm = NULL;
	return 0;
}

UINT OFccmControl::DisplayThread_Cam1(void* param)
{
	OFccmControl* pTccm = static_cast<OFccmControl*>(param);
	pTccm->DisplayThread(1);
	pTccm = NULL;
	return 0;
}

UINT OFccmControl::DisplayThread_Cam1OnlyOpen(void* param)
{
	OFccmControl* pTccm = static_cast<OFccmControl*>(param);
	pTccm->DisplayThreadOnly(1);
	pTccm = NULL;
	return 0;
}

UINT OFccmControl::ControlThread(void* param)
{
	OFccmControl* pTccm = static_cast<OFccmControl*>(param);
	pTccm->ControlThread();
	pTccm = NULL;
	return 0;
}

void OFccmControl::SendMachineTestResult(int CamID,BOOL bResult,int error_type)
{
	if(pMachineCtrl!=NULL)
		pMachineCtrl->CameraReturnResult(CamID,bResult,error_type);
}
int OFccmControl::MachineOpenCamBack(int CamID,BOOL bResult)
{
	int i=CamID;
	if(!bResult)
	{
		if(globalset.bMachine)
			SendMachineTestResult(i,FALSE);

		if(globalset.CamType == L"Dual")
		{
			if(Cam[1].pConfig->GetGeneralTab()->SocketEn)
			{
				if(Cam[1].pServerSocket!=NULL && Cam[1].pServerSocket->m_client!= NULL)
				{
					EnterCriticalSection(&proc_cs);
					if(CamID==0) AAstate[0] =2;
					if(CamID==1) AAstate[1] =2;
					if((AAstate[0] ==2||AAstate[1] ==2 )&& AAstate[2] ==0)
					{
						Ctrl_Addlog(1,_T("向Handler发送FAIL命令"),COLOR_GREEN);
						Cam[1].pServerSocket->m_client->Send("FAIL\n",5);
						Ctrl_Addlog(1,_T("FAIL命令发送成功"),COLOR_GREEN);
						AAstate[2] =1;
						AAstate[3] =1;
					} 
					LeaveCriticalSection(&proc_cs);
				}
			}
		}
		
	}
	else
	{
		if(globalset.bMachine)
			SendMachineTestResult(i,TRUE);

		if(globalset.CamType == L"Dual")
		{

			if(Cam[1].pConfig->GetGeneralTab()->SocketEn)
			{
				if(Cam[1].pServerSocket!=NULL && Cam[1].pServerSocket->m_client!= NULL)
				{
					EnterCriticalSection(&proc_cs);
					if(CamID==0) AAstate[0] =1;
					if(CamID==1) AAstate[1] =1;
					if((AAstate[0] == 1 &&AAstate[1] == 1 )&& AAstate[2] ==0)
					{
						Cam[1].pServerSocket->m_client->Send("PASS\n",5);
						AAstate[2] =1;
					} 
					LeaveCriticalSection(&proc_cs);
				}
			}
		}
	}
	return 0;
}

int OFccmControl::OpenPassProc(int CamID)
{
	Cam[CamID].bState.bCameraStandby=FALSE;
	Cam[CamID].bState.bCameraRunSate = 2;
	Cam[CamID].pCameraCtrl->SetLocalExp(); 
	MachineOpenCamBack( CamID, TRUE);
	UpdateUI(UI_PLAY_SUCESS,CamID);
    Ctrl_Addlog(CamID,_T("点亮OK！"),COLOR_GREEN,200);
	return 1;
}
int OFccmControl::OpenFailProc(int CamID)
{
	Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_FINISHED;
	SetEvent(Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead);
	Cam[CamID].bState.bCameraRunSate=0;
	CloseCamera(CamID,0);

	int CamType = Ctrl_GetCamType();

	if(CamType==1)//Dual
	{
		Sleep(500);
		int CammeraID=0;
		if(CamID==0) CammeraID=1;
		else CammeraID=0;
		int index=0;
		while (Cam[CammeraID].bState.bCameraRunSate==1)
		{
			Sleep(300);
			index++;
			if(index>30) break;
		}
		Ctrl_Addlog(CamID,Cam[CammeraID].bState.bCameraRunSate,COLOR_RED,200);
		//	if(Cam[CammeraID].bState.bCameraRunSate!=1
		if(Cam[CammeraID].bState.bCameraRunSate == 2)     //仅在一颗模组点亮失败，另一颗模组点亮状态关闭另一颗模组 
		{
			Ctrl_Addlog(CamID,_T("关闭另外一颗模组！"),COLOR_RED,200);
			pCcmCtrl->CloseCamera(CammeraID,0);
		}
	}
	Ctrl_SetCamErrorCode(CamID|0x02,2);
	MachineOpenCamBack( CamID,  FALSE);
	UpdateUI(UI_PLAY_FAILED,CamID);
	Ctrl_Addlog(CamID,_T("点亮失败！"),COLOR_RED,200);

	CString errorInfo = Ctrl_GetCamErrorInfo(CamID);

	if(errorInfo == L"TEST FAIL!")
		Ctrl_SetCamErrorInfo(CamID,_T("点亮失败！"));

	if(Cam[CamID].pConfig->GetGeneralTab()->m_TestMode == TESTMODE_AUTO)      //自动模式点不亮要刷屏
	{
		DrawingAndShowResult(CamID,_T("点亮失败"),0);
	}
	return 1;
}
int OFccmControl::GrabFramThread()
{
#if 0
	int flag  = 0;
	for(int i=0;i<2;i++)
	{
		flag = CamStartInit(i);
		if(flag!=1)
		{
			OpenFailProc(i);
			return 0;
		}
	}

	flag = Cam[0].pCameraCtrl->OpenCamera(2); 
	
	if(flag!=1)
	{
		Ctrl_SetCamErrorInfo(CamID,_T("点亮失败！"));

		for(int i=0;i<2;i++)
		{ 
			OpenFailProc(i); 
		}
		return 0;
	} 
	for(int i=0;i<2;i++)
	{
		if (Cam[i].pImgSensor!=NULL)
		{
			Cam[i].fuseID = L"";
			Cam[i].pImgSensor->GetFuseID(Cam[i].fuseID);
		}
	}	
	 
	/*if(!mesCheck(0) || !mesCheck(1))
	{
		for(int i=0;i<2;i++)
		{ 
			OpenFailProc(i); 
		}
		return 0;
	}*/

#pragma region
	 //启动成功，判断抓图是否成功
	UINT timeStart = 0;
	UINT timeStart0 = 0;
	UINT timeEnd = 0;
	UINT timeEnd0 = 0; 
	UINT FrameErrCnt = 0;
	UINT DeviceErrCnt = 0;
	float GrabFPS = 0.00;
	CString FpsInfo;
	ULONG RetSize=0;

	int x = 0;
	FrameInfo m_FrameInfo;
	CString mycharstr;
	int bRet = 0;
	WORD ImgH = 0, ImgV = 0;
	timeStart = GetTickCount();
	timeStart0 = GetTickCount();
#pragma endregion

	Cam[0].m_FrameInfo.PlayCnt = 0;
	Cam[0].m_FrameInfo.FrameCnt = 0;
	Cam[1].m_FrameInfo.PlayCnt = 0;
	Cam[1].m_FrameInfo.FrameCnt = 0;

	
	int m_GrabSize = 0;// = (Cam[0].m_FrameInfo.m_GrabSize>Cam[1].m_FrameInfo.m_GrabSize)?Cam[0].m_FrameInfo.m_GrabSize:Cam[1].m_FrameInfo.m_GrabSize;

	m_GrabSize = pDevice->GetGrabSize()+1024*5;
	BYTE* pGrabBuf = new BYTE[m_GrabSize];

	CString str;
	str.Format(_T("DualCam %d GrabThread"),CamID);
	Ctrl_AddToLog(str,CamID); 

	//判断抓图是否成功 ，如果没有开启强制显示
#pragma region
	if(!Cam[CamID].pConfig->GetGeneralTab()->bForceShow)
	{
		BOOL OpenPass[2] = {FALSE};
		for(int i=0;i<50;i++)
		{
			bRet = pDevice->GrabFrame(pGrabBuf,m_GrabSize ,&RetSize,&m_FrameInfo,0);
			if (m_FrameInfo.byChannel==1)
			{
				if(bRet ==1 )
					OpenPass[0] = TRUE;
			}
			else if(m_FrameInfo.byChannel==2)
			{
				if(bRet==1)
					OpenPass[1] = TRUE;
			}
			if(OpenPass[0]&&OpenPass[1])
			{
				break;
			}
			else if(i==49)
			{  
				OpenFailProc(0);
				OpenFailProc(1);
				return 0;
			}
		}
	}
#pragma endregion

	//执行抓图成功后操作
	for(int i=0;i<globalset.CamTotalNum;i++)
	{ 
		OpenPassProc(i);
		//OpenPassProc(1); 
	}
	 
	//启动循环
#pragma region
	while (Cam[0].bState.bLoopFlag&&Cam[1].bState.bLoopFlag)
	{ 
		bRet = pDevice->GrabFrame(pGrabBuf,m_GrabSize ,&RetSize,&m_FrameInfo,0); //pGrabBuf
		if(Cam[CamID].pConfig->GetGeneralTab()->bForceShow) bRet = 1;
		if (bRet<0)
		{
			FrameErrCnt++;

			DeviceErrCnt = 0;
			Sleep(1);
		}

		if (m_FrameInfo.byChannel==1)
		{
			if(bRet ==1)
			{   
				memcpy(Cam[0].pBuf.grab_buffer,pGrabBuf,RetSize);
				BOOL res=TripleBufferFlip(&Cam[0].pBuf.grab_buffer, TRUE,0);
				Cam[0].m_FrameInfo.PlayCnt++;
				Cam[0].m_FrameInfo.FrameCnt++;

				DeviceErrCnt = 0; 
				if (Cam[0].m_FrameInfo.PlayCnt%10==0)			 
				{
					timeEnd0 = GetTickCount() - timeStart0;//
					GrabFPS = (float)Cam[0].m_FrameInfo.PlayCnt*1000/(float)timeEnd0;
					Cam[0].m_FrameInfo.m_strFrameInfo.Format(_T("RetSize%d fps=%04.1f"), RetSize, GrabFPS); 
					ShowWorkingCurrentAndVoltage(Cam[0].m_FrameInfo.m_strFrameInfo,0);
					Cam[0].m_FrameInfo.PlayCnt = 0;
					timeEnd0 = 0;
					timeStart0 = GetTickCount();
				}
			}
			else
			{
				UpdateUI(UI_PLAY_FAILED,1);
				Ctrl_Addlog(CamID,_T("抓图像失败，请检查连接！3"),COLOR_RED,200);
				Ctrl_SetCamErrorInfo(CamID,_T("抓图像失败，请检查连接！"));
				if (globalset.CamType.Find(L"Dual")!=-1)
				{
					SendMachineTestResult(0,FALSE,1);
					SendMachineTestResult(1,FALSE,1);
				}
				else
				{
					SendMachineTestResult(CamID,FALSE,1);
				}
				

				CloseCamera(1,0);
			}
		}
		else if(m_FrameInfo.byChannel==2)
		{
			if(bRet==1)
			{
				memcpy(Cam[1].pBuf.grab_buffer,pGrabBuf,RetSize);
				BOOL res=TripleBufferFlip(&Cam[1].pBuf.grab_buffer, TRUE,1);
				Cam[1].m_FrameInfo.FrameCnt++;
				Cam[1].m_FrameInfo.PlayCnt++;

				DeviceErrCnt = 0; 
				if (Cam[1].m_FrameInfo.PlayCnt%10==0)			 
				{
					timeEnd = GetTickCount() - timeStart;//
					GrabFPS = (float)Cam[1].m_FrameInfo.PlayCnt*1000/(float)timeEnd;
					Cam[1].m_FrameInfo.m_strFrameInfo.Format(_T("RetSize%d fps=%04.1f"), RetSize, GrabFPS); 
					ShowWorkingCurrentAndVoltage(Cam[1].m_FrameInfo.m_strFrameInfo,1);
					Cam[1].m_FrameInfo.PlayCnt = 0;
					timeEnd = 0;
					timeStart = GetTickCount();
				}				
			}
			else
			{
				UpdateUI(UI_PLAY_FAILED,1);
				Ctrl_Addlog(CamID,_T("抓图像失败，请检查连接！4"),COLOR_RED,200);
				Ctrl_SetCamErrorInfo(CamID,_T("抓图像失败，请检查连接！"));
				if (globalset.CamType.Find(L"Dual")!=-1)
				{
					SendMachineTestResult(0,FALSE,1);
					SendMachineTestResult(1,FALSE,1);
				}
				else
				{
					SendMachineTestResult(CamID,FALSE,1);
				}
				CloseCamera(1,0);
			}
		}
		WaitForSingleObject(Cam[0].hThreadCtrl.m_hGrabPause, INFINITE);
		WaitForSingleObject(Cam[1].hThreadCtrl.m_hGrabPause, INFINITE);
	}
#pragma endregion
	SetEvent(Cam[0].hThreadCtrl.m_hCameraThreadEventDead);
	SetEvent(Cam[1].hThreadCtrl.m_hCameraThreadEventDead);
	//::AfxEndThread(0);
	SAFE_DELETE_ARRAY(pGrabBuf);
#endif
	return 0;
}

 
int OFccmControl::GrabFramThread(int CamID)
{	
	Cam[CamID].bState.bGrabThreadRunSate = 0;
	//int i = CamID;
	int flag  = 0;
	int errorFramLimit = globalset.errorFramLimit;
	Cam[CamID].TestItemPass=0;
	Cam[CamID].NGTestItemID=0;
	Ctrl_Addlog(CamID,_T("进入抓帧线程."),COLOR_BLUE,200);
	Cam[CamID].bState.bCameraRunSate = 1;

//	int icnt=0;	
//	do 
//	{
		flag = CamStartInit(CamID);
//		icnt++;
//	} while (flag != 1 && icnt < 3);

			
	if(flag!=1)
	{
		if (globalset.CamType==_T("Dual"))
		{
			Cam[0].NGTestItemID=-2;
			Cam[1].NGTestItemID=-2;
			Cam[0].pTestCtrl->bTestResult=FALSE;
			Cam[1].pTestCtrl->bTestResult=FALSE;
		}
		else
		{
		    Cam[CamID].NGTestItemID=-2;
			Cam[CamID].pTestCtrl->bTestResult=FALSE;
		}
		Cam[CamID].bState.bGrabThreadRunSate = 1;
		OpenFailProc(CamID); 
		return 0; 

	}
		
	pCcmCtrl->Ctrl_Addlog(CamID,_T("Mark0."),COLOR_BLUE,200);   //Mak0：OS/Load init setting ok
	if(Cam[CamID].bState.bCameraStandby == FALSE)
	{
//		icnt = 0;
//		do 
//		{
			flag = Cam[CamID].pCameraCtrl->OpenCamera(CamID);
//			icnt++;
//		} while (flag != 1 && icnt<3);
	}
	else flag=0;

		pCcmCtrl->Ctrl_Addlog(CamID,_T("Mark1."),COLOR_BLUE,200);   //Mark1：度信初始化、上电、sensor初始化等
		
		if(flag!=1)
		{
			Ctrl_SetCamErrorInfo(CamID,_T("点亮失败！"));
			if (globalset.CamType==_T("Dual"))
			{
				Cam[0].NGTestItemID=-2;
				Cam[1].NGTestItemID=-2;
				Cam[0].pTestCtrl->bTestResult=FALSE;
				Cam[1].pTestCtrl->bTestResult=FALSE;
			}
			else
			{
				Cam[CamID].NGTestItemID=-2;
				Cam[CamID].pTestCtrl->bTestResult=FALSE;
			}
			Cam[CamID].bState.bGrabThreadRunSate = 1;
			OpenFailProc(CamID); 
			return 0;
		}

	if(flag!=1)
	{
		Ctrl_SetCamErrorInfo(CamID,_T("点亮失败！"));
		if (globalset.CamType==_T("Dual"))
		{
			Cam[0].NGTestItemID=-2;
			Cam[1].NGTestItemID=-2;
			Cam[0].pTestCtrl->bTestResult=FALSE;
			Cam[1].pTestCtrl->bTestResult=FALSE;
		}
		else
		{
		    Cam[CamID].NGTestItemID=-2;
			Cam[CamID].pTestCtrl->bTestResult=FALSE;
		}
		Cam[CamID].bState.bGrabThreadRunSate = 1;
		OpenFailProc(CamID); 
		return 0;
	}
	if (flag==1)
	{
		if(CheckStandbyCurrent(CamID)==0)//检查存储工作电流
		{
			if (globalset.CamType==_T("Dual"))
			{
				Cam[0].NGTestItemID=-1;
				Cam[1].NGTestItemID=-1;
				Cam[0].pTestCtrl->bTestResult=FALSE;
				Cam[1].pTestCtrl->bTestResult=FALSE;
			}
			else
			{
			    Cam[CamID].NGTestItemID=-1;
				Cam[CamID].pTestCtrl->bTestResult=FALSE;
			}
			Cam[CamID].bState.bGrabThreadRunSate = 1;
			OpenFailProc(CamID); 
			return 0;
		}
	}

#pragma region
	UINT* FrameCnt = &Cam[CamID].m_FrameInfo.FrameCnt;
	UINT* PlayCnt = &Cam[CamID].m_FrameInfo.PlayCnt;
	UINT timeStart = 0;
	UINT timeEnd = 0;
	UINT FrameNum = 0;

	UINT FrameErrCnt = 0;
	UINT DeviceErrCnt = 0;
	float GrabFPS = 0.00;
	CString FpsInfo;
	ULONG RetSize=0;

	int x = 0;
	FrameInfo m_FrameInfo;
	CString mycharstr;
	int bRet = 0;
	WORD ImgH = 0, ImgV = 0;
	timeStart = GetTickCount();

	CString str;
	str.Format(_T("Cam %d GrabThread m_GrabSize%d"),CamID,Cam[CamID].m_FrameInfo.m_GrabSize);
	Ctrl_AddToLog(str,CamID); 
#pragma endregion
	Cam[CamID].m_FrameInfo.m_GrabSize = pDevice->GetGrabSize();
#pragma region
	//如果没有开启强制显示，抓图判断是否启动成功
	if(!Cam[CamID].pConfig->GetGeneralTab()->bForceShow)
	{
		for(int i=0;i<10;i++)
		{
			if(globalset.DeviceTypeSel != _T("UH920"))  // MUD952 UV910
			{
				if(Cam[CamID].bState.bCameraStandby == FALSE)
				{
				   bRet = pDevice->GrabFrame(Cam[CamID].pBuf.grab_buffer,Cam[CamID].m_FrameInfo.m_GrabSize ,&RetSize,&m_FrameInfo,CamID);
				}
				else bRet=0;
				Cam[CamID].m_FrameInfo.m_GrabSize = RetSize;
			}
			else
			{
				//bRet = pDevice->GrabFrame(Cam[CamID].pBuf.grab_buffer,Cam[CamID].m_FrameInfo.m_GrabSize ,&RetSize,&m_FrameInfo,0);
				bRet = pDevice->GrabFrame(Cam[CamID].pBuf.grab_buffer,Cam[CamID].m_FrameInfo.m_GrabSize ,&RetSize,&m_FrameInfo, CamID);//应该穿CamID
				Cam[CamID].m_FrameInfo.m_GrabSize = RetSize;
			}
			Sleep(30);
			if(bRet==1)
			{
				break;
			}
			else if(i==5)
			{ 
				Ctrl_SetCamErrorInfo(CamID,L"治具接触不稳定，请检查治具连接！0");
				if (globalset.CamType==_T("Dual"))
				{
					Cam[0].NGTestItemID=-2;
					Cam[1].NGTestItemID=-2;
					Cam[0].pTestCtrl->bTestResult=FALSE; 
					Cam[1].pTestCtrl->bTestResult=FALSE;
				}
				else
				{
					Cam[CamID].NGTestItemID=-2;
					Cam[CamID].pTestCtrl->bTestResult = FALSE; 
				}
				
				Cam[CamID].bState.bGrabThreadRunSate = 1;
				OpenFailProc(CamID); 
				return 0;
			}
		}
	}
#pragma endregion
	 
	OpenPassProc(CamID);  

	pCcmCtrl->Ctrl_Addlog(CamID,_T("Mark2."),COLOR_BLUE,200);     //Mark2：点亮OK，并能正常出图
	if ( Cam[CamID].pImgSensor!=NULL)
	{
		Cam[CamID].fuseID = L"";
		Cam[CamID].pImgSensor->GetFuseID(Cam[CamID].fuseID);
	}

	pCcmCtrl->Ctrl_Addlog(CamID,_T("Mark3."),COLOR_BLUE,200);   //Mark3：获取FuseID成功

#pragma region 
	strDisfps=_T("");
	while (Cam[CamID].bState.bLoopFlag)
	{
		if(globalset.DeviceTypeSel != _T("UH920"))
		{
             bRet = pDevice->GrabFrame(Cam[CamID].pBuf.grab_buffer,Cam[CamID].m_FrameInfo.m_GrabSize ,&RetSize,&m_FrameInfo,CamID);
			// Cam[CamID].m_FrameInfo.m_GrabSize = RetSize;
		}
		else
		{
			//bRet = pDevice->GrabFrame(Cam[CamID].pBuf.grab_buffer,Cam[CamID].m_FrameInfo.m_GrabSize ,&RetSize,&m_FrameInfo,0);
			bRet = pDevice->GrabFrame(Cam[CamID].pBuf.grab_buffer,Cam[CamID].m_FrameInfo.m_GrabSize ,&RetSize,&m_FrameInfo, CamID);//应该穿CamID
			//Cam[CamID].m_FrameInfo.m_GrabSize = RetSize;
		}
		
		if(Cam[CamID].pConfig->GetGeneralTab()->bForceShow) bRet = 1;
		if(bRet == 1)
		{			 
			BOOL res=TripleBufferFlip(&Cam[CamID].pBuf.grab_buffer, TRUE,CamID);

			(*FrameCnt)++; 
			FrameNum++; 
			DeviceErrCnt = 0;
			FrameErrCnt = 0;
		}
		else if(bRet == 0)
		{ 
			FrameErrCnt++;
			if(FrameErrCnt > errorFramLimit)
			{    
				Ctrl_Addlog(CamID,_T("治具接触不稳定，请检查治具连接！1"),COLOR_RED,200);
				Ctrl_SetCamErrorInfo(CamID,_T("治具接触不稳定，请检查治具连接！"));
				if (globalset.CamType.Find(L"Dual")!=-1)
				{
					SendMachineTestResult(0,FALSE,1);
					SendMachineTestResult(1,FALSE,1); 
					Cam[0].pTestCtrl->bTestResult=FALSE;
					Cam[1].pTestCtrl->bTestResult=FALSE;
					CloseCameraMsg(0);
					CloseCameraMsg(1);
				}
				else
				{
					SendMachineTestResult(CamID,FALSE,1);
					Cam[CamID].pTestCtrl->bTestResult=FALSE;
					CloseCamera(CamID,0); 
					DrawingAndShowResult(CamID,_T("治具接触不稳定，请检查治具连接！"),0);
				}
				
				break;
			}
			DeviceErrCnt = 0;
			Sleep(1);
		}
		else
		{
// 			int Current[5] = {0};
// 			pDevice->PmuGetCurrent(Current,5,0,CamID);
// 			int Voltage[5] = {0};
// 			pDevice->PmuGetVoltage(Voltage,5,0,CamID);

			DeviceErrCnt++;
			if(DeviceErrCnt > errorFramLimit)
			{
				if (globalset.CamType==_T("Dual"))
				{
					Cam[0].NGTestItemID=-2;
					Cam[1].NGTestItemID=-2;
					Cam[0].pTestCtrl->bTestResult=FALSE;
					Cam[1].pTestCtrl->bTestResult=FALSE;
				}
				else
				{
					Cam[CamID].NGTestItemID=-2;
					Cam[CamID].pTestCtrl->bTestResult=FALSE;
				}
				Ctrl_Addlog(CamID,_T("治具接触不稳定，请检查治具连接！2"),COLOR_RED,200);
				Ctrl_SetCamErrorInfo(CamID,_T("治具接触不稳定，请检查治具连接！"));
				
				if (globalset.CamType.Find(L"Dual")!=-1)
				{
					SendMachineTestResult(0,FALSE,1);
					SendMachineTestResult(1,FALSE,1);
					CloseCameraMsg(0);
					CloseCameraMsg(1);
					Sleep(300);
					if(Cam[CamID].pConfig->GetGeneralTab()->m_TestMode == TESTMODE_AUTO)      //自动模式点不亮要刷屏
					{
						DrawingAndShowResult(0,_T("治具接触不稳定，请检查治具连接！"),0);
						DrawingAndShowResult(1,_T("治具接触不稳定，请检查治具连接！"),0);
					}
				}
				else
				{
					SendMachineTestResult(CamID,FALSE,1);
					Cam[CamID].pTestCtrl->bTestResult=FALSE;
					CloseCamera(CamID,0);
					DrawingAndShowResult(CamID,_T("治具接触不稳定，请检查治具连接！"),0);
					
				}
				
				break;
			}
			Sleep(1);  
		}
		 
		if ((*FrameCnt)%20==0)			 
		{
			timeEnd = GetTickCount() - timeStart;//
			GrabFPS = (float)(*FrameCnt)*1000/(float)timeEnd;
			 
			Cam[CamID].m_FrameInfo.m_strFrameInfo.Format(_T("fps=%04.1f"),GrabFPS);
			Cam[CamID].m_FrameInfo.m_strFrameInfo=Cam[CamID].m_FrameInfo.m_strFrameInfo+strDisfps;
		    ShowWorkingCurrentAndVoltage(Cam[CamID].m_FrameInfo.m_strFrameInfo,CamID);
			(*FrameCnt) = 1;
			timeEnd = 0;
			timeStart = GetTickCount();
		}
		WaitForSingleObject(Cam[CamID].hThreadCtrl.m_hGrabPause, INFINITE);
	}	
#pragma endregion

	SetEvent(Cam[CamID].hThreadCtrl.m_hCameraThreadEventDead);
	//::AfxEndThread(0);
	FrameCnt = NULL;
	PlayCnt = NULL;
	Cam[CamID].bState.bGrabThreadRunSate = 1;
	return 0;
}

int OFccmControl::DisplayThread(int CamID)
{	
	Cam[CamID].bState.bDisplayThreadRunSate = 0;
	UINT type = Cam[CamID].pConfig->GetSensorTab()->type;
	UINT width =  Ctrl_GetImgInfo(CamID)->imgwidth;
	UINT height =  Ctrl_GetImgInfo(CamID)->imgheight;

	int rawformat =  Ctrl_GetImgInfo(CamID)->RawFormat;
	int rawBits   =  Ctrl_GetImgInfo(CamID)->RawBits;
	int TimeTest = GetTickCount();
	BOOL bTest = FALSE;

	int disPlayTime = GetTickCount();

	int isShow = 0;
	int iDisplayCnt=0;
	int ShowGap = globalset.DisplayInterval; 
	float Disfps=0;
	if(ShowGap==0) ShowGap=1;
	while (Cam[CamID].bState.bLoopFlag)
	{ 
		width =  Ctrl_GetImgInfo(CamID)->imgwidth;
		height =  Ctrl_GetImgInfo(CamID)->imgheight;
		if (!TripleBufferFlip(&Cam[CamID].pBuf.display_buffer, FALSE,CamID))
		{
			continue;
		} 
		
		if(Cam[CamID].m_FrameInfo.FrameCnt)
		{ 
			EnterCriticalSection(&Cam[CamID].m_DisPlayLock);
		
			switch (type)
			{ 	
			case D_MIPI_RAW10:
			case D_RAW10:
				//break;
				if((globalset.DeviceTypeSel==_T("R3"))||(globalset.DeviceTypeSel==_T("R5")))
					memcpy(Cam[CamID].pBuf.raw_buffer,Cam[CamID].pBuf.display_buffer,width*height*2);
				else
				    ImageProc::DTRaw10toRaw(Cam[CamID].pBuf.display_buffer, Cam[CamID].pBuf.raw_buffer, width, height);

				//Raw10插值到RGB24 DT插值
				ImageProc::RawtoRGB24_DT(Cam[CamID].pBuf.raw_buffer, Cam[CamID].pBuf.RGB24_buffer, width, height, Cam[CamID].m_ImageInfo.RawFormat, rawBits);	//切换点亮设定后，outformat未变化问题
				//ImageProc::RawtoRGB24MotoDemosaic(Cam[CamID].pBuf.raw_buffer, Cam[CamID].pBuf.RGB24_buffer, width, height, rawformat, rawBits);

				break;
			case D_MIPI_RAW8: //D_RAW8 0x01		 
				ImageProc::DTRaw8toRaw(Cam[CamID].pBuf.display_buffer, Cam[CamID].pBuf.raw_buffer, width, height); 
				ImageProc::RawtoRGB24_DT(Cam[CamID].pBuf.raw_buffer, Cam[CamID].pBuf.RGB24_buffer, width, height, Cam[CamID].m_ImageInfo.RawFormat, rawBits);				
				break;
			case D_YUV:
				ImageProc::YUV422toRGB24(Cam[CamID].pBuf.display_buffer,Cam[CamID].pBuf.RGB24_buffer,width,height,Cam[CamID].pConfig->GetSensorTab()->outformat);
				ImageProc::RGB24toRaw(Cam[CamID].pBuf.RGB24_buffer, Cam[CamID].pBuf.raw_buffer, width, height, Cam[CamID].m_ImageInfo.RawFormat, rawBits); //固定为RGGB
				break;
			default:
				break;
				TRACE("不支持的图像格式!");

			} 
		
		}
	//	if(CamID==0) ManuBalance( CamID);

		
		
		if((isShow%ShowGap==0)&&(globalset.DeviceTypeSel!=_T("CTS")))
		{
			DrawToDisplayMemDc(CamID);
		}
		LeaveCriticalSection(&Cam[CamID].m_DisPlayLock);
		
		Cam[CamID].pTestCtrl->Run();

		if((isShow%ShowGap==0)&&(globalset.DeviceTypeSel!=_T("CTS")))
		{
			//显示
			Ctrl_ShowMemDC(CamID); 
			isShow = 0;
		}
		if (iDisplayCnt%20==0)
		{
			disPlayTime = GetTickCount() - TimeTest;//
			Disfps = (float)20.0*1000/(float)disPlayTime;
			strDisfps.Format(_T("(%04.1f)"),Disfps);
			TimeTest = GetTickCount();
		}
		iDisplayCnt++;
		isShow++;
		
	} 
	SetEvent(Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead);
	//::AfxEndThread(0);
	Cam[CamID].bState.bDisplayThreadRunSate = 1;
	return 0;
}

int OFccmControl::DisplayThreadOnly(int CamID)
{	
	Cam[CamID].bState.bDisplayThreadRunSate = 0;
	UINT type = Cam[CamID].pConfig->GetSensorTab()->type;
	UINT width =  Ctrl_GetImgInfo(CamID)->imgwidth;
	UINT height =  Ctrl_GetImgInfo(CamID)->imgheight;

	int rawformat =  Ctrl_GetImgInfo(CamID)->RawFormat;
	int rawBits   =  Ctrl_GetImgInfo(CamID)->RawBits;
	int TimeTest = GetTickCount();
	BOOL bTest = FALSE;

	int disPlayTime = GetTickCount();

	int isShow = 0;
	int iDisplayCnt=0;
	int ShowGap = globalset.DisplayInterval; 
	float Disfps=0;
	if(ShowGap==0) ShowGap=1;
	while (Cam[CamID].bState.bLoopFlag)
	{ 
		width =  Ctrl_GetImgInfo(CamID)->imgwidth;
		height =  Ctrl_GetImgInfo(CamID)->imgheight;
		if (!TripleBufferFlip(&Cam[CamID].pBuf.display_buffer, FALSE,CamID))
		{
			continue;
		} 

		if(Cam[CamID].m_FrameInfo.FrameCnt)
		{ 
			EnterCriticalSection(&Cam[CamID].m_DisPlayLock);

			switch (type)
			{ 	
			case D_MIPI_RAW10:
			case D_RAW10:
				//break;
				if((globalset.DeviceTypeSel==_T("R3"))||(globalset.DeviceTypeSel==_T("R5")))
					memcpy(Cam[CamID].pBuf.raw_buffer,Cam[CamID].pBuf.display_buffer,width*height*2);
				else
					ImageProc::DTRaw10toRaw(Cam[CamID].pBuf.display_buffer, Cam[CamID].pBuf.raw_buffer, width, height);

				//Raw10插值到RGB24 DT插值
				ImageProc::RawtoRGB24_DT(Cam[CamID].pBuf.raw_buffer, Cam[CamID].pBuf.RGB24_buffer, width, height, Cam[CamID].m_ImageInfo.RawFormat, rawBits);	//切换点亮设定后，outformat未变化问题
				//ImageProc::RawtoRGB24MotoDemosaic(Cam[CamID].pBuf.raw_buffer, Cam[CamID].pBuf.RGB24_buffer, width, height, rawformat, rawBits);

				break;
			case D_MIPI_RAW8: //D_RAW8 0x01		 
				ImageProc::DTRaw8toRaw(Cam[CamID].pBuf.display_buffer, Cam[CamID].pBuf.raw_buffer, width, height); 
				ImageProc::RawtoRGB24_DT(Cam[CamID].pBuf.raw_buffer, Cam[CamID].pBuf.RGB24_buffer, width, height, Cam[CamID].m_ImageInfo.RawFormat, rawBits);				
				break;
			case D_YUV:
				ImageProc::YUV422toRGB24(Cam[CamID].pBuf.display_buffer,Cam[CamID].pBuf.RGB24_buffer,width,height,Cam[CamID].pConfig->GetSensorTab()->outformat);
				ImageProc::RGB24toRaw(Cam[CamID].pBuf.RGB24_buffer, Cam[CamID].pBuf.raw_buffer, width, height, Cam[CamID].m_ImageInfo.RawFormat, rawBits); //固定为RGGB
				break;
			default:
				break;
				TRACE("不支持的图像格式!");

			} 

		}
		//	if(CamID==0) ManuBalance( CamID);



		if((isShow%ShowGap==0)&&(globalset.DeviceTypeSel!=_T("CTS")))
		{
			DrawToDisplayMemDc(CamID);
		}
		LeaveCriticalSection(&Cam[CamID].m_DisPlayLock);

//		Cam[CamID].pTestCtrl->Run();

		if((isShow%ShowGap==0)&&(globalset.DeviceTypeSel!=_T("CTS")))
		{
			//显示
			Ctrl_ShowMemDCChart(CamID); 
			isShow = 0;
		}
		if (iDisplayCnt%20==0)
		{
			disPlayTime = GetTickCount() - TimeTest;//
			Disfps = (float)20.0*1000/(float)disPlayTime;
			strDisfps.Format(_T("(%04.1f)"),Disfps);
			TimeTest = GetTickCount();
		}
		iDisplayCnt++;
		isShow++;

	} 
	SetEvent(Cam[CamID].hThreadCtrl.m_hDisplayThreadEventDead);
	//::AfxEndThread(0);
	Cam[CamID].bState.bDisplayThreadRunSate = 1;
	return 0;
}

void OFccmControl::PauseDisplay(int CamID)
{
	ResetEvent(Cam[CamID].hThreadCtrl.m_hDisPlayPause);
}

BOOL OFccmControl::TripleBufferFlip(LPBYTE *pBuf, BOOL bPrimaryBuffer, int CamID)
{
	BOOL bFlipped = FALSE;

	EnterCriticalSection(&Cam[CamID].m_csLock);
	{
		if (bPrimaryBuffer || Cam[CamID].bTripleBufferDirty)
		{
			LPBYTE pTmp = *pBuf;
			*pBuf = Cam[CamID].pBuf.transfer_buffer;
			Cam[CamID].pBuf.transfer_buffer = pTmp;
			bFlipped = TRUE;
			pTmp = NULL;
		}
		Cam[CamID].bTripleBufferDirty = bPrimaryBuffer;
	}
	LeaveCriticalSection(&Cam[CamID].m_csLock);

	return bFlipped;	

}

int OFccmControl::VcmDr_Standby(int CamID)
{
	if(Cam[CamID].pVcmDriver == NULL)
	{
		Ctrl_Addlog(CamID,_T("请选择vcm_Drv"),COLOR_RED,200);
		return 0;
	}

	return Cam[CamID].pVcmDriver->Standby();
}
int OFccmControl::VcmDr_InitAF(int CamID)
{
	if(Cam[CamID].pVcmDriver == NULL)
	{
		Ctrl_Addlog(CamID,_T("请选择vcm_Drv"),COLOR_RED,200);
		return 0;
	}
	return Cam[CamID].pVcmDriver->Init();
}

int OFccmControl::VcmDr_AutoFocus(int CamID)
{
	if(Cam[CamID].pVcmDriver == NULL)
	{
		Ctrl_Addlog(CamID,_T("请选择vcm_Drv"),COLOR_RED,200);
		return 0;
	}

	return Cam[CamID].pVcmDriver->AutoFocus();
	//return 0;
}

int OFccmControl::VcmDr_ReadAF_Code(USHORT* code,int CamID)
{
	if(Cam[CamID].pVcmDriver == NULL)
	{
		Ctrl_Addlog(CamID,_T("请选择vcm_Drv"),COLOR_RED,200);
		return 0;
	}
	return Cam[CamID].pVcmDriver->ReadCode(code);
}
int OFccmControl::VcmDr_WriteAF_Code(USHORT code,int CamID)
{
	if(Cam[CamID].pVcmDriver == NULL)
	{
		Ctrl_Addlog(CamID,_T("请选择vcm_Drv"),COLOR_RED,200);
		return 0;
	}
	if (0xFFFF == code)
	{
		return Cam[CamID].pVcmDriver->Standby();
	}
	return Cam[CamID].pVcmDriver->WriteCode(code);
}

//写入指定的分区下某子项的值（STRING）
void OFccmControl::Config_WriteConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR string,int CamID) 
{
	Cam[CamID].pConfig->pConfigFileRw->WriteConfigString(AppName,KeyName,string);
}
//写入指定的分区下某子项的值(INT)
void OFccmControl::Config_WriteConfigInt(LPCTSTR AppName, LPCTSTR KeyName,int data,int datatype ,int CamID) 
{
	Cam[CamID].pConfig->pConfigFileRw->WriteConfigInt(AppName,KeyName,data,datatype);
}
//读取指定的分区下某子项的值(INT)
int OFccmControl::Config_ReadConfigInt(LPCTSTR AppName, LPCTSTR KeyName, int default,int CamID)
{
	return Cam[CamID].pConfig->pConfigFileRw->ReadConfigData(AppName,KeyName,default);
}
//读取指定的分区下某子项的值(STRING)
LPCTSTR OFccmControl::Config_ReadConfigString(LPCTSTR AppName, LPCTSTR KeyName, LPCTSTR default,int CamID) 
{
	return Cam[CamID].pConfig->pConfigFileRw->ReadConfigString(AppName,KeyName,default);
}
//删除子项(名称和值)
void OFccmControl::Config_ClearConfig(LPCTSTR AppName,int CamID) 
{
	if(AppName!=NULL)
	{
		Cam[CamID].pConfig->pConfigFileRw->ClearConfig(AppName);
	}
}
//删除分区(分区名称和其下所有子项)
void OFccmControl::Config_ClearConfig(LPCTSTR AppName, LPCTSTR KeyName,int CamID) 
{	 
	if(AppName!=NULL)
	{
		Cam[CamID].pConfig->pConfigFileRw->ClearConfig(AppName,KeyName);
	}

}
//得到TCCF文件的路径
LPCTSTR OFccmControl::Config_GetConfigPathName(int CamID) 
{
	return Cam[CamID].pConfig->GetConfigFilePath();
}
LPCTSTR OFccmControl::Config_GetExeDir()
{
	return globalset.GetCurrentPath();
}

vector<CString> OFccmControl::GetvecDriver()
{
	CString strDriver = ::GetVcmDriverList();
	vector<CString> vecDriver;
	globalset.SeparateString(strDriver,',',&vecDriver);
	return  vecDriver;
}

vector<CString> OFccmControl::GetvecImgSensor()
{
	CString strImgSensor = ::GetImgSensorList();
	vector<CString> vecImgSensor;
	globalset.SeparateString(strImgSensor,',',&vecImgSensor);
	return  vecImgSensor;
}

int OFccmControl::Ctrl_GetOutPutFormat(int CamID)
{
	return Cam[CamID].pConfig->GetSensorTab()->outformat;
}

void OFccmControl::ExecuteItemTest(int ID,int CamID)
{
	//PostThreadMessage(ControlThreadID,TM_SINGLETEST,(WPARAM)ID,(LPARAM)CamID);
	PostMsgToControler(TM_SINGLETEST,(WPARAM)CamID,ID);   
	GetFocus();
}

int OFccmControl::GetExpCurrentY(int CamID)
{
	Ctrl_GetImgInfo(CamID);
	int width = Cam[CamID].m_ImageInfo.imgwidth;
	int height = Cam[CamID].m_ImageInfo.imgheight;
	 int Ratio = 15;
	CRect CenterRect;
	CenterRect.left   = int(width * (50 - Ratio/2)/100);
	CenterRect.right  = int(width * (50 + Ratio/2)/100);
	CenterRect.top    = int(height* (50 - Ratio/2)/100);
	CenterRect.bottom = int(height* (50 + Ratio/2)/100);
	BYTE* Y_buffer = new BYTE[width*height];
	ImageProc::RGB24toY(Cam[CamID].m_ImageInfo.RGB24_buffer,Y_buffer,width,height);
	int AveY = (int)ImageProc::GetAveY_ROI(Y_buffer, width, height, CenterRect);
	SAFE_DELETE_ARRAY(Y_buffer);
	return AveY;
}
 
ImageInfo* OFccmControl::Ctrl_GetImgInfo(int CamID) 
{
	int Flag = QueryCurrentStatus(CamID);

	if (Flag == STATUS_OFFLINE)
	{	
		Cam[CamID].m_ImageInfo.imgwidth       = Cam[CamID].pImgExplorer->GetImageWidth();
		Cam[CamID].m_ImageInfo.imgheight      = Cam[CamID].pImgExplorer->GetImageHeight();
		Cam[CamID].m_ImageInfo.RGB24_buffer   = Cam[CamID].pImgExplorer->GetRGB24Buffer();
		/*Cam[CamID].m_ImageInfo.Y_buffer       = Cam[CamID].pImgExplorer->GetYBuffer();*/
		Cam[CamID].m_ImageInfo.Raw_buffer     = Cam[CamID].pImgExplorer->GetRawBuffer();
		Cam[CamID].m_ImageInfo.RawBits     = Cam[CamID].pImgExplorer->GetRawBits();
		Cam[CamID].m_ImageInfo.RawFormat   = Cam[CamID].pImgExplorer->GetRawFormat();  
		/*Cam[CamID].m_ImageInfo.Raw8_buffer  =Cam[CamID].pImgExplorer->GetRaw8Buffer();
		Cam[CamID].m_ImageInfo.NV21_buffer  =Cam[CamID].pImgExplorer->GetNv21Buffer();*/
	}
	else
	{
		EnterCriticalSection(&Cam[CamID].m_DisPlayLock);
		Cam[CamID].m_ImageInfo.imgwidth    = Cam[CamID].pConfig->GetSensorTab()->width;
		Cam[CamID].m_ImageInfo.imgheight   = Cam[CamID].pConfig->GetSensorTab()->height;
		Cam[CamID].m_ImageInfo.RGB24_buffer = Cam[CamID].pBuf.RGB24_buffer;

		//ImageProc::RGB24toY(Cam[CamID].pBuf.RGB24_buffer, Cam[CamID].pBuf.Y_buffer, Cam[CamID].m_ImageInfo.imgwidth, Cam[CamID].m_ImageInfo.imgheight);
		//ImageProc::RGB24toNV21(Cam[CamID].pBuf.RGB24_buffer,Cam[CamID].pBuf.NV21_buffer,Cam[CamID].m_ImageInfo.imgwidth, Cam[CamID].m_ImageInfo.imgheight);

		//Cam[CamID].m_ImageInfo.Y_buffer     = Cam[CamID].pBuf.Y_buffer;		
		Cam[CamID].m_ImageInfo.Raw_buffer   =Cam[CamID].pBuf.raw_buffer;

		//Cam[CamID].m_ImageInfo.Raw8_buffer   =Cam[CamID].pBuf.raw8_buffer;
		//Cam[CamID].m_ImageInfo.NV21_buffer   =Cam[CamID].pBuf.NV21_buffer;
		Cam[CamID].m_ImageInfo.RawBits   = GetRawBits(Cam[CamID].pConfig->GetSensorTab());
		Cam[CamID].m_ImageInfo.RawFormat = Cam[CamID].pConfig->GetSensorTab()->outformat; 
		LeaveCriticalSection(&Cam[CamID].m_DisPlayLock);
	}

	return &(Cam[CamID].m_ImageInfo);
}

BYTE OFccmControl::GetRawBits(SensorTab *CurrentSensor)
{
	switch(CurrentSensor->type)
	{
	case D_RAW10:
		return 10;
	case D_MIPI_RAW8: // = D_RAW8
		return 8;
	case D_MIPI_RAW10:
		return 10;
	case D_MIPI_RAW12: // = D_RAW12
		return 12;	
	case D_RAW16:
		return 16;
	case D_YUV:
		return 8;
	default:
		return 10;
	}
}

void OFccmControl::ResetBackGround(int CamID)
{

	ResetMemdcBackGround(CamID);
	Ctrl_ShowMemDC(CamID);

	 
}

int OFccmControl::OSTest(int CamID)
{
	OSTab* CurrentOStab = Cam[CamID].pConfig->GetOSTab();
	if ( !CurrentOStab->OS_en &&!CurrentOStab->PositiveOS_en&&!CurrentOStab->SensorpinOS_en) return 1;

	Ctrl_Addlog(CamID,_T("开短路测试中"));

	//分配结果保存空间
	int OSValueArray[DTPIN_MAX];
	memset((void*)OSValueArray, 0, DTPIN_MAX*sizeof(int));

	//开短路测试
	int nRet = OpenShortTest(CurrentOStab, OSValueArray,CamID);

	if (nRet != 1)
	{
		Ctrl_Addlog(CamID,_T("开短路模块异常"), COLOR_RED, 220);
		return -1;
	}

	CString sTemp; //显示结果的字符串
	Ctrl_Addlog(CamID,_T("/*************开短路测试结果****************/ "));
	int FailCount = 0;
	for (int i=0; i<45; i++)
	{
		double OSvalue = OSValueArray[i]/1000000.0;

		if (OSValueArray[i] < 3000000&&OSValueArray[i] >=0 ) //MASK为0的值，忽略  0xffff*1000
		{
			if (OSvalue > CurrentOStab->Pin[i].OpenLimit) //如果大于开路标准
			{
				sTemp.Format(_T("%d %s , 电压为:%5.3lfV    结果： Fail"), i+1, CurrentOStab->Pin[i].Name, OSvalue); 
				Ctrl_Addlog(CamID,sTemp, COLOR_RED);
				FailCount++;
			}
			else if ( (OSvalue < CurrentOStab->Pin[i].ShortLimit) && ( i < 5 || i>8) ) //如果小于短路标准
			{//5.6.7.8 为GND
				FailCount++;
				sTemp.Format(_T("%d %s , 电压为:%5.3lfV   结果  Fail"), i+1, CurrentOStab->Pin[i].Name, OSvalue);
				Ctrl_Addlog(CamID,sTemp, COLOR_RED);
			}
			else
			{
				sTemp.Format(_T("%d %s , 电压为:%5.3lfV   结果  PASS"), i+1, CurrentOStab->Pin[i].Name, OSvalue);
				Ctrl_Addlog(CamID,sTemp);
			}
		}

	}


	SaveOSResult(OSValueArray, FailCount,CamID);

	if ( FailCount > 0 ) 		
	{
		Ctrl_Addlog(CamID,_T("开短路NG"), COLOR_RED, 200);

		Ctrl_SetCamErrorInfo(CamID, _T("开短路NG"));
		return -1 ;
	}

	Ctrl_Addlog(CamID,_T("开短路OK"), COLOR_GREEN);
	Ctrl_Addlog(CamID,_T("**************开短路测试结束*******************\n"));
	return 1;
}

int  OFccmControl::OpenShortTest(OSTab *pOSTab, int* OSValueArray,int CamID)
{ 
	return pDevice->OpenShortTest(pOSTab,OSValueArray,CamID);
}

 
void OFccmControl::SaveOSResult(int * osValue, int NGCount,int CamID)
{
	CString Result   = NGCount>0 ?_T("FAIL"):_T("PASS");
	CString Headers     ;
	CString LowerLimits ;
	CString UpperLimits ;
	CString Contents    ;
	for(int i=0; i < DTPIN_MAX; i++)
	{
		CString temp;
		CString name = Cam[CamID].pConfig->GetOSTab()->Pin[i].Name;
		temp.Format(_T("%s,"),name);
		Headers += temp;

		temp.Format(_T("%.3lf,"),Cam[CamID].pConfig->GetOSTab()->Pin->ShortLimit);
		LowerLimits += temp;

		temp.Format(_T("%.3lf,"),Cam[CamID].pConfig->GetOSTab()->Pin->OpenLimit);
		UpperLimits += temp;

		if (osValue[i] != 0xffff*1000)
		{
			temp.Format(_T("%.3lf,"),osValue[i]/1000000.0);
			Contents += temp;
		}
		else
		{
			Contents += _T("null,");
		}

	}
	SaveReport(_T("OpenShort"),Headers,LowerLimits,UpperLimits,Contents,Result,CamID);

}

int OFccmControl::SaveReport(LPCTSTR lpszName, 
	LPCTSTR lpszReportHeaders, 
	LPCTSTR lpszLowerLimits, 
	LPCTSTR lpszUpperLimits,
	LPCTSTR lpszContents,
	LPCTSTR lpszResult,int CamID)
{
		//上传数据
#if 1
	vector<CString> strKey;
	vector<CString> strValue; 

	strKey.clear();
	strValue.clear();

	globalset.SeparateString(lpszReportHeaders,',',&strKey); 

	globalset.SeparateString(lpszContents,',',&strValue); 
	if(strKey.size() != strValue.size())
	{
		Ctrl_Addlog(CamID,_T("测试项数据名称与数据值不一致！"),COLOR_BLUE,200);
	}
	else
	{
		int ItemSize  = strValue.size();
		int cammeraID=0;
		if(CamID==0) cammeraID=1;
		if((Cam[CamID].usedMes)||((Ctrl_GetCamType()==1)&&(Cam[cammeraID].usedMes)))
		{ 
			int DualCamID_ = 0;
			EnterCriticalSection(&mMes_cs);
			CString result = lpszResult;
			if(ItemSize==0)
			{
				CString TestResult = lpszName;
				if(result.Find(_T("Pass"))!=-1)
				{
					Mes_AddParameter(CamID,TestResult+_T("_Result"),_T("PASS"));
				}
				else
				{
					Mes_AddParameter(CamID,TestResult+_T("_Result"),_T("NG"));
				}
				
			}
			else
			{
				for(int i=0;i<ItemSize;i++)
				{
					Mes_AddParameter(CamID,strKey[i],strValue[i]);
				}

			}	
			if(result.Find(_T("Pass"))!=-1)
			{
				Mes_AddItem(CamID,lpszName,_T("OK")) ; 
			}
			else
			{
				Mes_AddItem(CamID,lpszName,_T("NG")) ;
			}
			Mes_SetResult(CamID,lpszResult);
		
			LeaveCriticalSection(&mMes_cs);
		}
	}

	/*TestItemInfo tInfo;
	Ctrl_GetTestInfo(CamID,lpszName,tInfo);
	tInfo.vec_Data.clear();

	if(tInfo.Test_ID==-1)
		return 0;
	 
	
	for(UINT i=0;i<strValue.size();i++)
	{
		TestItemData tData;
		tData.paraName = strKey[i];
		tData.paraValue = strValue[i];
		tInfo.vec_Data.push_back(tData); 
	}
	Ctrl_SetTestInfo(CamID,tInfo.Test_ID,tInfo);*/

#endif
	if (lpszReportHeaders == _T("") || lpszContents == _T(""))
	{
		return -2;
	}

	//if(!Cam[CamID].pConfig->GetGeneralTab()->SaveReport_EnLocal) return -1; 

	CreateDirectory(Cam[CamID].pConfig->GetGeneralTab()->sReportPath, NULL);


	CString filePath = Cam[CamID].pConfig->GetGeneralTab()->sReportPath +
		_T("\\") +Cam[CamID].pConfig->pConfigFileRw->GetConfigTitle() + _T("_") + lpszName + _T(".csv");
	char sTmpReportPath[MAX_PATH];
	ImageProc::ConvertWideToChar(filePath,sTmpReportPath );
	MakeSureDirectoryPathExists(sTmpReportPath);
	CString str;
	CString comma = _T(",");

	CStdioFile file;
	//有这样的文件存在吗
	CFileFind Ft;
	

	//设定本地区域信息，转换为CHS
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	if(Ft.FindFile(filePath)) //存在
	{
		//已读写方式打开
		BOOL ret = file.Open(filePath, CFile::modeRead|CFile::modeWrite);
		if (!ret)
		{
			Ctrl_Addlog(CamID,_T("无法保存数据"));

			//还原区域设定
			setlocale( LC_CTYPE, old_locale );
			free( old_locale );
			return -2 ;
		}
	}
	else //不存在
	{
		//创建，读写方式
		BOOL ret = file.Open(filePath, CFile::modeRead|CFile::modeWrite|CFile::modeCreate);
		if (!ret)	
		{
			Ctrl_Addlog(CamID,_T("无法保存数据"));

			//还原区域设定
			setlocale( LC_CTYPE, old_locale );
			free( old_locale );
			return -2 ;
		}

		file.SeekToEnd();

		CString ID_header(_T("编号, "));
		CString time_header(_T("测试时间,"));
		/*	CString Operator_header(_T("操作者,"));
		CString MAC_header(_T("MAC,"));*/
		CString Result_header(_T("测试结果,"));

		str = ID_header +/* Operator_header +*/ time_header /*+ MAC_header*/ + Result_header + lpszReportHeaders + _T("\n");
		file.WriteString(str);
		  
		str = _T(",,,") + (CString)lpszLowerLimits + _T("\n");
		file.WriteString(str);

		str = _T(",,,") + (CString)lpszUpperLimits + _T("\n");
		file.WriteString(str);
	}

	CTime time = CTime::GetCurrentTime();

	CString ID = Ctrl_GetCamCode() +_T("_FuseID:") + Sensor_GetCamFuseID(CamID)+ _T(",");
	/*
	CString ID = L"";
	if (Cam[CamID].bState.bCameraOffLine)
	{
		ID = Ctrl_GetCamCode() +_T("_FuseID:") + Sensor_GetCamFuseID(CamID)+ _T(",");
	}
	else
	{
		Cam[CamID].fuseID = L"";
		//Cam[CamID].pImgSensor->GetFuseID(Cam[CamID].fuseID);
		if ( Cam[CamID].pImgSensor!=NULL)
		{
			Cam[CamID].fuseID = L"";
			Cam[CamID].pImgSensor->GetFuseID(Cam[CamID].fuseID);
		}
		ID = Ctrl_GetCamCode() + _T("_FuseID:") + Cam[CamID].fuseID + _T(",");
	}
	*/
	/*CString MAC = GetHostMAC() + _T(",");
	CString OperatorID = (CString)ConfigManager.GetOperatorID()  + _T(",");*/
	CString szTime = time.Format("%Y-%m-%d %H:%M:%S") + _T(",");

	file.SeekToEnd();

	str = ID  +/* OperatorID +*/ szTime  +/* MAC +*/ lpszResult + _T(",") + lpszContents + _T("\n");
	file.WriteString(str);
	
	file.Close();
	Ctrl_Addlog( CamID,(CString)lpszName + _T("测试数据已保存本机"), RGB(100,100,100));
	BOOL result = TRUE;
	 
	//还原区域设定
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );


	old_locale = NULL;
	 
	return 0;
}

int OFccmControl::Ctrl_bReOpen(int &OpenTimes)
{
	OpenTimes = this->OpenTimes;
	return 1;
}
 
void OFccmControl::Ctrl_GetCamCurrent(int CamCurrent[],int CamID)
{
	pDevice->PmuGetCurrent(CamCurrent,5,0,CamID);
}


int OFccmControl::CheckStandbyCurrent(int CamID)
{
	if (!Cam[CamID].pConfig->GetStandbyCurrentTab()->Test_en) return -1;
	//int VolSet[5] = {0,0,0,0,0};
	//pDevice->PmuSetVoltage(VolSet);
	//SetVoltage(POWER_VPP,0);
    //if(Cam[CamID].pTestCtrl->bTestResult==FALSE) return -1;
	Ctrl_Addlog(CamID,_T("******* 待机电流测试中... ********"), COLOR_BLUE);
    //ShowStandByCurrentTestingInfo(CamID);
	//Sleep(ConfigManager.GetCurrentTab()->StandByStableTime);
	int Current[5]  = {0};
	pDevice->PmuGetStandyCurrent(Current,5,0,CamID);

	//计算待机总和

	float fAVDD  = Current[0]/1000.0f;
	float fDOVDD = Current[1]/1000.0f;
	float fDVDD  = Current[2]/1000.0f;
	float fAFVDD  = Current[3]/1000.0f;

	float UP_Avdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[0].CurrentUpLimit;
	float UP_Dovdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[1].CurrentUpLimit;
	float UP_Dvdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[2].CurrentUpLimit;
	float UP_Afvdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[3].CurrentUpLimit;

	float Low_Avdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[0].CurrentLowLimit;
	float Low_Dovdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[1].CurrentLowLimit;
	float Low_Dvdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[2].CurrentLowLimit;
	float Low_Afvdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[3].CurrentLowLimit;

	float Total = 0;
	int FailCnt = 0;
	CString temp;

	//Add for Standby Power Test
	SensorTab *pTempSensorTab = Cam[CamID].pConfig->GetSensorTab();
	Total = (fAVDD * pTempSensorTab->avdd + fDOVDD * pTempSensorTab->dovdd + fDVDD * pTempSensorTab->dvdd) / 10.0;
	float Total_Low  = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->dbStandByPowerMin;
	float Total_High = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->dbStandByPowerMax;
	if ((Total < Total_Low) || (Total > Total_High))
	{
		temp.Format(L"待机功耗超标：%.1lf[%.1lf, %.1lf]uW", Total, Total_Low, Total_High);
		Ctrl_Addlog(CamID, temp, COLOR_RED, 200);
		FailCnt++;
	}

	//显示结果 AVDD
	if (fAVDD > UP_Avdd )
	{
		temp.Format(_T("AVDD 待机电流 %.1lf uA, 超标"), fAVDD);
		Ctrl_Addlog(CamID,temp, COLOR_RED, 220);
		FailCnt++;
	}

	//显示结果 DOVDD
	if (fDOVDD > UP_Dovdd)
	{
		temp.Format(_T("DOVDD 待机电流 %.1lf uA, 超标"), fDOVDD);
		Ctrl_Addlog(CamID,temp,COLOR_RED, 220);
		FailCnt++;
	}

	//显示结果 DVDD
	if (fDVDD > UP_Dvdd)
	{
		temp.Format(_T("DVDD 待机电流 %.1lf uA, 超标"), fDVDD);
		Ctrl_Addlog(CamID,temp,COLOR_RED, 220);
		FailCnt++;
	} 
	if (fAFVDD > UP_Afvdd)
	{
		temp.Format(_T("AFVDD 待机电流 %.1lf uA, 超标"), fAFVDD);
		Ctrl_Addlog(CamID,temp,COLOR_RED, 220);
		FailCnt++;
	} 

	ShowStandByCurrentTestingInfo(CamID);
	CString szResult;
	if (FailCnt>0)
	{
		szResult = _T("FAIL");
		if (!IsWindowVisible(Cam[CamID].hLogger)) //全屏状态下 LOGGER不可见
		{
			//AfxMessageBox(_T("待机测试电流NG")); //采用其他提示
			Ctrl_Addlog(CamID,_T("待机电流测试NG"),COLOR_RED);
		}
	}
	else
	{
		Ctrl_Addlog(CamID,_T("待机电流测试OK"),COLOR_BLUE);
		szResult = _T("Pass");

	}

	temp.Format(_T("AVDD %.1lf uA  DOVDD %.1lf uA  DVDD %.1lf uA AFVDD %.1lf uA Power %.1lfuW"),fAVDD,fDOVDD,fDVDD,fAFVDD, Total);
	Ctrl_Addlog(CamID,temp);


	//记录到CSV文件
	CString Headers  = _T("Power,<AFVCC,AVDD,DOVDD,DVDD");
	CString LowerLimits ;
	CString UpperLimits ;

	LowerLimits.Format(_T("%f,%f,%f,%f,%f"), Total_Low, Low_Afvdd, Low_Avdd, Low_Dovdd, Low_Dvdd);
	UpperLimits.Format(_T("%f,%f,%f,%f,%f"), Total_High, UP_Afvdd, UP_Avdd, UP_Dovdd, UP_Dvdd);
	CString Contents;
	Contents.Format(_T("%f,%f,%f,%f,%f>"), Total, fAFVDD, fAVDD, fDOVDD, fDVDD);
	if (0 == CamID)
	{
		SaveReport(_T("StandbyCurrent_Left^"),Headers,LowerLimits,UpperLimits,Contents,szResult,CamID);
	}
	else if(1 == CamID)
	{
		SaveReport(_T("StandbyCurrent_Right^"),Headers,LowerLimits,UpperLimits,Contents,szResult,CamID);
	}

//	SaveReport(_T("StandyCurrent^"),Headers,LowerLimits,UpperLimits,Contents,szResult,CamID);
	if (FailCnt>0)
	{
		Ctrl_SetCamErrorInfo(CamID, _T("待机电流测试NG"));
		return 0;
	}

	return 1;
}

/*
int OFccmControl::CheckWorkingCurrent(int* Current, int* Voltage,int CamID)
{
	if (!Cam[CamID].pConfig->GetWorkingCurrentTab()->Test_en) return -1;


	for (int i=0; i<5; i++)//获取工作中最大的电流
	{
		if((Current[i]/1000000.0f) >Cam[CamID].fWorkingCurrent[i] )
		{
			Cam[CamID].fWorkingCurrent[i] = Current[i]/1000000.0f;
		}
	}

	int FailedCount = 0;
	CString temp;

	//显示结果 AVDD
	if (Cam[CamID].fWorkingCurrent[0] > Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[0].CurrentUpLimit)
	{
		temp.Format(_T("AVDD 工作电流 %.1lf mA, NG"), Cam[CamID].fWorkingCurrent[0]);
		Ctrl_Addlog(CamID,temp, COLOR_RED, 220);
		FailedCount++;
	}


	//显示结果 DOVDD
	if (Cam[CamID].fWorkingCurrent[1] > Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[1].CurrentUpLimit)
	{
		temp.Format(_T("DOVDD 工作电流 %.1lf mA, NG"), Cam[CamID].fWorkingCurrent[1]);
		Ctrl_Addlog(CamID,temp,COLOR_RED, 220);
		FailedCount++;
	}


	//显示结果 DVDD
	if (Cam[CamID].fWorkingCurrent[2] > Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[2].CurrentUpLimit)
	{
		temp.Format(_T("DVDD 工作电流 %.1lf mA, NG"), Cam[CamID].fWorkingCurrent[2]);
		Ctrl_Addlog(CamID,temp,COLOR_RED, 220);
		FailedCount++;
	}

	if (Cam[CamID].fWorkingCurrent[3] > Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[3].CurrentUpLimit)
	{
		temp.Format(_T("AFVDD 工作电流 %.1lf mA, NG"), Cam[CamID].fWorkingCurrent[3]);
		Ctrl_Addlog(CamID,temp,COLOR_RED, 220);
		FailedCount++;
	}
	  
	if (FailedCount > 0)
	{    
		Cam[CamID].bWorkingCurrentFailed = TRUE;
		CloseCamera(CamID,0);
		//Stop();
	}

	return 0;
}
*/
void OFccmControl::ShowWorkingCurrentAndVoltage(CString fpsInfo,int CamID)
{ 
	int Current[5] = {0};
	pDevice->PmuGetCurrent(Current,5,0,CamID);
	int Voltage[5] = {0};
	pDevice->PmuGetVoltage(Voltage,5,0,CamID);
	CString sInfo;
	sInfo.Format(_T("AVDD:%.0lf(%.1lf)DOVDD:%.0lf(%.1lf)DVDD:%.0lf(%.1lf)AFVDD:%.0lf(%.1lf)VPP:%.0lf(%.1lf)"), 
		Current[0]/1000000.0f, Voltage[0]/1000.0f,
		Current[1]/1000000.0f, Voltage[1]/1000.0f,
		Current[2]/1000000.0f, Voltage[2]/1000.0f,
		Current[3]/1000000.0f, Voltage[3]/1000.0f,
		Current[4]/1000000.0f, Voltage[4]/1000.0f);
	sInfo = sInfo + fpsInfo;
	ShowTextInfo(sInfo,CamID);

//	CheckWorkingCurrent(Current, Voltage,CamID);
}

void OFccmControl::ShowStandByCurrentAndVoltage(int CamID)
{
	int Current[5] = {0};
	pDevice->PmuGetCurrent(Current,5,0,CamID);
	int Voltage[5] = {0};
	pDevice->PmuGetVoltage(Voltage,5,0,CamID);
	CString sInfo;
	sInfo.Format(_T("AVDD: %.0lf(%.1lf) DOVDD: %.0lf(%.1lf) DVDD: %.0lf(%.1lf) AFVDD: %.0lf(%.1lf) VPP: %.0lf(%.1lf)"), 
		Current[0]/1000.0f, Voltage[0]/1000.0f,
		Current[1]/1000.0f, Voltage[1]/1000.0f,
		Current[2]/1000.0f, Voltage[2]/1000.0f,
		Current[3]/1000.0f, Voltage[3]/1000.0f,
		Current[4]/1000.0f, Voltage[4]/1000.0f);
	ShowTextInfo(sInfo,CamID);
}

void OFccmControl::ShowStandByCurrentTestingInfo(int CamID)
{
	CDC* pDC = Cam[CamID].DisplayMemryDC;
	CRect rect;
	::GetClientRect(Cam[CamID].hDisplayWnd,&rect);
	CFont ft;
	ft.CreatePointFont(350,_T("黑体"));
	pDC->SelectObject(&ft);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,50,50));
	pDC->TextOutW(int(rect.Width()*0.1), int (rect.Height()*0.45),_T("待机电流测试中，请勿取下模组"));
	Ctrl_ShowMemDC(CamID);
	pDC = NULL;
}



void OFccmControl::ShowStaticCurrentAndVoltage(int CamID)
{
	int Current[5] = {0};
	pDevice->PmuGetCurrent(Current,5,0,Cam[CamID].channel);
	int Voltage[5] = {0};
	pDevice->PmuGetVoltage(Voltage,5,0,Cam[CamID].channel);
	CString sInfo;
	sInfo.Format(_T("AVDD:%.0lf(%.1lf) DOVDD:%.0lf(%.1lf) DVDD:%.0lf(%.1lf) AFVDD:%.0lf(%.1lf) VPP:%.0lf(%.1lf)"), 
		Current[0]/1000.0f, Voltage[0]/1000.0f,
		Current[1]/1000.0f, Voltage[1]/1000.0f,
		Current[2]/1000.0f, Voltage[2]/1000.0f,
		Current[3]/1000.0f, Voltage[3]/1000.0f,
		Current[4]/1000.0f, Voltage[4]/1000.0f);
	ShowTextInfo(sInfo,CamID);
}

void OFccmControl::ShowStaticCurrentTestingInfo(int CamID)
{
	CDC* pDC = Cam[CamID].DisplayMemryDC;
	CRect rect;
	::GetClientRect(Cam[CamID].hDisplayWnd,&rect);
	CFont ft;
	ft.CreatePointFont(100,_T("黑体"));
	pDC->SelectObject(&ft);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,50,50));
	pDC->TextOutW(int(rect.Width()*0.1), int (rect.Height()*0.45),_T("静态电流测试中，请勿取下模组"));
	Ctrl_ShowMemDC(CamID);
	pDC = NULL;
}

void OFccmControl::ShowTextInfo(LPCTSTR str,int CamID)
{
	static CString szTemp[512];
	static int index = 0;
	szTemp[index] = str;
	LPCTSTR lpstr = LPCTSTR(szTemp[index]);
	PostMessage(hTestDlgWnd, WM_SHOWTEXTINFO, (WPARAM)lpstr, (LPARAM)CamID);
	index++;
	if (index >= 256) index = 0;
}

 

int OFccmControl::QueryCurrentStatus(int CamID)
{
	if (isOffline(CamID))
	{
		return STATUS_OFFLINE;
	}

	if (isStandby(CamID))
	{
		return STATUS_STANDBY;
	}
	else
	{
		if (isRunning(CamID))
		{
			if(isPause(CamID))
				return STATUS_PAUSE;
			else
				return STATUS_RUNNING;
		}
		else
		{
			return STATUS_OPENNING;
		}
	}
	return 0;
}
/*
void OFccmControl::SaveWorkingCurrent(int CamID)
{
	//记录到CSV文件
	if (!Cam[CamID].pConfig->GetWorkingCurrentTab()->Test_en) return;

	CString  Result;
	if (Cam[CamID].bWorkingCurrentFailed)
	{    
		Result = _T("FAIL");		 
	}
	else
	{
		Result = _T("PASS");		 
		Ctrl_Addlog(CamID,_T("工作电流测试OK"),RGB(0,100,0));
	}

	CString Headers    = _T("AVDD,DOVDD,DVDD,AFVDD,VPP");
	CString LowerLimits ;
	CString UpperLimits ;
	LowerLimits.Format(_T("%f,%f,%f,%f,%f"),Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[0].CurrentLowLimit,
		Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[1].CurrentLowLimit,
		Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[2].CurrentLowLimit,
		Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[3].CurrentLowLimit,
		Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[4].CurrentLowLimit);

	UpperLimits.Format(_T("%f,%f,%f,%f,%f"),Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[0].CurrentUpLimit,
		Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[1].CurrentUpLimit,
		Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[2].CurrentUpLimit,
		Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[3].CurrentUpLimit,
		Cam[CamID].pConfig->GetWorkingCurrentTab()->Pin[4].CurrentUpLimit);
	CString Contents;
	Contents.Format(_T("%f,%f,%f,%f,%f"), Cam[CamID].fWorkingCurrent[0], Cam[CamID].fWorkingCurrent[1], Cam[CamID].fWorkingCurrent[2], Cam[CamID].fWorkingCurrent[3], Cam[CamID].fWorkingCurrent[4]);

	SaveReport(_T("WorkingCurrent"),Headers,LowerLimits,UpperLimits,Contents,Result,CamID);

	Contents.Format(_T("AVDD %.1lf mA \nDOVDD %.1lf mA \nDVDD %.1lf mA \nAFVDD %.1lf mA \nVPP %.1lf mA"), Cam[CamID].fWorkingCurrent[0], Cam[CamID].fWorkingCurrent[1], Cam[CamID].fWorkingCurrent[2], Cam[CamID].fWorkingCurrent[3], Cam[CamID].fWorkingCurrent[4]);
	Ctrl_Addlog(CamID,Contents);
}
*/
void OFccmControl::UpdateUI(int type,int CamID)
{
//	PostMessage(hTestDlgWnd, WM_UPDATEUI, type, CamID);
	SendMessage(hTestDlgWnd, WM_UPDATEUI, type, CamID);
}
 
HWND OFccmControl::Ctrl_GetMainWndHwnd()
{
	return hTestDlgWnd;
}

int OFccmControl::Offline_Process_thread(int CamID)
{
	//图像显示循环
	BOOL bTest = FALSE;

	Ctrl_PauseGrabFrame(CamID);

	Cam[CamID].bState.bCameraOffLine = TRUE;
	Cam[CamID].bState.bCameraStarted = FALSE;
	Cam[CamID].bState.bCameraStandby = TRUE;

	UpdateUI(UI_ENTER_OFFLINE,CamID);

	Cam[CamID].bState.bOffline_LoopFlag = TRUE;
	 
	Cam[CamID].pTestCtrl->TestInit();

	while(Cam[CamID].bState.bOffline_LoopFlag)
	{
		Cam[CamID].bState.bCameraOffLine = TRUE;
		if (globalset.OffLineTest)
		{

			if (isStandby(CamID))
			{  
				//Cam[CamID].bState.bCameraStandby = TRUE;
				//加载图片
				if(!Cam[CamID].pImgExplorer->Open())
				{				
					Sleep(200);
					break;
					//continue;				
				}
				SetCamCode(Cam[CamID].pImgExplorer->GetImgName());
				Cam[CamID].bState.bCameraStandby = FALSE;
				Cam[CamID].pTestCtrl->TestInit();

			}
			Cam[CamID].pTestCtrl->Run();
		}
		else
		{
			if (isStandby(CamID))
			{  
				Cam[CamID].bState.bCameraStandby = FALSE;
				//加载图片
				if(!Cam[CamID].pImgExplorer->Open())
				{
					ExitOfflineMode(CamID);
					break;
					//return FALSE;
				}
				SetCamCode(Cam[CamID].pImgExplorer->GetImgName());
			} 
			Cam[CamID].pTestCtrl->Run();
		}	 
		 


		Sleep(50);
	}

	SetEvent(Cam[CamID].hThreadCtrl.m_hOfflineThreadDead);
	return 0;
}

void OFccmControl::StartOfflineProcessThread(int CamID)
{ 
	Cam[CamID].hThreadCtrl.m_hOfflineThreadDead  = CreateEvent(NULL, TRUE, FALSE, NULL);  //抓取线程结束事件
	switch(CamID)
	{
	case 0:
		AfxBeginThread(Offline_Process_thread0,this);
		break;

	case 1:
		AfxBeginThread(Offline_Process_thread1,this);
		break;
	}

}

void OFccmControl::StopOfflineProcessThread(int CamID)
{
	//等待抓取线程结束事件
	Cam[CamID].bState.bOffline_LoopFlag = FALSE;
	if( Cam[CamID].hThreadCtrl.m_hOfflineThreadDead != NULL)
	{
		WaitForSingleObject(Cam[CamID].hThreadCtrl.m_hOfflineThreadDead, INFINITE); 
		CloseHandle(Cam[CamID].hThreadCtrl.m_hOfflineThreadDead);
		Cam[CamID].hThreadCtrl.m_hOfflineThreadDead = NULL;
	}

}

UINT OFccmControl::Offline_Process_thread0(LPVOID pParam)
{
	OFccmControl* pCtrl = static_cast<OFccmControl*> (pParam);
	pCtrl->Offline_Process_thread(0);
	pCtrl = NULL;
	return 0;
}
UINT OFccmControl::Offline_Process_thread1(LPVOID pParam)
{
	OFccmControl* pCtrl = static_cast<OFccmControl*> (pParam);
	pCtrl->Offline_Process_thread(1);
	pCtrl = NULL;
	return 0;
}

BOOL OFccmControl::isOffline(int CamID)
{
	return Cam[CamID].bState.bCameraOffLine;
}

BOOL OFccmControl::EnterOfflineMode(int CamID,BOOL bEnterOffLine)
{
	StartOfflineProcessThread(CamID);
	return TRUE;
}

void OFccmControl::ExitOfflineMode(int CamID)
{
	Cam[CamID].bState.bCameraOffLine = FALSE;
	Cam[CamID].bState.bCameraStarted = FALSE;
	Cam[CamID].bState.bCameraStandby = TRUE;

	StopOfflineProcessThread(CamID);
	Cam[CamID].pImgExplorer->Free();
	Cam[CamID].pTestCtrl->ResetALLButtonImage();//重新刷新测试按钮图标
	Cam[CamID].pTestCtrl->TestInit();
	UpdateUI(UI_EXIT_OFFLINE,CamID);
	CString str;
	str.Format(_T("Came%d已退出离线模式"),CamID);
	Ctrl_Addlog(CamID,str);
}


BOOL OFccmControl::isStandby(int CamID)
{
	return Cam[CamID].bState.bCameraStandby;
}

BOOL OFccmControl::isRunning(int CamID)
{
	return Cam[CamID].bState.bCameraStarted;
}

BOOL OFccmControl::isPause(int CamID)
{
	int ret = WaitForSingleObject(Cam[CamID].hThreadCtrl.m_hGrabPause,0);
	if(ret == WAIT_TIMEOUT)
		return TRUE;
	else
		return FALSE;
}

CRect OFccmControl::Ctrl_CalcRect_DisplayScreen(double cx, double cy, double x_length, double y_length,int CamID)
{
	CRect rect;
	Ctrl_GetDisplayWndRect(&rect,CamID);

	int width = rect.Width();
	int height = rect.Height();

	double top = (cy - 1.0*y_length/2)  * height/100;
	if (top<0) top = 0;
	double bottom = (cy + 1.0*y_length/2)  * height/100;
	if (bottom >= height) bottom = height - 1;

	double left = (cx - 1.0*x_length/2)  * width/100;
	if (left<0) left = 0;
	double right = (cx + 1.0*x_length/2)  * width/100;
	if (right >= width) right = width - 1; 

	return CRect((LONG)left,(LONG)top,(LONG)right,(LONG)bottom);
}

void OFccmControl::UpdateInspectionItemDlg(void)
{
	::PostMessage(hTestDlgWnd, WM_CREATESETTINGDLG, 0, 0);
}
void OFccmControl::SendMsgMachine(int CamID)
{
	CString str;

	
	//add by wzx，自动校验时
	CString strTemp;
	CString strLog;
	if (bCheckLight == TRUE)
	{
		//未点亮时，加入判断
		if (Cam[CamID].NGTestItemID == -2 || Cam[CamID].NGTestItemID== -1)
		{
			//1 strErrorInfo.Format(_T("Light On Fail!"));,//2.Standby Current Fail
			strLog = _T("Light On Fail, Send T0102!");
			pCcmCtrl->Ctrl_Addlog( 0,strLog,COLOR_BLUE,220);
			strTemp = L"T0102";
			Cam[CamID].camSocket->SendInfo(strTemp);
			return ;
		}
	
		int nsel = Config_ReadConfigInt(_T("WBSetting"),_T("AUTOLIGHTSTATUS"),0,0);
		strLog.Format(L"AutoLightSel:%d", nsel);
		pCcmCtrl->Ctrl_Addlog( 0,strLog,COLOR_BLUE,220);
		if (nsel == AUTOLIGHT_GOLDEN_SUCESS || nsel == AUTOLIGHT_SAVECOEFFEE_SUCESS)
		{
			strTemp = L"T0100";
			Cam[CamID].camSocket->SendInfo(strTemp);			
		}
		else if (nsel == AUTOLIGHT_GOLDEN_FAIL)
		{
			strTemp = L"T0102";
			Cam[CamID].camSocket->SendInfo(strTemp);
		}
		strLog = L"Send:";
		strLog += strTemp;
		pCcmCtrl->Ctrl_Addlog( 0,strLog,COLOR_BLUE,220);
		
		return ;
	}

	//DebugInfo("Mes Fail! 发送结果T01001");//打板
	if (Cam[CamID].pTestCtrl->bTestResult && (pCcmCtrl->Cam[CamID].TestItemNum == pCcmCtrl->Cam[CamID].TestItemPass))
	{
		if (Cam[CamID].camSocket->isStartTest == true)
		{
			str = _T("TestEnd_Pass");
		}
		else str = _T("T01000");//测试结果OK，
	}
	else
	{
		str.Format(_T("NG ID=%d"),Cam[CamID].NGTestItemID);
		Ctrl_Addlog(CamID,str,COLOR_RED,200);
		if (Cam[CamID].camSocket->isStartTest == true)
		{
			CString strErrorInfo = pCcmCtrl->Ctrl_GetCamErrorInfo(CamID);

			if (strErrorInfo.Find(_T("PASS")) != -1 ||
				strErrorInfo.Find(_T("OK")) != -1 ||
				strErrorInfo == _T(""))
			{
				strErrorInfo = _T("TEST FAIL!");
			}
// 			else if (IncludeChinese(strErrorInfo))
// 			{
// 				strErrorInfo = _T("TEST FAIL!");
// 			}
// 			else
// 			{
// 				;
// 			}

			if (strErrorInfo == "TEST FAIL!")
			{
				if (Cam[CamID].NGTestItemID == -2)
				{
					strErrorInfo.Format(_T("Light On Fail!"));
				}
				else if (Cam[CamID].NGTestItemID == -1)
				{
					strErrorInfo.Format(_T("Standby Current Fail!"));
				}
				else if (Cam[CamID].NGTestItemID > pCcmCtrl->Cam[CamID].TestItemNum-1 ||
					     Cam[CamID].NGTestItemID < 0)
				{
					strErrorInfo.Format(_T("Other Fail!"));
				}
				else
				{
					MyTestItem *item = Cam[CamID].pTestCtrl->GetInspectionItem_FromID(Cam[CamID].NGTestItemID);
					if (item)
					{
						strErrorInfo.Format(L"[%s]\nTEST_FAIL!",item->pItem->GetName());
					}
				}
			}

			str.Format(_T("TestEnd_Fail:%s"),strErrorInfo);
		}
		else
		{
			str = _T("T01001");//测试结果NG，
			if (Cam[CamID].pConfig->GetErrorCodeTab()->ErrorCode_en)
			{
				CString strTemp;
				strTemp.Format(_T("%d"),Cam[CamID].NGTestItemID);

				for (int i=0;i<15;i++)
				{
					if (Cam[CamID].NGTestItemID==-2)
					{
						str = _T("T0100110");//测试结果NG，
						break;
					}
					if (Cam[CamID].NGTestItemID==-1)
					{
						str = _T("T0100143");//测试结果NG，
						break;
					}
					if(Cam[CamID].pConfig->GetErrorCodeTab()->strTestID[i]==_T("")) break;
					vector <CString> MyTestID;
					ImageProc::SeparateString(Cam[CamID].pConfig->GetErrorCodeTab()->strTestID[i],_T(','),&MyTestID);
					for (int j=0;j<MyTestID.size();j++)
					{
						if (strTemp==MyTestID[j])
						{
							str=Cam[CamID].pConfig->GetErrorCodeTab()->strSocket[i];
							break;
						}
					}
				}
			}
		}
	}

	Cam[CamID].camSocket->SendInfo(str);
}


void OFccmControl::DualAutoTestSendMsgMachine(int CamID)
{
	CString str;

	int iReplyCamID = 0;//只使用CamID=0回复

	//DebugInfo("Mes Fail! 发送结果T01001");//打板
	if (Cam[0].pTestCtrl->bTestResult && Cam[0].TestItemPass == Cam[0].TestItemNum
		&& Cam[1].pTestCtrl->bTestResult &&  Cam[1].TestItemPass == Cam[1].TestItemNum)
	{
		if (Cam[iReplyCamID].camSocket->isStartTest == true)
		{
			str = _T("TestEnd_Pass");
		}
		else str = _T("T01000");//测试结果OK，
	}
	else
	{
		

		if (Cam[iReplyCamID].camSocket->isStartTest == true)
		{
			str = _T("TestEnd_Fail:Other Fail!"); //设个默认值,防呆
			for(int iCamIDIndex=0;iCamIDIndex<2;iCamIDIndex++)
			{
				CString strLog;
				strLog.Format(_T("CamID:%d,NGItem ID=%d"),iCamIDIndex, Cam[iCamIDIndex].NGTestItemID);
				Ctrl_Addlog(CamID,strLog,COLOR_BLUE,200);

				CamTab &stCamIndex = Cam[iCamIDIndex];
				if (stCamIndex.pTestCtrl->bTestResult)
				{
					continue;
				}
				CString strErrorInfo = pCcmCtrl->Ctrl_GetCamErrorInfo(iCamIDIndex);

				if (strErrorInfo.Find(_T("PASS")) != -1 ||
					strErrorInfo.Find(_T("OK")) != -1 ||
					strErrorInfo == _T(""))
				{
					strErrorInfo = _T("TEST FAIL!");
				}
// 				else if (IncludeChinese(strErrorInfo))
// 				{
// 					strErrorInfo = _T("TEST FAIL!");
// 				}
// 				else
// 				{
// 					;
// 				}

				if (strErrorInfo == "TEST FAIL!")
				{
					if (stCamIndex.NGTestItemID == -2)
					{
						strErrorInfo.Format(_T("Light On Fail!"));
					}
					else if (stCamIndex.NGTestItemID == -1)
					{
						strErrorInfo.Format(_T("Standby Current Fail!"));
					}
					else if (stCamIndex.NGTestItemID > stCamIndex.TestItemNum-1 ||
						stCamIndex.NGTestItemID < 0)
					{
						strErrorInfo.Format(_T("Other Fail!"));
					}
					else
					{
						MyTestItem *item = stCamIndex.pTestCtrl->GetInspectionItem_FromID(stCamIndex.NGTestItemID);
						if (item)
						{
							strErrorInfo.Format(L"[%s]\nTEST_FAIL!",item->pItem->GetName());
						}
					}
				}

				str.Format(_T("TestEnd_Fail:%s"),strErrorInfo);
				break;
			}
		}
		else
		{

			str = _T("T01001");//测试结果NG，
			CString strLog;
			CString strErrorCode;
			for (int iCamIDIndex = 0;iCamIDIndex<2;iCamIDIndex++)
			{
				strLog.Format(_T("CamID:%d,NGItem ID=%d"),iCamIDIndex, Cam[iCamIDIndex].NGTestItemID);
				Ctrl_Addlog(iCamIDIndex,strLog,COLOR_BLUE,200);
			 
				if (Cam[iCamIDIndex].pTestCtrl->bTestResult)
				{
					continue;
				}
				if (Cam[iCamIDIndex].pConfig->GetErrorCodeTab()->ErrorCode_en)
				{
					CString strTemp;
					strTemp.Format(_T("%d"),Cam[iCamIDIndex].NGTestItemID);
					for (int i=0;i<15;i++)
					{
						if (Cam[iCamIDIndex].NGTestItemID==-2)
						{
							strErrorCode = _T("10");
							
							break;
						}
						if (Cam[iCamIDIndex].NGTestItemID==-1)
						{
							strErrorCode = _T("43");
							break;
						}
						if(Cam[iCamIDIndex].pConfig->GetErrorCodeTab()->strTestID[i]==_T("")) break;;
						vector <CString> MyTestID;
						ImageProc::SeparateString(Cam[iCamIDIndex].pConfig->GetErrorCodeTab()->strTestID[i],_T(','),&MyTestID);
						for (int j=0;j<MyTestID.size();j++)
						{
							if (strTemp==MyTestID[j])
							{
								strErrorCode=Cam[iCamIDIndex].pConfig->GetErrorCodeTab()->strSocket[i];
								break;
							}
						}
					}
				}
			}
			strErrorCode.TrimLeft();
			strErrorCode.TrimRight();
			if (strErrorCode.GetLength()>0)
			{
				str += strErrorCode;
			}
			else
			{
				str += _T("99");//没有错误码，就加个99作为默认的错误码吧
			}
		}

	}
	if (CamID == iReplyCamID)
	{
		Cam[iReplyCamID].camSocket->SendInfo(str);
	}
}
void OFccmControl::InitSocket(int CamID,BOOL bCreateSrv)
{ 
	if (Cam[CamID].camSocket != NULL)
	{
         Cam[CamID].camSocket->Close();
		 SAFE_DELETE(Cam[CamID].camSocket);
	}
	Cam[CamID].camSocket = new ClientMFCSocket();
	Cam[CamID].camSocket->SocketInit(CamID);
	int Port= Cam[CamID].pConfig->GetGeneralTab()->iPort;
	CString RemoteIP = Cam[CamID].pConfig->GetGeneralTab()->RemoteIP;
	int Flag=Cam[CamID].camSocket->Create();
	CString str;
	str.Format(_T("Port = %d"),Port);	
	Ctrl_Addlog(CamID,RemoteIP,COLOR_GREEN,200);
	Ctrl_Addlog(CamID,str,COLOR_GREEN,200);
	if (Flag==1)
	{
		Ctrl_Addlog(CamID,_T("Socket创建成功"),COLOR_GREEN,200);
	}
	else
	{
		Ctrl_Addlog(CamID,_T("Socket创建失败"),COLOR_RED,200);
	}

	if(Cam[CamID].camSocket->Connect(RemoteIP,Port))
	{
		Ctrl_Addlog(CamID,_T("Socket成功连接!"),COLOR_GREEN,200);
	}
	else
	{		
		CString str;
		str.Format(_T("Port = %d"),Port);
		Ctrl_Addlog(CamID,RemoteIP,COLOR_RED,200);
		Ctrl_Addlog(CamID,str,COLOR_RED,200);
		Ctrl_Addlog(CamID,_T("Socket连接失败!"),COLOR_RED,200);
	}
	Cam[CamID].camSocket->p_Main= CWnd::FromHandle(hTestDlgWnd);
	Cam[CamID].pTestCtrl->p_Main = CWnd::FromHandle(hTestDlgWnd);

	if (bCreateSrv)
	{
		InitCamServerSocket(CamID);
	}
	

}

void OFccmControl::InitCamServerSocket(int CamID)
{
	if(CamID!=1) return;
	BOOL bFlag = FALSE;


	Cam[CamID].pServerSocket = new ServerMFCSocket(1,8000,CWnd::FromHandle(hTestDlgWnd));//8001
	bFlag = Cam[CamID].pServerSocket->Create(8000);

	if(!bFlag) 
	{
		Ctrl_Addlog(CamID,_T("ServerSocket创建失败"),COLOR_RED,200);
		SAFE_DELETE(Cam[CamID].pServerSocket);
		return;
	}
	else
	{
		Ctrl_Addlog(CamID,_T("ServerSocket创建成功"),COLOR_GREEN,200);
	}
	bFlag = Cam[CamID].pServerSocket->Listen();

	if(!bFlag) 
	{
		Ctrl_Addlog(CamID,_T("ServerSocket监听失败"),COLOR_RED,200);
		SAFE_DELETE(Cam[CamID].pServerSocket);
		return;
	}
	else
	{
		Ctrl_Addlog(CamID,_T("ServerSocket监听中..."),COLOR_GREEN,200);
	}
	 
}


CString  OFccmControl::Ctrl_GetCurrentLocalTime()
{
	CString sfilename; 

	CTime time = CTime::GetCurrentTime();
	sfilename.Format(_T("%02d%02d%02d-%02d%02d%02d"),
		time.GetYear(),
		time.GetMonth(), 
		time.GetDay(), 
		time.GetHour(), 
		time.GetMinute(), 
		time.GetSecond());

	return sfilename;
}
void OFccmControl::Ctrl_SaveImage(int CamID,BOOL bSetPath ,CString PathName,CString FileName,int ImgType,BYTE* imgBuf)
{ 
	CString sfilename = Ctrl_GetCurrentLocalTime(); 
	if (CamID > 100)
	{
	    return;
	}
	int ID = Cam[CamID].pTestCtrl->GetLastID();
	MyTestItem* pLastItem = Cam[CamID].pTestCtrl->GetInspectionItem_FromID(ID);

	if (pLastItem != NULL)
		sfilename = (CString)pLastItem->pItem->GetName() + _T("_") + sfilename;

	CFileDialog dlg(FALSE, NULL, sfilename);

	//dlg.m_ofn.lpstrFilter=_T("JPEG(*.jpg)\0*.jpg\0BMP(*.bmp)\0*.bmp\0SingleChannelRaw(*.bmp)\0*.bmp\0Raw(*.raw)\0*.raw\0AssistWnd(*.jpg)\0*.jpg\0DisplayWnd(*.jpg)\0*.jpg\0bufjpg(*.jpg)\0*.jpg\0Raw8(*.raw)\0*.raw\0\0");
	dlg.m_ofn.lpstrFilter=_T("JPEG(*.jpg)\0*.jpg\0BMP(*.bmp)\0*.bmp\0Raw(*.raw)\0*.raw\0Raw8(*.raw)\0*.raw\0\0");

	if (!bSetPath)
	{
		if (IDOK==dlg.DoModal())
		{
			PathName = dlg.GetPathName();
			ImgType = dlg.m_ofn.nFilterIndex;
		}
		else
		{
			pLastItem = NULL;
			return ;
		}
	}
	else
	{
		PathName = PathName + FileName;
	} 
	switch(ImgType)
	{
	case 1: //保存JPG格式图片
		{
			PathName=PathName+_T(".")+_T("jpg");
			SaveFullRGB(PathName,CamID);
			break;
		}
	case 2://保存BMP格式图片
		{
			PathName=PathName+_T(".")+_T("bmp");
			SaveFullRGB(PathName,CamID);
			break;
		}
	case 3://保存raw二进制流格式图片
		{	
			if (!bSetPath)
				SaveRaw(PathName,CamID);
			else
				SaveRaw(PathName,CamID,1);
			break;
		}
	case 4://保存RAW8的图片
		{	
			if (!bSetPath)
				SaveRaw8(PathName,CamID);
			else
				SaveRaw8(PathName,CamID,1);
			break;
		}	 
	case 5://保存指定buf的图片
		{		
			PathName=PathName+_T(".")+_T("jpg");
			SaveImgbuftoImage(PathName,CamID,imgBuf);
			break;
		}

	}

	pLastItem = NULL;
}
void OFccmControl::SaveImgbuftoImage(CString PathName,int CamID,BYTE* imgBuf)
{
	//创建图像
	CImage Image;
	int width = Ctrl_GetImgInfo(CamID)->imgwidth;
	int height = Ctrl_GetImgInfo(CamID)->imgheight;
	Image.Create(width, height, 24);
	//取得图像首地址指针
	BYTE* Image_Data = (BYTE*)Image.GetPixelAddress(0, height-1);

	//填充数据
	BYTE* pSrc = NULL;

	pSrc = imgBuf; 

	for(int i=0;i<height;i++)
	{
		int y=height-1-i;
		for(int j=0;j<width;j++)
		{
			Image_Data[(y*width+j)*3]   = pSrc[(i*width+j)*3];
			Image_Data[(y*width+j)*3+1] = pSrc[(i*width+j)*3+1];
			Image_Data[(y*width+j)*3+2] = pSrc[(i*width+j)*3+2];
		}
	}
	//存储
	Image.Save(PathName);
	CString str;
	str.Format(_T("已保存到：%s"),PathName);
	Ctrl_Addlog(CamID,str);
	pSrc = NULL;
	Image_Data = NULL;
}
void OFccmControl::SaveFullRGB(CString PathName,int CamID)
{
	//创建图像
	CImage Image;

	ImageInfo img = * Ctrl_GetImgInfo(CamID);

	EnterCriticalSection(&Cam[CamID].m_DisPlayLock);
	int width     = img.imgwidth;
	int height    = img.imgheight; 

	Image.Create(width, height, 24);
	//取得图像首地址指针
	BYTE* Image_Data = (BYTE*)Image.GetPixelAddress(0, height-1);

	//填充数据
	BYTE* pSrc = img.RGB24_buffer; 
	for(int i=0;i<height;i++)
	{
		int y=height-1-i;
		for(int j=0;j<width;j++)
		{
			Image_Data[(y*width+j)*3]   = pSrc[(i*width+j)*3];
			Image_Data[(y*width+j)*3+1] = pSrc[(i*width+j)*3+1];
			Image_Data[(y*width+j)*3+2] = pSrc[(i*width+j)*3+2];
		}
	}
	//存储
	Image.Save(PathName);

	LeaveCriticalSection(&Cam[CamID].m_DisPlayLock);

	CString str;
	str.Format(_T("已保存到：%s"),PathName);
	Ctrl_Addlog(CamID,str);
	Image_Data = NULL;
	pSrc = NULL;
}

void OFccmControl::SaveRaw(CString PathName,int CamID,int nameType)
{
	//把图像的分辨率，Bayer排列格式写入文件名中，方便后期辨识
	ImageInfo img = * Ctrl_GetImgInfo(CamID);
	int width     = img.imgwidth;
	int height    = img.imgheight;
	int Rawformat = img.RawFormat;
	int rawBits   = img.RawBits;

	CString info,szOutformat;
	switch(Rawformat)
	{
	case OUTFORMAT_BGGR:
		szOutformat = _T("BGGR");
		break;
	case OUTFORMAT_GBRG:
		szOutformat = _T("GBRG");
		break;
	case OUTFORMAT_GRBG:
		szOutformat = _T("GRBG");
		break;
	case OUTFORMAT_RGGB:
		szOutformat = _T("RGGB");
		break;
	default:
		Rawformat = 0;
		szOutformat = _T("RGGB");
		break;

	}
	info.Format(_T("-%d-%d-%s"),width, height,szOutformat);
	if(nameType==0)
	{
		PathName = PathName + info + _T(".raw"); 
	}
	else
	{
		PathName = PathName; 
	}  

	CFile file;
	BOOL ret = file.Open(PathName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if (!ret)
	{
		return;
	} 
	file.Write((void*) img.Raw_buffer, width * height * sizeof(USHORT) );
	file.Close();

	CString str;
	str.Format(_T("已保存到：%s"),PathName);
	Ctrl_Addlog(CamID,str);
}

void OFccmControl::SaveRaw8(CString PathName,int CamID,int nameType)
{
	//把图像的分辨率，Bayer排列格式写入文件名中，方便后期辨识
	ImageInfo img = * Ctrl_GetImgInfo(CamID);
	int width     = img.imgwidth;
	int height    = img.imgheight;
	int Rawformat = img.RawFormat;
	int rawBits   = img.RawBits;

	CString info,szOutformat;
	switch(Rawformat)
	{
	case OUTFORMAT_BGGR:
		szOutformat = _T("BGGR");
		break;
	case OUTFORMAT_GBRG:
		szOutformat = _T("GBRG");
		break;
	case OUTFORMAT_GRBG:
		szOutformat = _T("GRBG");
		break;
	case OUTFORMAT_RGGB:
		szOutformat = _T("RGGB");
		break;
	default:
		Rawformat = 0;
		szOutformat = _T("RGGB");
		break;
	}
	info.Format(_T("-%d-%d-%s"),width, height,szOutformat);

	if(nameType==0)
	{
      	PathName = PathName + info + _T(".raw"); 
	}
	else
	{
		PathName = PathName; 
	}


	CFile file;
	BOOL ret = file.Open(PathName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if (!ret)
	{
		return;
	}
	BYTE* Raw8_buffer = new BYTE[width*height*3];
	if (NULL == Raw8_buffer)
	{
		Ctrl_Addlog(CamID,_T("raw8 image保存失败"),COLOR_RED,200);
		SAFE_DELETE_ARRAY(Raw8_buffer);
		return;
	}
	ImageProc::RawToRaw8(img.Raw_buffer,Raw8_buffer,width,height,rawBits);

	file.Write((void*)  Raw8_buffer, width * height * sizeof(BYTE) );
	file.Close();

	CString str;
	str.Format(_T("已保存到：%s"),PathName);
	Ctrl_Addlog(CamID,str);
	SAFE_DELETE_ARRAY(Raw8_buffer);
}
 
int OFccmControl::Otp_OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,int CamID,USHORT page)
{
	if(Cam[CamID].pOtpSensor == NULL)
	{
		Ctrl_Addlog(CamID,_T("请选择Otp_Sensor"),COLOR_RED,200);
		return 0;
	}
	return Cam[CamID].pOtpSensor->OtpRead(startAddr,endAddr,buf,page);
}
int OFccmControl::Otp_OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,int CamID,USHORT page)
{
	if(Cam[CamID].pOtpSensor == NULL)
	{
		Ctrl_Addlog(CamID,_T("请选择Otp_Sensor"),COLOR_RED,200);
		return 0;
	}
	return Cam[CamID].pOtpSensor->OtpWrite(startAddr,endAddr,buf,page);

}


vector<CString> OFccmControl::GetvecOtpSensor()
{
	CString strOtpSensor = ::GetOtpSensorList();
	vector<CString> vecOtpSensor;
	globalset.SeparateString(strOtpSensor,',',&vecOtpSensor);
	return  vecOtpSensor;
}

int OFccmControl::pCam_Opencamera(int CamID)
{
	int flag = Cam[CamID].pCameraCtrl->OpenCamera(CamID);
	if(flag!=1)
		Cam[CamID].pCameraCtrl->CloseCamera(CamID);
		
	return flag;
}
 
 
void OFccmControl::SetCamCode(CString strCamCode)
{
	this->strCamCode = strCamCode;
}
CString OFccmControl::Ctrl_GetCamCode()
{
	return strCamCode;
}
CString OFccmControl::Sensor_GetCamFuseID(int CamID)
{ 
	return Cam[CamID].fuseID;
}

void OFccmControl::GetDirAllFile(CString dirPath,vector<CString> &fileList)
{
	CFileFind fileFinder;
	CString FindPath = dirPath + _T("*.*");
	BOOL bFind = fileFinder.FindFile(FindPath);
	fileList.clear();
	while (bFind)
	{		
		bFind = fileFinder.FindNextFile();
		//查找指定文件

		if(fileFinder.IsDirectory()) continue;  //如果是目录，跳过去
		if(fileFinder.IsDots()) continue;       //如果是点文件，跳过去
		//CString pathFine = fileFinder.GetFilePath();
		CString pathFile = fileFinder.GetFileName();
		 fileList.push_back(pathFile);
	}
	fileFinder.Close();
}

 void OFccmControl::GetDirAllDirs(CString dirPath,vector<CString> &dirList) 
{
	CFileFind fileFinder;
	CString FindPath = dirPath + _T("*.*");
	BOOL bFind = fileFinder.FindFile(FindPath);
	dirList.clear();
	while (bFind)
	{		
		bFind = fileFinder.FindNextFile();
		//查找指定文件

		if(fileFinder.IsDirectory()) 
		{
			CString pathFile = fileFinder.GetFileName();
			if(pathFile.GetLength()>2) dirList.push_back(pathFile);
		}  //如果是目录，
		if(fileFinder.IsDots()) continue;       //如果是点文件，跳过去 
	}
	fileFinder.Close();
}

BOOL  OFccmControl::isFileExist(CString filePath)
{
	CFileFind fileFinder; 
	BOOL bFind = fileFinder.FindFile(filePath);
	fileFinder.Close();
	return bFind;
}

USHORT OFccmControl::Ctrl_GetCamTemperature(int CamID)
{
	USHORT temperature(0);

	if(Cam[CamID].pImgSensor!=NULL)
	{
      Cam[CamID].pImgSensor->GetTemperature(temperature);	
	}
	else
	{
       Ctrl_Addlog(CamID,_T("没有设定sensor"), COLOR_RED, 220);
	}

	return temperature;


}
int OFccmControl::Ctrl_GetCamType()
{
	int CamType = 0;
	if(globalset.CamType==_T("Normal"))
	{
		CamType = 0;
	}
	else
	{
		CamType = 1;
	}

	return CamType;
}
void OFccmControl::Ctrl_SetTestItemID(int CamID,int TestItemID)
{
	//PostMsgToControler(TM_SINGLETEST,CamID,TestItemID);
	ExecuteItemTest( TestItemID, CamID);
	 
}
int OFccmControl::Ctrl_GetTestItemID(int CamID)
{ 
     if(CamID==2||CamID==3)//用于机器模式，不良分类,CamID:2表示0,3表示1
	 {
	     return Cam[CamID&0x01].pTestCtrl->GetLastID();
	 }
	 	return Cam[CamID].pTestCtrl->GetCurrentID();
}
void OFccmControl::Ctrl_SetDualCamData(int CamID,DualCamData m_DualCamData)
{
	Cam[CamID].m_DualCamData = m_DualCamData;
}
DualCamData* OFccmControl::Ctrl_GetDualCamData(int CamID)
{	
	return &Cam[CamID].m_DualCamData;
}
void OFccmControl::Ctrl_AutoAlignMoveTo(int CamID,int motor,double pos)
{
	CString str;
	str.Format(_T("CamID%d motor%d pos"),CamID,motor,pos);
	Ctrl_AddToLog(str,CamID);
	 
	Cam[CamID].camSocket->Move(motor,pos);
}
float OFccmControl::Ctrl_AutoAlignGetPos(int CamID,int motor)
{	 
	return (float)Cam[CamID].camSocket->GetPos(motor);
}
 

CString OFccmControl::Ctrl_GetCurrentPath()
{
	return globalset.GetCurrentPath();
}
BOOL OFccmControl::OpenImg(int CamID,CString srcImgPath)
{
	return Cam[CamID].pImgExplorer->Open(srcImgPath);
}
void OFccmControl::Ctrl_AddToLog(CString LogInfo,int CamID)
{
	if(!globalset.bLogSave) return;
	CString strInfo = LogInfo+ _T("\n");
	CStdioFile file;	
	if(file.Open(Cam[CamID].logPath,CFile::modeWrite))
	{
		file.SeekToEnd();
		file.WriteString(strInfo);
		file.Close();
	}
}

void OFccmControl::DrawingAndShowResult(int CamID,CString strInfo,BOOL bResult,BOOL Result_ok2,BOOL bReFreshUi )
{	
	if(bReFreshUi)
		return;
	int DualCamID = 0;
	
	CString strResult;

	strResult.Format(_T("Cam[%d].TestItemNum = %d"),CamID,pCcmCtrl->Cam[CamID].TestItemNum);
	Ctrl_Addlog(CamID,strResult, COLOR_BLUE, 200);

	strResult.Format(_T("Cam[%d].TestItemPass = %d"),CamID,pCcmCtrl->Cam[CamID].TestItemPass);
	Ctrl_Addlog(CamID,strResult, COLOR_BLUE, 200);

	CRect rect;
	Ctrl_GetDisplayWndRect(&rect,CamID);	
	int fontSize = rect.Height()*2/3;

	if(bResult)
	{		
		ShowResult[CamID] = 1;
		Ctrl_Addlog(CamID,strInfo, COLOR_GREEN, fontSize);

		Sleep(100); 
		
		if (globalset.CamType==_T("Dual"))
		{  
			//isShow = TRUE;
			if ( (ShowResult[0] == 1) && (ShowResult[1]==1) 
			&& (pCcmCtrl->Cam[0].TestItemNum == pCcmCtrl->Cam[0].TestItemPass) 
			&& (pCcmCtrl->Cam[1].TestItemNum == pCcmCtrl->Cam[1].TestItemPass))
			{
				strResult.Format(L"cam0:%d,cam1:%d", ShowResult[0], ShowResult[1]);
				Ctrl_Addlog(CamID,strResult, COLOR_BLUE, fontSize);

				for(int i=0;i<2;i++)
				{
					Ctrl_DrawingToMenDC(i ,COLOR_GREEN);
					Ctrl_DrawingToMenDC(i ,COLOR_BROWN,strInfo,fontSize,4,2);
					Ctrl_ShowMemDC(i );	
				}
			}
			else if ( ShowResult[1-CamID] == -1 || (ShowResult[0] == 1) 
			&& (ShowResult[1]==1) &&((pCcmCtrl->Cam[0].TestItemNum != pCcmCtrl->Cam[0].TestItemPass)  
			||  (pCcmCtrl->Cam[1].TestItemNum != pCcmCtrl->Cam[1].TestItemPass)))
			{
				strResult.Format(L"cam0:%d,cam1:%d", ShowResult[0], ShowResult[1]);
				Ctrl_Addlog(CamID,strResult, COLOR_BLUE, fontSize);

				strInfo = Cam[CamID].errorInfo;
				for (int i=0; i<2; i++)
				{
					if ((Cam[i].MesResult.Res == FALSE && Cam[i].MesResult.MesErrorInfo != _T("")))
					{
						fontSize = rect.Height()*1/3;
						strInfo = Cam[i].MesResult.MesErrorInfo;
						Ctrl_SetCamErrorInfo(CamID,strInfo);
					}
				}
	
				for(int i=0;i<2;i++)
				{
					Ctrl_DrawingToMenDC(i ,COLOR_RED);
					Ctrl_DrawingToMenDC(i ,COLOR_BROWN,strInfo,fontSize,4,2);
					Ctrl_ShowMemDC(i );
				} 
			}
			
		}
		else
		{
			if (pCcmCtrl->Cam[CamID].TestItemNum == pCcmCtrl->Cam[CamID].TestItemPass)
			{
 				if(bResult==2) 
					Ctrl_DrawingToMenDC(CamID,COLOR_YELLOW);
 				else
				    Ctrl_DrawingToMenDC(CamID,COLOR_GREEN);

				strInfo = Cam[CamID].errorInfo;
				if (strInfo.Compare(_T("TEST FAIL!")) == 0)
				{
					strInfo=_T("TEST PASS");
				}

				Ctrl_DrawingToMenDC(CamID,COLOR_BROWN,strInfo,fontSize,4,2);
				Ctrl_ShowMemDC(CamID);
			}
			else
			{
				Ctrl_DrawingToMenDC(CamID,COLOR_RED);
				strInfo = Cam[CamID].errorInfo;
				if (strInfo.Compare(_T("TEST PASS!")) == 0)
				{
					strInfo=_T("TEST Fail");
				}

				if (Cam[CamID].MesResult.Res == FALSE && Cam[CamID].MesResult.MesErrorInfo != _T(""))
				{
					fontSize = rect.Height()*1/3;
					strInfo = Cam[CamID].MesResult.MesErrorInfo;
					Ctrl_SetCamErrorInfo(CamID,strInfo);
				}

				//xzc add for 光源校准
				if (strInfo.Compare(_T("请放入CheckLast模组")) == 0)
				{
					Ctrl_DrawingToMenDC(CamID,COLOR_GREEN);
				}
				else if (strInfo.Compare(_T("CheckLast Pass")) == 0)
				{
					Ctrl_DrawingToMenDC(CamID,COLOR_GREEN);
				}

				Ctrl_DrawingToMenDC(CamID,COLOR_BROWN,strInfo,fontSize,4,2);
				Ctrl_ShowMemDC(CamID);
			}
			
		}
	}
	else
	{  
		CString strLog;
		Cam[CamID].errorInfo.MakeUpper();
		strLog.Format(L"NG Info = %s", Cam[CamID].errorInfo);
		Ctrl_Addlog(CamID,strLog, COLOR_RED, fontSize); 
		if ((Cam[CamID].errorInfo.Find(_T("PASS"))!=-1)||(Cam[CamID].errorInfo.Find(_T("OK"))!=-1)||Cam[CamID].errorInfo==_T(""))//异常处理,防止代码不规范有settingErrorInfo "PASS",""的情况
		{
			Cam[CamID].errorInfo=_T("TEST FAIL");
		}
		if ((strInfo.Find(_T("PASS"))!=-1)||(strInfo.Find(_T("OK"))!=-1)||strInfo==_T(""))//异常处理//机器模式刷屏时，此处传的是TEST_PASS,所以strInfo会被赋值为"TEST FAIL"
		{
			strInfo=_T("TEST FAIL");
		}
		if (Ctrl_GetCamType()==1 )
		{  
			ShowResult[CamID] = -1;
			//isShow = TRUE;
			if ( ShowResult[1-CamID] != 0)//都做完了再show ui
			{
				strResult.Format(L"cam0:%d,cam1:%d", ShowResult[0], ShowResult[1]);
				Ctrl_Addlog(CamID,strResult, COLOR_BLUE, fontSize);

				for(int i=0;i<2;i++)
				{
					if(strInfo==_T("TEST FAIL"))
						strInfo = Cam[CamID].errorInfo;

					if (Cam[i].MesResult.Res == FALSE && Cam[i].MesResult.MesErrorInfo != _T(""))
					{
						fontSize = rect.Height()*1/3;
						strInfo = Cam[i].MesResult.MesErrorInfo;
						Ctrl_SetCamErrorInfo(i,strInfo);
					}
					
					Ctrl_Addlog(i,strInfo, COLOR_RED, fontSize);
					Ctrl_DrawingToMenDC(i ,COLOR_RED);
					Ctrl_DrawingToMenDC(i ,COLOR_BROWN,strInfo,fontSize,4,2);
					Ctrl_ShowMemDC(i);
				} 
			}	
		}
		else
		{
			strInfo = Cam[CamID].errorInfo; 

			if (Cam[CamID].MesResult.Res == FALSE && Cam[CamID].MesResult.MesErrorInfo != _T(""))
			{
				fontSize = rect.Height()*1/3;
				strInfo = Cam[CamID].MesResult.MesErrorInfo;
				Ctrl_SetCamErrorInfo(CamID,strInfo);
			}

			Ctrl_Addlog(CamID,strInfo, COLOR_RED, fontSize); 
			  
			Ctrl_DrawingToMenDC(CamID,COLOR_RED);
			Ctrl_DrawingToMenDC(CamID,COLOR_BROWN,strInfo,fontSize,4,2);
			Ctrl_ShowMemDC(CamID);
		}
	}
}

void OFccmControl::Ctrl_SetAAResult(BOOL bAaResult)
{
	if(!Cam[1].pConfig->GetGeneralTab()->SocketEn)
	{
		Ctrl_Addlog(CamID,_T("SeverSocket未创建"), COLOR_RED, 220); 
		return ;
	}
	CString Info;
	 
	if(bAaResult)
	{
		Ctrl_Addlog(CamID,_T("发送AA结果PASS"), COLOR_BLUE, 220);
		Info =  _T("DualCameraFinalCheckPass\n");
	}
	else
	{
		Ctrl_Addlog(CamID,_T("发送AA结果FAIL"), COLOR_RED, 220);
		Info =  _T("DualCameraFinalCheckFail\n");
	}
	if(Cam[1].pServerSocket->m_client!=NULL)
	{
		Cam[1].pServerSocket->m_client->SendInfo(Info);
	}
	else
	{
		Ctrl_Addlog(CamID,_T("Socket掉了"), COLOR_RED, 220); 
	} 
}
 

USHORT OFccmControl::GetRgain(int CamID)
{
	int width = Cam[CamID].m_ImageInfo.imgwidth;
	int height = Cam[CamID].m_ImageInfo.imgheight;

	CRect rect;
	rect.left = width/2-width/10;
	rect.right = width/2+width/10;
	rect.top = height/2-height/10;
	rect.bottom = height/2+height/10;

	return ImageProc::GetAveRGain_ROI_RGB24(Cam[CamID].pBuf.RGB24_buffer,width,height,rect);

}
USHORT OFccmControl::GetBgain(int CamID)
{
	int width = Cam[CamID].m_ImageInfo.imgwidth;
	int height = Cam[CamID].m_ImageInfo.imgheight;

	CRect rect;
	rect.left = width/2-width/10;
	rect.right = width/2+width/10;
	rect.top = height/2-height/10;
	rect.bottom = height/2+height/10;

	return ImageProc::GetAveBGain_ROI_RGB24(Cam[CamID].pBuf.RGB24_buffer,width,height,rect);
}

void OFccmControl::ManuBalance(int CamID)
{
	int width = Cam[CamID].m_ImageInfo.imgwidth;
	int height = Cam[CamID].m_ImageInfo.imgheight;

	ImageProc::ManuBalance(Cam[CamID].pBuf.RGB24_buffer,width,height,Rgain,Bgain); 
}

int OFccmControl::SetRgain(int CamID)
{
	int width = Cam[CamID].m_ImageInfo.imgwidth;
	int height = Cam[CamID].m_ImageInfo.imgheight;

	CRect rect;
	rect.left = width/2-width/10;
	rect.right = width/2+width/10;
	rect.top = height/2-height/10;
	rect.bottom = height/2+height/10;

	Rgain = ImageProc::GetFloatBGain_ROI_RGB24(Cam[CamID].pBuf.RGB24_buffer,width,height,rect);
	return 0;
}

void OFccmControl::Ctrl_SetRgain(int CamID,float Rgain )
{
	this->Rgain = Rgain; 
}

void OFccmControl::Ctrl_SetBgain(int CamID,float Bgain)
{
	this->Bgain = Bgain;
}

int OFccmControl::SetBgain(int CamID)
{
	int width = Cam[CamID].m_ImageInfo.imgwidth;
	int height = Cam[CamID].m_ImageInfo.imgheight;

	CRect rect;
	rect.left = width/2-width/10;
	rect.right = width/2+width/10;
	rect.top = height/2-height/10;
	rect.bottom = height/2+height/10;

	Bgain = ImageProc::GetFloatBGain_ROI_RGB24(Cam[CamID].pBuf.RGB24_buffer,width,height,rect);
	return 0;
}

CPoint OFccmControl::Ctrl_GetCurrentRoiPos(int CamID)
{ 
	return Cam[CamID].CamPoint ;

}

int OFccmControl::VCM_WriteAF_Distance(int Distance, int CamID)
{
	VcmDr_InitAF(CamID); 
	if(!SetAF_CodeMap(CamID))
	{
		Ctrl_Addlog(CamID,_T("AFmapSetting不匹配"),COLOR_RED,200);
		return 0;
	}
	USHORT AF_Code = GetAfCode(vecAfCode,vecDist,Distance);
	VcmDr_WriteAF_Code(AF_Code,CamID);  
	return 0;
}
BOOL OFccmControl::SetAF_CodeMap(int CamID)
{ 
	vector<int> vecAfMap;
	CString CurrentPath = globalset.GetCurrentPath();
	CString FilePath = CurrentPath + _T("\\SettingTab\\AF_Code_Map.txt");
	CString KeyName ;
	KeyName.Format(_T("AfCodeMapCam%d_%s"),CamID,strCamCode);
	pConfigFileRw->SetConfigFilePath(FilePath);
	CString strAF_CodeMap =  pConfigFileRw->ReadConfigString(_T("AF_CodeMap"),KeyName,_T(""));

	CString strAF_DistMap =  pConfigFileRw->ReadConfigString(_T("AF_DistMap"),_T("AF_DistMap"),_T(""));
	vector<CString> strVecAF_CodeMap;
	vector<CString> strVecAF_DistMap;
	globalset.SeparateString(strAF_CodeMap,',',&strVecAF_CodeMap);
	globalset.SeparateString(strAF_DistMap,',',&strVecAF_DistMap);
	if(strVecAF_CodeMap.size() != strVecAF_DistMap.size() ) return FALSE;
	for(UINT i=0;i<strVecAF_CodeMap.size();i++)
	{ 
		vecAfCode.push_back( _ttoi(strVecAF_CodeMap[i]));
		vecDist.push_back( _ttoi(strVecAF_DistMap[i])); 
	}
	return TRUE;
}


int OFccmControl::GetAfCode(vector<int> vecCode,vector<int> vecDist,int Dist)
{
	int afcode = 0;
	for(UINT i=0;i<vecCode.size()-1;i++)
	{
		if(vecDist[i]<Dist&&Dist<vecDist[i+1])
		{ 
			afcode = vecCode[i] +(Dist-vecDist[i+1])/(vecDist[i]-vecDist[i+1])*(vecCode[i]-vecCode[i+1]); 
		}
		if(Dist>vecDist[i+1])
		{
			afcode = vecCode[i+1];
		}

	}
	return afcode;
}
void  OFccmControl::OnLButtonUpPoint(int CamID,CPoint cp)
{
	SetCamMousePos(  CamID,  cp);

	if(globalset.bOnLButtonUp)
	{
		Cam[CamID].pTestCtrl->SetTestItem(globalset.ButtonUpTestItem);
	}
}

void OFccmControl::SetCamMousePos(int CamID,CPoint cp)
{
	int x,y;
	CRect rect;
	Ctrl_GetDisplayWndRect(& rect,  CamID);
	int width = rect.Width();
	int height =  rect.Height();
	x =  100*cp.x/width;
	y =  100*cp.y/height;
	Cam[CamID].CamPoint.x =x;
	Cam[CamID].CamPoint.y =y; 
}
 
 void OFccmControl::Ctrl_SetTestInfo(int CamID,int TestItemID,TestItemInfo mTestItemInfo)
{ 
	Cam[CamID].pTestCtrl->SetTestInfo(TestItemID,mTestItemInfo);
}

  void OFccmControl::Ctrl_GetTestInfo(int CamID,int TestItemID,TestItemInfo& mTestItemInfo)
{
	mTestItemInfo = Cam[CamID].pTestCtrl->GetTestInfo(TestItemID);
}

  void OFccmControl::Ctrl_GetTestInfo(int CamID,CString itemName,TestItemInfo& mTestItemInfo)
  {
	  mTestItemInfo = Cam[CamID].pTestCtrl->GetTestInfo(itemName);
  }

int OFccmControl::Device_WriteI2cBlock(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,int iDevID,int CamID )
{
	return pDevice->WriteI2c( uDevAddr, uRegAddr, uRegAddrSize,pData, uSize, iDevID, CamID );
}


int OFccmControl::Device_ReadI2cBlock(UCHAR uDevAddr,USHORT uRegAddr,UCHAR uRegAddrSize,BYTE *pData,USHORT uSize,BOOL bNoStop,int iDevID,int CamID)
{
	if(pData==NULL) return -1; 
	return pDevice->ReadI2c( uDevAddr, uRegAddr, uRegAddrSize,pData, uSize, bNoStop, iDevID, CamID );
}
 
 
int OFccmControl::Otp_WriteToOtpBoard(int CamID,CString name,vector<BYTE> byteData,BOOL bIn) 
{
	CString otpInfo; 
	CString FileName;  
	FileName = _T("\\") + name;  
	CString filePath;
	if(bIn)
	{ 
		filePath = Cam[CamID].otpByteInPath + FileName; 
	}
	else
	{  
		filePath = Cam[CamID].otpByteOutPath + FileName; 
	} 
	if(PathFileExists(FileName))
	{	
		if (!DeleteFile(FileName))
		{
			Ctrl_Addlog(CamID,_T("无法删除旧文件!"),COLOR_RED,200);
			return FALSE;
		}
	}
    FILE * file;
	USES_CONVERSION;
	file=fopen(T2A(filePath),"wb"); 
	if (file==NULL)
	{
		Ctrl_Addlog(CamID,_T("无法生成bin文件!"),COLOR_RED,200);
		return FALSE;
	}
	int inputcount=byteData.size();
	int n=fwrite(&byteData[0],1,inputcount,file);
	if (n!=inputcount)
	{
		Ctrl_Addlog(CamID,_T("bin文件写入失败!"),COLOR_RED,200);
		return FALSE;
	}
	fclose(file);
	return 1;
}
int  OFccmControl::Otp_ReadOtpBoard(int CamID,CString name,vector<BYTE> *byteData,BOOL bIn)
{ 
	CString otpInfo; 
	FILE *file;
	CString filePath;
	CString FileName;  
	BYTE byteOtp;
	FileName = _T("\\") + name;  
	if(bIn)
	{ 
		filePath = Cam[CamID].otpByteInPath + FileName; 
	}
	else
	{  
		filePath = Cam[CamID].otpByteOutPath + FileName; 
	} 
	filePath = Cam[CamID].otpByteInPath + FileName;
	USES_CONVERSION;
	int outputcount=byteData[0].size();
	file=fopen(T2A(filePath),"rb"); 
	if (file==NULL)
	{
	   Ctrl_Addlog(CamID,_T("打开文件失败:")+filePath,COLOR_RED,200);
	   return FALSE;
	}
	int n=fread(&byteData[0][0],1,outputcount,file);
	fclose(file);
	if (n!=outputcount)
	{
		Ctrl_Addlog(CamID,_T("bin文件读取失败!"),COLOR_RED,200);
		return FALSE;
	} 
	return 1;
}

 
int OFccmControl::Otp_WriteToOtpBoard(int CamID,CString name,vector<USHORT> byteData,BOOL bIn) 
{
	CString otpInfo; 
	CStdioFile file;
	CString filePath;
	CString FileName;  
	FileName = _T("\\") + name;  
	if(bIn)
	{ 
 		filePath= Cam[CamID].otpItemdataPath + FileName; 
	}
	else
	{
		filePath= Cam[CamID].otpByteInPath + FileName; 
	}
	

	if(PathFileExists(FileName))
	{	
		if (!DeleteFile(FileName))
		{
			Ctrl_Addlog(CamID,_T("无法删除旧文件!"),COLOR_RED,200);
			return FALSE;
		}
	}
	BOOL ret = file.Open(filePath, CFile::modeRead|CFile::modeWrite);
	if (ret==1)
	{
		for(UINT j=0;j<byteData.size();j++)
		{
			otpInfo.Format(_T("0x%x\n"),byteData[j]);
			file.WriteString(otpInfo); 
		}
		file.Close();
	}
	
	return 1;
}
int  OFccmControl::Otp_ReadOtpBoard(int CamID,CString name,vector<USHORT> *byteData,BOOL bIn)
{ 
	CString otpInfo; 
	CStdioFile file;
	CString filePath;
	CString FileName;  
	
	FileName =  _T("\\") + name;  
	if(bIn)
	{ 
		filePath = Cam[CamID].otpItemdataPath + FileName; 
	}
	else
	{  
		filePath = Cam[CamID].otpCheckDataPath + FileName; 
	}   
	 
	if(!file.Open(filePath,CFile::modeRead)) return -1; 
	file.SeekToBegin();  
	do{
		file.ReadString(otpInfo);  
		if(otpInfo.GetLength()>1) 
		{
			USHORT byteOtp = 0;
			swscanf_s(otpInfo,_T("0x%hX"),&byteOtp);
			//byteOtp = 0;
			byteData->push_back(byteOtp); 
		}
		else
		{
			break;
		}
	}while(otpInfo.GetLength()>1);

	file.Close(); 

	return 1;
}


int OFccmControl::Sensor_ReadExp(int CamID,int &exp)
{
	if(Cam[CamID].pImgSensor==NULL)
	{
		return 0x600;
	}
	return Cam[CamID].pImgSensor->ReadExp(exp); 
}
int OFccmControl::Sensor_WriteExp(int CamID,int exp) 
{
	if(Cam[CamID].pImgSensor==NULL)
	{
		return 1;
	}
	return Cam[CamID].pImgSensor->WriteExp(exp); 
}

int OFccmControl::Sensor_ReadGain(int CamID,USHORT &gain)
{
	if(Cam[CamID].pImgSensor==NULL)
	{
		return 0x20;
	}
	return Cam[CamID].pImgSensor->ReadGain(gain);
}
int OFccmControl::Sensor_WriteGain(int CamID,USHORT gain)
{
	if(Cam[CamID].pImgSensor==NULL)
	{
		return 1;
	}
	return  Cam[CamID].pImgSensor->WriteGain(gain);
}

int OFccmControl::Sensor_Init(int CamID)
{
	return Cam[CamID].pImgSensor->Init();
	 
}
int OFccmControl::Sensor_ApplyAWBGain(int CamID,int rg, int bg, int Typical_rg, int Typical_bg) 
{
	return Cam[CamID].pImgSensor->ApplyAWBGain(rg,bg,Typical_rg,Typical_bg);
 
}
int OFccmControl::Sensor_ApplyLsc(int CamID,BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup) 
{
	return Cam[CamID].pImgSensor->ApplyLsc(raw8,width,height,nLSCTarget,ob,nLenCReg,nLenCRegCount,LSCGroup);
}
int OFccmControl::Sensor_GetLsc(int CamID,BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup) 
{
	return Cam[CamID].pImgSensor->GetLsc(P10Buff,width,height,nLSCTarget,ob,nLenCReg,nLenCRegCount,LSCGroup);
}

int OFccmControl::Sensor_ApplySpc(int CamID,short *pSPC)
{
	return Cam[CamID].pImgSensor->ApplySpc(pSPC);
}
int OFccmControl::Sensor_spcCal(int CamID,USHORT *imgBuf,short *pSPC)
{
	return Cam[CamID].pImgSensor->spcCal(imgBuf,pSPC);
}

int OFccmControl::Sensor_GetOB(int CamID)
{
	return Cam[CamID].pImgSensor->GetOB();
}

void OFccmControl::Ctrl_GetotpMapPath(int CamID,CString &otpMapPath)
{
	otpMapPath = Cam[CamID].otpMapPath + _T("\\");
}
void OFccmControl::Ctrl_GetotpByteDataPath(int CamID,CString &otpByteDataPath)
{
	otpByteDataPath = Cam[CamID].otpByteInPath + _T("\\");
}
void OFccmControl::Ctrl_GetotpOutBytePath(int CamID,CString &otpOutByteData)
{
	otpOutByteData = Cam[CamID].otpByteOutPath + _T("\\");
}
void OFccmControl::Ctrl_GetotpItemdataPath(int CamID,CString &otpItemdata) 
{
	otpItemdata = Cam[CamID].otpItemdataPath + _T("\\");
}
void OFccmControl::Ctrl_GetotpCheckDataPath(int CamID,CString &otpCheckDataPath)
{
	otpCheckDataPath = Cam[CamID].otpCheckDataPath + _T("\\");
}

 void OFccmControl::Ctrl_GetAwbRatio(int CamID,vector<float>& awbRatio) 
 {

 }

 void OFccmControl::LogMes()
 {
	 for(int i=0;i<CameraNum;i++)
	 { 
		 if(Cam[0].usedMes&&Cam[1].usedMes)
		 {
			 if (i==Cam[i].MesID)
			 {
				 if(Cam[i].m_Mes.ShowLoginDlg())
				 {
					 Ctrl_Addlog(i,L"Mes已登录",COLOR_GREEN,200);
				 }
			 }
		 }
		 else if (Cam[i].usedMes)
		 {
			 if(Cam[i].m_Mes.ShowLoginDlg())
			 {
				 Ctrl_Addlog(i,L"Mes已登录",COLOR_GREEN,200);
			 }
		 }
	 }
 }

 BOOL OFccmControl::mesCheck(int CamID)
 {
	 BOOL bMesDelete = (CamID>>8);
	 CamID = CamID&0xff;

	 if(!Cam[CamID].usedMes) return TRUE;
	 Cam[CamID].MesSelect = MESCHECK;

	 char temp[1024]={0}; 

	 int tempMesID = Cam[CamID].MesID;

//	 CString fuseid =Sensor_GetCamFuseID(tempMesID);

#if 1
	 CString fuseid = L"";
	 Cam[CamID].fuseID = L"";
	 if ( Cam[CamID].pImgSensor!=NULL)
	 {
		Cam[CamID].pImgSensor->GetFuseID(Cam[CamID].fuseID);
	 }
	 fuseid = Cam[CamID].fuseID;
#endif

	 int nGetCnt=0;
	 while(nGetCnt<3)
	 {
		 if (fuseid.GetLength()>5)
		 {
			 break;
		 }
#if 1
		 Cam[CamID].fuseID = L"";
		 if ( Cam[CamID].pImgSensor!=NULL)
		 {
			 Cam[CamID].pImgSensor->GetFuseID(Cam[CamID].fuseID);
	     }
		 fuseid = Cam[CamID].fuseID;
#endif
		 Sleep(200);
		 nGetCnt++;

	 }
	 fuseid.MakeUpper();

	 ImageProc::ConvertWideToChar(fuseid,Cam[tempMesID].m_Mes.Sensor_ID);
	 memset(temp,0,1024);
	 
	 Cam[tempMesID].m_Mes.MesCheck(temp);
	 CString str(temp);	
     memset(Cam[tempMesID].m_Mes.TestResult,0,512);
	 Ctrl_Addlog(CamID,L"MesResult init",COLOR_BLUE,200);
	 Ctrl_Addlog(CamID,str + fuseid,COLOR_BLUE,200);

	 if((str.Find(_T("OK"))!=-1) && (!bMesDelete))
	 {
		 return TRUE;
	 }
	 else
	 {
		 if (bMesDelete )
		 {
			 Ctrl_Addlog(CamID,L"开始Mes解绑!");
			 Cam[tempMesID].m_Mes.DeleteMesData(temp);
			 CString str1(temp);
			 if (str1.Find(_T("OK")) != -1)
			 {
				 Ctrl_Addlog(CamID,L"Mes解绑成功!");
				 return TRUE;
			 }
			 else
			 {
				 Ctrl_SetCamErrorInfo(CamID,L"Mes解绑Fail" + str1);
				 if (Cam[CamID].MesResult.Res == TRUE)
				 {
					 Cam[CamID].MesResult.Res = FALSE;
					 Cam[CamID].MesResult.MesErrorInfo = str1;
				 }
				 return FALSE;
			 }
		 }

		 Ctrl_SetCamErrorInfo(CamID,L"MesCheck Fail" + str);
		 if (Cam[CamID].MesResult.Res == TRUE)
		 {
			 Cam[CamID].MesResult.Res = FALSE;
			 Cam[CamID].MesResult.MesErrorInfo = str;
		 }
		 return FALSE;
	 }
 }

  

 BOOL OFccmControl::Mes_AddItem(int CamID,CString Name,CString Result)
 { 
	 if(Cam[CamID].usedMes)
	 {
		 int tempMesId = Cam[CamID].MesID;
		 char cName[512] = {0};
		 char cResult[512] = {0};
		 ImageProc::ConvertWideToChar(Name,cName );
		 ImageProc::ConvertWideToChar(Result,cResult );
		 return  Cam[tempMesId].m_Mes.AddTestItem(cName,cResult);
	 }
	 else 
		 return 1;
 }
 BOOL OFccmControl::Mes_AddParameter(int CamID,CString Name,CString Value)
 {
	 if(Cam[CamID].usedMes)
	 {
		 int tempMesId = Cam[CamID].MesID;
		 char cName[512] = {0};
		 char cValue[512] = {0};
		 ImageProc::ConvertWideToChar(Name,cName );
		 ImageProc::ConvertWideToChar(Value,cValue );
		 return  Cam[tempMesId].m_Mes.AddParameter(cName,cValue);
	 } 
	 else return 1;
 }

 BOOL OFccmControl::Mes_SetResult(int CamID,CString Result)
 {  
	
	 Result.MakeUpper();
	 if(Cam[CamID].usedMes)
	 {
		 int tempMesId = Cam[CamID].MesID;
		 if (strcmp(Cam[tempMesId].m_Mes.TestResult,"FAIL")==0)
		 {
			 return 1;
		 }
		 else
		 ImageProc::ConvertWideToChar(Result,Cam[tempMesId].m_Mes.TestResult );
	 }
	 return 1;
 }
 BOOL OFccmControl::Mes_SetErrorCode(int CamID, CString errorCode)
 {
	 if(Cam[CamID].usedMes)
	 {
		  int tempMesId = Cam[CamID].MesID;
		 ImageProc::ConvertWideToChar(errorCode, Cam[tempMesId].m_Mes.ErrorCode ); 
	 }
	  return 1;
 }

 void OFccmControl::SetMesInfo(int CamID,BOOL bMes,int MesID)
 {
	 Cam[CamID].MesID = MesID;
	 Cam[CamID].usedMes = bMes;
 }

 int OFccmControl::UpdateToMes(int CamID)
 {
	 BOOL bSuccessUp = FALSE;
	 if(Cam[CamID].usedMes)
	 { 
		/* if((Cam[0].MesID==1)&& Cam[0].usedMes && Cam[1].usedMes && CamID==0)
		 {
			 return TRUE;
		 }

		 if((Cam[1].MesID==0)&& Cam[0].usedMes && Cam[1].usedMes && CamID==1)
		 {
			 return TRUE;
		 }*/
#if 1
		 if (globalset.CamType == L"Dual")
		 {
			 if (Cam[0].pConfig->GetStandbyCurrentTab()->Test_en)
			 {
				 int Current[5]  = {0};
				 pDevice->PmuGetStandyCurrent(Current,5,0,0);
				 //计算待机总和
				 float fAVDD  = Current[0]/1000.0f;
				 float fDOVDD = Current[1]/1000.0f;
				 float fDVDD  = Current[2]/1000.0f;
				 float fAFVDD = Current[3]/1000.0f;

				 float UP_Avdd = (float)Cam[0].pConfig->GetStandbyCurrentTab()->Pin[0].CurrentUpLimit;
				 float UP_Dovdd = (float)Cam[0].pConfig->GetStandbyCurrentTab()->Pin[1].CurrentUpLimit;
				 float UP_Dvdd = (float)Cam[0].pConfig->GetStandbyCurrentTab()->Pin[2].CurrentUpLimit;
				 float UP_Afvdd = (float)Cam[0].pConfig->GetStandbyCurrentTab()->Pin[3].CurrentUpLimit;

				 float Low_Avdd = (float)Cam[0].pConfig->GetStandbyCurrentTab()->Pin[0].CurrentLowLimit;
				 float Low_Dovdd = (float)Cam[0].pConfig->GetStandbyCurrentTab()->Pin[1].CurrentLowLimit;
				 float Low_Dvdd = (float)Cam[0].pConfig->GetStandbyCurrentTab()->Pin[2].CurrentLowLimit;
				 float Low_Afvdd = (float)Cam[0].pConfig->GetStandbyCurrentTab()->Pin[3].CurrentLowLimit;

				 float Total = 0;
				 int FailCnt = 0;
				 CString temp;

				 //Add for Standby Power Test
				 SensorTab *pTempSensorTab = Cam[0].pConfig->GetSensorTab();
				 Total = (fAVDD * pTempSensorTab->avdd + fDOVDD * pTempSensorTab->dovdd + fDVDD * pTempSensorTab->dvdd) / 10.0;
				 float Total_Low  = (float)Cam[0].pConfig->GetStandbyCurrentTab()->dbStandByPowerMin;
				 float Total_High = (float)Cam[0].pConfig->GetStandbyCurrentTab()->dbStandByPowerMax;
				 if ((Total < Total_Low) || (Total > Total_High))
				 {
					 FailCnt++;
				 }

				 //显示结果 AVDD
				 if (fAVDD > UP_Avdd)
				 {
					 FailCnt++;
				 }

				 //显示结果 DOVDD
				 if (fDOVDD > UP_Dovdd)
				 {
					 FailCnt++;
				 }

				 //显示结果 DVDD
				 if (fDVDD > UP_Dvdd)
				 {
					 FailCnt++;
				 } 
				 if (fAFVDD > UP_Afvdd)
				 {
					 FailCnt++;
				 }

				 CString szResult;
				 if (FailCnt>0)
				 {
					 szResult = _T("FAIL");
				 }
				 else
				 {
					 szResult = _T("Pass");
				 }

				 //记录到CSV文件
				 CString Headers  = _T("Power,<AFVCC,AVDD,DOVDD,DVDD");
				 CString LowerLimits ;
				 CString UpperLimits ;

				 LowerLimits.Format(_T("%f,%f,%f,%f,%f"), Total_Low, Low_Afvdd, Low_Avdd, Low_Dovdd, Low_Dvdd);
				 UpperLimits.Format(_T("%f,%f,%f,%f,%f"), Total_High, UP_Afvdd, UP_Avdd, UP_Dovdd, UP_Dvdd);
				 CString Contents;
				 Contents.Format(_T("%f,%f,%f,%f,%f>"), Total, fAFVDD, fAVDD, fDOVDD, fDVDD);

				 SaveReport(_T("StandyCurrent_Left^"),Headers,LowerLimits,UpperLimits,Contents,szResult,0);
			 }

			 if (Cam[1].pConfig->GetStandbyCurrentTab()->Test_en)
			 {
				 int Current[5]  = {0};
				 pDevice->PmuGetStandyCurrent(Current,5,0,1);
				 //计算待机总和
				 float fAVDD  = Current[0]/1000.0f;
				 float fDOVDD = Current[1]/1000.0f;
				 float fDVDD  = Current[2]/1000.0f;
				 float fAFVDD = Current[3]/1000.0f;

				 float UP_Avdd = (float)Cam[1].pConfig->GetStandbyCurrentTab()->Pin[0].CurrentUpLimit;
				 float UP_Dovdd = (float)Cam[1].pConfig->GetStandbyCurrentTab()->Pin[1].CurrentUpLimit;
				 float UP_Dvdd = (float)Cam[1].pConfig->GetStandbyCurrentTab()->Pin[2].CurrentUpLimit;
				 float UP_Afvdd = (float)Cam[1].pConfig->GetStandbyCurrentTab()->Pin[3].CurrentUpLimit;

				 float Low_Avdd = (float)Cam[1].pConfig->GetStandbyCurrentTab()->Pin[0].CurrentLowLimit;
				 float Low_Dovdd = (float)Cam[1].pConfig->GetStandbyCurrentTab()->Pin[1].CurrentLowLimit;
				 float Low_Dvdd = (float)Cam[1].pConfig->GetStandbyCurrentTab()->Pin[2].CurrentLowLimit;
				 float Low_Afvdd = (float)Cam[1].pConfig->GetStandbyCurrentTab()->Pin[3].CurrentLowLimit;

				 float Total = 0;
				 int FailCnt = 0;
				 CString temp;

				 //Add for Standby Power Test
				 SensorTab *pTempSensorTab = Cam[1].pConfig->GetSensorTab();
				 Total = (fAVDD * pTempSensorTab->avdd + fDOVDD * pTempSensorTab->dovdd + fDVDD * pTempSensorTab->dvdd) / 10.0;
				 float Total_Low  = (float)Cam[1].pConfig->GetStandbyCurrentTab()->dbStandByPowerMin;
				 float Total_High = (float)Cam[1].pConfig->GetStandbyCurrentTab()->dbStandByPowerMax;
				 if ((Total < Total_Low) || (Total > Total_High))
				 {
					 FailCnt++;
				 }

				 //显示结果 AVDD
				 if (fAVDD > UP_Avdd)
				 {
					 FailCnt++;
				 }

				 //显示结果 DOVDD
				 if (fDOVDD > UP_Dovdd)
				 {
					 FailCnt++;
				 }

				 //显示结果 DVDD
				 if (fDVDD > UP_Dvdd)
				 {
					 FailCnt++;
				 } 
				 if (fAFVDD > UP_Afvdd)
				 {
					 FailCnt++;
				 }

				 CString szResult;
				 if (FailCnt>0)
				 {
					 szResult = _T("FAIL");
				 }
				 else
				 {
					 szResult = _T("Pass");
				 }

				 //记录到CSV文件
				 CString Headers  = _T("Power,<AFVCC,AVDD,DOVDD,DVDD");
				 CString LowerLimits ;
				 CString UpperLimits ;

				 LowerLimits.Format(_T("%f,%f,%f,%f,%f"), Total_Low, Low_Afvdd, Low_Avdd, Low_Dovdd, Low_Dvdd);
				 UpperLimits.Format(_T("%f,%f,%f,%f,%f"), Total_High, UP_Afvdd, UP_Avdd, UP_Dovdd, UP_Dvdd);
				 CString Contents;
				 Contents.Format(_T("%f,%f,%f,%f,%f>"), Total, fAFVDD, fAVDD, fDOVDD, fDVDD);

				 SaveReport(_T("StandyCurrent_Right^"),Headers,LowerLimits,UpperLimits,Contents,szResult,1);
			 }

		 }
		 else
		 {
			 if (Cam[CamID].pConfig->GetStandbyCurrentTab()->Test_en)
			 {
				 int Current[5]  = {0};
				 pDevice->PmuGetStandyCurrent(Current,5,0,CamID);
				 //计算待机总和
				 float fAVDD  = Current[0]/1000.0f;
				 float fDOVDD = Current[1]/1000.0f;
				 float fDVDD  = Current[2]/1000.0f;
				 float fAFVDD = Current[3]/1000.0f;

				 float UP_Avdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[0].CurrentUpLimit;
				 float UP_Dovdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[1].CurrentUpLimit;
				 float UP_Dvdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[2].CurrentUpLimit;
				 float UP_Afvdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[3].CurrentUpLimit;

				 float Low_Avdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[0].CurrentLowLimit;
				 float Low_Dovdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[1].CurrentLowLimit;
				 float Low_Dvdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[2].CurrentLowLimit;
				 float Low_Afvdd = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->Pin[3].CurrentLowLimit;

				 float Total = 0;
				 int FailCnt = 0;
				 CString temp;

				 //Add for Standby Power Test
				 SensorTab *pTempSensorTab = Cam[CamID].pConfig->GetSensorTab();
				 Total = (fAVDD * pTempSensorTab->avdd + fDOVDD * pTempSensorTab->dovdd + fDVDD * pTempSensorTab->dvdd) / 10.0;
				 float Total_Low  = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->dbStandByPowerMin;
				 float Total_High = (float)Cam[CamID].pConfig->GetStandbyCurrentTab()->dbStandByPowerMax;
				 if ((Total < Total_Low) || (Total > Total_High))
				 {
					 FailCnt++;
				 }

				 //显示结果 AVDD
				 if (fAVDD > UP_Avdd)
				 {
					 FailCnt++;
				 }

				 //显示结果 DOVDD
				 if (fDOVDD > UP_Dovdd)
				 {
					 FailCnt++;
				 }

				 //显示结果 DVDD
				 if (fDVDD > UP_Dvdd)
				 {
					 FailCnt++;
				 } 
				 if (fAFVDD > UP_Afvdd)
				 {
					 FailCnt++;
				 }

				 CString szResult;
				 if (FailCnt>0)
				 {
					 szResult = _T("FAIL");
				 }
				 else
				 {
					 szResult = _T("Pass");
				 }

				 //记录到CSV文件
				 CString Headers  = _T("Power,<AFVCC,AVDD,DOVDD,DVDD");
				 CString LowerLimits ;
				 CString UpperLimits ;

				 LowerLimits.Format(_T("%f,%f,%f,%f,%f"), Total_Low, Low_Afvdd, Low_Avdd, Low_Dovdd, Low_Dvdd);
				 UpperLimits.Format(_T("%f,%f,%f,%f,%f"), Total_High, UP_Afvdd, UP_Avdd, UP_Dovdd, UP_Dvdd);
				 CString Contents;
				 Contents.Format(_T("%f,%f,%f,%f,%f>"), Total, fAFVDD, fAVDD, fDOVDD, fDVDD);
				 if (0 == CamID)
				 {
					 SaveReport(_T("StandyCurrent_Left^"),Headers,LowerLimits,UpperLimits,Contents,szResult,CamID);
				 }
				 else
				 {
					 SaveReport(_T("StandyCurrent_Right^"),Headers,LowerLimits,UpperLimits,Contents,szResult,CamID);
				 }
			 }
		 }
#endif
		

		 int tempMesId  = Cam[CamID].MesID;
		 char reBack[10240] = {0};
		 memset(reBack, 0, sizeof(char) * 10240);
		 
         CString str;
		 for(int i=0;i<3;i++)
		 {
		    Cam[tempMesId].m_Mes.MesUpdateData(reBack);
		    str = reBack;
		    Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
			 if(str.Find(_T("OK"))!=-1 && str.GetLength()<4)
			 {
				 Ctrl_Addlog(CamID,_T("MES SUCCESS"),COLOR_GREEN,200);
				 bSuccessUp = TRUE;
				 Cam[CamID].bSuccessMes=TRUE;


				 Cam[tempMesId].m_Mes.ResetDataAfterMesUpdateMaybeFail();
				 return 1;
			 }
			 Sleep(1000);
		 }

		 Cam[tempMesId].m_Mes.ResetDataAfterMesUpdateMaybeFail();

		 Cam[CamID].bSuccessMes=bSuccessUp;
		 if(!bSuccessUp) 
		 {
			 if (Cam[CamID].MesResult.Res == TRUE)
			 {
				 Cam[CamID].MesResult.Res = FALSE;
				 Cam[CamID].MesResult.MesErrorInfo = str;
			 }
			 Ctrl_Addlog(CamID,str,COLOR_RED,200);
			 return -1;
		 }
	 }
	 return 1;
 }
  
 void OFccmControl::Ctrl_SetMachineItemID(int CamID,vector<int> MachineItemID)
 {
	 Cam[CamID].pTestCtrl->SetMachineItemID(MachineItemID);
 }

 void OFccmControl::Ctrl_SetCamErrorInfo(int CamID,CString errorInfo)
 {
	 Cam[CamID].errorInfo = errorInfo;

	 if (globalset.CamType.Find(_T("Dual")) !=-1)
	 {
		 Cam[0].errorInfo = errorInfo;
		 Cam[1].errorInfo = errorInfo;
		// Ctrl_Addlog(CamID,errorInfo,COLOR_RED,200);
	 }
	 Ctrl_Addlog(CamID, errorInfo, COLOR_RED, 200);
 }

 CString OFccmControl::Ctrl_GetCamErrorInfo(int CamID)
 {
	 return Cam[CamID].errorInfo;
 }

 void OFccmControl::Ctrl_SetCamErrorCode(int CamID,int errorCode)
 {
	 if (CamID==2||CamID==3)//只有Openfail有使用。
	 {
		 Cam[CamID-2].errorCode = errorCode;
	 }
 }
 int  OFccmControl::Ctrl_GetCamErrorCode(int CamID)
 {
	 return Cam[CamID].errorCode;
 }

  void OFccmControl::SetGpioPinLevel(int gpioNum,BOOL bH,int CamID)
 {
	pDevice->SetGpioPinLevel(gpioNum,bH,CamID);
 }
  void OFccmControl::SensorEnable(int gpioNum,BOOL bH,int CamID)
  {
	  pDevice->SensorEnable(gpioNum,bH,CamID);
  }
  int OFccmControl::GetSiteNo()
  {
	  return OFccmControl::globalset.siteNo;
  }

  BOOL OFccmControl::mesBanding(int CamID,CString Runcard,CString BarCode)
  {
	  if (!Cam[CamID].usedMes) return TRUE;
	  int tempMesID = Cam[CamID].MesID;

	  Cam[CamID].MesSelect = MESBINDING;
	  char temp[1024]={0};

//	  CString fuseid = Sensor_GetCamFuseID(tempMesID);
#if 1
	  CString fuseid = L"";
	  Cam[CamID].fuseID = L"";
	  if ( Cam[CamID].pImgSensor!=NULL)
	  {
		  Cam[CamID].pImgSensor->GetFuseID(Cam[CamID].fuseID);
	  }
	  
	  fuseid = Cam[CamID].fuseID;
#endif
	  int nGetCnt=0;
	  while(nGetCnt<3)
	  {
		  if (fuseid.GetLength()>5)
		  {
			  break;
		  }

		  Cam[CamID].fuseID = L"";
		  if ( Cam[CamID].pImgSensor!=NULL)
		  {
			Cam[CamID].pImgSensor->GetFuseID(Cam[CamID].fuseID);
		  }
		  fuseid = Cam[CamID].fuseID;
		  Sleep(200);
		  nGetCnt++;
	  }
	  fuseid.MakeUpper();	
	  ImageProc::ConvertWideToChar(fuseid,Cam[tempMesID].m_Mes.Sensor_ID);

	  if (Runcard.GetLength() > 0)
	  {
		  char cRunCard[512] = {0};
		  ImageProc::ConvertWideToChar(Runcard,cRunCard);
		  strcpy_s(Cam[tempMesID].m_Mes.Run_Card,512,cRunCard);
	  }
	  if (BarCode.GetLength() > 0)
	  {
		  char cBarcode[512] = {0};
		  ImageProc::ConvertWideToChar(BarCode,cBarcode);
		  strcpy_s(Cam[tempMesID].m_Mes.Barcode_SN,512,cBarcode);
	  }
	  memset(Cam[tempMesID].m_Mes.TestResult,0,512);//TestResult 初始化
	  return TRUE;
  }
  int  OFccmControl::InitiaSensor(int CamID,CString SettingPath)
  {
	  BOOL isChangeSize = CamID&0x0f;
	  CamID = CamID>>4;

#if 0
	  if (isChangeSize)
	  {
		  Cam[CamID].pCameraCtrl->CloseCamera(CamID);//关闭摄像头
		  Sleep(200);
	  }	
#endif

	  CString preSettingPath;
	  preSettingPath = Cam[CamID].pConfig->GetConfigFilePath();
	  CString strLog;
	  strLog.Format(L"PreSettingPath:%s",preSettingPath);
	  pCcmCtrl->Ctrl_Addlog(CamID, strLog, COLOR_BLUE, 200);
	  if (SettingPath.Right(4)==_T(".ini"))
	  {
		  SettingPath=Ctrl_GetCurrentPath()+_T("SensorTab\\")+SettingPath;
		  Cam[CamID].pConfig->SetConfigFilePath(SettingPath);
		  Cam[CamID].pConfig->pConfigFileRw->SetConfigFilePath(SettingPath);
		  Cam[CamID].pConfig->LoadSensorGenaralTab();
		  pDevice->SetCamTab(CamID);
	  }
	  else
	  {
		  pCcmCtrl->Ctrl_Addlog(CamID,L"点亮设定后缀命名错误,不为.ini!",COLOR_RED,200);
		  return 0;
	  }

#if 0
	  if (!isChangeSize)
	  {
	 	  if (!pDevice->InitiaSensor(CamID,SettingPath))
	 	  {
			  Cam[CamID].pConfig->SetConfigFilePath(preSettingPath);
	//		  Cam[CamID].pConfig->pConfigFileRw->SetConfigFilePath(preSettingPath);
	 		  return 0;
     	  }
	  }
#endif

	  if (isChangeSize)
	  {
		  SAFE_DELETE_ARRAY(Cam[CamID].pBuf.grab_buffer);  
		  SAFE_DELETE_ARRAY(Cam[CamID].pBuf.display_buffer); 
		  SAFE_DELETE_ARRAY(Cam[CamID].pBuf.transfer_buffer); 
		  SAFE_DELETE_ARRAY(Cam[CamID].pBuf.RGB24_buffer); 
		  SAFE_DELETE_ARRAY(Cam[CamID].pBuf.raw_buffer); 

		  GetBuffer(CamID); 
#if 0
		  pCcmCtrl->pDevice->SetDevName(pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->DevName); 
		  if(pCcmCtrl->pDevice->GetKitTypeDeviceInfo(CamID)<1)
		  { 	
			  pCcmCtrl->Ctrl_Addlog(CamID,_T("度信没有连接"),COLOR_RED,200);
			  pCcmCtrl->OpenFailProc(CamID);
			  pCcmCtrl->Ctrl_SetCamErrorInfo(CamID,_T("度信没有连接"));
			  Cam[CamID].pConfig->SetConfigFilePath(preSettingPath);
	//		  Cam[CamID].pConfig->pConfigFileRw->SetConfigFilePath(preSettingPath);
			  return 0; 
		  }

		  Cam[CamID].pCameraCtrl->OpenCamera(CamID);//打开摄像头
		  Sleep(200);	
#endif
	  }

	  if (!pDevice->InitiaSensor(CamID,SettingPath))
	  {
		  Cam[CamID].pConfig->SetConfigFilePath(preSettingPath);
		  return 0;
	  }

	  Cam[CamID].m_FrameInfo.m_GrabSize = pDevice->GetGrabSize();
	  Cam[CamID].pConfig->SetConfigFilePath(preSettingPath);
      return 1;
  }
  int OFccmControl::ReadReg(int CamID,USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
  {
     return  Cam[CamID].pImgSensor->ReadReg(startAddr,endAddr,buf,page);
  }
  int OFccmControl::WriteReg(int CamID,USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
  {
     return Cam[CamID].pImgSensor->WriteReg(startAddr,endAddr,buf,page);
  }

  void OFccmControl::LEDPortInitia(CWnd *pPort)
  {
	  if (globalset.bLEDEnable)
	  {
		  if(m_LEDPort.InitPort(pPort,OFccmControl::globalset.m_com,19200,'N',8,1,EV_RXCHAR|EV_CTS,512)) 
		  {
			  if(m_LEDPort.StartMonitoring())
			  {
                  Ctrl_Addlog(CamID,_T("串口初始化成功！"),COLOR_BLUE,200);
			  }
			  else
                 Ctrl_Addlog(CamID,_T("串口初始化失败！"),COLOR_RED,200);
		  }
		  else
		  {
			  Ctrl_Addlog(CamID,_T("该串口被占用！"),COLOR_RED,200);
		  }
	  }
  }
  int GetMacAddress(CString &LanMAC)
  {
#define MAX_ADAPTER_NAME_LENGTH 256                                                                                  
#define MAX_ADAPTER_DESCRIPTION_LENGTH 128                                                                           
#define MAX_ADAPTER_ADDRESS_LENGTH 8                                                                                 
	  typedef struct _NET_IP_ADDRESS_STRING{                                                                               
		  char String[16];                                                                                                   
	  } NET_IP_ADDRESS_STRING, *PNET_IP_ADDRESS_STRING, NET_IP_MASK_STRING, *PNET_IP_MASK_STRING;                          
	  typedef struct _NET_IP_ADDR_STRING {                                                                                 
		  struct _NET_IP_ADDR_STRING* Next;                                                                                  
		  NET_IP_ADDRESS_STRING IpAddress;                                                                                   
		  NET_IP_MASK_STRING IpMask;                                                                                         
		  DWORD Context;                                                                                                     
	  } NET_IP_ADDR_STRING, *PNET_IP_ADDR_STRING;                                                                          
	  typedef struct _NET_IP_ADAPTER_INFO {                                                                                
		  struct _NET_IP_ADAPTER_INFO* Next;                                                                                 
		  DWORD ComboIndex;                                                                                                  
		  char AdapterName[MAX_ADAPTER_NAME_LENGTH+4];                                                                       
		  char Description[MAX_ADAPTER_DESCRIPTION_LENGTH+4];                                                                
		  UINT AddressLength;                                                                                                
		  BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];                                                                          
		  DWORD Index;                                                                                                       
		  UINT Type;                                                                                                         
		  UINT DhcpEnabled;                                                                                                  
		  PNET_IP_ADDR_STRING CurrentIpAddress;                                                                              
		  NET_IP_ADDR_STRING IpAddressList;                                                                                  
		  NET_IP_ADDR_STRING GatewayList;                                                                                    
		  NET_IP_ADDR_STRING DhcpServer;                                                                                     
		  BOOL HaveWins;                                                                                                     
		  NET_IP_ADDR_STRING PrimaryWinsServer;                                                                              
		  NET_IP_ADDR_STRING SecondaryWinsServer;                                                                            
		  time_t LeaseObtained;                                                                                              
		  time_t LeaseExpires;                                                                                               
	  } NET_IP_ADAPTER_INFO, *PNET_IP_ADAPTER_INFO;                                                                        

	  // LanMAC.RemoveAll();                                                                                                  

	  HINSTANCE hLib = 0;                                                                                                  
	  if ( hLib = LoadLibrary( _T("iphlpapi.dll") ) ) //加载动态库                                                         
	  {                                                                                                                    
		  DWORD  (WINAPI*pGetAdaptersInfo ) (PNET_IP_ADAPTER_INFO,PULONG) =                                                  
			  ( DWORD (WINAPI*) (PNET_IP_ADAPTER_INFO,PULONG) )GetProcAddress(hLib, "GetAdaptersInfo");//获取地址              
		  if ( pGetAdaptersInfo != 0  )                                                                                      
		  {                                                                                                                  
			  DWORD AdapterInfoSize = 0;                                                                                       
			  if( pGetAdaptersInfo(0, &AdapterInfoSize) == ERROR_BUFFER_OVERFLOW) //获取长度                                   
			  {                                                                                                                
				  PNET_IP_ADAPTER_INFO pAdapterInfo = (PNET_IP_ADAPTER_INFO) new BYTE [AdapterInfoSize]; //分配内存              

				  if ( pGetAdaptersInfo(pAdapterInfo,   &AdapterInfoSize) == ERROR_SUCCESS) //获取信息                           
				  {                                                                                                              
					  PNET_IP_ADAPTER_INFO pAdapter = pAdapterInfo;                                                                
					  while(pAdapter)                                                                                              
					  {                                                                                                            
						  CString strMac;                                                                                            
						  strMac.Format( _T("%02X-%02X-%02X-%02X-%02X-%02X"),                                                        
							  pAdapter->Address[0],pAdapter->Address[1],pAdapter->Address[2],                                          
							  pAdapter->Address[3],pAdapter->Address[4],pAdapter->Address[5]);                                         
						  LanMAC.Format(strMac);                                                                                        

						  pAdapter = pAdapter->Next; //下一个区域                                                                    
					  }                                                                                                            
				  }                                                                                                              

				  delete [] pAdapterInfo; //释放内存                                                                             
			  }                                                                                                                
		  }                                                                                                                  

		  FreeLibrary( hLib ); //释放库                                                                                      

	  }
	  return 1;
  }
  BOOL OFccmControl::UnlocktheFile(int CamID)
  {
	  CStringA filename;
	  char UpdateTime[512]={0};
	  char keyname[32]={0};
	  USES_CONVERSION;
	  CString strfile=globalset.GetCamConfigPath(CamID);
	  if(CamID==0)
	  {
		  sprintf(keyname,"Cam0File%d",GetSiteNo());
		  filename=W2A(strfile.GetBuffer());
	  }
	  else 
	  {
		  sprintf(keyname,"Cam1File%d",GetSiteNo());
		  filename=W2A(globalset.CAM1_SensorInitFile.GetBuffer());
	  }

	  GetFileMD5(filename.GetBuffer(filename.GetLength()),UpdateTime);
	  RegSetKey("F_Auto",keyname,UpdateTime);
	  return TRUE;

  }
  BOOL OFccmControl::Unlock(int CamID)
  {
	  char Buffer[512];
	  CStringA filename;
	  char UpdateTime[512]={0};
	  char keyname[32]={0};
	  USES_CONVERSION;
	  CString strfile=globalset.GetCamConfigPath(CamID);
	  if(CamID==0)
	   {
		   sprintf(keyname,"Cam0File%d",GetSiteNo());
		   filename=W2A(strfile.GetBuffer());
		   RegReadKey("F_Auto",keyname,Buffer);
	  }
	  else 
		  {
			  sprintf(keyname,"Cam1File%d",GetSiteNo());
			  filename=W2A(globalset.CAM1_SensorInitFile.GetBuffer());
			  RegReadKey("F_Auto",keyname,Buffer);
	  }

	  GetFileMD5(filename.GetBuffer(filename.GetLength()),UpdateTime);
	  
	  if ((strcmp(UpdateTime,Buffer)!=0))
	  {
// 		  CString TempBuffer(Buffer);
// 		  if (TempBuffer.IsEmpty())
// 		  {
// 			  RegSetKey("F_Auto",keyname,UpdateTime);
// 			  return TRUE;
// 		  }
// 		  
// 		  else 
		  if(!IsUSBlock())
		  {
			  AfxMessageBox(_T("请先用加密狗解锁再进入程式!"));
			  return FALSE;
		  }
		  else{
			  RegSetKey("F_Auto",keyname,UpdateTime);
			  return TRUE;
		  }
	  }
	  return TRUE;
  }
  BOOL OFccmControl::IsUSBlock() 
  {
	  char Temp[128]={0};
	  bool Res=GetUSBKeyID(Temp);
	  if (Res)
	  {
		return TRUE;
	  }
	  else
	  {
		  CString MacAddress=_T("1");
		  GetMacAddress(MacAddress);
		  CString Database[300]={_T("1C-39-47-EA-36-0E"),_T("84-27-EA-A2-5F-1F"),_T("AC-B5-7D-FB-15-32"),_T("E4-A7-A0-EF-06-FB"),_T("00-23-24-89-F6-82"),_T("44-37-E6-D3-E6-6A"),_T("00-23-24-46-55-82"),_T("DC-0E-A1-63-60-E7"),_T("F0-76-1C-1B-4C-31"),_T("AA-22-0B-0E-21-44"),
			  _T("C8-5B-76-38-C8-F6"),_T("D8-CB-8A-0D-D3-65"),_T("00-24-33-A2-FE-50"),_T("1C-39-47-EA-33-24"),_T("00-23-24-3B-47-03"),_T("00-23-24-84-81-64"),_T("D4-3D-7E-A0-D3-F2"),_T("04-7D-7B-18-20-09"),_T("04-7D-7B-18-20-09"),_T("1C-39-47-EA-37-82"),
			  _T("EC-A8-6B-C0-2B-59"),_T("00-23-24-E1-65-5D"),_T("F0-BF-97-1B-8D-E6"),_T("F8-0F-41-BE-9F-8E"),_T("F0-76-1C-52-A8-D8"),_T("00-23-24-E1-65-5A"),_T("74-27-EA-1E-61-6B"),_T("F0-76-1C-52-A7-E3"),_T("00-23-24-40-5C-FA"),_T("00-23-24-82-D0-DC"),
			  _T("C8-5B-76-38-C6-98"),_T("10-08-B1-DA-1C-99"),_T("00-23-24-E1-65-4D"),_T("DC-53-60-76-48-4C"),_T("C8-5B-76-38-CF-46"),_T("F8-A9-63-48-DD-21"),_T("00-23-24-83-19-E0"),_T("C8-5B-76-38-CC-17"),_T("F0-76-1C-52-A6-31"),_T("C8-5B-76-38-C8-D5"),//SW
			  _T("58-FB-84-A0-6A-36"),_T("02-28-F8-C9-2F-8E"),_T("00-28-F8-C9-2F-8F"),_T("B0-35-9F-C1-CB-13"),_T("54-E1-AD-D3-08-55"),_T("54-E1-AD-B1-83-63"),_T(""),_T(""),_T(""),_T(""),
			  _T("00-23-24-E1-64-88"),_T("00-23-24-83-E1-25"),_T("74-27-EA-1B-37-73"),_T("C0-3F-D5-0C-9F-FF"),_T("00-23-24-84-81-2B"),_T("00-23-24-E1-5C-69"),_T("EC-A8-6B-81-9A-72"),_T("EC-A8-6B-6F-D1-3B"),_T("D8-CB-8A-0D-D5-48"),_T("00-26-C6-DF-BC-88"),//TE
			  _T("C0-CB-38-3E-18-77"),_T("C0-3F-D5-35-C3-CA"),_T("08-3E-8E-66-89-E0"),_T("‎D8-CB-8A-0D-D5-9C"),_T("B0-83-FE-50-BA-10"),_T("EC-A8-6B-80-DF-38"),_T("BC-EE-7B-9D-BC-45"),_T("00-23-24-82-D0-45"),_T("EC-A8-6B-83-23-D6"),_T("C0-CB-38-3E-1A-21"),
			  _T("70-8B-CD-80-0B-EE"),_T("34-97-F6-55-FC-C6"),_T("70-8B-CD-80-0B-E2"),_T("70-8B-CD-80-0B-DB"),_T("70-8B-CD-80-0B-C7"),_T("34-97-F6-55-FC-A1"),_T("70-8B-CD-80-0B-E0"),_T("34-97-F6-55-E1-08"),_T("70-8B-CD-80-0B-A2"),_T("00-0B-AB-A8-62-25"),//AA机台
			  _T("1C-1B-0D-01-8B-03"),_T("1C-1B-0D-08-47-F9"),_T("1C-1B-0D-08-A7-2B"),_T("1C-1B-0D-08-A4-D7"),_T("00-0B-AB-A8-5A-10"),_T("70-8B-CD-80-0B-EE"),_T("00-0B-AB-A8-55-49"),_T("00-0B-AB-A8-62-25"),_T("70-8B-CD-80-0B-E4"),_T("70-8B-CD-80-0B-0E"),//AA机台
			  _T("70-8B-CD-80-0B-E0"),_T("34-97-F6-55-FC-C6"),_T("1C-1B-0D-08-48-16"),_T("1C-1B-0D-08-A7-44"),_T("1C-1B-0D-01-1D-FA"),_T("1C-1B-0D-01-1D-F4"),_T("1C-1B-0D-08-A7-4B"),_T("1C-1B-0D-08-48-07"),_T("1C-1B-0D-08-46-83"),_T("1C-1B-0D-08-A7-52"),//AA机台
			  _T("34-97-F6-99-CB-55"),_T("34-97-F6-9A-CC-51"),_T("34-97-F6-99-CB-6D"),_T("D0-17-C2-98-72-FF"),_T("34-97-F6-99-CB-02"),_T("34-97-F6-9A-CB-F8"),_T("34-97-F6-99-CB-8B"),_T("D0-17-C2-9A-D3-7E"),_T("34-97-F6-99-CB-57"),_T("34-97-F6-99-CC-E9"),//AA机台
			  _T("34-97-F6-99-CB-C0"),_T("34-97-F6-9A-CC-FF"),_T("34-97-F6-9A-CB-E8"),_T("D0-17-C2-9A-D2-AE"),_T("D0-17-C2-97-76-C7"),_T("D0-17-C2-97-76-66"),_T("D0-17-C2-9A-D3-DB"),_T("34-97-F6-99-CC-39"),_T("1C-1B-0D-08-A7-29"),_T("1C-1B-0D-08-A7-3B"),//AA机台
		      _T("70-4D-7B-63-22-8F"),_T("70-4D-7B-63-23-07"),_T("70-4D-7B-61-CB-39"),_T("70-4D-7B-63-1F-41"),_T("70-4D-7B-63-1F-82"),_T("70-4D-7B-63-25-03"),_T("70-4D-7B-63-20-49"),_T("70-4D-7B-63-20-6F"),_T("70-4D-7B-63-21-11"),_T("70-4D-7B-63-22-F7"),
			  _T("D0-17-C2-98-72-90"),_T("D0-17-C2-98-72-EF"),_T("34-97-F6-9A-CB-E1"),_T("34-97-F6-9A-CC-71"),_T("34-97-F6-55-E1-08"),_T("70-8B-CD-80-0B-A2"),_T("34-97-F6-55-FC-A1"),_T("70-8B-CD-80-0B-E0"),_T("70-8B-CD-80-0B-C7"),_T("70-8B-CD-80-0B-96"),
			  _T("70-8B-CD-80-0B-E2"),_T("70-8B-CD-80-0B-DB"),_T("70-8B-CD-80-0B-EE"),_T("34-97-F6-55-FC-C6"),_T("70-8B-CD-80-0B-E4"),_T("70-8B-CD-80-0B-0E"),_T("00-0B-AB-A8-55-1B"),_T("00-0B-AB-A8-55-47"),_T("F0-76-1C-1B-4C-31"),_T("AC-B5-7D-FC-F0-41"),//培训中心AA
			  _T("B0-35-9F-C0-8A-0F"), _T("54-E1-AD-B3-B2-9E")
		  } ;
		  int i=0;
		  for(i=0;i<300;i++)
		  {
			  if(!Database[i].Compare(MacAddress))
				  break;
		  }
		  if(i>=300)
		  {
			  return FALSE;
		  }
	  }
	  return TRUE;
  }
  void HexStringFilter(CStringA &str)
  {
	  BOOL bOK;
	  for(int i=0;i<str.GetLength();)
	  {
		  bOK=((str.GetAt(i)>='0')&&(str.GetAt(i)<='9'))||
			  ((str.GetAt(i)>='A')&&(str.GetAt(i)<='F'))||
			  ((str.GetAt(i)>='a')&&(str.GetAt(i)<='f'));
		  if(!bOK)
			  str.Delete(i);
		  else i++;	
	  }
  }
  char CombineHexChar(char CharH,char CharL)
  {
	  char result;
	  CString temp;
	  if(CharH>='0'&&CharH<='9')			result=(CharH-'0');
	  else if(CharH>='a'&&CharH<='f')		result=(CharH-'a'+10);
	  else if(CharH>='A'&&CharH<='F')		result=(CharH-'A'+10);
	  else								result=0;
	  result<<=4;	
	  if(CharL>='0'&&CharL<='9')			result+=(CharL-'0');
	  else if(CharL>='a'&&CharL<='f')		result+=(CharL-'a'+10);
	  else if(CharL>='A'&&CharL<='F')		result+=(CharL-'A'+10);
	  else								result+=0;
	  return result;
  }

  CStringA ChangeCharstr2Hexstr(CStringA Charstr)
  {
	  CStringA Hexstr="";
	  Charstr.MakeUpper();
	  HexStringFilter(Charstr);
	  int Length=Charstr.GetLength();
	  if(Length%2)
		  Charstr.Delete(Length-1);
	  Length=Charstr.GetLength();
	  for(int i=0;i<Length/2;i++)
	  {
		  Hexstr+=CombineHexChar(Charstr.GetAt(i*2),Charstr.GetAt(i*2+1));
	  }
	  return Hexstr;
  }
  void  OFccmControl::ReturnSetLEDG4C(int CamID,int recLed)
  {
	  Cam[CamID].bLEDG4C_OK=recLed;
	  if (Ctrl_GetCamType()==1)
	  {
		  Cam[0].bLEDG4C_OK=recLed;
		  Cam[1].bLEDG4C_OK=recLed;
	  }
  }
  BOOL  OFccmControl::SetLEDChannel(int CamID,int channel,int type,int WaitFlag)
  {
	  CStringA LEDcmd;
	  CString strTemp;
	   Cam[CamID].bLEDG4C_OK=FALSE;
	  if ((WaitFlag>0)&&(Ctrl_GetCamType()==1))
	  {
		  Cam[CamID].CurrentChannel=channel;
		  int CammeraID=0;
		  if(CamID==0) CammeraID=1;
		  else CammeraID=0;

		  int Count=0;
		  while(channel!=Cam[CammeraID].CurrentChannel)
		  {
			  if(Ctrl_GetTestItemID(CammeraID)==-1)
			  {
				  Count=WaitFlag;
				  break;
			  }
			  Sleep(200);
			  strTemp.Format(_T("Wait..Count=%d"),Count);
			  Ctrl_Addlog(CamID,strTemp,COLOR_BLUE,200);
			  Count++;
			  if(Count>=WaitFlag) break;
		  }
	  }
	  if (type==1)//G4C,以下顺序固定死
	  {
		  CString strTemp;
		  if(Cam[CamID].pConfig->GetGeneralTab()->SocketEn)
		  {
			  if (channel==0)
			  {
				  strTemp.Format(_T("StartTestEn"));
				  Cam[CamID].camSocket->SendInfo(strTemp);
			  }
			  if (channel==1)
			  {
				  strTemp.Format(_T("CCT_A"));
				  Cam[CamID].camSocket->SendInfo(strTemp);
			  }
			  if (channel==2)
			  {
				  strTemp.Format(_T("CCT_TL84"));
				  Cam[CamID].camSocket->SendInfo(strTemp);
			  }
			  if (channel==3)
			  {
				  strTemp.Format(_T("CCT_D65"));
				  Cam[CamID].camSocket->SendInfo(strTemp);
			  }
		  }
		  
		  int SleepTime=0;
          while((SleepTime<100)&&(Cam[CamID].bLEDG4C_OK==FALSE))
		  {
			  Sleep(200);
			  strTemp.Format(_T("Wait..bLEDG4C_OK=%d"),Cam[CamID].bLEDG4C_OK);
			  Ctrl_Addlog(CamID,strTemp,COLOR_BLUE,200);
			  SleepTime++;
		  }
		  Cam[CamID].bLEDG4C_OK=FALSE;

	  }
	  if (type==0)//G3C
	  {
		  if (channel == 1)
		  {
			  LEDcmd.Format("4C4700000093");      //channel 1 on D65
		  } 
		  else if(channel == 2)
		  {
			  LEDcmd.Format("4C4701000094");      //channel 2 on  A
		  }
		  else if (channel == 3)
		  {
			  LEDcmd.Format("4C4702000095");      //channel 3 on TL84
		  }
		  else if (channel == 4)
		  {
			  LEDcmd.Format("4C4703000096");      //channel 4 on
		  }
		  else if (channel == 5)
		  {
			  LEDcmd.Format("4C4704000097");      //channel 5 on
		  }
		  else
		  {
			  return FALSE;
		  }

		  LEDcmd = ChangeCharstr2Hexstr(LEDcmd);
		  m_LEDPort.WriteToPort(LEDcmd.GetBuffer(LEDcmd.GetLength()),LEDcmd.GetLength());
	  }
	  Cam[CamID].CurrentChannel=channel;
	  Sleep(100);
	  return TRUE;
  }

  BOOL OFccmControl::IncludeChinese(CString str)
  {
	  int len = str.GetLength();
	  char *_str = NULL;//new char[5*len + 1];
	  
//	  if (NULL == _str)
//	  {
//		  return 1;
//	  }

//	  memset(_str,0,5*len+1);

	  USES_CONVERSION;
	  _str = T2A(str.GetBuffer(str.GetLength()));
	  str.ReleaseBuffer();

	  int nRet = 0;
	  char c;
	  while(c = *_str++)
	  {
		  if ((c&0x80) && (*_str & 0x80))
		  {
			  nRet = 1;
			  break;
		  }
	  }
//	  delete [] _str;
//	  _str = NULL;

	  return nRet;
  }


  void OFccmControl::SetFocusStart(int CamID)
  {
	  Cam[CamID].bFocusEnd = FALSE;
	  Cam[CamID].bFocusStatusChange = TRUE;
  }
