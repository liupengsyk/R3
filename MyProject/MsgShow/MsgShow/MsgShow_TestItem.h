#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class MsgShow_TestItem :public TestItemInterface
{
public:
	MsgShow_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~MsgShow_TestItem(void);

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
	int ShowMTFinfo();
	CString InfoShow;
	CPoint InfoPos;
	int InfoSize;
	CString InfoColor;
	BOOL bShowInfo;

	int DelayTime;	
	BOOL bDelay; 

	BOOL bShowMsgDlg;
	CString MsgInfo;


};