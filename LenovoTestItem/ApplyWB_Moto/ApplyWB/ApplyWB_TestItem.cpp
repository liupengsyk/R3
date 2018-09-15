#include "StdAfx.h"
#include "ApplyWB_TestItem.h"
#include "ImageProc.h"

#include <direct.h>
#include <iostream>
#include <string>
#include <fstream>
#include <ostream>
#include <cstdlib>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */

#define LSC_PATH_DEFINE								0

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new ApplyWB_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
ApplyWB_TestItem::ApplyWB_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

ApplyWB_TestItem::~ApplyWB_TestItem(void)
{

}


int ApplyWB_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption();

	CString CurrentName = GetName();
	SetName(_T("WBSetting"));

	m_strTestStation = ReadConfigString(_T("TestStation"), _T("XXXXX"));

	SetName(CurrentName);

	return 0;
}

/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int ApplyWB_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
	ApplyAWBFlag=TRUE;
	CheckCount=0;
	Beforedistance=0.0;
	Afterdistance=0.0;
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int ApplyWB_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	TestResult = TEST_PASS;

	m_AWBSpec.RGIndex  = pDlg->m_RGIndex;
	m_AWBSpec.BGIndex  = pDlg->m_BGIndex;
	m_AWBSpec.GGIndex  = pDlg->m_GGIndex;

	m_AWBSpec.RIndex  = pDlg->m_RIndex;
	m_AWBSpec.GrIndex  = pDlg->m_GrIndex;
	m_AWBSpec.GbIndex  = pDlg->m_GbIndex;
	m_AWBSpec.BIndex  = pDlg->m_BIndex;

	
	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID); 
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧1失败!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("抓帧1失败!"));
		SetResult(RESULT_FAIL);
		TestResult=TEST_FAIL;
		return 0;
	}
	if (ApplyAWBFlag == TRUE)
	{
		if (!CheckGoldenRange())
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB Golden Value not in range!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("AWB CheckGoldenRange Fail"));
			SetResult(RESULT_FAIL);
			TestResult = TEST_FAIL;
			return 0;
		}

		//Save Image
		if (pDlg->m_SaveImage)
		{
			USES_CONVERSION;
			FilePath = m_pInterface->Ctrl_GetCurrentPath()+_T("\\ApplyBefore\\");
			_mkdir(T2A(FilePath));
			FileName=m_pInterface->Ctrl_GetCamCode()+_T("ApplyBefore.raw");
			CString BmpFileName=m_pInterface->Ctrl_GetCamCode()+_T("ApplyBefore.bmp");
			TempMsg.Format(_T("%d_%s"),CamID,FileName);
			m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,TempMsg,3,NULL);   //BYTE *imgBuffer
			TempMsg.Format(_T("%d_%s"),CamID,BmpFileName);
			m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,TempMsg,2,NULL);   //BYTE *imgBuffer
		}

		//Calculate AWB
		if (pDlg->m_QTITool)//使用高通工具
		{
			if(!GetAWBData(img))
			{
			   m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
			   m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("GetAWBData Fail!"));
			   SetResult(RESULT_FAIL);
			   TestResult=TEST_FAIL;
			   return 0;
			}
		}
		else
		{
			if (pDlg->m_Raw8_EN)
			{
				GetAWBInfoRaw8(img);
			}
			else
			{
				GetAWBInfoRaw10(img);
			}
		}
		
		OTP_R = m_AWBInfo.Unit_R_fin;
		OTP_Gr = m_AWBInfo.Unit_Gr_fin;
		OTP_Gb = m_AWBInfo.Unit_Gb_fin;
		OTP_B = m_AWBInfo.Unit_B_fin;

		if (pDlg->m_QTITool)//使用高通工具
		{
			unsigned char Buff[6] = {0};
			CString ToolPath;
			ToolPath.Format(_T("AWBData\\Cam%d\\"),CamID);
			ToolPath=m_pInterface->Ctrl_GetCurrentPath()+ToolPath;
	
			CFile file1;
			BOOL ret = file1.Open(ToolPath+_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt"), CFile::modeRead|CFile::typeBinary);
			if (ret)
			{
				UINT ret = file1.Read((void*)Buff, sizeof(Buff)/sizeof(Buff[0]));
				file1.Close();

				OTP_RGain = MAKEWORD(Buff[0], Buff[1]);
				OTP_BGain = MAKEWORD(Buff[2], Buff[3]);
				OTP_GGain = MAKEWORD(Buff[4], Buff[5]);
			}
		}
		else
		{
			OTP_RGain = m_AWBInfo.RGain_fin;
			OTP_BGain = m_AWBInfo.BGain_fin;
			OTP_GGain = m_AWBInfo.GGain_fin;
		}

		TempMsg.Format(_T("Golden_R=%d\nGolden_Gr=%d\nGolden_Gb=%d\nGolden_B=%d"),m_AWBSpec.Golden_R,m_AWBSpec.Golden_Gr,m_AWBSpec.Golden_Gb,m_AWBSpec.Golden_B);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("Golden_RG=%d\nGolden_BG=%d\nGolden_GG=%d"),m_AWBSpec.Golden_RG,m_AWBSpec.Golden_BG,m_AWBSpec.Golden_GG);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("RawR=%d\nRawGr=%d\nRawGb=%d\nRawB=%d"),m_AWBInfo.Unit_R,m_AWBInfo.Unit_Gr,m_AWBInfo.Unit_Gb,m_AWBInfo.Unit_B);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("BLC = %d"),pDlg->m_BLC);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		TempMsg.Format(_T("[After Dec BLC:%d]:R=%d\nGr=%d\nGb=%d\nB=%d"), pDlg->m_BLC, OTP_R, OTP_Gr, OTP_Gb, OTP_B);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		TempMsg.Format(_T("Rindex=%d\nGrindex=%d\nGbindex=%d\nBindex=%d"),m_AWBSpec.RIndex,m_AWBSpec.GrIndex,m_AWBSpec.GbIndex,m_AWBSpec.BIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("[Gain Multi %d after dec BLC:%d]:RGain=%d\nRGindex=%d\nBGain=%d\nBGindex=%d\nGGain=%d\nGGindex=%d"),
							pDlg->m_Multiple,
							pDlg->m_BLC,
							OTP_RGain,
							m_AWBSpec.RGIndex,
							OTP_BGain,
							m_AWBSpec.BGIndex,
							OTP_GGain,
							m_AWBSpec.GGIndex
							);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		
		if (!CheckAWBRange())
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB Value not in range!"), COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("CheckAWBRange() Fail"));
			SetResult(RESULT_FAIL);
			TestResult = TEST_FAIL;
			return 0;
		}

		float m_OTPDistance = 100;
		if (pDlg->m_GG_GainEn)
		{
			m_OTPDistance=(float)sqrt(pow((m_AWBInfo.RGain_fin-m_AWBSpec.Golden_RG)*1.0/m_AWBSpec.Golden_RG,2) + pow((m_AWBInfo.BGain_fin-m_AWBSpec.Golden_BG)*1.0/m_AWBSpec.Golden_BG,2)+pow((m_AWBInfo.GGain_fin-m_AWBSpec.Golden_GG)*1.0/m_AWBSpec.Golden_GG,2));
		}
		else
		{
			m_OTPDistance=(float)sqrt(pow((m_AWBInfo.RGain_fin-m_AWBSpec.Golden_RG)*1.0/m_AWBSpec.Golden_RG,2) + pow((m_AWBInfo.BGain_fin-m_AWBSpec.Golden_BG)*1.0/m_AWBSpec.Golden_BG,2));
		}
		
		TempMsg.Format(_T("Apply Before:OTPDistance=%4.2f"), m_OTPDistance * 100);
		Beforedistance = m_OTPDistance * 100;
		m_pInterface->Ctrl_Addlog(CamID, TempMsg, COLOR_BLUE, 200);
		if (m_OTPDistance * 100 > pDlg->m_OTP_BeforeDistance)//*1000->*100
		{
			SaveAWBData(ApplyAWBFlag);
			TempMsg.Format(_T("ApplyAWB Distance 超出范围！"));
			m_pInterface->Ctrl_Addlog(CamID, TempMsg, COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("m_OTPDistance Fail!"));
			SetResult(RESULT_FAIL);
			TestResult=TEST_FAIL;
			return 0;	
		}
	}
	SaveAWBData(ApplyAWBFlag);
	if (ApplyAWBFlag == TRUE)
	{
		m_pInterface->Ctrl_PauseGrabFrame(CamID);
		Sleep(500);
		m_pInterface->Sensor_ApplyAWBGain(CamID, OTP_RGain, OTP_BGain, m_AWBSpec.Golden_RG, m_AWBSpec.Golden_BG);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		Sleep(1000);
		ApplyAWBFlag = FALSE;
		return 1;
	}

	return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int ApplyWB_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码
	if (CheckCount<4)
	{
		CheckCount++;
		return 1;
	}
	if (TestResult==TEST_PASS)
	{
		ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
		if (NULL == img.Raw_buffer)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("抓帧2失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("抓帧2失败!"));
			SetResult(RESULT_FAIL);
			TestResult=TEST_FAIL;
			return 0;
		}

		//Save Image	
		if (pDlg->m_SaveImage)
		{
			USES_CONVERSION;
			FilePath = m_pInterface->Ctrl_GetCurrentPath()+_T("\\ApplyAfter\\");
			_mkdir(T2A(FilePath));//创建FilePath目录
			FileName=m_pInterface->Ctrl_GetCamCode()+_T("ApplyAfter.raw");
			CString BmpFileName=m_pInterface->Ctrl_GetCamCode()+_T("ApplyAfter.bmp");
			TempMsg.Format(_T("%d_%s"),CamID,FileName);
			m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,TempMsg,3,NULL);   //BYTE *imgBuffer
			TempMsg.Format(_T("%d_%s"),CamID,BmpFileName);
			m_pInterface->Ctrl_SaveImage(CamID,1,FilePath,TempMsg,2,NULL);   //BYTE *imgBuffer
		}

		//Calculate AWB
		if (pDlg->m_QTITool)
		{
			if(!GetAWBData(img))
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("获取AWB失败!"));
				SetResult(RESULT_FAIL);
				TestResult=TEST_FAIL;
				return 0;
			}
		}
		else
		{
			if (pDlg->m_Raw8_EN)
			{
				GetAWBInfoRaw8(img);
			}
			else
			{
				GetAWBInfoRaw10(img);
			}
		}
		TempMsg.Format(_T("RawR=%d  RawGr=%d\nRawGb=%d  RawB=%d"),m_AWBInfo.Unit_R,m_AWBInfo.Unit_Gr,m_AWBInfo.Unit_Gb,m_AWBInfo.Unit_B);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("[After Dec BLC:%d]:R_fin=%d Gr_fin=%d Gb_fin=%d B_fin=%d"), pDlg->m_BLC, m_AWBInfo.Unit_R_fin, m_AWBInfo.Unit_Gr_fin, m_AWBInfo.Unit_Gb_fin, m_AWBInfo.Unit_B_fin);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("RGain=%d BGain=%d GGain=%d"),m_AWBInfo.RGain_fin,m_AWBInfo.BGain_fin,m_AWBInfo.GGain_fin);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("Golden_R=%d Golden_Gr=%d Golden_Gb=%d Golden_B=%d"),m_AWBSpec.Golden_R,m_AWBSpec.Golden_Gr,m_AWBSpec.Golden_Gb,m_AWBSpec.Golden_B);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		TempMsg.Format(_T("Golden_RGain=%d Golden_BGain=%d Golden_GGain=%d"),m_AWBSpec.Golden_RG,m_AWBSpec.Golden_BG,m_AWBSpec.Golden_GG);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		/*if (!CheckAWBRange())
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB Value not in range!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Fail!"));
			SetResult(RESULT_FAIL);
			return 0;
		}*/   //lx
		 
		float m_OTPDistance = 100;
		if (pDlg->m_GG_GainEn)
		{
			m_OTPDistance=(float)sqrt(pow((m_AWBInfo.RGain_fin-m_AWBSpec.Golden_RG)*1.0/m_AWBSpec.Golden_RG,2) + pow((m_AWBInfo.BGain_fin-m_AWBSpec.Golden_BG)*1.0/m_AWBSpec.Golden_BG,2)+ pow((m_AWBInfo.GGain_fin-m_AWBSpec.Golden_GG)*1.0/m_AWBSpec.Golden_GG,2));
		}
		else
		{
			m_OTPDistance=(float)sqrt(pow((m_AWBInfo.RGain_fin-m_AWBSpec.Golden_RG)*1.0/m_AWBSpec.Golden_RG,2) + pow((m_AWBInfo.BGain_fin-m_AWBSpec.Golden_BG)*1.0/m_AWBSpec.Golden_BG,2));
		}
		Afterdistance=m_OTPDistance*100;
		TempMsg.Format(_T("Apply After: OTPDistance=%4.2f"),m_OTPDistance*100);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
		if (m_OTPDistance * 100 > pDlg->m_OTP_Distance)//修改* 1000 -> * 100  LYC 20180521
		{
			SaveAWBData(ApplyAWBFlag);
			TempMsg.Format(_T("Distance 超出范围！"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Distance 超出范围!"));
			SetResult(RESULT_FAIL);
			TestResult=TEST_FAIL;
			return 0;	
		}
		SaveAWBData(ApplyAWBFlag);
		//保存烧录数据
		if(SaveOTPData() == FALSE)
		{
			TempMsg.Format(_T("AWB数据保存失败！"));
			m_pInterface->Ctrl_Addlog(CamID, TempMsg, COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("AWB Data Save Fail"));
			SetResult(RESULT_FAIL);
			TestResult=TEST_FAIL;
			return 0;
		}
		m_pInterface->Ctrl_Addlog(CamID,_T("AWB TEST PASS!"),COLOR_GREEN,200);
		//m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Pass!")); // 注释 此行打印红色 LYC 20180316
		SetResult(RESULT_PASS);
	}
	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int ApplyWB_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->m_count=ReadConfigInt(_T("count"),0);//单色还是多色光源
	CString suffix=_T("");
	if (pDlg->m_count!=0)
	{
		suffix.Format(_T("_%d"),pDlg->m_count);
	}
	CString CurrentName=GetName();
	SetName(_T("WBSetting"));
	pDlg->m_QTITool=ReadConfigInt(_T("QTITool"),0);
	pDlg->m_flow=ReadConfigInt(_T("flow"),1);
	GAverFlag=ReadConfigInt(_T("GAverFlag"),1);
	if (GAverFlag==1)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO1))->SetCheck(1);
	}
	else if (GAverFlag==2)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	else if (GAverFlag==3)
	{
		((CButton *)pDlg->GetDlgItem(IDC_RADIO3))->SetCheck(1);
	}

	/** 指示Moto光源点检方式是否是动态点检 **/
	m_MotoLightSrcMode = ReadConfigInt(_T("MotoLightSrcMode"), 0);
	if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
	{
		pDlg->GetDlgItem(IDC_STATIC_MotoLightSrcCalWay)->SetWindowText(_T("当前Moto光源点检方式是[动态点检]"));
		pDlg->MotoStaticLightSrcEnable(FALSE);
	}
	else
	{
		pDlg->GetDlgItem(IDC_STATIC_MotoLightSrcCalWay)->SetWindowText(_T("当前Moto光源点检方式是[静态点检]"));
		pDlg->MotoStaticLightSrcEnable(TRUE);
	}

	pDlg->m_Channel_EN  = ReadConfigInt(_T("SingleChannelEnable"),0);//联想使用此行代码获取单通道和Gain的选择设置，与点检和WBCheck使用同一个关键字
	pDlg->m_Gain_EN  = ReadConfigInt(_T("GainEnable"),1);

	pDlg->m_GG_GainEn=ReadConfigInt(_T("GG_GainEn"),0);
	pDlg->m_OTP_BeforeDistance=ReadConfigInt(_T("OTP_BeforeDistance")+suffix,0);

	pDlg->m_Ev_HighCCT = ReadConfigInt(_T("Ev_5000k"),0);
	pDlg->m_u_HighCCT  = ReadConfigInt(_T("u_5000k"),0);
	pDlg->m_v_HighCCT  = ReadConfigInt(_T("v_5000k"),0);
	CString TempStr = L"";
	
	pDlg->m_RGIndex = ReadConfigInt(_T("RG_index")+suffix,10000);
	pDlg->m_BGIndex  = ReadConfigInt(_T("BG_index")+suffix,10000);
	pDlg->m_GGIndex  = ReadConfigInt(_T("GG_index")+suffix,10000);

	pDlg->m_RIndex  = ReadConfigInt(_T("R_index")+suffix,10000);
	pDlg->m_GrIndex  = ReadConfigInt(_T("Gr_index")+suffix,10000);
	pDlg->m_GbIndex  = ReadConfigInt(_T("Gb_index")+suffix,10000);
	pDlg->m_BIndex  = ReadConfigInt(_T("B_index")+suffix,10000);

	//Golden Gain
	pDlg->m_GoldenRG  = ReadConfigInt(_T("Golden_RG")+suffix,200);//ReadConfigInt(_T("Typical_RG")+suffix,200);
	pDlg->m_GoldenBG  = ReadConfigInt(_T("Golden_BG")+suffix,200);//ReadConfigInt(_T("Typical_BG")+suffix,200);
	pDlg->m_GoldenGG  = ReadConfigInt(_T("Golden_GG")+suffix,200);//ReadConfigInt(_T("Typical_GG")+suffix,200);

	m_AWBSpec.Golden_RG = pDlg->m_GoldenRG;
	m_AWBSpec.Golden_BG = pDlg->m_GoldenBG;
	m_AWBSpec.Golden_GG = pDlg->m_GoldenGG;

	//Golden Channel
	pDlg->m_GoldenR  = ReadConfigInt(_T("Golden_R")+suffix,200);//ReadConfigInt(_T("Typical_R")+suffix,200);
	pDlg->m_GoldenGr = ReadConfigInt(_T("Golden_Gr")+suffix,200);//ReadConfigInt(_T("Typical_Gr")+suffix,200);
	pDlg->m_GoldenGb = ReadConfigInt(_T("Golden_Gb")+suffix,200);//ReadConfigInt(_T("Typical_Gb")+suffix,200);
	pDlg->m_GoldenB  = ReadConfigInt(_T("Golden_B")+suffix,200);//ReadConfigInt(_T("Typical_B")+suffix,200);
	m_AWBSpec.Golden_R  = pDlg->m_GoldenR;
	m_AWBSpec.Golden_Gr = pDlg->m_GoldenGr;
	m_AWBSpec.Golden_Gb = pDlg->m_GoldenGb;
	m_AWBSpec.Golden_B  = pDlg->m_GoldenB;

	//Channel Range
	pDlg->m_RLow=ReadConfigInt(_T("R_min")+suffix,30);
	pDlg->m_RHigh=ReadConfigInt(_T("R_max")+suffix,220);
	pDlg->m_GrLow=ReadConfigInt(_T("Gr_min")+suffix,30);
	pDlg->m_GrHigh=ReadConfigInt(_T("Gr_max")+suffix,220);
	pDlg->m_GbLow=ReadConfigInt(_T("Gb_min")+suffix,30);
	pDlg->m_GbHigh=ReadConfigInt(_T("Gb_max")+suffix,220);
	pDlg->m_BLow=ReadConfigInt(_T("B_min")+suffix,30);
	pDlg->m_BHigh=ReadConfigInt(_T("B_max")+suffix,220);

	m_AWBSpec.RHigh=pDlg->m_RHigh;
	m_AWBSpec.RLow=pDlg->m_RLow;
	m_AWBSpec.GrHigh=pDlg->m_GrHigh;
	m_AWBSpec.GrLow=pDlg->m_GrLow;
	m_AWBSpec.GbHigh=pDlg->m_GbHigh;
	m_AWBSpec.GbLow=pDlg->m_GbLow;
	m_AWBSpec.BHigh=pDlg->m_BHigh;
	m_AWBSpec.BLow=pDlg->m_BLow;


	//Gain Range-- 注释不分动态和静态 LYC 20180608 国内项目使用相同的diff卡控范围，与原来不同的是单通道需要减BLC，Gain不需要减BLC,注意index需要设置成10000
	//if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)//动态光源点检采用浮动系数，一般是正负0.05，联想案子这个值会烧录OTP中
	//{
		TempStr = ReadConfigString(_T("RGainHigh_diff"),_T("0.0"));
		pDlg->m_RGainHigh_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("RGainlow_diff"),_T("0.0"));
		pDlg->m_RGainlow_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("BGainHigh_diff"),_T("0.0"));
		pDlg->m_BGainHigh_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("BGainlow_diff"),_T("0.0"));
		pDlg->m_BGainLow_diff = atof(CT2A(TempStr));
	//}
	//else//静态光源点检就直接使用实际值
	//{
	//	pDlg->m_RGainHigh_diff = ReadConfigInt(_T("RG_max"), 1000);
	//	pDlg->m_RGainlow_diff = ReadConfigInt(_T("RG_min"), 220);
	//	pDlg->m_BGainHigh_diff = ReadConfigInt(_T("BG_max"), 1000);
	//	pDlg->m_BGainLow_diff = ReadConfigInt(_T("BG_min"), 220);
	//}

	//pDlg->m_RGLow=ReadConfigInt(_T("RG_min")+suffix,220);
	//pDlg->m_RGHigh=ReadConfigInt(_T("RG_max")+suffix,1000);
	//pDlg->m_BGLow=ReadConfigInt(_T("BG_min")+suffix,220);
	//pDlg->m_BGHigh=ReadConfigInt(_T("BG_max")+suffix,1000);
	pDlg->m_GGLow=ReadConfigInt(_T("GG_min")+suffix,220);
	pDlg->m_GGHigh=ReadConfigInt(_T("GG_max")+suffix,1000);


	//m_AWBSpec.RGainHigh=pDlg->m_RGHigh;
	//m_AWBSpec.RGainlow=pDlg->m_RGLow;
	//m_AWBSpec.BGainHigh=pDlg->m_BGHigh;
	//m_AWBSpec.BGainlow=pDlg->m_BGLow;
	m_AWBSpec.GGainHigh=pDlg->m_GGHigh;
	m_AWBSpec.GGainlow=pDlg->m_GGLow;

	//Golden Range
	pDlg->R_Golden_Min=ReadConfigInt(_T("R_Golden_Min")+suffix,30);
	pDlg->R_Golden_Max=ReadConfigInt(_T("R_Golden_Max")+suffix,220);
	pDlg->Gr_Golden_Min=ReadConfigInt(_T("Gr_Golden_Min")+suffix,30);
	pDlg->Gr_Golden_Max=ReadConfigInt(_T("Gr_Golden_Max")+suffix,220);
	pDlg->Gb_Golden_Min=ReadConfigInt(_T("Gb_Golden_Min")+suffix,30);
	pDlg->Gb_Golden_Max=ReadConfigInt(_T("Gb_Golden_Max")+suffix,220);
	pDlg->B_Golden_Min=ReadConfigInt(_T("B_Golden_Min")+suffix,30);
	pDlg->B_Golden_Max=ReadConfigInt(_T("B_Golden_Max")+suffix,220);
	pDlg->RG_Golden_Min=ReadConfigInt(_T("RG_Golden_Min")+suffix,30);
	pDlg->RG_Golden_Max=ReadConfigInt(_T("RG_Golden_Max")+suffix,220);
	pDlg->BG_Golden_Min=ReadConfigInt(_T("BG_Golden_Min")+suffix,30);
	pDlg->BG_Golden_Max=ReadConfigInt(_T("BG_Golden_Max")+suffix,220);
	pDlg->GG_Golden_Min=ReadConfigInt(_T("GG_Golden_Min")+suffix,30);
	pDlg->GG_Golden_Max=ReadConfigInt(_T("GG_Golden_Max")+suffix,220);

	pDlg->ReCal_RG_diff=ReadConfigInt(_T("ReCal_RG_diff")+suffix,30);
	pDlg->ReCal_BG_diff=ReadConfigInt(_T("ReCal_BG_diff")+suffix,220);
	pDlg->ReCal_GG_diff=ReadConfigInt(_T("ReCal_GG_diff")+suffix,30);


	pDlg->m_Raw8_EN = ReadConfigInt(_T("Raw8_EN"),0);
	pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
	pDlg->m_ROI_H = ReadConfigInt(_T("ROI_H"),5);
	pDlg->m_ROI_V = ReadConfigInt(_T("ROI_V"),5);
	pDlg->m_Multiple = ReadConfigInt(_T("Multiple"),512);

	SetName(CurrentName);
	pDlg->m_OTP_Distance  = ReadConfigInt(_T("OTP_Distance"),50);
	pDlg->m_OTP_BeforeDistance  = ReadConfigInt(_T("m_OTP_BeforeDistance"),150);
	//pDlg->m_Channel_EN  = ReadConfigInt(_T("Channel_EN"),0);// R3架构源代码使用此行代码获取使用单通道还是Gain值的设置
	//pDlg->m_Gain_EN  = ReadConfigInt(_T("Gain_EN"),1);
	pDlg->m_Raw10_EN  = ReadConfigInt(_T("Raw10_EN"),1);
	pDlg->m_SaveImage  = ReadConfigInt(_T("SaveImage"),200);


	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int ApplyWB_TestItem::SaveOption()
{
	if (!TestItemInterface::SaveOption())
	{
		return -1;
	}

	//TODO: 在此添加存储代码	
	WriteConfigInt(_T("count"), pDlg->m_count);

	CString suffix=_T("");
	if (pDlg->m_count != 0)
	{
		suffix.Format(_T("_%d"), pDlg->m_count);
	}

	CString CurrentName=GetName();
	SetName(_T("WBSetting"));
	WriteConfigInt(_T("QTITool"),pDlg->m_QTITool);
	WriteConfigInt(_T("flow"),pDlg->m_flow);
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		GAverFlag=1;
	}
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		GAverFlag=2;
	}
	if (((CButton *)pDlg->GetDlgItem(IDC_RADIO3))->GetCheck())
	{
		GAverFlag=3;
	}
	WriteConfigInt(_T("GAverFlag"),GAverFlag);
	WriteConfigInt(_T("GG_GainEn"),pDlg->m_GG_GainEn);
	WriteConfigInt(_T("OTP_BeforeDistance"+suffix),pDlg->m_OTP_BeforeDistance);
	//Golden Channel
	WriteConfigInt(_T("Golden_R")+suffix, pDlg->m_GoldenR);
	WriteConfigInt(_T("Golden_Gr")+suffix, pDlg->m_GoldenGr);
	WriteConfigInt(_T("Golden_Gb")+suffix, pDlg->m_GoldenGb);
	WriteConfigInt(_T("Golden_B")+suffix, pDlg->m_GoldenB);

	//LightSource
	WriteConfigInt(_T("Ev_5000k"), pDlg->m_Ev_HighCCT);
	WriteConfigInt(_T("u_5000k"), pDlg->m_u_HighCCT);
	WriteConfigInt(_T("v_5000k"), pDlg->m_v_HighCCT);

	//Golden Gain
	WriteConfigInt(_T("Golden_RG")+suffix, pDlg->m_GoldenRG);
	WriteConfigInt(_T("Golden_BG")+suffix, pDlg->m_GoldenBG);
	WriteConfigInt(_T("Golden_GG")+suffix, pDlg->m_GoldenGG);

	//Channel Range
	WriteConfigInt(_T("R_max")+suffix, pDlg->m_RHigh);
	WriteConfigInt(_T("R_min")+suffix, pDlg->m_RLow);
	WriteConfigInt(_T("Gr_max")+suffix, pDlg->m_GrHigh);
	WriteConfigInt(_T("Gr_min")+suffix, pDlg->m_GrLow);
	WriteConfigInt(_T("Gb_max")+suffix, pDlg->m_GbHigh);
	WriteConfigInt(_T("Gb_min")+suffix, pDlg->m_GbLow);
	WriteConfigInt(_T("B_max")+suffix, pDlg->m_BHigh);
	WriteConfigInt(_T("B_min")+suffix, pDlg->m_BLow);

	//Gain Range
	//WriteConfigInt(_T("RG_max")+suffix, pDlg->m_RGHigh);
	//WriteConfigInt(_T("RG_min")+suffix, pDlg->m_RGLow);
	//WriteConfigInt(_T("BG_max")+suffix, pDlg->m_BGHigh);
	//WriteConfigInt(_T("BG_min")+suffix, pDlg->m_BGLow);
	m_MotoLightSrcMode = ReadConfigInt(_T("MotoLightSrcMode"), 0);
	//if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)//动态光源点检采用浮动系数，一般是正负0.05，联想案子这个值会烧录OTP中
	//{
		CString TempStr = L"";
		TempStr.Format(_T("%f"),pDlg->m_RGainHigh_diff);
		WriteConfigString(_T("RGainHigh_diff"),TempStr);
		TempStr.Format(_T("%f"),pDlg->m_RGainlow_diff);
		WriteConfigString(_T("RGainlow_diff"),TempStr);
		TempStr.Format(_T("%f"),pDlg->m_BGainHigh_diff);
		WriteConfigString(_T("BGainHigh_diff"),TempStr);
		TempStr.Format(_T("%f"),pDlg->m_BGainLow_diff);
		WriteConfigString(_T("BGainlow_diff"),TempStr);
	//}
	//else
	//{
	//	WriteConfigInt(_T("RG_max")+suffix, pDlg->m_RGainHigh_diff);
	//	WriteConfigInt(_T("RG_min")+suffix, pDlg->m_RGainlow_diff);
	//	WriteConfigInt(_T("BG_max")+suffix, pDlg->m_BGainHigh_diff);
	//	WriteConfigInt(_T("BG_min")+suffix, pDlg->m_BGainLow_diff);
	//}
	WriteConfigInt(_T("GG_max")+suffix, pDlg->m_GGHigh);
	WriteConfigInt(_T("GG_min")+suffix, pDlg->m_GGLow);

	//Golden Range
	WriteConfigInt(_T("R_Golden_Min")+suffix, pDlg->R_Golden_Min);
	WriteConfigInt(_T("R_Golden_Max")+suffix, pDlg->R_Golden_Max);
	WriteConfigInt(_T("Gr_Golden_Min")+suffix, pDlg->Gr_Golden_Min);
	WriteConfigInt(_T("Gr_Golden_Max")+suffix, pDlg->Gr_Golden_Max);
	WriteConfigInt(_T("Gb_Golden_Min")+suffix, pDlg->Gb_Golden_Min);
	WriteConfigInt(_T("Gb_Golden_Max")+suffix, pDlg->Gb_Golden_Max);
	WriteConfigInt(_T("B_Golden_Min")+suffix, pDlg->B_Golden_Min);
	WriteConfigInt(_T("B_Golden_Max")+suffix, pDlg->B_Golden_Max);
	WriteConfigInt(_T("RG_Golden_Min")+suffix, pDlg->RG_Golden_Min);
	WriteConfigInt(_T("RG_Golden_Max")+suffix, pDlg->RG_Golden_Max);
	WriteConfigInt(_T("BG_Golden_Min")+suffix, pDlg->BG_Golden_Min);
	WriteConfigInt(_T("BG_Golden_Max")+suffix, pDlg->BG_Golden_Max);
	WriteConfigInt(_T("GG_Golden_Min")+suffix, pDlg->GG_Golden_Min);
	WriteConfigInt(_T("GG_Golden_Max")+suffix, pDlg->GG_Golden_Max);

	WriteConfigInt(_T("ReCal_RG_diff")+suffix, pDlg->ReCal_RG_diff);
	WriteConfigInt(_T("ReCal_BG_diff")+suffix, pDlg->ReCal_BG_diff);
	WriteConfigInt(_T("ReCal_GG_diff")+suffix, pDlg->ReCal_GG_diff);

	

	WriteConfigInt(_T("Raw8_EN"), pDlg->m_Raw8_EN);

	WriteConfigInt(_T("BLC"), pDlg->m_BLC);
	WriteConfigInt(_T("ROI_H"), pDlg->m_ROI_H);
	WriteConfigInt(_T("ROI_V"), pDlg->m_ROI_V);
	WriteConfigInt(_T("Multiple"), pDlg->m_Multiple);
	/** 为了配置文件能够在老的上面使用--Added by LiHai--20180502 **/
	WriteConfigInt(_T("Ratio"), pDlg->m_ROI_H/*pDlg->m_ROIWidth*/);
	WriteConfigInt(_T("index"), pDlg->m_Multiple/*pDlg->m_Ratio*/);

	WriteConfigInt(_T("R_index")+suffix,pDlg->m_RIndex);
	WriteConfigInt(_T("Gr_index")+suffix,pDlg->m_GrIndex);
	WriteConfigInt(_T("Gb_index")+suffix,pDlg->m_GbIndex);
	WriteConfigInt(_T("B_index")+suffix,pDlg->m_BIndex);
	WriteConfigInt(_T("RG_index")+suffix,pDlg->m_RGIndex);
	WriteConfigInt(_T("BG_index")+suffix,pDlg->m_BGIndex);
	WriteConfigInt(_T("GG_index")+suffix,pDlg->m_GGIndex);

	SetName(CurrentName);
	WriteConfigInt(_T("OTP_Distance"), pDlg->m_OTP_Distance);
	WriteConfigInt(_T("m_OTP_BeforeDistance"), pDlg->m_OTP_BeforeDistance);
	WriteConfigInt(_T("SaveImage"), pDlg->m_SaveImage);
	WriteConfigInt(_T("Channel_EN"), pDlg->m_Channel_EN);
	WriteConfigInt(_T("Gain_EN"), pDlg->m_Gain_EN);
	WriteConfigInt(_T("Raw10_EN"), pDlg->m_Raw10_EN); 


	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR ApplyWB_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");

	header += _T("Ev,U,V,");

	header += _T("OTP_R,OTP_Gr,OTP_Gb,OTP_B,");

	header += _T("OTP_RGain,OTP_BGain,OTP_GGain,");

	header += _T("APPLY_R,APPLY_Gr,APPLY_Gb,APPLY_B,");

	header += _T("APPLY_RGain,APPLY_BGain,APPLY_GGain,");
	header += _T("BeforeDistance,AfterDistance,");
	if(pDlg->m_Gain_EN)
	{
		header += _T("Golden_RG,Golden_BG,Golden_GG,RGIndex,BGIndex,GGIndex,");
	}

	if(pDlg->m_Channel_EN)
	{
		header += _T("Golden_R,Golden_Gr,Golden_Gb,Golden_B,RIndex,GrIndex,GbIndex,BIndex");
	}


	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR ApplyWB_TestItem::GetReportLowLimit()
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
LPCTSTR ApplyWB_TestItem::GetReportHightLimit()
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
LPCTSTR ApplyWB_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	CString temp;
	temp.Format(_T("%d,%d,%d,"),pDlg->m_Ev_HighCCT,pDlg->m_u_HighCCT,pDlg->m_v_HighCCT);
	Content+=temp;

	temp.Format(_T("%d,%d,%d,%d,"),OTP_R,OTP_Gr,OTP_Gb,OTP_B);
	Content+=temp;

	temp.Format(_T("%d,%d,%d,"),OTP_RGain,OTP_BGain,OTP_GGain);

	Content+=temp;

	/**
	*	GetReportContents()和GetReportHeads()中内容长度不一致时会导致MES上传失败
	*	--Added by LiHai--20180131
	**/
	//if (TestResult==TEST_PASS)
	{
		temp.Format(_T("%d,%d,%d,%d,"),m_AWBInfo.Unit_R_fin,m_AWBInfo.Unit_Gr_fin,m_AWBInfo.Unit_Gb_fin,m_AWBInfo.Unit_B_fin);
		Content+=temp;

		temp.Format(_T("%d,%d,%d,"),m_AWBInfo.RGain_fin,m_AWBInfo.BGain_fin,m_AWBInfo.GGain_fin);
		Content+=temp;
	}
	temp.Format(_T("%4.2f,%4.2f,"),Beforedistance,Afterdistance);
	Content+=temp;



	if(pDlg->m_Gain_EN)
	{
		temp.Format(_T("%d,%d,%d,%d,%d,%d"),m_AWBSpec.Golden_RG,m_AWBSpec.Golden_BG,m_AWBSpec.Golden_GG,pDlg->m_RGIndex, pDlg->m_BGIndex,pDlg->m_GGIndex);
		Content+=temp;
	}

	if(pDlg->m_Channel_EN)
	{
		temp.Format(_T("%d,%d,%d,%d,%d,%d,%d,%d"),m_AWBSpec.Golden_R,m_AWBSpec.Golden_Gr,m_AWBSpec.Golden_Gb,m_AWBSpec.Golden_B,pDlg->m_RIndex,pDlg->m_GrIndex,pDlg->m_GbIndex,pDlg->m_BIndex);
		Content+=temp;
	}


	return Content;
}

