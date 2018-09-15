#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 
#include "AutoROI.h"
#define MAXSTRLEN 512

#define  MAXROICOUNT	100
#define  MAXSTEPCOUNT	1024
struct AFData	 
{
	double SFR[MAXROICOUNT];
	int DAC;
};
struct ContrastData
{
	ULONG64 iContrastValue;
	int DAC;
};
struct MTFData
{
	double dMTF;
	int DAC;
};
struct SpecInfo
{
	wchar_t ROIName[MAXSTRLEN];	//ROIName
	float SFRSpec;								//SFR Spec			
};
struct ROIPosInfo
{
	int iLeft;
	int iTop;
	int iRight;
	int iBottom;
	int iWidth;
	int iHeight;
};
struct OfMarker
{
	int x;
	int y;
	int width;
	int height;
	COLORREF Color;
};

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

class AF_SFR_TestItem :public TestItemInterface
{
public:
	AF_SFR_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~AF_SFR_TestItem(void);

	class COPtion
	{
	public:
		int m_iSFRBurnRate;
		BOOL bSaveImage;
		BOOL bEngineerMode;
		BOOL m_bMidValue;//是否使用中值
		int m_iStartCode;
		int m_iEndCode;
		int m_iSkipFrame;

//		CString strSpecROIs;
		int m_iStep1;
		int m_iStep2;
		int m_iStep3;
		
		int m_iFrequency;

		BOOL m_bShakeEn;
		int m_iShakeSkipFrame;
		int m_iShakeCount;
		int m_iShakeDAC;
		
		int m_iPeakMin;
		int m_iPeakMax;

		float m_fCTC8;
		float m_fCTC6;
		float m_fCTC5;
		float m_fCTC8_H;
		float m_fCTC8_V;
		float m_fCTC6_H;
		float m_fCTC6_V;
		float m_fCTC5_H;
		float m_fCTC5_V;
		
		float m_CenH_Spec;
		float m_CenV_Spec;
		float m_F3H_Spec;
		float m_F3V_Spec;
		float m_F4H_Spec;
		float m_F4V_Spec;
		float m_F5H_Spec;
		float m_F5V_Spec;
		float m_F6H_Spec;
		float m_F6V_Spec;
		float m_F8H_Spec;
		float m_F8V_Spec;
		int m_Mid_TestTime;
		CString m_StationName;

		int ErrorCode;
		int m_InfinityCode;
		int m_MacroCode;
		int m_FQCCode;

		int m_iAFMethod;	//AF对焦算法：0:SFR   1:Contrast
		int m_iContrastROIWidth;
		int m_iContrastROIHeight;
		int m_iContrastROIStartX;
		int m_iContrastROIStartY;
	};
	COPtion m_Options;
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

private:
	float GetSFRValue(unsigned char *_InImageBuffer,ROIInfo roiinfo,int ROISize,wchar_t *stringinfo,int index,int blockIndex);
	bool PCutImageBuffer(unsigned char *_InImageBuffer,int _Width,int _Height,UINT _ByteDepth,UINT _ColorChannel, int _mStartX,int _mStartY,int _CutWidth,int _CutHeight,unsigned char *_OutImageBuffer);
	void GetBayerType(int iImageType,int iStartX,int iStartY,int& outformat);
	void RawToGrayscaleImage_Raw10(unsigned char *m_pBuff,int outformat,int m_width,int m_height,unsigned char *pOutRaw10);
	
	void StringSplit( const wstring & str, vector<wstring> & tokens, const wstring & delimiters);  
	BOOL AnalysisROISpec();		
	BOOL FindPeak(int& iPeakDAC,int StartIndex,int step);

	BOOL SFRTest(OFAutoROIOut ofDestDataOut,OfMarker* _ROIInfo,int &iROICount);
	BOOL AutoROI(OFAutoROIOut &ofDestDataOut);
	void DesideNextCode();
	BOOL DrawText(OfMarker* pStROIInfo,int iROICount,int iBlockCount);
	BOOL SaveSFRPosInfo();
	BOOL CheckFinalSpec();

	BOOL SaveAllData();
	BOOL SaveFinalData();
	BOOL SaveCheckData();
	BOOL SaveSFR();

private:
	BOOL m_bResult;
	int m_iCode;
	int m_iEndCode;
	int m_iStep;
	int m_iShakeTime;
	int m_iSkipFrame;
	int m_iPeroid;
	int m_nJumpPos;
	int m_iLastPos;
	OptionDlg* pDlg;
	BOOL m_bAFGoing;
	 int CamID;

	 int Width;
	 int Height;
	  
	 int m_iROICount;	//实际计算的ROI个数
	 int m_iSFRROICount;	//用于统计的ROI个数

	 OfMarker m_stROIInfo[160];
	 int m_iBlockCount;	//大框的个数

	 BOOL m_bROIConfigureError;
	 SpecInfo m_stSpec[MAXROICOUNT];
	 AFData m_stAFData[MAXSTEPCOUNT];   //记录全程中每个ROI的SFR值及其对应的DAC值
	 float m_dSFRValue[MAXROICOUNT];	//记录当前DAC对应的各个ROI的SFR	
	 ROIPosInfo m_stROIPos[MAXROICOUNT];		//记录设定中制定的ROI的具体位置坐标
	 float SFRAll[10][MAXROICOUNT];//记录回到Peak点后测试的全部SFR值
	 int iTestTimes; //回到Peak点后测试SFR的次数
	 int m_iPos;	//记录全程中AF的步数				
	 int m_iPeakDAC;	//记录峰值
	 float m_fCTC8;
	 float m_fCTC6;
	 float m_fCTC5;
	 float m_fCTC8_H;
	 float m_fCTC8_V;
	 float m_fCTC6_H;
	 float m_fCTC6_V;
	 float m_fCTC5_H;
	 float m_fCTC5_V;

	 ImageInfo img;
	 bool m_bGotAllROI;

	 OFAutoROIOut m_ofDestDataOut;
	 BOOL m_bFinalCheck;
	 BOOL m_bFocusing;
	 BOOL fqcFirstTime;
	 int m_iFinalAutoROITime;
	 int m_iFinalSFRTest;
	 CString m_errMsg;
	 int m_iROIFailCount;

	 //Contrast 参数
	 ContrastData m_stContrastData[100];
	 //MTF 参数
	 MTFData m_stMTFData[100];
	 float m_testFreq;
public:
	ULONG64 ContrastMeasure();
	void DrawContrastInfo();
	double MTFTest();
public:
	CString m_cfgName;
	BOOL bSaveBmpFile(CString sfilename, BYTE *pBuffer, UINT width, UINT height);
	CString GetModulePath();
	int GetTemperature(int* temperature);
	int GetTemperature_Driver(USHORT* temperature);
private:
	BOOL CodeDiffCheck(void);
	int GetPeakCodeByAfValue(double* afVal, double* afCode);
	int GetBestCode(CString afMode, AF_STEP_DATA stepData, int stepCode, int* startCode, int* endCode, float* bestAv);
public:
	USHORT m_CurrentStartCode;
	USHORT m_CurrentEndCode;
};