#include "StdAfx.h"
#include "MesCheck_TestItem.h"
#include "ImageProc.h"


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new MesCheck_TestItem(pInterface, lpszName);
}

enum 
{
	MESCHECK = 0,
	MESBINDING = 1
};

/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
MesCheck_TestItem::MesCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

MesCheck_TestItem::~MesCheck_TestItem(void)
{

}


int MesCheck_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption(); 

	m_pInterface->SetMesInfo(0,bCam0Mes,Cam0MesID);
	m_pInterface->SetMesInfo(1,bCam1Mes,Cam1MesID);
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int MesCheck_TestItem::Initialize()
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
int MesCheck_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	if (bMesCheck)
	{
		if (bMesBinding)
		{
			if (bMesDelete)
			{
				CamID = (bMesDelete<<8) + CamID;
			}
		}
		
		if(m_pInterface->mesCheck(CamID))
		{
			SetResult(RESULT_PASS);
		}
		else
		{
			CamID = CamID&0xff ;
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("MesCheck Failed!"));
			SetResult(RESULT_FAIL);
			return 0;
		}

		CamID = CamID&0xff ;
	}

	if(bMesBinding)
	{
		CString RunCard = _T("");
		CString BarCode = _T("");

		BOOL bResult = GetRunCardAndBarcode(RunCard,BarCode);
		if (FALSE == bResult)
		{
			SetResult(RESULT_FAIL);
			return 0;
		}

		//Runcard 有效性验证
		if ((RunCard.Mid(0,3) != _T("AEM")) &&
			(RunCard.Mid(0,3) != _T("AEP")) && 
			(RunCard.Mid(0,3) != _T("TEM")) &&
			(RunCard.Mid(0,3) != _T("ARM")))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("RunCard无效"),COLOR_RED,200);
			SetResult(RESULT_FAIL);
			return 0;
		}
		//Barcode 有效性验证
		if (bUseBarcode)
		{
			if (BarCode.GetLength() != iBarcodeLength)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Barcode长度不正确"),COLOR_RED,200);
				SetResult(RESULT_FAIL);
				return 0;
			}
			if (BarCode.Find(BarcodeKeyString.GetBuffer(BarcodeKeyString.GetLength())) == -1)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Barcode格式不正确"),COLOR_RED,200);
				SetResult(RESULT_FAIL);
				return 0;
			}
		}

		if(m_pInterface->mesBanding(CamID,RunCard,BarCode))
		{
			SetResult(RESULT_PASS);
		}
		else
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("MesBanding Fail!"));
			SetResult(RESULT_FAIL);
		}
	}

   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int MesCheck_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int MesCheck_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 

	bCam0Mes= ReadConfigInt(L"bCam0Mes",0);

	bCam1Mes = ReadConfigInt(L"bCam1Mes",0);

	Cam0MesID = ReadConfigInt(L"Cam0MesID",0);

	Cam1MesID = ReadConfigInt(L"Cam1MesID",1);	

	iBarcodeLength = ReadConfigInt(L"BarcodeLenght",9);

	bUseBarcode = (BOOL)ReadConfigInt(L"UseBarcode",0);

	BarcodeKeyString = ReadConfigString(L"KeyString",L"");

	bMesDelete = ReadConfigInt(L"bMesDelete",0);

	bMesCheck = ReadConfigInt(L"bMesCheck",0);
	bMesBinding = ReadConfigInt(L"bMesBinding",0);
	 
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int MesCheck_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	WriteConfigInt(L"bMesDelete",bMesDelete);

	WriteConfigInt(L"bCam0Mes",bCam0Mes);
	  
	WriteConfigInt(L"bCam1Mes",bCam1Mes);
	  
	WriteConfigInt(L"Cam0MesID",Cam0MesID);
	  
	WriteConfigInt(L"Cam1MesID",Cam1MesID);

	WriteConfigInt(L"BarcodeLenght",iBarcodeLength);

	WriteConfigInt(L"UseBarcode",bUseBarcode);

	WriteConfigString(L"KeyString",BarcodeKeyString);

	WriteConfigInt(L"bMesCheck",bMesCheck);

	WriteConfigInt(L"bMesBinding",bMesBinding);
	  
	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR MesCheck_TestItem::GetReportHeads()
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
LPCTSTR MesCheck_TestItem::GetReportLowLimit()
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
LPCTSTR MesCheck_TestItem::GetReportHightLimit()
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
LPCTSTR MesCheck_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}


void MesCheck_TestItem::SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist)
{
	if(SrcString == _T("")) return;
	strlist->clear();
	CString temp, str;
	int nStart = 0, index = 0;
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //判断是否已','结尾
	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//如果最后一个不是分割字符，再把后面的一段保存再退出
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);

				if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
				strlist->push_back(str);
			}
			break;//如果最后一个就是分割字符，退出
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
		strlist->push_back(str);
		nStart = index + 1;
	}
}


BOOL MesCheck_TestItem::GetRunCardAndBarcode(CString &RunCard,CString &Barcode)
{
	wchar_t key[512] = {0};
	wchar_t tempStr[512] = {0};

	swprintf_s(key,512,_T("Command_%d"),m_pInterface->GetSiteNo());
	RegReadKey(_T(""),key,tempStr);
	CString str(tempStr);
	vector<CString> infoStr ;
	SeparateString(str,_T(':'),&infoStr);
	if (infoStr.size() == 1 || infoStr.size() == 0)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("请先扫描RunCard，再扫描二维码"),COLOR_RED,200);
		return FALSE;
	}
	CString log;
	RunCard = infoStr[1];
	log.Format(_T("RunCard： %s"),RunCard);
	m_pInterface->Ctrl_Addlog(CamID,log,COLOR_BLUE,200);

	if (bUseBarcode)
	{
		if (infoStr.size() < 3)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("请扫描二维码后测试"),COLOR_RED,200);
			return FALSE;
		}else
		{
			Barcode = infoStr[2];
			
			log.Format(_T("Barcode： %s"),Barcode);
			m_pInterface->Ctrl_Addlog(CamID,log,COLOR_BLUE,200);
		}
	}
	//clear
	RegSetKey(_T(""),key,_T(""));

	return TRUE;
}