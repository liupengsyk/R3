#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class FPN_TestItem :public TestItemInterface
{
public:
	FPN_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~FPN_TestItem(void);

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
	ImageInfo img;

public:
	int interval;
	double RowMeanMax;
	double ColMeanMax;

	double RowPixelPercent;
	double ColPixelPercent;

	double RowDiffPercent;
	double ColDiffPercent;

	CString m_cfgName;
	CString GetModulePath();
	BOOL bSaveBmpFile(CString sfilename, BYTE *pBuffer, UINT width, UINT height);
	void bSaveRawFile(CString sfilename, USHORT *pBuffer, UINT width, UINT height);

	USHORT preGain;

	BOOL FixedPatternNoiseTest(USHORT * _RawImgaeBuffer,int width,int height);
};