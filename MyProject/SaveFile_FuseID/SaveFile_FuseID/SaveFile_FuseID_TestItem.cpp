#include "StdAfx.h"
#include "SaveFile_FuseID_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new SaveFile_FuseID_TestItem(pInterface, lpszName);
}


/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
SaveFile_FuseID_TestItem::SaveFile_FuseID_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

SaveFile_FuseID_TestItem::~SaveFile_FuseID_TestItem(void)
{

}


int SaveFile_FuseID_TestItem::InitItem()
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
int SaveFile_FuseID_TestItem::Initialize()
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
int SaveFile_FuseID_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
	CString currentPath = m_pInterface->Ctrl_GetCurrentPath();
	CString FuseID;
	FuseID = m_pInterface->Sensor_GetCamFuseID(CamID);
	CString fileSrc = pDlg->fileSave;
	CString fileNew = fileSrc+ FuseID;
	BOOL bSave  = pDlg->bSave;
	
	if (bSave)
	{
		vector<CString> fileList;
		CString srcPath = currentPath + fileSrc;
		CString targetPath = srcPath+L"\\" + fileNew;
		CreateDirectory(targetPath,0);
		m_pInterface->GetDirAllFile(srcPath+L"\\",fileList);
		for (int i=0;i<fileList.size();i++)
		{
			CopyFile(srcPath + L"\\" + fileList[i],targetPath + L"\\" + fileList[i],TRUE);
		}
		
	}	
	SetResult(RESULT_PASS);
	return 0;
}


/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int SaveFile_FuseID_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int SaveFile_FuseID_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	pDlg->fileSave = ReadConfigString(L"fileSave",L"ImgSave");
	pDlg->bSave = ReadConfigInt(L"bSave",1);
	
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int SaveFile_FuseID_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   

	WriteConfigString(L"fileSave",pDlg->fileSave);
	WriteConfigInt(L"bSave",pDlg->bSave);
	 
	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR SaveFile_FuseID_TestItem::GetReportHeads()
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
LPCTSTR SaveFile_FuseID_TestItem::GetReportLowLimit()
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
LPCTSTR SaveFile_FuseID_TestItem::GetReportHightLimit()
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
LPCTSTR SaveFile_FuseID_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

