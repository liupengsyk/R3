#include "StdAfx.h"
#include "ImageProc.h"
#include "math.h"
#include "nv21_converter.h"

#pragma comment(lib,"nv21_converter.lib")
 
using namespace std;
 
BOOL ImageProc::DTRaw10toRaw8(BYTE *pSrc, BYTE* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	int k = 0;
	int c = 0;
	for (int p=0; p<width *height; p++)
	{
		pDest[p]=pSrc[k];
		k++;
		c++;
		if (c==4)
		{
			c=0;
			k++;
		}
	}
	return TRUE;
}

BOOL ImageProc::RawToRaw8(BYTE* pSrc, BYTE* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	for (int p = 0; p<width*height; p++)
	{
		pDest[p] = pSrc[p]&0xff;
	}
	return TRUE;
}

BOOL ImageProc::RawToRaw8(USHORT* pSrc, BYTE* pDest, int width, int height,BYTE bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE shift = bits -8;

	for (int p = 0; p<width*height; p++)
	{
		pDest[p] = (pSrc[p]>>shift)&0xff;
	}
	return TRUE;
}

BOOL ImageProc::DTRaw8toRaw(BYTE* pSrc, USHORT* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	for (int p = 0; p<width*height; p++)
	{
		pDest[p] = pSrc[p];
	}
	return TRUE;
}

BOOL ImageProc::DTRaw10toRaw(BYTE* pSrc, USHORT* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	int k = 0;
	for (int p = 0; p<width*height*5/4; p += 5)
	{
		pDest[k] = (pSrc[p]<<2) | (pSrc[p+4]&0x03);
		pDest[k+1] = (pSrc[p+1]<<2) |  (pSrc[p+4]>>2&0x03) ;
		pDest[k+2] = (pSrc[p+2]<<2) |  (pSrc[p+4]>>4&0x03) ;
		pDest[k+3] = (pSrc[p+3]<<2) |  (pSrc[p+4]>>6&0x03) ;
		k+=4;
	}

	return TRUE;
}

BOOL ImageProc::DTRaw12toRaw(BYTE* pSrc, USHORT* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	int k = 0;
	for (int p = 0; p<width*height*3/2; p += 3)
	{
		pDest[k] = (pSrc[p]<<4) | (pSrc[p+2]>>4 & 0x0f);
		pDest[k+1] = (pSrc[p+1]<<4) |  (pSrc[p+2] & 0x0f);
		k+=2;
	}

	return TRUE;
}

BOOL ImageProc::Raw8toRGB24(BYTE *pSrc,BYTE* pDest, int width, int height,BYTE outformat)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	switch(outformat)
	{
	case IMG_OUTFORMAT_BGGR:
		Interpolation_BGGR(pSrc,pDest,width,height);
		break;
	case IMG_OUTFORMAT_GRBG:
		Interpolation_GRBG(pSrc,pDest,width,height);
		break;
	case IMG_OUTFORMAT_RGGB:
		Interpolation_RGGB(pSrc,pDest,width,height);
		break;
	}

	return TRUE;
}



BOOL ImageProc::ZoomOutRGBImage(BYTE *pSrc, int width, int height, BYTE* pDest, int ratio, int flag)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	int DestWidth   = width / ratio;
	int DestHeight  = height / ratio;

	int patcData = 0;
	if((DestWidth)%4!=0)
	{
		patcData = DestWidth%4;
	}

	DestWidth -= patcData;
	width -= patcData*ratio; 

	int count = ratio*ratio;
	int k = 0;

	switch (flag)
	{
	case 0: //均值
		{
			for (int i=0; i<height; i+=ratio)
			{
				for (int j=0; j<width; j+=ratio)
				{
					int R_sum = 0;
					int G_sum = 0;
					int B_sum = 0;
					for(int y=i; y<(i+ratio); y++)
					{
						for(int x=j; x<(j+ratio); x++)
						{
							B_sum += pSrc[ (y*width + x)*3 ];
							G_sum += pSrc[ (y*width + x)*3 +1];
							R_sum += pSrc[ (y*width + x)*3 +2];
						}
					}
					pDest[k++] = B_sum/count;
					pDest[k++] = G_sum/count;
					pDest[k++] = R_sum/count;
				}
				/*for(int m_patcData=0;m_patcData<patcData;m_patcData++)
				{
					pDest[k++] = 0;
					pDest[k++] = 0;
					pDest[k++] = 0;
				}*/

			}
			break;
		}
	default:
		{
			for(int i=0;i<DestHeight;i++)
			{
				for(int j=0;j<DestWidth;j++)
				{
					pDest[(i*DestWidth+j)*3]   = pSrc[(i*width+j)*ratio*3];
					pDest[(i*DestWidth+j)*3+1] = pSrc[(i*width+j)*ratio*3+1];
					pDest[(i*DestWidth+j)*3+2] = pSrc[(i*width+j)*ratio*3+2];
				}
				/*for(int m_patcData=0;m_patcData<patcData;m_patcData++)
				{
				pDest[k++] = 0;
				pDest[k++] = 0;
				pDest[k++] = 0;
				}*/
			}
			break;
		}

	}

	return TRUE;
}


BOOL ImageProc::ZoomOutYImage(BYTE *pSrc, int width, int height, BYTE* pDest, int ratio, int flag)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	if (ratio<=1)
	{
		ratio = 1;
	}
	else if ( (ratio&1) == 1)	//必须为2的倍数
	{
		ratio ++;
	} 
	int DestWidth   = width / ratio;
	int DestHeight  = height / ratio;


	int count = ratio*ratio;
	int k = 0;

	switch (flag)
	{
	case 0: //均值
		{
			for (int i=0; i<height; i+=ratio)
			{
				for (int j=0; j<width; j+=ratio)
				{
					int y_sum = 0;

					for(int y=i; y<(i+ratio); y++)
					{
						for(int x=j; x<(j+ratio); x++)
						{
							y_sum += pSrc[ (y*width + x) ];

						}
					}
					pDest[k++] = y_sum/count;

				}


			}
			break;
		}
	default:
		{
			for(int i=0;i<DestHeight;i++)
			{
				for(int j=0;j<DestWidth;j++)
				{
					pDest[(i*DestWidth+j)]   = pSrc[(i*width+j)*ratio];

				}

			}
			break;
		}

	}

	return TRUE;
}
 
double ImageProc::GetMTF_Iteration( BYTE *pSrc, int width, int height,CRect rect)
{

	if (!pSrc) return 0;
	if (rect.Height() == 0 && rect.Height() == 0)
		return 0;


	//像素点个数
	int count = 0;
	//灰度总和
	int GraySum = 0;
	//统计直方
	int histogram[256];
	memset(histogram, 0, 256*sizeof(int));

	for (int i = rect.top; i<rect.bottom; i++)
	{
		for (int j = rect.left; j<rect.right; j++)
		{
			int gray = pSrc[i*width+j];
			histogram[gray]++;
			//计算灰度总和
			GraySum += gray;
			count++;
		}
	}
	//去除掉1/100个最暗点
	int DarkSum = 0, HotSum = 0;
	for(int i=0; i<256; i++ )
	{
		DarkSum += histogram[i];
		if (DarkSum <= 0.01*count)
			histogram[i]=0;
		else
			break;
	}
	//去除掉1/100个最亮点
	for(int i=255; i>=0; i-- )
	{
		HotSum += histogram[i];
		if (HotSum <= 0.01*count)
			histogram[i]=0;
		else
			break;
	}

	//重新计算总体灰度和个数
	GraySum = 0;
	count = 0;
	for (int i=0; i<256; i++)
	{
		//计算灰度总和
		GraySum += i*histogram[i];
		count += histogram[i];
	}
	//得到平均灰度  
	double  aveGray = (double)GraySum / count;

	double MaxAve = 0;
	double MinAve = 0;
	//迭代求阈值
	double th = aveGray;
	double th_old  = 0;
	BOOL   iteration = TRUE;

	int MaxSum   = 0;
	int MinSum   = 0;
	int MaxCount = 0;
	int MinCount = 0;

	while(iteration)
	{
		if ( abs(th-th_old)<1 )  iteration = FALSE;
		MaxSum   = 0;
		MinSum   = 0;
		MaxCount = 0;
		MinCount = 0;
		for(int i=0;i<256;i++ )
		{
			if (i>th)
			{
				MaxSum+=i*histogram[i];
				MaxCount+=histogram[i];
			}
			else
			{
				MinSum+=i*histogram[i];
				MinCount+=histogram[i];
			}
		}
		if (MaxCount==0 || MinCount==0)
			return 0;
		MaxAve = MaxSum * 1.0 /MaxCount;
		MinAve = MinSum * 1.0 /MinCount;
		th_old = th;
		th = (MinAve+MaxAve)/2;
	}

	int MaxLimit = MaxCount*2/3;
	int MinLimit = MinCount*2/3;

	MaxSum   = 0;
	MinSum   = 0;
	MaxCount = 0;
	MinCount = 0;

	for(int i=255; i>=0; i-- )
	{
		if (MaxCount < MaxLimit )
		{
			MaxSum   += i*histogram[i];
			MaxCount += histogram[i];
		}
		else
			break;
	}

	for(int i=0; i<256; i++ )
	{
		if (MinCount < MinLimit )
		{
			MinSum   += i*histogram[i];
			MinCount += histogram[i];
		}
		else
			break;
	}

	//除数为0，返回0
	if (MaxCount== 0||MinCount==0)		return 0;

	MaxAve = MaxSum / MaxCount;
	MinAve = MinSum / MinCount;

	double vMTF = (MaxAve-MinAve)/(MaxAve+MinAve);
	return vMTF;
}

double ImageProc::GetMTF( BYTE *pSrc, int width, int height,CRect rect)
{
	//return GetMTF_Iteration(pSrc, width, height, rect);
	return nGetMTF(pSrc,  width,  height, rect);

}
//FROM OFILM
double  ImageProc::nGetMTF(BYTE *pSrc, int width, int height,CRect rect)
{  
	double MtfValue;  
	float	Y_Avr =  GetAveY_ROI( pSrc,  width,  height, rect);

	if ( (Y_Avr > 235) || (Y_Avr < 16) )
	{  
		return 0;
	} 
	int Y_max(0),Y_min(255),iMax_Count(0),iMin_Count(0),temp(0),Y_Max_Sum(0),Y_Min_Sum(0);
	float Y_Max_Avr(0.0),Y_Min_Avr(0.0);

	for (int i = rect.top; i < rect.bottom; i++)
	{
		for (int j = rect.left; j < rect.right; j++)
		{
			temp = pSrc[i * width + j];

			if (temp >Y_max)  //  
			{
				 Y_max = temp;
			}
			  if (temp <Y_min)  
			{ 
				Y_min= temp;
			}
		}
	}
	 
	for (int i = rect.top; i < rect.bottom; i++)
	{
		for (int j = rect.left; j < rect.right; j++)
		{
			temp = pSrc[i * width + j];

			if ((temp > Y_Avr + (Y_max - Y_Avr) / 3) && (temp < Y_max - 3))  // 后面的条件是减少亮点的影响
			{
				Y_Max_Sum += temp;
				iMax_Count++;
			}
			else if ((temp < Y_Avr - (Y_Avr - Y_min) / 3) && (temp > Y_min + 2))  // 后面的条件是减少脏点的影响
			{
				Y_Min_Sum += temp;
				iMin_Count++;
			}
		}
	}

	if (iMax_Count && iMin_Count)
	{
		Y_Max_Avr = (float)Y_Max_Sum / iMax_Count;
		Y_Min_Avr = (float)Y_Min_Sum / iMin_Count;
		//	Y_Min_Avr = 50;
		MtfValue = (float)((Y_Max_Avr - Y_Min_Avr)/ (Y_Max_Avr + Y_Min_Avr));
	}
	else
	{
		MtfValue = 0.0;
	}
	 
	return MtfValue;
}
double ImageProc::GetMTF_ratio( BYTE *pSrc, int width, int height,CRect rect,int ratio)
{
	//取20%区域
	if (!pSrc) return 0;

	if (rect.Height()==0 && rect.Height()==0)
		return 0;


	//像素点个数
	int allcount=0;
	//灰度总和
	int GraySum=0;
	int max=0;
	int min=255;

	int histogram[256];
	memset(histogram, 0, 256*sizeof(int));

	for(int i=rect.top;i<rect.bottom;i++)
	{
		for(int j = rect.left;j<rect.right;j++)
		{
			int gray=pSrc[i*width+j];
			histogram[gray]++;
			//计算灰度总和
			GraySum+=gray;
			allcount++;
		}
	}
	//去除掉1/10000个最暗点
	int tempSum = 0;
	for(int i=0;i<256;i++ )
	{
		tempSum += histogram[i];
		if (tempSum < 0.0001*allcount)
			histogram[i]=0;
		else
			break;
	}
	//去除掉1/10000个最良点
	tempSum = 0;
	for(int i=255;i>=0;i-- )
	{
		tempSum += histogram[i];
		if (tempSum < 0.0001*allcount)
			histogram[i]=0;
		else
			break;
	}

	//得到平均灰度  
	double  aveGray=GraySum*1.0/allcount;

	double MaxSum=0;
	double MinSum=0;
	int MaxCount=0;
	int MinCount=0;
	double MaxAve=0;
	double MinAve=0;


	for(int i=255; i>=0; i-- )
	{
		if (MaxCount < (allcount*ratio/100) )
		{
			MaxSum   += i*histogram[i];
			MaxCount += histogram[i];
		}
		else
			break;
	}

	for(int i=0; i<256; i++ )
	{
		if (MinCount < (allcount*ratio/100) )
		{
			MinSum   += i*histogram[i];
			MinCount += histogram[i];
		}
		else
			break;
	}


	//除数为0，返回0
	if (MaxCount== 0||MinCount==0)
		return 0;

	MaxAve = MaxSum / MaxCount;
	MinAve = MinSum / MinCount;


	double vMTF = (MaxAve-MinAve)/(MaxAve+MinAve);
	return vMTF;

}

BOOL ImageProc::RGB24toY(BYTE *pSrc,BYTE* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	for(int i=0; i<width*height; i++)
	{
		pDest[i]=(114*pSrc[i*3]+587*pSrc[i*3+1]+299*pSrc[i*3+2]+500)/1000;
	}
	return TRUE;
}

float ImageProc::GetAveY_ROI(BYTE *pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;

	DWORD sum=0;
	int count=0;
	float AveY;
	for(int i=rect.top;i<rect.bottom;i++)
	{
		for(int j = rect.left;j<rect.right;j++)
		{
			sum+=pSrc[i*width+j];
			count++;
		}
	}
	if (count==0)
	{
		return -1;
	}
	AveY=(float)sum/count;
	return AveY;
}

float ImageProc::GetAveY_ROI_RGB24(BYTE *pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;
	int sum=0;
	int count=0;
	float AveY;
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j = rect.left; j<rect.right; j++)
		{
			sum += (114*pSrc[(i*width+j)*3] + 587*pSrc[(i*width+j)*3+1] + 299*pSrc[(i*width+j)*3+2] + 500)/1000;
			count++;
		}
	}
	AveY=(float)sum/count;
	return AveY;
}
float ImageProc::GetAveR_ROI_RGB24(BYTE *pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;
	int sum=0;
	int count=0;
	float Ave;
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j = rect.left; j<rect.right; j++)
		{
			sum += pSrc[(i*width+j)*3+2];
			count++;
		}
	}
	Ave=(float)sum/count;
	return Ave;
}
float ImageProc::GetAveG_ROI_RGB24(BYTE *pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;
	int sum=0;
	int count=0;
	float Ave;
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j = rect.left; j<rect.right; j++)
		{
			sum += pSrc[(i*width+j)*3+1];
			count++;
		}
	}
	Ave=(float)sum/count;
	return Ave;
}
float ImageProc::GetAveB_ROI_RGB24(BYTE *pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;
	int sum=0;
	int count=0;
	float Ave;
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j = rect.left; j<rect.right; j++)
		{
			sum += pSrc[(i*width+j)*3];
			count++;
		}
	}
	Ave = (float)sum/count;
	return Ave;
}
float ImageProc::GetAveR_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat)
{
	if (!pSrc) return 0;

	int sum=0;
	int count=0;
	float AveR;
	int m,n;
	switch(outformat)
	{
	case IMG_OUTFORMAT_RGGB:
		if ( (rect.top&1)==0 && (rect.left&1)==0 ) // 00
		{
			m=0;
			n=0;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==0 ) // 10
		{
			m=1;
			n=0;
		}
		else if ( (rect.top&1)==0 && (rect.left&1)==1 ) // 01
		{
			m=0;
			n=1;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==1 ) // 11
		{
			m=1;
			n=1;
		}
		break;
	case IMG_OUTFORMAT_BGGR:
		if ( (rect.top&1)==0 && (rect.left&1)==0 ) // 00
		{
			m=1;
			n=1;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==0 ) // 10
		{
			m=0;
			n=1;
		}
		else if ( (rect.top&1)==0 && (rect.left&1)==1 ) // 01
		{
			m=1;
			n=0;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==1 ) // 11
		{
			m=0;
			n=0;
		}
		break;
	case IMG_OUTFORMAT_GRBG:
		if ( (rect.top&1)==0 && (rect.left&1)==0 ) // 00
		{
			m=0;
			n=1;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==0 ) // 10
		{
			m=1;
			n=1;
		}
		else if ( (rect.top&1)==0 && (rect.left&1)==1 ) // 01
		{
			m=0;
			n=0;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==1 ) // 11
		{
			m=1;
			n=0;
		}
		break;
	case IMG_OUTFORMAT_GBRG:
		if ( (rect.top&1)==0 && (rect.left&1)==0 ) // 00
		{
			m=1;
			n=0;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==0 ) // 10
		{
			m=0;
			n=0;
		}
		else if ( (rect.top&1)==0 && (rect.left&1)==1 ) // 01
		{
			m=1;
			n=1;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==1 ) // 11
		{
			m=0;
			n=1;
		}

		break;
	}

	for(int i=rect.top;i<rect.bottom;i+=2)
	{
		for(int j = rect.left;j<rect.right;j+=2)
		{
			sum+=pSrc[(i+m)*width+j+n];
			count++;
		}
	}

	AveR=(float)sum/count;
	return AveR;
}
float ImageProc::GetAveB_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat)
{
	if (!pSrc) return 0;

	int sum=0;
	int count=0;
	float AveB;
	int m,n;

	switch(outformat)
	{
	case IMG_OUTFORMAT_RGGB:
		if ( (rect.top&1)==0 && (rect.left&1)==0 ) // 00
		{
			m=1;
			n=1;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==0 ) // 10
		{
			m=0;
			n=1;
		}
		else if ( (rect.top&1)==0 && (rect.left&1)==1 ) // 01
		{
			m=1;
			n=0;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==1 ) // 11
		{
			m=0;
			n=0;
		}
		break;
	case IMG_OUTFORMAT_BGGR:
		if ( (rect.top&1)==0 && (rect.left&1)==0 ) // 00
		{
			m=0;
			n=0;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==0 ) // 10
		{
			m=1;
			n=0;
		}
		else if ( (rect.top&1)==0 && (rect.left&1)==1 ) // 01
		{
			m=0;
			n=1;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==1 ) // 11
		{
			m=1;
			n=1;
		}
		break;
	case IMG_OUTFORMAT_GRBG:
		if ( (rect.top&1)==0 && (rect.left&1)==0 ) // 00
		{
			m=1;
			n=0;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==0 ) // 10
		{
			m=0;
			n=0;
		}
		else if ( (rect.top&1)==0 && (rect.left&1)==1 ) // 01
		{
			m=1;
			n=1;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==1 ) // 11
		{
			m=0;
			n=1;
		}
		break;
	case IMG_OUTFORMAT_GBRG:
		if ( (rect.top&1)==0 && (rect.left&1)==0 ) // 00
		{
			m=0;
			n=1;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==0 ) // 10
		{
			m=1;
			n=1;
		}
		else if ( (rect.top&1)==0 && (rect.left&1)==1 ) // 01
		{
			m=0;
			n=0;
		}
		else if ( (rect.top&1)==1 && (rect.left&1)==1 ) // 11
		{
			m=1;
			n=0;
		}

		break;
	}

	for(int i=rect.top;i<rect.bottom;i+=2)
	{
		for(int j = rect.left;j<rect.right;j+=2)
		{
			sum += pSrc[(i+m)*width+j+n];
			count++;
		}
	}
	AveB=(float)sum/count;
	return AveB;
}
float ImageProc::GetAveG_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat)
{
	if (!pSrc) return 0;

	int sum=0;
	int count=0;
	float AveG;
	switch(outformat)
	{
	case IMG_OUTFORMAT_RGGB:
		if ( (rect.top&1) == (rect.left&1) )
		{
			for(int i=rect.top;i<rect.bottom;i+=2)
			{
				for(int j = rect.left;j<rect.right;j+=2)
				{
					sum+=pSrc[(i)*width+j+1];
					count++;
					sum+=pSrc[(i+1)*width+j];
					count++;
				}
			}
		}
		else
		{
			for(int i=rect.top;i<rect.bottom;i+=2)
			{
				for(int j = rect.left;j<rect.right;j+=2)
				{
					sum+=pSrc[(i)*width+j];
					count++;
					sum+=pSrc[(i+1)*width+j+1];
					count++;
				}
			}
		}

		break;
	case IMG_OUTFORMAT_BGGR:
		if ((rect.top&1) == (rect.left&1))
		{
			for(int i=rect.top;i<rect.bottom;i+=2)
			{
				for(int j = rect.left;j<rect.right;j+=2)
				{
					sum+=pSrc[(i)*width+j+1];
					count++;
					sum+=pSrc[(i+1)*width+j];
					count++;
				}
			}
		} 
		else
		{		
			for(int i=rect.top;i<rect.bottom;i+=2)
			{
				for(int j = rect.left;j<rect.right;j+=2)
				{
					sum+=pSrc[(i)*width+j];
					count++;
					sum+=pSrc[(i+1)*width+j+1];
					count++;
				}
			}
		}

		break;
	case IMG_OUTFORMAT_GRBG:
		if ((rect.top&1) == (rect.left&1))
		{
			for(int i=rect.top;i<rect.bottom;i+=2)
			{
				for(int j = rect.left;j<rect.right;j+=2)
				{
					sum+=pSrc[(i)*width+j];
					count++;
					sum+=pSrc[(i+1)*width+j+1];
					count++;
				}
			}
		}
		else
		{
			for(int i=rect.top;i<rect.bottom;i+=2)
			{
				for(int j = rect.left;j<rect.right;j+=2)
				{
					sum+=pSrc[(i)*width+j+1];
					count++;
					sum+=pSrc[(i+1)*width+j];
					count++;
				}
			}
		}
		break;
	case IMG_OUTFORMAT_GBRG:
		if ((rect.top&1) == (rect.left&1))
		{
			for(int i=rect.top;i<rect.bottom;i+=2)
			{
				for(int j = rect.left;j<rect.right;j+=2)
				{
					sum+=pSrc[(i)*width+j];
					count++;
					sum+=pSrc[(i+1)*width+j+1];
					count++;
				}
			}
		}
		else
		{
			for(int i=rect.top;i<rect.bottom;i+=2)
			{
				for(int j = rect.left;j<rect.right;j+=2)
				{
					sum+=pSrc[(i)*width+j+1];
					count++;
					sum+=pSrc[(i+1)*width+j];
					count++;
				}
			}
		}

		break;
	}

	AveG = (float)sum/(count);
	return AveG;
}
/********************************************************
函数名: GetHistogram
函数功能:  得到图像的直方图
函数参数：1-  pSrc      源数据区指针
2-  width     图像的长
3-  height    图像的宽
返回值： 直方图数据指针
作者 : lee**    创建日期 : 2013.4.21
********************************************************/
int* ImageProc::GetHistogram(BYTE *pSrc, int width, int height )
{
	if (!pSrc) return 0;

	int* histogram=new int[256];
	memset(histogram,0,256*sizeof(int));

	int total=0;
	int count=0;
	for(int i=0;i<height;i++)
	{
		int iPitch=i*width;
		for(int j=0;j<width;j++)
		{
			int gray=pSrc[iPitch+j];
			histogram[gray]++;
			total+=gray;
			count++;
		}
	}
	return histogram;
}

/********************************************************
函数名: GetHistogramThreshold
函数功能:  计算直方图面积比阈值，得到关于指定面积比的亮度值
函数参数：1-  pSrc      源数据区指针
2-  width     图像的长
3-  height    图像的宽
4-  ratio      指定的面积比
返回值： 亮度阈值
作者 : lee**    创建日期 : 2013.4.21
********************************************************/
int ImageProc::GetHistogramThreshold(BYTE *pSrc, int width, int height, float ratio)
{
	if (!pSrc) return 0;

	int* Histogram_buffer = ImageProc::GetHistogram(pSrc,width,height);

	int count=0; 
	int Sum=width*height;

	int th=0;
	for(int i=255;i>0;i--)
	{
		count += Histogram_buffer[i];
		if (count>Sum*ratio/100.0)
		{
			th = i;
			goto End;
		}
	}
End:
	delete [] Histogram_buffer;
	return th;
}

/********************************************************
函数名: GetOpticalCenter
函数功能:  计算光学中心
函数参数：1-  pSrc      源数据区指针
2-  width     图像的长
3-  height    图像的宽
返回值： 光学中心的坐标
作者 : lee**    创建日期 : 2013.4.21
********************************************************/
CPoint  ImageProc::GetOpticalCenter(BYTE *pSrc, int width, int height, int ratio)
{
	if (!pSrc) return 0;

	//得到图像阈值，此函数里调用直方图函数
	int threshold =(int) GetHistogramThreshold(pSrc,width,height,ratio);
	//二值化图像
	BYTE* temp = new BYTE[width*height];
	memcpy(temp,pSrc,width*height);
	Binary(temp,width,height,threshold);

	//计算光学中心
	double x=0,y=0;
	int count=0;
	for(int i = 0;i<height;i++)
	{
		for(int j= 0;j<width;j++)
		{
			if (temp[i*width+j]==0)
			{
				x+=j;
				y+=i;
				count++;
			}
		}
	}
	delete [] temp;
	x=x/count;
	y=y/count;
	CPoint pt((int)x,(int)y);
	return pt;
}


int* ImageProc::GetHistogramY_ROI(BYTE *pSrc, int width, int height,CRect rect )
{
	if (!pSrc) return 0;

	int* histogram=new int[256];
	memset(histogram,0,256*sizeof(int));

	int total=0;
	int count=0;

	for (int i = rect.top; i<rect.bottom; i++)
	{
		for (int j = rect.left; j<rect.right; j++)
		{
			int gray = pSrc[i*width+j];
			histogram[gray]++;
			//计算灰度总和
			total += gray;
			count++;
		}
	}

	return histogram;
}


int  ImageProc::GetHistogramThreshold(BYTE *pSrc, int width, int height, CRect rect,float ratio)
{
	if (!pSrc) return 0;

	int* Histogram_buffer = ImageProc::GetHistogramY_ROI(pSrc,width,height,rect);

	int count=0; 
	int Sum=rect.Width()*rect.Height();

	int th=0;
	for(int i=255;i>0;i--)
	{
		count += Histogram_buffer[i];
		if (count>Sum*ratio/100.0)
		{
			th = i;
			goto End;
		}
	}
End:
	delete [] Histogram_buffer;
	return th;
}


//获取指定roi块的中心
//指定rect roi
//输入块亮度相对roi平均亮度的值 ratio
//Type 指定模式 模式1为计算块中心，模式 0 为重新划定Roi
//返回整型坐标点  更新后的roi outRect
void ImageProc::GetBlackBlobkCenterY_ROI(BYTE *pSrc, int width, int height,CRect rect,int ratio,float &x,float &y)
{

	//Rect中y的平均值
	float AveY = ImageProc::GetAveY_ROI(pSrc,width,height,rect);

	double Total_x=0,Total_y=0;

	int count=0;
	int gray ;
	int m=0; 

	//计算黑块中心，取值有效黑块为平均值的1/10;
	for (int i = rect.top; i<rect.bottom; i++)
	{
		for (int j = rect.left; j<rect.right; j++)
		{
			gray = pSrc[i*width+j];
			if (gray<(AveY/ratio))
			{
				Total_x+=j;
				Total_y+=i;
				count++;
			}		 

		}
	}  
	if (count==0)
	{
		count =1;
	}
	x=Total_x/(double)count;
	y=Total_y/(double)count; 
}

