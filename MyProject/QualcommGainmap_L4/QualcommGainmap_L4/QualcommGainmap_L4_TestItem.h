#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
#include "PDAFCalibrationTools_Dll.h"


class QualcommGainmap_L4_TestItem :public TestItemInterface
{
public:
	QualcommGainmap_L4_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~QualcommGainmap_L4_TestItem(void);

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
	int LeftGainMin;
	int LeftGainMax;
	int RightGainMin;
	int RightGainMax;


	int Read_raw_file_2pd(CString filename, uint16_t * img_l, uint16_t * img_r, uint16_t width, uint16_t height, cfa_t cfa);
	void SaveGainMapData(gainmap_t gainmap);
};