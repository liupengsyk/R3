#include "StdAfx.h"
#include "Inspection_MTF.h"
#include "ImageProc.h"

#ifndef TCCM_EXP
#define TCCM_EXP	__declspec(dllexport)
#endif	/* TCCM_EXP */

extern "C" TCCM_EXP TestItemInterface* CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new Inspection_MTF(pInterface, lpszName);
}


Inspection_MTF::Inspection_MTF(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;                      //得到接口指针
	
	SetName(lpszName);//设置测试项目的名字
	
	pDlg = new MTFOptionDlg(NULL,this);             //生成对话框
     
	sMTF.ROI[0].cx = 50;
	sMTF.ROI[0].cy = 50;
	sMTF.ROI[0].Enable =1;
	sMTF.ROI[0].limit1 = 40;
	sMTF.ROI[0].limit2 = 30;

	sMTF.ROI[1].cx = 20;
	sMTF.ROI[1].cy = 20;
	sMTF.ROI[1].Enable = 1;
	sMTF.ROI[1].limit1 = 40;
	sMTF.ROI[1].limit2 = 30;

	sMTF.ROI[2].cx = 20;
	sMTF.ROI[2].cy =80;
	sMTF.ROI[2].Enable = 1;
	sMTF.ROI[2].limit1 = 40;
	sMTF.ROI[2].limit2 = 30;

	sMTF.ROI[3].cx = 80;
	sMTF.ROI[3].cy = 80;
	sMTF.ROI[3].Enable = 1;
	sMTF.ROI[3].limit1 = 40;
	sMTF.ROI[3].limit2 = 30;

	sMTF.ROI[4].cx = 80;
	sMTF.ROI[4].cy = 20;
	sMTF.ROI[4].Enable = 1;
	sMTF.ROI[4].limit1 = 40;
	sMTF.ROI[4].limit2 = 30;

	for(int i=5;i<16;i++)
	{	 
		sMTF.ROI[i].cx = 0;		 
		sMTF.ROI[i].cy = 0;		
		sMTF.ROI[i].Enable = 0;	
		sMTF.ROI[i].limit1 = 0;		
		sMTF.ROI[i].limit2 =0;
	}
	
	sMTF.HVratioLimit    = 100;
	sMTF.CornerDiffLimit = 100;
	sMTF.ROI_height      = 10;
	sMTF.ROI_width       = 10;
	sMTF.DequeSize       = 4;
	
	pDlg->m_Continue      = 0;	
	pDlg->m_ExposureEn       =  0;
	pDlg->m_exp_target       = 128;
	pDlg->m_exp_tolerance     = 2;

	RotateBack_en = 0;	
	RotateBack_MTF_LowerLimit = 0;	
	RotateBack_MTF_UpperLimit = 0;	

	m_bRotate = 0;
	pDlg->m_delay = 1;

	pDlg->Create(MTFOptionDlg::IDD,NULL);           //创建对话框
	SetOptionDlgHandle(pDlg->GetSafeHwnd());        //得到对话框句柄

	//TODO: 设置测试项目的类型 
	SetType(TYPE_INFINITY);
	Runmode =1;
	DelayNum = 0;
	Lock_OneTime = FALSE;
}

Inspection_MTF::~Inspection_MTF(void)
{

}

 int Inspection_MTF::InitItem()
 {
	 CamID  = GetCamID();
	 SetConfigPath(m_pInterface->Config_GetConfigPathName(CamID)); //设置CONFIG路径	
	 LoadOption(); 
	 
	 return 0;
 }

int Inspection_MTF::Initialize()
{
	TestItemInterface::Initialize();
	//TODO 
	SetbStopTest(FALSE);
	if(!Lock_OneTime)
	{
		m_timeCnt.Start();
		for (int i = 0; i<16; i++)
		{		
			sMTF.ROI[i].MTF_Deque.clear();     //清空节点信息
		}
		RotateBack_Flag = FALSE;
		Lock_OneTime = TRUE;
	}


	  img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	/*img.NV21_buffer;*/
	  
    Y_Buffer =  new BYTE[img.imgwidth*img.imgheight];
	return 0;
}

