#include "StdAfx.h"
#include "StereoCal_TestItem.h"
#include "ImageProc.h"
#include <iostream>
 

#pragma comment(lib,"MtkLib/mtk_stereo_verify.lib")
#pragma comment(lib,"mems_dualccm_twolayer\\mems_dualccm_twolayer.lib")
#pragma comment(lib,"HwCalLib\\HWCalibDll.lib")

#ifndef Tccm_EXP
#define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new StereoCal_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
StereoCal_TestItem::StereoCal_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;

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

StereoCal_TestItem::~StereoCal_TestItem(void)
{

}


int StereoCal_TestItem::InitItem()
{
	CamID = GetCamID();
	NearChartDist_mm = 300;
	FarChartDist_mm = 600;
	ChartPointPitch = 26;

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption(); 
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int StereoCal_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
	if(Platform == _T("Mtk"))
	{
		ImageInfo Img = *m_pInterface->Ctrl_GetImgInfo(0);

		leftFarImg.imgwidth = Img.imgwidth;
		leftFarImg.imgheight = Img.imgheight;
		leftNearImg.imgwidth = Img.imgwidth;
		leftNearImg.imgheight = Img.imgheight;
		int LeftImgSize = leftFarImg.imgwidth * leftFarImg.imgheight;
		leftFarImg.Raw_buffer = new USHORT[LeftImgSize];
		leftNearImg.Raw_buffer = new USHORT[LeftImgSize]; 

		Img = *m_pInterface->Ctrl_GetImgInfo(1);
		rightFarImg.imgwidth = Img.imgwidth;
		rightFarImg.imgheight = Img.imgheight;
		rightNearImg.imgwidth = Img.imgwidth;
		rightNearImg.imgheight = Img.imgheight;
		int RightImgSize = rightFarImg.imgwidth * rightFarImg.imgheight;
		rightFarImg.Raw_buffer = new USHORT[RightImgSize];
		rightNearImg.Raw_buffer = new USHORT[RightImgSize];

		gVerify_Items = new int[180]; 
	}
	else if(Platform == _T("Ofilm"))
	{

		LeftFarImagePoint_xy.clear();
		LeftNearImagePoint_xy.clear();
		RightFarImagePoint_xy.clear();
		RightNearImagePoint_xy.clear();

		LeftObjectArray.clear();
		RightObjectArray.clear();
	}
	 

	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int StereoCal_TestItem::Testing()
{
	//TODO:在此添加测试项代码
	int Flag = 0;
	if(Platform == _T("Mtk"))
	{
		Flag = Mtk_StereoCal();
		
	}
	else if(Platform == _T("Qualcomm"))
	{
		Flag = Qualcomm_StereoCal();
	}
	else if(Platform == _T("Ofilm"))
	{
		//Cam3DCalibration();
		Flag = Cam3DCalibrationSys();

		 
		//CamDxoCalibration();
	}
	else if(Platform == _T("HwHdc"))
	{
		Flag = HwHdcCal();
	}
	else if(Platform == _T("HwMMI"))
	{
		Flag = HwMmiCheck();
	}

	if(Flag!=1)
	{
		SetResult(RESULT_FAIL);
	}
	else
	{ 
		SetResult(RESULT_PASS);
	}
	return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int StereoCal_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	 

	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int StereoCal_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	ConfigName = ReadConfigString(_T("ConfigName"),_T(""));
	leftFarImgPath= ReadConfigString(_T("leftFarImgPath"),_T(""));
	leftNearImgPath= ReadConfigString(_T("leftNearImgPath"),_T(""));
	rightFarImgPath= ReadConfigString(_T("rightFarImgPath"),_T(""));
	rightNearImgPath= ReadConfigString(_T("rightNearImgPath"),_T(""));

	cfg_dir = ReadConfigString(L"cfg_dir",L"MTK_CFG");
	image_dir = ReadConfigString(L"image_dir",L"ImgSave");


	Platform= ReadConfigString(_T("Platform"),_T(""));
	MainCamID =  ReadConfigInt(_T("MainCamID"),0);

	 
	m_CamInfo[0].CamEFL = ReadConfigString(_T("Cam0EFL"),_T("3.81")); 
	m_CamInfo[0].CamPixelSize  = ReadConfigString(_T("Cam0PixelSize"),_T("1.12"));
 

	m_CamInfo[1].CamEFL = ReadConfigString(_T("Cam1EFL"),_T("3.81")); 
	m_CamInfo[1].CamPixelSize = ReadConfigString(_T("Cam1PixelSize"),_T("1.12"));
	 

	m_ChartInfo.objectPitch_x = ReadConfigInt(_T("objectPitch_x"),26);
	m_ChartInfo.NearChartDist = ReadConfigInt(_T("NearChartDist"),300);
	m_ChartInfo.FarChartDist = ReadConfigInt(_T("FarChartDist"),500);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int StereoCal_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   

	WriteConfigString(_T("ConfigName"),ConfigName);
	WriteConfigString(_T("leftFarImgPath"),leftFarImgPath);
	WriteConfigString(_T("leftNearImgPath"),leftNearImgPath);
	WriteConfigString(_T("rightFarImgPath"),rightFarImgPath);
	WriteConfigString(_T("rightNearImgPath"),rightNearImgPath);
	WriteConfigString(_T("Platform"),Platform);
	WriteConfigInt(_T("MainCamID"),MainCamID);	

	WriteConfigString(_T("cfg_dir"),cfg_dir);
	WriteConfigString(_T("image_dir"),image_dir);

	CString str;
	WriteConfigString(_T("Cam0EFL"),m_CamInfo[0].CamEFL); 
	WriteConfigString(_T("Cam0PixelSize"),m_CamInfo[0].CamPixelSize); 

	WriteConfigString(_T("Cam1EFL"),m_CamInfo[1].CamEFL); 
	WriteConfigString(_T("Cam1PixelSize"),m_CamInfo[1].CamPixelSize); 

	m_ChartInfo.objectPitch_x = ReadConfigInt(_T("objectPitch_x"),26);
	m_ChartInfo.NearChartDist = ReadConfigInt(_T("NearChartDist"),300);
	m_ChartInfo.FarChartDist = ReadConfigInt(_T("FarChartDist"),500);

	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR StereoCal_TestItem::GetReportHeads()
{
	static CString header;
	if(Platform == _T("Mtk"))
	{
		header = _T("");
		header += _T("status,");
		header += _T("quality level,");
		header += _T("top cover region,");
		header += _T("right cover region,");
		header += _T("bottom cover region,");
		header += _T("left cover region,");
		header += _T("intensity_main,");
		header += _T("difference intensity,");
		header += _T("MTK pattern,");
		header += _T("scene MTK ,");

		header += _T("quality level,");
		header += _T("top cover region,");
		header += _T("right cover region,");
		header += _T("bottom cover region,");
		header += _T("left cover region,");
		header += _T("intensity main ,");
		header += _T("difference intensity,");
		header += _T("MTK pattern,");
		header += _T("scene MTK ,");
	}
	else if(Platform == _T("HwMMI"))
	{
		header += _T("300depth,1000depth");
	}

	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR StereoCal_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("270,800,");
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR StereoCal_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("330,1200,");
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR StereoCal_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");

	if(Platform == _T("Mtk"))
	{
		for(UINT i = 0; i<vecOutFlag.size(); i++)
		{
			CString str;
			str.Format(_T("%d,"),vecOutFlag[i]);
			Content += str;
		}
	}
	else if(Platform == _T("Qualcomm"))
	{
	 
	} 
	else if(Platform == _T("HwMMI"))
	{
		if(tempStr.size()>1)
		{
			for(int i=0;i<2;i++)
			{
				Content += tempStr[i];
			}
		}
		
	}
	vecOutFlag.clear();
	return Content;

}

BOOL StereoCal_TestItem::GetImgRawbuf(CString srcImgPath,ImageInfo* img)
{ 
	CStdioFile file;
	int FlgOpen = file.Open(srcImgPath,CFile::modeRead|CFile::typeBinary);
	file.SeekToBegin();
	file.Read(img->Raw_buffer,img->imgwidth *img->imgheight *sizeof(USHORT));	 
	file.Close(); 
	/*file.Open(srcImgPath+_T("out"),CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	file.Write(img->Raw_buffer,img->imgwidth *img->imgheight *sizeof(USHORT));
	file.Close();
	*/return TRUE; 
}
int StereoCal_TestItem::Mtk_StereoCal()
{
	CString currentPath = m_pInterface->Ctrl_GetCurrentPath();
	int Flag = 0;
	char c_ConfigName[1024]={0};
	int bufflen=1024; 
	CString configFileFullPath = currentPath + cfg_dir + L"\\" + ConfigName;
	int len = configFileFullPath.GetLength();
	WideCharToMultiByte(CP_ACP,0,configFileFullPath,len,c_ConfigName,len,NULL,NULL);//T2A,W2A
	c_ConfigName[len]='\0';

	CString fullImagePath = currentPath + image_dir + L"\\";  

	GetImgRawbuf(fullImagePath+leftNearImgPath  ,&leftNearImg);
	GetImgRawbuf(fullImagePath+leftFarImgPath ,&leftFarImg);
	GetImgRawbuf(fullImagePath+rightNearImgPath ,&rightNearImg);
	GetImgRawbuf(fullImagePath+rightFarImgPath ,&rightFarImg);  

	int status;
	if(MainCamID==0)
	{

      status = IQC(gVerify_Items, leftNearImg.Raw_buffer, leftFarImg.Raw_buffer, leftFarImg.imgwidth, leftFarImg.imgheight, rightNearImg.Raw_buffer, rightFarImg.Raw_buffer, rightFarImg.imgwidth, rightFarImg.imgheight, c_ConfigName); 

	}else
	{
	  status = IQC(gVerify_Items, rightNearImg.Raw_buffer, rightFarImg.Raw_buffer, rightFarImg.imgwidth, rightFarImg.imgheight, leftNearImg.Raw_buffer, leftFarImg.Raw_buffer, leftFarImg.imgwidth, leftFarImg.imgheight, c_ConfigName); 

	}

	//if(status==1&&gVerify_Items[0]==0)   //1   10 
	if(gVerify_Items[1]==0&& gVerify_Items[10]==0)
	{
		Flag = 1;
	}
	else
	{ 
		Flag = -1;
	}

	CString strInfo;
	strInfo.Format(_T("status:%d,NearQulityLevel:%d,FarQulityLevel:%d,"),gVerify_Items[0],gVerify_Items[1],gVerify_Items[10]);
	m_pInterface->Ctrl_Addlog(CamID,strInfo,COLOR_BLUE,200);
	strInfo.Format(_T("NearMTK_Pattern:%d,FarMTK_Pattern:%d"),gVerify_Items[8],gVerify_Items[17]);
	m_pInterface->Ctrl_Addlog(CamID,strInfo,COLOR_BLUE,200);
	strInfo.Format(_T("NearIntensity:%d,FarIntesity:%d"),gVerify_Items[6],gVerify_Items[15]);
	m_pInterface->Ctrl_Addlog(CamID,strInfo,COLOR_BLUE,200);
	strInfo.Format(_T("NearTop:%d,Right:%d,Bottom:%d,Left:%d,"),gVerify_Items[2],gVerify_Items[3],gVerify_Items[4],gVerify_Items[5]);
	m_pInterface->Ctrl_Addlog(CamID,strInfo,COLOR_BLUE,200);
	strInfo.Format(_T("FarTop:%d,Right:%d,Bottom:%d,Left:%d,"),gVerify_Items[11],gVerify_Items[12],gVerify_Items[13],gVerify_Items[14]);
	m_pInterface->Ctrl_Addlog(CamID,strInfo,COLOR_BLUE,200);


	vecOutFlag.clear();
	for(int i=0;i<19;i++)
	{
		vecOutFlag.push_back(gVerify_Items[i]);
	}

	m_pInterface->Ctrl_Addlog(CamID,_T("MTK END"),COLOR_BLUE,200);
	SAFE_DELETE_ARRAY(gVerify_Items);
	ReleaseBuffer();

	return Flag;
}



int StereoCal_TestItem::Qualcomm_StereoCal()
{
#if 1
	ShellExecute(NULL, _T("open"), _T("qrun.bat"), _T(""), _T(""), SW_SHOW);
#else
	HINSTANCE handle = NULL;
	qcaldc_calibrate func_ptr = NULL;

	handle = LoadLibrary(L"Qualcomm\\qcaldc.dll");
	if (!handle)
		return 0;

	func_ptr = (qcaldc_calibrate)GetProcAddress(handle, "qcaldc_calibrate");
	if (!func_ptr)
		return 0;

	if (1 != func_ptr("Qualcomm\\qcaldc.cfg"))
	{
		FreeLibrary(handle);
		return 0;
	}

	if (handle)
		FreeLibrary(handle);
#endif
	return 0;
}

BOOL StereoCal_TestItem::GetImg_Info(CString srcImgPath,ImageInfo* img)
{
	CImage ScrImage;
	HRESULT h1= ScrImage.Load (srcImgPath);
	if(FAILED(h1))
	{
		AfxMessageBox(_T("打开失败"));
		return FALSE;
	}	
	//只支持宽度为4的
	if (ScrImage.GetPitch()%ScrImage.GetWidth() != 0 )
	{
		AfxMessageBox(_T("图像格式非规则，不能被4整除"));
		return FALSE;
	}

	int pitch  = ScrImage.GetPitch();
	int height = ScrImage.GetHeight();
	int width  = ScrImage.GetWidth();

	img->imgheight = height;
	img->imgwidth = width;
	img->RGB24_buffer = new BYTE[width*height*3];

	BYTE* pSrc = (BYTE*)ScrImage.GetBits();
	int k = 0;
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j< width; j++)
		{
			img->RGB24_buffer[k]   = pSrc[i*pitch + j*3]     ;
			img->RGB24_buffer[k+1] = pSrc[i*pitch + j*3 + 1] ;
			img->RGB24_buffer[k+2] = pSrc[i*pitch + j*3 + 2] ;
			k += 3;
		}
	} 
	//img->Y_buffer = new BYTE[width*height];
	//ImageProc::RGB24toY(img->RGB24_buffer, img->Y_buffer, width, height);	

	//删除临时BUFFER
	//SAFE_DELETE(img->RGB24_buffer);
	return TRUE;

}

