#include "StdAfx.h"
#include "OC_Item_TestItem.h"
#include "ImageProc.h"
#include "OPCenterThreshold.h"	


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */
#pragma comment(lib,"OPCenterThreshold.lib")

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new OC_Item_TestItem(pInterface, lpszName);
}


/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
OC_Item_TestItem::OC_Item_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

OC_Item_TestItem::~OC_Item_TestItem(void)
{

}


int OC_Item_TestItem::InitItem()
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
int OC_Item_TestItem::Initialize()
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
int OC_Item_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
	

		if (!OCTest())
		{
			SetResult(RESULT_FAIL);
		}
		else
			SetResult(RESULT_PASS);

		if (pDlg->m_Continue)
		{
			return 1;
		}
		else 
			return 0;
}
void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}
BOOL OC_Item_TestItem::OCTest()
{
	m_pInterface->Ctrl_Addlog(CamID,_T("**Get Optical Center**"),COLOR_BLUE,200);

	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID); 

	int  width   = img.imgwidth;
	int  height   =   img.imgwidth;
	BYTE * m_pDisplayBuffer=new BYTE[width*height];
	Raw10toRaw8(img.Raw_buffer,m_pDisplayBuffer,width*height);
	OFOPTCenterIn CenterIn;
	CenterIn.ImageBufferIn = m_pDisplayBuffer;
	CenterIn.Width = img.imgwidth;
	CenterIn.Height = img.imgheight;
	CenterIn.Outformat = img.RawFormat;

	UINT nSize = img.imgwidth * img.imgheight/4;
	LPBYTE  m_pImageBuffer = (LPBYTE)malloc(nSize);
	memset(m_pImageBuffer,0,nSize);

	OpticalCenterTest(&CenterIn,m_pImageBuffer,CenterX,CenterY);

	int mechanicalCenterX = img.imgwidth/2;
	int mechanicalCenterY = img.imgheight/2;

	CString TempMsg;
	TempMsg.Format(_T("%d,%d"),CenterX,CenterY);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

	if(CenterX>mechanicalCenterX)
	{
		if ((CenterX-mechanicalCenterX) > OCSpecX_hig)
		{
			if (pDlg->m_OCSpec_Shift)
			{
				if ((CenterX-mechanicalCenterX) < pDlg->m_OCSpec_x_AfterShift)
				{
					CenterX = 1.0*CenterX*OCSpecX_hig/pDlg->m_OCSpec_x_AfterShift;
					if ((CenterX-mechanicalCenterX)>OCSpecX_hig)
					{
						m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x ����NG!"),COLOR_RED,200);
						delete m_pDisplayBuffer;
						free(m_pImageBuffer);
						return 0;
					}
				}
				else
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x ����NG!"),COLOR_RED,200);
					delete m_pDisplayBuffer;
					free(m_pImageBuffer);
					return 0;
				}
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x ����NG!"),COLOR_RED,200);
				delete m_pDisplayBuffer;
				free(m_pImageBuffer);
				return 0;
			}
		}
	}
	if (CenterX<=mechanicalCenterX)
	{
		if ((mechanicalCenterX-CenterX) > OCSpecX_hig)
		{
			if (pDlg->m_OCSpec_Shift)
			{
				if ((mechanicalCenterX-CenterX)<pDlg->m_OCSpec_x_AfterShift)
				{
					CenterX = 1.0*CenterX*pDlg->m_OCSpec_x_AfterShift/OCSpecX_hig;
					if ((mechanicalCenterX-CenterX) > OCSpecX_hig)
					{
						m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x ����NG!"),COLOR_RED,200);
						delete m_pDisplayBuffer;
						free(m_pImageBuffer);
						return 0;
					}
				}
				else
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x ����NG!"),COLOR_RED,200);
					delete m_pDisplayBuffer;
					free(m_pImageBuffer);
					return 0;
				}
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("OC shift x ����NG!"),COLOR_RED,200);
				delete m_pDisplayBuffer;
				free(m_pImageBuffer);
				return 0;
			}
			
		}
	}
	if(CenterY>mechanicalCenterY)
	{
		if ((CenterY-mechanicalCenterY) > OCSpecY_hig)
		{
			if (pDlg->m_OCSpec_Shift)
			{
				if ((CenterY-mechanicalCenterY) < pDlg->m_OCSpec_y_AfterShift)
				{
					CenterY = 1.0*CenterY*OCSpecY_hig/pDlg->m_OCSpec_y_AfterShift;
					if ((CenterY-mechanicalCenterY) > OCSpecY_hig)
					{
						m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y ����NG!"),COLOR_RED,200);
						delete m_pDisplayBuffer;
						free(m_pImageBuffer);
						return 0;
					}
				}
				else
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y ����NG!"),COLOR_RED,200);
					delete m_pDisplayBuffer;
					free(m_pImageBuffer);
					return 0;
				}
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y ����NG!"),COLOR_RED,200);
				delete m_pDisplayBuffer;
				free(m_pImageBuffer);
				return 0;
			}
			
		}
	}
	if(CenterY<=mechanicalCenterY)
	{
		if ((mechanicalCenterY-CenterY) > OCSpecY_hig)
		{
			if (pDlg->m_OCSpec_Shift)
			{
				if ((mechanicalCenterY-CenterY) < pDlg->m_OCSpec_y_AfterShift)
				{
					CenterY = 1.0*CenterY*pDlg->m_OCSpec_y_AfterShift/OCSpecY_hig;
					if ((mechanicalCenterY-CenterY) > OCSpecY_hig)
					{
						m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y ����NG!"),COLOR_RED,200);
						delete m_pDisplayBuffer;
						free(m_pImageBuffer);
						return 0;
					}
				}
				else
				{
					m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y ����NG!"),COLOR_RED,200);
					delete m_pDisplayBuffer;
					free(m_pImageBuffer);
					return 0;
				}
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("OC shift y ����NG!"),COLOR_RED,200);
				delete m_pDisplayBuffer;
				free(m_pImageBuffer);
				return 0;
			}
			
		}
	}
	m_pInterface->Ctrl_Addlog(CamID,_T("OC ���� PASS !"),COLOR_BLUE,200);
	delete m_pDisplayBuffer;
	return 1;
}

