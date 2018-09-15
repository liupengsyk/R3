#include "StdAfx.h"
#include "POG_TestItem.h"
#include "ImageProc.h"


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new POG_TestItem(pInterface, lpszName);
}


/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
POG_TestItem::POG_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;
	m_cfgName = CString(lpszName);
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

POG_TestItem::~POG_TestItem(void)
{

}


int POG_TestItem::InitItem()
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
int POG_TestItem::Initialize()
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
int POG_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
	CString tmpStr = _T("");
	img = * m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.RGB24_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ץ֡1ʧ��!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	memset(&m_blemishInfo, 0, sizeof(BlemishInfo));

	HINSTANCE hInstance = LoadLibrary(_T("OfBlemishAlgorithm.dll"));
	if (hInstance == NULL)
	{
		FreeLibrary(hInstance);
		m_pInterface->Ctrl_Addlog(CamID,_T("Load OfBlemishAlgorithm.dll ʧ��!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG TEST FAILED!"));
		SetResult(RESULT_FAIL);
		return 0;
	}

	typedef int (*pogTest)(BYTE *SrcBMPBuffer,int iWidth, int iHeight,BlemishInfo *BlemishRes,CamSeletForPOG Cam);
	pogTest fnPogTest = (pogTest)GetProcAddress(hInstance, "fnBlemishAlgorithm");
	if (fnPogTest != NULL)
	{
		CString TempConfig=m_pInterface->Config_GetConfigPathName(CamID);
		CStringA PODConfig,PODKeyName;
		//wcstombs(PODConfig,TempConfig,MAX_COLUMN);
		PODConfig=CT2A(TempConfig);
		PODKeyName=CT2A(m_cfgName);

		int result = 0;
		if (CamID == 0)
			result = fnPogTest(img.RGB24_buffer, img.imgwidth, img.imgheight, &m_blemishInfo,POG_Cam0);
		else
			result = fnPogTest(img.RGB24_buffer, img.imgwidth, img.imgheight, &m_blemishInfo,POG_Cam1);

		if (result == -1)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("�ڴ�����ʧ��,POG TEST FAILED"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG TEST FAILED !!"));
			SetResult(RESULT_FAIL);
		}
		else
		{
			if (result == 1)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("POG TEST PASS"),COLOR_GREEN,200);
				SetResult(RESULT_PASS);
			}
			else
			{
				ShowInfo(m_blemishInfo);
				m_pInterface->Ctrl_Addlog(CamID,_T("POG TEST FAILED"),COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG TEST FAILED !!"));
				SetResult(RESULT_FAIL);
			}
		}		
	}
	else
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("POG DLL LOAD FAILED !!"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POG DLL LOAD FAILED !!"));
		SetResult(RESULT_FAIL);
	}

	FreeLibrary(hInstance);

   return 0;
}


