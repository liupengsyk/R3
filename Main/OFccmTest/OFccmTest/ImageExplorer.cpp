#include "StdAfx.h"
#include "ImageExplorer.h"
#include "math.h"
#include "ImageProc.h"
#include "OFccmControl.h"

ImageExplorer::ImageExplorer(HWND ScrWnd,  OFccmControl* Tccm,int CamID)
{
	
	hScrWnd      = ScrWnd;	 
	pTccm        = Tccm;
	RGB24_buffer = NULL; 
	raw_buffer = NULL;
	this->CamID = CamID;
    Init();	
	ZoomRatio = 0; //缩放倍率为0，代表FIT窗口尺寸
}


ImageExplorer::~ImageExplorer(void)
{
     Free();
	 pTccm = NULL;
}


BOOL ImageExplorer::CreateImage(BYTE* RGB24,int width, int height)
{
	if (RGB24 == NULL) return FALSE;

	//如果图像存在，删除
	if(!ScrImage.IsNull())		   
		ScrImage.Destroy();

    //建立源图
    ScrImage.Create(width,height,24);
	int pitch  = ScrImage.GetPitch();
	BYTE* pSrc = (BYTE*)ScrImage.GetBits();

	//数据区的复制
	int k = 0;
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j< width; j++)
		{
			pSrc[i*pitch + j*3]     = RGB24[k];
			pSrc[i*pitch + j*3 + 1] = RGB24[k+1];
			pSrc[i*pitch + j*3 + 2] = RGB24[k+2];
			k += 3;
		}
	}
	pSrc = NULL;
	//CreateNaviImage(&ScrImage);
	return TRUE;
}

 

void ImageExplorer::Init(void)
{
	NaviStatus.Moveable = FALSE;
	NaviStatus.LBDown  = FALSE;

	ScrStatus.LBDown = FALSE;
	ScrImageStartPoint   = CPoint(0,0);
	dStartPoint_x = ScrImageStartPoint.x;
	dStartPoint_y = ScrImageStartPoint.y;;
	ZoomRatio = 0;
	SAFE_DELETE_ARRAY(raw_buffer);
	SAFE_DELETE_ARRAY(RGB24_buffer);
}
CString ImageExplorer::GetImgPath()
{
	CString imgPath =  OFccmControl::globalset.GetOffLineTestPath(CamID);//+_T("\");	 
	CFileFind fileFinder;
	CString FindPath = imgPath + _T("\\*.*");
	BOOL bFind = fileFinder.FindFile(FindPath);
	while (bFind)
	{		
		bFind = fileFinder.FindNextFile();
		//查找指定文件 
		if(fileFinder.IsDirectory()) continue;  //如果是目录，跳过去
		if(fileFinder.IsDots()) continue;       //如果是点文件，跳过去
		
		CString pathFine = fileFinder.GetFilePath();
		ImgName = fileFinder.GetFileName();
		
		fileFinder.Close();
		return pathFine;
	}
	return _T("0");
}