//获取指定roi块的中心
//指定rect roi
//输入块亮度相对roi平均亮度的值 ratio
//Type 指定模式 模式1为计算块中心，模式 0 为重新划定Roi
//返回整型坐标点  更新后的roi outRect
void ImageProc::AjustRoiRect(BYTE *pSrc, int width, int height,int ratio,CRect rect,CRect *outRect)
{

	//Rect中y的平均值
	float AveY = ImageProc::GetAveY_ROI(pSrc,width,height,rect);

	double Total_x=0,Total_y=0;
	double x=0,y=0;
	int count=0; 
	int m=0;



	BOOL lefgFlag = TRUE;
	BOOL rightFlag = TRUE;
	BOOL topFlag = TRUE;
	BOOL botFlag = TRUE;

	int LeftStart = 0;
	int RightEnd = 0;
	int TopStart = 0;
	int BotEnd = 0;

	//step 1
	//识别左边正真开始的框,从上往下，从左往右，一列列扫描
	for (int j = rect.left; j<rect.right; j++)
	{
		for (int i = rect.top; i<rect.bottom; i++)
		{
			int gray = pSrc[i*width+j];
			if (gray<(AveY/ratio))
			{
				lefgFlag = FALSE;
				break;
			}
		}
		if (lefgFlag)
		{
			LeftStart = j;
			break;
		}
		lefgFlag = TRUE;			
	}

	//step 2
	//识别右边边正真开始的框，从上往下，从右往左，一列列扫描
	for (int j = rect.right; j>LeftStart; j--)
	{
		for (int i = rect.top; i<rect.bottom; i++)
		{
			int gray = pSrc[i*width+j];
			if (gray<(AveY/ratio))
			{
				rightFlag = FALSE;
				break;
			}
		}
		if (rightFlag)
		{
			RightEnd = j;
			break;
		}
		rightFlag = TRUE;			
	}

	//step 3
	//识别上边正真开始的框，从左往右，从上往下一行行扫描
	for (int i = rect.top; i<rect.bottom; i++)//int i = rect.top; i<rect.bottom; i++
	{
		for (int j = LeftStart; j<RightEnd; j++)
		{
			int gray = pSrc[i*width+j];
			if (gray<(AveY/ratio))
			{
				topFlag = FALSE;
				break;
			}
		}
		if (topFlag)
		{
			TopStart = i;
			break;
		}
		topFlag = TRUE;			
	}

	//step 4
	//识别下边正真开始的框，从左往右，从下往上一行行扫描
	for (int i = rect.bottom; i>TopStart; i--)//int i = rect.top; i<rect.bottom; i++
	{
		for (int j = LeftStart; j<RightEnd; j++)
		{
			int gray = pSrc[i*width+j];
			if (gray<(AveY/ratio))
			{
				botFlag = FALSE;
				break;
			}
		}
		if (botFlag)
		{
			BotEnd = i;
			break;
		}
		botFlag = TRUE;			
	} 

	outRect->left = LeftStart;
	outRect->right = RightEnd;
	outRect->top = TopStart;
	outRect->bottom = BotEnd; 
}

//处理二值化后的图像
//获取指定roi块的中心
//指定rect roi
//输入块亮度相对roi平均亮度的值 ratio
//Type 指定模式 模式1为计算块中心，模式 0 为重新划定Roi
//输出 更新后的roi outRect，输出块的坐标
void ImageProc::GetBlackBlobkCenterY_BinaryROI(BYTE *pSrc, int width, int height,CRect rect,float& Point_x,float& Point_y,int& PointSize)
{ 
	double Total_x=0,Total_y=0;
	double x=0,y=0;
	int count=0;
	int gray;
	int m=0;

	//int yAvg = (int) GetAveY_ROI(pSrc,width, height,  rect);

	//计算黑块中心，取值有效黑块为平均值的1/10;
	for (int i = rect.top; i<rect.bottom; i++)
	{
		for (int j = rect.left; j<rect.right; j++)
		{
			gray = pSrc[i*width+j];
			//if (gray<yAvg)
			if (gray>128)
			{
				Total_x+=j;
				Total_y+=i;
				count++;
			}		 

		}
	}  
	if (count==0)
	{
		count =1;
	}
	x=Total_x/count;
	y=Total_y/count;
	PointSize = count;
	Point_x = (float)x;
	Point_y  = (float)y;
	return; 
}

/********************************************************
函数名: Binary
函数功能:  二值化 大于阈值为0，小于为255（0xff）
函数参数：1-  pSrc      源数据区指针
2-  width     图像的长
3-  height    图像的宽
4-  Threshold      阈值
返回值： 无
作者 : lee**    创建日期 : 2013.4.21
********************************************************/
BOOL  ImageProc::Binary(int *pSrc, int width, int height, int Threshold)
{

	if (!pSrc) return FALSE;

	for(int i=0;i<height*width;i++)
	{
		if (pSrc[i]>=Threshold)
			pSrc[i]=0;
		else
			pSrc[i]=255;
	}

	return TRUE;
}
/********************************************************
函数名: Binary
函数功能:  二值化 大于阈值为0，小于为255（0xff）
函数参数：1-  pSrc      源数据区指针
2-  width     图像的长
3-  height    图像的宽
4-  Threshold      阈值
返回值： 无
作者 : lee**    创建日期 : 2013.4.21
********************************************************/
BOOL  ImageProc::Binary(BYTE *pSrc, int width, int height, int Threshold)
{
	if (!pSrc) return FALSE;

	for(int i=0;i<height*width;i++)
	{
		if (pSrc[i]>=Threshold)
			pSrc[i]=0;
		else
			pSrc[i]=255;
	}

	return TRUE;
}


/********************************************************
函数名: GetGr
函数功能:  得到Raw8图像的Gr的Buffer
函数参数：1-  pSrc         Raw10数据区指针
2-  pDest       Gr数据区的指针           
3-  width        Gr数据区图像的长
4-  height       Gr数据区图像的宽
5-  outformat  Sensor的输出格式
返回值： 无
作者 : lee**    创建日期 : 2013.4.21
********************************************************/
USHORT* ImageProc::GetGr_Channel(USHORT* pSrc, int width, int height, int outformat)
{
	if (!pSrc) return 0;

	USHORT* pDest = new USHORT[(width/2)*(height/2)];
	int k=0;
	switch(outformat)
	{
	case IMG_OUTFORMAT_RGGB:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[i*width+j+1];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_BGGR:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i+1)*width+j];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_GRBG:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i)*width+j];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_GBRG:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i+1)*width+j+1];
				k++;
			}
		}
		break;
	}

	return pDest;
}

USHORT* ImageProc::GetGb_Channel(USHORT* pSrc, int width, int height, int outformat)
{
	if (!pSrc) return 0;

	USHORT* pDest = new USHORT[(width/2)*(height/2)];
	int k=0;
	switch(outformat)
	{
	case IMG_OUTFORMAT_RGGB:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i+1)*width+j];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_BGGR:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i)*width+j+1];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_GRBG:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i+1)*width+j+1];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_GBRG:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i)*width+j];
				k++;
			}
		}
		break;
	}
	return pDest;
}

USHORT* ImageProc::GetR_Channel(USHORT* pSrc, int width, int height, int outformat)
{
	if (!pSrc) return 0;

	USHORT* pDest = new USHORT[(width/2)*(height/2)];
	int k=0;
	switch(outformat)
	{
	case IMG_OUTFORMAT_RGGB:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[i*width+j];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_BGGR:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i+1)*width+j+1];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_GRBG:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i)*width+j+1];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_GBRG:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i+1)*width+j];
				k++;
			}
		}
		break;
	}
	return pDest;
}

USHORT* ImageProc::GetB_Channel(USHORT* pSrc, int width, int height, int outformat)
{
	if (!pSrc) return 0;

	USHORT* pDest = new USHORT[(width/2)*(height/2)];
	int k = 0;
	switch(outformat)
	{
	case IMG_OUTFORMAT_RGGB:
		for(int i=0; i<height; i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i+1)*width+j+1];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_BGGR:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i)*width+j];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_GRBG:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i+1)*width+j];
				k++;
			}
		}
		break;
	case IMG_OUTFORMAT_GBRG:
		for(int i=0;i<height;i+=2)
		{
			for (int j=0;j<width;j+=2)
			{
				pDest[k]=pSrc[(i)*width+j+1];
				k++;
			}
		}
		break;
	}
	return pDest;
}

BOOL ImageProc::Interpolation_BGGR(BYTE* pSrc,BYTE *pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;



	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//R
			pDest[(i*width+j)*3]=(pSrc[(i-1)*width+j-1]+ pSrc[(i-1)*width+j+1]+ pSrc[(i+1)*width+j-1]+ pSrc[(i+1)*width+j+1])/4;
			pDest[(i*width+j)*3+1]=(pSrc[(i-1)*width+j]+pSrc[(i+1)*width+j]+pSrc[(i)*width+j-1]+pSrc[(i)*width+j+1])/4;
			pDest[(i*width+j)*3+2]=pSrc[i*width+j];
			//GR
			pDest[(i*width+j)*3+3]=(pSrc[(i-1)*width+j+1]+ pSrc[(i+1)*width+j+1])/2;
			pDest[(i*width+j)*3+4]=pSrc[i*width+j+1];
			pDest[(i*width+j)*3+5]=(pSrc[(i)*width+j]+pSrc[(i)*width+j+2])/2;
			//GB
			pDest[((i+1)*width+j)*3]=(pSrc[(i+1)*width+j-1]+pSrc[(i+1)*width+j+1])/2;
			pDest[((i+1)*width+j)*3+1]=pSrc[(i+1)*width+j];
			pDest[((i+1)*width+j)*3+2]=(pSrc[(i)*width+j]+ pSrc[(i+2)*width+j])/2;
			//B
			pDest[((i+1)*width+j)*3+3]=pSrc[(i+1)*width+j+1];
			pDest[((i+1)*width+j)*3+4]=(pSrc[(i)*width+j+1]+pSrc[(i+2)*width+j+1]+pSrc[(i+1)*width+j]+ pSrc[(i+1)*width+j+2])/4;
			pDest[((i+1)*width+j)*3+5]=(pSrc[(i)*width+j]+ pSrc[(i)*width+j+2]+ pSrc[(i+2)*width+j]+pSrc[(i+2)*width+j+2])/4;
		}
	}

	//边缘插值
	for(int j=1;j<width-1;j+=2)
	{
		int d1=((height-1)*width+j)*3;
		//Gb
		pDest[j*3]=(pSrc[j-1]+pSrc[j+1])/2;
		pDest[j*3+1]=pSrc[j];
		pDest[j*3+2]= pSrc[width+j];
		//B
		pDest[j*3+3]=pSrc[j+1];
		pDest[j*3+4]=(pSrc[width+j+1]+pSrc[j]+ pSrc[j+2])/3;
		pDest[j*3+5]=(pSrc[width+j]+ pSrc[width+j+2])/2;
		//R
		pDest[d1]=(pSrc[(height-2)*width+j-1]+ pSrc[(height-2)*width+j+1])/2;
		pDest[d1+1]=(pSrc[(height-2)*width+j]+pSrc[(height-1)*width+j+1]+pSrc[(height-1)*width+j-1])/3;
		pDest[d1+2]=pSrc[(height-1)*width+j];
		//Gr
		pDest[d1+3]=pSrc[(height-2)*width+j+1];
		pDest[d1+4]=pSrc[(height-1)*width+j+1];
		pDest[d1+5]=(pSrc[(height-1)*width+j]+pSrc[(height-1)*width+j+2])/2;

	}

	for(int i=1;i<height-1;i+=2)
	{

		//Gb
		pDest[i*width*3]=(pSrc[(i-1)*width]+pSrc[(i+1)*width])/2;
		pDest[i*width*3+1]=pSrc[i*width];
		pDest[i*width*3+2]= pSrc[i*width+1];
		//B
		pDest[(i+1)*width*3]=pSrc[(i+1)*width];
		pDest[(i+1)*width*3+1]=(pSrc[i*width]+pSrc[(i+2)*width]+ pSrc[(i+1)*width+1])/3;
		pDest[(i+1)*width*3+2]=(pSrc[i*width+1]+ pSrc[(i+2)*width+1])/2;
		//R
		pDest[((i+1)*width-1)*3]=(pSrc[(i)*width-2]+ pSrc[(i+2)*width-2])/2;
		pDest[((i+1)*width-1)*3+1]=(pSrc[(i)*width-1]+pSrc[(i+2)*width-1]+pSrc[(i+1)*width-2])/3;
		pDest[((i+1)*width-1)*3+2]=pSrc[(i+1)*width-1];
		//Gr
		pDest[((i+2)*width-1)*3]=pSrc[(i+2)*width-2];
		pDest[((i+2)*width-1)*3+1]=pSrc[(i+2)*width-1];
		pDest[((i+2)*width-1)*3+2]=(pSrc[(i+1)*width-1]+pSrc[(i+3)*width-1])/2;
	}
	//B
	pDest[0]=pSrc[0];
	pDest[1]=(pSrc[width]+pSrc[1])/2;
	pDest[2]=pSrc[width+1];
	//GB
	pDest[3*(width-1)]=pSrc[width-2];
	pDest[3*(width-1)+1]=pSrc[width-1];
	pDest[3*(width-1)+2]=pSrc[width-1+width];
	//GR
	pDest[(height-1)*3*(width)]=pSrc[(height-2)*width];
	pDest[(height-1)*3*(width)+1]=pSrc[(height-1)*width];
	pDest[(height-1)*3*(width)+2]=pSrc[(height-1)*width+1];
	//R
	pDest[(height*width-1)*3]=pSrc[(height-1)*width-1-1];
	pDest[(height*width-1)*3+1]=(pSrc[height*width-1-1]+pSrc[(height-1)*width-1])/2;
	pDest[(height*width-1)*3+2]=pSrc[height*width-1];

	return TRUE;
}
BOOL ImageProc::Interpolation_RGGB(BYTE* pSrc,BYTE *pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//B
			pDest[(i*width+j)*3] = pSrc[i*width+j];
			pDest[(i*width+j)*3+1] = (pSrc[(i-1)*width+j]+pSrc[(i+1)*width+j]+pSrc[(i)*width+j-1]+pSrc[(i)*width+j+1])/4;
			pDest[(i*width+j)*3+2] = (pSrc[(i-1)*width+j-1]+ pSrc[(i-1)*width+j+1]+ pSrc[(i+1)*width+j-1]+ pSrc[(i+1)*width+j+1])/4;
			//GB
			pDest[(i*width+j)*3+3]=(pSrc[(i)*width+j]+pSrc[(i)*width+j+2])/2;
			pDest[(i*width+j)*3+4]=pSrc[i*width+j+1];
			pDest[(i*width+j)*3+5]=(pSrc[(i-1)*width+j+1]+ pSrc[(i+1)*width+j+1])/2;
			//GR
			pDest[((i+1)*width+j)*3]=(pSrc[(i)*width+j]+ pSrc[(i+2)*width+j])/2;
			pDest[((i+1)*width+j)*3+1]=pSrc[(i+1)*width+j];
			pDest[((i+1)*width+j)*3+2]=(pSrc[(i+1)*width+j-1]+pSrc[(i+1)*width+j+1])/2;
			//R
			pDest[((i+1)*width+j)*3+3]=(pSrc[(i)*width+j]+ pSrc[(i)*width+j+2]+ pSrc[(i+2)*width+j]+pSrc[(i+2)*width+j+2])/4;
			pDest[((i+1)*width+j)*3+4]=(pSrc[(i)*width+j+1]+pSrc[(i+2)*width+j+1]+pSrc[(i+1)*width+j]+ pSrc[(i+1)*width+j+2])/4;
			pDest[((i+1)*width+j)*3+5]=pSrc[(i+1)*width+j+1];
		}
	}

	//边缘插值
	for(int j=1;j<width-1;j+=2)
	{
		int d1=((height-1)*width+j)*3;
		//Gr
		pDest[j*3]=pSrc[width+j];
		pDest[j*3+1]=pSrc[j];
		pDest[j*3+2]= (pSrc[j-1]+pSrc[j+1])/2;
		//R
		pDest[j*3+3]=(pSrc[width+j]+ pSrc[width+j+2])/2;
		pDest[j*3+4]=(pSrc[width+j+1]+pSrc[j]+ pSrc[j+2])/3;
		pDest[j*3+5]=pSrc[j+1];
		//B
		pDest[d1]=pSrc[(height-1)*width+j];
		pDest[d1+1]=(pSrc[(height-2)*width+j]+pSrc[(height-1)*width+j+1]+pSrc[(height-1)*width+j-1])/3;
		pDest[d1+2]=(pSrc[(height-2)*width+j-1]+ pSrc[(height-2)*width+j+1])/2;
		//Gb
		pDest[d1+3]=(pSrc[(height-1)*width+j]+pSrc[(height-1)*width+j+2])/2;
		pDest[d1+4]=pSrc[(height-1)*width+j+1];
		pDest[d1+5]=pSrc[(height-2)*width+j+1];
	}

	for(int i=1;i<height-1;i+=2)
	{
		//Gr
		pDest[i*width*3]=pSrc[i*width+1];
		pDest[i*width*3+1]=pSrc[i*width];
		pDest[i*width*3+2]= (pSrc[(i-1)*width]+pSrc[(i+1)*width])/2;
		//R
		pDest[(i+1)*width*3]=(pSrc[i*width+1]+ pSrc[(i+2)*width+1])/2;
		pDest[(i+1)*width*3+1]=(pSrc[i*width]+pSrc[(i+2)*width]+ pSrc[(i+1)*width+1])/3;
		pDest[(i+1)*width*3+2]=pSrc[(i+1)*width];
		//B
		pDest[((i+1)*width-1)*3]=pSrc[(i+1)*width-1];
		pDest[((i+1)*width-1)*3+1]=(pSrc[(i)*width-1]+pSrc[(i+2)*width-1]+pSrc[(i+1)*width-2])/3;
		pDest[((i+1)*width-1)*3+2]=(pSrc[(i)*width-2]+ pSrc[(i+2)*width-2])/2;
		//GB
		pDest[((i+2)*width-1)*3]=(pSrc[(i+1)*width-1]+pSrc[(i+3)*width-1])/2;
		pDest[((i+2)*width-1)*3+1]=pSrc[(i+2)*width-1];
		pDest[((i+2)*width-1)*3+2]=pSrc[(i+2)*width-2];
	}
	//R
	pDest[0]=pSrc[width+1];
	pDest[1]=(pSrc[width]+pSrc[1])/2;
	pDest[2]=pSrc[0];
	//GR
	pDest[3*(width-1)]=pSrc[width-1+width];
	pDest[3*(width-1)+1]=pSrc[width-1];
	pDest[3*(width-1)+2]=pSrc[width-2];
	//GB
	pDest[(height-1)*3*(width)]=pSrc[(height-1)*width+1];
	pDest[(height-1)*3*(width)+1]=pSrc[(height-1)*width];
	pDest[(height-1)*3*(width)+2]=pSrc[(height-2)*width];
	//B
	pDest[(height*width-1)*3]=pSrc[height*width-1];
	pDest[(height*width-1)*3+1]=(pSrc[height*width-1-1]+pSrc[(height-1)*width-1])/2;
	pDest[(height*width-1)*3+2]=pSrc[(height-1)*width-1-1];


	return TRUE;
}
BOOL ImageProc::Interpolation_GRBG(BYTE* pSrc,BYTE *pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//GB
			pDest[(i*width+j)*3]=(pSrc[(i)*width+j-1]+pSrc[(i)*width+j+1])/2;
			pDest[(i*width+j)*3+1]=pSrc[(i)*width+j];
			pDest[(i*width+j)*3+2]=(pSrc[(i-1)*width+j]+ pSrc[(i+1)*width+j])/2;
			//B
			pDest[(i*width+j)*3+3]=pSrc[(i)*width+j+1];
			pDest[(i*width+j)*3+4]=(pSrc[(i-1)*width+j+1]+pSrc[(i+1)*width+j+1]+pSrc[(i)*width+j]+ pSrc[(i)*width+j+2])/4;
			pDest[(i*width+j)*3+5]=(pSrc[(i-1)*width+j]+ pSrc[(i-1)*width+j+2]+ pSrc[(i+1)*width+j]+pSrc[(i+1)*width+j+2])/4;

			//R
			pDest[((i+1)*width+j)*3]=(pSrc[(i)*width+j-1]+ pSrc[(i)*width+j+1]+ pSrc[(i+2)*width+j-1]+ pSrc[(i+2)*width+j+1])/4;
			pDest[((i+1)*width+j)*3+1]=(pSrc[(i)*width+j]+pSrc[(i+2)*width+j]+pSrc[(i+1)*width+j-1]+pSrc[(i+1)*width+j+1])/4;
			pDest[((i+1)*width+j)*3+2]=pSrc[(i+1)*width+j];
			//GR
			pDest[((i+1)*width+j)*3+3]=(pSrc[(i)*width+j+1]+ pSrc[(i+2)*width+j+1])/2;
			pDest[((i+1)*width+j)*3+4]=pSrc[(i+1)*width+j+1];
			pDest[((i+1)*width+j)*3+5]=(pSrc[(i+1)*width+j]+pSrc[(i+1)*width+j+2])/2;

		}
	}

	//边缘插值 上下
	for(int j=1;j<width-1;j+=2)
	{
		int d1=((height-1)*width+j)*3;//最后一行

		//R
		pDest[j*3]=(pSrc[width+j-1]+ pSrc[width+j+1])/2;
		pDest[j*3+1]=(pSrc[width+j]+pSrc[j-1]+ pSrc[j+1])/3;
		pDest[j*3+2]=pSrc[j];
		//Gr
		pDest[j*3+3]= pSrc[width+j+1];
		pDest[j*3+4]=pSrc[j+1];
		pDest[j*3+5]=(pSrc[j]+pSrc[j+2])/2;
		//Gb
		pDest[d1]=(pSrc[(height-1)*width+j-1]+pSrc[(height-1)*width+j+1])/2;
		pDest[d1+1]=pSrc[(height-1)*width+j];
		pDest[d1+2]=pSrc[(height-2)*width+j];
		//B
		pDest[d1+3]=pSrc[(height-1)*width+j+1];
		pDest[d1+4]=(pSrc[(height-2)*width+j+1]+pSrc[(height-1)*width+j+2]+pSrc[(height-1)*width+j])/3;
		pDest[d1+5]=(pSrc[(height-2)*width+j]+ pSrc[(height-2)*width+j+2])/2;

	}
	//左右
	for(int i=1;i<height-1;i+=2)
	{
		//B
		pDest[(i)*width*3]=pSrc[(i)*width];
		pDest[(i)*width*3+1]=(pSrc[(i-1)*width]+pSrc[(i+1)*width]+ pSrc[(i)*width+1])/3;
		pDest[(i)*width*3+2]=(pSrc[(i-1)*width+1]+ pSrc[(i+1)*width+1])/2;
		//Gb
		pDest[(i+1)*width*3]=(pSrc[(i)*width]+pSrc[(i+2)*width])/2;
		pDest[(i+1)*width*3+1]=pSrc[(i+1)*width];
		pDest[(i+1)*width*3+2]= pSrc[(i+1)*width+1];
		//Gr
		pDest[((i+1)*width-1)*3]=pSrc[(i+1)*width-2];
		pDest[((i+1)*width-1)*3+1]=pSrc[(i+1)*width-1];
		pDest[((i+1)*width-1)*3+2]=(pSrc[(i)*width-1]+pSrc[(i+2)*width-1])/2;
		//R
		pDest[((i+2)*width-1)*3]=(pSrc[(i+1)*width-2]+ pSrc[(i+3)*width-2])/2;
		pDest[((i+2)*width-1)*3+1]=(pSrc[(i+1)*width-1]+pSrc[(i+3)*width-1]+pSrc[(i+2)*width-2])/3;
		pDest[((i+2)*width-1)*3+2]=pSrc[(i+2)*width-1];

	}	
	//Gr
	pDest[0]=pSrc[width];
	pDest[1]=pSrc[0];
	pDest[2]=pSrc[1];
	//R
	pDest[3*(width-1)]=pSrc[2*width-2];
	pDest[3*(width-1)+1]=(pSrc[width-2]+pSrc[2*width-1])/2;
	pDest[3*(width-1)+2]=pSrc[width-1];
	//B
	pDest[(height-1)*(width)*3]=pSrc[(height-1)*width];
	pDest[(height-1)*(width)*3+1]=(pSrc[(height-1)*width+1]+pSrc[(height-2)*width])/2;
	pDest[(height-1)*(width)*3+2]=pSrc[(height-2)*width+1];
	//Gb
	pDest[(height*width-1)*3]=pSrc[height*width-2];
	pDest[(height*width-1)*3+1]=pSrc[height*width-1];
	pDest[(height*width-1)*3+2]=pSrc[(height-1)*width-1];

	return TRUE;
}

