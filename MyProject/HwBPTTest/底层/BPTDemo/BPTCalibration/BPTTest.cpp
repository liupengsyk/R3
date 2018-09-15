#include "stdafx.h"

#define BPT_EXPORTS

#include "BPTTest.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>




#define SAFE_RELEASE(a)  if(a!=NULL){delete a; a=NULL;}

enum COLOR_CHANNEL
{
	CHANNEL_R = 0,
	CHANNEL_GR,
	CHANNEL_GB,
	CHANNEL_B,
	CHANNEL_COUNT
};

enum
{
	OUTFORMAT_RGGB = 0,
		OUTFORMAT_GRBG = 1,
		OUTFORMAT_GBRG = 2,
		OUTFORMAT_BGGR = 3
};


CString GetFilePath()
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	char ModulePath[128] = {0};
	char path[128]={0};
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
	 fp=freopen(sfilename,"at",stdout);
	 fseek(fp,0,SEEK_END);
	 nfileSize=ftell(fp);		  
	 if(nfileSize<10) 
	 {
		 printf("Number,xIndex,yIndex,x,y,NeighborNum,Y_Value");
		 printf("\n");  
	 }
	 fclose(fp);
	 return 1;
}


float GetSurroundingAreaAve_UshortInput( USHORT* pPanel, int nWidth, int nHeight, int X, int Y, int KenalW ,int KenalH,int LastSum,int &NowSum,int &lastCnt)
{
	unsigned long sum = 0;
	int nCnt = 0;
	int i=0;
	int j=0;
	
	
	int radusw = (KenalW-1)/2;	//KenalW = 31 
	int radush=(KenalH-1)/2;
	
    if (LastSum!=0)
    {
		nCnt=lastCnt;
       	for( int i = Y-radush; i < Y + radush; i++  )
		{
			for( int j = (X-radusw-1); j <= ( X-radusw-1); j++ )	//X本身已经加1，所以求前一列的和时需要减去1
			{
				
				if( i < 0 || i >= nHeight || j < 0 || j>= nWidth )
					continue; 
				
				sum += pPanel[i*nWidth+j];
				nCnt--;
			}
		}
		sum=LastSum-sum;
		for(int  i1 = Y-radush; i1 < Y + radush; i1++  )		// X本身已经加1 
		{
			for(int   j1 = (X+radusw); j1 <= ( X+radusw); j1++ )
			{
				if( i1 < 0 || i1 >= nHeight || j1 < 0 || j1>= nWidth )
					continue; 
				
				sum += pPanel[i1*nWidth+j1];
				nCnt++;
			}
		}
		
    }
	else
	{
		for( int i = Y-radush; i <Y + radush; i++ )
		{
			for( int j = X-radusw; j <X+radusw; j++ )
			{
				if( i < 0 || i >= nHeight || j < 0 || j>= nWidth )
					continue; 
				
				sum += pPanel[i*nWidth+j];
				nCnt += 1;
			}
		}
	}
    NowSum=sum;
	lastCnt=nCnt;
	return (float)(sum*1.0/nCnt);
}

void PushBadIntoStack(PixelInfo* Stack, PixelInfo BadPix, int nCnt, int nImgWidth, int nImgHeight, int StackMaxSize)
{
	
	int tempIndex = BadPix.yIndex*nImgWidth + BadPix.xIndex;
	
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
	
	Stack[i] = BadPix;  
	nCnt++;   
}


void GetBadPixel_UshortInput(USHORT* pRaw10Info, int nWidth, int nHeight, OFPixInfoStack* InfoStack)
{
	int i=0;
	int j=0;
	int spec;
	
	float temp=0.0;
	int tempSUM=0;
	int NowSum=0;
    int LastCnt=0;
    int m = 0;
	InfoStack->nBadCnt=0;
	for (i=0; i<nHeight; i++)
	{
		for (j=0; j<nWidth; j++)
		{
			if (InfoStack->nBadCnt >= 2000)
			{
				break;
			}
			//brightness > MAX {1.2 * mean of 30*30 neighborhood, 20 + mean of 30*30 neighborhood} (10bits)
			temp = GetSurroundingAreaAve_UshortInput(pRaw10Info, nWidth, nHeight, j, i, 31, 31,tempSUM,NowSum,LastCnt);
			tempSUM=NowSum;				
			spec = (int)(max(1.2*temp ,20+temp)+0.5);
			if (pRaw10Info[i*nWidth+j]>spec)	//大于规格,值为1023
			{
				CString str;
				str.Format("周边平均值=%f,当前值：%d",temp,pRaw10Info[i*nWidth+j]);
				OutputDebugString(str);
				InfoStack->BadStk[m].x = j;
				InfoStack->BadStk[m].y = i;
				InfoStack->BadStk[m].Y_Value	= pRaw10Info[i*nWidth+j];
				InfoStack->BadStk[m].Average	= (USHORT)(temp+0.5);
				InfoStack->BadStk[m].NeighborNum = 0;
				m++;
				InfoStack->nBadCnt += 1;
			}
		}
		tempSUM = 0;
	}
}