void ApplyWB_TestItem::GetAWBInfoRaw8(ImageInfo img)
{
	int  width   = img.imgwidth;
	int  height   =   img.imgheight;
	BYTE * Raw8buffer = new BYTE[width * height];
	if (NULL == Raw8buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Raw8buffer buffer 申请失败!"),COLOR_RED,200);
		return;
	}
	Raw10toRaw8(img.Raw_buffer, Raw8buffer, width * height);
	//calculate rawWidth & rawHeight  , size of ROI 
	int rawWidth  = width / (pDlg->m_ROI_H * 2) * 2;    //multiply two is to make sure that width & height is even  because of bayer order
	int rawHeight = height / (pDlg->m_ROI_V * 2) * 2; 

	//联想手机曝光和计算AWB数据都是取的【中间128*128像素区域】来计算的--Added by LiHai--20171213
	if ((pDlg->m_ROI_H > 100) && (pDlg->m_ROI_V > 100))
	{
		rawWidth = pDlg->m_ROI_H;
		rawHeight = pDlg->m_ROI_V;
	}

	int  StartX = (width  - rawWidth ) / 4 * 2;
	int  StartY = (height  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

	rect.left       = StartX;
	rect.right     = EndX;
	rect.top       = StartY;
	rect.bottom = EndY;


	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1  = 0, Pos2 = 0;

	int  SumGr = 0;
	double	Gr = 0.0;

	int  SumGb = 0;
	double Gb = 0.0;

	int  SumR  = 0;
	double R  = 0.0;

	int  SumB  = 0;
	double B  = 0.0;


	if ( img.RawFormat== OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumR  += Raw8buffer[Pos1];
				SumGr += Raw8buffer[Pos1 + 1];
				SumGb += Raw8buffer[Pos2];
				SumB  += Raw8buffer[Pos2 + 1];
				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGr += Raw8buffer[Pos1];
				SumR  += Raw8buffer[Pos1 + 1];
				SumB  += Raw8buffer[Pos2];
				SumGb += Raw8buffer[Pos2 + 1]; 

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGb += Raw8buffer[Pos1];
				SumB  += Raw8buffer[Pos1 + 1];
				SumR  += Raw8buffer[Pos2];
				SumGr += Raw8buffer[Pos2 + 1];

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumB  += Raw8buffer[Pos1];
				SumGb += Raw8buffer[Pos1 + 1];
				SumGr += Raw8buffer[Pos2];
				SumR  += Raw8buffer[Pos2 + 1];

				Count++;
				//so,OV8865 run to here
			}
		}
	}

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;

	m_AWBInfo.Unit_Gr = int(Gr);
	m_AWBInfo.Unit_Gb = int(Gb);
	m_AWBInfo.Unit_R  = int(R);
	m_AWBInfo.Unit_B  = int(B);
	float AA = 0.0;
	if (pDlg->m_flow)
	{
		AA = 0.5;
	}
	if (GAverFlag == 1)
	{
		m_AWBInfo.RGain = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) +AA );
	}
	else if (GAverFlag==2)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int(  1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) +AA );
	}
	else if (GAverFlag==3)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R -pDlg->m_BLC)/(m_AWBInfo.Unit_Gb-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B -pDlg->m_BLC)/(m_AWBInfo.Unit_Gb-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int(  1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gr-pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC) +AA );
	}

	if (pDlg->m_Channel_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC)*m_AWBSpec.RIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC)*m_AWBSpec.GrIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC)*m_AWBSpec.GbIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC)*m_AWBSpec.BIndex /10000.0 + 0.5);
	}
	else if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC));
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC));
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC));
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC));
	}


	if (pDlg->m_Channel_EN)
	{
		if(GAverFlag==1)
		{
			m_AWBInfo.RGain_fin = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Golden_RG=int(2.0*pDlg->m_Multiple*m_AWBSpec.Golden_R/(m_AWBSpec.Golden_Gr+m_AWBSpec.Golden_Gb)+0.5);
			m_AWBSpec.Golden_BG=int(2.0*pDlg->m_Multiple*m_AWBSpec.Golden_B/(m_AWBSpec.Golden_Gr+m_AWBSpec.Golden_Gb)+0.5);
			m_AWBSpec.Golden_GG=int(2.0*pDlg->m_Multiple*m_AWBSpec.Golden_Gr/(m_AWBSpec.Golden_Gr+m_AWBSpec.Golden_Gb)+0.5);
		}
		else if (GAverFlag==2)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin)   + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin) +0.5 );

			m_AWBSpec.Golden_RG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_R/(m_AWBSpec.Golden_Gr)+0.5);
			m_AWBSpec.Golden_BG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_B/(m_AWBSpec.Golden_Gr)+0.5);
			m_AWBSpec.Golden_GG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_Gb/(m_AWBSpec.Golden_Gr)+0.5);
		}
		else if (GAverFlag==3)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gb_fin)   + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int(  1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gr_fin)/( m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Golden_RG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_R/(m_AWBSpec.Golden_Gb)+0.5);
			m_AWBSpec.Golden_BG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_B/(m_AWBSpec.Golden_Gb)+0.5);
			m_AWBSpec.Golden_GG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_Gr/(m_AWBSpec.Golden_Gb)+0.5);
		}
		CString temp;
		temp.Format(_T("Golden_R=%d\nGolden_Gr=%d\nGolden_Gb=%d\nGolden_B=%d"),m_AWBSpec.Golden_R,m_AWBSpec.Golden_Gr,m_AWBSpec.Golden_Gb,m_AWBSpec.Golden_B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	else if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.RGain_fin = int(m_AWBInfo.RGain * m_AWBSpec.RGIndex /10000.0 + 0.5);
		m_AWBInfo.BGain_fin = int(m_AWBInfo.BGain * m_AWBSpec.BGIndex /10000.0 + 0.5);
		m_AWBInfo.GGain_fin = int(m_AWBInfo.GGain * m_AWBSpec.GGIndex /10000.0 + 0.5);
	}

	SAFE_DELETE_ARRAY(Raw8buffer);
}

