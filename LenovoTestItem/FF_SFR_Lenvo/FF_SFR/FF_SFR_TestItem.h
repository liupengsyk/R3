#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 
#include "AutoROI.h"
#define MAXSTRLEN 512

#define  MAXROICOUNT	68
struct AFData	 
{
	double SFR[MAXROICOUNT];
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
class FF_SFR_TestItem :public TestItemInterface
{
public:
	FF_SFR_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~FF_SFR_TestItem(void);

	class COPtion
	{
	public:
		int m_iSFRBurnRate;
		BOOL bSaveImage;
		BOOL bEngineerMode;
		BOOL m_bMidValue;//是否使用中值
		int m_iFrequency;
		int m_iShakeSkipFrame;

		float m_fCTC8;
		float m_fCTC6;
		float m_fCTC8_H;
		float m_fCTC8_V;
		float m_fCTC6_H;
		float m_fCTC6_V;
		
		float m_CenH_Spec;
		float m_CenV_Spec;
		float m_F4H_Spec;
		float m_F4V_Spec;
		float m_F6_Spec;
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
	float GetSFRValue(unsigned char *_InImageBuffer,ROIInfo roiinfo,int ROISize,wchar_t *stringinfo,int index);
	bool PCutImageBuffer(unsigned char *_InImageBuffer,int _Width,int _Height,UINT _ByteDepth,UINT _ColorChannel, int _mStartX,int _mStartY,int _CutWidth,int _CutHeight,unsigned char *_OutImageBuffer);
	void GetBayerType(int iImageType,int iStartX,int iStartY,int& outformat);
	void RawToGrayscaleImage_Raw10(unsigned char *m_pBuff,int outformat,int m_width,int m_height,unsigned char *pOutRaw10);
	
	void StringSplit( const wstring & str, vector<wstring> & tokens, const wstring & delimiters);  
	BOOL AnalysisROISpec();		

	BOOL SFRTest(OFAutoROIOut ofDestDataOut,OfMarker* _ROIInfo,int &iROICount);
	BOOL AutoROI(OFAutoROIOut &ofDestDataOut);

	BOOL DrawText(OfMarker* pStROIInfo,int iROICount,int iBlockCount);
	BOOL CheckFinalSpec();
	BOOL SaveCheckData();
	BOOL SaveSFR();
private:
	BOOL m_bResult;

	int m_iSkipFrame;
	int m_nJumpPos;
	OptionDlg* pDlg;
	 int CamID;

	 int Width;
	 int Height;
	  
	 int m_iROICount;	//实际计算的ROI个数
	 int m_iSFRROICount;	//用于统计的ROI个数

	 OfMarker m_stROIInfo[300];
	 int m_iBlockCount;	//大框的个数

	 BOOL m_bROIConfigureError;
	 SpecInfo m_stSpec[MAXROICOUNT];
	 float m_dSFRValue[MAXROICOUNT];	//记录当前DAC对应的各个ROI的SFR	
	 int m_dSFR40CM_Data[MAXROICOUNT];
	 int m_dSFR60CM_Data[MAXROICOUNT];
	 int m_dSFR120CM_Data[MAXROICOUNT];
	 float m_dSFRValue_Fact[MAXROICOUNT];//保存未补偿的SFR真实数据
	 float m_dSFRValue_Offset[MAXROICOUNT];//保存补偿之后的SFR数据

	 ROIPosInfo m_stROIPos[MAXROICOUNT];		//记录设定中制定的ROI的具体位置坐标
	 double SFRAll[10][MAXROICOUNT];//记录回到Peak点后测试的全部SFR值
	 int iTestTimes; //回到Peak点后测试SFR的次数	
	 float m_fCTC8;
	 float m_fCTC6;
	 float m_fCTC8_H;
	 float m_fCTC8_V;
	 float m_fCTC6_H;
	 float m_fCTC6_V;

	 float m_fCTC8_Fact;
	 float m_fCTC6_Fact;
	 float m_fCTC8_H_Fact;
	 float m_fCTC8_V_Fact;
	 float m_fCTC6_H_Fact;
	 float m_fCTC6_V_Fact;

	 ImageInfo img;
	 bool m_bGotAllROI;

	 OFAutoROIOut m_ofDestDataOut;
	 BOOL m_bFinalCheck;
	 BOOL m_bFocusing;
	 BOOL fqcFirstTime;
	 int m_iFinalAutoROITime;
	 int m_iFinalSFRTest;
	 CString m_errMsg;
	 int nTestTime;
public:
	CString m_cfgName;
	BOOL bSaveBmpFile(CString sfilename, BYTE *pBuffer, UINT width, UINT height);
	CString GetModulePath();

private:
	BOOL SFRSpecLimit();
	void SaveData_InFact(bool bResult);//保存实际值
	void SaveSFRData_Offset(bool bResult);//保存补偿之后的数据
	void Convert2Char(char* cString,int iMaxLength,CString strSting);
	void Save40CMSFR();
	void Save60CMSFR();
	void Save120CMSFR();
	void GetSFR40CM();
	void GetSFR60CM();
	void GetSFR120CM();
	bool bResult;
};