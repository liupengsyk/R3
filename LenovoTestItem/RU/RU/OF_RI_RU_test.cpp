#include "stdafx.h"
#include "OF_RI_RU_test.h"
#include <math.h>
#include <stdio.h>

#define ShrinkRatio_RU 16

void Raw2Channels( USHORT* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, OFRelativePixInfo* pGr, OFRelativePixInfo* pR, OFRelativePixInfo* pB,  OFRelativePixInfo* pGb )
{
	int GbCnt = 0;
	int GrCnt = 0;
	int BCnt  = 0;
	int RCnt  = 0;

	switch( nPixOrder)
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

void getNewPanel(OFRelativePixInfo* pPanel,OFRelativePixInfo* outpPanel,int nImgWidth ,int nImgHeight)
{
	int Cnt=0;
	int newWidth=(nImgWidth/2); 
	int newHeight=(nImgHeight/2);
	int position=0;
	int firstWidth = ShrinkRatio_RU;
	
	for( int i = 0; i < newHeight-1;  i=i+firstWidth )
	{
		for( int j = 0; j < newWidth-1; j = j + firstWidth )
		{
            if (j+firstWidth>newWidth)
            {
				j = newWidth - firstWidth;
            }
			if (i+firstWidth >newHeight)
			{
				i = newHeight - firstWidth;
			}
			
			position=i*newWidth+j;
			outpPanel[Cnt].xIndex=pPanel[i*newWidth+j].xIndex;
			outpPanel[Cnt].yIndex=pPanel[i*newWidth+j].yIndex;
			outpPanel[Cnt].Y_Value=0;
			long SumSum = 0;
			for (int h=0;h<firstWidth;h++)
			{
				for (int w=0;w<firstWidth;w++)
				{
					SumSum += pPanel[(i+h)*newWidth+j+w].Y_Value;
				}
			}
			outpPanel[Cnt].Y_Valuef = (SumSum*1.0/(firstWidth*firstWidth));
			outpPanel[Cnt].Y_Value=(int)(SumSum*1.0/(firstWidth*firstWidth)+0.5);
			Cnt++;
		}
	} 
}

void getNewPanel_RI(OFRelativePixInfo* pPanel,OFRelativePixInfo* outpPanel,int nImgWidth ,int nImgHeight)
{
	int Cnt=0;
	int newWidth=(nImgWidth/2); 
	int newHeight=(nImgHeight/2);
	int position=0;
	int firstWidth = (int)ceil(1.0*newWidth/32.0);
	int firstHeight= (int)ceil(1.0*newHeight/24.0);
	long y_Value = 0;
	
	for( int i = 0; i < newHeight-1;  i=i+firstHeight )
	{
		for( int j = 0; j < newWidth-1; j = j + firstWidth )
		{
            if (j+firstWidth>newWidth)
            {
				j = newWidth - firstWidth;
            }
			if (i+firstHeight >newHeight)
			{
				i = newHeight - firstHeight;
			}
			
			position=i*newWidth+j;
			outpPanel[Cnt].xIndex=pPanel[i*newWidth+j].xIndex;
			outpPanel[Cnt].yIndex=pPanel[i*newWidth+j].yIndex;
			outpPanel[Cnt].Y_Value=0;
			
			y_Value = 0;
		
			for (int h=0;h<firstHeight;h++)
			{
				for (int w=0;w<firstWidth;w++)
				{
					y_Value += pPanel[(i+h)*newWidth+j+w].Y_Value;
				}
			}
			outpPanel[Cnt].Y_Valuef = (y_Value*1.0/(firstWidth*firstHeight));
			outpPanel[Cnt].Y_Value=(int)(y_Value*1.0/(firstWidth*firstHeight)+0.5);
			Cnt++;
		}
	} 
}

BOOL GetRIFromPannel(OFRelativePixInfo* pPanel, int nWidth, int nHeight,double &RIvalue,double *AllRIValue)
{
	int finalWidth = 32;
	int finalHeight= 24;
	
	double MaxUI = 0.0;
	double MinUI = 1024.0;

	int MaxPos = 0;
	
	for( int i = 0; i < finalHeight; i++ )
	{
		for( int j = 0; j < finalWidth; j++)
		{
			if(pPanel[i*finalWidth+j].Y_Valuef > MaxUI)
			{
				MaxUI = pPanel[i*finalWidth+j].Y_Valuef;
				MaxPos = i*finalWidth+j;
			}
			
			if(pPanel[i*finalWidth+j].Y_Valuef < MinUI)
			{
				MinUI = pPanel[i*finalWidth+j].Y_Valuef;
			}
		}	
	}  
	
	RIvalue = (MinUI*1.0/MaxUI)*100;

	for(int i = 0; i < finalHeight; i++ )
	{
		for(int j = 0; j < finalWidth; j++)
		{
			int Pos = i*finalWidth+j;
			AllRIValue[Pos] = pPanel[Pos].Y_Valuef/pPanel[MaxPos].Y_Valuef;
		}	
	} 

	return TRUE;
}

double GetSurroundingAreaMax( OFRelativePixInfo* pPanel, int nWidth, int nHeight, int X, int Y)
{
	double RU_Value_Max = 0.0;
	double m_RUtemp = 0.0;
	int nCnt = 0;
	
	if ( (X-1) >= 0 )
	{
		m_RUtemp=(fabs(pPanel[Y*nWidth+X].Y_Valuef - pPanel[Y*nWidth+X-1].Y_Valuef)/(pPanel[Y*nWidth+X].Y_Valuef))*100;	
		
		if (RU_Value_Max < m_RUtemp)
		{
            RU_Value_Max = m_RUtemp;
		}
	}
	
	if ( (X+1) < nWidth )
	{
		m_RUtemp=(fabs(pPanel[Y*nWidth+X].Y_Valuef - pPanel[Y*nWidth+X+1].Y_Valuef)/(pPanel[Y*nWidth+X].Y_Valuef))*100;	
		
		if (RU_Value_Max < m_RUtemp)
		{
            RU_Value_Max = m_RUtemp;
		}
	}
	
	if ( (Y-1) >= 0 )
	{
		m_RUtemp=(fabs(pPanel[Y*nWidth+X].Y_Valuef - pPanel[(Y-1)*nWidth+X].Y_Valuef)/(pPanel[Y*nWidth+X].Y_Valuef))*100;	
		
		if (RU_Value_Max < m_RUtemp)
		{
            RU_Value_Max = m_RUtemp;
		}
	}
	
	if ( (Y+1) < nHeight )
	{
		m_RUtemp=(fabs(pPanel[Y*nWidth+X].Y_Valuef - pPanel[(Y+1)*nWidth+X].Y_Valuef)/(pPanel[Y*nWidth+X].Y_Valuef))*100;	
		
		if (RU_Value_Max < m_RUtemp)
		{
            RU_Value_Max = m_RUtemp;
		}
	}
	return RU_Value_Max;
}

BOOL GetRUFromPannel(OFRelativePixInfo* pPanel, int nWidth, int nHeight,double &RUvalue,double &Shadingvalue,double *AllRU_Value)
{
	int finalWidth = (int)ceil((nWidth/2.0)/ShrinkRatio_RU);
	int finalHeight= (int)ceil((nHeight/2.0)/ShrinkRatio_RU);
	 
	double MaxShading= 0.0;
	double MaxRU     = 0.0;
	double temp      = 0.0;
	double m_RUtemp  = 0.0;
	
	for( int i = 0; i < finalHeight; i++ )
	{
		for( int j = 0; j < finalWidth; j++)
		{
			if(((i==0)&&(j==0))||((i==0)&&(j==(finalWidth-1)))||((i==(finalHeight-1))&&(j==0))||((i==(finalHeight-1))&&(j==(finalWidth-1))))
			{
				m_RUtemp = GetSurroundingAreaMax(pPanel, finalWidth,finalHeight, j,i);
				
				if (m_RUtemp > MaxShading)
				{
					MaxShading = m_RUtemp;
				}
			}
			else
			{
				m_RUtemp = GetSurroundingAreaMax(pPanel, finalWidth,finalHeight, j,i);
				
				if (m_RUtemp > MaxRU)
				{
					MaxRU = m_RUtemp;
				}
			}
			AllRU_Value[i*finalWidth+j] = m_RUtemp;
		}		
	}  

    Shadingvalue = MaxShading;
    RUvalue      = MaxRU;

	return TRUE;
}

void RelativeIlluminationTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, RIValue *pRIValue)
{
	OFRelativePixInfo *pGr = new OFRelativePixInfo[nImgWidth * nImgHeight/4];
	OFRelativePixInfo *pB  = new OFRelativePixInfo[nImgWidth * nImgHeight/4];
	OFRelativePixInfo *pR  = new OFRelativePixInfo[nImgWidth * nImgHeight/4]; 
	OFRelativePixInfo *pGb = new OFRelativePixInfo[nImgWidth * nImgHeight/4];  
	
	OFRelativePixInfo *newpGr = new OFRelativePixInfo[nImgWidth * nImgHeight/4];
	OFRelativePixInfo *newpB  = new OFRelativePixInfo[nImgWidth * nImgHeight/4];
	OFRelativePixInfo *newpR  = new OFRelativePixInfo[nImgWidth * nImgHeight/4];  
	OFRelativePixInfo *newpGb = new OFRelativePixInfo[nImgWidth * nImgHeight/4];  
	
	USHORT * m_pBuff= new USHORT[nImgWidth*nImgHeight]; 

	for( int i = 0; i < nImgHeight; i++ )
	{
		for( int j = 0; j < nImgWidth; j++ )
		{
			m_pBuff[i*nImgWidth+j] = ((pRawImage[(i*nImgWidth+j)*2+1]<<8)+pRawImage[(i*nImgWidth+j)*2]);
		}
	}

	Raw2Channels( m_pBuff, nImgWidth, nImgHeight, nPixOrder, pGr, pR, pB, pGb );
	
	getNewPanel_RI(pR,newpR,nImgWidth,nImgHeight);
	getNewPanel_RI(pGr,newpGr,nImgWidth,nImgHeight);
	getNewPanel_RI(pB,newpB,nImgWidth,nImgHeight);
	getNewPanel_RI(pGb,newpGb,nImgWidth,nImgHeight);
	
	GetRIFromPannel(newpGr,nImgWidth,nImgHeight,pRIValue->RI_GrChannel,pRIValue->RI_GrChannelValue);
    GetRIFromPannel(newpGb,nImgWidth,nImgHeight,pRIValue->RI_GbChannel,pRIValue->RI_GbChannelValue);	
	GetRIFromPannel(newpR, nImgWidth,nImgHeight,pRIValue->RI_RChannel,pRIValue->RI_RChannelValue);
    GetRIFromPannel(newpB, nImgWidth,nImgHeight,pRIValue->RI_BChannel,pRIValue->RI_BChannelValue);	
   	
	delete [] pGr;
	delete [] pB;
	delete [] pR;
	delete [] pGb;
	
	delete [] newpGr;
	delete [] newpB;
	delete [] newpR;
	delete [] newpGb; 

	delete [] m_pBuff;
	
    return;
}

