#include "StdAfx.h"
#include "[!output PROJECT_NAME]_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new [!output PROJECT_NAME]_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
[!output PROJECT_NAME]_TestItem::[!output PROJECT_NAME]_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

[!output PROJECT_NAME]_TestItem::~[!output PROJECT_NAME]_TestItem(void)
{

}


int [!output PROJECT_NAME]_TestItem::InitItem()
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
int [!output PROJECT_NAME]_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
 
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int [!output PROJECT_NAME]_TestItem::Testing()
{
   //TODO:在此添加测试项代码


   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int [!output PROJECT_NAME]_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int [!output PROJECT_NAME]_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 



	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int [!output PROJECT_NAME]_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   



	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR [!output PROJECT_NAME]_TestItem::GetReportHeads()
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
LPCTSTR [!output PROJECT_NAME]_TestItem::GetReportLowLimit()
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
LPCTSTR [!output PROJECT_NAME]_TestItem::GetReportHightLimit()
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
LPCTSTR [!output PROJECT_NAME]_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