CString ImageExplorer::GetImgName()
{
	return ImgName;
}
BOOL ImageExplorer::Open(BOOL bRaw)
{
	CString srcImgPath ;
	CFileDialog dlg(true);
	if (OFccmControl::globalset.OffLineTest)
	{
		
		 srcImgPath = GetImgPath();	
		 if (srcImgPath ==_T("0"))
		 {
			 pTccm->Ctrl_Addlog(CamID,_T("测试完毕！"),COLOR_GREEN);
			 return FALSE;
		 }

		 CString str=srcImgPath.Right(3); //取得文件后缀
		 //文件后缀为RAW
		 if ( str == _T("raw") )     
		 {
			 bRaw = OFccmControl::globalset.bRawImg;
		 }
		 else
		 {
			 pTccm->Ctrl_Addlog(CamID,_T("文件格式错误！"),COLOR_RED);
			 return FALSE;
		 }
	}
	else
	{
		//通过对话框加载图像文件

		dlg.m_ofn.lpstrFilter=_T("JPEG files(*.jpg) and BMP files Raw files(*.bmp)\0*.jpg;*.bmp;*.raw\0\0");

		if(dlg.DoModal()!=IDOK) return FALSE;
		 srcImgPath = dlg.GetPathName();
		  
		 ImgName = dlg.GetFileName();
		 CString str=dlg.GetFileExt(); //取得文件后缀
		 if(str == _T("raw"))
			 bRaw = TRUE;


	}
	
	//加载新的图片之前初始化    
	if(!ScrImage.IsNull())		    ScrImage.Destroy();
	 

	//初始化
	Init();
	
	
   //文件后缀为RAW
	if ( bRaw )     
	{
		//根据RAW数据创建图像

		if( !CreateRawImage(srcImgPath,OFccmControl::globalset.OffLineTest) ) 
			return FALSE;

		pTccm->Ctrl_Addlog(CamID,_T("图片路径  ")+srcImgPath);
	}
	else
	{
		//加载BMP,JPEG等图像
		HRESULT h1= ScrImage.Load (srcImgPath);
		if(FAILED(h1))
		{
			AfxMessageBox(_T("打开失败"));
			return FALSE;
		}	
		//只支持宽度为4的
		if (ScrImage.GetPitch()%ScrImage.GetWidth() != 0 )
		{
			AfxMessageBox(_T("图像格式非规则，不能被4整除"));
			return FALSE;
		}
		//创建导航图
		//CreateNaviImage(&ScrImage);

		pTccm->Ctrl_Addlog(CamID,_T("打开图片")+srcImgPath);
	}

    ShowScrImage();
	ShowScrImageInfo();

	CreateBuffer();
	Sleep(100);
	if (OFccmControl::globalset.OffLineTest) DeleteFile(srcImgPath);
	return TRUE;
}

BOOL ImageExplorer::Open(CString srcImgPath)
{ 
	//加载新的图片之前初始化    
	if(!ScrImage.IsNull())		    ScrImage.Destroy();
	 

	//初始化
	Init();
	CString str=srcImgPath.Right(3); //取得文件后缀
	//文件后缀为RAW
	if ( str == _T("raw") )     
	{
		//根据RAW数据创建图像

		if( !CreateRawImage(srcImgPath) ) 
			return FALSE;

		pTccm->Ctrl_Addlog(CamID,_T("图片路径  ")+srcImgPath);
	}
	else	 
	{
		//加载BMP,JPEG等图像
		HRESULT h1= ScrImage.Load (srcImgPath);
		if(FAILED(h1))
		{
			AfxMessageBox(_T("打开失败"));
			return FALSE;
		}	
		//只支持宽度为4的
		if (ScrImage.GetPitch()%ScrImage.GetWidth() != 0 )
		{
			AfxMessageBox(_T("图像格式非规则，不能被4整除"));
			return FALSE;
		}
		//创建导航图
		 
		pTccm->Ctrl_Addlog(CamID,_T("打开图片")+srcImgPath);
		 
	}

	ShowScrImage();
	ShowScrImageInfo();

	CreateBuffer();
	Sleep(100); 
	return TRUE;
}


BOOL ImageExplorer::Open(ImageInfo* CurImage)
{
	Init(); 
	int len = CurImage->imgwidth * CurImage->imgheight;
  
	//更新部分raw选项的数据
	sRawInfo.bits      = CurImage->RawBits;
	sRawInfo.outformat = CurImage->RawFormat;

	//得到RGB24buffer;
	SAFE_DELETE_ARRAY(RGB24_buffer);
	RGB24_buffer = new BYTE[len*3];
	memcpy(RGB24_buffer, CurImage->RGB24_buffer, len*3);
	
	CreateImage(CurImage->RGB24_buffer, CurImage->imgwidth, CurImage->imgheight);
	 
	return TRUE;
}


