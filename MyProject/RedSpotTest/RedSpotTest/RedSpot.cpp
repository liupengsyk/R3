// RedSpot.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#define  RedSpot_FILE  1
#include "RedSpot.h"


#define B_OFFSET    0
#define G_OFFSET    1
#define R_OFFSET    2
typedef struct _StrYrgb
{
	double Y;
	double R;
	double G;
	double B;
}StrYrgb, *pStrYrgbr;

//这里做了一个转化，避免碰底
void nCheckBlockAreaSetting(LONG& x0, 
							LONG& y0, 
							UINT  width, 
							UINT  height, 
							UINT  ImageWidth, 
							UINT  ImageHeight)
{
    x0 = x0 - width  / 2;  // x0 is center x
	y0 = y0 - height / 2;  // y0 is center y,here 调整
	
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
double nGetYrgbAverage(BYTE *pIn,STRYrgb *pOut,UINT nWidth,UINT nHeight,CPoint *pStratPix,CSize *pSize)
{
	int  i = 0;
	int  j = 0;
	double  Y_Avr = 0.0;
	double  R_Avr  = 0.0;
	double  G_Avr = 0.0;
	double  B_Avr = 0.0;
	
	CPoint  *StartPosition = new CPoint; 
	
	double PixY   = 0;
	double PixR  = 0;
	double PixG  = 0;
	double PixB = 0; 
	
	double Y_Sum  = 0;
	double R_Sum = 0;
	double G_Sum = 0;
	double B_Sum = 0;
	
	int  nAreaWidth  = pSize->cx;
	int  nAreaHeight = pSize->cy;
	
	StartPosition->x = pStratPix->x ;
	StartPosition->y = pStratPix->y ;
	
	
	if ( (nAreaHeight == 0) || (nAreaWidth < 2) || (nWidth == 0) || (nHeight == 0))
	{
		if (StartPosition != NULL)
		{
			delete StartPosition;
			StartPosition = NULL;
		}
		
		return 0;  // error code		 
	}
	
#if 0	
	nCheckBlockAreaSetting(StartPosition->x,
						  StartPosition->y,
						  nAreaWidth,
						  nAreaHeight,
						  nWidth,
						  nHeight);
#endif	
	if (StartPosition->x % 2)
	{
		StartPosition->x++;//确保是偶数
		nAreaWidth--;
	}
	
	if (nAreaWidth % 2)
	{
		nAreaWidth--;
	}
	
	// Get Area and get Y average.
	for (i = 0; i <nAreaHeight; i++)
	{
		for (j =0; j <nAreaWidth; j++)
		{
			//在bayer后RGB反序，所以图像用之前最好能上下倒一下
			PixB = *(pIn+(((i+StartPosition->y))*nWidth+(StartPosition->x+j))*3+B_OFFSET);
			PixR = *(pIn+(((i+StartPosition->y))*nWidth+(StartPosition->x+j))*3+R_OFFSET);
			PixG = *(pIn+(((i+StartPosition->y))*nWidth+(StartPosition->x+j))*3+G_OFFSET); 
			
			PixY   = double ((9798)*float(PixR) + (19235)*float(PixG) + (3735)*float(PixB))/32768 ;
			Y_Sum += PixY;   
			R_Sum += PixR;
			G_Sum += PixG;
			B_Sum += PixB;	
		}
	}
	
	pOut->Y = (int)( Y_Sum / (nAreaHeight * nAreaWidth));  // get the Y average..
	pOut->R = (int)( R_Sum / (nAreaHeight * nAreaWidth)); 
	pOut->G = (int)( G_Sum / (nAreaHeight * nAreaWidth));
	pOut->B = (int)( B_Sum / (nAreaHeight * nAreaWidth));
	
	if (StartPosition != NULL)
	{
		
		delete StartPosition;
		StartPosition = NULL;
	}
	
	return 1; 
}
BOOL nGetYCbCrAverage(BYTE *pIn, STRYCbCr* pOut, UINT nWidth, UINT nHeight, CPoint *pStartPix, CSize *pSize,int OutputFormat)
{
	int  i = 0;
	int  j = 0;
	int  Y_Avr = 0;
	int  YOff  = 0;
	int  COff  = 0;
	int  CbOff = 0;
	
	int  nAreaWidth  = pSize->cx;
	int  nAreaHeight = pSize->cy;
	
	if ( (nAreaHeight == 0) || (nAreaWidth < 2) || (nWidth == 0) || (nHeight == 0))
	{
		return FALSE;  // error code 
	}
#if 0
	nCheckBlockAreaSetting(pStartPix->x,
						  pStartPix->y,
						  nAreaWidth,
						  nAreaHeight,
						  nWidth,
						  nHeight);
#endif	
	if (pStartPix->x % 2)
	{
		pStartPix->x++;
		nAreaWidth--;
	}
	
	if (nAreaWidth % 2)
	{
		nAreaWidth--;
	}
	
	BYTE PixY   = 0;
	BYTE PixCb  = 0;
	BYTE PixCr  = 0;
	
	LONG Y_Sum  = 0;
	LONG Cb_Sum = 0;
	LONG Cr_Sum = 0;
	
	YOff  = OutputFormat < 2 ? 0 : 1;
	COff  = OutputFormat < 2 ? 1 : 0;
	CbOff = OutputFormat % 2;
	
	// Get Area and get Y average.
	for (i = 0; i < nAreaHeight; i++)
	{
		for (j = 0; j < nAreaWidth; j++)
		{
			// Y
			PixY   = *(pIn + ((i + pStartPix->y) * nWidth + pStartPix->x + j) * 2 + YOff);  // get the Y value YCbCr 
			Y_Sum += PixY;   // caculate the summarize of the all Y value..
			
			// Cb
			if (j % 2 == CbOff)
			{
				PixCb   = *(pIn + ((i + pStartPix->y) * nWidth + pStartPix->x + j) * 2 + COff); // Cb
				Cb_Sum += PixCb;
			}
			
			// Cr
			else
			{
				PixCr   = *(pIn + ((i + pStartPix->y) * nWidth + pStartPix->x + j) * 2 + COff); //Cr
				Cr_Sum += PixCr;
			}
		}
	}
	
	pOut->Y  = (int)(    Y_Sum  / (nAreaHeight * nAreaWidth));  // get the Y average..
	pOut->Cb = (int)(2 * Cb_Sum / (nAreaHeight * nAreaWidth));  // get the Cb average..
	pOut->Cr = (int)(2 * Cr_Sum / (nAreaHeight * nAreaWidth));  // get the Cr average..
	
	return TRUE;
}
RedSpotDLLAPI int _stdcall  RedSpotTest(LPBYTE pTestBmpBuffer,int ImgWidth,int ImgHeight ,int RatioX,int RatioY,RedSpotY *pRedSpotYrgb)
{
	BYTE* pIn    = pTestBmpBuffer;
	UINT  width  = ImgWidth;
	UINT  height = ImgHeight;

    int AreaWidth=ImgWidth/RatioX;
	int AreaHeight=ImgHeight/RatioY;

	int BlockNum=RatioX*RatioY;
    STRYrgb *tempBlock=new STRYrgb[BlockNum];
	CPoint *Block=new CPoint[BlockNum];
	CSize s0;
	s0.cx=AreaWidth;
	s0.cy=AreaHeight;
    memset(tempBlock, 0, BlockNum*sizeof(STRYrgb));
	memset(pRedSpotYrgb, 0, sizeof(RedSpotY));
	for (int j=0;j<RatioY;j++)
	{
		for (int i=0;i<RatioX;i++)
		{
			Block[i+RatioX*j].x=i*AreaWidth;
			Block[i+RatioX*j].y=j*AreaHeight;
			nGetYrgbAverage(pIn, &tempBlock[i+RatioX*j], width, height, &Block[i+RatioX*j], &s0);
			if (tempBlock[i+RatioX*j].Y>pRedSpotYrgb->OutputYrgb.Y)
			{
				pRedSpotYrgb->OutputYrgb.Y=tempBlock[i+RatioX*j].Y;
				pRedSpotYrgb->YrgbPoint[0].x=i;
				pRedSpotYrgb->YrgbPoint[0].y=j;
			}
			if (tempBlock[i+RatioX*j].R>pRedSpotYrgb->OutputYrgb.R)
			{
				pRedSpotYrgb->OutputYrgb.R=tempBlock[i+RatioX*j].R;
				pRedSpotYrgb->YrgbPoint[1].x=i;
				pRedSpotYrgb->YrgbPoint[1].y=j;
			}
			if (tempBlock[i+RatioX*j].G>pRedSpotYrgb->OutputYrgb.G)
			{
				pRedSpotYrgb->OutputYrgb.G=tempBlock[i+RatioX*j].G;
				pRedSpotYrgb->YrgbPoint[2].x=i;
				pRedSpotYrgb->YrgbPoint[2].y=j;
			}
			if (tempBlock[i+RatioX*j].B>pRedSpotYrgb->OutputYrgb.B)
			{
				pRedSpotYrgb->OutputYrgb.B=tempBlock[i+RatioX*j].B;
				pRedSpotYrgb->YrgbPoint[3].x=i;
				pRedSpotYrgb->YrgbPoint[3].y=j;
			}
		}
	
	}
   delete[] tempBlock;
   delete[] Block; 
   return TRUE;
}
RedSpotDLLAPI int _stdcall  RedSpotTestYUV(LPBYTE pIn,int ImgWidth,int ImgHeight ,int RatioX,int RatioY,int OutFormat,RedSpotY *pRedSpotYCbCr)
{
	UINT  width  = ImgWidth;
	UINT  height = ImgHeight;
	
    int AreaWidth=ImgWidth/RatioX;
	int AreaHeight=ImgHeight/RatioY;
	
	int BlockNum=RatioX*RatioY;
    STRYCbCr *tempBlock=new STRYCbCr[BlockNum];
	CPoint *Block=new CPoint[BlockNum];
	CSize s0;
	s0.cx=AreaWidth;
	s0.cy=AreaHeight;
    memset(tempBlock, 0, BlockNum*sizeof(STRYCbCr));
	memset(pRedSpotYCbCr, 0, sizeof(RedSpotY));
	for (int j=0;j<RatioY;j++)
	{
		for (int i=0;i<RatioX;i++)
		{
			Block[i+RatioX*j].x=i*AreaWidth;
			Block[i+RatioX*j].y=j*AreaHeight;
			nGetYCbCrAverage(pIn, &tempBlock[i+RatioX*j], width, height, &Block[i+RatioX*j], &s0,OutFormat);

			if (tempBlock[i+RatioX*j].Y>pRedSpotYCbCr->OutputYrgb.Y)
			{
				pRedSpotYCbCr->OutputYrgb.Y=tempBlock[i+RatioX*j].Y;
				pRedSpotYCbCr->YrgbPoint[0].x=i;
				pRedSpotYCbCr->YrgbPoint[0].y=j;
			}
			if (tempBlock[i+RatioX*j].Cb>pRedSpotYCbCr->OutputYCbCr.Cb)
			{
				pRedSpotYCbCr->OutputYCbCr.Cb=tempBlock[i+RatioX*j].Cb;
				pRedSpotYCbCr->YrgbPoint[1].x=i;
				pRedSpotYCbCr->YrgbPoint[1].y=j;
			}
			if (tempBlock[i+RatioX*j].Cr>pRedSpotYCbCr->OutputYCbCr.Cr)
			{
				pRedSpotYCbCr->OutputYCbCr.Cr=tempBlock[i+RatioX*j].Cr;
				pRedSpotYCbCr->YrgbPoint[2].x=i;
				pRedSpotYCbCr->YrgbPoint[2].y=j;
			}
	
		}
		
	}
	delete[] tempBlock;
	delete[] Block; 
   return TRUE;
}