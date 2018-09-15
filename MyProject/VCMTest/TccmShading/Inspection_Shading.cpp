#include "StdAfx.h"
#include "Inspection_Shading.h"
#include "ImageProc.h"
#include "math.h"

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

	SetName(lpszName);	                             //设置测试项目的名字
	pDlg = new ShadingOptionDlg(NULL, this);         //生成对话框对象
	                                  //读取参数
	pDlg->Create(ShadingOptionDlg::IDD, NULL);       //创建对话框窗口
	SetOptionDlgHandle(pDlg->GetSafeHwnd());         //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_IMAGE);

	//其他类成员构造初始化

}

Inspection_Shading::~Inspection_Shading(void)
{

}

int Inspection_Shading::InitItem()
{
	CamID =GetCamID();
	SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID));	 //设置CONFIG路径	
	LoadOption();  
	pDlg->shadingTimes=0;

	return 0;
}

int Inspection_Shading::Initialize()
{
	TestItemInterface::Initialize();
	//TODO 
	SetbStopTest(FALSE);
	m_pInterface->VcmDr_InitAF(CamID);
	Sleep(pDlg->DelayTime);
	CamCode = m_pInterface->Ctrl_GetCamCode();

	for(int col=0;col<5;col++)
	{
		Rgain[col]=0.0;
		Bgain[col]=0.0;
	}
	if(pDlg->shadingTimes==3)
		pDlg->shadingTimes=0;
	if(pDlg->shadingTimes==0)
	{
		m_pInterface->VcmDr_WriteAF_Code(100,CamID);
	//	m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		m_pInterface->Ctrl_Addlog(CamID,L"VCM推动到100",COLOR_BLUE,200);
		pDlg->shadingTimes=1;
		Sleep(pDlg->DelayTime);
//		return -1;
	}
	return 0;
}

int Inspection_Shading::Testing()
{
	if(pDlg->shadingTimes==1||pDlg->shadingTimes==3)
	{
// 		m_pInterface->Ctrl_PauseGrabFrame(CamID);
// 		Sleep(pDlg->DelayTime);
		if (ShadingTest() == 0) 
		{

			return  0;
		}
		else
		{
			return -1;
		}
	}
}

