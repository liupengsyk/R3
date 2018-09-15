#include "StdAfx.h"
#include "Inspection_Shading.h"
#include "ImageProc.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef TCCM_EXP
#define TCCM_EXP	__declspec(dllexport)
#endif	/* TCCM_EXP */

extern "C" TCCM_EXP Inspection_Shading*  CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new Inspection_Shading(pInterface, lpszName);
}


Inspection_Shading::Inspection_Shading(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;

	SetName(lpszName);	                             //���ò�����Ŀ������
	pDlg = new ShadingOptionDlg(NULL, this);         //���ɶԻ������
	                                  //��ȡ����
	pDlg->Create(ShadingOptionDlg::IDD, NULL);       //�����Ի��򴰿�
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //�õ��Ի�����

	//TODO: ���ò�����Ŀ������ 
	SetType(TYPE_IMAGE);

	//�������Ա�����ʼ��
	Lock_OneTime = FALSE;

}

Inspection_Shading::~Inspection_Shading(void)
{

}

int Inspection_Shading::InitItem()
{
	CamID = GetCamID();
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //����CONFIG·��	
	LoadOption();

	CString CurrentName = GetName();
	SetName(_T("WBSetting"));

	m_strTestStation = ReadConfigString(_T("TestStation"), _T("XXXXX"));

	SetName(CurrentName);

	return 0;
}

int Inspection_Shading::Initialize()
{
	TestItemInterface::Initialize();
	//TODO 
	SetbStopTest(FALSE);
	CamCode = m_pInterface->Ctrl_GetCamCode();
	if (!Lock_OneTime)
	{
		Lock_OneTime = TRUE;

		//m_pInterface->ShowOriginalImage();
	}

	img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ץ֡1ʧ��!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
		return -1;
	}
	Y_Buffer = new BYTE[img.imgwidth*img.imgheight];
	if (NULL == Y_Buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Y_Buffer ����ʧ��!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
		return -1;
	}
	for(int col=0;col<5;col++)
	{
		Rgain[col]=0.0;
		Bgain[col]=0.0;
	}
	
	return 0;
}

int Inspection_Shading::Testing()
{
	Lock_OneTime = FALSE;

	if ( ShadingTest() == 0 ) 
	{
		return  0;
	}
	else
	{
		return -1;
	}
}

int Inspection_Shading::Finalize()
{
	Lock_OneTime = FALSE;
	SAFE_DELETE(Y_Buffer);
	return TestItemInterface::Finalize();
}


int Inspection_Shading::LoadOption()
{
	sShading.ROI[0].cx = ReadConfigInt(_T("ROI0_x"),50);
    sShading.ROI[0].cy = ReadConfigInt(_T("ROI0_y"),50);
	sShading.ROI[0].Enable = ReadConfigInt(_T("ROI0_En"),1);

	sShading.ROI[1].cx = ReadConfigInt(_T("ROI1_x"),5);
	sShading.ROI[1].cy = ReadConfigInt(_T("ROI1_y"),5);
	sShading.ROI[1].Enable = ReadConfigInt(_T("ROI1_En"),1);

	sShading.ROI[2].cx = ReadConfigInt(_T("ROI2_x"),5);
	sShading.ROI[2].cy = ReadConfigInt(_T("ROI2_y"),95);
	sShading.ROI[2].Enable = ReadConfigInt(_T("ROI2_En"),1);

	sShading.ROI[3].cx = ReadConfigInt(_T("ROI3_x"),95);
	sShading.ROI[3].cy = ReadConfigInt(_T("ROI3_y"),5);
	sShading.ROI[3].Enable = ReadConfigInt(_T("ROI3_En"),1);

	sShading.ROI[4].cx = ReadConfigInt(_T("ROI4_x"),95);
	sShading.ROI[4].cy = ReadConfigInt(_T("ROI4_y"),95);
    sShading.ROI[4].Enable = ReadConfigInt(_T("ROI4_En"),1);

	sShading.ROI_height   = ReadConfigInt(_T("ROI_height"),10);
	sShading.ROI_width    = ReadConfigInt(_T("ROI_width"),10);

	pDlg->m_Continue      = ReadConfigInt(_T("Continue"), 0);
	pDlg->m_bSavShadingDataPerBlock      = ReadConfigInt(_T("SavShadingDataPerBlock"), 0);

	pDlg->BLC=ReadConfigInt(_T("BLC"),16);	

	pDlg->m_RGMax=ReadConfigInt(_T("m_RGMax"),0);	
	pDlg->m_RGMin=ReadConfigInt(_T("m_RGMin"),0);	
	pDlg->m_BGMax=ReadConfigInt(_T("m_BGMax"),0);	
	pDlg->m_BGMin=ReadConfigInt(_T("m_BGMin"),0);	
	pDlg->m_YMax=ReadConfigInt(_T("m_YMax"),0);	
	pDlg->m_Ymin=ReadConfigInt(_T("m_Ymin"),0);	
	pDlg->m_YDiff=ReadConfigInt(_T("m_YDiff"),0);

	return TestItemInterface::LoadOption();
}

