#include "stdafx.h"

#include <stdlib.h>
#include <malloc.h>
#include "ctmf.c"
#include <stdio.h>

#include "OfBlemishTest.h"
#include "BlemishSpecDlg.h"

CBlemishSpecDlg SpecDlg;

#define  ShrinkWidthInt 400
#define  ShrinkHightInt 300

#define  ShrinkWidthFloat 400.0
#define  ShrinkHightFloat 300.0




static enum math{Add, Sub, Mult, Div };

typedef struct _ImageSize
{
	unsigned short Width;
	unsigned short Height;
	
}ImageSize;

typedef struct _ImagePoint
{
    unsigned short  X;
    unsigned short  Y;
}ImagePoint;

struct Result_POG
{
    ImagePoint              StartPoint;  
    ImagePoint              EndPoint;           
    unsigned short          PixelCount; 
    unsigned short          Intensity;  
    struct Result_POG       *Next;
};

struct ResultStruct
{
    unsigned short          POGamount;
    struct Result_POG       *POG;
};

struct ResultStruct         Result;

typedef struct _POGRes
{
	unsigned short MedianA;
	unsigned short MedianB;
	unsigned short Threshold;//10
	unsigned short _NormalPixelLimit;
	BOOL _Log;
	BOOL _ImageSave;
	BOOL _LoadRawFile;
	BOOL _LoadBmpFile;
	
}POGRes;


BYTE ImagePreprocess(BYTE *_srcImage,BYTE *OriginImage, ImageSize SrcImageSize)
{
	int ImageSizeNum = SrcImageSize.Width*SrcImageSize.Height;
	int ColorR = 0;
	int ColorG = 0;
	int ColorB = 0;

	int MaxR = 0;
	int MaxG = 0;
	int MaxB = 0;

	int MinR = 255;
	int MinG = 255;
	int MinB = 255;

	int difR = 0;
	int difG = 0;
	int difB = 0;

	int Yvalue = 0;

	int Count = 0;

	for(int i=0; i<ImageSizeNum; i++)
	{
		ColorB = _srcImage[Count++];
		ColorG = _srcImage[Count++];
		ColorR = _srcImage[Count++];

		
		if(ColorB > MaxB) MaxB = ColorB;
		if(ColorB < MinB) MinB = ColorB;
		if(ColorG > MaxG) MaxG = ColorG;
		if(ColorG < MinG) MinG = ColorG;
		if(ColorR > MaxR) MaxR = ColorR;
		if(ColorR < MinR) MinR = ColorR;
		
	}

	difR = ((MaxR - MinR) == 0) ? 1 : MaxR-MinR;
	difG = ((MaxG - MinG) == 0) ? 1 : MaxG-MinG;
	difB = ((MaxB - MinB) == 0) ? 1 : MaxB-MinB;

	Count = 0;

	for(i=0; i<ImageSizeNum; i++)
	{
		ColorB = _srcImage[Count++];
		ColorG = _srcImage[Count++];
		ColorR = _srcImage[Count++];

		ColorB = BYTE(255-((MaxB-ColorB)*255/difB));
		ColorG = BYTE(255-((MaxG-ColorG)*255/difG));
		ColorR = BYTE(255-((MaxR-ColorR)*255/difR));
		
		Yvalue = (ColorB+ColorG+ColorR)/3;

		OriginImage[i] = BYTE(Yvalue);
	}
	return TRUE;
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

BYTE ImageShrink(BYTE *_SrcImage, BYTE *_DestImage, ImageSize SrcImageSize)
{
	unsigned int PixelSum;
	unsigned int PixelCount;
	unsigned int AvgValue;
	unsigned int SrcImgPosition;
	
	double Hscale = (double)SrcImageSize.Width/ShrinkWidthFloat;
	double Vscale = (double)SrcImageSize.Height/ShrinkHightFloat;
	
	int Border4H = (int)(((Hscale/2)*1000+0.5)/1000);
	int Border4V = (int)(((Vscale/2)*1000+0.5)/1000);
	
	if(Border4H == 0)
        Border4H = 1;
    if(Border4V == 0)
        Border4V = 1;
	
	ImagePoint SrcPoint;
	int DestY,DestX,SrcY,SrcX;
	
	for (DestY=0; DestY<ShrinkHightInt; DestY++)
	{
		for (DestX=0; DestX<ShrinkWidthInt; DestX++)
		{
			SrcPoint.X = (int)(DestX*Hscale);
			SrcPoint.Y = (int)(DestY*Vscale);
			
			PixelSum	= 0;
			PixelCount  = 0;
			
			for (SrcY=(SrcPoint.Y-Border4V); SrcY<(SrcPoint.Y+Border4V); SrcY++)
			{
				for (SrcX=(SrcPoint.X-Border4H); SrcX<(SrcPoint.X+Border4H); SrcX++)
				{
					if(((SrcX >= 0) && (SrcX <= SrcImageSize.Width)) && ((SrcY >= 0) && (SrcY < SrcImageSize.Height)))
					{
						SrcImgPosition = (SrcY*SrcImageSize.Width)+SrcX;
						PixelSum += _SrcImage[SrcImgPosition];
						PixelCount++;
					} 
				}
			}
			
			PixelCount = (PixelCount == 0) ? 1 : PixelCount;
			AvgValue = PixelSum/PixelCount;
			*_DestImage++ = AvgValue;
		}
	}
	
	return TRUE;
} 

bool FastMedianFilter(BYTE *_SrcImg, BYTE *_DstImg, ImageSize _SrcSize, int _Range)
{
	ctmf(&_SrcImg[0], &_DstImg[0], _SrcSize.Width, _SrcSize.Height, _SrcSize.Width, 1, _SrcSize.Width, 1, _Range-1);
	
    return true;
}

void  DebugInfo(char * format,...)
{	
	char buf[2048];	
	va_list ap;
    va_start(ap, format);
	
	int len=vsprintf(buf,format,ap);
    buf[len]=0;
	char Budout[4096];
	strcpy(Budout,"Ofilm->");
	strcat(Budout,buf);
	OutputDebugString(Budout);
	va_end(ap);
	char FileContent[16384]={0};
	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);
	
	char TimeTemp[64]={0};
	char FileNameTemp[64]={0};
	CreateDirectory("TempLog",NULL);
	sprintf(FileNameTemp,"TempLog\\DebugInfo_%04d%02d%02d_%02d.log",NowTime.wYear,NowTime.wMonth,NowTime.wDay,NowTime.wHour);
	
	sprintf(TimeTemp,"%04d-%02d-%02d %02d:%02d:%02d:%03d",NowTime.wYear,NowTime.wMonth,NowTime.wDay,NowTime.wHour,NowTime.wMinute,NowTime.wSecond,NowTime.wMilliseconds);
	FILE *pFIle=fopen(FileNameTemp,"a");
	strcpy(FileContent,TimeTemp);
	strcat(FileContent,"   ");
	strcat(FileContent,buf);
	strcat(FileContent,"\n");
	fwrite(FileContent,1,strlen(FileContent),pFIle);
	fclose(pFIle);
}

BYTE ImageMath(BYTE *_SrcImage1, BYTE *_SrcImage2, BYTE *DestImage, ImageSize PostImageSize, math _Operater, int XStart,
			   int XEnd, int YStart, int YEnd )
{
	int temp;
	int index;
	
	for( int j = 0; j < PostImageSize.Height; j++ )
	{
		for( int i = 0; i < PostImageSize.Width; i++ )
		{
			index = j*PostImageSize.Width+i;
			
			switch( _Operater )
			{
			case Add:
				temp = _SrcImage1[index] + _SrcImage2[index];
				break;
				
			case Sub:
				temp = _SrcImage1[index] - _SrcImage2[index]; 
				
				break;
			case Mult:
				temp = _SrcImage1[index] * _SrcImage2[index];
				break;
			case Div:
				_SrcImage2[index] = (_SrcImage2[index] == 0) ? 1 : _SrcImage2[index];
				temp = _SrcImage1[index] / _SrcImage2[index];
				break;
			} 
			
			if(temp > 128)
			{
				temp = 128;
			}  
			
			DestImage[index] = temp;
			
		}
	}
	
	return TRUE;
}


BYTE PixelMath(BYTE *_SrcImage, double MeanValue, BYTE *_DestImage, ImageSize MathImageSize, math _Operater)
{
	int _MathImageSize = MathImageSize.Width * MathImageSize.Height;
	int i;

	if(_Operater == Div)
	{MeanValue = (MeanValue == 0) ? 1 : MeanValue;}

	for(i=0; i<_MathImageSize; i++)
	{
		switch(_Operater)
		{
			case Add:
				_DestImage[i] = (BYTE)(_SrcImage[i] + MeanValue);
				break;
			case Sub:
				_DestImage[i] = (BYTE)(_SrcImage[i] - MeanValue);
				break;
			case Mult:
				_DestImage[i] = (BYTE)(_SrcImage[i] * MeanValue);
				break;
			case Div:
				_DestImage[i] = (BYTE)(_SrcImage[i] / MeanValue);
				break;
		}
	}
	return TRUE;
}
void GetModulePath(char *path)
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	
	char ModulePath[128] = {0};
	
	GetModuleFileName(NULL, ModulePath, 128);
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
}


BYTE RemoveCorner(BYTE *_SrcImage, ImageSize CorImageSize, int Median, int Mean, int offset)
{
	ImagePoint CenterPoint;
	int Radius;
	int PixelPosition;
	double Length;
	int i,j;
	
    Radius = Median + offset;//10
	CenterPoint.X = Radius;
	CenterPoint.Y = Radius;
	
	for(i=0; i<Radius; i++)
	{
		for(j=0; j<Radius; j++)
		{
			PixelPosition = i*CorImageSize.Width + j;
			Length = sqrt(pow(CenterPoint.X-j,2) + pow(CenterPoint.Y-i ,2));
			if(Length >= Radius)
			{
				_SrcImage[PixelPosition] = Mean;
			}
		}	
	}
	
	CenterPoint.X = Radius;
	CenterPoint.Y = CorImageSize.Height-Radius;
	
	for(i=(CorImageSize.Height-Radius); i<CorImageSize.Height; i++)
	{
		for(j=0; j<Radius; j++)
		{
			PixelPosition = i*CorImageSize.Width + j;
			
			Length = sqrt(pow(CenterPoint.X-j,2) + pow(CenterPoint.Y-i,2));
			
			if(Length >= Radius)
			{
				_SrcImage[PixelPosition] = Mean;
			}
		}	
	}
	
	CenterPoint.X = CorImageSize.Width-Radius;
	CenterPoint.Y = Radius;
	
	for(i=0; i<Radius; i++)
	{
		for(j=(CorImageSize.Width-Radius); j<CorImageSize.Width; j++)
		{
			PixelPosition = i*CorImageSize.Width + j;
			Length = sqrt(pow(CenterPoint.X-j,2) + pow(CenterPoint.Y-i,2));
	
			if(Length >= Radius)
			{
				_SrcImage[PixelPosition] = Mean;
			}
		}	
	}
	
	CenterPoint.X = CorImageSize.Width-Radius;
	CenterPoint.Y = CorImageSize.Height-Radius;
	
	for(i=(CorImageSize.Height-Radius); i<CorImageSize.Height; i++)
	{
		for(j=(CorImageSize.Width-Radius); j<CorImageSize.Width; j++)
		{
			PixelPosition = i*CorImageSize.Width + j;
			Length = sqrt(pow(CenterPoint.X-j,2) + pow(CenterPoint.Y-i,2));
			if(Length >= Radius)
			{
				_SrcImage[PixelPosition] = Mean;
			}
		}	
	}
	
	return TRUE;
}

