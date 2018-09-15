#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"


class RX_OTPCheck_TestItem :public TestItemInterface
{
public:
	RX_OTPCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~RX_OTPCheck_TestItem(void);

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
	bool GT24C32A_Read(USHORT address,byte *Data,int size);
	CString GetModulePath();
	CString m_cfgName;
private:
	OptionDlg* pDlg;
	int CamID;
	int x;

public:
	int OtpCheckResult;
	int m_OTPSize;
    bool SaveEEPROM(BYTE *EEPROMData,CString filename);
};