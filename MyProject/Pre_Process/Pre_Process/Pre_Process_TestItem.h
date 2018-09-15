#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class Pre_Process_TestItem :public TestItemInterface
{
public:
	Pre_Process_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~Pre_Process_TestItem(void);

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

	//int GetTimeTag();
	int CalTimeTag;
	int ProductAdd;
	int ProductID;
	int ProductAdd1;
	int ProductID1;
	int SlaveID;
	CString FuseID[20];
	CString CurrentName;
	CString GlobalSetting;
};