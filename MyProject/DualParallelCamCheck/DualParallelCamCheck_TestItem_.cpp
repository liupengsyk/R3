#include "StdAfx.h"
 
#include "DualParallelCamCheck_TestItem.h"

#pragma comment(lib, "halcon.lib")
#pragma comment(lib, "halconcpp.lib")

using namespace Halcon;

#pragma comment(lib,"mems_dualccm_twolayer\\mems_dualccm_twolayer.lib")
  

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new DualParallelCamCheck_TestItem(pInterface, lpszName);
}
 

  int DualParallelCamCheck_TestItem::HistoThreshold = 0;

/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
DualParallelCamCheck_TestItem::DualParallelCamCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	//SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化

	CamCal.Infinity_PointChartInfo.ObjectLength = 500; 
	CamCal.Infinity_PointChartInfo.PointNum = 54;
	CamCal.Infinity_PointChartInfo.PointPitch = 26;
	CamCal.Infinity_PointChartInfo.PointSize = 16;

	CamCal. Macro_PointChartInfo.ObjectLength = 300; 
	CamCal. Macro_PointChartInfo.PointNum = 54; 
	CamCal. Macro_PointChartInfo.PointPitch = 26; 
	CamCal. Macro_PointChartInfo.PointSize = 16; 
	//.....
}

DualParallelCamCheck_TestItem::~DualParallelCamCheck_TestItem(void)
{
	
}


int DualParallelCamCheck_TestItem::InitItem()
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
int DualParallelCamCheck_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	 //TODO:在此添加初始化代码
    
	 //清空上一次的数据
	TestTimes = 0;
	TimesSet = 2;
	GetImgPointFailTimes[0] = 0;
	GetImgPointFailTimes[1] = 0;

	SetbStopTest(FALSE);
	 
	return 0;
}
 
