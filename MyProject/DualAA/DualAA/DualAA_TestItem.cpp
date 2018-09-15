#include "StdAfx.h"
#include "DualAA_TestItem.h"
#include "ImageProc.h"
 
#pragma comment(lib,"halcon.lib")
#pragma comment(lib,"halconcpp.lib")

using namespace Halcon;

#pragma comment(lib,"mems_dualccm_twolayer\\mems_dualccm_twolayer.lib")

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new DualAA_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
DualAA_TestItem::DualAA_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

DualAA_TestItem::~DualAA_TestItem(void)
{

}


int DualAA_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption(); 
	failGetMarkTimes = 0;
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int DualAA_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码

	camGetTiltPass[0] = FALSE;
	camGetTiltPass[1] = FALSE;
  
	pImgInfo[0] = m_pInterface->Ctrl_GetImgInfo(0);
	pImgInfo[1] = m_pInterface->Ctrl_GetImgInfo(1);
	int pitch_chart = CamCal.Infinity_PointChartInfo.PointPitch;// mm
	int object_far = CamCal.Infinity_PointChartInfo.ObjectLength;// mm
	int object_near = CamCal.Macro_PointChartInfo.ObjectLength;// mm

	float EFL_0 = CamCal.CamInfo[0].baseInfo.FocusLength;
	float EFL_1 = CamCal.CamInfo[1].baseInfo.FocusLength;

	float pixelSize_0 = CamCal.CamInfo[0].baseInfo.PixelSize;
	float pixelSize_1 = CamCal.CamInfo[1].baseInfo.PixelSize; 

	CamCal.CamInfo[0].pitchFarImg = pitch_chart*EFL_0*1000/object_far/pixelSize_0;
	CamCal.CamInfo[0].pitchNearImg = pitch_chart*EFL_0*1000/object_near/pixelSize_0;

	CamCal.CamInfo[1].pitchFarImg = pitch_chart*EFL_1*1000/object_far/pixelSize_1;
	CamCal.CamInfo[1].pitchNearImg = pitch_chart*EFL_1*1000/object_near/pixelSize_1;

	CamCal.DualData_dif.dif_Rotation = 100;//初始化
	CamCal.DualData_dif.dif_TiltX = 100;
	CamCal.DualData_dif.dif_TiltY = 100;
	CamCal.CamInfo[0].m_DualCamData.Rotation = 100;
	CamCal.CamInfo[0].m_DualCamData.TiltX = 100;
	CamCal.CamInfo[0].m_DualCamData.TiltY = 100;

	CamCal.CamInfo[0].m_DualCamData.shift_x = -1000;
	CamCal.CamInfo[0].m_DualCamData.shift_y = -1000;
	CamCal.CamInfo[0].m_DualCamData.shift_z= -1000;

	CamCal.CamInfo[0].m_DualCamData.OpticalCenter.x = -1000;
	CamCal.CamInfo[0].m_DualCamData.OpticalCenter.y = -1000; 
	CamCal.CamInfo[1].m_DualCamData.Rotation = -100;
	CamCal.CamInfo[1].m_DualCamData.TiltX = -100;
	CamCal.CamInfo[1].m_DualCamData.TiltY = -100;

	CamCal.CamInfo[1].m_DualCamData.shift_x = -1000;
	CamCal.CamInfo[1].m_DualCamData.shift_y = -1000;
	CamCal.CamInfo[1].m_DualCamData.shift_z= -1000;

	CamCal.CamInfo[1].m_DualCamData.OpticalCenter.x = -1000;
	CamCal.CamInfo[1].m_DualCamData.OpticalCenter.y = -1000; 


	if(bOnAA)
	{
		CamCal.DualData_dif.BaseLine = 10;
		CamCal.DualData_dif.bResult = TRUE;
		CamCal.DualData_dif.dif_shift_z = 0;
		CamCal.DualData_dif.dif_shift_y = 0;
		CamCal.DualData_dif.dif_shift_y = 0;
		CamCal.DualData_dif.OpticalCenter.x = 0;
		CamCal.DualData_dif.OpticalCenter.y = 0;
		SetDualCamData();
	}

	failGetMarkTimes = 0;
	error_Limit = 0;

	CheckTimes = 0;

	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int DualAA_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	CString camFuseID = m_pInterface->Sensor_GetCamFuseID(1);
	m_pInterface->Ctrl_Addlog(CamID,camFuseID,COLOR_RED,200);
	if( GetTilt_dif()!= 0)
	{
		error_Limit++;
		return 1;
	}
	else if (error_Limit>10)
	{
		m_pInterface->Ctrl_Addlog(CamID,L"图片异常",COLOR_RED,200);
		return 0;
	}
	 	
	

   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int DualAA_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int DualAA_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 

	bContinue = ReadConfigInt(_T("bContinue"),0);
	bOnAA = ReadConfigInt(_T("bOnAA"),0); 

	blogProc = ReadConfigInt(_T("blogProc"),0); 

	CString str;   
	str=ReadConfigString(_T("m_MarkIndex"),_T("1.5"));
	pDlg->m_MarkIndex=wcstod(str,NULL);
	str=ReadConfigString(_T("m_FarMarkIndex"),_T("1.5"));
	pDlg->m_FarMarkIndex=wcstod(str,NULL);
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

	CamCal.CamInfo[1].baseInfo.strPixelSize = ReadConfigString(_T("Cam1_PixelSize"),_T("1.75"));
	CamCal.CamInfo[1].baseInfo.PixelSize = (float)_ttof(CamCal.CamInfo[1].baseInfo.strPixelSize);
	CamCal.CamInfo[1].baseInfo.strFocusLength = ReadConfigString(_T("Cam1_FocusLength"),_T("1.94"));
	CamCal.CamInfo[1].baseInfo.FocusLength = (float)_ttof(CamCal.CamInfo[1].baseInfo.strFocusLength);

	CamCal.Infinity_PointChartInfo.ObjectLength =ReadConfigInt(_T("InfinityObjectLength"),700); 
	 
	CamCal.Infinity_PointChartInfo.PointPitch = ReadConfigInt(_T("InfinityPointPitch"),50);
	 

	CamCal.Macro_PointChartInfo.ObjectLength = ReadConfigInt(_T("MacroObjectLength"),350); 
	 
	CamCal.Macro_PointChartInfo.PointPitch = ReadConfigInt(_T("MacroPointPitch"),50); 


	pDlg->offsetTx = ReadConfigString(_T("offsetTx"),_T("0"));	
	offset_tx = _ttof(pDlg->offsetTx);

	pDlg->offsetTy = ReadConfigString(_T("offsetTy"),_T("0"));	
	offset_ty = _ttof(pDlg->offsetTy);

	pDlg->offsetTz = ReadConfigString(_T("offsetTz"),_T("0"));	
    offset_tz = _ttof(pDlg->offsetTz);
	  
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int DualAA_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	CString str;    
	str.Format(_T("%f"),pDlg->m_MarkIndex);
	WriteConfigString(_T("m_MarkIndex"),str);
	str.Format(_T("%f"),pDlg->m_FarMarkIndex);
	WriteConfigString(_T("m_FarMarkIndex"),str);
	WriteConfigInt(_T("bOnAA"),bOnAA); 
	WriteConfigInt(_T("bContinue"),bContinue); 

	WriteConfigInt(_T("blogProc"),blogProc); 

	WriteConfigString(_T("RotationSpc"),CamCal.m_SpecData.strRotationSpc); 
	WriteConfigString(_T("TiltXSpc"),CamCal.m_SpecData.strTiltXSpc); 
	WriteConfigString(_T("TiltYSpc"),CamCal.m_SpecData.strTiltYSpc);


	WriteConfigString(_T("Cam0_PixelSize"),CamCal.CamInfo[0].baseInfo.strPixelSize);	 
	WriteConfigString(_T("Cam0_FocusLength"),CamCal.CamInfo[0].baseInfo.strFocusLength);

	WriteConfigString(_T("Cam1_PixelSize"),CamCal.CamInfo[1].baseInfo.strPixelSize);	 
	WriteConfigString(_T("Cam1_FocusLength"),CamCal.CamInfo[1].baseInfo.strFocusLength);

	WriteConfigInt(_T("InfinityObjectLength"),CamCal.Infinity_PointChartInfo.ObjectLength); 
	WriteConfigInt(_T("InfinityPointPitch"),CamCal.Infinity_PointChartInfo.PointPitch); 
	 

	WriteConfigInt(_T("MacroObjectLength"),CamCal.Macro_PointChartInfo.ObjectLength);  
	WriteConfigInt(_T("MacroPointPitch"),CamCal.Macro_PointChartInfo.PointPitch); 

	WriteConfigString(_T("offsetTx"),pDlg->offsetTx);	 
	WriteConfigString(_T("offsetTy"),pDlg->offsetTy);	 
	WriteConfigString(_T("offsetTz"),pDlg->offsetTz);	 

  


	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR DualAA_TestItem::GetReportHeads()
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
LPCTSTR DualAA_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	CString temp;

	temp.Format(_T("-%4lf,"),0);
	LowerLimit += temp;
	temp.Format(_T("-%4lf,"),0);
	LowerLimit += temp;
	temp.Format(_T("-%4lf"),0);			 
	LowerLimit += temp; 

	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR DualAA_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	CString temp;

	temp.Format(_T("%4lf,"),CamCal.m_SpecData.RotationSpc);
	UpperLimit += temp;
	temp.Format(_T("%4lf,"),CamCal.m_SpecData.TiltXSpc);
	UpperLimit += temp;
	temp.Format(_T("%4lf"),CamCal.m_SpecData.TiltYSpc);			 
	UpperLimit += temp; 

	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR DualAA_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	CString temp;

	temp.Format(_T("%.2lf,"),(CamCal.DualData_dif.dif_Rotation)+0.005);
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


