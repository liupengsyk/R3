#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 

#define MAXGODLENNUM    20
class GetDNPIndex_TestItem :public TestItemInterface
{
public:
	enum AUTOLIGHTSTATUS    //与文件对应状态一致，用于自动校验
	{
		AUTOLIGHT_INIT = 0,  //初始值,0
		AUTOLIGHT_GOLDEN_SUCESS,     //获取GOLDEN值成功 T0100
		AUTOLIGHT_GOLDEN_FAIL_CAPTURE,    //获取色坐标失败 T0101
		AUTOLIGHT_GOLDEN_FAIL,    //获取GOLDEN值失败 T0102

		AUTOLIGHT_SAVECOEFFEE_SUCESS,    //导入成功  T0200
		AUTOLIGHT_SAVECOEFFEE_FAIL_AVAGEGODEN,    //获取Golden平均值失败 T0201
		AUTOLIGHT_SAVECOEFFEE_FAIL,    //导入失败 T0202
	};


	GetDNPIndex_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~GetDNPIndex_TestItem(void);

	int InitItem();

	int Testing();       //×ÓÀàÖØÔØ²âÊÔ´úÂë·Å·ÅÔÚ´Ëº¯Êý
	int Initialize();      //×ÓÀàÖØÔØ ³õÊ¼»¯´úÂë
	int Finalize();      //×ÓÀàÖØÔØ ³õÊ¼»¯

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
	int imageflag;

public:

	 int GlobalTime2;//多次色温矫正，因测试项而已
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
	 CString GlobalSetting;

	 struct WBInf
	 {
		 USHORT OTP_R;
		 USHORT OTP_Gr;
		 USHORT OTP_Gb;
		 USHORT OTP_B;
		 USHORT OTP_RG;
		 USHORT OTP_BG;
		 USHORT OTP_GG;
		 USHORT R;
		 USHORT Gr;
		 USHORT Gb;
		 USHORT B;
		 USHORT RG;
		 USHORT BG;
		 USHORT GG;
		 USHORT R_index;
		 USHORT Gr_index;
		 USHORT Gb_index;
		 USHORT B_index;
		 USHORT RG_index;
		 USHORT BG_index;
		 USHORT GG_index;
	 };
     CString MyfuseID[MAXGODLENNUM];    //pDlg->m_ModuleNum数组越界
	 WBInf MyWBInf[MAXGODLENNUM+1];     //第四次为平均值
	 void GetOTPData(WBInf &MyWBInf);
	 void CenterBlockRawInfo(ImageInfo *mySensor ,int Ratio,WBInf &MyWbInf,int index,int BLC);
	 BOOL GetAWBData(ImageInfo img,WBInf &MyWbInf);
	 int GAverFlag;//1:G_Aver 2:Gr_Aver 3:Gb_Aver

	 BOOL LEDInitFlag;
	 int GlobalTime;//与光源矫正联用
	 CString CurrentName;//与光源矫正联用
	// BOOL SetLEDFlag;

	 /////////////////////////////////////////////////////////////////////
	 //用于设置自动校准参数， nsel 范围如下
	 //0，初始值； 
	 //1, 获取GOLDEN值成功 T0100
     //2 获取色坐标失败 T0101
	//3 获取GOLDEN值失败 T0102
	//4 导入成功  T0200
	//5 获取Golden平均值失败 T0201
	//6 导入失败 T0202
	////////////////////////////////////////////////////////////////
	void SetAutoLightStatus(int nsel); 

};