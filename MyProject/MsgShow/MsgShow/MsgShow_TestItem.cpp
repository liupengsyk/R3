#include "StdAfx.h"
#include "MsgShow_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new MsgShow_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
MsgShow_TestItem::MsgShow_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	
	
	bShowInfo =  1;
	InfoShow = _T("请输入需要显示的信息");
	InfoPos.x = 50;
	InfoPos.y =50;
	InfoSize  =50;
	InfoColor = _T("蓝色"); 

	bDelay = 1;
	DelayTime =1000;

	bShowMsgDlg =1;
	MsgInfo =  _T("请输入需要显示的信息");

	//设置测试项目的名字
	pDlg = new OptionDlg(NULL,this);                 //生成对话框对象
	                                   //读取参数
	pDlg->Create(OptionDlg::IDD,NULL);               //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//TODO: 其他类成员构造初始化


	//.....
}

MsgShow_TestItem::~MsgShow_TestItem(void)
{

}


int MsgShow_TestItem::InitItem()
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
int MsgShow_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:在此添加初始化代码
 
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int MsgShow_TestItem::Testing()
{
   //TODO:在此添加测试项代码

	ShowMTFinfo();

	SetResult(RESULT_PASS);
   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int MsgShow_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int MsgShow_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 

	bShowInfo = ReadConfigInt(_T("bShowInfo"),0);
	InfoShow = ReadConfigString(_T("InfoShow"),_T("请输入需要显示的信息"));
	InfoPos.x = ReadConfigInt(_T("InfoPosx"),50);
	InfoPos.y = ReadConfigInt(_T("InfoPosy"),50);
	InfoSize = ReadConfigInt(_T("InfoSize"),50);
	InfoColor = ReadConfigString(_T("InfoColor"),_T("蓝色")); 

	bDelay = ReadConfigInt(_T("bDelay"),1);
	DelayTime = ReadConfigInt(_T("DelayTime"),100);

	bShowMsgDlg = ReadConfigInt(_T("bShowMsgDlg"),0);
	MsgInfo =  ReadConfigString(_T("MsgInfo"),_T("请输入需要显示的信息"));

	 
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int MsgShow_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   

	WriteConfigInt(_T("bShowInfo"),bShowInfo);
	WriteConfigString(_T("InfoShow"),InfoShow);
	WriteConfigInt(_T("InfoPosx"),InfoPos.x);
	WriteConfigInt(_T("InfoPosy"),InfoPos.y);
	WriteConfigInt(_T("InfoSize"),InfoSize);
	WriteConfigString(_T("InfoColor"),InfoColor); 

	WriteConfigInt(_T("bDelay"),bDelay);
	WriteConfigInt(_T("DelayTime"),DelayTime);

	WriteConfigInt(_T("bShowMsgDlg"),bShowMsgDlg);
	WriteConfigString(_T("MsgInfo"),MsgInfo);


	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR MsgShow_TestItem::GetReportHeads()
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
LPCTSTR MsgShow_TestItem::GetReportLowLimit()
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
LPCTSTR MsgShow_TestItem::GetReportHightLimit()
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
LPCTSTR MsgShow_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

int MsgShow_TestItem::ShowMTFinfo()
{
	if(bShowMsgDlg)
	{
		AfxMessageBox(MsgInfo);
	}

	if(bShowInfo)
	{
		CRect DisplayWndRect;
		m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);
		 
		CFont ft;
		ft.CreatePointFont(InfoSize,_T("宋体"));

		//初始化
		CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
		pDisplayWndMemDC->SelectObject(&ft);	
		pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
		pDisplayWndMemDC->SetBkMode(TRANSPARENT);
		pDisplayWndMemDC->SetTextColor(RGB(150,255,255));  

		COLORREF CurrentColor; 
		CRect rect;
		m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID);  

		if(InfoColor == _T("红色"))
		{
			CurrentColor = COLOR_RED; 
		}
		else if(InfoColor ==_T("绿色"))
		{ 
			CurrentColor = COLOR_GREEN;
		}
		else if(InfoColor == _T("蓝色"))
		{
			CurrentColor = COLOR_BLUE;
		}
		else
		{
			CurrentColor = COLOR_BLACK;
		} 
		CPen pen(PS_SOLID,2,CurrentColor);
		pDisplayWndMemDC->SelectObject(&pen); 

		//画字符
		int x = InfoPos.x * DisplayWndRect.Width()/100;
		int y = InfoPos.y * DisplayWndRect.Height()/100;  
		pDisplayWndMemDC->TextOut(x,y,InfoShow); 
		m_pInterface->Ctrl_ShowMemDC(CamID);
	}
	
	if(bDelay) Sleep(DelayTime);
	return 0;
} 