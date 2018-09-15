#include "StdAfx.h"
#include "AFCalibration_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new AFCalibration_TestItem(pInterface, lpszName);
}


/******************************************************************
函数名:    构造函数
函数功能:  对象生成的时候初始必须完成的配置
*******************************************************************/
AFCalibration_TestItem::AFCalibration_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

AFCalibration_TestItem::~AFCalibration_TestItem(void)
{

}


int AFCalibration_TestItem::InitItem()
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
int AFCalibration_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);

	m_nFuseIDNum = 0;
	m_strCurFuseID = m_pInterface->Ctrl_GetCamCode() +_T("_FuseID:") + m_pInterface->Sensor_GetCamFuseID(CamID);

    CString strDataDire = m_pInterface->Ctrl_GetCurrentPath() + _T("SFRCalibration\\");
    CreateDirectory(strDataDire, NULL);
    m_strStdPath    = strDataDire  + pDlg->m_strStdPath;

    CString strReportDire = m_pInterface->Config_ReadConfigString(_T("GENERAL_SETTING"), _T("sReportPath"), _T(""), CamID);
    m_strTestPath   = strReportDire + _T("\\") + pDlg->m_strTestPath;

	for (int i=0; i< MAX_SAMPLE_NUM; i++)
	{
		m_strFuseIDArry[i] = _T("");
	}
	
	return 0;
}



/******************************************************************
函数名:    Testing
函数功能:  完成某一项测试功能代码放与此
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AFCalibration_TestItem::Testing()
{	 
	CString errMsg;
    CString tip;
	BOOL bCaliflag = FALSE;
	BOOL bCaliMode = ReadBCaliMode();
	if(bCaliMode) // Calibration mode or normal test mode
	{  
        m_pInterface->Ctrl_Addlog(CamID, _T("进入点检模式"), COLOR_BLUE, 200);

		bCaliflag = Calibration(errMsg);
		if (bCaliflag) 
		{
            SetStationState(TRUE);
		}
        else
        {
            SetStationState(FALSE);
        }
	}
	else	                               
	{
		m_pInterface->Ctrl_Addlog(CamID, _T("正常测试中"), COLOR_BLUE, 200);

        if (IsNeedCali())
        {
            WriteBCaliMode(TRUE); // Next test enter calibration mode
            SetResult(RESULT_FAIL);
        }
        else
        {
            SetResult(RESULT_PASS);
        }

        return 0;
	}

	if (bCaliflag)
	{
        tip = _T("工位:") + pDlg->m_strStation + _T("点检OK");
		m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_GREEN, 200);
		SetResult(RESULT_PASS);
		
	}
    else
	{
        m_pInterface->Ctrl_Addlog(CamID, errMsg, COLOR_RED, 200);
        AfxMessageBox(errMsg);

        tip = _T("工位:") + pDlg->m_strStation + _T("点检Fail");
        m_pInterface->Ctrl_SetCamErrorInfo(CamID, tip);
		SetResult(RESULT_FAIL);
	}

    return 0;
}


/******************************************************************
函数名:    Finalize
函数功能:  用于Initialize申明的变量恢复原始值，申请的内存空间释放等
返回值：   0 - 完成   非0 - 未完成
*******************************************************************/
int AFCalibration_TestItem::Finalize()
{
	//TODO:在此添加回收结束代码


	return 0;
}


/******************************************************************
函数名:    LoadOption
函数功能:  用于从配置文件读取参数到变量，和赋值到对话框的绑定变量 (文件存储值->变量—>控件的值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AFCalibration_TestItem::LoadOption()
{
	//TODO: 在此添加读取代码 
	pDlg->LoadPara();

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
函数名:    SaveOption
函数功能:  用于将变量数值或对话框的绑定变量数值保存到配置文件中 (控件的值—>变量->文件存储值)
返回值：   0 - 成功   非0 - 不成功
*******************************************************************/
int AFCalibration_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: 在此添加存储代码	   
	pDlg->SavePara();

	//........................
	return 0;
}


/******************************************************************
函数名:    GetReportHeaders
函数功能:  保存需要保存的测试项目名字种类
返回值：   字符串指针
*******************************************************************/
LPCTSTR AFCalibration_TestItem::GetReportHeads()
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
LPCTSTR AFCalibration_TestItem::GetReportLowLimit()
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
LPCTSTR AFCalibration_TestItem::GetReportHightLimit()
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
LPCTSTR AFCalibration_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

BOOL AFCalibration_TestItem::IsNeedCali()
{
    CString tip;
	wchar_t wcSubReg[128] = {0};
	wchar_t wcval[32] = {0};

	//module check
	swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
	RegReadKey(wcSubReg, _T("Module"), wcval);
	CString strVal(wcval);

	if (strVal != pDlg->m_strModule)
	{
        tip = _T("当前机种未点检过，下次测试开始点检");
        AfxMessageBox(tip);
        m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_RED, 200);
        return TRUE;
	}
	
	//time check
	swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
	RegReadKey(wcSubReg, _T("Time"), wcval);
	strVal = wcval;

	int     separator = strVal.Find(_T(":"));
	CString lastTime  = strVal.Left(separator);
	CString hourCount = strVal.Mid((separator + 1));

	int iNowTimeTag = GetTimeTag();
	if(abs(iNowTimeTag - _ttoi(hourCount)) > pDlg->m_nIntervalTime)
	{
		tip.Format(_T("上次点检时间:%s,请点检!"), lastTime);
        AfxMessageBox(tip);
        m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_RED, 200);
        return TRUE;
	}
	else
	{
		tip.Format(_T("上次点检时间:%s."), lastTime);
        m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_BLUE, 200);
		return FALSE;
	}
}