BOOL StereoCal_TestItem::LoadImages()
{
	BOOL Flag = FALSE;
	Flag = GetImg_Info(leftFarImgPath,&leftFarImg);
	if(!Flag) return Flag;
	Flag = GetImg_Info(leftNearImgPath,&leftNearImg);
	if(!Flag) return Flag;
	Flag = GetImg_Info(rightFarImgPath,&rightFarImg);
	if(!Flag) return Flag;
	Flag = GetImg_Info(rightNearImgPath,&rightNearImg);
	if(!Flag) return Flag;
	return Flag;
}
BOOL StereoCal_TestItem::SetImagePoint_xy(ImageInfo img,vector< vector<fCPoint>> &ImagePoint_xy,CPoint &MarkPoint_ij)
{
	BOOL Flag = FALSE;
	int PointSizeLimit = 300;
	vector<float> Points_x;
	vector<float> Points_y;
	float MarkPoint_x;
	float MarkPoint_y;
	vector<fCPoint> Point_xy;

	Flag = ImageProc::CircleArrayPatternAnalysis(img.RGB24_buffer,img.imgwidth,img.imgheight,PointSizeLimit,Points_x,Points_y,MarkPoint_x,MarkPoint_y);
	if(!Flag) 
	{
		//SAFE_DELETE(img.RGB24_buffer);
		return Flag;
	}
	//SAFE_DELETE(img.RGB24_buffer);
	//vector< vector<fCPoint>> destPointArray;
	for(UINT i=0;i<Points_x.size();i++)
	{
		fCPoint fPoint;
		if(Points_x[i]>10&&Points_y[i]>10&&abs(Points_x[i]-img.imgwidth)>10&&abs(Points_y[i] - img.imgheight)>10)
		{
			fPoint.x = Points_x[i];
			fPoint.y = Points_y[i];
			Point_xy.push_back(fPoint);
		}
		
	}
	PointSort(Point_xy,ImagePoint_xy); 
	//destPointArray  中 按照从上到下，从左到右的顺序排列好
	vector<int> row_size;
	for(UINT i=0;i<ImagePoint_xy.size();i++)
	{
		row_size.push_back(ImagePoint_xy[i].size());
	}

	vector<int> rowSizeIndexDescend ,rowSizeIndex;
	int mutiItem = 0;// rowSizeIndexDescend[1];

	SortVecDescend(row_size,rowSizeIndexDescend,rowSizeIndex);
	mutiItem =  rowSizeIndexDescend[0]-1; 
	vector< vector<fCPoint>>::iterator it;// 
	BOOL bBegin = FALSE;
	BOOL bEnd = FALSE;

	//去掉头和尾少于目标个数的row
	for(it = ImagePoint_xy.begin();it != ImagePoint_xy.end();it++)
	{ 
		if(!bBegin)
		{
			it = ImagePoint_xy.begin();
			if(it->size()<mutiItem)
			{
				ImagePoint_xy.erase(it);  
			}
			else
			{
				bBegin = TRUE;
			}
		}

		if(!bEnd)
		{
			it = ImagePoint_xy.end()-1;
			if(it->size()<mutiItem)
			{
				ImagePoint_xy.erase(it);  
				it = ImagePoint_xy.begin();
			}
			else
			{
				bEnd = TRUE;
			}
		}
		if(bEnd&&bBegin)
		{ 
			break;
		}

	}

	for(it = ImagePoint_xy.begin();it != ImagePoint_xy.end();it++)
	{
		if(it->size()>mutiItem)
		{
			vector<fCPoint>::iterator row_it = it->begin();
			if(it->at(0).y<it->at(mutiItem).y)
			{
				row_it = it->end();
				row_it--; 
			}
			it->erase(row_it);
		}
		if(it->size()<3)
		{
			ImagePoint_xy.erase(it);
			it = ImagePoint_xy.begin();
		}
	}


	SetMarkPoint_ij(ImagePoint_xy,MarkPoint_ij);

	//对齐头      //和尾
	vector<int>	RowHeaderDif;
	vector<int> RowEndDif;
	BOOL bclockwise = TRUE;
	if(ImagePoint_xy[0][0].y>ImagePoint_xy[0][ImagePoint_xy[0].size()-1].y)  bclockwise = FALSE;
	float Pitch_x = ImagePoint_xy[0][1].x - ImagePoint_xy[0][0].x;
	if(bclockwise)
	{
		//对齐尾部
		for(it = ImagePoint_xy.begin();it != ImagePoint_xy.end()-1;it++)
		{				
			if(abs(it->at(it->size()-1).x-(it+1)->at((it+1)->size()-1).x)>Pitch_x/3)
			{
				(it+1)->erase((it+1)->end()-1);
			}			
		}
		//对齐tou部
		for(it = ImagePoint_xy.end()-1;it != ImagePoint_xy.begin();it--)
		{				
			if(abs(it->at(0).x-(it-1)->at(0).x)>Pitch_x/3)
			{
				(it-1)->erase((it-1)->begin());
				if(it-ImagePoint_xy.begin()== MarkPoint_ij.y+1)
				{
					MarkPoint_ij.x--;
				}
			}			
		}
	}
	else
	{
		//对齐tou部
		for(it = ImagePoint_xy.begin();it != ImagePoint_xy.end()-1;it++)
		{				
			if(abs(it->at(0).x-(it+1)->at(0).x)>Pitch_x/3)
			{
				(it+1)->erase((it+1)->begin());
				if(it-ImagePoint_xy.begin()== MarkPoint_ij.y-1)
				{
					MarkPoint_ij.x--;
				}
			}			
		}
		//对齐wei部
		for(it = ImagePoint_xy.end()-1;it != ImagePoint_xy.begin();it--)
		{				
			if(abs(it->at(it->size()-1).x-(it-1)->at((it-1)->size()-1).x)>Pitch_x/3)
			{
				(it-1)->erase((it-1)->end()-1);

			}			
		}
	}

	return TRUE;
}

int StereoCal_TestItem::GetPitchInfoOfArray(vector<float> srcData,float &minPitch,float &maxPitch)
{
	int dataSize = srcData.size();
	if(dataSize<2) return 0;
	minPitch = abs(srcData[1]-srcData[0]);
	maxPitch = abs(srcData[1]-srcData[0]);
	for(int i=1;i<dataSize-1;i++)
	{
		float difData = abs(srcData[i+1]-srcData[i]);
		if(minPitch>difData) minPitch = difData;
		if(maxPitch<difData) maxPitch = difData;
	}
	return 1;
}

int StereoCal_TestItem::GetMutiItemInVec(vector<int> vecItem,int &mutiItem)
{
	UINT ItemSize = vecItem.size();
	//map<int,int> ItemIndex;
	if(ItemSize<1) return 0;
	vector<int> DataIndex;
	int Index= 0;;
	for(UINT i=0;i<ItemSize;i++)
	{
		for(UINT j=0;j<ItemSize;j++)
		{
			if(vecItem[i] == vecItem[j])
			{
				Index++;
			}
		}
		//遍历完复位
		DataIndex.push_back(Index);
		//ItemIndex.insert(Index,i);
		Index = 0; 
	}
	int maxItem,maxItemIndex;
	vector<int> DescendVec,DescendVecIndex;
	SortVecDescend(DataIndex,DescendVec,DescendVecIndex);
	GetMaxInVec(DataIndex,maxItem,maxItemIndex); 

	mutiItem = vecItem[maxItemIndex-1];
	if(maxItem<ItemSize/2)
	{
		mutiItem = mutiItem-1;
	}
	return 1;
}

//如果有相同，取最后一项
void StereoCal_TestItem::GetMaxInVec(vector<int> vecItem,int &maxItem,int &Index)
{
	maxItem = vecItem[0];
	for(UINT i=1;i<vecItem.size();i++)
	{
		if(maxItem<vecItem[i])
		{
			maxItem = vecItem[i];
			Index = i;
		}
	}

}
//如果有相同，取最后一项
void StereoCal_TestItem::GetMinInVec(vector<int> vecItem,int &minItem,int &Index)
{
	minItem = vecItem[0];
	for(UINT i=1;i<vecItem.size();i++)
	{
		if(minItem>vecItem[i])
		{
			minItem = vecItem[i];
			Index = i;
		}
	}
}
//如果有相同，取最后一项
void StereoCal_TestItem::GetMaxInVec(vector<float> vecItem,float &maxItem,int &Index)
{
	maxItem = vecItem[0];
	for(UINT i=1;i<vecItem.size();i++)
	{
		if(maxItem<vecItem[i])
		{
			maxItem = vecItem[i];
			Index = i;
		}

	}

}
//如果有相同，取最后一项
void StereoCal_TestItem::GetMinInVec(vector<float> vecItem,float &minItem,int &Index)
{
	minItem = vecItem[0];
	for(UINT i=1;i<vecItem.size();i++)
	{
		if(minItem>vecItem[i])
		{
			minItem = vecItem[i];
			Index = i;
		}
	}
}

