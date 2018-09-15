#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"

class OTPBurn_TestItem :public TestItemInterface
{
public:
	OTPBurn_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	OTPBurn_TestItem(CString fileName);
	~OTPBurn_TestItem(void);

	int InitItem();

	int Testing();			//子类重载测试代码放放在此函数
	int Initialize();		//子类重载 初始化代码
	int Finalize();			//子类重载 初始化

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
	UINT SlaveID;
	CString CurfilePath;

	int ReadEEpromAllData();
};