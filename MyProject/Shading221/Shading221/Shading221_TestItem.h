#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class Shading221_TestItem :public TestItemInterface
{
public:
	Shading221_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~Shading221_TestItem(void);

	int InitItem();

	int Testing();       //×ÓÀàÖØÔØ²âÊÔ´úÂë·Å·ÅÔÚ´Ëº¯Êý
	int Initialize();      //×ÓÀàÖØÔØ ³õÊ¼»¯´úÂë
	int Finalize();      //×ÓÀàÖØÔØ ³õÊ¼»¯

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
	BOOL Qualcomm_LSCCal(void);
	BOOL ReadBinData(USHORT * LSC_Ratio);
	BOOL ColorShading_Test();
	void SaveColorShadingData(float * Color_RG , float * Color_BG , BOOL Result);
	BOOL Y_Shading_Test(void);
	BOOL ReadDataFromTXT(float * R_Channel, float * Gr_Channel, float * Gb_Channel, float * B_Channel);
	void SaveYShadingData(float * Y_Shading_Data,BOOL Result);
	float RGRef_Max;
	float RGRef_Min;
	float BGRef_Max;
	float BGRef_Min;
	float Y_Shading_Corner[4];
	float Y_Shading_Diff;
	CString SensorID ;
	CString ReportPath;
};