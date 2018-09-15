#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 
#define BLK_NUM_H											17
#define BLK_NUM_V											13
#define BLK_CENTER_INX										(BLK_NUM_H * BLK_NUM_V / 2)

class LscApplication_Moto_TestItem :public TestItemInterface
{
public:
	LscApplication_Moto_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~LscApplication_Moto_TestItem(void);

	int InitItem();

	int Testing();       //�������ز��Դ���ŷ��ڴ˺���
	int Initialize();      //�������� ��ʼ������
	int Finalize();      //�������� ��ʼ��

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

	CString m_strTestStation;
	void SaveColorShadingData(float *R_Channel, float *Gr_Channel, float *Gb_channel, float *B_Channel);
	void SaveLSCData(USHORT *OTPData);

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
	int LSC_Distance;//���

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
};