/*********************************************************
函数名：GetClusterFromStack
返回值：1 计算成功 0 内存分配失败
*********************************************************/

int GetClusterFromStack(OFPixInfoStack* SingleBadStack, BadPixelTable *DfcStk )
{
	if (SingleBadStack->nBadCnt == 0)
	{
		return 1;
	}
	int badCount = SingleBadStack->nBadCnt;

	//相邻的八个位置，有坏点，则记录当前点为坏点
	for (int i = 0; i < badCount ; i++)
	{	
		if (DfcStk->DefectStackCnt >= 2000)
		{
			return 0;
		}
		for (int index = i + 1; index < badCount ; index++)
		{
			USHORT xDiff = abs(SingleBadStack->BadStk[i].x - SingleBadStack->BadStk[index].x);
			USHORT yDiff = abs(SingleBadStack->BadStk[i].y - SingleBadStack->BadStk[index].y);
			
			if (xDiff >= 2 && yDiff >= 2)
			{
				break;
			}

			if (xDiff <= 1 && yDiff <= 1)
			{
				SingleBadStack->BadStk[i].NeighborNum++;
				SingleBadStack->BadStk[index].NeighborNum++;
			}
		}
		DfcStk->BadPixelStack[DfcStk->DefectStackCnt++] = SingleBadStack->BadStk[i];
	}
	return 1;
}



BOOL GetChannelBuffer(BYTE* pRawImage,USHORT*pChannelImage,USHORT nImgW,USHORT nImgH,int nPixOrder,int iChannel)
{
	
	int iChannelHeight = nImgH/2;
	int iChannelWidth = nImgW/2;
	
	USHORT PixValue = 0;
	unsigned long iPixIndex  = 0;
	
	switch(nPixOrder)
	{
	case OUTFORMAT_RGGB:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = 2*y*nImgW + 2*x;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case  CHANNEL_GR:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = 2*y*nImgW + 2*x + 1;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case  CHANNEL_GB:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = (2*y + 1)*nImgW + 2* x;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = (2*y + 1)*nImgW + 2* x + 1;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			}
		}
		break;
	case  OUTFORMAT_GBRG:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = (2*y + 1)*nImgW + 2* x ;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case  CHANNEL_GR:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex =  (2*y + 1)*nImgW + 2* x + 1;;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case  CHANNEL_GB:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex =  2*y*nImgW + 2* x ;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex =  2*y *nImgW + 2* x + 1;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			}
		}
		break;
	case  OUTFORMAT_BGGR:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex =  (2*y + 1)*nImgW + 2* x + 1;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case  CHANNEL_GR:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = (2*y +1)*nImgW + 2* x ;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case  CHANNEL_GB:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex =   2*y *nImgW + 2* x + 1;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = 2*y *nImgW + 2* x ;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			}
		}
		break;
	case  OUTFORMAT_GRBG:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = 2*y *nImgW + 2* x + 1;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case  CHANNEL_GR:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = 2*y *nImgW + 2* x;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case  CHANNEL_GB:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = (2*y + 1) *nImgW + 2* x + 1;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int y = 0; y < iChannelHeight; y++)
					{
						for (int x = 0; x < iChannelWidth; x++)
						{
							iPixIndex = (2*y + 1)*nImgW + 2* x;
							PixValue = pRawImage[iPixIndex*2] + pRawImage[iPixIndex*2 + 1] * 256;
							pChannelImage[y*iChannelWidth + x] = PixValue;
						}
					}
				}
				break;
			}
		}
		break;
	}
	return TRUE;
}
void GetBadPixelPosition(OFPixInfoStack* pStack,  int nPixOrder, int iChannel)
{
	switch(nPixOrder)
	{
	case  OUTFORMAT_RGGB:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y;
					}
				}
				break;
			case CHANNEL_GR:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x + 1;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y;
					}
				}
				break;
			case CHANNEL_GB:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x ;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y + 1;
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x + 1;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y + 1;
					}
				}
				break;
			}
		}
		break;
	case  OUTFORMAT_BGGR:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x + 1;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y + 1;
					}
				}
				break;
			case CHANNEL_GR:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y + 1;
					}
				}
				break;
			case CHANNEL_GB:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x + 1;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y;
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y;
					}
				}
				break;
			}
		}
		break;
	case  OUTFORMAT_GBRG:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y + 1;
					}
				}
				break;
			case CHANNEL_GR:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x + 1;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y + 1;
					}
				}
				break;
			case CHANNEL_GB:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y;
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x + 1;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y;
					}
				}
				break;
			}
		}
		break;
	case OUTFORMAT_GRBG:
		{
			switch(iChannel)
			{
			case CHANNEL_R:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x + 1;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y;
					}
				}
				break;
			case CHANNEL_GR:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y;
					}
				}
				break;
			case CHANNEL_GB:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x + 1;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y + 1;
					}
				}
				break;
			case CHANNEL_B:
				{
					for (int iIndex = 0; iIndex < pStack->nBadCnt; iIndex++)
					{
						pStack->BadStk[iIndex].xIndex = 2* pStack->BadStk[iIndex].x;
						pStack->BadStk[iIndex].yIndex = 2* pStack->BadStk[iIndex].y + 1;
					}
				}
				break;
			}
		}
		break;
	}
}

