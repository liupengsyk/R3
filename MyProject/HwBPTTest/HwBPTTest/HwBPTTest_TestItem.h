#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include <direct.h>
 
typedef struct _AFCode
{
	USHORT Up_Inf;
	USHORT Normal;
	USHORT Up_Macro;
	USHORT Hor_Inf;
	USHORT Hor_Macro;
	USHORT Down_Inf;
	USHORT Down_Macro;
}AFCode;

enum SENSOR
{
	IMX_SENSOR = 0,
	OV_SENSOR
};

class HwBPTTest_TestItem :public TestItemInterface
{
public:
	HwBPTTest_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~HwBPTTest_TestItem(void);

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

	int Sum();	
	CString GetSensorFuseID();
	BOOL b12m;
	BOOL bThreePosion;

	int CheckAF();
	void SaveOTPdata(USHORT*m_OTPdata);
	void WideToChar(CString wide,char* Char);
private:
	OptionDlg* pDlg;
	 
	 CString FuseID;

public:
	int CamID;
	int m_errCode;
	int m_frmDelayNum;
	int m_StartTiem;
	int m_EndTime;
	CString m_tstMode;
	int m_frmDelayCnt;

	int m_initExpTime;
	USHORT m_initExpGain;
	CString m_szName;
	CRITICAL_SECTION	m_csLock;
	int m_badPixelCount;
	void RAW10_To_BYTE(USHORT *Pin,BYTE *Pout,int ImageWidth,int ImageHeight);
	void Raw10toRaw8(USHORT *Pin,BYTE *Raw8out,int ImageWidth,int ImageHeight);
};