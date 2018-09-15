#pragma once

#ifndef _AutoROI_
#define _AutoROI_ extern "C" _declspec(dllexport)

#define  MAX_ROI_NUMBER 13
enum ImageType
{
	BMP_IMAGE = 0,
	RAW8_IMAGE,
	RAW10_PACKED_IMAGE,
	RAW10_UNPACKED_IMAGE,
	OTHER,
};

enum OUTFORMAT_RGB_
{
	OUTFORMAT_RGGB_ = 0,     ///<RGGB输出格式
	OUTFORMAT_GRBG_,	     ///<GRBG输出格式
	OUTFORMAT_GBRG_,	     ///<GBRG输出格式
	OUTFORMAT_BGGR_,	     ///<BGGR输出格式
	OUTFORMAT_FLIP_,
	OUTFORMAT_NOFLIP_,
};

struct OFAutoROIIn
{
	BYTE *ofSrcBuffer;    //输入图像Buffer
	int  ofWidth;
	int  ofHeight;
	ImageType ofImageType;            //输入图像类型
	OUTFORMAT_RGB_  ofOutFormat; //如果是Raw图，需要传入Format;如果是bmp，请选择是否有做Flip
};

struct ROIInfo
{
	CPoint LT;    //左上角坐标
	CPoint RB;    //右下角坐标
};

struct ROIDetailInfo
{
	ROIInfo ROIH1;
	ROIInfo ROIH2;
	ROIInfo ROIH3;
	ROIInfo ROIH4;

	ROIInfo ROIV1;
	ROIInfo ROIV2;
	ROIInfo ROIV3;
	ROIInfo ROIV4;

	double _x1,_x2,_y1,_y2,_A,_B,_C,_D;
};

struct OFAutoROIOut
{
	int  ofROInumber;
	int  ofROISize;
	ROIDetailInfo ofROIposition[MAX_ROI_NUMBER];
};

struct OFAuto4PointOut
{
	CPoint Circle[4];
	CPoint Center;
};

//显示设置规格的界面
_AutoROI_ void   ShowROIInfoDlg(HWND hwnd, BOOL bShowFlag);
//获取中心Block的ROI位置坐标
_AutoROI_ BOOL fnAutoROICenter(OFAutoROIIn *ofSrcDataIn, OFAutoROIOut *ofDestDataOut,char *ErrorMsg); 
//获取选择了的Block的ROI位置坐标
_AutoROI_ BOOL fnAutoROISelect(OFAutoROIIn *ofSrcDataIn, OFAutoROIOut *ofDestDataOut,char *ErrorMsg); 
//获取中心4个圈的位置，该功能用不了
_AutoROI_ BOOL fnGet4Point(OFAutoROIIn *ofSrcDataIn, OFAuto4PointOut *of4PointDataOut,char *ErrorMsg);    

#endif
