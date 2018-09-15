#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class OPCurrentTest_TestItem :public TestItemInterface
{
public:
	OPCurrentTest_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~OPCurrentTest_TestItem(void);

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
	float fOPCurrent[5];

private:
	OptionDlg* pDlg;
	int CamID;
	CString CamCode;

public:

	BYTE m_cVoltageAVDD;
	BYTE m_cVoltageDOVDD;
	BYTE m_cVoltageDVDD;
	float m_fPower;
};