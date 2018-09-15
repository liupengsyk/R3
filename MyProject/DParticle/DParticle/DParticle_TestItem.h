#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "ofWhiteSpot.h"


class DParticle_TestItem :public TestItemInterface
{
public:
	DParticle_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~DParticle_TestItem(void);

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
	int interval;
	WDefectStack  WhiteSpotResult;
	ImageInfo img;

public:
	CString GetModulePath();
	void ShowInfo();
	CString m_cfgName;

	USHORT preGain;
};