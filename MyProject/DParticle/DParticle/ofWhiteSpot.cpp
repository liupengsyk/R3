// OFPOD.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ofWhiteSpot.h"
#include <math.h>
#include <stdio.h>


#include <stdlib.h>

struct ImagepodPoint
{
	int x;
	int y;
	
};

struct ImagepodSize
{
	int cx;
	int cy;
};
struct WhiteSpotStand
{
	int nTopSL;     //不计算POD的区域
	int nBottomSL;	//不计算POD的区域
	int nLeftSL;    //不计算POD的区域
	int nRightSL;   //不计算POD的区域
	
	int useLog;
	
	int nWoundTopSL;     //不计算POD的区域
	int nWoundBottomSL;	//不计算POD的区域
	int nWoundLeftSL;    //不计算POD的区域
	int nWoundRightSL;   //不计算POD的区域
	
	int nAreaWidth;  //计算平均值所选取区域的大小
	int nAreaHeight; //计算平均值所选取区域的大小 
    
	int smallnAreaWidth;  //计算平均值所选取区域的大小
	int smallnAreaHeight; //计算平均值所选取区域的大小 
	
	int DeadUnit; //1 for%, 0 for pixel value;
	int Deadpc;
	int ClusterType; 	
	
};

BOOL bSaveRawFile( char* sfilename, BYTE *pBuffer, UINT width, UINT height,float type)
{
	HFILE rawFile;

	DWORD dwSizeImage = width * height*type;

	rawFile = _lcreat( sfilename, 0 );

	if( rawFile < 0 )

		return FALSE;

	UINT len;

	len = _lwrite( rawFile, (LPSTR)pBuffer, dwSizeImage );

	_lclose( rawFile );

	return TRUE;
}
void Raw2Channels( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, WPixInfo* pPanel, int nPanelFlag )
{//nPanelFlag     1: pGr   2:pR   3:pB     4:pGb
	int GbCnt = 0;
	int GrCnt = 0;
	int BCnt =0;
	int RCnt = 0;

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
						if (nPanelFlag ==4)
						{
							pPanel[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
							// pPanel[GbCnt].Average=pRawImage[i*nImgWidth+j];
							pPanel[GbCnt].xIndex = j/2;
							pPanel[GbCnt].yIndex = i/2;
							GbCnt++;
						}

						else if (nPanelFlag ==3)
						{
							pPanel[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
							//  pPanel[BCnt].Average=pRawImage[i*nImgWidth+j+1];	
							pPanel[BCnt].xIndex = (j+1)/2;
							pPanel[BCnt].yIndex = i/2;
							BCnt++; 
						}
					}
					else
					{
						if (nPanelFlag ==2)
						{
							pPanel[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pPanel[GbCnt].Average=pRawImage[i*nImgWidth+j];
							pPanel[RCnt].xIndex = j/2;
							pPanel[RCnt].yIndex = i/2;
							RCnt++;
						}

						else if (nPanelFlag==1)
						{
							pPanel[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//  pPanel[GrCnt].Average=pRawImage[i*nImgWidth+j+1];
							pPanel[GrCnt].xIndex = (j+1)/2;
							pPanel[GrCnt].yIndex = i/2;
							GrCnt++;
						}
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
						if (nPanelFlag==3)
						{
							pPanel[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pPanel[BCnt].Average = pRawImage[i*nImgWidth+j];
							pPanel[BCnt].xIndex = j/2;
							pPanel[BCnt].yIndex = i/2;
							BCnt++;
						}

						else if (nPanelFlag==4)
						{
							pPanel[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pPanel[GbCnt].Average = pRawImage[i*nImgWidth+j+1];
							pPanel[GbCnt].xIndex = (j+1)/2;
							pPanel[GbCnt].yIndex = i/2;
							GbCnt++;
						}
					}
					else
					{
						if (nPanelFlag==1)
						{
							pPanel[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pPanel[GrCnt].Average = pRawImage[i*nImgWidth+j];
							pPanel[GrCnt].xIndex = j/2;
							pPanel[GrCnt].yIndex = i/2;
							GrCnt++;
						}

						else if (nPanelFlag==2)
						{
							pPanel[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pPanel[RCnt].Average = pRawImage[i*nImgWidth+j+1];
							pPanel[RCnt].xIndex = (j+1)/2;
							pPanel[RCnt].yIndex = i/2;
							RCnt++;
						}
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
						if (nPanelFlag ==2)
						{
							pPanel[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pPanel[RCnt].Average = pRawImage[i*nImgWidth+j];
							pPanel[RCnt].xIndex = j/2;
							pPanel[RCnt].yIndex = i/2;
							RCnt++;
						}

						else if(nPanelFlag==1)
						{
							pPanel[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pPanel[GrCnt].Average = pRawImage[i*nImgWidth+j+1];
							pPanel[GrCnt].xIndex = (j+1)/2;
							pPanel[GrCnt].yIndex = i/2;
							GrCnt++; 
						}
					}
					else
					{
						if(nPanelFlag==4)
						{
							pPanel[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pPanel[GbCnt].Average = pRawImage[i*nImgWidth+j];
							pPanel[GbCnt].xIndex = j/2;
							pPanel[GbCnt].yIndex = i/2;
							GbCnt++;
						}

						else if(nPanelFlag==3)
						{
							pPanel[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pPanel[BCnt].Average = pRawImage[i*nImgWidth+j+1];
							pPanel[BCnt].xIndex = (j+1)/2;
							pPanel[BCnt].yIndex = i/2;
							BCnt++;
						}
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
						if(nPanelFlag==1)
						{
							pPanel[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pPanel[GrCnt].Average = pRawImage[i*nImgWidth+j];
							pPanel[GrCnt].xIndex = j/2;
							pPanel[GrCnt].yIndex = i/2;
							GrCnt++;
						}

						else if(nPanelFlag==2)
						{
							pPanel[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
							//	pPanel[RCnt].Average = pRawImage[i*nImgWidth+j+1];
							pPanel[RCnt].xIndex = (j+1)/2;
							pPanel[RCnt].yIndex = i/2;
							RCnt++; 
						}
					}
					else
					{
						if(nPanelFlag==3)
						{
							pPanel[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pPanel[BCnt].Average = pRawImage[i*nImgWidth+j];
							pPanel[BCnt].xIndex = j/2;
							pPanel[BCnt].yIndex = i/2;
							BCnt++;
						}

						else if(nPanelFlag==4)
						{
							pPanel[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pPanel[GbCnt].Average = pRawImage[i*nImgWidth+j+1];
							pPanel[GbCnt].xIndex = (j+1)/2;
							pPanel[GbCnt].yIndex = i/2;
							GbCnt++;
						}
					} 
				}
			}			
		}
		break;

	default:
		break;
	} 
}

void Raw2Channels( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, WPixInfo* pGr, WPixInfo* pR, WPixInfo* pB,  WPixInfo* pGb )
{

	int GbCnt = 0;
	int GrCnt = 0;
	int BCnt =0;
	int RCnt = 0;

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
                       // pGb[GbCnt].Average=pRawImage[i*nImgWidth+j];
						pGb[GbCnt].xIndex = j/2;
						pGb[GbCnt].yIndex = i/2;
						GbCnt++;
						
						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
                      //  pB[BCnt].Average=pRawImage[i*nImgWidth+j+1];	
						pB[BCnt].xIndex = (j+1)/2;
						pB[BCnt].yIndex = i/2;
						BCnt++; 
					
					}
					else
					{
						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
					//	pGb[GbCnt].Average=pRawImage[i*nImgWidth+j];
						pR[RCnt].xIndex = j/2;
						pR[RCnt].yIndex = i/2;
						RCnt++;
						
						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
                      //  pGr[GrCnt].Average=pRawImage[i*nImgWidth+j+1];
						pGr[GrCnt].xIndex = (j+1)/2;
						pGr[GrCnt].yIndex = i/2;
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
					//	pB[BCnt].Average = pRawImage[i*nImgWidth+j];
						pB[BCnt].xIndex = j/2;
						pB[BCnt].yIndex = i/2;
						BCnt++;
						
						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
					//	pGb[GbCnt].Average = pRawImage[i*nImgWidth+j+1];
						pGb[GbCnt].xIndex = (j+1)/2;
						pGb[GbCnt].yIndex = i/2;
						GbCnt++; 
					}
					else
					{
						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
					//	pGr[GrCnt].Average = pRawImage[i*nImgWidth+j];
						pGr[GrCnt].xIndex = j/2;
						pGr[GrCnt].yIndex = i/2;
						GrCnt++;
						
						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
					//	pR[RCnt].Average = pRawImage[i*nImgWidth+j+1];
						pR[RCnt].xIndex = (j+1)/2;
						pR[RCnt].yIndex = i/2;
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
					//	pR[RCnt].Average = pRawImage[i*nImgWidth+j];
						pR[RCnt].xIndex = j/2;
						pR[RCnt].yIndex = i/2;
						RCnt++;
						
						pGr[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
					//	pGr[GrCnt].Average = pRawImage[i*nImgWidth+j+1];
						pGr[GrCnt].xIndex = (j+1)/2;
						pGr[GrCnt].yIndex = i/2;
						GrCnt++; 
					}
					else
					{
						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
					//	pGb[GbCnt].Average = pRawImage[i*nImgWidth+j];
						pGb[GbCnt].xIndex = j/2;
						pGb[GbCnt].yIndex = i/2;
						GbCnt++;
						
						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
					//	pB[BCnt].Average = pRawImage[i*nImgWidth+j+1];
						pB[BCnt].xIndex = (j+1)/2;
						pB[BCnt].yIndex = i/2;
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
					//	pGr[GrCnt].Average = pRawImage[i*nImgWidth+j];
						pGr[GrCnt].xIndex = j/2;
						pGr[GrCnt].yIndex = i/2;
						GrCnt++;
						
						pR[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
					//	pR[RCnt].Average = pRawImage[i*nImgWidth+j+1];
						pR[RCnt].xIndex = (j+1)/2;
						pR[RCnt].yIndex = i/2;
						RCnt++; 
					}
					else
					{
						pB[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
					//	pB[BCnt].Average = pRawImage[i*nImgWidth+j];
						pB[BCnt].xIndex = j/2;
						pB[BCnt].yIndex = i/2;
						BCnt++;
						
						pGb[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
					//	pGb[GbCnt].Average = pRawImage[i*nImgWidth+j+1];
						pGb[GbCnt].xIndex = (j+1)/2;
						pGb[GbCnt].yIndex = i/2;
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

struct WPixInfoStack
{
	WPixInfo *pBrightDeadStk;
	int nDeadCnt;
};
void CheckBlockAreaSetting(int& x0, 
						   int& y0, 
						   UINT  width, 
						   UINT  height, 
						   UINT  ImageWidth, 
						   UINT  ImageHeight)
{
    x0 = x0 - width  / 2;  // x0 is center x
	y0 = y0 - height / 2;  // y0 is center y
	
	if (x0 < 0)
	{
		x0 = 0;
	}
	
	if (y0 < 0)
	{
		y0 = 0;
	}
	
	if (x0 + width > ImageWidth)
	{
		x0 = ImageWidth - width;
		
		if ( x0 < 0 )
		{
			x0 = 0;
		}
	}
	
	if (y0 + height > ImageHeight )
	{
		y0 = ImageHeight - height;
		
		if ( y0 < 0 )
		{
			y0 = 0;
		}
	}
}

void GetMeanMinMaxValueFromBlock(WPixInfo *pIn, int &YAve, int &YMost, int &YMin, UINT nWidth, UINT nHeight, ImagepodPoint *pStartPix, ImagepodSize *pSize)
{
	int nAreaWidth  = pSize->cx;
	int nAreaHeight = pSize->cy;
	
	if ( (nAreaHeight == 0) || (nAreaWidth < 2) ||(nWidth == 0) || (nHeight == 0) )
	{
		return ;//error code 
	}
	
	CheckBlockAreaSetting(pStartPix->x,
						  pStartPix->y,
						  nAreaWidth,
						  nAreaHeight,
						  nWidth,
						  nHeight);
	
	
	if (pStartPix->x % 2)
	{
		pStartPix->x++;
		nAreaWidth--;
	}
	
    if (nAreaWidth % 2)
	{
		nAreaWidth--;
	}
	
	BYTE PixY = 0; 
	
	LONG Y_Sum=0; 
	
	
	int Y_Most = 0;
	int Y_Min = 255;
	
	int i=0;
	int j=0;
	
	for (i = 0; i < nAreaHeight; i++)
	{
		for (j =  0; j < nAreaWidth; j++)
		{ 
			
			PixY = pIn[(i + pStartPix->y) * nWidth + pStartPix->x + j].Y_Value;			 
			
			Y_Sum += PixY;   //caculate the summarize of the all Y value..
			if ( PixY > Y_Most )
			{
				Y_Most = PixY;
			}
			if ( PixY < Y_Min )
			{
				Y_Min = PixY;
			}
			
		}
	}
	
	YAve  = int(Y_Sum*1.0 / (nAreaHeight*nAreaWidth)+0.5); //get   Y average..
	YMost = Y_Most;
	YMin  = Y_Min;

}

void PushDeadIntoStack(WPixInfo *Stack, WPixInfo DeadPix, int nCnt,int nImgWidth, int nImgHeight, int StackMaxSize )
{
	//int nWidth = ; 
	if( nCnt >= StackMaxSize)
		return; 

	int tempIndex = DeadPix.yIndex*nImgWidth + DeadPix.xIndex;
	int i=0;
	for(i = nCnt; i > 0; i-- )
	{
		if( Stack[i-1].yIndex * nImgWidth + Stack[i-1].xIndex > tempIndex )
		{  
			Stack[i] = Stack[i-1]; 
		}
		else
		{
			break;
		} 
	}
	
	Stack[i] = DeadPix;  
	nCnt++;   
}


float GetSurroundingAreaAve( WPixInfo* pPanel, int nWidth, int nHeight, int X, int Y, int KenalW ,int KenalH,int LastSum,int &NowSum,int &lastCnt)
{
	int sum = 0;
	int nCnt = 0;


	int radusw = (KenalW-1)/2;
	int radush=(KenalH-1)/2;

    if (LastSum!=0)
    {
		nCnt=lastCnt;
       	for( int i = Y-radush; i <= Y + radush; i++  )
		{
			for( int j = (X-radusw-1); j <= ( X-radusw-1); j++ )
			{

				if( i < 0 || i >= nHeight || j < 0 || j>= nWidth )
					continue; 
				
				sum += pPanel[i*nWidth+j].Y_Value;
				nCnt--;
			}
		}
		sum=LastSum-sum;
		for(int  i1 = Y-radush; i1 <= Y + radush; i1++  )
		{
			for(int   j1 = (X+radusw); j1 <= ( X+radusw); j1++ )
			{
				if( i1 < 0 || i1 >= nHeight || j1 < 0 || j1>= nWidth )
					continue; 
				
				sum += pPanel[i1*nWidth+j1].Y_Value;
				nCnt++;
			}
		}

    }
	else{
		for( int i = Y-radush; i <=Y + radush; i++ )
		{
			for( int j = X-radusw; j <=X+radusw; j++ )
			{
				if( i < 0 || i >= nHeight || j < 0 || j>= nWidth )
					continue; 
				
				sum += pPanel[i*nWidth+j].Y_Value;
				nCnt++;
			}
		}
	}
    NowSum=sum;
	lastCnt=nCnt;
	return (sum*1.0/nCnt);}


void GetDeadPixelFromPannel( WPixInfo* pPanel, int nWidth, int nHeight,WhiteSpotStand* pDeadStd,WPixInfoStack * InfoStack  )
{
	
	int finalWidth=(nWidth/2);
	int finalHeight=(nHeight/2);
    int nTopSL = pDeadStd->nTopSL; //shift several lines
	int nBottomSL = pDeadStd->nBottomSL; //shift several columns
	int nLeftSL  = pDeadStd->nLeftSL;
    int nRightSL = pDeadStd->nRightSL;
	int DeadUnit  = pDeadStd->DeadUnit;
	int DeadSpec=pDeadStd->Deadpc;

	int avg_Y=0;
	int sum_Y=0;
	float temp=0;
	float realSpec=0;
	float spec=0;
	int tempSUM=0;
	int NowSum=0;
	int LastCnt=0;
	for( int i = nTopSL; i < finalHeight-nBottomSL; i++ )
	{
		for( int j = nLeftSL; j < finalWidth-nRightSL; j++)
		{
			
			temp = GetSurroundingAreaAve(pPanel, finalWidth,finalHeight, j,i, pDeadStd->nAreaWidth,pDeadStd->nAreaHeight,tempSUM,NowSum,LastCnt);
			pPanel[i*finalWidth+j].Average=temp;
			tempSUM=NowSum;
			if(DeadUnit)
			{
				realSpec=(pPanel[i*finalWidth+j].Y_Value - temp)/temp*100;
			}
			else
			{
				realSpec=(pPanel[i*finalWidth+j].Y_Value*1.0 - temp);
			}
            spec=DeadSpec;
				
			if (realSpec>spec)
			{
				PushDeadIntoStack(InfoStack->pBrightDeadStk, pPanel[i*finalWidth+j],InfoStack->nDeadCnt, nWidth, nHeight, 10000); 
				InfoStack->nDeadCnt++;        
			}	
		}
		tempSUM=0;
	}  
}
 void GetClusterFromStack(WPixInfo* SingleDeadStack, int StackSize, int&nParticleNum, int nType, int nImageWidth, WDefectStack* DfcStk )
 {
	int Distance = nImageWidth*10; 	 
	nParticleNum = 0;

	if( (1 == nType) || StackSize >= 9999 ) //如果Cluster的类型为1，则直接将SingleDeadStact划入 
	{ 
		//StackSize = min( StackSize, 30 );
		StackSize = ( StackSize<30?StackSize:30 );
		
		for( int i = 0; i < StackSize; i++ )
		{	
            DfcStk->DfcStk[DfcStk->DefectStackCnt++]  = SingleDeadStack[i];	
			nParticleNum++;
		}    

		return; 
	}

	WPixInfo tempStack[100]; 
	int tempStackSize = 0;
	
	for( int i = 0; i < StackSize-1; i++ )
	{
		tempStack[0] = SingleDeadStack[i]; 
		tempStackSize = 1; 
		
		for( int j = i+1; j < StackSize; j++ )
		{  
			//看两个Pixel之间的距离是否足够远，如果足够远，则没有必要进行相邻性确认，直接进行下一个确认。
			if( ( SingleDeadStack[j].yIndex - tempStack[0].yIndex )*nImageWidth + ( SingleDeadStack[j].xIndex - tempStack[0].xIndex ) > Distance )
			{ 
				j = StackSize;
				continue;
			}  
			
			int tempStackSizeCurrent = tempStackSize;
			
			for( int k = 0; k < tempStackSizeCurrent; k++ )
			{ 
				if( (abs( SingleDeadStack[j].yIndex - tempStack[k].yIndex ) <=1 )  && (abs( SingleDeadStack[j].xIndex - tempStack[k].xIndex ) <= 1 ) )
				{ 
					tempStack[tempStackSize] = SingleDeadStack[j];			 
					
					tempStackSize++;  
					
					if( tempStackSize >=  nType )
					{ 					 

						DfcStk->DfcStk[DfcStk->DefectStackCnt++]  = SingleDeadStack[i];	
						nParticleNum++;
						
						if( nParticleNum >= 30 )
						{
							return;
						} 
						else
						{ 
							k = tempStackSizeCurrent; //已经确定该点为一个坏点，进入下一个点的判断。
							j = StackSize;
							continue;
						}
					}
					
					
					//已经确认该Pixel是相邻的，将该Pixel从待确认列表中移除。
					for( int index = j+1; index < StackSize; index++ )
					{
						SingleDeadStack[index-1] = SingleDeadStack[index];												
					} 
					
					StackSize--;				 
					j = j-1; //由于栈中所有数据都往前移了一位，所以计数器也必须往前移一位，否则就少判断一个点了。 
					
					k = tempStackSizeCurrent; //已经确定该点为一个坏点，进入下一个点的判断。
					continue; 
					
				}
			}  
		} 
	}     
 } 

 

int WhiteSpotTestInner( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, WhiteSpotStand* pDeadStd, WDefectStack* DfcStk)
{
    int Result  = 1;   

	WPixInfoStack GrDeadStack;
	WPixInfoStack BDeadStack;
	WPixInfoStack RDeadStack;
	WPixInfoStack GbDeadStack;

	GrDeadStack.pBrightDeadStk =NULL;
	RDeadStack.pBrightDeadStk = NULL;
	BDeadStack.pBrightDeadStk =NULL;
	GbDeadStack.pBrightDeadStk = NULL;


	GrDeadStack.nDeadCnt = 0;
	RDeadStack.nDeadCnt = 0;
	BDeadStack.nDeadCnt = 0;
	GbDeadStack.nDeadCnt = 0;

	//将Raw Image分成4个Channel.
  	//Raw2Channels( pRawImage, nImgWidth, nImgHeight, nPixOrder, pGr, pR, pB, pGb );
	int DeadType=pDeadStd->ClusterType;

	int nParticleNum = 0;

	int nDeadNum = 0;
    int nDarkNum = 0;
	int nWoundNum = 0;

//***************************************Gr*******************//
	WPixInfo *pGr = new WPixInfo[nImgWidth * nImgHeight/4];
	GrDeadStack.pBrightDeadStk = (WPixInfo*) new WPixInfo[10000];
		Raw2Channels( pRawImage, nImgWidth, nImgHeight, nPixOrder, pGr,1);

		GetDeadPixelFromPannel(pGr,nImgWidth,nImgHeight,pDeadStd,&GrDeadStack);

		GetClusterFromStack( GrDeadStack.pBrightDeadStk, GrDeadStack.nDeadCnt, nDeadNum, DeadType, nImgWidth/2, DfcStk);
		
		if( nDeadNum >0) 
		{  
			DfcStk->DeadNum=nDeadNum;
			DfcStk->Podtype=1;
			Result = 0;
		}
		else{
			int temp=0;
			for (int i=0;i<GrDeadStack.nDeadCnt;i++)
			{
				temp=(nImgWidth/2)*GrDeadStack.pBrightDeadStk[i].yIndex+GrDeadStack.pBrightDeadStk[i].xIndex;
                pGr[temp].Y_Value=GrDeadStack.pBrightDeadStk[i].Average;
			}			
		}
		delete[] pGr;
		delete []	GrDeadStack.pBrightDeadStk ;
		///***********************Gb*****************************//

		if(Result!=0)
		{
			WPixInfo *pGb = new WPixInfo[nImgWidth * nImgHeight/4];
			GbDeadStack.pBrightDeadStk = (WPixInfo*) new WPixInfo[10000];
			Raw2Channels( pRawImage, nImgWidth, nImgHeight, nPixOrder, pGb,4);	

            GetDeadPixelFromPannel(pGb,nImgWidth,nImgHeight,pDeadStd,&GbDeadStack);

			GetClusterFromStack( GbDeadStack.pBrightDeadStk, GbDeadStack.nDeadCnt, nDeadNum, DeadType, nImgWidth/2, DfcStk);
			if( nDeadNum >0) 
			{ 
			DfcStk->DeadNum=nDeadNum;
			DfcStk->Podtype=1;
			Result = 0;
			}
			else{
				int temp=0;
				for (int i=0;i<GbDeadStack.nDeadCnt;i++)
				{
					temp=(nImgWidth/2)*GbDeadStack.pBrightDeadStk[i].yIndex+GbDeadStack.pBrightDeadStk[i].xIndex;
					pGb[temp].Y_Value=GbDeadStack.pBrightDeadStk[i].Average;
				}				
			}
			delete [] pGb; 
			delete []	GbDeadStack.pBrightDeadStk ;
		}
		//***********************B***********************//
		if(Result!=0)
		{
			WPixInfo *pB= new WPixInfo[nImgWidth * nImgHeight/4];
			BDeadStack.pBrightDeadStk = (WPixInfo*) new WPixInfo[10000];
			Raw2Channels( pRawImage, nImgWidth, nImgHeight, nPixOrder, pB,3);	

			GetDeadPixelFromPannel(pB,nImgWidth,nImgHeight,pDeadStd,&BDeadStack);

			GetClusterFromStack( BDeadStack.pBrightDeadStk, BDeadStack.nDeadCnt, nDeadNum, DeadType, nImgWidth/2, DfcStk);
			if( nDeadNum >0) 
			{ 
			DfcStk->DeadNum=nDeadNum;
			DfcStk->Podtype=1;
			Result = 0;
			}
			else{
				int temp=0;
				for (int i=0;i<BDeadStack.nDeadCnt;i++)
				{
					temp=(nImgWidth/2)*BDeadStack.pBrightDeadStk[i].yIndex+BDeadStack.pBrightDeadStk[i].xIndex;
					pB[temp].Y_Value=BDeadStack.pBrightDeadStk[i].Average;
				}
				
			}
			delete [] pB; 
			delete []	BDeadStack.pBrightDeadStk ;
		}
		//*************************R**********************//
		if(Result!=0)
		{
			WPixInfo *pR= new WPixInfo[nImgWidth * nImgHeight/4];
			RDeadStack.pBrightDeadStk = (WPixInfo*) new WPixInfo[10000];
			Raw2Channels( pRawImage, nImgWidth, nImgHeight, nPixOrder, pR,3);

			GetDeadPixelFromPannel(pR,nImgWidth,nImgHeight,pDeadStd,&RDeadStack);

			GetClusterFromStack( RDeadStack.pBrightDeadStk, RDeadStack.nDeadCnt, nDeadNum,DeadType, nImgWidth/2, DfcStk);
			if( nDeadNum >0) 
			{ 
			DfcStk->DeadNum=nDeadNum;
			DfcStk->Podtype=1;
			Result = 0;
			}
			else{
				int temp=0;
				for (int i=0;i<RDeadStack.nDeadCnt;i++)
				{
					temp=(nImgWidth/2)*RDeadStack.pBrightDeadStk[i].yIndex+RDeadStack.pBrightDeadStk[i].xIndex;
					pR[temp].Y_Value=RDeadStack.pBrightDeadStk[i].Average;
				}
				
			}
			delete [] pR; 
			delete []	RDeadStack.pBrightDeadStk ;

		}

	return Result;  

}

CString GetModulePath()
{
	HMODULE module = GetModuleHandle(0);
	char modulePath[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, modulePath, MAX_PATH);

	CString ret(modulePath);
	int nPos = ret.ReverseFind('\\');
	ret = (nPos < 0) ? _T("") : ret.Left(nPos);

	return ret;
}

int WhiteSpotTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,WDefectStack* DfcStk)
{
     WhiteSpotStand pPDeadStd;

	 CString temppath = GetModulePath();
	 temppath += _T("\\OfDPaticleTest.ini");

	 pPDeadStd.nTopSL = GetPrivateProfileInt(L"Spec",L"TopSL",0,temppath);
	 pPDeadStd.nBottomSL = GetPrivateProfileInt(L"Spec",L"BottomSL",0,temppath);
	 pPDeadStd.nLeftSL =  GetPrivateProfileInt(L"Spec",L"LeftSL",0,temppath);
	 pPDeadStd.nRightSL = GetPrivateProfileInt(L"Spec",L"RightSL",0,temppath);
	 
	 pPDeadStd.nAreaWidth = GetPrivateProfileInt(L"Spec",L"AreaWidth",0,temppath);
	 pPDeadStd.nAreaHeight = GetPrivateProfileInt(L"Spec",L"AreaHeight",0,temppath);
	 
	 pPDeadStd.DeadUnit =  GetPrivateProfileInt(L"Spec",L"DeadUnit",0,temppath);

	 pPDeadStd.Deadpc =  GetPrivateProfileInt(L"Spec",L"DeadSpec",0,temppath);
	 pPDeadStd.ClusterType=GetPrivateProfileInt(L"Spec",L"DeadType",0,temppath);

	 int result=WhiteSpotTestInner( pRawImage, nImgWidth, nImgHeight 
            ,nPixOrder, &pPDeadStd, DfcStk );

	 return result;
}





