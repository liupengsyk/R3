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
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
CaliOk_TestItem::CaliOk_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

CaliOk_TestItem::~CaliOk_TestItem(void)
{

}


int CaliOk_TestItem::InitItem()
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
int CaliOk_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:�ڴ���ӳ�ʼ������
    m_nCaliOK = 0;
	return 0;
}



/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int CaliOk_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
     CString tip;
   if (!ReadBCaliMode())
   {   
	   tip = _T("����������");
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

           tip = _T("���OK,��������������");
           m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_GREEN, 200);  
           SetResult(RESULT_PASS);
       }
       else
       {
           tip.Format(_T("���칤λ%d,�Ե�칤λ%d,��������."), pDlg->m_nStationNum, m_nCaliOK);           
           m_pInterface->Ctrl_Addlog(CamID, tip, COLOR_RED, 200);
           m_pInterface->Ctrl_SetCamErrorInfo(CamID, _T("���δ��ɣ�"));
           SetResult(RESULT_FAIL);
       }
       AfxMessageBox(tip);
   }
   
   return 0;
}


/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int CaliOk_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int CaliOk_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
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
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int CaliOk_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   

    WriteConfigInt(_T("StationNum"), pDlg->m_nStationNum);
    WriteConfigString(_T("StdPath"), pDlg->m_strStdPath);
    WriteConfigString(_T("Module"), pDlg->m_strModule);


	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR CaliOk_TestItem::GetReportHeads()
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
LPCTSTR CaliOk_TestItem::GetReportLowLimit()
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
LPCTSTR CaliOk_TestItem::GetReportHightLimit()
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
        _T("%02d��%02dʱ%02d��:%d"),
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

    SetName(appName);//�û�ԭ��������
    //m_pInterface->UnlocktheFile(CamID);2.0.2.1ǰ��֧��
}

void CaliOk_TestItem::ChangeModule()
{
    wchar_t wcSubReg[128] = {0};
    wchar_t wcval[32] = {0};

    swprintf(wcSubReg, _T("AFCALIBRATION\\CamID%d_Site%d"), CamID, m_pInterface->GetSiteNo());
    swprintf(wcval, _T("%s"), pDlg->m_strModule);
    RegSetKey(wcSubReg, _T("Module"), wcval); //�л�����
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

    SetName(appName);//�û�ԭ��������

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
		m_pInterface->Ctrl_Addlog(CamID, _T("��׼���ݴ���"), COLOR_RED, 200);
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