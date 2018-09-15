#include "stdafx.h"
#include "AutoROI.h"
#include "SFRROISelect.h"
#include "math.h"
#include "resource.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
CSFRROISelect SpecDlg;
int  bSaveErrorImage;
int bDebugMode;
long  iErrorTime;
#define M_PI  3.14159265358979323846
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


struct ROIposition
{
	int Horizontal;    //水平方向
	int Vertical;        //竖直方向
	CPoint coordinate;
};

struct nLine 
{
	double k;
	double b;
};
//USHORT myCamID = 0;
void  DebugInfo(USHORT camID,char * format,...)
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
	if (0 == camID)
	{
		CreateDirectory("DebugInfo_Cam0",NULL);
		sprintf(FileNameTemp,"DebugInfo_Cam0\\DebugInfo_%04d%02d%02d_%02d.log",NowTime.wYear,NowTime.wMonth,NowTime.wDay,NowTime.wHour);
	} 
	else
	{
		CreateDirectory("DebugInfo_Cam1",NULL);
		sprintf(FileNameTemp,"DebugInfo_Cam1\\DebugInfo_%04d%02d%02d_%02d.log",NowTime.wYear,NowTime.wMonth,NowTime.wDay,NowTime.wHour);
	}
	

	sprintf(TimeTemp,"%04d-%02d-%02d %02d:%02d:%02d:%03d",NowTime.wYear,NowTime.wMonth,NowTime.wDay,NowTime.wHour,NowTime.wMinute,NowTime.wSecond,NowTime.wMilliseconds);
	FILE *pFIle=fopen(FileNameTemp,"a");
	if(pFIle != NULL)
	{
		strcpy(FileContent,TimeTemp);
		strcat(FileContent,"   ");
		strcat(FileContent,buf);
		strcat(FileContent,"\n");
		fwrite(FileContent,1,strlen(FileContent),pFIle);
		fclose(pFIle);
	}

	
}

