#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
  
class AE_TestItem :public TestItemInterface
{
public:
	AE_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~AE_TestItem(void);

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
	typedef struct _LightSourcePro
	{
		int Channel; //2856K
		int RGainHigh;   // ���ڹ�Դ�л��Ƿ�ɹ��ж�
		int RGainLow;
		int BGainHigh;
		int BGainLow;
	}LightSourcePro;

	typedef struct _LEDLightSource
	{
		LightSourcePro LAMP_CCT_D65; //2856K
		LightSourcePro LAMP_CCT_TL84 ; //5000K
		LightSourcePro LAMP_CCT_A;

	}LEDLightSource;

	typedef enum{
		LAMP_CCT_A = 0, //2856K
		LAMP_CCT_TL84, //4000K
		LAMP_CCT_D65, //6500K
		LAMP_CCT_MAX_NUM
	}HiStarOTP_LampType;
	LEDLightSource m_LightSource;

	BOOL ChangeLEDLSChannel(int LEDType,HiStarOTP_LampType lamptype);
	BOOL SetLEDFlag;
private:
	OptionDlg* pDlg;
	 int CamID;
	 int Count;
	 int interval;
	 float brightness; 
	 int exposure;
	 
public:
	void CenterBlockRawInfo(USHORT *pIn,ImageInfo *mySensor ,int Ratio,int Ratio2,float &RawG,BOOL WhiteEnable=FALSE);
	void CenterBlockRawInfo4Cell(USHORT *pIn,ImageInfo *mySensor ,int Ratio,int Ratio2,float &RawG,BOOL WhiteEnable=FALSE);

	void ShowROIInfo();
	CString m_cfgName;

	int intervalSpec;
	int AddressH;
	int AddressL;

	int GainVal1;
	int GainVal2;
	int GainReg1;
	int GainReg2;
	int mode;
	int SlaveID;
	int mode_Gain;

	CString  GlobalSetting;//���Դ��������
	CString CurrentName;//���Դ��������
	int GlobalTime;//���Դ��������

	void CenterBlockRawInfo2PD(USHORT * pIn, ImageInfo * mySensor, int Ratio, int Ratio2, float & RawG, bool WhiteEnable);
};