int Inspection_Shading::SaveOption()
{
	TestItemInterface::SaveOption();

	for(int i=0;i<5;i++)
	{
		CString KeyName;
		KeyName.Format(_T("ROI%d_x"),i);
		WriteConfigInt(KeyName,sShading.ROI[i].cx);

		KeyName.Format(_T("ROI%d_y"),i);
		WriteConfigInt(KeyName,sShading.ROI[i].cy);

		KeyName.Format(_T("ROI%d_Enable"),i);
		WriteConfigInt(KeyName,sShading.ROI[i].Enable);
	}



	WriteConfigInt(_T("ROI_height      "),sShading.ROI_height);
	WriteConfigInt(_T("ROI_width       "),sShading.ROI_width);


	WriteConfigInt(_T("Continue"), pDlg->m_Continue);
	WriteConfigInt(_T("SavShadingDataPerBlock"), pDlg->m_bSavShadingDataPerBlock);
	WriteConfigInt(_T("BLC"), pDlg->BLC);


	WriteConfigInt(_T("m_RGMax"),pDlg->m_RGMax);	
	WriteConfigInt(_T("m_RGMin"),pDlg->m_RGMin);	
	WriteConfigInt(_T("m_BGMax"),pDlg->m_BGMax);	
	WriteConfigInt(_T("m_BGMin"),pDlg->m_BGMin);	
	WriteConfigInt(_T("m_YMax"),pDlg->m_YMax);	
	WriteConfigInt(_T("m_Ymin"),pDlg->m_Ymin);	
	WriteConfigInt(_T("m_YDiff"),pDlg->m_YDiff);	
	return 0;
}


LPCTSTR Inspection_Shading::GetReportHeads()
{
	// TODO : Make report headers of this inspection and return that
	static CString header;
	header = _T("");
	CString temp;


	header += _T("Y_Blk0,Y_Blk1,Y_Blk2,Y_Blk3,Y_Blk4,");


	for (int col = 0; col < 5; ++col)
	{
		temp.Format(_T("B_%d-R/G,B_%d-B/G,"),col,col);
		header += temp;
	}
	for (int col = 0; col < 5; ++col)
	{
		temp.Format(_T("B_%d-R,B_%d-Gr,B_%d-Gb,B_%d-B,"),col,col,col,col);
		header += temp;
	}
	temp.Format(_T("Center_R/G,Center_B/G,TL_R/G,TL_B/G,BL_R/G,BL_B/G,TR_R/G,TR_B/G,BR_R/G,BR_B/G,"));
	header += temp;
	return header;
}

LPCTSTR Inspection_Shading::GetReportLowLimit()
{
	// TODO : Make report lower limits of this inspection and return that
	static CString LowerLimits;
	LowerLimits = _T("");
	return LowerLimits;
}

LPCTSTR Inspection_Shading::GetReportHightLimit()
{
	// TODO : Make report upper limits of this inspection and return that
	static CString UpperLimits;
	UpperLimits = _T("");
	return UpperLimits;
}

LPCTSTR Inspection_Shading::GetReportContents()
{
	// TODO : Make report contents of this inspection and return that
	static CString Contents;
	Contents = _T("");

	CString temp;
	for (int i = 0; i < 5; i++)
	{
		temp.Format(_T("%.4f,"),Y[i]);
		Contents += temp;
	}
	for (int col = 0; col < 5; col++)
	{
		temp.Format(_T("%.4f,%.4f,"),ColorRGain[col],ColorBGain[col]);
		Contents += temp;
	}
	for (int col = 0; col < 5; col++)
	{
		temp.Format(_T("%d,%d,%d,%d,"),sShading.ROI[col].aveR,sShading.ROI[col].aveGr,sShading.ROI[col].aveGb,sShading.ROI[col].aveB);
		Contents += temp;
	}
	for (int col = 0; col < 5; col++)
	{
		temp.Format(_T("%.4f,%.4f,"),Rgain[col],Bgain[col]);
		Contents += temp;
	}

	return Contents;
}

