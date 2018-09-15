#pragma once
#define  PI 3.14159265

#define	IMG_OUTFORMAT_RGGB  0
#define	IMG_OUTFORMAT_GRBG  1
#define	IMG_OUTFORMAT_GBRG  2 
#define	IMG_OUTFORMAT_BGGR  3

#define	IMG_OUTFORMAT_YCbYCr  0
#define	IMG_OUTFORMAT_YCrYCb  1
#define	IMG_OUTFORMAT_CbYCrY  2 
#define	IMG_OUTFORMAT_CrYCbY  3

#define CLIP(X)       (BYTE)(((X) > 255) ? 255 : (((X) < 0) ? 0 : (X)))



/* Standard C includes */
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
 

#include "Lib_Halcon\include\Halcon.h"
#include "Lib_Halcon\include\cpp\HalconCpp.h"
#include "Lib_SFR\Ofilm_GetSFR.h"
 
using namespace Halcon;
using namespace std;

typedef struct _SFR_PARAMETER
{
	int ContourLength;
	int RoiWidth;
	int RoiHeight;
	float RoiPosition; 
	vector<CRect> RoiRect;
	vector<float>RoiSFRValue;
	int Reserve;
} SFR_PARAMETER, *PSFR_PARAMETER;
 
namespace ImageProc
{
	//压缩RGB24图像
	BOOL ZoomOutRGBImage(BYTE *pSrc, int width, int height, BYTE* pDest, int ratio, int flag = 0);

	//压缩Y图像
	BOOL ZoomOutYImage(BYTE *pSrc, int width, int height, BYTE* pDest, int ratio, int flag = 0);

	//度信采集板出的压缩raw格式转化为标准RAW
	BOOL DTRaw10toRaw8(BYTE *pSrc, BYTE* pDest, int width, int height);

	BOOL DTRaw8toRaw(BYTE* pSrc, USHORT* pDest, int width, int height);
	BOOL DTRaw10toRaw(BYTE* pSrc, USHORT* pDest, int width, int height);
	BOOL DTRaw12toRaw(BYTE* pSrc, USHORT* pDest, int width, int height);
	BOOL RawToRaw8(USHORT* pSrc, BYTE* pDest, int width, int height,BYTE bits);
	BOOL RawToRaw8(BYTE* pSrc, BYTE* pDest, int width, int height);

	//得到图像Y亮度图
	BOOL RGB24toY(BYTE *pSrc,BYTE* pDest, int width, int height);
	//基于Y亮度得到某个区域的平均亮度
	float GetAveY_ROI(BYTE *pSrc, int width, int height,CRect rect);

	//获取某个区域的黑块的中心坐标 1表示初始识别，0表示矫正
	//获取指定roi块的中心
	//指定rect roi
	//输入块亮度相对roi平均亮度的值 ratio
	// faratio
	//Type 指定模式 模式1为计算块中心，模式 0 为重新划定Roi
	//返回整型坐标点  更新后的roi outRect
	void GetBlackBlobkCenterY_ROI(BYTE *pSrc, int width, int height,CRect rect,int ratio,float &x,float &y);
	void AjustRoiRect(BYTE *pSrc, int width, int height,int ratio,CRect rect,CRect *outRect);

	//处理二值化后的图像
	//获取指定roi块的中心
	//指定rect roi 
	//Type 指定模式 模式1为计算块中心， 
	// 输出块的坐标,以及块的大小
	void GetBlackBlobkCenterY_BinaryROI(BYTE *pSrc, int width, int height,CRect rect,float& Point_x,float& Point_y,int &PoinSize);

	//计算灰度图像光学中心  ratio 高亮区域百分比%
	CPoint  GetOpticalCenter(BYTE *pSrc, int width, int height,int ratio = 7);
	//得到0-255直方图
	int* GetHistogram(BYTE *pSrc, int width, int height );
	int GetHistogramThreshold(BYTE *pSrc, int width, int height, float ratio);

	int* GetHistogramY_ROI(BYTE *pSrc, int width, int height,CRect rect );
	//***************获取直方图指定比例样本的阈值 ---从最亮开始往下数***************************//
	int  GetHistogramThreshold(BYTE *pSrc, int width, int height, CRect rect,float ratio);
	//***************获取直方图指定比例样本的阈值---从最暗开始往下算***************************//
	int  GetHistogramLowThreshold(BYTE *pSrc, int width, int height, float ratio);

