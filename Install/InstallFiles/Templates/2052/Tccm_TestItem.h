#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class [!output PROJECT_NAME]_TestItem :public TestItemInterface
{
public:
	[!output PROJECT_NAME]_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~[!output PROJECT_NAME]_TestItem(void);

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
	CString CamCode;

public:


};