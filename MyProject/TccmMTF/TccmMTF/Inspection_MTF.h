#pragma once

#include "ccmBaseInterface.h"
#include "MTFOptionDlg.h"
#include "TestItemInterface.h"
#include <deque>
#include "CTimeCount.h"
using namespace std;


struct ROI_Info
{
	CRect pos;
	int cx;
	int cy;
	double MTFvalue;
	double HVratio;
	deque<double> MTF_Deque;
	int TestResult;
	int limit1;
	int limit2;
	BOOL Enable;

};

struct _MTF
{
	ROI_Info ROI[16];
	int ROI_height;
	int ROI_width;
	int CornerDiffLimit;
	int HVratioLimit;  //100
	int DequeSize;
};


class Inspection_MTF :	public TestItemInterface
{
public:
	Inspection_MTF(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~Inspection_MTF(void);
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

	MTFOptionDlg* pDlg;

	BOOL Lock_OneTime;
	CDC* pMemDC; 

	_MTF sMTF;

	int  MTFTest(void);
	void ShowMTFinfo(BOOL bResult);
	void CalcImageRect(void);
	int  m_Alg;
	double GetHVResolutionRatio(BYTE* pScr,int width,int height,CRect rect);
	int  OnCtrlKeyNum();

	double diff;

	BOOL RotateBack_Flag;
	BOOL RotateBack_en;
	int RotateBack_MTF_UpperLimit;
	int RotateBack_MTF_LowerLimit;

	BOOL m_bRotate;
	CTimeCount m_timeCnt;
	int Runmode ;
	int DelayNum;

	int CamID;

	int GetTemperate();
	BYTE* Y_Buffer;
	ImageInfo img  ;
};

