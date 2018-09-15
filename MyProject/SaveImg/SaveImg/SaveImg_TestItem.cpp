#include "StdAfx.h"
#include "SaveImg_TestItem.h"
#include "ImageProc.h" 


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new SaveImg_TestItem(pInterface, lpszName);
}


/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
SaveImg_TestItem::SaveImg_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

SaveImg_TestItem::~SaveImg_TestItem(void)
{

}


int SaveImg_TestItem::InitItem()
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
int SaveImg_TestItem::Initialize()
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
int SaveImg_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
 //  ImageInfo img = *(m_pInterface->Ctrl_GetImgInfo(CamID));
  // int width = img.imgwidth;
  // int height = img.imgheight;
   CString FileName = pDlg->m_strFileName;
   CString currentPath = m_pInterface->Ctrl_GetCurrentPath();
   CString fullImageSavePath = currentPath + FilePath + L"\\";
   int ImgTypeSel = 0;
    if(ImgType == _T(".jpg"))
	{
		ImgTypeSel = 1;
	}
	else if(ImgType == _T(".bmp"))
	{
		ImgTypeSel = 2;
	}
	else if(ImgType == _T(".raw"))
	{
		ImgTypeSel = 3;
	}
	else if(ImgType == _T(".raw8"))
	{
		ImgTypeSel = 4;
	}
	else if(ImgType == _T("HwHDC"))//HwHDC
	{
		ImgTypeSel = 5;
		 
		ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
		ImageProc::ZoomImgSize(img.RGB24_buffer,img.imgwidth,img.imgheight,1600,1200,fullImageSavePath + FileName,_T("jpg"));
		m_pInterface->Ctrl_Addlog(CamID,fullImageSavePath + FileName,COLOR_BLUE,200);
		SetResult(RESULT_PASS);
		return 0;
	}
	else if(ImgType == _T("HwMMI"))//HwMMI
	{
		ImgTypeSel = 6;
		 
		ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID);
		ImageProc::ZoomImgSize(img.RGB24_buffer,img.imgwidth,img.imgheight,1600,1200,fullImageSavePath + FileName,_T("bmp"));
		m_pInterface->Ctrl_Addlog(CamID,fullImageSavePath + FileName,COLOR_BLUE,200);
		SetResult(RESULT_PASS);
		return 0;
	} 
    m_pInterface->Ctrl_SaveImage( CamID, 1, fullImageSavePath, FileName, ImgTypeSel, NULL);  

   SetResult(RESULT_PASS);
   return 0;
}


/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int SaveImg_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int SaveImg_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	//UserInterface here.

	pDlg->m_strFileName = ReadConfigString(_T("FileName"),_T("L_20")); 
	//FilePath = ReadConfigString(_T("FilePath"),m_pInterface->Ctrl_GetCurrentPath()+_T("TestPath\\testRectify\\"));
	FilePath = ReadConfigString(_T("FilePath"),_T("ImgSave"));
	
	ImgType = ReadConfigString(_T("ImgType"),_T(".jpg"));

	//........................8
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int SaveImg_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   

	WriteConfigString(_T("FileName"),pDlg->m_strFileName);
	 
	WriteConfigString(_T("FilePath"),FilePath);
	WriteConfigString(_T("ImgType"),ImgType);

	
	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR SaveImg_TestItem::GetReportHeads()
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
LPCTSTR SaveImg_TestItem::GetReportLowLimit()
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
LPCTSTR SaveImg_TestItem::GetReportHightLimit()
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
LPCTSTR SaveImg_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

