#include "StdAfx.h"
#include "SensorLSCApply_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new SensorLSCApply_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
SensorLSCApply_TestItem::SensorLSCApply_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

SensorLSCApply_TestItem::~SensorLSCApply_TestItem(void)
{

}


int SensorLSCApply_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SensorLSCApply_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
    LoadOption(); 
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SensorLSCApply_TestItem::Testing()
{
   //TODO:在此添加测试项代码

	if (GlobalTime < 12)
	{
		m_pInterface->Ctrl_PauseGrabFrame(CamID);
		if (_T("S5K5E8") == SensorName)
		{
			m_pInterface->Sensor_ApplyLsc(CamID,NULL,0,0,0,0,NULL,0,0);
		}
		else if (_T("S5K4H7YX") == SensorName)
		{
			m_pInterface->Sensor_ApplyLsc(CamID,NULL,0,0,0,0,NULL,0,0);
		}
		else if (_T("S5K4H8_F1X9") == SensorName)
		{
			m_pInterface->Sensor_ApplyLsc(CamID,NULL,0,0,0,0,NULL,0,0);
		}
		else
		{
			Sleep(pDlg->m_DelayTime);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			m_pInterface->Ctrl_Addlog(CamID,_T("Sensor没有添加，请添加!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Sensor没有添加，请添加!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
		Sleep(pDlg->m_DelayTime);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		m_pInterface->Ctrl_Addlog(CamID,_T("Apply LSC Pass!"),COLOR_BLUE,200);
	}
	
	SetResult(RESULT_PASS);
    return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SensorLSCApply_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int SensorLSCApply_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	CString CurrentName = GetName();
	SetName(_T("Sensor"));
	SensorName = ReadConfigString(_T("SensorName"),_T("S5K5E8"));

	SetName(_T("WBSetting"));	
	GlobalTime=ReadConfigInt(_T("GlobalTime"), 3);

	SetName(CurrentName);
	pDlg->m_DelayTime = ReadConfigInt(_T("DelayTime"),500);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int SensorLSCApply_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(_T("DelayTime"),pDlg->m_DelayTime);

	LoadOption();
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR SensorLSCApply_TestItem::GetReportHeads()
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
LPCTSTR SensorLSCApply_TestItem::GetReportLowLimit()
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
LPCTSTR SensorLSCApply_TestItem::GetReportHightLimit()
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
LPCTSTR SensorLSCApply_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

