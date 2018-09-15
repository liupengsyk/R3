#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "OfPODTest.h"


class POD_TestItem :public TestItemInterface
{
public:
	POD_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~POD_TestItem(void);

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
	void ShowInfo();
	OptionDlg* pDlg;
	 int CamID;

public:
	ImageInfo img; 
	CString GetModulePath();
	CString m_cfgName;
	DefectStack m_podDefectStack;
	int m_PixelOrder;

};