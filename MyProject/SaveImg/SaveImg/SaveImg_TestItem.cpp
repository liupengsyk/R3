#include "StdAfx.h"
#include "SaveImg_TestItem.h"
#include "ImageProc.h" 


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new SaveImg_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
SaveImg_TestItem::SaveImg_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

SaveImg_TestItem::~SaveImg_TestItem(void)
{

}


int SaveImg_TestItem::InitItem()
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
int SaveImg_TestItem::Initialize()
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
int SaveImg_TestItem::Testing()
{
   //TODO:在此添加测试项代码
 //  ImageInfo img = *(m_pInterface->Ctrl_GetImgInfo(CamID));
  // int width = img.imgwidth;
  // int height = img.imgheight;
   CString FileName = pDlg->m_strFileName;
   CString currentPath = m_pInterface->Ctrl_GetCurrentPath();
   CString fullImageSavePath = currentPath + FilePath + L"\\";
   int ImgTypeSel = 0;
    if(ImgType == _T(".jpg"))
	{
		ImgTypeSel = 1;
	}
	else if(ImgType == _T(".bmp"))
	{
		ImgTypeSel = 2;
	}
	else if(ImgType == _T(".raw"))
	{
		ImgTypeSel = 3;
	}
	else if(ImgType == _T(".raw8"))
	{
		ImgTypeSel = 4;
	}
	else if(ImgType == _T("HwHDC"))//HwHDC
	{
		ImgTypeSel = 5;
		 
		ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
		ImageProc::ZoomImgSize(img.RGB24_buffer,img.imgwidth,img.imgheight,1600,1200,fullImageSavePath + FileName,_T("jpg"));
		m_pInterface->Ctrl_Addlog(CamID,fullImageSavePath + FileName,COLOR_BLUE,200);
		SetResult(RESULT_PASS);
		return 0;
	}
	else if(ImgType == _T("HwMMI"))//HwMMI
	{
		ImgTypeSel = 6;
		 
		ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
		ImageProc::ZoomImgSize(img.RGB24_buffer,img.imgwidth,img.imgheight,1600,1200,fullImageSavePath + FileName,_T("bmp"));
		m_pInterface->Ctrl_Addlog(CamID,fullImageSavePath + FileName,COLOR_BLUE,200);
		SetResult(RESULT_PASS);
		return 0;
	} 
    m_pInterface->Ctrl_SaveImage( CamID, 1, fullImageSavePath, FileName, ImgTypeSel, NULL);  

   SetResult(RESULT_PASS);
   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int SaveImg_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int SaveImg_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	//UserInterface here.

	pDlg->m_strFileName = ReadConfigString(_T("FileName"),_T("L_20")); 
	//FilePath = ReadConfigString(_T("FilePath"),m_pInterface->Ctrl_GetCurrentPath()+_T("TestPath\\testRectify\\"));
	FilePath = ReadConfigString(_T("FilePath"),_T("ImgSave"));
	
	ImgType = ReadConfigString(_T("ImgType"),_T(".jpg"));

	//........................8
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int SaveImg_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   

	WriteConfigString(_T("FileName"),pDlg->m_strFileName);
	 
	WriteConfigString(_T("FilePath"),FilePath);
	WriteConfigString(_T("ImgType"),ImgType);

	
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR SaveImg_TestItem::GetReportHeads()
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
LPCTSTR SaveImg_TestItem::GetReportLowLimit()
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
LPCTSTR SaveImg_TestItem::GetReportHightLimit()
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
LPCTSTR SaveImg_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

