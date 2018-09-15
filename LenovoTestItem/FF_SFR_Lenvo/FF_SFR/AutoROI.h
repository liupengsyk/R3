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
	OUTFORMAT_RGGB_ = 0,     ///<RGGB�����ʽ
	OUTFORMAT_GRBG_,	     ///<GRBG�����ʽ
	OUTFORMAT_GBRG_,	     ///<GBRG�����ʽ
	OUTFORMAT_BGGR_,	     ///<BGGR�����ʽ
	OUTFORMAT_FLIP_,
	OUTFORMAT_NOFLIP_,
};

struct OFAutoROIIn
{
	BYTE *ofSrcBuffer;    //����ͼ��Buffer
	int  ofWidth;
	int  ofHeight;
	ImageType ofImageType;            //����ͼ������
	OUTFORMAT_RGB_  ofOutFormat; //�����Rawͼ����Ҫ����Format;�����bmp����ѡ���Ƿ�����Flip
};

struct ROIInfo
{
	CPoint LT;    //���Ͻ�����
	CPoint RB;    //���½�����
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

//��ʾ���ù��Ľ���
_AutoROI_ void   ShowROIInfoDlg(HWND hwnd, BOOL bShowFlag);
//��ȡ����Block��ROIλ������
_AutoROI_ BOOL fnAutoROICenter(OFAutoROIIn *ofSrcDataIn, OFAutoROIOut *ofDestDataOut,char *ErrorMsg); 
//��ȡѡ���˵�Block��ROIλ������
_AutoROI_ BOOL fnAutoROISelect(OFAutoROIIn *ofSrcDataIn, OFAutoROIOut *ofDestDataOut,char *ErrorMsg); 
//��ȡ����4��Ȧ��λ�ã��ù����ò���
_AutoROI_ BOOL fnGet4Point(OFAutoROIIn *ofSrcDataIn, OFAuto4PointOut *of4PointDataOut,char *ErrorMsg);    

#endif
