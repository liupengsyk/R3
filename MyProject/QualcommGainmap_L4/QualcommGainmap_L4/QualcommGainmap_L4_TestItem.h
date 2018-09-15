#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "PDAFCalibrationTools_Dll.h"


class QualcommGainmap_L4_TestItem :public TestItemInterface
{
public:
	QualcommGainmap_L4_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~QualcommGainmap_L4_TestItem(void);

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

public:
	int LeftGainMin;
	int LeftGainMax;
	int RightGainMin;
	int RightGainMax;


	int Read_raw_file_2pd(CString filename, uint16_t * img_l, uint16_t * img_r, uint16_t width, uint16_t height, cfa_t cfa);
	void SaveGainMapData(gainmap_t gainmap);
};