BOOL ImageExplorer::CreateRawImage(LPCTSTR PathName,BOOL OffLineTest)
{
	CFile file;
	int type = 0;
	if(!file.Open(PathName, CFile::modeRead|CFile::typeBinary))
		return FALSE;
	if(!OffLineTest)
	{
		m_RawOptionDlg.m_FileLength = file.GetLength();

		if(m_RawOptionDlg.DoModal()!=IDOK) return FALSE;

		sRawInfo.width     = m_RawOptionDlg.m_Raw_Width;
		sRawInfo.height    = m_RawOptionDlg.m_Raw_Height;
		sRawInfo.bits      = m_RawOptionDlg.RawBits;
		sRawInfo.outformat = m_RawOptionDlg.RawOutForamt;
		type = m_RawOptionDlg.m_ShowType; //显示各个点
	}
	else
	{
		sRawInfo.width     = OFccmControl::globalset.m_Raw_Width;
		sRawInfo.height    = OFccmControl::globalset.m_Raw_Height;
		sRawInfo.bits      = OFccmControl::globalset.RawBits;
		sRawInfo.outformat = OFccmControl::globalset.RawOutForamt;
		type = OFccmControl::globalset.m_RawShowType; //显示各个点
	}

     

	int len = (int)file.GetLength()/2; //BYTE*2 = USHORT
	raw_buffer  = new USHORT[len];
	//将RAW数据读到raw_buffer;
	file.Read((void*)raw_buffer,len*2);

	SAFE_DELETE_ARRAY(RGB24_buffer);	
	
	//根据RAW插值成RGB24;
	 
	if(sRawInfo.bits!=8)
	{
		RGB24_buffer = new BYTE[len*3];
		switch (type)
		{
		case 1:
			CreateRawGrayPixel(RGB24_buffer, raw_buffer, sRawInfo.width, sRawInfo.height, sRawInfo.bits);
			break;
		case 2:
			CreateRawColorPixel(RGB24_buffer, raw_buffer, sRawInfo.width, sRawInfo.height, sRawInfo.bits, sRawInfo.outformat);
			break;
		default:
			ImageProc::RawtoRGB24_DT(raw_buffer,RGB24_buffer,sRawInfo.width,sRawInfo.height, sRawInfo.outformat, sRawInfo.bits);
			//ImageProc::RawtoRGB24MotoDemosaic(raw_buffer,RGB24_buffer,sRawInfo.width,sRawInfo.height, sRawInfo.outformat, sRawInfo.bits);
			break;
		}
	}
	else if(sRawInfo.bits==8)
	{
		BYTE* raw_buffer_byte  = new BYTE[len*2];
		for(int i=0;i<len*2;i+=2)
		{
			raw_buffer_byte[i+1] = raw_buffer[i/2]>>8;
			raw_buffer_byte[i] = raw_buffer[i/2]&0xff;
		}
		RGB24_buffer = new BYTE[len*2*3];
		switch (type)
		{
		case 1:
			CreateRawGrayPixel(RGB24_buffer, raw_buffer_byte, sRawInfo.width, sRawInfo.height);
			break;
		case 2:
			CreateRawColorPixel(RGB24_buffer, raw_buffer_byte, sRawInfo.width, sRawInfo.height, sRawInfo.outformat);
			break;
		default:
			ImageProc::RawtoRGB24_DT(raw_buffer_byte,RGB24_buffer,sRawInfo.width,sRawInfo.height, sRawInfo.outformat); 
			break;
		}
		SAFE_DELETE_ARRAY(raw_buffer_byte);
	}

	//根据BUFFER创建源图
	CreateImage(RGB24_buffer,sRawInfo.width,sRawInfo.height);
	//前面已经有了RAW和RGB24
	//SAFE_DELETE_ARRAY(raw_buffer);
	return TRUE;
}
 

BOOL ImageExplorer::ShowScrImage()
{
	
	if(ScrImage.IsNull()) return FALSE;

    //得到窗口尺寸
	CRect rect;
	::GetClientRect(hScrWnd,&rect);
	
    //计算显示区域
	CDC* pDC = pTccm->Ctrl_GetDisplayWndMemDC(CamID);

 	SetStretchBltMode(pDC->m_hDC, HALFTONE);
 	SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);

		//边界
	int a = int( (ScrImage.GetHeight()*ZoomRatio - rect.Height() )/ ZoomRatio);
	int b = int( (ScrImage.GetWidth()*ZoomRatio  - rect.Width() )/ ZoomRatio);

		//判断图像起点否超出边界
	if(ScrImageStartPoint.y>=a)
		ScrImageStartPoint.y=a;
	if(ScrImageStartPoint.x>=b)
		ScrImageStartPoint.x=b;

	if(ScrImageStartPoint.y<=0)
		ScrImageStartPoint.y=0;
	if(ScrImageStartPoint.x<=0)
		ScrImageStartPoint.x=0;

	if ( (a<=0 && b<=0) || (ZoomRatio == 0))
	{
		ScrImage.Draw(pDC->m_hDC,rect);
		ZoomRatio = (float) rect.Width() / ScrImage.GetWidth();
	} 
	else
	{
		int width  = int (rect.Width()/ZoomRatio);
		int height = int (rect.Height()/ZoomRatio) ;

		CRect display_rect(ScrImageStartPoint,CSize(width,height));
		ScrImage.StretchBlt(pDC->m_hDC,rect,display_rect);
		//释放DC		
	}
	pTccm->Ctrl_ShowMemDC(CamID);

	pDC = NULL;
	//ShowNaviIamge();
	return TRUE;

}
   