BYTE ContrastEnhance(BYTE *_SrcImage, BYTE *_DestImage, ImageSize TrastImageSize)
{
	int _TempSize = TrastImageSize.Width * TrastImageSize.Height;
	int Min;
	int Max;
	int Delta;
	int i;

	Min = 255;
	Max = 0;

	for(i=0; i<_TempSize; i++)
	{
		if(_SrcImage[i] > Max) Max = _SrcImage[i];
		if(_SrcImage[i] < Min) Min = _SrcImage[i];
	}
	Delta = ((Max-Min) == 0) ? 1 : (Max-Min);

	for(i=0; i<_TempSize; i++)
	{
		_DestImage[i] = (BYTE)(255-((Max-_SrcImage[i])*255/Delta));
	}

	return TRUE;
}

BYTE ThresholdPOG(BYTE *_SrcImage, BYTE *_DestImage, ImageSize SholdImageSize, int Threshold, int Ave)
{
	int _TempImageSize = SholdImageSize.Width * SholdImageSize.Height;
	int AveValue;
	int ThresholdValue;
	int i;

	AveValue = Ave;

	ThresholdValue = AveValue + Threshold;

	for(i=0; i<_TempImageSize; i++)
	{
		if(_SrcImage[i] >= ThresholdValue)
		{
			_DestImage[i] = _SrcImage[i] - ThresholdValue + 1;
		}
		else
		{
			_DestImage[i] = 0;
		}
	}

	return TRUE;
}

BYTE AddPOG(Result_POG *_POG)
{
    
	struct Result_POG *ptr;
    ptr = Result.POG;
    if(ptr == NULL)
    {
        Result.POG = _POG;
    }
    else
    {
        while(ptr[0].Next != NULL)
        {
            ptr = ptr[0].Next;
        }
        ptr[0].Next = _POG;
    }
    return TRUE;
	
}

int ImageMathPost( BYTE* _SrcImg, BYTE* _DstImg, ImageSize _SrcSize, int XStart, int XEnd, int YStart, int YEnd, int intensityCntSpc )
 { 
	 int i,j, x,y;
	 int PixelPosition;
	 int tmpPosition;
	 int Height;
	 int Width;
	 int ImgSize; 
	 int Intensity;
	 int tempx;
	 int tempy;
	 int intensityCnt;

	 int ClusterCheckFlag;

	 BOOL LoopCheckFlag;

	 Width = _SrcSize.Width;
	 Height = _SrcSize.Height;
	 ImgSize = Width*Height;

	 typedef struct _ImgInfo
	 {
		 unsigned short x;
		 unsigned short y;
		 bool bCount;
		 BYTE val;  

	 }ImgInfo; 

     ImgInfo* myImgInfo;
	 myImgInfo = (ImgInfo*)malloc(sizeof(ImgInfo)*ImgSize);

	 int* PositionStack;
	 int StackPoint = 0;
	 PositionStack = (int*)malloc(sizeof(int)*ImgSize);

	 int *RemainPixelArray;
     int RemainPixelCount; 
	 RemainPixelArray = (int *)malloc(sizeof(int)*ImgSize);

	 if (NULL == myImgInfo || NULL == PositionStack || NULL == RemainPixelArray)
	 {
		 if(myImgInfo != NULL) free(myImgInfo);
		 if(PositionStack != NULL) free(PositionStack);
		 if(RemainPixelArray != NULL) free(RemainPixelArray);
		 return -1;
	 }

	 int Stack[10000];
	 

	 Intensity = 0;

	 for( i = 0; i < ImgSize; i++ )
		 _DstImg[i] = _SrcImg[i]; 

	 for( i = 0; i < Height; i++ )
	 {
		 for( j = 0; j < Width; j++ )
		 {
			 PixelPosition = i*Width+j; 
             myImgInfo[PixelPosition].x = j;
			 myImgInfo[PixelPosition].y = i;
			 myImgInfo[PixelPosition].val = _SrcImg[PixelPosition];
			 myImgInfo[PixelPosition].bCount = FALSE;
		 }
	 } 

	 for(   y = 0; y < Height; y++ )
	 {
		 for(   x = 0; x < Width; x++ )
		 {
			 PixelPosition =  y*Width+x;

			 if( ( myImgInfo[PixelPosition].bCount == FALSE ) && ( myImgInfo[PixelPosition].val ==  1)  )
			 {
				 intensityCnt = 0;
				 ClusterCheckFlag = FALSE;
				 RemainPixelCount = 0;
				 StackPoint = 0;

				 Intensity = 1;   

				 while( !ClusterCheckFlag )
				 {
					 LoopCheckFlag = true;

					 if( myImgInfo[PixelPosition].val > Intensity )
					 {
						 Intensity = myImgInfo[PixelPosition].val; 
					 } 

					 if( RemainPixelArray[RemainPixelCount] != PixelPosition )
					 { 
						 if( myImgInfo[PixelPosition].val >= 2 )
						 {
							 if( intensityCnt == 0 )
							 {
								 Stack[0] = PixelPosition;
								 intensityCnt++;
							 }
							 else
							 {
								 for( int k = 0; k < intensityCnt; k++ )
								 {
									 if( Stack[k] == PixelPosition )
									 {
										 break;
									 }
								 }

								 if( k == intensityCnt )
								 {
									 Stack[intensityCnt] = PixelPosition;
							    	 intensityCnt++;
								 }
							 }
							     
						 }

						 RemainPixelArray[RemainPixelCount++] = PixelPosition;  
					 }

					 myImgInfo[PixelPosition].bCount = TRUE; 
				
					 tmpPosition = PixelPosition+1;

		    	 	 if( (myImgInfo[PixelPosition].x+1 < Width)  && (!myImgInfo[tmpPosition].bCount) )
					 {
						 if( (myImgInfo[tmpPosition].val >= 1) && (myImgInfo[tmpPosition].val < 20 ) && LoopCheckFlag )
						 {
							 PositionStack[StackPoint++] = PixelPosition;
				             RemainPixelArray[RemainPixelCount++] = tmpPosition;
                             PixelPosition = tmpPosition;

							 if( myImgInfo[PixelPosition].val > Intensity )
							 {
								 Intensity = myImgInfo[PixelPosition].val; 
							 }
							 
							 if( myImgInfo[PixelPosition].val >= 2 )
							 {
								 if( intensityCnt == 0 )
								 {
									 Stack[0] = PixelPosition;
									 intensityCnt++;
								 }
								 else
								 {
									 for( int k = 0; k < intensityCnt; k++ )
									 {
										 if( Stack[k] == PixelPosition )
										 {
											 break;
										 }
									 }
									 
									 if( k == intensityCnt )
									 {
										 Stack[intensityCnt] = PixelPosition;
										 intensityCnt++;
									 }
								 }
								 
							 }

							 LoopCheckFlag = false; 
						 }
					 } 

					 tmpPosition = PixelPosition - Width + 1;
					 if( (myImgInfo[PixelPosition].x+1 < Width) && (myImgInfo[PixelPosition].y-1 > 0) && (!myImgInfo[tmpPosition].bCount) )
					 {
						 if( (myImgInfo[tmpPosition].val >= 1) && (myImgInfo[tmpPosition].val <= 20 ) && LoopCheckFlag )
						 {
							 PositionStack[StackPoint++] = PixelPosition;
						 	 RemainPixelArray[RemainPixelCount++] = tmpPosition;
                             PixelPosition = tmpPosition;

							 if( myImgInfo[PixelPosition].val > Intensity )
							 {
								 Intensity = myImgInfo[PixelPosition].val; 
							 }
							 
							 if( myImgInfo[PixelPosition].val >= 2 )
							 {
								 if( intensityCnt == 0 )
								 {
									 Stack[0] = PixelPosition;
									 intensityCnt++;
								 }
								 else
								 {
									 for( int k = 0; k < intensityCnt; k++ )
									 {
										 if( Stack[k] == PixelPosition )
										 {
											 break;
										 }
									 }
									 
									 if( k == intensityCnt )
									 {
										 Stack[intensityCnt] = PixelPosition;
										 intensityCnt++;
									 }
								 }
								 
							 }

							 LoopCheckFlag = false; 
						 }
					 }

					 tmpPosition  = PixelPosition - Width;
					 if(  (myImgInfo[PixelPosition].y-1 > 0)&& (!myImgInfo[tmpPosition].bCount) )
					 {
						 if( (myImgInfo[tmpPosition].val >= 1) && (myImgInfo[tmpPosition].val <= 20 ) && LoopCheckFlag )
						 {
							 PositionStack[StackPoint++] = PixelPosition;
					 		 RemainPixelArray[RemainPixelCount++] = tmpPosition;
                             PixelPosition = tmpPosition;

							 if( myImgInfo[PixelPosition].val > Intensity )
							 {
								 Intensity = myImgInfo[PixelPosition].val; 
							 }
							 
							 if( myImgInfo[PixelPosition].val >= 2 )
							 {
								 if( intensityCnt == 0 )
								 {
									 Stack[0] = PixelPosition;
									 intensityCnt++;
								 }
								 else
								 {
									 for( int k = 0; k < intensityCnt; k++ )
									 {
										 if( Stack[k] == PixelPosition )
										 {
											 break;
										 }
									 }
									 
									 if( k == intensityCnt )
									 {
										 Stack[intensityCnt] = PixelPosition;
										 intensityCnt++;
									 }
								 }
								 
							 }

							 LoopCheckFlag = false; 
						 }
					 }

					 // check (x-1, y-1)
                     tmpPosition = PixelPosition - Width - 1;   
					 if(  (myImgInfo[PixelPosition].y-1 > 0) && (myImgInfo[PixelPosition].x-1 > 0)&& (!myImgInfo[tmpPosition].bCount))
					 {
						 if( (myImgInfo[tmpPosition].val >= 1) && (myImgInfo[tmpPosition].val <= 20 ) && LoopCheckFlag )
						 {
							 PositionStack[StackPoint++] = PixelPosition;
					 		 RemainPixelArray[RemainPixelCount++] = tmpPosition;
                             PixelPosition = tmpPosition;

							 if( myImgInfo[PixelPosition].val > Intensity )
							 {
								 Intensity = myImgInfo[PixelPosition].val; 
							 }
							 
							 if( myImgInfo[PixelPosition].val >= 2 )
							 {
								 if( intensityCnt == 0 )
								 {
									 Stack[0] = PixelPosition;
									 intensityCnt++;
								 }
								 else
								 {
									 for( int k = 0; k < intensityCnt; k++ )
									 {
										 if( Stack[k] == PixelPosition )
										 {
											 break;
										 }
									 }
									 
									 if( k == intensityCnt )
									 {
										 Stack[intensityCnt] = PixelPosition;
										 intensityCnt++;
									 }
								 } 
							 }

							 LoopCheckFlag = false; 
						 }
					 }

					 // check (x-1, y)
					 tmpPosition = PixelPosition - 1;  
					 if((myImgInfo[PixelPosition].x-1 > 0) && (!myImgInfo[tmpPosition].bCount))
					 {
						 if( (myImgInfo[tmpPosition].val >= 1) && (myImgInfo[tmpPosition].val <= 20 ) && LoopCheckFlag )
						 {
							 PositionStack[StackPoint++] = PixelPosition;
					 		 RemainPixelArray[RemainPixelCount++] = tmpPosition;
                             PixelPosition = tmpPosition;

							 if( myImgInfo[PixelPosition].val > Intensity )
							 {
								 Intensity = myImgInfo[PixelPosition].val; 
							 }
							 
							 if( myImgInfo[PixelPosition].val >= 2 )
							 {
								 if( intensityCnt == 0 )
								 {
									 Stack[0] = PixelPosition;
									 intensityCnt++;
								 }
								 else
								 {
									 for( int k = 0; k < intensityCnt; k++ )
									 {
										 if( Stack[k] == PixelPosition )
										 {
											 break;
										 }
									 }
									 
									 if( k == intensityCnt )
									 {
										 Stack[intensityCnt] = PixelPosition;
										 intensityCnt++;
									 }
								 }
								 
							}

							 LoopCheckFlag = false; 
						 }
					 }

					 // check (x-1, y+1)
                    tmpPosition = PixelPosition + Width - 1;

					if(  (myImgInfo[PixelPosition].x-1 > 0) &&  (myImgInfo[PixelPosition].y+1 < Height)&& (!myImgInfo[tmpPosition].bCount)  )
					{
						if( (myImgInfo[tmpPosition].val >= 1) && (myImgInfo[tmpPosition].val <= 20 ) && LoopCheckFlag )
						{
							PositionStack[StackPoint++] = PixelPosition;
					    	RemainPixelArray[RemainPixelCount++] = tmpPosition;
							PixelPosition = tmpPosition;

							if( myImgInfo[PixelPosition].val > Intensity )
							{
								Intensity = myImgInfo[PixelPosition].val; 
							}
							
							if( myImgInfo[PixelPosition].val >= 2 )
							{
								if( intensityCnt == 0 )
								{
									Stack[0] = PixelPosition;
									intensityCnt++;
								}
								else
								{
									for( int k = 0; k < intensityCnt; k++ )
									{
										if( Stack[k] == PixelPosition )
										{
											break;
										}
									}
									
									if( k == intensityCnt )
									{
										Stack[intensityCnt] = PixelPosition;
										intensityCnt++;
									}
								} 
							 }

							LoopCheckFlag = false; 
						}
					 }

					// check (x, y+1)
                    tmpPosition = PixelPosition + Width;
					
					if ( (myImgInfo[PixelPosition].y+1 < Height) && (!myImgInfo[tmpPosition].bCount) ) 
					{
						if( (myImgInfo[tmpPosition].val >= 1) && (myImgInfo[tmpPosition].val <= 20 ) && LoopCheckFlag )
						{
							PositionStack[StackPoint++] = PixelPosition;
					 		RemainPixelArray[RemainPixelCount++] = tmpPosition;
							PixelPosition = tmpPosition;

							if( myImgInfo[PixelPosition].val > Intensity )
							{
								Intensity = myImgInfo[PixelPosition].val; 
							}
							
							if( myImgInfo[PixelPosition].val >= 2 )
							{
								if( intensityCnt == 0 )
								{
									Stack[0] = PixelPosition;
									intensityCnt++;
								}
								else
								{
									for( int k = 0; k < intensityCnt; k++ )
									{
										if( Stack[k] == PixelPosition )
										{
											break;
										}
									}
									
									if( k == intensityCnt )
									{
										Stack[intensityCnt] = PixelPosition;
										intensityCnt++;
									}
								}
								
							 }

							LoopCheckFlag = false; 
						}
					 }

					tmpPosition = PixelPosition + Width+1;
					
					if((myImgInfo[PixelPosition].y+1 < Height) && (myImgInfo[PixelPosition].x+1 <Width)&& (!myImgInfo[tmpPosition].bCount)) 
					{
						if( (myImgInfo[tmpPosition].val >= 1) && (myImgInfo[tmpPosition].val <= 20 ) && LoopCheckFlag )
						{
							PositionStack[StackPoint++] = PixelPosition;
				    		RemainPixelArray[RemainPixelCount++] = tmpPosition;
							PixelPosition = tmpPosition;

							if( myImgInfo[PixelPosition].val > Intensity )
							{
								Intensity = myImgInfo[PixelPosition].val; 
							}
							
							if( myImgInfo[PixelPosition].val >= 2 )
							{
								if( intensityCnt == 0 )
								{
									Stack[0] = PixelPosition;
									intensityCnt++;
								}
								else
								{
									for( int k = 0; k < intensityCnt; k++ )
									{
										if( Stack[k] == PixelPosition )
										{
											break;
										}
									}
									
									if( k == intensityCnt )
									{
										Stack[intensityCnt] = PixelPosition;
										intensityCnt++;
									}
								}
								
							 }

							LoopCheckFlag = false; 
						}
					 }

					if(LoopCheckFlag && (StackPoint > 0))
                    {
                        PixelPosition = PositionStack[StackPoint-1];
                        StackPoint--;

						if( myImgInfo[PixelPosition].val > Intensity )
						{
							Intensity = myImgInfo[PixelPosition].val; 
						}
						
						if( myImgInfo[PixelPosition].val >= 2 )
						{
							if( intensityCnt == 0 )
							{
								Stack[0] = PixelPosition;
								intensityCnt++;
							}
							else
							{
								for( int k = 0; k < intensityCnt; k++ )
								{
									if( Stack[k] == PixelPosition )
									{
										break;
									}
								}
								
								if( k == intensityCnt )
								{
									Stack[intensityCnt] = PixelPosition;
									intensityCnt++;
								}
							}
							
						 }

                        LoopCheckFlag = false;
                    }

					if( LoopCheckFlag && (StackPoint == 0))
                    {
                        ClusterCheckFlag = true; 

						////////////////////////////////////
						if( Intensity < 2 || intensityCnt < intensityCntSpc )
						{
							for(i=0; i<RemainPixelCount; i++)
							{
								tempx = RemainPixelArray[i]%Width;
                                tempy = RemainPixelArray[i]/Width;

								if( tempx  < XStart || tempx > XEnd || tempy < YStart || tempy > YEnd )
								{
										_DstImg[RemainPixelArray[i]] = 0; 
								} 
							}
						} 
					}   
				 }
			 } 
		 }
	 }

	 free(PositionStack);
     free(RemainPixelArray);
	 free(myImgInfo);

	 return 1; 
}

