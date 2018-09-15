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
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
OTPBurn_TestItem::OTPBurn_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

OTPBurn_TestItem::~OTPBurn_TestItem(void)
{

}


int OTPBurn_TestItem::InitItem()
{
	CamID = GetCamID();
	CurfilePath = m_pInterface->Config_GetConfigPathName(CamID);
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	LoadOption(); 
	return 0;
}


/******************************************************************
������:    Initialize
��������:  ���ڲ���ǰ��ʼ��һЩ������״̬������ռ��
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int OTPBurn_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:�ڴ���ӳ�ʼ������
 
	return 0;
}



/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int OTPBurn_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
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
			StartAddress = strtol(list[i].startAddr, NULL, 16 );		//���ַ���ת��16��������
			EndAddress = strtol(list[i].endAddr,NULL,16);				//���ַ���ת��16��������

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
	��������:��ȡEEPROM�е����ݣ�������Ϊtxt�ĵ�
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
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int OTPBurn_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int OTPBurn_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	//CString CurrentName=GetName();
	//SetName(_T("GENERAL_SETTING"));
	SlaveID = ReadConfigInt(_T("m_EEpromSlaveId"),0);
	pDlg->m_erro_code = ReadConfigInt(_T("m_erro_code"),18);
	pDlg->m_erro_info = ReadConfigString(_T("m_erro_info"), _T("OTP burn fail"));;
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int OTPBurn_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   

	WriteConfigString(_T("m_erro_info"),pDlg->m_erro_info);

	WriteConfigInt(_T("m_erro_code"),pDlg->m_erro_code);

	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OTPBurn_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("");
	header = _T("Result");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OTPBurn_TestItem::GetReportLowLimit()
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
LPCTSTR OTPBurn_TestItem::GetReportHightLimit()
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