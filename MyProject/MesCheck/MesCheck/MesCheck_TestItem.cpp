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
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
MesCheck_TestItem::MesCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

MesCheck_TestItem::~MesCheck_TestItem(void)
{

}


int MesCheck_TestItem::InitItem()
{
	CamID = GetCamID();
	
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	LoadOption(); 

	m_pInterface->SetMesInfo(0,bCam0Mes,Cam0MesID);
	m_pInterface->SetMesInfo(1,bCam1Mes,Cam1MesID);
	return 0;
}


/******************************************************************
������:    Initialize
��������:  ���ڲ���ǰ��ʼ��һЩ������״̬������ռ��
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int MesCheck_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	//TODO:�ڴ���ӳ�ʼ������
 
	return 0;
}



/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int MesCheck_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
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

		//Runcard ��Ч����֤
		if ((RunCard.Mid(0,3) != _T("AEM")) &&
			(RunCard.Mid(0,3) != _T("AEP")) && 
			(RunCard.Mid(0,3) != _T("TEM")) &&
			(RunCard.Mid(0,3) != _T("ARM")))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("RunCard��Ч"),COLOR_RED,200);
			SetResult(RESULT_FAIL);
			return 0;
		}
		//Barcode ��Ч����֤
		if (bUseBarcode)
		{
			if (BarCode.GetLength() != iBarcodeLength)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Barcode���Ȳ���ȷ"),COLOR_RED,200);
				SetResult(RESULT_FAIL);
				return 0;
			}
			if (BarCode.Find(BarcodeKeyString.GetBuffer(BarcodeKeyString.GetLength())) == -1)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Barcode��ʽ����ȷ"),COLOR_RED,200);
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
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int MesCheck_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int MesCheck_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 

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
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int MesCheck_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   
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
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR MesCheck_TestItem::GetReportHeads()
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
LPCTSTR MesCheck_TestItem::GetReportLowLimit()
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
LPCTSTR MesCheck_TestItem::GetReportHightLimit()
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
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //�ж��Ƿ���','��β
	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//������һ�����Ƿָ��ַ����ٰѺ����һ�α������˳�
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);

				if (str == _T("")) str = _T("0"); //���Ϊ�գ�Ĭ�ϡ�0��,��ֹ���ݿ��ַ���Ϊ�ճ���
				strlist->push_back(str);
			}
			break;//������һ�����Ƿָ��ַ����˳�
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //���Ϊ�գ�Ĭ�ϡ�0��,��ֹ���ݿ��ַ���Ϊ�ճ���
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
		m_pInterface->Ctrl_Addlog(CamID,_T("����ɨ��RunCard����ɨ���ά��"),COLOR_RED,200);
		return FALSE;
	}
	CString log;
	RunCard = infoStr[1];
	log.Format(_T("RunCard�� %s"),RunCard);
	m_pInterface->Ctrl_Addlog(CamID,log,COLOR_BLUE,200);

	if (bUseBarcode)
	{
		if (infoStr.size() < 3)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("��ɨ���ά������"),COLOR_RED,200);
			return FALSE;
		}else
		{
			Barcode = infoStr[2];
			
			log.Format(_T("Barcode�� %s"),Barcode);
			m_pInterface->Ctrl_Addlog(CamID,log,COLOR_BLUE,200);
		}
	}
	//clear
	RegSetKey(_T(""),key,_T(""));

	return TRUE;
}