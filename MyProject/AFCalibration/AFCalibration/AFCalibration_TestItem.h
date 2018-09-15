#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"

#include "ReportData.h"

class AFCalibration_TestItem :public TestItemInterface
{
public:
	AFCalibration_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~AFCalibration_TestItem(void);

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

    int m_nFuseIDNum;
	CString m_strCurFuseID;
    CString m_strFuseIDArry[MAX_SAMPLE_NUM];

    CString m_strStdPath;
    CString m_strTestPath;
    CReportData m_stdData;
    CReportData m_testData;


public:
	// Function:  read regedit bCali flag
	// Returns:   BOOL, true: current is cali mode; false: current is normal mode
	BOOL ReadBCaliMode();
    
	// Function:  read module and last calibration time  from regedit, and check if need calibrate
	// Returns:   BOOL, true: time out or module mismatching; false: no need cali 
	BOOL IsNeedCali();
	
	// Function:  write next test is calibration mode or normal mode to regedit
	// Parameter: BOOL, bCaliMode: true: calibration mode; false: normal mode 
	void WriteBCaliMode(BOOL bCaliMode);

	// Function:  record current station if calibrate ok
	// Parameter: BOOL, state: true: calibrate ok; false: calibrate fali
	void SetStationState(BOOL state);

	// Function: load all samples data, fuseID and current test data from .csv
	BOOL LoadData(CString &errMsg);	

	// Function: check current fuseID if in standard .csv
	BOOL IsSample(CString &errMsg);

	// Function: contrast the last test data and standard data difference if in specification 
    BOOL CompData(CString &errMsg);

	BOOL Calibration(CString &errMsg);
};