BOOL ImageProc::RawtoRGB24(USHORT *pSrc,BYTE* pDest, int width, int height,BYTE outformat,BYTE bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	if (bits < 8)
	{
		return FALSE;
	}

	switch(outformat)
	{
	case IMG_OUTFORMAT_BGGR:
		Interpolation_BGGR(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_GRBG:
		Interpolation_GRBG(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_RGGB:
		Interpolation_RGGB(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_GBRG:
		Interpolation_GBRG(pSrc,pDest,width,height,bits);
		break;
	}	

	return TRUE;
}

BOOL ImageProc::Interpolation_BGGR(USHORT* pSrc,BYTE *pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//R
			pDest[(i*width+j)*3  ] = BYTE((pSrc[(i-1)*width + j-1] + pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+1] = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width + j  ] + pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+2] = BYTE( pSrc[(i  )*width + j  ] >>bits);
			//GR
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			pDest[(i*width+j)*3+4] = BYTE( pSrc[(i  )*width + j+1] >>bits);
			pDest[(i*width+j)*3+5] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2])/2 >>bits);
			//GB
			pDest[((i+1)*width+j)*3  ] = BYTE((pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE( pSrc[(i+1)*width + j  ] >>bits);
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width + j  ])/2 >>bits);
			//B
			pDest[((i+1)*width+j)*3+3] = BYTE( pSrc[(i+1)*width + j+1] >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+5] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2] + pSrc[(i+2)*width + j] + pSrc[(i+2)*width + j+2])/4 >>bits);
		}
	}

	//边缘插值
	for(int j=1;j<width-1;j+=2)
	{
		int d1=((height-1)*width+j)*3;
		//Gb
		pDest[j*3]   = BYTE((pSrc[j-1] + pSrc[j+1])/2 >>bits);
		pDest[j*3+1] = BYTE( pSrc[j  ] >>bits);
		pDest[j*3+2] = BYTE( pSrc[width+j] >>bits);
		//B
		pDest[j*3+3] = BYTE( pSrc[      j+1] >>bits);
		pDest[j*3+4] = BYTE((pSrc[width+j+1] + pSrc[      j  ] + pSrc[j+2])/3 >>bits);
		pDest[j*3+5] = BYTE((pSrc[width+j  ] + pSrc[width+j+2])/2 >>bits);
		//R
		pDest[d1]   = BYTE((pSrc[(height-2)*width + j-1] + pSrc[(height-2)*width + j+1])/2 >>bits);
		pDest[d1+1] = BYTE((pSrc[(height-2)*width + j  ] + pSrc[(height-1)*width + j+1] + pSrc[(height-1)*width + j-1])/3 >>bits);
		pDest[d1+2] = BYTE( pSrc[(height-1)*width + j  ] >>bits);
		//Gr
		pDest[d1+3] = BYTE( pSrc[(height-2)*width + j+1] >>bits);
		pDest[d1+4] = BYTE( pSrc[(height-1)*width + j+1] >>bits);
		pDest[d1+5] = BYTE((pSrc[(height-1)*width + j]+pSrc[(height-1)*width+j+2])/2 >>bits);

	}

	for(int i=1;i<height-1;i+=2)
	{
		//Gb
		pDest[i*width*3]   = BYTE((pSrc[(i-1)*width]+pSrc[(i+1)*width])/2 >>bits);
		pDest[i*width*3+1] = BYTE( pSrc[i*width] >>bits);
		pDest[i*width*3+2] = BYTE( pSrc[i*width+1] >>bits);
		//B
		pDest[(i+1)*width*3]   = BYTE( pSrc[(i+1)*width] >>bits);
		pDest[(i+1)*width*3+1] = BYTE((pSrc[i*width] + pSrc[(i+2)*width] + pSrc[(i+1)*width+1])/3 >>bits);
		pDest[(i+1)*width*3+2] = BYTE((pSrc[i*width+1] + pSrc[(i+2)*width+1])/2 >>bits);
		//R
		pDest[((i+1)*width-1)*3]   = BYTE((pSrc[(i )*width-2] + pSrc[(i+2)*width-2])/2 >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE((pSrc[(i )*width-1] + pSrc[(i+2)*width-1] + pSrc[(i+1)*width-2])/3 >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE( pSrc[(i+1)*width-1] >>bits);
		//Gr
		pDest[((i+2)*width-1)*3]   = BYTE( pSrc[(i+2)*width-2] >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1])/2 >>bits);
	}
	//B
	pDest[0] = BYTE( pSrc[0] >>bits);
	pDest[1] = BYTE((pSrc[width] + pSrc[1])/2 >>bits);
	pDest[2] = BYTE( pSrc[width+1] >>bits);
	//GB
	pDest[3*(width-1)]   = BYTE(pSrc[width-2] >>bits);
	pDest[3*(width-1)+1] = BYTE(pSrc[width-1] >>bits);
	pDest[3*(width-1)+2] = BYTE(pSrc[width-1+width] >>bits);
	//GR
	pDest[(height-1)*3*(width)]   = BYTE(pSrc[(height-2)*width] >>bits);
	pDest[(height-1)*3*(width)+1] = BYTE(pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*3*(width)+2] = BYTE(pSrc[(height-1)*width+1] >>bits);
	//R
	pDest[(height*width-1)*3]   = BYTE( pSrc[(height-1)*width-1-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE((pSrc[height*width-1-1] + pSrc[(height-1)*width-1])/2 >>bits);
	pDest[(height*width-1)*3+2] = BYTE( pSrc[height*width-1] >>bits);

	return TRUE;

}

BOOL ImageProc::Interpolation_RGGB(USHORT* pSrc,BYTE *pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//B
			pDest[(i*width+j)*3]   = BYTE( pSrc[(i  )*width+j  ] >>bits);
			pDest[(i*width+j)*3+1] = BYTE((pSrc[(i-1)*width+j  ] + pSrc[(i+1)*width + j  ] + pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+2] = BYTE((pSrc[(i-1)*width+j-1] + pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			//GB
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2])/2 >>bits);
			pDest[(i*width+j)*3+4] = BYTE( pSrc[(i  )*width + j+1] >>bits);
			pDest[(i*width+j)*3+5] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			//GR
			pDest[((i+1)*width+j)*3]   = BYTE((pSrc[(i  )*width + j  ]+ pSrc[(i+2)*width + j])/2 >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE( pSrc[(i+1)*width + j  ] >>bits);
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i+1)*width + j-1]+pSrc[(i+1)*width+j+1])/2 >>bits);
			//R
			pDest[((i+1)*width+j)*3+3] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width+j+2] + pSrc[(i+2)*width + j] + pSrc[(i+2)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width+j+1] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+5] = BYTE( pSrc[(i+1)*width + j+1] >>bits);
		}
	}

	//边缘插值
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;
		//Gr
		pDest[j*3]   = BYTE( pSrc[width+j] >>bits);
		pDest[j*3+1] = BYTE( pSrc[j] >>bits);
		pDest[j*3+2] = BYTE((pSrc[j-1] + pSrc[j+1])/2 >>bits);
		//R
		pDest[j*3+3] = BYTE((pSrc[width+j  ] + pSrc[width+j+2])/2 >>bits);
		pDest[j*3+4] = BYTE((pSrc[width+j+1] + pSrc[j]+ pSrc[j+2])/3 >>bits);
		pDest[j*3+5] = BYTE( pSrc[j+1] >>bits);
		//B
		pDest[d1]   = BYTE( pSrc[(height-1)*width+j] >>bits);
		pDest[d1+1] = BYTE((pSrc[(height-2)*width+j  ] + pSrc[(height-1)*width+j+1] + pSrc[(height-1)*width+j-1])/3 >>bits);
		pDest[d1+2] = BYTE((pSrc[(height-2)*width+j-1] + pSrc[(height-2)*width+j+1])/2 >>bits);
		//Gb
		pDest[d1+3] = BYTE((pSrc[(height-1)*width+j] + pSrc[(height-1)*width+j+2])/2 >>bits);
		pDest[d1+4] = BYTE( pSrc[(height-1)*width+j+1] >>bits);
		pDest[d1+5] = BYTE( pSrc[(height-2)*width+j+1] >>bits);

	}

	for(int i=1;i<height-1;i+=2)
	{
		//Gr
		pDest[i*width*3]   = BYTE( pSrc[i*width+1] >>bits);
		pDest[i*width*3+1] = BYTE( pSrc[i*width] >>bits);
		pDest[i*width*3+2] = BYTE((pSrc[(i-1)*width]+pSrc[(i+1)*width])/2 >>bits);
		//R
		pDest[(i+1)*width*3]   = BYTE((pSrc[i*width+1] + pSrc[(i+2)*width+1])/2 >>bits);
		pDest[(i+1)*width*3+1] = BYTE((pSrc[i*width  ] + pSrc[(i+2)*width  ] + pSrc[(i+1)*width+1])/3 >>bits);
		pDest[(i+1)*width*3+2] = BYTE( pSrc[(i+1)*width] >>bits);
		//B
		pDest[((i+1)*width-1)*3]   = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE((pSrc[(i)*width-1] + pSrc[(i+2)*width-1] + pSrc[(i+1)*width-2])/3 >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE((pSrc[(i)*width-2] + pSrc[(i+2)*width-2])/2 >>bits);
		//GB
		pDest[((i+2)*width-1)*3]   = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1])/2 >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE( pSrc[(i+2)*width-2] >>bits);
	}
	//R
	pDest[0] = BYTE(pSrc[width+1] >>bits);
	pDest[1] = BYTE((pSrc[width]+pSrc[1])/2 >>bits);
	pDest[2] = BYTE(pSrc[0] >>bits);
	//GR
	pDest[3*(width-1)]   = BYTE(pSrc[width-1+width] >>bits);
	pDest[3*(width-1)+1] = BYTE(pSrc[width-1] >>bits);
	pDest[3*(width-1)+2] = BYTE(pSrc[width-2] >>bits);
	//GB
	pDest[(height-1)*3*(width)]   = BYTE(pSrc[(height-1)*width+1] >>bits);
	pDest[(height-1)*3*(width)+1] = BYTE(pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*3*(width)+2] = BYTE(pSrc[(height-2)*width] >>bits);
	//B
	pDest[(height*width-1)*3]   = BYTE( pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE((pSrc[height*width-1-1] + pSrc[(height-1)*width-1])/2 >>bits);
	pDest[(height*width-1)*3+2] = BYTE( pSrc[(height-1)*width-1-1] >>bits);

	return TRUE;
}

BOOL ImageProc::Interpolation_GRBG(USHORT* pSrc,BYTE *pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//GB
			pDest[(i*width+j)*3]   =  BYTE((pSrc[(i  )*width + j-1] + pSrc[(i)*width+j + 1])/2 >>bits);
			pDest[(i*width+j)*3+1] =  BYTE( pSrc[(i  )*width + j  ] >>bits);
			pDest[(i*width+j)*3+2] =  BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width + j])/2 >>bits);
			//B
			pDest[(i*width+j)*3+3] =  BYTE( pSrc[(i  )*width + j+1] >>bits);
			pDest[(i*width+j)*3+4] =  BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1] + pSrc[(i  )*width + j] + pSrc[(i  )*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+5] =  BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i-1)*width + j+2] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);

			//R
			pDest[((i+1)*width+j)*3]   =  BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j-1] + pSrc[(i+2)*width + j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+1] =  BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width + j  ] + pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+2] =  BYTE( pSrc[(i+1)*width + j  ] >>bits);
			//GR
			pDest[((i+1)*width+j)*3+3] =  BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1])/2 >>bits);
			pDest[((i+1)*width+j)*3+4] =  BYTE( pSrc[(i+1)*width + j+1] >>bits);
			pDest[((i+1)*width+j)*3+5] =  BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i+1)*width + j+2])/2 >>bits);

		}
	}

	//边缘插值 上下
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;//最后一行

		//R
		pDest[j*3]   =  BYTE((pSrc[width+j-1]+ pSrc[width+j+1])/2 >>bits);
		pDest[j*3+1] =  BYTE((pSrc[width+j ] + pSrc[j-1] + pSrc[j+1])/3 >>bits);
		pDest[j*3+2] =  BYTE( pSrc[j] >>bits);
		//Gr
		pDest[j*3+3] =  BYTE( pSrc[width+j+1] >>bits);
		pDest[j*3+4] =  BYTE( pSrc[j+1] >>bits);
		pDest[j*3+5] =  BYTE((pSrc[j] + pSrc[j+2])/2 >>bits);
		//Gb
		pDest[d1]   =  BYTE((pSrc[(height-1)*width + j-1] + pSrc[(height-1)*width+j+1])/2 >>bits);
		pDest[d1+1] =  BYTE( pSrc[(height-1)*width + j] >>bits);
		pDest[d1+2] =  BYTE( pSrc[(height-2)*width + j] >>bits);
		//B
		pDest[d1+3] =  BYTE( pSrc[(height-1)*width + j+1] >>bits);
		pDest[d1+4] =  BYTE((pSrc[(height-2)*width + j+1] + pSrc[(height-1)*width + j+2] + pSrc[(height-1)*width+j])/3 >>bits);
		pDest[d1+5] =  BYTE((pSrc[(height-2)*width + j]   + pSrc[(height-2)*width + j+2])/2 >>bits);

	}
	//左右
	for(int i=1;i<height-1;i+=2)
	{
		//B
		pDest[(i)*width*3]   = BYTE( pSrc[(i  )*width] >>bits);
		pDest[(i)*width*3+1] = BYTE((pSrc[(i-1)*width  ] + pSrc[(i+1)*width] + pSrc[(i)*width+1])/3 >>bits);
		pDest[(i)*width*3+2] = BYTE((pSrc[(i-1)*width+1] + pSrc[(i+1)*width+1])/2 >>bits);
		//Gb
		pDest[(i+1)*width*3]   = BYTE((pSrc[(i  )*width  ] + pSrc[(i+2)*width])/2 >>bits);
		pDest[(i+1)*width*3+1] = BYTE( pSrc[(i+1)*width  ] >>bits);
		pDest[(i+1)*width*3+2] = BYTE( pSrc[(i+1)*width+1] >>bits);
		//Gr
		pDest[((i+1)*width-1)*3]   = BYTE( pSrc[(i+1)*width-2] >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE((pSrc[(i  )*width-1] + pSrc[(i+2)*width-1])/2 >>bits);
		//R
		pDest[((i+2)*width-1)*3] =   BYTE((pSrc[(i+1)*width-2] + pSrc[(i+3)*width-2])/2 >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1] + pSrc[(i+2)*width-2])/3 >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE( pSrc[(i+2)*width-1] >>bits);

	}	
	//Gr
	pDest[0] = BYTE(pSrc[width] >>bits);
	pDest[1] = BYTE(pSrc[0] >>bits);
	pDest[2] = BYTE(pSrc[1] >>bits);
	//R
	pDest[3*(width-1)] =   BYTE( pSrc[2*width-2] >>bits);
	pDest[3*(width-1)+1] = BYTE((pSrc[width-2] + pSrc[2*width-1])/2 >>bits);
	pDest[3*(width-1)+2] = BYTE( pSrc[width-1] >>bits);
	//B
	pDest[(height-1)*(width)*3] =   BYTE( pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*(width)*3+1] = BYTE((pSrc[(height-1)*width+1] + pSrc[(height-2)*width])/2 >>bits);
	pDest[(height-1)*(width)*3+2] = BYTE( pSrc[(height-2)*width+1] >>bits);
	//Gb
	pDest[(height*width-1)*3] =   BYTE(pSrc[height*width-2] >>bits);
	pDest[(height*width-1)*3+1] = BYTE(pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+2] = BYTE(pSrc[(height-1)*width-1] >>bits);

	return TRUE;
}

BOOL ImageProc::Interpolation_GBRG(USHORT* pSrc,BYTE *pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//Gr
			pDest[(i*width+j)*3]   = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width+j  ])/2 >>bits);
			pDest[(i*width+j)*3+1] = BYTE( pSrc[(i  )*width + j  ] >>bits);
			pDest[(i*width+j)*3+2] = BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width+j+1])/2 >>bits);
			//R
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i-1)*width+j+2] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+4] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width+j+1] + pSrc[(i  )*width + j] + pSrc[(i  )*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+5] = BYTE( pSrc[(i  )*width + j+1] >>bits);

			//B
			pDest[((i+1)*width+j)*3]   = BYTE( pSrc[(i+1)*width + j  ] >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width+j  ] + pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width+j+1] + pSrc[(i+2)*width + j-1] + pSrc[(i+2)*width + j+1])/4 >>bits);
			//Gb
			pDest[((i+1)*width+j)*3+3] = BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i+1)*width + j+2])/2 >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE( pSrc[(i+1)*width + j+1] >>bits);
			pDest[((i+1)*width+j)*3+5] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1])/2 >>bits);

		}
	}

	//边缘插值 上下
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;//最后一行

		//B
		pDest[j*3] =   BYTE( pSrc[j] >>bits);
		pDest[j*3+1] = BYTE((pSrc[width+j] + pSrc[j-1] + pSrc[j+1])/3 >>bits);
		pDest[j*3+2] = BYTE((pSrc[width+j-1] + pSrc[width+j+1])/2 >>bits);
		//Gb
		pDest[j*3+3] = BYTE((pSrc[j] + pSrc[j+2])/2 >>bits);
		pDest[j*3+4] = BYTE( pSrc[j+1] >>bits);
		pDest[j*3+5] = BYTE( pSrc[width+j+1] >>bits);
		//Gr
		pDest[d1] =   BYTE( pSrc[(height-2)*width + j] >>bits);
		pDest[d1+1] = BYTE( pSrc[(height-1)*width + j] >>bits);
		pDest[d1+2] = BYTE((pSrc[(height-1)*width + j-1] + pSrc[(height-1)*width+j+1])/2 >>bits);
		//R
		pDest[d1+3] = BYTE((pSrc[(height-2)*width+j  ] + pSrc[(height-2)*width+j+2])/2 >>bits);
		pDest[d1+4] = BYTE((pSrc[(height-2)*width+j+1] + pSrc[(height-1)*width+j+2] + pSrc[(height-1)*width+j])/3 >>bits);
		pDest[d1+5] = BYTE( pSrc[(height-1)*width+j+1] >>bits);

	}
	//左右
	for(int i=1;i<height-1;i+=2)
	{
		//R
		pDest[(i)*width*3] =   BYTE((pSrc[(i-1)*width + 1]+ pSrc[(i+1)*width + 1])/2 >>bits);
		pDest[(i)*width*3+1] = BYTE((pSrc[(i-1)*width] + pSrc[(i+1)*width] + pSrc[(i)*width+1])/3 >>bits);
		pDest[(i)*width*3+2] = BYTE( pSrc[(i)*width] >>bits);
		//Gb
		pDest[(i+1)*width*3]   = BYTE( pSrc[(i+1)*width+1] >>bits);
		pDest[(i+1)*width*3+1] = BYTE( pSrc[(i+1)*width] >>bits);
		pDest[(i+1)*width*3+2] = BYTE((pSrc[(i  )*width] + pSrc[(i+2)*width])/2 >>bits);
		//Gr
		pDest[((i+1)*width-1)*3]   = BYTE((pSrc[(i)*width-1] + pSrc[(i+2)*width-1])/2 >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE( pSrc[(i+1)*width-2] >>bits);
		//B
		pDest[((i+2)*width-1)*3]   = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1] + pSrc[(i+2)*width-2])/3 >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE((pSrc[(i+1)*width-2] + pSrc[(i+3)*width-2])/2 >>bits);

	}	
	//Gb
	pDest[0] = BYTE(pSrc[1] >>bits);
	pDest[1] = BYTE(pSrc[0] >>bits);
	pDest[2] = BYTE(pSrc[width] >>bits);
	//B
	pDest[3*(width-1)]   = BYTE( pSrc[width-1] >>bits);
	pDest[3*(width-1)+1] = BYTE((pSrc[width-2] + pSrc[2*width-1])/2 >>bits);
	pDest[3*(width-1)+2] = BYTE( pSrc[2*width-2] >>bits);
	//R
	pDest[(height-1)*(width)*3]   = BYTE( pSrc[(height-2)*width+1] >>bits);
	pDest[(height-1)*(width)*3+1] = BYTE((pSrc[(height-1)*width+1] + pSrc[(height-2)*width])/2 >>bits);
	pDest[(height-1)*(width)*3+2] = BYTE( pSrc[(height-1)*width] >>bits);
	//Gr
	pDest[(height*width-1)*3]   = BYTE(pSrc[(height-1)*width-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE(pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+2] = BYTE(pSrc[height*width-2] >>bits);

	return TRUE;
}



int ImageProc::GetStandardDeviation(BYTE* pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;

	//计算区域的灰度
	float sum=0;
	int count=0;
	float ave=GetAveY_ROI(pSrc,width,height,rect);
	for(int i=rect.top;i<rect.bottom;i++)
	{
		for(int j = rect.left;j<rect.right;j++)
		{
			sum+=(pSrc[i*width+j]-ave)*(pSrc[i*width+j]-ave);
			count++;
		}
	}
	int sd=(int)sqrt(double(sum/count));
	return sd;
}

BOOL ImageProc::AverageFrameData(BYTE* pSrc, int width, int height, int framelevel)
{
	if (!pSrc) return 0;


	static int frameCount = 0;
	static int* tempbuf = NULL;

	if (tempbuf==NULL)
	{
		tempbuf = new int[width*height];
		memset(tempbuf,0,sizeof(int)*width*height);
	}

	if (frameCount<framelevel)
	{
		for(int i=0;i<width*height;i++)
		{
			tempbuf[i]+=pSrc[i];
		}
		frameCount++;
		return FALSE;
	}
	else
	{
		for(int i=0;i<width*height;i++)
		{
			pSrc[i]=tempbuf[i]/framelevel;

		}
		frameCount=0;
		delete [] tempbuf;
		tempbuf = NULL;
		return TRUE;
	}
}
BOOL ImageProc::AverageFrameData(USHORT* pSrc, int width, int height, int framelevel)
{
	if (!pSrc) return 0;


	static int frameCount = 0;
	static int* tempbuf = NULL;

	if (tempbuf==NULL)
	{
		tempbuf = new int[width*height];
		memset(tempbuf,0,sizeof(int)*width*height);
	}

	if (frameCount<framelevel)
	{
		for(int i=0;i<width*height;i++)
		{
			tempbuf[i]+=pSrc[i];
		}
		frameCount++;
		return FALSE;
	}
	else
	{
		for(int i=0;i<width*height;i++)
		{
			pSrc[i]=tempbuf[i]/framelevel;

		}
		frameCount=0;
		delete [] tempbuf;
		tempbuf = NULL;
		return TRUE;
	}
}

BOOL ImageProc::AverageFrameData_RGB24(BYTE* pSrc, int width, int height, int framelevel)
{
	if (!pSrc) return 0;

	static int frameCount = 0;
	static int* tempbuf = NULL;
	if (tempbuf==NULL)
	{
		tempbuf = new int[width*height*3];
		memset(tempbuf,0,sizeof(int)*width*height*3);
	}

	if (frameCount<framelevel)
	{
		for(int i=0;i<width*height*3;i++)
		{
			tempbuf[i]+=pSrc[i];
		}
		frameCount++;
		return FALSE;
	}
	else
	{
		for(int i=0; i<width*height*3; i++)
		{
			pSrc[i] = tempbuf[i]/framelevel;
		}
		frameCount=0;
		delete [] tempbuf;
		tempbuf = NULL;
		return TRUE;
	}
}


double ImageProc::GetResolution_FV(BYTE* pSrc, int width, int height,CRect rect)
{
	if (!pSrc) return 0;

	double Row_sum = 0;
	double Col_sum = 0;
	double sum     = 0;
	double count   = 0;
	for(int i=rect.top+1;i<rect.bottom;i++)   
	{
		for(int j=rect.left+1;j<rect.right;j++)
		{
			Row_sum += (pSrc[i*width+j] - pSrc[i*width+j-1]) * (pSrc[i*width+j] - pSrc[i*width+j-1]);
			Col_sum += (pSrc[i*width+j] - pSrc[(i-1)*width+j]) * (pSrc[i*width+j] - pSrc[(i-1)*width+j]);
			count++;
		}
	}
	if (count == 0) return 0;
	sum = Row_sum + Col_sum;
	return sum*100 / count;
}

BOOL ImageProc::RGB24toRaw(BYTE *pSrc,USHORT* pDest, int width, int height, BYTE outformat,BYTE bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	if (bits < 8)
	{
		return FALSE;
	}

	switch(outformat)
	{
	case IMG_OUTFORMAT_BGGR:
		RGB24toRaw_BGGR(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_GRBG:
		RGB24toRaw_GRBG(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_RGGB:
		RGB24toRaw_RGGB(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_GBRG:
		RGB24toRaw_GBRG(pSrc,pDest,width,height,bits);
		break;
	}	

	return TRUE;
}

BOOL ImageProc::RGB24toRaw_BGGR(BYTE * pSrc,USHORT*pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for (int i = 0; i < height; i += 2)
	{
		for (int j = 0; j < width; j += 2)
		{
			//B
			pDest[i*width + j]        = pSrc[i*width*3 + j*3] << bits;
			//Gb
			pDest[i*width + (j+1)]    = pSrc[i*width*3 + (j+1)*3 + 1] << bits;
			//Gr
			pDest[(i+1)*width + j]    = pSrc[(i+1)*width*3 + j*3 + 1] << bits;
			//R
			pDest[(i+1)*width +(j+1)] = pSrc[(i+1)*width*3 + (j+1)*3 + 2] << bits;
		}
	}
	return TRUE;
}
BOOL ImageProc::RGB24toRaw_GRBG(BYTE * pSrc,USHORT*pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for (int i = 0; i < height; i += 2)
	{
		for (int j = 0; j < width; j+=2)
		{
			//Gr
			pDest[i*width + j] = pSrc[i*width*3 + j*3 + 1] << bits;
			//R
			pDest[i*width + (j+1)] = pSrc[i*width*3 + (j+1)*3 + 2] << bits;
			//B
			pDest[(i+1)*width + j] = pSrc[(i+1)*width*3 +j*3] << bits;
			//Gb
			pDest[(i+1)*width +(j+1)] = pSrc[(i+1)*width*3 + (j+1)*3 + 1] << bits;
		}
	}
	return TRUE;
}
BOOL ImageProc::RGB24toRaw_GBRG(BYTE * pSrc,USHORT*pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for (int i = 0; i < height; i += 2)
	{
		for (int j = 0; j < width; j += 2)
		{
			//Gb
			pDest[i*width + j] = pSrc[i*width*3 + j*3 + 1] << bits;
			//B
			pDest[i*width + (j+1)] = pSrc[i*width*3 + (j+1)*3] << bits;
			//R
			pDest[(i+1)*width + j] = pSrc[(i+1)*width*3 +j*3 + 2] << bits;
			//Gr
			pDest[(i+1)*width +(j+1)] = pSrc[(i+1)*width*3 + (j+1)*3 + 1] << bits;
		}
	}
	return TRUE;
}
BOOL ImageProc::RGB24toRaw_RGGB(BYTE * pSrc,USHORT* pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for (int i = 0; i < height; i += 2)
	{
		for (int j = 0; j < width; j += 2)
		{
			//R
			pDest[i*width + j] = pSrc[i*width*3 + j*3 + 2] << bits;
			//Gr
			pDest[i*width + (j+1)] = pSrc[i*width*3 + (j+1)*3 + 1] << bits;
			//Gb
			pDest[(i+1)*width + j] = pSrc[(i+1)*width*3 +j*3 + 1] << bits;
			//B
			pDest[(i+1)*width +(j+1)] = pSrc[(i+1)*width*3 + (j+1)*3] << bits;
		}
	}
	return TRUE;
}


// #define RR(Y, U, V)   CLIP((298 * ((Y))                   + 409 * ((V)-128) +128) >> 8)
// #define GG(Y, U, V)   CLIP((298 * ((Y)) - 100 * ((U)-128) - 208 * ((V)-128) +128) >> 8)
// #define BB(Y, U, V)   CLIP((298 * ((Y)) + 516 * ((U)-128)                   +128) >> 8)

#define RR(Y, U, V)   CLIP((256 * ((Y))                   + 351 * ((V)-128) + 128) >> 8)
#define GG(Y, U, V)   CLIP((256 * ((Y)) -  86 * ((U)-128) - 179 * ((V)-128) + 128) >> 8)
#define BB(Y, U, V)   CLIP((256 * ((Y)) + 443 * ((U)-128)                   + 128) >> 8)


BOOL ImageProc::YUV422toRGB24(BYTE* pSrc, BYTE* pDest, int width, int height, int outformat)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	switch(outformat)
	{
	case IMG_OUTFORMAT_YCbYCr:
		YUV422toRGB24_YCbYCr(pSrc,pDest,width,height);
		break;
	case IMG_OUTFORMAT_YCrYCb:
		YUV422toRGB24_YCrYCb(pSrc,pDest,width,height);
		break;
	case IMG_OUTFORMAT_CrYCbY:
		YUV422toRGB24_CrYCbY(pSrc,pDest,width,height);
		break;
	case IMG_OUTFORMAT_CbYCrY:
		YUV422toRGB24_CbYCrY(pSrc,pDest,width,height);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


BOOL ImageProc::YUV422toRGB24_YCbYCr(BYTE* pSrc, BYTE* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	int ScrPitch = width*2;
	int DestPitch = width*3;
	BYTE* src = NULL;
	BYTE* dst = NULL;
	BYTE y = 0, u = 0, v = 0;
	for (int i = 0; i < height; i ++)
	{
		src  = pSrc + i*ScrPitch;
		dst  = pDest + i*DestPitch;
		for (int j = 0; j < width; j+=2)
		{
			y = *src;
			u = *(src+1);
			v = *(src+3);

			*dst++ = BB(y, u, v);//b
			*dst++ = GG(y, u, v);//g
			*dst++ = RR(y, u, v);//r

			y = *(src+2);

			*dst++ = BB(y, u, v);//b
			*dst++ = GG(y, u, v);//g
			*dst++ = RR(y, u, v);//r

			src += 4;
		}
	}
	return TRUE;
}

BOOL ImageProc::YUV422toRGB24_YCrYCb(BYTE* pSrc, BYTE* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	int ScrPitch = width*2;
	int DestPitch = width*3;
	BYTE* src = NULL;
	BYTE* dst = NULL;
	BYTE y = 0, u = 0, v = 0;
	for (int i = 0; i < height; i ++)
	{
		src  = pSrc + i*ScrPitch;
		dst  = pDest + i*DestPitch;
		for (int j = 0; j < width; j+=2)
		{
			y = *src;
			u = *(src+3);
			v = *(src+1);

			*dst++ = BB(y, u, v);//b
			*dst++ = GG(y, u, v);//g
			*dst++ = RR(y, u, v);//r

			y = *(src+2);

			*dst++ = BB(y, u, v);//b
			*dst++ = GG(y, u, v);//g
			*dst++ = RR(y, u, v);//r

			src += 4;
		}
	}
	return TRUE;
}

BOOL ImageProc::YUV422toRGB24_CbYCrY(BYTE* pSrc, BYTE* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	int ScrPitch = width*2;
	int DestPitch = width*3;
	BYTE* src = NULL;
	BYTE* dst = NULL;
	BYTE y = 0, u = 0, v = 0;
	for (int i = 0; i < height; i ++)
	{
		src  = pSrc + i*ScrPitch;
		dst  = pDest + i*DestPitch;
		for (int j = 0; j < width; j+=2)
		{
			u = *src;
			y = *(src+1);
			v = *(src+2);

			*dst++ = BB(y, u, v);//b
			*dst++ = GG(y, u, v);//g
			*dst++ = RR(y, u, v);//r

			y = *(src+3);

			*dst++ = BB(y, u, v);//b
			*dst++ = GG(y, u, v);//g
			*dst++ = RR(y, u, v);//r

			src += 4;
		}
	}
	return TRUE;
}

BOOL ImageProc::YUV422toRGB24_CrYCbY(BYTE* pSrc, BYTE* pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	int ScrPitch = width*2;
	int DestPitch = width*3;
	BYTE* src = NULL;
	BYTE* dst = NULL;
	BYTE y = 0, u = 0, v = 0;
	for (int i = 0; i < height; i ++)
	{
		src  = pSrc + i*ScrPitch;
		dst  = pDest + i*DestPitch;
		for (int j = 0; j < width; j+=2)
		{
			v = *src;
			y = *(src+1);
			u = *(src+2);

			*dst++ = BB(y, u, v);//b
			*dst++ = GG(y, u, v);//g
			*dst++ = RR(y, u, v);//r

			y = *(src+3);

			*dst++ = BB(y, u, v);//b
			*dst++ = GG(y, u, v);//g
			*dst++ = RR(y, u, v);//r

			src += 4;
		}
	}

	return TRUE;
}

BOOL ImageProc::YUV422toY(BYTE* pSrc, BYTE* pDest, int width, int height,int outformat)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	switch(outformat)
	{
	case IMG_OUTFORMAT_YCbYCr:
	case IMG_OUTFORMAT_YCrYCb:
		for (int i = 0; i < width*height; i++)
		{
			pDest[i] = pSrc[i*2];
		}
		break;		
	case IMG_OUTFORMAT_CrYCbY:
	case IMG_OUTFORMAT_CbYCrY:
		for (int i = 0; i < width*height; i++)
		{
			pDest[i] = pSrc[i*2 + 1];
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;


}

BOOL ImageProc::Sharpen(BYTE* pSrc, BYTE* pDest, int width, int height, int* Template)
{
	if (pSrc == NULL || pDest == NULL)
	{
		return FALSE;
	}

	if(Template != NULL)
		return Template3x3(pSrc, pDest, width, height, Template);

	int temp;
	for (int i = 1; i < height-1; i++)
	{
		for (int j = 1; j < width-1; j++)
		{
			temp = (-2*pSrc[(i-1)*width+(j-1)] +   pSrc[(i-1)*width+j] - 2*pSrc[(i-1)*width+(j+1)] 
			+ pSrc[(i  )*width+(j-1)] + 6*pSrc[(i  )*width+j] +   pSrc[(i  )*width+(j+1)] 
			-2*pSrc[(i+1)*width+(j-1)] +   pSrc[(i+1)*width+j] - 2*pSrc[(i+1)*width+(j+1)])>>1;

			if (temp < 0)  
				pDest[i*width+j] = 0;
			else if (temp > 255) 
				pDest[i*width+j] = 255;
			else
				pDest[i*width+j] = temp;
		}
	}


	return TRUE;
}

BOOL ImageProc::Smooth(BYTE* pSrc, BYTE* pDest, int width, int height, int* Template)
{
	if (pSrc == NULL || pDest == NULL)
	{
		return FALSE;
	}

	if(Template != NULL)
		return Template3x3(pSrc, pDest, width, height, Template);

	int temp;
	for (int i = 1; i < height-1; i++)
	{
		for (int j = 1; j < width-1; j++)
		{
			temp = ( 1*pSrc[(i-1)*width+(j-1)] + 2*pSrc[(i-1)*width+j]  + 1*pSrc[(i-1)*width+(j+1)] 
			+2*pSrc[(i  )*width+(j-1)] + 4*pSrc[(i  )*width+j] + 2*pSrc[(i  )*width+(j+1)] 
			+1*pSrc[(i+1)*width+(j-1)] + 2*pSrc[(i+1)*width+j]  + 1*pSrc[(i+1)*width+(j+1)])>>4;

			if (temp < 0)  
				pDest[i*width+j] = 0;
			else if (temp > 255) 
				pDest[i*width+j] = 255;
			else
				pDest[i*width+j] = temp;
		}
	}

	return TRUE;
}

BOOL ImageProc::Template3x3(BYTE* pSrc, BYTE* pDest, int width, int height,int* Template)
{
	if (pSrc == NULL || pDest == NULL)
	{
		return FALSE;
	}

	int temp = 0;
	int div = 0;
	for (int i = 0; i<9; i++)
	{
		div += Template[i];
	}
	for (int i = 1; i < height-1; i++)
	{
		for (int j = 1; j < width-1; j++)
		{
			temp = ( Template[0]*pSrc[(i-1)*width+(j-1)] + Template[1]*pSrc[(i-1)*width+j]  + Template[2]*pSrc[(i-1)*width+(j+1)] 
			+Template[3]*pSrc[(i  )*width+(j-1)] + Template[4]*pSrc[(i  )*width+j]  + Template[5]*pSrc[(i  )*width+(j+1)] 
			+Template[6]*pSrc[(i+1)*width+(j-1)] + Template[7]*pSrc[(i+1)*width+j]  + Template[8]*pSrc[(i+1)*width+(j+1)] )/div;

			if (temp < 0)  
				pDest[i*width+j] = 0;
			else if (temp > 255) 
				pDest[i*width+j] = 255;
			else
				pDest[i*width+j] = temp;
		}
	}

	return TRUE;
}

BOOL ImageProc::RawtoRGB24_DT(USHORT *pSrc,BYTE* pDest, int width, int height,BYTE outformat,BYTE bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	if (bits < 8)
	{
		return FALSE;
	}

	switch(outformat)
	{
	case IMG_OUTFORMAT_BGGR:
		Interpolation_BGGR_DT(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_GRBG:
		Interpolation_GRBG_DT(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_RGGB:
		Interpolation_RGGB_DT(pSrc,pDest,width,height,bits);
		break;
	case IMG_OUTFORMAT_GBRG:
		Interpolation_GBRG_DT(pSrc,pDest,width,height,bits);
		break;
	}	

	return TRUE;
}

BOOL ImageProc::Interpolation_BGGR_DT(USHORT* pSrc,BYTE *pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//R
			pDest[(i*width+j)*3  ] = BYTE((pSrc[(i-1)*width + j-1] + pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+1] = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width + j  ] + pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+2] = BYTE( pSrc[(i  )*width + j  ] >>bits);
			//GR
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			pDest[(i*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i-1)*width + j+2])/2 >>bits);//DT
			pDest[(i*width+j)*3+5] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2])/2 >>bits);
			//GB
			pDest[((i+1)*width+j)*3  ] = BYTE((pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i  )*width + j+1])/2 >>bits);//DT
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width + j  ])/2 >>bits);
			//B
			pDest[((i+1)*width+j)*3+3] = BYTE( pSrc[(i+1)*width + j+1] >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+5] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2] + pSrc[(i+2)*width + j] + pSrc[(i+2)*width + j+2])/4 >>bits);
		}
	}

	//边缘插值
	for(int j=1;j<width-1;j+=2)
	{
		int d1=((height-1)*width+j)*3;
		//Gb
		pDest[j*3]   = BYTE((pSrc[j-1] + pSrc[j+1])/2 >>bits);
		pDest[j*3+1] = BYTE( pSrc[j  ] >>bits);
		pDest[j*3+2] = BYTE( pSrc[width+j] >>bits);
		//B
		pDest[j*3+3] = BYTE( pSrc[      j+1] >>bits);
		pDest[j*3+4] = BYTE((pSrc[width+j+1] + pSrc[      j  ] + pSrc[j+2])/3 >>bits);
		pDest[j*3+5] = BYTE((pSrc[width+j  ] + pSrc[width+j+2])/2 >>bits);
		//R
		pDest[d1]   = BYTE((pSrc[(height-2)*width + j-1] + pSrc[(height-2)*width + j+1])/2 >>bits);
		pDest[d1+1] = BYTE((pSrc[(height-2)*width + j  ] + pSrc[(height-1)*width + j+1] + pSrc[(height-1)*width + j-1])/3 >>bits);
		pDest[d1+2] = BYTE( pSrc[(height-1)*width + j  ] >>bits);
		//Gr
		pDest[d1+3] = BYTE( pSrc[(height-2)*width + j+1] >>bits);
		pDest[d1+4] = BYTE( pSrc[(height-1)*width + j+1] >>bits);
		pDest[d1+5] = BYTE((pSrc[(height-1)*width + j]+pSrc[(height-1)*width+j+2])/2 >>bits);

	}

	for(int i=1;i<height-1;i+=2)
	{
		//Gb
		pDest[i*width*3]   = BYTE((pSrc[(i-1)*width]+pSrc[(i+1)*width])/2 >>bits);
		pDest[i*width*3+1] = BYTE( pSrc[i*width] >>bits);
		pDest[i*width*3+2] = BYTE( pSrc[i*width+1] >>bits);
		//B
		pDest[(i+1)*width*3]   = BYTE( pSrc[(i+1)*width] >>bits);
		pDest[(i+1)*width*3+1] = BYTE((pSrc[i*width] + pSrc[(i+2)*width] + pSrc[(i+1)*width+1])/3 >>bits);
		pDest[(i+1)*width*3+2] = BYTE((pSrc[i*width+1] + pSrc[(i+2)*width+1])/2 >>bits);
		//R
		pDest[((i+1)*width-1)*3]   = BYTE((pSrc[(i )*width-2] + pSrc[(i+2)*width-2])/2 >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE((pSrc[(i )*width-1] + pSrc[(i+2)*width-1] + pSrc[(i+1)*width-2])/3 >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE( pSrc[(i+1)*width-1] >>bits);
		//Gr
		pDest[((i+2)*width-1)*3]   = BYTE( pSrc[(i+2)*width-2] >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1])/2 >>bits);
	}
	//B
	pDest[0] = BYTE( pSrc[0] >>bits);
	pDest[1] = BYTE((pSrc[width] + pSrc[1])/2 >>bits);
	pDest[2] = BYTE( pSrc[width+1] >>bits);
	//GB
	pDest[3*(width-1)]   = BYTE(pSrc[width-2] >>bits);
	pDest[3*(width-1)+1] = BYTE(pSrc[width-1] >>bits);
	pDest[3*(width-1)+2] = BYTE(pSrc[width-1+width] >>bits);
	//GR
	pDest[(height-1)*3*(width)]   = BYTE(pSrc[(height-2)*width] >>bits);
	pDest[(height-1)*3*(width)+1] = BYTE(pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*3*(width)+2] = BYTE(pSrc[(height-1)*width+1] >>bits);
	//R
	pDest[(height*width-1)*3]   = BYTE( pSrc[(height-1)*width-1-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE((pSrc[height*width-1-1] + pSrc[(height-1)*width-1])/2 >>bits);
	pDest[(height*width-1)*3+2] = BYTE( pSrc[height*width-1] >>bits);

	return TRUE;

}

BOOL ImageProc::Interpolation_RGGB_DT(USHORT* pSrc,BYTE *pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//B
			pDest[(i*width+j)*3]   = BYTE( pSrc[(i  )*width+j  ] >>bits);
			pDest[(i*width+j)*3+1] = BYTE((pSrc[(i-1)*width+j  ] + pSrc[(i+1)*width + j  ] + pSrc[(i  )*width+j-1] + pSrc[(i  )*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+2] = BYTE((pSrc[(i-1)*width+j-1] + pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width+j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			//GB
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2])/2 >>bits);
			pDest[(i*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i-1)*width + j+2])/2 >>bits);//DT
			pDest[(i*width+j)*3+5] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			//GR
			pDest[((i+1)*width+j)*3]   = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width + j  ])/2 >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i  )*width + j+1])/2 >>bits);//DT
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			//R
			pDest[((i+1)*width+j)*3+3] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width+j+2] + pSrc[(i+2)*width + j] + pSrc[(i+2)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width+j+1] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+5] = BYTE( pSrc[(i+1)*width + j+1] >>bits);
		}
	}

	//边缘插值
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;
		//Gr
		pDest[j*3]   = BYTE( pSrc[width+j] >>bits);
		pDest[j*3+1] = BYTE( pSrc[j] >>bits);
		pDest[j*3+2] = BYTE((pSrc[j-1] + pSrc[j+1])/2 >>bits);
		//R
		pDest[j*3+3] = BYTE((pSrc[width+j  ] + pSrc[width+j+2])/2 >>bits);
		pDest[j*3+4] = BYTE((pSrc[width+j+1] + pSrc[j]+ pSrc[j+2])/3 >>bits);
		pDest[j*3+5] = BYTE( pSrc[j+1] >>bits);
		//B
		pDest[d1]   = BYTE( pSrc[(height-1)*width+j] >>bits);
		pDest[d1+1] = BYTE((pSrc[(height-2)*width+j  ] + pSrc[(height-1)*width+j+1] + pSrc[(height-1)*width+j-1])/3 >>bits);
		pDest[d1+2] = BYTE((pSrc[(height-2)*width+j-1] + pSrc[(height-2)*width+j+1])/2 >>bits);
		//Gb
		pDest[d1+3] = BYTE((pSrc[(height-1)*width+j] + pSrc[(height-1)*width+j+2])/2 >>bits);
		pDest[d1+4] = BYTE( pSrc[(height-1)*width+j+1] >>bits);
		pDest[d1+5] = BYTE( pSrc[(height-2)*width+j+1] >>bits);

	}

	for(int i=1;i<height-1;i+=2)
	{
		//Gr
		pDest[i*width*3]   = BYTE( pSrc[i*width+1] >>bits);
		pDest[i*width*3+1] = BYTE( pSrc[i*width] >>bits);
		pDest[i*width*3+2] = BYTE((pSrc[(i-1)*width]+pSrc[(i+1)*width])/2 >>bits);
		//R
		pDest[(i+1)*width*3]   = BYTE((pSrc[i*width+1] + pSrc[(i+2)*width+1])/2 >>bits);
		pDest[(i+1)*width*3+1] = BYTE((pSrc[i*width  ] + pSrc[(i+2)*width  ] + pSrc[(i+1)*width+1])/3 >>bits);
		pDest[(i+1)*width*3+2] = BYTE( pSrc[(i+1)*width] >>bits);
		//B
		pDest[((i+1)*width-1)*3]   = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE((pSrc[(i)*width-1] + pSrc[(i+2)*width-1] + pSrc[(i+1)*width-2])/3 >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE((pSrc[(i)*width-2] + pSrc[(i+2)*width-2])/2 >>bits);
		//GB
		pDest[((i+2)*width-1)*3]   = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1])/2 >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE( pSrc[(i+2)*width-2] >>bits);
	}
	//R
	pDest[0] = BYTE(pSrc[width+1] >>bits);
	pDest[1] = BYTE((pSrc[width]+pSrc[1])/2 >>bits);
	pDest[2] = BYTE(pSrc[0] >>bits);
	//GR
	pDest[3*(width-1)]   = BYTE(pSrc[width-1+width] >>bits);
	pDest[3*(width-1)+1] = BYTE(pSrc[width-1] >>bits);
	pDest[3*(width-1)+2] = BYTE(pSrc[width-2] >>bits);
	//GB
	pDest[(height-1)*3*(width)]   = BYTE(pSrc[(height-1)*width+1] >>bits);
	pDest[(height-1)*3*(width)+1] = BYTE(pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*3*(width)+2] = BYTE(pSrc[(height-2)*width] >>bits);
	//B
	pDest[(height*width-1)*3]   = BYTE( pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE((pSrc[height*width-1-1] + pSrc[(height-1)*width-1])/2 >>bits);
	pDest[(height*width-1)*3+2] = BYTE( pSrc[(height-1)*width-1-1] >>bits);

	return TRUE;
}

