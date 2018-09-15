#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class SRNTest_TestItem :public TestItemInterface
{
public:
	SRNTest_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~SRNTest_TestItem(void);

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

	BOOL SetProperGain(void);
	void Raw10toP10(USHORT* pIn, BYTE* pOut, int Height, int Width);
/*	BOOL SRNtest(void);*/

private:
	OptionDlg* pDlg;
	int CamID;
   CString CamCode;

public:
	 ImageInfo m_img;
	 int m_frmDelayCnt;
	 CString m_tstMode;
	 int m_captureCnt;

	 int m_initExpoTime;
	 USHORT m_initGain;

	 BYTE * m_pRawBuff0;
	 BYTE * m_pRawBuff1;
	 BYTE * m_pRawBuff2;
	 BYTE * m_pRawBuff3;

	 int m_errCode;
	 int m_frmDelayNum;

	 float m_PPmax;
};