#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class InitiaSensor_TestItem :public TestItemInterface
{
public:
	InitiaSensor_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~InitiaSensor_TestItem(void);

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
	void GetvecSensorlist();
	CString SettingPath;
	CString SettingPath1;

private:
	OptionDlg* pDlg;
	int CamID;
	CString CamCode;

public:


};