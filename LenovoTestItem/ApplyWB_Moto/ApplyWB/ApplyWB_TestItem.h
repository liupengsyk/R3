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


	int Golden_R;
	int Golden_Gr;
	int Golden_Gb;
	int Golden_B;

	int Golden_RG;
	int Golden_BG;
	int Golden_GG;

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

	USHORT RGain;	//WB数据的R/G的值
	USHORT BGain;	//WB数据的B/G的值
	USHORT GGain;	//WB数据的Gb/Gr的值

	/** 注意高通工具出来的原始各通道平均值是减去了BLC的值   , 以下四个值是加回了BLC数据的, 所以是图片原始的各通道平均值     **/
	USHORT Unit_R;	//高通工具计算出来的原始图片的R分量平均值, 也后面计算WB数据R/G里面的被除数R值
	USHORT Unit_Gr;	//高通工具计算出来的原始图片的Gr分量平均值, 也后面计算WB数据R/G里面的被除数G值
	USHORT Unit_Gb;	//高通工具计算出来的原始图片的Gb分量平均值, 也后面计算WB数据R/G里面的被除数G值
	USHORT Unit_B;	//高通工具计算出来的原始图片的B分量平均值, 也后面计算WB数据B/G里面的被除数B值

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
	int GAverFlag;//1:G_Aver 2:Gr_Aver 3:Gb_Aver--就是在算WB数据R/G、B/G时, 除数G到底是(Gr + Gb)/2还是Gr或者Gb

	float Beforedistance;
	float Afterdistance;
	CString m_strTestStation;

	int m_MotoLightSrcMode;

	BOOL TestResult;
	BOOL ApplyAWBFlag;
	void GetAWBInfoRaw8(ImageInfo img);
	void GetAWBInfoRaw10(ImageInfo img);
	int CheckCount;
	BOOL CheckAWBRange();
	BOOL CheckGoldenRange();
	BOOL SaveOTPData();
	BOOL GetAWBData(ImageInfo img);
	void Raw10toRaw8(USHORT *Src, BYTE *Dest, long number);
	BOOL SaveAWBData(BOOL ApplyAWBFlag);
};