void Inspection_Shading::CalcImageRect(void)
{
	img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	int width  = img.imgwidth;
	int height = img.imgheight;
//	ImageProc::RGB24toY(img.RGB24_buffer,Y_Buffer,width,height);
	for(int i=0; i<5; i++)
	{
		if(sShading.ROI[i].Enable)
		{
			//����ת��Ϊʵ�����ص�
				//sShading.ROI[i].pos = m_pInterface->Ctrl_CalcRect_DisplayScreen(sShading.ROI[i].cx,	sShading.ROI[i].cy,	sShading.ROI_width,	sShading.ROI_width,CamID);
				//����ת��Ϊʵ�����ص�
				sShading.ROI[i].pos.top    = (int)((sShading.ROI[i].cy - sShading.ROI_height/2.0)  * height/100);
				sShading.ROI[i].pos.bottom = (int)((sShading.ROI[i].cy + sShading.ROI_height/2.0)  * height/100);
				sShading.ROI[i].pos.left   = (int)((sShading.ROI[i].cx - sShading.ROI_width/2.0)   * width/100) ;
				sShading.ROI[i].pos.right  = (int)((sShading.ROI[i].cx + sShading.ROI_width/2.0)   * width/100) ;
				sShading.ROI[i].pos.top =sShading.ROI[i].pos.top /4;
				sShading.ROI[i].pos.top =sShading.ROI[i].pos.top *4;
				sShading.ROI[i].pos.bottom=sShading.ROI[i].pos.bottom/4;
				sShading.ROI[i].pos.bottom=sShading.ROI[i].pos.bottom*4;
				sShading.ROI[i].pos.left =sShading.ROI[i].pos.left /4;
				sShading.ROI[i].pos.left =sShading.ROI[i].pos.left *4;
				sShading.ROI[i].pos.right =sShading.ROI[i].pos.right /4;
				sShading.ROI[i].pos.right =sShading.ROI[i].pos.right *4;


				if(sShading.ROI[i].pos.left < 0)
					sShading.ROI[i].pos.left = 0;
				if(sShading.ROI[i].pos.right >= width)
					sShading.ROI[i].pos.right = width-1;
				if(sShading.ROI[i].pos.top < 0)
					sShading.ROI[i].pos.top = 0;
				if(sShading.ROI[i].pos.bottom >= height)
					sShading.ROI[i].pos.bottom = height-1;
		}
	}
}

