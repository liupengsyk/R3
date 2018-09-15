#include "StdAfx.h"
#include "OC_Item_Moto_TestItem.h"
#include "ImageProc.h"
//#include "OPCenterThreshold.h"	


#ifndef Tccm_EXP
    #define Tccm_EXP	__declspec(dllexport)
#endif	/* Tccm_EXP */
//#pragma comment(lib,"OPCenterThreshold.lib")

extern "C" Tccm_EXP TestItemInterface * CreateTestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new OC_Item_Moto_TestItem(pInterface, lpszName);
}


/******************************************************************
������:    ���캯��
��������:  �������ɵ�ʱ���ʼ������ɵ�����
*******************************************************************/
OC_Item_Moto_TestItem::OC_Item_Moto_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName)
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

OC_Item_Moto_TestItem::~OC_Item_Moto_TestItem(void)
{

}


int OC_Item_Moto_TestItem::InitItem()
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
int OC_Item_Moto_TestItem::Initialize()
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
int OC_Item_Moto_TestItem::Testing()
{
   //TODO:�ڴ���Ӳ��������
	
	if (pDlg->m_PixelSize < 0.5)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("Pixel size���ù�С"),COLOR_RED,200);
		m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("OC Test fail!"));
		SetResult(RESULT_FAIL);
		return 0;
	}
		if (!OCTest())
		{
			m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("OC Test fail!"));
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
BOOL OC_Item_Moto_TestItem::OCTest()
{
	m_pInterface->Ctrl_Addlog(CamID,_T("**Get Optical Center**"),COLOR_BLUE,200);
	CString TempStr = _T("");

	ImageInfo img = *m_pInterface->Ctrl_GetImgInfo(CamID); 
	if (NULL == img.Raw_buffer)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("ץ֡1ʧ��!"),COLOR_RED,200);
		return FALSE;
	}

	int  width   = img.imgwidth;
	int  height   =   img.imgheight;

	float PC_x=float((width-1)/2.0);
	float PC_y=float((height-1)/2.0);

	CenterX = PC_x;
	CenterY = PC_y;

	OFOPTCenterIn CenterIn;
	CenterIn.ImageBufferIn = img.Raw_buffer;
	CenterIn.Width = img.imgwidth;
	CenterIn.Height = img.imgheight;
	CenterIn.Outformat = img.RawFormat;
	CenterIn.pixelSize = pDlg->m_PixelSize;

	if (!pDlg->m_isMonoSensor)
	{
		OpticalCenterTest(&CenterIn,&CenterOut);

		float channelIndex = 0.0;
		if (CenterOut.R_OpticalOut > pDlg->m_OCSpec)
		{
			if (pDlg->m_OCSpec_Shift && CenterOut.R_OpticalOut < pDlg->m_OCSpec_AfterShift)
			{
				channelIndex = (1-((pDlg->m_OCSpec-3.0)/CenterOut.R_OpticalOut));

				CenterOut.R_OpticalPixel.xIndex = (int)((PC_x - CenterOut.R_OpticalPixel.xIndex)*channelIndex + CenterOut.R_OpticalPixel.xIndex);
				CenterOut.R_OpticalPixel.yIndex = (int)((PC_y - CenterOut.R_OpticalPixel.yIndex)*channelIndex + CenterOut.R_OpticalPixel.yIndex);
				CenterOut.R_OpticalOut =  pDlg->m_PixelSize * (sqrt(pow((CenterOut.R_OpticalPixel.xIndex-PC_x),2)+pow((CenterOut.R_OpticalPixel.yIndex-PC_y),2)));
			}
			else
			{
				TempStr.Format(_T("OC_R = %f,xIndex=%d,yIndex=%d"),CenterOut.R_OpticalOut,CenterOut.R_OpticalPixel.xIndex,CenterOut.R_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
				return FALSE;
			}
		}
		if (CenterOut.Gr_OpticalOut > pDlg->m_OCSpec)
		{
			if (pDlg->m_OCSpec_Shift && CenterOut.Gr_OpticalOut < pDlg->m_OCSpec_AfterShift)
			{
				channelIndex = (1-((pDlg->m_OCSpec-3.0)/CenterOut.Gr_OpticalOut));

				CenterOut.Gr_OpticalPixel.xIndex = (int)((PC_x - CenterOut.Gr_OpticalPixel.xIndex)*channelIndex + CenterOut.Gr_OpticalPixel.xIndex);
				CenterOut.Gr_OpticalPixel.yIndex = (int)((PC_y - CenterOut.Gr_OpticalPixel.yIndex)*channelIndex + CenterOut.Gr_OpticalPixel.yIndex);
				CenterOut.Gr_OpticalOut =  pDlg->m_PixelSize * (sqrt(pow((CenterOut.Gr_OpticalPixel.xIndex-PC_x),2)+pow((CenterOut.Gr_OpticalPixel.yIndex-PC_y),2)));
			}
			else
			{
				TempStr.Format(_T("OC_Gr = %f,xIndex=%d,yIndex=%d"),CenterOut.Gr_OpticalOut,CenterOut.Gr_OpticalPixel.xIndex,CenterOut.Gr_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
				return FALSE;
			}
		}
		if (CenterOut.Gb_OpticalOut > pDlg->m_OCSpec)
		{
			if (pDlg->m_OCSpec_Shift && CenterOut.Gb_OpticalOut < pDlg->m_OCSpec_AfterShift)
			{
				channelIndex = (1-((pDlg->m_OCSpec-3.0)/CenterOut.Gb_OpticalOut));

				CenterOut.Gb_OpticalPixel.xIndex = (int)((PC_x - CenterOut.Gb_OpticalPixel.xIndex)*channelIndex + CenterOut.Gb_OpticalPixel.xIndex);
				CenterOut.Gb_OpticalPixel.yIndex = (int)((PC_y - CenterOut.Gb_OpticalPixel.yIndex)*channelIndex + CenterOut.Gb_OpticalPixel.yIndex);
				CenterOut.Gb_OpticalOut =  pDlg->m_PixelSize * (sqrt(pow((CenterOut.Gb_OpticalPixel.xIndex-PC_x),2)+pow((CenterOut.Gb_OpticalPixel.yIndex-PC_y),2)));
			}
			else
			{
				TempStr.Format(_T("OC_Gb = %f,xIndex=%d,yIndex=%d"),CenterOut.Gb_OpticalOut,CenterOut.Gb_OpticalPixel.xIndex,CenterOut.Gb_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
				return FALSE;
			}
		}
		if (CenterOut.B_OpticalOut > pDlg->m_OCSpec)
		{
			if (pDlg->m_OCSpec_Shift && CenterOut.B_OpticalOut < pDlg->m_OCSpec_AfterShift)
			{
				channelIndex = (1-((pDlg->m_OCSpec-3.0)/CenterOut.B_OpticalOut));

				CenterOut.B_OpticalPixel.xIndex = (int)((PC_x - CenterOut.B_OpticalPixel.xIndex)*channelIndex + CenterOut.B_OpticalPixel.xIndex);
				CenterOut.B_OpticalPixel.yIndex = (int)((PC_y - CenterOut.B_OpticalPixel.yIndex)*channelIndex + CenterOut.B_OpticalPixel.yIndex);
				CenterOut.B_OpticalOut =  pDlg->m_PixelSize * (sqrt(pow((CenterOut.B_OpticalPixel.xIndex-PC_x),2)+pow((CenterOut.B_OpticalPixel.yIndex-PC_y),2)));
			}
			else
			{
				TempStr.Format(_T("OC_B = %f,xIndex=%d,yIndex=%d"),CenterOut.B_OpticalOut,CenterOut.B_OpticalPixel.xIndex,CenterOut.B_OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
				return FALSE;
			}
		}

		TempStr.Format(_T("OC_R = %f,xIndex=%d,yIndex=%d"),CenterOut.R_OpticalOut,CenterOut.R_OpticalPixel.xIndex,CenterOut.R_OpticalPixel.yIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

		TempStr.Format(_T("OC_Gr = %f,xIndex=%d,yIndex=%d"),CenterOut.Gr_OpticalOut,CenterOut.Gr_OpticalPixel.xIndex,CenterOut.Gr_OpticalPixel.yIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

		TempStr.Format(_T("OC_Gb = %f,xIndex=%d,yIndex=%d"),CenterOut.Gb_OpticalOut,CenterOut.Gb_OpticalPixel.xIndex,CenterOut.Gb_OpticalPixel.yIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);

		TempStr.Format(_T("OC_B = %f,xIndex=%d,yIndex=%d"),CenterOut.B_OpticalOut,CenterOut.B_OpticalPixel.xIndex,CenterOut.B_OpticalPixel.yIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	}
	else
	{
		OpticalCenterTest_Mono(&CenterIn,&CenterOut_Mono);

		float channelIndex = 0.0;
		if (CenterOut_Mono.OC_Out > pDlg->m_OCSpec)
		{
			if (pDlg->m_OCSpec_Shift && CenterOut_Mono.OC_Out < pDlg->m_OCSpec_AfterShift)
			{
				channelIndex = (1-((pDlg->m_OCSpec-3.0)/CenterOut_Mono.OC_Out));
				CenterOut_Mono.OpticalPixel.xIndex = (int)((PC_x - CenterOut_Mono.OpticalPixel.xIndex)*channelIndex + CenterOut_Mono.OpticalPixel.xIndex);
				CenterOut_Mono.OpticalPixel.yIndex = (int)((PC_y - CenterOut_Mono.OpticalPixel.yIndex)*channelIndex + CenterOut_Mono.OpticalPixel.yIndex);
				CenterOut_Mono.OC_Out =  pDlg->m_PixelSize * (sqrt(pow((CenterOut_Mono.OpticalPixel.xIndex-PC_x),2)+pow((CenterOut_Mono.OpticalPixel.yIndex-PC_y),2)));
			}
			else
			{
				TempStr.Format(_T("OC_Mono = %f,xIndex=%d,yIndex=%d"),CenterOut_Mono.OC_Out,CenterOut_Mono.OpticalPixel.xIndex,CenterOut_Mono.OpticalPixel.yIndex);
				m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_RED,200);
				return FALSE;
			}
		}

		TempStr.Format(_T("OC_Mono = %f,xIndex=%d,yIndex=%d"),CenterOut_Mono.OC_Out,CenterOut_Mono.OpticalPixel.xIndex,CenterOut_Mono.OpticalPixel.yIndex);
		m_pInterface->Ctrl_Addlog(CamID,TempStr,COLOR_BLUE,200);
	}

	m_pInterface->Ctrl_Addlog(CamID,_T("OC ���� PASS !"),COLOR_BLUE,200);
	return TRUE;
}

/******************************************************************
������:    Finalize
��������:  ����Initialize�����ı����ָ�ԭʼֵ��������ڴ�ռ��ͷŵ�
����ֵ��   0 - ���   ��0 - δ���
*******************************************************************/
int OC_Item_Moto_TestItem::Finalize()
{
	//TODO:�ڴ���ӻ��ս�������

	return 0;
}


/******************************************************************
������:    LoadOption
��������:  ���ڴ������ļ���ȡ�������������͸�ֵ���Ի���İ󶨱��� (�ļ��洢ֵ->������>�ؼ���ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int OC_Item_Moto_TestItem::LoadOption()
{
	//TODO: �ڴ���Ӷ�ȡ���� 
	USES_CONVERSION;
	CString TempStr = _T("");

	TempStr = ReadConfigString(_T("PixelSize"),_T("1.4"));
	pDlg->m_PixelSize = strtod(T2A(TempStr),NULL);

	TempStr = ReadConfigString(_T("OCSpec"),_T("70"));
	pDlg->m_OCSpec = strtod(T2A(TempStr),NULL);

	TempStr = ReadConfigString(_T("OCSpec_AfterShift"),_T("70"));
	pDlg->m_OCSpec_AfterShift = strtod(T2A(TempStr),NULL);
	
	pDlg->m_OCSpec_Shift = ReadConfigInt(_T("OCSpec_Shift_En"),0);
	pDlg->m_Continue=ReadConfigInt(_T("m_Continue"),0);
	pDlg->m_isMonoSensor = ReadConfigInt(_T("isMonoSensor"),1);
	//........................
	return TestItemInterface::LoadOption();
}


/******************************************************************
������:    SaveOption
��������:  ���ڽ�������ֵ��Ի���İ󶨱�����ֵ���浽�����ļ��� (�ؼ���ֵ��>����->�ļ��洢ֵ)
����ֵ��   0 - �ɹ�   ��0 - ���ɹ�
*******************************************************************/
int OC_Item_Moto_TestItem::SaveOption()
{
	if(!TestItemInterface::SaveOption()) return -1;
	//TODO: �ڴ���Ӵ洢����	
	USES_CONVERSION;
	CString TempStr = _T("");

	TempStr.Format(_T("%f"),pDlg->m_PixelSize);
	WriteConfigString(_T("PixelSize"),TempStr);

	TempStr.Format(_T("%f"),pDlg->m_OCSpec);
	WriteConfigString(_T("OCSpec"),TempStr);

	TempStr.Format(_T("%f"),pDlg->m_OCSpec_AfterShift);
	WriteConfigString(_T("OCSpec_AfterShift"),TempStr);

	WriteConfigInt(_T("OCSpec_Shift_En"),pDlg->m_OCSpec_Shift);
	WriteConfigInt(_T("m_Continue"),pDlg->m_Continue);
	WriteConfigInt(_T("isMonoSensor"),pDlg->m_isMonoSensor);
	//........................
	return 0;
}


/******************************************************************
������:    GetReportHeaders
��������:  ������Ҫ����Ĳ�����Ŀ��������
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OC_Item_Moto_TestItem::GetReportHeads()
{
	static CString header;
	if (!pDlg->m_isMonoSensor)
	{
		header = _T("ģ����,OC_R_X,OC_R_Y,OC_R_Dis,OC_Gr_X,OC_Gr_Y,OC_Gr_Dis,OC_Gb_X,OC_Gb_Y,OC_Gb_Dis,OC_B_X,OC_B_Y,OC_B_Dis,OC_X,OC_Y,PixelSize");
	}
	else header = _T("ģ����,OptiX,OptiY,OC_Dis,OC_X,OC_Y,PixelSize");
	return header;
}


/******************************************************************
������:    GetReportLowerLimits
��������:  ������Ա�׼������ֵ
����ֵ��   �ַ���ָ��
*******************************************************************/
LPCTSTR OC_Item_Moto_TestItem::GetReportLowLimit()
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
LPCTSTR OC_Item_Moto_TestItem::GetReportHightLimit()
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
LPCTSTR OC_Item_Moto_TestItem::GetReportContents()
{
	static CString Content;
	CString CamCode=m_pInterface->Ctrl_GetCamCode();
	float PC_x=CenterX;
	float PC_y=CenterY;

	if (!pDlg->m_isMonoSensor)
	{
		Content.Format(_T("%s,%d,%d,%f,%d,%d,%f,%d,%d,%f,%d,%d,%f,%f,%f,%f"),CamCode,CenterOut.R_OpticalPixel.xIndex,CenterOut.R_OpticalPixel.yIndex,CenterOut.R_OpticalOut,CenterOut.Gr_OpticalPixel.xIndex,CenterOut.Gr_OpticalPixel.yIndex,CenterOut.Gr_OpticalOut,CenterOut.Gb_OpticalPixel.xIndex,CenterOut.Gb_OpticalPixel.yIndex,CenterOut.Gb_OpticalOut,CenterOut.B_OpticalPixel.xIndex,CenterOut.B_OpticalPixel.yIndex,CenterOut.B_OpticalOut,PC_x,PC_y,pDlg->m_PixelSize);
	}
	else 
	{
		Content.Format(_T("%s,%d,%d,%f,%d,%d,%f"),CamCode,CenterOut_Mono.OpticalPixel.xIndex,CenterOut_Mono.OpticalPixel.yIndex,CenterOut_Mono.OC_Out,PC_x,PC_y,pDlg->m_PixelSize);
	}
	
	return Content;
}

void Raw2Channels( USHORT* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, OFOPTCenterPixInfo* pGr, OFOPTCenterPixInfo* pR, OFOPTCenterPixInfo* pB,  OFOPTCenterPixInfo* pGb )
{
	int GbCnt = 0;
	int GrCnt = 0;
	int BCnt  = 0;
	int RCnt  = 0;

	switch(nPixOrder)
	{
	case 0: //rggb
		{
			for( int i = 0; i < nImgHeight;  i++ )
			{
				for( int j = 0; j < nImgWidth-1; j = j + 2 )
				{
					if( i%2 == 1 )
					{
						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pGb[GbCnt].xIndex = j;
						pGb[GbCnt].yIndex = i;
						GbCnt++;

						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
						pB[BCnt].xIndex = (j+1);
						pB[BCnt].yIndex = i;
						BCnt++; 
					}
					else
					{
						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pR[RCnt].xIndex = j;
						pR[RCnt].yIndex = i;
						RCnt++;

						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pGr[GrCnt].xIndex = (j+1);
						pGr[GrCnt].yIndex = i;
						GrCnt++;
					} 
				}
			}

		}
		break;

	case 1: //OUTFORMAT_GRBG
		{
			for( int i = 0; i < nImgHeight;  i++ )
			{
				for( int j = 0; j < nImgWidth-1; j = j + 2 )
				{
					if( i%2 == 1 )
					{
						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pB[BCnt].xIndex = j;
						pB[BCnt].yIndex = i;
						BCnt++;

						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
						pGb[GbCnt].xIndex = (j+1);
						pGb[GbCnt].yIndex = i;
						GbCnt++; 
					}
					else
					{
						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pGr[GrCnt].xIndex = j;
						pGr[GrCnt].yIndex = i;
						GrCnt++;

						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pR[RCnt].xIndex = (j+1); 
						pR[RCnt].yIndex = i;
						RCnt++;
					} 
				}
			}			
		}
		break;

	case 2: //OUTFORMAT_GBRG
		{
			for( int i = 0; i < nImgHeight;  i++ )
			{
				for( int j = 0; j < nImgWidth-1; j = j + 2 )
				{
					if( i%2 == 1 )
					{
						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pR[RCnt].xIndex = j;
						pR[RCnt].yIndex = i;
						RCnt++;

						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
						pGr[GrCnt].xIndex = (j+1);
						pGr[GrCnt].yIndex = i;
						GrCnt++; 
					}
					else
					{
						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pGb[GbCnt].xIndex = j;
						pGb[GbCnt].yIndex = i;
						GbCnt++;

						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pB[BCnt].xIndex = (j+1);
						pB[BCnt].yIndex = i;
						BCnt++;
					} 
				}
			}			
		}
		break;

	case 3: //OUTFORMAT_BGGR
		{
			for( int i = 0; i < nImgHeight;  i++ )
			{
				for( int j = 0; j < nImgWidth-1; j = j + 2 )
				{
					if( i%2 == 1 )
					{
						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pGr[GrCnt].xIndex = j;
						pGr[GrCnt].yIndex = i;
						GrCnt++;

						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
						pR[RCnt].xIndex = (j+1);
						pR[RCnt].yIndex = i;
						RCnt++; 
					}
					else
					{
						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
						pB[BCnt].xIndex = j;
						pB[BCnt].yIndex = i;
						BCnt++;

						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
						pGb[GbCnt].xIndex = (j+1);
						pGb[GbCnt].yIndex = i;
						GbCnt++;
					} 
				}
			}			
		}
		break;

	default:
		break;
	} 
}

void getNewPanel(OFOPTCenterPixInfo* pPanel,OFOPTCenterPixInfo* outpPanel,int nImgWidth ,int nImgHeight)
{
	long nCnt= 0;	
	int RawFileWidth = nImgWidth/2 ;
	int RawFileHeight= nImgHeight/2 ;
	int Brightness = 0;
	int i=0,j=0;
	for( j = 0; j < RawFileHeight; j++ )
	{
		for(i = 0; i < RawFileWidth; i++ )
		{
			outpPanel[nCnt].xIndex  = pPanel[j*RawFileWidth+i].xIndex;
			outpPanel[nCnt].yIndex  = pPanel[j*RawFileWidth+i].yIndex;
			outpPanel[nCnt].Y_Value = pPanel[j*RawFileWidth+i].Y_Value;
			nCnt++;

			outpPanel[nCnt].xIndex  = pPanel[j*RawFileWidth+i].xIndex+1;
			outpPanel[nCnt].yIndex  = pPanel[j*RawFileWidth+i].yIndex;
			if ((i+1) < RawFileWidth)
			{
				outpPanel[nCnt].Y_Value = (pPanel[j*RawFileWidth+i].Y_Value+pPanel[j*RawFileWidth+i+1].Y_Value)/2;
			}
			else
			{
				outpPanel[nCnt].Y_Value = pPanel[j*RawFileWidth+i].Y_Value;
			}		
			nCnt++;
		}
		nCnt = nCnt + nImgWidth;
	}

	nCnt = 0;
	for( j = 0; j < nImgHeight; j=j+2 )
	{
		nCnt = nCnt + nImgWidth;
		for( int i = 0; i < nImgWidth; i++ )
		{
			outpPanel[nCnt].xIndex  = outpPanel[j*nImgWidth+i].xIndex;
			outpPanel[nCnt].yIndex  = outpPanel[j*nImgWidth+i].yIndex+1;
			if ((j+2) < nImgHeight)
			{
				outpPanel[nCnt].Y_Value = (outpPanel[j*nImgWidth+i].Y_Value+outpPanel[(j+2)*nImgWidth+i].Y_Value)/2;
			}
			else
			{
				outpPanel[nCnt].Y_Value = outpPanel[j*nImgWidth+i].Y_Value;
			}
			nCnt++;
		}
	}
}

void InsertionSort(OFOPTCenterPixInfo* pPanel,int ImageWidth,int ImageHeight)
{
	//���뷨����
	OFOPTCenterPixInfo pPixel;

	for (long i = 1;i < (ImageWidth*ImageHeight);i++)
	{
		if (pPanel[i].Y_Value > pPanel[i-1].Y_Value)
		{
			pPixel.Y_Value = pPanel[i].Y_Value;
			pPixel.xIndex  = pPanel[i].xIndex ;
			pPixel.yIndex  = pPanel[i].yIndex ;

			long j = (i - 1);
			while((j>=0)&&(pPixel.Y_Value > pPanel[j].Y_Value))
			{
				pPanel[j+1].Y_Value = pPanel[j].Y_Value;
				pPanel[j+1].xIndex  = pPanel[j].xIndex;
				pPanel[j+1].yIndex  = pPanel[j].yIndex;	
				j--;
			}

			pPanel[j+1].Y_Value = pPixel.Y_Value;
			pPanel[j+1].xIndex  = pPixel.xIndex;
			pPanel[j+1].yIndex  = pPixel.yIndex;
		}
	}
}

void getOCCoord(OFOPTCenterPixInfo* pPanel,OFOPTCenterPixInfo *CenterPixInfo,int ImageWidth,int ImageHeight)
{
	signed __int64 x_Checksum = 0;
	signed __int64 y_Checksum = 0;
	signed __int64 Checksum = 0;
	int  nCnt = 0;
	int Index = (int)(ImageWidth*ImageHeight*0.1);
	int MaxBrightness = 0;
	int k=0;
	int n=0;
	int m=0;

	for (k = 0 ; k < ImageHeight;k++)      
	{
		for (int n = 0; n < ImageWidth;n++)
		{
			if (pPanel[n+k*ImageWidth].Y_Value > MaxBrightness)
			{
				MaxBrightness = pPanel[n+k*ImageWidth].Y_Value;
			}
		}	
	}

	for (m = MaxBrightness; m > 0;m--)
	{
		for (k = 0 ; k < ImageHeight;k++)      
		{
			for (n = 0; n < ImageWidth;n++)
			{
				if (pPanel[n+k*ImageWidth].Y_Value==m)
				{
					x_Checksum += ((pPanel[n+k*ImageWidth].xIndex)*(pPanel[n+k*ImageWidth].Y_Value));
					y_Checksum += ((pPanel[n+k*ImageWidth].yIndex)*(pPanel[n+k*ImageWidth].Y_Value));
					Checksum += pPanel[n+k*ImageWidth].Y_Value;
					nCnt++;
				}
			}	
		}

		if (nCnt>Index)
		{
			x_Checksum = (1.0*x_Checksum/Checksum + 0.5);
			y_Checksum = (1.0*y_Checksum/Checksum + 0.5);
			break;
		}
	}

	CenterPixInfo->xIndex = x_Checksum;
	CenterPixInfo->yIndex = y_Checksum;
}


void getOptCenter(OFOPTCenterPixInfo* pPanel,OFOPTCenterPixInfo *CenterPixInfo,int ImageWidth,int ImageHeight,double *Distance,SensorFormat ID)
{
	if (ID == BAYER)
	{
		getOCCoord(pPanel,CenterPixInfo,ImageWidth/2,ImageHeight/2);      //��ȡOC����ֵ
	}
	else getOCCoord(pPanel,CenterPixInfo,ImageWidth,ImageHeight);      //��ȡOC����ֵ


	//�����Ӧchannel�ļ�������
	float xIndex_1 = (ImageWidth-1)/2.0; 
	float yIndex_1 = (ImageHeight-1)/2.0;

	* Distance = sqrt(pow((CenterPixInfo->xIndex - xIndex_1),2)+pow((CenterPixInfo->yIndex - yIndex_1),2));
}

BOOL OC_Item_Moto_TestItem::OpticalCenterTest(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut * _OFOPTCenterOut)
{
	//���4channel����Ϣ
	OFOPTCenterPixInfo *pGr = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4];
	OFOPTCenterPixInfo *pB  = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4];
	OFOPTCenterPixInfo *pR  = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4];
	OFOPTCenterPixInfo *pGb = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)/4];

	if (NULL == pGr || NULL == pGb || NULL == pR || NULL == pB)
	{
		SAFE_DELETE_ARRAY(pGr);
		SAFE_DELETE_ARRAY(pGb);
		SAFE_DELETE_ARRAY(pB);
		SAFE_DELETE_ARRAY(pR);
		return FALSE;
	}

	//��Rawͼ��4��Channel
	Raw2Channels( _OFOPTCenterIn->ImageBufferIn, _OFOPTCenterIn->Width,_OFOPTCenterIn->Height, _OFOPTCenterIn->Outformat, pGr, pR, pB, pGb );

	//��ȡOptlcal center�������distance
	OFOPTCenterPixInfo  *OPTCenterPix = new OFOPTCenterPixInfo;
	if (NULL == OPTCenterPix)
	{
		SAFE_DELETE_ARRAY(pGr);
		SAFE_DELETE_ARRAY(pGb);
		SAFE_DELETE_ARRAY(pB);
		SAFE_DELETE_ARRAY(pR);
		return FALSE;
	}
	double m_Distance = 0;

	getOptCenter(pR, OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,BAYER);

	_OFOPTCenterOut->R_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->R_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->R_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	getOptCenter(pGr,OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,BAYER);

	_OFOPTCenterOut->Gr_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->Gr_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->Gr_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	getOptCenter(pGb,OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,BAYER);

	_OFOPTCenterOut->Gb_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->Gb_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->Gb_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	getOptCenter(pB, OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,BAYER);

	_OFOPTCenterOut->B_OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->B_OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->B_OpticalOut  = m_Distance*(_OFOPTCenterIn->pixelSize);

	SAFE_DELETE_ARRAY(pGr);
	SAFE_DELETE_ARRAY(pGb);
	SAFE_DELETE_ARRAY(pB);
	SAFE_DELETE_ARRAY(pR);
	SAFE_DELETE_ARRAY(OPTCenterPix);
	return TRUE;
}


