#pragma once

#include "ccmBaseInterface.h"
#include "TestItemInterface.h"
#include "ShadingOptionDlg.h"

struct _ShadingROI_Info
{
	BOOL Enable;
	CRect pos;
	int cx;
	int cy;
	int aveY;
	int aveR;
	int aveG;
	int aveB;
	int aveGr;
	int aveGb;

	double deltaR;
	double deltaG;
	double deltaB;
	double deltaY;

	int RGain;
	int BGain;
	int YRatio;

	double RGdif;
	double BGdif;
	int RG_result;
	int BG_result;
	int deltaY_result;
	int result;
};

struct _Shading
{
	_ShadingROI_Info ROI[5];
	int ROI_height;
	int ROI_width;
	double deltaY_limit;
	double Ydif_limit;
	double RGdif_limit;
	double BGdif_limit;
	double Ydif_max;
	int Ydif_result;
	double RGdif_max;
	double BGdif_max;
	double PPdif_max;
	int result;
};
enum OUTFORMAT_RGB
{
	OUTFORMAT_RGGB = 0,///<RGGB输出格式
	OUTFORMAT_GRBG,///<GRBG输出格式
	OUTFORMAT_GBRG,///<GBRG输出格式
	OUTFORMAT_BGGR,///<BGGR输出格式
}; 

#define BLK_NUM_H											5
#define BLK_NUM_V											5
#define BLK_CENTER_INX										(BLK_NUM_H * BLK_NUM_V / 2)
typedef struct
{
	int R;
	int Gr;
	int Gb;
	int B;
	int Y;
	float RG;
	float BG;
}_SHADING_INFO_;

class Inspection_Shading :public TestItemInterface
{
public:

	Inspection_Shading(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~Inspection_Shading(void);

	int Testing();       //测试代码放在这里
	int Initialize();     //初始化代码放在这里
	int Finalize();      //结束代码放在这里

	int InitItem();

	int LoadOption();
	int SaveOption();


	int CamID;

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit();
	LPCTSTR GetReportContents();

	ShadingOptionDlg* pDlg;

	BOOL Lock_OneTime;
	CDC* pMemDC; 

	_Shading sShading;
	int ShadingTest(void);
	void GetAWBInfoRaw8(int block);

	void CalcImageRect(void);
	void ShowShadingTest(void);

	CString m_strTestStation;
	void SaveColorShadingData(float *pRG, float *pBG, int Num);
	void SaveShadingChannelData(_SHADING_INFO_ *pShadingInfo, int Num);

	void GetBlock(BYTE * Raw8buffer, int x, int y, int width, int height, _SHADING_INFO_ *pShadingInfo);
	void GetRaw10PerBlock(void);
	_SHADING_INFO_ m_ShadingInfo[BLK_NUM_H * BLK_NUM_V];

	CString CamCode;
	BYTE* Y_Buffer;
	ImageInfo img;
	float Rgain[5];
	float Bgain[5];
	float ColorRGain[5];
	float ColorBGain[5];
	float Y[5];
};