void StereoCal_TestItem::GetFirstDestItemIndexInVec(vector<int> vecItem,int destItem,int &Index)
{
	for(UINT i=0;i<vecItem.size();i++)
	{
		if(vecItem[i] == destItem)
		{
			Index = i;
			break;
		}
	}
}

int StereoCal_TestItem::CalibrationSingleCam(IN_PARAMS m_IN_PARAMS,DualCamData &m_DualCamData)
{ 
	OUT_PARAMS m_OUT_PARAMS;
	DualCCM_TwoLayerCalibrate(m_IN_PARAMS , &m_OUT_PARAMS);
	//ImageProc::CalibraCam(m_IN_PARAMS,&m_OUT_PARAMS );

	float ratioToAngle = (float)180/(float)3.1415926;
	m_DualCamData.Rotation = m_OUT_PARAMS.rotation_rad*ratioToAngle;
	m_DualCamData.TiltX =m_OUT_PARAMS.tilt_x_rad*ratioToAngle;
	m_DualCamData.TiltY = m_OUT_PARAMS.tilt_y_rad*ratioToAngle;

	m_DualCamData.shift_x = m_OUT_PARAMS.shift_x;
	m_DualCamData.shift_y =m_OUT_PARAMS.shift_y;
	m_DualCamData.shift_z= m_OUT_PARAMS.shift_z;

	m_DualCamData.OpticalCenter.x = m_OUT_PARAMS.opticalcenter_x_pixel;
	m_DualCamData.OpticalCenter.y = m_OUT_PARAMS.opticalcenter_y_pixel;
	return 0;
}

int StereoCal_TestItem::Cam3DCalibration()
{
	BOOL Flag = LoadImages();
	if(!Flag)
	{
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_Addlog(CamID,_T("图片加载失败"),COLOR_RED,200);
		return 0;
	}
	CPoint LeftFar_MarkPoint_ij,RightFar_MarkPoint_ij,LeftNear_MarkPoint_ij,RightNear_MarkPoint_ij;
	
	SetImagePoint_xy(leftFarImg, LeftFarImagePoint_xy,LeftFar_MarkPoint_ij);
	SetImagePoint_xy(leftNearImg, LeftNearImagePoint_xy,LeftNear_MarkPoint_ij);
	SetImagePoint_xy(rightFarImg, RightFarImagePoint_xy,RightFar_MarkPoint_ij);
	SetImagePoint_xy(rightNearImg, RightNearImagePoint_xy,RightNear_MarkPoint_ij);
	Sleep(200);
	CreateObjectArray(LeftNearImagePoint_xy,LeftNear_MarkPoint_ij,LeftObjectArray);
	Sleep(200);
	CreateObjectArray(RightNearImagePoint_xy,RightNear_MarkPoint_ij,RightObjectArray);
	Sleep(200);

	vector<CString> LeftNear_strKey,RightNear_strKey;


	map<CString,fCPoint> LeftFar_ImagePoint_ij_xy,RightFar_ImagePoint_ij_xy;
	LeftFar_ImagePoint_ij_xy.clear();
	RightFar_ImagePoint_ij_xy.clear();
	SetPoint_ij_xy(LeftFarImagePoint_xy,  LeftFar_MarkPoint_ij,LeftFar_ImagePoint_ij_xy);
	//SetPoint_ij_xy(LeftNearImagePoint_xy,  LeftNear_MarkPoint_ij,LeftNear_ImagePoint_ij_xy);
	GetPoint_ij_xyKey(LeftNearImagePoint_xy,LeftNear_MarkPoint_ij, LeftNear_strKey);
	SetPoint_ij_xy(RightFarImagePoint_xy,  RightFar_MarkPoint_ij,RightFar_ImagePoint_ij_xy);
	//SetPoint_ij_xy(RightNearImagePoint_xy,  RightNear_MarkPoint_ij,RightNear_ImagePoint_ij_xy);
	GetPoint_ij_xyKey(RightNearImagePoint_xy,RightNear_MarkPoint_ij, RightNear_strKey);
	/*CString str;
	LeftFar_ImagePoint_ij_xy.find(str)->first;
	LeftFar_ImagePoint_ij_xy.find(str)->second;*/


	IN_PARAMS m_CamPara[2];
	DualCamData m_DualCamData[2];

	int Left_imgPoint_xySize = LeftNearImagePoint_xy.size()*LeftNearImagePoint_xy[0].size()*2*2-2*2;//
	m_CamPara[0].focal_length_mm =_ttof(m_CamInfo[0].CamEFL) ;
	m_CamPara[0].imgPoint_xy = Left_imgPoint_xy;
	m_CamPara[0].imgPoint_xySize = Left_imgPoint_xySize;
	m_CamPara[0].img_height = leftNearImg.imgheight;
	m_CamPara[0].img_width = leftNearImg.imgwidth;
	m_CamPara[0].Oblect_xyz = Left_Oblect_xyz;
	m_CamPara[0].Oblect_xyzSize = Left_imgPoint_xySize*3/2;
	m_CamPara[0].pixel_size_um =  _ttof(m_CamInfo[0].CamPixelSize);

	int Right_imgPoint_xySize = RightNearImagePoint_xy.size()*RightNearImagePoint_xy[0].size()*2*2-2*2;
	m_CamPara[1].focal_length_mm = _ttof(m_CamInfo[1].CamEFL) ;
	m_CamPara[1].imgPoint_xy = Right_imgPoint_xy;
	m_CamPara[1].imgPoint_xySize = Right_imgPoint_xySize;
	m_CamPara[1].img_height = rightNearImg.imgheight;
	m_CamPara[1].img_width = rightNearImg.imgwidth;
	m_CamPara[1].Oblect_xyz = Right_Oblect_xyz;
	m_CamPara[1].Oblect_xyzSize = Right_imgPoint_xySize*3/2;
	m_CamPara[1].pixel_size_um = _ttof(m_CamInfo[1].CamPixelSize) ;
	 int objectPitch_x = m_ChartInfo.objectPitch_x;
	 int FarChartDist = m_ChartInfo.FarChartDist;
	 int NearChartDist = m_ChartInfo.NearChartDist;
	//Init 	Left_imgPoint_xy Left_Object_xyIndex
	int Left_imgPoint_xyIndex(0),Left_Object_xyIndex(0);
     	 
	CreateImgPoint_xy(LeftNearImagePoint_xy,LeftFar_ImagePoint_ij_xy,LeftNear_strKey,Left_imgPoint_xy); 
	CreateImgPoint_xy(RightNearImagePoint_xy,RightFar_ImagePoint_ij_xy,RightNear_strKey,Right_imgPoint_xy);
	CreateObject_xyz(LeftObjectArray,objectPitch_x,FarChartDist-NearChartDist,Left_Oblect_xyz);
	CreateObject_xyz(RightObjectArray,objectPitch_x,FarChartDist-NearChartDist,Right_Oblect_xyz);


	CalibrationSingleCam( m_CamPara[0],m_DualCamData[0]);
	CalibrationSingleCam( m_CamPara[1],m_DualCamData[1]);
	CString str;
	str.Format(_T("rota:%f"),m_DualCamData[1].Rotation-m_DualCamData[0].Rotation);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("tiltx:%f"),m_DualCamData[1].TiltX-m_DualCamData[0].TiltX);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("tilty:%f"),m_DualCamData[1].TiltY-m_DualCamData[0].TiltY);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

	
	//SAFE_DELETE(Left_Oblect_xyz);
	
	SAFE_DELETE_ARRAY(leftFarImg.RGB24_buffer);
	SAFE_DELETE_ARRAY(leftNearImg.RGB24_buffer);
	SAFE_DELETE_ARRAY(rightFarImg.RGB24_buffer);
	SAFE_DELETE_ARRAY(rightNearImg.RGB24_buffer);


	return 1;

}

void StereoCal_TestItem::CreateObjectArray(vector< vector<fCPoint>> ImagePoint_xy,CPoint MarkPoint_ij,vector< vector<CPoint>> &ObjectArray)
{
	UINT RowConut = ImagePoint_xy.size();
	vector<CPoint> rowObject;
	ObjectArray.clear();
	for(UINT i=0;i<RowConut;i++)
	{
		for(UINT j=0;j<ImagePoint_xy[i].size();j++)
		{
			CPoint Object_xy;
			if (i == MarkPoint_ij.y&& j>MarkPoint_ij.x-1)
			{
				Object_xy.x = (j+1);
				Object_xy.y= i;
			}
			else
			{
				Object_xy.x = j;
				Object_xy.y= i;
			}

			rowObject.push_back(Object_xy);
		}
		ObjectArray.push_back(rowObject);
		rowObject.clear();
	}
}

int StereoCal_TestItem::SetPoint_ij_xy(vector< vector<fCPoint>> ImagePoint_xy,CPoint MarkPoint_ij,map<CString,fCPoint> &ImagePoint_ij_xy)
{
	UINT RowConut = ImagePoint_xy.size();
	vector<CPoint> rowObject;
	CPoint Point_ij;
	for(UINT i=0;i<RowConut;i++)
	{
		for(UINT j=0;j<ImagePoint_xy[i].size();j++)
		{
			Point_ij.x = j-MarkPoint_ij.x;
			Point_ij.y = i-MarkPoint_ij.y;
			CString str;
			str.Format(_T("ij_%d,%d"),Point_ij.x,Point_ij.y);
			ImagePoint_ij_xy.insert(pair<CString,fCPoint>(str,ImagePoint_xy[i][j]));
		}
	
	} 
	return 1;
}

int StereoCal_TestItem::GetPoint_ij_xyKey(vector< vector<fCPoint>> ImagePoint_xy,CPoint MarkPoint_ij,vector< CString> &strKey)
{
	UINT RowConut = ImagePoint_xy.size();
	UINT ColunCount = ImagePoint_xy[0].size();
	vector<CPoint> rowObject;
	CPoint Point_ij;
	for(UINT i=0;i<RowConut;i++)
	{
		for(UINT j=0;j<ImagePoint_xy[i].size();j++)
		{ 

			Point_ij.x = j-MarkPoint_ij.x;
			Point_ij.y = i-MarkPoint_ij.y;

			CString str;
			str.Format(_T("ij_%d,%d"),Point_ij.x,Point_ij.y);
			strKey.push_back(str);
		}
		 
	}
	return 1;
}

