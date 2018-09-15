#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"

typedef struct _AWBSpec
{

	int RIndex;
	int GrIndex;
	int GbIndex;
	int BIndex;

	int RGIndex;
	int BGIndex;
	int GGIndex;


	int Typical_R;
	int Typical_Gr;
	int Typical_Gb;
	int Typical_B;

	int Typical_RG;
	int Typical_BG;
	int Typical_GG;

	int RGainHigh;
	int RGainlow;
	int BGainHigh;
	int BGainlow;
	int GGainHigh;
	int GGainlow;

	int RHigh;
	int RLow;
	int GrHigh;
	int GrLow;
	int GbHigh;
	int GbLow;
	int BHigh;
	int BLow;

}AWBSpec;

typedef struct _AWBInfo
{

	USHORT RGain;
	USHORT BGain;
	USHORT GGain;

	USHORT Unit_R;
	USHORT Unit_Gr;
	USHORT Unit_Gb;
	USHORT Unit_B;

	USHORT RGain_fin;
	USHORT BGain_fin;
	USHORT GGain_fin;

	USHORT Unit_R_fin;
	USHORT Unit_Gr_fin;
	USHORT Unit_Gb_fin;
	USHORT Unit_B_fin;

}AWBInfo;
enum OUTFORMAT_RGB
{
	OUTFORMAT_RGGB = 0,///<RGGB输出格式
	OUTFORMAT_GRBG,///<GRBG输出格式
	OUTFORMAT_GBRG,///<GBRG输出格式
	OUTFORMAT_BGGR,///<BGGR输出格式
};
enum AWBTestResult
{
	TEST_PASS = 0,///PASS
	TEST_FAIL,///FAIL
};
class ApplyWB_TestItem :public TestItemInterface
{
public:
	ApplyWB_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~ApplyWB_TestItem(void);

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
	AWBSpec m_AWBSpec;
	AWBInfo m_AWBInfo;
	CString TempMsg;
	CString FilePath;
	CString FileName;
	CRect rect;

	int OTP_R;
	int OTP_Gr;
	int OTP_Gb;
	int OTP_B;

	int OTP_RGain;
	int OTP_BGain;
	int OTP_GGain;
	int GAverFlag;//1:G_Aver 2:Gr_Aver 3:Gb_Aver

	float Beforedistance;
	float Afterdistance;

	BOOL TestResult;
	BOOL ApplyAWBFlag;
	void GetAWBInfoRaw8(ImageInfo img);
	void GetAWBInfoRaw10(ImageInfo img);
	int CheckCount;
	BOOL ApplyWB_TestItem::CheckAWBRange();
	BOOL ApplyWB_TestItem::SaveAWBData();
	BOOL GetAWBData(ImageInfo img);
};