int ConnectedObjectFilter(BYTE *_SrcImg, BYTE *_DstImg, ImageSize _SrcSize, int _NormalPixelLimit, BOOL _Log)
{
	int i,x,y;
	int PixelPosition;
    int tmpPosition;
    int tempx, tempy;
	int StackPoint;
	int Height;
	int Width;
    int LeftEdge;
    int RightEdge;
    int TopEdge;
    int BottomEdge;
    long Intensity;
	int ImageSizeNum;
	int *PositionStack = NULL;
    int *RemainPixelArray = NULL;
    int RemainPixelCount;
	bool ClusterCheckFlag;
	bool LoopCheckFlag;
    bool BorderCheckFlag;
    ImagePoint DefectLocation;
    FILE *file_out = NULL;
    struct Result_POG *POG = NULL;

	BYTE _POGRes = TRUE;

	Height = _SrcSize.Height;
	Width = _SrcSize.Width;
	ImageSizeNum = Height * Width;
	PositionStack = (int *)malloc(sizeof(int)*ImageSizeNum);
    RemainPixelArray = (int *)malloc(sizeof(int)*ImageSizeNum);

	if (NULL == PositionStack || NULL == RemainPixelArray)
	{
		if(PositionStack != NULL) free(PositionStack);
		if(RemainPixelArray != NULL) free(RemainPixelArray);
		return -1;
	}

    // clear destination image buffer
    for(i=0; i<ImageSizeNum; i++)
        _DstImg[i] = 0;

    if(_Log)
    {
        file_out = fopen("blemish.txt", "a+");
    }

	for(y=0; y<Height; y++)
	{
		for(x=0; x<Width; x++)
		{
            BorderCheckFlag = false;
			PixelPosition = (y*Width)+x;
            LeftEdge = Width;
            RightEdge = 0;
            TopEdge = Height;
            BottomEdge = 0;
            Intensity = 0;
            DefectLocation.X = _SrcSize.Width;
            DefectLocation.Y = 0;
            if(_SrcImg[PixelPosition] > 0)
            {
                RemainPixelCount = 0;
	    		StackPoint = 0;
		    	ClusterCheckFlag = false;
                PositionStack[StackPoint++] = PixelPosition;
                RemainPixelArray[RemainPixelCount++] = PixelPosition;
                LeftEdge = x;
                RightEdge = x;
                TopEdge = y;
                BottomEdge = y;
			
    			while(!ClusterCheckFlag && (StackPoint < ImageSizeNum))
	    		{
		    		LoopCheckFlag = true;
                    Intensity += _SrcImg[PixelPosition];
			    	_SrcImg[PixelPosition] = 0;

                    tempx = PixelPosition%Width;
                    tempy = PixelPosition/Width;

                    if(DefectLocation.X > tempx)
                        DefectLocation.X = tempx;
                    if(DefectLocation.Y < tempy)
                        DefectLocation.Y = tempy;

                    // border check
                    if((tempx == 0) || (tempx == Width-1))
                    {
                        BorderCheckFlag = true;
                    }
                    else if((tempy == 0) || (tempy == Height-1))
                    {
                        BorderCheckFlag = true;
                    }
                    // calculate object edge
                    if(LeftEdge > tempx)
                        LeftEdge = tempx;
                    if(RightEdge < tempx)
                        RightEdge = tempx;
                    if(TopEdge > tempy)
                        TopEdge = tempy;
                    if(BottomEdge < tempy)
                        BottomEdge = tempy;


    				// check (x+1, y)
	    			tmpPosition = PixelPosition + 1;
		    		if((tmpPosition >=0) && (tmpPosition < ImageSizeNum) && ((tempx+1) < Width))
			    	{
				    	if((_SrcImg[PixelPosition+1] > 0) && LoopCheckFlag)
					    {
    						PositionStack[StackPoint++] = PixelPosition;
                            RemainPixelArray[RemainPixelCount++] = PixelPosition;
	    					PixelPosition = tmpPosition;
		    				LoopCheckFlag = false;
			    		}
				    }

    	            // check (x+1, y-1)
        	        tmpPosition = PixelPosition - Width + 1;
            	    if((tmpPosition >= 0) && (tmpPosition < ImageSizeNum) && ((tempx+1) < Width) && ((tempy-1) > 0))
                	{
                    	if((_SrcImg[PixelPosition-Width+1] > 0) && LoopCheckFlag)
                	    {
    	                    PositionStack[StackPoint++] = PixelPosition;
                            RemainPixelArray[RemainPixelCount++] = PixelPosition;
        	                PixelPosition = tmpPosition;
            	            LoopCheckFlag = false;
                	    }
            	    }

    	            // check (x, y-1)
        	        tmpPosition = PixelPosition - Width;
            	    if((tmpPosition >= 0) && (tmpPosition < ImageSizeNum) && ((tempy-1) > 0))
                	{
                    	if((_SrcImg[PixelPosition-Width] > 0) && LoopCheckFlag)
	                    {
        	                PositionStack[StackPoint++] = PixelPosition;
                            RemainPixelArray[RemainPixelCount++] = PixelPosition;
            	            PixelPosition = tmpPosition;
                	        LoopCheckFlag = false;
                    	}
            	    }

                    // check (x-1, y-1)
                    tmpPosition = PixelPosition - Width - 1;
                    if((tmpPosition >= 0) && (tmpPosition < ImageSizeNum) && ((tempx-1) > 0) && ((tempy-1) > 0))
                    {
                        if((_SrcImg[PixelPosition-Width-1] > 0) && LoopCheckFlag)
                        {
                            PositionStack[StackPoint++] = PixelPosition;
                            RemainPixelArray[RemainPixelCount++] = PixelPosition;
                            PixelPosition = tmpPosition;
                            LoopCheckFlag = false;
                        }
                    }

                    // check (x-1, y)
                    tmpPosition = PixelPosition - 1;
                    if((tmpPosition >= 0) && (tmpPosition < ImageSizeNum) && ((tempx-1) > 0))
                    {
                        if((_SrcImg[PixelPosition-1] > 0) && LoopCheckFlag)
                        {
                            PositionStack[StackPoint++] = PixelPosition;
                            RemainPixelArray[RemainPixelCount++] = PixelPosition;
                            PixelPosition = tmpPosition;
                            LoopCheckFlag = false;
                        }
                    }

                    // check (x-1, y+1)
                    tmpPosition = PixelPosition + Width - 1;
                    if((tmpPosition >= 0) && (tmpPosition < ImageSizeNum) && ((tempx-1) > 0) && ((tempy+1) < Height))
                    {
                        if((_SrcImg[PixelPosition+Width-1] > 0) && LoopCheckFlag)
                        {
                            PositionStack[StackPoint++] = PixelPosition;
                            RemainPixelArray[RemainPixelCount++] = PixelPosition;
                            PixelPosition = tmpPosition;
                            LoopCheckFlag = false;
                        }
                    }

                    // check (x, y+1)
                    tmpPosition = PixelPosition + Width;
                    if((tmpPosition >= 0) && (tmpPosition < ImageSizeNum) && ((tempy+1) < Height))
                    {
                        if((_SrcImg[PixelPosition+Width] > 0) && LoopCheckFlag)
                        {
                            PositionStack[StackPoint++] = PixelPosition;
                            RemainPixelArray[RemainPixelCount++] = PixelPosition;
                            PixelPosition = tmpPosition;
                            LoopCheckFlag = false;
                        }
                    }

                    // check (x+1, y+1)
                    tmpPosition = PixelPosition + Width + 1;
                    if((tmpPosition >= 0) && (tmpPosition < ImageSizeNum) && ((tempx+1) < Width) && ((tempy+1) < Height))
                    {
                        if((_SrcImg[PixelPosition+Width+1] > 0) && LoopCheckFlag)
                        {
                            PositionStack[StackPoint++] = PixelPosition;
                            RemainPixelArray[RemainPixelCount++] = PixelPosition;
                            PixelPosition = tmpPosition;
                            LoopCheckFlag = false;
                        }
                    }

                    if(LoopCheckFlag && (StackPoint > 0))
                    {
                        PixelPosition = PositionStack[StackPoint-1];
                        StackPoint--;
                        LoopCheckFlag = false;
                    }

                    if(LoopCheckFlag && (StackPoint == 0))
                    {
                        ClusterCheckFlag = true;

/*                        if((_Action == Keep) && !BorderCheckFlag)*/

                            if(RemainPixelCount >= _NormalPixelLimit)
                            {
								_POGRes = FALSE;
                                for(i=0; i<RemainPixelCount; i++)
                                {
                                    _DstImg[RemainPixelArray[i]] = 255;
                                }
                                POG = (Result_POG *)malloc(sizeof(Result_POG));
								if(POG == NULL) 
								{
									free(PositionStack);
	                                free(RemainPixelArray);
									return -1;
								}

                                POG[0].StartPoint.X = LeftEdge;
                                POG[0].StartPoint.Y = TopEdge;
                                POG[0].EndPoint.X = RightEdge;
                                POG[0].EndPoint.Y = BottomEdge;
                                POG[0].PixelCount = RemainPixelCount;
                                POG[0].Intensity = Intensity;
                                POG[0].Next = NULL;
                                AddPOG(POG);
								free(POG);

                                Result.POGamount++;

                                if(_Log && file_out != NULL)
                                    fprintf(file_out, "POG Position(%d, %d), PixelCount(%d), Intensity(%d)\n", DefectLocation.X, Height - DefectLocation.Y, RemainPixelCount, Intensity);
                            }
					} 
                } // end of while
            }
		} // end of for loop x
	} // end of for loop y

    if(_Log)
	{
		fclose(file_out);
	}
    
	free(PositionStack);
	free(RemainPixelArray);
    
    return _POGRes;
}