void DualAA_TestItem::SetDualCamData()
{  
	m_pInterface->Ctrl_SetDualCamData(CamID,CamCal.DualData_dif);
}

int DualAA_TestItem::GetCamTilt(int CamID)
{
	pImgInfo[CamID] = NULL;
	pImgInfo[CamID] = m_pInterface->Ctrl_GetImgInfo(CamID);
	//pImgInfo[1] = m_pInterface->Ctrl_GetImgInfo(1);

	ImageInfo* img = pImgInfo[CamID];

	if(img->RGB24_buffer== NULL)
	{
		img = NULL;
		return -1;
	}

	 
	float ChartDist = (float)(CamCal.Infinity_PointChartInfo.ObjectLength - CamCal. Macro_PointChartInfo.ObjectLength);

	vector<fCPoint> nearPoint_xy; 
	vector<fCPoint> farPoint_xy; 
	GetImgPoint(CamID,nearPoint_xy,farPoint_xy);  
	
	if(nearPoint_xy.size()<30||farPoint_xy.size()<30)
	{
		nearPoint_xy.clear();
		farPoint_xy.clear();
		m_pInterface->Ctrl_Addlog(CamID,L"模型不匹配",COLOR_RED,200);
		saveProcImage(CamID,L"MODEL FAIL");
		failGetMarkTimes++;
		//m_pInterface->Ctrl_SaveImage(CamID,0,0,L"modual FAIL");
		return -1;
	}
	 
	fCPoint nearMark_xy,farMark_xy;
	
	vector< vector<fCPoint>> NearImgCoordinate;
	vector< vector<fCPoint>> FarImgCoordinate;

	vector<fCPoint> invalidPoint_near;
	vector<fCPoint> invalidPoint_far;
	float pitch_n = CamCal.CamInfo[CamID].pitchNearImg;
	float pitch_f = CamCal.CamInfo[CamID].pitchFarImg;

	vector<fCPoint>::iterator it;
	for(it=nearPoint_xy.begin();it!=nearPoint_xy.end();it++)
	{
		float edgeLimmit = pitch_n*0.25;
		if (it->x<edgeLimmit || abs(it->x-img->imgwidth)<edgeLimmit || it->y<edgeLimmit || abs(it->y-img->imgheight)<edgeLimmit)
		{
			if (it!=nearPoint_xy.begin())
			{
				vector<fCPoint>::iterator it_ = it-1;
				nearPoint_xy.erase(it);
				it = it_;
			}
			else
			{ 
				if(nearPoint_xy.size()>0) 
					it = nearPoint_xy.begin();	
				else
				{
					break;
				}
			}
			
		}
	}

    for(it=farPoint_xy.begin();it!=farPoint_xy.end();it++)
	{
		float edgeLimmit = pitch_f*0.5;
		if (it->x<edgeLimmit || abs(it->x-img->imgwidth)<edgeLimmit || it->y<edgeLimmit || abs(it->y-img->imgheight)<edgeLimmit)
		{
			if (it!=farPoint_xy.begin())
			{
				vector<fCPoint>::iterator it_ = it-1;
				farPoint_xy.erase(it);
				it = it_;
			}
			else
			{
				farPoint_xy.erase(it);
				if(farPoint_xy.size()>0) 
					it = farPoint_xy.begin();	
				else
				{
					break;
				}
			}
			
		}
	}
	
	if(!ImgPoint_xyProc(CamID,nearPoint_xy,NearImgCoordinate,pitch_n, invalidPoint_near,0))
	{
		nearPoint_xy.clear();
		m_pInterface->Ctrl_Addlog(CamID,L"near mark get fail",COLOR_RED,200);
		saveProcImage(CamID,L"near mark fail");
		//m_pInterface->Ctrl_SaveImage(CamID,0,0,L"NEAR MARK FAIL");
		failGetMarkTimes++;
		if(blogProc)
		{
			for (UINT i=0;i<NearImgCoordinate.size();i++)
			{
				for (UINT j=0;j<NearImgCoordinate[i].size();j++)
				{
					if (NearImgCoordinate[i][j].PointSize==1)				 
						nearPoint_xy.push_back(NearImgCoordinate[i][j]);
				}
			}
			showCrossCoordinate(CamID,nearPoint_xy);
		}
		
		return -1;
	}
	if(!ImgPoint_xyProc(CamID,farPoint_xy,FarImgCoordinate, pitch_f,invalidPoint_far,1 ))
	{
		farPoint_xy.clear();
		failGetMarkTimes++;
		m_pInterface->Ctrl_Addlog(CamID,L"far mark get fail",COLOR_RED,200);
		saveProcImage( CamID,L"far mark fail");
		//m_pInterface->Ctrl_SaveImage(CamID,0,0,L"FAR MARK FAIL");
		if(blogProc)
		{
			for (UINT i=0;i<FarImgCoordinate.size();i++)
			{
				for (UINT j=0;j<FarImgCoordinate[i].size();j++)
				{
					if (FarImgCoordinate[i][j].PointSize==1)
						farPoint_xy.push_back(FarImgCoordinate[i][j]);
				}
			}
			showCrossCoordinate(CamID,farPoint_xy);
		}		
		return -1;
	}
	 
	CPoint nearMark_ij,farMark_ij; 
	GetMark_xy( CamID,invalidPoint_near,nearMark_xy ,invalidPoint_far,farMark_xy);
	GetMark_ij( NearImgCoordinate,nearMark_xy,nearMark_ij,pitch_n/2);
	GetMark_ij( FarImgCoordinate,farMark_xy,farMark_ij,pitch_f/2);

	vector<CString> imgNearPoint_ij,imgFarPoint_ij;
	GetImgPoint_xy_ij( NearImgCoordinate,nearMark_ij, imgNearPoint_ij);
	GetImgPoint_xy_ij( FarImgCoordinate,farMark_ij, imgFarPoint_ij);

	vector<CString> union_ij;
	matchNearFar(imgNearPoint_ij,imgFarPoint_ij,union_ij);

	vector<float>  img_xy;
	GetImg_xy(NearImgCoordinate,nearMark_ij,union_ij,img_xy);
	GetImg_xy(FarImgCoordinate,farMark_ij,union_ij,img_xy);
	
#if 0
	float ratio(1.5), height(1.2);
	for (UINT i=0;i<img_xy.size()/2-2;i+=2)
	{
		corrdinateOffset(  ratio,  height,img_xy[i], 700, 3.79,2104);
		corrdinateOffset(  ratio,  height,img_xy[i+1], 700, 3.79,1560);
	}

	for (UINT i=img_xy.size()/2-2;i<img_xy.size();i+=2)
	{
		corrdinateOffset(  ratio,  height,img_xy[i], 350, 3.79,2104);
		corrdinateOffset(  ratio,  height,img_xy[i+1], 350, 3.79,1560);
	}
#endif
	
	vector<int>  object_xyz;
	createObject_xyz(CamID,union_ij,object_xyz);

	vector<fCPoint> img_coordinate;
	
	for (UINT i=0;i<img_xy.size();i+=2)
	{
		fCPoint fcp;
		fcp.x = img_xy[i];
		fcp.y = img_xy[i+1];
		fcp.PointSize = 1;
		img_coordinate.push_back(fcp);
	}
	//显示最终匹配的坐标
	showCrossCoordinate(CamID,img_coordinate);
	  

	UINT chart_xyz_len = object_xyz.size();
	float* chart_xyz = new float[chart_xyz_len];
	memset(chart_xyz,0,sizeof(float)*chart_xyz_len);
	for (UINT i=0;i<chart_xyz_len;i++)
	{
		chart_xyz[i] = object_xyz[i];
	}
	 
	UINT CamImgPoint_xy_len = img_xy.size();
	float* CamImgPoint_xy = new float[CamImgPoint_xy_len];
	memset(CamImgPoint_xy,0,sizeof(float)*CamImgPoint_xy_len);
	for (UINT i=0;i<CamImgPoint_xy_len;i++)
	{
		CamImgPoint_xy[i] = img_xy[i];
	}
	  
	CamCal.CamInfo[CamID].m_IN_PARAMS.focal_length_mm =   CamCal.CamInfo[CamID].baseInfo.FocusLength;
	CamCal.CamInfo[CamID].m_IN_PARAMS.pixel_size_um = CamCal.CamInfo[CamID].baseInfo.PixelSize;
	CamCal.CamInfo[CamID].m_IN_PARAMS.img_height =  img->imgheight;
	CamCal.CamInfo[CamID].m_IN_PARAMS.img_width =   img->imgwidth;

	CamCal.CamInfo[CamID].m_IN_PARAMS.imgPoint_xy = CamImgPoint_xy;
	CamCal.CamInfo[CamID].m_IN_PARAMS.imgPoint_xySize = CamImgPoint_xy_len;

	CamCal.CamInfo[CamID].m_IN_PARAMS.Oblect_xyz = chart_xyz;
	CamCal.CamInfo[CamID].m_IN_PARAMS.Oblect_xyzSize = chart_xyz_len;

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
	SAFE_DELETE_ARRAY(chart_xyz);
	SAFE_DELETE_ARRAY(CamImgPoint_xy);
	return 0;
}