/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int DualParallelCamCheck_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	  
  if( GetTilt_dif()!=0)   return 1;
    
   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int DualParallelCamCheck_TestItem::Finalize()
{
	 
	//TODO:在此添加回收结束代码  
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int DualParallelCamCheck_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 

	bContinue = ReadConfigInt(_T("bContinue"),0);
	bOnAA = ReadConfigInt(_T("bOnAA"),0); 

	CString str;   
	 
	CamCal.m_SpecData.strRotationSpc = ReadConfigString(_T("RotationSpc"),_T("0.25"));
	CamCal.m_SpecData.RotationSpc = (float)_ttof(CamCal.m_SpecData.strRotationSpc);
	CamCal.m_SpecData.strTiltXSpc = ReadConfigString(_T("TiltXSpc"),_T("0.25"));
	CamCal. m_SpecData.TiltXSpc = (float)_ttof(CamCal.m_SpecData.strTiltXSpc);
	CamCal.m_SpecData.strTiltYSpc = ReadConfigString(_T("TiltYSpc"),_T("0.25"));
	CamCal. m_SpecData.TiltYSpc = (float)_ttof(CamCal.m_SpecData.strTiltYSpc);

	CamCal.CamInfo[0].baseInfo.strPixelSize = ReadConfigString(_T("Cam0_PixelSize"),_T("1.12"));
	CamCal.CamInfo[0].baseInfo.PixelSize = (float)_ttof(CamCal.CamInfo[0].baseInfo.strPixelSize);
	CamCal.CamInfo[0].baseInfo.strFocusLength = ReadConfigString(_T("Cam0_FocusLength"),_T("3.79"));
	CamCal.CamInfo[0].baseInfo.FocusLength = (float)_ttof(CamCal.CamInfo[0].baseInfo.strFocusLength);
 
	CamCal.CamInfo[1].baseInfo.strPixelSize = ReadConfigString(_T("Cam1_PixelSize"),_T("1.12"));
	CamCal.CamInfo[1].baseInfo.PixelSize = (float)_ttof(CamCal.CamInfo[1].baseInfo.strPixelSize);
	CamCal.CamInfo[1].baseInfo.strFocusLength = ReadConfigString(_T("Cam1_FocusLength"),_T("3.79"));
	CamCal.CamInfo[1].baseInfo.FocusLength = (float)_ttof(CamCal.CamInfo[1].baseInfo.strFocusLength);
 	 	 
	CamCal.Infinity_PointChartInfo.ObjectLength =ReadConfigInt(_T("InfinityObjectLength"),500); 
	CamCal.Infinity_PointChartInfo.PointNum = ReadConfigInt(_T("InfinityPointNum"),54);
	CamCal.Infinity_PointChartInfo.PointPitch = ReadConfigInt(_T("InfinityPointPitch"),26);
	CamCal.Infinity_PointChartInfo.PointSize = ReadConfigInt(_T("InfinityPointSize"),16);

	CamCal.Macro_PointChartInfo.ObjectLength = ReadConfigInt(_T("MacroObjectLength"),300); 
	CamCal.Macro_PointChartInfo.PointNum = ReadConfigInt(_T("MacroObPointNum"),54); 
	CamCal.Macro_PointChartInfo.PointPitch = ReadConfigInt(_T("MacroPointPitch"),26); 
	CamCal.Macro_PointChartInfo.PointSize = ReadConfigInt(_T("MacroPointSize"),16);  

	CamCal.CamInfo[0].nearPointImgSize = CamCal.Macro_PointChartInfo.PointSize*1000/CamCal.Macro_PointChartInfo.ObjectLength*CamCal.CamInfo[0].baseInfo.FocusLength/CamCal.CamInfo[0].baseInfo.PixelSize;
	CamCal.CamInfo[0].farPointImgSize = CamCal.Infinity_PointChartInfo.PointSize*1000/CamCal.Infinity_PointChartInfo.ObjectLength*CamCal.CamInfo[0].baseInfo.FocusLength/CamCal.CamInfo[0].baseInfo.PixelSize;
	 
	CamCal.CamInfo[1].nearPointImgSize = CamCal.Macro_PointChartInfo.PointSize*1000/CamCal.Macro_PointChartInfo.ObjectLength*CamCal.CamInfo[1].baseInfo.FocusLength/CamCal.CamInfo[1].baseInfo.PixelSize;
	CamCal.CamInfo[1].farPointImgSize = CamCal.Infinity_PointChartInfo.PointSize*1000/CamCal.Infinity_PointChartInfo.ObjectLength*CamCal.CamInfo[1].baseInfo.FocusLength/CamCal.CamInfo[1].baseInfo.PixelSize;
	 
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int DualParallelCamCheck_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	CString str;     
	WriteConfigInt(_T("bOnAA"),bOnAA); 
	WriteConfigInt(_T("bContinue"),bContinue); 
	 
	WriteConfigString(_T("RotationSpc"),CamCal.m_SpecData.strRotationSpc); 
	WriteConfigString(_T("TiltXSpc"),CamCal.m_SpecData.strTiltXSpc); 
	WriteConfigString(_T("TiltYSpc"),CamCal.m_SpecData.strTiltYSpc);

	  
	WriteConfigString(_T("Cam0_PixelSize"),CamCal.CamInfo[0].baseInfo.strPixelSize);	 
	WriteConfigString(_T("Cam0_FocusLength"),CamCal.CamInfo[0].baseInfo.strFocusLength);
	 
	WriteConfigString(_T("Cam1_PixelSize"),CamCal.CamInfo[1].baseInfo.strPixelSize);	 
	WriteConfigString(_T("Cam1_FocusLength"),CamCal.CamInfo[1].baseInfo.strFocusLength);
	 
	WriteConfigInt(_T("InfinityObjectLength"),CamCal.Infinity_PointChartInfo.ObjectLength); 
	WriteConfigInt(_T("InfinityPointNum"),CamCal.Infinity_PointChartInfo.PointNum); 
	WriteConfigInt(_T("InfinityPointPitch"),CamCal.Infinity_PointChartInfo.PointPitch); 
	WriteConfigInt(_T("InfinityPointSize"),CamCal.Infinity_PointChartInfo.PointSize); 

	WriteConfigInt(_T("MacroObjectLength"),CamCal.Macro_PointChartInfo.ObjectLength); 
	WriteConfigInt(_T("MacroObPointNum"),CamCal.Macro_PointChartInfo.PointNum); 
	WriteConfigInt(_T("MacroPointPitch"),CamCal.Macro_PointChartInfo.PointPitch); 
	WriteConfigInt(_T("MacroPointSize"),CamCal.Macro_PointChartInfo.PointSize);  
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR DualParallelCamCheck_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	CString temp;

	temp=_T("dRotatio,");
	header += temp;
	temp=_T("dXTilt,");		 
	header += temp;
	temp=_T("dYTilt,");			 
	header += temp;

	temp=_T("Cam0_Rotatio,");
	header += temp;
	temp=_T("Cam0_XTilt,");		 
	header += temp;
	temp=_T("Cam0_YTilt,");			 
	header += temp; 

	temp=_T("Cam1_Rotatio,");
	header += temp;
	temp=_T("Cam1_XTilt,");		 
	header += temp;
	temp=_T("Cam1_YTilt,");			 
	header += temp;

	temp=_T("Cam0_OCX,");			 
	header += temp;

	temp=_T("Cam0_OCY,");			 
	header += temp; 

	temp=_T("Cam1_OCX,");			 
	header += temp;

	temp=_T("Cam1_OCY");			 
	header += temp; 
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR DualParallelCamCheck_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	CString temp;

	temp.Format(_T("-%4lf,"),0);
	LowerLimit += temp;
	temp.Format(_T("-%4lf,"),0);
	LowerLimit += temp;
	temp.Format(_T("-%4lf,"),0);			 
	LowerLimit += temp; 

	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR DualParallelCamCheck_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	CString temp;

	temp.Format(_T("%4lf,"),CamCal.m_SpecData.RotationSpc);
	UpperLimit += temp;
	temp.Format(_T("%4lf,"),CamCal.m_SpecData.TiltXSpc);
	UpperLimit += temp;
	temp.Format(_T("%4lf,"),CamCal.m_SpecData.TiltYSpc);			 
	UpperLimit += temp; 

	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR DualParallelCamCheck_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	CString temp;
	 
	temp.Format(_T(" %.2lf,"),(CamCal.DualData_dif.dif_Rotation)+0.005);
	Content += temp;
	temp.Format(_T("%.2lf,"),(CamCal.DualData_dif.dif_TiltX)+0.005);
	Content += temp;
	temp.Format(_T("%.2lf,"),(CamCal.DualData_dif.dif_TiltY)+0.005);
	Content += temp;

	temp.Format(_T(" %.2lf,"),(CamCal.CamInfo[0].m_DualCamData.Rotation)+0.005);
	Content += temp;
	temp.Format(_T("%.2lf,"),(CamCal.CamInfo[0].m_DualCamData.TiltX)+0.005);
	Content += temp;
	temp.Format(_T("%.2lf,"),(CamCal.CamInfo[0].m_DualCamData.TiltY)+0.005);
	Content += temp;

	temp.Format(_T(" %.2lf,"),(CamCal.CamInfo[1].m_DualCamData.Rotation)+0.005);
	Content += temp;
	temp.Format(_T("%.2lf,"),(CamCal.CamInfo[1].m_DualCamData.TiltX)+0.005);
	Content += temp;
	temp.Format(_T("%.2lf,"),(CamCal.CamInfo[1].m_DualCamData.TiltY)+0.005);
	Content += temp;

	temp.Format(_T("%d,"),CamCal.CamInfo[0].m_DualCamData.OpticalCenter.x);
	Content += temp;

	temp.Format(_T("%d,"),CamCal.CamInfo[0].m_DualCamData.OpticalCenter.y);
	Content += temp; 

	temp.Format(_T("%d,"),CamCal.CamInfo[1].m_DualCamData.OpticalCenter.x);
	Content += temp;
	temp.Format(_T("%d"),CamCal.CamInfo[1].m_DualCamData.OpticalCenter.y);
	Content += temp;

	return Content;
}
  

