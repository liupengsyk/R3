#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class OC_Item_TestItem :public TestItemInterface
{
public:
	OC_Item_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~OC_Item_TestItem(void);

	int InitItem();

	int Testing();       //子类重载测试代码放放在此函数
	int Initialize();      //子类重载 初始化代码
	int Finalize();      //子类重载 初始化

	int LoadOption();
	int SaveOption();

	BOOL OCTest();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit() ;
	LPCTSTR GetReportContents() ;

private:
	OptionDlg* pDlg;
	 int CamID;

public:
	int OCSpecX_hig;
	int OCSpecY_hig;
	int CenterX;
	int CenterY;
};