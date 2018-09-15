#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "ReportData.h" 

const int MAX_SAMPLE_NUM = 30;

class CaliOk_TestItem :public TestItemInterface
{
public:
	CaliOk_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~CaliOk_TestItem(void);

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
    CString m_strStdPath;
    CReportData m_stdData;
    int m_nCaliOK;
    int m_nFuseIDNum;
    CString m_strFuseIDArry[MAX_SAMPLE_NUM];

public:
	// Function:  read regedit bCali flag
	// Returns:   BOOL, true: current is cali mode; false: current is normal mode
	BOOL ReadBCaliMode();

	// Function:  read regedit bCali flag
	// Returns:   BOOL, true: current is cali mode; false: current is normal mode
	BOOL IsCaliOver();

	// Function:  write current time to regedit
	void RecordTime();

	// Function:  write current module to regedit
	void ChangeModule();

	// Function:  write next test is calibration mode or normal mode to regedit
	// Parameter: BOOL, bCaliMode: true: calibration mode; false: normal mode 
	void WriteBCaliMode(BOOL bCaliMode);

	// Function:  if all station calibrate ok, reset every station ok flag
	void ResetStationOK();

//     BOOL LoadData();
//     BOOL IsSample();    
};