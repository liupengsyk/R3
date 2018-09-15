#include "StdAfx.h"
#include "OC_Item_TestItem.h"
#include "ImageProc.h"
#include "OPCenterThreshold.h"	


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */
#pragma comment(lib,"OPCenterThreshold.lib")

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new OC_Item_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
OC_Item_TestItem::OC_Item_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

OC_Item_TestItem::~OC_Item_TestItem(void)
{

}


int OC_Item_TestItem::InitItem()
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
int OC_Item_TestItem::Initialize()
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
int OC_Item_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	

		if (!OCTest())
		{
			SetResult(RESULT_FAIL);
		}
		else
			SetResult(RESULT_PASS);

		if (pDlg->m_Continue)
		{
			return 1;
		}
		else 
			return 0;
}
void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}
BOOL OC_Item_TestItem::OCTest()
{
	m_pInterface->Ctrl_Addlog(CamID,_T("**Get Optical Center**"),COLOR_BLUE,200);

	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID); 

	int  width   = img.imgwidth;
	int  height   =   img.imgwidth;
	BYTE * m_pDisplayBuffer=new BYTE[width*height];
	Raw10toRaw8(img.Raw_buffer,m_pDisplayBuffer,width*height);
	OFOPTCenterIn CenterIn;
	CenterIn.ImageBufferIn = m_pDisplayBuffer;
	CenterIn.Width = img.imgwidth;
	CenterIn.Height = img.imgheight;
	CenterIn.Outformat = img.RawFormat;

	UINT nSize = img.imgwidth * img.imgheight/4;
	LPBYTE  m_pImageBuffer = (LPBYTE)malloc(nSize);
	memset(m_pImageBuffer,0,nSize);

	OpticalCenterTest(&CenterIn,m_pImageBuffer,CenterX,CenterY);

	int mechanicalCenterX = img.imgwidth/2;
	int mechanicalCenterY = img.imgheight/2;

	CString TempMsg;
	TempMsg.Format(_T("%d,%d"),CenterX,CenterY);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	if(CenterX>mechanicalCenterX)
	{
		if ((CenterX-mechanicalCenterX) > OCSpecX_hig)
		{
			if (pDlg->m_OCSpec_Shift)
			{
				if ((CenterX-mechanicalCenterX) < pDlg->m_OCSpec_x_AfterShift)
				{
					CenterX = 1.0*CenterX*OCSpecX_hig/pDlg->m_OCSpec_x_AfterShift;
					if ((CenterX-mechanicalCenterX)>OCSpecX_hig)
					{
						m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x 测试NG!"),COLOR_RED,200);
						delete m_pDisplayBuffer;
						free(m_pImageBuffer);
						return 0;
					}
				}
				else
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x 测试NG!"),COLOR_RED,200);
					delete m_pDisplayBuffer;
					free(m_pImageBuffer);
					return 0;
				}
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x 测试NG!"),COLOR_RED,200);
				delete m_pDisplayBuffer;
				free(m_pImageBuffer);
				return 0;
			}
		}
	}
	if (CenterX<=mechanicalCenterX)
	{
		if ((mechanicalCenterX-CenterX) > OCSpecX_hig)
		{
			if (pDlg->m_OCSpec_Shift)
			{
				if ((mechanicalCenterX-CenterX)<pDlg->m_OCSpec_x_AfterShift)
				{
					CenterX = 1.0*CenterX*pDlg->m_OCSpec_x_AfterShift/OCSpecX_hig;
					if ((mechanicalCenterX-CenterX) > OCSpecX_hig)
					{
						m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x 测试NG!"),COLOR_RED,200);
						delete m_pDisplayBuffer;
						free(m_pImageBuffer);
						return 0;
					}
				}
				else
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x 测试NG!"),COLOR_RED,200);
					delete m_pDisplayBuffer;
					free(m_pImageBuffer);
					return 0;
				}
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x 测试NG!"),COLOR_RED,200);
				delete m_pDisplayBuffer;
				free(m_pImageBuffer);
				return 0;
			}
			
		}
	}
	if(CenterY>mechanicalCenterY)
	{
		if ((CenterY-mechanicalCenterY) > OCSpecY_hig)
		{
			if (pDlg->m_OCSpec_Shift)
			{
				if ((CenterY-mechanicalCenterY) < pDlg->m_OCSpec_y_AfterShift)
				{
					CenterY = 1.0*CenterY*OCSpecY_hig/pDlg->m_OCSpec_y_AfterShift;
					if ((CenterY-mechanicalCenterY) > OCSpecY_hig)
					{
						m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y 测试NG!"),COLOR_RED,200);
						delete m_pDisplayBuffer;
						free(m_pImageBuffer);
						return 0;
					}
				}
				else
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y 测试NG!"),COLOR_RED,200);
					delete m_pDisplayBuffer;
					free(m_pImageBuffer);
					return 0;
				}
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y 测试NG!"),COLOR_RED,200);
				delete m_pDisplayBuffer;
				free(m_pImageBuffer);
				return 0;
			}
			
		}
	}
	if(CenterY<=mechanicalCenterY)
	{
		if ((mechanicalCenterY-CenterY) > OCSpecY_hig)
		{
			if (pDlg->m_OCSpec_Shift)
			{
				if ((mechanicalCenterY-CenterY) < pDlg->m_OCSpec_y_AfterShift)
				{
					CenterY = 1.0*CenterY*pDlg->m_OCSpec_y_AfterShift/OCSpecY_hig;
					if ((mechanicalCenterY-CenterY) > OCSpecY_hig)
					{
						m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y 测试NG!"),COLOR_RED,200);
						delete m_pDisplayBuffer;
						free(m_pImageBuffer);
						return 0;
					}
				}
				else
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y 测试NG!"),COLOR_RED,200);
					delete m_pDisplayBuffer;
					free(m_pImageBuffer);
					return 0;
				}
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y 测试NG!"),COLOR_RED,200);
				delete m_pDisplayBuffer;
				free(m_pImageBuffer);
				return 0;
			}
			
		}
	}
	m_pInterface->Ctrl_Addlog(CamID,_T("OC 测试 PASS !"),COLOR_BLUE,200);
	delete m_pDisplayBuffer;
	return 1;
}

