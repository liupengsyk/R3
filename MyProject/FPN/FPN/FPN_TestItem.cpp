#include "StdAfx.h"
#include "FPN_TestItem.h"
#include "ImageProc.h"

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new FPN_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
FPN_TestItem::FPN_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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
	preGain = 0;

	//.....
}

FPN_TestItem::~FPN_TestItem(void)
{

}


int FPN_TestItem::InitItem()
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
int FPN_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
    interval = 0;

	CString path=m_pInterface->Ctrl_GetCurrentPath();
	path=path+L"SensorTab\\init.ini";
	SetConfigPath(path);	 //设置CONFIG路径	
	SetName(L"ErrorCode");
	CString temp = _T("");
	temp.Format(_T("CAM%d_ErrorCode"),CamID);
	WriteConfigInt(temp, pDlg->ErrorCode); 

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	SetName(m_cfgName);
	m_pInterface->Sensor_ReadGain(CamID,preGain);

	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int FPN_TestItem::Testing()
{
   //TODO:在此添加测试项代码

	if (interval==0)
	{
		m_pInterface->Sensor_WriteExp(CamID,pDlg->m_DefultExposure);
		m_pInterface->Sensor_WriteGain(CamID,pDlg->m_DefultGain);	
		interval++;
		return 1;
	}
	else if (interval > 3)
	{
		img = * m_pInterface->Ctrl_GetImgInfo(CamID);		
		BOOL ret=FixedPatternNoiseTest(img.Raw_buffer,img.imgwidth,img.imgheight);

		if(ret == FALSE)
		{
			if (pDlg->m_bSaveImage)
			{
				char filename[1024]  = {0};
				CString temppath = _T("");
				temppath = m_pInterface->Ctrl_GetCurrentPath();
				temppath += _T("FPN_Image");
				CreateDirectory(temppath,NULL);
				temppath += _T("\\");
				temppath += m_pInterface->Sensor_GetCamFuseID(CamID);
				temppath += _T(".raw") ;

				WideCharToMultiByte(CP_ACP,0,temppath,(temppath.GetLength()+1), filename, 1024, NULL, NULL);
				bSaveRawFile(temppath,img.Raw_buffer,img.imgwidth,img.imgheight);
			}

			m_pInterface->Ctrl_Addlog(CamID,_T("FPN TEST FAIL!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("FPN测试失败！"));
			SetResult(RESULT_FAIL);		
			m_pInterface->Ctrl_Addlog(CamID,_T("计算图片FPN测试失败"),COLOR_BLUE,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("FPN TEST FAIL!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("FPN测试失败！"));
			SetResult(RESULT_FAIL);		 
			return 0;
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("FPN TEST PASS!"),COLOR_GREEN,200);
			SetResult(RESULT_PASS);		 
		}
	}
	else
	{
		interval++;
		return 1;
	}

    return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int FPN_TestItem::Finalize()
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
int FPN_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->ErrorCode=ReadConfigInt(_T("ErrorCode"),16);
	pDlg->m_SinglePixelMax = ReadConfigInt(_T("SinglePixelMax"),16);
	pDlg->m_PixelCountPercent = ReadConfigInt(_T("PixelCountPercent"),10);
	pDlg->m_RowMaxSpec=ReadConfigInt(_T("RowMaxSpec"),16);
	pDlg->m_ColMaxSpec=ReadConfigInt(_T("ColMaxSpec"),16);
	pDlg->m_DiffRowPercent = ReadConfigInt(_T("DiffRowPercent"),16);
	pDlg->m_DiffColPercent = ReadConfigInt(_T("DiffColPercent"),16);

	pDlg->m_bSaveImage=ReadConfigInt(_T("bSaveImage"),16);
	pDlg->m_DefultExposure=ReadConfigInt(_T("DefultExposure"),16);
	pDlg->m_DefultGain =ReadConfigInt(_T("DefultGain"),16);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int FPN_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("ErrorCode"),pDlg->ErrorCode);

	WriteConfigInt(_T("SinglePixelMax"),pDlg->m_SinglePixelMax);
	WriteConfigInt(_T("PixelCountPercent"),pDlg->m_PixelCountPercent);
	WriteConfigInt(_T("RowMaxSpec"),pDlg->m_RowMaxSpec);
	WriteConfigInt(_T("ColMaxSpec"),pDlg->m_ColMaxSpec);
	WriteConfigInt(_T("DiffRowPercent"),pDlg->m_DiffRowPercent);
	WriteConfigInt(_T("DiffColPercent"),pDlg->m_DiffColPercent);

	WriteConfigInt(_T("bSaveImage"),pDlg->m_bSaveImage);
	WriteConfigInt(_T("DefultExposure"),pDlg->m_DefultExposure);
	WriteConfigInt(_T("DefultGain"),pDlg->m_DefultGain);
	//........................
	return 0;
}

BOOL FPN_TestItem::bSaveBmpFile(CString sfilename, BYTE *pBuffer, UINT width, UINT height)
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

void FPN_TestItem::bSaveRawFile(CString sfilename, USHORT *pBuffer, UINT width, UINT height)
{
	CFile file;
	BOOL ret = file.Open(sfilename, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if (!ret)
	{
		return;
	} 
	file.Write((void*) pBuffer, width * height * sizeof(USHORT) );
	file.Close();

	return;
}

/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR FPN_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header = _T("RowMeanMax,ColMeanMax,RowPixelPercent,ColPixelPercent,RowDiffPercent,ColDiffPercent");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR FPN_TestItem::GetReportLowLimit()
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
LPCTSTR FPN_TestItem::GetReportHightLimit()
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
LPCTSTR FPN_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	Content.Format(_T("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f"), RowMeanMax,ColMeanMax,RowPixelPercent,ColPixelPercent,RowDiffPercent,ColDiffPercent);
	return Content;
}

CString FPN_TestItem::GetModulePath()
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

BOOL FPN_TestItem::FixedPatternNoiseTest(USHORT * _RawImgaeBuffer,int width,int height)
{
	if (NULL == _RawImgaeBuffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("_RawImgaeBuffer为空!"),COLOR_RED,200);
		return FALSE;
	}

	BOOL Ret = TRUE;

//////////////////////行FPN计算///////////////////////////////
	CString AFXstr = _T("");
	int Count=0;
	int LineMax=0;
	int LineNum=0;

	RowMeanMax = 0.0;
	ColMeanMax = 0.0;
	RowPixelPercent = 100.0;
	ColPixelPercent = 100.0;
	RowDiffPercent = 0.0;
	ColDiffPercent = 0.0;

	float * RowMean = new float[height];
	float temp = 0.0;
	if (NULL == RowMean)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("RowMean内存申请失败!"),COLOR_RED,200);
		return FALSE;
	}

	for (int ii=0; ii<height; ii++)
	{
		for (int jj=0; jj<width; jj++)
		{
			if (_RawImgaeBuffer[ii*width + jj] > pDlg->m_SinglePixelMax)
			{
				Count++;
			}
			temp += _RawImgaeBuffer[ii*width + jj];
		}
		RowMean[ii] = temp/width;
		RowMeanMax = RowMean[ii]>RowMeanMax?RowMean[ii]:RowMeanMax;

		if (RowMean[ii] > pDlg->m_RowMaxSpec)
		{
			Ret = FALSE;
	//		AFXstr.Format(_T("第%d行平均值超出规格!"),ii+1);
	//		m_pInterface->Ctrl_Addlog(CamID,AFXstr,COLOR_RED,200);
		}
		temp = 0.0;
		if (Count > LineMax)
		{
			LineMax = Count;
			LineNum = ii;
		}
		Count = 0;
	}

	float percent = 0.0;
	percent = (float)LineMax*100.0/width;
	RowPixelPercent = percent;

	if (percent > pDlg->m_PixelCountPercent)
	{
		Ret = FALSE;
		AFXstr.Format(_T("第%d行PixelCount Percent值超过规格"),LineNum+1);
		m_pInterface->Ctrl_Addlog(CamID,AFXstr,COLOR_RED,200);
	}

	for (int ii=1; ii<height; ii++)
	{
		RowMean[ii]=RowMean[ii]==0?1:RowMean[ii];
		temp = fabs(RowMean[ii]-RowMean[ii-1])/RowMean[ii]*100.0;
		RowDiffPercent = temp>RowDiffPercent?temp:RowDiffPercent;

		if (temp > pDlg->m_DiffRowPercent)
		{
			Ret = FALSE;
			AFXstr.Format(_T("第%d行与第%d行差异超出规格"),ii,ii+1);
			m_pInterface->Ctrl_Addlog(CamID,AFXstr,COLOR_RED,200);
		}
	}
	delete [] RowMean;
	RowMean = NULL;