BOOL ImageProc::Interpolation_GRBG_DT(USHORT* pSrc,BYTE *pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//GB
			pDest[(i*width+j)*3]   =  BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1])/2 >>bits);
			pDest[(i*width+j)*3+1] =  BYTE((pSrc[(i  )*width + j  ] + pSrc[(i-1)*width + j+1])/2 >>bits); //DT
			pDest[(i*width+j)*3+2] =  BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width + j  ])/2 >>bits);

			//B
			pDest[(i*width+j)*3+3] =  BYTE( pSrc[(i  )*width + j+1] >>bits);
			pDest[(i*width+j)*3+4] =  BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1] + pSrc[(i  )*width+j] + pSrc[(i  )*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+5] =  BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i-1)*width + j+2] + pSrc[(i+1)*width+j] + pSrc[(i+1)*width + j+2])/4 >>bits);

			//R
			pDest[((i+1)*width+j)*3]   =  BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1] + pSrc[(i+2)*width+j-1] + pSrc[(i+2)*width+j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+1] =  BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width + j  ] + pSrc[(i+1)*width+j-1] + pSrc[(i+1)*width+j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+2] =  BYTE( pSrc[(i+1)*width + j  ] >>bits);

			//GR
			pDest[((i+1)*width+j)*3+3] =  BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1])/2 >>bits);
			pDest[((i+1)*width+j)*3+4] =  BYTE((pSrc[(i+1)*width + j+1] + pSrc[(i  )*width + j+2])/2 >>bits);//DT
			pDest[((i+1)*width+j)*3+5] =  BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i+1)*width + j+2])/2 >>bits);

		}
	}

	//边缘插值 上下
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;//最后一行

		//R
		pDest[j*3]   =  BYTE((pSrc[width+j-1]+ pSrc[width+j+1])/2 >>bits);
		pDest[j*3+1] =  BYTE((pSrc[width+j ] + pSrc[j-1] + pSrc[j+1])/3 >>bits);
		pDest[j*3+2] =  BYTE( pSrc[j] >>bits);
		//Gr
		pDest[j*3+3] =  BYTE( pSrc[width+j+1] >>bits);
		pDest[j*3+4] =  BYTE( pSrc[j+1] >>bits);
		pDest[j*3+5] =  BYTE((pSrc[j] + pSrc[j+2])/2 >>bits);
		//Gb
		pDest[d1]   =  BYTE((pSrc[(height-1)*width + j-1] + pSrc[(height-1)*width+j+1])/2 >>bits);
		pDest[d1+1] =  BYTE( pSrc[(height-1)*width + j] >>bits);
		pDest[d1+2] =  BYTE( pSrc[(height-2)*width + j] >>bits);
		//B
		pDest[d1+3] =  BYTE( pSrc[(height-1)*width + j+1] >>bits);
		pDest[d1+4] =  BYTE((pSrc[(height-2)*width + j+1] + pSrc[(height-1)*width + j+2] + pSrc[(height-1)*width+j])/3 >>bits);
		pDest[d1+5] =  BYTE((pSrc[(height-2)*width + j]   + pSrc[(height-2)*width + j+2])/2 >>bits);

	}
	//左右
	for(int i=1;i<height-1;i+=2)
	{
		//B
		pDest[(i)*width*3]   = BYTE( pSrc[(i  )*width] >>bits);
		pDest[(i)*width*3+1] = BYTE((pSrc[(i-1)*width  ] + pSrc[(i+1)*width] + pSrc[(i)*width+1])/3 >>bits);
		pDest[(i)*width*3+2] = BYTE((pSrc[(i-1)*width+1] + pSrc[(i+1)*width+1])/2 >>bits);
		//Gb
		pDest[(i+1)*width*3]   = BYTE((pSrc[(i  )*width  ] + pSrc[(i+2)*width])/2 >>bits);
		pDest[(i+1)*width*3+1] = BYTE( pSrc[(i+1)*width  ] >>bits);
		pDest[(i+1)*width*3+2] = BYTE( pSrc[(i+1)*width+1] >>bits);
		//Gr
		pDest[((i+1)*width-1)*3]   = BYTE( pSrc[(i+1)*width-2] >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE((pSrc[(i  )*width-1] + pSrc[(i+2)*width-1])/2 >>bits);
		//R
		pDest[((i+2)*width-1)*3] =   BYTE((pSrc[(i+1)*width-2] + pSrc[(i+3)*width-2])/2 >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1] + pSrc[(i+2)*width-2])/3 >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE( pSrc[(i+2)*width-1] >>bits);

	}	
	//Gr
	pDest[0] = BYTE(pSrc[width] >>bits);
	pDest[1] = BYTE(pSrc[0] >>bits);
	pDest[2] = BYTE(pSrc[1] >>bits);
	//R
	pDest[3*(width-1)] =   BYTE( pSrc[2*width-2] >>bits);
	pDest[3*(width-1)+1] = BYTE((pSrc[width-2] + pSrc[2*width-1])/2 >>bits);
	pDest[3*(width-1)+2] = BYTE( pSrc[width-1] >>bits);
	//B
	pDest[(height-1)*(width)*3] =   BYTE( pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*(width)*3+1] = BYTE((pSrc[(height-1)*width+1] + pSrc[(height-2)*width])/2 >>bits);
	pDest[(height-1)*(width)*3+2] = BYTE( pSrc[(height-2)*width+1] >>bits);
	//Gb
	pDest[(height*width-1)*3] =   BYTE(pSrc[height*width-2] >>bits);
	pDest[(height*width-1)*3+1] = BYTE(pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+2] = BYTE(pSrc[(height-1)*width-1] >>bits);

	return TRUE;
}

BOOL ImageProc::Interpolation_GBRG_DT(USHORT* pSrc,BYTE *pDest, int width, int height, BYTE Bits)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = Bits - 8;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//Gr
			pDest[(i*width+j)*3]   = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width + j  ])/2 >>bits);
			pDest[(i*width+j)*3+1] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i-1)*width + j+1])/2 >>bits); //DT
			pDest[(i*width+j)*3+2] = BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1])/2 >>bits);
			//R
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i-1)*width+j+2] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+4] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width+j+1] + pSrc[(i  )*width + j] + pSrc[(i  )*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+5] = BYTE( pSrc[(i  )*width + j+1] >>bits);

			//B
			pDest[((i+1)*width+j)*3]   = BYTE( pSrc[(i+1)*width + j  ] >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width+j  ] + pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width+j+1] + pSrc[(i+2)*width + j-1] + pSrc[(i+2)*width + j+1])/4 >>bits);
			//Gb
			pDest[((i+1)*width+j)*3+3] = BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i+1)*width + j+2])/2 >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE((pSrc[(i+1)*width + j+1] + pSrc[(i  )*width + j+2])/2 >>bits);//DT
			pDest[((i+1)*width+j)*3+5] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1])/2 >>bits);

		}
	}

	//边缘插值 上下
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;//最后一行

		//B
		pDest[j*3] =   BYTE( pSrc[j] >>bits);
		pDest[j*3+1] = BYTE((pSrc[width+j] + pSrc[j-1] + pSrc[j+1])/3 >>bits);
		pDest[j*3+2] = BYTE((pSrc[width+j-1] + pSrc[width+j+1])/2 >>bits);
		//Gb
		pDest[j*3+3] = BYTE((pSrc[j] + pSrc[j+2])/2 >>bits);
		pDest[j*3+4] = BYTE( pSrc[j+1] >>bits);
		pDest[j*3+5] = BYTE( pSrc[width+j+1] >>bits);
		//Gr
		pDest[d1] =   BYTE( pSrc[(height-2)*width + j] >>bits);
		pDest[d1+1] = BYTE( pSrc[(height-1)*width + j] >>bits);
		pDest[d1+2] = BYTE((pSrc[(height-1)*width + j-1] + pSrc[(height-1)*width+j+1])/2 >>bits);
		//R
		pDest[d1+3] = BYTE((pSrc[(height-2)*width+j  ] + pSrc[(height-2)*width+j+2])/2 >>bits);
		pDest[d1+4] = BYTE((pSrc[(height-2)*width+j+1] + pSrc[(height-1)*width+j+2] + pSrc[(height-1)*width+j])/3 >>bits);
		pDest[d1+5] = BYTE( pSrc[(height-1)*width+j+1] >>bits);

	}
	//左右
	for(int i=1;i<height-1;i+=2)
	{
		//R
		pDest[(i)*width*3] =   BYTE((pSrc[(i-1)*width + 1]+ pSrc[(i+1)*width + 1])/2 >>bits);
		pDest[(i)*width*3+1] = BYTE((pSrc[(i-1)*width] + pSrc[(i+1)*width] + pSrc[(i)*width+1])/3 >>bits);
		pDest[(i)*width*3+2] = BYTE( pSrc[(i)*width] >>bits);
		//Gb
		pDest[(i+1)*width*3]   = BYTE( pSrc[(i+1)*width+1] >>bits);
		pDest[(i+1)*width*3+1] = BYTE( pSrc[(i+1)*width] >>bits);
		pDest[(i+1)*width*3+2] = BYTE((pSrc[(i  )*width] + pSrc[(i+2)*width])/2 >>bits);
		//Gr
		pDest[((i+1)*width-1)*3]   = BYTE((pSrc[(i)*width-1] + pSrc[(i+2)*width-1])/2 >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE( pSrc[(i+1)*width-2] >>bits);
		//B
		pDest[((i+2)*width-1)*3]   = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1] + pSrc[(i+2)*width-2])/3 >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE((pSrc[(i+1)*width-2] + pSrc[(i+3)*width-2])/2 >>bits);

	}	
	//Gb
	pDest[0] = BYTE(pSrc[1] >>bits);
	pDest[1] = BYTE(pSrc[0] >>bits);
	pDest[2] = BYTE(pSrc[width] >>bits);
	//B
	pDest[3*(width-1)]   = BYTE( pSrc[width-1] >>bits);
	pDest[3*(width-1)+1] = BYTE((pSrc[width-2] + pSrc[2*width-1])/2 >>bits);
	pDest[3*(width-1)+2] = BYTE( pSrc[2*width-2] >>bits);
	//R
	pDest[(height-1)*(width)*3]   = BYTE( pSrc[(height-2)*width+1] >>bits);
	pDest[(height-1)*(width)*3+1] = BYTE((pSrc[(height-1)*width+1] + pSrc[(height-2)*width])/2 >>bits);
	pDest[(height-1)*(width)*3+2] = BYTE( pSrc[(height-1)*width] >>bits);
	//Gr
	pDest[(height*width-1)*3]   = BYTE(pSrc[(height-1)*width-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE(pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+2] = BYTE(pSrc[height*width-2] >>bits);

	return TRUE;
}

BOOL ImageProc::RawtoRGB24_DT(BYTE *pSrc,BYTE* pDest, int width, int height,BYTE outformat)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;
	 
	switch(outformat)
	{
	case IMG_OUTFORMAT_BGGR:
		Interpolation_BGGR_DT(pSrc,pDest,width,height);
		break;
	case IMG_OUTFORMAT_GRBG:
		Interpolation_GRBG_DT(pSrc,pDest,width,height);
		break;
	case IMG_OUTFORMAT_RGGB:
		Interpolation_RGGB_DT(pSrc,pDest,width,height);
		break;
	case IMG_OUTFORMAT_GBRG:
		Interpolation_GBRG_DT(pSrc,pDest,width,height);
		break;
	}	

	return TRUE;
}