BYTE MarkDefectPOG(BYTE *_SrcImg, ImageSize _SrcSize, MaxBlemishInfo MaxInfo)
{
    int         Position;
    double      ScaleH = 1.0;// (double)_SrcSize.Width/400.0;
    double      ScaleV = 1.0;//(double)_SrcSize.Height/300.0;
	int			ImageBytes = _SrcSize.Width*_SrcSize.Height*3;
    int         StartX;
    int         EndX;
    int         StartY;
    int         EndY;
    int         i,j,k;
	int			LineWidth;
	
	LineWidth = _SrcSize.Width / 512;//512
	

        StartX = int((double)MaxInfo.xStart);
        EndX = int((double)MaxInfo.xEnd);
        StartY = int((double)MaxInfo.yStart);
        EndY = int((double)MaxInfo.yEnd);

		MaxInfo.xStart = StartX;
		MaxInfo.xEnd   = EndX;
		MaxInfo.yStart = StartY;
		MaxInfo.yEnd   = EndY;
		
		for(k=0; k<LineWidth; k++)
		{
			// draw horizontal line
			Position = ((_SrcSize.Height-(StartY+k) )*_SrcSize.Width+StartX)*3;

			for(j=StartX; j<EndX; j++)
			{
				if ((Position >= 0) && (Position < ImageBytes))
				{
					_SrcImg[Position++] = 0;//0
					_SrcImg[Position++] = 0;//0
					_SrcImg[Position++] = 255;
				}
			}
			Position = ((_SrcSize.Height-(EndY-k))*_SrcSize.Width+StartX)*3;
			for(j=StartX; j<EndX; j++)
			{
				if((Position >= 0) && (Position < ImageBytes))
				{
					_SrcImg[Position++] = 0;
					_SrcImg[Position++] = 0;
					_SrcImg[Position++] = 255;
				}
			}
			// draw vertical line
			Position = ((_SrcSize.Height-EndY)*_SrcSize.Width+StartX+k)*3;
			for(i=StartY; i<EndY; i++)
			{
				if((Position >= 0) && (Position < ImageBytes))
				{
					_SrcImg[Position] = 0;
					_SrcImg[Position+1] = 0;
					_SrcImg[Position+2] = 255;
				}
				Position += _SrcSize.Width*3;
			}
			Position = ((_SrcSize.Height-EndY)*_SrcSize.Width+EndX-k)*3;
			for(i=StartY; i<EndY; i++)
			{
				if((Position >= 0) && (Position < ImageBytes))
				{
					_SrcImg[Position] = 0;
					_SrcImg[Position+1] = 0;
					_SrcImg[Position+2] = 255;
				}
				Position += _SrcSize.Width*3;
			}
		}

    return TRUE;
}

int LoadRawFile(char *filename,BYTE *data,int dataWidth,int dataHeight)
{
	FILE *fp;
	
	if((fp=fopen(filename,"rb"))==NULL) return 0;
	fread(data,sizeof(BYTE),dataHeight*dataWidth,fp);
	fclose(fp);
	return 1;
}

void SaveRawFile(char *filename,BYTE *data,int dataWidth,int dataHeight)
{
	FILE *fp;
	
	if((fp=fopen(filename,"wb"))==NULL) return;
	fwrite(data,sizeof(BYTE),dataHeight*dataWidth,fp);
	fclose(fp);
}

void SaveImageToBmp(char *path, BYTE* bmp_buffer, __int16 width, __int16 height)
{
	int		OffBits;
	HFILE	bmpFile;
	BITMAPFILEHEADER bmpHeader; //Header for Bitmap file
	BITMAPINFO		 bmpInfo;
	
	OffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	DWORD dwSizeImage = width*height*3;
	
	bmpHeader.bfType = ( (WORD)('M'<<8)|'B' );
	bmpHeader.bfSize = OffBits + dwSizeImage;
	bmpHeader.bfReserved1 = 0;
	bmpHeader.bfReserved2 = 0;
	bmpHeader.bfOffBits = OffBits;
	
	bmpInfo.bmiHeader.biSize            = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth           = width;
    bmpInfo.bmiHeader.biHeight          = height;
    bmpInfo.bmiHeader.biPlanes          = 1;
    bmpInfo.bmiHeader.biBitCount        = 24;
    bmpInfo.bmiHeader.biCompression     = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage       = 0; 
    bmpInfo.bmiHeader.biXPelsPerMeter   = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter   = 0;
    bmpInfo.bmiHeader.biClrUsed         = 0;
    bmpInfo.bmiHeader.biClrImportant    = 0;
	
	bmpFile = _lcreat( path, 0 );
	if( bmpFile < 0 ) return ;
	
	UINT len;
	len = _lwrite( bmpFile, (LPSTR)&bmpHeader, sizeof(BITMAPFILEHEADER) );
	len = _lwrite( bmpFile, (LPSTR)&bmpInfo, sizeof(BITMAPINFOHEADER) );
	len = _lwrite( bmpFile, (LPSTR)bmp_buffer, bmpHeader.bfSize - sizeof(bmpHeader) - sizeof(bmpInfo)+4 );  //+4 is for exact filesize
	_lclose( bmpFile );
	
	return ;	
}


BYTE SaveBmpImage(BYTE *pBmp,int width,int height, int mode)   
{
	char temp[MAX_PATH];
	char BTime[50];
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf(BTime,"%02d%02d%02d%02d%02d%02d-%d",time.wYear%100,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,mode);
	sprintf(temp,"D:\\Akon\\%s.bmp",BTime); 
	SaveImageToBmp(temp, pBmp,width, height);
	return TRUE;
}