void ShowROIInfoDlg(HWND hwnd, BOOL bShowFlag )
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
		SpecDlg.Create(IDD_DIALOG1,CWnd::FromHandle(GetDesktopWindow()));
		SpecDlg.ModifyStyle(WS_CHILD,DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_SYSMENU);
		SpecDlg.SetWindowText("AutoROI_Select_Setting");
		SpecDlg.CenterWindow(CWnd::FromHandle(GetDesktopWindow() ) );
	}	
	else
	{
		SpecDlg.Create(IDD_DIALOG1,pwnd);
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

void P10toRaw8(BYTE *Src, BYTE *Dest, int iWidth, int iHeight)
{
	//输入、输出可以是同一个buffer
	long DesCount = 0;

	for (long i = 0; i < ( iWidth*iHeight ) ; i++ )
	{
		Dest[DesCount++] = (BYTE)((Src[i*2]+Src[i*2+1]*256)/4.0);
	}
}

void R10toRaw8(BYTE *Src, BYTE *Dest, int iWidth, int iHeight)
{
	//输入、输出可以是同一个buffer
	long DesCount = 0;

	for (long i = 0; i < (iWidth*iHeight*1.25); i = i + 5)
	{
		Dest[DesCount++] = Src[i];
		Dest[DesCount++] = Src[i+1];
		Dest[DesCount++] = Src[i+2];
		Dest[DesCount++] = Src[i+3];
	}
}

void GetGChannelValue(BYTE *Src, BYTE *Dest, int iWidth, int iHeight)
{
	//输入、输出需要是不同的buffer
	for (int i = 0; i < iHeight ; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{         
			Dest[(iHeight- i)*iWidth +j] = Src[(i*iWidth+j)*3+1];		
		}
	}
}

void BayerConversionFlip( BYTE* pSrcBuffer, BYTE* pDstBuffer, int nImgWidth, int nImgHeight, int bayerType )
{
	int width = nImgWidth;
	int height  = nImgHeight;

	int x = 0; 
	int y = 0;
	int i = 0;

	switch( bayerType )
	{
	case OUTFORMAT_RGGB_: 
		{		
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

	case OUTFORMAT_GRBG_: //GRBG
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

	case OUTFORMAT_GBRG_:   //GBRG
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

	case OUTFORMAT_BGGR_:  //BGGR		
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

void GetSettingFromFile(USHORT camID,float *circleS, float *circleRMax,float *circleRMin)
{

	CString m_szSetup =SpecDlg.GetModulePath();

	if (0 == camID)
		{
			m_szSetup = m_szSetup + _T("ofset_Cam0.ini");
		} 
		else
		{
			m_szSetup = m_szSetup + _T("ofset_Cam1.ini");
		}
		

		char Temp[256] = {0};

		GetPrivateProfileString("SFRROIPosition","CircleS", "1", Temp, sizeof(Temp) , m_szSetup);
		*circleS =  atof(Temp);
		GetPrivateProfileString("SFRROIPosition","CircleRMax", "1", Temp, sizeof(Temp) , m_szSetup);
		*circleRMax =  atof(Temp);
		GetPrivateProfileString("SFRROIPosition","CircleRMin", "1", Temp, sizeof(Temp) , m_szSetup);
		*circleRMin =  atof(Temp);

		return;
}

BOOL GetSettingFromFile(USHORT camID,ROIposition *ofROIposition, int ofwidth, int ofheight, int *ofROISize, int *Offset,float *Threshold1,float *Threshold2)
{
	//获取要求计算的ROI、以及ROI长宽
	BOOL tempFlag = FALSE;

	CString m_szSetup =SpecDlg.GetModulePath();
	if (0==camID)
	{
		m_szSetup = m_szSetup + _T("ofset_Cam0.ini");
	}
	else
	{
		m_szSetup = m_szSetup + _T("ofset_Cam1.ini");
	}
	CString tempStr;
	tempStr.Format(_T("CamID:%d,Setting Fil:%s"),camID,m_szSetup);
	OutputDebugString(tempStr);
	CString tempStringH = _T("");
	CString tempStringV = _T("");
	char section[256] = {0};
	int  tempdata = 0;
	for(int i=0;i<MAX_ROI_NUMBER;i++)
	{
		tempStringH.Format(_T("H_B"));
		tempStringV.Format(_T("V_B"));
		sprintf_s(section, "%d",(i+1));
		tempStringH += section;
		tempStringV += section;

		tempdata = GetPrivateProfileInt("SFRROIPosition",tempStringH,0,m_szSetup);
		ofROIposition[i].Horizontal = tempdata;
		tempdata = GetPrivateProfileInt("SFRROIPosition",tempStringV,0,m_szSetup);
		ofROIposition[i].Vertical = tempdata;

		if ((ofROIposition[i].Horizontal)||(ofROIposition[i].Vertical))
		{
			tempFlag = TRUE;
		}
	}

	bDebugMode = GetPrivateProfileInt("SFRROIPosition","bDebugMode",0,m_szSetup);
	bSaveErrorImage  = GetPrivateProfileInt("SFRROIPosition","SaveErrorImage",0,m_szSetup);
	iErrorTime  = GetPrivateProfileInt("SFRROIPosition","iErrorTime",0,m_szSetup);
	char Temp[512]={0};

	*ofROISize = GetPrivateProfileInt("SFRROIPosition","ROI_Size",128,m_szSetup);
	*Offset = GetPrivateProfileInt("SFRROIPosition","Offset",128,m_szSetup);

	GetPrivateProfileString("SFRROIPosition","Threshold1", "0.25", Temp, sizeof(Temp) , m_szSetup);
	*Threshold1 =  atof(Temp);
	GetPrivateProfileString("SFRROIPosition","Threshold2", "0.5", Temp, sizeof(Temp) , m_szSetup);
	*Threshold2 =  atof(Temp);

	int  nCnt = 0;
	//中心
	ofROIposition[nCnt].coordinate.x = (int)(0.5 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.5 * ofheight);
	//0.3 视场
	ofROIposition[nCnt].coordinate.x = (int)(0.35 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.35 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.65 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.35 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.65 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.65 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.35 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.65 * ofheight);
	//0.4 视场
	ofROIposition[nCnt].coordinate.x = (int)(0.3 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.3 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.7 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.3 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.7 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.7 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.3 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.7 * ofheight);
	//0.5 视场
	ofROIposition[nCnt].coordinate.x = (int)(0.25 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.25 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.75 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.25 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.75 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.75 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.25 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.75 * ofheight);
	//0.6视场
	ofROIposition[nCnt].coordinate.x = (int)(0.2 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.2 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.8 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.2 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.8 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.8 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.2 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.8 * ofheight);
	//0.7视场
	ofROIposition[nCnt].coordinate.x = (int)(0.15 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.15 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.85 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.15 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.85 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.85 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.15 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.85 * ofheight);
	//0.8视场
	ofROIposition[nCnt].coordinate.x = (int)(0.1 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.1 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.9 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.1 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.9 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.9 * ofheight);
	ofROIposition[nCnt].coordinate.x = (int)(0.1 * ofwidth);
	ofROIposition[nCnt++].coordinate.y = (int)(0.9 * ofheight);

	return tempFlag;
}

BOOL GetRealCoord(USHORT camID,Mat SrcBuffer, int ofOffset , float Threshold1,float Threshold2, ROIposition *ofROIposition,int ofImageWidth,int ofImageHeight, OFAutoROIOut *ofROIoutInfo,char *ErrorMsg)
{
	OutputDebugString("Get Real Coord Start");
	vector<Point> centercood;
	vector<Point> Finalcood;
	int nCount = 0;
	int m_BlockVaild[MAX_ROI_NUMBER] = {0};
	for(int i=0;i<MAX_ROI_NUMBER;i++)
	{
		if ((ofROIposition[i].Horizontal)+(ofROIposition[i].Vertical))
		{
			Point coordinate(ofROIposition[i].coordinate.x,ofROIposition[i].coordinate.y);
			centercood.push_back(coordinate);
			m_BlockVaild[nCount++] = i+1 ;
		}
		ofROIoutInfo->ofROIposition[i]._x1 = 0.0;
		ofROIoutInfo->ofROIposition[i]._x2 = 0.0;
		ofROIoutInfo->ofROIposition[i]._y1 = 0.0;
		ofROIoutInfo->ofROIposition[i]._y2 = 0.0;
		ofROIoutInfo->ofROIposition[i]._A  = 0.0;
		ofROIoutInfo->ofROIposition[i]._B = 0.0;
		ofROIoutInfo->ofROIposition[i]._C = 0.0;
		ofROIoutInfo->ofROIposition[i]._D = 0.0;
	}
	OutputDebugString("ofROIoutInfo初始化");
	Mat midImage,dstImage;   
	int ROI_Size = ofROIoutInfo->ofROISize;

	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(SrcBuffer, SrcBuffer, MORPH_OPEN, element);
	morphologyEx(SrcBuffer, SrcBuffer, MORPH_CLOSE, element);
	OutputDebugString("Get Structuring Element and applies");
	vector<Point>::iterator iterpoint; 
	int  testtime = 0;
	OutputDebugString("For循环获取每个ROI的实际坐标");
	for (iterpoint = centercood.begin(),nCount = 0; iterpoint != centercood.end();nCount++)
	{
		int m_width  = (int)(ofImageWidth/8+0.5);     //10
		int m_height = (int)(ofImageHeight/6+0.5);    //7.5

		int startx = (*iterpoint).x - m_width/2;
		int starty = (*iterpoint).y - m_height/2;

		if (startx < 0 )    startx = 0;
		if (starty < 0 )    starty = 0;

		int endx = startx + m_width;
		int endy = starty + m_height;

		if (endx > (ofImageWidth-1))
		{	
			endx = ofImageWidth-1;
			startx = endx - m_width;
		}

		if (endy > (ofImageHeight-1))
		{	
			endy = ofImageHeight-1;
			starty = endy - m_height;
		}

		Rect roi(startx, starty, m_width, m_height);
		Mat  roi_of_image = SrcBuffer(roi);
#if 0
		cvtColor(roi_of_image,midImage, CV_RGB2GRAY); 
		CvScalar scalar = cvAvg(&midImage.operator IplImage());  
		Canny(midImage, dstImage, scalar.val[0] * Threshold1, scalar.val[0] * Threshold2, 3);
#else
		OutputDebugString("ROI灰阶转换");
		cvtColor(roi_of_image, midImage, CV_RGB2GRAY); 
		cv::Scalar scalar = cv::mean(midImage);

		cv::threshold(midImage, dstImage, 0, 255, CV_THRESH_OTSU);
		Canny(dstImage, dstImage, scalar.val[0] * Threshold1, scalar.val[0] * Threshold2, 3);
#endif
    	vector<Vec4i> lines;  
		OutputDebugString("HoughLinesP");
		if ((*iterpoint).x < (0.25*ofImageWidth)||(*iterpoint).x > (0.75*ofImageWidth))
		{
				HoughLinesP(dstImage, lines, 1, CV_PI/180, m_width*0.05, m_width*0.1, 10);
		}
		else
		{
				HoughLinesP(dstImage, lines, 1, CV_PI/180, m_width*0.07, m_width*0.15, 10);
		}

		if(bDebugMode)
		{
			char TimeTemp[256] = {0};
			Point cpoint((*iterpoint).x-startx,(*iterpoint).y-starty);
			circle(roi_of_image, cpoint, 1, Scalar(0,255,255),-1,8,0);
// 			sprintf(TimeTemp,"D:\\DebugMode\\%d_ROI.bmp",m_BlockVaild[nCount]);
// 			imwrite(TimeTemp,roi_of_image);  
		}
	
		vector<nLine> nline;
		//DebugInfo(camID,"Block%d 检测到%d条边",m_BlockVaild[nCount],lines.size());
		CString tempStr;
		tempStr.Format(_T("CamID:%d,Block%d 检测到%d条边"),camID,m_BlockVaild[nCount],lines.size());
		OutputDebugString(tempStr);
		for( size_t i = 0; i < lines.size(); i++ )
		{
			nLine m_Line;
			if (lines[i][2] != lines[i][0])
			{
				m_Line.k = 1.0*(lines[i][3] - lines[i][1])/(lines[i][2] - lines[i][0]);
				m_Line.b = lines[i][3] - m_Line.k * (lines[i][2]);

				nline.push_back(m_Line);
			}
			else
			{
				//DebugInfo(camID,"Block%d 一条边为水平直线",m_BlockVaild[nCount],lines.size());
				tempStr.Format(_T("CamID:%d,Block%d 一条边为水平直线"),camID,m_BlockVaild[nCount]);
				OutputDebugString(tempStr);
			}
		}
		OutputDebugString("水平直线检测完成");
		if (nline.size()==0)
		{
			//DebugInfo(camID,"Block%d一共检测到0条斜边\n",m_BlockVaild[nCount]);
			tempStr.Format(_T("CamID:%d,Block%d一共检测到0条斜边\n"),camID,m_BlockVaild[nCount]);
			OutputDebugString(tempStr);
			sprintf(ErrorMsg,"Block%d一共检测到0条斜边",m_BlockVaild[nCount]);
			return FALSE;
		}

		vector<nLine>::iterator iterline; 

		vector<nLine> nLineH;
		vector<nLine> nLineV;
		vector<nLine> nLineFinal;

		for (iterline = nline.begin(); iterline != nline.end(); iterline++)
		{	
			if (abs((*iterline).k)<1)
			{
				nLineH.push_back((*iterline));
			}
			else
			{
				nLineV.push_back((*iterline));
			}
		}
		OutputDebugString("水平、垂直分组完成");
		double m_coor_y[50] = {0.0};
		double m_distance_y[50] = {0.0}; 

		double temp_coor[4] = {0.0};
		double temp_A = 0.0;
		double temp_B = 0.0;
		double temp_C = 0.0;
		double temp_D = 0.0;

		if (nLineH.size()==0)
		{
			for( size_t i = 0; i < lines.size(); i++ )
			{
				Vec4i l = lines[i];
				line( roi_of_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
			}

			//DebugInfo(camID,"Block%d水平方向检测到0条斜边\n",m_BlockVaild[nCount]);
			tempStr.Format(_T("CamID:%d,Block%d水平方向检测到0条斜边\n"),camID,m_BlockVaild[nCount]);
			OutputDebugString(tempStr);
			sprintf(ErrorMsg,"Block%d水平方向检测到0条斜边",m_BlockVaild[nCount]);
			return FALSE;
		}
		else if (nLineH.size()<50)
		{
			int i = 0;
			for (iterline = nLineH.begin(),i=0; iterline != nLineH.end(); iterline++,i++)
			{
				m_coor_y[i] = (int)(((*iterline).k * m_width * 0.5) + (*iterline).b + 0.5);
				m_distance_y[i] = (m_coor_y[i] - m_height * 0.5);
			}

			double m_minDistance1= -1000;
			double m_minDistance2= 1000;
			int mmm1 = 0;
			int mmm2 = 0;
			for (int mm = 0; mm < i;mm++)
			{
				if ((m_minDistance1 < m_distance_y[mm])&&(m_distance_y[mm] < 0))
				{
					mmm1 = mm;
					m_minDistance1 = m_distance_y[mm];
				}
				if ((m_minDistance2 > m_distance_y[mm])&&(m_distance_y[mm] >= 0))
				{
					mmm2 = mm;
					m_minDistance2 = m_distance_y[mm];
				}
			}
			//DebugInfo(camID,"A = %.1f   B = %.1f", m_minDistance1,m_minDistance2);
			tempStr.Format(_T("CamID:%d,A = %.1f   B = %.1f\n"),camID,m_minDistance1,m_minDistance2);
			OutputDebugString(tempStr);
			if ((m_minDistance1 == -1000)||(m_minDistance2 == 1000))
			{
				for( size_t i = 0; i < lines.size(); i++ )
				{
					Vec4i l = lines[i];
					line( roi_of_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
				}

				//DebugInfo(camID,"Block%d水平方向检测不到两条边\n",m_BlockVaild[nCount]);
				tempStr.Format(_T("CamID:%d,Block%d水平方向检测不到两条边\n"),camID,m_BlockVaild[nCount]);
				OutputDebugString(tempStr);
				sprintf(ErrorMsg,"Block%d水平方向检测不到两条边",m_BlockVaild[nCount]);
				return FALSE;
			}

 			nLineFinal.push_back(nLineH.at(mmm1));
 			nLineFinal.push_back(nLineH.at(mmm2));

			temp_A = abs(m_minDistance1);
			temp_B = abs(m_minDistance2);

			temp_coor[0] = m_coor_y[mmm1];
			temp_coor[1] = m_coor_y[mmm2];
		}
		else
		{
			for( size_t i = 0; i < lines.size(); i++ )
			{
				Vec4i l = lines[i];
				line( roi_of_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
			}

			//DebugInfo(camID,"Block%d水平方向检测到斜边超过50条\n",m_BlockVaild[nCount]);
			tempStr.Format(_T("CamID:%d,Block%d水平方向检测到斜边超过50条\n"),camID,m_BlockVaild[nCount]);
			OutputDebugString(tempStr);
			sprintf(ErrorMsg,"Block%d水平方向检测到斜边超过50条",m_BlockVaild[nCount]);
			return FALSE;
		}
		OutputDebugString("水平方向斜线检测完成");
		double m_coor_x[50] = {0.0};
		double m_distance_x[50] = {0.0};

		if (nLineV.size()==0)
		{
			for( size_t i = 0; i < lines.size(); i++ )
			{
				Vec4i l = lines[i];
				line( roi_of_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
			}

			//DebugInfo(camID,"Block%d垂直方向检测到0条斜边\n",m_BlockVaild[nCount]);
			tempStr.Format(_T("CamID:%d,Block%d垂直方向检测到0条斜边\n"),camID,m_BlockVaild[nCount]);
			OutputDebugString(tempStr);
			sprintf(ErrorMsg,"Block%d垂直方向检测到0条斜边",m_BlockVaild[nCount]);
			return FALSE;
		}
		else if (nLineV.size()<50)
		{
			int i = 0;
			for (iterline = nLineV.begin(),i=0; iterline != nLineV.end(); iterline++,i++)
			{
				m_coor_x[i] = (int)((m_height * 0.5 -(*iterline).b)/(*iterline).k + 0.5);
				m_distance_x[i] = (m_coor_x[i] - m_width * 0.5);
			}

			double m_minDistance1= -1000;
			double m_minDistance2= 1000;
			int mmm1 = 0;
			int mmm2 = 0;

			for (int mm = 0; mm < i;mm++)
			{
				if ((m_minDistance1 < m_distance_x[mm])&&(m_distance_x[mm] < 0))
				{
					mmm1 = mm;
					m_minDistance1 = m_distance_x[mm];
				}
				if ((m_minDistance2 > m_distance_x[mm])&&(m_distance_x[mm] >= 0))
				{
					mmm2 = mm;
					m_minDistance2 = m_distance_x[mm];
				}
			}
			//DebugInfo(camID,"C = %.1f   D = %.1f", m_minDistance1,m_minDistance2);
			tempStr.Format(_T("CamID:%d,C = %.1f   D = %.1f\n"),camID,m_minDistance1,m_minDistance2);
			OutputDebugString(tempStr);
			if ((m_minDistance1 == -1000)||(m_minDistance2 == 1000))
			{
				for( size_t i = 0; i < lines.size(); i++ )
				{
					Vec4i l = lines[i];
					line( roi_of_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
				}

				//DebugInfo(camID,"Block%d垂直方向检测不到两条边\n",m_BlockVaild[nCount]);
				tempStr.Format(_T("CamID:%d,Block%d垂直方向检测不到两条边\n"),camID,m_BlockVaild[nCount]);
				OutputDebugString(tempStr);
				sprintf(ErrorMsg,"Block%d垂直方向检测不到两条边",m_BlockVaild[nCount]);
				return FALSE;
			}

			nLineFinal.push_back(nLineV.at(mmm1));
			nLineFinal.push_back(nLineV.at(mmm2));

			temp_C = abs(m_minDistance1);
			temp_D = abs(m_minDistance2);

			temp_coor[2] = m_coor_x[mmm1];
			temp_coor[3] = m_coor_x[mmm2];
		}
		else
		{
			for( size_t i = 0; i < lines.size(); i++ )
			{
				Vec4i l = lines[i];
				line( roi_of_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
			}

			//DebugInfo(camID,"Block%d垂直方向检测到斜边超过50条\n",m_BlockVaild[nCount]);
			tempStr.Format(_T("CamID:%d,Block%d垂直方向检测到斜边超过50条\n"),camID,m_BlockVaild[nCount]);
			OutputDebugString(tempStr);
			sprintf(ErrorMsg,"Block%d垂直方向检测到斜边超过50条",m_BlockVaild[nCount]);
			return FALSE;
		}
		OutputDebugString("垂直方向斜线检测完成");
		if(((temp_A+temp_B) < ROI_Size)||((temp_C+temp_D) < ROI_Size)||(min(temp_A,temp_B)==0)||(min(temp_C,temp_D)==0))
		{
			if (((temp_A+temp_B) < ROI_Size)||(min(temp_A,temp_B)==0))
			{
				if ( (*iterpoint).y < ofImageHeight*0.4)
				{
					(*iterpoint).y += 10;
				}
				else
				{
					(*iterpoint).y -= 10;
				}
			}
			if (((temp_C+temp_D) < ROI_Size)||(min(temp_C,temp_D)==0))
			{
				if ( (*iterpoint).x < ofImageWidth*0.4)
				{
					(*iterpoint).x += 10;
				}
				else
				{
					(*iterpoint).x -= 10;
				}
			}

			if (testtime > 5)
			{
				//DebugInfo(camID,"点到边的距离太近，微移多次仍不行");
				tempStr.Format(_T("CamID:%d,点到边的距离太近，微移多次仍不行\n"),camID);
				OutputDebugString(tempStr);
				sprintf(ErrorMsg,"中心点到边的距离太近");
				return FALSE;
			}

			Point recoordinate((*iterpoint).x,(*iterpoint).y);
			circle(SrcBuffer, recoordinate, 1, Scalar(255,0,255),-1,8,0);

			testtime++;
			nCount--;
			continue;
		}
		else if (((temp_A+temp_B)>(m_height*0.7))||((temp_C+temp_D)>(m_width*0.7)))
		{
			//DebugInfo(camID,"ROI之间距离太远，中间有边没检测到");
			tempStr.Format(_T("CamID:%d,ROI之间距离太远，中间有边没检测到\n"),camID);
			OutputDebugString(tempStr);
			sprintf(ErrorMsg,"ROI之间距离太远，中间有边没检测到");
			return FALSE;
		}
		
		testtime = 0;

		if(bDebugMode)
		{
			for( size_t i = 0; i < lines.size(); i++ )
			{
				Vec4i l = lines[i];
				line( roi_of_image, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
			}
		}
		
		if (ofROIposition[m_BlockVaild[nCount]-1].Vertical)
		{
			ofROIoutInfo->ofROIposition[nCount]._A = temp_A;
			ofROIoutInfo->ofROIposition[nCount]._B = temp_B;
			double mindistance = min(temp_C,temp_D);
			if (mindistance > (ROI_Size/2+ofOffset))
			{
				double kk = (nLineFinal.at(2).k+nLineFinal.at(3).k)*0.5 ;
				if (kk==0)
				{
					sprintf(ErrorMsg,"kk为0");
					return FALSE;
				}
				ofROIoutInfo->ofROIposition[nCount].ROIV1.LT.x =  ((int)(startx + m_width*0.5 - temp_A/kk -  ROI_Size*0.5 + 0.5))/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIV1.LT.y =  ((int)(starty + temp_coor[0] - ROI_Size*0.5 + 0.5))/2*2;		
				ofROIoutInfo->ofROIposition[nCount].ROIV1.RB.x = ofROIoutInfo->ofROIposition[nCount].ROIV1.LT.x + ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIV1.RB.y = ofROIoutInfo->ofROIposition[nCount].ROIV1.LT.y + ROI_Size;

				ofROIoutInfo->ofROIposition[nCount].ROIV2.LT.x =  ((int)(startx + m_width*0.5 + temp_B/kk - ROI_Size*0.5 + 0.5))/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIV2.LT.y =  ((int)(starty + temp_coor[1] - ROI_Size*0.5 + 0.5))/2*2;		
				ofROIoutInfo->ofROIposition[nCount].ROIV2.RB.x = ofROIoutInfo->ofROIposition[nCount].ROIV2.LT.x + ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIV2.RB.y = ofROIoutInfo->ofROIposition[nCount].ROIV2.LT.y + ROI_Size;

				//DebugInfo(camID,"Block%d V方向2个ROI",m_BlockVaild[nCount]);
				tempStr.Format(_T("CamID:%d,Block%d V方向2个ROI\n"),camID,m_BlockVaild[nCount]);
				OutputDebugString(tempStr);
			}
			else
			{
				int xxx1,yyy1,xxx2,yyy2,yyy3,yyy4;
				double kk = (nLineFinal.at(2).k+nLineFinal.at(3).k)*0.5;
				if (kk==0)
				{
					sprintf(ErrorMsg,"kk为0");
					return FALSE;
				}
				if (temp_C > temp_D)
				{
					xxx1 = (int)(temp_coor[3] - ROI_Size*0.5 - (temp_C + temp_D)/6.0 +0.5);
					xxx2 = (int)(temp_coor[3] + ROI_Size*0.5 + (temp_C + temp_D)/6.0 +0.5);

					ofROIoutInfo->ofROIposition[nCount]._x1 = (xxx2 - xxx1)*0.5 -  temp_D;
					ofROIoutInfo->ofROIposition[nCount]._x2 = (xxx2 - xxx1)*0.5 + temp_D;
				}
				else
				{
					xxx1 = (int)(temp_coor[2] - ROI_Size*0.5 -(temp_C + temp_D)/6.0 +0.5);
					xxx2 = (int)(temp_coor[2] + ROI_Size*0.5 + (temp_C + temp_D)/6.0 +0.5);

					ofROIoutInfo->ofROIposition[nCount]._x1 = (xxx2 - xxx1)*0.5 +  temp_C;
					ofROIoutInfo->ofROIposition[nCount]._x2 = (xxx2 - xxx1)*0.5 -  temp_C;
				}

				yyy1 = (int)(nLineFinal.at(0).k * (xxx1- temp_A/kk) +  nLineFinal.at(0).b + 0.5 );
				yyy2 = (int)(nLineFinal.at(1).k * (xxx1+temp_B/kk) +  nLineFinal.at(1).b + 0.5 );
				yyy3 = (int)(nLineFinal.at(0).k * (xxx2- temp_A/kk) +  nLineFinal.at(0).b + 0.5 );
				yyy4 = (int)(nLineFinal.at(1).k * (xxx2+temp_B/kk) +  nLineFinal.at(1).b + 0.5 );

				ofROIoutInfo->ofROIposition[nCount].ROIV1.LT.x =  (startx + xxx1-  temp_A/kk - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIV2.LT.x =  (startx + xxx1+ temp_B/kk - ROI_Size/2)/2*2;	
				ofROIoutInfo->ofROIposition[nCount].ROIV3.LT.x =  (startx + xxx2-  temp_A/kk- ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIV4.LT.x =  (startx + xxx2+ temp_B/kk- ROI_Size/2)/2*2;	

				ofROIoutInfo->ofROIposition[nCount].ROIV1.LT.y =  (starty + yyy1 - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIV2.LT.y =  (starty + yyy2 - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIV3.LT.y =  (starty + yyy3 - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIV4.LT.y =  (starty + yyy4 - ROI_Size/2)/2*2;

				ofROIoutInfo->ofROIposition[nCount].ROIV1.RB.x =  ofROIoutInfo->ofROIposition[nCount].ROIV1.LT.x +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIV2.RB.x =  ofROIoutInfo->ofROIposition[nCount].ROIV2.LT.x +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIV3.RB.x =  ofROIoutInfo->ofROIposition[nCount].ROIV3.LT.x +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIV4.RB.x =  ofROIoutInfo->ofROIposition[nCount].ROIV4.LT.x +ROI_Size;

				ofROIoutInfo->ofROIposition[nCount].ROIV1.RB.y =  ofROIoutInfo->ofROIposition[nCount].ROIV1.LT.y +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIV2.RB.y =  ofROIoutInfo->ofROIposition[nCount].ROIV2.LT.y +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIV3.RB.y =  ofROIoutInfo->ofROIposition[nCount].ROIV3.LT.y +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIV4.RB.y =  ofROIoutInfo->ofROIposition[nCount].ROIV4.LT.y +ROI_Size;

				//DebugInfo(camID,"x1 = %.1f, x2 = %.1f",ofROIoutInfo->ofROIposition[nCount]._x1,ofROIoutInfo->ofROIposition[nCount]._x2);
				//DebugInfo(camID,"Block%d V方向4个ROI",m_BlockVaild[nCount]);
				tempStr.Format(_T("CamID:%d,x1 = %.1f, x2 = %.1f\n"),camID,ofROIoutInfo->ofROIposition[nCount]._x1,ofROIoutInfo->ofROIposition[nCount]._x2);
				OutputDebugString(tempStr);
				tempStr.Format(_T("CamID:%d,Block%d V方向4个ROI\n"),camID,m_BlockVaild[nCount]);
				OutputDebugString(tempStr);
			}
		}
		
		if (ofROIposition[m_BlockVaild[nCount]-1].Horizontal)
		{
			ofROIoutInfo->ofROIposition[nCount]._C = temp_C;
			ofROIoutInfo->ofROIposition[nCount]._D = temp_D;
			double mindistance = min(temp_A,temp_B);
			if (mindistance > (ROI_Size/2+ofOffset))
			{
				double kk = (nLineFinal.at(0).k+nLineFinal.at(1).k)*0.5 ;
				if (kk==0)
				{
					sprintf(ErrorMsg,"kk为0");
					return FALSE;
				}
				ofROIoutInfo->ofROIposition[nCount].ROIH1.LT.x =  ((int)(startx + temp_coor[2] - ROI_Size*0.5 + 0.5))/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIH1.LT.y =  ((int)(starty + m_height*0.5 - temp_C*kk - ROI_Size*0.5 + 0.5))/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIH1.RB.x = ofROIoutInfo->ofROIposition[nCount].ROIH1.LT.x + ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIH1.RB.y = ofROIoutInfo->ofROIposition[nCount].ROIH1.LT.y + ROI_Size;

				ofROIoutInfo->ofROIposition[nCount].ROIH2.LT.x =  ((int)(startx + temp_coor[3] - ROI_Size*0.5 + 0.5))/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIH2.LT.y =  ((int)(starty + m_height*0.5 + temp_D*kk - ROI_Size*0.5 + 0.5))/2*2;	
				ofROIoutInfo->ofROIposition[nCount].ROIH2.RB.x = ofROIoutInfo->ofROIposition[nCount].ROIH2.LT.x + ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIH2.RB.y = ofROIoutInfo->ofROIposition[nCount].ROIH2.LT.y + ROI_Size;

				//DebugInfo(camID,"Block%d H方向2个ROI",m_BlockVaild[nCount]);
				tempStr.Format(_T("CamID:%d,Block%d H方向2个ROI\n"),camID,m_BlockVaild[nCount]);
				OutputDebugString(tempStr);
			}
			else
			{
				int xxx1,yyy1,xxx2,yyy2,xxx3,xxx4;
				double kk = (nLineFinal.at(0).k+nLineFinal.at(1).k)*0.5 ;
				if (kk==0)
				{
					sprintf(ErrorMsg,"kk为0");
					return FALSE;
				}
				if (temp_A > temp_B)
				{
					yyy2 = (int)(temp_coor[1] - ROI_Size*0.5 - (temp_A + temp_B)/6.0 + 0.5);
					yyy1 = (int)(temp_coor[1] + ROI_Size*0.5 + (temp_A  + temp_B)/6.0 + 0.5);

					ofROIoutInfo->ofROIposition[nCount]._y1 = (yyy1 - yyy2)*0.5 +  temp_B;
					ofROIoutInfo->ofROIposition[nCount]._y2 = (yyy1 - yyy2)*0.5 - temp_B;
				}
				else
				{
					yyy2 = (int)(temp_coor[0] - ROI_Size*0.5 - (temp_A + temp_B)/6.0 +0.5);
					yyy1 = (int)(temp_coor[0] + ROI_Size*0.5 + (temp_A + temp_B)/6.0 +0.5);

					ofROIoutInfo->ofROIposition[nCount]._y1 = (yyy1 - yyy2)*0.5 - temp_A;
					ofROIoutInfo->ofROIposition[nCount]._y2 = (yyy1 - yyy2)*0.5 + temp_A;
				}
				 
				xxx1 = (int)((yyy1- temp_C*kk -nLineFinal.at(2).b)/nLineFinal.at(2).k + 0.5); 
				xxx2 = (int)((yyy1+temp_D*kk -nLineFinal.at(3).b)/nLineFinal.at(3).k + 0.5); 
				xxx3 = (int)((yyy2- temp_C*kk -nLineFinal.at(2).b)/nLineFinal.at(2).k + 0.5); 
				xxx4 = (int)((yyy2+temp_D*kk -nLineFinal.at(3).b)/nLineFinal.at(3).k + 0.5); 

				ofROIoutInfo->ofROIposition[nCount].ROIH1.LT.x =  (startx + xxx1 - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIH2.LT.x =  (startx + xxx2 - ROI_Size/2)/2*2;	
				ofROIoutInfo->ofROIposition[nCount].ROIH3.LT.x =  (startx + xxx3 - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIH4.LT.x =  (startx + xxx4 - ROI_Size/2)/2*2;	

				ofROIoutInfo->ofROIposition[nCount].ROIH1.LT.y =  (starty + yyy1 - temp_C*kk - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIH2.LT.y =  (starty + yyy1+ temp_D*kk - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIH3.LT.y =  (starty + yyy2 - temp_C*kk - ROI_Size/2)/2*2;
				ofROIoutInfo->ofROIposition[nCount].ROIH4.LT.y =  (starty + yyy2+ temp_D*kk - ROI_Size/2)/2*2;

				ofROIoutInfo->ofROIposition[nCount].ROIH1.RB.x =  ofROIoutInfo->ofROIposition[nCount].ROIH1.LT.x +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIH2.RB.x =  ofROIoutInfo->ofROIposition[nCount].ROIH2.LT.x +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIH3.RB.x =  ofROIoutInfo->ofROIposition[nCount].ROIH3.LT.x +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIH4.RB.x =  ofROIoutInfo->ofROIposition[nCount].ROIH4.LT.x +ROI_Size;

				ofROIoutInfo->ofROIposition[nCount].ROIH1.RB.y =  ofROIoutInfo->ofROIposition[nCount].ROIH1.LT.y +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIH2.RB.y =  ofROIoutInfo->ofROIposition[nCount].ROIH2.LT.y +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIH3.RB.y =  ofROIoutInfo->ofROIposition[nCount].ROIH3.LT.y +ROI_Size;
				ofROIoutInfo->ofROIposition[nCount].ROIH4.RB.y =  ofROIoutInfo->ofROIposition[nCount].ROIH4.LT.y +ROI_Size;

				//DebugInfo(camID,"y1 = %.1f, y2 = %.1f",ofROIoutInfo->ofROIposition[nCount]._y1,ofROIoutInfo->ofROIposition[nCount]._y2);
				//DebugInfo(camID,"Block%d H方向4个ROI",m_BlockVaild[nCount]);
				tempStr.Format(_T("CamID:%d,y1 = %.1f, y2 = %.1f\n"),camID,ofROIoutInfo->ofROIposition[nCount]._y1,ofROIoutInfo->ofROIposition[nCount]._y2);
				OutputDebugString(tempStr);
				tempStr.Format(_T("CamID:%d,Block%d H方向4个ROI\n"),camID,m_BlockVaild[nCount]);
				OutputDebugString(tempStr);
			}
		}
		 iterpoint++;
	}
//	imwrite("D:\\MarkROI.bmp",SrcBuffer); 
	ofROIoutInfo->ofROInumber = nCount;
	return TRUE;
}

BOOL bSaveBmpFile(CString sfilename, BYTE *pBuffer, UINT width, UINT height)
{
	int				 OffBits;
	HFILE			 bmpFile;
	BITMAPFILEHEADER bmpHeader;  // Header for Bitmap file
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
	len = _lwrite(bmpFile, (LPSTR)pBuffer,    bmpHeader.bfSize - sizeof(bmpHeader) - sizeof(bmpInfo) + 4);  //+4 is for exact filesize
	_lclose(bmpFile);

	return TRUE;
}

BOOL myVertFlipBuf(BYTE  * inbuf, 
	UINT widthBytes, 
	UINT height)
{   
	BYTE  *tb1;
	BYTE  *tb2;

	if (inbuf==NULL)
		return FALSE;

	UINT bufsize;

	bufsize=widthBytes;

	tb1= (BYTE *)new BYTE[bufsize];
	if (tb1==NULL) 
	{
		return FALSE;
	}
	memset(tb1,0,sizeof(BYTE)*bufsize);
	tb2= (BYTE *)new BYTE [bufsize];
	if (tb2==NULL) 
	{
		delete [] tb1;
		tb1 = NULL;
		return FALSE;
	}
	memset(tb2,0,sizeof(BYTE)*bufsize);
	UINT row_cnt;     
	ULONG off1=0;
	ULONG off2=0;

	for (row_cnt=0;row_cnt<(height+1)/2;row_cnt++) {
		off1=row_cnt*bufsize;
		off2=((height-1)-row_cnt)*bufsize;   

		memcpy(tb1,inbuf+off1,bufsize);
		memcpy(tb2,inbuf+off2,bufsize);	

		memcpy(inbuf+off1,tb2,bufsize);
		memcpy(inbuf+off2,tb1,bufsize);
	}	

	delete [] tb1;
	delete [] tb2;
	tb1 = NULL;
	tb2 = NULL;
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


BOOL fnAutoROISelect(OFAutoROIIn *ofSrcDataIn, OFAutoROIOut *ofDestDataOut,char *ErrorMsg)
{
	USHORT camID  = ofSrcDataIn->CamID;
	ROIposition ofROIposition[MAX_ROI_NUMBER];    //存储勾选的要测试的ROI测试位置，水平？竖直？
	int  ofROISize = 0;       //需要测试的ROI Size
	int ofOffset = 0;
	float Threshold1 = 0.0;
	float Threshold2 = 0.0;
	CString tempStr = _T("");
	BYTE *bmpBuffer = new BYTE[ofSrcDataIn->ofWidth*ofSrcDataIn->ofHeight*3];     //转后的Bmp Buffer
	if (bmpBuffer == NULL)
	{
		//DebugInfo(camID,"bmpbuffer new fail......");
		tempStr.Format(_T("CamID:%d,bmpbuffer new fail......\n"),camID);
		OutputDebugString(tempStr);
		return FALSE;
	}
	memset(bmpBuffer,0,sizeof(BYTE)*ofSrcDataIn->ofWidth*ofSrcDataIn->ofHeight*3);
	//调整传入的图像为BMP格式，以便代码统一处理
	if (ofSrcDataIn->ofImageType == RAW10_PACKED_IMAGE)
	{
		R10toRaw8(ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );
	}
	else if (ofSrcDataIn->ofImageType == RAW10_UNPACKED_IMAGE)
	{
		P10toRaw8(ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );	
	}
	else if (ofSrcDataIn->ofImageType == RAW8_IMAGE)
	{
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );
	}
	else if(ofSrcDataIn->ofImageType == BMP_IMAGE)
	{
		for (int hh = 0; hh < (ofSrcDataIn->ofWidth* ofSrcDataIn->ofHeight * 3); hh++)
		{
			bmpBuffer[hh] = ofSrcDataIn->ofSrcBuffer[hh];
		}
	}
	else
	{
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		strcpy(ErrorMsg,"传入的图片格式错误");
		return FALSE;
	}
	
	if(ofSrcDataIn->ofOutFormat != OUTFORMAT_FLIP_)
	{
		   myVertFlipBuf(bmpBuffer,ofSrcDataIn->ofWidth*3,ofSrcDataIn->ofHeight);
	//       bSaveBmpFile("D:\\AutoROI2.bmp",bmpBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
	}

	Mat srcImage(ofSrcDataIn->ofHeight,ofSrcDataIn->ofWidth,CV_8UC3,bmpBuffer);
	if (srcImage.empty())
	{
		OutputDebugString("srcImage加载图片失败");
		strcpy(ErrorMsg,"srcImage加载图片失败");
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		return FALSE;
	}
	if (srcImage.data == NULL)
	{
		//DebugInfo(camID,"opencv create Mat fail.........");
		tempStr.Format(_T("CamID:%d,Opencv Create Mat fail......\n"),camID);
		OutputDebugString(tempStr);
		strcpy(ErrorMsg,"srcImage Data 为空");
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		return FALSE;
	}
	//获取要求计算的ROI、以及ROI长宽
	//获取大致的ROI位置坐标
	tempStr.Format(_T("CamID:%d,获取抓框设置\n"),camID);
	OutputDebugString(tempStr);
	BOOL ret = FALSE;
	ret = GetSettingFromFile(camID,ofROIposition,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight,&ofROISize,&ofOffset,&Threshold1,&Threshold2);
	
	if (!ret)
	{
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		strcpy(ErrorMsg,"需要测试的ROI数量为0个，请选择ROI位置");
		return FALSE;
	}
	tempStr.Format(_T("CamID:%d,获取抓框设置完成\n"),camID);
	OutputDebugString(tempStr);
// 	计算各个ROI的实际坐标
	ofDestDataOut->ofROISize  = ofROISize;
	//DebugInfo(camID,"计算各个ROI的实际坐标");
	tempStr.Format(_T("CamID:%d,计算各个ROI的实际坐标\n"),camID);
	OutputDebugString(tempStr);
	if (!GetRealCoord(camID,srcImage,ofOffset, Threshold1,Threshold2,ofROIposition, ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight, ofDestDataOut, ErrorMsg))
	{	
		tempStr.Format(_T("CamID:%d,计算各ROI实际坐标失败\n"),camID);
		OutputDebugString(tempStr);
		    if (bSaveErrorImage)
			{
				char filename[1024]  = {0};
				CString temppath = _T("");
				CString tempname = 	SpecDlg.GetModulePath()+_T("ofset.ini");;
				WritePrivateProfileString("SFRROIPosition","iErrorTime",itoa((iErrorTime+1),filename,10),tempname);
				temppath += _T("D:\\AutoROI_Error_Image");
				CreateDirectory(temppath,NULL);
				temppath += _T("\\");
				tempname.Format(_T("%d.bmp"),(iErrorTime+1));

				temppath += tempname;
				myVertFlipBuf(bmpBuffer,ofSrcDataIn->ofWidth*3,ofSrcDataIn->ofHeight);
				bSaveBmpFile(temppath,bmpBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
			}

			delete [] bmpBuffer;
			bmpBuffer = NULL;
			return FALSE;
	}
	if (bDebugMode)
	{
		CreateDirectory("D:\\DebugMode",NULL);
		myVertFlipBuf(bmpBuffer,ofSrcDataIn->ofWidth*3,ofSrcDataIn->ofHeight);
		bSaveBmpFile("D:\\DebugMode\\AutoROI.bmp",bmpBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
	}

	delete [] bmpBuffer;
	bmpBuffer = NULL;
	_CrtDumpMemoryLeaks();
	return TRUE;
}

BOOL fnAutoROICenter(OFAutoROIIn *ofSrcDataIn, OFAutoROIOut *ofDestDataOut,char *ErrorMsg)
{
	USHORT camID = ofSrcDataIn->CamID;
	ROIposition ofROIposition[MAX_ROI_NUMBER];    //存储勾选的要测试的ROI测试位置，水平？竖直？
	int  ofROISize = 0;       //需要测试的ROI Size
	int  ofOffset = 0;
	CString tempStr = _T("");
	BYTE *bmpBuffer = new BYTE[ofSrcDataIn->ofWidth*ofSrcDataIn->ofHeight*3];     //转后的Bmp Buffer
	if (bmpBuffer == NULL)
	{
		tempStr.Format(_T("CamID:%d,bmpbuffer new fail......\n"),camID);
		OutputDebugString(tempStr);
		return FALSE;
	}
	memset(bmpBuffer,0,sizeof(BYTE)*ofSrcDataIn->ofWidth*ofSrcDataIn->ofHeight*3);
	float Threshold1 = 0.0;
	float Threshold2 = 0.0;

	//调整传入的图像为BMP格式，以便代码统一处理
	if (ofSrcDataIn->ofImageType == RAW10_PACKED_IMAGE)
	{
		R10toRaw8(ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );
	}
	else if (ofSrcDataIn->ofImageType == RAW10_UNPACKED_IMAGE)
	{
		P10toRaw8(ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );	
	}
	else if (ofSrcDataIn->ofImageType == RAW8_IMAGE)
	{
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );
	}
	else if(ofSrcDataIn->ofImageType == BMP_IMAGE)
	{
		for (int hh = 0; hh < (ofSrcDataIn->ofWidth* ofSrcDataIn->ofHeight * 3); hh++)
		{
			bmpBuffer[hh] = ofSrcDataIn->ofSrcBuffer[hh];
		}
	}
	else
	{
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		strcpy(ErrorMsg,"传入的图片格式错误");
		return FALSE;
	}

	if(ofSrcDataIn->ofOutFormat != OUTFORMAT_FLIP_)
	{
		myVertFlipBuf(bmpBuffer,ofSrcDataIn->ofWidth*3,ofSrcDataIn->ofHeight);
	}

	Mat srcImage(ofSrcDataIn->ofHeight,ofSrcDataIn->ofWidth,CV_8UC3,bmpBuffer);
	if (srcImage.empty())
	{
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		OutputDebugString("srcImage加载图片失败");
		strcpy(ErrorMsg,"srcImage加载图片失败");
		return FALSE;
	}
	if (srcImage.data == NULL)
	{
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		OutputDebugString("srcImage data为空");
		strcpy(ErrorMsg,"srcImage data为空");
		return FALSE;
	}
	//获取要求计算的ROI、以及ROI长宽
	//获取大致的ROI位置坐标
	GetSettingFromFile(camID,ofROIposition,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight,&ofROISize,&ofOffset,&Threshold1,&Threshold2);
	

	ofROIposition[0].Horizontal = 1;
	ofROIposition[0].Vertical = 1;

	for (int ii = 1; ii < MAX_ROI_NUMBER; ii++)
	{
			ofROIposition[ii].Horizontal = 0;
			ofROIposition[ii].Vertical = 0;
	}

	// 	计算各个ROI的实际坐标
	ofDestDataOut->ofROISize  = ofROISize;
	if (!GetRealCoord(camID,srcImage,ofOffset,Threshold1,Threshold2,ofROIposition, ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight, ofDestDataOut, ErrorMsg))
	{
		if (bSaveErrorImage)
		{
			char filename[1024]  = {0};
			CString temppath = _T("");
			CString tempname = 	SpecDlg.GetModulePath()+_T("ofset.ini");
			WritePrivateProfileString("SFRROIPosition","iErrorTime",itoa((iErrorTime+1),filename,10),tempname);
			temppath += _T("D:\\AutoROI_Error_Image");
			CreateDirectory(temppath,NULL);
			temppath += _T("\\");
			tempname.Format(_T("%d.bmp"),(iErrorTime+1));
	
			temppath += tempname;
			myVertFlipBuf(bmpBuffer,ofSrcDataIn->ofWidth*3,ofSrcDataIn->ofHeight);
			bSaveBmpFile(temppath,bmpBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
		}
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		return FALSE;
	}

	delete [] bmpBuffer;
	bmpBuffer = NULL;
	return TRUE;
}

BOOL fnGet4Point(OFAutoROIIn *ofSrcDataIn, OFAuto4PointOut *of4PointDataOut,char *ErrorMsg)
{
	float circleS = 0.0;
	float circleRMax = 0.0;
	float circleRMin  = 0.0;

	BYTE *bmpBuffer = new BYTE[ofSrcDataIn->ofWidth*ofSrcDataIn->ofHeight*3];     //转后的Bmp Buffer
	if (bmpBuffer == NULL)
	{
		OutputDebugString(_T("fnGet4Point new buffer fail"));
		return FALSE;
	}
	memset(bmpBuffer,0,sizeof(BYTE)*ofSrcDataIn->ofWidth*ofSrcDataIn->ofHeight*3);
	GetSettingFromFile(0,&circleS, &circleRMax,&circleRMin);

	//调整传入的图像为BMP格式，以便代码统一处理
	if (ofSrcDataIn->ofImageType == RAW10_PACKED_IMAGE)
	{
		R10toRaw8(ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );
	}
	else if (ofSrcDataIn->ofImageType == RAW10_UNPACKED_IMAGE)
	{
		P10toRaw8(ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofSrcBuffer,ofSrcDataIn->ofWidth,ofSrcDataIn->ofHeight);
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );	
	}
	else if (ofSrcDataIn->ofImageType == RAW8_IMAGE)
	{
		BayerConversionFlip(ofSrcDataIn->ofSrcBuffer, bmpBuffer, ofSrcDataIn->ofWidth, ofSrcDataIn->ofHeight, ofSrcDataIn->ofOutFormat );
	}
	else if(ofSrcDataIn->ofImageType == BMP_IMAGE)
	{
		for (int hh = 0; hh < (ofSrcDataIn->ofWidth* ofSrcDataIn->ofHeight * 3); hh++)
		{
			bmpBuffer[hh] = ofSrcDataIn->ofSrcBuffer[hh];
		}
	}
	else
	{
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		strcpy(ErrorMsg,"传入的图片格式错误");
		return FALSE;
	}

	if(ofSrcDataIn->ofOutFormat != OUTFORMAT_FLIP_)
	{
		myVertFlipBuf(bmpBuffer,ofSrcDataIn->ofWidth*3,ofSrcDataIn->ofHeight);
	}
	Mat srcImage(ofSrcDataIn->ofHeight,ofSrcDataIn->ofWidth,CV_8UC3,bmpBuffer);

	Rect roi(0.4*ofSrcDataIn->ofWidth, 0.375*ofSrcDataIn->ofHeight, 0.2*ofSrcDataIn->ofWidth, 0.25*ofSrcDataIn->ofHeight);
	Mat roi_of_image = srcImage(roi);
	Mat midImage;

	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));    		//进行形态学操作
	morphologyEx(roi_of_image, roi_of_image, MORPH_OPEN, element);
	morphologyEx(roi_of_image, roi_of_image, MORPH_CLOSE, element);

	cvtColor(roi_of_image,midImage, CV_RGB2GRAY);          //转化边缘检测后的图为灰度图
	CvScalar scalar = cvAvg(&midImage.operator IplImage());  
	GaussianBlur( midImage, midImage, Size(9, 9), 2, 2 );

	//进行霍夫圆变换
	//两圆之间的最小距离、用于Canny的边缘阀值上限、累加器的阀值、最小圆半径、最大圆半径
	//用于Canny的边缘阀值上限
	//累加器的阀值---一个圆上像素要超过这个值，才算符合要求
	vector<Vec3f> circles;

	HoughCircles( midImage, circles, CV_HOUGH_GRADIENT,1, ofSrcDataIn->ofWidth/15.0, scalar.val[0] * 0.5,  (0.002*ofSrcDataIn->ofWidth*circleS),(0.005*ofSrcDataIn->ofWidth*circleRMin),( 0.015*ofSrcDataIn->ofWidth*circleRMax) );
	if (circles.size() < 4)
	{
			strcpy(ErrorMsg,"实际找到的圆个数小于4个，请调整参数，或检查环境");
			delete [] bmpBuffer;
			bmpBuffer = NULL;
			return FALSE;
	}
	else if(circles.size() > 4)
	{
		strcpy(ErrorMsg,"实际找到的圆个数大于4个，请调整参数，或检查环境");
		delete [] bmpBuffer;
		bmpBuffer = NULL;
		return FALSE;
	}

	for( size_t i = 0; i < circles.size(); i++ )
	{
		of4PointDataOut->Circle[i].x = cvRound(circles[i][0] + 0.4*ofSrcDataIn->ofWidth);
		of4PointDataOut->Circle[i].y = cvRound(circles[i][1] + 0.375*ofSrcDataIn->ofHeight);
	}

	Point centerc;
	of4PointDataOut->Center.x  = cvRound((circles[0][0] + circles[1][0] + circles[2][0] + circles[3][0])/4.0);
	of4PointDataOut->Center.y = cvRound((circles[0][1] + circles[1][1] + circles[2][1] + circles[3][1])/4.0);

	delete [] bmpBuffer;
	bmpBuffer = NULL;
	return TRUE;
}
