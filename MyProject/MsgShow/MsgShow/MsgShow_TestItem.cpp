#include "StdAfx.h"
#include "MsgShow_TestItem.h"
#include "ImageProc.h"



#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new MsgShow_TestItem(pInterface, lpszName);
}


/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
MsgShow_TestItem::MsgShow_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	
	SetName(lpszName);	
	
	bShowInfo =  1;
	InfoShow = _T("��������Ҫ��ʾ����Ϣ");
	InfoPos.x = 50;
	InfoPos.y =50;
	InfoSize  =50;
	InfoColor = _T("��ɫ"); 

	bDelay = 1;
	DelayTime =1000;

	bShowMsgDlg =1;
	MsgInfo =  _T("��������Ҫ��ʾ����Ϣ");

	//���ò�����Ŀ������
	pDlg = new OptionDlg(NULL,this);                 //���ɶԻ������
	                                   //��ȡ����
	pDlg->Create(OptionDlg::IDD,NULL);               //�����Ի��򴰿�
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //�õ��Ի�����

	//TODO: ���ò�����Ŀ������ 
	SetType(TYPE_IMAGE);

	//TODO: �������Ա�����ʼ��


	//.....
}

MsgShow_TestItem::~MsgShow_TestItem(void)
{

}


int MsgShow_TestItem::InitItem()
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
int MsgShow_TestItem::Initialize()
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
int MsgShow_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������

	ShowMTFinfo();

	SetResult(RESULT_PASS);
   return 0;
}


/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int MsgShow_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int MsgShow_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 

	bShowInfo = ReadConfigInt(_T("bShowInfo"),0);
	InfoShow = ReadConfigString(_T("InfoShow"),_T("��������Ҫ��ʾ����Ϣ"));
	InfoPos.x = ReadConfigInt(_T("InfoPosx"),50);
	InfoPos.y = ReadConfigInt(_T("InfoPosy"),50);
	InfoSize = ReadConfigInt(_T("InfoSize"),50);
	InfoColor = ReadConfigString(_T("InfoColor"),_T("��ɫ")); 

	bDelay = ReadConfigInt(_T("bDelay"),1);
	DelayTime = ReadConfigInt(_T("DelayTime"),100);

	bShowMsgDlg = ReadConfigInt(_T("bShowMsgDlg"),0);
	MsgInfo =  ReadConfigString(_T("MsgInfo"),_T("��������Ҫ��ʾ����Ϣ"));

	 
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int MsgShow_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   

	WriteConfigInt(_T("bShowInfo"),bShowInfo);
	WriteConfigString(_T("InfoShow"),InfoShow);
	WriteConfigInt(_T("InfoPosx"),InfoPos.x);
	WriteConfigInt(_T("InfoPosy"),InfoPos.y);
	WriteConfigInt(_T("InfoSize"),InfoSize);
	WriteConfigString(_T("InfoColor"),InfoColor); 

	WriteConfigInt(_T("bDelay"),bDelay);
	WriteConfigInt(_T("DelayTime"),DelayTime);

	WriteConfigInt(_T("bShowMsgDlg"),bShowMsgDlg);
	WriteConfigString(_T("MsgInfo"),MsgInfo);


	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR MsgShow_TestItem::GetReportHeads()
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
LPCTSTR MsgShow_TestItem::GetReportLowLimit()
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
LPCTSTR MsgShow_TestItem::GetReportHightLimit()
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
LPCTSTR MsgShow_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

int MsgShow_TestItem::ShowMTFinfo()
{
	if(bShowMsgDlg)
	{
		AfxMessageBox(MsgInfo);
	}

	if(bShowInfo)
	{
		CRect DisplayWndRect;
		m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);
		 
		CFont ft;
		ft.CreatePointFont(InfoSize,_T("����"));

		//��ʼ��
		CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
		pDisplayWndMemDC->SelectObject(&ft);	
		pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
		pDisplayWndMemDC->SetBkMode(TRANSPARENT);
		pDisplayWndMemDC->SetTextColor(RGB(150,255,255));  

		COLORREF CurrentColor; 
		CRect rect;
		m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID);  

		if(InfoColor == _T("��ɫ"))
		{
			CurrentColor = COLOR_RED; 
		}
		else if(InfoColor ==_T("��ɫ"))
		{ 
			CurrentColor = COLOR_GREEN;
		}
		else if(InfoColor == _T("��ɫ"))
		{
			CurrentColor = COLOR_BLUE;
		}
		else
		{
			CurrentColor = COLOR_BLACK;
		} 
		CPen pen(PS_SOLID,2,CurrentColor);
		pDisplayWndMemDC->SelectObject(&pen); 

		//���ַ�
		int x = InfoPos.x * DisplayWndRect.Width()/100;
		int y = InfoPos.y * DisplayWndRect.Height()/100;  
		pDisplayWndMemDC->TextOut(x,y,InfoShow); 
		m_pInterface->Ctrl_ShowMemDC(CamID);
	}
	
	if(bDelay) Sleep(DelayTime);
	return 0;
} 