void DualParallelCamCheck_TestItem::SetDualCamData()
{  
	m_pInterface->Ctrl_SetDualCamData(CamID,CamCal.DualData_dif);
}
 
int DualParallelCamCheck_TestItem::CamCalInit()
{ 
	for(int i=0;i<2;i++)
	{
		//if(CamCal.CamInfo[i].bEnable)
		CamCal.CamInfo[i].img = *m_pInterface->Ctrl_GetImgInfo(i);
	}
	return 1;
}
 
int DualParallelCamCheck_TestItem::GetCamTilt(int CamID)
{
	ImageInfo img = CamCal.CamInfo[CamID].img;
	 
	pBmpRed = (unsigned char *)malloc(img.imgwidth * img.imgheight);
	pBmpGreen = (unsigned char *)malloc(img.imgwidth * img.imgheight);
	pBmpBlue = (unsigned char *)malloc(img.imgwidth * img.imgheight);
	
	int TotalPoint = CamCal.Infinity_PointChartInfo.PointNum+CamCal. Macro_PointChartInfo.PointNum;
	float ChartDist = (float)(CamCal.Infinity_PointChartInfo.ObjectLength - CamCal. Macro_PointChartInfo.ObjectLength);

	vector<float> Point_xy; 
	GetImgPoint_xyByHalcon(CamID,&Point_xy); //GetImgPoint_xyByHalconContours(&Point_xy);			
  
	if(Point_xy.size()!=54*2*2)
	{ 
		CString str;
		str.Format(_T("%d"),Point_xy.size()/2/2);
		m_pInterface->Ctrl_Addlog(CamID,_T("没有找到足够的点"),COLOR_RED,200);
		Sleep(300);
		return 2;
	} 
	float chart_xyz[100*3*2] =
	{
		0	,	0	,	0	,	26	,	0	,	0	,	52	,	0	,	0	,	78	,	0	,	0	,	104	,	0	,	0	,	130	,	0	,	0	,	156	,	0	,	0	,	182	,	0	,	0	,	208	,	0	,	0	,	234	,	0	,	0	,	260	,	0	,	0	,	286	,	0	,	0	,	312	,	0	,	0	,
		0	,	26	,	0	,	26	,	26	,	0	,																																																							286	,	26	,	0	,	312	,	26	,	0	,
		0	,	52	,	0	,	26	,	52	,	0	,																																																							286	,	52	,	0	,	312	,	52	,	0	,
		0	,	78	,	0	,	26	,	78	,	0	,																																																							286	,	78	,	0	,	312	,	78	,	0	,
		0	,	104	,	0	,	26	,	104	,	0	,																																																							286	,	104	,	0	,	312	,	104	,	0	,
		0	,	130	,	0	,	26	,	130	,	0	,																																																							286	,	130	,	0	,	312	,	130	,	0	,
		0	,	156	,	0	,	26	,	156	,	0	,																																																							286	,	156	,	0	,	312	,	156	,	0	,
		0	,	182	,	0	,	26	,	182	,	0	,																																																							286	,	182	,	0	,	312	,	182	,	0	,
		0	,	208	,	0	,	26	,	208	,	0	,	52	,	208	,	0	,	78	,	208	,	0	,	104	,	208	,	0	,	130	,	208	,	0	,	156	,	208	,	0	,	182	,	208	,	0	,	208	,	208	,	0	,	234	,	208	,	0	,	260	,	208	,	0	,	286	,	208	,	0	,	312	,	208	,	0	,

		0	,	0	,	ChartDist	,	26	,	0	,	ChartDist	,	52	,	0	,	ChartDist	,	78	,	0	,	ChartDist	,	104	,	0	,	ChartDist	,	130	,	0	,	ChartDist	,	156	,	0	,	ChartDist	,	182	,	0	,	ChartDist	,	208	,	0	,	ChartDist	,	234	,	0	,	ChartDist	,	260	,	0	,	ChartDist	,	286	,	0	,	ChartDist	,	312	,	0	,	ChartDist	,
		0	,	26	,	ChartDist	,	26	,	26	,	ChartDist	,																																																							                                                                        286	,	26	,	ChartDist	,	312	,	26	,	ChartDist	,
		0	,	52	,	ChartDist	,	26	,	52	,	ChartDist	,																																																							                                                                        286	,	52	,	ChartDist	,	312	,	52	,	ChartDist	,
		0	,	78	,	ChartDist	,	26	,	78	,	ChartDist	,																																																							                                                                        286	,	78	,	ChartDist	,	312	,	78	,	ChartDist	,
		0	,	104	,	ChartDist	,	26	,	104	,	ChartDist	,																																																							                                                                        286	,	104	,	ChartDist	,	312	,	104	,	ChartDist	,
		0	,	130	,	ChartDist	,	26	,	130	,	ChartDist	,																																																							                                                                        286	,	130	,	ChartDist	,	312	,	130	,	ChartDist	,
		0	,	156	,	ChartDist	,	26	,	156	,	ChartDist	,																																																							                                                                        286	,	156	,	ChartDist	,	312	,	156	,	ChartDist	,
		0	,	182	,	ChartDist	,	26	,	182	,	ChartDist	,																																																							                                                                        286	,	182	,	ChartDist	,	312	,	182	,	ChartDist	,
		0	,	208	,	ChartDist	,	26	,	208	,	ChartDist	,	52	,	208	,	ChartDist	,	78	,	208	,	ChartDist	,	104	,	208	,	ChartDist	,	130	,	208	,	ChartDist	,	156	,	208	,	ChartDist	,	182	,	208	,	ChartDist	,	208	,	208	,	ChartDist	,	234	,	208	,	ChartDist	,	260	,	208	,	ChartDist	,	286	,	208	,	ChartDist	,	312	,	208	,	ChartDist	,

	};	 
	 
    float CamImgPoint_xy[100*2*2] = {0};
	CamCal.CamInfo[CamID].m_DualCamData.PointPos.clear();
	for(UINT i=0;i<Point_xy.size();i++)
	{
		CamImgPoint_xy[i] = Point_xy[i];
		if(i%2 == 1)
		{
			fCPoint fp;
			fp.x = CamImgPoint_xy[i-1];
			fp.y = CamImgPoint_xy[i];
			CamCal.CamInfo[CamID].m_DualCamData.PointPos.push_back(fp); 
		}
		
	}
	 
	int imgPoint_xySize = TotalPoint*2; //96*2-4*2*2;//4*2*6 

	CamCal.CamInfo[CamID].m_IN_PARAMS.focal_length_mm = CamCal.CamInfo[CamID].baseInfo.FocusLength;
	CamCal.CamInfo[CamID].m_IN_PARAMS.pixel_size_um = CamCal.CamInfo[CamID].baseInfo.PixelSize;
	CamCal.CamInfo[CamID].m_IN_PARAMS.img_height = img.imgheight;
	CamCal.CamInfo[CamID].m_IN_PARAMS.img_width = img.imgwidth;

	CamCal.CamInfo[CamID].m_IN_PARAMS.imgPoint_xy = CamImgPoint_xy;
	CamCal.CamInfo[CamID].m_IN_PARAMS.imgPoint_xySize = TotalPoint*2;

	CamCal.CamInfo[CamID].m_IN_PARAMS.Oblect_xyz = chart_xyz;
	CamCal.CamInfo[CamID].m_IN_PARAMS.Oblect_xyzSize = TotalPoint*3;

	DualCCM_TwoLayerCalibrate(CamCal.CamInfo[CamID].m_IN_PARAMS , &CamCal.CamInfo[CamID].m_OUT_PARAMS);
 

	float ratioToAngle = (float)180/(float)3.1415926;
	CamCal.CamInfo[CamID].m_DualCamData.Rotation = CamCal.CamInfo[CamID].m_OUT_PARAMS.rotation_rad*ratioToAngle;
	CamCal.CamInfo[CamID].m_DualCamData.TiltX = CamCal.CamInfo[CamID].m_OUT_PARAMS.tilt_x_rad*ratioToAngle;
	CamCal.CamInfo[CamID].m_DualCamData.TiltY = CamCal.CamInfo[CamID].m_OUT_PARAMS.tilt_y_rad*ratioToAngle;

	CamCal.CamInfo[CamID].m_DualCamData.shift_x = CamCal.CamInfo[CamID].m_OUT_PARAMS.shift_x;
	CamCal.CamInfo[CamID].m_DualCamData.shift_y = CamCal.CamInfo[CamID].m_OUT_PARAMS.shift_y;
	CamCal.CamInfo[CamID].m_DualCamData.shift_z= CamCal.CamInfo[CamID].m_OUT_PARAMS.shift_z;

	CamCal.CamInfo[CamID].m_DualCamData.OpticalCenter.x = CamCal.CamInfo[CamID].m_OUT_PARAMS.opticalcenter_x_pixel;
	CamCal.CamInfo[CamID].m_DualCamData.OpticalCenter.y = CamCal.CamInfo[CamID].m_OUT_PARAMS.opticalcenter_y_pixel; 

	return 1;
}
int DualParallelCamCheck_TestItem::GetTilt_dif()
{ 	 
	CamCalInit();
	int Flag = 0;
	//if(CamCal.CamInfo[0].bEnable)
	{
		Flag = GetCamTilt(0);
		if(Flag!=1)
		{
			if(Flag == 2)
			{
				GetImgPointFailTimes[0]++;
			}
			if(GetImgPointFailTimes[0]>10)
			{
				CamCal.DualData_dif.dif_Rotation = 100;
				SetbStopTest(TRUE);
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetDualCamData(CamID,CamCal.DualData_dif);
				return 0;
			}		
		
			return 1;
		}
		else
		{
			GetImgPointFailTimes[0] = 0;
		}
	}

	//if(CamCal.CamInfo[1].bEnable)
	{
		Flag = GetCamTilt(1);
		if(Flag!=1)
		{
			if(Flag==2)
			{
				GetImgPointFailTimes[1]++;
			}
			if(GetImgPointFailTimes[1]>10)
			{
				CamCal.DualData_dif.dif_Rotation = 100;
				SetbStopTest(TRUE);
				SetResult(RESULT_FAIL);
				m_pInterface->Ctrl_SetDualCamData(CamID,CamCal.DualData_dif);
				return 0;
			}			
			return 1;
		}
		else
		{
			GetImgPointFailTimes[1] = 0;
		}
	}

	//if(CamCal.CamInfo[0].bEnable&&CamCal.CamInfo[1].bEnable)
	{
		CamCal.DualData_dif.dif_Rotation = CamCal.CamInfo[1].m_DualCamData.Rotation -  CamCal.CamInfo[0].m_DualCamData.Rotation;
		CamCal.DualData_dif.dif_TiltX = CamCal.CamInfo[1].m_DualCamData.TiltX -  CamCal.CamInfo[0].m_DualCamData.TiltX;
		CamCal.DualData_dif.dif_TiltY = CamCal.CamInfo[1].m_DualCamData.TiltY -  CamCal.CamInfo[0].m_DualCamData.TiltY; 
	}
	 
	m_pInterface->Ctrl_SetDualCamData(CamID,CamCal.DualData_dif);
	return ProcTiltData();
 
}