void BayerConversionFlip( BYTE* pSrcBuffer, BYTE* pDstBuffer, int nImgWidth, int nImgHeight, int bayerType  )
{
	int width = nImgWidth;
	int height  = nImgHeight;

	int x = 0; 
	int y = 0;
	int i = 0;


	switch( bayerType )
	{
	case 0: //RGGB
		{
			//////////////////////////////			
			///Corner case;
			x = 0;
			y = 0;
			pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[(height-1)*width+1]; //B
			pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[ (height-2)*width+1] + pSrcBuffer[(height-1)*width])/2; //G
			pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[ (height-2)*width]; // R

			x = width - 1;
			y = 0;
			pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[ height*width - 1 ]; 
			pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[height*width -2] + pSrcBuffer[(height-1)*width-1] )/2;
			pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[(height-1)*width - 2];

			x = 0;
			y = height - 1;
			pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[width+1];
			pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (pSrcBuffer[1] + pSrcBuffer[width] )/2;
			pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[0];

			x = width - 1;
			y = height - 1;
			pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[width*2-1];
			pDstBuffer[ y * width * 3 + x * 3 + 1 ]  = ( pSrcBuffer[width*2 -2] + pSrcBuffer[width -1 ] )/2;
			pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[width-2];

			//for boundary
			for( int x = 1; x < width -1; x++ )
			{ 	
				if( x%2 == 1 )
				{
					y = 0;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[(height-1)*width+x]; //B
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[(height-1)*width+x-1] + pSrcBuffer[(height-1)*width+x+1] + pSrcBuffer[(height-2)*width+x] )/3; //G
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = ( pSrcBuffer[(height-2)*width+x-1] +  pSrcBuffer[(height-2)*width+x+1] )/2; //R

					y = height -1;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[width+x];
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[x] + pSrcBuffer[width+x-1] + pSrcBuffer[width+x+1] ) /3 ;
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = ( pSrcBuffer[x-1] + pSrcBuffer[x+1] )/2; 
				}
				else
				{
					y = 0;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = ( pSrcBuffer[(height-1)*width+x-1] + pSrcBuffer[(height-1)*width+x+1] )/2; //B
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[(height-1)*width+x] + pSrcBuffer[(height-2)*width+x-1] + pSrcBuffer[(height-2)*width + x +1])/3; //G //Here
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[(height-2)*width+x];	//R

					y = height -1;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[width+x-1] + pSrcBuffer[width+x+1] )/2;
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[width+x] + pSrcBuffer[x-1] + pSrcBuffer[x+1] )/3 ;
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[x] ; 

				} 
			}

			for( int y = 1;  y < height - 1; y++ )
			{
				if( y%2 == 1 )
				{
					x = 0;
					i = ( height -1 - y )*width;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = ( pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/2;
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i+1] )/3;
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[i];


					x = width -1;
					i = ( height - 1 - y)* width + x;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1])/3;  //here 
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = pSrcBuffer[i-1];  
				}
				else
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[i+1];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[i] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1])/3;          
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;         
						
						
					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[i];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] )/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1] )/2;  //Here

				}
			}


			//other area
			for( y = 1; y < height-1; y++ )
			{
				for( x=1; x < width-1; x++ )
				{
					i = (height-1-y)*width + x;

					if( y%2 == 1 )
					{
						if( x%2 == 1 ) //G1
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] =  ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] =  (pSrcBuffer[i-1] + pSrcBuffer[i+1])/2;//Here

						}
						else  //R
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] ) / 4;
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width]) / 4;
							pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[i]; 
						}
					}
					else
					{
						if( x%2  == 1)  //B
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] ) /4;//Here
							pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1]) /4;
						}
						else  //G2
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
						}

					}
				}
			}
		}
		break;

	case 1: //GRBG
		{ 
			//corner case
			x = 0;
			y = 0;
			i = (height-1)*width;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i]; //B
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+1] )/2; //G
			pDstBuffer[y * width * 3 + x * 3 + 2] =   pSrcBuffer[i-width+1]; 

			x = width -1;
			y = 0;			 
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[height*width-2];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[(height-1)*width-2] + pSrcBuffer[height*width-1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[(height-1)*width-1];

			x = 0;   
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[width];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[0] + pSrcBuffer[width+1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[1];

			x = width - 1;
			y = height - 1;
			pDstBuffer[y * width * 3 + x * 3 + 0] =  pSrcBuffer[width*2-2];
			pDstBuffer[y * width * 3 + x * 3 + 1] =  ( pSrcBuffer[width*2 -1] +  pSrcBuffer[width-2] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[width -1];
			

			//for boundary case
		    for( x = 1; x < width -1; x++ )
			{
				if( x%2 == 1 )
				{
					y = 0;                                             
					i = ( height -1  )*width + x;  //G                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = (  pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i-width]; 
						
						
					y = height - 1;                                      
					i = x;       //R               
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/2;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+width] + pSrcBuffer[i+1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i];    

				}
				else
				{
					y = 0;                                             
					i = ( height -1  )*width + x;  //B                     
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] )/2; 
					
					
					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i+width] ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/3 ;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;    
				}
			}
 
			for( y = 1; y < height - 1; y++ )
			{
				if( y%2 == 1 )
				{
					x = 0;                                             
					i = ( height -1 - y )*width;  //G                     
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = (  pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i+1]; 
					
					
					x = width -1;                                      
					i = ( height - 1 - y)* width + x; //R                     
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1] )/2;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i];   

				}
				else
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i+1])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/2; 
					
					
					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i-1] ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (  pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;  

				}
			}
			
			//other area
			for( y = 1; y < height -1; y++ )
			{
				for( x = 1; x < width -1; x++ )
				{
					i = ( height -1 -y)*width + x;

					if( y%2 == 1 )
					{
						if( x%2 == 1 ) //R
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/4;
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1])/4;
							pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[i];
 
						}
						else     //G1
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2; 
						}
					}
					else
					{
						if( x%2 == 1 ) //G2
						{   
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;

						}
						else //B
						{
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   pSrcBuffer[i];     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )/4;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/4; 
						}

					}
				}

			} 
		}
		break;

	case 2:   //GBRG
		{
			//corner case
			x = 0;
			y = 0;
			i = (height-1)*width;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i-width+1]; //B
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+1] )/2; //G
			pDstBuffer[y * width * 3 + x * 3 + 2] =   pSrcBuffer[i]; //R

			x = width -1;
			y = 0;			 
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[(height - 1)*width-1];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[(height-1)*width-2] + pSrcBuffer[height*width-1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[ height*width-2];

			x = 0;   
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[1];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[0] + pSrcBuffer[width +1 ] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[width];

			x = width -1;
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] =  pSrcBuffer[width-1];
			pDstBuffer[y * width * 3 + x * 3 + 1] =  ( pSrcBuffer[width-2] +  pSrcBuffer[width*2-1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[width*2 -2];
			

			//for boundary case
		    for( x = 1; x < width -1; x++ )
			{
				if( x%2 == 1 )
				{
					y = 0;                                             
					i = ( height -1  )*width + x;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[i-width];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
						
						
					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i];          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/2;    

				}
				else
				{
					y = 0;                                             
					i = ( height -1  )*width + x;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i]; 
					
					
					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1]  )/2 ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/3 ;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i+width];    
				}
			}
 
			for( y = 1; y < height - 1; y++ )
			{
				if( y%2 == 1 )
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i+1];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i-width+1]+ pSrcBuffer[i+width+1])/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;//Here 
					
					
					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i];          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+width])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1] )/2;   

				}
				else
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i+1] + pSrcBuffer[i+width])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i]; 
					
					
					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2 ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (  pSrcBuffer[i-width-1] + pSrcBuffer[i] + pSrcBuffer[i+width-1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i-1];  

				}
			}
			
			//other area
			for( y = 1; y < height -1; y++ )
			{
				for( x = 1; x < width -1; x++ )
				{
					i = ( height -1 -y)*width + x;

					if( y%2 == 1 )
					{
						if( x%2 == 1 )  
						{
							//B
							pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width])/4;
							pDstBuffer[y * width * 3 + x * 3 + 2] = (pSrcBuffer[i-width-1] +pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/4;
 
						}
						else      
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width] +  + pSrcBuffer[i+width])/2; 
						}
					}
					else
					{
						if( x%2 == 1 )  
						{   
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =     ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;

						}
						else  
						{
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width-1] +pSrcBuffer[i-width+1]+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/4;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width]   + pSrcBuffer[i-1] + pSrcBuffer[i+1] )/4;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   pSrcBuffer[i]; 
						}

					}
				}

			}

		}
		break;

		

	case 3:  //BGGR		
		//////////////////////////////////////
		{
			//corner case
			x = 0;
			y = 0;
			i = (height-1)*width;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i-width]; //B
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i] + pSrcBuffer[i-width+1] )/2;  
			pDstBuffer[y * width * 3 + x * 3 + 2] =   pSrcBuffer[i+1]; //R

			x = width -1;
			y = 0;			 
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[(height - 1)*width-2];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[height*width-2] + pSrcBuffer[(height-1)*width-1] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[ height*width-1];

			x = 0;   
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[0];
			pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[1] + pSrcBuffer[width] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] = pSrcBuffer[width+1];

			x = width -1;
			y = height -1;
			pDstBuffer[y * width * 3 + x * 3 + 0] =  pSrcBuffer[width-2];
			pDstBuffer[y * width * 3 + x * 3 + 1] =  ( pSrcBuffer[width-1] +  pSrcBuffer[width*2-2] )/2;
			pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[width*2 -1];
			

			//for boundary case
		    for( x = 1; x < width -1; x++ )
			{
				if( x%2 == 1 )
				{
					y = 0;                                             
					i = ( height -1  )*width + x;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = (  pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i]; 
						
						
					y = height - 1;                                      
					i = x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i+width];    

				}
				else
				{
					y = 0;                                             
					i = ( height -1  )*width + x;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i-width];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
					
					
					y = height - 1;                                      
					i = x;
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i] ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width] )/3 ;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/2;
				}
			}
 
			for( y = 1; y < height - 1; y++ )
			{
				if( y%2 == 1 )
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] = pSrcBuffer[i];      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+1] + pSrcBuffer[i+width] )/3;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  (pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1])/2; 
					
					
					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i-1];          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i] + pSrcBuffer[i+width-1])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;   

				}
				else
				{
					x = 0;                                             
					i = ( height -1 - y )*width;                       
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;      
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width+1] + pSrcBuffer[i] + pSrcBuffer[i+width+1])/3 ;      
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i+1]; 
					
					
					x = width -1;                                      
					i = ( height - 1 - y)* width + x;                      
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1] )/2 ;          
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (  pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+width])/3;        
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[i];  

				}
			}
			
			//other area
			for( y = 1; y < height -1; y++ )
			{
				for( x = 1; x < width -1; x++ )
				{
					i = ( height -1 -y)*width + x;

					if( y%2 == 1 )
					{
						if( x%2 == 1 )  
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;
							pDstBuffer[y * width * 3 + x * 3 + 2] = (pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;
 
						}
						else      
						{
							pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
							pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width ] )/4;
							pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/4; 
						}
					}
					else
					{
						if( x%2 == 1 )  
						{   
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/4;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )/4;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   pSrcBuffer[i];

						}
						else  
						{
							pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;     
							pDstBuffer[ y * width * 3 + x * 3 + 1 ] =   ( pSrcBuffer[i] + pSrcBuffer[i-width-1] )/2;  
							pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
						}

					}
				}

			} 

		}

		break;
	} 
}



BOOL bSaveBmpFile(char *sfilename, BYTE *pBuffer, UINT width, UINT height)
{
	int				 OffBits;
	HFILE			 bmpFile;
	BITMAPFILEHEADER bmpHeader;  
	BITMAPINFO		 bmpInfo;
	
	OffBits           = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	DWORD dwSizeImage = width * height * 3;  //IMAGESIZE_X*IMAGESIZE_Y*3;
	
	// VertFlipBuf(pBuffer, width * 3, height); //to correct the vertical flip problem.
	
	bmpHeader.bfType      = ( (WORD)('M' << 8) | 'B');
	bmpHeader.bfSize      = OffBits + dwSizeImage;
	bmpHeader.bfReserved1 = 0;
	bmpHeader.bfReserved2 = 0;
	bmpHeader.bfOffBits   = OffBits;
	
	bmpInfo.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth         = width;
    bmpInfo.bmiHeader.biHeight        = height;
    bmpInfo.bmiHeader.biPlanes        = 1;
    bmpInfo.bmiHeader.biBitCount      = 24;
    bmpInfo.bmiHeader.biCompression   = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage     = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed       = 0;
    bmpInfo.bmiHeader.biClrImportant  = 0;
	
	bmpFile = _lcreat(sfilename, FALSE); 
	if (bmpFile < 0) 
	{
		return FALSE;
	}
	
	UINT len = 0;
	len = _lwrite(bmpFile, (LPSTR)&bmpHeader, sizeof(BITMAPFILEHEADER));
	len = _lwrite(bmpFile, (LPSTR)&bmpInfo,   sizeof(BITMAPINFOHEADER));
	len = _lwrite(bmpFile, (LPSTR)pBuffer,  bmpHeader.bfSize - sizeof(bmpHeader) - sizeof(bmpInfo) + 4);  //+4 is for exact filesize
	_lclose(bmpFile);
	
	return TRUE;
} 