BOOL ImageProc::Interpolation_BGGR_DT(BYTE* pSrc,BYTE *pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = 0;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//R
			pDest[(i*width+j)*3  ] = BYTE((pSrc[(i-1)*width + j-1] + pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+1] = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width + j  ] + pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+2] = BYTE( pSrc[(i  )*width + j  ] >>bits);
			//GR
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			pDest[(i*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i-1)*width + j+2])/2 >>bits);//DT
			pDest[(i*width+j)*3+5] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2])/2 >>bits);
			//GB
			pDest[((i+1)*width+j)*3  ] = BYTE((pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i  )*width + j+1])/2 >>bits);//DT
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width + j  ])/2 >>bits);
			//B
			pDest[((i+1)*width+j)*3+3] = BYTE( pSrc[(i+1)*width + j+1] >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+5] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2] + pSrc[(i+2)*width + j] + pSrc[(i+2)*width + j+2])/4 >>bits);
		}
	}

	//边缘插值
	for(int j=1;j<width-1;j+=2)
	{
		int d1=((height-1)*width+j)*3;
		//Gb
		pDest[j*3]   = BYTE((pSrc[j-1] + pSrc[j+1])/2 >>bits);
		pDest[j*3+1] = BYTE( pSrc[j  ] >>bits);
		pDest[j*3+2] = BYTE( pSrc[width+j] >>bits);
		//B
		pDest[j*3+3] = BYTE( pSrc[      j+1] >>bits);
		pDest[j*3+4] = BYTE((pSrc[width+j+1] + pSrc[      j  ] + pSrc[j+2])/3 >>bits);
		pDest[j*3+5] = BYTE((pSrc[width+j  ] + pSrc[width+j+2])/2 >>bits);
		//R
		pDest[d1]   = BYTE((pSrc[(height-2)*width + j-1] + pSrc[(height-2)*width + j+1])/2 >>bits);
		pDest[d1+1] = BYTE((pSrc[(height-2)*width + j  ] + pSrc[(height-1)*width + j+1] + pSrc[(height-1)*width + j-1])/3 >>bits);
		pDest[d1+2] = BYTE( pSrc[(height-1)*width + j  ] >>bits);
		//Gr
		pDest[d1+3] = BYTE( pSrc[(height-2)*width + j+1] >>bits);
		pDest[d1+4] = BYTE( pSrc[(height-1)*width + j+1] >>bits);
		pDest[d1+5] = BYTE((pSrc[(height-1)*width + j]+pSrc[(height-1)*width+j+2])/2 >>bits);

	}

	for(int i=1;i<height-1;i+=2)
	{
		//Gb
		pDest[i*width*3]   = BYTE((pSrc[(i-1)*width]+pSrc[(i+1)*width])/2 >>bits);
		pDest[i*width*3+1] = BYTE( pSrc[i*width] >>bits);
		pDest[i*width*3+2] = BYTE( pSrc[i*width+1] >>bits);
		//B
		pDest[(i+1)*width*3]   = BYTE( pSrc[(i+1)*width] >>bits);
		pDest[(i+1)*width*3+1] = BYTE((pSrc[i*width] + pSrc[(i+2)*width] + pSrc[(i+1)*width+1])/3 >>bits);
		pDest[(i+1)*width*3+2] = BYTE((pSrc[i*width+1] + pSrc[(i+2)*width+1])/2 >>bits);
		//R
		pDest[((i+1)*width-1)*3]   = BYTE((pSrc[(i )*width-2] + pSrc[(i+2)*width-2])/2 >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE((pSrc[(i )*width-1] + pSrc[(i+2)*width-1] + pSrc[(i+1)*width-2])/3 >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE( pSrc[(i+1)*width-1] >>bits);
		//Gr
		pDest[((i+2)*width-1)*3]   = BYTE( pSrc[(i+2)*width-2] >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1])/2 >>bits);
	}
	//B
	pDest[0] = BYTE( pSrc[0] >>bits);
	pDest[1] = BYTE((pSrc[width] + pSrc[1])/2 >>bits);
	pDest[2] = BYTE( pSrc[width+1] >>bits);
	//GB
	pDest[3*(width-1)]   = BYTE(pSrc[width-2] >>bits);
	pDest[3*(width-1)+1] = BYTE(pSrc[width-1] >>bits);
	pDest[3*(width-1)+2] = BYTE(pSrc[width-1+width] >>bits);
	//GR
	pDest[(height-1)*3*(width)]   = BYTE(pSrc[(height-2)*width] >>bits);
	pDest[(height-1)*3*(width)+1] = BYTE(pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*3*(width)+2] = BYTE(pSrc[(height-1)*width+1] >>bits);
	//R
	pDest[(height*width-1)*3]   = BYTE( pSrc[(height-1)*width-1-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE((pSrc[height*width-1-1] + pSrc[(height-1)*width-1])/2 >>bits);
	pDest[(height*width-1)*3+2] = BYTE( pSrc[height*width-1] >>bits);

	return TRUE;

}

BOOL ImageProc::Interpolation_RGGB_DT(BYTE* pSrc,BYTE *pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = 0;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//B
			pDest[(i*width+j)*3]   = BYTE( pSrc[(i  )*width+j  ] >>bits);
			pDest[(i*width+j)*3+1] = BYTE((pSrc[(i-1)*width+j  ] + pSrc[(i+1)*width + j  ] + pSrc[(i  )*width+j-1] + pSrc[(i  )*width + j+1])/4 >>bits);
			pDest[(i*width+j)*3+2] = BYTE((pSrc[(i-1)*width+j-1] + pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width+j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			//GB
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width + j+2])/2 >>bits);
			pDest[(i*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i-1)*width + j+2])/2 >>bits);//DT
			pDest[(i*width+j)*3+5] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			//GR
			pDest[((i+1)*width+j)*3]   = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width + j  ])/2 >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i  )*width + j+1])/2 >>bits);//DT
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/2 >>bits);
			//R
			pDest[((i+1)*width+j)*3+3] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i  )*width+j+2] + pSrc[(i+2)*width + j] + pSrc[(i+2)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width+j+1] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[((i+1)*width+j)*3+5] = BYTE( pSrc[(i+1)*width + j+1] >>bits);
		}
	}

	//边缘插值
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;
		//Gr
		pDest[j*3]   = BYTE( pSrc[width+j] >>bits);
		pDest[j*3+1] = BYTE( pSrc[j] >>bits);
		pDest[j*3+2] = BYTE((pSrc[j-1] + pSrc[j+1])/2 >>bits);
		//R
		pDest[j*3+3] = BYTE((pSrc[width+j  ] + pSrc[width+j+2])/2 >>bits);
		pDest[j*3+4] = BYTE((pSrc[width+j+1] + pSrc[j]+ pSrc[j+2])/3 >>bits);
		pDest[j*3+5] = BYTE( pSrc[j+1] >>bits);
		//B
		pDest[d1]   = BYTE( pSrc[(height-1)*width+j] >>bits);
		pDest[d1+1] = BYTE((pSrc[(height-2)*width+j  ] + pSrc[(height-1)*width+j+1] + pSrc[(height-1)*width+j-1])/3 >>bits);
		pDest[d1+2] = BYTE((pSrc[(height-2)*width+j-1] + pSrc[(height-2)*width+j+1])/2 >>bits);
		//Gb
		pDest[d1+3] = BYTE((pSrc[(height-1)*width+j] + pSrc[(height-1)*width+j+2])/2 >>bits);
		pDest[d1+4] = BYTE( pSrc[(height-1)*width+j+1] >>bits);
		pDest[d1+5] = BYTE( pSrc[(height-2)*width+j+1] >>bits);

	}

	for(int i=1;i<height-1;i+=2)
	{
		//Gr
		pDest[i*width*3]   = BYTE( pSrc[i*width+1] >>bits);
		pDest[i*width*3+1] = BYTE( pSrc[i*width] >>bits);
		pDest[i*width*3+2] = BYTE((pSrc[(i-1)*width]+pSrc[(i+1)*width])/2 >>bits);
		//R
		pDest[(i+1)*width*3]   = BYTE((pSrc[i*width+1] + pSrc[(i+2)*width+1])/2 >>bits);
		pDest[(i+1)*width*3+1] = BYTE((pSrc[i*width  ] + pSrc[(i+2)*width  ] + pSrc[(i+1)*width+1])/3 >>bits);
		pDest[(i+1)*width*3+2] = BYTE( pSrc[(i+1)*width] >>bits);
		//B
		pDest[((i+1)*width-1)*3]   = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE((pSrc[(i)*width-1] + pSrc[(i+2)*width-1] + pSrc[(i+1)*width-2])/3 >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE((pSrc[(i)*width-2] + pSrc[(i+2)*width-2])/2 >>bits);
		//GB
		pDest[((i+2)*width-1)*3]   = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1])/2 >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE( pSrc[(i+2)*width-2] >>bits);
	}
	//R
	pDest[0] = BYTE(pSrc[width+1] >>bits);
	pDest[1] = BYTE((pSrc[width]+pSrc[1])/2 >>bits);
	pDest[2] = BYTE(pSrc[0] >>bits);
	//GR
	pDest[3*(width-1)]   = BYTE(pSrc[width-1+width] >>bits);
	pDest[3*(width-1)+1] = BYTE(pSrc[width-1] >>bits);
	pDest[3*(width-1)+2] = BYTE(pSrc[width-2] >>bits);
	//GB
	pDest[(height-1)*3*(width)]   = BYTE(pSrc[(height-1)*width+1] >>bits);
	pDest[(height-1)*3*(width)+1] = BYTE(pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*3*(width)+2] = BYTE(pSrc[(height-2)*width] >>bits);
	//B
	pDest[(height*width-1)*3]   = BYTE( pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE((pSrc[height*width-1-1] + pSrc[(height-1)*width-1])/2 >>bits);
	pDest[(height*width-1)*3+2] = BYTE( pSrc[(height-1)*width-1-1] >>bits);

	return TRUE;
}

BOOL ImageProc::Interpolation_GRBG_DT(BYTE* pSrc,BYTE *pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = 0;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//GB
			pDest[(i*width+j)*3]   =  BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1])/2 >>bits);
			pDest[(i*width+j)*3+1] =  BYTE((pSrc[(i  )*width + j  ] + pSrc[(i-1)*width + j+1])/2 >>bits); //DT
			pDest[(i*width+j)*3+2] =  BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width + j  ])/2 >>bits);

			//B
			pDest[(i*width+j)*3+3] =  BYTE( pSrc[(i  )*width + j+1] >>bits);
			pDest[(i*width+j)*3+4] =  BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width + j+1] + pSrc[(i  )*width+j] + pSrc[(i  )*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+5] =  BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i-1)*width + j+2] + pSrc[(i+1)*width+j] + pSrc[(i+1)*width + j+2])/4 >>bits);

			//R
			pDest[((i+1)*width+j)*3]   =  BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1] + pSrc[(i+2)*width+j-1] + pSrc[(i+2)*width+j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+1] =  BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width + j  ] + pSrc[(i+1)*width+j-1] + pSrc[(i+1)*width+j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+2] =  BYTE( pSrc[(i+1)*width + j  ] >>bits);

			//GR
			pDest[((i+1)*width+j)*3+3] =  BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1])/2 >>bits);
			pDest[((i+1)*width+j)*3+4] =  BYTE((pSrc[(i+1)*width + j+1] + pSrc[(i  )*width + j+2])/2 >>bits);//DT
			pDest[((i+1)*width+j)*3+5] =  BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i+1)*width + j+2])/2 >>bits);

		}
	}

	//边缘插值 上下
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;//最后一行

		//R
		pDest[j*3]   =  BYTE((pSrc[width+j-1]+ pSrc[width+j+1])/2 >>bits);
		pDest[j*3+1] =  BYTE((pSrc[width+j ] + pSrc[j-1] + pSrc[j+1])/3 >>bits);
		pDest[j*3+2] =  BYTE( pSrc[j] >>bits);
		//Gr
		pDest[j*3+3] =  BYTE( pSrc[width+j+1] >>bits);
		pDest[j*3+4] =  BYTE( pSrc[j+1] >>bits);
		pDest[j*3+5] =  BYTE((pSrc[j] + pSrc[j+2])/2 >>bits);
		//Gb
		pDest[d1]   =  BYTE((pSrc[(height-1)*width + j-1] + pSrc[(height-1)*width+j+1])/2 >>bits);
		pDest[d1+1] =  BYTE( pSrc[(height-1)*width + j] >>bits);
		pDest[d1+2] =  BYTE( pSrc[(height-2)*width + j] >>bits);
		//B
		pDest[d1+3] =  BYTE( pSrc[(height-1)*width + j+1] >>bits);
		pDest[d1+4] =  BYTE((pSrc[(height-2)*width + j+1] + pSrc[(height-1)*width + j+2] + pSrc[(height-1)*width+j])/3 >>bits);
		pDest[d1+5] =  BYTE((pSrc[(height-2)*width + j]   + pSrc[(height-2)*width + j+2])/2 >>bits);

	}
	//左右
	for(int i=1;i<height-1;i+=2)
	{
		//B
		pDest[(i)*width*3]   = BYTE( pSrc[(i  )*width] >>bits);
		pDest[(i)*width*3+1] = BYTE((pSrc[(i-1)*width  ] + pSrc[(i+1)*width] + pSrc[(i)*width+1])/3 >>bits);
		pDest[(i)*width*3+2] = BYTE((pSrc[(i-1)*width+1] + pSrc[(i+1)*width+1])/2 >>bits);
		//Gb
		pDest[(i+1)*width*3]   = BYTE((pSrc[(i  )*width  ] + pSrc[(i+2)*width])/2 >>bits);
		pDest[(i+1)*width*3+1] = BYTE( pSrc[(i+1)*width  ] >>bits);
		pDest[(i+1)*width*3+2] = BYTE( pSrc[(i+1)*width+1] >>bits);
		//Gr
		pDest[((i+1)*width-1)*3]   = BYTE( pSrc[(i+1)*width-2] >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE((pSrc[(i  )*width-1] + pSrc[(i+2)*width-1])/2 >>bits);
		//R
		pDest[((i+2)*width-1)*3] =   BYTE((pSrc[(i+1)*width-2] + pSrc[(i+3)*width-2])/2 >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1] + pSrc[(i+2)*width-2])/3 >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE( pSrc[(i+2)*width-1] >>bits);

	}	
	//Gr
	pDest[0] = BYTE(pSrc[width] >>bits);
	pDest[1] = BYTE(pSrc[0] >>bits);
	pDest[2] = BYTE(pSrc[1] >>bits);
	//R
	pDest[3*(width-1)] =   BYTE( pSrc[2*width-2] >>bits);
	pDest[3*(width-1)+1] = BYTE((pSrc[width-2] + pSrc[2*width-1])/2 >>bits);
	pDest[3*(width-1)+2] = BYTE( pSrc[width-1] >>bits);
	//B
	pDest[(height-1)*(width)*3] =   BYTE( pSrc[(height-1)*width] >>bits);
	pDest[(height-1)*(width)*3+1] = BYTE((pSrc[(height-1)*width+1] + pSrc[(height-2)*width])/2 >>bits);
	pDest[(height-1)*(width)*3+2] = BYTE( pSrc[(height-2)*width+1] >>bits);
	//Gb
	pDest[(height*width-1)*3] =   BYTE(pSrc[height*width-2] >>bits);
	pDest[(height*width-1)*3+1] = BYTE(pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+2] = BYTE(pSrc[(height-1)*width-1] >>bits);

	return TRUE;
}

BOOL ImageProc::Interpolation_GBRG_DT(BYTE* pSrc,BYTE *pDest, int width, int height)
{
	if (!pSrc) return FALSE;
	if (!pDest) return FALSE;

	BYTE bits = 0;

	for(int i=1;i<height-1;i+=2)
	{
		for (int j=1;j<width-1;j+=2)
		{
			//Gr
			pDest[(i*width+j)*3]   = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i+1)*width + j  ])/2 >>bits);
			pDest[(i*width+j)*3+1] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i-1)*width + j+1])/2 >>bits); //DT
			pDest[(i*width+j)*3+2] = BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width + j+1])/2 >>bits);
			//R
			pDest[(i*width+j)*3+3] = BYTE((pSrc[(i-1)*width + j  ] + pSrc[(i-1)*width+j+2] + pSrc[(i+1)*width + j] + pSrc[(i+1)*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+4] = BYTE((pSrc[(i-1)*width + j+1] + pSrc[(i+1)*width+j+1] + pSrc[(i  )*width + j] + pSrc[(i  )*width + j+2])/4 >>bits);
			pDest[(i*width+j)*3+5] = BYTE( pSrc[(i  )*width + j+1] >>bits);

			//B
			pDest[((i+1)*width+j)*3]   = BYTE( pSrc[(i+1)*width + j  ] >>bits);
			pDest[((i+1)*width+j)*3+1] = BYTE((pSrc[(i  )*width + j  ] + pSrc[(i+2)*width+j  ] + pSrc[(i+1)*width + j-1] + pSrc[(i+1)*width + j+1])/4 >>bits);
			pDest[((i+1)*width+j)*3+2] = BYTE((pSrc[(i  )*width + j-1] + pSrc[(i  )*width+j+1] + pSrc[(i+2)*width + j-1] + pSrc[(i+2)*width + j+1])/4 >>bits);
			//Gb
			pDest[((i+1)*width+j)*3+3] = BYTE((pSrc[(i+1)*width + j  ] + pSrc[(i+1)*width + j+2])/2 >>bits);
			pDest[((i+1)*width+j)*3+4] = BYTE((pSrc[(i+1)*width + j+1] + pSrc[(i  )*width + j+2])/2 >>bits);//DT
			pDest[((i+1)*width+j)*3+5] = BYTE((pSrc[(i  )*width + j+1] + pSrc[(i+2)*width + j+1])/2 >>bits);

		}
	}

	//边缘插值 上下
	for(int j=1;j<width-1;j+=2)
	{
		int d1 = ((height-1)*width+j)*3;//最后一行

		//B
		pDest[j*3] =   BYTE( pSrc[j] >>bits);
		pDest[j*3+1] = BYTE((pSrc[width+j] + pSrc[j-1] + pSrc[j+1])/3 >>bits);
		pDest[j*3+2] = BYTE((pSrc[width+j-1] + pSrc[width+j+1])/2 >>bits);
		//Gb
		pDest[j*3+3] = BYTE((pSrc[j] + pSrc[j+2])/2 >>bits);
		pDest[j*3+4] = BYTE( pSrc[j+1] >>bits);
		pDest[j*3+5] = BYTE( pSrc[width+j+1] >>bits);
		//Gr
		pDest[d1] =   BYTE( pSrc[(height-2)*width + j] >>bits);
		pDest[d1+1] = BYTE( pSrc[(height-1)*width + j] >>bits);
		pDest[d1+2] = BYTE((pSrc[(height-1)*width + j-1] + pSrc[(height-1)*width+j+1])/2 >>bits);
		//R
		pDest[d1+3] = BYTE((pSrc[(height-2)*width+j  ] + pSrc[(height-2)*width+j+2])/2 >>bits);
		pDest[d1+4] = BYTE((pSrc[(height-2)*width+j+1] + pSrc[(height-1)*width+j+2] + pSrc[(height-1)*width+j])/3 >>bits);
		pDest[d1+5] = BYTE( pSrc[(height-1)*width+j+1] >>bits);

	}
	//左右
	for(int i=1;i<height-1;i+=2)
	{
		//R
		pDest[(i)*width*3] =   BYTE((pSrc[(i-1)*width + 1]+ pSrc[(i+1)*width + 1])/2 >>bits);
		pDest[(i)*width*3+1] = BYTE((pSrc[(i-1)*width] + pSrc[(i+1)*width] + pSrc[(i)*width+1])/3 >>bits);
		pDest[(i)*width*3+2] = BYTE( pSrc[(i)*width] >>bits);
		//Gb
		pDest[(i+1)*width*3]   = BYTE( pSrc[(i+1)*width+1] >>bits);
		pDest[(i+1)*width*3+1] = BYTE( pSrc[(i+1)*width] >>bits);
		pDest[(i+1)*width*3+2] = BYTE((pSrc[(i  )*width] + pSrc[(i+2)*width])/2 >>bits);
		//Gr
		pDest[((i+1)*width-1)*3]   = BYTE((pSrc[(i)*width-1] + pSrc[(i+2)*width-1])/2 >>bits);
		pDest[((i+1)*width-1)*3+1] = BYTE( pSrc[(i+1)*width-1] >>bits);
		pDest[((i+1)*width-1)*3+2] = BYTE( pSrc[(i+1)*width-2] >>bits);
		//B
		pDest[((i+2)*width-1)*3]   = BYTE( pSrc[(i+2)*width-1] >>bits);
		pDest[((i+2)*width-1)*3+1] = BYTE((pSrc[(i+1)*width-1] + pSrc[(i+3)*width-1] + pSrc[(i+2)*width-2])/3 >>bits);
		pDest[((i+2)*width-1)*3+2] = BYTE((pSrc[(i+1)*width-2] + pSrc[(i+3)*width-2])/2 >>bits);

	}	
	//Gb
	pDest[0] = BYTE(pSrc[1] >>bits);
	pDest[1] = BYTE(pSrc[0] >>bits);
	pDest[2] = BYTE(pSrc[width] >>bits);
	//B
	pDest[3*(width-1)]   = BYTE( pSrc[width-1] >>bits);
	pDest[3*(width-1)+1] = BYTE((pSrc[width-2] + pSrc[2*width-1])/2 >>bits);
	pDest[3*(width-1)+2] = BYTE( pSrc[2*width-2] >>bits);
	//R
	pDest[(height-1)*(width)*3]   = BYTE( pSrc[(height-2)*width+1] >>bits);
	pDest[(height-1)*(width)*3+1] = BYTE((pSrc[(height-1)*width+1] + pSrc[(height-2)*width])/2 >>bits);
	pDest[(height-1)*(width)*3+2] = BYTE( pSrc[(height-1)*width] >>bits);
	//Gr
	pDest[(height*width-1)*3]   = BYTE(pSrc[(height-1)*width-1] >>bits);
	pDest[(height*width-1)*3+1] = BYTE(pSrc[height*width-1] >>bits);
	pDest[(height*width-1)*3+2] = BYTE(pSrc[height*width-2] >>bits);

	return TRUE;
}

//采用photoshop的算法
BOOL ImageProc::RGB24ToLab(BYTE *pScr, double *pDest, int width, int height)
{
	if (pScr == NULL)
	{
		return FALSE;
	}

	BYTE R, G, B;
	double lab_l,lab_a,lab_b;
	double X, Y, Z;
	double BLACK = 20.0;
	double YELLOW = 70.0;
	for (int i=0; i<height; i++)
	{
		for (int j=0; j<width; j++)
		{

			R = pScr[(i*width+j)*3];
			G = pScr[(i*width+j)*3+1];
			B = pScr[(i*width+j)*3+2];
			double r = R / 255.000; // rgb range: 0 ~ 1
			double g = G / 255.000;
			double b = B / 255.000;

			// gamma 2.2
			if ( r > 0.04045 ) 
				r = pow(( r + 0.055 ) / 1.055, 2.4);
			else               
				r = r / 12.92;
			if ( g > 0.04045 ) 
				g = pow(( g + 0.055 ) / 1.055, 2.4);
			else               
				g = g / 12.92;
			if ( b > 0.04045 ) 
				b = pow(( b + 0.055 ) / 1.055, 2.4);
			else               
				b = b / 12.92;

			// sRGB
			X = r * 0.436052025 + g * 0.385081593 + b * 0.143087414;
			Y = r * 0.222491598 + g * 0.716886060 + b * 0.060621486;
			Z = r * 0.013929122 + g * 0.097097002 + b * 0.714185470;

			// XYZ range: 0~100
			X = X * 100.000;
			Y = Y * 100.000;
			Z = Z * 100.000;

			// Reference White Point
			double ref_X = 96.4221;
			double ref_Y = 100.000;
			double ref_Z = 82.5211;

			X = X / ref_X;
			Y = Y / ref_Y;
			Z = Z / ref_Z;

			// Lab
			if (X > 0.008856) 
				X = pow(X, 1/3.000);
			else              
				X = ( 7.787 * X ) + ( 16 / 116.000 );
			if (Y > 0.008856) 
				Y = pow(Y, 1/3.000);
			else              
				Y = ( 7.787 * Y ) + ( 16 / 116.000 );
			if (Z > 0.008856) 
				Z = pow(Z, 1/3.000);
			else              
				Z = ( 7.787 * Z ) + ( 16 / 116.000 );

			lab_l = ( 116.000 * Y ) - 16.000;
			lab_a = 500.000 * ( X - Y );
			lab_b = 200.000 * ( Y - Z );

			// 归一化值Lab
			pDest[(i*width+j)*3] = lab_l;    // L
			pDest[(i*width+j)*3+1] = lab_a; // a
			pDest[(i*width+j)*3+2] = lab_b; // b


		}
	}
	return TRUE;
}
//采用photoshop的算法 Pixel
BOOL ImageProc::RGBToLab_Pixel(BYTE R, BYTE G, BYTE B, double &lab_l, double &lab_a, double &lab_b)
{

	double X, Y, Z;
	double BLACK = 20.0;
	double YELLOW = 70.0;

	double r = R / 255.000; // rgb range: 0 ~ 1
	double g = G / 255.000;
	double b = B / 255.000;

	// gamma 2.2
	if ( r > 0.04045 ) 
		r = pow(( r + 0.055 ) / 1.055, 2.4);
	else               
		r = r / 12.92;
	if ( g > 0.04045 ) 
		g = pow(( g + 0.055 ) / 1.055, 2.4);
	else               
		g = g / 12.92;
	if ( b > 0.04045 ) 
		b = pow(( b + 0.055 ) / 1.055, 2.4);
	else               
		b = b / 12.92;

	// sRGB
	X = r * 0.436052025 + g * 0.385081593 + b * 0.143087414;
	Y = r * 0.222491598 + g * 0.716886060 + b * 0.060621486;
	Z = r * 0.013929122 + g * 0.097097002 + b * 0.714185470;

	// XYZ range: 0~100
	X = X * 100.000;
	Y = Y * 100.000;
	Z = Z * 100.000;

	// Reference White Point
	double ref_X = 96.4221;
	double ref_Y = 100.000;
	double ref_Z = 82.5211;

	X = X / ref_X;
	Y = Y / ref_Y;
	Z = Z / ref_Z;

	// Lab
	if (X > 0.008856) 
		X = pow(X, 1/3.000);
	else              
		X = ( 7.787 * X ) + ( 16 / 116.000 );
	if (Y > 0.008856) 
		Y = pow(Y, 1/3.000);
	else              
		Y = ( 7.787 * Y ) + ( 16 / 116.000 );
	if (Z > 0.008856) 
		Z = pow(Z, 1/3.000);
	else              
		Z = ( 7.787 * Z ) + ( 16 / 116.000 );

	lab_l = ( 116.000 * Y ) - 16.000;
	lab_a = 500.000 * ( X - Y );
	lab_b = 200.000 * ( Y - Z );

	return TRUE;
}