int Inspection_MTF::Testing()
{
	Lock_OneTime = FALSE;

	if (MTFTest() == 0 ) 
	{	
		return  0;
	}
	else
	{
		return -1;
	}

}

int Inspection_MTF::Finalize()
{
	Lock_OneTime = FALSE;

	// for count total test time 
	CString info;
	m_timeCnt.End();
	double testTime = m_timeCnt.GetUseTime();
	info.Format(_T("MTF测试结束,共用时间: %5.1f ms"), testTime);
	m_pInterface->Ctrl_Addlog(CamID,info, COLOR_BLUE,200);
	SAFE_DELETE(Y_Buffer);
	return TestItemInterface::Finalize();
}


int Inspection_MTF::LoadOption()
{
	sMTF.ROI[0].cx = ReadConfigInt(_T("ROI0_x"),50);
	sMTF.ROI[0].cy = ReadConfigInt(_T("ROI0_y"),50);
	sMTF.ROI[0].Enable = ReadConfigInt(_T("ROI0_Enable"),1);
	sMTF.ROI[0].limit1 = ReadConfigInt(_T("ROI0_Limit1"),40);
	sMTF.ROI[0].limit2 = ReadConfigInt(_T("ROI0_Limit2"),30);

	sMTF.ROI[1].cx = ReadConfigInt(_T("ROI1_x"),20);
	sMTF.ROI[1].cy = ReadConfigInt(_T("ROI1_y"),20);
	sMTF.ROI[1].Enable = ReadConfigInt(_T("ROI1_Enable"),1);
	sMTF.ROI[1].limit1 = ReadConfigInt(_T("ROI1_Limit1"),40);
	sMTF.ROI[1].limit2 = ReadConfigInt(_T("ROI1_Limit2"),30);

	sMTF.ROI[2].cx = ReadConfigInt(_T("ROI2_x"),20);
	sMTF.ROI[2].cy = ReadConfigInt(_T("ROI2_y"),80);
	sMTF.ROI[2].Enable = ReadConfigInt(_T("ROI2_Enable"),1);
	sMTF.ROI[2].limit1 = ReadConfigInt(_T("ROI2_Limit1"),40);
	sMTF.ROI[2].limit2 = ReadConfigInt(_T("ROI2_Limit2"),30);

	sMTF.ROI[3].cx = ReadConfigInt(_T("ROI3_x"),80);
	sMTF.ROI[3].cy = ReadConfigInt(_T("ROI3_y"),80);
	sMTF.ROI[3].Enable = ReadConfigInt(_T("ROI3_Enable"),1);
	sMTF.ROI[3].limit1 = ReadConfigInt(_T("ROI3_Limit1"),40);
	sMTF.ROI[3].limit2 = ReadConfigInt(_T("ROI3_Limit2"),30);

	sMTF.ROI[4].cx = ReadConfigInt(_T("ROI4_x"),80);
	sMTF.ROI[4].cy = ReadConfigInt(_T("ROI4_y"),20);
	sMTF.ROI[4].Enable = ReadConfigInt(_T("ROI4_Enable"),1);
	sMTF.ROI[4].limit1 = ReadConfigInt(_T("ROI4_Limit1"),40);
	sMTF.ROI[4].limit2 = ReadConfigInt(_T("ROI4_Limit2"),30);

	for(int i=5;i<16;i++)
	{
		CString KeyName;
		KeyName.Format(_T("ROI%d_x"),i);
		sMTF.ROI[i].cx = ReadConfigInt(KeyName,0);

		KeyName.Format(_T("ROI%d_y"),i);
		sMTF.ROI[i].cy = ReadConfigInt(KeyName,0);

		KeyName.Format(_T("ROI%d_Enable"),i);
		sMTF.ROI[i].Enable = ReadConfigInt(KeyName,0);

		KeyName.Format(_T("ROI%d_Limit1"),i);
		sMTF.ROI[i].limit1 = ReadConfigInt(KeyName,0);

		KeyName.Format(_T("ROI%d_Limit2"),i);
		sMTF.ROI[i].limit2 = ReadConfigInt(KeyName,0);
	}



	sMTF.HVratioLimit    = ReadConfigInt(_T("HVratio_Limit"),100);
	sMTF.CornerDiffLimit = ReadConfigInt(_T("CornerDiffLimit"),100);
	sMTF.ROI_height      = ReadConfigInt(_T("ROI_height"),10);
    sMTF.ROI_width       = ReadConfigInt(_T("ROI_width"),10);
	sMTF.DequeSize       = ReadConfigInt(_T("DequeSize"),4);


	pDlg->m_Continue      = ReadConfigInt(_T("Continue"),0);	
	pDlg->m_ExposureEn       = ReadConfigInt(_T("ExposureEn"), 0);
	pDlg->m_exp_target       = ReadConfigInt(_T("Exp_Target"), 128);
	pDlg->m_exp_tolerance     = ReadConfigInt(_T("Exp_Tolerance"),2);

	RotateBack_en = ReadConfigInt(_T("RotateBack_en"),0);	
	RotateBack_MTF_LowerLimit = ReadConfigInt(_T("RotateBack_MTF_LowerLimit"),0);	
	RotateBack_MTF_UpperLimit = ReadConfigInt(_T("RotateBack_MTF_UpperLimit"),0);	

	m_bRotate = ReadConfigInt(_T("bRotate"),0);
	pDlg->m_delay = ReadConfigInt(_T("delay"),1);

	pDlg->m_tempInfo = ReadConfigInt(_T("TEMP_INFO"), 0);

	pDlg->m_iUseRaw8Test = ReadConfigInt(_T("USE_RAW8_TEST"), 1);

	return TestItemInterface::LoadOption();
}