void ImageExplorer::OnMouseWheel(short zDelta, CPoint Point)
{
	//记录缩放前的鼠标点
	if(ScrImage.IsNull()) return ;

	CRect rect;
	::GetWindowRect(hScrWnd,&rect);
	Point.x = Point.x - rect.left;
	Point.y = Point.y - rect.top;

	if (ZoomRatio == 0)
	{
		ZoomRatio = 0.125;
	}

	//计算当前倍率，归2
	for( float i=-3; i<5; i++)
	{
		if(ZoomRatio > pow(2,i) && ZoomRatio < pow(2,i+1))
			ZoomRatio = pow(2,i);
	} 

	float ImagePoint_x = ScrImageStartPoint.x + Point.x / ZoomRatio  ;
	float ImagePoint_y = ScrImageStartPoint.y + Point.y / ZoomRatio  ;

	if (zDelta < 0)
	{
		ZoomRatio = ZoomRatio/2;
	}
	else
	{
		ZoomRatio = ZoomRatio*2;
	}

	//计算缩放倍率上下限
	if (ZoomRatio>32)
	{
		ZoomRatio =32;
	}
	else if (ZoomRatio < 0.125)
	{
		ZoomRatio = 0.125;
	}
	//计算起点
    ScrImageStartPoint.x = int ((ImagePoint_x*ZoomRatio - Point.x)/ZoomRatio);
	ScrImageStartPoint.y = int ((ImagePoint_y*ZoomRatio - Point.y)/ZoomRatio);


	ShowScrImage();

	ShowPointInfo(Point);
}

void ImageExplorer::OnLButtonDown(CPoint Point)
{
	ScrStatus.LBDown = TRUE;
	ScrStatus.PrevPoint = Point;
}

void ImageExplorer::OnLButtonUp(CPoint Point)
{
	ScrStatus.LBDown = FALSE;
	ReleaseCapture();
}

void ImageExplorer::OnMouseMove(CPoint Point)
{
	if(ScrImage.IsNull()) return ;

	ScrStatus.CurPoint = Point;
	ShowPointInfo(Point);

	//响应鼠标拖动
	if (!ScrStatus.LBDown)
		return;

	SetCapture(hScrWnd);
	//得到VIEW的客户区大小
	CRect rect;
	GetClientRect(hScrWnd,&rect);

	CPoint MouseMoveDistance;
	MouseMoveDistance = ScrStatus.PrevPoint - ScrStatus.CurPoint;//计算鼠标移动距离

	//计算相对移动距离
	double x = MouseMoveDistance.x/ZoomRatio;
	double y = MouseMoveDistance.y/ZoomRatio;

	//浮点累加，满足1个pixel归0
	dStartPoint_x += x;
	dStartPoint_y += y;

	if ( abs(dStartPoint_x) >= 1 || abs(dStartPoint_y) >= 1)
	{
		//计算图像显示的新起点 
		ScrImageStartPoint.x = ScrImageStartPoint.x + int(dStartPoint_x);
		ScrImageStartPoint.y = ScrImageStartPoint.y + int(dStartPoint_y);
	    ShowScrImage();
		dStartPoint_x = 0;
		dStartPoint_y = 0;
	}
	//刷新
	ScrStatus.PrevPoint = Point;

}