int StereoCal_TestItem::GetMaxMin_xyInPointArray(vector<fCPoint> srcPointArray,fCPoint &min_xPoint,fCPoint &max_xPoint,fCPoint &min_yPoint,fCPoint &max_yPoint)
{
	if(srcPointArray.size()<1) return 0;
	min_xPoint.x = srcPointArray[0].x;
	min_xPoint.y = srcPointArray[0].y;
	max_xPoint.x = min_xPoint.x;
	max_xPoint.y = min_xPoint.y;
	min_yPoint.y = srcPointArray[0].y;
	min_yPoint.x = srcPointArray[0].x;
	max_yPoint.y = min_yPoint.y;
	max_yPoint.x = min_yPoint.x;
	for(UINT i=0;i<srcPointArray.size();i++)
	{
		if(min_xPoint.x>srcPointArray[i].x)
		{
			min_xPoint.x = srcPointArray[i].x;
			min_xPoint.y = srcPointArray[i].y;
		}

		if(max_xPoint.x<srcPointArray[i].x)
		{
			max_xPoint.x = srcPointArray[i].x;
			max_xPoint.y = srcPointArray[i].y;
		}

		if(min_yPoint.y>srcPointArray[i].y)
		{
			min_yPoint.y = srcPointArray[i].y;
			min_yPoint.x = srcPointArray[i].x;
		}
		if(max_yPoint.y<srcPointArray[i].y)
		{
			max_yPoint.y = srcPointArray[i].y;
			max_yPoint.x = srcPointArray[i].x;
		} 
	}
	return 1;
}
void StereoCal_TestItem::GetRowPoint(vector<fCPoint> &srcPointArray,int rowLimit,vector<fCPoint> &destPointRow)
{
	destPointRow.clear();
	if(srcPointArray.size()==1)  
	{
		destPointRow.push_back(srcPointArray[0]);
		srcPointArray.erase(srcPointArray.begin());
		return;
	}
	
	fCPoint min_x,max_x,min_y,max_y; 
	GetMaxMin_xyInPointArray( srcPointArray,min_x,max_x,min_y,max_y); 
	UINT arraySize = srcPointArray.size();
	fCPoint CurrentPoint;//得到当前点的坐标
	CurrentPoint=min_y;
	BOOL bEnd = FALSE;
	while(!bEnd)
	{ 
		UINT destArraySize = destPointRow.size();

		if(destArraySize!=0)
		{ 
			CurrentPoint=destPointRow[destArraySize-1];
		}
		vector<int> vecDist,vecAscendOrder,PointIndexVec,DistIndexVec;
		GetPointDist(srcPointArray,CurrentPoint,vecDist,max_y.y,PointIndexVec);//得到距离序列
		SortVecAscend( vecDist,vecAscendOrder,DistIndexVec);//升序排列

		int LimitSize = 0;
		BOOL minFlag[3] = {FALSE};
		int tempIndex[3] = {0};
		for(UINT i = 0;i<vecDist.size();i++)
		{
			if(i==3)
				break;
			tempIndex[i] = PointIndexVec[DistIndexVec[i]];
			LimitSize = i+1;
		}
		if(vecAscendOrder.size()>1)
		{
			if(rowLimit==0 && vecAscendOrder[0]==0) 
				rowLimit = vecAscendOrder[1]/2;
		}

		//根据距离当前点最近的三个点的分布判断当前点的位置情况  （起始，中间点，终点），并且找到相邻的那个点，

		vector<float> SameRowVec_x;
		vector<int> topRow,currentRow,nextRow;
		for(int j=0;j<LimitSize;j++)
		{
			float xdif = CurrentPoint.x - srcPointArray[tempIndex[j]].x;
			float ydif = CurrentPoint.y - srcPointArray[tempIndex[j]].y;
			if(ydif>rowLimit)//还有上面的行
			{
				topRow.push_back(j);
			}
			else if(abs(ydif)<rowLimit/3&&abs(xdif)<rowLimit*5&&(abs(xdif)>rowLimit*1.5||abs(xdif)==0))//与当前点同行
			{
				currentRow.push_back(j);
				SameRowVec_x.push_back(srcPointArray[tempIndex[j]].x);
			}
			else if(abs(ydif)>rowLimit)//在当前点的下一行
			{
				nextRow.push_back(j);
			}

		}	

		vector<fCPoint>::iterator it_src;

		if(topRow.size()>0)//需要重新找firstPoint
		{
			if(destPointRow.size()>0)
			{				
				//还回去
				vector<fCPoint>::iterator src_it = srcPointArray.end();								
				srcPointArray.insert(src_it,destPointRow.begin(),destPointRow.end());
				destPointRow.clear();
			}
			CurrentPoint = srcPointArray[tempIndex[topRow[0]]];
		}
		else if(currentRow.size()>0)
		{
			vector<float> AsendSameRowVec_x;
			vector<int>   Index_AsendSameRowVec_x;
			SortVecAscend(SameRowVec_x,AsendSameRowVec_x,Index_AsendSameRowVec_x);//对同一列的进行排序
			
			if(CurrentPoint.x <= srcPointArray[tempIndex[currentRow[Index_AsendSameRowVec_x[0]]]].x)//把同列的点都按顺序装进去
			{
				for(UINT i=0;i<Index_AsendSameRowVec_x.size();i++)
				{		
					it_src = srcPointArray.begin();
					int TempIndex = tempIndex[currentRow[Index_AsendSameRowVec_x[i]]]-i;
					if(TempIndex>-1&&srcPointArray.size()>TempIndex)
					{
						destPointRow.push_back(srcPointArray[TempIndex]);
						for(int j = 0;j<tempIndex[currentRow[Index_AsendSameRowVec_x[i]]]-i;j++)//当前点在最开始的Index;
						{
							it_src++;
						}

					}
					else
					{
						destPointRow.push_back(srcPointArray[0]);
					}
					
					srcPointArray.erase(it_src); 
				}
			}
			else
			{
				CurrentPoint = srcPointArray[tempIndex[Index_AsendSameRowVec_x[0]]];
				for(int row_i = 0;row_i<destPointRow.size();row_i++)
				{
					srcPointArray.push_back(destPointRow[row_i]);
						 
				}
				destPointRow.clear();
			}	

		}
		else
		{
			break;
		}

	}
}

int StereoCal_TestItem::PointSort(vector<fCPoint> srcPointArray,vector< vector<fCPoint>> &destPointArray)
{
	vector<fCPoint> destPointRow;
	vector<fCPoint> srcPointArrayTemp = srcPointArray;
	UINT tempSize = srcPointArrayTemp.size();
	int RowLimit = 0;
	while(tempSize)
	{				
		GetRowPoint(srcPointArrayTemp,0,destPointRow);
		if(destPointRow.size()>0)
		{
			destPointArray.push_back(destPointRow); 
		}
		
		/*if(destPointArray.size()==28)
		{
		tempSize = 1;
		}*/
		tempSize = srcPointArrayTemp.size();	
	}
	return 1;
}
int StereoCal_TestItem::GetPointDist(vector<fCPoint> srcPointArray,fCPoint originPoint,vector<int> &vecDist,int maxLimit,vector<int> &Index)
{
	//太远的丢掉不计算

	for(UINT i=0;i<srcPointArray.size();i++)
	{
		int x_dif = abs((int)srcPointArray[i].x - (int)originPoint.x);
		int y_dif = abs((int)srcPointArray[i].y - (int)originPoint.y);
		if(x_dif>maxLimit/5||y_dif>maxLimit/5)
		{
			continue;
		}
		int Dist = (int)sqrt((float)(x_dif*x_dif + y_dif*y_dif));
		vecDist.push_back(Dist);
		Index.push_back(i);
	}
	return 1;
}

//升序排列
void StereoCal_TestItem::SortVecAscend(vector<int> vecItem,vector<int> &vecAscendOrder,vector<int> &Index)
{
	if(vecItem.size()<1) return;
	int flag = 1;
	vecAscendOrder.clear();
	Index.clear();
	vecAscendOrder = vecItem;
	for(UINT i = 0;i<vecAscendOrder.size();i++)
	{
		Index.push_back(i);
	}
	while(1)
	{
		for(UINT i=0;i<vecAscendOrder.size()-1;i++)
		{
			if(vecAscendOrder[i]>vecAscendOrder[i+1])
			{
				int temp = vecAscendOrder[i];
				int tempIndex = Index[i];
				vecAscendOrder[i] = vecAscendOrder[i+1];
				vecAscendOrder[i+1] = temp; 

				Index[i] = Index[i+1];
				Index[i+1] = tempIndex; 

				flag = 0;

			}
			else
			{
				flag = flag*flag;				
			}

		}
		if(flag==0)
		{
			flag = 1;
		}
		else
		{
			break;
		}
	}

}
//降序排列
void StereoCal_TestItem::SortVecDescend(vector<int> vecItem,vector<int> &vecDescendOrder,vector<int> &Index)
{
	if(vecItem.size()<1) return;
	int flag = 1;
	vecDescendOrder.clear();
	Index.clear();
	vecDescendOrder = vecItem;
	for(UINT i = 0;i<vecDescendOrder.size();i++)
	{
		Index.push_back(i);
	}
	while(1)
	{
		for(UINT i=0;i<vecDescendOrder.size()-1;i++)
		{
			if(vecDescendOrder[i]<vecDescendOrder[i+1])
			{
				int temp = vecDescendOrder[i];
				int tempIndex = Index[i];
				vecDescendOrder[i] = vecDescendOrder[i+1];
				vecDescendOrder[i+1] = temp; 

				Index[i] = Index[i+1];
				Index[i+1] = tempIndex; 

				flag = 0;

			}
			else
			{
				flag = flag*flag;				
			}

		}
		if(flag==0)
		{
			flag = 1;
		}
		else
		{
			break;
		}
	}

}

void StereoCal_TestItem::SortVecAscend(vector<float> vecItem,vector<float> &vecAscendOrder,vector<int> &Index)
{
	if(vecItem.size()<1) return;
	int flag = 1;
	vecAscendOrder.clear();
	Index.clear();
	vecAscendOrder = vecItem;
	for(UINT i = 0;i<vecAscendOrder.size();i++)
	{
		Index.push_back(i);
	}
	while(1)
	{
		for(UINT i=0;i<vecAscendOrder.size()-1;i++)
		{
			if(vecAscendOrder[i]>vecAscendOrder[i+1])
			{
				int temp = vecAscendOrder[i];
				int tempIndex = Index[i];
				vecAscendOrder[i] = vecAscendOrder[i+1];
				vecAscendOrder[i+1] = temp; 

				Index[i] = Index[i+1];
				Index[i+1] = tempIndex; 

				flag = 0;

			}
			else
			{
				flag = flag*flag;				
			}

		}
		if(flag==0)
		{
			flag = 1;
		}
		else
		{
			break;
		}
	}

}
//降序排列
void StereoCal_TestItem::SortVecDescend(vector<float> vecItem,vector<float> &vecDescendOrder,vector<int> &Index)
{
	if(vecItem.size()<1) return;
	int flag = 1;
	vecDescendOrder.clear();
	Index.clear();
	vecDescendOrder = vecItem;
	for(UINT i = 0;i<vecDescendOrder.size();i++)
	{
		Index.push_back(i);
	}
	while(1)
	{
		for(UINT i=0;i<vecDescendOrder.size()-1;i++)
		{
			if(vecDescendOrder[i]<vecDescendOrder[i+1])
			{
				int temp = vecDescendOrder[i];
				int tempIndex = Index[i];
				vecDescendOrder[i] = vecDescendOrder[i+1];
				vecDescendOrder[i+1] = temp; 

				Index[i] = Index[i+1];
				Index[i+1] = tempIndex; 

				flag = 0;

			}
			else
			{
				flag = flag*flag;				
			}

		}
		if(flag==0)
		{
			flag = 1;
		}
		else
		{
			break;
		}
	}
}

