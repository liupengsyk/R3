#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "ofWhiteSpot.h"


class DParticle_TestItem :public TestItemInterface
{
public:
	DParticle_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~DParticle_TestItem(void);

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
	CString CamCode;
	int interval;
	WDefectStack  WhiteSpotResult;
	ImageInfo img;

public:
	CString GetModulePath();
	void ShowInfo();
	CString m_cfgName;

	USHORT preGain;
};