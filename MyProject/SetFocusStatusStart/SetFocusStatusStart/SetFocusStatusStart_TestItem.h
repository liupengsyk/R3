#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"


class SetFocusStatusStart_TestItem :public TestItemInterface
{
public:
	SetFocusStatusStart_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~SetFocusStatusStart_TestItem(void);

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

};