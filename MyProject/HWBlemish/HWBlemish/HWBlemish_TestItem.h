#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
// #include "HWBlemishTest.h"
// #pragma comment(lib,"HWBlemishTest.lib")
 


class HWBlemish_TestItem :public TestItemInterface
{
public:
	HWBlemish_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~HWBlemish_TestItem(void);

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
	CString m_cfgName;
	ImageInfo img;
	int FrameCount;
	BOOL BlemishRes;
	void ShowJPEGFromFile(CString Filename);
	BOOL testHwBlemish();
};