int Inspection_Shading::ShadingTest(void)//����Color Shading��Y Shading���ϴ����������Ƿֿ��ģ�R3����ϲ���һ��ע�⣺�����Color Shading���������16384�����˴���512--Added by LiHai--20171209
{
    CalcImageRect();
	ImageInfo img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ץ֡2ʧ��!"),COLOR_RED,200);
		SetResult(RESULT_FAIL);
	}
	int width  = img.imgwidth;
	int height = img.imgheight;

	BYTE* RGB24_Buffer = img.RGB24_buffer;
	CString TempMsg;

	//������
	for(int i=0;i<5;i++)
	{
		GetAWBInfoRaw8(i);//5�λ�ȡ��ͼƬ��ʵ��ͬһ��ͼƬ����ȡ5�Σ���������5���ڴ棬�˷�ʱ����ڴ�--Added by LiHai--20171026
	}
	int CenterRG=sShading.ROI[0].RGain;
	int CenterBG=sShading.ROI[0].BGain;
	int CenterY=sShading.ROI[0].aveY;
	//shanding ԭʼֵ
	for(int i=0;i<5;i++)
	{
		Rgain[i]=sShading.ROI[i].RGain/512.0;
		Bgain[i]=sShading.ROI[i].BGain/512.0;
		TempMsg.Format(_T("[Block_0 is Center]:Block_%d, Unit_R/G=%.4f, Unit_B/G=%.4f"),i,Rgain[i],Bgain[i]);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	}
   //
	for(int i=0;i<5;i++)
	{
		ColorRGain[i]=sShading.ROI[i].RGain*1.0/CenterRG;
        ColorBGain[i]=sShading.ROI[i].BGain*1.0/CenterBG;
        Y[i]=sShading.ROI[i].aveY*1.0/CenterY;
		sShading.ROI[i].RGain=(int)(sShading.ROI[i].RGain*100.0/CenterRG);
		sShading.ROI[i].BGain=(int)(sShading.ROI[i].BGain*100.0/CenterBG);
		sShading.ROI[i].aveY=(int)(sShading.ROI[i].aveY*100.0/CenterY);
	}

	int centerR=sShading.ROI[0].aveR;
	int centerGr=sShading.ROI[0].aveGr;
	int centerGb=sShading.ROI[0].aveGb;
	int centerB=sShading.ROI[0].aveB;

	for(int i=0;i<5;i++)
	{
		sShading.ROI[i].aveR=(int)(sShading.ROI[i].aveR*100.0/centerR);
		sShading.ROI[i].aveGr=(int)(sShading.ROI[i].aveGr*100.0/centerGr);
		sShading.ROI[i].aveGb=(int)(sShading.ROI[i].aveGb*100.0/centerGb);
		sShading.ROI[i].aveB=(int)(sShading.ROI[i].aveB*100.0/centerB);
		TempMsg.Format(_T("[Block_0 is Center]:Block_%d, R=%d%%, Gr=%d%%, Gb=%d%%, B=%d%%"),i,sShading.ROI[i].aveR,sShading.ROI[i].aveGr,sShading.ROI[i].aveGb,sShading.ROI[i].aveB);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	}

	int Ymax = 0;
	int Ymin = 255;

	int RGMax = 0;
	int BGMax = 0;

	int RGMin = sShading.ROI[0].RGain;
	int BGMin = sShading.ROI[0].RGain;

	int blockRGMax=0;
	int blockRGMin=0;
	int blockBGMax=0;
	int blockBGMin=0;
	for(int i=0;i<5;i++)
	{
		if (sShading.ROI[i].RGain>RGMax) 
		{
			RGMax=sShading.ROI[i].RGain;
			blockRGMax=i;
		}
		if (sShading.ROI[i].RGain<RGMin) 
		{
			RGMin=sShading.ROI[i].RGain;
			blockRGMin=i;
		}
		if (sShading.ROI[i].BGain>BGMax) 
		{
			BGMax=sShading.ROI[i].BGain;
			blockBGMax=i;
		}

		if (sShading.ROI[i].BGain<BGMin) 
		{
			BGMin=sShading.ROI[i].BGain;
			blockBGMin=i;
		}
	}

	TempMsg.Format(_T("Block_%d maxRG=%d%%   <%d%%, %d%%>"),blockRGMax,RGMax,pDlg->m_RGMin,pDlg->m_RGMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("Block_%d minRG=%d%%   <%d%%, %d%%>"),blockRGMin,RGMin,pDlg->m_RGMin,pDlg->m_RGMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("Block_%d maxBG=%d%%   <%d%%, %d%%>"),blockBGMax,BGMax,pDlg->m_BGMin,pDlg->m_BGMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("Block_%d minBG=%d%%   <%d%%, %d%%>"),blockBGMin,BGMin,pDlg->m_BGMin,pDlg->m_BGMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);



	//�õ����յ�ֵ
	int YMax=0;
	int YMin=sShading.ROI[1].aveY;
	int YDiff;
	for (int i=1;i<5;i++)
	{
		if (sShading.ROI[i].aveY < YMin) 
		{
			YMin=sShading.ROI[i].aveY;
		}
		if (sShading.ROI[i].aveY>YMax) 
		{
			YMax=sShading.ROI[i].aveY;
		}
		YDiff=YMax-YMin;
		if (YDiff>pDlg->m_YDiff)
		{
			sShading.Ydif_result = RESULT_FAIL;
		}
		else
		{
			sShading.Ydif_result = RESULT_PASS;
		}

		TempMsg.Format(_T("Y_Block_%d=%d%%"),i,sShading.ROI[i].aveY);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	}

	//��¼���
	for(int i=1;i<5;i++)
	{	
		//�ж�deltaY�Ƿ񳬱�
		if (sShading.ROI[i].aveY>pDlg->m_YMax||sShading.ROI[i].aveY<pDlg->m_Ymin)
		{
			sShading.ROI[i].deltaY_result = RESULT_FAIL;
		}
		else
		{
			sShading.ROI[i].deltaY_result = RESULT_PASS;
		}

		if (sShading.ROI[i].RGain>pDlg->m_RGMax||sShading.ROI[i].RGain<pDlg->m_RGMin)
		{
			sShading.ROI[i].RG_result = RESULT_FAIL;
		}
		else
		{
			sShading.ROI[i].RG_result = RESULT_PASS;
		}

		if (sShading.ROI[i].BGain>pDlg->m_BGMax||sShading.ROI[i].BGain<pDlg->m_BGMin)
		{
			sShading.ROI[i].BG_result = RESULT_FAIL;
		}
		else
		{
			sShading.ROI[i].BG_result = RESULT_PASS;
		}

		//�����һ�����꣬��ROI������NG��
		if (sShading.ROI[i].BG_result == RESULT_FAIL
			|| sShading.ROI[i].RG_result == RESULT_FAIL
			|| sShading.ROI[i].deltaY_result == RESULT_FAIL
			|| sShading.Ydif_result==RESULT_FAIL
			)
		{
			sShading.ROI[i].result = RESULT_FAIL;
		}
		else
		{
			sShading.ROI[i].result = RESULT_PASS;
		}
	}

	sShading.ROI[0].BG_result = RESULT_PASS; //0 Ĭ��OK
	sShading.ROI[0].RG_result = RESULT_PASS; //0 Ĭ��OK
	sShading.ROI[0].deltaY_result = RESULT_PASS; //0 Ĭ��OK
	sShading.ROI[0].result = RESULT_PASS; //0 Ĭ��OK

	//�����һ��ROI���NG,����������Ŀ���NG
	int FailCount=0;

	for(int i=0; i<5; i++)
	{
		if (sShading.ROI[i].result == RESULT_FAIL )
		{
			FailCount++;
		}
	}

	if(FailCount>0)
	{
		SetResult(RESULT_FAIL);
	}
	else
	{
		SetResult(RESULT_PASS);
	}

 	ShowShadingTest();
	if (TRUE == pDlg->m_bSavShadingDataPerBlock)
	{
		GetRaw10PerBlock();
	}

	if (pDlg->m_Continue)
	{
		return 2;
	}
	else
	{
		if (FailCount>0)
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("Shading ����ʧ��!"));
		}
		return 0;
	}
}