int Inspection_MTF::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;

	for(int i=0;i<16;i++)
	{
		CString KeyName;
		KeyName.Format(_T("ROI%d_x"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].cx);

		KeyName.Format(_T("ROI%d_y"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].cy);

		KeyName.Format(_T("ROI%d_Enable"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].Enable);

		KeyName.Format(_T("ROI%d_Limit1"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].limit1);

		KeyName.Format(_T("ROI%d_Limit2"),i);
		WriteConfigInt(KeyName,sMTF.ROI[i].limit2);
	}

	WriteConfigInt(_T("ExposureEn      "),pDlg->m_ExposureEn);
	WriteConfigInt(_T("Exp_Target      "),pDlg->m_exp_target);
	WriteConfigInt(_T("Exp_Tolerance   "),pDlg->m_exp_tolerance);

	WriteConfigInt(_T("HVratio_Limit   "),sMTF.HVratioLimit);
	WriteConfigInt(_T("ROI_height      "),sMTF.ROI_height);
	WriteConfigInt(_T("ROI_width       "),sMTF.ROI_width);
	WriteConfigInt(_T("DequeSize       "),sMTF.DequeSize);
	WriteConfigInt(_T("CornerDiffLimit "),sMTF.CornerDiffLimit);

	WriteConfigInt(_T("Continue"),pDlg->m_Continue);

	WriteConfigInt(_T("RotateBack_en"), RotateBack_en);
	WriteConfigInt(_T("RotateBack_MTF_LowerLimit"), RotateBack_MTF_LowerLimit);
	WriteConfigInt(_T("RotateBack_MTF_UpperLimit"), RotateBack_MTF_UpperLimit);

	WriteConfigInt(_T("bRotate"), m_bRotate);
	WriteConfigInt(_T("delay"),pDlg->m_delay);

	WriteConfigInt(_T("TEMP_INFO"), pDlg->m_tempInfo);
	 
	WriteConfigInt(_T("USE_RAW8_TEST"), pDlg->m_iUseRaw8Test);

	return 0;
}


