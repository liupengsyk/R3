#include "StdAfx.h"
#include "CaliOk_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new CaliOk_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
CaliOk_TestItem::CaliOk_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

CaliOk_TestItem::~CaliOk_TestItem(void)
{

}


int CaliOk_TestItem::InitItem()
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
int CaliOk_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:在此添加初始化代码
    m_nCaliOK = 0;
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int CaliOk_TestItem::Testing()
{
   //TODO:在此添加测试项代码
     CString tip;
   if (!ReadBCaliMode())
   {   
	   tip = _T("正常测试中");
	   m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_BLUE, 200);           
	   SetResult(RESULT_PASS);
   }
   else 
   {
       if (IsCaliOver())
       {
           RecordTime();
           ChangeModule();
           WriteBCaliMode(FALSE);
           ResetStationOK();

           tip = _T("点检OK,可以正常生产！");
           m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_GREEN, 200);  
           SetResult(RESULT_PASS);
       }
       else
       {
           tip.Format(_T("需点检工位%d,以点检工位%d,请继续点检."), pDlg->m_nStationNum, m_nCaliOK);           
           m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_RED, 200);
           m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("点检未完成！"));
           SetResult(RESULT_FAIL);
       }
       AfxMessageBox(tip);
   }
   
   return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int CaliOk_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int CaliOk_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
    pDlg->m_nStationNum = ReadConfigInt(_T("StationNum"), 1);
    pDlg->m_strStdPath  = ReadConfigString(_T("StdPath"), _T(""));
    pDlg->m_strModule   = ReadConfigString(_T("Module"), _T("OGP0757"));

    CString strDataDire = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRCalibration\\");
    CreateDirectory(strDataDire, NULL);
    m_strStdPath    = strDataDire + pDlg->m_strStdPath;

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int CaliOk_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   

    WriteConfigInt(_T("StationNum"), pDlg->m_nStationNum);
    WriteConfigString(_T("StdPath"), pDlg->m_strStdPath);
    WriteConfigString(_T("Module"), pDlg->m_strModule);


	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR CaliOk_TestItem::GetReportHeads()
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
LPCTSTR CaliOk_TestItem::GetReportLowLimit()
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
LPCTSTR CaliOk_TestItem::GetReportHightLimit()
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
LPCTSTR CaliOk_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

void CaliOk_TestItem::RecordTime()
{
    wchar_t wcSubReg[128] = {0};
    wchar_t wcval[32] = {0};
    CTime t=CTime::GetCurrentTime();

    swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
    swprintf(wcval,
        _T("%02d日%02d时%02d分:%d"),
        t.GetDay(),
        t.GetHour(),
        t.GetMinute(),
        GetTimeTag()
        );
    RegSetKey(wcSubReg, _T("Time"), wcval);
}

BOOL CaliOk_TestItem::ReadBCaliMode()
{
	CString tip;
	wchar_t wcSubReg[128] = {0};
	wchar_t wcval[32] = {0};

	swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
	RegReadKey(wcSubReg, _T("bCaliMode"), wcval);

	return _ttoi(wcval);
}

void CaliOk_TestItem::WriteBCaliMode(BOOL bCaliMode)
{
    wchar_t wcSubReg[128] = {0};
    wchar_t wcval[32] = {0};

    swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
    swprintf(wcval, _T("%d"), bCaliMode);
    RegSetKey(wcSubReg, _T("bCaliMode"), wcval); //
}

void CaliOk_TestItem::ResetStationOK()
{
    CString appName = GetName();
    SetName(_T("SampleModule"));

    CString keyName;
    for (int i=0; i<pDlg->m_nStationNum; i++)
    {
        keyName.Format(_T("Station%d"), i);
        WriteConfigInt(keyName, 0);
    }

    SetName(appName);//置回原测试项名
    //m_pInterface->UnlocktheFile(CamID);2.0.2.1前不支持
}

void CaliOk_TestItem::ChangeModule()
{
    wchar_t wcSubReg[128] = {0};
    wchar_t wcval[32] = {0};

    swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
    swprintf(wcval, _T("%s"), pDlg->m_strModule);
    RegSetKey(wcSubReg, _T("Module"), wcval); //切换机种
}

BOOL CaliOk_TestItem::IsCaliOver()
{
    CString appName = GetName();
    SetName(_T("SampleModule"));

    CString keyName;
    for (int i=0; i<pDlg->m_nStationNum; i++)
    {
        keyName.Format(_T("Station%d"), i);
        BOOL ok = ReadConfigInt(keyName, 0);
        if (ok)
        {
            m_nCaliOK++;
        }
    }

    SetName(appName);//置回原测试项名

    return pDlg->m_nStationNum == m_nCaliOK ? TRUE : FALSE;
}

/*
BOOL CaliOk_TestItem::LoadData()
{
	CString errMsg;
	m_stdData.Init();
	if (!m_stdData.LoadCaliFile(m_strStdPath, errMsg))
	{
		m_pInterface->Ctrl_Addlog(CamID, errMsg, COLOR_RED, 200);
		return FALSE;
	}

	if (m_stdData.m_data.size() < 2)
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("标准数据错误"), COLOR_RED, 200);
		return FALSE;
	}

	m_nFuseIDNum = m_stdData.m_data.size() - 1;
	for (UINT i=0; i<m_nFuseIDNum; i++)
	{
		m_strFuseIDArry[i] = m_stdData.m_data[i + 1].fuseID;
	}
	return TRUE;
}

BOOL CaliOk_TestItem::IsSample()
{
	CString fuseID = CamCode +_T("_FuseID:") + m_pInterface->Sensor_GetCamFuseID(CamID);

	for (int i=0; i<m_nFuseIDNum; i++)
	{
		if (fuseID == m_strFuseIDArry[i])
		{
			return TRUE;
		}
	}

	return FALSE;
}*/