float CornerCheck( BYTE* ImgBuf, int Width, int Height, int& index, float& Range, int& index_Min, BOOL _Log )
{ 
	int ImageWidth = Width;
	int ImageHeight = Height;

	int BlockWidth = ImageWidth/20;
	int BlockHeight = ImageHeight/20;

	int BlockWidth_Inner = ImageWidth/40;
	int BlockHeight_Inner = ImageHeight/40;

	int Sum = 0;
	int Current_Val = 0;
	float Ave_Value[4];
	float Ave_Value_Inner[4];
	
	int i = 0;
	int j = 0;

	for(  i = 0; i < BlockHeight; i++ )
	{
		for( j = 0; j < BlockWidth; j++ )
		{
			Current_Val = ImgBuf[i*ImageWidth+j];
			Sum+=Current_Val;
		}
	}

	Ave_Value[0] = Sum*1.0/(BlockWidth*BlockHeight);

	Sum = 0;

	for(  i = 0; i < BlockHeight; i++ )
	{
		for( j = 0; j < BlockWidth; j++ )
		{
			Current_Val = ImgBuf[(ImageHeight-1-i)*ImageWidth+j];
			Sum+=Current_Val;
		}
	}

	Ave_Value[1] = Sum*1.0/(BlockWidth*BlockHeight);

	Sum = 0;

	for(  i = 0; i < BlockHeight; i++ )
	{
		for( j = 0; j < BlockWidth; j++ )
		{
			Current_Val = ImgBuf[(ImageHeight-1-i)*ImageWidth+(ImageWidth-1-j)];
			Sum+=Current_Val;
		}
	}

	Ave_Value[2] = Sum*1.0/(BlockWidth*BlockHeight);

	Sum = 0;

	for(  i = 0; i < BlockHeight; i++ )
	{
		for( j = 0; j < BlockWidth; j++ )
		{
			Current_Val = ImgBuf[i*ImageWidth+(ImageWidth-1-j)];
			Sum+=Current_Val;
		}
	}
	
	Ave_Value[3] = Sum*1.0/(BlockWidth*BlockHeight);

	Sum = 0;
 
	for(  i = 0; i < BlockHeight_Inner; i++ )
	{
		for( j = 0; j < BlockWidth_Inner; j++ )
		{
			Current_Val = ImgBuf[i*ImageWidth+j];
			Sum+=Current_Val;
		}
	}
	
	Ave_Value_Inner[0] = Sum*1.0/(BlockWidth_Inner*BlockHeight_Inner);

	Sum = 0;
	
	for(  i = 0; i < BlockHeight_Inner; i++ )
	{
		for( j = 0; j < BlockWidth_Inner; j++ )
		{
			Current_Val = ImgBuf[(ImageHeight-1-i)*ImageWidth+j];
			Sum+=Current_Val;
		}
	}
	
	Ave_Value_Inner[1] = Sum*1.0/(BlockWidth_Inner*BlockHeight_Inner);

	Sum = 0;
	
	for(  i = 0; i < BlockHeight_Inner; i++ )
	{
		for( j = 0; j < BlockWidth_Inner; j++ )
		{
			Current_Val = ImgBuf[(ImageHeight-1-i)*ImageWidth+(ImageWidth-1-j)];
			Sum+=Current_Val;
		}
	}
	
	Ave_Value_Inner[2] = Sum*1.0/(BlockWidth_Inner*BlockHeight_Inner);

	Sum = 0;	
	
	for(  i = 0; i < BlockHeight_Inner; i++ )
	{
		for( j = 0; j < BlockWidth_Inner; j++ )
		{
			Current_Val = ImgBuf[i*ImageWidth+(ImageWidth-1-j)];
			Sum+=Current_Val;
		}
	}
	
	Ave_Value_Inner[3] = Sum*1.0/(BlockWidth_Inner*BlockHeight_Inner);

	Sum  = 0; 

	float  Min = 255;
	float  Max = 0; 

	for( i = 0; i < 4; i++)
	{
		if( Ave_Value[i] > Max) 
		{
			Max = Ave_Value[i];
		}

		if( Ave_Value[i] < Min )
		{
			Min = Ave_Value[i];
			index_Min = i;
		}
	}

	Range = Max - Min;

	float Max_Diff = 0;

	FILE *file_out;
	
	if(_Log)
	{
		file_out = fopen("blemish.txt", "a+");
	}  

	for( i = 0; i < 4; i++ )
	{
		float temp = (Ave_Value[i] - Ave_Value_Inner[i]);

		if(_Log && NULL != file_out)
			fprintf(file_out, "Inner %f, Outer %f\n", Ave_Value_Inner[i], Ave_Value[i]);  

		if( temp > Max_Diff )
		{
			Max_Diff = temp;


			index = i;
		}
	} 
		 

	if(_Log && NULL != file_out)
	{
		fprintf(file_out, "MaxDiff %f\n", Max_Diff); 
		fclose(file_out);
	}

	return Max_Diff; 
}


void ShrinkImgAny( BYTE *SrcImg, BYTE * DstImg, int SrcImgWidth, int SrcImgHeight, int DstImgWidth, int DstImgHeight )
{
	int HScal = SrcImgWidth / DstImgWidth;
	int VScal = SrcImgHeight / DstImgHeight;

	int Sum;
	int Size;
	int Cnt = 0; 
	
	for( int j = 0; j <= SrcImgHeight - VScal; j = j + VScal )
	{
		for( int i = 0; i <= SrcImgWidth - HScal; i = i + HScal )
		{
			Sum = 0;
			Size = 0;

			int iStart = 0;
			int jStart = 0;

			if( i + HScal >= SrcImgWidth )
			{
				iStart = SrcImgWidth-HScal;
			}
			else
			{
				iStart = i;
			}

			if( j + VScal >= SrcImgHeight )
			{
				jStart = SrcImgHeight - VScal;
			}
			else
			{
				jStart = j;
			}
			
			for( int l = 0; l < VScal; l++)
			{ 
				for( int k = 0;  k < HScal; k++ )
				{ 
					Sum += SrcImg[(jStart+l)*SrcImgWidth+iStart+k];
					Size++;
				}
			}

			DstImg[Cnt++] = int(Sum/Size);
		}
	} 
}

void SubUpOne( BYTE* SrcBuf, BYTE *DstBuf, int ImgWidth, int ImgHeigh, BYTE Spc )
{

	for( int j =0; j < ImgWidth; j++ )
	{
		DstBuf[j] = 0;
	}
 
	for( int i = 1; i < ImgHeigh; i++ )
	{
		for( int j = 0; j < ImgWidth; j++ )
		{ 
			DstBuf[i*ImgWidth+j] = abs(SrcBuf[i*ImgWidth+j] - SrcBuf[(i-1)*ImgWidth+j]);
				
			if( DstBuf[i*ImgWidth+j] < Spc )
			{
				DstBuf[i*ImgWidth+j] = 0;
			} 			 
		}
	}
}

void SubPreviewOne( BYTE* SrcBuf, BYTE *DstBuf, int ImgWidth, int ImgHeigh, BYTE Spc )
{
	for( int i = 0; i < ImgHeigh; i++ )
	{
		for( int j = 0; j < ImgWidth; j++ )
		{
			if( j == 0)
			{
				DstBuf[i*ImgWidth+j] = 0;
			}
			else
			{
				DstBuf[i*ImgWidth+j] = abs(SrcBuf[i*ImgWidth+j] - SrcBuf[i*ImgWidth+j-1]);

				if( DstBuf[i*ImgWidth+j] < Spc )
				{
				   DstBuf[i*ImgWidth+j] = 0;
				} 
			}
		}
	}
}

void MergeImage( BYTE* SrcBuf1, BYTE* SrcBuf2, int ImgWidth, int ImgHeight )
{
	for( int i = 0; i < ImgWidth; i++)
	{
		for( int j = 0; j < ImgHeight; j++ )
		{
			if( SrcBuf1[j*ImgWidth+i] < SrcBuf2[j*ImgWidth+i] )
			{
				SrcBuf1[j*ImgWidth+i] = SrcBuf2[j*ImgWidth+i];
			} 
		}
	} 
}