void StereoCal_TestItem::GetDxoImgPoint(ImageInfo NearImg,ImageInfo FarImg,vector< vector<fCPoint>> &NearImgPoint,vector< vector<fCPoint>> &FarImgPoint)
{
	// Local iconic variables 
	Hobject  Image, GrayImage, ImageMean, ImageSub;
	Hobject  ImageEmphasize, Rectangle, ImageReduced, Region;
	Hobject  ConnectedRegions, RegionClosing, RegionTrans, RegionUnion;
	Hobject  ConnectedRegionsPoint, SelectedRegionsMax, SelectedRegions;
	Hobject  RegionLines, RegionTmp, SortedRegions;


	// Local control variables 
	HTuple  POINT_CIRCULARITY, POINT_AREA, POINT_MASK_DILATION;
	HTuple  POINT_DISTANCE, POINT_D, POINT_MASK_D, ImageFiles;
	HTuple  Index, Width, Height, WindowHandle, Row1, Column1;
	HTuple  Row2, Column2, Area, Row, Column, pxbymm, deltaH;
	HTuple  AreaLine, RowLine, ColumnLine, Length, Phi, rowNum;
	HTuple  rowUpNum, colNum, colLeftNum, firstPointRow, firstPointCol;
	HTuple  rowTotalPointNum, tmpPosi, AreaPoint, RowPoint;
	HTuple  ColumnPoint, colTotalPointNum;

	POINT_CIRCULARITY = 0.8;
	POINT_AREA = 100;
	POINT_MASK_DILATION = 40.5;
	POINT_DISTANCE = 26;
	POINT_D = 8;
	POINT_MASK_D = 20;

	// dev_update_off(...); only in hdevelop
	//Code generated by Image Acquisition 01
	/*list_files("E:/Work/Temp/DXO", (HTuple("files").Append("follow_links")), &ImageFiles);
	tuple_regexp_select(ImageFiles, (HTuple("\\.(tif|tiff|gif|bmp|jpg|jpeg|jp2|png|pcx|pgm|ppm|pbm|xwd|ima)$").Append("ignore_case")), 
	&ImageFiles);
	for (Index=0; Index<=(ImageFiles.Num())-1; Index+=1)
	{*/
	//read_image(&Image, HTuple(ImageFiles[Index]));

	unsigned char * pBmpRed = (unsigned char *)malloc(NearImg.imgwidth * NearImg.imgheight);
	unsigned char * pBmpGreen = (unsigned char *)malloc(NearImg.imgwidth * NearImg.imgheight);
	unsigned char * pBmpBlue = (unsigned char *)malloc(NearImg.imgwidth * NearImg.imgheight);


	for (int i=0; i<NearImg.imgheight; i++)
	{
		for (int j=0; j<NearImg.imgwidth; j++)
		{
			pBmpBlue[i*NearImg.imgwidth+j] = NearImg.RGB24_buffer[(i*NearImg.imgwidth+j)*3];
			pBmpGreen[i*NearImg.imgwidth+j] = NearImg.RGB24_buffer[(i*NearImg.imgwidth+j)*3+1];
			pBmpRed[i*NearImg.imgwidth+j] = NearImg.RGB24_buffer[(i*NearImg.imgwidth+j)*3+2];
		}
	}


	gen_image3_extern(&Image, "byte", NearImg.imgwidth, NearImg.imgheight, (long)pBmpRed, (long)pBmpGreen, (long)pBmpBlue, (long)free);

	//read_image(&Image, HTuple(NearImgPath));
	//Do something
	get_image_size(Image, &Width, &Height);
	/* if (HDevWindowStack::IsOpen())
	close_window(HDevWindowStack::Pop());
	set_window_attr("background_color","black");
	open_window(0,0,Width/4,Height/4,0,"","",&WindowHandle);
	HDevWindowStack::Push(WindowHandle);
	if (HDevWindowStack::IsOpen())
	disp_obj(Image, HDevWindowStack::GetActive());*/
	rgb1_to_gray(Image, &GrayImage);
	mean_image(GrayImage, &ImageMean, Width/4, Height/4);
	sub_image(GrayImage, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 1);

	gen_rectangle2(&Rectangle, Height/2, Width/2, 0, Width/4, Height/4);
	reduce_domain(ImageEmphasize, Rectangle, &ImageReduced);
	bin_threshold(ImageReduced, &Region);
	connection(Region, &ConnectedRegions);
	closing_circle(ConnectedRegions, &RegionClosing, 3.5);
	shape_trans(RegionClosing, &RegionTrans, "rectangle1");
	union1(RegionTrans, &RegionUnion);
	connection(RegionUnion, &ConnectedRegionsPoint);
	select_shape_std(ConnectedRegionsPoint, &SelectedRegionsMax, "max_area", 70);
	smallest_rectangle1(SelectedRegionsMax, &Row1, &Column1, &Row2, &Column2);
	area_center(SelectedRegionsMax, &Area, &Row, &Column);

	pxbymm = ((Column2-Column1)*1.0)/POINT_MASK_D;
	deltaH = pxbymm*POINT_DISTANCE;
	gen_rectangle2(&Rectangle, Row, Column, 0, Width, deltaH/2);
	reduce_domain(ImageReduced, Rectangle, &ImageReduced);
	bin_threshold(ImageReduced, &Region);
	connection(Region, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, (HTuple("area").Append("circularity")), 
		"and", POINT_AREA.Concat(POINT_CIRCULARITY), (HTuple(999999).Append(1)));
	area_center(SelectedRegions, &AreaLine, &RowLine, &ColumnLine);
	tuple_length(AreaLine, &Length);
	gen_region_line(&RegionLines, HTuple(RowLine[0]), HTuple(ColumnLine[0]), HTuple(RowLine[Length-1]), 
		HTuple(ColumnLine[Length-1]));
	orientation_region(RegionLines, &Phi);

	rowNum = Height/deltaH;
	tuple_int(rowNum, &rowNum);
	rowUpNum = Row/deltaH;
	tuple_int(rowUpNum, &rowUpNum);
	colNum = Width/deltaH;
	tuple_int(colNum, &colNum);
	colLeftNum = Column/deltaH;
	tuple_int(colLeftNum, &colLeftNum);

	firstPointRow = 0;
	firstPointCol = 0;
	rowTotalPointNum = rowNum-2;
	for (Index=2; Index<=rowNum-1; Index+=1)
	{
		vector<fCPoint> rowPoint;
		tmpPosi = ((rowUpNum-Index)+1)*deltaH;
		gen_rectangle2(&Rectangle, Row-tmpPosi, Column, Phi, (Width-(deltaH*3))/2, 
			deltaH/2);

		reduce_domain(ImageEmphasize, Rectangle, &ImageReduced);
		bin_threshold(ImageReduced, &RegionTmp);

		connection(RegionTmp, &ConnectedRegions);
		sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "column");
		area_center(SortedRegions, &AreaPoint, &RowPoint, &ColumnPoint);

		HTuple RowPointLength,ColumnPointLength;
		tuple_length(RowPoint,&RowPointLength);
		tuple_length(ColumnPoint,&ColumnPointLength);
		for(HTuple PointIndex=0;PointIndex<RowPointLength;PointIndex++)
		{
			fCPoint imgPoint;
			imgPoint.y = RowPoint[PointIndex].D();
			imgPoint.x = ColumnPoint[PointIndex].D();

			rowPoint.push_back(imgPoint);
		}


		tuple_length(AreaPoint, &colTotalPointNum);
		if (0 != (Index==2))
		{
			firstPointRow = RowPoint[0];
			firstPointCol = ColumnPoint[0];
		}
		NearImgPoint.push_back(rowPoint);
	}
	rowUpNum = (Row-firstPointRow)/deltaH;
	tuple_int(rowUpNum, &rowUpNum);
	colLeftNum = (Column-firstPointCol)/deltaH;
	tuple_int(colLeftNum, &colLeftNum);
	// stop(); only in hdevelop
	unsigned char * pFarBmpRed = (unsigned char *)malloc(FarImg.imgwidth * FarImg.imgheight);
	unsigned char * pFarBmpGreen = (unsigned char *)malloc(FarImg.imgwidth * FarImg.imgheight);
	unsigned char * pFarBmpBlue = (unsigned char *)malloc(FarImg.imgwidth * FarImg.imgheight);


	for (int i=0; i<FarImg.imgheight; i++)
	{
		for (int j=0; j<NearImg.imgwidth; j++)
		{
			pFarBmpRed[i*NearImg.imgwidth+j] = FarImg.RGB24_buffer[(i*FarImg.imgwidth+j)*3];
			pFarBmpGreen[i*NearImg.imgwidth+j] = FarImg.RGB24_buffer[(i*FarImg.imgwidth+j)*3+1];
			pFarBmpBlue[i*NearImg.imgwidth+j] = FarImg.RGB24_buffer[(i*FarImg.imgwidth+j)*3+2];
		}
	}


	gen_image3_extern(&Image, "byte", FarImg.imgwidth, FarImg.imgheight, (long)pFarBmpRed, (long)pFarBmpGreen, (long)pFarBmpBlue, (long)free);

	/* read_image(&Image, "E:/Work/Temp/DXO/20151230-141121Cam0.jpg");*/
	get_image_size(Image, &Width, &Height);
	/*   if (HDevWindowStack::IsOpen())
	close_window(HDevWindowStack::Pop());
	set_window_attr("background_color","black");
	open_window(0,0,Width/4,Height/4,0,"","",&WindowHandle);
	HDevWindowStack::Push(WindowHandle);
	if (HDevWindowStack::IsOpen())
	disp_obj(Image, HDevWindowStack::GetActive());*/
	rgb1_to_gray(Image, &GrayImage);
	mean_image(GrayImage, &ImageMean, Width/4, Height/4);
	sub_image(GrayImage, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 1);

	gen_rectangle2(&Rectangle, Height/2, Width/2, 0, Width/4, Height/4);
	reduce_domain(ImageEmphasize, Rectangle, &ImageReduced);
	bin_threshold(ImageReduced, &Region);
	connection(Region, &ConnectedRegions);
	closing_circle(ConnectedRegions, &RegionClosing, 3.5);
	shape_trans(RegionClosing, &RegionTrans, "rectangle1");
	union1(RegionTrans, &RegionUnion);
	connection(RegionUnion, &ConnectedRegionsPoint);
	select_shape_std(ConnectedRegionsPoint, &SelectedRegionsMax, "max_area", 70);
	smallest_rectangle1(SelectedRegionsMax, &Row1, &Column1, &Row2, &Column2);
	area_center(SelectedRegionsMax, &Area, &Row, &Column);

	pxbymm = ((Column2-Column1)*1.0)/POINT_MASK_D;
	deltaH = pxbymm*POINT_DISTANCE;
	gen_rectangle2(&Rectangle, Row, Column, 0, Width, deltaH/2);
	reduce_domain(ImageReduced, Rectangle, &ImageReduced);
	bin_threshold(ImageReduced, &Region);
	connection(Region, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, (HTuple("area").Append("circularity")), 
		"and", POINT_AREA.Concat(POINT_CIRCULARITY), (HTuple(999999).Append(1)));
	area_center(SelectedRegions, &AreaLine, &RowLine, &ColumnLine);
	tuple_length(AreaLine, &Length);
	gen_region_line(&RegionLines, HTuple(RowLine[0]), HTuple(ColumnLine[0]), HTuple(RowLine[Length-1]), 
		HTuple(ColumnLine[Length-1]));
	orientation_region(RegionLines, &Phi);

	for (Index=1; Index<=rowTotalPointNum; Index+=1)
	{
		tmpPosi = ((rowUpNum-Index)+1)*deltaH;
		gen_rectangle2(&Rectangle, Row-tmpPosi, Column, Phi, (deltaH*colTotalPointNum)/2, 
			deltaH/2);

		reduce_domain(ImageEmphasize, Rectangle, &ImageReduced);
		bin_threshold(ImageReduced, &RegionTmp);

		connection(RegionTmp, &ConnectedRegions);
		sort_region(ConnectedRegions, &SortedRegions, "first_point", "true", "row");
		area_center(SortedRegions, &AreaPoint, &RowPoint, &ColumnPoint);
		tuple_length(AreaPoint, &colTotalPointNum);
		if (0 != (Index==2))
		{
			firstPointRow = RowPoint[0];
			firstPointCol = ColumnPoint[0];
		}
	}
	// stop(); only in hdevelop

	Image.~Hobject();
	GrayImage.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Rectangle.~Hobject();
	ImageReduced.~Hobject();
	Region.~Hobject();
	ConnectedRegions.~Hobject();
	RegionClosing.~Hobject();
	RegionTrans.~Hobject();
	RegionUnion.~Hobject();
	ConnectedRegionsPoint.~Hobject();
	SelectedRegionsMax.~Hobject();
	SelectedRegions.~Hobject();
	RegionLines.~Hobject();
	RegionTmp.~Hobject();
	SortedRegions.~Hobject(); 
}