void SorttBPTPixel(BadPixelTable* DfcStk,int left,int right)
{	
	PixelInfo temp;
	for (int i = left; i < right; i++)
	{
		for (int j = left; j < right; j++)
		{
			if (DfcStk->BadPixelStack[j].NeighborNum<DfcStk->BadPixelStack[j+1].NeighborNum)
			{
				temp = DfcStk->BadPixelStack[j+1];
				DfcStk->BadPixelStack[j+1] = DfcStk->BadPixelStack[j];
				DfcStk->BadPixelStack[j] = temp;
			}
		}
	}
}
/*
算法说明：
1.	Record the pixels whose brightness > MAX {1.2 * mean of 30*30 neighborhood, 20 + mean of 30*30 neighborhood} 
	(10bits) in RAW image. R, G, B channels are processed respectively.
2.	If the above bad pixels are 8- neighborhood connected in R channel (and G channel, and B channel, respectively) 
	domain, and then calculate the area of connected component. Only record the area which >1 pixel. 
3.	If the bad dots are more than 2000 PCS, then record the dots with bigger area as first priority.
*/
BOOL GetBPTTest( BYTE* pRawImage, int nImgWidth, int nImgHeight, int nPixOrder, BadPixelTable *DfcStk,int SaveData, int SaveImage)
{
	if (SaveImage)	//保存图片
	{
		bSaveRawFile("D:\\BPTTest\\pIn.raw",pRawImage,nImgWidth*nImgHeight*2);
	}
	DfcStk->DefectStackCnt = 0;
	USHORT* pChannelBuff = new USHORT[nImgHeight*nImgWidth/4];
	if(pChannelBuff==NULL)
	{
		OutputDebugString("内存分配失败");		
		return 0;
	}
	OFPixInfoStack BadPixelStack_Channel;
	BadPixelStack_Channel.BadStk = new PixelInfo[2000];
	if(BadPixelStack_Channel.BadStk==NULL)
	{
		OutputDebugString("内存分配失败");
		SAFE_RELEASE(pChannelBuff);
		return 0;
	}
	BOOL bResult = TRUE;	
	for (int iChannel = CHANNEL_R; iChannel < CHANNEL_COUNT; iChannel++)
	{
		//获取Channel Buffer
		memset(pChannelBuff,0,sizeof(pChannelBuff));
		GetChannelBuffer(pRawImage,pChannelBuff,nImgWidth,nImgHeight,nPixOrder,iChannel);
		//获取单Channel中的Bad pixel
		memset(BadPixelStack_Channel.BadStk,0,sizeof(PixelInfo)*2000);		
		BadPixelStack_Channel.nBadCnt = 0;
		GetBadPixel_UshortInput(pChannelBuff,nImgWidth/2,nImgHeight/2,&BadPixelStack_Channel);
		if (BadPixelStack_Channel.nBadCnt >= 2000)
		{
			OutputDebugString("Bad Pixel Count超过2000，环境异常");
			bResult = FALSE;
			break;
		}
		//获取bad pixel在整张影像中的位置
		GetBadPixelPosition(&BadPixelStack_Channel,nPixOrder,iChannel);
		//确定bad pixel相邻关系，并记录相邻个数>=1的bad pixel用于烧录
		BOOL ret = GetClusterFromStack(&BadPixelStack_Channel,DfcStk);	//获取测试结果

		if (ret == 0)
		{
			OutputDebugString("Get Cluster From Stack失败");
			bResult = FALSE;
			break;
		}
	}	
	SAFE_RELEASE(pChannelBuff);
	SAFE_RELEASE(BadPixelStack_Channel.BadStk);
	//将bad pixel按照相邻个数从大到小排序
	SorttBPTPixel(DfcStk,0,DfcStk->DefectStackCnt);

	if (SaveData)
    {
		OutputDebugString("-----------------save data-----------start");
		CreateDirectory("D:\\BPTTest",NULL);
		CSV_MakeHeader("D:\\BPTTest\\BPT.csv");
		FILE *fp=NULL;
		fp=freopen("D:\\BPTTest\\BPT.csv","at+",stdout);
		fseek(fp,0,SEEK_END);
		for(int  i=0; i<DfcStk->DefectStackCnt; i++)
		{
			printf("%d,%d,%d,%d,%d,%d,%d",i,DfcStk->BadPixelStack[i].xIndex,DfcStk->BadPixelStack[i].yIndex,DfcStk->BadPixelStack[i].x, DfcStk->BadPixelStack[i].y, DfcStk->BadPixelStack[i].NeighborNum,DfcStk->BadPixelStack[i].Y_Value);
			printf("\n");
		}
        fclose(fp);
		OutputDebugString("-----------------save data-----------end");
    }
	
	return bResult;
}