void Inspection_Shading::ShowShadingTest(void)
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

	COLORREF CurrentColor;

	for(int i=0;i<5;i++)
	{
		if(sShading.ROI[i].Enable)
		{
	      if (sShading.ROI[i].result == RESULT_FAIL)
		      CurrentColor = COLOR_RED;
	      else
	          CurrentColor = COLOR_GREEN;

	        CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);

			CRect rect;
			rect.left   =  (sShading.ROI[i].cx - sShading.ROI_width/2)  * DisplayWndRect.Width() /100;
			rect.right  =  (sShading.ROI[i].cx + sShading.ROI_width/2)  * DisplayWndRect.Width() /100;
			rect.top    =  (sShading.ROI[i].cy - sShading.ROI_height/2) * DisplayWndRect.Height()/100;
			rect.bottom =  (sShading.ROI[i].cy + sShading.ROI_height/2) * DisplayWndRect.Height()/100;

			pDisplayWndMemDC->Rectangle(rect);

			//���ַ�
			int x = sShading.ROI[i].cx * DisplayWndRect.Width()/100;
			int y = sShading.ROI[i].cy * DisplayWndRect.Height()/100;

			CString str;
			str.Format(_T("B%d"),i);
			pDisplayWndMemDC->TextOut(x-10,y-10,str);
			 
			str.Format(_T("Roi = %d:aveY = %d,aveR = %d,aveG= %d,aveB = %d"),i,sShading.ROI[i].aveY,sShading.ROI[i].aveR,sShading.ROI[i].aveG,sShading.ROI[i].aveB);
			pDisplayWndMemDC->TextOut(DisplayWndRect.Width()/2-150,DisplayWndRect.Height()/2-120+i*20, str);
		}
	}

	CString result;
	 
	if (GetResult()== RESULT_FAIL)
	{
		CurrentColor = COLOR_RED;
	    result = _T("NG");
	}
	else
	{
		CurrentColor = COLOR_GREEN;
		result = _T("OK");
	}

	CFont font2;
	font2.CreatePointFont(400,_T("Arial"));

	pDisplayWndMemDC->SetTextColor(CurrentColor);
	pDisplayWndMemDC->SelectObject(&font2);

	result = _T("Shading") + result;
	pDisplayWndMemDC->TextOut(
		DisplayWndRect.Width()/2 - 150, 
		DisplayWndRect.Height()/2 + 50, 
		result);

	m_pInterface->Ctrl_Addlog(CamID, result,CurrentColor,200);
	//��ʾDC
	m_pInterface->Ctrl_ShowMemDC(CamID);
}