void RelativeUniformityTest(BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, RUValue *pRUValue)
{
	OFRelativePixInfo *pGr = new OFRelativePixInfo[nImgWidth * nImgHeight/4];
	OFRelativePixInfo *pB  = new OFRelativePixInfo[nImgWidth * nImgHeight/4];
	OFRelativePixInfo *pR  = new OFRelativePixInfo[nImgWidth * nImgHeight/4]; 
	OFRelativePixInfo *pGb = new OFRelativePixInfo[nImgWidth * nImgHeight/4];  
	
	OFRelativePixInfo *newpGr = new OFRelativePixInfo[nImgWidth * nImgHeight/4];
	OFRelativePixInfo *newpB  = new OFRelativePixInfo[nImgWidth * nImgHeight/4];
	OFRelativePixInfo *newpR  = new OFRelativePixInfo[nImgWidth * nImgHeight/4];  
	OFRelativePixInfo *newpGb = new OFRelativePixInfo[nImgWidth * nImgHeight/4];  
		
	USHORT * m_pBuff= new USHORT[nImgWidth*nImgHeight]; 
	
	for( int i = 0; i < nImgHeight; i++ )
	{
		for( int j = 0; j < nImgWidth; j++ )
		{
			m_pBuff[i*nImgWidth+j] = ((pRawImage[(i*nImgWidth+j)*2+1]<<8)+pRawImage[(i*nImgWidth+j)*2]);
		}
	}

	Raw2Channels( m_pBuff, nImgWidth, nImgHeight, nPixOrder, pGr, pR, pB, pGb );
	
	getNewPanel(pR,newpR,nImgWidth,nImgHeight);
	getNewPanel(pGr,newpGr,nImgWidth,nImgHeight);
	getNewPanel(pB,newpB,nImgWidth,nImgHeight);
	getNewPanel(pGb,newpGb,nImgWidth,nImgHeight);
	
	GetRUFromPannel(newpGr,nImgWidth,nImgHeight,pRUValue->RU_GrChannel,pRUValue->Shding_GrChannel,pRUValue->RU_GrChannelValue);
	GetRUFromPannel(newpGb,nImgWidth,nImgHeight,pRUValue->RU_GbChannel,pRUValue->Shding_GbChannel,pRUValue->RU_GbChannelValue);
	GetRUFromPannel(newpR, nImgWidth,nImgHeight,pRUValue->RU_RChannel, pRUValue->Shding_RChannel,pRUValue->RU_RChannelValue);
	GetRUFromPannel(newpB, nImgWidth,nImgHeight,pRUValue->RU_BChannel, pRUValue->Shding_BChannel,pRUValue->RU_BChannelValue);
	
	delete [] pGr;
	delete [] pB;
	delete [] pR;
	delete [] pGb;
	
	delete [] newpGr;
	delete [] newpB;
	delete [] newpR;
	delete [] newpGb; 

	delete [] m_pBuff;
	
    return;
}