void ApplyWB_TestItem::GetAWBInfoRaw10(ImageInfo img)
{
	int  width  = img.imgwidth;
	int  height = img.imgheight;
	//calculate rawWidth & rawHeight  , size of ROI 
	int  rawWidth  = width / (pDlg->m_ROI_H * 2) * 2;    //multiply two is to make sure that width & height is even  because of bayer order
	int  rawHeight = height / (pDlg->m_ROI_V * 2) * 2; 

	//联想手机曝光和计算AWB数据都是取的【中间128*128像素区域】来计算的--Added by LiHai--20171213
	if ((pDlg->m_ROI_H > 100) && (pDlg->m_ROI_V > 100))
	{
		rawWidth = pDlg->m_ROI_H;
		rawHeight = pDlg->m_ROI_V;
	}

	int  StartX = (width  - rawWidth ) / 4 * 2;
	int  StartY = (height  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

	rect.left   = StartX;
	rect.right  = EndX;
	rect.top    = StartY;
	rect.bottom = EndY;


	int  x = 0;
	int  y = 0;
	int  Count = 0;
	int  Pos1  = 0, Pos2 = 0;

	int  SumGr = 0;
	double	Gr = 0.0;

	int  SumGb = 0;
	double Gb = 0.0;

	int  SumR  = 0;
	double R  = 0.0;

	int  SumB  = 0;
	double B  = 0.0;


	if ( img.RawFormat== OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumR  += img.Raw_buffer[Pos1];
				SumGr += img.Raw_buffer[Pos1 + 1];
				SumGb += img.Raw_buffer[Pos2];
				SumB  += img.Raw_buffer[Pos2 + 1];
				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGr += img.Raw_buffer[Pos1];
				SumR  += img.Raw_buffer[Pos1 + 1];
				SumB  += img.Raw_buffer[Pos2];
				SumGb += img.Raw_buffer[Pos2 + 1]; 

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGb += img.Raw_buffer[Pos1];
				SumB  += img.Raw_buffer[Pos1 + 1];
				SumR  += img.Raw_buffer[Pos2];
				SumGr += img.Raw_buffer[Pos2 + 1];

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumB  += img.Raw_buffer[Pos1];
				SumGb += img.Raw_buffer[Pos1 + 1];
				SumGr += img.Raw_buffer[Pos2];
				SumR  += img.Raw_buffer[Pos2 + 1];

				Count++;
				//so,OV8865 run to here
			}
		}
	}

	Gr = 1.0 * SumGr / Count;
	Gb = 1.0 * SumGb / Count;
	R  = 1.0 * SumR  / Count;
	B  = 1.0 * SumB  / Count;

	float AA = 0.0;
	if (pDlg->m_flow)
	{
		AA = 0.5;
	}
	m_AWBInfo.Unit_Gr = int(Gr + AA);// added  + AA LYC 20180315
	m_AWBInfo.Unit_Gb = int(Gb + AA);
	m_AWBInfo.Unit_R  = int(R + AA);
	m_AWBInfo.Unit_B  = int(B + AA);

	if (GAverFlag == 1)
	{
		m_AWBInfo.RGain = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr+m_AWBInfo.Unit_Gb-2*pDlg->m_BLC) + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gr/*-pDlg->m_BLC*/)/( m_AWBInfo.Unit_Gb/*-pDlg->m_BLC*/) + AA );// 修改Gb/Gr->Gr/Gb 不减BLC LYC 20180317
	}
	else if (GAverFlag == 2)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb/*-pDlg->m_BLC*/)/( m_AWBInfo.Unit_Gr/*-pDlg->m_BLC*/) + AA );
	}
	else if (GAverFlag == 3)
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC) + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gr/*-pDlg->m_BLC*/)/( m_AWBInfo.Unit_Gb/*-pDlg->m_BLC*/) + AA );
	}

	CString temp;
	temp.Format(_T("Raw_R=%d\nRaw_Gr=%d\nRaw_Gb=%d\nRaw_B=%d\nRaw_RG:%d\nRaw_BG:%d\nRaw_GG:%d\r\n"),
					m_AWBInfo.Unit_R,
					m_AWBInfo.Unit_Gr,
					m_AWBInfo.Unit_Gb,
					m_AWBInfo.Unit_B,
					m_AWBInfo.RGain,
					m_AWBInfo.BGain,
					m_AWBInfo.GGain
					);
	m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);

	if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
	{
		/** Moto动态点检:单通道值是不减BLC的，直接存储原始值的四舍五入值--Added by LiHai--20180507 **/
		m_AWBInfo.Unit_R_fin = m_AWBInfo.Unit_R;
		m_AWBInfo.Unit_Gr_fin = m_AWBInfo.Unit_Gr;
		m_AWBInfo.Unit_Gb_fin = m_AWBInfo.Unit_Gb;
		m_AWBInfo.Unit_B_fin = m_AWBInfo.Unit_B;

		/** Moto动态点检:RG、BG的值是用单通道值先减BLC然后按16384的公式计算，但是GG是不减BLC然后按16384的公式计算--Added by LiHai--20180507 **/
		m_AWBInfo.RGain_fin = m_AWBInfo.RGain;
		m_AWBInfo.BGain_fin = m_AWBInfo.BGain;
		m_AWBInfo.GGain_fin = m_AWBInfo.GGain;
	}
	else
	{
		if (pDlg->m_Channel_EN)
		{
			m_AWBInfo.Unit_R_fin = int((m_AWBInfo.Unit_R - pDlg->m_BLC) * m_AWBSpec.RIndex / 10000.0 + 0.5);
			m_AWBInfo.Unit_Gr_fin = int((m_AWBInfo.Unit_Gr - pDlg->m_BLC) * m_AWBSpec.GrIndex / 10000.0 + 0.5);
			m_AWBInfo.Unit_Gb_fin = int((m_AWBInfo.Unit_Gb - pDlg->m_BLC) * m_AWBSpec.GbIndex / 10000.0 + 0.5);
			m_AWBInfo.Unit_B_fin = int((m_AWBInfo.Unit_B - pDlg->m_BLC) * m_AWBSpec.BIndex / 10000.0 + 0.5);

			if (GAverFlag == 1)
			{
				m_AWBInfo.RGain_fin = int( 2.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / (float)( m_AWBInfo.Unit_Gr_fin + m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
				m_AWBInfo.BGain_fin = int( 2.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / (float)( m_AWBInfo.Unit_Gr_fin + m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
				m_AWBInfo.GGain_fin = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gr_fin) / (float)( m_AWBInfo.Unit_Gb_fin) + 0.5 );
			}
			else if (GAverFlag == 2)
			{
				m_AWBInfo.RGain_fin = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );// Rave/Gave
				m_AWBInfo.BGain_fin = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );  //Bave/Gave 
				m_AWBInfo.GGain_fin = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gb_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );
			}
			else if (GAverFlag == 3)
			{
				m_AWBInfo.RGain_fin = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
				m_AWBInfo.BGain_fin = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
				m_AWBInfo.GGain_fin = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gr_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );
			}
		}
		else if (pDlg->m_Gain_EN)
		{
			m_AWBInfo.Unit_R_fin = int(m_AWBInfo.Unit_R - pDlg->m_BLC);
			m_AWBInfo.Unit_Gr_fin = int(m_AWBInfo.Unit_Gr - pDlg->m_BLC);
			m_AWBInfo.Unit_Gb_fin = int(m_AWBInfo.Unit_Gb - pDlg->m_BLC);
			m_AWBInfo.Unit_B_fin = int(m_AWBInfo.Unit_B - pDlg->m_BLC);

			if (GAverFlag == 1)
			{
				m_AWBInfo.RGain = int( 2.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / (float)( m_AWBInfo.Unit_Gr_fin + m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
				m_AWBInfo.BGain = int( 2.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / (float)( m_AWBInfo.Unit_Gr_fin + m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
				m_AWBInfo.GGain = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gr_fin) / (float)( m_AWBInfo.Unit_Gb_fin) + 0.5 );
			}
			else if (GAverFlag == 2)
			{
				m_AWBInfo.RGain = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );// Rave/Gave
				m_AWBInfo.BGain = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );  //Bave/Gave 
				m_AWBInfo.GGain = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gb_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );
			}
			else if (GAverFlag == 3)
			{
				m_AWBInfo.RGain = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
				m_AWBInfo.BGain = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
				m_AWBInfo.GGain = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gr_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );
			}

			m_AWBInfo.RGain_fin = int(m_AWBInfo.RGain * (float)m_AWBSpec.RGIndex / 10000.0 + 0.5);
			m_AWBInfo.BGain_fin = int(m_AWBInfo.BGain * (float)m_AWBSpec.BGIndex / 10000.0 + 0.5);
			m_AWBInfo.GGain_fin = int(m_AWBInfo.GGain * (float)m_AWBSpec.GGIndex / 10000.0 + 0.5);
		}
	}

	temp.Format(_T("Final_R=%d\nFinal_Gr=%d\nFinal_Gb=%d\nFinal_B=%d\nFinal_RG:%d\nFinal_BG:%d\nFinal_GG:%d\r\n"),
				m_AWBInfo.Unit_R_fin,
				m_AWBInfo.Unit_Gr_fin,
				m_AWBInfo.Unit_Gb_fin,
				m_AWBInfo.Unit_B_fin,
				m_AWBInfo.RGain_fin,
				m_AWBInfo.BGain_fin,
				m_AWBInfo.GGain_fin
				);
	m_pInterface->Ctrl_Addlog(CamID, temp, COLOR_BLUE, 200);
}

