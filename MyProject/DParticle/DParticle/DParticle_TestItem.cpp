#include "StdAfx.h"
#include "DParticle_TestItem.h"
#include "ImageProc.h"

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new DParticle_TestItem(pInterface, lpszName);
}

/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
DParticle_TestItem::DParticle_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

DParticle_TestItem::~DParticle_TestItem(void)
{

}


int DParticle_TestItem::InitItem()
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
int DParticle_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
	CString path=m_pInterface->Ctrl_GetCurrentPath();
	path=path+L"OfDPaticleTest.ini";
	SetConfigPath(path);	 //设置CONFIG路径	
	SetName(L"Spec");

	WriteConfigInt(_T("LeftSL"), pDlg->nLeftSL, 0);
	WriteConfigInt(_T("RightSL"), pDlg->nRightSL, 0);
	WriteConfigInt(_T("TopSL"), pDlg->nTopSL, 0);
	WriteConfigInt(_T("BottomSL"), pDlg->nBottomSL, 0);

	WriteConfigInt(_T("AreaHeight"), pDlg->nAreaHeight, 0);
	WriteConfigInt(_T("AreaWidth"), pDlg->nAreaWidth, 0);
	WriteConfigInt(_T("DeadSpec"), pDlg->nDeadSpec, 0);
	WriteConfigInt(_T("DeadType"), pDlg->nDeadType, 0);
	WriteConfigInt(_T("DeadUnit"), pDlg->nDeadUnit, 0);

	path=m_pInterface->Ctrl_GetCurrentPath();
	path=path+L"SensorTab\\init.ini";
	SetConfigPath(path);	 //设置CONFIG路径	
	SetName(L"ErrorCode");
	CString temp = _T("");
	temp.Format(_T("CAM%d_ErrorCode"),CamID);
	WriteConfigInt(temp, pDlg->ErrorCode); 

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(m_cfgName);
  
	interval=0;

	m_pInterface->Sensor_ReadGain(CamID,preGain);

	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int DParticle_TestItem::Testing()
{
   //TODO:在此添加测试项代码
  if (	interval==0)
  {
	  	m_pInterface->Sensor_WriteExp(CamID,pDlg->nDefultExposure);
		m_pInterface->Sensor_WriteGain(CamID,pDlg->nDefultGain);	
	    interval++;
		return 1;
  }
  else if (interval > 3)
  {
		WhiteSpotResult.DfcStk = (WPixInfo*) new WPixInfo[10000];
		WhiteSpotResult.DefectStackCnt = 0;
		WhiteSpotResult.DeadNum = 0; 			
		
		for(  int k = 0; k < 10000; k++)
		{
			WhiteSpotResult.DfcStk[k].xIndex = 0;
			WhiteSpotResult.DfcStk[k].Y_Value = 0;
			WhiteSpotResult.DfcStk[k].yIndex = 0;
		}

		img = * m_pInterface->Ctrl_GetImgInfo(CamID);

		int wspotresult=0;
		BYTE* raw8Bytes = new BYTE[img.imgwidth * img.imgheight];

		for (int y =0; y < img.imgheight; y++)
		{
			for(int x =0; x < img.imgwidth; x++)
			{
				raw8Bytes[y*img.imgwidth + x] = (img.Raw_buffer[y*img.imgwidth + x] >>2 ) & 0xff;
			}
		}

		wspotresult = WhiteSpotTest(raw8Bytes, img.imgwidth,img.imgheight,img.RawFormat,&WhiteSpotResult);

		if (wspotresult == 0)
		{
			CString log;
			log.Format(_T("DPaticle Count: %d"),WhiteSpotResult.DefectStackCnt);
			m_pInterface->Ctrl_Addlog(CamID,log,COLOR_GREEN,200);
			ShowInfo();

			if (pDlg->bSaveImage)
			{
				char filename[1024]  = {0};
				CString temppath = _T("");
				temppath = GetModulePath();
				temppath += _T("\\\\");
				temppath += _T("DPaticle_Image");
				CreateDirectory(temppath,NULL);
				temppath += _T("\\\\");
				temppath += m_pInterface->Sensor_GetCamFuseID(CamID);
				temppath += _T(".raw") ;

				WideCharToMultiByte(CP_ACP,0,temppath,(temppath.GetLength()+1), filename, 1024, NULL, NULL);
				//		sprintf(filename,"%s",temppath.GetBuffer(temppath.GetLength()));
				bSaveRawFile(filename,raw8Bytes,img.imgwidth*img.imgheight);
			}

			m_pInterface->Ctrl_Addlog(CamID,_T("POD TEST FAILED"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD测试失败！"));
			SetResult(RESULT_FAIL);		 
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("DPaticle TEST PASS"),COLOR_GREEN,200);
			SetResult(RESULT_PASS);
		}
	   delete [] raw8Bytes;
	   if (WhiteSpotResult.DfcStk != NULL)
	   {
		   delete[] WhiteSpotResult.DfcStk;
		   WhiteSpotResult.DfcStk = NULL;
	   }
  }
  else
  {
	  interval++;
	  return 1;
  }

   return 0;
}

void DParticle_TestItem::ShowInfo()
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
		for (UINT i=0; i< WhiteSpotResult.DefectStackCnt; i++)
		{
			CurrentColor = COLOR_RED;

			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			int cx1,cy1;
			cx1 = (int)(WhiteSpotResult.DfcStk[i].xIndex * Xratio);
			cy1 = (int)((img.imgheight - WhiteSpotResult.DfcStk[i].yIndex) * Yratio);
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
int DParticle_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码

	m_pInterface->Sensor_WriteGain(CamID,preGain);

	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int DParticle_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->ErrorCode =ReadConfigInt(_T("ErrorCode"),16);
	pDlg->nAreaHeight=ReadConfigInt(_T("AreaHeight"),16);
	pDlg->nAreaWidth=ReadConfigInt(_T("AreaWidth"),16);
	pDlg->nBottomSL=ReadConfigInt(_T("BottomSL"),16);
	pDlg->nTopSL=ReadConfigInt(_T("TopSL"),16);
	pDlg->nLeftSL=ReadConfigInt(_T("LeftSL"),16);
	pDlg->nRightSL=ReadConfigInt(_T("RightSL"),16);

	pDlg->nDeadSpec=ReadConfigInt(_T("DeadSpec"),16);
	pDlg->nDeadType=ReadConfigInt(_T("DeadType"),16);
	pDlg->nDeadUnit =ReadConfigInt(_T("DeadUnit"),16);
	pDlg->nDefultExposure=ReadConfigInt(_T("DefultExposure"),16);
	pDlg->nDefultGain=ReadConfigInt(_T("DefultGain"),16);
	pDlg->bSaveImage=ReadConfigInt(_T("bSaveImage"),16);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int DParticle_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("ErrorCode"),pDlg->ErrorCode);
	WriteConfigInt(_T("AreaHeight"),pDlg->nAreaHeight);
	WriteConfigInt(_T("AreaWidth"),pDlg->nAreaWidth);
	WriteConfigInt(_T("BottomSL"),pDlg->nBottomSL);
	WriteConfigInt(_T("TopSL"),pDlg->nTopSL);
	WriteConfigInt(_T("LeftSL"),pDlg->nLeftSL);
	WriteConfigInt(_T("RightSL"),pDlg->nRightSL);

	WriteConfigInt(_T("DeadSpec"),pDlg->nDeadSpec);
	WriteConfigInt(_T("DeadType"),pDlg->nDeadType);
	WriteConfigInt(_T("DeadUnit"),pDlg->nDeadUnit);
	WriteConfigInt(_T("DefultExposure"),pDlg->nDefultExposure);
	WriteConfigInt(_T("DefultGain"),pDlg->nDefultGain);
	WriteConfigInt(_T("bSaveImage"),pDlg->bSaveImage);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR DParticle_TestItem::GetReportHeads()
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
LPCTSTR DParticle_TestItem::GetReportLowLimit()
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
LPCTSTR DParticle_TestItem::GetReportHightLimit()
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
LPCTSTR DParticle_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

CString DParticle_TestItem::GetModulePath()
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