	//MTF 
	double GetMTF( BYTE *pSrc, int width, int height,CRect rect);
	//计算黑白比重的MTF
	double GetMTF_ratio( BYTE *pSrc, int width, int height,CRect rect,int ratio);
	//带迭代算法的MTF
	double GetMTF_Iteration( BYTE *pSrc, int width, int height,CRect rect);
	//FV算法
	double GetResolution_FV(BYTE* pSrc, int width, int height,CRect rect);

	double nGetMTF(BYTE *pSrc, int width, int height,CRect rect);


	/*************************BAYER插值*************************************/

	//Raw8转RGB24  3X3 双线性插值 (bayer8)
	BOOL Raw8toRGB24(BYTE *pSrc, BYTE* pDest, int width, int height,BYTE outformat);
	BOOL Interpolation_BGGR(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL Interpolation_RGGB(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL Interpolation_GRBG(BYTE* pSrc, BYTE* pDest, int width, int height);

	

//度信的Raw转RGB24，add by zl 2013.11.29
//（ 注： 反汇编推演出度信插值公式：//Raw转RGB24  3X3 双线性插值 (bayer16)
	BOOL RawtoRGB24(USHORT *pSrc, BYTE* pDest, int width, int height, BYTE outformat, BYTE Bits);
	BOOL Interpolation_BGGR(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_RGGB(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GRBG(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GBRG(USHORT* pSrc, BYTE *pDest, int width, int height, BYTE Bits);
//      在G通道算RGB时，G分量有做均值处理 G(x,y) != G(x,y) 而是 G(x,y) = (G(x,y) + G(x+1,y-1)/2)，其他同双线性插值
	BOOL RawtoRGB24_DT(USHORT *pSrc, BYTE* pDest, int width, int height, BYTE outformat, BYTE Bits);
	BOOL Interpolation_BGGR_DT(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_RGGB_DT(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GRBG_DT(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GBRG_DT(USHORT* pSrc, BYTE *pDest, int width, int height, BYTE Bits);

	BOOL RawtoRGB24_DT(BYTE *pSrc, BYTE* pDest, int width, int height, BYTE outformat);
	BOOL Interpolation_BGGR_DT(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL Interpolation_RGGB_DT(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL Interpolation_GRBG_DT(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL Interpolation_GBRG_DT(BYTE* pSrc, BYTE *pDest, int width, int height);

	//RGB24转Raw 点对点复原
	BOOL RGB24toRaw(BYTE *pSrc, USHORT* pDest, int width, int height, BYTE outformat, BYTE Bits);
    BOOL RGB24toRaw_RGGB(BYTE* pSrc,USHORT* pDest, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_GBRG(BYTE* pSrc,USHORT* pDest, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_BGGR(BYTE* pSrc,USHORT* pDest, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_GRBG(BYTE* pSrc,USHORT* pDest, int width, int height, BYTE Bits);

	void RGB24toNV21(BYTE* pSrc,BYTE* pDest, int width, int height);

	//二值化图像
	BOOL  Binary(BYTE *pSrc, int width, int height, int Threshold);
	//二值化图像
	BOOL  Binary(int *pSrc, int width, int height, int Threshold);

	//得到某个通道的buffer
	USHORT* GetGr_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetGb_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetR_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetB_Channel(USHORT* pSrc, int width, int height, int outformat);

	//得到某个区域的标准差
	int GetStandardDeviation(BYTE* pSrc, int width, int height,CRect rect);

	//得到平均帧数据
	BOOL AverageFrameData(BYTE* pSrc, int width, int height, int framelevel);
	BOOL AverageFrameData(USHORT* pSrc, int width, int height, int framelevel);
	BOOL AverageFrameData_RGB24(BYTE* pSrc, int width, int height, int framelevel);
	
	//得到某个通道的区域平均值
	float GetAveR_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat);
	float GetAveG_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat);
	float GetAveB_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat);

	//得到RGB某个区域的平均值
	float GetAveY_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	float GetAveR_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	float GetAveG_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	float GetAveB_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	USHORT GetAveRGain_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	USHORT GetAveBGain_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	float GetFloatRGain_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	float GetFloatBGain_ROI_RGB24(BYTE *pSrc, int width, int height, CRect rect);
	void ManuBalance(BYTE *pSrc, int width, int height,float Rgain, float Bgain);

	BOOL YUV422toRGB24(BYTE* pSrc, BYTE* pDest, int width, int height, int outformat);
	BOOL YUV422toRGB24_YCbYCr(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL YUV422toRGB24_YCrYCb(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL YUV422toRGB24_CbYCrY(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL YUV422toRGB24_CrYCbY(BYTE* pSrc, BYTE* pDest, int width, int height);

	BOOL YUV422toY(BYTE* pSrc, BYTE* pDest, int width, int height, int outformat);


	BOOL Sharpen(BYTE* pSrc, BYTE* pDest, int width, int height, int* Template); //定值锐化

	BOOL Smooth(BYTE* pSrc, BYTE* pDest, int width, int height, int* Template);//定值平滑

	BOOL Template3x3(BYTE* pSrc, BYTE* pDest, int width, int height, int* Template);

	//RGB转LAB
	BOOL RGB24ToLab(BYTE *pScr, double *pDest, int width, int height);//采用photoshop的算法
	double GetAveL_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//采用photoshop的算法
	double GetAvea_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//采用photoshop的算法
	double GetAveb_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//采用photoshop的算法
	BOOL RGBToLab_Pixel(BYTE R, BYTE G, BYTE B, double &lab_l, double &lab_a, double &lab_b);//采用photoshop的算法

	//车载tv转 digital 算法 lifangsen 2014.08.01
	////////////ELA去隔行算法///////////////////
	void EdageDirectLineAvg(unsigned char *pDisplayBuffer, int width, int height);

	///////////////场内平均 Bob //////////////////////
	void Bob(unsigned char *pDisplayBuffer, int width, int height);

	/////////////编织法weave////////////
	void weave(unsigned char *pDisplayBuffer, int width, int height);

	////////////动态调变去隔行算法///////////////////
	void DynamicModule(unsigned char *pDisplayBuffer, int width, int height);

	////////////////动态ELA///////////////////////////
	void DynamicELA(unsigned char *pDisplayBuffer, int width, int height);

	////*******************修正沿ELA********************////////
	void ModifiedELA(unsigned char *pDisplayBuffer, int width, int height);

	//***********************边沿自适应***********************//
	void EdageAdption(unsigned char *pDisplayBuffer, int width, int height);
	void NormalizationAndBinaryzation( BYTE* pBmp, CPoint pStart, CSize pROIRect, UINT iImgWidth, UINT iImgHeight );
	
	//***********************获取特征块的中心坐标***********************//
	UINT GetSingleCircleCenter( BYTE* pBmp, CPoint pStart, CSize pROIRect, UINT iImgWidth, UINT iImgHeight, float& centerX, float& centerY );

	//****************Y增强***************//
	BYTE ImageEnhanceY(BYTE *_srcImage,BYTE *DestImage, UINT Width,UINT  Height);
	//****************Y 局部增强***************//
	BYTE ImageEnhanceY(BYTE *_srcImage,BYTE *DestImage, CRect rect,UINT Width,UINT  Height);

	//****************Y 局部增强***************//
	BYTE ImageEnhanceY(int *_srcImage,BYTE *DestImage, CRect rect,UINT Width,UINT  Height);
	 

	//****************rgb增强***************//
	BYTE ImageEnhanceRGB(BYTE *_srcImage,BYTE *EnhanceBMPBuffer, int width, int height);

	//****************图像压缩***************//
	BYTE ImageShrink(BYTE *_SrcImage, BYTE *_DestImage, int srcwidth, int srcheight,int DestWidth,float DestHeight);
	 

	enum math
	{
		math_Add,
		math_Sub,
		math_Mult,
		math_Div 
	};

	//_Operater
	//enum math
	//{
	//	math_Add,
	//	math_Sub,
	//	math_Mult,
	//	math_Div 
	//};
	//图像运算 +-*/	
	BOOL ImageMath(BYTE *_SrcImage1, BYTE *_SrcImage2, BYTE *DestImage, int srcwidth, int srcheight, int _Operater);


	//_Operater
	//enum math
	//{
	//	math_Add,
	//	math_Sub,
	//	math_Mult,
	//	math_Div 
	//};
	//图像运算 +-*/	
	BOOL ImageMath(BYTE *_SrcImage1, BYTE *_SrcImage2, int *DestImage, int srcwidth, int srcheight, int _Operater);

	//_Operater
	//enum math
	//{
	//	math_Add,
	//	math_Sub,
	//	math_Mult,
	//	math_Div 
	//};
	//图像运算 +-*/	
	BYTE  PixelMath(BYTE *_SrcImage, double MeanValue, BYTE *_DestImage,int srcwidth, int srcheight, int _Operater);
 
 
  
	void RowDif( BYTE* SrcBuf, BYTE *DstBuf, int ImgWidth, int ImgHeigh,CRect rect, BYTE Threshold );

	void  LineDif( BYTE* SrcBuf, BYTE *DstBuf, int ImgWidth, int ImgHeigh,CRect rect, BYTE Threshold );

	//SrcBuf1是合并后的图片
	void MergeImage( BYTE* SrcBuf1, BYTE* SrcBuf2, int ImgWidth, int ImgHeight,CRect rect );

	 
	BOOL VertFlipBuf(BYTE  * inbuf,  UINT ImgSizeBytes,UINT height);  

	void GetSfrRoi_byHalcon(BYTE* RGB_buf,int ImgWidth,int ImgHeight,PSFR_PARAMETER pSfrPara);

	int  GetSFR_ofilm( USHORT *_imageBuffer, int _height, int _width,CRect RoiRect,float freqInput ,float& sfrOut,_sfrresult *sfrRet );
	int  GetSFR_ofilm( BYTE *_imageBuffer, int _height, int _width,CRect RoiRect,float freqInput ,float& sfrOut,_sfrresult *sfrRet );
	void MotoDemosaic( BYTE* pSrcBuffer, BYTE* pDstBuffer, int nImgWidth, int nImgHeight, int bayerType  );

	//Raw转RGB24  3X3 双线性插值 (bayer16)
	BOOL RawtoRGB24MotoDemosaic(USHORT *pSrc, BYTE* pDest, int width, int height, BYTE outformat, BYTE Bits);
	BOOL Interpolation_BGGR_MotoDemosaic(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_RGGB_MotoDemosaic(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GRBG_MotoDemosaic(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GBRG_MotoDemosaic(USHORT* pSrc, BYTE *pDest, int width, int height, BYTE Bits);

	//Raw转RGB24  3X3 双线性插值 (bayer16)
	BOOL RawtoRGB24_MotoDemosaic(BYTE *pSrcBuffer, BYTE* pDstBuffer, int width, int height, BYTE outformat);
	BOOL Interpolation_BGGR_MotoDemosaic(BYTE* pSrcBuffer, BYTE* pDstBuffer, int width, int height);
	BOOL Interpolation_RGGB_MotoDemosaic(BYTE* pSrcBuffer, BYTE* pDstBuffer, int width, int height);
	BOOL Interpolation_GRBG_MotoDemosaic(BYTE* pSrcBuffer, BYTE* pDstBuffer, int width, int height);
	BOOL Interpolation_GBRG_MotoDemosaic(BYTE* pSrcBuffer, BYTE *pDstBuffer, int width, int height);

	//void CalibraCam(IN_PARAMS in_val, OUT_PARAMS *out_val);
	//CircleArrayPatternAnalysis();
	BOOL CircleArrayPatternAnalysis(BYTE* RGB_buf,int ImgWidth,int ImgHeight,int PointSizeLimit,vector<float> &Out_x,vector<float> &Out_y,float &Mark_x,float &Mark_y );
	
	void ConvertWideToChar(CString wide,char* mchar); 

	void ZoomImgSize(BYTE* ImgBuf,int width,int height,int destwidth,int destheight,CString path,CString strType);

	void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);

	float GetSrValue(float ois_on_dif,float ois_off_dif); 
};