int DualAA_TestItem::GetTilt_dif()
{   
	int Flag = 0;

	if (failGetMarkTimes>5)
	{
		 EndTest();
		 return 0;
	}

	if (!camGetTiltPass[0])
	{
		Flag = GetCamTilt(0);
		if (Flag!=0)
		{
			failGetMarkTimes++;
			Sleep(300);
			camGetTiltPass[0] = FALSE;
			return -1;
		}

		camGetTiltPass[0] = TRUE;
	}
	
	if (!camGetTiltPass[1])
	{
		Flag = GetCamTilt(1);
		if (Flag!=0)
		{
			failGetMarkTimes++;
			camGetTiltPass[1] = FALSE;
			Sleep(300);
			return -1;
		} 
		camGetTiltPass[1] = TRUE;
	}
	   
	CamCal.DualData_dif.dif_Rotation = CamCal.CamInfo[1].m_DualCamData.Rotation -  CamCal.CamInfo[0].m_DualCamData.Rotation+ offset_tz;
	CamCal.DualData_dif.dif_TiltX = CamCal.CamInfo[1].m_DualCamData.TiltX -  CamCal.CamInfo[0].m_DualCamData.TiltX+ offset_tx;
	CamCal.DualData_dif.dif_TiltY = CamCal.CamInfo[1].m_DualCamData.TiltY -  CamCal.CamInfo[0].m_DualCamData.TiltY+ offset_ty; 


	CamCal.DualData_dif.BaseLine = CamCal.DualData_dif.dif_shift_x;
	if(bOnAA)
		m_pInterface->Ctrl_SetDualCamData(CamID,CamCal.DualData_dif);

	return ProcTiltData();

}

