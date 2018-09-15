#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
  
class RedSpot_TestItem :public TestItemInterface
{
public:
	RedSpot_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~RedSpot_TestItem(void);

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
	BYTE  m_cSensorSlaveId;
	BYTE  m_cSensorMode;

public:

	BOOL ManualExposure();
	void nDrawBlockRect(CDC *pDC, 
		CPoint StartPos,
		CSize ImgSize,
		CSize BlockSize,
		CSize ViewWNDSize,
		CString Text,
		int ntestResult ,BOOL textUpDown = FALSE);
	void  nDrawRectLine(CDC *pDC, int x, int y, UINT width, UINT height);
	int m_iFrmDelayCnt;
};