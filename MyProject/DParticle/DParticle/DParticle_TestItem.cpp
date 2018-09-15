#include "StdAfx.h"
#include "DParticle_TestItem.h"
#include "ImageProc.h"

#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */


extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new DParticle_TestItem(pInterface, lpszName);
}

/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
DParticle_TestItem::DParticle_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

BOOL bSaveRawFile( char* sfilename, BYTE *pBuffer, DWORD BufSize)
{	

	if(BufSize<=0) return FALSE;

	HFILE rawFile;
	DWORD dwSizeImage = BufSize;

	rawFile = _lcreat( sfilename, 0 );
	if( rawFile < 0 )
		return FALSE;

	UINT len;
	len = _lwrite( rawFile, (LPSTR)pBuffer, dwSizeImage );

	_lclose( rawFile );

	return TRUE;	
}

DParticle_TestItem::~DParticle_TestItem(void)
{

}


int DParticle_TestItem::InitItem()
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
int DParticle_TestItem::Initialize()
{
	TestItemInterface::Initialize();
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	//TODO:�ڴ���ӳ�ʼ������
	CString path=m_pInterface->Ctrl_GetCurrentPath();
	path=path+L"OfDPaticleTest.ini";
	SetConfigPath(path);	 //����CONFIG·��	
	SetName(L"Spec");

	WriteConfigInt(_T("LeftSL"), pDlg->nLeftSL, 0);
	WriteConfigInt(_T("RightSL"), pDlg->nRightSL, 0);
	WriteConfigInt(_T("TopSL"), pDlg->nTopSL, 0);
	WriteConfigInt(_T("BottomSL"), pDlg->nBottomSL, 0);

	WriteConfigInt(_T("AreaHeight"), pDlg->nAreaHeight, 0);
	WriteConfigInt(_T("AreaWidth"), pDlg->nAreaWidth, 0);
	WriteConfigInt(_T("DeadSpec"), pDlg->nDeadSpec, 0);
	WriteConfigInt(_T("DeadType"), pDlg->nDeadType, 0);
	WriteConfigInt(_T("DeadUnit"), pDlg->nDeadUnit, 0);

	path=m_pInterface->Ctrl_GetCurrentPath();
	path=path+L"SensorTab\\init.ini";
	SetConfigPath(path);	 //����CONFIG·��	
	SetName(L"ErrorCode");
	CString temp = _T("");
	temp.Format(_T("CAM%d_ErrorCode"),CamID);
	WriteConfigInt(temp, pDlg->ErrorCode); 

	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	SetName(m_cfgName);
  
	interval=0;

	m_pInterface->Sensor_ReadGain(CamID,preGain);

	return 0;
}



