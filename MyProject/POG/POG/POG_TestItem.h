#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "OfBlemishTest.h"


class POG_TestItem :public TestItemInterface
{
public:
	POG_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~POG_TestItem(void);

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
	ImageInfo img; 
	CString GetModulePath();
	CString m_cfgName;
	void ShowInfo(BlemishInfo blemishInfo);
	BlemishInfo m_blemishInfo;
};