int DualAA_TestItem::ProcTiltData(BOOL bShowInfo)
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
		saveProcImage(0,L"AA pass");
		saveProcImage(1,L"AA pass");
		tiltData.push_back(CamCal.DualData_dif.dif_TiltX*1000);
		tiltData.push_back(CamCal.DualData_dif.dif_TiltY*1000);
		tiltData.push_back(CamCal.DualData_dif.dif_Rotation*1000);
		m_pInterface->Otp_WriteToOtpBoard(CamID,GetName(),tiltData,TRUE);
		if(!bShowInfo)
		{
			return 0;
		}

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
		if(!bOnAA&&CheckTimes<5)
		{
			saveProcImage(0,L"AA  check fail");
			saveProcImage(1,L"AA  check fail");

			CheckTimes++;
			Sleep(500);
			camGetTiltPass[0] = FALSE;
			camGetTiltPass[1] = FALSE;

			return 1;
		}
		if(!bOnAA)
		{
			SetbStopTest(TRUE); 
		} 
		SetResult(RESULT_FAIL);

		if(!bShowInfo)
		{
			return 0;
		}
	}
	ShowInfo(0);
	ShowInfo(1);
	return 0;

}

void DualAA_TestItem::saveProcImage(int CamID,CString name)
{
	if(!blogProc) return;
	CString FileName;
	CString camFuseID = m_pInterface->Sensor_GetCamFuseID(0);
	FileName.Format(L"%s_%s_%d",camFuseID,name,CamID);
	CString currentPath = m_pInterface->Ctrl_GetCurrentPath();
	CString fullImageSavePath = currentPath +  + L"ImgSave\\";

	int ImgTypeSel = 1;//JPG
	m_pInterface->Ctrl_SaveImage( CamID, 1, fullImageSavePath, FileName, ImgTypeSel, NULL);   
}

void DualAA_TestItem::EndTest()
{
	SetbStopTest(TRUE);
	SetResult(RESULT_FAIL);
	 
	saveProcImage(0,L"EndTest");
	saveProcImage(1,L"EndTest");

	/*if(!bOnAA)
	{
		CamCal.DualData_dif = *m_pInterface->Ctrl_GetDualCamData(CamID);
		 ProcTiltData(FALSE);
	}*/
//	m_pInterface->Ctrl_SetDualCamData(CamID,CamCal.DualData_dif);
	 
}

void DualAA_TestItem::ShowInfo(int CamID)
{
	if (!blogProc)
	{
		return ;
	}
	//CamID = 0;
	ImageInfo* img = pImgInfo[CamID];

	if (img->RGB24_buffer==NULL)
	{
		return;
	}
	 
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
	int ImgWidth  = img->imgwidth;
	int ImgHeight = img->imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight; 
  
	int x = DisplayWndRect.Width()/2;
	int y = DisplayWndRect.Height()/2;

	CString str; 
	  
	str.Format(_T("rotatio = %.2f"),CamCal.CamInfo[CamID].m_DualCamData.Rotation+0.005);
	pDisplayWndMemDC->TextOut(0,2*y-20,str); 
	str.Format(_T("tiltx = %.2f"),CamCal.CamInfo[CamID].m_DualCamData.TiltX+0.005);
	pDisplayWndMemDC->TextOut(x*2/3,2*y-20,str); 
	str.Format(_T("tilty = %.2f"),CamCal.CamInfo[CamID].m_DualCamData.TiltY+0.005);
	pDisplayWndMemDC->TextOut(x*4/3,2*y-20,str);  

	m_pInterface->Ctrl_ShowMemDC(CamID);
}

void DualAA_TestItem::GetImgPoint(int CamID, vector<fCPoint> &NearImgPoint, vector<fCPoint> &FarImgPoint)
{
	LogProc(L"GetImgPoint");
	CString nearModelName;
	CString farModelName;

	CString shapeModeName;
	
#if 1  //使用多个model shaple
	 
	vector<CString> nearModelPath;
	vector<CString> farModelPath;

	vector<vector<vector<fCPoint>>> _ImgPoint;
	vector<vector<CString>> _ModelPath;


	float pitch_n = CamCal.CamInfo[CamID].pitchNearImg/10.0;
	float pitch_f = CamCal.CamInfo[CamID].pitchFarImg/ 10.0;

	for (int i=0;i<3;i++)
	{
		shapeModeName.Format(L"Cam%dFarModel%d.shm",CamID,i);
		farModelName = m_pInterface->Ctrl_GetCurrentPath()+L"ImgSave\\" + shapeModeName;
		farModelPath.push_back(farModelName);

		shapeModeName.Format(L"Cam%dNearModel%d.shm",CamID,i);
		nearModelName = m_pInterface->Ctrl_GetCurrentPath()+L"ImgSave\\" + shapeModeName;
		nearModelPath.push_back(nearModelName); 
	}

	_ModelPath.push_back(nearModelPath);
	_ModelPath.push_back(farModelPath);
	BOOL flag = findShapeModel(CamID,_ModelPath,&_ImgPoint);
	 
	UINT arraySize = _ImgPoint.size();
	if (!flag||arraySize!=2)
	{
		return;
	}
	for (UINT j=0;j<arraySize;j++)
	{
		vector<fCPoint> tempImgPoint;
		for (UINT i=0;i<_ImgPoint[j].size();i++)
		{
			union_Array(_ImgPoint[j][i],tempImgPoint,pitch_n);
			//union_Array(tempFarImgPoint[i],tempImgPoint,pitch_f); 
		}
		if (j==0)
		{
			NearImgPoint = tempImgPoint;
		}
		else
		{
			FarImgPoint = tempImgPoint;
		}
	}
	
	 
	for (int i=0;i<NearImgPoint.size();i++)
	{
		NearImgPoint[i].x = NearImgPoint[i].x/(float)NearImgPoint[i].PointSize;
		NearImgPoint[i].y = NearImgPoint[i].y/(float)NearImgPoint[i].PointSize;
		NearImgPoint[i].PointSize = 1;
	}

	for (int i=0;i<FarImgPoint.size();i++)
	{
		FarImgPoint[i].x = FarImgPoint[i].x/(float)FarImgPoint[i].PointSize;
		FarImgPoint[i].y = FarImgPoint[i].y/(float)FarImgPoint[i].PointSize;
		FarImgPoint[i].PointSize = 1;
	}
	
	
  
#else
	 CString imgpathN,imgpathF;
	 imgpathN.Format(L"%s\\Cam%dN.jpg",m_pInterface->Ctrl_GetCurrentPath()+L"ImgSave",CamID);
	 imgpathF.Format(L"%s\\Cam%dF.jpg",m_pInterface->Ctrl_GetCurrentPath()+L"ImgSave",CamID);
	 
	 findShapeModel(imgpathN,nearModelName,&NearImgPoint);
	 findShapeModel(imgpathF,farModelName,&FarImgPoint);
#endif
}
  