int Inspection_Shading::Finalize()
{
// 	m_pInterface->Ctrl_ResumeGrabFrame(CamID);
// 	Sleep(pDlg->DelayTime);
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

	pDlg->m_Continue      = ReadConfigInt(_T("Continue"),0);	

	pDlg->BLC=ReadConfigInt(_T("BLC"),16);	



	pDlg->m_RGMax=ReadConfigInt(_T("m_RGMax"),0);	
	pDlg->m_RGMin=ReadConfigInt(_T("m_RGMin"),0);	
	pDlg->m_BGMax=ReadConfigInt(_T("m_BGMax"),0);	
	pDlg->m_BGMin=ReadConfigInt(_T("m_BGMin"),0);	
	pDlg->m_YMax=ReadConfigInt(_T("m_YMax"),0);	
	pDlg->m_Ymin=ReadConfigInt(_T("m_Ymin"),0);	
	pDlg->m_YDiff=ReadConfigInt(_T("m_YDiff"),0);	

	pDlg->Y1diffmax=ReadConfigInt(_T("Y1diffmax"),0);
	pDlg->Y1diffmin=ReadConfigInt(_T("Y1diffmin"),0);

	pDlg->Y2diffmax=ReadConfigInt(_T("Y2diffmax"),0);
	pDlg->Y2diffmin=ReadConfigInt(_T("Y2diffmin"),0);

	pDlg->Y3diffmax=ReadConfigInt(_T("Y3diffmax"),0);
	pDlg->Y3diffmin=ReadConfigInt(_T("Y3diffmin"),0);

	pDlg->Y4diffmax=ReadConfigInt(_T("Y4diffmax"),0);
	pDlg->Y4diffmin=ReadConfigInt(_T("Y4diffmin"),0);

	pDlg->DelayTime=ReadConfigInt(_T("DelayTime"),1000);

	//pDlg->DelayTime
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
	WriteConfigInt(_T("BLC"), pDlg->BLC);


	WriteConfigInt(_T("m_RGMax"),pDlg->m_RGMax);	
	WriteConfigInt(_T("m_RGMin"),pDlg->m_RGMin);	
	WriteConfigInt(_T("m_BGMax"),pDlg->m_BGMax);	
	WriteConfigInt(_T("m_BGMin"),pDlg->m_BGMin);	
	WriteConfigInt(_T("m_YMax"),pDlg->m_YMax);	
	WriteConfigInt(_T("m_Ymin"),pDlg->m_Ymin);	
	WriteConfigInt(_T("m_YDiff"),pDlg->m_YDiff);	

	WriteConfigInt(_T("Y1diffmax"),pDlg->Y1diffmax);
	WriteConfigInt(_T("Y1diffmin"),pDlg->Y1diffmin);

	WriteConfigInt(_T("Y2diffmax"),pDlg->Y2diffmax);
	WriteConfigInt(_T("Y2diffmin"),pDlg->Y2diffmin);

	WriteConfigInt(_T("Y3diffmax"),pDlg->Y3diffmax);
	WriteConfigInt(_T("Y3diffmin"),pDlg->Y3diffmin);

	WriteConfigInt(_T("Y4diffmax"),pDlg->Y4diffmax);
	WriteConfigInt(_T("Y4diffmin"),pDlg->Y4diffmin);

	WriteConfigInt(_T("DelayTime"),pDlg->DelayTime);
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

	for(int i=0; i<5; i++)
	{
		if(sShading.ROI[i].Enable)
		{
			//比例转化为实际像素点
			//比例转化为实际像素点
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

int Inspection_Shading::ShadingTest(void)
{
	CalcImageRect();
	ImageInfo img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	if (NULL == img.RGB24_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("抓帧失败1"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,L"抓帧失败1");
		SetResult(RESULT_FAIL);
		return 0;
	}
	int width  = img.imgwidth;
	int height = img.imgheight;

	BYTE* RGB24_Buffer = img.RGB24_buffer;


	//计算结果
	for(int i=0;i<5;i++)
	{
		GetAWBInfoRaw8(i);
	}
	int CenterRG=sShading.ROI[0].RGain;
	int CenterBG=sShading.ROI[0].BGain;
	int CenterY=sShading.ROI[0].aveY;
	//shanding 原始值
	for(int i=0;i<5;i++)
	{
		Rgain[i]=sShading.ROI[i].RGain/512.0;
		Bgain[i]=sShading.ROI[i].BGain/512.0;
		CString TempMsg;
		TempMsg.Format(_T("Block_%d,Unit_R/G=%.4f,Unit_B/G=%.4f"),i,Rgain[i],Bgain[i]);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	}

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
		CString TempMsg;
		TempMsg.Format(_T("Block_%d,R=%d,Gr=%d,Gb=%d,B=%d"),i,sShading.ROI[i].aveR,sShading.ROI[i].aveGr,sShading.ROI[i].aveGb,sShading.ROI[i].aveB);
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
	CString TempMsg;
	TempMsg.Format(_T("Block_%d maxRG=%d   <%d,%d>"),blockRGMax,RGMax,pDlg->m_RGMin,pDlg->m_RGMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("Block_%d minRG=%d   <%d,%d>"),blockRGMin,RGMin,pDlg->m_RGMin,pDlg->m_RGMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("Block_%d maxBG=%d   <%d,%d>"),blockBGMax,BGMax,pDlg->m_BGMin,pDlg->m_BGMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);
	TempMsg.Format(_T("Block_%d minBG=%d   <%d,%d>"),blockBGMin,BGMin,pDlg->m_BGMin,pDlg->m_BGMax);
	m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);



	//得到最终的值


	int YMax=0;
	int YMin=sShading.ROI[1].aveY;
	int YDiff;
	int YDiffMin[4]={pDlg->Y1diffmin,pDlg->Y2diffmin,pDlg->Y3diffmin,pDlg->Y4diffmin};
	int YDiffMax[4]={pDlg->Y1diffmax,pDlg->Y2diffmax,pDlg->Y3diffmax,pDlg->Y4diffmax};

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
			sShading.Ydif_result = RESULT_FAIL;
		else
			sShading.Ydif_result = RESULT_PASS;

		CString TempMsg;
		TempMsg.Format(_T("Y_Blk%d=%d"),i,sShading.ROI[i].aveY);
		m_pInterface->Ctrl_Addlog(CamID,TempMsg,COLOR_BLUE,200);

		if(pDlg->shadingTimes==1)
		{
			FirtTimeBlockValue[i]=sShading.ROI[i].aveY;
		}
		if(pDlg->shadingTimes==3)
		{
			if(abs(sShading.ROI[i].aveY-FirtTimeBlockValue[i])>YDiffMax[i-1]||abs(sShading.ROI[i].aveY-FirtTimeBlockValue[i])<YDiffMin[i-1])
			{
				m_pInterface->Ctrl_Addlog(CamID,L"两组Shading测试差异过小,马达未推动",COLOR_RED,200);
				m_pInterface->Ctrl_SetCamErrorInfo(CamID,L"马达测试失败！！");
				SetResult(RESULT_FAIL);
				return 0;
			}
			FirtTimeBlockValue[i]=999.0;
		}	
	}

	if(pDlg->shadingTimes<2)
	{
		m_pInterface->VcmDr_WriteAF_Code(1000,CamID);
//		m_pInterface->Ctrl_ResumeGrabFrame(CamID);
		m_pInterface->Ctrl_Addlog(CamID,L"VCM推动到1000",COLOR_RED,200);
		Sleep(pDlg->DelayTime);
		pDlg->shadingTimes=3;
		return 1;
	}
	else
	{
		SetResult(RESULT_PASS);
		return 0;
	}
}

#if 0
void Inspection_Shading::ShowShadingTest(void)
{
	CRect DisplayWndRect;
	m_pInterface->Ctrl_GetDisplayWndRect(&DisplayWndRect,CamID);

	//得到系统默认字体
	CFont ft;
	HGDIOBJ hfont = ::GetStockObject(SYSTEM_FONT);
	ft.Attach(hfont);

	//初始化
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

			//画字符
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
	//显示DC
	m_pInterface->Ctrl_ShowMemDC(CamID);

}
#endif

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

	int  width   = img.imgwidth;
	int  height   =   img.imgheight;
	BYTE * Raw8buffer=new BYTE[width*height];
	Raw10toRaw8(img.Raw_buffer,Raw8buffer,width*height);

	int  StartX = sShading.ROI[block].pos.left;
	int  StartY = sShading.ROI[block].pos.top;
	int  EndX   = sShading.ROI[block].pos.right;
	int  EndY   = sShading.ROI[block].pos.bottom;

	if (EndX >width )
	{
		EndX = width ;
	}
	if (EndY >height )
	{
		EndY = height;
	}

	int  x = 0;
	int  y = 0;
	unsigned long  Count = 0;
	int  Pos1  = 0, Pos2 = 0;

	unsigned long  SumGr = 0;
	double	Gr = 0.0;

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

	Gr = 1.0*SumGr / Count;
	Gb = 1.0*SumGb / Count;
	R  = 1.0*SumR  / Count;
	B  = 1.0*SumB  / Count;

	
	sShading.ROI[block].aveR  = int(R);
	sShading.ROI[block].aveB  = int(B);
	sShading.ROI[block].aveGr  = int(Gr);
	sShading.ROI[block].aveGb  = int(Gb);
	sShading.ROI[block].aveG  = int((Gr + Gb) / 2.0); 
	sShading.ROI[block].RGain = int( 512.0*(R-pDlg->BLC )/( (Gr+Gb)/2.0-pDlg->BLC) + 0.5 );// Rave/Gave
	sShading.ROI[block].BGain = int( 512.0*(B -pDlg->BLC)/( (Gr+Gb)/2.0-pDlg->BLC) + 0.5 );  //Bave/Gave 

	sShading.ROI[block].aveY= 0.299 *sShading.ROI[block].aveR + 0.587*sShading.ROI[block].aveG + 0.114*sShading.ROI[block].aveB;

	delete []Raw8buffer;
}