int DualParallelCamCheck_TestItem::ProcTiltData()
{  

	if( abs(CamCal.DualData_dif.dif_Rotation)<CamCal. m_SpecData.RotationSpc  &&  abs(CamCal.DualData_dif.dif_TiltX)< CamCal. m_SpecData.TiltXSpc  &&  abs(CamCal.DualData_dif.dif_TiltY) < CamCal. m_SpecData.TiltYSpc  )
	{
		CString str;
		str.Format(_T("dif_Rotation %.2f"),CamCal.DualData_dif.dif_Rotation);
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_GREEN,220);
		str.Format(_T("dif_TiltX %.2f"),CamCal.DualData_dif.dif_TiltX);
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_GREEN,220);
		str.Format(_T("dif_TiltY %.2f"),CamCal.DualData_dif.dif_TiltY);
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_GREEN,220);
		if(!bOnAA) SetbStopTest(TRUE);
		SetResult(RESULT_PASS);

		vector<USHORT> tiltData; 

		tiltData.push_back(CamCal.DualData_dif.dif_TiltX*1000);
		tiltData.push_back(CamCal.DualData_dif.dif_TiltY*1000);
		tiltData.push_back(CamCal.DualData_dif.dif_Rotation*1000);
		m_pInterface->Otp_WriteToOtpBoard(CamID,GetName(),tiltData,TRUE);

	}
	else
	{
		CString str;
		str.Format(_T("dif_Rotation %.2f"),CamCal.DualData_dif.dif_Rotation);
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,220);
		str.Format(_T("dif_TiltX %.2f"),CamCal.DualData_dif.dif_TiltX);
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,220);
		str.Format(_T("dif_TiltY %.2f"),CamCal.DualData_dif.dif_TiltY);
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,220);
		if(!bOnAA) SetbStopTest(TRUE);
		SetResult(RESULT_FAIL);
	}
	return 0;
				 
}
    