BOOL ApplyWB_TestItem::CheckAWBRange()
{
	if (m_AWBInfo.Unit_R_fin > m_AWBInfo.Unit_Gr_fin
		|| m_AWBInfo.Unit_R_fin > m_AWBInfo.Unit_Gb_fin
		|| m_AWBInfo.Unit_B_fin > m_AWBInfo.Unit_Gr_fin
		|| m_AWBInfo.Unit_B_fin > m_AWBInfo.Unit_Gb_fin)
	{
		TempMsg.Format(_T("Unit_R_fin=%d Unit_B_fin=%d > <%d,%d>"), m_AWBInfo.Unit_R_fin, m_AWBInfo.Unit_B_fin, m_AWBInfo.Unit_Gr_fin, m_AWBInfo.Unit_Gb_fin);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("单通道AWB不符合规律!"),COLOR_RED,200);
		return 0;
	}
	if (m_AWBInfo.RGain_fin > m_AWBInfo.GGain_fin || m_AWBInfo.BGain_fin > m_AWBInfo.GGain_fin)
	{
		TempMsg.Format(_T("RGain_fin=%d BGain_fin=%d > <%d>"), m_AWBInfo.RGain_fin, m_AWBInfo.Unit_B_fin, m_AWBInfo.GGain_fin);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("单通道AWB不符合规律!"),COLOR_RED,200);
		return 0;
	}

	if (m_AWBInfo.Unit_R_fin > m_AWBSpec.RHigh || m_AWBInfo.Unit_R_fin < m_AWBSpec.RLow)
	{
		TempMsg.Format(_T("Unit_R_fin=%d not in<%d,%d>"), m_AWBInfo.Unit_R_fin, m_AWBSpec.RLow, m_AWBSpec.RHigh);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("R值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (m_AWBInfo.Unit_Gr_fin > m_AWBSpec.GrHigh || m_AWBInfo.Unit_Gr_fin < m_AWBSpec.GrLow)
	{
		TempMsg.Format(_T("Unit_Gr_fin=%d not in<%d,%d>"), m_AWBInfo.Unit_Gr_fin, m_AWBSpec.GrLow, m_AWBSpec.GrHigh);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("Gr值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (m_AWBInfo.Unit_Gb_fin > m_AWBSpec.GbHigh || m_AWBInfo.Unit_Gb_fin < m_AWBSpec.GbLow)
	{
		TempMsg.Format(_T("Unit_Gb_fin=%d not in<%d,%d>"), m_AWBInfo.Unit_Gb_fin, m_AWBSpec.GbLow, m_AWBSpec.GbHigh);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("Gb值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (m_AWBInfo.Unit_B_fin > m_AWBSpec.BHigh || m_AWBInfo.Unit_B_fin < m_AWBSpec.BLow)
	{
		TempMsg.Format(_T("Unit_B_fin=%d not in<%d,%d>"), m_AWBInfo.Unit_B_fin, m_AWBSpec.BLow, m_AWBSpec.BHigh);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("B值不在范围!"),COLOR_RED,200);
		return 0;
	}

	//if ((m_AWBInfo.RGain_fin > m_AWBSpec.RGainHigh) || (m_AWBInfo.RGain_fin< m_AWBSpec.RGainlow) )
	//{
	//	TempMsg.Format(_T("RGain_fin=%d not in<%d,%d>"), m_AWBInfo.RGain_fin, m_AWBSpec.RGainlow, m_AWBSpec.RGainHigh);
	//	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
	//	m_pInterface->Ctrl_Addlog(CamID,_T("RGain值不在范围!"),COLOR_RED,200);
	//	return 0;
	//}
	//if ((m_AWBInfo.BGain_fin > m_AWBSpec.BGainHigh) || (m_AWBInfo.BGain_fin < m_AWBSpec.BGainlow) )
	//{
	//	TempMsg.Format(_T("BGain_fin=%d not in<%d,%d>"), m_AWBInfo.BGain_fin, m_AWBSpec.BGainlow, m_AWBSpec.BGainHigh);
	//	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
	//	m_pInterface->Ctrl_Addlog(CamID,_T("BGain值不在范围!"),COLOR_RED,200);
	//	return 0;
	//}
	//if (pDlg->m_Gain_EN)//所有的点检方式都要卡控7个值的范围 added LYC 20180521
	//{
		int RGainHigh_HighCCT = 0;
		int RGainLow_HighCCT  = 0;   
		int BGainHigh_HighCCT = 0;
		int BGainLow_HighCCT  = 0;
		//if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)--国内项目也使用diff进行卡控 LYC 20180608
		//{
			RGainHigh_HighCCT = int(m_AWBSpec.Golden_RG + pDlg->m_RGainHigh_diff * pDlg->m_Multiple + 0.5);
			RGainLow_HighCCT  = int(m_AWBSpec.Golden_RG + pDlg->m_RGainlow_diff  * pDlg->m_Multiple + 0.5);   
			BGainHigh_HighCCT = int(m_AWBSpec.Golden_BG + pDlg->m_BGainHigh_diff * pDlg->m_Multiple + 0.5);
			BGainLow_HighCCT  = int(m_AWBSpec.Golden_BG + pDlg->m_BGainLow_diff  * pDlg->m_Multiple + 0.5);
		//}
		//else
		//{
		//	RGainHigh_HighCCT = int(pDlg->m_RGainHigh_diff + 0.5);
		//	RGainLow_HighCCT  = int(pDlg->m_RGainlow_diff + 0.5);   
		//	BGainHigh_HighCCT = int(pDlg->m_BGainHigh_diff + 0.5);
		//	BGainLow_HighCCT  = int(pDlg->m_BGainLow_diff + 0.5);
		//}

		if ((m_AWBInfo.RGain_fin > RGainHigh_HighCCT) || (m_AWBInfo.RGain_fin < RGainLow_HighCCT))
		{
			TempMsg.Format(_T("RGain_fin=%d not in<%d,%d>"), m_AWBInfo.RGain_fin, RGainLow_HighCCT, RGainHigh_HighCCT);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("RGain值不在范围!"),COLOR_RED,200);
			return 0;
		}

		if ((m_AWBInfo.BGain_fin > BGainHigh_HighCCT) || (m_AWBInfo.BGain_fin < BGainLow_HighCCT))
		{
			TempMsg.Format(_T("BGain_fin=%d not in<%d,%d>"), m_AWBInfo.BGain_fin, BGainLow_HighCCT, BGainHigh_HighCCT);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("BGain值不在范围!"),COLOR_RED,200);
			return 0;
		}
		if ((m_AWBInfo.GGain_fin > m_AWBSpec.GGainHigh) || (m_AWBInfo.GGain_fin < m_AWBSpec.GGainlow) )
		{
			TempMsg.Format(_T("GGain_fin=%d not in<%d,%d>"), m_AWBInfo.GGain_fin, m_AWBSpec.GGainlow, m_AWBSpec.GGainHigh);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("GGain值不在范围!"),COLOR_RED,200);
			return 0;
		}
	//}

	int Cal_RG = 0;
	int Cal_BG = 0;
	int Cal_GG = 0;
	float AA = 0.0;
	if (pDlg->m_flow)
	{
		AA = 0.5;
	}
	if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
	{
		if (GAverFlag == 1)
		{
			Cal_RG = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin-2*pDlg->m_BLC) + AA );// Rave/Gave
			Cal_BG = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin-2*pDlg->m_BLC) + AA );  //Bave/Gave 
			Cal_GG = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gr_fin/*-pDlg->m_BLC*/)/( m_AWBInfo.Unit_Gb_fin/*-pDlg->m_BLC*/) + AA );// 修改Gb/Gr->Gr/Gb 不减BLC LYC 20180317
		}
		else if (GAverFlag == 2)
		{
			Cal_RG = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr_fin-pDlg->m_BLC) + AA );// Rave/Gave
			Cal_BG = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr_fin-pDlg->m_BLC) + AA );  //Bave/Gave 
			Cal_GG = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb_fin/*-pDlg->m_BLC*/)/( m_AWBInfo.Unit_Gr_fin/*-pDlg->m_BLC*/) + AA );
		}
		else if (GAverFlag == 3)
		{
			Cal_RG = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin -pDlg->m_BLC)/( m_AWBInfo.Unit_Gb_fin-pDlg->m_BLC) + AA );// Rave/Gave
			Cal_BG = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin -pDlg->m_BLC)/( m_AWBInfo.Unit_Gb_fin-pDlg->m_BLC) + AA );  //Bave/Gave 
			Cal_GG = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gr_fin/*-pDlg->m_BLC*/)/( m_AWBInfo.Unit_Gb_fin/*-pDlg->m_BLC*/) + AA );
		}

		if ((Cal_RG!=m_AWBInfo.RGain_fin)||(Cal_BG!=m_AWBInfo.BGain_fin)||(Cal_GG!=m_AWBInfo.GGain_fin))
		{
			TempMsg.Format(_T("Cal_RG=%d,RG_OTP=%d\nCal_BG=%d,BG_OTP=%d\nCal_GG=%d,GG_OTP=%d"), 
				Cal_RG,m_AWBInfo.RGain_fin,Cal_BG,m_AWBInfo.BGain_fin,Cal_GG,m_AWBInfo.GGain_fin);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("OTP通过四通道重新计算的比值与存储的比值对不上!"),COLOR_RED,200);
			return 0;
		}
	}
	else
	{
		if (GAverFlag == 1)
		{
			Cal_RG = int( 2.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / ( m_AWBInfo.Unit_Gr_fin + m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			Cal_BG = int( 2.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / ( m_AWBInfo.Unit_Gr_fin + m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			Cal_GG = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gr_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );
		}
		else if (GAverFlag == 2)
		{
			Cal_RG = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );// Rave/Gave
			Cal_BG = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );  //Bave/Gave 
			Cal_GG = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gb_fin) / ( m_AWBInfo.Unit_Gr_fin) + 0.5 );
		}
		else if (GAverFlag == 3)
		{
			Cal_RG = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_R_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			Cal_BG = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_B_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			Cal_GG = int( 1.0 * pDlg->m_Multiple * (m_AWBInfo.Unit_Gr_fin) / ( m_AWBInfo.Unit_Gb_fin) + 0.5 );
		}

		if (pDlg->m_Channel_EN)
		{
			if ((Cal_RG!=m_AWBInfo.RGain_fin)||(Cal_BG!=m_AWBInfo.BGain_fin)||(Cal_GG!=m_AWBInfo.GGain_fin))
			{
				TempMsg.Format(_T("Cal_RG=%d,RG_OTP=%d\nCal_BG=%d,BG_OTP=%d\nCal_GG=%d,GG_OTP=%d"), 
					Cal_RG,m_AWBInfo.RGain_fin,Cal_BG,m_AWBInfo.BGain_fin,Cal_GG,m_AWBInfo.GGain_fin);
				m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
				m_pInterface->Ctrl_Addlog(CamID,_T("OTP通过四通道重新计算的比值与存储的比值对不上!"),COLOR_RED,200);
				return 0;
			}
		}
	}

	return 1;
}

