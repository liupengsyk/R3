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
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
AFCalibration_TestItem::AFCalibration_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	                             //���ò�����Ŀ������
	pDlg = new OptionDlg(NULL,this);                 //���ɶԻ������
	                                   //��ȡ����
	pDlg->Create(OptionDlg::IDD,NULL);               //�����Ի��򴰿�
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //�õ��Ի�����

	//TODO: ���ò�����Ŀ������ 
	SetType(TYPE_IMAGE);

	//TODO: �������Ա�����ʼ��


	//.....
}

AFCalibration_TestItem::~AFCalibration_TestItem(void)
{

}


int AFCalibration_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	LoadOption();

	return 0;
}


/******************************************************************
������:    Initialize
��������:  ���ڲ���ǰ��ʼ��һЩ������״̬������ռ��
����ֵ��   0 - ���   ��0 - δ���
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
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int AFCalibration_TestItem::Testing()
{	 
	CString errMsg;
    CString tip;
	BOOL bCaliflag = FALSE;
	BOOL bCaliMode = ReadBCaliMode();
	if(bCaliMode) // Calibration mode or normal test mode
	{  
        m_pInterface->Ctrl_Addlog(CamID, _T("������ģʽ"), COLOR_BLUE, 200);

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
		m_pInterface->Ctrl_Addlog(CamID, _T("����������"), COLOR_BLUE, 200);

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
        tip = _T("��λ:") + pDlg->m_strStation + _T("���OK");
		m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_GREEN, 200);
		SetResult(RESULT_PASS);
		
	}
    else
	{
        m_pInterface->Ctrl_Addlog(CamID, errMsg, COLOR_RED, 200);
        AfxMessageBox(errMsg);

        tip = _T("��λ:") + pDlg->m_strStation + _T("���Fail");
        m_pInterface->Ctrl_SetCamErrorInfo(CamID, tip);
		SetResult(RESULT_FAIL);
	}

    return 0;
}


/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int AFCalibration_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int AFCalibration_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	pDlg->LoadPara();

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int AFCalibration_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   
	pDlg->SavePara();

	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR AFCalibration_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR AFCalibration_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("");
	return LowerLimit;
}


/******************************************************************
������:    GetReportUpperLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR AFCalibration_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("");
	return UpperLimit;
}


/******************************************************************
������:    GetReportContents
��������:  �����Ӧ������������Ĳ��Ե�����
����ֵ��   �ַ���ָ��
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
        tip = _T("��ǰ����δ�������´β��Կ�ʼ���");
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
		tip.Format(_T("�ϴε��ʱ��:%s,����!"), lastTime);
        AfxMessageBox(tip);
        m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_RED, 200);
        return TRUE;
	}
	else
	{
		tip.Format(_T("�ϴε��ʱ��:%s."), lastTime);
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

    if (!IsSample(errMsg)) //sampleģ����ܵ��
    {
        return FALSE;
    }

	if (CompData(errMsg)) //���������Check
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

    errMsg.Format(_T("��ǰģ��%s��Sample, �뻻Sample���!"), m_pInterface->Sensor_GetCamFuseID(CamID));
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

       if ((abs(diff[i])+ 0.000002) < abs((std - test))) //����������
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
		errMsg = _T("��ǰ�������ݳ���׼��Χ");
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

    //m_pInterface->UnlocktheFile(CamID); //���Ľ��μܹ�������
}

BOOL AFCalibration_TestItem::LoadData(CString &errMsg)
{
    m_stdData.Init();
    m_testData.Init();
    if (!m_testData.LoadLastRecode(m_strTestPath, errMsg))  //���ز�������
    {
        return FALSE;
    }
    
    CString tip;
    tip.Format(_T("currentFuseID:%s"), m_strCurFuseID);
    m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_BLUE, 200);

    if (m_strCurFuseID != m_testData.m_data[0].fuseID) //���ڲ���Ա
    {
        errMsg = _T("�޴�ģ���������,���Ȳ��Խ�����.");
        return FALSE;
    }

    if (!m_stdData.LoadCaliFile(m_strStdPath, errMsg))
    {
        return FALSE;
    }

    if (m_testData.m_head.size() != m_stdData.m_head.size())
    {
        errMsg = _T("�����������׼���ݸ�ʽ��ͬ");
        return FALSE;
    }

    if (m_stdData.m_data.size() < 2)
    {
        errMsg = _T("��׼���ݸ�ʽ����");
        return FALSE;
    }

    m_nFuseIDNum = m_stdData.m_data.size() - 1;
    for (int i=0; i<m_nFuseIDNum; i++)
    {
        m_strFuseIDArry[i] = m_stdData.m_data[i + 1].fuseID;
    }

    return TRUE;
}