LPCTSTR Inspection_MTF::GetReportHeads()
{
	// TODO : Make report headers of this inspection and return that
	static CString header; ;
	header = _T("");
	CString temp;
	for(int i=0;i<16;i++)
	{
		temp.Format(_T("B%d,"),i);
		header += temp;
	}
	return header;
}

LPCTSTR Inspection_MTF::GetReportLowLimit()
{
	// TODO : Make report lower limits of this inspection and return that

	return _T("");
}

LPCTSTR Inspection_MTF::GetReportHightLimit()
{
	// TODO : Make report upper limits of this inspection and return that

	return _T("");
}

LPCTSTR Inspection_MTF::GetReportContents()
{
	// TODO : Make report contents of this inspection and return that
	static CString Contents;
	Contents = _T("");
	CString temp;
	for(int i=0;i<16;i++)
	{
		if (sMTF.ROI[i].Enable)
		{
			temp.Format(_T("%.2lf,"),sMTF.ROI[i].MTFvalue*100);
		}
		else
		{
		    temp = _T("0,");
		}
		Contents += temp;
	}

	if (pDlg->m_tempInfo == TRUE)
	{
		temp.Format(_T("%d, "), GetTemperate());
		Contents += temp;
	}

	return Contents;
}

int Inspection_MTF::MTFTest(void)
{
	DelayNum++;
	if (Runmode==2 || Runmode==3)
	{
		//显示相关信息
		ShowMTFinfo(RotateBack_Flag);
		if (Runmode==2 &&DelayNum<pDlg->m_delay)
		{
			return 1;
		}
		else if(Runmode==3)
		{
			Runmode=1;
			return 2;
		}
		else
		{
			DelayNum = 0;
			Runmode=1;
			return 0;
		}
	} 
	 
	CalcImageRect();

	  img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	/*img.NV21_buffer;*/
	int width =  img.imgwidth;
	int height = img.imgheight;

	if (pDlg->m_iUseRaw8Test)
	{
		BYTE *pRaw8Buf = new BYTE[width*height];
		if (NULL == pRaw8Buf)
		{
			CString Msg;
			Msg.Format(_T("%s malloc memory error!"),GetName());
			m_pInterface->Ctrl_Addlog(CamID,Msg, COLOR_RED,500);	
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,Msg);
			SetResult(RESULT_FAIL);
			return 0;
		}
		ImageProc::RawToRaw8(img.Raw_buffer,pRaw8Buf, width, height, img.RawBits);
		ImageProc::Raw8toRGB24(pRaw8Buf, img.RGB24_buffer, width, height, img.RawFormat);
		ImageProc::RGB24toY(img.RGB24_buffer,Y_Buffer,width,height);
		delete [] pRaw8Buf;
	}
	else
	{
		ImageProc::RGB24toY(img.RGB24_buffer,Y_Buffer,width,height);
	}
	

	
	int FailCount = 0;
	BOOL DequeFull = FALSE;
	double vMTF = 0, Max = 0, Min = 100;
	for(int i=0;i<16;i++)
	{
		if(sMTF.ROI[i].Enable)
		{
			//得到指定区域的MTF值	
		   ROI_Info* PROI = &sMTF.ROI[i];
		   switch(m_Alg)
		   {
		   case 0:
			   vMTF = ImageProc::GetMTF(Y_Buffer,width,height,PROI->pos);	
			   break;
		   default:
			   vMTF = ImageProc::GetMTF(Y_Buffer,width,height,PROI->pos);	//固定一种
			   break;
		   }
		   
           PROI->MTF_Deque.push_back(vMTF); //入队列

			//支持动态修改队列长度
		    int DequeSizeLimit = sMTF.DequeSize + 1;// 注意COMBOX 的数字要减1
			int OverSizes = PROI->MTF_Deque.size() - DequeSizeLimit;
			if ( OverSizes < 0 )
				continue;
			else
			{
				DequeFull = TRUE;
				for (int k = 0; k < OverSizes ; k++)
				{
                    PROI->MTF_Deque.pop_front(); //弹出第一个
				}
				
				//求队列中数据的平均值
				double value = 0 ;
				for (UINT k = 0; k < PROI->MTF_Deque.size(); k++)
				{
					value += PROI->MTF_Deque[k];
				}
				PROI->MTFvalue = value/PROI->MTF_Deque.size();

				//计算当前帧水平和垂直解像力差异
				PROI->HVratio = GetHVResolutionRatio(Y_Buffer,width,height,PROI->pos);

				//计算每个区域的测试结果
				if(PROI->MTFvalue*100 > PROI->limit1)
				{
					PROI->TestResult = 1;
				}
				else if (PROI->MTFvalue*100 > PROI->limit2)
				{
					if(PROI->HVratio > (sMTF.HVratioLimit/100.0))
					{
						PROI->TestResult = 0;	
					    FailCount++;
					}
					else
						PROI->TestResult = 2;
				}
				else
				{		
					FailCount++;
					PROI->TestResult=0;
				}

				if(i>=1)
				{
					Max = max(Max, vMTF);
					Min = min(Min, vMTF);
				}

			}
		}
	}


	if (!DequeFull)
	{
		return -2;
	}
	diff = 100*(Max - Min);
	//判断边角差异
	if ( diff > sMTF.CornerDiffLimit) 		FailCount++;

	//根据失败个数，判断OK还是NG
	 
	if (FailCount>0)	
	{
		CString Msg;
		Msg.Format(_T("%s不良!"),GetName());
		m_pInterface->Ctrl_Addlog(CamID,Msg, COLOR_RED,500);	
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,Msg);
		SetResult(RESULT_FAIL);
	}
	else
	{
		SetResult(RESULT_PASS);
		RotateBack_Flag = TRUE;
	}

	

	////是否打开持续显示，持续不退出
	if (pDlg->m_Continue)
	{
		Runmode =3;
		return 2;
	}
	else
	{
		Runmode =2;
		return 1;
	}
}