void ImageExplorer::ShowPointInfo(CPoint point)
{

	HDC hdc = ::GetDC(hScrWnd);
	float x = ScrImageStartPoint.x + point.x / ZoomRatio  ;
	float y = ScrImageStartPoint.y + point.y / ZoomRatio  ;

	COLORREF clr = ::GetPixel(hdc,point.x,point.y);
	BYTE r = GetRValue(clr);
	BYTE g = GetGValue(clr);
	BYTE b = GetBValue(clr);
	 
}



void ImageExplorer::ShowScrImageInfo(void)
{
	if (ScrImage.IsNull()) return;

	CString temp;
	temp.Format(_T("图长 = %d   图宽 = %d"),ScrImage.GetWidth(),ScrImage.GetHeight());
	pTccm->Ctrl_Addlog(CamID,temp);

}


void ImageExplorer::Free(void)
{
	if(!ScrImage.IsNull())		    
		ScrImage.Destroy();
 
	SAFE_DELETE_ARRAY(RGB24_buffer); 
	SAFE_DELETE_ARRAY(raw_buffer); 
	  
}

void ImageExplorer::CreateRaw_buffer(void)
{
	if (raw_buffer==NULL)
	{ 
		int pitch  = ScrImage.GetPitch();   
		int height = ScrImage.GetHeight();
		int width  = ScrImage.GetWidth();
		raw_buffer = new USHORT[height*width];
		ImageProc::RGB24toRaw_BGGR(RGB24_buffer,raw_buffer,width,height,8);
	}
}
 
void ImageExplorer::CreateRGB24buffer(void)
{
	if (ScrImage.IsNull()) return ;

	 
	int pitch  = ScrImage.GetPitch();   
	int height = ScrImage.GetHeight();
	int width  = ScrImage.GetWidth();
	
	RGB24_buffer = new BYTE[width*height*3];
	BYTE* pSrc = (BYTE*)ScrImage.GetBits();
	int k = 0;
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j< width; j++)
		{
			RGB24_buffer[k]   = pSrc[i*pitch + j*3]     ;
			RGB24_buffer[k+1] = pSrc[i*pitch + j*3 + 1] ;
			RGB24_buffer[k+2] = pSrc[i*pitch + j*3 + 2] ;
			k += 3;
		}
	}
	pSrc = NULL;
}

 
 
 

void ImageExplorer::CreateBuffer()
{
	SAFE_DELETE_ARRAY(RGB24_buffer);

	CreateRGB24buffer();
	
	 CreateRaw_buffer(); 
}


BYTE* ImageExplorer::GetRGB24Buffer()
{
	return RGB24_buffer;
}

USHORT* ImageExplorer::GetRawBuffer()
{
	return raw_buffer;
}
 

int ImageExplorer::GetImageWidth()
{
	return  ScrImage.GetWidth();
}

int ImageExplorer::GetImageHeight()
{
	return  ScrImage.GetHeight();
}

int ImageExplorer::GetRawFormat()
{
	return sRawInfo.outformat;
}

BYTE ImageExplorer::GetRawBits(void)
{
	return sRawInfo.bits;
}
 
void ImageExplorer::CreateRawGrayPixel(BYTE* RGB24, USHORT* Raw, int width, int height, int Bits)
{
	int j = 0;
	int bits = Bits - 8;
	for (int i=0; i<width*height; i++)
	{
		BYTE temp = Raw[i]>>bits;
		RGB24[j] = temp;
		RGB24[j+1] = temp;
		RGB24[j+2] = temp;
		j += 3;
	}
 
}

void ImageExplorer::CreateRawGrayPixel(BYTE* RGB24, BYTE* Raw, int width, int height)
{
	int j = 0;
	int bits = 0;
	for (int i=0; i<width*height; i++)
	{
		BYTE temp = Raw[i]>>bits;
		RGB24[j] = temp;
		RGB24[j+1] = temp;
		RGB24[j+2] = temp;
		j += 3;
	}
 
}


