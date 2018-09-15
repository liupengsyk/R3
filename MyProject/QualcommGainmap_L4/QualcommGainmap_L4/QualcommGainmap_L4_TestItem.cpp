#include "StdAfx.h"
#include "QualcommGainmap_L4_TestItem.h"
#include "ImageProc.h"
#pragma  comment(lib, "PDAFCalibrationTools_RevL_Dll.lib")

//#pragma  comment(lib, "T_PDAF/Qualcomm/L4/PDAFCalibrationTools_RevL_Dll.lib")
#define SAFE_FREE(p)	{	if(p) { free(p);					(p)=NULL; }	}


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new QualcommGainmap_L4_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
QualcommGainmap_L4_TestItem::QualcommGainmap_L4_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

QualcommGainmap_L4_TestItem::~QualcommGainmap_L4_TestItem(void)
{

}


int QualcommGainmap_L4_TestItem::InitItem()
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
int QualcommGainmap_L4_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码

	LeftGainMin=10000;
	LeftGainMax=0;
	RightGainMin=10000;
	RightGainMax=0;
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int QualcommGainmap_L4_TestItem::Testing()
{
   //TODO:在此添加测试项代码
   USES_CONVERSION;

   vector <BYTE> Step1Data;
    CString FileName;
    CString filename1=_T("");
    CString filename2=_T("");
    CString filename3=_T("");
    CString filepath=_T("");
	filepath=m_pInterface->Ctrl_GetCurrentPath()+_T("T_PDAF\\Qualcomm\\");
	filename1.Format(_T("Site%d_CamID%d_"),m_pInterface->GetSiteNo(),CamID);
	filename1=filename1+_T("Step1_1.raw");
	FileName=filepath+filename1;
	ImageInfo img= *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("抓帧1失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	int width  = img.imgwidth;
	int height = img.imgheight;
	int PixelOrder = img.RawFormat;

	BOOL Res_Order = TRUE;

	if (PixelOrder == 0 )
	{
		if (pDlg->m_CFA != 1)
		{
			Res_Order = FALSE;
		}
	}
	else if (PixelOrder == 1 || PixelOrder == 2)
	{
		if (pDlg->m_CFA != (PixelOrder+2))
		{
			Res_Order = FALSE;
		}
	}
	else if (PixelOrder == 3)
	{
		if (pDlg->m_CFA != 2)
		{
			Res_Order = FALSE;
		}
	}
	else
	{
		Res_Order = FALSE;
	}

	if (Res_Order == FALSE)
	{
		CString tempStr;
		tempStr.Format(_T("outputFormat = %d,CFA = %d"),PixelOrder,pDlg->m_CFA);
		m_pInterface->Ctrl_Addlog(CamID,tempStr,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("Pixel Order设置错误!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Pixel Order set false!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	m_pInterface->Ctrl_SaveImage(CamID,1,filepath,filename1,3);
	Sleep(200);
	filename2.Format(_T("Site%d_CamID%d_"),m_pInterface->GetSiteNo(),CamID);
	filename2=filename2+_T("Step1_2.raw");
	img=*m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧2失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("抓帧2失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	m_pInterface->Ctrl_SaveImage(CamID,1,filepath,filename2,3);
	Sleep(200);
	filename3.Format(_T("Site%d_CamID%d_"),m_pInterface->GetSiteNo(),CamID);
	filename3=filename3+_T("Step1_3.raw");
	img=*m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧3失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("抓帧3失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	m_pInterface->Ctrl_SaveImage(CamID,1,filepath,filename3,3);	
	m_pInterface->Ctrl_Addlog(CamID,_T("SAVE RAW OK!"),COLOR_BLUE,200);
	//
	int i=0;
	void *p=NULL; 
	char dll_version_info[1024];
	char dll_feature_info[2048];
	CString RawFile      = m_pInterface->Ctrl_GetCurrentPath()+_T("flat_field.raw");
	CString GainmapFile  = m_pInterface->Ctrl_GetCurrentPath()+_T("gainmap.txt");
	dll_config_t     dll_cfg;
	sensor_config_t  sensor_cfg;
	gainmap_t        gain_map;
	gainmap_limits_t gm_limits;

	uint16_t *img_l=NULL, *img_r=NULL;
	uint16_t raw_width, raw_height,pd_width,pd_height;
	int32_t  rc;
		// initialize return code
	rc = 0;
	
	// set test limits
	// calibration DLL will still return gain map even if limits are exceeded
	gm_limits.pd_max_limit   = 950;  // Max PD pixel value after LPF must be below 950
	gm_limits.pd_min_limit   = 100;  // Min PD pixel value after LPF must exceed 300
	gm_limits.gain_max_limit = (uint16_t) (7.999f * (1<<GAIN_MAP_Q_FMT));  // max gain of 6x
	float sensor_gain = 2.0f;
	// load dll config parameters
    get_dll_cfg( &dll_cfg );
	
	// allocate scratch buffer for calibration dll
	int j;
	for ( j=0;j<5;j++)
	{
		p = PDAF_Cal_create( &dll_cfg ); 
		if( p != NULL ) 
		{
			 j=0;
		     break;
		}
	}
	if (j!=0)
	{
		PDAF_Cal_destroy( p );
        m_pInterface->Ctrl_Addlog(CamID,_T("Cannot find PDAF_Cal_create"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Cannot find PDAF_Cal_create"));
		SetResult(RESULT_FAIL);
		return 0;  // dll memory allocation error
	}
	
	// calibration DLL for version number and version description
	PDAF_Cal_get_lib_version(dll_version_info);  printf("%s", dll_version_info);
	PDAF_Cal_get_lib_features(dll_feature_info); printf("%s", dll_feature_info);
	
	// obtain new sensor config file from sensor manufacturer
	// add extern declaration to PDAFCalibrationTools_Dll.h and call sensor cfg here
	sensor_cfg.cfa=(_cfa_pattern_t)pDlg->m_CFA;
	sensor_cfg.image_width=width;
	sensor_cfg.image_height=height;
	sensor_cfg.black_lvl=pDlg->m_BLC;
	sensor_cfg.bit_depth=pDlg->m_BIT;
	sensor_cfg.fv_measure_channel =pDlg->m_channel;
	sensor_cfg.pd_block_width=pDlg->m_block_Width;
	sensor_cfg.pd_block_height=pDlg->m_Block_Height;
	sensor_cfg.pd_block_start_x=pDlg->m_Start_X;
	sensor_cfg.pd_block_start_y=pDlg->m_Start_Y;
	sensor_cfg.pd_block_n_pd_pairs=pDlg->m_PD_pairs;//???

	raw_width  = sensor_cfg.image_width;
	raw_height = sensor_cfg.image_height;
	pd_width = raw_width>>2;
	pd_height = raw_height>>2;

	ZeroMemory(sensor_cfg.pd_block_r_x,MAX_PD_PIXELS_PER_BLOCK*sizeof(uint16_t));
	ZeroMemory(sensor_cfg.pd_block_r_y,MAX_PD_PIXELS_PER_BLOCK*sizeof(uint16_t));
	ZeroMemory(sensor_cfg.pd_block_l_x,MAX_PD_PIXELS_PER_BLOCK*sizeof(uint16_t));
	ZeroMemory(sensor_cfg.pd_block_l_y,MAX_PD_PIXELS_PER_BLOCK*sizeof(uint16_t));

/*	uint16_t pd_block_l_x[] =
	{6,6, 14, 14, 22,22,30,30};
	uint16_t pd_block_l_y[] = 
	{14,30,2,18,14,30,2,18};	
	
	uint16_t pd_block_r_x[] =
	{6,6, 14, 14, 22,22,30,30};
 	uint16_t pd_block_r_y[] =
 	{10,26,6,22,10,26,6,22};

	memcpy(sensor_cfg.pd_block_l_x, pd_block_l_x, sensor_cfg.pd_block_n_pd_pairs*sizeof(uint16_t));
	memcpy(sensor_cfg.pd_block_l_y, pd_block_l_y, sensor_cfg.pd_block_n_pd_pairs*sizeof(uint16_t)); 
	memcpy(sensor_cfg.pd_block_r_x, pd_block_r_x, sensor_cfg.pd_block_n_pd_pairs*sizeof(uint16_t)); 
    memcpy(sensor_cfg.pd_block_r_y, pd_block_r_y, sensor_cfg.pd_block_n_pd_pairs*sizeof(uint16_t)); */
   //获取pd block坐标

	{
		CString tempStr;
		tempStr = pDlg->m_PD_Block_Right_X;
		tempStr.Trim();
		tempStr = tempStr.Right(1);
		if (tempStr != _T(","))
		{
			PDAF_Cal_destroy( p );
			m_pInterface->Ctrl_Addlog(CamID,_T("m_PD_Block_Right_X设置不符合规范!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("m_PD_Block_Right_X设置不符合规范!"));
			SetResult(RESULT_FAIL);
			return 0;
		}

		tempStr = pDlg->m_PD_Block_Right_Y;
		tempStr.Trim();
		tempStr = tempStr.Right(1);
		if (tempStr != _T(","))
		{
			PDAF_Cal_destroy( p );
			m_pInterface->Ctrl_Addlog(CamID,_T("m_PD_Block_Right_Y设置不符合规范!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("m_PD_Block_Right_Y设置不符合规范!"));
			SetResult(RESULT_FAIL);
			return 0;
		}

		tempStr = pDlg->m_PD_Block_Left_X;
		tempStr.Trim();
		tempStr = tempStr.Right(1);
		if (tempStr != _T(","))
		{
			PDAF_Cal_destroy( p );
			m_pInterface->Ctrl_Addlog(CamID,_T("m_PD_Block_Left_X设置不符合规范!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("m_PD_Block_Left_X设置不符合规范!"));
			SetResult(RESULT_FAIL);
			return 0;
		}

		tempStr = pDlg->m_PD_Block_Left_Y;
		tempStr.Trim();
		tempStr = tempStr.Right(1);
		if (tempStr != _T(","))
		{
			PDAF_Cal_destroy( p );
			m_pInterface->Ctrl_Addlog(CamID,_T("m_PD_Block_Left_Y设置不符合规范!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("m_PD_Block_Left_Y设置不符合规范!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}

   int start =0;
   int end = 0;

   if (!pDlg->m_is2PD)
   {
	   end =(pDlg->m_PD_Block_Right_X).Find((_T(",")),start);
	   i=0;
	   while(end!=-1)
	   {
		   CString tmp = pDlg->m_PD_Block_Right_X.Mid(start,end-start);
		   int a = atoi(T2A(tmp));
		   sensor_cfg.pd_block_r_x[i++]= a;
		   start = end + 1;
		   end = (pDlg->m_PD_Block_Right_X).Find((_T(",")),start);
	   }

	   if (i != pDlg->m_PD_pairs)
	   {
		   PDAF_Cal_destroy( p );
		   m_pInterface->Ctrl_Addlog(CamID,_T("sensor_cfg.pd_block_r_x读取异常!"),COLOR_RED,200);
		   m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("sensor_cfg.pd_block_r_x读取异常!"));
		   SetResult(RESULT_FAIL);
		   return 0;
	   }

	   start =0;
	   end = 0;
	   end =((pDlg->m_PD_Block_Right_Y)).Find((_T(",")),start);
	   i=0;
	   while(end!=-1)
	   {
		   CString tmp = pDlg->m_PD_Block_Right_Y.Mid(start,end-start);
		   int a = atoi(T2A(tmp));
		   sensor_cfg.pd_block_r_y[i++]= a;
		   start = end + 1;
		   end = (pDlg->m_PD_Block_Right_Y).Find((_T(",")),start);
	   }
	   if (i != pDlg->m_PD_pairs)
	   {
		   PDAF_Cal_destroy( p );
		   m_pInterface->Ctrl_Addlog(CamID,_T("sensor_cfg.pd_block_r_y读取异常!"),COLOR_RED,200);
		   m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("sensor_cfg.pd_block_r_y读取异常!"));
		   SetResult(RESULT_FAIL);
		   return 0;
	   }

	   start =0;
	   end = 0;
	   end =((pDlg->m_PD_Block_Left_X)).Find((_T(",")),start);
	   i=0;
	   while(end!=-1)
	   {
		   CString tmp = pDlg->m_PD_Block_Left_X.Mid(start,end-start);
		   int a = atoi(T2A(tmp));
		   sensor_cfg.pd_block_l_x[i++]= a;
		   start = end + 1;
		   end = (pDlg->m_PD_Block_Left_X).Find((_T(",")),start);
	   }
	   if (i != pDlg->m_PD_pairs)
	   {
		   PDAF_Cal_destroy( p );
		   m_pInterface->Ctrl_Addlog(CamID,_T("sensor_cfg.pd_block_l_x读取异常!"),COLOR_RED,200);
		   m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("sensor_cfg.pd_block_l_x读取异常!"));
		   SetResult(RESULT_FAIL);
		   return 0;
	   }

	   start =0;
	   end = 0;
	   end =((pDlg->m_PD_Block_Left_Y)).Find((_T(",")),start);
	   i=0;
	   while(end!=-1)
	   {
		   CString tmp = pDlg->m_PD_Block_Left_Y.Mid(start,end-start);
		   int a = atoi(T2A(tmp));
		   sensor_cfg.pd_block_l_y[i++]= a;
		   start = end + 1;
		   end = (pDlg->m_PD_Block_Left_Y).Find((_T(",")),start);
	   }
	   if (i != pDlg->m_PD_pairs)
	   {
		   PDAF_Cal_destroy( p );
		   m_pInterface->Ctrl_Addlog(CamID,_T("sensor_cfg.pd_block_l_y读取异常!"),COLOR_RED,200);
		   m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("sensor_cfg.pd_block_l_y读取异常!"));
		   SetResult(RESULT_FAIL);
		   return 0;
	   }
   }
   
	////////////////读取第1张图片///////////////////////////////////////
	FILE *fp=NULL;
	filename1=filepath+filename1;
	fp = fopen(T2A(filename1),"rb");	
	if (fp == NULL)
	{
		PDAF_Cal_destroy( p );
		m_pInterface->Ctrl_Addlog(CamID,_T("Cannot find Step1_1.raw!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Cannot find Step1_1.raw!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	unsigned short *Frombuffer;	
	Frombuffer = (unsigned short *)calloc(raw_height*raw_width,sizeof(unsigned short));
	if (NULL == Frombuffer)
	{
		PDAF_Cal_destroy( p );
		m_pInterface->Ctrl_Addlog(CamID,_T("Frombuffer1 buffer申请失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Frombuffer1 buffer申请失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	fread((void *)Frombuffer,2,width*height,fp);
	fclose(fp);
	////////////////读取第2张图片///////////////////////////////////////
	filename2=filepath+filename2;
	fp = fopen(T2A(filename2),"rb");	
	if (fp == NULL)
	{
		PDAF_Cal_destroy( p );
		SAFE_FREE(Frombuffer);

	    m_pInterface->Ctrl_Addlog(CamID,_T("Cannot find Step1_2.raw!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Cannot find Step1_2.raw!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	unsigned short *Frombuffer1;	
	Frombuffer1 = (unsigned short *)calloc(raw_height*raw_width,sizeof(unsigned short));
	if (NULL == Frombuffer1)
	{
		SAFE_FREE(Frombuffer);

		PDAF_Cal_destroy( p );
		m_pInterface->Ctrl_Addlog(CamID,_T("Frombuffer2 buffer申请失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Frombuffer2 buffer申请失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	fread((void *)Frombuffer1,2,width*height,fp);
	fclose(fp);

	for (int i=0; i<raw_height*raw_width; i++)
	{
		Frombuffer[i] += Frombuffer1[i]; 
	}
	////////////////读取第3张图片///////////////////////////////////////
	filename3=filepath+filename3;
	fp = fopen(T2A(filename3),"rb");	
	if (fp == NULL)
	{
		PDAF_Cal_destroy( p );
		SAFE_FREE(Frombuffer1);
		SAFE_FREE(Frombuffer);
		m_pInterface->Ctrl_Addlog(CamID,_T("Cannot find Step1_3.raw!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Cannot find Step1_3.raw!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	memset(Frombuffer1, 0, width*height*sizeof(unsigned short));
	fread((void *)Frombuffer1,2,width*height,fp);
	fclose(fp);

	for (int i=0; i<raw_height*raw_width; i++)
	{
		Frombuffer[i] += Frombuffer1[i]; 
		Frombuffer[i] = (Frombuffer[i])/3.0 + 0.5;
	}

// 	unsigned short *Frombuffer;	
// 	Frombuffer = (unsigned short *)calloc(raw_height*raw_width,sizeof(unsigned short));
// 	if (NULL == Frombuffer)
// 	{
// 		PDAF_Cal_destroy( p );
// 		SAFE_FREE(Frombuffer1);
// 		SAFE_FREE(Frombuffer2);
// 		SAFE_FREE(Frombuffer3);
// 		m_pInterface->Ctrl_Addlog(CamID,_T("Frombuffer buffer申请失败!"),COLOR_RED,200);
// 		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Frombuffer buffer申请失败!"));
// 		SetResult(RESULT_FAIL);
// 		return 0;
// 	}


	SAFE_FREE(Frombuffer1);
//	SAFE_FREE(Frombuffer2);
//	SAFE_FREE(Frombuffer3);

#if 0

	BYTE *GmCalibImage;
	GmCalibImage = (BYTE *)calloc(raw_height*raw_width*2,sizeof(BYTE)); 
	if (NULL == GmCalibImage)
	{
		PDAF_Cal_destroy( p );
		SAFE_FREE(Frombuffer);

		m_pInterface->Ctrl_Addlog(CamID,_T("GmCalibImage buffer申请失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("GmCalibImage buffer申请失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	int n=0;
	for (int nummm=0;nummm<raw_height*raw_width;nummm++)
	{		
		GmCalibImage[n++]=Frombuffer[nummm]&0xFF;
		GmCalibImage[n++]=Frombuffer[nummm]>>8;
	}

#endif


	filename1.Format(_T("Site%d_CamID%d_"),m_pInterface->GetSiteNo(),CamID);
	filename1 = filename1 + _T("flat_field.raw");
	CString path_flatfield_file = filepath + filename1; 

	CFile file;
	BOOL ret = file.Open(path_flatfield_file, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if (!ret)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("flat_field.raw保存失败!"),COLOR_RED,200);
	} 
	else
	{
		file.Write((void*) Frombuffer, width * height * sizeof(USHORT) );
	}
	file.Close();

//	SAFE_FREE(GmCalibImage);
	 
	if (pDlg->m_is2PD)
	{
		img_l = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
		img_r = (uint16_t *)malloc(raw_width * raw_height * sizeof(uint16_t));
		if (NULL == img_l || NULL == img_r)
		{
			SAFE_FREE(img_l);
			SAFE_FREE(img_r);
			SAFE_FREE(Frombuffer);

			m_pInterface->Ctrl_Addlog(CamID,_T("左右图片内存分配失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("左右图片内存分配失败!"));
			SetResult(RESULT_FAIL);
			return FALSE;
		}

		if (Read_raw_file_2pd(path_flatfield_file, img_l, img_r, raw_width, raw_height, sensor_cfg.cfa) ) 
		{
			SAFE_FREE(img_l);
			SAFE_FREE(img_r);
			SAFE_FREE(Frombuffer);

			m_pInterface->Ctrl_Addlog(CamID,_T("计算GainMap图片读取失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("计算GainMap图片读取失败!"));
			SetResult(RESULT_FAIL);
			return FALSE;
		}

		rc = PDAF_Cal_get_gainmap_2pd(p,img_l,img_r,pd_width,pd_height,sensor_cfg.black_lvl,&gain_map,&gm_limits,sensor_gain);
		SAFE_FREE(img_l);
		SAFE_FREE(img_r);
		SAFE_FREE(Frombuffer);
		PDAF_Cal_destroy(p);
	}
	else
	{
		rc = PDAF_Cal_get_gainmap(p, Frombuffer, &sensor_cfg, &gain_map, &gm_limits );
		SAFE_FREE(Frombuffer);
		PDAF_Cal_destroy( p );
	}
	
	if(rc!=0)
	{		
		m_pInterface->Ctrl_Addlog(CamID,_T("Gain Map 校准失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Gain Map 校准失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	for(i = 0; i <gain_map.width*gain_map.height; i ++)  
	{
		if(gain_map.gm_r[i]>=RightGainMax)
		{
			RightGainMax=gain_map.gm_r[i];
		}
		if(gain_map.gm_r[i]<=RightGainMin)
		{
			RightGainMin=gain_map.gm_r[i];
		}
		if(gain_map.gm_l[i]>=LeftGainMax)
		{
			LeftGainMax=gain_map.gm_l[i];
		}
		if(gain_map.gm_l[i]<=LeftGainMin)
		{
			LeftGainMin=gain_map.gm_l[i];
		}
	}

	gain_map.cal_version = dll_cfg.cal_version;

	SaveGainMapData(gain_map);

	if(pDlg->m_savealldata==1)  //SAVE DATA 
	{
		FILE *fp_w=NULL;
		char GainmapFilename[1024];
		CString outputFilename=_T("");
		char GainmapBaseFilename[1024];
		outputFilename=filepath+_T("L4_");

		CString strFuseID;
		strFuseID=m_pInterface->Sensor_GetCamFuseID(CamID);
		outputFilename=outputFilename+strFuseID;

		strcpy(GainmapBaseFilename,T2A(outputFilename));
		sprintf(GainmapFilename,"%sGainmapdata.txt",GainmapBaseFilename);
		fp_w = fopen(GainmapFilename,"wb");
		if (fp_w == NULL)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Cannot open Gainmapdata.txt!"),COLOR_RED,200);
			SetResult(RESULT_FAIL);
			return 0;
		}
		fprintf(fp_w,"%d\n",dll_cfg.cal_version);
		fprintf(fp_w,"%d\n",gain_map.width);
		fprintf(fp_w,"%d\n",gain_map.height);
		for (i = 0; i <gain_map.width*gain_map.height; i ++)
			fprintf(fp_w,"%d\n",gain_map.gm_l[i]);
		for (i = 0; i < gain_map.width*gain_map.height; i ++)
			fprintf(fp_w,"%d\n", gain_map.gm_r[i]);

		fprintf(fp_w,"RightGainMax=%d\n",RightGainMax);
		fprintf(fp_w,"RightGainMin=%d\n",RightGainMin);
		fprintf(fp_w,"LeftGainMax=%d\n",LeftGainMax);
		fprintf(fp_w,"LeftGainMin=%d\n",LeftGainMin);
		fclose(fp_w);
	}

	CString temp2;
	temp2.Format(_T("RightGainMax=%d RightGainMin=%d LeftGainMax=%d LeftGainMin=%d"),RightGainMax,RightGainMin,LeftGainMax,LeftGainMin);
	m_pInterface->Ctrl_Addlog(CamID,temp2,COLOR_BLUE,200);
	for(int i = 0; i <gain_map.width*gain_map.height; i ++)
	{
		if(gain_map.gm_r[i]>pDlg->m_GainmapMAX||gain_map.gm_r[i]<pDlg->m_GainmapMIN||gain_map.gm_l[i]>pDlg->m_GainmapMAX||gain_map.gm_l[i]<pDlg->m_GainmapMIN)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("生成Gainmap超过范围!"),COLOR_RED,200);
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	
	USHORT OTP_GainmapData[1030];
	if(pDlg->m_save==0)
	{
		OTP_GainmapData[0] =dll_cfg.cal_version>> 8 ;
		OTP_GainmapData[1] =dll_cfg.cal_version&0xff;

		OTP_GainmapData[2]=gain_map.width>>8;
		OTP_GainmapData[3]=gain_map.width&0xff ;

		OTP_GainmapData[4]=gain_map.height>>8;
		OTP_GainmapData[5]=gain_map.height&0xff ; 


		for (int i=0;i<gain_map.width*gain_map.height;i++)
		{
			OTP_GainmapData[6+i*2]=gain_map.gm_l[i]>>8;
			OTP_GainmapData[7+i*2]=gain_map.gm_l[i]&0xff;

			OTP_GainmapData[448+i*2]=gain_map.gm_r[i]>>8;
			OTP_GainmapData[449+i*2]=gain_map.gm_r[i]&0xff;

		}
	}
	if(pDlg->m_save==1)
	{  
		OTP_GainmapData[0] = dll_cfg.cal_version&0xff ;
		OTP_GainmapData[1] = dll_cfg.cal_version>> 8;

		OTP_GainmapData[2]= gain_map.width&0xff  ;
		OTP_GainmapData[3]= gain_map.width>>8;

		OTP_GainmapData[4]= gain_map.height&0xff;
		OTP_GainmapData[5]= gain_map.height>>8; 


		for (int i=0;i<gain_map.width*gain_map.height;i++)
		{
			OTP_GainmapData[6+i*2]= gain_map.gm_l[i]&0xff;
			OTP_GainmapData[7+i*2]= gain_map.gm_l[i]>>8;

			OTP_GainmapData[448+i*2]= gain_map.gm_r[i]&0xff;
			OTP_GainmapData[449+i*2]= gain_map.gm_r[i]>>8;

		}
	}
    for (i=890;i<1030;i++)
	{
	    OTP_GainmapData[i]=0;
	}

    Step1Data.clear();
	for(int i=0;i<1030;i++)
	{
	 Step1Data.push_back(OTP_GainmapData[i]);
	}
    int siteNo=m_pInterface->GetSiteNo();
	FileName.Format(_T("Gainmap_%d.bin"), siteNo);
	if(!m_pInterface->Otp_WriteToOtpBoard(CamID,FileName,Step1Data,1)) //和写入长度有关 
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Bin文件生成失败!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
		return 0;
	}

   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int QualcommGainmap_L4_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int QualcommGainmap_L4_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_CFA=ReadConfigInt(_T("CFA"),2);

    pDlg->m_BLC=ReadConfigInt(_T("BLC"),64);
	pDlg->m_BIT=ReadConfigInt(_T("Bit_depth"),10);
	pDlg->m_channel=ReadConfigInt(_T("Measure_Channel"),1);
	pDlg->m_block_Width=ReadConfigInt(_T("block_Width"),64);
	pDlg->m_Block_Height=ReadConfigInt(_T("block_Height"),64);
	pDlg->m_PD_pairs=ReadConfigInt(_T("PD_pairs"),8);
	pDlg->m_Start_X=ReadConfigInt(_T("Start_X"),1);
	pDlg->m_Start_Y=ReadConfigInt(_T("Start_Y"),1);

	
    pDlg->m_savealldata=ReadConfigInt(_T("Savedata"),1);
	pDlg->m_save=ReadConfigInt(_T("Gainmap_Enable"),1);
    pDlg->m_GainmapMAX=ReadConfigInt(_T("GainmapMAX"),1200);
	pDlg->m_GainmapMIN=ReadConfigInt(_T("GainmapMIN"),0);

	pDlg->m_is2PD = ReadConfigInt(_T("Is2PD"),0);

	pDlg->m_PD_Block_Right_X=ReadConfigString(_T("Block_Right_X"),_T("1,1"));
	pDlg->m_PD_Block_Right_Y=ReadConfigString(_T("Block_Right_Y"),_T("1,1"));
	pDlg->m_PD_Block_Left_X=ReadConfigString(_T("Block_Left_X"),_T("1,1"));
	pDlg->m_PD_Block_Left_Y=ReadConfigString(_T("Block_Left_Y"),_T("1,1"));


	


	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int QualcommGainmap_L4_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	  
	WriteConfigInt(_T("CFA"),pDlg->m_CFA);
	WriteConfigInt(_T("BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("Bit_depth"),pDlg->m_BIT);
	WriteConfigInt(_T("Measure_Channel"),pDlg->m_channel);
	WriteConfigInt(_T("block_Width"),pDlg->m_block_Width);
	WriteConfigInt(_T("block_Height"),pDlg->m_Block_Height);
	WriteConfigInt(_T("PD_pairs"),pDlg->m_PD_pairs);
	WriteConfigInt(_T("Start_X"),pDlg->m_Start_X);
	WriteConfigInt(_T("Start_Y"),pDlg->m_Start_Y);
	WriteConfigInt(_T("Is2PD"),pDlg->m_is2PD);
	
    WriteConfigInt(_T("Savedata"),pDlg->m_savealldata);
	WriteConfigInt(_T("Gainmap_Enable"),pDlg->m_save);
    WriteConfigInt(_T("GainmapMAX"),pDlg->m_GainmapMAX);
	WriteConfigInt(_T("GainmapMIN"),pDlg->m_GainmapMIN);

	WriteConfigString(_T("Block_Right_X"),pDlg->m_PD_Block_Right_X);
	WriteConfigString(_T("Block_Right_Y"),pDlg->m_PD_Block_Right_Y);
	WriteConfigString(_T("Block_Left_X"),pDlg->m_PD_Block_Left_X);
	WriteConfigString(_T("Block_Left_Y"),pDlg->m_PD_Block_Left_Y);
	
 
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR QualcommGainmap_L4_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header+=_T("RightGainMax,RightGainMin,LeftGainMax,LeftGainMin,");
	
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR QualcommGainmap_L4_TestItem::GetReportLowLimit()
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
LPCTSTR QualcommGainmap_L4_TestItem::GetReportHightLimit()
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
LPCTSTR QualcommGainmap_L4_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	CString temp;
	temp.Format(_T("%d,%d,%d,%d,"),RightGainMax,RightGainMin,LeftGainMax,LeftGainMin);
	Content+=temp;
	return Content;
}



int QualcommGainmap_L4_TestItem::Read_raw_file_2pd(CString filename, uint16_t * img_l, uint16_t * img_r, uint16_t width, uint16_t height, cfa_t cfa)
{
	int i, j, k, l;
	FILE *fp=NULL;
	uint16_t *img=NULL;
	uint16_t pix;
	CString str;

	USES_CONVERSION;

	fp = fopen(CT2A(filename.GetBuffer(filename.GetLength())), "rb");
	if (fp == NULL) {
		str.Format(_T("Cannot open file for read: %s\n"), filename);
		m_pInterface->Ctrl_Addlog(CamID,str,COLOR_RED,200);
		return -1;
	}

	// allocate temp buffer
	img = (uint16_t *) malloc( width * height * sizeof(uint16_t));
	if( img == NULL ) {
		m_pInterface->Ctrl_Addlog(CamID,_T("img buffer申请失败!"),COLOR_RED,200);
		return -1;
	}

	// read file content
	for (i = 0; i < height; i++)	{
		for (j = 0; j < width; j++) {
			fread(&pix, sizeof(uint16_t), 1, fp);
			img[i*width + j] = uint16_t(pix);
		}
	}
	fclose(fp);
	fp = NULL;


	// parse left and right pd images with 2x2 green binning to mimic tail mode buffer
	// if tail mode buffer is directly from sensor (e.g. IMX362 mode 3/4) bypass this step
	if (cfa == GRBG || cfa == GBRG ) {
		for (i = 0; i < (height>>2); i++)	{
			for (j = 0; j < (width>>2); j++) {
				k = i<<2;
				l = j<<2;

				img_l[ i*(width>>2) + j] = 
					( img[ (k+0)*width + (l+0)] + 
					img[ (k+1)*width + (l+2)] + 
					img[ (k+2)*width + (l+0)] + 
					img[ (k+3)*width + (l+2)] )>>2;

				img_r[i*(width>>2) + j] =
					( img[ (k+0)*width + (l+1)] + 
					img[ (k+1)*width + (l+3)] + 
					img[ (k+2)*width + (l+1)] + 
					img[ (k+3)*width + (l+3)] )>>2;      
			}
		}
	}else {
		for (i = 0; i < (height>>2); i++)	{
			for (j = 0; j < (width>>2); j++) {
				k = i<<2;
				l = j<<2;

				img_l[ i*(width>>2) + j] = 
					( img[ (k+0)*width + (l+2)] + 
					img[ (k+1)*width + (l+0)] + 
					img[ (k+2)*width + (l+2)] + 
					img[ (k+3)*width + (l+0)] )>>2;

				img_r[i*(width>>2) + j] =
					( img[ (k+0)*width + (l+3)] + 
					img[ (k+1)*width + (l+1)] + 
					img[ (k+2)*width + (l+3)] + 
					img[ (k+3)*width + (l+1)] )>>2;      
			}
		}

	}
	SAFE_FREE(img);

	return 0;
}


void QualcommGainmap_L4_TestItem::SaveGainMapData(gainmap_t gainmap)
{
	USES_CONVERSION;

	int i = 0;
	FILE  *fp  = NULL;
	CTime time = CTime::GetCurrentTime();

	CString RecordTime = _T("");

	CFile MyFile;
	long fileSizeTemp=0;

	CString CurrentName = GetName();

	CString FileName;
	SetName(_T("GENERAL_SETTING"));
	TCHAR szReportPath[MAX_PATH] = {0};
	CString strReportPath = ReadConfigString(_T("sReportPath"), szReportPath, MAX_PATH);
	FileName.Format(_T("%s\\GainMapData_Test_Site_A%04d%02d%02d.xls"), strReportPath.GetBuffer(), time.GetYear(), time.GetMonth(), time.GetDay()); ;

	m_pInterface->Ctrl_Addlog(CamID,FileName,COLOR_RED,200);
	CString strLogInfo = _T("");
	fp=freopen(CT2A(FileName),"at",stdout);
	if(fp==NULL)  
	{
		strLogInfo.Format(_T("%s文件打开失败，数据保存失败!", FileName.GetBuffer()));
		m_pInterface->Ctrl_Addlog(CamID, strLogInfo,COLOR_RED,200);
		return;
	}

	fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);
	if(fileSizeTemp==0)
	{
		printf("FUSE ID\tTest Time\t");
		printf("LeftRight\tcal_version\tGainMap width\tGainMap height\t");
		for (i = 0; i < 221; i++)
		{
			printf( "%s%d\t", "GainMap_", i+1);
		}	
// 		for (i = 0; i < 221; i++)
// 		{
// 			printf( "%s%d\t", "RightGainMap_", i+1);
// 		}
	}

	printf("\n");
	printf(CT2A(m_pInterface->Sensor_GetCamFuseID(CamID)));
	printf("#\t");

	printf(CT2A(RecordTime));
	printf("\t");

	printf("LeftGainMap\t");

	printf("%d\t%d\t%d\t",gainmap.cal_version,gainmap.width,gainmap.height);

	for (i = 0; i < 221; i++)
	{
		printf( "%d\t",gainmap.gm_l[i]);
	}	

	printf("\n");
	printf("\t\tRightGainMap\t\t\t\t");
	for (i = 0; i < 221; i++)
	{
		printf( "%d\t",gainmap.gm_r[i]);
	}	

	fclose(fp);
	SetName(CurrentName);
}