BOOL ApplyWB_TestItem::CheckGoldenRange()
{
	if (pDlg->m_GoldenR > pDlg->R_Golden_Max || pDlg->m_GoldenR < pDlg->R_Golden_Min)
	{
		TempMsg.Format(_T("m_GoldenR=%d not in<%d,%d>"), pDlg->m_GoldenR, pDlg->R_Golden_Min, pDlg->R_Golden_Max);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("Golden_R值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (pDlg->m_GoldenGr > pDlg->Gr_Golden_Max || pDlg->m_GoldenGr < pDlg->Gr_Golden_Min)
	{
		TempMsg.Format(_T("m_GoldenGr=%d not in<%d,%d>"), pDlg->m_GoldenGr, pDlg->Gr_Golden_Min, pDlg->Gr_Golden_Max);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("Golden_Gr值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (pDlg->m_GoldenGb > pDlg->Gb_Golden_Max || pDlg->m_GoldenGb < pDlg->Gb_Golden_Min)
	{
		TempMsg.Format(_T("m_GoldenGb=%d not in<%d,%d>"), pDlg->m_GoldenGb, pDlg->Gb_Golden_Min, pDlg->Gb_Golden_Max);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("Golden_Gb值不在范围!"),COLOR_RED,200);
		return 0;
	}
	if (pDlg->m_GoldenB > pDlg->B_Golden_Max || pDlg->m_GoldenB < pDlg->B_Golden_Min)
	{
		TempMsg.Format(_T("m_GoldenB=%d not in<%d,%d>"), pDlg->m_GoldenB, pDlg->B_Golden_Min, pDlg->B_Golden_Max);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
		m_pInterface->Ctrl_Addlog(CamID,_T("Golden_B值不在范围!"),COLOR_RED,200);
		return 0;
	}

	if (pDlg->m_Gain_EN)
	{
		if (pDlg->m_GoldenRG > pDlg->RG_Golden_Max || pDlg->m_GoldenRG < pDlg->RG_Golden_Min)
		{
			TempMsg.Format(_T("m_GoldenRG=%d not in<%d,%d>"), pDlg->m_GoldenRG, pDlg->RG_Golden_Min, pDlg->RG_Golden_Max);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("Golden_RGain值不在范围!"),COLOR_RED,200);
			return 0;
		}

		if (pDlg->m_GoldenBG > pDlg->BG_Golden_Max || pDlg->m_GoldenBG < pDlg->BG_Golden_Min)
		{
			TempMsg.Format(_T("m_GoldenBG=%d not in<%d,%d>"), pDlg->m_GoldenBG, pDlg->BG_Golden_Min, pDlg->BG_Golden_Max);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("Golden_BGain值不在范围!"),COLOR_RED,200);
			return 0;
		}
		if ((pDlg->m_GoldenGG > m_AWBSpec.GGainHigh) || (pDlg->m_GoldenGG < m_AWBSpec.GGainlow) )//Golden_GG的范围是否使用OTP_GG的范围pDlg->GG_Golden_Max
		{
			TempMsg.Format(_T("m_GoldenGG=%d not in<%d,%d>"), pDlg->m_GoldenGG, m_AWBSpec.GGainlow, m_AWBSpec.GGainHigh);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("Golden_GGain值不在范围!"),COLOR_RED,200);
			return 0;
		}

		int Cal_RG=int(pDlg->m_Multiple * (pDlg->m_GoldenR-pDlg->m_BLC)/((pDlg->m_GoldenGr+pDlg->m_GoldenGb)/2.0-pDlg->m_BLC)+0.5);
		int Cal_BG=int(pDlg->m_Multiple * (pDlg->m_GoldenB-pDlg->m_BLC)/((pDlg->m_GoldenGr+pDlg->m_GoldenGb)/2.0-pDlg->m_BLC)+0.5);
		int Cal_GG=int(pDlg->m_Multiple * pDlg->m_GoldenGr/pDlg->m_GoldenGb+0.5);

		if ((abs(Cal_RG-pDlg->m_GoldenRG)>pDlg->ReCal_RG_diff)||
			(abs(Cal_BG-pDlg->m_GoldenBG)>pDlg->ReCal_BG_diff)||
			(abs(Cal_GG-pDlg->m_GoldenGG)>pDlg->ReCal_GG_diff))
		{
			TempMsg.Format(_T("<%d,%d> RG_diff=%d\n<%d,%d> BG_diff=%d\n<%d,%d> GG_diff=%d"), Cal_RG,pDlg->m_GoldenRG,pDlg->ReCal_RG_diff,
				Cal_BG,pDlg->m_GoldenBG,pDlg->ReCal_BG_diff,Cal_GG,pDlg->m_GoldenGG,pDlg->ReCal_GG_diff);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("通过Golden四通道计算的Gain值和实际存储的Gain值差异不在范围内!"),COLOR_RED,200);
			return 0;
		}
		if (Cal_RG > pDlg->RG_Golden_Max || Cal_RG < pDlg->RG_Golden_Min)
		{
			TempMsg.Format(_T("Cal_RG=%d not in<%d,%d>"), Cal_RG, pDlg->RG_Golden_Min, pDlg->RG_Golden_Max);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("通过四通道计算的RG值不在范围!"),COLOR_RED,200);
			return 0;
		}

		if (Cal_BG > pDlg->BG_Golden_Max || Cal_BG < pDlg->BG_Golden_Min)
		{
			TempMsg.Format(_T("Cal_BG=%d not in<%d,%d>"), Cal_BG, pDlg->BG_Golden_Min, pDlg->BG_Golden_Max);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("通过四通道计算的BG值不在范围!"),COLOR_RED,200);
			return 0;
		}
		if ((Cal_GG > m_AWBSpec.GGainHigh) || (Cal_GG < m_AWBSpec.GGainlow) )//Golden_GG的范围是否使用OTP_GG的范围pDlg->GG_Golden_Max
		{
			TempMsg.Format(_T("Cal_GG=%d not in<%d,%d>"), Cal_GG, m_AWBSpec.GGainlow, m_AWBSpec.GGainHigh);
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			m_pInterface->Ctrl_Addlog(CamID,_T("通过四通道计算的GG值不在范围!"),COLOR_RED,200);
			return 0;
		}
	}
	return 1;
}

