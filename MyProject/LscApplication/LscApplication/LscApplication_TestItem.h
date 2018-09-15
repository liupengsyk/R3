#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class LscApplication_TestItem :public TestItemInterface
{
public:
	LscApplication_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~LscApplication_TestItem(void);

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

public:
	CString FilePath;
	CString FileName;
	CString TempMsg;
	BYTE  m_LSCData[1868];
	BYTE  LSCBurningData[1869];
	BOOL LSCDataCheck(USHORT *m_LSCData);
    void SaveColorShadingData(float * Color_RG , float * Color_BG , BOOL Result);
	void MTKLSC_Paramter(CString Filepath);
	BOOL MTK_LSCCal();
	BOOL MTKMONO_LSCCal();
	BOOL Qualcomm_LSCCal();
	BOOL Sensor_LSCCal();
	BOOL ReadDataFromTXT(float *R_Channel, float *Gr_Channel, float *Gb_channel, float *B_Channel);
	BOOL ReadBinData(USHORT *OTPData);
	vector <BYTE> BurnData;
	vector<float> vtYDecay;

	int LSC_R_Max;
	int LSC_Gr_Max;
	int LSC_Gb_Max;
	int LSC_B_Max;
	int LSC_R_Min;
	int LSC_Gr_Min;
	int LSC_Gb_Min;
	int LSC_B_Min;
	int LSC_Distance;//规格

	int LSCData_R_Max;
	int LSCData_Gr_Max;
	int LSCData_Gb_Max;
	int LSCData_B_Max;
	int LSCData_R_Min;
	int LSCData_Gr_Min;
	int LSCData_Gb_Min;
	int LSCData_B_Min;
	int tempMax;
	CString CurrentName;
	CString GlobalSetting;
	void SaveQualLSCData(int LSCData[][4]);
};