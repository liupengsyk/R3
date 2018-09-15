#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"

typedef struct  DNPinfo
{
	CString FuseId;
#if 1//LIGHT_SRC_CHECK_MODE == LIGHT_SRC_CHECK_MODE_DYNAMIC
	int Error;
#endif
	int month;
	int day;
	int hour;
	int min;
	int sec;
	int RGindex;
	int BGindex;
	int GGindex;
	/*int m_Rindex_5100K;
	int m_Grindex_5100K;
	int m_Gbindex_5100K;
	int m_Bindex_5100K;
	int m_Rindex_3000K;
	int m_Grindex_3000K;
	int m_Gbindex_3000K;
	int m_Bindex_3000K;*/

	int R_ave;
	int Gr_ave;
	int Gb_ave;
	int B_ave;
	int RG;
	int BG;
	int GG;
	
	float ErrorInfo;
}DNPINFO;

class GetDNPIndex_Moto_TestItem :public TestItemInterface
{
public:
	GetDNPIndex_Moto_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~GetDNPIndex_Moto_TestItem(void);

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

	int GlobalTime2;//多次色温矫正，因测试项而已---本次光源校准过程中已经校准了的Golden模组数--Addedd by LiHai--20171221
	int m_SlaveID;
	int R_StartAdd;
	int Gr_StartAdd;
	int Gb_StartAdd;
	int B_StartAdd;
	int RG_StartAdd;
	int BG_StartAdd;
	int GG_StartAdd;
	int ByteCount;//表示存入EEPROM中的AWB数据是一字节数据还是两字节数据，根据经验一般都是两字节数据，因为大多是计算RGain、BGain、GGain值，这三个值的计算公式会乘以一个系数K，这个系数K一般好大，至少都要512了
	int A_Multi;//上面ByteCount为两字节时，那么最后的结果是需要两字节合并成一个WORD的，这个表示第一个字节需要乘以的系数，下面的B_Multi表示第二个字节需要乘以的系数。就是说哪个在高字节，那么对应的系数就是256，低字节的就是1。
	int B_Multi;
	CString GlobalSetting;
	struct WBInf
	{
		//以下七个是模组内部存储的OTP值，由于光源校准过程用的是Golden模组，所以这七个值其实也是Golden值--Added by LiHai--20171211
		USHORT OTP_R;
		USHORT OTP_Gr;
		USHORT OTP_Gb;
		USHORT OTP_B;
		USHORT OTP_RG;
		USHORT OTP_BG;
		USHORT OTP_GG;

		//以下七个是模组本次测试的WB值，由于光源校准过程用的是Golden模组，所以这七个值其实也是Golden模组本次测试的WB值--Added by LiHai--20171211
		USHORT R;
		USHORT Gr;
		USHORT Gb;
		USHORT B;
		USHORT RG;
		USHORT BG;
		USHORT GG;

		//以下七个是模组本次测试的WB值和模组内部存储的OTP对应的WB值的比值，保留到小数点后4位，即结果乘以10000，其实也就是Golden模组本次测试的值和挑选Golden时的值之间的比值--Added by LiHai--20171211
		USHORT R_index;
		USHORT Gr_index;
		USHORT Gb_index;
		USHORT B_index;
		USHORT RG_index;
		USHORT BG_index;
		USHORT GG_index;
	};
	CString MyfuseID[MAXGODLENNUM];
	WBInf MyWBInf[MAXGODLENNUM+1];//第四次为平均值
	void GetOTPData(WBInf &MyWBInf);
	void CenterBlockRawInfo(ImageInfo *mySensor ,int Ratio,WBInf &MyWbInf,int index,int BLC);
	BOOL GetAWBData(ImageInfo img,WBInf &MyWbInf);
	BOOL GetLightSource(void);

	int GAverFlag;//1:G_Aver 2:Gr_Aver 3:Gb_Aver--光源校准DNP项的配置也是调用的AWB项的设置，所以此处表示曝光后使用的是G的平均值、Gr的平均值、Gb的平均值？
	BOOL LEDInitFlag;
	int GlobalTime;//与光源矫正联用
	CString CurrentName;//与光源矫正联用
	BOOL SetLEDFlag;

	/** 联想的光源点检数据 **/
	int m_HighCCT_Ev;
	long m_HighCCT_u;
	long m_HighCCT_v;
	_DNP_COLOR_VAL_ m_CurVal;	//Current Module Val
	//_DNP_COLOR_VAL_ m_TypVal;	//Typical Module Val
	_DNP_COLOR_VAL_ m_CurSum;	//多颗Golden模组校准的当前值的和

	BOOL SaveMotoDynamic_DNPData(void);
	int MotoDynamicLightSrcCalibration(void);
	int MotoStaticLightSrcCalibration(void);

	DNPINFO DnpInfo[50];

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