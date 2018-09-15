#include "StdAfx.h"
#include "RedSpot_TestItem.h"
#include "ImageProc.h"
#include "RedSpot.h"


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */

void nCheckBlockAreaSetting(LONG& x0, 
	LONG& y0, 
	UINT  width, 
	UINT  height, 
	UINT  ImageWidth, 
	UINT  ImageHeight);

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new RedSpot_TestItem(pInterface, lpszName);
}

/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
RedSpot_TestItem::RedSpot_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	m_cSensorSlaveId = 0;
	m_cSensorMode = 0;
	m_iFrmDelayCnt = 0;
	//.....
}

RedSpot_TestItem::~RedSpot_TestItem(void)
{

}


int RedSpot_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	

	
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int RedSpot_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	m_iFrmDelayCnt = 0;

	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int RedSpot_TestItem::Testing()
{
	if (m_iFrmDelayCnt == 0)
	{
		ManualExposure(); 
		m_iFrmDelayCnt++;
		return 1;
	}
	else if (m_iFrmDelayCnt < pDlg->iManualExDelayFrame)
	{
		Sleep(50);
		m_iFrmDelayCnt++;
		return 1;
	}

	m_pInterface->Ctrl_Addlog(CamID, _T("RedSpot Testing..."), COLOR_BLUE,200);

	CSize ViewSize;
	CString strTemp;
	CTime t;

	ImageInfo *pImg = m_pInterface->Ctrl_GetImgInfo(CamID);
	UINT height = pImg->imgheight;
	UINT width = pImg->imgwidth;
	int outputFormat = pImg->RawFormat;


	CSize imgSize;
	imgSize.cx=width;
	imgSize.cy=height;


	CSize  s0 ;
	CPoint p0 ;



	pImg = m_pInterface->Ctrl_GetImgInfo(CamID);

	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);
	ViewSize.cx=DisplayWndRect.right-DisplayWndRect.left;
	ViewSize.cy=DisplayWndRect.bottom-DisplayWndRect.top;

	RedSpotY m_RedSpotYrgb;
	int iRet = RedSpotTest(pImg->RGB24_buffer,width,height,pDlg->XBlockNum,pDlg->YBlockNum,&m_RedSpotYrgb);
	s0.cx=width/pDlg->XBlockNum;
	s0.cy=height/pDlg->YBlockNum;

	


	if (m_RedSpotYrgb.OutputYrgb.Y > pDlg->Redspot_Ymax||m_RedSpotYrgb.OutputYrgb.R > pDlg->Redspot_Rmax||
		m_RedSpotYrgb.OutputYrgb.G > pDlg->Redspot_Gmax||m_RedSpotYrgb.OutputYrgb.B > pDlg->Redspot_Bmax)
	{
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("RedSpot 测试失败 !!"));
	}
	else
	{
		SetResult(RESULT_PASS);
	}

	CDC *pMemDc = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);


	for(int i=0;i<4;i++)
	{
		p0.x=(m_RedSpotYrgb.YrgbPoint[i].x*s0.cx+s0.cx/2)*100/width;
		p0.y=(m_RedSpotYrgb.YrgbPoint[i].y*s0.cy+s0.cy/2)*100/height;

		strTemp.Format(_T("Y=%.0f R=%.0f G=%.0f B=%.0f"),m_RedSpotYrgb.OutputYrgb.Y,m_RedSpotYrgb.OutputYrgb.R,m_RedSpotYrgb.OutputYrgb.G,m_RedSpotYrgb.OutputYrgb.B);


		nDrawBlockRect(pMemDc,
			p0,
			imgSize,
			s0,
			ViewSize,
			strTemp,
			GetResult());
		m_pInterface->Ctrl_ShowMemDC(CamID);
		Sleep(200);

	}
					



	return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int RedSpot_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int RedSpot_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->Redspot_Ymax = ReadConfigInt(_T("Redspot_Ymax"),18);
	pDlg->Redspot_Rmax = ReadConfigInt(_T("Redspot_Rmax"),18);
	pDlg->Redspot_Gmax = ReadConfigInt(_T("Redspot_Gmax"),18);
	pDlg->Redspot_Bmax = ReadConfigInt(_T("Redspot_Bmax"),18);

	pDlg->XBlockNum    = ReadConfigInt(_T("XBlockNum"),10);
	pDlg->YBlockNum    = ReadConfigInt(_T("YBlockNum"),10);

	pDlg->bManualExposure = ReadConfigInt(_T("RedspotChartExposureEn"), 1 );
	pDlg->strExpVal = ReadConfigString(_T("RedspotChartExposure"), _T("0x0cB8") );
	pDlg->strGainVal = ReadConfigString(_T("RedspotChartGain"), _T("0x0020") );

	pDlg->strExposureRegHig =  ReadConfigString( _T("ExposureRegHig"), _T("0"));
	pDlg->strExposureRegLow = ReadConfigString(_T("ExposureRegLow"), _T("0")); 

	pDlg->strGainRegHig =  ReadConfigString( _T("GainRegHig"), _T("0") );
	pDlg->strGainRegLow = ReadConfigString( _T("GainRegLow"), _T("0")); 

	pDlg->iManualExDelayFrame = ReadConfigInt(_T("ManualExDelayFrame"), 3);


	m_cSensorSlaveId = GetPrivateProfileInt(_T("Sensor"), _T("SlaveID"), 0, m_pInterface->Config_GetConfigPathName(CamID));
	m_cSensorMode = GetPrivateProfileInt(_T("Sensor"), _T("mode"), 0, m_pInterface->Config_GetConfigPathName(CamID));
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int RedSpot_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	  
	//........................

	//TODO: 在此添加读取代码 


	WriteConfigInt(_T("Redspot_Ymax"),pDlg->Redspot_Ymax);
	WriteConfigInt(_T("Redspot_Rmax"),pDlg->Redspot_Rmax);
	WriteConfigInt(_T("Redspot_Gmax"),pDlg->Redspot_Gmax);
	WriteConfigInt(_T("Redspot_Bmax"),pDlg->Redspot_Bmax);

	WriteConfigInt(_T("XBlockNum"),pDlg->XBlockNum);
	WriteConfigInt(_T("YBlockNum"),pDlg->YBlockNum);

	WriteConfigInt(_T("RedspotChartExposureEn"), pDlg->bManualExposure );
	WriteConfigString(_T("RedspotChartExposure"), pDlg->strExpVal );
	WriteConfigString(_T("RedspotChartGain"), pDlg->strGainVal );

	WriteConfigString( _T("ExposureRegHig"), pDlg->strExposureRegHig );
	WriteConfigString(_T("ExposureRegLow"), pDlg->strExposureRegLow); 

	WriteConfigString( _T("GainRegHig"), pDlg->strGainRegHig );
	WriteConfigString( _T("GainRegLow"), pDlg->strGainRegLow); 

	WriteConfigInt(_T("ManualExDelayFrame"), pDlg->iManualExDelayFrame );
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR RedSpot_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header = _T("brightness");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR RedSpot_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR RedSpot_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR RedSpot_TestItem::GetReportContents()
{
	static CString Content;
	return Content;
}