void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number)
{
	long DesCount = 0;

	for (long i = 0; i < number; i ++)
	{
		Dest[DesCount++] =(Src[i]>>2);
	}
}
void Inspection_Shading::GetAWBInfoRaw8(int block)
{
	ImageInfo img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ץ֡3ʧ��!"),COLOR_RED,200);
		return;
	}

	int  width   = img.imgwidth;
	int  height  = img.imgheight;
	BYTE * Raw8buffer = new BYTE[width * height];
	if (NULL == Raw8buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Raw8buffer ����ʧ��!"), COLOR_RED, 200);
		return;
	}
	Raw10toRaw8(img.Raw_buffer, Raw8buffer, width * height);

	int  StartX = sShading.ROI[block].pos.left;
	int  StartY = sShading.ROI[block].pos.top;
	int  EndX   = sShading.ROI[block].pos.right;
	int  EndY   = sShading.ROI[block].pos.bottom;

	if (EndX > width )
	{
		EndX = width ;
	}
	if (EndY > height )
	{
		EndY = height;
	}

	int x = 0;
	int y = 0;
	unsigned long Count = 0;
	int  Pos1 = 0, Pos2 = 0;

	unsigned long SumGr = 0;
	double Gr = 0.0;

	unsigned long SumGb = 0;
	double Gb = 0.0;

	unsigned long  SumR  = 0;
	double R  = 0.0;

	unsigned long  SumB  = 0;
	double B  = 0.0;


	if ( img.RawFormat== OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumR  += Raw8buffer[Pos1];
				SumGr += Raw8buffer[Pos1 + 1];
				SumGb += Raw8buffer[Pos2];
				SumB  += Raw8buffer[Pos2 + 1];
				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGr += Raw8buffer[Pos1];
				SumR  += Raw8buffer[Pos1 + 1];
				SumB  += Raw8buffer[Pos2];
				SumGb += Raw8buffer[Pos2 + 1]; 

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumGb += Raw8buffer[Pos1];
				SumB  += Raw8buffer[Pos1 + 1];
				SumR  += Raw8buffer[Pos2];
				SumGr += Raw8buffer[Pos2 + 1];

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + width * y;
				Pos2 = x + width * (y + 1);

				SumB  += Raw8buffer[Pos1];
				SumGb += Raw8buffer[Pos1 + 1];
				SumGr += Raw8buffer[Pos2];
				SumR  += Raw8buffer[Pos2 + 1];

				Count++;
			}
		}
	}

	Gr = 1.0 * SumGr / Count;
	Gb = 1.0 * SumGb / Count;
	R  = 1.0 * SumR  / Count;
	B  = 1.0 * SumB  / Count;

	sShading.ROI[block].aveR  = int(R);
	sShading.ROI[block].aveB  = int(B);
	sShading.ROI[block].aveGr = int(Gr);
	sShading.ROI[block].aveGb = int(Gb);
	sShading.ROI[block].aveG  = int((Gr + Gb) / 2.0); 
	sShading.ROI[block].RGain = int( 512.0*(R-pDlg->BLC )/( (Gr+Gb)/2.0-pDlg->BLC) + 0.5 );// Rave/Gave
	sShading.ROI[block].BGain = int( 512.0*(B -pDlg->BLC)/( (Gr+Gb)/2.0-pDlg->BLC) + 0.5 );  //Bave/Gave 

	/*
	*	��YRGB��ʽ������YCbCr��ʽ������Ӧ�ϴ����е�GetYrgbAverage()����.
	*	--Added by LiHai--20171209
	*/
	sShading.ROI[block].aveY = 0.299 * sShading.ROI[block].aveR + 0.587 * sShading.ROI[block].aveG + 0.114 * sShading.ROI[block].aveB;

	SAFE_DELETE_ARRAY(Raw8buffer);
}

void Inspection_Shading::SaveShadingChannelData(_SHADING_INFO_ *pShadingInfo, int Num)
{
	USES_CONVERSION;

	if (NULL == pShadingInfo)
	{
		return;
	}

	int i = 0;
	FILE  *fp  = NULL;
	CTime time = CTime::GetCurrentTime();

	CString RecordTime = _T("");

	CFile MyFile;
	long fileSizeTemp = 0;

	/*_mkdir(T2A(path));
	path = path+_T("Data\\");
	_mkdir(T2A(path));*/
	CString strCurrentPath = _T("D:\\Data\\");
	if (0 == CreateDirectory(strCurrentPath, NULL))
	{
		DWORD dwLastError = GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			return;
		}
	}

	strCurrentPath += m_strTestStation;
	if (0 == CreateDirectory(strCurrentPath, NULL))
	{
		DWORD dwLastError = GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			return;
		}
	}

	strCurrentPath += _T("\\");
	CString FileName = _T("");
	FileName.Format(_T("Block%d_Data_Site_%d_%04d%02d%02d.xls"), Num, m_pInterface->GetSiteNo(), time.GetYear(), time.GetMonth(), time.GetDay());
	FileName = /*ReportPath*/strCurrentPath + FileName;

	m_pInterface->Ctrl_Addlog(CamID, FileName, COLOR_BLUE, 200);

	fp = freopen(CT2A(FileName),"at",stdout);
	if (fp == NULL)  
	{
		CString strLog;

		strLog.Format(_T("%s Error---%s open fail\r\n"), __FUNCTION__, FileName.GetBuffer(FileName.GetLength()));
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		return;
	}

	RecordTime.Format(_T("%02d:%02d:%02d"),time.GetHour(), time.GetMinute(), time.GetSecond());

	fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);
	if(fileSizeTemp==0)
	{
		printf("SN\tFUSE ID\tTest Time\t");
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			int inx = i + 1;
			printf("R_B%d\tGr_B%d\tGb_B%d\tB_B%d\t", inx, inx, inx, inx);
		}
		printf("Result\t");
	}

	printf("\n\t");
	printf(CT2A(m_pInterface->Sensor_GetCamFuseID(CamID)));
	printf("#\t");

	printf(CT2A(RecordTime));
	printf("\t");

	for (i = 0; i < Num; i++)
	{
		printf( "%d\t%d\t%d\t%d\t", pShadingInfo[i].R * 4, pShadingInfo[i].Gr * 4, pShadingInfo[i].Gb * 4, pShadingInfo[i].B * 4);	//�����������Raw8�ģ����ǿͻ�Ҫ���Ǳ���Raw10��
	}

	if (RESULT_PASS == GetResult())
	{
		printf("%s", "PASS");
	}
	else
	{
		printf("%s", "NG");
	}

	printf("\t");

	fclose(fp); 
}

