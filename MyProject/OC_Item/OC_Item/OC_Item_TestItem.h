#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class OC_Item_TestItem :public TestItemInterface
{
public:
	OC_Item_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~OC_Item_TestItem(void);

	int InitItem();

	int Testing();       //�������ز��Դ���ŷ��ڴ˺���
	int Initialize();      //�������� ��ʼ������
	int Finalize();      //�������� ��ʼ��

	int LoadOption();
	int SaveOption();

	BOOL OCTest();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit() ;
	LPCTSTR GetReportContents() ;

private:
	OptionDlg* pDlg;
	 int CamID;

public:
	int OCSpecX_hig;
	int OCSpecY_hig;
	int CenterX;
	int CenterY;
};