BOOL AFCalibration_TestItem::Calibration(CString &errMsg)
{
    if (!LoadData(errMsg))
    {
        return FALSE;
    }

    if (!IsSample(errMsg)) //sample模组才能点检
    {
        return FALSE;
    }

	if (CompData(errMsg)) //解析力达标Check
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

void AFCalibration_TestItem::WriteBCaliMode(BOOL bCaliMode)
{
    wchar_t wcSubReg[128] = {0};
    wchar_t wcval[32] = {0};

    swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
    swprintf(wcval, _T("%d"), bCaliMode);
    RegSetKey(wcSubReg, _T("bCaliMode"), wcval);
}

BOOL AFCalibration_TestItem::ReadBCaliMode()
{
	CString tip;
	wchar_t wcSubReg[128] = {0};
	wchar_t wcval[32] = {0};

	swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
	RegReadKey(wcSubReg, _T("bCaliMode"), wcval);

	return _ttoi(wcval);
}

BOOL AFCalibration_TestItem::IsSample(CString &errMsg)
{
	for (int i=0; i<m_nFuseIDNum; i++)
	{
		if (m_strCurFuseID == m_strFuseIDArry[i])
		{
			return TRUE;
		}
	}

    errMsg.Format(_T("当前模组%s非Sample, 请换Sample点检!"), m_pInterface->Sensor_GetCamFuseID(CamID));
	return FALSE;
}

BOOL AFCalibration_TestItem::CompData(CString &errMsg)
{
    int index = 0;
    for (int i=1; i<=m_nFuseIDNum; i++)
    {
       if (m_stdData.m_data[i].fuseID == m_strCurFuseID)
       {
           index = i;
           break;
       }
    }

    float *diff = new float[(m_stdData.m_head.size() - 3)];
	if (nullptr == diff)
	{
		errMsg =  _T("new diff buff fail!");
		return FALSE;
	}
    memset(diff, 0, (m_stdData.m_head.size() - 3)*sizeof(float));

    for (UINT i=0; i<(m_stdData.m_head.size() - 3); i++)
    {
        diff[i] = m_stdData.m_data[0].data[i];
    }

    BOOL flag = TRUE;
    CString info;
    for (UINT i=0; i<(m_testData.m_head.size() - 3); i++)
    {
       float std  = m_stdData.m_data[index].data[i];
       float test = m_testData.m_data[0].data[i];

       if ((abs(diff[i])+ 0.000002) < abs((std - test))) //浮点数精度
       {
           flag = FALSE;
           info.Format(_T("%s:(std - test = %0.6f),spe:%0.6f")
               ,m_testData.m_head[i+3], (std - test), abs(diff[i]));
           m_pInterface->Ctrl_Addlog(CamID, info, COLOR_RED, 200);
       }
       else
       {
           info.Format(_T("%s:(std - test = %0.6f),spe:%0.6f")
               ,m_testData.m_head[i+3], (std - test), abs(diff[i]));
           m_pInterface->Ctrl_Addlog(CamID, info, COLOR_BLUE, 200);
       }
	}

	if (!flag)
	{
		errMsg = _T("当前测试数据超标准范围");
	}
	delete[] diff; 
    return flag;
}

void AFCalibration_TestItem::SetStationState(BOOL state)
{
    CString appName = GetName();
    CString keyName;
    keyName.Format(_T("Station%d"), pDlg->m_nStationNum);
	SetName(_T("SampleModule"));
    WriteConfigInt(keyName, state);
    SetName(appName);

    //m_pInterface->UnlocktheFile(CamID); //老四焦段架构不兼容
}

BOOL AFCalibration_TestItem::LoadData(CString &errMsg)
{
    m_stdData.Init();
    m_testData.Init();
    if (!m_testData.LoadLastRecode(m_strTestPath, errMsg))  //加载测试数据
    {
        return FALSE;
    }
    
    CString tip;
    tip.Format(_T("currentFuseID:%s"), m_strCurFuseID);
    m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_BLUE, 200);

    if (m_strCurFuseID != m_testData.m_data[0].fuseID) //改内部成员
    {
        errMsg = _T("无此模组测试数据,请先测试解析力.");
        return FALSE;
    }

    if (!m_stdData.LoadCaliFile(m_strStdPath, errMsg))
    {
        return FALSE;
    }

    if (m_testData.m_head.size() != m_stdData.m_head.size())
    {
        errMsg = _T("测试数据与标准数据格式不同");
        return FALSE;
    }

    if (m_stdData.m_data.size() < 2)
    {
        errMsg = _T("标准数据格式错误");
        return FALSE;
    }

    m_nFuseIDNum = m_stdData.m_data.size() - 1;
    for (int i=0; i<m_nFuseIDNum; i++)
    {
        m_strFuseIDArry[i] = m_stdData.m_data[i + 1].fuseID;
    }

    return TRUE;
}