void Inspection_MTF::ShowMTFinfo(BOOL bResult)
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

 
	CRect rect;
	m_pInterface->Ctrl_GetDisplayWndRect(&rect,CamID);
 


	ImageInfo img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	double Xratio = 1.0*DisplayWndRect.Width()/img.imgwidth;
	double Yratio = 1.0*DisplayWndRect.Height()/img.imgheight;

	
	for(int i=0;i<16;i++)
	{
		if(sMTF.ROI[i].Enable)
		{
			switch(sMTF.ROI[i].TestResult)
			{
			case 1:
				CurrentColor = COLOR_GREEN;
				break;
			case 2:
				CurrentColor = COLOR_YELLOW;
				break;
			case 0:
				CurrentColor = COLOR_RED;
				break;
		     }
			CPen pen(PS_SOLID,2,CurrentColor);
			pDisplayWndMemDC->SelectObject(&pen);
			CRect rect = sMTF.ROI[i].pos;

			rect.left   =  LONG(rect.left   *  Xratio);
			rect.right  =  LONG(rect.right  *  Xratio);
			rect.top    =  LONG(rect.top    *  Yratio);
			rect.bottom =  LONG(rect.bottom *  Yratio);

			pDisplayWndMemDC->Rectangle(rect);

			//画字符
			int x = sMTF.ROI[i].cx * DisplayWndRect.Width()/100;
			int y = sMTF.ROI[i].cy * DisplayWndRect.Height()/100;
			
			CString str;
			str.Format(_T("B%d"),i);
			pDisplayWndMemDC->TextOut(x-10,y-20,str);

			str.Format(_T("%.1lf%%"),sMTF.ROI[i].MTFvalue*100);
 
			pDisplayWndMemDC->TextOut(x-20,y,str);

			CString info;
			info.Format(_T("B%d:%.1lf%%"),i,sMTF.ROI[i].MTFvalue*100);
			m_pInterface->Ctrl_Addlog(CamID,info, CurrentColor,200);
		}
	}

	CString result;
	if (!bResult)
	{
		CurrentColor = COLOR_RED;
		result = _T("MTF Fail!!,更换模组！");
	}
	else
	{
		CurrentColor = COLOR_GREEN;
		result = _T("MTF Pass，更换模组！");
	}
 
 
	CFont ft2;
	ft2.CreatePointFont(300,_T("Arial"));
	pDisplayWndMemDC->SelectObject(ft2);
	pDisplayWndMemDC->SetTextColor(CurrentColor);
	pDisplayWndMemDC->TextOutW( int(rect.Width()*0.2), int(rect.Height()*0.9), result);
	m_pInterface->Ctrl_ShowMemDC(CamID);
	if (Runmode==2&&DelayNum==1)
	{
		m_pInterface->Ctrl_Addlog(CamID,result,CurrentColor,220);
	}
	
}

