#include "StdAfx.h"
#include "OTPBurn_TestItem.h"
#include "ImageProc.h"
#include "XML.h"
#include "string.h"


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new OTPBurn_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
OTPBurn_TestItem::OTPBurn_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

OTPBurn_TestItem::~OTPBurn_TestItem(void)
{

}


int OTPBurn_TestItem::InitItem()
{
	CamID = GetCamID();
	CurfilePath = m_pInterface->Config_GetConfigPathName(CamID);
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption(); 
	return 0;
}


/******************************************************************
函数名:    Initialize
函数功能:  用于测试前初始化一些变量，状态，分配空间等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OTPBurn_TestItem::Initialize()
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
int OTPBurn_TestItem::Testing()
{
   //TODO:在此添加测试项代码
	vector<ItemInfo> list;
	CString m_Site;
  switch(m_pInterface->GetSiteNo())
   {
   case  0:   
   case  1:
	   m_Site = _T("A"); 
	   break;
   case  2:
	   m_Site = _T("B");
	   break;
   case  3:
	   m_Site = _T("C");
	   break;
   case  4:
	   m_Site = _T("D");
	   break;
   }


	CString filePath = m_pInterface->Ctrl_GetCurrentPath()+_T("PDAFTool\\")+m_Site+_T("\\OTPBurnData.xml");

	CXML XML(filePath);
	BOOL ret = XML.GetItems(list);
	USHORT StartAddress,EndAddress;
	USHORT BurnBuff[0x2000];
	USHORT CheckBurnBuff[0x2000]={0};
	CString strTmp;
	if (ret)
	{
		for (int i = 0; i < list.size(); i++)
		{
			CStringA BurnInfo;
			StartAddress = strtol(list[i].startAddr, NULL, 16 );		//将字符串转成16进制数字
			EndAddress = strtol(list[i].endAddr,NULL,16);				//将字符串转成16进制数字

			memset(BurnBuff,0,0x2000);
			for (int index = 0; index < list[i].length; index++)
			{
				BurnBuff[index] =  list[i].dataList[index];
			}
			for(int j=0;j<5;j++)
			{
				ret = m_pInterface->Otp_OtpWrite(StartAddress,EndAddress,BurnBuff,CamID);
				if (ret == 1)
				{
					break;
				}
			}
 			
			memset(CheckBurnBuff,0,0x2000);
			if(1 == ret)
			{
				ret = m_pInterface->Otp_OtpRead(StartAddress,EndAddress,CheckBurnBuff,CamID);
				for (int index=0;index < sizeof(CheckBurnBuff)-1;index++)
				{
					if(BurnBuff[i] != CheckBurnBuff[i])
					{
						strTmp.Format(_T("Check BurnBuff Error"));
						m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);				
						DeleteFile(filePath);
						m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_erro_info);
						m_pInterface->Ctrl_SetCamErrorCode(CamID, pDlg->m_erro_code);
						SetResult(RESULT_FAIL);
						return 0;
					}
				}	
			}
			else
			{
				strTmp.Format(_T("Burn Error Code %d"), ret);
				m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
				DeleteFile(filePath);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_erro_info);
				m_pInterface->Ctrl_SetCamErrorCode(CamID, pDlg->m_erro_code);
				SetResult(RESULT_FAIL);
				return 0;
			}
		}
	}

	if(1 == ret)
	{
		strTmp.Format(_T("Burn OK"));
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_BLUE, 200);
		DeleteFile(filePath);
		SetResult(RESULT_PASS);
		return 0;
	}
	else
	{
		strTmp.Format(_T("ReadI2C Error Code %d"), ret);
		m_pInterface->Ctrl_Addlog(CamID, strTmp, COLOR_RED, 200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID, pDlg->m_erro_info);
		m_pInterface->Ctrl_SetCamErrorCode(CamID, pDlg->m_erro_code);
		SetResult(RESULT_FAIL);
		DeleteFile(filePath);
		return 0;
	}
	return 0;
}

/**
	函数功能:读取EEPROM中的数据，并保存为txt文档
***/
int OTPBurn_TestItem::ReadEEpromAllData()
{
	const UINT EEPROMBuffSize = 0x2000;
	USHORT tmpBuff[EEPROMBuffSize] = {0};
	m_pInterface->Otp_OtpRead(0x0000, EEPROMBuffSize-1, tmpBuff, CamID);

	CStdioFile cf;
	cf.Open(_T("D:\\EepromDump.txt"), CFile::modeCreate|CFile::modeWrite|CFile::typeText);

	CString content = _T("");
	for (int i=0; i<EEPROMBuffSize; i++)
	{
		CString tmp = _T("");
		tmp.Format(_T("0x%.4X, 0x%.2X\n"), i, tmpBuff[i]);
		content += tmp;
	}
	cf.WriteString(content);
	cf.Close();

	m_pInterface->Ctrl_Addlog(CamID, _T("EEPROM All Data Dump Finished ."), COLOR_GREEN, 200);
	SetResult(RESULT_PASS);
	return 0;
}
/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int OTPBurn_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OTPBurn_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	//CString CurrentName=GetName();
	//SetName(_T("GENERAL_SETTING"));
	SlaveID = ReadConfigInt(_T("m_EEpromSlaveId"),0);
	pDlg->m_erro_code = ReadConfigInt(_T("m_erro_code"),18);
	pDlg->m_erro_info = ReadConfigString(_T("m_erro_info"), _T("OTP burn fail"));;
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int OTPBurn_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   

	WriteConfigString(_T("m_erro_info"),pDlg->m_erro_info);

	WriteConfigInt(_T("m_erro_code"),pDlg->m_erro_code);

	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR OTPBurn_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header = _T("Result");
	return header;
}


/******************************************************************
函数名:    GetReportLowerLimits
函数功能:  保存测试标准的下限值
返回值：   字符串指针
*******************************************************************/
LPCTSTR OTPBurn_TestItem::GetReportLowLimit()
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
LPCTSTR OTPBurn_TestItem::GetReportHightLimit()
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
LPCTSTR OTPBurn_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	if (GetResult()==RESULT_FAIL)
	{
		Content +=_T("Fail");
	}
	else if (GetResult()==RESULT_PASS)
	{
		Content +=_T("Pass");
	}
	else 
	{
		Content +=_T("Pass");
	}
	return Content;
}