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
	//ѹ��RGB24ͼ��
	BOOL ZoomOutRGBImage(BYTE *pSrc, int width, int height, BYTE* pDest, int ratio, int flag = 0);

	//ѹ��Yͼ��
	BOOL ZoomOutYImage(BYTE *pSrc, int width, int height, BYTE* pDest, int ratio, int flag = 0);

	//���Ųɼ������ѹ��raw��ʽת��Ϊ��׼RAW
	BOOL DTRaw10toRaw8(BYTE *pSrc, BYTE* pDest, int width, int height);

	BOOL DTRaw8toRaw(BYTE* pSrc, USHORT* pDest, int width, int height);
	BOOL DTRaw10toRaw(BYTE* pSrc, USHORT* pDest, int width, int height);
	BOOL DTRaw12toRaw(BYTE* pSrc, USHORT* pDest, int width, int height);
	BOOL RawToRaw8(USHORT* pSrc, BYTE* pDest, int width, int height,BYTE bits);
	BOOL RawToRaw8(BYTE* pSrc, BYTE* pDest, int width, int height);

	//�õ�ͼ��Y����ͼ
	BOOL RGB24toY(BYTE *pSrc,BYTE* pDest, int width, int height);
	//����Y���ȵõ�ĳ�������ƽ������
	float GetAveY_ROI(BYTE *pSrc, int width, int height,CRect rect);

	//��ȡĳ������ĺڿ���������� 1��ʾ��ʼʶ��0��ʾ����
	//��ȡָ��roi�������
	//ָ��rect roi
	//������������roiƽ�����ȵ�ֵ ratio
	// faratio
	//Type ָ��ģʽ ģʽ1Ϊ��������ģ�ģʽ 0 Ϊ���»���Roi
	//�������������  ���º��roi outRect
	void GetBlackBlobkCenterY_ROI(BYTE *pSrc, int width, int height,CRect rect,int ratio,float &x,float &y);
	void AjustRoiRect(BYTE *pSrc, int width, int height,int ratio,CRect rect,CRect *outRect);

	//�����ֵ�����ͼ��
	//��ȡָ��roi�������
	//ָ��rect roi 
	//Type ָ��ģʽ ģʽ1Ϊ��������ģ� 
	// ����������,�Լ���Ĵ�С
	void GetBlackBlobkCenterY_BinaryROI(BYTE *pSrc, int width, int height,CRect rect,float& Point_x,float& Point_y,int &PoinSize);

	//����Ҷ�ͼ���ѧ����  ratio ��������ٷֱ�%
	CPoint  GetOpticalCenter(BYTE *pSrc, int width, int height,int ratio = 7);
	//�õ�0-255ֱ��ͼ
	int* GetHistogram(BYTE *pSrc, int width, int height );
	int GetHistogramThreshold(BYTE *pSrc, int width, int height, float ratio);

	int* GetHistogramY_ROI(BYTE *pSrc, int width, int height,CRect rect );
	//***************��ȡֱ��ͼָ��������������ֵ ---��������ʼ������***************************//
	int  GetHistogramThreshold(BYTE *pSrc, int width, int height, CRect rect,float ratio);
	//***************��ȡֱ��ͼָ��������������ֵ---�����ʼ������***************************//
	int  GetHistogramLowThreshold(BYTE *pSrc, int width, int height, float ratio);

	//MTF 
	double GetMTF( BYTE *pSrc, int width, int height,CRect rect);
	//����ڰױ��ص�MTF
	double GetMTF_ratio( BYTE *pSrc, int width, int height,CRect rect,int ratio);
	//�������㷨��MTF
	double GetMTF_Iteration( BYTE *pSrc, int width, int height,CRect rect);
	//FV�㷨
	double GetResolution_FV(BYTE* pSrc, int width, int height,CRect rect);

	double nGetMTF(BYTE *pSrc, int width, int height,CRect rect);


	/*************************BAYER��ֵ*************************************/

	//Raw8תRGB24  3X3 ˫���Բ�ֵ (bayer8)
	BOOL Raw8toRGB24(BYTE *pSrc, BYTE* pDest, int width, int height,BYTE outformat);
	BOOL Interpolation_BGGR(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL Interpolation_RGGB(BYTE* pSrc, BYTE* pDest, int width, int height);
	BOOL Interpolation_GRBG(BYTE* pSrc, BYTE* pDest, int width, int height);

	

//���ŵ�RawתRGB24��add by zl 2013.11.29
//�� ע�� ��������ݳ����Ų�ֵ��ʽ��//RawתRGB24  3X3 ˫���Բ�ֵ (bayer16)
	BOOL RawtoRGB24(USHORT *pSrc, BYTE* pDest, int width, int height, BYTE outformat, BYTE Bits);
	BOOL Interpolation_BGGR(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_RGGB(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GRBG(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GBRG(USHORT* pSrc, BYTE *pDest, int width, int height, BYTE Bits);
//      ��Gͨ����RGBʱ��G����������ֵ���� G(x,y) != G(x,y) ���� G(x,y) = (G(x,y) + G(x+1,y-1)/2)������ͬ˫���Բ�ֵ
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

	//RGB24תRaw ��Ե㸴ԭ
	BOOL RGB24toRaw(BYTE *pSrc, USHORT* pDest, int width, int height, BYTE outformat, BYTE Bits);
    BOOL RGB24toRaw_RGGB(BYTE* pSrc,USHORT* pDest, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_GBRG(BYTE* pSrc,USHORT* pDest, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_BGGR(BYTE* pSrc,USHORT* pDest, int width, int height, BYTE Bits);
	BOOL RGB24toRaw_GRBG(BYTE* pSrc,USHORT* pDest, int width, int height, BYTE Bits);

	void RGB24toNV21(BYTE* pSrc,BYTE* pDest, int width, int height);

	//��ֵ��ͼ��
	BOOL  Binary(BYTE *pSrc, int width, int height, int Threshold);
	//��ֵ��ͼ��
	BOOL  Binary(int *pSrc, int width, int height, int Threshold);

	//�õ�ĳ��ͨ����buffer
	USHORT* GetGr_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetGb_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetR_Channel(USHORT* pSrc, int width, int height, int outformat);
	USHORT* GetB_Channel(USHORT* pSrc, int width, int height, int outformat);

	//�õ�ĳ������ı�׼��
	int GetStandardDeviation(BYTE* pSrc, int width, int height,CRect rect);

	//�õ�ƽ��֡����
	BOOL AverageFrameData(BYTE* pSrc, int width, int height, int framelevel);
	BOOL AverageFrameData(USHORT* pSrc, int width, int height, int framelevel);
	BOOL AverageFrameData_RGB24(BYTE* pSrc, int width, int height, int framelevel);
	
	//�õ�ĳ��ͨ��������ƽ��ֵ
	float GetAveR_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat);
	float GetAveG_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat);
	float GetAveB_ROI_Raw(USHORT *pSrc, int width, int height,CRect rect, int outformat);

	//�õ�RGBĳ�������ƽ��ֵ
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


	BOOL Sharpen(BYTE* pSrc, BYTE* pDest, int width, int height, int* Template); //��ֵ��

	BOOL Smooth(BYTE* pSrc, BYTE* pDest, int width, int height, int* Template);//��ֵƽ��

	BOOL Template3x3(BYTE* pSrc, BYTE* pDest, int width, int height, int* Template);

	//RGBתLAB
	BOOL RGB24ToLab(BYTE *pScr, double *pDest, int width, int height);//����photoshop���㷨
	double GetAveL_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//����photoshop���㷨
	double GetAvea_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//����photoshop���㷨
	double GetAveb_LabROI_RGB24(BYTE* pScr, int width, int height, CRect rect);//����photoshop���㷨
	BOOL RGBToLab_Pixel(BYTE R, BYTE G, BYTE B, double &lab_l, double &lab_a, double &lab_b);//����photoshop���㷨

	//����tvת digital �㷨 lifangsen 2014.08.01
	////////////ELAȥ�����㷨///////////////////
	void EdageDirectLineAvg(unsigned char *pDisplayBuffer, int width, int height);

	///////////////����ƽ�� Bob //////////////////////
	void Bob(unsigned char *pDisplayBuffer, int width, int height);

	/////////////��֯��weave////////////
	void weave(unsigned char *pDisplayBuffer, int width, int height);

	////////////��̬����ȥ�����㷨///////////////////
	void DynamicModule(unsigned char *pDisplayBuffer, int width, int height);

	////////////////��̬ELA///////////////////////////
	void DynamicELA(unsigned char *pDisplayBuffer, int width, int height);

	////*******************������ELA********************////////
	void ModifiedELA(unsigned char *pDisplayBuffer, int width, int height);

	//***********************��������Ӧ***********************//
	void EdageAdption(unsigned char *pDisplayBuffer, int width, int height);
	void NormalizationAndBinaryzation( BYTE* pBmp, CPoint pStart, CSize pROIRect, UINT iImgWidth, UINT iImgHeight );
	
	//***********************��ȡ���������������***********************//
	UINT GetSingleCircleCenter( BYTE* pBmp, CPoint pStart, CSize pROIRect, UINT iImgWidth, UINT iImgHeight, float& centerX, float& centerY );

	//****************Y��ǿ***************//
	BYTE ImageEnhanceY(BYTE *_srcImage,BYTE *DestImage, UINT Width,UINT  Height);
	//****************Y �ֲ���ǿ***************//
	BYTE ImageEnhanceY(BYTE *_srcImage,BYTE *DestImage, CRect rect,UINT Width,UINT  Height);

	//****************Y �ֲ���ǿ***************//
	BYTE ImageEnhanceY(int *_srcImage,BYTE *DestImage, CRect rect,UINT Width,UINT  Height);
	 

	//****************rgb��ǿ***************//
	BYTE ImageEnhanceRGB(BYTE *_srcImage,BYTE *EnhanceBMPBuffer, int width, int height);

	//****************ͼ��ѹ��***************//
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
	//ͼ������ +-*/	
	BOOL ImageMath(BYTE *_SrcImage1, BYTE *_SrcImage2, BYTE *DestImage, int srcwidth, int srcheight, int _Operater);


	//_Operater
	//enum math
	//{
	//	math_Add,
	//	math_Sub,
	//	math_Mult,
	//	math_Div 
	//};
	//ͼ������ +-*/	
	BOOL ImageMath(BYTE *_SrcImage1, BYTE *_SrcImage2, int *DestImage, int srcwidth, int srcheight, int _Operater);

	//_Operater
	//enum math
	//{
	//	math_Add,
	//	math_Sub,
	//	math_Mult,
	//	math_Div 
	//};
	//ͼ������ +-*/	
	BYTE  PixelMath(BYTE *_SrcImage, double MeanValue, BYTE *_DestImage,int srcwidth, int srcheight, int _Operater);
 
 
  
	void RowDif( BYTE* SrcBuf, BYTE *DstBuf, int ImgWidth, int ImgHeigh,CRect rect, BYTE Threshold );

	void  LineDif( BYTE* SrcBuf, BYTE *DstBuf, int ImgWidth, int ImgHeigh,CRect rect, BYTE Threshold );

	//SrcBuf1�Ǻϲ����ͼƬ
	void MergeImage( BYTE* SrcBuf1, BYTE* SrcBuf2, int ImgWidth, int ImgHeight,CRect rect );

	 
	BOOL VertFlipBuf(BYTE  * inbuf,  UINT ImgSizeBytes,UINT height);  

	void GetSfrRoi_byHalcon(BYTE* RGB_buf,int ImgWidth,int ImgHeight,PSFR_PARAMETER pSfrPara);

	int  GetSFR_ofilm( USHORT *_imageBuffer, int _height, int _width,CRect RoiRect,float freqInput ,float& sfrOut,_sfrresult *sfrRet );
	int  GetSFR_ofilm( BYTE *_imageBuffer, int _height, int _width,CRect RoiRect,float freqInput ,float& sfrOut,_sfrresult *sfrRet );
	void MotoDemosaic( BYTE* pSrcBuffer, BYTE* pDstBuffer, int nImgWidth, int nImgHeight, int bayerType  );

	//RawתRGB24  3X3 ˫���Բ�ֵ (bayer16)
	BOOL RawtoRGB24MotoDemosaic(USHORT *pSrc, BYTE* pDest, int width, int height, BYTE outformat, BYTE Bits);
	BOOL Interpolation_BGGR_MotoDemosaic(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_RGGB_MotoDemosaic(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GRBG_MotoDemosaic(USHORT* pSrc, BYTE* pDest, int width, int height, BYTE Bits);
	BOOL Interpolation_GBRG_MotoDemosaic(USHORT* pSrc, BYTE *pDest, int width, int height, BYTE Bits);

	//RawתRGB24  3X3 ˫���Բ�ֵ (bayer16)
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