BOOL RedSpot_TestItem::ManualExposure()
{
	if(pDlg->bManualExposure)
	{
		CString strLog;
		strLog.Format(_T("RedSpot Manual Exposure. ExpValue:%s,GainVal:%s"), pDlg->strExpVal, pDlg->strGainVal);
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_BLUE,200);
		int iExposureValue = 0;
		if (!swscanf(pDlg->strExpVal, _T("0x%x"), &iExposureValue))
			swscanf(pDlg->strExpVal, _T("%d"), &iExposureValue);

		int iGainValue = 0;
		if (!swscanf(pDlg->strGainVal, _T("0x%x"), &iGainValue))
			swscanf(pDlg->strGainVal, _T("%d"), &iGainValue);

		int iExposureRegHigh = 0;
		int iExposureRegLow = 0;
		int iGainRegHigh = 0;
		int iGainRegLow = 0;


		if (!swscanf(pDlg->strExposureRegHig, _T("0x%x"), &iExposureRegHigh))
			swscanf(pDlg->strExposureRegHig, _T("%d"), &iExposureRegHigh);

		if (!swscanf(pDlg->strExposureRegLow, _T("0x%x"), &iExposureRegLow))
			swscanf(pDlg->strExposureRegLow, _T("%d"), &iExposureRegLow);

		if (!swscanf(pDlg->strGainRegHig, _T("0x%x"), &iGainRegHigh))
			swscanf(pDlg->strGainRegHig, _T("%d"), &iGainRegHigh);

		if (!swscanf(pDlg->strGainRegLow, _T("0x%x"), &iGainRegLow))
			swscanf(pDlg->strGainRegLow, _T("%d"), &iGainRegLow);


		if (m_cSensorMode==4)
		{
			
			m_pInterface->Device_WriteI2c(m_cSensorSlaveId, iExposureRegHigh, iExposureValue, m_cSensorMode, CamID);
			//m_pInterface->Device_WriteI2c( CurrentSensor.SlaveID, ExposureRegLow, ExposureValueLow, CurrentSensor.mode);

			m_pInterface->Device_WriteI2c(m_cSensorSlaveId, iGainRegHigh, iGainValue, m_cSensorMode, CamID);
			//  WriteSensorReg( CurrentSensor.SlaveID, GainRegLow, GainValueLow, CurrentSensor.mode);
		}
		else
		{
			int iExposureValueHigh = iExposureValue>>8;
			int iExposureValueLow = iExposureValue%256;

			int iGainValueHigh = iGainValue>>8;
			int iGainValueLow = iGainValue%256;


			m_pInterface->Device_WriteI2c(m_cSensorSlaveId, iExposureRegHigh, iExposureValueHigh, m_cSensorMode, CamID);
			m_pInterface->Device_WriteI2c(m_cSensorSlaveId, iExposureRegLow, iExposureValueLow, m_cSensorMode, CamID);
			m_pInterface->Device_WriteI2c(m_cSensorSlaveId, iGainRegHigh, iGainValueHigh, m_cSensorMode, CamID);
			m_pInterface->Device_WriteI2c(m_cSensorSlaveId, iGainRegLow, iGainValueLow, m_cSensorMode, CamID);
		}
		Sleep(10);
	}
	
	
	return TRUE; 
}