void Inspection_MTF::CalcImageRect(void)
{
	ImageInfo img  = *m_pInterface->Ctrl_GetImgInfo(CamID);
	int width =  img.imgwidth;
	int height = img.imgheight;
	for(int i=0;i<16;i++)
	{
		if(sMTF.ROI[i].Enable)
		{
		//比例转化为实际像素点
			ROI_Info* PROI = &sMTF.ROI[i];

			if (PROI->cy == 0 && PROI->cx == 0)
			{
				PROI->Enable = FALSE;
			} 
			PROI->pos.top    = int((PROI->cy - sMTF.ROI_height/2.0)  * height/100);
			PROI->pos.bottom = int((PROI->cy + sMTF.ROI_height/2.0)  * height/100);
			PROI->pos.left   = int((PROI->cx - sMTF.ROI_width/2.0)   * width/100);
			PROI->pos.right  = int((PROI->cx + sMTF.ROI_width/2.0)   * width/100);

			if (m_bRotate)
			{
				if (i>=8)
				{
					int w = PROI->pos.Width();
					int h = PROI->pos.Height();

					PROI->pos.top    = (PROI->cy * height/100) - w/2;
					PROI->pos.bottom = (PROI->cy * height/100) + w/2;
					PROI->pos.left   = (PROI->cx * width/100)  - h/2;
					PROI->pos.right  = (PROI->cx * width/100)  + h/2;
				}
			}


			if(PROI->pos.left < 0)
				PROI->pos.MoveToX(0);
			if(PROI->pos.right >= width)
				PROI->pos.MoveToX(width - 1 - PROI->pos.Width());
			if(PROI->pos.top < 0)
				PROI->pos.MoveToY(0);
			if(PROI->pos.bottom >= height)
				PROI->pos.MoveToY(height - 1 - PROI->pos.Height());

			if (PROI->pos.Width() == 0 || PROI->pos.Height() == 0)
			{
				PROI->Enable = FALSE;
			}
	    }
	}

}

double Inspection_MTF::GetHVResolutionRatio(BYTE* pScr,int width,int height,CRect rect)
{
	double Row_sum=0;
	double Col_sum=0;
	double ratio=0;
	double count=0;
	for(int i=rect.top+1;i<rect.bottom;i++)
	{
		for(int j=rect.left+1;j<rect.right;j++)
		{
			Row_sum += (pScr[i*width+j] - pScr[i*width+j-1]) * (pScr[i*width+j] - pScr[i*width+j-1]);
			Col_sum += (pScr[i*width+j] - pScr[(i-1)*width+j]) * (pScr[i*width+j] - pScr[(i-1)*width+j]);
			count++;
		}
	}
	if(count==0) return 0;

	double sum = Row_sum + Col_sum;

	double diff = abs((Row_sum / sum ) - (Col_sum / sum )); 

	return diff;
}

int Inspection_MTF::OnCtrlKeyNum()
{
	return 0;
}

int Inspection_MTF::GetTemperate()
{
	//Sleep(500); 
	USHORT temperate;
	Sleep(500);
	if (CamID == 0)
		m_pInterface->Device_ReadI2c(0x34,0x013A,&temperate,3,CamID);
	else
		m_pInterface->Device_ReadI2c(0x20,0x013A,&temperate,3,CamID);
	Sleep(500);

	return (int)temperate; 
}