void Inspection_Shading::SaveColorShadingData(float *pRG, float *pBG, int Num)
{
	USES_CONVERSION;

	if ((NULL == pRG) || (NULL == pBG))
	{
		return;
	}

	int i = 0;
	FILE  *fp  = NULL;
	CTime time = CTime::GetCurrentTime();

	CString RecordTime = _T("");

	CFile MyFile;
	long fileSizeTemp = 0;

	/*_mkdir(T2A(path));
	path = path+_T("Data\\");
	_mkdir(T2A(path));*/
	CString strCurrentPath = _T("D:\\Data\\");
	if (0 == CreateDirectory(strCurrentPath, NULL))
	{
		DWORD dwLastError = GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			return;
		}
	}

	strCurrentPath += m_strTestStation;
	if (0 == CreateDirectory(strCurrentPath, NULL))
	{
		DWORD dwLastError = GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			return;
		}
	}

	strCurrentPath += _T("\\");
	CString FileName = _T("");
	FileName.Format(_T("Block%d_ColorShading_Data_Site_%d_%04d%02d%02d.xls"), Num, m_pInterface->GetSiteNo(), time.GetYear(), time.GetMonth(), time.GetDay());
	FileName = /*ReportPath*/strCurrentPath + FileName;

	m_pInterface->Ctrl_Addlog(CamID, FileName, COLOR_BLUE, 200);

	fp = freopen(CT2A(FileName),"at",stdout);
	if (fp == NULL)  
	{
		CString strLog;

		strLog.Format(_T("SaveColorShadingData() Error---%s open fail\r\n"), FileName.GetBuffer(FileName.GetLength()));
		m_pInterface->Ctrl_Addlog(CamID, strLog.GetBuffer(strLog.GetLength()), COLOR_RED, 200);
		return;
	}

	RecordTime.Format(_T("%02d:%02d:%02d"),time.GetHour(), time.GetMinute(), time.GetSecond());

	fileSizeTemp=ftell(fp);
	fseek(fp,0,SEEK_END );
	fileSizeTemp=ftell(fp);
	if(fileSizeTemp==0)
	{
		printf("SN\tFUSE ID\tTest Time\t");
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			printf("R/G_Blk%d\t", i + 1);
		}
		for (int i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
		{
			printf("B/G_Blk%d\t", i + 1);
		}
		printf("Result\t");
	}

	printf("\n\t");
	printf(CT2A(m_pInterface->Sensor_GetCamFuseID(CamID)));
	printf("#\t");

	printf(CT2A(RecordTime));
	printf("\t");

	for (i = 0; i < Num; i++)
	{
		printf( "%f\t", pRG[i]);
	}
	for (i = 0; i < Num; i++)
	{
		printf( "%f\t", pBG[i]);
	}

	if (RESULT_PASS == GetResult())
	{
		printf("%s", "PASS");
	}
	else
	{
		printf("%s", "NG");
	}

	printf("\t");

	fclose(fp); 
}

