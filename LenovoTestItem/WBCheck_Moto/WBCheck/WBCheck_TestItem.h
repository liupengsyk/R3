#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 


class WBCheck_TestItem :public TestItemInterface
{
public:
	WBCheck_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~WBCheck_TestItem(void);

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

private:
	OptionDlg* pDlg;
	int CamID;
	CString CamCode;

public:
	int AfterRG;
	int AfterBG;
	int AfterGG;
	int AfterR;
	int AfterGr;
	int AfterGb;
	int AfterB;
	int AfterRG_fin;
	int AfterBG_fin;
	int AfterGG_fin;
	int AfterR_fin;
	int AfterGr_fin;
	int AfterGb_fin;
	int AfterB_fin;
	float Afterdistance;
	int m_SlaveID;
	int R_StartAdd;
	int Gr_StartAdd;
	int Gb_StartAdd;
	int B_StartAdd;
	int RG_StartAdd;
	int BG_StartAdd;
	int GG_StartAdd;
	int ByteCount;
	int A_Multi;
	int B_Multi;
	CString CurrentName;
	CString GlobalSetting;
	BOOL flag;
	int R_index;
	int Gr_index;
	int Gb_index;
	int B_index;
	int RG_index;
	int BG_index;
	int GG_index;
	int GAverFlag;//1:G_Aver 2:Gr_Aver 3:Gb_Aver
	USHORT Typical_RG;
	USHORT Typical_BG;
	USHORT Typical_GG;

	int m_MotoLightSrcMode;

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
	int BeforeAWB();
	int AfterAWB();
	void CenterBlockRawInfo(ImageInfo *mySensor ,int Ratio,WBInf &MyWbInf,int index,int BLC);
	BOOL GetAWBData(ImageInfo img,WBInf &MyWbInf);
};