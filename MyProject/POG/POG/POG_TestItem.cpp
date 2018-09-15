#include "StdAfx.h"
#include "POG_TestItem.h"
#include "ImageProc.h"


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new POG_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
POG_TestItem::POG_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

POG_TestItem::~POG_TestItem(void)
{

}


int POG_TestItem::InitItem()
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
int POG_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
 
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int POG_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	CString tmpStr = _T("");
	img = * m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.RGB24_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	memset(&m_blemishInfo, 0, sizeof(BlemishInfo));

	HINSTANCE hInstance = LoadLibrary(_T("OfBlemishAlgorithm.dll"));
	if (hInstance == NULL)
	{
		FreeLibrary(hInstance);
		m_pInterface->Ctrl_Addlog(CamID,_T("Load OfBlemishAlgorithm.dll 失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	typedef int (*pogTest)(BYTE *SrcBMPBuffer,int iWidth, int iHeight,BlemishInfo *BlemishRes,CamSeletForPOG Cam);
	pogTest fnPogTest = (pogTest)GetProcAddress(hInstance, "fnBlemishAlgorithm");
	if (fnPogTest != NULL)
	{
		CString TempConfig=m_pInterface->Config_GetConfigPathName(CamID);
		CStringA PODConfig,PODKeyName;
		//wcstombs(PODConfig,TempConfig,MAX_COLUMN);
		PODConfig=CT2A(TempConfig);
		PODKeyName=CT2A(m_cfgName);

		int result = 0;
		if (CamID == 0)
			result = fnPogTest(img.RGB24_buffer, img.imgwidth, img.imgheight, &m_blemishInfo,POG_Cam0);
		else
			result = fnPogTest(img.RGB24_buffer, img.imgwidth, img.imgheight, &m_blemishInfo,POG_Cam1);

		if (result == -1)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("内存申请失败,POG TEST FAILED"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG TEST FAILED !!"));
			SetResult(RESULT_FAIL);
		}
		else
		{
			if (result == 1)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("POG TEST PASS"),COLOR_GREEN,200);
				SetResult(RESULT_PASS);
			}
			else
			{
				ShowInfo(m_blemishInfo);
				m_pInterface->Ctrl_Addlog(CamID,_T("POG TEST FAILED"),COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG TEST FAILED !!"));
				SetResult(RESULT_FAIL);
			}
		}		
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("POG DLL LOAD FAILED !!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG DLL LOAD FAILED !!"));
		SetResult(RESULT_FAIL);
	}

	FreeLibrary(hInstance);

   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int POG_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int POG_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码
 	SetConfigPath(GetModulePath() + "\\OfblemishTest.ini");
 	SetName(_T("Spec"));

	pDlg->m_pogCamera0saveImage = ReadConfigInt(_T("bSaveImage"), 0);
	pDlg->m_pogCamera0saveLog = ReadConfigInt(_T("bSaveLog"), 0);

	SetName(_T("Spec_Cam0"));
	pDlg->m_pogCamera0medianA = ReadConfigString(_T("MedianA"), _T("25"));
	pDlg->m_pogCamera0medianB = ReadConfigString(_T("MedianB"), _T(""));
	pDlg->m_pogCamera0axStart = ReadConfigString(_T("ZoneAXStart"), _T(""));
	pDlg->m_pogCamera0ayStart = ReadConfigString(_T("ZoneAYStart"), _T(""));
	pDlg->m_pogCamera0offset = ReadConfigString(_T("offset"), _T(""));
	pDlg->m_pogCamera0cornerDiff = ReadConfigString(_T("CornerDiff"), _T(""));
	pDlg->m_pogCamera0edgeLimit = ReadConfigString(_T("EdgeLimit"), _T(""));
	pDlg->m_pogCamera0threshold = ReadConfigString(_T("Threshold"), _T(""));
	pDlg->m_pogCamera0pixelLimit = ReadConfigString(_T("PixelLimit"), _T(""));
	pDlg->m_pogCamera0axEnd = ReadConfigString(_T("ZoneAXEnd"), _T(""));
	pDlg->m_pogCamera0ayEnd = ReadConfigString(_T("ZoneAYEnd"), _T(""));
	pDlg->m_pogCamera0intensity = ReadConfigString(_T("IntensityCntSpc"), _T(""));
	pDlg->m_pogCamera0rangeSpc = ReadConfigString(_T("RangeSpc"), _T(""));
	pDlg->m_pogCamera0edgePixelLimit = ReadConfigString(_T("EdgePixelLimit"), _T(""));

	SetName(_T("Spec_Cam1"));
	pDlg->m_pogCamera1medianA = ReadConfigString(_T("MedianA"), _T("25"));
	pDlg->m_pogCamera1medianB = ReadConfigString(_T("MedianB"), _T(""));
	pDlg->m_pogCamera1axStart = ReadConfigString(_T("ZoneAXStart"), _T(""));
	pDlg->m_pogCamera1ayStart = ReadConfigString(_T("ZoneAYStart"), _T(""));
	pDlg->m_pogCamera1offset = ReadConfigString(_T("offset"), _T(""));
	pDlg->m_pogCamera1cornerDiff = ReadConfigString(_T("CornerDiff"), _T(""));
	pDlg->m_pogCamera1edgeLimit = ReadConfigString(_T("EdgeLimit"), _T(""));
	pDlg->m_pogCamera1threshold = ReadConfigString(_T("Threshold"), _T(""));
	pDlg->m_pogCamera1pixelLimit = ReadConfigString(_T("PixelLimit"), _T(""));
	pDlg->m_pogCamera1axEnd = ReadConfigString(_T("ZoneAXEnd"), _T(""));
	pDlg->m_pogCamera1ayEnd = ReadConfigString(_T("ZoneAYEnd"), _T(""));
	pDlg->m_pogCamera1intensity = ReadConfigString(_T("IntensityCntSpc"), _T(""));
	pDlg->m_pogCamera1rangeSpc = ReadConfigString(_T("RangeSpc"), _T(""));
	pDlg->m_pogCamera1edgePixelLimit = ReadConfigString(_T("EdgePixelLimit"), _T(""));

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(m_cfgName);

	pDlg->m_pogCamera0Enable = ReadConfigInt(_T("POG_CAMERA0_ENABLE"), 0);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int POG_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码
	SetConfigPath(GetModulePath() + "\\OfblemishTest.ini");
	SetName(_T("Spec"));

	WriteConfigInt(_T("bSaveImage"), pDlg->m_pogCamera0saveImage, 0);
	WriteConfigInt(_T("bSaveLog"), pDlg->m_pogCamera0saveLog, 0);

	SetName(_T("Spec_Cam0"));
	WriteConfigString(_T("MedianA"), pDlg->m_pogCamera0medianA);
	WriteConfigString(_T("MedianB"), pDlg->m_pogCamera0medianB);
	WriteConfigString(_T("ZoneAXStart"), pDlg->m_pogCamera0axStart);
	WriteConfigString(_T("ZoneAYStart"), pDlg->m_pogCamera0ayStart);
	WriteConfigString(_T("offset"), pDlg->m_pogCamera0offset);
	WriteConfigString(_T("CornerDiff"), pDlg->m_pogCamera0cornerDiff);
	WriteConfigString(_T("EdgeLimit"), pDlg->m_pogCamera0edgeLimit);
	WriteConfigString(_T("Threshold"), pDlg->m_pogCamera0threshold);
	WriteConfigString(_T("PixelLimit"), pDlg->m_pogCamera0pixelLimit);
	WriteConfigString(_T("ZoneAXEnd"), pDlg->m_pogCamera0axEnd);
	WriteConfigString(_T("ZoneAYEnd"), pDlg->m_pogCamera0ayEnd);
	WriteConfigString(_T("IntensityCntSpc"), pDlg->m_pogCamera0intensity);
	WriteConfigString(_T("RangeSpc"), pDlg->m_pogCamera0rangeSpc);
	WriteConfigString(_T("EdgePixelLimit"), pDlg->m_pogCamera0edgePixelLimit);

	SetName(_T("Spec_Cam1"));
	WriteConfigString(_T("MedianA"), pDlg->m_pogCamera1medianA);
	WriteConfigString(_T("MedianB"), pDlg->m_pogCamera1medianB);
	WriteConfigString(_T("ZoneAXStart"), pDlg->m_pogCamera1axStart);
	WriteConfigString(_T("ZoneAYStart"), pDlg->m_pogCamera1ayStart);
	WriteConfigString(_T("offset"), pDlg->m_pogCamera1offset);
	WriteConfigString(_T("CornerDiff"), pDlg->m_pogCamera1cornerDiff);
	WriteConfigString(_T("EdgeLimit"), pDlg->m_pogCamera1edgeLimit);
	WriteConfigString(_T("Threshold"), pDlg->m_pogCamera1threshold);
	WriteConfigString(_T("PixelLimit"), pDlg->m_pogCamera1pixelLimit);
	WriteConfigString(_T("ZoneAXEnd"), pDlg->m_pogCamera1axEnd);
	WriteConfigString(_T("ZoneAYEnd"), pDlg->m_pogCamera1ayEnd);
	WriteConfigString(_T("IntensityCntSpc"), pDlg->m_pogCamera1intensity);
	WriteConfigString(_T("RangeSpc"), pDlg->m_pogCamera1rangeSpc);
	WriteConfigString(_T("EdgePixelLimit"), pDlg->m_pogCamera1edgePixelLimit);

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(m_cfgName);

	WriteConfigInt(_T("POG_CAMERA0_ENABLE"), pDlg->m_pogCamera0Enable);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR POG_TestItem::GetReportHeads()
{
	static CString header;

	header = _T("POG COUNT");

	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR POG_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("0");
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR POG_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("0");
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR POG_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");

	CString temp = _T("");
	temp.Format(_T("%d"), m_blemishInfo.POGCount);

	Content += temp;

	return Content;
}

CString POG_TestItem::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	return ret;
}

void POG_TestItem::ShowInfo(BlemishInfo blemishInfo)
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

	COLORREF CurrentColor = COLOR_YELLOW;

	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	//if (bParallel)
	{
		for (UINT i=0; i<blemishInfo.POGCount; i++)
		{
			CurrentColor = COLOR_RED;

			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			int cx1,cy1, cx2, cy2;
			cx1 = blemishInfo.xStart[i];
			cy1 = blemishInfo.yStart[i];
			int px1 = (int)(cx1* Xratio);
			int py1 = (int)(cy1* Yratio);

			cx2 = blemishInfo.xEnd[i];
			cy2 = blemishInfo.yEnd[i];
			int px2 = (int)(cx2* Xratio);
			int py2 = (int)(cy2* Yratio);
			BOOL bShowCen = TRUE;
			if(bShowCen)
				pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		}
	} 
	m_pInterface->Ctrl_ShowMemDC(CamID);
}