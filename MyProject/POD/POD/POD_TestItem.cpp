#include "StdAfx.h"
#include "POD_TestItem.h"
#include "ImageProc.h"


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new POD_TestItem(pInterface, lpszName);
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

/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
POD_TestItem::POD_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

POD_TestItem::~POD_TestItem(void)
{

}


int POD_TestItem::InitItem()
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
int POD_TestItem::Initialize()
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
int POD_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	CString tmpStr = _T("");
	img = * m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	memset(&m_podDefectStack, 0, sizeof(DefectStack));

	m_podDefectStack.DfcStk = new OFPixInfo[1000];
	if (NULL == m_podDefectStack.DfcStk)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("m_podDefectStack.DfcStk 申请失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	memset(m_podDefectStack.DfcStk,0,sizeof(OFPixInfo)*1000);	
	
	HINSTANCE hInstance = LoadLibrary(_T("ParticleAlgorithm.dll"));
	if (hInstance == NULL)
	{
		SAFE_DELETE_ARRAY(m_podDefectStack.DfcStk);
		FreeLibrary(hInstance);
		m_pInterface->Ctrl_Addlog(CamID,_T("POD DLL LOAD FAILED !!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
		return 0;
	}
		
	typedef int (*podTest)(BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,DefectStack* DfcStk,CamSelectForPOD Cam);
	podTest fnPodTest = (podTest)GetProcAddress(hInstance, "OFPODTest");
	if (fnPodTest != NULL)
	{
		USHORT* raw8 = NULL;
		if (pDlg->m_podCamera0Enable)
		{
			tmpStr.Format(_T("PixelOrder = %d"),m_PixelOrder);
			m_pInterface->Ctrl_Addlog(CamID,tmpStr,COLOR_BLUE,200);
			raw8 = new USHORT[img.imgwidth * img.imgheight];
			if (NULL == raw8)
			{
				FreeLibrary(hInstance);
				SAFE_DELETE_ARRAY(m_podDefectStack.DfcStk);
				m_pInterface->Ctrl_Addlog(CamID,_T("raw8 buffer 申请失败!"),COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD TEST FAILED!"));
				SetResult(RESULT_FAIL);
				return 0;
			}
			if (m_PixelOrder == 0)
			{
				ImageProc::RGB24toRaw_RGGB(img.RGB24_buffer, raw8, img.imgwidth, img.imgheight, 8);
			}
			else if (m_PixelOrder == 1)
			{
				ImageProc::RGB24toRaw_GRBG(img.RGB24_buffer, raw8, img.imgwidth, img.imgheight, 8);
			}
			else if (m_PixelOrder == 2)
			{
				ImageProc::RGB24toRaw_GBRG(img.RGB24_buffer, raw8, img.imgwidth, img.imgheight, 8);
			}
			else
			{
				ImageProc::RGB24toRaw_BGGR(img.RGB24_buffer, raw8, img.imgwidth, img.imgheight, 8);
			}
		}
		
		BYTE* raw8Bytes = new BYTE[img.imgwidth * img.imgheight];
		if (NULL == raw8Bytes)
		{
			FreeLibrary(hInstance);
			SAFE_DELETE_ARRAY(m_podDefectStack.DfcStk);
			SAFE_DELETE_ARRAY(raw8);
			m_pInterface->Ctrl_Addlog(CamID,_T("raw8Bytes buffer申请失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD TEST FAILED"));
			SetResult(RESULT_FAIL);
			return 0;
		}
		if (pDlg->m_podCamera0Enable)
		{
			for (int i=0; i<img.imgwidth * img.imgheight; i++)
				raw8Bytes[i] = (BYTE)raw8[i];
		}

		if (pDlg->m_podCamera0Enable == FALSE)
		{
			for (int y =0; y < img.imgheight; y++)
			{
				for(int x =0; x < img.imgwidth; x++)
					raw8Bytes[y*img.imgwidth + x] = (img.Raw_buffer[y*img.imgwidth + x] >>2 ) & 0xff;
			}
		}

		int result = 0;
		
		if (CamID == 0)
			result  = fnPodTest(raw8Bytes, img.imgwidth, img.imgheight, 0, &m_podDefectStack, POD_Cam0);
		else
			result  = fnPodTest(raw8Bytes, img.imgwidth, img.imgheight, 0, &m_podDefectStack, POD_Cam1);

		bSaveRawFile("D://POD.raw",raw8Bytes,img.imgwidth*img.imgheight);
	
		SAFE_DELETE_ARRAY(raw8);
		SAFE_DELETE_ARRAY(raw8Bytes);

		if (result == -1)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("内存申请失败,POD TEST FAILED"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD TEST FAILED !!"));
			SetResult(RESULT_FAIL);
		}
		else
		{
			if (result == 1)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("POD TEST PASS"),COLOR_GREEN,200);
				SetResult(RESULT_PASS);
			}
			else
			{
				CString log;
				log.Format(_T("POD Count: %d   deadnum:%d  wound num:%d"),m_podDefectStack.DefectStackCnt,m_podDefectStack.DeadNum,m_podDefectStack.WoundNum);
				m_pInterface->Ctrl_Addlog(CamID,log,COLOR_GREEN,200);
				ShowInfo();
				m_pInterface->Ctrl_Addlog(CamID,_T("POD TEST FAILED"),COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD TEST FAILED !!"));
				SetResult(RESULT_FAIL);
			}
		}		
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("POD DLL LOAD FAILED !!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD DLL LOAD FAILED !!"));
		SetResult(RESULT_FAIL);
	}
	
	FreeLibrary(hInstance);
	SAFE_DELETE_ARRAY(m_podDefectStack.DfcStk);
   return 0;
}

void POD_TestItem::ShowInfo()
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
		for (UINT i=0; i< m_podDefectStack.DefectStackCnt; i++)
		{
			CurrentColor = COLOR_RED;

			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			int cx1,cy1;
			cx1 = (int)(m_podDefectStack.DfcStk[i].xIndex * 2 * Xratio);
			cy1 = (int)(m_podDefectStack.DfcStk[i].yIndex * 2 * Yratio);
			int px1 = cx1 - 10;
			int py1 = cy1 - 10;
			int px2 = cx1 + 10;
			int py2 = cy1 + 10;

	
			BOOL bShowCen = TRUE;
			if(bShowCen)
				pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		}
	} 
	m_pInterface->Ctrl_ShowMemDC(CamID);
}
/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int POD_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int POD_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 

	SetConfigPath(GetModulePath() + "\\OfPODTest.ini");
	SetName(_T("Spec_Cam0"));

	//SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	//SetName(m_cfgName);

	pDlg->m_podCamera0Enable = ReadConfigInt(_T("Camera0Enable"), 0);

	pDlg->m_podCamera0DeadEnable = ReadConfigInt(_T("Dead_Enable"), 0);
	pDlg->m_podCamera0DeadUseUnit = ReadConfigInt(_T("DeadUnit"), 0);
	pDlg->m_podCamera0WoundEnable = ReadConfigInt(_T("Wound_Enable"), 0);
	pDlg->m_podCamera0WoundUseUnit = ReadConfigInt(_T("WoundUnit"), 0);
	pDlg->m_podCamera0UseLog = ReadConfigInt(_T("UseLog"), 0);

	pDlg->m_podCamera0LeftShieldLines = ReadConfigString(_T("LeftSL"), _T("25"));
	pDlg->m_podCamera0TopShieldLines = ReadConfigString(_T("TopSL"), _T(""));
	pDlg->m_podCamera0WLeftShieldLines = ReadConfigString(_T("WLeftSL"), _T(""));
	pDlg->m_podCamera0WTopShieldLines = ReadConfigString(_T("WTopSL"), _T(""));
	pDlg->m_podCamera0RightShieldLines = ReadConfigString(_T("RightSL"), _T(""));
	pDlg->m_podCamera0BottomShieldLines = ReadConfigString(_T("BottomSL"), _T(""));
	pDlg->m_podCamera0WRightShieldLines = ReadConfigString(_T("WRightSL"), _T(""));
	pDlg->m_podCamera0WBottomShieldLines = ReadConfigString(_T("WBottomSL"), _T(""));
	pDlg->m_podCamera0AreaWidth = ReadConfigString(_T("AreaWidth"), _T(""));
	pDlg->m_podCamera0AreaHeight = ReadConfigString(_T("AreaHeight"), _T(""));
	pDlg->m_podCamera0SmallAreaWidth = ReadConfigString(_T("smallAreaWidth"), _T(""));
	pDlg->m_podCamera0SmallAreaHeight = ReadConfigString(_T("smallAreaHeight"), _T(""));
	pDlg->m_podCamera0DeadType = ReadConfigString(_T("ClusterType"), _T(""));
	pDlg->m_podCamera0DeadSpc = ReadConfigString(_T("DeadSpc"), _T(""));
	pDlg->m_podCamera0WoundSpcEdge = ReadConfigString(_T("WoundSpc_edge"), _T(""));
	pDlg->m_podCamera0WoundSpecCenter = ReadConfigString(_T("WoundSpc_center"), _T(""));


	SetName(_T("Spec_Cam1"));
	pDlg->m_podCamera1Enable = ReadConfigInt(_T("Camera1Enable"), 0);

	pDlg->m_podCamera1DeadEnable = ReadConfigInt(_T("Dead_Enable"), 0);
	pDlg->m_podCamera1DeadUseUnit = ReadConfigInt(_T("DeadUnit"), 0);
	pDlg->m_podCamera1WoundEnable = ReadConfigInt(_T("Wound_Enable"), 0);
	pDlg->m_podCamera1WoundUseUnit = ReadConfigInt(_T("WoundUnit"), 0);
	pDlg->m_podCamera1UseLog = ReadConfigInt(_T("UseLog"), 0);

	pDlg->m_podCamera1LeftShieldLines = ReadConfigString(_T("LeftSL"), _T("25"));
	pDlg->m_podCamera1TopShieldLines = ReadConfigString(_T("TopSL"), _T(""));
	pDlg->m_podCamera1WLeftShieldLines = ReadConfigString(_T("WLeftSL"), _T(""));
	pDlg->m_podCamera1WTopShieldLines = ReadConfigString(_T("WTopSL"), _T(""));
	pDlg->m_podCamera1RightShieldLines = ReadConfigString(_T("RightSL"), _T(""));
	pDlg->m_podCamera1BottomShieldLines = ReadConfigString(_T("BottomSL"), _T(""));
	pDlg->m_podCamera1WRightShieldLines = ReadConfigString(_T("WRightSL"), _T(""));
	pDlg->m_podCamera1WBottomShieldLines = ReadConfigString(_T("WBottomSL"), _T(""));
	pDlg->m_podCamera1AreaWidth = ReadConfigString(_T("AreaWidth"), _T(""));
	pDlg->m_podCamera1AreaHeight = ReadConfigString(_T("AreaHeight"), _T(""));
	pDlg->m_podCamera1SmallAreaWidth = ReadConfigString(_T("smallAreaWidth"), _T(""));
	pDlg->m_podCamera1SmallAreaHeight = ReadConfigString(_T("smallAreaHeight"), _T(""));
	pDlg->m_podCamera1DeadType = ReadConfigString(_T("ClusterType"), _T(""));
	pDlg->m_podCamera1DeadSpc = ReadConfigString(_T("DeadSpc"), _T(""));
	pDlg->m_podCamera1WoundSpcEdge = ReadConfigString(_T("WoundSpc_edge"), _T(""));
	pDlg->m_podCamera1WoundSpecCenter = ReadConfigString(_T("WoundSpc_center"), _T(""));

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(_T("Sensor"));
	m_PixelOrder =  ReadConfigInt(_T("outformat"), 0);

	SetName(m_cfgName);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int POD_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	//SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	//SetName(m_cfgName);

	SetConfigPath(GetModulePath() + "\\OfPODTest.ini");
	SetName(_T("Spec_Cam0"));

	WriteConfigInt(_T("Camera0Enable"), pDlg->m_podCamera0Enable);

	WriteConfigInt(_T("Dead_Enable"), pDlg->m_podCamera0DeadEnable, 0);
	WriteConfigInt(_T("DeadUnit"), pDlg->m_podCamera0DeadUseUnit, 0);
	WriteConfigInt(_T("Wound_Enable"), pDlg->m_podCamera0WoundEnable, 0);
	WriteConfigInt(_T("WoundUnit"), pDlg->m_podCamera0WoundUseUnit, 0);
	WriteConfigInt(_T("UseLog"), pDlg->m_podCamera0UseLog, 0);

	WriteConfigString(_T("LeftSL"), pDlg->m_podCamera0LeftShieldLines);
	WriteConfigString(_T("TopSL"), pDlg->m_podCamera0TopShieldLines);
	WriteConfigString(_T("WLeftSL"), pDlg->m_podCamera0WLeftShieldLines);
	WriteConfigString(_T("WTopSL"), pDlg->m_podCamera0WTopShieldLines);
	WriteConfigString(_T("RightSL"), pDlg->m_podCamera0RightShieldLines);
	WriteConfigString(_T("BottomSL"), pDlg->m_podCamera0BottomShieldLines);
	WriteConfigString(_T("WRightSL"), pDlg->m_podCamera0WRightShieldLines);
	WriteConfigString(_T("WBottomSL"), pDlg->m_podCamera0WBottomShieldLines);
	WriteConfigString(_T("AreaWidth"), pDlg->m_podCamera0AreaWidth);
	WriteConfigString(_T("AreaHeight"), pDlg->m_podCamera0AreaHeight);
	WriteConfigString(_T("smallAreaWidth"), pDlg->m_podCamera0SmallAreaWidth);
	WriteConfigString(_T("smallAreaHeight"), pDlg->m_podCamera0SmallAreaHeight);
	WriteConfigString(_T("ClusterType"), pDlg->m_podCamera0DeadType);
	WriteConfigString(_T("DeadSpc"), pDlg->m_podCamera0DeadSpc);
	WriteConfigString(_T("WoundSpc_edge"), pDlg->m_podCamera0WoundSpcEdge);
	WriteConfigString(_T("WoundSpc_center"), pDlg->m_podCamera0WoundSpecCenter);


	SetName(_T("Spec_Cam1"));
	WriteConfigInt(_T("Camera1Enable"), pDlg->m_podCamera1Enable);

	WriteConfigInt(_T("Dead_Enable"), pDlg->m_podCamera1DeadEnable, 0);
	WriteConfigInt(_T("DeadUnit"), pDlg->m_podCamera1DeadUseUnit, 0);
	WriteConfigInt(_T("Wound_Enable"), pDlg->m_podCamera1WoundEnable, 0);
	WriteConfigInt(_T("WoundUnit"), pDlg->m_podCamera1WoundUseUnit, 0);
	WriteConfigInt(_T("UseLog"), pDlg->m_podCamera1UseLog, 0);

	WriteConfigString(_T("LeftSL"), pDlg->m_podCamera1LeftShieldLines);
	WriteConfigString(_T("TopSL"), pDlg->m_podCamera1TopShieldLines);
	WriteConfigString(_T("WLeftSL"), pDlg->m_podCamera1WLeftShieldLines);
	WriteConfigString(_T("WTopSL"), pDlg->m_podCamera1WTopShieldLines);
	WriteConfigString(_T("RightSL"), pDlg->m_podCamera1RightShieldLines);
	WriteConfigString(_T("BottomSL"), pDlg->m_podCamera1BottomShieldLines);
	WriteConfigString(_T("WRightSL"), pDlg->m_podCamera1WRightShieldLines);
	WriteConfigString(_T("WBottomSL"), pDlg->m_podCamera1WBottomShieldLines);
	WriteConfigString(_T("AreaWidth"), pDlg->m_podCamera1AreaWidth);
	WriteConfigString(_T("AreaHeight"), pDlg->m_podCamera1AreaHeight);
	WriteConfigString(_T("smallAreaWidth"), pDlg->m_podCamera1SmallAreaWidth);
	WriteConfigString(_T("smallAreaHeight"), pDlg->m_podCamera1SmallAreaHeight);
	WriteConfigString(_T("ClusterType"), pDlg->m_podCamera1DeadType);
	WriteConfigString(_T("DeadSpc"), pDlg->m_podCamera1DeadSpc);
	WriteConfigString(_T("WoundSpc_edge"), pDlg->m_podCamera1WoundSpcEdge);
	WriteConfigString(_T("WoundSpc_center"), pDlg->m_podCamera1WoundSpecCenter);

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(m_cfgName);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR POD_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR POD_TestItem::GetReportLowLimit()
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
LPCTSTR POD_TestItem::GetReportHightLimit()
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
LPCTSTR POD_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}


CString POD_TestItem::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	return ret;
}