void DualAA_TestItem::union_Array(vector<fCPoint> srcImgPoint,vector<fCPoint> &ImgPoint,float pitch_)
{
	LogProc(L"union_Array");
	for (UINT j=0;j<srcImgPoint.size();j++)
	{   
		BOOL bFind = FALSE;
		for (UINT k=0;k<ImgPoint.size();k++)
		{
			int pintSize = ImgPoint[k].PointSize;
			float x_dif = srcImgPoint[j].x - ImgPoint[k].x/pintSize;
			float y_dif = srcImgPoint[j].y - ImgPoint[k].y/pintSize;
			if (abs(x_dif)< pitch_ && abs(y_dif)< pitch_)
			{
				ImgPoint[k].x =  (srcImgPoint[j].x +  ImgPoint[k].x);
				ImgPoint[k].y =  (srcImgPoint[j].y +  ImgPoint[k].y);
				ImgPoint[k].PointSize++;
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)
		{
			ImgPoint.push_back(srcImgPoint[j]); 
		}
	}
}

void DualAA_TestItem::ImgPointGetRow(int CamID,vector<fCPoint> &ImgPoint,vector<fCPoint> &ImgRowPoint,float avg_y,int range)
{
	LogProc(L"ImgPointGetRow");
	ImgRowPoint.clear();
	vector<fCPoint>::iterator it; 
	//因为已经y做了升序排列，所以只要不满足 abs(it->y - avg_y) < range 就可以break；
	if(ImgPoint.size()<1) return;
	it = ImgPoint.begin();
	while(abs(it->y - avg_y) < range)
	{
		 
		avg_y = it->y;
		ImgRowPoint.push_back(*it);
		ImgPoint.erase(it);
		if (ImgPoint.size()<1)
		{
			break;
		}
		 
		if(ImgPoint.size()>0) 
			it = ImgPoint.begin();
		else
			break;

	} 
}
BOOL DualAA_TestItem::ImgPoint_xyProc(int CamID,vector<fCPoint> &ImgPoint,vector< vector<fCPoint>> &ImgCoordinate,int pitch_y,vector<fCPoint> &invalidPoint,BOOL Disflag)
{
	LogProc(L"ImgPoint_xyProc");
	//
	ImgCoordinate.clear();
	invalidPoint.clear();
	  
	vector<fCPoint> row_invalidPoint;

	vector<fCPoint> rowPoint;	 
	ReOrderfCpointAscend_y(ImgPoint);
	float avgTarget = ImgPoint[0].y; 
	int rowcnt=0;
	CString strInf;
	while(ImgPoint.size()>0)
	{
		row_invalidPoint.clear();
		BOOL bRe = TRUE;
		ImgPointGetRow( CamID,ImgPoint,rowPoint, avgTarget, pitch_y*0.3);
		if (rowPoint.size()>0)
		{
			/*GetAvgPoint_y(rowPoint,avgTarget);
			avgTarget += pitch_y;*/
			 ReOrderfCpointAscend_x(rowPoint);//重排
			 rowcnt++;
			 strInf.Format(_T("第%d行，共有%d个圆圈"),rowcnt,rowPoint.size());
			 LogProc(strInf);
			if (rowPoint.size()>5)
			{
				bRe = RowPointCheck(rowPoint,pitch_y,row_invalidPoint,ImgPoint,Disflag);

				if(ImgCoordinate.size()==0 && row_invalidPoint.size()!=0)//第一排不能有invalidate point
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("第一排不能有无效点!"),COLOR_RED,200);
					row_invalidPoint.clear();
					continue;
				}
// 				else if (row_invalidPoint.size()!=0 && ImgPoint.size()<rowPoint.size()&&ImgPoint.size()>0)//最后一排不能有invalidate point
// 				{
// 					m_pInterface->Ctrl_Addlog(CamID,_T("最后一排不能有无效点!"),COLOR_RED,200);
// 					row_invalidPoint.clear();
// 					continue;
// 				}
				else
				{
					ImgCoordinate.push_back(rowPoint);
					for (UINT i=0;i<row_invalidPoint.size();i++)
					{
						invalidPoint.push_back(row_invalidPoint[i]);
					} 
					
				}
				 
				//显示对齐以后的坐标
				//showCrossCoordinate(CamID,rowPoint);
			}
			
		} 
		if (!bRe)
		{
			ReOrderfCpointAscend_y(ImgPoint);
		}
		
		if(ImgPoint.size()>0)
		{
			avgTarget = ImgPoint[0].y; 
		}
		else
		{
			break;
		}
		 
	}  
	AlignArrayRow(ImgCoordinate, pitch_y);
	if(invalidPoint.size()!=1 || ImgCoordinate.size()<5)
	{ 
		return FALSE;
	}
	return TRUE;
	//显示对齐后的坐标
#if 0
	for (UINT i=0;i<ImgCoordinate.size();i++)
	{
		showCrossCoordinate(CamID,ImgCoordinate[i]);
	}
#endif
	


}

void DualAA_TestItem::GetAvgPoint_y(vector<fCPoint> ImgPoint,float &avg_y)
{
	LogProc(L"GetAvgPoint_y");
	avg_y = 0;
	UINT rowSize = ImgPoint.size();
	for (UINT i=0;i<rowSize;i++)
	{
		avg_y += ImgPoint[i].y/rowSize;
	}
}
 

BOOL DualAA_TestItem::GetMark_xy(int CamID,vector<fCPoint> invalidPoint_near,fCPoint &markNear_xy,vector<fCPoint> invalidPoint_far,fCPoint &markFar_xy)
{  
	LogProc(L"GetMark_xy");
	if(invalidPoint_far.size()!=1 || invalidPoint_near.size()!=1)
	{
		return FALSE;
	}
	markNear_xy = invalidPoint_near[0];
	markFar_xy = invalidPoint_far[0];
	
	if (markNear_xy.x==0||markNear_xy.y==0||markFar_xy.x==0||markFar_xy.y==0)
	{
		return FALSE;
	}	

	return TRUE;
}

void DualAA_TestItem::GetMark_ij(vector< vector<fCPoint>> ImgCoordinate,fCPoint mark_xy, CPoint &mark_ij,int pitch_)
{

	LogProc(L"GetMark_ij");
	 for (UINT i=0;i<ImgCoordinate.size();i++)
	 {
		 if (abs(ImgCoordinate[i][0].y - mark_xy.y)<pitch_)
		 {
			 mark_ij.y  =i;
			 for (UINT j=0;j<ImgCoordinate[i].size();j++)
			 {
				 if (abs(ImgCoordinate[i][j].x - mark_xy.x)<pitch_)
				 {
					  mark_ij.x  =j;
					  break;
				 }
			 }
		 }
		
	 }
}

void DualAA_TestItem::GetImgPoint_xy_ij(vector< vector<fCPoint>> ImgCoordinate, CPoint mark_ij,vector<CString> &imgPoint_ij)
{
	LogProc(L"GetImgPoint_xy_ij");
	imgPoint_ij.clear();
	for (UINT i=0;i<ImgCoordinate.size();i++)
	{
		for (UINT j=0;j<ImgCoordinate[i].size();j++)
		{
			if (ImgCoordinate[i][j].PointSize==1)
			{
				CString indexStr;
				indexStr.Format(L"%d_%d",j - mark_ij.x,i - mark_ij.y) ; 
				imgPoint_ij.push_back(indexStr);
			}
		}
	}
}

void DualAA_TestItem::matchNearFar(vector<CString> imgNearPoint_ij,vector<CString> imgFarPoint_ij,vector<CString> &union_ij)
{
	LogProc(L"matchNearFar");
	union_ij.clear();
	for (UINT i=0;i<imgNearPoint_ij.size();i++)
	{
		for (UINT j=0;j<imgFarPoint_ij.size();j++)
		{
			if (imgNearPoint_ij[i]==imgFarPoint_ij[j])
			{
				union_ij.push_back(imgNearPoint_ij[i]);
			}
		}
	}
}

