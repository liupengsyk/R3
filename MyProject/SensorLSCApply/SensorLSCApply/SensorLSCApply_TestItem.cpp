#include "StdAfx.h"
#include "SensorLSCApply_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new SensorLSCApply_TestItem(pInterface, lpszName);
}


/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
SensorLSCApply_TestItem::SensorLSCApply_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

SensorLSCApply_TestItem::~SensorLSCApply_TestItem(void)
{

}


int SensorLSCApply_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	
	return 0;
}


/******************************************************************
������:    Initialize
��������:  ���ڲ���ǰ��ʼ��һЩ������״̬������ռ��
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int SensorLSCApply_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:�ڴ���ӳ�ʼ������
    LoadOption(); 
	return 0;
}



/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int SensorLSCApply_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������

	if (GlobalTime < 12)
	{
		m_pInterface->Ctrl_PauseGrabFrame(CamID);
		if (_T("S5K5E8") == SensorName)
		{
			m_pInterface->Sensor_ApplyLsc(CamID,NULL,0,0,0,0,NULL,0,0);
		}
		else if (_T("S5K4H7YX") == SensorName)
		{
			m_pInterface->Sensor_ApplyLsc(CamID,NULL,0,0,0,0,NULL,0,0);
		}
		else if (_T("S5K4H8_F1X9") == SensorName)
		{
			m_pInterface->Sensor_ApplyLsc(CamID,NULL,0,0,0,0,NULL,0,0);
		}
		else
		{
			Sleep(pDlg->m_DelayTime);
			m_pInterface->Ctrl_ResumeGrabFrame(CamID);
			m_pInterface->Ctrl_Addlog(CamID,_T("Sensorû����ӣ������!"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Sensorû����ӣ������!"));
			SetResult(RESULT_FAIL);
			return 0;
		}
		Sleep(pDlg->m_DelayTime);
		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		m_pInterface->Ctrl_Addlog(CamID,_T("Apply LSC Pass!"),COLOR_BLUE,200);
	}
	
	SetResult(RESULT_PASS);
    return 0;
}


/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int SensorLSCApply_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int SensorLSCApply_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	CString CurrentName = GetName();
	SetName(_T("Sensor"));
	SensorName = ReadConfigString(_T("SensorName"),_T("S5K5E8"));

	SetName(_T("WBSetting"));	
	GlobalTime=ReadConfigInt(_T("GlobalTime"), 3);

	SetName(CurrentName);
	pDlg->m_DelayTime = ReadConfigInt(_T("DelayTime"),500);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int SensorLSCApply_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   
	WriteConfigInt(_T("DelayTime"),pDlg->m_DelayTime);

	LoadOption();
	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR SensorLSCApply_TestItem::GetReportHeads()
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
LPCTSTR SensorLSCApply_TestItem::GetReportLowLimit()
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
LPCTSTR SensorLSCApply_TestItem::GetReportHightLimit()
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
LPCTSTR SensorLSCApply_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

