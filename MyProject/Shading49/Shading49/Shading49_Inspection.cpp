#include "StdAfx.h"
#include "Shading49_Inspection.h"
#include "ImageProc.h"

#include "math.h"

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new Shading49_Inspection(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
Shading49_Inspection::Shading49_Inspection(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;

	SetName(lpszName);
	
	
	CString temp;
	for (int i = 0; i < ROI_NUM; i++)
	{		
		sShading49.ROI[i].Enable =  1;
	}
	              
	sShading49.ROI[0].cx  =  2;
	sShading49.ROI[0].cy  = 2 ;
	sShading49.ROI[1].cx  = 18;
	sShading49.ROI[1].cy  = 2 ;
	sShading49.ROI[2].cx  =  34;
	sShading49.ROI[2].cy  = 2 ;
	sShading49.ROI[3].cx  =  50;
	sShading49.ROI[3].cy  = 2 ;
	sShading49.ROI[4].cx  = 66;
	sShading49.ROI[4].cy  =  2 ;
	sShading49.ROI[5].cx  =  82;
	sShading49.ROI[5].cy  =  2;
	sShading49.ROI[6].cx  =  98;
	sShading49.ROI[6].cy  = 2 ;
	sShading49.ROI[7].cx  =  2 ;
	sShading49.ROI[7].cy  = 18;
	sShading49.ROI[8].cx  = 18;
	sShading49.ROI[8].cy  = 18;
	sShading49.ROI[9].cx  =  34;
	sShading49.ROI[9].cy  = 18;
	sShading49.ROI[10].cx = 50;
	sShading49.ROI[10].cy = 18;
	sShading49.ROI[11].cx = 66;
	sShading49.ROI[11].cy = 18;
	sShading49.ROI[12].cx = 82;
	sShading49.ROI[12].cy = 18;
	sShading49.ROI[13].cx = 98;
	sShading49.ROI[13].cy = 18;
	sShading49.ROI[14].cx = 2 ;
	sShading49.ROI[14].cy = 34;
	sShading49.ROI[15].cx =18;
	sShading49.ROI[15].cy = 34;
	sShading49.ROI[16].cx = 34;
	sShading49.ROI[16].cy = 34;
	sShading49.ROI[17].cx = 50;
	sShading49.ROI[17].cy = 34;
	sShading49.ROI[18].cx = 66;
	sShading49.ROI[18].cy = 34;
	sShading49.ROI[19].cx = 82;
	sShading49.ROI[19].cy = 34;      
	sShading49.ROI[20].cx = 98;
	sShading49.ROI[20].cy = 34;
	sShading49.ROI[21].cx = 2 ;
	sShading49.ROI[21].cy = 50;
	sShading49.ROI[22].cx = 18;
	sShading49.ROI[22].cy = 50;
	sShading49.ROI[23].cx = 34;
	sShading49.ROI[23].cy =50;
	sShading49.ROI[24].cx = 50;
	sShading49.ROI[24].cy = 50;
	sShading49.ROI[25].cx = 66;
	sShading49.ROI[25].cy = 50;
	sShading49.ROI[26].cx = 82;
	sShading49.ROI[26].cy = 50;
	sShading49.ROI[27].cx = 98;
	sShading49.ROI[27].cy = 50;
	sShading49.ROI[28].cx = 2 ;
	sShading49.ROI[28].cy = 66;
	sShading49.ROI[29].cx = 18;
	sShading49.ROI[29].cy = 66; 
	sShading49.ROI[30].cx = 34;
	sShading49.ROI[30].cy = 66;
	sShading49.ROI[31].cx = 50;
	sShading49.ROI[31].cy = 66;
	sShading49.ROI[32].cx = 66;
	sShading49.ROI[32].cy = 66;
	sShading49.ROI[33].cx = 82;
	sShading49.ROI[33].cy = 66;
	sShading49.ROI[34].cx = 98;
	sShading49.ROI[34].cy = 66;
	sShading49.ROI[35].cx = 2 ;
	sShading49.ROI[35].cy = 82;
	sShading49.ROI[36].cx = 18;
	sShading49.ROI[36].cy = 82;
	sShading49.ROI[37].cx = 34;
	sShading49.ROI[37].cy = 82;
	sShading49.ROI[38].cx = 50;
	sShading49.ROI[38].cy = 82;
	sShading49.ROI[39].cx = 66;
	sShading49.ROI[39].cy = 82; 
	sShading49.ROI[40].cx = 82;
	sShading49.ROI[40].cy = 82;
	sShading49.ROI[41].cx = 98;
	sShading49.ROI[41].cy = 82;
	sShading49.ROI[42].cx = 2 ;
	sShading49.ROI[42].cy = 98;
	sShading49.ROI[43].cx = 18;
	sShading49.ROI[43].cy = 98;
	sShading49.ROI[44].cx = 34;
	sShading49.ROI[44].cy = 98;
	sShading49.ROI[45].cx = 50;
	sShading49.ROI[45].cy = 98;
	sShading49.ROI[46].cx = 66;
	sShading49.ROI[46].cy = 98;
	sShading49.ROI[47].cx = 82;
	sShading49.ROI[47].cy = 98;
	sShading49.ROI[48].cx = 98;
	sShading49.ROI[48].cy = 98;


	sShading49.ROI_height     = 3;
	sShading49.ROI_width      = 3;

	//TOSHADING
/*对客户有标准，但内部无法达到定为A+标准。对客户无标准，定为A 标准——2012.05.08
			(A+) Y	Y_dif	R/G	 B/G		(A) Y	Y_dif	R/G	 B/G		
3M以下			70	 30		15	 15				60	 35		20	 20
3M&3M以上YUV	    75	 20		12	 12				70	 30		20	 20
3M以上Raw+LSC	70	 20		无 	 无				60	 30		无	 无
3M以上Raw		50	 20		无	 无				40	 30		无	 无
OV8825			60	 40(BBK 30)		50	 50	(OTP烧录-MTK14:70,10,15,15)		Qisda(OV8825)50,20,100,100
*/
	sShading49.deltaY_limit     = 70;
	sShading49.Ydif_limit       = 10;
	sShading49.RGdif_limit      = 12;
	sShading49.BGdif_limit      = 12;
	sShading49.PPdif_limit      = 12;

	m_Continue         = 1;	
	m_ExposureEn       =  0;
	m_exp_target       =  128;
	m_exp_tolerance    = 2;
	
	//设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                     //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化
	dif_Max_rb = 0;

	//.....
}

Shading49_Inspection::~Shading49_Inspection(void)
{
	
}

int Shading49_Inspection::InitItem()
{
	CamID = GetCamID();
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption(); 
	return 1;
}

/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int Shading49_Inspection::Initialize()
{
	TestItemInterface::Initialize();
	//TODO:在此添加初始化代码
	SetbStopTest(FALSE);
	if(!Lock_OneTime)
	{
		Lock_OneTime = TRUE;
	}
	CamCode = m_pInterface->Ctrl_GetCamCode();
	if (m_ExposureEn)
	{	
		int ret = 1;//m_pInterface->Ctrl_Exposure(CamID,m_exp_target, m_exp_tolerance);
		if(ret == 0)	
			return 0;
		else
			return -1;
	}
	AvgNum = 0;
	img  =* m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
		return -1;
	}
	Y_Buffer = new BYTE[img.imgwidth*img.imgheight];
	if (NULL == Y_Buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Y_Buffer 申请失败!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
		return -1;
	}
	vec_Ydif_max.clear();
	vec_RGdif_max.clear();
	vec_BGdif_max.clear();
	vec_PPdif_max.clear();
	vec_RG_MaxValue.clear();
	vec_BG_MaxValue.clear();
	dif_Max_rb = 0;
	bRG_ok2 = FALSE;

	return 0;
}