void DualAA_TestItem::GetImg_xy(vector< vector<fCPoint>> ImgCoordinate,CPoint mark_ij,vector<CString> union_ij,vector<float> &img_xy)
{ 
	LogProc(L"GetImg_xy");
	int indexUnio = 0;
	for (UINT i=0;i<ImgCoordinate.size();i++)
	{
		for (UINT j=0;j<ImgCoordinate[i].size();j++)
		{
			CString indexStr;
			indexStr.Format(L"%d_%d",j - mark_ij.x,i - mark_ij.y) ;
			for (UINT index = indexUnio;index<union_ij.size();index++)
			{
				if (indexStr==union_ij[index])
				{
					indexUnio = index;
					img_xy.push_back(ImgCoordinate[i][j].x);
					img_xy.push_back(ImgCoordinate[i][j].y);
					break;
				}
			}
			
		}
	}
}

void DualAA_TestItem::createObject_xyz(int CamID,vector<CString> union_ij,vector<int> &object_xyz)
{
	LogProc(L"createObject_xyz");
	int pitch_ = CamCal.Macro_PointChartInfo.PointPitch;
	int distPitch = CamCal.Infinity_PointChartInfo.ObjectLength - CamCal.Macro_PointChartInfo.ObjectLength;
	int offset = 0;
	vector<fCPoint> obj_ij;
	for (UINT i=0;i<union_ij.size();i++)
	{
		vector<CString> ij_;
		ImageProc::SeparateString(union_ij[i],'_',&ij_);
		object_xyz.push_back((_ttoi(ij_[0])+offset)*pitch_);
		object_xyz.push_back((_ttoi(ij_[1])+offset)*pitch_);
		object_xyz.push_back(0); //0
		  
	}
	//显示物理坐标
	//showCrossCoordinate(CamID,obj_ij);
	for (UINT i=0;i<union_ij.size();i++)
	{
		vector<CString> ij_;
		ImageProc::SeparateString(union_ij[i],'_',&ij_);
		object_xyz.push_back((_ttoi(ij_[0])+offset)*pitch_);
		object_xyz.push_back((_ttoi(ij_[1])+offset)*pitch_);
		object_xyz.push_back(distPitch); //distPitch
	}
}
 

