#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"

class SetLEDChannel_TestItem :public TestItemInterface
{
public:
	SetLEDChannel_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~SetLEDChannel_TestItem(void);

	int InitItem();

	int Testing();       //子类重载测试代码放放在此函数
	int Initialize();      //子类重载 初始化代码
	int Finalize();      //子类重载 初始化

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit() ;
	LPCTSTR GetReportContents() ;

	struct WBInf
	{
		USHORT R;
		USHORT Gr;
		USHORT Gb;
		USHORT B;
		USHORT RG;
		USHORT BG;
		USHORT GG;
	};
	void CenterBlockRawInfo(ImageInfo *mySensor ,int Ratio,WBInf &MyWbInf,int index,int BLC);

	typedef struct _LightSourcePro
	{
		int Channel; //2856K
		int RGainHigh;   // 用于光源切换是否成功判断
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
	BOOL ImageVerify(HiStarOTP_LampType lamptype);



private:
	OptionDlg* pDlg;
	int CamID;
	CString CamCode;
	int imageflag;

public:
	BOOL LEDInitFlag;
	int GlobalTime;//与光源矫正联用
	CString CurrentName;//与光源矫正联用
};