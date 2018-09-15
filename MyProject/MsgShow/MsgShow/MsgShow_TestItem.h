#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class MsgShow_TestItem :public TestItemInterface
{
public:
	MsgShow_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~MsgShow_TestItem(void);

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
	int ShowMTFinfo();
	CString InfoShow;
	CPoint InfoPos;
	int InfoSize;
	CString InfoColor;
	BOOL bShowInfo;

	int DelayTime;	
	BOOL bDelay; 

	BOOL bShowMsgDlg;
	CString MsgInfo;


};