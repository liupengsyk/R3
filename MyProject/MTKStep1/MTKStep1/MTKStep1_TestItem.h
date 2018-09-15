#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class MTKStep1_TestItem :public TestItemInterface
{
public:
	MTKStep1_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~MTKStep1_TestItem(void);

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
	BOOL SaveStep1Bin(BYTE *GainBin);

private:
	OptionDlg* pDlg;
	int CamID;
	CString CamCode;

public:


};