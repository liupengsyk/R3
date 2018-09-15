#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 

struct DParticalInfo
{
	USHORT iXIndex;
	USHORT iYIndex;
	USHORT iChannelXIndex;
	USHORT iChannelYIndex;
	USHORT iDNValue;
};

enum SENSOR_TYPE
{
	UNIVERSAL_SENSOR = 0,
	OV12A10 = 1,
};

enum SLAVE_ID
{
	OV12A10_SLAVED_ID = 0x6C,
	HI846_SLAVED_ID = 0x40,
};

class HwDarkPartical_TestItem :public TestItemInterface
{
public:
	HwDarkPartical_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~HwDarkPartical_TestItem(void);

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

	BOOL GetCluster(DParticalInfo* pInfos,int iStartIndex,int iparticalNum);
	BOOL GetDPartical(USHORT* pChannel, int iImgW, int iImgH, int avg,int& particalnum, float fSpec, int iDNDiff,DParticalInfo* pInfos);
	BOOL GetChannelBuffer(USHORT* pRawImage,USHORT*pChannelImage,USHORT nImgW,USHORT nImgH,int nPixOrder,int iChannel,long long &sum);
	BOOL DParticalTest(USHORT* pImg,int iOutformat, int iImgW,int iImgH,float fSpec,int iDNDiff);
	void GetBadPixelPosition(DParticalInfo* pStack,int iStart, int iCount, int nPixOrder, int iChannel);
	void SaveData(DParticalInfo* pInfo,int iCount,BOOL bResult);
private:
	OptionDlg* pDlg;
	int CamID;
	CString CamCode;
	BOOL m_bResult;
	float m_fQuantitySpec;	//界面设置的QuantitySpec
	int m_nDnDiff;			//界面设置的DnDff
	USHORT m_PrimaryGain;
	int m_PrimaryExposure;
	USHORT m_AnalogGain;
	int m_InitExposure;
	USHORT m_FrameCount;
	USHORT m_GainSettingTime;
	int m_DParticalResult;
public:
	
	USHORT m_PrimaryReg[4];
};