//升序排列
void DualAA_TestItem::SortVecAscend(vector<float> vecItem,vector<float> &vecAscendOrder,vector<int> &Index)
{
	LogProc(L"SortVecAscend");
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

void DualAA_TestItem::ReOrderfCpointAscend_x(vector<fCPoint> &vecItem)
{
	LogProc(L"ReOrderfCpointAscend_x");
	vector<float> x_;
	vector<float> x_out;
	vector<int> index_;
	for (UINT i=0;i<vecItem.size();i++)
	{
		x_.push_back(vecItem[i].x);
	}
	SortVecAscend( x_, x_out, index_);
	vector<fCPoint> vecItem_ = vecItem;
	vecItem.clear();
	for (UINT i=0;i<index_.size();i++)
	{
		int oIndex = index_[i];
		vecItem.push_back(vecItem_[oIndex]);
	}
}

void DualAA_TestItem::ReOrderfCpointAscend_y(vector<fCPoint> &vecItem)
{
	LogProc(L"ReOrderfCpointAscend_y");
	vector<float> y_;
	vector<float> y_out;
	vector<int> index_y;
	for (UINT i=0;i<vecItem.size();i++)
	{
		y_.push_back(vecItem[i].y);
	}
	SortVecAscend( y_, y_out, index_y);
	vector<fCPoint> vecItem_ = vecItem;
	vecItem.clear();
	for (UINT i=0;i<index_y.size();i++)
	{
		int oIndex = index_y[i];
		vecItem.push_back(vecItem_[oIndex]);
	}
}
 
BOOL DualAA_TestItem::RowPointCheck(vector<fCPoint> &vecItem,int pitch_,vector<fCPoint> & InvalidPoint,vector<fCPoint> &srcPoint,BOOL Disflag)
{ 
	LogProc(L"RowPointCheck");
	 vector<fCPoint>::iterator it;
	 BOOL bOk = FALSE;
	 BOOL bRe = TRUE;
	 int MaxDis=0;
	 float index=0;
	 if(Disflag==0) index=pDlg->m_MarkIndex;
	 else index=pDlg->m_FarMarkIndex;
	 while(!bOk)
	 {
		 bOk = TRUE;
		 if (vecItem.size()<3)
		 {
			 bRe = FALSE; 
			 break;
		 }
		 for (it =vecItem.begin();it!=vecItem.end()-1;it++)
		 {
			
			 int a=abs(it->x - (it+1)->x);
			 int b=abs(it->y - (it+1)->y);
			 if (MaxDis<abs(it->x - (it+1)->x))  MaxDis=abs(it->x - (it+1)->x);

			 if ( abs(it->x - (it+1)->x) >pitch_*index && abs(it->y - (it+1)->y)<pitch_*0.2)//添加
			 {
				 fCPoint fcp;
				 fcp.x = (it->x + (it+1)->x)/2;
				 fcp.y = (it->y + (it+1)->y)/2;
				 fcp.PointSize = 0;//区分抓到的坐标 ，将不参与计算tilt。
				 vecItem.insert(it+1,fcp); 
				 InvalidPoint.push_back(fcp);
				 m_pInterface->Ctrl_Addlog(CamID,L"get invalidate  point",COLOR_BLUE,200);
				 bOk = FALSE;
				 break;
			 }
			 else if (abs(it->x - (it+1)->x) <pitch_*0.6 || abs(it->y - (it+1)->y) >pitch_*0.3)//去除
			 { 
				 if (it==vecItem.begin())//需要三个来判定哪个为异常点
				 {
					 if (vecItem.size()<3)
					 {
						 bOk = TRUE;
						 break;
					 }
					 if (abs((it+2)->y - (it+1)->y) >pitch_*0.2)//
					 {
						  
						 srcPoint.push_back(*(it+1));					  
						 vecItem.erase(it+1); 
						 bOk = FALSE;
						 bRe = FALSE;
						 break;
					 }
					 else
					 {
						 srcPoint.push_back(*(it));
						 vecItem.erase(it); // 
						 bOk = FALSE;
						 bRe = FALSE;
						 break;
					 }
				 } 
				 else
				 {
					 srcPoint.push_back(*(it+1));// 
					 vecItem.erase(it+1); 
					 bOk = FALSE;
					 bRe = FALSE;
					 break;
				 }
				 
			 }
		 }
		 CString strInf;
		 strInf.Format(_T("最大点间距为：%d，规格为：%.2f,最大点间距必须大于规格!"),MaxDis,pitch_*index);
		 LogProc(strInf);
	 }
	 return bRe;
	 
}
 
void DualAA_TestItem::AlignArrayRow(vector<  vector<fCPoint>> &arrayRow,int pitch_)
{
	LogProc(L"AlignArrayRow");
	vector<  vector<fCPoint>>::iterator it;

	BOOL bAlign = FALSE;
	while(!bAlign)
	{
		bAlign = TRUE;
		if(arrayRow.size()<5)
		{
			break;
		}
		for (it = arrayRow.begin();it != arrayRow.end()-1;it++)
		{
			if (it->size()<5)
			{
				arrayRow.erase(it);
				bAlign = FALSE;
				break; 
			}

			if ((it+1)->size()<5)
			{
				arrayRow.erase(it+1);
				bAlign = FALSE;
				break; 
			}

			//***      //***
			/***/      ///**
			if (abs(it->at(0).x - (it+1)->at(0).x)>pitch_*0.5)
			{
				//***
				/***/
				if (it->at(0).x>(it+1)->at(0).x)
				{
					if (it!=arrayRow.begin())
					{
						CString str;
						str.Format(L"cp_x:%.4f,cp_y:%.4f",(it+1)->begin()->x,(it+1)->begin()->y);
						//m_pInterface->Ctrl_Addlog(CamID,L"arrayRow.begin()_BEGIN"+str,COLOR_RED,200);
						LogProc(L"1_BEGIN");
						(it+1)->erase((it+1)->begin());	
						LogProc(L"1_BEGIN PASS");
					}
					else
					{
						LogProc(L"2_BEGIN");
						arrayRow.erase(it);
						LogProc(L"2_BEGIN PASS");
					}
			 		
				}
				else if ((it+1)!= arrayRow.end()-1)
				{
					//***
					///**
					CString str;
					str.Format(L"cp_x:%.4f,cp_y:%.4f",(it)->begin()->x,(it)->begin()->y);
					//m_pInterface->Ctrl_Addlog(CamID,L"arrayRow.end()_BEGIN" + str,COLOR_RED,200);
					 LogProc(L"3_BEGIN");
					it->erase(it->begin());
					LogProc(L"3_BEGIN PASS");
				}
				else
				{
					 LogProc(L"4_BEGIN");
					arrayRow.erase(it+1); 
					LogProc(L"4_BEGIN PASS");
				} 
				bAlign = FALSE;
				break;
			}

			/****/      /****/
			/***/       /*****/
			if (abs((it->end()-1)->x - ((it+1)->end()-1)->x)>pitch_*0.5)
			{
				if ((it->end()-1)->x < ((it+1)->end()-1)->x)
				{
					if (it!= arrayRow.begin())
					{
						CString str;
						str.Format(L"cp_x:%.4f,cp_y:%.4f",((it+1)->end()-1)->x,((it+1)->end()-1)->y);
						//m_pInterface->Ctrl_Addlog(CamID,L"arrayRow.begin()_END"+ str,COLOR_RED,200);
						 LogProc(L"1_END");
						(it+1)->erase(((it+1)->end()-1));
						LogProc(L"1_END PASS");
					}
					else
					{
						LogProc(L"2_END");
						arrayRow.erase(it);
						LogProc(L"2_END PASS");
					}
					
				}
				else if ((it+1)!= arrayRow.end()-1)
				{
					/****/
					/***/
					CString str;
					str.Format(L"cp_x:%.4f,cp_y:%.4f",((it)->end()-1)->x,((it)->end()-1)->y);
					//m_pInterface->Ctrl_Addlog(CamID,L"arrayRow.end()-1_END" + str,COLOR_BLUE,200);
					 LogProc(L"3_END");
					it->erase((it->end()-1));
					LogProc(L"3_END PASS");
				}
				else
				{
					LogProc(L"4_END");
					arrayRow.erase(it+1); 
					LogProc(L"4_END PASS");
				}

				bAlign = FALSE;
				break;
			}
		}
	}
	LogProc(L"AlignArrayRow_END");
}
BOOL DualAA_TestItem::findShapeModel(int CamID,vector< vector< CString>> shapeModelPath,vector<vector< vector< fCPoint>>>* shapeCoordinate)
{
	LogProc(L"findShapeModel");
	shapeCoordinate->clear();
	ImageInfo* pImg = pImgInfo[CamID];  

	if(pImg->RGB24_buffer==NULL) 
	{
		return FALSE;
	}

	pBmpRed = new unsigned char[pImg->imgwidth * pImg->imgheight];
	pBmpGreen =new unsigned char[pImg->imgwidth * pImg->imgheight];
	pBmpBlue =new unsigned char[pImg->imgwidth * pImg->imgheight];

	for (int i=0; i<pImg->imgheight; i++)
	{
		for (int j=0; j<pImg->imgwidth; j++)
		{
			pBmpBlue[i*pImg->imgwidth+j] = pImg->RGB24_buffer[(i*pImg->imgwidth+j)*3];
			pBmpGreen[i*pImg->imgwidth+j] = pImg->RGB24_buffer[(i*pImg->imgwidth+j)*3+1];
			pBmpRed[i*pImg->imgwidth+j] = pImg->RGB24_buffer[(i*pImg->imgwidth+j)*3+2];
		}
	}

	// Local iconic variables 
	Hobject  Image, GrayImage, ImageMean;
	Hobject  ImageSub, ImageEmphasize,Region; 
	  
	// Local control variables 
	HTuple  Width, Height;
	HTuple  Row, Column,ModelID,Angle,Score;
	HTuple length;
	
	gen_image3_extern(&Image, "byte", pImg->imgwidth, pImg->imgheight, (long)pBmpRed, (long)pBmpGreen, (long)pBmpBlue,NULL);
	//read_image(&Image,"E:/CCMSDK_1224/MyProject/DualAA/AA_NewChart/ImgSave/20160725-111120Cam0.jpg");
	get_image_size(Image, &Width, &Height);

	rgb1_to_gray(Image, &GrayImage);

#if 1
	Hobject  ImageMeanA = Hobject(), ImageMeanB = Hobject(), ImageEquHisto = Hobject();
	mean_image(GrayImage, &ImageMeanA, 3, 3);
	
	mean_image(GrayImage, &ImageMeanB, 31, 31);
	sub_image(ImageMeanA, ImageMeanB, &ImageSub, 1, 128);
	equ_histo_image(ImageSub, &ImageEmphasize);
	if(blogProc)
	{
		write_image(ImageSub,"jpg",0,"D:\\ImageSub.jpg");
		write_image(ImageMeanB,"jpg",0,"D:\\ImageMeanB.jpg");
		write_image(ImageMeanA,"jpg",0,"D:\\ImageMeanA.jpg");
		write_image(ImageEmphasize,"jpg",0,"D:\\ImageEmphasize.jpg");
	}
#else
	mean_image(GrayImage, &ImageMean, Width/10, Height/10);
	sub_image(GrayImage, ImageMean, &ImageSub, 1, 128);
	write_image(ImageSub,"jpg",0,"D:\\DualShare_r3\\Execute\\Debug\\ImgSave\\ImageSub-old.jpg");
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 1);

	write_image(ImageEmphasize,"jpg",0,"D:\\DualShare_r3\\Execute\\Debug\\ImgSave\\ImageEmphasize-old.jpg");
	//emphasize(ImageSub, &ImageEmphasize, 201, 201, 1);
#endif

	//threshold(ImageEmphasize, &Region, 0, 80);
	 
	for(UINT j=0;j<shapeModelPath.size();j++)
	{
		vector<vector<fCPoint>> tempImgPoint;
		for(UINT i=0;i<shapeModelPath[j].size();i++)
		{
			char path[1024] = {0}; 
			ImageProc::ConvertWideToChar(shapeModelPath[j][i],path);
			read_shape_model(path,&ModelID);
			//find_shape_model(ImageEmphasize, ModelID, -0.39, 0.78, 0.5, 0, 0.5, "least_squares", 0, 0.9, &Row, &Column, &Angle, &Score);
			find_shape_model(ImageEmphasize, ModelID, -0.39, 0.78, 0.3, 0, 0.8, "least_squares", 0, 1, &Row, &Column, &Angle, &Score);
			 
			tuple_length(Row,&length);

			vector<fCPoint> shapeCoordinate_temp;

			for (HTuple len = 0;len<length/*-HTuple(1)*/;len+=HTuple(1))
			{
				fCPoint fcp;
				fcp.x = Column[len].D();
				fcp.y = Row[len].D();
				fcp.PointSize = 1;
				shapeCoordinate_temp.push_back(fcp);			
			}
			tempImgPoint.push_back(shapeCoordinate_temp); 
		}
		shapeCoordinate->push_back(tempImgPoint);

	}
	
	//显示当前找到点的坐标多个 shape model
	//showCrossCoordinate(CamID,*shapeCoordinate);

	SAFE_DELETE_ARRAY(pBmpRed);
	SAFE_DELETE_ARRAY(pBmpGreen);
	SAFE_DELETE_ARRAY(pBmpBlue);
	 
	Image.~Hobject();
	GrayImage.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	 
	Width.~HTuple();
	Height.~HTuple();
	Row.~HTuple();
	Column.~HTuple();
	ModelID.~HTuple();
	Angle.~HTuple();
	Score.~HTuple();
	length.~HTuple();


	
	return TRUE;


}

