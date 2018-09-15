#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 
enum SensorFormat
{
	BAYER = 0,
	MONO,
};

struct OFOPTCenterPixInfo
{
	int xIndex;
	int yIndex;
	int Y_Value;
};

struct OFOPTCenterIn
{
	USHORT *ImageBufferIn;
	int  Width;
	int  Height;
	BYTE Outformat;
	double pixelSize;
};

struct OFOPTCenterOut
{
	double R_OpticalOut;
	double Gr_OpticalOut;
	double Gb_OpticalOut;
	double B_OpticalOut;

	OFOPTCenterPixInfo R_OpticalPixel;
	OFOPTCenterPixInfo Gr_OpticalPixel;
	OFOPTCenterPixInfo Gb_OpticalPixel;
	OFOPTCenterPixInfo B_OpticalPixel;
};

struct OFOPTCenterOut_Mono
{
	double OC_Out;
	OFOPTCenterPixInfo OpticalPixel;
};

class OC_Item_Moto_TestItem :public TestItemInterface
{
public:
	OC_Item_Moto_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~OC_Item_Moto_TestItem(void);

	int InitItem();

	int Testing();       //�������ز��Դ���ŷ��ڴ˺���
	int Initialize();      //�������� ��ʼ������
	int Finalize();      //�������� ��ʼ��

	int LoadOption();
	int SaveOption();

	BOOL OCTest();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit() ;
	LPCTSTR GetReportContents() ;

private:
	OptionDlg* pDlg;
	 int CamID;

public:
 	float CenterX;
 	float CenterY;
	OFOPTCenterOut_Mono CenterOut_Mono;
	OFOPTCenterOut CenterOut;
	BOOL OpticalCenterTest(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut * _OFOPTCenterOut);
	BOOL OpticalCenterTest_Mono(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut_Mono * _OFOPTCenterOut);
};