void DualParallelCamCheck_TestItem::ShowInfo()
{
	ImageInfo img = CamCal.CamInfo[CamID].img;
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

	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight; 


	int x = DisplayWndRect.Width()/2;
	int y =DisplayWndRect.Height()/2;

	CString str; 

	str.Format(_T("rotatio = %.2f"),CamCal.DualData_dif.dif_Rotation+0.005);
	pDisplayWndMemDC->TextOut(0,2*y-20,str); 
	str.Format(_T("tiltx = %.2f"),CamCal.DualData_dif.dif_TiltX+0.005);
	pDisplayWndMemDC->TextOut(x*2/3,2*y-20,str); 
	str.Format(_T("tilty = %.2f"),CamCal.DualData_dif.dif_TiltY+0.005);
	pDisplayWndMemDC->TextOut(x*4/3,2*y-20,str);  

	m_pInterface->Ctrl_ShowMemDC(CamID);
}
 
void DualParallelCamCheck_TestItem::SortPoints (Halcon::HTuple inX, Halcon::HTuple inY, Halcon::HTuple start, Halcon::HTuple end, Halcon::HTuple *outX, Halcon::HTuple *outY)
{
	using namespace Halcon; 
	// Local control variables 
	HTuple  tmp, i, j;

	tmp = 0;
	for (i=start; i<=end-1; i+=1)
	{
		for (j=i; j<=end; j+=1)
		{
			if (0 != (HTuple(inX[i])>HTuple(inX[j])))
			{
				tmp = inX[i];
				inX[i] = HTuple(inX[j]);
				inX[j] = tmp;

				tmp = inY[i];
				inY[i] = HTuple(inY[j]);
				inY[j] = tmp;
			}
		}
	}

	(*outX) = inX;
	(*outY) = inY;
	return;
}