int StereoCal_TestItem::CamDxoCalibration()
{	
	BOOL Flag = LoadImages();
	if(!Flag)
	{
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_Addlog(CamID,_T("图片加载失败"),COLOR_RED,200);
		return 0;
	}

	GetDxoImgPoint(leftNearImg,leftFarImg,LeftNearImagePoint_xy,LeftFarImagePoint_xy);
	GetDxoImgPoint(rightNearImg,rightFarImg,RightNearImagePoint_xy,RightFarImagePoint_xy);

	CPoint LeftNear_MarkPoint_ij,RightNear_MarkPoint_ij;
	SetMarkPoint_ij(LeftFarImagePoint_xy,LeftNear_MarkPoint_ij);
	SetMarkPoint_ij(RightFarImagePoint_xy,RightNear_MarkPoint_ij);

	CreateObjectArray(LeftNearImagePoint_xy,LeftNear_MarkPoint_ij,LeftObjectArray);
	Sleep(200);
	CreateObjectArray(RightNearImagePoint_xy,RightNear_MarkPoint_ij,RightObjectArray);
	Sleep(200);

	vector<CString> LeftNear_strKey,RightNear_strKey;

	IN_PARAMS m_CamPara[2];
	DualCamData m_DualCamData[2];

	int Left_imgPoint_xySize = LeftNearImagePoint_xy.size()*LeftNearImagePoint_xy[0].size()*2*2-2*2;//
	float* Left_imgPoint_xy = new float[Left_imgPoint_xySize+100];
	float* Left_Oblect_xyz = new float[LeftNearImagePoint_xy.size()/2*3+100];
	m_CamPara[0].focal_length_mm = 3.47;
	m_CamPara[0].imgPoint_xy = Left_imgPoint_xy;
	m_CamPara[0].imgPoint_xySize = Left_imgPoint_xySize;
	m_CamPara[0].img_height = leftNearImg.imgheight;
	m_CamPara[0].img_width = leftNearImg.imgwidth;
	m_CamPara[0].Oblect_xyz = Left_Oblect_xyz;
	m_CamPara[0].Oblect_xyzSize = Left_imgPoint_xySize*3/2;
	m_CamPara[0].pixel_size_um = 1.12;

	int Right_imgPoint_xySize = RightNearImagePoint_xy.size()*RightNearImagePoint_xy[0].size()*2*2-2*2;
	float* Right_imgPoint_xy = new float[Right_imgPoint_xySize+100];
	float* Right_Oblect_xyz = new float[Right_imgPoint_xySize/2*3+100];
	m_CamPara[1].focal_length_mm = 3.47;
	m_CamPara[1].imgPoint_xy = Right_imgPoint_xy;
	m_CamPara[1].imgPoint_xySize = Right_imgPoint_xySize;
	m_CamPara[1].img_height = rightNearImg.imgheight;
	m_CamPara[1].img_width = rightNearImg.imgwidth;
	m_CamPara[1].Oblect_xyz = Right_Oblect_xyz;
	m_CamPara[1].Oblect_xyzSize = Right_imgPoint_xySize*3/2;
	m_CamPara[1].pixel_size_um = 1.12;
	int objectPitch_x = 26;
	int FarChartDist(500),NearChartDist(200);
	//Init 	Left_imgPoint_xy Left_Object_xyIndex
	int Left_imgPoint_xyIndex(0),Left_Object_xyIndex(0);
	for(UINT i = 0;i<LeftNearImagePoint_xy.size();i++)
	{
		for(UINT j = 0;j<LeftNearImagePoint_xy[i].size();j++)
		{ 
			Left_imgPoint_xy[Left_imgPoint_xyIndex] = LeftNearImagePoint_xy[i][j].x;
			Left_imgPoint_xy[Left_imgPoint_xyIndex+1] = LeftNearImagePoint_xy[i][j].y;
			Left_imgPoint_xyIndex+=2;
			Left_Oblect_xyz[Left_Object_xyIndex] = LeftObjectArray[i][j].x*objectPitch_x;
			Left_Oblect_xyz[Left_Object_xyIndex+1] = LeftObjectArray[i][j].y*objectPitch_x;
			Left_Oblect_xyz[Left_Object_xyIndex+2] = 0;
			Left_Object_xyIndex+=3;
		}
	}


	int KeyIndex(0);
	for(UINT i = 0;i<LeftNearImagePoint_xy.size();i++)
	{
		for(UINT j = 0;j<LeftNearImagePoint_xy[i].size();j++)
		{ 
			Left_imgPoint_xy[Left_imgPoint_xyIndex] = LeftFarImagePoint_xy[i][j].x;
			Left_imgPoint_xy[Left_imgPoint_xyIndex+1] = RightFarImagePoint_xy[i][j].y;
			KeyIndex++;
			Left_imgPoint_xyIndex+=2;
			Left_Oblect_xyz[Left_Object_xyIndex] = LeftObjectArray[i][j].x*objectPitch_x;
			Left_Oblect_xyz[Left_Object_xyIndex+1] = LeftObjectArray[i][j].y*objectPitch_x;
			Left_Oblect_xyz[Left_Object_xyIndex+2] = FarChartDist-NearChartDist;
			Left_Object_xyIndex+=3;
		}
	} 

	//Init 	Right_imgPoint_xy

	int Right_imgPoint_xyIndex(0),Right_Object_xyIndex(0);
	for(UINT i = 0;i<RightNearImagePoint_xy.size();i++)
	{
		for(UINT j = 0;j<RightNearImagePoint_xy[i].size();j++)
		{ 
			Right_imgPoint_xy[Right_imgPoint_xyIndex] = RightNearImagePoint_xy[i][j].x;
			Right_imgPoint_xy[Right_imgPoint_xyIndex+1] = RightNearImagePoint_xy[i][j].y;
			Right_imgPoint_xyIndex+=2;
			Right_Oblect_xyz[Right_Object_xyIndex] = RightObjectArray[i][j].x*objectPitch_x;
			Right_Oblect_xyz[Right_Object_xyIndex+1] = RightObjectArray[i][j].y*objectPitch_x;
			Right_Oblect_xyz[Right_Object_xyIndex+2] = 0;
			Right_Object_xyIndex+=3;
		}
	}
	KeyIndex = 0;
	for(UINT i = 0;i<RightNearImagePoint_xy.size();i++)
	{
		for(UINT j = 0;j<RightNearImagePoint_xy[i].size();j++)
		{ 
			Right_imgPoint_xy[Right_imgPoint_xyIndex] = RightFarImagePoint_xy[i][j].x;
			Right_imgPoint_xy[Right_imgPoint_xyIndex+1] = RightFarImagePoint_xy[i][j].y;
			KeyIndex++;
			Right_imgPoint_xyIndex+=2;
			Right_Oblect_xyz[Right_Object_xyIndex] = RightObjectArray[i][j].x*objectPitch_x;
			Right_Oblect_xyz[Right_Object_xyIndex+1] = RightObjectArray[i][j].y*objectPitch_x;
			Right_Oblect_xyz[Right_Object_xyIndex+2] = FarChartDist-NearChartDist;
			Right_Object_xyIndex+=3;
		}
	}

	CalibrationSingleCam( m_CamPara[0],m_DualCamData[0]);
	CalibrationSingleCam( m_CamPara[1],m_DualCamData[1]);
	CString str;
	str.Format(_T("rota:%f"),m_DualCamData[1].Rotation-m_DualCamData[0].Rotation);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("tiltx:%f"),m_DualCamData[1].TiltX-m_DualCamData[0].TiltX);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("tilty:%f"),m_DualCamData[1].TiltY-m_DualCamData[0].TiltY);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

	SAFE_DELETE_ARRAY(Left_Oblect_xyz);
	SAFE_DELETE_ARRAY(Left_imgPoint_xy);
	 
	SAFE_DELETE_ARRAY(Right_imgPoint_xy);
	SAFE_DELETE_ARRAY(Right_Oblect_xyz);
	ReleaseBuffer();
}

void StereoCal_TestItem::SetMarkPoint_ij(vector< vector<fCPoint>> &ImagePoint_xy,CPoint &MarkPoint_ij)
{
	vector< vector<fCPoint>>::iterator it;
	int i = 0;	
	for(it = ImagePoint_xy.begin()+1;it!=ImagePoint_xy.end()-1;it++)
	{  
		i++;
		int CurrentSize(0),LastSize(0),NextSize(0);
		CurrentSize = it->size(); 
		LastSize = (it-1)->size();  
		NextSize = (it+1)->size(); 
		if((CurrentSize+NextSize+1)==LastSize)
		{
			MarkPoint_ij.y = i;
			MarkPoint_ij.x = it->size();

			for(int j=0;j<(it+1)->size();j++)
			{
				it->push_back((it+1)->at(j));
			}
			ImagePoint_xy.erase(it+1);
			break;
		} 
		if((CurrentSize+NextSize)==LastSize)
		{
			MarkPoint_ij.y = i;
			MarkPoint_ij.x = it->size();

			for(int j=0;j<(it+1)->size()-1;j++)
			{
				it->push_back((it+1)->at(j));
			}
			ImagePoint_xy.erase(it+1);
			break;
		} 
	}

}