/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int POG_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������


	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int POG_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ����
 	SetConfigPath(GetModulePath() + "\\OfblemishTest.ini");
 	SetName(_T("Spec"));

	pDlg->m_pogCamera0saveImage = ReadConfigInt(_T("bSaveImage"), 0);
	pDlg->m_pogCamera0saveLog = ReadConfigInt(_T("bSaveLog"), 0);

	SetName(_T("Spec_Cam0"));
	pDlg->m_pogCamera0medianA = ReadConfigString(_T("MedianA"), _T("25"));
	pDlg->m_pogCamera0medianB = ReadConfigString(_T("MedianB"), _T(""));
	pDlg->m_pogCamera0axStart = ReadConfigString(_T("ZoneAXStart"), _T(""));
	pDlg->m_pogCamera0ayStart = ReadConfigString(_T("ZoneAYStart"), _T(""));
	pDlg->m_pogCamera0offset = ReadConfigString(_T("offset"), _T(""));
	pDlg->m_pogCamera0cornerDiff = ReadConfigString(_T("CornerDiff"), _T(""));
	pDlg->m_pogCamera0edgeLimit = ReadConfigString(_T("EdgeLimit"), _T(""));
	pDlg->m_pogCamera0threshold = ReadConfigString(_T("Threshold"), _T(""));
	pDlg->m_pogCamera0pixelLimit = ReadConfigString(_T("PixelLimit"), _T(""));
	pDlg->m_pogCamera0axEnd = ReadConfigString(_T("ZoneAXEnd"), _T(""));
	pDlg->m_pogCamera0ayEnd = ReadConfigString(_T("ZoneAYEnd"), _T(""));
	pDlg->m_pogCamera0intensity = ReadConfigString(_T("IntensityCntSpc"), _T(""));
	pDlg->m_pogCamera0rangeSpc = ReadConfigString(_T("RangeSpc"), _T(""));
	pDlg->m_pogCamera0edgePixelLimit = ReadConfigString(_T("EdgePixelLimit"), _T(""));

	SetName(_T("Spec_Cam1"));
	pDlg->m_pogCamera1medianA = ReadConfigString(_T("MedianA"), _T("25"));
	pDlg->m_pogCamera1medianB = ReadConfigString(_T("MedianB"), _T(""));
	pDlg->m_pogCamera1axStart = ReadConfigString(_T("ZoneAXStart"), _T(""));
	pDlg->m_pogCamera1ayStart = ReadConfigString(_T("ZoneAYStart"), _T(""));
	pDlg->m_pogCamera1offset = ReadConfigString(_T("offset"), _T(""));
	pDlg->m_pogCamera1cornerDiff = ReadConfigString(_T("CornerDiff"), _T(""));
	pDlg->m_pogCamera1edgeLimit = ReadConfigString(_T("EdgeLimit"), _T(""));
	pDlg->m_pogCamera1threshold = ReadConfigString(_T("Threshold"), _T(""));
	pDlg->m_pogCamera1pixelLimit = ReadConfigString(_T("PixelLimit"), _T(""));
	pDlg->m_pogCamera1axEnd = ReadConfigString(_T("ZoneAXEnd"), _T(""));
	pDlg->m_pogCamera1ayEnd = ReadConfigString(_T("ZoneAYEnd"), _T(""));
	pDlg->m_pogCamera1intensity = ReadConfigString(_T("IntensityCntSpc"), _T(""));
	pDlg->m_pogCamera1rangeSpc = ReadConfigString(_T("RangeSpc"), _T(""));
	pDlg->m_pogCamera1edgePixelLimit = ReadConfigString(_T("EdgePixelLimit"), _T(""));

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	SetName(m_cfgName);

	pDlg->m_pogCamera0Enable = ReadConfigInt(_T("POG_CAMERA0_ENABLE"), 0);

	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int POG_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����
	SetConfigPath(GetModulePath() + "\\OfblemishTest.ini");
	SetName(_T("Spec"));

	WriteConfigInt(_T("bSaveImage"), pDlg->m_pogCamera0saveImage, 0);
	WriteConfigInt(_T("bSaveLog"), pDlg->m_pogCamera0saveLog, 0);

	SetName(_T("Spec_Cam0"));
	WriteConfigString(_T("MedianA"), pDlg->m_pogCamera0medianA);
	WriteConfigString(_T("MedianB"), pDlg->m_pogCamera0medianB);
	WriteConfigString(_T("ZoneAXStart"), pDlg->m_pogCamera0axStart);
	WriteConfigString(_T("ZoneAYStart"), pDlg->m_pogCamera0ayStart);
	WriteConfigString(_T("offset"), pDlg->m_pogCamera0offset);
	WriteConfigString(_T("CornerDiff"), pDlg->m_pogCamera0cornerDiff);
	WriteConfigString(_T("EdgeLimit"), pDlg->m_pogCamera0edgeLimit);
	WriteConfigString(_T("Threshold"), pDlg->m_pogCamera0threshold);
	WriteConfigString(_T("PixelLimit"), pDlg->m_pogCamera0pixelLimit);
	WriteConfigString(_T("ZoneAXEnd"), pDlg->m_pogCamera0axEnd);
	WriteConfigString(_T("ZoneAYEnd"), pDlg->m_pogCamera0ayEnd);
	WriteConfigString(_T("IntensityCntSpc"), pDlg->m_pogCamera0intensity);
	WriteConfigString(_T("RangeSpc"), pDlg->m_pogCamera0rangeSpc);
	WriteConfigString(_T("EdgePixelLimit"), pDlg->m_pogCamera0edgePixelLimit);

	SetName(_T("Spec_Cam1"));
	WriteConfigString(_T("MedianA"), pDlg->m_pogCamera1medianA);
	WriteConfigString(_T("MedianB"), pDlg->m_pogCamera1medianB);
	WriteConfigString(_T("ZoneAXStart"), pDlg->m_pogCamera1axStart);
	WriteConfigString(_T("ZoneAYStart"), pDlg->m_pogCamera1ayStart);
	WriteConfigString(_T("offset"), pDlg->m_pogCamera1offset);
	WriteConfigString(_T("CornerDiff"), pDlg->m_pogCamera1cornerDiff);
	WriteConfigString(_T("EdgeLimit"), pDlg->m_pogCamera1edgeLimit);
	WriteConfigString(_T("Threshold"), pDlg->m_pogCamera1threshold);
	WriteConfigString(_T("PixelLimit"), pDlg->m_pogCamera1pixelLimit);
	WriteConfigString(_T("ZoneAXEnd"), pDlg->m_pogCamera1axEnd);
	WriteConfigString(_T("ZoneAYEnd"), pDlg->m_pogCamera1ayEnd);
	WriteConfigString(_T("IntensityCntSpc"), pDlg->m_pogCamera1intensity);
	WriteConfigString(_T("RangeSpc"), pDlg->m_pogCamera1rangeSpc);
	WriteConfigString(_T("EdgePixelLimit"), pDlg->m_pogCamera1edgePixelLimit);

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	SetName(m_cfgName);

	WriteConfigInt(_T("POG_CAMERA0_ENABLE"), pDlg->m_pogCamera0Enable);
	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR POG_TestItem::GetReportHeads()
{
	static CString header;

	header = _T("POG COUNT");

	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR POG_TestItem::GetReportLowLimit()
{
	static CString LowerLimit;
	LowerLimit = _T("0");
	return LowerLimit;
}


/******************************************************************
������:    GetReportUpperLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR POG_TestItem::GetReportHightLimit()
{
	static CString UpperLimit;
	UpperLimit = _T("0");
	return UpperLimit;
}


/******************************************************************
������:    GetReportContents
��������:  �����Ӧ������������Ĳ��Ե�����
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR POG_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");

	CString temp = _T("");
	temp.Format(_T("%d"), m_blemishInfo.POGCount);

	Content += temp;

	return Content;
}

CString POG_TestItem::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	return ret;
}

void POG_TestItem::ShowInfo(BlemishInfo blemishInfo)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//�õ�ϵͳĬ������
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//��ʼ��
	CDC* pDisplayWndMemDC = m_pInterface->Ctrl_GetDisplayWndMemDC(CamID);
	pDisplayWndMemDC->SelectObject(&ft);	
	pDisplayWndMemDC->SelectStockObject(NULL_BRUSH);
	pDisplayWndMemDC->SetBkMode(TRANSPARENT);
	pDisplayWndMemDC->SetTextColor(RGB(150,255,255)); 

	COLORREF CurrentColor = COLOR_YELLOW;

	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID); 
	int ImgWidth  = img.imgwidth;
	int ImgHeight = img.imgheight;
	double Xratio = 1.0*DisplayWndRect.Width()/ImgWidth;
	double Yratio = 1.0*DisplayWndRect.Height()/ImgHeight;

	//if (bParallel)
	{
		for (UINT i=0; i<blemishInfo.POGCount; i++)
		{
			CurrentColor = COLOR_RED;

			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			int cx1,cy1, cx2, cy2;
			cx1 = blemishInfo.xStart[i];
			cy1 = blemishInfo.yStart[i];
			int px1 = (int)(cx1* Xratio);
			int py1 = (int)(cy1* Yratio);

			cx2 = blemishInfo.xEnd[i];
			cy2 = blemishInfo.yEnd[i];
			int px2 = (int)(cx2* Xratio);
			int py2 = (int)(cy2* Yratio);
			BOOL bShowCen = TRUE;
			if(bShowCen)
				pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		}
	} 
	m_pInterface->Ctrl_ShowMemDC(CamID);
}