/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int Shading49_Inspection::Testing()
{
   //TODO:在此添加测试项代码
	if ( ShadingTest() == 0 ) 
		return  0;
	else
		return -1;

   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int Shading49_Inspection::Finalize()
{
	//TODO:在此添加回收结束代码
	Lock_OneTime = FALSE;
	SAFE_DELETE(Y_Buffer);
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int Shading49_Inspection::LoadOption()
{
	//TODO: 在此添加读取代码 
	CString temp;
	for (int i = 0; i < ROI_NUM; i++)
	{
		temp.Format(_T("ROI%d_En"), i);
		sShading49.ROI[i].Enable = ReadConfigInt(temp, 1);
	}
        
	sShading49.ROI[0].cx  = ReadConfigInt(_T("ROI0_x"), 2 );
	sShading49.ROI[0].cy  = ReadConfigInt(_T("ROI0_y"), 2 );
	sShading49.ROI[1].cx  = ReadConfigInt(_T("ROI1_x"), 18);
	sShading49.ROI[1].cy  = ReadConfigInt(_T("ROI1_y"), 2 );
	sShading49.ROI[2].cx  = ReadConfigInt(_T("ROI2_x"), 34);
	sShading49.ROI[2].cy  = ReadConfigInt(_T("ROI2_y"), 2 );
	sShading49.ROI[3].cx  = ReadConfigInt(_T("ROI3_x"), 50);
	sShading49.ROI[3].cy  = ReadConfigInt(_T("ROI3_y"), 2 );
	sShading49.ROI[4].cx  = ReadConfigInt(_T("ROI4_x"), 66);
	sShading49.ROI[4].cy  = ReadConfigInt(_T("ROI4_y"), 2 );
	sShading49.ROI[5].cx  = ReadConfigInt(_T("ROI5_x"), 82);
	sShading49.ROI[5].cy  = ReadConfigInt(_T("ROI5_y"), 2 );
	sShading49.ROI[6].cx  = ReadConfigInt(_T("ROI6_x"), 98);
	sShading49.ROI[6].cy  = ReadConfigInt(_T("ROI6_y"), 2 );
	sShading49.ROI[7].cx  = ReadConfigInt(_T("ROI7_x"), 2 );
	sShading49.ROI[7].cy  = ReadConfigInt(_T("ROI7_y"), 18);
	sShading49.ROI[8].cx  = ReadConfigInt(_T("ROI8_x"), 18);
	sShading49.ROI[8].cy  = ReadConfigInt(_T("ROI8_y"), 18);
	sShading49.ROI[9].cx  = ReadConfigInt(_T("ROI9_x"), 34);
	sShading49.ROI[9].cy  = ReadConfigInt(_T("ROI9_y"), 18);
	sShading49.ROI[10].cx = ReadConfigInt(_T("ROI10_x"),50);
	sShading49.ROI[10].cy = ReadConfigInt(_T("ROI10_y"),18);
	sShading49.ROI[11].cx = ReadConfigInt(_T("ROI11_x"),66);
	sShading49.ROI[11].cy = ReadConfigInt(_T("ROI11_y"),18);
	sShading49.ROI[12].cx = ReadConfigInt(_T("ROI12_x"),82);
	sShading49.ROI[12].cy = ReadConfigInt(_T("ROI12_y"),18);
	sShading49.ROI[13].cx = ReadConfigInt(_T("ROI13_x"),98);
	sShading49.ROI[13].cy = ReadConfigInt(_T("ROI13_y"),18);
	sShading49.ROI[14].cx = ReadConfigInt(_T("ROI14_x"),2 );
	sShading49.ROI[14].cy = ReadConfigInt(_T("ROI14_y"),34);
	sShading49.ROI[15].cx = ReadConfigInt(_T("ROI15_x"),18);
	sShading49.ROI[15].cy = ReadConfigInt(_T("ROI15_y"),34);
	sShading49.ROI[16].cx = ReadConfigInt(_T("ROI16_x"),34);
	sShading49.ROI[16].cy = ReadConfigInt(_T("ROI16_y"),34);
	sShading49.ROI[17].cx = ReadConfigInt(_T("ROI17_x"),50);
	sShading49.ROI[17].cy = ReadConfigInt(_T("ROI17_y"),34);
	sShading49.ROI[18].cx = ReadConfigInt(_T("ROI18_x"),66);
	sShading49.ROI[18].cy = ReadConfigInt(_T("ROI18_y"),34);
	sShading49.ROI[19].cx = ReadConfigInt(_T("ROI19_x"),82);
	sShading49.ROI[19].cy = ReadConfigInt(_T("ROI19_y"),34);      
	sShading49.ROI[20].cx = ReadConfigInt(_T("ROI20_x"),98);
	sShading49.ROI[20].cy = ReadConfigInt(_T("ROI20_y"),34);
	sShading49.ROI[21].cx = ReadConfigInt(_T("ROI21_x"),2 );
	sShading49.ROI[21].cy = ReadConfigInt(_T("ROI21_y"),50);
	sShading49.ROI[22].cx = ReadConfigInt(_T("ROI22_x"),18);
	sShading49.ROI[22].cy = ReadConfigInt(_T("ROI22_y"),50);
	sShading49.ROI[23].cx = ReadConfigInt(_T("ROI23_x"),34);
	sShading49.ROI[23].cy = ReadConfigInt(_T("ROI23_y"),50);
	sShading49.ROI[24].cx = ReadConfigInt(_T("ROI24_x"),50);
	sShading49.ROI[24].cy = ReadConfigInt(_T("ROI24_y"),50);
	sShading49.ROI[25].cx = ReadConfigInt(_T("ROI25_x"),66);
	sShading49.ROI[25].cy = ReadConfigInt(_T("ROI25_y"),50);
	sShading49.ROI[26].cx = ReadConfigInt(_T("ROI26_x"),82);
	sShading49.ROI[26].cy = ReadConfigInt(_T("ROI26_y"),50);
	sShading49.ROI[27].cx = ReadConfigInt(_T("ROI27_x"),98);
	sShading49.ROI[27].cy = ReadConfigInt(_T("ROI27_y"),50);
	sShading49.ROI[28].cx = ReadConfigInt(_T("ROI28_x"),2 );
	sShading49.ROI[28].cy = ReadConfigInt(_T("ROI28_y"),66);
	sShading49.ROI[29].cx = ReadConfigInt(_T("ROI29_x"),18);
	sShading49.ROI[29].cy = ReadConfigInt(_T("ROI29_y"),66); 
	sShading49.ROI[30].cx = ReadConfigInt(_T("ROI30_x"),34);
	sShading49.ROI[30].cy = ReadConfigInt(_T("ROI30_y"),66);
	sShading49.ROI[31].cx = ReadConfigInt(_T("ROI31_x"),50);
	sShading49.ROI[31].cy = ReadConfigInt(_T("ROI31_y"),66);
	sShading49.ROI[32].cx = ReadConfigInt(_T("ROI32_x"),66);
	sShading49.ROI[32].cy = ReadConfigInt(_T("ROI32_y"),66);
	sShading49.ROI[33].cx = ReadConfigInt(_T("ROI33_x"),82);
	sShading49.ROI[33].cy = ReadConfigInt(_T("ROI33_y"),66);
	sShading49.ROI[34].cx = ReadConfigInt(_T("ROI34_x"),98);
	sShading49.ROI[34].cy = ReadConfigInt(_T("ROI34_y"),66);
	sShading49.ROI[35].cx = ReadConfigInt(_T("ROI35_x"),2 );
	sShading49.ROI[35].cy = ReadConfigInt(_T("ROI35_y"),82);
	sShading49.ROI[36].cx = ReadConfigInt(_T("ROI36_x"),18);
	sShading49.ROI[36].cy = ReadConfigInt(_T("ROI36_y"),82);
	sShading49.ROI[37].cx = ReadConfigInt(_T("ROI37_x"),34);
	sShading49.ROI[37].cy = ReadConfigInt(_T("ROI37_y"),82);
	sShading49.ROI[38].cx = ReadConfigInt(_T("ROI38_x"),50);
	sShading49.ROI[38].cy = ReadConfigInt(_T("ROI38_y"),82);
	sShading49.ROI[39].cx = ReadConfigInt(_T("ROI39_x"),66);
	sShading49.ROI[39].cy = ReadConfigInt(_T("ROI39_y"),82); 
	sShading49.ROI[40].cx = ReadConfigInt(_T("ROI40_x"),82);
	sShading49.ROI[40].cy = ReadConfigInt(_T("ROI40_y"),82);
	sShading49.ROI[41].cx = ReadConfigInt(_T("ROI41_x"),98);
	sShading49.ROI[41].cy = ReadConfigInt(_T("ROI41_y"),82);
	sShading49.ROI[42].cx = ReadConfigInt(_T("ROI42_x"),2 );
	sShading49.ROI[42].cy = ReadConfigInt(_T("ROI42_y"),98);
	sShading49.ROI[43].cx = ReadConfigInt(_T("ROI43_x"),18);
	sShading49.ROI[43].cy = ReadConfigInt(_T("ROI43_y"),98);
	sShading49.ROI[44].cx = ReadConfigInt(_T("ROI44_x"),34);
	sShading49.ROI[44].cy = ReadConfigInt(_T("ROI44_y"),98);
	sShading49.ROI[45].cx = ReadConfigInt(_T("ROI45_x"),50);
	sShading49.ROI[45].cy = ReadConfigInt(_T("ROI45_y"),98);
	sShading49.ROI[46].cx = ReadConfigInt(_T("ROI46_x"),66);
	sShading49.ROI[46].cy = ReadConfigInt(_T("ROI46_y"),98);
	sShading49.ROI[47].cx = ReadConfigInt(_T("ROI47_x"),82);
	sShading49.ROI[47].cy = ReadConfigInt(_T("ROI47_y"),98);
	sShading49.ROI[48].cx = ReadConfigInt(_T("ROI48_x"),98);
	sShading49.ROI[48].cy = ReadConfigInt(_T("ROI48_y"),98);


	sShading49.ROI_height     = ReadConfigInt(_T("ROI_height"),3);
	sShading49.ROI_width      = ReadConfigInt(_T("ROI_width"),3);
	pDlg->AvgTimes      = ReadConfigInt(_T("AvgTimes"),5);

	//TOSHADING
/*对客户有标准，但内部无法达到定为A+标准。对客户无标准，定为A 标准——2012.05.08
			(A+) Y	Y_dif	R/G	 B/G		(A) Y	Y_dif	R/G	 B/G		
3M以下			70	 30		15	 15				60	 35		20	 20
3M&3M以上YUV	    75	 20		12	 12				70	 30		20	 20
3M以上Raw+LSC	70	 20		无 	 无				60	 30		无	 无
3M以上Raw		50	 20		无	 无				40	 30		无	 无
OV8825			60	 40(BBK 30)		50	 50	(OTP烧录-MTK14:70,10,15,15)		Qisda(OV8825)50,20,100,100
*/
	sShading49.deltaY_limit     = ReadConfigInt(_T("deltaY_limit"),70);
	sShading49.Ydif_limit       = ReadConfigInt(_T("Ydif_limit"),10);
	sShading49.RGdif_limit      = ReadConfigInt(_T("RGdif_limit"),12);
	sShading49.BGdif_limit      = ReadConfigInt(_T("BGdif_limit"),12);
	sShading49.PPdif_limit      = ReadConfigInt(_T("PPdif_limit"),12);
	/*sShading49.RG_Max  = ReadConfigInt(_T("RG_Max"),2);	
	sShading49.BG_Max = ReadConfigInt(_T("BG_Max"),2);*/
	sShading49.bCheckRG_max      = ReadConfigInt(_T("bCheckRG_max"),12);
	sShading49.bCheckBG_max      = ReadConfigInt(_T("bCheckBG_max"),12);
	CString strb;
	strb = ReadConfigString(_T("RG_Max"),_T("0"));
	sShading49.RG_Max = _ttof(strb);
	strb = ReadConfigString(_T("RG_Max2"),_T("0"));
	sShading49.RG_Max2 = _ttof(strb);
	
	strb = ReadConfigString(_T("BG_Max"),_T("0"));	
	sShading49.BG_Max = _ttof(strb);

	m_Continue         = ReadConfigInt(_T("Continue"),1);	
	m_ExposureEn       = ReadConfigInt(_T("ExposureEn"), 0);
	m_exp_target       = ReadConfigInt(_T("Exp_Target"), 128);
	m_exp_tolerance    = ReadConfigInt(_T("Exp_Tolerance"),2);
	 
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int Shading49_Inspection::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   

	for(int i = 0; i < ROI_NUM; i++)
	{
		CString KeyName;
		KeyName.Format(_T("ROI%d_x"),i);
		WriteConfigInt(KeyName,sShading49.ROI[i].cx);

		KeyName.Format(_T("ROI%d_y"),i);
		WriteConfigInt(KeyName,sShading49.ROI[i].cy);

		KeyName.Format(_T("ROI%d_Enable"),i);
		WriteConfigInt(KeyName,sShading49.ROI[i].Enable);
	}

	WriteConfigInt(_T("ExposureEn      "),m_ExposureEn);
	WriteConfigInt(_T("Exp_Target      "),m_exp_target);
	WriteConfigInt(_T("Exp_Tolerance   "),m_exp_tolerance);

	WriteConfigInt(_T("deltaY_limit    "),(int)sShading49.deltaY_limit);
	WriteConfigInt(_T("Ydif_limit      "),(int)sShading49.Ydif_limit);
	WriteConfigInt(_T("RGdif_limit     "),(int)sShading49.RGdif_limit);
	WriteConfigInt(_T("BGdif_limit     "),(int)sShading49.BGdif_limit);
	WriteConfigInt(_T("PPdif_limit     "),(int)sShading49.PPdif_limit);

	WriteConfigInt(_T("ROI_height      "),sShading49.ROI_height);
	WriteConfigInt(_T("ROI_width       "),sShading49.ROI_width);


	WriteConfigInt(_T("Continue"), m_Continue);

	
	WriteConfigInt(_T("bCheckRG_max"),sShading49.bCheckRG_max );	
	WriteConfigInt(_T("bCheckBG_max"),sShading49.bCheckBG_max);

	CString strb;

	/*WriteConfigInt(_T("RG_Max"),sShading49.RG_Max);	
	WriteConfigInt(_T("BG_Max"),sShading49.BG_Max);*/

	strb.Format(_T("%0.1f"),sShading49.RG_Max);
	WriteConfigString(_T("RG_Max"),strb);

	strb.Format(_T("%0.1f"),sShading49.RG_Max2);
	WriteConfigString(_T("RG_Max2"),strb);
	 
	strb.Format(_T("%0.1f"),sShading49.BG_Max);
	WriteConfigString(_T("BG_Max"),strb);

	WriteConfigInt(_T("AvgTimes"),pDlg->AvgTimes);
	 	
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR Shading49_Inspection::GetReportHeads()
{
	static CString header;
	header = _T("");

	CString temp;

	header += _T("模组编号,");

	header += _T("Ydif_max,");
	header += _T("RGdif_max,");
	header += _T("BGdif_max,");
	header += _T("PPdif_max,");
	header += _T("RG_MaxValue,");
	header += _T("BG_MaxValue,");
	
	

	header += _T("Ydiff,");
	for(int i = 0; i < ROI_NUM; i++)
	{
		temp.Format(_T("B%d-DeltaY,B%d-R/G,B%d-B/G,"),i,i,i);
		header += temp;
	}

	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR Shading49_Inspection::GetReportLowLimit()
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
LPCTSTR Shading49_Inspection::GetReportHightLimit()
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
LPCTSTR Shading49_Inspection::GetReportContents()
{
	static CString Content;
	Content = _T("");

	CString temp;
	/*temp.Format(_T("%.0lf,"),sShading49.Ydif_max);
	Content += temp;*/

	Content += CamCode;

	temp.Format(_T("%.1f,"),sShading49.Ydif_max);
	Content += temp;
	temp.Format(_T("%.1f,"),sShading49.RGdif_max);
	Content += temp;
	temp.Format(_T("%.1f,"),sShading49.BGdif_max);
	Content += temp;
	temp.Format(_T("%.1f,"),sShading49.PPdif_max);
	Content += temp;
	temp.Format(_T("%.1f,"),sShading49.RG_MaxValue);
	Content += temp;
	temp.Format(_T("%.1f,"),sShading49.BG_MaxValue);
	Content += temp;
 

	for(int i = 0; i < ROI_NUM; i++)
	{
		if (sShading49.ROI[i].Enable)
		{
			temp.Format(_T("%.0lf,"),sShading49.ROI[i].deltaY);
			Content += temp;
			temp.Format(_T("%.0lf,"),sShading49.ROI[i].RGdif);
			Content += temp;
			temp.Format(_T("%.0lf,"),sShading49.ROI[i].BGdif);
			Content += temp;
		}
	}
	return Content;
}

void Shading49_Inspection::CalcImageRect(void)
{
	 img  =* m_pInterface->Ctrl_GetImgInfo(CamID);
	int width  = img.imgwidth;
	int height = img.imgheight;
	ImageProc::RGB24toY(img.RGB24_buffer,Y_Buffer,width,height);
	
	
	for(int i = 0; i < ROI_NUM; i++)
	{
		if(sShading49.ROI[i].Enable)
		{
			//比例转化为实际像素点
			sShading49.ROI[i].pos.top    = (int)((sShading49.ROI[i].cy - sShading49.ROI_height/2.0)  * height/100);
			sShading49.ROI[i].pos.bottom = (int)((sShading49.ROI[i].cy + sShading49.ROI_height/2.0)  * height/100);
			sShading49.ROI[i].pos.left   = (int)((sShading49.ROI[i].cx - sShading49.ROI_width/2.0)   * width/100) ;
			sShading49.ROI[i].pos.right  = (int)((sShading49.ROI[i].cx + sShading49.ROI_width/2.0)   * width/100) ;

			if(sShading49.ROI[i].pos.left < 0)
				sShading49.ROI[i].pos.left = 0;
			if(sShading49.ROI[i].pos.right >= width)
				sShading49.ROI[i].pos.right = width-1;
			if(sShading49.ROI[i].pos.top < 0)
				sShading49.ROI[i].pos.top = 0;
			if(sShading49.ROI[i].pos.bottom >= height)
				sShading49.ROI[i].pos.bottom = height-1;
		}
	}
	
}

int Shading49_Inspection::ShadingTest(void)
{
	CalcImageRect();

	ImageInfo img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧2失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("抓帧2失败!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	int width =  img.imgwidth;
	int height = img.imgheight;

	BYTE* RGB24_Buffer = img.RGB24_buffer;
 
	sShading49.RG_MaxValue = 0;
	sShading49.BG_MaxValue = 0;
	//计算结果
	for(int i = 0; i < ROI_NUM; i++)
	{
		sShading49.ROI[i].aveY = ImageProc::GetAveY_ROI_RGB24(RGB24_Buffer, width, height, sShading49.ROI[i].pos);
		sShading49.ROI[i].aveR = ImageProc::GetAveR_ROI_RGB24(RGB24_Buffer, width, height, sShading49.ROI[i].pos);
		sShading49.ROI[i].aveG = ImageProc::GetAveG_ROI_RGB24(RGB24_Buffer, width, height, sShading49.ROI[i].pos);
		sShading49.ROI[i].aveB = ImageProc::GetAveB_ROI_RGB24(RGB24_Buffer, width, height, sShading49.ROI[i].pos);

		sShading49.RG_MaxValue = max(sShading49.RG_MaxValue, (sShading49.ROI[i].aveR -sShading49.ROI[i].aveG ) );
		sShading49.BG_MaxValue = max(sShading49.BG_MaxValue, (sShading49.ROI[i].aveB -sShading49.ROI[i].aveG ) );
		dif_Max_rb = max(dif_Max_rb,sShading49.ROI[i].aveR -sShading49.ROI[i].aveB);
	}


	if (sShading49.bCheckRG_max||sShading49.bCheckBG_max)
	{
		int roiNum_w = 33;
		int roiNum_h = 33;
		for (int i=0;i<roiNum_h;i++)
		{
			for (int j=0;j<roiNum_w;j++)
			{
				CRect sShading990_ROI;
				//比例转化为实际像素点
				sShading990_ROI.top    = (int)((sShading49.ROI_height*(j) - sShading49.ROI_height/2.0)  * height/100);
				sShading990_ROI.bottom = (int)((sShading49.ROI_height*(j) + sShading49.ROI_height/2.0)  * height/100);
				sShading990_ROI.left   = (int)((sShading49.ROI_width*(i) - sShading49.ROI_width/2.0)   * width/100) ;
				sShading990_ROI.right  = (int)((sShading49.ROI_width*(i) + sShading49.ROI_width/2.0)   * width/100) ;

				if(sShading990_ROI.left < 0)
					sShading990_ROI.left = 0;
				if(sShading990_ROI.right >= width)
					sShading990_ROI.right = width-1;
				if(sShading990_ROI.top < 0)
					sShading990_ROI.top = 0;
				if(sShading990_ROI.bottom >= height)
					sShading990_ROI.bottom = height-1;

				float aveY = ImageProc::GetAveY_ROI_RGB24(RGB24_Buffer, width, height, sShading990_ROI);
				float aveR = ImageProc::GetAveR_ROI_RGB24(RGB24_Buffer, width, height, sShading990_ROI);
				float aveG = ImageProc::GetAveG_ROI_RGB24(RGB24_Buffer, width, height, sShading990_ROI);
				float aveB = ImageProc::GetAveB_ROI_RGB24(RGB24_Buffer, width, height, sShading990_ROI);

				sShading49.RG_MaxValue = max(sShading49.RG_MaxValue, (aveR -aveG ));
				sShading49.BG_MaxValue = max(sShading49.BG_MaxValue, (aveB -aveG ));
			} 
		}
	}
	 
	double Ymax = 0;
	double Ymin = 255;

	double RGdif = 0;
	double BGdif = 0;

	

	double PPmin = 255;
	double PPmax = 0;
	//和中心支的差异
	for(int i = 0; i < ROI_NUM; i++)
	{
		//记录差异
		sShading49.ROI[i].deltaY = 1.0 * sShading49.ROI[i].aveY / sShading49.ROI[24].aveY * 100.0;
		sShading49.ROI[i].deltaR = 1.0 * sShading49.ROI[i].aveR / sShading49.ROI[24].aveR * 100.0;
		sShading49.ROI[i].deltaG = 1.0 * sShading49.ROI[i].aveG / sShading49.ROI[24].aveG * 100.0;
		sShading49.ROI[i].deltaB = 1.0 * sShading49.ROI[i].aveB / sShading49.ROI[24].aveB * 100.0;

		sShading49.ROI[i].RGdif = sShading49.ROI[i].deltaR - sShading49.ROI[i].deltaG;
		sShading49.ROI[i].BGdif = sShading49.ROI[i].deltaB - sShading49.ROI[i].deltaG;

		

		//找到最大的差异
		RGdif = max(RGdif, fabs(sShading49.ROI[i].RGdif) );
		BGdif = max(BGdif, fabs(sShading49.ROI[i].BGdif) );

		//对比中心和四角 0， 6， 42， 48
// 		Ymax = max(sShading49.ROI[i].aveY,Ymax);
// 		Ymin = min(sShading49.ROI[i].aveY,Ymin);

		PPmax = max(PPmax, sShading49.ROI[i].RGdif);
		PPmax = max(PPmax, sShading49.ROI[i].BGdif);

		PPmin = min(PPmin, sShading49.ROI[i].RGdif);
		PPmin = min(PPmin, sShading49.ROI[i].BGdif);

		
	}
	Ymax = max(sShading49.ROI[0].aveY, sShading49.ROI[6].aveY);
	Ymax = max(Ymax, sShading49.ROI[42].aveY);
	Ymax = max(Ymax, sShading49.ROI[48].aveY);

	Ymin = min(sShading49.ROI[0].aveY, sShading49.ROI[6].aveY);
	Ymin = min(Ymin, sShading49.ROI[42].aveY);
	Ymin = min(Ymin, sShading49.ROI[48].aveY);

	sShading49.Ydif_max = (Ymax - Ymin) / sShading49.ROI[24].aveY * 100;
	//得到最终的值
//	sShading49.Ydif_max = Ymax - Ymin;
	sShading49.RGdif_max = RGdif;
	sShading49.BGdif_max = BGdif;
	sShading49.PPdif_max = PPmax - PPmin;

	AvgNum++;
	vec_Ydif_max.push_back(sShading49.Ydif_max);
	vec_RGdif_max.push_back(sShading49.RGdif_max);
	vec_BGdif_max.push_back(sShading49.BGdif_max);
	vec_PPdif_max.push_back(sShading49.PPdif_max);
	vec_RG_MaxValue.push_back(sShading49.RG_MaxValue);
	vec_BG_MaxValue.push_back(sShading49.BG_MaxValue);
	
	if (AvgNum<pDlg->AvgTimes)
	{		
		return 1;
	}
	else
	{
		int  Ni= vec_Ydif_max.size();
		sShading49.Ydif_max=0;
		sShading49.RGdif_max=0;
		sShading49.BGdif_max=0;
		sShading49.PPdif_max=0;
		sShading49.RG_MaxValue=0;
		sShading49.BG_MaxValue=0;
		for (int i=0;i<Ni;i++)
		{
			sShading49.Ydif_max+=vec_Ydif_max[i];
			sShading49.RGdif_max+=vec_RGdif_max[i];
			sShading49.BGdif_max+=vec_BGdif_max[i];
			sShading49.PPdif_max+=vec_PPdif_max[i];
			sShading49.RG_MaxValue+=vec_RG_MaxValue[i];
			sShading49.BG_MaxValue+=vec_BG_MaxValue[i];
		}

		sShading49.Ydif_max=sShading49.Ydif_max/Ni;
		sShading49.RGdif_max=sShading49.RGdif_max/Ni;
		sShading49.BGdif_max=sShading49.BGdif_max/Ni;
		sShading49.PPdif_max=sShading49.PPdif_max/Ni;
		sShading49.RG_MaxValue=sShading49.RG_MaxValue/Ni;
		sShading49.BG_MaxValue=sShading49.BG_MaxValue/Ni;

	}


	//记录结果

	for(int i = 0; i < ROI_NUM; i++)
	{	
		//判断deltaY是否超标
		if(sShading49.ROI[i].deltaY < sShading49.deltaY_limit)
		{
			sShading49.ROI[i].deltaY_result = RESULT_FAIL;
			CString strs;
			strs.Format(_T("%.02f"),sShading49.ROI[i].deltaY);
			m_pInterface->Ctrl_Addlog(CamID,strs,COLOR_RED,200);
			//break;
		}
			
		else
			sShading49.ROI[i].deltaY_result = RESULT_PASS;
		//判断RG是否超标
		if (abs(sShading49.ROI[i].RGdif) > sShading49.RGdif_limit )
			sShading49.ROI[i].RG_result = RESULT_FAIL;
		else
			sShading49.ROI[i].RG_result = RESULT_PASS;
		//判断BG是否超标
		if (abs(sShading49.ROI[i].BGdif) > sShading49.BGdif_limit) 
			sShading49.ROI[i].BG_result = RESULT_FAIL;
		else
			sShading49.ROI[i].BG_result = RESULT_PASS;

		if (sShading49.Ydif_max > sShading49.Ydif_limit)
			sShading49.Ydif_result = RESULT_FAIL;
		else
			sShading49.Ydif_result = RESULT_PASS;

		if (sShading49.PPdif_max > sShading49.PPdif_limit)
			sShading49.PP_result = RESULT_FAIL;
		else
			sShading49.PP_result = RESULT_PASS;

		sShading49.RGmax_result = RESULT_PASS;
		if (sShading49.bCheckRG_max)
		{
			if (sShading49.RG_MaxValue <= sShading49.RG_Max)
			{
				sShading49.RGmax_result = RESULT_PASS;
				bRG_ok2 = FALSE;
			}
			else if(sShading49.RG_MaxValue <= sShading49.RG_Max2)
			{
				sShading49.RGmax_result = RESULT_PASS;
				bRG_ok2 = TRUE;
			}
		   else
		   {
			   sShading49.RGmax_result = RESULT_FAIL;
			   bRG_ok2 = FALSE;
		   }
		 
		}
		sShading49.BGmax_result = RESULT_PASS;
		if (sShading49.bCheckBG_max)
		{
			if (sShading49.BG_MaxValue > sShading49.BG_Max)
				sShading49.BGmax_result = RESULT_FAIL;
			else
				sShading49.BGmax_result = RESULT_PASS;
		}
		
		

		/*sShading49.RG_Max = max(m_RGdifMax, (sShading49.ROI[i].RGdif) );
		sShading49.BG_Max = max(m_BGdifMax, (sShading49.ROI[i].BGdif) );*/

		//如果有一个超标，此ROI总体结果NG；
		if (sShading49.ROI[i].BG_result == RESULT_FAIL||
			sShading49.ROI[i].RG_result == RESULT_FAIL||
			sShading49.ROI[i].deltaY_result == RESULT_FAIL||
			sShading49.PP_result == RESULT_FAIL||
			sShading49.RGmax_result == RESULT_FAIL||
			sShading49.BGmax_result == RESULT_FAIL||
			sShading49.Ydif_result == RESULT_FAIL)
		{
			sShading49.ROI[i].result = RESULT_FAIL;
		}
		else
		{
			sShading49.ROI[i].result = RESULT_PASS;
		}
	}

	//如果有一个ROI结果NG,整个测试项目结果NG
	int FailCount=0;
	for(int i = 0; i < ROI_NUM; i++)
	{
		if (sShading49.ROI[i].result == RESULT_FAIL )
		{
			FailCount++;
		}
	}

	if(FailCount > 0)
		SetResult(RESULT_FAIL);
	else
		SetResult(RESULT_PASS);

	ShowShadingTest();

	if (m_Continue)
		return 2;
	else	
		return 0;
}

void Shading49_Inspection::ShowShadingTest()
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

	COLORREF CurrentColor;

	for(int i = 0; i < ROI_NUM; i++)
	{
		if(sShading49.ROI[i].Enable)
		{
			if (sShading49.ROI[i].result == RESULT_FAIL)
				CurrentColor = COLOR_RED;
			else
				CurrentColor = COLOR_GREEN;

			CPen pen(PS_SOLID,1,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);

			CRect rect;
			rect.left   =  (int)((sShading49.ROI[i].cx - sShading49.ROI_width /2.0) * DisplayWndRect.Width() /100.0);
			rect.right  =  (int)((sShading49.ROI[i].cx + sShading49.ROI_width /2.0) * DisplayWndRect.Width() /100.0);
			rect.top    =  (int)((sShading49.ROI[i].cy - sShading49.ROI_height/2.0) * DisplayWndRect.Height()/100.0);
			rect.bottom =  (int)((sShading49.ROI[i].cy + sShading49.ROI_height/2.0) * DisplayWndRect.Height()/100.0);

			pDisplayWndMemDC->Rectangle(rect);

			//画字符
			int x = sShading49.ROI[i].cx * DisplayWndRect.Width()/100;
			int y = sShading49.ROI[i].cy * DisplayWndRect.Height()/100;

			CString str;
			str.Format(_T("B%d"),i);
			// 字符最后一排
			if (i > 41)
				pDisplayWndMemDC->TextOut(x-12,y-30,str);
			else
				pDisplayWndMemDC->TextOut(x-12,y+13,str);

		}
	}


#if 0 
	//初始化画背景
	CDC* pInfoMemDC = m_pInterface->GetAssistantWndMemDC();
	CRect rect;
	m_pInterface->GetAssistantWndRect(&rect);
	pInfoMemDC->FillRect(&rect,&CBrush(RGB(160,160,160)));
	pInfoMemDC->SetBkMode(TRANSPARENT);
	pInfoMemDC->SelectObject(&ft);

	int x = 10;
	int y = -2;

	CString header = _T("NO.    Y     △R    △G    △B    △Y   R/G   B/G ");
	pInfoMemDC->SetTextColor(RGB(0,0,0));
	pInfoMemDC->TextOut(x, y, header);


	CPen pen(PS_SOLID,1,RGB(0,0,100));
	pInfoMemDC->SelectObject(&pen);
	pInfoMemDC->MoveTo(0,15);
	pInfoMemDC->LineTo(300,15);

	CString szSpace1 = _T("  ");
	CString szSpace2 = _T("    ");

	for(int i = 0; i < ROI_NUM; i+=2)
	{
		if(sShading49.ROI[i].Enable)
		{
			x = 10;
			y = 18 + (i/2)* 12 ;
			CString data;
			if (i < 10)
			{
				data.Format(_T("B%d      %.1f %s %.0lf%s  %.0lf%s  %.0lf"),
					i,
					sShading49.ROI[i].aveY,
					sShading49.ROI[i].aveY >= 100 ? szSpace1:szSpace2,
					sShading49.ROI[i].deltaR,
					sShading49.ROI[i].deltaR >= 100 ? szSpace1:szSpace2,
					sShading49.ROI[i].deltaG,
					sShading49.ROI[i].deltaG >= 100 ? szSpace1:szSpace2,
					sShading49.ROI[i].deltaB);
			}
			else
			{
				data.Format(_T("B%d    %.1f%s %.0lf%s  %.0lf%s  %.0lf"),
					i,
					sShading49.ROI[i].aveY,
					sShading49.ROI[i].aveY >= 100 ? szSpace1:szSpace2,
					sShading49.ROI[i].deltaR,
					sShading49.ROI[i].deltaR >= 100 ? szSpace1:szSpace2,
					sShading49.ROI[i].deltaG,
					sShading49.ROI[i].deltaG >= 100 ? szSpace1:szSpace2,
					sShading49.ROI[i].deltaB);
			}
			

			pInfoMemDC->SetTextColor(RGB(0,0,0));
			pInfoMemDC->TextOut(x, y, data);

			//根据deltaY值显示值和颜色
			CString deltaY;
			deltaY.Format(_T("%.0lf"),sShading49.ROI[i].deltaY);
			if (sShading49.ROI[i].deltaY_result == RESULT_FAIL)
				CurrentColor = COLOR_RED;
			else
				CurrentColor = COLOR_GREEN;

			pInfoMemDC->SetTextColor(CurrentColor);
			pInfoMemDC->TextOut(x + 190, y, deltaY);

			//根据RGdif值显示值和颜色
			CString RG;
			RG.Format(_T("%2.0lf"),sShading49.ROI[i].RGdif);
			if (sShading49.ROI[i].RG_result == RESULT_FAIL)
				CurrentColor = COLOR_RED;
			else
				CurrentColor = COLOR_GREEN;

			pInfoMemDC->SetTextColor(CurrentColor);
			pInfoMemDC->TextOut(x+230, y, RG);

			//根据BGdif值显示值和颜色
			CString BG;
			BG.Format(_T("%2.0lf"),sShading49.ROI[i].BGdif);
			if (sShading49.ROI[i].BG_result == RESULT_FAIL)
				CurrentColor = COLOR_RED;
			else
				CurrentColor = COLOR_GREEN;

			pInfoMemDC->SetTextColor(CurrentColor);
			pInfoMemDC->TextOut(x+270, y, BG);
		}
	}

	//画分割线	
	pInfoMemDC->SetTextColor(COLOR_BLACK);
	pInfoMemDC->MoveTo(0,325);
	pInfoMemDC->LineTo(300,325);


	CString str;

	//显示标题
	str = _T("Max Ydif");
	pInfoMemDC->TextOut(320, 70, str); 
	//根据MAX Ydif 值显示值和颜色
	if (sShading49.Ydif_max > sShading49.Ydif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	pInfoMemDC->SetTextColor(CurrentColor);
	str.Format(_T("%0.lf"),sShading49.Ydif_max);
	pInfoMemDC->TextOut(400, 70, str);

	//显示标题
	str = _T("Max RGdif");
	pInfoMemDC->SetTextColor(COLOR_BLACK);
	pInfoMemDC->TextOut(320, 95, str);
	//根据MAX RGdif 值显示值和颜色
	if (sShading49.RGdif_max > sShading49.RGdif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	pInfoMemDC->SetTextColor(CurrentColor);
	str.Format(_T("%0.lf"),sShading49.RGdif_max);
	pInfoMemDC->TextOut(400, 95, str);


	//显示标题
	str = _T("Max BGdif");	
	pInfoMemDC->SetTextColor(COLOR_BLACK);
	pInfoMemDC->TextOut(320, 120, str);
	//根据MAX BGdif 值显示值和颜色
	if (sShading49.BGdif_max > sShading49.BGdif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	pInfoMemDC->SetTextColor(CurrentColor);
	str.Format(_T("%0.lf"),sShading49.BGdif_max);
	pInfoMemDC->TextOut(400, 120, str);


	//显示标题
	str = _T("MAX PPdif");	
	pInfoMemDC->SetTextColor(COLOR_BLACK);
	pInfoMemDC->TextOut(320, 145, str);
	//根据MAX BGdif 值显示值和颜色
	if (sShading49.PPdif_max > sShading49.PPdif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	pInfoMemDC->SetTextColor(CurrentColor);
	str.Format(_T("%0.lf"),sShading49.PPdif_max);
	pInfoMemDC->TextOut(400, 145, str);

	CString result;
	if (GetResult()== RESULT_FAIL)
	{
		CurrentColor = COLOR_RED;
		result = _T("NG");
	}
	else
	{
		CurrentColor = COLOR_GREEN;
		result = _T("OK");
	}

	

	pInfoMemDC->SetTextColor(CurrentColor);
	pInfoMemDC->SelectObject(&font2);
	pInfoMemDC->TextOut(335, 220, result);

	m_pInterface->ShowMemDC(pInfoMemDC);
#endif
	CString str;
	if (sShading49.Ydif_max > sShading49.Ydif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	 
	str.Format(_T("MaxYdif %0.lf"),sShading49.Ydif_max);
    m_pInterface->Ctrl_Addlog(CamID,str,CurrentColor,200);

 
	//根据MAX RGdif 值显示值和颜色
	if (sShading49.RGdif_max > sShading49.RGdif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
 
	str.Format(_T("RGdif %0.lf"),sShading49.RGdif_max);
	m_pInterface->Ctrl_Addlog(CamID,str,CurrentColor,200);


	 
	if (sShading49.BGdif_max > sShading49.BGdif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	 
	str.Format(_T("BGdif%0.lf"),sShading49.BGdif_max);
	m_pInterface->Ctrl_Addlog(CamID,str,CurrentColor,200);
	
	 
	//根据MAX BGdif 值显示值和颜色
	if (sShading49.PPdif_max > sShading49.PPdif_limit)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;
	 
	str.Format(_T("MAX PPdif %0.lf"),sShading49.PPdif_max);
	m_pInterface->Ctrl_Addlog(CamID,str,CurrentColor,200);

	//根据MAX BGdif 值显示值和颜色
	if (!sShading49.RGmax_result)
		CurrentColor = COLOR_RED;
	else
		CurrentColor = COLOR_GREEN;

	str.Format(_T("MAX RGdif %.1f"),sShading49.RG_MaxValue);
	m_pInterface->Ctrl_Addlog(CamID,str,CurrentColor,200);

	/*str.Format(_T("dif_Max_rb %.1f"),dif_Max_rb);
	m_pInterface->Ctrl_Addlog(CamID,str,CurrentColor,200);*/
	
	//根据MAX BGdif 值显示值和颜色

    if (bRG_ok2)
    {
		CurrentColor = COLOR_YELLOW;
    }
	else
	{
		if (!sShading49.BGmax_result)
			CurrentColor = COLOR_RED;
		else
			CurrentColor = COLOR_GREEN;
	}
	

	str.Format(_T("MAX BGdif %.1f"),sShading49.BG_MaxValue);
	m_pInterface->Ctrl_Addlog(CamID,str,CurrentColor,200);

	CString result;
	if (bRG_ok2)
	{
		CurrentColor = COLOR_YELLOW;
		result = _T("OK2");
		SetResult(RESULT_OK2);
	}
	else
	{
		if (GetResult()== RESULT_FAIL)
		{
			CurrentColor = COLOR_RED;
			result = _T("NG");
		}
		else
		{
			CurrentColor = COLOR_GREEN;
			result = _T("OK1");
		}
	}
	
	CFont font2;
	font2.CreatePointFont(300,_T("Arial"));
	pDisplayWndMemDC->SetTextColor(CurrentColor);
	pDisplayWndMemDC->SelectObject(&font2);
	pDisplayWndMemDC->TextOut(
		DisplayWndRect.Width()/2 - 105, 
		DisplayWndRect.Height()/2 + 40, 
		_T("Shading ") + result);

	//显示DC
//	m_pInterface->Ctrl_ShowMemDC(pDisplayWndMemDC);
	
}