//////////////////////列FPN计算///////////////////////////////
	float * ColMean = new float[width];
	if (NULL == ColMean)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ColMean申请内存失败!"),COLOR_RED,200);
		return FALSE;
	}
	int ColMax=0;
	int ColNum=0;
	temp = 0.0;
	Count = 0;
	for (int ii=0; ii<width; ii++)
	{
		for (int jj=0; jj<height; jj++)
		{
			if (_RawImgaeBuffer[jj*width + ii] > pDlg->m_SinglePixelMax)
			{
				Count++;
			}
			temp += _RawImgaeBuffer[jj*width + ii];
		}
		ColMean[ii] = temp/height;
		ColMeanMax = ColMean[ii]>ColMeanMax?ColMean[ii]:ColMeanMax;

		if (ColMean[ii] > pDlg->m_ColMaxSpec)
		{
			Ret = FALSE;
		//	AFXstr.Format(_T("第%d列平均值超规格!"),ii+1);
		//	m_pInterface->Ctrl_Addlog(CamID,AFXstr,COLOR_RED,200);
		}
		temp = 0.0;
		if (Count > ColMax)
		{
			ColMax = Count;
			ColNum = ii;
		}
		Count = 0;
	}

	percent = (float)ColMax*100.0/height;
	ColPixelPercent = percent>ColPixelPercent?percent:ColPixelPercent;

	if (percent > pDlg->m_PixelCountPercent)
	{
		Ret = FALSE;
		AFXstr.Format(_T("第%d列PixelCount Percent超过规格!"),LineNum+1);
		m_pInterface->Ctrl_Addlog(CamID,AFXstr,COLOR_RED,200);
	}

	for (int ii=1; ii<width; ii++)
	{
		ColMean[ii] = ColMean[ii]==0?1:ColMean[ii];
		temp = fabs(ColMean[ii]-ColMean[ii-1])/ColMean[ii]*100.0;
		ColDiffPercent = temp>ColDiffPercent?temp:ColDiffPercent;

		if (temp > pDlg->m_DiffColPercent)
		{
			Ret = FALSE;
			AFXstr.Format(_T("第%d行与第%d行差异超出规格"),ii,ii+1);
			m_pInterface->Ctrl_Addlog(CamID,AFXstr,COLOR_RED,200);
		}
	}

	delete [] ColMean;
	ColMean = NULL;

	return Ret;
}
