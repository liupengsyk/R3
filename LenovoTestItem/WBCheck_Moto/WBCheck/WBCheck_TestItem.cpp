#include "StdAfx.h"
#include "WBCheck_TestItem.h"
#include "ImageProc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new WBCheck_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
WBCheck_TestItem::WBCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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
	flag=0;

	//.....
}

WBCheck_TestItem::~WBCheck_TestItem(void)
{

}


int WBCheck_TestItem::InitItem()
{
	CamID = GetCamID();
	CurrentName=GetName();
	GlobalSetting=_T("WBSetting");
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption(); 
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int WBCheck_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
	AfterBG=0;
	AfterGG=0;
	AfterR=0;
	AfterGr=0;
	AfterGb=0;
	AfterB=0;
	AfterRG_fin=0;
	AfterBG_fin=0;
	AfterGG_fin=0;
	AfterR_fin=0;
	AfterGr_fin=0;
	AfterGb_fin=0;
	AfterB_fin=0;
	Afterdistance=0.0;//保存数据
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int WBCheck_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	if (flag==0)
	{
		flag=BeforeAWB();
		return flag;
	}
	else
	{
		if(flag>=3)
		{
			flag=0;
			int iresult=AfterAWB();
			if (iresult==0)
			{
				//m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Test Pass!"));// 注释此行 失败时使用该接口打印结果 LYC 20180316
				m_pInterface->Ctrl_Addlog(CamID,_T("AWB Check PASS!"),COLOR_GREEN,200);
				SetResult(RESULT_PASS);
			}
		}
		else
		{
			flag++;
			return 1;
		}
	}

	CString strLog;

	//strLog.Format(_T("IsGoldenForCopy:%d\r\n"), pDlg->m_IsGoldenForCopy);
	//m_pInterface->Ctrl_SetCamErrorInfo(CamID, strLog.GetBuffer(strLog.GetLength()));
	if (pDlg->m_IsGoldenForCopy)
	{
		USHORT OTP_RG;
		USHORT OTP_BG;
		if (ByteCount == 1)
		{
			m_pInterface->Otp_OtpRead(RG_StartAdd, RG_StartAdd, &OTP_RG, CamID, pDlg->page);
			m_pInterface->Otp_OtpRead(BG_StartAdd, BG_StartAdd, &OTP_BG, CamID, pDlg->page);
		}
		else if (ByteCount == 2)
		{
			USHORT OTPData[2] = {0};
			m_pInterface->Otp_OtpRead(RG_StartAdd, RG_StartAdd + 1, OTPData, CamID, pDlg->page);
			OTP_RG = OTPData[0] * A_Multi + OTPData[1] * B_Multi;
			m_pInterface->Otp_OtpRead(BG_StartAdd, BG_StartAdd + 1, OTPData, CamID, pDlg->page);
			OTP_BG = OTPData[0] * A_Multi + OTPData[1] * B_Multi;
		}

		float RG_Read = OTP_RG;     //判断是否为Golden模组
		float BG_Read = OTP_BG;

		float RG_Golden = pDlg->m_TypicalRG;
		float BG_Golden = pDlg->m_TypicalBG;

		float ErrorValue = sqrt(pow((RG_Read - RG_Golden), 2) + pow((BG_Read - BG_Golden), 2));

		strLog.Format(_T("RG_Read:%d  BG_Read:%d  Golden_RG:%d  Golden_BG:%d  ErrorValue:%f  ErrH:%d  ErrL:%d\r\n"),
						OTP_RG,
						OTP_BG,
						pDlg->m_TypicalRG,
						pDlg->m_TypicalBG,
						ErrorValue,
						pDlg->m_GoldenCopy_ErrH,
						pDlg->m_GoldenCopy_ErrL
						);
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()),COLOR_YELLOW,200);

		if (ErrorValue < pDlg->m_GoldenCopy_ErrH
			&& ErrorValue > pDlg->m_GoldenCopy_ErrL
			)
		{
			AfxMessageBox(_T("该模组为Golden，请单独拿出!"));
		}
	}

   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int WBCheck_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int WBCheck_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码
	pDlg->m_count=ReadConfigInt(_T("count"),12);
	CString suffix=_T("");
	if (pDlg->m_count!=0)
	{
		suffix.Format(_T("_%d"),pDlg->m_count);
	}
	SetName(GlobalSetting);	
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


	pDlg->m_before_distance=ReadConfigInt(_T("before_distance_check")+suffix, 50);
	pDlg->m_GG_GainEn=ReadConfigInt(_T("GG_GainEn"), 0);
	pDlg->m_SlaveID= ReadConfigString(_T("SlaveID"), _T("0x20"));
	pDlg->R_StartAdd= ReadConfigString(_T("R_StartAdd")+suffix, _T("0x0000"));
	pDlg->Gr_StartAdd= ReadConfigString(_T("Gr_StartAdd")+suffix, _T("0x0000"));
	pDlg->Gb_StartAdd= ReadConfigString(_T("Gb_StartAdd")+suffix, _T("0x0000"));
	pDlg->B_StartAdd= ReadConfigString(_T("B_StartAdd")+suffix, _T("0x0000"));
	pDlg->RG_StartAdd= ReadConfigString(_T("RG_StartAdd")+suffix, _T("0x0000"));
	pDlg->BG_StartAdd= ReadConfigString(_T("BG_StartAdd")+suffix, _T("0x0000"));
	pDlg->GG_StartAdd= ReadConfigString(_T("GG_StartAdd")+suffix, _T("0x0000"));
	pDlg->R_min=ReadConfigInt(_T("R_min")+suffix,30);
	pDlg->R_max=ReadConfigInt(_T("R_max")+suffix,220);
	pDlg->Gr_min=ReadConfigInt(_T("Gr_min")+suffix,30);
	pDlg->Gr_max=ReadConfigInt(_T("Gr_max")+suffix,220);
	pDlg->Gb_min=ReadConfigInt(_T("Gb_min")+suffix,30);
	pDlg->Gb_max=ReadConfigInt(_T("Gb_max")+suffix,220);
	pDlg->B_min=ReadConfigInt(_T("B_min")+suffix,30);
	pDlg->B_max=ReadConfigInt(_T("B_max")+suffix,220);
	pDlg->RG_min=ReadConfigInt(_T("RG_min")+suffix,220);
	pDlg->RG_max=ReadConfigInt(_T("RG_max")+suffix,1000);
	pDlg->BG_min=ReadConfigInt(_T("BG_min")+suffix,220);
	pDlg->BG_max=ReadConfigInt(_T("BG_max")+suffix,1000);
	pDlg->GG_min=ReadConfigInt(_T("GG_min")+suffix,220);
	pDlg->GG_max=ReadConfigInt(_T("GG_max")+suffix,1000);
	pDlg->ByteCount=ReadConfigString(_T("ByteCount"), _T("2"));
	pDlg->A_Multi=ReadConfigString(_T("A_Multi"), _T("256"));
	pDlg->B_Multi=ReadConfigString(_T("B_Multi"), _T("1"));
	//pDlg->Station=ReadConfigString(_T("Station"), _T("OHWBA06"));
	pDlg->SingleChannelEnable=ReadConfigInt(_T("SingleChannelEnable"),1);
	pDlg->GainEnable=ReadConfigInt(_T("GainEnable"),0);
	pDlg->m_BLC=ReadConfigInt(_T("BLC"),16);
	pDlg->m_ROI_H=ReadConfigInt(_T("ROI_H"),5);
	pDlg->m_ROI_V=ReadConfigInt(_T("ROI_V"),5);
	pDlg->m_Multiple=ReadConfigInt(_T("Multiple"),512);
	pDlg->page=ReadConfigInt(_T("page")+suffix,0);
	m_SlaveID= ReadConfigInt(_T("SlaveID"), 0x20);
	R_StartAdd=ReadConfigInt(_T("R_StartAdd")+suffix, 0x0000);
	Gr_StartAdd=ReadConfigInt(_T("Gr_StartAdd")+suffix, 0x0000);
	Gb_StartAdd=ReadConfigInt(_T("Gb_StartAdd")+suffix, 0x0000);
	B_StartAdd=ReadConfigInt(_T("B_StartAdd")+suffix, 0x0000);
	RG_StartAdd=ReadConfigInt(_T("RG_StartAdd")+suffix, 0x0000);
	BG_StartAdd=ReadConfigInt(_T("BG_StartAdd")+suffix, 0x0000);
	GG_StartAdd=ReadConfigInt(_T("GG_StartAdd")+suffix, 0x0000);
	ByteCount=ReadConfigInt(_T("ByteCount"), 2);
	A_Multi=ReadConfigInt(_T("A_Multi"), 256);
	B_Multi=ReadConfigInt(_T("B_Multi"), 1);
	pDlg->m_TypicalR  = ReadConfigInt(_T("Golden_R")+suffix,200);
	pDlg->m_TypicalGr  = ReadConfigInt(_T("Golden_Gr")+suffix,200);
	pDlg->m_TypicalGb  = ReadConfigInt(_T("Golden_Gb")+suffix,200);
	pDlg->m_TypicalB  = ReadConfigInt(_T("Golden_B")+suffix,200);

	//Typical Gain
	pDlg->m_TypicalRG  = ReadConfigInt(_T("Golden_RG")+suffix,200);
	pDlg->m_TypicalBG  = ReadConfigInt(_T("Golden_BG")+suffix,200);
	pDlg->m_TypicalGG  = ReadConfigInt(_T("Golden_GG")+suffix,200);
	pDlg->m_Raw8_EN  = ReadConfigInt(_T("Raw8_EN"),1);
	pDlg->m_distance  = ReadConfigInt(_T("distance_check")+suffix,1);
	R_index=ReadConfigInt(_T("R_index")+suffix,10000);
	Gr_index=ReadConfigInt(_T("Gr_index")+suffix,10000);
	Gb_index=ReadConfigInt(_T("Gb_index")+suffix,10000);
	B_index=ReadConfigInt(_T("B_index")+suffix,10000);
	RG_index=ReadConfigInt(_T("RG_index")+suffix,10000);
	BG_index=ReadConfigInt(_T("BG_index")+suffix,10000);
    GG_index=ReadConfigInt(_T("GG_index")+suffix,10000);

	/** 指示Moto光源点检方式是否是动态点检 **/
	m_MotoLightSrcMode = ReadConfigInt(_T("MotoLightSrcMode"), 0);
	if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
	{
		pDlg->GetDlgItem(IDC_STATIC_MotoLightSrcCalWay)->SetWindowText(_T("当前Moto光源点检方式是[动态点检]"));


		/** Step 1:读取当前Golden值 **/
		//int tmpGoldenRG  = ReadConfigInt(_T("Golden_RG") + suffix, 200);//ReadConfigInt(_T("Typical_RG")+suffix,200);
		//int tmpGoldenBG  = ReadConfigInt(_T("Golden_BG") + suffix, 200);//ReadConfigInt(_T("Typical_BG")+suffix,200);
		//pDlg->m_GoldenGG  = ReadConfigInt(_T("Golden_GG")+suffix,200);//ReadConfigInt(_T("Typical_GG")+suffix,200);

		/** Step 2:读取当前上下浮动比例 **/
		CString TempStr = ReadConfigString(_T("RGainHigh_diff"),_T("0.0"));
		float tmpRGainHigh_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("RGainlow_diff"),_T("0.0"));
		float tmpRGainlow_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("BGainHigh_diff"),_T("0.0"));
		float tmpBGainHigh_diff = atof(CT2A(TempStr));
		TempStr = ReadConfigString(_T("BGainlow_diff"),_T("0.0"));
		float tmpBGainLow_diff = atof(CT2A(TempStr));

		/** Step 3:计算上下浮动范围 **/
		pDlg->RG_min = (int)(pDlg->m_TypicalRG + tmpRGainlow_diff * 16384.0 + 0.5);	//注意本身有负号，所以用【加】
		pDlg->RG_max = (int)(pDlg->m_TypicalRG + tmpRGainHigh_diff * 16384.0 + 0.5);
		pDlg->BG_min = (int)(pDlg->m_TypicalBG + tmpBGainLow_diff * 16384.0 + 0.5);   //注意本身有负号，所以用【加】
		pDlg->BG_max = (int)(pDlg->m_TypicalBG + tmpBGainHigh_diff * 16384.0 + 0.5);
	}
	else
	{
		pDlg->GetDlgItem(IDC_STATIC_MotoLightSrcCalWay)->SetWindowText(_T("当前Moto光源点检方式是[静态点检]"));
	}


	SetName(CurrentName);

	pDlg->m_IsGoldenForCopy = ReadConfigInt(_T("IsGoldenForCopy"), 1);
	pDlg->m_GoldenCopy_ErrH = ReadConfigInt(_T("GoldenCopy_ErrH"), 5);
	pDlg->m_GoldenCopy_ErrL = ReadConfigInt(_T("GoldenCopy_ErrL"), 0);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int WBCheck_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码
	CString suffix=_T("");
	if (pDlg->m_count!=0)
	{
		suffix.Format(_T("_%d"),pDlg->m_count);
	}
	WriteConfigInt(_T("count"),pDlg->m_count);
	SetName(GlobalSetting);
	WriteConfigInt(_T("QTITool"),pDlg->m_QTITool);
	WriteConfigInt(_T("flow"),pDlg->m_flow);
	WriteConfigString(_T("SlaveID"), pDlg->m_SlaveID);
	WriteConfigString(_T("R_StartAdd")+suffix, pDlg->R_StartAdd);
	WriteConfigString(_T("Gr_StartAdd")+suffix, pDlg->Gr_StartAdd);
	WriteConfigString(_T("Gb_StartAdd")+suffix, pDlg->Gb_StartAdd);
	WriteConfigString(_T("B_StartAdd")+suffix, pDlg->B_StartAdd);
	WriteConfigString(_T("RG_StartAdd")+suffix, pDlg->RG_StartAdd);
	WriteConfigString(_T("BG_StartAdd")+suffix, pDlg->BG_StartAdd);
	WriteConfigString(_T("GG_StartAdd")+suffix, pDlg->GG_StartAdd);
	WriteConfigInt(_T("R_min")+suffix,pDlg->R_min);
	WriteConfigInt(_T("R_max")+suffix,pDlg->R_max);
	WriteConfigInt(_T("Gr_min")+suffix,pDlg->Gr_min);
	WriteConfigInt(_T("Gr_max")+suffix,pDlg->Gr_max);
	WriteConfigInt(_T("Gb_min")+suffix,pDlg->Gb_min);
	WriteConfigInt(_T("Gb_max")+suffix,pDlg->Gb_max);
	WriteConfigInt(_T("B_min")+suffix,pDlg->B_min);
	WriteConfigInt(_T("B_max")+suffix,pDlg->B_max);
	WriteConfigInt(_T("RG_min")+suffix,pDlg->RG_min);
	WriteConfigInt(_T("RG_max")+suffix,pDlg->RG_max);
	WriteConfigInt(_T("BG_min")+suffix,pDlg->BG_min);
	WriteConfigInt(_T("BG_max")+suffix,pDlg->BG_max);
	WriteConfigInt(_T("GG_min")+suffix,pDlg->GG_min);
	WriteConfigInt(_T("GG_max")+suffix,pDlg->GG_max);
	WriteConfigString(_T("ByteCount"), pDlg->ByteCount);
	WriteConfigString(_T("A_Multi"), pDlg->A_Multi);
	WriteConfigString(_T("B_Multi"), pDlg->B_Multi);
	//WriteConfigString(_T("Station"), pDlg->Station);
	WriteConfigInt(_T("SingleChannelEnable"),pDlg->SingleChannelEnable);
	WriteConfigInt(_T("GainEnable"),pDlg->GainEnable);
	WriteConfigInt(_T("ROI_H"),pDlg->m_ROI_H);
	WriteConfigInt(_T("ROI_V"),pDlg->m_ROI_V);
	WriteConfigInt(_T("BLC"),pDlg->m_BLC);
	WriteConfigInt(_T("Multiple"),pDlg->m_Multiple);
	WriteConfigInt(_T("Golden_R")+suffix, pDlg->m_TypicalR);
	WriteConfigInt(_T("Golden_Gr")+suffix, pDlg->m_TypicalGr);
	WriteConfigInt(_T("Golden_Gb")+suffix, pDlg->m_TypicalGb);
	WriteConfigInt(_T("Golden_B")+suffix, pDlg->m_TypicalB);

	//Typical Gain
	WriteConfigInt(_T("Golden_RG")+suffix, pDlg->m_TypicalRG);
	WriteConfigInt(_T("Golden_BG")+suffix, pDlg->m_TypicalBG);
	WriteConfigInt(_T("Golden_GG")+suffix, pDlg->m_TypicalGG);
	WriteConfigInt(_T("Raw8_EN"), pDlg->m_Raw8_EN);
	WriteConfigInt(_T("distance_check")+suffix, pDlg->m_distance);
	WriteConfigInt(_T("before_distance_check")+suffix, pDlg->m_before_distance);
	WriteConfigInt(_T("GG_GainEn"), pDlg->m_GG_GainEn);
	WriteConfigInt(_T("page")+suffix, pDlg->page);
	SetName(CurrentName);

	WriteConfigInt(_T("IsGoldenForCopy"), pDlg->m_IsGoldenForCopy);
	WriteConfigInt(_T("GoldenCopy_ErrH"), pDlg->m_GoldenCopy_ErrH);
	WriteConfigInt(_T("GoldenCopy_ErrL"), pDlg->m_GoldenCopy_ErrL);

	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR WBCheck_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");

	header += _T("APPLY_R,APPLY_Gr,APPLY_Gb,APPLY_B,");

	if(pDlg->GainEnable)
	{
		header += _T("APPLY_RGain,APPLY_BGain,APPLY_GGain,");
		header += _T("APPLY_RGain_fin,APPLY_BGain_fin,APPLY_GGain_fin,Typical_RG,Typical_BG,Typical_GG,RGIndex,BGIndex,GGIndex,");
	}

	if(pDlg->SingleChannelEnable)
	{
		header += _T("APPLY_R_fin,APPLY_Gr_fin,APPLY_Gb_fin,APPLY_B_fin,");
		header += _T("APPLY_RGain_fin,APPLY_BGain_fin,APPLY_GGain_fin,Typical_R,Typical_Gr,Typical_Gb,Typical_B,RIndex,GrIndex,GbIndex,BIndex,");
	}
	header += _T("AfterDistance,");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR WBCheck_TestItem::GetReportLowLimit()
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
LPCTSTR WBCheck_TestItem::GetReportHightLimit()
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
LPCTSTR WBCheck_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	CString temp;
	temp.Format(_T("%d,%d,%d,%d,"),AfterR,AfterGr,AfterGb,AfterB);
	Content+=temp;

	if(pDlg->GainEnable)
	{
		temp.Format(_T("%d,%d,%d,"),AfterRG,AfterBG,AfterGG);
		Content+=temp;
		temp.Format(_T("%d,%d,%d,"),AfterRG_fin,AfterBG_fin,AfterGG_fin);
		Content+=temp;
		temp.Format(_T("%d,%d,%d,%d,%d,%d,"),pDlg->m_TypicalRG,pDlg->m_TypicalBG,pDlg->m_TypicalGG,RG_index, BG_index,GG_index);
		Content+=temp;
	}

	if(pDlg->SingleChannelEnable)
	{
		temp.Format(_T("%d,%d,%d,%d,"),AfterR_fin,AfterGr_fin,AfterGb_fin,AfterB_fin);
		Content+=temp;
		temp.Format(_T("%d,%d,%d,"),AfterRG_fin,AfterBG_fin,AfterGG_fin);
		Content+=temp;
		temp.Format(_T("%d,%d,%d,%d,%d,%d,%d,%d,"),pDlg->m_TypicalR,pDlg->m_TypicalGr,pDlg->m_TypicalGb,pDlg->m_TypicalB,R_index,Gr_index,Gb_index,B_index);
		Content+=temp;
	}
	temp.Format(_T("%.2f"),Afterdistance);
	Content+=temp;
	return Content;
}
int WBCheck_TestItem::BeforeAWB()
{
	USHORT OTP_R=0;
	USHORT OTP_Gr=0;
	USHORT OTP_Gb=0;
	USHORT OTP_B=0;
	USHORT OTP_RG=0;
	USHORT OTP_BG=0;
	USHORT OTP_GG=0;
	Typical_RG=0;
	Typical_BG=0;
	Typical_GG=0;

	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}

	if (ByteCount==1)
	{
		if (pDlg->SingleChannelEnable)
		{
			m_pInterface->Otp_OtpRead(R_StartAdd,R_StartAdd,&OTP_R,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(Gr_StartAdd,Gr_StartAdd,&OTP_Gr,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(Gb_StartAdd,Gb_StartAdd,&OTP_Gb,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(B_StartAdd,B_StartAdd,&OTP_B,CamID,pDlg->page);

			if (GAverFlag == 1)
			{
				OTP_RG = 2.0*pDlg->m_Multiple*OTP_R / (OTP_Gr+OTP_Gb)+AA;
				OTP_BG = 2.0*pDlg->m_Multiple*OTP_B / (OTP_Gr+OTP_Gb)+AA;
				OTP_GG = 1.0*pDlg->m_Multiple*OTP_Gr / OTP_Gb+AA;

				Typical_RG = 2.0*pDlg->m_Multiple*pDlg->m_TypicalR/(pDlg->m_TypicalGr+pDlg->m_TypicalGb)+AA;
				Typical_BG = 2.0*pDlg->m_Multiple*pDlg->m_TypicalB/(pDlg->m_TypicalGr+pDlg->m_TypicalGb)+AA;
				Typical_GG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalGr/pDlg->m_TypicalGb + AA;// Gr/Gb 20180516
			}
			else if (GAverFlag==2)
			{
				OTP_RG = 1.0*pDlg->m_Multiple*OTP_R / OTP_Gr+AA;
				OTP_BG = 1.0*pDlg->m_Multiple*OTP_B / OTP_Gr+AA;
				OTP_GG = 1.0*pDlg->m_Multiple*OTP_Gb / OTP_Gr+AA;

				Typical_RG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalR/pDlg->m_TypicalGr+AA;
				Typical_BG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalB/pDlg->m_TypicalGr+AA;
				Typical_GG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalGb/pDlg->m_TypicalGr+AA;
			}
			else if (GAverFlag==3)
			{
				OTP_RG = 1.0*pDlg->m_Multiple*OTP_R / OTP_Gb+AA;
				OTP_BG = 1.0*pDlg->m_Multiple*OTP_B / OTP_Gb+AA;
				OTP_GG = 1.0*pDlg->m_Multiple*OTP_Gr / OTP_Gb+AA;

				Typical_RG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalR/pDlg->m_TypicalGb+AA;
				Typical_BG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalB/pDlg->m_TypicalGb+AA;
				Typical_GG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalGr/pDlg->m_TypicalGb+AA;
			}
		}
		else if (pDlg->GainEnable)
		{
			m_pInterface->Otp_OtpRead(RG_StartAdd,RG_StartAdd,&OTP_RG,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(BG_StartAdd,BG_StartAdd,&OTP_BG,CamID,pDlg->page);
			m_pInterface->Otp_OtpRead(GG_StartAdd,GG_StartAdd,&OTP_GG,CamID,pDlg->page);

			Typical_RG=pDlg->m_TypicalRG;
			Typical_BG=pDlg->m_TypicalBG;
			Typical_GG=pDlg->m_TypicalGG;
		}
	}
	else if (ByteCount==2)
	{
		if (pDlg->SingleChannelEnable)
		{
			USHORT OTPData[2]={0};
			m_pInterface->Otp_OtpRead(R_StartAdd,R_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_R=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(Gr_StartAdd,Gr_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_Gr=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(Gb_StartAdd,Gb_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_Gb=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(B_StartAdd,B_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_B=OTPData[0]*A_Multi+OTPData[1]*B_Multi;

			if (GAverFlag == 1)
			{
				OTP_RG = 2.0*pDlg->m_Multiple*OTP_R / (OTP_Gr+OTP_Gb)+AA;
				OTP_BG = 2.0*pDlg->m_Multiple*OTP_B / (OTP_Gr+OTP_Gb)+AA;
				OTP_GG = 1.0*pDlg->m_Multiple*OTP_Gr / OTP_Gb+AA;

				Typical_RG = 2.0*pDlg->m_Multiple*pDlg->m_TypicalR/(pDlg->m_TypicalGr+pDlg->m_TypicalGb)+AA;
				Typical_BG = 2.0*pDlg->m_Multiple*pDlg->m_TypicalB/(pDlg->m_TypicalGr+pDlg->m_TypicalGb)+AA;
				Typical_GG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalGb/pDlg->m_TypicalGr + AA;
			}
			else if (GAverFlag==2)
			{
				OTP_RG = 1.0*pDlg->m_Multiple*OTP_R / OTP_Gr+AA;
				OTP_BG = 1.0*pDlg->m_Multiple*OTP_B / OTP_Gr+AA;
				OTP_GG = 1.0*pDlg->m_Multiple*OTP_Gb / OTP_Gr+AA;

				Typical_RG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalR/pDlg->m_TypicalGr+AA;
				Typical_BG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalB/pDlg->m_TypicalGr+AA;
				Typical_GG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalGb/pDlg->m_TypicalGr+AA;
			}
			else if (GAverFlag==3)
			{
				OTP_RG = 1.0*pDlg->m_Multiple*OTP_R / OTP_Gb+AA;
				OTP_BG = 1.0*pDlg->m_Multiple*OTP_B / OTP_Gb+AA;
				OTP_GG = 1.0*pDlg->m_Multiple*OTP_Gr / OTP_Gb+AA;

				Typical_RG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalR/pDlg->m_TypicalGb+AA;
				Typical_BG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalB/pDlg->m_TypicalGb+AA;
				Typical_GG = 1.0*pDlg->m_Multiple*pDlg->m_TypicalGr/pDlg->m_TypicalGb+AA;
			}
		}
		if (pDlg->GainEnable)
		{
			USHORT OTPData[2]={0};
			m_pInterface->Otp_OtpRead(RG_StartAdd,RG_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_RG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;
			m_pInterface->Otp_OtpRead(BG_StartAdd,BG_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_BG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;

			m_pInterface->Otp_OtpRead(GG_StartAdd,GG_StartAdd+1,OTPData,CamID,pDlg->page);
			OTP_GG=OTPData[0]*A_Multi+OTPData[1]*B_Multi;

			if ((OTP_RG<=0)||(OTP_BG<=0))
			{
				CString strTemp;
				strTemp.Format(_T("RG,BG异常，不能为0，RG=%d BG=%d!"),OTP_RG,OTP_BG);
				m_pInterface->Ctrl_Addlog(CamID,strTemp,COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,strTemp);
				SetResult(RESULT_FAIL);
				return 0;
			}

			Typical_RG=pDlg->m_TypicalRG;
			Typical_BG=pDlg->m_TypicalBG;
			Typical_GG=pDlg->m_TypicalGG;
		}
	}
	Typical_RG=pDlg->m_TypicalRG;//所有计算Distance使用的Golden值为点亮设定读取,不用Golden_R计算 added LYC 20180521
	Typical_BG=pDlg->m_TypicalBG;
	Typical_GG=pDlg->m_TypicalGG;
	CString temp;
	BOOL WBResult=TRUE;
	if (pDlg->SingleChannelEnable)
	{
		temp.Format(_T("当前模组 OTP_R=%d"),OTP_R);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_Gr=%d"),OTP_Gr);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_Gb=%d"),OTP_Gb);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_B=%d"),OTP_B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		if (OTP_R>pDlg->R_max||OTP_R<pDlg->R_min) WBResult=FALSE;
		if (OTP_Gr>pDlg->Gr_max||OTP_Gr<pDlg->Gr_min) WBResult=FALSE;
		if (OTP_Gb>pDlg->Gb_max||OTP_Gb<pDlg->Gb_min) WBResult=FALSE;
		if (OTP_B>pDlg->B_max||OTP_B<pDlg->B_min) WBResult=FALSE;
		if (WBResult!=TRUE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("单通道R Gr Gb B超规格!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("单通道R Gr Gb B超规格!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	if (pDlg->GainEnable)
	{
		temp.Format(_T("当前模组 OTP_RG=%d"),OTP_RG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_BG=%d"),OTP_BG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("当前模组 OTP_GG=%d"),OTP_GG); 
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		if (OTP_RG>pDlg->RG_max||OTP_RG<pDlg->RG_min) WBResult=FALSE;
		if (OTP_BG>pDlg->BG_max||OTP_BG<pDlg->BG_min) WBResult=FALSE;
		if (OTP_GG>pDlg->GG_max||OTP_GG<pDlg->GG_min) WBResult=FALSE;
		if (WBResult!=TRUE)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("Gain值超规格!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Gain值超规格!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	ImageInfo *MyImageInfo = NULL;		
	MyImageInfo = m_pInterface->Ctrl_GetImgInfo(CamID);
	if (MyImageInfo->Raw_buffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("grab image fail!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("grab image fail!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
	WBInf MyWBInf;
	memset(&MyWBInf,0,sizeof(WBInf));
	if (pDlg->m_QTITool)
	{
		if(!GetAWBData(MyImageInfo[0],MyWBInf))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("获取AWB失败!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	else
	{
		CenterBlockRawInfo(MyImageInfo, pDlg->m_ROI_H, MyWBInf, pDlg->m_Multiple, pDlg->m_BLC);
	}

	temp.Format(_T("Typical RG=%d"),Typical_RG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

	temp.Format(_T("Typical BG=%d"),Typical_BG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

	temp.Format(_T("补偿前影像 R=%d"),MyWBInf.R - pDlg->m_BLC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 Gr=%d"),MyWBInf.Gr - pDlg->m_BLC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 Gb=%d"),MyWBInf.Gb - pDlg->m_BLC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 B=%d"),MyWBInf.B - pDlg->m_BLC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 RG=%d"),MyWBInf.RG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 BG=%d"),MyWBInf.BG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿前影像 GG=%d"),MyWBInf.GG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

	if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
	{
		/** Moto动态点检:单通道值是不减BLC的，直接存储原始值的四舍五入值--Added by LiHai--20180507 **/
		MyWBInf.R = MyWBInf.R;
		MyWBInf.Gr = MyWBInf.Gr;
		MyWBInf.Gb = MyWBInf.Gb;
		MyWBInf.B = MyWBInf.B;
		temp.Format(_T("BeforeR_fin=%d"),MyWBInf.R);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("BeforeGr_fin=%d"),MyWBInf.Gr);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("BeforeGb_fin=%d"),MyWBInf.Gb);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("BeforeB_fin=%d"),MyWBInf.B);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		/** Moto动态点检:RG、BG的值是用单通道值先减BLC然后按16384的公式计算，但是GG是不减BLC然后按16384的公式计算--Added by LiHai--20180507 **/
		MyWBInf.RG = MyWBInf.RG;
		MyWBInf.BG = MyWBInf.BG;
		MyWBInf.GG = MyWBInf.GG;
		temp.Format(_T("BeforeRG_fin=%d"),MyWBInf.RG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("BeforeBG_fin=%d"),MyWBInf.BG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("BeforeGG_fin=%d"),MyWBInf.GG);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	else
	{
		if (pDlg->SingleChannelEnable)
		{
			MyWBInf.R = int((MyWBInf.R - pDlg->m_BLC) * R_index / 10000.0 + 0.5);
			MyWBInf.Gr = int((MyWBInf.Gr - pDlg->m_BLC) * Gr_index / 10000.0 + 0.5);
			MyWBInf.Gb = int((MyWBInf.Gb - pDlg->m_BLC) * Gb_index / 10000.0 + 0.5);
			MyWBInf.B = int((MyWBInf.B - pDlg->m_BLC) * B_index / 10000.0 + 0.5);
			temp.Format(_T("x系数 R=%d index=%d"),MyWBInf.R, R_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 Gr=%d index=%d"),MyWBInf.Gr, Gr_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 Gb=%d index=%d"),MyWBInf.Gb, Gb_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 B=%d index=%d"),MyWBInf.B, B_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

			if (GAverFlag == 1)
			{
				MyWBInf.RG = int( 2.0 * pDlg->m_Multiple * (MyWBInf.R) / ( MyWBInf.Gr + MyWBInf.Gb) + 0.5 );// Rave/Gave
				MyWBInf.BG = int( 2.0 * pDlg->m_Multiple * (MyWBInf.B) / ( MyWBInf.Gr + MyWBInf.Gb) + 0.5 );  //Bave/Gave 
				MyWBInf.GG = int( 1.0 * pDlg->m_Multiple * (MyWBInf.Gr) / ( MyWBInf.Gb) + 0.5 );
			}
			else if (GAverFlag == 2)
			{
				MyWBInf.RG = int( 1.0 * pDlg->m_Multiple * (MyWBInf.R) / ( MyWBInf.Gr) + 0.5 );// Rave/Gave
				MyWBInf.BG = int( 1.0 * pDlg->m_Multiple * (MyWBInf.B) / ( MyWBInf.Gr) + 0.5 );  //Bave/Gave 
				MyWBInf.GG = int( 1.0 * pDlg->m_Multiple * (MyWBInf.Gb) / ( MyWBInf.Gr) + 0.5 );
			}
			else if (GAverFlag == 3)
			{
				MyWBInf.RG = int( 1.0 * pDlg->m_Multiple * (MyWBInf.R) / ( MyWBInf.Gb) + 0.5 );// Rave/Gave
				MyWBInf.BG = int( 1.0 * pDlg->m_Multiple * (MyWBInf.B) / ( MyWBInf.Gb) + 0.5 );  //Bave/Gave 
				MyWBInf.GG = int( 1.0 * pDlg->m_Multiple * (MyWBInf.Gr) / ( MyWBInf.Gb) + 0.5 );
			}

			temp.Format(_T("RG=%d"),MyWBInf.RG);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("BG=%d"),MyWBInf.BG);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("GG=%d"),MyWBInf.GG);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		}

		if (pDlg->GainEnable)
		{
			MyWBInf.RG = int(MyWBInf.RG * RG_index / 10000.0 + 0.5);
			MyWBInf.BG = int(MyWBInf.BG * BG_index / 10000.0 + 0.5);
			MyWBInf.GG = int(MyWBInf.GG * GG_index / 10000.0 + 0.5);

			temp.Format(_T("x系数 RG=%d index=%d"),MyWBInf.RG,RG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 BG=%d index=%d"),MyWBInf.BG,BG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 GG=%d index=%d"),MyWBInf.GG,GG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		}
	}

	//m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	float distance=0;
	if (pDlg->m_GG_GainEn)
	{
		distance=(float)sqrt(pow((MyWBInf.RG-Typical_RG)*1.0/Typical_RG,2) + pow((MyWBInf.BG-Typical_BG)*1.0/Typical_BG,2)+ pow((MyWBInf.GG-Typical_GG)*1.0/Typical_GG,2));
	}
	else
	{
		distance=(float)sqrt(pow((MyWBInf.RG-Typical_RG)*1.0/Typical_RG,2) + pow((MyWBInf.BG-Typical_BG)*1.0/Typical_BG,2));
	}
	temp.Format(_T("补偿前Distance=%.2f"),distance*100);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	//m_pInterface->Ctrl_Addlog(CamID, _T("WBCheck_TestItem::BeforeAWB()---111---Ctrl_PauseGrabFrame()\r\n"), COLOR_RED, 200);
	m_pInterface->Ctrl_PauseGrabFrame(CamID);
	Sleep(500);
	//m_pInterface->Ctrl_Addlog(CamID, _T("WBCheck_TestItem::BeforeAWB()---222---Ctrl_ResumeGrabFrame()\r\n"), COLOR_RED, 200);
	m_pInterface->Sensor_ApplyAWBGain(CamID, OTP_RG, OTP_BG, Typical_RG, Typical_BG);
	m_pInterface->Ctrl_ResumeGrabFrame(CamID);
	Sleep(500);
	return 1;
}
int WBCheck_TestItem::AfterAWB()
{
	ImageInfo *MyImageInfo = NULL;		
	MyImageInfo = m_pInterface->Ctrl_GetImgInfo(CamID);
	if (MyImageInfo->Raw_buffer == NULL)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("grab image fail!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("grab image fail!"));
		SetResult(RESULT_FAIL);
		return -1;
	}
	WBInf MyWBInf;
	memset(&MyWBInf,0,sizeof(WBInf));
	CString temp;
	if (pDlg->m_QTITool)
	{
		if(!GetAWBData(MyImageInfo[0],MyWBInf))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("获取AWB失败!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("获取AWB失败!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
	}
	else
	{
		CenterBlockRawInfo(MyImageInfo, pDlg->m_ROI_H, MyWBInf, pDlg->m_Multiple, pDlg->m_BLC);
	}
	temp.Format(_T("补偿后影像 R=%d"), MyWBInf.R - pDlg->m_BLC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 Gr=%d"), MyWBInf.Gr - pDlg->m_BLC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 Gb=%d"), MyWBInf.Gb - pDlg->m_BLC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 B=%d"), MyWBInf.B - pDlg->m_BLC);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 RG=%d"), MyWBInf.RG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 BG=%d"), MyWBInf.BG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	temp.Format(_T("补偿后影像 GG=%d"), MyWBInf.GG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

	AfterR=MyWBInf.R;
	AfterGr=MyWBInf.Gr;
	AfterGb=MyWBInf.Gb;
	AfterB=MyWBInf.B;
	AfterRG=MyWBInf.RG;
	AfterBG=MyWBInf.BG;
	AfterGG=MyWBInf.GG;

	if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
	{
		/** Moto动态点检:单通道值是不减BLC的，直接存储原始值的四舍五入值--Added by LiHai--20180507 **/
		AfterR_fin = MyWBInf.R;
		AfterGr_fin = MyWBInf.Gr;
		AfterGb_fin = MyWBInf.Gb;
		AfterB_fin = MyWBInf.B;
		temp.Format(_T("AfterR_fin=%d"),AfterR_fin);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("AfterGr_fin=%d"),AfterGr_fin);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("AfterGb_fin=%d"),AfterGb_fin);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("AfterB_fin=%d"),AfterB_fin);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		/** Moto动态点检:RG、BG的值是用单通道值先减BLC然后按16384的公式计算，但是GG是不减BLC然后按16384的公式计算--Added by LiHai--20180507 **/
		AfterRG_fin = MyWBInf.RG;
		AfterBG_fin = MyWBInf.BG;
		AfterGG_fin = MyWBInf.GG;

		temp.Format(_T("AfterRG_fin=%d"),AfterRG_fin);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("AfterBG_fin=%d"),AfterBG_fin);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		temp.Format(_T("AfterGG_fin=%d"),AfterGG_fin);
		m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	}
	else
	{
		if (pDlg->SingleChannelEnable)
		{
			AfterR_fin = int((MyWBInf.R - pDlg->m_BLC) * R_index / 10000.0 + 0.5);
			AfterGr_fin = int((MyWBInf.Gr - pDlg->m_BLC) * Gr_index / 10000.0 + 0.5);
			AfterGb_fin = int((MyWBInf.Gb - pDlg->m_BLC) * Gb_index / 10000.0 + 0.5);
			AfterB_fin = int((MyWBInf.B - pDlg->m_BLC) * B_index / 10000.0 + 0.5);
			temp.Format(_T("x系数 AfterR_fin=%d index=%d"), AfterR_fin, R_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 AfterGr_fin=%d index=%d"), AfterGr_fin, Gr_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 AfterGb_fin=%d index=%d"), AfterGb_fin, Gb_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 AfterB_fin=%d index=%d"), AfterB_fin, B_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

			if (GAverFlag == 1)
			{
				AfterRG_fin = int( 2.0 * pDlg->m_Multiple * (AfterR_fin) / ( AfterGr_fin + AfterGb_fin) + 0.5 );// Rave/Gave
				AfterBG_fin = int( 2.0 * pDlg->m_Multiple * (AfterB_fin) / ( AfterGr_fin + AfterGb_fin) + 0.5 );  //Bave/Gave 
				AfterGG_fin = int( 1.0 * pDlg->m_Multiple * (AfterGr_fin) / ( AfterGb_fin) + 0.5 );
			}
			else if (GAverFlag == 2)
			{
				AfterRG_fin = int( 1.0 * pDlg->m_Multiple * (AfterR_fin) / ( AfterGr_fin) + 0.5 );// Rave/Gave
				AfterBG_fin = int( 1.0 * pDlg->m_Multiple * (AfterB_fin) / ( AfterGr_fin) + 0.5 );  //Bave/Gave 
				AfterGG_fin = int( 1.0 * pDlg->m_Multiple * (AfterGb_fin) / ( AfterGr_fin) + 0.5 );
			}
			else if (GAverFlag == 3)
			{
				AfterRG_fin = int( 1.0 * pDlg->m_Multiple * (AfterR_fin) / ( AfterGb_fin) + 0.5 );// Rave/Gave
				AfterBG_fin = int( 1.0 * pDlg->m_Multiple * (AfterB_fin) / ( AfterGb_fin) + 0.5 );  //Bave/Gave 
				AfterGG_fin = int( 1.0 * pDlg->m_Multiple * (AfterGr_fin) / ( AfterGb_fin) + 0.5 );
			}

			temp.Format(_T("AfterRG_fin=%d"),AfterRG_fin);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("AfterBG_fin=%d"),AfterBG_fin);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		}

		if (pDlg->GainEnable)
		{
			AfterRG_fin = int(MyWBInf.RG * RG_index / 10000.0 + 0.5);
			AfterBG_fin = int(MyWBInf.BG * BG_index / 10000.0 + 0.5);
			AfterGG_fin = int(MyWBInf.GG * GG_index / 10000.0 + 0.5);

			temp.Format(_T("x系数 RG=%d index=%d") ,AfterRG_fin, RG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 BG=%d index=%d"), AfterBG_fin, BG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
			temp.Format(_T("x系数 GG=%d index=%d"), AfterGG_fin, GG_index);
			m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
		}
	}

	temp.Format(_T("Typical_RG=%d  Typical_BG=%d"),Typical_RG,Typical_BG);
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	float distance=0;
	if (pDlg->m_GG_GainEn)
	{
		distance=(float)sqrt(pow((AfterRG_fin-Typical_RG)*1.0/Typical_RG,2) + pow((AfterBG_fin-Typical_BG)*1.0/Typical_BG,2)+ pow((AfterGG-Typical_GG)*1.0/Typical_GG,2));
	}
	else
	{
	    distance=(float)sqrt(pow((AfterRG_fin-Typical_RG)*1.0/Typical_RG,2) + pow((AfterBG_fin-Typical_BG)*1.0/Typical_BG,2));
	}
	temp.Format(_T("补偿后Distance=%.2f"),distance*100);
	Afterdistance=distance*100;
	m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);
	if (distance*1000>pDlg->m_distance)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Distance 超范围!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Distance 超范围!"));
		SetResult(RESULT_FAIL);
		return -1;
	}
	return 0;
}
void WBCheck_TestItem::CenterBlockRawInfo(ImageInfo *mySensor, int Ratio, WBInf &MyWbInf, int Multiple, int BLC)
{
	USHORT *pIn = new USHORT[mySensor->imgwidth*mySensor->imgheight];
	if (NULL == pIn)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("pIn buffer 申请失败!"),COLOR_RED,200);
		return;
	}
	memset(pIn,0,mySensor->imgwidth*mySensor->imgheight*sizeof(USHORT));
	if (pDlg->m_Raw8_EN)
	{
		for (int k=0;k<mySensor->imgwidth*mySensor->imgheight;k++)
		{
			pIn[k]=(mySensor->Raw_buffer[k]>>2);
		}
	}
	else
	{
		memcpy(pIn,mySensor->Raw_buffer,mySensor->imgwidth*mySensor->imgheight*sizeof(USHORT));
	}
	int  PixW = mySensor->imgwidth;
	int  PixH = mySensor->imgheight;
	//calculate rawWidth & rawHeight  , size of ROI 
	int rawWidth  = PixW / pDlg->m_ROI_H/*Ratio*/;    //multiply two is to make sure that width & height is even  because of bayer order
	int rawHeight = PixH / pDlg->m_ROI_V/*Ratio*/;

	//联想手机曝光和计算AWB数据都是取的【中间128*128像素区域】来计算的--Added by LiHai--20171213
	if ((pDlg->m_ROI_H >= 100) && (pDlg->m_ROI_V >= 100))
	{
		rawWidth = pDlg->m_ROI_H;
		rawHeight = pDlg->m_ROI_V;
	}

	int  StartX = (PixW  - rawWidth ) / 4 * 2;
	int  StartY = (PixH  - rawHeight) / 4 * 2;

	int  EndX   = StartX + rawWidth;
	int  EndY   = StartY + rawHeight;

	BYTE outformat=mySensor->RawFormat;

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

	if ( outformat== 0)//RGGB
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumR  += pIn[Pos1];
				SumGr += pIn[Pos1 + 1];
				SumGb += pIn[Pos2];
				SumB  += pIn[Pos2 + 1];
				Count++;
			}
		}
	}

	if (outformat == 1)//GRBG
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumGr += pIn[Pos1];
				SumR  += pIn[Pos1 + 1];
				SumB  += pIn[Pos2];
				SumGb += pIn[Pos2 + 1]; 

				Count++;
			}
		}
	}

	if (outformat == 2)//GBRG
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1); 

				SumGb += pIn[Pos1];
				SumB  += pIn[Pos1 + 1];
				SumR  += pIn[Pos2];
				SumGr += pIn[Pos2 + 1];

				Count++;
			}
		}
	}

	if (outformat == 3)//BGGR
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + PixW * y;
				Pos2 = x + PixW * (y + 1);

				SumB  += pIn[Pos1];
				SumGb += pIn[Pos1 + 1];
				SumGr += pIn[Pos2];
				SumR  += pIn[Pos2 + 1];

				Count++;
			}
		}
	}

	SAFE_DELETE_ARRAY(pIn);

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;
	
	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	int Unit_Gr = int(Gr + AA);// added  + AA LYC 20180315
	int Unit_Gb = int(Gb + AA);
	int Unit_R  = int(R + AA);
	int Unit_B  = int(B + AA);

	MyWbInf.R = Unit_R;
	MyWbInf.Gr = Unit_Gr;
	MyWbInf.Gb = Unit_Gb;
	MyWbInf.B = Unit_B;

	if(GAverFlag==1)
	{
		MyWbInf.RG = int( (float)Multiple*2.0*(Unit_R - BLC)/(float)((Unit_Gr + Unit_Gb - 2*BLC)) + AA );// Rave/Gave
		MyWbInf.BG = int( (float)Multiple*2.0*(Unit_B - BLC)/(float)((Unit_Gr + Unit_Gb - 2*BLC)) + AA );  //Bave/Gave 
		if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
		{
			MyWbInf.GG = int( (float)Multiple*1.0*(Unit_Gr)/(float)((Unit_Gb)) + AA );
		} 
		else
		{
			MyWbInf.GG = int( (float)Multiple*1.0*(Unit_Gr - BLC)/(float)((Unit_Gb - BLC)) + AA );
		}
	}
	else if (GAverFlag==2)
	{
		MyWbInf.RG = int( (float)Multiple*(Unit_R - BLC)/(float)(Unit_Gr - BLC) + AA );// Rave/Gave
		MyWbInf.BG = int( (float)Multiple*(Unit_B - BLC)/(float)(Unit_Gr - BLC) + AA );  //Bave/Gave 
		if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
		{
			MyWbInf.GG = int( (float)Multiple*1.0*(Unit_Gr)/(float)((Unit_Gb)) + AA );
		} 
		else
		{
			MyWbInf.GG = int( (float)Multiple*1.0*(Unit_Gr - BLC)/(float)((Unit_Gb - BLC)) + AA );
		}
	}
	else if (GAverFlag==3)
	{
		MyWbInf.RG = int( (float)Multiple*(Unit_R - BLC)/(float)(Unit_Gb - BLC) + AA );// Rave/Gave
		MyWbInf.BG = int( (float)Multiple*(Unit_B - BLC)/(float)(Unit_Gb - BLC) + AA );  //Bave/Gave 
		if (MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC == m_MotoLightSrcMode)
		{
			MyWbInf.GG = int( (float)Multiple*1.0*(Unit_Gr)/(float)((Unit_Gb)) + AA );
		} 
		else
		{
			MyWbInf.GG = int( (float)Multiple*1.0*(Unit_Gr - BLC)/(float)((Unit_Gb - BLC)) + AA );
		}
	}
}
void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}