void StereoCal_TestItem::CreateImgPoint_xy(vector< vector<fCPoint>> NearImgPoint_xy,map<CString, fCPoint> farImgPointMap,vector<CString>  farImgPointMapKey,float* imgPoint_xy)
{
	int imgPoint_xyIndex(0);
	for(UINT i = 0;i<NearImgPoint_xy.size();i++)
	{
		for(UINT j = 0;j<NearImgPoint_xy[i].size();j++)
		{ 
			imgPoint_xy[imgPoint_xyIndex] = NearImgPoint_xy[i][j].x;
			imgPoint_xy[imgPoint_xyIndex+1] = NearImgPoint_xy[i][j].y;
			imgPoint_xyIndex+=2;			
		}
	}
	int KeyIndex(0);
	//vector<fCPoint> farImgPoint;
	for(UINT i = 0;i<farImgPointMapKey.size();i++)
	{
		//farImgPoint.push_back(farImgPointMap.find(farImgPointMapKey[i])->second);
		imgPoint_xy[imgPoint_xyIndex] = farImgPointMap.find(farImgPointMapKey[i])->second.x;
		imgPoint_xy[imgPoint_xyIndex+1] = farImgPointMap.find(farImgPointMapKey[i])->second.y;
		imgPoint_xyIndex += 2;
	}
}

void StereoCal_TestItem::CreateObject_xyz(vector< vector<CPoint>> ObjectArray,int ObjectPitch,int ChartDistDiff,float* Object_xyz)
{
	int Object_xyIndex(0);
	for(UINT i = 0;i<ObjectArray.size();i++)
	{
		for(UINT j = 0;j<ObjectArray[i].size();j++)
		{ 
			Object_xyz[Object_xyIndex] = ObjectArray[i][j].x*ObjectPitch;
			Object_xyz[Object_xyIndex+1] = ObjectArray[i][j].y*ObjectPitch;
			Object_xyz[Object_xyIndex+2] = 0;
			Object_xyIndex+=3;
		}
	}
	for(UINT i = 0;i<ObjectArray.size();i++)
	{
		for(UINT j = 0;j<ObjectArray[i].size();j++)
		{ 
			Object_xyz[Object_xyIndex] = ObjectArray[i][j].x*ObjectPitch;
			Object_xyz[Object_xyIndex+1] = ObjectArray[i][j].y*ObjectPitch;
			Object_xyz[Object_xyIndex+2] = ChartDistDiff;
			Object_xyIndex+=3;
		}
	}
}

int StereoCal_TestItem::Cam3DCalibrationSys()
{
	BOOL Flag = LoadImages();
	if(!Flag)
	{
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_Addlog(CamID,_T("图片加载失败"),COLOR_RED,200);
		return 0;
	}
	CPoint LeftFar_MarkPoint_ij,RightFar_MarkPoint_ij,LeftNear_MarkPoint_ij,RightNear_MarkPoint_ij;
	vector< vector<CPoint>> LeftfarObjectArray;
	vector< vector<CPoint>> RightfarObjectArray;
	SetImagePoint_xy(leftFarImg, LeftFarImagePoint_xy,LeftFar_MarkPoint_ij);
	SetImagePoint_xy(leftNearImg, LeftNearImagePoint_xy,LeftNear_MarkPoint_ij);
	SetImagePoint_xy(rightFarImg, RightFarImagePoint_xy,RightFar_MarkPoint_ij);
	SetImagePoint_xy(rightNearImg, RightNearImagePoint_xy,RightNear_MarkPoint_ij);
	Sleep(200);
	CreateObjectArraySys(LeftNearImagePoint_xy,LeftNear_MarkPoint_ij,LeftObjectArray);
	CreateObjectArraySys(LeftFarImagePoint_xy,LeftFar_MarkPoint_ij,LeftfarObjectArray);
	Sleep(200);
	CreateObjectArraySys(RightNearImagePoint_xy,RightNear_MarkPoint_ij,RightObjectArray);
	CreateObjectArraySys(RightFarImagePoint_xy,RightFar_MarkPoint_ij,RightfarObjectArray);
	Sleep(200); 

	IN_PARAMS m_CamPara[2];
	DualCamData m_DualCamData[2];

	int Left_imgPoint_xySize = LeftNearImagePoint_xy.size()*LeftNearImagePoint_xy[0].size()*2 + LeftFarImagePoint_xy.size()*LeftFarImagePoint_xy[0].size()*2 -2*2;//
	m_CamPara[0].focal_length_mm =_ttof(m_CamInfo[0].CamEFL) ;
	m_CamPara[0].imgPoint_xy = Left_imgPoint_xy;
	m_CamPara[0].imgPoint_xySize = Left_imgPoint_xySize;
	m_CamPara[0].img_height = leftNearImg.imgheight;
	m_CamPara[0].img_width = leftNearImg.imgwidth;
	m_CamPara[0].Oblect_xyz = Left_Oblect_xyz;
	m_CamPara[0].Oblect_xyzSize = Left_imgPoint_xySize*3/2;
	m_CamPara[0].pixel_size_um =  _ttof(m_CamInfo[0].CamPixelSize);

	int Right_imgPoint_xySize = RightNearImagePoint_xy.size()*RightNearImagePoint_xy[0].size()*2 + RightFarImagePoint_xy.size()*RightFarImagePoint_xy[0].size()*2 - 2*2;
	m_CamPara[1].focal_length_mm = _ttof(m_CamInfo[1].CamEFL) ;
	m_CamPara[1].imgPoint_xy = Right_imgPoint_xy;
	m_CamPara[1].imgPoint_xySize = Right_imgPoint_xySize;
	m_CamPara[1].img_height = rightNearImg.imgheight;
	m_CamPara[1].img_width = rightNearImg.imgwidth;
	m_CamPara[1].Oblect_xyz = Right_Oblect_xyz;
	m_CamPara[1].Oblect_xyzSize = Right_imgPoint_xySize*3/2;
	m_CamPara[1].pixel_size_um = _ttof(m_CamInfo[1].CamPixelSize) ;
	 int objectPitch_x = m_ChartInfo.objectPitch_x;
	 int FarChartDist = m_ChartInfo.FarChartDist;
	 int NearChartDist = m_ChartInfo.NearChartDist;
	//Init 	Left_imgPoint_xy Left_Object_xyIndex
	int Left_imgPoint_xyIndex(0),Left_Object_xyIndex(0);
     	 
	CreateImgPoint_xy(LeftNearImagePoint_xy,LeftFarImagePoint_xy,Left_imgPoint_xy); 
	CreateImgPoint_xy(RightNearImagePoint_xy,RightFarImagePoint_xy,Right_imgPoint_xy);
	CreateObject_xyz(LeftObjectArray,LeftfarObjectArray,objectPitch_x,FarChartDist-NearChartDist,Left_Oblect_xyz);
	CreateObject_xyz(RightObjectArray,RightfarObjectArray,objectPitch_x,FarChartDist-NearChartDist,Right_Oblect_xyz);


	CalibrationSingleCam( m_CamPara[0],m_DualCamData[0]);
	CalibrationSingleCam( m_CamPara[1],m_DualCamData[1]);
	CString str;
	str.Format(_T("rota:%f"),m_DualCamData[1].Rotation-m_DualCamData[0].Rotation);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("tiltx:%f"),m_DualCamData[1].TiltX-m_DualCamData[0].TiltX);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	str.Format(_T("tilty:%f"),m_DualCamData[1].TiltY-m_DualCamData[0].TiltY);
	m_pInterface->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	 
	ReleaseBuffer();
	


	return 1;

}

void StereoCal_TestItem::CreateImgPoint_xy(vector< vector<fCPoint>> NearImgPoint_xy,vector< vector<fCPoint>> FarImgPoint_xy,float* imgPoint_xy)
{
	int imgPoint_xyIndex(0);
	for(UINT i = 0;i<NearImgPoint_xy.size();i++)
	{
		for(UINT j = 0;j<NearImgPoint_xy[i].size();j++)
		{ 
			imgPoint_xy[imgPoint_xyIndex] = NearImgPoint_xy[i][j].x;
			imgPoint_xy[imgPoint_xyIndex+1] = NearImgPoint_xy[i][j].y;
			imgPoint_xyIndex+=2;			
		}
	} 
	for(UINT i = 0;i<FarImgPoint_xy.size();i++)
	{
		for(UINT j = 0;j<FarImgPoint_xy[i].size();j++)
		{ 
			imgPoint_xy[imgPoint_xyIndex] = FarImgPoint_xy[i][j].x;
			imgPoint_xy[imgPoint_xyIndex+1] = FarImgPoint_xy[i][j].y;
			imgPoint_xyIndex+=2;			
		}
	}
}

void StereoCal_TestItem::CreateObject_xyz(vector< vector<CPoint>> NearObjectArray,vector< vector<CPoint>> FarObjectArray,int ObjectPitch,int ChartDistDiff,float* Object_xyz)
{
	int Object_xyIndex(0);
	for(UINT i = 0;i<NearObjectArray.size();i++)
	{
		for(UINT j = 0;j<NearObjectArray[i].size();j++)
		{ 
			Object_xyz[Object_xyIndex] = NearObjectArray[i][j].x*ObjectPitch;
			Object_xyz[Object_xyIndex+1] = NearObjectArray[i][j].y*ObjectPitch;
			Object_xyz[Object_xyIndex+2] = 0;
			Object_xyIndex+=3;
		}
	}
	for(UINT i = 0;i<FarObjectArray.size();i++)
	{
		for(UINT j = 0;j<FarObjectArray[i].size();j++)
		{ 
			Object_xyz[Object_xyIndex] = FarObjectArray[i][j].x*ObjectPitch;
			Object_xyz[Object_xyIndex+1] = FarObjectArray[i][j].y*ObjectPitch;
			Object_xyz[Object_xyIndex+2] = ChartDistDiff;
			Object_xyIndex+=3;
		}
	}
}

void StereoCal_TestItem::CreateObjectArraySys(vector< vector<fCPoint>> ImagePoint_xy,CPoint MarkPoint_ij,vector< vector<CPoint>> &ObjectArray)
{
	UINT RowConut = ImagePoint_xy.size();
	vector<CPoint> rowObject;
	ObjectArray.clear();
	for(UINT i=0;i<RowConut;i++)
	{
		for(UINT j=0;j<ImagePoint_xy[i].size();j++)
		{
			CPoint Object_xy;
			if (i == MarkPoint_ij.y&& j>MarkPoint_ij.x-1)
			{
				Object_xy.x = (j+1) - MarkPoint_ij.x;
				Object_xy.y= i -  MarkPoint_ij.y;
			}
			else
			{
				Object_xy.x = j-MarkPoint_ij.x;
				Object_xy.y= i -MarkPoint_ij.y ;
			}

			rowObject.push_back(Object_xy);
		}
		ObjectArray.push_back(rowObject);
		rowObject.clear();
	}
}
 
 
int StereoCal_TestItem::HwHdcCal()
{ 
	CString toolPath,toolname;
	toolname = _T("run.bat");
	toolPath = m_pInterface->Ctrl_GetCurrentPath() + _T("HDC\\HwHDC");
	//::SetCurrentDirectory(_T("E:\\CCMSDK_1224\\Execute\\Debug\\HDC\\HwHDC"));
	::ShellExecute(NULL,_T("open"),toolname,_T(""),toolPath,SW_SHOW);//SW_HIDE //SW_SHOW
	Sleep(4000);
	CString strResult;
	CStdioFile file;
	BOOL  bOpen = FALSE;
	while(!bOpen)
	{
		 bOpen = file.Open(toolPath + _T("\\log.txt"),CFile::modeRead);
		 Sleep(300);
	}
	
	file.SeekToBegin();
	file.ReadString(strResult);
	file.Close();

	if(strResult.Find(_T("HDC_PASS"))!=-1)
	{
		//保存otp数据
		

		/*if(!file.Open(toolPath + _T("\\otp_0.bin"),CFile::modeRead)) return -1; 
		int OtpLen = file.GetLength();
		BYTE* OtpData = new BYTE[OtpLen];
		file.Read(OtpData,OtpLen);
		file.Close();*/


		CString binPath = toolPath + _T("\\otp_0.bin");
		
		FILE *fp;
		char path[512] = {0};
		int bytelen = 600;
		BYTE* OtpData = new BYTE[bytelen];
		ImageProc::ConvertWideToChar(binPath,path);
		fp = fopen(path,"rb");
		fread(OtpData,sizeof(unsigned char),bytelen*sizeof(unsigned char),fp);
		fclose(fp);

		::DeleteFile(binPath);

		vector<USHORT> vecOtp;
		for(int i=0;i<bytelen;i++)
		{
			vecOtp.push_back(OtpData[i]);
		}
		m_pInterface->Otp_WriteToOtpBoard(CamID,GetName(),vecOtp,TRUE);
		SAFE_DELETE_ARRAY(OtpData);
		 SetResult(RESULT_PASS);
		 fileCopy();
		return 1;
	}
	else
	{
#if 0
   		file.Open(toolPath + _T("\\otp_0.bin"),CFile::modeRead); 
		int OtpLen = file.GetLength();
		BYTE* OtpData = new BYTE[OtpLen];
		file.Read(OtpData,OtpLen);
		file.Close();
		vector<USHORT> vecOtp;
		for(int i=0;i<OtpLen;i++)
		{
			vecOtp.push_back(OtpData[i]);
		}
		m_pInterface->Otp_WriteToOtpBoard(CamID,GetName(),vecOtp,TRUE);
		SAFE_DELETE_ARRAY(OtpData);
#endif
		SetResult(RESULT_FAIL);
		fileCopy();
		return -1;
	}
	
}