void Inspection_Shading::GetBlock(BYTE * Raw8buffer, int BlkX, int BlkY, int BlkW, int BlkH, _SHADING_INFO_ *pShadingInfo)
{
	ImageInfo img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if ((NULL == img.Raw_buffer) || (NULL == Raw8buffer) || (NULL == pShadingInfo))
	{
		return;
	}

	int  StartX = BlkX;
	int  StartY = BlkY;
	int  EndX   = BlkX + BlkW - 1;
	int  EndY   = BlkY + BlkH - 1;

	if (EndX > img.imgwidth)
	{
		EndX = img.imgwidth;
	}
	if (EndY > img.imgheight)
	{
		EndY = img.imgheight;
	}

	int x = 0;
	int y = 0;
	unsigned long Count = 0;
	int  Pos1 = 0, Pos2 = 0;

	unsigned long SumGr = 0;
	double Gr = 0.0;

	unsigned long SumGb = 0;
	double Gb = 0.0;

	unsigned long  SumR  = 0;
	double R  = 0.0;

	unsigned long  SumB  = 0;
	double B  = 0.0;


	if ( img.RawFormat == OUTFORMAT_RGGB)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + img.imgwidth * y;
				Pos2 = x + img.imgwidth * (y + 1);

				SumR  += Raw8buffer[Pos1];
				SumGr += Raw8buffer[Pos1 + 1];
				SumGb += Raw8buffer[Pos2];
				SumB  += Raw8buffer[Pos2 + 1];
				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GRBG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + img.imgwidth * y;
				Pos2 = x + img.imgwidth * (y + 1);

				SumGr += Raw8buffer[Pos1];
				SumR  += Raw8buffer[Pos1 + 1];
				SumB  += Raw8buffer[Pos2];
				SumGb += Raw8buffer[Pos2 + 1]; 

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_GBRG)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + img.imgwidth * y;
				Pos2 = x + img.imgwidth * (y + 1);

				SumGb += Raw8buffer[Pos1];
				SumB  += Raw8buffer[Pos1 + 1];
				SumR  += Raw8buffer[Pos2];
				SumGr += Raw8buffer[Pos2 + 1];

				Count++;
			}
		}
	}

	if ( img.RawFormat == OUTFORMAT_BGGR)
	{
		for (y = StartY; y < EndY; y += 2)
		{
			for (x = StartX; x < EndX; x += 2)
			{
				Pos1 = x + img.imgwidth * y;
				Pos2 = x + img.imgwidth * (y + 1);

				SumB  += Raw8buffer[Pos1];
				SumGb += Raw8buffer[Pos1 + 1];
				SumGr += Raw8buffer[Pos2];
				SumR  += Raw8buffer[Pos2 + 1];

				Count++;
			}
		}
	}

	Gr = 1.0 * SumGr / Count;
	Gb = 1.0 * SumGb / Count;
	R  = 1.0 * SumR  / Count;
	B  = 1.0 * SumB  / Count;

	pShadingInfo->R  = int(R + 0.5);
	pShadingInfo->B  = int(B + 0.5);
	pShadingInfo->Gr = int(Gr + 0.5);
	pShadingInfo->Gb = int(Gb + 0.5);
	pShadingInfo->RG = (R - pDlg->BLC) / (((Gr + Gb) / 2.0 - pDlg->BLC) + 0.5 );	// Rave/Gave
	pShadingInfo->BG = (B - pDlg->BLC) / (((Gr + Gb) / 2.0 - pDlg->BLC) + 0.5 );	//Bave/Gave 

	/*
	*	��YRGB��ʽ������YCbCr��ʽ������Ӧ�ϴ����е�GetYrgbAverage()����.
	*	--Added by LiHai--20171209
	*/
	pShadingInfo->Y = 0.299 * pShadingInfo->R + 0.587 * ((pShadingInfo->Gr + pShadingInfo->Gb) / 2) + 0.114 * pShadingInfo->B;
}

void Inspection_Shading::GetRaw10PerBlock(void)
{
	ImageInfo img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ץ֡3ʧ��!"),COLOR_RED,200);
		return;
	}

	int  width   = img.imgwidth;
	int  height  = img.imgheight;
	BYTE * Raw8buffer = new BYTE[width * height];
	if (NULL == Raw8buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Raw8buffer ����ʧ��!"), COLOR_RED, 200);
		return;
	}
	Raw10toRaw8(img.Raw_buffer, Raw8buffer, width * height);

	int i = 0;
	int j = 0;
	float RGMax = 0;
	float BGMax = 0;
	int blockWidth  = img.imgwidth /(BLK_NUM_H * 2) * 2;
	int blockHeight = img.imgheight /(BLK_NUM_V * 2) * 2;
	memset(m_ShadingInfo, 0x00, sizeof(m_ShadingInfo));
	for (j = 0; j < BLK_NUM_H; j++)//�޸����ݱ�������˳�� ��������ÿ�п�ʼ���� LYC 20180603
	{
		for (i = 0; i < BLK_NUM_V; i++)
		{
			int x = i * blockWidth;
			int y = j * blockHeight;
			GetBlock(Raw8buffer, x, y, blockWidth, blockHeight, m_ShadingInfo + BLK_NUM_H * j + i);

			if (RGMax < m_ShadingInfo[BLK_NUM_H * j + i].RG)
			{
				RGMax = m_ShadingInfo[BLK_NUM_H * j + i].RG;
			}

			if (BGMax < m_ShadingInfo[BLK_NUM_H * j + i].BG)
			{
				BGMax = m_ShadingInfo[BLK_NUM_H * j + i].BG;
			}
		}
	}

	float ColorShadingVal_RG[BLK_NUM_H * BLK_NUM_V] = {0};
	float ColorShadingVal_BG[BLK_NUM_H * BLK_NUM_V] = {0};
	RGMax = m_ShadingInfo[BLK_CENTER_INX].RG;	//�����ٷ����Ĳο��ĵ��У�����ľ��ǳ�������Block��ֵ�������ǳ������ֵ--Added by LiHai--20180503
	BGMax = m_ShadingInfo[BLK_CENTER_INX].BG;
	for (i = 0; i < BLK_NUM_H * BLK_NUM_V; i++)
	{
		ColorShadingVal_RG[i] = m_ShadingInfo[i].RG / RGMax;
		ColorShadingVal_BG[i] = m_ShadingInfo[i].BG / BGMax;
	}

	SaveColorShadingData(ColorShadingVal_RG, ColorShadingVal_BG, BLK_NUM_H * BLK_NUM_V);
	SaveShadingChannelData(m_ShadingInfo, BLK_NUM_H * BLK_NUM_V);

	SAFE_DELETE_ARRAY(Raw8buffer);
}