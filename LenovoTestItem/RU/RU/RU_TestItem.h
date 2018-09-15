#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
//#include "OfRUTest.h"

#define RU_DEBUG														0
#if RU_DEBUG == 1
	#define RU_TEST_BY_READ_RUPIC_BIN										0
	#define RU_TEST_OUTPUT_INFO												1
#endif

#define RU_TST_PIC_NUM								5	//RU������Ҫ�Ѽ���ͼƬ����

#define BLC_RAW8									16
#define BLC_RAW10									64


class RU_TestItem :public TestItemInterface
{
public:
	RU_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~RU_TestItem(void);

	int InitItem();

	int Testing();       //�������ز��Դ���ŷ��ڴ˺���
	int Initialize();      //�������� ��ʼ������
	int Finalize();      //�������� ��ʼ��

	int LoadOption();
	int SaveOption();

	BOOL m_RUResult;
	BOOL m_ShadingResult;

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit();
	LPCTSTR GetReportContents();

#if RU_TEST_OUTPUT_INFO == 1
	void OutputDebugInfo(BYTE InfoType, int i);
#endif
#if RU_TEST_BY_READ_RUPIC_BIN == 1
	void RuTstByReadRuPicBin(struct RUValue &pRUValue);
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
	LPWORD m_pBuf[RU_TST_PIC_NUM];
};