/******************************************************************
������:    Testing
��������:  ���ĳһ����Թ��ܴ�������
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int DParticle_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
  if (	interval==0)
  {
	  	m_pInterface->Sensor_WriteExp(CamID,pDlg->nDefultExposure);
		m_pInterface->Sensor_WriteGain(CamID,pDlg->nDefultGain);	
	    interval++;
		return 1;
  }
  else if (interval > 3)
  {
		WhiteSpotResult.DfcStk = (WPixInfo*) new WPixInfo[10000];
		WhiteSpotResult.DefectStackCnt = 0;
		WhiteSpotResult.DeadNum = 0; 			
		
		for(  int k = 0; k < 10000; k++)
		{
			WhiteSpotResult.DfcStk[k].xIndex = 0;
			WhiteSpotResult.DfcStk[k].Y_Value = 0;
			WhiteSpotResult.DfcStk[k].yIndex = 0;
		}

		img = * m_pInterface->Ctrl_GetImgInfo(CamID);

		int wspotresult=0;
		BYTE* raw8Bytes = new BYTE[img.imgwidth * img.imgheight];

		for (int y =0; y < img.imgheight; y++)
		{
			for(int x =0; x < img.imgwidth; x++)
			{
				raw8Bytes[y*img.imgwidth + x] = (img.Raw_buffer[y*img.imgwidth + x] >>2 ) & 0xff;
			}
		}

		wspotresult = WhiteSpotTest(raw8Bytes, img.imgwidth,img.imgheight,img.RawFormat,&WhiteSpotResult);

		if (wspotresult == 0)
		{
			CString log;
			log.Format(_T("DPaticle Count: %d"),WhiteSpotResult.DefectStackCnt);
			m_pInterface->Ctrl_Addlog(CamID,log,COLOR_GREEN,200);
			ShowInfo();

			if (pDlg->bSaveImage)
			{
				char filename[1024]  = {0};
				CString temppath = _T("");
				temppath = GetModulePath();
				temppath += _T("\\\\");
				temppath += _T("DPaticle_Image");
				CreateDirectory(temppath,NULL);
				temppath += _T("\\\\");
				temppath += m_pInterface->Sensor_GetCamFuseID(CamID);
				temppath += _T(".raw") ;

				WideCharToMultiByte(CP_ACP,0,temppath,(temppath.GetLength()+1), filename, 1024, NULL, NULL);
				//		sprintf(filename,"%s",temppath.GetBuffer(temppath.GetLength()));
				bSaveRawFile(filename,raw8Bytes,img.imgwidth*img.imgheight);
			}

			m_pInterface->Ctrl_Addlog(CamID,_T("POD TEST FAILED"),COLOR_RED,200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("POD����ʧ�ܣ�"));
			SetResult(RESULT_FAIL);		 
		}
		else
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("DPaticle TEST PASS"),COLOR_GREEN,200);
			SetResult(RESULT_PASS);
		}
	   delete [] raw8Bytes;
	   if (WhiteSpotResult.DfcStk != NULL)
	   {
		   delete[] WhiteSpotResult.DfcStk;
		   WhiteSpotResult.DfcStk = NULL;
	   }
  }
  else
  {
	  interval++;
	  return 1;
  }

   return 0;
}

void DParticle_TestItem::ShowInfo()
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
		for (UINT i=0; i< WhiteSpotResult.DefectStackCnt; i++)
		{
			CurrentColor = COLOR_RED;

			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			int cx1,cy1;
			cx1 = (int)(WhiteSpotResult.DfcStk[i].xIndex * Xratio);
			cy1 = (int)((img.imgheight - WhiteSpotResult.DfcStk[i].yIndex) * Yratio);
			int px1 = cx1 - 10;
			int py1 = cy1 - 10;
			int px2 = cx1 + 10;
			int py2 = cy1 + 10;

	
			BOOL bShowCen = TRUE;
			if(bShowCen)
				pDisplayWndMemDC->Rectangle(px1, py1, px2, py2);
		}
	} 
	m_pInterface->Ctrl_ShowMemDC(CamID);
}
/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int DParticle_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������

	m_pInterface->Sensor_WriteGain(CamID,preGain);

	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int DParticle_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	pDlg->ErrorCode =ReadConfigInt(_T("ErrorCode"),16);
	pDlg->nAreaHeight=ReadConfigInt(_T("AreaHeight"),16);
	pDlg->nAreaWidth=ReadConfigInt(_T("AreaWidth"),16);
	pDlg->nBottomSL=ReadConfigInt(_T("BottomSL"),16);
	pDlg->nTopSL=ReadConfigInt(_T("TopSL"),16);
	pDlg->nLeftSL=ReadConfigInt(_T("LeftSL"),16);
	pDlg->nRightSL=ReadConfigInt(_T("RightSL"),16);

	pDlg->nDeadSpec=ReadConfigInt(_T("DeadSpec"),16);
	pDlg->nDeadType=ReadConfigInt(_T("DeadType"),16);
	pDlg->nDeadUnit =ReadConfigInt(_T("DeadUnit"),16);
	pDlg->nDefultExposure=ReadConfigInt(_T("DefultExposure"),16);
	pDlg->nDefultGain=ReadConfigInt(_T("DefultGain"),16);
	pDlg->bSaveImage=ReadConfigInt(_T("bSaveImage"),16);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int DParticle_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	   
	WriteConfigInt(_T("ErrorCode"),pDlg->ErrorCode);
	WriteConfigInt(_T("AreaHeight"),pDlg->nAreaHeight);
	WriteConfigInt(_T("AreaWidth"),pDlg->nAreaWidth);
	WriteConfigInt(_T("BottomSL"),pDlg->nBottomSL);
	WriteConfigInt(_T("TopSL"),pDlg->nTopSL);
	WriteConfigInt(_T("LeftSL"),pDlg->nLeftSL);
	WriteConfigInt(_T("RightSL"),pDlg->nRightSL);

	WriteConfigInt(_T("DeadSpec"),pDlg->nDeadSpec);
	WriteConfigInt(_T("DeadType"),pDlg->nDeadType);
	WriteConfigInt(_T("DeadUnit"),pDlg->nDeadUnit);
	WriteConfigInt(_T("DefultExposure"),pDlg->nDefultExposure);
	WriteConfigInt(_T("DefultGain"),pDlg->nDefultGain);
	WriteConfigInt(_T("bSaveImage"),pDlg->bSaveImage);
	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR DParticle_TestItem::GetReportHeads()
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
LPCTSTR DParticle_TestItem::GetReportLowLimit()
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
LPCTSTR DParticle_TestItem::GetReportHightLimit()
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
LPCTSTR DParticle_TestItem::GetReportContents()
{
	static CString Content;
	Content = _T("");
	return Content;
}

CString DParticle_TestItem::GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	ret.Replace(_T("\\"),_T("\\\\"));

	return ret;
}