void DualParallelCamCheck_TestItem::GetImgPoint_xyByHalconContours(int CamID,vector<float> *Point_xy)
{
	ImageInfo img = CamCal.CamInfo[CamID].img;
	for (int i=0; i<img.imgheight; i++)
	{
		for (int j=0; j<img.imgwidth; j++)
		{
			pBmpBlue[i*img.imgwidth+j] = img.RGB24_buffer[(i*img.imgwidth+j)*3];
			pBmpGreen[i*img.imgwidth+j] = img.RGB24_buffer[(i*img.imgwidth+j)*3+1];
			pBmpRed[i*img.imgwidth+j] = img.RGB24_buffer[(i*img.imgwidth+j)*3+2];
		}
	}

	Hobject  Image, GrayImage, ImageGauss, Edges;
	Hobject  SmoothedContours, SelectedContours, SelectedContoursFar;
	Hobject  SelectedContoursNear, SortedContoursFar, SortedContoursNear;

	gen_image3_extern(&Image, "byte", img.imgwidth, img.imgheight, (long)pBmpRed, (long)pBmpGreen, (long)pBmpBlue, (long)free);
	rgb1_to_gray(Image, &GrayImage);

	HTuple  Index, Width, Height;
	HTuple  Area, RowFar, ColumnFar, PointOrder, RowNear, ColumnNear;
	HTuple  LengthFar, LengthNear, outXNear, outYNear, outXFar;
	HTuple  outYFar, outXFarLength, outXNearLength;

	gauss_image(GrayImage, &ImageGauss, 11);
	edges_sub_pix(ImageGauss, &Edges, "canny", 3, 20, 40);
    smooth_contours_xld(Edges, &SmoothedContours, 5);
	select_contours_xld(SmoothedContours, &SelectedContours, "contour_length", 30, 200, -0.5, 0.5);
	select_contours_xld(SelectedContours, &SelectedContoursFar, "contour_length", 30, 105, -0.5, 0.5);
    select_contours_xld(SelectedContours, &SelectedContoursNear, "contour_length", 105, 200, -0.5, 0.5);
    sort_contours_xld(SelectedContoursFar, &SortedContoursFar, "upper_left", "true", "row");
    sort_contours_xld(SelectedContoursNear, &SortedContoursNear, "upper_left", "true", "row");
	area_center_xld(SortedContoursFar, &Area, &RowFar, &ColumnFar, &PointOrder);
    area_center_xld(SortedContoursNear, &Area, &RowNear, &ColumnNear, &PointOrder);

	tuple_length(RowFar, &outXFarLength);
	tuple_length(RowNear, &outXNearLength);
	int a = outXFarLength[0].I();
	int b = outXNearLength[0].I();
	if ((a != 54) || (b != 54))
		return;

	SortPoints(ColumnNear, RowNear, 0, 12, &outXNear, &outYNear);
    SortPoints(outXNear, outYNear, 13, 16, &outXNear, &outYNear);
    SortPoints(outXNear, outYNear, 17, 20, &outXNear, &outYNear);
    SortPoints(outXNear, outYNear, 21, 24, &outXNear, &outYNear);
    SortPoints(outXNear, outYNear, 25, 28, &outXNear, &outYNear);
    SortPoints(outXNear, outYNear, 29, 32, &outXNear, &outYNear);
    SortPoints(outXNear, outYNear, 33, 36, &outXNear, &outYNear);
    SortPoints(outXNear, outYNear, 37, 40, &outXNear, &outYNear);
    SortPoints(outXNear, outYNear, 41, 53, &outXNear, &outYNear);

    SortPoints(ColumnFar, RowFar, 0, 12, &outXFar, &outYFar);
    SortPoints(outXFar, outYFar, 13, 16, &outXFar, &outYFar);
    SortPoints(outXFar, outYFar, 17, 20, &outXFar, &outYFar);
    SortPoints(outXFar, outYFar, 21, 24, &outXFar, &outYFar);
    SortPoints(outXFar, outYFar, 25, 28, &outXFar, &outYFar);
    SortPoints(outXFar, outYFar, 29, 32, &outXFar, &outYFar);
    SortPoints(outXFar, outYFar, 33, 36, &outXFar, &outYFar);
    SortPoints(outXFar, outYFar, 37, 40, &outXFar, &outYFar);
    SortPoints(outXFar, outYFar, 41, 53, &outXFar, &outYFar);

	for (int i=0; i<54; i++)
	{
		Point_xy->push_back((float)outXNear[i].D());
		Point_xy->push_back((float)outYNear[i].D());
	}
	for (int i=0; i<54; i++)
	{
		Point_xy->push_back((float)outXFar[i].D());
		Point_xy->push_back((float)outYFar[i].D());
	}

	Image.~Hobject();
	GrayImage.~Hobject();
	ImageGauss.~Hobject();
	Edges.~Hobject();
	SmoothedContours.~Hobject();
	SelectedContours.~Hobject();
	SelectedContoursFar.~Hobject();
	SelectedContoursNear.~Hobject();
	SortedContoursFar.~Hobject();
	SortedContoursNear.~Hobject();
}

