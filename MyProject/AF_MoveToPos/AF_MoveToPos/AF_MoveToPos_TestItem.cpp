#include "StdAfx.h"
#include "AF_MoveToPos_TestItem.h"
#include "ImageProc.h"
#include "ccmBaseInterface.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new AF_MoveToPos_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
AF_MoveToPos_TestItem::AF_MoveToPos_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	//pDlg->m_MoveCode        = 0;
	m_AFMoveCode.bCheck = 0;
	m_AFMoveCode.MoveCode =  0;
	
	m_bInfinityCode.bCheck = 0;
	m_bMacroCode.bCheck = 0;

	ModuleEFL = 0.0;

	
	                                   //读取参数
	
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化


	//.....
}

AF_MoveToPos_TestItem::~AF_MoveToPos_TestItem(void)
{

}


int AF_MoveToPos_TestItem::InitItem()
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
int AF_MoveToPos_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
	CString TempStr;
	wchar_t temp[64]={0};
	wcscpy(temp,pDlg->m_ModuleEFL);
	ModuleEFL = _wtof(temp);

	TempStr.Format(_T("EFL = %f"),ModuleEFL);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	
	wcscpy(temp,pDlg->m_InfDistance);
	m_bInfinityCode.InfDistance = _wtof(temp);

	TempStr.Format(_T("InfDistance = %f"),m_bInfinityCode.InfDistance);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

	wcscpy(temp,pDlg->m_InfRegHigh);
	swscanf(temp,_T("%x"),&m_bInfinityCode.RegHigh);
	wcscpy(temp,pDlg->m_InfRegLow);
	swscanf(temp,_T("%x"),&m_bInfinityCode.RegLow);

	m_bInfinityCode.bCheck = pDlg->m_InfinityPosChecked;

	TempStr.Format(_T("InfRegHigh = 0x%04x"),m_bInfinityCode.RegHigh);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	TempStr.Format(_T("InfRegLow = 0x%04x"),m_bInfinityCode.RegLow);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

	USHORT RegHighVal;
	USHORT RegLowVal;
	m_pInterface->Otp_OtpRead(m_bInfinityCode.RegHigh,m_bInfinityCode.RegHigh,&RegHighVal,CamID,0);
	m_pInterface->Otp_OtpRead(m_bInfinityCode.RegLow,m_bInfinityCode.RegLow,&RegLowVal,CamID,0);
	m_bInfinityCode.InfinityCode = RegHighVal*256 + RegLowVal;

	TempStr.Format(L"InfinityCode = %d",m_bInfinityCode.InfinityCode);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

	wcscpy(temp,pDlg->m_MacroDistance);
	m_bMacroCode.MacroDistance = _wtof(temp);
	wcscpy(temp,pDlg->m_MacroRegHigh);
	swscanf(temp,_T("%x"),&m_bMacroCode.RegHigh);
	wcscpy(temp,pDlg->m_MacroRegLow);
	swscanf(temp,_T("%x"),&m_bMacroCode.RegLow);
	m_bMacroCode.bCheck = pDlg->m_MacroPosChecked;

	TempStr.Format(_T("MacroRegHigh = 0x%04x"),m_bMacroCode.RegHigh);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	TempStr.Format(_T("MacroRegLow = 0x%04x"),m_bMacroCode.RegLow);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

	m_pInterface->Otp_OtpRead(m_bMacroCode.RegHigh,m_bMacroCode.RegHigh,&RegHighVal,CamID,0);
	m_pInterface->Otp_OtpRead(m_bMacroCode.RegLow,m_bMacroCode.RegLow,&RegLowVal,CamID,0);
	m_bMacroCode.MacroCode = RegHighVal*256 + RegLowVal;

	TempStr.Format(_T("MacroDistance = %d"),m_bMacroCode.MacroDistance);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	TempStr.Format(L"MacroCode = %d",m_bMacroCode.MacroCode);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

	m_bMiddleCode.bCheck = pDlg->m_MiddlePosChecked;
	m_bMiddleCode.MidCode = (m_bInfinityCode.InfinityCode + m_bMacroCode.MacroCode)/2.0 + 0.5;

	TempStr.Format(L"MiddleCode = %d",m_bMiddleCode.MidCode);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

	m_AFMoveCode.bCheck = pDlg->m_MoveToPosChecked;
	wcscpy(temp,pDlg->m_MoveToPos);
	m_AFMoveCode.MoveDiatance = _wtof(temp);

	TempStr.Format(_T("MoveDis = %f"),m_AFMoveCode.MoveDiatance);
	m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

	if (m_AFMoveCode.bCheck)
	{
		double MovePosLensShift = pow(ModuleEFL,2)/(ModuleEFL - m_AFMoveCode.MoveDiatance*1000);
		double InfLensShift = pow(ModuleEFL,2)/(ModuleEFL - m_bInfinityCode.InfDistance*1000);
		double MacLensShift = pow(ModuleEFL,2)/(ModuleEFL - m_bMacroCode.MacroDistance*1000);

		TempStr.Format(_T("MoveLensShift = %f"),MovePosLensShift);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

		TempStr.Format(_T("InfLensShift = %f"),InfLensShift);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

		TempStr.Format(_T("MacLensShift = %f"),MacLensShift);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	//	double InfMacLensShift = InfLensShift - MacLensShift;
	//	double InfMacCodeDiff = m_bInfinityCode.InfinityCode - m_bMacroCode.MacroCode;
	//	double InfAFPosLensShift = InfLensShift - MovePosLensShift;
	//	m_AFMoveCode.MoveCode = m_bInfinityCode.InfinityCode - (InfMacCodeDiff / InfMacLensShift * InfAFPosLensShift);
		m_AFMoveCode.MoveCode = 1.0*(m_bInfinityCode.InfinityCode*(MovePosLensShift-MacLensShift)+m_bMacroCode.MacroCode*(InfLensShift-MovePosLensShift))/(InfLensShift-MacLensShift);
		TempStr.Format(L"MoveCode = %d",m_AFMoveCode.MoveCode);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	}

	if (pDlg->m_OtherCode_En)
	{
		TempStr.Format(_T("OtherCode = %d"),pDlg->m_OtherCode);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	}

	m_pInterface->VcmDr_InitAF(CamID);
	Sleep(500);

	CString strPeakReadfileName = m_pInterface->Ctrl_GetCurrentPath() + _T("PeakRead.ini");
	::DeleteFile(strPeakReadfileName);

	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AF_MoveToPos_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	CString Info;
	int Result=1;

	int Flag = m_AFMoveCode.bCheck + m_bInfinityCode.bCheck + m_bMacroCode.bCheck + m_bMiddleCode.bCheck + pDlg->m_OtherCode_En;
	
	if(Flag != 1)
	{
		Info.Format(_T("选择出错，请重新选择需要移动的位置!"));
		m_pInterface->Ctrl_Addlog(CamID,Info,COLOR_RED,220);
		SetResult(RESULT_FAIL);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("选择出错，请重新选择需要移动的位置!"));
		Result = 0;
	}
	else
	{
		CString fileName = m_pInterface->Ctrl_GetCurrentPath() + _T("PeakRead.ini");
		CString keyName = _T("PeakRead");
		


		if(m_AFMoveCode.bCheck)
		{
			m_pInterface->VcmDr_WriteAF_Code(m_AFMoveCode.MoveCode,CamID);
	
		}
		if(m_bInfinityCode.bCheck)
		{
			m_pInterface->VcmDr_WriteAF_Code(m_bInfinityCode.InfinityCode,CamID);
			CString strInfiniCode;
			strInfiniCode.Format(_T("%d"), m_bInfinityCode.InfinityCode);
			WritePrivateProfileString(keyName, _T("InfCode"), strInfiniCode, fileName);//获取已经有的CODE值
		}
		if(m_bMacroCode.bCheck)
		{
			m_pInterface->VcmDr_WriteAF_Code(m_bMacroCode.MacroCode,CamID);
			CString strMacroCode;
			strMacroCode.Format(_T("%d"), m_bMacroCode.MacroCode);
			WritePrivateProfileString(keyName, _T("MacroCode"), strMacroCode, fileName);
		}
		if(m_bMiddleCode.bCheck)
		{
			m_pInterface->VcmDr_WriteAF_Code(m_bMiddleCode.MidCode,CamID);

		}
		if (pDlg->m_OtherCode_En)
		{
			m_pInterface->VcmDr_WriteAF_Code(pDlg->m_OtherCode,CamID);
		}
		SetResult(RESULT_PASS);
	}
	Sleep(500);
   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AF_MoveToPos_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AF_MoveToPos_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
    pDlg->m_MoveToPosChecked = ReadConfigInt(_T("AF_MoveCodeCheck"), 0);
	pDlg->m_InfinityPosChecked = ReadConfigInt(_T("InfinityCodeCheck"), 0);
	pDlg->m_MacroPosChecked = ReadConfigInt(_T("MacroCodeCheck"), 0);
	pDlg->m_MiddlePosChecked = ReadConfigInt(_T("MiddleCodeCheck"),0);
	pDlg->m_OtherCode_En = ReadConfigInt(_T("OtherCodeCheck"),0);

	pDlg->m_InfDistance = ReadConfigString(_T("InfDistance"),_T("5"));
	pDlg->m_InfRegHigh = ReadConfigString(_T("InfRegHigh"),_T("0x0000"));
	pDlg->m_InfRegLow  = ReadConfigString(_T("InfRegLow"),_T("0x0000"));

	pDlg->m_MacroDistance = ReadConfigString(_T("MacroDistance"),_T("0.1"));
	pDlg->m_MacroRegHigh = ReadConfigString(_T("MacroRegHigh"),_T("0x0000"));
	pDlg->m_MacroRegLow  = ReadConfigString(_T("MacroRegLow"),_T("0x0000"));

	pDlg->m_MoveToPos = ReadConfigString(_T("MoveToPos"),_T("0.5"));
	pDlg->m_ModuleEFL = ReadConfigString(_T("ModuleEFL"),_T("0.35"));

	pDlg->m_OtherCode = ReadConfigInt(_T("OtherCode"),100);

	pDlg->m_SaveAFCode = ReadConfigInt(_T("SaveAFCode"),0);

	
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AF_MoveToPos_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("AF_MoveCodeCheck"),pDlg->m_MoveToPosChecked);
	WriteConfigInt(_T("InfinityCodeCheck"),pDlg->m_InfinityPosChecked);
	WriteConfigInt(_T("MacroCodeCheck"),pDlg->m_MacroPosChecked);
	WriteConfigInt(_T("MiddleCodeCheck"),pDlg->m_MiddlePosChecked);
	WriteConfigInt(_T("OtherCodeCheck"),pDlg->m_OtherCode_En);

	WriteConfigString(_T("InfDistance"),pDlg->m_InfDistance);
	WriteConfigString(_T("InfRegHigh"),pDlg->m_InfRegHigh);
	WriteConfigString(_T("InfRegLow"),pDlg->m_InfRegLow);
	WriteConfigString(_T("MacroDistance"),pDlg->m_MacroDistance);
	WriteConfigString(_T("MacroRegHigh"),pDlg->m_MacroRegHigh);
	WriteConfigString(_T("MacroRegLow"),pDlg->m_MacroRegLow);
	WriteConfigString(_T("MoveToPos"),pDlg->m_MoveToPos);
	WriteConfigString(_T("ModuleEFL"),pDlg->m_ModuleEFL);

	WriteConfigInt(_T("OtherCode"),pDlg->m_OtherCode);

	WriteConfigInt(_T("SaveAFCode"),pDlg->m_SaveAFCode);

	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_MoveToPos_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");

	if (pDlg->m_SaveAFCode)
	{
		header += _T("MacroCode,InfinityCode,");
	}
	
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR AF_MoveToPos_TestItem::GetReportLowLimit()
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
LPCTSTR AF_MoveToPos_TestItem::GetReportHightLimit()
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
LPCTSTR AF_MoveToPos_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	
	if (pDlg->m_SaveAFCode)
	{
		CString Str = _T("");
		Str.Format(_T("%d,"),m_bMacroCode.MacroCode);
		Content += Str;
		Str.Format(_T("%d,"),m_bInfinityCode.InfinityCode);
		Content += Str;
	}
	
	return Content;
}