int fnBlemishAlgorithmInner(BYTE *SrcBMPBuffer, int iWidth, int iHeight,  POGRes *POGInfo, BlemishInfo *BlemishRes, int XStart, int XEnd, int YStart, int YEnd, int offset,
							int intensityCntSpc, float CornerDiff, float Range_Spec, int EdgeLimit, int EdgePixelLimit )
{
	ImageSize SrcImageSize;
	ImageSize ProcImgSize;
	SrcImageSize.Width		= iWidth;
	SrcImageSize.Height		= iHeight;
	ProcImgSize.Width		= ShrinkWidthInt;
	ProcImgSize.Height		= ShrinkHightInt;

	int MedianA				= POGInfo->MedianA;
	int MedianB				= POGInfo->MedianB;
	int Threshold			= POGInfo->Threshold;//10
	int _NormalPixelLimit   = POGInfo->_NormalPixelLimit;
	BOOL _Log				= POGInfo->_Log;
	BOOL _LoadRawFile		= POGInfo->_LoadRawFile;
	BOOL _LoadBmpFile		= POGInfo->_LoadBmpFile;
	BOOL _ImageSave			= POGInfo->_ImageSave;
	 
	BYTE *OriginImage	= (BYTE *)malloc(sizeof(BYTE)*(iWidth*iHeight));
	BYTE *BMPBuffer		= (BYTE *)malloc(sizeof(BYTE)*(iWidth*iHeight)*3);
	BYTE *MeanBMPBuffer = (BYTE *)malloc(sizeof(BYTE)*(ShrinkWidthInt*ShrinkHightInt)*3);
	BYTE *BmpFlipBuffer	= (BYTE *)malloc(sizeof(BYTE)*(iWidth*iHeight)*3);
	BYTE *SharkImage	= (BYTE *)malloc(sizeof(BYTE)*(ShrinkWidthInt*ShrinkHightInt));
	BYTE *Temp1Image	= (BYTE *)malloc(sizeof(BYTE)*(ShrinkWidthInt*ShrinkHightInt));
	BYTE *Temp2Image	= (BYTE *)malloc(sizeof(BYTE)*(ShrinkWidthInt*ShrinkHightInt));
	BYTE *Temp3Image	= (BYTE *)malloc(sizeof(BYTE)*(ShrinkWidthInt*ShrinkHightInt)); 
	BYTE * Temp4Image = (BYTE *)malloc(sizeof(BYTE)*(400*300));
	BYTE * Temp5Image = (BYTE *)malloc(sizeof(BYTE)*(400*300));

	if (NULL == OriginImage || NULL == BMPBuffer || NULL == MeanBMPBuffer || NULL == BmpFlipBuffer || NULL == SharkImage ||
		NULL == Temp1Image || NULL == Temp2Image || NULL == Temp3Image || NULL == Temp4Image || NULL == Temp5Image )
	{	
		if(OriginImage != NULL)   free (OriginImage);
		if(BMPBuffer != NULL)     free(BMPBuffer);
		if(MeanBMPBuffer != NULL) free(MeanBMPBuffer);
		if(BmpFlipBuffer != NULL) free(BmpFlipBuffer);
		if(SharkImage != NULL)    free(SharkImage);
		if(Temp1Image != NULL)    free(Temp1Image);
		if(Temp2Image != NULL)    free(Temp2Image);
		if(Temp3Image != NULL)    free(Temp3Image);
		if(Temp4Image != NULL)    free(Temp4Image);
	    if(Temp5Image != NULL)    free(Temp5Image);
		return 0;
	}

	double  ScaleH; // = (double)SrcImageSize.Width/ShrinkWidthFloat;
    double  ScaleV; // = (double)SrcImageSize.Height/ShrinkHightFloat;
 	
	// 增强对比度
	ImagePreprocess(&SrcBMPBuffer[0],&OriginImage[0],SrcImageSize);

	if(_ImageSave)
	{
		memset(BMPBuffer,0,SrcImageSize.Width*SrcImageSize.Height*3);
		BayerConversionFlip(OriginImage,BMPBuffer,SrcImageSize.Width,SrcImageSize.Height,3);
		bSaveBmpFile("D:\\BlemishDbgImg\\01.增强对比度.bmp",BMPBuffer,SrcImageSize.Width,SrcImageSize.Height);
	}
	
	//图片压缩为400*300
	ImageShrink(&OriginImage[0],&SharkImage[0],SrcImageSize); 
	ShrinkImgAny( SharkImage,Temp4Image, ShrinkWidthInt, ShrinkHightInt, 200, 150 ); 
	SubPreviewOne( Temp4Image, Temp5Image, 200,150, EdgeLimit );
	SubUpOne( Temp4Image, Temp3Image, 200,150, EdgeLimit );
	MergeImage( Temp5Image,Temp3Image, 200,150 );

	if(_ImageSave)
	{
		memset(MeanBMPBuffer,0,200*150*1);
		bSaveRawFile("D:\\BlemishDbgImg\\011.进一步压缩.raw", Temp5Image, 200*150);

		memset(BMPBuffer,0,200*150*3);
		BayerConversionFlip(Temp5Image,BMPBuffer,200,150,3);
		bSaveBmpFile("D:\\BlemishDbgImg\\011.进一步压缩.bmp",BMPBuffer,200,150);
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	Result.POGamount = 0;
	ImageSize tempImgSize;
	tempImgSize.Width = 200;
	tempImgSize.Height = 150;

	ScaleH = (double )SrcImageSize.Width/tempImgSize.Width;
    ScaleV = (double)SrcImageSize.Height/tempImgSize.Height;  

	int ConnectedObjectFilterStatus = ConnectedObjectFilter(&Temp5Image[0], &Temp3Image[0], tempImgSize, EdgePixelLimit,  _Log);
	if(ConnectedObjectFilterStatus == -1)
	{
		free (OriginImage);
		free(BMPBuffer);
		free(MeanBMPBuffer);
		free(BmpFlipBuffer);
		free(SharkImage);
		free(Temp1Image);
		free(Temp2Image);
		free(Temp3Image);
		free(Temp4Image);
		free(Temp5Image);
		
		return 0;
	}
    /////////////////////////////////////////////////////////////////////////////////////////////
	if( ConnectedObjectFilterStatus )
	{
		ScaleH = (double )SrcImageSize.Width/ShrinkWidthFloat;
	    ScaleV = (double)SrcImageSize.Height/ShrinkHightFloat;  

		int Index =0;
		int Index_Min = 0;
		float Range = 0;	

		float Max_Differ = CornerCheck(SharkImage, 400, 300, Index, Range, Index_Min, _Log ); 

		if( Range > Range_Spec )
		{
			switch (Index_Min)
			{

			case 0:
				
				BlemishRes->xStart[0]			  = int(1*ScaleH);
				BlemishRes->xEnd[0]				  = int(20*ScaleH);
				BlemishRes->yStart[0]			  = int(1*ScaleV);
				BlemishRes->yEnd[0]				  = int(20*ScaleV);
				BlemishRes->PixelCount[0]		  = 100;
				BlemishRes->Intensity[0]   =     10; 
				
				break;
				
			case 1:
				BlemishRes->xStart[0]			  = int(1*ScaleH);
				BlemishRes->xEnd[0]				  = int(20*ScaleH);
				BlemishRes->yStart[0]			  = int(280*ScaleV);
				BlemishRes->yEnd[0]				  = int(299*ScaleV);
				BlemishRes->PixelCount[0]		  = 100;
				BlemishRes->Intensity[0]   =     10; 

				break;

			case 2:
				BlemishRes->xStart[0]			  = int(380*ScaleH);
				BlemishRes->xEnd[0]				  = int(399*ScaleH);
				BlemishRes->yStart[0]			  = int(280*ScaleV);
				BlemishRes->yEnd[0]				  = int(299*ScaleV);
				BlemishRes->PixelCount[0]		  = 100;
				BlemishRes->Intensity[0]   =     10; 
				break;
				
			case 3:
				BlemishRes->xStart[0]			  = int(380*ScaleH);
				BlemishRes->xEnd[0]				  = int(399*ScaleH);
				BlemishRes->yStart[0]			  = int(1*ScaleV);
				BlemishRes->yEnd[0]				  = int(20*ScaleV);
				BlemishRes->PixelCount[0]		  = 100;
				BlemishRes->Intensity[0]   =     10; 
				break; 
			}

			
			BlemishRes->_MaxBlemish.xStart = BlemishRes->xStart[0];
			BlemishRes->_MaxBlemish.yStart = BlemishRes->yStart[0];
			BlemishRes->_MaxBlemish.xEnd = BlemishRes->xEnd[0];
			BlemishRes->_MaxBlemish.yEnd = BlemishRes->yEnd[0];
			BlemishRes->_MaxBlemish.Intensity = BlemishRes->Intensity[0];
			BlemishRes->_MaxBlemish.MaxPixelCount = BlemishRes->PixelCount[0];
			
			BlemishRes->POGCount=1;

			free (OriginImage);
			free(BMPBuffer);
			free(MeanBMPBuffer);
			free(BmpFlipBuffer);
			free(SharkImage);
			free(Temp1Image);
			free(Temp2Image);
			free(Temp3Image);
			free(Temp4Image);
	        free(Temp5Image);

			return 0;

		} 
		else if( Max_Differ > CornerDiff )
		{
			switch (Index)
			{
			case 0:
				
				BlemishRes->xStart[0]			  = int(1*ScaleH);
				BlemishRes->xEnd[0]				  = int(20*ScaleH);
				BlemishRes->yStart[0]			  = int(1*ScaleV);
				BlemishRes->yEnd[0]				  = int(20*ScaleV);
				BlemishRes->PixelCount[0]		  = 100;
				BlemishRes->Intensity[0]   =     10; 
			
				break;

			case 1:
				BlemishRes->xStart[0]			  = int(1*ScaleH);
				BlemishRes->xEnd[0]				  = int(20*ScaleH);
				BlemishRes->yStart[0]			  = int(280*ScaleV);
				BlemishRes->yEnd[0]				  = int(299*ScaleV);
				BlemishRes->PixelCount[0]		  = 100;
				BlemishRes->Intensity[0]   =     10; 
				break;
			case 2:
				BlemishRes->xStart[0]			  = int(380*ScaleH);
				BlemishRes->xEnd[0]				  = int(399*ScaleH);
				BlemishRes->yStart[0]			  = int(280*ScaleV);
				BlemishRes->yEnd[0]				  = int(299*ScaleV);
				BlemishRes->PixelCount[0]		  = 100;
				BlemishRes->Intensity[0]   =     10; 
				break;

			case 3:
				BlemishRes->xStart[0]			  = int(380*ScaleH);
				BlemishRes->xEnd[0]				  = int(399*ScaleH);
				BlemishRes->yStart[0]			  = int(1*ScaleV);
				BlemishRes->yEnd[0]				  = int(20*ScaleV);
				BlemishRes->PixelCount[0]		  = 100;
				BlemishRes->Intensity[0]   =     10; 
				break; 
			}

			BlemishRes->_MaxBlemish.xStart = BlemishRes->xStart[0];
			BlemishRes->_MaxBlemish.yStart = BlemishRes->yStart[0];
			BlemishRes->_MaxBlemish.xEnd = BlemishRes->xEnd[0];
			BlemishRes->_MaxBlemish.yEnd = BlemishRes->yEnd[0];
			BlemishRes->_MaxBlemish.Intensity = BlemishRes->Intensity[0];
			BlemishRes->_MaxBlemish.MaxPixelCount = BlemishRes->PixelCount[0];
			
			BlemishRes->POGCount=1;

			free (OriginImage);
			free(BMPBuffer);
			free(MeanBMPBuffer);
			free(BmpFlipBuffer);
			free(SharkImage);
			free(Temp1Image);
			free(Temp2Image);
			free(Temp3Image);
			free(Temp4Image);
	        free(Temp5Image);

			return 0;
		} 

		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
			BayerConversionFlip(SharkImage,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
			bSaveBmpFile("D:\\BlemishDbgImg\\02.图片压缩.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		}

		//中值滤波A
		FastMedianFilter(&SharkImage[0], &Temp1Image[0], ProcImgSize, MedianA);

		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
			bSaveRawFile("D:\\BlemishDbgImg\\203.中值滤波A.raw", Temp1Image, ShrinkWidthInt*ShrinkHightInt);
			BayerConversionFlip(Temp1Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
			bSaveBmpFile("D:\\BlemishDbgImg\\03.中值滤波A.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		}

		//中值滤波B
		FastMedianFilter(&SharkImage[0], &Temp2Image[0], ProcImgSize, MedianB);

		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
			bSaveRawFile("D:\\BlemishDbgImg\\204.中值滤波B.raw", Temp2Image, ShrinkWidthInt*ShrinkHightInt);
			BayerConversionFlip(Temp2Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
			bSaveBmpFile("D:\\BlemishDbgImg\\04.中值滤波B.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		}

		//中值滤波A 与 B 作差
		ImageMath(&Temp1Image[0], &Temp2Image[0], &Temp3Image[0], ProcImgSize, Sub, XStart,XEnd,YStart, YEnd); 

		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
      		bSaveRawFile("D:\\BlemishDbgImg\\205.中值滤波AB差.raw", Temp3Image, ShrinkWidthInt*ShrinkHightInt);
			BayerConversionFlip(Temp3Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
			bSaveBmpFile("D:\\BlemishDbgImg\\05.中值滤波AB差.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		}

		
		BYTE ImageMathPostStatus = ImageMathPost( &Temp3Image[0], &Temp2Image[0], ProcImgSize, XStart,XEnd,YStart, YEnd, intensityCntSpc);
        if(ImageMathPostStatus == -1)
		{
			free (OriginImage);
			free(BMPBuffer);
			free(MeanBMPBuffer);
			free(BmpFlipBuffer);
			free(SharkImage);
			free(Temp1Image);
			free(Temp2Image);
			free(Temp3Image);
			free(Temp4Image);
			free(Temp5Image);
			
			return 0;
		}
		
		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
			bSaveRawFile("D:\\BlemishDbgImg\\2052.后处理.raw", Temp2Image, ShrinkWidthInt*ShrinkHightInt);
			BayerConversionFlip(Temp3Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
			bSaveBmpFile("D:\\BlemishDbgImg\\052.后处理.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		}


		int  A_x,A_y,B_x,B_y;
		A_x = 0;
		A_y = 0;
		B_x = 0;
		B_y = 0;

		int i,j;

		for(  i = 0; i < ShrinkHightInt; i++ )
		{
			for(  j = 0; j < ShrinkWidthInt; j++ )
			{
				if( Temp2Image[i*ShrinkWidthInt+j] == 255 )
				{
					A_x = j; 
					A_y = i;
					i = ShrinkHightInt;
					break;
				}
			}
		}

		for(   i = 0; i < ShrinkHightInt; i++ )
		{
			for(   j = 0; j < ShrinkWidthInt; j++ )
			{
				if( Temp2Image[i*ShrinkWidthInt+j] == 1 )
				{
					B_x = j; 
					B_y = i;
					i = ShrinkHightInt;
					break;
				}
			}
		}  

		//影像滤波
		PixelMath(&Temp2Image[0], 127, &Temp2Image[0], ProcImgSize, Add);

		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
			bSaveRawFile("D:\\BlemishDbgImg\\206.影像滤波.raw", Temp2Image, ShrinkWidthInt*ShrinkHightInt);
			BayerConversionFlip(Temp3Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
			bSaveBmpFile("D:\\BlemishDbgImg\\06.影像滤波.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		}
		
		//Remove Conner 去死角
		RemoveCorner(&Temp2Image[0], ProcImgSize, MedianA, 127, offset);

		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
			bSaveRawFile("D:\\BlemishDbgImg\\207.去死角.raw", Temp2Image, ShrinkWidthInt*ShrinkHightInt);
			BayerConversionFlip(Temp3Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
			bSaveBmpFile("D:\\BlemishDbgImg\\07.去死角.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		}
		
		//增强对比度
		ContrastEnhance(&Temp2Image[0],&Temp1Image[0],ProcImgSize);
		
		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
			bSaveRawFile("D:\\BlemishDbgImg\\208.增强对比度.raw", Temp1Image, ShrinkWidthInt*ShrinkHightInt);
			BayerConversionFlip(Temp1Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
			bSaveBmpFile("D:\\BlemishDbgImg\\08.增强对比度.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		} 
		
		int temp = Temp1Image[B_y*ShrinkWidthInt+B_x ] - Temp1Image[A_y*ShrinkWidthInt+A_x]-1;
		int avg = Temp1Image[A_y*ShrinkWidthInt+A_x];

		if( temp > 	Threshold)
		{
			Threshold = temp;
		}

		//根据阀值二值化
		ThresholdPOG(&Temp1Image[0], &Temp2Image[0], ProcImgSize, Threshold,avg);

		if(_ImageSave)
		{
			memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
			bSaveRawFile("D:\\BlemishDbgImg\\209.二值化.raw", Temp2Image, ShrinkWidthInt*ShrinkHightInt);
			BayerConversionFlip(Temp2Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3);	
	 		bSaveBmpFile("D:\\BlemishDbgImg\\09.二值化.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		} 

		Result.POGamount = 0;
		ConnectedObjectFilterStatus = ConnectedObjectFilter(&Temp2Image[0], &Temp3Image[0], ProcImgSize, _NormalPixelLimit,  _Log);

		if(ConnectedObjectFilterStatus == -1)
		{
			free (OriginImage);
			free(BMPBuffer);
			free(MeanBMPBuffer);
			free(BmpFlipBuffer);
			free(SharkImage);
			free(Temp1Image);
			free(Temp2Image);
			free(Temp3Image);
			free(Temp4Image);
			free(Temp5Image);
			
			return 0;
	}
}

		BlemishRes->POGCount = Result.POGamount;
		int POGCount = Result.POGamount;
		int TickCount = 0;
		BlemishRes->_MaxBlemish.MaxPixelCount = 0;
 
		if( POGCount != 0 )
		{
			while (( Result.POG != NULL ) && (POGCount>0) && (TickCount < 260) )
			{
				BlemishRes->xStart[TickCount]			  = int(Result.POG[0].StartPoint.X*ScaleH);
				BlemishRes->xEnd[TickCount]				  = int(Result.POG[0].EndPoint.X*ScaleH);
				BlemishRes->yStart[TickCount]			  = int(Result.POG[0].StartPoint.Y*ScaleV);
				BlemishRes->yEnd[TickCount]				  = int(Result.POG[0].EndPoint.Y*ScaleV);
				BlemishRes->PixelCount[TickCount]		  = Result.POG[0].PixelCount;
		 		BlemishRes->Intensity[TickCount]		  = Result.POG[0].Intensity;
				
				Result.POG = Result.POG[0].Next;

				if(BlemishRes->PixelCount[TickCount] > BlemishRes->_MaxBlemish.MaxPixelCount)
				{
					BlemishRes->_MaxBlemish.MaxPixelCount = BlemishRes->PixelCount[TickCount];
					BlemishRes->_MaxBlemish.xStart        = BlemishRes->xStart[TickCount];
					BlemishRes->_MaxBlemish.xEnd		  = BlemishRes->xEnd[TickCount];
					BlemishRes->_MaxBlemish.yStart		  = BlemishRes->yStart[TickCount];
					BlemishRes->_MaxBlemish.yEnd          = BlemishRes->yEnd[TickCount];
		    		BlemishRes->_MaxBlemish.Intensity     = BlemishRes->Intensity[TickCount];
				}

				POGCount--;
				TickCount++;
			}
		}

		if(_ImageSave)
		{
	    	memset(MeanBMPBuffer,0,ShrinkWidthInt*ShrinkHightInt*3);
	    	bSaveRawFile("D:\\BlemishDbgImg\\210.POG.raw",Temp3Image, ShrinkWidthInt*ShrinkHightInt);
	    	BayerConversionFlip(Temp3Image,MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt,3 );	
	    	bSaveBmpFile("D:\\BlemishDbgImg\\10.POG.bmp",MeanBMPBuffer,ShrinkWidthInt,ShrinkHightInt);
		} 

		free (OriginImage);
		free(BMPBuffer);
		free(MeanBMPBuffer);
		free(BmpFlipBuffer);
		free(SharkImage);
		free(Temp1Image);
		free(Temp2Image);
		free(Temp3Image);
		free(Temp4Image);
		free(Temp5Image);
 
		return ConnectedObjectFilterStatus;
}

int fnBlemishAlgorithm(BYTE *SrcBMPBuffer,int iWidth, int iHeight, BlemishInfo *BlemishRes ,CamSeletForPOG Cam)
{
	if(NULL == SrcBMPBuffer || NULL == BlemishRes) return 0;

	char m_szSetup[512]={0};
	GetModulePath(m_szSetup);
	strcat(m_szSetup,"OfblemishTest.ini");
	
	BOOL _ImageSave=GetPrivateProfileInt("Spec","bSaveImage",0,m_szSetup);
	
	BOOL _Log=GetPrivateProfileInt("Spec","bSaveLog",0,m_szSetup);
	
	char Temp[512]={0};
	int MedianA;
	int MedianB;
	int Threshold;
	int _NormalPixelLimit;
	int  XStart;
	int XEnd;
	int YStart;
	int YEnd;
	int offset;
	int intensityCntSpc;
	int CornerDiff_I;
	float CornerDiff_F;
	int RangeSpc_I;
	float RangeSpc_F;
	int EdgeLimit;
	int EdgePixelLimit;

	if (Cam == POG_Cam0)
	{
		GetPrivateProfileString("Spec_Cam0","MedianA","25",Temp,512,m_szSetup);
		MedianA	=atoi(Temp);
		
		GetPrivateProfileString("Spec_Cam0","MedianB","5",Temp,512,m_szSetup);
		MedianB=atoi(Temp);
		
		GetPrivateProfileString("Spec_Cam0","Threshold","15",Temp,512,m_szSetup);
		Threshold=atoi(Temp);
		
		GetPrivateProfileString("Spec_Cam0","PixelLimit","50",Temp,512,m_szSetup);
		_NormalPixelLimit=atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam0","ZoneAXStart","0",Temp,512,m_szSetup);
		XStart =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam0","ZoneAXEnd","400",Temp,512,m_szSetup);
		XEnd =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam0","ZoneAYStart","0",Temp,512,m_szSetup);
		YStart =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam0","ZoneAYEnd","300",Temp,512,m_szSetup);
		YEnd = atoi(Temp);  
		
		GetPrivateProfileString("Spec_Cam0","offset","7",Temp,512,m_szSetup);
		offset =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam0","IntensityCntSpc","7",Temp,512,m_szSetup);
		intensityCntSpc = atoi(Temp); 
		////////////////////////////////////////////////
		
		GetPrivateProfileString("Spec_Cam0","CornerDiff","70",Temp,512,m_szSetup);
		CornerDiff_I =atoi(Temp); 
		CornerDiff_F = CornerDiff_I*1.0/10;
		
		GetPrivateProfileString("Spec_Cam0","RangeSpc","70",Temp,512,m_szSetup);
		RangeSpc_I = atoi(Temp); 
		RangeSpc_F = RangeSpc_I*1.0/10;
		
		GetPrivateProfileString("Spec_Cam0","EdgeLimit","4",Temp,512,m_szSetup);
		EdgeLimit =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam0","EdgePixelLimit","9",Temp,512,m_szSetup);
    	EdgePixelLimit = atoi(Temp); 
	}
	
	if (Cam == POG_Cam1)
	{
		GetPrivateProfileString("Spec_Cam1","MedianA","25",Temp,512,m_szSetup);
		MedianA	=atoi(Temp);
		
		GetPrivateProfileString("Spec_Cam1","MedianB","5",Temp,512,m_szSetup);
		MedianB=atoi(Temp);
		
		GetPrivateProfileString("Spec_Cam1","Threshold","15",Temp,512,m_szSetup);
		Threshold=atoi(Temp);
		
		GetPrivateProfileString("Spec_Cam1","PixelLimit","50",Temp,512,m_szSetup);
		_NormalPixelLimit=atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam1","ZoneAXStart","0",Temp,512,m_szSetup);
		XStart =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam1","ZoneAXEnd","400",Temp,512,m_szSetup);
		XEnd =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam1","ZoneAYStart","0",Temp,512,m_szSetup);
		YStart =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam1","ZoneAYEnd","300",Temp,512,m_szSetup);
		YEnd = atoi(Temp);  
		
		GetPrivateProfileString("Spec_Cam1","offset","7",Temp,512,m_szSetup);
		offset =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam1","IntensityCntSpc","7",Temp,512,m_szSetup);
		intensityCntSpc = atoi(Temp); 
		////////////////////////////////////////////////
		
		GetPrivateProfileString("Spec_Cam1","CornerDiff","70",Temp,512,m_szSetup);
		CornerDiff_I =atoi(Temp); 
		CornerDiff_F = CornerDiff_I*1.0/10;
		
		GetPrivateProfileString("Spec_Cam1","RangeSpc","70",Temp,512,m_szSetup);
		RangeSpc_I = atoi(Temp); 
		RangeSpc_F = RangeSpc_I*1.0/10;
		
		GetPrivateProfileString("Spec_Cam1","EdgeLimit","4",Temp,512,m_szSetup);
		EdgeLimit =atoi(Temp); 
		
		GetPrivateProfileString("Spec_Cam1","EdgePixelLimit","9",Temp,512,m_szSetup);
		EdgePixelLimit = atoi(Temp); 
	}

	////////////////////////////////////////////////////////////////////////
	
	if (_Log)
	{
		DebugInfo("Spec_bSaveImage=%d",_ImageSave);
		DebugInfo("Spec_MedianA=%d",MedianA);
		DebugInfo("Spec_MedianB=%d",MedianB);
		DebugInfo("Spec_Threshold=%d",Threshold);
		DebugInfo("Spec_NormalPixelLimit=%d",_NormalPixelLimit);
		
		DebugInfo("Spec_AXStart=%d",XStart);
		DebugInfo("Spec_AXEnd=%d",XEnd);
		DebugInfo("Spec_AYStart=%d",YStart);
		DebugInfo("Spec_AYEnd =%d",YEnd);
		DebugInfo("Spec_offset=%d",offset);
		DebugInfo("IntensityCntSpc=%d",intensityCntSpc); 

		DebugInfo("CornerDiff = %d", CornerDiff_I);	
		DebugInfo("RangeSpc = %d", RangeSpc_I);
	} 
	
	POGRes *POGInfo = new POGRes[200]; 
	if(NULL == POGInfo) return FALSE;
	
	POGInfo->MedianA = MedianA;
	POGInfo->MedianB = MedianB;
	POGInfo->Threshold =  Threshold;
	POGInfo->_NormalPixelLimit =  _NormalPixelLimit;
	POGInfo->_Log =  _Log;
	POGInfo->_ImageSave = _ImageSave; 
 
	
	int BlemishResult =  fnBlemishAlgorithmInner( SrcBMPBuffer,iWidth,iHeight,POGInfo, BlemishRes, XStart, XEnd, YStart, YEnd, offset, 
		intensityCntSpc, CornerDiff_F, RangeSpc_F, EdgeLimit, EdgePixelLimit );
	
	int trytime = 0;
	
	while( (BlemishRes->POGCount >= 7) && (trytime <= 20) )
	{
		POGInfo->Threshold += 1;
		trytime++;
		BlemishResult = fnBlemishAlgorithmInner( SrcBMPBuffer,iWidth,iHeight,POGInfo, BlemishRes, XStart, XEnd, YStart, YEnd, offset, intensityCntSpc, CornerDiff_F, RangeSpc_F,EdgeLimit, EdgePixelLimit );
	}
	
	delete [] POGInfo; 
	return BlemishResult;	
} 

 
void ShowBlemishSpecDlg(HWND hwnd, BOOL bShowFlag )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CWnd *pwnd=CWnd::FromHandle (hwnd);

	if( bShowFlag )
	{
		if(SpecDlg.m_hWnd)
		{
			SpecDlg.ShowWindow(SW_SHOW);
			return;
		}	
	}
	else
	{
		if(SpecDlg.m_hWnd)
		{
			SpecDlg.ShowWindow(SW_HIDE);
			return;
		}
	}
	
	if (hwnd==0)
	{
		SpecDlg.Create(IDD_BLEMISH_DLG,CWnd::FromHandle(GetDesktopWindow()));
		SpecDlg.ModifyStyle(WS_CHILD,DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU);
		SpecDlg.SetWindowText("Blemish_Spec_Setting");
		SpecDlg.CenterWindow(CWnd::FromHandle(GetDesktopWindow() ) );
	}	
	else
	{
		SpecDlg.Create(IDD_BLEMISH_DLG,pwnd);
		RECT rt;
		pwnd->GetClientRect(&rt);
		SpecDlg.MoveWindow(&rt);
		
	}

	if( bShowFlag )
	{
		SpecDlg.ShowWindow(SW_SHOW);
	}
	else
	{
		SpecDlg.ShowWindow(SW_HIDE); 
	}
	
	 
	
	return;
}