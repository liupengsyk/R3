#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "ofSFR.h"

class AF_TestItem :public TestItemInterface
{
public:
	AF_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~AF_TestItem(void);

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
	OptionDlg* pDlg;
	int CamID;
	CString CamCode;

public:
	int m_afErrCode;
	int m_afFrmDelayNum;

	CString m_afMode;
	int m_afFrmDelayCnt;

	int m_afStartCode;
	int m_afEndCode;
	int m_afMiniStartCode;
	int m_afMiniEndCode;
	int m_afStepCode;
	int m_afStepCnt;
	int m_afTargetCode;
	int m_afBestCode;
	int m_afFinalOtpCode;
	float m_afBestAv;

	int m_afPosiCnt;

	AF_STEP_DATA m_afStepData;

	CPoint m_sortRoi[20];
	SFR_ROI_VALUE m_sfrRoiValue[21];

	int m_afAeOldExpT;
	int m_afAeExpNum;
	int m_afAeExpCnt;
	int m_afAeTarget;
	int m_afAeRange;
	float m_aeBrightness;
	float m_aeFactor;

	int m_errCnt;
	int m_finalEndCnt;
	int m_finalChkCnt;
	vector<float> m_mtfValBuff[16];
	vector<float> m_finalAvBuff[128];
public :
	float GetRoiAfValue(CString algorithm);
	void ShowRoiInformation(MTF_PARA mtfPara);
	void ShowSfrTestROI(int roiNum, CString type, int offsetX, int offsetY, int roiWidth, int roiHeight, CPoint* sortRoi, SFR_ROI_VALUE* sfrRoiValue);
	void ShowSfrTestMotoChartROI(int roiNum, int offsetX, int offsetY, int roiWidth, int roiHeight, CPoint roiCnPosi, float sfrVal);
	void ShowAllTargetNumber(CPoint* sortROI);
	void ShowAeRoiInfo(CRect* rc);
	void ShowMtfRoiInfo(MTF_PARA mtfPara);
	void AppendAfBurnFile(CString strAlgo, int cnt, int code, PSFR_ROI_VALUE roiVal);
	void AppendAfBurnFileForFinalCheck(CString strAlgo);
	float AE_GetCenterRoiBrightness(BYTE* pRaw8Buff, ImageInfo* pImgInfo, AE_PARA aePara, CRect* rc);
	void SaveMtfPara();
	void GetMtfPara();
	void SaveSquareChartSfrPara();
	void GetSquareChartSfrPara();
	void ShowSquareChartSfrInfoROI(int roiNum, CPoint tagPosi, int* roiType, CPoint* roiCnPosi, CSize roiSize, float* sfrValue, SQUARE_CHART_SFR_PARA sfrPara);
	void ShowXChartSfrInfoROI(int roiNum, CPoint tagPosi, CPoint* roiCnPosi, CSize roiSize, float* sfrValue, X_CHART_SFR_PARA sfrPara);
	float GetRoiAfValueForFinalCheck(CString algorithm);
	CStdioFile m_fileLogMsg;
	void SaveXChartSfrPara();
	void GetXChartSfrPara();
	float MedielFilterFor3(float a, float b, float c);
	void SaveMotoChartSfrPara();
	void GetMotoChartSfrPara();
	BOOL OV20880_Remosaic_FullSize_Raw10_DPC(USHORT *MIPI_Raw10BufferIn, USHORT *Raw10BufferOut);
	int ReadReg(int id, int addr);
	int OV20880_ReadOTPDPCData(int *OTP_DPC_Data);
};