//采用发哥给的算法
BOOL RGBToLab1(BYTE R, BYTE G, BYTE B, double &lab_l, double &lab_a, double &lab_b)
{
	double XX=0,YY=0,ZZ=0,aa=0,bb=0,LL=0; 
	double var_R,var_G,var_B,var_X,var_Y,var_Z; 

	double X1,Y1,Z1;

	// RGB to XYZ 
	var_R = (double)R/255.000;
	var_G = (double)G/255.000;
	var_B = (double)B/255.000;
	if(var_R>0.04045)
		var_R=pow((var_R+0.055)/1.055,2.4);
	else
		var_R=var_R/12.92;
	if(var_G>0.04045)
		var_G=pow((var_G+0.055)/1.055,2.4);
	else
		var_G=var_G/12.92;
	if(var_B>0.04045)
		var_B=pow((var_B+0.055)/1.055,2.4);
	else
		var_B=var_B/12.92;
	var_R=var_R*100.000;
	var_G=var_G*100.000;
	var_B=var_B*100.000;
	// X1=var_R*0.4124+var_G*0.3576+var_B*0.1805;
	// Y1=var_R*0.2126+var_G*0.7152+var_B*0.0722;
	// Z1=var_R*0.0193+var_G*0.1192+var_B*0.9505;
	X1=0.412453*var_R+0.357580*var_G+0.180423*var_B;
	Y1=0.212671*var_R+0.715160*var_G+0.072169*var_B;
	Z1=0.019334*var_R+0.119193*var_G+0.950227*var_B;
	//XYZ to Lab
	var_X=X1/95.047;
	var_Y=Y1/100.000;
	var_Z=Z1/108.883;
	if(var_X>0.008856)
		var_X=pow(var_X,1.000/3.000);
	else
		var_X=(7.787*var_X)+16.000/116.000;
	if(var_Y>0.008856)
		var_Y=pow(var_Y,1.000/3.000);
	else
		var_Y=(7.787*var_Y)+16.000/116.000;
	if(var_Z>0.008856)
		var_Z=pow(var_Z,1.000/3.000);
	else
		var_Z=(7.787*var_Z)+16.000/116.000;
	lab_l=(116.000*var_Y)-16.000;
	lab_a=500.000*(var_X-var_Y);
	lab_b=200.000*(var_Y-var_Z);
	// end color translate

	return TRUE;
}
//采用网络搜的算法
BOOL RGBToLab2(BYTE R, BYTE G, BYTE B, double &lab_l, double &lab_a, double &lab_b)
{
	double x,y,z;
	double fx,fy,fz;
	double BLACK = 20.0;
	double YELLOW = 70.0;

	// 转至X-Y-Z
	//[ X ]   [ 0.412453  0.357580  0.180423 ]   [ R ]
	//[ Y ] = [ 0.212671  0.715160  0.072169 ] * [ G ]
	//[ Z ]   [ 0.019334  0.119193  0.950227 ]   [ B ]
	x = 0.412453*R + 0.357580*G + 0.180423*B;
	y = 0.212671*R + 0.715160*G + 0.072169*B;
	z = 0.019334*R + 0.119193*G + 0.950227*B;

	// 除255即归一化
	x = x/(255.0*0.950456);
	y = y/255.0;
	z = z/(255.0*1.088754);

	if(y>0.008856)
	{
		fy = pow(y,1.0/3.0);
	}else
	{
		fy = 7.787*y + 16.0/116.0;
	}

	if(x>0.008856)
	{
		fx = pow(x,1.0/3.0);
	}else
	{
		fx = 7.787*x + 16.0/116.0;
	}

	if(z>0.008856)
	{
		fz = pow(z,1.0/3.0);
	}else
	{
		fz = 7.787*z + 16.0/116.0;
	}
	lab_l=(116.000*fy)-16.000;
	lab_a = 500.0*(fx-fy);
	lab_b = 200.0*(fy-fz);

	// 	if (lab_l< BLACK)
	// 	{
	// 		lab_a *= exp((lab_l - BLACK) / (BLACK/ 4));
	// 		lab_b *= exp((lab_l - BLACK) / (BLACK/ 4));
	// 		lab_l = 20;
	// 	}
	// 	if (lab_b > YELLOW)lab_b = YELLOW;

	return TRUE;
}
//采用photoshop的算法
double ImageProc::GetAveL_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect)
{
	if (pScr == NULL)
	{
		return 0;
	}

	double sum=0;
	int count=0;
	double AveL;
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j = rect.left; j<rect.right; j++)
		{
			if(j<0 || j>=width || i<0 || i>=height) break;

			BYTE R = pScr[(i*width+j)*3];
			BYTE G = pScr[(i*width+j)*3+1];
			BYTE B = pScr[(i*width+j)*3+2];
			double r = R / 255.000; // rgb range: 0 ~ 1
			double g = G / 255.000;
			double b = B / 255.000;

			// gamma 2.2
			if ( r > 0.04045 ) 
				r = pow(( r + 0.055 ) / 1.055, 2.4);
			else               
				r = r / 12.92;
			if ( g > 0.04045 ) 
				g = pow(( g + 0.055 ) / 1.055, 2.4);
			else               
				g = g / 12.92;
			if ( b > 0.04045 ) 
				b = pow(( b + 0.055 ) / 1.055, 2.4);
			else               
				b = b / 12.92;

			// sRGB
			double X = r * 0.436052025 + g * 0.385081593 + b * 0.143087414;
			double Y = r * 0.222491598 + g * 0.716886060 + b * 0.060621486;
			double Z = r * 0.013929122 + g * 0.097097002 + b * 0.714185470;

			// XYZ range: 0~100
			X = X * 100.000;
			Y = Y * 100.000;
			Z = Z * 100.000;

			// Reference White Point
			double ref_X = 96.4221;
			double ref_Y = 100.000;
			double ref_Z = 82.5211;

			X = X / ref_X;
			Y = Y / ref_Y;
			Z = Z / ref_Z;

			// Lab
			if (X > 0.008856) 
				X = pow(X, 1/3.000);
			else              
				X = ( 7.787 * X ) + ( 16 / 116.000 );
			if (Y > 0.008856) 
				Y = pow(Y, 1/3.000);
			else              
				Y = ( 7.787 * Y ) + ( 16 / 116.000 );
			if (Z > 0.008856) 
				Z = pow(Z, 1/3.000);
			else              
				Z = ( 7.787 * Z ) + ( 16 / 116.000 );

			double lab_l = ( 116.000 * Y ) - 16.000;
			double lab_a = 500.000 * ( X - Y );
			double lab_b = 200.000 * ( Y - Z );

			sum += lab_l;
			count++;
		}
	}

	if(count == 0) return 0;
	AveL=sum/count;
	return AveL;

}
//采用photoshop的算法
double ImageProc::GetAvea_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect)
{
	if (pScr == NULL)
	{
		return 0;
	}


	double sum=0;
	int count=0;
	double Avea;
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j = rect.left; j<rect.right; j++)
		{
			if(j<0 || j>=width || i<0 || i>=height) break;

			BYTE R = pScr[(i*width+j)*3];
			BYTE G = pScr[(i*width+j)*3+1];
			BYTE B = pScr[(i*width+j)*3+2];
			double r = R / 255.000; // rgb range: 0 ~ 1
			double g = G / 255.000;
			double b = B / 255.000;

			// gamma 2.2
			if ( r > 0.04045 ) 
				r = pow(( r + 0.055 ) / 1.055, 2.4);
			else               
				r = r / 12.92;
			if ( g > 0.04045 ) 
				g = pow(( g + 0.055 ) / 1.055, 2.4);
			else               
				g = g / 12.92;
			if ( b > 0.04045 ) 
				b = pow(( b + 0.055 ) / 1.055, 2.4);
			else               
				b = b / 12.92;

			// sRGB
			double X = r * 0.436052025 + g * 0.385081593 + b * 0.143087414;
			double Y = r * 0.222491598 + g * 0.716886060 + b * 0.060621486;
			double Z = r * 0.013929122 + g * 0.097097002 + b * 0.714185470;

			// XYZ range: 0~100
			X = X * 100.000;
			Y = Y * 100.000;
			Z = Z * 100.000;

			// Reference White Point
			double ref_X = 96.4221;
			double ref_Y = 100.000;
			double ref_Z = 82.5211;

			X = X / ref_X;
			Y = Y / ref_Y;
			Z = Z / ref_Z;

			// Lab
			if (X > 0.008856) 
				X = pow(X, 1/3.000);
			else              
				X = ( 7.787 * X ) + ( 16 / 116.000 );
			if (Y > 0.008856) 
				Y = pow(Y, 1/3.000);
			else              
				Y = ( 7.787 * Y ) + ( 16 / 116.000 );
			if (Z > 0.008856) 
				Z = pow(Z, 1/3.000);
			else              
				Z = ( 7.787 * Z ) + ( 16 / 116.000 );

			double lab_l = ( 116.000 * Y ) - 16.000;
			double lab_a = 500.000 * ( X - Y );
			double lab_b = 200.000 * ( Y - Z );

			sum += lab_a;
			count++;
		}
	}

	if(count == 0) return 0;
	Avea=sum/count;
	return Avea;

}
//采用photoshop的算法
double ImageProc::GetAveb_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect)
{
	if (pScr == NULL)
	{
		return 0;
	}

	double sum=0;
	int count=0;
	double Aveb;
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j = rect.left; j<rect.right; j++)
		{
			if(j<0 || j>=width || i<0 || i>=height) break;

			BYTE R = pScr[(i*width+j)*3];
			BYTE G = pScr[(i*width+j)*3+1];
			BYTE B = pScr[(i*width+j)*3+2];
			double r = R / 255.000; // rgb range: 0 ~ 1
			double g = G / 255.000;
			double b = B / 255.000;

			// gamma 2.2
			if ( r > 0.04045 ) 
				r = pow(( r + 0.055 ) / 1.055, 2.4);
			else               
				r = r / 12.92;
			if ( g > 0.04045 ) 
				g = pow(( g + 0.055 ) / 1.055, 2.4);
			else               
				g = g / 12.92;
			if ( b > 0.04045 ) 
				b = pow(( b + 0.055 ) / 1.055, 2.4);
			else               
				b = b / 12.92;

			// sRGB
			double X = r * 0.436052025 + g * 0.385081593 + b * 0.143087414;
			double Y = r * 0.222491598 + g * 0.716886060 + b * 0.060621486;
			double Z = r * 0.013929122 + g * 0.097097002 + b * 0.714185470;

			// XYZ range: 0~100
			X = X * 100.000;
			Y = Y * 100.000;
			Z = Z * 100.000;

			// Reference White Point
			double ref_X = 96.4221;
			double ref_Y = 100.000;
			double ref_Z = 82.5211;

			X = X / ref_X;
			Y = Y / ref_Y;
			Z = Z / ref_Z;

			// Lab
			if (X > 0.008856) 
				X = pow(X, 1/3.000);
			else              
				X = ( 7.787 * X ) + ( 16 / 116.000 );
			if (Y > 0.008856) 
				Y = pow(Y, 1/3.000);
			else              
				Y = ( 7.787 * Y ) + ( 16 / 116.000 );
			if (Z > 0.008856) 
				Z = pow(Z, 1/3.000);
			else              
				Z = ( 7.787 * Z ) + ( 16 / 116.000 );

			double lab_l = ( 116.000 * Y ) - 16.000;
			double lab_a = 500.000 * ( X - Y );
			double lab_b = 200.000 * ( Y - Z );

			sum += lab_b;
			count++;
		}
	}

	if(count == 0) return 0;
	Aveb=sum/count;
	return Aveb;

}


//车载 tv转 数字算法 lifangsen 2014.08。01 
////////////ELA去隔行算法///////////////////

void ImageProc::EdageDirectLineAvg(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,width*height*2);

	int i,j,k,m;
	int linebyte=width*2;		 
	unsigned char *p0,*p1;

	for (i=0;i<height;i++)//第i行
	{
		//偶数列时
		for (j=0;j<width;j=j+2)//第j列
		{ 
			if(i%2==0)//偶數行
			{
				for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
			}
			else
			{
				if (j<2||j>width-3)
				{
					for(k=0;k<2;k++)
						*(pDisplayBuffer+i*linebyte+j*2+k)=
						(*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
				}
				else			
				{
					for(k=0;k<2;k++)
					{
						p0=(pBmp+(i-1)*linebyte/2+(j-(-2))*2+k);
						p1=(pBmp+(i+1)*linebyte/2+(j+(-2))*2+k);
						for(m=0;m<4;m=m+2)
						{
							if (abs(*p0-*p1)>abs(*(pBmp+(i+1)*linebyte/2+(j+m)*2+k)-*(pBmp+(i-1)*linebyte/2+(j-m)*2+k)))
							{p1=(pBmp+(i+1)*linebyte/2+(j+m)*2+k);
							p0=(pBmp+(i-1)*linebyte/2+(j-m)*2+k);
							}
						}								
						*(pDisplayBuffer+i*linebyte+j*2+k)=(*p0+*p1)/2;
					}						
				}
			}
		}
		//奇数列时
		for (j=1;j<width;j=j+2)
		{if (i%2==0)
		{for (k=0;k<2;k++)
		*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);			
		}
		else
		{if (j<2||j>width-3)
		for(k=0;k<2;k++)
			*(pDisplayBuffer+i*linebyte+j*2+k)=
			(*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
		else
		{
			for(k=0;k<2;k++)
			{
				p0=(pBmp+(i-1)*linebyte/2+(j-(-2))*2+k);
				p1=(pBmp+(i+1)*linebyte/2+(j+(-2))*2+k);
				for (m=0;m<4;m=m+2)
				{
					if (abs(*p0-*p1)>abs(*(pBmp+(i+1)*linebyte/2+(j+m)*2+k)-*(pBmp+(i-1)*linebyte/2+(j-m)*2+k)))
					{
						p1=(pBmp+(i+1)*linebyte/2+(j+m)*2+k);
						p0=(pBmp+(i-1)*linebyte/2+(j-m)*2+k);
					}
				}						 							 
				*(pDisplayBuffer+i*linebyte+j*2+k)=(*p0+*p1)/2;							 
			}

		}
		}
		}
	}
	delete [] pBmp;
}

//2
///////////////场内平均 Bob //////////////////////
void ImageProc::Bob(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);

	int i,j,k;
	int linebyte=width<<1;

	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			if(i%2==1)
			{
				for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=
					(*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;		
			}
			else{
				for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);		
			}
		}
	}
	delete [] pBmp;

}

//3
/////////////编织法weave////////////
void ImageProc::weave(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);

	int i;
	int linebyte=width<<1;
	/***********	
	for (i=0;i<height;i++)
	{
	for (j=0;j<width;j++)
	{
	if (i%2 == 1)
	{
	for (k=0;k<2;k++)
	*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i/2*linebyte+j*2+k);
	} 

	else
	{
	for (k=0;k<2;k++)
	*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+(i/2+height/2)*linebyte+j*2+k);
	}
	}
	}
	delete [] pBmp;
	********************/
	for (i=0;i<height;i++)
	{	
		if (i%2 == 0)
			memcpy(pDisplayBuffer+i*linebyte,pBmp+i/2*linebyte,linebyte);

		else		
			memcpy(pDisplayBuffer+i*linebyte,pBmp+(i/2+height/2)*linebyte,linebyte);		
	}
	delete [] pBmp;
}

//4
////////////动态调变去隔行算法///////////////////

void ImageProc::DynamicModule(unsigned char *pDisplayBuffer, int width, int height)
{
	long bmpsize=height*width*2;
	unsigned char *pBmp=new unsigned char [bmpsize];
	memcpy(pBmp,pDisplayBuffer,bmpsize);

	unsigned char *pBmp2=new unsigned char [bmpsize];
	memcpy(pBmp2,pDisplayBuffer,bmpsize);

	unsigned char *pBmp3=new unsigned char [bmpsize];
	memcpy(pBmp3,pDisplayBuffer,bmpsize);

	int i,j,k;
	int linebyte=width<<1;
	int MaxPixelDiff=0;           //最大像素差
	int Threshold =1 ,Threshold1 =1;   //门限值	 
	float MixCoe;               //混合系数
	int a=0,b=0;

	weave(pBmp, width,height);

	Bob(pBmp2, width,height);			 
	////////////////////动态调变/////////////////////
	for (i=0;i<height>>1;i++)
	{				 
		for(j=0;j<width;j++)
		{
			a+=*(pBmp3+i*linebyte+j*2+1);
			b+=*(pBmp3+(i+height/2)*linebyte+j*2+1);
		}
		if (MaxPixelDiff<abs(a-b))
		{
			MaxPixelDiff=abs(a-b);
			Threshold=(MaxPixelDiff+9)>>5;
		}	 
	}
	for (i=0;i<height>>1;i++)
	{  
		a=0;
		b=0;
		for (j=0;j<width;j++)
		{
			a+=*(pBmp3+i*linebyte+j*2+1);
			b+=*(pBmp3+(i+height/2)*linebyte+j*2+1);
			Threshold1=(abs(a-b)+9)>>5;
		}
		for(j=0;j<width;j++)			 
		{ 
			for (k=0;k<2;k++)
			{
				MixCoe=(float)(Threshold1-1)/(float)Threshold;
				*(pDisplayBuffer+2*i*linebyte+j*2+k)=*(pBmp+2*i*linebyte+j*2+k)*MixCoe
					+*(pBmp2+2*i*linebyte+j*2+k)*(1-MixCoe);
				*(pDisplayBuffer+(2*i+1)*linebyte+j*2+k)=*(pBmp+(2*i+1)*linebyte+j*2+k)*MixCoe
					+*(pBmp2+(2*i+1)*linebyte+j*2+k)*(1-MixCoe);
			}	
		}		 
	}
	delete [] pBmp;
	delete [] pBmp2;
	delete [] pBmp3;
}


//5
////////////////动态ELA///////////////////////////
void ImageProc::DynamicELA(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);

	unsigned char *pBmp2=new unsigned char [height*width*2];
	memcpy(pBmp2,pDisplayBuffer,height*width*2);

	unsigned char *pBmp3=new unsigned char [height*width*2];
	memcpy(pBmp3,pDisplayBuffer,height*width*2);

	int i,j,k;
	int linebyte=width*2;
	int MaxPixelDiff;           //最大像素差
	int Threshold;              //门限值
	int Threshold1;
	float MixCoe;               //混合系数
	int a=0;
	int b=0;
	MaxPixelDiff=0;

	weave(pBmp, width,height);

	EdageDirectLineAvg(pBmp2, width, height);

	////////////////////动态ELA/////////////////////
	for (i=0;i<height/2;i++)
	{				 
		for(j=0;j<width;j++)
		{
			a+=*(pBmp3+i*linebyte+j*2+1);
			b+=*(pBmp3+(i+height/2)*linebyte+j*2+1);
		}
		if (MaxPixelDiff<abs(a-b))
		{
			MaxPixelDiff=abs(a-b);
			Threshold=(MaxPixelDiff+9)/10;
		}	 
	}
	for (i=0;i<height/2;i++)
	{  
		a=0;
		b=0;
		for (j=0;j<width;j++)
		{
			a+=*(pBmp3+i*linebyte+j*2+1);
			b+=*(pBmp3+(i+height/2)*linebyte+j*2+1);
			Threshold1=(abs(a-b)+9)/10;
		}
		for(j=0;j<width;j++)			 
		{ 
			for (k=0;k<2;k++)
			{
				MixCoe=(Threshold1-1)/Threshold;
				*(pDisplayBuffer+2*i*linebyte+j*2+k)=*(pBmp+2*i*linebyte+j*2+k)*MixCoe
					+*(pBmp2+2*i*linebyte+j*2+k)*(1-MixCoe);
				*(pDisplayBuffer+(2*i+1)*linebyte+j*2+k)=*(pBmp+(2*i+1)*linebyte+j*2+k)*MixCoe
					+*(pBmp2+(2*i+1)*linebyte+j*2+k)*(1-MixCoe);
			}	
		}		
	}
	delete [] pBmp;
	delete [] pBmp2;
	delete [] pBmp3;
}



//6
////*******************修正沿ELA********************////////
void ImageProc::ModifiedELA(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);

	unsigned char *p0,*p1;

	int i,j,k,m;
	int linebyte=width*2;
	int PixelDiff;

	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j=j+2)
		{
			if (i%2==0)
			{for (k=0;k<2;k++)
			*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
			} 
			else
			{if (j<2||j>width-3)
			{
				for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=
					(*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
			} 
			else
			{
				PixelDiff=abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i/2+height/2)*linebyte+(j-2)*2+1))+
					abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i/2+height/2)*linebyte+(j+2)*2+1))+
					abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i-1)/2*linebyte+j*2+1))+
					abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i+1)/2*linebyte+j*2+1));
				if (PixelDiff<10)
				{
					for(k=0;k<2;k++)
						*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+(i/2+height/2)*linebyte+j*2+k);
				}

				else
				{
					for(k=0;k<2;k++)
					{
						p0=(pBmp+(i-1)*linebyte/2+(j-(-2))*2+k);
						p1=(pBmp+(i+1)*linebyte/2+(j+(-2))*2+k);			
						for (m=0;m<4;m=m+2)
						{while (abs(*p0-*p1)>abs(*(pBmp+(i+1)*linebyte/2+(j+m)*2+k)-*(pBmp+(i-1)*linebyte/2+(j-m)*2+k)))
						{
							p1=(pBmp+(i+1)*linebyte/2+(j+m)*2+k);
							p0=(pBmp+(i-1)*linebyte/2+(j-m)*2+k); 
						}
						}
						*(pDisplayBuffer+i*linebyte+j*2+k)=(*p0+*p1)/2;
					}		 
				} 					
			}
			}
		}
		for (j=1;j<width;j=j+2)
		{
			if (i%2==0)
			{
				for (k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
			} 
			else
			{if (j<2||j>width-3)
			{
				for(k=0;k<2;k++)
					*(pDisplayBuffer+i*linebyte+j*2+k)=
					(*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
			} 
			else
			{
				PixelDiff=abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i/2+height/2)*linebyte+(j-2)*2+1))+
					abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i/2+height/2)*linebyte+(j+2)*2+1))+
					abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i-1)/2*linebyte+j*2+1))+
					abs(*(pBmp+(i/2+height/2)*linebyte+j*2+1)-*(pBmp+(i+1)/2*linebyte+j*2+1));
				if (PixelDiff<5)
				{for(k=0;k<2;k++)
				*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+(i/2+height/2)*linebyte+j*2+k);
				} 
				else
				{for (k=0;k<2;k++)
				{
					p0=(pBmp+(i-1)*linebyte/2+(j-(-2))*2+k);
					p1=(pBmp+(i+1)*linebyte/2+(j+(-2))*2+k);
					for (m=0;m<4;m=m+2)
					{
						while (abs(*p0-*p1)>abs(*(pBmp+(i+1)*linebyte/2+(j+m)*2+k)-*(pBmp+(i-1)*linebyte/2+(j-m)*2+k)))
						{
							p1=(pBmp+(i+1)*linebyte/2+(j+m)*2+k);
							p0=(pBmp+(i-1)*linebyte/2+(j-m)*2+k); 
						}
					}
					*(pDisplayBuffer+i*linebyte+j*2+k)=(*p0+*p1)/2;
				}
				}
			}
			}
		}
	}
}

//7
//***********************边沿自适应***********************//

void ImageProc::EdageAdption(unsigned char *pDisplayBuffer, int width, int height)
{
	unsigned char *pBmp=new unsigned char [height*width*2];
	memcpy(pBmp,pDisplayBuffer,height*width*2);			

	int linebyte=width*2;
	int i,j,k,m;
	//int MinPiexlDiff;
	int PixelAdd;
	int diffmd=32700;
	//int diffsd=32700;
	int diff=0;
	int md,md1;
	//int sd,sd1,m1,m2;
	PixelAdd=0;
	for (i=0;i<height;i++)
	{
		for (j=0;j<width;j=j+2)
		{
			if(i%2==0)     //偶数行不变
			{for(k=0;k<2;k++)
			*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
			}
			else
			{
				if (j<4||j>width-5)  //前两列、后两列
				{for(k=0;k<2;k++)
				*(pDisplayBuffer+i*linebyte+j*2+k)=
				(*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
				} 
				else
				{					
					//主方向群像素对最小值
					for (md=-2;md<4;md=md+2)           
					{for (m=-2;m<4;m=m+2)
					{diff+=abs(*(pBmp+(i-1)*linebyte/2+(j-md+m)*2+1)
					-*(pBmp+(i+1)*linebyte/2+(j+md+m)*2+1));
					}
					if (diffmd>diff)
					{
						diffmd=diff;
						md1=md;
						//		 m1=m;	  
					}
					diff=0;										
					}			
					for(k=0;k<2;k++)
					{
						for (m=-2;m<4;m=m+2)
						{//*(pDisplayBuffer+i*linebyte+j*2+k)=(*(pBmp+(i-1)*linebyte/2+(j-offset-m)*2+k)+*(pBmp+(i+1)*linebyte/2+(j+offset+m)*2+k))/2;
							PixelAdd+=(*(pBmp+(i-1)*linebyte/2+(j-md1+m)*2+k)+*(pBmp+(i+1)*linebyte/2+(j+md1+m)*2+k));
						}
						*(pDisplayBuffer+i*linebyte+j*2+k)=PixelAdd/6;
						PixelAdd=0;
					}	
				}
			}
		}
		for (j=1;j<width;j=j+2)
		{
			if(i%2==0)     //偶数行不变
			{for(k=0;k<2;k++)
			*(pDisplayBuffer+i*linebyte+j*2+k)=*(pBmp+i*linebyte/2+j*2+k);
			}
			else
			{
				if (j<4||j>width-5)  //前两列、后两列
				{for(k=0;k<2;k++)
				*(pDisplayBuffer+i*linebyte+j*2+k)=
				(*(pBmp+(i-1)*linebyte/2+j*2+k)+*(pBmp+(i+1)*linebyte/2+j*2+k))/2;
				} 
				else
				{					
					//主方向群像素对最小值
					for (md=-2;md<4;md=md+2)           
					{for (m=-2;m<4;m=m+2)
					{diff+=abs(*(pBmp+(i-1)*linebyte/2+(j-md+m)*2+1)
					-*(pBmp+(i+1)*linebyte/2+(j+md+m)*2+1));
					}
					if (diffmd>diff)
					{
						diffmd=diff;
						md1=md;
						//			m1=m;	  
					}
					diff=0;										
					}

					for(k=0;k<2;k++)
					{						
						for (m=-2;m<4;m=m+2)
						{//*(pDisplayBuffer+i*linebyte+j*2+k)=(*(pBmp+(i-1)*linebyte/2+(j-offset-m)*2+k)+*(pBmp+(i+1)*linebyte/2+(j+offset+m)*2+k))/2;	
							PixelAdd+=(*(pBmp+(i-1)*linebyte/2+(j-md1+m)*2+k)+*(pBmp+(i+1)*linebyte/2+(j+md1+m)*2+k));
						}
						*(pDisplayBuffer+i*linebyte+j*2+k)=PixelAdd/6;
						PixelAdd=0;
					}
				}
			}
		}
	}
}

void ImageProc::NormalizationAndBinaryzation( BYTE* pBmp, CPoint pStart, CSize pROIRect, UINT iImgWidth, UINT iImgHeight )
{
	int i,j;
	BYTE GCurrent;
	BYTE GMax;
	BYTE GMin; 

	GMax = 0;
	GMin = 255;

	int BOFFSET = 0;
	int GOFFSET = 1;
	int ROFFSET = 2; 

	for(  i = 0; i < pROIRect.cy; i++ )
	{
		for(  j = 0; j < pROIRect.cx; j++)
		{ 
			GCurrent = pBmp[ ((iImgHeight-1-pStart.y-i)*iImgWidth+ pStart.x+j)*3+GOFFSET];

			if( GCurrent > GMax )
			{
				GMax = GCurrent;
			}

			if( GCurrent < GMin )
			{
				GMin = GCurrent;
			}
		}
	} 

	BYTE tempVal;

	for(  i = 0; i < pROIRect.cy; i++ )
	{
		for(  j = 0; j < pROIRect.cx; j++)
		{ 
			GCurrent = pBmp[ ((iImgHeight-1-pStart.y-i)*iImgWidth+ pStart.x+j)*3+GOFFSET];
			tempVal = int((GCurrent - GMin)*1.0*255/(GMax-GMin));  

			if( tempVal >= 128)
			{
				pBmp[ ((iImgHeight-1-pStart.y-i)*iImgWidth+ pStart.x+j)*3+GOFFSET] = 255;
			}
			else
			{
				pBmp[ ((iImgHeight-1-pStart.y-i)*iImgWidth+ pStart.x+j)*3+GOFFSET] = 0;
			}
		}
	}   
}

UINT ImageProc::GetSingleCircleCenter( BYTE* pBmp, CPoint pStart, CSize pROIRect, UINT iImgWidth, UINT iImgHeight, float& centerX, float& centerY )
{
	//First, Get Mean value of G channel,
	int Cnt = 0;

	int GMax = 0;
	int GMin = 255;
	int GCurrent = 0;
	int GMean = 0;

	int BOFFSET = 0;
	int GOFFSET = 1;
	int ROFFSET = 2; 

	int BlackXSum = 0;
	int BlackYSum = 0;
	int BlackCnt = 0; 

	//pay attention here,we get mean value, not average value.
	int i,j;

	NormalizationAndBinaryzation(pBmp, pStart, pROIRect, iImgWidth, iImgHeight );

	//Second, Get average X, average Y of black pixels, we define black pixel which are those below mean value.	

	for( i =0; i < pROIRect.cy; i++  )
	{
		for( j = 0; j < pROIRect.cx; j++ )
		{

			GCurrent = pBmp[((iImgHeight-1-pStart.y-i)*iImgWidth+ pStart.x+j)*3+GOFFSET];

			if( GCurrent == 0 )
			{ 
				BlackYSum += ( pStart.y+i );
				BlackXSum += ( pStart.x+j );
				BlackCnt++; 
			} 
		}
	} 

	if( BlackCnt != 0 )
	{
		centerX =(float) (BlackXSum*1.0 / BlackCnt);
		centerY =(float) (BlackYSum*1.0 / BlackCnt);

		pBmp[((iImgHeight-1-int(centerY))*iImgWidth+int(centerX))*3+0] = 255;
		pBmp[((iImgHeight-1-int(centerY))*iImgWidth+int(centerX))*3+1] = 255;
		pBmp[((iImgHeight-1-int(centerY))*iImgWidth+int(centerX))*3+2] = 255;  

		return 1;  

	}
	else
	{
		centerX = 0;
		centerY = 0;

		return 0;
	} 
}

//增强图像对比度
BYTE ImageProc::ImageEnhanceY(BYTE *_srcImage,BYTE *DestImage,UINT Width,UINT  Height)
{
	int ImageSize = Width*Height;


	int MaxY = 0;
	int MinY = 255;	 
	int difY= 0;
	int Yvalue = 0;

	//找出当前图片中最亮和最暗的点的亮度值
	for(int i=0; i<ImageSize; i++)
	{
		Yvalue = _srcImage[i];		
		if(Yvalue > MaxY) MaxY = Yvalue;
		if(Yvalue < MinY) MinY = Yvalue;		
	}

	//计算差值
	difY = ((MaxY - MinY) == 0) ? 1 : MaxY-MinY;
	//l亮度拉伸
	for(int i=0; i<ImageSize; i++)
	{
		Yvalue = _srcImage[i];
		DestImage[i] = BYTE(255-((MaxY-Yvalue)*255/difY));//为什么用255去减？  因为 MaxB-ColorB 表示暗的部分，用255去减相当于取反，及为亮度
		DestImage[i] = Yvalue;
	}
	return TRUE;
}

BYTE ImageProc::ImageEnhanceY(BYTE *_srcImage,BYTE *DestImage, CRect rect,UINT Width,UINT  Height)
{  
	int MaxY = 0;
	int MinY = 255;	 
	int difY= 0;
	int Yvalue = 0;

	for(UINT i=0;i<Width*Height;i++)
	{
		DestImage[i] = _srcImage[i];
	}

	//找出当前图片中最亮和最暗的点的亮度值
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j=rect.left; j<rect.right; j++)
		{
			Yvalue = _srcImage[i*Width+j];		
			if(Yvalue > MaxY) MaxY = Yvalue;
			if(Yvalue < MinY) MinY = Yvalue;	
		}			
	}

	//计算差值
	difY = ((MaxY - MinY) == 0) ? 1 : MaxY-MinY;
	//l亮度拉伸
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j=rect.left; j<rect.right; j++)
		{
			Yvalue = _srcImage[i*Width+j];		
			DestImage[i*Width+j] = BYTE(255-((MaxY-Yvalue)*255/difY));//为什么用255去减？  因为 MaxB-ColorB 表示暗的部分，用255去减相当于取反，及为亮度

		}			
	}
	return TRUE;
}