BOOL ApplyWB_TestItem::SaveAWBData(BOOL ApplyAWBFlag)
{
	FILE  *fp  = NULL;
	CTime time = CTime::GetCurrentTime();
	CString fileName;
	CString otpDataFileName;
	CString applyWbDataFileName;
	CString shadingDataFileName;
	CString lightsourceDataFileName;
	CString RecordTime;
	CString path;
	CString SNnumber;
	CFile MyFile;
	long fileSizeTemp=0;
	int j;
	USES_CONVERSION;
	RecordTime.Format(_T("%02d:%02d:%02d"), time.GetHour(), time.GetMinute(), time.GetSecond());

	path = _T("D:\\Data\\");
	_mkdir(T2A(path));
	path = path + m_strTestStation + _T("\\");
	_mkdir(T2A(path));

	if (m_pInterface->GetSiteNo() == 0 || m_pInterface->GetSiteNo() == 1)
	{
		otpDataFileName.Format(_T("WB_Data_Site_A%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay());
		applyWbDataFileName.Format(_T("ApplyWB_OTP_Data_Site_A%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay());
	}
	else if (m_pInterface->GetSiteNo() == 2)
	{
		otpDataFileName.Format(_T("WB_Data_Site_B%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay());
		applyWbDataFileName.Format(_T("ApplyWB_OTP_Data_Site_B%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay());
	}
	else if (m_pInterface->GetSiteNo() == 3)
	{
		otpDataFileName.Format(_T("WB_Data_Site_C%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay());
		applyWbDataFileName.Format(_T("ApplyWB_OTP_Data_Site_C%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay());
	}
	else if (m_pInterface->GetSiteNo() == 4)
	{
		otpDataFileName.Format(_T("WB_Data_Site_D%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay());
		applyWbDataFileName.Format(_T("ApplyWB_OTP_Data_Site_D%04d%02d%02d.xls"), time.GetYear(), time.GetMonth(), time.GetDay());
	}
	
	if (ApplyAWBFlag)
	{
		fileName = path + otpDataFileName;
	}
	else
	{
		fileName = path + applyWbDataFileName;
	}
	

	fp=freopen(T2A(fileName),"at",stdout);
	if(fp==NULL)  
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("open file fail!"),COLOR_RED,200);
		return FALSE;
	}
	fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);

	if(fileSizeTemp==0)
	{
		printf("SN\tFUSE ID\tTest Time\t");
		if (ApplyAWBFlag)
		{
			printf("R_OTP\tGr_OTP\tGb_OTP\tB_OTP\tRG_OTP\tBG_OTP\tGG_OTP\tR_Gol\tGr_Gol\tGb_Gol\tB_Gol\tRG_Gol\tBG_Gol\tGG_Gol\t");
		}
		else
		{
			printf("R\tGr\tGb\tB\tRGain_fin\tBGain_fin\tGrGbGain_fin\tGolden_RG\tGolden_BG\tDistance");
			//printf("R_ApplyWB\tGr_ApplyWB\tGb_ApplyWB\tB_ApplyWB\tRG_ApplyWB\tBG_ApplyWB\tGG_ApplyWB\tR_Gol\tGr_Gol\tGb_Gol\tB_Gol\tRG_Gol\tBG_Gol\tGG_Gol\t");
		}
	}
	printf("\n");

	printf(T2A(m_pInterface->Ctrl_GetCamCode()));
	printf("\t");

	printf(T2A(m_pInterface->Sensor_GetCamFuseID(CamID)));
	printf("#\t"); 

	printf(T2A(RecordTime));
	printf("\t");

	if (ApplyAWBFlag)
	{
		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", m_AWBInfo.Unit_R_fin, m_AWBInfo.Unit_Gr_fin, m_AWBInfo.Unit_Gb_fin,
			m_AWBInfo.Unit_B_fin,m_AWBInfo.RGain_fin,m_AWBInfo.BGain_fin,m_AWBInfo.GGain_fin,m_AWBSpec.Golden_R,m_AWBSpec.Golden_Gr,
			m_AWBSpec.Golden_Gb,m_AWBSpec.Golden_B,m_AWBSpec.Golden_RG,m_AWBSpec.Golden_BG,m_AWBSpec.Golden_GG);
	}
	else
	{
		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%4.2f", m_AWBInfo.Unit_R_fin, m_AWBInfo.Unit_Gr_fin, m_AWBInfo.Unit_Gb_fin,
			m_AWBInfo.Unit_B_fin,m_AWBInfo.RGain_fin,m_AWBInfo.BGain_fin,m_AWBInfo.GGain_fin,m_AWBSpec.Golden_RG,m_AWBSpec.Golden_BG,
			Afterdistance);
	}
	fclose(fp);
	return TRUE;
}
BOOL ApplyWB_TestItem::SaveOTPData()
{
	USES_CONVERSION;
	FilePath.Format(_T("otpdataManager\\Cam%d\\otpInputByte\\"), CamID);
	FilePath = m_pInterface->Ctrl_GetCurrentPath() + FilePath;

	int siteNo = m_pInterface->GetSiteNo();
	_mkdir(T2A(FilePath));
	if (pDlg->m_count!=0)
	{
		FileName.Format(_T("AWBData_%d_%d.ini"), siteNo, pDlg->m_count);
	}
	else
	{
		FileName.Format(_T("AWBData_%d.ini"), siteNo);//写入的路径为【F:\OHP0792\Execute\Debug\otpdataManager】, 还可以使用m_pInterface->Otp_WriteToOtpBoard函数存储
	}
	FileName = FilePath + FileName;
	//删除旧文件
	if (PathFileExists(FileName))
	{
		if (!DeleteFile(FileName))
		{
			TempMsg.Format(_T("无法删除旧OTP文件!"));
			m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_RED,200);
			return FALSE;
		}
	}
	ofstream outfile(FileName,ios::out|ios::trunc);

	outfile<<"[AWBData]"<<endl;

	outfile<<"OTP_R="<<OTP_R<<endl;
	outfile<<"OTP_Gr="<<OTP_Gr<<endl;
	outfile<<"OTP_Gb="<<OTP_Gb<<endl;
	outfile<<"OTP_B="<<OTP_B<<endl;

	outfile<<"OTP_RGain="<<OTP_RGain<<endl;
	outfile<<"OTP_BGain="<<OTP_BGain<<endl;
	outfile<<"OTP_GGain="<<OTP_GGain<<endl;

	outfile<<"Ev_5000k="<<pDlg->m_Ev_HighCCT<<endl;
	outfile<<"u_5000k="<<pDlg->m_u_HighCCT<<endl;
	outfile<<"v_5000k="<<pDlg->m_v_HighCCT<<endl;

	outfile<<"RGainHigh_diff="<<pDlg->m_RGainHigh_diff<<endl;
	outfile<<"RGainlow_diff="<<pDlg->m_RGainlow_diff<<endl;
	outfile<<"BGainHigh_diff="<<pDlg->m_BGainHigh_diff<<endl;
	outfile<<"BGainlow_diff="<<pDlg->m_BGainLow_diff<<endl;

	outfile<<"Golden_R="<<pDlg->m_GoldenR<<endl;
	outfile<<"Golden_Gr="<<pDlg->m_GoldenGr<<endl;
	outfile<<"Golden_Gb="<<pDlg->m_GoldenGb<<endl;
	outfile<<"Golden_B="<<pDlg->m_GoldenB<<endl;

	outfile<<"Golden_RG="<<pDlg->m_GoldenRG<<endl;
	outfile<<"Golden_BG="<<pDlg->m_GoldenBG<<endl;
	outfile<<"Golden_GG="<<pDlg->m_GoldenGG<<endl;
	
	outfile.close();

	return TRUE;
}

BOOL ApplyWB_TestItem::GetAWBData(ImageInfo img)
{
#if LSC_PATH_DEFINE == 1
	char AWB_LSC_File[1024]={0};
	char ONLY_LSC_File[1024]={0};
	char ONLY_AWB_File[1024]={0};
#endif
	int R,Gr,Gb,B;
	CString ToolPath;
	ToolPath.Format(_T("AWBData\\Cam%d\\"),CamID);
	ToolPath=m_pInterface->Ctrl_GetCurrentPath()+ToolPath;
	SetCurrentDirectory(ToolPath);	//下面LSCCalibrationDll.dll库不带路径，所以需要设置系统默认工作目录，否则最后的结果在exe目录下

	CString sfilename=ToolPath+_T("AWB_LSC_CALIBRATION_DATA.txt");
#if LSC_PATH_DEFINE == 1
	USES_CONVERSION;
	sprintf(AWB_LSC_File,W2A(ToolPath.GetBuffer()));
#endif
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB_LSC_CALIBRATION_DATA.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt");
#if LSC_PATH_DEFINE == 1
	sprintf(ONLY_LSC_File,W2A(ToolPath.GetBuffer()));
#endif
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt");
#if LSC_PATH_DEFINE == 1
	sprintf(ONLY_AWB_File,W2A(ToolPath.GetBuffer()));
#endif
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}

#if LSC_PATH_DEFINE == 1
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7,char * AWB_LSC_Path,char * ONLY_LSC_Path,char * ONLY_AWB_Path);
#else
	typedef int (*lpFun)(unsigned char * pImage, int nWidth, int nHeight,
		int CFA_pattern, short R_black_offset, short Gr_black_offset,
		short Gb_black_offset, short B_black_offset, bool bMode9x7);
#endif

	HINSTANCE hDll;   //DLL句柄 
	sfilename=ToolPath+_T("LSCCalibrationDll.dll");
	hDll = LoadLibrary(sfilename);
	if (NULL==hDll)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("LSCCalibrationDll.dll丢失,请将文件放回程式目录!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}

	lpFun LensCorrection = (lpFun)GetProcAddress(hDll,"LensCorrectionLib");
	if (NULL==LensCorrection)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("DLL中函数寻找失败!!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}
	int  width   = img.imgwidth;
	int  height   =   img.imgheight;
	BYTE * Raw8buffer=new BYTE[width*height];
	if (NULL == Raw8buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Raw8buffer buffer 申请失败!!"),COLOR_RED,200);
		FreeLibrary(hDll);
		return FALSE;
	}
	Raw10toRaw8(img.Raw_buffer,Raw8buffer,width*height);
	int CFA_Pattern = OUTFORMAT_RGGB/*0*/;
	if ((OUTFORMAT_RGGB == img.RawFormat/* == 0*/)
		|| (OUTFORMAT_GRBG == img.RawFormat/* == 1*/)
		)
	{
		CFA_Pattern = img.RawFormat;
	}
	if (img.RawFormat == OUTFORMAT_GBRG/*2*/)
	{
		CFA_Pattern = 3;	//注意：高通库中此值和我们定义的值不同
	}
	if (img.RawFormat == OUTFORMAT_BGGR/*3*/)
	{
		CFA_Pattern = 2;	//注意：高通库中此值和我们定义的值不同
	}
	CFile file1;
	BOOL ret = file1.Open(ToolPath+_T("Raw8.raw"), CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	if (ret)
	{
		file1.Write((void*)Raw8buffer, width * height * sizeof(BYTE) );
		file1.Close();
	}

#if LSC_PATH_DEFINE == 1
	LensCorrection(Raw8buffer,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,0,AWB_LSC_File,ONLY_LSC_File,ONLY_AWB_File);
#else
	LensCorrection(Raw8buffer,width,height,CFA_Pattern,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,pDlg->m_BLC,0);
#endif
	SAFE_DELETE_ARRAY(Raw8buffer);
	FreeLibrary(hDll);
	CStdioFile file;
	CString Path = ToolPath;
	Path = Path + _T("AWB_LSC_CALIBRATION_DATA.txt");

	if(file.Open(Path , CFile::modeRead | CFile::typeText ))
	{
		CString strTemp;
		CString a;
		while(file.ReadString(strTemp))
		{
			if (strTemp.Find(_T("r_oc_ave"),0) == -1)
			{
				continue;
			}
			else
			{
				float temp=0.00;
				a = strTemp.Mid(10,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				R=(temp+0.5); //R

				file.ReadString(strTemp);
				a = strTemp.Mid(10,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				B=(temp+0.5); //B

				file.ReadString(strTemp);
				a = strTemp.Mid(11,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				Gr=(temp+0.5); //Gr

				file.ReadString(strTemp);
				a = strTemp.Mid(11,5);
				temp=_wtof(a.GetBuffer(a.GetLength()));
				Gb=(temp+0.5); //Gb
				break;
			}	
		}
		file.Close();
	}
	else {
		m_pInterface->Ctrl_Addlog(CamID,_T("打开AWB_LSC_CALIBRATION_DATA.txt失败!"),COLOR_RED,200);
		return FALSE;
	}

	/** 注意高通工具出来的原始各通道平均值【R B Gr Gb】是减去了BLC的值, 以下四个值是加回了BLC数据的, 所以是图片原始的各通道平均值 **/
	/** 需要确定工具里面Gr、Gb、R、B四个值是否有减BLC? **/
	m_AWBInfo.Unit_Gr = int(Gr)+pDlg->m_BLC;
	m_AWBInfo.Unit_Gb = int(Gb)+pDlg->m_BLC;
	m_AWBInfo.Unit_R  = int(R)+pDlg->m_BLC;
	m_AWBInfo.Unit_B  = int(B)+pDlg->m_BLC;
	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	if (GAverFlag==1)//除数是(Gr + Gb)/2
	{
		m_AWBInfo.RGain = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R - pDlg->m_BLC)/( m_AWBInfo.Unit_Gr + m_AWBInfo.Unit_Gb - 2 * pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B - pDlg->m_BLC)/( m_AWBInfo.Unit_Gr + m_AWBInfo.Unit_Gb - 2 * pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb - pDlg->m_BLC)/( m_AWBInfo.Unit_Gr - pDlg->m_BLC) + AA );
	}
	else if (GAverFlag==2)//除数是Gr
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B -pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int(  1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb-pDlg->m_BLC)/( m_AWBInfo.Unit_Gr-pDlg->m_BLC) + AA );
	}
	else if (GAverFlag==3)//除数是Gb
	{
		m_AWBInfo.RGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R -pDlg->m_BLC)/(m_AWBInfo.Unit_Gb-pDlg->m_BLC) + AA );// Rave/Gave
		m_AWBInfo.BGain = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B -pDlg->m_BLC)/(m_AWBInfo.Unit_Gb-pDlg->m_BLC)  + AA );  //Bave/Gave 
		m_AWBInfo.GGain = int(  1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gr-pDlg->m_BLC)/( m_AWBInfo.Unit_Gb-pDlg->m_BLC) + AA );
	}

	if (pDlg->m_Channel_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC)*m_AWBSpec.RIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC)*m_AWBSpec.GrIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC)*m_AWBSpec.GbIndex /10000.0 + 0.5);
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC)*m_AWBSpec.BIndex /10000.0 + 0.5);
	}
	else if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.Unit_R_fin=int((m_AWBInfo.Unit_R-pDlg->m_BLC));
		m_AWBInfo.Unit_Gr_fin=int((m_AWBInfo.Unit_Gr-pDlg->m_BLC));
		m_AWBInfo.Unit_Gb_fin=int((m_AWBInfo.Unit_Gb-pDlg->m_BLC));
		m_AWBInfo.Unit_B_fin=int((m_AWBInfo.Unit_B-pDlg->m_BLC));
	}

	if (pDlg->m_Channel_EN)
	{
		if(GAverFlag==1)
		{
			m_AWBInfo.RGain_fin = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int( 2.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin+m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Golden_RG=int(2.0*pDlg->m_Multiple*m_AWBSpec.Golden_R/(m_AWBSpec.Golden_Gr+m_AWBSpec.Golden_Gb)+0.5);
			m_AWBSpec.Golden_BG=int(2.0*pDlg->m_Multiple*m_AWBSpec.Golden_B/(m_AWBSpec.Golden_Gr+m_AWBSpec.Golden_Gb)+0.5);
			m_AWBSpec.Golden_GG=int(2.0*pDlg->m_Multiple*m_AWBSpec.Golden_Gr/(m_AWBSpec.Golden_Gr+m_AWBSpec.Golden_Gb)+0.5);
		}
		else if (GAverFlag==2)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gr_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gr_fin)   + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int(  1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gb_fin)/( m_AWBInfo.Unit_Gr_fin) +0.5 );

			m_AWBSpec.Golden_RG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_R/(m_AWBSpec.Golden_Gr)+0.5);
			m_AWBSpec.Golden_BG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_B/(m_AWBSpec.Golden_Gr)+0.5);
			m_AWBSpec.Golden_GG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_Gb/(m_AWBSpec.Golden_Gr)+0.5);
		}
		else if (GAverFlag==3)
		{
			m_AWBInfo.RGain_fin = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_R_fin)/( m_AWBInfo.Unit_Gb_fin) + 0.5 );// Rave/Gave
			m_AWBInfo.BGain_fin = int( 1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_B_fin)/( m_AWBInfo.Unit_Gb_fin) + 0.5 );  //Bave/Gave 
			m_AWBInfo.GGain_fin =int(  1.0*pDlg->m_Multiple*(m_AWBInfo.Unit_Gr_fin)/( m_AWBInfo.Unit_Gb_fin) +0.5 );

			m_AWBSpec.Golden_RG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_R/(m_AWBSpec.Golden_Gb)+0.5);
			m_AWBSpec.Golden_BG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_B/(m_AWBSpec.Golden_Gb)+0.5);
			m_AWBSpec.Golden_GG=int(1.0*pDlg->m_Multiple*m_AWBSpec.Golden_Gr/(m_AWBSpec.Golden_Gb)+0.5);
		}
		CString temp;
		temp.Format(_T("Golden_R=%d Golden_Gr=%d Golden_Gb=%d Golden_B=%d"),m_AWBSpec.Golden_R,m_AWBSpec.Golden_Gr,m_AWBSpec.Golden_Gb,m_AWBSpec.Golden_B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	
	if (pDlg->m_Gain_EN)
	{
		m_AWBInfo.RGain_fin = int(m_AWBInfo.RGain * m_AWBSpec.RGIndex /10000.0 + 0.5);
		m_AWBInfo.BGain_fin = int(m_AWBInfo.BGain * m_AWBSpec.BGIndex /10000.0 + 0.5);
		m_AWBInfo.GGain_fin = int(m_AWBInfo.GGain * m_AWBSpec.GGIndex /10000.0 + 0.5);
	}

	return TRUE;
}

void ApplyWB_TestItem::Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}