int StereoCal_TestItem::HwMmiCheck()
{
	CStdioFile file;
	CString toolPath,toolname;
	toolPath = m_pInterface->Ctrl_GetCurrentPath() + _T("HDC\\HwMMI");
	//::DeleteFile(toolPath + _T("\\result.csv"));
	//file.Open(toolPath + _T("\\result.csv"),CFile::modeCreate);
	//file.Close();

	Sleep(1000);


	USHORT* otpDataOut= new USHORT[600];
	BYTE* bDataOut= new BYTE[600];
	
	
	//根据mOtpConver 中item 数量一个个读取出来
	//if(mOtpConver.mapOtpReg.m_otpReg.bUsed)
	//	{
	USHORT sReg = 0x1215; // 0x146C
	USHORT eReg = 0x146C;
	USHORT uPage = 0;
	int Len = eReg - sReg + 1;
	int rFlag = m_pInterface->Otp_OtpRead(sReg,eReg,otpDataOut,0,uPage);
	/*int rFlag = m_pInterface->Otp_OtpRead(0x1215,0x1300,otpDataOut,0,uPage);
	Sleep(500);
	rFlag = m_pInterface->Otp_OtpRead(0x1301,0x1375,&otpDataOut[0x1300-0x1215+1],0,uPage);
	Sleep(500);
	rFlag = m_pInterface->Otp_OtpRead(0x1376,0x146c,&otpDataOut[0x1375-0x1215+1],0,uPage);*/

	
//	CString binPath = _T("E:\\CCMSDK_1224\\Execute\\Debug\\HDC\\HwMMI\\otp.bin");
	 
//	CFile otpBin;
	CString binPath = m_pInterface->Ctrl_GetCurrentPath() + _T("HDC\\HwMMI\\otp.bin");
	//BOOL binFILE = file.Open(binPath,CFile::modeCreate|CFile::modeWrite);
//	otpBin.Open(binPath,CFile::modeCreate|CFile::modeWrite);
	
	

	int calChecksum = 0;

	for(int i=0;i<600;i++)
	{
		//bDataOut[i] = otpDataOut[i]&0xff;
		bDataOut[i] = 0xFF;
	}

	//calChecksum = calChecksum%65535;

	// m_pInterface->Otp_OtpRead(0x146d,0x146e,otpDataOut,0,uPage);
	 //int readChecksum = otpDataOut[0]*256 + otpDataOut[1];

	//int readChecksum = 0;
	//readChecksum = m_pInterface->

	FILE *fp;
	char path[512] = {0};
	ImageProc::ConvertWideToChar(binPath,path);
	fp = fopen(path,"wb");
	fwrite(bDataOut,sizeof(unsigned char),600*sizeof(unsigned char),fp);
	fclose(fp);

	BYTE * rebackData = new BYTE[600];
	fp = fopen(path,"rb");
	fread(rebackData,sizeof(unsigned char),600*sizeof(unsigned char),fp);
	fclose(fp);
	CStdioFile file_txt;
	CString binPath_txt = m_pInterface->Ctrl_GetCurrentPath() + _T("HDC\\HwMMI\\otp.txt");
	file_txt.Open(binPath_txt,CFile::modeCreate|CFile::modeWrite);
	for(int i=0;i<600;i++)
	{
		if(rebackData[i]!=bDataOut[i])
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("bin读写有差异"),COLOR_RED,200);
			//break;
		}
		CString str;
		str.Format(_T("%d %02x %02x \n"),i,rebackData[i],bDataOut[i]);
		file_txt.WriteString(str);
	}
	file_txt.Close();
	SAFE_DELETE_ARRAY(rebackData);
//	otpBin.Write(bDataOut,600);
	SAFE_DELETE_ARRAY(otpDataOut);
	SAFE_DELETE_ARRAY(bDataOut);
//	otpBin.Close();


	CStdioFile cvfile;
	 
	
	//cvfile.Open(toolPath + _T("\\result.csv"),CFile::modeCreate);
	//cvfile.Close();
	

	
	toolname = _T("run30.bat");
	 
	::ShellExecute(NULL,_T("open"),toolname,_T(""),toolPath,SW_SHOW);//SW_HIDE //SW_SHOW
	Sleep(5000);

	toolname = _T("run100.bat");
	 
	::ShellExecute(NULL,_T("open"),toolname,_T(""),toolPath,SW_SHOW);//SW_HIDE //SW_SHOW
	Sleep(10000);
	while(1)
	{
	   BOOL bOpen = cvfile.Open(toolPath + _T("\\result.csv"),CFile::modeRead);
	   if(bOpen)
	   {
		   break;
	   }
	   Sleep(500);
	}
	depth30.clear();
	depth100.clear();
	tempStr.clear();
	tempStr30.clear();
	cvfile.SeekToBegin();
	CString str;
	
	do{		
		vector<CString> vecStr;
		cvfile.ReadString(str); 
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);
		if(str.GetLength()<2) break;

		ImageProc::SeparateString(str,'_',&vecStr);
		if(_ttoi(vecStr[0])==30)
		{
			vecStr.clear();
			ImageProc::SeparateString(str,',',&vecStr);

			depth30.push_back(_ttof(vecStr[1]));
			tempStr30.push_back(vecStr[1]);
		}
		else if(_ttoi(vecStr[0])==100)
		{
			vecStr.clear();
			ImageProc::SeparateString(str,',',&vecStr);

			depth100.push_back(_ttof(vecStr[1]));
			tempStr.push_back(vecStr[1]);
		}
		 

		if((vecStr.size()<2)&&(depth30.size()<2))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("没有数据"),COLOR_RED,200);
			return -1;
		}
		
		
	}while(str.GetLength()>2);

	cvfile.Close();

	if(depth30.size()<1)
	{
		return -1;
	}
	int depthSize = depth30.size();
	int depth100Size = depth100.size();
	if(abs(depth30[depthSize-1]-300)>30||abs(depth100[depth100Size-1]-1000)>200)
	{
		m_pInterface->Ctrl_Addlog(CamID,tempStr30[depthSize-1],COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,tempStr[depth100Size-1],COLOR_RED,200);
		::DeleteFile(toolPath + _T("\\result.csv"));
		return -1;
	}

	m_pInterface->Ctrl_Addlog(CamID,tempStr30[depthSize-1],COLOR_GREEN,200);
	m_pInterface->Ctrl_Addlog(CamID,tempStr[depth100Size-1],COLOR_GREEN,200);
	::DeleteFile(toolPath + _T("\\result.csv"));	

	return 1;
	
}
 
 
int StereoCal_TestItem::ReleaseBuffer()
{
	m_pInterface->Ctrl_Addlog(CamID,_T("DEL BUF"),COLOR_BLUE,200);
	//SAFE_DELETE_ARRAY(leftFarImg.Raw_buffer);
	SAFE_DELETE_ARRAY(leftNearImg.Raw_buffer);
	SAFE_DELETE_ARRAY(rightFarImg.Raw_buffer);
	SAFE_DELETE_ARRAY(rightNearImg.Raw_buffer);

	delete[] leftFarImg.Raw_buffer;
	leftFarImg.Raw_buffer = NULL;

	/*SAFE_DELETE_ARRAY(leftFarImg.RGB24_buffer);
	SAFE_DELETE_ARRAY(leftNearImg.RGB24_buffer);
	SAFE_DELETE_ARRAY(rightFarImg.RGB24_buffer);
	SAFE_DELETE_ARRAY(rightNearImg.RGB24_buffer);
*/
	return 1;
}

int StereoCal_TestItem::fileCopy()
{
	CString targetFilePath = m_pInterface->Ctrl_GetCurrentPath() + _T("HDC\\HwHDC\\") + m_pInterface->Ctrl_GetCamCode();
	CreateDirectory(targetFilePath, NULL);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\A_20cm_0000.jpg"), targetFilePath+  _T("\\A_20cm_0000.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\A_20cm_0015.jpg"), targetFilePath+  _T("\\A_20cm_0015.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\A_20cm_1015.jpg"), targetFilePath+  _T("\\A_20cm_1015.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\A_50cm_0000.jpg"), targetFilePath+  _T("\\A_50cm_0000.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\A_50cm_0015.jpg"), targetFilePath+  _T("\\A_50cm_0015.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\A_50cm_1015.jpg"), targetFilePath+  _T("\\A_50cm_1015.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\B_20cm_0000.jpg"), targetFilePath+  _T("\\B_20cm_0000.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\B_20cm_0015.jpg"), targetFilePath+  _T("\\B_20cm_0015.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\B_20cm_1015.jpg"), targetFilePath+  _T("\\B_20cm_1015.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\B_50cm_0000.jpg"), targetFilePath+  _T("\\B_50cm_0000.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\B_50cm_0015.jpg"), targetFilePath+  _T("\\B_50cm_0015.jpg"), FALSE);
	CopyFile(m_pInterface->Ctrl_GetCurrentPath() + _T("HuaWei\\B_50cm_1015.jpg"), targetFilePath+  _T("\\B_50cm_1015.jpg"), FALSE);

	CString toolPath = m_pInterface->Ctrl_GetCurrentPath() + _T("HDC\\HwHDC");
	//toolPath =  toolPath + _T("\\otp_0.bin");	CopyFile( toolPath + _T("\\otp_0.bin"), targetFilePath+  _T("\\otp_0.bin"), FALSE);
	CopyFile( toolPath + _T("\\out.txt"), targetFilePath+  _T("\\out.txt"), FALSE);
 
	return 1;
 
}