BOOL OC_Item_Moto_TestItem::OpticalCenterTest_Mono(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut_Mono * _OFOPTCenterOut)
{
	//���4channel����Ϣ
	OFOPTCenterPixInfo *pNewBuffer = new OFOPTCenterPixInfo[(_OFOPTCenterIn->Width) * (_OFOPTCenterIn->Height)];
	if (NULL == pNewBuffer)
	{
		SAFE_DELETE_ARRAY(pNewBuffer);
		return FALSE;
	}
	for (int i=0; i<_OFOPTCenterIn->Height; i++)
	{
		for (int j=0; j<_OFOPTCenterIn->Width; j++)
		{
			int Pos = i*_OFOPTCenterIn->Width + j;
			pNewBuffer[Pos].Y_Value = _OFOPTCenterIn->ImageBufferIn[Pos];
			pNewBuffer[Pos].xIndex = j;
			pNewBuffer[Pos].yIndex = i;
		}
	}

	//��ȡOptlcal center�������distance
	OFOPTCenterPixInfo  *OPTCenterPix = new OFOPTCenterPixInfo;
	if (NULL == OPTCenterPix)
	{
		SAFE_DELETE_ARRAY(pNewBuffer);
		return FALSE;
	}
	double m_Distance = 0;

	getOptCenter(pNewBuffer, OPTCenterPix,_OFOPTCenterIn->Width,_OFOPTCenterIn->Height,&m_Distance,MONO);

	_OFOPTCenterOut->OpticalPixel.xIndex  = OPTCenterPix->xIndex;
	_OFOPTCenterOut->OpticalPixel.yIndex  = OPTCenterPix->yIndex;

	_OFOPTCenterOut->OC_Out  = m_Distance*(_OFOPTCenterIn->pixelSize);

	SAFE_DELETE_ARRAY(pNewBuffer);
	SAFE_DELETE_ARRAY(OPTCenterPix);

	return TRUE;
}
