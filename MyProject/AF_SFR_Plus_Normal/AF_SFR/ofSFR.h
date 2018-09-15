// ofSFR.h : ofSFR DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


typedef struct _SFR_SINGLE_ROI_PARA
{
	BOOL TopRoiEn;
	BOOL BottomRoiEn;
	BOOL LeftRoiEn;
	BOOL RightRoiEn;

	int Freq;
	int SpecH;
	int SpecV;

	int OffsetX;
	int OffsetY;
	int OffsetValue;
} SFR_SINGLE_ROI_PARA, *PSFR_SINGLE_ROI_PARA;

typedef struct _SFR_PARA
{
	SFR_SINGLE_ROI_PARA CurrSigRoiPara;
	SFR_SINGLE_ROI_PARA AllRoiPara[21];

	BOOL SetSfrParaEn;

	int AreaThred;
	int LightThred;
	int RoiWidth;
	int RoiHeight;
	int RoiOffsetX;
	int RoiOffsetY;

	int TestCnt;
	BOOL ContinueTest;
	BOOL SaveRoiImage;
	BOOL Demosaic;
} SFR_PARA, *PSFR_PARA;

typedef struct _SFR_ROI_VALUE
{
	double topVal;
	double bottomVal;
	double leftVal;
	double rightVal;

	BOOL topResult;
	BOOL bottomResult;
	BOOL leftResult;
	BOOL rightResult;
} SFR_ROI_VALUE, *PSFR_ROI_VALUE;

typedef struct _STEP_INFO
{
	int code;
	float av;
} STEP_INFO, *PSTEP_INFO;

typedef struct _AF_STEP_DATA
{
	int bigStepCnt;
	STEP_INFO bigStepData[256];
	int miniStepCnt;
	STEP_INFO miniStepData[256];
} AF_STEP_DATA, *PAF_STEP_DATA;


// CofSFRApp
// 有关此类实现的信息，请参阅 ofSFR.cpp
//

class CofSFRApp : public CWinApp
{
public:
	CofSFRApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


BOOL SFR_Test(BYTE* bmpChannelYBuff, int imgWidth, int imgHeight, SFR_PARA sfrPara, SFR_ROI_VALUE* sfrRoiValue);

int GetAllTestTargetCenter(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* fov85Cn, CPoint* fov55Cn, CPoint* fov35Cn, CPoint* fovCn);

int GetCenterForAF(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* fovCn);

int GetCenterMotoChartForAF(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* fovCn);

void AllROI_Sort(CPoint* fov85Cn, CPoint* fov55Cn, CPoint* fov35Cn, CPoint* fovCn, CPoint* outSortROI);

int SaveRoiImageForSFR(BYTE* pRoiBuff, CRect roiRect, CString filaName);

double GetSfrValueForROI(BYTE* pYBuffData, int roiNum, CString type, int imgWidth, int imgHeight, int offsetX, int offsetY, SFR_PARA sfrPara, CPoint* sortRoi);

double GetSfrValueForRawBalanceROI(USHORT* pRawData, int roiNum, CString type, int imgWidth, int imgHeight, int offsetX, int offsetY, SFR_PARA sfrPara, CPoint* sortRoi);

int GetAllTestTargetCenterForMotoChart(BYTE* pGrayBuff, int imgWidth, int imgHeight, CPoint* target);

double GetSfrValueForROI_MotoChart(BYTE* pYBuffData, int roiNum, int imgWidth, int imgHeight, int offsetX, int offsetY, SFR_PARA sfrPara, CPoint* sortRoi);

bool PCutImageBufferDll(unsigned char *_InImageBuffer, int _Width, int _Height, int _ByteDepth, int _mStartX, int _mStartY, int _CutWidth, int _CutHeight, unsigned char *_OutImageBuffer);

void GetBayerTypeDll(int iImageType, int iStartX, int iStartY, int& outformat);

void RawToGrayscaleImage_Raw10Dll(unsigned char *m_pBuff, int outformat, int m_width, int m_height, unsigned char *pOutRaw10);

int GetBestCode(CString afMode, AF_STEP_DATA stepData, int stepCode, int* startCode, int* endCode, float* bestAv);

BOOL IsRGB_forQualCali(BYTE* rgb24, int imgWidth, int imgHeight);

int GetSquareChartTargetCenter(BYTE* pGrayBuff, CSize imgSize, CPoint tagtPosi, CSize maskSize, int maskPhi, int* type, CPoint* roiCenterPosi, CString* msg);

int GetSquareChartSfrValueForOPPO(int roiNum, USHORT* pRawData, int rawFrm, CSize imgSize, int* roiType, CPoint* roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg);

int GetSquareChartSfrValueForWhiteBalance(int roiNum, USHORT* pRawData, int rawFrm, CSize imgSize, int* roiType, CPoint* roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg);

int GetXChartTargetCenter(BYTE* pGrayBuff, CSize imgSize, CPoint tagtPosi, CSize maskSize, CPoint* roiCenterPosi, CString* msg);

int GetXChartSfrValue(int roiNum, USHORT* pRawData, int rawFrm, CSize imgSize, CPoint* roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg);

int GetMotoChartTargetCenter(BYTE* pGrayBuff, CSize imgSize, CPoint tagtPosi, CSize maskSize, int maskOffsetX, CPoint* roiCenterPosi, CString* msg);

int GetMotoChartSfrValue(int roiNum, USHORT* pRawData, int rawFrm, CSize imgSize, CPoint* roiCnPosi, CSize roiSize, float freq, float* sfrValue, CString* msg);