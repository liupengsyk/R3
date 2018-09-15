#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"

struct _ShadingROI_Info
{
	BOOL Enable;
	CRect pos;
	int cx;
	int cy;
	float aveY;
	float aveR;
	float aveG;
	float aveB;

	float deltaR;
	float deltaG;
	float deltaB;
	float deltaY;
	float RGdif;
	float BGdif;
	int RG_result;
	int BG_result;
	int deltaY_result;
	
	int result;
};


const int ROI_NUM = 49;

struct _Shading
{
	_ShadingROI_Info ROI[ROI_NUM];
	int ROI_height;
	int ROI_width;
	double deltaY_limit;
	double Ydif_limit;
	double RGdif_limit;
	double BGdif_limit;
	double PPdif_limit;//增加一个pp值
	double Ydif_max;
	int Ydif_result;
	int PP_result;	
	double RGdif_max;
	double BGdif_max;
	double PPdif_max;

	double RG_Max ;// 
	double RG_Max2;
	double BG_Max;
	double RG_MaxValue ;// 
	double BG_MaxValue;
	int RGmax_result;
	int BGmax_result;
	BOOL bCheckRG_max;
	BOOL bCheckBG_max;

	int result;
};

class Shading49_Inspection :public TestItemInterface
{
public:
	Shading49_Inspection(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~Shading49_Inspection(void);

	int Testing();       //子类重载测试代码放放在此函数
	int Initialize();      //子类重载 初始化代码
	int Finalize();      //子类重载 初始化

	int InitItem();

	int CamID;

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit();
	LPCTSTR GetReportContents();

private:
	OptionDlg* pDlg;

public:

	BOOL Lock_OneTime;
	CDC* pMemDC; 

	UINT m_exp_tolerance;
	UINT m_exp_target;
	BOOL m_ExposureEn;
	BOOL m_Continue;

	_Shading sShading49;

	int ShadingTest(void);
	void CalcImageRect(void);
	void ShowShadingTest(void);

	CString CamCode;
	int AvgNum;
	vector<float> vec_Ydif_max;
	vector<float> vec_RGdif_max;
	vector<float> vec_BGdif_max;
	vector<float> vec_PPdif_max;
	vector<float> vec_RG_MaxValue;
	vector<float> vec_BG_MaxValue;
	float dif_Max_rb;
	BOOL bRG_ok2;
	BYTE* Y_Buffer;
	ImageInfo img ;

};