void ImageExplorer::CreateRawColorPixel(BYTE* RGB24, BYTE* Raw, int width, int height, int outformat )
{
	int j = 0;
	int bits = 0;
	for (int i=0; i<height; i+=2)
	{
		for (int j=0; j<width; j+=2)
		{
			BYTE temp1 = Raw[  i*  width + j  ]>>bits;	
			BYTE temp2 = Raw[  i*  width + j+1]>>bits;	
			BYTE temp3 = Raw[(i+1)*width + j  ]>>bits;	
			BYTE temp4 = Raw[(i+1)*width + j+1]>>bits;	

			switch(outformat)
			{
			case IMG_OUTFORMAT_RGGB:  //BGR
				RGB24[(i*width + j)*3 + 0] = 0;
				RGB24[(i*width + j)*3 + 1] = 0;
				RGB24[(i*width + j)*3 + 2] = temp1;

				RGB24[(i*width + j+1)*3 + 0] = 0;
				RGB24[(i*width + j+1)*3 + 1] = temp2;
				RGB24[(i*width + j+1)*3 + 2] = 0;

				RGB24[((i+1)*width + j)*3 + 0] = 0;
				RGB24[((i+1)*width + j)*3 + 1] = temp3;
				RGB24[((i+1)*width + j)*3 + 2] = 0;

				RGB24[((i+1)*width + j+1)*3 + 0] = temp4;
				RGB24[((i+1)*width + j+1)*3 + 1] = 0;
				RGB24[((i+1)*width + j+1)*3 + 2] = 0;
				break;

			case IMG_OUTFORMAT_BGGR:
				RGB24[(i*width + j)*3 + 0] = temp1;
				RGB24[(i*width + j)*3 + 1] = 0;
				RGB24[(i*width + j)*3 + 2] = 0;

				RGB24[(i*width + j+1)*3 + 0] = 0;
				RGB24[(i*width + j+1)*3 + 1] = temp2;
				RGB24[(i*width + j+1)*3 + 2] = 0;

				RGB24[((i+1)*width + j)*3 + 0] = 0;
				RGB24[((i+1)*width + j)*3 + 1] = temp3;
				RGB24[((i+1)*width + j)*3 + 2] = 0;

				RGB24[((i+1)*width + j+1)*3 + 0] = 0;
				RGB24[((i+1)*width + j+1)*3 + 1] = 0;
				RGB24[((i+1)*width + j+1)*3 + 2] = temp4;
				break;

			case IMG_OUTFORMAT_GBRG:
				RGB24[(i*width + j)*3 + 0] = 0;
				RGB24[(i*width + j)*3 + 1] = temp1;
				RGB24[(i*width + j)*3 + 2] = 0;

				RGB24[(i*width + j+1)*3 + 0] = temp2;
				RGB24[(i*width + j+1)*3 + 1] = 0;
				RGB24[(i*width + j+1)*3 + 2] = 0;

				RGB24[((i+1)*width + j)*3 + 0] = 0;
				RGB24[((i+1)*width + j)*3 + 1] = 0;
				RGB24[((i+1)*width + j)*3 + 2] = temp3;

				RGB24[((i+1)*width + j+1)*3 + 0] = 0;
				RGB24[((i+1)*width + j+1)*3 + 1] = temp4;
				RGB24[((i+1)*width + j+1)*3 + 2] = 0;
				break;

			case IMG_OUTFORMAT_GRBG:
				RGB24[(i*width + j)*3 + 0] = 0;
				RGB24[(i*width + j)*3 + 1] = temp1;
				RGB24[(i*width + j)*3 + 2] = 0;

				RGB24[(i*width + j+1)*3 + 0] = 0;
				RGB24[(i*width + j+1)*3 + 1] = 0;
				RGB24[(i*width + j+1)*3 + 2] = temp2;

				RGB24[((i+1)*width + j)*3 + 0] = temp3;
				RGB24[((i+1)*width + j)*3 + 1] = 0;
				RGB24[((i+1)*width + j)*3 + 2] = 0;

				RGB24[((i+1)*width + j+1)*3 + 0] = 0;
				RGB24[((i+1)*width + j+1)*3 + 1] = temp4;
				RGB24[((i+1)*width + j+1)*3 + 2] = 0;
				break;
			}
		}
	}
}

