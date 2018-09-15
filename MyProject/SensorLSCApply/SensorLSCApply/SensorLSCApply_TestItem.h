#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class SensorLSCApply_TestItem :public TestItemInterface
{
public:
	SensorLSCApply_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~SensorLSCApply_TestItem(void);

	int InitItem();

	int Testing();       //�������ز��Դ���ŷ��ڴ˺���
	int Initialize();      //�������� ��ʼ������
	int Finalize();      //�������� ��ʼ��

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
	CString SensorName;
	int GlobalTime;
};