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

	int Testing();			//�������ز��Դ���ŷ��ڴ˺���
	int Initialize();		//�������� ��ʼ������
	int Finalize();			//�������� ��ʼ��

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