void ImageExplorer::CreateRawColorPixel(BYTE* RGB24, USHORT* Raw, int width, int height,int Bits, int outformat )
{
	int j = 0;
	int bits = Bits - 8;
	for (int i=0; i<height; i+=2)
	{
		for (int j=0; j<width; j+=2)
		{
			BYTE temp1 = Raw[  i*  width + j  ]>>bits;	
			BYTE temp2 = Raw[  i*  width + j+1]>>bits;	
			BYTE temp3 = Raw[(i+1)*width + j  ]>>bits;	
			BYTE temp4 = Raw[(i+1)*width + j+1]>>bits;	

			switch(outformat)
			{
			case IMG_OUTFORMAT_RGGB:  //BGR
				RGB24[(i*width + j)*3 + 0] = 0;
				RGB24[(i*width + j)*3 + 1] = 0;
				RGB24[(i*width + j)*3 + 2] = temp1;

				RGB24[(i*width + j+1)*3 + 0] = 0;
				RGB24[(i*width + j+1)*3 + 1] = temp2;
				RGB24[(i*width + j+1)*3 + 2] = 0;

				RGB24[((i+1)*width + j)*3 + 0] = 0;
				RGB24[((i+1)*width + j)*3 + 1] = temp3;
				RGB24[((i+1)*width + j)*3 + 2] = 0;

				RGB24[((i+1)*width + j+1)*3 + 0] = temp4;
				RGB24[((i+1)*width + j+1)*3 + 1] = 0;
				RGB24[((i+1)*width + j+1)*3 + 2] = 0;
				break;

			case IMG_OUTFORMAT_BGGR:
				RGB24[(i*width + j)*3 + 0] = temp1;
				RGB24[(i*width + j)*3 + 1] = 0;
				RGB24[(i*width + j)*3 + 2] = 0;

				RGB24[(i*width + j+1)*3 + 0] = 0;
				RGB24[(i*width + j+1)*3 + 1] = temp2;
				RGB24[(i*width + j+1)*3 + 2] = 0;

				RGB24[((i+1)*width + j)*3 + 0] = 0;
				RGB24[((i+1)*width + j)*3 + 1] = temp3;
				RGB24[((i+1)*width + j)*3 + 2] = 0;

				RGB24[((i+1)*width + j+1)*3 + 0] = 0;
				RGB24[((i+1)*width + j+1)*3 + 1] = 0;
				RGB24[((i+1)*width + j+1)*3 + 2] = temp4;
				break;

			case IMG_OUTFORMAT_GBRG:
				RGB24[(i*width + j)*3 + 0] = 0;
				RGB24[(i*width + j)*3 + 1] = temp1;
				RGB24[(i*width + j)*3 + 2] = 0;

				RGB24[(i*width + j+1)*3 + 0] = temp2;
				RGB24[(i*width + j+1)*3 + 1] = 0;
				RGB24[(i*width + j+1)*3 + 2] = 0;

				RGB24[((i+1)*width + j)*3 + 0] = 0;
				RGB24[((i+1)*width + j)*3 + 1] = 0;
				RGB24[((i+1)*width + j)*3 + 2] = temp3;

				RGB24[((i+1)*width + j+1)*3 + 0] = 0;
				RGB24[((i+1)*width + j+1)*3 + 1] = temp4;
				RGB24[((i+1)*width + j+1)*3 + 2] = 0;
				break;

			case IMG_OUTFORMAT_GRBG:
				RGB24[(i*width + j)*3 + 0] = 0;
				RGB24[(i*width + j)*3 + 1] = temp1;
				RGB24[(i*width + j)*3 + 2] = 0;

				RGB24[(i*width + j+1)*3 + 0] = 0;
				RGB24[(i*width + j+1)*3 + 1] = 0;
				RGB24[(i*width + j+1)*3 + 2] = temp2;

				RGB24[((i+1)*width + j)*3 + 0] = temp3;
				RGB24[((i+1)*width + j)*3 + 1] = 0;
				RGB24[((i+1)*width + j)*3 + 2] = 0;

				RGB24[((i+1)*width + j+1)*3 + 0] = 0;
				RGB24[((i+1)*width + j+1)*3 + 1] = temp4;
				RGB24[((i+1)*width + j+1)*3 + 2] = 0;
				break;
			}
		}
	}
}