void DualParallelCamCheck_TestItem::GetImgPoint_xyByHalcon(int CamID,vector<float> *Point_xy)
{
	ImageInfo img = CamCal.CamInfo[CamID].img;
	for (int i=0; i<img.imgheight; i++)
	{
		for (int j=0; j<img.imgwidth; j++)
		{
			pBmpBlue[i*img.imgwidth+j] = img.RGB24_buffer[(i*img.imgwidth+j)*3];
			pBmpGreen[i*img.imgwidth+j] = img.RGB24_buffer[(i*img.imgwidth+j)*3+1];
			pBmpRed[i*img.imgwidth+j] = img.RGB24_buffer[(i*img.imgwidth+j)*3+2];
		}
	}

	// Local iconic variables 
	Hobject  Image, GrayImage, ImageGauss, ImageEmphasize;
	Hobject  Region, RegionFillUp, ConnectedRegions, SelectedRegions;
	Hobject  RegionsFar, RegionUnion, RegionUnionFar, RegionUnionNear;
	Hobject  RegionsNear, SortedRegionsFar, SortedRegionsNear,RegionBackground,RegionFillUpBackground,ImageReducedBackground;

	// Local control variables 
	HTuple  Area, Row, Column, Max, Min, RowFar, ColumnFar;
	HTuple  RowNear, ColumnNear, LengthFar, LengthNear, outXNear;
	HTuple  outYNear, outXFar, outYFar;
	HTuple  AbsoluteHisto, RelativeHisto, MinThresh, MaxThresh,MinThreshLenght;

	gen_image3_extern(&Image, "byte", img.imgwidth, img.imgheight, (long)pBmpRed, (long)pBmpGreen, (long)pBmpBlue, (long)free);
	rgb1_to_gray(Image, &GrayImage);
	 
	gray_histo(GrayImage, GrayImage, &AbsoluteHisto, &RelativeHisto);
	 
	while(1)
	{
		histo_to_thresh(RelativeHisto, HistoThreshold, &MinThresh, &MaxThresh);
	    tuple_length(MinThresh, &MinThreshLenght);
		int HistoNum = MinThreshLenght[0].I();
		if(HistoNum ==2)
		{
			break;
		}
		else
		{
			HistoThreshold +=4;
			if(HistoThreshold>30)
			{
				HistoThreshold = 0;
				return;
			}
		}
	}
	threshold(GrayImage, &RegionBackground, HTuple(MinThresh[1]), HTuple(MaxThresh[1]));
	fill_up(RegionBackground, &RegionFillUpBackground);
	reduce_domain(GrayImage, RegionFillUpBackground, &ImageReducedBackground);

	HTuple MinThreshSort = HTuple(MinThresh[0]);
	HTuple MaxThreshSort = HTuple(MaxThresh[0]);
	BOOL bGetPoint = FALSE;
	int PointNum = 54;
 
	threshold(ImageReducedBackground, &Region, MinThreshSort, MaxThreshSort);

	fill_up(Region, &RegionFillUp);
	connection(RegionFillUp, &ConnectedRegions);

	select_shape(ConnectedRegions, &SelectedRegions, (HTuple("area").Append("circularity")), "and", (HTuple(CamCal.CamInfo[CamID].farPointImgSize*0.6).Append(0.8)), (HTuple(99999).Append(1)));

	area_center(SelectedRegions, &Area, &Row, &Column);

	tuple_length(Row, &LengthFar);
	tuple_length(Column, &LengthNear);
	int a = LengthFar[0].I();
	int b = LengthNear[0].I();
	if ((a != 108) || (b != 108))
	{
		return;
	}
		 
	tuple_max(Area, &Max);
	tuple_min(Area, &Min);
	select_shape(SelectedRegions, &RegionsFar, "area", "and", Min, (Max+Min)/2);

	union1(SelectedRegions, &RegionUnion);
	union1(RegionsFar, &RegionUnionFar);
	difference(RegionUnion, RegionUnionFar, &RegionUnionNear);
	connection(RegionUnionNear, &RegionsNear);

	sort_region(RegionsFar, &SortedRegionsFar, "first_point", "true", "row");
	sort_region(RegionsNear, &SortedRegionsNear, "first_point", "true", "row");

	area_center(SortedRegionsFar, &Area, &RowFar, &ColumnFar);
	area_center(SortedRegionsNear, &Area, &RowNear, &ColumnNear); 

	HTuple  outX300, outY300, outX500, outY500;
	SortPoints(ColumnNear, RowNear, 0, 12, &outX300, &outY300);
	SortPoints(outX300, outY300, 13, 16, &outX300, &outY300);
	SortPoints(outX300, outY300, 17, 20, &outX300, &outY300);
	SortPoints(outX300, outY300, 21, 24, &outX300, &outY300);
	SortPoints(outX300, outY300, 25, 28, &outX300, &outY300);
	SortPoints(outX300, outY300, 29, 32, &outX300, &outY300);
	SortPoints(outX300, outY300, 33, 36, &outX300, &outY300);
	SortPoints(outX300, outY300, 37, 40, &outX300, &outY300);
	SortPoints(outX300, outY300, 41, 53, &outX300, &outY300);

	SortPoints(ColumnFar, RowFar, 0, 12, &outX500, &outY500);
	SortPoints(outX500, outY500, 13, 16, &outX500, &outY500);
	SortPoints(outX500, outY500, 17, 20, &outX500, &outY500);
	SortPoints(outX500, outY500, 21, 24, &outX500, &outY500);
	SortPoints(outX500, outY500, 25, 28, &outX500, &outY500);
	SortPoints(outX500, outY500, 29, 32, &outX500, &outY500);
	SortPoints(outX500, outY500, 33, 36, &outX500, &outY500);
	SortPoints(outX500, outY500, 37, 40, &outX500, &outY500);
	SortPoints(outX500, outY500, 41, 53, &outX500, &outY500);
	 

	for (int i=0; i<PointNum; i++)
	{
		Point_xy->push_back((float)outX300[i].D()); 
		Point_xy->push_back((float)outY300[i].D()); 
	}
	 
	for (int i=0; i<PointNum; i++)
	{
		Point_xy->push_back((float)outX500[i].D());
		Point_xy->push_back((float)outY500[i].D());
	}

	Image.~Hobject();
	Image.~Hobject();
	GrayImage.~Hobject();
	ImageGauss.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	RegionFillUp.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	RegionsFar.~Hobject();
	RegionUnion.~Hobject();
	RegionUnionFar.~Hobject();
	RegionUnionNear.~Hobject();
	RegionsNear.~Hobject();
	SortedRegionsFar.~Hobject();
	SortedRegionsNear.~Hobject();
	RegionBackground.~Hobject();
	RegionFillUpBackground.~Hobject();
	ImageReducedBackground.~Hobject();
}
 