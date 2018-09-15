// OFPOD.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "OfPODTest.h"
#include <math.h>

#include <stdio.h>


#include <stdlib.h>
#include "PODSpecDlg.h"

PODSpecDlg SpecDlg;
/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
*/
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
struct SingleSpc
{
	int Deadpc;
	int WoundSpc_center;
	int WoundSpc_edge;
};

struct SinglePix
{
	SingleSpc WoundPix;
	
};

struct Cluster
{
	int ClusterType; //连续多少个Single为一个Cluster.
};

struct ClusterSpc
{
	/*	Cluster BrightWound;*/
	Cluster Dead;
	/*	Cluster DarkDead;*/
};
struct PODStand
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
	int WoundUnit;
    int Dead_EN;
	int Wound_EN;
	SinglePix RGBChannel;
	ClusterSpc ClusterSp; 	
	
};

CString GetFilePath()
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	char ModulePath[512] = {0};
	char path[512]={0};
	GetModuleFileName(NULL, ModulePath, 512);
	len = strlen(ModulePath);
	for (i = len - 1; i >= 0; i--)
	{
		if (ModulePath[i] == '\\')
		{
			ModulePath[i + 1] = 0;
			flag = true;
			strcpy(path, ModulePath);	
			break;
		}
	}
	
	if (!flag)
	{
		strcpy(path, "");
		
	}
	myStr.Format(path);
	return myStr;
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
int CSV_MakeHeader(char *sfilename)
{
  	 FILE *fp=NULL;
	 int nfileSize=0;
	 int i;
	 /* CString strSavePath=path;*/
	 /*	 CTime t; 	*/		  
	 fp=freopen(sfilename,"at",stdout);
	 // 	 if(fp==NULL) 
	 // 	 {
	 // 		AfxMessageBox(strSavePath+" open fail\n");
	 // 		return -1;
	 // 	 }
	 fseek(fp,0,SEEK_END);
	 nfileSize=ftell(fp);		  
	 if(nfileSize<10) 
	 {
		 printf("Channel,Spec_Center,Spec_Edge");
		 printf("\n");  
	 }
	 fclose(fp);
	 return 1;
}
void Raw2Channels( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, OFPixInfo* pGr, OFPixInfo* pR, OFPixInfo* pB,  OFPixInfo* pGb )
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
void NewRaw2Channels( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, OFPixInfo* pY,int channel)
{

	//channel:0 Gr 1:Gb 2:R 3:B
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
						if (channel==1)//Gb
						{
							pY[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
							// pGb[GbCnt].Average=pRawImage[i*nImgWidth+j];
							pY[GbCnt].xIndex = j/2;
							pY[GbCnt].yIndex = i/2;
						    GbCnt++;
						}
						if (channel==3)//B
						{
							pY[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
							//  pB[BCnt].Average=pRawImage[i*nImgWidth+j+1];	
							pY[BCnt].xIndex = (j+1)/2;
							pY[BCnt].yIndex = i/2;
					 	    BCnt++; 
						}
						
						
					}
					else
					{
						if (channel==2)//R
						{
							pY[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pGb[GbCnt].Average=pRawImage[i*nImgWidth+j];
							pY[RCnt].xIndex = j/2;
							pY[RCnt].yIndex = i/2;
						    RCnt++;
						}
						if (channel==0)//Gr
						{
							pY[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//  pGr[GrCnt].Average=pRawImage[i*nImgWidth+j+1];
							pY[GrCnt].xIndex = (j+1)/2;
							pY[GrCnt].yIndex = i/2;
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
						if (channel==3)
						{
							pY[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pB[BCnt].Average = pRawImage[i*nImgWidth+j];
							pY[BCnt].xIndex = j/2;
							pY[BCnt].yIndex = i/2;
						    BCnt++;
						}
						if (channel==1)
						{
							pY[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pGb[GbCnt].Average = pRawImage[i*nImgWidth+j+1];
							pY[GbCnt].xIndex = (j+1)/2;
							pY[GbCnt].yIndex = i/2;
						    GbCnt++; 
						}
						
						
					}
					else
					{
						if (channel==0)
						{
							pY[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pGr[GrCnt].Average = pRawImage[i*nImgWidth+j];
							pY[GrCnt].xIndex = j/2;
							pY[GrCnt].yIndex = i/2;
						    GrCnt++;
						}
						if (channel==2)
						{
							pY[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pR[RCnt].Average = pRawImage[i*nImgWidth+j+1];
							pY[RCnt].xIndex = (j+1)/2;
							pY[RCnt].yIndex = i/2;
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
						if (channel==2)
						{
							pY[RCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pR[RCnt].Average = pRawImage[i*nImgWidth+j];
							pY[RCnt].xIndex = j/2;
							pY[RCnt].yIndex = i/2;
						    RCnt++;
						}
					    if (channel==0)
					    {
							pY[GrCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pGr[GrCnt].Average = pRawImage[i*nImgWidth+j+1];
							pY[GrCnt].xIndex = (j+1)/2;
							pY[GrCnt].yIndex = i/2;
						    GrCnt++;
					    }
				 
					}
					else
					{
						if (channel==1)
						{
							pY[GbCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pGb[GbCnt].Average = pRawImage[i*nImgWidth+j];
							pY[GbCnt].xIndex = j/2;
							pY[GbCnt].yIndex = i/2;
						    GbCnt++;
						}
						
						if (channel==3)
						{
							pY[BCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pB[BCnt].Average = pRawImage[i*nImgWidth+j+1];
							pY[BCnt].xIndex = (j+1)/2;
							pY[BCnt].yIndex = i/2;
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
						if (channel==0)
						{
							pY[GrCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pGr[GrCnt].Average = pRawImage[i*nImgWidth+j];
							pY[GrCnt].xIndex = j/2;
							pY[GrCnt].yIndex = i/2;
						    GrCnt++;
						}
						if (channel==2)
						{
							pY[RCnt].Y_Value = pRawImage[i*nImgWidth+j+1];	
							//	pR[RCnt].Average = pRawImage[i*nImgWidth+j+1];
							pY[RCnt].xIndex = (j+1)/2;
							pY[RCnt].yIndex = i/2;
						    RCnt++;
						}
						 
					}
					else
					{
						if (channel==3)
						{
							pY[BCnt].Y_Value = pRawImage[i*nImgWidth+j];
							//	pB[BCnt].Average = pRawImage[i*nImgWidth+j];
							pY[BCnt].xIndex = j/2;
							pY[BCnt].yIndex = i/2;
						    BCnt++;
						}
					    if (channel==1)
					    {
							pY[GbCnt].Y_Value = pRawImage[i*nImgWidth+j+1];
							//	pGb[GbCnt].Average = pRawImage[i*nImgWidth+j+1];
							pY[GbCnt].xIndex = (j+1)/2;
							pY[GbCnt].yIndex = i/2;
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

struct OFPixInfoStack
{
	OFPixInfo *pBrightDeadStk;
	OFPixInfo *pDarkDeadStk;
    OFPixInfo *pWoundStk;
	USHORT nDeadCnt;
	USHORT nDarkCnt;
	USHORT nWoundCnt;
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

void GetMeanMinMaxValueFromBlock(OFPixInfo *pIn, int &YAve, int &YMost, int &YMin, UINT nWidth, UINT nHeight, ImagepodPoint *pStartPix, ImagepodSize *pSize)
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

void PushDeadIntoStack(OFPixInfo *Stack, OFPixInfo DeadPix, int nCnt,int nImgWidth, int nImgHeight, int StackMaxSize )
{
	//int nWidth = ; 
	if( nCnt >= StackMaxSize)
		return; 

	int tempIndex = DeadPix.yIndex*nImgWidth + DeadPix.xIndex;
	
	for( int i = nCnt; i > 0; i-- )
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

int getNewPanel(OFPixInfo* pPanel,OFPixInfo* outpPanel,int nImgWidth ,int nImgHeight,int firstWidth,int firstHeight,int ChaCase)
{
	int Cnt=0;
	int newWidth=(nImgWidth/2); 
	int newHeight=(nImgHeight/2);
	int position=0;
	LPBYTE m_pCameraBuffer  = NULL;
	int nsize=((newHeight-firstWidth)+1)*((newWidth-firstWidth)+1);
    m_pCameraBuffer = (LPBYTE)malloc(nsize);

	if(NULL == m_pCameraBuffer) return -1;

    memset(m_pCameraBuffer, 0, nsize);


     ///////////////
	for( int i = 0; i <= (newHeight-firstHeight);  i=i+firstHeight )
	{
		for( int j = 0; j <= (newWidth-firstWidth); j = j + firstWidth )
		{
			position=i*newWidth+j;
			outpPanel[Cnt].xIndex=pPanel[i*newWidth+j].xIndex;
			outpPanel[Cnt].yIndex=pPanel[i*newWidth+j].yIndex;
			outpPanel[Cnt].Y_Value=0;

			for (int h=0;h<firstHeight;h++)
			{
				for (int w=0;w<firstWidth;w++)
				{
                  outpPanel[Cnt].Y_Value=outpPanel[Cnt].Y_Value+pPanel[(i+h)*newWidth+j+w].Y_Value;
				}
			}

			outpPanel[Cnt].Y_Value=(int)(outpPanel[Cnt].Y_Value*1.0/(firstWidth*firstHeight)+0.5);
			m_pCameraBuffer[Cnt]=outpPanel[Cnt].Y_Value;
			Cnt++;
			
		}
	}
	if (ChaCase==1)
	{
		bSaveRawFile("D:\\R.raw",m_pCameraBuffer,nsize);
	}
	else if (ChaCase==2)
	{
		bSaveRawFile("D:\\Gr.raw",m_pCameraBuffer,nsize);
	}
	else if (ChaCase==3)
	{
		bSaveRawFile("D:\\B.raw",m_pCameraBuffer,nsize);
	}
	else
	{
		bSaveRawFile("D:\\Gb.raw",m_pCameraBuffer,nsize);
	}
    if (m_pCameraBuffer!=NULL)
    {
		free(m_pCameraBuffer);
    }
	
	return TRUE;
}

float GetSurroundingAreaAve( OFPixInfo* pPanel, int nWidth, int nHeight, int X, int Y, int KenalW ,int KenalH,int LastSum,int &NowSum,int &lastCnt)
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

	nCnt = (nCnt==0) ? 1:nCnt;
    NowSum=sum;
	lastCnt=nCnt;
	return (sum*1.0/nCnt);
}



void GetWoundPixelFromPannel( OFPixInfo* pPanel, int nWidth, int nHeight,PODStand* pDeadStd,OFPixInfoStack * InfoStack,int ChanCase)
{
	
   int finalWidth=(nWidth/2)/pDeadStd->smallnAreaWidth;
   int finalHeight=(nHeight/2)/pDeadStd->smallnAreaHeight;
   int nLeftSL=pDeadStd->nWoundLeftSL;
   int nTopSL=pDeadStd->nWoundTopSL;
   int nRightSL=pDeadStd->nWoundRightSL;
   int nBottomSL=pDeadStd->nWoundBottomSL;
   int avg_Y=0;
   int sum_Y=0;
   float temp;
   float realSpec=0;
   float tempSpec=0;
   int tempSUM=0;
   int NowSum=0;
   int LastCnt=0;
   int tempH1=nTopSL+pDeadStd->nAreaHeight/2;
   int tempH2=finalHeight-nBottomSL-pDeadStd->nAreaHeight/2; 

   int tempW1=nLeftSL+pDeadStd->nAreaWidth/2;
   int tempW2=finalWidth-nRightSL-pDeadStd->nAreaWidth/2; 


   int Xmin=(nWidth/8);
   int Ymin=(nHeight/8);
   int Xmax=(nWidth*3)/8;
   int Ymax=(nHeight*3)/8;
   float max1=0;
   float max2=0;
   int wound_indexX=0;
   int wound_indexY=0;


   for( int i = nTopSL; i < finalHeight-nBottomSL; i++ )
   {
	   for( int j = nLeftSL; j < finalWidth-nRightSL; j++)
	   {
		   temp = GetSurroundingAreaAve(pPanel, finalWidth,finalHeight, j,i, pDeadStd->nAreaWidth,pDeadStd->nAreaHeight,tempSUM,NowSum,LastCnt);
		  if (pDeadStd->WoundUnit)
		  {
			  realSpec=fabs(pPanel[i*finalWidth+j].Y_Value - temp)/temp*100;
		  }
		  else{
              realSpec=fabs(pPanel[i*finalWidth+j].Y_Value - temp);
		  }

		   
           tempSUM=NowSum;

		   if( (pPanel[i*finalWidth+j].xIndex>Xmin&&pPanel[i*finalWidth+j].xIndex<Xmax)&&(pPanel[i*finalWidth+j].yIndex>Ymin&&pPanel[i*finalWidth+j].yIndex<Ymax))
		   {
			   tempSpec  = pDeadStd->RGBChannel.WoundPix.WoundSpc_center;
			   if (realSpec>max1)
			   {
				   max1=realSpec;
			   }
		   }
		   else
		   {
			   tempSpec  = pDeadStd->RGBChannel.WoundPix.WoundSpc_edge;
			   if (realSpec>max2)
			   {
				   max2=realSpec;
			   }
		   }
		   if (realSpec>tempSpec)
		   {
			   PushDeadIntoStack(InfoStack->pWoundStk, pPanel[i*finalWidth+j],InfoStack->nWoundCnt, nWidth, nHeight, 10000); 
			   InfoStack->nWoundCnt++;
// 			   wound_indexX=pPanel[i*finalWidth+j].xIndex*2;
// 			   wound_indexY=pPanel[i*finalWidth+j].yIndex*2;
               wound_indexX=j;
               wound_indexY=i;
			   

		   }

	   }
	   tempSUM=0;
   } 
   if (pDeadStd->useLog)
   {
	      CSV_MakeHeader("D:\\POD_TEST.csv");
		  FILE *fp=NULL;
		  fp=freopen("D:\\POD_TEST.csv","at+",stdout);

		  if(NULL == fp) return;
          fseek(fp,0,SEEK_END);
		  if (ChanCase==1)
		  {
             printf("Gr,%.2f,%.2f\n",max1,max2);
		  }
		  else if (ChanCase==2)
		  {
			  printf("R,%.2f,%.2f\n",max1,max2);
		  }
		  else if (ChanCase==3)
		  {
			  printf("Gb,%.2f,%.2f\n",max1,max2);
		  }
		  else if (ChanCase==4)
		  {
			  printf("B,%.2f,%.2f\n",max1,max2);
		  }
		  if (InfoStack->nWoundCnt>0)
		  {
             printf("  ,wound_indexX=%d,wound_indexY=%d\n",wound_indexX,wound_indexY);
		  }
          fclose(fp);

   }

}


void GetDeadPixelFromPannel( OFPixInfo* pPanel, int nWidth, int nHeight,PODStand* pDeadStd,OFPixInfoStack * InfoStack  )
{
	
	int finalWidth=(nWidth/2);
	int finalHeight=(nHeight/2);
    int nTopSL = pDeadStd->nTopSL; //shift several lines
	int nBottomSL = pDeadStd->nBottomSL; //shift several columns
	int nLeftSL  = pDeadStd->nLeftSL;
    int nRightSL = pDeadStd->nRightSL;
	int DeadUnit  = pDeadStd->DeadUnit;
	int DeadSpec=pDeadStd->RGBChannel.WoundPix.Deadpc;

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
			if (realSpec<(0-spec))
			{
				PushDeadIntoStack(InfoStack->pDarkDeadStk, pPanel[i*finalWidth+j],InfoStack->nDarkCnt, nWidth, nHeight, 10000); 
				InfoStack->nDarkCnt++;        
			}
			// tempSUM=0;
			
		}
		tempSUM=0;
	}  
}
 void GetClusterFromStack(OFPixInfo* SingleDeadStack, int StackSize, int&nParticleNum, int nType, int nImageWidth, DefectStack* DfcStk )
 {
	int Distance = nImageWidth*10; 	 
	nParticleNum = 0;

	if( (1 == nType) || StackSize >= 9999 ) //如果Cluster的类型为1，则直接将SingleDeadStact划入 
	{ 
		StackSize = min( StackSize, 30 );
		
		for( int i = 0; i < StackSize; i++ )
		{	
            DfcStk->DfcStk[DfcStk->DefectStackCnt++]  = SingleDeadStack[i];	
			nParticleNum++;
		}    

		return; 
	}

	OFPixInfo tempStack[100]; 
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

 

int OFPODTestInner( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, PODStand* pDeadStd, DefectStack* DfcStk)
{
    int Result  = 1; 
	/////先测试Gr////////   
	OFPixInfo *pGr = new OFPixInfo[nImgWidth * nImgHeight/4];
	if (pGr==NULL)
	{
		Result=0;
		return Result;
	}
    OFPixInfo *newpGr = new OFPixInfo[nImgWidth * nImgHeight/4];
	if (newpGr==NULL)
	{
		delete []pGr;
		Result=0;
		return Result;
	}
    OFPixInfoStack GrDeadStack;
	GrDeadStack.pBrightDeadStk = (OFPixInfo*) new OFPixInfo[10000];
	if (GrDeadStack.pBrightDeadStk==NULL)
	{
		delete []pGr;
		delete []newpGr;
		Result=0;
		return Result;
	}
	GrDeadStack.pDarkDeadStk = (OFPixInfo*) new OFPixInfo[10000];
	if (GrDeadStack.pDarkDeadStk==NULL)
	{
		delete []pGr;
		delete []newpGr;
		delete []GrDeadStack.pBrightDeadStk;
		Result=0;
		return Result;
	}
	GrDeadStack.pWoundStk = (OFPixInfo*) new OFPixInfo[10000];
	if (GrDeadStack.pWoundStk==NULL)
	{
		delete []pGr;
		delete []newpGr;
		delete []GrDeadStack.pBrightDeadStk;
        delete []GrDeadStack.pDarkDeadStk;
		Result=0;
		return Result;
	}
    
	int DeadType=pDeadStd->ClusterSp.Dead.ClusterType;
	
	int nParticleNum = 0;
	
	int nDeadNum = 0;
    int nDarkNum = 0;
	int nWoundNum = 0;
	GrDeadStack.nDeadCnt = 0;
	GrDeadStack.nDarkCnt = 0;
	GrDeadStack.nWoundCnt = 0;
    NewRaw2Channels(pRawImage, nImgWidth, nImgHeight, nPixOrder, pGr,0);//Gr
	if (pDeadStd->Dead_EN)//Gr Dead
    {
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
		
		if (Result!=0)
		{
			GetClusterFromStack( GrDeadStack.pDarkDeadStk, GrDeadStack.nDarkCnt, nDarkNum, DeadType, nImgWidth/2, DfcStk);
			if( nDarkNum >0) 
			{  
				DfcStk->DeadNum=nDarkNum;
				DfcStk->Podtype=2;
				Result = 0;
			}
			else{
				int temp=0;
				for (int i=0;i<GrDeadStack.nDarkCnt;i++)
				{
					temp=(nImgWidth/2)*GrDeadStack.pDarkDeadStk[i].yIndex+GrDeadStack.pDarkDeadStk[i].xIndex;
                    pGr[temp].Y_Value=GrDeadStack.pDarkDeadStk[i].Average;
				}
				
			}
		}
	}
	if (pDeadStd->Wound_EN&&Result!=0)//Gr Wound
    {
		if (getNewPanel(pGr,newpGr,nImgWidth,nImgHeight,pDeadStd->smallnAreaWidth,pDeadStd->smallnAreaHeight,2) == -1)
		{
			delete []	GrDeadStack.pBrightDeadStk ;	
			delete []	GrDeadStack.pDarkDeadStk ;	
			delete []	GrDeadStack.pWoundStk ;	
			delete [] pGr;
         	delete [] newpGr;
        	return 0;
		}
		
		GetWoundPixelFromPannel(newpGr,nImgWidth,nImgHeight,pDeadStd,&GrDeadStack,1);
		if( GrDeadStack.nWoundCnt >0) 
		{  
			GetClusterFromStack( GrDeadStack.pWoundStk, GrDeadStack.nWoundCnt, nWoundNum, 1, nImgWidth/2, DfcStk);
			DfcStk->WoundNum=nWoundNum;
			Result = 0;
		}
	}
	delete []	GrDeadStack.pBrightDeadStk ;	
	delete []	GrDeadStack.pDarkDeadStk ;	
	delete []	GrDeadStack.pWoundStk ;	
	delete [] pGr;
	delete [] newpGr;
	////////////////////////////////////////////////////////
	/////////测试Gb////////////
	OFPixInfo *pGb = new OFPixInfo[nImgWidth * nImgHeight/4]; 
	if (pGb==NULL)
	{
		Result=0;
		return Result;
	}
    OFPixInfo *newpGb = new OFPixInfo[nImgWidth * nImgHeight/4];
	if (newpGb==NULL)
	{
		delete []pGb;
		Result=0;
		return Result;
	}
    OFPixInfoStack GbDeadStack;
	GbDeadStack.pBrightDeadStk = (OFPixInfo*) new OFPixInfo[10000];
	if (GbDeadStack.pBrightDeadStk==NULL)
	{
		delete []pGb;
		delete []newpGb;
		Result=0;
		return Result;
	}
	GbDeadStack.pDarkDeadStk = (OFPixInfo*) new OFPixInfo[10000];
	if (GbDeadStack.pDarkDeadStk==NULL)
	{
		delete []pGb;
		delete []newpGb;
		delete []GbDeadStack.pBrightDeadStk;
		Result=0;
		return Result;
	}
	GbDeadStack.pWoundStk = (OFPixInfo*) new OFPixInfo[10000];
	if (GbDeadStack.pWoundStk==NULL)
	{
		delete []pGb;
		delete []newpGb;
		delete []GbDeadStack.pBrightDeadStk;
        delete []GbDeadStack.pDarkDeadStk;
		Result=0;
		return Result;
	}
	
	GbDeadStack.nDeadCnt = 0;
	GbDeadStack.nDarkCnt = 0;
	GbDeadStack.nWoundCnt = 0;
    NewRaw2Channels(pRawImage, nImgWidth, nImgHeight, nPixOrder, pGb,1);//Gb
    if (pDeadStd->Dead_EN&&Result!=0)
    {

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
		if (Result!=0)
		{
			GetClusterFromStack( GbDeadStack.pDarkDeadStk, GbDeadStack.nDarkCnt, nDarkNum, DeadType, nImgWidth/2, DfcStk);
			if( nDarkNum >0) 
			{  
				DfcStk->DeadNum=nDarkNum;
				DfcStk->Podtype=2;
				Result = 0;
			}
			else{
				int temp=0;
				for (int i=0;i<GbDeadStack.nDarkCnt;i++)
				{
					temp=(nImgWidth/2)*GbDeadStack.pDarkDeadStk[i].yIndex+GbDeadStack.pDarkDeadStk[i].xIndex;
					pGb[temp].Y_Value=GbDeadStack.pDarkDeadStk[i].Average;
				}
				
			}
		}
		
	}
	if (pDeadStd->Wound_EN&&Result!=0)//Gb Wound
    {
		if (getNewPanel(pGb,newpGb,nImgWidth,nImgHeight,pDeadStd->smallnAreaWidth,pDeadStd->smallnAreaHeight,4) == -1)
		{
			delete []	GbDeadStack.pBrightDeadStk ;	
			delete []	GbDeadStack.pDarkDeadStk ;	
			delete []	GbDeadStack.pWoundStk ;	
			delete [] pGb;
        	delete [] newpGb;
			return 0;
		}
		
		if(Result!=0)
		{
            GetWoundPixelFromPannel(newpGb,nImgWidth,nImgHeight,pDeadStd,&GbDeadStack,3);
			if( GbDeadStack.nWoundCnt>0) 
			{  
				GetClusterFromStack( GbDeadStack.pWoundStk, GbDeadStack.nWoundCnt, nWoundNum, 1, nImgWidth/2, DfcStk);
				DfcStk->WoundNum=nWoundNum;
				Result = 0;
			}
		}
	}
	delete []	GbDeadStack.pBrightDeadStk ;	
	delete []	GbDeadStack.pDarkDeadStk ;	
	delete []	GbDeadStack.pWoundStk ;	
	delete [] pGb;
	delete [] newpGb;
	////////////////////////////////////////////////////////
	/////////测试R////////////
    OFPixInfo *pR = new OFPixInfo[nImgWidth * nImgHeight/4]; 
	if (pR==NULL)
	{
		Result=0;
		return Result;
	}
    OFPixInfo *newpR = new OFPixInfo[nImgWidth * nImgHeight/4];
	if (newpR==NULL)
	{
		delete []pR;
		Result=0;
		return Result;
	}
    OFPixInfoStack RDeadStack;
	RDeadStack.pBrightDeadStk = (OFPixInfo*) new OFPixInfo[10000];
	if (RDeadStack.pBrightDeadStk==NULL)
	{
		delete []pR;
		delete []newpR;
		Result=0;
		return Result;
	}
	RDeadStack.pDarkDeadStk = (OFPixInfo*) new OFPixInfo[10000];
	if (RDeadStack.pDarkDeadStk==NULL)
	{
		delete []pR;
		delete []newpR;
		delete []RDeadStack.pBrightDeadStk;
		Result=0;
		return Result;
	}
	RDeadStack.pWoundStk = (OFPixInfo*) new OFPixInfo[10000];
	if (RDeadStack.pWoundStk==NULL)
	{
		delete []pR;
		delete []newpR;
		delete []RDeadStack.pBrightDeadStk;
        delete []RDeadStack.pDarkDeadStk;
		Result=0;
		return Result;
	}
	
	RDeadStack.nDeadCnt = 0;
	RDeadStack.nDarkCnt = 0;
	RDeadStack.nWoundCnt = 0;
    NewRaw2Channels(pRawImage, nImgWidth, nImgHeight, nPixOrder, pR,2);//R
    if (pDeadStd->Dead_EN&&Result!=0)//R Dead
    {
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
		if (Result!=0)
		{
			GetClusterFromStack( RDeadStack.pDarkDeadStk, RDeadStack.nDarkCnt, nDarkNum, DeadType, nImgWidth/2, DfcStk);
			if( nDarkNum >0) 
			{  
				DfcStk->DeadNum=nDarkNum;
				Result = 0;
				DfcStk->Podtype=2;
			}
			else{
				int temp=0;
				for (int i=0;i<RDeadStack.nDarkCnt;i++)
				{
					temp=(nImgWidth/2)*RDeadStack.pDarkDeadStk[i].yIndex+RDeadStack.pDarkDeadStk[i].xIndex;
					pR[temp].Y_Value=RDeadStack.pDarkDeadStk[i].Average;
				}
				
			}
		}
	}
	if (pDeadStd->Wound_EN&&Result!=0)//R Wound
    {
		if (getNewPanel(pR,newpR,nImgWidth,nImgHeight,pDeadStd->smallnAreaWidth,pDeadStd->smallnAreaHeight,1) == -1)
		{
			delete []	RDeadStack.pBrightDeadStk ;	
			delete []	RDeadStack.pDarkDeadStk ;	
			delete []	RDeadStack.pWoundStk ;	
			delete [] pR;
	        delete [] newpR;
			return 0;
		}
		
		GetWoundPixelFromPannel(newpR,nImgWidth,nImgHeight,pDeadStd,&RDeadStack,2);
		if(RDeadStack.nWoundCnt>0) 
		{  
			GetClusterFromStack( RDeadStack.pWoundStk, RDeadStack.nWoundCnt, nWoundNum, 1, nImgWidth/2, DfcStk);
			DfcStk->WoundNum=nWoundNum;
			Result = 0;
		}
	}
    delete []	RDeadStack.pBrightDeadStk ;	
	delete []	RDeadStack.pDarkDeadStk ;	
	delete []	RDeadStack.pWoundStk ;	
	delete [] pR;
	delete [] newpR;
	////////////////////////////////////////////////////////
	/////////测试B////////////
    OFPixInfo *pB = new OFPixInfo[nImgWidth * nImgHeight/4]; 
	if (pB==NULL)
	{
		Result=0;
		return Result;
	}
    OFPixInfo *newpB = new OFPixInfo[nImgWidth * nImgHeight/4];
	if (newpB==NULL)
	{
		delete []pB;
		Result=0;
		return Result;
	}
    OFPixInfoStack BDeadStack;
	BDeadStack.pBrightDeadStk = (OFPixInfo*) new OFPixInfo[10000];
	if (BDeadStack.pBrightDeadStk==NULL)
	{
		delete []pB;
		delete []newpB;
		Result=0;
		return Result;
	}
	BDeadStack.pDarkDeadStk = (OFPixInfo*) new OFPixInfo[10000];
	if (BDeadStack.pDarkDeadStk==NULL)
	{
		delete []pB;
		delete []newpB;
		delete []BDeadStack.pBrightDeadStk;
		Result=0;
		return Result;
	}
	BDeadStack.pWoundStk = (OFPixInfo*) new OFPixInfo[10000];
	if (BDeadStack.pWoundStk==NULL)
	{
		delete []pB;
		delete []newpB;
		delete []BDeadStack.pBrightDeadStk;
        delete []BDeadStack.pDarkDeadStk;
		Result=0;
		return Result;
	}
	
	BDeadStack.nDeadCnt = 0;
	BDeadStack.nDarkCnt = 0;
	BDeadStack.nWoundCnt = 0;
    NewRaw2Channels(pRawImage, nImgWidth, nImgHeight, nPixOrder, pB,3);//B
    if (pDeadStd->Dead_EN&&Result!=0)//B Dead
    {
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
		if (Result!=0)
		{
			GetClusterFromStack( BDeadStack.pDarkDeadStk, BDeadStack.nDarkCnt, nDarkNum, DeadType, nImgWidth/2, DfcStk);
			if( nDarkNum >0) 
			{  
				DfcStk->DeadNum=nDarkNum;
				DfcStk->Podtype=2;
				Result = 0;
			}
			else{
				int temp=0;
				for (int i=0;i<BDeadStack.nDarkCnt;i++)
				{
					temp=(nImgWidth/2)*BDeadStack.pDarkDeadStk[i].yIndex+BDeadStack.pDarkDeadStk[i].xIndex;
					pB[temp].Y_Value=BDeadStack.pDarkDeadStk[i].Average;
				}
				
			}
		}
	}
	if (pDeadStd->Wound_EN&&Result!=0)//B Wound
    {
		if (getNewPanel(pB,newpB,nImgWidth,nImgHeight,pDeadStd->smallnAreaWidth,pDeadStd->smallnAreaHeight,3) == -1)
		{
			delete []	BDeadStack.pBrightDeadStk ;	
			delete []	BDeadStack.pDarkDeadStk ;	
			delete []	BDeadStack.pWoundStk ;	
			delete [] pB;
	        delete [] newpB;
			return 0;
		}
		
       	GetWoundPixelFromPannel(newpB,nImgWidth,nImgHeight,pDeadStd,&BDeadStack,4);
		if(BDeadStack.nWoundCnt>0) 
		{  
			GetClusterFromStack( BDeadStack.pWoundStk, BDeadStack.nWoundCnt, nWoundNum, 1, nImgWidth/2, DfcStk);
			DfcStk->WoundNum=nWoundNum;
			Result = 0;
		}
	}
	delete []	BDeadStack.pBrightDeadStk ;	
	delete []	BDeadStack.pDarkDeadStk ;	
	delete []	BDeadStack.pWoundStk ;	
	delete [] pB;
	delete [] newpB;
	return Result;  

}
int OFPODTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder,DefectStack* DfcStk,CamSelectForPOD Cam)
{
	if (NULL == pRawImage || NULL == DfcStk || NULL == DfcStk->DfcStk)
	{
		return FALSE;
	}
	CString m_szSetup;
	 m_szSetup=GetFilePath()+"OfPODTest.ini";
     PODStand pPDeadStd;

	 if (Cam == POD_Cam0)
	 {
		 pPDeadStd.nTopSL = GetPrivateProfileInt("Spec_Cam0","TopSL",20,m_szSetup);
		 pPDeadStd.nBottomSL = GetPrivateProfileInt("Spec_Cam0","BottomSL",20, m_szSetup);
		 pPDeadStd.nLeftSL = GetPrivateProfileInt("Spec_Cam0","LeftSL",20, m_szSetup);
		 pPDeadStd.nRightSL = GetPrivateProfileInt("Spec_Cam0","RightSL",20, m_szSetup);
		 
		 pPDeadStd.nWoundTopSL = GetPrivateProfileInt("Spec_Cam0","WTopSL",20, m_szSetup);
		 pPDeadStd.nWoundBottomSL = GetPrivateProfileInt("Spec_Cam0","WBottomSL",20,m_szSetup);
		 pPDeadStd.nWoundLeftSL = GetPrivateProfileInt("Spec_Cam0","WLeftSL",20, m_szSetup);
		 pPDeadStd.nWoundRightSL = GetPrivateProfileInt("Spec_Cam0","WRightSL",20, m_szSetup);
		 
		 pPDeadStd.nAreaWidth = GetPrivateProfileInt("Spec_Cam0","AreaWidth",20, m_szSetup);
		 pPDeadStd.nAreaHeight =GetPrivateProfileInt("Spec_Cam0","AreaHeight",20, m_szSetup);
		 
		 pPDeadStd.smallnAreaWidth = GetPrivateProfileInt("Spec_Cam0","smallAreaWidth",20, m_szSetup);
		 pPDeadStd.smallnAreaHeight =GetPrivateProfileInt("Spec_Cam0","smallAreaHeight",20, m_szSetup);
		 
		 pPDeadStd.DeadUnit = GetPrivateProfileInt("Spec_Cam0","DeadUnit",1, m_szSetup);
		 pPDeadStd.WoundUnit = GetPrivateProfileInt("Spec_Cam0","WoundUnit",1, m_szSetup);
		 pPDeadStd.Dead_EN =GetPrivateProfileInt("Spec_Cam0","Dead_Enable",1, m_szSetup);
		 pPDeadStd.Wound_EN =GetPrivateProfileInt("Spec_Cam0","Wound_Enable",1, m_szSetup);
		 
		 pPDeadStd.RGBChannel.WoundPix.Deadpc = GetPrivateProfileInt("Spec_Cam0","DeadSpc",20, m_szSetup);
		 pPDeadStd.RGBChannel.WoundPix.WoundSpc_center =GetPrivateProfileInt("Spec_Cam0","WoundSpc_center",20, m_szSetup);
		 pPDeadStd.RGBChannel.WoundPix.WoundSpc_edge=GetPrivateProfileInt("Spec_Cam0","WoundSpc_edge",20, m_szSetup);
		 pPDeadStd.useLog=GetPrivateProfileInt("Spec_Cam0","UseLog",1, m_szSetup);
		 
	     pPDeadStd.ClusterSp.Dead.ClusterType=GetPrivateProfileInt("Spec_Cam0","ClusterType",20, m_szSetup);
	 }

	 if (Cam == POD_Cam1)
	 {
		 pPDeadStd.nTopSL = GetPrivateProfileInt("Spec_Cam1","TopSL",20,m_szSetup);
		 pPDeadStd.nBottomSL = GetPrivateProfileInt("Spec_Cam1","BottomSL",20, m_szSetup);
		 pPDeadStd.nLeftSL = GetPrivateProfileInt("Spec_Cam1","LeftSL",20, m_szSetup);
		 pPDeadStd.nRightSL = GetPrivateProfileInt("Spec_Cam1","RightSL",20, m_szSetup);
		 
		 pPDeadStd.nWoundTopSL = GetPrivateProfileInt("Spec_Cam1","WTopSL",20, m_szSetup);
		 pPDeadStd.nWoundBottomSL = GetPrivateProfileInt("Spec_Cam1","WBottomSL",20,m_szSetup);
		 pPDeadStd.nWoundLeftSL = GetPrivateProfileInt("Spec_Cam1","WLeftSL",20, m_szSetup);
		 pPDeadStd.nWoundRightSL = GetPrivateProfileInt("Spec_Cam1","WRightSL",20, m_szSetup);
		 
		 pPDeadStd.nAreaWidth = GetPrivateProfileInt("Spec_Cam1","AreaWidth",20, m_szSetup);
		 pPDeadStd.nAreaHeight =GetPrivateProfileInt("Spec_Cam1","AreaHeight",20, m_szSetup);
		 
		 pPDeadStd.smallnAreaWidth = GetPrivateProfileInt("Spec_Cam1","smallAreaWidth",20, m_szSetup);
		 pPDeadStd.smallnAreaHeight =GetPrivateProfileInt("Spec_Cam1","smallAreaHeight",20, m_szSetup);
		 
		 pPDeadStd.DeadUnit = GetPrivateProfileInt("Spec_Cam1","DeadUnit",1, m_szSetup);
		 pPDeadStd.WoundUnit = GetPrivateProfileInt("Spec_Cam1","WoundUnit",1, m_szSetup);
		 pPDeadStd.Dead_EN =GetPrivateProfileInt("Spec_Cam1","Dead_Enable",1, m_szSetup);
		 pPDeadStd.Wound_EN =GetPrivateProfileInt("Spec_Cam1","Wound_Enable",1, m_szSetup);
		 
		 pPDeadStd.RGBChannel.WoundPix.Deadpc = GetPrivateProfileInt("Spec_Cam1","DeadSpc",20, m_szSetup);
		 pPDeadStd.RGBChannel.WoundPix.WoundSpc_center =GetPrivateProfileInt("Spec_Cam1","WoundSpc_center",20, m_szSetup);
		 pPDeadStd.RGBChannel.WoundPix.WoundSpc_edge=GetPrivateProfileInt("Spec_Cam1","WoundSpc_edge",20, m_szSetup);
		 pPDeadStd.useLog=GetPrivateProfileInt("Spec_Cam1","UseLog",1, m_szSetup);
		 
		 pPDeadStd.ClusterSp.Dead.ClusterType=GetPrivateProfileInt("Spec_Cam1","ClusterType",20, m_szSetup);
	 }
	 
	 if (pPDeadStd.useLog)
	 {
		 bSaveRawFile("D:\\pod_test.raw", pRawImage,nImgWidth*nImgHeight);
	 }
	 int result=OFPODTestInner( pRawImage, nImgWidth, nImgHeight,nPixOrder, &pPDeadStd, DfcStk );

	 return result;
}

void ShowPODSpecDlg(HWND hwnd,BOOL ShowFlag)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CWnd *pwnd=CWnd::FromHandle (hwnd);
	
	if (ShowFlag==FALSE)
	{
		if(SpecDlg.m_hWnd)
		{
			SpecDlg.ShowWindow(SW_HIDE);
		}
		return;
	}
	else
	{
        if(SpecDlg.m_hWnd)
		{
			SpecDlg.ShowWindow(SW_SHOW);
			return;
		}
	}
	if (hwnd==0)
	{
		SpecDlg.Create(IDD_PODSPECDLG_DIALOG,CWnd::FromHandle(GetDesktopWindow()));
		SpecDlg.ModifyStyle(WS_CHILD,DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU);
		SpecDlg.SetWindowText("POD_Spec_Setting");
		SpecDlg.CenterWindow(CWnd::FromHandle(GetDesktopWindow() ) );
	}	
	else
	{
		SpecDlg.Create(IDD_PODSPECDLG_DIALOG,pwnd);
		RECT rt;
		pwnd->GetClientRect(&rt);
		SpecDlg.MoveWindow(&rt);
		
	}
	
	SpecDlg.ShowWindow(SW_SHOW); 
	
	return;
}