/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OC_Item_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码

	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OC_Item_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 

	OCSpecX_hig=ReadConfigInt(_T("OCSpecX_hig"),100);
	OCSpecY_hig=ReadConfigInt(_T("OCSpecY_hig"),100);

	pDlg->m_OCSpec_Shift = ReadConfigInt(_T("OCSpec_Shift"),0);
	pDlg->m_OCSpec_x_AfterShift = ReadConfigInt(_T("OCSpec_x_AfterShift"),100);
	pDlg->m_OCSpec_y_AfterShift = ReadConfigInt(_T("OCSpec_y_AfterShift"),100);
	

	pDlg->m_Continue=ReadConfigInt(_T("m_Continue"),0);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OC_Item_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   

	WriteConfigInt(_T("OCSpecX_hig"),pDlg->m_OCSpecX_hig);
	WriteConfigInt(_T("OCSpecY_hig"),pDlg->m_OCSpecY_hig);
	WriteConfigInt(_T("OCSpec_Shift"),pDlg->m_OCSpec_Shift);
	WriteConfigInt(_T("OCSpec_x_AfterShift"),pDlg->m_OCSpec_x_AfterShift);
	WriteConfigInt(_T("OCSpec_y_AfterShift"),pDlg->m_OCSpec_y_AfterShift);

	WriteConfigInt(_T("m_Continue"),pDlg->m_Continue);
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR OC_Item_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("模组编号,OptiX,OptiY");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR OC_Item_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
//	LowerLimit.Format(_T("%d,%d"),OCSpecX_low,OCSpecY_low);
	return LowerLimit;
}


/******************************************************************
函数名:    GetReportUpperLimits
函数功能:  保存测试标准的上限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR OC_Item_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
//	UpperLimit.Format(_T("%d,%d"),OCSpecX_hig,OCSpecY_hig);
	return UpperLimit;
}


/******************************************************************
函数名:    GetReportContents
函数功能:  保存对应测试名字种类的测试的数据
返回值：   字符串指针
*******************************************************************/
LPCTSTR OC_Item_TestItem::GetReportContents()
{
	static CString Content;
	CString CamCode=m_pInterface->Ctrl_GetCamCode();
	Content.Format(_T("%s,%d,%d"),CamCode,CenterX,CenterY);
	return Content;
}