//width & height显示的方框（ROI）的大小   x ,y 为百分数，showWidth & showHeight viewWindow.size,
//实现了缩放功能，
//text为在框下面显示的文本
//textUpdown TRUE -->up ,default false --->down
//for draw box in video
void RedSpot_TestItem::nDrawBlockRect(CDC *pDC, 
	CPoint StartPos,
	CSize ImgSize,
	CSize BlockSize,
	CSize ViewWNDSize,
	CString Text,
	int ntestResult ,BOOL textUpDown)
{
	int ImageWidth  = ImgSize.cx;
	int ImageHeight = ImgSize.cy;
	int ShowWidth=ViewWNDSize.cx;
	int ShowHeight=ViewWNDSize.cy;
	int x=StartPos.x;
	int y=StartPos.y;
	int TextLengh   = Text.GetLength();
	CSize  TextExtentSize=pDC->GetTextExtent(Text);
	LONG x0 = x * ImageWidth  / 100;
	LONG y0 = y * ImageHeight / 100;
	int  nBlockWidth;
	int nBlockHeight;
	nCheckBlockAreaSetting(x0, y0, BlockSize.cx, BlockSize.cy, ImageWidth, ImageHeight);

	x0     = x0 * ShowWidth  / ImageWidth;   // left
	y0     = y0 * ShowHeight / ImageHeight;  // top
	nBlockWidth  = BlockSize.cx  * ShowWidth  / ImageWidth;
	nBlockHeight = BlockSize.cy * ShowHeight / ImageHeight;	
	CPen*    pPenRed   = new CPen;
	CPen*    pPenOK2 = new CPen;
	CPen*    pPenBlue  = new CPen;

	pPenRed  ->CreatePen(PS_SOLID, 5,RGB(255,0,0));
	pPenOK2->CreatePen(PS_SOLID, 5, RGB(255,255,0));
	pPenBlue ->CreatePen(PS_SOLID, 5, RGB(0, 0, 255));



	CPen  * pPenOld=NULL;
	if (ntestResult == RESULT_FAIL)
	{
		pPenOld = pDC->SelectObject(pPenRed);
	}else if(ntestResult == RESULT_OK2)
	{
		pPenOld = pDC->SelectObject(pPenOK2); 
	}
	else
	{ 	
		pPenOld = pDC->SelectObject(pPenBlue);
	}

	nDrawRectLine(pDC, x0, y0, nBlockWidth, nBlockHeight);

	pDC->SelectObject(pPenOld);

	if(pPenBlue) delete pPenBlue;
	if(pPenOK2) delete pPenOK2;
	if(pPenRed) delete pPenRed;
	if (TextLengh)
	{
		if(!textUpDown)
		{

			y0=y0+nBlockHeight-TextExtentSize.cy;
		}

		if (x0 >= (LONG)(ShowWidth -TextExtentSize.cx))
		{

			x0 = (LONG)(ShowWidth -TextExtentSize.cx) ;

		}

		if (y0 >= (LONG)(ShowHeight - TextExtentSize.cy))
		{
			y0 =  (LONG)(ShowHeight - TextExtentSize.cy) ;
		}

		if (ntestResult == RESULT_FAIL)
		{
			pDC->SetTextColor(RGB(255,0,0));
		}else if(ntestResult == RESULT_OK2)
		{
			pDC->SetTextColor(RGB(255,255,0));
		}
		else
		{
			pDC->SetTextColor(RGB(0,0,255));
		}

		pDC->TextOut(x0, y0 , Text, TextLengh);
	}
}



//draw box in video
void  RedSpot_TestItem::nDrawRectLine(CDC *pDC, int x, int y, UINT width, UINT height)
{
	int x1 = x + width;
	int y1 = y + height;

	pDC->MoveTo(x,  y);
	pDC->LineTo(x1, y);
	pDC->LineTo(x1, y1);
	pDC->LineTo(x,  y1);
	pDC->LineTo(x,  y);
}