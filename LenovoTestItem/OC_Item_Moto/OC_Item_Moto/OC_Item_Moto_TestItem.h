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
//#if OC_TEST_BY_YVALUE_DEFINE == 1
	double YVal_OpticalOut;

	OFOPTCenterPixInfo YVal_OpticalPixel;
//#else
	double R_OpticalOut;
	double Gr_OpticalOut;
	double Gb_OpticalOut;
	double B_OpticalOut;

	OFOPTCenterPixInfo R_OpticalPixel;
	OFOPTCenterPixInfo Gr_OpticalPixel;
	OFOPTCenterPixInfo Gb_OpticalPixel;
	OFOPTCenterPixInfo B_OpticalPixel;
//#endif
};

#define YVALUE(r,g,b) (0.299*double(r)+0.587*double(g)+0.114*double(b))

/// RAWͼ��4�������ʽ���塣
typedef enum
{
	OUTFORMAT_RGGB = 0,///<RGGB�����ʽ
	OUTFORMAT_GRBG,///<GRBG�����ʽ
	OUTFORMAT_GBRG,///<GBRG�����ʽ
	OUTFORMAT_BGGR,///<BGGR�����ʽ
}_OUTFORMAT_RGB_;

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
	BOOL SaveAWBData(void);	//����İ��ӣ��ܶ���Ҫ��OTP����¼OC���ݣ��ο�AWB����ͨ���ļ�����OTP��¼������˴�Ҳ��ͨ���ļ�����OTP��¼��--Added by LiHai--20171213

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
//#if OC_TEST_BY_YVALUE_DEFINE == 0
	BOOL OpticalCenterTest(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut * _OFOPTCenterOut);
//#else
	BOOL OpticalCenterTest_Y(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut * _OFOPTCenterOut, BYTE *pRGB24);
//#endif
	BOOL OpticalCenterTest_Mono(OFOPTCenterIn * _OFOPTCenterIn, OFOPTCenterOut_Mono * _OFOPTCenterOut);
#if OC_TEST_SAVE_PIC_DEFINE == 1
	void SaveRaw(CString PathName, OFOPTCenterPixInfo *pCh, int nLen);
#endif
	void getOCCoord(OFOPTCenterPixInfo* pPanel,OFOPTCenterPixInfo *CenterPixInfo,int ImageWidth,int ImageHeight);
	void getOptCenter(OFOPTCenterPixInfo* pPanel,OFOPTCenterPixInfo *CenterPixInfo,int ImageWidth,int ImageHeight,double *Distance,SensorFormat ID);
};