/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int OC_Item_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������

	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int OC_Item_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 

	OCSpecX_hig=ReadConfigInt(_T("OCSpecX_hig"),100);
	OCSpecY_hig=ReadConfigInt(_T("OCSpecY_hig"),100);

	pDlg->m_OCSpec_Shift = ReadConfigInt(_T("OCSpec_Shift"),0);
	pDlg->m_OCSpec_x_AfterShift = ReadConfigInt(_T("OCSpec_x_AfterShift"),100);
	pDlg->m_OCSpec_y_AfterShift = ReadConfigInt(_T("OCSpec_y_AfterShift"),100);
	

	pDlg->m_Continue=ReadConfigInt(_T("m_Continue"),0);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int OC_Item_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   

	WriteConfigInt(_T("OCSpecX_hig"),pDlg->m_OCSpecX_hig);
	WriteConfigInt(_T("OCSpecY_hig"),pDlg->m_OCSpecY_hig);
	WriteConfigInt(_T("OCSpec_Shift"),pDlg->m_OCSpec_Shift);
	WriteConfigInt(_T("OCSpec_x_AfterShift"),pDlg->m_OCSpec_x_AfterShift);
	WriteConfigInt(_T("OCSpec_y_AfterShift"),pDlg->m_OCSpec_y_AfterShift);

	WriteConfigInt(_T("m_Continue"),pDlg->m_Continue);
	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OC_Item_TestItem::GetReportHeads()
{
	static CString header;
	header = _T("ģ����,OptiX,OptiY");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OC_Item_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
//	LowerLimit.Format(_T("%d,%d"),OCSpecX_low,OCSpecY_low);
	return LowerLimit;
}


/******************************************************************
������:    GetReportUpperLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OC_Item_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
//	UpperLimit.Format(_T("%d,%d"),OCSpecX_hig,OCSpecY_hig);
	return UpperLimit;
}


/******************************************************************
������:    GetReportContents
��������:  �����Ӧ������������Ĳ��Ե�����
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OC_Item_TestItem::GetReportContents()
{
	static CString Content;
	CString CamCode=m_pInterface->Ctrl_GetCamCode();
	Content.Format(_T("%s,%d,%d"),CamCode,CenterX,CenterY);
	return Content;
}

