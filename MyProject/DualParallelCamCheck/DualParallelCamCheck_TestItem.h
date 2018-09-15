#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "mems_dualccm_twolayer\mems_dualccm_twolayer.h"
#include "ImageProc.h"
#include "LIB_Halcon/include/Halcon.h"
#include "LIB_Halcon/include/cpp/HalconCpp.h"
using namespace std;
using namespace Halcon;
#include <map>
  

struct SpecData
{
	//与控件交换数据
	CString strRotationSpc;
	CString strTiltXSpc;
	CString strTiltYSpc; 

	float RotationSpc;
	float TiltXSpc;
	float TiltYSpc;
	
};
 

struct PointChartInfo
{
	int PointSize; 
	int ObjectLength; 
	int PointPitch;
	int PointNum;	 
};

 
struct CamBaseInfo
{
	float FocusLength;
	float PixelSize;
	CString strFocusLength;
	CString strPixelSize;
};

struct CamInputInfo
{
	
	//setting Input
	CamBaseInfo baseInfo; 
	IN_PARAMS m_IN_PARAMS;
	OUT_PARAMS m_OUT_PARAMS; 
	ImageInfo img;  

	float farPointImgSize;
	float nearPointImgSize;
	//data out
  	DualCamData m_DualCamData;
 };

struct CamDualData
{
	CamInputInfo CamInfo[2];
	PointChartInfo Infinity_PointChartInfo;
  	PointChartInfo Macro_PointChartInfo; 
	SpecData m_SpecData; 
	DualCamData DualData_dif;
};
 
class DualParallelCamCheck_TestItem :public TestItemInterface
{
public:
	DualParallelCamCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~DualParallelCamCheck_TestItem(void);

	int InitItem();

	int Testing();       //子类重载测试代码放放在此函数
	int Initialize();      //子类重载 初始化代码
	int Finalize();      //子类重载 初始化

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit() ;
	LPCTSTR GetReportContents() ;

	int GetImgPointFailTimes[2];

private:
	OptionDlg* pDlg;
	 int CamID; 
public: 

	BOOL bContinue;  
	BOOL bOnAA; 
	int TestTimes;  
	int TimesSet; 

	CamDualData CamCal;

	//获取tilt信息
	int GetTilt_dif();
	int GetCamTilt(int CamID);
	int CamCalInit(); 

	void SetDualCamData();  

	int ProcTiltData(); 
	void ShowInfo(); 
	 
	void SortPoints (Halcon::HTuple inX, Halcon::HTuple inY, Halcon::HTuple start, Halcon::HTuple end, Halcon::HTuple *outX, Halcon::HTuple *outY);
	void GetImgPoint_xyByHalcon(int CamID,vector<float> *Point_xy);
	void GetImgPoint_xyByHalconContours(int CamID,vector<float> *Point_xy); 
	void ShowInfo(int TopCut,int LeftCut,int BottomCut,int RightCut,int imgwidth,int imgheight,int camid);
	void GetVerCutImage(int &CurrentTopCut,int &CurrentBottomCut,ImageInfo* pImg);
	void GetHorCutImage(int &CurrentLeftCut,int &CurrentRightCut,ImageInfo* pImg);

	unsigned char *pBmpRed ;
	unsigned char *pBmpGreen ;
	unsigned char *pBmpBlue ; 

	float offset_tx;
	float offset_ty;
	float offset_tz;
	
	static int HistoThreshold;
private:
	int TimeStart; 

	int GetPointErrorProc(int CamID);
};