void DualAA_TestItem::findShapeModel(CString imgPath,CString shapeModelPath,vector<fCPoint>* shapeCoordinate)
{ 
	
	LogProc(L"findShapeModel");

  	// Local iconic variables 
	Hobject  Image, GrayImage, ImageMean;
	Hobject  ImageSub, ImageEmphasize,Region; 

	// Local control variables 
	HTuple  Width, Height;
	HTuple  Row, Column,ModelID,Angle,Score;
	HTuple length;
	 
	char imgpath_[1024] = {0};
	ImageProc::ConvertWideToChar(imgPath,imgpath_);
	 
	read_image(&Image,imgpath_);
	get_image_size(Image, &Width, &Height);

	rgb1_to_gray(Image, &GrayImage);

	mean_image(GrayImage, &ImageMean, Width/10, Height/10);
	sub_image(GrayImage, ImageMean, &ImageSub, 1, 128);
	emphasize(ImageSub, &ImageEmphasize, Width, Height, 1);

	//threshold(ImageEmphasize, &Region, 0, 80);

	char path[1024] = {0};
	ImageProc::ConvertWideToChar(shapeModelPath,path);
	read_shape_model(path,&ModelID);
	//find_shape_model(ImageEmphasize, ModelID, -0.39, 0.78, 0.5, 0, 0.5, "least_squares", 0, 0.9, &Row, &Column, &Angle, &Score);
	find_shape_model(ImageEmphasize, ModelID, -0.39, 0.78, 0.5, 0, 0.5, "least_squares", 0, 1, &Row, &Column, &Angle, &Score);

	tuple_length(Row,&length);
	for (HTuple len = 0;len<length/*-HTuple(1)*/;len+=HTuple(1))
	{
		fCPoint fcp;
		fcp.x = Column[len].D();
		fcp.y = Row[len].D();
		fcp.PointSize = 1;
		shapeCoordinate->push_back(fcp);
	}
	//显示当前找到点的坐标  单个shape model
	//showCrossCoordinate(CamID,*shapeCoordinate);
	Image.~Hobject();
	GrayImage.~Hobject();
	ImageMean.~Hobject();
	ImageSub.~Hobject();
	ImageEmphasize.~Hobject();
	Region.~Hobject();
	 
}

void DualAA_TestItem::showCrossCoordinate(int CamID,vector<fCPoint> corrdinate)
{
	LogProc(L"showCrossCoordinate");
	if (!blogProc)
	{
		return;
	}
	CRect showRect;
	ImageInfo img = *pImgInfo[CamID];

	if (img.RGB24_buffer==NULL)
	{
		return;
	}

	for (UINT i=0;i<corrdinate.size();i++)
	{
		int size = 30;
		showRect.left = (long)(corrdinate[i].x - size);
		showRect.right = (long)(corrdinate[i].x + size);
		showRect.top = (long)(corrdinate[i].y - size);
		showRect.bottom = (long)(corrdinate[i].y + size);
		//m_pInterface->Ctrl_DrawingToMenDC(CamID,COLOR_BLUE,showRect,img);
		 
		if (corrdinate[i].PointSize==0)
		{
			m_pInterface->Ctrl_DrawingToMenDC(CamID,COLOR_RED,CPoint(showRect.left,corrdinate[i].y),CPoint(showRect.right,corrdinate[i].y),img);
			m_pInterface->Ctrl_DrawingToMenDC(CamID,COLOR_RED,CPoint(corrdinate[i].x,showRect.bottom),CPoint(corrdinate[i].x,showRect.top),img);
		}
		else
		{
			m_pInterface->Ctrl_DrawingToMenDC(CamID,COLOR_YELLOW,CPoint(showRect.left,corrdinate[i].y),CPoint(showRect.right,corrdinate[i].y),img);
			m_pInterface->Ctrl_DrawingToMenDC(CamID,COLOR_YELLOW,CPoint(corrdinate[i].x,showRect.bottom),CPoint(corrdinate[i].x,showRect.top),img);
		}
	
	  // Sleep(200);
	  //m_pInterface->Ctrl_ShowMemDC(CamID);

	}
	m_pInterface->Ctrl_ShowMemDC(CamID);

}

void DualAA_TestItem::corrdinateOffset(float ratio,float height,float &img_xy,float distance,float efl,int length)
{
	float offset;//mm
	float x;//入射角
	float p;//折射角
	float tan_x = abs(img_xy-length)*1.12/efl/1000;

	x = atan(tan_x);
	  
	 p = x/ratio;

	 offset = (tan(x) - tan(p))*height;
	 float pixelOffset = (offset*efl/distance)*1000/1.12;
	 if (img_xy<length)
	 {
		 img_xy += pixelOffset;
	 }
	 else
	 {
		  img_xy -= pixelOffset;
	 }
	
}

void DualAA_TestItem::LogProc(CString functionName)
{
	if(blogProc)
		m_pInterface->Ctrl_Addlog(CamID,functionName,COLOR_BLUE,200);
}