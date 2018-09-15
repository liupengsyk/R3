#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "OfBlemishTest.h"


class POG_TestItem :public TestItemInterface
{
public:
	POG_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~POG_TestItem(void);

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

public:
	ImageInfo img; 
	CString GetModulePath();
	CString m_cfgName;
	void ShowInfo(BlemishInfo blemishInfo);
	BlemishInfo m_blemishInfo;
};