BYTE ImageProc::ImageEnhanceY(int *_srcImage,BYTE *DestImage, CRect rect,UINT Width,UINT  Height)
{  
	int MaxY = 0;
	int MinY = 255;	 
	int difY= 0;
	int Yvalue = 0;

	for(UINT i=0;i<Width*Height;i++)
	{
		DestImage[i] = _srcImage[i];
	}

	//找出当前图片中最亮和最暗的点的亮度值
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j=rect.left; j<rect.right; j++)
		{
			Yvalue = _srcImage[i*Width+j];		
			if(Yvalue > MaxY) MaxY = Yvalue;
			if(Yvalue < MinY) MinY = Yvalue;	
		}			
	}

	//计算差值
	difY = ((MaxY - MinY) == 0) ? 1 : MaxY-MinY;
	//l亮度拉伸
	for(int i=rect.top; i<rect.bottom; i++)
	{
		for(int j=rect.left; j<rect.right; j++)
		{
			Yvalue = _srcImage[i*Width+j];		
			DestImage[i*Width+j] = BYTE(255-((MaxY-Yvalue)*255/difY));//为什么用255去减？  因为 MaxB-ColorB 表示暗的部分，用255去减相当于取反，及为亮度

		}			
	}
	return TRUE;
}

//获取直方图指定比例阈值，从暗的开始算
int  ImageProc::GetHistogramLowThreshold(BYTE *pSrc, int width, int height, float ratio)
{
	if (!pSrc) return 0;

	int* Histogram_buffer = ImageProc::GetHistogram(pSrc,width,height);

	int count=0; 
	int Sum=width*height;

	int th=0;
	for(int i=0;i<255;i++)
	{
		count += Histogram_buffer[i];
		if (count>Sum*ratio/100.0)
		{
			th = i;
			goto End;
		}
	}
End:
	delete [] Histogram_buffer;
	return th;
}

BYTE ImageProc::ImageEnhanceRGB(BYTE *_srcImage,BYTE *EnhanceBMPBuffer, int width, int height)
{
	int ImageSize = width*height;
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
	for(int j=0;j<height;j++)
	{
		for(int i=0;i<width;i++)
		{
			ColorR = _srcImage[width*j+i];
			ColorG = _srcImage[width*j+i+1];
			ColorB = _srcImage[width*j+i+2];

			if(ColorR > MaxR) MaxR = ColorR;
			if(ColorR < MinR) MinR = ColorR;			
			if(ColorG > MaxG) MaxG = ColorG;
			if(ColorG < MinG) MinG = ColorG;
			if(ColorB > MaxB) MaxB = ColorB;
			if(ColorB < MinB) MinB = ColorB;

		}
	} 
	difR = ((MaxR - MinR) == 0) ? 1 : MaxR-MinR;
	difG = ((MaxG - MinG) == 0) ? 1 : MaxG-MinG;
	difB = ((MaxB - MinB) == 0) ? 1 : MaxB-MinB;

	for(int j=0;j<height;j++)
	{
		for(int i=0;i<width;i++)
		{
			ColorR = _srcImage[width*j+i];
			ColorG = _srcImage[width*j+i+1];
			ColorB = _srcImage[width*j+i+2];

			ColorR = BYTE(255-((MaxB-ColorB)*255/difB));
			ColorG = BYTE(255-((MaxG-ColorG)*255/difG));
			ColorB = BYTE(255-((MaxR-ColorR)*255/difR));

			EnhanceBMPBuffer[width*j+i] = ColorR;
			EnhanceBMPBuffer[width*j+i+1] = ColorG;
			EnhanceBMPBuffer[width*j+i+2] = ColorB; 
		}
	} 
	return TRUE;
}


BYTE ImageProc::ImageShrink(BYTE *_SrcImage, BYTE *_DestImage, int srcwidth, int srcheight,int DestWidth,float DestHeight)
{
	unsigned int PixelSum;
	unsigned int PixelCount;
	unsigned int AvgValue;
	unsigned int SrcImgPosition;

	double Hscale = (double)srcwidth/DestWidth;
	double Vscale = (double)srcheight/DestHeight;

	int Border4H = (int)(((Hscale/2)*1000+0.5)/1000);
	int Border4V = (int)(((Vscale/2)*1000+0.5)/1000);

	if(Border4H == 0)
		Border4H = 1;
	if(Border4V == 0)
		Border4V = 1;

	CPoint SrcPoint;
	int DestY,DestX,SrcY,SrcX;

	for (DestY=0; DestY<DestHeight; DestY++)
	{
		for (DestX=0; DestX<DestWidth; DestX++)
		{
			SrcPoint.x = (int)(DestX*Hscale);
			SrcPoint.y = (int)(DestY*Vscale);

			PixelSum	= 0;
			PixelCount  = 0;

			for (SrcY=(SrcPoint.y-Border4V); SrcY<(SrcPoint.y+Border4V); SrcY++)
			{
				for (SrcX=(SrcPoint.x-Border4H); SrcX<(SrcPoint.x+Border4H); SrcX++)
				{
					if(((SrcX >= 0) && (SrcX <= srcwidth)) && ((SrcY >= 0) && (SrcY < srcheight)))
					{
						SrcImgPosition = (SrcY*srcwidth)+SrcX;
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

BOOL ImageProc::ImageMath(BYTE *_SrcImage1, BYTE *_SrcImage2, BYTE *DestImage, int srcwidth, int srcheight, int _Operater)
{
	int temp;
	int index;

	for( int j = 0; j < srcheight; j++ )
	{
		for( int i = 0; i < srcwidth; i++ )
		{
			index = j*srcwidth+i;

			switch( _Operater )
			{
			case math_Add:
				temp = _SrcImage1[index] + _SrcImage2[index];
				break;

			case math_Sub:
				temp = _SrcImage1[index] - _SrcImage2[index]; 

				break;
			case math_Mult:
				temp = _SrcImage1[index] * _SrcImage2[index];
				break;
			case math_Div:
				_SrcImage2[index] = (_SrcImage2[index] == 0) ? 1 : _SrcImage2[index];
				temp = _SrcImage1[index] / _SrcImage2[index];
				break;
			} 

			if(temp > 255)
			{
				temp = 255;
			}  

			DestImage[index] = temp;

		}
	}
	return TRUE;
}

BOOL ImageProc::ImageMath(BYTE *_SrcImage1, BYTE *_SrcImage2, int *DestImage, int srcwidth, int srcheight, int _Operater)
{
	int temp;
	int index;

	for( int j = 0; j < srcheight; j++ )
	{
		for( int i = 0; i < srcwidth; i++ )
		{
			index = j*srcwidth+i;

			switch( _Operater )
			{
			case math_Add:
				temp = _SrcImage1[index] + _SrcImage2[index];
				break;

			case math_Sub:
				temp = _SrcImage1[index] - _SrcImage2[index]; 

				break;
			case math_Mult:
				temp = _SrcImage1[index] * _SrcImage2[index];
				break;
			case math_Div:
				_SrcImage2[index] = (_SrcImage2[index] == 0) ? 1 : _SrcImage2[index];
				temp = _SrcImage1[index] / _SrcImage2[index];
				break;
			} 
			DestImage[index] = temp;

		}
	}
	return TRUE;
}

BYTE ImageProc::PixelMath(BYTE *_SrcImage, double MeanValue, BYTE *_DestImage, int srcwidth, int srcheight, int _Operater)
{
	int _MathImageSize = srcwidth * srcheight;
	//	double temp;
	int i;

	if(_Operater == math_Div)
	{MeanValue = (MeanValue == 0) ? 1 : MeanValue;}

	for(i=0; i<_MathImageSize; i++)
	{
		switch(_Operater)
		{
		case math_Add:
			_DestImage[i] = (BYTE)(_SrcImage[i] + MeanValue);
			break;
		case math_Sub:
			_DestImage[i] = (BYTE)(_SrcImage[i] - MeanValue);
			break;
		case math_Mult:
			_DestImage[i] = (BYTE)(_SrcImage[i] * MeanValue);
			break;
		case math_Div:
			_DestImage[i] = (BYTE)(_SrcImage[i] / MeanValue);
			break;
		}		
	}
	return TRUE;
}

void ImageProc::RowDif( BYTE* SrcBuf, BYTE *DstBuf, int ImgWidth, int ImgHeigh,CRect rect, BYTE Threshold )
{ 
	for(int i=0;i<ImgWidth*ImgHeigh;i++)
	{
		DstBuf[i] = SrcBuf[i];
	}
	int offset = 5;
	for( int i = rect.top; i < rect.bottom-offset; i++ )
	{
		for( int j = rect.left; j < rect.right; j++ )
		{ 
			if( j == rect.left)
			{
				DstBuf[i*ImgWidth+j] = 0;
			}
			else
			{
				DstBuf[i*ImgWidth+j] = abs(SrcBuf[i*ImgWidth+j] - SrcBuf[(i+offset)*ImgWidth+j]);//相邻列作差

				if( DstBuf[i*ImgWidth+j] < Threshold )
				{
					DstBuf[i*ImgWidth+j] = 0;
				}
				else
				{
					DstBuf[i*ImgWidth+j] = 200;
				}
			}

		}
	}
}

void ImageProc::LineDif( BYTE* SrcBuf, BYTE *DstBuf, int ImgWidth, int ImgHeigh,CRect rect, BYTE Threshold )
{
	for(int i=0;i<ImgWidth*ImgHeigh;i++)
	{
		DstBuf[i] = SrcBuf[i];
	}
	int offset = 5;
	for( int i = rect.top; i < rect.bottom; i++ )
	{
		for( int j = rect.left; j < rect.right-offset; j++ )
		{ 
			if( i == rect.top)
			{
				DstBuf[i*ImgWidth+j] = 0;
			}
			else
			{
				//相邻行作差
				DstBuf[i*ImgWidth+j] = abs(SrcBuf[i*ImgWidth+j] - SrcBuf[i*ImgWidth+j+offset]);
				int DstBuf_y = DstBuf[i*ImgWidth+j];
				if(  DstBuf_y< Threshold )
				{
					DstBuf[i*ImgWidth+j] = 0;
				} 
				else
				{
					DstBuf[i*ImgWidth+j] = 200;
				}
			}
		}
	}
}

void ImageProc::MergeImage( BYTE* SrcBuf1, BYTE* SrcBuf2, int ImgWidth, int ImgHeight,CRect rect )
{

	for( int i = rect.top; i < rect.bottom; i++ )
	{
		for( int j = rect.left; j < rect.right; j++ )
		{ 
			//SrcBuf1[i*ImgWidth+j] += SrcBuf2[i*ImgWidth+j];
			if( SrcBuf1[i*ImgWidth+j] < SrcBuf2[i*ImgWidth+j] )
			{
				SrcBuf1[i*ImgWidth+j] = SrcBuf2[i*ImgWidth+j];
			} 
		}
	} 
}



BOOL ImageProc::VertFlipBuf(BYTE  * inbuf,  UINT widthBytes,UINT height)
{   
	BYTE  *tb1;
	BYTE  *tb2;
	if (inbuf==NULL)
		return FALSE;
	UINT bufsize;
	bufsize=widthBytes;
	tb1= (BYTE *)new BYTE[bufsize];
	if (tb1==NULL) {
		return FALSE;
	}
	tb2= (BYTE *)new BYTE [bufsize];
	if (tb2==NULL) {
		delete [] tb1;
		return FALSE;
	}
	UINT row_cnt;     
	ULONG off1=0;
	ULONG off2=0;
	for (row_cnt=0;row_cnt<(height+1)/2;row_cnt++) 
	{
		off1=row_cnt*bufsize;
		off2=((height-1)-row_cnt)*bufsize;   
		memcpy(tb1,inbuf+off1,bufsize);
		memcpy(tb2,inbuf+off2,bufsize);
		memcpy(inbuf+off1,tb2,bufsize);
		memcpy(inbuf+off2,tb1,bufsize);
	}
	delete [] tb1;
	delete [] tb2;
	return TRUE;
} 

void ImageProc::RGB24toNV21(BYTE* pSrc,BYTE* pDest, int width, int height)
{ 
	//cv::Mat rgb_img( height, width, CV_8UC3, pSrc ), yuv_img;
	//   cv::cvtColor( rgb_img, yuv_img, CV_RGB2YUV );//CV_RGB2YUV_YV12

	//   unsigned int data_size = yuv_img.rows*yuv_img.cols/6;
	//   
	//unsigned char *v_data = new unsigned char[ data_size ], *u_data = new unsigned char[ data_size ];
	//   memcpy( v_data, (unsigned char*)(yuv_img.data)+4*data_size, data_size );
	//   memcpy( u_data, (unsigned char*)(yuv_img.data)+5*data_size, data_size );

	//   unsigned char *data = (unsigned char*)(yuv_img.data)+4*data_size;
	//   for ( int i=0; i<data_size; i++ ){
	//       (*data) = v_data[i];	data++;
	//       (*data) = u_data[i];	data++;
	//}
	//delete v_data;
	//delete u_data;

	//memcpy( pDest, (unsigned char*)(yuv_img.data), data_size*6 );    
	IN_PARAMS in_val;
	in_val.img_rgb = pSrc;
	in_val.img_width = width;
	in_val.img_height = height;
	Mems_NV21_Converter( in_val, pDest );
}
USHORT ImageProc::GetAveRGain_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect)
{
	float AveR = GetAveR_ROI_RGB24(pSrc,  width,  height,  rect);
	float AveG = GetAveG_ROI_RGB24(pSrc,  width,  height,  rect);
	float RGainRatio = AveG/AveR;
	USHORT RGain = 0;
	int HighReg = (int)RGainRatio;
	int LowReg = (RGainRatio - HighReg)*100;

	RGain = (HighReg<<8) + LowReg;

	return RGain;


}
USHORT ImageProc::GetAveBGain_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect)
{  

	float AveB = GetAveB_ROI_RGB24(pSrc,  width,  height,  rect);
	float AveG = GetAveG_ROI_RGB24(pSrc,  width,  height,  rect);
	float BGainRatio = AveG/AveB;
	USHORT BGain = 0;
	int HighReg = (int)BGainRatio;
	int LowReg = (BGainRatio - HighReg)*100;

	BGain = (HighReg<<8) + LowReg;

	return BGain; 
}

float ImageProc::GetFloatRGain_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect)
{
	float AveR = GetAveR_ROI_RGB24(pSrc,  width,  height,  rect);
	float AveG = GetAveG_ROI_RGB24(pSrc,  width,  height,  rect);
	float RGainRatio = (AveG-32)/(AveR-32); 

	return RGainRatio;
}
float ImageProc::GetFloatBGain_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect)
{
	float AveB = GetAveB_ROI_RGB24(pSrc,  width,  height,  rect);
	float AveG = GetAveG_ROI_RGB24(pSrc,  width,  height,  rect);
	float BGainRatio = (AveG-32)/(AveB-32);

	return BGainRatio; 

}

void ImageProc::ManuBalance(BYTE *pSrc, int width, int height,float Rgain, float Bgain)
{

	for(int j= 0;j<height;j++)
	{
		for(int i = 0;i<width;i++)
		{
			pSrc[(j*width +i)*3] = (pSrc[(j*width +i)*3]*Bgain<255)?pSrc[(j*width +i)*3]*Bgain:255;
			pSrc[(j*width +i)*3+1] = pSrc[(j*width +i)*3+1];
			pSrc[(j*width +i)*3+2] = (pSrc[(j*width +i)*3+2]*Rgain<255)?pSrc[(j*width +i)*3+2]*Rgain:255;
		}

	}

}

void ImageProc::GetSfrRoi_byHalcon(BYTE* RGB_buf,int ImgWidth,int ImgHeight,PSFR_PARAMETER pSfrPara)
{

	unsigned char *pBmpRed ;
	unsigned char *pBmpGreen ;
	unsigned char *pBmpBlue ;

	pBmpRed = (unsigned char *)malloc(ImgWidth * ImgHeight);
	pBmpGreen = (unsigned char *)malloc(ImgWidth * ImgHeight);
	pBmpBlue = (unsigned char *)malloc(ImgWidth * ImgHeight);

	for (int i=0; i<ImgHeight; i++)
	{
		for (int j=0; j<ImgWidth; j++)
		{
			pBmpBlue[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3];
			pBmpGreen[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3+1];
			pBmpRed[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3+2];
		}
	}

	// Local iconic variables 
	Hobject  Image, GrayImage, Region, RegionClosing;
	Hobject  RegionFillUp, ImageReduced, RegionLines1, RegionLines2;
	Hobject  RegionUnion, RegionObject, RegionUnionFore, ConnectedRegions;
	Hobject  SelectedRegions, RegionOpening, Contours, ContoursSplit;
	Hobject  SelectedContours, ResultObject, ObjectSelected;
	Hobject  EllipseMin, EllipseMax, EllipseC, RoiObject, XLDTrans;
	Hobject  Rectangle;


	// Local control variables 
	HTuple  Width, Height, AbsoluteHisto, RelativeHisto;
	HTuple  HistoThresh, MinThresh, MaxThresh, HistoThreshLength;
	HTuple  Area, Row, Column, PointOrder;
	HTuple  Length, i, j, RowCP, ColumnCP, GrayvalLeft, GrayvalRight;
	HTuple  Rmin, Rmax, Rc, SfrPI, Number, Index, Phi, R;
	HTuple  NumberResult, RowResultStart, ColumnResultStart, RowResultEnd, ColumnResultEnd;


	gen_image3_extern(&Image, "byte", ImgWidth, ImgHeight, (long)pBmpRed, (long)pBmpGreen, (long)pBmpBlue, (long)free);
	get_image_size(Image, &Width, &Height);
	rgb1_to_gray(Image, &GrayImage);

	gray_histo(GrayImage, GrayImage, &AbsoluteHisto, &RelativeHisto);
	HistoThresh = 1;
	histo_to_thresh(RelativeHisto, HistoThresh, &MinThresh, &MaxThresh);
	tuple_length(MinThresh, &HistoThreshLength);
	while (0 != (HistoThreshLength>2))
	{
		HistoThresh += 1;
		histo_to_thresh(RelativeHisto, HistoThresh, &MinThresh, &MaxThresh);
		tuple_length(MinThresh, &HistoThreshLength);
	}
	threshold(GrayImage, &Region, HTuple(MinThresh[1]), HTuple(MaxThresh[1]));
	closing_circle(Region, &RegionClosing, 3.5);
	fill_up(RegionClosing, &RegionFillUp);
	reduce_domain(GrayImage, RegionFillUp, &ImageReduced);

	gen_region_line(&RegionLines1, 0, 0, Height-1, Width-1);
	gen_region_line(&RegionLines2, Height-1, 0, 0, Width-1);
	union2(RegionLines1, RegionLines2, &RegionUnion);
	threshold(ImageReduced, &RegionObject, HTuple(MinThresh[0]), HTuple(MaxThresh[0]));
	union2(RegionUnion, RegionObject, &RegionUnionFore);
	connection(RegionUnionFore, &ConnectedRegions);
	select_shape_std(ConnectedRegions, &SelectedRegions, "max_area", 70);
	opening_circle(SelectedRegions, &RegionOpening, 3.5);

	gen_contour_region_xld(RegionOpening, &Contours, "border");
	area_center_xld(Contours, &Area, &Row, &Column, &PointOrder);

	segment_contours_xld(Contours, &ContoursSplit, "lines", 5, 4, 2);
	select_contours_xld(ContoursSplit, &SelectedContours, "contour_length", HTuple(pSfrPara->ContourLength), 1000, -0.5, 0.5);
	tuple_length(Area, &Length);
	gen_empty_obj(&ResultObject);
	for (i=1; i<=Length; i+=1)
	{
		if (0 != (HTuple(HTuple(HTuple(Row[i-1])<HTuple(Row[(Length-1)/2])).And(HTuple(Column[i-1])<HTuple(Column[(Length-1)/2]))).Or(HTuple(HTuple(Row[i-1])>HTuple(Row[(Length-1)/2])).And(HTuple(Column[i-1])<HTuple(Column[(Length-1)/2])))))
		{
			for (j=1; j<=4; j+=1)
			{
				select_obj(SelectedContours, &ObjectSelected, ((i-1)*4)+j);
				area_center_points_xld(ObjectSelected, &Area, &RowCP, &ColumnCP);
				get_grayval(ImageReduced, RowCP.Int(), (ColumnCP.Int())-50, &GrayvalLeft);
				get_grayval(ImageReduced, RowCP.Int(), (ColumnCP.Int())+50, &GrayvalRight);
				if (0 != (GrayvalLeft<GrayvalRight))
					concat_obj(ResultObject, ObjectSelected, &ResultObject);
			}
		}

		if (0 != (HTuple(HTuple(HTuple(Row[i-1])<HTuple(Row[(Length-1)/2])).And(HTuple(Column[i-1])>HTuple(Column[(Length-1)/2]))).Or(HTuple(HTuple(Row[i-1])>HTuple(Row[(Length-1)/2])).And(HTuple(Column[i-1])>HTuple(Column[(Length-1)/2])))))
		{
			for (j=1; j<=4; j+=1)
			{
				select_obj(SelectedContours, &ObjectSelected, ((i-1)*4)+j);
				area_center_points_xld(ObjectSelected, &Area, &RowCP, &ColumnCP);
				get_grayval(ImageReduced, RowCP.Int(), (ColumnCP.Int())-50, &GrayvalLeft);
				get_grayval(ImageReduced, RowCP.Int(), (ColumnCP.Int())+50, &GrayvalRight);
				if (0 != (GrayvalLeft>GrayvalRight))
					concat_obj(ResultObject, ObjectSelected, &ResultObject);
			}
		}

		if (0 != (i==((Length+1)/2)))
		{
			for (j=1; j<=4; j+=1)
			{
				select_obj(SelectedContours, &ObjectSelected, ((i-1)*4)+j);
				concat_obj(ResultObject, ObjectSelected, &ResultObject);
			}
		}
	}

	Rmin = (((((Height/2)*Height)/2)+(((Width/2)*Width)/2)).Sqrt())*(HTuple(pSfrPara->RoiPosition)-0.1);
	Rmax = (((((Height/2)*Height)/2)+(((Width/2)*Width)/2)).Sqrt())*(HTuple(pSfrPara->RoiPosition)+0.1);
	Rc = (((((Height/2)*Height)/2)+(((Width/2)*Width)/2)).Sqrt())*0.25;
	SfrPI = 3.14159;
	gen_empty_obj(&RoiObject);
	count_obj(ResultObject, &Number);
	for (Index=1; Index<=Number; Index+=1)
	{
		select_obj(ResultObject, &ObjectSelected, Index);

		shape_trans_xld(ObjectSelected, &XLDTrans, "rectangle1");
		orientation_points_xld(XLDTrans, &Phi);

		area_center_points_xld(ObjectSelected, &Area, &RowCP, &ColumnCP);
		R = ((((Height/2)-RowCP)*((Height/2)-RowCP))+(((Width/2)-ColumnCP)*((Width/2)-ColumnCP))).Sqrt();
		if (0 != (HTuple(HTuple(R>Rmin).And(R<Rmax)).Or(R<Rc)))
		{
			if (0 != (HTuple(Phi>(SfrPI/4)).And(Phi<((SfrPI*3)/4))))
				gen_rectangle1(&Rectangle, RowCP-(HTuple(pSfrPara->RoiWidth)/2), ColumnCP-(HTuple(pSfrPara->RoiHeight)/2), RowCP+(HTuple(pSfrPara->RoiWidth)/2), ColumnCP+(HTuple(pSfrPara->RoiHeight)/2));
			else
				gen_rectangle1(&Rectangle, RowCP-(HTuple(pSfrPara->RoiHeight)/2), ColumnCP-(HTuple(pSfrPara->RoiWidth)/2), RowCP+(HTuple(pSfrPara->RoiHeight)/2), ColumnCP+(HTuple(pSfrPara->RoiWidth)/2));
			concat_obj(RoiObject, Rectangle, &RoiObject);
		}
	}

	count_obj(RoiObject, &NumberResult);
	for (Index=1; Index<=NumberResult; Index+=1)
	{
		select_obj(RoiObject, &ObjectSelected, Index);
		smallest_rectangle1(ObjectSelected, &RowResultStart, &ColumnResultStart, &RowResultEnd, &ColumnResultEnd);
		CRect rect;
		rect.left = (int)ColumnResultStart[0].I();
		rect.right = (int)ColumnResultEnd[0].I();
		rect.top = (int)RowResultStart[0].I();
		rect.bottom = (int)RowResultEnd[0].I();
		pSfrPara->RoiRect.push_back(rect);

	}

	Image.~Hobject();
	GrayImage.~Hobject();
	Region.~Hobject();
	RegionClosing.~Hobject();;
	RegionFillUp.~Hobject();
	ImageReduced.~Hobject();
	RegionLines1.~Hobject();
	RegionLines2.~Hobject();;
	RegionUnion.~Hobject();
	RegionObject.~Hobject();
	RegionUnionFore.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	RegionOpening.~Hobject();
	Contours.~Hobject();
	ContoursSplit.~Hobject();
	SelectedContours.~Hobject();
	ResultObject.~Hobject();
	ObjectSelected.~Hobject();;
	EllipseMin.~Hobject();
	EllipseMax.~Hobject();
	EllipseC.~Hobject();
	RoiObject.~Hobject();
	XLDTrans.~Hobject();
	Rectangle.~Hobject();
}

int  ImageProc::GetSFR_ofilm( USHORT *_imageBuffer, int _height, int _width,CRect rect,float freqInput ,float& sfrOut, _sfrresult *sfrRet)
{ 
	dataformat dataByte = bayer10;

	int w = rect.Width(), h = rect.Height();

	unsigned char * imgBuff = new unsigned char[w * h];
	long posi = 0;
	for (int j=rect.top; j<rect.bottom; j++)
	{
		for (int k=rect.left; k<rect.right; k++)
		{
			imgBuff[posi++] = _imageBuffer[j * _width + k];
		}
	}

	memset(sfrRet, 0, sizeof(_sfrresult));
	int flag = ::GetSFR_ofilm( sfrOut , sfrRet, freqInput , (const void*)imgBuff,  dataByte,  h,  w);
	delete imgBuff;
	imgBuff = NULL;
	return flag;
}

int  ImageProc::GetSFR_ofilm( BYTE *_imageBuffer, int _height, int _width,CRect rect,float freqInput ,float& sfrOut, _sfrresult *sfrRet )
{ 
	dataformat dataByte = bayer8;

	int w = rect.Width(), h = rect.Height();

	unsigned char * imgBuff = new unsigned char[w * h];
	long posi = 0;
	for (int j=rect.top; j<rect.bottom; j++)
	{
		for (int k=rect.left; k<rect.right; k++)
		{
			imgBuff[posi++] = _imageBuffer[j * _width + k];
		}
	}

	memset(sfrRet, 0, sizeof(_sfrresult));
	int flag = ::GetSFR_ofilm( sfrOut , sfrRet, freqInput , (const void*)imgBuff,  dataByte,  h,  w);
	delete imgBuff;
	imgBuff = NULL;
	return flag;
}

void ImageProc::MotoDemosaic( BYTE* pSrcBuffer, BYTE* pDstBuffer, int nImgWidth, int nImgHeight, int bayerType  )
{
	int width = nImgWidth;
	int height  = nImgHeight;   
	switch( bayerType )
	{
	case 0: //RGGB
		Interpolation_RGGB_MotoDemosaic( pSrcBuffer,  pDstBuffer,  width,  height);
		break;
	case 1: //GRBG
		Interpolation_GRBG_MotoDemosaic( pSrcBuffer,  pDstBuffer,  width,  height);
		break;
	case 2:   //GBRG
		Interpolation_GBRG_MotoDemosaic( pSrcBuffer,  pDstBuffer,  width,  height);
		break; 
	case 3:  //BGGR		
		//////////////////////////////////////
		Interpolation_BGGR_MotoDemosaic( pSrcBuffer, pDstBuffer,  width,  height);
		break;
	} 
}

BOOL ImageProc::RawtoRGB24MotoDemosaic(USHORT *pSrcBuffer, BYTE* pDstBuffer, int width, int height, BYTE outformat, BYTE Bits )
{   
	BYTE* Raw8 = new BYTE[width*height];
	for(int i=0;i<width*height;i++)
	{
		Raw8[i] = (pSrcBuffer[i]>>(Bits-8));
	}
	switch( outformat )
	{
	case 0: //RGGB
		Interpolation_RGGB_MotoDemosaic( Raw8,  pDstBuffer,  width,  height);
		break;
	case 1: //GRBG
		Interpolation_GRBG_MotoDemosaic( Raw8,  pDstBuffer,  width,  height);
		break;
	case 2:   //GBRG
		Interpolation_GBRG_MotoDemosaic( Raw8,  pDstBuffer,  width,  height);
		break; 
	case 3:  //BGGR		
		//////////////////////////////////////
		Interpolation_BGGR_MotoDemosaic( Raw8, pDstBuffer,  width,  height);
		break;
	} 
	delete Raw8;
	Raw8 = NULL;
	return TRUE;
}

BOOL ImageProc::RawtoRGB24_MotoDemosaic( BYTE *pSrcBuffer, BYTE* pDstBuffer, int width, int height, BYTE outformat )
{	    
	switch( outformat )
	{
	case 0: //RGGB
		Interpolation_RGGB_MotoDemosaic( pSrcBuffer,  pDstBuffer,  width,  height);
		break;
	case 1: //GRBG
		Interpolation_GRBG_MotoDemosaic( pSrcBuffer,  pDstBuffer,  width,  height);
		break;
	case 2:   //GBRG
		Interpolation_GBRG_MotoDemosaic( pSrcBuffer,  pDstBuffer,  width,  height);
		break; 
	case 3:  //BGGR		
		//////////////////////////////////////
		Interpolation_BGGR_MotoDemosaic( pSrcBuffer, pDstBuffer,  width,  height);
		break;
	} 
	return TRUE;
}


BOOL ImageProc::Interpolation_RGGB_MotoDemosaic(BYTE* pSrcBuffer, BYTE* pDstBuffer, int width, int height)
{
	int x = 0,y = 0,i=0;
	int tempData = 0;
	{
		//////////////////////////////		
		//R G R G R G R G
		//G B G B G B G B
		//R G R G R G R G
		//G B G B G B G B
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
		for(  x = 1; x < width -1; x++ )
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

				y = height-1;
				pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  ( pSrcBuffer[width+x-1] + pSrcBuffer[width+x+1] )/2;
				pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  ( pSrcBuffer[width+x] + pSrcBuffer[x-1] + pSrcBuffer[x+1] )/3 ;
				pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  pSrcBuffer[x];  
			} 
		}

		for(  y = 1;  y < height - 1; y++ )
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

		//Sub boundary case
		for( y = 1, x = 1; x < width - 1; x++ )
		{
			i = (height-1-y)*width + x;

			if( y%2 == 1 )
			{
				if( x%2 == 1 ) //G1
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5; 
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
				if( x%2 == 1)  //B
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
					pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] ) /4;//Here
					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1]) /4;
				}
				else  //G2
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5; 
					pDstBuffer[y * width * 3 + x * 3 + 2] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
				} 
			} 
		}

		for( y = height-2, x = 1; x < width - 1; x++ )
		{
			i = (height-1-y)*width + x;

			if( y%2 == 1 )
			{
				if( x%2 == 1 ) //G1
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5; 
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
				if( x%2 == 1)  //B
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
					pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] ) /4;//Here
					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1]) /4;
				}
				else  //G2
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5; 
					pDstBuffer[y * width * 3 + x * 3 + 2] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
				} 
			}
		} 

		for( x = 1, y = 2; y < height - 2; y++ )
		{
			i = (height-1-y)*width + x;

			if( y%2 == 1 )
			{
				if( x%2 == 1 ) //G1
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5; 
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
				if( x%2 == 1)  //B
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
					pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] ) /4;//Here
					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1]) /4;
				}
				else  //G2
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5; 
					pDstBuffer[y * width * 3 + x * 3 + 2] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
				} 
			} 
		}

		for( x = width-2, y = 2; y < height - 2; y++ )
		{
			i = (height-1-y)*width + x;

			if( y%2 == 1 )
			{
				if( x%2 == 1 ) //G1
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5; 
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
				if( x%2 == 1)  //B
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];
					pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] ) /4;//Here
					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1]) /4;
				}
				else  //G2
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] =  ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2;
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5; 
					pDstBuffer[y * width * 3 + x * 3 + 2] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;
				} 
			} 
		}


		//other area
		for( y = 2; y < height-2; y++ )
		{
			for( x=2; x < width-2; x++ )
			{
				i = (height-1-y)*width + x;

				if( y%2 == 1 )
				{
					if( x%2 == 1 ) //G1
					{
						tempData  = int(  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )*0.5 +(pSrcBuffer[i]*0.625-
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1]+pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.125
							+(pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.0625) );

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}				

						pDstBuffer[ y * width * 3 + x * 3 + 0 ] = tempData;
						pDstBuffer[ y * width * 3 + x * 3 + 1 ] = pSrcBuffer[i];  

						tempData =  int( ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.5 + (pSrcBuffer[i]*0.625 -
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i-2]+pSrcBuffer[i+2]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1])*0.125+
							(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.0625) ); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 2 ] = tempData;


					}
					else  //R
					{
						//	pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] ) / 4;
						//	pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width]) / 4;
						tempData =   ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])*0.25+
							(pSrcBuffer[i]*0.75-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width]+pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.1875 ); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[ y * width * 3 + x * 3 + 0 ] = tempData;

						//G at R locations
						tempData =  int( ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.25+
							(pSrcBuffer[i]*0.5-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width] + pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.125));  

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}							
						pDstBuffer[ y * width * 3 + x * 3 + 1 ] = tempData;							
						pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   pSrcBuffer[i]; 

					}
				}
				else
				{
					if( x%2  == 1)  //B
					{
						pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];

						//G at B locations
						tempData = int(( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width ] )*0.25 +
							(pSrcBuffer[i]*0.5-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width] + pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.125));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[y * width * 3 + x * 3 + 1] = tempData;

						tempData = int(( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])*0.25+
							(pSrcBuffer[i]*0.75-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width]+pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.1875)); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[y * width * 3 + x * 3 + 2] = tempData;

					}
					else  //G2
					{ 
						tempData =  int(( pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.5 + (pSrcBuffer[i]*0.625 -
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i-2]+pSrcBuffer[i+2]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1])*0.125+
							(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.0625));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[y * width * 3 + x * 3 + 0] =  tempData; 

						pDstBuffer[y * width * 3 + x * 3 + 1] = pSrcBuffer[i];

						////R at green in B row, R column
						tempData = int((pSrcBuffer[i-width] + pSrcBuffer[i+width])*0.5+(pSrcBuffer[i]*0.625-
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1]+pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.125
							+(pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.0625));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[y * width * 3 + x * 3 + 2] = tempData;
					}

				}
			}
		}
	}
	return	TRUE;
}
BOOL ImageProc::Interpolation_GRBG_MotoDemosaic(BYTE* pSrcBuffer, BYTE* pDstBuffer, int width, int height)
{
	int x = 0,y = 0,i=0;
	int tempData = 0;

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
				i = ( height - 1 - y )*width;                       
				pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   pSrcBuffer[i];      
				pDstBuffer[ y * width * 3 + x * 3 + 1 ] =  (  pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i+1])/3 ;      
				pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width+1] + pSrcBuffer[i+width+1] )/2; 


				x = width -1;                                      
				i = ( height - 1 - y)* width + x;                      
				pDstBuffer[ y * width * 3 + x * 3 + 0 ] =  pSrcBuffer[i-1] ;          
				pDstBuffer[ y * width * 3 + x * 3 + 1 ] = (  pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i+width-1])/3;        
				pDstBuffer[ y * width * 3 + x * 3 + 2 ] =  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )/2;  

			}
		}

		//For sub boundary case
		for( y = 1, x = 1; x < width - 1; x++ )
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
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;


					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2; 
				}
			}
			else
			{
				if( x%2 == 1 ) //G2
				{   
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2;     
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;

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

		for( y = height-2, x = 1; x < width - 1; x++ )
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
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;


					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2; 
				}
			}
			else
			{
				if( x%2 == 1 ) //G2
				{   
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2;     
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;

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

		for( x = 1, y = 2; y < height - 2; y++ )
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
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;


					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2; 
				}
			}
			else
			{
				if( x%2 == 1 ) //G2
				{   
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2;     
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;

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

		for( x = width-2, y = 2; y < height - 2; y++ )
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
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;


					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2; 
				}
			}
			else
			{
				if( x%2 == 1 ) //G2
				{   
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1])/2;     
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;

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

		//other area
		for( y = 2; y < height- 2; y++ )
		{
			for( x = 2; x < width - 2; x++ )
			{
				i = ( height -1 -y)*width + x;

				if( y%2 == 1 )
				{
					if( x%2 == 1 ) //R
					{
						//	pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1] )/4;
						//	pDstBuffer[y * width * 3 + x * 3 + 1] = ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1])/4;
						//	pDstBuffer[y * width * 3 + x * 3 + 2] =  pSrcBuffer[i];
						tempData =   ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])*0.25+
							(pSrcBuffer[i]*0.75-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width]+pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.1875 ); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 0 ] = tempData;

						//G at R locations
						tempData =  int( ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.25+
							(pSrcBuffer[i]*0.5-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width] + pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.125));  

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 1 ] = tempData;							
						pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   pSrcBuffer[i]; 


					}
					else     //G1
					{
						//B at green in R row, B column
						tempData  = int(  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )*0.5 +(pSrcBuffer[i]*0.625-
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1]+pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.125
							+(pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.0625) );

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 0 ] = tempData;  
						pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    pSrcBuffer[i]; 
						tempData =  int( ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.5+ (pSrcBuffer[i]*0.625 -
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i-2]+pSrcBuffer[i+2]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1])*0.125+
							(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.0625) ); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 2 ] = tempData;
					}
				}
				else
				{
					if( x%2 == 1 ) //G2
					{  

						//B at green in B row, R column
						tempData =  int(( pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.5 + (pSrcBuffer[i]*0.625 -
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i-2]+pSrcBuffer[i+2]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1])*0.125+
							(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.0625));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[y * width * 3 + x * 3 + 0] =  tempData;
						pDstBuffer[y * width * 3 + x * 3 + 1] = pSrcBuffer[i];

						////R at green in B row, R column
						tempData = int((pSrcBuffer[i-width] + pSrcBuffer[i+width])*0.5+(pSrcBuffer[i]*0.625-
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1]+pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.125
							+(pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.0625));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[y * width * 3 + x * 3 + 2] = tempData;

					}
					else //B
					{

						pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];

						//G at B locations
						tempData = int(( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width ] )*0.25 +
							(pSrcBuffer[i]*0.5-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width] + pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.125));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[y * width * 3 + x * 3 + 1] = tempData;

						//
						tempData = int(( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])*0.25+
							(pSrcBuffer[i]*0.75-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width]+pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.1875)); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[y * width * 3 + x * 3 + 2] = tempData; 
					} 
				}
			} 
		} 
	}
	return TRUE;
}
BOOL ImageProc::Interpolation_GBRG_MotoDemosaic(BYTE* pSrcBuffer, BYTE* pDstBuffer, int width, int height)
{
	int x = 0,y = 0,i=0;
	int tempData = 0;

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

		//For Sub Boundary Case:
		for( y = 1, x = 1; x < width - 1; x++ )
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
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;

					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width] +  + pSrcBuffer[i+width])/2; 
				}
			}
			else
			{
				if( x%2 == 1 )  
				{   
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;     
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;


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

		for( y = height-2, x = 1; x < width - 1; x++ )
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
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;

					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width] +  + pSrcBuffer[i+width])/2; 
				}
			}
			else
			{
				if( x%2 == 1 )  
				{   
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;     
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;


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

		for( x = 1, y = 2; y < height - 2; y++ )
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
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;

					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width] +  + pSrcBuffer[i+width])/2; 
				}
			}
			else
			{
				if( x%2 == 1 )  
				{   
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;     
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;


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

		for( x = width-2, y = 2; y < height - 2; y++ )
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
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;

					pDstBuffer[y * width * 3 + x * 3 + 2] = ( pSrcBuffer[i-width] +  + pSrcBuffer[i+width])/2; 
				}
			}
			else
			{
				if( x%2 == 1 )  
				{   
					pDstBuffer[ y * width * 3 + x * 3 + 0 ] =   ( pSrcBuffer[i-width] + pSrcBuffer[i+width])/2;     
					pDstBuffer[y * width * 3 + x * 3 + 1] =   (pSrcBuffer[i] + pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1]
					+ pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])/5;


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

		//other area
		for( y = 2; y < height-2; y++ )
		{
			for( x = 2; x < width-2; x++ )
			{
				i = ( height -1 -y)*width + x;

				if( y%2 == 1 )
				{
					if( x%2 == 1 )  
					{
						//B
						pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];

						//G at B locations
						tempData = int(( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width ] )*0.25 +
							(pSrcBuffer[i]*0.5-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width] + pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.125));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[y * width * 3 + x * 3 + 1] = tempData; 

						tempData = int(( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])*0.25+
							(pSrcBuffer[i]*0.75-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width]+pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.1875)); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[y * width * 3 + x * 3 + 2] = tempData;

					}
					else      
					{							 
						tempData =  int(( pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.5 + (pSrcBuffer[i]*0.625 -
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i-2]+pSrcBuffer[i+2]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1])*0.125+
							(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.0625));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[y * width * 3 + x * 3 + 0] =  tempData;
						pDstBuffer[y * width * 3 + x * 3 + 1] = pSrcBuffer[i];

						//R at green in B row, R column
						tempData = int((pSrcBuffer[i-width] + pSrcBuffer[i+width])*0.5+(pSrcBuffer[i]*0.625-
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1]+pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.125
							+(pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.0625));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[y * width * 3 + x * 3 + 2] = tempData;
					}
				}
				else
				{
					if( x%2 == 1 )  
					{ 	 
						//B at green in R row, B column
						tempData  = int(  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )*0.5 +(pSrcBuffer[i]*0.625-
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1]+pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.125
							+(pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.0625) );

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 0 ] = tempData;


						pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    pSrcBuffer[i]; 


						tempData =  int( ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.5+ (pSrcBuffer[i]*0.625 -
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i-2]+pSrcBuffer[i+2]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1])*0.125+
							(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.0625) ); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 2 ] = tempData;


					}
					else  
					{
						tempData =   ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])*0.25+
							(pSrcBuffer[i]*0.75-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width]+pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.1875 ); 
						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 0 ] = tempData;

						//G at R locations
						tempData =  int( ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.25+
							(pSrcBuffer[i]*0.5-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width] + pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.125));  

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}							
						pDstBuffer[ y * width * 3 + x * 3 + 1 ] = tempData;							
						pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   pSrcBuffer[i]; 
					} 
				}
			} 
		} 
	}

	return TRUE;
}
BOOL ImageProc::Interpolation_BGGR_MotoDemosaic(BYTE* pSrcBuffer, BYTE *pDstBuffer, int width, int height)
{
	int x = 0,y = 0,i=0;
	int tempData = 0;
	//////////////////////////////////////
	{
		//Corner case
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

		//sub boundary case  
		for( y = 1, x = 1; x < width - 1; x++ )
		{
			i = (height-1-y)*width + x;

			if( y%2 == 1 )
			{
				if( x%2 == 1 )  
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
					pDstBuffer[y * width * 3 + x * 3 + 1] = pSrcBuffer[i]; 
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
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    pSrcBuffer[i]; 
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
				}						
			} 
		}

		for( y = height-2, x = 1; x < width - 1; x++ )
		{
			i = (height-1-y)*width + x;

			if( y%2 == 1 )
			{
				if( x%2 == 1 )  
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
					pDstBuffer[y * width * 3 + x * 3 + 1] = pSrcBuffer[i]; 
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
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    pSrcBuffer[i]; 
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
				}						
			} 
		}

		for( x = 1, y = 2; y < height - 2; y++ )
		{
			i = (height-1-y)*width + x;

			if( y%2 == 1 )
			{
				if( x%2 == 1 )  
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
					pDstBuffer[y * width * 3 + x * 3 + 1] = pSrcBuffer[i]; 
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
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    pSrcBuffer[i]; 
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
				}						
			} 
		}

		for( x = width-2, y = 2; y < height - 2; y++ )
		{
			i = (height-1-y)*width + x;

			if( y%2 == 1 )
			{
				if( x%2 == 1 )  
				{
					pDstBuffer[y * width * 3 + x * 3 + 0] = ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
					pDstBuffer[y * width * 3 + x * 3 + 1] = pSrcBuffer[i]; 
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
					pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    pSrcBuffer[i]; 
					pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )/2; 
				}						
			} 
		}

		//other area
		for( y = 2; y < height-2; y++ )
		{
			for( x = 2; x < width-2; x++ )
			{
				i = (height-1-y)*width + x;

				if( y%2 == 1 )
				{
					if( x%2 == 1 )  
					{
						//B at green in B row, R column
						tempData =  int(( pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.5 + (pSrcBuffer[i]*0.625 -
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i-2]+pSrcBuffer[i+2]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1])*0.125+
							(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.0625));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[y * width * 3 + x * 3 + 0] =  tempData; 

						pDstBuffer[y * width * 3 + x * 3 + 1] = pSrcBuffer[i];

						////R at green in B row, R column
						tempData = int((pSrcBuffer[i-width] + pSrcBuffer[i+width])*0.5+(pSrcBuffer[i]*0.625-
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1]+pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.125
							+(pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.0625));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						} 

						pDstBuffer[y * width * 3 + x * 3 + 2] = tempData;
					}
					else      
					{
						pDstBuffer[y * width * 3 + x * 3 + 0] = pSrcBuffer[i];

						//G at B locations
						tempData = int(( pSrcBuffer[i-width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] + pSrcBuffer[i+width ] )*0.25 +
							(pSrcBuffer[i]*0.5-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width] + pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.125));

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[y * width * 3 + x * 3 + 1] = tempData;

						//
						tempData = int(( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])*0.25+
							(pSrcBuffer[i]*0.75-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width]+pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.1875)); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[y * width * 3 + x * 3 + 2] = tempData;

					}
				}
				else
				{
					if( x%2 == 1 )  
					{   
						tempData =   ( pSrcBuffer[i-width-1] + pSrcBuffer[i-width+1] + pSrcBuffer[i+width-1] + pSrcBuffer[i+width+1])*0.25+
							(pSrcBuffer[i]*0.75-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width]+pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.1875 ); 
						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 0 ] = tempData;

						//G at R locations
						tempData =  int( ( pSrcBuffer[i-width] + pSrcBuffer[i+width] + pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.25+
							(pSrcBuffer[i]*0.5-(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width] + pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.125));  

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 1 ] = tempData;							
						pDstBuffer[ y * width * 3 + x * 3 + 2 ] =   pSrcBuffer[i]; 
					}
					else  
					{
						//B at green in R row, B column
						tempData  = int(  ( pSrcBuffer[i-width] + pSrcBuffer[i+width] )*0.5 +(pSrcBuffer[i]*0.625-
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1]+pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.125
							+(pSrcBuffer[i-2]+pSrcBuffer[i+2])*0.0625) );

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 0 ] = tempData;


						pDstBuffer[ y * width * 3 + x * 3 + 1 ] =    pSrcBuffer[i]; 


						tempData =  int( ( pSrcBuffer[i-1] + pSrcBuffer[i+1] )*0.5+ (pSrcBuffer[i]*0.625 -
							(pSrcBuffer[i-width-1]+pSrcBuffer[i-width+1]+pSrcBuffer[i-2]+pSrcBuffer[i+2]+pSrcBuffer[i+width-1]+pSrcBuffer[i+width+1])*0.125+
							(pSrcBuffer[i-2*width]+pSrcBuffer[i+2*width])*0.0625) ); 

						if( tempData < 0 )
						{
							tempData = 0;
						}
						else if( tempData > 255 )
						{
							tempData = 255;
						}

						pDstBuffer[ y * width * 3 + x * 3 + 2 ] = tempData;

					}						
				}
			}				
		}  
	}
	return TRUE; 
} 

