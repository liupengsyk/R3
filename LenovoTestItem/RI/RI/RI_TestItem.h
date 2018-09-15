#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
//#include "OfRITest.h"

#define RI_DEBUG														0
#if RI_DEBUG == 1
	#define RI_TEST_BY_READ_RIPIC_BIN										0
	#define RI_TEST_OUTPUT_INFO												1
#endif

#define RI_TST_PIC_NUM								1	//RI������Ҫ�Ѽ���ͼƬ����

#define BLC_RAW8									16
#define BLC_RAW10									64

class RI_TestItem :public TestItemInterface
{
public:
	RI_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~RI_TestItem(void);

	int InitItem();

	int Testing();       //�������ز��Դ���ŷ��ڴ˺���
	int Initialize();      //�������� ��ʼ������
	int Finalize();      //�������� ��ʼ��

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit();
	LPCTSTR GetReportContents();

#if RI_TEST_OUTPUT_INFO == 1
	void OutputDebugInfo(BYTE InfoType, int i);
#endif
#if RI_TEST_BY_READ_RUPIC_BIN == 1
	void RiTstByReadRiPicBin(struct RIValue &pRIValue);
#endif

private:
	//void ShowInfo();
	OptionDlg* pDlg;
	int CamID;

public:
	ImageInfo img; 
	CString GetModulePath();
	CString m_cfgName;
	//DefectStack m_podDefectStack;
	//int m_PixelOrder;
	BYTE m_CurPicInx;
	LPWORD m_pBuf[RI_TST_PIC_NUM];
};