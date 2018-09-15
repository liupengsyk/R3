#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class MesCheck_TestItem :public TestItemInterface
{
public:
	MesCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~MesCheck_TestItem(void);

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
	void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);
	BOOL GetRunCardAndBarcode(CString &RunCard,CString &Barcode);

	
private:
	OptionDlg* pDlg;
	 int CamID;

public:
	BOOL bCam0Mes;
	BOOL bCam1Mes;
	int Cam0MesID;
	int Cam1MesID;
	BOOL bUseBarcode;
	int iBarcodeLength;
	CString BarcodeKeyString;
	BOOL bMesDelete;
	BOOL bMesCheck;
	BOOL bMesBinding;
};