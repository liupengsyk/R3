#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
 
#include "OTPCheck.h"
#pragma comment(lib, "OTPCheck.lib")


#define E2STARTADDR     0x0000

/** Module ID Addr **/
#define OTP_CHK_MODULE_ID_ADDR_START								0x0000
#define OTP_CHK_MODULE_ID_ADDR_END									0x0024

#define OTP_CHK_MODULE_ID_CHKSUM_ADDR_START							0x0025
#define OTP_CHK_MODULE_ID_CHKSUM_ADDR_END							0x0026

#define OTP_MODULE_LEN												(OTP_CHK_MODULE_ID_CHKSUM_ADDR_END - OTP_CHK_MODULE_ID_ADDR_START + 1)

/** AWB Addr **/
#define OTP_CHK_AWB_ADDR_START										0x0031
#define OTP_CHK_AWB_ADDR_END										0x0052

#define OTP_CHK_AWB_CHKSUM_ADDR_START								0x0053
#define OTP_CHK_AWB_CHKSUM_ADDR_END									0x0054

#define OTP_AWB_LEN													(OTP_CHK_AWB_CHKSUM_ADDR_END - OTP_CHK_AWB_ADDR_START + 1)

/** OC Addr **/
#define OTP_CHK_OC_ADDR_START										0x0055
#define OTP_CHK_OC_ADDR_END											0x0064

#define OTP_CHK_OC_CHKSUM_ADDR_START								0x0065
#define OTP_CHK_OC_CHKSUM_ADDR_END									0x0066

#define OTP_OC_LEN													(OTP_CHK_OC_CHKSUM_ADDR_END - OTP_CHK_OC_ADDR_START + 1)

/** SFR Addr **/
#define OTP_CHK_SFR_ADDR_START										0x0067
#define OTP_CHK_SFR_ADDR_END										0x00C3

#define OTP_CHK_SFR_CHKSUM_ADDR_START								0x00C4
#define OTP_CHK_SFR_CHKSUM_ADDR_END									0x00C5

/** LSC Addr **/
#define OTP_CHK_LSC_ADDR_START										0x08D4
#define OTP_CHK_LSC_ADDR_END										0x0FBB

#define OTP_CHK_LSC_CHKSUM_ADDR_START								0x0FBC
#define OTP_CHK_LSC_CHKSUM_ADDR_END									0x0FBD

#define OTP_LSC_LEN													(OTP_CHK_LSC_CHKSUM_ADDR_END - OTP_CHK_LSC_ADDR_START + 1)

// CrossTalk
#define OTP_CHK_CTalk_ADDR_START									0x13A5
#define OTP_CHK_CTalk_ADDR_END										0x17C4

#define OTP_CHK_CTalk_CHKSUM_ADDR_START								0x17C5
#define OTP_CHK_CTalk_CHKSUM_ADDR_END								0x17C6

#define OTP_CTalk_LEN												(OTP_CHK_CTalk_CHKSUM_ADDR_END - OTP_CHK_CTalk_ADDR_START + 1)

class L476F00_OTP_TestItem :public TestItemInterface
{
public:
	L476F00_OTP_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~L476F00_OTP_TestItem(void);

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

	unsigned char GetBCD(unsigned char ascii);

private:
	OptionDlg* pDlg;
	int CamID;
	CString CamCode;

public:
	void Dump();
	int GetCRC(USHORT *Data, int Len);
};