BOOL ImageProc::CircleArrayPatternAnalysis(BYTE* RGB_buf,int ImgWidth,int ImgHeight,int PointSizeLimit,vector<float> &Out_x,vector<float> &Out_y,float &Mark_x,float &Mark_y )
{
	unsigned char *pBmpRed ;
	unsigned char *pBmpGreen ;
	unsigned char *pBmpBlue ;

	pBmpRed = (unsigned char *)malloc(ImgWidth * ImgHeight);
	pBmpGreen = (unsigned char *)malloc(ImgWidth * ImgHeight);
	pBmpBlue = (unsigned char *)malloc(ImgWidth * ImgHeight);

	for (int i=0; i<ImgHeight; i++)
	{
		for (int j=0; j<ImgWidth; j++)
		{
			pBmpBlue[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3];
			pBmpGreen[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3+1];
			pBmpRed[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3+2];
		}
	}

	// Local iconic variables 
	Hobject  image, grayImage, Region, ConnectedRegions,ConnectedRegions1,meanImage1,meanImage2,subImage,emphasizeImage;
	Hobject  SelectedRegions, RegionDifference, RegionOpening,SelectedRegions1;

	// Local control variables 
	HTuple  width, height, WindowHandle, Area, Row;
	HTuple  Column, Area1, Row1, Column1,Index,NumberResult;

	gen_image3_extern(&image, "byte", ImgWidth, ImgHeight, (long)pBmpRed, (long)pBmpGreen, (long)pBmpBlue, (long)free);
	get_image_size(image, &width, &height);	

	rgb1_to_gray(image, &grayImage);
	mean_image(grayImage,&meanImage1,9,9);
	mean_image(grayImage,&meanImage2,1000,1000);
	sub_image(meanImage1,meanImage2,&subImage,1,128);
	emphasize(subImage,&emphasizeImage,width,height,1); 

	bin_threshold(emphasizeImage, &Region);
	connection(Region, &ConnectedRegions);
	select_shape(ConnectedRegions, &SelectedRegions, "circularity", "and", 0.8, 1);
	difference(Region, SelectedRegions, &RegionDifference);

	//得到mark的坐标
	{
		opening_circle(RegionDifference, &RegionOpening, 3.5);
		connection(RegionOpening,&ConnectedRegions1);
		select_shape_std(ConnectedRegions1,&SelectedRegions1, "max_area", 70);
		area_center(SelectedRegions1, &Area, &Row, &Column);
		tuple_length(Row,&NumberResult);
		if(NumberResult[0].I()<0)
		{
			image.~Hobject();
			grayImage.~Hobject();
			Region.~Hobject();
			ConnectedRegions.~Hobject();
			SelectedRegions.~Hobject();
			RegionDifference.~Hobject();
			RegionOpening.~Hobject();
			ConnectedRegions1.~Hobject();
			meanImage1.~Hobject();
			meanImage2.~Hobject();
			subImage.~Hobject();
			emphasizeImage.~Hobject();
			return FALSE;
		}
		Mark_y = Row[0].D();
		Mark_x = Column[0].D();
	}

	//得到circle的坐标
	{
		area_center(SelectedRegions, &Area1, &Row1, &Column1);//得到circle的坐标
		tuple_length(Row1,&NumberResult);
		if(NumberResult[0].I()<0)
		{
			image.~Hobject();
			grayImage.~Hobject();
			Region.~Hobject();
			ConnectedRegions.~Hobject();
			SelectedRegions.~Hobject();
			RegionDifference.~Hobject();
			RegionOpening.~Hobject();
			ConnectedRegions1.~Hobject();
			meanImage1.~Hobject();
			meanImage2.~Hobject();
			subImage.~Hobject();
			emphasizeImage.~Hobject();
			return FALSE;
		}
		//count_obj(Row1, &NumberResult);
		for(Index = 0;Index<NumberResult;Index++)
		{
			Out_x.push_back(Column1[Index].D());
			Out_y.push_back(Row1[Index].D());
		}
	}

	image.~Hobject();
	grayImage.~Hobject();
	Region.~Hobject();
	ConnectedRegions.~Hobject();
	SelectedRegions.~Hobject();
	RegionDifference.~Hobject();
	RegionOpening.~Hobject();
	ConnectedRegions1.~Hobject();
	meanImage1.~Hobject();
	meanImage2.~Hobject();
	subImage.~Hobject();
	emphasizeImage.~Hobject();
	return TRUE;

}

void ImageProc::ConvertWideToChar(CString wide,char* mchar)
{ 
	int len = wide.GetLength();
	WideCharToMultiByte(CP_ACP,0,wide,len,mchar,len,NULL,NULL);//T2A,W2A
	//mchar[len+1] = '\0';
}

void ImageProc::ZoomImgSize(BYTE* RGB_buf,int ImgWidth,int ImgHeight,int destwidth,int destheight,CString path,CString strType)
{
	BYTE* pBmpRed = NULL ;
	BYTE *pBmpGreen = NULL ;
	BYTE *pBmpBlue = NULL ;

	pBmpRed =  new BYTE[ImgWidth * ImgHeight];
	pBmpGreen = new BYTE[ImgWidth * ImgHeight];
	pBmpBlue = new BYTE[ImgWidth * ImgHeight];

	for (int i=0; i<ImgHeight; i++)
	{
		for (int j=0; j<ImgWidth; j++)
		{
			pBmpBlue[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3];
			pBmpGreen[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3+1];
			pBmpRed[i*ImgWidth+j] = RGB_buf[(i*ImgWidth+j)*3+2];
		}
	}

	// Local iconic variables 
	Hobject  image, ImageZoom;
	  
	gen_image3_extern(&image, "byte", ImgWidth, ImgHeight, (long)pBmpRed, (long)pBmpGreen, (long)pBmpBlue, (long)0);//free
	  
	zoom_image_size(image, &ImageZoom, destwidth, destheight, "bilinear");
	char file_path[512] = {0};
	ConvertWideToChar(path,file_path);
	char cType[512] = {0};
	ConvertWideToChar(strType,cType);
	
	write_image(ImageZoom,cType,0,file_path);
		 
	
	if(pBmpRed!=NULL)
	{
       delete pBmpRed;
	   pBmpRed = NULL;
	}

	if(pBmpGreen!=NULL)
	{
       delete pBmpGreen;
	   pBmpGreen = NULL;
	}

	if(pBmpBlue!=NULL)
	{
       delete pBmpBlue;
	   pBmpBlue = NULL;
	}

	image.~Hobject();
	ImageZoom.~Hobject();

}

void ImageProc::SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist)
{
	if(SrcString == _T("")) return;
	strlist->clear();
	CString temp, str;
	int nStart = 0, index = 0;
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //判断是否已','结尾
	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//如果最后一个不是分割字符，再把后面的一段保存再退出
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);

				if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
				strlist->push_back(str);
			}
			break;//如果最后一个就是分割字符，退出
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //如果为空，默认“0”,防止数据库字符串为空出错
		strlist->push_back(str);
		nStart = index + 1;
	}
}

float ImageProc::GetSrValue(float ois_on_dif,float ois_off_dif)
{
	float sr = -20*log((ois_on_dif)/(ois_off_dif))/log(10.0);
	return sr;
}
 
 