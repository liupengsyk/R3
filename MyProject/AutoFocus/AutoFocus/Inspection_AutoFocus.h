#pragma once

#include "ccmBaseInterface.h"
#include "AutoFocusOptionDlg.h"
#include "TestItemInterface.h"
#include "AFCurve.h"
#include "ImageProc.h"

#define AF_EndFlag 100
class Inspection_AutoFocus :public TestItemInterface
{
public:

	Inspection_AutoFocus(ccmBaseInterface* pInterface,LPCTSTR lpszName);
	~Inspection_AutoFocus(void);

	int InitItem();

	int Testing();       //测试代码放在这里
	int Initialize();     //初始化代码放在这里
	int Finalize();      //结束代码放在这里

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit() ;
	LPCTSTR GetReportContents() ;

	int AutoFocus();

	int CurSearchStep;

    AFCurve m_Curve;
    int m_Direction;
	int m_CurStepCode;


private:
	AutoFocusOptionDlg* pDlg;
public:
	int Show(vector<AFinfo>* Array);
	BOOL Lock_OneTime;
	CDC* pMemDC; 

	double GetROIValue(int Alg_type);
	 
	SearchParam m_SearchParam[4];

	int Code_max;
	int Code_min;

	int OnCtrlKeyNum();

	int FramePitch;
	int AF_End_Check(void);

	int BestAFcode; 
	int CalculateCode;

	int CamID;
	
	int Code_upperlimit;
	int ZeroNum;//sunle

	static int RecomAFcode;
	 
	vector<int> vecCurrentTemperature;
	int GetTemperate();
	vector<int> vecCode;
	vector<double> vecValue;
	 
	void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);

public:
	//SFR_PARAMETER m_sfrPara;
	int m_Alg_Type;
	int m_ResolutionLimit_Alg;
	//int ShowSFR_Info();
	//int ShowSFR_InfoAF();
	double GetContrastAF(BYTE *pBuffer, int width, int height,CRect rect,int type);
	void MIPIP10ToRaw8(USHORT * pIn, USHORT * pOut, long number);
	BOOL Parabola(long double* x, long double* y, int n, long double* a, long double* r);
	BOOL Solve(long double** A, long double* b, long double* x, int n);

	int GetCruveAF(int m_CutStep);
	double GetResolution_Sobel(BYTE* pSrc, int width, int height,CRect rect);
	double GetResolution_Variance(BYTE* pSrc, int width, int height,CRect rect);
	double GetResolution_FVUp(BYTE* pSrc, int width, int height,CRect rect);
	

	ImageInfo img;
	BYTE* Y_buffer ;
//	BYTE *ROI_Buffer;
	CRect AFRect;
	int SearchCode;
	DWORD t[10];
	CString strLog;
	int NGTimes;
};