#define LSC_PATH_DEFINE								0
BOOL WBCheck_TestItem::GetAWBData(ImageInfo img,WBInf &MyWbInf)
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
	SetCurrentDirectory(ToolPath);

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
	int CFA_Pattern=0;
	if ((img.RawFormat == 0) ||
		(img.RawFormat == 1))
	{
		CFA_Pattern =img.RawFormat;
	}
	if (img.RawFormat == 2)
	{
		CFA_Pattern = 3;
	}
	if (img.RawFormat == 3)
	{
		CFA_Pattern = 2;
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
	MyWbInf.Gr = int(Gr);
	MyWbInf.Gb = int(Gb);
	MyWbInf.R  = int(R);
	MyWbInf.B  = int(B);
	int index=pDlg->m_Multiple;
	float AA=0.0;
	if (pDlg->m_flow)
	{
		AA=0.5;
	}
	if(GAverFlag==1)
	{
		MyWbInf.RG = int( (float)index*2.0*(MyWbInf.R)/(float)((MyWbInf.Gr + MyWbInf.Gb)) + AA );// Rave/Gave
		MyWbInf.BG = int( (float)index*2.0*(MyWbInf.B)/(float)((MyWbInf.Gr + MyWbInf.Gb)) + AA );  //Bave/Gave 
		MyWbInf.GG = int( (float)index*2.0*(MyWbInf.Gb)/(float)((MyWbInf.Gr+ MyWbInf.Gb)) + AA );
	}
	else if (GAverFlag==2)
	{
		MyWbInf.RG = int( (float)index*(MyWbInf.R)/(float)(MyWbInf.Gr) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)index*(MyWbInf.B)/(float)(MyWbInf.Gr) + AA );  //Bave/Gave 
		MyWbInf.GG =int( (float)index*(MyWbInf.Gb)/(float)(MyWbInf.Gr) + AA);
	}
	else if (GAverFlag==3)
	{
		MyWbInf.RG = int( (float)index*(MyWbInf.R)/(float)(MyWbInf.Gb) + AA  );// Rave/Gave
		MyWbInf.BG = int( (float)index*(MyWbInf.B)/(float)(MyWbInf.Gb) + AA ); //Bave/Gave 
		MyWbInf.GG =int( (float)index*(MyWbInf.Gr)/(float)(MyWbInf.Gb) + AA);
	}

	sfilename=ToolPath+_T("AWB_LSC_CALIBRATION_DATA.txt");
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("AWB_LSC_CALIBRATION_DATA.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt");
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_LSC_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}
	sfilename=ToolPath+_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt");
	if(PathFileExists(sfilename))
	{
		if(!DeleteFile(sfilename))
		{	
			m_pInterface->Ctrl_Addlog(CamID,_T("ONLY_AWB_CALIBRATION_DATA_BYTE_FORMAT.txt"),COLOR_RED,200);
			return FALSE;
		}
	}

	return TRUE;
}

